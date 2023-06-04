#pragma once

#include "conquest/graphics/screen.h"
#include "conquest/input/input.h"
#include "conquest/types.h"

namespace conquest::gui {

	/**
	 * A object that can be drawn to the GUI.
	 */
	class IWidget
	{
	public:
		/**
		 * Draws the widget
		 *
		 * @param isHighlighted - Indicates if the user is currently pointing to this object.
		 */
		virtual void draw(Screen& screen, bool isHighlighted) = 0;
	};

	/**
	 * An object that can be accept input.
	 */
	class IInputWidget
	{
	public:
		/**
		 * Sends input to the widget.
		 *
		 * @returns bool - True if the input was consumed.
		 */
		virtual bool sendInput(const InputResult& input) = 0;
	};
}