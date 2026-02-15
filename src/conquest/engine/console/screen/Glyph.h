#pragma once

#include "conquest/engine/console/screen/Color.h"
#include "conquest/engine/console/assets/SpriteManager.h"


namespace conquest::engine::console {

struct Glyph
{
	SpriteId sprite;

	Color foreground;
	Color background;
};

}
