#include <ftxui/component/loop.hpp>

#include "conquest/engine/sdl/event/EventPoller.h"
#include "conquest/engine/sdl/event/EventConversion.h"
#include "conquest/engine/console/Console.h"

#include "conquest/engine/SyncLoop.h"


#ifdef _WIN32

#include <windows.h>

#endif


namespace conquest::engine {

namespace {

void disableConsoleInput()
{
#ifdef _WIN32
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);
#endif
}

}

SyncLoop::SyncLoop(ftxui::ScreenInteractive *screen)
	: m_Screen(screen)
{}

void SyncLoop::run(ftxui::Component root) const
{
	ftxui::Loop loop(m_Screen, std::move(root));
	disableConsoleInput();

	sdl::EventPoller poller;
	while(!poller.exitRequested()) {
		while(auto event = poller.poll()) {
			if(auto ftxuiEvent = sdl::convertEvent(event.value()); ftxuiEvent.has_value()) {
				m_Screen->PostEvent(ftxuiEvent.value());
			}
		}

		loop.RunOnce();
		console::g_Instance->present();
	}
}


}
