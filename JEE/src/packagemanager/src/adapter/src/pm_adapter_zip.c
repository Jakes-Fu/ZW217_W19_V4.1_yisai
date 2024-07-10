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
 * @file     pm_adapter_zip.c
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

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "pm_adapter_zip.h"

#include "pm_utils.h"
//#include "unzip_gzip_export.h"
#include "miniunzip.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN (256)
#endif /* MAX_PATH_LEN */

/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
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
                                 uint16 *dest_base_dir, uint16 *dest_dir_name)
{
    int ret = 0; // fail:0. pass:1
    uint16 dst_dir_path[MAX_PATH_LEN] = {0};

    uint8 unzip_src_file[MAX_PATH_LEN] = {0};
    uint8 unzip_dst_dir[MAX_PATH_LEN] = {0};

    path_combine(dest_base_dir, dest_dir_name, dst_dir_path);

    wstr_to_str(src_file_name_ptr, unzip_src_file);
    wstr_to_str(dst_dir_path, unzip_dst_dir);
    // pass:0, fail:other in mini_unzip return!
    ret = mini_unzip((char *)unzip_src_file, (char *)unzip_dst_dir);
    return ret;
}

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
                                   wchar *dst_dir_path)
{
    int ret = 0;

    char unzip_src_file[MAX_PATH_LEN] = {0};
    char unzip_file_name[MAX_PATH_LEN] = {0};
    char unzip_dst_dir[MAX_PATH_LEN] = {0};

    wstr_to_str(src_zip_file, unzip_src_file);
    wstr_to_str(decode_file_name, unzip_file_name);
    wstr_to_str(dst_dir_path, unzip_dst_dir);
    // pass:0, fail:other in mini_unzip_onefile return!
    ret = mini_unzip_single_file((char *)unzip_src_file, (char *)unzip_dst_dir,
                             unzip_file_name);
    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif