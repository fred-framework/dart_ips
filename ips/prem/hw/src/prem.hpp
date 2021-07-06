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

// Export for test bench
static const args_t MODULE_ID = 1;

const uint32_t IN_BUFF_SIZE =  64;
const uint32_t IN_BUFF_SIZE_BYTE = (sizeof(data_t) * IN_BUFF_SIZE);
const uint32_t EXEC_SIZE = 1024;
const uint32_t OUT_BUFF_SIZE = 64;
const uint32_t OUT_BUFF_SIZE_BYTE = (sizeof(data_t) * OUT_BUFF_SIZE);

void prem(volatile data_t *mem_in, volatile data_t *mem_out);

#endif /* prem_H */
