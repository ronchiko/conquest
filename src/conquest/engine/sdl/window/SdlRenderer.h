#pragma once

#include <memory>

#include <SDL3/SDL.h>

#include "conquest/engine/sdl/window/SdlWindow.h"
#include "conquest/engine/sdl/texture/SdlTexture.h"
#include "conquest/engine/sdl/texture/SdlSurface.h"
#include "conquest/engine/sdl/Rect.h"


namespace conquest::engine::sdl {

class SdlRenderer
{
public:
	explicit SdlRenderer(const SdlWindow& window);

    /**
        Presents the renderer.
     */
    void preset() const;

    /**
		Returns the raw SDL_Renderer object from this object.
     */
    [[nodiscard]] SDL_Renderer& raw() const;

    /**
        Creates a new texture for this renderer from a surface.

		@param surface - The surface to create the texture from.
		@param scaling - The scaling mode to use for the texture.
     */
    [[nodiscard]] SdlTexture createTexture(const SdlSurface& surface, ScaleMode scaling) const;

    /**
        Prints a part of a texture to a part of the renderer.

		@param texture - The texture to draw.
		@param source - The area of the texture to draw.
		@param destination - The area of the renderer to draw to.
     */
    void draw(const SdlTexture& texture, const RectF& source, const RectF& destination) const;

private:
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_Renderer;
};

}
