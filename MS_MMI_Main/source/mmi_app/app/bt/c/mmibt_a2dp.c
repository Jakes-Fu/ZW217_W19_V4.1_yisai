/****************************************************************************
** File Name:      mmibt_a2dp.c                                       *
** Author:         kelly.li                                            *
** Date:           August/11/2006                                             *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME             DESCRIPTION                             *
** August/11/2006   kelly.li         Create                                  *
**                                                                         *
****************************************************************************/
//#ifndef _MMIBT_A2DP_C_
#define _MMIBT_A2DP_C_
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmibt_a2dp.h"
#include "mmiset_export.h"
#include "mmibt_app.h"
#include "mmi_appmsg.h"
#include "mmibt_func.h"
#include "mmibt_id.h"
#include "mmi_default.h"
#include "audio_config.h"
#include "mmifmm_export.h"   //audio_api.h中用到了ffs.h中的结构
#include "audio_api.h"
#include "mmimp3_export.h"
#include "mmimp3_earphone.h"
#include "mmifm_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmiudisk_export.h"
#include "mmibt_headset.h"
#ifdef BT_A2DP_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

extern HAUDIODEV hA2DPDev; /*lint !e762*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIA2DP_DEVICE_INFO_T     s_mmi_a2dp_dev_info = {0};              //record the device information
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Get A2DP actived device information
//  Author:         
//  Note:
//  Parameter:      has_active_device: used to tell app that whether there is a 
//					actived device
//					TRUE:  has a actived a2dp device
//					FALSE: no actived device
//  Returns:        return the address of actived device, if has_active_device
//					is FALSE, app should ignore this return parameter. 
//****************************************************************************/
//LOCAL BT_ADDRESS MMIBT_GetActiveA2DPDevice(BOOLEAN* has_active_device);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : if there is active A2DP headset
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIBT_IsExitedActiveA2dp(void)
{
    BOOLEAN         is_exited = FALSE;
    BT_ADDRESS      bt_address = {0};
    
    bt_address = MMIBT_GetActiveA2DPDevice(&is_exited);
    if(is_exited)
    {
         is_exited = MMIBT_DeviceIsConnectedHeadset(&bt_address);
    }
    return is_exited;
}

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIBT_IsA2dpReadyForPlay(void)
{
    BOOLEAN result = FALSE;

    if(MMIBT_HEADSET_STATUS_OPEN   == MMIBT_GetDeviceStatus()  ||
       MMIBT_HEADSET_STATUS_RESUME == MMIBT_GetDeviceStatus())
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIBT] MMIBT_IsA2dpReadyForPlay: bt_status = %d, result = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_112_112_2_18_2_7_43_0,(uint8*)"dd", MMIBT_GetDeviceStatus(),result);

    return result;
}

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIBT_IsA2dpPaused(void)
{
    BOOLEAN result = FALSE;

    if(MMIBT_HEADSET_STATUS_PAUSE == MMIBT_GetDeviceStatus())
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIBT] MMIBT_IsA2dpPaused: bt_status = %d, result = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_130_112_2_18_2_7_43_1,(uint8*)"dd", MMIBT_GetDeviceStatus(),result);
    
    return result;
}

/*****************************************************************************/
//  Description : pause a2dp device of playing mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpPause(
                     uint32 sample_ret
                    )
{
    BOOLEAN  action_result = FALSE;
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_A2dpPause enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_144_112_2_18_2_7_43_2,(uint8*)"");
    
    if (BT_SUCCESS == A2DP_PauseDevice())
    {
        //SCI_TRACE_LOW:"[MMIBT]Bt_A2dpPause: A2DP_PauseDevice is called!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_148_112_2_18_2_7_43_3,(uint8*)"");
        action_result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIBT]Bt_A2dpPause: call A2DP_PauseDevice fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_153_112_2_18_2_7_43_4,(uint8*)"");
        action_result = FALSE;
    }
    return action_result;
}

/*****************************************************************************/
//  Description : stop a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpStopDevice(
                    uint32 sample_ret
                    )
{
#if 0
    BOOLEAN                     action_result = FALSE;

    //SCI_TRACE_LOW:"[MMIBT]: MMIBT_A2dpStopDevice"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_170_112_2_18_2_7_43_5,(uint8*)"");

    if(MMIBT_HEADSET_STATUS_RESUME != MMIBT_GetDeviceStatus() 
        && (MMIBT_HEADSET_STATUS_PAUSE != MMIBT_GetDeviceStatus())
        && (MMIBT_HEADSET_STATUS_OPEN != MMIBT_GetDeviceStatus()) 
       )
    {
        //need first stop device
        MMIBT_HandleAction(MMIBT_Out_ActionQueue());//HandleA2DPAction(MMIBT_Out_ActionQueue());	
    }
    else if (BT_SUCCESS == A2DP_CloseDevice())
    {
        //SCI_TRACE_LOW:"[MMIBT]:MMIBT_A2dpStopDevice: A2DP_CloseDevice is called!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_182_112_2_18_2_7_43_6,(uint8*)"");
        action_result = TRUE;
    }   
    else
    {
        //SCI_TRACE_LOW:"[MMIBT]:MMIBT_A2dpStopDevice: call A2DP_CloseDevice fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_187_112_2_18_2_7_43_7,(uint8*)"");
         action_result = FALSE;
    }

    return action_result;
#else
    if(BT_SUCCESS == A2DP_CloseDevice())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
#endif
}

/*****************************************************************************/
//  Description : Resume a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIBT_A2dpResumeDevice(
                     uint32 sample_ret
                    )
{
    BOOLEAN result = FALSE;

    //SCI_TRACE_LOW:"[MMIBT]: MMIBT_A2dpResumeDevice"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_215_112_2_18_2_7_43_8,(uint8*)"");
    //call a2dp function to resume music
    if(MMIBT_HEADSET_STATUS_PAUSE != MMIBT_GetDeviceStatus())
    {
        MMIBT_HandleAction(MMIBT_Out_ActionQueue());//HandleA2DPAction(MMIBT_Out_ActionQueue());
    }
    else if (BT_SUCCESS == A2DP_ResumeDevice())
    {
        //SCI_TRACE_LOW:"[MMIBT]MMIBT_A2dpResumeDevice: A2DP_ResumeDevice is called!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_223_112_2_18_2_7_43_9,(uint8*)"");
        //MMIBT_HiddenBtForAPP(MMIBT_MODULE_A2DP,BT_PAGE_ENABLE);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIBT]MMIBT_A2dpResumeDevice: call A2DP_ResumeDevice fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_229_112_2_18_2_7_43_10,(uint8*)"");
        result = FALSE;
    }           
    return result;
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpOpenDevice(
                    uint32 sample_ret
                    )
{
    //目前存在PCM模式则断掉链路
    BOOLEAN action_result = FALSE;

    //SCI_TRACE_LOW("[MMIBT]MMIBT_A2dpOpenDevice");
    //call a2dp function to play music 
    if (BT_SUCCESS == A2DP_OpenDevice(sample_ret))
    {
        action_result = TRUE;
        //MMIBT_HiddenBtForAPP(MMIBT_MODULE_A2DP,BT_PAGE_ENABLE);
        //SCI_TRACE_LOW:"[MMIBT]MMIBT_A2dpOpenDevice: A2DP_OpenDevice is called!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_252_112_2_18_2_7_43_11,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIBT]MMIBT_A2dpOpenDevice: A2DP_OpenDevice failed !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_256_112_2_18_2_7_43_12,(uint8*)"");
    }

    return action_result;
}

/*****************************************************************************/
//  Discription: Get the status of current a2dp
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetCurrentA2dpStatus(
                                        BT_ADDRESS  *addr_ptr
                                        )
{
    BOOLEAN         is_active = FALSE;
    BOOLEAN         is_exited_active = FALSE;
    BT_ADDRESS      addr_info = {0};

    if (PNULL == addr_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_GetCurrentA2dpStatus PNULL == addr_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_277_112_2_18_2_7_43_13,(uint8*)"");
        return FALSE;
    }

    addr_info = MMIBT_GetActiveA2DPDevice(&is_exited_active);

     //the a2dp is actived already
    if (is_exited_active)
    {       
        if (0 == GUI_StringCompare(addr_ptr->addr,
                                BT_ADDRESS_SIZE,
                                addr_info.addr,
                                BT_ADDRESS_SIZE))
        {       
            is_active = TRUE;
        }
    }

    return (is_active);
}

/*****************************************************************************/
//  Description : get a2dp link connected indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpLinkConnected(BT_MSG_T     *msg_body_ptr)
{
    BT_A2DP_INFO_T   a2dp_info = *(( BT_A2DP_INFO_T * )msg_body_ptr->body_ptr);
 
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        //set the flag and address
        s_mmi_a2dp_dev_info.address = a2dp_info.dev_addr;
        s_mmi_a2dp_dev_info.has_connected_dev = TRUE;
    }
}

/*****************************************************************************/
//  Description : get a2dp link disconnected indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpLinkDisconnect(BT_MSG_T     *msg_body_ptr)
{   
    //set active device flag to false
    s_mmi_a2dp_dev_info.has_connected_dev = FALSE;
    MMIDEFAULT_SetBackLight(TRUE);
}

/*****************************************************************************/
//  Description : a2dp avrcp play music back word
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpBackword(void)
{
    //mp3 is played in mp3 main window
    MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_KEY_PREV);
}

/*****************************************************************************/
//  Description : a2dp avrcp play music forward
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpForward(void)
{
    //mp3 is played in mp3 main window
    MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_KEY_NEXT);
}

/*****************************************************************************/
//  Description : a2dp avrcp play music adjust volume 
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpAdjustVolum(BOOLEAN volume_up)
{
    if(volume_up)
    {
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_KEY_VOL_UP);
    }
    else
    {
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_KEY_VOL_DOWN);
    }
}

/*****************************************************************************/
//  Description : headset avrcp of a2dp play music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpPlay(void)
{
      MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_KEY_RESUME);
}

/*****************************************************************************/
//  Description : headset avrcp of a2dp stop play music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpStop(void)
{
     MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_KEY_STOP);
}

/*****************************************************************************/
//  Description : headset avrcp of a2dp stop pause music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpPause(void)
{
    MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_KEY_PAUSE);
}

/*****************************************************************************/
//  Description :耳机端主动发起resume stream ind
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpResumeStreamInd(BT_MSG_T* msg_body )
{
//    if (BT_SUCCESS == msg_body->status)
//    {
//        if(MMIBT_HEADSET_STATUS_PAUSE == MMIBT_GetDeviceStatus())
//        {
//            //need pause device
//            A2DP_PauseDevice();
//            //MMIBT_SetA2dpAudioStatus(BT_A2DP_PAUSING_CNF);
//        }
//        else if(MMIBT_HEADSET_STATUS_STOP == MMIBT_GetDeviceStatus() ||MMIBT_HEADSET_NO_STATUS == MMIBT_GetDeviceStatus())
//        {
//            //need close device
//            A2DP_CloseDevice();
//        } 
//     }
}

/*****************************************************************************/
//  Description : 耳机端发起建立链路的消息
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpStartStreamInd(BT_MSG_T* msg_body )
{
    //耳机端建立链路，不主动放MP3，只是close device
    if (BT_SUCCESS == msg_body->status)
    {
        if(MMIBT_HEADSET_STATUS_STOP == MMIBT_GetDeviceStatus() ||MMIBT_HEADSET_NO_STATUS == MMIBT_GetDeviceStatus())
        {
            A2DP_CloseDevice();
        }
        else if(MMIBT_HEADSET_STATUS_PAUSE == MMIBT_GetDeviceStatus())
        {
            //need pause device
            A2DP_PauseDevice();
            //MMIBT_SetA2dpAudioStatus(BT_A2DP_PAUSING_CNF);
        }
    }
}

/*****************************************************************************/
//  Description :耳机端主动发起pause stream的消息
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpPauseStreamInd(BT_MSG_T* msg_body)
{
    if (BT_SUCCESS == msg_body->status)
    {
        //得到pause ind时，如果MP3没有暂停需要暂停
        
        MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_PAUSE);

        //xingdong_bt
        MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_PAUSE);

        //MMIBT_RecoveryBtStatus(MMIBT_MODULE_A2DP);
    }
}

/*****************************************************************************/
//  Description :get a2dp abort indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIBT_A2dpGetAbortInd(void)
{
	//a2dp device has error,need to abort.MMI reset status
	//xingdong_bt
    MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_DISCONNECT);
    
//	SCI_TRACE_LOW("MMIBT_A2dpGetAbortInd: get a2dp abort ind.");
}

/*****************************************************************************/
//  Description : open a2dp headset stereo
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenA2dpHeadset(BT_ADDRESS address)
{
	BOOLEAN result = FALSE;
    
	if (BT_SUCCESS == A2DP_ConnectDevice(address))
	{
		result = TRUE;
	}

	//SCI_TRACE_LOW:"[MMIBT]mmibt_a2dp.c:MMIBT_OpenA2dpHeadset:call A2DP_ConnectDevice:result is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_477_112_2_18_2_7_44_14,(uint8*)"d", result);
	return result;
}

/*****************************************************************************/
//  Description : disconnect a a2dp device by address
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_CloseA2dpHeadset(void)
{
	BOOLEAN result = FALSE;

	if (BT_SUCCESS == A2DP_DisConnectDevice())
	{
		result = TRUE;
	}

	//SCI_TRACE_LOW:"[MMIBT]mmibt_a2dp.c:MMIBT_CloseA2dpHeadset:call A2DP_DisConnectDevice:result is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_494_112_2_18_2_7_44_15,(uint8*)"d", result);
	return result;
}
	
        

/*****************************************************************************/
//  Description : get the sample rate of a2dp stream
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetA2DPSampleRate(void)
{
	return A2DP_GetSampleRate();
}	

/*****************************************************************************/
//  Description:    Get A2DP actived device information
//  Author:         
//  Note:
//  Parameter:      has_active_device: used to tell app that whether there is a 
//					actived device
//					TRUE:  has a actived a2dp device
//					FALSE: no actived device
//  Returns:        return the address of actived device, if has_active_device
//					is FALSE, app should ignore this return parameter. 
//****************************************************************************/
PUBLIC BT_ADDRESS MMIBT_GetActiveA2DPDevice(BOOLEAN* has_active_device)
{
	BT_ADDRESS addr = {0};
	 
	if(s_mmi_a2dp_dev_info.has_connected_dev)
	{
		//get the address of the a2dp device actived 
		addr = s_mmi_a2dp_dev_info.address;
		*has_active_device = TRUE;
	}
	else
	{
		//there is no a2dp device which is actived
		*has_active_device = FALSE;
	}
	
	//SCI_TRACE_LOW:"[MMIBT]MMIBT_GetActiveA2DPDevice:state = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_A2DP_535_112_2_18_2_7_44_16,(uint8*)"d", *has_active_device);
	return addr;
}
//#endif

/*****************************************************************************/
//  Description : get a2dp address
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BT_ADDRESS MMIBT_GetA2dAddress(BT_MSG_T  *msg_body_ptr)
{
    BT_A2DP_INFO_T   a2dp_info = {0};

    if(PNULL != msg_body_ptr && PNULL != msg_body_ptr->body_ptr) 
    {
        a2dp_info = *(( BT_A2DP_INFO_T * )msg_body_ptr->body_ptr);
    }
   SCI_TRACE_LOW("[MMIBT]MMIBT_GetA2dAddress set address(first 4bytes) = 0x%x", a2dp_info.dev_addr.addr);/*lint !e437*/
   return a2dp_info.dev_addr;
}

#else
/*****************************************************************************/
//  Description : pause a2dp device of playing mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpPause( uint32 sample_ret)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpOpenDevice(uint32 sample_ret)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : Resume a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIBT_A2dpResumeDevice( uint32 sample_ret)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : stop a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpStopDevice( uint32 sample_ret)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : get a2dp address
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BT_ADDRESS MMIBT_GetA2dAddress(BT_MSG_T     *msg_body_ptr)
{
    BT_ADDRESS bt_addr = {0};

    return bt_addr;
}

/*****************************************************************************/
//  Description : if there is active A2DP headset
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN	MMIBT_IsExitedActiveA2dp(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : get a2dp link connected indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpLinkConnected(BT_MSG_T     *msg_body_ptr)
{
}

/*****************************************************************************/
//  Description : get a2dp link disconnected indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpLinkDisconnect(BT_MSG_T     *msg_body_ptr)
{

}

/*****************************************************************************/
//  Description : a2dp avrcp play music back word
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpBackword(void)
{

}

/*****************************************************************************/
//  Discription: Get the status of current a2dp
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetCurrentA2dpStatus(BT_ADDRESS	*addr_ptr)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : a2dp avrcp play music forward
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpForward(void)
{
}

/*****************************************************************************/
//  Description : a2dp avrcp  play music adjust volume 
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpAdjustVolum(BOOLEAN volume_up)
{
}

/*****************************************************************************/
//  Description : headset avrcp of a2dp stop play music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpPlay(void)
{
}

/*****************************************************************************/
//  Description : a2dp stop play music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpStop(void)
{
}

/*****************************************************************************/
//  Description : headset avrcp of a2dp stop pause music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpPause(void)
{
}

/*****************************************************************************/
//  Description : 耳机端发起建立链路的消息
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpStartStreamInd(BT_MSG_T* msg_body )
{
}


/*****************************************************************************/
//  Description :耳机端主动发起pause stream的消息
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void		MMIBT_A2dpPauseStreamInd(BT_MSG_T* msg_body )
{
}

/*****************************************************************************/
//  Description :耳机端主动发起resume stream ind
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpResumeStreamInd(BT_MSG_T* msg_body )
{
}

/*****************************************************************************/
//  Description : open a2dp headset stereo
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenA2dpHeadset(BT_ADDRESS address)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : disconnect a a2dp device by address
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_CloseA2dpHeadset(void)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : get the sample rate of a2dp stream
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetA2DPSampleRate(void)
{
    return 0;
}

/*****************************************************************************/
//  Description :get a2dp abort indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIBT_A2dpGetAbortInd(void)
{
}

/*****************************************************************************/
//  Description:    Get A2DP actived device information
//  Author:         
//  Note:
//  Parameter:      has_active_device: used to tell app that whether there is a 
//					actived device
//					TRUE:  has a actived a2dp device
//					FALSE: no actived device
//  Returns:        return the address of actived device, if has_active_device
//					is FALSE, app should ignore this return parameter. 
//****************************************************************************/
PUBLIC BT_ADDRESS MMIBT_GetActiveA2DPDevice(BOOLEAN* has_active_device)
{
    BT_ADDRESS bt_addr = {0};

    return bt_addr;
}

/*****************************************************************************/
//  Description : the error handle function of a2dp
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2DPErrorHandle(void)
{
}

#endif
