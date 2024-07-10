/****************************************************************************
** File Name:      mmivt_kernel_324Adapter.c                               *
** Author:                                                                 *
** Date:           12/18/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2009        samboo.shen         Create
****************************************************************************/
//#ifndef _MMIVT_KERNEL_324ADAPTER_C_
#define _MMIVT_KERNEL_324ADAPTER_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_vt_trc.h"
#ifdef __cplusplus
extern   "C"
{
#endif
    
    //#include "mmivt_export.h"
#ifdef VT_SUPPORT
    
#include "mmi_osbridge.h"
#include "mmi_appmsg.h"
#include "tasks_id.h"
    
#ifndef WIN32
#include "sci_mplapi.h"
#endif
    
    
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
#ifdef WIN32
/*H324M ---> APP MSG*/
typedef enum
{
    MPLH324M_MSG_NULL = 0,
        MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC = 1,
        MPLH324M_MSG_STARTMEDIATX,
        MPLH324M_MSG_STARTMEDIARX,
        MPLH324M_MSG_STARTMEDIATXRX,
        MPLH324M_MSG_CLOSE_IND,
        MPLH324M_MSG_CLOSE_CNF,
        MPLH324M_MSG_MEDIA_INACTIVE_IND,
        MPLH324M_MSG_MEDIA_ACTIVE_IND,
        MPLH324M_MSG_ROUNDTRIP_CNF,
        MPLH324M_MSG_ABORTMEDIA,
        MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_START,
        MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_STOP,
        MPLH324M_MSG_END
}MPLH324M_MSG_TYPE_E;

/*CLOSE causes */
typedef enum
{
    CLOSE_CAUSE_BYUSER = 1,
        CLOSE_CAUSE_BYREMOTE,
        CLOSE_CAUSE_BYPROTOCAL,
        CLOSE_CAUSE_BYSW
}MPLH324M_MSG_CLOSE_CAUSE_E;
#endif
    
    
typedef struct _VT_MSG_Tag
{
    uint32		msg_id;
    uint8		status;
    uint32      body_size;
    void*       body_ptr;
} VT_MSG_T;

typedef struct _VT_SIGNAL324{
    SIGNAL_VARS
    VT_MSG_T	    content;			// message content
} VT_SIGNAL324;
    
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : callback function, used in MPLH324M_Open
//	Global resource dependence : 
//  Author: jibin
//	Note:
/*****************************************************************************/
LOCAL void CallbackMsg(MPLH324M_MSG_TYPE_E i_msg, int argc, void *argv[]);

/*****************************************************************************/
// 	Description : SendSignal
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SendSignal( uint16 msg_id);


/*****************************************************************************/
// 	Description : callback function
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void CallbackMsg(MPLH324M_MSG_TYPE_E i_msg, int argc, void *argv[])
{
    
    //SCI_TRACE_LOW:"mmivt.c CallbackMsg() i_msg = %04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_111_112_2_18_3_6_2_0,(uint8*)"d", i_msg);
    switch (i_msg)
    {
    case MPLH324M_MSG_STARTMEDIATX:
    case MPLH324M_MSG_STARTMEDIARX:
    case MPLH324M_MSG_STARTMEDIATXRX:
        {
            SendSignal(APPSIG_MPLH324M_MSG_STARTMEDIA);
        }  
        break;
        
    case MPLH324M_MSG_CLOSE_IND:    //closed msg
        //SCI_TRACE_LOW:"mmivt.c: CallbackMsg() -> MPLH324M_MSG_CLOSE_IND argc = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_123_112_2_18_3_6_2_1,(uint8*)"d",argc);
        if ( 1 == argc)
        {
            SendSignal(APPSIG_MPLH324M_MSG_CLOSE_IND);
        }
        break;
    case MPLH324M_MSG_CLOSE_CNF:    //closed msg
        {
            SendSignal(APPSIG_MPLH324M_MSG_CLOSE_CNF);
        }   
        break;
        
    case MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_START://对方无视频数据
        {
            SendSignal(APPSIG_MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_START);
        }   
        break;
    case MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_STOP://对方恢复视频数据
        {
            SendSignal(APPSIG_MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_STOP);
        }   
        break;
        
    case MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC://user input
        {
            VT_SIGNAL324    *sendSignal ;
            //SCI_TRACE_LOW:"mmivt.c CallbackMsg() i_msg = MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_149_112_2_18_3_6_2_2,(uint8*)"");
            MmiCreateSignal(APPSIG_MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC,sizeof(VT_SIGNAL324),(MmiSignalS**)&sendSignal);
            sendSignal->Sender = SCI_IdentifyThread();
            sendSignal->content = *(VT_MSG_T*)argv[0];
            MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
        }
        break;

    case MPLH324M_MSG_MEDIA_INACTIVE_IND:
        {
            SendSignal(APPSIG_MPLH324M_MSG_MEDIA_INACTIVE_IND);
        }
        break;

    case MPLH324M_MSG_MEDIA_ACTIVE_IND:
        {
            SendSignal(APPSIG_MPLH324M_MSG_MEDIA_ACTIVE_IND);
        }   
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : SendSignal
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SendSignal( uint16 msg_id)
{
    MmiSignalS *sendSignal = PNULL;
    MmiCreateSignal(msg_id,sizeof(MmiSignalS), (MmiSignalS**)&sendSignal);
    MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
}

/*****************************************************************************/
// 	Description : open the 324 stack
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_Open(int32 b_mo)
{
    //SCI_TRACE_LOW:"mmivt MMIVT_Open() b_mo[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_191_112_2_18_3_6_3_3,(uint8*)"d", b_mo);
    
#ifndef WIN32
    {
        int32 error_code = MPLH324M_Open(b_mo,CallbackMsg);//@zhaohui modify

        if (0 != error_code)
        {
            //SCI_TRACE_LOW:"mmivt.c MMIVT_Open() fail error_code = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_199_112_2_18_3_6_3_4,(uint8*)"d", error_code);
            return FALSE;
        }
    }

#endif
    return TRUE;
}

/*****************************************************************************/
// 	Description : close the 324 stack
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_324_Close(void)
{
    //SCI_TRACE_LOW:"mmivt.c MMIVT_324_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_214_112_2_18_3_6_3_5,(uint8*)"");
#ifndef WIN32
    MPLH324M_Close();
#endif
}

/*****************************************************************************/
// 	Description :  make call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_MakeCall(void)
{
    //SCI_TRACE_LOW:"mmivt.c MMIVT_MakeCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_226_112_2_18_3_6_3_6,(uint8*)"");

#ifndef WIN32
    {
        int32 error_code = MPLH324M_MakeCall(); 
    
        if (0 != error_code)
        {
            //SCI_TRACE_LOW:"videcall.c MMIVT_MakeCall() fail error_code = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_234_112_2_18_3_6_3_7,(uint8*)"d", error_code);
            return FALSE;
        }
    }
#else
    CallbackMsg(MPLH324M_MSG_STARTMEDIATXRX, 0, 0);
#endif
    return TRUE;
}

/*****************************************************************************/
// 	Description :  listen for call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_ConnectReq(void)
{
    //SCI_TRACE_LOW:"mmivt.c MMIVT_324_ConnectReq"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_250_112_2_18_3_6_3_8,(uint8*)"");
#ifndef WIN32
    {
        int32 error_code = MPLH324M_ListenForCall();

        if (0 != error_code)
        {
            //SCI_TRACE_LOW:"videcall.c MMIVT_ListenForCall() fail error_code = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_257_112_2_18_3_6_3_9,(uint8*)"d", error_code);
            return FALSE;
        }
    }

#else
    CallbackMsg(MPLH324M_MSG_STARTMEDIATXRX, 0, 0);
#endif
    return TRUE;
}

/*****************************************************************************/
// 	Description :  hangup
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_HangUp(void)
{
    //SCI_TRACE_LOW:"mmivt.c MMIVT_324_HangUp"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_274_112_2_18_3_6_3_10,(uint8*)"");
#ifndef WIN32
    {
        int32 error_code = MPLH324M_HangUp();

        if (0 != error_code)
        {
            //SCI_TRACE_LOW:"videcall.c MMIVT_HangUp() fail error_code = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_281_112_2_18_3_6_3_11,(uint8*)"d", error_code);
            return FALSE;
        }        
    }
#else
    {
        void *argv[1] = {PNULL};
        argv[0] = (void*)CLOSE_CAUSE_BYUSER;
        CallbackMsg(MPLH324M_MSG_CLOSE_IND, 1, argv);
        CallbackMsg(MPLH324M_MSG_CLOSE_CNF, 1, argv);    
    }
#endif
    return TRUE;
}

/*****************************************************************************/
// 	Description :  SendUserInput
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_324_SendUserInput(uint8 send_char)
{
    if((send_char >= '0' && send_char <= '9') || 
         (send_char >= 'A' && send_char <= 'D') ||
         (send_char >= 'a' && send_char <= 'd') ||
          send_char == '#' ||
          send_char =='*')
        
    {
#ifndef WIN32
        MPLH324M_SendUsrInput(send_char);
#else 
        void *argv[1] = {PNULL};
        argv[0] = (void*)&send_char;
        CallbackMsg(MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC, 1, argv);
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"mmivt send user input wrong %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_319_112_2_18_3_6_3_12,(uint8*)"d", send_char);
    }
    
}


/*****************************************************************************/
// 	Description :  MMIVT_324_EnableLocalImageToRemote
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: 通知对方打开或者关闭摄像头
/*****************************************************************************/
PUBLIC void MMIVT_324_EnableCamera(BOOLEAN is_enable)
{
    int32 error_code = 0;
#ifndef WIN32
    error_code = MPLH324M_SetVideoSuspend((int)!is_enable);

#endif
    if (0 != error_code)
    {
        //SCI_TRACE_LOW:"mmivt MMIVT_324_EnableCamera fail code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_324ADAPTER_338_112_2_18_3_6_3_13,(uint8*)"d", error_code);
        return;
    }
}

    
#endif //VT_SUPPORT
    
#ifdef   __cplusplus
    }
#endif
    
