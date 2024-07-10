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
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL SAMPLE_HANDLE CreateFSM(SAMPLE_CALLBACK callback);

/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFSM(SAMPLE_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : 分发请求消息
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(SAMPLE_HANDLE   fsm_handle,  SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void FsmStateTrans(SAMPLE_FSM_T *fsm_ptr, SAMPLE_STATE_E state);

/*****************************************************************************/
//  Description : 创建状态机消息
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 返回的指针需要调用DestroyMsg释放
/*****************************************************************************/
LOCAL SAMPLE_FSM_MSG_T* CreatMsg(SAMPLE_HANDLE fsm_handle, SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : 释放状态机消息
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(SAMPLE_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 准备好状态的处理
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 应用初始化
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  SAMPLE_Init(void);



const   FSM_DISPATCH  s_sample_fsm_dispatch[SAMPLE_STATE_MAX]    = {         //状态机的分配处理函数
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
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL SAMPLE_HANDLE CreateFSM(SAMPLE_CALLBACK callback)
{
    
    SAMPLE_FSM_T              *fsm_ptr              = PNULL;
    SAMPLE_FSM_LIST_T         *cur_fsm_node_ptr     = PNULL;

    //为状态机分配空间
    fsm_ptr = (SAMPLE_FSM_T*)SCI_ALLOCA(sizeof(SAMPLE_FSM_T));
    if (PNULL == fsm_ptr)
    {
        return 0;
    }
    SCI_MEMSET(fsm_ptr,0,sizeof(SAMPLE_FSM_T));

    //初始化状态机信息    
    fsm_ptr->state = SAMPLE_STATE_IDLE;
    fsm_ptr->samplecallback = callback;
	

    //给s_fsm_list加锁    
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);
    
    //将当前状态机插入到状态机列表中
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
    
    //给s_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
	s_current_fsm_ptr = fsm_ptr;
    
    return (SAMPLE_HANDLE)fsm_ptr;
}

/*****************************************************************************/
//  Description : 销毁状态机
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

    //从状态机列表中查找该状态机
    current_fsm_ptr = (SAMPLE_FSM_LIST_T *)s_fsm_list;
    if (PNULL != current_fsm_ptr && (SAMPLE_FSM_T *)fsm_handle == current_fsm_ptr->fsm_info_ptr)
    {
        //从状态机列表中删除该状态机
        s_fsm_list = (uint32)(current_fsm_ptr->next_fsm_ptr);
        //释放该状态机
        SCI_FREE(current_fsm_ptr->fsm_info_ptr);
        current_fsm_ptr->fsm_info_ptr = PNULL;
        SCI_FREE(current_fsm_ptr);
        current_fsm_ptr = PNULL;       
    }
    else if(PNULL != current_fsm_ptr)
    {
        pre_fsm_ptr = current_fsm_ptr;
        current_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
        ////从状态机列表中查找该状态机
        while (PNULL != current_fsm_ptr)
        {
            if ((SAMPLE_FSM_T *)fsm_handle == current_fsm_ptr->fsm_info_ptr)
            {
                //从状态机列表中删除该状态机
                pre_fsm_ptr->next_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
                //释放该状态机
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
     //给s_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
    s_current_fsm_ptr = PNULL;
}

/*****************************************************************************/
//  Description : 创建状态机消息
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 返回的指针需要调用DestroyMsg释放
/*****************************************************************************/
LOCAL SAMPLE_FSM_MSG_T* CreatMsg(SAMPLE_HANDLE fsm_handle, SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    SAMPLE_FSM_MSG_T    *msg_ptr        =   PNULL;
    
    //为状态机消息分配连续空间，便于后续释放
    
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
//  Description : 释放状态机消息
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
//  Description : 分发请求消息
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(SAMPLE_HANDLE fsm_handle,  SAMPLE_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    SAMPLE_FSM_T        *fsm_ptr    =  (SAMPLE_FSM_T *)fsm_handle;
    BOOLEAN				result      = FALSE;
    SAMPLE_FSM_MSG_T    *req_msg_ptr = PNULL;
    
    //参数检查 
    //判断状态是否有效
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
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void FsmStateTrans(SAMPLE_FSM_T *fsm_ptr, SAMPLE_STATE_E state)
{        
    //参数检查
    if(fsm_ptr == PNULL  || state >= SAMPLE_STATE_MAX)
    {
        return;
    }    
    FsmDispatch((SAMPLE_HANDLE)fsm_ptr,SAMPLE_EXIT_REQ, PNULL,0);       
    //转换状态
    fsm_ptr->state = state;    
    
    FsmDispatch((SAMPLE_HANDLE)fsm_ptr,SAMPLE_ENTRY_REQ, PNULL,0);    
    
}

/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr)
{
    BOOLEAN    result = TRUE;
    //参数检查
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
//  Description : 准备好状态的处理
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE; 
    uint32              service_id     = 0;
    
    //参数检查
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
//  Description : 应用初始化
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  SAMPLE_Init(void)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC  SAMPLE_HANDLE  SrvSample_FSMCreate(SAMPLE_CALLBACK callback)
{
    SAMPLE_HANDLE  fsm_handle=   0; 

    //创建状态机，并将状态机指针做为handle
    fsm_handle = CreateFSM(callback);

    return fsm_handle;
    
}

/*****************************************************************************/
//  Description : 封装接口示例
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SrvSample_FSMRelease(SAMPLE_HANDLE   fsm_handle)
{  
    BOOLEAN                  result         =   FALSE;
    
    
    //检查fsm_handle的有效性；
    if (0 == fsm_handle )
    {
        return FALSE; 
    } 
    
    result = FsmDispatch(fsm_handle,SAMPLE_RELEASE_REQ, PNULL,0); 
    return result;
}

