/********************************************************************************
** File Name:    mmi_dlist.h                                                    *
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

#ifndef __DOUBLY_LIST_H__
#define __DOUBLY_LIST_H__

/*------------------------------------------------------------------------------*
**                            DEPENDENCY                                        *
**------------------------------------------------------------------------------*/
#include "mmi_base_common.h"

/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_DLST_ERROR_NUMBER(err)  ((err) & 0xFF)
#define MMI_DLST_ERROR_LOCATION(err)(((err) & 0xFFFFFF00) >> 8)

#define MMI_DLIST_FOREACH(lst, node) for(node = MMI_DLIST_GetFirst(lst); node == MMI_DLIST_GetLast(lst); node = MMI_DLIST_GetNext(node))

/*------------------------------------------------------------------------------*
**                            TYPE AND DEFINITIONS                              *
**  define opaque data structure to hide its detailed information from external *
**  programmer                                                                  *
**------------------------------------------------------------------------------*/
typedef struct  MMI_DLIST_TAG *MMI_DLISTHANDLE_T;
typedef struct  MMI_DLISTNODE_TAG *MMI_DLISTNODEHANDLE_T;
typedef int32   (*MMI_DLIST_TrvFunc)(void *data);
typedef int32   (*MMI_DLIST_TrvStopFunc)(void *data, void *param);
typedef int     (*MMI_DLIST_CmpFunc)(const void *src, const void *dst);

enum
{
    MMI_DLST_ERROR_NULL_ELEMENT = 1,
    MMI_DLST_ERROR_EMPTY,
    MMI_DLST_ERROR_OVERFLOW,
    MMI_DLST_ERROR_UNDERFLOW,
    MMI_DLST_ERROR_OUT_OF_MEMORY,
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
MMI_DLISTHANDLE_T MMI_DLIST_Create(uint32 capacity, uint32 elementSize);

/********************************************************************************
** Description: This function is used to destroy a list                         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Destroy(MMI_DLISTHANDLE_T *lst);

/********************************************************************************
** Description: This function is used to prepend an element                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_Prepend(MMI_DLISTHANDLE_T lst, void * element);

/********************************************************************************
** Description: This function is used to append an element                      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_Append(MMI_DLISTHANDLE_T lst, void * element);

/********************************************************************************
** Description: This function is used to insert an element after a node         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_InsertBefore(MMI_DLISTHANDLE_T lst, MMI_DLISTNODEHANDLE_T node, void * element);

/********************************************************************************
** Description: This function is used to insert an element after a node         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_InsertAfter(MMI_DLISTHANDLE_T lst, MMI_DLISTNODEHANDLE_T node, void * element);

/********************************************************************************
** Description: This function is used to peek an element in head                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Peek(MMI_DLISTHANDLE_T lst, void * element);

/********************************************************************************
** Description: This function is used to retrieve an element in head            *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Retrieve(MMI_DLISTHANDLE_T lst, void * element);

/********************************************************************************
** Description: This function is used to delete a node                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Delete(MMI_DLISTHANDLE_T lst, MMI_DLISTNODEHANDLE_T *node);

/********************************************************************************
** Description: This function is used to delete a node                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_RemoveAll(MMI_DLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to check if a list is empty or not        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_IsEmpty(MMI_DLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to check if a stack is full or not        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_IsFull(MMI_DLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get item numbers in a list             *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_GetLength(MMI_DLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get first node in a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_DLISTNODEHANDLE_T MMI_DLIST_GetFirst(MMI_DLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get last node in a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_DLISTNODEHANDLE_T MMI_DLIST_GetLast(MMI_DLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get next node in a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_GetPrev(MMI_DLISTNODEHANDLE_T node);

/********************************************************************************
** Description: This function is used to get next node in a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_GetNext(MMI_DLISTNODEHANDLE_T node);

/********************************************************************************
** Description: This function is used to get item data in a node                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void * MMI_DLIST_GetElement(MMI_DLISTNODEHANDLE_T node);

/********************************************************************************
** Description: This function is used to replace item data in a node            *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_SetElement(MMI_DLISTNODEHANDLE_T node, void *element, uint32 element_size);

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Traverse(MMI_DLISTHANDLE_T lst, MMI_DLIST_TrvFunc func);

/********************************************************************************
** Description: This function is used to traverse through a list and stop when  *
**                condition meets                                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void* MMI_DLIST_TraverseStop(MMI_DLISTHANDLE_T lst, void *param, MMI_DLIST_TrvStopFunc func, int32 condition);

/********************************************************************************
** Description: This function is used to traverse through a list in reverse     *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Traverse_Reverse(MMI_DLISTHANDLE_T lst, MMI_DLIST_TrvFunc func);

/********************************************************************************
** Description: This function is used to traverse through a list in reverse     *
**              direction, and stop if condition meets                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void* MMI_DLIST_TraverseStop_Reverse(MMI_DLISTHANDLE_T lst, void *param, MMI_DLIST_TrvStopFunc func, int32 condition);

/********************************************************************************
** Description: This function is used to sort a list with func                  *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Sort(MMI_DLISTHANDLE_T lst, MMI_DLIST_CmpFunc func);

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_GetCurrErrorCode(MMI_DLISTHANDLE_T lst);

#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_Validate(MMI_DLISTHANDLE_T lst);
#endif

#endif