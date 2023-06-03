#if false

#include <cstring>
#include <iostream>
#include <sstream>

#include "character.h"
#include "assets.h"

#include "parsing.h"

#define PROP_Header(name) PROP_CastedHeader(name, Item, item)
#define PROP_Wrapper(name) PROP_CastWrapper(name, Item)

std::vector<Item*> Item::all;

Item::Item(const std::string& _name) {
	memcpy(name, _name.c_str(), MAX_ITEM_NAME_LENGTH);
	name[MAX_ITEM_NAME_LENGTH] = 0;
	for(int i = 0; i < MAX_ITEM_NAME_LENGTH; i++) if(name[i] == '_') name[i] = ' ';
}

static Item::Effect makeEffect(const std::string& statName, std::stringstream& stream, 
		const std::string& error, int* success) {
	char pre;
	int amount;
	*success = 1;
	stream >> pre >> amount; 
	Item::BoostType_e type = pre == '%' ? Item::MOD_PRECENT : pre == '-' ? Item::MOD_SUB : Item::MOD_ADD;
	Stats stat = getStatIndex(statName);
	if(stat == -1){
		std::cout << error << ": No stat named '" << statName << "'\n";
		*success = 0;
	}
	return {false, stat, (float)amount, type};
}

PROP_Header(Rate) {
	std::string rate;
	if(line >> rate) {
		ItemRating_e rating = getRatingId(rate);
		if(rating == -1) {
			std::cout << error << ": No item rating named '" << rate << "'\n";
			return false;
		}
		item->setRating(rating);
		return true;
	}
	std::cout << error << ": Rate field expects a rating name\n";
	return false;
}

PROP_Header(Cost) {
	int cost;
	if(line >> cost) {
		cost = std::max(cost, 0);
		item->setCost(cost);
		return true;
	}
	std::cout << error << ": COST property expects an integer as the cost value\n";
	return false;
}

PROP_Header(Desc) {
	int result;
	std::string desc = readQuotedString(line, &result);
	if(result != 0) {
		std::cout << error << ": failed to read description '" << desc << "'\n";
		return false;
	}
	item->setDescription(desc);
	return true;
}

PROP_Header(Consume) {
	int consume = 0;
	if(line >> consume){
		item->setConsumable(consume);
		return true;
	}
	std::cout << error << ": CONSUME expects an integer\n";
	return false;

}

PROP_Header(Requirement) {
	std::string stat;
	int level;
	if(line >> stat >> level) {
		Stats stat_t = getStatIndex(stat);
		if(stat_t == -1) {
			std::cout << error << ": No stat named '" << stat << "'\n";
			return false;
		}
		level = std::max(level, 0);
		item->addRequirement(stat_t, level);
		return true;
	}
	std::cout << error << ": REQSTAT requires a stat name and minimum level values\n";
	return false;
}

PROP_Header(Default) {
	std::string prop;
	line >> prop;
	
	switch(prop[0]) {
		case '%': return true;	// add trait
	}

	bool baseModifer = false;
	if(prop.rfind("BASE_", 0) == 0){
		baseModifer = true;
		prop = prop.substr(5);
	}

	int successful = 0;
	Item::Effect effect = makeEffect(prop, line, error, &successful);
	
	if(!successful){
		std::cout << error << ": no property named '";
		if(baseModifer) std::cout << "BASE_";
		std::cout << prop << "'\n";
		return false;
	}
	effect.premanent = baseModifer;
	item->addEffect(effect);
	return true;
}

parser::subparser_map parser::maps::gItemSubparser = parser::subparser_map({
	{"RATE", 	PROP_Wrapper(Rate) },
	{"COST", 	PROP_Wrapper(Cost) },
	{"DESC", 	PROP_Wrapper(Desc) },
	{"REQSTAT", 	PROP_Wrapper(Requirement) },
	{"CONSUME", 	PROP_Wrapper(Consume) }
}, PROP_Wrapper(Default), nullptr);

bool Item::addPropertyFromLine(Item* item, const std::string& line, const std::string& error) {
	std::stringstream ss; ss << line;

	std::string prop;
	ss >> prop;
	
	if(prop.length() <= 0) return false;
	if(prop[0] == '#') return true;	// Comment
	if(prop == "HEAL") prop = "HP";
	
	if(!parser::maps::gItemSubparser.contains(prop)) {
		ss.str(line);
		ss.clear();
	}
	auto subparser = parser::maps::gItemSubparser.safe_get(prop);
	if(subparser != nullptr) 
		return subparser(item, ss, error);
	
	return false;
}

void Item::load(void){
	
	Asset asset("data/items", ASSET_DATA);

	std::stringstream ss;
	ss << asset.getData();
	std::string line;
	
	while(getline(ss, line)) {
		if(line.length() <= 0) continue;

		char name[MAX_ITEM_NAME_LENGTH + 1] = { 0 };
		str_scanf(line, "item %32s", name);
		if(name[0] == 0) continue;

		Item* item = new Item(name);
		Item::all.push_back(item);
		std::string error = makeErrorMessage(name);

		while(getline(ss, line)) {
			if(line.length() <= 0) break;
			
			if(!Item::addPropertyFromLine(item, line, error)) break;
		}
	}

	std::cout << "\033[32m[Loaded " << Item::all.size() << " items]\033[0m\n";
}

#endif