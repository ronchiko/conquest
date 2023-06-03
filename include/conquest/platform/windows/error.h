#pragma once

#include <sstream>

#include "conquest/error.h"
#include "conquest/platform/windows.h"

namespace conquest::win32 {

	/**
	 * An error that was raised by windows.
	 */
	class WindowsError : public ConquestError
	{
	public:
		explicit WindowsError(const std::string& message)
			: WindowsError(message, GetLastError())
		{}

		explicit WindowsError(const std::string& message, const DWORD errorCode)
			: ConquestError(buildErrorMessage(message, errorCode))
		{}

	private:
		inline static std::string buildErrorMessage(const std::string& message, const DWORD errorCode)
		{
			char buffer[256];
			if(ERROR_SUCCESS !=
			   FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, LANG_USER_DEFAULT, buffer, 255, nullptr)) {
				strncpy_s(buffer, "Failed to get error message", 255);
			}

			std::stringstream stream;
			stream << message << "Windows Error: (" << errorCode << "): " << buffer;

			return stream.str();
		}
	};
}
