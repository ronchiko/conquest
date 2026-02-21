#include <SDL3/SDL.h>
#include <iostream>

#include "ftxui/component/component.hpp"

#include "conquest/scenes/title/TitleScreen.h"
#include "conquest/scenes/newGame/Intro.h"
#include "conquest/scenes/newGame/CharacterCreator.h"
#include "conquest/data/Package.h"
#include "conquest/log/Log.h"
#include "conquest/log/sink/LogFileSink.h"
#include "conquest/engine/console/Console.h"

#include <random>

void initializeLogger()
{
	conquest::log::initialize();
	conquest::log::addSink(std::make_shared<conquest::log::sink::LogFileSink>("log.txt"));
}

int main(void)
{
	using namespace conquest;

	auto sdlGuard = conquest::finally([] { SDL_Quit(); });

	initializeLogger();
	conquest::log::info("Loading conquest");

	package::load(R"(C:\Dev\conquest\assets)", true);

	engine::console::g_Instance = std::make_unique<engine::console::Console>(80, 40);
	engine::console::g_Instance->mapper().loadMapping("art/textures/font/font.toml");

	const auto titleScreenChoice = scenes::title::show();
	if(scenes::title::ResultType::Exit == titleScreenChoice.type) {
		log::info("Exiting game");
		return -1;
	}

	auto character = scenes::newGame::creator::createCharacter();
	return 0;
}
