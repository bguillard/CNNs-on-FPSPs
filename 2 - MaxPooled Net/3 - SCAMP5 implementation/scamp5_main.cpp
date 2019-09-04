/*
 * Scamp5d M0 Program
 * This program runs a 2 layer CNN, using maxpooling
 */

/*
 * 2-layer AnalogNet, using maxpooling: 15 fps network
 * Author: Benoit Guillard, MSc Thesis, Imperial College London
 */

#include <scamp5.hpp>
#include <vs_import.hpp>
#include "fc_weights.hpp"
#include "kernel_instructions.hpp"

using namespace SCAMP5_PE;


/*
 * Define functions used for the fully connected layers' computation
 */

// Given SCAN_SIZE coordinates,
// group them into 4*4=16 bins, filling the count array
// -> 4 feature maps, 4 bins per feature maps (each corner)
template <int SCAN_SIZE>
inline void sum_pooling_events(uint8_t coordinates[2*SCAN_SIZE], int count[16]){
	uint8_t x,y;
	for(int i = 0; i < 16; i++){
		count[i] = 0;
	}
	for(int i = 0; i < SCAN_SIZE; i++)
	{
		x = coordinates[2*i];
		y = coordinates[2*i+1];
		if(x == 0 && y == 0)
			return;
		if(x >= 114 && x <= 141 && y >= 114 && y <= 141){
			x = x-114;
			y = y-114;

			if(x <= 13){
				if(y <= 13){
					count[(x%2) + 2*(y%2)] ++;
				} else {
					count[4 + (x%2) + 2*(y%2)] ++;
				}

			} else {
				if(y <= 13){
					count[8 + (x%2) + 2*(y%2)] ++;
				} else {
					count[12 + (x%2) + 2*(y%2)] ++;
				}
			}
		}
	}
}


// Compute a fully connected layer's output
// Use the above global constant weights,
// and modify the result array in place
template <int IN_SIZE, int OUT_SIZE>
inline void fc_computation(const int weights[IN_SIZE][OUT_SIZE], const int biases[OUT_SIZE],
							int in[IN_SIZE], int out[OUT_SIZE]){
	long int partialSum;
	for (int j=0; j<OUT_SIZE; j++){
		partialSum = 0;
		for (int i=0; i<IN_SIZE; i++){
			partialSum += in[i]*weights[i][j];
		}
		out[j] = partialSum + biases[j];
	}
	return;
}


// Apply ReLU to a fixed size array
template <int SIZE>
inline void relu(int vec[SIZE]){
	for (int i=0; i< SIZE; i++){
		if (vec[i] < 0){
			vec[i] = 0;
		}
	}
	return;
}


/*
 * Define SCAMP5 utils kernels that are used during the convolutional
 * part of the network.
 * !!!!!!!!!!!!!!!!!!!!!!!!!
 * IMPORTANT NOTE: the kernels are defined as macros! Doing so allows
 * us to use them with different arguments.
 * This is because once compiled, 'the instructions
 * will remain same even if there are variables inside it that are
 * changing upon different execution.'
 */

// Adds a constant value to whole source AREG.
// Can be used to add bias to one whole register
// Uses aux AREG F. Modifies source AREG.
#define add_const(source, value)({\
	scamp5_in(F, value);\
	scamp5_kernel_begin();\
		add(source, source, F);\
	scamp5_kernel_end();\
})



// To launch one dynamically created scamp5 kernel
#define dynamic_kernel(id)	{	scamp5_launch_kernel(array_kernels[id]);}

int main(){
    // Initialization
    vs_init();
    //vs_stopwatch stopwatch;

    /////////////////
    // Setup Host GUI
    /////////////////
    // DISPLAYS:
    // First convolution result and binarised version
    auto display_a = vs_gui_add_display("Register A",0,0);
    auto display_d = vs_gui_add_display("R8",0,1);
    // Second convolution result and binarised version
    auto display_b = vs_gui_add_display("Register B",1,0);
    auto display_e = vs_gui_add_display("R9",1,1);
    // Third convolution result and binarised version
    auto display_c = vs_gui_add_display("Register C",2,0);
    auto display_f = vs_gui_add_display("R10",2,1);
    // Input image
    auto original_input = vs_gui_add_display("Original Input",2,2);
    // Binarised input image
    auto display_6 = vs_gui_add_display("R6",1,2);
    // Cropped (28*28 square) binarised input image
    auto display_7 = vs_gui_add_display("R7",0,2);

    // SLIDERS:
    // Input binarisation threshold
    auto threshold = vs_gui_add_slider("Input thresh: ",10,127,50);
    uint8_t threshold_value;
    // Output thresholds for each of the 8 outputted feature maps
    auto t1 = vs_gui_add_slider("Thresh conv2_1: ",-10,200,conv_2_1_bias);
    auto t2 = vs_gui_add_slider("Thresh conv2_2: ",-10,200,conv_2_2_bias);
    auto t3 = vs_gui_add_slider("Thresh conv2_3: ",-10,200,conv_2_3_bias);
    auto t4 = vs_gui_add_slider("Thresh conv2_4: ",-10,200,conv_2_4_bias);
    auto t5 = vs_gui_add_slider("Thresh conv2_5: ",-10,200,conv_2_5_bias);
    auto t6 = vs_gui_add_slider("Thresh conv2_6: ",-10,200,conv_2_6_bias);
    auto t7 = vs_gui_add_slider("Thresh conv2_7: ",-10,200,conv_2_7_bias);
    auto t8 = vs_gui_add_slider("Thresh conv2_8: ",-20,200,-conv_2_8_bias);
    auto recording = vs_gui_add_slider("Recording: ",0,1,0);
    int recording_value;
    auto output_videos = vs_gui_add_slider("Output videos: ",0,1,0);
    int output_videos_value;

    // VAR DECLARATIONS
    // To read the events (=coordinates of 1s)
    uint8_t coordinates[200];
    // Number of events in the input frame
    uint8_t input_count;
    // Number of consecutively empty input frames
    int empty_input_frames = 0;
    // Flattened output of the convolution layers
    int conv_outputs[32];
    // First fully connected layer output
    int fc1_result[50];
    // Second fully connected layer output
    int fc2_result[10];
    // Output label (index of max. in results)
    uint8_t max_index;

    vs_on_host_connect([&](){
        vs_post_text("\n2-layer, maxpooled, segmented, AnalogNet architecture\n");
        vs_led_on(VS_LED_2);
    });

    vs_on_host_disconnect([&](){
        vs_led_off(VS_LED_2);
    });

    // Set framerate to 12 fps,
    // with constant loop time
    vs_configure_frame_trigger(VS_FRAME_TRIGGER_CONST_LOOP, 12);

    // Frame Loop
    while(1){
        vs_process_message();

        // Get GUI values at start of frame
        threshold_value = vs_gui_read_slider(threshold);
        t1_value = vs_gui_read_slider(t1);
        t2_value = vs_gui_read_slider(t2);
        t3_value = vs_gui_read_slider(t3);
        t4_value = vs_gui_read_slider(t4);
        t5_value = vs_gui_read_slider(t5);
        t6_value = vs_gui_read_slider(t6);
        t7_value = vs_gui_read_slider(t7);
        t8_value = vs_gui_read_slider(t8);
        recording_value = vs_gui_read_slider(recording);
        output_videos_value = vs_gui_read_slider(output_videos);

        //vs_post_text("%lu\n", stopwatch.get_usec());
        //stopwatch.reset();

        /*
         * Begin Input
         */
        // Perform synchronized acquisition (constant FPS):
        vs_wait_frame_trigger();
        scamp5_get_image(A, B, 1);
        if (output_videos_value)
        	scamp5_output_image(A, original_input);

		// Load the first kernels, accessible by their id *conv name*_id
        // in the array conv_kernels
        // WARNING: When using such dynamically defined kernels, one should
        // NOT use any static kernel (otherwise it would NOT execute as intended).
		fill_kernels_part_1();

        // Reverse binarisation
        // (on display: black digit on white background)
        // Binarised input image is stored in R6
        scamp5_in(F, threshold_value);
        dynamic_kernel(binarise_input_id);
        // Keep only central 28*28 square: R6->R7
        dynamic_kernel(clear_R2_id);
    	scamp5_draw_begin(R2);
    		scamp5_draw_rect(114, 114, 141, 141);
    	scamp5_draw_end();
    	dynamic_kernel(central_mask_id);

        // Display results of binarization:
        // binarised + binarised and cropped
        if (output_videos_value){
        	scamp5_output_image(R6,display_6);
        	scamp5_output_image(R7,display_7);
        }

        /*
         * CONVOLUTIONS
         */
		// Load masking DREGs: we consider a register to be a grid
		// of 2*2 squares, with the following pattern:
		// 						1 2
		// 						3 4
		// Rx has the PEs marked x as 1s, others as 0s. (ex: R1 has
		// positions 1 as 1s, positions 2, 3 and 4 as 0s).
		// It is needed to reload them at each loop, since they are decaying.
        // Some instructions overwrite the content of these registers, such
        // as drawing rectangles, or counting events. Check consistency and
        // availability with:
        /*
		scamp5_output_image(R1,display_d);
		scamp5_output_image(R2,display_e);
		scamp5_output_image(R3,display_f);
		scamp5_output_image(R4,display_c);
        */
		scamp5_load_pattern(R1, 0, 0, 254, 254);
		scamp5_load_pattern(R2, 0, 1, 254, 254);
		scamp5_load_pattern(R3, 1, 0, 254, 254);
		scamp5_load_pattern(R4, 1, 1, 254, 254);

		// FIRST LAYER, filters applied in A/B, and
        // max pooled + ReLU-ed results stored in register A
		scamp5_in(F, 120);
        dynamic_kernel(A_from_R7_id);
        dynamic_kernel(conv_1_1_id);
        dynamic_kernel(maxpooling_pos_1_A_id);

        scamp5_in(F, 120);
        dynamic_kernel(B_from_R7_id);
        dynamic_kernel(conv_1_2_id);
        dynamic_kernel(maxpooling_pos_2_B_id);
        dynamic_kernel(partial_copy_A_B_R2_id);

        scamp5_in(F, 120);
        dynamic_kernel(B_from_R7_id);
        dynamic_kernel(conv_1_3_id);
        dynamic_kernel(maxpooling_pos_3_B_id);
        dynamic_kernel(partial_copy_A_B_R3_id);

        scamp5_in(F, 120);
        dynamic_kernel(B_from_R7_id);
        dynamic_kernel(conv_1_4_id);
        //add_const(B, conv1_4_bias);	// add bias (other conv1 filters have bias=0)
        dynamic_kernel(maxpooling_pos_4_B_id);
        dynamic_kernel(partial_copy_A_B_R4_id);

        scamp5_in(F, 0);
        dynamic_kernel(conv_relu_A_id);

        //////////////////////////
        // SECOND LAYER: conv_2_1 to conv_2_4
		//////////////////////////

        // conv_2_1: each of the 4 kernels in a different position in C,
        // then copied in B before being summed
        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_1_1_id);
        dynamic_kernel(partial_copy_B_C_R1_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_1_2_id);
        dynamic_kernel(partial_copy_B_C_R2_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_1_3_id);
        dynamic_kernel(partial_copy_B_C_R3_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_1_4_id);
        dynamic_kernel(partial_copy_B_C_R4_id);

        // Sum each of the 4 kernels composing the first (out of 8) feature map
        dynamic_kernel(sum_conv_2_1_id);

        // Load the kernels for the second part of the convolutions
        delete[] array_kernels;
        fill_kernels_part_2();


        // conv2_2: each of the 4 kernels in a different position in C,
        // then moved to the second position (R2), before being added to B
        dynamic_kernel(res_B_R2_id);
        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_2_1_id);
        dynamic_kernel(shift_C_R1_R2_id);
        dynamic_kernel(partial_sum_B_C_R2_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_2_2_id);
		// R2->R2, no shift
		dynamic_kernel(partial_sum_B_C_R2_id);

		dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_2_3_id);
        // Shift R3->R2:
        dynamic_kernel(shift_C_R3_R2_id);
        dynamic_kernel(partial_sum_B_C_R2_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_2_4_id);
        // Shift R4->R2:
        dynamic_kernel(shift_C_R4_R2_id);
        dynamic_kernel(partial_sum_B_C_R2_id);


        // conv2_3: each of the 4 kernels in a different position in C,
		// then moved to the third position (R3), before being added to B
        dynamic_kernel(res_B_R3_id);
        dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_3_1_id);
		// Shift R1->R3:
		dynamic_kernel(shift_C_R1_R3_id);
		dynamic_kernel(partial_sum_B_C_R3_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_3_2_id);
		// Shift R2->R3:
		dynamic_kernel(shift_C_R2_R3_id);
		dynamic_kernel(partial_sum_B_C_R3_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_3_3_id);
		// Shift R3->R3, no shift
		dynamic_kernel(partial_sum_B_C_R3_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_3_4_id);
		// Shift R4->R3:
		dynamic_kernel(shift_C_R4_R3_id);
		dynamic_kernel(partial_sum_B_C_R3_id);


		// conv2_4: each of the 4 kernels in a different position in C,
		// then moved to the fourth position (R4), before being added to B
		dynamic_kernel(res_B_R4_id);
		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_4_1_id);
		// Shift R1->R4:
		dynamic_kernel(shift_C_R1_R4_id);
		dynamic_kernel(partial_sum_B_C_R4_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_4_2_id);
		// Shift R2->R4:
		dynamic_kernel(shift_C_R2_R4_id);
		dynamic_kernel(partial_sum_B_C_R4_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_4_3_id);
		// Shift R3->R4:
		dynamic_kernel(shift_C_R3_R4_id);
		dynamic_kernel(partial_sum_B_C_R4_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_4_4_id);
		// Shift R4->R4, no shift
		dynamic_kernel(partial_sum_B_C_R4_id);

        //////////////////////////
        // Output Thresholding
		//////////////////////////

		// AREG B->R8, with 4 different threshold values,
		// 1 for each position (=feature map)
		// Load the array of thresholds in F
		scamp5_in(E, t1_value);
		dynamic_kernel(partial_copy_F_E_R1_id);
		scamp5_in(E, t2_value);
		dynamic_kernel(partial_copy_F_E_R2_id);
		scamp5_in(E, t3_value);
		dynamic_kernel(partial_copy_F_E_R3_id);
		scamp5_in(E, t4_value);
		dynamic_kernel(partial_copy_F_E_R4_id);
		// Threshold B to R8 according to above values
		dynamic_kernel(out_thresh_R8_B_conv21_to_conv24_id);

		// Load the kernels for the third part of the convolutions
		delete[] array_kernels;
		fill_kernels_part_3();

		//////////////////////////
        // SECOND LAYER: conv_2_5 to conv_2_8
		//////////////////////////
		dynamic_kernel(res_B_id);

        // conv_2_5: each of the 4 kernels in a different position in C,
        // then copied in B before being summed
        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_5_1_id);
        dynamic_kernel(partial_copy_B_C_R1_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_5_2_id);
        dynamic_kernel(partial_copy_B_C_R2_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_5_3_id);
        dynamic_kernel(partial_copy_B_C_R3_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_5_4_id);
        dynamic_kernel(partial_copy_B_C_R4_id);

        // Sum each of the 4 kernels composing the first (out of 8) feature map
        dynamic_kernel(sum_conv_2_5_id);

        // conv2_6: each of the 4 kernels in a different position in C,
        // then moved to the second position (R2), before being added to B
        dynamic_kernel(res_B_R2_id);
        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_6_1_id);
        dynamic_kernel(shift_C_R1_R2_id);
        dynamic_kernel(partial_sum_B_C_R2_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_6_2_id);
		// R2->R2, no shift
		dynamic_kernel(partial_sum_B_C_R2_id);

		dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_6_3_id);
        // Shift R3->R2:
        dynamic_kernel(shift_C_R3_R2_id);
        dynamic_kernel(partial_sum_B_C_R2_id);

        dynamic_kernel(copy_areg_C_A_id);
        dynamic_kernel(conv_2_6_4_id);
        // Shift R4->R2:
        dynamic_kernel(shift_C_R4_R2_id);
        dynamic_kernel(partial_sum_B_C_R2_id);


		// Load the kernels for the fourth part of the convolutions
		delete[] array_kernels;
		fill_kernels_part_4();

        // conv2_7: each of the 4 kernels in a different position in C,
		// then moved to the third position (R3), before being added to B
        dynamic_kernel(res_B_R3_id);
        dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_7_1_id);
		// Shift R1->R3:
		dynamic_kernel(shift_C_R1_R3_id);
		dynamic_kernel(partial_sum_B_C_R3_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_7_2_id);
		// Shift R2->R3:
		dynamic_kernel(shift_C_R2_R3_id);
		dynamic_kernel(partial_sum_B_C_R3_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_7_3_id);
		// Shift R3->R3, no shift
		dynamic_kernel(partial_sum_B_C_R3_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_7_4_id);
		// Shift R4->R3:
		dynamic_kernel(shift_C_R4_R3_id);
		dynamic_kernel(partial_sum_B_C_R3_id);


		// conv2_8: each of the 4 kernels in a different position in C,
		// then moved to the fourth position (R4), before being added to B
		dynamic_kernel(res_B_R4_id);
		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_8_1_id);
		// Shift R1->R4:
		dynamic_kernel(shift_C_R1_R4_id);
		dynamic_kernel(partial_sum_B_C_R4_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_8_2_id);
		// Shift R2->R4:
		dynamic_kernel(shift_C_R2_R4_id);
		dynamic_kernel(partial_sum_B_C_R4_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_8_3_id);
		// Shift R3->R4:
		dynamic_kernel(shift_C_R3_R4_id);
		dynamic_kernel(partial_sum_B_C_R4_id);

		dynamic_kernel(copy_areg_C_A_id);
		dynamic_kernel(conv_2_8_4_id);
		// Shift R4->R4, no shift
		dynamic_kernel(partial_sum_B_C_R4_id);


		//////////////////////////
        // Output Thresholding
		//////////////////////////
		// AREG B->R9, with 4 different threshold values,
		// 1 for each position (=feature map)
		// Load the array of thresholds in F
		scamp5_in(E, t5_value);
		dynamic_kernel(partial_copy_F_E_R1_id);
		scamp5_in(E, t6_value);
		dynamic_kernel(partial_copy_F_E_R2_id);
		scamp5_in(E, t7_value);
		dynamic_kernel(partial_copy_F_E_R3_id);
		scamp5_in(E, t8_value);
		dynamic_kernel(partial_copy_F_E_R4_id);
		// Threshold B to R9 according to above values
		dynamic_kernel(out_thresh_R9_B_conv25_to_conv28_id);

		delete[] array_kernels;

		///////////////////////////////////////////////////
		//Processing from here is on microcontroller
		///////////////////////////////////////////////////

		/*
		 * COUNT 1s in Convolution Filter Results
		 */
		// Process R8, conv_2_1 to conv_2_4
		scamp5_scan_events(R8, coordinates, 125);
		sum_pooling_events<125>(coordinates, &conv_outputs[0]);

		// Process R9, conv_2_5 to conv_2_8
		scamp5_scan_events(R9, coordinates, 150);
		sum_pooling_events<150>(coordinates, &conv_outputs[16]);

		if (recording_value){
			// If recording the output of the conv layers,
			// count the events in the binarised input. If no event is found,
			// a white screen was shown to the camera. This is used
			// to synchronise the acquisition process with the
			// display process.
			scamp5_scan_events(R7, coordinates, 10);
			input_count = 0;
			for(int i = 0; i < 10; i++){
				if (coordinates[2*i] != 0 || coordinates[2*i+1] != 0)
					input_count ++;
			}
			if(input_count <= 3){
				empty_input_frames ++;
				// After 300 empty frames (25 s at 60 fps),
				// save and clear current output
				if(empty_input_frames == 300){
					vs_post_text("!save");
					vs_post_text("!clear");
				}
				// After 720 empty frames (1 min at 60 fps),
				// forever loop (ie stop main processing loop)
				if(empty_input_frames > 720){
					while(true){
					}
				}
			} else {
				empty_input_frames = 0;
			}

			// Structure of one output line:
			// an array of 33 elements: the first one is the count
			// of 1s in the input binarised image, the 32 others
			// is the output of the conv layer
			vs_post_text("[%i, ", input_count);
			for(int i=0; i<31; i++)
				vs_post_text("%i, ", conv_outputs[i]);
			vs_post_text("%i]\n", conv_outputs[31]);
		}
		/*
		 * Dense Layer Computation
		 */
		// Compute neuron results
		fc_1(conv_outputs, fc1_result);
		relu<50>(fc1_result);
		fc_2(fc1_result, fc2_result);

		// Find max index (=label) in results
		max_index = 0;
		for (int i=1; i<10; i++){
			if (fc2_result[i] > fc2_result[max_index])
				max_index = i;
		}

		if(!recording_value){
			// Output value of each neuron
			/*
			for (int i=0; i<10; i++){
				  vs_post_text("%i,", fc2_result[i]);
			}
			vs_post_text("\n");
			*/
			// Output value of max index (=output label)
			vs_post_text("%i,\n\n", max_index);
		}

		// Output images to GUI
		if (output_videos_value){
			scamp5_output_image(A,display_a);
			scamp5_output_image(B,display_b);
			scamp5_output_image(C,display_c);
			scamp5_output_image(R8,display_d);
			scamp5_output_image(R9,display_e);
			scamp5_output_image(R10,display_f);
		}

        // increase loop_counter by 1
        vs_loop_counter_inc();
    }
    return 0;
}
