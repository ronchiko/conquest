#pragma once

#include "conquest/engine/types.h"
#include "conquest/types.h"

namespace conquest {

/**
 * Decribes how to draw a rectangle.
 *
 * Pattern[0] - Corner glyph
 * Pattern[1] - Horizonal edge glyph
 * Pattern[2] - Vertical edge glyph
 */
struct RectPattern
{
	char corner;
	char horizontal;
	char vertical;
};

/**
	 Premade rect patterns.
 */
namespace patterns {
inline constexpr RectPattern FRAME = { '+', '-', '|' };
inline constexpr RectPattern STAR = { '*', '*', '*' };
inline constexpr RectPattern EMPTY = { ' ', ' ', ' ' };
}

struct Area
{
	v2<uint32> point;
	v2<uint32> size;

	constexpr v2<uint32> end() const {
		return point + size;
	}
};

struct Rect
{
	Area area;
	Color background;

	struct
	{
		Color foreground;
		Color background;
		RectPattern pattern;
	} outline;
};
}