/*////////////////////////
// Configurable Variables:
*/////////////////////////

#define CFG_WIDTH 814
#define CFG_HEIGHT 900
#define CFG_DIVSIZE 12
#define CFG_FPS 300.0f
#define CFG_GCPS 300.0f
#define CFG_GAMEUNITSWIDTH 150
#define CFG_GAMEUNITSHEIGHT 150

#define MAX_ENEMY_OBJECTS 30
#define MAX_PLAYERPROJECTILE_OBJECTS 60
#define MAX_ENEMYPROJECTILE_OBJECTS 60
#define MAX_HUD_OBJECTS 2
#define MAX_EXPLOSIONS 10
#define MAX_EXPLOSION_PARTICLES 4000
#define MAX_STARS 2000

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

typedef struct ObjectTemplate ObjectTemplate;
typedef struct SoundEffect SoundEffect;
typedef struct Font Font;

extern ObjectTemplate enemy1;
extern ObjectTemplate enemy1_bullet;
extern ObjectTemplate enemy2;
extern ObjectTemplate enemy2_bullet;
extern ObjectTemplate spaceShip;
extern ObjectTemplate bullet1;


extern Font *mainfont;
extern Font *newfont;
extern Font *scoreFont;

extern SoundEffect playerShooting;
extern SoundEffect playerOuch;
extern SoundEffect playerOuttaMyWay;
extern SoundEffect enemyBoom;

extern SDL_Surface *hudHealth_imageSurface;
extern SDL_Surface *hudStats_imageSurface;
extern SDL_Surface *enemy1_imageSurface;
extern SDL_Surface *enemy1Bullet_imageSurface;
extern SDL_Surface *enemy2_imageSurface;
extern SDL_Surface *enemy2Bullet_imageSurface;
extern SDL_Surface *player_imageSurface;
extern SDL_Surface *playerBullet_imageSurface;

#include "dorris_input.h"
#include "dorris_game.h"
#include "dorris_windows.h"
#include "dorris_handles.h"
#include "dorris_widgets.h"
#include "dorris_sounds.h"
#include "dorris_ttf.h"
#include "dorris_misc.h"
