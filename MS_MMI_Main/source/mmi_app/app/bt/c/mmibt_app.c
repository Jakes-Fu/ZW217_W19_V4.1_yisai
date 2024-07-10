/****************************************************************************
** File Name:      mmibt_app.c                                              *
** Author:         baokun.yin                                               *
** Date:           03/05/2009                                               *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file handle bt headset play music action proecess   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME             DESCRIPTI                             *
** 03/05/2009       baokun.yin       Create                                *
**                                                                         *
****************************************************************************/
#define _MMIBT_APP_C_
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmibt_app.h"
#include "mmipub.h"
#include "mmibt_a2dp.h"
#include "mmi_appmsg.h"
#include "mmibt_func.h"
#include "mmimp3_export.h"
#include "mmimp3_earphone.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmi_default.h"
#include "mmibt_export.h"
#include "mmisrvaud_api.h"
#include "mmibt_devicelist.h"
#include "mmibt_devicedetails.h"
#include "mmibt_filetransfer.h"
#include "mmibt_inquireddevice.h"
#include "mmibt_savelocation.h"
#include "mmibt_deviceservice.h"
#include "mmibt_setvisibility.h"
#include "mmibt_uitest.h"
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmisrv_handlelist.h"
#include "mmicl_export.h"
#include "mmiidle_func.h"
#include "mmiudisk_export.h"
#include "mmiidle_statusbar.h"
#include "mmibt_headset.h"
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#include "autotest_device.h"
#include "mmiidle_func.h"
#include "mmicc_id.h"
#include "ual_bt.h"

#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif

#ifdef ADULT_WATCH_SUPPORT
#include "adultwatch_bt_win.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIBT_FTP_UDISK_ROOT            s_bt_ftd_udisk_folder//"\x00\x44\x00\x3A\x00\x00"
#define MMIBT_FTP_SD_CARD_ROOT          s_bt_ftd_sdcard_folder//"\x00\x45\x00\x3A\x00\x00"
#define MMIBT_FTP_UDISK_DIR_LEN          2
#define MMIBT_FTP_SD_CARD_DIR_LEN          2
#ifdef JAVA_SUPPORT
#define MMIBT_DIR_JAVA              s_bt_java_folder//"\x00\x4A\x00\x41\x00\x56\x00\x41\x00\x2F\x00\x4C\x00\x6F\x00\x63\x00\x61\x00\x6C\x00\x49\x00\x6E\x00\x73\x00\x74\x00\x61\x00\x6C\x00\x6C\x00\x00"  //"JAVA/LocalInstall"
#define MMIBT_DIR_JAVA_LEN          17
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/*************************************************************** OpenDevice ****************CloseDevice********ResumeDevice****************PauseDevice*****/
const MMIBT_DeviceFunc s_a2dp_func_table[MMIBT_FUNCTION_NUM] = {MMIBT_A2dpOpenDevice, MMIBT_A2dpStopDevice, MMIBT_A2dpResumeDevice,MMIBT_A2dpPause};
const MMIBT_DeviceFunc s_hfg_func_table[MMIBT_FUNCTION_NUM]  = {MMIBT_HFGOpenDevice, MMIBT_HFGStopDevice, MMIBT_HFGResumeDevice, MMIBT_HFGPauseDevice};/*lint !e64*/
const MMIBT_DeviceFunc *s_process_func_ptr = PNULL;

const wchar s_bt_ftd_udisk_folder[] = {0x44, 0x3A, 0x00};
const wchar s_bt_ftd_sdcard_folder[] = {0x45, 0x3A, 0x00};
#ifdef JAVA_SUPPORT
const wchar s_bt_java_folder[] = {0x4A, 0x41, 0x56,0x41,0x2F, 0x4C, 0x6F, 0x63, 
                                   0x61, 0x6C, 0x49, 0x6E, 0x73, 0x74, 0x61, 0x6C, 
                                   0x6C, 0x00 };
#endif

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
MMIBT_HEADSET_STATUS_E s_device_status = MMIBT_HEADSET_NO_STATUS;
static  uint32    s_current_service = 0;
MMIBT_OPERATION_INFO_T s_bt_opertor = {0};/*lint !e64*/
MMIBT_OPERTATION_TYPE_E g_interrupt_opertor = MMIBT_OPC_INIT;
LOCAL MMIA2DP_ACT_QUEUE_T        s_action_queue = {0};/*lint !e64*/
LOCAL BOOLEAN                    s_is_a2dp_busy = FALSE; //TRUE: a2dp pending , not recieved call back; FALSE : a2dp finished action
LOCAL MMIBT_A2DP_APP_INFO_T      s_a2dp_app_info = {0};/*lint !e64*/
LOCAL MMIBT_HEADSET_TYPE         s_specified_headset = 0;//if 0, use default headset
LOCAL SCI_MUTEX_PTR              s_opendevice_mutex_p = PNULL;
LOCAL SCI_MUTEX_PTR              s_sub_mutex_p = PNULL;
#ifdef BLUETOOTH_SUPPORT
static  BOOLEAN             s_bt_suspend[MMIBT_SUSPEND_MAX] = {0};  //Record BT Supend MP3 type // @baokun fix cr84558                                                          
#endif

LOCAL DATASRV_BT_INSTANCE_T   s_btsrv_instance = {0};

#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
LOCAL BOOLEAN s_is_fts_connect = FALSE;
#endif
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
#ifndef WIN32
extern void OS_Pfree(void *ptr);
#endif
#ifndef WIN32
extern boolean bt_ual_msg_call_back(void *p_param);
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : error process function
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleError(void);
/*****************************************************************************/
//  Description : Is Same Dev Addr
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BTIsSameDevAddr(BT_ADDRESS* des, BT_ADDRESS* src);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef BLUETOOTH_SUPPORT

/*****************************************************************************/
//  Description : 用户指定播放耳机类型，如果该类型不支持则不能播放
//  Global resource dependence : none
//  Paramter: headset_type:用户指定的耳机类型
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SpecifyHeadset(MMIBT_HEADSET_TYPE headset_type)
{
	//SCI_TRACE_LOW:"[MMIBT] MMIAPIBT_SpecifyHeadset headset_type %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_71_112_2_18_2_7_44_17,(uint8*)"d", headset_type);
	s_specified_headset = headset_type;
      MMIAPIBT_GetActiveBTHeadset();
}

/*****************************************************************************/
//  Description : Get Active Device to play music
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: first check if active A2DP device exist, only not active A2DP device exist, 
//        then check if exist HFG device
//  Return: TRUE: Exist active device to play; FALSE : No device to play
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetActiveBTHeadset(void)
{
    BOOLEAN result = FALSE;
    BT_ADDRESS      address = {0};

	switch(s_specified_headset)
	{
	case MMIBT_HFG_HEADSET:
//#ifdef BLUETOOTH_HFG_PLAY_MUSIC_SUPPORT
		if(BT_GetActivatedHS(&address))
		{
			s_process_func_ptr = s_hfg_func_table;
			result = TRUE;
		}
//#endif
		break;
	case MMIBT_A2DP_HEADSET:
		if(MMIBT_IsExitedActiveA2dp())
		{
			s_process_func_ptr = s_a2dp_func_table;
			result = TRUE;
		}
		break;
	default:
		if(MMIBT_IsExitedActiveA2dp())
		{
			s_process_func_ptr = s_a2dp_func_table;
			result = TRUE;
		}
//#ifdef BLUETOOTH_HFG_PLAY_MUSIC_SUPPORT
		else if(BT_GetActivatedHS(&address))
		{
			s_process_func_ptr = s_hfg_func_table;
			result = TRUE;
		}
//#endif
		break;
	}

    //SCI_TRACE_LOW:"[MMIBT]MMIAPIBT_GetActiveBTHeadset result %d s_specified_headset %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_121_112_2_18_2_7_45_18,(uint8*)"dd",result, s_specified_headset);
    return result;
}

/*****************************************************************************/
//  Description : get active device type
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_HEADSET_TYPE MMIAPIBT_GetActiveHeadsetType(void)
{
    BT_ADDRESS      address = {0};
    MMIBT_HEADSET_TYPE type = MMIBT_DEFAULT_HEADSET;

	if(MMIBT_DEFAULT_HEADSET != s_specified_headset)
	{
		return s_specified_headset;
	}
	else
	{
		if(MMIBT_IsExitedActiveA2dp())
		{
			type = MMIBT_A2DP_HEADSET;
		}
//#ifdef BLUETOOTH_HFG_PLAY_MUSIC_SUPPORT
		else if(BT_GetActivatedHS(&address))
		{
			type = MMIBT_HFG_HEADSET;
		}
//#endif
	}
    //SCI_TRACE_LOW:"[MMIBT] MMIAPIBT_GetActiveHeadsetType type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_151_112_2_18_2_7_45_19,(uint8*)"d", type);
    return type;
}


/*****************************************************************************/
//  Description : MMIBT_HandleAppOperation.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_TYPE_E opr_type)
{
    if(opr_type < MMIBT_A2DP_OPR_FUNC_TYPE_MAX)
    {
        if(PNULL != s_a2dp_app_info.opr_func[opr_type])
        {
            s_a2dp_app_info.opr_func[opr_type]();
        }
    }
}

/*****************************************************************************/
//  Description : Get application type.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC  MMIBT_A2DP_APP_TYPE_E  MMIBT_GetA2dpAppType(
                                    void
                                    )
{
    return s_a2dp_app_info.app_type;
}

/*****************************************************************************/
//  Description : pause device 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_PauseDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    BOOLEAN               action_result = TRUE;
    //MMIBT_FUNCTION_TYPE_E act_init = {0}; 
    
    //SCI_TRACE_LOW:"[MMIBT]: MMIBT_PauseDevice a2dp_app_type = %d, s_a2dp_app_info.app_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_192_112_2_18_2_7_45_20,(uint8*)"dd",a2dp_app_type,s_a2dp_app_info.app_type);
    if(a2dp_app_type != s_a2dp_app_info.app_type || (PNULL ==s_process_func_ptr))
    {
        return FALSE;
    }
    if(MMIBT_IsActionQueueEmpty() && (!MMIBT_DeviceIsBusy()))
    {
        //can do this action directly
		SCI_GetMutex(s_opendevice_mutex_p, SCI_WAIT_FOREVER);
        action_result = s_process_func_ptr[MMIBT_PAUSE_DEVICE](0);
        if(action_result)
        {
            MMIBT_SetBtDeviceBusy(TRUE);
            MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_PAUSE);
        }
		SCI_PutMutex(s_opendevice_mutex_p);
    }
    else 
    {
        if(!MMIBT_IsActionQueueEmpty() && (!MMIBT_DeviceIsBusy()))
        {
            //not busy, and has action need to da
            MMIBT_HandleAction(MMIBT_Out_ActionQueue());            
        }
        MMIBT_In_ActionQueue(MMIBT_PAUSE_DEVICE);
    }

    return action_result;
}

/*****************************************************************************/
//  Description : MMIBT_SetAppInfo
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetAppInfo(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    )
{
    s_a2dp_app_info = a2dp_app_info;
}

/*****************************************************************************/
//  Description : MMIBT_SetAppInfo
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ClearAppInfo(
                    MMIBT_A2DP_APP_TYPE_E   app_type
                    )
{
    if(app_type == s_a2dp_app_info.app_type)
    {
        SCI_MEMSET(&s_a2dp_app_info, 0x00, sizeof(MMIBT_A2DP_APP_INFO_T));
    }
}

/*****************************************************************************/
//  Description : open device 
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenDevice(
                    uint32 sample_ret
                    )
{
#ifdef WIN32
    return TRUE;
#else
    BOOLEAN result = TRUE;

    result = s_process_func_ptr[MMIBT_OPEN_DEVICE](sample_ret);
    
 
    //SCI_TRACE_LOW:"MMIBT_OpenDevice result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_263_112_2_18_2_7_45_21,(uint8*)"d", result);
    return result;
#endif
}

/*****************************************************************************/
//  Description : Resume  device 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIBT_ResumeDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    BOOLEAN  action_result = FALSE;

    //SCI_TRACE_LOW:"[MMIBT]: MMIBT_ResumeDevice a2dp_app_type = %d, s_a2dp_app_info.app_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_278_112_2_18_2_7_45_22,(uint8*)"dd",a2dp_app_type,s_a2dp_app_info.app_type);
    if(a2dp_app_type != s_a2dp_app_info.app_type)
    {
        return FALSE;
    }
    if(!MMIAPIBT_GetActiveBTHeadset() || PNULL ==s_process_func_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_ResumeDevice s_process_func_ptr PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_285_112_2_18_2_7_45_23,(uint8*)"");
        return FALSE;
    }
    if(MMIBT_IsActionQueueEmpty() && (!MMIBT_DeviceIsBusy()))
    {
        //do this action right now
        //SCI_PASSERT(PNULL != s_process_func_ptr, ("MMIBT_ResumeDevice s_process_func_ptr == PNULL"));
		SCI_GetMutex(s_opendevice_mutex_p, SCI_WAIT_FOREVER);
        action_result = s_process_func_ptr[MMIBT_RESUME_DEVICE](0);
        if(action_result)
        {
            MMIBT_SetBtDeviceBusy(TRUE);
            MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_RESUME);
        } 
		SCI_PutMutex(s_opendevice_mutex_p);
    }
    else 
    {
        if(!MMIBT_IsActionQueueEmpty() && (!MMIBT_DeviceIsBusy()))
        {
            //not busy, and has action need to do
            MMIBT_HandleAction(MMIBT_Out_ActionQueue());
        }
        //current action enter to queue
        MMIBT_In_ActionQueue(MMIBT_RESUME_DEVICE);
    }
    //SCI_TRACE_LOW:"MMIBT_ResumeDevice action_result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_311_112_2_18_2_7_45_24,(uint8*)"d", action_result);
    return action_result;
}

/*****************************************************************************/
//  Description : stop a2dp device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_StopDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    BOOLEAN               action_result = FALSE;
    
    //SCI_TRACE_LOW:"[MMIBT]: MMIBT_StopDevice"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_325_112_2_18_2_7_45_25,(uint8*)"");

    if(PNULL ==s_process_func_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_StopDevice s_process_func_ptr PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_329_112_2_18_2_7_45_26,(uint8*)"");
        return FALSE;
    }
     action_result = s_process_func_ptr[MMIBT_CLOSE_DEVICE](0);

    //SCI_TRACE_LOW:"[MMIBT]MMIBT_StopDevice result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_334_112_2_18_2_7_45_27,(uint8*)"d", action_result);
    return action_result;
}

/*****************************************************************************/
//  Description : stop a2dp device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetAudioSampleRate(
                    void
                    )
{
    return s_a2dp_app_info.sample_rate;
}

/*****************************************************************************/
//  Description : get headset status
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_HEADSET_STATUS_E MMIBT_GetDeviceStatus(void)
{
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_GetDeviceStatus s_device_status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_355_112_2_18_2_7_45_28,(uint8*)"d", s_device_status);
    return s_device_status;
}

/*****************************************************************************/
//  Description : set headset status
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetDeviceStatus( MMIBT_HEADSET_STATUS_E audio_status)
{
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_SetDeviceStatus audio_status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_365_112_2_18_2_7_45_29,(uint8*)"d", audio_status);
    s_device_status = audio_status;
}

/*****************************************************************************/
//  Description : open device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_OpenDeviceCnf(BT_MSG_T *msg_body)
{
	
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_OpenDeviceCnf:  status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_377_112_2_18_2_7_45_30,(uint8*)"d",msg_body->status);
    
    if (BT_SUCCESS == msg_body->status)
    {
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_OPENED);
    }
    else
    {	
        //SCI_TRACE_LOW:"[MMIBT]MMIBT_OpenDeviceCnf: error!!!!!!  "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_385_112_2_18_2_7_45_31,(uint8*)"");
    }
 }

/*****************************************************************************/
//  Description :resume device confirm
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ResumeDeviceCnf(BT_MSG_T  *msg_body)
{
    MMIBT_FUNCTION_TYPE_E act_init = MMIBT_OPEN_DEVICE;
    
	//MMIAPISET_StopAllVibrator();
	SCI_GetMutex(s_opendevice_mutex_p, SCI_WAIT_FOREVER);
	SCI_GetMutex(s_sub_mutex_p, SCI_WAIT_FOREVER);
    MMIBT_SetBtDeviceBusy(FALSE);
    MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_RESUME);
    act_init = MMIBT_Out_ActionQueue();
    SCI_PutMutex(s_opendevice_mutex_p);
	SCI_PutMutex(s_sub_mutex_p);
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_ResumeDeviceCnf:  status = %d,act_init = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_405_112_2_18_2_7_45_32,(uint8*)"dd",msg_body->status, act_init);
    
	if (BT_SUCCESS == msg_body->status)
	{
        if(MMIBT_FUNCTION_NUM == act_init || MMIBT_RESUME_DEVICE == act_init)
        {
            MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_RESUME);
        }
        MMIBT_HandleAction(act_init);
	}
	else
	{
		HandleError();
	}
}

/*****************************************************************************/
//  Description : pause device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PauseDeviceCnf(BT_MSG_T *msg_body)
{
    MMIBT_FUNCTION_TYPE_E    act_init = MMIBT_OPEN_DEVICE;
    
	SCI_GetMutex(s_opendevice_mutex_p, SCI_WAIT_FOREVER);
	SCI_GetMutex(s_sub_mutex_p, SCI_WAIT_FOREVER);
	//SCI_TRACE_LOW:"[MMIBT]MMIBT_PauseDeviceCnf:  pause cnf status is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_431_112_2_18_2_7_45_33,(uint8*)"d",msg_body->status);
    MMIBT_SetBtDeviceBusy(FALSE);
    MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_PAUSE);     
    act_init = MMIBT_Out_ActionQueue();
	SCI_PutMutex(s_opendevice_mutex_p);
	SCI_PutMutex(s_sub_mutex_p);
	if (BT_SUCCESS == msg_body->status)
	{
	    //Read Action queue, if not empty, do next action
        MMIBT_HandleAction(act_init);
	}
	else
	{
		HandleError();
		//SCI_TRACE_LOW:"[MMIBT]MMIBT_PauseDeviceCnf: pause audio fail,close audio service"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_446_112_2_18_2_7_45_34,(uint8*)"");
	}
}

/*****************************************************************************/
//  Description : close device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_CloseDeviceCnf(BT_MSG_T     *msg_body)
{
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_CloseDeviceCnf:  status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_456_112_2_18_2_7_45_35,(uint8*)"d",msg_body->status);

    if (BT_SUCCESS == msg_body->status)
    {		        
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_CLOSED);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIBT]MMIBT_CloseDeviceCnf:  error!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_464_112_2_18_2_7_45_36,(uint8*)"");
    }        
}

/*****************************************************************************/
//  Description : error process function
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleError(void)
{
	//xingdong_bt
    MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_DISCONNECT);
    
    MMIBT_StopDevice(MMIBT_GetA2dpAppType());
    //恢复蓝牙功能已经状态
//     MMIBT_RecoveryBtStatus(MMIBT_MODULE_A2DP);	
    MMIBT_SetDeviceStatus(MMIBT_HEADSET_NO_STATUS);
}

/*****************************************************************************/
//  Description :  open HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGOpenDevice(uint32 sample_ret)
{
    BOOLEAN result = FALSE;

    BT_HFGOpenDevice();
    if(BT_PENDING == BT_HFGPlay())
    {
        result = TRUE;
        //MMIBT_HiddenBtForAPP(MMIBT_MODULE_A2DP,BT_PAGE_ENABLE);
    } 

    //SCI_TRACE_LOW:"[MMIBT]HFG_OpenDevice result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_498_112_2_18_2_7_45_37,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description :  stop HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGStopDevice( uint32 sample_ret)
{
    BOOLEAN result = FALSE;

    if(BT_PENDING == BT_HFGStop())
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIBT]HFG_StopDevice result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_515_112_2_18_2_7_45_38,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description :  Resume HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGResumeDevice( uint32 sample_ret)
{
    BOOLEAN result = FALSE;
    BT_HFGOpenDevice();
    if(BT_PENDING == BT_HFGPlay())
    {        
        result = TRUE;
        //MMIBT_HiddenBtForAPP(MMIBT_MODULE_A2DP,BT_PAGE_ENABLE);
    }

    //SCI_TRACE_LOW:"[MMIBT]HFG_ResumeDevice result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_535_112_2_18_2_7_46_39,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description :  stop HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGPauseDevice(void)
{
    BOOLEAN result = FALSE;
    

        if(BT_PENDING == BT_HFGStop())
        {
            result = TRUE;
        }

    //SCI_TRACE_LOW:"[MMIBT]HFG_PauseDevice result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_553_112_2_18_2_7_46_40,(uint8*)"d", result);
    return result;    
}

/*****************************************************************************/
//  Description : Get Active Headset Device Name
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetActiveHeadsetName(
                    wchar   *headset_name_ptr,
                    uint16  *name_len_ptr
                    )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_a2dp_exist = FALSE;
    BT_ADDRESS  bt_address = {0};

    if (PNULL == headset_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIBT_GetActiveHeadsetName PNULL == headset_name_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_572_112_2_18_2_7_46_41,(uint8*)"");
        return FALSE;
    }
	switch(s_specified_headset)
	{
	case MMIBT_HFG_HEADSET:
//#ifdef BLUETOOTH_HFG_PLAY_MUSIC_SUPPORT
		if(BT_GetActivatedHS(&bt_address))
		{
			result = TRUE;
		}
//#endif
		break;
	case MMIBT_A2DP_HEADSET:
		if(MMIBT_IsExitedActiveA2dp())
		{
            bt_address = MMIBT_GetActiveA2DPDevice(&is_a2dp_exist);
			result = TRUE;
		}
		break;
	default:
		if(MMIBT_IsExitedActiveA2dp())
		{
			bt_address = MMIBT_GetActiveA2DPDevice(&is_a2dp_exist);
			result = TRUE;
		}
//#ifdef BLUETOOTH_HFG_PLAY_MUSIC_SUPPORT
		else if(BT_GetActivatedHS(&bt_address))
		{
			result = TRUE;
		}
//#endif
		break;
	}
	
    if(result)
    {
        BT_GetPairedDeviceName(&bt_address, (uint16*)headset_name_ptr);
        *name_len_ptr = MMIAPICOM_Wstrlen(headset_name_ptr);
        if(*name_len_ptr == 0)
        {
            result = FALSE;
        }
    }
    //SCI_TRACE_LOW:"[MMIBT] MMIAPIBT_GetActiveHeadsetName result = %d, *name_len_ptr = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_616_112_2_18_2_7_46_42,(uint8*)"dd",result, *name_len_ptr);
    return result;
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : Get Connected Device Name
//  Global resource dependence : none
//  Author: cheney wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetConnectedDeviceName(
                                               wchar   *headset_name_ptr,
                                               uint16  *name_len_ptr
                                               )
{
    BOOLEAN     result = FALSE;
    BT_ADDRESS  bt_address = {0};
	
    if (PNULL == headset_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIBT_GetActiveHeadsetName PNULL == headset_name_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_572_112_2_18_2_7_46_41,(uint8*)"");
        return FALSE;
    }
	
    if(MMIBT_IsConnectDevice(&bt_address))
    {
        result = TRUE;
    }
	
    if(TRUE == result)
    {
        BT_GetPairedDeviceName(&bt_address, (uint16*)headset_name_ptr);
        *name_len_ptr = MMIAPICOM_Wstrlen(headset_name_ptr);
        if(*name_len_ptr == 0)
        {
            result = FALSE;
        }
    }
	
    return result;
    
}
#endif

#else//
/*****************************************************************************/
//  Description : 用户指定播放耳机类型，如果该类型不支持则不能播放
//  Global resource dependence : none
//  Paramter: headset_type:用户指定的耳机类型
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SpecifyHeadset(MMIBT_HEADSET_TYPE headset_type)
{
    return;
}
/*****************************************************************************/
//  Description : Get Active Device to play music
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: first check if active A2DP device exist, only not active A2DP device exist, 
//        then check if exist HFG device
//  Return: TRUE: Exist active device to play; FALSE : No device to play
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetActiveBTHeadset(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : get active device type
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_HEADSET_TYPE MMIAPIBT_GetActiveHeadsetType(void)
{
    return MMIBT_DEFAULT_HEADSET;
}

/*****************************************************************************/
//  Description : pause device 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_PauseDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : MMIBT_SetAppInfo
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetAppInfo(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    )
{
    return;
}

/*****************************************************************************/
//  Description : MMIBT_SetAppInfo
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ClearAppInfo(
                    MMIBT_A2DP_APP_TYPE_E   app_type
                    )
{
    return;
}

/*****************************************************************************/
//  Description : open device 
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenDevice(
                    uint32 sample_rate
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Resume  device 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIBT_ResumeDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : stop a2dp device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_StopDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : stop a2dp device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetAudioSampleRate(
                    void
                    )
{
    return s_a2dp_app_info.sample_rate;
}

/*****************************************************************************/
//  Description : get headset status
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_HEADSET_STATUS_E MMIBT_GetDeviceStatus(void)
{
    //SCI_TRACE_LOW:"MMIBT_GetDeviceStatus s_device_status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_734_112_2_18_2_7_46_43,(uint8*)"d", s_device_status);
    return s_device_status;
}

/*****************************************************************************/
//  Description : set headset status
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetDeviceStatus( MMIBT_HEADSET_STATUS_E audio_status)
{
    return;
}


/*****************************************************************************/
//  Description : open device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_OpenDeviceCnf(BT_MSG_T *msg_body)
{
    return;
}

/*****************************************************************************/
//  Description :resume device confirm
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ResumeDeviceCnf(BT_MSG_T  *msg_body)
{
    return;
}

/*****************************************************************************/
//  Description : pause device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PauseDeviceCnf(BT_MSG_T *msg_body)
{
    return;
}

/*****************************************************************************/
//  Description : close device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_CloseDeviceCnf(BT_MSG_T     *msg_body)
{
    return;
}

/*****************************************************************************/
//  Description : error process function
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleError(void)
{
	//xingdong_bt
    //MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_DISCONNECT);
    
    MMIBT_StopDevice(MMIBT_GetA2dpAppType());
    MMIBT_SetDeviceStatus(MMIBT_HEADSET_NO_STATUS);
}

/*****************************************************************************/
//  Description :  open HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGOpenDevice(uint32 sample_ret)
{
    return FALSE;
}

/*****************************************************************************/
//  Description :  stop HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGStopDevice( uint32 sample_ret)
{
    return FALSE;
}

/*****************************************************************************/
//  Description :  Resume HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGResumeDevice( uint32 sample_ret)
{
    return FALSE;
}

/*****************************************************************************/
//  Description :  stop HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGPauseDevice(void)
{
    return FALSE;    
}

/*****************************************************************************/
//  Description : MMIBT_HandleAppOperation.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_TYPE_E opr_type)
{
    return;
}

/*****************************************************************************/
//  Description : Get application type.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  MMIBT_A2DP_APP_TYPE_E  MMIBT_GetA2dpAppType(
                                    void
                                    )
{
    return MMIBT_A2DP_AUDIO_NONE;
}

#endif//not support bluetooth

/*****************************************************************************/
//  Description : init a2dp action queue 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_Init_ActionQueue(void)
{
    MMIBT_SetBtDeviceBusy(FALSE);
    SCI_MEMSET(&s_action_queue, 0x00, sizeof(MMIA2DP_ACT_QUEUE_T));
    if(PNULL != s_opendevice_mutex_p)
    {
		//SCI_PutMutex(s_opendevice_mutex_p);
        SCI_DeleteMutex(s_opendevice_mutex_p);
        s_opendevice_mutex_p = PNULL;
    }
    s_opendevice_mutex_p = SCI_CreateMutex("mmi_bt_opendevice", SCI_INHERIT);

	if(PNULL != s_sub_mutex_p)
    {
		//SCI_PutMutex(s_sub_mutex_p);
        SCI_DeleteMutex(s_sub_mutex_p);
        s_sub_mutex_p = PNULL;
    }
	
    s_sub_mutex_p = SCI_CreateMutex("s_sub_mutex_p", SCI_INHERIT);	
}

/*****************************************************************************/
//  Description : Remove action  from queue 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_RemoveAction_FromQueue(MMIBT_FUNCTION_TYPE_E act)
{
	uint8 i = 0;
	uint8 j = 0;
	
	for(i = 0; i < s_action_queue.action_num; i++)
	{
		//SCI_TRACE_LOW:"s_action_queue.action[%d] %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_893_112_2_18_2_7_46_45,(uint8*)"dd", i,s_action_queue.action[i]);
		
		if(act | s_action_queue.action[i])/*lint !e655*/
		{
			//remove this action
			for(j = i; j < s_action_queue.action_num; j++)
			{
				s_action_queue.action[j] = s_action_queue.action[j+1];
			}
			s_action_queue.action_num--;
			break;
		}
	}
	//SCI_TRACE_LOW:"MMIBT_RemoveAction_FromQueue s_action_queue.action_num %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_906_112_2_18_2_7_46_46,(uint8*)"d", s_action_queue.action_num);
}

/*****************************************************************************/
//  Description :  a2dp action insert to action queue 
//  Global resource dependence : none
//  Author: 
//  Note: if queue full, return false, if act is stop, clean queue's actions and insert only this action
//        if act is pause, need clean the play and resume action
//        if action is play or resume, need clean the pause action
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_In_ActionQueue(MMIBT_FUNCTION_TYPE_E act)
{
     BOOLEAN ret = FALSE;

     if(MMIBT_ACT_QUEUE_MAX_LEN < s_action_queue.action_num)
     {
         //queue full, can not response actions
         //SCI_TRACE_LOW:"MMIBT_In_ActionQueue queue full  %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_922_112_2_18_2_7_46_47,(uint8*)"d", s_action_queue.action_num);
         return FALSE;
     }
     else
     {
         //Insert it to queue
         switch (act)
         {
         case MMIBT_OPEN_DEVICE:
             s_action_queue.action_num = 0;
             break;
         case MMIBT_RESUME_DEVICE:
             //remove_act = MMIBT_RESUME_DEVICE | MMIBT_CLOSE_DEVICE | MMIBT_PAUSE_DEVICE;
             MMIBT_RemoveAction_FromQueue(MMIBT_RESUME_DEVICE);
             MMIBT_RemoveAction_FromQueue(MMIBT_CLOSE_DEVICE);
             MMIBT_RemoveAction_FromQueue(MMIBT_PAUSE_DEVICE);
             break;
         case MMIBT_CLOSE_DEVICE:
             //remove_act = MMIBT_OPEN_DEVICE | MMIBT_RESUME_DEVICE | MMIBT_CLOSE_DEVICE | MMIBT_PAUSE_DEVICE;
             s_action_queue.action_num = 0;
             break;
         case MMIBT_PAUSE_DEVICE:
             MMIBT_RemoveAction_FromQueue(MMIBT_RESUME_DEVICE);
             MMIBT_RemoveAction_FromQueue(MMIBT_CLOSE_DEVICE);
             MMIBT_RemoveAction_FromQueue(MMIBT_PAUSE_DEVICE);
             break;
         default:
             //SCI_TRACE_LOW:"MMIBT_In_ActionQueue act %d"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_949_112_2_18_2_7_46_48,(uint8*)"d", act);
             return FALSE;
         }
         //MMIBT_RemoveAction_FromQueue(remove_act);
         s_action_queue.action[s_action_queue.action_num] = act;
         s_action_queue.action_num ++;
         ret = TRUE;
     }
     //SCI_TRACE_LOW:"[MMIBT] In_ActionQueue act %d ret %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_957_112_2_18_2_7_46_49,(uint8*)"dd", act, ret);
     return ret;
}

/*****************************************************************************/
//  Description :  get action from queue head
//  Global resource dependence : none
//  Author: 
//  Note: return action in queue head, if queue empty, return BT_A2DP_NONE
/*****************************************************************************/
PUBLIC MMIBT_FUNCTION_TYPE_E MMIBT_Out_ActionQueue(void)
{
     MMIBT_FUNCTION_TYPE_E ret = MMIBT_OPEN_DEVICE;
     uint8                 i = 0;
     if(0 == s_action_queue.action_num)
     {
         //queue empty, need to do action
         ret = MMIBT_FUNCTION_NUM;
     }
     else
     {

         //get action from queue head
         ret = s_action_queue.action[0];
         for(i = 1; i < s_action_queue.action_num; i++)
         {
            s_action_queue.action[i -1] = s_action_queue.action[i];
         }
         s_action_queue.action[i] = MMIBT_FUNCTION_NUM;
         s_action_queue.action_num --;
     }
     //SCI_TRACE_LOW:"[MMIBT] Out_ActionQueue act %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_986_112_2_18_2_7_46_50,(uint8*)"d", ret);
     return ret;
}

/*****************************************************************************/
//  Description :  get is the action queue empty
//  Global resource dependence : none
//  Author: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsActionQueueEmpty(void)
{
    BOOLEAN  ret = FALSE;
    if(0 == s_action_queue.action_num)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
    //SCI_TRACE_LOW:"MMIBT_IsActionQueueEmpty ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_1004_112_2_18_2_7_47_51,(uint8*)"d", ret);
    return ret;
}

/*****************************************************************************/
//  Description :  process next action
//  Global resource dependence : none
//  Author: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HandleAction(MMIBT_FUNCTION_TYPE_E act)
{
//    BOOLEAN               result = FALSE;
// 
//     SCI_TRACE_LOW("HandleA2DPAction act %d", act);
//     if(act < MMIBT_FUNCTION_NUM && (PNULL !=s_process_func_ptr))
//     {
// 	   SCI_GetMutex(s_sub_mutex_p, SCI_WAIT_FOREVER);
//        result = s_process_func_ptr[act]();
//        if(result)
//        {
//            MMIBT_SetBtDeviceBusy(TRUE);
//            switch(act)
//             {
//             case MMIBT_OPEN_DEVICE:
//                 MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_OPEN);
//                 break;
//             case MMIBT_CLOSE_DEVICE:
//                 MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_STOP);
//                 break;
//             case MMIBT_RESUME_DEVICE:
//                 MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_RESUME);
//                 break;
//             case MMIBT_PAUSE_DEVICE:
//                 MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_PAUSE);
//                 break;
//              default:
//                 MMIBT_SetDeviceStatus(MMIBT_HEADSET_NO_STATUS);
//                 break;
//             }
//        }
// 	   SCI_PutMutex(s_sub_mutex_p);
// 	   
//     }
    return FALSE;
}

/*****************************************************************************/
//  Description : if is  a2dp device busy
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpBusy(void)
{
    return MMIBT_DeviceIsBusy();
}

/*****************************************************************************/
//  Description : set current a2dp device busy or not
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetBtDeviceBusy(BOOLEAN busy_flag)
{
    //SCI_TRACE_LOW:"MMIBT Bt_SetA2dpBusyFlag %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_1063_112_2_18_2_7_47_52,(uint8*)"d", busy_flag);
    s_is_a2dp_busy = busy_flag;
}

/*****************************************************************************/
//  Description : get current device busy or not
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsBusy(void)
{
    //SCI_TRACE_LOW:"MMIBT_DeviceIsBusy %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_APP_1073_112_2_18_2_7_47_53,(uint8*)"d", s_is_a2dp_busy);
    return s_is_a2dp_busy;
}
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendOneLineTextIconListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            MMI_IMAGE_ID_T   imgae_id,
                            uint16           pos,
                            MMI_TEXT_ID_T    left_text_id,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    

    item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[0] = left_text_id;
    item_data.softkey_id[2] = STXT_RETURN;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = imgae_id;


    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendOneLineIconTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_IMAGE_ID_T   imgae_id,
                            MMI_TEXT_ID_T    text_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = imgae_id;

    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;


    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//  Discription: append spliter
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendSplitListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;

    //item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;
    item_t.item_state |=GUIITEM_STATE_SPLIT;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
  //  GUILIST_SetItemSplit(ctrl_id, pos, TRUE);
}
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendTwoLineTextIconSmallTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id1,
                            MMI_IMAGE_ID_T   imgae_id,
                            MMI_STRING_T     str_info,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;

    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[2] = STXT_RETURN;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id1;
    item_data.item_content[2].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer = str_info;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = imgae_id;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
#endif 

/*****************************************************************************/
//  Discription: set the inquire info,call the function when begin searching device
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_SetCurrentSevice(
                            uint32	service
                            )
{
    s_current_service = service;
}
/*****************************************************************************/
//  Discription: set the inquire info,call the function when begin searching device
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetCurrentSevice(void)
{
    return s_current_service;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_SetInterruptOper(
                            uint32	interrupt_oper
                            )
{
    g_interrupt_opertor = interrupt_oper;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMIBT_OPERTATION_TYPE_E MMIBT_GetInterruptOper(void)
{
    return g_interrupt_opertor;
}
/*****************************************************************************/
//  Discription: set bt opertor
//  Global resource dependence: None
//  Author: 
//  Parameter: root_opertion : it descript the reason enter to BT (Set, Send or other Opt), 
//                             the next step will be determined  by root_opertion
//             current_opertion: it  descript current BT operation, after recived BT callback 
//                               message, it will decide what's to do. 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetOpertor(
                                MMIBT_OPERTATION_TYPE_E root_opertion, //IN:
                                MMIBT_OPERTATION_TYPE_E current_opertion//IN:
                            )
{

    if(MMIBT_OPC_NOT_CHANGE != root_opertion)
    {
        s_bt_opertor.root_opertion = root_opertion;
    }
    if(MMIBT_OPC_NOT_CHANGE != current_opertion)
    {
        s_bt_opertor.current_opertion = current_opertion;           
    }

    SCI_TRACE_LOW("[MMIBT] MMIBT_SetOpertor root_opt %d, current_opt %d", root_opertion, current_opertion);
}
/*****************************************************************************/
//  Discription: get bt opertor
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return: 
/*****************************************************************************/
PUBLIC MMIBT_OPERATION_INFO_T MMIBT_GetOpertor(void)
{
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7852_112_2_18_2_8_16_293,(uint8*)"dd",s_bt_opertor.root_opertion, s_bt_opertor.current_opertion);
    return s_bt_opertor;
}
/*****************************************************************************/
//  Discription: 根据current_opt的不同，提示不同的提示信息，让用户选择是否要进行该操作
//  Global resource dependence: None
//  Author: 
//  current_opt：  当前的操作类型，根据该值的不同，显示不同的提示信息；
//  parent_win_id：用户选择了"Yes",或者"No"，要把用户选择通过窗口的信息
//                 发送该parent_win_id指向的窗口，如果parent_win_id为0，
//                 表示需要在本窗口中处理
//  Note : 
//  Return: 
/*****************************************************************************/
PUBLIC void OpenQueryWin(   MMI_WIN_ID_T             win_id,
						   MMI_STRING_T*            display_str1,
                           MMI_STRING_T*            display_str2,
                           MMIBT_OPERTATION_TYPE_E  opc
                        )
                               
{
    uint32       alert_time_period = 0;
    MMI_WIN_ID_T query_id = win_id;
    //Turn on back light
    MMIDEFAULT_SetBackLight(TRUE);

    if(MMK_IsOpenWin(win_id))
    {
        MMK_SendMsg(query_id, MSG_APP_CANCEL,  PNULL);
        MMK_CloseWin(query_id);
    }
    //set query opc
    MMIBT_SetInterruptOper(opc);
    //Open alert window
    MMIPUB_OpenAlertWinByTextPtr(
                                &alert_time_period,
                                display_str1,
                                display_str2,
                                IMAGE_PUBWIN_QUERY,
                                &query_id,
                                PNULL,
                                MMIPUB_SOFTKEY_OKCANCEL,
                                HandleQueryWinMsg
                                );
}
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Discription: check the type of new received file is drm 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_CheckNewFileType( uint16*file_suffix_ptr)
{
    int32               i = 0;
    int32               type_num = 0;
    uint8   s_bt_drm_file_suffix[][MMIFILE_FULL_PATH_MAX_LEN] = {{".dm"},{".dcf"}};

    if( PNULL == file_suffix_ptr)
    {
		return FALSE;
    }
    
    //check if is DRM file 
    type_num = sizeof(s_bt_drm_file_suffix)/sizeof(*s_bt_drm_file_suffix);
    for (i=0; i < type_num; i++)
    {
        if(MMIAPICOM_IsSameFileType( file_suffix_ptr, (uint8*)(s_bt_drm_file_suffix[i])))
        {            
            return TRUE;
        }
    }    
    return FALSE;
}



#endif
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpsConnectReq(BT_MSG_T *msg_body_ptr)
{
    DoOperation(MMIBT_OPS_CONNECT_REQ,TRUE);    
}

/*****************************************************************************/
//  Discription: Clear the client rect  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC  void  MMIBT_ClrClientRect(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
	GUI_RECT_T          bg_rect         =   MMITHEME_GetClientRect();
	
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
	GUIRES_DisplayImg(PNULL,
		&bg_rect,
		&bg_rect,
		win_id,
		IMAGE_COMMON_BG,
		&lcd_dev_info);	
}

/*****************************************************************************/
//  Discription: Update current file list
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_UpdateCurrentFileList(void)
{
    //图片、音乐、视频文件列表选择窗口
    //SCI_TRACE_LOW:"UpdateCurrentFileList enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10081_112_2_18_2_8_21_334,(uint8*)"");
    MMIAPIFMM_CloseSelectFileWin();
    MMIAPIFMM_UpdateCurrentFileLIst();
}
/*****************************************************************************/
//  Discription: BtPauseBGPlay
//  Global resource dependence: none 
//  Author: 
//  Note: pause background play in BT by MMIBT_SUSPEND_TYPE_E
/*****************************************************************************/
PUBLIC void BtPauseBGPlay(MMIBT_SUSPEND_TYPE_E type)
{
#if defined(BLUETOOTH_SUPPORT) 
    //MMIBT_SetWaitPauseCnfFlag(TRUE);
    if(type >= MMIBT_OPP_SUSPEND && type < MMIBT_SUSPEND_MAX)
    {
       s_bt_suspend[type] = TRUE;
        //MMIAUDIO_PauseOtherBgPlay(MMIBGPLAY_TYPE_BT);
    }
    //SCI_TRACE_LOW:"[MMIBT]BtPauseBGPlay: TYPE %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8902_112_2_18_2_8_18_307,(uint8*)"d",type);
#endif
}

/*****************************************************************************/
//  Discription: BtResumeBGPlay
//  Global resource dependence: none 
//  Author: 
//  Note: resume background play in BT by MMIBT_SUSPEND_TYPE_E
//        first set s_bt_suspend[type] = FALSE
//        if s_bt_suspend[] all is FLASE, this resume permit to do
/*****************************************************************************/
PUBLIC void BtResumeBGPlay(MMIBT_SUSPEND_TYPE_E type)
{
#ifdef BLUETOOTH_SUPPORT
    BOOLEAN ret = TRUE;
    BOOLEAN exist = FALSE;
    int32   i = 0;
    
    //SCI_TRACE_LOW:"BtResumeBGPlay type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8919_112_2_18_2_8_18_308,(uint8*)"d",type);
    if(type >= MMIBT_OPP_SUSPEND && type < MMIBT_SUSPEND_MAX)
    {
        ret = s_bt_suspend[type];
        s_bt_suspend[type] = FALSE;
        
        if(ret)
        {
            //this type need resume BG play
            for(i = 0; i < MMIBT_SUSPEND_MAX; i++)
            {
                //check if other BT suspend s_bt_suspend
                if(s_bt_suspend[i])
                {
                   exist = TRUE;
                }
            }
            if(!exist)
            {
                //no exist other BT suspend, permit resume BT play
               //MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_BT);
            }
        }
     }
    //SCI_TRACE_LOW:"[MMIBT]BtResumeBGPlay: TYPE %d ret %d exist %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8943_112_2_18_2_8_18_309,(uint8*)"ddd",type, ret, exist);
#endif
}
/*****************************************************************************/
//  Discription:get type is need BT On first
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: TRUE: need BT On first
//          FALSE:no need BT On first
/*****************************************************************************/
PUBLIC BOOLEAN IsNeedBTOn(MMIBT_OPERTATION_TYPE_E type)
{
    BOOLEAN ret = TRUE;
    if(MMIBT_OPC_NOT_CHANGE == type
     || MMIBT_OPC_INIT == type
     || MMIBT_OPC_MAIN == type
     || MMIBT_OPC_POWER_ONOFF == type
     || MMIBT_OPC_POWER_ONOFF == type
	 || MMIBT_OPC_DEVICE_DELETE == type 
	 || MMIBT_OPC_DEVICE_DELETE_ALL == type
    )
    {
        //these operation need not BT On first
        ret = FALSE;
    }
    return ret;
}
/*****************************************************************************/
//  Discription: Clear the client rect  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC  void  ClosePrmAndOpenDoneWin(
                                    uint32 win_id,
                                    uint32 txt_id,
                                    uint32 img_id,
                                    uint32 tm_dur
                                    )
{
    //Open done win
    if(!MMK_IsOpenWin(MMIPUB_ALERT_WIN_ID))
    {
        MMIPUB_OpenAlertWinByTextId(&tm_dur,txt_id,TXT_NULL,img_id,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    if(MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenSearchDeviceWaitWin(
                                   uint32 service_type
                                   )
{
    OpenSearchDeviceWaitWin(service_type);
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetSelfPair(BOOLEAN  is_self_pair)
{
    SetSelfPair(is_self_pair);
} 
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC  BT_DEVICE_INFO MMIBT_GetPinRequestInfo(void)
{   
    BT_DEVICE_INFO  pin_request_info = {0};
    
    pin_request_info = GetPinRequestInfo();

    return pin_request_info;
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetPinCancel(BOOLEAN  is_pin_cancel)
{
    SetIsPinCancel(is_pin_cancel);
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsPinCancel(void)
{
    return IsPinCancel();
}

/*****************************************************************************/
//  Discription: open pin input win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CreatePinInputWin(BT_DEVICE_INFO *param_ptr)
{    
    if(PNULL != param_ptr)
    {
        OpenPinInputWin(param_ptr,InputPairedPin);        
    }
}

PUBLIC void MMIBT_DataSrvinit(void)
{
    SCI_TRACE_LOW("MMIBT_DataSrvinit enter");
    SCI_MEMSET(&s_btsrv_instance, 0, sizeof(DATASRV_BT_INSTANCE_T));

    //Create datasrv handle instance
    s_btsrv_instance.srvhandle_lst = DataSrvHandle_Create(sizeof(BTDATA_SRV_HANDLE_T),PNULL,PNULL);
}

LOCAL void MMIBT_DataSrvDestroy(void)
{
    SCI_TRACE_LOW("MMIBT_DataSrvDestroy enter");
    DataSrvHandle_Destroy(&s_btsrv_instance.srvhandle_lst);

    SCI_MEMSET(&s_btsrv_instance, 0, sizeof(DATASRV_BT_INSTANCE_T));
}
/*****************************************************************************/
//  Discription: mmibt search device   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_StartOperationWin(MMI_HANDLE_T win_handle,
                                            MMIBT_OPERTATION_TYPE_E operation_type, BTFUNCWINCALLBACK callback)
{
    BT_STATUS res = BT_SUCCESS;
    BT_HANDLE_T  new_handle = PNULL;

    if(PNULL == s_btsrv_instance.srvhandle_lst)
    {
        MMIBT_DataSrvinit();
    }
    new_handle = (MMI_HANDLE_T *)DataSrvHandle_NewHandle(s_btsrv_instance.srvhandle_lst);

    if(new_handle == PNULL)
    {
        return BT_ERROR;
    }
    
    SCI_TRACE_LOW("MMIBT_StartOperationWin operation_type=%d",operation_type);
    
    s_btsrv_instance.cur_handle = new_handle;     
    new_handle->win_handle = win_handle;
    new_handle->call_back = callback;
    new_handle->user_data = (void*)operation_type;

    return res;
}
/*****************************************************************************/
//  Discription: get current bt operation   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
LOCAL int CmpHandle(BT_HANDLE_T srv_handle,MMIBT_OPERTATION_TYPE_E* cmp_data)
{
    if (srv_handle== PNULL || cmp_data == PNULL)
    {
        return -1;      //Error
    }

    SCI_TRACE_LOW("CmpHandle bt srv_handle->user_data=%d,cmp_data=%d",srv_handle->user_data,*cmp_data);

    if ((MMIBT_OPERTATION_TYPE_E)srv_handle->user_data == *cmp_data)/*lint !e549*/
    {
        return 0;       //Equal
    }

    return 1;           //Different
}
/*****************************************************************************/
//  Discription: mmibt search device cnf  
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_StopOperationWin(MMIBT_OPERTATION_TYPE_E operation_type)
{
    BT_HANDLE_T cmd_handle = PNULL;
    MMIBT_OPERTATION_TYPE_E oper_type = operation_type;
    
    SCI_TRACE_LOW("MMIBT_StopOperationWin oper_type=%d",oper_type);
    
    cmd_handle = DataSrvHandle_FindHandle(s_btsrv_instance.srvhandle_lst,
                &oper_type,
                CmpHandle
                );
        
    if (cmd_handle == PNULL)
    {
        return;
    }
    if(cmd_handle->call_back(cmd_handle->win_handle,PNULL,PNULL))
    {
        DataSrvHandle_FreeHandle(s_btsrv_instance.srvhandle_lst,cmd_handle);
    }
}
/*****************************************************************************/
//  Discription:  check is disk space enough
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DiskEnoughSpaceForOverWrite(wchar *file_name,
										  uint32 dest_size, 
										  MMIFILE_DEVICE_E devie)
{
	BOOLEAN is_enough = FALSE;
	uint32  file_size = 0;
	
	MMIAPIFMM_GetFileInfo(file_name, MMIAPICOM_Wstrlen(file_name), &file_size, PNULL, PNULL);
	if(file_size >= dest_size || MMIAPIFMM_IsDevEnoughSpace((dest_size - file_size), devie))
	{
		is_enough = TRUE;
	}

	return is_enough;
}
/*****************************************************************************/
//  Discription: save ftp folder to nv
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_UpdateFtpSharedFolder(BT_MSG_T* msg_body_ptr)
{
    wchar  *root_folder_ptr = (wchar *)msg_body_ptr->body_ptr;
	uint16 folder_len = 0;
    MMIBT_USER_SETTING_U *setting_ptr = PNULL;

    if (PNULL == root_folder_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_UpdateFtpSharedFolder PNULL == root_folder_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_1606_112_2_18_2_8_2_238,(uint8*)"");
        return;
    }
    setting_ptr = (MMIBT_USER_SETTING_U *)SCI_ALLOC_APP(sizeof(MMIBT_USER_SETTING_U));
	if(setting_ptr != PNULL)
	{
		folder_len = MMIAPICOM_Wstrlen(root_folder_ptr);
		SCI_MEMCPY(setting_ptr->dir.shaired_path_name, root_folder_ptr, (sizeof(wchar)*folder_len));
		MMINV_WRITE(MMINV_BT_SHAIRED_DIRECTORY,setting_ptr);
		SCI_Free(setting_ptr);
	}
}
/*****************************************************************************/
//  Discription: get the device is active earphone by address of addr_ptr
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsDeviceActivedEarphone(
                                             const BT_ADDRESS    *addr_ptr
                                              )
{
    BT_STATUS       return_value = BT_SUCCESS;
    BOOLEAN         res = FALSE;
    
    if (PNULL == addr_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_IsDeviceActivedEarphone PNULL == addr_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_1633_112_2_18_2_8_2_239,(uint8*)"");
        return res;
    }
    return_value = BT_GetConnStatus(addr_ptr,BT_SERVICE_HANDFREE|BT_SERVICE_HANDSET);/*lint !e605 */    
    //SCI_TRACE_LOW:" MMIBT_IsDeviceActivedEarphone: return value is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_1638_112_2_18_2_8_2_240,(uint8*)"d",return_value);
    
    if(BT_SUCCESS == return_value)
    {
        res = TRUE;
    }
    return  res;
}
#ifdef JAVA_SUPPORT
/*****************************************************************************/
//  Discription: check the java directory whether exist
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_CheckJavaDirExist(void)
{
    wchar               *file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    wchar               full_path_arr[MMIBT_FULL_PATH_LEN +1] = {0};
    uint16              full_path_len = MMIBT_FULL_PATH_LEN;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/

    MMIBT_GetFileUserSetting(MMINV_BT_FILE_LOCATION, &setting);

    file_dev_ptr = MMIAPIFMM_GetDevicePath(setting.filedev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(setting.filedev);

    if((PNULL != file_dev_ptr) && (0 != file_dev_len))
    {
        if(MMIAPIFMM_CombineFullPath((wchar *)file_dev_ptr, file_dev_len,
                                (wchar *)MMIBT_DIR_JAVA, (MMIBT_DIR_JAVA_LEN), 
                                NULL, 0, 
                                (wchar *)full_path_arr, &full_path_len))
        {
            // check the exist of java directory
            if(MMIAPIFMM_IsFolderExist((wchar *)full_path_arr, full_path_len))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}
#endif
/****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_DIAL_NUMBER
//  make a call from HFAG
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_HfagDiaNumber(BT_MSG_T* msg_body_ptr)
{
    BT_DIAL_NUMBER      dial_number = *((BT_DIAL_NUMBER *)msg_body_ptr->body_ptr);      
    uint8               num_len = 0;

    #ifndef BT_BQB_SUPPORT
    num_len = strlen((char*)dial_number.phone_number);
    if ( num_len > 0 )
    {
        MMIAPICC_MakeCall(
            MN_DUAL_SYS_MAX,
            dial_number.phone_number,
            num_len,
            PNULL,
            PNULL,
            CC_CALL_SIM_MAX,
            CC_CALL_NORMAL_CALL,
            PNULL
            );
    }
    #else
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
    
    num_len = strlen((char*)dial_number.phone_number);
    if ( num_len > 0 )
    {
        cc_result = MMIAPICC_MakeCall(
            MN_DUAL_SYS_MAX,
            dial_number.phone_number,
            num_len,
            PNULL,
            PNULL,
            CC_CALL_SIM_MAX,
            CC_CALL_NORMAL_CALL,
            PNULL
            );
        if(CC_RESULT_SUCCESS == cc_result)
        {
            MMIAPIBT_SetCallSuccess();
        }
        else
        {
            MMIAPIBT_SetCallFail();
        }
    }
    else
    {
        MMIAPIBT_SetCallFail();
    }
    #endif  //BT_BQB_SUPPORT
}
/*****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_REDIAL
//  make a call from HFAG in the call record
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_HfagRedailCall(void)
{
    MMIAPICL_MakeCallByRecordIndex(MMICL_CALL_DIALED,0);
}
/*****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_DIAL_MEMORY
//  make a call from HFAG in the call record
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_HfagDiaMemory(BT_MSG_T* msg_body_ptr)
{
    BT_DIAL_MEMORY      memory_info = *((BT_DIAL_MEMORY *) msg_body_ptr->body_ptr);
	
	#ifndef BT_BQB_SUPPORT
    //底层传的参数是index,可以已拨电话中根据index呼出电话
    MMIAPICL_MakeCallByRecordIndex(MMICL_CALL_MISSED,(uint16)memory_info.memory_indicator);
    #else
    //+++ TC_AG_OCM_BV_01_I
    uint32 index = 0;
    uint8 strbuf[MMIPB_MAX_STRING_LEN +1] = {0};
    uint16 len = 0;
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
    
    if(memory_info.memory_indicator > 0)
        index = memory_info.memory_indicator - 1;
    //SCI_TRACE_LOW:"MMIBT_HfagDiaMemory index=%d, memory_indicat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6549_112_2_18_2_8_13_277,(uint8*)"dd", index, memory_info.memory_indicator);


    if(MMIAPIPB_GetNumberByIndex(index, &strbuf[0]))
    {
        len = strlen((char *)&strbuf[0]);
        if ( len > 0 )
        {
            cc_result = MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, strbuf, len, PNULL, PNULL,
                CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
            if(CC_RESULT_SUCCESS == cc_result)
            {
                MMIAPIBT_SetCallSuccess();
            }
            else
            {
                MMIAPIBT_SetCallFail();
            }
        }
        else
        {
            MMIAPIBT_SetCallFail();
        }
    }
    else
    {
        MMIAPIBT_SetCallFail();
    }
    //--- TC_AG_OCM_BV_01_I
    #endif //BT_BQB_SUPPORT
}
/*****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_STATUS_REQ
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetHfuStatus(void)
{
#ifdef WIN32

#else

    uint8                   call_setup = BT_CALL_SETUP_IDLE;
   	MN_DUAL_SYS_E           active_sim = MMIAPISET_GetActiveSim();

#ifndef BLUETOOTH_SUPPORT_SPRD_BT
    uint8                   exited_call = MMIAPICC_GetExitedCallCount();
   	uint8                   hold_call 	= MMIAPICC_GetHoldCallCount();
#else
    BT_HFG_INDICATOR_T      bt_hfg_indicator = {0};
    BOOLEAN                 active_call = MMIAPICC_GetActiveCallCount();
    uint8                   call_status = MMIAPIBT_GetCallHeldStatus();
#endif
    
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
    {
        call_setup = BT_CALL_SETUP_INCOMING;
    }
    else if(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {   
        call_setup = BT_CALL_SETUP_OUTGOING;
    }
    else if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        call_setup = BT_CALL_SETUP_IDLE;
    }
    
    //call the function to inform the call status
    //蓝牙协议兼容BT_SetCallStatus的参数修改
#ifndef BLUETOOTH_SUPPORT_SPRD_BT    
    BT_SetCallStatus(MMIAPIPHONE_IsSimAvailable(active_sim), exited_call, hold_call, call_setup);//dual sys later
#else
    bt_hfg_indicator.cell_service = MMIAPIPHONE_IsSimAvailable(active_sim);
    bt_hfg_indicator.active_calls = active_call;
    bt_hfg_indicator.hold_call = call_status;
    bt_hfg_indicator.call_setup = call_setup;
    //add signal strength & roam & battery	
    bt_hfg_indicator.rxlevel = MMIIDLE_GetIdleWinInfo()->rxlevel[active_sim];
    bt_hfg_indicator.is_roaming = MMIIDLE_GetIdleWinInfo()->is_roaming[active_sim];
    bt_hfg_indicator.batlevel =  MMIIDLE_GetIdleWinInfo()->batlevel;
    BT_HFGSetCind(bt_hfg_indicator);//dual sys later
#endif

#endif
}
/*****************************************************************************/
//  Discription: when get ID_STATUS_HFAG_AUDIO_CONNECTED
//  change audio device from vb to pcm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_AudioConnected(BT_MSG_T* msg_body_ptr)
{
    BT_ADDRESS address = {0};

    if (PNULL == msg_body_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6625_112_2_18_2_8_13_278,(uint8*)"");
        return;
    }
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_OPENED);
    }
    else
    {
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_CLOSED);
    }

    MMIAPIBT_CallConnect(&address);//bug1585260

	if (MMICC_STATUS_WIN_ID == MMK_GetFocusWinId())
    {
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
	
}

/*****************************************************************************/
//  Discription: when get ID_STATUS_HFAG_AUDIO_DISCONNECTED
//  change audio device from pcm to vb
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_AudioDisconnected(BT_MSG_T* msg_body_ptr)
{
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_AudioDisconnected PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6655_112_2_18_2_8_13_279,(uint8*)"");
        return;
    }
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_CLOSED);    
    }
}
/*****************************************************************************/
//  Discription: Get if the device is visible:inquiry enable and page enable
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN  MMIBT_GetIsDeviceVisible(void)
{
    uint32      visible_info = 0;

    visible_info = BT_GetVisibility();
    //SCI_TRACE_LOW:"MMIBT_GetIsDeviceVisible: visible status is = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7254_112_2_18_2_8_15_289,(uint8*)"d",visible_info);

    if ((BT_INQUIRY_ENABLE|BT_PAGE_ENABLE) == visible_info)
    {
        // Only when device is inquiry and page enable
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : intial bt module  
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_GetFileUserSetting(BT_NV_ITEM_E type, MMIBT_USER_SETTING_U *setting_ptr)
{

    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    BOOLEAN                 folder_exist = FALSE;
    uint32                  ucs2_len = 0;
    MMIFILE_DEVICE_E        dev = MMI_DEVICE_UDISK;
    
    //SCI_PASSERT(type < BT_MAX_NV_ITEM_NUM,("MMIBT_GetFileUserSetting type %d", type));/*assert verified*/
    if(type >= BT_MAX_NV_ITEM_NUM)
    {
        return;
    }
    
    //get user setting from NV
    MMINV_READ(type,setting_ptr,return_value);
    if (MMIAPIUDISK_UdiskIsRun())
    {
        return;
    }
    //通过path得到DEV
    dev = MMIAPIFMM_GetDeviceTypeByPath(setting_ptr->dir.shaired_path_name, MMIFILE_DEVICE_LEN);
#ifdef MMIBT_FTS_SUPPORT
    if((MN_RETURN_SUCCESS != return_value) && (MMINV_BT_SHAIRED_AUTHORITY == type))
    {
        //authority
        SCI_MEMCPY(setting_ptr->authority.file_mode, MMIBT_FTP_RW_MODE, strlen(MMIBT_FTP_RW_MODE));/*lint !e666*/
    }
    else
#endif
		if(MMINV_BT_SHAIRED_DIRECTORY == type)
    {
        //get shared dir,need check this folder exist or not
        ucs2_len = MMIAPICOM_Wstrlen((wchar *)(setting_ptr->dir.shaired_path_name));
        if(ucs2_len > 3)
        {
           //not root dir
           folder_exist = MMIAPIFMM_IsFolderExist((wchar *)(setting_ptr->dir.shaired_path_name), ucs2_len);
        }
        else if(ucs2_len)
        {
            folder_exist = MMIAPIFMM_GetDeviceTypeStatus(dev);
        }
        if(dev >= MMI_DEVICE_NUM)
        {
            dev = MMIAPIFMM_GetDefaultDisk();
        }
        if(!folder_exist || (MN_RETURN_SUCCESS != return_value))
        {
            SCI_MEMSET(setting_ptr->dir.shaired_path_name, 0x00, sizeof(setting_ptr->dir.shaired_path_name));
			MMIAPICOM_Wstrncpy(setting_ptr->dir.shaired_path_name, MMIAPIFMM_GetDevicePath(dev), MMIFILE_DEVICE_LEN);
            setting_ptr->dir.shaired_path_name[1] = MMIFILE_COLON;
            //setting_ptr->dir.shaired_path_name[2] = MMIFILE_SLASH;

            MMINV_WRITE(MMINV_BT_SHAIRED_DIRECTORY,&(setting_ptr->filedev));

            /*add weiliying 2009.08.28 to modify CR 149851  start */
            MMIBT_SaveUserSetting(MMINV_BT_SHAIRED_DIRECTORY,setting_ptr);				
            /*add weiliying over*/	
        }
    }
    else if(MMINV_BT_FILE_LOCATION == type)
    {
        //file save location setting
        MMIFILE_DEVICE_E file_dev = MMIAPIFMM_GetValidateDevice(setting_ptr->filedev);
        //其实上面已经对于UDISK进行了判断
        if(MMI_DEVICE_NUM <= file_dev)
        {
            setting_ptr->filedev = MMI_DEVICE_UDISK;
        }
        else
        {
            setting_ptr->filedev = file_dev;
        }
        //udisk and sd all exist
        if(MN_RETURN_SUCCESS != return_value)
        {
            //read nv error, recovery NV
            setting_ptr->filedev = MMI_DEVICE_UDISK;
            MMINV_WRITE(MMINV_BT_FILE_LOCATION,&(setting_ptr->filedev));
        }
    }
    return;
}
/*****************************************************************************/
//  Discription: set save file location info 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_SaveUserSetting(BT_NV_ITEM_E type, MMIBT_USER_SETTING_U *setting_ptr)
{
    MMIBT_USER_SETTING_U    setting = {0}; /*lint !e64*/
    BT_STATUS           action_result = BT_SUCCESS;

    //SCI_PASSERT(setting_ptr != NULL,("MMIBT_SaveUserSetting setting_ptr != NULL"));/*assert verified*/
    if(setting_ptr == PNULL)
    {
        return BT_INVALID_PARM;
    }

    if(MMINV_BT_SHAIRED_AUTHORITY == type)
    {
        //shared directory
        //get shared directory from nv
        MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
        action_result = BT_SetFtpRootFolder((uint16*)setting.dir.shaired_path_name, (char*)setting_ptr->authority.file_mode);
    }
    else if(MMINV_BT_FILE_LOCATION != type)
    {
        //shared directory
        //get shared mode from nv
        MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_AUTHORITY, &setting);
        action_result = BT_SetFtpRootFolder((uint16*)setting_ptr->dir.shaired_path_name, (char*)setting.authority.file_mode);
    }
    if(BT_SUCCESS == action_result)
    {
        MMINV_WRITE(type,setting_ptr);
    }
    return action_result;
}
/*****************************************************************************/
//  Description : set ftp root folder 
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_SetFtpRootFolder(MMIFILE_DEVICE_E storage)
{

    BT_STATUS               action_result = BT_SUCCESS;
    MMIBT_SHAIRED_DIR_T  dir = {0};
    MMIBT_SHAIRED_AUT_T authority = {0}; 
    MMIBT_USER_SETTING_U    setting = {0}; /*lint !e64*/
    uint32                usc_len = 0;
    

    MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_AUTHORITY, &setting);
    SCI_MEMCPY(authority.file_mode, setting.authority.file_mode, strlen((char*)setting.authority.file_mode));/*lint !e666*/
    SCI_MEMSET(&setting, 0x00, sizeof(setting));
    if(MMI_DEVICE_NUM > storage)
    {
         //not invalid, get shaired directory from NV
         MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
         usc_len = MIN(MMIAPICOM_Wstrlen( (setting.dir.shaired_path_name)), MMIFILE_FULL_PATH_MAX_LEN);/*lint !e666*/
         MMIAPICOM_Wstrncpy(dir.shaired_path_name, setting.dir.shaired_path_name, usc_len);
    }
    else
    {
         //set invalid shared dirctory         
         MMIAPICOM_Wstrncpy(dir.shaired_path_name, MMIBT_FTP_UDISK_ROOT, MMIBT_FTP_UDISK_DIR_LEN);
    }
    action_result = BT_SetFtpRootFolder((uint16*)dir.shaired_path_name, (char*)authority.file_mode);
    return action_result;
}
/*****************************************************************************/
//  Discription: append item to list control(device name to list) 
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendDeviceListItem(
                                      BT_DEVICE_INFO   *device,
                                      GUIITEM_STYLE_E  item_style,
                                      MMI_TEXT_ID_T    left_softkey_id,
                                      MMI_TEXT_ID_T    middle_softkey_id,
                                      MMI_TEXT_ID_T    right_softkey_id,
                                      MMI_CTRL_ID_T    ctrl_id
                                      )
{
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32                 index = 0;

    if(device == PNULL)
    {
        return;
    }
    
    item_t.item_style    = item_style; //GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    //set softkey    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    //set left icon
    if(GUIITEM_STYLE_ONE_LINE_ICON_TEXT == item_style)
    {
        item_data.item_content[index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = GetDeviceListIcon(device);
        index ++;
    }
    //set text
    item_data.item_content[index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen((wchar*)device->name); 
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = device->name;
    GUILIST_AppendItem( ctrl_id, &item_t);
}
/*****************************************************************************/
//  Discription: append item to list control(device name to list) 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendStringListItem(
                                      MMI_TEXT_ID_T    text_id,
                                      GUIITEM_STYLE_E  item_style,
                                      MMI_TEXT_ID_T    left_softkey_id,
                                      MMI_TEXT_ID_T    middle_softkey_id,
                                      MMI_TEXT_ID_T    right_softkey_id,
                                      MMI_CTRL_ID_T    ctrl_id
                                      )
{
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    item_t.item_style    = item_style; //GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    GUILIST_AppendItem( ctrl_id, &item_t );  
}
/*****************************************************************************/
//  Discription: This function is used to Get the Paired bt dev list
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairedDevList(
                                   uint32 service,
                                   uint8  *paired_index_ptr
                                   )
{
     uint16     i =0;
     uint16     device_number =0;
     BT_STATUS  status = 0;
     BT_DEVICE_INFO dev_info = {0};

    if (PNULL == paired_index_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_GetPairedDevList PNULL == paired_dev_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7573_112_2_18_2_8_15_290,(uint8*)"");
        return;
    }
     device_number = MIN(BT_GetPairedDeviceCount(service), MMIBT_MAX_DEV_ITEMS);/*lint !e666*/
     //get handfree first
     for (i = 1; i <= device_number; i++)
     {
         SCI_MEMSET(&dev_info, 0x00, sizeof(dev_info));
         status = BT_GetPairedDeviceInfo(service, i, &dev_info);
         *paired_index_ptr = i;
		 if(BT_SUCCESS != status)
		 {
			 continue;
		 }
         else if((BT_SERVICE_HANDFREE|BT_SERVICE_HANDSET|BT_SERVICE_A2DP) & BT_GetPairedDeviceService(&(dev_info.addr)))
         {
            //handfree
			if(!((BT_SERVICE_OPP| BT_SERVICE_FTP) & BT_GetPairedDeviceService(&(dev_info.addr))))
			{
				//only has headset service 
				 paired_index_ptr ++;
			}
  
         }
     }
     //get not handfree device
     for (i = 1; i <= device_number; i++)
     {
         SCI_MEMSET(&dev_info, 0x00, sizeof(dev_info));
         status = BT_GetPairedDeviceInfo(service, i, &dev_info);
         *paired_index_ptr = i;

		 if(BT_SUCCESS != status)
		 {
			 continue;
		 }
		 if((BT_SERVICE_OPP| BT_SERVICE_FTP) & BT_GetPairedDeviceService(&(dev_info.addr)))
		 {
			 paired_index_ptr ++;
		 }
         else if(!((BT_SERVICE_HANDFREE|BT_SERVICE_HANDSET|BT_SERVICE_A2DP) & BT_GetPairedDeviceService(&(dev_info.addr))))
         {
            //not handfree device
            paired_index_ptr ++;
         }
     }
}
/*****************************************************************************/
//  Discription: update the idle blue tooth icon
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_UpdateBtIdleIcon(
                                      BOOLEAN   is_power_on,
                                      BOOLEAN   is_visible   )
{
    BLUETOOTH_STATE_TYPE     bt_status = OFF_BLUETOOTH;
#ifdef BT_DIALER_SUPPORT
	BT_ADDRESS               dev_addr = {0};
	BT_DEVICE_INFO           dev_info = {0};
#endif

    if ( is_power_on )
    {
#if defined(BT_DIALER_SUPPORT)
		if((MMIBT_IsConnectDevice(&dev_addr)) || (MMIBT_OpenedHeadsetIsExist(&dev_info)))
		{
			bt_status = ON_AND_CONNECT;
		}
		else
		{
			bt_status = ON_AND_DISCONNECT;
		}
#elif defined(ADULT_WATCH_SUPPORT)
        if(MMIAPIBT_GetActiveBTHeadset())
        {
            bt_status = ON_AND_CONNECT;
        }
        else
        {
            bt_status = ON_AND_DISCONNECT;
        }
#else
        if ( is_visible)
        {
            bt_status = ON_AND_VISIBLE;
        }
        else
        {
            bt_status = ON_AND_HIDDEN;
        }
#endif
    }
    else
    {
        bt_status = OFF_BLUETOOTH;
    }
    
    MAIN_SetIdleBlueToothState(bt_status);
}
/*****************************************************************************/
//  Discription: get blue tooth status from nv and show icon on the idle win
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetBtStatusForIcon(void)
{   
    BLUETOOTH_STATE_TYPE    bt_status = 0;
    BOOLEAN                 is_poweron = FALSE;
#ifdef BT_DIALER_SUPPORT
	BT_ADDRESS              dev_addr = {0};
	BOOLEAN                 is_connect = FALSE;
	BT_DEVICE_INFO          dev_info = {0};
#else
    BOOLEAN                 is_visible = FALSE;
#endif
#ifdef BBM_ROLE_SUPPORT
    is_poweron = Bbm_GetBTOn();
#else
    is_poweron = BT_GetState();
#endif
#ifdef BT_DIALER_SUPPORT
	if(is_poweron)
	{
		is_connect = (MMIBT_IsConnectDevice(&dev_addr)) || (MMIBT_OpenedHeadsetIsExist(&dev_info));
	}
#else
    is_visible = MMIBT_GetIsDeviceVisible(); 
#endif      
    
    if (is_poweron)
    {
#ifdef BT_DIALER_SUPPORT
		if(is_connect)
		{
			bt_status = ON_AND_CONNECT;
		}
		else
		{
			bt_status = ON_AND_DISCONNECT;
		}
#else
        if ( is_visible)
        {
            bt_status = ON_AND_VISIBLE;
        }
        else
        {
            bt_status = ON_AND_HIDDEN;
        }
#endif
    }
    else
    {
        bt_status = OFF_BLUETOOTH;
    }
    //set blue tooth icon status
    MAIN_SetIdleBTStateWhenPowerOn(bt_status);
}
/*****************************************************************************/
//  Discription: set blue tooth file save location: u disk and sd card
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetFileSaveLocation(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMIFILE_DEVICE_E        location_info = MMI_DEVICE_UDISK;
    MMIBT_SHAIRED_DIR_T     dir = {0};
#ifdef MMIBT_FTS_SUPPORT
    MMIBT_SHAIRED_AUT_T     authority = {0}; 
#endif
        
    //get the file location info from NV
    MMINV_READ(MMINV_BT_FILE_LOCATION,&location_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        location_info = MMI_DEVICE_UDISK;
        MMINV_WRITE(MMINV_BT_FILE_LOCATION,&location_info);
    }

    if(!MMIAPIFMM_GetDeviceTypeStatus(location_info))
    {
        location_info = MMIAPIFMM_GetFirstValidDevice(); 
    }
    
    //SCI_TRACE_LOW:"SetFileSaveLocation: location info is = %d, return_value is = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7774_112_2_18_2_8_16_291,(uint8*)"dd",location_info,return_value);
    MMINV_WRITE(MMINV_BT_FILE_LOCATION,&location_info);
    //set shared directory 
    MMINV_READ(MMINV_BT_SHAIRED_DIRECTORY,&dir,return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        //udisk
        MMIAPICOM_Wstrncpy(
                            dir.shaired_path_name, 
                            MMIAPIFMM_GetDevicePath(location_info),
                            MMIAPIFMM_GetDevicePathLen(location_info)
                            );
        dir.shaired_path_name[1] = MMIFILE_COLON;
        MMINV_WRITE(MMINV_BT_SHAIRED_DIRECTORY, &dir);
    }
#ifdef MMIBT_FTS_SUPPORT    
    //set shared authority
    MMINV_READ(MMINV_BT_SHAIRED_AUTHORITY, &authority,return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMCPY(authority.file_mode, MMIBT_FTP_RW_MODE, strlen(MMIBT_FTP_RW_MODE));/*lint !e666*/
        //reset shared directory
        MMINV_WRITE(MMINV_BT_SHAIRED_AUTHORITY, &authority);
    }
#endif
}
/*****************************************************************************/
//  Discription: get the put request, OPP Server, FTP Server
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_GetReceiveFilePutReq( 
                                        BT_MSG_T                        *msg_body_ptr,
                                        MMIBT_FILE_SERVER_TYPE_E        service_type
                                        )
{
     if(MMIBT_IsPhoneReady())
     {
        //turn on the back light           
        MMIDEFAULT_SetBackLight(TRUE);
        
        //open new file request window
        MMIBT_OpenIncomingFileWin(msg_body_ptr,service_type); 
     }
}
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsDelFail(BT_MSG_T    *msg_body_ptr)
{
	MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);
}

/*****************************************************************************/
//  Discription: the request of add folder by FTP
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsAddFolderReq(BT_MSG_T  *msg_body_ptr)
{
    BT_CreateNewFolder(TRUE);
    MMIBT_UpdateCurrentFileList();
}
#endif//MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Description : set ftp root folder after device power on
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void SetFtpRootFolderAfterPowerOn(void)
{    
    MMIBT_SetFtpRootFolder(MMI_DEVICE_UDISK);
}
/*****************************************************************************/
//  Discription: judge if file open 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN IsFileUsed(const uint16 *file_name)
{
    SFS_ERROR_E     sfs_del_rlt = SFS_NO_ERROR;
    SFS_HANDLE      file_handle = SFS_ERROR_NONE;
    BOOLEAN         is_file_type = TRUE;
    BOOLEAN         result = FALSE;
    
    is_file_type = MMIAPIFMM_GetFileInfo(file_name,MMIAPICOM_Wstrlen(file_name),PNULL,PNULL,PNULL);
    //SCI_TRACE_LOW:"bt is_file_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8078_112_2_18_2_8_16_296,(uint8*)"d", is_file_type);
    
    if(is_file_type)
    {
        file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING,PNULL,PNULL);
        if(SFS_INVALID_HANDLE == file_handle)
        {
            result = TRUE;
        }
        //SCI_TRACE_LOW:"bt file_handle = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8087_112_2_18_2_8_16_297,(uint8*)"d", file_handle);
        SFS_CloseFile(file_handle);
    }
    else
    {
        sfs_del_rlt = MMIAPIFMM_CreateDirectory(file_name);
        if(SFS_ERROR_ACCESS == sfs_del_rlt)
        {
            result = TRUE;
        }
        //SCI_TRACE_LOW:"bt SFS_ERROR_E = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8097_112_2_18_2_8_16_298,(uint8*)"d", sfs_del_rlt);
    }      

    return result;
}
/*****************************************************************************/
//  Discription: DeviceIsPaired
//  Global resource dependence: none 
//  Author: 
//  Note: get bt device is paired by address
/*****************************************************************************/
PUBLIC BOOLEAN DeviceIsPaired(BT_ADDRESS * addr)
{
    BOOLEAN ret = FALSE;
    uint16      i =0;
    uint16      device_number =0;
    BT_STATUS   status = 0;
    BT_DEVICE_INFO dev = {0};

    if (PNULL == addr)
    {
        //SCI_TRACE_LOW:"DeviceIsPaired PNULL == addr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8961_112_2_18_2_8_18_310,(uint8*)"");
        return FALSE;
    }
    device_number = MIN(BT_GetPairedDeviceCount(BT_SERVICE_ALL), MMIBT_MAX_DEV_ITEMS);/*lint !e666*/

    for (i = 1; i <= device_number; i++)
    {
        status = BT_GetPairedDeviceInfo(BT_SERVICE_ALL, i, &dev);
        //SCI_PASSERT(BT_SUCCESS == status,("status = %d",status));
        ret = !memcmp(addr, &(dev.addr), sizeof(BT_ADDRESS));
        if(ret)
        {
            break;
        }
    }
    //SCI_TRACE_LOW:"mmibt.c: DeviceIsPaired device number is %d ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8976_112_2_18_2_8_18_311,(uint8*)"dd", device_number, ret);
    return ret;
}

#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
/*****************************************************************************/
//  Discription:check the deviece is connect ftp device 
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedFTP(BT_ADDRESS * in_addr)
{
	BOOLEAN ret = FALSE;
	uint32    service_type = 0;
	
	if(PNULL != in_addr)
	{
		service_type = BT_GetPairedDeviceService(in_addr);
		if((BT_SERVICE_FTP & service_type) && (TRUE == s_is_fts_connect))
		{
			ret = TRUE;
		}
	}   
	//SCI_TRACE_LOW:"DeviceIsConnectedFTP ret %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9046_112_2_18_2_8_18_314,(uint8*)"d",ret);
	return ret;
}

/*****************************************************************************/
//  Discription:check is connect ftp device 
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectedFTP(void)
{
	SCI_TRACE_LOW("MMIBT_IsConnectedFTP return %d", s_is_fts_connect);
	return s_is_fts_connect;
}
#endif

/*****************************************************************************/
//  Discription:check the deviece is on transfer status 
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsOnTransferStatus(BT_ADDRESS * in_addr,BT_ADDRESS * transfer_addr)
{
	BOOLEAN ret = FALSE;
	
	if(PNULL != in_addr && PNULL != transfer_addr)
	{		
		if(MMIBT_IsOnTransferStatus() && BTIsSameDevAddr(in_addr,transfer_addr))
		{
            ret = TRUE;
        }		
	}
	SCI_TRACE_LOW("MMIBT_DeviceIsOnTransferStatus ret %d", ret);
	return ret;
}
/*****************************************************************************/
//  Description : Is Same Dev Addr
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BTIsSameDevAddr(BT_ADDRESS* des, BT_ADDRESS* src)
{
    BOOLEAN res = FALSE;

    if(PNULL == des || PNULL == src)
    {
        return res;
    }
    if((des->addr[0] == src->addr[0])&&(des->addr[1] == src->addr[1])
        &&(des->addr[2] == src->addr[2])&&(des->addr[3] == src->addr[3])
        &&(des->addr[4] == src->addr[4])&&(des->addr[5] == src->addr[5])
    )
    {
        res = TRUE;
    }
    return res;
}
/*****************************************************************************/
//  Discription:get device icon by device info
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: device icon
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T GetDeviceListIcon(BT_DEVICE_INFO *dev)
{
    MMI_IMAGE_ID_T icon = IMAGE_BT_PC_NO_PAIRE;
    uint16          dev_type = 0;
    uint16          icon_index = 0;
    MMI_IMAGE_ID_T  no_pair_icon_array[] = {IMAGE_BT_HEADSET_NO_PAIRE, IMAGE_BT_HANFREE_NO_PAIRE, IMAGE_BT_PC_NO_PAIRE};    
    MMI_IMAGE_ID_T  pair_icon_array[] = {IMAGE_BT_HEADSET_DISCONNECT, IMAGE_BT_HANFREE_PAIRE, IMAGE_BT_PC_PAIRE};

    MMI_IMAGE_ID_T  watch_no_pair_icon_array[] = {IMAGE_BT_WATCH_HEADSET_NO_PAIRE,IMAGE_BT_WATCH_PHONE_NO_PAIRE,IMAGE_BT_WATCH_PC_NO_PAIRE};
    
    //SCI_PASSERT(NULL != dev,("GetDeviceListIcon dev %d",dev));
    if (PNULL == dev)
    {
        //SCI_TRACE_LOW:"GetDeviceListIcon PNULL == dev!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9445_112_2_18_2_8_19_317,(uint8*)"");
        return icon;
    }
    BT_GetDeviceType(dev, &dev_type);
    //SCI_TRACE_LOW:"BT_GetDeviceType dev_type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9449_112_2_18_2_8_19_318,(uint8*)"d",dev_type);
    if(dev_type & BT_DEVICE_CLASS_AUDIO)
    {
        icon_index = 0;
    }
    else if(dev_type & BT_DEVICE_CLASS_PHONE)
    {
        icon_index = 1;
    }
    else
    {
        icon_index = 2;
    }
    if(DeviceIsPaired(&(dev->addr)))
    {
        //paired device
        if((dev_type & BT_DEVICE_CLASS_AUDIO) && MMIBT_DeviceIsConnectedHeadset(&(dev->addr)))
        {
            //connect headset
            icon = IMAGE_BT_HEADSET_CONNECT;
        }
#ifdef BT_DIALER_SUPPORT
#ifndef WIN32
        else if((dev_type & BT_DEVICE_CLASS_PHONE) && MMIBT_DeviceIsConnectedHandsetGateway(&(dev->addr)))
        {
            //connect phone
            icon = IMAGE_BT_HANFREE_CONNECT;
        }
        else if((dev_type & BT_DEVICE_CLASS_COMPUTER) && BT_HfuGetConnStatus(&(dev->addr)))
        {
            //connect computer
            icon = IMAGE_BT_PC_CONNECT;
        }
#endif
#endif
        else
        {
             icon = pair_icon_array[icon_index];
        }
    }
    else
    {
        //not paired device
        //icon = no_pair_icon_array[icon_index];
        icon = watch_no_pair_icon_array[icon_index];
    }
    return icon;
}
#ifdef BT_BQB_SUPPORT
//mdy by qing.yu@for cr238862
/*****************************************************************************/
//  Discription:
//  Parameter: 
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_ClccInd(BT_MSG_T* msg_body)
{
    //SCI_TRACE_LOW:"MMIBT_ClccInd msg_body %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11160_112_2_18_2_8_23_358,(uint8*)"d", msg_body);
    if(PNULL != msg_body)
    {
       MMIAPICC_SendCallInfo2HFG();
    }
}

/*****************************************************************************/
//  Discription:
//  Parameter: 
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_PariedHandFree(void)
{
    //set opeartion info
    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
    MMIBT_EnterDeviceListWin(PNULL,MMIBT_OPC_MAIN,BT_SERVICE_HANDSET);
}
#endif //BT_BQB_SUPPORT
/*****************************************************************************/
//  Description : Cancel BT transfer file (send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_StopBTTransferFile(void)
{ 
    BT_STATUS   return_value = BT_PENDING;
    
    return_value = MMIBT_CancelFileTransfer(TRUE);
    if(BT_PENDING == return_value)
    {
       OpenCancelWaitingWin();
    }
    else
    {
        if(BT_SUCCESS ==return_value)
        {
            MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);
        }
        else
        {
            //SCI_TRACE_LOW:"[&&&]MMIBT_StopBTTransferFile"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11475_112_2_18_2_8_23_364,(uint8*)"");
            MMIPUB_OpenAlertFailWin(TXT_COMM_SEND_FAILED);
        }
    }
}
/*****************************************************************************/
//  Discription: Callback function of blue tooth
//  Global resource dependence: None
//  Author: Kelly.li
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CallBackFunc(DPARAM param)
{
    uint16						msg_id = 0;
    BT_MSG_T					*pMsg = (BT_MSG_T *)param;
	POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();

    //SCI_TRACE_LOW:"MMIBT_CallBackFunc:restart_condition=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1637_112_2_18_2_7_50_133,(uint8*)"d", restart_condition);

#if !defined(WIN32) && defined(AUTOTEST_DEVICE_SUPPORT)
    MMIAutotest_BTDeviceMode(pMsg);
#endif    

	if ((RESTART_BY_ALARM == restart_condition) || (RESTART_BY_CHARGE == restart_condition))
	{
		//restart with alarm, need not to handle BT message
		//Free msg space
		if (pMsg->body_size >0)
		{
#ifdef WIN32
			SCI_FREE(pMsg->body_ptr);
#else		
			OS_Pfree(pMsg->body_ptr);
#endif
			pMsg->body_ptr = PNULL;
		}
		return;
	}
    //先处理ual能够处理的message
    if(TRUE == bt_ual_msg_call_back(pMsg))//bt dev 已经用CMS模块处理
    {
        return;
    }

	//SCI_TRACE_LOW:"MMIBT_CallBackFunc, msg_id = %d"
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1655_112_2_18_2_7_50_134,(uint8*)"d", pMsg->msg_id);

    switch(pMsg->msg_id)
    {
    case ID_STATUS_BT_ON_RES:
        //SCI_TRACE_LOW:"Bluetooth power on success.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1660_112_2_18_2_7_50_135,(uint8*)"");
        msg_id = MMI_BT_ME_ON_CNF;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_BT_OFF_RES:
        //SCI_TRACE_LOW:"Bluetooth power off success.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1666_112_2_18_2_7_50_136,(uint8*)"");
        msg_id = MMI_BT_ME_OFF_CNF;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_CM_INQUIRY_RES:
        //SCI_TRACE_LOW:"Bluetooth found 1 device: "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1672_112_2_18_2_7_50_137,(uint8*)"");
        msg_id = MMI_BT_INQ_DEV_NAME;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_CM_INQUIRY_REQ: 
        //SCI_TRACE_LOW:"Bluetooth got the address and the name of the device.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1678_112_2_18_2_7_50_138,(uint8*)"");
        msg_id = MMI_BT_INQ_DEV_ADDR;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_CM_INQUIRY_FINISH:
        //SCI_TRACE_LOW:"Bluetooth inquiry completed\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1684_112_2_18_2_7_50_139,(uint8*)"");
        msg_id = MMI_BT_INQ_COMP_CNF;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_CM_CANCEL_INQUIRY_RES:
        //SCI_TRACE_LOW:"Bluetooth inquiry is cancelled.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1690_112_2_18_2_7_50_140,(uint8*)"");
        msg_id = MMI_BT_INQUIRY_CANCEL;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_CM_VISIBLE_RES:
        //SCI_TRACE_LOW:"Bluetooth set visible success.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1696_112_2_18_2_7_50_141,(uint8*)"");
        msg_id = MMI_BT_VISIBILE_CNF;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_CM_DISVISIBLE_RES:
        //SCI_TRACE_LOW:"Bluetooth set invisible success.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1702_112_2_18_2_7_50_142,(uint8*)"");
        msg_id = MMI_BT_HIDDEN_CNF;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_SC_PASSKEY_REQ:
        //SCI_TRACE_LOW:"Bluetooth input pin request"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1708_112_2_18_2_7_50_143,(uint8*)"");
        msg_id = MMI_BT_DEV_PIN_REQ;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_SC_SSP_NUM_IND:
        SCI_TRACE_LOW("Bluetooth ID_STATUS_SC_SSP_NUM_IND");
        msg_id = MMI_BT_SSP_NUM_IND;
        MMIBT_SendSignal(msg_id, param);
        break;


    case ID_STATUS_SC_PAIR_RES:
        //SCI_TRACE_LOW:"Bluetooth pairing success.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1714_112_2_18_2_7_50_144,(uint8*)"");
        msg_id = MMI_BT_DEV_PAIR_COMPLETE;
        MMIBT_SendSignal(msg_id,param);
        break;

#ifdef MMIBT_HEADSET_AUTHORIZE 
    case ID_STATUS_SC_AUTH_IND:
        SCI_TRACE_LOW("Bluetooth ID_STATUS_SC_AUTH_IND");
        msg_id = MMI_BT_DEV_AUTH_IND;
        MMIBT_SendSignal(msg_id,param);
        break;
#endif

    case ID_STATUS_SC_PAIR_IND:
        //SCI_TRACE_LOW:"Bluetooth earphone pairing success.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1720_112_2_18_2_7_50_145,(uint8*)"");
        msg_id = MMI_BT_DEV_PAIR_COMPLETE_IND;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_SC_DEPAIR_RES:
        //SCI_TRACE_LOW:"Bluetooth got delete paired device result.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1725_112_2_18_2_7_50_146,(uint8*)"");
        msg_id = MMI_BT_DELETE_DEVICE_RES;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_SD_READ_SERVICES_RES:
        //SCI_TRACE_LOW:"Bluetooth read service result.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1731_112_2_18_2_7_50_147,(uint8*)"");
        msg_id = MMI_BT_DEV_SERVICE_RESULT;
        MMIBT_SendSignal(msg_id,param);
        break;
    //HFG Message define begin	
    case ID_STATUS_HFAG_SLC_CONNECTED:
        //SCI_TRACE_LOW:"Bluetooth headset service connected.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1737_112_2_18_2_7_50_148,(uint8*)"");
        msg_id = MMI_BT_HFAG_SLC_CONNECTED;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_HFAG_SLC_CONNECTED_IND:
        SCI_TRACE_LOW("Bluetooth headset service connected.");
        //SCI_TRACE_LOW:"IND Bluetooth headset service connected.\n"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1743_112_2_18_2_7_50_149,(uint8*)"");
        msg_id = MMI_BT_HFAG_SLC_CONNECTED_IND;
        MMIBT_SendSignal(msg_id,param);
        break;
    case ID_STATUS_HFAG_SLC_DISCONNECTED:
        //SCI_TRACE_LOW:"Bluetooth hand free service disconnected.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1748_112_2_18_2_7_50_150,(uint8*)"");
        msg_id = MMI_BT_HFAG_SLC_DISCONNECTED;
        MMIBT_SendSignal(msg_id,param);
        break;
    case ID_STATUS_HFAG_PLAY_CFM:
        //HFG play or resume
        SCI_TRACE_LOW("Bluetooth hand free resume device.");
        //SCI_TRACE_LOW:"MMI_BT_PLAY_RESUNME_DEVICE_CNF.\n"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1754_112_2_18_2_7_50_151,(uint8*)"");
        msg_id = MMI_BT_PLAY_RESUNME_DEVICE_CNF;
        MMIBT_SendSignal(msg_id,param);
        break;
    case ID_STATUS_HFAG_STOP_CFM:
        //HFG pause or close device
        SCI_TRACE_LOW("Bluetooth hand free pause device.\n");
        //SCI_TRACE_LOW:"MMI_BT_PAUSE_STOP_DEVICE_CNF.\n"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1770_112_2_18_2_7_50_152,(uint8*)"");
        msg_id = MMI_BT_PAUSE_STOP_DEVICE_CNF;
        MMIBT_SendSignal(msg_id,param);
        break;
    case ID_STATUS_HFAG_HFU_CALL_ANSWER:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_CALL_ANSWER.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1785_112_2_18_2_7_50_153,(uint8*)"");
        msg_id = MMI_BT_HFU_CALL_ANSWER;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_HFAG_HFU_CALL_HANGUP: 		
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HF_CALL1_HUNGUP.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1791_112_2_18_2_7_51_154,(uint8*)"");
        msg_id = MMI_BT_HFU_CALL_HANGUP;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_HFAG_HFU_DIAL_NUMBER:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_DIAL_NUMBER.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1797_112_2_18_2_7_51_155,(uint8*)"");
        msg_id = MMI_BT_HFAG_DIAL_NUMBER;     
        MMIBT_SendSignal(msg_id,param);	
        break;

    case ID_STATUS_HFAG_HFU_DIAL_MEMORY:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_DIAL_MEMORY.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1803_112_2_18_2_7_51_156,(uint8*)"");
        msg_id = MMI_BT_HFAG_DIAL_MEMORY;     
        MMIBT_SendSignal(msg_id,param);	
        break;

    case ID_STATUS_HFAG_HFU_REDIAL:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_REDIAL.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1809_112_2_18_2_7_51_157,(uint8*)"");
        msg_id = MMI_BT_HFAG_REDAIL_CALL;     
        MMIBT_SendSignal(msg_id,param);	
        break;

    case ID_STATUS_HFAG_HFU_HOLD:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_HOLD.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1815_112_2_18_2_7_51_158,(uint8*)"");
        msg_id = MMI_BT_HFAG_HFU_HOLD;
        MMIBT_SendSignal(msg_id,param);	
        break;
    case ID_STATUS_HFAG_HFU_DTMF_NUMBER:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_DTMF_NUMBER.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1820_112_2_18_2_7_51_159,(uint8*)"");
        msg_id = MMI_BT_HFU_DTMF_NUMBER;     
        MMIBT_SendSignal(msg_id,param);	
        break;
    case ID_STATUS_HFAG_AUDIO_CONNECTED:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_AUDIO_CONNECTED.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1825_112_2_18_2_7_51_160,(uint8*)"");
        msg_id = MMI_BT_HFAG_AUDIO_CONNECTED;     
        MMIBT_SendSignal(msg_id,param);		
        break;

    case ID_STATUS_HFAG_AUDIO_DISCONNECTED:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_AUDIO_DISCONNECTED.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1831_112_2_18_2_7_51_161,(uint8*)"");
        msg_id = MMI_BT_HFAG_AUDIO_DISCONNECTED;     
        MMIBT_SendSignal(msg_id,param);	
        break;

    case ID_STATUS_HFAG_HFU_STATUS_REQ:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_STATUS_REQ.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1837_112_2_18_2_7_51_162,(uint8*)"");
        msg_id = MMI_BT_HFU_STATUS_REQ;				
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_HS_MIC_VOL_CHANGE:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HS_MIC_VOL_CHANGE.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1843_112_2_18_2_7_51_163,(uint8*)"");
        msg_id = MMI_BT_HS_MIC_VOL_CHANGE;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_HS_SPK_VOL_CHANGE:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HS_SPK_VOL_CHANGE.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1849_112_2_18_2_7_51_164,(uint8*)"");
        msg_id = MMI_BT_HS_SPK_VOL_CHANGE;
        MMIBT_SendSignal(msg_id,param);
        break;
#ifdef BT_BQB_SUPPORT
    case ID_STATUS_HFAG_HFU_CLCC:
        //SCI_TRACE_LOW:"Bluetooth ID_STATUS_HFAG_HFU_CLCC.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1855_112_2_18_2_7_51_165,(uint8*)"");
        msg_id = MMI_BT_HFU_CLCC;
        MMIBT_SendSignal(msg_id,param);
        break;
#endif
    //HFG Message define end

    //OPP Client message begin(sending file)
    case ID_STATUS_OPC_CONNECT_RES:
        //SCI_TRACE_LOW:"Bluetooth Opc connect result.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1864_112_2_18_2_7_51_166,(uint8*)"");
        msg_id = MMI_BT_OPC_CONNECT_RESULT;
        MMIBT_SendSignal(msg_id,param);
        break;

    case ID_STATUS_OPC_SERVER_ABORT:
    //SCI_TRACE_LOW:"Bluetooth Opc server abort ind.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1870_112_2_18_2_7_51_167,(uint8*)"");
    msg_id = MMI_BT_OPC_SERVER_ABORT;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_OPC_PUT_ABORT:
    //SCI_TRACE_LOW:"Bluetooth Opc put abort ind.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1876_112_2_18_2_7_51_168,(uint8*)"");
    msg_id = MMI_BT_SEND_CANCELED;
    MMIBT_SendSignal(msg_id,param);		
    break;

    case ID_STATUS_OPC_PUT_OBJECT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1882_112_2_18_2_7_51_169,(uint8*)"");
    msg_id = MMI_BT_PACKET_SENT;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_OPC_PUT_FINAL:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_OPC_PUT_FINAL.\n");
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1888_112_2_18_2_7_51_170,(uint8*)"");
    msg_id = MMI_BT_OPC_SEND_RES;
    MMIBT_SendSignal(msg_id,param);	
    break;

    //+++ CR244706
    case ID_STATUS_OPC_DISCONNECT_IND:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPC_DISCONNECT_IND"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1895_112_2_18_2_7_51_171,(uint8*)"");
    msg_id = MMI_BT_OPC_DISCONNECT_IND;
    MMIBT_SendSignal(msg_id, param);
    break;
    //--- CR244706
    //OPP Client message end (sending file)
    //DUN msg
#ifdef BT_DUN_SUPPORT
    case ID_STATUS_DG_LINK_REQ:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_DUN_LINK_REQ.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1904_112_2_18_2_7_51_172,(uint8*)"");
    msg_id = MMI_BT_DUN_LINK_REQ;
    MMIBT_SendSignal(msg_id,param); 
    break;
    case ID_STATUS_DG_LINK_STATUS_RES:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_DG_LINK_STATUS_RES.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1909_112_2_18_2_7_51_173,(uint8*)"");
    msg_id = MMI_BT_DUN_LINK_SUC;
    MMIBT_SendSignal(msg_id,param); 
    break;
    case ID_STATUS_DG_LINK_CLOSE_IND:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_DG_LINK_CLOSE_IND.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1914_112_2_18_2_7_51_174,(uint8*)"");
    msg_id = MMI_BT_DUN_LINK_CLOSE;
    MMIBT_SendSignal(msg_id,param);             
    break;	
#endif
    //DUN msg end			 
    case ID_STATUS_FTS_GET_REQ:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_GET_REQ.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1921_112_2_18_2_7_51_175,(uint8*)"");
    msg_id = MMI_BT_FTS_GET_REQ;
    MMIBT_SendSignal(msg_id,param);	
    break;

    //FTP Client message define begin (sending file)
    case ID_STATUS_FTS_UPDATE_ROOT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_UPDATE_ROOT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1928_112_2_18_2_7_51_176,(uint8*)"");
    msg_id = MMI_BT_FTS_SHARED_FOLDER_UPDATE;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_FTC_CONNECT_RES:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTC_CONNECT_RES.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1933_112_2_18_2_7_51_177,(uint8*)"");
    //msg_id = MMI_BT_FTS_SHARED_FOLDER_UPDATE;
    //MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_FTC_SERVER_ABORT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTC_SERVER_ABORT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1939_112_2_18_2_7_51_178,(uint8*)"");
    msg_id = MMI_BT_OPC_PUT_ABORT;
    MMIBT_SendSignal(msg_id,param);  
    break;

    case ID_STATUS_FTC_PUT_ABORT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTC_PUT_ABORT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1945_112_2_18_2_7_51_179,(uint8*)"");
    msg_id = MMI_BT_OPC_PUT_ABORT;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_FTC_PUT_OBJECT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTC_PUT_OBJECT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1951_112_2_18_2_7_51_180,(uint8*)"");
    msg_id = MMI_BT_PACKET_SENT;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_FTC_PUT_FINAL:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTC_PUT_FINAL.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1957_112_2_18_2_7_51_181,(uint8*)"");
    msg_id = MMI_BT_OPC_SEND_RES;
    MMIBT_SendSignal(msg_id,param);
    break;
    //FTP Client message define end (sending file)

    //OPP Server Message begin (receive file)
    case ID_STATUS_OPS_PUT_REQ:
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1965_112_2_18_2_7_51_182,(uint8*)"");
    SCI_TRACE_LOW("Bluetooth ID_STATUS_OPS_PUT_REQ.\n");
    msg_id = MMI_BT_OPS_PUT_REQ;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_OPS_CLIENT_ABORT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPS_CLIENT_ABORT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1971_112_2_18_2_7_51_183,(uint8*)"");
    msg_id = MMI_BT_OPS_CLIENT_ABORT;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_OPS_PUT_ABORT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPS_PUT_ABORT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1977_112_2_18_2_7_51_184,(uint8*)"");
    msg_id = MMI_BT_RECEIVE_CANCELED;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_OPS_PUT_OBJECT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPS_PUT_OBJECT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1983_112_2_18_2_7_51_185,(uint8*)"");
    msg_id = MMI_BT_PACKET_RECEIVED;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_OPS_PUT_FINAL:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPS_PUT_FINAL.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1989_112_2_18_2_7_51_186,(uint8*)"");
    msg_id = MMI_BT_OPS_RECEIVE_FILE_RES;
    MMIBT_SendSignal(msg_id,param);	
    break;

    //+++ CR244706
    case ID_STATUS_OPS_DISCONNECT_IND:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPS_DISCONNECT_IND.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1996_112_2_18_2_7_51_187,(uint8*)"");
    msg_id = MMI_BT_OPS_DISCONNECT_IND;
    MMIBT_SendSignal(msg_id,param);
    break;    
    //--- CR244706
    //OPP Server Message end (receive file)

    //FTP Server Message begin (receive file)
    case ID_STATUS_FTS_PUT_REQ:
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2005_112_2_18_2_7_51_188,(uint8*)"");
    SCI_TRACE_LOW("Bluetooth ID_STATUS_FTS_PUT_REQ.\n");
    msg_id = MMI_BT_FTS_PUT_REQ;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_FTS_CONNECT_REQ:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_CONNECT_REQ.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2011_112_2_18_2_7_51_189,(uint8*)"");
    msg_id = MMI_BT_FTS_CONNECT_REQ;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_OPS_CONNECT_REQ:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_OPS_CONNECT_REQ.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2017_112_2_18_2_7_51_190,(uint8*)"");
    msg_id = MMI_BT_OPS_CONNECT_REQ;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_FTS_CLIENT_ABORT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_CLIENT_ABORT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2023_112_2_18_2_7_51_191,(uint8*)"");
    msg_id = MMI_BT_FTS_CLIENT_ABORT;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_FTS_PUT_ABORT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_PUT_ABORT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2029_112_2_18_2_7_51_192,(uint8*)"");
    msg_id = MMI_BT_RECEIVE_CANCELED;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_FTS_PUT_OBJECT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_PUT_OBJECT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2035_112_2_18_2_7_51_193,(uint8*)"");
    msg_id = MMI_BT_PACKET_RECEIVED;
    MMIBT_SendSignal(msg_id,param);	
    break;

    case ID_STATUS_FTS_PUT_FINAL:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_PUT_FINAL.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2041_112_2_18_2_7_51_194,(uint8*)"");
    msg_id = MMI_BT_FTS_RECEIVE_FILE_RES;
    MMIBT_SendSignal(msg_id,param);	
    break;
    case ID_STATUS_FTS_GET_ABORT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_GET_ABORT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2046_112_2_18_2_7_51_195,(uint8*)"");
    msg_id = MMI_BT_SEND_CANCELED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_FTS_GET_FINAL:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_GET_FINAL.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2051_112_2_18_2_7_51_196,(uint8*)"");
    msg_id = MMI_BT_FTS_SEND_RES;
    MMIBT_SendSignal(msg_id,param);	
    break;
    case ID_STATUS_FTS_GET_OBJECT:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_GET_OBJECT.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2056_112_2_18_2_7_51_197,(uint8*)"");
    msg_id = MMI_BT_PACKET_SENT;
    MMIBT_SendSignal(msg_id,param);	
    break;		
    //FTP Server Message end (receive file)			
    case ID_STATUS_FTS_DEL_REQ:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_DEL_REQ.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2062_112_2_18_2_7_51_198,(uint8*)"");
    msg_id = MMI_BT_FTS_DEL_REQ;
    MMIBT_SendSignal(msg_id,param);	
    break;
    case ID_STATUS_FTS_DEL_BUSY:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_DEL_BUSY.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2067_112_2_18_2_7_51_199,(uint8*)"");
    msg_id = MMI_BT_FTS_DEL_FAIL;
    MMIBT_SendSignal(msg_id,param);	
    break;
    case ID_STATUS_FTS_ADD_FOLDER_REQ:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_ADD_FOLDER_REQ.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2072_112_2_18_2_7_51_200,(uint8*)"");
    msg_id = MMI_BT_FTS_ADD_FOLDER_REQ;
    MMIBT_SendSignal(msg_id,param);			
    break;

    //+++ CR244706
    case ID_STATUS_FTS_DISCONNECT_IND:
    //SCI_TRACE_LOW:"Bluetooth ID_STATUS_FTS_DISCONNECT_IND.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2079_112_2_18_2_7_51_201,(uint8*)"");
    msg_id = MMI_BT_FTS_DISCONNECT_IND;
    MMIBT_SendSignal(msg_id,param);
    break;    
    //--- CR244706

#ifdef BT_SPP_SUPPORT
    //SPP Message begin
    case ID_STATUS_SPP_LINK_STATUS_RES:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_SPP_LINK_STATUS_RES.\n");
    //msg_id = MMI_BT_SPP_LINK_STATUS_RES;
    //MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_SPP_DATA_RECIEVE_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_SPP_DATA_RECIEVE_IND.\n");
    msg_id = MMI_BT_SPP_DATA_RECIEVE_IND;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_SPP_DATA_RECIEVE_CFM:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_SPP_DATA_RECIEVE_CFM.\n");
    msg_id = MMI_BT_SPP_DATA_RECIEVE_CFM;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_SPP_CONNECT_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_SPP_CONNECT_IND.\n");
    msg_id = MMI_BT_SPP_CONNECT_IND;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_SPP_DISCONNECT_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_SPP_DISCONNECT_IND.\n");
    msg_id = MMI_BT_SPP_DISCONNECT_IND;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_SPP_STATUS_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_SPP_STATUS_IND.\n");
    msg_id = MMI_BT_SPP_STATUS_IND;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_SPP_SERVICE_NAME_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_SPP_SERVICE_NAME_IND.\n");
    //msg_id = MMI_BT_SPP_SERVICE_NAME_IND;
    //MMIBT_SendSignal(msg_id,param);
    break;
    //SPP Message end
#endif
    //PBAP Message start
#ifdef BT_PBAP_SUPPORT
    case ID_STATUS_PBAP_TP_DISCONNECTED:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_PBAP_TP_DISCONNECTED.\n");
    msg_id = MMI_BT_PBAP_TP_DISCONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_PBAP_COMPLETE:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_PBAP_COMPLETE.\n");
    msg_id = MMI_BT_PBAP_COMPLETE;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_PBAP_ABORT:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_PBAP_ABORT.\n");
    msg_id = MMI_BT_PBAP_ABORT;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_PBAP_PARAMS_RX:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_PBAP_PARAMS_RX.\n");
    msg_id = MMI_BT_PBAP_PARAMS_RX;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_PBAP_DATA_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_PBAP_DATA_IND.\n");
    msg_id = MMI_BT_PBAP_DATA_IND;
    MMIBT_SendSignal(msg_id,param);
    break;
#endif
    //PBAP Message end

#if defined(BT_MAP_SUPPORT)
    case ID_STATUS_MAP_TP_DISCONNECTED:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_TP_DISCONNECTED.\n");
    msg_id = MMI_BT_MAP_TP_DISCONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_COMPLETE:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_COMPLETE.\n");
    msg_id = MMI_BT_MAP_COMPLETE;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_ABORT:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_ABORT.\n");
    msg_id = MMI_BT_MAP_ABORT;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_CLIENT_PARAMS_RX:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_CLIENT_PARAMS_RX.\n");
    msg_id = MMI_BT_MAP_CLIENT_PARAMS_RX;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_SERVER_PARAMS_RX:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_SERVER_PARAMS_RX.\n");
    msg_id = MMI_BT_MAP_SERVER_PARAMS_RX;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_DATA_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_DATA_IND.\n");
    msg_id = MMI_BT_MAP_DATA_IND;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_MNS_SERVER_CONNECTED:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_MNS_SERVER_CONNECTED.\n");
    msg_id = MMI_BT_MAP_MNS_SERVER_CONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_MNS_SERVER_PARAMS_RX:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_MNS_SERVER_PARAMS_RX.\n");
    msg_id = MMI_BT_MAP_MNS_SERVER_PARAMS_RX;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_MAP_MNS_SERVER_DATA_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_MAP_MNS_SERVER_DATA_IND.\n");
    msg_id = MMI_BT_MAP_MNS_SERVER_DATA_IND;
    MMIBT_SendSignal(msg_id,param);
    break;
#endif

#ifdef BT_AV_SNK_SUPPORT
    case ID_STATUS_A2DP_SINK_CONNECT_COMPLETED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_A2DP_SINK_CONNECT_COMPLETED.\n");
    msg_id = MMI_BT_A2DP_SINK_CONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_A2DP_SINK_DISCONNECT_COMPLETED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_A2DP_SINK_DISCONNECT_COMPLETED.\n");
    msg_id = MMI_BT_A2DP_SINK_DISCONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_A2DP_SINK_STREAM_STARTED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_A2DP_SINK_STREAM_STARTED.\n");
    msg_id = MMI_BT_A2DP_SINK_STREAM_STARTED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_A2DP_SINK_STREAM_SUSPENDED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_A2DP_SINK_STREAM_SUSPENDED.\n");
    msg_id = MMI_BT_A2DP_SINK_STREAM_SUSPENDED;
    MMIBT_SendSignal(msg_id,param);
    break;
#endif

#ifdef BT_AV_CT_SUPPORT
    case ID_STATUS_AVRCP_CT_CONNECT_COMPLETE:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_CT_CONNECT_COMPLETE.\n");
    msg_id = MMI_BT_AVRCP_CT_CONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_CT_DISCONNECT_COMPLETE:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_CT_DISCONNECT_COMPLETE.\n");
    msg_id = MMI_BT_AVRCP_CT_DISCONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_CT_MEDIA_ATTRIBUTES:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_CT_MEDIA_ATTRIBUTES");
    msg_id = MMI_BT_AVRCP_CT_MEDIA_ATTRIBUTES;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_EVENT_TRACK_CHANGED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_EVENT_TRACK_CHANGED");
    msg_id = MMI_BT_AVRCP_CT_TRACK_CHANGED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_CT_PLAY_STATUS:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_CT_PLAY_STATUS");
    msg_id = MMI_BT_AVRCP_CT_PLAY_STATUS;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_EVENT_PLAYBACK_CHANGED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_EVENT_PLAYBACK_CHANGED");
    msg_id = MMI_BT_AVRCP_CT_PLAYBACK_CHANGED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_EVENT_PLAYBACK_POS_CHANGED");
    msg_id = MMI_BT_AVRCP_CT_PLAYBACK_POS_CHANGED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_EVENT_PLAYER_APPLICATION_CHANGED:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_EVENT_PLAYER_APPLICATION_CHANGED");
    msg_id = MMI_BT_AVRCP_CT_PLAYER_APPLICATION_CHANGED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_AVRCP_SUPPORTED_PLAYER_ATTRIBUTES:
    SCI_TRACE_LOW("Bluetooth BT_AV, ID_STATUS_AVRCP_SUPPORTED_PLAYER_ATTRIBUTES");
    msg_id = MMI_BT_AVRCP_CT_SUPPORTED_PLAYER_ATTRIBUTES;
    MMIBT_SendSignal(msg_id,param);
    break;
#endif

#ifdef BT_HID_SUPPORT
    case ID_STATUS_HID_CONNECTED_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_HID_CONNECTED_IND");
    msg_id = MMI_BT_HID_CONNECT_IND;
    MMIBT_SendSignal(msg_id,param);
    break;

    case ID_STATUS_HID_DISCONNECTED_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_HID_DISCONNECTED_IND");
    msg_id = MMI_BT_HID_DISCONNECT_IND;
    MMIBT_SendSignal(msg_id,param);
    break;
#endif
#ifdef BT_BLE_SUPPORT
    case ID_STATUS_BLE_CONNECTED:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_BLE_CONNECTED");
    msg_id = MMI_BT_BLE_CONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_BLE_DISCONNECTED:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_BLE_DISCONNECTED");
    msg_id = MMI_BT_BLE_DISCONNECTED;
    MMIBT_SendSignal(msg_id,param);
    break;
    case ID_STATUS_BLE_DATA_IND:
    SCI_TRACE_LOW("Bluetooth ID_STATUS_BLE_DATA_IND");
    msg_id = MMI_BT_BLE_DATA_IND;
    MMIBT_SendSignal(msg_id,param);
    break;
#endif
    default:
    //SCI_TRACE_LOW:"Bluetooth signal unsolvable 0x%x.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2086_112_2_18_2_7_51_202,(uint8*)"d",pMsg->msg_id);
    if (pMsg->body_size >0)
    {
#ifdef WIN32
    SCI_FREE(pMsg->body_ptr);
#else		
    OS_Pfree(pMsg->body_ptr);
#endif
    pMsg->body_ptr = PNULL;
    }
    break;
    }
	// TO DO: construct the signal using BT_MSG_T and send it to MMI task. 
}

/*****************************************************************************/
//  Discription: Callback function of blue tooth
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_A2DPCallBackFunc(DPARAM param)
{
#ifdef	BT_A2DP_SUPPORT
	uint16						msg_id = 0;
	BT_MSG_T					*pMsg = (BT_MSG_T*)param;
	POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();

    //SCI_TRACE_LOW:"MMIBT_A2DPCallBackFunc:restart_condition=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2113_112_2_18_2_7_51_203,(uint8*)"d", restart_condition);

	if ((RESTART_BY_ALARM == restart_condition) || (RESTART_BY_CHARGE == restart_condition))
	{
		//restart with alarm, need not to handle BT message
		//Free msg space
		if (pMsg->body_size >0)
		{
#ifdef WIN32
			SCI_FREE(pMsg->body_ptr);
#else		
			OS_Pfree(pMsg->body_ptr);
#endif
			pMsg->body_ptr = PNULL;
		}
		return;
	}

	//SCI_TRACE_LOW:"MMIBT_A2DPCallBackFunc: pMsg->msg_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2131_112_2_18_2_7_51_204,(uint8*)"d", pMsg->msg_id);

	switch(pMsg->msg_id)
	{
	//AVRCP control message begin
	case ID_STATUS_AVRCP_OPERATION_ID_BACKWARD:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_BACKWARD.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2137_112_2_18_2_7_51_205,(uint8*)"");
		msg_id = MMI_BT_A2DP_OPERATION_BACKWARD;
        MMIBT_SendSignal(msg_id,param);
		break;

	case ID_STATUS_AVRCP_OPERATION_ID_FORWARD:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_FORWARD.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2143_112_2_18_2_7_51_206,(uint8*)"");
		msg_id = MMI_BT_A2DP_OPERATION_FORWARD;
        MMIBT_SendSignal(msg_id,param);		
		break;

	case ID_STATUS_AVRCP_OPERATION_ID_VOL_UP:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_VOL_UP.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2149_112_2_18_2_7_51_207,(uint8*)"");
		msg_id = MMI_BT_A2DP_OPERATION_VOL_UP;
        MMIBT_SendSignal(msg_id,param);		
		break;
		
	case ID_STATUS_AVRCP_OPERATION_ID_VOL_DOWN:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_VOL_DOWN.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2155_112_2_18_2_7_51_208,(uint8*)"");
		msg_id = MMI_BT_A2DP_OPERATION_VOL_DOWN;
        MMIBT_SendSignal(msg_id,param);			
		break;

	case ID_STATUS_AVRCP_OPERATION_ID_PLAY:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_PLAY.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2161_112_2_18_2_7_51_209,(uint8*)"");
		msg_id = MMI_BT_A2DP_OPERATION_PLAY;
        MMIBT_SendSignal(msg_id,param);	
		break;

	case ID_STATUS_AVRCP_OPERATION_ID_STOP:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_STOP.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2167_112_2_18_2_7_51_210,(uint8*)"");
		msg_id = MMI_BT_A2DP_OPERATION_STOP;
        MMIBT_SendSignal(msg_id,param);
		break;

	case ID_STATUS_AVRCP_OPERATION_ID_PAUSE:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_PAUSE.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2173_112_2_18_2_7_51_211,(uint8*)"");
		msg_id = MMI_BT_A2DP_OPERATION_PAUSE;
        MMIBT_SendSignal(msg_id,param);		
		break;
		
	case ID_STATUS_AVRCP_CONNECT_COMPLETE:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_CONNECT_COMPLETE.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2179_112_2_18_2_7_51_212,(uint8*)"");
		msg_id = MMI_BT_AVRCP_CONNECT;
        MMIBT_SendSignal(msg_id,param);	
		break;
			
	case ID_STATUS_AVRCP_DISCONNECT_COMPLETE:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_AVRCP_DISCONNECT_COMPLETE.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2185_112_2_18_2_7_51_213,(uint8*)"");
		msg_id = MMI_BT_AVRCP_DISCONNECT;
        MMIBT_SendSignal(msg_id,param);	
		break;
	//AVRCP control message end
	
	//A2DP control message begin
	case ID_STATUS_A2DP_SRC_CONFIG_STREAM: 
		//SCI_TRACE_LOW:"bluetooth A2DP SRC config strem"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2193_112_2_18_2_7_51_214,(uint8*)"");
		msg_id = MMI_BT_A2DP_SRC_CONFIG_STREAM;
        MMIBT_SendSignal(msg_id,param);
		break;

	case ID_STATUS_A2DP_SRC_OPEN_STREAM:
		//SCI_TRACE_LOW:"bluetooth A2DP SRC open strem"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2199_112_2_18_2_7_51_215,(uint8*)"");
		break;
		
	case ID_STATUS_A2DP_SRC_START_STREAM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_SRC_START_STREAM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2203_112_2_18_2_7_51_216,(uint8*)"");
		msg_id = MMI_BT_OPEN_DEVICE_CNF;
        MMIBT_SendSignal(msg_id,param);	
		break;

	case ID_STATUS_A2DP_SRC_RESUME_STREAM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_SRC_RESUME_STREAM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2209_112_2_18_2_7_51_217,(uint8*)"");
		msg_id = MMI_BT_RESUME_DEVICE_CNF;
        MMIBT_SendSignal(msg_id,param);		
		break;

	case ID_STATUS_A2DP_SRC_SUSPEND_STREAM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_SRC_SUSPEND_STREAM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2215_112_2_18_2_7_51_218,(uint8*)"");
		msg_id = MMI_BT_SUSPEND_DEVICE_CNF;
        MMIBT_SendSignal(msg_id,param);	
		break;

	case ID_STATUS_A2DP_SRC_CLOSE_STREAM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_SRC_CLOSE_STREAM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2221_112_2_18_2_7_51_219,(uint8*)"");
		msg_id = MMI_BT_CLOSE_DEVICE_CNF;
        MMIBT_SendSignal(msg_id,param);	
		break;
	
	case ID_STATUS_A2DP_CONNECTED:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_CONNECTED.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2227_112_2_18_2_7_51_220,(uint8*)"");
		msg_id = MMI_BT_A2DP_CONNECTED;
        MMIBT_SendSignal(msg_id,param);			
		break;
	
    case ID_STATUS_A2DP_CONNECTED_IND:
		//SCI_TRACE_LOW:"IND Bluetooth ID_STATUS_A2DP_CONNECTED.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2233_112_2_18_2_7_51_221,(uint8*)"");
		msg_id = MMI_BT_A2DP_CONNECTED_IND;
        MMIBT_SendSignal(msg_id,param);			
		break;

	case ID_STATUS_A2DP_DISCONNECTED:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_DISCONNECTED.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2239_112_2_18_2_7_51_222,(uint8*)"");
		msg_id = MMI_BT_A2DP_DISCONNECTED;
        MMIBT_SendSignal(msg_id,param);		
		break;

	case ID_STATUS_A2DP_SNK_START_STREAM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_SNK_START_STREAM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2245_112_2_18_2_7_51_223,(uint8*)"");
		msg_id = MMI_BT_A2DP_START_STREAM_IND;
        MMIBT_SendSignal(msg_id,param);	
		break;

	case ID_STATUS_A2DP_SNK_SUSPEND_STREAM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_SNK_SUSPEND_STREAM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2251_112_2_18_2_7_51_224,(uint8*)"");
		msg_id = MMI_BT_A2DP_SUSPEND_STREAM_IND;
        MMIBT_SendSignal(msg_id,param);	
		break;

	case ID_STATUS_A2DP_ABORT_CFM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_ABORT_CFM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2257_112_2_18_2_7_51_225,(uint8*)"");
		msg_id = MMI_BT_A2DP_ABORT_CFM;
        MMIBT_SendSignal(msg_id,param);	
		break;

	case ID_STATUS_A2DP_ABORT_IND:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_ABORT_IND.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2263_112_2_18_2_7_51_226,(uint8*)"");
		msg_id = MMI_BT_A2DP_ABORT_IND;
        MMIBT_SendSignal(msg_id,param);	
		break;

	case ID_STATUS_A2DP_SNK_RESUME_STREAM:
		//SCI_TRACE_LOW:"Bluetooth ID_STATUS_A2DP_SNK_RESUME_STREAM.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2269_112_2_18_2_7_51_227,(uint8*)"");
		msg_id = MMI_BT_A2DP_RESUME_STREAM_IND;
        MMIBT_SendSignal(msg_id,param);	
		break;
	//A2DP control message end
	
	default:
		//SCI_TRACE_LOW:"Bluetooth signal unsolvable 0x%x.\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2276_112_2_18_2_7_52_228,(uint8*)"d",pMsg->msg_id);
        if (pMsg->body_size >0)
		{
#ifdef WIN32
			SCI_FREE(pMsg->body_ptr);
#else		
			OS_Pfree(pMsg->body_ptr);
#endif
			pMsg->body_ptr = PNULL;
		}
		break;
	}
	// TO DO: construct the signal using BT_MSG_T and send it to MMI task. 
#endif
}

/*****************************************************************************/
//  Discription: This function used to process msg comeback form blue tooth
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E AppHandleBTMsg(
								  PWND app_ptr,
								  uint16 msg_id,
								  DPARAM param
								  )
{
	BT_MSG_T     *msg_body = &((BT_SIGNAL*)param)->content;
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	
	//SCI_TRACE_LOW:"AppHandleBTMsg: msg_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1154_112_2_18_2_7_49_60,(uint8*)"d", msg_id);

	switch(msg_id)
	{
	case MMI_BT_ME_ON_CNF:
        //SCI_TRACE_LOW:"AppHandleBTMsg: MMI_BT_ME_ON_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1159_112_2_18_2_7_49_61,(uint8*)"");
        if(TRUE == MMIENG_UITestBTIsOpenWin())
        {
            MMIENG_UITestBT_ON_CNF(msg_body,msg_id);
            break;
        }
        #if 0
        if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
        {
            BTMainUpdateUI(TRUE);
            break;
        }
        #else
        if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
        {
            WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
            break;
        }
        #endif
        MMIBT_GetBtOnOffCnf(msg_body,TRUE);
        break;

        case MMI_BT_ME_OFF_CNF:
            //SCI_TRACE_LOW:"AppHandleBTMsg:MMI_BT_ME_OFF_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1164_112_2_18_2_7_49_62,(uint8*)"");	
#if 0
            if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                BTMainUpdateUI(FALSE);
                break;
            }
#else
            if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                break;
            }
#endif
            MMIBT_GetBtOnOffCnf(msg_body, FALSE);
            break;

	case MMI_BT_INQUIRY_CANCEL:
	{
            //BOOLEAN flag = FALSE;
            //SCI_TRACE_LOW:"AppHandleBTMsg:MMI_BT_INQUIRY_CANCEL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1169_112_2_18_2_7_49_63,(uint8*)"");
            //flag = MMK_IsOpenWin(MMIBT_MAIN_WIN_ID);
            #if 0
            if(TRUE == flag)
            {
                MMK_SendMsg(MMIBT_MAIN_WIN_ID,MMI_BT_INQUIRY_CANCEL,PNULL);
                break;
            }
            #else
            if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                break;
            }
            #endif
            MMIBT_CancelSearchCnf(msg_body);
            break;
	}
		
	case MMI_BT_VISIBILE_CNF:
            //SCI_TRACE_LOW:"AppHandleBTMsg:MMI_BT_VISIBILE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1174_112_2_18_2_7_49_64,(uint8*)"");
            #if 0
            if(MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                MMK_SendMsg(MMIBT_MAIN_WIN_ID,MMI_BT_VISIBILE_CNF,PNULL);
                break;
            }
            #else
            if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                break;
            }
            #endif
            MMIBT_GetVisibleCnf(msg_body, TRUE);
            break;
			
	case MMI_BT_HIDDEN_CNF:
            //SCI_TRACE_LOW:"AppHandleBTMsg:MMI_BT_HIDDEN_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1179_112_2_18_2_7_49_65,(uint8*)"");
            #if 0
            if(MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
            MMK_SendMsg(MMIBT_MAIN_WIN_ID,MMI_BT_VISIBILE_CNF,PNULL);
            break;
            }
            #else
            if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                break;
            }
            #endif
            MMIBT_GetVisibleCnf(msg_body, FALSE);			
            break;

        case MMI_BT_INQ_DEV_NAME:
        {

            //SCI_TRACE_LOW:"AppHandleBTMsg: MMI_BT_INQ_DEV_NAME"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1184_112_2_18_2_7_49_66,(uint8*)"");
            if(TRUE == MMIENG_UITestBTIsOpenWin())
            {
                MMIENG_UITestBT_ON_CNF(msg_body,msg_id);
                break;
            }
            if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                break;
            }
            MMIBT_GetInqDevName(msg_body);
        }
        break;

	case MMI_BT_INQ_COMP_CNF://搜索设备结束
        {

            //SCI_TRACE_LOW:"MMI_BT_INQ_COMP_CNF"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1189_112_2_18_2_7_49_67,(uint8*)"");
            SCI_TRACE_LOW("lzk log. MMI_BT_INQ_COMP_CNF begin.");
            if(TRUE == MMIENG_UITestBTIsOpenWin())
            {
                 MMIENG_UITestBT_ON_CNF(msg_body,msg_id);
                 break;
            }

            if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                break;
            }
            
            MMIBT_GetInqCompCnf();
            SCI_TRACE_LOW("lzk log. MMI_BT_INQ_COMP_CNF end.");
            break;
        }   

	case MMI_BT_DEV_PIN_REQ:
         {
		//BT_PairDevice callback msg
		//SCI_TRACE_LOW:"MMI_BT_DEV_PIN_REQ"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1195_112_2_18_2_7_49_68,(uint8*)"");
        //if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))//bug1531710
        {
            WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
            break;
        }
		//MMIBT_GetInputPinReq(msg_body);
		//break;
	}

	case MMI_BT_SSP_NUM_IND:
	{
            SCI_TRACE_LOW("MMI_BT_INQ_COMP_CNF begin.");
            //if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                SCI_TRACE_LOW("MMI_BT_INQ_COMP_CNF end.");
                break;
            }
            //MMIBT_GetNumericPasskeyReq(msg_body);
            //break;
	}


	case MMI_BT_DEV_PAIR_COMPLETE:
	{

            SCI_TRACE_LOW("MMI_BT_DEV_PAIR_COMPLETE begin.");
            //SCI_TRACE_LOW:"MMI_BT_DEV_PAIR_COMPLETE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1200_112_2_18_2_7_49_69,(uint8*)"");
            //if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
            {
                WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
                break;
            }
            //MMIBT_GetPairComCnf(msg_body);
            //break;
	}
			

#ifdef MMIBT_HEADSET_AUTHORIZE         
	case MMI_BT_DEV_AUTH_IND:
        SCI_TRACE_LOW("MMI_BT_DEV_AUTH_IND");
        MMIBT_SetAuthorization(msg_body);
        break;
#endif        
    case MMI_BT_DEV_PAIR_COMPLETE_IND:
		//SCI_TRACE_LOW:"MMI_BT_DEV_PAIR_COMPLETE_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1205_112_2_18_2_7_49_70,(uint8*)"");
        WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
        break;
        //MMIBT_GetPairComIND(msg_body);
		//break;
	case MMI_BT_DELETE_DEVICE_RES:
		//SCI_TRACE_LOW:"MMI_BT_DELETE_DEVICE_RES"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1209_112_2_18_2_7_49_71,(uint8*)"");
        if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
        {
            WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
            break;
        }
        MMIBT_DeleteDeviceCnf(msg_body);
        break;

//HFG Message define begin
	case MMI_BT_HFAG_SLC_CONNECTED:     //Bluetooth HFAG SLC connected   
		//SCI_TRACE_LOW:"MMI_BT_HFAG_SLC_CONNECTED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1215_112_2_18_2_7_49_72,(uint8*)"");
		MMIBT_OpenHeadsetCnf(msg_body, FALSE);
#ifdef ADULT_WATCH_SUPPORT
             MAIN_SetIdleBlueToothState(ON_AND_CONNECT);

             if(TRUE == MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
             {
                 AdultWatchBt_ProcessBtPsMsg(msg_id, msg_body);
             }
#endif
		break;
    case MMI_BT_HFAG_SLC_CONNECTED_IND:     //Bluetooth HFAG SLC connected   IND
		//SCI_TRACE_LOW:"MMI_BT_HFAG_SLC_CONNECTED_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1219_112_2_18_2_7_49_73,(uint8*)"");
		MMIBT_OpenHeadsetConfirmIND(msg_body, FALSE);
#ifdef ADULT_WATCH_SUPPORT
             MAIN_SetIdleBlueToothState(ON_AND_CONNECT);

             if(TRUE == MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
             {
                  AdultWatchBt_ProcessBtPsMsg(msg_id, msg_body);
             }
#endif
		break;
	case MMI_BT_HFAG_SLC_DISCONNECTED:  //Bluetooth HFAG SLC disconnected
		//SCI_TRACE_LOW:"MMI_BT_HFAG_SLC_DISCONNECTED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1223_112_2_18_2_7_49_74,(uint8*)"");
        MMIBT_CloseHeadsetCnf(msg_body, FALSE);
       if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
       {
            WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
       }
#ifdef ADULT_WATCH_SUPPORT
        AdultWatch_RemoveHeadset(msg_body,FALSE);
        MAIN_SetIdleBlueToothState(ON_AND_DISCONNECT);

        if(TRUE == MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
        {
             AdultWatchBt_ProcessBtPsMsg(msg_id, msg_body);
        }
#endif
		break;

	case MMI_BT_HFU_CALL_ANSWER:       //Bluetooth call 1 connect
		//SCI_TRACE_LOW:"MMI_BT_CALL1_CONNECTED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1228_112_2_18_2_7_49_75,(uint8*)"");
		MMIAPPICC_BtAcceptCall();
		break;

	case MMI_BT_HFU_CALL_HANGUP:   
            //SCI_TRACE_LOW:"MMI_BT_HFU_CALL_HANGUP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1233_112_2_18_2_7_49_76,(uint8*)"");
            MMIAPICC_BtCancelCall();            
		break;		

	case MMI_BT_HFAG_DIAL_NUMBER:
		//SCI_TRACE_LOW:"MMI_BT_HFAG_DIAL_NUMBER"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1238_112_2_18_2_7_49_77,(uint8*)"");
		MMIBT_HfagDiaNumber(msg_body);
		break;

	case MMI_BT_HFAG_DIAL_MEMORY:
		//SCI_TRACE_LOW:"MMI_BT_HFAG_DIAL_MEMORY"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1243_112_2_18_2_7_49_78,(uint8*)"");
		MMIBT_HfagDiaMemory(msg_body);
		break;

	case MMI_BT_HFAG_REDAIL_CALL:
        //答录机过程中蓝牙未建立链路，认为不处于通话中，按键会发出重拨消息,此时做挂断处理
#ifdef ASP_SUPPORT
            if(MMIAPIASP_IsMachineWorking())
            {
                //SCI_TRACE_LOW:"MMI_BT_HFU_CALL_HANGUP"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1252_112_2_18_2_7_49_79,(uint8*)"");
                MMIAPICC_BtCancelCall();  //MMIAPIASP_RealseCall();
            }
            else
#endif
            {
                //SCI_TRACE_LOW:"MMI_BT_HFAG_REDAIL_CALL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1258_112_2_18_2_7_49_80,(uint8*)"");
                MMIBT_HfagRedailCall();
            }
            break;

	case MMI_BT_HFAG_HFU_HOLD:
		//SCI_TRACE_LOW:"MMI_BT_HFAG_HFU_HOLD"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1264_112_2_18_2_7_49_81,(uint8*)"");
		MMIAPICC_HandleBtHfuHoldInd(msg_body);
		break;
    case MMI_BT_HFU_DTMF_NUMBER:
        //SCI_TRACE_LOW:"MMI_BT_HFU_DTMF_NUMBER"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1268_112_2_18_2_7_49_82,(uint8*)"");
        if(msg_body != PNULL && msg_body->body_ptr != PNULL)
        {
            //SCI_TRACE_LOW:"MMI_BT_HFU_DTMF_NUMBER char 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1271_112_2_18_2_7_49_83,(uint8*)"d", *(uint8 *)msg_body->body_ptr);
            MMIAPICC_SendDtmfByChar(MN_DUAL_SYS_MAX,*(uint8 *)msg_body->body_ptr);
        }
        break;
		
	case MMI_BT_HFAG_AUDIO_CONNECTED:
		//SCI_TRACE_LOW:"MMI_BT_HFAG_AUDIO_CONNECTED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1277_112_2_18_2_7_49_84,(uint8*)"");
		MMIBT_AudioConnected(msg_body);
		break;

	case MMI_BT_HFAG_AUDIO_DISCONNECTED:
		//SCI_TRACE_LOW:"MMI_BT_HFAG_AUDIO_DISCONNECTED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1282_112_2_18_2_7_49_85,(uint8*)"");
		MMIBT_AudioDisconnected(msg_body);
		break;
	
	case MMI_BT_HFU_STATUS_REQ:
		//SCI_TRACE_LOW:"MMI_BT_HFU_STATUS_REQ"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1287_112_2_18_2_7_49_86,(uint8*)"");
		MMIBT_SetHfuStatus();
		break;

    case MMI_BT_HS_SPK_VOL_CHANGE:
        //SCI_TRACE_LOW:"MMI_BT_HS_SPK_VOL_CHANGE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1293_112_2_18_2_7_49_87,(uint8*)"");
        //MMIAPICC_BTSpeakerVolChange();
#ifdef ADULT_WATCH_SUPPORT
        WatchCC_BTSpeakerVolChange();
#endif
        break;
        
#ifdef BT_BQB_SUPPORT        
    case MMI_BT_HFU_CLCC:
        //SCI_TRACE_LOW:"MMI_BT_HFU_CLCC"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1298_112_2_18_2_7_49_88,(uint8*)"");
        MMIBT_ClccInd(msg_body);
        break;
#endif
		
//HFG Message define end
//DUN Message define begin
#ifdef BT_DUN_SUPPORT			
      case MMI_BT_DUN_LINK_REQ:
          //SCI_TRACE_LOW:"MMI_BT_DUN_LINK_REQ"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1307_112_2_18_2_7_50_89,(uint8*)"");
          MMIBT_OpenDUNWin(msg_body);
          break;
      case MMI_BT_DUN_LINK_SUC:
          //SCI_TRACE_LOW:"MMI_BT_DUN_LINK_SUC"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1311_112_2_18_2_7_50_90,(uint8*)"");
          MMIBT_DUNOpenedInd(msg_body);
          break;
      case MMI_BT_DUN_LINK_CLOSE:
          //SCI_TRACE_LOW:"MMI_BT_DUN_LINK_CLOSE"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1315_112_2_18_2_7_50_91,(uint8*)"");
          MMIBT_DUNClosedInd(msg_body);
          break;
#endif
//DUN Message define end

//OPP Client send file begin
	case MMI_BT_OPC_CONNECT_RESULT:
		//BT_OppSendFile callback msg
		//SCI_TRACE_LOW:"MMI_BT_OPC_CONNECT_RESULT"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1324_112_2_18_2_7_50_92,(uint8*)"");
		MMIBT_ConnectDeviceResCnf(msg_body);
		break;

	case MMI_BT_OPC_PUT_ABORT:
	case MMI_BT_OPC_SERVER_ABORT:
		//SCI_TRACE_LOW:"MMI_BT_OPC_SERVER_ABORT"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1330_112_2_18_2_7_50_93,(uint8*)"");
		MMIBT_SendFileFailedInd();
		break;
	case MMI_BT_SEND_CANCELED:
		//SCI_TRACE_LOW:"MMI_BT_SEND_CANCELED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1334_112_2_18_2_7_50_94,(uint8*)"");
		MMIAPIBT_TransferCancelled();
		break;
	case MMI_BT_RECEIVE_CANCELED:
		//SCI_TRACE_LOW:"MMI_BT_RECEIVE_CANCELED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1338_112_2_18_2_7_50_95,(uint8*)"");
		MMIBT_TransferCancelled(FALSE);
		break;
	case MMI_BT_PACKET_SENT:
		//SCI_TRACE_LOW:"MMI_BT_PACKET_SENT"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1342_112_2_18_2_7_50_96,(uint8*)"");
		MMIBT_GetPacketSentInd(msg_body);
		break;

	case MMI_BT_OPC_SEND_RES:
		//SCI_TRACE_LOW:"MMI_BT_OPC_SEND_RES"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1347_112_2_18_2_7_50_97,(uint8*)"");
		MMIBT_SendFileResInd (msg_body);
		break;
//OPP Client send file end

	case MMI_BT_FTS_GET_REQ:
		//SCI_TRACE_LOW:"MMI_BT_FTS_GET_REQ"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1353_112_2_18_2_7_50_98,(uint8*)"");
		MMIBT_GetFtpReq(msg_body);
		break;

    case MMI_BT_FTS_SEND_RES:
		SCI_TRACE_LOW("MMI_BT_FTS_SEND_RES");
		MMIBT_SendFileResInd (msg_body);
        MMIBT_SendNextFileCallBack();
		break;
    
	case MMI_BT_FTS_SHARED_FOLDER_UPDATE:
		//SCI_TRACE_LOW:"MMI_BT_FTS_SHARED_FOLDER_UPDATE"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1358_112_2_18_2_7_50_99,(uint8*)"");
		MMIBT_UpdateFtpSharedFolder(msg_body);
		break;

    case MMI_BT_OPS_CONNECT_REQ:
		//SCI_TRACE_LOW:"MMI_BT_OPS_CONNECT_REQ"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1363_112_2_18_2_7_50_100,(uint8*)"");
		MMIBT_OpsConnectReq(msg_body);
		break;
        
//receive file begin
	case MMI_BT_OPS_PUT_REQ:
		//SCI_TRACE_LOW:"MMI_BT_OPS_PUT_REQ!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1369_112_2_18_2_7_50_101,(uint8*)"");
		MMIBT_GetReceiveFilePutReq( msg_body,MMIBT_FILE_SERVER_OPP_RECIEVE);
		break;

	case MMI_BT_FTS_CLIENT_ABORT:
	case MMI_BT_OPS_CLIENT_ABORT:
		//SCI_TRACE_LOW:"Opp/FTP server client abort!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1375_112_2_18_2_7_50_102,(uint8*)"");
		MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
		//对方取消发送文件
		MMIBT_ReceiveFileFail();
		break;

    //+++ CR244706
    case MMI_BT_FTS_DISCONNECT_IND://手机和电脑连接(包含发送、接收)
      SCI_TRACE_LOW("FTS_DISCONNECT_IND");
	  //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1383_112_2_18_2_7_50_103,(uint8*)"");
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
	  s_is_fts_connect = FALSE;
#endif
	  MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
	  MMIBT_UpdateMainMenu(TRUE);
#ifdef BT_DIALER_SUPPORT
      if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
      {
          MMIBT_UpdateBtPhoneWin(TRUE);
      }
#endif
	  break;
    case MMI_BT_OPS_DISCONNECT_IND: //手机接收文件
	  SCI_TRACE_LOW("OPS_DISCONNECT_IND");
	  //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1383_112_2_18_2_7_50_103,(uint8*)"");
	  MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
	  MMIBT_UpdateMainMenu(TRUE);
#ifdef BT_DIALER_SUPPORT
      if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
      {
          MMIBT_UpdateBtPhoneWin(TRUE);
      }
#endif
	  break;
    case MMI_BT_OPC_DISCONNECT_IND://手机发送文件
        //SCI_TRACE_LOW:"OPC_DISCONNECT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1387_112_2_18_2_7_50_104,(uint8*)"");
        MMIBT_SendNextFileCallBack();
		break;
    //--- CR244706
	case MMI_BT_PACKET_RECEIVED:
		//SCI_TRACE_LOW:"MMI_BT_PACKET_RECEIVED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1392_112_2_18_2_7_50_105,(uint8*)"");
		MMIBT_GetPacketReceivedInd(msg_body);
		break;

	case MMI_BT_OPS_RECEIVE_FILE_RES:
		SCI_TRACE_LOW("MMI_BT_OPS_RECEIVE_FILE_RES");
		//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1397_112_2_18_2_7_50_106,(uint8*)"");
		MMIBT_ReceiveFileResInd(msg_body, FALSE);
		break;
#ifdef MMIBT_FTS_SUPPORT
	case MMI_BT_FTS_RECEIVE_FILE_RES:
		SCI_TRACE_LOW("MMI_BT_FTS_RECEIVE_FILE_RES");
		//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1397_112_2_18_2_7_50_106,(uint8*)"");
		MMIBT_ReceiveFileResInd(msg_body, TRUE);
		break;		
	case MMI_BT_FTS_PUT_REQ:
		//SCI_TRACE_LOW:"Ftp server put request!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1402_112_2_18_2_7_50_107,(uint8*)"");
		MMIBT_GetReceiveFilePutReq( msg_body,MMIBT_FILE_SERVER_FTP_RECIEVE);
		break;
	case MMI_BT_FTS_DEL_REQ:
		//SCI_TRACE_LOW:"MMI_BT_FTS_DEL_REQ!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1406_112_2_18_2_7_50_108,(uint8*)"");
		MMIBT_FtsDelReq(msg_body);
		break;

    case MMI_BT_FTS_DEL_FAIL:
		//SCI_TRACE_LOW:"MMI_BT_FTS_DEL_FAIL!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1411_112_2_18_2_7_50_109,(uint8*)"");
		MMIBT_FtsDelFail(msg_body);
		break;
	case MMI_BT_FTS_ADD_FOLDER_REQ:
		//SCI_TRACE_LOW:"MMI_BT_FTS_ADD_FOLDER_REQ!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1415_112_2_18_2_7_50_110,(uint8*)"");
		MMIBT_FtsAddFolderReq(msg_body);
		break;

     case MMI_BT_FTS_CONNECT_REQ:
		//SCI_TRACE_LOW:"MMI_BT_FTS_CONNECT_REQ"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1420_112_2_18_2_7_50_111,(uint8*)"");
		MMIBT_FtsConnectReq(msg_body);
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
	    s_is_fts_connect = TRUE;
#endif
		break;	
#endif//MMIBT_FTS_SUPPORT
//receive file end	
	case MMI_BT_OPEN_DEVICE_CNF:
		//SCI_TRACE_LOW:"MMI_BT_OPEN_DEVICE_CNF"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1426_112_2_18_2_7_50_112,(uint8*)"");
		MMIBT_OpenDeviceCnf(msg_body);
		break;
		
	case MMI_BT_RESUME_DEVICE_CNF:
		//SCI_TRACE_LOW:"MMI_BT_RESUME_DEVICE_CNF"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1431_112_2_18_2_7_50_113,(uint8*)"");
		MMIBT_ResumeDeviceCnf(msg_body);
		break;

	case MMI_BT_SUSPEND_DEVICE_CNF:
		//SCI_TRACE_LOW:"MMI_BT_SUSPEND_DEVICE_CNF"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1436_112_2_18_2_7_50_114,(uint8*)"");
		MMIBT_PauseDeviceCnf(msg_body);
		break;

	case MMI_BT_CLOSE_DEVICE_CNF:
		//SCI_TRACE_LOW:"MMI_BT_CLOSE_DEVICE_CNF"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1441_112_2_18_2_7_50_115,(uint8*)"");
        //SCI_Sleep(50);
		MMIBT_CloseDeviceCnf(msg_body);
		break;	
	case MMI_BT_PLAY_RESUNME_DEVICE_CNF:
        //SCI_TRACE_LOW:"MMI_BT_PLAY_RESUNME_DEVICE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1446_112_2_18_2_7_50_116,(uint8*)"");
		MMIBT_OpenDeviceCnf(msg_body);  
		break;
	case MMI_BT_PAUSE_STOP_DEVICE_CNF:
        //SCI_TRACE_LOW:"MMI_BT_PAUSE_STOP_DEVICE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1450_112_2_18_2_7_50_117,(uint8*)"");
        MMIBT_CloseDeviceCnf(msg_body);
		break;
#ifdef BT_A2DP_SUPPORT
//begin for a2dp message

	case MMI_BT_A2DP_OPERATION_BACKWARD:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_OPERATION_BACKWARD"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1457_112_2_18_2_7_50_118,(uint8*)"");
		MMIBT_A2dpAvrcpBackword();
		break;

	case MMI_BT_A2DP_OPERATION_FORWARD:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_OPERATION_FORWARD"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1462_112_2_18_2_7_50_119,(uint8*)"");
		MMIBT_A2dpAvrcpForward();
		break;

	case MMI_BT_A2DP_OPERATION_VOL_UP:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_OPERATION_VOL_UP"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1467_112_2_18_2_7_50_120,(uint8*)"");
		MMIBT_A2dpAvrcpAdjustVolum(TRUE);
		break;
	
	case MMI_BT_A2DP_OPERATION_VOL_DOWN:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_OPERATION_VOL_DOWN"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1472_112_2_18_2_7_50_121,(uint8*)"");
		MMIBT_A2dpAvrcpAdjustVolum(FALSE);
		break;

	case MMI_BT_A2DP_OPERATION_PLAY:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_OPERATION_PLAY"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1477_112_2_18_2_7_50_122,(uint8*)"");
		MMIBT_AvrcpA2dpPlay();
		break;

	case MMI_BT_A2DP_OPERATION_STOP:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_OPERATION_STOP"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1482_112_2_18_2_7_50_123,(uint8*)"");
		MMIBT_AvrcpA2dpStop();
		break;

	case MMI_BT_A2DP_OPERATION_PAUSE:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_OPERATION_PAUSE"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1487_112_2_18_2_7_50_124,(uint8*)"");
		MMIBT_AvrcpA2dpPause();
		break;

	case MMI_BT_A2DP_START_STREAM_IND:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_START_STREAM_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1492_112_2_18_2_7_50_125,(uint8*)"");
		MMIBT_A2dpStartStreamInd(msg_body);
		break;
		
	case MMI_BT_A2DP_SUSPEND_STREAM_IND:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_SUSPEND_STREAM_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1497_112_2_18_2_7_50_126,(uint8*)"");
		MMIBT_A2dpPauseStreamInd(msg_body);
		break;

	case MMI_BT_A2DP_RESUME_STREAM_IND:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_RESUME_STREAM_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1502_112_2_18_2_7_50_127,(uint8*)"");
		MMIBT_A2dpResumeStreamInd(msg_body);
		break;

	case MMI_BT_A2DP_CONNECTED:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_CONNECTED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1507_112_2_18_2_7_50_128,(uint8*)"");
		//MMIBT_A2dpLinkConnected(msg_body);
		MMIBT_OpenHeadsetCnf(msg_body, TRUE);
#ifdef ADULT_WATCH_SUPPORT
                     MAIN_SetIdleBlueToothState(ON_AND_CONNECT);

                     if(TRUE == MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
                     {
                          AdultWatchBt_ProcessBtPsMsg(msg_id, msg_body);
                     }
#endif

		break;
		
	case MMI_BT_A2DP_ABORT_IND:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_ABORT_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1513_112_2_18_2_7_50_129,(uint8*)"");
		MMIBT_A2dpGetAbortInd();
		break;
		
    case MMI_BT_A2DP_CONNECTED_IND:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_CONNECTED_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1518_112_2_18_2_7_50_130,(uint8*)"");
		MMIBT_OpenHeadsetConfirmIND(msg_body, TRUE);
		break;
	case MMI_BT_A2DP_DISCONNECTED:
		//SCI_TRACE_LOW:"MMI_BT_A2DP_DISCONNECTED"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1522_112_2_18_2_7_50_131,(uint8*)"");
		//MMIBT_A2dpLinkDisconnect(msg_body);
		//在设备close之前调用切换音频
        MMIBT_CloseHeadsetCnf(msg_body, TRUE);
        if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
        {
            WatchBtMain_ProcessBtPsMsg(msg_id, msg_body);
        }
#ifdef ADULT_WATCH_SUPPORT
        AdultWatch_RemoveHeadset(msg_body,TRUE);
        MAIN_SetIdleBlueToothState(ON_AND_DISCONNECT);
         if(TRUE == MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
         {
              AdultWatchBt_ProcessBtPsMsg(msg_id, msg_body);
         }
#endif

		break;
//end for a2dp message
#endif

#ifdef BT_SPP_SUPPORT
    case MMI_BT_SPP_DATA_RECIEVE_IND:
		SCI_TRACE_LOW("MMI_BT_SPP_DATA_RECIEVE_IND");
        MMIBT_SppDataRecieveInd(msg_body);
		break;

	case MMI_BT_SPP_DATA_RECIEVE_CFM:
		SCI_TRACE_LOW("MMI_BT_SPP_DATA_RECIEVE_CFM");
        //MMIAPIBT_SetSppWriteDataStatus(TRUE);	
		break;

    case MMI_BT_SPP_CONNECT_IND:
		SCI_TRACE_LOW("MMI_BT_SPP_CONNECT_IND");
        MMIBT_SppConnectInd(msg_body);
		break;

    case MMI_BT_SPP_DISCONNECT_IND:
		SCI_TRACE_LOW("MMI_BT_SPP_DISCONNECT_IND");
		//MMIBT_SPPDisconnectDeviceInd(msg_body);
		break;

    case MMI_BT_SPP_STATUS_IND:
		SCI_TRACE_LOW("MMI_BT_SPP_STATUS_IND");
        MMIBT_SppStatusInd(msg_body);
		break;
#endif

#if 0 //#ifdef BT_DIALER_SUPPORT

    case UAL_BT_MSG_HFU_SLC_CONNECTED:
        SCI_TRACE_LOW("[MMIBT] UAL_BT_MSG_HFU_SLC_CONNECTED");

		MMIBT_ConnectDeviceCnf(msg_body);
		BT_SetVisibility(BT_PAGE_ENABLE);//set blue tooth hidden
#ifdef BT_MAP_SUPPORT
        if(MMIAPISMS_GetBoxTotalNumber() == 0)
        {
            MMIAPISMS_SetCurOperIsDisconnect(FALSE);
        }
#endif
		break;
    case UAL_BT_MSG_HFU_SLC_DISCONNECTED:
		SCI_TRACE_LOW("[MMIBT] UAL_BT_MSG_HFU_SLC_DISCONNECTED");
#ifdef BT_MAP_SUPPORT
        MMIAPISMS_SetCurOperIsDisconnect(TRUE);
#endif    
		MMIBT_DisconnectDeviceCnf(msg_body);
        MMIAPICC_ReleaseBtCall();
        BT_SetVisibility((BT_INQUIRY_ENABLE | BT_PAGE_ENABLE));
        MMIAPIBT_CleanBtDialerSyncData();
		break;
#endif

#ifdef BT_PBAP_SUPPORT
	case MMI_BT_PBAP_TP_DISCONNECTED:
		SCI_TRACE_LOW("[MMIBT] MMI_BT_PBAP_TP_DISCONNECTED");
		MMIBT_PbapTpDisconnect(msg_body);
		break;

	case MMI_BT_PBAP_COMPLETE:
		SCI_TRACE_LOW("[MMIBT] MMI_BT_PBAP_COMPLETE");
		MMIBT_PbapComplete(msg_body);
		break;

	case MMI_BT_PBAP_ABORT:
		SCI_TRACE_LOW("[MMIBT] MMI_BT_PBAP_ABORT");
		MMIBT_PbapAbort(msg_body);
		break;

	case MMI_BT_PBAP_PARAMS_RX:
		SCI_TRACE_LOW("[MMIBT] MMI_BT_PBAP_PARAMS_RX");
		MMIBT_PbapParamsRx(msg_body);
		break;

	case MMI_BT_PBAP_DATA_IND:
		SCI_TRACE_LOW("[MMIBT] MMI_BT_PBAP_DATA_IND");
		MMIBT_PbapDataInd(msg_body);
		break;
#endif

#if defined(BT_MAP_SUPPORT)
    case MMI_BT_MAP_TP_DISCONNECTED:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_TP_DISCONNECTED");
        MMIBT_MapTpDisconnect(msg_body);
        break;    
        
    case MMI_BT_MAP_COMPLETE:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_COMPLETE");
        MMIBT_MapComplete(msg_body);
        break;    
        
    case MMI_BT_MAP_ABORT:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_ABORT");
        MMIBT_MapAbort(msg_body);
        break;
        
    case MMI_BT_MAP_CLIENT_PARAMS_RX:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_CLIENT_PARAMS_RX");
        MMIBT_MapParamsRx(msg_body);
        break;    
        
    case MMI_BT_MAP_SERVER_PARAMS_RX:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_SERVER_PARAMS_RX");
        break;
        
    case MMI_BT_MAP_DATA_IND:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_DATA_IND");
        MMIBT_MapDataInd(msg_body);
        break;
        
    case MMI_BT_MAP_MNS_SERVER_CONNECTED:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_MNS_SERVER_CONNECTED");
        break;
        
    case MMI_BT_MAP_MNS_SERVER_PARAMS_RX:
        SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_MNS_SERVER_PARAMS_RX");
        break;
        
    case MMI_BT_MAP_MNS_SERVER_DATA_IND:
        {
            SCI_TRACE_LOW("[MMIBT] MMI_BT_MAP_MNS_SERVER_DATA_IND");

#if defined(TRACE_MAP_BUFFER)
            uint16 len = 0;
            uint16 i;
            uint16 count = 0;
            uint16 buff_len = 0;

            buff_len = (uint16)(( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body->body_ptr)->len;
            SCI_TRACE_LOW("buff_len = %d", buff_len);

            for( i = 0; i < (buff_len/TRACE_MAP_BUF_COUNT_BYTE) +1;  i++ )
            {
                SCI_TRACE_LOW("[MMISMS] TRACE_MAP_BUF_COUNT_BYTE, i = %d", i);
                len = buff_len - i*TRACE_MAP_BUF_COUNT_BYTE;
                count = i*TRACE_MAP_BUF_COUNT_BYTE;
        
                if( len >= TRACE_MAP_BUF_COUNT_BYTE)
                {
                    SCI_TRACE_BUF("buff is :", ((( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body->body_ptr)->buff+count), TRACE_MAP_BUF_COUNT_BYTE);
                }
                else
                {
                    SCI_TRACE_BUF("buff is :", ((( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body->body_ptr)->buff+count), len);
                }
            }
#endif
            SCI_TRACE_LOW("msg_body status = %d", msg_body->status);
            MMIBT_MapServerDataInd(msg_body);
        }
        break;
#endif

#ifdef BT_AV_SNK_SUPPORT
	case MMI_BT_A2DP_SINK_CONNECTED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_A2DP_SINK_CONNECTED");
		MMIBT_HandleA2dpSinkConnected(msg_body);
		break;
	case MMI_BT_A2DP_SINK_DISCONNECTED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_A2DP_SINK_DISCONNECTED");
		MMIBT_HandleA2dpSinkDisconnected(msg_body);
		break;
	case MMI_BT_A2DP_SINK_STREAM_STARTED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_A2DP_SINK_STREAM_STARTED");
        MMIBT_HandleA2dpStreamStarted(msg_body);
		break;
	case MMI_BT_A2DP_SINK_STREAM_SUSPENDED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_A2DP_SINK_STREAM_SUSPENDED");
        MMIBT_HandleA2dpStreamSuspended(msg_body);
		break;
#endif

#ifdef BT_AV_CT_SUPPORT
	case MMI_BT_AVRCP_CT_CONNECTED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_CONNECTED");
		MMIBT_HandleAvrcpCtConnected(msg_body);
		break;
	case MMI_BT_AVRCP_CT_DISCONNECTED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_DISCONNECTED");
		MMIBT_HandleAvrcpCtDisconnected(msg_body);
		break;
	case MMI_BT_AVRCP_CT_MEDIA_ATTRIBUTES:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_MEDIA_ATTRIBUTES");
		MMIBT_HandleAvrcpMediaAttributes(msg_body);
		break;
	case MMI_BT_AVRCP_CT_PLAY_STATUS:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_PLAY_STATUS");
		MMIBT_HandleAvrcpPlayStatus(msg_body);
		break;
	case MMI_BT_AVRCP_CT_TRACK_CHANGED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_TRACK_CHANGED");
		MMIBT_HandleAvrcpTrackChanged(msg_body);
		break;
	case MMI_BT_AVRCP_CT_PLAYBACK_CHANGED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_PLAYBACK_CHANGED");
		MMIBT_HandleAvrcpPlaybackChanged(msg_body);
		break;
	case MMI_BT_AVRCP_CT_PLAYBACK_POS_CHANGED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_PLAYBACK_POS_CHANGED");
		MMIBT_HandleAvrcpPlayPositionChanged(msg_body);
		break;
	case MMI_BT_AVRCP_CT_PLAYER_APPLICATION_CHANGED:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_PLAYER_APPLICATION_CHANGED");
		MMIBT_HandleAvrcpPlayerApplicationChanged(msg_body);
		break;
	case MMI_BT_AVRCP_CT_SUPPORTED_PLAYER_ATTRIBUTES:
		SCI_TRACE_LOW("BT_AV, MMI_BT_AVRCP_CT_SUPPORTED_PLAYER_ATTRIBUTES");
		MMIBT_HandleAvrcpSupportedPlayerAttributes(msg_body);
		break;
#endif

#ifdef BT_HID_SUPPORT
	case MMI_BT_HID_CONNECT_IND:
		//SCI_TRACE_LOW("BT_HID, MMI_BT_HID_CONNECT_IND");
		MMIBT_HandleHIDConnected(msg_body);
		break;

	case MMI_BT_HID_DISCONNECT_IND:
		//SCI_TRACE_LOW("BT_HID, MMI_BT_HID_DISCONNECT_IND");
		break;
#endif
#ifdef BT_BLE_SUPPORT
    /*BLE Begin */
    case MMI_BT_BLE_CONNECTED:
        SCI_TRACE_LOW("[BLE Test] MMI_BT_BLE_CONNECTED");
        MMIENGBT_HandleBLEMsgProcess(msg_id,msg_body);
        break;
    case MMI_BT_BLE_DISCONNECTED:
        SCI_TRACE_LOW("[BLE Test] MMI_BT_BLE_DISCONNECTED");
        MMIENGBT_HandleBLEMsgProcess(msg_id,msg_body);
        break;
    case MMI_BT_BLE_DATA_IND:
        SCI_TRACE_LOW("[BLE Test] MMI_BT_BLE_DATA_IND");
        MMIENGBT_HandleBLEMsgProcess(msg_id,msg_body);
        break;
    /*BLE End*/
#endif
	default:
				//SCI_TRACE_LOW:"MMI_BT   default"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1530_112_2_18_2_7_50_132,(uint8*)"");
		break;
	}

	//Free msg space
    if (msg_body != PNULL && msg_body->body_ptr != PNULL && msg_body->body_size >0)
	{
		#ifdef WIN32
		SCI_FREE(msg_body->body_ptr);
		#else		
		OS_Pfree(msg_body->body_ptr); /*lint !e718 */
		#endif
		msg_body->body_ptr = PNULL;
	}
	return result;
}
/*****************************************************************************/
//  Discription: This function is used to send signal from blue tooth to mmi
//  recive the bt app response 
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  void MMIBT_SendSignal(uint32 sig_id,DPARAM param)
{
	BT_SIGNAL *signal = PNULL;

	//SCI_TRACE_LOW:"mmibt.c: MMIBT_SendSignal:sig_id=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1126_112_2_18_2_7_49_59,(uint8*)"d", sig_id);
    //SCI_ASSERT(PNULL != param);/*assert verified*/
    if(PNULL == param)
    {
        return;
    }
    
	MmiCreateSignal(sig_id, sizeof(BT_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_MEMCPY(&(signal->content), param, sizeof(BT_MSG_T));

	MmiSendSignal(P_APP, (MmiSignalS*)signal);
}
