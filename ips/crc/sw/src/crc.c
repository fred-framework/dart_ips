#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fred_lib.h"

typedef uint64_t data_t;

// make sure these constants match with the hw design
#define  ARRAY_SIZE (1024)
//#define  BLOCK_SIZE_BYTE (ARRAY_SIZE * sizeof(data_t))
// the size of the vector where the CRC is actually calculated
#define  VET_SIZE 10

data_t *mem_in, *mem_out;

const int hw_id = 100;

void init_vect(data_t * base, data_t value)
{
	for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
		base[i] = value + i;
	}
}

void print_vect(data_t *base, unsigned int size)
{
	printf("[ ");
	for (unsigned int i = 0; i < size; ++i) {
		printf( "%02X", (uint8_t) base[i] );
	}
	printf("] \n");
}

// reference
uint64_t crc64(uint8_t *data, uint32_t len){
	uint64_t crc = 0;
	uint32_t i;
	//uint8_t byte;

	while(len--){
		//byte = *data;
		crc ^= (uint64_t)*data++ << 56;
		//crc ^= (uint64_t)byte << 56;
		//data++;

		for(i = 0; i < 8; ++i)
			crc = crc << 1 ^ (crc & 0x8000000000000000 ? 0x42f0e1eba9ea3693 : 0x0000000000000000); 
	}
	return crc;
}

int main (int argc, char **argv)
{
	printf(" starting CRC \n");
	int retval;
	int error_code = 0;

	struct fred_data *fred;
	struct fred_hw_task *hw_ip;
    
	retval = fred_init(&fred);
	if (retval) {
		printf("fred_init failed for hw-task %u\n", hw_id);
		error_code = 1;
	}
	
	// Bind with HW-memcpy having hw-id 100
	retval = fred_bind(fred, &hw_ip, hw_id);
	if (retval) {
		printf("fred_bind failed for hw-task %u\n", hw_id);
		error_code = 1;
	}

	mem_in  = fred_map_buff(fred, hw_ip, 0);
	if (!mem_in) {
		printf("fred_map_buff failed on buff 0 for mem_in\n");
		error_code = 1;
	}
	mem_out = fred_map_buff(fred, hw_ip, 1);
	if (!mem_out) {
		printf("fred_map_buff failed on buff 1 for mem_out\n");
		error_code = 1;
	}

	// set the vet length and its initial value
	mem_in[0] = 10;
	init_vect(&(mem_in[1]), 0x4030201004030201);

	// Call fred IP
	retval = fred_accel(fred, hw_ip);
	if (retval) {
		printf("fred_accel failed for hw-task %u\n", hw_id);
		error_code = 1;
	}

	// compare the hw output mem_out[0] w the sw reference
	data_t expected_out = crc64((uint8_t*)&(mem_in[1]),VET_SIZE*sizeof(data_t));
	if (expected_out != mem_out[0]){
		printf("Mismatch!\n");
		error_code = 1;
	}else{
		printf("Match!\n");
	}

	printf("Expected value: ");
	print_vect((uint8_t*)&expected_out, 8);
	printf("Output value  : ");
	print_vect((uint8_t*)&(mem_out[0]), 8);

	//cleanup and finish
	fred_free(fred);
	printf("Fred finished\n");

	return(error_code);
}
