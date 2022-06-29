#pragma once

typedef struct WidgetGame WidgetGame;

// Game Objects

typedef enum EnemyType EnemyType;
typedef enum ObjectType ObjectType;
typedef enum ObjectMovementDirection ObjectMovementDirection;
typedef enum ObjectArrayType ObjectArrayType;
typedef struct ObjectCommon ObjectCommon;
typedef struct ObjectPlayer ObjectPlayer;
typedef struct ObjectEnemy ObjectEnemy;
typedef struct ObjectProp ObjectProp;
typedef enum ObjectProjectileType ObjectProjectileType;
typedef struct ObjectEffect ObjectEffect;
typedef struct ObjectProjectile ObjectProjectile;
typedef struct ObjectHUD ObjectHUD;

typedef union Object Object;
typedef struct ObjectTemplate ObjectTemplate;

typedef struct Explosion Explosion;
typedef struct ExplosionParticle ExplosionParticle;

typedef struct PlayerExplosion PlayerExplosion;

typedef enum StarType StarType;
typedef struct Star Star;
typedef struct Starfield Starfield;

enum EnemyType
{
  ENEMY_TYPE_1 = 0,
  ENEMY_TYPE_2 = 1,
};

enum StarType
{
  STAR_TYPE_SMALL = 0,
  STAR_TYPE_LARGE = 1,
};

struct Star
{
  StarType type;
  float pos_x;
  float pos_y;
  float velocity;
};

struct Starfield
{
  Star starArray[2000];
  int starArrayNum;
  bool isStarfieldActive;

  int totalStars;
  float smallStarPercent;
  float largeStarPercent;
  int largeStarMinVelocity;
  int largeStarMaxVelocity;
  int smallStarMinVelocity;
  int smallStarMaxVelocity;

  WidgetGame *widgetGame;
};

struct ExplosionParticle
{
  float pos_x;
  float pos_y;
  int buffer_x;
  int buffer_y;
  float r;
  float g;
  float b;
  float a;
  float particleSpeed;
  float alphaFade;
  float deltaX;
  float deltaY;

  // References
  Explosion *explosion;
};

struct Explosion
{
  ExplosionParticle explosionParticleArray[3000];
  int explosionParticleArrayNum;
  bool isActive;
  float pos_x;
  float pos_y;
  int densityMultiplier;
  SDL_Surface *imageSurface;

  // references
  Object *object;
};

struct PlayerExplosion
{
  bool isActive;

  float pos_x;
  float pos_y;

  float currentRadius;
  float thickness;
  float unitsPerSecond;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  float a;

  // references
  Object *object;
};

enum ObjectType
{
  OBJECT_TYPE_PLAYER = 1,
  OBJECT_TYPE_ENEMY = 2,
  OBJECT_TYPE_PROP = 3,
  OBJECT_TYPE_EFFECT = 4,
  OBJECT_TYPE_PROJECTILE = 5,
  OBJECT_TYPE_HUD = 6,
};

enum ObjectMovementDirection
{
  OBJECT_MOVEMENTDIRECTION_NONE = 0,
  OBJECT_MOVEMENTDIRECTION_NORTH = 1,
  OBJECT_MOVEMENTDIRECTION_SOUTH = 2,
  OBJECT_MOVEMENTDIRECTION_EAST = 3,
  OBJECT_MOVEMENTDIRECTION_WEST = 4,
  OBJECT_MOVEMENTDIRECTION_NORTHEAST = 5,
  OBJECT_MOVEMENTDIRECTION_SOUTHEAST = 6,
  OBJECT_MOVEMENTDIRECTION_NORTHWEST = 7,
  OBJECT_MOVEMENTDIRECTION_SOUTHWEST = 8,
  OBJECT_MOVEMENTDIRECTION_FORWARD = 9,
  OBJECT_MOVEMENTDIRECTION_BACKWARD = 10,
  OBJECT_MOVEMENTDIRECTION_LEFT = 11,
  OBJECT_MOVEMENTDIRECTION_RIGHT = 12
};

// Game Object: Effect Type
enum ObjectProjectileType
{
  OBJECT_PROJECTILETYPE_NONE = 0,
  OBJECT_PROJECTILETYPE_BULLET1 = 1,
  OBJECT_PROJECTILETYPE_BULLET2 = 2,
};

enum ObjectArrayType
{
  OBJECT_ARRAYTYPE_NONE = 0,
  OBJECT_ARRAYTYPE_PLAYER = 1,
  OBJECT_ARRAYTYPE_PLAYERPROJECTILES = 2,
  OBJECT_ARRAYTYPE_Enemy = 3,
  OBJECT_ARRAYTYPE_EnemyPROJECTILES = 4,
};


struct ObjectTemplate
{
  ObjectType type;

  // Setup
  const char *filename;
  SDL_Texture *sdlTexture;
  SDL_Surface *sdlSurface;
  Uint32 format;
  int access;

  // References
  WidgetGame *widget;
  int arrayElem;

  // Properties
  int texture_w;
  int texture_h;
};

// Game Object: Common
struct ObjectCommon
{
  ObjectType type;

  // References
  ObjectTemplate *objectTemplate;
  WidgetGame *widgetGame;

  // 2D Properties
  float pos_x;
  float pos_y;
  ObjectMovementDirection movementDirection;
  float movementSpeed;

  // Hitbox Info
  int hitboxArray[10][4]; // pixels | x1, y1, x2, y2
  int hitboxArrayNum;

  // Tracking Info
  bool useHitbox;
  bool useCollision;
  bool isVisible;
};

// Game Object: Player
struct ObjectPlayer
{
  ObjectCommon common;

  // Basic Info
  int health;
  int lives;

  // Statistics
  int stats_points;
  int stats_kills;
  int stats_deaths;

  // Weapon and Projectiles
  int weapon1_ticksWhenFired;
  float weapon1_fireRate;

  ObjectProjectileType weapon1_projectileType;
  ObjectMovementDirection weapon1_direction;
  float weapon1_1_x;
  float weapon1_1_y;
  float weapon1_2_x;
  float weapon1_2_y;
  int weapon1_damage;

  // Status
  bool isShooting;
  bool isControllable;
  bool isDamageable;

  // Death Ticks
  bool isDead;
  int deathTickStart;
  int deathTickDuration;
};

// Game Object: Enemy
struct ObjectEnemy
{
  ObjectCommon common;
  EnemyType enemyType;
  int health;

  // Enemy Spawning
  Uint32 spawnerTicks;
  float spawnerDelay;

  // Weapon and Projectiles
  int weapon1_ticksWhenFired;
  float weapon1_fireRate;
  float weapon1_fireRateBurst;
  ObjectMovementDirection weapon1_direction;
  float weapon1_x;
  float weapon1_y;
  int weapon1_damage;
  int weapon1_speed;
};

// Game Object: Prop
struct ObjectProp
{
  ObjectCommon common;
  int health;
  bool isDamageable;
};

// Game Object: Effect
struct ObjectEffect
{
  ObjectCommon common;
  int pos_direction;
};

// Game Object: Projectile
struct ObjectProjectile
{
  ObjectCommon common;
  int spawnTick;

  int damage;

  float deltaX;
  float deltaY;

  bool hasLocked;
  int lockOnStart;
  int lockOnDuration;

  Object *parentObject;
};

struct ObjectHUD
{
  ObjectCommon common;
};

// Game Object
union Object
{
  ObjectType type;
  ObjectCommon common;
  ObjectPlayer player;
  ObjectEnemy enemy;
  ObjectProp prop;
  ObjectEffect effect;
  ObjectProjectile projectile;
  ObjectHUD hud;
};

void Game_Func_clampObjectInsideGame(Object *object);
void Game_Func_clampObjectOutsideObject(Object *object1, Object *object2);
int Game_Func_checkGameBorder(Object *object);
bool Game_Func_checkObjectCollision(Object *object1, Object *object2);
void Game_Func_moveObjectAwayFromObject(Object *object1, Object *object2);
void Game_Func_fetchAndSetPlayerWeapons(WidgetGame *game);
void Game_Func_fetchAndSetPlayerMovement(WidgetGame *game);
void Game_Func_moveObject(Object *object);
void Game_Func_moveObjectTowardsPos(Object *object, float pos_x, float pos_y, float unitsPerSecond);
void Game_Func_moveObjectAlongDelta(Object *object, float unitsPerSecond, float deltaX, float deltaY);
void Game_func_createPlayerExplosion(Object *object, float pos_x, float pos_y, float startRadius, float unitsPerSecond, int thickness);
void Game_Func_updatePlayerExplosion(PlayerExplosion *playerExplosion);
void Game_initStarfield(Starfield *starfield, WidgetGame *widgetGame, int totalStars, float smallStarPercent, float largeStarPercent, int smallStarMinVelocity, int smallStarMaxVelocity, int largeStarMinVelocity, int largeStarMaxVelocity);
void Game_updateStarfield(Starfield *starfield);
void Game_initObjectExplosion(Object *explodingObject, Explosion *explosion, int densityMultiplier, int particleSpeed, int alphaFade);
void Game_updateObjectExplosion(Explosion *explosion);
void Game_Func_mainLoop(WidgetGame *game);
void Game_ObjectTemplate_create(ObjectTemplate *objectTemplate, WidgetGame *widgetgame, ObjectType type, const char *filename);
void Game_Object_constructObjectCommon(ObjectCommon *objectCommon, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable);
void Game_Object_constructObjectPlayer(ObjectPlayer *objectPlayer, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable);
void Game_Object_constructObjectEnemy(ObjectEnemy *objectEnemy, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable);
void Game_Object_constructObjectProp(ObjectProp *objectProp, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable);
void Game_Object_constructObjectEffect(ObjectEffect *objectEffect, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable);
void Game_Object_constructObjectProjectile(ObjectProjectile *objectProjectile, ObjectTemplate *objectTemplate, Object *parentObject, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable);
void Game_Object_constructObjectHUD(ObjectHUD *objectHUD, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable);
void Game_Starfield_render(Starfield *starfield);
void Game_Explosion_render(Explosion *explosion);
void Game_playerExplosion_render(PlayerExplosion *playerExplosion);
void Game_Object_render(Object *object);

