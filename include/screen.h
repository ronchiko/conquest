#pragma once

#include <ncurses.h>
#include <string>
#include "assets.h"

int color(int fg, int bg = COLOR_BLACK, int bold = 0);

int color_by_name(const std::string&);

class Screen {
public:
	Screen();
	~Screen();
	
	void draw(int x, int y, char c, int color = COLOR_WHITE);
	void draw(int x, int y, const std::string& str, int color = COLOR_WHITE, int max = -1);

	void clear(void);

	int width() const { return m_Width; }
	int height() const { return m_Height; }
	
	// Pattern is a 3 char long string where [0] is corner, [1] is horizontal edge and [2] is vertical edge
	void drawRect(int x, int y, int w, int h, const std::string& pattern, int color = COLOR_WHITE);
	void drawRect(int x, int y, int w, int h, char c, int color = COLOR_WHITE);
	void fillRect(int x, int y, int w, int h, char c, int color = COLOR_WHITE);
	
	void drawAsset(const Asset& asset, int x, int y, int color = COLOR_WHITE);

	/* Draws the buffer to the screen */
	void print(void) const;
private:
	char* m_Buffer;
	int m_Width, m_Height;
	bool m_Colors;
};
