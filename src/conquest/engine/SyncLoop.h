#pragma once

#include <ftxui/component/screen_interactive.hpp>


namespace conquest::engine {

class SyncLoop
{
public:
	explicit SyncLoop(ftxui::ScreenInteractive *screen);

	void run(ftxui::Component root) const;

private:
	ftxui::ScreenInteractive *m_Screen = nullptr;
};

}
