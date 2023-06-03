#pragma once

#include <memory>

#include <ncurses.h>

#include "conquest/platform/unix.h"


namespace conquest {
	/**
	 * Singleton class that is resposible for creating and closing the ncurses context
	 */
	struct NCursesContext {
		static inline std::unique_ptr<NCursesContext> sInstance = nullptr;	
	
		explicit NCursesContext() {
			initscr();
			cbreak();
			noecho();
			keypad(stdscr, TRUE);
		}

		~NCursesContext() {
			endwin();
		}
	};

	inline void initializeNCurses() {
		if (nullptr != NCursesContext::sInstance) {
			return;
		}

		NCursesContext::sInstance = std::make_unique<NCursesContext>();
	}
}
