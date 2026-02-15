#pragma once

#include <string>
#include <unordered_set>

#include "conquest/game/battle/WeaponType.h"
#include "conquest/game/character/Stats.h"


namespace conquest {

class Profession
{
public:
	explicit Profession(std::string name,
						std::string description,
						const Stats<int64_t>& base,
						const Stats<float>& growth,
						std::unordered_set<WeaponType> weapons);

	explicit Profession(const toml::Toml& document);

	[[nodiscard]] const std::string& name() const;
	[[nodiscard]] const std::string& description() const;

	[[nodiscard]] const Stats<int64_t>& base() const;
	[[nodiscard]] const Stats<float>& growth() const;

	[[nodiscard]] const std::unordered_set<WeaponType>& weapons() const;

private:
	std::string m_Name;
	std::string m_Description;

	Stats<int64_t> m_Base;
	Stats<float> m_Growth;

	std::unordered_set<WeaponType> m_AllowedWeapons;
};

CREATE_LOADABLE_ASSET_SINGLETON(Profession);

}
