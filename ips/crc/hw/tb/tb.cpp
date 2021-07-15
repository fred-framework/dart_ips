
#include <iostream>
#include <cstring>

typedef uint32_t args_t;
typedef uint64_t data_t;
static const uint8_t ARGS_SIZE = 8;

// the size of the vector allocated in memory
const unsigned int BUFF_SIZE = 1024;
//const unsigned int BUFF_SIZE_BYTE = (sizeof(data_t) * BUFF_SIZE);
// the size of the vector where the CRC is actually calculated
const unsigned int VET_SIZE = 10;

void crc_top(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out);

data_t mem_in[BUFF_SIZE];
data_t mem_out[1]; // yeah! weird, but it avoids changing other parts of the TB ;)

void init_vect(data_t* base_idx, data_t value)
{
	for (unsigned int i = 0; i < BUFF_SIZE; ++i) {
		base_idx[i] = value + i;
	}
}

void print_vect(uint8_t *base_idx, unsigned int size)
{
	std::cout << "[ ";
	for (int i = 0; i < size; ++i) {
		printf( "%02X", (uint8_t) base_idx[i] );
	}
	std::cout << " ]" << std::endl;
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

int main()
{
	unsigned int a_idx = 0;
	int error_code=0;

	args_t id_out;
	args_t args[ARGS_SIZE];

	// the size of the input vet
	mem_in[0] = VET_SIZE;
	init_vect(&(mem_in[1]), 0x4030201004030201);

	// Set hw accelerator args
	// The base address is the memory array start address
	args[0] = (args_t)0;
	args[1] = (args_t)0;

	crc_top(&id_out, args, mem_in, mem_out);

	// compare the hw output mem_out[0] w the sw reference
	data_t expected_out = crc64((uint8_t*)&(mem_in[1]),VET_SIZE*sizeof(data_t));
	if (expected_out != mem_out[0]){
		std::cout << "Mismatch!\n";
		error_code = 1;
	}else{
		std::cout << "Match!\n";
	}

	std::cout << "Expected value: ";
	print_vect((uint8_t*)&expected_out, 8);
	std::cout << "Output value  : ";
	print_vect((uint8_t*)&(mem_out[0]), 8);
	
	return error_code ;
}
