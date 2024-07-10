/******************************************************************************
 ** File Name:      mmi_mem.c                                               *
 ** Author:         gang.tong                                              *
 ** DATE:           03/13/2008                                                *
 ** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block memory including malloc and free              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/13/2008     gang.tong        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef _MMI_MEM_H_
#define _MMI_MEM_H_


#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define MMIMEM_MEM_MAX                          3
#define MMIMEM_BLOCK_TYPE_MAX                   12
        
typedef struct 
{
    uint16 size;            /* the size of every block space, such as 16 bytes, 32 bytes, and so on. */
    uint16 num;             /* the number of block with this type */
}MMIMEM_BLOCK_TYPE;

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    config the mem block info, memory start address, memory size
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC int16 MMIAPIMEM_Config(uint8 * mem_start_addr, uint32 mem_total_size, MMIMEM_BLOCK_TYPE const * block_type_arr, uint16 type_num);
/*****************************************************************************/
//  Description:  malloc buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPIMEM_Malloc(int16 mem_id, uint32 size);
/*****************************************************************************/
//  Description:  free buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMEM_Free(uint8* ptr);
/*****************************************************************************/
//  Description:  destroy memory manager
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMEM_Destroy(int16 mem_id);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif 


