#pragma once

#include <ftxui/component/component.hpp>


namespace conquest::ui {

//clang-format off

inline ftxui::Decorator width(size_t width) { return ftxui::size(ftxui::WIDTH, ftxui::EQUAL, static_cast<int>(width)); }
inline ftxui::Decorator height(size_t height) { return ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, static_cast<int>(height)); }

inline ftxui::Decorator widerThan(size_t width) { return ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, static_cast<int>(width)); }
inline ftxui::Decorator tallerThan(size_t height) { return ftxui::size(ftxui::HEIGHT, ftxui::GREATER_THAN, static_cast<int>(height)); }

inline ftxui::Decorator thinerThan(size_t width) { return ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, static_cast<int>(width)); }
inline ftxui::Decorator shorterThan(size_t height) { return ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, static_cast<int>(height)); }

//clang-format on

}
