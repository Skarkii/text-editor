#include <string>
#include <ncursesw/ncurses.h>
#include <windowcontroller.h>
#include <iostream>
#include <input.h>
#include <color.h>
#include <locale.h>

bool init_ncurses() {
  setlocale(LC_ALL, ""); 
  if(initscr() == nullptr) {
    return false;
  }
  if(init_colors() == false) {
    return false;
  }
  noecho();
  raw();
  cbreak();
  return true;
}

#include <buffer.h>
int main(int argc, char** argv) {
  Buffer* buf = new Buffer;
  for(size_t i = 1; i < argc; i++) {
    if(argv[i] == "-h") { // Needs to be implemented correctly later on
      exit(0);
    } else {   
      buf->open_file(argv[i]);
    }
  }

  if(init_ncurses() == false) {
    std::cout << "Could not initalise ncurses!" << std::endl;
    exit(1);
  }

  WindowController* win_control = new WindowController;
  
  win_control->add_window(stdscr);

  win_control->create_panel_window();
  win_control->set_active_window(0);
  win_control->get_panel()->set_bkgd(PANEL_PAIR);
    
  Input* input = new Input(win_control);

  if(buf->is_empty()) {
    free(buf);
  }else {
    win_control->add_buffer(buf);
    win_control->load_buffer(0);
  }
  
  while(input->get_state() != QUIT){
    input->handle_input();
  }

  endwin();

  //win_control->active_window()->get_buffer()->print_contents();
  
  return 0;
};
