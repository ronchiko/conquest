#pragma once

#include "conquest/platform/unix.h"

#include "conquest/input/types.h"

namespace conquest {

	/**
	 * A class that manager providing input to the user.
	 */
	class InputManager
	{
	public:
		explicit InputManager();

		/**
		 * Gets the input from the console in a blocking manner.
		 */
		InputResult get();

		/**
		 * Gets the input from the console without blocking
		 */
		InputResult getAsync();
	};
}
