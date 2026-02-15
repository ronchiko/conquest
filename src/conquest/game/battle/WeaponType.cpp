#include "conquest/game/battle/WeaponType.h"

namespace conquest {

WeaponType::WeaponType(std::string name, const bool dualHanded)
	: m_Name(std::move(name))
	, m_DualHanded(dualHanded)
{}

WeaponType::WeaponType(const toml::Toml& table)
	: m_Name(table["Name"].as<std::string>())
	, m_DualHanded(table["DualHanded"].as<bool>())

{}

const std::string& WeaponType::name() const
{
	return m_Name;
}

bool WeaponType::isDualHanded() const
{
	return m_DualHanded;
}

bool WeaponType::operator==(const WeaponType& other) const
{
	return m_Name == other.m_Name && m_DualHanded == other.m_DualHanded;
}

}
