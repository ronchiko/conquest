
#pragma once

#include <map>

#include "item.h"
#define MAX_MOVE_NAME_LENGTH 32

#define PROP(name) bool PROP_##name(Move* move, std::stringstream& line, const std::string& error)

enum DamageFormula_e {
	DIRECT,
	LIN_LIGHT,
	LIN_HEAVY,
	LIN_SUB
};

typedef float (*DamageFormula_f)(Stats, Stats);

class Move {
public:
	enum Targets_e {
		TARGET_SELF = 0x1,
		TARGET_ALLY = 0x2,
		TARGET_ENEMY = 0x4,
		TARGET_ALL = 0x7,
	};
	enum SelectionType_e {
		SELECT_PICK,
		SELECT_RANDOM
	};

	static std::vector<Move*> all;
	static void load(void);
	static Move* find(const std::string&);

	Move(const char* name, const Move* parent);
	
	char name[MAX_MOVE_NAME_LENGTH + 1];
	int damage;
	DamageFormula_e formula;
	Stats attack, defence;
	Stats targetStat;
	struct {
		int amount = 1;
		SelectionType_e select = Move::SELECT_PICK;
		Targets_e flags = Move::TARGET_ENEMY;
	} target;
	struct {
		float boost = 2.f;
		float chance = 0.1f;
	} critical;
	int rest = 0, await = 0;
	struct {
		int min = 1;
		int max = 1;
	} repeat;
	struct {
		int precent;
		Stats to;
	} drain;
	int cost = 3;
	std::string description;
	Element* element;
	bool requireElement;
	struct {
		int hitChance = 100;
		std::map<Stats, int> requiredStats;
	} details;
	// Children are not copied from parent
	std::vector<Move*> children;
};

typedef Move Moves;
