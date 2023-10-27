#pragma once

#include <ncursesw/ncurses.h>

// Background starting at 16
#define COLOR_BACKGROUND 16
#define COLOR_PANEL_BACKGROUND 17

// Text colors starting at 64
#define COLOR_DEFAULT_TEXT 64
#define COLOR_ROW_NUMBER_TEXT 65
#define COLOR_CURRENT_ROW_NUMBER_TEXT 66

// Color pairs startin at 8
#define DEFAULT_PAIR 8
#define PANEL_PAIR 9
#define PAIR_CURRENT_ROW_NUMBER_TEXT 10

bool init_colors();
