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

int sobelFilter(unsigned char inImg[IMG_HEIGHT][IMG_WIDTH], unsigned char outImg[IMG_HEIGHT][IMG_WIDTH]);
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
    unsigned char in_gray_buffer [IMG_WIDTH][IMG_HEIGHT];
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
    unsigned int idx=0,row,col;
    int red,green,blue;
    unsigned char *img_ptr = img_buffer;
	for(row=0; row < IMG_HEIGHT; row++)
	{
		for(col=0; col < IMG_WIDTH; col++)
		{
            //convert to grayscale as values are read in.
            //red = fgetc(inpFile);
            //green = fgetc(inpFile);
            //blue = fgetc(inpFile);
            //gray_buffer[row][col] = toGrayscale(red, green, blue);
            // 1st read the value and then increment the pointer
            //red = (int)*(img_ptr++);
            //green = (int)*(img_ptr++);
            //blue =(int)*(img_ptr++);
            red = (unsigned char)*(img_ptr++);
            green = (unsigned char)*(img_ptr++);
            blue =(unsigned char)*(img_ptr++);
            // original convertion
			//in_gray_buffer[row][col] = (0.3*red) + (0.59*green) + (0.11*blue);
			// result in darker image
			in_gray_buffer[row][col] = (unsigned char)(red>>2) + (unsigned char)(green>>1) + (unsigned char)(blue>>2);
			// same as the previous one
			//in_gray_buffer[row][col] = (int)(red>>2) + (int)(green>>1) + (int)(blue>>2);
		}
	}
	save_img("gray.gray",in_gray_buffer);
	
	sobelFilter(in_gray_buffer, out_gray_buffer);
	
	// save the output
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

//Only supports black/white images
int sobelFilter(unsigned char inImg[IMG_HEIGHT][IMG_WIDTH], unsigned char outImg[IMG_HEIGHT][IMG_WIDTH])
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
			window_i[0][0] = window_i[0][1]; window_i[0][1] = window_i[0][2]; window_i[0][2] = inImg[row+0][col];
			window_i[1][0] = window_i[1][1]; window_i[1][1] = window_i[1][2]; window_i[1][2] = inImg[row+1][col];
			window_i[2][0] = window_i[2][1]; window_i[2][1] = window_i[2][2]; window_i[2][2] = inImg[row+2][col];
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
				outImg[row*1][col-1] = sum1 + sum2;
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

