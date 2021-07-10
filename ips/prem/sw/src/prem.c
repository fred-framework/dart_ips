#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fred_lib.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef uint64_t data_t;

// change here to set up the # in/out and execution cycles of the hw module
#define IN_MEM_SIZE 16
#define OUT_MEM_SIZE 16
#define EXEC_CYCLES 128

// do not change this part
#define IN_MEM_SIZE_BYTE (sizeof(data_t) * IN_MEM_SIZE)
#define OUT_MEM_SIZE_BYTE (sizeof(data_t) * OUT_MEM_SIZE)
// the input and output time does not count in the prem model; 30 is the constant additional latency of the internal pipeline
#define EXEC_SIZE EXEC_CYCLES-30-IN_MEM_SIZE-OUT_MEM_SIZE

data_t *mem_in, *mem_out;

const int hw_id = 100;

void init_vect(data_t* base_idx, data_t value, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		base_idx[i] = value + i;
	}
}

void print_vect(data_t *base, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		printf("%d\t%ld\n",i,base[i]);
	}
	printf("\n");
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

int main (int argc, char **argv)
{
	printf(" starting prem \n");

	struct fred_data *fred;
	struct fred_hw_task *hw_ip;
	int retval;
	int error_code = 0;
	data_t count_input_val=0;
    
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
	init_vect(mem_in, 0, IN_MEM_SIZE);

	// Call fred IP
	retval = fred_accel(fred, hw_ip);
	if (retval) {
		printf("fred_accel failed \n");
		error_code = 1;
	}		

	// calculate the base for the expected value
	for (int i = 0; i < IN_MEM_SIZE; ++i) {
		count_input_val += mem_in[i];
	}
	for (int i = 0; i < EXEC_SIZE; ++i) {
		count_input_val += i;
	}	

	if (check_output(mem_out, OUT_MEM_SIZE, count_input_val) != 1){
		printf("Mismatch!\n");
		error_code = 1;
	}else{
		//std::cout << "Match!\n";
		printf("Match!\n");
	}
	printf("Input Content [0:9]:\n");
	print_vect(&(mem_in[3]), MIN(10,IN_MEM_SIZE));
	printf("Expected Initial value at the output : %d \n", count_input_val);
	printf("Output Content [0:9]:\n");
	print_vect(mem_out, MIN(10,OUT_MEM_SIZE));

	//cleanup and finish
	fred_free(fred);

	printf("Fred finished\n");

	return(error_code);
}
