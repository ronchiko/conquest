#pragma once

#include <functional>

#include "conquest/engine/sdl/texture/SdlSurface.h"
#include "conquest/engine/sdl/texture/SurfacePixel.h"


namespace conquest::engine::sdl {

class SurfaceLock
{
public:
	explicit SurfaceLock(const SdlSurface& sdlSurface);

	SurfaceLock(const SurfaceLock&) = delete;
	SurfaceLock(SurfaceLock&&) = delete;
	SurfaceLock& operator=(const SurfaceLock&) = delete;
	SurfaceLock& operator=(SurfaceLock&&) = delete;

	~SurfaceLock();

    /**
		Invokes a function on each pixel of the locked surface.

		@param function - The function to invoke.
     */
    void foreachPixel(const std::function<void(SurfacePixel&)> &function) const;

private:
	const SdlSurface *m_Surface;
};

}
