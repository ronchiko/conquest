
#include <iostream>
#include <sstream>

#include "conquest/graphics/screen.h"
#include "conquest/input/input.h"
// #include "conquest/magic.h"
// #include "conquest/moves.h"
// #include "conquest/random.h"
// #include "conquest/scenes/character.h"
// #include "conquest/weapons.h"

#include "conquest/scenes/title.h"

void clean(void)
{
	// for(Element *elem : Element::all) {
	// 	delete elem;
	// }
	// for(Proffesion *prof : Proffesion::all) {
	// 	delete prof;
	// }
	// for(Race *race : Race::all) {
	// 	delete race;
	// }
	// for(Item *item : Item::all) {
	// 	delete item;
	// }
}

int main(void)
{
	try {
		// Element::load();
		// Moves::load();

		// Item::load();
		// Equipment::load();
		// Weapon::load();

		// Proffesion::load();
		// Race::load();
		std::cout << "Loading stuff" << std::endl;

		conquest::Input::initialize();
		auto s = conquest::Input::get();
		std::cout << static_cast<uint32_t>(s.type) << ": " << s.glyph.ascii << std::endl;

		std::cout << "Booting game" << std::endl;
		conquest::Screen screen(conquest::v2<conquest::uint32>{ 0, 0 });

		std::cout << "Loading title screen" << std::endl;
		const auto titleResult = conquest::startTitle(screen);

		if(titleResult == conquest::TitleScreenResult::Exit) {
			return 0;
		}

		// Character *player = create_character(screen);

		// if(player == nullptr) {
		// 	clean();
		// 	std::cout << "\033[31;1m[ERROR]\033[0m: Failed to create character\n";
		// 	return -1;
		// }

		// delete player;
		clean();
		return 0;
	} catch(const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}
