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
  void insert_character(int ch);
  void remove_character();
  void tab();
  void shift_tab();
  void new_line();
  void move_left();
  void move_right();
  void move_up();
  void move_down();
  void go_end_of_line();
  bool is_printable_char(unsigned int ch);
  void enter_command_prompt();
  bool execute_command(std::string str);
  void jump_to_row(int row);
private:
  int m_input = -1;
  int m_state = RUN;
  WindowController* m_win_control;
  std::vector<std::string> split_string(std::string str, char delimiter);
  bool is_string_digit(std::string str);
};
