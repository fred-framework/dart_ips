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

#ifndef SLOTS_HPP
#define SLOTS_HPP

#include <stdint.h>
#include "ap_int.h"

typedef uint32_t args_t;
typedef ap_uint<64> data_t;
static const uint8_t ARGS_SIZE = 8;

void memcpy(volatile data_t *in, volatile data_t *out);

#endif /* SLOTS_HPP */
