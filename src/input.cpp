#include <input.h>


Input::Input(WindowController* win_control) {
  m_win_control = win_control;
}

Input::~Input(){
}

int Input::get_state() {
  return m_state;
}

void Input::handle_input() {
  int ch = wgetch(m_win_control->active_window()->get_win());

  switch(ch) {
  case KEY_F(1):
    m_state = QUIT;
    return;
  case KEY_F(2):
    m_win_control->set_active_window(0);
    break;
  case KEY_F(3):
    m_win_control->set_active_window(1);
    break;
  case KEY_F(5):
    m_win_control->add_window();
    m_win_control->set_active_window(1);
    break;
  default:
    wprintw(m_win_control->active_window()->get_win(), "%c", ch);
    break;
  };
  wrefresh(m_win_control->active_window()->get_win());  
}
