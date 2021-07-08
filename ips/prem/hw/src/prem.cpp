/*
 * generic FRED hardware accelerator for Amalthea runnables.
 *
 * Copyright (C) 2021, Alexandre Amory, ReTiS Lab.
 * <alexandre.amory(at)santannapisa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include "prem.hpp"

void prem(volatile data_t *mem_in, volatile data_t *mem_out)
{
	data_t count_input_val=0;
	data_t i;

	mem_rd_loop:for (i = 0; i < IN_MEM_SIZE; ++i) {
		#pragma HLS pipeline
		count_input_val += (data_t)mem_in[i];
	}

	// mem_in[0] is added to avoid optime the exec_loop
	exec_loop:for (i = 0; i < EXEC_SIZE; ++i) {
		#pragma HLS pipeline
		count_input_val += (data_t)mem_in[0] + i;
	}

	mem_wr_loop:for (i = 0; i < OUT_MEM_SIZE; ++i) {
		#pragma HLS pipeline
		mem_out[i] = (data_t)count_input_val + i;
	}
}
