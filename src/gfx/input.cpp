
#include <map>
#include <ncurses.h>

#include "input.h"

static std::map<int32_t, Key> KEYS = {
	{KEY_LEFT,		Key::Left},
	{KEY_RIGHT,		Key::Right},
	{KEY_UP,		Key::Up},
	{KEY_DOWN,		Key::Down},
	{KEY_BACKSPACE, Key::Backspace},
	{KEY_ENTER,		Key::Enter},
	{'\n',		Key::Enter}
};


KeyPress get_key(void) {
	int32_t input = getch();
	if(KEYS.find(input) == KEYS.end()) {
		return KeyPress{
			.key = Key::Character,
			.glyph = static_cast<char>(input)
		};
	}

	return KeyPress{
		.key = KEYS[input],
		.glyph = '\0'
	};
}