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

#include "hw_mod.hpp"

void hw_mod(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	#pragma HLS DATAFLOW

	*id = MODULE_ID;
	data_t a, b, op;

	args_t op_addr = args[0];
	args_t a_addr = args[1];
	args_t b_addr = args[2];

	for (int i = 0; i < ARRAY_SIZE; ++i) {
		#pragma HLS PIPELINE
		a = *(mem_in + i + (a_addr / sizeof(data_t)));
		b = *(mem_in + i + (b_addr / sizeof(data_t)));
		op = ~(a | b);
		*(mem_out + i + (op_addr / sizeof(data_t))) = op;
	}
}
