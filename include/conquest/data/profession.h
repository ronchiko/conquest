#pragma once

#include <string>
#include <unordered_map>

#include "conquest/data/load.h"
#include "conquest/data/singleton.h"
#include "conquest/data/statistics.h"
#include "conquest/lang/object.h"

namespace conquest {

	struct Profession : public DataSingleton<Profession>
	{
	public:
		std::string name;
		StatChart<int32> buffs;

		static LoadResult loadFromObject(lang::Object& object);
	};
}
