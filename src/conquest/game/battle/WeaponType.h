#pragma once

#include "conquest/data/Toml.h"
#include "conquest/data/storage/AssetSingleton.h"


namespace conquest {

/**
	Describes the basic common characteristics of a weapon.
 */
class WeaponType
{
public:
	explicit WeaponType() = default;

	explicit WeaponType(std::string name, bool dualHanded);
	explicit WeaponType(const toml::Toml& table);
	
	/**
		The name of this weapon type.
	 */
	[[nodiscard]] const std::string& name() const;

	/**
		Do weapons of this type requires both hands to be used?
	 */
	[[nodiscard]] bool isDualHanded() const;

	bool operator==(const WeaponType& other) const;

private:
	std::string m_Name;
	bool m_DualHanded;
};

CREATE_LOADABLE_ASSET_SINGLETON(WeaponType);

}

template<>
struct std::hash<conquest::WeaponType>
{
	size_t operator()(const conquest::WeaponType& weaponType) const noexcept
	{
		return std::hash<std::string>()(weaponType.name());
	}
};