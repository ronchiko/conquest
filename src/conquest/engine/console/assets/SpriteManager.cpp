#include "conquest/data/Package.h"
#include "conquest/log/Log.h"

#include "conquest/engine/sdl/texture/SurfaceLock.h"
#include "conquest/engine/console/assets/SpriteManager.h"


namespace conquest::engine::console {

namespace {
/**
	Returns true if a surface has an alpha channel.
 */
bool hasAlphaChannel(const sdl::SdlSurface& surface)
{
	return surface.format()->Abits > 0;
}

/**
	Transforms the black pixels of a surface to be transparent.
 */
void eraseBlackPixels(const sdl::SdlSurface& surface)
{
	const auto lock = sdl::SurfaceLock{ surface };
	lock.foreachPixel([](sdl::SurfacePixel& pixel) {
		if(pixel.get() != sdl::BLACK) {
			return;
		}

		pixel.set(sdl::TRANSPARENT);
	});
}

/**
	Loads a surface from an asset path, ensuring it has an alpha channel.
 */
sdl::SdlSurface loadSurfaceWithAlphaChannel(const std::filesystem::path& texturePath)
{
	const auto stream = sdl::IoStream::fromBuffer(package::asset::raw(texturePath));
	auto surface = sdl::SdlSurface::fromBmpStream(stream);

	if(hasAlphaChannel(surface)) {
		return surface;
	}

	// We dont have an alpha channel, so we need to duplicate the surface to a new one
	// with an alpha channel.

    auto newSurface = sdl::SdlSurface::create(
		static_cast<size_t>(surface.raw().w),
		static_cast<size_t>(surface.raw().h),
		sdl::PixelFormat::RGBA8888
	);
	newSurface.blit(surface, std::nullopt, std::nullopt);
	return  newSurface;
}

/**
	Loads a texture, and changes the black pixels to be transparent.
 */
sdl::SdlTexture loadTexture(const std::filesystem::path& texturePath, const sdl::SdlRenderer& renderer)
{
	const auto surface = loadSurfaceWithAlphaChannel(texturePath);
	eraseBlackPixels(surface);

	return renderer.createTexture(surface, sdl::ScaleMode::PixelArt);
}

}

SpriteManager::SpriteManager(sdl::SdlRenderer& renderer)
	: m_Renderer(&renderer)
{}

TextureId SpriteManager::registerTexture(const std::filesystem::path& textureAsset)
{
	m_Textures.emplace_back(loadTexture(textureAsset, *m_Renderer));
	return m_Textures.size() - 1;
}

SpriteId SpriteManager::registerSprite(
	const TextureId textureId, 
	const uint32_t x, 
	const uint32_t y, 
	const uint32_t width, 
	const uint32_t height
) {
	m_Sprites.emplace_back(Sprite{
			.texture = textureId,
			.x = x,
			.y = y,
		    .width = width,
		    .height = height,
	});
	return m_Sprites.size() - 1;
}

void SpriteManager::draw(const SpriteId spriteId, const Rect &destination, const Color foreground, const Color background) const
{
	const auto& sprite = m_Sprites.at(spriteId);
	const auto& texture = m_Textures.at(sprite.texture);

	SDL_SetRenderDrawColor(&m_Renderer->raw(), background.red, background.green, background.blue, 255);
	SDL_RenderFillRect(&m_Renderer->raw(), destination);

	SDL_SetTextureColorMod(&texture.raw(), foreground.red, foreground.green, foreground.blue);
	m_Renderer->draw(texture, sprite.area(), destination);
}

Rect SpriteManager::Sprite::area() const
{
	return Rect(x, y, width, height);
}

}
