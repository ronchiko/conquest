
#include <cstring>
#include <iostream>
#include <sstream>
#include <map>

#include "screen.h"
#include "assets.h"
#include "magic.h"
#include "character.h"

#include "parsing.h"

std::vector<Element*> Element::all;

Element::Element(const char* _name) {
	memcpy(name, _name, MAX_ELEMENT_NAME_LENGTH);
	name[MAX_ELEMENT_NAME_LENGTH] = 0;
}

float Element::getEffectiveness(const Element* other) const {
	for(auto& matchup : m_Matchups){
		if(matchup.element == other) return matchup.value;
	}
	return 1;
}

Element* Element::find(const std::string& name) {
	if(name == "NONE") return nullptr;

	for(auto& element : all) {
		if(element->getName() == name) return element;
	}
	return nullptr;
}

void Element::load(void) {
	
	std::map<std::string, Element*> elements;
	Asset asset("data/elements", ASSET_DATA);

	std::stringstream ss;
	ss << asset.getData();
	std::string line;

	// First we find all the element to map their names
	while(getline(ss, line)){
		if(line.length() <= 0) continue;
		
		char first[6] = { 0 };
		str_scanf(line, "%5s", first);
		if(first[0] == 0) continue;

		if((std::string)first == "elem"){
			char name[MAX_ELEMENT_NAME_LENGTH + 1];
			str_scanf(line, "elem %16s", name);
			
			Element* element = new Element(name);
			
			all.push_back(element);
			elements[name] = element;
		}
	}
	
	std::cout << "\033[32m[Loaded " << all.size() << " elements]\033[0m\n";

	ss.clear();
	// After the elements are mapped, we can parse thier data
	while(getline(ss, line)){
		if(line.length() <= 0) continue;
	
		char first[6] = { 0 };
		str_scanf(line, "%5s", first);
		if(first[0] == 0) continue;

		if((std::string)first == "elem"){
			char name[MAX_ELEMENT_NAME_LENGTH + 1];
			char trash[100];
			str_scanf(line, "elem %16s", name);
			
			Element* element = elements[name];
			while(getline(ss, line)){
				if(line.length() <= 0) break;
				
				name[0] = 0;
				str_scanf(line, "%99[ \t]%16s", trash, name);
				if(name[0] == 0) break;
				// If is an element modifer
				if(elements.find(name) != elements.end()){
					float effectiveness = 1;
					str_scanf(line, "%99[ \t]%16s%99[ \t]%f", trash, name, trash, &effectiveness);

					ElementMatchup matchup = { elements[name], effectiveness };
					element->m_Matchups.push_back(matchup);
				}else if((std::string)name == "COLOR"){
					// Map colors
					char colorName[21];
					str_scanf(line, "%99[ \t]%16s%99[ \t]%20s", trash, name, trash, colorName);
					element->setColor(color_by_name(colorName));
				}else std::cout << "\033[31;0m[Error in element " << element->getName() << "]\033[0m:"
					<< " No element/property named " << name << "\n";

			}
		}
	}
}
