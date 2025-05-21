#pragma once

#if defined(__clang__)
	#define COMPILER_CLANG
#elif defined(__GCC__) || defined(__GNUC__)
	#define COMPILER_GCC
#elif defined(_MSC_VER)
	#define COMPILER_MSVC
#else
	#define COMPILER_OTHER
#endif

#if defined (__linux__)
	#define OS_LINUX
#elif defined(_WIN32) || defined(_WIN64)
	#define OS_WINDOWS
#endif

#if defined(COMPILER_CLANG) || defined (COMPILER_GCC)
	#define mf_force_inline __attribute__((always_inline))
#elif defined(COMPILER_MSVC)
	#define mf_force_inline __forceinline
#else
	#define mf_force_inline
#endif

