
#include <iostream>
#include <cstring>
#include "memcpy_top.hpp"
#include "memcpy.hpp"

data_t mem_in[BLOCK_SIZE_DT];
data_t mem_out[BLOCK_SIZE_DT];

void init_vect(unsigned int base_idx, int value)
{
	for (unsigned int i = 0; i < BLOCK_SIZE_DT; ++i) {
		mem_in[base_idx + i] = value + i;
	}
}

void print_vect(unsigned int base_idx, unsigned int size)
{
	for (int i = 0; i < size; ++i) {
		std::cout << mem_in[base_idx + i] << "\t" << mem_out[base_idx + i] << "\n";
	}
	std::cout << std::endl;
}

int main()
{
	unsigned int a_idx = 0;

	args_t id_out;
	args_t args[ARGS_SIZE];

	init_vect(a_idx, 0);

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)((uint64_t)&mem_out[a_idx]);
	args[1] = (args_t)((uint64_t)&mem_in[a_idx]);

	memcpy_top(&id_out, args, mem_in, mem_out);

	if (memcmp(mem_in,mem_out, BLOCK_SIZE_BYTE) !=0){
		printf("Mismatch!\n");
		print_vect(0, 10);
		return(1);
	}
	printf("Match!\n");
	return 0;
}

