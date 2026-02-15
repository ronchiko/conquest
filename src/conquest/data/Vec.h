#pragma once

#include <concepts>


namespace conquest {

template<std::integral T>
struct Vec2 {
	T x;
	T y;

	constexpr Vec2(T x, T y) : x(x), y(y) {}
};

}
