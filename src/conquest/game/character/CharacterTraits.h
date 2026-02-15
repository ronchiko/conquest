#pragma once

#include "conquest/game/character/traits/Race.h"
#include "conquest/game/character/traits/Profession.h"


namespace conquest {

class CharacterTraits
{
public:
	explicit CharacterTraits(const Race& race, const Profession& profession);

	[[nodiscard]] const Profession& profession() const;
	[[nodiscard]] const Race& race() const;

	[[nodiscard]] Stats<float> growth() const;
	[[nodiscard]] Stats<int64_t> base() const; 

private:
	std::reference_wrapper<const Race> m_Race;
	std::reference_wrapper<const Profession> m_Profession;
};

}
