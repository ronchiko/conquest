#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "conquest/error.h"
#include "conquest/types.h"

#include "conquest/traits/variant.h"

namespace conquest::lang {

	struct Attribute
	{
	public:
		using BaseAttribute = std::variant<int32, float, std::string>;

		explicit Attribute(std::vector<BaseAttribute> values)
			: mValues(std::move(values))
		{}

		/**
		   Acquires the types from this attribute as a tuple.
		 */
		template<typename... Values>
		std::tuple<Values...> acquire() const
		{
			static_assert((is_variant_alternate_v<Values, BaseAttribute> && ...),
						  "Not all types are possible alternates");

			if(sizeof...(Values) != mValues.size()) {
				throw ConquestError("Attempted to acquire invalid types.");
			}

			// We need to pass the internal acquire implementation the type of the output tuple, and
			// since we cant pass it down explicitly (because we cant pass the indecies), we have to
			// let the compiler infer its type. We do this by passing down a pointer to the output type
			// and because that pointer wont be null, we just pass null as that pointer.
			return acquire(reinterpret_cast<std::tuple<Values...> *>(nullptr), std::index_sequence_for<Values...>{});
		}

	private:
		template<typename OutType, size_t... Is>
		OutType acquire([[maybe_unused]] OutType *, std::index_sequence<Is...> sequence) const
		{
			try {
				return std::make_tuple(std::get<std::tuple_element_t<Is, OutType>>(mValues[Is])...);
			} catch(const std::bad_variant_access& err) {
				// Proxy the error as a conquest error.
				throw ConquestError(err.what());
			}
		}

		std::vector<BaseAttribute> mValues;
	};

	struct Object
	{
	public:
		enum class Type
		{
			Element,
			Equipment,
			Race,
			WeaponType,
			Weapon,
			Item,
			Profession,
			Armor,
			Move
		};

		Type type;
		std::string name;
		Object *parent = nullptr;

		std::vector<std::string> tags;
		std::unordered_map<std::string, Attribute> attributes;

		template<typename... Values>
		using AttributeT = std::conditional_t<sizeof...(Values) == 1,
											  std::tuple_element_t<0, std::tuple<Values...>>,
											  std::tuple<Values...>>;

		template<typename... Values>
		AttributeT<Values...> attribute(const std::string& attrib)
		{
			static_assert(sizeof...(Values) >= 1, "Expected at least one value.");

			auto iterator = attributes.find(attrib);
			if(attributes.end() == iterator) {
				if(nullptr == parent) {
					throw ConquestError::make("Object '", name, "' has no such attribute: ", attrib);
				}

				return parent->attribute<Values...>(attrib);
			}

			try {
				if constexpr(sizeof...(Values) == 1) {
					// If we have one value, the just return it.
					return std::get<0>(iterator->second.acquire<Values...>());
				} else {
					return iterator->second.acquire<Values...>();
				}
			} catch(const ConquestError&) {
				throw ConquestError::make("The attribute ", attrib, " doesnt match the type ", makeTypeList());
			}
		}

	private:
		template<typename... List>
		std::string makeTypeList()
		{
			std::stringstream stream;

			if constexpr (sizeof...(List) > 1) {
				stream << "{";
			}

			(stream << ... << typeid(List).name());

			if constexpr(sizeof...(List) > 1) {
				stream << "}";
			}
			return stream.str();
		}
	};
}
