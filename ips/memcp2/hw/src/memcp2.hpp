/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#ifndef MEMCP_HPP
#define MEMCP_HPP
#include "memcp2_top.hpp"

const args_t MODULE_ID = 202;

const unsigned int BUFF_SIZE = 10;

void memcpy(volatile data_t *mem_port_in, volatile data_t *mem_port_out);

#endif /* MEMCP_HPP */
