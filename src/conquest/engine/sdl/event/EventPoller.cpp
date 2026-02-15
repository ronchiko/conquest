#include "conquest/engine/sdl/event/EventPoller.h"


namespace conquest::engine::sdl {

std::optional<sdl::Event> EventPoller::poll()
{
	SDL_Event event;
	if(!SDL_PollEvent(&event)) {
		return std::nullopt;
	}

	if(event.type == SDL_EVENT_QUIT) {
		m_ExitRequested = true;
		return std::nullopt;
	}

	return event;
}

bool EventPoller::exitRequested() const
{
	return m_ExitRequested;
}

}
