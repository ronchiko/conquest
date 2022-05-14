#pragma once

#include <string>
#include <vector>
#include <map>

#include "magic.h"

#define MAX_ITEM_NAME_LENGTH 32

typedef unsigned char byte_t;

enum Stats : byte_t {
	STAT_HP = 0,
	STAT_ENERGY,
	STAT_FAME,
	STAT_STRENGTH,
	STAT_DEFENCE,
	STAT_INTELLECT,
	STAT_RESISTANCE,
	STAT_SPEED,
	STAT_STEALTH,
	STAT_LUCK,
};

/* Rating is the stat on an item that defines how commonly it will be seen */
enum ItemRating_e : byte_t {
	RATE_COMMON = 0,
	RATE_UNCOMMON,
	RATE_RARE,
	RATE_VERY_RARE,
	RATE_EPIC,
	RATE_LENGDARY,
	// Out of random pools
	RATE_INTRO,	// Intro items cant be recieved as rewards (Prompt will be common)
	RATE_MYTHICAL,	// Mythical items cant be recieved as rewards (Prompt will be legendary)
};

class Character;

class Item {
public:
	enum BoostType_e {
		MOD_PRECENT,
		MOD_ADD,
		MOD_SUB
	};

	struct Effect {
		bool premanent;		// Is the change to the base stat or a boost
		Stats stat;		// The stat to effect
		float amount;		// The amount to add or multiply
		BoostType_e type;	// Should the amount be mulitplied or added
	
		inline static Effect make_additive(Stats stat, float amount){
			return {false, stat, amount, MOD_ADD};
		}
	};
	
	struct Requirement {
		Stats stat;
		int min;

		inline static Requirement make(Stats stat, int min) {
			return { stat, std::min(min, 0) };
		}
	};

	static std::vector<Item*> all;
	static void load(void);
	static const Item* find(const std::string&);
	
	Item(const char*);
	
	const char* getName(void) const { return name; }
	ItemRating_e getRating(void) const { return rating; }
	int getCost(void) const { return cost; }
	int getUses(void) const { return consumable; }
	const std::string& getDescription(void) const { return description; }

	void applyEffectsAndTraits(Character*) const;

	void setDescription(const std::string& s) { description = s; }
	void setRating(ItemRating_e rate) { rating = rate; }
	void setConsumable(int v) { consumable = v; }
	void setCost(int cost) { this->cost = cost; }
	void addEffect(const Effect& e) { effects.push_back(e); }
	void addTrait(Trait* trait) { traits.push_back(trait); }
	void addRequirement(Stats e, int minl) { requirements[e] = minl; }

	static bool addPropertyFromLine(Item* item, const std::string& line, const std::string& err); 
private:
	char name[MAX_ITEM_NAME_LENGTH + 1];
	std::string description;
	int consumable;			// The amount of times the item can be consumed
	ItemRating_e rating;		// How common will the item be pooled
	int cost;			// The avarge price of the item
	std::map<Stats, int> requirements;
	std::vector<Effect> effects;	// Effects will apply when the item is consumed
	std::vector<Trait*> traits;	// The traits the item will inflict on consumpltion
};
