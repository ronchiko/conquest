#pragma once

#include <ftxui/component/component.hpp>

namespace conquest::window::style {

namespace detail {

inline ftxui::ButtonOption createAnimatedButtonOption() {
	auto options = ftxui::ButtonOption::Animated(
		ftxui::Color::Black,
		ftxui::Color::GrayLight,
		ftxui::Color::Blue3Bis,
		ftxui::Color::White
	);

	options.transform = [](const ftxui::EntryState& s) {
		auto element = ftxui::text(s.label);
		if(s.focused) {
			element |= ftxui::bold;
		}
		return element;
	};

	return options;
}

}

const auto ANIMATED_BUTTON = detail::createAnimatedButtonOption();

}
