#pragma once
#include <type_traits>

namespace conquest {

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
class Flag
{
public:
	using Underlying_t = std::underlying_type_t<T>;

	constexpr Flag() = default;

	constexpr Flag(T value)
		: m_Value(static_cast<Underlying_t>(value))
	{}

	constexpr Flag operator|(T other) const
	{
		return Flag(*this | Flag(other));
	}

	constexpr Flag operator|(Flag other) const
	{
		return Flag(m_Value | other.m_Value);
	}

	constexpr bool operator&(Flag other) const
	{
		return m_Value & other.m_Value;
	}

	constexpr bool operator&(T value) const
	{
		return *this & Flag(value);
	}

private:
	explicit Flag(Underlying_t value)
		: m_Value(value)
	{}

	Underlying_t m_Value;
};

}
