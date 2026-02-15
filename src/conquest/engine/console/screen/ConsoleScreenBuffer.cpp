#include "conquest/engine/console/screen/ConsoleScreenBuffer.h"


namespace conquest::engine::console {

ConsoleScreenBuffer::ConsoleScreenBuffer(SpriteManager& manager, const size_t width, const size_t height, const float tileScale)
    : m_Mapper(&manager)
    , m_Width(width)
    , m_Height(height)
    , m_TileScale(tileScale)
{
	m_Buffer.resize(m_Width * m_Height);
}

void ConsoleScreenBuffer::setGlyph(const size_t x, const size_t y, const Glyph glyph)
{
	m_Buffer[x + y * m_Width] = glyph;
}

size_t ConsoleScreenBuffer::width() const
{
	return m_Width;
}

size_t ConsoleScreenBuffer::height() const
{
	return m_Height;
}

void ConsoleScreenBuffer::draw() const
{
	for(size_t y = 0; y < m_Height; ++y) {
		for(size_t x = 0; x < m_Width; ++x) {
			const auto&[sprite, foreground, background] = m_Buffer[x + y * m_Width];
			const auto destination = Rect{ x * m_TileScale, y * m_TileScale, m_TileScale, m_TileScale };

			m_Mapper->draw(
				sprite,
				destination,
				foreground,
				background
			);
		}
	}
}

}
