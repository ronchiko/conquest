#pragma once

#include "conquest/engine/console/hook/Commands.h"


namespace conquest::engine::console {

namespace detail {

enum class Direction
{
	Up,
	Down,
	Forward,
	Backward,
};

template<CommandIdentifier_t Id, Direction Direction>
class MoveCommand : public CommandBase<Id, MoveCommand<Id, Direction>>
{
public:
	static void execute(std::istringstream& stream, TextWriter& writer)
	{
		uint32_t amount = 1;
		stream >> amount;

		auto& cursor = writer.cursor();
		switch(Direction) {
		case Direction::Up:
			cursor.y = (amount > cursor.y) ? 0 : (cursor.y - amount);
			break;
		case Direction::Down:
			cursor.y += amount;
			break;
		case Direction::Forward:
			cursor.x += amount;
			break;
		case Direction::Backward:
			cursor.x = (amount > cursor.x) ? 0 : (cursor.x - amount);
			break;
		}
	}
};

}

using MoveCursorUpCommand = detail::MoveCommand<'A', detail::Direction::Up>;
using MoveCursorDownCommand = detail::MoveCommand<'B', detail::Direction::Down>;
using MoveCursorForwardCommand = detail::MoveCommand<'C', detail::Direction::Forward>;
using MoveCursorBackwardCommand = detail::MoveCommand<'D', detail::Direction::Backward>;

}
