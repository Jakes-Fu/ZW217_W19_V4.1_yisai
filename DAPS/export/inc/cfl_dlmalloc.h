/*****************************************************************************
** File Name:		cfl_dlmalloc.h                                           *
** Author:			bruce.chi                                                *
** Date:			08/03/2009                                               *
** Copyright:		2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:		This file is used to describe the dlmalloc               *
*****************************************************************************/

#ifndef _CFL_DLMALLOC_H_
#define _CFL_DLMALLOC_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//#define DAPS_DLMALLOC_MEMCHECK

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef void* CFL_MSPACE_HANDLE;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Create memory space handle
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: capacity should be larger than [128*sizeof(int32)]
/*****************************************************************************/
PUBLIC CFL_MSPACE_HANDLE CFL_CreateMspace(void* base_ptr, int32 capacity);

/*****************************************************************************/
//  Description : Destroy memory space handle
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: returning the total number of bytes freed
/*****************************************************************************/
PUBLIC int32 CFL_DestroyMspace(CFL_MSPACE_HANDLE msp_handle);

/*****************************************************************************/
//  Description : Memory space alloc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void* CFL_MspaceMalloc(CFL_MSPACE_HANDLE msp_handle, int32 bytes);

/*****************************************************************************/
//  Description : Memory space free
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void CFL_MspaceFree(CFL_MSPACE_HANDLE msp_handle, void* mem_ptr);

/*****************************************************************************/
//  Description : Memory space realloc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void* CFL_MspaceRealloc(CFL_MSPACE_HANDLE msp_handle, void* mem_ptr, int32 newsize);
//sheng.zhu add 

/*****************************************************************************/
//  Description : Get Memory space information
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
typedef struct  
{
    int non_mmapped_space;          /* non-mmapped space allocated from system */
    int n_free_chunks;              /* number of free chunks */
    int mmapped_space;              /* space in mmapped regions */
    int max_total_allocated_space;  /* maximum total allocated space */
    int total_allocated_space;      /* total allocated space */
    int total_free_space;           /* total free space */
    int releaseable_space;          /* releasable (via malloc_trim) space */
}CFL_MspaceInfo_T;
PUBLIC void CFL_MspaceGetInfo(CFL_MSPACE_HANDLE msp_handle, CFL_MspaceInfo_T *info_ptr);

/*****************************************************************************/
//  Description : Create memory space handle
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: capacity should be larger than [128*sizeof(int32)]
/*****************************************************************************/
PUBLIC CFL_MSPACE_HANDLE CFL_CreateMspaceEx(uint32 granularity, uint8 max_blocks);

/*****************************************************************************/
//  Description : Memory space alloc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void* CFL_MspaceMallocEx(CFL_MSPACE_HANDLE msp_info_handle, int32 bytes);

/*****************************************************************************/
//  Description : Memory space free
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void CFL_MspaceFreeEx(CFL_MSPACE_HANDLE msp_info_handle, void* mem_ptr);

/*****************************************************************************/
//  Description : Memory space realloc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void* CFL_MspaceReallocEx(CFL_MSPACE_HANDLE msp_info_handle, void* mem_ptr, int32 newsize);

/*****************************************************************************/
//  Description : Destroy memory space handle
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC int32 CFL_DestroyMspaceEx(CFL_MSPACE_HANDLE msp_info_handle);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
