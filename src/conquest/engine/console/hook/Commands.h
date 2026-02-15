#pragma once

#include <variant>
#include <sstream>

#include "conquest/engine/console/hook/TextWriter.h"


namespace conquest::engine::console {

using CommandIdentifier_t = int8_t;

template<typename T>
concept Command_t = requires(std::istringstream& stream, TextWriter& writer) {
	{ T::execute(stream, writer) };
	{ T::IDENTIFIER } -> std::convertible_to<CommandIdentifier_t>;
};

template<CommandIdentifier_t Index, typename T>
class CommandBase
{
public:
	using Instance_t = T;

	static inline constexpr CommandIdentifier_t IDENTIFIER = Index;
};

}
