#pragma once

#include <ranges>
#include <string>
#include <map>
#include <optional>

#include "conquest/data/storage/Singleton.h"


namespace conquest {

namespace detail {

template<typename ValueT, typename KeyT = std::string>
class Mapping
{
public:
	using Map_t = std::map<KeyT, std::optional<ValueT>>;

	explicit Mapping() = default;

	template<typename... Ts>
	void add(KeyT&& key, Ts&&... params) {
		m_Mapping[std::forward<KeyT>(key)] = ValueT(std::forward<Ts>(params)...);
	}

	const ValueT& get(const KeyT& key) const {
		return *m_Mapping.at(key);
	}

	auto names() const {
		return m_Mapping | std::views::keys;
	}

	auto all() const {
		// clang-format off
		return m_Mapping
			| std::views::values
			| std::views::filter([](auto& value) { return value.has_value(); })
			| std::views::transform([](auto& value) { return std::cref(value.value()); });
		// clang-format on
	}

private:
	Map_t m_Mapping;
};

}

#define CREATE_LOADABLE_ASSET_SINGLETON(type) \
	using type##Singleton = ::conquest::Singleton<::conquest::detail::Mapping<type>>; \

}
