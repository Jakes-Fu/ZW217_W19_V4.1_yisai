/***************************************************************************************
** File Name:      mmiemail_vector.h                                                   *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:                                                                        *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_vector.h                            *
**                                                                                     *
***************************************************************************************/
#ifndef _MMIEMAIL_VECTOR_H_
#define _MMIEMAIL_VECTOR_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sci_types.h"

/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern  "C"
{
#endif
    
/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/


/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef void (* WEPFNNOTIFY)(void * pdata);

typedef struct email_vector_tag
{
    void         **item_pptr;        // pointer to array of WE_VOID* items
    uint32       nitems;           // number of valid items in array
    int          cbsize;           // allocation size of ppItems (for MemGrow)
    int          cbused;           // number of bytes used in ppItems (for MemGrow)
    WEPFNNOTIFY  pfnfree;          // free function (called in MMIEMAIL_VectorDtor)
    int          grow_by;          // grow-by size for subsequent allocations
}EMAIL_VECTOR_T;

/*-------------------------------------------------------------------------------------*
**                           FUNCTION DECLARE                                          *
**------------------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: Free vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorFreeItem(EMAIL_VECTOR_T *vector_ptr, uint32 nindex);

/*****************************************************************************/
// 	Description: new a vector
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorNew(EMAIL_VECTOR_T **vector_pptr);

/*****************************************************************************/
// 	Description: delete vector
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_VectorDelete(EMAIL_VECTOR_T *vector_ptr);

/*****************************************************************************/
// 	Description: Delete vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorDeleteAt(EMAIL_VECTOR_T *vector_ptr, uint32 nindex);

/*****************************************************************************/
// 	Description: Delete vector's date all
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_VectorDeleteAll(EMAIL_VECTOR_T *vector_ptr);

/*****************************************************************************/
// 	Description: Insert date into vector by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorInsertAt(EMAIL_VECTOR_T *vector_ptr, uint32 nindex, void *item_ptr);

/*****************************************************************************/
// 	Description: Insert all date into vector
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorInsert(EMAIL_VECTOR_T *vector_ptr, void *item_ptr);

/*****************************************************************************/
// 	Description: Get the vector date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorGetAt(EMAIL_VECTOR_T *vector_ptr, uint32 nindex, void **item_pptr);

/*****************************************************************************/
// 	Description: Replace the vector date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorReplaceAt(EMAIL_VECTOR_T *vector_ptr, uint32 nindex, void *new_ptr);

/*****************************************************************************/
// 	Description: Get vector size
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorGetVectorNumber(EMAIL_VECTOR_T *vector_ptr);

/*****************************************************************************/
// 	Description: set vector's pfnfree
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC WEPFNNOTIFY MMIEMAIL_VectorSetPfnFree(EMAIL_VECTOR_T *vector_ptr, WEPFNNOTIFY pfnfree);

/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  /* _MMIEMAIL_VECTOR_H_ */