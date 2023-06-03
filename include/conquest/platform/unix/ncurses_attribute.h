#pragma once

#include "conquest/platform/unix.h"

namespace conquest {
	/**
	 * Context for turning on and off ncurses.h attributes.
	 *
	 */
	struct NCursesAttriubte
	{
	public:
		explicit NCursesAttriubte(short attribute)
			: mAttribute(attribute)
		{
			attron(mAttribute);
		}

		NCursesAttriubte(const NCursesAttriubte&) = delete;
		NCursesAttriubte(NCursesAttriubte&&) = delete;

		NCursesAttriubte& operator=(const NCursesAttriubte&) = delete;
		NCursesAttriubte& operator=(NCursesAttriubte&&) = delete;

		~NCursesAttriubte() noexcept
		{
			attroff(mAttribute);
		}

	private:
		short mAttribute;
	};
}