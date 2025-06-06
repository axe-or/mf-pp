#pragma once
#include "traits.hpp"
#include "ensure.hpp"

namespace mf {
template<typename Val, typename Err>
struct Result {
	union {
		Val value_;
		Err error_;
	};
	bool has_value_;

	[[nodiscard]]
	Val unwrap(){
		if(!has_value_){
			panic("Cannot unwrap error result");
		}
		Val tmp = move(value_);
		has_value_ = false;
		error_ = Err();
		return tmp;
	}

	[[nodiscard]]
	Val or_else(Val alt){
		if(!has_value_){
			return forward<Val>(alt);
		}
		Val tmp = move(has_value_);
		has_value_ = false;
		return tmp;
	}

	Result<Val, Err>* clear(){
		if(has_value_){
			value_.~Val();
		}
		has_value_ = false;
		return this;
	}

	Result() : error_{}, has_value_{false} {}
	Result(Err const& e) : error_{e}, has_value_{false} {}

	Result(Val const& v) : value_{v}, has_value_{true} {}
	Result(Val && v) : value_{move(v)}, has_value_{true} {}

	Result(Result<Val, Err> const& r) : has_value_{r.has_value_} {
		if(has_value_){
			new (&value_) Val(r.value_);
		} else {
			new (&error_) Err(r.error_);
		}
	}

	Result(Result<Val, Err>&& r){
		has_value_ = exchange((r.has_value_), false);
		if(has_value_){
			new (&value_) Val(move(r.value_));
		} else {
			new (&error_) Err(r.error_);
		}
	}

	auto& operator=(Result<Val, Err> const& res){
		return *(new (clear()) Result<Val, Err>{ res });
	}

	auto& operator=(Result<Val, Err> && res){
		return *(new (clear()) Result<Val, Err>{ move(res) });
	}

	auto& operator=(Val const& v){
		auto p = *(new (clear()) Result<Val, Err>{ v });
		has_value_ = true;
		return *p;
	}

	auto& operator=(Val&& v){
		auto p = *(new (clear()) Result<Val, Err>{ move(v) });
		has_value_ = false;
		return *p;
	}

	auto& operator=(Err const& e){
		auto p = *(new (clear()) Result<Val, Err>{ e });
		has_value_ = false;
		return *p;
	}

	mf_force_inline constexpr auto ok() const { return has_value_; }

	~Result(){
		clear();
	}
};
}
