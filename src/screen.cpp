
#include <iostream>
#include <map>

#include "screen.h"

constexpr int COLOR_ID_MASK = 0x77;
constexpr int BOLD_MASK = 0x80;

static std::map<std::string, int> gColorMap = {
	{"BLACK", color(COLOR_BLACK) },
	{"RED", color(COLOR_RED) },
	{"GREEN", color(COLOR_GREEN) },
	{"YELLOW", color(COLOR_YELLOW) },
	{"BLUE", color(COLOR_BLUE) },
	{"CYAN", color(COLOR_CYAN) },
	{"MAGENTA", color(COLOR_MAGENTA) },
	{"WHITE", color(COLOR_WHITE) }
};

int color(int fg, int bg, int fgB) {
	return (fg - COLOR_BLACK) | ((bg - COLOR_BLACK) << 4) | (fgB << 7);
}

int color_by_name(const std::string& name){
	if(name.rfind("BOLD_", 0) == 0){
		return color(color_by_name(name.substr(5)), COLOR_BLACK, TRUE);
	}
	// If the color name is not valid
	if(gColorMap.find(name) == gColorMap.end()) {
		std::cout << "No color named " << name << "\n";
		return COLOR_WHITE;
	}
	return gColorMap[name];
}

Screen::Screen() : m_Buffer(nullptr), m_Width(0), m_Height(0), m_Colors(false) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	if(has_colors()) {
		start_color();
		m_Colors = true;
		for(int i = COLOR_BLACK; i <= COLOR_WHITE; i++){
			for(int j = COLOR_BLACK; j <= COLOR_WHITE; j++){
				int id = color(i, j);
				init_pair(id, i, j);
			}
		}
	}

	m_Width = COLS;
	m_Height = LINES;

	m_Buffer = new char[m_Width * m_Height * 2];
	for(int i = 0; i < m_Width; i++){
		for(int j = 0; j < m_Height; j++){
			int x = (i + j * m_Width) << 1;
			m_Buffer[x + 1] = ' ';
			m_Buffer[x] = (short)color(COLOR_WHITE, COLOR_BLACK);
		}
	}
}

Screen::~Screen() {
	delete[] m_Buffer;
	endwin();
}

void Screen::draw(int x, int y, char c, int color) {
	if(x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;
	
	int i = (x + y * m_Width) << 1;
	m_Buffer[i + 1] = c;
	m_Buffer[i] = (char)color;
}

void Screen::draw(int x, int y, const std::string& str, int color, int maxChars) {
	int chars = 0;
	for(auto& c : str) {
		if(x < 0 || x >= m_Width || y < 0 || y >= m_Height || (maxChars >= 0 && maxChars <= chars)) break;
		int i = (x + y * m_Width) << 1;
		m_Buffer[i + 1] = c;
		m_Buffer[i] = (char)color;
		x++;
		chars++;
	}
}

void Screen::drawRect(int x, int y, int w, int h, char c, int color) {
	for(int i = std::max(0, x); i < x + w && i < m_Width; i++){
		for(int j = std::max(y, 0); j < y + h && j < m_Height; j++){
			// Draw only the edges
			if(i == x || i == x + w - 1 || j == y || j == y + h - 1) draw(i, j, c, color);
		}
	}	
}

void Screen::fillRect(int x, int y, int w, int h, char c, int color) {
	for(int i = std::max(x, 0); i < x + w && i < m_Width; i++){
		for(int j = std::max(y, 0); j < y + h && j < m_Height; j++){
			draw(i, j, c, color);
		}
	}
}

void Screen::drawRect(int x, int y, int w, int h, const std::string& pattern, int color) {
	if(pattern.length() != 3) return;

	for(int i = std::max(x, 0); i < x + w && i < m_Width; i++){
		for(int j = std::max(y, 0); j < y + h && j < m_Height; j++){
			if((i == x || i == x + w - 1) && (j == y || j == y + h - 1)) draw(i, j, pattern[0], color);
			else if(i == x || i == x + w - 1) draw(i, j, pattern[2], color);
			else if(j == y || j == y + h - 1) draw(i, j, pattern[1], color);	
		}
	}
}

void Screen::drawAsset(const Asset& asset, int x, int y, int color){
	// Only draw art assets
	if(asset.getType() != ASSET_ART) {
		return;
	}

	for(int ix = std::max(x, 0); ix < x + asset.width() && ix < m_Width; ix++){
		for(int iy = std::max(y, 0); iy < y + asset.height() && iy < m_Height; iy++){
			draw(ix, iy, asset.getData()[(ix - x)+ (iy - y) * asset.width()], color);
		}
	}
}	

void Screen::print(void) const {
	char lLastColor = COLOR_ID_MASK;
	if(m_Colors) attron(COLOR_PAIR(lLastColor));
	for(int i = 0; i < m_Width; i++){
		for(int j = 0; j < m_Height; j++){
			int index = (i + j * m_Width) << 1;
			char c = m_Buffer[index + 1];
			// Read and apply the correct color for this character
			char color = m_Buffer[index];
			if(color != lLastColor && m_Colors){
				attroff(COLOR_PAIR(lLastColor & COLOR_ID_MASK));
				attron(COLOR_PAIR(color & COLOR_ID_MASK));
				lLastColor = color;

				if(color & BOLD_MASK) attron(A_BOLD);
				else attroff(A_BOLD);
			}
			// Print the character to the screen
			mvaddch(j, i, c);
		}
	}
	refresh();
}
