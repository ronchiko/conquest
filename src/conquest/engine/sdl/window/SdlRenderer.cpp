#include <stdexcept>

#include <SDL3/SDL.h>

#include "conquest/engine/sdl/Sdl.h"
#include "conquest/engine/sdl/SdlError.h"
#include "conquest/log/Log.h"

#include "conquest/engine/sdl/window/SdlRenderer.h"


namespace conquest::engine::sdl {

namespace {

SDL_Renderer *createRenderer(SDL_Window& window)
{
	sdl::initialize();

	SDL_Renderer *renderer = SDL_CreateRenderer(&window, nullptr);
	if(!renderer) {
		raiseSdlError("Failed to create SDL renderer");
	}

	return renderer;
}

}

SdlRenderer::SdlRenderer(const SdlWindow& window)
	: m_Renderer(createRenderer(window.raw()), &SDL_DestroyRenderer)
{}

void SdlRenderer::preset() const
{
	SDL_RenderPresent(m_Renderer.get());
}

SDL_Renderer & SdlRenderer::raw() const
{
	return *m_Renderer;
}

SdlTexture SdlRenderer::createTexture(const SdlSurface &surface, const ScaleMode scaling) const
{
	const auto texture = SDL_CreateTextureFromSurface(&raw(), &surface.raw());
	if (nullptr == texture) {
		raiseSdlError("Failed to create texture from surface");
	}

	return SdlTexture(texture, scaling);
}

void SdlRenderer::draw(const SdlTexture &texture, const RectF &source, const RectF &destination) const
{
	if(!SDL_RenderTexture(&raw(), &texture.raw(), source, destination)) {
		raiseSdlError("Failed to render texture");
	}
}

}
