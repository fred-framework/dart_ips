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
#include <stdlib.h>
#include "cstring"
#include "ap_fixed.h"

//layers
#include "activation.h"
#include "convolution.h"
#include "fullyconnected.h"
#include "pooling.h"

using namespace std;

//conv layer weight & bias
const hw_fixed Wconv1[CONV_1_TYPE][CONV_1_WH][CONV_1_WH] = {
#include "Wconv1.h"
};
const hw_fixed Bconv1[CONV_1_TYPE] = {
#include "bconv1.h"
};
const hw_fixed Wconv2[CONV_2_TYPE][CONV_1_TYPE][CONV_2_WH][CONV_2_WH] = {
#include "Wconv3.h"
};
const hw_fixed Bconv2[CONV_2_TYPE] = {
#include "bconv3.h"
};
const hw_fixed Wconv3[CONV_3_TYPE][CONV_2_TYPE][CONV_3_WH][CONV_3_WH] = {
#include "Wconv5.h"
};
const hw_fixed Bconv3[CONV_3_TYPE] = {
#include "bconv5.h"
};

//pool layer weight & bias
const hw_fixed Wpool1[POOL_1_TYPE*4] = {
#include "Wpool1.h"
};
const hw_fixed Bpool1[POOL_1_TYPE] = {
#include "bpool1.h"
};
const hw_fixed Wpool2[POOL_2_TYPE*4] = {
#include "Wpool2.h"
};
const hw_fixed Bpool2[POOL_2_TYPE] = {
#include "bpool2.h"
};

//fc layer weight & bias
const hw_fixed Wfc1[FILTER_NN_1_SIZE] = {
#include "Wfc1.h"
};
const hw_fixed Bfc1[BIAS_NN_1_SIZE] = {
#include "bfc1.h"
};
const hw_fixed Wfc2[FILTER_NN_2_SIZE] = {
#include "Wfc2.h"
};
const hw_fixed Bfc2[BIAS_NN_2_SIZE] = {
#include "bfc2.h"
};

//create layer
//hw_fixed input[image_Batch][INPUT_WH][INPUT_WH];
hw_fixed conv1[image_Batch][CONV_1_TYPE][CONV_1_OUTPUT_WH][CONV_1_OUTPUT_WH];
hw_fixed pool1[image_Batch][CONV_1_TYPE][CONV_2_INPUT_WH][CONV_2_INPUT_WH];
hw_fixed conv2[image_Batch][CONV_2_TYPE][CONV_2_OUTPUT_WH][CONV_2_OUTPUT_WH];
hw_fixed pool2[image_Batch][CONV_2_TYPE][CONV_3_INPUT_WH][CONV_3_INPUT_WH];
hw_fixed conv3[image_Batch][CONV_3_TYPE];
hw_fixed fc1[image_Batch][OUTPUT_NN_1_SIZE];
//hw_fixed output[image_Batch*OUTPUT_NN_2_SIZE];

// generates the reference data for later comparison
#if defined(REF_DATA) || defined(FRED_REF_DATA)
	void save_data(FILE *fp, hw_fixed *input, const uint32_t size){
		for(int k=0; k<size; k++){
			fprintf(fp,"%s : %s\n", input[k].to_string(10).c_str(),input[k].to_string(2).c_str()); 
			//fprintf(fp,"%2X\n", input[k].to_int());//
		}
	}
#endif
#ifdef COMP_DATA
	void comp_data(FILE *fp, hw_fixed *input, const uint32_t size){
		hw_fixed temp;
		char hexa_data[20];
		//long int aux_int;
		float aux_float;
		//printf("antes do seg fault\n");
		for(int k=0; k<size; k++){
			//fscanf(fp,"%02X",&aux_int);
			if (fgets(hexa_data,20,fp) == NULL){
				printf("ERROR: reference file is empty\n");
				exit(1);
			}
			// returns a long int
			//aux_int = strtol(hexa_data, NULL, 10);
			aux_float = strtof(hexa_data, NULL);
			
			//temp = (int8_t)aux_int;
			//printf("%s\n", temp.to_string(16).c_str());
			if (aux_float != input[k].to_float()){
				//printf("mismatch found in position %d. expect %2X but got %2X\n", k, temp, input[k]);
				printf("mismatch found in position %04d. expect %f but got %f - d %s - b %s - h %s\n", k, aux_float, input[k].to_float(), input[k].to_string(10).c_str(), input[k].to_string(2).c_str(), input[k].to_string(16).c_str());
				return;
			}
		}
	}
#endif


void LeNet(hw_fixed input1[image_Batch][INPUT_WH][INPUT_WH], hw_fixed output1[image_Batch*OUTPUT_NN_2_SIZE], int id){

#ifdef REF_DATA
	FILE *fp;
	fp = fopen("input_ref.dat", "w");
#endif
#ifdef FRED_REF_DATA
	FILE *fp;
	fp = fopen("fred_input_ref.dat", "w");
#endif
#if defined(REF_DATA) || defined(FRED_REF_DATA)
	if (fp == NULL)	{
	    printf("Error opening file!\n");
		exit(1);
	}
	printf("Writing the reference data\n");
	save_data(fp,(hw_fixed *)input1,image_Batch*INPUT_WH*INPUT_WH);
#endif
#ifdef COMP_DATA
	FILE *fp;
	fp = fopen("input_ref.dat", "r");
	if (fp == NULL)	{
	    printf("Error opening file!\n");
		exit(1);
	}
	printf("Comparing the simulated data with the reference data\n");
	comp_data(fp,(hw_fixed *)input1,image_Batch*INPUT_WH*INPUT_WH);
#endif

	//cout<<"loaded image"<<endl;
	//calc 
	Convolution_Layer_1(input1, Wconv1, Bconv1, conv1);
#if defined(REF_DATA) || defined(FRED_REF_DATA)
	save_data(fp,(hw_fixed *)conv1, image_Batch*CONV_1_TYPE*CONV_1_OUTPUT_WH*CONV_1_OUTPUT_WH);
#endif
#ifdef COMP_DATA
	comp_data(fp,(hw_fixed *)conv1, image_Batch*CONV_1_TYPE*CONV_1_OUTPUT_WH*CONV_1_OUTPUT_WH);
#endif
	//cout<<"conv1"<<endl;
	Pooling_Layer_1(conv1, Wpool1, Bpool1, pool1);
	//cout<<"pool1"<<endl;
	Convolution_Layer_2(pool1, Wconv2, Bconv2, conv2);
	//cout<<"conv2"<<endl;
	Pooling_Layer_2(conv2, Wpool2, Bpool2, pool2);
	//cout<<"pool2"<<endl;
	Convolution_Layer_3(pool2, Wconv3, Bconv3, conv3);
	//cout<<"conv3"<<endl;
	Fully_Connected_Layer_1(conv3, Wfc1, Bfc1, fc1);
	//cout<<"fc1"<<endl;
#if defined(REF_DATA) || defined(FRED_REF_DATA)
	save_data(fp,(hw_fixed *)fc1, image_Batch*OUTPUT_NN_1_SIZE);
#endif
	Fully_Connected_Layer_2(fc1, Wfc2, Bfc2, output1);
	//cout<<"fc2"<<endl;
#if defined(REF_DATA) || defined(FRED_REF_DATA)
	save_data(fp,(hw_fixed *)output1, image_Batch*OUTPUT_NN_2_SIZE);
	fclose(fp);
#endif
#if defined(REF_OUT_DATA)
	FILE *fp;
	fp = fopen("output_ref.dat", "w");
	for(int k=0; k<image_Batch*OUTPUT_NN_2_SIZE; k++){
		//fprintf(fp,"%s\n", output1[k].to_string(2).c_str()); 
		fprintf(fp,"%04d. f %f - d %s - b %s - h %s\n", k, output1[k].to_float(), output1[k].to_string(10).c_str(), output1[k].to_string(2).c_str(), output1[k].to_string(16).c_str());
		//fprintf(fp,"%2X\n", input[k].to_int());//
	}
	fclose(fp);
#endif
}
