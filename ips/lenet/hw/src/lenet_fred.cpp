/*
 * FRED interface for LeNet CNN.
 *
 * Copyright (C) 2021, Alexandre Amory, ReTiS Lab.
 * <alexandre.amory(at)santannapisa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include "lenet_fred.hpp"
#include "LeNet.h"

#include "parameters.h"

#if defined(FRED_REF_DATA)
void print_bin(char *str, data_t n, uint32_t nbits){
	int i=0;
	for (i = 0; i < nbits; i++)	{
		if (n & 1)
			str[i] = '1';
		else
			str[i] = '0';

		n >>= 1;
	}
	str[i] = '\0';
}
#endif

void lenet_fred(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	// the order of the args must be consistent w the order in the fred_bind function
	data_t *data_in = (data_t *)&mem_in[args[0] / sizeof (data_t)];
	data_t *data_out = (data_t *)&mem_out[args[1] / sizeof (data_t)];
	hw_fixed fred_out[image_Batch*OUTPUT_NN_2_SIZE];
	uint32_t index, batch, i, j, b;

	hw_fixed fred_in[image_Batch][INPUT_WH][INPUT_WH];
	data_t * fred_in_ptr = (data_t *)fred_in;
	uint64_t shift_reg;
	int8_t aux_byte;
	hw_fixed aux_fixed;
	float aux_float;

	index=0;
	load_batch:
	for(batch=0; batch<image_Batch; batch++){
		load_row:
		for(i=0; i<INPUT_WH; i++){
			Load_col:
			for(j=0; j<INPUT_WH/sizeof(data_t); j++){
				shift_reg = data_in[index];
				//printf("data_in[%d] = %s\n", index, shift_reg.to_string(16).c_str());
				for(b = 0; b < sizeof(data_t); b++){
					#pragma HLS unroll
					aux_byte = shift_reg & 0xFF;
					//aux_fixed = aux_byte;
					aux_float = ((float)aux_byte)/(float)DATA_CONVERT_MUL;
					fred_in[batch][i][j * sizeof(data_t) +b ] = aux_float;
					// printf("[%02d][%02d][%02d] = b %02X - fi %s - f %f\n", batch, i, j * sizeof(data_t) +b, aux_byte, aux_fixed.to_string(16).c_str(), aux_float);
					shift_reg >>= 8;
				}
				index++;
			}
		}
	}


#if defined(FRED_REF_DATA)
	FILE *fp;
	fp = fopen("fred_output_ref.dat", "w");
	if (fp == NULL)	{
	    printf("Error opening file!\n");
		exit(1);
	}
	printf("Writing the reference OUTPUT data\n");
	for(int k=0; k<image_Batch*OUTPUT_NN_2_SIZE; k++){
		//fprintf(fp,"%s\n", fred_out[k].to_string(2).c_str()); 
		fprintf(fp,"%04d. f %f - d %s - b %s - h %s\n", k, fred_out[k].to_float(), fred_out[k].to_string(10).c_str(), fred_out[k].to_string(2).c_str(), fred_out[k].to_string(16).c_str());
		//fprintf(fp,"%2X\n", input[k].to_int());//
	}
	char buffer[50];
#endif

	LeNet(fred_in,fred_out,0);


	for (i = 0; i < image_Batch*OUTPUT_NN_2_SIZE; i++){
		// 8bit fixed-size output is converted to float and assigned to 64bit integer
		data_out[i] = (data_t)(fred_out[i].to_float()*DATA_CONVERT_MUL);
	}
	//memcpy(data_out,fred_out_ptr,image_Batch*OUTPUT_NN_2_SIZE);
}
