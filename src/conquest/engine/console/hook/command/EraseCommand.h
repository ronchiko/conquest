#pragma once

#include "conquest/engine/console/hook/Commands.h"


namespace conquest::engine::console {

class EraseInLineCommand final : public CommandBase<'K', EraseInLineCommand>
{
public:
	static void execute(std::istringstream& stream, TextWriter& writer);
};

}
