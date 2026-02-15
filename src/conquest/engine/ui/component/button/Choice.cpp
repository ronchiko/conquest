#include "conquest/engine/ui/component/button/Choice.h"

namespace conquest::ui {

namespace detail {

Choice::Choice(std::vector<std::string> options, const int32_t value)
	: m_Options(std::move(options))
	, m_Value(value)
{}


ftxui::Component Choice::createMenu(ftxui::MenuOption options)
{
	return ftxui::Menu(m_Options, &m_Value, std::move(options));
}

ftxui::Component Choice::makeReflectiveButton(const std::function<void()>& onClick, const std::string& fallbackText)
{
	auto options = ftxui::ButtonOption::Ascii();
	options.transform = [&, fallbackText](auto& s) {
		const auto name = didChoose() ? value() : fallbackText;
		const auto text = s.focused ? std::string("[" + name + "]") : std::string(" " + name + " ");
		auto element = ftxui::text(text);
		if(!didChoose()) {
			element |= color(ftxui::Color::Grey53);
		}

		return element;
	};

	options.on_click = onClick;
	return ftxui::Button(std::move(options));
}

}

}

