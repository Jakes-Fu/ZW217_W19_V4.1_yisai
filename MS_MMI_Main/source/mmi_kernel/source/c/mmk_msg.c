/****************************************************************************
** File Name:      mmk_msg.c                                               *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
** 
****************************************************************************/
#define MMK_MSG_C

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/

#include "mmi_kernel_trc.h"
#include "mmk_msg.h"
#include "mmk_msg_internal.h"
#include "mmi_osbridge.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "tb_dal.h"
#include "mmi_port.h"
#include "mmk_kbd_internal.h"
#include "mmi_signal.h"
#include "mmi_theme.h"
#include "mmk_window_internal.h"
#include "mmk_applet_internal.h"
#include "mmk_handle_internal.h"
#include "mmk_gsensor.h"
#include "mmi_default.h"
#include "ctrlsys_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
//============== MET MEX  start ===================
#ifdef MET_MEX_SUPPORT
#include "met_mex_osbridge.h"
#include "met_mex_export.h"
#endif // MET_MEX_SUPPORT
//============== MET MEX  end ===================
#ifdef UAL_SUPPORT
#include "ual_cms.h"
#endif
#ifdef UAL_GNSS_SUPPORT
#include "ual_gnss.h"
#include "gps_env_signal.h"
#endif


/**-----------------------------------------------------------------------------*
 **                         MACRO Declaration                                   *
 **----------------------------------------------------------------------------*/
#define MMK_MSG_QUEUE_MAX_LEN   32  // the size of message queue

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
uint32 g_reg_app_idx = 0;
LOCAL MMK_MSG_QUEUE_T   s_mmk_msg_queue;    //the message queue
#ifdef WRE_SUPPORT
LOCAL MMI_MESSAGE_T     s_mwin_msg;
#endif

/**-------------------------------------------------------------------------*
 **                         Global Variables                                *
 **------------------------------------------------------------------------*/
extern const REGISTER_APP_SIG_T g_reg_app[];

/*--------------------------------------------------------------------------*/
/*                    LOCAL FUNCTION DECLARE                                */
/*--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : dispatch the ps signal 
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL void DispatchSysSig(
                          MmiSignalS* signal_ptr // the pointer of message which need dispatch
                          );

/*****************************************************************************/
// 	Description : add the asynchronous  message to queue
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AddMsgToQue(
                          MMI_HANDLE_T        handle,
                          MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                          DPARAM              param_ptr,      //the param of the message
                          uint32              size_of_param,  //the size of the param
                          MMI_MESSAGE_TYPE_E  msg_type        //the type of message
                          );

/*****************************************************************************/
// 	Description : whether the message queue is empty, TRUE: empty, FALSE: not empty
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MSGQueueIsEmpty(
                              void
                              );

/*****************************************************************************/
// 	Description : check the message queue whether it has message and return
//                the pointer of this message
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMSGQueue(
                          MMI_MESSAGE_PTR_T  *mmi_msg_ptr //return the pointer of message
                          );

/*****************************************************************************/
// 	Description : whether the message is valid, TRUE: valid, FALSE: invalid
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MSGIsValid(
                         MMI_MESSAGE_T* msg_ptr //[IN] the message pointer
                         );

/*****************************************************************************/
// 	Description : handle message function
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  HandleMsgFunc(
                            MMI_HANDLE_T        handle,
                            MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                            DPARAM              param_ptr   //the param of the message
                            );

/*****************************************************************************/
// 	Description : dispatch other mmi task signal 
//	Global resource dependence :
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void DispatchMmiSig(
                          MmiSignalS* signal_ptr // the pointer of message which need dispatch
                          );

/*****************************************************************************/
// 	Description : 是否动画作为背景
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAnimBg(
                       MMI_HANDLE_T     win_handle,
                       MMI_MESSAGE_ID_E msg_id
                       );

/*****************************************************************************/
// 	Description : get WRE message for queue
//	Global resource dependence :
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
#ifdef WRE_SUPPORT
LOCAL BOOLEAN GetWreMSGQueue(
                          MMI_MESSAGE_PTR_T  *mmi_msg_ptr //return the pointer of message
                          );
#endif

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : whether the message queue is empty, TRUE: empty, FALSE: not empty
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MSGQueueIsEmpty(void)
{
    BOOLEAN     result = FALSE;

    if (((s_mmk_msg_queue.readpos == s_mmk_msg_queue.writepos) && 
        (s_mmk_msg_queue.is_queue_full)) || 
        (s_mmk_msg_queue.readpos != s_mmk_msg_queue.writepos))
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : whether the message is valid, TRUE: valid, FALSE: invalid
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MSGIsValid(
                         MMI_MESSAGE_T* msg_ptr //[IN] the message pointer
                         )
{
    if ( PNULL == msg_ptr )
    {
        return FALSE;
    }

    //SCI_PASSERT(PNULL != msg_ptr, ("MSGIsValid: the msg_ptr is PNULL!"));

    if ( 0 == msg_ptr->handle
        && MSG_TYPE_NONE == msg_ptr->msg_type
        && 0 == msg_ptr->msg_id
        && PNULL == msg_ptr->param_ptr )
    {
        return (FALSE);
    }
    else
    {
        return (TRUE);
    }
}

/*****************************************************************************/
// 	Description : init the message queue
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_InitMSGQueue(
                        uint8               msgQueue_len        //the length of message queue
                        )
{
    //check whether the input param is valid
    
    if (msgQueue_len == 0)
    {
        msgQueue_len = MMK_MSG_QUEUE_MAX_LEN;
    } 

	//alloc the space for message 
    s_mmk_msg_queue.msg = SCI_ALLOC_APP( sizeof (MMI_MESSAGE_T) * msgQueue_len );/*lint !e26 !e64*/

    SCI_ASSERT( PNULL != s_mmk_msg_queue.msg );/*assert verified*/
    SCI_MEMSET((void *)(s_mmk_msg_queue.msg), 0 , (sizeof (MMI_MESSAGE_T) * msgQueue_len));


    s_mmk_msg_queue.len       = msgQueue_len;

    //init the read and write postion
    s_mmk_msg_queue.readpos   = 0;
    s_mmk_msg_queue.writepos  = 0;
    s_mmk_msg_queue.is_queue_full = FALSE;

}

/*****************************************************************************/
// 	Description : check the message queue whether it has message and return
//                the pointer of this message
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMSGQueue(
                          MMI_MESSAGE_PTR_T  *mmi_msg_ptr //return the pointer of message
                          )
{
    //check whether the input param is valid
    SCI_ASSERT( PNULL != mmi_msg_ptr );/*assert verified*/
    
    *mmi_msg_ptr = &(s_mmk_msg_queue.msg[ s_mmk_msg_queue.readpos ]);
    
    //move the identify of read position to next position
    s_mmk_msg_queue.readpos++;
    if( s_mmk_msg_queue.readpos >= s_mmk_msg_queue.len )
    {
        s_mmk_msg_queue.readpos = 0;
    }
    
    s_mmk_msg_queue.is_queue_full = FALSE;
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : get WRE message for queue
//	Global resource dependence :
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
#ifdef WRE_SUPPORT
LOCAL BOOLEAN GetWreMSGQueue(
                          MMI_MESSAGE_PTR_T  *mmi_msg_ptr //return the pointer of message
                          )
{
    memset(&s_mwin_msg, 0, sizeof(s_mwin_msg));
    *mmi_msg_ptr = &s_mwin_msg;
    return TRUE;
}
#endif

/*****************************************************************************/
// 	Description : check the message queue whether it has message and return
//                the pointer of this message
//	Global resource dependence :
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
BOOLEAN MMK_GetMSGQueue(
                        MMI_MESSAGE_PTR_T  *mmi_msg_ptr //[OUT] return the pointer of message
                        )
{

    MMI_MESSAGE_T* temp_msg_ptr = PNULL;

    //check whether the input param is valid
    //SCI_ASSERT( PNULL != mmi_msg_ptr );
    if ( PNULL == mmi_msg_ptr )
    {
        return FALSE;
    }

    while(1)/*lint !e716*/
    {
        if (!MSGQueueIsEmpty())
        {
            GetMSGQueue(mmi_msg_ptr);
            
            temp_msg_ptr = *mmi_msg_ptr;
            
            if (MSGIsValid(temp_msg_ptr))
            {
                return (TRUE);
            }
        }
#ifdef WRE_SUPPORT
        else if(MMIWRE_GetWreMessageForMocor())
        {
            GetWreMSGQueue(mmi_msg_ptr);
            temp_msg_ptr = *mmi_msg_ptr;
            temp_msg_ptr->msg_type  = MSG_TYPE_WRE;
            
            return TRUE;
        }
#endif
        else
        {
            return (FALSE);
        }
    }
}


/*****************************************************************************/
// 	Description : dispatch the message which get from message queue
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_DispatchMSGQueue(
                          MMI_MESSAGE_PTR_T mmi_msg_ptr // the pointer of message which need dispatch
                          )
{

    //check whether the input param is valid
    //SCI_ASSERT( PNULL != mmi_msg_ptr );
    if ( PNULL == mmi_msg_ptr )
    {
        return;
    }

    switch( mmi_msg_ptr->msg_type )
    {
    case MSG_TYPE_WINDOW:
        
        MMK_DispatchWinMSG( mmi_msg_ptr );        
        break;

//     case MSG_TYPE_FOCUS:
//         MMK_DispMsgToWin( mmi_msg_ptr->msg_id, mmi_msg_ptr->param_ptr );
//         break;

    case MSG_TYPE_BROADCAST:  
        MMK_DispatchBCMSG( mmi_msg_ptr );
        break;
#ifdef WRE_SUPPORT
    case MSG_TYPE_WRE:
        MMIWRE_DispatchWreMSG();
        break;
#endif
    default:
        SCI_TRACE_ERROR("MMK_MSG.C: the messaeg type is %d",mmi_msg_ptr->msg_type);
        break;
    }
}

/*****************************************************************************/
// 	Description : release the space of the message param
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_FreeMSG(
                 MMI_MESSAGE_PTR_T  mmi_msg_ptr // the pointer of message which need release
                 )
{
    //check whether the input param is valid
    //SCI_ASSERT( PNULL != mmi_msg_ptr );
    if ( PNULL == mmi_msg_ptr )
    {
        return;
    }

    if( NULL != mmi_msg_ptr->param_ptr  )
    {
        //Free the space of param
        SCI_FREE( mmi_msg_ptr->param_ptr);
        mmi_msg_ptr->param_ptr = NULL;
    }

    //reset the data of message queue
    mmi_msg_ptr->handle = 0;
    mmi_msg_ptr->msg_id = 0;
    mmi_msg_ptr->msg_type = MSG_TYPE_NONE;
}

/*****************************************************************************/
// 	Description : send the synchronous message to the pointed control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SendMsg(
                           MMI_HANDLE_T     handle,
                           MMI_MESSAGE_ID_E msg_id,
                           DPARAM           param_ptr
                           )
{
    BOOLEAN     result = FALSE;

    //convert handle
    handle = MMK_ConvertIdToHandle(handle);
    if (0 != handle)
    {
        result = MMK_DispatchToHandle(handle,msg_id,param_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_MSG.C mmk send msg handle is 0!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MSG_413_112_2_18_3_22_41_8,(uint8*)"");
    }

    return (result);
}

/*****************************************************************************/
// 	Description : send the asynchronous message to the pointed control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_PostMsg(
                           MMI_HANDLE_T     handle,         // the handle
                           MMI_MESSAGE_ID_E msg_id,         //the identify of message
                           DPARAM           param_ptr,      //the param of the message
                           uint32           size_of_param   //the size of the param
                           )
{
    BOOLEAN     result = FALSE;

    //convert handle
    handle = MMK_ConvertIdToHandle(handle);

    //add msg to queue
    if (MMI_HANDLE_NONE != MMK_GetHandleType(handle))
    {
        result = AddMsgToQue(handle,msg_id,param_ptr,size_of_param,MSG_TYPE_WINDOW);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : add the asynchronous  message to queue
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AddMsgToQue(
                          MMI_HANDLE_T        handle,
                          MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                          DPARAM              param_ptr,      //the param of the message
                          uint32              size_of_param,  //the size of the param
                          MMI_MESSAGE_TYPE_E  msg_type        //the type of message
                          )
{
    DPARAM              msgParam_ptr = PNULL;
    MMI_MESSAGE_PTR_T   msg_ptr      = PNULL;

    //check whether the message queue is full
    if( s_mmk_msg_queue.is_queue_full )
    {
        //SCI_TRACE_LOW:"MMK_MSG.C: AddMsgToQue the message queue is full. len = %d, readpos = %d, writepos = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MSG_462_112_2_18_3_22_41_9,(uint8*)"ddd", s_mmk_msg_queue.len, s_mmk_msg_queue.readpos, s_mmk_msg_queue.writepos);

        return FALSE;
    }

    //alloc the space for the message param
    if( PNULL != param_ptr )
    {
        msgParam_ptr = SCI_ALLOC_APP(size_of_param);/*lint !e26 !e64*/

        if( PNULL == msgParam_ptr )
        {
            SCI_TRACE_ERROR("MMK_MSG.C: SCI_ALLOC_APP is error");
            return FALSE;
        }

         MMI_MEMCPY( 
            msgParam_ptr, 
            size_of_param, 
            param_ptr,
            size_of_param,
            size_of_param);
    }
    
    // Write the message data and advance write pointer */
    msg_ptr             = &(s_mmk_msg_queue.msg[ s_mmk_msg_queue.writepos ]);
    msg_ptr->handle     = handle;
    msg_ptr->msg_id     = msg_id;
    msg_ptr->param_ptr  = msgParam_ptr;
    msg_ptr->msg_type   = msg_type;

    s_mmk_msg_queue.writepos++;
    if (s_mmk_msg_queue.writepos >= s_mmk_msg_queue.len) 
    {
        s_mmk_msg_queue.writepos = 0;
    }

    //queue full
    if(  s_mmk_msg_queue.writepos  % s_mmk_msg_queue.len == s_mmk_msg_queue.readpos )
    {
        s_mmk_msg_queue.is_queue_full = TRUE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : post the broadcast  message to the window
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
BOOLEAN MMK_PostBCMessage(
                            MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                            DPARAM              param_ptr,      //the param of the message
                            uint32              size_of_param   //the size of the param
                            )
{
    return AddMsgToQue(NULL, msg_id, param_ptr, size_of_param, MSG_TYPE_BROADCAST );


}// end MMK_PostBCMessage

/*****************************************************************************/
// 	Description : dispatch the external signal 
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_DispatchExtSig(
                          MmiSignalS** signal_ptr // the pointer of message which need dispatch
                          )
{
    //check whether the input param is valid
    //SCI_ASSERT( PNULL != signal_ptr );
    if ( PNULL == signal_ptr || PNULL == (*signal_ptr) )
    {
        return;
    }

    switch( (*signal_ptr)->Sender )
    {
    case KPDSVR:
        //the signal  is the keypad signal
        MMK_DispatchMSGKbd( (MmiKeyPress*)(*signal_ptr) );
        break;

    case P_APP:
        //the signal is the timer
        if( APP_TIMER_EXPIRY == (*signal_ptr)->SignalCode )
        {
            MMK_DispatchMSGTimer( (Signal*)(*signal_ptr));
        }
        else if( APP_GSENSOR == (*signal_ptr)->SignalCode )
        {
            MMK_DispatchGsensorMsg( (*signal_ptr));
        }
        else if(MMI_GROUP == (((*signal_ptr)->SignalCode&0xff00) >> 8))
        {
            DispatchMmiSig((*signal_ptr));
        }
        else
        {
            DispatchSysSig((*signal_ptr));
        }
        break;
        
    case P_TPP:
#ifdef TOUCH_PANEL_SUPPORT
        //the signal  is the touch pannel signal
        MMK_DispatchMSGTp( (MmiTPPress*)(*signal_ptr) );
#endif
    	break;
//============== MET MEX  start ===================
#ifdef MET_MEX_SUPPORT
    case MET_MEX_SEND:
        {
            MetMex_DispatchSignal( (MmiTimerExpiryS*)*signal_ptr );
        }
        break;
#endif // MET_MEX_SUPPORT
//============== MET MEX  end ===================
    default:
        //SCI_TRACE_LOW:"MMK_DispatchExtSig, send = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MSG_579_112_2_18_3_22_42_10,(uint8*)"d", (*signal_ptr)->Sender );
        DispatchSysSig((*signal_ptr));
        break;
    }
}// end MMK_DispatchExtSig

/*****************************************************************************/
// 	Description : dispatch the window message 
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_DispatchWinMSG(
                          MMI_MESSAGE_PTR_T mmi_msg_ptr // the pointer of message which need dispatch
                          )
{
    //check whether the input param is valid                
    SCI_ASSERT( PNULL != mmi_msg_ptr );/*assert verified*/
    
    //translate the own of message and if the own is control then get 
    //the control parent and dispatch the message to window
    if( 0 != mmi_msg_ptr->handle )
    {
        //translate the message to the pointed window
        MMK_DispatchToHandle(mmi_msg_ptr->handle,
            mmi_msg_ptr->msg_id,
            mmi_msg_ptr->param_ptr 
            );
    }
//     else
//     {
//         
//         //translate the message to the focus window
//         MMK_DispMsgToWin( mmi_msg_ptr->msg_id , mmi_msg_ptr->param_ptr );
//     }
}

/*****************************************************************************/
// 	Description : set the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseNodeCompare( 
                              MMI_BASE_NODE_T* node_ptr,
                              uint32 condition1, 
                              uint32 condition2
                              ) 
{
    if( MMI_HANDLE_WINDOW == node_ptr->handle_type )
    {
        HandleMsgFunc( node_ptr->handle, condition1, (DPARAM )condition2 );
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : dispatch the broadcast message to the opend window
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_DispatchBCMSG(
                          MMI_MESSAGE_PTR_T mmi_msg_ptr // the pointer of message which need dispatch
                          )
{
    MMK_FindInBaseNodeByCondition( BaseNodeCompare, mmi_msg_ptr->msg_id, (uint32)mmi_msg_ptr->param_ptr, 0, FALSE );
#if 0
    MMI_WINDOW_T*       openedwin_ptr   = PNULL;

    //check whether the input param is valid
    SCI_ASSERT( PNULL != mmi_msg_ptr  );/*assert verified*/

    //get the first virtual window pointer
    openedwin_ptr = MMK_GetVirtualWinHead();

    while( PNULL != openedwin_ptr )
    {
        HandleMsgFunc((PWND)openedwin_ptr,mmi_msg_ptr->msg_id ,mmi_msg_ptr->param_ptr );
        
        openedwin_ptr = MMK_GetPrevWinHandle( openedwin_ptr );
        
    }// end while


    //get the first focus window
    openedwin_ptr = MMK_GetFocusWin();

    while( PNULL != openedwin_ptr )
    {
        HandleMsgFunc((PWND)openedwin_ptr,mmi_msg_ptr->msg_id ,mmi_msg_ptr->param_ptr );

        openedwin_ptr = MMK_GetPrevWinHandle( openedwin_ptr );
        
    }//end while
#endif
}//end MMK_DispatchBCMSG


/*****************************************************************************/
// 	Description : dispatch the ps signal 
//	Global resource dependence : g_reg_app
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL void DispatchSysSig(
                          MmiSignalS* signal_ptr // the pointer of message which need dispatch
                          )
{
    MMI_APPLICATION_T*  handleapp_ptr   = PNULL;
    MMI_RESULT_E        result          = MMI_RESULT_FALSE;
    uint32              i               = 0;
    uint32              regapp_num      = 0;
    //check whether the input param is valid
    SCI_ASSERT( PNULL != signal_ptr );/*assert verified*/

    SCI_TRACE_LOW("DispatchSysSig msg signal code :0x%x",signal_ptr->SignalCode);
#ifdef UAL_SUPPORT
    //check是否为UAL msg范围内的消息处理
    if( UAL_CMS_GROUP == (signal_ptr->SignalCode>>8 & 0xFF))
    {
    //发送来的msg_id为UAL msg dispatch功能处理的内容，因此直接调用ual_msg_dispatch函数
        ual_cms_msg_dispatch((DPARAM)signal_ptr);
        SCI_TRACE_LOW("msg is ual msg, ");
        return;
    }
#endif
#ifdef UAL_GNSS_SUPPORT
    if(signal_ptr->SignalCode >= LIBGPS_DOWNLOAD_CNF && signal_ptr->SignalCode <= LIBGPS_END_SIGNAL)
    {
        SCI_TRACE_LOW("msg is from libgps");
        ual_gnss_process_libgps_msg(signal_ptr);
        return;   //bug2081879
    }
#endif
    // get the regapp number
    regapp_num = MMI_GetRegAppNum();

    for( i = 0; i < regapp_num; i++ )
    {
        if( signal_ptr->SignalCode >= g_reg_app[i].start_sig &&
            signal_ptr->SignalCode <= g_reg_app[i].end_sig  &&
            NULL != g_reg_app[i].app_ptr )
        {
            //get the pointer of the application which register this signal
            //call the callback function of this application and send the signal
            //to the function
            handleapp_ptr = g_reg_app[i].app_ptr;
            g_reg_app_idx = i;

            if(NULL != handleapp_ptr->ProcessMsg)
            {
                result = (* (handleapp_ptr->ProcessMsg))((PWND)handleapp_ptr,
                                                signal_ptr->SignalCode,
                                                (DPARAM)signal_ptr);
            }
            else
            {
                SCI_TRACE_LOW("DispatchSysSig ProcessMsg func is NULL, SignalCode =%d,i =%d",signal_ptr->SignalCode,i);
            }

            //result of callback function is TRUE then return this fucntion
            //else search the next application which register this signal
            if( MMI_RESULT_TRUE == result )
            {
                return;
            }


        }// end if

    } //end for

}// end DispatchSysSig
/*****************************************************************************/
// 	Description : dispatch the window message to window or the parent window of control
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
BOOLEAN  MMK_DispatchToHandle(
                              MMI_HANDLE_T        handle,
                              MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                              DPARAM              param_ptr   //the param of the message
                              )
{
    BOOLEAN           def_handle_result     = FALSE;
    BOOLEAN           openwin_handle_result = FALSE;
    BOOLEAN           bResult               = FALSE;
    MMI_HANDLE_T      old_handle            = MMK_GetProcMsgHandle();
    MMI_RESULT_E      win_handle_result     = MMI_RESULT_FALSE;
    MMI_RESULT_E      ctrl_handle_result    = MMI_RESULT_FALSE;
    MMI_HANDLE_TYPE_E type                  = MMK_GetHandleType( handle );
	MMI_CheckAllocatedMemInfo();
    switch (type)
    {
    case MMI_HANDLE_CONTROL:
        bResult = CTRLSYS_DispatchToHandle(handle,msg_id,param_ptr);
        break;

    case MMI_HANDLE_WINDOW:
        //set the window pointer that is processing the message  
        MMK_SetProcMsgHandle( handle );
        
        //call the default proces function to 
        ctrl_handle_result = MMK_DefaultProcessWinMsg( handle, msg_id, param_ptr );
        
        switch( ctrl_handle_result )
        {
        case MMI_RESULT_TRUE:
            bResult = TRUE;
            break;
            
        case MMI_RESULT_FALSE:
            //this situation is that the hwnd is window pointer 
            //and the result of default process is FALSE
            openwin_handle_result = MMK_ProcOpenWinMsg( handle, msg_id, param_ptr );
            
#ifdef UI_P3D_SUPPORT   
            MMK_HandleWinPreMoveMsg( handle, msg_id, param_ptr);
#endif
            if (IsAnimBg(handle,msg_id))
            {
                //call process function to handle the special window message
                def_handle_result = MMK_ProcSpecialWinMsg( handle, msg_id, param_ptr );
                
                //call the process function of handlewin_ptr
                win_handle_result =  MMK_RunWinProc( handle, msg_id, param_ptr );
            }
            else
            {
                //call the process function of handlewin_ptr
                win_handle_result =  MMK_RunWinProc( handle, msg_id, param_ptr );
                
                //call process function to handle the special window message
                def_handle_result = MMK_ProcSpecialWinMsg( handle, msg_id, param_ptr );
            }
#ifdef UI_P3D_SUPPORT   
            //note: MMK_HandleWinMoveMsg(MSG_FULL_PAINT) must after MMK_RunWinProc
            MMK_HandleWinMoveMsg( handle, msg_id, param_ptr);
#endif

#ifdef UI_MULTILAYER_SUPPORT
            if (MSG_CLOSE_WINDOW == msg_id)
            {
                UILAYER_ReleaseAllLayerOfHandle(handle);
            }
            else 
#endif
            if (MSG_LOSE_FOCUS == msg_id)
            {
#ifdef UI_MULTILAYER_SUPPORT
                MMI_HANDLE_T parent_handle = MMK_GetParentWinHandle( handle );

                if ( ( 0 == parent_handle || MMK_GetWinDisplayStyleState( parent_handle, WS_TOOL_WIN ) )
                    && MMK_IsWinNeedCopyLcdInLoseFocus(handle))
                {
                    UILAYER_CopyLcdToMainLayer();
                }

                UILAYER_DeactiveAllLayerOfHandle(handle);
#endif
            }
            
            //return the result of handle message
            bResult = (BOOLEAN)(win_handle_result | def_handle_result | openwin_handle_result);

            if (!bResult)
            {
                MMK_TRAVEL_ACTIVE_E travel_active = MMK_TRAVEL_ACTIVE_MAX;

                switch (msg_id)
                {
                case MSG_APP_UP:
                    travel_active = MMK_TRAVEL_ACTIVE_UP;
                    break;

                case MSG_APP_DOWN:
                    travel_active = MMK_TRAVEL_ACTIVE_DOWN;
                    break;

                case MSG_APP_LEFT:
                    travel_active = MMK_TRAVEL_ACTIVE_LEFT;
                    break;

                case MSG_APP_RIGHT:
                    travel_active = MMK_TRAVEL_ACTIVE_RIGHT;
                    break;

                default:
                    break;
                }

                if (MMK_TRAVEL_ACTIVE_MAX != travel_active)
                {
                    MMK_TravelActiveCtrl(handle, travel_active);
                }
            }
            break;
            
        default:
            //SCI_TRACE_LOW:"MMK_DispatchToHandle, ctrl_handle_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MSG_897_112_2_18_3_22_42_11,(uint8*)"d", ctrl_handle_result);
            break;
        }

        MMK_SetProcMsgHandle( old_handle );
        break;
    
    case MMI_HANDLE_APPLET:
        MMK_ProcSpecialAppletMsg( handle, msg_id, param_ptr );
        MMK_RunAppletProc( handle, msg_id, param_ptr );/* Stop applet after all windows closed */
        bResult = TRUE;
        break;

    default:
        //SCI_TRACE_LOW:"MMK_DispatchToHandle handle type is MMI_HANDLE_NONE!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MSG_911_112_2_18_3_22_42_12,(uint8*)"");
        break;
    }
    MMI_CheckAllocatedMemInfo();
    return bResult;
}

/*****************************************************************************/
// 	Description : dispatch the window message to focus window
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DispMsgToFocusParentWin(
                                      MMI_MESSAGE_ID_E  msg_id,     //the identify of message
                                      DPARAM            param_ptr   //the param of the message
                                      )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T win_handle = MMK_GetFocusParentWinHandle();

	if ( 0 != win_handle )
	{
		result = MMK_DispatchToHandle( win_handle, msg_id, param_ptr );
	}	
    
    return result;
}

/*****************************************************************************/
// 	Description : dispatch the window message to focus window
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
BOOLEAN  MMK_DispMsgToFocusWin(
                                MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                                DPARAM              param_ptr   //the param of the message
                                )
{
    BOOLEAN      result     = FALSE;
    MMI_HANDLE_T win_handle = MMK_GetFocusWinHandle();

    if(msg_id == MSG_APP_CANCEL && (MMICC_IsInActiveCallWin() || MMIZDT_ShouldStayInChargingWin() || MMIZDT_ShouldStayInClassModeWin()))
    {
        return TRUE;
    }

    result = MMK_DispatchToHandle( win_handle, msg_id, param_ptr );

    return result;
}

/*****************************************************************************/
// 	Description : dispatch the window message to focus window
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
BOOLEAN  MMK_DispMsgToWin(
                          MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                          DPARAM              param_ptr   //the param of the message
                          )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T win_handle = MMK_GetFocusParentWinHandle();

    if ( MMK_GetWinDisplayStyleState( win_handle, WS_DISPATCH_TO_CHILDWIN ) )
    {
        result = MMK_DispMsgToFocusWin( msg_id, param_ptr );
        if (!result)
        {
            result = DispMsgToFocusParentWin( msg_id, param_ptr );
        }
    }
    else
    {
        result = DispMsgToFocusParentWin( msg_id, param_ptr );
        if (!result)
        {
            result = MMK_DispMsgToFocusWin( msg_id, param_ptr );
        }
    }

    //if msg_id is MSG_BACKLIGHT_TURN_ON, post MSG_FULL_PAINT to win
    if( MSG_BACKLIGHT_TURN_ON == msg_id )
    {
        //需要刷新所有的窗口, 不能只画当前窗口
        MMK_UpdateScreen();
    }

    return result;
}

/*****************************************************************************/
// 	Description : handle message function
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  HandleMsgFunc(
                             MMI_HANDLE_T        handle,
                             MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                             DPARAM              param_ptr   //the param of the message
                             )
{
    MMI_RESULT_E        ctrl_handle_result = MMI_RESULT_FALSE;
    BOOLEAN             win_handle_result  = FALSE;
    MMI_HANDLE_T        old_handle         = MMK_GetProcMsgHandle();
    
    //set the window pointer that is processing the message  
    MMK_SetProcMsgHandle( handle );

    //call the default proces function to 
    ctrl_handle_result = MMK_DefaultProcessWinMsg( handle, msg_id, param_ptr );

    switch( ctrl_handle_result )
    {
    case MMI_RESULT_TRUE:
        win_handle_result = TRUE;
        break;

    case MMI_RESULT_FALSE:
        //call the process function of handle window
        win_handle_result = (BOOLEAN)MMK_RunWinProc( handle, msg_id, param_ptr );

        break;

    default:
        break;    
    }

    MMK_SetProcMsgHandle( old_handle );

    return win_handle_result;
}

/*****************************************************************************/
// 	Description : get the current queue msg
//	Global resource dependence : ;
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_MESSAGE_PTR_T MMK_GetCurQueueMsg(
                                            void
                                            )
{
    return (&(s_mmk_msg_queue.msg[s_mmk_msg_queue.readpos]));
}
/*****************************************************************************/
// 	Description : delete the message by the window or control handleptr
//                在Close窗口或者控件时需要调用这个函数来清理消息队列，
//                以避免当窗口或控件被释放后仍然收到消息的问题。
//                实际上解决这个问题还有另外一种思路：即将内部队列的管理权
//                交给窗口，当窗口销毁时，消息也同时销毁。我们目前对消息的
//                集中式管理导致同步有些困难，下面这个函数就是为了同步用到的。              
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DeleteMSGByHandle(
                                  MMI_HANDLE_T handle
                                  )
{
    uint8           i            = 0;
    MMI_MESSAGE_T*  temp_msg_ptr = PNULL;

    for( i = 0; i < s_mmk_msg_queue.len; i++ )
    {
        temp_msg_ptr = &(s_mmk_msg_queue.msg[i]);

        if ( handle == temp_msg_ptr->handle )
        {
            // 需要释放Param占用的MEM空间
            if (PNULL != temp_msg_ptr->param_ptr)
            {
                SCI_FREE(temp_msg_ptr->param_ptr);
            }

            temp_msg_ptr->handle    = PNULL;
            temp_msg_ptr->msg_id    = 0;
            temp_msg_ptr->msg_type  = MSG_TYPE_NONE;
            temp_msg_ptr->param_ptr = PNULL;
        }
    }
}

/*****************************************************************************/
// 	Description : 删除队列中对应窗口的对应消息
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DeleteMSGByHwndAndMsg(
                                      MMI_HANDLE_T handle,
                                      MMI_MESSAGE_ID_E msg_id //[IN] message id
                                      )
{
    uint8           i            = 0;
    MMI_MESSAGE_T*  temp_msg_ptr = PNULL;

    //SCI_PASSERT(0 != handle, ("MMK_DeleteMSGByHwndAndMsg: the handle is 0!"));
    if ( 0 == handle )
    {
        return;
    }

    for(i = 0; i < s_mmk_msg_queue.len; i++)
    {
        temp_msg_ptr = &(s_mmk_msg_queue.msg[i]);

        if ( handle == temp_msg_ptr->handle
            && msg_id == temp_msg_ptr->msg_id )
        {
            // 需要释放Param占用的MEM空间
            if ( PNULL != temp_msg_ptr->param_ptr )
            {
                SCI_FREE( temp_msg_ptr->param_ptr );
            }

            temp_msg_ptr->handle    = 0;
            temp_msg_ptr->msg_id    = 0;
            temp_msg_ptr->msg_type  = MSG_TYPE_NONE;
            temp_msg_ptr->param_ptr = PNULL;
        }
    }
}

/*****************************************************************************/
// 	Description : 删除队列中对应的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DeleteMsgByCallback(
                                    void                *in_param_ptr,      //in:
                                    MMK_DEL_MSG_PFUNC   is_delete_callback  //in:
                                    )
{
	uint8           i = 0;
    MMI_MESSAGE_T   *msg_ptr = PNULL;

    for (i=0; i<s_mmk_msg_queue.len; i++)
    {
        msg_ptr = &(s_mmk_msg_queue.msg[i]);

        if (is_delete_callback(in_param_ptr,msg_ptr))
        {
            if (PNULL != msg_ptr->param_ptr)
            {
                SCI_FREE(msg_ptr->param_ptr);
            }

            msg_ptr->handle    = 0;
            msg_ptr->msg_id    = 0;
            msg_ptr->msg_type  = MSG_TYPE_NONE;
            msg_ptr->param_ptr = PNULL;
        }
    }
}

/*****************************************************************************/
// 	Description : dispatch other mmi task signal 
//	Global resource dependence :
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void DispatchMmiSig(
                          MmiSignalS* signal_ptr // the pointer of message which need dispatch
                          )
{
    MMK_HandleMmiSig(signal_ptr->SignalCode, (DPARAM)signal_ptr);
}

/*****************************************************************************/
// 	Description : 是否动画作为背景
//	Global resource dependence :
//  Author: Jassmine
//	Note: 比如墙纸,full paint需要先发给控件,后发给窗口
/*****************************************************************************/
LOCAL BOOLEAN IsAnimBg(
                       MMI_HANDLE_T     win_handle,
                       MMI_MESSAGE_ID_E msg_id
                       )
{
    BOOLEAN     result = FALSE;

    if ( MSG_FULL_PAINT == msg_id )
    {
        win_handle = MMK_ConvertIdToHandle( win_handle );
        
        if (MMK_GetWinDisplayStyleState(win_handle,WS_HAS_ANIM_BG))
        {
            result = TRUE;
        }
    }

    return (result);
}
