#pragma once

#include "conquest/engine/console/hook/Commands.h"


namespace conquest::engine::console {


class TextCommand : public CommandBase<'m', TextCommand>
{
public:
	static void execute(std::istringstream& stream, TextWriter& writer);
};

}
