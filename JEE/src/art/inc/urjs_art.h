
/******************************************************************************
 ** File Name:    urjs_art.h                                                 *
 ** Author:       jie.yang                                               *
 ** DATE:         2021-11-16                                                    *
 ** Copyright:    2003 Unisoc, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
#ifndef  _JS_ART_H_
#define  _JS_ART_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define _DEBUG_LOG

#define MAX_PATH 256

#define APP_ID_LENGTH (128)


#define	aloge(fmt, ...)   SCI_TRACE_LOW(" JSR Error  :  " fmt, ##__VA_ARGS__)

#ifdef _DEBUG_LOG
#define alogd(fmt, ...)   SCI_TRACE_LOW(" JSR Debug  :  " fmt, ##__VA_ARGS__)
#define alogi(fmt, ...)   SCI_TRACE_LOW(" JSR Info   :  " fmt, ##__VA_ARGS__)
#else
#define alogi(fmt, ...)
#define alogd(fmt, ...)
#endif


typedef void* (*JSART_MALLOC)(int size);
typedef int (*JSART_FREE)(void* prt);


int art_open_js_file(char* full_path_ptr, uint32 mode);
int art_read_js_file(int fd, char* data_buf_ptr, int rsize);
int art_get_js_file_size(int fd);
int art_close_js_file(int fd);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
