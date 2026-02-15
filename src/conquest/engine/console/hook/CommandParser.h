#pragma once

#include <sstream>
#include <unordered_map>
#include <functional>

#include "conquest/common/data/Stream.h"

#include "conquest/engine/console/hook/command/TextCommand.h"
#include "conquest/engine/console/hook/command/ToggleCommand.h"
#include "conquest/engine/console/hook/command/CursorCommand.h"
#include "conquest/engine/console/hook/Commands.h"


namespace conquest::engine::console {

namespace detail {

template<Command_t... T>
class CommandsCreator
{
public:
	static void execute(const CommandIdentifier_t id, std::istringstream& stream, TextWriter& writer)
	{
		const auto parser = s_Parsers.find(id);
		if(s_Parsers.end() == parser) {
			throw std::runtime_error("Unknown command identifier");
		}

		return parser->second(stream, writer);
	}

private:
	using Parser_t = std::function<void(std::istringstream&, TextWriter&)>;

    inline static std::unordered_map<CommandIdentifier_t, Parser_t> s_Parsers = {
		{ T::IDENTIFIER, [](auto& stream, auto& writer) {  T::execute(stream, writer); } }...
	};
};

}

using CommandCreator = detail::CommandsCreator<
    TextCommand,
    SetToggleCommand,
    ResetToggleCommand,
    MoveCursorUpCommand,
    MoveCursorDownCommand,
    MoveCursorForwardCommand,
    MoveCursorBackwardCommand
>;

class CommandParser
{
public:
	explicit CommandParser(TextWriter& writer);

    /**
        Attempts to feed the command 
     */
    bool feed(int32_t character);

private:
    void parseCommand();

    TextWriter& m_Writer;

	bool m_IsOpen = false;
	Stream<char> m_Command;
};

}
