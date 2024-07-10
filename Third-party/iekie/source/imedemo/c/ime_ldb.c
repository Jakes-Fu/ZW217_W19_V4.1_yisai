/*
 * ime_ldb.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "imedemo.h"
#include "mainmenu.h"
#include "menubar.h"
#include "titlebar.h"

#include "IIMEk2api.h"
#include "ime_ldb.h"
#include "kmx_all_ldb.h"
#include "kmxime.h"

#define IME_LDB_ITEM_COUNT    9
#define IME_LDB_ITEM_HEIGHT   28

static RECT ime_ldb_pos = {0, GUI_BAR_HEIGHT, GUI_WIDTH, GUI_HEIGHT - GUI_BAR_HEIGHT};//{0, 18, 160, 162};

static int ime_ldb_font = GUI_SYSTEM_FONT;

static int ime_ldb_bk_color            = RGB(255, 255, 210);
static int ime_ldb_text_color          = RGB(0, 0, 0);
static int ime_ldb_selected_bk_color   = RGB(0, 0, 255);
static int ime_ldb_selected_text_color = RGB(255, 255, 255);
static int ime_ldb_on_color            = RGB(255, 0, 0);
static int ime_ldb_off_color           = RGB(192, 192, 192);

static int ime_ldb_top = 0;
static int ime_ldb_selected = 0;
static int ime_ldb_count = 0;
static int ime_ldb_menu_id = 200;

static const WCHAR *name[] = 
{
    L"IIME_ldb_py_gb2312_c1_20k",
    L"IIME_ldb_py_gb2312_c1_10k",
    L"IIME_ldb_py_gb2312_c1_5k",
    L"IIME_ldb_py_gb2312_c2_20k",
    L"IIME_ldb_py_gb2312_c2_10k",
    L"IIME_ldb_py_gb2312_c2_5k",
    L"IIME_ldb_py_gbk_c1_20k",
    L"IIME_ldb_py_gbk_c1_10k",
    L"IIME_ldb_py_gbk_c1_5k",
    L"IIME_ldb_py_gbk_c2_20k",
    L"IIME_ldb_py_gbk_c2_10k",
    L"IIME_ldb_py_gbk_c2_5k",
    L"IIME_ldb_py_gb18030_c1_20k",
    L"IIME_ldb_py_gb18030_c1_10k",
    L"IIME_ldb_py_gb18030_c1_5k",
    L"IIME_ldb_py_gb18030_c2_20k",
    L"IIME_ldb_py_gb18030_c2_10k",
    L"IIME_ldb_py_gb18030_c2_5k",

    L"IIME_ldb_py_big5_c1_20k",
    L"IIME_ldb_py_bignormal_c1_20k",
    
    L"IIME_ldb_dzpy_gb2312_20k",
    L"IIME_ldb_dzpy_gb2312_10k",
    L"IIME_ldb_dzpy_gb2312_5k",
    L"IIME_ldb_dzpy_gb2312_0k",
    
    L"IIME_ldb_dzpy_gbk_20k",
    L"IIME_ldb_dzpy_gbk_10k",
    L"IIME_ldb_dzpy_gbk_5k",
    L"IIME_ldb_dzpy_gbk_0k"
};

static const WCHAR *lang_name[] = 
{
    L"English",
    0
};

IIME_IME_INFO IIME_ldb_pinyin_dynamic = {IM_PINYIN, 0};
IIME_IME_INFO IIME_ldb_zhuyin_dynamic = {IM_ZHUYIN_MT, 0};

static unsigned int *ldb_buffer = 0;

static KMX_LDB_HEADER ldb_header = {0}; 

static void ime_ldb_init(void)
{
    DWORD file_size;
    //WCHAR file_name[] = L"D:\\input_method\\csk2\\src\\ldb_pinyin\\py_gb2312_level1_20k_digit.le";
    WCHAR file_name[] = L"D:\\input_method\\csk2\\src\\ldb_pinyin\\dzpy_gb2312_level0_5k_qwerty.le";

    HANDLE hFile = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD temp;
        file_size = GetFileSize(hFile, NULL);

        if (ldb_buffer)
            free(ldb_buffer);

        ldb_buffer = malloc(file_size);
        
        ReadFile(hFile, ldb_buffer, file_size, &temp, NULL);
        
        CloseHandle(hFile);

        if (kmx_pinyin_relocate_ldb(ldb_buffer, file_size, &ldb_header))
            IIME_ldb_pinyin_dynamic.language_database = &ldb_header;
    }

    {
        HMENU hMenu, hSubMenu;

        hMenu = GetMenu(g_main_window);
        hSubMenu = GetSubMenu(hMenu, 4);
        ime_ldb_count = GetMenuItemCount(hSubMenu);
    }
}

static void ime_ldb_paint(void)
{
    RECT r;
    int x;

    int i;
    HMENU hMenu, hSubMenu;

    /* paint background */

    gui_draw_rect(&ime_ldb_pos, ime_ldb_bk_color);

    /* paint item list */

    x = 4;

    r.left   = ime_ldb_pos.left;
    r.right  = ime_ldb_pos.right;
    r.top    = ime_ldb_pos.top;
    r.bottom = r.top + IME_LDB_ITEM_HEIGHT;

    hMenu = GetMenu(g_main_window);
    hSubMenu = GetSubMenu(hMenu, 4);

    for (i = 0; i < IME_LDB_ITEM_COUNT; i++)
    {
        int pos = i + ime_ldb_top;
        MENUITEMINFO menu_info;

        memset(&menu_info, 0, sizeof(MENUITEMINFO));
        menu_info.cbSize = sizeof(MENUITEMINFO);
        menu_info.fMask = MIIM_STRING | MIIM_ID;
        menu_info.fType = MFT_STRING;

        GetMenuItemInfo(hSubMenu, pos, TRUE, &menu_info);

        menu_info.dwTypeData = malloc(sizeof(TCHAR) * (menu_info.cch + 1));
        menu_info.cch++;

        GetMenuItemInfo(hSubMenu, pos, TRUE, &menu_info);

        if (pos == ime_ldb_selected)
        {
            gui_draw_rect(&r, ime_ldb_selected_bk_color);
            gui_draw_text(r.left + x, r.top, ime_ldb_font,
                menu_info.dwTypeData, menu_info.cch,
                ime_ldb_selected_text_color);

            ime_ldb_menu_id = menu_info.wID;
        }
        else
        {
            gui_draw_text(r.left + x, r.top, ime_ldb_font,
                menu_info.dwTypeData, menu_info.cch,
                ime_ldb_text_color);
        }

        free(menu_info.dwTypeData);

        r.top += IME_LDB_ITEM_HEIGHT;
        r.bottom += IME_LDB_ITEM_HEIGHT;
    }
}

static void ime_ldb_key_handler(int msg, int key, int time)
{
    switch (key)
    {
        case GUI_KEY_UP:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (ime_ldb_selected == 0)
                {
                    ime_ldb_selected = ime_ldb_count - 1;
                    ime_ldb_top = ime_ldb_count - IME_LDB_ITEM_COUNT;
                }
                else
                {
                    ime_ldb_selected--;

                    if (ime_ldb_top > ime_ldb_selected)
                        ime_ldb_top = ime_ldb_selected;
                }
                ime_ldb_paint();
            }
            break;

        case GUI_KEY_DOWN:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (ime_ldb_selected == ime_ldb_count - 1)
                {
                    ime_ldb_selected = 0;
                    ime_ldb_top = 0;
                }
                else
                {
                    ime_ldb_selected++;
                    if (ime_ldb_top + IME_LDB_ITEM_COUNT <= ime_ldb_selected)
                        ime_ldb_top++;
                }
                ime_ldb_paint();
            }
            break;

        case GUI_KEY_OK:
        case GUI_KEY_LSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
//                imeoption_value[imeoption_selected] = 1 - imeoption_value[imeoption_selected];
//                imeoption_paint();
            }
            break;

        case GUI_KEY_RSK:
            if (msg == GUI_MSG_KEYDOWN)
            {
//                imeoption_save();
                mainmenu_show();
            }
            break;
    }
}

void ime_ldb_show(void)
{
    ime_ldb_init();

    titlebar_set(L"选择输入法库", L"", L"");
    menubar_set(L"选择", L"回主菜单");

    ime_ldb_paint();

    gui_set_key_handler(ime_ldb_key_handler);
}

void ime_language_show(void)
{

}
