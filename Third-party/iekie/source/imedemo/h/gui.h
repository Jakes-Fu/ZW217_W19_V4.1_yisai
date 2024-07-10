
/*
 * gui.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef GUI_H
#define GUI_H

#include "config.h"

#define GUI_KEY_0               0
#define GUI_KEY_1               1
#define GUI_KEY_2               2
#define GUI_KEY_3               3
#define GUI_KEY_4               4
#define GUI_KEY_5               5
#define GUI_KEY_6               6
#define GUI_KEY_7               7
#define GUI_KEY_8               8
#define GUI_KEY_9               9
#define GUI_KEY_STAR            10
#define GUI_KEY_POUND           11
#define GUI_KEY_LEFT            12
#define GUI_KEY_RIGHT           13
#define GUI_KEY_UP              14
#define GUI_KEY_DOWN            15
#define GUI_KEY_OK              16
#define GUI_KEY_LSK             17
#define GUI_KEY_RSK             18
#define GUI_KEY_L1              19
#define GUI_KEY_L2              20
#define GUI_KEY_L3              21
#define GUI_KEY_R1              22
#define GUI_KEY_R2              23
#define GUI_KEY_R3              24
#define GUI_KEY_L4              25
#define GUI_KEY_R4              26

#define GUI_KEY_COUNT           27//19

#define GUI_KEYBOARD_DIGIT      0
#define GUI_KEYBOARD_QWERTY     1
#define GUI_KEYBOARD_SURETYPE   2

#define GUI_MSG_KEYDOWN         1
#define GUI_MSG_KEYREPEAT       2
#define GUI_MSG_KEYUP           3

#define GUI_WIDTH               288     /* width of LCD display in pixel  */
#define GUI_HEIGHT              336     /* height of LCD display in pixel */
#define GUI_BAR_HEIGHT          36

#define GUI_SYSTEM_FONT         0
#define GUI_SMALL_FONT          1

#define GUI_IME_TIMER_ID_1      201
#define GUI_IME_TIMER_ID_2      202

typedef void (*GUI_KEY_HANDLER)(int msg, int key, int time);
typedef void (*GUI_CHAR_HANDLER)(int key);

typedef void (*GUI_TIMER_HANDLER)(void);

extern int gui_input_method_count;
extern int* gui_input_method_list;

void gui_set_key_handler(GUI_KEY_HANDLER handler);
void gui_set_char_handler(GUI_CHAR_HANDLER handler);

void gui_enable_ime_timer(int time, GUI_TIMER_HANDLER handler);
void gui_disable_ime_timer(void);

void gui_enable_timer(int time, int timer_id, GUI_TIMER_HANDLER handler);
void gui_disable_timer(int timer_id);

void gui_draw_pixel(int x, int y, int color);
void gui_draw_line(int x1, int y1, int x2, int y2, int color);
void gui_draw_rect(const RECT* r, int color);
void gui_draw_button(int x, int y, int id);
void gui_draw_text(int x, int y, int font, const WCHAR* text, int text_len, int color);

int gui_get_text_height(int font);
int gui_get_char_width(int font, WCHAR chr);
int gui_get_text_width(int font, const WCHAR* text, int text_len);

void* gui_load_ime_userdata(int* size);
void gui_save_ime_userdata(void* data, int size);

void* gui_load_ime_options(int* size);
void gui_save_ime_options(void* data, int size);

const void gui_get_input_method_name(int input_method, WCHAR *pName);

void gui_set_default_language(int lang);

#endif /* GUI_H */
