#include "conquest/engine/sdl/texture/SdlTexture.h"


namespace conquest::engine::sdl {

SdlTexture::SdlTexture(SDL_Texture *texture, ScaleMode scaling)
	: m_Texture(texture, &SDL_DestroyTexture)
{
	SDL_SetTextureScaleMode(m_Texture.get(), static_cast<SDL_ScaleMode>(scaling));
}

SDL_Texture &SdlTexture::raw() const
{
	return *m_Texture;
}


}