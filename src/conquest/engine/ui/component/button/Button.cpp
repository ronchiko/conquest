#include "conquest/engine/ui/component/button/Button.h"

namespace conquest::ui {

ftxui::Component button(const std::string& text, const std::function<void()>& action)
{
	return ftxui::Button(text, action);
}

ftxui::Component conditionalButton(
	const std::string& text,
	const std::function<void()>& action,
	const std::function<bool()>& condition
) {
	auto options = ftxui::ButtonOption::Simple();
	options.label = text;
	options.on_click = [action, condition] {
		if(condition()) {
			action();
		}
	};

	options.transform = [condition](const ftxui::EntryState& state) {
		auto element = ftxui::text(state.label);
		if(!condition()) {
			element |= ftxui::color(ftxui::Color::GrayDark);
		} else if(state.active) {
			element |= ftxui::color(ftxui::Color::White) | ftxui::bold;
		}

		if(state.focused) {
			element |= ftxui::inverted;
		}

		return element;
	};

	return ftxui::Button(options);
}


}
