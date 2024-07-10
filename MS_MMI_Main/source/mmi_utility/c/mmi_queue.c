/******************************************************************************
 ** File Name:      mmi_link.c                                               *
 ** Author:         gang.tong                                              *
 ** DATE:           05/15/2008                                                *
 ** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block memory including malloc and free              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/15/2008     gang.tong        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_queue.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   create a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC MMI_QUEUE_NODE_T* MMIQUEUE_InitQueue(uint32 max_queue_count, uint32 node_size)
{

	MMI_QUEUE_NODE_T *queue_ptr = (MMI_QUEUE_NODE_T *)SCI_ALLOC_APP(sizeof(MMI_QUEUE_NODE_T));

	if(queue_ptr != PNULL)
	{
		queue_ptr->max_count = max_queue_count;
		queue_ptr->node_size = node_size;
		queue_ptr->current_count = 0;
		queue_ptr->queue = SCI_ALLOC_APP(max_queue_count*node_size); 
		if(queue_ptr->queue != PNULL)
		{
			SCI_MEMSET(queue_ptr->queue, 0x00, (max_queue_count*node_size));
		}
	}
	return queue_ptr;
}

/*****************************************************************************/
//  Description:   destory a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void MMIQUEUE_DestoryQueue(MMI_QUEUE_NODE_T *queue_ptr)
{

	if(queue_ptr != PNULL)
	{
		if(queue_ptr->queue != PNULL)
		{
			SCI_FREE(queue_ptr->queue);
			queue_ptr->queue = PNULL;
		}
		SCI_FREE(queue_ptr);
		queue_ptr = PNULL;
	}
}

/*****************************************************************************/
//  Description:   enter to queue tail
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQUEUE_InQueue(MMI_QUEUE_NODE_T *queue_ptr, void *data)
{

	BOOLEAN result = FALSE;
	uint8*  buf_ptr = PNULL;
	if(queue_ptr != PNULL && (queue_ptr->queue != PNULL))
	{
        if(queue_ptr->current_count < queue_ptr->max_count)
		{
			buf_ptr = (uint8 *)queue_ptr->queue + (queue_ptr->node_size *queue_ptr->current_count);
			if(buf_ptr != PNULL)/*lint !e774*/
			{
				SCI_MEMCPY(buf_ptr, data, queue_ptr->node_size);
				result = TRUE;
				queue_ptr->current_count++;
			}
		}
	}
	return result;
}

/*****************************************************************************/
//  Description:   out of queue head
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void MMIQUEUE_OutQueue(MMI_QUEUE_NODE_T *queue_ptr)
{
	if(queue_ptr != PNULL && (queue_ptr->queue != PNULL))
	{
        if(queue_ptr->current_count > 0)
		{
			queue_ptr->current_count--;
			SCI_MEMCPY(queue_ptr->queue, (uint8 *)queue_ptr->queue + queue_ptr->node_size, queue_ptr->node_size*queue_ptr->current_count);
		}
	}
}

/*****************************************************************************/
//  Description:   out of queue head
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void MMIQUEUE_RemoveNode(MMI_QUEUE_NODE_T *queue_ptr, uint32 index)
{
	if(queue_ptr != PNULL && (queue_ptr->queue != PNULL))
	{
        if(index < queue_ptr->current_count)
		{
			queue_ptr->current_count--;
            if(index < queue_ptr->current_count)
            {
                SCI_MEMCPY((uint8 *)queue_ptr->queue + (queue_ptr->node_size*index), (uint8 *)queue_ptr->queue + (queue_ptr->node_size*(index+1)), queue_ptr->node_size*(queue_ptr->current_count - index));
            }
		}
	}
}

/*****************************************************************************/
//  Description:   get head node of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void* MMIQUEUE_PeekQueue(MMI_QUEUE_NODE_T *queue_ptr, uint32 index)
{

	uint8*  buf_ptr = PNULL;
	if(queue_ptr != PNULL && (queue_ptr->queue != PNULL))
	{
        if(queue_ptr->current_count > 0)
		{
			buf_ptr = (uint8 *)queue_ptr->queue + (queue_ptr->node_size *(index));
		}
	}
	return (void*)buf_ptr;
}

/*****************************************************************************/
//  Description:   get current index of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIQUEUE_GetCurrentIndex(MMI_QUEUE_NODE_T *queue_ptr)
{

	uint32 index = MMIQUEUE_INVALID_INDEX;
	if(queue_ptr != PNULL)
	{
        if(queue_ptr->current_count > 0)
		{
			index = queue_ptr->current_count -1;
		}
	}
	return index;
}

/*****************************************************************************/
//  Description:   get queue index by its node vlude
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIQUEUE_GetNodeIndex(MMI_QUEUE_NODE_T *queue_ptr, void *data, MMIQUEUE_COMPARE_FUNC func)
{
	uint32 i = 0;
	uint32 index = MMIQUEUE_INVALID_INDEX;
	uint8*  buf_ptr = PNULL;
	if(queue_ptr != PNULL && (queue_ptr->queue != PNULL) && (func != PNULL))
	{
        for(i =0; i < queue_ptr->current_count; i++)
		{
			buf_ptr = (uint8 *)queue_ptr->queue + (queue_ptr->node_size *(i));
            if(func(data, buf_ptr))
			{
				index = i;
				break;
			}
		}
	}
	return index;
}

/*****************************************************************************/
//  Description:   is queue empty
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQUEUE_QueueIsEmpty(MMI_QUEUE_NODE_T *queue_ptr)
{

	BOOLEAN result = TRUE;
	if(queue_ptr != PNULL)
	{
        if(queue_ptr->current_count > 0)
		{
			result = FALSE;
		}
	}
	return result;
}

/*****************************************************************************/
//  Description:   is queue full
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQUEUE_QueueIsFull(MMI_QUEUE_NODE_T *queue_ptr)
{

	BOOLEAN result = FALSE;
	if(queue_ptr != PNULL)
	{
        if(queue_ptr->current_count >= queue_ptr->max_count)
		{
			result = TRUE;
		}
	}
	return result;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

