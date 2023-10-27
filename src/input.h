#pragma once

#include <windowcontroller.h>
#include <string>
#include <color.h>
#include <iostream>

enum input_state { RUN, QUIT };

class Input
{
 public:
  Input(WindowController* win_controller);
  ~Input();
  void handle_input();
  int get_state();
  void update_panel();
  void insert_character(int ch);
  void remove_character();
  void tab();
  void shift_tab();
  void new_line();
  void move_left();
  void move_right();
  void move_up();
  void move_down();
private:
  int m_input = -1;
  int m_state = RUN;
  WindowController* m_win_control;
};
