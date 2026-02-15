#pragma once

#include <string>
#include <ftxui/component/component.hpp>


namespace conquest::ui {

/**
    Creates a title text component.
 */
ftxui::Component title(const std::string& text, ftxui::Color color = ftxui::Color::White);

}
