
/*****************************************************************************
** File Name:      srvsample_instance.c                                               *
** Author:                                                          *
** Date:                                                          *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
                                *
******************************************************************************/
#include "mmk_timer.h"

typedef struct 
{
    uint8  change_timer_id;
    uint16 index;
    uint16 age;
}SRVSAMPLE_OPERATION_DATA_T;

typedef enum
{
    SRVSAMPLE_CHANGING_IDLE,
    SRVSAMPLE_CHANGING_RUNNING,
    SRVSAMPLE_CHANGING_COMPLETED,
    SRVSAMPLE_CHANGING_MAX,
}SRVSAMPLE_PROCESS_FLAG_T;

typedef struct
{
    uint32 ref_no;
    SRVSAMPLE_PROCESS_FLAG_T flag;
    BOOLEAN isSuccess;
    SRVSAMPLE_OPERATION_DATA_T operation_data;
}SRVSAMPLE_INSTANCE_T;

typedef struct
{
    MMI_HANDLE_T my_handle;
    SRVSAMPLE_INSTANCE_T *srv_instance;  
}SRVSAMPLE_HANDLE_T;

//Create Srv Handle
PUBLIC void* SrvSample_CreateSrv(void)
{
    SRVSAMPLE_HANDLE_T *srv_handle=PNULL;
	SRVSAMPLE_INSTANCE_T *srv_instance=PNULL;

    //Alloc Handle and instance
    srv_handle = SCI_ALLOC(sizeof(SRVSAMPLE_HANDLE_T));
    if (srv_handle ==PNULL)
    {
        //Raise Error
        return PNULL;
    }

    srv_instance = SCI_ALLOC(sizeof(SRVSAMPLE_INSTANCE_T));
    if (srv_instance ==PNULL)
    {
        //Raise Error
        SCI_FREE(srv_handle);
        return PNULL;
    }
    SCI_MEMSET(srv_handle,0,sizeof(SRVSAMPLE_HANDLE_T));
    SCI_MEMSET(srv_instance,0,sizeof(SRVSAMPLE_INSTANCE_T));
    return srv_handle;
}

LOCAL void SrvSample_TimeOut(uint8  timer_id, uint32 param)
{
    SRVSAMPLE_HANDLE_T *srv_handle= (SRVSAMPLE_HANDLE_T*)param;
    if (srv_handle == PNULL)
    {
        //Raise Error
        return;
    }
    if (srv_handle->srv_instance->flag != SRVSAMPLE_CHANGING_RUNNING)
    {
        //Raise Error
        return;
    }
    //save operation result and flag;
    srv_handle->srv_instance->isSuccess = SAMPLESRV_EditOneAgebyIndex(srv_handle->srv_instance->operation_data.index,
                srv_handle->srv_instance->operation_data.age);
    srv_handle->srv_instance->flag =   SRVSAMPLE_CHANGING_COMPLETED;
    
    MMK_SampleTriggerEvent(MMISAMPLE_APP_CHANGE_COMPLETED,srv_handle,sizeof(srv_handle));

    return;
}

PUBLIC BOOLEAN SrvSample_GetResult(SRVSAMPLE_HANDLE_T *srv_handle)
{
    
}

PUBLIC BOOLEAN SrvSample_ChangeAgeAsyn(void *srv_handle_in,uint16 index,uint16 age)
{
    SRVSAMPLE_HANDLE_T *srv_handle = (SRVSAMPLE_HANDLE_T *)srv_handle_in;
    BOOLEAN ret = FALSE;

    if (srv_handle == PNULL)
    {
        //Raise Error
        return FALSE;
    }

    srv_handle->srv_instance->operation_data.index = index;
    srv_handle->srv_instance->operation_data.age = age;

    //create timer
    srv_handle->srv_instance->operation_data.change_timer_id = 
        MMK_CreateTimerCallback(5000, SrvSample_TimeOut, srv_handle, FALSE);

    if(srv_handle->srv_instance->operation_data.change_timer_id == 0)
    {   
        //Cann't create timer;
        return FALSE;
    }
    
    //set operation flag;
    srv_handle->srv_instance->flag = SRVSAMPLE_CHANGING_RUNNING;

    return TRUE;
}



PUBLIC void SrvSample_FreeSrv(void *srv_handle_in)
{
    SRVSAMPLE_HANDLE_T *srv_handle = (SRVSAMPLE_HANDLE_T *)srv_handle_in;
    SCI_FREE(srv_handle->srv_instance);
    SCI_FREE(srv_handle);
}
