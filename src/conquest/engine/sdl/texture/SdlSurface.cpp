#include "conquest/engine/sdl/SdlError.h"

#include "conquest/engine/sdl/texture/SdlSurface.h"



namespace conquest::engine::sdl
{
SdlSurface SdlSurface::fromBmpStream(const IoStream &stream)
{
	auto* imageSurface = SDL_LoadBMP_IO(&stream.raw(), /* close io = */ false);
	if(nullptr == imageSurface) {
		raiseSdlError("Failed to load surface from bmp image");
	}

	return SdlSurface(imageSurface);
}

SdlSurface SdlSurface::create(const size_t width, const size_t height, const PixelFormat format)
{
	auto* surface = SDL_CreateSurface(static_cast<int32_t>(width), static_cast<int32_t>(height), static_cast<SDL_PixelFormat>(format));
	if(nullptr == surface) {
		raiseSdlError("Failed to create SDL surface");
	}

	return SdlSurface(surface);
}

SDL_Surface & SdlSurface::raw() const
{
	return *m_Surface;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void SdlSurface::blit(
    const SdlSurface &source,
    const std::optional<Rect<int>> &sourceRect,
    const std::optional<Rect<int>> &targetRect
) {
	const auto sourceRectPtr = sourceRect.has_value() ? static_cast<const SDL_Rect *>(*sourceRect) : nullptr;
	const auto targetRectPtr = targetRect.has_value() ? static_cast<const SDL_Rect *>(*targetRect) : nullptr;

	SDL_BlitSurface(&source.raw(), sourceRectPtr, m_Surface.get(), targetRectPtr);
}

size_t SdlSurface::width() const
{
	return static_cast<size_t>(m_Surface->w);
}

size_t SdlSurface::height() const
{
	return static_cast<size_t>(m_Surface->h);
}

const SDL_PixelFormatDetails * SdlSurface::format() const
{
	return SDL_GetPixelFormatDetails(m_Surface->format);
}

SdlSurface::SdlSurface(SDL_Surface *surface)
    : m_Surface(surface, &SDL_DestroySurface)
{}

}
