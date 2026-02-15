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
