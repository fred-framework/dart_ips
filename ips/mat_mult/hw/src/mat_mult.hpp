/*
 * Matrix multiplication DART/Fred enabled hardware accelerator.
 *
 * Copyright (C) 2019, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 * Copyright (C) 2021, Alexandre Amory, ReTiS Lab.
 * <alexandre.amory(at)santannapisa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#ifndef HW_MOD_HPP
#define HW_MOD_HPP

// tested on the pynq board with sizes 8, 16, 32
// with size 64, the design reaches the limit of the Pynq board and it is not feasible;
#define MAT_SIZE 32

// Although "data_t" cannot be changed to work w DART, the internal data size can be changed
//  to save resources. Change "int_data_t" accordingly to change the internal data size.
typedef uint64_t int_data_t;

// ****************************************************************
// Implementation parameters to play with area vs latency tradeoff
// ****************************************************************
// The multiplication operation has a delay of 4 clock cycles. So, in order to have a perfect pipeline
// the design needs a partition factor of half the MAT_SIZE and the allocation limit must be half of the partition factor.
//  - for MAT_SIZE up to 16, choose ALLOCATION_LIMIT 4 works w PARTITION_FACTOR 8 -- latency of about 1800 using 40 DSP
//  - for MAT_SIZE up to 32, choose ALLOCATION_LIMIT 8 works w PARTITION_FACTOR 16 -- latency of about 7200 using 80 DSP
// For the pynq board, the following configuration is not feasible:
//  - for MAT_SIZE up to 64, choose ALLOCATION_LIMIT 16 works w PARTITION_FACTOR 32 -- latency of about 28700 using 160 DSP
// It uses 98% of BRAMs and 72% of DSPs. So the partition factor was relaxed to: 
//  - for MAT_SIZE up to 64, choose ALLOCATION_LIMIT 8 works w PARTITION_FACTOR 16 -- DOES NOT WORK ON THE FPGA

// PARTITION_FACTOR is related to the BRAMs allocated to the design. The memory is partitioned in smaller memories, feeding more multipliers in parallel
// ALLOCATION_LIMIT is related to the DPSs allocated to the design. It means the number of parallel multipliers

// Those configurations below were tested sucessfully in the Pynq board.
// TODO: test other configurations and latency vs area tradeoff

#if MAT_SIZE == 8
	const unsigned int PARTITION_FACTOR = 4;
	const unsigned int ALLOCATION_LIMIT = 2;
#elif MAT_SIZE == 16
	const unsigned int PARTITION_FACTOR = 8;
	const unsigned int ALLOCATION_LIMIT = 4;
#elif MAT_SIZE == 32
	const unsigned int PARTITION_FACTOR = 16;
	const unsigned int ALLOCATION_LIMIT = 8;
#else
	#warning "mat_mult configuration not tested in the FPGA"
	const unsigned int PARTITION_FACTOR = 4;
	const unsigned int ALLOCATION_LIMIT = 2;
#endif


void mat_mult(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out,
			args_t args0, args_t args1, args_t args2);

#endif /* HW_MOD_HPP */
