#include "conquest/graphics/types.h"

#include <string>
#include <unordered_map>

#include "conquest/error.h"

namespace conquest {

	static const std::unordered_map<std::string, Color> COLOR_NAMES = { 
		{ "BLACK", Color::Black },
		{ "RED", Color::Red },
		{ "GREEN", Color::Green },
		{ "YELLOW", Color::Yellow }, 
		{ "BLUE", Color::Blue }, 
		{ "MAGENTA", Color::Magenta },
		{ "CYAN", Color::Cyan },	 
		{ "GREY", Color::Grey},

		{ "BOLD_BLACK", Color::Black },	 
		{ "BOLD_RED", Color::BrightRed },	  
		{ "BOLD_GREEN", Color::BrightGreen },
		{ "BOLD_YELLOW", Color::BrightYellow }, 
		{ "BOLD_BLUE", Color::BrightBlue }, 
		{ "BOLD_MAGENTA", Color::BrightMagenta },
		{ "BOLD_CYAN", Color::BrightCyan },	 
		{ "WHITE", Color::White },
	};

	Color getColorByName(const std::string& name) {
		auto iterator = COLOR_NAMES.find(name);
		if (COLOR_NAMES.end() == iterator) {
			throw ConquestError::make(name, " is not a color");
		}

		return iterator->second;
	}
}
