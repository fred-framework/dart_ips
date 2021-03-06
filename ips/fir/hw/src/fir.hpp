/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#ifndef FIR_HPP
#define FIR_HPP
#include "fir_top.hpp"

const args_t MODULE_ID = 202;

//const unsigned int MAT_SIZE = 64;
//const int iterations = 30;
const unsigned int BUFF_SIZE = 4 * 1024;
const unsigned int FIR_WINDOW_SIZE = 100;
const unsigned int OUT_BUFF_SIZE = BUFF_SIZE + FIR_WINDOW_SIZE - 1;
const int iterations = 1;

// Use this variable to constraint the total number of multipliers, 
// otherwise it will use all the available DSPs and the design will not be feasible.
// Tweak this value for the target platform to tradeoff area and performance
const unsigned int FIR_NUM_MULT = 1;

void fir(volatile data_t *mem_port_in, volatile data_t *mem_port_out);

#endif /* FIR_HPP */
