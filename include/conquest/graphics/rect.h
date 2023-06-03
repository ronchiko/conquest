#pragma once

#include "conquest/types.h"

namespace conquest {

	/**
	 * Decribes how to draw a rectangle.
	 *
	 * Pattern[0] - Corner glyph
	 * Pattern[1] - Horizonal edge glyph
	 * Pattern[2] - Vertical edge glyph
	 */
	union RectPattern
	{
		const char text[3];

		struct
		{
			char corner;
			char horizontal;
			char vertical;
		} primitive;
	};

	/**
	 * Premade rect patterns.
	 */
	namespace patterns

	{
		inline constexpr RectPattern FRAME = { 'o', '-', '|' };
		inline constexpr RectPattern STAR = { '*', '*', '*' };
	}

	struct Rect
	{
		v2<uint32> point;
		v2<uint32> size;
	};
} // namespace conquest