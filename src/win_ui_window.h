#include <windows.h>
#include <stdbool.h>

typedef struct WUIWindow {
  char* title;
  int width, height;
  bool should_close;

  struct {
    HWND handle;
    MSG messages;
    WNDCLASSA window_class;
  } win32;

  struct {
    int r,g,b;
  } styles;

} WUIWindow;

WUIWindow* win_ui_create_window(const char* title, int width, int height);
void win_ui_poll_events(WUIWindow* window);
void win_ui_destroy_window(WUIWindow* window);
bool win_ui_should_close(WUIWindow* window);
void win_ui_set_bg_color(WUIWindow* window, int r, int g, int b);
