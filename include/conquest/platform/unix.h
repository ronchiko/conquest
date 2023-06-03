#pragma once

#if !defined(__unix__) && !defined(unix)
#error This file can only be compiled on unix machines
#endif

#include <ncurses.h>

#include "conquest/platform/unix/ncurses_init.h"
#include "conquest/platform/unix/ncurses_attribute.h"
