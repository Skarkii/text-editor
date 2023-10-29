#include <color.h>
#include <iostream>
bool create_color(unsigned int id, unsigned int r, unsigned int b, unsigned int g) {
  init_color(id, (unsigned int)(r*1000/255), (unsigned int)(b*1000/255), (unsigned int)(g*1000/255));
  return true;
}

bool init_colors(){
  if(has_colors() == false) {
    return false;
  }
  start_color();

  //gruber_darker
  //create_color(COLOR_BACKGROUND, 28, 28, 28);
  //create_color(COLOR_PANEL_BACKGROUND, 38, 38, 38);
  //create_color(COLOR_DEFAULT_TEXT, 204, 204, 204);

  //one dark pro
  create_color(COLOR_BACKGROUND, 40, 44, 53);
  //#28 2b 35
  create_color(COLOR_PANEL_BACKGROUND, 35, 38, 48);
  create_color(COLOR_DEFAULT_TEXT, 204, 204, 204);

  
  init_pair(DEFAULT_PAIR, COLOR_DEFAULT_TEXT, COLOR_BACKGROUND);
  init_pair(PANEL_PAIR, COLOR_DEFAULT_TEXT, COLOR_PANEL_BACKGROUND);
  init_pair(PAIR_CURRENT_ROW_NUMBER_TEXT, COLOR_YELLOW, COLOR_BACKGROUND);
  return true;
}
