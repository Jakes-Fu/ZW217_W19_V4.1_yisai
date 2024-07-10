/*****************************************************************************
** File Name:      srvsample_fsm.c                                           *
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
#include "srvsample_fsm.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL SAMPLE_HANDLE CreateFSM(SAMPLE_CALLBACK callback);

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFSM(SAMPLE_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : �ַ�������Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(SAMPLE_HANDLE   fsm_handle,  SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : ת��״̬
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void FsmStateTrans(SAMPLE_FSM_T *fsm_ptr, SAMPLE_STATE_E state);

/*****************************************************************************/
//  Description : ����״̬����Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: ���ص�ָ����Ҫ����DestroyMsg�ͷ�
/*****************************************************************************/
LOCAL SAMPLE_FSM_MSG_T* CreatMsg(SAMPLE_HANDLE fsm_handle, SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : �ͷ�״̬����Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(SAMPLE_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : ����״̬�Ĵ���
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : ׼����״̬�Ĵ���
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : Ӧ�ó�ʼ��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  SAMPLE_Init(void);



const   FSM_DISPATCH  s_sample_fsm_dispatch[SAMPLE_STATE_MAX]    = {         //״̬���ķ��䴦����
		FsmIdle,
        FsmReady      
};

LOCAL SAMPLE_FSM_T	*s_current_fsm_ptr = PNULL;
LOCAL uint32		s_fsm_list = 0;
LOCAL SCI_MUTEX_PTR s_fsm_list_mutex = 0;
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL SAMPLE_HANDLE CreateFSM(SAMPLE_CALLBACK callback)
{
    
    SAMPLE_FSM_T              *fsm_ptr              = PNULL;
    SAMPLE_FSM_LIST_T         *cur_fsm_node_ptr     = PNULL;

    //Ϊ״̬������ռ�
    fsm_ptr = (SAMPLE_FSM_T*)SCI_ALLOCA(sizeof(SAMPLE_FSM_T));
    if (PNULL == fsm_ptr)
    {
        return 0;
    }
    SCI_MEMSET(fsm_ptr,0,sizeof(SAMPLE_FSM_T));

    //��ʼ��״̬����Ϣ    
    fsm_ptr->state = SAMPLE_STATE_IDLE;
    fsm_ptr->samplecallback = callback;
	

    //��s_fsm_list����    
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);
    
    //����ǰ״̬�����뵽״̬���б���
    if(s_fsm_list == 0)
    {
        cur_fsm_node_ptr = (SAMPLE_FSM_LIST_T* )SCI_ALLOCA(sizeof(SAMPLE_FSM_LIST_T));
        if (PNULL != cur_fsm_node_ptr)
        {
            cur_fsm_node_ptr->fsm_info_ptr = fsm_ptr;
            cur_fsm_node_ptr->next_fsm_ptr = PNULL;
            s_fsm_list = (uint32)cur_fsm_node_ptr;
        }
        else
        {
            SCI_FREE(fsm_ptr);
            fsm_ptr = PNULL;
        }
    }
    else
    {
        cur_fsm_node_ptr = (SAMPLE_FSM_LIST_T* )s_fsm_list;
        while (PNULL != cur_fsm_node_ptr->next_fsm_ptr)
        {
            cur_fsm_node_ptr = cur_fsm_node_ptr->next_fsm_ptr;
        }
        
        cur_fsm_node_ptr->next_fsm_ptr = (SAMPLE_FSM_LIST_T*)SCI_ALLOCA(sizeof(SAMPLE_FSM_LIST_T));
        if(PNULL != cur_fsm_node_ptr->next_fsm_ptr)
        {
            cur_fsm_node_ptr->next_fsm_ptr->fsm_info_ptr = fsm_ptr;
            cur_fsm_node_ptr->next_fsm_ptr->next_fsm_ptr = PNULL;
        }
        else
        {
            SCI_FREE(fsm_ptr);
            fsm_ptr = PNULL; 
        }
    }
    
    //��s_fsm_list����
    SCI_PutMutex(s_fsm_list_mutex);
	s_current_fsm_ptr = fsm_ptr;
    
    return (SAMPLE_HANDLE)fsm_ptr;
}

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFSM(SAMPLE_HANDLE fsm_handle)
{
    SAMPLE_FSM_LIST_T        *current_fsm_ptr = PNULL;
    SAMPLE_FSM_LIST_T        *pre_fsm_ptr = PNULL;
    
    if (s_fsm_list == PNULL)
    {
        return;
    }
   
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);

    //��״̬���б��в��Ҹ�״̬��
    current_fsm_ptr = (SAMPLE_FSM_LIST_T *)s_fsm_list;
    if (PNULL != current_fsm_ptr && (SAMPLE_FSM_T *)fsm_handle == current_fsm_ptr->fsm_info_ptr)
    {
        //��״̬���б���ɾ����״̬��
        s_fsm_list = (uint32)(current_fsm_ptr->next_fsm_ptr);
        //�ͷŸ�״̬��
        SCI_FREE(current_fsm_ptr->fsm_info_ptr);
        current_fsm_ptr->fsm_info_ptr = PNULL;
        SCI_FREE(current_fsm_ptr);
        current_fsm_ptr = PNULL;       
    }
    else if(PNULL != current_fsm_ptr)
    {
        pre_fsm_ptr = current_fsm_ptr;
        current_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
        ////��״̬���б��в��Ҹ�״̬��
        while (PNULL != current_fsm_ptr)
        {
            if ((SAMPLE_FSM_T *)fsm_handle == current_fsm_ptr->fsm_info_ptr)
            {
                //��״̬���б���ɾ����״̬��
                pre_fsm_ptr->next_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
                //�ͷŸ�״̬��
                SCI_FREE(current_fsm_ptr->fsm_info_ptr);
                current_fsm_ptr->fsm_info_ptr = PNULL;
                SCI_FREE(current_fsm_ptr);
                current_fsm_ptr= PNULL;        
                break;
            }
            else
            {
                pre_fsm_ptr = current_fsm_ptr;
                current_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
            }
        }
    }
     //��s_fsm_list����
    SCI_PutMutex(s_fsm_list_mutex);
    s_current_fsm_ptr = PNULL;
}

/*****************************************************************************/
//  Description : ����״̬����Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: ���ص�ָ����Ҫ����DestroyMsg�ͷ�
/*****************************************************************************/
LOCAL SAMPLE_FSM_MSG_T* CreatMsg(SAMPLE_HANDLE fsm_handle, SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    SAMPLE_FSM_MSG_T    *msg_ptr        =   PNULL;
    
    //Ϊ״̬����Ϣ���������ռ䣬���ں����ͷ�
    
    msg_ptr = (SAMPLE_FSM_MSG_T*)SCI_ALLOCA(sizeof(SAMPLE_FSM_MSG_T)+msg_body_size);
    
    if(PNULL != msg_ptr)
    {
        SCI_MEMSET(msg_ptr,0,(sizeof(SAMPLE_FSM_MSG_T)+ msg_body_size));
        msg_ptr->fsm_handle = fsm_handle;
        msg_ptr->msg_id = msg_id;
        msg_ptr->msg_size = sizeof(SAMPLE_FSM_MSG_T);
        if (0 == msg_body_size)
        {
            msg_ptr->msg_body = PNULL;
        }
        else
        {
            msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(SAMPLE_FSM_MSG_T));
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
LOCAL BOOLEAN DestroyMsg(SAMPLE_FSM_MSG_T *msg_ptr)
{
    if (PNULL != msg_ptr)
    {
        SCI_FREE(msg_ptr);
        msg_ptr = PNULL;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : �ַ�������Ϣ
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(SAMPLE_HANDLE fsm_handle,  SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    SAMPLE_FSM_T        *fsm_ptr    =  (SAMPLE_FSM_T *)fsm_handle;
    BOOLEAN				result      = FALSE;
    SAMPLE_FSM_MSG_T    *req_msg_ptr = PNULL;
    
    //������� 
    //�ж�״̬�Ƿ���Ч
    if(PNULL == fsm_ptr || (PNULL == msg_body_ptr && 0 != msg_body_size))
    {        
        return FALSE;
    }
    
    req_msg_ptr = CreatMsg(fsm_handle,msg_id, msg_body_ptr,msg_body_size);    
    if (PNULL != req_msg_ptr && fsm_ptr->state < SAMPLE_STATE_MAX && PNULL != s_sample_fsm_dispatch[fsm_ptr->state])
    {
        result = (*s_sample_fsm_dispatch[fsm_ptr->state])(fsm_ptr,req_msg_ptr);      
        
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
LOCAL void FsmStateTrans(SAMPLE_FSM_T *fsm_ptr, SAMPLE_STATE_E state)
{        
    //�������
    if(fsm_ptr == PNULL  || state >= SAMPLE_STATE_MAX)
    {
        return;
    }    
    FsmDispatch((SAMPLE_HANDLE)fsm_ptr,SAMPLE_EXIT_REQ, PNULL,0);       
    //ת��״̬
    fsm_ptr->state = state;    
    
    FsmDispatch((SAMPLE_HANDLE)fsm_ptr,SAMPLE_ENTRY_REQ, PNULL,0);    
    
}

/*****************************************************************************/
//  Description : ����״̬�Ĵ���
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr)
{
    BOOLEAN    result = TRUE;
    //�������
    if(PNULL == fsm_ptr || PNULL == msg_ptr)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[srvsample] FsmIdle msg_ptr->msg_id = %x",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case SAMPLE_INIT_REQ:
		result = SAMPLE_Init();
        if(result)
        {
            FsmStateTrans(fsm_ptr,SAMPLE_STATE_READY);    
        }     
        break;    
        
    case SAMPLE_RELEASE_REQ:
        DestroyFSM((SAMPLE_HANDLE)fsm_ptr);
        break;
    default:
        result =  FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : ׼����״̬�Ĵ���
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE; 
    uint32              service_id     = 0;
    
    //�������
    if(PNULL == fsm_ptr || PNULL == msg_ptr)
    {       
        return FALSE;
    }
    
    switch(msg_ptr->msg_id)
    {
    case SAMPLE_CLOSE_REQ:
        FsmStateTrans(fsm_ptr,SAMPLE_STATE_IDLE);          
        break;  

    default:       
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : Ӧ�ó�ʼ��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  SAMPLE_Init(void)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC  SAMPLE_HANDLE  SrvSample_FSMCreate(SAMPLE_CALLBACK callback)
{
    SAMPLE_HANDLE  fsm_handle=   0; 

    //����״̬��������״̬��ָ����Ϊhandle
    fsm_handle = CreateFSM(callback);

    return fsm_handle;
    
}

/*****************************************************************************/
//  Description : ��װ�ӿ�ʾ��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SrvSample_FSMRelease(SAMPLE_HANDLE   fsm_handle)
{  
    BOOLEAN                  result         =   FALSE;
    
    
    //���fsm_handle����Ч�ԣ�
    if (0 == fsm_handle )
    {
        return FALSE; 
    } 
    
    result = FsmDispatch(fsm_handle,SAMPLE_RELEASE_REQ, PNULL,0); 
    return result;
}

