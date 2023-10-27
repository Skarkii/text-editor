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
  tempstr += std::to_string(row + 1) + ':' + std::to_string(col + 1 - m_win_control->active_window()->get_margin());
  wclear(temp);
  mvwprintw(temp, 0, 0, "%s", m_win_control->active_window()->get_buffer()->get_file_name().c_str());
  mvwprintw(temp, 0, getmaxx(temp) - std::to_string(row+1).size() - std::to_string(col+1-m_win_control->active_window()->get_margin()).size() - 1, "%s", tempstr.c_str());
  mvwprintw(temp, 1, 0, "%s", "");
  wrefresh(temp);
  return;
}

void Input::insert_character(int ch){
    Window* cur = m_win_control->active_window();
    winsch(cur->get_win(), m_input);
    std::string a = "";
    a += m_input;
    
    if(m_input == 0XC3 || m_input == 0XC2) { // Special case, 4 bytes per char
      m_input = getch();
      winsch(cur->get_win(), m_input);
      a+= m_input;
    }
    if(m_input == 0xE2) { // Special case, 6 bytes per char
      m_input = getch();
      winsch(cur->get_win(), m_input);
      a += m_input;
      m_input = getch();
      winsch(cur->get_win(), m_input);
      a += m_input;
    }

    cur->get_buffer()->insert(cur->get_row(), cur->get_col(), a);
    
    if(cur->get_col() + cur->get_margin() + 1 >= getmaxx(cur->get_win())){
      cur->set_col(0);
      cur->set_row(cur->get_row() + 1);
    }
    else {
      cur->set_col(cur->get_col() + 1);
    }
}

void Input::remove_character(){
  Window* cur = m_win_control->active_window();
  if(cur->get_col() != 0) {
    cur->get_buffer()->remove(cur->get_row(), cur->get_col() - 1);
    mvwdelch(cur->get_win(), cur->get_row(), cur->get_col() + cur->get_margin() - 1);
    cur->set_col(cur->get_col() - 1);
  }
  else if(cur->get_row() > 0) {
    std::string str = cur->get_buffer()->get_line(cur->get_row());
    unsigned int size = cur->get_buffer()->get_row_length(cur->get_row());

    cur->set_row(cur->get_row() - 1);
    cur->set_col(cur->get_buffer()->get_row_length(cur->get_row()));
    cur->get_buffer()->remove_line(cur->get_row());
    deleteln();
    
    wmove(cur->get_win(), cur->get_row(), cur->get_col());
    mvwprintw(cur->get_win(), cur->get_row(), cur->get_col() + cur->get_margin(),"%s", str.c_str());
  }
}

void Input::move_left(){
  Window* cur = m_win_control->active_window();
  if(cur->get_col() > 0){
    cur->set_col(cur->get_col() - 1);
  } else if(cur->get_row() > 0) {
    cur->set_row(cur->get_row() - 1);
    cur->set_col(cur->get_buffer()->get_row_length(cur->get_row()));
  }
}

void Input::move_right(){
  Window* cur = m_win_control->active_window();
  if(cur->get_col() + 1 <= cur->get_buffer()->get_row_length(cur->get_row())) {
    cur->set_col(cur->get_col() + 1);
  }
  else if(cur->get_row() + 1 < cur->get_buffer()->get_row_count()){
    cur->set_row(cur->get_row() + 1);
    cur->set_col(0);
  }
}

void Input::move_up(){
  Window* cur = m_win_control->active_window();
  if(cur->get_row() > 0) {
    cur->set_row(cur->get_row() - 1);
    if(cur->get_col() > cur->get_buffer()->get_row_length(cur->get_row())) {
	cur->set_col(cur->get_buffer()->get_row_length(cur->get_row()));
      }
  }
}

void Input::move_down(){
  Window* cur = m_win_control->active_window();
  if(cur->get_row() + 1 < cur->get_buffer()->get_row_count()) {
      cur->set_row(cur->get_row() + 1);
      if(cur->get_col() > cur->get_buffer()->get_row_length(cur->get_row())) {
	cur->set_col(cur->get_buffer()->get_row_length(cur->get_row()));
      }
  }
}
  
void Input::new_line(){
  Window* cur = m_win_control->active_window();

  size_t char_size = cur->get_buffer()->get_real_index(cur->get_row(),cur->get_buffer()->get_row_length(cur->get_row())) - cur->get_buffer()->get_real_index(cur->get_row(), cur->get_col());
  char str[char_size];
  winnstr(cur->get_win(), str, char_size); // Copy everything to the end of line

  
  cur->get_buffer()->remove_rest_of_line(cur->get_row(), cur->get_col()); // Remove the rest of the line
  waddch(cur->get_win(), '\n');
  insertln();
  cur->set_row(cur->get_row() + 1);
  cur->get_buffer()->insert_new_line(cur->get_row(), str);
  cur->set_col(0);
  mvwaddstr(cur->get_win(), cur->get_row(), cur->get_col() + cur->get_margin(), str);
}

void Input::tab(){
}

void Input::shift_tab(){
}

void Input::handle_input() {
  curs_set(0);
  switch(m_input) {
  case -1:
    break;
  case KEY_RESIZE:
    break;
  case KEY_F(1):
    m_state = QUIT;
    return;
    break;
  case KEY_F(2):
    m_win_control->set_active_window(0);
    break;
  case KEY_F(3):
    m_win_control->set_active_window(2);
    break;
  case KEY_F(4):
    break;
  case KEY_F(5):
    m_win_control->add_window();
    m_win_control->set_active_window(2);
    break;
  case KEY_F(6):
    break;
  case KEY_F(7):
    break;
  case KEY_F(8):
    break;
  case KEY_F(9): // TEMP SAVE
    m_win_control->active_window()->get_buffer()->save_file(m_win_control->active_window()->get_buffer()->get_file_name());
    break;
  case KEY_F(10): // TEMP LOAD
    m_win_control->active_window()->get_buffer()->open_file(m_win_control->active_window()->get_buffer()->get_file_name());
    break;
  case KEY_F(11):
    break;
  case KEY_F(12):
    m_win_control->active_window()->cycle_row_type();
    break;
  case KEY_LEFT:
    move_left();
    break;
  case KEY_RIGHT:
    move_right();
    break;
  case KEY_DOWN:
    move_down();
    break;
  case KEY_UP:
    move_up();
    break;
  case '\n':
    new_line();
    break;
  case KEY_BACKSPACE:
    remove_character();
    break;
  case 9: // tab
    tab();
    break;
  case 353: // shift tab
    shift_tab();
    break;
  default:
    insert_character(m_input);
    break;
  };
  Window* wnd = m_win_control->active_window();

  wrefresh(m_win_control->active_window()->get_win());

  wnd->update_row_num();
  
  wmove(wnd->get_win(), wnd->get_row(), wnd->get_col() + wnd->get_margin());
  
  update_panel();
  curs_set(1);
  m_input = wgetch(m_win_control->active_window()->get_win());
}
