#pragma once

#include <streambuf>

#include "conquest/engine/console/screen/ConsoleScreenBuffer.h"
#include "conquest/engine/console/assets/SpriteMapper.h"
#include "conquest/engine/console/hook/CommandParser.h"
#include "conquest/engine/console/hook/TextWriter.h"


namespace conquest::engine::console {

class CoutHook : public std::streambuf
{
public:
	explicit CoutHook(ConsoleScreenBuffer& screen, SpriteMapper& mapper);

	std::streamsize xsputn(const char *stream, std::streamsize count) override;

	int_type overflow(int_type a) override;

private:
	void handleCharacter(int32_t character);

	bool handleSpecialCharacter(int32_t character);

	TextWriter m_Writer;
	CommandParser m_CommandParser;
};

}
