#ifdef false

#include <map>

#include "assets.h"
#include "parsing.h"
#include "weapons.h"

static Weapon::Template& getTemplate(WeaponType_e type);
static std::map<WeaponType_e, Weapon::Template> gWeaponTemplates = {
	{ WEAPON_AXE, Weapon::Template::make(STAT_STRENGTH, STAT_DEFENCE) },
	{ WEAPON_SWORD, Weapon::Template::make(STAT_STRENGTH, STAT_DEFENCE) },
	{ WEAPON_ORB, Weapon::Template::make(STAT_INTELLECT, STAT_RESISTANCE) },
	{ WEAPON_STAFF, Weapon::Template::make(STAT_INTELLECT, STAT_RESISTANCE) },
	{ WEAPON_SPEAR, Weapon::Template::make(STAT_STRENGTH, STAT_SPEED) },
	{ WEAPON_SHIELD, Weapon::Template::make(STAT_STRENGTH, STAT_DEFENCE) },
	{ WEAPON_SCYTH, Weapon::Template::make(STAT_INTELLECT, STAT_DEFENCE) },
	{ WEAPON_DAGGER, Weapon::Template::make(STAT_STEALTH, STAT_SPEED) },
	{ WEAPON_BOW, Weapon::Template::make(STAT_STEALTH, STAT_DEFENCE) },
	{ WEAPON_CROSSBOW, Weapon::Template::make(STAT_STRENGTH, STAT_DEFENCE) },
	{ WEAPON_MUSKET, Weapon::Template::make(STAT_SPEED, STAT_DEFENCE) }
};
static Weapon::Template TEMPLATE_DEFAULT = Weapon::Template::make(STAT_STRENGTH, STAT_DEFENCE);

static Weapon::Template& getTemplate(WeaponType_e type)
{
	if(gWeaponTemplates.find(type) == gWeaponTemplates.end()) {
		return TEMPLATE_DEFAULT;
	}
	return gWeaponTemplates[type];
}

typedef bool (*property_setter_t)(Weapon *, std::stringstream&, const std::string&);

#define PROP_Header(name)  PROP_CastedHeader(name, Weapon, weapon)
#define PROP_Wrapper(name) PROP_CastWrapper(name, Weapon)

PROP_Header(Damage)
{
	int damage;
	if(line >> damage) {
		damage = std::max(damage, 0);
		weapon->setDamage(damage);
		return true;
	}
	std::cout << error << ": DMG property expects an integer as the damage value\n";
	return false;
}

PROP_Header(Defence)
{
	int defence;
	if(line >> defence) {
		defence = std::max(defence, 0);
		weapon->setDefence(defence);
		return true;
	}
	std::cout << error << ": DEF property expects an integer as the defence value\n";
	return false;
}

PROP_Header(Type)
{
	std::string type;
	if(line >> type) {
		WeaponType_e wType = getWeaponType(type);
		if(wType == -1) {
			std::cout << error << ": No weapon type named '" << type << "'\n";
			return false;
		}
		weapon->setWeaponType(wType);
		return true;
	}
	std::cout << error << ": TYPE field expects a type name\n";
	return false;
}

parser::subparser_map parser::maps::gWeaponSubparser = parser::subparser_map(
	{
		{ "DMG", PROP_Wrapper(Damage) },
		{ "DEF", PROP_Wrapper(Defence) },
		{ "TYPE", PROP_Wrapper(Type) },
	},
	nullptr,
	&parser::maps::gEquipmentSubparser);

std::vector<Weapon *> Weapon::all;

void Weapon::load(void)
{
	Asset data("data/weapons", ASSET_DATA);
	std::stringstream dataStream;
	dataStream << data.getData();
	std::string lineString;

	while(getline(dataStream, lineString)) {
		if(lineString.length() <= 0) {
			continue;
		}

		std::stringstream line;
		line << lineString;
		std::string opener;

		if(line >> opener && opener == "weapon") {
			if(!(line >> opener)) {
				std::cout << makeErrorMessage("data/weapons") << ": expected weapon name\n";
				continue;
			}

			Weapon *weapon = new Weapon(opener.c_str());
			Weapon::all.push_back(weapon);
			Equipment::all.push_back(weapon);
			Item::all.push_back(weapon);

			std::string error = makeErrorMessage("Weapon:" + opener);

			while(getline(dataStream, lineString)) {
				line.str("");
				line.clear();
				line << lineString;

				if(!(line >> opener)) {
					break;
				}

				if(!parser::maps::gWeaponSubparser.contains(opener)) {
					parser::invoke_defaults(parser::maps::gWeaponSubparser, weapon, lineString, error);
					continue;
				}

				auto& invoker = parser::maps::gWeaponSubparser[opener];
				if(invoker) {
					invoker(weapon, line, error);
				} else {
					std::cout << error << ": No weapon property named '" << opener << "'\n";
				}
			}
		}
	}
	std::cout << "\033[32m[Loaded " << Weapon::all.size() << " weapons]\033[0m\n";
}

#endif