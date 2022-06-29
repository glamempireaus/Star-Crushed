#include "includes.h"

void debugprint(char *msg)
{
  fprintf(stderr, msg);
  fflush(stderr);
}

// Window Create
Window *Window_create(char *title, int width, int height, int gcps, int fps)
{
  // Allocate mem to window
  Window *window = malloc(sizeof(Window));

  // Set metainfo to window struct
  window->isClosed = false;
  window->height = height;
  window->width = width;
  window->title = title;

  // Create SDL window from function parameters
  window->sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
  window->sdlRenderer = SDL_CreateRenderer(window->sdlWindow, -1, SDL_RENDERER_SOFTWARE || SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);

  window->widgetnum = 0;

  return window;
}

void Window_addWidget(Window *window, Widget *widget)
{
  if (window->widgetnum == 0)
  {
    window->widgetarray = malloc(sizeof(Widget *));
  }
  else
  {
    window->widgetarray = realloc(window->widgetarray, sizeof(Widget *) * (window->widgetnum + 1));
  }
  window->widgetarray[window->widgetnum] = widget;
  window->widgetnum++;
}

// Window Close
void Window_close(Window* window)
{
  window->isClosed = true;
  free(window);
}

// Window Set Background Color
void Window_setBackgroundColor(Window* window, int r, int g, int b)
{
  SDL_SetRenderDrawColor(window->sdlRenderer, r, g, b, 255);
  SDL_RenderClear(window->sdlRenderer);
}

// Render Widgets in Window

void Window_renderWidgets(Window *window)
{
  for (int i = 0; i < window->widgetnum; i++)
  {
    Widget_render(window->widgetarray[i], window->sdlRenderer);
  }
}

void Window_disableWidgetRendering(Window *window)
{
  for (int i = 0; i < window->widgetnum; i++)
  {
    window->widgetarray[i]->common.isVisible = false;
  }
}

void Window_resizeWidgets(Window *window, int divSize, int windowWidth, int windowHeight)
{
  for (int i = 0; i < window->widgetnum; i++)
  {
    window->widgetarray[i]->common.sdlRect.x  = ceil(windowWidth/divSize*window->widgetarray[i]->common.x_div+1);
    window->widgetarray[i]->common.sdlRect.y  = ceil(windowHeight/divSize*window->widgetarray[i]->common.y_div+1);
    window->widgetarray[i]->common.sdlRect.w = ceil(windowWidth/divSize*window->widgetarray[i]->common.w_div+1);
    window->widgetarray[i]->common.sdlRect.h = ceil(windowHeight/divSize*window->widgetarray[i]->common.h_div+1);
  }
}
