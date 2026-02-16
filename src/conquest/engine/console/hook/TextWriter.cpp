#include "conquest/common/Finally.h"

#include "conquest/engine/console/hook/TextWriter.h"


namespace conquest::engine::console {

TextWriter::TextWriter(ConsoleScreenBuffer& screen, SpriteMapper& mapper)
	: m_Screen(screen)
	, m_SpriteMapper(mapper)
	, m_Cursor(0, 0)
	, m_Foreground{ DEFAULT_FOREGROUND }
	, m_Background{ DEFAULT_BACKGROUND }
{}

void TextWriter::newline()
{
	m_Cursor.y++;
	carriageReturn();
}

void TextWriter::carriageReturn()
{
	m_Cursor.x = 0;
}

void TextWriter::write(const SpriteMapper::SpriteIndex_t character)
{
	if(m_Cursor.x >= m_Screen.width() || m_Cursor.y >= m_Screen.height()) {
		return;
	}

	auto incrementGuard = finally([&] { ++m_Cursor.x; });
	m_Screen.setGlyph(m_Cursor.x, m_Cursor.y, { m_SpriteMapper.fetch(character), m_Foreground, m_Background });
}

void TextWriter::eraseInLine(const EraseMode mode)
{
	if(m_Cursor.y >= m_Screen.height()) {
		return;
	}

	const auto blankGlyph = Glyph{ m_SpriteMapper.fetch(' '), DEFAULT_FOREGROUND, DEFAULT_BACKGROUND };

	size_t start = 0;
	size_t end = m_Screen.width();

	switch(mode) {
	case EraseMode::CursorToEnd:
		start = m_Cursor.x;
		break;
	case EraseMode::StartToCursor:
		end = m_Cursor.x + 1;
		break;
	case EraseMode::EntireLine:
		break;
	}

	for(size_t x = start; x < end; ++x) {
		m_Screen.setGlyph(x, m_Cursor.y, blankGlyph);
	}
}

void TextWriter::setForeground(const Color &color)
{
	m_Foreground = color;
}

void TextWriter::setBackground(const Color &color)
{
	m_Background = color;
}

const Vec2<uint32_t> & TextWriter::cursor() const
{
	return m_Cursor;
}

Vec2<uint32_t> & TextWriter::cursor()
{
	return m_Cursor;
}

}
