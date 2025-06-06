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

void* Pool::alloc_block(){
	auto p = this;
	PoolNode* node = p->head;
	if(node == nullptr){
		return nullptr;
	}

	p->head = p->head->next;

	mem_set(node, 0, p->block_size);
	return (void*)node;
}

void* Pool::alloc(usize size, usize align){
	if(size > block_size || align == 0 || align > block_align){
		return nullptr;
	}
	return this->alloc_block();
}

bool Pool::free(void* ptr, usize, usize){
	if(ptr == nullptr){ return false; }
	auto p = this;

	void* start = p->data;
	void* end = &p->data[p->capacity];

	if(ptr < start || ptr >= end){ /* Out of bounds free, this check is always perfomed */
		return false;
	}

	PoolNode* node = (PoolNode*)ptr;
	node->next = p->head;
	p->head = node;
	return true;
}

bool Pool::free_all(){
	usize block_count = capacity / block_size;

	for(usize i = 0; i < block_count; i++){
		void* ptr =  &data[i * block_size];
		PoolNode* node = (PoolNode*)ptr;
		node->next = head;
		head = node;
	}
	return true;
}

bool Pool::resize(void* ptr, usize, usize new_size){
	if(ptr == nullptr){ return false; }

	void* start = this->data;
	void* end = &this->data[this->capacity];

	if(ptr < start || ptr >= end){ /* Out of bounds resize, this check is always perfomed */
		return false;
	}

	if(new_size > block_size){
		return false;
	}

	return true;
}

Option<Pool> Pool::init(Slice<byte> buf, usize block_size, usize block_alignment){
	Pool p;
	usize buf_size = buf.len();
	uintptr original_start = (uintptr)buf.data();
	uintptr aligned_start = align_forward(original_start, block_alignment);

	buf_size -=  (usize)(aligned_start - original_start);

	block_size = align_forward(block_size, block_alignment);

	if(block_size < sizeof(PoolNode)){
		/* Block size is too small */
		return {};
	}
	if(block_size >= buf_size){
		/* Backing buffer cannot contain one block */
		return {};
	}

	p.data = buf.data();
	p.capacity = buf_size;
	p.block_size = block_size;
	p.block_align = block_alignment;
	p.head = nullptr;

	p.free_all();
	return p;
}
}
