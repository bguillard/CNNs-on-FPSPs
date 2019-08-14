/*
 * Scamp5d M0 Program
 * This program demonstrates the accumulation of noise on the device,
 * and how it affects the signal.
 *
 * On a constant, uniform input (of value INPUT_VALUE),
 * we perform division by 2 and addition with itself (which
 * should yield the same content as the input) a variable number
 * of times (1 to 32).
 * This operation is carried out 100 times, after a trigger is pressed.
 *
 *
 * A post processing script will eventually compute the
 * PSNR we get, and compensate for constant bias.
 * 
 * Author: Benoit GUILLARD, Imperial College
 */


#include <scamp5.hpp>
#include <vs_import.hpp>
#include <string>

using namespace SCAMP5_PE;

#define INPUT_VALUE 0
#define PRE_ITERATIONS 24
/*
 * We are limited to having 20 screens in the host interface. For this reason,
 * we cut our program in 4:
 * 	first, we do iterations 1 to 8
 * 	then 9 to 16
 * 	then 17 to 24
 * 	then 25 to 32
 * This is done with the PRE_ITERATIONS constant: setting it to 16
 * will make the program realize the third quarter.
 */

scamp5_kernel my_id_operation([]{
    using namespace scamp5_kernel_api;
	diva(A,E,F);
	mov(F,A);
	add(A,A,F);
});

inline void apply_id_operation_iter(int it, vs_handle in_screen, vs_handle out_screen){
	// Input
	scamp5_in(A, INPUT_VALUE);
	scamp5_output_image(A, in_screen);

	// Apply kernel multiple times
    for(int i=0; i<it; i++){
    	scamp5_launch_kernel(my_id_operation);
    }

    // Output result
    scamp5_output_image(A, out_screen);
}

int main(){
    // Initialization
    vs_init();

    /////////////////
    // Setup Host GUI
    /////////////////
    // TRIGGER
    // Binary switch: launch loop or not
	auto start_loop = vs_gui_add_slider("Start loop: ",0,1,0);
	int start_loop_value = 0;

    // DISPLAYS:
    vs_handle display_array[16];
    char display_names[16][16];
    for(int i=0; i<2; i++){
    	for(int j=0; j<4; j++){
    		std::string n = std::to_string(1 + j + 4*i + PRE_ITERATIONS);

    		strcpy(display_names[i*8 + j*2], ("Input f "+n).c_str());
    		display_array[i*8 + j*2]     = vs_gui_add_display(display_names[i*8 + j*2],i,j*2);

    		strcpy(display_names[i*8 + j*2 + 1], ("Output f "+n).c_str());
    		display_array[i*8 + j*2 + 1] = vs_gui_add_display(display_names[i*8 + j*2 + 1],i,j*2+1);
    	}
    }

    vs_on_host_connect([&](){
        vs_post_text("\nNoise accumulation demo.\n");
        vs_led_on(VS_LED_2);
    });

    vs_on_host_disconnect([&](){
        vs_led_off(VS_LED_2);
    });


    // Heating loop
    // (for compiling kernels, init. USB connection...)
    for(int i=0; i<8; i++){
    	for(int j=0; j<10; j++){
    		vs_process_message();
    		apply_id_operation_iter(PRE_ITERATIONS+i+1, display_array[2*i], display_array[2*i+1]);
    		vs_loop_counter_inc();
    	}
    }

    // Wait for trigger
    while(!start_loop_value){
		vs_process_message();
		start_loop_value = vs_gui_read_slider(start_loop);
		vs_loop_counter_inc();
    }


    // For each output screen, iterate 100 times
    for(int i=0; i<8; i++){
    	for(int j=0; j<100; j++){
    		vs_process_message();
    		apply_id_operation_iter(PRE_ITERATIONS+i+1, display_array[2*i], display_array[2*i+1]);
    		vs_loop_counter_inc();
    	}
    }

    return 0;
}
