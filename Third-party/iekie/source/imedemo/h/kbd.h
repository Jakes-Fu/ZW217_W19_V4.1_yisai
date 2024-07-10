
/*
 * kbd.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef KBD_H
#define KBD_H

#include "imedemo.h"

void kbd_init(void);
void kbd_cleanup(void);

void kbd_update_layout(void);

int kbd_hit_test(const POINT* pt);
void kbd_show_pressed_key(int key);

void kbd_paint(HDC hdc);

#endif /* KBD_H */
