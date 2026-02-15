#pragma once

#include <optional>

#include "conquest/engine/sdl/event/Event.h"


namespace conquest::engine::sdl {

class EventPoller
{
public:
    /**
        Polls the next event in the queue if possible.
		If no event is available, returns std::nullopt.
     */
    std::optional<sdl::Event> poll();

    /**
        Was a quit event requested.
     */
    [[nodiscard]] bool exitRequested() const;

private:
	bool m_ExitRequested = false;
};

}

