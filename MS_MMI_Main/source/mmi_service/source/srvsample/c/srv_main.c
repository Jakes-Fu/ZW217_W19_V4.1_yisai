
/*****************************************************************************
** File Name:      srv_main.c                                         *
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

//typedef for srv_main
typedef enum
{
    SRVSTATUS_NULL,
    SRVSTATUS_INITIALIZING,
    SRVSTATUS_READY,
    SRVSTATUS_OPERATING,
    SRVSTATUS_MAX
}DATASRV_STATUS_E;


typedef struct
{
    DATASRV_STATUS_E    srv_status;
    SCI_MUTEX_PTR       srv_mutex;
    MMI_SLISTHANDLE_T   srv_handlelist;
    SAMPLE_HANDLE_T     cur_handle;
}DATASRV_SAMPLE_INSTANCE_T;


MMI_APPLICATION_T			g_datasrvsample_app;     

LOCAL DATASRV_SAMPLE_INSTANCE_T   s_srvsample_instance={0};


//typedef for srv operations
typedef enum
{
   SRV_OPERATION_READ,
   SRV_OPERATION_WRITE,
   SRV_OPERATION_COPY,

   SRV_OPERATION_MAX
}SRV_OPERATION_ENUM_E;

typedef struct
{
    BOOLEAN result;                     /* Result of the operation: TRUE: success; FALSE: failed */
    SRV_OPERATION_ENUM_E operation;     /* indicate the operation (read/save/Delete...)*/
    void *opreation_cb;                 /* Callback data*/
    void *user_data;                    /* User data */
} SRV_CALLBACK_STRUCT_T;

typedef void(*SrvCallbackFunc)(SRV_CALLBACK_STRUCT_T* callback_data);

typedef struct
{
    SRV_CALLBACK_STRUCT_T    oper_cb_data;
    SrvCallbackFunc          callback_func;

}SRV_OPERATION_DATA_T;

typedef struct
{
    SRV_OPERATION_DATA_T oper_data;
}SRV_HANDLE_TAG;


//


//Mutex operation
LOCAL void CreateSampleSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }
    if(*srv_mutex== 0)
    {
        *srv_mutex = SCI_CreateMutex("SMS_SRV_HANDLELIST_MUTEX",SCI_INHERIT);
    }
}

LOCAL void DestroySampleSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }

	if(*srv_mutex != 0)
	{
		SCI_DeleteMutex(*srv_mutex);
		*srv_mutex= PNULL;		
	}	
	return;
}

LOCAL void GetSampleSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }

    SCI_GetMutex(*srv_mutex, SCI_WAIT_FOREVER);    
}

LOCAL void PutSampleSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }
    SCI_PutMutex(*srv_mutex);    
}

//Srv main functions
LOCAL MMI_RESULT_E SampleDataSrv_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);

PUBLIC void SampleDataSrv_init(void)
{
    g_datasrvsample_app.ProcessMsg = SampleDataSrv_HandlePsMsg;

    SCI_MEMSET(&s_srvsample_instance,0,sizeof(DATASRV_SAMPLE_INSTANCE_T));

    SAMPLEHandle_InitList();
    
    CreateSampleSrvMutex(&s_srvsample_instance.srv_mutex);
    //Start read from nv and build msg_list
    
}

LOCAL void SampleDataSrv_Destroy(void)
{
    //
    //SAMPLEHandle_FreeList(s_srvsample_instance.samplehandle_list);
    SCI_MEMSET(&s_srvsample_instance,0,sizeof(DATASRV_SAMPLE_INSTANCE_T));
    DestroySampleSrvMutex(&s_srvsample_instance.srv_mutex);
}


PUBLIC BOOLEAN SampleDataSrvAPI_IsReady(void)
{   
    if (s_srvsample_instance.srv_status == SRVSTATUS_READY)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}


PUBLIC BOOLEAN SampleDataSrvAPI_IsOperating(void)
{
    if (s_srvsample_instance.srv_status == SRVSTATUS_OPERATING)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

LOCAL void SampleDataSrv_EnterReady()
{
    s_srvsample_instance.srv_status = SRVSTATUS_READY;
}

LOCAL BOOLEAN SampleDataSrv_EnterOperating(void)
{
    if (SampleDataSrvAPI_IsReady())
    {
        s_srvsample_instance.srv_status= SRVSTATUS_OPERATING;
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}



LOCAL BOOLEAN SampleDataSrv_ExitOperating(void)
{
    //Free cur handle;
    SAMPLEHandle_NewHandle(s_srvsample_instance.cur_handle);
#ifdef SAMPLE_AUTO_NEXT
    //Set next valid Handle as current
    //SAMPLEHandle_GetNextHandle()
    //Do opreation

    return;
#endif


    if (SampleDataSrvAPI_IsOperating())
    {
        s_srvsample_instance.srv_status= SRVSTATUS_READY;
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}


LOCAL MMI_RESULT_E SampleDataSrv_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    //Get cnf data

    //Get cur handle

    //build result data

    //Callback
    

    SampleDataSrv_ExitOperating();
}

PUBLIC BOOLEAN SampleDataSrv_PostEvent()
{}

//SRV  APIS
PUBLIC SAMPLE_HANDLE_T SAMPLEDATASRV_Read(
        uint32  read_id,
        SrvCallbackFunc read_callback,  
        SAMPLEDATASRV_OPERATE_RESULT_T *action_data,
        void *user_data
)
{
    SRV_HANDLE_TAG *cmd_handle=PNULL;
    SRV_OPERATION_DATA_T *oper_ptr=PNULL;
    
    //Check srv status

    // is Srv ready
    if (!SampleDataSrvAPI_IsReady())
    {
        return PNULL;
    }

    if (SAMPLEHandle_isFull(s_srvsample_instance.srv_handlelist))
    {
        return PNULL;
    }

    GetSampleSrvMutex(&s_srvsample_instance.srv_mutex);
    cmd_handle = (SRV_HANDLE_TAG *)SAMPLEHandle_NewHandle(s_srvsample_instance.srv_handlelist);  
    PutSampleSrvMutex(&s_srvsample_instance.srv_mutex);  
    if (cmd_handle == PNULL)
    {
        return PNULL;        
    }

    //construct operator data and save it
    cmd_handle->oper_data.oper_cb_data.operation = SRV_OPERATION_READ;
    cmd_handle->oper_data.oper_cb_data.result = FALSE;
    cmd_handle->oper_data.callback_func = read_callback;
    cmd_handle->oper_data.oper_cb_data.user_data = user_data;
    cmd_handle->oper_data.oper_cb_data.opreation_cb = action_data;

    // is Srv operating
    if (SampleDataSrvAPI_IsOperating())
    {
        return cmd_handle;
    }

    SampleDataSrv_EnterOperating();

    //Set Cur handle
    s_srvsample_instance.cur_handle = cmd_handle;
    
    //Do operation
    
    return cmd_handle;
}


