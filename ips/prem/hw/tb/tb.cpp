
#include <iostream>
#include <cstring>

#define IN_BUFF_SIZE 10
#define OUT_BUFF_SIZE 13
#define EXEC_CYCLES 10000

#define IN_BUFF_SIZE_BYTE (sizeof(data_t) * IN_BUFF_SIZE)
#define OUT_BUFF_SIZE_BYTE (sizeof(data_t) * OUT_BUFF_SIZE)
// the input and output time does not count in the prem model; 30 is the constant additional latency of the internal pipeline
#define EXEC_SIZE EXEC_CYCLES-30-IN_BUFF_SIZE-OUT_BUFF_SIZE

#if defined EXEC_SIZE <= 0
#error "EXEC_SIZE must be positive"
#endif

typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;

void prem_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);
void prem(volatile data_t *mem_in, volatile data_t *mem_out);


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

	data_t mem_in[IN_BUFF_SIZE];
	data_t mem_out[OUT_BUFF_SIZE];	

	init_vect(mem_in, 0, IN_BUFF_SIZE);

	prem_top(&id_out, args, mem_in, mem_out);

	// calculate the base for the expected value
	for (int i = 0; i < IN_BUFF_SIZE; ++i) {
		count_input_val += mem_in[i];
	}
	for (int i = 0; i < EXEC_SIZE; ++i) {
		count_input_val += i;
	}

	//validate
	if (check_output(mem_out, OUT_BUFF_SIZE, count_input_val) != 1){
		std::cout << "Mismatch!\n";
		std::cout << "Input Content [0:9]:\n";
		print_vect(mem_in, MIN(10,IN_BUFF_SIZE));
		std::cout << "Output Content [0:9]:\n";
		print_vect(mem_out, MIN(10,OUT_BUFF_SIZE));
		error_code = 1;
	}else{
		std::cout << "Match!\n";
	}
	std::cout << "Fred finished\n";

	return(error_code);
}

