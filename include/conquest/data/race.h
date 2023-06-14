#pragma once

#include <string>

#include "conquest/data/statistics.h"
#include "conquest/data/load.h"
#include "conquest/lang/object.h"
#include "conquest/data/singleton.h"

namespace conquest {
	struct Race : public DataSingleton<Race>
	{
	public:
		std::string name;
		StatChart<int16> statModifiers;
		StatChart<float> statGrowth;

		static LoadResult loadFromObject(lang::Object& object);
	};
}
