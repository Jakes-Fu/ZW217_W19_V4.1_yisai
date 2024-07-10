/*****************************************************************************
** File Name:      datasrv_handle.c                                         *
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

#include "os_api.h"

#include "mmisrv_handlelist.h"

#define SRVHANDLE_DEBUG_TRACE(x)   SCI_TRACE_LOW(x)

typedef struct MMI_SRVHANDLE_TAG
{
    MMI_ALISTHANDLE_T   srv_lst;
    uint32              srv_elesize;
    SrvHandleFreeCallback   srv_freecbfunc;
    SrvHandleCreateCallback srv_createcbfunc;
} MMI_SRVHANDLE_INSTANCE_T;



/******************************************************************************
//  Description : handle srv instance init
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC MMI_SRVHANDLE_T DataSrvHandle_Create(uint32 ele_size,
                                             SrvHandleCreateCallback createfunc,
                                             SrvHandleFreeCallback freefunc)
{
    MMI_SRVHANDLE_INSTANCE_T* srvhandle_ptr = PNULL;

    //Check in param
    if (ele_size == 0 && createfunc == PNULL)
    {
        return PNULL;
    }

    srvhandle_ptr = (MMI_SRVHANDLE_INSTANCE_T*)SCI_ALLOCAZ(sizeof(MMI_SRVHANDLE_INSTANCE_T));

    if (srvhandle_ptr == PNULL)
    {
        return PNULL;
    }

    srvhandle_ptr->srv_lst = (MMI_ALISTHANDLE_T)MMI_ALIST_Create();/*lint !e746*/
    srvhandle_ptr->srv_elesize = ele_size;
    srvhandle_ptr->srv_createcbfunc = createfunc;
    srvhandle_ptr->srv_freecbfunc = freefunc;


    SRVHANDLE_DEBUG_TRACE("SRVHANDLE DataSrvHandle_Created");

    return srvhandle_ptr;
}

/******************************************************************************
//  Description : handle content free
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void FreeHandle(MMI_SRVHANDLE_T srv_data, void* handle)
{
    if (srv_data == PNULL || handle == PNULL)
    {
        return;
    }
    if (srv_data->srv_freecbfunc != PNULL)
    {
        srv_data->srv_freecbfunc(handle);
        handle = PNULL;
    }
    else
    {
        SCI_FREE(handle);
    }
    return;
}


/******************************************************************************
//  Description : destroy srv handle instance
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void DataSrvHandle_Destroy(MMI_SRVHANDLE_T *srvhandle_ptr)
{
    uint32 lst_size = 0;
    uint32 i = 0;
    void* srv_handle = PNULL;

    if (srvhandle_ptr == PNULL ||*srvhandle_ptr == PNULL)
    {
        return;
    }
    SRVHANDLE_DEBUG_TRACE("SRVHANDLE DataSrvHandle_Destroyed");

    if ((*srvhandle_ptr)->srv_lst != PNULL)
    {

        //free the datsrv_handle in srv_lst
        lst_size = MMI_ALIST_GetSize((*srvhandle_ptr)->srv_lst);
        for (i = 0; i < lst_size; i++)
        {
            srv_handle = MMI_ALIST_Get((*srvhandle_ptr)->srv_lst, i);
            FreeHandle((*srvhandle_ptr), srv_handle);
        }

        //free list
        MMI_ALIST_Destroy(&(*srvhandle_ptr)->srv_lst,PNULL);
    }    

    SCI_FREE(*srvhandle_ptr);

    return ;
}



/******************************************************************************
//  Description : new handle and append to handle list
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void* DataSrvHandle_NewHandle(MMI_SRVHANDLE_T srv_data)
{
    void* element_ptr = PNULL;

    if (srv_data == PNULL || srv_data->srv_lst == PNULL)
    {
        return PNULL;
    }

    SRVHANDLE_DEBUG_TRACE("SRVHANDLE DataSrvHandle_NewHandle");

    if (srv_data->srv_createcbfunc != PNULL)
    {
        element_ptr = srv_data->srv_createcbfunc();
    }
    else
    {
        element_ptr = SCI_ALLOCAZ(srv_data->srv_elesize);
    }

    MMI_ALIST_Append(srv_data->srv_lst, element_ptr);

    return element_ptr;
}

/******************************************************************************
//  Description : free handle and its element
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void DataSrvHandle_FreeHandle(MMI_SRVHANDLE_T srv_data, void* srv_handle)
{
    uint32 index = 0;

    if (srv_data == PNULL || srv_data->srv_lst == PNULL || srv_handle == PNULL)
    {
        return;
    }

    SRVHANDLE_DEBUG_TRACE("SRVHANDLE DataSrvHandle_FreeHandle ");

    index = DataSrvHandle_Find(srv_data, srv_handle);

    if (index == -1)/*lint !e650 !e737*/
    {
        return;
    }

    FreeHandle(srv_data, srv_handle);
    //Remove it from srv_lst
    MMI_ALIST_RemoveItem(srv_data->srv_lst, index, srv_data->srv_freecbfunc);

    return ;
}


/******************************************************************************
//  Description : Get first valid handle
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void* DataSrvHandle_GetFirstHandle(MMI_SRVHANDLE_T srv_data)
{
    if (srv_data == PNULL || srv_data->srv_lst == PNULL)
    {
        return PNULL;
    }
    if (MMI_ALIST_GetSize(srv_data->srv_lst) == 0)
    {
        return PNULL;
    }
    return MMI_ALIST_Get(srv_data->srv_lst, 0);
}

/******************************************************************************
//  Description : check handle list is empty or not
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN DataSrvHandle_isEmpty(MMI_SRVHANDLE_T srv_data)
{
    if (srv_data == PNULL || srv_data->srv_lst == PNULL)
    {
        return FALSE;
    }
    if (MMI_ALIST_GetSize(srv_data->srv_lst) == 0)
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************************************
//  Description : find the handle
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN DataSrvHandle_isInList(MMI_SRVHANDLE_T srv_data, void* srv_handle)
{
    int ret = 0;

    ret = DataSrvHandle_Find(srv_data, srv_handle);

    if (ret == -1)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/******************************************************************************
//  Description : find the handle
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC int DataSrvHandle_Find(MMI_SRVHANDLE_T srv_data, void* srv_handle)
{
    uint32 lst_size = 0;
    uint32 i = 0;

    if (srv_data == PNULL || srv_data->srv_lst == PNULL)
    {
        return -1;
    }
    if (MMI_ALIST_GetSize(srv_data->srv_lst) == 0)
    {
        return -1;
    }
    SRVHANDLE_DEBUG_TRACE("SRVHANDLE DataSrvHandle_Find ");

    lst_size = MMI_ALIST_GetSize(srv_data->srv_lst);
    //Find the datsrv_handle in srv_lst
    for (i = 0; i < lst_size; i++)
    {
        if (srv_handle == MMI_ALIST_Get(srv_data->srv_lst, i))
        {
            return i;
        }
    }
    return -1;

}

/******************************************************************************
//  Description : find the handle with cmpfunc
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void* DataSrvHandle_FindHandle(MMI_SRVHANDLE_T srv_data,
                                      void* cmp_data,
                                      SrvHandleFindCallback cmpfunc)
{
    uint32 lst_size = 0;
    uint32 i = 0;
    void*  srv_handle = PNULL;

    if (srv_data == PNULL || srv_data->srv_lst == PNULL || cmpfunc == PNULL)
    {
        return PNULL;
    }
    if (MMI_ALIST_GetSize(srv_data->srv_lst) == 0)
    {
        return PNULL;
    }

    SRVHANDLE_DEBUG_TRACE("SRVHANDLE DataSrvHandle_FindHandle ");

    lst_size = MMI_ALIST_GetSize(srv_data->srv_lst);

    for (i = 0; i < lst_size; i++)
    {
        srv_handle = MMI_ALIST_Get(srv_data->srv_lst, i);

        if (0 == cmpfunc(srv_handle,cmp_data))     //equal
        {
            break;
        }
        else
        {
            srv_handle = PNULL;
        }    
    }
    
    return srv_handle;

}


