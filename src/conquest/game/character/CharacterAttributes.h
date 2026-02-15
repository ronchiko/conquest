#pragma once

#include <string>

#include "conquest/game/character/Stats.h"


namespace conquest {

class CharacterAttributes
{
public:
	explicit CharacterAttributes(std::string name, const Stats<int64_t>& distribution);

	/**
		The name of the character.
	 */
	[[nodiscard]] const std::string& name() const;

	/**
		The specialization stat distribution of the character.
	 */
	[[nodiscard]] Stats<int64_t>& specialization();
	[[nodiscard]] const Stats<int64_t>& specialization() const;

private:
	std::string m_Name;
	Stats<int64_t> m_Distribution;
};

}
