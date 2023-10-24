#pragma once

#include <ncurses.h>

class Window
{
 public:
  Window(int lines, int cols, int beg_y, int beg_x);
  Window(WINDOW* win);
  ~Window();
  WINDOW* get_win();
 private:
  WINDOW* m_win;
};
