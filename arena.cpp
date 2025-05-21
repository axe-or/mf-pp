#include "memory.hpp"

namespace mf {

void* Arena::alloc(usize size, usize align){
	uintptr base = (uintptr)data;
	uintptr current = base + (uintptr)offset;

	size_t available = capacity - (current - base);

	uintptr aligned  = align_forward<uintptr>(current, align);
	uintptr padding  = aligned - current;
	uintptr required = padding + size;

	if(required > available){
		return nullptr; /* Out of memory */
	}

	offset += required;
	void* allocation = (void*)aligned;
	mem_set(allocation, 0, size);

	last_allocation = allocation;

	return allocation;
}

bool Arena::free(void* p, usize, usize){
	if(p == last_allocation){
		offset = (uintptr)last_allocation - (uintptr)data;
		return true;
	}
	else {
		return false;
	}
}

bool Arena::free_all(){
	offset = 0;
	last_allocation = nullptr;
	return true;
}

bool Arena::resize(void* ptr, usize, usize new_size){
	uintptr base    = (uintptr)data;
	uintptr current = base + (uintptr)offset;
	uintptr limit   = base + capacity;

	ensure((uintptr)ptr >= base && (uintptr)ptr < limit, "Pointer is not owned by arena");

	if(ptr == last_allocation){
		size_t last_allocation_size = current - (uintptr)last_allocation;
		if(((current - last_allocation_size) + new_size) > limit){
			return false; /* No space left */
		}

		offset += new_size - last_allocation_size;
		return true;
	}

	return false;
}

}
