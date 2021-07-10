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

#define IN_MEM_SIZE_BYTE (sizeof(data_t) * IN_MEM_SIZE)
#define OUT_MEM_SIZE_BYTE (sizeof(data_t) * OUT_MEM_SIZE)
// the input and output time does not count in the prem model; 30 is the constant additional latency of the internal pipeline
#define EXEC_SIZE EXEC_CYCLES-30-IN_MEM_SIZE-OUT_MEM_SIZE

void prem(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

#endif /* prem_H */
