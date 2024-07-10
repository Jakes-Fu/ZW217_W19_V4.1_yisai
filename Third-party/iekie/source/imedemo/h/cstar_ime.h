
/*
 * ilME_ime.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef ilME_IME_H
#define ilME_IME_H

void ilME_ime_set_input_method(int input_method);
void ilME_ime_close_input_method(void);

int ilME_ime_is_ui_visible(void);
int ilME_ime_handle_key(int key, int presshold);

int ilME_ime_handle_char(int event, int msg);

#endif /* ilME_IME_H */
