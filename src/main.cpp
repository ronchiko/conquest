
#include <iostream>
#include <sstream>

#include "magic.h"
#include "weapons.h"
#include "moves.h"
#include "character.h"
#include "screen.h"
#include "title.h"
#include "random.h"

Random::_rand_instance Random::instance;

void clean(void) {
	for(Element* elem : Element::all) delete elem;	
	for(Proffesion* prof : Proffesion::all) delete prof;
	for(Race* race : Race::all) delete race;
	for(Item* item : Item::all) delete item;
}

int main(void) {
	Element::load();
	Moves::load();	
	
	Item::load();
	Equipment::load();	
	Weapon::load();

	Proffesion::load();
	Race::load();
	
	getchar();	
	Screen screen;

	TitleResult_e titleResult = start_title(screen);
		
	if(titleResult == TITLE_QUIT) {
		return 0;
	}

	Character* player = create_character(screen);
	
	if(player == nullptr) {
		clean();
		std::cout << "\033[31;1m[ERROR]\033[0m: Failed to create character\n";
		return -1;
	}

	delete player;
	clean();
	return 0;
}
