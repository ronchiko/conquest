#pragma once

#include <cstdint>

namespace conquest {

enum class CharacterExperienceRate
{
	Fast,
	Normal,
	Slow,
};

class CharacterExperience
{
public:
	explicit CharacterExperience(
		CharacterExperienceRate rate,
		uint16_t level,
		uint64_t experience = 0
	);

	[[nodiscard]] uint16_t level() const;
	[[nodiscard]] uint64_t experience() const;
	[[nodiscard]] uint64_t requiredExperience() const;

	/**
		Adds experience to this character.
	 */
	void receiveExperience(uint64_t amount);

private:
	CharacterExperienceRate m_Rate;
	uint16_t m_Level;

	uint64_t m_Experience;
	uint64_t m_RequiredExperience;
};

}
