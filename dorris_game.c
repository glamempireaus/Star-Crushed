#include "includes.h"

// Global Variables

int collisionTotalTicks = 200;
int collisionCurrentTick = 0;
int collisionStartTick = 0;
Object *collisionObject = NULL;
int currentChannel = -2;

// Functions

// Create Game Object
void Game_ObjectTemplate_create(ObjectTemplate *objectTemplate, WidgetGame *widgetgame, ObjectType type, const char *filename)
{
  // Set attributes to game object
  objectTemplate->type = type;
  objectTemplate->filename = filename;
  objectTemplate->widget = widgetgame;
  objectTemplate->sdlTexture = IMG_LoadTexture(objectTemplate->widget->common.window->sdlRenderer, filename);
  objectTemplate->sdlSurface = IMG_Load(filename);

  if(!objectTemplate->sdlSurface) {
      printf("IMG_Load: %s\n", IMG_GetError());

      // handle error
      exit(55);
  }

  SDL_QueryTexture(objectTemplate->sdlTexture, &objectTemplate->format, &objectTemplate->access, &objectTemplate->texture_w, &objectTemplate->texture_h);

  WidgetGame_addObjectTemplate(objectTemplate, widgetgame);
}

// Spawning an Object into the object array
void Game_Object_constructObjectCommon(ObjectCommon *objectCommon, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable)
{
  objectCommon->type = objectTemplate->type;
  objectCommon->objectTemplate = objectTemplate;
  objectCommon->widgetGame = objectTemplate->widget;
  objectCommon->pos_x = pos_x;
  objectCommon->pos_y = pos_y;

  objectCommon->movementDirection = OBJECT_MOVEMENTDIRECTION_NONE;
  objectCommon->movementSpeed = movementSpeed;

  objectCommon->hitboxArrayNum = 0;

  objectCommon->isVisible = true;
  objectCommon->useCollision = true;
  objectCommon->useHitbox = false;
}

void Game_Object_constructObjectPlayer(ObjectPlayer *objectPlayer, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable)
{
  Game_Object_constructObjectCommon((ObjectCommon *)objectPlayer, objectTemplate, pos_x, pos_y, movementSpeed, health, lives, isDamageable);
  objectPlayer->common.type = OBJECT_TYPE_PLAYER;
  objectPlayer->health = 100;
  objectPlayer->lives = 3;

  objectPlayer->isShooting = false;
  objectPlayer->isDamageable = true;
  objectPlayer->isControllable = true;

  objectPlayer->stats_deaths = 0;
  objectPlayer->stats_kills = 0;
  objectPlayer->stats_points = 0;

  // Weapons:

  if (strcmp(objectTemplate->filename,"assets/player.png") == 0)
  {
    objectPlayer->weapon1_ticksWhenFired = 0;
    objectPlayer->weapon1_fireRate = 20;
    objectPlayer->weapon1_projectileType = OBJECT_PROJECTILETYPE_NONE;
    objectPlayer->weapon1_direction = OBJECT_MOVEMENTDIRECTION_NORTH;
    objectPlayer->weapon1_1_x = 6;
    objectPlayer->weapon1_1_y = 28;
    objectPlayer->weapon1_2_x = 37;
    objectPlayer->weapon1_2_y = 28;
    objectPlayer->weapon1_damage = 1;

    // Hitbox Generation:

    objectPlayer->common.useHitbox = true;

    // Top
    objectPlayer->common.hitboxArray[0][0] = 21;
    objectPlayer->common.hitboxArray[0][1] = 1;
    objectPlayer->common.hitboxArray[0][2] = 28;
    objectPlayer->common.hitboxArray[0][3] = 15;

    // Middle
    objectPlayer->common.hitboxArray[1][0] = 17;
    objectPlayer->common.hitboxArray[1][1] = 16;
    objectPlayer->common.hitboxArray[1][2] = 31;
    objectPlayer->common.hitboxArray[1][3] = 59;

    // Bottom left
    objectPlayer->common.hitboxArray[2][0] = 1;
    objectPlayer->common.hitboxArray[2][1] = 41;
    objectPlayer->common.hitboxArray[2][2] = 17;
    objectPlayer->common.hitboxArray[2][3] = 55;

    // Bottom Right
    objectPlayer->common.hitboxArray[3][0] = 32;
    objectPlayer->common.hitboxArray[3][1] = 41;
    objectPlayer->common.hitboxArray[3][2] = 48;
    objectPlayer->common.hitboxArray[3][3] = 55;

    // Side Left Close
    objectPlayer->common.hitboxArray[4][0] = 12;
    objectPlayer->common.hitboxArray[4][1] = 28;
    objectPlayer->common.hitboxArray[4][2] = 16;
    objectPlayer->common.hitboxArray[4][3] = 40;

    // Side Right Close
    objectPlayer->common.hitboxArray[5][0] = 33;
    objectPlayer->common.hitboxArray[5][1] = 28;
    objectPlayer->common.hitboxArray[5][2] = 37;
    objectPlayer->common.hitboxArray[5][3] = 40;

    // Side Left Mid
    objectPlayer->common.hitboxArray[6][0] = 7;
    objectPlayer->common.hitboxArray[6][1] = 33;
    objectPlayer->common.hitboxArray[6][2] = 11;
    objectPlayer->common.hitboxArray[6][3] = 40;

    // Side Right Mid
    objectPlayer->common.hitboxArray[7][0] = 38;
    objectPlayer->common.hitboxArray[7][1] = 33;
    objectPlayer->common.hitboxArray[7][2] = 42;
    objectPlayer->common.hitboxArray[7][3] = 40;

    // Side Left Far
    objectPlayer->common.hitboxArray[8][0] = 2;
    objectPlayer->common.hitboxArray[8][1] = 36;
    objectPlayer->common.hitboxArray[8][2] = 6;
    objectPlayer->common.hitboxArray[8][3] = 40;

    // Side Right Far
    objectPlayer->common.hitboxArray[9][0] = 43;
    objectPlayer->common.hitboxArray[9][1] = 36;
    objectPlayer->common.hitboxArray[9][2] = 47;
    objectPlayer->common.hitboxArray[9][3] = 40;

    objectPlayer->common.hitboxArrayNum = 10;
  }
}

void Game_Object_constructObjectEnemy(ObjectEnemy *objectEnemy, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable)
{
  Game_Object_constructObjectCommon((ObjectCommon *)objectEnemy, objectTemplate, pos_x, pos_y, movementSpeed, health, lives, isDamageable);
  objectEnemy->common.type = OBJECT_TYPE_ENEMY;

  if (strcmp(objectTemplate->filename,"assets/enemy1.png") == 0)
  {
    objectEnemy->enemyType = ENEMY_TYPE_1;
    objectEnemy->health = 50;

    // Weapons
    objectEnemy->weapon1_ticksWhenFired = 0;
    objectEnemy->weapon1_fireRate = 2600;
    objectEnemy->weapon1_fireRateBurst = 0;
    objectEnemy->weapon1_direction = OBJECT_MOVEMENTDIRECTION_SOUTH;
    objectEnemy->weapon1_x = 28;
    objectEnemy->weapon1_y = 67;
    objectEnemy->weapon1_damage = 1;
    objectEnemy->weapon1_speed = 30;

    // Hitbox Generation:

    objectEnemy->common.useHitbox = true;

    // Middle
    objectEnemy->common.hitboxArray[0][0] = 24;
    objectEnemy->common.hitboxArray[0][1] = 0;
    objectEnemy->common.hitboxArray[0][2] = 40;
    objectEnemy->common.hitboxArray[0][3] = 68;

    // Middle Left
    objectEnemy->common.hitboxArray[1][0] = 21;
    objectEnemy->common.hitboxArray[1][1] = 23;
    objectEnemy->common.hitboxArray[1][2] = 23;
    objectEnemy->common.hitboxArray[1][3] = 61;

    // Middle Right
    objectEnemy->common.hitboxArray[2][0] = 41;
    objectEnemy->common.hitboxArray[2][1] = 23;
    objectEnemy->common.hitboxArray[2][2] = 43;
    objectEnemy->common.hitboxArray[2][3] = 61;

    // Far Left
    objectEnemy->common.hitboxArray[3][0] = 0;
    objectEnemy->common.hitboxArray[3][1] = 9;
    objectEnemy->common.hitboxArray[3][2] = 19;
    objectEnemy->common.hitboxArray[3][3] = 36;

    // Far Right
    objectEnemy->common.hitboxArray[4][0] = 45;
    objectEnemy->common.hitboxArray[4][1] = 9;
    objectEnemy->common.hitboxArray[4][2] = 64;
    objectEnemy->common.hitboxArray[4][3] = 36;

    objectEnemy->common.hitboxArrayNum = 5;
  }
  else if (strcmp(objectTemplate->filename,"assets/enemy2.png") == 0)
  {
    objectEnemy->enemyType = ENEMY_TYPE_2;
    objectEnemy->health = 20;

    // Weapon Stats
    objectEnemy->weapon1_ticksWhenFired = 0;
    objectEnemy->weapon1_fireRate = 3000;
    objectEnemy->weapon1_fireRateBurst = 2;
    objectEnemy->weapon1_direction = OBJECT_MOVEMENTDIRECTION_SOUTH;
    objectEnemy->weapon1_x = 28;
    objectEnemy->weapon1_y = 67;
    objectEnemy->weapon1_damage = 1;
    objectEnemy->weapon1_speed = 40;

    // Hitbox Generation:

    objectEnemy->common.useHitbox = true;

    // Middle
    objectEnemy->common.hitboxArray[0][0] = 35;
    objectEnemy->common.hitboxArray[0][1] = 0;
    objectEnemy->common.hitboxArray[0][2] = 44;
    objectEnemy->common.hitboxArray[0][3] = 49;

    // Middle Left
    objectEnemy->common.hitboxArray[1][0] = 26;
    objectEnemy->common.hitboxArray[1][1] = 9;
    objectEnemy->common.hitboxArray[1][2] = 34;
    objectEnemy->common.hitboxArray[1][3] = 35;

    // Middle Right
    objectEnemy->common.hitboxArray[2][0] = 49;
    objectEnemy->common.hitboxArray[2][1] = 9;
    objectEnemy->common.hitboxArray[2][2] = 53;
    objectEnemy->common.hitboxArray[2][3] = 55;

    // Far Left
    objectEnemy->common.hitboxArray[3][0] = 18;
    objectEnemy->common.hitboxArray[3][1] = 20;
    objectEnemy->common.hitboxArray[3][2] = 25;
    objectEnemy->common.hitboxArray[3][3] = 31;

    // Far Right
    objectEnemy->common.hitboxArray[4][0] = 54;
    objectEnemy->common.hitboxArray[4][1] = 20;
    objectEnemy->common.hitboxArray[4][2] = 61;
    objectEnemy->common.hitboxArray[4][3] = 31;

    // Far Left
    objectEnemy->common.hitboxArray[5][0] = 0;
    objectEnemy->common.hitboxArray[5][1] = 14;
    objectEnemy->common.hitboxArray[5][2] = 17;
    objectEnemy->common.hitboxArray[5][3] = 24;

    // Far Right
    objectEnemy->common.hitboxArray[6][0] = 62;
    objectEnemy->common.hitboxArray[6][1] = 14;
    objectEnemy->common.hitboxArray[6][2] = 79;
    objectEnemy->common.hitboxArray[6][3] = 24;

    objectEnemy->common.hitboxArrayNum = 7;
  }
}

void Game_Object_constructObjectProp(ObjectProp *objectProp, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable)
{
  Game_Object_constructObjectCommon((ObjectCommon *)objectProp, objectTemplate, pos_x, pos_y, movementSpeed, health, lives, isDamageable);
  objectProp->common.type = OBJECT_TYPE_PROP;
  objectProp->health = 1000;
  objectProp->isDamageable = false;
}

void Game_Object_constructObjectEffect(ObjectEffect *objectEffect, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable)
{
  Game_Object_constructObjectCommon((ObjectCommon *)objectEffect, objectTemplate, pos_x, pos_y, movementSpeed, health, lives, isDamageable);
  objectEffect->common.type = OBJECT_TYPE_EFFECT;
  objectEffect->pos_direction = 0;
}

void Game_Object_constructObjectProjectile(ObjectProjectile *objectProjectile, ObjectTemplate *objectTemplate, Object *parentObject, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable)
{
  Game_Object_constructObjectCommon((ObjectCommon *)objectProjectile, objectTemplate, pos_x, pos_y, movementSpeed, health, lives, isDamageable);
  objectProjectile->common.type = OBJECT_TYPE_PROJECTILE;
  objectProjectile->parentObject = parentObject;

  if (strcmp(objectTemplate->filename,"assets/bullet1.png") == 0)
  {
    objectProjectile->common.movementDirection = OBJECT_MOVEMENTDIRECTION_NORTH;
  }
  else if (strcmp(objectTemplate->filename,"assets/enemy1_bullet.png") == 0)
  {
    objectProjectile->common.movementDirection = OBJECT_MOVEMENTDIRECTION_SOUTH;
  }
  else if (strcmp(objectTemplate->filename,"assets/enemy2_bullet.png") == 0)
  {
    objectProjectile->common.movementDirection = OBJECT_MOVEMENTDIRECTION_SOUTH;
    objectProjectile->spawnTick = SDL_GetTicks();

    objectProjectile->hasLocked = false;
    objectProjectile->lockOnStart = 0;
    objectProjectile->lockOnDuration = 2000;
  }
}

void Game_Object_constructObjectHUD(ObjectHUD *objectHUD, ObjectTemplate *objectTemplate, float pos_x, float pos_y, float movementSpeed, int health, int lives, bool isDamageable)
{
  Game_Object_constructObjectCommon((ObjectCommon *)objectHUD, objectTemplate, pos_x, pos_y, 0, 0, 0, 0);
  objectHUD->common.type = OBJECT_TYPE_HUD;
}

void Game_Func_clampObjectInsideGame(Object *object)
{
  WidgetGame *game = object->common.widgetGame;

  if (object->common.pos_x < game->common.sdlRect.x)
  {
    object->common.pos_x = game->common.sdlRect.x;
  }
  if (object->common.pos_x + object->common.objectTemplate->texture_w > game->common.sdlRect.x + game->common.sdlRect.w)
  {
    object->common.pos_x = game->common.sdlRect.x + game->common.sdlRect.w - object->common.objectTemplate->texture_w;
  }
  if (object->common.type != OBJECT_TYPE_ENEMY)
  {
    if (object->common.pos_y < game->common.sdlRect.y)
    {
      object->common.pos_y = game->common.sdlRect.y;
    }
    if (object->common.pos_y + object->common.objectTemplate->texture_h > game->common.sdlRect.y + game->common.sdlRect.h - 70)
    {
      object->common.pos_y = game->common.sdlRect.y + game->common.sdlRect.h - object->common.objectTemplate->texture_h - 70;
    }
  }
}

int Game_Func_checkGameBorder(Object *object)
{
  WidgetGame *game = object->common.widgetGame;

  if (object != NULL
   && object->common.pos_x + object->common.objectTemplate->texture_w < game->common.sdlRect.x + game->common.sdlRect.w
   && object->common.pos_x > game->common.sdlRect.x
   && object->common.pos_y + object->common.objectTemplate->texture_h < game->common.sdlRect.y + game->common.sdlRect.h
   && object->common.pos_y > game->common.sdlRect.y)
  {
    return 1;
  }
  return 0;
}

bool Game_Func_checkObjectCollision(Object *object1, Object *object2)
{
  // generic error testing
  if (object1 == NULL && object2 == NULL)
  {
    printf("ERROR! Game_Func_checkObjectCollision(Object *object1, Object *object2) is screwed! One of them is null");
  }
  else
  {
    // hitboxarray vs. hitboxarray
    if (object1->common.useHitbox && object2->common.useHitbox)
    {
      // cycle through object1's hitboxes
      for (int i = 0; i < object1->common.hitboxArrayNum; i++)
      {
        // cycle through object2's hitboxes
        for (int k = 0; k < object2->common.hitboxArrayNum; k++)
        {
          if (object1->common.useCollision && object1->common.useCollision
           && object1->common.pos_x + object1->common.hitboxArray[i][0] < object2->common.pos_x + object2->common.hitboxArray[k][2]
           && object1->common.pos_x + object2->common.hitboxArray[i][2] > object2->common.pos_x + object1->common.hitboxArray[k][0]
           && object1->common.pos_y + object1->common.hitboxArray[i][1] < object2->common.pos_y + object2->common.hitboxArray[k][3]
           && object1->common.pos_y + object2->common.hitboxArray[i][3] > object2->common.pos_y + object1->common.hitboxArray[k][1])
          {
            return true;
          }
        }
      }
    }

    // rectangle vs. rectangle
    else if (!object1->common.useHitbox && !object2->common.useHitbox)
    {
      if (object1->common.pos_x < object2->common.pos_x + object2->common.objectTemplate->texture_w &&
          object1->common.pos_x + object1->common.objectTemplate->texture_w > object2->common.pos_x &&
          object1->common.pos_y < object2->common.pos_y + object2->common.objectTemplate->texture_h &&
          object1->common.pos_y + object1->common.objectTemplate->texture_h > object2->common.pos_y)
      {
        return true;
      }
    }

    // rectangle vs. hitboxarray
    else
    {
      Object *hitboxObject;
      Object *rectangleObject;

      // determine which object has a hitbox
      if (object1->common.useHitbox)
      {
        hitboxObject = object1;
        rectangleObject = object2;
      }
      else
      {
        hitboxObject = object2;
        rectangleObject = object1;
      }

      for (int i = 0; i < hitboxObject->common.hitboxArrayNum; i++)
      {
        if (hitboxObject->common.pos_x + hitboxObject->common.hitboxArray[i][0] < rectangleObject->common.pos_x + rectangleObject->common.objectTemplate->texture_w
         && hitboxObject->common.pos_x + hitboxObject->common.hitboxArray[i][2] > rectangleObject->common.pos_x
         && hitboxObject->common.pos_y + hitboxObject->common.hitboxArray[i][1] < rectangleObject->common.pos_y + rectangleObject->common.objectTemplate->texture_h
         && hitboxObject->common.pos_y + hitboxObject->common.hitboxArray[i][3] > rectangleObject->common.pos_y)
        {
          return true;
        }
      }
    }
  }
  return false;
}

void Game_Func_moveObjectAwayFromObject(Object *object1, Object *object2)
{
  float object1CenterX = (object1->common.pos_x) + object1->common.objectTemplate->texture_w / 2;
  float object1CenterY = (object1->common.pos_y) + object1->common.objectTemplate->texture_h / 2;
  float object2CenterX = (object2->common.pos_x) + object2->common.objectTemplate->texture_w / 2;
  float object2CenterY = (object2->common.pos_y) + object2->common.objectTemplate->texture_h / 2;

  float deltaX = object1CenterX - object2CenterX;
  float deltaY = object1CenterY - object2CenterY;



  object1->common.pos_x += deltaX * ((object1->common.movementSpeed * 0.05f) / (CFG_GCPS) * (float)object1->common.widgetGame->pixelsPerUnit_Width);
  object1->common.pos_y += deltaY * ((object1->common.movementSpeed * 0.05f) / (CFG_GCPS) * (float)object1->common.widgetGame->pixelsPerUnit_Height);
}

void Game_Func_fetchAndSetPlayerWeapons(WidgetGame *game)
{
  if (game->currentPlayer.common.widgetGame->pressedkey_space)
  {
    game->currentPlayer.weapon1_projectileType = OBJECT_PROJECTILETYPE_BULLET1;

    Sound_playSoundEffect(&playerShooting, 0, 0, 10, true);
  }
  else
  {
    game->currentPlayer.weapon1_projectileType = OBJECT_PROJECTILETYPE_NONE;
  }
}

void Game_Func_fetchAndSetPlayerMovement(WidgetGame *game)
{
  if (game->pressedkey_w)
  {
    if (game->pressedkey_a)
    {
     game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_NORTHWEST;
    }
    else if (game->pressedkey_d)
    {
     game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_NORTHEAST;
    }
    else
    {
     game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_NORTH;
    }
  }
  else if (game->pressedkey_s)
  {
    if (game->pressedkey_a)
    {
     game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_SOUTHWEST;
    }
    else if (game->pressedkey_d)
    {
     game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_SOUTHEAST;
    }
    else
    {
     game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_SOUTH;
    }
  }
  else if (game->pressedkey_d)
  {
    game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_EAST;
  }

  else if (game->pressedkey_a)
  {
    game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_WEST;
  }

  else
  {
    game->currentPlayer.common.movementDirection = OBJECT_MOVEMENTDIRECTION_NONE;
  }
}

void Game_Func_moveObject(Object *object)
{
  switch (object->common.movementDirection)
  {
    case OBJECT_MOVEMENTDIRECTION_NORTH:
    {
      object->common.pos_y = object->common.pos_y - (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Height);
      break;
    }
    case OBJECT_MOVEMENTDIRECTION_SOUTH:
    {
      object->common.pos_y = object->common.pos_y + (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Height);
      break;
    }
    case OBJECT_MOVEMENTDIRECTION_EAST:
    {
      object->common.pos_x = object->common.pos_x + (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Width);
      break;
    }

    case OBJECT_MOVEMENTDIRECTION_WEST:
    {
      object->common.pos_x = object->common.pos_x - (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Width);
      break;
    }

    case OBJECT_MOVEMENTDIRECTION_NORTHEAST:
    {
      object->common.pos_y = object->common.pos_y - (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Height);
      object->common.pos_x = object->common.pos_x + (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Width);
      break;
    }

    case OBJECT_MOVEMENTDIRECTION_NORTHWEST:
    {
      object->common.pos_y = object->common.pos_y - (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Height);
      object->common.pos_x = object->common.pos_x - (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Width);
      break;
    }

    case OBJECT_MOVEMENTDIRECTION_SOUTHEAST:
    {
      object->common.pos_y = object->common.pos_y + (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Height);
      object->common.pos_x = object->common.pos_x + (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Width);
      break;
    }

    case OBJECT_MOVEMENTDIRECTION_SOUTHWEST:
    {
      object->common.pos_y = object->common.pos_y + (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Height);
      object->common.pos_x = object->common.pos_x - (object->common.movementSpeed / (CFG_GCPS) * (float)object->common.widgetGame->pixelsPerUnit_Width);
      break;
    }

    default:
    {
      break;
    }
  }
}

void Game_Func_moveObjectTowardsPos(Object *object, float pos_x, float pos_y, float unitsPerSecond)
{
  float deltaX = pos_x - object->common.pos_x;
  float deltaY = pos_y - object->common.pos_y;
  float magnitude = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

  // store deltaX and deltaY
  if (object->type == OBJECT_TYPE_PROJECTILE)
  {
    object->projectile.deltaX = deltaX;
    object->projectile.deltaY = deltaY;
  }

  object->common.pos_x += deltaX / magnitude * (unitsPerSecond / CFG_GCPS * object->common.widgetGame->pixelsPerUnit_Width);
  object->common.pos_y += deltaY / magnitude * (unitsPerSecond / CFG_GCPS * object->common.widgetGame->pixelsPerUnit_Height);
}

void Game_Func_moveObjectAlongDelta(Object *object, float unitsPerSecond, float deltaX, float deltaY)
{
  float magnitude = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

  object->common.pos_x += deltaX / magnitude * (unitsPerSecond / CFG_GCPS * object->common.widgetGame->pixelsPerUnit_Width);
  object->common.pos_y += deltaY / magnitude * (unitsPerSecond / CFG_GCPS * object->common.widgetGame->pixelsPerUnit_Height);
}

void Game_func_createPlayerExplosion(Object *object, float pos_x, float pos_y, float startRadius, float unitsPerSecond, int thickness)
{
  if (object == NULL || thickness < 1 || unitsPerSecond < 1 || startRadius < 0)
  {
    printf("Game_func_createPlayerExplosion screwed up!");
    return;
  }

  PlayerExplosion *playerExplosion = &object->common.widgetGame->playerExplosion;
  playerExplosion->isActive = true;
  playerExplosion->pos_x = pos_x;
  playerExplosion->pos_y = pos_y;
  playerExplosion->currentRadius = startRadius + thickness;
  playerExplosion->unitsPerSecond = unitsPerSecond;
  playerExplosion->thickness = thickness;
  playerExplosion->object = object;
}


void Game_Func_updatePlayerExplosion(PlayerExplosion *playerExplosion)
{
  playerExplosion->currentRadius++;
  playerExplosion->a -= 0.5;

  // Check of explosion wave is dead
  if (playerExplosion->currentRadius == 1000)
  {
    playerExplosion->isActive = false;
  }
}

void Game_initStarfield(Starfield *starfield, WidgetGame *widgetGame, int totalStars, float smallStarPercent, float largeStarPercent, int smallStarMinVelocity, int smallStarMaxVelocity, int largeStarMinVelocity, int largeStarMaxVelocity)
{
  // set struct
  starfield->isStarfieldActive = true;
  starfield->totalStars = totalStars;

  starfield->smallStarPercent = smallStarPercent;
  starfield->smallStarMinVelocity = smallStarMinVelocity;
  starfield->smallStarMaxVelocity = smallStarMaxVelocity;

  starfield->largeStarPercent = largeStarPercent;
  starfield->largeStarMinVelocity = largeStarMinVelocity;
  starfield->largeStarMaxVelocity = largeStarMaxVelocity;

  starfield->widgetGame = widgetGame;

  // calculate how many small and large stars needed
  if (starfield->smallStarPercent + starfield->largeStarPercent != 1.0f)
  {
    printf("starfield density wasn't set properly! defaulting to 0.5 density for both types of stars");
    starfield->largeStarPercent = 0.5f;
    starfield->smallStarPercent = 0.5f;
  }

  // set the starting properties for small and large stars
  Star *star;
  starfield->starArrayNum = 0;
  for (int i = 0; i < starfield->totalStars; i++)
  {
    star = &(starfield->starArray[starfield->starArrayNum]);
    starfield->starArrayNum++;

    // generate position anywhere random between 0 and max width and height

    star->pos_x = (rand() % starfield->widgetGame->common.sdlRect.w) + starfield->widgetGame->common.sdlRect.x;
    star->pos_y = (rand() % starfield->widgetGame->common.sdlRect.h) + starfield->widgetGame->common.sdlRect.y;

    // determine small and large type and velocity based off iteration value
    if (i < starfield->totalStars * starfield->smallStarPercent)
    {
      star->type = STAR_TYPE_SMALL;
      star->velocity = ((rand() % (starfield->smallStarMaxVelocity - starfield->smallStarMinVelocity)) + starfield->smallStarMinVelocity) / 2;
    }
    else
    {
      star->type = STAR_TYPE_LARGE;
      star->velocity = ((rand() % (starfield->largeStarMaxVelocity - starfield->largeStarMinVelocity)) + starfield->largeStarMinVelocity) / 2;
    }
  }
}

void Game_updateStarfield(Starfield *starfield)
{
  Star *star;
  WidgetGame *widgetGame = starfield->widgetGame;

  // iterate through the stars in starfield array
  for (int i = 0; i < starfield->starArrayNum; i++)
  {
    star = &(starfield->starArray[i]);

    // move the star downwards
    star->pos_y += (float)star->velocity*.1; //((float)star->velocity / (CFG_GCPS) * (float)widgetGame->pixelsPerUnit_Height);

    // check if the star is visible or is off screen
    if (star->pos_y > widgetGame->common.sdlRect.y + widgetGame->common.sdlRect.h)
    {
      // respawn the star at y = 0 but a random x value
      star->pos_x = (rand() % starfield->widgetGame->common.sdlRect.w) + starfield->widgetGame->common.sdlRect.x;
      star->pos_y = widgetGame->common.sdlRect.y;
    }
  }
}

void Game_initObjectExplosion(Object *explodingObject, Explosion *explosion, int particleDensity, int particleSpeed, int alphaFade)
{
  // General parameter tests
  if ((explodingObject->type != OBJECT_TYPE_ENEMY && explodingObject->type != OBJECT_TYPE_PLAYER)
   || particleDensity <= 0 || particleSpeed < 0 || alphaFade <= 0)
  {
    return;
  }
  else
  {
    // find center points x and y for explosion
    explosion->pos_x = explodingObject->common.pos_x + explodingObject->common.objectTemplate->texture_w / 2;
    explosion->pos_y = explodingObject->common.pos_y + explodingObject->common.objectTemplate->texture_h / 2;
    explosion->isActive = true;
    explosion->explosionParticleArrayNum = 0;
    explosion->object = explodingObject;

    // assign image surface
    if (strcmp(explosion->object->common.objectTemplate->filename,"assets/enemy1.png") == 0)
    {
      explosion->imageSurface = enemy1_imageSurface;
    }
    else if (strcmp(explosion->object->common.objectTemplate->filename,"assets/enemy2.png") == 0)
    {
      explosion->imageSurface = enemy2_imageSurface;
    }
    else if (strcmp(explosion->object->common.objectTemplate->filename,"assets/player.png") == 0)
    {
      explosion->imageSurface = player_imageSurface;
    }

    // check errors
    if (!explosion->imageSurface)
    {
      fprintf(stdout, "objectImage = IMG_Load(object->common.objectTemplate->filename screwed up: %s", IMG_GetError());
      fflush(stdout);
      return;
    }

    Uint8 *buffer = explosion->imageSurface->pixels;

    int currentPixelAlpha;
    // search through each pixel on the object's image, check for it's alpha value and spawn them if alpha != 0
    for (int i = 0; i < explosion->imageSurface->h; i++) // y down
    {
      for (int j = 0; j < explosion->imageSurface->w; j++) // x across
      {
        // check if alpha is minimal
        currentPixelAlpha = buffer[(i * explosion->imageSurface->pitch) + (j*4) + 3];
        // Alpha Value check *BGRA*
        if (currentPixelAlpha >= 10)
        {
          ExplosionParticle *explosionParticle = &(explosion->explosionParticleArray[explosion->explosionParticleArrayNum]); // create particle

          // particle position and delta values
          explosionParticle->pos_x = (float)j + explodingObject->common.pos_x;
          explosionParticle->pos_y = (float)i + explodingObject->common.pos_y;
          explosionParticle->deltaX = explosionParticle->pos_x - explosion->pos_x;
          explosionParticle->deltaY = explosionParticle->pos_y - explosion->pos_y;

          // reference the particle to the parent explosion
          explosionParticle->explosion = explosion;

          // generate random properties between parameter variables
          explosionParticle->alphaFade = rand() % (alphaFade - 1 + 1) + 1; // generate between 1 and *alphaFade
          explosionParticle->particleSpeed = rand() % (particleSpeed - 1 + 1) + 1; // generate between 1 and *particleSpeed

          // set a fire-looking colour
          explosionParticle->r = 255;
          explosionParticle->g = currentPixelAlpha - 2;
          explosionParticle->b = 0;
          explosionParticle->a = 255;

          // insert particle into explosionParticleArray
          explosion->explosionParticleArrayNum++; // increment elem after storage
        }
      }
    }
  }
}

void Game_updateObjectExplosion(Explosion *explosion)
{
  // parameter testing
  if (explosion == NULL || !explosion->isActive)
  {
    return;
  }
  else
  {
    WidgetGame *widgetGame = explosion->object->common.widgetGame;
    bool hasExplosionFinished = true; // cheap test to check status of explosion

    ExplosionParticle *explosionParticle;
    for (int i = 0; i < explosion->explosionParticleArrayNum; i++)
    {
      if (explosion->explosionParticleArray[i].a > 0)
      {
        explosionParticle = &explosion->explosionParticleArray[i];
        hasExplosionFinished = false;

        // change position x and y
        explosionParticle->pos_x += explosionParticle->deltaX * (explosionParticle->particleSpeed* 0.4f / CFG_GCPS * widgetGame->pixelsPerUnit_Width);
        explosionParticle->pos_y += explosionParticle->deltaY * (explosionParticle->particleSpeed* 0.4f / CFG_GCPS * widgetGame->pixelsPerUnit_Height);

        // check if it's within widget border
        if (explosionParticle->pos_x > widgetGame->common.sdlRect.x
         && explosionParticle->pos_x < widgetGame->common.sdlRect.x + widgetGame->common.sdlRect.w
         && explosionParticle->pos_y > widgetGame->common.sdlRect.y
         && explosionParticle->pos_y < widgetGame->common.sdlRect.y + widgetGame->common.sdlRect.h)
        {
          // optimisation variables of x and y
          explosionParticle->buffer_x = ((int)explosionParticle->pos_x) * 4;
          explosionParticle->buffer_y = ((int)explosionParticle->pos_y) * widgetGame->surface->pitch;

          // fade the alpha level
          explosionParticle->a = explosionParticle->a - explosionParticle->alphaFade;
        }
        else
        {
          explosionParticle->a = 0;
        }
      }
    }
    if (hasExplosionFinished)
    {
      explosion->isActive = false; // disable explosion entirely
    }
  }
}

void Game_Func_mainLoop(WidgetGame *widgetGame)
{
  // Starfield
  if (widgetGame->starfield.isStarfieldActive)
  {
    Game_updateStarfield(&widgetGame->starfield);
  }
  else
  {
    Game_initStarfield(&widgetGame->starfield, widgetGame, 100, 0.8, 0.2, 2, 3, 4, 5);
  }

  // PLAYER
  if (widgetGame->currentPlayer.lives > 0 && SDL_GetTicks() > widgetGame->currentPlayer.deathTickStart + widgetGame->currentPlayer.deathTickDuration)
  {
    ObjectPlayer *objectPlayer = &widgetGame->currentPlayer;

    if (objectPlayer->isDead)
    {
      objectPlayer->isDead = false;
      objectPlayer->health = 100;
      objectPlayer->common.isVisible = true;
      objectPlayer->common.useCollision = true;
    }


    // Grab info from current key pressing data
    Game_Func_fetchAndSetPlayerMovement(widgetGame);
    Game_Func_fetchAndSetPlayerWeapons(widgetGame);

    // Check if the player can move
    if (objectPlayer->common.movementDirection != OBJECT_MOVEMENTDIRECTION_NONE && objectPlayer->isControllable)
    {
      // Check if player is outside the border
      Game_Func_moveObject((Object *)objectPlayer);
      Game_Func_clampObjectInsideGame((Object *)objectPlayer);
    }

    // Check if player's health is gone
    if (objectPlayer->health <= 0)
    {
      Explosion *enemyExplosion = WidgetGame_findUnusedExplosionInArray(widgetGame);
      if (enemyExplosion)
      {
        Game_initObjectExplosion((Object *)objectPlayer, enemyExplosion, objectPlayer->common.objectTemplate->texture_w * objectPlayer->common.objectTemplate->texture_h, 8, 7);

        float pos_x = objectPlayer->common.pos_x + (float)(objectPlayer->common.objectTemplate->texture_w / 2);
        float pos_y = objectPlayer->common.pos_y + (float)(objectPlayer->common.objectTemplate->texture_h / 2);

        Game_func_createPlayerExplosion((Object *)objectPlayer, pos_x, pos_y, 1, 10, 5);
      }
      objectPlayer->lives -= 1;

      int getTicks = SDL_GetTicks();
      objectPlayer->deathTickStart = getTicks;
      objectPlayer->deathTickDuration = 1800;
      objectPlayer->common.isVisible = false;
      objectPlayer->common.useCollision = false;
      objectPlayer->isDead = true;
    }

    // Spawning a bullet
    if  (objectPlayer->weapon1_projectileType == OBJECT_PROJECTILETYPE_BULLET1)
    {
      // Check if ready to fire a bullet based on fire rate
      if (SDL_GetTicks() > objectPlayer->weapon1_ticksWhenFired + (1000 / objectPlayer->weapon1_fireRate))
      {
        objectPlayer->weapon1_ticksWhenFired = SDL_GetTicks();

        ObjectProjectile *weapon1_bullet1 = WidgetGame_findUnusedPlayerProjectileInArray(widgetGame);
        if (weapon1_bullet1 != NULL)
        {
          Game_Object_constructObjectProjectile(weapon1_bullet1, &bullet1, (Object *)objectPlayer, (objectPlayer->common.pos_x + objectPlayer->weapon1_1_x), (objectPlayer->common.pos_y + objectPlayer->weapon1_1_y), 500, 0, 0, true);
        }

        ObjectProjectile *weapon1_bullet2 = WidgetGame_findUnusedPlayerProjectileInArray(widgetGame);
        if (weapon1_bullet2 != NULL)
        {
          Game_Object_constructObjectProjectile(weapon1_bullet2, &bullet1, (Object *)objectPlayer, (objectPlayer->common.pos_x + objectPlayer->weapon1_2_x), (objectPlayer->common.pos_y + objectPlayer->weapon1_2_y), 500, 0, 0, true);
        }

        weapon1_bullet1->damage = objectPlayer->weapon1_damage;
        weapon1_bullet2->damage = objectPlayer->weapon1_damage;
      }
    }
    fflush(stdout);
  }

  // PLAYER BULLETS
  for (int i = 0; i < MAX_PLAYERPROJECTILE_OBJECTS; i++)
  {
    if (widgetGame->playerProjectileArray[i].common.isVisible)
    {
      ObjectProjectile *objectProj = &(widgetGame->playerProjectileArray[i]); // Ease of Use

      // Handling Player bullets
      if (strcmp(objectProj->common.objectTemplate->filename,"assets/bullet1.png") == 0)
      {
        Game_Func_moveObject((Object *)objectProj);

        // Check if hit Enemy
        for (int i = 0; i < MAX_ENEMY_OBJECTS; i++)
        {
          if (widgetGame->enemyArray[i].common.isVisible)
          {
            if (Game_Func_checkObjectCollision((Object *)&widgetGame->enemyArray[i], (Object *)objectProj))
            {
              objectProj->common.isVisible = false;
              widgetGame->enemyArray[i].health -= widgetGame->currentPlayer.weapon1_damage;
              if (widgetGame->enemyArray[i].health <= 0)
              {
                widgetGame->currentPlayer.stats_points += 5;
              }
              else
              {
                widgetGame->currentPlayer.stats_kills += 1;
              }
            }
          }
        }

        // Check if touched border
        if (!Game_Func_checkGameBorder((Object *)objectProj))
        {
          widgetGame->playerProjectileArray[i].common.isVisible = false;
        }
      }
    }
  }

  // ENEMIES
  for (int i = 0; i < MAX_ENEMY_OBJECTS; i++)
  {
    ObjectEnemy *objectEnemy = &(widgetGame->enemyArray[i]); // Ease of Use
    if (objectEnemy->common.isVisible)
    {
      if (objectEnemy->health > 0)
      {
        Game_Func_moveObject((Object *)objectEnemy);
        Game_Func_clampObjectInsideGame((Object *)objectEnemy);

        // Check Collision and start
        if (Game_Func_checkObjectCollision((Object *)&widgetGame->currentPlayer, (Object *)objectEnemy) && collisionCurrentTick == 0 && collisionObject == NULL)
        {
          widgetGame->currentPlayer.health -= 10;
          collisionCurrentTick = SDL_GetTicks();
          collisionStartTick = SDL_GetTicks();
          collisionObject = (Object *)objectEnemy;

          Sound_playSoundEffect(&playerOuttaMyWay, 0, 0, 20, true);
        }

        // Update Object Collision Path
        if (collisionObject == (Object *)objectEnemy)
        {
          if (collisionCurrentTick != 0 && collisionCurrentTick < collisionStartTick + collisionTotalTicks)
          {
            collisionCurrentTick = SDL_GetTicks();
            objectEnemy->common.widgetGame->currentPlayer.isControllable = false;
            Game_Func_moveObjectAwayFromObject((Object *)objectEnemy, (Object *)&widgetGame->currentPlayer);
            //Game_Func_moveObjectAwayFromObject((Object *)&widgetGame->currentPlayer, (Object *)objectEnemy);
          }
          else
          {
            objectEnemy->common.widgetGame->currentPlayer.isControllable = true;
            collisionCurrentTick = 0;
            collisionObject = NULL;
          }
        }

        // Check if over the border
        if (objectEnemy->common.pos_y > widgetGame->common.sdlRect.y + widgetGame->common.sdlRect.h + 50)
        {
          objectEnemy->health = 0;
          objectEnemy->common.isVisible = false;
        }

        // Spawning a bullet (Check if ready to fire a bullet based on fire rate)
        if (SDL_GetTicks() > objectEnemy->weapon1_ticksWhenFired + objectEnemy->weapon1_fireRate)
        {
          objectEnemy->weapon1_ticksWhenFired = SDL_GetTicks();
          ObjectProjectile *weapon1_bullet1 = WidgetGame_findUnusedEnemyProjectileInArray(widgetGame);

          if (weapon1_bullet1 != NULL && objectEnemy->enemyType == ENEMY_TYPE_1)
          {
            Game_Object_constructObjectProjectile(weapon1_bullet1, &enemy1_bullet, (Object *)objectEnemy, (objectEnemy->common.pos_x + objectEnemy->weapon1_x), (objectEnemy->common.pos_y + objectEnemy->weapon1_y), objectEnemy->weapon1_speed, 0, 0, true);
          }
          else if (weapon1_bullet1 != NULL && objectEnemy->enemyType == ENEMY_TYPE_2)
          {
            Game_Object_constructObjectProjectile(weapon1_bullet1, &enemy2_bullet, (Object *)objectEnemy, (objectEnemy->common.pos_x + objectEnemy->weapon1_x), (objectEnemy->common.pos_y + objectEnemy->weapon1_y), objectEnemy->weapon1_speed, 0, 0, true);
          }
          if (weapon1_bullet1)
          {
            weapon1_bullet1->damage = objectEnemy->weapon1_damage;
          }
        }
      }
      else // If health is 0 or less
      {
        Sound_playSoundEffect(&enemyBoom, 0, 0, 20, true);
        Explosion *enemyExplosion = WidgetGame_findUnusedExplosionInArray(widgetGame);
        if (enemyExplosion)
        {
          Game_initObjectExplosion((Object *)objectEnemy, enemyExplosion, objectEnemy->common.objectTemplate->texture_w * objectEnemy->common.objectTemplate->texture_h, 8, 7);
        }
        objectEnemy->common.isVisible = false;
      }
    }
  }

  // ENEMY BULLETS
  for (int i = 0; i < MAX_ENEMYPROJECTILE_OBJECTS; i++)
  {
    if (widgetGame->enemyProjectileArray[i].common.isVisible && widgetGame->currentPlayer.common.isVisible)
    {
      ObjectProjectile *objectProj = &(widgetGame->enemyProjectileArray[i]); // Ease of Use

      // Move Bullet
      if (objectProj->parentObject->type == OBJECT_TYPE_ENEMY && objectProj->parentObject->enemy.enemyType == ENEMY_TYPE_1)
      {
        Game_Func_moveObject((Object *)objectProj);
      }
      else if (objectProj->parentObject->type == OBJECT_TYPE_ENEMY && objectProj->parentObject->enemy.enemyType == ENEMY_TYPE_2)
      {
        if (objectProj->parentObject->common.isVisible
         && SDL_GetTicks() < objectProj->spawnTick + objectProj->lockOnDuration + objectProj->lockOnStart && SDL_GetTicks() > objectProj->spawnTick + objectProj->lockOnStart)
        {
          // while heatseaking missiles are on.
          objectProj->hasLocked = true;

          float pos_x = (widgetGame->currentPlayer.common.pos_x) + widgetGame->currentPlayer.common.objectTemplate->texture_w / 2;
          float pos_y = (widgetGame->currentPlayer.common.pos_y) + widgetGame->currentPlayer.common.objectTemplate->texture_h / 2;

          Game_Func_moveObjectTowardsPos((Object *)objectProj, pos_x, pos_y, objectProj->parentObject->enemy.weapon1_speed);
        }
        else if (objectProj->hasLocked)
        {
          Game_Func_moveObjectAlongDelta((Object *)objectProj, objectProj->parentObject->enemy.weapon1_speed, objectProj->deltaX, objectProj->deltaY);
        }
        else
        {
          Game_Func_moveObject((Object *)objectProj);
        }
      }

      // Hit Detection
      if (Game_Func_checkObjectCollision((Object *)&widgetGame->currentPlayer, (Object *)objectProj))
      {
        objectProj->common.isVisible = false;
        widgetGame->currentPlayer.health -= objectProj->damage;
        Sound_playSoundEffect(&playerOuch, 0, 0, 10, true);
      }

      // Border Detection
      if (!Game_Func_checkGameBorder((Object *)objectProj))
      {
        objectProj->common.isVisible = false;
      }
    }
  }

  // Explosions
  for (int i = 0; i < MAX_EXPLOSIONS; i++)
  {
    if (widgetGame->explosionArray[i].isActive)
    {
      Game_updateObjectExplosion(&widgetGame->explosionArray[i]);
    }

    if (widgetGame->playerExplosion.isActive)
    {
      Game_Func_updatePlayerExplosion(&widgetGame->playerExplosion);
    }
  }

  // Enemy Generator

  // ENEMY 1
  if (SDL_GetTicks() > widgetGame->enemy1_spawnerTicks + (widgetGame->enemy1_spawnerDelay))
  {
    widgetGame->enemy1_spawnerTicks = SDL_GetTicks();

    ObjectEnemy *spawnedEnemy = WidgetGame_findUnusedEnemyInArray(widgetGame);
    if (spawnedEnemy)
    {
      float x_pos = rand() % (widgetGame->common.sdlRect.x + widgetGame->common.sdlRect.w);
      Game_Object_constructObjectEnemy(spawnedEnemy, &enemy1,
                                 x_pos,
                                (widgetGame->common.sdlRect.y),
                                 10,
                                 0,
                                 0,
                                 true);
      spawnedEnemy->common.pos_y -= spawnedEnemy->common.objectTemplate->texture_h;
      spawnedEnemy->common.movementDirection = OBJECT_MOVEMENTDIRECTION_SOUTH;
      //spawnedEnemy->weapon1_ticksWhenFired = rand() % (1000 / (int)spawnedEnemy->weapon1_fireRate);
    }
    if (widgetGame->enemy1_spawnerDelay > 2000)
    {
      widgetGame->enemy1_spawnerDelay -= 50;
    }
  }

  // ENEMY 2
  if (SDL_GetTicks() > widgetGame->enemy2_spawnerTicks + (widgetGame->enemy2_spawnerDelay))
  {
    widgetGame->enemy2_spawnerTicks = SDL_GetTicks();

    ObjectEnemy *spawnedEnemy = WidgetGame_findUnusedEnemyInArray(widgetGame);
    if (spawnedEnemy)
    {
      float x_pos = rand() % (widgetGame->common.sdlRect.x + widgetGame->common.sdlRect.w);
      Game_Object_constructObjectEnemy(spawnedEnemy, &enemy2,
                                 x_pos,
                                 widgetGame->common.sdlRect.y,
                                 15,
                                 0,
                                 0,
                                 true);
      spawnedEnemy->common.pos_y -= spawnedEnemy->common.objectTemplate->texture_h;
      spawnedEnemy->common.movementDirection = OBJECT_MOVEMENTDIRECTION_SOUTH;
      //spawnedEnemy->weapon1_ticksWhenFired = rand() % (1000 / (int)spawnedEnemy->weapon1_fireRate);
    }

    if (widgetGame->enemy2_spawnerDelay > 1800)
    {
      widgetGame->enemy2_spawnerDelay -= 30;
    }
  }
}

void Game_Starfield_render(Starfield *starfield)
{
  Star *star;

  // iterate through starfield array and blit to render
  for (int i = 0; i < starfield->starArrayNum; i++)
  {
    star = &starfield->starArray[i];
    SDL_SetRenderDrawColor(starfield->widgetGame->common.window->sdlRenderer, 255, 255, 255, 255);

    // draw small star (singular dot)
    if (star->type == STAR_TYPE_SMALL)
    {
      SDL_RenderDrawPoint(starfield->widgetGame->common.window->sdlRenderer, (int)star->pos_x, (int)star->pos_y);
    }

    // draw large star (dot with 1 pixel extra on each side)
    else if (star->type == STAR_TYPE_LARGE)
    {
      SDL_RenderDrawPoint(starfield->widgetGame->common.window->sdlRenderer, (int)star->pos_x, (int)star->pos_y);
      SDL_RenderDrawPoint(starfield->widgetGame->common.window->sdlRenderer, (int)star->pos_x + 1, (int)star->pos_y);
      SDL_RenderDrawPoint(starfield->widgetGame->common.window->sdlRenderer, (int)star->pos_x - 1, (int)star->pos_y);
      SDL_RenderDrawPoint(starfield->widgetGame->common.window->sdlRenderer, (int)star->pos_x, (int)star->pos_y + 1);
      SDL_RenderDrawPoint(starfield->widgetGame->common.window->sdlRenderer, (int)star->pos_x, (int)star->pos_y - 1);
    }
  }
}

void Game_playerExplosion_render(PlayerExplosion *playerExplosion)
{
  if (playerExplosion == NULL || playerExplosion->isActive == true)
  {
    printf("Game_playerExplosion_render screwed up!");
    return;
  }

  else if (playerExplosion->currentRadius < 1000)
  {
    for (int i = 0; i < playerExplosion->thickness; i++)
    {
      dorris_drawCircle(playerExplosion->object->common.widgetGame->common.window->sdlRenderer, playerExplosion->pos_x, playerExplosion->pos_y, playerExplosion->currentRadius - i, 255, 255, 0, (Uint8)playerExplosion->a);
    }
  }
}

void Game_Explosion_render(Explosion *explosion)
{
  // short references
  WidgetGame *widgetGame = explosion->object->common.widgetGame;
  ExplosionParticle *explosionParticle;

  // temporarily change blend mode to allow alpha blend
  SDL_SetRenderDrawBlendMode(widgetGame->common.window->sdlRenderer, SDL_BLENDMODE_BLEND);

  int bufferMax = (widgetGame->common.sdlRect.w * 4) + (widgetGame->common.sdlRect.h * widgetGame->surface->pitch);
  for (int i = 0; i < explosion->explosionParticleArrayNum; i++)
  {
    explosionParticle = &explosion->explosionParticleArray[i];
    if (explosionParticle->a > 0 && bufferMax > explosionParticle->buffer_x + explosionParticle->buffer_y)
    {
      SDL_SetRenderDrawColor(widgetGame->common.window->sdlRenderer, (Uint8)explosionParticle->r, (Uint8)explosionParticle->g, (Uint8)explosionParticle->b, (Uint8)explosionParticle->a);
      SDL_RenderDrawPoint(widgetGame->common.window->sdlRenderer, (int)explosionParticle->pos_x, (int)explosionParticle->pos_y);

//      buffer[explosionParticle->buffer_x + explosionParticle->buffer_y] = (Uint8)explosionParticle->b; // Blue
//      buffer[explosionParticle->buffer_x + explosionParticle->buffer_y + 1] = (Uint8)explosionParticle->g; // Green
//      buffer[explosionParticle->buffer_x + explosionParticle->buffer_y + 2] = (Uint8)explosionParticle->r; // Red
//      buffer[explosionParticle->buffer_x + explosionParticle->buffer_y + 3] = (Uint8)explosionParticle->a; // Alpha
    }

    if (i == 4)
    {
      fprintf(stdout, "%d\n", (Uint8)explosionParticle->a);
      fflush(stdout);
    }
  }

  SDL_SetRenderDrawBlendMode(widgetGame->common.window->sdlRenderer, SDL_BLENDMODE_NONE);
}

void Game_Object_render(Object *object)
{
  if (object->common.isVisible)
  {
    SDL_Renderer *renderer = object->common.widgetGame->common.window->sdlRenderer;

    SDL_Rect objectRect;
    SDL_Rect gameRect;

    objectRect.x = (int)object->common.pos_x;
    objectRect.y = (int)object->common.pos_y;
    objectRect.w = object->common.objectTemplate->texture_w;
    objectRect.h = object->common.objectTemplate->texture_h;

    gameRect.x = object->common.widgetGame->common.sdlRect.x;
    gameRect.y = object->common.widgetGame->common.sdlRect.y;
    gameRect.w = object->common.widgetGame->common.sdlRect.w;
    gameRect.h = object->common.widgetGame->common.sdlRect.h;

    SDL_RenderSetClipRect(renderer, &gameRect); // render only in gameRect
    SDL_RenderCopy(renderer, object->common.objectTemplate->sdlTexture, NULL, &objectRect); // Send texture in rect to renderer
    SDL_RenderSetClipRect(renderer, NULL); // Resetting renderer to render everything on screen

    if (object->type == OBJECT_TYPE_HUD)
    {
      ObjectPlayer *currentPlayer = &object->common.widgetGame->currentPlayer;
      if (strcmp(object->common.objectTemplate->filename,"assets/hud_health.png") == 0)
      {
        // Render Health Bar
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // the rect color (solid red)
        SDL_Rect filledHealth; // the rectangle
        filledHealth.x = objectRect.x + 5;
        filledHealth.y = objectRect.y + 5;
        filledHealth.w = (ceil((float)currentPlayer->health / (float)100 * (float)objectRect.w)) - 10;
        filledHealth.h = objectRect.h - 10;
        SDL_RenderFillRect(renderer, &filledHealth);

        // Render Text Over Health Bar
        SDL_Color rgba;
        rgba.r = 255;
        rgba.g = 120;
        rgba.b = 120;
        rgba.a = 255;

        SDL_Surface *fontSurface = TTF_RenderText_Blended(mainfont->sdlFont, "Health", rgba);
        SDL_Surface *windowSurface = SDL_GetWindowSurface(object->common.widgetGame->common.window->sdlWindow);
        SDL_SetSurfaceBlendMode(windowSurface, SDL_BLENDMODE_BLEND);

        // Text Position
        SDL_Rect textRect = objectRect;
        textRect.x = textRect.x + ((textRect.w - fontSurface->w) / 2);
        textRect.y = textRect.y + ((textRect.h - fontSurface->h) / 2);

        SDL_BlitSurface(fontSurface, NULL, windowSurface, &textRect);
        SDL_FreeSurface(fontSurface);
      }

      // Render Stat Bar
      else if (strcmp(object->common.objectTemplate->filename,"assets/hud_stats.png") == 0)
      {
        // Render Text Over Stat Bar
        SDL_Color rgba;
        rgba.r = 255;
        rgba.g = 255;
        rgba.b = 255;
        rgba.a = 255;

        char charText[64];
        //sprintf(charText, "%d", currentPlayer->stats_points);
        SDL_Surface *fontSurface = TTF_RenderText_Blended(mainfont->sdlFont, charText, rgba);
        SDL_Surface *windowSurface = SDL_GetWindowSurface(object->common.widgetGame->common.window->sdlWindow);
        SDL_SetSurfaceBlendMode(windowSurface, SDL_BLENDMODE_BLEND);

        // Text Position
        SDL_Rect textRect = objectRect;
        textRect.x = textRect.x + ((textRect.w - fontSurface->w) / 2);
        textRect.y = textRect.y + ((textRect.h - fontSurface->h) / 2);

        SDL_BlitSurface(fontSurface, NULL, windowSurface, &textRect);
        SDL_FreeSurface(fontSurface);
      }
    }
  }
}
