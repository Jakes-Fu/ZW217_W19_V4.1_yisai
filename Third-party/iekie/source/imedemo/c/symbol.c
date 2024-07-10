
/*
 * symbol.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "symbol.h"
#include "menubar.h"
#include "titlebar.h"
#include "sms.h"

#define SYMBOL_ROW      7
#define SYMBOL_COL      8
#define SYMBOL_COUNT    (SYMBOL_ROW * SYMBOL_COL)

#define SYMBOL_CX_CELL  34
#define SYMBOL_CY_CELL  34

static RECT symbol_pos = {0, GUI_BAR_HEIGHT, GUI_WIDTH, GUI_HEIGHT - GUI_BAR_HEIGHT};

static WCHAR symbol_list[] =
    L"，。！？、：“”"
    L"＠＃％／＼＊＆～"
    L"；…＋－＝｜℃※"
    L"［］｛｝＜＞％‰"
    L"（）《》〖〗【】"
    L"￥￡＄￠¤…—·"
    L"☆★○●◇◆△▲";

static int symbol_selected_row = 0;
static int symbol_selected_col = 0;

static int symbol_font = GUI_SYSTEM_FONT;

static int symbol_bk_color   = RGB(255, 255, 210);
static int symbol_text_color = RGB(0, 0, 0);

static int symbol_selected_bk_color   = RGB(0, 255, 0);
static int symbol_selected_text_color = RGB(0, 0, 0);

static void symbol_paint()
{
    RECT r;
    WCHAR c;
    int x, y, w, h, col, row;

    gui_draw_rect(&symbol_pos, symbol_bk_color);

    h = gui_get_text_height(symbol_font);

    for (row = 0; row < SYMBOL_ROW; row++)
    {
        r.top = symbol_pos.top + (symbol_pos.bottom - symbol_pos.top - SYMBOL_ROW * SYMBOL_CY_CELL) / 2 + row * SYMBOL_CY_CELL;
        r.bottom = r.top + SYMBOL_CY_CELL;
        for (col = 0; col < SYMBOL_COL; col++)
        {
            r.left = symbol_pos.left + (symbol_pos.right - symbol_pos.left - SYMBOL_COL * SYMBOL_CX_CELL) / 2 + col * SYMBOL_CX_CELL;
            r.right = r.left + SYMBOL_CX_CELL;
            c = symbol_list[row * SYMBOL_COL + col];
            w = gui_get_text_width(symbol_font, &c, 1);
            x = r.left + (SYMBOL_CX_CELL - w) / 2;
            y = r.top + (SYMBOL_CY_CELL - h) / 2;
            if ( (row == symbol_selected_row) && (col == symbol_selected_col) )
            {
                gui_draw_rect(&r, symbol_selected_bk_color);
                gui_draw_text(x, y, symbol_font, &c, 1, symbol_selected_text_color);
            }
            else
            {
                gui_draw_text(x, y, symbol_font, &c, 1, symbol_text_color);
            }
        }
    }
}

static void symbol_key_handler(int msg, int key, int time)
{
    WCHAR c;

    switch (key)
    {
        case GUI_KEY_LEFT:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (symbol_selected_col > 0)
                {
                    symbol_selected_col--;
                    symbol_paint();
                }
            }
            break;

        case GUI_KEY_RIGHT:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (symbol_selected_col + 1 < SYMBOL_COL)
                {
                    symbol_selected_col++;
                    symbol_paint();
                }
            }
            break;

        case GUI_KEY_UP:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (symbol_selected_row > 0)
                {
                    symbol_selected_row--;
                    symbol_paint();
                }
            }
            break;

        case GUI_KEY_DOWN:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (symbol_selected_row + 1 < SYMBOL_ROW)
                {
                    symbol_selected_row++;
                    symbol_paint();
                }
            }
            break;

        case GUI_KEY_OK:
        case GUI_KEY_LSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
                c = symbol_list[symbol_selected_row * SYMBOL_COL + symbol_selected_col];
                sms_show();
                mle_insert_char(&sms_editor, c);
                sms_update_status();
            }
            break;

        case GUI_KEY_RSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
                sms_show();
            }
            break;
    }
}

void symbol_show(void)
{
    menubar_set(L"选择", L"取消");
    titlebar_set(L"符号表", L"", L"");

    symbol_paint();

    gui_set_key_handler(symbol_key_handler);
}
