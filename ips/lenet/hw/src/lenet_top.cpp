/*
 * FRED interface for LeNet CNN.
 *
 * Copyright (C) 2021, Alexandre Amory, ReTiS Lab.
 * <alexandre.amory(at)santannapisa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include "lenet_top.hpp"
#include "lenet_fred.hpp"

void lenet_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	// AXI Lite control bus
	#pragma HLS INTERFACE s_axilite port=return bundle=ctrl_bus
	#pragma HLS INTERFACE s_axilite port=id bundle=ctrl_bus
	#pragma HLS INTERFACE s_axilite port=args bundle=ctrl_bus

	// AXI Master memory ports
	// The only reason to define the port depth is to avoid errors in the C/RTL cosim.
	// The module works in FPGA with or without this definitions
	#pragma HLS INTERFACE m_axi depth=AXIM_MAX_DATA_SIZE port=mem_in offset=slave bundle=mem_bus
	#pragma HLS INTERFACE s_axilite port=mem_in bundle=ctrl_bus
	#pragma HLS INTERFACE m_axi depth=AXIM_MAX_DATA_SIZE port=mem_out offset=slave bundle=mem_bus
	#pragma HLS INTERFACE s_axilite port=mem_out bundle=ctrl_bus

	lenet_fred(id, args, mem_in, mem_out);
}
