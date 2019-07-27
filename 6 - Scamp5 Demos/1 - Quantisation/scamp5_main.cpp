/*
 * Scamp5d M0 Program 1 - Based on
 * Image Capturing and Host Display,
 * this program performs quantisation (either signed
 * or unsigned) on 3 bits (+1 is signed)
 */
#define SIGNED_QUANTISATION false

#include <scamp5.hpp>

using namespace SCAMP5_PE;
using namespace scamp5_kernel_api;


// Stores a quantised version of source AREG in
// target DREGs target1, target2, target3, target4;
// Uses auxilliary AREG D, E, and F
#define quantise_signed_areg(target1, target2, target3, target4, source)({\
	scamp5_kernel_begin();\
		CLR(target1);\
		CLR(target2);\
		CLR(target3);\
		CLR(target4);\
		/* Sign in target1 */\
		where(A);\
			MOV(target1, FLAG);\
		all();\
		NOT(R1);\
		abs(D, source);\
		/* First MSB in target2 */\
	scamp5_kernel_end();\
	scamp5_in(E, 64);\
	scamp5_kernel_begin();\
		sub(F, D, E);\
		where(F);\
			MOV(target2, FLAG);\
			mov(D, F);\
		all();\
		/* Second MSB in target3 */\
	scamp5_kernel_end();\
	scamp5_in(E, 32);\
	scamp5_kernel_begin();\
		sub(F, D, E);\
		where(F);\
			MOV(target3, FLAG);\
			mov(D, F);\
		all();\
		/* Third MSB in target4 */\
	scamp5_kernel_end();\
	scamp5_in(E, 16);\
	scamp5_kernel_begin();\
		sub(F, D, E);\
		where(F);\
			MOV(target4, FLAG);\
		all();\
	scamp5_kernel_end();\
})


// Reconstruct value stored in DREGs source1, source2, source3, source4,
// to target AREG.
// Uses auxilliary AREG F
#define unquantise_signed_dregs(target, source1, source2, source3, source4)({\
	/* First MSB */\
	scamp5_in(F, 64);\
	scamp5_kernel_begin();\
		res(target);\
		WHERE(source2);\
			add(target, target, F);\
		all();\
	scamp5_kernel_end();\
	/* Second MSB */\
	scamp5_in(F, 32);\
	scamp5_kernel_begin();\
		WHERE(source3);\
			add(target, target, F);\
		all();\
	scamp5_kernel_end();\
	/* Third MSB */\
	scamp5_in(F, 16);\
	scamp5_kernel_begin();\
		WHERE(source4);\
			add(target, target, F);\
		all();\
		/* Sign */\
		WHERE(source1);\
			neg(F, target);\
			mov(target, F);\
		all();\
	scamp5_kernel_end();\
})


// Stores a quantised version of source AREG in
// target DREGs target1, target2, target3
// Uses auxilliary AREG D, E, and F
#define quantise_unsigned_areg(target1, target2, target3, source)({\
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
#define unquantise_unsigned_dregs(target, source1, source2, source3)({\
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

	// Setup Host GUI
    auto display_1 = vs_gui_add_display("capture",0,0);
    auto display_2 = vs_gui_add_display("reconstruct",1,0);
    auto display_3 = vs_gui_add_display("sign",0,1);
    auto display_4 = vs_gui_add_display("MSB",1,1);


    vs_on_host_connect([&](){
        vs_post_text("Scamp5d Demo Program - Quantisation\n");
        vs_led_on(VS_LED_2);
    });

	vs_on_host_disconnect([&](){
		vs_led_off(VS_LED_2);
	});

	// Frame Loop
    while(1){
        vs_process_message();
        // Get input image
        scamp5_get_image(A,B);

#if SIGNED_QUANTISATION
        // Signed:

		// Quantise AREG A to DREGs R1 -> R4
        quantise_signed_areg(R1, R2, R3, R4, A);
		// Reconstruct value stored in R1->R4 to AREG B
        unquantise_signed_dregs(B, R1, R2, R3, R4);

#else
        // Unsigned:
        scamp5_kernel_begin();
        abs(F,A);
        mov(A,F);
        scamp5_kernel_end();

        // Discretise AREG A to DREGs R2 -> R4
        quantise_unsigned_areg(R2, R3, R4, A);
        // Reconstruct value stored in R2->R4 to AREG B
        unquantise_unsigned_dregs(B, R2, R3, R4);
#endif

		// Output
		scamp5_output_image(A,display_1);
		scamp5_output_image(B,display_2);
		scamp5_output_image(R1,display_3);
		scamp5_output_image(R2,display_4);
        vs_loop_counter_inc();
    }

	return 0;
}
