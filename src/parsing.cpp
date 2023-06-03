
#if false
#include "parsing.h"

static std::string replaceChar(const std::string& src, char f, char t); 

// Tables & Lists
static std::string STATS_NAMES[STAT_MAX] = {
	"HP",
	"ENRG",
	"FAME",
	"STRG",
	"DEFC",
	"INT",
	"RES",
	"SPD",
	"STLH",
	"LUCK"
};

static const char* EQUIPMENT_TYPES[] = {
	"WEAPON",
	"OFFHAND",
	"ARTEFACT",
	"NECKLACE",
	"RING",
	"HEADGEAR",
	"ARMOR",
	"GLOVES",
	NULL
};

static const char* WEAPON_TYPES[] = {
	"",
	"SWORD",
	"DAGGER",
	"ORB",
	"",
	"SPEAR",
	"STAFF",
	"",
	"AXE",
	"SCYTH",
	"",
	"SHIELD",
	"",
	"BOW",
	"CROSSBOW",
	"MUSKET",
	NULL
};

static const char* ARMOR_TYPES[] = {
	"ARMOR",
	"HEAVY",
	"ROBE",
	"LIGHT",
	NULL
};

static const char* HEADGEAR_TYPES[] = {
	"HELMET",
	"CROWN",
	"MASK",
	NULL
};

static const char* RATING_NAMES[] = {
	"COMMON",
	"UNCOMMON",
	"RARE",
	"VERY RARE",
	"EPIC",
	"LEGENDARY",
	"COMMON",
	"LEGENDARY",
	NULL
};

// Name to number & vice versa
Stats getStatIndex(const std::string& statName) {
	for(int i = 0; i < STAT_MAX; i++) {
		if(statName == STATS_NAMES[i]) return (Stats)i;
	}
	return (Stats)-1;	// Not a valid stat name
}

const std::string& getStatName(Stats stat) {
	return STATS_NAMES[(int)stat];
}

ItemRating_e getRatingId(const std::string& text) {
	std::string nameStr = replaceChar(text, '_', ' ');
	const char** nstring = RATING_NAMES;
	int i = (int)RATE_COMMON;
	while(*nstring != NULL){
		if(*nstring == nameStr) return (ItemRating_e)i;
		++nstring;
		++i;
	}
	return (ItemRating_e)-1;
}

const char* getRatingName (ItemRating_e rating) {
	if(rating == (ItemRating_e)-1) return "ITEM";
	return RATING_NAMES[(int)rating];
}

EquipmentType_e getEquipmentType(const std::string& t) {
	int i = 0;
	const char** name = EQUIPMENT_TYPES;
	while(*name != NULL) {
		if(*name == t) return (EquipmentType_e)i;
		++name;
		++i;
	}
	return (EquipmentType_e)-1;
}

const char* getEquipmentName(EquipmentType_e q) {
	if(q == (EquipmentType_e)-1) return "EQUIPMENT";
	return EQUIPMENT_TYPES[(int)q];
}

WeaponType_e getWeaponType(const std::string& n) {
	int i = 0;
	const char** name = WEAPON_TYPES;
	while(*name != NULL){
		if(*name != "" && n == *name) return (WeaponType_e)i;
		++i;
		++name;
	}
	return (WeaponType_e)-1;
}

const char* getWeaponName(WeaponType_e t) {
	if(t == (WeaponType_e)-1) return "WEAPON";
	return WEAPON_TYPES[(int)t];
}

// Helper functions
static std::string replaceChar(const std::string& src, char f, char t) {
	int i = src.find(f);
	std::string dest = src;
	if(i >= 0) dest[i] = t;
	return dest;
}

std::string readQuotedString(std::stringstream& line, int* result) {
	bool inQuote = false;
	std::stringstream ss;
	*result = 0;
	char c = 'A';
	while(line >> c) {
		if(c == '"') {
			inQuote = !inQuote;
			if(inQuote == false) break;
			continue;
		}
		if(inQuote) ss << c;
	}
	if(inQuote) *result = 1;
	return ss.str();
}

std::string getRQSError(int result) {
	switch(result){
		case 1:
			return  "string quotes not closed";
		default:
			return "failed to read description string";
	}
}

std::string makeErrorMessage(const std::string& in){
	std::stringstream ss;
	ss << "\033[31;1m[Error in " << in << "]\033[0m";
	return ss.str();
}


#endif