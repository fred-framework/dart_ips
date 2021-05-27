#include <stdio.h>
#include "fred_lib.h"
#include <stdint.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	printf(" starting memcpy2 \n");
	
	const unsigned BUFF_SIZE = 10;

	struct fred_data *fred;
	struct fred_hw_task *hw_task_0;
    
	//sw_task_init(&fred);

	fred_init(&fred);
	
	// Bind with HW-task 0 having hw-id 100
	fred_bind(fred, &hw_task_0, 100);

	uint64_t *hw_task_0_buff_0;
	uint64_t *hw_task_0_buff_1;

	hw_task_0_buff_0 = fred_map_buff(fred, hw_task_0, 0);
	hw_task_0_buff_1 = fred_map_buff(fred, hw_task_0, 1);

	//init the input buffer
	int expected_sum=0;
	for(unsigned i = 0; i < BUFF_SIZE; i++) {
        *(hw_task_0_buff_0 + i) = (uint64_t) i;
		expected_sum +=i;
	}

	// Call fir_accelerator
	fred_accel(fred, hw_task_0);

	//validate		
    int received_val, received_sum = 0;
    for (unsigned j = 0; j < BUFF_SIZE; j++) {
		received_val = *(uint64_t *) (hw_task_0_buff_1 + j);
        printf("%llu\n", received_val);
		received_sum += received_val;
    }

    if (received_sum != expected_sum)
        printf("Test FAILED\n");
    else
        printf("Test PASSED.");

	//cleanup and finish
	fred_free(fred);

	printf("Fred finished \n");
}
