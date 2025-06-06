#pragma once
#include "traits.hpp"
#include "ensure.hpp"

namespace mf {
template<typename T>
struct Option {
	union {
		T data_;
	};
	bool has_data_;

	[[nodiscard]]
	T unwrap(){
		if(!has_data_){
			panic("Cannot unwrap empty option");
		}
		T tmp = move(data_);
		has_data_ = false;
		return tmp;
	}

	[[nodiscard]]
	T or_else(T alt){
		if(!has_data_){
			return forward<T>(alt);
		}
		T tmp = move(data_);
		has_data_ = false;
		return tmp;
	}

	Option<T>* clear(){
		if(has_data_){
			data_.~T();
		}
		has_data_ = false;
		return this;
	}

	mf_force_inline constexpr auto ok() const { return has_data_; }

	Option() : has_data_{false} {}
	Option(T const& v) : has_data_{true} { new (&data_) T(v); }
	Option(T && v) : has_data_{true} { new (&data_) T(move(v)); }

	Option(Option<T> const& opt) : has_data_{opt.has_data_} {
		if(opt.has_data_){
			new (&data_) T(opt.data_);
		}
	}

	Option(Option<T> && opt) {
		has_data_ = exchange(opt.has_data_, false);
		if(has_data_){
			new (&data_) Option<T>{ mf::move(opt.data_) };
		}
	}

	auto& operator=(T const& v){
		return *(new (clear()) Option<T>{ v });
	}

	auto& operator=(T && v){
		return *(new (clear()) Option<T>{ mf::move(v) });
	}

	~Option(){
		clear();
	}
};

}

