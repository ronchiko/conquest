#pragma once

#include <tuple>

#include <SDL3/SDL.h>


namespace conquest::engine::sdl {

struct ColorChannels
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;

	constexpr bool operator==(const ColorChannels& other) const
	{
		return std::tie(red, green, blue, alpha) == std::tie(other.red, other.green, other.blue, other.alpha);
	}

	constexpr bool operator!=(const ColorChannels& other) const
	{
		return !(*this == other);
	}
};

inline constexpr ColorChannels BLACK = { 0, 0, 0, 255 };
inline constexpr ColorChannels WHITE = { 255, 255, 255, 255 };
inline constexpr ColorChannels TRANSPARENT = { 0, 0, 0, 0 };

class SurfacePixel
{
public:
	explicit SurfacePixel(const SDL_PixelFormatDetails* format, void* address);

	void set(const ColorChannels& color);
	[[nodiscard]] ColorChannels get() const;

private:
	const SDL_PixelFormatDetails *m_Format;
	void *m_Address;
};

}
