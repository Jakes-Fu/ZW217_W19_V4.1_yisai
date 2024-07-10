/***************************************************************************************
** File Name:      mmiemail_vector.c                                                   *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    This File will gather functions that all events to this app are     *
**                 handled. These functions don't be changed by project changed.       *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_vector.c                            *
**                                                                                     *
***************************************************************************************/

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "mmiemail_vector.h"
#include "mmiemail_utils.h"
/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#define VECTOR_SUCCESS  0x00
#define VECTOR_NOMEMORY 0x10
#define VECTOR_BADPARM  0x21
#define VECTOR_ERROR    0xFF

#define VECTOR_GROW     10

/*-------------------------------------------------------------------------------------*
**                           FUNCTION DECLARE                                          *
**------------------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: Destroy vector
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void VectorDtor(EMAIL_VECTOR_T *vector_ptr);
/*****************************************************************************/
// 	Description: Expand vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void VectorExpandItems(
                             EMAIL_VECTOR_T *vector_ptr, 
                             uint32 nindex, 
                             BOOLEAN is_expand
                             );
/*****************************************************************************/
// 	Description: realloc vector's memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL int VectorMemGrowEx(
                          EMAIL_VECTOR_T *vector_ptr, 
                          void **mem_pptr, 
                          int *pcb_ptr, 
                          int cbused, 
                          int cbmore, 
                          int cballoc_extra
                          );
/*-------------------------------------------------------------------------------------*
**                           FUNCTION DEFINITION                                       *
**------------------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: Free vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL int VectorFreeItem(EMAIL_VECTOR_T *vector_ptr, uint32 nindex) 
{
    return MMIEMAIL_VectorReplaceAt(vector_ptr, nindex, PNULL);
}

/*****************************************************************************/
// 	Description: Free vector's date
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_VectorDelete(EMAIL_VECTOR_T *vector_ptr)
{
    if(PNULL != vector_ptr)
    {
        VectorDtor(vector_ptr);
        EMA_UTILS_FREE(PNULL, vector_ptr);
    }
}

/*****************************************************************************/
// 	Description: Destroy vector
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void VectorDtor(EMAIL_VECTOR_T *vector_ptr)
{
    if (PNULL != vector_ptr)
    {
        MMIEMAIL_VectorDeleteAll(vector_ptr);
        EMA_UTILS_FREE(PNULL, vector_ptr->item_pptr);
    }
}

/*****************************************************************************/
// 	Description: Get vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorGetAt(EMAIL_VECTOR_T *vector_ptr, uint32 nindex, void **item_pptr)
{
    if (nindex >= vector_ptr->nitems) 
    {
        *item_pptr = PNULL;
        return VECTOR_BADPARM;
    }
    
    *item_pptr = vector_ptr->item_pptr[nindex];
    return VECTOR_SUCCESS;
}

/*****************************************************************************/
// 	Description: Replace vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorReplaceAt(EMAIL_VECTOR_T *vector_ptr, uint32 nindex, void *new_ptr)
{
    void *item_ptr = PNULL;
    int nerr = MMIEMAIL_VectorGetAt(vector_ptr, nindex, &item_ptr);
    
    if (VECTOR_SUCCESS == nerr) 
    {
        if (item_ptr && vector_ptr->pfnfree) 
        {
            vector_ptr->pfnfree(item_ptr);
        }
        vector_ptr->item_pptr[nindex] = new_ptr;
    }
    
    return nerr;
}

/*****************************************************************************/
// 	Description: Expand vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void VectorExpandItems(EMAIL_VECTOR_T *vector_ptr, uint32 nindex, BOOLEAN is_expand)
{
    int ndir = is_expand ? 1 : -1;
    
    if (nindex < vector_ptr->nitems) 
    {
        uint32 index_src = nindex;
        uint32 index_dest = nindex;
        
        if (is_expand) 
        {
            index_dest++;
        } 
        else 
        {
            index_src++;
        }
        
        EMA_UTILS_MEMMOVE(vector_ptr->item_pptr+index_dest, vector_ptr->item_pptr+index_src, (vector_ptr->nitems - index_src) * sizeof(void *));
    }
    
    vector_ptr->nitems += (uint32)ndir;
    vector_ptr->cbused = vector_ptr->nitems * sizeof(void *);
}

/*****************************************************************************/
// 	Description: realloc vector's memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL int VectorMemGrowEx(
                          EMAIL_VECTOR_T *vector_ptr, 
                          void **mem_pptr, 
                          int *pcb_ptr, 
                          int cbused, 
                          int cbmore, 
                          int cballoc_extra
                          )
{
    void *mem_ptr  = *mem_pptr;
    int  cb     = *pcb_ptr;
    int  cbfree = cb - cbused;
    
    if (cbfree < cbmore) 
    {
        cb += cbmore - cbfree + cballoc_extra;
        
        mem_ptr = EMA_UTILS_REALLOC(PNULL, mem_ptr, cb);
        if (PNULL == mem_ptr)
        {
            return VECTOR_NOMEMORY;
        }
        
        *mem_pptr = mem_ptr;
        *pcb_ptr  = cb;
    }
    
    return VECTOR_SUCCESS;
}

/*****************************************************************************/
// 	Description: Insert date to vector by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorInsertAt(
                                   EMAIL_VECTOR_T *vector_ptr, 
                                   uint32 nindex, 
                                   void *item_ptr
                                   )
{
    int nerr = VECTOR_SUCCESS;
    
    if (nindex > vector_ptr->nitems)
    {
        nindex = vector_ptr->nitems;
    }
    
    nerr = VectorMemGrowEx(vector_ptr, (void *)&vector_ptr->item_pptr, &vector_ptr->cbsize, vector_ptr->cbused, 
        sizeof(void *), (((uint)vector_ptr->grow_by) * sizeof(void *)));
    
    if (VECTOR_SUCCESS == nerr) 
    {
        VectorExpandItems(vector_ptr, nindex, 1);
        vector_ptr->item_pptr[nindex] = item_ptr;
    }
    
    return nerr;
}

/*****************************************************************************/
// 	Description: Insert all date into vector
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorInsert(EMAIL_VECTOR_T *vector_ptr, void *item_ptr)
{
    int nerr = VECTOR_SUCCESS;
    uint32 nindex = 0;
    
    nindex = vector_ptr->nitems;
    
    nerr = VectorMemGrowEx(vector_ptr, (void *)&vector_ptr->item_pptr, &vector_ptr->cbsize, vector_ptr->cbused, 
        sizeof(void *), (((uint)vector_ptr->grow_by) * sizeof(void *)));
    
    if (VECTOR_SUCCESS == nerr) 
    {
        VectorExpandItems(vector_ptr, nindex, 1);
        vector_ptr->item_pptr[nindex] = item_ptr;
    }
    
    return nerr;
}

/*****************************************************************************/
// 	Description: Delete vector's date by index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorDeleteAt(EMAIL_VECTOR_T *vector_ptr, uint32 nindex)
{
    int nerr = VectorFreeItem(vector_ptr, nindex);
    
    if (VECTOR_SUCCESS == nerr)
    {
        VectorExpandItems(vector_ptr, nindex, 0);
    }
    return nerr;
}

/*****************************************************************************/
// 	Description: Delete vector's date all
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_VectorDeleteAll(EMAIL_VECTOR_T *vector_ptr)
{
    uint32 nindex;
    
    for (nindex = 0; nindex < vector_ptr->nitems; nindex++) 
    {
        VectorFreeItem(vector_ptr, nindex);
    }
    vector_ptr->nitems = 0;
    vector_ptr->cbused = 0;
}

/*****************************************************************************/
// 	Description: Get vector size
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorGetVectorNumber(EMAIL_VECTOR_T *vector_ptr) 
{
    return vector_ptr->nitems;
}

/*****************************************************************************/
// 	Description: new a vector
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_VectorNew(EMAIL_VECTOR_T **vector_pptr)
{
    EMAIL_VECTOR_T *vector_ptr = PNULL;
    int    iret = VECTOR_SUCCESS;
    
    do
    {
        vector_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_VECTOR_T));
        if(PNULL == vector_ptr)
        {
            iret = VECTOR_ERROR;
            break;
        }
        EMA_UTILS_MEMSET(vector_ptr, 0x00, sizeof(EMAIL_VECTOR_T));
        
        vector_ptr->grow_by = VECTOR_GROW;
        *vector_pptr = vector_ptr;
        
    } while (0);
    
    return iret;
}

/*****************************************************************************/
// 	Description: set vector's pfnfree
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC WEPFNNOTIFY MMIEMAIL_VectorSetPfnFree(EMAIL_VECTOR_T *vector_ptr, WEPFNNOTIFY pfnfree)
{
    WEPFNNOTIFY pfnold = vector_ptr->pfnfree;
    vector_ptr->pfnfree = pfnfree;
    return pfnold; // return old pfn
}

