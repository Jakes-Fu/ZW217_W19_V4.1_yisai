/********************************************************************************
** File Name:    mmi_arraylist.h                                                *
** Author:       Hongbing Ju                                                    *
** Date:         06/25/2012                                                     *
** Copyright:    2003 Spreadtrum Communications Incorporated. All Right Reserved*
** Description:  This file is used to describe data structure and API of array  *
**               list                                                           *
*********************************************************************************
**                            Important Edit history                            *
**-----------------------------------------------------------------------------**
** DATE            NAME            DESCRIPTION                                  *
** 06/25/2012      Hongbing Ju      Created                                     *
*********************************************************************************/

#ifndef __MMI_ARRAY_LIST_H__
#define __MMI_ARRAY_LIST_H__

/*------------------------------------------------------------------------------*
**                            DEPENDENCY                                        *
**------------------------------------------------------------------------------*/
#include "mmi_base_common.h"

/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_ALST_ERROR_NUMBER(err)  ((err) & 0xFF)
#define MMI_ALST_ERROR_LOCATION(err)(((err) & 0xFFFFFF00) >> 8)

/* caller needs to provide start position of the for loop */
#define MMI_ALIST_FOREACH(lst, index) for(; MMI_ALIST_HasNext(lst, index); index++)

/*------------------------------------------------------------------------------*
**                            TYPE AND DEFINITIONS                              *
**  define opaque data structure to hide its detailed information from external *
**  programmer                                                                  *
**------------------------------------------------------------------------------*/
typedef struct MMI_ALIST_Tag *MMI_ALISTHANDLE_T;
typedef void (*MMI_ALIST_Func)(void *data);
typedef int  (*MMI_ALIST_CmpFunc)(const void *src, const void *dst);
typedef void (*MMI_ALIST_RemoveFunc)(void *data);

enum
{
    MMI_ALST_ERROR_NO_ERROR,
    MMI_ALST_ERROR_NULL_ELEMENT = 1,
    MMI_ALST_ERROR_EMPTY,
    MMI_ALST_ERROR_OVERFLOW,
    MMI_ALST_ERROR_UNDERFLOW,
    MMI_ALST_ERROR_OUT_OF_MEMORY,
    MMI_ALST_ERROR_TRIM_FAILURE,
    MMI_ALST_ERROR_REALLOC_FAIL,
    MMI_ALST_ERROR_NULL_HANDLE,
    MMI_ALST_ERROR_INVALID_PARAM,
    MMI_ALST_ERROR_NOT_FOUND
};

typedef enum
{
    ALIST_SEARCH_NORMAL,
    ALIST_SEARCH_FIRST,
    ALIST_SEARCH_LAST,
    ALIST_SEARCH_MODE_TOTAL
} ALIST_SEARCH_MODE_T;

/*------------------------------------------------------------------------------*
**                            FUNCTION DEFINITIONS                              *
**------------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to create a list                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_ALISTHANDLE_T MMI_ALIST_Create(void);

/********************************************************************************
** Description: This function is used to destroy a list                         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Destroy(MMI_ALISTHANDLE_T *lst, MMI_ALIST_RemoveFunc rmv_func);

/********************************************************************************
** Description: This function is used to Add item at the end of the list.       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Append(MMI_ALISTHANDLE_T lst, void* item);

/********************************************************************************
** Description: This function is used to Pop (remove and return) an item off    *
**              the end of the list.                                            *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void* MMI_ALIST_Pop(MMI_ALISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to Return item located at index           *
**              of the list.                                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void* MMI_ALIST_Get(MMI_ALISTHANDLE_T lst, uint32 index);

/********************************************************************************
** Description: This function is used to Replace item at index with given value *
**              of the list.                                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Set(MMI_ALISTHANDLE_T lst, uint32 index, void* value);

/********************************************************************************
** Description: This function is used to Insert item at index, shifting the     *
**              following items by one spot.                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Insert(MMI_ALISTHANDLE_T lst, uint32 index, void* value);

/********************************************************************************
** Description: This function is used to Remove the item at index, shifting the *
**              following items by one spot.                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void* MMI_ALIST_RemoveItem(MMI_ALISTHANDLE_T lst, uint32 index, MMI_ALIST_RemoveFunc func);

/********************************************************************************
** Description: This function is used to Remove all item in the array list      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_RemoveAllItems(MMI_ALISTHANDLE_T lst, MMI_ALIST_RemoveFunc func);

/********************************************************************************
** Description: This function is used to Clear list of all items                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Clear(MMI_ALISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to Return a slice of the list (of given   *
**              length starting at index) as a new arraylist                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_ALISTHANDLE_T MMI_ALIST_Slice(MMI_ALISTHANDLE_T lst, uint32 index, uint32 length);

/********************************************************************************
** Description: This function is used to Return a slice of the list (from index *
**              to the end) as a new arraylist.                                 *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_ALISTHANDLE_T MMI_ALIST_SliceEnd(MMI_ALISTHANDLE_T lst, uint32 index);

/********************************************************************************
** Description: This function is used to Return a copy of the arraylist         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_ALISTHANDLE_T MMI_ALIST_Copy(MMI_ALISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to Append a list onto another, in-place.  *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Join(MMI_ALISTHANDLE_T lst, MMI_ALISTHANDLE_T source);

/********************************************************************************
** Description: This function is used to Insert a list into another at the      *
**              given index, in-place.                                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Splice(MMI_ALISTHANDLE_T lst, MMI_ALISTHANDLE_T source, uint32 index);

/********************************************************************************
** Description: This function is used to Return the number of items contained   *
**              in the list                                                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC uint32 MMI_ALIST_GetSize(MMI_ALISTHANDLE_T lst);

/********************************************************************************
** Description: This function is used to trim the capacity to current size      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_TrimToSize(MMI_ALISTHANDLE_T lst);


/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC BOOLEAN MMI_ALIST_HasNext(MMI_ALISTHANDLE_T lst, uint32 index);

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Traverse(MMI_ALISTHANDLE_T lst, MMI_ALIST_Func func);

/********************************************************************************
** Description: This function is used to sort a list with func                  *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Sort(MMI_ALISTHANDLE_T lst, MMI_ALIST_CmpFunc func);

/********************************************************************************
** Description: This function is used to sort a list with func                  *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Find(MMI_ALISTHANDLE_T lst, void* data, MMI_ALIST_CmpFunc func);

/********************************************************************************
** Description: This function is used to sort a list with func                  *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_FindFirst(MMI_ALISTHANDLE_T lst, void* key, MMI_ALIST_CmpFunc func);

/********************************************************************************
** Description: This function is used to sort a list with func                  *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_FindLast(MMI_ALISTHANDLE_T lst, void* key, MMI_ALIST_CmpFunc func);

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_GetCurrErrorCode(MMI_ALISTHANDLE_T lst);

#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_ALIST_Validate(MMI_ALISTHANDLE_T lst);
#endif

#endif