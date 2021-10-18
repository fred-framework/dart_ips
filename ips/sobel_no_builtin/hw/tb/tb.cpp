
#include <iostream>
#include <cstring>

#ifdef WITH_OPENCV
#include <opencv2/opencv.hpp>
using namespace cv;
#endif

#define WITH_SAVE_OUTPUT

//using namespace std;

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

static const unsigned int IMG_WIDTH = 256;
static const unsigned int IMG_HEIGHT = 256;

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
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


// assign a lower height to print the image partially
void print_img(void *base_idx, unsigned int width, unsigned int height, uint8_t gray)
{
	int i,j;
	std::cout << "[ \n";
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			if (gray==1){
				printf("0x%02X ",(uint8_t)((uint8_t*)base_idx)[j + width * i]);
			}
			else{
				printf("0x%016lX ",(data_t)((data_t*)base_idx)[j + width * i]);
			}
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
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


int main()
{
	unsigned int idx,aux,i,j;
	int error_code=0;

	args_t id_out;
	args_t args[ARGS_SIZE];
	// point to the input RGB stream
	uint8_t *img_buf_ptr;
	// input for both the reference and the accelerated functions
	data_t mem_in[IMG_WIDTH*IMG_HEIGHT];
	// output format (8UC1 in uint64_t) from the reference and the accelerator
	data_t mem_out[IMG_WIDTH*IMG_HEIGHT];
	data_t expected_out[IMG_WIDTH*IMG_HEIGHT];
	// output format converted to 8UC1 in uint8_t
	uint8_t mem_out_8UC1[IMG_WIDTH*IMG_HEIGHT];
	uint8_t expected_out_8UC1[IMG_WIDTH*IMG_HEIGHT];
	data_t pixel64;
	uint32_t pixel32;
	//printf("SOBEL ....\n");

#ifdef WITH_OPENCV
	// the image is save in the dir ips/sobel/hw/sobel/solution_0/csim/build
	//Mat image = imread("../../../../../img/checkerboard-512.bmp",IMREAD_RGB);
	Mat image = imread("../../../../../img/box.bmp",IMREAD_RGB);

	// Error Handling
	if (image.empty()) {
		printf("ERROR: BMP Image File Not Found!\n");
		// wait for any key press
		cin.get();
		return -1;
	}
	// gets the pointer to the RGB stream
	img_buf_ptr = image.ptr();
#else
	// the image is save in the dir ips/sobel/hw/sobel/solution_0/csim/build
	//FILE *fptr = fopen("../../../../../img/checkerboard-512.rgb","r");
	FILE *fptr = fopen("../../../../../img/box.rgb","r");
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
	// gets the pointer to the RGB stream
	img_buf_ptr = (uint8_t*)img_buffer;
#endif

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)0;
	args[1] = (args_t)0;

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

	// run the sw reference
	sobel_64b(mem_in, expected_out);

	// run FRED
	sobel_top(&id_out, args, mem_in, mem_out);

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
		std::cout << "Mismatch!\n";
		error_code = 1;
	}else{
		std::cout << "Match!\n";
	}
	// print only the 5 initial lines of the images
	//std::cout << "Expected value: ";
	//print_img((uint8_t*)expected_out_8UC1, IMG_WIDTH,5,1);
	//std::cout << "Output value  : ";
	//print_img((uint8_t*)mem_out_8UC1, IMG_WIDTH,5,1);

#ifdef WITH_SAVE_OUTPUT
	save_img("output.gray",mem_out_8UC1);
	save_img("reference.gray",expected_out_8UC1);
#endif


#ifdef WITH_OPENCV
	// transform the 8UC1 bytestreams into openCV type
	Mat img(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, mem_out_8UC1);
	Mat img_ref(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, expected_out_8UC1);

	// Show images
	imwrite("ref_output.bmp", img_ref); 
	imwrite("output.bmp", img); 

	// Show images
	imshow("FRED Output", img);
	imshow("Reference Output", img_ref);

	// Wait for any keystroke
	waitKey(0);
#endif

	return error_code ;
}
