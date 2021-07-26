/*
 * LeNet.h
 *
 *  Created on: 2020. 2. 27.
 *      Author: floyed
 */
#ifndef __LENET_H__
#define __LENET_H__

//system
#include "ap_int.h"
#include "ap_axi_sdata.h"

//parameters
#include "parameters.h"

void LeNet(hw_fixed input1[image_Batch][INPUT_WH][INPUT_WH], hw_fixed output1[image_Batch*OUTPUT_NN_2_SIZE], int id);

#endif
