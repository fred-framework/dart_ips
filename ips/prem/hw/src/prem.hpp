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

#ifndef prem_H
#define prem_H

#include "prem_top.hpp"

#define IN_MEM_SIZE 16
#define OUT_MEM_SIZE 16
#define EXEC_CYCLES 128

#define MAX(x,y) ( (x) > (y) ? (x) : (y) )

// the input and output time does not count in the prem model; 20 is the constant additional latency of the internal pipeline. with this constant, the final latency match with EXEC_CYCLES
//#define EXEC_SIZE MAX(0,EXEC_CYCLES-20-IN_MEM_SIZE-OUT_MEM_SIZE)
#define EXEC_SIZE EXEC_CYCLES

void prem(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

#endif /* prem_H */
