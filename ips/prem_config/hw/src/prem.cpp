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
	uint32_t mem_rds = mem_in[0];
	uint32_t exec_cycles = mem_in[1];
	uint32_t mem_wrs = mem_in[2];

	mem_rd_loop:for (i = 3; i < mem_rds+3; ++i) {
		#pragma HLS pipeline
		count_input_val += mem_in[i];
	}

	// mem_in[3] is added to avoid optimizing the exec_loop
	exec_loop:for (i = 0; i < exec_cycles; ++i) {
		#pragma HLS pipeline
		count_input_val += mem_in[3] + i;
	}

	mem_wr_loop:for (i = 0; i < mem_wrs; ++i) {
		#pragma HLS pipeline
		mem_out[i] = count_input_val + i;
	}
}
