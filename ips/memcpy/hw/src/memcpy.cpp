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
//#include <iostream>

void memcpy(volatile data_t *mem_in, volatile data_t *mem_out)
{
	//#pragma HLS DATAFLOW

	//*id = MODULE_ID;
	data_t temp;

	//args_t arg_out = args[0];
	//args_t arg_in = args[1];

	for (int i = 0; i < BLOCK_SIZE_DT; ++i) {
		//#pragma HLS PIPELINE
		//printf("%p - %#X\n",mem_in,arg_in);
		//printf("%p\n",(mem_in + i));
		//printf("%p\n",(mem_in + i + (arg_in / sizeof(data_t))));
		//temp = *(mem_in + i + (arg_in / sizeof(data_t)));
		//*(mem_out + i + (arg_out / sizeof(data_t))) = temp;
		temp = mem_in[i];
		mem_out[i] = temp;
	}
}
