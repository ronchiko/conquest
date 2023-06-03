#pragma once

#include <string>

#include "magic.h"
#include "item.h"

#define str_scanf(str, ...) sscanf(str.c_str(), __VA_ARGS__)
// Inherit from this if the class has a name field
class IName { public: virtual const char* getName(void) const = 0; };

enum EquipmentType_e : byte_t {
	EQUIPMENT_WEAPON = 0x1,
	EQUIPMENT_OFFHAND = 0x2,
	EQUIPMENT_ARTEFACT = 0x4,
	EQUIPMENT_NECKLACE = 0x8,
	EQUIPMENT_RING = 0x10,
	EQUIPMENT_HEADGEAR = 0x20,
	EQUIPMENT_ARMOR = 0x40,
	EQUIPMENT_GLOVES = 0x80
};

enum HeadgearType_e {
	HEADGEAR_HELMET,
	HEADGEAR_CROWN,
	HEADGEAR_MASK
};

enum ArmorType_e {
	ARMOR_GENERIC,
	ARMOR_HEAVY,
	ARMOR_ROBE,
	ARMOR_LIGHT
};

enum WeaponType_e {
	// Dual prepose weapons
	_WEAPON_LIGHT,
	WEAPON_SWORD,	// STRG, DEFC
	WEAPON_DAGGER,	// STLH, SPD
	WEAPON_ORB,	// INT,  RES
	// Only primary weapon
	_WEAPON_PRIMARY,
	WEAPON_SPEAR,	// STRG, SPD
	WEAPON_STAFF,	// INT, RES
	// Dual handed
	_WEAPON_HEAVY,
	WEAPON_AXE,	// STRG, DEFC
	WEAPON_SCYTH,	// INT,	 DEFC
	// Offhand only
	_WEAPON_OFFHAND,
	WEAPON_SHIELD,	// STRG, DEFC
	// Ranged
	_WEAPON_RANGED,
	WEAPON_BOW,	// STLH, DEFC
	WEAPON_CROSSBOW,// STRG, DEFC
	WEAPON_MUSKET,	// SPD,	 DEFC
	_WEAPON_MAX
};

#define HEADGEAR_MAX (HEADGEAR_MASK + 1)
#define WEAPON_MAX (WEAPON_SHIELD + 1)
#define ARMOR_MAX (ARMOR_LIGHT + 1)

class Move;
typedef const std::vector<const Move*> rd_attack_vector;

class Equipment : public Item {
public:
	static std::vector<Equipment*> all;
	static void load(void);
	static const Equipment* find(const std::string&);

	Equipment(const std::string& name) : Item(name) { setConsumable(false); }

        const char* getLabel(void) const;

	EquipmentType_e getType(void) const { return type; }
	byte_t getArcType(void) const { return arctype; }
	const Element* getElement(void) const { return element; }
	rd_attack_vector& getMoveVector(void) const { return moves; }

	void addMove(const Move* m) { moves.push_back(m); }
	void setElement(const Element* e) { element = e; }
	void setArcType(EquipmentType_e primary, byte_t arc) { arctype = arc; type = primary; }
private:
	EquipmentType_e type;
	byte_t arctype;
	const Element* element;
	bool requireElement;
	std::vector<const Move*> moves;
};

class Weapon : public Equipment {
public:
	static std::vector<Weapon*> all;
	static void load(void);
	static const Weapon* find(const std::string&);
	
	struct Template {
		Stats damager;
		Stats defender;

		static Template make(Stats dmg, Stats def) { return (Template){dmg, def}; }
	};	

	Weapon(const char* name) : Equipment(name) { setArcType(EQUIPMENT_WEAPON, 0); }
	
	WeaponType_e getWeaponType(void) const { return weaponType; }
	void setWeaponType(WeaponType_e w) { weaponType = w; }
	void setDamage(int dmg) { damage = dmg; }
	void setDefence(int def) { defence = def; }

	bool canBeOffhand(void) const;
	
private:
	/* Add the damage required by the weapon*/
	void addEffects(void);

	WeaponType_e weaponType;
	int damage;
	int defence;
};
