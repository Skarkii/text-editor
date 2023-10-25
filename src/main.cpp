#include <string>
#include <ncurses.h>
#include <window.h>
#include <windowcontroller.h>
#include <iostream>
#include <input.h>

bool init_ncurses() {
  if(initscr() == nullptr) {
    return false;
  }
  setlocale(LC_ALL, "C.UTF-8"); 
  raw();
  cbreak();
  noecho();
  return true;
}

int main(int argc, char** argv) {
  if(init_ncurses() == false) {
    std::cout << "Could not initalise ncurses!" << std::endl;
    exit(1);
  }

  WindowController* win_control = new WindowController;
  
  win_control->add_window(stdscr);

  win_control->create_panel_window();

  Input* input = new Input(win_control);
  
  while(input->get_state() != QUIT){
    input->handle_input();
  }
  endwin();
  return 0;
};
