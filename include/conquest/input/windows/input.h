#pragma once

#include "conquest/platform/windows.h"

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

	private:
		/**
		 * Creates an InputResult from an INPUT_RECORD
		 *
		 * @param record - The record to handle
		 */
		InputResult handleInputEvent(const INPUT_RECORD& record);

		win32::Handle mHandle;
	};
}