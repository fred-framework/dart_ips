
#include <iostream>
#include <cstring>

// make sure these parameters match the same used in the hw module
const uint32_t AXIM_MAX_DATA_SIZE = 32;
typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;

// change here to set up the # in/out and execution cycles of the hw module
#define IN_MEM_SIZE 16
#define OUT_MEM_SIZE 16
#define EXEC_CYCLES 128

// do not change this part
#define IN_MEM_SIZE_BYTE (sizeof(data_t) * IN_MEM_SIZE)
#define OUT_MEM_SIZE_BYTE (sizeof(data_t) * OUT_MEM_SIZE)
// the input and output time does not count in the prem model; 30 is the constant additional latency of the internal pipeline
#define EXEC_SIZE EXEC_CYCLES-30-IN_MEM_SIZE-OUT_MEM_SIZE
void prem_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void init_vect(data_t* base_idx, int value, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		base_idx[i] = value + i;
	}
}

void print_vect(data_t * base_idx, unsigned int size)
{
	for (int i = 0; i < size; ++i) {
		std::cout << i << "\t" << base_idx[i] << "\n";
	}
	std::cout << std::endl;
}

// the expected output is the sum of the inputs + the number for clock cycles for execution + the output position index.
// for instance, if the input is a vector of 10 positions with values 1, 2, 3 ... 10
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

int main()
{
	unsigned int count_input_val = 0;
	args_t id_out;
	args_t args[ARGS_SIZE];
	int error_code = 0;

	data_t mem_in[IN_MEM_SIZE];
	data_t mem_out[OUT_MEM_SIZE];

	// checking configuration
	if (EXEC_SIZE <= 0){
		std::cout << "EXEC_SIZE must be positive\n";
		return 1;
	}

	if (AXIM_MAX_DATA_SIZE-IN_MEM_SIZE <= 0){
		std::cout << "IN_MEM_SIZE is too large for the current hw FIFO size\n";
		return 1;
	}

	if (AXIM_MAX_DATA_SIZE-OUT_MEM_SIZE <= 0){
		std::cout << "OUT_MEM_SIZE is too large for the current hw FIFO size\n";
		return 1;
	}

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)0;
	args[1] = (args_t)0;

	init_vect(mem_in, 0, IN_MEM_SIZE);

	// C/RTL cosim requires at least two executions of the design under test
	prem_top(&id_out, args, mem_in, mem_out);
	// uncomment if you want to simulate 2 flows
	//prem_top(&id_out, args, mem_in, mem_out);

	// calculate the base for the expected value
	for (int i = 0; i < IN_MEM_SIZE; ++i) {
		count_input_val += mem_in[i];
	}
	for (int i = 0; i < EXEC_SIZE; ++i) {
		count_input_val += i;
	}

	//validate
	if (check_output(mem_out, OUT_MEM_SIZE, count_input_val) != 1){
		std::cout << "Mismatch!\n";
		error_code = 1;
	}else{
		std::cout << "Match!\n";
	}
	std::cout << "Input Content [0:9]:\n";
	print_vect(mem_in, MIN(10,IN_MEM_SIZE));
	std::cout << "Expected Initial value at the output :\n";
	std::cout << count_input_val << std::endl;
	std::cout << "Output Content [0:9]:\n";
	print_vect(mem_out, MIN(10,OUT_MEM_SIZE));

	std::cout << "TB finished\n";

	return(error_code);
}

