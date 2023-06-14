#include "conquest/data/statistics.h"

#include <unordered_map>

#include "conquest/error.h"

namespace conquest {

	static const std::unordered_map<std::string, StatType> STATS = { 
		{ "HP", StatType::Hp },
		{ "ENRG", StatType::Energy },
		{ "FAME", StatType::Fame },
		{ "CHRS", StatType::Charisma },
		{ "STRG", StatType::Strength },
		{ "DEFC", StatType::Defence },
		{ "INT", StatType::Intellect },
		{ "RES", StatType::Resistance },
		{ "SPD", StatType::Speed },
		{ "STLH", StatType::Stealth },
		{ "LUCK", StatType::Luck },
	};

	StatType getStatFromName(const std::string& name) {
		auto iterator = STATS.find(name);
		if (STATS.end() == iterator) {
			throw ConquestError::make("Not stat named: ", name);
		}

		return iterator->second;
	}
}
