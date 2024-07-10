/*****************************************************************************
** File Name:      mmiatv_fsm.c                                             *
** Author:                                                                   *
** Date:           2012-06                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe atv flow control            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2012        apple.zhang           Create                                   *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiatv_fsm.h"
#include "mmi_app_atv_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_msg.h"
#include "dal_dcamera.h"
#include "dal_recorder.h"
#include "dal_player.h"
#include "mmieng_export.h"
#include "Mmisrvaud_api.h"
#include "mmiatv_data.h"
#include "mmiatv_id.h"
#include "mmiatv_main.h"
#include "mmiatv_position.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define AUDIO_CHANNEL                       1
#define AUDIO_SAMPLE_RATE                   8000
#define VIDEO_320X240_HEIGHT                240
#define VIDEO_320X240_WIDTH                 320

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 准备好状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 录制状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmRecording(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);
/*****************************************************************************/
//  Description : 播放中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlaying(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 设置频道
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSettingCHN(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 搜索中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSearching(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 终止视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Close(void);

/*****************************************************************************/
//  Description : 终止视频播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Stop(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : 发起视频搜索
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Search(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : 停止视频搜索
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_StopSearch(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : 发起视频播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Play(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  ATV_Open(void);

/*****************************************************************************/
//  Description : open ATV_StopRecord
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_StopRecord(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyUIMsg(MMI_MESSAGE_ID_E msg_id, DPARAM param_ptr,  uint32 size_of_param);

/*****************************************************************************/
//  Description : set channel id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetATVChannelID(uint32 service_id,
                              DCAMERA_ATV_CHN_OP_MODE_E op_mode);

/*****************************************************************************/
//  Description : report Callback
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 ReportCallback(DRECORDER_REPORT_DATA_U *report_ptr);

/*****************************************************************************/
//  Description : 录制的callback函数
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void RecordEndCallBack(int32 i_type);

/*****************************************************************************/
//  Description : get preview param
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void GetPreviewParam(DRECORDER_PREVIEW_PARAM_T *preview_param_ptr);

/*****************************************************************************/
//  Description : ParamCtrlCallBack
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL uint32 ParamCtrlCallBack(void *param_ptr, uint32 param_size);

/*****************************************************************************/
//  Description : ParamCtrlCallBack
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHN_ParamCtrlCallBack(void *param_ptr, uint32 param_size);

/*****************************************************************************/
//  Description : 处理callback消息
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleReportMsg(DRECORDER_REPORT_DATA_U *param_ptr);

/*****************************************************************************/
//  Description : ATV_Record
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Record(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : send signal to task
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SendSignalToTask(uint16 sig_code,void *signal_data_ptr, uint32 task_id);

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Init(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : release
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void ATV_Release(FSMSRV_HANDLE fsm_handle);

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
LOCAL FSMSRV_HANDLE s_fsm_handle = 0;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{
    BOOLEAN    result = TRUE;
    //参数检查
    if(PNULL ==fsm_handle || PNULL == msg_ptr)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("[ATV] FsmIdle msg_ptr->msg_id = %x",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSMSRV_INIT_REQ:
        if(!ATV_Init(fsm_handle))
        {
            result =  FALSE;
        }
        break;

    case ATV_OPEN_REQ:
        result = ATV_Open();
        if(result)
        {
            FSMSRV_StateTrans(fsm_handle,FsmReady);    
        }
		else
		{
			SCI_TRACE_LOW("[ATV] FsmIdle ATV_Open failed!");
		}
        break;    
        
    case ATV_RELEASE_REQ:
        ATV_Release(fsm_handle);
		FSMSRV_Destruct(&fsm_handle);
        s_fsm_handle=0;
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE; 
    uint32              service_id     = 0;
    ATV_FSM_T           *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    //参数检查
    if(PNULL == atv_fsm_ptr || PNULL == msg_ptr)
    {       
        return FALSE;
    }
    
    switch(msg_ptr->msg_id)
    {
    case FSMSRV_ENTRY_REQ:
        if (atv_fsm_ptr->is_close)
        {
            atv_fsm_ptr->is_close = FALSE;
            MMIATV_FSMClose(fsm_handle);           
        }
        break;
        
    case ATV_PLAY_REQ:
        if (PNULL != msg_ptr->msg_body)
        {
            service_id = *(uint32 *)msg_ptr->msg_body;
        }
        SCI_TRACE_LOW("[ATV] FsmReady service_id = %d",service_id);
        atv_fsm_ptr->service_id = service_id;
        result = ATV_Play(fsm_handle);
        if(result)
        {
            FSMSRV_StateTrans(fsm_handle,FsmPlaying);  
        }
		else
		{
            SCI_TRACE_LOW("[ATV] FsmReady ATV_Play failed!");
		}
        break;
        
    case ATV_SEARCH_REQ:
        result = ATV_Search(fsm_handle);
        if (result)
        {
            FSMSRV_StateTrans(fsm_handle,FsmSearching);     
        }
		else
		{
            SCI_TRACE_LOW("[ATV] FsmReady ATV_Search failed!");
		}
        break;
        
    case ATV_CLOSE_REQ:
        ATV_Close();
        FSMSRV_StateTrans(fsm_handle,FsmIdle);          
        break;  
        
    
    default:       
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 搜索状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSearching(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE;
    MMIATV_SERVICE_NODE_T* service_ptr = PNULL;
    ATV_FSM_T           *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    //参数检查
    if(PNULL == atv_fsm_ptr || PNULL == msg_ptr)
    {       
        return FALSE;
    }
    
    SCI_TRACE_LOW("[ATV] FsmSearching msg_ptr->msg_id = %x",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    { 
        
    case ATV_STOP_SEARCH_REQ:
        result = ATV_StopSearch(fsm_handle);  
        FSMSRV_StateTrans(fsm_handle,FsmReady);
        break;
        
    case ATV_SEARCH_CNF:
        atv_fsm_ptr->service_id = 0;
        if(MMIATV_GetServiceListNum() > 0)
        {
            service_ptr = MMIATV_GetServiceNodeViaIndex(0);
            if(PNULL != service_ptr)
            {
                atv_fsm_ptr->service_id = service_ptr->service_id;
            }      
        }
        FSMSRV_StateTrans(fsm_handle,FsmReady); 
        NotifyUIMsg(MSG_APP_ATV_SEARCH_CNF,PNULL,PNULL);          
        break;  
        
    default:       
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 播放状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlaying(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE;   
    uint32              service_id     = 0; 
    ATV_FSM_T           *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    //参数检查
    if(PNULL == atv_fsm_ptr || PNULL == msg_ptr)
    {       
        return FALSE;
    }
    
    SCI_TRACE_LOW("[ATV] FsmPlaying msg_ptr->msg_id = %x",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case ATV_STOP_REQ:
        result = ATV_Stop(fsm_handle);
        if (result)
        {
            SCI_TRACE_LOW("[ATV] FsmPlaying,ATV_Stop successfully. result = %d",result);
            FSMSRV_StateTrans(fsm_handle,FsmReady);    
        }
        else
        {
            SCI_TRACE_LOW("[ATV] FsmPlaying,ATV_Stop fail. result = %d",result);
            FSMSRV_StateTrans(fsm_handle,FsmReady);    
        }           
        break;       
        
    case ATV_RECORD_REQ:
        result = ATV_Record(fsm_handle);
        if (result)
        {
            FSMSRV_StateTrans(fsm_handle,FsmRecording); 
        }
		else
		{
			SCI_TRACE_LOW("[ATV] FsmPlaying ATV_Record failed!");
		}
        break;
        
    case ATV_PLAY_REQ:
        if (PNULL != msg_ptr->msg_body)
        {
            service_id = *(uint32 *)msg_ptr->msg_body;
        }
        SCI_TRACE_LOW("[ATV] FsmPlaying service_id = %d",service_id);
        atv_fsm_ptr->service_id = service_id;
        result = SetATVChannelID(atv_fsm_ptr->service_id,DCAMERA_ATV_CHN_NORMAL_OP);
        if (result)
        {
            FSMSRV_StateTrans(fsm_handle,FsmSettingCHN);
        }
		else
		{
			SCI_TRACE_LOW("[ATV] FsmPlaying SetATVChannelID failed!");
		}
        break;
        
    default:       
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 设置频道
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSettingCHN(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE;   
    ATV_FSM_T           *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    //参数检查
    if(PNULL == atv_fsm_ptr || PNULL == msg_ptr)
    {       
        return FALSE;
    }
    
    SCI_TRACE_LOW("[ATV] FsmSettingCHN msg_ptr->msg_id = %x",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case ATV_STOP_REQ:
        result = ATV_Stop(fsm_handle);
        if (result)
        {
            FSMSRV_StateTrans(fsm_handle,FsmReady);    
        }
		else
		{
			SCI_TRACE_LOW("[ATV] FsmSettingCHN ATV_Stop failed!");
		}
        break;  
        
    case ATV_CHN_CNF:
        MMISRVAUD_SetAtvServiceId(atv_fsm_ptr->srv_handle,atv_fsm_ptr->service_id);
        FSMSRV_StateTrans(fsm_handle,FsmPlaying);  
        break;  
        
        
    default:       
        result = FALSE;
        break;
    }
    return result;
}


/*****************************************************************************/
//  Description : 录制状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmRecording(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE;      
    
    
    //参数检查
    if(PNULL == fsm_handle ||  PNULL == msg_ptr)
    {       
        return FALSE;
    }
    
    SCI_TRACE_LOW("[ATV] FsmRecording msg_ptr->msg_id = %x",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case ATV_STOP_RECORD_REQ:        
        result = ATV_StopRecord(fsm_handle);
        if(PNULL != msg_ptr->msg_body
			&& *(BOOLEAN *)msg_ptr->msg_body)
        {           
            FSMSRV_StateTrans(fsm_handle,FsmReady);     
        }
		else
		{
			SCI_TRACE_LOW("[ATV] FsmRecording ATV_STOP_RECORD_REQ failed!");
		}

        break;       
        
    case ATV_RECORD_END:
		if(PNULL != msg_ptr->msg_body)
		{
			if(0 == *(int32 *)(msg_ptr->msg_body))
			{
	            result = ATV_StopRecord(fsm_handle);//to avoid ATV_RECORD_END before ATV_STOP_RECORD_REQ when space of T-card is insufficient  
				SCI_TRACE_LOW("[ATV] FsmRecording ATV_StopRecord result = %d",result);
				
			}
		}
		
        FSMSRV_StateTrans(fsm_handle,FsmReady);
        NotifyUIMsg(MSG_APP_ATV_RECORD_END,msg_ptr->msg_body,sizeof(int32));
        break;
        
        
    default:       
        result = FALSE;
        break;
    }
    return result;
}



/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC  FSMSRV_HANDLE  MMIATV_FSMCreate(void)
{
    FSMSRV_HANDLE		atv_handle  =  0; 
    ATV_FSM_T			atv_fsm = {0};
    
    //创建状态机，并将状态机指针做为handle
    atv_handle = FSMSRV_Construct((FSM_OBJECT)(&atv_fsm),sizeof(atv_fsm),FsmIdle);
    s_fsm_handle = atv_handle;
    //返回v handle
    SCI_TRACE_LOW("[ATV] MMIATV_FSMCreate atv_handle = %x",atv_handle);
    return atv_handle;
    
}

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIATV_FSMOpen(FSMSRV_HANDLE   atv_handle)
{     
    BOOLEAN                 result          =   FALSE;
    
    if (0 == atv_handle)
    {
        return FALSE;
    }    
    //向 control发送消息
    result = FSMSRV_Dispatch(atv_handle,ATV_OPEN_REQ, PNULL,PNULL);
    SCI_TRACE_LOW("[ATV] MMIATV_FSMOpen result = %d",result);    
    
    //返回v handle
    return result;
    
}


/*****************************************************************************/
//  Description : 发起视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMPlay(FSMSRV_HANDLE   atv_handle, uint32 service_id )
{   
    BOOLEAN                 result          =   FALSE;
    //检查的有效性；
    
    if (0 == atv_handle)
    {
        return FALSE; 
    }
    //向 control发送消息
    result = FSMSRV_Dispatch(atv_handle,ATV_PLAY_REQ, &service_id,sizeof(service_id));
    SCI_TRACE_LOW("[ATV] MMIATV_FSMPlay result = %d",result);
    
    return result;
    
}

/*****************************************************************************/
//  Description : MMIATV_FSMSearch
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMSearch(FSMSRV_HANDLE   atv_handle)
{   
    BOOLEAN                 result          =   FALSE;
    //检查的有效性；
    
    if (0 == atv_handle)
    {
        return FALSE; 
    }
    //向 control发送消息
    result = FSMSRV_Dispatch(atv_handle,ATV_SEARCH_REQ, PNULL,PNULL);    
    SCI_TRACE_LOW("[ATV] MMIATV_FSMSearch result = %d",result);
    return result;
    
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMStopSearch(FSMSRV_HANDLE   atv_handle)
{   
    BOOLEAN                 result          =   FALSE;
    //检查的有效性；
    
    if (0 == atv_handle)
    {
        return FALSE; 
    }
    //向 control发送消息
    result = FSMSRV_Dispatch(atv_handle,ATV_STOP_SEARCH_REQ, PNULL,PNULL);    
    SCI_TRACE_LOW("[ATV] MMIATV_FSMStopSearch result = %d",result);
    return result;
    
}
/*****************************************************************************/
//  Description : MMIATV_FSMStop
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMStop(FSMSRV_HANDLE   atv_handle)
{  
    BOOLEAN                  result         =   FALSE;
    
    //检查vp_handler的有效性；
    if (0 == atv_handle )
    {
        return FALSE; 
    } 
    
    result = FSMSRV_Dispatch(atv_handle,ATV_STOP_REQ, PNULL,0);   
    SCI_TRACE_LOW("[ATV] MMIATV_FSMStop result = %d",result);
    return result;
}

/*****************************************************************************/
//  Description : MMIATV_FSMRecord
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMRecord(FSMSRV_HANDLE   atv_handle)
{  
    BOOLEAN                  result         =   FALSE;
    
    
    //检查vp_handler的有效性；
    if (0 == atv_handle )
    {
        return FALSE; 
    } 
    
    result = FSMSRV_Dispatch(atv_handle,ATV_RECORD_REQ, PNULL,0);   
    SCI_TRACE_LOW("[ATV] MMIATV_FSMRecord result = %d",result);
    
    return result;
}

/*****************************************************************************/
//  Description : MMIATV_FSMStopRecord
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMStopRecord(FSMSRV_HANDLE   atv_handle,
									BOOLEAN is_stop_now)
{  
    BOOLEAN                  result         =   FALSE;
    
    
    //检查vp_handler的有效性；
    if (0 == atv_handle )
    {
        return FALSE; 
    } 
    
    result = FSMSRV_Dispatch(atv_handle,ATV_STOP_RECORD_REQ, &is_stop_now,sizeof(BOOLEAN));   
    SCI_TRACE_LOW("[ATV] MMIATV_FSMStopRecord result = %d",result);
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMClose(FSMSRV_HANDLE   atv_handle)
{  
    BOOLEAN             result         =   FALSE;
    ATV_FSM_T           *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(atv_handle);
    
    //检查vp_handler的有效性；
    if (PNULL ==atv_fsm_ptr || 0 == atv_handle )
    {
        return FALSE; 
    } 
    
    result = FSMSRV_Dispatch(atv_handle,ATV_CLOSE_REQ, PNULL,0);  
    
    if (!result)
    {
        atv_fsm_ptr->is_close = TRUE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMRelease(FSMSRV_HANDLE   atv_handle)
{  
    BOOLEAN                  result         =   FALSE;
    
    
    //检查vp_handler的有效性；
    if (0 == atv_handle )
    {
        return FALSE; 
    } 
    
    result = FSMSRV_Dispatch(atv_handle,ATV_RELEASE_REQ, PNULL,0);   
    SCI_TRACE_LOW("[ATV] MMIATV_FSMRelease result = %d",result);
    return result;
}
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Init(FSMSRV_HANDLE fsm_handle)
{
    BOOLEAN   result  = FALSE;
    ATV_FSM_T *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    if (PNULL != atv_fsm_ptr)
    {
        SCI_MEMSET(atv_fsm_ptr,0,sizeof(ATV_FSM_T));
        atv_fsm_ptr->srv_handle = MMIATV_RequestHandle();
        atv_fsm_ptr->is_close = FALSE;
        if(0 != atv_fsm_ptr->srv_handle)
        {   
            result = TRUE;
        }
        SCI_TRACE_LOW("[ATV] ATV_Init atv_fsm_ptr->srv_handle = %x",atv_fsm_ptr->srv_handle);
        
    }
    return result;      
}

/*****************************************************************************/
//  Description : release
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void ATV_Release(FSMSRV_HANDLE fsm_handle)
{
    ATV_FSM_T *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    if (PNULL != atv_fsm_ptr)
    {
        MMISRVAUD_Stop(atv_fsm_ptr->srv_handle);
        MMISRVMGR_Free(atv_fsm_ptr->srv_handle);
		atv_fsm_ptr->srv_handle = PNULL;
    }     
}

/*****************************************************************************/
//  Description : 终止视频播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Close(void)
{
    if (DRECORDER_SUCCESS == DRECORDER_Close())
    {
        return TRUE;
    } 
    return FALSE;
}

/*****************************************************************************/
//  Description : 终止视频播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Stop(FSMSRV_HANDLE fsm_handle)
{
    ATV_FSM_T   *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    if (PNULL == atv_fsm_ptr)
    {
        return FALSE;
    }
    return MMISRVAUD_Stop(atv_fsm_ptr->srv_handle);        
}

/*****************************************************************************/
//  Description : 发起视频搜索
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Search(FSMSRV_HANDLE fsm_handle)
{
    DRECORDER_PREVIEW_PARAM_T preview_param = {0};/*lint !e64*/
    DRECORDER_RETURN_E        search_ret = DRECORDER_SUCCESS; 
    ATV_FSM_T   *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    if (PNULL == atv_fsm_ptr)
    {
        return FALSE;
    }
    
    atv_fsm_ptr->service_id = 0;   
    
    //set region
    DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_ATV_REGION, MMIATV_GetRegion(), ParamCtrlCallBack);
    
    //get total num before search
    atv_fsm_ptr->search_cur_num = 0;
    atv_fsm_ptr->search_valid_num = 0;
    atv_fsm_ptr->search_total_num = 0;
    DRECORDER_GetInfo(DRECORDER_INFO_ATV_CH_ALL_NUM, &atv_fsm_ptr->search_total_num);
    
    //get search param
    GetPreviewParam(&preview_param);
    search_ret = DRECORDER_ATV_StartScanChn(&preview_param);
#ifdef WIN32
    MMIATV_WIN32SearchMsg(&preview_param);
#endif
    if(DRECORDER_SUCCESS == search_ret)
    {
        return TRUE;  
    }
    return FALSE;    
}

/*****************************************************************************/
//  Description : 停止视频搜索
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_StopSearch(FSMSRV_HANDLE fsm_handle)
{
    uint32 search_ret = 0;    
    MMIATV_SERVICE_NODE_T* service_ptr = PNULL;
    ATV_FSM_T   *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    if (PNULL == atv_fsm_ptr)
    {
        return FALSE;
    }

    //stop search
    atv_fsm_ptr->service_id = 0;
    search_ret = DRECORDER_ATV_StopScanChn();   
    if(MMIATV_GetServiceListNum() > 0)
    {
        service_ptr = MMIATV_GetServiceNodeViaIndex(0);
        if(PNULL != service_ptr)
        {
            atv_fsm_ptr->service_id = service_ptr->service_id;
        }      
    }  
    
    SCI_TRACE_LOW("[ATV] ATV_StopSearch atv_fsm_ptr->service_id = %d",atv_fsm_ptr->service_id);
    if (DRECORDER_SUCCESS == search_ret)
    {
        return TRUE;
    }
    return FALSE;
    
}

/*****************************************************************************/
//  Description : 发起视频播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Play(FSMSRV_HANDLE fsm_handle)
{
    
    DRECORDER_PREVIEW_PARAM_T record_param = {0};
    MMISRVAUD_TYPE_U audio_info = {0};
    ATV_FSM_T   *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);

	SCI_TRACE_LOW("[ATV] ATV_Play entry atv_fsm_ptr = %x",atv_fsm_ptr);
    
    if (PNULL == atv_fsm_ptr)
    {
        return FALSE;
    }     
    SCI_TRACE_LOW("[ATV] ATV_Play atv_fsm_ptr->service_id = %d",atv_fsm_ptr->service_id);
    //play service
    GetPreviewParam(&record_param);
    //set info
    audio_info.type = MMISRVAUD_TYPE_ATV;
    audio_info.atv.param_ptr = &record_param;
    audio_info.atv.param_size = sizeof(DRECORDER_PREVIEW_PARAM_T);
    audio_info.atv.region_id = (uint32)MMIATV_GetRegion();
    audio_info.atv.service_id = atv_fsm_ptr->service_id;
    MMISRVAUD_SetAudioInfo(atv_fsm_ptr->srv_handle,&audio_info);
    if(MMISRVAUD_Play(atv_fsm_ptr->srv_handle,0))
    {
        return TRUE;
    }
    else
    {
        SCI_TRACE_LOW("[ATV] ATV_Play MMISRVAUD_Play failed");
        return FALSE;
    }    
    
}

/*****************************************************************************/
//  Description : open decorder
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  ATV_Open(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    BOOLEAN return_val = FALSE;
    
    
    ret = DRECORDER_SelectSrcDevice(DRECORDER_SRC_DEV_ATV); 
    ret = DRECORDER_Open();
    
    
    if(DRECORDER_SUCCESS == ret)
    {
        
        return_val = TRUE;
    }    
    
    return return_val;   
}

/*****************************************************************************/
//  Description : open ATV_StopRecord
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_StopRecord(FSMSRV_HANDLE fsm_handle)
{
    BOOLEAN ret = FALSE;
    ATV_FSM_T   *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    if (PNULL != atv_fsm_ptr)
    {
        ret = MMISRVAUD_StopRecordAtv(atv_fsm_ptr->srv_handle);
        MMISRVAUD_Stop(atv_fsm_ptr->srv_handle);   
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : ATV_Record
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATV_Record(FSMSRV_HANDLE fsm_handle)
{
    BOOLEAN     return_val = TRUE;
    
    DRECORDER_START_RECORD_T recorder_start_t = {0};
    MEDIA_AUDIO_INFO_T  audio_info = {0};
    MEDIA_VIDEO_INFO_T  video_info = {0};
    ATV_FSM_T   *atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(fsm_handle);
    
    if(PNULL != atv_fsm_ptr && MMIATV_CreateRecordFileName(&recorder_start_t))
    {
        /* set audio info */
        audio_info.audio_support = 1;   
        audio_info.channels = AUDIO_CHANNEL;
        audio_info.sample_rate = AUDIO_SAMPLE_RATE;
        audio_info.audio_type = AUDIO_TYPE_AMR;
        
        /* set video info */
        if(MMIATV_VIDEO_TYPE_3GP == MMIATV_GetVideoFileType())
        {
            audio_info.audio_type = AUDIO_TYPE_IMAADPCM;
#ifdef TYPE_3GP_SUPPORT_MP4
            video_info.video_type = VIDEO_TYPE_MP4V;
#else
            video_info.video_type = VIDEO_TYPE_MJPEG;
#endif
            recorder_start_t.file_type = DRECORDER_FILETYPE_3GP;
        }
        else if(MMIATV_VIDEO_TYPE_AVI == MMIATV_GetVideoFileType())
        {
            audio_info.audio_type = AUDIO_TYPE_IMAADPCM;
            video_info.video_type = VIDEO_TYPE_MJPEG;
            recorder_start_t.file_type = DRECORDER_FILETYPE_AVI;
        }
        else
        {
            audio_info.audio_type = AUDIO_TYPE_IMAADPCM;
            video_info.video_type = VIDEO_TYPE_MJPEG;
            recorder_start_t.file_type = DRECORDER_FILETYPE_AVI;
        }
        video_info.frame_rate   = (uint32)MMIAPIENG_GetDVFrameFre();
        video_info.height = VIDEO_320X240_HEIGHT;
        video_info.width = VIDEO_320X240_WIDTH;
        video_info.video_support = TRUE;
        
        recorder_start_t.audio_info_ptr     = &audio_info;
        recorder_start_t.video_info_ptr     = &video_info;
        recorder_start_t.frame_callback     = PNULL;
        recorder_start_t.file_limit_size = 0;
        recorder_start_t.end_callback       = (DRECORDER_END_CALLBACK)RecordEndCallBack;
        if(MMISRVAUD_StartRecordAtv(atv_fsm_ptr->srv_handle,&recorder_start_t))
        {
            return_val = TRUE;
        }     
        else
        {
            MMISRVAUD_Stop(atv_fsm_ptr->srv_handle);
            return_val = FALSE;
            
        }
    }
    else
    {
        NotifyUIMsg(MSG_ATV_NO_SPACE,PNULL,PNULL);
        return_val = FALSE;
        
    }
    return return_val;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyUIMsg(MMI_MESSAGE_ID_E msg_id, DPARAM param_ptr,  uint32 size_of_param)
{
    SCI_TRACE_LOW("[ATV] NotifyUIMsg msg_id = %d", msg_id);
    MMK_PostMsg(MMIATV_MAIN_WIN_ID,msg_id,param_ptr,size_of_param);
}

/*****************************************************************************/
//  Description : set channel id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetATVChannelID(uint32 service_id,
                              DCAMERA_ATV_CHN_OP_MODE_E op_mode)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    uint32 chn_id = 0;
    
    switch(op_mode) 
    {
    case DCAMERA_ATV_CHN_NORMAL_OP:
    case DCAMERA_ATV_CHN_NOLY_SET:
    case DCAMERA_ATV_CHN_NOLY_DECT:
        chn_id=((op_mode<<0x10)&0xffff0000)|(service_id&0xffff);  //ref需要根据高位来做对应的处理
        break;
    default:
        chn_id = service_id;
        break;
    }
    ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_ATV_CHN, chn_id, CHN_ParamCtrlCallBack);
    SCI_TRACE_LOW("[ATV] SetATVChannelID chn_id = %d, ret = %d", chn_id,ret);
    if (DRECORDER_SUCCESS == ret)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : handle app msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIATV_HandleSysMsg(
                                        PWND     app_ptr,    //pointer of application
                                        uint16   msg_id,     //the identify of message
                                        DPARAM   param       //the param of the message
                                        )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    DRECORDER_REPORT_DATA_U  *sig_data_ptr = PNULL;
    uint32  signal_header_length = sizeof(xSignalHeaderRec);
    
    SCI_TRACE_LOW("[ATV] MMIATV_HandleSysMsg msg_id = %x", msg_id);
    switch(msg_id)
    {
    case MMIATV_CALLBACK_MSG_REPORT:
        sig_data_ptr = (DRECORDER_REPORT_DATA_U  *)((uint32)param + signal_header_length);
        HandleReportMsg(sig_data_ptr);
        break;
        
    case MMIATV_RECORD_END:
        {
        void *param_buf = (void *)((uint32)param + signal_header_length);

        result = FSMSRV_Dispatch(s_fsm_handle,ATV_RECORD_END, param_buf,sizeof(int32));
        }
        break;
        
    case MMIATV_CHN_CNF:
        result = FSMSRV_Dispatch(s_fsm_handle,ATV_CHN_CNF, PNULL,PNULL);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : get search total num
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetSearchTotalNum(void)
{
    ATV_FSM_T*	atv_fsm_ptr = PNULL;
    
    atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(s_fsm_handle);
    if(PNULL != atv_fsm_ptr)
    {
        return atv_fsm_ptr->search_total_num;
    }
    
    return 0;
}

/*****************************************************************************/
//  Description : get search current num
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetSearchCurrentNum(void)
{
    ATV_FSM_T*	atv_fsm_ptr = PNULL;
    
    atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(s_fsm_handle);
    if(PNULL != atv_fsm_ptr)
    {
        return atv_fsm_ptr->search_cur_num;
    }
    
    return 0;
}

/*****************************************************************************/
//  Description : get atv signal level 
//  Global resource dependence : none
//  Author: xin.li
//  Note: level 0-5
/*****************************************************************************/
PUBLIC uint8 MMIATV_GetTVSignalLevel(void)
{
    ATV_FSM_T*	atv_fsm_ptr = PNULL;
    
    atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(s_fsm_handle);
    if(PNULL != atv_fsm_ptr)
    {
        return atv_fsm_ptr->signal_level;
    }
    
    return 0;
}


/*****************************************************************************/
//  Description : 处理callback消息
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleReportMsg(DRECORDER_REPORT_DATA_U *param_ptr)
{
    DRECORDER_REPORT_TYPE_E report_type = DRECORDER_REPORT_ATV_RSSI;
    ATV_CHN_INFO_T          chn_info = {0};
    ATV_FSM_T*	            atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(s_fsm_handle);
    
    if(PNULL == param_ptr
        || PNULL == atv_fsm_ptr)
    {
        return 0;
    }    
    
    report_type = param_ptr->atv_report.report_typ;
    SCI_TRACE_LOW("[ATV] HandleReportMsg report_type = %d",report_type);
    switch(report_type) 
    {
    case DRECORDER_REPORT_ATV_SCAN_CHN:         
        atv_fsm_ptr->search_cur_num ++;  
        chn_info.chn_is_valid = param_ptr->atv_report.chn_is_valid;
        chn_info.chn_id = param_ptr->atv_report.chn_id;      
        chn_info.search_valid_num = atv_fsm_ptr->search_valid_num;
        if (chn_info.chn_is_valid)
        {
            atv_fsm_ptr->search_valid_num ++;
        }
        chn_info.search_cur_num = atv_fsm_ptr->search_cur_num;
        chn_info.search_total_num = atv_fsm_ptr->search_total_num;
        SCI_TRACE_LOW("[ATV] HandleReportMsg chn_id = %d, search_valid_num = %d, chn_info.search_cur_num = %d",chn_info.chn_id,atv_fsm_ptr->search_valid_num,chn_info.search_cur_num);
        NotifyUIMsg(MSG_APP_SCAN_PROCESS_IND, &chn_info, sizeof(ATV_CHN_INFO_T));    
        
        break;
        
    case DRECORDER_REPORT_ATV_SCAN_CHN_STOP:
    case DRECORDER_REPORT_ATV_SCAN_CHN_END:
        //search end
        FSMSRV_Dispatch(s_fsm_handle,ATV_SEARCH_CNF,PNULL,PNULL);        
        break; 
        
    case DRECORDER_REPORT_ATV_RSSI: 
        if(0 == param_ptr->atv_report.rssi)
        {
            NotifyUIMsg(MSG_APP_ATV_WEAK_SINGAL,PNULL,PNULL);            
        }
        else if(0 == atv_fsm_ptr->signal_level && 0 < param_ptr->atv_report.rssi)
        {
            NotifyUIMsg(MSG_APP_ATV_NORMAL_SINGAL,PNULL,PNULL); 
            
        }        
        //rssi changed, REF level 0-4
        
        atv_fsm_ptr->signal_level = (uint8)param_ptr->atv_report.rssi;
        if(atv_fsm_ptr->signal_level > MMIATV_SIGNAL_LEVEL_MAX)
        {
            atv_fsm_ptr->signal_level = MMIATV_SIGNAL_LEVEL_MAX;
        }
        
        break;
        
    case DRECORDER_REPORT_ATV_ERR:
        NotifyUIMsg(MSG_APP_ATV_FATAL_ERROR,PNULL,PNULL);        
        break;
        
    default:
        break;
    }
    
    return 0;
}

/*****************************************************************************/
//  Description : ParamCtrlCallBack
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHN_ParamCtrlCallBack(void *param_ptr, uint32 param_size)
{    
    SCI_TRACE_LOW("[ATV] CHN_ParamCtrlCallBack");
    SendSignalToTask(MMIATV_CHN_CNF,PNULL,P_APP);   
    return 0;
}

/*****************************************************************************/
//  Description : ParamCtrlCallBack
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL uint32 ParamCtrlCallBack(void *param_ptr, uint32 param_size)
{
    return 0;
}


/*****************************************************************************/
//  Description : get preview param
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void GetPreviewParam(DRECORDER_PREVIEW_PARAM_T *preview_param_ptr)
{
    GUILCD_COORDINATE_PARAM_T logic_coor = {0};
    GUILCD_COORDINATE_PARAM_T phy_coor = {0};
    uint32 x = 0;
    uint32 y = 0;
    uint32 dx = 0;
    uint32 dy = 0;
    
    if(PNULL == preview_param_ptr)
    {
        return;
    }
    
    //rotation
    logic_coor.angle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
    //SCI_TRACE_LOW:"[MMIATV]: GetPreviewParam cur_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_616_112_2_18_2_3_11_56,(uint8*)"d", logic_coor.angle);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&logic_coor.lcd_width,&logic_coor.lcd_height);
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        //horizontal
        logic_coor.rect.left = 0;
        logic_coor.rect.top = 0;
        logic_coor.rect.right = MMIATV_FULLSCREEN_VIDEO_WIDTH - 1;
        logic_coor.rect.bottom = MMIATV_FULLSCREEN_VIDEO_HEIGHT - 1;
    }
    else
    {
        //vertical
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 
        if(MMITHEME_IsIstyle())
        {
            logic_coor.rect.top = MMIATV_VIDEO_TOP + (MMIATV_ISTYLE_TITLE_HEIGHT/2);
        }
        else
#endif
        {
            logic_coor.rect.top = MMIATV_VIDEO_TOP;
        }
        
        logic_coor.rect.left = MMIATV_VIDEO_LEFT;
        logic_coor.rect.right = MMIATV_VIDEO_LEFT + MMIATV_VIDEO_WIDTH - 1;
        logic_coor.rect.bottom = logic_coor.rect.top + MMIATV_VIDEO_HEIGHT - 1;
        
    }
    
    
    //SCI_TRACE_LOW:"[MMIATV]: GetPreviewParam , logic_coor.rect {%d %d %d %d} rotation %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_666_112_2_18_2_3_11_58,(uint8*)"ddddd",logic_coor.rect.left,logic_coor.rect.top,logic_coor.rect.right,logic_coor.rect.bottom,logic_coor.angle);
    
    GUILCD_ConvertLogicToPhysicalCoordinate(0, &logic_coor, &phy_coor);
    x = phy_coor.rect.left;   
    y = phy_coor.rect.top;   
    dx = phy_coor.rect.right - phy_coor.rect.left + 1;    
    dy = phy_coor.rect.bottom - phy_coor.rect.top + 1;
/*    MMIATV_GetAlign8Offset(&dx,&dy,&x,&y);*/
	MMIATV_GetAlign4Offset(&dx, &dy, &x, &y);
    
    preview_param_ptr->target_rect.x = x;
    preview_param_ptr->target_rect.y = y;
    preview_param_ptr->target_rect.dx = dx;
    preview_param_ptr->target_rect.dy = dy;
    
    preview_param_ptr->LcdId = GUI_MAIN_LCD_ID;
    preview_param_ptr->LcdBlkId = GUI_BLOCK_0;
    preview_param_ptr->display_mode = DRECORDER_DISP_FULLVIEW;
    preview_param_ptr->DisplayRotateAngle = phy_coor.angle;
    
    logic_coor.rect = MMITHEME_GetFullScreenRect();
    
    GUILCD_ConvertLogicToPhysicalCoordinate(0, &logic_coor, &phy_coor);
    x = phy_coor.rect.left;   
    y = phy_coor.rect.top;   
    dx = phy_coor.rect.right - phy_coor.rect.left + 1;    
    dy = phy_coor.rect.bottom - phy_coor.rect.top + 1;
/*    MMIATV_GetAlign8Offset(&dx,&dy,&x,&y);*/
    MMIATV_GetAlign4Offset(&dx, &dy, &x, &y);
    preview_param_ptr->disp_rect.x = x;
    preview_param_ptr->disp_rect.y = y;
    preview_param_ptr->disp_rect.dx = dx;
    preview_param_ptr->disp_rect.dy  = dy;
    
    
    preview_param_ptr->image_format = MMIAPIENG_GetSensorMode();
    preview_param_ptr->report_callback = ReportCallback;
    
    //SCI_TRACE_LOW:"[MMIATV]: GetPreviewParam , disp_rect {%d %d %d %d},target rect {%d %d %d %d},rotation %d,disp_mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_697_112_2_18_2_3_11_59,(uint8*)"dddddddddd",preview_param_ptr->disp_rect.x,preview_param_ptr->disp_rect.y,preview_param_ptr->disp_rect.dx,preview_param_ptr->disp_rect.dy,preview_param_ptr->target_rect.x,preview_param_ptr->target_rect.y,preview_param_ptr->target_rect.dx,preview_param_ptr->target_rect.dy,preview_param_ptr->DisplayRotateAngle,preview_param_ptr->display_mode);
    
}

/*****************************************************************************/
//  Description : report Callback
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 ReportCallback(DRECORDER_REPORT_DATA_U *report_ptr)
{
    DRECORDER_REPORT_DATA_U   sig_data = {0};
    
    if(PNULL != report_ptr)
    {
        SCI_MEMCPY(&sig_data, report_ptr, sizeof(DRECORDER_REPORT_DATA_U)); /*lint !e668*/
    }    
    
    SCI_TRACE_LOW("[ATV] ReportCallback sig_data.atv_report.report_typ = %d",sig_data.atv_report.report_typ);
    switch(sig_data.atv_report.report_typ)
    {
    case DRECORDER_REPORT_ATV_SCAN_CHN:
    case DRECORDER_REPORT_ATV_SCAN_CHN_END:
    case DRECORDER_REPORT_ATV_SCAN_CHN_STOP:
    case DRECORDER_REPORT_ATV_RSSI:
    case DRECORDER_REPORT_ATV_ERR:
        SendSignalToTask(MMIATV_CALLBACK_MSG_REPORT, (void*)&sig_data, P_APP);
        break;
        
    default:
        break;
    }
    
    return 0;
}

/*****************************************************************************/
//  Description : send signal to task
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SendSignalToTask(uint16 sig_code,void *signal_data_ptr, uint32 task_id)
{
    ATV_CALLBACK_SIG_T *sig_to_mmi_ptr = PNULL;
    uint32 result = 0;
    
    if(task_id == P_APP )
    {
        sig_to_mmi_ptr = (ATV_CALLBACK_SIG_T*)SCI_ALLOC(sizeof(ATV_CALLBACK_SIG_T));
        if(PNULL == sig_to_mmi_ptr)
        {
            return;
        }
        sig_to_mmi_ptr->SignalCode = sig_code;
        sig_to_mmi_ptr->SignalSize = sizeof(ATV_CALLBACK_SIG_T);
        sig_to_mmi_ptr->Sender = SCI_IdentifyThread();
        if (PNULL != signal_data_ptr)
        {
            sig_to_mmi_ptr->data = *(DRECORDER_REPORT_DATA_U*)signal_data_ptr;
        }           
        result = SCI_SendSignal((xSignalHeader)sig_to_mmi_ptr, task_id);
    }
}


/*****************************************************************************/
//  Description : 录制的callback函数
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void RecordEndCallBack(int32 i_type)
{
    SendSignalToTask(MMIATV_RECORD_END,&i_type,P_APP);    
}


/*****************************************************************************/
//  Description : MMIATV_IsRecording
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsRecording(void)
{
    BOOLEAN result = FALSE;
    FSM_STATE_FUNC   state_func = FSMSRV_GetState(s_fsm_handle);

	if(FsmRecording == state_func)
	{
		result = TRUE;
	}

    SCI_TRACE_LOW("[ATV] MMIATV_IsRecording result = %d",result);
    return result;
}

/*****************************************************************************/
//  Description : MMIATV_IsPlaying
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsPlaying(void)
{
    BOOLEAN result = FALSE;
    FSM_STATE_FUNC   state_func = FSMSRV_GetState(s_fsm_handle);

	if(FsmPlaying == state_func)
	{
		result = TRUE;
	}

    SCI_TRACE_LOW("[ATV] MMIATV_IsPlaying result = %d",result);
    return result;
}

/*****************************************************************************/
//  Description : MMIATV_IsSearching
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsSearching(void)
{
    BOOLEAN result = FALSE;
    FSM_STATE_FUNC   state_func = FSMSRV_GetState(s_fsm_handle);

	if(FsmSearching == state_func)
	{
		result = TRUE;
	}

    SCI_TRACE_LOW("[ATV] MMIATV_IsSearching result = %d",result);
    return result;
}

/*****************************************************************************/
//  Description : MMIATV_IsSettingCHN
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsSettingCHN(void)
{
    BOOLEAN result = FALSE;
    FSM_STATE_FUNC   state_func = FSMSRV_GetState(s_fsm_handle);

	if(FsmSettingCHN == state_func)
	{
		result = TRUE;
	}

    SCI_TRACE_LOW("[ATV] MMIATV_IsSettingCHN result = %d",result);
    return result;

}


/*****************************************************************************/
//  Description : get current play service id
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetCurrentServiceID(void)
{
    ATV_FSM_T*	atv_fsm_ptr = PNULL;
    
    atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(s_fsm_handle);
    if(PNULL != atv_fsm_ptr)
    {
        return atv_fsm_ptr->service_id;
    }
    
    return 0;
}

/*****************************************************************************/
//  Description : set current play service id
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetCurrentServiceID(uint32 service_id)
{
    ATV_FSM_T*	atv_fsm_ptr = PNULL;
    
    atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(s_fsm_handle);
    if(PNULL != atv_fsm_ptr)
    {
        atv_fsm_ptr->service_id = service_id;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_GetAlign8Offset(
                                        uint32 *width_ptr, //[IN/OUT]
                                        uint32 *height_ptr, //[IN/OUT]
                                        uint32 *x_offset_ptr, //[IN/OUT]
                                        uint32 *y_offset_ptr //[IN/OUT]                                                         
                                        )
{
    if (PNULL == width_ptr
        || PNULL == height_ptr
        || PNULL == x_offset_ptr
        || PNULL == y_offset_ptr)
    {
        return FALSE;
    }
    *width_ptr = *width_ptr - MMIAPICOM_GetAlign8Offset(*x_offset_ptr);        
    *width_ptr = *width_ptr - (*width_ptr % 8);
    *x_offset_ptr = *x_offset_ptr + MMIAPICOM_GetAlign8Offset(*x_offset_ptr);
    *height_ptr = *height_ptr - MMIAPICOM_GetAlign8Offset(*y_offset_ptr);
    *height_ptr = *height_ptr - (*height_ptr % 8);    
    *y_offset_ptr = *y_offset_ptr + MMIAPICOM_GetAlign8Offset(*y_offset_ptr);
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: yanxianzhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_GetAlign4Offset(
									  uint32 *width_ptr, //[IN/OUT]
									  uint32 *height_ptr, //[IN/OUT]
									  uint32 *x_offset_ptr, //[IN/OUT]
									  uint32 *y_offset_ptr //[IN/OUT]                                                         
									  )
{
    if (PNULL == width_ptr
        || PNULL == height_ptr
        || PNULL == x_offset_ptr
        || PNULL == y_offset_ptr)
    {
        return FALSE;
    }
    *width_ptr = *width_ptr - MMIAPICOM_GetAlign4Offset(*x_offset_ptr);        
    *width_ptr = *width_ptr - (*width_ptr % 4);
    *x_offset_ptr = *x_offset_ptr + MMIAPICOM_GetAlign4Offset(*x_offset_ptr);
    *height_ptr = *height_ptr - MMIAPICOM_GetAlign4Offset(*y_offset_ptr);
    *height_ptr = *height_ptr - (*height_ptr % 4);    
    *y_offset_ptr = *y_offset_ptr + MMIAPICOM_GetAlign4Offset(*y_offset_ptr);
    return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T  MMIATV_GetAudioHandle(void)
{
    ATV_FSM_T*	atv_fsm_ptr = PNULL;
    
    atv_fsm_ptr = (ATV_FSM_T *)FSMSRV_GetObject(s_fsm_handle);
    if(PNULL != atv_fsm_ptr)
    {
        return atv_fsm_ptr->srv_handle;
    }
    
    return 0;
}
