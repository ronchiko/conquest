#pragma once

#include <vector>

#include "conquest/engine/console/screen/Glyph.h"
#include "conquest/engine/console/assets/SpriteManager.h"


namespace conquest::engine::console {

constexpr auto TILE_SCALE = 18.0f;

class ConsoleScreenBuffer
{
public:
	explicit ConsoleScreenBuffer(SpriteManager& manager, size_t width, size_t height, float tileScale = TILE_SCALE);

    /**
        Sets a glyph on the screen buffer.
        Notice: THIS ROUTINE IS UNSAFE AND DOES NOT CHECK BOUNDARIES. 
     */
    void setGlyph(size_t x, size_t y, Glyph glyph);

    [[nodiscard]] size_t width() const;
	[[nodiscard]] size_t height() const;

	void draw() const;

private:
	SpriteManager *m_Mapper;

	size_t m_Width;
	size_t m_Height;
	float m_TileScale;
	std::vector<Glyph> m_Buffer;
};


}
