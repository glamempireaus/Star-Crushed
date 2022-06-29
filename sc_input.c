#include "includes.h"

void KeyEvent_createFromSDLEvent(KeyEvent *keyEvent, SDL_Event sdlEvent)
{
  switch (sdlEvent.key.keysym.sym)
  {
    case SDLK_w:
    {
      keyEvent->button = KEYEVENT_BUTTON_W;
      break;
    }
    case SDLK_a:
    {
      keyEvent->button = KEYEVENT_BUTTON_A;
      break;
    }
    case SDLK_s:
    {
      keyEvent->button = KEYEVENT_BUTTON_S;
      break;
    }
    case SDLK_d:
    {
      keyEvent->button = KEYEVENT_BUTTON_D;
      break;
    }
    case SDLK_SPACE:
    {
      keyEvent->button = KEYEVENT_BUTTON_SPACE;
      break;
    }
    case SDLK_ESCAPE:
    {
      keyEvent->button = KEYEVENT_BUTTON_ESCAPE;
      break;
    }
    default:
    {
      keyEvent->button = KEYEVENT_BUTTON_NONE;
    }
  }

  switch (sdlEvent.key.type)
  {
    case SDL_KEYDOWN:
    {
      keyEvent->state = KEYEVENT_STATE_PRESSED;
      break;
    }
    case SDL_KEYUP:
    {
      keyEvent->state = KEYEVENT_STATE_RELEASED;
      break;
    }
  }
}

void MouseEvent_createFromSDLEvent(MouseEvent *mouseEvent, SDL_Event sdlEvent)
{
  switch (sdlEvent.type)
  {
    // Convert to Mouse Motion
    case SDL_MOUSEMOTION:
    {
      mouseEvent->type = MOUSEEVENT_TYPE_MOUSEMOTION;

      if (sdlEvent.motion.state == SDL_PRESSED)
      {
        mouseEvent->state = MOUSEEVENT_STATE_PRESSED;
      }
      else if (sdlEvent.motion.state == SDL_RELEASED)
      {
        mouseEvent->state = MOUSEEVENT_STATE_RELEASED;
      }

      switch (sdlEvent.button.button)
      {
        case SDL_BUTTON_LEFT:
        {
          mouseEvent->button = MOUSEEVENT_BUTTON_LEFT;
          break;
        }
        case SDL_BUTTON_RIGHT:
        {
          mouseEvent->button = MOUSEEVENT_BUTTON_RIGHT;
          break;
        }
        case SDL_BUTTON_MIDDLE:
        {
          mouseEvent->button = MOUSEEVENT_BUTTON_MIDDLE;
          break;
        }
        case SDL_BUTTON_X1:
        {
          mouseEvent->button = MOUSEEVENT_BUTTON_X1;
          break;
        }
        case SDL_BUTTON_X2:
        {
          mouseEvent->button = MOUSEEVENT_BUTTON_X2;
          break;
        }
        default:
        {
          mouseEvent->button = MOUSEEVENT_BUTTON_NONE;
        }
      }

      mouseEvent->x = sdlEvent.motion.x;
      mouseEvent->y = sdlEvent.motion.y;

      break;
    }

    case SDL_MOUSEBUTTONDOWN:
    {
      mouseEvent->type = MOUSEEVENT_TYPE_BUTTONDOWN;
      mouseEvent->button = MOUSEEVENT_BUTTON_LEFT;
      mouseEvent->state = MOUSEEVENT_STATE_PRESSED;

      mouseEvent->x = sdlEvent.button.x;
      mouseEvent->y = sdlEvent.button.y;
      break;
    }

    case SDL_MOUSEBUTTONUP:
    {
      mouseEvent->type = MOUSEEVENT_TYPE_BUTTONUP;
      mouseEvent->button = MOUSEEVENT_BUTTON_LEFT;
      mouseEvent->state = MOUSEEVENT_STATE_RELEASED;

      mouseEvent->x = sdlEvent.button.x;
      mouseEvent->y = sdlEvent.button.y;
      break;
    }
  }
}
