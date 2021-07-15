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

#include "crc.hpp"

void crc(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	data_t crc;
	uint32_t i,b, len;
	uint8_t word[8];
	data_t *word64=(data_t*)word;

	// the order of the args must be consistent w the order in the fred_bind function
	data_t *data_in = (data_t *)&mem_in[args[0] / sizeof (data_t)];
	data_t *data_out = (data_t *)&mem_out[args[1] / sizeof (data_t)];

	len = (uint32_t)data_in[0];
	data_in ++; // skipping the 1st word
	crc = 0;

	while(len--){
		*word64 = *data_in;
		data_in++;
		// 8 bytes of words of 64 bits
		for(b = 0; b < 8; ++b){
			#pragma HLS pipeline
			crc ^= (uint64_t)word[b] << 56; 
			for(i = 0; i < 8; ++i)
				#pragma HLS unroll
				crc = crc << 1 ^ (crc & 0x8000000000000000 ? 0x42f0e1eba9ea3693 : 0x0000000000000000); 
		}
	}
	data_out[0] = crc;
}
