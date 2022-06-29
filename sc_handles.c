#include "includes.h"

// Global Variables
Widget *previous_widget = NULL;

// Handle cursor
Widget *handle_cursorOverWindow(Window *window, MouseEvent *mouseEvent)
{
  Widget *return_widget = NULL;

  Widget *current_widget;
  current_widget = getWidgetFromPos(window, mouseEvent->x, mouseEvent->y); // Set Current Widget

  // **Reset previous widget before changing current widget's properties**
  if (previous_widget != NULL)
  {
    // Button is activated
    if (current_widget != NULL && mouseEvent->state != MOUSEEVENT_STATE_PRESSED && previous_widget->common.state == WIDGET_STATE_CLICK && current_widget == previous_widget)
    {
      return_widget = current_widget;
    }

    // Button unclicked on different widget
    if (mouseEvent->state != MOUSEEVENT_STATE_PRESSED && previous_widget != current_widget)
    {
      previous_widget->common.state = WIDGET_STATE_DEFAULT;
    }
  }

  if (current_widget != NULL)
  {
    // Button clicked on widget
    if (current_widget->common.state == WIDGET_STATE_HOVER && mouseEvent->type == MOUSEEVENT_TYPE_BUTTONDOWN)
    {
      current_widget->common.state = WIDGET_STATE_CLICK;
      previous_widget = current_widget; // Save widget for next cursor handle
    }

    // Button over widget
    if (mouseEvent->type != MOUSEEVENT_TYPE_BUTTONDOWN && mouseEvent->state == MOUSEEVENT_STATE_RELEASED)
    {
      current_widget->common.state = WIDGET_STATE_HOVER;
      previous_widget = current_widget; // Save widget for next cursor handle
    }
  }

  return return_widget; // returns NULL on non-activation or the widget clicked
}

void handle_KeyPressOverGame(WidgetGame *widgetGame, KeyEvent *keyEvent)
{
  if (keyEvent->state == KEYEVENT_STATE_PRESSED)
  {
    switch (keyEvent->button)
    {
      case KEYEVENT_BUTTON_W:
      {
        widgetGame->pressedkey_w = true;
        break;
      }
      case KEYEVENT_BUTTON_A:
      {
        widgetGame->pressedkey_a = true;
        break;
      }
      case KEYEVENT_BUTTON_S:
      {
        widgetGame->pressedkey_s = true;
        break;
      }
      case KEYEVENT_BUTTON_D:
      {
        widgetGame->pressedkey_d = true;
        break;
      }
      case KEYEVENT_BUTTON_SPACE:
      {
        widgetGame->pressedkey_space = true;
        break;
      }
      default:
      {
        break;
      }
    }
  }
  else if (keyEvent->state == KEYEVENT_STATE_RELEASED)
  {
    switch (keyEvent->button)
    {
      case KEYEVENT_BUTTON_W:
      {
        widgetGame->pressedkey_w = false;
        break;
      }
      case KEYEVENT_BUTTON_A:
      {
        widgetGame->pressedkey_a = false;
        break;
      }
      case KEYEVENT_BUTTON_S:
      {
        widgetGame->pressedkey_s = false;
        break;
      }
      case KEYEVENT_BUTTON_D:
      {
        widgetGame->pressedkey_d = false;
        break;
      }
      case KEYEVENT_BUTTON_SPACE:
      {
        widgetGame->pressedkey_space = false;
        break;
      }
      default:
      {
        break;
      }
    }
  }
}





































