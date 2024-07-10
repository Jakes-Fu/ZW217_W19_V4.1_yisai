
/*****************************************************************************
** File Name:      srv_handle.c                                         *
** Author:                                                                  *
** Date:                                                                     *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
                                *
******************************************************************************/


#include "srvinternal_h.h"

//Handle operation
PUBLIC MMI_SLISTHANDLE_T SAMPLEHandle_InitList(void)
{
    return MMI_SLIST_Create(sizeof(SAMPLE_HANDLE_T));
}

PUBLIC SAMPLE_HANDLE_T SAMPLEHandle_NewHandle(MMI_SLISTHANDLE_T lst)
{
    MMI_SLISTNODEHANDLE_T node;
    SAMPLE_HANDLE_T*    element_ptr=PNULL;

    if (lst == PNULL)
    {
        return PNULL;
    }
    //
    element_ptr=(SAMPLE_HANDLE_T)SCI_ALLOCAZ(sizeof(SAMPLE_HANDLE_T));
    //Append to sample handle list
    node = MMI_SLIST_Append(lst,element_ptr);
    //return element of appended node
    return (SAMPLE_HANDLE_T)(MMI_SLIST_GetElement(node));
}

PUBLIC void SAMPLEHandle_FreeHandle(MMI_SLISTHANDLE_T lst,SAMPLE_HANDLE_T sample_handle)
{
    MMI_SLISTNODEHANDLE_T* node=PNULL;

    if (lst == PNULL ||sample_handle == PNULL)
    {
        return;
    }
    
    //find the node of this handle
//    node =lst->head;
//    while (node!=lst->tail)
    {
        if ( MMI_SLIST_GetElement(node) == sample_handle)
        {
            MMI_SLIST_Delete(lst,node);
//            break;
        }
    }
    return ;
}

PUBLIC SAMPLE_HANDLE_T SAMPLEHandle_GetHeadHandle(MMI_SLISTHANDLE_T lst)
{
    if (lst == PNULL)
    {
        return PNULL;
    }

//    return (SAMPLE_HANDLE_T*)(MMI_SLIST_GetElement(lst->head));

}

PUBLIC SAMPLE_HANDLE_T SAMPLEHandle_GetTailHandle(MMI_SLISTHANDLE_T lst)
{
    if (lst == PNULL)
    {
        return PNULL;
    }
 //   return (SAMPLE_HANDLE_T*)(MMI_SLIST_GetElement(lst->tail));
}

PUBLIC SAMPLE_HANDLE_T SAMPLEHandle_GetNextHandle(MMI_SLISTHANDLE_T lst,SAMPLE_HANDLE_T sample_handle)
{
    MMI_SLISTNODEHANDLE_T* node=PNULL;
    BOOLEAN isFind = FALSE;

    if (lst == PNULL ||sample_handle == PNULL)
    {
        return PNULL;
    }

//    node = MMI_SLIST_FindElement(sample_handle) 
    {
        return PNULL;
    }
    
    //find the node of this handle

//    node =lst->head;
//    while (node!=lst->tail)
    {
        if ( MMI_SLIST_GetElement(node) == sample_handle)
        {
            isFind = TRUE;
          //  break;
        }
    }

    if (isFind)
    {
//        return (SAMPLE_HANDLE_T*)(MMI_SLIST_GetElement(node->next));
    }
    else
    {
        return PNULL;
    }


}
LOCAL void FreeElement(void * val)
{
    SCI_FREE(val);
}


PUBLIC void SAMPLEHandle_DestroyList(MMI_SLISTHANDLE_T lst)
{
    if (lst == PNULL)
    {
        return;
    }
    MMI_SLIST_Traverse(lst, FreeElement);
    //free list
    MMI_SLIST_Destroy(&lst);
    return ;
}


///

PUBLIC BOOLEAN SAMPLEHandle_isFull(MMI_SLISTHANDLE_T lst)
{}


PUBLIC SAMPLE_HANDLE_T SAMPLEHandle_GetNextValidHandle(MMI_SLISTHANDLE_T lst)
{}

