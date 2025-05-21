#pragma once
#include "traits.hpp"
#include "types.hpp"
#include "ensure.hpp"
#include "slice.hpp"

void* operator new(size_t, void*);

namespace mf {

template<typename T>
T align_forward(T v, T a){
	ensure((a != 0) && ((a & (a - 1)) == 0), "Inaalid alignment");

	T mod = v & (a - T(1)); /* Fast modulo for powers of 2 */
	if(mod > T(0)){
		v += (a - mod);
	}
	return v;
}

void mem_copy(void* dest, void const* source, usize count);

void mem_copy_no_overlap(void* dest, void const* source, usize count);

void mem_set(void* dest, byte value, usize count);

isize mem_compare(void const* lhs, void const* rhs, usize count);

struct Allocator {
	/* Alloc `size` bytes aligned to `align`, returns null on failure */
	virtual void* alloc(usize size, usize align) = 0;
	/* Free pointer of `size` bytes aligned to `align`, returns false on failure */
	virtual bool free(void* p, usize size, usize align) = 0;
	/* Free all memory associated with allocator, returns false on failure */
	virtual bool free_all() = 0;
	/* Resize allocation of point p from `old_size` to `new_size` */
	virtual bool resize(void* p, usize old_size, usize new_size) = 0;

	/* Helper to allocate and construct one instance of type T */
	template<typename T> [[nodiscard]]
	T* create(){
		auto ptr = (T*)alloc(sizeof(T), alignof(T));
		if(ptr != nullptr){
			new (ptr) T();
		}
		return ptr;
	}

	/* Helper to deallocate and destroy one instance of type T */
	template<typename T>
	void destroy(T* p){
		if(p != nullptr){
			p->~T();
			this->free(p, sizeof(T), alignof(T));
		}
	}

	virtual ~Allocator() = default;
};

struct Arena : public Allocator {
	void* data;
	usize capacity;
	usize offset;
	void* last_allocation;

	void* alloc(usize size, usize align) override;
	bool free(void* p, usize size, usize align) override;
	bool free_all() override;
	bool resize(void* p, usize old_size, usize new_size) override;

	Arena() : data{nullptr}, capacity{0}, offset{0}, last_allocation{nullptr} {}
	Arena(Arena const&) = delete;

	Arena(Arena && a)
		: data{ exchange(a.data, nullptr) }
		, capacity{ exchange(a.capacity, 0) } {}

	static Arena from_buffer(Slice<u8> buf){
		Arena a;
		a.capacity = buf.len();
		a.data = buf.data();
		return a;
	}
};

struct Pool : public Allocator {
	void* data;
	usize capacity;
	usize block_size;

	void* alloc(usize size, usize align) override;
	bool free(void* p, usize size, usize align) override;
	bool free_all() override;
	bool resize(void* p, usize old_size, usize new_size) override;

	Pool() : data{nullptr}, capacity{0}, block_size{0} {}
};


}


