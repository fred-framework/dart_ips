/*
 * Fred hardware accelerator stub.
 *
 * Copyright (C) 2019, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <hls_video.h>
#include <ap_fixed.h>

#include "gmap_top.hpp"
#include "hw_mod.hpp"
#include "img_stream.hpp"
#include "data.hpp"

template<int ROWS, int COLS, int T>
static inline void processing_stack_gradient_map2(hls::Mat<ROWS, COLS, T> &src_image, hls::Mat<ROWS, COLS, T> &dest_image)
{
	hls::Mat<ROWS, COLS, HLS_8UC1> img_gray_0;
	hls::Mat<ROWS, COLS, HLS_8UC1> img_gray_1;
	hls::Mat<ROWS, COLS, HLS_8UC1> img_gray_2;
	hls::Scalar<HLS_MAT_CN(HLS_8UC1), HLS_TNAME(HLS_8UC1)> grayPix;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pix;
	uint32_t colorPoint;

#pragma HLS dataflow

	hls::CvtColor<HLS_RGB2GRAY>(src_image, img_gray_0);
	hls::Erode(img_gray_0, img_gray_1);
	hls::Dilate(img_gray_1, img_gray_2);

	for(int row = 0; row < ROWS; row++) {
	#pragma HLS loop_flatten off
		for(int col = 0; col < COLS; col++) {
		#pragma HLS pipeline II=1
			img_gray_2 >> grayPix;

			colorPoint = gradient_map[grayPix.val[0]];
			pix.val[0] = U32_B2(colorPoint);
			pix.val[1] = U32_B1(colorPoint);
			pix.val[2] = U32_B0(colorPoint);

			dest_image << pix;
		}
	}
}

// RGB channel order
// DEMO -----------------
// processing_stack_fastx
// processing_stack_sobel
// processing_stack_gradient_map2
//-----------------------
void hw_mod(volatile args_t *id, volatile data_t *mem_port_in, volatile data_t *mem_port_out, args_t args0, args_t args1)
{
	hls::Mat<IMG_HEIGHT, IMG_WIDTH, HLS_8UC3> src_img;
//#pragma HLS stream variable=mat_image_in //depth=8

	hls::Mat<IMG_HEIGHT, IMG_WIDTH, HLS_8UC3> dest_img;
//#pragma HLS stream variable=mat_image_out //depth=8

	*id = MODULE_ID;

#pragma HLS dataflow

	// Read image from main memory
	read_image_32_(mem_port_in + (args0 / sizeof(data_t)), src_img);

	// Apply the filter stack
	processing_stack_gradient_map2(src_img, dest_img);

	// Write image back main memory
	write_image_32_(mem_port_out + (args1 / sizeof(data_t)), dest_img);
}

