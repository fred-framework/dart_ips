/*
 * Sobel filter without builtin Xilinx functions
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

#define U32_PACK(B3, B2, B1, B0) (((B3) << 24) | ((B2) << 16) | ((B1) << 8) | (B0))

#define ABS(x) ((x)>0?(x):-(x))

// read an RGB image and transform it to gray scale
static inline void read_image(volatile data_t *img_addr, uint8_t *mat_image){
	uint32_t itr = 0;
	data_t pix;
	for (int i = 0; i<IMG_HEIGHT; i++)
	{
		for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
			pix = img_addr[itr];
			// avoid floating point match, but produces a image a bit darker
			mat_image[itr] = U32_B0(pix)>>2 + U32_B1(pix)>>1 + U32_B2(pix)>>2;
			itr++;
		}
	}
}

static inline void write_image(volatile data_t *img_addr, uint8_t *mat_image){
	uint32_t itr = 0;
	data_t pix;
	for (int i = 0; i<IMG_HEIGHT; i++)
	{
		for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
			pix = (data_t)mat_image[itr];
			img_addr[itr] = pix;
			itr++;
		}
	}
}

static inline void sobel(uint8_t *img_in, uint8_t *img_out){
	const int sobel_x[3][3] = {
			{-1,0,1},
			{-2,0,2},
			{-1,0,1}
		};
	const int sobel_y[3][3] = {
			{-1,-2,-1},
			{0,0,0},
			{1,2,1}
	};
	uint8_t window_i[3][3];
	uint8_t window_o[3][3];
	uint32_t idx;

	#pragma HLS ARRAY_PARTITION variable=window_i complete
	#pragma HLS ARRAY_PARTITION variable=window_o complete
	#pragma HLS ARRAY_PARTITION variable=sobel_x complete
	#pragma HLS ARRAY_PARTITION variable=sobel_y complete

	for (int j = 0; j<IMG_HEIGHT-2; j++)
	{
		//#pragma HLS PIPELINE
		// loading the initial values of the rolling windows
		window_i[0][0] = img_in[j*IMG_WIDTH+0]; window_i[0][1] = img_in[(j+1)*IMG_WIDTH+0] ; window_i[0][2] = img_in[(j+2)*IMG_WIDTH+0];
		window_i[1][0] = img_in[j*IMG_WIDTH+1]; window_i[1][1] = img_in[(j+1)*IMG_WIDTH+1] ; window_i[1][2] = img_in[(j+2)*IMG_WIDTH+1];
		window_i[2][0] = img_in[j*IMG_WIDTH+2]; window_i[2][1] = img_in[(j+1)*IMG_WIDTH+2] ; window_i[2][2] = img_in[(j+2)*IMG_WIDTH+2];

		window_o[0][0] = img_out[j*IMG_WIDTH+0]; window_o[0][1] = img_out[(j+1)*IMG_WIDTH+0] ; window_o[0][2] = img_out[(j+2)*IMG_WIDTH+0];
		window_o[1][0] = img_out[j*IMG_WIDTH+1]; window_o[1][1] = img_out[(j+1)*IMG_WIDTH+1] ; window_o[1][2] = img_out[(j+2)*IMG_WIDTH+1];
		window_o[2][0] = img_out[j*IMG_WIDTH+2]; window_o[2][1] = img_out[(j+1)*IMG_WIDTH+2] ; window_o[2][2] = img_out[(j+2)*IMG_WIDTH+2];

		idx = (j+2)*IMG_WIDTH;
		for (int i = 0; i<IMG_WIDTH-2; i++)
		{
			#pragma HLS PIPELINE
			int pixval_x =
				(sobel_x[0][0] * window_i[0][0]) + (sobel_x[0][1] * window_i[0][1]) + (sobel_x[0][2] * window_i[0][2]) +
				(sobel_x[1][0] * window_i[1][0]) + (sobel_x[1][1] * window_i[1][1]) + (sobel_x[1][2] * window_i[1][2]) +
				(sobel_x[2][0] * window_i[2][0]) + (sobel_x[2][1] * window_i[2][1]) + (sobel_x[2][2] * window_i[2][2]);

			int pixval_y =
				(sobel_y[0][0] * window_o[0][0]) + (sobel_y[0][1] * window_o[0][1]) + (sobel_y[0][2] * window_o[0][2]) +
				(sobel_y[1][0] * window_o[1][0]) + (sobel_y[1][1] * window_o[1][1]) + (sobel_y[1][2] * window_o[1][2]) +
				(sobel_y[2][0] * window_o[2][0]) + (sobel_y[2][1] * window_o[2][1]) + (sobel_y[2][2] * window_o[2][2]);

			int sum = ABS(pixval_x) + ABS(pixval_y);
			if (sum > 255)
			{
				sum = 255; //for best performance
			}
			img_out[j*IMG_WIDTH+i] = sum;

			// rolling the windows maximizing internal memory reuse and minimizing the input bandwidth
			idx += i;
			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = img_in[idx];
			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = img_in[idx+1];
			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2]; window_i[2][2] = img_in[idx+2];

			window_o[0][0] = window_o[0][1]; window_o[0][1] = window_o[0][2]; window_o[0][2] = img_out[idx];
			window_o[1][0] = window_o[1][1]; window_o[1][1] = window_o[1][2]; window_o[1][2] = img_out[idx+1];
			window_o[2][0] = window_o[2][1]; window_o[2][1] = window_o[2][2]; window_o[2][2] = img_out[idx+2];
		}
     }
}


void edges(uint8_t img_out[IMG_HEIGHT][IMG_WIDTH]){
	uint32_t row,col;

	const int height = IMG_HEIGHT;
	const int width = IMG_WIDTH;
	//#pragma HLS DATAFLOW

    // set the edges to black
	for(row=0; row < height; row++)
	{
		for(col=0; col < width; col++)
		{
			#pragma HLS PIPELINE
			img_out[row][col] = 0;	//black edge
		}
	}

}

// reference function
void sobel_hw(const uint8_t img_in[IMG_HEIGHT][IMG_WIDTH], uint8_t img_out[IMG_HEIGHT][IMG_WIDTH]){
	uint8_t window_i[3][3];
	uint32_t row,col;
	int sum1,sum2;

	const int height = IMG_HEIGHT;
	const int width = IMG_WIDTH;
	//#pragma HLS DATAFLOW

    // set the edges to black

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


	// apply the filter using a rolling window
	for(row=1; row < height-1; row++)
	{

		for(col=1; col < width-1; col++)
		{	
			#pragma HLS PIPELINE

			if (row >= 1 && col ==1){
				// loading the initial values of the rolling windows
				window_i[0][0] = img_in[row-1][0]; window_i[0][1] = img_in[row-1][1] ; window_i[0][2] = img_in[row-1][2];
				window_i[1][0] = img_in[row+0][0]; window_i[1][1] = img_in[row+0][1] ; window_i[1][2] = img_in[row+0][2];
				window_i[2][0] = img_in[row+1][0]; window_i[2][1] = img_in[row+1][1] ; window_i[2][2] = img_in[row+1][2];

			}else{
	            window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = img_in[row-1][col];
	            window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = img_in[row+0][col];
	            window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2]; window_i[2][2] = img_in[row+1][col];
			}

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

            //Non-optimized method equivalent to the filter:
			/*
				const int sobel_x[3][3] = {
						{-1,0,1},
						{-2,0,2},
						{-1,0,1}
					};
				const int sobel_y[3][3] = {
						{-1,-2,-1},
						{0,0,0},
						{1,2,1}
				};			
            sum1 = (-1 * img_in[row-1][col-1]) + 
            (1 * img_in[row-1][col+1]) +
            (-2 * img_in[row][col-1]) + 
            (2 * img_in[row][col+1]) + 
            (-1 * img_in[row+1][col-1]) + 
            (1 * img_in[row+1][col+1]);

            sum2 = (-1 * img_in[row-1][col-1]) + 
            (-2 * img_in[row-1][col]) +
            (-1 * img_in[row-1][col+1]) + 
            (1 * img_in[row+1][col-1]) + 
            (2 * img_in[row+1][col]) +
            (1 * img_in[row+1][col+1]);
            */

            if(sum1 < 0)
            {
            	sum1 = -sum1;
            }
            if(sum2 < 0)
            {
            	sum2 = -sum2;
            }

			img_out[row][col] = sum1 + sum2;
			/*
            if(sum1 + sum2 > curMax)
            {
            	curMax = sum1 + sum2;
            }
			*/
		}
	}
}

void data_t_to_gray(const data_t *mem_port_in, uint8_t src_img[IMG_HEIGHT][IMG_WIDTH],args_t args){
	uint8_t red,green,blue;
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
			src_img[i][j] = (uint8_t)(red>>2) + (uint8_t)(green>>1) + (uint8_t)(blue>>2);

			// jump by the 5 unused bytes of data_t
			//img_ptr +=5;
			idx++;
		}
	}

}


void gray_to_data_t(const uint8_t dest_img[IMG_HEIGHT][IMG_WIDTH], data_t *mem_port_out,args_t args){

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
	uint8_t src_img[IMG_HEIGHT][IMG_WIDTH];
	uint8_t dest_img[IMG_HEIGHT][IMG_WIDTH];

	// make sure that the consecutive addresses are in different memories to improve the pipeline
	//#pragma HLS ARRAY_PARTITION variable=src_img cyclic factor=4
	#pragma HLS ARRAY_RESHAPE variable=src_img cyclic factor=4
	#pragma HLS DATAFLOW

	data_t_to_gray((data_t *)mem_port_in,src_img,args0);

	//edges(dest_img);
	sobel_hw(src_img,dest_img);

	gray_to_data_t(dest_img, (data_t *)mem_port_out,args1);

}

