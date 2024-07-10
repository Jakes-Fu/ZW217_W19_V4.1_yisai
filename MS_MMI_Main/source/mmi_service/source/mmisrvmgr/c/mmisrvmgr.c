/****************************************************************************
** File Name:      mmisrvaud.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             22/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the data struct of                 
**                     AUDIO service module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 22/04/2011       Yintang.ren         Create
** 
****************************************************************************/
#include "mmi_service_trc.h"
#include "mmisrvmgr.h"
#include "threadx_os.h"
#include "mmisrv_adapt.h"
//#ifdef SXH_APPLE_SUPPORT
//#include "apple_export.h"
//#endif
#define SRV_NUM_MAX 3

#define STR_SRVMGR_NAME "MMI SRV MANAGER"
#define MMISRVMGR_MUTEX SCI_MUTEX_PTR
#define MMISRVMGR_CRREAT_MUTEXT {mmisrvmgr_mutex = SCI_CreateMutex(STR_SRVMGR_NAME, SCI_INHERIT);}
#define MMISRVMGR_LOCK SCI_GetMutex(mmisrvmgr_mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
#define MMISRVMGR_UNLOCK SCI_PutMutex(mmisrvmgr_mutex);

typedef struct
{
    uint32 id;
    uint8 name[MMISRVMGR_SERVICE_NAME_LEN + 1];
    uint16 pri_num;
    uint16 max_num;
    MMISRVMGR_SERVICE_CMD_FUNC cmd_func;
    //BOOL is_ext;	/*ÔÝÊ±È¥µô*/
    void *ext_data;
    uint32 ext_data_size;
} MMISRVMGR_SERVICE_DB_T;


typedef struct
{
    uint32 total;
    MMISRVMGR_SERVICE_DB_T srv_db[SRV_NUM_MAX];
}SRV_DB_INFO_T;

LOCAL SRV_DB_INFO_T s_srv_db_info = {0};
LOCAL MMISRVMGR_MUTEX mmisrvmgr_mutex = 0;

//#ifdef SXH_APPLE_SUPPORT
//#include "apple_export.h"
//#endif //#ifdef SXH_APPLE_SUPPORT


/*****************************************************************************/
//  Description : Get interface entity by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL IMMISRV_UNKNOWN_T *GetEntityPtr(MMISRV_HANDLE_T handle)
{
    uint32 i = 0;
    uint8 service_id = 0;
    
    MMISRVMGR_SERVICE_CMD_PARAM_T param = {0};
    MMISRVMGR_SERVICE_CMD_FUNC cmd_func = PNULL;    
    BOOLEAN res = FALSE;
    uint32 res_data = 0;        
    
    IMMISRV_UNKNOWN_T *entity_ptr = PNULL;
        
    if(handle != 0)
    {
        service_id = (uint8)(handle >> 24);
        if(service_id != 0)
        {
            for(i = 0; i < s_srv_db_info.total; i++)
            {
                if(service_id == s_srv_db_info.srv_db[i].id)
                {
                    cmd_func = s_srv_db_info.srv_db[i].cmd_func;
                    break;
                }
            }
            
            if(cmd_func != PNULL)
            {            
                param.cmd = MMISRVMGR_SERVICE_CMD_GET_ENTITY;
                param.data = handle;
                param.ext_data = 0;
                
                res = cmd_func(&param, &res_data);
                if(res)
                {
                    entity_ptr = (IMMISRV_UNKNOWN_T *)res_data;
                }
            }
        }
    }
    return entity_ptr;
}

/*****************************************************************************/
//  Description : service send report to management.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_ServiceSendReport(MMISRVMGR_SERVICE_REPORT_PARAM_T  *param)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : Request a new service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVMGR_Request(char *srv_name, MMISRVMGR_SERVICE_REQ_T *req_data, void *ext_data)
{
    uint32 i = 0;
    MMISRVMGR_SERVICE_CMD_FUNC cmd_func = PNULL;
    MMISRVMGR_SERVICE_CMD_PARAM_T param = {0};
    MMISRV_HANDLE_T handle = 0;
    uint32 res_data = 0;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Request() entry, srv_name=0x%x, req_data=0x%x,ext_data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_113_112_2_18_3_24_3_390,(uint8*)"ddd", srv_name, req_data, ext_data));
    
    for(i = 0; i < s_srv_db_info.total; i++)
    {
        if(strncmp(srv_name, (char *)s_srv_db_info.srv_db[i].name, MMISRVMGR_SERVICE_NAME_LEN) == 0)
        {
            cmd_func = s_srv_db_info.srv_db[i].cmd_func;
            break;
        }
    }
    if(cmd_func != PNULL)
    {
        param.cmd = MMISRVMGR_SERVICE_CMD_REQ;
        param.data = (uint32)req_data;
        param.ext_data = (uint32)ext_data;
        MMISRVMGR_LOCK;
        if(cmd_func(&param, &res_data))
        {
            handle = res_data;
        }
        MMISRVMGR_UNLOCK;
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Request() exit, handle=0x%x,cmd_func=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_135_112_2_18_3_24_3_391,(uint8*)"dd", handle, cmd_func));
    
    return handle;
}

/*****************************************************************************/
//  Description : Free a service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Free(MMISRV_HANDLE_T handle)
{
    IMMISRV_UNKNOWN_T *srv_ptr = PNULL;
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Free() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_149_112_2_18_3_24_3_392,(uint8*)"d", handle));

    if(handle != 0)
    {
        MMISRVMGR_LOCK;
        srv_ptr = GetEntityPtr(handle);
        
        if(PNULL != srv_ptr && PNULL != srv_ptr->vbtl_ptr && PNULL != srv_ptr->vbtl_ptr->Free)
        {
            srv_ptr->vbtl_ptr->Free(srv_ptr);
            res = TRUE;
        }
        MMISRVMGR_UNLOCK;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Free() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_161_112_2_18_3_24_3_393,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Suspend a service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Suspend(MMISRV_HANDLE_T handle)
{
    IMMISRV_UNKNOWN_T *srv_ptr = PNULL;
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Suspend() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_175_112_2_18_3_24_3_394,(uint8*)"d", handle));

    if(handle != 0)
    {
        MMISRVMGR_LOCK;
        srv_ptr = GetEntityPtr(handle);
        
        if(PNULL != srv_ptr && PNULL != srv_ptr->vbtl_ptr && PNULL != srv_ptr->vbtl_ptr->Suspend)
        {
            srv_ptr->vbtl_ptr->Suspend(srv_ptr);
            res = TRUE;
        }
        MMISRVMGR_UNLOCK;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Suspend() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_187_112_2_18_3_24_3_395,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Resume a service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Resume(MMISRV_HANDLE_T handle)
{
    IMMISRV_UNKNOWN_T *srv_ptr = PNULL;
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Resume() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_201_112_2_18_3_24_3_396,(uint8*)"d", handle));

    if(handle != 0)
    {
        MMISRVMGR_LOCK;
        srv_ptr = GetEntityPtr(handle);
    
        if(PNULL != srv_ptr && PNULL != srv_ptr->vbtl_ptr && PNULL != srv_ptr->vbtl_ptr->Resume)
        {
            srv_ptr->vbtl_ptr->Resume(srv_ptr);
            res = TRUE;
        }
        MMISRVMGR_UNLOCK;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_Resume() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_213_112_2_18_3_24_3_397,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Add a new service priority.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SetPri(MMISRV_HANDLE_T handle, uint16 pri)
{
    IMMISRV_UNKNOWN_T *srv_ptr = PNULL;
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_SetPri() entry,handle =0x%x pri=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_227_112_2_18_3_24_3_398,(uint8*)"dd", handle, pri));

    if(handle != 0)
    {
        MMISRVMGR_LOCK;
        srv_ptr = GetEntityPtr(handle);
    
        if(PNULL != srv_ptr && PNULL != srv_ptr->vbtl_ptr && PNULL != srv_ptr->vbtl_ptr->SetPri)
        {
            srv_ptr->vbtl_ptr->SetPri(srv_ptr, pri);
            res = TRUE;
        }
        MMISRVMGR_UNLOCK;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_SetPri() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_239_112_2_18_3_24_3_399,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Set service notify function.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SetNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_FUNC notify)
{
    IMMISRV_UNKNOWN_T *srv_ptr = PNULL;
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_SetNotify() entry,handle =0x%x notify=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_253_112_2_18_3_24_3_400,(uint8*)"dd", handle, notify));

    if(handle != 0)
    {
        MMISRVMGR_LOCK;
        srv_ptr = GetEntityPtr(handle);
    
        if(PNULL != srv_ptr && PNULL != srv_ptr->vbtl_ptr && PNULL != srv_ptr->vbtl_ptr->SetNotify)
        {
            srv_ptr->vbtl_ptr->SetNotify(srv_ptr, notify);
            res = TRUE;
        }
        MMISRVMGR_UNLOCK;
    }    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MGR MMISRVMGR_SetNotify() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVMGR_265_112_2_18_3_24_3_401,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Communicate between client and service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_IoCtrl(MMISRV_HANDLE_T handle, uint32 data, void *res)
{
    IMMISRV_UNKNOWN_T *srv_ptr = PNULL;
    BOOLEAN result = FALSE;

    if(handle != 0)
    {
        MMISRVMGR_LOCK;
        srv_ptr = GetEntityPtr(handle);
    
        if(PNULL != srv_ptr && PNULL != srv_ptr->vbtl_ptr && PNULL != srv_ptr->vbtl_ptr->IoCtrl)
        {
            result = srv_ptr->vbtl_ptr->IoCtrl(srv_ptr, data, res);
        }
        MMISRVMGR_UNLOCK;
    }
    return result;
}


/*****************************************************************************/
//  Description : Add a new service to service db.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISRVMGR_AddService(MMISRVMGR_SERVICE_CREATE_T *srv_data, void *ext_data, uint32 ext_data_size)
{
    MMISRVMGR_SERVICE_DB_T *db_item = PNULL;
    if(s_srv_db_info.total >= SRV_NUM_MAX)
    {
        return 0;
    }

    db_item = &s_srv_db_info.srv_db[s_srv_db_info.total];
    SCI_MEMCPY(db_item->name, srv_data->name, MMISRVMGR_SERVICE_NAME_LEN);
    db_item->pri_num = srv_data->pri_num;
    db_item->max_num = srv_data->max_num;
    db_item->cmd_func = srv_data->cmd_func;
    db_item->ext_data = ext_data;
    db_item->ext_data_size = ext_data_size;
    
    s_srv_db_info.total++;

    /* we regard the No. index as the service ID */
    db_item->id = s_srv_db_info.total;
    
    return db_item->id;
}


/*****************************************************************************/
//  Description : Send command to a appointed service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SendCmd(char *srv_name, uint32 cmd_data, uint32 *res_data)
{
    uint32 i = 0;
    MMISRVMGR_SERVICE_CMD_FUNC cmd_func = PNULL;
    MMISRVMGR_SERVICE_CMD_PARAM_T param = {0};
    BOOLEAN res = FALSE;
    
    for(i = 0; i < s_srv_db_info.total; i++)
    {
        if(strncmp(srv_name, (char *)s_srv_db_info.srv_db[i].name, MMISRVMGR_SERVICE_NAME_LEN) == 0)
        {
            cmd_func = s_srv_db_info.srv_db[i].cmd_func;            
            break;
        }
    }
    if(cmd_func != PNULL)
    {
        param.cmd = MMISRVMGR_SERVICE_CMD_EXT;
        param.data = (uint32)cmd_data;
        param.ext_data = 0;
        
        MMISRVMGR_LOCK;
        res = cmd_func(&param, res_data);
        MMISRVMGR_UNLOCK;
    }
    
    return res;
}

/*****************************************************************************/
//  Description : Init service mananement.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Init(void)
{
    SCI_MEMSET(&s_srv_db_info, 0, sizeof(SRV_DB_INFO_T));
    MMISRVMGR_CRREAT_MUTEXT;
    
#ifdef SXH_APPLE_SUPPORT
	APPLE_SRVMGR_Init();
#endif
    return TRUE;
}

