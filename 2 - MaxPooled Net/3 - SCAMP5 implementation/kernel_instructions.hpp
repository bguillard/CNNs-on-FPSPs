/*
 * conv_instructions.hpp
 *
 *  Created on: Jul 4, 2019
 *      Author: Benoît GUILLARD
 */
#ifndef KERNEL_INSTRUCTIONS_HPP_
#define KERNEL_INSTRUCTIONS_HPP_

#include <scamp5.hpp>
using namespace SCAMP5_PE;

// Perform Binarisation Procedure on the content of the AREG source.
// All values inferior to -1*thresh are set to 1 in the target DREG,
// others are set to 0.
// Uses auxiliary AREG F.
#define binarise_input(target, source, threshold_value)({\
	add(source, source, F);\
	CLR(target);\
	where(source);\
		MOV(target, FLAG);\
	ALL();\
	NOT(target);\
})

// Only preserve data in central 28 x 28 square of source (DREG),
// copy its content to target (DREG) (everything else marked as 0),
// Uses auxiliary DREG R2,
// and ASSUMES central square is positively masked in R2.
#define central_mask(target, source)({\
	/* Mask everything that is outside this central square */\
	CLR(target);\
	AND(target, source, R2);\
	CLR(R2);\
})

// Convert binary image from source DREG
// into analog image with uniform analog value (120)
// in target AREG. Uses auxiliary AREG F,
// and ASSUMES F is uniformly filled with 120.
// MACRO, because kernel uses different source/target..
#define dreg_to_areg(target, source)({\
	res(target);\
	WHERE(source);\
		mov(target, F);\
	ALL();\
})


// Max pooling source AREG, putting the maximum of each 2*2
// square in the position defined by (dir_x, dir_y):
// (west, south): target is top-left corner of each square (R1: (0,0))
// (east, south): target is top-right corner (R2, (0,1))
// (west, north): target is bottom-left corner of each square (R3: (1,0))
// (east, north): target is bottom-right corner of each squre (R4: (1,1))
// Uses auxiliary AREG F. Modifies source AREG.
#define maxpooling(source, dir_x, dir_y)({\
	/* Pooling along x-axis */ \
	sub(F, source, dir_x, source);\
		where(F);\
		add(source, source, F);\
	all();\
	/* Pooling along y-axis */\
	sub(F, source, dir_y, source);\
		where(F);\
		add(source, source, F);\
	all();\
})

// Aliases of the above, hiding the direction parameters
#define maxpooling_pos_1(source) 	{maxpooling(source, west, south);}
#define maxpooling_pos_2(source) 	{maxpooling(source, east, south);}
#define maxpooling_pos_3(source) 	{maxpooling(source, west, north);}
#define maxpooling_pos_4(source) 	{maxpooling(source, east, north);}


// Copying the content of source AREG to dest AREG,
// only at places positively masked by mask DREG
#define partial_copy(target, source, mask)({\
	WHERE(mask);\
		mov(target, source);\
	ALL();\
})


// Copies the content of a source AREG to the target AREG
#define copy_areg(target, source)({\
	mov(target, source);\
})


// Applies ReLU on whole source AREG,
// Uses aux AREGs E and F,
// and ASSUMES F is uniformly filled with 0.
#define conv_relu(source)({\
	neg(E, source);\
	where(E);\
		mov(source, F);\
	all();\
})


// Adding the content of source AREG to dest AREG,
// only at places positively masked by mask DREG
#define partial_sum(target, source, mask)({\
	WHERE(mask);\
		add(target, target, source);\
	ALL();\
})


// Output thresholding: from source AREG, all locations >= AREG F
// are set to 1 in target DREG. Others are set to 0.
// Uses auxilliary AREG E and F.
#define out_thresholding(target, source)({\
	CLR(target);\
	sub(E, source, F);\
	where(E);\
		MOV(target, FLAG);\
	ALL();\
})


// Macros to simply copy/paste CSIM code
// as convolution codes
#define west(X,Y) 	{mov(X,Y,west);}
#define east(X,Y) 	{mov(X,Y,east);}
#define south(X,Y) 	{mov(X,Y,south);}
#define north(X,Y) 	{mov(X,Y,north);}
#define div2(X,Y)	{div(X,F,Y);}
#define div2_inplace(X,Y) 	{diva(X,E,F);}
#define neg_inplace(X,Y)	{	mov(F,Y);	neg(X,F);}
#define add_twice(X, Y, Z)	{	mov(F, Z);	add(X, Y, F);}
#define sub_inplace(X, Y, Z){	sub(F, Y, Z);	mov(X, F);}


// Macros for double shifting operations.
// Allows for in place convolution computations
// on interleaved feature maps.
// ALL OF THEM ASSUME DREG R5 CONTAINS THE CURRENT MASK OF INTEREST
// and use aux AREG F
#define double_west(X,Y)	{	ALL();	west(F,Y);	west(F,F);	WHERE(R5);	mov(X,F);}
#define double_east(X,Y)	{	ALL();	east(F,Y);	east(F,F);	WHERE(R5);	mov(X,F);}
#define double_south(X,Y)	{	ALL();	south(F,Y);	south(F,F);	WHERE(R5);	mov(X,F);}
#define double_north(X,Y)	{	ALL();	north(F,Y);	north(F,F);	WHERE(R5);	mov(X,F);}

// Dynamic array of kernels, that are
// recompiled mid-loop, to save some space in the
// 40kb allocated for vision chip instructions.
// This is done at the expense of a slower loop (kernels are
// recompiled each time), but is currently the only way of
// having our program run in the memory budget...
//scamp5_kernel array_kernels[8];
scamp5_kernel *array_kernels;

uint8_t binarise_input_id;
uint8_t clear_R2_id;
uint8_t central_mask_id;
uint8_t A_from_R7_id;
uint8_t B_from_R7_id;

uint8_t maxpooling_pos_1_A_id;
uint8_t maxpooling_pos_2_B_id;
uint8_t maxpooling_pos_3_B_id;
uint8_t maxpooling_pos_4_B_id;

uint8_t partial_copy_A_B_R2_id;
uint8_t partial_copy_A_B_R3_id;
uint8_t partial_copy_A_B_R4_id;

uint8_t conv_relu_A_id;

uint8_t conv_1_1_id;
uint8_t conv_1_2_id;
uint8_t conv_1_3_id;
uint8_t conv_1_4_id;

const int8_t conv1_4_bias = -2;

uint8_t copy_areg_C_A_id;
uint8_t conv_2_1_1_id;
uint8_t partial_copy_B_C_R1_id;
uint8_t conv_2_1_2_id;
uint8_t partial_copy_B_C_R2_id;
uint8_t conv_2_1_3_id;
uint8_t partial_copy_B_C_R3_id;
uint8_t conv_2_1_4_id;
uint8_t partial_copy_B_C_R4_id;
uint8_t sum_conv_2_1_id;
uint8_t conv_2_1_bias = 127;

uint8_t res_B_R2_id;
uint8_t conv_2_2_1_id;
uint8_t shift_C_R1_R2_id;
uint8_t partial_sum_B_C_R2_id;
uint8_t conv_2_2_2_id;
uint8_t conv_2_2_3_id;
uint8_t shift_C_R3_R2_id;
uint8_t conv_2_2_4_id;
uint8_t shift_C_R4_R2_id;
uint8_t conv_2_2_bias = 15;

uint8_t res_B_R3_id;
uint8_t conv_2_3_1_id;
uint8_t shift_C_R1_R3_id;
uint8_t partial_sum_B_C_R3_id;
uint8_t conv_2_3_2_id;
uint8_t shift_C_R2_R3_id;
uint8_t conv_2_3_3_id;
uint8_t conv_2_3_4_id;
uint8_t shift_C_R4_R3_id;
uint8_t conv_2_3_bias = 20;

uint8_t res_B_R4_id;
uint8_t conv_2_4_1_id;
uint8_t shift_C_R1_R4_id;
uint8_t partial_sum_B_C_R4_id;
uint8_t conv_2_4_2_id;
uint8_t shift_C_R2_R4_id;
uint8_t conv_2_4_3_id;
uint8_t shift_C_R3_R4_id;
uint8_t conv_2_4_4_id;
uint8_t conv_2_4_bias = 0;

uint8_t t1_value, t2_value, t3_value, t4_value;
uint8_t partial_copy_F_E_R1_id, partial_copy_F_E_R2_id,
		partial_copy_F_E_R3_id, partial_copy_F_E_R4_id;
uint8_t out_thresh_R8_B_conv21_to_conv24_id;

uint8_t res_B_id;

uint8_t conv_2_5_1_id;
uint8_t conv_2_5_2_id;
uint8_t conv_2_5_3_id;
uint8_t conv_2_5_4_id;
uint8_t sum_conv_2_5_id;
uint8_t conv_2_5_bias = 30;

uint8_t conv_2_6_1_id;
uint8_t conv_2_6_2_id;
uint8_t conv_2_6_3_id;
uint8_t conv_2_6_4_id;
uint8_t conv_2_6_bias = 15;

uint8_t conv_2_7_1_id;
uint8_t conv_2_7_2_id;
uint8_t conv_2_7_3_id;
uint8_t conv_2_7_4_id;
uint8_t conv_2_7_bias = 30;

uint8_t conv_2_8_1_id;
uint8_t conv_2_8_2_id;
uint8_t conv_2_8_3_id;
uint8_t conv_2_8_4_id;
uint8_t conv_2_8_bias = 15;

uint8_t t5_value, t6_value, t7_value, t8_value;
uint8_t out_thresh_R9_B_conv25_to_conv28_id;

// Load the kernels for the first part of the convolutions,
// 					conv_1_1 to conv_2_1_4
// with all util operations included.
// (27 kernels total) loaded in the global array array_kernels,
// accessible by their id *kernel name*_id
void fill_kernels_part_1(){
	//delete[] array_kernels;
	array_kernels = new scamp5_kernel[27];
	uint8_t i = 0;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		binarise_input(R6, A, threshold_value);
	});
	binarise_input_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R2);
	});
	clear_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		central_mask(R7, R6);
	});
	central_mask_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		dreg_to_areg(A, R7);
	});
	A_from_R7_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		dreg_to_areg(B, R7);
	});
	B_from_R7_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		maxpooling_pos_1(A);
	});
	maxpooling_pos_1_A_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		maxpooling_pos_2(B);
	});
	maxpooling_pos_2_B_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		maxpooling_pos_3(B);
	});
	maxpooling_pos_3_B_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		maxpooling_pos_4(B);
	});
	maxpooling_pos_4_B_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(A, B, R2);
	});
	partial_copy_A_B_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(A, B, R3);
	});
	partial_copy_A_B_R3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(A, B, R4);
	});
	partial_copy_A_B_R4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		conv_relu(A);
	});
	conv_relu_A_id = i;
	i++;

	//Convolution: Filter 1.1, source/target: AREG A
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:1 s:2 neg:0
		// _transform(A, A, 0, 1, 2, 0);
		north(A, A);
		div2_inplace(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(B, A, -1, 0, 0, 0);
		west(B, A);
		add(A, A, B);
		// [B] -> [B] || x:0 y:-2 s:0 neg:0
		// _transform(B, B, 0, -2, 0, 0);
		south(B, B);
		south(B, B);
		add(A, A, B);
		// ----------------------------------------------------
	});
	conv_1_1_id = i;
	i++;

	//Convolution: Filter 1.2, source/target: AREG B
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(B, B, 0, 0, 1, 1);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:1 s:1 neg:0
		// _transform(C, B, 0, 1, 1, 0);
		north(C, B);
		div2_inplace(C, C);
		// [D] -> [C] || x:-1 y:0 s:0 neg:1
		// _transform(D, C, -1, 0, 0, 1);
		west(D, C);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:-2 s:0 neg:0
		// _transform(E, D, 0, -2, 0, 0);
		south(E, D);
		south(E, E);
		sub(D, D, E);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		add(C, C, B);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(B, B, C);
		add(B, B, D);
		// [C] -> [D] || x:2 y:0 s:0 neg:0
		// _transform(C, D, 2, 0, 0, 0);
		east(C, D);
		east(C, C);
		sub(B, B, C);
		// ----------------------------------------------------
	});
	conv_1_2_id = i;
	i++;

	//Convolution: Filter 1.3, source/target: AREG B
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(B, B, 0, 0, 1, 1);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:1 y:0 s:1 neg:0
		// _transform(C, B, 1, 0, 1, 0);
		east(C, B);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		north(D, C);
		sub(B, B, D);
		add(C, C, B);
		// [B] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(B, B, -1, 0, 0, 0);
		west(B, B);
		add(B, B, C);
		// ----------------------------------------------------
	});
	conv_1_3_id = i;
	i++;

	//Convolution: Filter 1.4, source/target: AREG B
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:0 y:-1 s:2 neg:0
		// _transform(B, B, 0, -1, 2, 0);
		south(B, B);
		div2_inplace(B, B);
		div2_inplace(B, B);
		// [C] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(C, B, 1, 0, 0, 0);
		east(C, B);
		// [B] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(B, B, -1, 0, 0, 0);
		west(B, B);
		add(B, B, C);
		// [C] -> [C] || x:0 y:2 s:0 neg:1
		// _transform(C, C, 0, 2, 0, 1);
		north(C, C);
		north(C, C);
		neg_inplace(C, C);
		add(B, B, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		add(B, B, C);
		// ----------------------------------------------------
	});
	conv_1_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		copy_areg(C,A);
	});
	copy_areg_C_A_id = i;
	i++;

	// Convolution: Filter 2.1.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:1 s:0 neg:0
		// _transform(D, C, 1, 1, 0, 0);
		double_north(D, C);
		double_east(D, D);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:0 s:1 neg:1
		// _transform(D, D, 0, 0, 1, 1);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:1 neg:1
		// _transform(D, D, -1, 0, 1, 1);
		double_west(D, D);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:-2 s:0 neg:0
		// _transform(D, D, -1, -2, 0, 0);
		double_south(D, D);
		double_south(D, D);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:1
		// _transform(D, D, 1, 0, 0, 1);
		double_east(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(E, D, 1, 0, 0, 0);
		double_east(E, D);
		sub(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		double_north(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_1_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R1);
	});
	partial_copy_B_C_R1_id = i;
	i++;

	// Convolution: Filter 2.1.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(C, C, 0, 0, 1, 0);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(D, C, 0, 0, 1, 0);
		div2(D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:1
		// _transform(D, D, 0, 1, 0, 1);
		double_north(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:1
		// _transform(D, D, 1, 0, 0, 1);
		double_east(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:-2 s:0 neg:0
		// _transform(E, D, 0, -2, 0, 0);
		double_south(E, D);
		double_south(E, E);
		add(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:-1 y:-1 s:0 neg:0
		// _transform(C, C, -1, -1, 0, 0);
		double_south(C, C);
		double_west(C, C);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_1_2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R2);
	});
	partial_copy_B_C_R2_id = i;
	i++;

	// Convolution: Filter 2.1.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:1 y:0 s:0 neg:1
		// _transform(C, C, 1, 0, 0, 1);
		double_east(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(D, C, 0, -1, 0, 0);
		double_south(D, C);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:0 s:0 neg:1
		// _transform(D, D, 0, 0, 0, 1);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:0 s:2 neg:1
		// _transform(D, D, 0, 0, 2, 1);
		div2_inplace(D, D);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:2 s:0 neg:0
		// _transform(D, D, -1, 2, 0, 0);
		double_north(D, D);
		double_north(D, D);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:-2 s:0 neg:1
		// _transform(D, D, -1, -2, 0, 1);
		double_south(D, D);
		double_south(D, D);
		double_west(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:2 y:1 s:0 neg:0
		// _transform(D, D, 2, 1, 0, 0);
		double_north(D, D);
		double_east(D, D);
		double_east(D, D);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		double_north(E, D);
		add(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:-2 y:-1 s:0 neg:0
		// _transform(D, D, -2, -1, 0, 0);
		double_south(D, D);
		double_west(D, D);
		double_west(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_1_3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R3);
	});
	partial_copy_B_C_R3_id = i;
	i++;

	// Convolution: Filter 2.1.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(C, C, 0, 0, 1, 0);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:1 neg:0
		// _transform(D, C, 0, 1, 1, 0);
		double_north(D, C);
		div2_inplace(D, D);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		double_west(C, C);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		double_east(D, D);
		// [E] -> [D] || x:0 y:-2 s:0 neg:0
		// _transform(E, D, 0, -2, 0, 0);
		double_south(E, D);
		double_south(E, E);
		sub(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		sub_inplace(C, D, C);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_1_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R4);
	});
	partial_copy_B_C_R4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	mov(F,B,west);
    	add(B,B,F);
    	mov(F,B,south);
    	add(B,B,F);

    	neg_inplace(B, B);	///////////////////////////////////////// Inverted sign here!!!
	});
	sum_conv_2_1_id = i;
	i++;
}

// Load the kernels for the second part of the convolutions,
// 					conv_2_2_1 to conv_2_4_4
// with all util operations included.
// (33 kernels total) loaded in the global array array_kernels,
// accessible by their id *kernel name*_id
void fill_kernels_part_2(){
	array_kernels = new scamp5_kernel[33];
	uint8_t i = 0;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	WHERE(R2);
    		res(B);
    	ALL();
	});
	res_B_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		copy_areg(C,A);
	});
	copy_areg_C_A_id = i;
	i++;

	// Convolution: Filter 2.2.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:2 neg:0
		// _transform(C, C, 0, 0, 2, 0);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:-1 y:-1 s:0 neg:1
		// _transform(D, C, -1, -1, 0, 1);
		double_south(D, C);
		double_west(D, D);
		neg_inplace(D, D);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		double_north(C, C);
		add(C, C, D);
		// [D] -> [D] || x:2 y:0 s:0 neg:0
		// _transform(D, D, 2, 0, 0, 0);
		double_east(D, D);
		double_east(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_2_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,east);
	});
	shift_C_R1_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_sum(B, C, R2);
	});
	partial_sum_B_C_R2_id = i;
	i++;

	// Convolution: Filter 2.2.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		// [D] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(D, C, 0, 0, 1, 0);
		div2(D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:2 s:1 neg:0
		// _transform(D, D, 0, 2, 1, 0);
		double_north(D, D);
		double_north(D, D);
		div2_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:1
		// _transform(D, D, 1, 0, 0, 1);
		double_east(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:1 y:-1 s:0 neg:0
		// _transform(E, D, 1, -1, 0, 0);
		double_south(E, D);
		double_east(E, E);
		sub(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(D, D, 0, -1, 0, 0);
		double_south(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_2_2_id = i;
	i++;

	// Convolution: Filter 2.2.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(D, C, 0, -1, 0, 0);
		double_south(D, C);
		add_twice(D, D, D);
		// [C] -> [C] || x:1 y:0 s:1 neg:0
		// _transform(C, C, 1, 0, 1, 0);
		double_east(C, C);
		div2_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:1 neg:0
		// _transform(C, C, 0, -1, 1, 0);
		double_south(C, C);
		div2_inplace(C, C);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_2_3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	mov(C,C,east);
    	mov(C,C,south);
	});
	shift_C_R3_R2_id = i;
	i++;

	// Convolution: Filter 2.2.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:1 neg:1
		// _transform(C, C, 0, 0, 1, 1);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:-1 y:0 s:1 neg:0
		// _transform(D, C, -1, 0, 1, 0);
		double_west(D, C);
		div2_inplace(D, D);
		// [E] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(E, D, 0, -1, 0, 0);
		double_south(E, D);
		sub(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:1 y:1 s:1 neg:0
		// _transform(C, C, 1, 1, 1, 0);
		double_north(C, C);
		double_east(C, C);
		div2_inplace(C, C);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_2_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,south);
	});
	shift_C_R4_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	WHERE(R3);
    		res(B);
    	ALL();
	});
	res_B_R3_id = i;
	i++;

	// Convolution: Filter 2.3.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:1 y:0 s:1 neg:1
		// _transform(C, C, 1, 0, 1, 1);
		double_east(C, C);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(D, C, 0, 0, 1, 0);
		div2(D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		add(C, C, D);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		double_north(E, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		sub(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_3_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,north);
	});
	shift_C_R1_R3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_sum(B, C, R3);
	});
	partial_sum_B_C_R3_id = i;
	i++;

	// Convolution: Filter 2.3.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:-1 s:2 neg:0
		// _transform(C, C, 0, -1, 2, 0);
		double_south(C, C);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:-1 y:0 s:0 neg:1
		// _transform(D, C, -1, 0, 0, 1);
		double_west(D, C);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		double_north(E, D);
		add(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:1
		// _transform(C, C, 1, 0, 0, 1);
		double_east(C, C);
		neg_inplace(C, C);
		add(D, D, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:2 s:0 neg:0
		// _transform(C, C, 0, 2, 0, 0);
		double_north(C, C);
		double_north(C, C);
		sub_inplace(C, D, C);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_3_2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,west);
		mov(C,C,north);
	});
	shift_C_R2_R3_id = i;
	i++;

	// Convolution: Filter 2.3.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(C, C, 0, 0, 1, 0);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:1 neg:0
		// _transform(D, C, 0, 1, 1, 0);
		double_north(D, C);
		div2_inplace(D, D);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		add(C, C, D);
		// [E] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(E, D, -1, 0, 0, 0);
		double_west(E, D);
		add(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(D, D, 0, -1, 0, 0);
		double_south(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(D, D, 0, -1, 0, 0);
		double_south(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_3_3_id = i;
	i++;

	// Convolution: Filter 2.3.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:2 neg:1
		// _transform(C, C, 0, 0, 2, 1);
		div2_inplace(C, C);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:-1 y:1 s:0 neg:0
		// _transform(D, C, -1, 1, 0, 0);
		double_north(D, C);
		double_west(D, D);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:1
		// _transform(D, D, 0, -1, 0, 1);
		double_south(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(D, D, 0, -1, 0, 0);
		double_south(D, D);
		add(C, C, D);
		// [D] -> [D] || x:2 y:0 s:0 neg:0
		// _transform(D, D, 2, 0, 0, 0);
		double_east(D, D);
		double_east(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_3_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,west);
	});
	shift_C_R4_R3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	WHERE(R4);
    		res(B);
    	ALL();
	});
	res_B_R4_id = i;
	i++;

	// Convolution: Filter 2.4.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:-1 y:0 s:2 neg:0
		// _transform(C, C, -1, 0, 2, 0);
		double_west(C, C);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(D, C, 0, 0, 0, 1);
		neg(D, C);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		double_north(E, D);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		add(C, C, E);
		// [E] -> [C] || x:2 y:0 s:0 neg:0
		// _transform(E, C, 2, 0, 0, 0);
		double_east(E, C);
		double_east(E, E);
		add(C, C, E);
		add(C, C, D);
		// [D] -> [D] || x:1 y:1 s:0 neg:0
		// _transform(D, D, 1, 1, 0, 0);
		double_north(D, D);
		double_east(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_4_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,north);
		mov(C,C,east);
	});
	shift_C_R1_R4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_sum(B, C, R4);
	});
	partial_sum_B_C_R4_id = i;
	i++;

	// Convolution: Filter 2.4.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:2 neg:0
		// _transform(C, C, 0, 0, 2, 0);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:1 y:1 s:0 neg:0
		// _transform(D, C, 1, 1, 0, 0);
		double_north(D, C);
		double_east(D, D);
		add(C, C, D);
		// [E] -> [D] || x:0 y:-2 s:0 neg:0
		// _transform(E, D, 0, -2, 0, 0);
		double_south(E, D);
		double_south(E, E);
		add(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_4_2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,north);
	});
	shift_C_R2_R4_id = i;
	i++;

	// Convolution: Filter 2.4.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:2 neg:0
		// _transform(C, C, 0, 0, 2, 0);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(D, C, 0, -1, 0, 0);
		double_south(D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:1
		// _transform(D, D, -1, 0, 0, 1);
		double_west(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:2 s:0 neg:1
		// _transform(D, D, 0, 2, 0, 1);
		double_north(D, D);
		double_north(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		double_east(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_4_3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,east);
	});
	shift_C_R3_R4_id = i;
	i++;

	// Convolution: Filter 2.4.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		// [D] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(D, C, -1, 0, 0, 0);
		double_west(D, C);
		// [E] -> [D] || x:0 y:1 s:2 neg:0
		// _transform(E, D, 0, 1, 2, 0);
		double_north(E, D);
		div2_inplace(E, E);
		div2_inplace(E, E);
		sub(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:1 y:0 s:1 neg:0
		// _transform(C, C, 1, 0, 1, 0);
		double_east(C, C);
		div2_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:2 s:0 neg:0
		// _transform(C, C, 0, 2, 0, 0);
		double_north(C, C);
		double_north(C, C);
		// [E] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(E, C, -1, 0, 0, 0);
		double_west(E, C);
		sub(C, C, E);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:1 neg:0
		// _transform(C, C, 0, -1, 1, 0);
		double_south(C, C);
		div2_inplace(C, C);
		sub_inplace(C, D, C);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_4_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R1);
	});
	partial_copy_F_E_R1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R2);
	});
	partial_copy_F_E_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R3);
	});
	partial_copy_F_E_R3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R4);
	});
	partial_copy_F_E_R4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		out_thresholding(R8, B);
	});
	out_thresh_R8_B_conv21_to_conv24_id = i;
	i++;
}

// Load the kernels for the third part of the convolutions,
// 					conv_2_5_1 to conv_2_6_4
// (20 kernels total) loaded in the global array array_kernels,
// accessible by their id *conv name*_id
void fill_kernels_part_3(){
	array_kernels = new scamp5_kernel[20];
	uint8_t i = 0;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		res(B);
	});
	res_B_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		copy_areg(C,A);
	});
	copy_areg_C_A_id = i;
	i++;

	// Convolution: Filter 2.5.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:-1 y:1 s:0 neg:1
		// _transform(C, C, -1, 1, 0, 1);
		double_north(C, C);
		double_west(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:2 neg:1
		// _transform(D, C, 0, 0, 2, 1);
		div2(D, C);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-2 s:0 neg:0
		// _transform(D, D, 0, -2, 0, 0);
		double_south(D, D);
		double_south(D, D);
		add(C, C, D);
		// [D] -> [D] || x:2 y:0 s:0 neg:0
		// _transform(D, D, 2, 0, 0, 0);
		double_east(D, D);
		double_east(D, D);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		// [E] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(E, D, -1, 0, 0, 0);
		double_west(E, D);
		add(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:1
		// _transform(D, D, 0, 1, 0, 1);
		double_north(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:1 s:0 neg:0
		// _transform(D, D, 1, 1, 0, 0);
		double_north(D, D);
		double_east(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_5_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R1);
	});
	partial_copy_B_C_R1_id = i;
	i++;

	// Convolution: Filter 2.5.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:-1 s:2 neg:0
		// _transform(C, C, 0, -1, 2, 0);
		double_south(C, C);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(D, C, 1, 0, 0, 0);
		double_east(D, C);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:2 s:0 neg:1
		// _transform(D, D, 0, 2, 0, 1);
		double_north(D, D);
		double_north(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:1
		// _transform(D, D, -1, 0, 0, 1);
		double_west(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(D, D, 0, -1, 0, 0);
		double_south(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(D, D, 0, -1, 0, 0);
		double_south(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_5_2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R2);
	});
	partial_copy_B_C_R2_id = i;
	i++;

	// Convolution: Filter 2.5.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:-1 y:0 s:0 neg:1
		// _transform(C, C, -1, 0, 0, 1);
		double_west(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:1 neg:0
		// _transform(D, C, 0, 1, 1, 0);
		double_north(D, C);
		div2_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:2 y:0 s:1 neg:1
		// _transform(D, D, 2, 0, 1, 1);
		double_east(D, D);
		double_east(D, D);
		div2_inplace(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(E, D, 0, -1, 0, 0);
		double_south(E, D);
		sub(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:-1 s:0 neg:0
		// _transform(D, D, -1, -1, 0, 0);
		double_south(D, D);
		double_west(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_5_3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R3);
	});
	partial_copy_B_C_R3_id = i;
	i++;

	// Convolution: Filter 2.5.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:-1 s:2 neg:0
		// _transform(C, C, 0, -1, 2, 0);
		double_south(C, C);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:1 y:0 s:0 neg:1
		// _transform(D, C, 1, 0, 0, 1);
		double_east(D, C);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		double_north(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		double_north(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-2 y:0 s:0 neg:0
		// _transform(D, D, -2, 0, 0, 0);
		double_west(D, D);
		double_west(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_5_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(B,C,R4);
	});
	partial_copy_B_C_R4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	mov(F,B,west);
    	add(B,B,F);
    	mov(F,B,south);
    	add(B,B,F);
	});
	sum_conv_2_5_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	WHERE(R2);
    		res(B);
    	ALL();
	});
	res_B_R2_id = i;
	i++;

	// Convolution: Filter 2.6.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		// [D] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(D, C, 0, 0, 1, 0);
		div2(D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:1 neg:0
		// _transform(D, D, -1, 0, 1, 0);
		double_west(D, D);
		div2_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:2 s:0 neg:0
		// _transform(D, D, 0, 2, 0, 0);
		double_north(D, D);
		double_north(D, D);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		double_east(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:1
		// _transform(D, D, 0, -1, 0, 1);
		double_south(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:-1 s:0 neg:0
		// _transform(D, D, 1, -1, 0, 0);
		double_south(D, D);
		double_east(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_6_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,east);
	});
	shift_C_R1_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_sum(B, C, R2);
	});
	partial_sum_B_C_R2_id = i;
	i++;

	// Convolution: Filter 2.6.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:-1 s:0 neg:1
		// _transform(C, C, 0, -1, 0, 1);
		double_south(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(D, C, -1, 0, 0, 0);
		double_west(D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:2 neg:1
		// _transform(D, D, 0, 1, 2, 1);
		double_north(D, D);
		div2_inplace(D, D);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:-1 s:0 neg:1
		// _transform(D, D, 1, -1, 0, 1);
		double_south(D, D);
		double_east(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:1
		// _transform(D, D, 1, 0, 0, 1);
		double_east(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:1
		// _transform(D, D, 0, 1, 0, 1);
		double_north(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		double_north(E, D);
		add(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:-2 y:-1 s:0 neg:0
		// _transform(D, D, -2, -1, 0, 0);
		double_south(D, D);
		double_west(D, D);
		double_west(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_6_2_id = i;
	i++;

	// Convolution: Filter 2.6.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [D] -> [C] || x:0 y:-1 s:1 neg:1
		// _transform(D, C, 0, -1, 1, 1);
		double_south(D, C);
		div2_inplace(D, D);
		neg_inplace(D, D);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		double_north(C, C);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		double_east(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:1 neg:0
		// _transform(D, D, 0, 1, 1, 0);
		double_north(D, D);
		div2_inplace(D, D);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		double_north(E, D);
		sub(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_6_3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	mov(C,C,east);
    	mov(C,C,south);
	});
	shift_C_R3_R2_id = i;
	i++;

	// Convolution: Filter 2.6.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(C, C, 0, 0, 1, 0);
		div2_inplace(C, C);
		// [D] -> [C] || x:1 y:-1 s:0 neg:1
		// _transform(D, C, 1, -1, 0, 1);
		double_south(D, C);
		double_east(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:0 s:1 neg:1
		// _transform(D, D, 0, 0, 1, 1);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-2 y:0 s:0 neg:0
		// _transform(D, D, -2, 0, 0, 0);
		double_west(D, D);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		double_north(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_6_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,south);
	});
	shift_C_R4_R2_id = i;
	i++;
}

// Load the kernels for the fourth part of the convolutions,
// 					conv_2_7_1 to conv_2_8_4
// (24 kernels total) loaded in the global array array_kernels,
// accessible by their id *conv name*_id
void fill_kernels_part_4(){
	array_kernels = new scamp5_kernel[24];
	uint8_t i = 0;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		copy_areg(C,A);
	});
	copy_areg_C_A_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	WHERE(R3);
    		res(B);
    	ALL();
	});
	res_B_R3_id = i;
	i++;

	// Convolution: Filter 2.7.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:-1 y:0 s:0 neg:1
		// _transform(C, C, -1, 0, 0, 1);
		double_west(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:2 neg:0
		// _transform(D, C, 0, 0, 2, 0);
		div2(D, C);
		div2_inplace(D, D);
		// [E] -> [D] || x:2 y:-1 s:0 neg:0
		// _transform(E, D, 2, -1, 0, 0);
		double_south(E, D);
		double_east(E, E);
		double_east(E, E);
		sub(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:1 s:1 neg:0
		// _transform(C, C, 0, 1, 1, 0);
		double_north(C, C);
		div2_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:2 y:1 s:0 neg:0
		// _transform(C, C, 2, 1, 0, 0);
		double_north(C, C);
		double_east(C, C);
		double_east(C, C);
		add(D, D, C);
		// [C] -> [C] || x:-2 y:0 s:1 neg:1
		// _transform(C, C, -2, 0, 1, 1);
		double_west(C, C);
		double_west(C, C);
		div2_inplace(C, C);
		neg_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:1 y:-1 s:0 neg:0
		// _transform(C, C, 1, -1, 0, 0);
		double_south(C, C);
		double_east(C, C);
		add(D, D, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		double_west(C, C);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_7_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,north);
	});
	shift_C_R1_R3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_sum(B, C, R3);
	});
	partial_sum_B_C_R3_id = i;
	i++;

	// Convolution: Filter 2.7.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:1 s:2 neg:0
		// _transform(D, C, 1, 1, 2, 0);
		double_north(D, C);
		double_east(D, D);
		div2_inplace(D, D);
		div2_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:-2 s:0 neg:1
		// _transform(D, D, -1, -2, 0, 1);
		double_south(D, D);
		double_south(D, D);
		double_west(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_7_2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,west);
		mov(C,C,north);
	});
	shift_C_R2_R3_id = i;
	i++;

	// Convolution: Filter 2.7.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(C, C, 0, 0, 1, 0);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(D, C, 0, 0, 1, 0);
		div2(D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:1
		// _transform(D, D, 0, -1, 0, 1);
		double_south(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:2 s:0 neg:0
		// _transform(D, D, 0, 2, 0, 0);
		double_north(D, D);
		double_north(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_7_3_id = i;
	i++;

	// Convolution: Filter 2.7.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:2 neg:1
		// _transform(C, C, 0, 0, 2, 1);
		div2_inplace(C, C);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:-1 y:0 s:0 neg:1
		// _transform(D, C, -1, 0, 0, 1);
		double_west(D, C);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		double_north(E, D);
		add(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		// [E] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(E, C, -1, 0, 0, 0);
		double_west(E, C);
		add(C, C, E);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		double_east(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_7_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,west);
	});
	shift_C_R4_R3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
    	WHERE(R4);
    		res(B);
    	ALL();
	});
	res_B_R4_id = i;
	i++;

	// Convolution: Filter 2.8.1, source/target: AREG C, position 1
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R1);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:1 y:0 s:1 neg:1
		// _transform(C, C, 1, 0, 1, 1);
		double_east(C, C);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:-1 s:1 neg:0
		// _transform(D, C, 0, -1, 1, 0);
		double_south(D, C);
		div2_inplace(D, D);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		double_north(C, C);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:1
		// _transform(D, D, -1, 0, 0, 1);
		double_west(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		double_west(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_8_1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,north);
		mov(C,C,east);
	});
	shift_C_R1_R4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_sum(B, C, R4);
	});
	partial_sum_B_C_R4_id = i;
	i++;

	// Convolution: Filter 2.8.2, source/target: AREG C, position 2
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R2);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		double_east(C, C);
		// [D] -> [C] || x:0 y:1 s:1 neg:1
		// _transform(D, C, 0, 1, 1, 1);
		double_north(D, C);
		div2_inplace(D, D);
		neg_inplace(D, D);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		add(C, C, D);
		// [D] -> [D] || x:-2 y:0 s:0 neg:0
		// _transform(D, D, -2, 0, 0, 0);
		double_west(D, D);
		double_west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:0 s:1 neg:0
		// _transform(D, D, 0, 0, 1, 0);
		div2_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:1
		// _transform(D, D, 0, -1, 0, 1);
		double_south(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:1
		// _transform(D, D, 1, 0, 0, 1);
		double_east(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(E, D, 1, 0, 0, 0);
		double_east(E, D);
		add(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:1 s:0 neg:0
		// _transform(D, D, -1, 1, 0, 0);
		double_north(D, D);
		double_west(D, D);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_8_2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,north);
	});
	shift_C_R2_R4_id = i;
	i++;

	// Convolution: Filter 2.8.3, source/target: AREG C, position 3
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R3);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:2 neg:0
		// _transform(C, C, 0, 0, 2, 0);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		double_north(D, C);
		// [E] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(E, D, -1, 0, 0, 0);
		double_west(E, D);
		add(D, D, E);
		// [E] -> [C] || x:1 y:1 s:0 neg:0
		// _transform(E, C, 1, 1, 0, 0);
		double_north(E, C);
		double_east(E, E);
		add(C, C, E);
		// [E] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(E, D, 0, -1, 0, 0);
		double_south(E, D);
		add(C, C, E);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		add(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_8_3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		mov(C,C,east);
	});
	shift_C_R3_R4_id = i;
	i++;

	// Convolution: Filter 2.8.4, source/target: AREG C, position 4
	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		CLR(R5);
		MOV(R5,R4);
		WHERE(R5);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:0 s:2 neg:0
		// _transform(C, C, 0, 0, 2, 0);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		double_north(D, C);
		add(C, C, D);
		// [E] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(E, C, -1, 0, 0, 0);
		double_west(E, C);
		add(C, C, E);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-2 s:0 neg:0
		// _transform(D, D, 0, -2, 0, 0);
		double_south(D, D);
		double_south(D, D);
		add(C, C, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		double_east(D, D);
		sub(C, C, D);
		// ----------------------------------------------------
		ALL();
		CLR(R5);
	});
	conv_2_8_4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R1);
	});
	partial_copy_F_E_R1_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R2);
	});
	partial_copy_F_E_R2_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R3);
	});
	partial_copy_F_E_R3_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		partial_copy(F, E, R4);
	});
	partial_copy_F_E_R4_id = i;
	i++;

	array_kernels[i]([]{
		using namespace scamp5_kernel_api;
		out_thresholding(R9, B);
	});
	out_thresh_R9_B_conv25_to_conv28_id = i;
	i++;
}

#endif /* KERNEL_INSTRUCTIONS_HPP_ */
