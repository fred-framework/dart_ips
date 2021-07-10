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

#include "memcpy.hpp"

void memcpy(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	#pragma HLS DATAFLOW
	data_t temp;

	*id = MODULE_ID;
	args_t in_addr = args[0];
	args_t out_addr = args[1];

	for (int i = 0; i < BUFF_SIZE; ++i) {
		#pragma HLS PIPELINE
		temp = *(mem_in + i + (in_addr / sizeof(data_t)));
		*(mem_out + i + (out_addr / sizeof(data_t))) = temp;
	}
}
