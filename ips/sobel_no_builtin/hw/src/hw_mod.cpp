/*
 * Sobel filter without builtin Xilinx functions and following the PREM principle (i.e. read, exec, write)
 *
 * Copyright (C) 2021, Alexandre Amory, ReTiS Lab.
 * <alexandre.amory@santannapisa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <hls_video.h>
#include <ap_fixed.h>
#include <stdint.h>
#include "sobel_top.hpp"
#include "hw_mod.hpp"

#define U32_B0(pixel) ((pixel) & 0xff)
#define U32_B1(pixel) (((pixel) >> 8) & 0xff)
#define U32_B2(pixel) (((pixel) >> 16) & 0xff)
#define U32_B3(pixel) (((pixel) >> 24) & 0xff)

// #define PERFORMANCE
// When this parameter is defined, the input memory is duplicated, removing internal dependencies in the main pipeline,
// leading the to the most optimized performance assuming the PREM approach


#ifdef PERFORMANCE
void sobel_hw(const uint8_t img_in1[IMG_HEIGHT][IMG_WIDTH], const uint8_t img_in2[IMG_HEIGHT][IMG_WIDTH], uint8_t img_out[IMG_HEIGHT][IMG_WIDTH]);
void to_gray(volatile data_t *mem_port_in, uint8_t src_img1[IMG_HEIGHT][IMG_WIDTH], uint8_t src_img2[IMG_HEIGHT][IMG_WIDTH], args_t args);
#else
void sobel_hw(const uint8_t img_in1[IMG_HEIGHT][IMG_WIDTH], uint8_t img_out[IMG_HEIGHT][IMG_WIDTH]);
void to_gray(volatile data_t *mem_port_in, uint8_t src_img1[IMG_HEIGHT][IMG_WIDTH], args_t args);
#endif

// reference function
#ifdef PERFORMANCE
void sobel_hw(const uint8_t img_in1[IMG_HEIGHT][IMG_WIDTH], const uint8_t img_in2[IMG_HEIGHT][IMG_WIDTH], uint8_t img_out[IMG_HEIGHT][IMG_WIDTH])
#else
void sobel_hw(const uint8_t img_in1[IMG_HEIGHT][IMG_WIDTH], uint8_t img_out[IMG_HEIGHT][IMG_WIDTH])
#endif
{
	uint8_t window_i[3][3];
	uint32_t row,col;
	int sum1,sum2;

	const int height = IMG_HEIGHT;
	const int width = IMG_WIDTH;
	// it seems that we could avoid setting the edges to black by using the reset. TODO: confirm it 
	#pragma HLS reset variable=img_out

	// for synthesis, this is not required, but for simulation it its. Otherwise the simulation wont result in a match
	#ifndef PERFORMANCE
	edges:{
		//#pragma HLS PIPELINE rewind
		row_edge:for(row=1; row < height-1; row++)
		{
			img_out[row][0] = 0;
			img_out[row][width-1] = 0;
		}
		col_edge:for(col=0; col < width; col++)
		{
			img_out[0][col] = 0;
			img_out[height-1][col] = 0;
		}
	}
	#endif

	// apply the filter using a rolling window
	for(row=0; row < height-2; row++)
	{
		for(col=0; col < width; col++)
		{	
			//#pragma HLS UNROLL FACTOR=8
			#pragma HLS PIPELINE
			// loading the initial values of the rolling windows
			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = img_in1[row+0][col];
			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = img_in1[row+1][col];
			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2];
			#ifdef PERFORMANCE
				window_i[2][2] = img_in2[row+2][col];
			#else
				window_i[2][2] = img_in1[row+2][col];
			#endif
			if (col >= 2){
				//Optimized way with no multiplication and replacing A+A == A<<1
				sum1 = (-window_i[0][0]) + 0 + (window_i[0][2]) +
					//(-window_i[1][0] - window_i[1][0]) + 0 + (window_i[1][2] + window_i[1][2]) + 
					(-(window_i[1][0] << 1)) + 0 + (window_i[1][2] << 1) + 
					(-window_i[2][0]) + 0 + (window_i[2][2]);

				//sum2 = (-window_i[0][0]) + (-window_i[0][1] - window_i[0][1]) + (-window_i[0][2]) + 
				sum2 = (-window_i[0][0]) + (-(window_i[0][1] << 1)) + (-window_i[0][2]) + 
					0 + 0 + 0 +
					//(window_i[2][0]) + (window_i[2][1] + window_i[2][1]) + (window_i[2][2]);
					(window_i[2][0]) + (window_i[2][1] << 1) + (window_i[2][2]);

				//Non-optimized method
				/*
				sum1 = (-1 * inImg[row-1][col-1]) + 
				(1 * inImg[row-1][col+1]) +
				(-2 * inImg[row][col-1]) + 
				(2 * inImg[row][col+1]) + 
				(-1 * inImg[row+1][col-1]) + 
				(1 * inImg[row+1][col+1]);

				sum2 = (-1 * inImg[row-1][col-1]) + 
				(-2 * inImg[row-1][col]) +
				(-1 * inImg[row-1][col+1]) + 
				(1 * inImg[row+1][col-1]) + 
				(2 * inImg[row+1][col]) +
				(1 * inImg[row+1][col+1]);
				*/

				if(sum1 < 0)
				{
					sum1 = -sum1;
				}
					if(sum2 < 0)
				{
					sum2 = -sum2;
				}
				img_out[row+1][col-1] = sum1 + sum2;
			}
		}
	}
}

#ifdef PERFORMANCE
void to_gray(volatile data_t *mem_port_in, uint8_t src_img1[IMG_HEIGHT][IMG_WIDTH], uint8_t src_img2[IMG_HEIGHT][IMG_WIDTH], args_t args)
#else
void to_gray(volatile data_t *mem_port_in, uint8_t src_img1[IMG_HEIGHT][IMG_WIDTH], args_t args)
#endif
{
	uint8_t red,green,blue,pix8;
	// access the input data byte by byte instead of dta_t
	data_t pix;
	uint32_t idx=0;

	data_t * mem_in  = (data_t *)mem_port_in + (args / sizeof(data_t));
	//uint8_t *img_ptr = (uint8_t *)mem_in;

	data_in1:for (int i = 0; i<IMG_HEIGHT; i++)
	{
		//uint8_t gray_pix;
		data_in2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
            //red = (uint8_t)*(img_ptr++);
            //green = (uint8_t)*(img_ptr++);
            //blue =(uint8_t)*(img_ptr++);
			pix = mem_in[idx];
            red = U32_B0(pix);
            green = U32_B1(pix);
			blue = U32_B2(pix);
			// avoid floating point math, but produces an image a bit darker
            // original conversion
			//in_gray_buffer[row][col] = (0.3*red) + (0.59*green) + (0.11*blue);
			pix8 = (uint8_t)(red>>2) + (uint8_t)(green>>1) + (uint8_t)(blue>>2);
			src_img1[i][j] = pix8;
			#ifdef PERFORMANCE
				src_img2[i][j] = pix8;
			#endif
			// jump by the 5 unused bytes of data_t
			//img_ptr +=5;
			idx++;
		}
	}
}

void from_gray(const uint8_t dest_img[IMG_HEIGHT][IMG_WIDTH], volatile data_t *mem_port_out,args_t args){

	// Write image back main memory
	uint32_t itr=0;
	data_t * mem_out = (data_t *)mem_port_out + (args / sizeof(data_t));

	data_out1:for (int i = 0; i<IMG_HEIGHT; i++)
	{
		data_out2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
			mem_out[itr] = (data_t)dest_img[i][j];
			itr++;
		}
	}
}

//////////////////////////////////////////////////////////////////
// NO FUNCTION CALLS w WINDOWS
//////////////////////////////////////////////////////////////////
void hw_mod(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out, args_t args0, args_t args1)
{
	// the input memory is duplicated to reduce the dependencies in the main pipeline
	uint8_t src_img1[IMG_HEIGHT][IMG_WIDTH];
#ifdef PERFORMANCE
	uint8_t src_img2[IMG_HEIGHT][IMG_WIDTH];
#endif
	uint8_t dest_img[IMG_HEIGHT][IMG_WIDTH];

	// partial partitions, like the one below, are not enough to break the dependency.
	//#pragma HLS ARRAY_PARTITION variable=src_img cyclic factor=4
	//#pragma HLS ARRAY_RESHAPE variable=src_img block factor=4
	// Only complete partition, like the one below, can break the dependency, doubling the amount of BRAMs.
	//#pragma HLS ARRAY_PARTITION variable = src_img dim = 1 complete
	#pragma HLS DATAFLOW

#ifdef PERFORMANCE
	to_gray(mem_port_in,src_img1,src_img2,args0);
#else
	to_gray(mem_port_in,src_img1,args0);
#endif
	
	// sobel
#ifdef PERFORMANCE
	sobel_hw(src_img1,src_img2, dest_img);
#else
	sobel_hw(src_img1, dest_img);
#endif

	from_gray(dest_img, mem_port_out,args1);
}

