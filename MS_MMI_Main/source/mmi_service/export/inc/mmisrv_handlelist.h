/*****************************************************************************
 ** File Name:   mmisrv_handlelist.h
 ** Author:      sam.hua
 ** Date:        17/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe srv_handlelist
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  17/aug/2012      sam.hua           created
 ****************************************************************************/


#ifndef _SRV_HANDLE_LIST_H_
#define _SRV_HANDLE_LIST_H_

#include "mmi_arraylist.h"

typedef void (*SrvHandleFreeCallback)(void* handle);
typedef void* (*SrvHandleCreateCallback)(void);
typedef int (*SrvHandleFindCallback)(void* handle,void* cmp_data);

typedef struct MMI_SRVHANDLE_TAG* MMI_SRVHANDLE_T;


/******************************************************************************
//  Description : handle srv instance construtor
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC MMI_SRVHANDLE_T DataSrvHandle_Create(uint32 ele_size,
                                            SrvHandleCreateCallback createfunc,
                                            SrvHandleFreeCallback freefunc);


/******************************************************************************
//  Description : destroy srv handle instance
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void DataSrvHandle_Destroy(MMI_SRVHANDLE_T *srvhandle_ptr);


/******************************************************************************
//  Description : new handle and append to handle list
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void* DataSrvHandle_NewHandle(MMI_SRVHANDLE_T srv_data);

/******************************************************************************
//  Description : free handle and its element
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void DataSrvHandle_FreeHandle(MMI_SRVHANDLE_T srv_data, void* srv_handle);

/******************************************************************************
//  Description : check handle list is empty or not
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN DataSrvHandle_isEmpty(MMI_SRVHANDLE_T srv_data);

/******************************************************************************
//  Description : Get first valid handle
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void* DataSrvHandle_GetFirstHandle(MMI_SRVHANDLE_T srv_data);

/******************************************************************************
//  Description : find the handle in the srv_list or not
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN DataSrvHandle_isInList(MMI_SRVHANDLE_T ssrv_data, void* srv_handle);

/******************************************************************************
//  Description : find the handle
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC int DataSrvHandle_Find(MMI_SRVHANDLE_T srv_data, void* srv_handle);

/******************************************************************************
//  Description : find the handle with cmpfunc
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void* DataSrvHandle_FindHandle(MMI_SRVHANDLE_T srv_data,
                                      void* cmp_data,
                                      SrvHandleFindCallback cmpfunc);

/******************************************************************************
//  Description : find the handle with cmpfunc
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void* DataSrvHandle_SetMutexName(MMI_SRVHANDLE_T srv_data, char* mutex_name);



#endif //_SRV_HANDLE_LIST_H_

