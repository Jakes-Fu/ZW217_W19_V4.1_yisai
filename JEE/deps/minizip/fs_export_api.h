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
 * @file     fs_export_api.h
 * @brief    -
 * @author   handle
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

#ifndef MINIUNZIP_FS_EXPORTAPI_H
#define MINIUNZIP_FS_EXPORTAPI_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cpluspluss
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

// file
#define fopen(path, mode) fs_##fopen(path, mode)
#define fclose(ptr) fs_##fclose(ptr)
#define feof(ptr) fs_##feof(ptr)
#define fread(ptr, size, nbyte, stream) fs_##fread(ptr, size, nbyte, stream)
#define fwrite(ptr, size, nbyte, stream) fs_##fwrite(ptr, size, nbyte, stream)
#define fseek(stream, offset, whence) fs_##fseek(stream, offset, whence)
#define ftell(stream) fs_##ftell(stream)
#define rewind(stream) fs_##rewind(stream)

#define fileno(stream) fs_##fileno(stream)
#define mktemp(ptr) fs_##mktemp(ptr)
#define setbuf(stream, buf) fs_##setbuf(stream, buf)
#define setbuffe(stream, buf, size) fs_##setbuffe(stream, buf, size)
#define fflush(stream) fs_##fflush(stream)
#define freopen(path, mode, stream) fs_##freopen(path, mode, stream)
#define fdopen(fd, mode) fs_##fdopen(fd, mode)

// dir
#define mkdir(path) fs_##mkdir(path)
#define mkdirs(path) fs_##mkdirs(path)

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End