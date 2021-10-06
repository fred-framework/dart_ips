#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "fred_lib.h"

#include "macros.h"
#include "sobel.h"
#include "file_operations.h"

typedef uint64_t data_t;

static const unsigned int IMG_WIDTH = 512;
static const unsigned int IMG_HEIGHT = 512;

// these buffers should be of size IMG_WIDTH*IMG_HEIGHT*sizeof(data_t) because data_t encode one pixel RGB 
// make sure that hw_tasks.csv has at least this buffer size. for 512 image it would be 2097152
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
void print_img(byte *base_idx, unsigned int width, unsigned int height, unsigned rgb)
{
	int i,j,aux=0;
	data_t pixel;
	printf("[\n");
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			pixel = base_idx[j + height * i];
			if (rgb){
				printf("%d,%d,%d ", base_idx[aux+2], base_idx[aux+1], base_idx[aux]);
			}else{
				printf("%d ",(uint32_t)U32_PACK(0,base_idx[aux+2], base_idx[aux+1], base_idx[aux]));
			}
		}
		printf("\n");
	}
	printf("]\n");
}

int main (int argc, char **argv)
{
	printf(" starting Sobel filter for images %dx%d\n",IMG_WIDTH,IMG_HEIGHT);
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
// these are not used, but kept to avoid changing the original code
         *sobel_h_res,
         *sobel_v_res,
         *contour_img;
	// has FRED ouput image
	byte img_out[IMG_WIDTH*IMG_HEIGHT*3];

	int width = IMG_WIDTH;
	int height = IMG_HEIGHT;
	int rgb_size = width*height*3;

	// File names
	strcpy(file_in,"src/reference/imgs/img.rgb");
	strcpy(file_out,"src/reference/imgs/img_out.gray");

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

	// copying the bmp data into the hw buffer with data_t format
	clock_gettime(CLOCK_MONOTONIC, &start);
	aux = 0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			// 32bits alpha, r, g, b is transformed into data_t/64bits alpha, r, g, b
			// very unoptimised encoding
			pixel32 = U32_PACK(0,rgb[aux+2],rgb[aux+1],rgb[aux]);
			mem_in[j + IMG_HEIGHT * i] = (data_t)pixel32;
			aux +=3;
		}
	}

	// Call fred IP
	retval = fred_accel(fred, hw_ip);
	if (retval) {
		printf("fred_accel failed for hw-task %u\n", hw_id);
		error_code = 1;
	}

	// copying the hw buffer with data_t format into the bmp data format
	aux = 0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			// convert data_t/64bits alpha, r, g, b into 8bits alpha, r, g, b
			pixel64 = mem_out[j + IMG_HEIGHT * i];
			img_out[i+2] = (uint8_t)U32_B2(pixel64);
			img_out[i+1] = (uint8_t)U32_B1(pixel64);
			img_out[i]   = (uint8_t)U32_B0(pixel64);
			aux +=3;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);		

	// Calculating total time taken by the FPGA offloading.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by FRED is : %09f\n", time_taken);


	// checking if the output image is empty
	aux = 0;
	unsigned not_empty_img = 0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			pixel32 = U32_PACK(0,img_out[aux+2],img_out[aux+1],img_out[aux]);
			if (pixel32 != 0){
				not_empty_img = 1;
				break;
			}
			aux +=3;
		}
		if (not_empty_img){
			break;
		}
	}

	if (not_empty_img){
		// if the img is not empty, then compare the hw output w the sw reference
		if (memcmp(contour_img, img_out,IMG_WIDTH*IMG_HEIGHT*3) != 0){
			printf("Mismatch!\n");
			error_code = 1;
		}else{
			printf("Match!\n");
		}
		// print only the 10 initial lines of the images
		printf("Expected value: \n");
		//print_img((byte*)contour_img, IMG_WIDTH,5,1);
		printf("Output value: \n");
		//print_img((byte*)img_out, IMG_WIDTH,5,1);
	}else{
		printf("Mismatch - Empty output image!\n");
		error_code = 1;
	}

	// this loop is required just to avoid messing up with the printed messages 
	// caused by the messages printed by fred_free
	for(i=0;i<100000000;i++);

	//cleanup and finish
	free(gray);
	free(sobel_h_res);
	free(sobel_v_res);
	free(contour_img);	
	fred_free(fred);
	printf("Fred finished\n");

	return(error_code);
}
