#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <map>
#include <functional>

#include "character.h"

namespace parser {
	typedef bool (*parser_function)(void*, std::stringstream&, const std::string&);
	
	template<class T, class E>
	class map {
	public:

		map(const std::map<T, E>& map, const E& default_, parser::map<T,E> *p = nullptr) : 
			default_(default_), context(map), parent(p) {}
		
		bool contains(const T& name) const {
			if(context.find(name) != context.end()) return true;
			return parent ? parent->contains(name) : false;
		}

		E& operator[](const T& name) {
			if(context.find(name) == context.end()){
				return parent ? parent->operator[](name) : default_; 
			}
			return context[name];
		}

		E safe_get(const T& name) {
			if(contains(name)) return operator[](name);
			return default_;
		}


	private:
		friend bool invoke_defaults(map<std::string, parser_function>&, void*, const std::string&, const std::string&);
		
		E default_;
		std::map<T, E> context;
		parser::map<T, E>* parent;
	};
	

	typedef map<std::string, parser_function> subparser_map;
	
	inline bool invoke_defaults(subparser_map& pmap, void* arg, const std::string& line,
			const std::string& err) {
		std::stringstream l; l << line;
		if(pmap.default_ && pmap.default_(arg, l, err)) return true;
		return invoke_defaults(*(pmap.parent), arg, line, err);
	}

	namespace maps {
		extern subparser_map gMoveSubparser;

		extern subparser_map gItemSubparser;
		extern subparser_map gEquipmentSubparser;
		extern subparser_map gWeaponSubparser;	
	};
};

#define PROP_Name(name) PROP_##name
#define PROP_CastWrapper(name, type) [](void* a, std::stringstream& b, const std::string& e) { \
	return PROP_Name(name)((type*)a, b, e); \
}
#define PROP_CastedHeader(name, type, tname) static bool PROP_Name(name)(type* tname, std::stringstream& line, \
		const std::string& error)

std::string makeErrorMessage(const std::string& ss);
std::string readQuotedString(std::stringstream&, int*);
std::string getRQSError(int r);

ItemRating_e getRatingId(const std::string&);
const char* getRatingName(ItemRating_e);

Stats getStatIndex(const std::string& statName);
const std::string& getStatName(Stats statIndex);

EquipmentType_e getEquipmentType(const std::string&);
const char* getEquipmentName(EquipmentType_e);

WeaponType_e getWeaponType(const std::string&);
const char* getWeaponName(WeaponType_e);
