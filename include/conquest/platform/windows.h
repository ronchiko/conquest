#pragma once

// Header file to help include windows stuff.

#ifndef _WIN32
#error This file can only be compiled on windows machines
#endif

#include <Windows.h>

// Remove the min and max macros that come with windows
#undef max
#undef min

#include "conquest/platform/windows/error.h"
#include "conquest/platform/windows/handle.h"
