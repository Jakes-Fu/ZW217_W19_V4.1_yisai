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
 * @file     pm_internal.h
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

#ifndef PM_INTERNAL_H
#define PM_INTERNAL_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "packagemanager.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

// For lint issue
#ifndef uint16
#define uint16 unsigned short
#endif /* uint16 */

#ifndef wchar
#define wchar unsigned short
#endif /* wchar */

#ifndef wchar_t
#define wchar_t unsigned short
#endif /* wchar_t */

#ifndef uint8
#define uint8 unsigned char
#endif /* uint8 */

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Variables                                *
 **---------------------------------------------------------------------------*/

/**
app.json:
{
    "app": {
        "id": "com.unisoc.exsample",
        "name": "testAPP",
        "vendor": "unisoc",
        "version": {
            "code": 1,
            "name": "1.0"
        },
        "parameter": "2",
        "apiVersion": {
            "compatible": "3",
            "target": "4"
        },
        "icon": "//common/logo.png",
        "permissions": [
            {
                "name": "PERMISSION.CAMEA"
            },
            {
                "name": "PERMISSION.CALL_PHONE"
            }
        ],
        "pages":[
                "pages/plist/plist",
                "pages/play/play",
                "pages/volume/volume"
        ]
    }
}
**/
// Should match pm_package_info_t
#define APP_JSON_ROOT "app"
#define APP_JSON_ID "id"
#define APP_JSON_NAME "name"
#define APP_JSON_VENDOR "vendor"
#define APP_JSON_PARAMTER "parameter"
#define APP_JSON_VERSION "version"
#define APP_JSON_VERSION_CODE "code"
#define APP_JSON_VERSION_NAME "name"
#define APP_JSON_APIVERSION "apiVersion"
#define APP_JSON_APIVERSION_COMPATIBLE "compatible"
#define APP_JSON_APIVERSION_TARGET "target"
#define APP_JSON_ICON "icon"
#define APP_JSON_PERMISSONS "permissions"
#define APP_JSON_PERMISSON_NAME "name"
#define APP_JSON_PAGES "pages"
// packagelist.json
#define APP_JSON_PACKAGELIST_ROOT "packages"

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

typedef struct pm_found_files_t
{
    wchar_t *found_file;
    uint32 found_file_path_len;
    list_node node;
} pm_found_files_t;

typedef struct pm_install_sign_t
{
    SIGNAL_VARS /*!<Signal Header.*/
        wchar *app_path;
    uint32 app_path_len;
} pm_install_sign_t;

typedef struct pm_uninstall_sign_t
{
    SIGNAL_VARS /*!<Signal Header.*/
        char *app_id;
} pm_uninstall_sign_t;

typedef enum
{
    PM_SIG_INSTALL_APP,
    PM_SIG_UNINSTALL_APP,
    PM_SIG_INIT,
    PM_SIG_MAX
} pm_request_sig_e;

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC int get_app_info_internal(char *app_id, pm_package_info_t *output_info);

PUBLIC int get_app_list_head(list_node *output_list_head);

PUBLIC int send_install_app_sig(wchar *p_path, int len);

PUBLIC int send_uninstall_app_sig(char *app_id);

PUBLIC int send_init_sig(void);

PUBLIC void pm_dump_internal(void);

PUBLIC int veryfySig();

PUBLIC void create_packagemanager_thread(void);

PUBLIC int app_install_internal(wchar *p_path, int len);

PUBLIC int app_uninstall_internal(char *app_id);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End