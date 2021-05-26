#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fred_lib.h"

// make sure these constants match with the hw design
#define  ARRAY_SIZE (1024)
#define  BLOCK_SIZE_BYTE (ARRAY_SIZE * sizeof(uint32_t))

uint32_t A_in[ARRAY_SIZE];
uint32_t B_in[ARRAY_SIZE];
uint32_t C_out[ARRAY_SIZE];

void init_vect(uint32_t * base, int value)
{
	for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
		base[i] = value;
	}
}

void print_vect(uint32_t *base, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		//std::cout << mem_in[base_idx + i] << "\t" << mem_out[base_idx + i] << "\n";
		printf("%d\t%d\n", i, base[i]);
	}
	printf("\n");
	//std::cout << std::endl;
}

uint32_t check_output(uint32_t *base, unsigned int size)
{
	uint32_t sum=0;
	for (unsigned int i = 0; i < size; ++i) {
		sum += base[i];
	}
	return sum;
}

int main (int argc, char **argv)
{
	//std::cout << " starting memcpy \n";
	printf(" starting vector sum \n");

	init_vect(A_in, 0);
	init_vect(B_in, 1);
	
	struct fred_data *fred;
	struct fred_hw_task *hw_vsum;
    
	//sw_task_init(&fred);

	fred_init(&fred);
	
	// Bind with HW-memcpy having hw-id 100
	fred_bind(fred, &hw_vsum, 100);

	uint64_t *hw_vsum_buff_in0;
	// this buffer is not actually used
	uint64_t *hw_vsum_buff_out0;

	hw_vsum_buff_in0 = (uint64_t*) fred_map_buff(fred, hw_vsum, 0);
	hw_vsum_buff_out0 = (uint64_t*) fred_map_buff(fred, hw_vsum, 1);

	// set the memcpy parameters (dest, source)
	*(hw_vsum_buff_in0 + 0) = (uint64_t) C_out;
	*(hw_vsum_buff_in0 + 1) = (uint64_t) A_in;
	*(hw_vsum_buff_in0 + 2) = (uint64_t) B_in;

	// Call fred IP
	fred_accel(fred, hw_vsum);

	//validate
	if (check_output(C_out, ARRAY_SIZE) != ARRAY_SIZE){
		//std::cout << "Mismatch!\n";
		printf("Mismatch!\n");
		print_vect(C_out, 10);
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
