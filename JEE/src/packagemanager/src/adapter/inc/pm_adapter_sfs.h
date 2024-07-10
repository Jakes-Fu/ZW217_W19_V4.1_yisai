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
 * @file     pm_adapter_sfs.h
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

#ifndef PM_ADAPTER_SFS_H
#define PM_ADAPTER_SFS_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sfs.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define PM_CREATE_FILE SFS_CreateFile
#define PM_WRITE_FILE  SFS_WriteFile 
#define PM_CLOSE_FILE  SFS_CloseFile 
#define PM_FIND_FIRST_FILE SFS_FindFirstFile
#define PM_FIND_CLOSE  SFS_FindClose
#define PM_DELETE_FILE SFS_DeleteFile
#define PM_DELETE_DIRECTORY SFS_DeleteDirectory

//For lint issue
#ifndef uint16
#define uint16  unsigned short
#endif /* uint16 */

#ifndef wchar
#define wchar  unsigned short
#endif /* wchar */

#ifndef wchar_t
#define wchar_t  unsigned short
#endif /* wchar_t */

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC int64 pm_get_device_free_space(const uint16 *disk_name);

PUBLIC BOOLEAN pm_delete_dir(const uint16 *folder_path_ptr,const uint16 *folder_name_ptr);

PUBLIC SFS_ERROR_E pm_create_dir(uint16* dir_path);

PUBLIC SFS_HANDLE pm_create_or_open_file(uint16 *urar_src_file_path, int file_path_len, uint32 access_mode);

PUBLIC SFS_ERROR_E pm_write_file(SFS_HANDLE sfs_fd_ptr, void* buff_write, uint32 byte_to_write, uint32 *bytes_written);

PUBLIC SFS_ERROR_E pm_read_file(SFS_HANDLE sfs_fd_ptr, void *read_bytes, uint32 bytes_to_read, uint32 *bytes_readed, int64 seek_offset);

PUBLIC uint32 pm_path_combine(const uint16 *p_base, const uint16 *p_dir, uint16 *outout_path);

BOOLEAN pm_copy_tree(uint16 *dstDir, uint16 *srcDir);

BOOLEAN pm_copy_file(uint16 *dst, uint16 *src);

BOOLEAN pm_is_file_exist (
    const wchar *full_path_ptr,     //in
    uint16       full_path_len,       //in
	BOOLEAN is_file
);

BOOLEAN pm_split_file_name (
        const wchar      *file_name_ptr,     //in
        uint16           file_name_len,      //in
        wchar            *name_ptr,          //out, one of name_ptr and suffix_ptr can be NULL
        uint16           *name_len_ptr,      //out
        wchar            *suffix_ptr,        //out
        uint16           *suffix_len_ptr     //in/out
);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End