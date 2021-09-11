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
#include "ap_int.h"

// pseudo random number generator based on an LFSR w 32 bits
// from https://highlevel-synthesis.com/2017/02/10/lfsr-in-hls/
// other possible complete polynomials https://literature.cdn.keysight.com/litweb/pdf/ads2001/ccsys/ccsys073.html
// implements a single iteration of the LFSR
unsigned int pseudo_random(unsigned int seed) {
  static ap_uint<32> lfsr;
  lfsr = seed;
  bool b_32 = lfsr.get_bit(32-32);
  bool b_22 = lfsr.get_bit(32-22);
  bool b_2 = lfsr.get_bit(32-2);
  bool b_1 = lfsr.get_bit(32-1);
  bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;
  lfsr = lfsr >> 1;
  lfsr.set_bit(31, new_bit);

  return lfsr.to_uint();
}

void prem(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	data_t count_input_val=0;
	data_t i;
	unsigned int lfsr;

	// the order of the args must be consistent w the order in the fred_bind function
	data_t *data_in = (data_t *)&mem_in[args[0] / sizeof (data_t)];
	data_t *data_out = (data_t *)&mem_out[args[1] / sizeof (data_t)];

	mem_rd_loop:for (i = 0; i < IN_MEM_SIZE; ++i) {
		#pragma HLS pipeline
		count_input_val += data_in[i];
	}

	// set the LFSR seed
	lfsr = count_input_val;
	exec_loop:for (i = 0; i < EXEC_SIZE; ++i) {
		//#pragma HLS pipeline
		lfsr = pseudo_random(lfsr);
	}

	mem_wr_loop:for (i = 0; i < OUT_MEM_SIZE; ++i) {
		#pragma HLS pipeline
		data_out[i] = lfsr;
		lfsr = pseudo_random(lfsr);
	}
}
