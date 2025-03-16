#include "win_ui_window.h"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  WUIWindow* window = (WUIWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  switch (msg) {
    case WM_CREATE: {
      LPCREATESTRUCT pCreate = (LPCREATESTRUCT)lParam;
      window = (WUIWindow*)pCreate->lpCreateParams;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      if (window) { // Ensure window pointer is valid
        HBRUSH color = CreateSolidBrush(RGB(window->styles.r, window->styles.g, window->styles.b));
        FillRect(hdc, &ps.rcPaint, color);
        DeleteObject(color);
      }
      EndPaint(hwnd, &ps);
      return 0;
    }
    case WM_CLOSE:
      if (window) window->should_close = true;
      PostQuitMessage(0);
      return 0;
    } 
  return DefWindowProcA(hwnd, msg, wParam, lParam);
}

WUIWindow* win_ui_create_window(const char* title, int width, int height) {
  WUIWindow* window = malloc(sizeof(WUIWindow));
  WNDCLASSA w32_wnd_class;
  ZeroMemory(&w32_wnd_class, sizeof(WNDCLASSA));

  w32_wnd_class.lpszClassName = title;
  w32_wnd_class.lpfnWndProc = WndProc;
  w32_wnd_class.hInstance = GetModuleHandleA(NULL);

  if (!RegisterClassA(&w32_wnd_class))
      MessageBoxA(NULL, "Cannot register the class", "win_ui", MB_OK);

  window->title = title;
  window->win32.handle = NULL;
  window->should_close = false;
  window->styles.r = 255; // Default white background
  window->styles.g = 255;
  window->styles.b = 255;

  window->win32.handle = CreateWindowExA(
      WS_EX_APPWINDOW, title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
      width, height, NULL, NULL, w32_wnd_class.hInstance, window);

  if (!window->win32.handle)
      MessageBoxA(NULL, "Cannot create window", "win_ui", MB_OK);

  ShowWindow(window->win32.handle, SW_SHOW);
  UpdateWindow(window->win32.handle);
  ZeroMemory(&window->win32.messages, sizeof(MSG));

  return window;
}

void win_ui_poll_events(WUIWindow* window) {
  while (PeekMessageA(&window->win32.messages, NULL, 0, 0, PM_REMOVE)) {
      if (window->win32.messages.message == WM_QUIT)
          window->should_close = true;

      TranslateMessage(&window->win32.messages);
      DispatchMessage(&window->win32.messages);
  }
}

void win_ui_destroy_window(WUIWindow* window) {
  DestroyWindow(window->win32.handle);
  free(window);
}

bool win_ui_should_close(WUIWindow* window) {
  return window->should_close;
}

void win_ui_set_bg_color(WUIWindow* window, int r, int g, int b) {
  window->styles.r = r;
  window->styles.g = g;
  window->styles.b = b;

  // Force the window to repaint with the new background color
  InvalidateRect(window->win32.handle, NULL, TRUE);
  UpdateWindow(window->win32.handle);
}
