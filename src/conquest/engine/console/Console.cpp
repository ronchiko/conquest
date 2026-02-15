#include "conquest/engine/console/Console.h"


namespace conquest::engine::console {

Console::Console(const size_t width, const size_t height)
	: m_Window(std::make_unique<sdl::SdlWindow>("Conquest", width * TILE_SCALE, height * TILE_SCALE))
    , m_Renderer(std::make_unique<sdl::SdlRenderer>(*m_Window))
    , m_SpriteManager(std::make_unique<SpriteManager>(*m_Renderer))
	, m_SpriteMapper(std::make_unique<SpriteMapper>(*m_SpriteManager))
	, m_ScreenBuffer(std::make_unique<ConsoleScreenBuffer>(*m_SpriteManager, width, height))
    , m_CoutHook(std::make_unique<CoutHook>(*m_ScreenBuffer, *m_SpriteMapper))
{}

SpriteMapper& Console::mapper() const
{
	return *m_SpriteMapper;
}

SpriteManager& Console::manager() const
{
	return *m_SpriteManager;
}

void Console::present() const
{
	m_ScreenBuffer->draw();
	m_Renderer->preset();
}

size_t Console::width() const
{
	return m_ScreenBuffer->width();
}

size_t Console::height() const
{
	return m_ScreenBuffer->height();
}

}
