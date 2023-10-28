#pragma once

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

class Buffer
{
 public:
  Buffer();
  ~Buffer();
  bool open_file(std::string file_name);
  bool save_file(std::string file_name);
  bool is_empty();
  unsigned int get_row_count();
  unsigned int get_row_length(unsigned int i);
  void print_contents();
  std::string get_line(int i);
  std::string get_file_name();
  bool insert(unsigned int row, unsigned col, std::string ch);
  bool remove(unsigned int row, unsigned int col);
  bool insert_new_line(unsigned int i, std::string str);
  bool remove_rest_of_line(unsigned int row, unsigned int col);
  unsigned int get_real_index(unsigned int row, unsigned int col);
  size_t get_char_size(unsigned int ch);
  bool remove_line(unsigned int row);
  size_t get_real_length(unsigned int row);
private:
  //           id                     text         characters(ignoring wide)
  std::map<unsigned int, std::pair<std::string, unsigned int>> m_rows; 
  std::string m_file_name = "";
  bool is_extra_bytes(char ch);
};
#include <ncurses.h>
