
/*
 * about.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "about.h"
#include "resource.h"

static BOOL CALLBACK about_dialog_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}

void execute_about_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_ABOUT_DIALOG), parent, about_dialog_proc);
}
