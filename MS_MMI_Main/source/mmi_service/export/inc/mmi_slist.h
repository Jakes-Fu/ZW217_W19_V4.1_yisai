/********************************************************************************
** File Name:    mmi_slist.h                                                    *
** Author:       Xiaomao Xiao                                                   *
** Date:         06/25/2012                                                     *
** Copyright:    2003 Spreadtrum Communications Incorporated. All Right Reserved*
** Description:  This file is used to describe data structure and API of single *
**               direction linked list                                          *
*********************************************************************************
**                            Important Edit history                            *
**-----------------------------------------------------------------------------**
** DATE            NAME            DESCRIPTION                                  *
** 06/25/2012    Xiaomao Xiao    Created                                        *
*********************************************************************************/

#ifndef __SINGLE_LIST_H__
#define __SINGLE_LIST_H__

/*------------------------------------------------------------------------------*
**                            DEPENDENCY                                        *
**------------------------------------------------------------------------------*/
#include "mmi_base_common.h"

/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/

#define MMI_SLST_ERROR_NUMBER(err)  ((err) & 0xFF)
#define MMI_SLST_ERROR_LOCATION(err)(((err) & 0xFFFFFF00) >> 8)

#define MMI_SLIST_FOREACH(lst, node) for(node = MMI_SLIST_GetFirst(lst); node != PNULL; node = MMI_SLIST_GetNext(node))

/*------------------------------------------------------------------------------*
**                            TYPE AND DEFINITIONS                              *
**  define opaque data structure to hide its detailed information from external *
**  programmer                                                                  *
**------------------------------------------------------------------------------*/
typedef struct MMI_SLIST_TAG *MMI_SLISTHANDLE_T;
typedef struct MMI_SLISTNODE_TAG *MMI_SLISTNODEHANDLE_T;
typedef void (*MMI_SLIST_TrvFunc)(void *data);
typedef int32 (*MMI_SLIST_TrvStopFunc)(void *data, void *param);

enum
{
  MMI_SLST_ERROR_NULL_ELEMENT = 1,
  MMI_SLST_ERROR_OUT_OF_MEMORY,
};

/*------------------------------------------------------------------------------*
**                            FUNCTION DEFINITIONS                              *
**------------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to create a list                          *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_SLISTHANDLE_T MMI_SLIST_Create(uint32 elementSize);

/********************************************************************************
** Description: This function is used to destroy a list                         *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_SLIST_Destroy(MMI_SLISTHANDLE_T *lst);

/********************************************************************************
** Description: This function is used to prepend an element                     *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_SLISTNODEHANDLE_T MMI_SLIST_Prepend(MMI_SLISTHANDLE_T lst, void * element);

/********************************************************************************
** Description: This function is used to append an element                      *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_SLISTNODEHANDLE_T MMI_SLIST_Append(MMI_SLISTHANDLE_T lst, void * element);

/********************************************************************************
** Description: This function is used to insert an element after a node         *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_SLISTNODEHANDLE_T MMI_SLIST_InsertAfter(MMI_SLISTHANDLE_T lst, MMI_SLISTNODEHANDLE_T node, void * element);

/********************************************************************************
** Description: This function is used to delete a node                          *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_SLIST_Delete(MMI_SLISTHANDLE_T lst, MMI_SLISTNODEHANDLE_T *node);

/********************************************************************************
** Description: This function is used to check if a list is empty or not        *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_SLIST_IsEmpty(MMI_SLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get item numbers in a list             *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_SLIST_GetLength(MMI_SLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get first node in a list                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_SLISTNODEHANDLE_T MMI_SLIST_GetFirst(MMI_SLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get last node in a list                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_SLISTNODEHANDLE_T MMI_SLIST_GetLast(MMI_SLISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to get next node in a list                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_SLISTNODEHANDLE_T MMI_SLIST_GetNext(MMI_SLISTNODEHANDLE_T node);

/********************************************************************************
** Description: This function is used to get item data in a node                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void * MMI_SLIST_GetElement(MMI_SLISTNODEHANDLE_T node);

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_SLIST_Traverse(MMI_SLISTHANDLE_T lst, MMI_SLIST_TrvFunc func);

/********************************************************************************
** Description: This function is used to traverse through a list and stop when  *
**                condition meets                                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void* MMI_SLIST_TraverseStop(MMI_SLISTHANDLE_T lst, void *param, MMI_SLIST_TrvStopFunc func, int32 condition);

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_SLIST_GetCurrErrorCode(MMI_SLISTHANDLE_T lst);

#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_SLIST_Validate(MMI_SLISTHANDLE_T lst);
#endif

#endif