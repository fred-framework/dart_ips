
#include <iostream>
#include <cstring>

typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;
const unsigned int MAT_SIZE = 4;

void mat_mult_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

void print_mat(data_t *base_idx, unsigned int size)
{
	int i,j;
	std::cout << "[ \n";
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			std::cout << base_idx[i + size * j] <<  " ";
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}

// reference
void mat_mult_sw(data_t *dataA, data_t *dataB, data_t *dataOut, uint32_t mat_size){
	uint32_t c,d,k;
	data_t tot=0,a,b;

	for (c = 0; c < mat_size; c++) {
		for (d = 0; d < mat_size; d++) {
			for (k = 0; k < mat_size; k++) {
//				a = *(dataA[c + mat_size * k]);
//				b = *(dataB[k + mat_size * d]);
				a = dataA[c + mat_size * k];
				b = dataB[k + mat_size * d];
				tot = tot + a * b;
			}
			//*dataOut[c + mat_size * d] = tot;
			dataOut[c + mat_size * d] = tot;
			tot = 0;
		}
	}
}

int main()
{
	unsigned int a_idx = 0;
	int error_code=0, i, j, aux;

	args_t id_out;
	args_t args[ARGS_SIZE];

	data_t mem_in[2][MAT_SIZE][MAT_SIZE];
	data_t * mem_a = &(mem_in[0][0][0]);
	data_t * mem_b = &(mem_in[1][0][0]);
	data_t * mem_b_aux = (data_t *)mem_b;
	data_t * mem_a_aux = (data_t *)mem_a;
	data_t mem_expected_out[MAT_SIZE][MAT_SIZE];
	data_t mem_out[MAT_SIZE][MAT_SIZE];

	// fills mem_a with
	// 	{2,0,0,0},
	// 	{0,2,0,0},
	// 	{0,0,2,0},
	// 	{0,0,0,2}
	for (i=0;i<MAT_SIZE;i++){
		for (j=0;j<MAT_SIZE;j++){
			if (i == j)
				*mem_a_aux = 2;
			else
				*mem_a_aux = 0;
			mem_a_aux++;
		}
	}

	// fill mem_b with 1, 2, 3, 4, ..., 15
	aux=0;
	for (i=0;i<MAT_SIZE;i++){
		for (j=0;j<MAT_SIZE;j++){
			*mem_b_aux = aux;
			aux++;
			mem_b_aux++;
		}
	}

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)0; // mem_a
	args[1] = (args_t)MAT_SIZE*MAT_SIZE*sizeof(data_t); // mem_b
	args[2] = (args_t)0; // mem_out

	mat_mult_top(&id_out, args, (data_t *)mem_in, (data_t *)mem_out);

	// generate the reference output
	//mat_mult_sw((data_t *)&(mem_in[0][0][0]), (data_t *)&(mem_in[1][0][0]), (data_t *)mem_expected_out, MAT_SIZE);
	mat_mult_sw((data_t *)mem_a, (data_t *)mem_b, (data_t *)mem_expected_out, MAT_SIZE);

	if (memcmp(mem_out, mem_expected_out, MAT_SIZE*MAT_SIZE*sizeof(data_t))){
		std::cout << "Mismatch!\n";
		error_code = 1;
	}else{
		std::cout << "Match!\n";
	}

	std::cout << "Expected value: ";
	print_mat((data_t *)mem_expected_out, MAT_SIZE);
	std::cout << "Output value  : ";
	print_mat((data_t *)mem_out, MAT_SIZE);
	
	return error_code ;
}
