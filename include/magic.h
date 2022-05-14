#pragma once

#include <vector>
#include <string>

#define MAX_ELEMENT_NAME_LENGTH 16
#define MAX_TRAIT_NAME_LENGTH 32

int clamp(int x, int min, int max);

class Element;

struct ElementMatchup {
	Element* element;
	float value;
};

class Element {
public:
	static std::vector<Element*> all;
	static void load(void);
	static Element* find(const std::string&);

	Element(const char*);
	
	int getColor(void) const { return m_Color; }
	void setColor(int color) { m_Color = color; }
	const char* getName(void) const { return name; }
	float getEffectiveness(const Element*) const;
private:
	char name[MAX_ELEMENT_NAME_LENGTH + 1];
	std::vector<ElementMatchup> m_Matchups;
	int m_Color;
};

typedef Element Elements;

class Character;
class Item;

typedef void (*EffectCallback)(Character*);
typedef int (*EffectCombatCallback)(Character*, Character*, int);
typedef int (*EffectGainCallback)(Character*, Item*);

enum TraitType_e {
	TRAIT_BLESSING,
	TRAIT_CURSE,
	TRAIT_NEUTRAL
};

struct Trait {
	static std::vector<Trait*> all;
	
	Trait(const char* name);

	char name[MAX_TRAIT_NAME_LENGTH + 1];
	TraitType_e type = TRAIT_NEUTRAL;
	EffectCallback onRecieve = NULL, onRemove = NULL, onProgress = NULL, onMilestone = NULL;
	EffectGainCallback onGainItem = NULL;
	EffectCombatCallback onDamageTaken = NULL, onDamageInflicted = NULL;
};
