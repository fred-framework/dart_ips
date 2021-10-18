#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "fred_lib.h"

typedef uint64_t data_t;

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
void print_img(uint8_t *base_idx, unsigned int width, unsigned int height, unsigned rgb)
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


void save_img(const char *filename, const uint8_t *image){

	// the image is save in the dir ips/sobel/hw/sobel/solution_0/csim/build
	// run the following command to convert the file to a image format
	FILE *fout = fopen(filename,"wb");
	if(fout == NULL)
	{
		printf("ERROR: gray image %s cannot be saved!\n",filename);
		exit(1);
	}
	uint32_t wbytes = fwrite(image,1,IMG_WIDTH*IMG_HEIGHT,fout);
	if (wbytes != IMG_WIDTH*IMG_HEIGHT)
	{
		printf("ERROR: gray image size is %d but the expected is %d!\n", wbytes, IMG_WIDTH*IMG_HEIGHT);
		exit(1);
	}
	fclose(fout);
	// convert -size 512x512 -depth 8 ./sobel/lat-in/csim/build/ref_output.gray img_out.png
	//system("/usr/bin/convert -size 512x512 -depth 8 ./output.gray ./output.png");
	char command[100];
	sprintf(command, "/usr/bin/convert -size 512x512 -depth 8 ./%s ./%s.png", filename, filename);
	system(command);
}

// reference function
void sobel(uint8_t img_in[IMG_HEIGHT][IMG_WIDTH], uint8_t img_out[IMG_HEIGHT][IMG_WIDTH]){
	uint8_t window_i[3][3];
	uint32_t row,col,curMax=0;
	int sum1,sum2;

	const int height = IMG_HEIGHT;
	const int width = IMG_WIDTH;

    // set the edges to black
	for(row=1; row < height-1; row++)
	{
        img_out[row][0] = 0;
        img_out[row][width-1] = 0;
    }	
	for(col=0; col < width; col++)
	{
        img_out[0][col] = 0;
        img_out[height-1][col] = 0;
    }

	// apply the filter using a rolling window
	for(row=0; row < height-2; row++)
	{
		for(col=0; col < width; col++)
		{	
			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = img_in[row+0][col];
			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = img_in[row+1][col];
			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2]; window_i[2][2] = img_in[row+2][col];
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
				img_out[row+1][col-1] = sum1 + sum2;
				if(sum1 + sum2 > curMax)
				{
					curMax = sum1 + sum2;
				}
			}
		}
	}
}

void sobel_64b(data_t *img_in, data_t *img_out){
	uint8_t src_img[IMG_HEIGHT][IMG_WIDTH];
	uint8_t dest_img[IMG_HEIGHT][IMG_WIDTH];

	uint32_t itr = 0;
	data_t pix;
	int red,green,blue;
	uint8_t *img_ptr = (uint8_t *)img_in;
	for (int i = 0; i<IMG_HEIGHT; i++)
	{
		uint8_t gray_pix;
		data_in2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE rewind
            red = (uint8_t)*(img_ptr++);
            green = (uint8_t)*(img_ptr++);
            blue =(uint8_t)*(img_ptr++);
			// avoid floating point math, but produces an image a bit darker
            // original conversion
			//in_gray_buffer[row][col] = (0.3*red) + (0.59*green) + (0.11*blue);
			src_img[i][j] = (uint8_t)(red>>2) + (uint8_t)(green>>1) + (uint8_t)(blue>>2);

			// alternative solution
			// pix = img_in[itr];
			// gray_pix = ((uint8_t)(U32_B0(pix)>>2)) + ((uint8_t)(U32_B1(pix)>>1)) + ((uint8_t)(U32_B2(pix)>>2));
			// src_img[itr] = gray_pix;

			// jump by the 5 unused bytes of data_t
			img_ptr +=5;
			//itr++;
		}
	}
	//save_img("gray.gray",src_img);

	sobel(src_img,dest_img);

	itr=0;
	for (int i = 0; i<IMG_HEIGHT; i++)
	{
		for (int j = 0; j<IMG_WIDTH; j++)
		{
			pix = (data_t)dest_img[i][j];
			img_out[itr] = pix;
			itr++;
		}
	}		
}

int main (int argc, char **argv)
{
	printf(" starting Sobel filter for images %dx%d\n",IMG_WIDTH,IMG_HEIGHT);
	int retval;
	int error_code = 0,idx,aux,i,j;
	// point to the input RGB stream
	uint8_t *img_buf_ptr;
	// input for both the reference and the accelerated functions
	data_t *mem_in;
	// output format (8UC1 in uint64_t) from the reference and the accelerator
	data_t *mem_out;
	data_t expected_out[IMG_WIDTH*IMG_HEIGHT];
	// output format converted to 8UC1 in uint8_t
	uint8_t mem_out_8UC1[IMG_WIDTH*IMG_HEIGHT];
	uint8_t expected_out_8UC1[IMG_WIDTH*IMG_HEIGHT];
	data_t pixel64;
	uint32_t pixel32;
	struct timespec start, end;
	double time_taken;

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

	// load the raw rgb image
	FILE *fptr = fopen("./img/box.rgb","r");
	if(fptr == NULL)
	{
		printf("ERROR: RGB Image File Not Found!\n");
		exit(1);             
	}
   	uint8_t img_buffer [IMG_WIDTH*IMG_HEIGHT*3];
	uint32_t bytes = fread(img_buffer,1,IMG_WIDTH*IMG_HEIGHT*3,fptr);
	if (bytes != IMG_WIDTH*IMG_HEIGHT*3)
	{
		printf("ERROR: RGB Image size is %d but the expected is %d!\n", bytes, IMG_WIDTH*IMG_HEIGHT*3);
		exit(1);
	}
	fclose(fptr);
	img_buf_ptr = (uint8_t*)img_buffer;

	// copying the bmp data into the hw buffer with data_t format
	aux = 0;
	idx = 0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			// 24bits r, g, b is transformed into data_t/64bits 0, r, g, b
			// very unoptimized encoding
			pixel32 = U32_PACK(0,img_buf_ptr[aux+2],img_buf_ptr[aux+1],img_buf_ptr[aux]);
			mem_in[idx] = (data_t)pixel32;
			//if (i==0)
			// 	printf ("%016lX - %08X - (%02X,%02X,%02X)\n", mem_in[idx], pixel32,img_buf_ptr[aux],img_buf_ptr[aux+1],img_buf_ptr[aux+2]);
			aux +=3;
			idx++;
		}
		
	}

	clock_gettime(CLOCK_MONOTONIC, &start);
	sobel_64b(mem_in,expected_out);
	clock_gettime(CLOCK_MONOTONIC, &end);

	// Calculating total time taken by the sw implementation.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by the CPU is : %09f\n", time_taken);

	// Call fred IP
	clock_gettime(CLOCK_MONOTONIC, &start);
	retval = fred_accel(fred, hw_ip);
	if (retval) {
		printf("fred_accel failed for hw-task %u\n", hw_id);
		error_code = 1;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	// Calculating total time taken by the FPGA offloading.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by FRED is : %09f\n", time_taken);

	// copying the hw buffer with data_t format into the 8UC1 bmp data format
	idx = 0;
	for (i = 0; i < IMG_HEIGHT; i++){
		for (j = 0; j < IMG_WIDTH; j++)	{
			// convert 8UC1 64bits into 8UC1 8bits
			pixel64 = mem_out[idx];
			mem_out_8UC1[idx] = (uint8_t)U32_B0(pixel64);
			pixel64 = expected_out [idx];
			expected_out_8UC1[idx] = (uint8_t)U32_B0(pixel64);
			idx++;
		}
	}

	// if the img is not empty, then compare the hw output w the sw reference
	if (memcmp(expected_out_8UC1, mem_out_8UC1,IMG_WIDTH*IMG_HEIGHT)){
		printf("Mismatch!\n");
		error_code = 1;
	}else{
		printf("Match!\n");
	}

#ifdef DEBUG
	save_img("output.gray",mem_out_8UC1);
	save_img("reference.gray",expected_out_8UC1);
#endif

	// this loop is required just to avoid messing up with the printed messages 
	// caused by the messages printed by fred_free
	for(i=0;i<100000000;i++);

	fred_free(fred);
	printf("Fred finished\n");

	return(error_code);
}
