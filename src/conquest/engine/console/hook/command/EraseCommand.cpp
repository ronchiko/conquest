#include "conquest/engine/console/hook/command/EraseCommand.h"


namespace conquest::engine::console {

void EraseInLineCommand::execute(std::istringstream& stream, TextWriter& writer)
{
	uint32_t mode = 0;
	stream >> mode;

	writer.eraseInLine(static_cast<EraseMode>(mode));
}

}
