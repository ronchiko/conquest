#pragma once

#include <string>
#include <ftxui/component/component.hpp>


namespace conquest::ui {


inline ftxui::Component text(const std::string& text)
{
    return ftxui::Renderer([=] { return ftxui::paragraph(text); });
}

/**
	Adds a text prefix to a component, aligning them horizontally.
 */
template<typename T>
ftxui::Component prefix(std::string text, T&& component)
{
	return ftxui::Container::Horizontal({
		ftxui::Renderer([text = std::move(text)] { return ftxui::text(text); }),
		std::forward<T>(component),
	});
}

}
