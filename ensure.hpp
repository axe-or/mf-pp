#pragma once
#include "build_context.hpp"
#include "types.hpp"

#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
	#define mf_trap_impl() __builtin_trap()
#elif defined(COMPILER_MSVC)
	#define mf_trap_impl() __debugbreak()
#else
	extern "C" { [[noreturn]] void abort() noexcept; }
	#define mf_trap_impl() ::abort()
#endif

extern "C" { int printf(char const *, ...); }

namespace mf  {

[[noreturn]] static inline
void panic(char const* msg){
	printf("Panic: %s\n", msg);
	mf_trap_impl();
}

static inline
void ensure(bool predicate, char const* msg){
	if(!predicate){
		printf("Assertion failed: %s\n", msg);
		mf_trap_impl();
	}
}

static inline
void bounds_check(bool predicate, char const* msg){
	#if !defined(DISABLE_BOUNDS_CHECK)
	if(!predicate){
		printf("Bounds check error: %s\n", msg);
		mf_trap_impl();
	}
	#else
	(void)lo; (void)index; (void)hi;
	#endif
}
}

#undef mf_trap_impl
