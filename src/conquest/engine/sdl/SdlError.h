#pragma once

#include <stdexcept>

#include "conquest/log/Log.h"

namespace conquest::engine::sdl {

class SdlError : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

/**
   Raises an sdl error and logs the error reason.

   @param message - The error message.
 */
[[noreturn]] void raiseSdlError(const std::string &message);

}
