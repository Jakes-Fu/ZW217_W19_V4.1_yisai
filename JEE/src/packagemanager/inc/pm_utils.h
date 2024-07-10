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
 * @date     2021-9-1
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

#ifndef PM_UTILS_H
#define PM_UTILS_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "ual_type.h"
#include <stdio.h>
#include <stdlib.h>

#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef wchar
typedef unsigned short   wchar;
#endif
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

#define BIG_TO_LITTLE16(A)                                                     \
    ((((uint16)(A)&0xff00) >> 8) | (((uint16)(A)&0x00ff) << 8))

#define BIG_TO_LITTLE32(A)                                                     \
    ((((uint32)(A)&0xff000000) >> 24) | (((uint32)(A)&0x00ff0000) >> 8)        \
     | (((uint32)(A)&0x0000ff00) << 8) | (((uint32)(A)&0x000000ff) << 24))

#ifndef uint16
#define uint16 unsigned short
#endif /* uint16 */

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC wchar *wstrncpy(wchar *dst, const wchar *src, size_t count);
PUBLIC wchar *wstrncat(wchar *dst, const wchar *src, size_t count);
PUBLIC wchar *wstrcat(wchar *dst, const wchar *src);
PUBLIC size_t wstrlen(const wchar *str);
PUBLIC int wstrncmp(const wchar *string1, const wchar *string2, size_t count);
PUBLIC uint8 *wstr_to_str(const wchar *src, uint8 *dst);
PUBLIC wchar *str_to_wstr(const uint8 *src, wchar *dst);
PUBLIC uint8 *wstrn_to_str(const wchar *src, uint8 *dst, size_t count);

// FILE
PUBLIC void split_file_name(/*INOUT*/ uint16 *pname, /*OUT*/ uint16 *fname);
PUBLIC int64 get_device_free_space(const uint16 *disk_name);
PUBLIC BOOLEAN delete_dir(const uint16 *folder_path_ptr,
                          const uint16 *folder_name_ptr);

PUBLIC BOOLEAN check_file_suffix(const uint16 *full_path,
                                 const uint16 *check_suffix_name);
PUBLIC BOOLEAN check_device_enough_space(uint16 *p_path, uint16 *device_name);

// DIR
PUBLIC uint32 path_combine(const uint16 *p_base, const uint16 *p_dir,
                           uint16 *outout_path);

// ZIP
PUBLIC int decode_zip_by_file(uint16 *src_file_name_ptr, uint16 *dest_base_dir,
                              uint16 *dest_dir_name);
PUBLIC int decode_single_file_inzip(wchar *src_zip_file,
                                    wchar *decode_file_name,
                                    wchar *dst_dir_path);

// LCD
PUBLIC BOOLEAN get_lcd_width_height(uint16 *lcd_width, uint16 *lcd_height);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End