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
 * @file     permision_ui.c
 * @brief    -
 * @author  zhigao.sun@unisoc.com
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
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
#include "cJSON.h"
#include "cafcontrol.h"
#include "guibutton.h"
#include "guictrl_api.h"
#include "guifont.h"
#include "guilcd.h"
#include "guires.h"
#include "guistring.h"
#include "mmi_id.h"
#include "mmi_resource.h"
#include "mmi_textfun.h"
#include "mmipub.h"
#include "packagemanager.h"
#include "permission_check.h"
#include "sci_types.h"
#include "sfs.h"
#include "std_header.h"
#include "window_parse.h"

#define MAX_NAME_LENTH (128)
#define MMI_TITLE_HEIGHT (32)
#define BUTTON_TOP (257)
#define BUTTON_BOTTOM (297)
#define RECT_TOP (260)
#define NAME_LENGTH_MAX (256)
#define BUTTONN_ALLOW_ALWAYS_LEFT (45)
#define BUTTONN_ALLOW_ALWAYS_RIGHT (105)
#define BUTTONN_ALLOW_ONCE_LEFT (125)
#define BUTTONN_ALLOW_ONCE_RIGHT (225)
#define BUTTONN_FORBID_LEFT (250)
#define BUTTONN_FORBID_RIGHT (300)
#define LABEL_CONTENT_LEFT (10)
#define LABEL_CONTENT_TOP (125)
#define LABEL_CONTENT_RIGHT (350)
#define LABEL_CONTENT_BOTTOM (175)
#define LABEL_NAME_LEFT (10)
#define LABEL_NAME_TOP (200)
#define LABEL_NAME_RIGHT (350)
#define LABEL_NAME_BOTTOM (230)

/*****************************************************************************/
//  Global resource dependence :
//  Author: zhigao.sun
//  Note:
/*****************************************************************************/

static app_grant_permission_info_t *g_show_info = NULL;
static PERM_NOTFIY g_perm_notify;
//当前工作节点
static list_node *g_current_node = NULL;

static sensitivepermission_id_e permission_info[] = {
    TXT_PERMISSION_NAME_CAMERA,
    TXT_PERMISSION_NAME_READ_CONTACTS,
    TXT_PERMISSION_NAME_WRITE_CONTACTS,
    TXT_PERMISSION_NAME_ACCESS_FINE_LOCATION,
    TXT_PERMISSION_NAME_ACCESS_COARSE_LOCATION,
    TXT_PERMISSION_NAME_READ_SMS,
    TXT_PERMISSION_NAME_WRITE_SMS,
    TXT_PERMISSION_NAME_RECORD_AUDIO,
    TXT_PERMISSION_NAME_CALL_PHNOE,
    TXT_PERMISSION_NAME_BODY_SENSORS,
    TXT_PERMISSION_NAME_PACKAGES_MANAGER,
    TXT_PERMISSION_NAME_SYSTEM_STORAGE,
    TXT_PERMISSION_NAME_INTERNET};

LOCAL MMI_RESULT_E HandlePermissionRuntimeMsg(MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param);

WINDOW_TABLE(MMI_PERMISSION_RUNTIME_WIN_TAB) = {
    WIN_HIDE_STATUS,
    WIN_ID(MMI_PERMISSION_RUNTIME_WIN_ID),
    WIN_FUNC((uint32)HandlePermissionRuntimeMsg),
    CREATE_BUTTON_CTRL(PNULL, MMI_PERMISSION_ALLOW_AWALYS_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMI_PERMISSION_ALLOW_ONCE_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMI_PERMISSION_ALLOW_FROBBID_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_PERMISSION_CONTENT_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_PERMISSION_NAME_CTRL_ID),
    END_WIN};

//根据当前工作指针找到下一个需要弹框的节点
LOCAL list_node *get_next_permission_node(list_node *current_node)
{
    list_node *perm_list_head = g_show_info->p_grant_permission_list_head;
    list_node *find_node = NULL;

    for (find_node = current_node; find_node != perm_list_head;
         find_node = find_node->next)
    {
        grant_permission_info_t *perm_info = NULL;
        perm_info = listnode_entry(find_node, grant_permission_info_t,
                                   grant_permission_info_node);
        if ((perm_info->grant_permission_status
             == PREMISSION_STATUS_GRANTED_ONCE)
            || (perm_info->grant_permission_status == PREMISSION_STATUS_NONE))
        {
            if (0 <= perm_info->grant_permission_id
                && perm_info->grant_permission_id <= 10000)
            {
                return find_node;
            }
        }
    }
    return NULL;
}

//更新节点
LOCAL int update_permission_node(list_node *find_node, int status)
{
    grant_permission_info_t *perm_info = NULL;
    if ((find_node == NULL) || (status < 0))
    {
        return -1;
    }
    perm_info = listnode_entry(find_node, grant_permission_info_t,
                               grant_permission_info_node);
    perm_info->grant_permission_status = status;
    return 0;
}

LOCAL BOOLEAN perm_file_access(char *permission_file_path)
{
    SFS_HANDLE sfs_fd_ptr;
    BOOLEAN ret = FALSE;
    wchar buff[MAX_NAME_LENTH] = {0};

    MMIAPICOM_StrToWstr(permission_file_path, buff);
    sfs_fd_ptr = SFS_CreateFile(
        (uint16 *)buff, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    ret = (sfs_fd_ptr > 0) ? TRUE : FALSE;
    SFS_CloseFile(sfs_fd_ptr);
    SCI_TRACE_LOW("%s completed,ret is:%d \n", __FUNCTION__, ret);
    return ret;
}

//保存permission.json文件
LOCAL void write_permission_to_file(app_grant_permission_info_t *g_show_info)
{
    SFS_HANDLE sfs_fd_ptr = 0;
    cJSON *root = NULL, *thm = NULL, *arr = NULL, *obj = NULL;
    char *file_write = NULL;
    uint32 bytes_written = 0;
    SFS_ERROR_E write_ret = SFS_NO_ERROR;
    wchar file_path[SFS_MAX_PATH] = {0};

    grant_permission_info_t *perm_info = NULL;
    list_node *find_perm_node = NULL;

    MMIAPICOM_StrToWstr((uint8 *)g_show_info->permission_file_path, file_path);

    SCI_TRACE_LOW("%s start running\n", __FUNCTION__);

    root = cJSON_CreateObject();
    thm = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "permissions", thm);
    cJSON_AddStringToObject(thm, "app_id", g_show_info->app_id);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(thm, "grantPermissions", arr);

    listnode_for_each(find_perm_node, g_show_info->p_grant_permission_list_head)
    {
        grant_permission_info_t *perm_info = NULL;
        perm_info = listnode_entry(find_perm_node, grant_permission_info_t,
                                   grant_permission_info_node);

        cJSON_AddItemToArray(arr, obj = cJSON_CreateObject());
        cJSON_AddNumberToObject(obj, "name", perm_info->grant_permission_id);
        cJSON_AddNumberToObject(obj, "status",
                                perm_info->grant_permission_status);
    }

    if (perm_file_access(g_show_info->permission_file_path))
    {
        // permission.json exist
        sfs_fd_ptr = SFS_CreateFile((uint16 *)file_path,
                                    SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE
                                        | SFS_MODE_READ,
                                    NULL, NULL);
    }
    else
    {
        // permission.json dont exist
        sfs_fd_ptr = SFS_CreateFile(
            (uint16 *)file_path,
            SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE | SFS_MODE_READ, NULL, NULL);
    }

    file_write = cJSON_Print(root);
    write_ret = SFS_WriteFile(sfs_fd_ptr, file_write, strlen(file_write),
                              &bytes_written, NULL);
    if (write_ret != SFS_NO_ERROR)
    {
        SCI_TRACE_LOW("%s,failed to write permission file \n", __FUNCTION__);
    }
    SCI_FREE(file_write);
    SFS_FlushFile(sfs_fd_ptr, NULL);
    SFS_CloseFile(sfs_fd_ptr);
    cJSON_Delete(root);
    SCI_TRACE_LOW("%s completed\n", __FUNCTION__);
}

LOCAL int node_to_permission_id(list_node *find_node)
{
    if (find_node)
    {
        grant_permission_info_t *perm_info = NULL;
        perm_info = listnode_entry(find_node, grant_permission_info_t,
                                   grant_permission_info_node);
        return perm_info->grant_permission_id;
    }
    return -1;
}

//根据权限ID在界面上显示不同的权限名称
LOCAL void show_permission_content_over_screen(MMI_WIN_ID_T win_id,
                                               list_node *find_node)
{
    GUI_RECT_T label_perm_name = {0};
    GUI_FONT_ALL_T font_id = {0};
    // bug id 1958585 CID 589220  589123
    int perm_index = 0 ;

    label_perm_name.left = LABEL_NAME_LEFT;
    label_perm_name.top = LABEL_NAME_TOP;
    label_perm_name.right = LABEL_NAME_RIGHT;
    label_perm_name.bottom = LABEL_NAME_BOTTOM;

    font_id.color = 0x81fb;
    font_id.font = CAF_FONT_22;

    CTRLLABEL_SetRect(MMI_PERMISSION_NAME_CTRL_ID, &label_perm_name, TRUE);
    CTRLLABEL_SetFont(MMI_PERMISSION_NAME_CTRL_ID, font_id.font, font_id.color);
    // bug id 1958585 CID 589220   589123
    perm_index = node_to_permission_id(find_node) ;
    perm_index = perm_index > 0 ? perm_index : 0 ;
    CTRLLABEL_SetTextById(MMI_PERMISSION_NAME_CTRL_ID,
                          permission_info[perm_index],
                          TRUE);
}

//权限页面布局
LOCAL void init_permission_window(MMI_WIN_ID_T win_id,
                                  list_node *g_current_node, GUI_RECT_T rect,
                                  GUI_LCD_DEV_INFO lcd_dev_info)
{
    GUI_RECT_T button_allow_always = {0};
    GUI_RECT_T button_allow_once = {0};
    GUI_RECT_T button_forbid = {0};
    GUI_RECT_T label_content = {0};
    GUI_RECT_T label_perm_name = {0};
    GUI_FONT_ALL_T font_id = {0};

    //定义允许，本次允许，禁止三个Button按钮在界面上的位置
    button_allow_always.top = BUTTON_TOP;
    button_allow_always.bottom = BUTTON_BOTTOM;
    button_allow_always.left = BUTTONN_ALLOW_ALWAYS_LEFT;
    button_allow_always.right = BUTTONN_ALLOW_ALWAYS_RIGHT;

    button_allow_once.top = BUTTON_TOP;
    button_allow_once.bottom = BUTTON_BOTTOM;
    button_allow_once.left = BUTTONN_ALLOW_ONCE_LEFT;
    button_allow_once.right = BUTTONN_ALLOW_ONCE_RIGHT;

    button_forbid.top = BUTTON_TOP;
    button_forbid.bottom = BUTTON_BOTTOM;
    button_forbid.left = BUTTONN_FORBID_LEFT;
    button_forbid.right = BUTTONN_FORBID_RIGHT;

    label_content.top = LABEL_CONTENT_TOP;
    label_content.bottom = LABEL_CONTENT_BOTTOM;
    label_content.left = LABEL_CONTENT_LEFT;
    label_content.right = LABEL_CONTENT_RIGHT;

    CTRLBUTTON_SetRect(MMI_PERMISSION_ALLOW_AWALYS_CTRL_ID,
                       &button_allow_always);
    CTRLBUTTON_SetRect(MMI_PERMISSION_ALLOW_ONCE_CTRL_ID, &button_allow_once);
    CTRLBUTTON_SetRect(MMI_PERMISSION_ALLOW_FROBBID_CTRL_ID, &button_forbid);
    CTRLLABEL_SetRect(MMI_PERMISSION_CONTENT_CTRL_ID, &label_content, TRUE);

    //添加背景图片
    GUIRES_DisplayImg(PNULL, &rect, &rect, win_id, IMAGE_PERMISSION,
                      &lcd_dev_info);

    //字体大小和颜色
    font_id.color = 0x0000;
    font_id.font = CAF_FONT_24;
    //显示要允许访问以下权限吗？
    CTRLLABEL_SetFont(MMI_PERMISSION_CONTENT_CTRL_ID, font_id.font,
                      font_id.color);
    CTRLLABEL_SetTextById(MMI_PERMISSION_CONTENT_CTRL_ID,
                          TXT_IF_ALLOW_ACCESS_TO_THE_FOLLOWING_PERMISSION,
                          TRUE);
    //改变字体大小
    font_id.font = CAF_FONT_22;
    //显示首个权限名称
    show_permission_content_over_screen(win_id, g_current_node);

    //显示允许
    CTRLBUTTON_SetTextAlign(MMI_PERMISSION_ALLOW_AWALYS_CTRL_ID,
                            ALIGN_HVMIDDLE);
    CTRLBUTTON_SetFont(MMI_PERMISSION_ALLOW_AWALYS_CTRL_ID, &font_id);
    CTRLBUTTON_SetTextId(MMI_PERMISSION_ALLOW_AWALYS_CTRL_ID,
                         TXT_PERMISSION_ALLOW);

    //显示本次允许
    CTRLBUTTON_SetTextAlign(MMI_PERMISSION_ALLOW_ONCE_CTRL_ID, ALIGN_HVMIDDLE);
    CTRLBUTTON_SetFont(MMI_PERMISSION_ALLOW_ONCE_CTRL_ID, &font_id);
    CTRLBUTTON_SetTextId(MMI_PERMISSION_ALLOW_ONCE_CTRL_ID,
                         TXT_PERMISSION_ALLOWONCE);

    //显示禁止
    CTRLBUTTON_SetTextAlign(MMI_PERMISSION_ALLOW_FROBBID_CTRL_ID,
                            ALIGN_HVMIDDLE);
    CTRLBUTTON_SetFont(MMI_PERMISSION_ALLOW_FROBBID_CTRL_ID, &font_id);
    CTRLBUTTON_SetTextId(MMI_PERMISSION_ALLOW_FROBBID_CTRL_ID,
                         TXT_PERMISSION_NOTALLOW);
}

//处理界面消息
LOCAL MMI_RESULT_E HandlePermissionRuntimeMsg(MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0, 0, 359, 479};
    grant_permission_status_e status = 0;
    list_node *find_node = NULL;

    SCI_TRACE_LOW("%s start running \n", __FUNCTION__);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
        init_permission_window(win_id, g_current_node, rect, lcd_dev_info);
        break;
    }

    case MSG_APP_CANCEL:
    {
        MMK_CloseWin(win_id);
        break;
    }

    case MSG_NOTIFY_PENOK:
    {
        MMI_NOTIFY_T notify = *(MMI_NOTIFY_T *)param;
        MMI_HANDLE_T ctrlHandle = notify.src_handle;
        MMI_CTRL_ID_T ctrlId = MMK_GetCtrlId(ctrlHandle);

        SCI_TRACE_LOW("%s ,MSG_NOTIFY_PENOK\n", __FUNCTION__);

        if (ctrlId == MMI_PERMISSION_ALLOW_AWALYS_CTRL_ID)
        {
            status = PREMISSION_STATUS_GRANTED;
        }
        else if (ctrlId == MMI_PERMISSION_ALLOW_ONCE_CTRL_ID)
        {
            status = PREMISSION_STATUS_GRANTED_ONCE;
        }
        else if (ctrlId == MMI_PERMISSION_ALLOW_FROBBID_CTRL_ID)
        {
            status = PREMISSION_STATUS_DENIED;
        }
        else
        {
            return recode;
        }
        //根据界面选择结果更新链表
        update_permission_node(g_current_node, status);
        g_current_node = get_next_permission_node(g_current_node->next);

        //如果当前节点next指针指向头节点，则关闭界面
        if (g_current_node == NULL)
        {
            MMK_CloseWin(win_id);
            write_permission_to_file(g_show_info);
            g_perm_notify(0);
        }
        else
        {
            show_permission_content_over_screen(win_id, g_current_node);
            MMK_UpdateScreen();
        }
        break;
    }

    default:
    {
        recode = MMI_RESULT_FALSE;
        break;
    }
    }

    return recode;
}

PUBLIC void MMIPERMISS_CreateUI(app_grant_permission_info_t *_info,
                                PERM_NOTFIY perm_notify)
{
    g_show_info = _info;
    g_perm_notify = perm_notify;

    g_current_node = get_next_permission_node(
        g_show_info->p_grant_permission_list_head->next);

    //无需弹出界面
    if (g_current_node == NULL)
    {
        g_perm_notify(0);
        SCI_TRACE_LOW("%s completed \n", "g_perm_notify(0)");
    }
    else
    {
        //需要弹出界面

        MMK_CreateWin((uint32 *)MMI_PERMISSION_RUNTIME_WIN_TAB, NULL);
    }
}
#endif
