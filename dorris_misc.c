#include "includes.h"

// Miscellanious Functions

void dorris_drawCircle(SDL_Renderer *renderer, float pos_x, float pos_y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  SDL_SetRenderDrawColor(renderer, r, g, b, a);

  float x;
  float y;
  float sin45 = 0.7071067;

  int total = (int)(sin45 * radius);
  for (x = 0; x <= total; x++)
  {
    y = sqrt(radius*radius - x*x);

    SDL_RenderDrawPoint(renderer, (int)(x + pos_x), (int)(y + pos_y));
    SDL_RenderDrawPoint(renderer, (int)(-x + pos_x), (int)(y + pos_y));
    SDL_RenderDrawPoint(renderer, (int)(x + pos_x), (int)(-y + pos_y));
    SDL_RenderDrawPoint(renderer, (int)(-x + pos_x), (int)(-y + pos_y));
    SDL_RenderDrawPoint(renderer, (int)(y + pos_x), (int)(x + pos_y));
    SDL_RenderDrawPoint(renderer, (int)(-y + pos_x), (int)(x + pos_y));
    SDL_RenderDrawPoint(renderer, (int)(y + pos_x), (int)(-x + pos_y));
    SDL_RenderDrawPoint(renderer, (int)(-y + pos_x), (int)(-x + pos_y));
  }
}

void dads_drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, SDL_Surface *windowSurface)
{
  int power = 0; // this will hold what power of 2 the pitch is
  int pitch = windowSurface->pitch;
  // exit if pitch is zero
  if (pitch == 0)
  {
    printf("destination has zero width");
    return;
  }
  // keep shifting right while there is a zero in the least significant digit of pitch
  while ((pitch & 1) == 0)
  {
    pitch >>= 1;  // perform the shift
    power++; // this is counting how many times we had to shift
  }
  if (pitch != 1)
  {
    printf("destination not a power of 2");
    return;
  }
  int bitmask = 0xffffffffffffffff << power;
  int eighteenMinusPower = 18 - power; // we are going to use 18 fixed bits of fraction

  uint8_t *buffer = windowSurface->pixels;

  uint32_t color = a << 24 | r << 16 | g << 8 | b;

  int deltaX = x2 - x1;
  int deltaY = y2 - y1;
  int absDeltaX = abs(deltaX);
  int absDeltaY = abs(deltaY);

  if (absDeltaY == 0)
  {
    int xInc;
    if (deltaX < 0)
    {
      xInc = -1;
    }
    else
    {
      xInc = 1;
    }
    (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
    while (x1 != x2)
    {
      x1 += xInc;
      (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
    }
  }
  else if (absDeltaX > absDeltaY)
  {
    // moving horizontally
    int xInc;
    if (deltaX < 0)
    {
      xInc = -1;
    }
    else
    {
      xInc = 1;
    }
    //                                                deltaY                 00000000000000000000000000000000000000000000000000000000000001010010101.0000000000000000
    //                                                delatX                 00000000000000000000000000000000000000000000000000000001001001010010101.0000000000000000
    int yInc = (deltaY << 18) / absDeltaX;   // fixed point integer 14 bits with 50 bits of fraction   00000000000000.000000000000000000000000000000000000000000000000000000000
    int yShifted = y1 << 18;
    (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
    while (x1 != x2)
    {
      x1 += xInc;
      yShifted += yInc;
      (*(uint32_t *)(&(buffer[((yShifted >> eighteenMinusPower) & bitmask) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
    }
  }
  else
  {
    // moving vertically
    int yInc;
    if (deltaY < 0)
    {
      yInc = -1;
    }
    else
    {
      yInc = 1;
    }
    //                                                deltaY                 00000000000000000000000000000000000000000000000000000000000001010010101.0000000000000000
    //                                                delatX                 00000000000000000000000000000000000000000000000000000001001001010010101.0000000000000000
    int xInc = (deltaX << 18) / absDeltaY;   // fixed point integer 14 bits with 50 bits of fraction   00000000000000.000000000000000000000000000000000000000000000000000000000
    int xShifted = x1 << 18;
    (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
    while (y1 != y2)
    {
      y1 += yInc;
      xShifted += xInc;
      (*(uint32_t *)(&(buffer[(y1 << power) + ((xShifted >> 18) << 2)]))) = color; // Set the color of the pixel to renderer surface
    }
  }
}

//void dads_textureLine(int x1, int y1, int x2, int y2, SDL_Surface *windowSurface, int u1, int v1, int u2, int v2, SDL_Surface *textureSurface)
//{
//  int power = 0; // this will hold what power of 2 the pitch is
//  int pitch = windowSurface->pitch;
//  // exit if pitch is zero
//  if (pitch == 0)
//  {
//    printf("destination has zero width");
//    return;
//  }
//  // keep shifting right while there is a zero in the least significant digit of pitch
//  while ((pitch & 1) == 0)
//  {
//    pitch >>= 1;  // perform the shift
//    power++; // this is counting how many times we had to shift
//  }
//  if (pitch != 1)
//  {
//    printf("destination not a power of 2");
//    return;
//  }
//  int bitmask = 0xffffffffffffffff << power;
//  int eighteenMinusPower = 18 - power; // we are going to use 18 fixed bits of fraction
//
//  uint8_t *buffer = windowSurface->pixels;
//
//  uint32_t color = a << 24 | r << 16 | g << 8 | b;
//
//  int deltaX = x2 - x1;
//  int deltaY = y2 - y1;
//  int absDeltaX = abs(deltaX);
//  int absDeltaY = abs(deltaY);
//
//  if (absDeltaY == 0)
//  {
//    int xInc;
//    if (deltaX < 0)
//    {
//      xInc = -1;
//    }
//    else
//    {
//      xInc = 1;
//    }
//    (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
//    while (x1 != x2)
//    {
//      x1 += xInc;
//      (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
//    }
//  }
//  else if (absDeltaX > absDeltaY)
//  {
//    // moving horizontally
//    int xInc;
//    if (deltaX < 0)
//    {
//      xInc = -1;
//    }
//    else
//    {
//      xInc = 1;
//    }
//    //                                                deltaY                 00000000000000000000000000000000000000000000000000000000000001010010101.0000000000000000
//    //                                                delatX                 00000000000000000000000000000000000000000000000000000001001001010010101.0000000000000000
//    int yInc = (deltaY << 18) / absDeltaX;   // fixed point integer 14 bits with 50 bits of fraction   00000000000000.000000000000000000000000000000000000000000000000000000000
//    int yShifted = y1 << 18;
//    (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
//    while (x1 != x2)
//    {
//      x1 += xInc;
//      yShifted += yInc;
//      (*(uint32_t *)(&(buffer[((yShifted >> eighteenMinusPower) & bitmask) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
//    }
//  }
//  else
//  {
//    // moving vertically
//    int yInc;
//    if (deltaY < 0)
//    {
//      yInc = -1;
//    }
//    else
//    {
//      yInc = 1;
//    }
//    //                                                deltaY                 00000000000000000000000000000000000000000000000000000000000001010010101.0000000000000000
//    //                                                delatX                 00000000000000000000000000000000000000000000000000000001001001010010101.0000000000000000
//    int xInc = (deltaX << 18) / absDeltaY;   // fixed point integer 14 bits with 50 bits of fraction   00000000000000.000000000000000000000000000000000000000000000000000000000
//    int xShifted = x1 << 18;
//    (*(uint32_t *)(&(buffer[(y1 << power) + (x1 << 2)]))) = color; // Set the color of the pixel to renderer surface
//    while (y1 != y2)
//    {
//      y1 += yInc;
//      xShifted += xInc;
//      (*(uint32_t *)(&(buffer[(y1 << power) + ((xShifted >> 18) << 2)]))) = color; // Set the color of the pixel to renderer surface
//    }
//  }
//}


void misc_drawSquare(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, SDL_Surface *windowSurface)
{

//  int x3 = x1;
//  int y3 = y2;
//  int x4 = x2;
//  int y4 = y1;

//  misc_drawLine(x1, y1, x1, y2, r, g, b, a, windowSurface);
//  misc_drawLine(x1, y2, x2, y2, r, g, b, a, windowSurface);
//  misc_drawLine(x2, y2, x2, y1, r, g, b, a, windowSurface);
//  misc_drawLine(x2, y1, x1, y1, r, g, b, a, windowSurface);
}
