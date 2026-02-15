#pragma once

#include <filesystem>

#include "conquest/engine/console/assets/SpriteManager.h"


namespace conquest::engine::console {

class SpriteMapper
{
public:
	using SpriteIndex_t = uint64_t;

	explicit SpriteMapper(SpriteManager& manager);

	/**
	   Loads a sprite map asset from a path.
	 */
	void loadMapping(const std::filesystem::path& path);

    /**
        Draw a sprite by its name.

        @param name - The name of the sprite to draw.
		@param destination - The area to draw the sprite to.
		@param foreground - The foreground color to use.
		@param background - The background color to use.
     */
    void draw(SpriteIndex_t name, const Rect& destination, Color foreground, Color background) const;

    /**
        Returns the SpriteId of a named sprite.
     */
    [[nodiscard]] SpriteId fetch(SpriteIndex_t name) const;

private:
	SpriteManager *m_Manager;
	std::unordered_map<SpriteIndex_t, SpriteId> m_Mapping;
};

}
