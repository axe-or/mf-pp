#pragma once

#include "types.hpp"
#include "ensure.hpp"

namespace mf {
template<typename T>
struct Slice {
	T* _data;
	usize _length;

	T& operator[](usize idx){
		bounds_check(idx < _length, "Index to slice is out of range");
		return _data[idx];
	}

	T const& operator[](usize idx) const {
		bounds_check(idx < _length, "Index to slice is out of range");
		return _data[idx];
	}

	Slice<T> take(usize n){
		bounds_check(n <= _length, "Cannot take more elements than slice has");
		return Slice<T>(_data, n);
	}

	Slice<T> skip(usize n){
		bounds_check(n <= _length, "Cannot skip more elements than slice has");
		return Slice<T>(&_data[n], _length - n);
	}

	Slice<T> slice(usize start, usize end){
		bounds_check(start <= end, "Slice range start cannot be greater than end");
		bounds_check(end <= _length, "Slice range goes beyond slice");
		return Slice<T>(&_data[start], end - start);
	}

	// Option<T> at()

	mf_force_inline constexpr auto empty() const { return _length == 0; }
	mf_force_inline constexpr auto len() const { return _length; }
	mf_force_inline constexpr auto data() const { return _data; }

	Slice() : _data{nullptr}, _length{0} {}
	Slice(T* p, usize n) : _data{p}, _length{n} {}
};
}

