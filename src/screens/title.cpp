
#include "conquest/scenes/title.h"

#include <string>

#include "conquest/assets.h"
#include "conquest/input/input.h"

namespace conquest {

	const std::string OPTIONS[3] = { "Start Game", "Continue", "Exit" };

	TitleScreenResult startTitle(Screen& screen)
	{
		Asset titleArt("art/title.txt", ASSET_ART);

		uint32 pointer = 0;
		const uint32 origin = (screen.width() - titleArt.width()) >> 1;
		while(1) {
			screen.draw(v2<uint32>{ origin, 5 }, titleArt, Color::BrightYellow, Color::Black);

			uint32 startY = 20;
			for(uint32 i = 0; i < 3; i++) {
				const Color foregroundColor = pointer == i ? Color::BrightBlue : Color::White;
				uint32 x = static_cast<uint32>((screen.width() - OPTIONS[i].length()) >> 1);
				screen.draw(v2<uint32>{ x, startY }, OPTIONS[i], foregroundColor, Color::Black);
				startY += 2;
			}

			screen.flush();
			const auto input = Input::get();
			switch(input.type) {
			case InputType::Down:
				pointer++;
				break;
			case InputType::Up:
				pointer--;
				break;
			case InputType::Enter: // Input
				return static_cast<TitleScreenResult>(pointer);
			}

			if(pointer < 0) {
				pointer = 0;
			}
			if(pointer > 2) {
				pointer = 2;
			}
		}

		return TitleScreenResult::NewGame;
	}
}