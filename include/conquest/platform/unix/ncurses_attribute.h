#pragma once

#include "conquest/types.h"
#include "conquest/platform/unix.h"

namespace conquest {
	/**
	 * Context for turning on and off ncurses.h attributes.
	 *
	 */
	struct NCursesAttriubte
	{
	public:
		explicit NCursesAttriubte(uint32 attribute)
			: mAttribute(attribute)
		{
			if (0 != mAttribute) {
			attron(mAttribute);
			}
		}

		NCursesAttriubte(const NCursesAttriubte&) = delete;
		NCursesAttriubte(NCursesAttriubte&&) = delete;

		NCursesAttriubte& operator=(const NCursesAttriubte&) = delete;
		NCursesAttriubte& operator=(NCursesAttriubte&&) = delete;

		~NCursesAttriubte() noexcept
		{
			if (0 != mAttribute) {
				attroff(mAttribute);
			}
		}

	private:
		uint32 mAttribute;
	};
}
