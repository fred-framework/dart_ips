/* 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include "memcp2.hpp"

int main (int argc, char **argv)
{
    const unsigned N = 10;
    const unsigned OUT_BUFF_SIZE = BUFF_SIZE + FIR_WINDOW_SIZE - 1;
    //const unsigned N = 25;

    data_t in_buff [BUFF_SIZE], out_buff[BUFF_SIZE];
    data_t in_buff_gold [BUFF_SIZE], out_buff_gold[BUFF_SIZE];

    //FIR coeff
    int coeff[FIR_WINDOW_SIZE] = {13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6};
    // int coeff[FIR_WINDOW_SIZE] = {13, -2, 9, 11, 26, 18, 95, -43, 6, 74};
    //int coeff[FIR_WINDOW_SIZE] = {13, -2};//, 9, 11, 26, 18, 95, -43, 6, 74};

    //prepare input data
    for(unsigned i = 0; i < BUFF_SIZE; i++)
        in_buff[i] = (data_t) i;

    for(unsigned n = 0; n < BUFF_SIZE; n++) {
        out_buff_gold[n] = 0;    
        for (unsigned i = 0; i < FIR_WINDOW_SIZE; i++){    
            int temp = (int) n - i;
            if(temp >= 0)
                out_buff_gold[n] += coeff[i] * in_buff[n - i];
            }
        }

    //call memcp2 accelerator
    memcp2(in_buff, out_buff);

    //validate
    int errors = 0;
    for (unsigned j = 0; j < BUFF_SIZE; j++) {
        if (out_buff[j] != out_buff_gold[j])
            errors++;
    }

    if (errors)
        std::cout << "Test FAILED with " << errors << " errors." << std::endl;
    else
        std::cout << "Test PASSED." << std::endl;
}
