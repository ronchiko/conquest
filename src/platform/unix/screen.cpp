#if defined(unix) || defined(__unix__)
#include "conquest/graphics/unix/screen.h"

#include <unordered_map>

#include <ncurses.h>

#include "conquest/error.h"

namespace conquest {

	static const std::unordered_map<Color, int> COLOR_IN_NCURSES{
		{ Color::Black, COLOR_BLACK },	   
		{ Color::Red, COLOR_RED },	 
		{ Color::Green, COLOR_GREEN },
		{ Color::Yellow, COLOR_YELLOW },   
		{ Color::Blue, COLOR_BLUE },	 
		{ Color::Cyan, COLOR_CYAN },
		{ Color::Magenta, COLOR_MAGENTA }, 
		{ Color::Grey, COLOR_WHITE },
	};

	// clang-format off
	/**
	 * Gets color id of 2 colors. Doesn't keep track of bright colors because ncurses treats
	 * brighteness as an attributes and not a diffrenent color.
	 * 
	 * @param foreground - The foreground color.
	 * @param background - The background color.
 	 */
	// clang-format on
	static byte getColorId(const uint32 foreground, const uint32 background)
	{
		union
		{
			byte id;

			struct
			{
				byte foreground:4;
				byte background:4;
			};
		} value;
		
		// Putting the values while discarding their BRIGHT_BIT
		value.background = static_cast<byte>(background & ~BRIGHT_BIT);
		value.foreground = static_cast<byte>(foreground & ~BRIGHT_BIT);


		// We force the colors into a byte because pair indecies are
		// usually limited to 256.
		return value.id;
	}

	/**
	 * Initializes NCurses.h and gets the console size.
	 */
	static v2<uint32> initializeNCursesAndGetConsoleSize()
	{
		// Initialize ncurses
		initializeNCurses();
		
		return v2<uint32>{ static_cast<uint32>(COLS), static_cast<uint32>(LINES) };
	}

	ConsoleScreen::ConsoleScreen(const v2<uint32> minimumSize)
		: mSize(initializeNCursesAndGetConsoleSize())
		, mScreenBuffer(mSize.x * mSize.y)
	{
		if(mSize.x < minimumSize.x || mSize.y < minimumSize.y) {
			throw ConquestError("Console size is too small.");
		}

		// Ensure the console supports colors.
		if(!has_colors()) {
			throw ConquestError("Console doesn't support colors.");
		}

		// Initialize the colors for ncurses.
		start_color();
		const auto firstColor = static_cast<uint32>(Color::Black);
		const auto lastColor = static_cast<uint32>(Color::Grey);
		
		for(uint32 i = firstColor; i <= lastColor; ++i) {
			for(uint32 j = firstColor; j <= lastColor; ++j) {
				const short id = getColorId(i, j);
				init_pair(id,
					  COLOR_IN_NCURSES.at(static_cast<Color>(i)), 
					  COLOR_IN_NCURSES.at(static_cast<Color>(j)));
			}
		}

		for(uint32 y = 0; y < mSize.y; ++y) {
			for(uint32 x = 0; x < mSize.x; ++x) {
				mScreenBuffer[x + mSize.x * y].foreground = Color::White;
				mScreenBuffer[x + mSize.x * y].background = Color::Black;
				mScreenBuffer[x + mSize.x * y].glyph.ascii = ' ';
			}
		}
	}
	
	void ConsoleScreen::draw(const v2<uint32> point, const Pixel& pixel)
	{
		if(point.x >= mSize.x || point.y >= mSize.y) {
			throw ConquestError("Attempted to draw out of bounds");
		}

		mScreenBuffer[point.x + mSize.x * point.y] = pixel;
	}

	void ConsoleScreen::flush()
	{
		// A couple of differences from windows to unix in drawing.
		// First is that we dont have bright background color, this is
		// because the A_BLINK attribute is unrelyable.
		// The other diffence is that bright colors are bold an not
		// nessecarly a different color that their darker counter parts.
		for(uint32 y = 0; y < mSize.y; ++y) {
			for(uint32 x = 0; x < mSize.x; ++x) {
				const auto& pixel = mScreenBuffer[x + mSize.x * y];
				const auto foreground = static_cast<uint32>(pixel.foreground);
				const auto background = static_cast<uint32>(pixel.background);

				const auto colorId = getColorId(foreground, background);
				
				const bool useBold = foreground & BRIGHT_BIT;
				const bool useBlink = background & BRIGHT_BIT;
				
				NCursesAttriubte attribute1(COLOR_PAIR(colorId));
				NCursesAttriubte attribute2(useBold ? A_BOLD: 0);
				mvaddch(y, x, pixel.glyph.ascii);
			}
		}
		refresh();
	}
}

#endif
