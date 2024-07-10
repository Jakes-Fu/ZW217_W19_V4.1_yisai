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
 * @file     fs_api.h
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

#ifndef MINIUNZIP_FSAPI_H
#define MINIUNZIP_FSAPI_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "stdio.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cpluspluss
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

FILE *fs_fopen(const char *path, const char *mode);
int fs_fclose(FILE *stream);
int fs_feof(FILE *stream);
size_t fs_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fs_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int fs_fseek(FILE *stream, long offset, int whence);
long fs_ftell(FILE *stream);
void fs_rewind(FILE *stream);

int fs_fileno(FILE *stream);
char *fs_mktemp(char *temp);
void fs_setbuf(FILE *stream, char *buf);
void fs_setbuffer(FILE *stream, char *buf, size_t size);
int fs_fflush(FILE *stream);
FILE *fs_freopen(const char *path, const char *mode, FILE *stream);
FILE *fs_fdopen(int fildes, const char *mode);

// dir
int fs_mkdir(const char *dirname);
int fs_mkdirs(const char *dirname);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End