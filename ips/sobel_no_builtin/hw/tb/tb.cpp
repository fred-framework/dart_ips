
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
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

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
	// uint8_t window_o[3][3];
	uint32_t idx;

	for (int j = 0; j<IMG_HEIGHT-2; j++)
	{
		// loading the initial values of the rolling windows
		window_i[0][0] = img_in[j*IMG_WIDTH+0]; window_i[0][1] = img_in[(j+1)*IMG_WIDTH+0] ; window_i[0][2] = img_in[(j+2)*IMG_WIDTH+0];
		window_i[1][0] = img_in[j*IMG_WIDTH+1]; window_i[1][1] = img_in[(j+1)*IMG_WIDTH+1] ; window_i[1][2] = img_in[(j+2)*IMG_WIDTH+1];
		window_i[2][0] = img_in[j*IMG_WIDTH+2]; window_i[2][1] = img_in[(j+1)*IMG_WIDTH+2] ; window_i[2][2] = img_in[(j+2)*IMG_WIDTH+2];

		// window_o[0][0] = img_out[j*IMG_WIDTH+0]; window_o[0][1] = img_out[(j+1)*IMG_WIDTH+0] ; window_o[0][2] = img_out[(j+2)*IMG_WIDTH+0];
		// window_o[1][0] = img_out[j*IMG_WIDTH+1]; window_o[1][1] = img_out[(j+1)*IMG_WIDTH+1] ; window_o[1][2] = img_out[(j+2)*IMG_WIDTH+1];
		// window_o[2][0] = img_out[j*IMG_WIDTH+2]; window_o[2][1] = img_out[(j+1)*IMG_WIDTH+2] ; window_o[2][2] = img_out[(j+2)*IMG_WIDTH+2];

		idx = (j+2)*IMG_WIDTH;
		for (int i = 0; i<IMG_WIDTH-2; i++)
		{
			int pixval_x =
				(sobel_x[0][0] * window_i[0][0]) + (sobel_x[0][1] * window_i[0][1]) + (sobel_x[0][2] * window_i[0][2]) +
				(sobel_x[1][0] * window_i[1][0]) + (sobel_x[1][1] * window_i[1][1]) + (sobel_x[1][2] * window_i[1][2]) +
				(sobel_x[2][0] * window_i[2][0]) + (sobel_x[2][1] * window_i[2][1]) + (sobel_x[2][2] * window_i[2][2]);

			int pixval_y =
				(sobel_y[0][0] * window_i[0][0]) + (sobel_y[0][1] * window_i[0][1]) + (sobel_y[0][2] * window_i[0][2]) +
				(sobel_y[1][0] * window_i[1][0]) + (sobel_y[1][1] * window_i[1][1]) + (sobel_y[1][2] * window_i[1][2]) +
				(sobel_y[2][0] * window_i[2][0]) + (sobel_y[2][1] * window_i[2][1]) + (sobel_y[2][2] * window_i[2][2]);

			// uint32_t pixval_x =
			// 	(sobel_x[0][0] * img_in[j*IMG_WIDTH+0]) + (sobel_x[0][1] * img_in[(j+1)*IMG_WIDTH+0]) + (sobel_x[0][2] * img_in[(j+2)*IMG_WIDTH+0]) +
			// 	(sobel_x[1][0] * img_in[j*IMG_WIDTH+1]) + (sobel_x[1][1] * img_in[(j+1)*IMG_WIDTH+1]) + (sobel_x[1][2] * img_in[(j+2)*IMG_WIDTH+1]) +
			// 	(sobel_x[2][0] * img_in[j*IMG_WIDTH+2]) + (sobel_x[2][1] * img_in[(j+1)*IMG_WIDTH+2]) + (sobel_x[2][2] * img_in[(j+2)*IMG_WIDTH+2]);

			// uint32_t pixval_y =
			// 	(sobel_y[0][0] * img_in[j*IMG_WIDTH+0]) + (sobel_y[0][1] * img_in[(j+1)*IMG_WIDTH+0]) + (sobel_y[0][2] * img_in[(j+2)*IMG_WIDTH+0]) +
			// 	(sobel_y[1][0] * img_in[j*IMG_WIDTH+1]) + (sobel_y[1][1] * img_in[(j+1)*IMG_WIDTH+1]) + (sobel_y[1][2] * img_in[(j+2)*IMG_WIDTH+1]) +
			// 	(sobel_y[2][0] * img_in[j*IMG_WIDTH+2]) + (sobel_y[2][1] * img_in[(j+1)*IMG_WIDTH+2]) + (sobel_y[2][2] * img_in[(j+2)*IMG_WIDTH+2]);



			//int sum = ABS(pixval_x) + ABS(pixval_y);
			int sum = ABS(pixval_x + pixval_y);
			if (sum > 255)
			{
				sum = 255; //for best performance
			}
			img_out[j*IMG_WIDTH+i] = (uint8_t)sum;

			// rolling the windows maximizing internal memory reuse and minimizing the input bandwidth
			idx += i;
			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = img_in[idx];
			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = img_in[idx+1];
			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2]; window_i[2][2] = img_in[idx+2];

			// window_o[0][0] = window_o[0][1]; window_o[0][1] = window_o[0][2]; window_o[0][2] = img_out[idx];
			// window_o[1][0] = window_o[1][1]; window_o[1][1] = window_o[1][2]; window_o[1][2] = img_out[idx+1];
			// window_o[2][0] = window_o[2][1]; window_o[2][1] = window_o[2][2]; window_o[2][2] = img_out[idx+2];
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
		uint32_t gray_pix_32;
		data_in2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE rewind
			pix = img_in[itr];
			// avoid floating point match, but produces an image a bit darker
			gray_pix_32 = ((uint8_t)U32_B0(pix)>>2) + ((uint8_t)U32_B1(pix)>>1) + ((uint8_t)U32_B2(pix)>>2);
			// clamp at most to 0xFF
			gray_pix = MIN(gray_pix_32,255);
			//if (pix!=0)
			//{
			//	printf("0x%08X (0x%02X): 0x%02X 0x%02X 0x%02X\n",pix, gray_pix, U32_B0(pix)>>2, U32_B1(pix)>>1, U32_B2(pix)>>2);
			//}
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

	//printf("INPUT IMG (uint8_t)\n");
	//print_img(img_buf_ptr, IMG_WIDTH,5,1);

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
			// very unoptimised encoding
			pixel32 = U32_PACK(0,img_buf_ptr[aux+2],img_buf_ptr[aux+1],img_buf_ptr[aux]);
			mem_in[idx] = (data_t)pixel32;
			//if (i==0)
			// 	printf ("%016lX - %08X - (%02X,%02X,%02X)\n", mem_in[idx], pixel32,img_buf_ptr[aux],img_buf_ptr[aux+1],img_buf_ptr[aux+2]);
			aux +=3;
			idx++;
		}
		
	}
	//printf("INPUT IMG (data_t)\n");
	//print_img((void*)mem_in,IMG_WIDTH,5,0);

	// run the sw reference
	sobel_64b(mem_in, expected_out);
	//printf("REF END\n");

	// run FRED
	sobel_top(&id_out, args, mem_in, mem_out);
	//printf("FRED END\n");

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
	std::cout << "Expected value: ";
	print_img((uint8_t*)expected_out_8UC1, IMG_WIDTH,5,1);
	std::cout << "Output value  : ";
	print_img((uint8_t*)mem_out_8UC1, IMG_WIDTH,5,1);

#ifdef WITH_SAVE_OUTPUT
	// the image is save in the dir ips/sobel/hw/sobel/solution_0/csim/build
	// run the following command to convert the file to a image format
	FILE *fout = fopen("output.gray","wb");
	FILE *fref_out = fopen("ref_output.gray","wb");
	if(fout == NULL || fref_out == NULL)
	{
		printf("ERROR: gray image cannot be saved!\n");
		exit(1);             
	}
	uint32_t wbytes = fwrite(mem_out_8UC1,1,IMG_WIDTH*IMG_HEIGHT,fout);
	if (wbytes != IMG_WIDTH*IMG_HEIGHT)
	{
		printf("ERROR: gray image size is %d but the expected is %d!\n", wbytes, IMG_WIDTH*IMG_HEIGHT);
		exit(1);
	}
	wbytes = fwrite(expected_out_8UC1,1,IMG_WIDTH*IMG_HEIGHT,fref_out);
	if (wbytes != IMG_WIDTH*IMG_HEIGHT)
	{
		printf("ERROR: gray image size is %d but the expected is %d!\n", bytes, IMG_WIDTH*IMG_HEIGHT);
		exit(1);
	}
	fclose(fout);
	fclose(fref_out);
	// convert -size 512x512 -depth 8 ./sobel/lat-in/csim/build/ref_output.gray img_out.png
	//system("/usr/bin/convert -size 512x512 -depth 8 ./ref_output.gray ./ref_output.png");
	//system("/usr/bin/convert -size 512x512 -depth 8 ./output.gray ./output.png");
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
