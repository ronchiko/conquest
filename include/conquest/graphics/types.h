#pragma once

#include "conquest/types.h"

namespace conquest {
	enum class Color : conquest::byte
	{
		Black = 0,
		Red,
		Green,
		Yellow,
		Blue,
		Cyan,
		Magenta,
		Grey,
		

		// Bright colors, starts at 9 because there is not bright black...
		BrightRed = 0x9,
		BrightGreen,
		BrightYellow,
		BrightBlue,
		BrightCyan,
		BrightMagenta,

		White,
	};

	inline constexpr auto BRIGHT_BIT = 0b1000;

	/**
	 * Represents a pixel in a screen.
	 */
	struct Pixel
	{
		union Char
		{
			char ascii = ' ';
			wchar_t unicode;
		} glyph;

		Color foreground = Color::Black;
		Color background = Color::Black;
	};
}
