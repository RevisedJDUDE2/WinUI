#include "win_ui_window.h"

int main() {
  WUIWindow* window = win_ui_create_window("hello", 800, 600);

  while(!win_ui_should_close(window)) {
    win_ui_poll_events(window);
  }

  win_ui_destroy_window(window);
  free(window);
}
