/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xscugic.h"
#include "xparameters.h"
#include "xacc_hw.h"
#include "xacc.h"
#include "xil_cache.h"

#include <stdio.h>
//#include "fred_lib.h"
XAcc fir_inst;
XScuGic ScuGic; //Interrupt Controller Instance

unsigned int ResultAvailfir = 0;

int setup_interrupt();
int fir_init();
void fir_start(void *InstancePtr);
void fir_isr(void *InstancePtr);

int main()
{
	int status;

    init_platform();
    Xil_DCacheDisable();

    printf(" \n starting fir \n");

    const unsigned BUFF_SIZE = 10;
    const unsigned FIR_WINDOW_SIZE = 25;

    int coeff[] = {13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6};
    u64 in_buff [BUFF_SIZE],
				out_buff[BUFF_SIZE],
				out_buff_gold[BUFF_SIZE];

    //setup acc
    status = fir_init(&fir_inst);
    if(status != XST_SUCCESS){
       print("HLS peripheral setup failed\n\r");
       exit(-1);
    }

    status = setup_interrupt();
    if(status != XST_SUCCESS){
       print("Interrupt setup failed\n\r");
       exit(-1);
    }

    //init the fir input buffer
    for(unsigned i = 0; i < BUFF_SIZE; i++) {
        in_buff[i] = (u64 ) i;
    }

    //calculate gold
    for(unsigned n = 0; n < BUFF_SIZE; n++) {
        out_buff_gold[n] = 0;
        for (unsigned i = 0; i < FIR_WINDOW_SIZE; i++){
            int temp = (int) n - i;
            if(temp >= 0)
                out_buff_gold[n] += coeff[i] * in_buff[n - i];
            }
        }

    //send input and output buffer to fir_acc
    Xil_Out32(XPAR_XACC_0_S_AXI_CTRL_BUS_BASEADDR +
    		XACC_CTRL_BUS_ADDR_MEM_IN_V_DATA,
    		 (UINTPTR)in_buff);
    Xil_Out32(XPAR_XACC_0_S_AXI_CTRL_BUS_BASEADDR +
    		XACC_CTRL_BUS_ADDR_MEM_OUT_V_DATA,
			(UINTPTR)out_buff);

    printf("%p %p \n", in_buff, out_buff);

    if (XAcc_IsReady(&fir_inst))
       print("HLS peripheral is ready.  Starting... ");
    else {
       print("!!! HLS peripheral is not ready! Exiting...\n\r");
       exit(-1);
    }

    if (1) { // use interrupt
       fir_start(&fir_inst);
       while(!ResultAvailfir); //spin
       print("Interrupt received from HLS HW.\n\r");
    } else { // Simple non-interrupt driven test
       XAcc_Start(&fir_inst);
       do {
       } while (!XAcc_IsReady(&fir_inst));
       print("Detected FIR peripheral complete. Result received.\n\r");
    }

    //validate
    int errors = 0;
    for (unsigned j = 0; j < BUFF_SIZE; j++) {
        if (out_buff_gold[j] != out_buff[j])
            errors++;
    }

    if (errors)
        printf("Test FAILED with %i errors \n", errors);
    else
        printf("Test PASSED.");

    cleanup_platform();

    return 0;
}


int fir_init(XAcc *firPtr)
{
   XAcc_Config *cfgPtr;
   int status;

   cfgPtr = XAcc_LookupConfig(XPAR_XACC_0_DEVICE_ID);
   if (!cfgPtr) {
      print("ERROR: Lookup of acclerator configuration failed.\n\r");
      return XST_FAILURE;
   }
   status = XAcc_CfgInitialize(firPtr, cfgPtr);
   if (status != XST_SUCCESS) {
      print("ERROR: Could not initialize accelerator.\n\r");
      return XST_FAILURE;
   }
   return status;
}

void fir_start(void *InstancePtr){
	XAcc *pAccelerator = (XAcc *)InstancePtr;
	XAcc_InterruptEnable(pAccelerator,1);
	XAcc_InterruptGlobalEnable(pAccelerator);
	XAcc_Start(pAccelerator);
}


int setup_interrupt()
{
	   //This functions sets up the interrupt on the ARM
	   int result;
	   XScuGic_Config *pCfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	   if (pCfg == NULL){
	      print("Interrupt Configuration Lookup Failed\n\r");
	      return XST_FAILURE;
	   }
	   result = XScuGic_CfgInitialize(&ScuGic,pCfg,pCfg->CpuBaseAddress);
	   if(result != XST_SUCCESS){
	      return result;
	   }
	   // self test
	   result = XScuGic_SelfTest(&ScuGic);
	   if(result != XST_SUCCESS){
	      return result;
	   }
	   // Initialize the exception handler
	   Xil_ExceptionInit();
	   // Register the exception handler
	   //print("Register the exception handler\n\r");
	   Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,&ScuGic);
	   //Enable the exception handler
	   Xil_ExceptionEnable();
	   // Connect the Adder ISR to the exception table
	   //print("Connect the Adder ISR to the Exception handler table\n\r");
	   result = XScuGic_Connect(&ScuGic,XPAR_FABRIC_ACC_0_INTERRUPT_INTR,(Xil_InterruptHandler)fir_isr,&fir_inst);
	   if(result != XST_SUCCESS){
	      return result;
	   }
	   //print("Enable the Adder ISR\n\r");
	   XScuGic_Enable(&ScuGic,XPAR_FABRIC_ACC_0_INTERRUPT_INTR);
	   return XST_SUCCESS;
}

void fir_isr(void *InstancePtr)
{
	   XAcc *pAccelerator = (XAcc *)InstancePtr;

	   //Disable the global interrupt
	   XAcc_InterruptGlobalDisable(pAccelerator);
	   //Disable the local interrupt
	   XAcc_InterruptDisable(pAccelerator,0xffffffff);

	   // clear the local interrupt
	   XAcc_InterruptClear(pAccelerator,1);

	   ResultAvailfir = 1;
	   // restart the core if it should run again
}

