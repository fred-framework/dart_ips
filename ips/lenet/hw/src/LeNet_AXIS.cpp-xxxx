/*
 * LeNet.cpp
 *
 *  Created on: 2020. 2. 27.
 *      Author: floyed
 */

/********************************************************************/
/*					correct rate: 0.975938                          */
/********************************************************************/
#include "LeNet.h"
#include "iostream"
#include "cstring"
#include "ap_fixed.h"
using namespace std;

//create layer

void LeNet_AXIS(ap_axis<HW_DATA_WIDTH,1,1,1>src[BUFFER_SIZE], ap_axis<HW_DATA_WIDTH,1,1,1>dst[CLASSES], int id){

	ap_axis<HW_DATA_WIDTH, 1, 1,1> data;
	hw_fixed input[image_Batch][INPUT_WH][INPUT_WH];
	hw_fixed output[CLASSES];
	int index=0;

	Copy:{
		data.dest = src[0].dest;
		data.id = src[0].id;
		data.keep = src[0].keep;
		data.last = src[0].last;
		data.strb = src[0].strb;
		data.user = src[0].user;
	}

	load_batch:
	for(int batch=0; batch<image_Batch; batch++){
		load_row:
		for(int i=0; i<INPUT_WH; i++){
			Load_col:
			for(int j=0; j<INPUT_WH; j++){
				input[batch][i][j] = ((float)src[index].data)/DATA_CONVERT_MUL;
				index ++;
			}
		}
	}

	LeNet(input,output,0);

	index = 0;
	Output:
	for(int batch=0; batch<image_Batch-1; batch++){
		for(int i=0; i<MNIST_LABEL_SIZE; i++){
	//#pragma HLS pipeline
			dst[index].data = ((float)output[index])*DATA_CONVERT_MUL;//((int*)output)[index];
			dst[index].keep = data.keep;
			dst[index].strb = data.strb;
			dst[index].user = data.user;
			dst[index].last = data.last;
			dst[index].id = data.id;
			dst[index].dest = data.dest;
			index ++;
		}
	}
	// the last batch must have last woth with the 'last' signal enabled
	for(int i=0; i<MNIST_LABEL_SIZE-1; i++){
//#pragma HLS pipeline
		dst[index].data = ((float)output[index])*DATA_CONVERT_MUL;//((int*)output)[index];
		dst[index].keep = data.keep;
		dst[index].strb = data.strb;
		dst[index].user = data.user;
		dst[index].last = data.last;
		dst[index].id = data.id;
		dst[index].dest = data.dest;
		index ++;
	}

	dst[index].data = ((float)output[index])*DATA_CONVERT_MUL;//((int*)output)[i];
	dst[index].keep = data.keep;
	dst[index].strb = data.strb;
	dst[index].user = data.user;
	dst[index].last = 1;
	dst[index].id = data.id;
	dst[index].dest = data.dest;
	/*
	Output_fill:
	for(int i=CLASSES; i<BUFFER_SIZE; i++){
		dst[i].data = data[i].data;
		dst[i].keep = data[i].keep;
		dst[i].strb = data[i].strb;
		dst[i].user = data[i].user;
		dst[i].last = data[i].last;
		dst[i].id = data[i].id;
		dst[i].dest = data[i].dest;
	}*/

}
