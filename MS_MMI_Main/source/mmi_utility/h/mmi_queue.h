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
#ifndef _MMI_QUEUE_H_
#define _MMI_QUEUE_H_

#include "sci_types.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 
#define MMIQUEUE_INVALID_INDEX         0xFFFFFFFF//无效的索引号

typedef struct
{
    uint32 max_count;
	uint32 current_count;
	uint32 node_size;
    void *queue;    
}MMI_QUEUE_NODE_T;

typedef  BOOLEAN (*MMIQUEUE_COMPARE_FUNC)(void *data_ptr1, void *data_ptr2);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   create a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC MMI_QUEUE_NODE_T* MMIQUEUE_InitQueue(uint32 max_queue_count, uint32 node_size);

/*****************************************************************************/
//  Description:   destory a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void MMIQUEUE_DestoryQueue(MMI_QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   enter to queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQUEUE_InQueue(MMI_QUEUE_NODE_T *queue_ptr, void *data);

/*****************************************************************************/
//  Description:   out of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void MMIQUEUE_OutQueue(MMI_QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   out of queue head
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void MMIQUEUE_RemoveNode(MMI_QUEUE_NODE_T *queue_ptr, 
								uint32 index);

/*****************************************************************************/
//  Description:   get current node of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void* MMIQUEUE_PeekQueue(MMI_QUEUE_NODE_T *queue_ptr, uint32 index);

/*****************************************************************************/
//  Description:   get current index of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIQUEUE_GetCurrentIndex(MMI_QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   get queue index by its node vlude
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIQUEUE_GetNodeIndex(MMI_QUEUE_NODE_T *queue_ptr, 
									void *data, 
									MMIQUEUE_COMPARE_FUNC func);

/*****************************************************************************/
//  Description:   is queue empty
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQUEUE_QueueIsEmpty(MMI_QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   is queue full
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQUEUE_QueueIsFull(MMI_QUEUE_NODE_T *queue_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif 
