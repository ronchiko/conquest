#if defined(_WIN32) // Only compile this file if we are building to windows

#include "conquest/graphics/windows/screen.h"

#include <unordered_map>

#include "conquest/error.h"

#define FOREGROUND_YELLOW  (FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN	   (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_MAGENTA (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FOREGROUND_WHITE   (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

#define BACKGROUND_YELLOW  (BACKGROUND_RED | BACKGROUND_GREEN)
#define BACKGROUND_CYAN	   (BACKGROUND_RED | BACKGROUND_BLUE)
#define BACKGROUND_MAGENTA (BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BACKGROUND_WHITE   (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)

static const std::unordered_map<conquest::Color, DWORD> FOREGROUND_COLORS{
	{ conquest::Color::Black, 0 },
	// Darker colors
	{ conquest::Color::Red, FOREGROUND_RED },
	{ conquest::Color::Green, FOREGROUND_GREEN },
	{ conquest::Color::Yellow, FOREGROUND_YELLOW },
	{ conquest::Color::Blue, FOREGROUND_BLUE },
	{ conquest::Color::Cyan, FOREGROUND_CYAN },
	{ conquest::Color::Magenta, FOREGROUND_MAGENTA },
	{ conquest::Color::Grey, FOREGROUND_WHITE }, // Not intense white is grey

	// Bright color
	{ conquest::Color::BrightRed, FOREGROUND_RED | FOREGROUND_INTENSITY },
	{ conquest::Color::BrightGreen, FOREGROUND_GREEN | FOREGROUND_INTENSITY },
	{ conquest::Color::BrightYellow, FOREGROUND_YELLOW | FOREGROUND_INTENSITY },
	{ conquest::Color::BrightBlue, FOREGROUND_BLUE | FOREGROUND_INTENSITY },
	{ conquest::Color::BrightCyan, FOREGROUND_CYAN | FOREGROUND_INTENSITY },
	{ conquest::Color::BrightMagenta, FOREGROUND_MAGENTA | FOREGROUND_INTENSITY },
	{ conquest::Color::White, FOREGROUND_WHITE | FOREGROUND_INTENSITY },

};

static const std::unordered_map<conquest::Color, DWORD> BACKGROUND_COLORS{
	{ conquest::Color::Black, 0 },

	// Darker colors
	{ conquest::Color::Red, BACKGROUND_RED },
	{ conquest::Color::Green, BACKGROUND_GREEN },
	{ conquest::Color::Yellow, BACKGROUND_YELLOW },
	{ conquest::Color::Blue, BACKGROUND_BLUE },
	{ conquest::Color::Cyan, BACKGROUND_CYAN },
	{ conquest::Color::Magenta, BACKGROUND_MAGENTA },
	{ conquest::Color::Grey, BACKGROUND_WHITE }, // Not intense white is grey

	// Bright color
	{ conquest::Color::BrightRed, BACKGROUND_RED | BACKGROUND_INTENSITY },
	{ conquest::Color::BrightGreen, BACKGROUND_GREEN | BACKGROUND_INTENSITY },
	{ conquest::Color::BrightYellow, BACKGROUND_YELLOW | BACKGROUND_INTENSITY },
	{ conquest::Color::BrightBlue, BACKGROUND_BLUE | BACKGROUND_INTENSITY },
	{ conquest::Color::BrightCyan, BACKGROUND_CYAN | BACKGROUND_INTENSITY },
	{ conquest::Color::BrightMagenta, BACKGROUND_MAGENTA | BACKGROUND_INTENSITY },
	{ conquest::Color::White, BACKGROUND_WHITE | BACKGROUND_INTENSITY },

};

namespace conquest {
	/**
	 * Acquires the console handle for the screen.
	 *
	 * @return HANDLE
	 */
	static HANDLE acquireConsoleHandle()
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if(INVALID_HANDLE_VALUE == handle || nullptr == handle) {
			throw win32::WindowsError("Failed to get console output handle!");
		}
		return handle;
	}

	/**
	 * Get the console screen size from an handle.
	 *
	 * @param handle - The handle to get the size from.
	 * @return v2<uint32>
	 */
	static v2<uint32> getConsoleScreenSize(const win32::Handle& handle)
	{
		CONSOLE_SCREEN_BUFFER_INFOEX screenInformation;
		ZeroMemory(&screenInformation, sizeof(screenInformation));
		screenInformation.cbSize = sizeof(screenInformation);

		if(!GetConsoleScreenBufferInfoEx(handle, &screenInformation)) {
			throw win32::WindowsError("Failed to query screen buffer information.");
		}

		return { static_cast<uint32>(screenInformation.dwSize.X), static_cast<uint32>(screenInformation.dwSize.Y) };
	}

	/**
	 * Converts a pixel to a CHAR_INFO which is what the screen buffer draws at the end.
	 *
	 * @param pixel - The pixel to draw.
	 * @return CHAR_INFO
	 */
	static CHAR_INFO convertToCharInfo(const Pixel& pixel)
	{
		CHAR_INFO character{};
		character.Char.AsciiChar = pixel.glyph.ascii;
		character.Attributes = static_cast<WORD>(FOREGROUND_COLORS.at(pixel.foreground) |
												 BACKGROUND_COLORS.at(pixel.background));
		return character;
	}

	ConsoleScreen::ConsoleScreen(const v2<uint32> minimumSize)
		: mScreenHandle(acquireConsoleHandle())
		, mSize(getConsoleScreenSize(mScreenHandle))
		, mScreenBuffer(mSize.x * mSize.y)
	{
		if(mSize.x < minimumSize.x || mSize.y < minimumSize.y) {
			throw ConquestError("Console screen is too small.");
		}

		const Pixel empty{ ' ', Color::Black, Color::Black };
		for(uint32 y = 0; y < mSize.y; ++y) {
			for (uint32 x = 0; x < mSize.x; ++x) {
				draw(v2<uint32>{ x, y }, empty);
			}
		}
	}

	void ConsoleScreen::draw(v2<uint32> point, const Pixel& pixel)
	{
		if(point.x >= mSize.x || point.y >= mSize.y) {
			throw ConquestError("Attempted to draw out of bounds");
		}

		mScreenBuffer[point.x + mSize.x * point.y] = convertToCharInfo(pixel);
	}

	void ConsoleScreen::flush()
	{
		COORD scales{ static_cast<SHORT>(mSize.x), static_cast<SHORT>(mSize.y) };
		SMALL_RECT drawRegion{ 0, 0, static_cast<SHORT>(mSize.x), static_cast<SHORT>(mSize.y) };

		WriteConsoleOutputA(mScreenHandle, mScreenBuffer.data(), scales, { 0, 0 }, &drawRegion);
	}
}

#endif