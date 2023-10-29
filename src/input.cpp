#include <input.h>


Input::Input(WindowController* win_control) {
  m_win_control = win_control;
}

Input::~Input(){
}

int Input::get_state() {
  return m_state;
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
    wdeleteln(cur->get_win());
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
  winnstr(cur->get_win(), str, char_size); 
  
  waddch(cur->get_win(), '\n');
  winsertln(cur->get_win());
  cur->set_row(cur->get_row() + 1);

  cur->get_buffer()->remove_rest_of_line(cur->get_row() - 1, cur->get_col());
  cur->get_buffer()->insert_new_line(cur->get_row(), str);
   
  cur->set_col(0);
  mvwaddstr(cur->get_win(), cur->get_row(), cur->get_col() + cur->get_margin(), str);
}

void Input::go_end_of_line(){
  Window* cur = m_win_control->active_window();
  cur->set_col(cur->get_buffer()->get_row_length(cur->get_row()));
}

void Input::tab(){
}

void Input::shift_tab(){
}

std::vector<std::string> Input::split_string(std::string str, char delimiter) {
  std::vector<std::string> strings;
  std::string s;
  std::stringstream full(str);
  while(std::getline(full, s, delimiter)){
    strings.push_back(s);
  }
  return strings;
}

bool Input::is_string_digit(std::string str){
  for(int i = 0; i < str.size(); i++){
    if(!isdigit(str[i])){
	return false;
      }
  }
  return true;
}

void Input::jump_to_row(int row){
  unsigned int new_row = 0;
  unsigned int new_col = 0;
  if(row < 0) {
    m_win_control->active_window()->set_row(0);
    m_win_control->active_window()->set_col(0);
  }
  else if(row > m_win_control->active_window()->get_buffer()->get_row_count() - 1) {
    m_win_control->active_window()->set_row(m_win_control->active_window()->get_buffer()->get_row_count() - 1);
    m_win_control->active_window()->set_col(0);
  }
  else {
    m_win_control->active_window()->set_row(row);
    m_win_control->active_window()->set_col(0);
  }
}

bool Input::execute_command(std::string str){
  std::vector<std::string> words = split_string(str, ' ');
  if(str[0] == ':' || str[0] == '+' || str[0] == '-'){
    if(words.size() != 1) { return false; }
    std::string temp = "";
    for(int i = 1; i < words[0].size(); i++) {
      temp += words[0][i];
    }
    
    if(is_string_digit(temp)){
      int dig = std::stoi(temp);
      if(str[0] == ':') {
	jump_to_row(dig - 1);
      }
      else if(str[0] == '+') {
	jump_to_row(m_win_control->active_window()->get_row() + dig);
      }
      else if(str[0] == '-') {
	jump_to_row(m_win_control->active_window()->get_row() - dig);
      }
      return true;
    }
    return false;
  }
  if(words[0] == "write" || words[0] == "w"){
    std::string file_name = m_win_control->active_window()->get_buffer()->get_file_name();
    if(words.size() == 2) {
      file_name = words[1];
      m_win_control->active_window()->get_buffer()->save_file(file_name);
      std::string msg = "Wrote " + file_name;
      m_win_control->set_panel_message(msg);
      return true;
    }
    else if(file_name != "New Buffer") {
      m_win_control->save_current_buffer();
      return true;
    }
    return false;
  }
  if(words[0] == "open" || words[0] == "o"){
    if(words.size() != 2) return false;

    std::string file_name = words[1];
    
    return true;
  }

  return false;
}

void Input::enter_command_prompt(){
  
  Window* cur = m_win_control->get_panel();
  m_win_control->update_panel();
  int ch = -1;
  std::string buf = "";
  int pos = 0;
  wmove(cur->get_win(), 1, pos);
  curs_set(1);
  while(true){
    ch = wgetch(cur->get_win());
    if(ch == KEY_F(1)){
      m_state = QUIT;
      return;
    }
    if(ch == KEY_F(4)){
      m_win_control->set_panel_message("aborted");
      return;
     }
    if(ch == '\n') {
      curs_set(0);
      break;
    }
    if(!is_printable_char(ch)){
      continue;
    }

    winsch(cur->get_win(), ch);
    buf += ch;
    pos ++;
    
    if(ch == 0XC3 || ch == 0XC2) { // Special case, 4 bytes per char
      ch = getch();
      winsch(cur->get_win(), ch);
      buf += ch;
    }
    if(ch == 0xE2) { // Special case, 6 bytes per char
      ch = getch();
      winsch(cur->get_win(), ch);
      buf += ch;
      
      ch = getch();
      winsch(cur->get_win(), ch);
      buf += m_input;
    }
    
    wmove(cur->get_win(), 1, pos);
  }
  if(!execute_command(buf)){
    std::string msg = "Failed to execute : " + buf;
    m_win_control->set_panel_message(msg);
  }
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
    enter_command_prompt();
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
  case KEY_F(9):
    m_win_control->save_current_buffer();
    break;
  case KEY_F(10): // TEMP LOAD
    m_win_control->set_panel_message("load");
    m_win_control->active_window()->get_buffer()->open_file(m_win_control->active_window()->get_buffer()->get_file_name());
    break;
  case KEY_F(11):
    break;
  case KEY_F(12):
    {
      unsigned int type = m_win_control->active_window()->cycle_row_type() + 1;
      std::string str = "Set row type : " + std::to_string(type);
      m_win_control->set_panel_message(str);
    }
    break;
  case KEY_LEFT:
    move_left();
    break;
  case 546: //ctrl left
    break;
  case KEY_RIGHT:
    move_right();
    break;
  case 561: //ctrl right
    break;
  case KEY_DOWN:
    move_down();
    break;
  case 526: //ctrl down
    break;
  case KEY_UP:
    move_up();
    break;
  case 567: //ctrl up
    break;
  case '\n':
    new_line();
    break;
  case KEY_BACKSPACE:
    remove_character();
    break;
  case 5:
    go_end_of_line();
    break;
  case 9: // tab
    tab();
    break;
  case 330: //DELETE button
    break;
  case 353: // shift tab
    shift_tab();
    break;
  default:
    if(is_printable_char(m_input)){
      insert_character(m_input);
    }
    break;
  };
  Window* wnd = m_win_control->active_window();

  wrefresh(m_win_control->active_window()->get_win());

  wnd->update_row_num();
  
  wmove(wnd->get_win(), wnd->get_row(), wnd->get_col() + wnd->get_margin());
  
  m_win_control->update_panel();
  curs_set(1);
  if(m_state != QUIT) {
    m_input = wgetch(m_win_control->active_window()->get_win());
  }
}

bool Input::is_printable_char(unsigned int ch){
  if(ch >= 0X20 && ch <= 0X7E) {
    return true;
  } else if(ch == 0XC2 || ch == 0XC3 || ch == 0XE2) {
    return true;
  }
  return false;
}
