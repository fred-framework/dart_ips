#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fred_lib.h"

// make sure these constants match with the hw design
#define  BLOCK_SIZE_BYTE (10 * 1024)
#define  BLOCK_SIZE_DT (BLOCK_SIZE_BYTE / sizeof(uint64_t))

uint64_t mem_in[BLOCK_SIZE_DT];
uint64_t mem_out[BLOCK_SIZE_DT];

void init_vect(unsigned int base_idx, int value)
{
	for (unsigned int i = 0; i < BLOCK_SIZE_DT; ++i) {
		mem_in[base_idx + i] = value + i;
	}
}

void print_vect(unsigned int base_idx, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		//std::cout << mem_in[base_idx + i] << "\t" << mem_out[base_idx + i] << "\n";
		printf("%d\t%d\n");
	}
	printf("\n");
	//std::cout << std::endl;
}

int main (int argc, char **argv)
{
	//std::cout << " starting memcpy \n";
	printf(" starting memcpy \n");

	init_vect(0, 0);
	
	struct fred_data *fred;
	struct fred_hw_task *hw_memcpy;
    
	//sw_task_init(&fred);

	fred_init(&fred);
	
	// Bind with HW-memcpy having hw-id 100
	fred_bind(fred, &hw_memcpy, 100);

	uint64_t *hw_memcpy_buff_in0;
	// this buffer is not actually used
	uint64_t *hw_memcpy_buff_out0;

	hw_memcpy_buff_in0 = (uint64_t*) fred_map_buff(fred, hw_memcpy, 0);
	hw_memcpy_buff_out0 = (uint64_t*) fred_map_buff(fred, hw_memcpy, 1);

	// set the memcpy parameters (dest, source)
	*(hw_memcpy_buff_in0 + 0) = (uint64_t) mem_out;
	*(hw_memcpy_buff_in0 + 1) = (uint64_t) mem_in;

	// Call fred IP
	fred_accel(fred, hw_memcpy);

	//validate
	int error_code = memcmp(mem_in,mem_out, BLOCK_SIZE_BYTE);
	if (error_code !=0){
		//std::cout << "Mismatch!\n";
		printf("Mismatch!\n");
		print_vect(0, 10);
	}else{
		//std::cout << "Match!\n";
		printf("Match!\n");
	}

	//cleanup and finish
	fred_free(fred);

	//std::cout << "Fred finished\n";
	printf("Fred finished\n");

	return(error_code);
}
