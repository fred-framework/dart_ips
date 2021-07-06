#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fred_lib.h"

typedef uint64_t data_t;

// make sure these constants match with the hw design
#define IN_BUFF_SIZE 64
#define IN_BUFF_SIZE_BYTE (sizeof(data_t) * IN_BUFF_SIZE)
#define EXEC_SIZE 1024
#define OUT_BUFF_SIZE 64
#define OUT_BUFF_SIZE_BYTE (sizeof(data_t) * OUT_BUFF_SIZE)

data_t *mem_in, *mem_out;

void init_vect(data_t *base, int value)
{
	for (unsigned int i = 0; i < BUFF_SIZE; ++i) {
		base = value + i;
		base++;
	}
}

void print_vect(data_t * base, unsigned int size)
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
	int retval, error_code;

	struct fred_data *fred;
	struct fred_hw_task *hw_prem;
    
	retval = fred_init(&fred);
	if (retval) {
		printf("fred_init failed \n");
		error_code = 1;
	}		
	
	// Bind with HW-prem having hw-id 100
	retval = fred_bind(fred, &hw_prem, 100);
	if (retval) {
		printf("fred_bind failed \n");
		error_code = 1;
	}		

	mem_out = fred_map_buff(fred, hw_prem, 0);
	if (mem_out) {
		printf("fred_map_buff failed on buff 0 for mem_out\n");
		error_code = 1;
	}		
	mem_in = fred_map_buff(fred, hw_prem, 1);
	if (mem_in) {
		printf("fred_map_buff failed on buff 1 for mem_in\n");
		error_code = 1;
	}		

	// set the prem parameters (dest, source)
	//init_vect(mem_in, 0);
	for (unsigned int i = 0; i < BUFF_SIZE; ++i) {
		*(mem_in+i) = (data_t)i;
	}

	// Call fred IP
	retval = fred_accel(fred, hw_prem);
	if (retval) {
		printf("fred_accel failed \n");
		error_code = 1;
	}		

	//validate
	error_code = memcmp(mem_in,mem_out, BUFF_SIZE_BYTE);
	if (error_code !=0){
		printf("Mismatch!\n");
		print_vect(0, 10);
	}else{
		printf("Match!\n");
	}

	//cleanup and finish
	fred_free(fred);

	printf("Fred finished\n");

	return(error_code);
}
