/*
 * Scamp5d M0 Example 1 - Image Capturing and Host Display
 *
 */

#include <scamp5.hpp>
#include "conv_instructions.hpp"
#include "fc_weights.hpp"

using namespace SCAMP5_PE;

//#define COLLECTED_EVENTS 5500
#define COLLECTED_EVENTS 5000

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


// Given SCAN_SIZE coordinates,
// group them into 9 bins, filling the count array
template <int SCAN_SIZE>
inline void sum_pooling_events(uint8_t coordinates[2*SCAN_SIZE], int count[9]){
	uint8_t x,y,px,py;
	for(int i = 0; i < 9; i++){
		count[i] = 0;
	}
	for(int i = 0; i < SCAN_SIZE; i++)
	{
		x = coordinates[2*i];
		y = coordinates[2*i+1];
		if(x == 0 && y == 0)
			return;
		if(x >= 32 && x <= 223 && y >= 32 && y <= 223){
			if(x <= 95){
				px=0;
			}else if (x <= 159){
				px=1;
			}else{
				px=2;
			}

			if(y <= 95){
				py=0;
			}else if (y <= 159){
				py=1;
			}else{
				py=2;
			}

			count[3*px+py]++;
		}
	}
}

template <int SCAN_SIZE>
void display_events_count(uint8_t coordinates[2*SCAN_SIZE]){
	int count_events = 0;
	uint8_t x, y;
	for(int i = 0; i < SCAN_SIZE; i++){
		x = coordinates[2*i];
		y = coordinates[2*i+1];
		if(x == 0 && y == 0){
			vs_post_text("Gathered events: %i\n", count_events);
			return;
		}
		else{
			count_events ++;
		}
	}
	vs_post_text("Gathered events: %i\n", count_events);
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

int main(){

	// Initialization
	vs_init();
	//vs_stopwatch stopwatch;

    vs_on_host_connect([&](){
        vs_post_text("Scamp5d - Lane following\n");
        vs_led_on(VS_LED_2);
    });

	vs_on_host_disconnect([&](){
		vs_led_off(VS_LED_2);
	});

    /////////////////
    // Setup Host GUI
    /////////////////
    // DISPLAYS:
    // Binarised input
	//auto display_a = vs_gui_add_display("Input img",0,0);
	auto display_a = vs_gui_add_display("Input img", 0, 0, 2);
	//auto display_b = vs_gui_add_display("Conv1",1,0);
	//auto display_c = vs_gui_add_display("Conv2",1,1);
	//auto display_d = vs_gui_add_display("Conv3",0,1);

	// SLIDERS:
	// Input binarisation threshold
	auto in_threshold = vs_gui_add_slider("Input thresh: ",10,127,75);
    // Output thresholds for each of the 3 convolutions
    auto t1 = vs_gui_add_slider("Thresh conv1: ",10,100,20);
    auto t2 = vs_gui_add_slider("Thresh conv2: ",10,100,20);
    auto t3 = vs_gui_add_slider("Thresh conv3: ",10,100,20);
	// Binary switch: output videos or not
	auto output_videos = vs_gui_add_slider("Output videos: ",0,1,0);
	int t1_value, t2_value, t3_value;
	int in_threshold_value, output_videos_value;

    // VAR DECLARATIONS
    // To read the events (=coordinates of 1s)
    uint8_t coordinates[COLLECTED_EVENTS*2];
    // Number of events in the input frame
    //uint8_t input_count;
    // Number of consecutively empty input frames
    //int empty_input_frames = 0;
    // Flattened output of the convolution layers
    int conv_outputs[27];
    // First fully connected layer output
    int fc1_result[50];
    // Second fully connected layer output
    int fc2_result[3];
    // Output label (index of max. in fc2_result)
    uint8_t max_index;

	// Frame Loop
	// Set framerate to 20 fps,
	// with constant loop time
	vs_configure_frame_trigger(VS_FRAME_TRIGGER_CONST_PIX, 20);
	//int c = 0;
    while(1){
        vs_process_message();
        // Read sliders
        in_threshold_value = vs_gui_read_slider(in_threshold);
        scamp5_in(D, in_threshold_value);
        output_videos_value = vs_gui_read_slider(output_videos);
        t1_value = vs_gui_read_slider(t1);
        t2_value = vs_gui_read_slider(t2);
        t3_value = vs_gui_read_slider(t3);

        //vs_post_text("%lu\n", stopwatch.get_usec());
        //stopwatch.reset();

        // Input image
        vs_wait_frame_trigger();
        scamp5_get_image(A, B, 1);

        // Perform Binarisation Procedure on AREG A,
        // store binarised input image in R5
        scamp5_kernel_begin();
        	add(A, A, D);
        	CLR(R5);
        	CLR(R6);
        	CLR(R7);
        	where(A);
        		OR(R5, FLAG, R6);
        	ALL();
        scamp5_kernel_end();

        // Crop image: remove margins of 32 px
        scamp5_draw_begin(R6);
        	scamp5_draw_rect(32, 32, 223, 223);
        scamp5_draw_end();

        scamp5_kernel_begin();
        	CLR(R7);
        	AND(R7, R6, R5);
        scamp5_kernel_end();

        // Output frames
        if (output_videos_value){
        	scamp5_output_image(R7,display_a);
        }


        /*
         * Convolutions
         */
        scamp5_load_pattern(R1, 0, 0, 254, 254);

        dreg_to_areg();
        conv_1();
        scamp5_in(F, t1_value);
        scamp5_kernel_begin();
            CLR(R8);
        	sub(E, A, F);
        	where(E);
        		MOV(R8, FLAG);
        	ALL();
        scamp5_kernel_end();

        dreg_to_areg();
        conv_2();
        scamp5_in(F, t2_value);
        scamp5_kernel_begin();
            CLR(R9);
        	sub(E, A, F);
        	where(E);
        		MOV(R9, FLAG);
        	ALL();
        scamp5_kernel_end();

        dreg_to_areg();
        conv_3();
        scamp5_in(F, t3_value);
        scamp5_kernel_begin();
            CLR(R10);
        	sub(E, A, F);
        	where(E);
        		MOV(R10, FLAG);
        	ALL();
        scamp5_kernel_end();

        // Keep only 1/4 of the pixels
        scamp5_kernel_begin();
        	AND(R11, R10, R1);
        	AND(R10, R9, R1);
        	AND(R9, R8, R1);
        scamp5_kernel_end();

		///////////////////////////////////////////////////
		//Processing from here is on microcontroller
		///////////////////////////////////////////////////

		/*
		 * COUNT 1s in Convolution Filter Results
		 */
		// Process Register A
		scamp5_scan_events(R9, coordinates, COLLECTED_EVENTS);
		sum_pooling_events<COLLECTED_EVENTS>(coordinates, &conv_outputs[0]);
		//display_events_count<COLLECTED_EVENTS>(coordinates);

		// Process Register B
		scamp5_scan_events(R10, coordinates, COLLECTED_EVENTS);
		sum_pooling_events<COLLECTED_EVENTS>(coordinates, &conv_outputs[9]);
		//display_events_count<COLLECTED_EVENTS>(coordinates);

		// Process Register C
		scamp5_scan_events(R11, coordinates, COLLECTED_EVENTS);
		sum_pooling_events<COLLECTED_EVENTS>(coordinates, &conv_outputs[18]);
		//display_events_count<COLLECTED_EVENTS>(coordinates);

		// Structure of one output line:
		// an array of 27 elements: 9 elements per feature map
		/*
		vs_post_text("[%i, ", conv_outputs[0]);
		for(int i=1; i<26; i++)
			vs_post_text("%i, ", conv_outputs[i]);
		vs_post_text("%i]\n", conv_outputs[26]);
		*/

		// Fully connected forward pass:
		fc_1(conv_outputs, fc1_result);
		relu<50>(fc1_result);
		fc_2(fc1_result, fc2_result);


        // Output command (ie. direction to go)
        //vs_post_text("%i\n", 1+c/100);
        //c = (c+1) % 300;
		if(fc2_result[0] > fc2_result[1] && fc2_result[0] > fc2_result[2]){
			max_index = 0;
		} else if (fc2_result[1] > fc2_result[2]) {
			max_index = 1;
		} else {
			max_index = 2;
		}
		vs_post_text("%i\n", max_index+1);

        // Output frames
		/*
        if (output_videos_value){
        	scamp5_output_image(R9, display_b);
        	scamp5_output_image(R10,display_c);
        	scamp5_output_image(R11,display_d);
        }*/

        vs_loop_counter_inc();
    }

	return 0;
}
