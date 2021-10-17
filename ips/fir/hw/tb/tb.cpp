/* 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <iostream>
#include <cstring>

const unsigned int BUFF_SIZE = 4 * 1024;
const unsigned int FIR_WINDOW_SIZE = 100;
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

void fir_reference(data_t *mem_port_in, data_t *mem_port_out)
{
    int j, i;
    int acc;
    int in_buff [BUFF_SIZE], out_buff [BUFF_SIZE];
    //Shift registers
    int shift_reg[FIR_WINDOW_SIZE];

    reset_shift_reg:for (j = 0; j < FIR_WINDOW_SIZE; j++) {
        shift_reg[j] = 0;
    }

    //fetch data
    fetch_loop:for (unsigned i = 0; i < BUFF_SIZE; i++) {
        in_buff[i] = (int)mem_port_in[i];
    }

    //FIR coeff
    const int coeff[FIR_WINDOW_SIZE] = {
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6,
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6,
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6,
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6
    };

    // loop through each output
    main_loop:for (i = 0; i < BUFF_SIZE; i++ ) {
        acc = 0;
        // shift registers
        shift_reg:for (j = FIR_WINDOW_SIZE - 1; j > 0; j--) {
            shift_reg[j] = shift_reg[j -1];
        }
        // put the new input value into the first register
        shift_reg[0] = in_buff[i];
        // do multiply-accumulate operation
        mac:for (j = 0; j < FIR_WINDOW_SIZE; j++) {
            acc += shift_reg[j] * coeff[j];
        }
        out_buff[i] = acc;
    }

    //store
    store_loop:for (i = 0; i < BUFF_SIZE; i++) {
        mem_port_out[i] = (long)out_buff[i];
    }
}


int main (int argc, char **argv)
{
    args_t id_out;
    args_t args[ARGS_SIZE]={0};
    data_t in_buff [BUFF_SIZE], out_buff[BUFF_SIZE];
    data_t out_buff_gold[BUFF_SIZE];
    int i,j,error_code = 0;

    fir_reference(in_buff,out_buff_gold);
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
