
#include "conquest/scenes/title.h"

#include <string>

#include "conquest/graphics/asset.h"
#include "conquest/graphics/widgets/container.h"
#include "conquest/graphics/widgets/image.h"
#include "conquest/graphics/widgets/selection.h"
#include "conquest/input/input.h"

namespace conquest {

	TitleScreenResult startTitle(Screen& screen)
	{
		Asset titleArt = Asset::load("art/title.txt", AssetType::Art);

		gui::HorizontalContainer host;

		gui::SelectionSettings style;
		style.spacing = 2;

		gui::Selection selection(Area{ screen.width() / 2 - 10, 10, 20, 20 },
								 { "Start Game", "Continue", "Exit" },
								 style);
		host.add(gui::Focus(selection));

		gui::Image title(v2<uint32>{ (screen.width() - titleArt.width()) / 2, 5 }, "art/title.txt", Color::BrightYellow);
		host.add(title);

		for(;;) {
			host.draw(screen, true);
			screen.flush();

			if(const auto input = Input::getAsync()) {
				if(!host.sendInput(input) && InputType::Enter == input.type) {
					return static_cast<TitleScreenResult>(selection.current());
				}
			}
		}


		return TitleScreenResult::NewGame;
	}
}