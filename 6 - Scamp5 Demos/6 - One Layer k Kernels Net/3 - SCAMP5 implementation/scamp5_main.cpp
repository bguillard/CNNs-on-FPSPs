/*
 * Scamp5d M0 Program 2
 * This program runs the legacy AnalogNet architecture on the SCAMP5 device,
 * with a new output bining process (12 bins instead of 9, and a better
 * differentiation of central pixels, that now fall in different bins),
 * AND
 */

/*
 * AnalogNet: 15 fps network
 * Author: Benoit Guillard, MSc Thesis, Imperial College London
 * Weights and AnalogNet architecture from Matthew Wong
 */

#include <scamp5.hpp>
#include <vs_import.hpp>
#include "conv_instructions.hpp"

using namespace SCAMP5_PE;

// Given SCAN_SIZE coordinates,
// group them into 12 (overlapping) bins, filling the count array
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

// Copy a DREG to an AREG, with fixed value
// Convert binary image into analog image with uniform analog value (120)
// R7 to A, with value 120, using aux AREG D
inline void dreg_to_areg(){
    scamp5_in(D, 120);
	scamp5_kernel_begin();
		res(A);
		res(B);
		res(C);

		WHERE(R7);
			mov(A, D);
		ALL();
	scamp5_kernel_end();
}


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
    auto display_5 = vs_gui_add_display("R5",0,2);
    // Cropped (28*28 square) binarised input image
    auto display_6 = vs_gui_add_display("R6",1,2);

    // SLIDERS:
    // Input binarisation threshold
    auto threshold = vs_gui_add_slider("Input thresh: ",10,127,50);
    int threshold_value;
    // Output thresholds for each of the 3 convolutions
    auto t1 = vs_gui_add_slider("Thresh conv1: ",-40,100,30);
    auto t2 = vs_gui_add_slider("Thresh conv2: ",-40,100,15);
    int t1_value, t2_value;
    auto recording = vs_gui_add_slider("Recording: ",0,1,0);
    int recording_value;
    auto output_videos = vs_gui_add_slider("Output videos: ",0,1,0);
    int output_videos_value;

    // VAR DECLARATIONS
    // To read the events (=coordinates of 1s)
    uint8_t coordinates[400];
    // Number of events in the input frame
    uint8_t input_count;
    // Number of consecutively empty input frames
    int empty_input_frames = 0;
    // Flattened output of the convolution layers
    int conv_outputs[12*8];

    vs_on_host_connect([&](){
        vs_post_text("\nSingle layer architecture, with increasingly many kernels\n");
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
        recording_value = vs_gui_read_slider(recording);
        output_videos_value = vs_gui_read_slider(output_videos);

        //vs_post_text("%lu\n", stopwatch.get_usec());
        //stopwatch.reset();


        /*
         * Begin Input
         */
        // Perform Binarisation Procedure, store
        // binarised input image in R6
        vs_wait_frame_trigger();
        scamp5_in(D, threshold_value);

        scamp5_get_image(A, B, 1);
        scamp5_kernel_begin();
        	add(A, A, D);
        	CLR(R6);
        	where(A);
        		OR(R5, FLAG, R6);
        	ALL();
        	NOT(R6, R5);
        	CLR(R5);
        scamp5_kernel_end();

        // Only preserve data in 28 x 28 square, everything else marked as 0
        // R7 will contain binary image of input within 28 x 28 square
        scamp5_draw_begin(R5);
        	scamp5_draw_rect(114, 114, 141, 141);
        scamp5_draw_end();

        scamp5_kernel_begin();
        	CLR(R7);
        	AND(R7, R6, R5);
        scamp5_kernel_end();

        // Convert binary image into analog image with uniform analog value (120)
        // Binary uniform analog images now stored in Registers A, B, C
        scamp5_in(D, 120);
        scamp5_kernel_begin();
        	res(A);
        	res(B);
        	res(C);
        scamp5_kernel_end();

        // Display results of binarization:
        // original input; binarised; binarised and cropped
        if (output_videos_value){
        	scamp5_output_image(A, original_input);
        	scamp5_output_image(R6,display_6);
        	scamp5_output_image(R7,display_5);
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
		}
        /*
         * (binary Images are now in Registers A, B and C)
         * CONVOLUTIONS: filters applied and
         * results stored in registers A
         * Increasingly many kernels are applied
         */
		// Cut program in 4, because kernel limit...
		// 1-4 ; 5-6 ; 7; 8
        for(int k_num=8; k_num<=8; k_num++){
        	for(int k_id=1; k_id<=k_num; k_id++){
        		dreg_to_areg();
        		conv(k_num, k_id);

        		// Output Thresholding
                scamp5_in(F, thresh[k_num-1][k_id-1]);
                scamp5_kernel_begin();
                    CLR(R8);
                	sub(E, A, F);
                	where(E);
                		MOV(R8, FLAG);
                	ALL();
                scamp5_kernel_end();

                // Collect events
				scamp5_scan_events(R8, coordinates, 200);
				sum_pooling_events<200>(coordinates, &conv_outputs[12*(k_id-1)]);


				// MANUALLY ADJUSTING THRESHOLDS
				/*
				if(k_num==8 && k_id==7)
				{
	                scamp5_in(F, t1_value);
	                scamp5_kernel_begin();
	                    CLR(R8);
	                	sub(E, A, F);
	                	where(E);
	                		MOV(R8, FLAG);
	                	ALL();
	                scamp5_kernel_end();

	                scamp5_output_image(R8, display_a);
				}

				if(k_num==8 && k_id==8)
				{
	                scamp5_in(F, t2_value);
	                scamp5_kernel_begin();
	                    CLR(R8);
	                	sub(E, A, F);
	                	where(E);
	                		MOV(R8, FLAG);
	                	ALL();
	                scamp5_kernel_end();

	                scamp5_output_image(R8, display_b);
				}
				*/
        	}

        	if (recording_value){
				// Output one flattend array
				// Structure of one output line:
				// an array of 1+12*(k_num) elements: the first one is the count
				// of 1s in the input binarised image, the 12*(k_num) others
				// is the output of the conv layer
				vs_post_text("[%i, ", input_count);
				for(int i=0; i<12*k_num-1; i++)
					vs_post_text("%i, ", conv_outputs[i]);
				vs_post_text("%i]\n", conv_outputs[12*k_num-1]);
        	}
        }

		///////////////////////////////////////////////////
		//Processing from here is on microcontroller
		///////////////////////////////////////////////////

		if (recording_value){
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
		}

        // increase loop_counter by 1
        vs_loop_counter_inc();
    }
    return 0;
}
