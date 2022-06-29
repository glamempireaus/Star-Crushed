#pragma once

// Outside Structs
typedef struct Window Window;
typedef union Object Object;
typedef struct Font Font;
typedef enum KeyEventButton KeyEventButton;

// Listed Structs
typedef struct WidgetCommon WidgetCommon;
typedef struct WidgetButton WidgetButton;
typedef struct WidgetGame WidgetGame;
typedef enum WidgetType WidgetType;
typedef enum WidgetState WidgetState;
typedef union Widget Widget;

// WIDGETS

// Widget Types
enum WidgetType
{
  WIDGET_TYPE_BUTTON = 0,
  WIDGET_TYPE_GAME = 1
};

// Widget States
enum WidgetState
{
  WIDGET_STATE_DEFAULT = 0,
  WIDGET_STATE_HOVER = 1,
  WIDGET_STATE_CLICK = 2
};

// Widget: Common
struct WidgetCommon
{
  WidgetType type;
  WidgetState state;
  SDL_Rect sdlRect;
  bool isVisible;
  Window *window;

  int x_div;
  int y_div;
  int w_div;
  int h_div;

  Uint8 r_default;
  Uint8 g_default;
  Uint8 b_default;
  Uint8 r_hover;
  Uint8 g_hover;
  Uint8 b_hover;
  Uint8 r_press;
  Uint8 g_press;
  Uint8 b_press;
};

// Widget: Button
struct WidgetButton
{
  WidgetCommon common;

  Font *font;
  char *text;

  bool isMousePressDraggable;
};

// Widget: Game
struct WidgetGame
{
  WidgetCommon common;

  SDL_Surface *surface;

  ObjectPlayer currentPlayer;
  ObjectEnemy enemyArray[MAX_ENEMY_OBJECTS];
  ObjectProjectile playerProjectileArray[MAX_PLAYERPROJECTILE_OBJECTS];
  ObjectProjectile enemyProjectileArray[MAX_ENEMYPROJECTILE_OBJECTS];
  ObjectHUD HUDArray[MAX_HUD_OBJECTS];
  Explosion explosionArray[MAX_EXPLOSIONS];
  Starfield starfield;
  PlayerExplosion playerExplosion;

  ObjectTemplate **objectTemplateArray;
  int objectTemplateArrayNum;
  int totalUnits_Width;
  int totalUnits_Height;
  float pixelsPerUnit_Height;
  float pixelsPerUnit_Width;

  Uint32 enemy1_spawnerTicks;
  float enemy1_spawnerDelay;

  Uint32 enemy2_spawnerTicks;
  float enemy2_spawnerDelay;

  Uint32 enemy3_spawnerTicks;
  float enemy3_spawnerDelay;


  Uint32 ticks_mainLoop;

  bool pressedkey_w;
  bool pressedkey_a;
  bool pressedkey_s;
  bool pressedkey_d;
  bool pressedkey_space;
};

// Widget Union
union Widget
{
  WidgetType type;

  WidgetCommon common;
  WidgetButton button;
  WidgetGame game;
};

void Widget_constructWidgetCommon(WidgetCommon *widgetCommon, Window *window, int div_x, int div_y, int div_w, int div_h, int windowWidth, int windowHeight, int divSize);
void Widget_constructWidgetButton(WidgetButton *widgetButton, Window *window, int div_x, int div_y, int div_w, int div_h, int windowWidth, int windowHeight, int divSize);
void Widget_constructWidgetGame(WidgetGame *widgetGame, Window *window, int div_x, int div_y, int div_w, int div_h, int windowWidth, int windowHeight, int divSize);
void Widget_render(Widget *widget, SDL_Renderer *renderer);

void Widget_enableRendering(Widget *widget);
void Widget_disable(Widget *widget);
void WidgetButton_setMousePressDraggable(WidgetButton *widgetButton, bool isMousePressDraggable);
void Widget_setRGBDefault(Widget *widget, Uint8 r, Uint8 g, Uint8 b);
void Widget_setRGBHover(Widget *widget, Uint8 r, Uint8 g, Uint8 b);
void Widget_setRGBPress(Widget *widget, Uint8 r, Uint8 g, Uint8 b);
void Widget_setText(Widget *widget, char *text, Font *font);

ObjectTemplate *WidgetGame_addObjectTemplate(ObjectTemplate *objectTemplate, WidgetGame *widgetGame);
ObjectEnemy *WidgetGame_findUnusedEnemyInArray(WidgetGame *widgetGame);
ObjectProjectile *WidgetGame_findUnusedEnemyProjectileInArray(WidgetGame *widgetGame);
ObjectProjectile *WidgetGame_findUnusedPlayerProjectileInArray(WidgetGame *widgetGame);
ObjectHUD *WidgetGame_findUnusedHUDInArray(WidgetGame *widgetGame);
void WidgetGame_addObjectToPlayerProjectileArray(ObjectProjectile *object);
Explosion *WidgetGame_findUnusedExplosionInArray(WidgetGame *widgetGame);

void WidgetGame_renderObjects(WidgetGame *game);
void WidgetGame_renderExplosions(WidgetGame *widgetGame);
void WidgetGame_setUnitData(WidgetGame *widget, int unitWidth, int unitHeight);

int isCursorOverWidget(Widget *widgetname, int x, int y);
Widget *getWidgetFromPos(Window *window, int x, int y);



