#include <window.h>

Window::Window(int lines, int cols, int beg_y, int beg_x){
  m_win = newwin(lines, cols, beg_y, beg_x);
  keypad(m_win, TRUE);
  scrollok(m_win, TRUE);
};

Window::Window(WINDOW* win){
  m_win = win;
  keypad(m_win, TRUE);
  scrollok(m_win, TRUE);
};

Window::~Window(){
  delwin(m_win);
};

WINDOW* Window::get_win(){
  return m_win;
}
