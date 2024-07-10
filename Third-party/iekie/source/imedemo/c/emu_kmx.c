
/*
 * emu_l,x.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "emu_kmx.h"
#include "resource.h"
#include "userdata.h"
#include "IIMEk2api.h"


static void on_init_dialog(HWND hwnd)
{
}


static BOOL CALLBACK emu_kmx_dialog_proc(
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
            }
            break;
    }

    return FALSE;
}

void execute_emu_kmx_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_EMU_KMX_DIALOG), parent, emu_kmx_dialog_proc);
}

