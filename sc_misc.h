#pragma once

void dorris_drawCircle(SDL_Renderer *renderer, float pos_x, float pos_y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void misc_drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, SDL_Surface *windowSurface);
void dads_drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, SDL_Surface *windowSurface);
void dads_textureLine(int x1, int y1, int x2, int y2, SDL_Surface *windowSurface, int u1, int v1, int u2, int v2, SDL_Surface *textureSurface);
void misc_drawSquare(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, SDL_Surface *windowSurface);
