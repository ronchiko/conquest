#pragma once

#include <cstdint>
#include <algorithm>
#include <functional>

#include "conquest/Enum.h"


namespace conquest {

enum class StatType
{
	Health,
	Energy,
	Fame,
	Power,
	Fortitude,
	Intellect,
	Resilience,
	Agility,
	Stealth,
	Fortune,
};

static constexpr auto STAT_COUNT = static_cast<uint8_t>(StatType::Fortune) + 1;

template<typename T>
class Stats
{
public:

	explicit Stats() = default;
	explicit Stats(T base);
	explicit Stats(T values[STAT_COUNT]);

// clang-format off
#define DEFINE_ACCESSOR(name, stat) \
	constexpr T& name() { return operator[](stat); } \
	constexpr const T& name() const { return operator[](stat); } \
// clang-format on

	DEFINE_ACCESSOR(health, StatType::Health);
	DEFINE_ACCESSOR(energy, StatType::Energy);
	DEFINE_ACCESSOR(fame, StatType::Fame);
	DEFINE_ACCESSOR(power, StatType::Power);
	DEFINE_ACCESSOR(fortitude, StatType::Fortitude);
	DEFINE_ACCESSOR(intellect, StatType::Intellect);
	DEFINE_ACCESSOR(resilience, StatType::Resilience);
	DEFINE_ACCESSOR(agility, StatType::Agility);
	DEFINE_ACCESSOR(stealth, StatType::Stealth);
	DEFINE_ACCESSOR(fortune, StatType::Fortune);

#undef DEFINE_ACCESSOR

	constexpr T& operator[](StatType stat);
	constexpr const T& operator[](StatType stat) const;

private:
	T m_Values[STAT_COUNT];
};

template <typename T>
Stats<T>::Stats(T base)
{
	std::generate(std::begin(m_Values), std::end(m_Values), [=] { return base; });
}

template <typename T>
Stats<T>::Stats(T values[STAT_COUNT])
	: m_Values{values}
{}

template <typename T>
constexpr T& Stats<T>::operator[](StatType stat)
{
	return m_Values[static_cast<uint8_t>(stat)];
}

template <typename T>
constexpr const T& Stats<T>::operator[](StatType stat) const
{
	return m_Values[static_cast<uint8_t>(stat)];
}

constexpr void foreachStat(const std::function<void(StatType)>& action)
{
	for (auto i = 0; i < STAT_COUNT; ++i) {
		action(static_cast<StatType>(i));
	}
}

template<typename E, typename U>
constexpr Stats<E> cast(const Stats<U>& original)
{
	Stats<E> result;
	foreachStat([&](auto name) { result[name] = static_cast<E>(original[name]); });

	return result;
} 

constexpr std::string_view toShortName(const StatType value)
{
	// clang-format off
	switch (value) {
		case StatType::Health: return "HP";
		case StatType::Energy: return "ENRG";
		case StatType::Fame:   return "FAME";
		case StatType::Power:  return "PWR";
		case StatType::Fortitude: return "FRT";
		case StatType::Intellect: return "INT";
		case StatType::Resilience: return "RES";
		case StatType::Agility: return "AGI";
		case StatType::Stealth: return "STLH";
		case StatType::Fortune: return "FRTN";
	}
	// clang-format on

	throw ConquestError::make("No such stat type: ", static_cast<int>(value));
}

}

template<typename E, typename U, typename R = decltype(E() * U())>
conquest::Stats<R> operator+(const conquest::Stats<E>& lhs, const conquest::Stats<U>& rhs)
{
	conquest::Stats<R> result;
	conquest::foreachStat([&](auto name) { result[name] = static_cast<R>(lhs[name] + rhs[name]); });

	return result;
} 

template<typename E, typename U, typename R = decltype(E() * U())>
conquest::Stats<R> operator*(const conquest::Stats<E>& lhs, const conquest::Stats<U>& rhs)
{
	conquest::Stats<R> result;
	conquest::foreachStat([&](auto name) { result[name] = static_cast<R>(lhs[name] * rhs[name]); });

	return result;
} 


template<typename E, typename U, typename R = decltype(E() * U())>
conquest::Stats<R> operator*(const conquest::Stats<E>& lhs, const U rhs)
{
	conquest::Stats<R> result;
	conquest::foreachStat([&](auto name) { result[name] = static_cast<R>(lhs[name] * rhs); });

	return result;
} 

ENUM_BI_LOOKUP(std::string, conquest::StatType, {
	{"Health", conquest::StatType::Health},
	{"Energy", conquest::StatType::Energy},
	{"Fame", conquest::StatType::Fame},
	{"Power", conquest::StatType::Power},
	{"Fortitude", conquest::StatType::Fortitude},
	{"Intellect", conquest::StatType::Intellect},
	{"Resilience", conquest::StatType::Resilience},
	{"Agility", conquest::StatType::Agility},
	{"Stealth", conquest::StatType::Stealth},
	{"Fortune", conquest::StatType::Fortune},
});
