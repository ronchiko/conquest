// #if defined(unix) || defined(__unix__) // This should only be compiled on unix machines.
#include "conquest/input/unix/input.h"

#include <unordered_map>

#include "conquest/platform/unix.h"
#include "conquest/types.h"

namespace conquest {

	static const std::unordered_map<uint32, InputType> KEYS{
		{ KEY_LEFT, InputType::Left }, { KEY_RIGHT, InputType::Right },			{ KEY_UP, InputType::Up },
		{ KEY_DOWN, InputType::Down }, { KEY_BACKSPACE, InputType::Backspace }, { KEY_ENTER, InputType::Enter },
		{ '\n', InputType::Enter },
	};

	void InputManager::get()
	{
		uint32 input = getchar();
		if(KEYS.find(input) == KEYS.end()) {
			return InputResult{ InputType::Character, static_cast<char>(input) };
		}

		return InputResult{ KEYS[input], 0 };
	}

	void InputManager::getAsync()
	{
		uint32 input = getch();
		if(KEYS.find(input) == KEYS.end()) {
			if(!iswalnum(input)) {
				return InputResult{ InputType::Nothing, 0 };
			}

			return InputResult{ InputType::Character, static_cast<char>(input) };
		}

		return InputResult{ KEYS[input], 0 };
	}
}

#endif