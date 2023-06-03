#pragma once

#include <cstdint>
#include <string>

#include "types.h"
#include "assets.h"

Pixel color(Color fg, Color bg = Color::Black, bool bold = false, bool backBold = false);

Pixel color_by_name(const std::string&);


class Screen {
public:
	static Pixel WHITE;

	Screen();
	~Screen();
	
	void draw(int x, int y, char c, Pixel color);
	void draw(int x, int y, char c, Color _color) {
		draw(x, y, c, color(_color));
	}

	void draw(int x, int y, const std::string& str, Pixel color, int max = -1);
	void draw(int x, int y, const std::string& str, Color _color, int max = -1) {
		draw(x, y, str, color(_color), max);
	}

	void clear(void);

	int width() const { return m_Width; }
	int height() const { return m_Height; }
	
	// Pattern is a 3 char long string where [0] is corner, [1] is horizontal edge and [2] is vertical edge
	void drawRect(int x, int y, int w, int h, const std::string& pattern, Pixel color);
	void drawRect(int x, int y, int w, int h, const std::string& pattern, Color _color) {
		drawRect(x, y, w, h, pattern, color(_color));
	}
	void drawRect(int x, int y, int w, int h, char c, Pixel color);
	void drawRect(int x, int y, int w, int h, char c, Color _color) {
		drawRect(x, y, w, h, c, color(_color));
	}
	void fillRect(int x, int y, int w, int h, char c, Pixel color);
	void fillRect(int x, int y, int w, int h, char c, Color _color) {
		fillRect(x, y, w, h, c, color(_color));
	}
	
	void drawAsset(const Asset& asset, int x, int y, Pixel color);
	void drawAsset(const Asset& asset, int x, int y, Color _color) {
		drawAsset(asset, x, y, color(_color));
	}

	/* Draws the buffer to the screen */
	void print(void) const;
private:
	char* m_Buffer;
	int m_Width, m_Height;
	bool m_Colors;
};
