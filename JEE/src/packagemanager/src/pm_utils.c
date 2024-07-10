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
 * @file     pm_utils.c
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

#include "pm_utils.h"

#include "guilcd.h"

// Adapter interface
#include "pm_adapter_sfs.h"
#include "pm_adapter_zip.h"

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

#define MIN_DISK_SPACE_SIZE (1 * 1024 * 1024) // uint:byte 1M

/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC wchar *wstrncpy(wchar *dst, const wchar *src, size_t count)
{
    wchar *start = dst;
    if (PNULL == src || PNULL == dst)
    {
        return start;
    }

    while ((0 != count) && ('\0' != (*dst++ = *src++))) /* copy string */
    {
        count--;
    }

    if (count) /* pad out with zeroes */
    {
        while (--count)
        {
            *dst++ = '\0';
        }
    }

    return (start);
}

PUBLIC size_t wstrlen(const wchar *str)
{
    const wchar *ustr = str;

    if (PNULL == str)
    {
        return 0;
    }

    while (*ustr++)
    {
        ;
    }

    return ((size_t)(ustr - str - 1));
}

PUBLIC wchar *wstrcat(wchar *dst, const wchar *src)
{
    wchar *cp = dst;

    if (PNULL == src || PNULL == dst)
    {
        return dst;
    }

    while (*cp)
    {
        cp++; /* find end of dst */
    }

    while (PNULL != (*cp++ = *src++))
    {
        ; /* copy src to end of dst */
    }

    return (dst); /* return dst */
}

PUBLIC wchar *wstrncat(wchar *dst, const wchar *src, size_t count)
{
    wchar *start = dst;

    if (PNULL == src || PNULL == dst)
    {
        return start;
    }

    while (*dst++)
    {
        ;
    }

    dst--;

    while (count--)
    {
        if ('\0' == (*dst++ = *src++))
        {
            return (start);
        }
    }

    *dst = 0;

    return (start);
}

PUBLIC int wstrncmp(const wchar *string1, const wchar *string2, size_t count)
{
    int result = 0;
    if (!count)
    {
        return (0);
    }

    if (string1 == PNULL || string2 == PNULL)
    {
        if (string1 == PNULL)
        {
            return -1;
        }
        else if (string2 != PNULL)
        {
            return 1;
        }
        return (0);
    }
    else
    {
        /*lint -save -e613 -e794 */
        while (--count && *string1 && *string1 == *string2)
        {
            string1++;
            string2++;
        }

        result = (*(wchar *)string1 - *(wchar *)string2);
        /*lint -restore */
        return (result > 0 ? 1 : (result < 0 ? -1 : 0));
    }
}

PUBLIC wchar *str_to_wstr(const uint8 *src, wchar *dst)
{
    wchar *return_wstr = dst;

    if (src == PNULL || PNULL == dst)
    {
        return return_wstr;
    }

    while ('\0' != (*dst++ = *src++)) /* copy string */
    {
        ;
    }

    return return_wstr;
}

PUBLIC uint8 *wstr_to_str(const wchar *src, uint8 *dst)
{
    uint8 *return_str = dst;

    if (src == PNULL || PNULL == dst)
    {
        return return_str;
    }

    while (!(*src & 0xFF00) && '\0' != (*dst++ = *src++))
    {
        ;
    }

    return return_str;
}

PUBLIC uint8 *wstrn_to_str(const wchar *src, uint8 *dst, size_t count)
{
    uint8 *return_str = dst;

    if (src == PNULL || PNULL == dst)
    {
        return return_str;
    }

    while (--count && !(*src & 0xFF00) && '\0' != (*dst++ = *src++))
    {
        ;
    }

    return return_str;
}

PUBLIC void split_path_file_name(/*INOUT*/ uint16 *pname, /*OUT*/ uint16 *fname)
{
    uint32 i;
    uint32 k;
    uint32 pos;

    fname[0] = 0;
    pos = 0;

    i = 0;
    k = 0;
    while (0 != pname[i])
    {
        if (((uint16)('/') != pname[i]) && ((uint16)('\\') != pname[i]))
        {
            fname[k++] = pname[i++];
        }
        else
        {
            pos = i++;
            k = 0;
            fname[0] = 0;
        }
    }
    pname[pos] = 0;
    fname[k] = 0;
}

PUBLIC BOOLEAN delete_dir(const uint16 *folder_path_ptr,
                          const uint16 *folder_name_ptr)
{
    BOOLEAN ret = FALSE;
    ret = pm_delete_dir(folder_path_ptr, folder_name_ptr);
    return ret;
}

PUBLIC int64 get_device_free_space(const uint16 *disk_name)
{
    int64 totalSize = 0;
    totalSize = pm_get_device_free_space(disk_name);
    return totalSize;
}

PUBLIC int decode_zip_by_file(uint16 *src_file_name_ptr, uint16 *dest_base_dir,
                              uint16 *dest_dir_name)
{
    int ret = 0; // 0:pass. other:fail

    ret =
        pm_decode_zip_by_file(src_file_name_ptr, dest_base_dir, dest_dir_name);
    return ret;
}

PUBLIC int decode_single_file_inzip(wchar *src_zip_file,
                                    wchar *decode_file_name,
                                    wchar *dst_dir_path)
{
    int ret = 0; // 0:pass. other:fail

    ret = pm_decode_single_file_inzip(src_zip_file, decode_file_name,
                                      dst_dir_path);
    return ret;
}

PUBLIC uint32 path_combine(const uint16 *p_base, const uint16 *p_dir,
                           uint16 *outout_path)
{
    uint32 ret = 0;

    ret = pm_path_combine(p_base, p_dir, outout_path);
    return ret;
}

PUBLIC BOOLEAN check_file_suffix(const uint16 *full_path,
                                 const uint16 *check_suffix_name)
{
    BOOLEAN result = FALSE;
    uint16 path_ptr[SFS_MAX_PATH];
    uint16 name_ptr[SFS_MAX_PATH];
    uint16 suffix_ptr[SFS_MAX_PATH];
    uint16 suffix_len = 0;
    size_t len = SFS_MAX_PATH;

    if (full_path == NULL)
    {
        return FALSE;
    }
    if (check_suffix_name == NULL)
    {
        return FALSE;
    }

    SCI_MEMSET(path_ptr, 0, len); /*lint !e718 */
    SCI_MEMSET(name_ptr, 0, len);
    SCI_MEMSET(suffix_ptr, 0, len);

    wstrncpy(path_ptr, full_path, len);
    split_path_file_name(path_ptr, name_ptr);
    suffix_len = wstrlen(check_suffix_name);
    pm_split_file_name(name_ptr, wstrlen(name_ptr), NULL, NULL, suffix_ptr,
                       &suffix_len);

    if (wstrncmp(check_suffix_name, suffix_ptr, wstrlen(check_suffix_name))
        == 0)
    {
        result = TRUE;
    }
    return result;
}

PUBLIC BOOLEAN check_device_enough_space(uint16 *p_path, uint16 *device_name)
{
    uint32 free_space = 0;
    uint32 file_size = 0;
    BOOLEAN is_enough = TRUE;
    SFS_ERROR_E errCode = SFS_ERROR_NONE;
    SFS_HANDLE sfs_fd_ptr;

    if (device_name == NULL)
    {
        return FALSE;
    }
    if (p_path == NULL)
    {
        return FALSE;
    }

    sfs_fd_ptr = pm_create_or_open_file(p_path, wstrlen(p_path),
                                        SFS_MODE_OPEN_EXISTING | SFS_MODE_READ);
    if (sfs_fd_ptr == NULL)
    {
        return FALSE;
    }

    // Check free disk space
    free_space = (uint32)get_device_free_space(device_name);
    errCode = SFS_GetFileSize(sfs_fd_ptr, &file_size);
    if (free_space < MIN_DISK_SPACE_SIZE + file_size)
    {
        SCI_TRACE_LOW("%s free_space not enough! free_space:%d \n",
                      __FUNCTION__, free_space);
        is_enough = FALSE;
    }
    SFS_CloseFile(sfs_fd_ptr);
    return is_enough;
}

PUBLIC BOOLEAN get_lcd_width_height(uint16 *lcd_width, uint16 *lcd_height)
{
    BOOLEAN ret = FALSE;
    ret = GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, lcd_width, lcd_height);
    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
