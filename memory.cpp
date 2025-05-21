#include "memory.hpp"

/* Memory runtime utilities */
#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
namespace mf {
void mem_copy(void* dest, void const* source, usize count) {
	__builtin_memmove(dest, source, count);
}

void mem_copy_no_overlap(void* dest, void const* source, usize count){
	__builtin_memcpy(dest, source, count);
}

void mem_set(void* dest, byte value, usize count){
	__builtin_memset(dest, value, count);
}

isize mem_compare(void const* lhs, void const* rhs, usize count){
	return __builtin_memcmp(lhs, rhs, count);
}
}
#else
#include <string.h>

namespace mf {
void mem_copy(void* dest, void const* source, usize count) {
	memmove(dest, source, count);
}

void mem_copy_no_overlap(void* dest, void const* source, usize count){
	memcpy(dest, source, count);
}

void mem_set(void* dest, byte value, usize count){
	memset(dest, value, count);
}

isize mem_compare(void const* lhs, void const* rhs, usize count){
	return memcmp(lhs, rhs, count);
}
}
#endif

