#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fred_lib.h"

// make sure these constants match with the hw design
#define  BLOCK_SIZE_BYTE (10 * 1024)
#define  BLOCK_SIZE_DT (BLOCK_SIZE_BYTE / sizeof(uint32_t))

// uint32_t mem_in[BLOCK_SIZE_DT];
// uint32_t mem_out[BLOCK_SIZE_DT];

uint32_t *mem_in, *mem_out;

void init_vect(uint32_t *base, int value)
{
	for (unsigned int i = 0; i < BLOCK_SIZE_DT; ++i) {
		base = value + i;
		base++;
	}
}

void print_vect(unsigned int base_idx, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		//std::cout << mem_in[base_idx + i] << "\t" << mem_out[base_idx + i] << "\n";
		printf("%d\t%d\n",i,mem_out[i]);
	}
	printf("\n");
	//std::cout << std::endl;
}

int main (int argc, char **argv)
{
	//std::cout << " starting memcpy \n";
	printf(" starting memcpy \n");
	int retval, error_code;


	
	struct fred_data *fred;
	struct fred_hw_task *hw_memcpy;
    
	//sw_task_init(&fred);

	retval = fred_init(&fred);
	if (retval) {
		printf("fred_init failed \n");
		error_code = 1;
	}		
	
	// Bind with HW-memcpy having hw-id 100
	retval = fred_bind(fred, &hw_memcpy, 100);
	if (retval) {
		printf("fred_bind failed \n");
		error_code = 1;
	}		

	// uint64_t *hw_memcpy_buff_in0;
	// // this buffer is not actually used
	// uint64_t *hw_memcpy_buff_out0;

	mem_out = fred_map_buff(fred, hw_memcpy, 0);
	if (mem_out) {
		printf("fred_map_buff failed on buff 0 for mem_out\n");
		error_code = 1;
	}		
	mem_in = fred_map_buff(fred, hw_memcpy, 1);
	if (mem_in) {
		printf("fred_map_buff failed on buff 1 for mem_in\n");
		error_code = 1;
	}		

	// set the memcpy parameters (dest, source)
	//init_vect(mem_in, 0);
	for (unsigned int i = 0; i < BLOCK_SIZE_DT; ++i) {
		*(mem_in+i) = i;
		//base++;
	}
	// *(hw_memcpy_buff_in0 + 0) = (uint64_t) mem_out;
	// *(hw_memcpy_buff_in0 + 1) = (uint64_t) mem_in;

	// Call fred IP
	retval = fred_accel(fred, hw_memcpy);
	if (retval) {
		printf("fred_accel failed \n");
		error_code = 1;
	}		

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
