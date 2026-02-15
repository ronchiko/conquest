#pragma once

#include "conquest/data/storage/AssetSingleton.h"
#include "conquest/game/character/Stats.h"
#include "conquest/data/Toml.h"


namespace conquest {

class Race
{
public:
	explicit Race(const toml::Toml& document);
	explicit Race(std::string name, std::string description, const Stats<int64_t>& base, const Stats<float>& growth);

	[[nodiscard]] const std::string& name() const;
	[[nodiscard]] const std::string& description() const;

	[[nodiscard]] const Stats<int64_t>& base() const;
	[[nodiscard]] const Stats<float>& growth() const;

private:
	std::string m_Name;
	std::string m_Description;

	Stats<int64_t> m_Base;
	Stats<float> m_Growth;
};

CREATE_LOADABLE_ASSET_SINGLETON(Race);

}
