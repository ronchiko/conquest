#pragma once

enum class Key {
	Left,
	Right,
	Up,
	Down,

	Character,
	Escape,
	Backspace,
	Enter
};


struct KeyPress {
	Key key;
	char glyph;
};

KeyPress get_key(void);
