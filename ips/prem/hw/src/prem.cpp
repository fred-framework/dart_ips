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

void prem(volatile data_t *mem_in, volatile data_t *mem_out)
{
	data_t count_input_val=0;

	in_loop:for (int i = 0; i < IN_BUFF_SIZE; ++i) {
		count_input_val += mem_in[i];
	}

	exec_loop:for (int i = 0; i < EXEC_SIZE; ++i) {
		count_input_val += i;
	}

	out_loop:for (int i = 0; i < OUT_BUFF_SIZE; ++i) {
		mem_out[i] = count_input_val + i;
	}
}
