
/*******************************************************************************

Copyright (C) 2000-2007 Beijing Komoxo Inc.
All rights reserved.
     
*******************************************************************************/

#ifndef IIMESTD_H
#define IIMESTD_H

#ifdef _MSC_VER
    /* Disable Microsoft Visial C++ warning: unreferenced formal parameter */
    #pragma warning(disable: 4100)
    /* Disable Microsoft Visial C++ warning: unreferenced local function has been removed */
    #pragma warning(disable: 4505)
#endif
#if 1
#include "IIMEk2api.h"
#else
#include "inc/IIMEk2api.h"
#endif
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Basic type defines                                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#ifndef IIME_NULL
#ifdef  __cplusplus
#define IIME_NULL    0
#else
#define IIME_NULL    ((void *)0)
#endif
#endif

/* Forward decleration helper */

#define IIME_FORWARD_STRUCT(s)    \
    struct tag##s;              \
    typedef struct tag##s s

#ifndef __cplusplus
    #define inline
#endif

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Error code defines                                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define IIME_ERROR_NOMOREPAGE                 0x00000400

/* 添加未响应原因 */
#define IIME_ERROR_MAGIC_ONE                  0x00000802
#define IIME_ERROR_RETURN_LAST_PROVIDER       0x00000803

#include "IIMEstring.h"
#include "IIMEmemory.h"

#include <string.h>

#ifdef NDEBUG
    #define IIME_ASSERT(e)   ((void)0)
#else
    #include <assert.h>
    #define IIME_ASSERT      assert
#endif

#endif /* IIMESTD_H */
