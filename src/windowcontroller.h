#pragma once

#include <window.h>
#include <vector>
#include <buffer.h>

class WindowController
{
 public:
  WindowController();
  ~WindowController();
  bool add_window();
  bool set_active_window(int i);
  bool add_window(Window* window);
  bool add_window(WINDOW* window);
  bool add_buffer(Buffer* buffer);
  bool create_panel_window();
  bool save_current_buffer();
  bool load_buffer(int i);
  Window* get_panel();
  Window* active_window();
  void update_panel();
  void display_panel_message();
  bool set_panel_message(std::string msg);
private:
  Window* m_active_window = nullptr;
  Window* m_panel_window = nullptr;
  std::vector<Window*> m_windows;
  std::vector<Buffer*> m_buffers;
  int m_panel_size = 2;
  std::string m_message = "";
};
