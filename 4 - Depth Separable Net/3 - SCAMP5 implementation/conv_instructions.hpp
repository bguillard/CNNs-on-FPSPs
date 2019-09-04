/*
 * conv_instructions.hpp
 *
 *  Created on: Jul 4, 2019
 *      Author: Benoît GUILLARD
 */
#ifndef CONV_INSTRUCTIONS_HPP_
#define CONV_INSTRUCTIONS_HPP_

#include <scamp5.hpp>
using namespace SCAMP5_PE;

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

#if LAYERS == 1
// ONE CONV LAYER version:

//Convolution: Filter A
inline void conv_A(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(A, A, 0, 0, 1, 0);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:-1 s:0 neg:1
		// _transform(B, A, 0, -1, 0, 1);
		south(B, A);
		neg_inplace(B, B);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		// [C] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(C, B, 1, 0, 0, 0);
		east(C, B);
		add(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:-1 y:0 s:1 neg:0
		// _transform(B, B, -1, 0, 1, 0);
		west(B, B);
		div2_inplace(B, B);
		add(A, A, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		sub(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshA = 15;

//Convolution: Filter B
inline void conv_B(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:0 s:1 neg:1
		// _transform(A, A, 1, 0, 1, 1);
		east(A, A);
		div2_inplace(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(B, A, 0, 1, 0, 0);
		north(B, A);
		add_twice(C, B, B);
		// [D] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(D, B, 0, 0, 1, 1);
		div2(D, B);
		neg_inplace(D, D);
		// [B] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(B, D, -1, 0, 0, 0);
		west(B, D);
		add(A, A, B);
		add(B, C, D);
		// [C] -> [D] || x:-2 y:-2 s:0 neg:0
		// _transform(C, D, -2, -2, 0, 0);
		south(C, D);
		south(C, C);
		west(C, C);
		west(C, C);
		sub(B, B, C);
		add(C, B, A);
		// [B] -> [A] || x:-1 y:-1 s:0 neg:0
		// _transform(B, A, -1, -1, 0, 0);
		south(B, A);
		west(B, B);
		add(B, B, C);
		add(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshB = 25;

//Convolution: Filter C
inline void conv_C(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:1 s:0 neg:0
		// _transform(A, A, 1, 1, 0, 0);
		north(A, A);
		east(A, A);
		// [B] -> [A] || x:-1 y:0 s:2 neg:1
		// _transform(B, A, -1, 0, 2, 1);
		west(B, A);
		div2_inplace(B, B);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:-2 s:0 neg:1
		// _transform(C, B, 0, -2, 0, 1);
		south(C, B);
		south(C, C);
		neg_inplace(C, C);
		add_twice(D, C, C);
		add(D, D, C);
		// [C] -> [C] || x:-1 y:1 s:0 neg:1
		// _transform(C, C, -1, 1, 0, 1);
		north(C, C);
		west(C, C);
		neg_inplace(C, C);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [E] -> [D] || x:1 y:1 s:0 neg:0
		// _transform(E, D, 1, 1, 0, 0);
		north(E, D);
		east(E, E);
		add(D, D, E);
		add(D, D, A);
		// [A] -> [A] || x:-2 y:-2 s:2 neg:0
		// _transform(A, A, -2, -2, 2, 0);
		south(A, A);
		south(A, A);
		west(A, A);
		west(A, A);
		div2_inplace(A, A);
		div2_inplace(A, A);
		// [E] -> [C] || x:2 y:-1 s:0 neg:0
		// _transform(E, C, 2, -1, 0, 0);
		south(E, C);
		east(E, E);
		east(E, E);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		north(C, C);
		add(C, C, E);
		add(C, C, A);
		// [A] -> [A] || x:2 y:0 s:0 neg:0
		// _transform(A, A, 2, 0, 0, 0);
		east(A, A);
		east(A, A);
		add(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:1 y:-1 s:0 neg:0
		// _transform(B, B, 1, -1, 0, 0);
		south(B, B);
		east(B, B);
		sub_inplace(B, D, B);
		add(A, A, B);
		// [B] -> [B] || x:0 y:0 s:2 neg:0
		// _transform(B, B, 0, 0, 2, 0);
		div2_inplace(B, B);
		div2_inplace(B, B);
		sub(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshC = 60;
#endif

#if LAYERS == 2
// TWO DEPTH SEPARABLE LAYERS version

//Convolution: Filter 1.1
inline void conv_1_1(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:-1 s:1 neg:1
		// _transform(A, A, 0, -1, 1, 1);
		south(A, A);
		div2_inplace(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:1 y:0 s:0 neg:1
		// _transform(B, A, 1, 0, 0, 1);
		east(B, A);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [D] -> [A] || x:0 y:2 s:0 neg:0
		// _transform(D, A, 0, 2, 0, 0);
		north(D, A);
		north(D, D);
		add(A, A, D);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		north(D, C);
		sub_inplace(D, C, D);
		add(D, D, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		add(B, B, D);
		add(B, B, A);
		// [D] -> [A] || x:-1 y:0 s:1 neg:0
		// _transform(D, A, -1, 0, 1, 0);
		west(D, A);
		div2_inplace(D, D);
		// [A] -> [C] || x:-2 y:1 s:0 neg:0
		// _transform(A, C, -2, 1, 0, 0);
		north(A, C);
		west(A, A);
		west(A, A);
		sub(A, A, D);
		add(B, B, A);
		// [A] -> [A] || x:2 y:0 s:0 neg:0
		// _transform(A, A, 2, 0, 0, 0);
		east(A, A);
		east(A, A);
		add(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.1
inline void conv_2_1(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		// [B] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(B, A, 0, 1, 0, 0);
		north(B, A);
		// [C] -> [B] || x:0 y:0 s:2 neg:1
		// _transform(C, B, 0, 0, 2, 1);
		div2(C, B);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(D, C, 0, -1, 0, 0);
		south(D, C);
		add(C, C, D);
		// [D] -> [D] || x:0 y:-1 s:0 neg:1
		// _transform(D, D, 0, -1, 0, 1);
		south(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(E, D, -1, 0, 0, 0);
		west(E, D);
		add(C, C, E);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		east(D, D);
		add(D, D, B);
		// [B] -> [B] || x:1 y:0 s:2 neg:0
		// _transform(B, B, 1, 0, 2, 0);
		east(B, B);
		div2_inplace(B, B);
		div2_inplace(B, B);
		sub_inplace(B, D, B);
		add(B, B, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		add(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(B, C, 1, 0, 0, 0);
		east(B, C);
		add(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshA = 25;

//Convolution: Filter 1.2
inline void conv_1_2(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(A, A, 0, 0, 1, 0);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		// [C] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(C, B, -1, 0, 0, 0);
		west(C, B);
		add(A, A, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(A, A, 0, -1, 0, 0);
		south(A, A);
		sub_inplace(A, C, A);
		add(B, B, A);
		// [A] -> [A] || x:1 y:1 s:0 neg:0
		// _transform(A, A, 1, 1, 0, 0);
		north(A, A);
		east(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.2
inline void conv_2_2(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		// [B] -> [A] || x:-1 y:1 s:0 neg:0
		// _transform(B, A, -1, 1, 0, 0);
		north(B, A);
		west(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		// [D] -> [C] || x:1 y:-1 s:1 neg:1
		// _transform(D, C, 1, -1, 1, 1);
		south(D, C);
		east(D, D);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:1 s:0 neg:0
		// _transform(D, D, -1, 1, 0, 0);
		north(D, D);
		west(D, D);
		sub(C, C, D);
		// [D] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(D, B, 0, 0, 1, 1);
		div2(D, B);
		neg_inplace(D, D);
		// [E] -> [A] || x:-1 y:0 s:2 neg:0
		// _transform(E, A, -1, 0, 2, 0);
		west(E, A);
		div2_inplace(E, E);
		div2_inplace(E, E);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		add(A, A, E);
		add(C, C, D);
		// [D] -> [D] || x:2 y:0 s:1 neg:0
		// _transform(D, D, 2, 0, 1, 0);
		east(D, D);
		east(D, D);
		div2_inplace(D, D);
		add(C, C, D);
		// [D] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(D, B, 1, 0, 0, 0);
		east(D, B);
		add(C, C, D);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		add(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshB = 30;

//Convolution: Filter 1.3
inline void conv_1_3(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:1
		// _transform(B, A, 0, 0, 1, 1);
		div2(B, A);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:-1 s:1 neg:1
		// _transform(C, B, 0, -1, 1, 1);
		south(C, B);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(D, C, 0, 0, 0, 1);
		neg(D, C);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		add(B, B, D);
		sub(B, B, D);
		add(B, B, A);
		// [A] -> [A] || x:0 y:-1 s:1 neg:0
		// _transform(A, A, 0, -1, 1, 0);
		south(A, A);
		div2_inplace(A, A);
		add(A, A, B);
		// [B] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(B, C, -1, 0, 0, 0);
		west(B, C);
		add(A, A, B);
		add(A, A, C);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.3
inline void conv_2_3(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(B, A, 0, 0, 2, 0);
		div2(B, A);
		div2_inplace(B, B);
		// [C] -> [B] || x:1 y:0 s:0 neg:1
		// _transform(C, B, 1, 0, 0, 1);
		east(C, B);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(D, C, 0, -1, 0, 0);
		south(D, C);
		sub(C, C, D);
		// [D] -> [A] || x:-1 y:-1 s:0 neg:0
		// _transform(D, A, -1, -1, 0, 0);
		south(D, A);
		west(D, D);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		sub(B, B, D);
		add(A, A, C);
		// [C] -> [C] || x:-2 y:0 s:0 neg:0
		// _transform(C, C, -2, 0, 0, 0);
		west(C, C);
		west(C, C);
		add(B, B, C);
		add(A, A, B);
		// [B] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(B, B, 1, 0, 0, 0);
		east(B, B);
		add(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();

	const int threshC = 15;
}
#endif

#if LAYERS == 3
// THREE DEPTH SEPARABLE LAYERS version

//Convolution: Filter 1.1
inline void conv_1_1(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:-1 y:1 s:0 neg:0
	// _transform(A, A, -1, 1, 0, 0);
	north(A, A);
	west(A, A);
	// [B] -> [A] || x:0 y:-2 s:1 neg:0
	// _transform(B, A, 0, -2, 1, 0);
	south(B, A);
	south(B, B);
	div2_inplace(B, B);
	// [C] -> [B] || x:0 y:1 s:1 neg:0
	// _transform(C, B, 0, 1, 1, 0);
	north(C, B);
	div2_inplace(C, C);
	// [D] -> [C] || x:1 y:0 s:0 neg:1
	// _transform(D, C, 1, 0, 0, 1);
	east(D, C);
	neg_inplace(D, D);
	add(C, C, B);
	// [B] -> [B] || x:1 y:0 s:0 neg:0
	// _transform(B, B, 1, 0, 0, 0);
	east(B, B);
	add(C, C, D);
	// [E] -> [A] || x:0 y:0 s:2 neg:0
	// _transform(E, A, 0, 0, 2, 0);
	div2(E, A);
	div2_inplace(E, E);
	sub(C, C, E);
	add(A, A, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	sub(A, A, B);
	// [B] -> [D] || x:0 y:-1 s:0 neg:0
	// _transform(B, D, 0, -1, 0, 0);
	south(B, D);
	add(A, A, B);
	add(A, A, D);
	add(A, A, C);
	// [B] -> [C] || x:1 y:0 s:0 neg:0
	// _transform(B, C, 1, 0, 0, 0);
	east(B, C);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.1
inline void conv_2_1(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(A, A, -1, 0, 0, 0);
	west(A, A);
	// [B] -> [A] || x:0 y:-1 s:1 neg:1
	// _transform(B, A, 0, -1, 1, 1);
	south(B, A);
	div2_inplace(B, B);
	neg_inplace(B, B);
	// [C] -> [B] || x:0 y:0 s:1 neg:0
	// _transform(C, B, 0, 0, 1, 0);
	div2(C, B);
	add(B, B, C);
	// [C] -> [C] || x:0 y:0 s:0 neg:1
	// _transform(C, C, 0, 0, 0, 1);
	neg_inplace(C, C);
	// [D] -> [C] || x:1 y:0 s:0 neg:0
	// _transform(D, C, 1, 0, 0, 0);
	east(D, C);
	// [E] -> [D] || x:1 y:0 s:0 neg:0
	// _transform(E, D, 1, 0, 0, 0);
	east(E, D);
	add(C, C, E);
	// [D] -> [D] || x:0 y:2 s:0 neg:0
	// _transform(D, D, 0, 2, 0, 0);
	north(D, D);
	north(D, D);
	add(D, D, A);
	// [A] -> [A] || x:0 y:0 s:2 neg:0
	// _transform(A, A, 0, 0, 2, 0);
	div2_inplace(A, A);
	div2_inplace(A, A);
	sub_inplace(A, D, A);
	add(D, C, B);
	sub(D, D, B);
	// [B] -> [C] || x:0 y:2 s:0 neg:0
	// _transform(B, C, 0, 2, 0, 0);
	north(B, C);
	north(B, B);
	add(B, B, D);
	add(B, B, C);
	add(B, B, A);
	// [A] -> [A] || x:0 y:-1 s:0 neg:0
	// _transform(A, A, 0, -1, 0, 0);
	south(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 3.1
inline void conv_3_1(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:-1 y:0 s:1 neg:0
	// _transform(A, A, -1, 0, 1, 0);
	west(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:0 neg:1
	// _transform(B, A, 0, 0, 0, 1);
	neg(B, A);
	// [C] -> [B] || x:0 y:-1 s:0 neg:0
	// _transform(C, B, 0, -1, 0, 0);
	south(C, B);
	add_twice(C, C, C);
	// [D] -> [B] || x:0 y:0 s:1 neg:0
	// _transform(D, B, 0, 0, 1, 0);
	div2(D, B);
	add(B, B, D);
	// [D] -> [D] || x:0 y:0 s:0 neg:1
	// _transform(D, D, 0, 0, 0, 1);
	neg_inplace(D, D);
	add(B, B, A);
	// [A] -> [A] || x:1 y:1 s:0 neg:1
	// _transform(A, A, 1, 1, 0, 1);
	north(A, A);
	east(A, A);
	neg_inplace(A, A);
	// [E] -> [D] || x:0 y:-1 s:0 neg:0
	// _transform(E, D, 0, -1, 0, 0);
	south(E, D);
	add(D, D, E);
	add(D, D, C);
	// [C] -> [C] || x:1 y:0 s:1 neg:1
	// _transform(C, C, 1, 0, 1, 1);
	east(C, C);
	div2_inplace(C, C);
	neg_inplace(C, C);
	add(B, B, A);
	// [A] -> [A] || x:-1 y:-1 s:0 neg:0
	// _transform(A, A, -1, -1, 0, 0);
	south(A, A);
	west(A, A);
	add(A, A, B);
	add(A, A, C);
	// [B] -> [C] || x:1 y:0 s:1 neg:0
	// _transform(B, C, 1, 0, 1, 0);
	east(B, C);
	div2_inplace(B, B);
	add(A, A, B);
	add(A, A, D);
	// [B] -> [D] || x:0 y:1 s:0 neg:0
	// _transform(B, D, 0, 1, 0, 0);
	north(B, D);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}


const int threshA = 30;

//Convolution: Filter 1.2
inline void conv_1_2(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:1 y:1 s:0 neg:1
	// _transform(A, A, 1, 1, 0, 1);
	north(A, A);
	east(A, A);
	neg_inplace(A, A);
	// [B] -> [A] || x:-1 y:-1 s:1 neg:0
	// _transform(B, A, -1, -1, 1, 0);
	south(B, A);
	west(B, B);
	div2_inplace(B, B);
	// [C] -> [B] || x:0 y:-1 s:0 neg:0
	// _transform(C, B, 0, -1, 0, 0);
	south(C, B);
	add_twice(D, C, C);
	// [C] -> [C] || x:0 y:0 s:0 neg:1
	// _transform(C, C, 0, 0, 0, 1);
	neg_inplace(C, C);
	sub(D, D, C);
	// [C] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(C, C, -1, 0, 0, 0);
	west(C, C);
	add(C, C, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	sub_inplace(B, C, B);
	// [C] -> [D] || x:0 y:0 s:1 neg:1
	// _transform(C, D, 0, 0, 1, 1);
	div2(C, D);
	neg_inplace(C, C);
	add(D, D, A);
	// [A] -> [A] || x:-2 y:0 s:2 neg:1
	// _transform(A, A, -2, 0, 2, 1);
	west(A, A);
	west(A, A);
	div2_inplace(A, A);
	div2_inplace(A, A);
	neg_inplace(A, A);
	add(D, D, C);
	sub_inplace(C, D, C);
	// [D] -> [A] || x:0 y:-1 s:0 neg:0
	// _transform(D, A, 0, -1, 0, 0);
	south(D, A);
	add(C, C, D);
	add(A, A, C);
	add(A, A, B);
	// [B] -> [B] || x:1 y:0 s:1 neg:0
	// _transform(B, B, 1, 0, 1, 0);
	east(B, B);
	div2_inplace(B, B);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.2
inline void conv_2_2(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:-1 s:1 neg:1
	// _transform(A, A, 0, -1, 1, 1);
	south(A, A);
	div2_inplace(A, A);
	neg_inplace(A, A);
	// [B] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(B, A, 1, 0, 0, 0);
	east(B, A);
	// [C] -> [B] || x:0 y:1 s:1 neg:1
	// _transform(C, B, 0, 1, 1, 1);
	north(C, B);
	div2_inplace(C, C);
	neg_inplace(C, C);
	// [D] -> [C] || x:-1 y:1 s:0 neg:0
	// _transform(D, C, -1, 1, 0, 0);
	north(D, C);
	west(D, D);
	add(C, C, D);
	add(A, A, D);
	add(D, D, C);
	// [C] -> [C] || x:0 y:0 s:0 neg:1
	// _transform(C, C, 0, 0, 0, 1);
	neg_inplace(C, C);
	add(D, D, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	sub_inplace(B, D, B);
	add(B, B, C);
	// [C] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(C, C, -1, 0, 0, 0);
	west(C, C);
	sub(A, A, C);
	add(B, B, A);
	// [A] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(A, A, 1, 0, 0, 0);
	east(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 3.2
inline void conv_3_2(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:1 y:1 s:1 neg:0
	// _transform(A, A, 1, 1, 1, 0);
	north(A, A);
	east(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:0 neg:1
	// _transform(B, A, 0, 0, 0, 1);
	neg(B, A);
	// [C] -> [B] || x:0 y:0 s:1 neg:0
	// _transform(C, B, 0, 0, 1, 0);
	div2(C, B);
	add(B, B, C);
	add(B, B, C);
	add(C, C, B);
	// [B] -> [B] || x:0 y:-2 s:0 neg:0
	// _transform(B, B, 0, -2, 0, 0);
	south(B, B);
	south(B, B);
	// [D] -> [A] || x:-2 y:0 s:0 neg:1
	// _transform(D, A, -2, 0, 0, 1);
	west(D, A);
	west(D, D);
	neg_inplace(D, D);
	// [E] -> [B] || x:0 y:0 s:2 neg:0
	// _transform(E, B, 0, 0, 2, 0);
	div2(E, B);
	div2_inplace(E, E);
	sub(B, B, E);
	add(C, C, D);
	// [D] -> [D] || x:2 y:-1 s:1 neg:1
	// _transform(D, D, 2, -1, 1, 1);
	south(D, D);
	east(D, D);
	east(D, D);
	div2_inplace(D, D);
	neg_inplace(D, D);
	add(C, C, A);
	// [A] -> [A] || x:0 y:-1 s:1 neg:0
	// _transform(A, A, 0, -1, 1, 0);
	south(A, A);
	div2_inplace(A, A);
	add(C, C, D);
	// [D] -> [D] || x:0 y:1 s:0 neg:0
	// _transform(D, D, 0, 1, 0, 0);
	north(D, D);
	add(C, C, D);
	// [D] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(D, A, -1, 0, 0, 0);
	west(D, A);
	add(C, C, D);
	add(A, A, C);
	add(A, A, B);
	// [B] -> [B] || x:0 y:2 s:0 neg:0
	// _transform(B, B, 0, 2, 0, 0);
	north(B, B);
	north(B, B);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshB = 15;

//Convolution: Filter 1.3
inline void conv_1_3(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [B] -> [A] || x:0 y:0 s:1 neg:0
	// _transform(B, A, 0, 0, 1, 0);
	div2(B, A);
	// [A] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(A, A, -1, 0, 0, 0);
	west(A, A);
	add(A, A, B);
	// [B] -> [B] || x:0 y:0 s:1 neg:0
	// _transform(B, B, 0, 0, 1, 0);
	div2_inplace(B, B);
	add(C, A, B);
	// [A] -> [B] || x:1 y:0 s:0 neg:0
	// _transform(A, B, 1, 0, 0, 0);
	east(A, B);
	add(A, A, C);
	// [C] -> [C] || x:1 y:0 s:0 neg:0
	// _transform(C, C, 1, 0, 0, 0);
	east(C, C);
	add(A, A, C);
	add(A, A, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	add(B, B, A);
	// [A] -> [A] || x:0 y:0 s:2 neg:0
	// _transform(A, A, 0, 0, 2, 0);
	div2_inplace(A, A);
	div2_inplace(A, A);
	sub_inplace(A, B, A);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.3
inline void conv_2_3(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:1 y:0 s:1 neg:1
	// _transform(A, A, 1, 0, 1, 1);
	east(A, A);
	div2_inplace(A, A);
	neg_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:1 neg:0
	// _transform(B, A, 0, 0, 1, 0);
	div2(B, A);
	// [C] -> [B] || x:-2 y:0 s:0 neg:0
	// _transform(C, B, -2, 0, 0, 0);
	west(C, B);
	west(C, C);
	add(B, B, A);
	// [D] -> [C] || x:0 y:0 s:0 neg:1
	// _transform(D, C, 0, 0, 0, 1);
	neg(D, C);
	add(A, A, B);
	// [B] -> [B] || x:0 y:0 s:0 neg:1
	// _transform(B, B, 0, 0, 0, 1);
	neg_inplace(B, B);
	// [E] -> [D] || x:1 y:-1 s:0 neg:0
	// _transform(E, D, 1, -1, 0, 0);
	south(E, D);
	east(E, E);
	add(D, D, E);
	add(A, A, B);
	// [E] -> [B] || x:-2 y:1 s:0 neg:0
	// _transform(E, B, -2, 1, 0, 0);
	north(E, B);
	west(E, E);
	west(E, E);
	// [B] -> [C] || x:0 y:1 s:0 neg:0
	// _transform(B, C, 0, 1, 0, 0);
	north(B, C);
	sub(B, B, E);
	add(A, A, B);
	// [B] -> [B] || x:2 y:-1 s:0 neg:0
	// _transform(B, B, 2, -1, 0, 0);
	south(B, B);
	east(B, B);
	east(B, B);
	add(A, A, B);
	add(A, A, D);
	// [B] -> [D] || x:1 y:1 s:0 neg:0
	// _transform(B, D, 1, 1, 0, 0);
	north(B, D);
	east(B, B);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 3.3
inline void conv_3_3(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:-1 y:1 s:0 neg:0
	// _transform(A, A, -1, 1, 0, 0);
	north(A, A);
	west(A, A);
	// [B] -> [A] || x:0 y:-2 s:1 neg:1
	// _transform(B, A, 0, -2, 1, 1);
	south(B, A);
	south(B, B);
	div2_inplace(B, B);
	neg_inplace(B, B);
	// [C] -> [B] || x:2 y:0 s:0 neg:0
	// _transform(C, B, 2, 0, 0, 0);
	east(C, B);
	east(C, C);
	add(B, B, C);
	// [C] -> [C] || x:0 y:1 s:0 neg:0
	// _transform(C, C, 0, 1, 0, 0);
	north(C, C);
	// [D] -> [C] || x:0 y:0 s:1 neg:0
	// _transform(D, C, 0, 0, 1, 0);
	div2(D, C);
	sub(B, B, D);
	add(C, C, A);
	// [A] -> [A] || x:0 y:0 s:2 neg:0
	// _transform(A, A, 0, 0, 2, 0);
	div2_inplace(A, A);
	div2_inplace(A, A);
	sub_inplace(A, C, A);
	add(A, A, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshC = 25;
#endif

#if LAYERS == 4
// THREE DEPTH SEPARABLE LAYERS version

//Convolution: Filter 1.1
inline void conv_1_1(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:-1 y:-1 s:0 neg:1
	// _transform(A, A, -1, -1, 0, 1);
	south(A, A);
	west(A, A);
	neg_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:2 neg:1
	// _transform(B, A, 0, 0, 2, 1);
	div2(B, A);
	div2_inplace(B, B);
	neg_inplace(B, B);
	// [C] -> [B] || x:1 y:0 s:0 neg:1
	// _transform(C, B, 1, 0, 0, 1);
	east(C, B);
	neg_inplace(C, C);
	add(B, B, A);
	// [A] -> [A] || x:0 y:2 s:0 neg:0
	// _transform(A, A, 0, 2, 0, 0);
	north(A, A);
	north(A, A);
	// [D] -> [C] || x:0 y:1 s:0 neg:0
	// _transform(D, C, 0, 1, 0, 0);
	north(D, C);
	sub(C, C, D);
	add(C, C, A);
	// [A] -> [A] || x:2 y:0 s:2 neg:0
	// _transform(A, A, 2, 0, 2, 0);
	east(A, A);
	east(A, A);
	div2_inplace(A, A);
	div2_inplace(A, A);
	add(A, A, C);
	add(A, A, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.1
inline void conv_2_1(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:-1 s:1 neg:0
	// _transform(A, A, 0, -1, 1, 0);
	south(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:1 neg:0
	// _transform(B, A, 0, 0, 1, 0);
	div2(B, A);
	add(A, A, B);
	// [B] -> [B] || x:0 y:0 s:0 neg:1
	// _transform(B, B, 0, 0, 0, 1);
	neg_inplace(B, B);
	// [C] -> [B] || x:1 y:0 s:0 neg:1
	// _transform(C, B, 1, 0, 0, 1);
	east(C, B);
	neg_inplace(C, C);
	add(B, B, C);
	// [C] -> [C] || x:0 y:1 s:0 neg:1
	// _transform(C, C, 0, 1, 0, 1);
	north(C, C);
	neg_inplace(C, C);
	// [D] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(D, C, -1, 0, 0, 0);
	west(D, C);
	add(B, B, D);
	// [C] -> [C] || x:0 y:1 s:0 neg:0
	// _transform(C, C, 0, 1, 0, 0);
	north(C, C);
	add(C, C, A);
	// [A] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(A, A, -1, 0, 0, 0);
	west(A, A);
	sub_inplace(A, C, A);
	add(A, A, B);
	// [B] -> [B] || x:-1 y:0 s:0 neg:0
	// _transform(B, B, -1, 0, 0, 0);
	west(B, B);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 3.1
inline void conv_3_1(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:0 s:0 neg:1
	// _transform(A, A, 0, 0, 0, 1);
	neg_inplace(A, A);
	// [B] -> [A] || x:1 y:1 s:0 neg:0
	// _transform(B, A, 1, 1, 0, 0);
	north(B, A);
	east(B, B);
	add_twice(B, B, B);
	// [A] -> [A] || x:0 y:0 s:2 neg:0
	// _transform(A, A, 0, 0, 2, 0);
	div2_inplace(A, A);
	div2_inplace(A, A);
	// [C] -> [A] || x:0 y:1 s:0 neg:1
	// _transform(C, A, 0, 1, 0, 1);
	north(C, A);
	neg_inplace(C, C);
	// [D] -> [B] || x:0 y:-1 s:2 neg:0
	// _transform(D, B, 0, -1, 2, 0);
	south(D, B);
	div2_inplace(D, D);
	div2_inplace(D, D);
	add(C, C, D);
	add(B, B, C);
	// [C] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(C, C, -1, 0, 0, 0);
	west(C, C);
	// [A] -> [A] || x:0 y:-1 s:0 neg:0
	// _transform(A, A, 0, -1, 0, 0);
	south(A, A);
	add(A, A, C);
	add(B, B, A);
	// [A] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(A, A, 1, 0, 0, 0);
	east(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 4.1
inline void conv_4_1(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:0 s:2 neg:1
	// _transform(A, A, 0, 0, 2, 1);
	div2_inplace(A, A);
	div2_inplace(A, A);
	neg_inplace(A, A);
	// [B] -> [A] || x:0 y:-1 s:0 neg:0
	// _transform(B, A, 0, -1, 0, 0);
	south(B, A);
	// [C] -> [B] || x:1 y:0 s:0 neg:0
	// _transform(C, B, 1, 0, 0, 0);
	east(C, B);
	add_twice(C, C, C);
	// [B] -> [B] || x:-1 y:0 s:0 neg:1
	// _transform(B, B, -1, 0, 0, 1);
	west(B, B);
	neg_inplace(B, B);
	add(B, B, A);
	// [A] -> [A] || x:0 y:1 s:0 neg:0
	// _transform(A, A, 0, 1, 0, 0);
	north(A, A);
	add(C, C, A);
	// [A] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(A, A, 1, 0, 0, 0);
	east(A, A);
	add(A, A, C);
	add(A, A, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}


const int threshA = 30;

//Convolution: Filter 1.2
inline void conv_1_2(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:-1 s:1 neg:0
	// _transform(A, A, 0, -1, 1, 0);
	south(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:0 neg:1
	// _transform(B, A, 0, 0, 0, 1);
	neg(B, A);
	add_twice(C, B, B);
	// [D] -> [B] || x:0 y:1 s:1 neg:1
	// _transform(D, B, 0, 1, 1, 1);
	north(D, B);
	div2_inplace(D, D);
	neg_inplace(D, D);
	add(C, C, A);
	// [A] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(A, A, -1, 0, 0, 0);
	west(A, A);
	sub_inplace(A, C, A);
	add(A, A, B);
	// [B] -> [B] || x:1 y:0 s:0 neg:0
	// _transform(B, B, 1, 0, 0, 0);
	east(B, B);
	add(B, B, D);
	// [C] -> [D] || x:0 y:1 s:0 neg:0
	// _transform(C, D, 0, 1, 0, 0);
	north(C, D);
	add(B, B, C);
	add(B, B, A);
	// [A] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(A, A, 1, 0, 0, 0);
	east(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.2
inline void conv_2_2(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:1 s:1 neg:1
	// _transform(A, A, 0, 1, 1, 1);
	north(A, A);
	div2_inplace(A, A);
	neg_inplace(A, A);
	// [B] -> [A] || x:-1 y:-1 s:0 neg:0
	// _transform(B, A, -1, -1, 0, 0);
	south(B, A);
	west(B, B);
	// [C] -> [B] || x:0 y:0 s:1 neg:1
	// _transform(C, B, 0, 0, 1, 1);
	div2(C, B);
	neg_inplace(C, C);
	add(D, A, C);
	// [A] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(A, A, 1, 0, 0, 0);
	east(A, A);
	add(A, A, D);
	// [D] -> [B] || x:0 y:1 s:1 neg:0
	// _transform(D, B, 0, 1, 1, 0);
	north(D, B);
	div2_inplace(D, D);
	add(A, A, D);
	// [D] -> [C] || x:1 y:-1 s:0 neg:0
	// _transform(D, C, 1, -1, 0, 0);
	south(D, C);
	east(D, D);
	add(B, B, D);
	sub(B, B, C);
	add(B, B, A);
	// [A] -> [A] || x:0 y:-1 s:0 neg:0
	// _transform(A, A, 0, -1, 0, 0);
	south(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 3.2
inline void conv_3_2(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:0 s:0 neg:1
	// _transform(A, A, 0, 0, 0, 1);
	neg_inplace(A, A);
	// [B] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(B, A, 1, 0, 0, 0);
	east(B, A);
	// [C] -> [A] || x:0 y:0 s:2 neg:1
	// _transform(C, A, 0, 0, 2, 1);
	div2(C, A);
	div2_inplace(C, C);
	neg_inplace(C, C);
	// [D] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(D, C, -1, 0, 0, 0);
	west(D, C);
	// [E] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(E, A, -1, 0, 0, 0);
	west(E, A);
	sub_inplace(D, E, D);
	sub_inplace(A, A, A);
	sub(A, A, B);
	add(B, B, D);
	// [D] -> [D] || x:2 y:0 s:0 neg:0
	// _transform(D, D, 2, 0, 0, 0);
	east(D, D);
	east(D, D);
	add(B, B, D);
	// [D] -> [C] || x:0 y:1 s:0 neg:0
	// _transform(D, C, 0, 1, 0, 0);
	north(D, C);
	add(B, B, D);
	add(B, B, C);
	add(B, B, A);
	// [A] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(A, A, -1, 0, 0, 0);
	west(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 4.2
inline void conv_4_2(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:1 s:2 neg:0
	// _transform(A, A, 0, 1, 2, 0);
	north(A, A);
	div2_inplace(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:1 y:0 s:0 neg:0
	// _transform(B, A, 1, 0, 0, 0);
	east(B, A);
	// [C] -> [B] || x:0 y:-2 s:0 neg:0
	// _transform(C, B, 0, -2, 0, 0);
	south(C, B);
	south(C, C);
	sub_inplace(C, B, C);
	add(B, B, A);
	// [A] -> [A] || x:0 y:-1 s:0 neg:0
	// _transform(A, A, 0, -1, 0, 0);
	south(A, A);
	sub_inplace(A, B, A);
	add(B, A, C);
	// [A] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(A, A, -1, 0, 0, 0);
	west(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

const int threshB = 15;

//Convolution: Filter 1.3
inline void conv_1_3(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:1 s:1 neg:1
	// _transform(A, A, 0, 1, 1, 1);
	north(A, A);
	div2_inplace(A, A);
	neg_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:1 neg:0
	// _transform(B, A, 0, 0, 1, 0);
	div2(B, A);
	add(A, A, B);
	// [B] -> [B] || x:0 y:0 s:0 neg:1
	// _transform(B, B, 0, 0, 0, 1);
	neg_inplace(B, B);
	// [C] -> [B] || x:1 y:0 s:0 neg:1
	// _transform(C, B, 1, 0, 0, 1);
	east(C, B);
	neg_inplace(C, C);
	// [D] -> [C] || x:0 y:-1 s:0 neg:0
	// _transform(D, C, 0, -1, 0, 0);
	south(D, C);
	add(A, A, D);
	// [D] -> [D] || x:0 y:-1 s:0 neg:0
	// _transform(D, D, 0, -1, 0, 0);
	south(D, D);
	sub(C, C, D);
	add(A, A, B);
	// [B] -> [B] || x:-1 y:-1 s:0 neg:0
	// _transform(B, B, -1, -1, 0, 0);
	south(B, B);
	west(B, B);
	add(A, A, B);
	add(A, A, C);
	// [B] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(B, C, -1, 0, 0, 0);
	west(B, C);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 2.3
inline void conv_2_3(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:1 y:0 s:1 neg:0
	// _transform(A, A, 1, 0, 1, 0);
	east(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:0 neg:1
	// _transform(B, A, 0, 0, 0, 1);
	neg(B, A);
	// [C] -> [B] || x:0 y:-1 s:1 neg:0
	// _transform(C, B, 0, -1, 1, 0);
	south(C, B);
	div2_inplace(C, C);
	// [A] -> [A] || x:0 y:1 s:0 neg:0
	// _transform(A, A, 0, 1, 0, 0);
	north(A, A);
	add(A, A, C);
	// [C] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(C, C, -1, 0, 0, 0);
	west(C, C);
	add(B, B, C);
	add(A, A, B);
	// [B] -> [B] || x:-1 y:0 s:0 neg:0
	// _transform(B, B, -1, 0, 0, 0);
	west(B, B);
	sub(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 3.3
inline void conv_3_3(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:0 s:2 neg:0
	// _transform(A, A, 0, 0, 2, 0);
	div2_inplace(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:1 y:0 s:0 neg:1
	// _transform(B, A, 1, 0, 0, 1);
	east(B, A);
	neg_inplace(B, B);
	// [C] -> [B] || x:0 y:-1 s:0 neg:1
	// _transform(C, B, 0, -1, 0, 1);
	south(C, B);
	neg_inplace(C, C);
	add_twice(D, B, B);
	// [B] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(B, B, 0, 1, 0, 0);
	north(B, B);
	sub_inplace(B, C, B);
	// [C] -> [B] || x:-1 y:0 s:0 neg:0
	// _transform(C, B, -1, 0, 0, 0);
	west(C, B);
	add(B, B, D);
	// [D] -> [D] || x:0 y:-1 s:0 neg:0
	// _transform(D, D, 0, -1, 0, 0);
	south(D, D);
	sub(B, B, D);
	add(D, C, A);
	// [A] -> [C] || x:-1 y:0 s:0 neg:0
	// _transform(A, C, -1, 0, 0, 0);
	west(A, C);
	add(A, A, B);
	// [B] -> [C] || x:1 y:0 s:0 neg:0
	// _transform(B, C, 1, 0, 0, 0);
	east(B, C);
	add(A, A, B);
	add(A, A, D);
	// [B] -> [D] || x:-1 y:0 s:0 neg:0
	// _transform(B, D, -1, 0, 0, 0);
	west(B, D);
	add(A, A, B);
	// ----------------------------------------------------
	scamp5_kernel_end();
}

//Convolution: Filter 4.3
inline void conv_4_3(){
	scamp5_kernel_begin();
	// ----------------------------------------------------
	// DO NOT MODIFY! (Automatically generated kernel code)
	// [A] -> [A] || x:0 y:0 s:2 neg:0
	// _transform(A, A, 0, 0, 2, 0);
	div2_inplace(A, A);
	div2_inplace(A, A);
	// [B] -> [A] || x:0 y:0 s:0 neg:1
	// _transform(B, A, 0, 0, 0, 1);
	neg(B, A);
	// [C] -> [B] || x:0 y:1 s:0 neg:0
	// _transform(C, B, 0, 1, 0, 0);
	north(C, B);
	add(A, A, C);
	// [B] -> [B] || x:0 y:-1 s:0 neg:0
	// _transform(B, B, 0, -1, 0, 0);
	south(B, B);
	add(A, A, B);
	// [B] -> [B] || x:0 y:0 s:0 neg:1
	// _transform(B, B, 0, 0, 0, 1);
	neg_inplace(B, B);
	sub(A, A, B);
	// [B] -> [B] || x:1 y:0 s:0 neg:0
	// _transform(B, B, 1, 0, 0, 0);
	east(B, B);
	add(B, B, A);
	// [A] -> [A] || x:-1 y:0 s:0 neg:0
	// _transform(A, A, -1, 0, 0, 0);
	west(A, A);
	add(A, A, B);
	// ----------------------------------------------------
	//neg_inplace(A, A);	/////////////////////////////////// Invert sign
	scamp5_kernel_end();
}

const int threshC = 15;

#endif

#endif /* CONV_INSTRUCTIONS_HPP_ */
