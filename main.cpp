#include "memory.hpp"
#include "option.hpp"
#include "result.hpp"

enum class Error {
	Bruh = 1,
};
int main(){
	using namespace mf;
	auto arena_memory = Slice<byte>(new byte[5000], 5000);

	Result<Arena, Error> arena = Arena::from_buffer(arena_memory);
	// arena = Result<Arena, Error>{Error::Bruh};

	Option<Arena> ad = arena.unwrap();

	auto a2 = ad.unwrap();

	printf("%p\n", a2.create<i32>());

	return 0;
}

