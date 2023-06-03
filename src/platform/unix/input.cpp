#if defined(unix) || defined(__unix__) // This should only be compiled on unix machines.
#include "conquest/input/unix/input.h"

#include <cctype>
#include <iostream>
#include <unordered_map>

#include "conquest/platform/unix.h"
#include "conquest/platform/unix/ncurses_init.h"
#include "conquest/types.h"

namespace conquest {

	static const std::unordered_map<uint32, InputType> KEYS{
		{ KEY_LEFT, InputType::Left }, { KEY_RIGHT, InputType::Right },			{ KEY_UP, InputType::Up },
		{ KEY_DOWN, InputType::Down }, { KEY_BACKSPACE, InputType::Backspace }, { KEY_ENTER, InputType::Enter },
		{ '\n', InputType::Enter },
	};

	/**
	 * Handle a InputResult according to a key input
	 *
	 * @param key - The key to turn into an input
	 * result.

	 */
	static InputResult handleInputKey(const uint32 input)
	{
		const auto key = KEYS.find(input);
		if(KEYS.end() == key) {
			// If key is not readable we got no input.
			if(ERR == input) {
				return InputResult{ InputType::Nothing, 0 };
			}
			return InputResult{ InputType::Character, static_cast<char>(input) };
		}

		return InputResult{ key->second, 0 };
	}

	InputManager::InputManager()
	{
		initializeNCurses();
	}

	InputResult InputManager::get()
	{
		// This is a bit of a hack, since ncurses only supports either async or sync
		// input gathering at a time. We set to use async gathering, and instead of
		// to implement the sync gathering we just keep calling the async method
		// until we get a result.
		auto result = getAsync();
		while(!result) {
			result = getAsync();
		}
		return result;
	}

	InputResult InputManager::getAsync()
	{
		return handleInputKey(getch());
	}
}

#endif
