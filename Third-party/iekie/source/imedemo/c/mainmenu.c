
/*
 * mainmenu.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "IIMEk2api.h"
#include "mainmenu.h"
#include "menubar.h"
#include "titlebar.h"
#include "sms.h"
#include "imeoption.h"
#include "ime_ldb.h"
#include "multi_line_editor.h"

#define MAINMENU_ITEM_COUNT     8
#define MAINMENU_ITEM_HEIGHT    32

static RECT mainmenu_pos = {0, GUI_BAR_HEIGHT, GUI_WIDTH, GUI_HEIGHT - GUI_BAR_HEIGHT};

static int mainmenu_font = GUI_SYSTEM_FONT;

static int mainmenu_bk_color            = RGB(255, 255, 210);
static int mainmenu_text_color          = RGB(0, 0, 0);
static int mainmenu_selected_bk_color   = RGB(0, 0, 255);
static int mainmenu_selected_text_color = RGB(255, 255, 255);

static const WCHAR* mainmenu_list[MAINMENU_ITEM_COUNT] =
{
    L"短消息",
    L"输入法数据库选择",
    L"输入法设置",
    L"照相机",
    L"浏览器",
    L"播放器",
    L"收音机",
    L"游戏"
};

static int mainmenu_selected = 0;

static void mainmenu_paint(void)
{
    RECT r;
    int i, x, y, h;

    /* paint background */

    gui_draw_rect(&mainmenu_pos, mainmenu_bk_color);

    /* paint item list */

    h = gui_get_text_height(mainmenu_font);

    x = 4;
    y = (MAINMENU_ITEM_HEIGHT - h) / 2;

    r.left = mainmenu_pos.left;
    r.right = mainmenu_pos.right;
    r.top = mainmenu_pos.top;
    r.bottom = r.top + MAINMENU_ITEM_HEIGHT;

    for (i = 0; i < MAINMENU_ITEM_COUNT; i++)
    {
        if (i == mainmenu_selected)
        {
            gui_draw_rect(&r, mainmenu_selected_bk_color);
            gui_draw_text(r.left + x, r.top + y, mainmenu_font,
                mainmenu_list[i], lstrlen(mainmenu_list[i]),
                mainmenu_selected_text_color);
        }
        else
        {
            gui_draw_text(r.left + x, r.top + y, mainmenu_font,
                mainmenu_list[i], lstrlen(mainmenu_list[i]),
                mainmenu_text_color);
        }
        r.top += MAINMENU_ITEM_HEIGHT;
        r.bottom += MAINMENU_ITEM_HEIGHT;
    }
}

static void mainmenu_key_handler(int msg, int key, int time)
{
    switch (key)
    {
        case GUI_KEY_UP:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (mainmenu_selected == 0)
                    mainmenu_selected = MAINMENU_ITEM_COUNT - 1;
                else
                    mainmenu_selected--;
                mainmenu_paint();
            }
            break;

        case GUI_KEY_DOWN:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (mainmenu_selected == MAINMENU_ITEM_COUNT - 1)
                    mainmenu_selected = 0;
                else
                    mainmenu_selected++;
                mainmenu_paint();
            }
            break;

        case GUI_KEY_OK:
        case GUI_KEY_LSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
                if (mainmenu_selected == 0)
                {
                    mle_reset(&sms_editor);
                    sms_show();
                }
                else if (mainmenu_selected == 1)
                {
                    mle_reset(&sms_editor);
//                    ime_ldb_show();
                }
                else if (mainmenu_selected == 2)
                {
                    mle_reset(&sms_editor);
                    imeoption_show();
                }
            }
            break;
    }
}

void mainmenu_show(void)
{
    titlebar_set(L"主菜单", L"", L"");
    menubar_set(L"选择", L"");

    mainmenu_paint();

    gui_set_key_handler(mainmenu_key_handler);
}
