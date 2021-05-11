#include "HWA-MM.h"

/*
 A simple code reading 32 words starting from the default base address assuming that the IP core is mapped starting from 0xb0000000  and keeping the default burst length=16 words, executing 1000 clk cycles and writing 32 words to the default write address
*/

int main(void){
  HWA_setDefaultConfiguration(0xb0000000, 2, 2, 1000);
  HWA_startExec(0xb0000000);
}
