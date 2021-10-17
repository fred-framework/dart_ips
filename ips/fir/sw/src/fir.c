#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "fred_lib.h"

#define BUFF_SIZE 4 * 1024
#define FIR_WINDOW_SIZE 100
#define OUT_BUFF_SIZE = BUFF_SIZE + FIR_WINDOW_SIZE - 1

typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;

void fir_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

void print_vect(data_t *base, unsigned int size)
{
	printf("[ ");
	for (unsigned int i = 0; i < size; ++i) {
			printf("%llu ", base[i]);
	}
	printf("] \n");
}

void fir_reference(data_t *mem_port_in, data_t *mem_port_out)
{
    int j, i;
    int acc;
    int in_buff [BUFF_SIZE], out_buff [BUFF_SIZE];
    //Shift registers
    int shift_reg[FIR_WINDOW_SIZE];

    reset_shift_reg:for (j = 0; j < FIR_WINDOW_SIZE; j++) {
        shift_reg[j] = 0;
    }

    //fetch data
    fetch_loop:for (unsigned i = 0; i < BUFF_SIZE; i++) {
        in_buff[i] = (int)mem_port_in[i];
    }

    //FIR coeff
    const int coeff[FIR_WINDOW_SIZE] = {
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6,
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6,
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6,
            13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6
    };

    // loop through each output
    main_loop:for (i = 0; i < BUFF_SIZE; i++ ) {
        acc = 0;
        // shift registers
        shift_reg:for (j = FIR_WINDOW_SIZE - 1; j > 0; j--) {
            shift_reg[j] = shift_reg[j -1];
        }
        // put the new input value into the first register
        shift_reg[0] = in_buff[i];
        // do multiply-accumulate operation
        mac:for (j = 0; j < FIR_WINDOW_SIZE; j++) {
            acc += shift_reg[j] * coeff[j];
        }
        out_buff[i] = acc;
    }

    //store
    store_loop:for (i = 0; i < BUFF_SIZE; i++) {
        mem_port_out[i] = (long)out_buff[i];
    }
}


int main (int argc, char **argv)
{
	printf(" starting fir \n");
    args_t id_out;
    args_t args[ARGS_SIZE];
    data_t out_buff_gold[BUFF_SIZE];
    int i,j,retval,error_code = 0;

	struct fred_data *fred;
	struct fred_hw_task *hw_ip;
	const int hw_id = 100;
	// MAKE SURE the FRED buffers have size > of BUFF_SIZE*sizeof(data_t)
	data_t *in_buff;
	data_t *out_buff;

	struct timespec start, end;
	double time_taken;

    // fred setup
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

	in_buff = fred_map_buff(fred, hw_ip, 0);
	if (!in_buff) {
		printf("fred_map_buff failed on buff 0 for in_buff\n");
		error_code = 1;
	}
	out_buff = fred_map_buff(fred, hw_ip, 1);
	if (!out_buff) {
		printf("fred_map_buff failed on buff 1 for out_buff\n");
		error_code = 1;
	}

    //prepare input data
    for(i = 0; i < BUFF_SIZE; i++)
        in_buff[i] = (data_t) i;

    // running the reference output
	clock_gettime(CLOCK_MONOTONIC, &start);
	fir_reference(in_buff,out_buff_gold);
	clock_gettime(CLOCK_MONOTONIC, &end);

	// Calculating total time taken by the sw implementation.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by the CPU is : %09f\n", time_taken);

	// Call fir_accelerator
	clock_gettime(CLOCK_MONOTONIC, &start);
	fred_accel(fred, hw_ip);
	clock_gettime(CLOCK_MONOTONIC, &end);

	// Calculating total time taken by the sw implementation.
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
	printf("Time taken by FRED is : %09f\n", time_taken);

	//validate		
	if (memcmp(out_buff_gold, out_buff,BUFF_SIZE*sizeof(data_t)) != 0){
		printf("Mismatch!\n");
		error_code = 1;
	}else{
		printf("Match!\n");
	}

	printf("Expected value[0:9]:\n");
	print_vect(out_buff_gold, 10);
	printf("Output value[0:9]:\n");
	print_vect(out_buff, 10);

	// this loop is required just to avoid messing up with the printed messages 
	// caused by the messages printed by fred_free
	for(i=0;i<100000000;i++);

	//cleanup and finish
	fred_free(fred);
	printf("Fred finished \n");

	return error_code;
}
