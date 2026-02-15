#pragma once

#include <ftxui/component/component.hpp>

#include "conquest/game/character/Stats.h"


namespace conquest::ui {

namespace detail
{

ftxui::Element statLineTransform(const std::string& name, int64_t value);

}

struct StatSheetOptions
{
	bool useShortNames = true;
	std::function<ftxui::Element(const std::string&, int64_t)> transform = detail::statLineTransform;

	/**
		Stat sheet display configuration that color stats over a specific value with a color
		and ones under with another.
	 */
	static StatSheetOptions highlightValue(
		int64_t normalValue, 
		ftxui::Color over = ftxui::Color::Green3, 
		ftxui::Color under = ftxui::Color::Red3
	);

	/**
		Stat sheet configuration that only shows the stats that change.
	 */
	static StatSheetOptions difference(int64_t value = 0);
};

/**
	Creates a stat sheet widget that display each name and its corresponding value of a stat.
 */
ftxui::Element statSheet(Stats<int64_t> values, StatSheetOptions options = {});

}