#pragma once

#include "conquest/engine/console/hook/Commands.h"


namespace conquest::engine::console {

class SetToggleCommand final : public CommandBase<'h', SetToggleCommand>
{
public:
	static void execute(std::istringstream& stream, TextWriter& writer);
};

class ResetToggleCommand final : public CommandBase<'l', ResetToggleCommand>
{
public:
	static void execute(std::istringstream& stream, TextWriter& writer);
};

}
