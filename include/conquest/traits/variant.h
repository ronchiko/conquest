#pragma once

#include <type_traits>
#include <variant>

namespace conquest {

	template<typename TypeT, typename VariantT>
	struct is_variant_alternate : std::false_type
	{};

	template<typename TypeT, typename... AlternatesTs>
	struct is_variant_alternate<TypeT, std::variant<AlternatesTs...>>
		: std::disjunction<std::is_same<TypeT, AlternatesTs>...>
	{};

	template<typename TypeT, typename VariantT>
	constexpr bool is_variant_alternate_v = is_variant_alternate<TypeT, VariantT>::value;
}
