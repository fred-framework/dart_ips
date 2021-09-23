#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "fred_lib.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(x,y) ( (x) > (y) ? (x) : (y) )

typedef uint64_t data_t;
const uint32_t AXIM_MAX_DATA_SIZE = 32;

// change here to set up the # in/out and execution cycles of the hw module
#define IN_MEM_SIZE 16
#define OUT_MEM_SIZE 16
#define EXEC_CYCLES 128

#define MAX(x,y) ( (x) > (y) ? (x) : (y) )
// do not change this part
// the input and output time does not count in the prem model; 20 is the constant additional latency of the internal pipeline. with this constant, the final latency match with EXEC_CYCLES
#define EXEC_SIZE MAX(0,EXEC_CYCLES-20-IN_MEM_SIZE-OUT_MEM_SIZE)

data_t *mem_in, *mem_out;

const int hw_id = 100;

void init_vect(data_t* base_idx, data_t value, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		base_idx[i] = value;
	}
}

void print_vect(data_t *base, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		printf("%d\t0x%lX\n",i,(long unsigned)base[i]);
	}
	printf("\n");
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

// pseudo random number generator based on an LFSR w 32 bits
// "basic C" version of the LFSR defined in prem.cpp
unsigned int pseudo_random(unsigned int seed) {
	unsigned int lfsr;
	lfsr = seed;
	unsigned char b_32 = (lfsr >> (32-32)) & 1;
	unsigned char b_22 = (lfsr >> (32-22)) & 1;
	unsigned char b_2  = (lfsr >> (32-2)) & 1;
	unsigned char b_1  = (lfsr >> (32-1)) & 1;
	unsigned char new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;
	lfsr = lfsr >> 1;
	//lfsr.set_bit(31, new_bit);
	lfsr |= (uint32_t)( new_bit ) << 31;

	return lfsr;
}

int main (int argc, char **argv)
{
	printf(" starting prem \n");

	struct fred_data *fred;
	struct fred_hw_task *hw_ip;
	int retval,i;
	unsigned int lfsr;
	int error_code = 0;
	data_t count_input_val=0;
	data_t expected_mem_out[OUT_MEM_SIZE];
	struct timespec start, end;
    
	retval = fred_init(&fred);
	if (retval) {
		printf("fred_init failed \n");
		error_code = 1;
	}
	
	// Bind with HW-prem having hw-id 100
	retval = fred_bind(fred, &hw_ip, hw_id);
	if (retval) {
		printf("fred_bind failed \n");
		error_code = 1;
	}

	mem_in = fred_map_buff(fred, hw_ip, 0);
	if (!mem_in) {
		printf("fred_map_buff failed on buff 0 for mem_in\n");
		error_code = 1;
	}
	mem_out = fred_map_buff(fred, hw_ip, 1);
	if (!mem_out) {
		printf("fred_map_buff failed on buff 1 for mem_out\n");
		error_code = 1;
	}

	// set input values
	mem_in[0]=IN_MEM_SIZE;
	mem_in[1]=EXEC_SIZE;
	mem_in[2]=OUT_MEM_SIZE;
	init_vect(&(mem_in[3]), 1, IN_MEM_SIZE);

	// Call fred IP
	clock_gettime(CLOCK_MONOTONIC, &start);
	retval = fred_accel(fred, hw_ip);
	if (retval) {
		printf("fred_accel failed \n");
		error_code = 1;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);		

	// Calculating total time taken by the FPGA offloading.
	double time_taken;
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by FRED is : %09f\n", time_taken);

	// calculate expected value
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (i = 3; i < IN_MEM_SIZE+3; ++i) {
		count_input_val += mem_in[i];
	}
	lfsr = count_input_val;
	//printf("in: 0x%lX\n", (long unsigned)lfsr);
	for (i = 0; i < EXEC_SIZE; ++i) {
		lfsr = pseudo_random(lfsr);
	}
	//printf("exec: 0x%lX\n", (long unsigned)lfsr);
	for (i = 0; i < OUT_MEM_SIZE; ++i) {
		expected_mem_out[i] = lfsr;
		lfsr = pseudo_random(lfsr);		
	}
	//printf("out: 0x%lX\n", (long unsigned)lfsr);
	clock_gettime(CLOCK_MONOTONIC, &end);

	// Calculating total time taken by the program running in the CPU.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by the CPU is : %09f\n", time_taken);

	// validation
	if (check_output(mem_out, expected_mem_out, OUT_MEM_SIZE) != 1){
		printf("Mismatch!\n");
		error_code = 1;
	}else{
		printf("Match!\n");
	}
	printf("Input Content [0:9]:\n");
	print_vect(&(mem_in[3]), MIN(5,IN_MEM_SIZE));
	printf("Expected Initial value at the output : 0x%lX \n", (long unsigned)expected_mem_out[0]);
	print_vect(expected_mem_out, MIN(5,OUT_MEM_SIZE));
	printf("Output Content [0:9]:\n");
	print_vect(mem_out, MIN(5,OUT_MEM_SIZE));

	// this loop is required just to avoid messing up with the printed messages 
	// caused by the messages printed by fred_free
	for(i=0;i<100000000;i++);

	//cleanup and finish
	fred_free(fred);

	printf("Fred finished\n");

	return(error_code);
}
