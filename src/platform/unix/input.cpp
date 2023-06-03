#if defined(unix) || defined(__unix__) // This should only be compiled on unix machines.
#include "conquest/input/types.h"

#include <ncurses.h>
#include <unordered_map>

namespace conquest {

	static const std::unordered_map<int32_t, InputType> KEYS{
		{ KEY_LEFT, InpuType::Left },  { KEY_RIGHT, InputType::Right },			{ KEY_UP, InputType::Up },
		{ KEY_DOWN, InputType::Down }, { KEY_BACKSPACE, InputType::Backspace }, { KEY_ENTER, InputType::Enter },
		{ '\n', InputType::Enter },
	};

	InputResult queryInput()
	{
		int32_t input = getch();
		if(KEYS.find(input) == KEYS.end()) {
			return InputResult{ .type = InputType::Character, .glyph.ascii = static_cast<char>(input) };
		}


		return InputResult{ .type = KEYS[input], .glyph.ascii = 0 };
	}
}

#endif