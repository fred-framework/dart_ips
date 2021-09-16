/* 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <iostream>
#include <cstring>

const unsigned int BUFF_SIZE = 10;
const unsigned int FIR_WINDOW_SIZE = 25;
const unsigned int OUT_BUFF_SIZE = BUFF_SIZE + FIR_WINDOW_SIZE - 1;

typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;

void fir_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

void print_vect(data_t * base_idx, unsigned int size)
{
	for (int i = 0; i < size; ++i) {
		std::cout << i << "\t" << base_idx[i] << "\n";
	}
	std::cout << std::endl;
}

int main (int argc, char **argv)
{
    args_t id_out;
    args_t args[ARGS_SIZE];
    data_t in_buff [BUFF_SIZE], out_buff[BUFF_SIZE];
    data_t out_buff_gold[BUFF_SIZE];
    int i,j,error_code = 0;

    //FIR coeff
    data_t coeff[FIR_WINDOW_SIZE] = {13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6};

    //Shift registers
    data_t shift_reg[FIR_WINDOW_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    //prepare input data
    for(i = 0; i < BUFF_SIZE; i++)
        in_buff[i] = (data_t) i;

    // running the reference output
    for (i = 0; i < BUFF_SIZE; i++ ) {
        data_t acc = 0;

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
        out_buff_gold[i] = acc;
    }

    //call fir accelerator
    fir_top(&id_out, args, in_buff, out_buff);

    //validate
    if (memcmp(out_buff_gold, out_buff,BUFF_SIZE*sizeof(data_t))){
        std::cout << "Mismatch!\n";
        error_code = 1;
    }else{
        std::cout << "Match!\n";
    }
    // print only the 5 initial lines of the images
    std::cout << "Expected value: \n";
    print_vect((data_t*)out_buff_gold, 5);
    std::cout << "Output value  : \n";
    print_vect((data_t*)out_buff, 5);

    return error_code;
}
