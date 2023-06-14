#include "conquest/data/weapon_type.h"

namespace conquest {

	static const std::unordered_map<std::string, WeaponType::Category> CATEGORIES{
		{ "LIGHT", WeaponType::Category::Light },	   
		{ "PRIMARY", WeaponType::Category::Primary },
		{ "DUAL_HANDED", WeaponType::Category::Dual },
		{ "OFFHAND", WeaponType::Category::Offhand },
		{ "RANGED", WeaponType::Category::Ranged },
	};

	LoadResult WeaponType::loadFromObject(lang::Object& object)
	{
		LoadResult result;

		Category category;
		result.guard([&] {
			auto categoryName = std::move(object.attribute<std::string>("CATEGORY"));
			auto iterator = CATEGORIES.find(categoryName);
			if (CATEGORIES.end() == iterator) {
				throw ConquestError::make("No weapon category: ", categoryName);
			}

			category = iterator->second;
		});

		StatType damage = StatType::Strength, reduce = StatType::Defence;
		result.guard([&] {
			auto damageName = std::move(object.attribute<std::string>("DAMAGE"));
			damage = getStatFromName(damageName);
		});
		result.guard([&] {
			auto reduceName = std::move(object.attribute<std::string>("REDUCE"));
			reduce = getStatFromName(reduceName);
		});

		emplace(std::move(object.name), category, damage, reduce);
		return result;
	}
}
