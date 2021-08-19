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

#ifndef IMG_STREAM
#define IMG_STREAM

#include <hls_video.h>
#include <iostream>
#include <ap_fixed.h>

#define U32_B0(pixel) ((pixel) & 0xff)
#define U32_B1(pixel) (((pixel) >> 8) & 0xff)
#define U32_B2(pixel) (((pixel) >> 16) & 0xff)
#define U32_B3(pixel) (((pixel) >> 24) & 0xff)

#define U32_PACK(B3, B2, B1, B0) (((B3) << 24) | ((B2) << 16) | ((B1) << 8) | (B0))

using namespace std;

static inline void word32_print_(const char *text, data_t word)
{
	std::cout 	<< text << " word: ["
				<< (unsigned int)U32_B3(word) << ", "
				<< (unsigned int)U32_B2(word) << ", "
				<< (unsigned int)U32_B1(word) << ", "
				<< (unsigned int)U32_B0(word)
			    << "]" << std::endl;
}

template<int N, typename T>
static inline void pix_print_(const char *text, hls::Scalar<N, T> pixel)
{
	std::cout 	<< text << " pix: ["
				<< (unsigned int)pixel.val[0] << ", "
				<< (unsigned int)pixel.val[1] << ", "
				<< (unsigned int)pixel.val[2]
			    << "]" << std::endl;
}

template<int ROWS, int COLS, int T>
static inline void read_image_32_(volatile data_t *img_addr, hls::Mat<ROWS, COLS, T> &mat_image)
{
	data_t pixel_in_p;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_in;

	for (int line = 0; line < ROWS; ++line) {
		for (int off = 0; off < COLS; ++off) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
			// Get packed pixel from the memory
			pixel_in_p = img_addr[line * IMG_WIDTH + off];

#ifndef __SYNTHESIS__
			if (line == 0 && off == 0) {
				std::cout << "in pk: " << std::hex << pixel_in_p << std::endl;

				std::cout << "in: " << (data_t)U32_B1(pixel_in_p) << " "
							<< (data_t)U32_B2(pixel_in_p) << " " << (data_t)U32_B3(pixel_in_p)
							<< std::endl;
			}
#endif
			// Unpack pixel
			pixel_in.val[0] = U32_B2(pixel_in_p);	// R
			pixel_in.val[1] = U32_B1(pixel_in_p);	// G
			pixel_in.val[2] = U32_B0(pixel_in_p);	// B

			// Store into the Mat
			mat_image << pixel_in;
		}
	}
}

template<int ROWS, int COLS, int T>
static inline void write_image_32_(volatile data_t *img_addr, hls::Mat<ROWS, COLS, T> &mat_image)
{
	data_t pixel_out_p;
	hls::Scalar<HLS_MAT_CN(HLS_8UC3), HLS_TNAME(HLS_8UC3)> pixel_out;

	for (int line = 0; line < ROWS; ++line) {
		for (int off = 0; off < COLS; ++off) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
			// Read pixel from the Mat
			mat_image >> pixel_out;
			pixel_out_p = (data_t)U32_PACK(0, pixel_out.val[0], pixel_out.val[1], pixel_out.val[2]);

#ifndef __SYNTHESIS__
			if (line == 0 && off == 0) {
				std::cout << "out: " << (data_t)pixel_out.val[0] << " "
						<< (data_t)pixel_out.val[1] << " " <<
						(data_t)pixel_out.val[2] << std::endl;

				std::cout << "out pk: " << std::hex <<
						(data_t)U32_PACK(pixel_out.val[0], pixel_out.val[1], pixel_out.val[2], 0)
						<< std::endl;
			}
#endif
			// Store pixel into memory
			img_addr[line * IMG_WIDTH + off] = pixel_out_p;
		}
	}

}

template<int ROWS, int COLS, int T>
static inline void read_image_24_(volatile data_t *img_addr, hls::Mat<ROWS, COLS, T> &mat_image)
{
	data_t word32;
	size_t word32_idx = 0;

	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_0_in;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_1_in;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_2_in;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_3_in;

	int state = 0;

	for (int line = 0; line < ROWS; ++line) {
		for (int off = 0; off < COLS; ++off) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1

			switch (state) {
				case 0:
					word32 = img_addr[word32_idx++];
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						word32_print_("in", word32);
#endif

					pixel_0_in.val[0] = U32_B0(word32);		// R0
					pixel_0_in.val[1] = U32_B1(word32);		// G0
					pixel_0_in.val[2] = U32_B2(word32);		// B0

					mat_image << pixel_0_in;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("in", pixel_0_in);
#endif
					pixel_1_in.val[0] = U32_B3(word32);		// R1

					state = 1;
					break;

				case 1:
					word32 = img_addr[word32_idx++];
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						word32_print_("in", word32);
#endif

					pixel_1_in.val[1] = U32_B0(word32);		// G1
					pixel_1_in.val[2] = U32_B1(word32);		// B1

					mat_image << pixel_1_in;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("in", pixel_1_in);
#endif
					pixel_2_in.val[0] = U32_B2(word32);		// R2
					pixel_2_in.val[1] = U32_B3(word32);		// G2

					state = 2;
					break;

				case 2:
					word32 = img_addr[word32_idx++];
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						word32_print_("in", word32);
#endif

					pixel_2_in.val[2] = U32_B0(word32);		// B2

					mat_image << pixel_2_in;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("in", pixel_2_in);
#endif

					pixel_3_in.val[0] = U32_B1(word32);		// R3
					pixel_3_in.val[1] = U32_B2(word32);		// G3
					pixel_3_in.val[2] = U32_B3(word32);		// B3

					state = 3;
					break;

				case 3:
					mat_image << pixel_3_in;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("in", pixel_3_in);
#endif
					state = 0;
					break;

				default:
					break;
			}
		}
	}
}

template<int ROWS, int COLS, int T>
static inline void write_image_24_(volatile data_t *img_addr, hls::Mat<ROWS, COLS, T> &mat_image)
{
	data_t word32;
	size_t word32_idx = 0;

	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_0_out;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_1_out;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_2_out;
	hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pixel_3_out;

	int state = 0;

	for (int line = 0; line < ROWS; ++line) {
		for (int off = 0; off < COLS; ++off) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1

			switch (state) {
				case 0:
					mat_image >> pixel_0_out;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("out", pixel_0_out);
#endif

					state = 1;
					break;

				case 1:
					mat_image >> pixel_1_out;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("out", pixel_1_out);
#endif

					// [0] -> R, [1] -> G, [2] -> B
					word32 = U32_PACK(	pixel_1_out.val[0],
										pixel_0_out.val[2],
										pixel_0_out.val[1],
										pixel_0_out.val[0]);
					img_addr[word32_idx++] = word32;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						word32_print_("out", word32);
#endif

					state = 2;
					break;

				case 2:
					mat_image >> pixel_2_out;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("out", pixel_2_out);
#endif

					word32 = U32_PACK(	pixel_2_out.val[1],
										pixel_2_out.val[0],
										pixel_1_out.val[2],
										pixel_1_out.val[1]);
					img_addr[word32_idx++] = word32;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						word32_print_("out", word32);
#endif

					state = 3;
					break;

				case 3:
					mat_image >> pixel_3_out;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						pix_print_("out", pixel_3_out);
#endif

					word32 = U32_PACK(	pixel_3_out.val[2],
										pixel_3_out.val[1],
										pixel_3_out.val[0],
										pixel_2_out.val[2]);
					img_addr[word32_idx++] = word32;
#ifndef __SYNTHESIS__
					if (line == 0 && off < 10)
						word32_print_("out", word32);
#endif

					state = 0;
					break;

				default:
					break;
			}
		}
	}
}

#endif /* IMG_STREAM */
