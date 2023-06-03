#pragma once

namespace conquest {
	enum class InputType
	{
		// Directional
		Left,
		Right,
		Up,
		Down,

		// Special
		Escape,
		Backspace,
		Enter,

		// Generic character
		Character,

		// Means no input was given.
		Nothing,
	};

	struct InputResult
	{
		InputType type; // What kind of key press just happend.

		union
		{
			char ascii;
			wchar_t unicode;
		} glyph;

		constexpr operator bool() const
		{
			return InputType::Nothing != type;
		}

		constexpr bool operator!() const
		{
			return InputType::Nothing == type;
		}
	};
}