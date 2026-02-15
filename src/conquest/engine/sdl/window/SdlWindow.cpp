#include "conquest/engine/sdl/Sdl.h"
#include "conquest/engine/sdl/SdlError.h"

#include "conquest/engine/sdl/window/SdlWindow.h"


namespace conquest::engine::sdl {

namespace {

/**
	Creates a new SDL window with a title and width and height.
 */
SDL_Window *createWindow(const std::string& title, const uint32_t width, const uint32_t height)
{
	sdl::initialize();

	SDL_Window *window = SDL_CreateWindow(title.c_str(), static_cast<int>(width), static_cast<int>(height), 0);
	if(!window) {
		raiseSdlError("Failed to create SDL window");
	}

	return window;
}

}

SdlWindow::SdlWindow(const std::string& title, const uint32_t width, const uint32_t height)
	: m_Window(createWindow(title, width, height), &SDL_DestroyWindow)
{}

SDL_Window& SdlWindow::raw() const
{
	return *m_Window;
}

}
