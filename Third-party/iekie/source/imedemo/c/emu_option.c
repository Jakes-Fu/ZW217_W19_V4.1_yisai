
/*
 * emu_option.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "emu_option.h"
#include "resource.h"
#include "userdata.h"
#include "IIMEk2api.h"


int g_stroke_layout = 1;
int g_active_state = kmx_en_actived;
int g_pinyin_mode = 1;
int g_candidate_mode = 1;
int g_marscript = 1;
int g_fullpinyin_mode = 1;
int g_magicone_mode = 0;
int g_zhuyin_as_symbol = 0;

static void on_init_dialog(HWND hwnd)
{
    if (g_stroke_layout == 1)
        CheckRadioButton(hwnd, ID_ENABLE_STROKE_LAYOUT1, ID_ENABLE_STROKE_LAYOUT2, ID_ENABLE_STROKE_LAYOUT1);
    else
        CheckRadioButton(hwnd, ID_ENABLE_STROKE_LAYOUT1, ID_ENABLE_STROKE_LAYOUT2, ID_ENABLE_STROKE_LAYOUT2);

    if (g_active_state == kmx_en_try)
        CheckRadioButton(hwnd, ID_STATE_TRY, ID_STATE_ACTIVE, ID_STATE_TRY);
    else if (g_active_state == kmx_en_notice)
        CheckRadioButton(hwnd, ID_STATE_TRY, ID_STATE_ACTIVE, ID_STATE_NOTICE);
    else if (g_active_state == kmx_en_actived)
        CheckRadioButton(hwnd, ID_STATE_TRY, ID_STATE_ACTIVE, ID_STATE_ACTIVE);

    if (g_pinyin_mode == 0)
        CheckRadioButton(hwnd, ID_STATE_MODE1, ID_STATE_MODE5, ID_STATE_MODE1);
    else if (g_pinyin_mode == 1)
        CheckRadioButton(hwnd, ID_STATE_MODE1, ID_STATE_MODE5, ID_STATE_MODE2);
    else if (g_pinyin_mode == 2)
        CheckRadioButton(hwnd, ID_STATE_MODE1, ID_STATE_MODE5, ID_STATE_MODE3);
    else if (g_pinyin_mode == 3)
        CheckRadioButton(hwnd, ID_STATE_MODE1, ID_STATE_MODE5, ID_STATE_MODE4);
    else if (g_pinyin_mode == 4)
        CheckRadioButton(hwnd, ID_STATE_MODE1, ID_STATE_MODE5, ID_STATE_MODE5);

    if (g_candidate_mode == 1)
        CheckRadioButton(hwnd, ID_STATE_CANDIDATE1, ID_STATE_CANDIDATE2, ID_STATE_CANDIDATE1);
    else if (g_candidate_mode == 2)
        CheckRadioButton(hwnd, ID_STATE_CANDIDATE1, ID_STATE_CANDIDATE2, ID_STATE_CANDIDATE2);

    if (g_marscript == 1)
        CheckRadioButton(hwnd, ID_STATE_MARS_ON, ID_STATE_MARS_OFF, ID_STATE_MARS_ON);
    else
        CheckRadioButton(hwnd, ID_STATE_MARS_ON, ID_STATE_MARS_OFF, ID_STATE_MARS_OFF);

    if (g_fullpinyin_mode == 1)
        CheckRadioButton(hwnd, ID_STATE_FULLPINYIN_MODE1, ID_STATE_FULLPINYIN_MODE2, ID_STATE_FULLPINYIN_MODE1);
    else
        CheckRadioButton(hwnd, ID_STATE_FULLPINYIN_MODE1, ID_STATE_FULLPINYIN_MODE2, ID_STATE_FULLPINYIN_MODE2);

    if (g_magicone_mode == 0)
        CheckRadioButton(hwnd, ID_STATE_MAGICONE_DEFAULT, ID_STATE_MAGICONE_STAR, ID_STATE_MAGICONE_DEFAULT);
    else
        CheckRadioButton(hwnd, ID_STATE_MAGICONE_DEFAULT, ID_STATE_MAGICONE_STAR, ID_STATE_MAGICONE_STAR);

    if (g_zhuyin_as_symbol == 0)
        CheckRadioButton(hwnd, ID_STATE_ZHUYINASSYMBOL_OFF, ID_STATE_ZHUYINASSYMBOL_ON, ID_STATE_ZHUYINASSYMBOL_OFF);
    else
        CheckRadioButton(hwnd, ID_STATE_ZHUYINASSYMBOL_OFF, ID_STATE_ZHUYINASSYMBOL_ON, ID_STATE_ZHUYINASSYMBOL_ON);
}

static void on_ok(HWND hwnd)
{
    CS_IMM_OPTIONS* opt;

    opt = (CS_IMM_OPTIONS*)imeoptiondata;

    if (IsDlgButtonChecked(hwnd, ID_ENABLE_STROKE_LAYOUT1) == BST_CHECKED)
    {
        g_stroke_layout = 1;
        opt->dwStrokeLayout = CS_STROKE_STANDARD_LAYOUT;
    }
    else
    {
        g_stroke_layout = 2;
        opt->dwStrokeLayout = CS_STROKE_CSTAR_LAYOUT_ONE;
    }

    if (IsDlgButtonChecked(hwnd, ID_STATE_TRY) == BST_CHECKED)
        g_active_state = kmx_en_try;
    else if (IsDlgButtonChecked(hwnd, ID_STATE_NOTICE) == BST_CHECKED)
        g_active_state = kmx_en_notice;
    else if (IsDlgButtonChecked(hwnd, ID_STATE_ACTIVE) == BST_CHECKED)
        g_active_state = kmx_en_actived;

    if (IsDlgButtonChecked(hwnd, ID_STATE_MODE1) == BST_CHECKED)
    {
        g_pinyin_mode = 0;
        opt->dwPinyinMode = CS_PINYIN_MODE_1;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_MODE2) == BST_CHECKED)
    {
        g_pinyin_mode = 1;
        opt->dwPinyinMode = CS_PINYIN_MODE_2;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_MODE3) == BST_CHECKED)
    {
        g_pinyin_mode = 2;
        opt->dwPinyinMode = CS_PINYIN_MODE_3;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_MODE4) == BST_CHECKED)
    {
        g_pinyin_mode = 3;
        opt->dwPinyinMode = CS_PINYIN_MODE_4;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_MODE5) == BST_CHECKED)
    {
        g_pinyin_mode = 4;
        opt->dwPinyinMode = CS_PINYIN_MODE_5;
    }

    if (IsDlgButtonChecked(hwnd, ID_STATE_CANDIDATE1) == BST_CHECKED)
    {
        g_candidate_mode = 1;
        opt->dwCandidateMode = CS_CANDIDATE_MODE_1;
    }
    else if(IsDlgButtonChecked(hwnd, ID_STATE_CANDIDATE2) == BST_CHECKED)
    {
        g_candidate_mode = 2;
        opt->dwCandidateMode = CS_CANDIDATE_MODE_2;
    }

    if (IsDlgButtonChecked(hwnd, ID_STATE_MARS_ON) == BST_CHECKED)
    {
        g_marscript = 1;
        opt->dwMarScript = 1;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_MARS_OFF) == BST_CHECKED)
    {
        g_marscript = 0;
        opt->dwMarScript = 0;
    }

    if (IsDlgButtonChecked(hwnd, ID_STATE_FULLPINYIN_MODE1) == BST_CHECKED)
    {
        g_fullpinyin_mode = 1;
        opt->dwFullkeyPinyinMode = 1;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_FULLPINYIN_MODE2) == BST_CHECKED)
    {
        g_fullpinyin_mode = 2;
        opt->dwFullkeyPinyinMode = 2;
    }

    if (IsDlgButtonChecked(hwnd, ID_STATE_MAGICONE_DEFAULT) == BST_CHECKED)
    {
        g_magicone_mode = 0;
        opt->dwMagicOne = CS_MAGICONE_DEFAULT;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_MAGICONE_STAR) == BST_CHECKED)
    {
        g_magicone_mode = 1;
        opt->dwMagicOne = CS_MAGICONE_STAR;
    }

    if (IsDlgButtonChecked(hwnd, ID_STATE_ZHUYINASSYMBOL_OFF) == BST_CHECKED)
    {
        g_zhuyin_as_symbol = 0;
        opt->dwZhuyinSymbolAsCand = CS_ZHUYIN_SYMBOL_AS_CAND_OFF;
    }
    else if (IsDlgButtonChecked(hwnd, ID_STATE_ZHUYINASSYMBOL_ON) == BST_CHECKED)
    {
        g_zhuyin_as_symbol = 1;
        opt->dwZhuyinSymbolAsCand = CS_ZHUYIN_SYMBOL_AS_CAND_ON;
    }

    EndDialog(hwnd, 0);

    userdata_save();

    emulator_reset();
}

static BOOL CALLBACK emu_option_dialog_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            on_init_dialog(hwnd);
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    on_ok(hwnd);
                    return TRUE;

                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}

void execute_emu_option_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_EMU_OPTION_DIALOG), parent, emu_option_dialog_proc);
}
