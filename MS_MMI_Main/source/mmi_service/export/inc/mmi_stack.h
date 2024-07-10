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

#ifndef __MMI_STACK_H__
#define __MMI_STACK_H__

/*------------------------------------------------------------------------------*
**                            DEPENDENCY                                        *
**------------------------------------------------------------------------------*/
#include "mmi_base_common.h"

/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_STACK_ERROR_NUMBER(err)  ((err) & 0xFF)
#define MMI_STACK_ERROR_LOCATION(err)(((err) & 0xFFFFFF00) >> 8)

/*------------------------------------------------------------------------------*
**                            TYPE AND DEFINITIONS                              *
**  define opaque data structure to hide its detailed information from external *
**  programmer                                                                  *
**------------------------------------------------------------------------------*/
#define MMI_STACK_Create            MMI_DLIST_Create
#define MMI_STACK_Destroy           MMI_DLIST_Destroy
#define MMI_STACK_Push              MMI_DLIST_Prepend
#define MMI_STACK_Pop               MMI_DLIST_Retrieve
#define MMI_STACK_Peek              MMI_DLIST_Peek
#define MMI_STACK_IsEmpty           MMI_DLIST_IsEmpty
#define MMI_STACK_IsFull            MMI_DLIST_IsFull
#define MMI_STACK_GetDepth          MMI_DLIST_GetLength
#define MMI_STACK_Traverse          MMI_DLIST_Traverse
#define MMI_STACK_GetCurrErrorCode  MMI_DLIST_GetCurrErrorCode
#define MMI_STACK_TrvFunc            MMI_DLIST_TrvFunc    

typedef struct MMI_DLIST_TAG *      MMI_STACKHANDLE_T;
typedef struct MMI_DLISTNODE_TAG *  MMI_STACKNODEHANDLE_T;

enum
{
    STCK_ERROR_NULL_ELEMENT = 1,
    STCK_ERROR_EMPTY,
    STCK_ERROR_OVERFLOW,
    STCK_ERROR_UNDERFLOW,
    STCK_ERROR_OUT_OF_MEMORY,
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
MMI_STACKHANDLE_T MMI_STACK_Create(int32 capacity, int32 elementSize);

/********************************************************************************
** Description: This function is used to destroy a list                         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_STACK_Destroy(MMI_STACKHANDLE_T *stk);

/********************************************************************************
** Description: This function is used to push an element                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_STACKNODEHANDLE_T MMI_STACK_Push(MMI_STACKHANDLE_T stk, void * element);

/********************************************************************************
** Description: This function is used to pop an element                         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_STACK_Pop(MMI_STACKHANDLE_T stk, void * element);

/********************************************************************************
** Description: This function is used to peek an element                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_STACK_Peek(MMI_STACKHANDLE_T stk, void * element);

/********************************************************************************
** Description: This function is used to check if a list is empty or not        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_STACK_IsEmpty(MMI_STACKHANDLE_T stk);

/********************************************************************************
** Description: This function is used to check if a list is empty or not        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_STACK_IsFull(MMI_STACKHANDLE_T stk);

/********************************************************************************
** Description: This function is used to get item numbers in a list             *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_STACK_GetDepth(MMI_STACKHANDLE_T stk);

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_STACK_Traverse(MMI_STACKHANDLE_T stk, MMI_STACK_TrvFunc func);

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_STACK_GetCurrErrorCode(MMI_STACKHANDLE_T stk);

#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_STACK_Validate(MMI_STACKHANDLE_T stk);
#endif

#endif