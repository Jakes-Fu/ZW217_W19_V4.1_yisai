
/*****************************************************************************
** File Name:      srvsample_main.c                                         *
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

#include "srvsample_define.h"
#include "srvsample_export.h"
#include "mmk_timer.h"

#include "mmi_event_api.h"
//#include "mmi_common.h"

//#include "srvsample_instance.c"
//#include "srvsample_fsm.c"

//#include "mmi_appmsg.h"
#include "sig_code.h"

//global variable declare
//MMI_APPLICATION_T			g_srvsample_app;     

typedef struct
{
    uint32 ref_no;
    BOOLEAN isOperating;
}SRV_SAMPLE_INSTANCE_T;

#include "mmisrv_handlelist.h"


//LOCAL 
PUBLIC MMI_RESULT_E SrvSample_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);



//Init
PUBLIC void SAMPLESRV_Init(void)
{
//    g_srvsample_app.ProcessMsg = SrvSample_HandlePsMsg;
}



PUBLIC MMI_RESULT_E SrvSample_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;


    if(PNULL== app_ptr)
    {
        //Raise Error
        return MMI_RESULT_FALSE;
    }

    switch ( msg_id )
    {
        
    case APP_MN_READ_SMS_CNF:
        
        break;
        
        
    case APP_MN_EDIT_SMS_CNF :
        
        break;
        
    case APP_MN_WRITE_SMS_CNF:
        
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return ( result );
}

    //Set Default value
void SAMPLESRV_SetDefault(SRVSAMPLE_PERSON_DATA_T *srvdata_ptr)
{
    uint16 i = 0;
    
    if (srvdata_ptr == PNULL)
    {
        //Raise Error
        return;
    }
    for (i=0;i<SRVSAMPLE_MAX_RECORD_NUM;i++)
    {
        srvdata_ptr->person_item[i].age = i*5;
        srvdata_ptr->person_item[i].sex= 0;
    }
}

//Write Nv
LOCAL void SAMPLESRV_WriteNV(SRVSAMPLE_PERSON_DATA_T *srvdata_ptr)
{
    if (srvdata_ptr == PNULL)
    {
        //Raise Error
        return;
    }

   // MMINV_WRITE(MMINV_APPSAMPLE_SAVED_DATA, srvdata_ptr);

    MMK_SampleTriggerEvent(MMISAMPLE_APP_UPDATE, PNULL,0);

    return;
}

//Read Nv
LOCAL BOOLEAN SAMPLESRV_ReadNV(SRVSAMPLE_PERSON_DATA_T *srvdata_ptr)
{
    BOOLEAN   return_value = FALSE;

    if (srvdata_ptr == PNULL)
    {
        //Raise Error
        return FALSE;
    }


   // MMINV_READ(MMINV_APPSAMPLE_SAVED_DATA, srvdata_ptr, return_value);

    if (!return_value)
    {
        SAMPLESRV_SetDefault(srvdata_ptr);
        SAMPLESRV_WriteNV(srvdata_ptr);
    }
    return TRUE;
}





typedef struct SAMPLE_SRV_HANDLE_TAG
{
    uint32               index;
    uint32               oper_type;
    SRV_OPERATION_DATA_T oper_data;
}SAMPLE_SRV_HANDLE_T;


typedef struct
{
    uint32              srv_status;
    MMI_SRVHANDLE_T     srvhandle_lst;
    SCI_MUTEX_PTR       srv_mutex;
    SAMPLE_HANDLE_T     cur_handle;
} SAPMPLE_INSTANCE_T;

LOCAL SAPMPLE_INSTANCE_T   s_sample_instance = {0};


/******************************************************************************
//  Description : create mutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void CreateDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL )
    {
        return;
    }
    if (*srv_mutex == 0)
    {
        *srv_mutex = SCI_CreateMutex("SAMPLE_SRV_HANDLELIST_MUTEX", SCI_INHERIT);
    }
}

/******************************************************************************
//  Description : destroy mutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void DestroyDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }

    if (*srv_mutex != 0)
    {
        SCI_DeleteMutex(*srv_mutex);
        *srv_mutex = PNULL;
    }
    return;
}

/******************************************************************************
//  Description : GetMutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void GetDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }
    SCI_GetMutex(*srv_mutex, SCI_WAIT_FOREVER);
}

/******************************************************************************
//  Description : Put Mutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void PutDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }
    SCI_PutMutex(*srv_mutex);
}



PUBLIC void SampleSrv_init(void)
{
    char mutex_name[] = "";


    SCI_MEMSET(&s_sample_instance, 0, sizeof(SAPMPLE_INSTANCE_T));

    //Create datasrv handle instance
    s_sample_instance.srvhandle_lst = DataSrvHandle_Create(sizeof(SAMPLE_HANDLE_T),
                                                           PNULL,
                                                           PNULL);

    CreateDataSrvMutex(&s_sample_instance.srv_mutex);
}

LOCAL void SampleSrv_Destroy(void)
{
    DataSrvHandle_Destroy(&s_sample_instance.srvhandle_lst);

    SCI_MEMSET(&s_sample_instance, 0, sizeof(SAPMPLE_INSTANCE_T));

}


PUBLIC BOOLEAN SampleSrvAPI_IsReady(void)
{

}


PUBLIC BOOLEAN SampleSrvAPI_IsOperating(void)
{
}

LOCAL void SampleSrv_EnterReady()
{
}



PUBLIC BOOLEAN Sample_Sync_Callback(
    SRV_OPERATION_ENUM_E operation,
    BOOLEAN result,
    uint32 cause,
    void *opreation_cb,
    void *user_data,
    SrvCallbackFunc callback_func)
{

    SRV_CALLBACK_STRUCT_T oper_data = {0};

    if (callback_func == PNULL)
    {
        return;
    }
    oper_data.operation = operation;
    oper_data.opreation_cb = opreation_cb;
    oper_data.result = result;
    oper_data.user_data = user_data;

    return callback_func(&oper_data);

}


LOCAL void SrvSample_ReadTimeOut(uint8  timer_id, uint32 param)
{
    SAMPLE_HANDLE_T  cmd_handle= (SAMPLE_HANDLE_T)param;
    SRVSAMPLE_PERSON_DATA_T srvdata={0};
    
    if (cmd_handle == PNULL)
    {
        return;
    }
    if (SAMPLESRV_ReadNV(&srvdata))
    {
        //SCI_MEMCPY((uint16*)cmd_handle->oper_data.oper_cb_data.opreation_cb,
                //&srvdata.person_item[cmd_handle->index].age,
                //sizeof(uint16));
    
        if(Sample_Sync_Callback(
            cmd_handle->oper_data.oper_cb_data.operation,
            cmd_handle->oper_data.oper_cb_data.result,
            0,
            cmd_handle->oper_data.oper_cb_data.opreation_cb,
            cmd_handle->oper_data.oper_cb_data.user_data,
            cmd_handle->oper_data.callback_func))
        {
            DataSrvHandle_FreeHandle(s_sample_instance.srvhandle_lst,cmd_handle);
        }
    }
    return;
}


PUBLIC SAMPLE_HANDLE_T SAMPLESRV_AysnReadOnebyIndex(
    uint32 index,
    SrvCallbackFunc read_callback,
    void *action_data,
    void *user_data)
{
    SAMPLE_HANDLE_T  new_handle = PNULL;

    SRV_CALLBACK_STRUCT_T *oper_data = PNULL;

    if (s_sample_instance.srvhandle_lst == PNULL)
    {
        SampleSrv_init();
    }

    GetDataSrvMutex(&s_sample_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SAMPLE_HANDLE_T)DataSrvHandle_NewHandle(s_sample_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        return PNULL;
    }

    s_sample_instance.cur_handle = new_handle;

    //construct operator data and save it
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
    new_handle->oper_data.callback_func = read_callback;
    //send to MN
    new_handle->index = index;
    
    //Do read actions
    MMK_CreateTimerCallback(5000, SrvSample_ReadTimeOut, (uint32)new_handle, FALSE);


    PutDataSrvMutex(&s_sample_instance.srv_mutex);

    return new_handle;
}



//Req Delete One and wait result
PUBLIC BOOLEAN SAMPLESRV_AysnDeleteOnebyIndex(uint16 index)
{}


#include "srvsample_API.c"

