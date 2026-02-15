#pragma once

#include <optional>

#include <SDL3/SDL.h>
#include <ftxui/component/event.hpp>

#include "conquest/engine/sdl/event/Event.h"


namespace conquest::engine::sdl {

/**
   Converts an SDL event and event that could be published to an FTXUI interactive screen.
 */
std::optional<ftxui::Event> convertEvent(const Event &event);

}
