#pragma once

#include <memory>

#include "conquest/input/types.h"

#if defined(_WIN32)
#include "conquest/input/windows/input.h"
#elif defined(__unix__) || defined(unix)
#include "conquest/input/unix/input.h"
#else
#error Rendering is not supported on this platform.
#endif

namespace conquest {

	/**
	 * Class that returns input from the user.
	 */
	class Input
	{
	public:
		/**
		 * Initializes the input class.
		 */
		static void initialize()
		{
			sInstance = std::make_unique<InputManager>();
		}

		/**
		 * Gets a key (blocking)
		 */
		static InputResult get()
		{
			return sInstance->get();
		}

		/**
		 * Gets a key press (non-blocking)
		 *
		 * @return InputResult - InputResult.type will be InputType::Nothing if no key was pressed.
		 */
		static InputResult getAsync()
		{
			return sInstance->getAsync();
		}

	private:
		inline static std::unique_ptr<InputManager> sInstance = nullptr;
	};
}