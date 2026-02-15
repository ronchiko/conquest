#include <iostream>

#include "conquest/log/Log.h"
#include "conquest/common/Unicode.h"
#include "conquest/engine/console/hook/CoutHook.h"


namespace conquest::engine::console {

CoutHook::CoutHook(ConsoleScreenBuffer& screen, SpriteMapper& mapper)
	: m_Writer(screen, mapper)
    , m_CommandParser(m_Writer)
{
	std::cout.rdbuf(this);
}

std::streamsize CoutHook::xsputn(const char *stream, const std::streamsize count)
{
	std::string buf;
	buf.resize(2, '\0');
	for(auto i = 0; i < count; ++stream) {
		const auto [current, distance] = parseUnicodeString(stream);
		handleCharacter(current);

		i += distance;
	}

	return count;
}

std::streambuf::int_type CoutHook::overflow(const int_type a)
{
	handleCharacter(static_cast<int32_t>(a));
    return a;
}

void CoutHook::handleCharacter(const int32_t character)
{
	if(handleSpecialCharacter(character)) {
		// We got a special character, and already handled it.
		return;
	}

	if(m_CommandParser.feed(character)) {
	    return;
	}

	try {
		m_Writer.write(character);
	} catch(const std::exception& ex) {
		log::error("Failed to map character to sprite")
			.param("character", static_cast<uint32_t>(character))
			.param("exception", ex.what());
	}
}

bool CoutHook::handleSpecialCharacter(const int32_t character)
{
	if(character == '\n') {
		m_Writer.newline();
		return true;
	}

	if(character == '\r') {
		m_Writer.carriageReturn();
		return true;
	}

	// If we got a null terminator than we can just ignore it.
	return character == 0;
}

}
