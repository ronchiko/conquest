#pragma once

#include "conquest/graphics/screen.h"

namespace conquest {
	enum class TitleScreenResult
	{
		NewGame,
		LoadGame,
		Exit
	};

	/**
	 * Launches the title screen, prompting the player to start a game.
	 * 
	 * @param screen - The screen to draw the title to. 
	 */
	TitleScreenResult startTitle(Screen& screen);
}