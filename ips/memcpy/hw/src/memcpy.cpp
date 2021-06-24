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

void memcpy(volatile data_t *mem_in, volatile data_t *mem_out)
{
	data_t temp;

	for (int i = 0; i < BUFF_SIZE; ++i) {
		temp = mem_in[i];
		mem_out[i] = temp;
	}
}
