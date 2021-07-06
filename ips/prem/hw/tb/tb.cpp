
#include <iostream>
#include <cstring>
#include "prem_top.hpp"
#include "prem.hpp"

data_t mem_in[BUFF_SIZE];
data_t mem_out[BUFF_SIZE];

void init_vect(data_t* base_idx, int value)
{
	for (unsigned int i = 0; i < BUFF_SIZE; ++i) {
		base_idx[i] = value + i;
	}
}

void print_vect(data_t * base_idx, unsigned int size)
{
	for (int i = 0; i < size; ++i) {
		std::cout << i << "\t" << base_idx[i] << "\n";
	}
	std::cout << std::endl;
}

// the expected output is the sum of the inputs + the number fo clock cycles for execution + the output position index.
// for instance, if the input is a vectore of 10 positions with values 1, 2, 3 ... 10
// the there is 1000 clock cycles for execution, then the output is:
// 1+2+3+...+10+1000 + 0 for position 0 of the output vector
// 1+2+3+...+10+1000 + 1 for position 1 of the output vector
// 1+2+3+...+10+1000 + 2 for position 2 of the output vector
// ...
uint32_t check_output(data_t *base, unsigned int size, data_t expected_value)
{
	for (unsigned int i = 0; i < size; ++i) {
		if (base[i] != (expected_value+i))
			return 0;
	}
	return 1;
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

	prem_top(&id_out, args, mem_in, mem_out);

	if (memcmp(mem_in,mem_out, BUFF_SIZE_BYTE) !=0){
		printf("Mismatch!\n");
		print_vect(0, 10);
		return(1);
	}
	printf("Match!\n");
	return 0;
}

