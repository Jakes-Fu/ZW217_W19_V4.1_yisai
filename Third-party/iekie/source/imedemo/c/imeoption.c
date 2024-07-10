
/*
 * imeoption.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "imeoption.h"
#include "mainmenu.h"
#include "menubar.h"
#include "titlebar.h"

#include "IIMEk2api.h"

#define IMEOPTION_ITEM_COUNT    9
#define IMEOPTION_ITEM_HEIGHT   28

static RECT imeoption_pos = {0, GUI_BAR_HEIGHT, GUI_WIDTH, GUI_HEIGHT - GUI_BAR_HEIGHT};//{0, 18, 160, 162};

static int imeoption_font = GUI_SYSTEM_FONT;

static int imeoption_bk_color            = RGB(255, 255, 210);
static int imeoption_text_color          = RGB(0, 0, 0);
static int imeoption_selected_bk_color   = RGB(0, 0, 255);
static int imeoption_selected_text_color = RGB(255, 255, 255);
static int imeoption_on_color            = RGB(255, 0, 0);
static int imeoption_off_color           = RGB(192, 192, 192);

static const WCHAR* imeoption_list[IMEOPTION_ITEM_COUNT] =
{
    L"联想输入",
    L"模糊音 z/zh",
    L"模糊音 c/ch",
    L"模糊音 s/sh",
    L"模糊音 h/f",
    L"模糊音 n/l",
    L"模糊音 in/ing",
    L"模糊音 en/eng",
    L"模糊音 an/ang"
};

static int imeoption_value[IMEOPTION_ITEM_COUNT] =
{
    0, 0, 0, 0, 0, 0, 0, 0
};

static const WCHAR imeoption_on[]  = L"开";
static const WCHAR imeoption_off[] = L"关";

static int imeoption_selected = 1;

static void imeoption_init(void)
{
    CS_IMM_OPTIONS* opt;

    opt = gui_load_ime_options(0);

    if ( (opt->dwAssoPinyin == CS_ASSOCIATION_ON) || (opt->dwAssoStroke == CS_ASSOCIATION_ON) )
        imeoption_value[0] = 1;
    else
        imeoption_value[0] = 0;

    if (opt->dwPYFuzzy & CS_FUZZY_Z_ZH)
        imeoption_value[1] = 1;
    else
        imeoption_value[1] = 0;

    if (opt->dwPYFuzzy & CS_FUZZY_C_CH)
        imeoption_value[2] = 1;
    else
        imeoption_value[2] = 0;

    if (opt->dwPYFuzzy & CS_FUZZY_S_SH)
        imeoption_value[3] = 1;
    else
        imeoption_value[3] = 0;

    if (opt->dwPYFuzzy & CS_FUZZY_H_F)
        imeoption_value[4] = 1;
    else
        imeoption_value[4] = 0;

    if (opt->dwPYFuzzy & CS_FUZZY_N_L)
        imeoption_value[5] = 1;
    else
        imeoption_value[5] = 0;

    if (opt->dwPYFuzzy & CS_FUZZY_IN_ING)
        imeoption_value[6] = 1;
    else
        imeoption_value[6] = 0;

    if (opt->dwPYFuzzy & CS_FUZZY_EN_ENG)
        imeoption_value[7] = 1;
    else
        imeoption_value[7] = 0;

    if (opt->dwPYFuzzy & (CS_FUZZY_AN_ANG | CS_FUZZY_IAN_IANG | CS_FUZZY_UAN_UANG))
        imeoption_value[8] = 1;
    else
        imeoption_value[8] = 0;
}

static void imeoption_save(void)
{
    CS_IMM_OPTIONS* opt;

    opt = gui_load_ime_options(0);

    if (imeoption_value[0])
    {
        opt->dwAssoPinyin = CS_ASSOCIATION_ON;
        opt->dwAssoStroke = CS_ASSOCIATION_ON;
    }
    else
    {
        opt->dwAssoPinyin = CS_ASSOCIATION_OFF;
        opt->dwAssoStroke = CS_ASSOCIATION_OFF;
    }

    opt->dwPYFuzzy = 0;

    if (imeoption_value[1])
        opt->dwPYFuzzy |= CS_FUZZY_Z_ZH;

    if (imeoption_value[2])
        opt->dwPYFuzzy |= CS_FUZZY_C_CH;

    if (imeoption_value[3])
        opt->dwPYFuzzy |= CS_FUZZY_S_SH;

    if (imeoption_value[4])
        opt->dwPYFuzzy |= CS_FUZZY_H_F;

    if (imeoption_value[5])
        opt->dwPYFuzzy |= CS_FUZZY_N_L;

    if (imeoption_value[6])
        opt->dwPYFuzzy |= CS_FUZZY_IN_ING;

    if (imeoption_value[7])
        opt->dwPYFuzzy |= CS_FUZZY_EN_ENG;

    if (imeoption_value[8])
        opt->dwPYFuzzy |= CS_FUZZY_AN_ANG | CS_FUZZY_IAN_IANG | CS_FUZZY_UAN_UANG;

    gui_save_ime_options(opt, sizeof(CS_IMM_OPTIONS));
}

static void imeoption_paint_state(int y, int on)
{
    int x1, x2;

    x1 = 175;
    x2 = 200;

    if (on)
    {
        gui_draw_text(
            x1, y, imeoption_font,
            imeoption_on, lstrlen(imeoption_on),
            imeoption_on_color);
        gui_draw_text(
            x2, y, imeoption_font,
            imeoption_off, lstrlen(imeoption_off),
            imeoption_off_color);
    }
    else
    {
        gui_draw_text(
            x1, y, imeoption_font,
            imeoption_on, lstrlen(imeoption_on),
            imeoption_off_color);
        gui_draw_text(
            x2, y, imeoption_font,
            imeoption_off, lstrlen(imeoption_off),
            imeoption_on_color);
    }
}

static void imeoption_paint(void)
{
    RECT r;
    int i, x;

    /* paint background */

    gui_draw_rect(&imeoption_pos, imeoption_bk_color);

    /* paint item list */

    x = 4;

    r.left   = imeoption_pos.left;
    r.right  = imeoption_pos.right;
    r.top    = imeoption_pos.top;
    r.bottom = r.top + IMEOPTION_ITEM_HEIGHT;

    for (i = 0; i < IMEOPTION_ITEM_COUNT; i++)
    {
        if (i == imeoption_selected)
        {
            gui_draw_rect(&r, imeoption_selected_bk_color);
            gui_draw_text(r.left + x, r.top, imeoption_font,
                imeoption_list[i], lstrlen(imeoption_list[i]),
                imeoption_selected_text_color);
        }
        else
        {
            gui_draw_text(r.left + x, r.top, imeoption_font,
                imeoption_list[i], lstrlen(imeoption_list[i]),
                imeoption_text_color);
        }
        imeoption_paint_state(r.top, imeoption_value[i]);
        r.top += IMEOPTION_ITEM_HEIGHT;
        r.bottom += IMEOPTION_ITEM_HEIGHT;
    }
}

static void imeoption_key_handler(int msg, int key, int time)
{
    switch (key)
    {
        case GUI_KEY_UP:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (imeoption_selected == 0)
                    imeoption_selected = IMEOPTION_ITEM_COUNT - 1;
                else
                    imeoption_selected--;
                imeoption_paint();
            }
            break;

        case GUI_KEY_DOWN:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (imeoption_selected == IMEOPTION_ITEM_COUNT - 1)
                    imeoption_selected = 0;
                else
                    imeoption_selected++;
                imeoption_paint();
            }
            break;

        case GUI_KEY_LEFT:
            if (msg == GUI_MSG_KEYDOWN)
            {
                imeoption_value[imeoption_selected] = 1;
                imeoption_paint();
            }
            break;

        case GUI_KEY_RIGHT:
            if (msg == GUI_MSG_KEYDOWN)
            {
                imeoption_value[imeoption_selected] = 0;
                imeoption_paint();
            }
            break;

        case GUI_KEY_OK:
        case GUI_KEY_LSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
                imeoption_value[imeoption_selected] = 1 - imeoption_value[imeoption_selected];
                imeoption_paint();
            }
            break;

        case GUI_KEY_RSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
                imeoption_save();
                mainmenu_show();
            }
            break;
    }
}

void imeoption_show(void)
{
    imeoption_init();

    titlebar_set(L"输入法设置", L"", L"");
    menubar_set(L"切换", L"回主菜单");

    imeoption_paint();

    gui_set_key_handler(imeoption_key_handler);
}
