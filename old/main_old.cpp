#include <ncurses.h>
#include <string>
#include <map>
#include <fstream>
#include <assert.h>
#include <sstream>
#include <iostream>

std::map<unsigned int, std::pair<std::string, unsigned int>> eol;
unsigned int col_margin = 3;
unsigned int row, col;
unsigned int scrolled = 0;
std::string debug_string = "";

#define GRASS_PAIR     1
#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4


void print_row_len(){
  unsigned int i = 0;
  while(eol.count(i) != 0 && i < (unsigned int)getmaxy(stdscr)) {
    if(i == row+scrolled) { 
      mvprintw(i, 0, "   ");
      attron(COLOR_PAIR(PLAYER_PAIR));
      mvprintw(i, 0, "%d", eol[i + scrolled].second - col_margin);
      i++;
      attroff(COLOR_PAIR(PLAYER_PAIR));
      continue;
    }
    mvprintw(i, 0, "   ");
    mvprintw(i, 0, "%d", eol[i + scrolled].second - col_margin);
    i++;
  }
  move(row,col);
}

void print_row_num(){
  int i = 0;
  while(eol.count(i) != 0 && i - (int)scrolled < (int)getmaxy(stdscr)) {
    mvprintw(i, 0, "   ");
    mvprintw(i, 0, "%d", scrolled + i + 1);
    i++;
  }
  move(row,col);
}

void print_row_num_relative(){
  int i = 0;
  while(eol.count(i) != 0) {
    mvprintw(i, 0, "   ");
    mvprintw(i, 0, "%d", abs(row - i));
    i++;
  }
  move(row, col);
}


bool save_contents(std::string filename) {
  std::ofstream file;
  file.open(filename, std::ios::out | std::ios::binary);

  if(!file.is_open()) {
    assert(false && "could not open file");
  }
  
  std::stringstream buffer;
  int i = 0;
  while(eol.count(i) != 0) {
    if(eol[i].second == col_margin) {
      buffer << '\n';
      i++;
      continue;
    }
    for(unsigned int j = 0; j < eol[i].second - col_margin; j++) {
      buffer << eol[i].first.at(j);
    }
    buffer << '\n';
    i++;
  }

  file << buffer.str();
  return true;
}

bool load_contents(std::string filename) {
  std::ifstream file;
  clear();
  file.open(filename);
  eol.clear();
  if(!file.is_open()) {
    assert(false && "file does not exist");
  }
  int i = 0;
  std::stringstream str;
  std::string line = "";
  
  while(std::getline(file, line)) {
    int real_len = 0;
    for(size_t j = 0; j < line.size(); j++) {
      if(line[j] != (char)195) {
	real_len++;
      }
    }
    
    eol[i].first = line;
    eol[i].second = col_margin + real_len;

    if(i < getmaxy(stdscr)) {
      mvprintw(i, col_margin, "%s", line.c_str());
    }
    i++;
  }
  
  row = 0;
  scrolled = 0;
  col = col_margin;
  move(row, col);
  
  return true;
}

void scroll_wnd(int i) {
  if(i == 1 && (unsigned int)getmaxy(stdscr) + scrolled > eol.rbegin()->first) {
    return;
  }

  if(scrolled > 0 && i == -1) {
    curs_set(0);
    scrl(-1);
    scrolled -= 1;
    mvprintw(0, col_margin, "%s", eol[scrolled].first.c_str());
    curs_set(1);
  } else if(i == 1) {
      curs_set(0);
      scrl(1);
      scrolled += 1;
      if(eol.count(getmaxy(stdscr) + scrolled - 1) != 0) {
	//debug_string = std::to_string(getmaxy(stdscr) + scrolled);
	mvprintw(getmaxy(stdscr) -1, col_margin, "%s", eol[getmaxy(stdscr) + scrolled -1].first.c_str());
	}
  curs_set(1);
  }
}

int main(int argc, const char** argv) {
  int ch = -1;
  int last_ch = -1;
  setlocale(LC_ALL, "C.UTF-8");
  
  initscr(); // Creates stdscr
  raw(); // Disables line-buffering
  keypad(stdscr, TRUE); // Enables F1-F12 etc
  scrollok(stdscr, TRUE);
  cbreak();
  noecho();

  start_color();
  init_pair(GRASS_PAIR, COLOR_YELLOW, COLOR_GREEN);
  init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
  init_pair(MOUNTAIN_PAIR, COLOR_BLACK, COLOR_WHITE);
  init_pair(PLAYER_PAIR, COLOR_RED, COLOR_BLACK);
  
  
  row = 0;
  col = col_margin;
  eol[0].second = col;

  std::string file = "savefile";
  if(argc == 2) {
    file = argv[1];
    std::ifstream f;
    f.open(file);
    if(f.is_open()) {
      f.close();
      load_contents(file);
    }
  }
  unsigned int line_num_method = 0;
  move(row, col);
  refresh();
  while(ch != KEY_F(1)) {
    
    
    switch(ch) {
    case KEY_F(4):
      if(line_num_method++ > 1) { line_num_method = 0; }
      break;
    case KEY_F(9):
      save_contents(file);
      break;
    case KEY_F(10):
      load_contents(file);
      break;
    case KEY_F(5):
      scroll_wnd(-1);
      break;
    case KEY_F(6):
      scroll_wnd(1);
      break;
    case KEY_LEFT:
      if(col <= col_margin) {
	if(row > 0) {
	  row --;
	  col = eol[row + scrolled].second;
	}
	break;
      }
      col --;
      break;
    case KEY_RIGHT:
      if(col + 1 <= eol[row + scrolled].second) {
	col++;
      } else if(eol.count(row + scrolled + 1) != 0) {
	row++;
	col = col_margin;
      }
	
      break;
    case KEY_UP:
      if(row + scrolled < 1) { break; }
      row --;
      if(row <= 1) {
	scroll_wnd(-1);
      }
      if(col > eol[row + scrolled].second) {
	col = eol[row+scrolled].second;
      }
      break;
    case KEY_DOWN:
      if(eol.count(row + 1) > 0) {
	
	if(row + 1 >= (unsigned int)getmaxy(stdscr)) {
	  scroll_wnd(1);
	  row++;
	}
	else {
	  row++;
	}
      
      if(col > eol[row+scrolled].second) {
	col = eol[row+scrolled].second;
      }
      }
      break;
    case '\n':
      {
	if(row += 1 >= (unsigned int)getmaxx(stdscr) - 3) {
	  scroll_wnd(1);
	}
	for(unsigned int i = eol.rbegin()->first + 1; i > row + scrolled; i--) {
	  eol[i] = eol[i - 1];
	}
	int next_line_len = eol[row + scrolled].second - col + col_margin;
	char next_line[getmaxx(stdscr) - col];
	instr(next_line);
	
	eol[row + scrolled].second = col;
	printw("\n");
	insertln();
	row++;
	col = col_margin;
	mvprintw(row, col_margin, "%s", next_line);
	eol[row + scrolled].first = next_line;
	eol[row + scrolled].second = next_line_len;
	
      }
      move(row,col);
      
      break;
    case KEY_BACKSPACE:
      {
        if(col - 1 < col_margin) {
	  if(row == 0) { break; }
	  int len = eol[row+scrolled].second - col_margin;
	  char text_to_add[len];
	  instr(text_to_add);
	  row--;
	  col = eol[row + scrolled].second;
	  mvprintw(row, col, "%s", text_to_add);
	  deleteln();
	  eol[row + scrolled].second = eol[row + scrolled].second + len;
	  int i = row+scrolled+1;
	  while(eol.count(i) != 0) {
	    if(eol.count(i+1) != 0 ) {
	      eol[i] = eol[i+1];
	      i++;
	    }
	    else {
	      break;
	    }
	  }
	  debug_string = std::to_string(i);
	  eol.erase(i
		    + 1);
	  if(eol.count(scrolled + getmaxy(stdscr))!= 0) {
	  mvprintw(getmaxy(stdscr) -1, col_margin, "%s", eol[getmaxy(stdscr) + scrolled - 1].first.c_str());
	  }
	  
	} else {
	  char rest_of_line[getmaxx(stdscr) - col];
	  instr(rest_of_line);
	  col--;
	  mvprintw(row, col, "%s ", rest_of_line);
	  eol[row + scrolled].second = eol[row + scrolled].second - 1;
	  char full_line[eol[row+scrolled].second];
	  mvinstr(row, col_margin, full_line);
	  eol[row + scrolled].first = full_line;
	}
      }
      break;
    case 9: // TAB
      {
      
      }
      break;
    case 353: // Shift tab
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
	if(eol.count(row+scrolled) == 0) {
	  eol[row+ scrolled].second = col_margin;
	}
	
	eol[row + scrolled].second = eol[row + scrolled].second + 1;
	col++;
	char buf[getmaxx(stdscr) - col_margin];
	mvinstr(row, col_margin, buf);
	eol[row + scrolled].first = buf;
      }
      
      break;
    };
    
    move(row, col);
    
    last_ch = ch;

    if(line_num_method == 0) {
      print_row_num();
    } else if( line_num_method == 1) {
      print_row_len();
    } else if(line_num_method == 2) {
      print_row_num_relative();
    }
    mvprintw(0, 30, "%s            ", debug_string.c_str());
    move(row, col);
    refresh();
    ch = getch();
  };
  
  endwin();

  return 0;
}
