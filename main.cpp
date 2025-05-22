#include "memory.hpp"
#include "option.hpp"
#include "result.hpp"

enum class Error {
	Bruh = 1,
};

int main(){
	using namespace mf;
	auto arena_memory = Slice<byte>(new byte[5000], 5000);

	auto pool = Pool::init(arena_memory, 128, 16).unwrap();
	printf("N:%td A:%td\n", pool.block_size, pool.block_align);
	printf("%p\n", pool.alloc(4,4));

	return 0;
}

