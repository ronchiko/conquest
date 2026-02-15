#pragma once

#include <cstdint>
#include <memory>

#include "conquest/engine/sdl/window/SdlWindow.h"
#include "conquest/engine/sdl/window/SdlRenderer.h"

#include "conquest/engine/console/assets/SpriteManager.h"
#include "conquest/engine/console/assets/SpriteMapper.h"
#include "conquest/engine/console/screen/ConsoleScreenBuffer.h"
#include "conquest/engine/console/hook/CoutHook.h"


namespace conquest::engine::console {

class Console
{
public:
	explicit Console(size_t width, size_t height);

	[[nodiscard]] SpriteMapper& mapper() const;
	[[nodiscard]] SpriteManager& manager() const;

	void present() const;

	[[nodiscard]] size_t width() const;
	[[nodiscard]] size_t height() const;

private:
	std::unique_ptr<sdl::SdlWindow> m_Window;
	std::unique_ptr<sdl::SdlRenderer> m_Renderer;
	std::unique_ptr<SpriteManager> m_SpriteManager;
	std::unique_ptr<SpriteMapper> m_SpriteMapper;
	std::unique_ptr<ConsoleScreenBuffer> m_ScreenBuffer;
	std::unique_ptr<CoutHook> m_CoutHook;
};

inline std::unique_ptr<Console> g_Instance = nullptr;

}
