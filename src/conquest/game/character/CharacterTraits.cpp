#include "conquest/game/character/CharacterTraits.h"

namespace conquest {

CharacterTraits::CharacterTraits(const Race& race, const Profession& profession)
	: m_Race(std::cref(race))
	, m_Profession(std::cref(profession))
{}

const Profession& CharacterTraits::profession() const
{
	return m_Profession.get();
}

const Race& CharacterTraits::race() const
{
	return m_Race.get();
}

Stats<float> CharacterTraits::growth() const
{
	return profession().growth() * race().growth();
}

Stats<int64_t> CharacterTraits::base() const
{
	return profession().base() + race().base();
}

}
