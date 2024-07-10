/**
 * Copyright (c) 2021, Unisoc Communications Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 **/
/**
 *****************************************************************************
 * @file     fs_api.c
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date     2022-4-15
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
/*lint -save -e746 -e718 -e716 -e628 -e516 -e515 */
#include <string.h>

#include "os_api.h"
#include "fs_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cpluspluss
extern "C" {
#endif

// For lint issue
#ifndef uint16
#define uint16 unsigned short
#endif /* uint16 */

#ifndef int32
#define int32 signed int
#endif /* int32 */

#ifndef uint8
#define uint8 unsigned char
#endif /* uint8 */

/*lint -save -e746 -e718 -e716 -e628 -e516 -e515 */
#include "sfs.h"

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#define FILE_MAX_PATH (256)

/**---------------------------------------------------------------------------*
 **                         Local Functions                                   *
 **---------------------------------------------------------------------------*/

/*******************************************************************/
//  Interface:      str_to_wstr
//  Description : change str to wstr
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
static uint16 *str_to_wstr(const char *src, uint16 *dst)
{
    uint16 *return_wstr = dst;

    if (src == PNULL)
    {
        return return_wstr;
    }

    while ('\0' != (*dst++ = *src++)) /* copy string */
    {
        NULL; /*lint !e522*/
    }

    return return_wstr;
}

/*******************************************************************/
//  Interface:      fs_sflags
//  Description : set fs flag
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
static uint32 fs_sflags(const char *mode)
{
    int ret = 0;
    int m = 0;
    int o = 0;

    switch (*mode++)
    {
    case 'r': /* open for reading */
        m = SFS_MODE_READ;
        o = SFS_MODE_OPEN_EXISTING;
        break;

    case 'w': /* open for writing */
        m = SFS_MODE_WRITE;
        o = SFS_MODE_CREATE_ALWAYS;
        break;

    case 'a': /* open for appending */
        m = SFS_MODE_WRITE;
        o = SFS_MODE_APPEND;
        break;

    default: /* illegal mode */
        return (0);
    }

    /* [rwa]\+ or [rwa]b\+ means read and write */
    if (*mode == '+' || (*mode == 'b' && mode[1] == '+'))
    {
        m = SFS_MODE_READ | SFS_MODE_WRITE;
    }
    ret = m | o;
    return (ret);
}

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

/*******************************************************************/
//  Interface:      fs_fopen
//  Description : fs_fopen
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
FILE *fs_fopen(const char *path, const char *mode)
{
    uint16 w_path[FILE_MAX_PATH + 2] = {0};
    FILE *pfile = 0;

    if (PNULL == path || PNULL == mode)
    {
        return 0;
    }

    str_to_wstr(path, w_path);
    pfile = (FILE *)SFS_CreateFile(w_path, fs_sflags(mode), 0, 0);

    return pfile;
}

/*******************************************************************/
//  Interface:      fs_fclose
//  Description : fs_fclose
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
int fs_fclose(FILE *stream)
{
    SFS_ERROR_E sfs_ret;
    if (NULL == stream)
    {
        return -1;
    }
    sfs_ret = SFS_CloseFile(((SFS_HANDLE)stream));
    return sfs_ret;
}

/*******************************************************************/
//  Interface:      fs_feof
//  Description :   fs_feof
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
int fs_feof(FILE *stream)
{
    uint32 length = 0;
    int32 pos = 0;

    if (NULL == stream)
    {
        return 0;
    }

    if (0 == SFS_GetFileSize((SFS_HANDLE)stream, &length))
    {
        if (0 == SFS_GetFilePointer((SFS_HANDLE)stream, SFS_SEEK_BEGIN, &pos))
        {
            if (length == (uint32)pos)
            {
                return 1;
            }
        }
    }

    return 0;
}

/*******************************************************************/
//  Interface:      fs_fread
//  Description : fs_fread
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
size_t fs_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    uint32 bytes_read = 0;

    if (NULL == ptr || NULL == stream)
    {
        return 0;
    }

    if (SFS_NO_ERROR
        == SFS_ReadFile((SFS_HANDLE)stream, ptr, (size * nmemb), &bytes_read,
                        NULL))
    {
        return (bytes_read / (uint32)size);
    }

    return 0;
}

/*******************************************************************/
//  Interface:      fs_fwrite
//  Description : fs_fwrite
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
size_t fs_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    uint32 bytes_write = 0;

    if (NULL == ptr || NULL == stream)
    {
        return 0;
    }

    if (SFS_NO_ERROR
        == SFS_WriteFile((SFS_HANDLE)stream, ptr, (size * nmemb), &bytes_write,
                         NULL))
    {
        return (bytes_write / (uint32)size);
    }

    return 0;
}

/*******************************************************************/
//  Interface:      fs_fseek
//  Description : fs_fseek
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
int fs_fseek(FILE *stream, long offset, int whence)
{
    if (NULL == stream)
    {
        return (-1);
    }
    return SFS_SetFilePointer((SFS_HANDLE)stream, (__int64)offset, whence);
}

/*******************************************************************/
//  Interface:      fs_ftell
//  Description : fs_ftell
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
long fs_ftell(FILE *stream)
{
    int32 postion = 0;

    if (NULL == stream)
    {
        return (-1);
    }
    if (SFS_NO_ERROR
        == SFS_GetFilePointer((SFS_HANDLE)stream, SFS_SEEK_BEGIN, &postion))
    {
        return postion;
    }

    return 0;
}

/*******************************************************************/
//  Interface:      fs_rewind
//  Description : fs_rewind
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
void fs_rewind(FILE *stream)
{
    SFS_SetFilePointer((SFS_HANDLE)stream, (__int64)0, SFS_SEEK_BEGIN);
}

/*******************************************************************/
//  Interface:      fs_fileno
//  Description : fs_fileno
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
int fs_fileno(FILE *stream)
{
    // SCI_ASSERT(FALSE);
    return 0;
}

/*******************************************************************/
//  Interface:      fs_mktemp
//  Description : fs_mktemp
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
char *fs_mktemp(char *temp)
{
    // SCI_ASSERT(FALSE);
    return 0;
}
/*******************************************************************/
//  Interface:      fs_setbuf
//  Description : fs_setbuf
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
void fs_setbuf(FILE *stream, char *buf)
{
    // SCI_ASSERT(FALSE);
}

/*******************************************************************/
//  Interface:      fs_setbuffer
//  Description : fs_setbuffer
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
void fs_setbuffer(FILE *stream, char *buf, size_t size)
{
    // SCI_ASSERT(FALSE);
}

/*******************************************************************/
//  Interface:      fs_fflush
//  Description : fs_fflush
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
int fs_fflush(FILE *stream)
{
    // SCI_ASSERT(FALSE);
    return 0;
}

/*******************************************************************/
//  Interface:      fs_freopen
//  Description : fs_freopen
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
FILE *fs_freopen(const char *path, const char *mode, FILE *stream)
{
    // SCI_ASSERT(FALSE);
    return 0;
}

/*******************************************************************/
//  Interface:      fs_fdopen
//  Description : fs_fdopen
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
FILE *fs_fdopen(int fildes, const char *mode)
{
    // SCI_ASSERT(FALSE);
    return 0;
}

/*******************************************************************/
//  Interface:      fs_mkdir
//  Description : fs_mkdir
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
int fs_mkdir(const char *dirname)
{
    SFS_ERROR_E sfs_ret = SFS_NO_ERROR;
    uint16 w_path[FILE_MAX_PATH * 2] = {0};
    char path[FILE_MAX_PATH] = {0};
    int dirname_len = 0;

    if (NULL == dirname)
    {
        return -1;
    }
    dirname_len = strlen(dirname);
    if (dirname_len <= 0)
    {
        return -1;
    }
    strncpy(path, dirname, dirname_len);
    dirname_len = strlen(path);
    if (dirname_len <= 0)
    {
        return -1;
    }
    // check last char if '\\' or '/', replace to '\0'
    if ((path[dirname_len - 1] == '\\') || (path[dirname_len - 1] == '/'))
    {
        path[dirname_len - 1] = '\0';
    }
    str_to_wstr(path, w_path);
    sfs_ret = SFS_CreateDirectory(w_path); /*lint !e64 */
    return sfs_ret;
}

/*******************************************************************/
//  Interface:      fs_mkdirs
//  Description : fs_mkdirs
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*******************************************************************/
int fs_mkdirs(const char *newdir)
{
    char *buffer = NULL;
    char *p = NULL;
    if (NULL == newdir)
    {
        return 0;
    }
    int len = strlen(newdir);
    if (len <= 0)
    {
        return 0;
    }
    buffer = (char *)SCI_ALLOC_APP(len + 1);
    if (buffer == NULL)
    {
        SCI_TRACE_LOW("Error allocating memory\n");
        return -1;
    }
    strcpy(buffer, newdir); /*lint !e628 */

    if (buffer[len - 1] == '/')
    {
        buffer[len - 1] = '\0';
    }
    if (fs_mkdir(buffer) == SFS_NO_ERROR)
    {
        SCI_FREE(buffer);
        return 0;
    }

    p = buffer + 1;
    while (1)
    {
        char hold;

        while (*p && *p != '\\' && *p != '/')
        {
            p++;
        }

        hold = *p;
        *p = 0;
        if ((fs_mkdir(buffer) != SFS_NO_ERROR))
        {
            SCI_TRACE_LOW("couldn't create directory %s\n", buffer);
            SCI_FREE(buffer);
            return 0;
        }
        if (hold == 0)
        {
            break;
        }
        *p++ = hold;
    }
    SCI_FREE(buffer);
    return 1;
}
/*lint -restore */

/**---------------------------------------------------------------------------*

 * **                         Compiler Flag *

 * **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif