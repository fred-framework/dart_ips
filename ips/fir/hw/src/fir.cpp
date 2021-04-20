/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include "fir.hpp"

void fir(volatile data_t *mem_port_in, volatile data_t *mem_port_out)
{
    data_t in_buff [BUFF_SIZE], out_buff [BUFF_SIZE];

    //fetch data
fetch_loop:for (unsigned i = 0; i < BUFF_SIZE; i++) {
                in_buff[i] = mem_port_in[i];        
            }

    //compute
    //FIR window size
    const unsigned N  = 25;

    int j, i;

    //FIR coeff
    int coeff[FIR_WINDOW_SIZE] = {13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6};
    //int coeff[FIR_WINDOW_SIZE] = {13, -2, 9, 11, 26, 18, 95, -43, 6, 74};

    //Shift registersint
    int shift_reg[FIR_WINDOW_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //int shift_reg[FIR_WINDOW_SIZE] ={0, 0};//, 0, 0, 0, 0, 0, 0, 0, 0};

    // loop through each output
    for (i = 0; i < BUFF_SIZE; i++ ) {
        int acc = 0;

        // shift registers
        for (j = FIR_WINDOW_SIZE - 1; j > 0; j--) {
            shift_reg[j] = shift_reg[j -1];
        }

        // put the new input value into the first register
        shift_reg[0] = in_buff[i];

        // do multiply-accumulate operation
        for (j = 0; j < FIR_WINDOW_SIZE; j++) {
            acc += shift_reg[j] * coeff[j];
        }

        out_buff[i] = acc;

    }
    
    //store
    store_loop:for (unsigned i = 0; i < BUFF_SIZE; i++) {
                   mem_port_out[i] = out_buff[i];
               }
}
