
/*
 * gui_imp.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef GUI_IMP_H
#define GUI_IMP_H

#include "imedemo.h"

extern HBITMAP lcd_bitmap;

void gui_init(void);
void gui_cleanup(void);

void gui_handle_key(int msg, int key, int time);
void gui_handle_timer(int timer_id);
void gui_handle_char(int key);

void gui_paint(HDC hdc);

#endif /* GUI_IMP_H */
