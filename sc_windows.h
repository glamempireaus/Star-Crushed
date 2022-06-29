#pragma once

void debugprint(char *msg);

// Outside Structs
typedef union Widget Widget;

// Listed Structs
typedef struct Window Window;

struct Window
{
  SDL_Window *sdlWindow;  // 64 bits - 8 bytes
  SDL_Renderer *sdlRenderer; // 64 bits - 8 bytes

  char *title;
  int type;
  int width;
  int height;

  Widget **widgetarray;
  int widgetnum;

  bool isClosed;
};

Window *Window_create(char *title, int width, int height, int gcps, int fps);
void Window_addWidget(Window *window, Widget *widget);
void Window_close(Window* window);
void Window_setBackgroundColor(Window* window, int r, int g, int b);
void Window_renderWidgets(Window *window);
void Window_disableWidgetRendering(Window *window);
void Window_resizeWidgets(Window *window, int divSize, int windowWidth, int windowHeight);
