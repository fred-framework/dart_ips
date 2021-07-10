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

#ifndef memcpy_H
#define memcpy_H

#include "memcpy_top.hpp"

// Export for test bench
static const args_t MODULE_ID = 1;

const unsigned int BUFF_SIZE = 1024;
const unsigned int BUFF_SIZE_BYTE = (sizeof(data_t) * 1024);

void memcpy(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

#endif /* memcpy_H */
