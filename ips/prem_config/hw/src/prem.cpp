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

void prem(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	data_t count_input_val=0;
	data_t i;
	uint32_t mem_rds;
	uint32_t exec_cycles;
	uint32_t mem_wrs;

	*id = MODULE_ID;
	// the order of the args must be consistent w the order in the fred_bind function
	data_t *data_in = (data_t *)&mem_in[args[0] / sizeof (data_t)];
	data_t *data_out = (data_t *)&mem_out[args[1] / sizeof (data_t)];

	mem_rds = data_in[0];
	exec_cycles = data_in[1];
	mem_wrs = data_in[2];

	mem_rd_loop:for (i = 3; i < mem_rds+3; ++i) {
		#pragma HLS pipeline
		count_input_val += data_in[i];
	}

	// mem_in[3] is added to avoid optimizing the exec_loop
	exec_loop:for (i = 0; i < exec_cycles; ++i) {
		#pragma HLS pipeline
		count_input_val += data_in[3] + i;
	}

	mem_wr_loop:for (i = 0; i < mem_wrs; ++i) {
		#pragma HLS pipeline
		data_out[i] = count_input_val + i;
	}
}
