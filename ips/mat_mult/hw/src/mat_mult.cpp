/*
 * Fred hardware accelerator stub.
 *
 * Copyright (C) 2019, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <cstring>

#include "mat_mult_top.hpp"
#include "mat_mult.hpp"

void mat_mult(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out,
			args_t args0, args_t args1, args_t args2)
{
	data_t mat_a[MAT_SIZE][MAT_SIZE];
	data_t mat_b[MAT_SIZE][MAT_SIZE];
	data_t mat_p[MAT_SIZE][MAT_SIZE];
	data_t a, b, mult, sum;
	int result;

	//*id = MODULE_ID;

	#pragma HLS ARRAY_PARTITION variable=mat_a cyclic factor=MAT_SIZE dim=2
	#pragma HLS ARRAY_PARTITION variable=mat_b cyclic factor=MAT_SIZE dim=1

	//#pragma HLS dataflow
	//#pragma HLS ALLOCATION instances=MulnS limit=4 core
	#pragma HLS ALLOCATION instances=mul limit=8 operation

	for (int mat_i = 0; mat_i < iterations; ++mat_i) {

		// Copy input data to local memory
		std::memcpy((void *)mat_a,
					(const void *)( mem_port_in + (args0 + mat_i * MAT_SIZE * MAT_SIZE * sizeof(data_t)) / sizeof(data_t) ),
					MAT_SIZE * MAT_SIZE * sizeof(data_t));

		std::memcpy((void *)mat_b,
					(const void *)( mem_port_in + (args1 + mat_i * MAT_SIZE * MAT_SIZE * sizeof(data_t)) / sizeof(data_t) ),
					MAT_SIZE * MAT_SIZE * sizeof(data_t));

		a_row_loop: for (int r = 0; r < MAT_SIZE; r++) {
			b_col_loop: for (int c = 0; c < MAT_SIZE; c++) {

			// pipelined inner loop
			#pragma HLS PIPELINE
				dp_loop: for (int k = 0; k < MAT_SIZE; k++) {

					a = mat_a[r][k];
					b = mat_b[k][c];
					mult = a * b;

					if (k == 0)
						sum = mult;
					else
						sum += mult;

					mat_p[r][c] = sum;
				}
			}
		}

		// Copy local memory contents to output
		std::memcpy((void *)( mem_port_out + (args2 + mat_i * MAT_SIZE * MAT_SIZE * sizeof(data_t)) / sizeof(data_t) ),
					(const void *)mat_p,
					MAT_SIZE * MAT_SIZE * sizeof(data_t));
	}
}
