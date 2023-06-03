#pragma once

#include "conquest/assets.h"
#include "conquest/graphics/rect.h"
#include "conquest/graphics/types.h"


#if defined(_WIN32)
#include "conquest/graphics/windows/screen.h"
#elif defined(__unix__) || defined(unix)
#include "conquest/graphics/unix/screen.h"
#else
#error Rendering is not supported on this platform.
#endif

namespace conquest {
	class Screen : public ConsoleScreen // This type changes depending on the platform.
	{
	public:
		using ConsoleScreen::draw; // Allow drawing of a pixel

		explicit Screen(v2<uint32> minimumSize);

		/**
		 * Draws a string to the screen
		 *
		 * @param point - Where to draw the string.
		 * @param message - The string to write.
		 * @param foreground - The foreground color of the string.
		 * @param background - The background color of the string.
		 */
		void draw(v2<uint32> point, const std::string& message, Color foreground, Color background);

		/**
		 * Draw a rectangle to the screen.
		 *
		 * @param rect - The rect to draw
		 * @param background - The background color.
		 * @param pattern - The pattern of the rectangle edges.
		 * @param foreground - The foreground color.
		 */
		void draw(const Rect& rect, Color background, RectPattern pattern, Color foreground);

		/**
		 * Draws an asset to the screen.
		 *
		 * @param point - The top-left origin of the asset
		 * @param asset - The asset to draw.
		 * @param foreground - The foreground color of the asset.
		 * @param background - The background color of the asset.
		 */
		void draw(v2<uint32> point, const Asset& asset, Color foreground, Color background = Color::Black);

		/**
		 * Fills a rectangle.
		 *
		 * @param rect - The area to fill.
		 * @param background - The background color of the rect.
		 * @param glyph - The glyph to put inside the rectangle.
		 * @param foreground - The foreground color of the glyph.
		 */
		void fill(const Rect& rect, Color background, char glyph, Color foreground);
	};

}