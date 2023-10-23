#include <ncurses.h>
#include <string>
#include <map>
#include <fstream>
#include <assert.h>
#include <sstream>
std::map<unsigned int, unsigned int> eol;
unsigned int col_margin = 3;

bool save_contents(std::string filename) {
  std::ofstream file;
  file.open(filename);

  if(!file.is_open()) {
    assert(false && "could not open file");
  }
  char str[getmaxx(stdscr)];
  std::stringstream buffer;
  int i = 0;
  while(eol.count(i) != 0) {
    mvinnstr(i, col_margin, str, eol[i] - col_margin);
    buffer << str << '\n';
    i ++;
  }

  file << buffer.str();
  return true;
}

bool load_contents(std::string filename) {
  std::fstream file;

  file.open(filename);

  if(!file.is_open()) {
    assert(false && "could not open file");
  }

  std::stringstream buffer;
  char c;
  while(file >> c) {
    buffer << c;
  }

  mvprintw(0, col_margin, "%s", buffer.str().c_str());
  
  return true;
}

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
  col = col_margin;
  eol[0] = col;
  
  move(row, col);
  refresh();
  int temp = -1;
  while(ch != KEY_F(1)) {
  
    switch(ch) {
    case KEY_F(9):
      save_contents("savefile");
      break;
    case KEY_F(10):
      load_contents("savefile");
      break;
    case KEY_LEFT:
      if(col <= col_margin) { break; }
      col --;
      break;
    case KEY_RIGHT:
      if(col + 1 <= eol[row]) {
	col++;
      }
      break;
    case KEY_UP:
      if(row < 1) { break; }
      row --;
      if(col > eol[row]) { col = eol[row]; }
      break;
    case KEY_DOWN:
      if(eol.count(row + 1) > 0) {
          row++;	
      }
      if(col > eol[row]) { col = eol[row];}
      break;
    case '\n':
      {
	char str[getmaxx(stdscr) - col];
	int len = eol[row] - col + col_margin;
	eol[row] = col;
	instr(str);
	printw("\n");
	row = row + 1;
	col = col_margin;
	move(row, col);
        
	insertln();
	printw("%s", str);

	for(unsigned int i = eol.rbegin()->first + 1; i > row; i --) {
	  eol[i] = eol[i - 1];
	}
	
	eol[row] = len;
      }
      break;
    case KEY_BACKSPACE:
      {
	int len = eol[row] - col;
	char str[getmaxx(stdscr) - col];
	instr(str);
	if(col <= col_margin) {
	  if(row > 0) {
	    row = row - 1;
	    col = eol[row];
	    move(row, col);
	    printw("%s", str);
	    eol[row] = eol[row] + len;
	    deleteln();
	    unsigned int i = row + 1;
	    while(eol.count(i) > 0) {
	      if(eol.count(i + 1) <= 0) {
		eol.erase(i);
		break;
	      } else {
	      eol[i] = eol[i + 1];
	      i++;
	      }
	    }
	    break;
	  }
	  break;
	}
	col --;
	move(row, col);
	printw("%s ", str);
      }
      eol[row] = eol[row] - 1;
      break;
    case 9:
      break;
    case 353:
      break;
    case -1:
      break;
    default:
      if(ch <= 189 && ch >= 32) { // Printable characters
	if(ch == 130) {
	  break;
	}
	{
	  int char_count = getmaxx(stdscr) - col;
	  char str[char_count];
	  instr(str);
	  mvprintw(row, col+1,"%s", str);
	  move(row, col);
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
	eol[row] = eol[row] + 1;
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
    key_at_cursor = "key at cursor: " + std::to_string(mvinch(row, col)) + "         ";
    std::string row1len;
    row1len += "row 1 len: " + std::to_string(eol[0]) + "    ";
    //mvprintw(35, 0, "%s", row2len.c_str());
    //mvprintw(34, 0, "%s", row1len.c_str());
    //mvprintw(30, 0, "%s", key_at_cursor.c_str());
    //mvprintw(31, 0, "%s", key_pressed.c_str());
    //mvprintw(32, 0, "%s", current_row.c_str());
    //mvprintw(33, 0, "%s", current_col.c_str());
    move(row, col);
    refresh();
    last_ch = ch;
    ch = getch();
  };
  
  endwin();

  return 0;
}
