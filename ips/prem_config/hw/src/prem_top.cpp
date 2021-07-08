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

#include "prem_top.hpp"
#include "prem.hpp"

void prem_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	// AXI Lite control bus
	#pragma HLS INTERFACE s_axilite port=return bundle=ctrl_bus
	#pragma HLS INTERFACE s_axilite port=id bundle=ctrl_bus
	#pragma HLS INTERFACE s_axilite port=args bundle=ctrl_bus

	// AXI Master memory ports
	#pragma HLS INTERFACE m_axi port=mem_in offset=slave bundle=mem_bus
	#pragma HLS INTERFACE s_axilite port=mem_in bundle=ctrl_bus
	#pragma HLS INTERFACE m_axi port=mem_out offset=slave bundle=mem_bus
	#pragma HLS INTERFACE s_axilite port=mem_out bundle=ctrl_bus

	//prem(mem_in + args[0] / sizeof(data_t), mem_out + args[1] / sizeof(data_t));
	prem(mem_in, mem_out);
}
