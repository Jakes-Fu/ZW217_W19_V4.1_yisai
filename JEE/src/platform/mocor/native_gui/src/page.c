/**
 * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
 * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
 *
 * Copyright 2022-present Unisoc (Shanghai) Technologies Co., Ltd.
 * Licensed under the Unisoc General Software License V1;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 * See the Unisoc General Software License v1 for more details.
 *
 **/

 /**
  *****************************************************************************
  * @file     page.c
  * @brief    -
  * @author   qingjun.yu@unisoc.com
  * @version  V1.0.0
  * @date
  * @history
  * @note
  * detailed description:
  *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2022 unisoc </center></h2>
  **/

#include "sci_types.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "caf.h"
#include "guiprgbox.h"
#include "ctrlslider_export.h"
#include "page.h"
#include "js_event.h"
#include "mmk_kbd.h"
#include "id.h"
#include <stdlib.h>
#include <string.h>

LOCAL void DisplayWinPanelBg(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T win_rect = { 0 };
    GUI_LCD_DEV_INFO lcd_dev_info = { 0 };
    js_native_page_data* data = NULL;
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    data = MMK_GetWinUserData(win_id);
    if (data != NULL) {
        GUI_FillRect(&lcd_dev_info, win_rect, data->uc.bg_color);
    }
}


LOCAL MMI_RESULT_E js_native_gui_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param) {
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if (msg_id >= MSG_KEYDOWN && msg_id <= MSG_KEYREPEAT_MAX_NUM) {
        KeyEvent ke = { 0 };
        int this_val = MMK_GetWinAddDataPtr(win_id);

        ke.base.type = EVENT_KEY;
        ke.code = (msg_id & 0x00FF);
        ke.action = (((msg_id - KEY_PRESSED) & 0xFF00) >> 8);
        js_gui_send_msg(this_val, &ke);

        return result;
    }
    //temp fix.bug2114788
    if( TRUE == js_art_get_force_exit_flag())//check the current js app for errors
    {
        js_art_req_exit();
        js_native_gui_releaseALLId();
        MMK_CloseWin(win_id);
        return result;
    }

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        break;
    case MSG_FULL_PAINT:
    {
        BaseEvent base = { 0 };
        int this_val = MMK_GetWinAddDataPtr(win_id);
        DisplayWinPanelBg(win_id);
        if (0 != this_val) {
            base.type = EVENT_PAGESHOW;
            //js_gui_send_msg(this_val, &base);
        }
        break;
    }
    case MSG_TP_PRESS_DOWN:
    {
        TouchEvent te = { 0 };
        int this_val = MMK_GetWinAddDataPtr(win_id);
        if (0 != this_val)
        {
            te.base.type = EVENT_TOUCHSTART;
            js_gui_send_msg(this_val, &te);
        }
    }
    break;
    case MSG_TP_PRESS_MOVE:
    {
        TouchEvent te = { 0 };
        int this_val = MMK_GetWinAddDataPtr(win_id);
        if (0 != this_val)
        {
            te.base.type = EVENT_TOUCHMOVE;
            js_gui_send_msg(this_val, &te);
        }
    }
    break;
    case MSG_TP_PRESS_UP:
    {
        TouchEvent te = { 0 };
        int this_val = MMK_GetWinAddDataPtr(win_id);
        if (0 != this_val)
        {
            te.base.type = EVENT_TOUCHEND;
            js_gui_send_msg(this_val, &te);
        }
    }
    break;
    //case MSG_NOTIFY_LIST_ITEM_CONTENT_CLICK:
    case MSG_NOTIFY_LIST_ENTER:
    case MSG_NOTIFY_PENOK:
    {
        BaseEvent base = { 0 };
        MMI_NOTIFY_T notify = *(MMI_NOTIFY_T*)param;
        MMI_HANDLE_T ctrlHandle = notify.src_handle;
        int this_val = CTRLBASE_GetUserData(ctrlHandle);
        if (0 != this_val)
        {
            base.type = EVENT_CLICK;
            js_gui_send_msg(this_val, &base);
        }
    }
    break;
    //change to key cancel
    case MSG_NOTIFY_CANCEL:
    {
        KeyEvent ke = { 0 };
        int this_val = MMK_GetWinAddDataPtr(win_id);
        msg_id = MSG_KEYDOWN_CANCEL;

        ke.base.type = EVENT_KEY;
        ke.code = (msg_id & 0x00FF);
        ke.action = 0;
        js_gui_send_msg(this_val, &ke);
    }
    break;
    case MSG_NOTIFY_PENLONGOK:
    {
        BaseEvent base = { 0 };
        MMI_NOTIFY_T notify = *(MMI_NOTIFY_T*)param;
        MMI_HANDLE_T ctrlHandle = notify.src_handle;
        int this_val = CTRLBASE_GetUserData(ctrlHandle);
        if (0 != this_val)
        {
            base.type = EVENT_LONGPRESS;
            js_gui_send_msg(this_val, &base);
        }
    }
    break;
    case MSG_NOTIFY_GET_ACTIVE:
    {
        BaseEvent be = { 0 };
        MMI_NOTIFY_T notify = *(MMI_NOTIFY_T*)param;
        MMI_HANDLE_T ctrlHandle = notify.src_handle;
        int this_val = CTRLBASE_GetUserData(ctrlHandle);
        if (0 != this_val)
        {
            be.type = EVENT_FOCUS;
            js_gui_send_msg(this_val, &be);
        }
    }
    break;
    case MSG_NOTIFY_LOSE_ACTIVE:
    {
        BaseEvent be = { 0 };
        MMI_NOTIFY_T notify = *(MMI_NOTIFY_T*)param;
        MMI_HANDLE_T ctrlHandle = notify.src_handle;
        int this_val = CTRLBASE_GetUserData(ctrlHandle);
        if (0 != this_val)
        {
            be.type = EVENT_BLUR;
            js_gui_send_msg(this_val, &be);
        }
    }
    break;

    case MSG_NOTIFY_PRGBOX_CHANGE:
    {
        GUIPRGBOX_NOTIFY_T notify = { 0 };
        ChangeEvent be = { 0 };
        int this_val = 0;
        SCI_MEMCPY(&notify, param, sizeof(GUIPRGBOX_NOTIFY_T));
        this_val = CTRLBASE_GetUserData(notify.notify.src_handle);
        if (0 != this_val)
        {
            be.value = notify.item_index;;
            be.base.type = EVENT_CHANGE;
            js_gui_send_msg(this_val, &be);
        }
    }
    break;

    case MSG_NOTIFY_SLIDER_CHANGED:
    {
        CTRLSLIDER_NOTIFY_T notify = {0};
        ChangeEvent be = {0};
        int this_val = 0;
        SCI_MEMCPY(&notify, param, sizeof(CTRLSLIDER_NOTIFY_T));
        this_val = CTRLBASE_GetUserData(notify.notify.src_handle);
        if (0 != this_val)
        {
            be.value = notify.cur_position;
            switch (notify.slider_mode)
            {
            case CTRLSLIDER_MODE_START:
                SCI_MEMCPY(be.mode, "start", strlen("start") + 1);
                break;
            case CTRLSLIDER_MODE_MOVE:
                SCI_MEMCPY(be.mode, "move", strlen("move") + 1);
                break;
            case CTRLSLIDER_MODE_END:
                SCI_MEMCPY(be.mode, "end", strlen("end") + 1);
                break;
            case CTRLSLIDER_MODE_VALUE_CHANGED:
                SCI_MEMCPY(be.mode, "value", strlen("value") + 1);
                break;
            default:
                break;
            }
            be.base.type = EVENT_CHANGE;
            js_gui_send_msg(this_val, &be);
        }
    }
    break;
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
    {
        MMI_NOTIFY_T    notify = { 0 };
        ChangeEvent be = { 0 };
        int this_val = 0;
        SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
        this_val = CTRLBASE_GetUserData(notify.src_handle);
        if (0 != this_val) {
            be.value = 0;
            be.base.type = EVENT_CHANGE;
            js_gui_send_msg(this_val, &be);
        }
    }
    break;
    case MSG_TP_PRESS_FLING:
    {
        SwipeEvent te = { 0 };
        MMI_TP_MSG_PARAM_T msg_info = { 0 };
        int this_val = MMK_GetWinAddDataPtr(win_id);
        if (0 != this_val)
        {
            SCI_MEMCPY(&msg_info, param, sizeof(MMI_TP_MSG_PARAM_T));
            te.base.type = EVENT_SWIPE;
            switch (msg_info.tp_fling_direction)
            {
            case TP_FLING_UP:
            {
                SCI_MEMCPY(te.direction, "up", strlen("up") + 1);
                te.distance = abs(msg_info.down_tp_point.y - msg_info.up_tp_point.y);
            }
            break;
            case TP_FLING_DOWN:
            {
                SCI_MEMCPY(te.direction, "down", strlen("down") + 1);
                te.distance = abs(msg_info.down_tp_point.y - msg_info.up_tp_point.y);
            }
            break;
            case TP_FLING_LEFT:
            {
                SCI_MEMCPY(te.direction, "left", strlen("left") + 1);
                te.distance = abs(msg_info.down_tp_point.x - msg_info.up_tp_point.x);
            }
            break;
            case TP_FLING_RIGHT:
            {
                SCI_MEMCPY(te.direction, "right", strlen("right") + 1);
                te.distance = abs(msg_info.down_tp_point.x - msg_info.up_tp_point.x);
            }
            break;
            default:
                break;
            }
            js_gui_send_msg(this_val, &te);
        }
    }
    break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

uint32 js_native_page_create(uint32 this_value, char* type, char* title) {
    MMI_WINDOW_CREATE_T wc = { 0 };
    MMI_HANDLE_T win_handle = NULL;
    GUI_RECT_T rect = { 0 };
    wc.applet_handle = MMK_GetFirstAppletHandle();
    wc.win_id = js_native_gui_lookupWinId();
    wc.win_priority = WIN_ONE_LEVEL;
    wc.add_data_ptr = jerry_acquire_value(this_value);
    wc.window_move_style = MOVE_BY_SETTING;
    wc.window_style = WS_NO_DEFAULT_STYLE | WS_DISABLE_RETURN_WIN;
    wc.func = (PROCESSMSG_FUNC)js_native_gui_win_func;

    win_handle = MMK_CreateWindow(&wc);

    return win_handle;
}

void js_native_page_set_move_style(uint32 handle, char* move_style) {
    MMK_SetWinMoveStyle(handle, move_style);
}

void js_native_page_destroy(uint32 handle) {
    js_native_gui_releaseWinId(MMK_GetWinId(handle));
    jerry_release_value(MMK_GetWinAddDataPtr(handle));
    MMK_CloseWin(handle);
}
