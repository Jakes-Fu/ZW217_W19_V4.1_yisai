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
 * @file     pm_adapter_zip.h
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

#ifndef PM_ADAPTER_ZIP_H
#define PM_ADAPTER_ZIP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Variables                                  *
 **---------------------------------------------------------------------------*/

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

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Unzip a zip format file to dst dir.
//  Note:
//  Parameter:      src_file_name_ptr: Input zip format file
//                  dest_base_dir: Parent path of dst dir
//                  dest_dir_name: name without pahe of dst dir
//  Returns:        pass:0, other:fail
//****************************************************************************/
PUBLIC int pm_decode_zip_by_file(uint16 *src_file_name_ptr,
                                    uint16 *dest_base_dir,
                                    uint16 *dest_dir_name);

/*****************************************************************************/
//  Description:    Unzip single file in zip format file to dst dir.
//  Note:
//  Parameter:      src_zip_file: Input zip format file
//                  decode_file_name: File name for decode.
//                  dest_base_dir: Parent path of dst dir
//                  dest_dir_name: name without pahe of dst dir
//  Returns:        pass:0, other:fail
//****************************************************************************/
PUBLIC int pm_decode_single_file_inzip(wchar *src_zip_file, wchar *decode_file_name,
                                   wchar *dst_dir_path);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End