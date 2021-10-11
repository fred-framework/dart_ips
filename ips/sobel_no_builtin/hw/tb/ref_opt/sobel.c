// adapted from https://raw.githubusercontent.com/mjpatter88/SobelEdgeDetection/master/sobel.c
//Will support both ppm and pgm format files, in both 1 byte and 2 byte format.
//Converts every image to grayscale as it is read in.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U32_B0(pixel) ((pixel) & 0xff)         // B
#define U32_B1(pixel) (((pixel) >> 8) & 0xff)  // G
#define U32_B2(pixel) (((pixel) >> 16) & 0xff) // R
#define U32_B3(pixel) (((pixel) >> 24) & 0xff) // alpha

static const unsigned int IMG_WIDTH = 512;
static const unsigned int IMG_HEIGHT = 512;

//#define PERFORMANCE
// When this parameter is defined, the input memory is duplicated, removing internal depedencies in the main pipeline,
// leading the to the most optimized performance assuming the PREM approach

#ifdef PERFORMANCE
void sobelFilter(const unsigned char inImg1[IMG_HEIGHT][IMG_WIDTH],const unsigned char inImg2[IMG_HEIGHT][IMG_WIDTH], unsigned char outImg[IMG_HEIGHT][IMG_WIDTH]);
void to_gray(const unsigned char *mem_in,unsigned char src_img1[IMG_HEIGHT][IMG_WIDTH],unsigned char src_img2[IMG_HEIGHT][IMG_WIDTH]);
#else
void sobelFilter(const unsigned char inImg1[IMG_HEIGHT][IMG_WIDTH], unsigned char outImg[IMG_HEIGHT][IMG_WIDTH]);
void to_gray(const unsigned char *mem_in,unsigned char src_img1[IMG_HEIGHT][IMG_WIDTH]);
#endif

void save_img(const char *filename, const unsigned char image[IMG_HEIGHT][IMG_WIDTH]);

int main(int argc, char* argv[])
{
	FILE *fptr = fopen("./box.rgb","r");
	if(fptr == NULL)
	{
		printf("ERROR: RGB Image File Not Found!\n");
		exit(1);             
	}
   	unsigned char img_buffer [IMG_WIDTH*IMG_HEIGHT*3];
#ifdef PERFORMANCE
    unsigned char in_gray_buffer1 [IMG_WIDTH][IMG_HEIGHT];
	unsigned char in_gray_buffer2 [IMG_WIDTH][IMG_HEIGHT];
#else
	unsigned char in_gray_buffer1 [IMG_WIDTH][IMG_HEIGHT];
#endif
    unsigned char out_gray_buffer [IMG_WIDTH][IMG_HEIGHT];

    // read the raw rgb image, wo any image format
	unsigned int bytes = fread(img_buffer,1,IMG_WIDTH*IMG_HEIGHT*3,fptr);
	if (bytes != IMG_WIDTH*IMG_HEIGHT*3)
	{
		printf("ERROR: RGB Image size is %d but the expected is %d!\n", bytes, IMG_WIDTH*IMG_HEIGHT*3);
		exit(1);
	}
	fclose(fptr);
    
    // to gray
#ifdef PERFORMANCE
	to_gray(img_buffer,in_gray_buffer1,in_gray_buffer2);
#else
	to_gray(img_buffer,in_gray_buffer1);
#endif
	save_img("gray.gray",in_gray_buffer1);
	
	// sobel
#ifdef PERFORMANCE
	sobelFilter(in_gray_buffer1,in_gray_buffer2, out_gray_buffer);
#else
	sobelFilter(in_gray_buffer1, out_gray_buffer);
#endif
	save_img("output.gray",out_gray_buffer);
    
	//TODO: free memory
	return 0;
}


void save_img(const char *filename, const unsigned char image[IMG_HEIGHT][IMG_WIDTH]){

	// the image is save in the dir ips/sobel/hw/sobel/solution_0/csim/build
	// run the following command to convert the file to a image format
	FILE *fout = fopen(filename,"wb");
	if(fout == NULL)
	{
		printf("ERROR: gray image %s cannot be saved!\n",filename);
		exit(1);
	}
	int wbytes = fwrite(image,1,IMG_WIDTH*IMG_HEIGHT,fout);
	if (wbytes != IMG_WIDTH*IMG_HEIGHT)
	{
		printf("ERROR: gray image size is %d but the expected is %d!\n", wbytes, IMG_WIDTH*IMG_HEIGHT);
		exit(1);
	}
	fclose(fout);
	// convert -size 512x512 -depth 8 ./sobel/lat-in/csim/build/ref_output.gray img_out.png
	char command[100];
	sprintf(command, "/usr/bin/convert -size 512x512 -depth 8 ./%s ./%s.png", filename, filename);
	system(command);
}


#ifdef PERFORMANCE
void to_gray(const unsigned char *mem_in,unsigned char src_img1[IMG_HEIGHT][IMG_WIDTH],unsigned char src_img2[IMG_HEIGHT][IMG_WIDTH])
#else
void to_gray(const unsigned char *mem_in,unsigned char src_img1[IMG_HEIGHT][IMG_WIDTH])
#endif
{
	unsigned char red,green,blue,pix8;
	// access the input data byte by byte instead of dta_t
	//data_t pix;
	//uint32_t idx=0;

	unsigned char *img_ptr = (unsigned char *)mem_in;
	data_in1:for (int i = 0; i<IMG_HEIGHT; i++)
	{
		data_in2:for (int j = 0; j<IMG_WIDTH; j++)
		{
			#pragma HLS PIPELINE
			red = (unsigned char)*(img_ptr++);
			green = (unsigned char)*(img_ptr++);
			blue =(unsigned char)*(img_ptr++);
			//pix = mem_in[idx];
			//red = U32_B0(pix);
			//green = U32_B1(pix);
			//blue = U32_B2(pix);
			// avoid floating point math, but produces an image a bit darker
			// original conversion
			//in_gray_buffer[row][col] = (0.3*red) + (0.59*green) + (0.11*blue);
			pix8 = (unsigned char)(red>>2) + (unsigned char)(green>>1) + (unsigned char)(blue>>2);
			src_img1[i][j] = pix8;
			#ifdef PERFORMANCE
				src_img2[i][j] = pix8;
			#endif
			// jump by the 5 unused bytes of data_t
			//img_ptr +=5;
			//idx++;
		}
	}

}

//Only supports black/white images
#ifdef PERFORMANCE
void sobelFilter(const unsigned char inImg1[IMG_HEIGHT][IMG_WIDTH], const unsigned char inImg2[IMG_HEIGHT][IMG_WIDTH], unsigned char outImg[IMG_HEIGHT][IMG_WIDTH])
#else
void sobelFilter(const unsigned char inImg1[IMG_HEIGHT][IMG_WIDTH], unsigned char outImg[IMG_HEIGHT][IMG_WIDTH])
#endif
{
	int row;
	int col;
	int height;
	int width;
	int sum1;
	int sum2;
	int curMax = 0;
	
	height = IMG_HEIGHT;
	width = IMG_WIDTH;

    // set the edges to black
	for(row=1; row < height-1; row++)
	{
        outImg[row][0] = 0;
        outImg[row][width-1] = 0;
    }	
	for(col=0; col < width; col++)
	{
        outImg[0][col] = 0;
        outImg[height-1][col] = 0;
    }
	//Loop through the whole image pixel by pixel
	unsigned char window_i[3][3];
	for(row=0; row < height-2; row++)
	{
		for(col=0; col < width; col++)
		{	
			// loading the initial values of the rolling windows
			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = inImg1[row+0][col];
			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = inImg1[row+1][col];
			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2];
			#ifdef PERFORMANCE
			window_i[2][2] = inImg2[row+2][col];
			#else
			window_i[2][2] = inImg1[row+2][col];
			#endif

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

				//Non-optimized method
				/*
				sum1 = (-1 * inImg[row-1][col-1]) + 
				(1 * inImg[row-1][col+1]) +
				(-2 * inImg[row][col-1]) + 
				(2 * inImg[row][col+1]) + 
				(-1 * inImg[row+1][col-1]) + 
				(1 * inImg[row+1][col+1]);

				sum2 = (-1 * inImg[row-1][col-1]) + 
				(-2 * inImg[row-1][col]) +
				(-1 * inImg[row-1][col+1]) + 
				(1 * inImg[row+1][col-1]) + 
				(2 * inImg[row+1][col]) +
				(1 * inImg[row+1][col+1]);
				*/

				if(sum1 < 0)
				{
				sum1 = -sum1;
				}
				if(sum2 < 0)
				{
				sum2 = -sum2;
				}
				outImg[row+1][col-1] = sum1 + sum2;
				if(sum1 + sum2 > curMax)
				{
				curMax = sum1 + sum2;
				}	
			}

		}
	}
	
    printf("Max: %d.\n", curMax);
	//Scale the values in the output image
    /*
	for(row=0; row < height; row++)
	{
		for(col=0; col < width; col++)
		{
			//TODO: can this line be optimized? For sure just do (*255/curMax) instead of a division every time.
			outImg[row][col] = (outImg[row][col] * 255) / curMax;
		}
	}
    */
	return 0;
}

