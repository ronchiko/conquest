#include <SDL3/SDL.h>

#include "conquest/Error.h"

#include "conquest/engine/sdl/texture/SurfacePixel.h"


namespace conquest::engine::sdl {

namespace {

enum class ByteOrder
{
    BigEndian,
	LittleEndian,
};

constexpr auto BYTE_ORDER = SDL_BYTEORDER == SDL_BIG_ENDIAN ? ByteOrder::BigEndian : ByteOrder::LittleEndian;

template<size_t N, ByteOrder Order, size_t... I>
uint32_t readPixelValue(const uint8_t* address, std::integer_sequence<size_t, I...>) {
	if constexpr(ByteOrder::BigEndian == Order) {
		return (0 | ... | (address[N - 1 - I] << (I * 8)));
	} else {
		return (0 | ... | (address[I] << (I * 8)));
	}
}

template<size_t N, ByteOrder Order, size_t... I>
void writePixelValue(uint8_t *address, uint32_t value, std::integer_sequence<size_t, I...>)
{
	if constexpr(ByteOrder::BigEndian == Order) {
		((address[N - 1 - I] = (value >> (I * 8)) & 0xFF), ...);
	} else {
		((address[I] = (value >> (I * 8)) & 0xFF), ...);
	}
}

template<size_t N>
uint32_t readPixel(void* address)
{
	auto bytes = static_cast<uint8_t *>(address);
	return readPixelValue<N, BYTE_ORDER>(bytes, std::make_integer_sequence<size_t, N>{});
}


template<size_t N>
void writePixel(void *address, uint32_t value)
{
	auto bytes = static_cast<uint8_t *>(address);
	writePixelValue<N, BYTE_ORDER>(bytes, value, std::make_integer_sequence<size_t, N>{});
}

uint32_t loadPixel(const SDL_PixelFormatDetails *format, void *address)
{
	switch(format->bytes_per_pixel) {
	case 1: return readPixel<1>(address);
	case 2: return readPixel<2>(address);
	case 3: return readPixel<3>(address);
	case 4: return readPixel<4>(address);
	default:
		throw ConquestError::make("Invalid format bytes per pixel value: ", format->bytes_per_pixel);
	}
}

void storePixel(const SDL_PixelFormatDetails* format, void* address, const uint32_t value)
{
    switch(format->bytes_per_pixel) {
	case 1: writePixel<1>(address, value); break;
	case 2: writePixel<2>(address, value); break;
	case 3: writePixel<3>(address, value); break;
	case 4: writePixel<4>(address, value); break;
	default:
		throw ConquestError::make("Invalid format bytes per pixel value: ", format->bytes_per_pixel);
	}
}

}

SurfacePixel::SurfacePixel(const SDL_PixelFormatDetails* format, void *address)
    : m_Format(format)
    , m_Address(address)
{}

// ReSharper disable once CppMemberFunctionMayBeConst
void SurfacePixel::set(const ColorChannels& color)
{
	const auto pixelValue = SDL_MapRGBA(m_Format, nullptr, color.red, color.green, color.blue, color.alpha);
	storePixel(m_Format, m_Address, pixelValue);
}


ColorChannels SurfacePixel::get() const
{
	const auto pixelValue = loadPixel(m_Format, m_Address);
	ColorChannels channels;

	SDL_GetRGBA(
		pixelValue,
		m_Format,
		nullptr,
		&channels.red,
		&channels.green,
		&channels.blue,
		&channels.alpha
	);

	return channels;
}


}