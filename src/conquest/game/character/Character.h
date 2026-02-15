#pragma once

#include "conquest/game/character/CharacterAttributes.h"
#include "conquest/game/character/CharacterExperience.h"
#include "conquest/game/character/CharacterTraits.h"
#include "conquest/game/character/Stats.h"


namespace conquest {

class Character
{
public:
	explicit Character(CharacterTraits traits, CharacterAttributes attributes, const CharacterExperience &level);

	const CharacterTraits& traits() const;

	CharacterExperience& experience();
	const CharacterExperience& experience() const;

	CharacterAttributes& attributes();
	const CharacterAttributes& attributes() const;

private:
	/**
	   Re-Calculates the current stats of the character.
	 */
	void recacheStats();

	CharacterAttributes m_Attributes;
	CharacterTraits m_Traits;
	CharacterExperience m_Level;

	Stats<uint32_t> m_Cache;
};

}
