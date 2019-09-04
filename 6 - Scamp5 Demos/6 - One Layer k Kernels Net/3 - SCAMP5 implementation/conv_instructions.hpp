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


void conv1(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(A, A, 0, -1, 0, 0);
		south(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		// [C] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(C, B, 1, 0, 0, 0);
		east(C, B);
		add(A, A, C);
		// [B] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(B, B, 0, 0, 1, 1);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:-1 y:1 s:0 neg:0
		// _transform(C, B, -1, 1, 0, 0);
		north(C, B);
		west(C, C);
		add(B, B, C);
		add(A, A, B);
		// [B] -> [B] || x:1 y:1 s:0 neg:0
		// _transform(B, B, 1, 1, 0, 0);
		north(B, B);
		east(B, B);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

void conv2(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		add_twice(B, A, A);
		// [A] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(A, A, 0, 0, 1, 0);
		div2_inplace(A, A);
		add(B, B, A);
		// [A] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(A, A, 0, 0, 1, 0);
		div2_inplace(A, A);
		add(B, B, A);
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		add_twice(C, A, A);
		// [D] -> [C] || x:0 y:0 s:1 neg:1
		// _transform(D, C, 0, 0, 1, 1);
		div2(D, C);
		neg_inplace(D, D);
		add(E, C, A);
		add(B, B, D);
		// [C] -> [E] || x:-1 y:0 s:0 neg:0
		// _transform(C, E, -1, 0, 0, 0);
		west(C, E);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		west(D, D);
		sub_inplace(C, D, C);
		add(D, E, A);
		// [A] -> [A] || x:1 y:1 s:0 neg:0
		// _transform(A, A, 1, 1, 0, 0);
		north(A, A);
		east(A, A);
		add(A, A, B);
		// [B] -> [B] || x:1 y:1 s:2 neg:0
		// _transform(B, B, 1, 1, 2, 0);
		north(B, B);
		east(B, B);
		div2_inplace(B, B);
		div2_inplace(B, B);
		sub(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(B, C, 1, 0, 0, 0);
		east(B, C);
		add(A, A, B);
		sub(A, A, D);
		add(B, D, A);
		// [A] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(A, A, 0, 0, 2, 0);
		div2_inplace(A, A);
		div2_inplace(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 2:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(A, A, 0, 0, 1, 0);
		div2_inplace(A, A);
		// [B] -> [A] || x:-1 y:0 s:0 neg:1
		// _transform(B, A, -1, 0, 0, 1);
		west(B, A);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(C, B, 0, 1, 0, 0);
		north(C, B);
		add(A, A, C);
		// [C] -> [C] || x:2 y:0 s:1 neg:0
		// _transform(C, C, 2, 0, 1, 0);
		east(C, C);
		east(C, C);
		div2_inplace(C, C);
		add(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:0 y:-1 s:1 neg:0
		// _transform(B, B, 0, -1, 1, 0);
		south(B, B);
		div2_inplace(B, B);
		add(A, A, B);
		// [B] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(B, B, 1, 0, 0, 0);
		east(B, B);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

void conv3(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		add_twice(B, A, A);
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		// [C] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(C, A, 0, 0, 2, 0);
		div2(C, A);
		div2_inplace(C, C);
		add(A, A, C);
		// [D] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(D, C, 0, 0, 0, 1);
		neg(D, C);
		add(A, A, C);
		sub(A, A, D);
		// [E] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(E, C, 0, -1, 0, 0);
		south(E, C);
		// [D] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(D, D, 0, -1, 0, 0);
		south(D, D);
		sub(D, D, E);
		// [E] -> [B] || x:1 y:0 s:2 neg:0
		// _transform(E, B, 1, 0, 2, 0);
		east(E, B);
		div2_inplace(E, E);
		div2_inplace(E, E);
		// [C] -> [C] || x:2 y:0 s:0 neg:0
		// _transform(C, C, 2, 0, 0, 0);
		east(C, C);
		east(C, C);
		add(C, C, E);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		add(B, B, A);
		// [A] -> [D] || x:1 y:0 s:1 neg:0
		// _transform(A, D, 1, 0, 1, 0);
		east(A, D);
		div2_inplace(A, A);
		// [E] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(E, A, 1, 0, 0, 0);
		east(E, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		sub(A, A, E);
		add(A, A, D);
		// [D] -> [D] || x:0 y:0 s:1 neg:1
		// _transform(D, D, 0, 0, 1, 1);
		div2_inplace(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(E, D, 1, 0, 0, 0);
		east(E, D);
		add(D, D, E);
		// [E] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(E, C, -1, 0, 0, 0);
		west(E, C);
		sub(C, C, E);
		add(A, A, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		north(D, D);
		add(B, B, D);
		add(C, C, B);
		// [B] -> [B] || x:0 y:-1 s:2 neg:0
		// _transform(B, B, 0, -1, 2, 0);
		south(B, B);
		div2_inplace(B, B);
		div2_inplace(B, B);
		sub(A, A, B);
		add(B, A, C);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 2:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [B] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(B, A, -1, 0, 0, 0);
		west(B, A);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		// [D] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(D, C, 0, 0, 0, 1);
		neg(D, C);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		sub_inplace(A, B, A);
		// [B] -> [D] || x:2 y:-1 s:1 neg:0
		// _transform(B, D, 2, -1, 1, 0);
		south(B, D);
		east(B, B);
		east(B, B);
		div2_inplace(B, B);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		north(D, D);
		sub_inplace(B, D, B);
		add(B, B, C);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		north(C, C);
		sub(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(A, A, 0, 0, 2, 0);
		div2_inplace(A, A);
		div2_inplace(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 3:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:-1 s:1 neg:0
		// _transform(A, A, 0, -1, 1, 0);
		south(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(B, A, -1, 0, 0, 0);
		west(B, A);
		// [C] -> [B] || x:0 y:1 s:1 neg:0
		// _transform(C, B, 0, 1, 1, 0);
		north(C, B);
		div2_inplace(C, C);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		north(D, C);
		// [E] -> [A] || x:1 y:1 s:0 neg:0
		// _transform(E, A, 1, 1, 0, 0);
		north(E, A);
		east(E, E);
		add(B, B, E);
		add(A, A, D);
		// [D] -> [D] || x:1 y:-1 s:0 neg:0
		// _transform(D, D, 1, -1, 0, 0);
		south(D, D);
		east(D, D);
		sub(A, A, D);
		add(A, A, C);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		north(C, C);
		add(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:0 y:1 s:1 neg:0
		// _transform(B, B, 0, 1, 1, 0);
		north(B, B);
		div2_inplace(B, B);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

void conv4(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		// [B] -> [A] || x:1 y:0 s:0 neg:1
		// _transform(B, A, 1, 0, 0, 1);
		east(B, A);
		neg_inplace(B, B);
		// [D] -> [B] || x:0 y:0 s:2 neg:0
		// _transform(D, B, 0, 0, 2, 0);
		div2(D, B);
		div2_inplace(D, D);
		// [C] -> [D] || x:-1 y:-2 s:0 neg:0
		// _transform(C, D, -1, -2, 0, 0);
		south(C, D);
		south(C, C);
		west(C, C);
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		add(A, A, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:1
		// _transform(C, C, -1, 0, 0, 1);
		west(C, C);
		neg_inplace(C, C);
		add(A, A, B);
		sub(A, A, D);
		add(A, A, C);
		// [B] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(B, C, 0, 1, 0, 0);
		north(B, C);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 2:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:0 s:1 neg:1
		// _transform(A, A, -1, 0, 1, 1);
		west(A, A);
		div2_inplace(A, A);
		neg_inplace(A, A);
		add_twice(B, A, A);
		// [C] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(C, A, 0, 0, 1, 0);
		div2(C, A);
		add(B, B, A);
		// [A] -> [A] || x:2 y:-1 s:0 neg:1
		// _transform(A, A, 2, -1, 0, 1);
		south(A, A);
		east(A, A);
		east(A, A);
		neg_inplace(A, A);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [D] -> [A] || x:0 y:0 s:1 neg:1
		// _transform(D, A, 0, 0, 1, 1);
		div2(D, A);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:2 s:0 neg:0
		// _transform(E, D, 0, 2, 0, 0);
		north(E, D);
		north(E, E);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		west(D, D);
		add(D, D, E);
		add(B, B, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		sub_inplace(C, D, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:1 s:1 neg:0
		// _transform(A, A, 0, 1, 1, 0);
		north(A, A);
		div2_inplace(A, A);
		add(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(B, C, -1, 0, 0, 0);
		west(B, C);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 3:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:-1 s:0 neg:1
		// _transform(A, A, 0, -1, 0, 1);
		south(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:0 y:0 s:2 neg:1
		// _transform(B, A, 0, 0, 2, 1);
		div2(B, A);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:1 y:1 s:0 neg:0
		// _transform(C, B, 1, 1, 0, 0);
		north(C, B);
		east(C, C);
		add(B, B, C);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		north(C, C);
		add_twice(D, C, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:-2 y:0 s:0 neg:1
		// _transform(C, C, -2, 0, 0, 1);
		west(C, C);
		west(C, C);
		neg_inplace(C, C);
		add(D, D, C);
		// [C] -> [C] || x:2 y:0 s:0 neg:0
		// _transform(C, C, 2, 0, 0, 0);
		east(C, C);
		east(C, C);
		add(C, C, D);
		add(C, C, A);
		// [A] -> [A] || x:1 y:0 s:2 neg:0
		// _transform(A, A, 1, 0, 2, 0);
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
		scamp5_kernel_end();break;
	case 4:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		// [B] -> [A] || x:1 y:0 s:2 neg:1
		// _transform(B, A, 1, 0, 2, 1);
		east(B, A);
		div2_inplace(B, B);
		div2_inplace(B, B);
		neg_inplace(B, B);
		add_twice(C, B, B);
		add(B, B, C);
		add(B, B, C);
		// [C] -> [C] || x:-2 y:-2 s:0 neg:1
		// _transform(C, C, -2, -2, 0, 1);
		south(C, C);
		south(C, C);
		west(C, C);
		west(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:0 s:1 neg:1
		// _transform(D, C, 1, 0, 1, 1);
		east(D, C);
		div2_inplace(D, D);
		neg_inplace(D, D);
		// [E] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(E, A, 0, 0, 2, 0);
		div2(E, A);
		div2_inplace(E, E);
		sub(A, A, E);
		add(A, A, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		north(D, D);
		add(E, B, C);
		// [B] -> [C] || x:2 y:2 s:0 neg:0
		// _transform(B, C, 2, 2, 0, 0);
		north(B, C);
		north(B, B);
		east(B, B);
		east(B, B);
		sub_inplace(B, E, B);
		add(B, B, D);
		// [C] -> [D] || x:1 y:1 s:0 neg:0
		// _transform(C, D, 1, 1, 0, 0);
		north(C, D);
		east(C, C);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

void conv5(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:2 neg:1
		// _transform(A, A, 0, 0, 2, 1);
		div2_inplace(A, A);
		div2_inplace(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(B, A, -1, 0, 0, 0);
		west(B, A);
		add_twice(C, B, B);
		// [B] -> [B] || x:0 y:0 s:0 neg:1
		// _transform(B, B, 0, 0, 0, 1);
		neg_inplace(B, B);
		// [D] -> [A] || x:1 y:-1 s:0 neg:0
		// _transform(D, A, 1, -1, 0, 0);
		south(D, A);
		east(D, D);
		add(A, A, D);
		// [D] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(D, B, 0, -1, 0, 0);
		south(D, B);
		add(B, B, D);
		add(B, B, A);
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		add(C, C, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		add(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		sub(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 2:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:-1 s:0 neg:0
		// _transform(A, A, 1, -1, 0, 0);
		south(A, A);
		east(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		add(A, A, B);
		// [B] -> [B] || x:0 y:0 s:0 neg:1
		// _transform(B, B, 0, 0, 0, 1);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		add(B, B, C);
		// [C] -> [C] || x:-1 y:2 s:0 neg:1
		// _transform(C, C, -1, 2, 0, 1);
		north(C, C);
		north(C, C);
		west(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:-1 s:0 neg:0
		// _transform(D, C, 1, -1, 0, 0);
		south(D, C);
		east(D, D);
		// [E] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(E, C, -1, 0, 0, 0);
		west(E, C);
		add(D, D, E);
		// [E] -> [A] || x:0 y:2 s:1 neg:0
		// _transform(E, A, 0, 2, 1, 0);
		north(E, A);
		north(E, E);
		div2_inplace(E, E);
		sub(A, A, E);
		add(A, A, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		sub(A, A, C);
		add(A, A, B);
		sub(A, A, B);
		add(A, A, D);
		// [B] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(B, D, 0, -1, 0, 0);
		south(B, D);
		sub(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 3:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		// [B] -> [A] || x:0 y:0 s:2 neg:1
		// _transform(B, A, 0, 0, 2, 1);
		div2(B, A);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:2 y:1 s:0 neg:1
		// _transform(C, B, 2, 1, 0, 1);
		north(C, B);
		east(C, C);
		east(C, C);
		neg_inplace(C, C);
		add_twice(D, C, C);
		add(D, D, C);
		// [C] -> [C] || x:0 y:-2 s:0 neg:0
		// _transform(C, C, 0, -2, 0, 0);
		south(C, C);
		south(C, C);
		sub_inplace(C, C, C);
		add(C, C, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		// [E] -> [D] || x:0 y:-2 s:0 neg:0
		// _transform(E, D, 0, -2, 0, 0);
		south(E, D);
		south(E, E);
		sub(C, C, E);
		add(D, D, B);
		// [B] -> [B] || x:1 y:0 s:0 neg:1
		// _transform(B, B, 1, 0, 0, 1);
		east(B, B);
		neg_inplace(B, B);
		add(D, D, A);
		// [A] -> [A] || x:1 y:-1 s:1 neg:0
		// _transform(A, A, 1, -1, 1, 0);
		south(A, A);
		east(A, A);
		div2_inplace(A, A);
		add(A, A, D);
		add(A, A, B);
		// [B] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(B, B, 1, 0, 0, 0);
		east(B, B);
		add(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(B, C, 0, 1, 0, 0);
		north(B, C);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 4:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:0 s:0 neg:1
		// _transform(A, A, 1, 0, 0, 1);
		east(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:0 y:1 s:2 neg:1
		// _transform(B, A, 0, 1, 2, 1);
		north(B, A);
		div2_inplace(B, B);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:-2 y:0 s:0 neg:1
		// _transform(C, B, -2, 0, 0, 1);
		west(C, B);
		west(C, C);
		neg_inplace(C, C);
		add(B, B, C);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:-2 s:0 neg:0
		// _transform(D, C, 0, -2, 0, 0);
		south(D, C);
		south(D, D);
		add(D, D, B);
		add(D, D, A);
		// [A] -> [A] || x:0 y:1 s:1 neg:0
		// _transform(A, A, 0, 1, 1, 0);
		north(A, A);
		div2_inplace(A, A);
		sub_inplace(A, D, A);
		add(A, A, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		add(A, A, C);
		// [C] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(C, B, 0, -1, 0, 0);
		south(C, B);
		add(A, A, C);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 5:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		add(A, A, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		// [D] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(D, C, 0, 1, 0, 0);
		north(D, C);
		sub(B, B, D);
		// [E] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(E, C, 1, 0, 0, 0);
		east(E, C);
		// [D] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(D, C, 0, -1, 0, 0);
		south(D, C);
		add(D, D, E);
		add(A, A, B);
		// [B] -> [B] || x:1 y:0 s:1 neg:0
		// _transform(B, B, 1, 0, 1, 0);
		east(B, B);
		div2_inplace(B, B);
		add(A, A, B);
		// [B] -> [C] || x:1 y:1 s:0 neg:0
		// _transform(B, C, 1, 1, 0, 0);
		north(B, C);
		east(B, B);
		sub(A, A, B);
		// [B] -> [C] || x:2 y:-1 s:0 neg:0
		// _transform(B, C, 2, -1, 0, 0);
		south(B, C);
		east(B, B);
		east(B, B);
		add(B, B, D);
		// [C] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(C, D, 0, 1, 0, 0);
		north(C, D);
		sub(A, A, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(A, A, 0, 0, 2, 0);
		div2_inplace(A, A);
		div2_inplace(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

void conv6(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:1
		// _transform(B, A, 0, 0, 1, 1);
		div2(B, A);
		neg_inplace(B, B);
		// [C] -> [B] || x:1 y:-1 s:0 neg:1
		// _transform(C, B, 1, -1, 0, 1);
		south(C, B);
		east(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:0 y:0 s:1 neg:0
		// _transform(D, C, 0, 0, 1, 0);
		div2(D, C);
		add(C, C, D);
		// [D] -> [D] || x:0 y:0 s:0 neg:1
		// _transform(D, D, 0, 0, 0, 1);
		neg_inplace(D, D);
		add(A, A, D);
		// [D] -> [D] || x:0 y:2 s:0 neg:1
		// _transform(D, D, 0, 2, 0, 1);
		north(D, D);
		north(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(E, D, -1, 0, 0, 0);
		west(E, D);
		sub_inplace(D, E, D);
		// [E] -> [B] || x:0 y:-1 s:1 neg:0
		// _transform(E, B, 0, -1, 1, 0);
		south(E, B);
		div2_inplace(E, E);
		add(D, D, E);
		add(B, B, C);
		// [C] -> [C] || x:-2 y:0 s:0 neg:1
		// _transform(C, C, -2, 0, 0, 1);
		west(C, C);
		west(C, C);
		neg_inplace(C, C);
		add(B, B, C);
		// [C] -> [C] || x:1 y:2 s:0 neg:0
		// _transform(C, C, 1, 2, 0, 0);
		north(C, C);
		north(C, C);
		east(C, C);
		add(B, B, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		south(C, C);
		add(B, B, C);
		add(B, B, A);
		sub_inplace(A, B, A);
		add(A, A, D);
		// [B] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(B, D, -1, 0, 0, 0);
		west(B, D);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 2:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:0 s:1 neg:0
		// _transform(A, A, -1, 0, 1, 0);
		west(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(B, A, 0, 1, 0, 0);
		north(B, A);
		// [D] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(D, B, 1, 0, 0, 0);
		east(D, B);
		// [C] -> [D] || x:0 y:0 s:1 neg:1
		// _transform(C, D, 0, 0, 1, 1);
		div2(C, D);
		neg_inplace(C, C);
		add(A, A, C);
		// [C] -> [C] || x:1 y:-1 s:0 neg:1
		// _transform(C, C, 1, -1, 0, 1);
		south(C, C);
		east(C, C);
		neg_inplace(C, C);
		add(A, A, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(C, C, 0, -1, 0, 0);
		south(C, C);
		add(B, B, C);
		// [C] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(C, D, 1, 0, 0, 0);
		east(C, D);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(A, A, 0, -1, 0, 0);
		south(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 3:
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
		// [C] -> [B] || x:0 y:2 s:0 neg:0
		// _transform(C, B, 0, 2, 0, 0);
		north(C, B);
		north(C, C);
		// [D] -> [A] || x:0 y:1 s:1 neg:0
		// _transform(D, A, 0, 1, 1, 0);
		north(D, A);
		div2_inplace(D, D);
		sub(A, A, D);
		// [B] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(B, B, -1, 0, 0, 0);
		west(B, B);
		add(B, B, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		sub(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 4:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:1 s:0 neg:0
		// _transform(A, A, -1, 1, 0, 0);
		north(A, A);
		west(A, A);
		// [B] -> [A] || x:0 y:-1 s:1 neg:1
		// _transform(B, A, 0, -1, 1, 1);
		south(B, A);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(C, B, 0, 0, 1, 1);
		div2(C, B);
		neg_inplace(C, C);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(B, B, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:1
		// _transform(C, C, 1, 0, 0, 1);
		east(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(D, C, 1, 0, 0, 0);
		east(D, C);
		add(D, D, A);
		// [A] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(A, A, 0, 0, 2, 0);
		div2_inplace(A, A);
		div2_inplace(A, A);
		sub_inplace(A, D, A);
		add(A, A, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		add(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(B, B, 1, 0, 0, 0);
		east(B, B);
		sub(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 5:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:0 s:1 neg:0
		// _transform(A, A, 1, 0, 1, 0);
		east(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:-1 s:0 neg:1
		// _transform(B, A, 0, -1, 0, 1);
		south(B, A);
		neg_inplace(B, B);
		add(C, A, B);
		add(C, C, B);
		// [B] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(B, B, 0, 0, 1, 0);
		div2_inplace(B, B);
		sub_inplace(B, C, B);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		add(A, A, B);
		// [B] -> [B] || x:-2 y:1 s:0 neg:0
		// _transform(B, B, -2, 1, 0, 0);
		north(B, B);
		west(B, B);
		west(B, B);
		sub(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 6:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:-1 s:0 neg:1
		// _transform(A, A, -1, -1, 0, 1);
		south(A, A);
		west(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		add(A, A, B);
		// [B] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(B, B, 0, 0, 1, 0);
		div2_inplace(B, B);
		add(A, A, B);
		// [B] -> [B] || x:0 y:0 s:0 neg:1
		// _transform(B, B, 0, 0, 0, 1);
		neg_inplace(B, B);
		// [C] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(C, B, 1, 0, 0, 0);
		east(C, B);
		// [D] -> [C] || x:-1 y:1 s:0 neg:0
		// _transform(D, C, -1, 1, 0, 0);
		north(D, C);
		west(D, D);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		add(B, B, D);
		// [D] -> [B] || x:2 y:0 s:0 neg:0
		// _transform(D, B, 2, 0, 0, 0);
		east(D, B);
		east(D, D);
		add(A, A, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [E] -> [D] || x:0 y:-1 s:1 neg:0
		// _transform(E, D, 0, -1, 1, 0);
		south(E, D);
		div2_inplace(E, E);
		sub(C, C, E);
		// [E] -> [B] || x:1 y:1 s:1 neg:0
		// _transform(E, B, 1, 1, 1, 0);
		north(E, B);
		east(E, E);
		div2_inplace(E, E);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		north(D, D);
		add(D, D, E);
		add(A, A, C);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		north(C, C);
		add(B, B, C);
		add(A, A, D);
		// [C] -> [D] || x:-1 y:-2 s:0 neg:0
		// _transform(C, D, -1, -2, 0, 0);
		south(C, D);
		south(C, C);
		west(C, C);
		sub(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

void conv7(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(A, A, 0, -1, 0, 0);
		south(A, A);
		// [B] -> [A] || x:1 y:0 s:0 neg:1
		// _transform(B, A, 1, 0, 0, 1);
		east(B, A);
		neg_inplace(B, B);
		add(A, A, B);
		// [B] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(B, B, 0, 0, 1, 1);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:-1 y:0 s:0 neg:1
		// _transform(C, B, -1, 0, 0, 1);
		west(C, B);
		neg_inplace(C, C);
		// [B] -> [B] || x:0 y:2 s:0 neg:0
		// _transform(B, B, 0, 2, 0, 0);
		north(B, B);
		north(B, B);
		// [D] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(D, C, -1, 0, 0, 0);
		west(D, C);
		add(B, B, D);
		// [C] -> [C] || x:0 y:1 s:1 neg:0
		// _transform(C, C, 0, 1, 1, 0);
		north(C, C);
		div2_inplace(C, C);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:0 s:2 neg:1
		// _transform(A, A, 0, 0, 2, 1);
		div2_inplace(A, A);
		div2_inplace(A, A);
		neg_inplace(A, A);
		add(B, B, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 2:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:1 s:0 neg:1
		// _transform(A, A, -1, 1, 0, 1);
		north(A, A);
		west(A, A);
		neg_inplace(A, A);
		// [B] -> [A] || x:1 y:0 s:1 neg:1
		// _transform(B, A, 1, 0, 1, 1);
		east(B, A);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:-2 s:0 neg:1
		// _transform(C, B, 0, -2, 0, 1);
		south(C, B);
		south(C, C);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:0 s:1 neg:1
		// _transform(D, C, 1, 0, 1, 1);
		east(D, C);
		div2_inplace(D, D);
		neg_inplace(D, D);
		// [E] -> [D] || x:0 y:2 s:0 neg:0
		// _transform(E, D, 0, 2, 0, 0);
		north(E, D);
		north(E, E);
		sub(B, B, E);
		add(D, D, A);
		// [A] -> [A] || x:0 y:-1 s:1 neg:1
		// _transform(A, A, 0, -1, 1, 1);
		south(A, A);
		div2_inplace(A, A);
		neg_inplace(A, A);
		add(D, D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		sub_inplace(C, D, C);
		// [D] -> [A] || x:2 y:0 s:0 neg:0
		// _transform(D, A, 2, 0, 0, 0);
		east(D, A);
		east(D, D);
		sub(A, A, D);
		add(A, A, B);
		// [B] -> [B] || x:0 y:-2 s:0 neg:0
		// _transform(B, B, 0, -2, 0, 0);
		south(B, B);
		south(B, B);
		sub_inplace(B, C, B);
		add(A, A, B);
		// [B] -> [B] || x:0 y:0 s:2 neg:0
		// _transform(B, B, 0, 0, 2, 0);
		div2_inplace(B, B);
		div2_inplace(B, B);
		sub(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 3:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		add(A, A, B);
		// [B] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(B, B, 0, 0, 1, 0);
		div2_inplace(B, B);
		// [C] -> [B] || x:0 y:-1 s:0 neg:1
		// _transform(C, B, 0, -1, 0, 1);
		south(C, B);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:0 s:0 neg:1
		// _transform(D, C, 1, 0, 0, 1);
		east(D, C);
		neg_inplace(D, D);
		add(A, A, B);
		// [E] -> [B] || x:-1 y:-1 s:0 neg:0
		// _transform(E, B, -1, -1, 0, 0);
		south(E, B);
		west(E, E);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		sub(C, C, E);
		// [E] -> [D] || x:0 y:2 s:0 neg:0
		// _transform(E, D, 0, 2, 0, 0);
		north(E, D);
		north(E, E);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		sub(B, B, E);
		add(D, D, A);
		// [A] -> [A] || x:1 y:-1 s:0 neg:0
		// _transform(A, A, 1, -1, 0, 0);
		south(A, A);
		east(A, A);
		add(A, A, D);
		add(A, A, C);
		// [C] -> [C] || x:1 y:0 s:1 neg:0
		// _transform(C, C, 1, 0, 1, 0);
		east(C, C);
		div2_inplace(C, C);
		add(A, A, C);
		add(A, A, B);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 4:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		add_twice(B, A, A);
		// [A] -> [A] || x:0 y:0 s:1 neg:1
		// _transform(A, A, 0, 0, 1, 1);
		div2_inplace(A, A);
		neg_inplace(A, A);
		sub(C, A, B);
		// [D] -> [A] || x:-1 y:0 s:1 neg:0
		// _transform(D, A, -1, 0, 1, 0);
		west(D, A);
		div2_inplace(D, D);
		// [E] -> [C] || x:0 y:1 s:1 neg:0
		// _transform(E, C, 0, 1, 1, 0);
		north(E, C);
		div2_inplace(E, E);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		add(A, A, E);
		add(C, C, B);
		// [B] -> [B] || x:-2 y:1 s:3 neg:0
		// _transform(B, B, -2, 1, 3, 0);
		north(B, B);
		west(B, B);
		west(B, B);
		div2_inplace(B, B);
		div2_inplace(B, B);
		div2_inplace(B, B);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		north(E, D);
		sub(D, D, E);
		// [E] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(E, A, 0, -1, 0, 0);
		south(E, A);
		add(C, C, E);
		add(A, A, B);
		// [B] -> [B] || x:2 y:0 s:0 neg:0
		// _transform(B, B, 2, 0, 0, 0);
		east(B, B);
		east(B, B);
		sub(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(B, C, 0, -1, 0, 0);
		south(B, C);
		add(B, B, D);
		add(A, A, B);
		// [B] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(B, B, -1, 0, 0, 0);
		west(B, B);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 5:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:-1 s:0 neg:0
		// _transform(A, A, -1, -1, 0, 0);
		south(A, A);
		west(A, A);
		// [B] -> [A] || x:2 y:0 s:0 neg:1
		// _transform(B, A, 2, 0, 0, 1);
		east(B, A);
		east(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:0 s:2 neg:1
		// _transform(C, B, 0, 0, 2, 1);
		div2(C, B);
		div2_inplace(C, C);
		neg_inplace(C, C);
		// [D] -> [A] || x:0 y:1 s:1 neg:1
		// _transform(D, A, 0, 1, 1, 1);
		north(D, A);
		div2_inplace(D, D);
		neg_inplace(D, D);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [E] -> [C] || x:0 y:2 s:0 neg:0
		// _transform(E, C, 0, 2, 0, 0);
		north(E, C);
		north(E, E);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		sub(C, C, E);
		// [E] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(E, D, 1, 0, 0, 0);
		east(E, D);
		add(C, C, E);
		add(D, D, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		sub_inplace(A, D, A);
		add(A, A, B);
		// [B] -> [B] || x:-2 y:0 s:0 neg:0
		// _transform(B, B, -2, 0, 0, 0);
		west(B, B);
		west(B, B);
		sub(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(B, C, -1, 0, 0, 0);
		west(B, C);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 6:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		// [B] -> [A] || x:-1 y:-1 s:0 neg:0
		// _transform(B, A, -1, -1, 0, 0);
		south(B, A);
		west(B, B);
		// [C] -> [B] || x:0 y:0 s:2 neg:1
		// _transform(C, B, 0, 0, 2, 1);
		div2(C, B);
		div2_inplace(C, C);
		neg_inplace(C, C);
		add(B, B, C);
		// [C] -> [C] || x:0 y:2 s:0 neg:0
		// _transform(C, C, 0, 2, 0, 0);
		north(C, C);
		north(C, C);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:1 neg:0
		// _transform(A, A, 1, 0, 1, 0);
		east(A, A);
		div2_inplace(A, A);
		add(A, A, B);
		// [B] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(B, C, 1, 0, 0, 0);
		east(B, C);
		sub_inplace(B, C, B);
		add(B, B, A);
		// [A] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(A, A, 0, 1, 0, 0);
		north(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 7:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(A, A, 0, 0, 2, 0);
		div2_inplace(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(B, A, 0, -1, 0, 0);
		south(B, A);
		// [C] -> [B] || x:-1 y:0 s:0 neg:1
		// _transform(C, B, -1, 0, 0, 1);
		west(C, B);
		neg_inplace(C, C);
		// [D] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(D, A, 0, 1, 0, 0);
		north(D, A);
		sub_inplace(D, B, D);
		sub_inplace(C, C, C);
		add(E, A, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		east(D, D);
		// [A] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(A, B, 1, 0, 0, 0);
		east(A, B);
		add(A, A, D);
		add(B, C, A);
		// [A] -> [A] || x:-2 y:0 s:0 neg:0
		// _transform(A, A, -2, 0, 0, 0);
		west(A, A);
		west(A, A);
		add(A, A, B);
		add(A, A, E);
		// [B] -> [E] || x:-1 y:0 s:0 neg:0
		// _transform(B, E, -1, 0, 0, 0);
		west(B, E);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

void conv8(int k_id){
	switch(k_id){
	case 1:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:1 s:2 neg:0
		// _transform(A, A, 0, 1, 2, 0);
		north(A, A);
		div2_inplace(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:1 y:0 s:0 neg:1
		// _transform(B, A, 1, 0, 0, 1);
		east(B, A);
		neg_inplace(B, B);
		add_twice(C, B, B);
		add(C, C, B);
		// [B] -> [B] || x:0 y:-1 s:0 neg:1
		// _transform(B, B, 0, -1, 0, 1);
		south(B, B);
		neg_inplace(B, B);
		// [D] -> [B] || x:0 y:0 s:0 neg:1
		// _transform(D, B, 0, 0, 0, 1);
		neg(D, B);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(B, B, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		// [E] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(E, D, 0, 1, 0, 0);
		north(E, D);
		add(B, B, E);
		// [D] -> [D] || x:-1 y:0 s:0 neg:0
		// _transform(D, D, -1, 0, 0, 0);
		west(D, D);
		add(D, D, A);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		add(A, A, D);
		// [D] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(D, C, -1, 0, 0, 0);
		west(D, C);
		add(C, C, D);
		add(C, C, B);
		// [B] -> [B] || x:-2 y:0 s:0 neg:0
		// _transform(B, B, -2, 0, 0, 0);
		west(B, B);
		west(B, B);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 2:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:1 neg:1
		// _transform(A, A, 0, 0, 1, 1);
		div2_inplace(A, A);
		neg_inplace(A, A);
		// [D] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(D, A, -1, 0, 0, 0);
		west(D, A);
		// [B] -> [D] || x:0 y:-1 s:0 neg:0
		// _transform(B, D, 0, -1, 0, 0);
		south(B, D);
		add_twice(C, B, B);
		// [B] -> [B] || x:0 y:0 s:1 neg:1
		// _transform(B, B, 0, 0, 1, 1);
		div2_inplace(B, B);
		neg_inplace(B, B);
		add(A, A, B);
		// [B] -> [D] || x:0 y:1 s:0 neg:1
		// _transform(B, D, 0, 1, 0, 1);
		north(B, D);
		neg_inplace(B, B);
		add(C, C, B);
		// [B] -> [B] || x:2 y:0 s:1 neg:0
		// _transform(B, B, 2, 0, 1, 0);
		east(B, B);
		east(B, B);
		div2_inplace(B, B);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 3:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		// [B] -> [A] || x:-1 y:1 s:1 neg:1
		// _transform(B, A, -1, 1, 1, 1);
		north(B, A);
		west(B, B);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:-1 s:1 neg:0
		// _transform(C, B, 0, -1, 1, 0);
		south(C, B);
		div2_inplace(C, C);
		// [A] -> [A] || x:1 y:-1 s:0 neg:0
		// _transform(A, A, 1, -1, 0, 0);
		south(A, A);
		east(A, A);
		// [D] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(D, C, 0, -1, 0, 0);
		south(D, C);
		add(A, A, D);
		// [C] -> [C] || x:1 y:0 s:0 neg:1
		// _transform(C, C, 1, 0, 0, 1);
		east(C, C);
		neg_inplace(C, C);
		add(A, A, B);
		// [B] -> [B] || x:1 y:0 s:0 neg:0
		// _transform(B, B, 1, 0, 0, 0);
		east(B, B);
		sub(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(B, C, 1, 0, 0, 0);
		east(B, C);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 4:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:1 s:1 neg:0
		// _transform(A, A, 0, 1, 1, 0);
		north(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		// [C] -> [B] || x:1 y:-2 s:0 neg:1
		// _transform(C, B, 1, -2, 0, 1);
		south(C, B);
		south(C, C);
		east(C, C);
		neg_inplace(C, C);
		add(B, B, C);
		add_twice(D, C, C);
		// [C] -> [C] || x:-2 y:0 s:0 neg:0
		// _transform(C, C, -2, 0, 0, 0);
		west(C, C);
		west(C, C);
		add(D, D, B);
		// [B] -> [B] || x:0 y:0 s:0 neg:1
		// _transform(B, B, 0, 0, 0, 1);
		neg_inplace(B, B);
		add(A, A, C);
		// [C] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(C, C, 0, 1, 0, 0);
		north(C, C);
		add(A, A, C);
		add(C, D, B);
		// [B] -> [B] || x:-1 y:0 s:0 neg:0
		// _transform(B, B, -1, 0, 0, 0);
		west(B, B);
		add(A, A, B);
		add(B, A, C);
		// [A] -> [A] || x:1 y:0 s:0 neg:0
		// _transform(A, A, 1, 0, 0, 0);
		east(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 5:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:1 s:1 neg:0
		// _transform(A, A, 0, 1, 1, 0);
		north(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:1 y:-2 s:0 neg:1
		// _transform(B, A, 1, -2, 0, 1);
		south(B, A);
		south(B, B);
		east(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:0 y:0 s:1 neg:0
		// _transform(C, B, 0, 0, 1, 0);
		div2(C, B);
		add(B, B, C);
		// [C] -> [C] || x:0 y:0 s:0 neg:1
		// _transform(C, C, 0, 0, 0, 1);
		neg_inplace(C, C);
		// [D] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(D, C, -1, 0, 0, 0);
		west(D, C);
		add(C, C, D);
		// [D] -> [D] || x:-1 y:1 s:0 neg:1
		// _transform(D, D, -1, 1, 0, 1);
		north(D, D);
		west(D, D);
		neg_inplace(D, D);
		add(C, C, D);
		// [D] -> [D] || x:0 y:1 s:0 neg:0
		// _transform(D, D, 0, 1, 0, 0);
		north(D, D);
		add(B, B, D);
		// [D] -> [D] || x:2 y:-2 s:0 neg:0
		// _transform(D, D, 2, -2, 0, 0);
		south(D, D);
		south(D, D);
		east(D, D);
		east(D, D);
		add(B, B, D);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:1 neg:0
		// _transform(A, A, 1, 0, 1, 0);
		east(A, A);
		div2_inplace(A, A);
		add(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:0 y:1 s:0 neg:0
		// _transform(B, C, 0, 1, 0, 0);
		north(B, C);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 6:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:0 y:0 s:0 neg:1
		// _transform(A, A, 0, 0, 0, 1);
		neg_inplace(A, A);
		add_twice(B, A, A);
		// [A] -> [A] || x:0 y:0 s:2 neg:1
		// _transform(A, A, 0, 0, 2, 1);
		div2_inplace(A, A);
		div2_inplace(A, A);
		neg_inplace(A, A);
		// [C] -> [A] || x:0 y:1 s:0 neg:1
		// _transform(C, A, 0, 1, 0, 1);
		north(C, A);
		neg_inplace(C, C);
		// [D] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(D, C, 1, 0, 0, 0);
		east(D, C);
		sub(A, A, D);
		// [D] -> [B] || x:1 y:-1 s:0 neg:0
		// _transform(D, B, 1, -1, 0, 0);
		south(D, B);
		east(D, D);
		add(B, B, D);
		add(B, B, A);
		// [A] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(A, A, 0, -1, 0, 0);
		south(A, A);
		add(A, A, C);
		add(B, B, A);
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		add(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 7:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:-1 y:0 s:0 neg:0
		// _transform(A, A, -1, 0, 0, 0);
		west(A, A);
		// [B] -> [A] || x:0 y:0 s:2 neg:1
		// _transform(B, A, 0, 0, 2, 1);
		div2(B, A);
		div2_inplace(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:1 y:1 s:0 neg:0
		// _transform(C, B, 1, 1, 0, 0);
		north(C, B);
		east(C, C);
		add_twice(D, C, C);
		add(D, D, C);
		// [C] -> [C] || x:1 y:0 s:0 neg:0
		// _transform(C, C, 1, 0, 0, 0);
		east(C, C);
		// [A] -> [A] || x:0 y:-1 s:0 neg:0
		// _transform(A, A, 0, -1, 0, 0);
		south(A, A);
		add(A, A, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		add(A, A, D);
		// [D] -> [D] || x:1 y:0 s:0 neg:0
		// _transform(D, D, 1, 0, 0, 0);
		east(D, D);
		add(A, A, D);
		add(A, A, C);
		// [C] -> [C] || x:-1 y:0 s:0 neg:0
		// _transform(C, C, -1, 0, 0, 0);
		west(C, C);
		add(C, C, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:0
		// _transform(B, B, 0, 1, 0, 0);
		north(B, B);
		add(B, B, C);
		add(B, B, A);
		// [A] -> [A] || x:0 y:0 s:2 neg:0
		// _transform(A, A, 0, 0, 2, 0);
		div2_inplace(A, A);
		div2_inplace(A, A);
		sub_inplace(A, B, A);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	case 8:
		scamp5_kernel_begin();
		// ----------------------------------------------------
		// DO NOT MODIFY! (Automatically generated kernel code)
		// [A] -> [A] || x:1 y:0 s:1 neg:0
		// _transform(A, A, 1, 0, 1, 0);
		east(A, A);
		div2_inplace(A, A);
		// [B] -> [A] || x:0 y:0 s:1 neg:0
		// _transform(B, A, 0, 0, 1, 0);
		div2(B, A);
		add(A, A, B);
		// [B] -> [B] || x:0 y:1 s:0 neg:1
		// _transform(B, B, 0, 1, 0, 1);
		north(B, B);
		neg_inplace(B, B);
		// [C] -> [B] || x:-2 y:0 s:0 neg:0
		// _transform(C, B, -2, 0, 0, 0);
		west(C, B);
		west(C, C);
		add_twice(D, C, C);
		// [C] -> [C] || x:0 y:-1 s:0 neg:1
		// _transform(C, C, 0, -1, 0, 1);
		south(C, C);
		neg_inplace(C, C);
		// [E] -> [C] || x:2 y:0 s:0 neg:0
		// _transform(E, C, 2, 0, 0, 0);
		east(E, C);
		east(E, E);
		sub(C, C, E);
		add(C, C, B);
		// [B] -> [B] || x:0 y:0 s:0 neg:1
		// _transform(B, B, 0, 0, 0, 1);
		neg_inplace(B, B);
		// [E] -> [A] || x:0 y:1 s:0 neg:0
		// _transform(E, A, 0, 1, 0, 0);
		north(E, A);
		add(B, B, E);
		add(E, A, D);
		// [A] -> [D] || x:1 y:-2 s:0 neg:1
		// _transform(A, D, 1, -2, 0, 1);
		south(A, D);
		south(A, A);
		east(A, A);
		neg_inplace(A, A);
		add(D, B, E);
		// [B] -> [B] || x:0 y:-1 s:0 neg:0
		// _transform(B, B, 0, -1, 0, 0);
		south(B, B);
		add(B, B, D);
		add(B, B, A);
		// [A] -> [A] || x:1 y:0 s:1 neg:0
		// _transform(A, A, 1, 0, 1, 0);
		east(A, A);
		div2_inplace(A, A);
		add(A, A, B);
		add(A, A, C);
		// [B] -> [C] || x:0 y:-1 s:0 neg:0
		// _transform(B, C, 0, -1, 0, 0);
		south(B, C);
		sub(A, A, B);
		// ----------------------------------------------------
		scamp5_kernel_end();break;
	}
}

int thresh[8][8] = {
   {15, -100, -100, -100, -100, -100, -100, -100} ,
   {20, 10, -100, -100, -100, -100, -100, -100} ,
   {15, 70, 80, -100, -100, -100, -100, -100} ,
   {25, 30, 25, 45, -100, -100, -100, -100} ,
   {30, 25, 30, 20, 20, -100, -100, -100} ,
   {30, 100, 15, 20, 25, 20, -100, -100} ,
   {15, 20, 15, 25, 20, 50, 100, -100} ,
   {45, 15, 15, 35, 15, 25, 35, 100}
};


void conv(int k_num, int k_id){
	switch(k_num){
	case 1:
		conv1(k_id);
		break;
	case 2:
		conv2(k_id);
		break;
	case 3:
		conv3(k_id);
		break;
	case 4:
		conv4(k_id);
		break;
	case 5:
		conv5(k_id);
		break;
	case 6:
		conv6(k_id);
		break;
	case 7:
		conv7(k_id);
		break;
	case 8:
		conv8(k_id);
		break;
	}
}


#endif /* CONV_INSTRUCTIONS_HPP_ */
