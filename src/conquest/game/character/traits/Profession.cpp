#include "conquest/game/character/traits/Profession.h"


namespace conquest {

Profession::Profession(const toml::Toml& document)
	: m_Name(document["Name"].as<std::string>())
	, m_Description(document["Description"].as<std::string>())
	, m_Base(0ll)
	, m_Growth(1.f)
{
	for(const auto& [stat, name] : enumeration::iterateOver<StatType, std::string>()) {
		if(!document.has(name)) {
			continue;
		}

		m_Base[stat] = document[name]["base"].as<int64_t>();
		m_Growth[stat] = static_cast<float>(document[name]["growth"].as<double>());
	}

	WeaponTypeSingleton weaponTypes;
	for(const auto& node : document["Weapons"].as<::toml::array>()) {
		const auto weaponType = node.as_string();
		if(nullptr == weaponType) {
			throw ConquestError("Weapons types must be a string");
		}

		m_AllowedWeapons.insert(weaponTypes->get(weaponType->get()));
	}
}

Profession::Profession(std::string name,
					   std::string description,
					   const Stats<int64_t>& base,
					   const Stats<float>& growth,
					   std::unordered_set<WeaponType> weapons)
	: m_Name(std::move(name))
	, m_Description(std::move(description))
	, m_Base(base)
	, m_Growth(growth)
	, m_AllowedWeapons(std::move(weapons))
{}

const std::string& Profession::name() const
{
	return m_Name;
}

const std::string& Profession::description() const
{
	return m_Description;
}

const Stats<int64_t>& Profession::base() const
{
	return m_Base;
}

const Stats<float>& Profession::growth() const
{
	return m_Growth;
}

const std::unordered_set<WeaponType>& Profession::weapons() const
{
	return m_AllowedWeapons;
}

}
