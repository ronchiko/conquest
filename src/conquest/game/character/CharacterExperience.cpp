#include "conquest/game/character/CharacterExperience.h"

#include <valarray>

namespace conquest {

namespace {

uint64_t getRequiredExperienceFor(const uint16_t level, const CharacterExperienceRate rate)
{
	return static_cast<uint64_t>(level * std::pow(static_cast<uint64_t>(rate) + 3, 5));
}

}

CharacterExperience::CharacterExperience(const CharacterExperienceRate rate,
							   const uint16_t level,
							   const uint64_t experience)
	: m_Rate(rate)
	, m_Level(level)
	, m_Experience(experience)
	, m_RequiredExperience(getRequiredExperienceFor(level, rate))
{}

uint16_t CharacterExperience::level() const
{
	return m_Level;
}

uint64_t CharacterExperience::experience() const
{
	return m_Experience;
}

uint64_t CharacterExperience::requiredExperience() const
{
	return m_RequiredExperience;
}

void CharacterExperience::receiveExperience(const uint64_t amount)
{
	m_Experience += amount;
	while(m_Experience > m_RequiredExperience) {
		m_Experience -= m_RequiredExperience;
		++m_Level;

		m_RequiredExperience = getRequiredExperienceFor(m_Level, m_Rate);

		// TODO: Level up
	}
}

}
