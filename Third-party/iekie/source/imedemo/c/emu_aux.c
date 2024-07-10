
/*
 * emu_aux.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "emu_aux.h"
#include "resource.h"
#include "userdata.h"
#include "csk2api.h"

#if !defined(INTERFACE_08_ONLY)

static void on_init_dialog(HWND hwnd)
{
}

static void on_query_pinyin(HWND hwnd)
{
#if defined(CS_PINYIN) || defined(CS_PINYIN_MINI)
    TCHAR wszInput[2];
    unsigned short wszPinyin[8] = L"\0";

    UINT ret = GetDlgItemText(hwnd, IDC_EDIT_AUX_INPUT, wszInput, 2);
    if (ret)
    {
        CS_IMM_QueryCodePinyin(wszInput[0], wszPinyin);
    }

    SetDlgItemText(hwnd, IDC_EDIT_AUX_OUTPUT, wszPinyin);
#endif
}

static void on_query_zhuyin(HWND hwnd)
{
#if defined(CS_ZHUYIN) || defined(CS_ZHUYIN_MINI)
    TCHAR wszInput[2];
    unsigned short wszPinyin[8] = L"\0";

    UINT ret = GetDlgItemText(hwnd, IDC_EDIT_AUX_INPUT, wszInput, 2);
    if (ret)
    {
        CS_IMM_QueryCodeZhuyin(wszInput[0], wszPinyin);
    }

    SetDlgItemText(hwnd, IDC_EDIT_AUX_OUTPUT, wszPinyin);
#endif
}

static void on_query_asso_sc(HWND hwnd)
{
#if defined(CS_PINYIN) || defined(CS_PINYIN_MINI)
    TCHAR wszInput[2];
    unsigned short wszAsso[68] = L"\0";
    int nAsso;

    UINT ret = GetDlgItemText(hwnd, IDC_EDIT_AUX_INPUT, wszInput, 2);
    if (ret)
    {
        nAsso = 64;
        HanziAsso_QueryAsso(1, wszInput[0], wszAsso, &nAsso);
        wszAsso[nAsso] = 0;
    }

    SetDlgItemText(hwnd, IDC_EDIT_AUX_OUTPUT, wszAsso);
#endif
}

static void on_query_asso_tc(HWND hwnd)
{
#if defined(CS_ZHUYIN) || defined(CS_ZHUYIN_MINI) 
    TCHAR wszInput[2];
    unsigned short wszAsso[68] = L"\0";
    int nAsso;

    UINT ret = GetDlgItemText(hwnd, IDC_EDIT_AUX_INPUT, wszInput, 2);
    if (ret)
    {
        nAsso = 64;
        HanziAsso_QueryAsso(0, wszInput[0], wszAsso, &nAsso);
        wszAsso[nAsso] = 0;
    }

    SetDlgItemText(hwnd, IDC_EDIT_AUX_OUTPUT, wszAsso);
#endif
}


static BOOL CALLBACK emu_aux_dialog_proc(
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
                    EndDialog(hwnd, 0);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;
                case IDC_BTN_QUERY_PY:
                    on_query_pinyin(hwnd);
                    return TRUE;
                case IDC_BTN_QUERY_ZY:
                    on_query_zhuyin(hwnd);
                    return TRUE;
                case IDC_BTN_QUERY_ASSO_SC:
                    on_query_asso_sc(hwnd);
                    return TRUE;
                case IDC_BTN_QUERY_ASSO_TC:
                    on_query_asso_tc(hwnd);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}

void execute_emu_aux_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_EMU_AUX_DIALOG), parent, emu_aux_dialog_proc);
}

#endif /* !defined(INTERFACE_08_ONLY) */