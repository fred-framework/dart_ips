#include <stdio.h>
#include "fred_lib.h"
#include <stdint.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	printf(" starting fir \n");
	
	const unsigned BUFF_SIZE = 10;
	const unsigned FIR_WINDOW_SIZE = 25;
	
	uint64_t coeff[] = {13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 13, -2, 9, 11, 26, 18, 95, -43, 6, 74, 26, 18, 95, -43, 6};
	uint64_t in_buff [BUFF_SIZE], out_buff_gold[BUFF_SIZE];

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

	//init the fir input buffer
	for(unsigned i = 0; i < BUFF_SIZE; i++) {
        *(hw_task_0_buff_0 + i) = (uint64_t) i;
		in_buff[i] = (uint64_t) i;
	}

	//calculate gold
    for(unsigned n = 0; n < BUFF_SIZE; n++) {
        out_buff_gold[n] = 0;    
        for (unsigned i = 0; i < FIR_WINDOW_SIZE; i++){    
            int temp = (int) n - i;
            if(temp >= 0)
                out_buff_gold[n] += coeff[i] * in_buff[n - i];
            }
        }

	// Call fir_accelerator
	fred_accel(fred, hw_task_0);

	//validate		
    int errors = 0;
    for (unsigned j = 0; j < BUFF_SIZE; j++) {
        printf("%llu  %llu \n", *(uint64_t *) (hw_task_0_buff_1 + j), out_buff_gold[j]);
	if (out_buff_gold[j] != *(hw_task_0_buff_1 + j))
            errors++;
    }

    if (errors)
        printf("Test FAILED with %i errors \n", errors);
    else
        printf("Test PASSED.");

	//cleanup and finish
	fred_free(fred);

	printf("Fred finished \n");
}
