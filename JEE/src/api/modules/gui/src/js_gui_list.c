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
  * @file     js_gui_list.c
  * @brief    -
  * @author   ningbiao.huang@unisoc.com
  * @version  V1.0.0
  * @date     2022-4-27
  * @history
  *****************************************************************************
  **/

#include "js_gui_list.h"
#include "mmk_type.h"
#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "caf.h"
#include "ctrlbase.h"
#include "ctrllist_export.h"

#include "form.h"

#define WSTR_MAX_LEN (100)

DECLARE_PUBLIC_HANDLER(js_gui_list_create)
{
    JS_DEBUG_LOG("creat dynamic list control\n");
    JERRY_CHECK_ARGS(2, object, number);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int page_handle = jerry_get_number_value(args_p[1]);
    MMI_CONTROL_CREATE_T create = {0};
    MMI_HANDLE_T handle = NULL;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();
    GUILIST_INIT_DATA_T listinitData = {0};
    CTRLBASE_OBJ_T *ctrl_ptr = NULL;

    MMI_HANDLE_T parentHandle = page_handle;
    // Use diff create fun by parentHandle.
    if (MMI_HANDLE_CONTROL == MMK_GetHandleType(parentHandle)
        && is_form_ctrl(parentHandle))
    {
        GUILIST_INIT_DATA_T init_data = {0};
        handle = js_native_form_create_child(parentHandle, ctrl_id,
                                                 SPRD_GUI_LIST_ID, &init_data);
    }
    else
    {
        // creat list
        create.ctrl_id = ctrl_id;
        create.guid = SPRD_GUI_LIST_ID;
        create.parent_win_handle = page_handle;
        create.parent_ctrl_handle = NULL;
        create.init_data_ptr = &listinitData;
        ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_CreateControl(&create);
        if (PNULL != ctrl_ptr)
        {
            handle = ctrl_ptr->handle;
        }
    }

    CTRLBASE_SetUserData(handle, jerry_acquire_value(obj));
    CTRLLIST_SetListState(ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
    // Set active ctrl in win child only
    if (MMI_HANDLE_WINDOW == MMK_GetHandleType(parentHandle))
    {
        // this api will send MSG_NOTIFY_GET_ACTIVE,should be
        // called after CTRLBASE_SetUserData
        MMK_SetAtvCtrl(page_handle, ctrl_id);
    }

    return jerry_create_number(handle);
}
DECLARE_PUBLIC_HANDLER(js_gui_list_append_data)
{
    JERRY_CHECK_ARGS(3, number, string, string);
    MMI_STRING_T str1 = {0};
    MMI_STRING_T str2 = {0};
    int list_handle = jerry_get_number_value(args_p[0]);
    jerry_string_t content1 = JERRY_GET_ARG(1, string);
    jerry_string_t content2 = JERRY_GET_ARG(2, string);
    jerry_size_t size1 = jerry_string_size(&content1);
    jerry_size_t size2 = jerry_string_size(&content2);
    wchar *p_buff1 = jerry_buffer_allocate((size1 + 1) * sizeof(wchar));
    if (NULL == p_buff1)
    {
        return jerry_create_null();
    }
    wchar *p_buff2 = jerry_buffer_allocate((size2 + 1) * sizeof(wchar));
    if (NULL == p_buff2)
    {
        jerry_buffer_release(p_buff1);
        return jerry_create_null();
    }
    GUI_UTF8ToWstr(p_buff1, size1, jerry_string_data(&content1), size1);
    GUI_UTF8ToWstr(p_buff2, size2, jerry_string_data(&content2), size2);
    str1.wstr_ptr = p_buff1;
    str1.wstr_len = size1;
    str2.wstr_ptr = p_buff2;
    str2.wstr_len = size2;
    WatchCOM_ListItem_Draw_2Str_2Lin_ListItem(list_handle, str1, str2);
    jerry_buffer_release(p_buff1);
    p_buff1 = NULL;
    jerry_buffer_release(p_buff2);
    p_buff2 = NULL;
    jerry_string_destroy(&content1);
    jerry_string_destroy(&content2);

    return jerry_create_null();
}
DECLARE_PUBLIC_HANDLER(js_gui_list_set_bg_color)
{
    JERRY_CHECK_ARGS(2, number, number);
    int list_handle = jerry_get_number_value(args_p[0]);
    int bg_color = jerry_get_number_value(args_p[1]);
    CTRLLIST_SetBgColor(list_handle, bg_color);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_list_set_text_scroll)
{
    JERRY_CHECK_ARGS(2, number, number);
    int list_handle = jerry_get_number_value(args_p[0]);
    int is_scroll = jerry_value_to_boolean(args_p[1]);
    JS_DEBUG_LOG("text is scroll, number is %d\n", is_scroll);
    CTRLLIST_SetListState(list_handle, GUILIST_STATE_TEXTSCROLL_ENABLE,
                          is_scroll);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_list_set_font)
{
    JERRY_CHECK_ARGS(3, number, number, number);
    int list_handle = jerry_get_number_value(args_p[0]);
    int font_size = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("font size is %d\n", font_size);
    int font_color = jerry_get_number_value(args_p[2]);
    CTRLLIST_SetTextFont(list_handle, font_size, font_color);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_list_set_scrollbar)
{
    JERRY_CHECK_ARGS(2, number, number);
    int list_handle = jerry_get_number_value(args_p[0]);
    int is_show = jerry_value_to_boolean(args_p[1]);
    JS_DEBUG_LOG("TRUE or FALSE,result is %d\n", is_show);
    CTRLLIST_SetNeedPrgbar(list_handle, is_show);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_list_destroy)
{
    JERRY_CHECK_ARGS(1, number);
    int list_handle = jerry_get_number_value(args_p[0]);
    js_native_gui_releaseCtrlId(MMK_GetCtrlId(list_handle));
    jerry_release_value(CTRLBASE_GetUserData(list_handle));
    MMK_DestroyControl(list_handle);

    return jerry_create_null();
}
DECLARE_PUBLIC_HANDLER(js_gui_list_setMaxItem)
{
    JERRY_CHECK_ARGS(2, number, number);
    int list_handle = jerry_get_number_value(args_p[0]);
    int item_max = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("max item is %d\n", item_max);
    CTRLLIST_SetMaxItem(list_handle, item_max, FALSE);

    return jerry_create_null();
}
DECLARE_PUBLIC_HANDLER(js_gui_list_set_item_height)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint16 index = 0;
    int list_handle = jerry_get_number_value(args_p[0]);
    int item_height = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("item height is %d\n", item_height);
    THEMELIST_ITEM_STYLE_T *style_ptr = PNULL;

    index = CTRLLIST_GetTotalItemNum(list_handle) - 1;
    style_ptr = GUILIST_GetItemStyle(list_handle, index);
    if (PNULL == style_ptr)
    {
        JS_DEBUG_LOG("js_gui_list style_ptr is NULL");
        js_art_set_force_exit_flag(TRUE);//need exit js
        return jerry_create_null();
    }
    style_ptr->height = item_height;
    style_ptr->height_focus = item_height;
    CTRLLIST_SetItemStyle(list_handle, index, style_ptr);

    return jerry_create_null();
}
DECLARE_PUBLIC_HANDLER(js_gui_list_set_font_size)
{
    JERRY_CHECK_ARGS(3, number, number, number);
    uint16 index = 0;
    THEMELIST_ITEM_STYLE_T *font_ptr = PNULL;

    int list_handle = jerry_get_number_value(args_p[0]);
    JS_DEBUG_LOG("list_handle is %d\n", list_handle);
    int font_size0 = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("line1 font size is %d\n", font_size0);
    int font_size1 = jerry_get_number_value(args_p[2]);
    JS_DEBUG_LOG("line2 font size is %d\n", font_size1);

    index = CTRLLIST_GetTotalItemNum(list_handle) - 1;
    font_ptr = GUILIST_GetItemStyle(list_handle, index);
    if(font_ptr == PNULL)
    {
        JS_DEBUG_LOG("js_gui_list_set_font_size font_ptr is NULL");
        js_art_set_force_exit_flag(TRUE);//need exit js
        return jerry_create_null();
    }
    font_ptr->content[0].font = font_size0;
    font_ptr->content[0].font_focus = font_size0;
    font_ptr->content[1].font = font_size1;
    font_ptr->content[1].font_focus = font_size1;
    CTRLLIST_SetItemStyle(list_handle, index, font_ptr);

    return jerry_create_null();
}
DECLARE_PUBLIC_HANDLER(js_gui_list_get_cur_index)
{
    JERRY_CHECK_ARGS(1, number);
    uint16 index = 0;
    int list_handle = jerry_get_number_value(args_p[0]);
    index = CTRLLIST_GetCurItemIndex(list_handle);
    return jerry_create_number(index);
}
DECLARE_PUBLIC_HANDLER(js_gui_list_set_item_font_color)
{
    JERRY_CHECK_ARGS(4, number, number, number, number);
    int list_handle = jerry_get_number_value(args_p[0]);
    int item_index = jerry_get_number_value(args_p[1]);
    int item_font_color = jerry_get_number_value(args_p[2]);
    int is_modify = jerry_value_to_boolean(args_p[3]);
#ifdef ADULT_WATCH_SUPPORT	
    WatchCOM_ListItem_Modify_OneItem_FontColor(list_handle, item_index,
                                               item_font_color, is_modify);
#else
	//instead
#endif
    return jerry_create_null();
}
