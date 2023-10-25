#pragma once

#include <windowcontroller.h>
#include <string>

enum input_state { RUN, QUIT };

class Input
{
 public:
  Input(WindowController* win_controller);
  ~Input();
  void handle_input();
  int get_state();
  void update_panel();
 private:
  int m_inputs[5]; // Keep track of the last 5 key presses
  int m_state = RUN;
  WindowController* m_win_control;
};
