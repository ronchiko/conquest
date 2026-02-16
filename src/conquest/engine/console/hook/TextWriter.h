#pragma once

#include "conquest/data/Vec.h"
#include "conquest/engine/console/screen/ConsoleScreenBuffer.h"
#include "conquest/engine/console/assets/SpriteMapper.h"
#include "conquest/engine/console/screen/Color.h"


namespace conquest::engine::console {

inline constexpr Color DEFAULT_FOREGROUND = { 255, 255, 255 };
inline constexpr Color DEFAULT_BACKGROUND = { 0, 0, 0 };

enum class EraseMode : uint32_t
{
	CursorToEnd = 0,
	StartToCursor = 1,
	EntireLine = 2,
};

class TextWriter
{
public:
	explicit TextWriter(ConsoleScreenBuffer& screen, SpriteMapper& mapper);

    /**
        Drops the writer to a new line.
     */
    void newline();

	/**
		Returns the cursor to the start of the current line.
	 */
	void carriageReturn();

	void write(SpriteMapper::SpriteIndex_t character);

	/**
		Erases part of the current line.
	 */
	void eraseInLine(EraseMode mode);

	void setForeground(const Color& color);
	void setBackground(const Color& color);

	const Vec2<uint32_t>& cursor() const;
	Vec2<uint32_t>& cursor();

private:
	ConsoleScreenBuffer& m_Screen;
	SpriteMapper& m_SpriteMapper;

	Vec2<uint32_t> m_Cursor;
	Color m_Foreground;
	Color m_Background;
};

}
