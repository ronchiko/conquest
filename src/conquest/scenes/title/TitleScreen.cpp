#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

#include "conquest/engine/window/Manager.h"
#include "conquest/engine/window/styles/Button.h"
#include "conquest/data/Package.h"

#include "conquest/scenes/title/TitleScreen.h"


namespace conquest::scenes::title {

namespace
{

ftxui::Component createMainComponent(window::Context<Result>& context)
{
	auto choiceMenu = ftxui::Container::Vertical({
		ftxui::Button("New Game", [&] { context.newGame(); context.pop(); }, window::style::ANIMATED_BUTTON),
		ftxui::Button("Load Game", [&] {
			context.push("no-saves");
		}, window::style::ANIMATED_BUTTON),
		ftxui::Button("Exit", [&] { context.exit(); context.pop(/* full = */ true);}, window::style::ANIMATED_BUTTON),
	}) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 40) | ftxui::center;

	auto titleImage = ftxui::Renderer([] {
	  return ftxui::vbox({
		  ftxui::text("OOOO OOOO OO O OOOO  O  O OOOO OOOO OOO"),
		  ftxui::text("O    O  O O OO O  O  O  O Ooo  *Oo   O "),
		  ftxui::text("OOOO OOOO O  O OOOOo OOOO OOOO OOOO  O "),
		  ftxui::text("               ADVENTURE               "),
	  }) | ftxui::color(ftxui::Color::Yellow);
	}) | ftxui::center;

	auto root = ftxui::Container::Vertical({
		ftxui::Renderer([] { return ftxui::filler() | size(ftxui::HEIGHT, ftxui::EQUAL, 2); }),
		titleImage,
		ftxui::Renderer([] { return ftxui::filler(); }),
		choiceMenu,
		ftxui::Renderer([] { return ftxui::filler() | size(ftxui::HEIGHT, ftxui::EQUAL, 4); }),
	});

	return root;
}

ftxui::Component createNoSavesModal(window::Context<Result>& context)
{
	return ftxui::Container::Vertical({
		ftxui::Renderer([] { return ftxui::text("Not Implemented") | ftxui::color(ftxui::Color::Red3Bis); }),
		ftxui::Renderer([] { return ftxui::filler() | size(ftxui::HEIGHT, ftxui::EQUAL, 2); }),
		ftxui::Button("Ok", [&] { context.pop(); }, ftxui::ButtonOption::Border()),
	})  | ftxui::borderEmpty| ftxui::border | ftxui::center;
}

}

Result show()
{
	auto manager = window::manager<Result>(createMainComponent)
		.add("no-saves", createNoSavesModal);

	return manager.start();
}

}
