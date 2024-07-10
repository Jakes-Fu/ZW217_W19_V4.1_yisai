
/*
 * kbd.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "kbd.h"
#include "gui.h"
#include "emu_option.h"
#include "IIMEk2api.h"

typedef struct tagKEY_DEFINE
{
    WCHAR   label1[16];
    WCHAR   label2[16];
    WCHAR   label3[16];
} KEY_DEFINE;

static const KEY_DEFINE KBD_LAYOUT_0[GUI_KEY_COUNT] =
{
    /* GUI_KEY_0     */ { L"0",       L"",        L""         },
    /* GUI_KEY_1     */ { L"1",       L"",        L"\x4E00"   },
    /* GUI_KEY_2     */ { L"2",       L"ABC",     L"\x4E28"   },
    /* GUI_KEY_3     */ { L"3",       L"DEF",     L"\x4E3F"   },
    /* GUI_KEY_4     */ { L"4",       L"GHI",     L"\x4E36"   },
    /* GUI_KEY_5     */ { L"5",       L"JKL",     L"\x4E5B"   },
    /* GUI_KEY_6     */ { L"6",       L"MNO",     L"\xFF1F"   },
    /* GUI_KEY_7     */ { L"7",       L"PQRS",    L""         },
    /* GUI_KEY_8     */ { L"8",       L"TUV",     L""         },
    /* GUI_KEY_9     */ { L"9",       L"WXYZ",    L""         },
    /* GUI_KEY_STAR  */ { L"*",       L"",        L""         },
    /* GUI_KEY_POUND */ { L"#",       L"",        L""         },
    /* GUI_KEY_LEFT  */ { L"\x25C4",  L"",        L""         },
    /* GUI_KEY_RIGHT */ { L"\x25BA",  L"",        L""         },
    /* GUI_KEY_UP    */ { L"\x25B2",  L"",        L""         },
    /* GUI_KEY_DOWN  */ { L"\x25BC",  L"",        L""         },
    /* GUI_KEY_OK    */ { L"",        L"",        L""         },
    /* GUI_KEY_LSK   */ { L"",        L"",        L""         },
    /* GUI_KEY_RSK   */ { L"C",       L"",        L""         },

    /* GUI_KEY_10    */ { 0 },
    /* GUI_KEY_11    */ { 0 },
    /* GUI_KEY_12    */ { 0 },
    /* GUI_KEY_13    */ { 0 },
    /* GUI_KEY_14    */ { 0 },
    /* GUI_KEY_15    */ { 0 },
    /* GUI_KEY_16    */ { 0 },
    /* GUI_KEY_17    */ { 0 }
};

static const KEY_DEFINE KBD_LAYOUT_1[GUI_KEY_COUNT] =
{
    /* GUI_KEY_0     */ { L"0",       L"",        L""         },
    /* GUI_KEY_1     */ { L"1",       L"",        L""         },
    /* GUI_KEY_2     */ { L"2",       L"ABC",     L"\x4E00"   },
    /* GUI_KEY_3     */ { L"3",       L"DEF",     L"\x4E28"   },
    /* GUI_KEY_4     */ { L"4",       L"GHI",     L"\x4E3F"   },
    /* GUI_KEY_5     */ { L"5",       L"JKL",     L"\x4E36"   },
    /* GUI_KEY_6     */ { L"6",       L"MNO",     L"\x4E5B"   },
    /* GUI_KEY_7     */ { L"7",       L"PQRS",    L"\xFF1F"   },
    /* GUI_KEY_8     */ { L"8",       L"TUV",     L""         },
    /* GUI_KEY_9     */ { L"9",       L"WXYZ",    L""         },
    /* GUI_KEY_STAR  */ { L"*",       L"",        L""         },
    /* GUI_KEY_POUND */ { L"#",       L"",        L""         },
    /* GUI_KEY_LEFT  */ { L"\x25C4",  L"",        L""         },
    /* GUI_KEY_RIGHT */ { L"\x25BA",  L"",        L""         },
    /* GUI_KEY_UP    */ { L"\x25B2",  L"",        L""         },
    /* GUI_KEY_DOWN  */ { L"\x25BC",  L"",        L""         },
    /* GUI_KEY_OK    */ { L"",        L"",        L""         },
    /* GUI_KEY_LSK   */ { L"",        L"",        L""         },
    /* GUI_KEY_RSK   */ { L"C",       L"",        L""         },

    /* GUI_KEY_10    */ { 0 },
    /* GUI_KEY_11    */ { 0 },
    /* GUI_KEY_12    */ { 0 },
    /* GUI_KEY_13    */ { 0 },
    /* GUI_KEY_14    */ { 0 },
    /* GUI_KEY_15    */ { 0 },
    /* GUI_KEY_16    */ { 0 },
    /* GUI_KEY_17    */ { 0 }

};

static const KEY_DEFINE KBD_LAYOUT_2[GUI_KEY_COUNT] =
{
    /* GUI_KEY_0     */ { L"0",       L"",        L""           },
    /* GUI_KEY_1     */ { L"1",       L"",        L"¨Å¨È¨Ë¨Ú"   },
    /* GUI_KEY_2     */ { L"2",       L"ABC",     L"¨Í¨Ð¨Þ¨ç"   },
    /* GUI_KEY_3     */ { L"3",       L"DEF",     L"¨Ó¨×¨â¨æ"   },
    /* GUI_KEY_4     */ { L"4",       L"GHI",     L"¨Æ¨É¨Ì¨Û"   },
    /* GUI_KEY_5     */ { L"5",       L"JKL",     L"¨Î¨Ñ¨ß¨è"   },
    /* GUI_KEY_6     */ { L"6",       L"MNO",     L"¨Ô¨Ø¨ã¨ä"   },
    /* GUI_KEY_7     */ { L"7",       L"PQRS",    L"¨Ê¨Ç¨Ü¨Ý"   },
    /* GUI_KEY_8     */ { L"8",       L"TUV",     L"¨Ï¨Ò¨à¨á¨é" },
    /* GUI_KEY_9     */ { L"9",       L"WXYZ",    L"¨Õ¨Ö¨Ù¨å"   },
    /* GUI_KEY_STAR  */ { L"*",       L"",        L""           },
    /* GUI_KEY_POUND */ { L"#",       L"",        L""           },
    /* GUI_KEY_LEFT  */ { L"\x25C4",  L"",        L""           },
    /* GUI_KEY_RIGHT */ { L"\x25BA",  L"",        L""           },
    /* GUI_KEY_UP    */ { L"\x25B2",  L"",        L""           },
    /* GUI_KEY_DOWN  */ { L"\x25BC",  L"",        L""           },
    /* GUI_KEY_OK    */ { L"",        L"",        L""           },
    /* GUI_KEY_LSK   */ { L"",        L"",        L""           },
    /* GUI_KEY_RSK   */ { L"C",       L"",        L""           },

    /* GUI_KEY_10    */ { 0 },
    /* GUI_KEY_11    */ { 0 },
    /* GUI_KEY_12    */ { 0 },
    /* GUI_KEY_13    */ { 0 },
    /* GUI_KEY_14    */ { 0 },
    /* GUI_KEY_15    */ { 0 },
    /* GUI_KEY_16    */ { 0 },
    /* GUI_KEY_17    */ { 0 }
};

static const KEY_DEFINE KBD_LAYOUT_3[GUI_KEY_COUNT] =
{
    /* GUI_KEY_0     */ { L"0",       L"",        L"¨ç¨è¨é"     },
    /* GUI_KEY_1     */ { L"1",       L"",        L"¨Å¨Æ¨Ç¨È"   },
    /* GUI_KEY_2     */ { L"2",       L"ABC",     L"¨É¨Ê¨Ë¨Ì"   },
    /* GUI_KEY_3     */ { L"3",       L"DEF",     L"¨Í¨Î¨Ï"     },
    /* GUI_KEY_4     */ { L"4",       L"GHI",     L"¨Ð¨Ñ¨Ò"     },
    /* GUI_KEY_5     */ { L"5",       L"JKL",     L"¨Ó¨Ô¨Õ¨Ö"   },
    /* GUI_KEY_6     */ { L"6",       L"MNO",     L"¨×¨Ø¨Ù"     },
    /* GUI_KEY_7     */ { L"7",       L"PQRS",    L"¨Ú¨Û¨Ü¨Ý"   },
    /* GUI_KEY_8     */ { L"8",       L"TUV",     L"¨Þ¨ß¨à¨á"   },
    /* GUI_KEY_9     */ { L"9",       L"WXYZ",    L"¨â¨ã¨ä¨å¨æ" },
    /* GUI_KEY_STAR  */ { L"*",       L"",        L"\x02C9\x02CA\x02C7\x02CB\x02D9" },
    /* GUI_KEY_POUND */ { L"#",       L"",        L""           },
    /* GUI_KEY_LEFT  */ { L"\x25C4",  L"",        L""           },
    /* GUI_KEY_RIGHT */ { L"\x25BA",  L"",        L""           },
    /* GUI_KEY_UP    */ { L"\x25B2",  L"",        L""           },
    /* GUI_KEY_DOWN  */ { L"\x25BC",  L"",        L""           },
    /* GUI_KEY_OK    */ { L"",        L"",        L""           },
    /* GUI_KEY_LSK   */ { L"",        L"",        L""           },
    /* GUI_KEY_RSK   */ { L"C",       L"",        L""           },

    /* GUI_KEY_10    */ { 0 },
    /* GUI_KEY_11    */ { 0 },
    /* GUI_KEY_12    */ { 0 },
    /* GUI_KEY_13    */ { 0 },
    /* GUI_KEY_14    */ { 0 },
    /* GUI_KEY_15    */ { 0 },
    /* GUI_KEY_16    */ { 0 },
    /* GUI_KEY_17    */ { 0 }
};

static const KEY_DEFINE KBD_LAYOUT_4[GUI_KEY_COUNT] =
{
    /* GUI_KEY_0     */ { L"0",       L"\x2423",        L""         },
    /* GUI_KEY_1     */ { L"1",       L"ER",      L"\x4E00"   },
    /* GUI_KEY_2     */ { L"2",       L"TY",      L"\x4E28"   },
    /* GUI_KEY_3     */ { L"3",       L"UI",      L"\x4E3F"   },
    /* GUI_KEY_4     */ { L"4",       L"DF",      L"\x4E36"   },
    /* GUI_KEY_5     */ { L"5",       L"GH",      L"\x4E5B"   },
    /* GUI_KEY_6     */ { L"6",       L"JK",      L"\xFF1F"   },
    /* GUI_KEY_7     */ { L"7",       L"CV",      L""         },
    /* GUI_KEY_8     */ { L"8",       L"BN",      L""         },
    /* GUI_KEY_9     */ { L"9",       L"M",       L""         },
    /* GUI_KEY_STAR  */ { L"*",       L"",        L""         },
    /* GUI_KEY_POUND */ { L"#",       L"",        L""         },
    /* GUI_KEY_LEFT  */ { L"\x25C4",  L"",        L""         },
    /* GUI_KEY_RIGHT */ { L"\x25BA",  L"",        L""         },
    /* GUI_KEY_UP    */ { L"\x25B2",  L"",        L""         },
    /* GUI_KEY_DOWN  */ { L"\x25BC",  L"",        L""         },
    /* GUI_KEY_OK    */ { L"",        L"",        L""         },
    /* GUI_KEY_LSK   */ { L"",        L"",        L""         },
    /* GUI_KEY_RSK   */ { L"C",       L"",        L""         },

    /* GUI_KEY_10    */ { L"!",       L"QW",      L""         },
    /* GUI_KEY_11    */ { L"?",       L"AS",      L""         },
    /* GUI_KEY_12    */ { L"@",       L"ZX",      L""         },
    /* GUI_KEY_13    */ { L"-",       L"OP",      L""         },
    /* GUI_KEY_14    */ { L"'",       L"L",       L""         },
    /* GUI_KEY_15    */ { L":;",      L".,",      L""         },
    /* GUI_KEY_16    */ { L"\x21E7",  L"",        L""         },
    /* GUI_KEY_17    */ { L"\x21B5",  L"",        L""         }
};

static const RECT key_pos_digit[GUI_KEY_COUNT] =
{
    /* GUI_KEY_0     */ {120, 590, 200, 624}, 
    /* GUI_KEY_1     */ { 34, 476, 114, 510}, 
    /* GUI_KEY_2     */ {120, 476, 200, 510}, 
    /* GUI_KEY_3     */ {206, 476, 286, 510}, 
    /* GUI_KEY_4     */ { 34, 514, 114, 548}, 
    /* GUI_KEY_5     */ {120, 514, 200, 548}, 
    /* GUI_KEY_6     */ {206, 514, 286, 548}, 
    /* GUI_KEY_7     */ { 34, 552, 114, 586}, 
    /* GUI_KEY_8     */ {120, 552, 200, 586}, 
    /* GUI_KEY_9     */ {206, 552, 286, 586}, 
    /* GUI_KEY_STAR  */ { 34, 590, 114, 624}, 
    /* GUI_KEY_POUND */ {206, 590, 286, 624}, 
    /* GUI_KEY_LEFT  */ { 94, 394, 134, 428}, 
    /* GUI_KEY_RIGHT */ {186, 394, 226, 428}, 
    /* GUI_KEY_UP    */ {140, 356, 180, 390}, 
    /* GUI_KEY_DOWN  */ {140, 432, 180, 466}, 
    /* GUI_KEY_OK    */ {140, 394, 180, 428}, 
    /* GUI_KEY_LSK   */ { 74, 356, 134, 390}, 
    /* GUI_KEY_RSK   */ {186, 356, 246, 390},  

    /* GUI_KEY_10    */ { 0 }, 
    /* GUI_KEY_11    */ { 0 }, 
    /* GUI_KEY_12    */ { 0 }, 
    /* GUI_KEY_13    */ { 0 }, 
    /* GUI_KEY_14    */ { 0 }, 
    /* GUI_KEY_15    */ { 0 }, 
    /* GUI_KEY_16    */ { 0 }, 
    /* GUI_KEY_17    */ { 0 } 
};

static const RECT key_pos_suretype[GUI_KEY_COUNT] = 
{
    /* GUI_KEY_0     */ {128, 590, 191, 624}, 
    /* GUI_KEY_1     */ { 64, 476, 127, 510}, 
    /* GUI_KEY_2     */ {128, 476, 191, 510}, 
    /* GUI_KEY_3     */ {192, 476, 255, 510}, 
    /* GUI_KEY_4     */ { 64, 514, 127, 548}, 
    /* GUI_KEY_5     */ {128, 514, 191, 548}, 
    /* GUI_KEY_6     */ {192, 514, 255, 548}, 
    /* GUI_KEY_7     */ { 64, 552, 127, 586}, 
    /* GUI_KEY_8     */ {128, 552, 191, 586}, 
    /* GUI_KEY_9     */ {192, 552, 255, 586}, 
    /* GUI_KEY_STAR  */ { 64, 590, 127, 624}, 
    /* GUI_KEY_POUND */ {192, 590, 255, 624}, 

    /* GUI_KEY_LEFT  */ { 94, 394, 134, 428}, 
    /* GUI_KEY_RIGHT */ {186, 394, 226, 428}, 
    /* GUI_KEY_UP    */ {140, 356, 180, 390}, 
    /* GUI_KEY_DOWN  */ {140, 432, 180, 466}, 
    /* GUI_KEY_OK    */ {140, 394, 180, 428}, 
    /* GUI_KEY_LSK   */ { 74, 356, 134, 390}, 
    /* GUI_KEY_RSK   */ {186, 356, 246, 390},  

    /* GUI_KEY_10    */ {0,   476, 63,  510}, 
    /* GUI_KEY_11    */ {0,   514, 63,  548}, 
    /* GUI_KEY_12    */ {0,   552, 63,  586}, 
    /* GUI_KEY_13    */ {256, 476, 319, 510}, 
    /* GUI_KEY_14    */ {256, 514, 319, 548}, 
    /* GUI_KEY_15    */ {256, 552, 319, 586}, 
    /* GUI_KEY_16    */ {0,   590, 63,  624}, 
    /* GUI_KEY_17    */ {256, 590, 319, 624} 
};

static int keyboard_type = 0;
static const RECT *KEY_POS;

static int pressed = -1;
static const KEY_DEFINE* keys;

static HFONT label1_font = 0;
static HFONT label2_font = 0;
static HFONT label3_font = 0;

static COLORREF key_text_color = RGB(0, 0, 0);

extern int ilME_initialized;
extern int sms_input_method;

static const KEY_DEFINE* kbd_get_current_layout(void)
{
    KEY_POS = key_pos_digit;
    if ( ilME_initialized && (sms_input_method > IM_SURETYPE_BASE && sms_input_method < IM_USER_MULTITYPE_BASE) )    {
        KEY_POS = key_pos_suretype;
        return KBD_LAYOUT_4;
    }
    else if ( ilME_initialized && (sms_input_method == IM_ZHUYIN) )
        return KBD_LAYOUT_2;
    else if ( ilME_initialized && (sms_input_method == IM_ZHUYIN_MT) )
        return KBD_LAYOUT_3;
    else if (g_stroke_layout == 1)
        return KBD_LAYOUT_0;
    else
        return KBD_LAYOUT_1;
}

void kbd_init(void)
{
    LOGFONT lf;

    memset(&lf, 0, sizeof(lf));
    lf.lfHeight = 26;
    lf.lfWeight = FW_BOLD;
    lstrcpy(lf.lfFaceName, L"Arial Unicode MS");
    label1_font = CreateFontIndirect(&lf);

    memset(&lf, 0, sizeof(lf));
    lf.lfHeight = 14;
    lf.lfWeight = FW_BOLD;
    lstrcpy(lf.lfFaceName, L"Arial Unicode MS");
    label2_font = CreateFontIndirect(&lf);

    memset(&lf, 0, sizeof(lf));
    lf.lfHeight = 14;
    lstrcpy(lf.lfFaceName, L"Arial Unicode MS");
    label3_font = CreateFontIndirect(&lf);

    pressed = -1;

    keys = kbd_get_current_layout();
}

void kbd_cleanup(void)
{
    DeleteObject(label3_font);
    DeleteObject(label2_font);
    DeleteObject(label1_font);
}

void kbd_update_layout(void)
{
    const KEY_DEFINE* new_keys;

    new_keys = kbd_get_current_layout();

    if (keys != new_keys)
    {
        keys = new_keys;
        InvalidateRect(g_main_window, NULL, FALSE);
    }
}

int kbd_hit_test(const POINT* pt)
{
    int i;

    for (i = 0; i < GUI_KEY_COUNT; i++)
    {
        if (PtInRect(KEY_POS + i, *pt))
            return i;
    }

    return -1;
}

void kbd_show_pressed_key(int key)
{
    if (pressed != key)
    {
        if (pressed >= 0)
            InvalidateRect(g_main_window, KEY_POS + pressed, FALSE);

        pressed = key;

        if (pressed >= 0)
            InvalidateRect(g_main_window, KEY_POS + pressed, FALSE);

        UpdateWindow(g_main_window);
    }
}

static void paint_button(
    HDC hdc,
    const RECT* pos,
    const KEY_DEFINE* key,
    BOOL is_pressed)
{
    RECT r;
    HBRUSH hBrush;
    HFONT hSaveFont;
    int x, y, w;
    SIZE s1, s2, s3;

    r = *pos;

    /* paint key face */

    hBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
    FillRect(hdc, &r, hBrush);
    DeleteObject(hBrush);

    /* paint key edge */

    if (is_pressed)
        DrawEdge(hdc, &r, BDR_SUNKENOUTER, BF_RECT);
    else
        DrawEdge(hdc, &r, EDGE_RAISED, BF_RECT);

    /* paint key text */

    if (is_pressed)
        OffsetRect(&r, 1, 1);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, key_text_color);

    if ( (lstrlen(key->label2) == 0) &&
         (lstrlen(key->label3) == 0) )
    {
        hSaveFont = (HFONT)SelectObject(hdc, label1_font);

        GetTextExtentPoint32(hdc, key->label1, lstrlen(key->label1), &s1);
        x = r.left + (r.right - r.left - s1.cx) / 2;
        y = r.top + (r.bottom - r.top - s1.cy) / 2;
        SetTextAlign(hdc, TA_LEFT | TA_TOP);
        TextOut(hdc, x, y, key->label1, lstrlen(key->label1));

        SelectObject(hdc, hSaveFont);
    }
    else
    {
        hSaveFont = (HFONT)SelectObject(hdc, label1_font);
        GetTextExtentPoint32(hdc, key->label1, lstrlen(key->label1), &s1);

        SelectObject(hdc, label2_font);
        GetTextExtentPoint32(hdc, key->label2, lstrlen(key->label2), &s2);

        SelectObject(hdc, label3_font);
        GetTextExtentPoint32(hdc, key->label3, lstrlen(key->label3), &s3);

        if (s2.cx >= s3.cx)
            w = s1.cx + s2.cx + 4;
        else
            w = s1.cx + s3.cx + 4;

        SelectObject(hdc, label1_font);
        x = r.left + (r.right - r.left - w) / 2;
        y = r.top + (r.bottom - r.top - s1.cy) / 2;
        SetTextAlign(hdc, TA_LEFT | TA_TOP);
        TextOut(hdc, x, y, key->label1, lstrlen(key->label1));

        SelectObject(hdc, label2_font);
        x += s1.cx + 4;
        y = (r.top + r.bottom) / 2;
        SetTextAlign(hdc, TA_LEFT | TA_TOP);
        TextOut(hdc, x, y, key->label2, lstrlen(key->label2));

        SelectObject(hdc, label3_font);
        SetTextAlign(hdc, TA_LEFT | TA_BOTTOM);
        TextOut(hdc, x, y, key->label3, lstrlen(key->label3));

        SelectObject(hdc, hSaveFont);
    }
}

void kbd_paint(HDC hdc)
{
    int i;

    for (i = 0; i < GUI_KEY_COUNT; i++)
    {
        if (i == pressed)
            paint_button(hdc, KEY_POS+i, keys+i, TRUE);
        else
            paint_button(hdc, KEY_POS+i, keys+i, FALSE);
    }
}
