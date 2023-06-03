#pragma once

#include <optional>

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
	};

	struct InputResult
	{
		InputType type; // What kind of key press just happend.

		union
		{
			char ascii;
			wchar_t unicode;
		} glyph;
	};

	/**
	 * Initializes the input manager.
	 */
	void initializeInputManager();

	/**
	 * Queries input in a blocking manner.
	 */
	InputResult queryInput();

	/**
	 * Queries input without stopping the program.
	 */
	std::optional<InputResult> queryInputAsync();
}
