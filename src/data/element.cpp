#include "conquest/data/element.h"

#include "conquest/error.h"

inline constexpr float DEFAULT_MATCHUP = 1.f;

namespace conquest {

	LoadResult Element::loadFromObject(lang::Object& object)
	{
		LoadResult result;

		std::string color;
		result.guard([&] {
			color = object.attribute<std::string>("COLOR");
			object.attributes.erase("COLOR");
		});

		std::unordered_map<std::string, float> matchups;
		for(auto& attribute : object.attributes) {
			result.guard([&] {
				const auto& name = attribute.first;

				const auto multiplier = object.attribute<float>(name);
				matchups.emplace(name, multiplier);
			});
		}
		
		emplace(std::move(object.name), getColorByName(color), std::move(matchups));
		return result;
	}

	float Element::against(const Element& other) const {
		auto iterator = matchups.find(other.name);
		if (matchups.end() == iterator) {
			return DEFAULT_MATCHUP;
		}

		return iterator->second;
	}
}
