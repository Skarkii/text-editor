#include <ncurses.h>
#include <string>

int main(int argc, const char** argv) {

  int ch = -1;
  int last_ch = -1;
  setlocale(LC_ALL, "C.UTF-8");
  
  initscr(); // Creates stdscr
  raw(); // Disables line-buffering
  keypad(stdscr, TRUE); // Enables F1-F12 etc
  
  noecho();

  unsigned int row, col;
  row = 0;
  col = 0;
  
  move(row, col);
  refresh();
  int temp = -1;
  while(ch != KEY_F(1)) {

    switch(ch) {
    case KEY_LEFT:
      if(col < 1) { break; }
      col --;
      break;
    case KEY_RIGHT:
      col++;
      break;
    case KEY_UP:
      if(row < 1) { break; }
      row --;
      break;
    case KEY_DOWN:
      row++;
      break;
    case '\n':
      row++;
      col = 0;
      break;
    case KEY_BACKSPACE:
      if(col < 1) {
	if(row > 0) {
	  row --;
	}
	break;
      }
      col --;
      move(row, col);
      printw(" ");
      break;
    case -1:
      break;
    default:
      if(ch <= 189 && ch >= 32) { // Printable characters
	if(ch == 130) {
	  break;
	}
	switch(ch) {
	  break;
	case 163:
	  printw("£");
	  break;
	case 167:
	  printw("§");
	  break;
	case 168:
	  printw("¨");
	  break;
	case 172:
	  printw("€");
	  break;
	case 180:
	  printw("´");
	  break;
	case 189:
	  printw("½");
	case 190:
	  printw("½");
	  break;
	case 165:
	  printw("å");
	  break;
	case 133:
	  printw("Å");
	  break;
	case 164:
	  if(last_ch == 195) {printw("ä"); }
	  else if(last_ch == 194) { printw("¤"); }
	  break;
	case 132:
	  printw("Ä");
	  break;
	case 182:
	  printw("ö");
	  break;
	case 150:
	  printw("Ö");
	  break;
	default:
	  printw("%c", ch);
	  break;
	}
	col++;
      }
      
      break;
    };
    temp++;
    std::string current_row;
    current_row = "row: " + std::to_string(row) + "     ";
    std::string current_col;
    current_col = "col: " + std::to_string(col) + "     ";
    std::string key_pressed;
    key_pressed = "key pressed: " + std::to_string(ch) + "     ";
    std::string key_at_cursor;
    key_at_cursor = "key at cursor: " + std::to_string(mvinch(row, col));
    mvprintw(8, 5, "%s", key_at_cursor.c_str());
    mvprintw(7, 5, "%s", key_pressed.c_str());
    mvprintw(5, 5, "%s", current_row.c_str());
    mvprintw(6, 5, "%s", current_col.c_str());
    move(row, col);
    refresh();
    last_ch = ch;
    ch = getch();
  };
  
  endwin();

  return 0;
}
