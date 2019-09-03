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
#define neg_inplace(X,Y)({\
	mov(F,Y);\
	neg(X,F);\
})
#define add_twice(X, Y, Z)({\
	mov(F, Z);\
	add(X, Y, F);\
})
#define sub_inplace(X, Y, Z)({\
	sub(F, Y, Z);\
	mov(X, F);\
})


// Convolution: Filter 1.1, source/target: AREG A,
// available AREGs: A to F
inline void conv_1_1(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:-1 s:1 neg:0
		// _transform(A, A, 0, -1, 1, 0);
		south(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:-1 y:0 s:0 neg:1
		// _transform(B, A, -1, 0, 0, 1);
		west(B, A);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(C, B, 0, 0, 1, 1);
		div2(C, B);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		north(D, C);
		sub(B, B, D);
		add(C, C, A);
		// [A] -> [A] || x:1 y:2 s:1 neg:0
		// _transform(A, A, 1, 2, 1, 0);
		north(A, A);
		north(A, A);
		east(A, A);
		div2_inplace(A, A);
		add(C, C, B);
		// [B] -> [B] || x:2 y:0 s:0 neg:0
		// _transform(B, B, 2, 0, 0, 0);
		east(B, B);
		east(B, B);
		sub(A, A, B);
		add(B, C, A);
		// [A] -> [A] || x:-2 y:0 s:0 neg:0
		// _transform(A, A, -2, 0, 0, 0);
		west(A, A);
		west(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
	scamp5_kernel_end();
}
const int8_t conv1_1_bias = 0;

// Convolution: Filter 1.2, source/target: AREG B,
// available AREGs: B to F
inline void conv_1_2(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:0 y:-1 s:1 neg:0
		// _transform(B, B, 0, -1, 1, 0);
		south(B, B);
		div2_inplace(B, B);
		// [C] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(C, B, 1, 0, 0, 0);
		east(C, B);
		// [D] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(D, C, 0, 0, 1, 0);
		div2(D, C);
		sub(C, C, D);
		add(D, D, C);
		// [C] -> [C] || x:0 y:2 s:0 neg:0
		// _transform(C, C, 0, 2, 0, 0);
		north(C, C);
		north(C, C);
		sub_inplace(C, D, C);
		// [D] -> [B] || x:0 y:1 s:0 neg:1
		// _transform(D, B, 0, 1, 0, 1);
		north(D, B);
		neg_inplace(D, D);
		add(B, B, D);
		// [D] -> [D] || x:-1 y:1 s:0 neg:0
		// _transform(D, D, -1, 1, 0, 0);
		north(D, D);
		west(D, D);
		add(C, C, D);
		// [D] -> [D] || x:2 y:0 s:0 neg:0
		// _transform(D, D, 2, 0, 0, 0);
		east(D, D);
		east(D, D);
		add(C, C, D);
		add(C, C, B);
		// [B] -> [B] || x:1 y:0 s:1 neg:0
		// _transform(B, B, 1, 0, 1, 0);
		east(B, B);
		div2_inplace(B, B);
		add(B, B, C);
		// ----------------------------------------------------
	scamp5_kernel_end();
}
const int8_t conv1_2_bias = 0;

// Convolution: Filter 1.3, source/target: AREG C,
// available AREGs: C to F
inline void conv_1_3(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:1 y:0 s:2 neg:0
		// _transform(C, C, 1, 0, 2, 0);
		east(C, C);
		div2_inplace(C, C);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		north(D, C);
		// [E] -> [D] || x:-2 y:0 s:0 neg:0
		// _transform(E, D, -2, 0, 0, 0);
		west(E, D);
		west(E, E);
		add(D, D, E);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:1
		// _transform(C, C, 0, -1, 0, 1);
		south(C, C);
		neg_inplace(C, C);
		add(D, D, C);
		add(D, D, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		// [E] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(E, C, 0, 1, 0, 0);
		north(E, C);
		add(C, C, E);
		add(D, D, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		add(C, C, D);
		// ----------------------------------------------------
	scamp5_kernel_end();
}
const int8_t conv1_3_bias = 1;


// Convolution: Filter 2.1.1, source/target: AREG A,
// available AREGs: D to F
inline void conv_2_1_1(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:-1 s:0 neg:0
		// _transform(A, A, 1, -1, 0, 0);
		south(A, A);
		east(A, A);
		add_twice(D, A, A);
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		add(D, D, A);
		// [A] -> [A] || x:0 y:2 s:1 neg:0
		// _transform(A, A, 0, 2, 1, 0);
		north(A, A);
		north(A, A);
		div2_inplace(A, A);
		add(D, D, A);
		// [A] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(A, A, 0, 0, 1, 0);
		div2_inplace(A, A);
		add(D, D, A);
		// [A] -> [A] || x:0 y:-1 s:0 neg:1
		// _transform(A, A, 0, -1, 0, 1);
		south(A, A);
		neg_inplace(A, A);
		add(D, D, A);
		add(D, D, A);
		// [A] -> [A] || x:-1 y:0 s:0 neg:1
		// _transform(A, A, -1, 0, 0, 1);
		west(A, A);
		neg_inplace(A, A);
		add(D, D, A);
		// [A] -> [A] || x:0 y:-1 s:0 neg:1
		// _transform(A, A, 0, -1, 0, 1);
		south(A, A);
		neg_inplace(A, A);
		add(D, D, A);
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		// [E] -> [A] || x:0 y:2 s:0 neg:0
		// _transform(E, A, 0, 2, 0, 0);
		north(E, A);
		north(E, E);
		sub(A, A, E);
		add(D, D, A);
		// [A] -> [A] || x:2 y:0 s:0 neg:0
		// _transform(A, A, 2, 0, 0, 0);
		east(A, A);
		east(A, A);
		add(A, A, D);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

// Convolution: Filter 2.1.2, source/target: AREG B,
// available AREGs: D to F
inline void conv_2_1_2(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:1 y:-1 s:0 neg:0
		// _transform(B, B, 1, -1, 0, 0);
		south(B, B);
		east(B, B);
		// [D] -> [B] || x:-1 y:2 s:2 neg:0
		// _transform(D, B, -1, 2, 2, 0);
		north(D, B);
		north(D, D);
		west(D, D);
		div2_inplace(D, D);
		div2_inplace(D, D);
		sub_inplace(D, B, D);
		add(D, D, B);
		// [B] -> [B] || x:0 y:0 s:0 neg:1
		// _transform(B, B, 0, 0, 0, 1);
		neg_inplace(B, B);
		add(D, D, B);
		// [B] -> [B] || x:-2 y:1 s:1 neg:0
		// _transform(B, B, -2, 1, 1, 0);
		north(B, B);
		west(B, B);
		west(B, B);
		div2_inplace(B, B);
		add(D, D, B);
		// [E] -> [B] || x:2 y:0 s:1 neg:0
		// _transform(E, B, 2, 0, 1, 0);
		east(E, B);
		east(E, E);
		div2_inplace(E, E);
		sub(B, B, E);
		add(D, D, B);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(B, B, D);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

// Convolution: Filter 2.1.3, source/target: AREG B,
// available AREGs: D to F
inline void conv_2_1_3(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:0 y:0 s:2 neg:0
		// _transform(B, B, 0, 0, 2, 0);
		div2_inplace(B, B);
		div2_inplace(B, B);
		// [D] -> [B] || x:-1 y:1 s:0 neg:0
		// _transform(D, B, -1, 1, 0, 0);
		north(D, B);
		west(D, D);
		add(B, B, D);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int8_t conv2_1_bias = 25;


// Convolution: Filter 2.2.1, source/target: AREG B,
// available AREGs: D to F
inline void conv_2_2_1(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		// [D] -> [B] || x:1 y:-1 s:2 neg:0
		// _transform(D, B, 1, -1, 2, 0);
		south(D, B);
		east(D, D);
		div2_inplace(D, D);
		div2_inplace(D, D);
		sub_inplace(D, B, D);
		add(D, D, B);
		// [B] -> [B] || x:-1 y:0 s:1 neg:0
		// _transform(B, B, -1, 0, 1, 0);
		west(B, B);
		div2_inplace(B, B);
		add(D, D, B);
		// [E] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(E, B, 1, 0, 0, 0);
		east(E, B);
		sub(B, B, E);
		add(D, D, B);
		// [B] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(B, B, 0, 0, 1, 1);
		div2_inplace(B, B);
		neg_inplace(B, B);
		add(D, D, B);
		// [B] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(B, B, 1, 0, 0, 0);
		east(B, B);
		add(D, D, B);
		// [B] -> [B] || x:0 y:-2 s:0 neg:0
		// _transform(B, B, 0, -2, 0, 0);
		south(B, B);
		south(B, B);
		add(D, D, B);
		// [B] -> [B] || x:0 y:2 s:0 neg:0
		// _transform(B, B, 0, 2, 0, 0);
		north(B, B);
		north(B, B);
		add(B, B, D);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

// Convolution: Filter 2.2.2, source/target: AREG C,
// available AREGs: D to F
inline void conv_2_2_2(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:0 y:1 s:2 neg:1
		// _transform(C, C, 0, 1, 2, 1);
		north(C, C);
		div2_inplace(C, C);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

// Convolution: Filter 2.2.3, source/target: AREG C,
// available AREGs: D to F
inline void conv_2_2_3(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [C] -> [C] || x:-1 y:-1 s:0 neg:1
		// _transform(C, C, -1, -1, 0, 1);
		south(C, C);
		west(C, C);
		neg_inplace(C, C);
		add_twice(D, C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(C, C, 0, 0, 1, 0);
		div2_inplace(C, C);
		add(D, D, C);
		// [E] -> [D] || x:0 y:0 s:1 neg:0
		// _transform(E, D, 0, 0, 1, 0);
		div2(E, D);
		sub(D, D, E);
		add(D, D, C);
		add(D, D, C);
		// [C] -> [C] || x:1 y:0 s:1 neg:0
		// _transform(C, C, 1, 0, 1, 0);
		east(C, C);
		div2_inplace(C, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		// [C] -> [C] || x:1 y:2 s:0 neg:0
		// _transform(C, C, 1, 2, 0, 0);
		north(C, C);
		north(C, C);
		east(C, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:1
		// _transform(C, C, 0, -1, 0, 1);
		south(C, C);
		neg_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		south(C, C);
		// [E] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(E, C, -1, 0, 0, 0);
		west(E, C);
		sub(C, C, E);
		add(D, D, C);
		// [C] -> [C] || x:-1 y:2 s:0 neg:1
		// _transform(C, C, -1, 2, 0, 1);
		north(C, C);
		north(C, C);
		west(C, C);
		neg_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-2 s:0 neg:0
		// _transform(C, C, 0, -2, 0, 0);
		south(C, C);
		south(C, C);
		add(C, C, D);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int8_t conv2_2_bias = 25;


// Convolution: Filter 2.3.1, source/target: AREG B,
// available AREGs: C to F
inline void conv_2_3_1(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(B, B, 1, 0, 0, 0);
		east(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(C, C, 0, 0, 1, 0);
		div2_inplace(C, C);
		add(B, B, C);
		// [D] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(D, C, 0, 0, 0, 1);
		neg(D, C);
		add(B, B, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		west(D, D);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		north(C, C);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		west(D, D);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		north(E, D);
		sub(D, D, E);
		add(C, C, D);
		// [D] -> [D] || x:2 y:-1 s:0 neg:0
		// _transform(D, D, 2, -1, 0, 0);
		south(D, D);
		east(D, D);
		east(D, D);
		sub(B, B, D);
		add(C, C, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		add(B, B, C);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

// Convolution: Filter 2.3.2, source/target: AREG B,
// available AREGs: C to F
inline void conv_2_3_2(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(B, B, -1, 0, 0, 0);
		west(B, B);
		// [C] -> [B] || x:0 y:1 s:2 neg:1
		// _transform(C, B, 0, 1, 2, 1);
		north(C, B);
		div2_inplace(C, C);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:2 y:0 s:0 neg:1
		// _transform(D, C, 2, 0, 0, 1);
		east(D, C);
		east(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:-1 s:0 neg:0
		// _transform(D, D, -1, -1, 0, 0);
		south(D, D);
		west(D, D);
		add(D, D, B);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(C, C, B);
		// [B] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(B, B, 0, 0, 1, 1);
		div2_inplace(B, B);
		neg_inplace(B, B);
		add(C, C, B);
		// [B] -> [B] || x:2 y:0 s:0 neg:0
		// _transform(B, B, 2, 0, 0, 0);
		east(B, B);
		east(B, B);
		add(B, B, C);
		add(B, B, D);
		// [C] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(C, D, 0, -1, 0, 0);
		south(C, D);
		add(B, B, C);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

// Convolution: Filter 2.3.3, source/target: AREG A,
// available AREGs: B to F
inline void conv_2_3_3(){
	scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		// [A] -> [A] || x:-1 y:-1 s:0 neg:0
		// _transform(A, A, -1, -1, 0, 0);
		south(A, A);
		west(A, A);
		add(A, A, B);
		// [B] -> [B] || x:0 y:1 s:1 neg:0
		// _transform(B, B, 0, 1, 1, 0);
		north(B, B);
		div2_inplace(B, B);
		add(A, A, B);
		// ----------------------------------------------------
	scamp5_kernel_end();
}

const int8_t conv2_3_bias = 75;


#endif /* CONV_INSTRUCTIONS_HPP_ */
