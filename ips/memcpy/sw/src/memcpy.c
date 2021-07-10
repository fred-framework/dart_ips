#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fred_lib.h"

typedef uint64_t data_t;

// make sure these constants match with the hw design
#define BUFF_SIZE 1024
#define BUFF_SIZE_BYTE (sizeof(data_t) * 1024)

void init_vect(data_t *base, int value, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		base[i] = (data_t)value + i;
	}
}

void print_vect(data_t *base, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		printf("%d\t%ld\n", i, base[i]);
	}
	printf("\n");
}

int main (int argc, char **argv)
{
	printf(" starting memcpy \n");
	int retval, error_code;

	data_t *mem_in, *mem_out;

	struct fred_data *fred;
	struct fred_hw_task *hw_memcpy;
    
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
	init_vect(mem_in, 0,BUFF_SIZE);
	/*
	for (unsigned int i = 0; i < BUFF_SIZE; ++i) {
		*(mem_in+i) = (data_t)i;
	}
	*/

	// Call fred IP
	retval = fred_accel(fred, hw_memcpy);
	if (retval) {
		printf("fred_accel failed \n");
		error_code = 1;
	}		

	//validate
	error_code = memcmp(mem_in,mem_out, BUFF_SIZE_BYTE);
	if (error_code !=0){
		printf("Mismatch!\n");
	}else{
		printf("Match!\n");
	}

	printf("IN:\n");
	print_vect(mem_in, 10);
	printf("OUT:\n");
	print_vect(mem_out, 10);
	//cleanup and finish
	fred_free(fred);

	printf("Fred finished\n");

	return(error_code);
}
