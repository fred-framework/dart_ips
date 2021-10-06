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

//static const args_t FASTX_HWT_ID = 300;
//static const args_t SOBEL_HWT_ID = 310;
//static const args_t GMAP_HWT_ID = 312;

static const args_t MODULE_ID = 100;

//static const unsigned int IMG_WIDTH = 800;
//static const unsigned int IMG_HEIGHT = 600;

// static const unsigned int IMG_WIDTH = 640;
// static const unsigned int IMG_HEIGHT = 480;

//static const unsigned int IMG_WIDTH = 64;
//static const unsigned int IMG_HEIGHT = 48;

static const unsigned int IMG_WIDTH = 512;
static const unsigned int IMG_HEIGHT = 512;

void hw_mod(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out, args_t args0, args_t args1);

#endif /* HW_MOD_HPP */


