/*
 * Scamp5d M0 Program
 * This program demonstrates the principle of recompiling
 * vision chip kernels mid-loop. This trick can be used
 * to save memory space. This is especially useful for large CNNs.
 * 
 * Author: Benoit GUILLARD, Imperial College
 */


#include <scamp5.hpp>
#include <vs_import.hpp>

using namespace SCAMP5_PE;

scamp5_kernel conv_kernels[1];

int main(){
    // Initialization
    vs_init();
    //vs_stopwatch stopwatch;

    /////////////////
    // Setup Host GUI
    /////////////////
    // DISPLAYS:
    auto display_a = vs_gui_add_display("Register A",0,0);
    auto display_b = vs_gui_add_display("Register B",0,1);

    vs_on_host_connect([&](){
        vs_post_text("\nProgram segmentation demo.\n");
        vs_led_on(VS_LED_2);
    });

    vs_on_host_disconnect([&](){
        vs_led_off(VS_LED_2);
    });

    // Frame Loop
    scamp5_kernel *pointer_to_array;
    while(1){
        vs_process_message();

        /*
         * INPUT
         */
        scamp5_in(A, 120);
        scamp5_in(B, 120);

        /*
         * KERNELS
         */
        // Acting on AREG A
        pointer_to_array = new scamp5_kernel[1];
        pointer_to_array[0]([]{
			using namespace scamp5_kernel_api;
			diva(A,E,F);
			diva(A,E,F);
			diva(A,E,F);
        });
        scamp5_launch_kernel(pointer_to_array[0]);
        delete[] pointer_to_array;

        pointer_to_array = new scamp5_kernel[1];
        pointer_to_array[0]([]{
			using namespace scamp5_kernel_api;
			diva(B,E,F);
			diva(B,E,F);
			diva(B,E,F);
        });
        scamp5_launch_kernel(pointer_to_array[0]);
        delete[] pointer_to_array;

        /*
         * OUTPUT
         */

		// Output images to GUI
		scamp5_output_image(A,display_a);
		scamp5_output_image(B,display_b);

        // increase loop_counter by 1
        vs_loop_counter_inc();
    }
    return 0;
}
