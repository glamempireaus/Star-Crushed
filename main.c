/*
 * main.c
 *
 *  Created on: 6 Aug 2019
 *      Author: dorris
 */

#include "includes.h"

ObjectTemplate spaceShip;
ObjectTemplate bullet1;
ObjectTemplate enemy1;
ObjectTemplate enemy1_bullet;
ObjectTemplate enemy2;
ObjectTemplate enemy2_bullet;
ObjectTemplate hudHealth;
ObjectTemplate hudStats;

Font *mainfont;
Font *newfont;
Font *scoreFont;

SoundEffect playerShooting;
SoundEffect playerOuch;
SoundEffect playerOuttaMyWay;
SoundEffect enemyBoom;

SDL_Surface *hudHealth_imageSurface;
SDL_Surface *hudStats_imageSurface;
SDL_Surface *enemy1_imageSurface;
SDL_Surface *enemy1Bullet_imageSurface;
SDL_Surface *enemy2_imageSurface;
SDL_Surface *enemy2Bullet_imageSurface;
SDL_Surface *player_imageSurface;
SDL_Surface *playerBullet_imageSurface;

Widget *chosenwidget = NULL;
int gameState = 0;

int main(int argc, char *argv[])
{
  /*/////////////////////
  // API Initialisations:
  *//////////////////////

  // SDL Main
  SDL_Init(SDL_INIT_EVERYTHING);

  // SDL TTF
  if (TTF_Init() == -1)
  {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(2);
  }

  // SDL IMG
  int imgFlags = 0x00000001 | 0x00000002;
  if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
  {
    printf("IMG_Init: %s\n", IMG_GetError());
    exit(2);
  }

  // load support for the OGG and MOD sample/music formats
  // Mixer Sounds
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
  {
    return false;
  }
  else
  {
    Mix_AllocateChannels(16);
  }
  int mixerFlags = MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_MP3;
  if ((Mix_Init(mixerFlags) & mixerFlags) != mixerFlags)
  {
    printf("Mix_Init: %s\n", Mix_GetError());
    exit(2);
  }

  /*//////////////////
  // Struct Instances:
  *///////////////////

  // Window
  Window *mainwindow = Window_create("dorris box", CFG_WIDTH, CFG_HEIGHT, CFG_GCPS, CFG_FPS);
  Window_setBackgroundColor(mainwindow, 255, 255, 255);

  int window_w = CFG_WIDTH;
  int window_h = CFG_HEIGHT;

  // Fonts
  mainfont = Font_create("ebrima.ttf", 26, FONT_STYLE_NORMAL, 0, FONT_HINTING_NORMAL, 1);
  newfont = Font_create("ebrima.ttf", 26, FONT_STYLE_BOLD, 0, FONT_HINTING_NORMAL, 1);
  scoreFont = Font_create("ebrima.ttf", 27, FONT_STYLE_ITALIC, 0, FONT_HINTING_NORMAL, 1);

  // Widgets

  // Top Buttons
  WidgetButton navbar1;
  Widget_constructWidgetButton(&navbar1, mainwindow, 0, 0, 4, 1.5, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&navbar1, 255,0,0);
  Widget_setRGBHover((Widget *)&navbar1, 155,0,0);
  Widget_setRGBPress((Widget *)&navbar1, 100,0,0);
  Widget_setText((Widget *)&navbar1, "First Page", mainfont);
  Widget_enableRendering((Widget *)&navbar1);

  WidgetButton navbar2;
  Widget_constructWidgetButton(&navbar2, mainwindow, 4, 0, 4, 1.5, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&navbar2, 0,255,0);
  Widget_setRGBHover((Widget *)&navbar2, 0,155,0);
  Widget_setRGBPress((Widget *)&navbar2, 0,100,0);
  Widget_setText((Widget *)&navbar2, "Second Page", mainfont);
  Widget_enableRendering((Widget *)&navbar2);

  WidgetButton navbar3;
  Widget_constructWidgetButton(&navbar3, mainwindow, 8, 0, 4, 1.5, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&navbar3, 0,0,255);
  Widget_setRGBHover((Widget *)&navbar3, 0,0,155);
  Widget_setRGBPress((Widget *)&navbar3, 0,0,100);
  Widget_setText((Widget *)&navbar3, "Third Page", newfont);
  Widget_enableRendering((Widget *)&navbar3);

  // First Page
  WidgetButton button1_1;
  Widget_constructWidgetButton(&button1_1, mainwindow, 0, 4, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&button1_1, 150,0,100);
  Widget_setRGBHover((Widget *)&button1_1, 100,0,50);
  Widget_setRGBPress((Widget *)&button1_1, 75,0,25);
  Widget_setText((Widget *)&button1_1, "TEST1", newfont);

  WidgetButton button1_2;
  Widget_constructWidgetButton(&button1_2, mainwindow, 4, 4, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&button1_2, 150,0,100);
  Widget_setRGBHover((Widget *)&button1_2, 100,0,50);
  Widget_setRGBPress((Widget *)&button1_2, 75,0,25);
  Widget_setText((Widget *)&button1_2, "TEST2", newfont);

  WidgetButton button1_3;
  Widget_constructWidgetButton(&button1_3, mainwindow, 8, 4, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&button1_3, 150,0,100);
  Widget_setRGBHover((Widget *)&button1_3, 100,0,50);
  Widget_setRGBPress((Widget *)&button1_3, 75,0,25);
  Widget_setText((Widget *)&button1_3, "TEST3", newfont);

  // First Page
  WidgetButton button2_1;
  Widget_constructWidgetButton(&button2_1, mainwindow, 0, 8, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&button2_1, 70,150,100);
  Widget_setRGBHover((Widget *)&button2_1, 50,111,111);
  Widget_setRGBPress((Widget *)&button2_1, 30,50,50);
  Widget_setText((Widget *)&button2_1, "you", newfont);

  WidgetButton button2_2;
  Widget_constructWidgetButton(&button2_2, mainwindow, 4, 8, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&button2_2, 70,150,100);
  Widget_setRGBHover((Widget *)&button2_2, 50,111,111);
  Widget_setRGBPress((Widget *)&button2_2, 30,50,50);
  Widget_setText((Widget *)&button2_2, "you", newfont);

  WidgetButton button2_3;
  Widget_constructWidgetButton(&button2_3, mainwindow, 8, 8, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&button2_3, 70,150,100);
  Widget_setRGBHover((Widget *)&button2_3, 50,111,111);
  Widget_setRGBPress((Widget *)&button2_3, 30,50,50);
  Widget_setText((Widget *)&button2_3, "you", newfont);

  // Game Buttons
  WidgetButton buttongame_play;
  Widget_constructWidgetButton(&buttongame_play, mainwindow, 4, 4, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&buttongame_play, 70,150,100);
  Widget_setRGBHover((Widget *)&buttongame_play, 50,111,111);
  Widget_setRGBPress((Widget *)&buttongame_play, 30,50,50);
  Widget_setText((Widget *)&buttongame_play, "Play The Game", newfont);

  WidgetButton buttongame_options;
  Widget_constructWidgetButton(&buttongame_options, mainwindow, 4, 8, 4, 1, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&buttongame_options, 70,150,100);
  Widget_setRGBHover((Widget *)&buttongame_options, 50,111,111);
  Widget_setRGBPress((Widget *)&buttongame_options, 30,50,50);
  Widget_setText((Widget *)&buttongame_options, "Configure The Game", newfont);

  // Game Wrapper
  WidgetGame gameWidget;
  Widget_constructWidgetGame(&gameWidget, mainwindow, 0, 1.5, 12, 11, CFG_WIDTH, CFG_HEIGHT, CFG_DIVSIZE);
  Widget_setRGBDefault((Widget *)&gameWidget, 10,10,10);
  WidgetGame_setUnitData(&gameWidget, CFG_GAMEUNITSWIDTH, CFG_GAMEUNITSHEIGHT);

  //Game Objects
  Game_ObjectTemplate_create(&spaceShip, &gameWidget, OBJECT_TYPE_PLAYER, "assets/player.png");
  Game_ObjectTemplate_create(&bullet1, &gameWidget, OBJECT_TYPE_PROJECTILE, "assets/bullet1.png");
  Game_ObjectTemplate_create(&enemy1, &gameWidget, OBJECT_TYPE_ENEMY, "assets/enemy1.png");
  Game_ObjectTemplate_create(&enemy1_bullet, &gameWidget, OBJECT_TYPE_PROJECTILE, "assets/enemy1_bullet.png");
  Game_ObjectTemplate_create(&enemy2, &gameWidget, OBJECT_TYPE_ENEMY, "assets/enemy2.png");
  Game_ObjectTemplate_create(&enemy2_bullet, &gameWidget, OBJECT_TYPE_PROJECTILE, "assets/enemy2_bullet.png");
  Game_ObjectTemplate_create(&hudHealth, &gameWidget, OBJECT_TYPE_HUD, "assets/hud_health.png");
  Game_ObjectTemplate_create(&hudStats, &gameWidget, OBJECT_TYPE_HUD, "assets/hud_stats.png");

  ObjectPlayer player;
  Game_Object_constructObjectPlayer(&player, &spaceShip,
                             (gameWidget.common.sdlRect.x + ((gameWidget.common.sdlRect.w - spaceShip.texture_w) / 2)),
                             (gameWidget.common.sdlRect.y + ((gameWidget.common.sdlRect.h - spaceShip.texture_h) / 2)),
                             100,
                             0,
                             0,
                             true);

  ObjectHUD *healthbar = WidgetGame_findUnusedHUDInArray(&gameWidget);
  Game_Object_constructObjectHUD(healthbar, &hudHealth,
                             .2 * (gameWidget.common.sdlRect.w / 10),
                             10.2 * (gameWidget.common.sdlRect.h / 10),
                             0,
                             0,
                             0,
                             true);

  ObjectHUD *statsbar = WidgetGame_findUnusedHUDInArray(&gameWidget);
  Game_Object_constructObjectHUD(statsbar, &hudStats,
                             .1 * (gameWidget.common.sdlRect.w / 10),
                             1 *  (gameWidget.common.sdlRect.h / 10),
                             0,
                             0,
                             0,
                             true);

  gameWidget.currentPlayer = player;

  player_imageSurface = IMG_Load("assets/player.png");
  playerBullet_imageSurface = IMG_Load("assets/bullet1.png");
  enemy1_imageSurface = IMG_Load("assets/enemy1.png");
  enemy1Bullet_imageSurface = IMG_Load("assets/enemy1_bullet.png");
  enemy2_imageSurface = IMG_Load("assets/enemy2.png");
  enemy2Bullet_imageSurface = IMG_Load("assets/enemy2_bullet.png");
  hudHealth_imageSurface = IMG_Load("assets/hud_health.png");
  hudStats_imageSurface = IMG_Load("assets/hud_stats.png");

  Sound_constructSoundEffect(&playerShooting, "assets/playerShooting.wav");
  Sound_constructSoundEffect(&enemyBoom, "assets/enemyExplosion.wav");
  Sound_constructSoundEffect(&playerOuch, "assets/ouch.mp3");
  Sound_constructSoundEffect(&playerOuttaMyWay, "assets/outtamyway.mp3");

  /*///////////////
  // Process Loops:
  *////////////////

  // Handle SDL Events
  SDL_Event sdlEvent;

  //// Main Window Loop
  while (!(mainwindow->isClosed))
  {
    while (SDL_PollEvent(&sdlEvent) != 0)
    {
      // Cycle through events
      switch (sdlEvent.type)
      {
        // Mouse Movement Events
        case SDL_MOUSEMOTION:
        {
          MouseEvent mouseEvent;
          MouseEvent_createFromSDLEvent(&mouseEvent, sdlEvent);
          handle_cursorOverWindow(mainwindow, &mouseEvent);
          break;
        }

        // Mouse Button Events
        case SDL_MOUSEBUTTONDOWN:
        {
          if (sdlEvent.button.button == SDL_BUTTON_LEFT)
          {
            MouseEvent mouseEvent;
            MouseEvent_createFromSDLEvent(&mouseEvent, sdlEvent);
            handle_cursorOverWindow(mainwindow, &mouseEvent);
          }
          break;
        }

        case SDL_MOUSEBUTTONUP:
        {
          if (sdlEvent.button.button == SDL_BUTTON_LEFT)
          {
            MouseEvent mouseEvent;
            MouseEvent_createFromSDLEvent(&mouseEvent, sdlEvent);

            // Check if left click is on a Navbar Widget
            Widget *widgetcheck = handle_cursorOverWindow(mainwindow, &mouseEvent);
            if (widgetcheck != NULL && widgetcheck->common.type == WIDGET_TYPE_BUTTON)
            {
              chosenwidget = widgetcheck;

              // Choose widget if it's clicked
              if (chosenwidget == (Widget *)&navbar1)
              {
                gameState = 0;
                Window_disableWidgetRendering(mainwindow);
                Widget_enableRendering((Widget *)&button1_1);
                Widget_enableRendering((Widget *)&button1_2);
                Widget_enableRendering((Widget *)&button1_3);
              }
              else if (chosenwidget == (Widget *)&navbar2)
              {
                gameState = 0;
                Window_disableWidgetRendering(mainwindow);
                Widget_enableRendering((Widget *)&button2_1);
                Widget_enableRendering((Widget *)&button2_2);
                Widget_enableRendering((Widget *)&button2_3);
              }
              else if (chosenwidget == (Widget *)&navbar3)
              {
                if (!gameState)
                {
                  Window_disableWidgetRendering(mainwindow);
                  gameState = 2;
                  Widget_enableRendering((Widget *)&buttongame_play);
                  Widget_enableRendering((Widget *)&buttongame_options);
                }
              }
              else if (widgetcheck == (Widget *)&buttongame_play)
              {
                gameState = 1;
              }
              else if (widgetcheck == (Widget *)&buttongame_options)
              {
                // To do
              }

              // Allow Render for Navbar Widgets
              Widget_enableRendering((Widget *)&navbar1);
              Widget_enableRendering((Widget *)&navbar2);
              Widget_enableRendering((Widget *)&navbar3);
            }
          }
          break;
        }

        case SDL_KEYDOWN:
        {
          KeyEvent keyEvent;
          KeyEvent_createFromSDLEvent(&keyEvent, sdlEvent);
          if (gameState == 1)
          {
            handle_KeyPressOverGame(&gameWidget, &keyEvent);
          }

          // Pause Function
          if (keyEvent.button == KEYEVENT_BUTTON_ESCAPE)
          {
            if (gameState == 2)
            {
              gameState = 1;
            }
            else if (gameState == 1)
            {
              gameState = 2;
            }
          }
          break;
        }

        case SDL_KEYUP:
        {
          if (gameState == 1)
          {
            KeyEvent keyEvent;
            KeyEvent_createFromSDLEvent(&keyEvent, sdlEvent);
            handle_KeyPressOverGame(&gameWidget, &keyEvent);
          }
          break;
        }

        // Closing the application
        case SDL_QUIT:
        {
          TTF_Quit();
          IMG_Quit();
          SDL_Quit();
          Mix_Quit();
          return 0;
        }
      }
    }

    // Resize Window if Changed
    SDL_GetWindowSize(mainwindow->sdlWindow, &window_w, &window_h);
    Window_resizeWidgets(mainwindow, CFG_DIVSIZE, window_w, window_h);

    // Game Logic

    // If playing game
    if (gameState == 1 )
    {
      if (SDL_GetTicks() > gameWidget.ticks_mainLoop + (1000 / CFG_GCPS))
      {
        gameWidget.ticks_mainLoop = SDL_GetTicks();
        Game_Func_mainLoop(&gameWidget);
      }
    }

    // Render Widgets
    SDL_SetRenderDrawColor(mainwindow->sdlRenderer, 255, 255, 255, 255); // Draw render colour to white
    SDL_RenderClear(mainwindow->sdlRenderer); // Clear render to white
    if (gameState == 1)
    {
      Widget_enableRendering((Widget *)&gameWidget);
      Window_renderWidgets(mainwindow); // Render Widgets overtop of background
      Game_Starfield_render(&gameWidget.starfield);
      Game_playerExplosion_render(&gameWidget.playerExplosion);
      WidgetGame_renderExplosions(&gameWidget);
      WidgetGame_renderObjects(&gameWidget);
    }
    else
    {
      if (gameState == 2)
      {
        Widget_enableRendering((Widget *)&gameWidget);
      }
      Window_renderWidgets(mainwindow); // Render Widgets overtop of background
      Widget_render((Widget *)&buttongame_play,mainwindow->sdlRenderer);
      Widget_render((Widget *)&buttongame_options,mainwindow->sdlRenderer);
    }

    SDL_RenderPresent(mainwindow->sdlRenderer); // Present rendered pixels to display
  }
  return 0;
}

