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

  wmove(m_active_window->get_win(), 0, m_active_window->get_margin());
  
  return true;
}
