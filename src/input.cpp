#include <input.h>


Input::Input(WindowController* win_control) {
  m_win_control = win_control;
}

Input::~Input(){
}

int Input::get_state() {
  return m_state;
}

void Input::update_panel() {
  WINDOW* temp = m_win_control->get_panel()->get_win();
  std::string tempstr = "";
  int row;
  int col;
  getyx(m_win_control->active_window()->get_win(), row, col);
  tempstr += std::to_string(row + 1) + ':' + std::to_string(col + 1);
  wclear(temp);
  mvwprintw(temp, 0, 0, "%s", tempstr.c_str());
  wrefresh(temp);
  return;
}

void Input::handle_input() {
  int ch = wgetch(m_win_control->active_window()->get_win());
  curs_set(0);
  switch(ch) {
  case KEY_F(1):
    m_state = QUIT;
    return;
  case KEY_F(2):
    m_win_control->set_active_window(0);
    break;
  case KEY_F(3):
    m_win_control->set_active_window(2);
    break;
  case KEY_F(5):
    m_win_control->add_window();
    m_win_control->set_active_window(2);
    break;
  default:
    wprintw(m_win_control->active_window()->get_win(), "%c", ch);
    break;
  };
  update_panel();
  wrefresh(m_win_control->active_window()->get_win());  
  curs_set(1);
}
