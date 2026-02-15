#include "conquest/engine/sdl/texture/SurfaceLock.h"

namespace conquest::engine::sdl {

SurfaceLock::SurfaceLock(const SdlSurface& sdlSurface)
	: m_Surface(&sdlSurface)
{
    if(auto *surface = &m_Surface->raw(); SDL_MUSTLOCK(surface)) {
		SDL_LockSurface(surface);
	}
}

SurfaceLock::~SurfaceLock()
{
    if(auto *surface = &m_Surface->raw(); SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}

void SurfaceLock::foreachPixel(const std::function<void(SurfacePixel&)>& function) const
{
	const auto *formatDetail = m_Surface->format();

	for(uint32_t y = 0; y < m_Surface->height(); ++y) {
		auto *row = static_cast<uint8_t *>(m_Surface->raw().pixels) + static_cast<size_t>(y * m_Surface->raw().pitch);
		for(uint32_t x = 0; x < m_Surface->width(); ++x) {
			auto *address = row + static_cast<size_t>(x * formatDetail->bytes_per_pixel);

			auto pixel = SurfacePixel(formatDetail, address);
			function(pixel);
		}
	}
}

}
