
#include <iostream>
#include <cstring>
// source: https://github.com/sol-prog/cpp-bmp-images
#include "BMP.h"

// Vitis Vision Library
// https://xilinx.github.io/Vitis_Libraries/vision/2020.1/index.html
// https://xilinx.github.io/Vitis_Libraries/vision/2020.1/api-reference.html#sobel-filter
// https://github.com/Xilinx/Vitis_Libraries/tree/master/vision/L1/examples/sobelfilter

// opencv example https://github.com/STFleming/HLS_OpenCV/tree/master
// https://github.com/ATaylorCEngFIET/Hackster/tree/master/Sobel <===
//   https://www.hackster.io/adam-taylor/fpga-based-edge-detection-using-hls-192ad2
//   https://github.com/ATaylorCEngFIET/Hackster/blob/master/Sobel/solution1/script.tcl

typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;

static const unsigned int IMG_WIDTH = 512;
static const unsigned int IMG_HEIGHT = 512;

void sobel_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

// the pixel is encoded like this
#define U32_PACK(B3, B2, B1, B0) (((B3) << 24) | ((B2) << 16) | ((B1) << 8) | (B0))
// U32_PACK(0,R,G,B)
// (data_t)U32_PACK(0, pixel_out.val[0], pixel_out.val[1], pixel_out.val[2]);

// pixel decoding
#define U32_B0(pixel) ((pixel) & 0xff)         // B
#define U32_B1(pixel) (((pixel) >> 8) & 0xff)  // G
#define U32_B2(pixel) (((pixel) >> 16) & 0xff) // R
#define U32_B3(pixel) (((pixel) >> 24) & 0xff) // alpha

#define ABS(x) ((x)>0?(x):-(x))

data_t expected_out[IMG_WIDTH*IMG_HEIGHT] = {
	//U32_PACK(0,1,2,3), ... , U32_PACK(0,255,200,100), 
	0
};

// reference function
void sobel(uint8_t *img_in, uint8_t *img_out){
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

	for (int j = 0; j<IMG_HEIGHT-2; j++)
	{
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

void sobel_64b(data_t *img_in, data_t *img_out){
	uint8_t src_img[IMG_HEIGHT*IMG_WIDTH];
	uint8_t dest_img[IMG_HEIGHT*IMG_WIDTH];

	uint32_t itr = 0;
	data_t pix;
	for (int i = 0; i<IMG_HEIGHT; i++)
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

	sobel(src_img,dest_img);

	itr=0;
	for (int i = 0; i<IMG_HEIGHT; i++)
	{
		for (int j = 0; j<IMG_WIDTH; j++)
		{
			pix = (data_t)dest_img[itr];
			img_out[itr] = pix;
			itr++;
		}
	}		
}

// assign a lower height to print the image partially
void print_img(data_t *base_idx, unsigned int width, unsigned int height, bool rgb=true)
{
	int i,j;
	data_t pixel;
	std::cout << "[ \n";
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			pixel = base_idx[j + height * i];
			if (rgb){
				std::cout 	<< (unsigned int)U32_B3(pixel) << ","
							<< (unsigned int)U32_B2(pixel) << ","
							<< (unsigned int)U32_B1(pixel) << ","
							<< (unsigned int)U32_B0(pixel) << ", ";
			}else{
				std::cout << base_idx[j + height * i] <<  " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}


int main()
{
	unsigned int aux = 0,i,j;
	int error_code=0;

	args_t id_out;
	args_t args[ARGS_SIZE];
	data_t mem_in[IMG_WIDTH*IMG_HEIGHT];
	data_t mem_out[IMG_WIDTH*IMG_HEIGHT];
	data_t pixel64;
	uint32_t pixel32;
	//printf("SOBEL ....\n");

	// using has_alpha = true to be compatible with the hw encoding
	BMP bmp(IMG_WIDTH, IMG_HEIGHT, true);
	BMP bmp_out(IMG_WIDTH, IMG_HEIGHT, true);
	// fill w a red square
	bmp.fill_region(0, 0, 120, 120, 0, 0, 255, 0);
	// make sure the output image is blank
	bmp_out.fill_region(0, 0, IMG_WIDTH, IMG_HEIGHT, 0, 0, 0, 0);
	// the image is save in the dir ips/sobel/hw/sobel/solution_0/csim/build
	bmp.write("input.bmp");	

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)0;
	args[1] = (args_t)0;

	// copying the bmp data into the hw buffer with data_t format
	aux = 0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			// 32bits alpha, r, g, b is transformed into data_t/64bits alpha, r, g, b
			// very unoptimised encoding
			pixel32 = U32_PACK(bmp.data[aux+3],bmp.data[aux+2],bmp.data[aux+1],bmp.data[aux]);
			mem_in[j + IMG_WIDTH * i] = (data_t)pixel32;
			// if (i==0)
			// 	printf ("%lld - %d - (%d,%d,%d,%d)\n", mem_in[j + IMG_HEIGHT * i], pixel32,bmp.data[aux],bmp.data[aux+1],bmp.data[aux+2],bmp.data[aux+3]);
			aux +=4;
		}
		
	}
	//printf("INPUT IMG\n");
	// checking the copy above
	/*
	aux=0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			pixel64 = mem_in[j + IMG_HEIGHT * i];
			if (i==0)
				printf ("%lld - (%d,%d,%d,%d) - (%d,%d,%d,%d)\n",pixel64, bmp.data[aux],bmp.data[aux+1],bmp.data[aux+2],bmp.data[aux+3],
					(uint8_t)U32_B0(pixel64), (uint8_t)U32_B1(pixel64), (uint8_t)U32_B2(pixel64), (uint8_t)U32_B3(pixel64));
			if ((uint8_t)U32_B3(pixel64) != bmp.data[aux+3]) // alpha
				error_code = 1;
			if ((uint8_t)U32_B2(pixel64) != bmp.data[aux+2]) // R
				error_code = 1;
			if ((uint8_t)U32_B1(pixel64) != bmp.data[aux+1]) // G
				error_code = 1;
			if ((uint8_t)U32_B0(pixel64) != bmp.data[aux])   // B
				error_code = 1;
			if (error_code == 1){
				printf("error copying the input image\n");
				break;
			}
			aux +=4;
		}
		if (error_code == 1){
			break;
		}
	}
	*/

	// run FRED
	sobel_top(&id_out, args, mem_in, mem_out);
	//printf("FRED END\n");

	// run the sw reference
	sobel_64b(mem_in, expected_out);
	//printf("REF END\n");

	// copying the hw buffer with data_t format into the bmp data format
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			// convert data_t/64bits alpha, r, g, b into 8bits alpha, r, g, b
			pixel64 = mem_out[j + IMG_HEIGHT * i];
			bmp_out.set_pixel(i,j,
				(uint8_t)U32_B0(pixel64),(uint8_t)U32_B1(pixel64),
				(uint8_t)U32_B2(pixel64),(uint8_t)U32_B3(pixel64));
		}
		
	}

	// checking the previous copy
	/*
	aux=0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			pixel64 = mem_out[j + IMG_HEIGHT * i];
			if (i==0)
				printf ("%lld - (%d,%d,%d,%d) - (%d,%d,%d,%d)\n",pixel64, bmp_out.data[aux],bmp_out.data[aux+1],bmp_out.data[aux+2],bmp_out.data[aux+3],
					(uint8_t)U32_B0(pixel64), (uint8_t)U32_B1(pixel64), (uint8_t)U32_B2(pixel64), (uint8_t)U32_B3(pixel64));
			if ((uint8_t)U32_B3(pixel64) != bmp_out.data[aux+3]) // alpha
				error_code = 1;
			if ((uint8_t)U32_B2(pixel64) != bmp_out.data[aux+2]) // R
				error_code = 1;
			if ((uint8_t)U32_B1(pixel64) != bmp_out.data[aux+1]) // G
				error_code = 1;
			if ((uint8_t)U32_B0(pixel64) != bmp_out.data[aux])   // B
				error_code = 1;
			if (error_code == 1){
				printf("error copying the input image\n");
				break;
			}
			aux +=4;
		}
		if (error_code == 1){
			break;
		}
	}
	*/
	bmp_out.write("output.bmp");

	// checking if the output image is empty
	aux = 0;
	bool not_empty_img = false;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			pixel32 = U32_PACK(bmp_out.data[aux+3],bmp_out.data[aux+2],bmp_out.data[aux+1],bmp_out.data[aux]);
			if (pixel32 != 0){
				not_empty_img = true;
				break;
			}
			aux +=4;
		}
		if (not_empty_img){
			break;
		}
	}

	if (not_empty_img){
		// if the img is not empty, then compare the hw output w the sw reference
		if (memcmp(expected_out, mem_out,IMG_WIDTH*IMG_HEIGHT)){
			std::cout << "Mismatch!\n";
			error_code = 1;
		}else{
			std::cout << "Match!\n";
		}
		// print only the 10 initial lines of the images
		std::cout << "Expected value: ";
		print_img((data_t*)expected_out, IMG_WIDTH,5,true);
		std::cout << "Output value  : ";
		print_img((data_t*)mem_out, IMG_WIDTH,5,true);
	}else{
		std::cout << "Mismatch - Empty output image!\n";
		error_code = 1;
	}
	
	return error_code ;
}
