
#include <map>

#include "assets.h"
#include "weapons.h"
#include "parsing.h"

#define PROP_Header(name) PROP_CastedHeader(name, Equipment, equipment)
#define PROP_Wrapper(name) PROP_CastWrapper(name, Equipment)

PROP_Header(Element) {
	std::string element;
	if(line >> element) {
		Element* elem = Elements::find(element);
		if(elem == nullptr && element != "NONE"){
			std::cout << error << ": No such element named '" << element << "'.\n";
			return false;
		}
		equipment->setElement(elem);
		return true;
	}
	std::cout << error << ": ELEM expects an element name\n";
	return false;
}

PROP_Header(Type) {
	std::string primary;
	if(line >> primary) {
		EquipmentType_e type = getEquipmentType(primary);
		if(type == -1){
			std::cout << error << ": no equipment type '" << primary << "'\n";
			return false;
		}
		equipment->setArcType(type, 0);
		return true;
	}
	std::cout << error << ": TYPE expects a type name\n";
	return false;
}

PROP_Header(Default) {
	std::string opener;
	if(line >> opener) {
		if(opener[0] == '+') return true; // add move
		return false;
	}
	return false;
}

parser::subparser_map parser::maps::gEquipmentSubparser = parser::subparser_map({
	{"ELEM", PROP_Wrapper(Element) },
	{"TYPE", PROP_Wrapper(Type) },
	{"CONSUME", nullptr}	// Remove CONSUME field, since equipments cant be consumed
}, PROP_Wrapper(Default) , &parser::maps::gItemSubparser);

std::vector<Equipment*> Equipment::all;

void Equipment::load(void) {
	
	Asset asset("data/equipment", ASSET_DATA);
	std::stringstream stream;
	stream << asset.getData();
	std::string line;

	while(getline(stream, line)) {
		std::stringstream lineStream;
		lineStream << line;

		std::string opener;
		lineStream >> opener;
		if(opener != "equipment") continue;

		std::string name;
		lineStream >> name;

		Equipment* equipment = new Equipment(name.c_str());
		Equipment::all.push_back(equipment);
		Item::all.push_back(equipment);
		std::string error = makeErrorMessage("Equipment:" + name);

		while(getline(stream, line)) {
			lineStream.str("");
			lineStream.clear();

			lineStream << line;
			if(!(lineStream >> opener)) break;
			
			if(!parser::maps::gEquipmentSubparser.contains(opener)){
				parser::invoke_defaults(parser::maps::gEquipmentSubparser,
						equipment, line, error);
				continue;
			}
			
			auto& applierMethod = parser::maps::gEquipmentSubparser[opener];
			if(applierMethod != nullptr) applierMethod(equipment, lineStream, error);
			else std::cout << error << ": No equipment property named '" << opener << "'\n";
		}
	}

	std::cout << "\033[32m[Loaded " << Equipment::all.size() << " general equipment]\033[0m\n";
}
