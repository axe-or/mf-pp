#include "memory.hpp"
#include "option.hpp"

int main(){
	using namespace mf;
	auto arena_memory = Slice(new byte[5000], 5000);

	Option<Arena> arena = Arena::from_buffer(arena_memory);

	Option<Arena> ad = move(arena);

	auto a2 = arena.unwrap();

	printf("%p", a2.create<i32>());

	return 0;
}

