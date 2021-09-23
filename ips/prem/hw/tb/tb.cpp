
#include <iostream>
#include <cstring>
#include "ap_int.h"

// make sure these parameters match the same used in the hw module
const uint32_t AXIM_MAX_DATA_SIZE = 32;
typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;

// change here to set up the # in/out and execution cycles of the hw module
#define IN_MEM_SIZE 16
#define OUT_MEM_SIZE 16
#define EXEC_CYCLES 128

#define MAX(x,y) ( (x) > (y) ? (x) : (y) )
// do not change this part
// the input and output time does not count in the prem model; 20 is the constant additional latency of the internal pipeline. with this constant, the final latency match with EXEC_CYCLES
#define EXEC_SIZE MAX(0,EXEC_CYCLES-20-IN_MEM_SIZE-OUT_MEM_SIZE)

void prem_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void init_vect(data_t* base_idx, int value, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		base_idx[i] = value;
	}
}

void print_vect(data_t * base_idx, unsigned int size)
{
	for (int i = 0; i < size; ++i) {
		//std::cout << i << "\t" << base_idx[i] << "\n";
		printf("%d\t0x%lX\n",i,(long unsigned)base_idx[i]);
	}
	std::cout << std::endl;
}

// compare both vectors
uint32_t check_output(data_t *base, data_t *expected, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		if (base[i] != expected[i])
			return 0;
	}
	return 1;
}

// implements a single iteration of the LFSR. defined in prem.cpp
unsigned int pseudo_random(unsigned int seed);

int main()
{
	unsigned int count_input_val = 0, i, lfsr;
	args_t id_out;
	args_t args[ARGS_SIZE];
	int error_code = 0;

	data_t mem_in[IN_MEM_SIZE]; // lets make sure that all the input buffer is initialized, even if it is not entirely used
	data_t mem_out[OUT_MEM_SIZE];
	data_t expected_mem_out[OUT_MEM_SIZE];

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)0;
	args[1] = (args_t)0;

	// assuming the input vector is filled with 'ones' 
	init_vect(mem_in, 1, IN_MEM_SIZE);

	//printf("before top\n");
	// C/RTL cosim requires at least two executions of the design under test
	prem_top(&id_out, args, mem_in, mem_out);
	// uncomment if you want to simulate 2 flows
	//prem_top(&id_out, args, mem_in, mem_out);
	//printf("after top\n");

	// calculate the expected value
	for (i = 0; i < IN_MEM_SIZE; ++i) {
		count_input_val += mem_in[i];
	}
	lfsr = count_input_val;
	printf("in: 0x%lX\n", (long unsigned)lfsr);
	for (i = 0; i < EXEC_SIZE; ++i) {
		lfsr = pseudo_random(lfsr);
	}
	printf("exec: 0x%lX\n", (long unsigned)lfsr);
	for (i = 0; i < OUT_MEM_SIZE; ++i) {
		expected_mem_out[i] = lfsr;
		lfsr = pseudo_random(lfsr);
	}
	printf("out: 0x%lX\n", (long unsigned)lfsr);

	// validate
	if (check_output(mem_out, expected_mem_out, OUT_MEM_SIZE) != 1){
		std::cout << "Mismatch!\n";
		error_code = 1;
	}else{
		std::cout << "Match!\n";
	}
	std::cout << "Input Content [0:9]:\n";
	print_vect(mem_in, MIN(10,IN_MEM_SIZE));
	printf("Expected Initial value at the output : 0x%lX \n", (long unsigned)expected_mem_out[0]);
	print_vect(expected_mem_out, MIN(10,OUT_MEM_SIZE));
	std::cout << "Output Content [0:9]:\n";
	print_vect(mem_out, MIN(10,OUT_MEM_SIZE));

	std::cout << "TB finished\n";

	return(error_code);
}
