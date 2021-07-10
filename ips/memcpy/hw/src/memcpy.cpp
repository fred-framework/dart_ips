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
	data_t *data_out = (data_t *)&mem_out[args[0] / sizeof (data_t)];
	data_t *data_in = (data_t *)&mem_in[args[1] / sizeof (data_t)];

	for (int i = 0; i < BUFF_SIZE; ++i) {
		#pragma HLS PIPELINE
		temp = data_in[i];
		data_out[i] = temp;
	}
}
