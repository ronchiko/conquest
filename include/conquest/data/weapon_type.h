#pragma once

#include "conquest/data/load.h"
#include "conquest/data/singleton.h"
#include "conquest/data/statistics.h"
#include "conquest/lang/object.h"

namespace conquest {

	struct WeaponType : public DataSingleton<WeaponType>
	{
	public:
		enum class Category
		{
			Light,
			Primary,
			Offhand,
			Dual,
			Ranged
		};

		std::string name;
		Category category;
		StatType target; // The stat that this weapon uses to compute damage
		StatType reduce; // The opposing stat this weapon uses to reduce the targeted damage.

		/**
		   Loads a weapon type from an object.
		 */
		static LoadResult loadFromObject(lang::Object& object);		
	};
}
