
#include <iostream>
#include <sstream>
#include <cstring>

#include <unordered_map>

#include "character.h"
#include "assets.h"

#include "parsing.h"

std::vector<Proffesion*> Proffesion::all;

typedef void (*prof_option)(Proffesion*, std::stringstream&, const std::string&); 

static void PROF_addStatModifer (Proffesion* proffesion, std::stringstream& line, const std::string& err) {
	
	std::string statName;
	int modifer = 0;
	if(!(line >> statName >> modifer)){
		std::cout << err << ": Stat modifer should be defined like so: STT <stat:name> <mod:int>\n";
		return;
	}
	
	if(statName == "ALL"){
		for(int i = 0; i < STAT_MAX; i++){
			proffesion->setStatModifer((Stats)i, modifer);
		}
		return;
	}

	Stats statId = getStatIndex(statName);
	if(statId == (Stats)-1) {
		std::cout << err << ": Invalid stat name '" << statName << "'\n";
		return;
	}

	proffesion->setStatModifer(statId, modifer);
}

static std::unordered_map<std::string, prof_option> gLineParser = {
	{"EQP", NULL},
	{"STT", &PROF_addStatModifer}
};

Proffesion::Proffesion(const char* _name) {
	memcpy(name, _name, MAX_RACE_NAME_LENGTH);
	name[MAX_RACE_NAME_LENGTH] = '\0';
	for(int i = 0; i < STAT_MAX; i++){
		statTable[i] = 0;
	}
}

void Proffesion::load(void) {
	
	Asset asset("data/proffesions", ASSET_DATA);
	
	std::stringstream stream;
	stream << asset.getData();
	std::string line;

	while(getline(stream, line)) {	
		if(line.length() <= 0) continue;
		
		std::stringstream lineStream;
		lineStream << line;
		
		std::string first;
		lineStream >> first;
		
		// New proffesion defenition
		if(first ==  "prof") {
			char name[MAX_RACE_NAME_LENGTH + 1];
			sscanf(line.c_str(), "prof %16s\n", name);

			Proffesion* proffesion = new Proffesion(name);
			std::string error = makeErrorMessage(name);

			while(getline(stream, line)){
				if(line.length() <= 0) break;

				// Reset stream
				lineStream.str("");
				lineStream.clear();
				lineStream << line;
				// Read property
				std::string property;
				lineStream >> property;
				
				if(property.length() <= 0) break;

				// Check if its a comment
				if(property[0] == '#') continue;
				if(property[0] == '+') {
					// Additive property
					continue;
				}	
				// If the property is invalid move to the next one
				if(gLineParser.find(property) == gLineParser.end()) {
					std::cout << error << ": Invalid property '" << property << "'\n";
					continue;
				}
				
				// Call the specified parser for the option
				prof_option _optionCall = gLineParser[property];
				if(_optionCall) (*_optionCall)(proffesion, lineStream, error);
			}

			all.push_back(proffesion);
		}

	}
	std::cout << "\033[32m[Loaded " << all.size() << " proffesions]\033[0m\n";
}
