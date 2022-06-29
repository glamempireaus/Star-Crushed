#pragma once

// Outside Structs

// Listed Structs
typedef struct Font Font;
typedef enum FontStyle FontStyle;
typedef enum FontHinting FontHinting;

enum FontStyle
{
  FONT_STYLE_NORMAL = 0,
  FONT_STYLE_BOLD = 1,
  FONT_STYLE_ITALIC = 2,
  FONT_STYLE_UNDERLINE = 3,
  FONT_STYLE_STRIKETHROUGH = 4
};

enum FontHinting
{
  FONT_HINTING_NORMAL = 0,
  FONT_HINTING_LIGHT = 1,
  FONT_HINTING_MONO = 2,
  FONT_HINTING_NONE = 3
};

struct Font
{
  TTF_Font *sdlFont;
  const char *filename;
  int size;
  FontStyle style;
  int outline;
  FontHinting hinting;
  bool usekerning;
};

// Create New Font
Font *Font_create(const char* filename, int size, FontStyle style, int outline, FontHinting hinting, bool usekerning);

// Edit Already Added Font
void *Font_edit(Font *font, int size, int style, int outline, int hinting, bool usekerning);
