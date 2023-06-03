#if false

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

#include "assets.h"
#include "character.h"

#define RACE_PATH "data/races"

#include "parsing.h"

std::vector<Race*> Race::all;

void Race::load(void) {
	
	Asset racesFile(RACE_PATH, ASSET_DATA);

	std::stringstream data;
	data << racesFile.getData();
	std::string line, word;

	while(getline(data, line)){
		// Check if its an empty line
		if(line.length() <= 0) continue;
		
		char first[2] = { 0 };
		sscanf(line.c_str(), "%1s", first);
		if(*first == '#') continue;
		
		if(line.length() > 0 && line[line.length() - 1]  == ':'){	

			char raceName[MAX_RACE_NAME_LENGTH + 2] = { 0 };
			sscanf(line.c_str(), "%16s", raceName);

			for(int i = 15; i >= 0; i--) 
				if(raceName[i] == ':') {
					raceName[i] = '\0';
					break;
				}

			Race *race = new Race(raceName);

			while(getline(data, line)){
				if(line.length() <= 0) break;
				
				char first[2] = { 0 };
				sscanf(line.c_str(), "%1s", first);
				
				switch(*first){
					case '+': 	// Blessing
					case '#':	// Comment
					case '\0':
						break;
				default: {
					char statName[6] = {0};
					char trash[100] = {0};
					int modifer = 0;
					float growth = 1;
					sscanf(line.c_str(), "%5s%99[ \t]%d%99[ \t]%f", statName, trash, &modifer, trash, &growth);
					
					Stats statIndex = getStatIndex(statName);
				        if(statIndex == (Stats)-1){
						std::cout << "\033[31;1m[Error in Race:" << race->name  
							<< "]\033[0m: Invalid stat" << "name '" << statName << "'\n";
						break;
					}
					race->setGrowthRate(statIndex, growth);
					race->setStatModifer(statIndex, modifer);
					 } break;
				}
			}

			Race::all.push_back(race);
		}

	}
	//std::cout << racesFile.getDat:();
	std::cout << "\033[32m[Loaded " << all.size() << " races]\033[0m\n";
}

Race::Race(const char* _name) {
	memcpy(name, _name, MAX_RACE_NAME_LENGTH);
	name[MAX_RACE_NAME_LENGTH] = '\0';
	for(int i = 0; i < STAT_MAX; i++){
		statTable[i] = 0;
		growthTable[i] = 1;
	}
}

#endif