#include "conquest/engine/ui/component/custom/StatView.h"

namespace conquest::ui {

ftxui::Element detail::statLineTransform(const std::string& name, const int64_t value)
{
	return ftxui::hbox({
		ftxui::text(name),
		ftxui::filler() | size(ftxui::WIDTH, ftxui::GREATER_THAN, 4) | ftxui::flex,
		ftxui::text(std::to_string(value)),
	});
}



StatSheetOptions StatSheetOptions::highlightValue(const int64_t normalValue, const ftxui::Color over, const ftxui::Color under)
{
	return StatSheetOptions{ .transform = [=](const auto& name, auto value) {
		auto element = detail::statLineTransform(name, value);
		if(value > normalValue) {
			return element | color(over);
		}

		if(value < normalValue) {
			return element | color(under);
		}

		return element;
	} };
}

StatSheetOptions StatSheetOptions::difference(int64_t value)
{
	return StatSheetOptions {
		.transform = [=](const auto& name, auto statValue) {
			if (value != statValue) {
				return detail::statLineTransform(name, statValue);
			}

			return ftxui::emptyElement();
		},
	};
}

ftxui::Element statSheet(Stats<int64_t> values, StatSheetOptions options)
{
	ftxui::Elements lines;
	lines.reserve(STAT_COUNT);

	conquest::foreachStat([&](auto stat) {
		auto name = options.useShortNames ? std::string(conquest::toShortName(stat))
										  : enumeration::lookupEnum<std::string>(stat);
		auto value = values[stat];

		lines.emplace_back(options.transform(name, value));
	});

	return ftxui::vbox(std::move(lines));
}

}
