#pragma once
#include "build_context.hpp"

namespace mf {

/* Remove reference */
namespace detail {
template<typename T> struct RemoveReference      { using Type = T; };
template<typename T> struct RemoveReference<T&>  { using Type = T; };
template<typename T> struct RemoveReference<T&&> { using Type = T; };
}

template<typename T>
using RemoveReference = typename detail::RemoveReference<T>::Type;

/* Type equality */
template<typename A, typename B>
constexpr auto is_same = false;

template<typename T>
constexpr auto is_same<T, T> = true;

template<typename T>
constexpr T&& forward(RemoveReference<T>&& arg) {
	return static_cast<T&&>(arg);
}

template<typename T>
constexpr T&& forward(RemoveReference<T>& arg) {
	return static_cast<T&&>(arg);
}

template<typename T>
mf_force_inline constexpr RemoveReference<T>&& move(T&& arg) {
	return static_cast<RemoveReference<T>&&>(arg);
}

template<typename T, typename U = T>
mf_force_inline constexpr T exchange(T& obj, U&& new_value) {
	T old_value = move(obj);
	obj = forward<U>(new_value);
	return old_value;
}

}

