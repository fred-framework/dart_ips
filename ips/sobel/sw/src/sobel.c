#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "fred_lib.h"
// source: https://github.com/sol-prog/cpp-bmp-images
//#include "BMP.h"


#include "macros.h"
#include "sobel.h"
#include "file_operations.h"


typedef uint64_t data_t;

static const unsigned int IMG_WIDTH = 512;
static const unsigned int IMG_HEIGHT = 512;

data_t *mem_in, *mem_out;

const int hw_id = 100;

// the pixel is encoded like this
#define U32_PACK(B3, B2, B1, B0) (((B3) << 24) | ((B2) << 16) | ((B1) << 8) | (B0))
// U32_PACK(0,R,G,B)
// (data_t)U32_PACK(0, pixel_out.val[0], pixel_out.val[1], pixel_out.val[2]);

// pixel decoding
#define U32_B0(pixel) ((pixel) & 0xff)         // B
#define U32_B1(pixel) (((pixel) >> 8) & 0xff)  // G
#define U32_B2(pixel) (((pixel) >> 16) & 0xff) // R
#define U32_B3(pixel) (((pixel) >> 24) & 0xff) // alpha

//data_t expected_out[IMG_WIDTH*IMG_HEIGHT] ;
// = {
// 	//U32_PACK(0,1,2,3), ... , U32_PACK(0,255,200,100), 
// 	0
// };

// assign a lower height to print the image partially
void print_img(data_t *base_idx, unsigned int width, unsigned int height, unsigned rgb)
{
	int i,j;
	data_t pixel;
	//std::cout << "[ \n";
	printf("[\n");
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			pixel = base_idx[j + height * i];
			if (rgb){
				printf("%d,%d,%d,%d ",
					(unsigned int)U32_B3(pixel),
					(unsigned int)U32_B2(pixel),
					(unsigned int)U32_B1(pixel),
					(unsigned int)U32_B0(pixel));
				// std::cout 	<< (unsigned int)U32_B3(pixel) << ","
				// 			<< (unsigned int)U32_B2(pixel) << ","
				// 			<< (unsigned int)U32_B1(pixel) << ","
				// 			<< (unsigned int)U32_B0(pixel) << ", ";
			}else{
				//std::cout << base_idx[j + height * i] <<  " ";
				printf("%lld ",base_idx[j + height * i]);
			}
		}
		//std::cout << std::endl;
		printf("\n");
	}
	//std::cout << "]" << std::endl;
	printf("]\n");
}

int main (int argc, char **argv)
{
	printf(" starting Sobel filter for images[%d][%d]\n",IMG_WIDTH,IMG_HEIGHT);
	int retval;
	int error_code = 0,idx,aux,i,j;
	data_t pixel64;
	uint32_t pixel32;
	struct timespec start, end;
	double time_taken;

    char file_in[50],
         file_out[50];

    byte *rgb,
         *gray,
         *sobel_h_res,
         *sobel_v_res,
         *contour_img;

    int width = IMG_WIDTH;
    int height = IMG_HEIGHT;
	int rgb_size = width*height*3;

    // File names
    strcpy(file_in,"src/reference/imgs/img.rgb");
    strcpy(file_out,"src/reference/imgs/img_out.gray");

/*
	// fred related data structures and setup
	struct fred_data *fred;
	struct fred_hw_task *hw_ip;
    
	retval = fred_init(&fred);
	if (retval) {
		printf("fred_init failed for hw-task %u\n", hw_id);
		error_code = 1;
	}
	
	// Bind with HW-memcpy having hw-id 100
	retval = fred_bind(fred, &hw_ip, hw_id);
	if (retval) {
		printf("fred_bind failed for hw-task %u\n", hw_id);
		error_code = 1;
	}

	mem_in  = (data_t *)fred_map_buff(fred, hw_ip, 0);
	if (!mem_in) {
		printf("fred_map_buff failed on buff 0 for mem_in\n");
		error_code = 1;
	}
	mem_out = (data_t *)fred_map_buff(fred, hw_ip, 1);
	if (!mem_out) {
		printf("fred_map_buff failed on buff 1 for mem_out\n");
		error_code = 1;
	}
*/

    // Read file to rgb and get size
    readFile(file_in, &rgb, rgb_size);

	clock_gettime(CLOCK_MONOTONIC, &start);
    int gray_size = sobelFilter(rgb, &gray, &sobel_h_res, &sobel_v_res, &contour_img, width, height);
	clock_gettime(CLOCK_MONOTONIC, &end);

    // Write sobel img to a file
    writeFile(file_out, contour_img, gray_size);

	// Calculating total time taken by the sw implementation.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by the CPU is : %09f\n", time_taken);
/*
	// using has_alpha = true to be compatible with the hw encoding
	BMP bmp(IMG_WIDTH, IMG_HEIGHT, true);
	BMP bmp_out(IMG_WIDTH, IMG_HEIGHT, true);
	// fill w a red square
	bmp.fill_region(0, 0, 20, 20, 0, 0, 255, 0);
	// make sure the output image is blank
	bmp_out.fill_region(0, 0, IMG_WIDTH, IMG_HEIGHT, 0, 0, 0, 0);
	// the image is save in the dir ips/sobel/hw/sobel/solution_0/csim/build
	bmp.write("input.bmp");	

	// copying the bmp data into the hw buffer with data_t format
	clock_gettime(CLOCK_MONOTONIC, &start);
	aux = 0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			// 32bits alpha, r, g, b is transformed into data_t/64bits alpha, r, g, b
			// very unoptimised encoding
			pixel32 = U32_PACK(bmp.data[aux+3],bmp.data[aux+2],bmp.data[aux+1],bmp.data[aux]);
			mem_in[j + IMG_HEIGHT * i] = (data_t)pixel32;
			// if (i==0)
			// 	printf ("%lld - %d - (%d,%d,%d,%d)\n", mem_in[j + IMG_HEIGHT * i], pixel32,bmp.data[aux],bmp.data[aux+1],bmp.data[aux+2],bmp.data[aux+3]);
			aux +=4;
		}
		
	}

	// Call fred IP
	retval = fred_accel(fred, hw_ip);
	if (retval) {
		printf("fred_accel failed for hw-task %u\n", hw_id);
		error_code = 1;
	}


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
	clock_gettime(CLOCK_MONOTONIC, &end);		
	bmp_out.write("output.bmp");

	// Calculating total time taken by the FPGA offloading.
	double time_taken;
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by FRED is : %09f\n", time_taken);


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
		print_img((data_t*)mem_out, IMG_WIDTH,IMG_HEIGHT);
	}else{
		std::cout << "Mismatch - Empty output image!\n";
		error_code = 1;
	}
*/
	// this loop is required just to avoid messing up with the printed messages 
	// caused by the messages printed by fred_free
	for(i=0;i<100000000;i++);

	//cleanup and finish
	//fred_free(fred);
	printf("Fred finished\n");

	return(error_code);
}