#include "conquest/game/character/CharacterAttributes.h"

namespace conquest {

CharacterAttributes::CharacterAttributes(std::string name, const Stats<int64_t>& distribution)
	: m_Name(std::move(name))
	, m_Distribution(distribution)
{}

const std::string& CharacterAttributes::name() const
{
	return m_Name;
}

Stats<int64_t>& CharacterAttributes::specialization()
{
	return m_Distribution;
}

const Stats<int64_t>& CharacterAttributes::specialization() const
{
	return m_Distribution;
}


}