#include"HWA-MM.h"
//#include<math.h>


// ---------------- ADDRESS MAP REGISTER --------------------------
// ---slv_reg0-------|--------|---------|---------|-|-|-|-|-|-|gs|r|
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
//---------------------------------------------------------------------

#define write_reg(base_address, reg_offset, data) \
  	Xil_Out32((base_address) + (reg_offset), (u32)(data))

#define read_reg(base_address, reg_offset) \
    Xil_In32((base_address) + (reg_offset))


uint32_t HWA_startExec(uintptr_t baseaddr)
{
  write_reg(baseaddr,0x0,read_reg(baseaddr,0x0) | (1 << 1));
  return 1;
}

uint32_t HWA_Init(HWA_t* InstancePtr, uintptr_t baseaddr)
{
	InstancePtr->baseaddr = baseaddr;
	return 1;
}


uint32_t HWA_setDefaultConfiguration(uintptr_t baseaddr, uint32_t num_r_trans, uint32_t num_w_trans, uint32_t exec_time)
{
  HWA_reset(baseaddr);

  HWA_setReadBaseAddr(baseaddr,R_DEFAULT_ADDRESS);
  HWA_setWriteBaseAddr(baseaddr,W_DEFAULT_ADDRESS);
  HWA_setReadDisplacement(baseaddr,R_DEFAULT_DISPL);
  HWA_setWriteDisplacement(baseaddr,W_DEFAULT_DISPL);
  HWA_setReadMaxOutstanding(baseaddr,R_DEFAULT_MAX_OUTS);
  HWA_setWriteMaxOutstanding(baseaddr,W_DEFAULT_MAX_OUTS);
  HWA_setReadPhase(baseaddr,R_DEFAULT_PHASE);
  HWA_setWritePhase(baseaddr,W_DEFAULT_PHASE);
  HWA_setNumReadTrans(baseaddr,num_r_trans);
  HWA_setNumWriteTrans(baseaddr,num_w_trans);
  HWA_setReadBurstLength(baseaddr,R_DEFAULT_BURST_LEN);
  HWA_setWriteBurstLength(baseaddr,W_DEFAULT_BURST_LEN);
  HWA_setExecutionTime_clk(baseaddr,exec_time);

  return 1;
}

uint32_t HWA_reset(uintptr_t baseaddr)
{
  write_reg(baseaddr,0x0,read_reg(baseaddr,0x0) | (1 << 0));
  return 1;
}

uint32_t HWA_setReadBaseAddr(uintptr_t baseaddr, uint32_t address)
{
  write_reg(baseaddr,0x4,address);
  return 1;
}

uint32_t HWA_setWriteBaseAddr(uintptr_t baseaddr, uint32_t address)
{
  write_reg(baseaddr,0x8,address);
  return 1;
}

uint32_t HWA_setReadDisplacement(uintptr_t baseaddr, uint8_t displacement);
{
  write_reg(baseaddr,0xc,(read_reg(baseaddr,0xc) & ~LSB_FOURTH_BYTE_MASK) | (displacement << 24));
  return 1;
}

uint32_t HWA_setWriteDisplacement(uintptr_t baseaddr, uint8_t displacement);
{
  write_reg(baseaddr,0xc,(read_reg(baseaddr,0xc) & ~LSB_THIRD_BYTE_MASK) | (displacement << 16));
  return 1;
}

uint32_t HWA_setReadMaxOutstanding(uintptr_t baseaddr, uint8_t outs);
{
  write_reg(baseaddr,0xc,(read_reg(baseaddr,0xc) & ~LSB_SECOND_BYTE_MASK) | (outs << 8));
  return 1;
}

uint32_t HWA_setWriteMaxOutstanding(uintptr_t baseaddr, uint8_t outs);
{
  write_reg(baseaddr,0xc,(read_reg(baseaddr,0xc) & ~LSB_FIRST_BYTE_MASK) | (outs << 0));
  return 1;
}

uint32_t HWA_setReadPhase(uintptr_t baseaddr, uint16_t phase);
{
  write_reg(baseaddr,0x10,(read_reg(baseaddr,0x10) & ~LSB_SECOND_HALFWORD_MASK) | (phase << 16));
  return 1;
}

uint32_t HWA_setWritePhase(uintptr_t baseaddr, uint16_t phase);
{
  write_reg(baseaddr,0x10,(read_reg(baseaddr,0x10) & ~LSB_FIRST_HALFWORD_MASK) | (phase << 0));
  return 1;
}

uint32_t HWA_setNumReadTrans(uintptr_t baseaddr, uint16_t num_trans);
{
  write_reg(baseaddr,0x14,(read_reg(baseaddr,0x14) & ~LSB_SECOND_HALFWORD_MASK) | (num_trans << 16));
  return 1;
}

uint32_t HWA_setNumWriteTrans(uintptr_t baseaddr, uint16_t num_trans);
{
  write_reg(baseaddr,0x14,(read_reg(baseaddr,0x14) & ~LSB_FIRST_HALFWORD_MASK) | (num_trans << 0));
  return 1;
}

uint32_t HWA_setReadBurstLength(uintptr_t baseaddr, uint8_t length);
{
  write_reg(baseaddr,0x18,(read_reg(baseaddr,0x18) & ~LSB_FOURTH_BYTE_MASK) | (length << 24));
  return 1;
}

uint32_t HWA_setWriteBurstLength(uintptr_t baseaddr, uint8_t length);
{
  write_reg(baseaddr,0x18,(read_reg(baseaddr,0x18) & ~LSB_THIRD_BYTE_MASK) | (length << 16));
  return 1;
}

uint32_t HWA_setReadMisbeh_clk(uintptr_t baseaddr, uint16_t cycles);
{
  write_reg(baseaddr,0x1c,(read_reg(baseaddr,0x1c) & ~LSB_SECOND_HALFWORD_MASK) | (cycles << 16));
  return 1;
}

uint32_t HWA_setWriteMisbeh_clk(uintptr_t baseaddr, uint16_t cycles);
{
  write_reg(baseaddr,0x1c,(read_reg(baseaddr,0x1c) & ~LSB_FIRST_HALFWORD_MASK) | (cycles << 0));
  return 1;
}

uint32_t HWA_setExecutionTime_clk(uintptr_t baseaddr, uint32_t exec_time);
{
  write_reg(baseaddr,0x20,exec_time);
  return 1;
}
l
