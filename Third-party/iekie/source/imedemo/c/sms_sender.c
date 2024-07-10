
/*
 * sms.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "sms_sender.h"
#include "sms.h"
#include "menubar.h"
#include "titlebar.h"
#include "mainmenu.h"

static const WCHAR* sms_sender_text = 0;
static int sms_sender_text_len = 0;

static RECT sms_sender_pos = {0, GUI_BAR_HEIGHT, GUI_WIDTH, GUI_HEIGHT - GUI_BAR_HEIGHT};

static int sms_sender_font = GUI_SYSTEM_FONT;
static int sms_sender_bk_color = RGB(255, 255, 210);
static int sms_sender_text_color = RGB(0, 0, 0);

static void sms_sender_paint(void)
{
    WCHAR s[32] = L"\0";
    WCHAR t[32] = L"\0";
    int x, y, w;

    gui_draw_rect(&sms_sender_pos, sms_sender_bk_color);

    lstrcpy(s, L"短消息已发送");
    w = gui_get_text_width(sms_sender_font, s, lstrlen(s));
    x = sms_sender_pos.left + (sms_sender_pos.right - sms_sender_pos.left - w) / 2;
    y = sms_sender_pos.top + 40;
    gui_draw_text(x, y, sms_sender_font, s, lstrlen(s), sms_sender_text_color);

    if (sms_sender_text_len > 0)
    {
        if (sms_sender_text_len > 9)
        {
            memcpy(t, sms_sender_text, sizeof(WCHAR) * 9);
            t[9] = 0;
            lstrcat(t, L"…");
        }
        else
        {
            wcsncpy(t, sms_sender_text, sms_sender_text_len);
        }
        wsprintf(s, L"“%s”", t);
        w = gui_get_text_width(sms_sender_font, s, lstrlen(s));
        x = sms_sender_pos.left + (sms_sender_pos.right - sms_sender_pos.left - w) / 2;
        y = sms_sender_pos.top + 60;
        gui_draw_text(x, y, sms_sender_font, s, lstrlen(s), RGB(0, 0, 255));
    }

    wsprintf(s, L"共%d个字符", sms_sender_text_len);
    w = gui_get_text_width(sms_sender_font, s, lstrlen(s));
    x = sms_sender_pos.left + (sms_sender_pos.right - sms_sender_pos.left - w) / 2;
    y = sms_sender_pos.top + 80;
    gui_draw_text(x, y, sms_sender_font, s, lstrlen(s), sms_sender_text_color);
}

static void sms_sender_key_handler(int msg, int key, int time)
{
    switch (key)
    {
        case GUI_KEY_RSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
                mainmenu_show();
            }
            break;
    }
}

void sms_sender_show(
    const WCHAR*            text,
    int                     text_len)
{
    sms_sender_text = text;
    sms_sender_text_len = text_len;

    titlebar_set(L"发送短消息", L"", L"");
    menubar_set(L"", L"回主菜单");

    sms_sender_paint();
    gui_set_key_handler(sms_sender_key_handler);
}
