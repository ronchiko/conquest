#include <optional>

#include "conquest/common/data/Expect.h"
#include "conquest/log/Log.h"

#include "conquest/engine/console/hook/CommandParser.h"


namespace conquest::engine::console {

namespace {

constexpr auto COMMAND_INITIAL_CHARACTER = '\x1B';
constexpr auto COMMAND_SECONDARY_CHARACTER = '[';

}

CommandParser::CommandParser(TextWriter &writer)
    : m_Writer(writer)
{}

bool CommandParser::feed(const int32_t character)
{
	if(m_Command.empty() || COMMAND_INITIAL_CHARACTER == character) {
		if(COMMAND_INITIAL_CHARACTER != character) {
			return false;
		}

		m_Command.clear();
		m_Command.write(static_cast<char>(character));
		return true;
	}

	if(COMMAND_INITIAL_CHARACTER == m_Command.peek() && COMMAND_SECONDARY_CHARACTER == character) {
		m_Command.write(static_cast<char>(character));
		m_IsOpen = true;
		return true;

		// TODO: Else deal with basic ESC+single commands
	}

	if(!m_IsOpen) {
		return false;
	}

	m_Command.write(static_cast<char>(character));
	if(character >= '@' && character <= '~') {
		// Basic CSI commands are terminated by a character in the range 0x40 to 0x7E
		parseCommand();
	}

	// The command instance is broken.
	return true;
}

void CommandParser::parseCommand()
{
	try {
		m_Command.write('\0');
		std::istringstream stream(m_Command.get());

		CommandIdentifier_t commandType;
		stream.seekg(-1, std::ios::end);
		stream >> commandType;

		stream.seekg(0, std::ios::beg);
		stream >> expect(COMMAND_INITIAL_CHARACTER) >> expect(COMMAND_SECONDARY_CHARACTER);

        CommandCreator::execute(commandType, stream, m_Writer);

		// detail::CommandInfo<std::optional> edits;
		// switch(commandId) {
		// case 0:
		// 	m_LatestCommand = DEFAULT_COMMAND;
		// 	break;
		// case 38:
		// {
		// 	int32_t subCommandId = 0, red = 0, green = 0, blue = 0;
		// 	stream >> expect(';') >> subCommandId;
		// 	if(subCommandId != 2) {
		// 		break;
		// 	}
		//
		// 	stream >> expect(';') >> red >> expect(';') >> green >> expect(';') >> blue >> expect('m');
		// 	edits.foreground = Color{static_cast<uint8_t>(red), static_cast<uint8_t>(green), static_cast<uint8_t>(blue)};
		// 	break;
		// }
		// case 48:
		// {
		// 	int32_t subCommandId = 0, red = 0, green = 0, blue = 0;
		// 	stream >> expect(';') >> subCommandId;
		// 	if(subCommandId != 2) {
		// 		break;
		// 	}
		//
		// 	stream >> expect(';') >> red >> expect(';') >> green >> expect(';') >> blue >> expect('m');
		// 	edits.background = Color{ static_cast<uint8_t>(red), static_cast<uint8_t>(green), static_cast<uint8_t>(blue) };
		// 	break;
		// }
		// }
	} catch(const std::exception& e) {
        log::error("Failed to parse console command")
		    .param("command", m_Command.get())
	        .param("exception", e.what());
	}

	m_IsOpen = false;
	m_Command.clear();
}

}
