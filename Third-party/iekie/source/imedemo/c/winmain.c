
/*
 * winmain.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "imedemo.h"
#include "kbd.h"
#include "gui.h"
#include "gui_imp.h"
#include "guidebug.h"
#include "guizoom.h"

#include "about.h"
#include "emu_option.h"
#include "emu_aux.h"
#include "emu_kmx.h"
#include "kmx_ime.h"

#include "userdata.h"
#include "update_data.h"

#include "resource.h"

HINSTANCE g_instance = 0;
HWND g_main_window = 0;

#define CX_WND 320
#define CY_WND 632

#define KEY_REPEAT_DELAY        600
#define KEY_REPEAT_PERIOD       90
#define KEY_REPEAT_TIMER_ID     100

static const WCHAR MAIN_WND_CLASS[] = L"imedemo";
static const WCHAR MAIN_WND_TITLE[] = L"ÊäÈë·¨ÑÝÊ¾";

void emulator_init(void)
{
    kbd_init();
    gui_init();
}

void emulator_cleanup(void)
{
    gui_cleanup();
    kbd_cleanup();
}

void emulator_reset(void)
{
    emulator_cleanup();
    emulator_init();
    InvalidateRect(g_main_window, NULL, FALSE);
}

static void init_window_pos(HWND hwnd)
/*
 * adjust window size, move to the center of screen
 */
{
    int x, y, cx, cy;
    RECT r1, r2, r3;

    GetWindowRect(hwnd, &r1);
    GetClientRect(hwnd, &r2);
    SystemParametersInfo(SPI_GETWORKAREA, 0, &r3, 0);

    cx = (r1.right - r1.left) - (r2.right - r2.left) + CX_WND;
    cy = (r1.bottom - r1.top) - (r2.bottom - r2.top) + CY_WND;
    x = ((r3.right - r3.left) - cx) / 2;
    y = ((r3.bottom - r3.top) - cy) / 2;

    MoveWindow(hwnd, x, y, cx, cy, FALSE);
}

static void on_create_window(HWND hwnd)
{
    g_main_window = hwnd;
    init_window_pos(hwnd);

    userdata_init();
    emulator_init();
}

static void on_close_window(HWND hwnd)
{
    userdata_save();
    emulator_cleanup();
    DestroyWindow(hwnd);
}

static void on_paint_window(HWND hwnd)
{
    RECT r;
    HDC hdc;
    HDC mem_hdc;
    HBRUSH brush;
    PAINTSTRUCT ps;
    HBITMAP lcd_bmp;
    HBITMAP save_bmp;

    hdc = BeginPaint(hwnd, &ps);

    mem_hdc = CreateCompatibleDC(hdc);
    lcd_bmp = CreateCompatibleBitmap(hdc, CX_WND, CY_WND);
    save_bmp = (HBITMAP)SelectObject(mem_hdc, lcd_bmp);

    brush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
    GetClientRect(hwnd, &r);
    FillRect(mem_hdc, &r, brush);
    DeleteObject(brush);

    kbd_paint(mem_hdc);
    gui_paint(mem_hdc);

    BitBlt(hdc, 0, 0, CX_WND, CY_WND, mem_hdc, 0, 0, SRCCOPY);

    SelectObject(mem_hdc, save_bmp);
    DeleteObject(lcd_bmp);
    DeleteDC(mem_hdc);

    EndPaint(hwnd, &ps);
}

void on_char(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    gui_handle_char(wParam);
}

void on_handle_key(int msg, int key)
{
    gui_handle_key(msg, key, GetTickCount());
}

void on_timer(int event_id)
{
    gui_handle_timer(event_id);
}

void on_button_down(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    MSG msg;
    POINT pt;
    int key;
    BOOL pressed;
    BOOL processed;
    BOOL end_msg_loop;
    BOOL timer_actived;
    BOOL repeat_actived;

    if (GetCapture() != NULL)
        return;

    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    key = kbd_hit_test(&pt);
    pressed = TRUE;
    kbd_show_pressed_key(key);
    on_handle_key(GUI_MSG_KEYDOWN, key);

    timer_actived = FALSE;
    repeat_actived = FALSE;
    SetTimer(hwnd, KEY_REPEAT_TIMER_ID, KEY_REPEAT_DELAY, NULL);
    timer_actived = TRUE;

    SetCapture(hwnd);
    end_msg_loop = FALSE;
    while (!end_msg_loop)
    {
        if (GetCapture() != hwnd)
            break;
            
        GetMessage(&msg, NULL, 0, 0);
        processed = FALSE;

        switch (msg.message)
        {
            case WM_TIMER:
                if (msg.wParam == KEY_REPEAT_TIMER_ID)
                {
                    processed = TRUE;
                    if (pressed && timer_actived)
                    {
                        if (!repeat_actived)
                        {
                            repeat_actived = TRUE;
                            SetTimer(hwnd, KEY_REPEAT_TIMER_ID, KEY_REPEAT_PERIOD, NULL);
                        }
                        on_handle_key(GUI_MSG_KEYREPEAT, key);
                    }
                }
                break;

            case WM_CAPTURECHANGED:
                if ( ((HWND)(msg.lParam) != hwnd) || (GetCapture() != hwnd) )
                    end_msg_loop = TRUE;
                processed = TRUE;
                break;

            case WM_LBUTTONUP:
                end_msg_loop = TRUE;

            case WM_MOUSEMOVE:
                processed = TRUE;
                GetCursorPos(&pt);
                ScreenToClient(hwnd, &pt);
                if (kbd_hit_test(&pt) == key)
                {
                    if (!pressed)
                    {
                        pressed = TRUE;
                        kbd_show_pressed_key(key);
                        SetTimer(hwnd, KEY_REPEAT_TIMER_ID, KEY_REPEAT_DELAY, NULL);
                        timer_actived = TRUE;
                        on_handle_key(GUI_MSG_KEYDOWN, key);
                    }
                }
                else
                {
                    if (pressed)
                    {
                        pressed = FALSE;
                        kbd_show_pressed_key(-1);
                        if (timer_actived)
                        {
                            KillTimer(hwnd, KEY_REPEAT_TIMER_ID);
                            timer_actived = FALSE;
                        }
                        on_handle_key(GUI_MSG_KEYUP, key);
                    }
                }
                break;
                    
            case WM_RBUTTONDOWN:
                processed = TRUE;
                break;
        }

        if (!processed)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (GetCapture() == hwnd)
        ReleaseCapture();

    if (timer_actived)
        KillTimer(hwnd, KEY_REPEAT_TIMER_ID);

    if (pressed)
    {
        kbd_show_pressed_key(-1);
        on_handle_key(GUI_MSG_KEYUP, key);
    }
}

static LRESULT CALLBACK main_window_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
            on_create_window(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CLOSE:
            on_close_window(hwnd);
            return 0;

        case WM_PAINT:
            on_paint_window(hwnd);
            return 0;

        case WM_LBUTTONDOWN:
            on_button_down(hwnd, wParam, lParam);
            return 0;

        case WM_TIMER:
            on_timer(wParam);
            return 0;

        case WM_CHAR:
            on_char(hwnd, wParam, lParam);
            return 0;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case ID_CMD_EXIT:
                    on_close_window(hwnd);
                    return 0;

                case ID_CMD_EMU_RESET:
                    emulator_reset();
                    return 0;

                case ID_CMD_EMU_CLEAR_USERDATA:
                    memset(userdata, 0, userdata_size);
                    userdata_save();
                    emulator_reset();
                    return 0;

                case ID_CMD_EMU_OPTIONS:
                    execute_emu_option_dialog(hwnd);
                    return 0;

                case ID_CMD_LOAD_UPDATE:
                    exec_load_update_data(hwnd);
                    return 0;
#if !defined(INTERFACE_08_ONLY)
                case ID_CMD_EMU_AUX:
                    execute_emu_aux_dialog(hwnd);
                    return 0;
#endif
                case ID_CMD_EMU_KMX:
                    execute_emu_kmx_dialog(hwnd);
                    return 0;

                case ID_CMD_PINYIN_TEST:
                    execute_pinyin_test_dialog(hwnd);
                    return 0;

                case ID_CMD_ZHUYIN_TEST:
                    execute_zhuyin_test_dialog(hwnd);
                    return 0;

                case ID_CMD_LATIN_TEST:
                    execute_latin_test_dialog(hwnd);
                    return 0;

                case ID_CMD_STROKE_TEST:
                    execute_stroke_test_dialog(hwnd);
                    return 0;

                case ID_CMD_ABOUT:
                    execute_about_dialog(hwnd);
                    return 0;

                case ID_CMD_SWITCH_GUI_DEBUG_WINDOW:
                    if (g_gui_debug)
                        CloseGuiDebugWindow();
                    else
                        ShowGuiDebugWindow();
                    return 0;

                case ID_CMD_SWITCH_GUI_ZOOM_WINDOW:
                    if (g_gui_zoom)
                        CloseGuiZoomWindow();
                    else
                        ShowGuiZoomWindow();
                    return 0;
                default:    /* MMI_LANG_FIRST - MMI_LANG_LAST */
                    if (LOWORD(wParam) >= MMI_LANG_FIRST && LOWORD(wParam) <= MMI_LANG_LAST)
                    {
                        gui_set_default_language(LOWORD(wParam));
                    }
                    return 0;

            }
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR pCmdLine,
    int nShowCmd)
{
    MSG msg;
    WNDCLASS wc;

    g_instance = hInstance;

    /* register main window class */

    memset(&wc, 0, sizeof(wc));

    wc.lpfnWndProc = main_window_proc;
    wc.hInstance = g_instance;
    wc.hIcon = LoadIcon(g_instance, MAKEINTRESOURCE(ID_APP_ICON));
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32649));
    wc.lpszMenuName = MAKEINTRESOURCE(ID_MAIN_MENU);
    wc.lpszClassName = MAIN_WND_CLASS;

    RegisterClass(&wc);

    /* create main window */

    CreateWindow(
        MAIN_WND_CLASS,
        MAIN_WND_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        300,
        350,
        NULL,
        NULL,
        g_instance,
        NULL);

    /* show main window */

    ShowWindow(g_main_window, nShowCmd);
    UpdateWindow(g_main_window);

    /* enter message loop */

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

//    _CrtDumpMemoryLeaks();

    return msg.wParam;
}

