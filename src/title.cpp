
#include "title.h"
#include "assets.h"

const std::string gOptions[3] = {
	"Start Game",
	"Continue",
	"Exit"
};

TitleResult_e start_title(Screen& screen) {
	
	Asset titleArt("art/title.txt", ASSET_ART);
	
	int pointer = 0;
	int origin = (screen.width() - titleArt.width()) >> 1;
	while(1){
		screen.drawAsset(titleArt, origin, 5, color(COLOR_YELLOW, COLOR_BLACK, TRUE));

		int startY = 20;
		for(int i = 0; i < 3; i++)
		{
			int _color = pointer == i ? COLOR_BLUE : COLOR_WHITE;
			int x = (screen.width() - gOptions[i].length()) >> 1;
			screen.draw(x, startY, gOptions[i], color(_color, COLOR_BLACK, 1));
			startY += 2;
		}	

		screen.print();
		int in = getch();
		switch(in) {
			case KEY_DOWN: pointer++; break;
			case KEY_UP: pointer--; break;
			case '\n':	// Input
				     switch(pointer){
					case 0: return TITLE_NEW_GAME;
					case 1: return TITLE_LOAD;
					case 2: return TITLE_QUIT;
				     }
				     break;
		}

		if(pointer < 0) pointer = 0;
		if(pointer > 2) pointer = 2;
	}

	return TITLE_NEW_GAME;
}
