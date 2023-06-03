#if false
#include <map>

#include "moves.h"
#include "parsing.h"
#include "assets.h"

#define PROP_Wrapper(name) PROP_CastWrapper(name, Move)

static const char* FORMULA_NAMES[4] = {
	"DIRECT",
	"LIN_LIGHT",
	"LIN_HEAVY",
	"LIN_SUB"
};
static DamageFormula_e getFormula(const std::string& name) {
	for(int i = 0; i < 4; i++) if(FORMULA_NAMES[i] == name) return (DamageFormula_e)i;
	return (DamageFormula_e)-1;
}

int clamp(int x, int min, int max) {
	return x > max ? max : x < min ? min : x;
}

static bool PROP_SetDamage(Move* move, std::stringstream& line, const std::string& error) {
	int amount;
	std::string formula = "";
	if(line >> amount >> formula){
		move->damage = amount;
		DamageFormula_e eFormula = getFormula(formula);
		if(eFormula == (DamageFormula_e)-1) {
			std::cout << error << ": '" << formula << "' is not a valid damage formula name\n";
			return false;
		}
		move->formula = eFormula;
		return true;
	}
	std::cout << error << ": Damage must defined like so: DMG <amount> <formula>\n";
	return false;
}

static bool PROP_SetCost(Move* move, std::stringstream& line, const std::string& error) {
	int cost;
	if(line >> cost) {
		move->cost = std::min(cost, 0);
		return true;
	}
	std::cout << error << ": Cost must be an integer\n";
	return false;
}

static bool PROP_SetDesc(Move* move, std::stringstream& line, const std::string& error) {
	int result = 0;
	std::string description = readQuotedString(line, &result);
	if(result != 0){
		std::cout << error << ": " << getRQSError(result) << "\n";
		return false;
	}
	move->description = description;	
	return true;
}

static bool PROP_SetDrain(Move* move, std::stringstream& line, const std::string& error) {
	int precent;
	std::string stat;
	if(line >> precent) {
		move->drain.precent = precent;
		if(line >> stat) {
			Stats sStat = getStatIndex(stat);
			if(sStat == -1) {
				std::cout << error << ": '" << stat << "' is not a valid stat name\n";
				return false;
			}
			move->drain.to = sStat;
		}
		return true;
	}
	std::cout << error << ": Drain must defined like so: DRAIN <precent> <OPTIONAL:stat = HP>\n";
	return false;
}

#define STAT_PROP(name, stat) bool PROP_##name (Move* move, std::stringstream& ss, const std::string& err) { \
	std::string name;	\
	if(ss >> name) { \
		Stats _stat = getStatIndex(name); \
		if(_stat == -1) { std::cout << err << ": '" << name << "' is not a stat name\n"; return false; } \
		move->stat = _stat; return true; }\
	std::cout << err << ": Expected an stat name\n"; return false; } 

STAT_PROP(SetAStat, attack)
STAT_PROP(SetDStat, defence)
STAT_PROP(SetTStat, targetStat)

static bool PROP_SetTargets(Move* move, std::stringstream& line, const std::string& error) {
	int targets = 0;
	if(!(line >> targets)) {
		std::cout << error << ": Must defined amount of targets\n";
		return false;
	}
	move->target.amount = targets;
	std::string selectionType;
	if(line >> selectionType) {
		if(selectionType == "RANDOM") move->target.select = Move::SELECT_RANDOM;
		else if(selectionType == "PICK") move->target.select = Move::SELECT_PICK;
		else {
			std::cout << error << ": selection type must be RANDOM or PICK\n";
			return false;
		}
	}
	
	Move::Targets_e _targets = (Move::Targets_e)0;
	std::string flags;
	while(line >> flags) {
		if(flags == "SELF") _targets = (Move::Targets_e)(_targets | Move::TARGET_SELF);
		else if(flags == "ALLY") _targets = (Move::Targets_e)(_targets | Move::TARGET_ALLY);
		else if(flags == "ENEMY") _targets = (Move::Targets_e)(_targets | Move::TARGET_ENEMY);
		else if(flags == "ALL") _targets = (Move::Targets_e)(_targets | Move::TARGET_ALL);
		else {
			std::cout << error << ": invalid target name\n";
			return false;
		}
	}
	if(_targets == 0) _targets = Move::TARGET_ENEMY;
	move->target.flags = _targets;
	return true;
}

static bool PROP_SetMiss(Move* move, std::stringstream& line, const std::string& error) {
	int chance;
	if(!(line >> chance)) {
		std::cout << error << ": Miss chance expected to be an integer\n";
		return false;		
	}
	chance = clamp(chance, 0, 100);
	chance = 100 - chance;
	move->details.hitChance = chance;
	return true;
}

static bool PROP_AddRequirement(Move* move, std::stringstream& line, const std::string& error) {
	std::string statName;
	int level;
	if(line >> statName >> level) {
		level = std::min(0, level);
		if(statName == "ALL") {	// For all stats
			for(int i = 0; i < STAT_MAX; i++) move->details.requiredStats[(Stats)i] = level;
			return true;
		}
		Stats stat = getStatIndex(statName);
		if(stat == -1) {
			std::cout << error << ": Invalid stat name\n";
			return false;
		}
		move->details.requiredStats[stat] = level;
		return true;
	}
	std::cout << error << ": Define stat requirement like so: REQSTAT <stat_name> <min_level>\n";
	return false;
}

static bool PROP_SetCrit(Move* move, std::stringstream& line, const std::string& error) {
	int chance;
	if(!(line >> chance)){
		std::cout << error << ": crit chance must be an integer between 0 to 100\n";
		return false;
	}
	move->critical.chance  = clamp(chance, 0, 100) / 100.f;
	int mult;
	if(line >> mult) {
		move->critical.boost = clamp(mult, 0, 100) / 100.f;
	}
	
	return true;
}

#define INT_PROP(name, prop) static bool PROP_##name(Move* move, std::stringstream& ss, const std::string& er) { \
	int value; \
	if(ss >> value){ move->prop = value; return true; } \
	std::cout << er << ": Expected an integer\n"; return false; }

INT_PROP(SetAwait, await)
INT_PROP(SetRest, rest)

static bool PROP_SetElement(Move* move, std::stringstream& line, const std::string& error) {
	std::string name;
	if(line >> name) {
		Element* element = name != "NONE" ? Element::find(name) : nullptr;
		if(name != "NONE" && element == nullptr){
			std::cout << ": No element named '" << name << "'\n";
			return false;
		}
		move->element = element;
		if(!(line >> move->requireElement)) move->requireElement = true;
		return true;
	}
	std::cout << error << ": Expected an element name\n";
	return false;
}

static bool PROP_Add(Move* move, std::stringstream& line, const std::string& err) {
	return true;
}

static bool PROP_Repeat(Move* move, std::stringstream& line, const std::string& error) {
	int min, max;
	if(line >> min) {
		move->repeat.min = move->repeat.max = min;
		if(line >> max) {
			move->repeat.max = max;
		}
		return true;
	}
	std::cout << error << " Repeat expects one or two integers\n";
	return false;
}

 parser::subparser_map parser::maps::gMoveSubparser = parser::subparser_map({
	{"DMG", 	PROP_Wrapper(SetDamage)},
	{"COST", 	PROP_Wrapper(SetCost)},
	{"DESC", 	PROP_Wrapper(SetDesc)},
	{"DRAIN",	PROP_Wrapper(SetDrain)},
	{"ASTAT",	PROP_Wrapper(SetAStat)},
	{"DSTAT",	PROP_Wrapper(SetDStat)},
	{"TSTAT",	PROP_Wrapper(SetTStat)},
	{"TARGET",	PROP_Wrapper(SetTargets)},
	{"MISS",	PROP_Wrapper(SetMiss)},
	{"REQSTAT", 	PROP_Wrapper(AddRequirement)},
	{"CRIT", 	PROP_Wrapper(SetCrit)},
	{"AWAIT", 	PROP_Wrapper(SetAwait)},
	{"REST", 	PROP_Wrapper(SetRest)},
	{"ELEM", 	PROP_Wrapper(SetElement)},
	{"ADD", 	PROP_Wrapper(Add)},
	{"REPEAT", 	PROP_Wrapper(Repeat)}
}, nullptr);

std::vector<Move*> Moves::all;

Move::Move(const char* _name, const Move* parent) {
	targetStat = STAT_HP;
	drain.precent = 0;
	if(parent != nullptr){
		// Sadly just coping memory doesn't work std::map
		targetStat = parent->targetStat;
		attack = parent->attack;
		defence = parent->defence;
		damage = parent->damage;
		formula = parent->formula;
		
		target.amount = parent->target.amount;
		target.select = parent->target.select;
		target.flags  = parent->target.flags;

		critical.boost  = parent->critical.boost;
		critical.chance = parent->critical.chance;
		await = parent->await;
		rest = parent->rest;
		repeat.min = parent->repeat.min;
		repeat.max = parent->repeat.max;	

		drain.precent = parent->drain.precent;
		drain.to = parent->drain.to;
		
		cost = parent->cost;
		description = parent->description;
		
		element = parent->element;
		requireElement = parent->requireElement;

		details.hitChance = parent->details.hitChance;
		details.requiredStats = parent->details.requiredStats;
	}

	std::memcpy(name, _name, MAX_MOVE_NAME_LENGTH);
	name[MAX_MOVE_NAME_LENGTH] = 0;
	for(int i = 0; i < MAX_MOVE_NAME_LENGTH; i++) if (name[i] == '_') name[i] = ' ';
}

Move* Moves::find(const std::string& name) {
	for(auto& move : Moves::all)
		if(move->name == name) return move;
	return nullptr;
}

void Moves::load(void) {
	Asset asset("data/moves", ASSET_DATA);
	
	Move* physical = new Move("Physical", nullptr);
	physical->attack = STAT_STRENGTH; physical->defence = STAT_DEFENCE;
	Move* magical = new Move("Magical", nullptr);
	magical->attack = STAT_INTELLECT; magical->defence = STAT_RESISTANCE;
	Move* stealth = new Move("Stealth", nullptr);
	stealth->attack = STAT_STEALTH; stealth->defence = STAT_SPEED;
	std::map<std::string, Move*> moves = {
		{ "Physical", physical },
		{ "Magical", magical },
		{ "Stealth", stealth }
	};

	std::stringstream ss;
	ss << asset.getData();

	std::string line;
	int loadedMoves = 0;

	while(getline(ss, line)) {
		
		std::stringstream lineStream;
		lineStream << line;

		std::string first;
		lineStream >> first;
		// Comment or empty line
		if(first.length() <= 0 || first[0] == '#') continue;

		if(first != "move") continue;

		std::string name;
		lineStream >> name;
		std::string error = makeErrorMessage("Move:" + name);

		// Do inheritance & upgrades
		char additional;
		Move* parent = nullptr;
		std::string f_name;
		while(lineStream >> additional){
			switch(additional) {
				case ':':
					if(lineStream >> f_name) {
						if(parent != nullptr){
							std::cout << error << ": a move can only have one parent\n";
							break;
						}
						if(moves.find(f_name) == moves.end()){
							std::cout << error << ": no move named '" << f_name << "'\n";
							break;
						}
						parent = moves[f_name];
						break;
					}
					std::cout << error << ": Expected an parent name\n";
					break;
				case '%':
				case '$':
					lineStream >> f_name;
					break;
				default:
					std::cout << error << ": moves can only have flags, parent or upgrades\n";
					break;

			}
		}
			
		Move* move = new Move(name.c_str(), (const Move*)parent);

		while(getline(ss, line)) {
			lineStream.clear();
			lineStream.str(line);

			std::string prop;
			lineStream >> prop;
			
			if(prop.length() <= 0) break;
			if(prop[0] ==  '#') continue;
			
			// Match property
			if(!parser::maps::gMoveSubparser.contains(prop)) {
				std::cout << error << ": No property named '" << prop << "'\n";
				continue;
			}
			auto& func = parser::maps::gMoveSubparser[prop];
			if(func != nullptr) {
				if(!(*func)(move, lineStream, error)) break;
			}
		}

		Moves::all.push_back(move);
		loadedMoves++;
	}
	std::cout << "\033[32m[Loaded " << loadedMoves << " moves]\033[0m\n";

	delete physical;
	delete magical;
	delete stealth;
}

#endif