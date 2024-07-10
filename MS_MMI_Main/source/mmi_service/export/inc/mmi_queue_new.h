/********************************************************************************
** File Name:    mmi_queue.h                                                    *
** Author:       Hongbing Ju                                                    *
** Date:         06/25/2012                                                     *
** Copyright:    2003 Spreadtrum Communications Incorporated. All Right Reserved*
** Description:  This file is used to describe data structure and API of dual   *
**               direction linked list                                          *
*********************************************************************************
**                            Important Edit history                            *
**-----------------------------------------------------------------------------**
** DATE            NAME            DESCRIPTION                                  *
** 06/25/2012      Hongbing Ju      Created                                     *
*********************************************************************************/

#ifndef __MMI_QUEUE_H__
#define __MMI_QUEUE_H__

/*------------------------------------------------------------------------------*
**                            DEPENDENCY                                        *
**------------------------------------------------------------------------------*/
#include "mmi_base_common.h"
#include "mmi_dlist.h"
/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_QUEUE_ERROR_NUMBER(err)  ((err) & 0xFF)
#define MMI_QUEUE_ERROR_LOCATION(err)(((err) & 0xFFFFFF00) >> 8)

/*------------------------------------------------------------------------------*
**                            TYPE AND DEFINITIONS                              *
**  define opaque data structure to hide its detailed information from external *
**  programmer                                                                  *
**------------------------------------------------------------------------------*/
#define MMI_QUEUE_Create            MMI_DLIST_Create
#define MMI_QUEUE_Destroy           MMI_DLIST_Destroy
#define MMI_QUEUE_PostInFront       MMI_DLIST_Prepend
#define MMI_QUEUE_Post              MMI_DLIST_Append
#define MMI_QUEUE_Retrieve          MMI_DLIST_Retrieve
#define MMI_QUEUE_Peek              MMI_DLIST_Peek
#define MMI_QUEUE_Clear             MMI_DLIST_RemoveAll
#define MMI_QUEUE_IsEmpty           MMI_DLIST_IsEmpty
#define MMI_QUEUE_GetDepth          MMI_DLIST_GetLength
#define MMI_QUEUE_GetElement        MMI_DLIST_GetElement
#define MMI_QUEUE_Traverse          MMI_DLIST_Traverse
#define MMI_QUEUE_GetCurrErrorCode  MMI_DLIST_GetCurrErrorCode
#define MMI_QUEUE_TrvFunc           MMI_DLIST_TrvFunc    

typedef struct MMI_DLIST_TAG *      MMI_QUEUEHANDLE_T;
typedef struct MMI_DLISTNODE_TAG *  MMI_QUEUENODEHANDLE_T;

enum
{
    MMI_QUEUE_ERROR_NULL_ELEMENT = 1,
    MMI_QUEUE_ERROR_EMPTY,
    MMI_QUEUE_ERROR_OVERFLOW,
    MMI_QUEUE_ERROR_UNDERFLOW,
    MMI_QUEUE_ERROR_OUT_OF_MEMORY,
};

/*------------------------------------------------------------------------------*
**                            FUNCTION DEFINITIONS                              *
**------------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to create a list                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_QUEUEHANDLE_T MMI_QUEUE_Create(int32 capacity, int32 elementSize);

/********************************************************************************
** Description: This function is used to destroy a list                         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_QUEUE_Destroy(MMI_QUEUEHANDLE_T *queue);

/********************************************************************************
** Description: This function is used to append an element                      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_QUEUENODEHANDLE_T MMI_QUEUE_Post(MMI_QUEUEHANDLE_T queue, void * element);

/********************************************************************************
** Description: This function is used to prepend an element                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_QUEUENODEHANDLE_T MMI_QUEUE_PostInFront(MMI_QUEUEHANDLE_T queue, void * element);

/********************************************************************************
** Description: This function is used to peek a element without remove it       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_QUEUE_Peek(MMI_QUEUEHANDLE_T queue, void *element);

/********************************************************************************
** Description: This function is used to retrieve a element                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_QUEUE_Retrieve(MMI_QUEUEHANDLE_T queue, void *element);

/********************************************************************************
** Description: This function is used to remove all nodes                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_QUEUE_Clear(MMI_QUEUEHANDLE_T queue);

/********************************************************************************
** Description: This function is used to check if a list is empty or not        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_QUEUE_IsEmpty(MMI_QUEUEHANDLE_T queue);

/********************************************************************************
** Description: This function is used to get item numbers in a list             *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_QUEUE_GetDepth(MMI_QUEUEHANDLE_T queue);

/********************************************************************************
** Description: This function is used to get item data in a node                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void * MMI_QUEUE_GetElement(MMI_QUEUENODEHANDLE_T node);

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_QUEUE_Traverse(MMI_QUEUEHANDLE_T queue, MMI_QUEUE_TrvFunc func);

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_QUEUE_GetCurrErrorCode(MMI_QUEUEHANDLE_T queue);

#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_QUEUE_Validate(MMI_QUEUEHANDLE_T queue);
#endif

#endif