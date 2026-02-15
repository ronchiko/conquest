#include "conquest/game/character/traits/Race.h"

namespace conquest {

Race::Race(const toml::Toml& document)
	: m_Name(document["Name"].as<std::string>())
	, m_Description(document["Description"].as<std::string>())
	, m_Base(0ll)
	, m_Growth(0.f)
{
	for(const auto& [stat, name] : enumeration::iterateOver<StatType, std::string>()) {
		if(!document.has(name)) {
			continue;
		}

		m_Base[stat] = document[name]["base"].as<int64_t>();
		m_Growth[stat] = static_cast<float>(document[name]["growth"].as<double>());
	}
}

Race::Race(std::string name, std::string description, const Stats<int64_t>& base, const Stats<float>& growth)
	: m_Name(std::move(name))
	, m_Description(std::move(description))
	, m_Base(base)
	, m_Growth(growth)
{}

const std::string& Race::name() const
{
	return m_Name;
}

const std::string& Race::description() const
{
	return m_Description;
}

const Stats<int64_t>& Race::base() const
{
	return m_Base;
}

const Stats<float>& Race::growth() const
{
	return m_Growth;
}

}
