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
 * @file     packagemanager.h
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:This is a JS app packagemanager
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "packagemanager.h"
#include "pm_internal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#define SYNC_WORK

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
/**
//  Description:    Install a JS app, app install dir see in "pm_internal.h",
                    default in "C:\app" dir.
//  Note:
//  Parameter:      p_path: The full path of app
//                  len: The length of path
//  Returns:        See pm_install_msg_e in "packagemanager.h"
**/
//****************************************************************************/
PUBLIC int js_app_install(wchar *p_path, int len)
{
    int ret = -1;
#ifdef SYNC_WORK
    ret = app_install_internal(p_path, len);
#else
    ret = send_install_app_sig(p_path, len);
#endif
    return ret;
}

/*****************************************************************************/
/**
//  Description:    Uninstall a JS app.
//  Note:
//  Parameter:      app_id: The app id of app
//  Returns:        See pm_uninstall_msg_e in "packagemanager.h"
**/
//****************************************************************************/
PUBLIC int js_app_uninstall(char *app_id)
{
    int ret = -1;
#ifdef SYNC_WORK
    ret = app_uninstall_internal(app_id);
#else
    ret = send_uninstall_app_sig(app_id);
#endif
    return ret;
}

/*****************************************************************************/
/**
//  Description:    Get app info by app id.
//  Note:
//  Parameter:      app_id: The app id of app
//                  pm_package_info_t: output data struct
//                  需要释放permission_list 和 page_list
//  Returns:        0:PASS other:FAIL
**/
//****************************************************************************/
PUBLIC int js_get_app_info(char *app_id, pm_package_info_t *result)
{
    return get_app_info_internal(app_id, result);
}

/*****************************************************************************/
/**
//  Description:    Get all app info.
//  Note:
//  Parameter:     output_list_head: The list head of link list, See define in
"pm_list.h"
//  Returns:        NONE
**/
//****************************************************************************/
PUBLIC void js_get_all_app_info(list_node *output_list_head)
{
    get_app_list_head(output_list_head);
}

/*****************************************************************************/
/**
//  Description:    Start packagemanager service, preload package in
"D:\package_preload".
//  Note:
//  Parameter:      NONE
//  Returns:        NONE
**/
//****************************************************************************/
PUBLIC void js_start_pacakge_manager_service()
{
    create_packagemanager_thread();

    // init work, install preload app
    send_init_sig();
}

/*****************************************************************************/
/**
//  Description:    Dump print all app info string.
//  Note:
//  Parameter:      NONE
//  Returns:        NONE
**/
//****************************************************************************/
PUBLIC void js_pm_dump(void)
{
    pm_dump_internal();
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif