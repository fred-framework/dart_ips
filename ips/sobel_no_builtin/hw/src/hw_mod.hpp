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

#ifndef HW_MOD_HPP
#define HW_MOD_HPP

// assuming the Pynq board, the max sizes are about:
// - when PERFORMANCE is defined: 256x256
// - when PERFORMANCE is not defined: 512x512
static const unsigned int IMG_WIDTH = 256;
static const unsigned int IMG_HEIGHT = 256;

void hw_mod(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out, args_t args0, args_t args1);

#endif /* HW_MOD_HPP */


