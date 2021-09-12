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

#ifndef SLOTS_HPP
#define SLOTS_HPP

#include <stdint.h>

// do not change these parameters because they match with the static design produced by DART
typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;
// change this to allow longer inputs/outputs
const uint32_t AXIM_MAX_DATA_SIZE = 32;

void prem_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

#endif /* SLOTS_HPP */
