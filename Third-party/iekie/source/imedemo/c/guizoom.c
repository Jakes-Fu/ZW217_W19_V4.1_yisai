
/*
 * guizoom.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "guizoom.h"
#include "gui.h"
#include "gui_imp.h"
#include "resource.h"

BOOL g_gui_zoom = FALSE;

static HWND g_gui_zoom_window = 0;
static BOOL g_gui_zoom_window_registered = FALSE;

static const WCHAR GUIZOOM_WND_CLASS[] = L"guizoom";
static const WCHAR GUIZOOM_WND_TITLE[] = L"GUI·Å´ó";

static void init_window_pos(HWND hwnd)
{
    int cx, cy;
    RECT r1, r2, r3;

    GetWindowRect(hwnd, &r1);
    GetClientRect(hwnd, &r2);
    GetWindowRect(g_main_window, &r3);

    cx = (r1.right - r1.left) - (r2.right - r2.left) + GUI_WIDTH  * 2;
    cy = (r1.bottom - r1.top) - (r2.bottom - r2.top) + GUI_HEIGHT * 2;

    MoveWindow(hwnd, r3.left - cx, r3.top, cx, cy, FALSE);
}

static void on_create_window(HWND hwnd)
{
    g_gui_zoom_window = hwnd;
    init_window_pos(hwnd);
}

static void on_close_window(void)
{
    HMENU hMenu;

    g_gui_zoom = FALSE;
    DestroyWindow(g_gui_zoom_window);
    g_gui_zoom_window = 0;

    hMenu = GetMenu(g_main_window);
    CheckMenuItem(GetSubMenu(hMenu, 2), ID_CMD_SWITCH_GUI_ZOOM_WINDOW, MF_BYCOMMAND | MF_UNCHECKED);
}

static void on_paint_window(HWND hwnd)
{
    HDC hdc;
    HDC mem_hdc;
    PAINTSTRUCT ps;
    HBITMAP save_bmp;

    hdc = BeginPaint(hwnd, &ps);

    mem_hdc = CreateCompatibleDC(hdc);
    save_bmp = (HBITMAP)SelectObject(mem_hdc, lcd_bitmap);

    StretchBlt(hdc, 0, 0, GUI_WIDTH * 2, GUI_HEIGHT * 2,
        mem_hdc, 0, 0, GUI_WIDTH, GUI_HEIGHT, SRCCOPY);

    SelectObject(mem_hdc, save_bmp);
    DeleteDC(mem_hdc);

    EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK gui_zoom_window_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
            on_create_window(hwnd);
            return 0;

        case WM_CLOSE:
            on_close_window();
            return 0;

        case WM_PAINT:
            on_paint_window(hwnd);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ShowGuiZoomWindow(void)
{
    WNDCLASS wc;
    HMENU hMenu;

    if (g_gui_zoom)
        return;

    /* register window class */

    if (!g_gui_zoom_window_registered)
    {
        memset(&wc, 0, sizeof(wc));

        wc.lpfnWndProc = gui_zoom_window_proc;
        wc.hInstance = g_instance;
        wc.hIcon = LoadIcon(g_instance, MAKEINTRESOURCE(ID_APP_ICON));
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = GUIZOOM_WND_CLASS;

        RegisterClass(&wc);

        g_gui_zoom_window_registered = TRUE;
    }

    /* create window */

    CreateWindow(
        GUIZOOM_WND_CLASS,
        GUIZOOM_WND_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        100,
        100,
        g_main_window,
        NULL,
        g_instance,
        NULL);

    g_gui_zoom = TRUE;
    hMenu = GetMenu(g_main_window);
    CheckMenuItem(GetSubMenu(hMenu, 2), ID_CMD_SWITCH_GUI_ZOOM_WINDOW, MF_BYCOMMAND | MF_CHECKED);

    /* show window */

    ShowWindow(g_gui_zoom_window, SW_SHOW);
    UpdateWindow(g_gui_zoom_window);
}

void CloseGuiZoomWindow(void)
{
    if (g_gui_zoom)
        on_close_window();
}

void UpdateGuiZoomWindow(const RECT* r)
{
    RECT t;

    if (g_gui_zoom)
    {
        t.left = r->left * 2;
        t.top = r->top * 2;
        t.right = r->right * 2;
        t.bottom = r->bottom * 2;
        InvalidateRect(g_gui_zoom_window, &t, FALSE);
    }
}
