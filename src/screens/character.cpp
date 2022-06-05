
#include <cstring>
#include <cmath>

#include "character.h"
#include "parsing.h"
#include "input.h"

enum Menu_e {
	MENU_NAME,
	MENU_PROF_RACE,
	MENU_APPLY
};

Character::Character(const char* _name) {
	memcpy(name, _name, MAX_NAME_LENGTH);
	name[MAX_NAME_LENGTH] = 0;
	for(int i = 0; i < STAT_MAX; i++){
		stats[i] = 20;
	}
	
	level = 1;
	race = nullptr;
	proffesion = nullptr;
}

void recalculate_stats(int* stats, Race* race, Proffesion* proffesion, int level){
	for(int i = 0; i < STAT_MAX; i++){
		Stats j = (Stats)i;
		// A stat cannot be less then 1
		stats[i] = std::max(20 + level * race->getGrowthRate(j) + 
					race->getStatModifer(j) + proffesion->getStatModifer(j), 1.f);
	}
}

template<class T>
SelectionBox<T>::SelectionBox(int w, int h, std::vector<T*>& c) : m_Scroll(0), m_Selected(0), m_Width(w), m_Height(h),
	m_Collection(c) {}

template<class T>
void SelectionBox<T>::draw(Screen& screen, int x, int y, bool highlight) const {
	
	int _color = color(highlight ? Color::Blue : Color::White, Color::Black, highlight);
	screen.drawRect(x, y, m_Width, m_Height, "#-|", _color);

	for(int i = 0; i < m_Height - 2; i++){
		if(i + m_Scroll >= m_Collection.size()) break;
		
		for(int q = 0; q < m_Width - 2; q++)
			screen.draw(x + q + 1, y + 1 + i, ' ', Color::White);
		
		bool selected = m_Selected == i + m_Scroll;
		const char* name = m_Collection[i + m_Scroll]->getName();
	        int center = (m_Width - 2 - strlen(name)) >> 1;
		_color = color(selected ? Color::Cyan : Color::White, Color::Black, selected & highlight);
		screen.draw(x + center + 1, y + i + 1, name, _color);
	}
}

template<class T>
void SelectionBox<T>::move(int x) {
	
	const int ROOM = m_Height - 2;
	m_Selected = clamp(m_Selected + x, 0, m_Collection.size() - 1);
	if(m_Selected >= m_Scroll + ROOM) m_Scroll = m_Selected - ROOM + 1;
	if(m_Selected < m_Scroll) m_Scroll = m_Selected;
	m_Scroll = clamp(m_Scroll, 0, std::max(0, (int)m_Collection.size() - ROOM));
}

Character* create_character(Screen& screen) {
	
	screen.fillRect(0, 0, screen.width(), screen.height(), ' ', color(Color::White, Color::Black));
	screen.draw(5, 3, "Create a Character", color(Color::Yellow, Color::Black, true));	
	

	char name[MAX_NAME_LENGTH + 1] = {0};
	int nameLength = 0;
	
	// Initialization	
	Menu_e option = MENU_NAME;
	int tabIndex = 0;
	bool picking = false;
	int raceIndex = 0, raceScroll = 0;
	int proffesionIndex = 0, proffesionScroll = 0;
	int BOX_H = (screen.height() >> 1) & ~0x1;
		
	SelectionBox<Race> raceBox(20, BOX_H, Race::all);
       	SelectionBox<Proffesion> proffesionBox(20, BOX_H, Proffesion::all);
       	SelectionBox<Element> elemBox(20, BOX_H, Element::all);

	int stats[STAT_MAX] = { 0 };
	recalculate_stats(stats, raceBox.getSelected(), proffesionBox.getSelected(), 0);

	while(1) {
		/********************** DRAW NAME TEXT BOX **********************/
		screen.draw(6, 7, "Name: ", color(option == MENU_NAME ? Color::Blue : Color::White, 
					Color::Black, option == MENU_NAME));
		screen.draw(12, 7, name, Color::White);
		for(int i = 0; i < MAX_NAME_LENGTH - nameLength; i++) 
			screen.draw(12 + nameLength + i, 7,  ' ', Color::White);
		if(option == 0){
			if(nameLength < MAX_NAME_LENGTH)
				screen.draw(12 + nameLength, 7, ' ', color(Color::White, Color::White));
		}

		// Draw race selection box
		
		bool selected = option == MENU_PROF_RACE && tabIndex == 0;
		int _color = color( selected ? Color::Blue : Color::White, Color::Black, selected);
		
		/******************** DRAW BOXES ****************************/
		screen.draw(7, 9, "Race", _color);
		
		// The box is only highlighted if the player is picking a race
		selected &= picking;
		raceBox.draw(screen, 7, 10, selected);

		selected = option == MENU_PROF_RACE && tabIndex == 1;
		_color = color(selected ? Color::Blue : Color::White, Color::Black, selected);

		screen.draw(30, 9, "Proffesion", _color);

		selected &= picking;
		proffesionBox.draw(screen, 30, 10, selected);
		
		selected = option == MENU_PROF_RACE && tabIndex == 2;
		_color = color(selected ? Color::Blue : Color::White, Color::Black, selected);

		screen.draw(54, 9, "Element", _color);

		selected &= picking;
		elemBox.draw(screen, 54, 10, selected);
		
		/************************* DRAW STATS ********************************/
		screen.draw(screen.width() - 22, 10, "Character Stats", color(Color::White, Color::Black, true));
		for(int i = 0; i < STAT_MAX; i++){
			_color = color(stats[i] < 20 ? Color::Red : stats[i] > 20 ? Color::Green : Color::White, 
					Color::Black, stats[i] != 20);
			
			std::string statName = getStatName((Stats)i);
			char stringBuffer[12];
			char spaceBuffer[10];
			
			for(int i = 0; i < 8 - statName.length(); i++) spaceBuffer[i] = ' ';
			spaceBuffer[8 - statName.length()] = 0;
			
			int successful = snprintf(stringBuffer, 11, "%s%s%d", statName.c_str(), spaceBuffer, stats[i]);
			if(successful < 0) {
				std::cout << "SNPrinf failed" << std::endl;
			}

			stringBuffer[11]= 0;
			screen.draw(screen.width() - 20, 11 + i, stringBuffer, _color);
		}
		
		/******************************** START BUTTON *********************************/
		_color = color(option == MENU_APPLY ? Color::Blue : Color::White, Color::Black, option == MENU_APPLY);
		screen.draw(screen.width() - 30, screen.height() - 4, "Start Adventure", _color); 
		
		screen.print();
		
		/********************************* HANDLE INPUT *********************************/
		KeyPress in = get_key();
		switch(in.key){
			case Key::Enter:
				switch(option) {
				case MENU_PROF_RACE:
					if(picking) {
						recalculate_stats(stats, raceBox.getSelected(), 
								proffesionBox.getSelected(), 0);
					}
					picking = !picking;
					break;
				case MENU_NAME:
					option = (Menu_e)((int)option + 1);
					break;
				case MENU_APPLY:
					{
					Character* character = new Character(name);
					character->setRace(raceBox.getSelected());
					character->setElement(elemBox.getSelected());
					character->setProffesion(proffesionBox.getSelected());
					return character;
					}
				default:
					return nullptr;
				}
				break;
			case Key::Right:
				if(option == MENU_PROF_RACE && !picking) tabIndex = std::min(2, tabIndex + 1);
				break;
			case Key::Left:
				if(option == MENU_PROF_RACE && !picking) tabIndex = std::max(0, tabIndex - 1);
				break;
			case Key::Down:
				if(option == MENU_PROF_RACE && picking){
					if(tabIndex == 0) raceBox.move(1);
					else if(tabIndex == 2) elemBox.move(1);
					else proffesionBox.move(1);
					recalculate_stats(stats, raceBox.getSelected(), proffesionBox.getSelected(), 0);
					
				}else {
					option = (Menu_e)((int)option + 1);
					if(option > MENU_APPLY) option = MENU_APPLY;
				}
				break;
			case Key::Up:
				if(option == MENU_PROF_RACE && picking) {
					if(tabIndex == 0) raceBox.move(-1);
					else if(tabIndex == 2) elemBox.move(-1);
					else proffesionBox.move(-1);	
					recalculate_stats(stats, raceBox.getSelected(), proffesionBox.getSelected(), 0);
				}else {
					option = (Menu_e)((int)option - 1);
					if(option < MENU_NAME) option = MENU_NAME;
				}
				break;
			case Key::Backspace:
				if(option == MENU_NAME && nameLength > 0){
					name[--nameLength] = 0;
				}
				break;
			default:
				if(option == MENU_NAME && (isalnum(in.glyph) || in.glyph == ' ') && nameLength < MAX_NAME_LENGTH){
					name[nameLength] = in.glyph;
					nameLength++;
				}
				break;
		}
	}

	return nullptr;
}


