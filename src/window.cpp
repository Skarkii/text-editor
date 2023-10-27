#include <window.h>

//Window::Window(int lines, int cols, int beg_y, int beg_x){
  //Window(newwin(lines, cols, beg_y, beg_x));
	 //keypad(m_win, TRUE);
	 //scrollok(m_win, TRUE);
//};

void Window::set_buffer(Buffer* buf){
  free(m_buf); //Free the old buffer before assigning new (COULD CAUSE ISSUES IF MULTIPLE WINDOWS LOAD SAME BUFFER LOOK INTO LATER)
  m_buf = buf;
}

Buffer* Window::get_buffer(){
  return m_buf;
}

Window::Window(WINDOW* win){
  m_win = win;
  keypad(m_win, TRUE);
  scrollok(m_win, TRUE);
  m_buf = new Buffer;
  wbkgd(m_win, COLOR_PAIR(DEFAULT_PAIR));
};

Window::~Window(){
  delwin(m_win);
};

WINDOW* Window::get_win(){
  return m_win;
}

void Window::set_bkgd(unsigned int pair){
  wbkgd(m_win, COLOR_PAIR(pair));
  return;
}

unsigned int Window::get_margin(){
  return m_margin;
}


unsigned int Window::get_row(){
  return m_row;
}
unsigned int Window::get_col(){
  return m_col;
}
void Window::set_row(unsigned int row){
  m_row = row;
}
void Window::set_col(unsigned int col){
  m_col = col;
}

unsigned int count_digits(int n){
  if(n == 0) return 1;
  if(n < 0) n = -n;
  unsigned int count = 0;
  while(n > 0) {
    n = n / 10;
    count++;
  }
  return count;
}

bool Window::update_row_num(){
  if(m_row_type == ROW_NUM) { // Default row numbers
    for(unsigned int i = 0; i < getmaxy(m_win); i++) {
      if(i < m_buf->get_row_count()) {
      std::string spaces(m_margin - count_digits(i+1) - 1, ' ');
      if(i == m_row) {
	wattron(m_win, COLOR_PAIR(PAIR_CURRENT_ROW_NUMBER_TEXT));
	mvwprintw(m_win, i, 0, "%s%d", spaces.c_str(), i + 1);
	wattroff(m_win, COLOR_PAIR(PAIR_CURRENT_ROW_NUMBER_TEXT));
	continue;
      }
      mvwprintw(m_win, i, 0, "%s%d", spaces.c_str(), i + 1);
    } else {
      std::string temp(m_margin, ' ');
      mvwprintw(m_win, i, 0, "%s", temp.c_str());
    }
    }
    
    return true;
  }

  
  else if(m_row_type == ROW_LEN) { // Length of each row (mostly debugging)
    unsigned int rows = m_buf->get_row_count();
    
    for(unsigned int i = 0; i < getmaxy(m_win); i++) {
      if(i < rows){
	unsigned int t = m_buf->get_row_length(i);
	std::string spaces(m_margin - count_digits(t) - 1, ' ');
	if(i == m_row) {
	  wattron(m_win, COLOR_PAIR(PAIR_CURRENT_ROW_NUMBER_TEXT));
	  mvwprintw(m_win, i, 0, "%s%d", spaces.c_str(), t);
	  wattroff(m_win, COLOR_PAIR(PAIR_CURRENT_ROW_NUMBER_TEXT));
	}else {
	  mvwprintw(m_win, i, 0, "%s%d", spaces.c_str(), t);
	}
      } else {
	std::string spaces(m_margin, ' ');
	mvwprintw(m_win, i, 0, "%s", spaces.c_str());
      }
    }
    return true;
  }


  
  else if(m_row_type == ROW_REL) { // Relative to current row
    for(unsigned int i = 0; i < getmaxy(m_win); i++) {
      if(i < m_buf->get_row_count()) {
      std::string spaces(m_margin - count_digits(i - m_row) - 1, ' ');
      if(i == m_row) {
	std::string spaces(m_margin - count_digits(i+1) - 1, ' ');
	wattron(m_win, COLOR_PAIR(PAIR_CURRENT_ROW_NUMBER_TEXT));
        mvwprintw(m_win, i, 0, "%s%d", spaces.c_str(), i);
	wattroff(m_win, COLOR_PAIR(PAIR_CURRENT_ROW_NUMBER_TEXT));
	continue;
      }
      mvwprintw(m_win, i, 0, "%s%d", spaces.c_str(), abs(i - m_row));
      } else {
	std::string spaces(m_margin, ' ');
	mvwprintw(m_win, i, 0, "%s", spaces.c_str());
      }

    }
    return true;
  }

  
  else if(m_row_type == ROW_DISABLED){ // No line numbers
    for(unsigned int i = 0; i < getmaxy(m_win); i++) {
      std::string temp(m_margin, ' ');
      mvwprintw(m_win, i, 0, "%s", temp.c_str());
    }
  }
  return false;
}


void Window::set_row_type(unsigned int type) {
  m_row_type = type;
}

void Window::cycle_row_type(){
  m_row_type = (row_display_type)((m_row_type + 1) % ROW_LAST);
}
