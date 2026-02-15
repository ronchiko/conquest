#pragma once

#include <unordered_map>

#include "conquest/Error.h"


namespace conquest::enumeration {

namespace detail {

template<typename Key, typename Value>
struct EnumLookup;

template<typename K, typename V>
auto inverseMapItems(const std::unordered_map<K, V>& other)
{
	std::unordered_map<V, K> inverse;
	inverse.reserve(other.size());

	for(const auto& [key, value] : other) {
		inverse[value] = key;
	}

	return inverse;
}

template<typename T>
struct LookupIterable
{
	using type = std::add_const_t<decltype(T::g_Map)>;

	static auto begin() -> decltype(std::declval<type>().begin())
	{
		return static_cast<type&>(T::g_Map).begin();
	}

	static auto end() -> decltype(std::declval<type>().end())
	{
		return static_cast<type&>(T::g_Map).end();
	}
};

template<typename T>
auto castToPrintable(const T& value)
{
	if constexpr(std::is_enum_v<T>) {
		return std::underlying_type_t<T>(value);
	} else {
		return value;
	}
}

}

template<typename ValueT, typename KeyT>
const ValueT& lookupEnum(const KeyT& key)
{
	using LookupT = detail::EnumLookup<KeyT, ValueT>;

	auto iterator = LookupT::g_Map.find(key);
	if(LookupT::g_Map.end() == iterator) {
		throw ConquestError::make("Failed to lookup ", detail::castToPrintable(key), "as a ", LookupT::KEY_NAME);
	}

	return iterator->second;
}

template<typename K, typename V>
auto iterateOver() -> detail::LookupIterable<detail::EnumLookup<K, V>>
{
	return detail::LookupIterable<detail::EnumLookup<K, V>>{};
}

template<typename T, typename V, typename = void>
struct is_lookup_pair : std::false_type
{};

template<typename T, typename V>
struct is_lookup_pair<T, V, std::void_t<decltype(detail::EnumLookup<T, V>::g_Map)>> : std::true_type
{};

template<typename T, typename V>
constexpr bool is_lookup_pair_v = is_lookup_pair<T, V>::value;
}


#define ENUM_LOOKUP(keyType, valueType, ...)                                                                           \
	template<>                                                                                                         \
	struct ::conquest::enumeration::detail::EnumLookup<keyType, valueType>                                                          \
	{                                                                                                                  \
		inline static auto KEY_NAME = #keyType;                                                                        \
		inline static auto VALUE_NAME = #valueType;                                                                    \
		inline static std::unordered_map<keyType, valueType> g_Map = __VA_ARGS__;                                      \
	};

#define ENUM_BI_LOOKUP(keyType, valueType, ...)                                                                        \
	ENUM_LOOKUP(keyType, valueType, __VA_ARGS__);                                                                      \
	template<>                                                                                                         \
	struct ::conquest::enumeration::detail::EnumLookup<valueType, keyType>                                                          \
	{                                                                                                                  \
		inline static auto KEY_NAME = #keyType;                                                                        \
		inline static auto VALUE_NAME = #valueType;                                                                    \
		inline static auto g_Map = inverseMapItems(EnumLookup<keyType, valueType>::g_Map);                             \
	};