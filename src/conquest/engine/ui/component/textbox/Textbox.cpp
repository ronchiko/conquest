#include <ftxui/component/event.hpp>

#include "conquest/engine/ui/component/textbox/Textbox.h"


namespace conquest::ui {

namespace {

ftxui::InputOption createInputOptions(const size_t characters)
{
	auto options = ftxui::InputOption{};
	options.multiline = false;
	options.transform = [=](ftxui::InputState state) {
		if(state.focused) {
			state.element |= ftxui::underlined;
		}

		if(state.hovered) {
			state.element |= ftxui::color(ftxui::Color::White);
		}

		if(state.is_placeholder) {
			state.element |= ftxui::dim;
		}

		return state.element | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, static_cast<int>(characters));
	};

	return options;
}

}

ftxui::Component textBox(std::string& text, const std::string& placeholder, Limit limit)
{
	auto options = createInputOptions(limit.characters);
	options.multiline = limit.multiline;

	return ftxui::Input(&text, placeholder, options) | ftxui::CatchEvent(
		[&, characters=limit.characters](const ftxui::Event& event) {
		    return event.is_character() && text.length() > characters;
	    }
	);
	
}


}