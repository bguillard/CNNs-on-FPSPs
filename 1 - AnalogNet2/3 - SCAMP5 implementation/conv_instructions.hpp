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

//Convolution: Filter A
inline void conv_A(){
	scamp5_kernel_begin();
		mov(A, A, west);
		diva(A, E, F);
		diva(A, E, F);
		mov(D, A, south);
		mov(D, D, east);
		mov(E, A, north);

		mov(F, E);
		add(E, E, F);

		sub(A, A, E);
		mov(E, D, west);
		sub(A, A, E);
		mov(E, D, north);
		mov(E, E, north);
		sub(D, D, E);
		add(A, A, D);
		mov(D, D, east);

		mov(F, D);
		add(D, D, F);

		add(A, A, D);
	scamp5_kernel_end();
}

//Convolution: Filter B
inline void conv_B(){
	scamp5_kernel_begin();
		mov(B, B, east);
		diva(B, E, F);
		diva(B, E, F);
		mov(D, B, west);

		neg(F, D);
		mov(D, F);

		mov(E, B, north);
		add(B, B, E);
		add(B, B, D);
		mov(D, D, east);
		mov(E, B, south);
		add(B, B, E);
		add(B, B, D);
		mov(D, D, north);
		mov(D, D, west);

		mov(F, D);
		add(D, D, F);

		add(B, B, D);
	scamp5_kernel_end();
}

//Convolution: Filter C
inline void conv_C(){
	scamp5_kernel_begin();
		mov(C, C, west);
		diva(C, E, F);
		diva(C, E, F);
		mov(D, C, east);
		mov(D, D, east);

		neg(F, D);
		mov(D, F);

		mov(E, C, south);

		mov(F, E);
		add(E, E, F);

		add(C, C, E);
		mov(E, D, south);
		mov(E, E, west);
		mov(E, E, west);
		sub(C, C, E);
		mov(E, D, north);

		mov(F, E);
		add(E, E, F);

		mov(F, E);
		add(E, E, F);

		sub(C, C, E);

		mov(E, D, south);
		mov(E, E, west);

		mov(F, E);
		add(E, E, F);

		add(C, C, E);
		mov(E, D, south);

		mov(F, E);
		add(E, E, F);

		mov(F, E);
		add(E, E, F);

		add(C, C, E);
		mov(E, D, north);
		add(C, C, E);
		mov(E, D, north);
		mov(E, E, west);
		add(D, D, E);
		add(C, C, D);
		mov(D, D, west);

		mov(F, D);
		add(D, D, F);

		mov(F, D);
		add(D, D, F);

		add(C, C, D);
	scamp5_kernel_end();
}

#endif /* CONV_INSTRUCTIONS_HPP_ */
