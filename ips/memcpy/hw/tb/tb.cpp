
#include <iostream>
#include <cstring>
#include "memcpy_top.hpp"
#include "memcpy.hpp"

data_t mem_in[BUFF_SIZE];
data_t mem_out[BUFF_SIZE];

void init_vect(data_t* base_idx, int value)
{
	for (unsigned int i = 0; i < BUFF_SIZE; ++i) {
		base_idx[i] = value + i;
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

	init_vect(mem_in, 0);

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)((data_t)&mem_out[a_idx]);
	args[1] = (args_t)((data_t)&mem_in[a_idx]);

	memcpy_top(&id_out, args, mem_in, mem_out);

	if (memcmp(mem_in,mem_out, BUFF_SIZE_BYTE) !=0){
		printf("Mismatch!\n");
		print_vect(0, 10);
		return(1);
	}
	printf("Match!\n");
	return 0;
}

