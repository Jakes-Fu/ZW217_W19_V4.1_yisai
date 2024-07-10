
/*
 * menubar.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "menubar.h"

static RECT menubar_pos             = {0, GUI_HEIGHT - GUI_BAR_HEIGHT, GUI_WIDTH, GUI_HEIGHT};

static int menubar_left_text_len    = 0;
static WCHAR menubar_left_text[32]  = L"";
static RECT menubar_left_text_pos   = {0, GUI_HEIGHT - GUI_BAR_HEIGHT, GUI_WIDTH / 2, GUI_HEIGHT};

static int menubar_right_text_len   = 0;
static WCHAR menubar_right_text[32] = L"";
static RECT menubar_right_text_pos  = {GUI_WIDTH / 2, GUI_HEIGHT - GUI_BAR_HEIGHT, GUI_WIDTH, GUI_HEIGHT};

static int menubar_font             = GUI_SYSTEM_FONT;
static int menubar_bk_color         = RGB(183, 91, 0);
static int menubar_text_color       = RGB(255, 255, 255);

static void menubar_paint(void)
{
    int x, y, w, h;

    gui_draw_rect(&menubar_pos, menubar_bk_color);

    h = gui_get_text_height(menubar_font);
    y = menubar_pos.top + (menubar_pos.bottom - menubar_pos.top - h) / 2;

    w = gui_get_text_width(menubar_font, menubar_left_text, lstrlen(menubar_left_text));
    x = menubar_left_text_pos.left + (menubar_left_text_pos.right - menubar_left_text_pos.left - w) / 2;
    gui_draw_text(x, y, menubar_font,
        menubar_left_text, lstrlen(menubar_left_text),
        menubar_text_color);

    w = gui_get_text_width(menubar_font, menubar_right_text, lstrlen(menubar_right_text));
    x = menubar_right_text_pos.left + (menubar_right_text_pos.right - menubar_right_text_pos.left - w) / 2;
    gui_draw_text(x, y, menubar_font,
        menubar_right_text, lstrlen(menubar_right_text),
        menubar_text_color);
}

void menubar_set(
    const WCHAR* left_text,
    const WCHAR* right_text)
{
    lstrcpy(menubar_left_text, left_text);
    menubar_left_text_len = lstrlen(menubar_left_text);

    lstrcpy(menubar_right_text, right_text);
    menubar_right_text_len = lstrlen(menubar_right_text);

    menubar_paint();
}
