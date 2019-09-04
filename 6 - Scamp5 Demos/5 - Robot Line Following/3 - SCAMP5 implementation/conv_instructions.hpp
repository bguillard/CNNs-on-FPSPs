/*
 * conv_instructions.hpp
 *
 *  Created on: Aug 6, 2019
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


// Macros for double shifting operations.
// Allows for in place convolution computations
// on interleaved feature maps.
// ALL OF THEM ASSUME DREG R5 CONTAINS THE CURRENT MASK OF INTEREST
// and use aux AREG F
#define double_west(X,Y)	{	ALL();	west(F,Y);	west(F,F);	WHERE(R1);	mov(X,F);}
#define double_east(X,Y)	{	ALL();	east(F,Y);	east(F,F);	WHERE(R1);	mov(X,F);}
#define double_south(X,Y)	{	ALL();	south(F,Y);	south(F,F);	WHERE(R1);	mov(X,F);}
#define double_north(X,Y)	{	ALL();	north(F,Y);	north(F,F);	WHERE(R1);	mov(X,F);}


//Convolution: Filter 1.1, source/target: AREG A
inline void conv_1(){
	scamp5_kernel_begin();
		WHERE(R1);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(A, A, 0, 0, 2, 0);
		div2_inplace(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(B, A, 0, 1, 0, 0);
		double_north(B, A);
		// [C] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(C, B, 1, 0, 0, 0);
		double_east(C, B);
		add(A, A, C);
		// [C] -> [C] || x:0 y:-2 s:0 neg:0
		// _transform(C, C, 0, -2, 0, 0);
		double_south(C, C);
		double_south(C, C);
		add(B, B, C);
		add(B, B, C);
		// [C] -> [C] || x:-2 y:0 s:0 neg:1
		// _transform(C, C, -2, 0, 0, 1);
		double_west(C, C);
		double_west(C, C);
		neg_inplace(C, C);
		add(A, A, C);
		// [C] -> [C] || x:0 y:2 s:0 neg:0
		// _transform(C, C, 0, 2, 0, 0);
		double_north(C, C);
		double_north(C, C);
		sub(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(B, B, -1, 0, 0, 0);
		double_west(B, B);
		add(A, A, B);
		// ----------------------------------------------------
		ALL();
	scamp5_kernel_end();
}

//Convolution: Filter 2, source/target: AREG A
inline void conv_2(){
	scamp5_kernel_begin();
		WHERE(R1);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:1 neg:1
		// _transform(A, A, 0, 0, 1, 1);
		div2_inplace(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:1 y:0 s:0 neg:1
		// _transform(B, A, 1, 0, 0, 1);
		double_east(B, A);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		add(B, B, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		// [D] -> [A] || x:0 y:1 s:1 neg:0
		// _transform(D, A, 0, 1, 1, 0);
		double_north(D, A);
		div2_inplace(D, D);
		add(A, A, C);
		// [C] -> [C] || x:-2 y:0 s:0 neg:0
		// _transform(C, C, -2, 0, 0, 0);
		double_west(C, C);
		double_west(C, C);
		add(A, A, C);
		// [C] -> [D] || x:0 y:0 s:0 neg:1
		// _transform(C, D, 0, 0, 0, 1);
		neg(C, D);
		add(B, B, C);
		// [C] -> [C] || x:1 y:-1 s:0 neg:0
		// _transform(C, C, 1, -1, 0, 0);
		double_south(C, C);
		double_east(C, C);
		add(B, B, C);
		add(B, B, D);
		sub(B, B, D);
		add(B, B, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		double_north(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		ALL();
	scamp5_kernel_end();
}

//Convolution: Filter 3, source/target: AREG A
inline void conv_3(){
	scamp5_kernel_begin();
		WHERE(R1);
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:1 s:1 neg:0
		// _transform(A, A, 0, 1, 1, 0);
		double_north(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:-1 y:0 s:1 neg:1
		// _transform(B, A, -1, 0, 1, 1);
		double_west(B, A);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:-1 s:0 neg:1
		// _transform(C, B, 0, -1, 0, 1);
		double_south(C, B);
		neg_inplace(C, C);
		add(A, A, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		double_south(C, C);
		add(B, B, C);
		add(B, B, C);
		// [C] -> [C] || x:2 y:0 s:0 neg:0
		// _transform(C, C, 2, 0, 0, 0);
		double_east(C, C);
		double_east(C, C);
		add(B, B, C);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		double_north(C, C);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		double_east(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		ALL();
	scamp5_kernel_end();
}

#endif /* CONV_INSTRUCTIONS_HPP_ */
