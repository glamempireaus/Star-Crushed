#include "includes.h"

Font *Font_create(const char* filename, int size, FontStyle style, int outline, FontHinting hinting, bool usekerning)
{
  Font *font = malloc(sizeof(Font));

  // Create SDL Font
  font->sdlFont = TTF_OpenFont(filename, size);
  if (!font->sdlFont)
  {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    exit(2);
  }

  // Store Attributes
  font->filename = filename;
  font->size = size;
  font->style = style;
  font->outline = outline;
  font->hinting = hinting;
  font->usekerning = usekerning;

  // Run SDL Functions
  TTF_SetFontStyle(font->sdlFont, style);
  TTF_SetFontOutline(font->sdlFont, outline);
  TTF_SetFontHinting(font->sdlFont, hinting);
  TTF_SetFontKerning(font->sdlFont, usekerning);

  return font;
}
