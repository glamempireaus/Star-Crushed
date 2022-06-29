#include "includes.h"

// WIDGETS

// Create Widget
void Widget_constructWidgetCommon(WidgetCommon *widgetCommon, Window *window, int div_x, int div_y, int div_w, int div_h, int windowWidth, int windowHeight, int divSize)
{
  float tmp_x = windowWidth/divSize*div_x+1;
  float tmp_y = windowHeight/divSize*div_y+1;
  float tmp_w = windowWidth/divSize*div_w+1;
  float tmp_h = windowHeight/divSize*div_h+1;

  if (tmp_w * window->widgetnum + 1 == windowWidth)
  {
    tmp_w += 1;
  }

  widgetCommon->sdlRect.x = ceil(tmp_x);
  widgetCommon->sdlRect.y = ceil(tmp_y);
  widgetCommon->sdlRect.w = ceil(tmp_w);
  widgetCommon->sdlRect.h = ceil(tmp_h);

  widgetCommon->x_div = div_x;
  widgetCommon->y_div = div_y;
  widgetCommon->w_div = div_w;
  widgetCommon->h_div = div_h;

  widgetCommon->state = WIDGET_STATE_DEFAULT;
  widgetCommon->window = window;

  widgetCommon->isVisible = false;

  Window_addWidget(window, (Widget *)widgetCommon);
}

void Widget_constructWidgetButton(WidgetButton *widgetButton, Window *window, int div_x, int div_y, int div_w, int div_h, int windowWidth, int windowHeight, int divSize)
{
  widgetButton->common.type = WIDGET_TYPE_BUTTON;
  Widget_constructWidgetCommon((WidgetCommon *)widgetButton, window, div_x, div_y, div_w, div_h, windowWidth, windowHeight, divSize);

  widgetButton->font = NULL;
  widgetButton->text = NULL;
}

void Widget_constructWidgetGame(WidgetGame *widgetGame, Window *window, int div_x, int div_y, int div_w, int div_h, int windowWidth, int windowHeight, int divSize)
{
  widgetGame->common.type = WIDGET_TYPE_GAME;
  Widget_constructWidgetCommon((WidgetCommon *)widgetGame, window, div_x, div_y, div_w, div_h, windowWidth, windowHeight, divSize);

  widgetGame->surface = SDL_GetWindowSurface(window->sdlWindow);
  widgetGame->objectTemplateArrayNum = 0;

  widgetGame->enemy1_spawnerTicks = 0;
  widgetGame->enemy1_spawnerDelay = 4200;

  widgetGame->enemy2_spawnerTicks = 0;
  widgetGame->enemy2_spawnerDelay = 5000;

  widgetGame->enemy3_spawnerTicks = 0;
  widgetGame->enemy3_spawnerDelay = 5000;

  widgetGame->starfield.isStarfieldActive = false;
  widgetGame->ticks_mainLoop = 0;

  widgetGame->pressedkey_w = false;
  widgetGame->pressedkey_a = false;
  widgetGame->pressedkey_s = false;
  widgetGame->pressedkey_d = false;
  widgetGame->pressedkey_space = false;

  for (int i = 0; i < MAX_ENEMY_OBJECTS; i++)
  {
    widgetGame->enemyArray[i].health = 0;
    widgetGame->enemyArray[i].common.isVisible = false;
  }

  for (int i = 0; i < MAX_PLAYERPROJECTILE_OBJECTS; i++)
  {
    widgetGame->playerProjectileArray[i].common.isVisible = false;
  }

  for (int i = 0; i < MAX_ENEMYPROJECTILE_OBJECTS; i++)
  {
    widgetGame->enemyProjectileArray[i].common.isVisible = false;
  }

  for (int i = 0; i < MAX_EXPLOSIONS; i++)
  {
    widgetGame->explosionArray[i].isActive = false;
    widgetGame->explosionArray[i].object = NULL;
    widgetGame->explosionArray[i].imageSurface = NULL;
  }

}

void Widget_enableRendering(Widget *widget)
{
  widget->common.isVisible = true;
}

void Widget_disable(Widget *widget)
{
  widget->common.isVisible = false;
}

void WidgetButton_setMousePressDraggable(WidgetButton *widgetButton, bool isMousePressDraggable)
{
  widgetButton->isMousePressDraggable = isMousePressDraggable;
}

// Set Widget RGB
void Widget_setRGBDefault(Widget *widget, Uint8 r, Uint8 g, Uint8 b)
{
  widget->common.r_default = r;
  widget->common.g_default = g;
  widget->common.b_default = b;
}

// Set Widget RGB for hovering
void Widget_setRGBHover(Widget *widget, Uint8 r, Uint8 g, Uint8 b)
{
  widget->common.r_hover = r;
  widget->common.g_hover = g;
  widget->common.b_hover = b;
}

// Set Widget RGB for pressing
void Widget_setRGBPress(Widget *widget, Uint8 r, Uint8 g, Uint8 b)
{
  widget->common.r_press = r;
  widget->common.g_press = g;
  widget->common.b_press = b;
}

// Set Widget Text Field
void Widget_setText(Widget *widget, char *text, Font *font)
{
  switch (widget->type)
  {
    case (WIDGET_TYPE_BUTTON):
    {
      widget->button.font = font;
      widget->button.text = text;
      break;
    }
    case (WIDGET_TYPE_GAME):
    {

    }
  }
}

// Add object to Widget Template Array
ObjectTemplate *WidgetGame_addObjectTemplate(ObjectTemplate *objectTemplate, WidgetGame *widgetGame)
{
  if (widgetGame->objectTemplateArrayNum == 0)
  {
    widgetGame->objectTemplateArray = malloc(sizeof(ObjectTemplate *));
  }
  else
  {
    widgetGame->objectTemplateArray = realloc(widgetGame->objectTemplateArray, sizeof(ObjectTemplate *) * (widgetGame->objectTemplateArrayNum + 1));
  }
  widgetGame->objectTemplateArray[widgetGame->objectTemplateArrayNum] = objectTemplate; //
  objectTemplate->arrayElem = widgetGame->objectTemplateArrayNum;
  widgetGame->objectTemplateArrayNum++;
  return objectTemplate;
}

// Add object to Object Player Array
ObjectEnemy *WidgetGame_findUnusedEnemyInArray(WidgetGame *widgetGame)
{
  for (int i = 0; i < MAX_ENEMY_OBJECTS; i++)
  {
    // If it's NULL, replace with a new object
    if (widgetGame->enemyArray[i].health == 0)
    {
      return &(widgetGame->enemyArray[i]);
    }
  }
  return NULL;
}

// Add object to Object Player Projectile Array
ObjectProjectile *WidgetGame_findUnusedEnemyProjectileInArray(WidgetGame *widgetGame)
{
  for (int i = 0; i < MAX_ENEMYPROJECTILE_OBJECTS; i++)
  {
    // If it's NULL, replace with a new object
    if (!widgetGame->enemyProjectileArray[i].common.type < 0)
    {
      return &(widgetGame->enemyProjectileArray[i]);
    }
  }
  return NULL;
}

// Add object to Object Player Projectile Array
ObjectProjectile *WidgetGame_findUnusedPlayerProjectileInArray(WidgetGame *widgetGame)
{
  for (int i = 0; i < MAX_PLAYERPROJECTILE_OBJECTS; i++)
  {
    // If it's NULL, replace with a new object
    if (!widgetGame->playerProjectileArray[i].common.type < 0)
    {
      return &(widgetGame->playerProjectileArray[i]);
    }
  }
  return NULL;
}

ObjectHUD *WidgetGame_findUnusedHUDInArray(WidgetGame *widgetGame)
{
  for (int i = 0; i < MAX_HUD_OBJECTS; i++)
  {
    // If it's NULL, replace with a new object
    if (widgetGame->HUDArray[i].common.type < 0)
    {
      return &(widgetGame->HUDArray[i]);
    }
  }
  return NULL;
}

Explosion *WidgetGame_findUnusedExplosionInArray(WidgetGame *widgetGame)
{
  for (int i = 0; i < MAX_EXPLOSIONS; i++)
  {
    // If it's NULL, replace with a new object
    if (!widgetGame->explosionArray[i].isActive)
    {
      return &(widgetGame->explosionArray[i]);
    }
  }
  return NULL;
}


void WidgetGame_renderObjects(WidgetGame *widgetGame)
{
  Game_Object_render((Object *)&(widgetGame->currentPlayer));

  for (int i = 0; i < MAX_ENEMYPROJECTILE_OBJECTS; i++)
  {
    if (widgetGame->enemyProjectileArray[i].common.isVisible)
    {
      Game_Object_render((Object *)&(widgetGame->enemyProjectileArray[i]));
    }
  }

  for (int i = 0; i < MAX_ENEMY_OBJECTS; i++)
  {
    if (widgetGame->enemyArray[i].common.isVisible)
    {
      Game_Object_render((Object *)&(widgetGame->enemyArray[i]));
    }
  }

  for (int i = 0; i < MAX_PLAYERPROJECTILE_OBJECTS; i++)
  {
    if (widgetGame->playerProjectileArray[i].common.isVisible)
    {
      Game_Object_render((Object *)&(widgetGame->playerProjectileArray[i]));
    }
  }

  for (int i = 0; i < MAX_HUD_OBJECTS; i++)
  {
    if (widgetGame->HUDArray[i].common.isVisible)
    {
      Game_Object_render((Object *)&(widgetGame->HUDArray[i]));
    }
  }
}

void WidgetGame_renderExplosions(WidgetGame *widgetGame)
{
  for (int i = 0; i < MAX_EXPLOSIONS; i++)
  {
    if (widgetGame->explosionArray[i].isActive)
    {
      Game_Explosion_render(&widgetGame->explosionArray[i]);
    }
  }
}

void WidgetGame_setUnitData(WidgetGame *widget, int unitWidth, int unitHeight)
{
  widget->totalUnits_Width = unitWidth;
  widget->totalUnits_Height = unitHeight;

  widget->pixelsPerUnit_Width = widget->common.sdlRect.w / (float)widget->totalUnits_Width; // Widget's width divided by widget's total width units
  widget->pixelsPerUnit_Height = widget->common.sdlRect.h / (float)widget->totalUnits_Height; // Widget's height divided by widget's total height units
}

// Cursor Over Widget
int isCursorOverWidget(Widget *widget, int x, int y)
{
  // If cursor is in widget's coordinates
  if (x > widget->common.sdlRect.x &&
      x < (widget->common.sdlRect.x + widget->common.sdlRect.w) &&
      y > widget->common.sdlRect.y &&
      y < (widget->common.sdlRect.y + widget->common.sdlRect.h))
  {
    return 0; // On success
  }
  return -1; // On failure
}

Widget *getWidgetFromPos(Window *window, int x, int y)
{
  Widget *widget;

  for (int i = 0; i < window->widgetnum; i++)
  {
    if (window->widgetarray[i]->common.isVisible == true)
    {
      // Check if X and Y within widget's dimensions
      if (x > window->widgetarray[i]->common.sdlRect.x &&
          x < (window->widgetarray[i]->common.sdlRect.x + window->widgetarray[i]->common.sdlRect.w) &&
          y > window->widgetarray[i]->common.sdlRect.y &&
          y < (window->widgetarray[i]->common.sdlRect.y + window->widgetarray[i]->common.sdlRect.h))
      {
        widget = window->widgetarray[i];
        return widget;
      }
    }
  }
  return NULL;
}

// Render Widget
void Widget_render(Widget *widget, SDL_Renderer *renderer)
{
  // Only render widgets which are visible
  if (widget->common.isVisible)
  {
    // Render Widget Colours
    if (widget->type == WIDGET_TYPE_BUTTON)
    {
      switch (widget->common.state)
      {
        case (WIDGET_STATE_DEFAULT):
          SDL_SetRenderDrawColor(renderer, widget->common.r_default, widget->common.g_default, widget->common.b_default, 255);
          SDL_RenderFillRect(renderer, &(widget->common.sdlRect));
          break;
        case (WIDGET_STATE_HOVER):
          SDL_SetRenderDrawColor(renderer, widget->common.r_hover, widget->common.g_hover, widget->common.b_hover, 255);
          SDL_RenderFillRect(renderer, &(widget->common.sdlRect));
          break;
        case (WIDGET_STATE_CLICK):
          SDL_SetRenderDrawColor(renderer, widget->common.r_press, widget->common.g_press, widget->common.b_press, 255);
          SDL_RenderFillRect(renderer, &(widget->common.sdlRect));
      }
    }
    else if (widget->type == WIDGET_TYPE_GAME)
    {
      SDL_SetRenderDrawColor(renderer, widget->common.r_default, widget->common.g_default, widget->common.b_default, 255);
      SDL_RenderFillRect(renderer, &(widget->common.sdlRect));
    }

    // Render Fonts
    if (widget->type == WIDGET_TYPE_BUTTON && widget->button.font != NULL)
    {

      SDL_Color rgba;
      rgba.r = 0;
      rgba.g = 0;
      rgba.b = 0;
      rgba.a = 255;

      SDL_Surface *fontSurface = TTF_RenderText_Blended(widget->button.font->sdlFont, widget->button.text, rgba);
      SDL_Surface *windowSurface = SDL_GetWindowSurface(widget->common.window->sdlWindow);
      SDL_SetSurfaceBlendMode(windowSurface, SDL_BLENDMODE_BLEND);

      // Text Position
      SDL_Rect rect = widget->common.sdlRect;
      rect.x = rect.x + ((rect.w - fontSurface->w) / 2);
      rect.y = rect.y + ((rect.h - fontSurface->h) / 2);

      SDL_BlitSurface(fontSurface, NULL, windowSurface, &rect);
      SDL_FreeSurface(fontSurface);
    }
  }
}
