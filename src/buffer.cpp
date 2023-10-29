#include <buffer.h>

Buffer::Buffer(){
};

Buffer::~Buffer(){
};

std::string Buffer::get_line(int i){
  if(m_rows.size() < i) {
    assert(false && "invalid here");
    return "";
  }
  return m_rows[i].first;
}

unsigned int Buffer::get_row_length(unsigned int i){
  return m_rows[i].second;
}

bool Buffer::insert(unsigned int row, unsigned col, std::string ch){
  if(get_row_count() < row) {
    return false;
  }

  if(col == 0) {
    m_rows[row].first.insert(col, ch);
    m_rows[row].second ++;
    return true;
  }

  m_rows[row].first.insert(get_real_index(row, col), ch);
  m_rows[row].second ++;
  
  return true;
}

bool Buffer::is_extra_bytes(char ch){
  if(ch != (char)0XC3 && ch != (char)0XC2 && ch != (char)0XE2 && ch != (char)0X82){
    return false;
  }
  return true;
}

unsigned int Buffer::get_real_index(unsigned int row, unsigned int col){
  unsigned int index = 0;

  if(col > m_rows[row].first.size()) {
    endwin();
    std::cout << "ERROR: REQUESTED INVALID COLUMN FOR REAL INDEX" << std::endl;
    exit(1);
  }
  if(col == 0) {
    return 0;
  }
  
  for(size_t i = 0; i < m_rows[row].first.size(); i++) {
    if(!is_extra_bytes(m_rows[row].first[i])) {
      index++;
    }
    if(index == col) {
      i++;
      return i;
      break;
    }
  }
  endwin();
  std::cout << m_rows[row].first << std::endl << row << std::endl << col << std::endl;
  exit(1);
  return -1;
}

bool Buffer::insert_new_line(unsigned int i, std::string str){
  for(size_t j = m_rows.size(); j >= i; j--){
    m_rows[j] = m_rows[j-1];
  }
  
  size_t size = 0;
  for(size_t j = 0; j < str.size(); j++){
    if(!is_extra_bytes(str[j])){
	size++;
      }
  }

  m_rows[i].first = str;
  m_rows[i].second = size;
  return true;
}

bool Buffer::remove_rest_of_line(unsigned int row, unsigned int col) {
  m_rows[row].first.erase(get_real_index(row, col));
  m_rows[row].second = col;
  return true;
}

size_t Buffer::get_char_size(unsigned int ch){
  size_t count = 1;
  if(ch == (char)0XC3 || ch == (char)0XC2){
    count = 2;
  }
  else if(ch == (char)0XE2){
    count = 3;
  }
  return count;
}

bool Buffer::remove_line(unsigned int row){
  std::string str = get_line(row);
  unsigned int size = get_row_length(row);

  for(size_t i = row; i < m_rows.size() - 1; i++) {
    m_rows[i] = m_rows[i+1];
  }
  m_rows.erase(m_rows.size() - 1);

  m_rows[row].first = str + m_rows[row].first;
  m_rows[row].second += size;

  return true;
}

bool Buffer::remove(unsigned int row, unsigned int col){
  if(get_row_count() < row) {
    return false;
  }

  if(col == 0) {
    char ch = m_rows[row].first[col];
    size_t count = get_char_size(ch);
    m_rows[row].first.erase(col, count);
    m_rows[row].second --;
    return true;
  }

  size_t index = get_real_index(row, col);
  
  m_rows[row].first.erase(index, get_char_size(m_rows[row].first[index]));
  m_rows[row].second --;
  return true;
}

unsigned int Buffer::get_row_count(){
  return m_rows.size();
}

size_t Buffer::get_real_length(unsigned int row){
  return m_rows[row].first.size();
}

bool Buffer::is_empty(){
  if(m_rows.size() < 1) {
    return true;
  }
  return false;
}

bool Buffer::open_file(std::string file_name){
  std::ifstream file;
  file.open(file_name);
  if(!file.is_open()) {
    std::cout << "File does not exist" << std::endl;
    exit(1); // Create empty buffer but remember file name later on
  }

  std::string line;
  size_t i = 0;
  while(std::getline(file, line)) {
    int len = 0;
    for(size_t j = 0; j < line.length(); j++){
      if(line[j] != (char)0XC3 && line[j] != (char)0XC2 && line[j] != (char)0XE2 && line[j] != (char)0X82) { 
	len++;
      }
    }
    m_rows[i].first = line;
    m_rows[i].second = len;
    i++;
  }
  file.close();
  m_file_name = file_name;
  return true;
}


std::string Buffer::get_file_name(){
  if(m_file_name == ""){
    return "New Buffer";
  }
  return m_file_name;
}

bool Buffer::save_file(std::string file_name){
  std::ofstream file;
  file.open(file_name);
  if(!file.is_open()){
    return false;
  }
  for(int i = 0; i < get_row_count(); i++){
    file << get_line(i) << '\n';
  }
  file.close();
  std::string str = "Wrote " + file_name;
  return true;
}

void Buffer::print_contents(){
  for(size_t i = 0; i < m_rows.size(); i++) {
    std::cout << m_rows[i].second << "\t|";
    for(size_t j = 0; j < m_rows[i].first.length(); j++){
      std::cout << m_rows[i].first.at(j);
    }
    std::cout << std::endl;
  }
};
