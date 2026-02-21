#include "conquest/engine/sdl/event/EventConversion.h"

#include <unordered_map>

namespace conquest::engine::sdl {

namespace {

std::optional<ftxui::Event> convertSpecialKey(SDL_Keycode key)
{
	switch(key) {
	case SDLK_UP:       return ftxui::Event::ArrowUp;
	case SDLK_DOWN:     return ftxui::Event::ArrowDown;
	case SDLK_LEFT:     return ftxui::Event::ArrowLeft;
	case SDLK_RIGHT:    return ftxui::Event::ArrowRight;
	case SDLK_HOME:     return ftxui::Event::Home;
	case SDLK_END:      return ftxui::Event::End;
	case SDLK_PAGEUP:   return ftxui::Event::PageUp;
	case SDLK_PAGEDOWN: return ftxui::Event::PageDown;
	case SDLK_INSERT:   return ftxui::Event::Insert;
	case SDLK_RETURN:   return ftxui::Event::Return;
	default:            return std::nullopt;
	}
}

inline const std::unordered_map<SDL_Keycode, wchar_t> g_ConversionMap = {
	{ SDLK_ESCAPE, L'\x1B' },
	{ SDLK_BACKSPACE, L'\b' },
	{ SDLK_TAB, L'\t' },
	{ SDLK_SPACE, L' ' },
	{ SDLK_EXCLAIM, L'!' },
	{ SDLK_DBLAPOSTROPHE, L'"' },
	{ SDLK_HASH, L'#' },
	{ SDLK_DOLLAR, L'$' },
	{ SDLK_PERCENT, L'%' },
	{ SDLK_AMPERSAND, L'&' },
	{ SDLK_APOSTROPHE, L'\'' },
	{ SDLK_LEFTPAREN, L'(' },
	{ SDLK_RIGHTPAREN, L')' },
	{ SDLK_ASTERISK, L'*' },
	{ SDLK_PLUS, L'+' },
	{ SDLK_COMMA, L',' },
	{ SDLK_MINUS, L'-' },
	{ SDLK_PERIOD, L'.' },
	{ SDLK_SLASH, L'/' },
	{ SDLK_0, L'0' },
	{ SDLK_1, L'1' },
	{ SDLK_2, L'2' },
	{ SDLK_3, L'3' },
	{ SDLK_4, L'4' },
	{ SDLK_5, L'5' },
	{ SDLK_6, L'6' },
	{ SDLK_7, L'7' },
	{ SDLK_8, L'8' },
	{ SDLK_9, L'9' },
	{ SDLK_COLON, L':' },
	{ SDLK_SEMICOLON, L';' },
	{ SDLK_LESS, L'<' },
	{ SDLK_EQUALS, L'=' },
	{ SDLK_GREATER, L'>' },
	{ SDLK_QUESTION, L'?' },
	{ SDLK_AT, L'@' },
	{ SDLK_LEFTBRACKET, L'[' },
	{ SDLK_BACKSLASH, L'\\' },
	{ SDLK_RIGHTBRACKET, L']' },
	{ SDLK_CARET, L'^' },
	{ SDLK_UNDERSCORE, L'_' },
	{ SDLK_GRAVE, L'`' },
	{ SDLK_A, L'a' },
	{ SDLK_B, L'b' },
	{ SDLK_C, L'c' },
	{ SDLK_D, L'd' },
	{ SDLK_E, L'e' },
	{ SDLK_F, L'f' },
	{ SDLK_G, L'g' },
	{ SDLK_H, L'h' },
	{ SDLK_I, L'i' },
	{ SDLK_J, L'j' },
	{ SDLK_K, L'k' },
	{ SDLK_L, L'l' },
	{ SDLK_M, L'm' },
	{ SDLK_N, L'n' },
	{ SDLK_O, L'o' },
	{ SDLK_P, L'p' },
	{ SDLK_Q, L'q' },
	{ SDLK_R, L'r' },
	{ SDLK_S, L's' },
	{ SDLK_T, L't' },
	{ SDLK_U, L'u' },
	{ SDLK_V, L'v' },
	{ SDLK_W, L'w' },
	{ SDLK_X, L'x' },
	{ SDLK_Y, L'y' },
	{ SDLK_Z, L'z' },
	{ SDLK_LEFTBRACE, L'{' },
	{ SDLK_PIPE, L'|' },
	{ SDLK_RIGHTBRACE, L'}' },
	{ SDLK_TILDE, L'~' },
	{ SDLK_DELETE, L'\x7F' },
};

std::optional<ftxui::Event> convertKeyDownEvent(const SDL_KeyboardEvent& event)
{
	if(auto special = convertSpecialKey(event.key)) {
		return special;
	}

	const auto character = g_ConversionMap.find(event.key);
	if(g_ConversionMap.end() == character) {
		return std::nullopt;
	}

	auto rawCharacter = character->second;
	if(event.mod & SDL_KMOD_SHIFT) {
		rawCharacter = static_cast<wchar_t>(toupper(rawCharacter));
	}

	return ftxui::Event::Character(rawCharacter);
}

}

std::optional<ftxui::Event> convertEvent(const Event& event)
{
	switch(event.type) {
	case SDL_EVENT_KEY_DOWN:
		return convertKeyDownEvent(event.key);
	}

	return std::nullopt;
}

}
