#include <SDL3/SDL.h>

#include "conquest/engine/sdl/SdlError.h"


namespace conquest::engine::sdl {

[[noreturn]] void raiseSdlError(const std::string &message)
{
	log::error(message).param("error", SDL_GetError());
	throw SdlError(message);
}

}	
