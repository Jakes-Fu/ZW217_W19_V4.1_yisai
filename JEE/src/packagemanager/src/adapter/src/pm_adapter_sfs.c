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
 * @file     pm_adapter_sfs.c
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
#include "pm_adapter.h"
#include "pm_adapter_sfs.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Macor Variables                                  *
 **---------------------------------------------------------------------------*/
/* string */
#define MAX_PATH_LEN                                                           \
    (SFS_MAX_PATH)             /* max path length, equal to SFS_MAX_PATH       \
                                */
#define PATH_SEPARTOR (0x005c) /* path separtor '\' */
#define STRING_END '\0'        /* string end */

/* string "\" */
#define MMIFILE_SLASH PATH_SEPARTOR

/* string "." */
#define MMIFILE_DOT 0x002e

#define FILECOPY_BYTES_LOOP (20 * 1024)

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
 **---------------------------------------------------------------------------*/

LOCAL size_t sfs_wstrlen(const wchar *str);
LOCAL wchar *sfs_wstrcat(wchar *dst, const wchar *src);
LOCAL wchar *sfs_wstrncpy(wchar *dst, const wchar *src, size_t count);

LOCAL size_t sfs_wstrlen(const wchar *str)
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

LOCAL wchar *sfs_wstrcat(wchar *dst, const wchar *src)
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

LOCAL wchar *sfs_wstrncpy(wchar *dst, const wchar *src, size_t count)
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

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC BOOLEAN pm_delete_dir(const uint16 *folder_path_ptr,
                             const uint16 *folder_name_ptr)
{
    SFS_HANDLE sfs_handle = 0;
    uint16 *full_path_ptr = 0;
    uint16 *find_path_ptr = 0;
    SFS_FIND_DATA_T fd = {0};
    uint16 slash_name[] = L"\\";
    uint16 filter[] = L"*";

    if (folder_path_ptr != PNULL)
    {
        full_path_ptr = (uint16 *)SCI_ALLOC(SFS_MAX_PATH * 2);
        if (full_path_ptr == PNULL)
        {
            PM_TRACE_LOW("full_path_ptr fail!");
            return FALSE;
        }

        find_path_ptr = (uint16 *)SCI_ALLOC(SFS_MAX_PATH * 2);
        if (find_path_ptr == PNULL)
        {
            PM_TRACE_LOW("find_path_ptr fail!");
            SCI_FREE(full_path_ptr);
            return FALSE;
        }

        SCI_MEMSET(full_path_ptr, 0, SFS_MAX_PATH * 2); /*lint !e718 */
        SCI_MEMSET(find_path_ptr, 0, SFS_MAX_PATH * 2);

        sfs_wstrcat(full_path_ptr, folder_path_ptr);
        if (full_path_ptr[sfs_wstrlen(full_path_ptr) - 1] != slash_name[0])
        {
            sfs_wstrcat(full_path_ptr, slash_name);
        }
        sfs_wstrcat(full_path_ptr, folder_name_ptr);

        sfs_wstrcat(find_path_ptr, full_path_ptr);
        if (find_path_ptr[sfs_wstrlen(find_path_ptr) - 1] != slash_name[0])
        {
            sfs_wstrcat(find_path_ptr, slash_name);
        }
        sfs_wstrcat(find_path_ptr, filter);
        sfs_handle = SFS_FindFirstFile(find_path_ptr, &fd);
        if (sfs_handle != 0)
        {
            do
            {
                if ((fd.attr & SFS_ATTR_DIR) > 0)
                {
                    // delete folder recursion
                    if (!pm_delete_dir(full_path_ptr, fd.name))
                    {
                        SFS_FindClose(sfs_handle);
                        SCI_FREE(full_path_ptr);
                        SCI_FREE(find_path_ptr);
                        return FALSE;
                    }
                }
                else // delete file find
                {
                    uint16 *temp_path_ptr =
                        (uint16 *)SCI_ALLOC(SFS_MAX_PATH * 2);
                    if (temp_path_ptr == 0)
                    {
                        SFS_FindClose(sfs_handle);
                        SCI_FREE(full_path_ptr);
                        SCI_FREE(find_path_ptr);
                        return FALSE;
                    }
                    SCI_MEMSET(temp_path_ptr, 0, SFS_MAX_PATH * 2);
                    sfs_wstrcat(temp_path_ptr, full_path_ptr);
                    if (temp_path_ptr[sfs_wstrlen(temp_path_ptr) - 1]
                        != slash_name[0])
                    {
                        sfs_wstrcat(temp_path_ptr, slash_name);
                    }

                    sfs_wstrcat(temp_path_ptr, fd.name);
                    if (SFS_NO_ERROR != SFS_DeleteFile(temp_path_ptr, 0))
                    {
                        SFS_FindClose(sfs_handle);
                        SCI_FREE(full_path_ptr);
                        SCI_FREE(find_path_ptr);
                        SCI_FREE(temp_path_ptr);
                        return FALSE;
                    }
                    SCI_FREE(temp_path_ptr);
                }
            } while (SFS_NO_ERROR == SFS_FindNextFile(sfs_handle, &fd));
            SFS_FindClose(sfs_handle);
            // delete the folder input
            if (SFS_NO_ERROR != SFS_DeleteDirectory(full_path_ptr))
            {
                SFS_FindClose(sfs_handle);
                SCI_FREE(full_path_ptr);
                SCI_FREE(find_path_ptr);
                return FALSE;
            }
        }
        else // empty folder, delete folder directly
        {
            SFS_FindClose(sfs_handle);
            if (SFS_NO_ERROR != SFS_DeleteDirectory(full_path_ptr))
            {
                SFS_FindClose(sfs_handle);
                SCI_FREE(full_path_ptr);
                SCI_FREE(find_path_ptr);
                return FALSE;
            }
        }
        SCI_FREE(full_path_ptr);
        SCI_FREE(find_path_ptr);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

PUBLIC int64 pm_get_device_free_space(const uint16 *disk_name)
{
    uint32 fs_hsize = 0;
    uint32 fs_lsize = 0;

    if (SFS_GetDeviceFreeSpace(disk_name, (uint32 *)&fs_hsize,
                               (uint32 *)&fs_lsize)
        != SFS_NO_ERROR)
    {
        return -1;
    }
    return (int64)(((int64)fs_hsize << 32) | fs_lsize);
}

PUBLIC SFS_ERROR_E pm_create_dir(uint16 *dir_path)
{
    if (NULL == dir_path)
    {
        return -1;
    }

    SFS_ERROR_E errCode = SFS_CreateDirectory(dir_path);
    return errCode;
}

PUBLIC SFS_HANDLE pm_create_or_open_file(uint16 *urar_src_file_path,
                                         int file_path_len, uint32 access_mode)
{
    SFS_HANDLE sfs_fd_ptr = 0;

    sfs_fd_ptr =
        SFS_CreateFile((uint16 *)urar_src_file_path, access_mode, NULL, NULL);
    if (sfs_fd_ptr == SCI_NULL)
    {
        PM_TRACE_LOW("SFS_CreateFile FAIL!");
        return NULL;
    }
    else
    {
        return sfs_fd_ptr;
    }
}

PUBLIC SFS_ERROR_E pm_write_file(SFS_HANDLE sfs_fd_ptr, void *buff_write,
                                 uint32 byte_to_write, uint32 *bytes_written)
{
    SFS_ERROR_E result = SFS_ERROR_INVALID_HANDLE;

    if (sfs_fd_ptr == SCI_NULL)
    {
        PM_TRACE_LOW("%s sfs_fd_ptr SCI_NULL!\n", __FUNCTION__);
        return SFS_ERROR_INVALID_HANDLE;
    }
    if (byte_to_write == 0)
    {
        PM_TRACE_LOW("%s Wrong byte_to_write=%d \n", __FUNCTION__,
                     byte_to_write);
        return SFS_ERROR_SYSTEM;
    }

    result = SFS_WriteFile(sfs_fd_ptr, buff_write, byte_to_write, bytes_written,
                           NULL);
    if (result == SFS_NO_ERROR)
    {
        SFS_FlushFile(sfs_fd_ptr, NULL);
    }
    return result;
}

PUBLIC SFS_ERROR_E pm_read_file(SFS_HANDLE sfs_fd_ptr, void *read_bytes,
                                uint32 bytes_to_read, uint32 *bytes_readed,
                                int64 seek_offset)
{
    SFS_ERROR_E result = SFS_ERROR_INVALID_HANDLE;
    uint32 file_size = 0;

    if (sfs_fd_ptr == SCI_NULL)
    {
        PM_TRACE_LOW("%s sfs_fd_ptr SCI_NULL!\n", __FUNCTION__);
        return SFS_ERROR_INVALID_HANDLE;
    }
    else
    {
        SFS_SetFilePointer(sfs_fd_ptr, (seek_offset < 0) ? 0 : seek_offset,
                           SFS_SEEK_BEGIN);
        SFS_GetFileSize(sfs_fd_ptr, &file_size);
        if (file_size > 0 && bytes_to_read > 0)
        {
            result = SFS_ReadFile(sfs_fd_ptr, read_bytes, bytes_to_read,
                                  bytes_readed, NULL);
        }
    }
    return result;
}

/**************************************************************************
 * pm_path_combine
 * Combine base and dir together for output
 *
 * return          0     Error
 *                >0     The length of combined path, exclude'\0'
 **************************************************************************/
uint32 pm_path_combine(const uint16 *p_base, const uint16 *p_dir,
                       uint16 *outout_path)
{
    const uint16 *p;
    uint32 i = 0;

    /* return fail if either string is null */
    if (p_base == NULL || p_dir == NULL)
    {
        return 0;
    }
    /* Get and check length */
    p = p_base;
    while (i < SFS_MAX_PATH && *p != '\0')
    {
        outout_path[i++] = *p++;
    }
    if (i >= SFS_MAX_PATH)
    {
        return 0;
    }
    if (outout_path[i - 1] != PATH_SEPARTOR)
    {
        outout_path[i++] = PATH_SEPARTOR;
    }
    p = p_dir;
    while (i < SFS_MAX_PATH && *p != '\0')
    {
        outout_path[i++] = *p++;
    }
    if (i >= SFS_MAX_PATH)
    {
        return 0;
    }
    /* Here must set string end */
    outout_path[i] = STRING_END;

    // outout_path[i++] = PATH_SEPARTOR;
    /* base + dir shoule less than MAX_PATH_LEN */
    if (i >= SFS_MAX_PATH)
    {
        return 0;
    }

    return i;
}

BOOLEAN pm_copy_file(uint16 *dst, uint16 *src)
{
    SFS_HANDLE fpr = PNULL;
    SFS_HANDLE fpw = PNULL;
    SFS_ERROR_E fError = SFS_NO_ERROR;
    uint32 fileSize = 0;
    uint32 dwCopyNum = 0;
    uint8 *lpBuff = NULL;

    fpr = SFS_CreateFile(src, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL,
                         PNULL);
    if (fpr == NULL)
    {
        return FALSE;
    }

    fpw = SFS_CreateFile(dst, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, PNULL,
                         PNULL);
    if (fpw == NULL)
    {
        SFS_CloseFile(fpr);
        return FALSE;
    }

    fError = SFS_GetFileSize(fpr, &fileSize);
    if (fileSize == 0)
    {
        SFS_CloseFile(fpr);
        SFS_CloseFile(fpw);

        return FALSE;
    }

    lpBuff = (uint8 *)SCI_ALLOC_APP(FILECOPY_BYTES_LOOP);
    if (lpBuff == NULL)
    {
        SFS_CloseFile(fpr);
        SFS_CloseFile(fpw);

        return FALSE;
    }

    memset(lpBuff, 0, FILECOPY_BYTES_LOOP);

    dwCopyNum = fileSize;
    while (dwCopyNum > 0)
    {
        uint32 dwToRW =
            (dwCopyNum > FILECOPY_BYTES_LOOP) ? FILECOPY_BYTES_LOOP : dwCopyNum;
        uint32 dwRW = 0;
        memset(lpBuff, 0, FILECOPY_BYTES_LOOP);
        fError = SFS_ReadFile(fpr, lpBuff, dwToRW, &dwRW, NULL); /*lint !e64 */
        if (dwToRW != dwRW)
        {
            break;
        }

        fError = SFS_WriteFile(fpw, lpBuff, dwToRW, &dwRW, NULL); /*lint !e64 */
        if (dwToRW != dwRW)
        {
            break;
        }

        dwCopyNum -= dwToRW;
    }

    SCI_FREE(lpBuff);
    lpBuff = NULL;

    SFS_FlushFile(fpw, NULL);

    SFS_CloseFile(fpr);
    SFS_CloseFile(fpw);

    if (dwCopyNum != 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOLEAN pm_copy_tree(uint16 *dstDir, uint16 *srcDir)
{
    wchar strDest[SFS_MAX_PATH] = {0};
    wchar strSour[SFS_MAX_PATH] = {0};
    wchar find_path[SFS_MAX_PATH] = {0};
    SFS_HANDLE fp = NULL;
    SFS_FIND_DATA_T findData = {0};
    SFS_ERROR_E fError = SFS_NO_ERROR;
    BOOLEAN bRes = TRUE;

    if (dstDir == NULL || srcDir == NULL)
    {
        return FALSE;
    }

    SCI_MEMSET(find_path, 0, SFS_MAX_PATH);
    sfs_wstrncpy(find_path, srcDir, sfs_wstrlen(srcDir));
    sfs_wstrcat(find_path, L"\\");
    sfs_wstrcat(find_path, L"*");

    fp = SFS_FindFirstFile(find_path, &findData);
    if (fp == NULL)
    {
        return FALSE;
    }

    while (TRUE) // lint !e716
    {
        SCI_MEMSET(strSour, 0, SFS_MAX_PATH);
        sfs_wstrncpy(strSour, srcDir, sfs_wstrlen(srcDir));
        sfs_wstrcat(strSour, L"\\");
        sfs_wstrcat(strSour, findData.name);

        SCI_MEMSET(strDest, 0, SFS_MAX_PATH);
        sfs_wstrncpy(strDest, dstDir, sfs_wstrlen(dstDir));
        sfs_wstrcat(strDest, L"\\");
        sfs_wstrcat(strDest, findData.name);

        if ((findData.attr & SFS_ATTR_DIR) > 0)
        {
            // Make dir here fix empty dir issue
            SFS_CreateDirectory(strDest); /*lint !e64 */
            if (!pm_copy_tree(strDest, strSour))
            {
                bRes = FALSE;
                // break;
            }
        }
        else
        {
            if (!pm_copy_file(strDest, strSour))
            {
                bRes = FALSE;
                // break;
            }
        }

        fError = SFS_FindNextFile(fp, &findData);
        if (fError != SFS_NO_ERROR)
        {
            bRes = FALSE;
            break;
        }
    }

    SFS_FindClose(fp);
    fp = NULL;

    return bRes;
}

/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN pm_is_file_exist(const wchar *full_path_ptr, // in
                         uint16 full_path_len,       // in
                         BOOLEAN is_file)
{
    BOOLEAN result = FALSE;
    SFS_HANDLE sfs_handle = 0;
    SFS_FIND_DATA_T find_data = {0};

    if (PNULL == full_path_ptr)
    {
        return FALSE;
    }

    if (0 == sfs_wstrlen(full_path_ptr))
    {
        result = FALSE;
    }
    else
    {
        sfs_handle = SFS_FindFirstFile(full_path_ptr, &find_data);

        if (0 != sfs_handle)
        {
            if (is_file)
            {
                if (!(SFS_ATTR_DIR & find_data.attr))
                {
                    result = TRUE;
                }
            }
            else
            {
                if ((SFS_ATTR_DIR & find_data.attr))
                {
                    result = TRUE;
                }
            }
        }
        SFS_FindClose(sfs_handle);
    }
    return result;
}

BOOLEAN pm_split_file_name(
    const wchar *file_name_ptr, // in
    uint16 file_name_len,       // in
    wchar *name_ptr,       // out, one of name_ptr and suffix_ptr can be NULL
    uint16 *name_len_ptr,  // out
    wchar *suffix_ptr,     // out
    uint16 *suffix_len_ptr // in/out
)
{
    BOOLEAN result = FALSE;
    uint16 wchar_value = 0;
    uint16 suffix_len = 0;
    int32 i = 0;
    BOOLEAN is_dot_file = FALSE; //".mp3 ...."

    if (NULL == file_name_ptr || 0 == file_name_len)
    {
        return FALSE;
    }

    // find DOT in file_name_ptr
    for (i = file_name_len - 1; i >= 0; i--)
    {
        wchar_value = *(file_name_ptr + i);

        if (MMIFILE_SLASH == wchar_value)
        {
            // this file no suffix name
            break;
        }

        if (MMIFILE_DOT == wchar_value)
        {
            is_dot_file = TRUE;
            suffix_len = file_name_len - i - 1;
            break;
        }
    }

    if (!is_dot_file)
    {
        i = file_name_len;
    }
    else if (suffix_len > *suffix_len_ptr)
    {
        // suffix name too long, short it.
        i = file_name_len - *suffix_len_ptr - 1;
    }

    if (PNULL != suffix_len_ptr)
    {
        // reset suffix len
        *suffix_len_ptr = 0;
    }

    if ((0 < i && i <= file_name_len) || is_dot_file)
    {
        if (NULL != name_ptr && NULL != name_len_ptr)
        {
            sfs_wstrncpy(name_ptr, file_name_ptr, i);

            *name_len_ptr = (uint16)i;
        }

        if (NULL != suffix_ptr && NULL != suffix_len_ptr
            && i < file_name_len - 1)
        {
            sfs_wstrncpy(suffix_ptr, file_name_ptr + i + 1,
                         file_name_len - i - 1);

            *suffix_len_ptr = (uint16)(file_name_len - i - 1);
        }

        result = TRUE;
    }

    return result;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif