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
#include "conv_instructions.hpp"
#include "fc_weights.hpp"

using namespace SCAMP5_PE;
// In our case, bias is very small compared to noise
// (typical values between 0 and 4). For this reason,
// we try without adding bias
#define NEGLECT_BIAS true

/*
 * Define functions used for the fully connected layers' computation
 */

// Given SCAN_SIZE coordinates,
// group them into 12 bins, filling the count array
template <int SCAN_SIZE>
inline void sum_pooling_events(uint8_t coordinates[2*SCAN_SIZE], int count[12]){
	uint8_t x,y;
	for(int i = 0; i < 12; i++){
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

			if(x >= 5 && x <= 13 && y >= 0 && y <= 8)
				count[0] ++;
			if(x >= 14 && x <= 22 && y >= 0 && y <= 8)
				count[1] ++;

			if(x >= 0 && x <= 8 && y >= 5 && y <= 13)
				count[2] ++;
			if(x >= 5 && x <= 13 && y >= 5 && y <= 13)
				count[3] ++;
			if(x >= 14 && x <= 22 && y >= 5 && y <= 13)
				count[4] ++;
			if(x >= 19 && x <= 27 && y >= 5 && y <= 13)
				count[5] ++;

			if(x >= 0 && x <= 8 && y >= 14 && y <= 22)
				count[6] ++;
			if(x >= 5 && x <= 13 && y >= 14 && y <= 22)
				count[7] ++;
			if(x >= 14 && x <= 22 && y >= 14 && y <= 22)
				count[8] ++;
			if(x >= 19 && x <= 27 && y >= 14 && y <= 22)
				count[9] ++;

			if(x >= 5 && x <= 13 && y >= 19 && y <= 27)
				count[10] ++;
			if(x >= 14 && x <= 22 && y >= 19 && y <= 27)
				count[11] ++;
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

// Perform Binarisation Procedure on the content of the AREG source.
// All values inferior to -1*thresh are set to 1 in the target DREG,
// others are set to 0.
// Uses auxilliary AREG F.
#define binarise_input(target, source, threshold_value)({\
	scamp5_in(F, threshold_value);\
	scamp5_kernel_begin();\
		add(source, source, F);\
		CLR(target);\
		where(source);\
			MOV(target, FLAG);\
		ALL();\
		NOT(target);\
	scamp5_kernel_end();\
})


// Only preserve data in central 28 x 28 square of source (DREG),
// copy its content to target (DREG) (everything else marked as 0),
// Uses auxilliary DREG R2.
#define central_mask(target, source)({\
	/* Draw central square */\
	scamp5_kernel_begin();\
		CLR(R2);\
	scamp5_kernel_end();\
	scamp5_draw_begin(R2);\
		scamp5_draw_rect(114, 114, 141, 141);\
	scamp5_draw_end();\
	/* Mask everything that is outside this central square */\
	scamp5_kernel_begin();\
		CLR(target);\
		AND(target, source, R2);\
		CLR(R2);\
	scamp5_kernel_end();\
})


// Convert binary image from source DREG
// into analog image with uniform analog value (120)
// in target AREG. Uses auxilliary AREG F.
#define dreg_to_areg(target, source)({\
	scamp5_in(F, 84);\
	scamp5_kernel_begin();\
		res(target);\
		WHERE(source);\
			mov(target, F);\
		ALL();\
	scamp5_kernel_end();\
})


// Output thresholding: from source AREG, bias is subtracted,
// and all locations >= value
// are set to 1 in target DREG. Others are set to 0.
// Uses auxilliary AREG E and F.
#define out_thresholding(target, source, value)({\
	scamp5_in(F, value);\
	scamp5_kernel_begin();\
		CLR(target);\
		sub(E, source, F);\
		where(E);\
			MOV(target, FLAG);\
		ALL();\
	scamp5_kernel_end();\
})


// Adds a constant value to whole source AREG.
// Can be used to add bias to one whole register
// Uses aux AREG F. Modifies source AREG.
#define add_const(source, value)({\
	scamp5_in(F, value);\
	scamp5_kernel_begin();\
		add(source, source, F);\
	scamp5_kernel_end();\
})


// Applies ReLU on whole source AREG,
// Uses aux AREGs E and F.
#define conv_relu(source)({\
	scamp5_in(F, 0);\
	scamp5_kernel_begin();\
		neg(E, source);\
		where(E);\
			mov(source, F);\
		all();\
	scamp5_kernel_end();\
})

// Copies the content of a source AREG to the target AREG
#define copy_areg(target, source)({\
	scamp5_kernel_begin();\
		mov(target, source);\
	scamp5_kernel_end();\
})


// Quantises the content of a source AREG to 3 target
// DREGs, using the 3 most significant bits (MSB)
// UNSIGNED quantisation, assumes source AREG is > 0
// Uses auxilliary AREG D, E and F.
#define quantise_areg(target1, target2, target3, source)({\
	/* First MSB in target1 */\
	scamp5_in(E, 64);\
	scamp5_kernel_begin();\
		CLR(target1);\
		abs(D,source);\
		sub(F, D, E);\
		where(F);\
			MOV(target1, FLAG);\
			mov(D, F);\
		all();\
	scamp5_kernel_end();\
	/* Second MSB in target2 */\
	scamp5_in(E, 32);\
	scamp5_kernel_begin();\
	CLR(target2);\
		sub(F, D, E);\
		where(F);\
			MOV(target2, FLAG);\
			mov(D, F);\
		all();\
	scamp5_kernel_end();\
	/* Third MSB in target3 */\
	scamp5_in(E, 16);\
	scamp5_kernel_begin();\
		CLR(target3);\
		sub(F, D, E);\
		where(F);\
			MOV(target3, FLAG);\
		all();\
	scamp5_kernel_end();\
})


// Reconstruct value stored in DREGs source1, source2, source3
// to target AREG.
// Uses auxilliary AREG F
#define unquantise_dregs(target, source1, source2, source3)({\
	/* Restore first MSB */\
	scamp5_in(F, 64);\
	scamp5_kernel_begin();\
		res(target);\
		WHERE(source1);\
			add(target, target, F);\
		all();\
	scamp5_kernel_end();\
	/* Restore second MSB */\
	scamp5_in(F, 32);\
	scamp5_kernel_begin();\
		WHERE(source2);\
			add(target, target, F);\
		all();\
	scamp5_kernel_end();\
	/* Restore third MSB */\
	scamp5_in(F, 16);\
	scamp5_kernel_begin();\
		WHERE(source3);\
			add(target, target, F);\
		all();\
	scamp5_kernel_end();\
})


int main(){
    // Initialization
    vs_init();
    vs_stopwatch stopwatch;

    /////////////////
    // Setup Host GUI
    /////////////////
    // DISPLAYS:
    // First convolution result and binarised version
    auto display_a = vs_gui_add_display("Register A",0,0);
    auto display_d = vs_gui_add_display("R9: conv_2_1",0,1);
    // Second convolution result and binarised version
    auto display_b = vs_gui_add_display("Register B",1,0);
    auto display_e = vs_gui_add_display("R10: conv_2_2",1,1);
    // Third convolution result and binarised version
    auto display_c = vs_gui_add_display("Register C",2,0);
    auto display_f = vs_gui_add_display("R11: conv_2_3",2,1);
    // Input image
    auto original_input = vs_gui_add_display("Original Input",2,2);
    // Binarised input image
    auto display_R6 = vs_gui_add_display("Binarised input",1,2);
    // Cropped (28*28 square) binarised input image
    auto display_R7 = vs_gui_add_display("Cropped input",0,2);

    // SLIDERS:
    // Input binarisation threshold
    auto threshold = vs_gui_add_slider("Input thresh: ",10,127,50);
    uint8_t threshold_value;
    // Output thresholds for each of the 3 convolutions
    auto t1 = vs_gui_add_slider("Thresh conv_2_1: ",-10,100,conv2_1_bias);
    auto t2 = vs_gui_add_slider("Thresh conv_2_2: ",-10,100,conv2_2_bias);
    auto t3 = vs_gui_add_slider("Thresh conv_2_3: ",-10,100,conv2_2_bias);
    int t1_value=conv2_1_bias, t2_value=conv2_2_bias, t3_value=conv2_3_bias;
    auto recording = vs_gui_add_slider("Recording: ",0,1,0);
    int recording_value;
    auto output_videos = vs_gui_add_slider("Output videos: ",0,1,0);
    int output_videos_value;

    // VAR DECLARATIONS
    // To read the events (=coordinates of 1s)
    uint8_t coordinates[300];
    // Number of events in the input frame
    uint8_t input_count;
    // Number of consecutively empty input frames
    int empty_input_frames = 0;
    // Flattened output of the convolution layers
    int conv_outputs[36];
    // First fully connected layer output
    int fc1_result[50];
    // Second fully connected layer output
    int fc2_result[10];
    // Output label (index of max. in results)
    uint8_t max_index;

    vs_on_host_connect([&](){
        vs_post_text("\n2-layer, quantised, AnalogNet architecture\n");
        vs_led_on(VS_LED_2);
    });

    vs_on_host_disconnect([&](){
        vs_led_off(VS_LED_2);
    });

    // Set framerate to 12 fps,
    // with constant loop time
    //vs_configure_frame_trigger(VS_FRAME_TRIGGER_CONST_LOOP, 12);

    // Frame Loop
    while(1){
        vs_process_message();

        // Get GUI values at start of frame
        threshold_value = vs_gui_read_slider(threshold);
        t1_value = vs_gui_read_slider(t1);
        t2_value = vs_gui_read_slider(t2);
        t3_value = vs_gui_read_slider(t3);
        recording_value = vs_gui_read_slider(recording);
        output_videos_value = vs_gui_read_slider(output_videos);

        vs_post_text("%lu\n", stopwatch.get_usec());
        stopwatch.reset();

        /*
         * Begin Input
         */
        // Perform synchronized acquisition (constant FPS):
        //vs_wait_frame_trigger();
        scamp5_get_image(A, B, 1);

        if (output_videos_value)
        	scamp5_output_image(A, original_input);

        // Reverse binarisation
        // (on display: black digit on white background
        // we need the contrary: white digit on black background)
        // Binarised input image is stored in R6
        binarise_input(R6, A, threshold_value);
        // Keep only central 28*28 square: R6->R7
        central_mask(R7, R6);

        // Display results of binarization:
        // binarised + binarised and cropped
        if (output_videos_value){
        	scamp5_output_image(R6,display_R6);
        	scamp5_output_image(R7,display_R7);
        }

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
				// After 300 empty frames (25 s at 12 fps),
				// save and clear current output
				if(empty_input_frames == 300){
					vs_post_text("!save");
					vs_post_text("!clear");
				}
				// After 720 empty frames (1 min at 12 fps),
				// forever loop (ie stop main processing loop)
				if(empty_input_frames > 720){
					while(true){
					}
				}
			} else {
				empty_input_frames = 0;
			}
        }

        /*
         * CONVOLUTIONS
         */
		// FIRST LAYER, filters applied in A,B,C,
        dreg_to_areg(A, R7);
        conv_1_1();
#if !NEGLECT_BIAS
        add_const(A, conv2_1_bias);
#endif
        conv_relu(A);

        dreg_to_areg(B, R7);
        conv_1_2();
#if !NEGLECT_BIAS
        add_const(B, conv2_2_bias);
#endif
        conv_relu(B);

        dreg_to_areg(C, R7);
        conv_1_3();
#if !NEGLECT_BIAS
        add_const(C, conv2_3_bias);
#endif
        conv_relu(C);

        // The two first conv are quantised
        quantise_areg(R2, R3, R4, A);
        quantise_areg(R6, R7, R8, B);

        // SECOND LAYER
        // conv_2_1: each of the 3 kernels computed in A,B,B
        // then summed in A and outputted
        conv_2_1_1();	// In A

        conv_2_1_2();	// In B
        scamp5_kernel_begin();
        	add(A,A,B);
        scamp5_kernel_end();

        copy_areg(B, C);
        conv_2_1_3();	// In B
        scamp5_kernel_begin();
        	add(A,A,B);
        scamp5_kernel_end();

        // Output Thresholding: AREG A->R9
        out_thresholding(R9, A, t1_value);

        // conv_2_2: each of the 3 kernels computed in C,B,C
		// then summed in B and outputted
        copy_areg(A,C);	// make a copy of conv_1_3 in A
		conv_2_2_3();	// In C

        unquantise_dregs(B, R3, R4, R5);	// retrieve conv_1_1
        conv_2_2_1();	// In B
        scamp5_kernel_begin();
        	add(B,B,C);	// B stores conv_2_2_1 + conv_2_2_3
        scamp5_kernel_end();

        unquantise_dregs(C, R6, R7, R8);	// retrieve conv_1_2
        conv_2_2_2();	// In C
		scamp5_kernel_begin();
			add(B,B,C);	// B stores conv_2_2
		scamp5_kernel_end();

		// Output Thresholding: AREG B->R10
		out_thresholding(R10, B, t2_value);

        // conv_2_3: each of the 3 kernels computed in C,B,C
		// then summed in B and outputted
		conv_2_3_3();	// In A

		unquantise_dregs(B, R3, R4, R5);	// retrieve conv_1_1
		conv_2_3_1();	// In B
		scamp5_kernel_begin();
			add(A,A,B);	// A stores conv_2_3_1 + conv_2_3_3
		scamp5_kernel_end();

        unquantise_dregs(B, R6, R7, R8);	// retrieve conv_1_2
        conv_2_3_2();	// In B
        scamp5_kernel_begin();
        	add(A,A,B);	// A stores conv_2_3
        scamp5_kernel_end();

		// Output Thresholding: AREG A->R11
		out_thresholding(R11, A, t3_value);

		// Mask all 3 output feature maps, to remove
		// events due to noise outside the central zone.
		//central_mask(R6, R9);
		//central_mask(R7, R10);
		//central_mask(R8, R11);
		scamp5_kernel_begin();
			MOV(R6, R9);
			MOV(R7, R10);
			MOV(R8, R11);
		scamp5_kernel_end();

		///////////////////////////////////////////////////
		//Processing from here is on microcontroller
		///////////////////////////////////////////////////

		/*
		 * COUNT 1s in Convolution Filter Results
		 */
		// Process conv_2_1
		scamp5_scan_events(R6, coordinates, 150);
		sum_pooling_events<150>(coordinates, &conv_outputs[0]);

		// Process conv_2_2
		scamp5_scan_events(R7, coordinates, 150);
		sum_pooling_events<150>(coordinates, &conv_outputs[12]);

		// Process conv_2_3
		scamp5_scan_events(R8, coordinates, 150);
		sum_pooling_events<150>(coordinates, &conv_outputs[24]);

		if (recording_value){
			// Structure of one output line:
			// an array of 37 elements: the first one is the count
			// of 1s in the input binarised image, the 36 others
			// is the output of the conv layer
			vs_post_text("[%i, ", input_count);
			for(int i=0; i<35; i++)
				vs_post_text("%i, ", conv_outputs[i]);
			vs_post_text("%i]\n", conv_outputs[35]);
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
			scamp5_output_image(R6,display_d);
			scamp5_output_image(R7,display_e);
			scamp5_output_image(R8,display_f);
		}

        // increase loop_counter by 1
        vs_loop_counter_inc();
    }
    return 0;
}
