#if defined(_WIN32) // Only compile this file if we are building to windows
#include "conquest/input/windows/input.h"

#include <unordered_map>

static const std::unordered_map<DWORD, conquest::InputType> DIRECTIONAL_KEYS{
	{ VK_LEFT, conquest::InputType::Left },		{ VK_RIGHT, conquest::InputType::Right },
	{ VK_UP, conquest::InputType::Up },			{ VK_DOWN, conquest::InputType::Down },
	{ VK_RETURN, conquest::InputType::Enter },	{ VK_BACK, conquest::InputType::Backspace },
	{ VK_ESCAPE, conquest::InputType::Escape },
};

namespace conquest {
	/**
	 * Get the console input handle
	 */
	static win32::Handle getConsoleInputHandle()
	{
		HANDLE inHandle = GetStdHandle(STD_INPUT_HANDLE);
		if(INVALID_HANDLE_VALUE == inHandle || nullptr == inHandle) {
			throw win32::WindowsError("Failed to get console input handle.");
		}

		return win32::Handle(inHandle);
	}

	InputManager::InputManager()
		: mHandle(getConsoleInputHandle())
	{
		get(); // The first key is enter.
	}

	InputResult InputManager::get()
	{
		INPUT_RECORD record;
		DWORD eventsRead;

		for(;;) { // Keep searching for events
			if(!ReadConsoleInputA(mHandle, &record, 1, &eventsRead)) {
				throw win32::WindowsError("Failed to read console input.");
			}

			if(eventsRead < 1) {
				continue; // We didn't read any events.
			}

			// Parse the events we read.
			if(const auto result = handleInputEvent(record); result) {
				return result;
			}
		}
	}

	InputResult InputManager::getAsync()
	{
		DWORD eventCount = 0;
		DWORD eventsRead = 0;

		if(!GetNumberOfConsoleInputEvents(mHandle, &eventCount)) {
			throw win32::WindowsError("Failed to query number of console events.");
		}

		if(eventCount <= 0) { // If no event are present, then no key was pressed.
			return InputResult{ InputType::Nothing, '\0' };
		}

		// Read the results from the console.
		std::vector<INPUT_RECORD> records;
		records.resize(eventCount);

		if(!ReadConsoleInputA(mHandle, records.data(), eventCount, &eventsRead)) {
			throw win32::WindowsError("Failed to read console input.");
		}

		for(DWORD i = 0; i < eventsRead; ++i) {
			if(const auto result = handleInputEvent(records[i]); result) {
				return result;
			}
		}

		return InputResult{ InputType::Nothing, '\0' };
	}

	InputResult InputManager::handleInputEvent(const INPUT_RECORD& record)
	{
		// Discard all non key events.
		if(record.EventType != KEY_EVENT) {
			return InputResult{ InputType::Nothing, '\0' };
		}

		if(!record.Event.KeyEvent.bKeyDown) {
			return InputResult{ InputType::Nothing, '\0' };
		}

		// Try to understand what key was pressed.
		const auto key = DIRECTIONAL_KEYS.find(record.Event.KeyEvent.wVirtualKeyCode);
		if(DIRECTIONAL_KEYS.end() != key) {
			return InputResult{ key->second, '\0' };
		}

		return InputResult{ InputType::Character, record.Event.KeyEvent.uChar.AsciiChar };
	}
}

#endif