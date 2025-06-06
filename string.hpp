#pragma once
#include "ensure.hpp"
#include "types.hpp"

namespace mf {
struct String {
	u8 const* _data;
	usize _length;

	byte operator[](usize offset){
		bounds_check(offset < _length, "Out of bounds access to string");
		return _data[offset];
	}

	String take(usize n){
		bounds_check(n <= _length, "Cannot take more elements than slice has");
		return String(_data, n);
	}

	String skip(usize n){
		bounds_check(n <= _length, "Cannot skip more elements than slice has");
		return String(&_data[n], _length - n);
	}

	String slice(usize start, usize end){
		bounds_check(start <= end, "Slice range start cannot be greater than end");
		bounds_check(end <= _length, "Slice range goes beyond slice");
		return String(&_data[start], end - start);
	}

	mf_force_inline constexpr auto len() const { return _length; }
	mf_force_inline constexpr auto data() const { return _data; }
	mf_force_inline constexpr auto empty() const { return _length == 0; }

	String() : _data{nullptr}, _length{0} {}
	String(u8 const* p, usize n) : _data{p}, _length{n} {}
};
}

