#include <windowcontroller.h>

WindowController::WindowController(){
};

WindowController::~WindowController(){
};

bool WindowController::add_window(Window* window){
  m_windows.push_back(window);
  if(m_active_window == nullptr) {
    m_active_window = window;
  }
  return true;
}

bool WindowController::add_buffer(Buffer* buffer){
  m_buffers.push_back(buffer);
  return true;
}

bool WindowController::load_buffer(int i){
  if(i > m_buffers.size()) {
    return false;
  }

  m_active_window->set_buffer(m_buffers[i]);

  Buffer* buf = m_active_window->get_buffer();
  
  for(size_t j = 0; j < buf->get_row_count(); j++){
    mvwprintw(m_active_window->get_win(), j, m_active_window->get_margin(), "%s", buf->get_line(j).c_str());
  }
  
  wrefresh(m_active_window->get_win());
  wmove(m_active_window->get_win(), 0, m_active_window->get_margin());
  return true;
}


bool WindowController::add_window(WINDOW* window){
  return WindowController::add_window(new Window(window));
}

bool WindowController::create_panel_window(){
  if(m_windows.size() < 1) {
    add_window(stdscr);
  }
  
  WINDOW* temp = m_windows.at(0)->get_win();
  int lines = m_panel_size;
  int cols = COLS;
  int beg_y = LINES - m_panel_size;
  int beg_x = 0;
  m_panel_window = new Window(lines, cols, beg_y, beg_x);
  m_windows.push_back(m_panel_window);
  
  wresize(temp, getmaxy(temp) - m_panel_size, cols);
  return true;
}

Window* WindowController::get_panel(){
  return m_panel_window;
}

bool WindowController::add_window() {
  if(m_windows.size() < 1) {
    add_window(stdscr);
  }
  WINDOW* temp = m_windows.at(0)->get_win();
  int lines = getmaxy(temp);
  int cols = getmaxx(temp) / 2;
  int beg_y = 0;
  int beg_x = getmaxx(temp) / 2;
  
  m_windows.push_back(new Window(lines, cols, beg_y, beg_x));

  wresize(temp, lines, cols);
  return true;
}

Window* WindowController::active_window(){
  return m_active_window;
}

bool WindowController::set_active_window(int i) {
  if(i+1 > m_windows.size()) {
    return false;
  }

  m_active_window = m_windows.at(i);  
  return true;
}

void WindowController::update_panel() {
  WINDOW* temp = get_panel()->get_win();
  std::string tempstr = "";
  
  int row = m_active_window->get_row() + 1; // Correct for index 0 in code, row 1 in visual
  int col = m_active_window->get_col() + 1;
  
  tempstr += std::to_string(row) + ':' + std::to_string(col);
  wclear(temp);
  mvwprintw(temp, 0, 0, "%s", active_window()->get_buffer()->get_file_name().c_str());
  mvwprintw(temp, 0, getmaxx(temp) - tempstr.size(), "%s", tempstr.c_str());

  display_panel_message();
  wrefresh(temp);
}

void WindowController::display_panel_message(){
  mvwprintw(get_panel()->get_win(), 1, 0, "%s", m_message.c_str());
  if(m_message != "") { m_message = ""; }
}

bool WindowController::set_panel_message(std::string msg){
  m_message = msg;
  return true;
}

bool WindowController::save_current_buffer(){
  if(active_window()->get_buffer()->save_file(active_window()->get_buffer()->get_file_name())) {
    std::string str = "Wrote " + active_window()->get_buffer()->get_file_name();
    set_panel_message(str);
    return true;
  }
  else {
    set_panel_message("Could not save file");
    return false;
  }
}
