#include <thread>
#include <mutex>

#include <SDL3/SDL.h>

#include "conquest/log/Log.h"

#include "conquest/engine/sdl/Sdl.h"


namespace conquest::engine::sdl {

constexpr auto SDL_INITIALIZATION_FLAGS = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;

std::once_flag g_InitializeSdl;


void initialize()
{
	std::call_once(g_InitializeSdl, [](){
		if(!SDL_Init(SDL_INITIALIZATION_FLAGS)) {
			log::error("Failed to initialize sdl")
		        .param("error", SDL_GetError());

			return;
		}

		log::info("Initialized SDL");
	});
}

}
