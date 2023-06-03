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

#if false

constexpr int COLOR_ID_MASK = 0x77;
constexpr int BOLD_MASK = 0x80;

static std::map<Color, int> COLOR_IN_NCURSES{ { Color::Black, COLOR_BLACK },	 { Color::Red, COLOR_RED },
											  { Color::Green, COLOR_GREEN },	 { Color::Yellow, COLOR_YELLOW },
											  { Color::Blue, COLOR_BLUE },		 { Color::Cyan, COLOR_CYAN },
											  { Color::Magenta, COLOR_MAGENTA }, { Color::Grey, COLOR_WHITE } };


static std::map<std::string, Color> gColorMap = { { "BLACK", Color::Black },	 { "RED", Color::Red },
												  { "GREEN", Color::Green },	 { "YELLOW", Color::Yellow },
												  { "BLUE", Color::Blue },		 { "CYAN", Color::Cyan },
												  { "MAGENTA", Color::Magenta }, { "WHITE", Color::White } };

Pixel Screen::WHITE = color(Color::White);

Pixel color(Color fg, Color bg, bool fgB, bool bgB)
{
	int32_t fgc = COLOR_IN_NCURSES[fg] - COLOR_BLACK;
	int32_t bgc = COLOR_IN_NCURSES[bg] - COLOR_BLACK;

	return fgc | (bgc << 4);
}

static Color get_color_or_white(const std::string& name)
{
	if(gColorMap.find(name) == gColorMap.end()) {
		std::cout << "No color named " << name << "\n";
		return Color::White;
	}
	return gColorMap[name];
}

Pixel color_by_name(const std::string& name)
{
	if(name.rfind("BOLD_", 0) == 0) {
		return color(get_color_or_white(name.substr(5)), Color::Black, TRUE);
	}

	return color(get_color_or_white(name));
}

Screen::Screen()
	: m_Buffer(nullptr)
	, m_Width(0)
	, m_Height(0)
	, m_Colors(false)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	if(has_colors()) { // Initialize colors
		start_color();
		m_Colors = true;
		for(Color i = Color::Black; i <= Color::White; i = (Color)((int)i + 1)) {
			for(Color j = Color::Black; j <= Color::White; j = (Color)((int)j + 1)) {
				int id = color(i, j);
				init_pair(id, COLOR_IN_NCURSES[i], COLOR_IN_NCURSES[j]);
			}
		}
	}

	m_Width = COLS;
	m_Height = LINES;

	m_Buffer = new char[m_Width * m_Height * 2];
	for(int i = 0; i < m_Width; i++) {
		for(int j = 0; j < m_Height; j++) {
			int x = (i + j * m_Width) << 1;
			m_Buffer[x + 1] = ' ';
			m_Buffer[x] = (short)color(Color::White, Color::Black);
		}
	}
}

Screen::~Screen()
{
	delete[] m_Buffer;
	endwin();
}

void Screen::draw(int x, int y, char c, Pixel color)
{
	if(x < 0 || x >= m_Width || y < 0 || y >= m_Height) {
		return;
	}

	int i = (x + y * m_Width) << 1;
	m_Buffer[i + 1] = c;
	m_Buffer[i] = (char)color;
}

void Screen::draw(int x, int y, const std::string& str, Pixel color, int maxChars)
{
	int chars = 0;
	for(auto& c : str) {
		if(x < 0 || x >= m_Width || y < 0 || y >= m_Height || (maxChars >= 0 && maxChars <= chars)) {
			break;
		}
		int i = (x + y * m_Width) << 1;
		m_Buffer[i + 1] = c;
		m_Buffer[i] = (char)color;
		x++;
		chars++;
	}
}

void Screen::drawRect(int x, int y, int w, int h, char c, Pixel color)
{
	for(int i = std::max(0, x); i < x + w && i < m_Width; i++) {
		for(int j = std::max(y, 0); j < y + h && j < m_Height; j++) {
			// Draw only the edges
			if(i == x || i == x + w - 1 || j == y || j == y + h - 1) {
				draw(i, j, c, color);
			}
		}
	}
}

void Screen::fillRect(int x, int y, int w, int h, char c, Pixel color)
{
	for(int i = std::max(x, 0); i < x + w && i < m_Width; i++) {
		for(int j = std::max(y, 0); j < y + h && j < m_Height; j++) {
			draw(i, j, c, color);
		}
	}
}

void Screen::drawRect(int x, int y, int w, int h, const std::string& pattern, Pixel color)
{
	if(pattern.length() != 3) {
		return;
	}

	for(int i = std::max(x, 0); i < x + w && i < m_Width; i++) {
		for(int j = std::max(y, 0); j < y + h && j < m_Height; j++) {
			if((i == x || i == x + w - 1) && (j == y || j == y + h - 1)) {
				draw(i, j, pattern[0], color);
			} else if(i == x || i == x + w - 1) {
				draw(i, j, pattern[2], color);
			} else if(j == y || j == y + h - 1) {
				draw(i, j, pattern[1], color);
			}
		}
	}
}

void Screen::drawAsset(const Asset& asset, int x, int y, Pixel color)
{
	// Only draw art assets
	if(asset.getType() != ASSET_ART) {
		return;
	}

	for(int ix = std::max(x, 0); ix < x + asset.width() && ix < m_Width; ix++) {
		for(int iy = std::max(y, 0); iy < y + asset.height() && iy < m_Height; iy++) {
			draw(ix, iy, asset.getData()[(ix - x) + (iy - y) * asset.width()], color);
		}
	}
}

void Screen::print(void) const
{
	char lLastColor = COLOR_ID_MASK;
	if(m_Colors) {
		attron(COLOR_PAIR(lLastColor));
	}
	for(int i = 0; i < m_Width; i++) {
		for(int j = 0; j < m_Height; j++) {
			int index = (i + j * m_Width) << 1;
			char c = m_Buffer[index + 1];
			// Read and apply the correct color for this character
			char color = m_Buffer[index];
			if(color != lLastColor && m_Colors) {
				attroff(COLOR_PAIR(lLastColor & COLOR_ID_MASK));
				attron(COLOR_PAIR(color & COLOR_ID_MASK));
				lLastColor = color;

				if(color & BOLD_MASK) {
					attron(A_BOLD);
				} else {
					attroff(A_BOLD);
				}
			}
			// Print the character to the screen
			mvaddch(j, i, c);
		}
	}
	refresh();
}

#endif
#endif
