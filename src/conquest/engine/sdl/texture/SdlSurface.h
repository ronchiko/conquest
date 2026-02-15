#pragma once

#include <memory>

#include <SDL3/SDL.h>

#include "conquest/engine/sdl/Rect.h"
#include "conquest/engine/sdl/stream/IoStream.h"


namespace conquest::engine::sdl {

enum class PixelFormat : uint32_t
{
	RGBA8888 = SDL_PIXELFORMAT_RGBA8888,
	BGRA8888 = SDL_PIXELFORMAT_BGRA8888,
};

class SdlSurface
{
public:
	/**
	   Creates a new surface from a stream of a bmp image.

	   @param stream - The stream to read the bmp image from.
	 */
	static SdlSurface fromBmpStream(const IoStream &stream);

    /**
        Creates a new surface.

		@param width - The width of the surface.
		@param height - The height of the surface.
		@param format - The pixel format of the surface.
     */
	static SdlSurface create(size_t width, size_t height, PixelFormat format = PixelFormat::RGBA8888);

    /**
		Returns the raw SDL_Surface object from this surface.
     */
    [[nodiscard]] SDL_Surface& raw() const;

    /**
		Blits an area of another surface to this an area on this surface.

		@param source - The surface to blit to the surface.
		@param sourceRect - The area of the source surface to blit, if nullptr will blit the entire rect.
		@param targetRect - The area of this surface to blit to, if nullptr will blit to the entire rect.
     */
    void blit(
        const SdlSurface& source,
        const std::optional<Rect<int>> &sourceRect = std::nullopt,
        const std::optional<Rect<int>> &targetRect = std::nullopt
    );

	[[nodiscard]] size_t width() const;
	[[nodiscard]] size_t height() const;

	const SDL_PixelFormatDetails *format() const;

private:
	explicit SdlSurface(SDL_Surface *surface);

	std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> m_Surface;
};

}
