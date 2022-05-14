#pragma once

#include "screen.h"

typedef enum {
	TITLE_NEW_GAME,
	TITLE_LOAD,
	TITLE_QUIT
} TitleResult_e;	

// Begins the title screen
TitleResult_e start_title(Screen& s);
