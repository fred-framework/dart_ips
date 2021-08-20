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

#include "fastx_top.hpp"
#include "hw_mod.hpp"
#include "img_stream.hpp"
//#include "data.hpp"

template<int ROWS, int COLS, int T>
static inline void processing_stack_fastx(hls::Mat<ROWS, COLS, T> &src_image, hls::Mat<ROWS, COLS, T> &dest_image)
{
	hls::Mat<ROWS, COLS, T> img_proc;

	hls::Mat<ROWS, COLS, T> img_back_gray_color;
	hls::Mat<ROWS, COLS, HLS_8UC1> img_in_gray;
	hls::Mat<ROWS, COLS, HLS_8UC1> img_proc_gray;
	hls::Mat<ROWS, COLS, HLS_8UC1> img_back_gray;
	hls::Mat<ROWS, COLS, HLS_8UC1> mask;
	hls::Mat<ROWS, COLS, HLS_8UC1> big_mask;

	hls::Scalar<HLS_MAT_CN(HLS_8UC3), HLS_TNAME(HLS_8UC3)> mask_color(255, 230, 0);

	const int fastx_treshold = 25;

#pragma HLS stream depth=8000 variable=img_back_gray_color.data_stream
#pragma HLS dataflow

	hls::CvtColor<HLS_RGB2GRAY>(src_image, img_in_gray);
	hls::Duplicate(img_in_gray, img_proc_gray, img_back_gray);

	hls::FASTX(img_proc_gray, mask, fastx_treshold, true);
	hls::Dilate(mask, big_mask);

	hls::CvtColor<HLS_GRAY2RGB>(img_back_gray, img_back_gray_color);
	hls::PaintMask(img_back_gray_color, big_mask, dest_image, mask_color);
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
	processing_stack_fastx(src_img, dest_img);

	// Write image back main memory
	write_image_32_(mem_port_out + (args1 / sizeof(data_t)), dest_img);
}

