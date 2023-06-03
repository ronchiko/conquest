#include "conquest/graphics/screen.h"

namespace conquest {

	Screen::Screen(v2<uint32> minimumSize)
		: ConsoleScreen(minimumSize)
	{}

	void Screen::draw(const v2<uint32> point, const std::string& message, const Color foreground, const Color background)
	{
		Pixel current;
		current.background = background;
		current.foreground = foreground;
		for(uint32 index = 0; index < message.length(); ++index) {
			// Check we are not drawing out of bounds.
			if(point.x + index >= width()) {
				return;
			}

			current.glyph.ascii = message[index];
			draw(v2<uint32>{ point.x + index, point.y }, current);
		}
	}

	void Screen::draw(const Rect& rectangle, const Color background, const RectPattern pattern, const Color foreground)
	{
		const auto finalX = rectangle.point.x + rectangle.size.x;
		const auto finalY = rectangle.point.y + rectangle.size.y;

		Pixel current{};
		current.background = background;
		current.foreground = foreground;
		for(uint32 y = rectangle.point.y; y < finalY; ++y) {
			for(uint32 x = rectangle.point.x; x < finalX; ++x) {
				if(y >= height() || x >= width()) {
					continue;
				}

				const bool isHorizontalEdge = y == rectangle.point.y || finalY - 1 == y;
				const bool isVerticalEdge = x == rectangle.point.x || finalX - 1 == x;

				if(isHorizontalEdge && isVerticalEdge) {
					current.glyph.ascii = pattern.primitive.corner;
				} else if(isHorizontalEdge) {
					current.glyph.ascii = pattern.primitive.horizontal;
				} else if(isVerticalEdge) {
					current.glyph.ascii = pattern.primitive.vertical;
				}

				// Only draw if we are at the edge.
				if(isHorizontalEdge || isVerticalEdge) {
					draw(v2<uint32>{ x, y }, current);
				}
			}
		}
	}

	void Screen::draw(const v2<uint32> point, const Asset& asset, const Color foreground, const Color background)
	{
		if(AssetType::Art != asset.getType()) {
			return; // We only support drawing art assets.
		}

		const auto finalX = point.x + asset.width();
		const auto finalY = point.y + asset.height();

		Pixel current{};
		current.background = background;
		current.foreground = foreground;

		for(uint32 y = point.y; y < finalY; ++y) {
			for(uint32 x = point.x; x < finalX; ++x) {
				if(y >= height() || x >= width()) {
					continue;
				}

				const auto index = (x - point.x) + asset.width() * (y - point.y);

				current.glyph.ascii = asset.getData()[index];
				draw(v2<uint32>{ x, y }, current);
			}
		}
	}

	void Screen::fill(const Rect& rectangle, const Color background, const char glyph, const Color foreground)
	{
		const auto finalX = std::min(rectangle.point.x + rectangle.size.x, width() - 1);
		const auto finalY = std::min(rectangle.point.y + rectangle.size.y, height() - 1);

		Pixel current{};
		current.background = background;
		current.foreground = foreground;
		current.glyph.ascii = glyph;

		for(uint32 y = rectangle.point.y; y < finalY; ++y) {
			for(uint32 x = rectangle.point.x; x < finalX; ++x) {
				draw(v2<uint32>{ x, y }, current);
			}
		}
	}
}