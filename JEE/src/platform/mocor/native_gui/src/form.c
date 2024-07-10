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
 * @file     form.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/*lint -save -e746 -e718 */
#include "sci_types.h"

#include "ctrlbase.h"
#include "ctrlform.h"
#include "ctrlform_export.h"
#include "guictrl.h"
#include "mmk_type.h"

#include "form.h"

// JS
#include "urjs_art.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
 **---------------------------------------------------------------------------*/

// set child align
LOCAL void set_child_align(MMI_HANDLE_T form_handle,
                           MMI_HANDLE_T form_child_handle)
{
    GUIFORM_LAYOUT_E layout_type = CTRLFORM_GetLayoutType(form_handle);
    GUIFORM_CHILD_ALIGN_E align = CTRLFORM_GetChildAlign(form_handle);
    // Align support in order layout only.
    if (layout_type == -1 || layout_type == GUIFORM_LAYOUT_SBS)
    {
        return;
    }
    CTRLFORM_SetChildAlign(form_handle, form_child_handle, align);
}

// set child width
LOCAL void set_child_width(MMI_HANDLE_T form_handle, MMI_HANDLE_T child_handle)
{
    GUI_RECT_T ctrl_rect = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};

    GUICTRL_GetRect(MMK_GetCtrlPtr(child_handle), &ctrl_rect);

    if (ctrl_rect.left == 0 && ctrl_rect.top == 0 && ctrl_rect.right == 0
        && ctrl_rect.bottom == 0)
    {
        return;
    }
    if (ctrl_rect.right - ctrl_rect.left > 0)
    {
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = ctrl_rect.right - ctrl_rect.left;
    }
    else
    {
        child_width.type = GUIFORM_CHILD_WIDTH_AUTO;
    }
    CTRLFORM_SetChildWidth(form_handle, child_handle, &child_width);
}

LOCAL void set_child_height(MMI_HANDLE_T form_handle, MMI_HANDLE_T child_handle)
{
    GUI_RECT_T ctrl_rect = {0};
    GUIFORM_CHILD_HEIGHT_T child_height = {0};

    GUICTRL_GetRect(MMK_GetCtrlPtr(child_handle), &ctrl_rect);

    if (ctrl_rect.left == 0 && ctrl_rect.top == 0 && ctrl_rect.right == 0
        && ctrl_rect.bottom == 0)
    {
        return;
    }
    CAF_GUID_T guid = IGUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(child_handle));
    switch (guid)
    {
    // Set form child maybe cause layout issue
    case SPRD_GUI_FORM_ID:
        child_height.type = GUIFORM_CHILD_HEIGHT_AUTO;
        break;
    default:
        if (ctrl_rect.bottom - ctrl_rect.top <= 0)
        {
            child_height.type = GUIFORM_CHILD_HEIGHT_AUTO;
        }
        else
        {
            child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            child_height.add_data = ctrl_rect.bottom - ctrl_rect.top;
        }
        break;
    }
    CTRLFORM_SetChildHeight(form_handle, child_handle, &child_height);
}

// form set
LOCAL void set_form_default_param(MMI_HANDLE_T form_handle)
{
    // Set bg default FALSE
    CTRLFORM_PermitChildBg(form_handle, FALSE);
    // Set Border default FALSE
    CTRLFORM_PermitChildBorder(form_handle, FALSE);
    // Set Font default FALSE
    CTRLFORM_PermitChildFont(form_handle, FALSE);
}

LOCAL void set_form_active_child(MMI_HANDLE_T form_ctrl_handle)
{
    CTRLFORM_OBJ_T *form_ctrl_ptr = PNULL;

    form_ctrl_ptr = CTRLFORM_GetPtr(MMK_ConvertIdToHandle(form_ctrl_handle));
    if (NULL == form_ctrl_ptr)
    {
        return;
    }
    // Set active child in top level form ctrl only.
    if (PNULL != form_ctrl_ptr->first_child_ptr)
    {
        MMI_HANDLE_T parent_handle = 0;
        IGUICTRL_T *ctrl_ptr = PNULL;

        // get parent handle
        parent_handle = MMK_GetParentCtrlHandle(form_ctrl_handle);
        if (parent_handle != 0)
        {
            ctrl_ptr = MMK_GetCtrlPtr(parent_handle);
            if (SPRD_GUI_FORM_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
            {
                // If ctrl parent is form,just return.
                return;
            }
        }
        // Now parent is window.
        // Set child pointer by first_valid_index
        CTRLFORM_SetActiveChild(form_ctrl_handle,
                                form_ctrl_ptr->first_child_ptr->ctrl_handle);
    }
}

LOCAL void set_form_child_default_param(MMI_HANDLE_T form_ctrl_handle,
                                        MMI_HANDLE_T child_ctrl_handle)
{
    // Set first child is active ctrl default.
    set_form_active_child(form_ctrl_handle);

    // set form child width by ctrlbase width
    set_child_width(form_ctrl_handle, child_ctrl_handle);
    // set form child height by ctrlbase height
    set_child_height(form_ctrl_handle, child_ctrl_handle);
    // set child align
    set_child_align(form_ctrl_handle, child_ctrl_handle);
}

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC BOOLEAN is_form_ctrl(uint32 handle)
{
    BOOLEAN is_form = FALSE;
    IGUICTRL_T *ctrl_ptr = PNULL;

    ctrl_ptr = MMK_GetCtrlPtr(MMK_ConvertIdToHandle(handle));
    if (SPRD_GUI_FORM_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
    {
        is_form = TRUE;
    }

    return is_form;
}

/**
 *=========================================================
 * Function     :   js_native_form_create_child
 * Author       :   ningbiao.huang
 * Description  :   create a form's child ctrl.
 * Input        :   parent_form_id:    The parent form ctrl id.
 *                  child_id:  The id of child ctrl.
 *                  guid:  guid of child ctrl.
 *                  init_data:  init data poiter.
 * Output       :   NONE
 * Return       :   child ctrl handle
 * History      :   2022/04/28  Initial draft
 *=========================================================
 */
PUBLIC uint32 js_native_form_create_child(uint32 parent_form_id,
                                          uint32 child_id, uint32 guid,
                                          void *init_data)
{
    MMI_HANDLE_T child_handle = 0;
    MMI_WIN_ID_T win_id = 0;
    GUIFORM_DYNA_CHILD_T form_child_ctrl = {0};

    // create child ctrl
    form_child_ctrl.child_handle = child_id;
    form_child_ctrl.guid = guid;
    form_child_ctrl.init_data_ptr = init_data;
    form_child_ctrl.is_bg = FALSE;
    form_child_ctrl.is_softkey = FALSE;
    form_child_ctrl.is_get_active = TRUE;

    // get win id from parent form
    win_id = MMK_GetWinHandleByCtrl(MMK_ConvertIdToHandle(parent_form_id));
    // create form child ctrl
    CTRLFORM_CreatDynaChildCtrl(win_id, parent_form_id, &form_child_ctrl);
    // id to handle
    child_handle = MMK_ConvertIdToHandle(child_id);
    // set child param
    // set_form_child_default_param(parent_form_id, child_handle);

    return child_handle;
}

PUBLIC uint32 js_native_form_create(uint32 this_value, uint32 win_handle)
{
    MMI_HANDLE_T parentHandle = win_handle;
    MMI_HANDLE_T handle = NULL;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();
    // Use diff create fun by parentHandle.
    if (MMI_HANDLE_CONTROL == MMK_GetHandleType(parentHandle)
        && is_form_ctrl(parentHandle))
    {
        GUIFORM_INIT_DATA_T init_data = {0};
        // set init data
        init_data.layout_type = GUIFORM_LAYOUT_ORDER;
        js_native_form_create_child(parentHandle, ctrl_id, SPRD_GUI_FORM_ID,
                                    &init_data);
    }
    else
    {
        CTRLFORM_CreatDynaCtrl(parentHandle, ctrl_id, GUIFORM_LAYOUT_ORDER);
    }

    handle = MMK_ConvertIdToHandle(ctrl_id);
    if (this_value != 0)
    {
        CTRLBASE_SetUserData(handle, jerry_acquire_value(this_value));
    }
    set_form_default_param(handle);
    return handle;
}

PUBLIC void js_native_form_destory(uint32 form_ctrl_handle)
{
    /*lint -e18*/
    js_native_gui_releaseCtrlId(
        MMK_GetCtrlId(MMK_ConvertIdToHandle(form_ctrl_handle)));
    jerry_release_value(CTRLBASE_GetUserData(form_ctrl_handle));
    MMK_DestroyControl(form_ctrl_handle);
}

PUBLIC void js_native_form_add_child(uint32 win_handle, uint32 form_ctrl_handle,
                                     uint32 child_ctrl_handle)
{
    // Add child in create
    /*
    BOOLEAN ret = FALSE;
    ret =
        CTRLFORM_AddChildCtrl(win_handle, form_ctrl_handle, child_ctrl_handle);

    alogi("js_native_gui_form_add_child ret=%d", ret);
    set_form_child_default_param(form_ctrl_handle, child_ctrl_handle);
    */
    /* Set param here , makesure param has's been setup.*/
    set_form_child_default_param(form_ctrl_handle, child_ctrl_handle);
}

PUBLIC void js_native_form_remove_child(uint32 form_ctrl_handle,
                                        uint32 child_ctrl_handle)
{
    BOOLEAN ret = FALSE;
    MMI_CTRL_ID_T form_id =
        MMK_GetCtrlId(MMK_ConvertIdToHandle(form_ctrl_handle));
    MMI_CTRL_ID_T child_id =
        MMK_GetCtrlId(MMK_ConvertIdToHandle(child_ctrl_handle));

    ret = CTRLFORM_DestroyDynaChildCtrl(form_id, child_id);

    //ret = CTRLFORM_ForceLayout(form_id);
    //MMK_UpdateScreen();
}

PUBLIC void
js_native_form_set_flex_direction(uint32 form_ctrl_handle,
                                  div_flex_direction_e flex_direction)
{
    if (flex_direction == DIV_FLEX_DIRECTION_ROW)
    {
        CTRLFORM_SetLayoutType(form_ctrl_handle, GUIFORM_LAYOUT_SBS);
    }
    else if (flex_direction == DIV_FLEX_DIRECTION_COLUMN)
    {
        CTRLFORM_SetLayoutType(form_ctrl_handle, GUIFORM_LAYOUT_ORDER);
    }
    else
    {
        alogi("Unknow direction flex_direction=%d", flex_direction);
    }
}

PUBLIC void js_native_form_set_flex_wrap(uint32 form_ctrl_handle,
                                         div_flex_wrap_e flex_wrap)
{
    if (flex_wrap == DIV_FLEX_WRAP_WRAP)
    {
        // TODO
    }
    else if (flex_wrap == DIV_FLEX_WRAP_NOWRAP)
    {
        // TODO
    }
    else
    {
        alogi("Unknow flex_wrap=%d", flex_wrap);
    }
}

PUBLIC void
js_native_form_set_justify_content(uint32 form_ctrl_handle,
                                   div_justify_content_e justify_content)
{
    if (justify_content == DIV_JUSTIFY_CONTENT_FLEX_START)
    {
        // CTRLFORM_SetAllChildAlign(form_ctrl_handle,
        // GUIFORM_CHILD_ALIGN_LEFT);
        CTRLFORM_SetAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_LEFT);
    }
    else if (justify_content == DIV_JUSTIFY_CONTENT_FLEX_END)
    {
        // CTRLFORM_SetAllChildAlign(form_ctrl_handle,
        // GUIFORM_CHILD_ALIGN_RIGHT);
        CTRLFORM_SetAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_RIGHT);
    }
    else if (justify_content == DIV_JUSTIFY_CONTENT_CENTER)
    {
        // CTRLFORM_SetAllChildAlign(form_ctrl_handle,
        // GUIFORM_CHILD_ALIGN_HMIDDLE);
        CTRLFORM_SetAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);
    }
    else
    {
        alogi("Unknow justify_content=%d", justify_content);
    }
}

PUBLIC void js_native_form_set_align_items(uint32 form_ctrl_handle,
                                           div_align_items_e align_items)
{
    if (align_items == DIV_ALIGN_ITEMS_STRETCH)
    {
        // do nothing
    }
    else if (align_items == DIV_ALIGN_ITEMS_FLEX_START)
    {
        // CTRLFORM_SetAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_LEFT);
        CTRLFORM_SetAllChildAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_LEFT);
    }
    else if (align_items == DIV_ALIGN_ITEMS_FLEX_END)
    {
        // CTRLFORM_SetAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_RIGHT);
        CTRLFORM_SetAllChildAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_RIGHT);
    }
    else if (align_items == DIV_ALIGN_ITEMS_CENTER)
    {
        // CTRLFORM_SetAlign(form_ctrl_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);
        CTRLFORM_SetAllChildAlign(form_ctrl_handle,
                                  GUIFORM_CHILD_ALIGN_HMIDDLE);
    }
    else
    {
        alogi("Unknow align_items=%d", align_items);
    }
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*lint -restore */
