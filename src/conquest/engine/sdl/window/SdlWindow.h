#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>


namespace conquest::engine::sdl {

class SdlWindow
{
public:
    /**
		Creates a new SDL window with a title and a width and height.

		@param title - The title of the window.
		@param width - The width of the window.
		@param height - The height of the window.
     */
    explicit SdlWindow(const std::string& title, uint32_t width, uint32_t height);

    /**
        Returns the raw SDL_Window object from this window.
     */
    [[nodiscard]] SDL_Window& raw() const;

private:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window;
};

}
