#pragma once

#include <window.h>
#include <vector>

class WindowController
{
 public:
  WindowController();
  ~WindowController();
  bool add_window();
  bool set_active_window(int i);
  bool add_window(Window* window);
  bool add_window(WINDOW* window);
  bool create_panel_window();
  Window* get_panel();
  Window* active_window();
 private:
  Window* m_active_window = nullptr;
  Window* m_panel_window = nullptr;
  std::vector<Window*> m_windows;
};
