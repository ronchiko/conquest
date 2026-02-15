#pragma once


namespace conquest::character
{

constexpr auto DITHER_LIGHT = static_cast<char>(176);
constexpr auto DITHER_MEDIUM = static_cast<char>(177);
constexpr auto DITHER_DARK = static_cast<char>(178);

namespace light_edge {

constexpr auto TOP_LEFT = static_cast<char>(218); 
constexpr auto BOTTOM_RIGHT= static_cast<char>(217); 
constexpr auto TOP_RIGHT = static_cast<char>(191);
constexpr auto BOTTOM_LEFT = static_cast<char>(192);

constexpr auto VERTICAL = static_cast<char>(179);
constexpr auto HORIZONTAL = static_cast<char>(196);

}

namespace strong_edge {

constexpr auto TOP_LEFT = static_cast<char>(201);
constexpr auto TOP_RIGHT = static_cast<char>(187);
constexpr auto BOTTOM_LEFT = static_cast<char>(200);
constexpr auto BOTTOM_RIGHT = static_cast<char>(188);

constexpr auto VERTICAL = static_cast<char>(186);
constexpr auto HORIZONTAL = static_cast<char>(205);

}

}
