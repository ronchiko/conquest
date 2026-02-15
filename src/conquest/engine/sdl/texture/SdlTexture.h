#pragma once

#include <memory>

#include <SDL3/SDL.h>


namespace conquest::engine::sdl {

enum class ScaleMode
{
	Linear = SDL_SCALEMODE_LINEAR,
	Nearest = SDL_SCALEMODE_NEAREST,
	PixelArt = SDL_SCALEMODE_PIXELART,
};

class SdlTexture
{
public:
	explicit SdlTexture(SDL_Texture *texture, ScaleMode scaling = ScaleMode::Linear);

	/**
	   Returns the raw representation of the texture.
	 */
	[[nodiscard]] SDL_Texture& raw() const;

private:
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_Texture;
};

}
