#pragma once

#include <string>

#include "conquest/data/load.h"
#include "conquest/data/singleton.h"
#include "conquest/graphics/types.h"
#include "conquest/lang/object.h"

namespace conquest {
	struct Element : public DataSingleton<Element>
	{
	public:
		std::string name;
		Color color;
		std::unordered_map<std::string, float> matchups;

		/**
		   Loads an element from an object.
		 */
		static LoadResult loadFromObject(lang::Object& object);

		/**
		   Gets the elemental matchup against anther element.

		   \param other: The element to matchup against.
		 */
		float against(const Element& other) const;
	};
}
