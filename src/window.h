#pragma once

#include <ncursesw/ncurses.h>
#include <color.h>
#include <buffer.h>
#include <iostream>

enum row_display_type { ROW_NUM, ROW_REL,ROW_LEN, ROW_DISABLED, ROW_LAST };

unsigned int count_digits(int n);

class Window
{
 public:
  Window(int lines, int cols, int beg_y, int beg_x) : Window(newwin(lines, cols, beg_y, beg_x)){}
  Window(WINDOW* win);
  ~Window();
  void set_bkgd(unsigned int pair);
  unsigned int get_margin();
  WINDOW* get_win();
  void set_buffer(Buffer* buf);
  Buffer* get_buffer();
  unsigned int get_row();
  unsigned int get_col();
  void set_row(unsigned int row);
  void set_col(unsigned int col);
  bool update_row_num();
  void set_row_type(unsigned int type);
  void cycle_row_type();
 private:
  Buffer* m_buf = nullptr;
  WINDOW* m_win = nullptr;
  unsigned int m_row = 0;
  unsigned int m_col = 0;
  unsigned int m_margin = 4;
  unsigned int m_row_type = ROW_NUM;
};
