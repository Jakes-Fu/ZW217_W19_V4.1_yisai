/*****************************************************************************
** File Name:      mmisrv_fsm.c                                              *
** Author:                                                                   *
** Date:           2012/7/3													 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe fsm						 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/7/3       aoke.hu			Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmisrv_fsm.h"
#include "os_api.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
//״̬����Ϣ�ṹ
typedef struct  
{    
    FSM_STATE_FUNC              state_func; //״̬��״̬����  
	FSM_OBJECT					object_data;
}FSMSRV_T;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ����״̬����Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: ���ص�ָ����Ҫ����DestroyMsg�ͷ�
/*****************************************************************************/
LOCAL FSMSRV_MSG_T* CreatMsg(FSMSRV_HANDLE fsm_handle, uint32 msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : �ͷ�״̬����Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(FSMSRV_MSG_T *msg_ptr);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ����״̬����Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: ���ص�ָ����Ҫ����DestroyMsg�ͷ�
/*****************************************************************************/
LOCAL FSMSRV_MSG_T* CreatMsg(FSMSRV_HANDLE fsm_handle, uint32 msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    FSMSRV_MSG_T    *msg_ptr        =   PNULL;
    
    //Ϊ״̬����Ϣ���������ռ䣬���ں����ͷ�
    
    msg_ptr = (FSMSRV_MSG_T*)SCI_ALLOCA(sizeof(FSMSRV_MSG_T)+msg_body_size);
    
    if(PNULL != msg_ptr)
    {
        SCI_MEMSET(msg_ptr,0,(sizeof(FSMSRV_MSG_T)+ msg_body_size));
        msg_ptr->fsm_handle = fsm_handle;
        msg_ptr->msg_id = msg_id;
        msg_ptr->msg_size = sizeof(FSMSRV_MSG_T);
        if (0 == msg_body_size)
        {
            msg_ptr->msg_body = PNULL;
        }
        else
        {
            msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(FSMSRV_MSG_T));
            if (PNULL != msg_ptr->msg_body)
            {                
                SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,msg_body_size);
            }
        } 
    }    

    return msg_ptr;
}

/*****************************************************************************/
//  Description : �ͷ�״̬����Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(FSMSRV_MSG_T *msg_ptr)
{
    if (PNULL != msg_ptr)
    {
        SCI_FREE(msg_ptr);
        msg_ptr = PNULL;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSMSRV_HANDLE FSMSRV_Construct(FSM_OBJECT		fsm_object,
									  uint32			fsm_object_size,
									  FSM_STATE_FUNC	fsm_state_init
									  )
{
	FSMSRV_T              *fsm_ptr  = PNULL;

    //Ϊ״̬������ռ�
    fsm_ptr = (FSMSRV_T*)SCI_ALLOCA(sizeof(FSMSRV_T)+fsm_object_size);
    if (PNULL == fsm_ptr)
    {
        return 0;
    }
    SCI_MEMSET(fsm_ptr,0,sizeof(FSMSRV_T));

    //��ʼ��״̬����Ϣ    
	if (0 == fsm_object_size)
	{
		fsm_ptr->object_data = PNULL;
	}
	else
	{
		fsm_ptr->object_data = (FSM_OBJECT)((uint32)fsm_ptr+sizeof(FSMSRV_T));
		if (PNULL != fsm_ptr->object_data)
		{                
			SCI_MEMCPY(fsm_ptr->object_data,fsm_object,fsm_object_size);
		}
	}
	fsm_ptr->state_func = fsm_state_init;
	if(!FSMSRV_Dispatch((FSMSRV_HANDLE)fsm_ptr,FSMSRV_INIT_REQ, PNULL,0))
    {
        FSMSRV_Destruct((FSMSRV_HANDLE*)&fsm_ptr);
    }

    return (FSMSRV_HANDLE)fsm_ptr;
}

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void FSMSRV_Destruct(FSMSRV_HANDLE *fsm_handle_ptr)
{ 
    if (PNULL != fsm_handle_ptr && PNULL != *fsm_handle_ptr)
    {
        SCI_FREE(*fsm_handle_ptr);/*lint !e64*/
        *fsm_handle_ptr = PNULL;
    }
    //�ͷŸ�״̬��
    
}

/*****************************************************************************/
//  Description : �ַ�������Ϣ msg_body_ptr����Ƕ��ָ�룬���������л���
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN FSMSRV_Dispatch(FSMSRV_HANDLE fsm_handle,  uint32 msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    FSMSRV_T        *fsm_ptr    =  (FSMSRV_T *)fsm_handle;
    BOOLEAN			result      = FALSE;
    FSMSRV_MSG_T    *req_msg_ptr = PNULL;
    
    //������� 
    //�ж�״̬�Ƿ���Ч
    if(PNULL == fsm_ptr || (PNULL == msg_body_ptr && 0 != msg_body_size))
    {        
        return FALSE;
    }
    
    req_msg_ptr = CreatMsg(fsm_handle,msg_id, msg_body_ptr,msg_body_size);    
    if (PNULL != req_msg_ptr && PNULL != fsm_ptr->state_func)
    {
        result = (*fsm_ptr->state_func)(fsm_handle,req_msg_ptr);      
        
    } 
    DestroyMsg(req_msg_ptr); 
    return result;
}

/*****************************************************************************/
//  Description : ת��״̬
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void FSMSRV_StateTrans(FSMSRV_HANDLE fsm_handle, FSM_STATE_FUNC	fsm_func)
{   
	FSMSRV_T   *fsm_ptr =  (FSMSRV_T *)fsm_handle;
    //�������
    if(PNULL == fsm_ptr  || PNULL == fsm_func)
    {
        return;
    }    
    FSMSRV_Dispatch(fsm_handle,FSMSRV_EXIT_REQ, PNULL,0);       
    //ת��״̬ 
	fsm_ptr->state_func = fsm_func;
    
    FSMSRV_Dispatch(fsm_handle,FSMSRV_ENTRY_REQ, PNULL,0);     
}

/*****************************************************************************/
//  Description : ��ȡ��ǰ״̬����
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSM_STATE_FUNC FSMSRV_GetState(FSMSRV_HANDLE fsm_handle)
{
	FSMSRV_T   *fsm_ptr =  (FSMSRV_T *)fsm_handle;
	FSM_STATE_FUNC fsm_func = PNULL;

	if(PNULL != fsm_ptr)
	{
		fsm_func = fsm_ptr->state_func;
	}

	return fsm_func;
}

/*****************************************************************************/
//  Description : ��ȡ�������ݽṹ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSM_OBJECT FSMSRV_GetObject(FSMSRV_HANDLE fsm_handle)
{
	FSMSRV_T   *fsm_ptr =  (FSMSRV_T *)fsm_handle;
	FSM_OBJECT fsm_object = PNULL;

	if(PNULL != fsm_ptr)
	{
		fsm_object = fsm_ptr->object_data;
	}

	return fsm_object;
}

/****************%%%%%%%%%%%%%%%%*********************&&&&&&&&&&&&&&&******/
/****************%%%%%%%%%%%%%%%%*********************&&&&&&&&&&&&&&&******/
/*---------------------------------���Է���-------------------------------*/
/****************%%%%%%%%%%%%%%%%*********************&&&&&&&&&&&&&&&******/
/****************%%%%%%%%%%%%%%%%*********************&&&&&&&&&&&&&&&******/
//#define FSMSRV_DEBUG_SAMPLE

#ifdef FSMSRV_DEBUG_SAMPLE

LOCAL BOOLEAN SAMPLE_Init(FSMSRV_HANDLE fsm_handle);
LOCAL BOOLEAN SAMPLE_Open(FSMSRV_HANDLE fsm_handle);
LOCAL BOOLEAN SAMPLE_Close(FSMSRV_HANDLE fsm_handle);

LOCAL BOOLEAN FsmState_Idle(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);
LOCAL BOOLEAN FsmState_Ready(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);
LOCAL BOOLEAN FsmState_Playing(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);

FSMSRV_HANDLE s_sample_fsm_handle=0;

//״̬�����յ���Ϣ
typedef enum
{
	SAMPLE_FSM_OPEN_REQ = FSMSRV_USER_REQ,
	SAMPLE_FSM_PLAY_REQ,
	SAMPLE_FSM_STOP_REQ,
	SAMPLE_FSM_RELEASE_REQ,       
	SAMPLE_FSM_CLOSE_REQ,
    SAMPLE_FSM_MSG_MAX
}SAMPLE_FSM_MSG_E; 

typedef struct  
{
	uint32						index; 
	uint8						level;
	BOOLEAN                     is_test;
}SAMPLE_FSM_T;


LOCAL BOOLEAN SAMPLE_Init(FSMSRV_HANDLE fsm_handle)
{
	return TRUE;
}

LOCAL BOOLEAN SAMPLE_Open(FSMSRV_HANDLE fsm_handle)
{
	return TRUE;
}

LOCAL BOOLEAN SAMPLE_Close(FSMSRV_HANDLE fsm_handle)
{
	return TRUE;
}


LOCAL BOOLEAN FsmState_Idle(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{
    BOOLEAN    result = TRUE;
    //�������
    if(PNULL == fsm_handle || PNULL == msg_ptr)
    {
        return FALSE;
    }

    switch(msg_ptr->msg_id)
    {
    case FSMSRV_INIT_REQ:
        result = SAMPLE_Init(fsm_handle);  //FSMSRV_INIT_REQ��Ϣ����FALSE������״̬����
        break;

    case SAMPLE_FSM_OPEN_REQ:
        result = SAMPLE_Open(fsm_handle);
        if(result)
        {
            FSMSRV_StateTrans(fsm_handle,FsmState_Ready);    
        }       
        break;

    case SAMPLE_FSM_RELEASE_REQ:
		FSMSRV_Destruct(&fsm_handle);
        s_sample_fsm_handle=0;
        break;

    default:
        result =  FALSE;
        break;
    }
    return result;
}

LOCAL BOOLEAN FsmState_Ready(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{
    BOOLEAN    result = TRUE;
	SAMPLE_FSM_T *fsm_ptr = PNULL;
	uint32 index=0;
    //�������
    if(PNULL == fsm_handle || PNULL == msg_ptr)
    {
        return FALSE;
    }
	

    switch(msg_ptr->msg_id)
    {
	case FSMSRV_ENTRY_REQ:       
        break;  
		
	case SAMPLE_FSM_PLAY_REQ:
		if (PNULL != msg_ptr->msg_body)
        {
            index = *(uint32 *)msg_ptr->msg_body;
        }
		fsm_ptr = (SAMPLE_FSM_T *)FSMSRV_GetObject(fsm_handle);
		if(PNULL!=fsm_ptr)
		{
			fsm_ptr->index=index;
		}
		FSMSRV_StateTrans(fsm_handle,FsmState_Playing);
        break;  
        
    case SAMPLE_FSM_CLOSE_REQ:
		SAMPLE_Close(fsm_handle);
        FSMSRV_StateTrans(fsm_handle,FsmState_Idle);
        break;
        
    default:
        result =  FALSE;
        break;
    }
    return result;
}

LOCAL BOOLEAN FsmState_Playing(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{
    BOOLEAN    result = TRUE;
    //�������
    if(PNULL == fsm_handle || PNULL == msg_ptr)
    {
        return FALSE;
    }

    switch(msg_ptr->msg_id)
    {
	case FSMSRV_ENTRY_REQ:       
        break; 

    case SAMPLE_FSM_STOP_REQ: 
		FSMSRV_StateTrans(fsm_handle,FsmState_Ready);
        break;    
        
    default:
        result =  FALSE;
        break;
    }
    return result;
}

PUBLIC BOOLEAN MMISAMPLE_Play(void)
{
	uint32 index=23;
	BOOLEAN result = FALSE;

	result = FSMSRV_Dispatch(s_sample_fsm_handle,SAMPLE_FSM_PLAY_REQ, &index,sizeof(index));

	return result;
}

PUBLIC BOOLEAN MMISAMPLE_IsPlaying(void)
{
	FSM_STATE_FUNC state_func=FSMSRV_GetState(s_sample_fsm_handle);
	
	if(FsmState_Playing == state_func)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

PUBLIC void MMISAMPLE_Stop(void)
{
	FSMSRV_Dispatch(s_sample_fsm_handle,SAMPLE_FSM_STOP_REQ, PNULL,0);
}

PUBLIC void MMISAMPLE_Close(void)
{
	FSMSRV_Dispatch(s_sample_fsm_handle,SAMPLE_FSM_CLOSE_REQ, PNULL,0);
}

PUBLIC void MMISAMPLE_Release(void)
{  
    FSMSRV_Dispatch(s_sample_fsm_handle,SAMPLE_FSM_RELEASE_REQ, PNULL,0);
}

PUBLIC void MMISAMPLE_Exit(void)
{
	MMISAMPLE_Stop();
	MMISAMPLE_Close();
    MMISAMPLE_Release();
}

PUBLIC void MMISAMPLE_Entry(void)
{
	SAMPLE_FSM_T sample_t={0};
	sample_t.index=8;
	sample_t.level=10;
	sample_t.is_test=TRUE;

	s_sample_fsm_handle=FSMSRV_Construct((FSM_OBJECT)(&sample_t),sizeof(sample_t),FsmState_Idle);
    //FsmState_Idle����Ҫ��ȷ����FSMSRV_INIT_REQ��Ϣ
}

#endif