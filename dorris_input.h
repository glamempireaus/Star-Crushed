#pragma once

typedef enum KeyEventButton KeyEventButton;
typedef enum KeyEventState KeyEventState;
typedef struct KeyEvent KeyEvent;

enum KeyEventButton
{
  KEYEVENT_BUTTON_NONE = 0,
  KEYEVENT_BUTTON_W = 1,
  KEYEVENT_BUTTON_A = 2,
  KEYEVENT_BUTTON_S = 3,
  KEYEVENT_BUTTON_D = 4,
  KEYEVENT_BUTTON_SPACE = 5,
  KEYEVENT_BUTTON_ESCAPE = 6,
};

enum KeyEventState
{
  KEYEVENT_STATE_NONE = 0,
  KEYEVENT_STATE_PRESSED = 1,
  KEYEVENT_STATE_RELEASED = 2
};

struct KeyEvent
{
  KeyEventButton button;
  KeyEventState state;
};

typedef enum MouseEventButton MouseEventButton;
typedef enum MouseEventState MouseEventState;
typedef enum MouseEventType MouseEventType;
typedef struct MouseEvent MouseEvent;

enum MouseEventButton
{
  MOUSEEVENT_BUTTON_NONE = 0,
  MOUSEEVENT_BUTTON_LEFT = 1,
  MOUSEEVENT_BUTTON_RIGHT = 2,
  MOUSEEVENT_BUTTON_MIDDLE = 3,
  MOUSEEVENT_BUTTON_X1 = 4,
  MOUSEEVENT_BUTTON_X2 = 5
};

enum MouseEventState
{
  MOUSEEVENT_STATE_PRESSED = 0,
  MOUSEEVENT_STATE_RELEASED = 1
};

enum MouseEventType
{
  MOUSEEVENT_TYPE_MOUSEMOTION = 0,
  MOUSEEVENT_TYPE_BUTTONDOWN = 1,
  MOUSEEVENT_TYPE_BUTTONUP = 2
};

struct MouseEvent
{
  MouseEventButton button;
  MouseEventState state;
  MouseEventType type;
  Sint32 x;
  Sint32 y;
};

KeyEvent *KeyEvent_create();
void KeyEvent_createFromSDLEvent(KeyEvent *keyEvent, SDL_Event sdlEvent);

MouseEvent *MouseEvent_create();
void MouseEvent_createFromSDLEvent(MouseEvent *mouseEvent, SDL_Event sdlEvent);
