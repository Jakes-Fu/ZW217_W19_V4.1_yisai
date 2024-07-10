
/******************************************************************************
 ** File Name:    urjs_art_memory.h                                           *
 ** Author:       jie.yang                                               *
 ** DATE:         2020-12-15                                                    *
 ** Copyright:    2003 Unisoc, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
#ifndef  _JAVASCRIPTART_MEMORY_EXPORT_H_    
#define  _JAVASCRIPTART_MEMORY_EXPORT_H_   


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
		int art_free(void* ptr);
		int jsmempool_destroy(int handle);
		void* art_malloc(int size);
		char* art_strdup(char* str);
		

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
