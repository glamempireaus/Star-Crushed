#pragma once

// Outside Structs
typedef struct MouseEvent MouseEvent;
typedef struct KeyEvent KeyEvent;

Widget *handle_cursorOverWindow(Window *window, MouseEvent *mouseEvent);
void handle_KeyPressOverGame(WidgetGame *widgetGame, KeyEvent *keyEvent);
