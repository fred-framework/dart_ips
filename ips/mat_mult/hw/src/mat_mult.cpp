/*
 * Matrix multiplication DART/Fred enabled hardware accelerator.
 *
 * Copyright (C) 2019, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 * Copyright (C) 2021, Alexandre Amory, ReTiS Lab.
 * <alexandre.amory(at)santannapisa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <cstring>
#include <stdint.h>
// TODO: in the future, try something similar to this Xilinx builtin matrix multiplication, which deprecated in 2019.2 
//#include "hls_linear_algebra.h"
#include "mat_mult_top.hpp"
#include "mat_mult.hpp"

// solution based on https://github.com/Xilinx/SDSoC_Examples/blob/master/cpp/getting_started/direct_connect/src/mmult.cpp

void mat_mult(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out,
			args_t args0, args_t args1, args_t args2)
{
	// TODO: it would be nice to set the internal buffers to 32 bits instead of 64 to save BRAMs. However, for some 
	// unknown reason, the data copy procedure presented below works only with memcpy

	data_t mat_a[MAT_SIZE][MAT_SIZE];
	data_t mat_b[MAT_SIZE][MAT_SIZE];
	data_t mat_p[MAT_SIZE][MAT_SIZE];
	data_t result, mult;
	int i,j,k,idx;

	// TODO investigate the use of dataflow pargma messes up with the design (e.g. hls/dart gives error). check it further in the future
 	//#pragma HLS DATAFLOW

	// factor of 4 cause these warnings below which means that the latency is suboptimal due to the lack of enough memory ports to read/write form the memories
	// WARNING: [SCHED 204-69] Unable to satisfy resource constraint for operation type mul(II = 1)..
	// WARNING: [SCHED 204-69] Unable to schedule 'load' operation ('mat_a_0_load_4', src/mat_mult.cpp:73->src/mat_mult_top.cpp:31->src/mat_mult_top.cpp:16) on array 'mat_a[0]', src/mat_mult.cpp:26->src/mat_mult_top.cpp:31->src/mat_mult_top.cpp:16 due to limited memory ports. Please consider using a memory core with more ports or partitioning the array 'mat_a_0'.
	#pragma HLS ARRAY_PARTITION variable=mat_a cyclic factor=PARTITION_FACTOR dim=2
	#pragma HLS ARRAY_PARTITION variable=mat_b cyclic factor=PARTITION_FACTOR dim=1
	// uncomment this to have a fully partitioned memory, with maximal usage of BRAMs
	// a nice design exploration approach is to use the maximal partition your device
	// can support (even better if complete partition is possible) so you get ride of 
	// any input bandwidth constraint. Then tweak the other
	// #pragma HLS ARRAY_PARTITION variable=mat_a complete dim=2
	// #pragma HLS ARRAY_PARTITION variable=mat_b complete dim=1

	// Burst read for matrix A, and B
	// There are 3 different implementations to perform the initial memory copy: 
	// - dual loop without pipeline pragma: works in the FPGA but the latency is too high. It stops working if pipeline pragma is used;
	// - single loop with pipeline pragma: wont work in the FPGA because of the pipeline pragma
	// - memcpy: the only solution that works in the FPGA and has a reasable latency. However, it does not allow to reduce the buffer to 32bit intergers.

/*
	data_t *data_A = (data_t *)&mem_port_in[args0 / sizeof (data_t)];
	data_t *data_B = (data_t *)&mem_port_in[args1 / sizeof (data_t)];
	data_t temp;
	
	idx=0;
	read_data: for(i = 0; i < MAT_SIZE; i++){
		for(j = 0; j < MAT_SIZE; j++){
			temp = data_A[idx];
			mat_a[i][j] = temp;
			temp = data_B[idx];
			mat_b[i][j] = temp;
			idx++;
		}
	}
	*/
	/*

    read_data: for(int itr = 0 , i = 0 , j =0; itr < MAT_SIZE * MAT_SIZE; itr++, j++){
    	#pragma HLS PIPELINE
        if(j == MAT_SIZE) { j = 0 ; i++; }
        mat_a[i][j] = data_A[itr];
        mat_b[i][j] = data_B[itr];
    }
*/

	std::memcpy((void *)mat_a,
				(const void *)( mem_port_in + args0 / sizeof(data_t) ),
				MAT_SIZE * MAT_SIZE * sizeof(data_t));

	std::memcpy((void *)mat_b,
				(const void *)( mem_port_in + args1 / sizeof(data_t) ),
				MAT_SIZE * MAT_SIZE * sizeof(data_t));

	// this limits the number of parallel multiplication it can handle.
	// increasing this value will increase the number of DSP but decrease the latency
	// For the Pynq board a limit of 16 uses 70% og the DSP. So this limit is good for this board.
	#pragma HLS ALLOCATION instances=mul limit=ALLOCATION_LIMIT operation

	// Performs matrix multiply over matrices A and B and stores the result
	// in "out". All the matrices are square matrices of the form (size x size)
	// Typical Matrix multiplication Algorithm is as below
	mmult1: for (i = 0; i < MAT_SIZE ; i++) {
	//#pragma HLS LOOP_TRIPCOUNT min=c_min max=c_max
		mmult2: for (j = 0; j < MAT_SIZE ; j++) {
		#pragma HLS PIPELINE
		//#pragma HLS LOOP_TRIPCOUNT min=c_min max=c_max
			result = 0;
			mmult3: for (k = 0; k < MAT_SIZE; k++) {
			//#pragma HLS LOOP_TRIPCOUNT min=c_min max=c_max
				mult = mat_a[i][k] * mat_b[k][j];
				result += mult;
			}
			mat_p[i][j] = result;
		}
	}

	// uncomment this to use xilinx builtin matrix multiplication
	// hls::matrix_multiply <hls::NoTranspose, hls::NoTranspose, MAT_SIZE, MAT_SIZE, MAT_SIZE, MAT_SIZE, MAT_SIZE, MAT_SIZE, int_data_t, data_t> (mat_a, mat_b, mat_p);

	// Copy local memory contents to output
	std::memcpy((void *)( mem_port_out + args2 / sizeof(data_t) ),
				(const void *)mat_p,
				MAT_SIZE * MAT_SIZE * sizeof(data_t));
}
