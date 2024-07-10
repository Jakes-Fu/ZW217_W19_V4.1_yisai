
/*
 * titlebar.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "titlebar.h"

static RECT titlebar_pos                = {0, 0, GUI_WIDTH, GUI_BAR_HEIGHT};

static int titlebar_title_len           = 0;
static WCHAR titlebar_title[32]         = L"";
static RECT titlebar_title_pos          = {4, 0, GUI_WIDTH / 3, GUI_BAR_HEIGHT};

static int titlebar_ime_status_len      = 0;
static WCHAR titlebar_ime_status[16]    = L"";
static RECT titlebar_ime_status_pos     = {GUI_WIDTH / 3, 0, GUI_WIDTH * 2 / 3, GUI_BAR_HEIGHT};

static int titlebar_editor_status_len   = 0;
static WCHAR titlebar_editor_status[16] = L"";
static RECT titlebar_editor_status_pos  = {GUI_WIDTH * 2 / 3, 0, GUI_WIDTH - 4, GUI_BAR_HEIGHT};

static int titlebar_font                = GUI_SYSTEM_FONT;
static int titlebar_bk_color            = RGB(135, 39, 146);
static int titlebar_text_color          = RGB(255, 255, 255);

static void titlebar_paint(void)
{
    int x, y, w, h;

    gui_draw_rect(&titlebar_pos, titlebar_bk_color);

    h = gui_get_text_height(titlebar_font);
    y = titlebar_pos.top + (titlebar_pos.bottom - titlebar_pos.top - h) / 2;

    if (titlebar_title_len > 0)
    {
        x = titlebar_title_pos.left;
        gui_draw_text(x, y, titlebar_font,
            titlebar_title, titlebar_title_len,
            titlebar_text_color);
    }

    if (titlebar_ime_status_len > 0)
    {
        x = titlebar_ime_status_pos.left;
        gui_draw_text(x, y, titlebar_font,
            titlebar_ime_status, titlebar_ime_status_len,
            titlebar_text_color);
    }

    if (titlebar_editor_status > 0)
    {
        w = gui_get_text_width(titlebar_font, titlebar_editor_status, titlebar_editor_status_len);
        x = titlebar_editor_status_pos.right - w;
        gui_draw_text(x, y, titlebar_font,
            titlebar_editor_status, lstrlen(titlebar_editor_status),
            titlebar_text_color);
    }
}

void titlebar_set(
    const WCHAR*        title,
    const WCHAR*        ime_status,
    const WCHAR*        editor_status)
{
    lstrcpy(titlebar_title, title);
    titlebar_title_len = lstrlen(titlebar_title);

    lstrcpy(titlebar_ime_status, ime_status);
    titlebar_ime_status_len = lstrlen(titlebar_ime_status);

    lstrcpy(titlebar_editor_status, editor_status);
    titlebar_editor_status_len = lstrlen(titlebar_editor_status);

    titlebar_paint();
}

void titlebar_set_title(
    const WCHAR*        title)
{
    lstrcpy(titlebar_title, title);
    titlebar_title_len = lstrlen(titlebar_title);

    titlebar_paint();
}

void titlebar_set_ime_status(
    const WCHAR*        ime_status)
{
    lstrcpy(titlebar_ime_status, ime_status);
    titlebar_ime_status_len = lstrlen(titlebar_ime_status);

    titlebar_paint();
}

void titlebar_set_editor_status(
    const WCHAR*        editor_status)
{
    lstrcpy(titlebar_editor_status, editor_status);
    titlebar_editor_status_len = lstrlen(titlebar_editor_status);

    titlebar_paint();
}
