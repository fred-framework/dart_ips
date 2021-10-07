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

/*
//////////////////////////////////////////////////////////////////
// NO FUNCTION CALLS wo WINDOWS
//////////////////////////////////////////////////////////////////
void hw_mod(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out, args_t args0, args_t args1)
{
	uint8_t src_img[IMG_HEIGHT*IMG_WIDTH];
	uint8_t dest_img[IMG_HEIGHT*IMG_WIDTH];

	data_t * mem_in  = (data_t *)mem_port_in + (args0 / sizeof(data_t));
	data_t * mem_out = (data_t *)mem_port_out + (args1 / sizeof(data_t));

	uint32_t itr = 0,idx;
	data_t pix;
	data_in1:for (int i = 0; i<IMG_HEIGHT; i++)
	{
		data_in2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
			pix = mem_in[itr];
			// avoid floating point match, but produces a image a bit darker
			src_img[itr] = U32_B0(pix)>>2 + U32_B1(pix)>>1 + U32_B2(pix)>>2;
			// copy the input image
			dest_img[itr] = src_img[itr];
			itr++;
		}
	}
//#pragma HLS stream variable=mat_image_in //depth=8
//#pragma HLS stream variable=mat_image_out //depth=8
//#pragma HLS ARRAY_PARTITION variable=src_img cyclic factor=2 dim=2
//#pragma HLS ARRAY_PARTITION variable=dest_img cyclic factor=2 dim=2

//#pragma HLS dataflow


	// Apply the filter stack
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
//	uint8_t window_i[3][3];
//	uint8_t window_o[3][3];
//
//	#pragma HLS ARRAY_PARTITION variable=window_i complete
//	#pragma HLS ARRAY_PARTITION variable=window_o complete
	#pragma HLS ARRAY_PARTITION variable=sobel_x complete
	#pragma HLS ARRAY_PARTITION variable=sobel_y complete

	sobel1:for (int j = 0; j<IMG_HEIGHT-2; j++)
	{
		//#pragma HLS PIPELINE
		// loading the initial values of the rolling windows
//		window_i[0][0] = src_img[j*IMG_WIDTH+0]; window_i[0][1] = src_img[(j+1)*IMG_WIDTH+0] ; window_i[0][2] = src_img[(j+2)*IMG_WIDTH+0];
//		window_i[1][0] = src_img[j*IMG_WIDTH+1]; window_i[1][1] = src_img[(j+1)*IMG_WIDTH+1] ; window_i[1][2] = src_img[(j+2)*IMG_WIDTH+1];
//		window_i[2][0] = src_img[j*IMG_WIDTH+2]; window_i[2][1] = src_img[(j+1)*IMG_WIDTH+2] ; window_i[2][2] = src_img[(j+2)*IMG_WIDTH+2];
//
//		window_o[0][0] = dest_img[j*IMG_WIDTH+0]; window_o[0][1] = dest_img[(j+1)*IMG_WIDTH+0] ; window_o[0][2] = dest_img[(j+2)*IMG_WIDTH+0];
//		window_o[1][0] = dest_img[j*IMG_WIDTH+1]; window_o[1][1] = dest_img[(j+1)*IMG_WIDTH+1] ; window_o[1][2] = dest_img[(j+2)*IMG_WIDTH+1];
//		window_o[2][0] = dest_img[j*IMG_WIDTH+2]; window_o[2][1] = dest_img[(j+1)*IMG_WIDTH+2] ; window_o[2][2] = dest_img[(j+2)*IMG_WIDTH+2];

//		idx = (j+2)*IMG_WIDTH;
		sobel2:for (int i = 0; i<IMG_WIDTH-2; i++)
		{
			#pragma HLS PIPELINE
			uint32_t pixval_x =
				(sobel_x[0][0] * src_img[j*IMG_WIDTH+0]) + (sobel_x[0][1] * src_img[(j+1)*IMG_WIDTH+0]) + (sobel_x[0][2] * src_img[(j+2)*IMG_WIDTH+0]) +
				(sobel_x[1][0] * src_img[j*IMG_WIDTH+1]) + (sobel_x[1][1] * src_img[(j+1)*IMG_WIDTH+1]) + (sobel_x[1][2] * src_img[(j+2)*IMG_WIDTH+1]) +
				(sobel_x[2][0] * src_img[j*IMG_WIDTH+2]) + (sobel_x[2][1] * src_img[(j+1)*IMG_WIDTH+2]) + (sobel_x[2][2] * src_img[(j+2)*IMG_WIDTH+2]);

			//cout<<"Display i: "<< i <<endl;
			//cout<<"Display j: "<< j <<endl;
			//newimg.at(j,i) = pixval;
			//newimg.at(j,i) = pixval_x;

			uint32_t pixval_y =
				(sobel_y[0][0] * dest_img[j*IMG_WIDTH+0]) + (sobel_y[0][1] * dest_img[(j+1)*IMG_WIDTH+0]) + (sobel_y[0][2] * dest_img[(j+2)*IMG_WIDTH+0]) +
				(sobel_y[1][0] * dest_img[j*IMG_WIDTH+1]) + (sobel_y[1][1] * dest_img[(j+1)*IMG_WIDTH+1]) + (sobel_y[1][2] * dest_img[(j+2)*IMG_WIDTH+1]) +
				(sobel_y[2][0] * dest_img[j*IMG_WIDTH+2]) + (sobel_y[2][1] * dest_img[(j+1)*IMG_WIDTH+2]) + (sobel_y[2][2] * dest_img[(j+2)*IMG_WIDTH+2]);

			int sum = ABS(pixval_x) + ABS(pixval_y);
			if (sum > 255)
			{
				sum = 255; //for best performance
			}
			//cout << sum << endl;
			dest_img[j*IMG_WIDTH+i] = sum;

			// rolling the windows maximizing internal memory reuse and minimizing the input bandwidth
//			idx += i;
//			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = src_img[idx];
//			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = src_img[idx+1];
//			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2]; window_i[2][2] = src_img[idx+2];
//
//			window_o[0][0] = window_o[0][1]; window_o[0][1] = window_o[0][2]; window_o[0][2] = dest_img[idx];
//			window_o[1][0] = window_o[1][1]; window_o[1][1] = window_o[1][2]; window_o[1][2] = dest_img[idx+1];
//			window_o[2][0] = window_o[2][1]; window_o[2][1] = window_o[2][2]; window_o[2][2] = dest_img[idx+2];
		}
     }



	// Write image back main memory
	itr=0;
	data_out1:for (int i = 0; i<IMG_HEIGHT; i++)
	{
		data_out2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
			pix = (data_t)dest_img[itr];
			mem_out[itr] = pix;
			itr++;
		}
	}
}
*/


//////////////////////////////////////////////////////////////////
// NO FUNCTION CALLS w WINDOWS
//////////////////////////////////////////////////////////////////
void hw_mod(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out, args_t args0, args_t args1)
{
	uint8_t src_img[IMG_HEIGHT*IMG_WIDTH];
	uint8_t dest_img[IMG_HEIGHT*IMG_WIDTH];

	// make sure that the consecutive addresses are in different memories to improve the pipeline
//	#pragma HLS ARRAY_RESHAPE variable=src_img cyclic factor=4
//	#pragma HLS ARRAY_RESHAPE variable=dest_img cyclic factor=4

	data_t * mem_in  = (data_t *)mem_port_in + (args0 / sizeof(data_t));
	data_t * mem_out = (data_t *)mem_port_out + (args1 / sizeof(data_t));

	uint32_t itr = 0,idx;
	data_t pix;
	data_in1:for (int i = 0; i<IMG_HEIGHT; i++)
	{
		uint8_t gray_pix;
		data_in2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE rewind
			pix = mem_in[itr];
			// avoid floating point match, but produces an image a bit darker
			gray_pix = U32_B0(pix)>>2 + U32_B1(pix)>>1 + U32_B2(pix)>>2;
			src_img[itr] = gray_pix;
			// copy the input gray image
			dest_img[itr] = gray_pix;
			itr++;
		}
	}
//#pragma HLS stream variable=mat_image_in //depth=8
//#pragma HLS stream variable=mat_image_out //depth=8
//#pragma HLS ARRAY_PARTITION variable=src_img cyclic factor=2 dim=2
//#pragma HLS ARRAY_PARTITION variable=dest_img cyclic factor=2 dim=2

//#pragma HLS dataflow


	// Apply the filter stack
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

	#pragma HLS ARRAY_PARTITION variable=window_i complete
	#pragma HLS ARRAY_PARTITION variable=window_o complete
	#pragma HLS ARRAY_PARTITION variable=sobel_x complete
	#pragma HLS ARRAY_PARTITION variable=sobel_y complete

	sobel1:for (int j = 0; j<IMG_HEIGHT-2; j++)
	{
		//#pragma HLS PIPELINE
		// loading the initial values of the rolling windows
		window_i[0][0] = src_img[j*IMG_WIDTH+0]; window_i[0][1] = src_img[(j+1)*IMG_WIDTH+0] ; window_i[0][2] = src_img[(j+2)*IMG_WIDTH+0];
		window_i[1][0] = src_img[j*IMG_WIDTH+1]; window_i[1][1] = src_img[(j+1)*IMG_WIDTH+1] ; window_i[1][2] = src_img[(j+2)*IMG_WIDTH+1];
		window_i[2][0] = src_img[j*IMG_WIDTH+2]; window_i[2][1] = src_img[(j+1)*IMG_WIDTH+2] ; window_i[2][2] = src_img[(j+2)*IMG_WIDTH+2];

		window_o[0][0] = dest_img[j*IMG_WIDTH+0]; window_o[0][1] = dest_img[(j+1)*IMG_WIDTH+0] ; window_o[0][2] = dest_img[(j+2)*IMG_WIDTH+0];
		window_o[1][0] = dest_img[j*IMG_WIDTH+1]; window_o[1][1] = dest_img[(j+1)*IMG_WIDTH+1] ; window_o[1][2] = dest_img[(j+2)*IMG_WIDTH+1];
		window_o[2][0] = dest_img[j*IMG_WIDTH+2]; window_o[2][1] = dest_img[(j+1)*IMG_WIDTH+2] ; window_o[2][2] = dest_img[(j+2)*IMG_WIDTH+2];

		idx = (j+2)*IMG_WIDTH;
		sobel2:for (int i = 0; i<IMG_WIDTH-2; i++)
		{
			#pragma HLS PIPELINE

			pix_x: int pixval_x =
				(sobel_x[0][0] * window_i[0][0]) + (sobel_x[0][1] * window_i[0][1]) + (sobel_x[0][2] * window_i[0][2]) +
				(sobel_x[1][0] * window_i[1][0]) + (sobel_x[1][1] * window_i[1][1]) + (sobel_x[1][2] * window_i[1][2]) +
				(sobel_x[2][0] * window_i[2][0]) + (sobel_x[2][1] * window_i[2][1]) + (sobel_x[2][2] * window_i[2][2]);

			pix_y: int pixval_y =
				(sobel_y[0][0] * window_o[0][0]) + (sobel_y[0][1] * window_o[0][1]) + (sobel_y[0][2] * window_o[0][2]) +
				(sobel_y[1][0] * window_o[1][0]) + (sobel_y[1][1] * window_o[1][1]) + (sobel_y[1][2] * window_o[1][2]) +
				(sobel_y[2][0] * window_o[2][0]) + (sobel_y[2][1] * window_o[2][1]) + (sobel_y[2][2] * window_o[2][2]);

			int sum = ABS(pixval_x) + ABS(pixval_y);
			if (sum > 255)
			{
				sum = 255; //for best performance
			}
			out_assign: dest_img[j*IMG_WIDTH+i] = sum;

			// rolling the windows maximizing internal memory reuse and minimizing the input bandwidth
			idx += i;
			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = src_img[idx];
			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = src_img[idx+1];
			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2]; window_i[2][2] = src_img[idx+2];

			window_o[0][0] = window_o[0][1]; window_o[0][1] = window_o[0][2]; window_o[0][2] = dest_img[idx];
			window_o[1][0] = window_o[1][1]; window_o[1][1] = window_o[1][2]; window_o[1][2] = dest_img[idx+1];
			window_o[2][0] = window_o[2][1]; window_o[2][1] = window_o[2][2]; window_o[2][2] = dest_img[idx+2];
		}
     }


	// Write image back main memory
	itr=0;
	data_out1:for (int i = 0; i<IMG_HEIGHT; i++)
	{
		data_out2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
			pix = (data_t)dest_img[itr];
			mem_out[itr] = pix;
			itr++;
		}
	}
}

/*
//////////////////////////////////////////////////////////////////
// WITH FUNCTION CALLS
//////////////////////////////////////////////////////////////////
void hw_mod(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out, args_t args0, args_t args1)
{
	uint8_t src_img[IMG_HEIGHT][IMG_WIDTH];
	uint8_t dest_img[IMG_HEIGHT][IMG_WIDTH];
//#pragma HLS stream variable=mat_image_in //depth=8
//#pragma HLS stream variable=mat_image_out //depth=8
//#pragma HLS ARRAY_PARTITION variable=src_img cyclic factor=2 dim=2
//#pragma HLS ARRAY_PARTITION variable=dest_img cyclic factor=2 dim=2

//#pragma HLS dataflow

	// Read image from main memory
	read_image(mem_port_in + (args0 / sizeof(data_t)), (uint8_t *)src_img);

	// Apply the filter stack
	sobel((uint8_t *)src_img, (uint8_t *)dest_img);

	// Write image back main memory
	write_image(mem_port_out + (args1 / sizeof(data_t)), (uint8_t *)dest_img);
}
*/
