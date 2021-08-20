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
//#include <hls_opencv.h>
#include <ap_fixed.h>

#include "sobel_top.hpp"
#include "hw_mod.hpp"
#include "img_stream.hpp"
#include "data.hpp"

template<int ROWS, int COLS, int T>
static inline void processing_stack_sobel(hls::Mat<ROWS, COLS, T> &src_image, hls::Mat<ROWS, COLS, T> &dest_image)
{
#pragma HLS dataflow
// On how to use Array2Mat
// https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/Usage-of-hls-Array2Mat-with-RGB-Image/td-p/903310

	hls::Mat<ROWS, COLS, HLS_8UC1> img_gray_0;
	hls::Mat<ROWS, COLS, HLS_8UC1> img_gray_1;
	// cv::Mat img0=cv::Mat::zeros(ROWS,COLS,HLS_8UC1);
	// cv::Mat img1=cv::Mat::zeros(ROWS,COLS,HLS_8UC1);
	//ap_uint<32> rgb[ROWS*COLS] = {};
	//data_t rgb[ROWS*COLS] = {};

	//hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3> rgb(MAX_HEIGHT, MAX_WIDTH);
	//hls::Mat2Array<COLS, ap_uint<32>, ROWS, COLS, HLS_8UC3>(src_image, rgb);
	//write_image_32_(rgb,src_image);


	hls::CvtColor<HLS_RGB2GRAY>(src_image, img_gray_0);
	// hlsMat2cvMat(img_gray_0, img0);
	// cv::imwrite("gray-in.jpg",img0);
	hls::Sobel<1,0,3>(img_gray_0, img_gray_1);
	// hlsMat2cvMat(img_gray_1, img1);
	// cv::imwrite("gray-out.jpg",img1);
	hls::CvtColor<HLS_GRAY2RGB>(img_gray_1, dest_image);
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
	processing_stack_sobel(src_img, dest_img);

	// Write image back main memory
	write_image_32_(mem_port_out + (args1 / sizeof(data_t)), dest_img);
}

