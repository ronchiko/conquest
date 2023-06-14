#include "conquest/data/race.h"

namespace conquest {

	LoadResult Race::loadFromObject(lang::Object& object)
	{
		LoadResult result;

		Race race;

		for(const auto& pair : object.attributes) {
			result.guard([&] {
				const auto stat = getStatFromName(pair.first);
				const auto [modifier, growth] = object.attribute<int32, float>(pair.first);

				race.statGrowth[stat] = growth;
				race.statModifiers[stat] = modifier;
			});
		}

		race.name = std::move(object.name);

		emplace(std::move(race));
		return result;
	}
}
