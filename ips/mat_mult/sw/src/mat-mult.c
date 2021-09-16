#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "fred_lib.h"

typedef uint64_t data_t;
const unsigned int MAT_SIZE = 64;
data_t *mem_a, *mem_b,*mem_out;
const int hw_id = 100;


void print_mat(data_t *base_idx, unsigned int size)
{
	int i,j;
	printf("[\n");
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			printf("%lld ", base_idx[i + size * j]);
		}
		printf("\n");
	}
	printf("]\n");
}

// reference
void mat_mult_sw(data_t *dataA, data_t *dataB, data_t *dataOut, uint32_t mat_size){
	uint32_t c,d,k;
	data_t tot=0,a,b;

	for (c = 0; c < mat_size; c++) {
		for (d = 0; d < mat_size; d++) {
			for (k = 0; k < mat_size; k++) {
				a = dataA[c + mat_size * k];
				b = dataB[k + mat_size * d];
				tot = tot + a * b;
			}
			dataOut[c + mat_size * d] = tot;
			tot = 0;
		}
	}
}

int main (int argc, char **argv)
{
	printf(" starting Matrix Multiplication[%d][%d]\n",MAT_SIZE,MAT_SIZE);
	int retval;
	int error_code = 0,idx,aux,i,j;
	data_t mem_expected_out[MAT_SIZE*MAT_SIZE];
	struct timespec start, end;

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

	mem_a  = fred_map_buff(fred, hw_ip, 0);
	if (!mem_a) {
		printf("fred_map_buff failed on buff 0 for mem_a\n");
		error_code = 1;
	}
	mem_b  = fred_map_buff(fred, hw_ip, 1);
	if (!mem_b) {
		printf("fred_map_buff failed on buff 1 for mem_b\n");
		error_code = 1;
	}
	mem_out = fred_map_buff(fred, hw_ip, 2);
	if (!mem_out) {
		printf("fred_map_buff failed on buff 2 for mem_out\n");
		error_code = 1;
	}

	// set the input initial value
	// fills mem_a with
	// 	{2,0,0,0},
	// 	{0,2,0,0},
	// 	{0,0,2,0},
	// 	{0,0,0,2}	
	idx=0;
	for (i=0;i<MAT_SIZE;i++){
		for (j=0;j<MAT_SIZE;j++){
			if (i == j)
				mem_a[idx] = 2;
			else
				mem_a[idx] = 0;
			idx++;
		}
	}

	// fill mem_b with 1, 2, 3, 4, ..., 15
	idx=0;
	for (i=0;i<MAT_SIZE;i++){
		for (j=0;j<MAT_SIZE;j++){
			mem_b[idx] = idx;
			idx++;
		}
	}	

	// Call fred IP
	clock_gettime(CLOCK_MONOTONIC, &start);
	retval = fred_accel(fred, hw_ip);
	if (retval) {
		printf("fred_accel failed for hw-task %u\n", hw_id);
		error_code = 1;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);		

	// Calculating total time taken by the FPGA offloading.
	double time_taken;
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by FRED is : %09f\n", time_taken);

	// generate the reference output
	clock_gettime(CLOCK_MONOTONIC, &start);
	mat_mult_sw((data_t *)mem_a, (data_t *)mem_b, (data_t *)mem_expected_out, MAT_SIZE);
	clock_gettime(CLOCK_MONOTONIC, &end);

	// Calculating total time taken by the program running in the CPU.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by the CPU is : %09f\n", time_taken);


	if (memcmp(mem_out, mem_expected_out, MAT_SIZE*MAT_SIZE*sizeof(data_t))){
		printf("Mismatch!\n");
		error_code = 1;
	}else{
		printf("Match!\n");
	}

	printf("Expected value: ");
	print_mat((data_t *)mem_expected_out, MAT_SIZE);
	printf("Output value  : ");
	print_mat((data_t *)mem_out, MAT_SIZE);

	// this loop is required just to avoid messing up with the printed messages 
	// caused by the messages printed by fred_free
	for(i=0;i<100000000;i++);

	//cleanup and finish
	fred_free(fred);
	printf("Fred finished\n");

	return(error_code);
}
