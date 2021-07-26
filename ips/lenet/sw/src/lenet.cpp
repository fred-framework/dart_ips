/*
 * LeNet_tb.cpp
 *
 *  Created on: 2021. 7. 26.
 *      Author: Alexandre Amory
 */
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <string>
#include <iostream>

#include "parameters.h"
//#include "MNIST_DATA.h"
#include "fred_lib.h"

typedef uint64_t data_t;

float MNIST_IMG[image_Move*MNIST_PAD_SIZE];
int MNIST_LABEL[image_Move];

#define TRUE 1
#define FALSE 0
using namespace std;

int ReverseInt(int i) {
	
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;

	return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;

}

// Read MNIST Data to padded array
void READ_MNIST_DATA(string filename, float* arr, float scale_min, float scale_max, int image_num) {
	ifstream file(filename.c_str(), ios::binary);
	cout << "Read MNIST DATA..."<< endl;
	if (file.is_open())
	{
		
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		file.read((char*)&n_rows, sizeof(n_rows));
		n_rows = ReverseInt(n_rows);
		file.read((char*)&n_cols, sizeof(n_cols));
		n_cols = ReverseInt(n_cols);
		int batch = number_of_images > image_num ? image_num : number_of_images;
		for (int i = 0; i<batch; ++i)
		{
			//for (int r = 0; r<n_rows; ++r)
			for (int r = 0; r<32; ++r)
			{
				//for (int c = 0; c<n_cols; ++c)
				for(int c=0;c<32;++c)
				{

					float _temp;
					if(c<2 || r<2 || c>=30|| r>=30)
						_temp = scale_min;
					else{
						unsigned char temp = 0;
						file.read((char*)&temp, sizeof(temp));
						_temp = ((float)temp / 255.0 )*(scale_max - scale_min) + scale_min;
					}

					arr[i*1024 + r*32 + c] = _temp;
				}
			}
		}
		cout << "MNIST DATA is loaded" << endl;

	}
	else {
		cout << "Failed to read MNIST DATA" << endl;
	}


}

void READ_MNIST_LABEL(string filename, int* label, int image_num, int one_hot)
{
	ifstream file(filename.c_str(), ios::binary);
	cout << "Read MNIST Label..." << endl;
	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;

		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		int batch = number_of_images > image_num ? image_num : number_of_images;
		for (int i = 0; i<batch; ++i)
		{
			unsigned char temp = 0;
			file.read((char*)&temp, sizeof(temp));
			if (one_hot) 
			{
				label[i * 10 + (int)temp] = 1;
			}
			else
			{
				label[i] = (int)temp;
			}
			
		}
		cout << "MNIST Label is loaded" << endl;
	}
	else
	{
		cout << "Failed to read MNIST Label" << endl;
	}
//	file.close();
}

int main(int argc, char* argv[]){
	printf("hello fred-lenet\r\n");

	struct fred_data *fred;
	struct fred_hw_task *hw_ip;
    data_t *mem_in, *mem_out;
	int retval;
	int error_code = 0;

   // the cwd is mnist_hls/Vivado_hls/LeNet_hls/origin/csim/build
	READ_MNIST_DATA("../../../../MNIST_DATA/t10k-images.idx3-ubyte",
			MNIST_IMG,-1.0f, 1.0f, image_Move);
	READ_MNIST_LABEL("../../../../MNIST_DATA/t10k-labels.idx1-ubyte",
			MNIST_LABEL,image_Move,false);

	retval = fred_init(&fred);
	if (retval) {
		printf("fred_init failed \n");
		error_code = 1;
	}
	// fred setup
    retval = fred_bind(fred, &hw_ip, 100);
    if (retval) {
        printf("fred_bind failed \n");
        error_code = 1;
    }

    mem_in = (data_t *)fred_map_buff(fred, hw_ip, 0);
    if (!mem_in) {
        printf("fred_map_buff failed on buff 0 for mem_in\n");
        error_code = 1;
    }
    mem_out = (data_t *)fred_map_buff(fred, hw_ip, 1);
    if (!mem_out) {
        printf("fred_map_buff failed on buff 1 for mem_out\n");
        error_code = 1;
    }


//	FILE *fin;
//	if ((fin = fopen("tb_datain.dat","w+")) == NULL)
//	{
//	    printf("Error opening file!\n");
//		exit(1);
//	}

	// uint8_t aux_uint8;
	// int8_t aux_int8;
	float aux_float;
	int idx=0;
    int8_t *mem_in_int8_ptr  = (int8_t *)mem_in;
	int test_num = image_Move / image_Batch;
	int correct = 0;
	//for(int i=0; i<test_num; i++){
	for(int i=0; i<10; i++){

        // set input values
		for(int batch=0; batch<image_Batch; batch++){
			for(int j=0; j<INPUT_WH; j++){
				for(int k=0; k<INPUT_WH; k++){
					aux_float = MNIST_IMG[(i+batch)*MNIST_PAD_SIZE + j*INPUT_WH + k]*DATA_CONVERT_MUL;
					mem_in_int8_ptr[batch*MNIST_PAD_SIZE + j*INPUT_WH + k] = (char)aux_float;
				}
			}
		}

		// printf("FRED INPUT: \n");
		// for(int batch=0; batch<image_Batch; batch++){
		// 	printf("batch: %d\n", batch);
		// 	for(int j=0; j<INPUT_WH; j++){
		// 		for(int k=0; k<INPUT_WH; k++){
		// 			fprintf(fin, "%02X %02X\n",  mem_in_int8_ptr[batch*MNIST_PAD_SIZE + j*INPUT_WH + k], mem_in[batch*MNIST_PAD_SIZE + j*INPUT_WH + k]);
		// 		}
		// 		fprintf(fin, "\n");
		// 	}
		// }
		// fprintf(fin,"\n\n\n");

        // fred run -- fpga offloading
        retval = fred_accel(fred, hw_ip);
        if (retval) {
            printf("fred_accel failed \n");
            error_code = 1;
        }		

		float max_num = -10000;
		int max_id = 0;
		char tmp;
		float result[MNIST_LABEL_SIZE];
		for(int batch=0; batch<image_Batch; batch++){
			max_id = 0;
			max_num = -10000;
			printf("Output for iteration %d, batch %d:\n", i, batch);
			for(int index=0; index<MNIST_LABEL_SIZE; index++){
				tmp = mem_out[batch*MNIST_LABEL_SIZE + index];
				result[index] = (float)tmp/DATA_CONVERT_MUL;
				if(result[index] > max_num){
					max_num = result[index];
					max_id = index;
				}
				printf("%0.3f ", result[index]);
			}
			cout << endl;
			// for(int index=0; index<MNIST_LABEL_SIZE; index++){
			// 	tmp = dst[batch*MNIST_LABEL_SIZE + index].data;
			// 	printf("%d: %x ", batch*MNIST_LABEL_SIZE + index, tmp & 0xFF);
			// }
			// cout << endl;
			if(MNIST_LABEL[i*image_Batch+batch] == max_id){
                correct ++;
            }else{
                // the 1st 10 inferences must match, otherwise you have a bug to fix.
                error_code = 1;
            }
				
			cout << "Expected idx: " << i*image_Batch+batch << endl;
			cout << "Expected: " << std::dec << MNIST_LABEL[i*image_Batch+batch] << endl;
			cout << "Obtained: " << std::dec << max_id << ", (" << max_num << ")"<< endl;
			cout << "Rate: " << (float)correct/(i*image_Batch+batch+1) << endl << endl;
		}
    }

	//cleanup and finish
	fred_free(fred);
	printf("Fred finished\n");

	return(error_code);
}
