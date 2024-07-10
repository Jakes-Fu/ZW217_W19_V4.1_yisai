
/*
 * touchkeypad.h
 *
 * Copyright (C) 2009 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef TOUCHKEYPAD_H
#define TOUCHKEYPAD_H

void keypad_create(int bottom_line);

void keypad_destroy();

void keypad_redraw();

int keypad_handle_event(int event, unsigned long wParam, unsigned long lParam);

void keypad_show();

void keypad_hide();

#endif /* TOUCHKEYPAD_H */
