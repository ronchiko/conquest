#include "conquest/data/profession.h"

namespace conquest {

	LoadResult Profession::loadFromObject(lang::Object& object) {
		LoadResult result;
		Profession profession;

		profession.name = std::move(object.name);
		for (auto& attribute : object.attributes) {
			if ("EQP" == attribute.first) {
				result.messages.add("WARNING: Equipment modification is not implemented.");
				continue; // Equipment modification, not supported yet.
			}

			result.guard([&] {
				const auto buff = object.attribute<int32>(attribute.first);
				const auto stat = getStatFromName(attribute.first);

				profession.buffs[stat] = buff;
			});
		}

		emplace(std::move(profession));
		return result;
	}
}
