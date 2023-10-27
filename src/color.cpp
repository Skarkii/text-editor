#include <color.h>


bool init_colors(){
  if(has_colors() == false) {
    return false;
  }
  start_color();

  init_color(COLOR_BACKGROUND, 110, 110, 110);
  init_color(COLOR_PANEL_BACKGROUND, 150, 150, 150);
  init_color(COLOR_DEFAULT_TEXT, 800, 800, 800);
  
  init_pair(DEFAULT_PAIR, COLOR_DEFAULT_TEXT, COLOR_BACKGROUND);
  init_pair(PANEL_PAIR, COLOR_DEFAULT_TEXT, COLOR_PANEL_BACKGROUND);
  init_pair(PAIR_CURRENT_ROW_NUMBER_TEXT, COLOR_YELLOW, COLOR_BACKGROUND);
  return true;
}
