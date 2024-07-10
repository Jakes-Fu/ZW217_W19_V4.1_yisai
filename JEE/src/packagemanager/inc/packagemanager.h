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
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

#ifndef PM_PACKAGEMANAGER_H
#define PM_PACKAGEMANAGER_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "ual_type.h"
#include <stdio.h>
#include <stdlib.h>
#include "sci_types.h"
#include "jee_list.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#ifndef wchar
typedef unsigned short   wchar;
#endif
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define MAX_VERSION_LENTH (32)
#define MAX_PATH_LENTH (255)

#ifndef MAX_NAME_LENTH
#define MAX_NAME_LENTH (128)
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef enum
{
    PM_INSTALL_OK,
    PM_INSTALL_INIT_ERROR,
    PM_INSTALL_PARAM_ERROR,
    PM_INSTALL_SUFFIX_ERROR,
    PM_INSTALL_UNZIP_ERROR,
    PM_INSTALL_PARSE_JSON_ERROR,
    PM_INSTALL_VERSION_LOWER_ERROR,
    PM_INSTALL_NO_ENOUGH_SPACE_ERROR,
    PM_INSTALL_UNKNOW_ERROR,
    PM_INSTALL_MSG_MAX
} pm_install_msg_e;

typedef enum
{
    PM_UNINSTALL_OK,
    PM_UNINSTALL_INIT_ERROR,
    PM_UNINSTALL_PARAM_ERROR,
    PM_UNINSTALL_UNKNOW_ERROR,
    PM_UNINSTALL_MSG_MAX
} pm_uninstall_msg_e;

// Page list
typedef struct tag_pm_pages_info_t
{
    char page_name[MAX_NAME_LENTH];
    list_node page_info_node;
} pm_pages_info_t;

typedef struct tag_pm_permisson_info_t
{
    char permisson_name[MAX_NAME_LENTH];
    list_node permisson_info_node;
} pm_permisson_info_t;

typedef struct tag_pm_package_info_t
{
    char app_id[MAX_NAME_LENTH];
    char app_name[MAX_NAME_LENTH];
    char vendor_name[MAX_NAME_LENTH];
    char parameter[MAX_NAME_LENTH];
    int app_version_code;
    char app_version_name[MAX_VERSION_LENTH];
    char api_version_compatible[MAX_VERSION_LENTH];
    char api_version_target[MAX_VERSION_LENTH];
    char app_install_path[MAX_PATH_LENTH];
    char app_icon[MAX_PATH_LENTH];
    list_node *p_permisson_list_head;
    list_node *p_pages_list_head;
    list_node package_info_node;
} pm_package_info_t;

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Install a JS app.
//  Note:
//  Parameter:      p_path: The full path of app
//                  len: The length of path
//  Returns:        See pm_install_msg_e in "packagemanager.h"
//****************************************************************************/
PUBLIC int js_app_install(wchar *p_path, int len);

/*****************************************************************************/
//  Description:    Uninstall a JS app.
//  Note:
//  Parameter:      app_id: The id of app
//  Returns:        See pm_uninstall_msg_e in "packagemanager.h"
//****************************************************************************/
PUBLIC int js_app_uninstall(char *app_id);

/*****************************************************************************/
//  Description:    Get app info by app id.
//  Note:
//  Parameter:      app_id: The app id of app
//                  result: output data struct
//pm_package_info_t
//  Returns:        0: Get app info pass
//					-1: Get app info fail
//****************************************************************************/
PUBLIC int js_get_app_info(char *app_id, pm_package_info_t *result);

/*****************************************************************************/
//  Description:    Get all app info.
//  Note:
//  Parameter:      output_list_head: The list head of link list, See define in
//  "pm_list.h" Returns:        NONE
//****************************************************************************/
PUBLIC void js_get_all_app_info(list_node *output_list_head);

/*****************************************************************************/
//  Description:    Start packagemanager service, install preload package.
//  Note:
//  Parameter:      NONE
//  Returns:        NONE
//****************************************************************************/
PUBLIC void js_start_pacakge_manager_service();

/*****************************************************************************/
//  Description:    Dump print all app info string.
//  Note:
//  Parameter:      NONE
//  Returns:        NONE
//****************************************************************************/
PUBLIC void js_pm_dump(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End