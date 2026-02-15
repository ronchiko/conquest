#include "conquest/game/character/Character.h"

namespace conquest {

Character::Character(const CharacterTraits traits, CharacterAttributes attributes, const CharacterExperience& level)
	: m_Attributes(std::move(attributes))
    , m_Traits(traits)
    , m_Level(level)
    , m_Cache()
{}


CharacterAttributes& Character::attributes()
{
	return m_Attributes;
}

const CharacterAttributes& Character::attributes() const
{
	return m_Attributes;
}

CharacterExperience& Character::experience()
{
	return m_Level;
}

const CharacterExperience& Character::experience() const
{
	return m_Level;
}

const CharacterTraits& Character::traits() const
{
	return m_Traits;
}

void Character::recacheStats()
{
	const auto base = traits().base() + attributes().specialization();
	m_Cache = cast<uint32_t>(base + traits().growth() * experience().level());
}

}