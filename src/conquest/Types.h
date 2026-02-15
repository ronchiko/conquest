#pragma once

#include <cstdint>

namespace conquest {
using byte = unsigned char; // NOLINT

using int16 = std::int16_t;	  // NOLINT
using uint16 = std::uint16_t; // NOLINT

using int32 = std::int32_t;	  // NOLINT
using uint32 = std::uint32_t; // NOLINT

template<typename StoredT>
struct v2
{
	StoredT x;
	StoredT y;

	constexpr v2 operator+(const v2& other) const
	{
		return { x + other.x, y + other.y };
	}

	constexpr v2 operator-(const v2& other) const
	{
		return { x - other.x, y - other.y };
	}

	constexpr v2 operator*(const StoredT value) const
	{
		return { x * value, y * value };
	}

	constexpr v2 operator/(const StoredT value) const
	{
		return { x / value, y / value };
	}
};

template<typename... Ts>
struct Overload : Ts...
{
	using Ts::operator()...;
};

template<typename... Ts>
Overload(Ts...) -> Overload<Ts...>;
}

template<typename IntegralT>
constexpr bool operator==(const conquest::v2<IntegralT>& lhs, const conquest::v2<IntegralT>& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<typename IntegralT>
constexpr bool operator!=(const conquest::v2<IntegralT>& lhs, const conquest::v2<IntegralT>& rhs)
{
	return lhs.x != rhs.x && lhs.y != rhs.y;
}