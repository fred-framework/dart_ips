#include <stdint.h>
#include "xil_io.h"

#ifndef Prog_HWA_H_
#define Prog_HWA_H_

#define LSB_FIRST_BYTE_MASK                             0x000000FF
#define LSB_SECOND_BYTE_MASK                            0x0000FF00
#define LSB_THIRD_BYTE_MASK                             0x00FF0000
#define LSB_FOURTH_BYTE_MASK                            0xFF000000

#define LSB_SECOND_HALFWORD_MASK                        0xFFFF0000
#define LSB_FIRST_HALFWORD_MASK                         0x0000FFFF

#define AXI_HYPERCONNECT_S_AXI_CTRL_SLV_CTRL_REG_OFFSET 	   	0
#define AXI_HYPERCONNECT_S_AXI_CTRL_SLV_ABU_PERIOD_OFFSET		  4


#define R_DEFAULT_ADDRESS                               0x0a000000
#define W_DEFAULT_ADDRESS                               0x0a800000
#define R_DEFAULT_DISPL                                 0x05
#define W_DEFAULT_DISPL                                 0x05
#define R_DEFAULT_MAX_OUTS                              0x08
#define W_DEFAULT_MAX_OUTS                              0x08
#define R_DEFAULT_PHASE                                 0x0000
#define W_DEFAULT_PHASE                                 0x0000
#define R_DEFAULT_BURST_LEN                             0xf
#define W_DEFAULT_BURST_LEN                             0xf


// ---------------- ADDRESS MAP REGISTER --------------------------
// ---slv_reg0-------|--------|---------|---------|-|-|-|-|sw|sr|gs|r|
// ---slv_reg1-------|----------r-base-addr-----------------|
// ---slv_reg2-------|----------w-base-addr-----------------|
// ---slv_reg3-------|r_displ-|w_displ--|r-max-outs|w-max-outs|
// ---slv-reg4-------|r-phase-----------|w-phase------------|
// ---slv_reg5-------|r-num-trans-------|w-num-trans--------|
// ---slv_reg6-------|r_b_len-|w_b_len--|-------------------|
// ---slv_reg7-------|r-misbehave-clk---|w-misbehave-clk----|
// ---slv-reg8-------|---------exec-time--------------------|

// --- r: reset
// --- gs: global start
// --- sr: start read
// --- sw: start write
//---------------------------------------------------------------------


typedef struct HWA {
  uintptr_t  baseaddr;
} HWA_t;

uint32_t HWA_Init(HWA_t* InstancePtr, uintptr_t baseaddr);

uint32_t HWA_startExec(uintptr_t baseaddr);

uint32_t HWA_setDefaultConfiguration(uintptr_t baseaddr, uint32_t num_r_trans, uint32_t num_w_trans, uint32_t exec_time);

uint32_t HWA_reset(uintptr_t baseaddr);

uint32_t HWA_setReadBaseAddr(uintptr_t baseaddr, uint32_t address);
uint32_t HWA_setWriteBaseAddr(uintptr_t baseaddr, uint32_t address);

uint32_t HWA_setReadDisplacement(uintptr_t baseaddr, uint8_t displacement);
uint32_t HWA_setWriteDisplacement(uintptr_t baseaddr, uint8_t displacement);

uint32_t HWA_setReadMaxOutstanding(uintptr_t baseaddr, uint8_t outs);
uint32_t HWA_setWriteMaxOutstanding(uintptr_t baseaddr, uint8_t outs);

uint32_t HWA_setReadPhase(uintptr_t baseaddr, uint16_t phase);
uint32_t HWA_setWritePhase(uintptr_t baseaddr, uint16_t phase);

uint32_t HWA_setNumReadTrans(uintptr_t baseaddr, uint16_t num_trans);
uint32_t HWA_setNumWriteTrans(uintptr_t baseaddr, uint16_t num_trans);

uint32_t HWA_setReadBurstLength(uintptr_t baseaddr, uint8_t length);
uint32_t HWA_setWriteBurstLength(uintptr_t baseaddr, uint8_t length);

uint32_t HWA_setReadMisbeh_clk(uintptr_t baseaddr, uint16_t cycles);
uint32_t HWA_setWriteMisbeh_clk(uintptr_t baseaddr, uint16_t cycles);

uint32_t HWA_setExecutionTime_clk(uintptr_t baseaddr, uint32_t exec_time);


#endif
