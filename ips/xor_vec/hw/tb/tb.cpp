
#include <iostream>
#include <cstring>
#include "hw_mod.hpp"
#include "hw_task.hpp"

void init_vect(data_t * base, data_t value)
{
	for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
		base[i] = value;
	}
}

void print_vect(data_t * base_idx, unsigned int size)
{
	for (int i = 0; i < size; ++i) {
		std::cout << i << "\t" << base_idx[i] << "\n";
	}
	std::cout << std::endl;
}

uint32_t check_output(data_t *base, unsigned int size, data_t expected_value)
{
	uint32_t sum=0;
	for (unsigned int i = 0; i < size; ++i) {
		if (base[i] != expected_value)
			return 0;
	}
	return 1;
}

int main()
{
	unsigned int a_idx = 0;

	args_t id_out;
	args_t args[ARGS_SIZE];
	int error_code = 0;

	data_t mem_in[ARRAY_SIZE*2];
	data_t mem_out[ARRAY_SIZE];


	init_vect(mem_in, 1); // the 1st half if for the vectorA
	init_vect(&(mem_in[ARRAY_SIZE]), 2); // the 2nd half if for the vectorB

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)0; // starting addr for the output
	args[1] = (args_t)0; // starting addr for the input A
	args[2] = (args_t)ARRAY_SIZE*sizeof(data_t); // starting addr for the input B

	xor_vec_top(&id_out, args, mem_in, mem_out);

	//validate
	if (check_output(mem_out, ARRAY_SIZE,1^2) != 1){
		std::cout << "Mismatch!\n";
		std::cout << "Content of A[0:9]:\n";
		print_vect(mem_in, 10);
		std::cout << "Content of B[0:9]:\n";
		print_vect(&(mem_in[ARRAY_SIZE]), 10);
		std::cout << "Content of C[0:9]:\n";
		print_vect(mem_out, 10);
		error_code = 1;
	}else{
		std::cout << "Match!\n";
	}

	std::cout << "Fred finished\n";

	return(error_code);

}

