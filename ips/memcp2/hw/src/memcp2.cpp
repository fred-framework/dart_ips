/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include "memcp2.hpp"

void memcpy(volatile data_t *mem_port_in, volatile data_t *mem_port_out)
{
    data_t val;

    //copy data
    for (unsigned i = 0; i < BUFF_SIZE; i++) {
        val = mem_port_in[i]; 
        mem_port_out[i] = val;       
    }
}
