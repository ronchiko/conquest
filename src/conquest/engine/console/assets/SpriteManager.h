#pragma once

#include <vector>
#include <filesystem>
#include <unordered_map>

#include "conquest/engine/sdl/texture/SdlTexture.h"
#include "conquest/engine/sdl/window/SdlRenderer.h"
#include "conquest/engine/console/screen/Color.h"


namespace conquest::engine::console {

using Rect = sdl::RectF;

using TextureId = size_t;
using SpriteId = size_t;

class SpriteManager
{
public:
	/**
	   Creates a new sprite manager that is bound to a renderer.
	 */
	explicit SpriteManager(sdl::SdlRenderer& renderer);

	/**
	   Registers a texture with the sprite manager.
	   Returns the texture id.
	 */
	TextureId registerTexture(const std::filesystem::path &textureAsset);

    /**
        Registers a sprite to the manager.

        @param textureId - The name of the texture.
        @param x - The x coordinate of the sprite.
		@param y - The y coordinate of the sprite.
		@param width - The width of the sprite.
		@param height - The height of the sprite.
     */
    SpriteId registerSprite(TextureId textureId, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	void draw(SpriteId spriteId, const Rect& destination, Color foreground, Color background) const;

private:
	struct Sprite
	{
		TextureId texture;
		uint32_t x;
		uint32_t y;
		uint32_t width;
		uint32_t height;

		[[nodiscard]] Rect area() const;
	};

	sdl::SdlRenderer *m_Renderer;
	std::vector<sdl::SdlTexture> m_Textures;
	std::vector<Sprite> m_Sprites;
};

}
