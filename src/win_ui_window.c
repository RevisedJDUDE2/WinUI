#include "win_ui_window.h"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  WUIWindow* window = (WUIWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  switch (msg) {
    case WM_CLOSE:
      if(window) window->should_close = true;
      PostQuitMessage(0);
      return 0;
  }

  return DefWindowProcA(hwnd, msg, wParam, lParam);
}


WUIWindow* win_ui_create_window(const char* title, int width, int height) {
  WUIWindow* class = malloc(sizeof(WUIWindow));
  WNDCLASSA w32_wnd_class;
  ZeroMemory(&w32_wnd_class, sizeof(WNDCLASSA));
  w32_wnd_class.lpszClassName = title;
  w32_wnd_class.lpfnWndProc = WndProc;
  w32_wnd_class.hInstance = GetModuleHandleA(NULL);

  class->title = title;
  class->win32.handle = NULL;
  class->should_close = false;

  if(!RegisterClassA(&w32_wnd_class))
    MessageBoxA(NULL, "cannot register the class", "win_ui", MB_OK);

  class->win32.handle = CreateWindowExA(WS_EX_APPWINDOW, title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 
      NULL, NULL, w32_wnd_class.hInstance, NULL);

  if(!class->win32.handle)
      MessageBoxA(NULL, "cannot create window", "win_ui", MB_OK);

  class->width = width;
  class->height = height;

  ShowWindow(class->win32.handle, SW_SHOW);
  UpdateWindow(class->win32.handle);
  ZeroMemory(&class->win32.messages, sizeof(MSG));
  return class;
};
void win_ui_poll_events(WUIWindow* window) {
  while(PeekMessageA(&window->win32.messages, NULL, 0, 0, PM_REMOVE)) {
    if(window->win32.messages.message == WM_QUIT)
      window->should_close = true;

    TranslateMessage(&window->win32.messages);
    DispatchMessage(&window->win32.messages);
  }
};
void win_ui_destroy_window(WUIWindow* window) {
  DestroyWindow(window->win32.handle);
};
bool win_ui_should_close(WUIWindow* window) {
  return window->should_close;
};
