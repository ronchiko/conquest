#pragma once

#include "conquest/buffer.h"
#include "conquest/graphics/types.h"
#include "conquest/platform/unix.h"
#include "conquest/types.h"

namespace conquest {

	class ConsoleScreen
	{
	public:
		/**
		 * Construct a new Screen object
		 *
		 * @param minimumSize - The minimum screen size required.
		 */
		explicit ConsoleScreen(v2<uint32> minimumSize);

		/**
		 * Draws a pixel to the screen.
		 *
		 * @param point - Where to draw the pixel
		 * @param pixel - The pixel to draw
		 */
		void draw(v2<uint32> point, const Pixel& pixel);

		/**
		 * Draws the screen.
		 */
		void flush();

		/**
		 * The width of the screen.
		 */
		constexpr uint32 width() const;
		/**
		 * The height of the screen.
		 */
		constexpr uint32 height() const;

	private:
		v2<uint32> mSize;
		Buffer<Pixel> mScreenBuffer;
	};
}

constexpr conquest::uint32 conquest::ConsoleScreen::width() const
{
	return mSize.x;
}

constexpr conquest::uint32 conquest::ConsoleScreen::height() const
{
	return mSize.y;
}