/*****************************************************************************
** File Name:      mmi_sd.c                                                  *
** Author:                                                                   *
** Date:           07/2006                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe sd card                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007       Liqing.Peng       Creat                                     *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_udisk_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MSDC_CARD_SUPPORT
//#include "sci_types.h"
#include "scm_api.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmi_id.h"
#include "mmiacc_text.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmibt_export.h"
#include "mmimp3_export.h"
#include "mmimms_export.h"
#include "mmiota_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
//#include "mmiebook.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
//#include "mmimultim.h"
#include "mmifmm_export.h"
#include "mmiset_export.h"
#include "mmidc_export.h"
#include "mmipic_edit_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_modu_main.h"
#include "mmi_default.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmisd_export.h"
#include "mmisd_internal.h"
#include "mmiudisk_export.h"
#include "mmk_app.h"
//#include "mmk_modinfo.h"
#include "mmidc_setting.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmipicview_export.h"
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#include "mmifmm_id.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "scm_api.h"
#ifdef FM_SUPPORT
#include "mmifm_export.h"
#endif
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
//#include "mmiset_wintab.h"
#endif
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif

#ifdef MMIEMAIL_SUPPORT//MS00217073
#include "mmiemail_export.h"
#endif

#ifdef MMI_MEMO_SUPPORT
#include "mmimemo_export.h"
#endif
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
#ifdef WRE_SUPPORT
#include "mmiwre_other.h"
#endif /* WRE_SUPPORT */
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif /* MET_MEX_SUPPORT */
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmicc_export.h"
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmipb_export.h"
#include "mmicc_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN           s_delayed_devices[SCM_MAX_SLOT_NO] = {FALSE}; //是否需要延时处理sd卡插拔消息
LOCAL MMI_MESSAGE_ID_E  s_delayed_devices_msg[SCM_MAX_SLOT_NO] = {0};    //记录上一次插拔的消息
LOCAL BOOLEAN           s_sdplug_down_devices[SCM_MAX_SLOT_NO] = {FALSE};       //记录SD卡按键状态
//LOCAL BOOLEAN           s_is_file_wallpaper = FALSE;  //记录拔出sd卡墙纸是否是文件

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern void MMIDEFAULT_SetAlwaysHalfOnBackLight(BOOLEAN b_half_on);

// extern void MMIAPIWRE_ExitWre(); // extern wre_boot\mmiwre_startup.c
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init sd card or stop sd card using
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
/*LOCAL BOOLEAN InitSDOrStopSD(
                             MMI_MESSAGE_ID_E msg_id
                             );*/

/*****************************************************************************/
//  Description : register or unregister file system of this sd card
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RegisterOneFileSystem(
							 SCM_SLOT_NAME_E slot_type,
                             BOOLEAN         is_register//IN:True:register; FALSE:unregister
                             );

/*****************************************************************************/
//  Description : 处理 SD 卡插入拔出
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleSDPlug(
						SCM_SLOT_NAME_E slot_type,
                        MMI_MESSAGE_ID_E plug_msg_id
                        );

/*****************************************************************************/
//  Description : update app
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
// LOCAL void UpdateAppAfterSDPlug(
//                                 MMI_MESSAGE_ID_E msg_id
//                                 );

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 处理 SD 卡插入拔出
//  Global resource dependence : 
//  Author: louis.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_HandleSDPlug(
								  void             *param_ptr, 
								  MMI_MESSAGE_ID_E plug_msg_id
								  )
{
#ifndef MAINLCD_SIZE_128X64
//#ifndef WIN32
#ifdef MSDC_CARD_SUPPORT
    SCM_STATUS scmstatus = SCM_STATUS_NORMAL;
	SCM_SLOT_NAME_E scm_name = SCM_SLOT_0;
	MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;

	if(param_ptr != PNULL)
	{
		scm_name = *(SCM_SLOT_NAME_E *)param_ptr;
	}
    if(MSG_HANDLE_SD_ERR == plug_msg_id)
	{
		//sd 卡出错，直接提示返回，不需要后续处理，也不需要延长
		MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(scm_name), FALSE);
		MMIAPISD_StopCurOperation(MSG_HANDLE_SD_ERR);
		device_type = MMISD_GetSDCardType(scm_name);

        if (MMIAPIFMM_GetDeviceTypeStatus(device_type))//if SD Card exist physical must shut down fs_mmc file system
        {
            MMIAPIFMM_UnRegisterDevice(MMIAPIFMM_GetDevicePath(device_type),MMIAPIFMM_GetDevicePathLen(device_type));
        }
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_SD_FAIL);
		return;
	}

//1@added by charlie for compile error 2011.02.10	
#ifndef WIN32
    scmstatus = SCM_GetSlotStatus(scm_name);
#else
    scmstatus = SCM_STATUS_NORMAL;
#endif

    switch(scmstatus)
    {
    case SCM_STATUS_NORMAL:
        MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(scm_name), TRUE);
        break;
    case SCM_NOT_EXIST:
    case SCM_STATUS_ERROR:
        MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(scm_name), FALSE);    
        break;
    default:
        //SCI_TRACE_LOW:"MMIAPISD_HandleSDPlug scmstatus = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_211_112_2_18_3_4_2_14,(uint8*)"d",scmstatus);
        MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(scm_name), FALSE);    
        return;
    }  
#endif
    //SCI_TRACE_LOW:"mmi_sd_plug.c: MMIAPISD_HandleSDPlug, plug_msg_id=%d,scmstatus = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_216_112_2_18_3_4_2_15,(uint8*)"dd", plug_msg_id,scmstatus);
//#endif

    if(s_delayed_devices[scm_name])
    {
        SCI_TRACE_LOW("mmi_sd_plug.c: MMIAPISD_HandleSDPlug, ignore all"); //还有需要延时处理的消息未处理
        return;
    }
    
    if (MSG_KEYDOWN_SDCARD_DETECT == plug_msg_id)
    {
         if(s_sdplug_down_devices[scm_name]) //sd卡已经插入无需处理该消息
        {
             //SCI_TRACE_LOW:"mmi_sd_plug.c: MMIAPISD_HandleSDPlug MSG_KEYDOWN_SDCARD_DETECT, ignore"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_228_112_2_18_3_4_2_16,(uint8*)"");
             return;
        }
        else
        {
#ifndef WIN32
            if(SCM_NOT_EXIST== scmstatus)
            {              
                return;
            }
#endif
            s_sdplug_down_devices[scm_name] = TRUE;
        }
    }
    else if (MSG_KEYUP_SDCARD_DETECT == plug_msg_id)
    {
        if(!s_sdplug_down_devices[scm_name]) //sd卡已经拔出，无需处理该消息
        {
            //SCI_TRACE_LOW:"mmi_sd_plug.c: MMIAPISD_HandleSDPlug MSG_KEYUP_SDCARD_DETECT, ignore"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_246_112_2_18_3_4_2_17,(uint8*)"");
            return;
        }
        else
        {     
            s_sdplug_down_devices[scm_name] = FALSE;
        }
    }
    else
    {
         return;
    }
    
    MMIDEFAULT_TurnOnBackLight();

    //s_need_handle_sdplug = FALSE;
    //s_last_sdplug_msg = 0;
    s_delayed_devices[scm_name] = FALSE;
    s_delayed_devices_msg[scm_name] = 0;
/*
    if(MMIAPIUDISK_UdiskIsRun())
    {
          //U盘使用中
        InitSDOrStopSD(plug_msg_id);
		
    }
    else 
*/
    if(MMIAPIUDISK_UdiskIsRun())
    {
          //U盘使用中
// 		if(MSG_KEYUP_SDCARD_DETECT == plug_msg_id)
// 		{
// 			RegisterOneFileSystem(scm_name, FALSE);
// 		}
// 		else
// 		{
// 			RegisterOneFileSystem(scm_name, TRUE);
// 		}
    }
    else if(MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID))
    {
        //开机初始化没有结束，sd卡拔插需要延迟处理
        s_delayed_devices[scm_name] = TRUE;
        s_delayed_devices_msg[scm_name] = plug_msg_id;
    }
    else
    {
        //正常处理SD卡插拔
        HandleSDPlug(scm_name, plug_msg_id);
    }
#endif
}


/*****************************************************************************/
//  Description : Is need handle SD plug msg
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
// PUBLIC BOOLEAN  MMIAPISD_NeedHandleSDPlug(void)
// {
//     SCI_TRACE_LOW("MMIAPISD_NeedHandleSDPlug, s_need_handle_sdplug = %d", s_need_handle_sdplug);
//     return s_need_handle_sdplug;
// }

/*****************************************************************************/
//  Description : handle SD plug msg
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIAPISD_ReHandleSDPlug(void)
// {
//     SCI_TRACE_LOW("MMIAPISD_ReHandleSDPlug, s_last_sdplug_msg = %d", s_last_sdplug_msg);
//     
//     HandleSDPlug(s_last_sdplug_msg);
//     
//     s_need_handle_sdplug = FALSE;
//     s_last_sdplug_msg = 0;
// }
PUBLIC void MMIAPISD_CheckAllDelayedDevice(void)
{
	SCM_SLOT_NAME_E slot_type = SCM_SLOT_0;

    //SCI_TRACE_LOW:"MMIAPISD_CheckAllDelayedDevice, enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_327_112_2_18_3_4_2_18,(uint8*)"");
    
    for(slot_type = SCM_SLOT_0; slot_type < MMI_SDCARD_MAX_NUM; slot_type++)
	{
		if(s_delayed_devices[slot_type])
		{
			//SCI_TRACE_LOW:"MMIAPISD_CheckAllDelayedDevice,s_delayed_devices_msg[%d] 0x%x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_333_112_2_18_3_4_2_19,(uint8*)"dd", slot_type, s_delayed_devices_msg[slot_type]);
			//need re-load this device file system
			HandleSDPlug(slot_type, s_delayed_devices_msg[slot_type]);
			s_delayed_devices_msg[slot_type] = 0;
			s_delayed_devices[slot_type] = FALSE;
		}


	}	
}
/*****************************************************************************/
//  Description : 处理 SD 卡插入拔出
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleSDPlug(
						SCM_SLOT_NAME_E  slot_type,
                        MMI_MESSAGE_ID_E plug_msg_id
                        )
{
    MMI_STRING_T    wait_text   =   {0};
    MMI_WIN_ID_T    win_id      =   MAIN_SD_WAIT_WIN_ID;
    MMI_MESSAGE_ID_E post_msg_id = MSG_HANDLE_SDPLUG_IN_STEP_1;

    //SCI_TRACE_LOW:"mmi_sd_plug.c: HandleSDPlug, plug_msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_356_112_2_18_3_4_2_20,(uint8*)"d", plug_msg_id);
    MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
  
    
    MMIPUB_CloseAlertWin();  
    
    MMK_CloseWin(win_id);  
    
    if(MSG_KEYDOWN_SDCARD_DETECT == plug_msg_id)
    {
		//sd plug in, need 3 steps to dealwith this action
        MMI_GetLabelTextByLang(TXT_SD_DETECTED, &wait_text);
		post_msg_id = MSG_HANDLE_SDPLUG_IN_STEP_1;
    }
    else if (MSG_KEYUP_SDCARD_DETECT == plug_msg_id)
    {
		//sd plug out, need 3 steps to dealwith this action
        MMI_GetLabelTextByLang(TXT_SD_PULLOUT, &wait_text);
		post_msg_id = MSG_HANDLE_SDPLUG_OUT_STEP_1;
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    }
    
    MMIPUB_OpenWaitWin(
        1,
        &wait_text,
        PNULL,
        PNULL,
        win_id,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL, //WIN_THREE_LEVEL,
        MMIPUB_SOFTKEY_NONE,
        MMIAPISD_HandleSDPlugWaiting
        );

    MMK_PostMsg(win_id, post_msg_id, &slot_type, sizeof(slot_type));
    //MMK_PostMsg(VIRTUAL_WIN_ID, MSG_HANDLE_SDPLUG_STEP_1, &plug_msg_id, sizeof(plug_msg_id));
    //MMIAPICOM_TriggerMMITask();
}

/*****************************************************************************/
//  Description : 处理等待窗口
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISD_HandleSDPlugWaiting (
                                        MMI_WIN_ID_T       win_id,  //窗口的ID
                                        MMI_MESSAGE_ID_E    msg_id,     //内部消息ID
                                        DPARAM             param        //相应消息的参数
                                        )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    //MMI_MESSAGE_ID_E        plug_msg = 0;
    //MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
    SCM_SLOT_NAME_E        scm_type = SCM_SLOT_0;
	MMI_MESSAGE_ID_E       post_msg_id = 0;
	MMI_WIN_ID_T           alert_id = MMIFMM_SD_ALERT_WIN;
	MMI_TEXT_ID_T          text_id = TXT_NULL;
	BOOLEAN                is_plug_in = FALSE;

	
    switch(msg_id )
    {
    case MSG_APP_RED:
        break;
        
    case MSG_HANDLE_SDPLUG_IN_STEP_1:
	case MSG_HANDLE_SDPLUG_OUT_STEP_1:
		//plug in/plug out step 1: Register or unregister this device file system
        if(param != PNULL)
		{
			scm_type = *(SCM_SLOT_NAME_E *)param;
		}
        //SCI_TRACE_LOW:"mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_1, scm_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_431_112_2_18_3_4_2_21,(uint8*)"d", scm_type);
        if(msg_id == MSG_HANDLE_SDPLUG_OUT_STEP_1)
        {
			is_plug_in = FALSE;
			post_msg_id = MSG_HANDLE_SDPLUG_OUT_STEP_2;
#ifndef CMCC_UI_STYLE
            MMIAPIMMS_ListInit();
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_InitOTAPUSH();
#endif
#endif
        }
		else
		{
			is_plug_in = TRUE;
			post_msg_id = MSG_HANDLE_SDPLUG_IN_STEP_2;
		}
        //初始化SD卡或者停止使用SD卡
        if (!RegisterOneFileSystem(scm_type, is_plug_in))
        {
            //SD卡初始化失败
           // MMIPUB_OpenAlertTextWinByTextId(PNULL,TXT_COMMON_SD_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
           MMIPUB_OpenAlertFailWin(TXT_COMMON_SD_FAIL);
		   MMK_CloseWin(win_id);
        }
        else
		{
			//执行下一步
			MMK_PostMsg(win_id, post_msg_id, &scm_type, sizeof(scm_type));
		}
        break;
        
    case MSG_HANDLE_SDPLUG_IN_STEP_2:
	case MSG_HANDLE_SDPLUG_OUT_STEP_2:
        if(param != PNULL)
		{
			scm_type = *(SCM_SLOT_NAME_E *)param;
		}        
        //SCI_TRACE_LOW:"mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_2, scm_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_465_112_2_18_3_4_2_22,(uint8*)"d", scm_type);
        //plug in/plug out step 2: stop all applications about FS: such as: mp3, fmm,ebook, java ect.
		if(MSG_HANDLE_SDPLUG_IN_STEP_2 == msg_id)
		{
			MMIAPISD_StopCurOperation(MSG_KEYDOWN_SDCARD_DETECT);
			post_msg_id  = MSG_HANDLE_SDPLUG_IN_STEP_3;
		}
		else
		{
			MMIAPISD_StopCurOperation(MSG_KEYUP_SDCARD_DETECT);
			post_msg_id  = MSG_HANDLE_SDPLUG_OUT_STEP_3;
		}



#ifdef DYNAMIC_MODULE_SUPPORT
        /* begin 这里要停掉所有关于applet的信息。*/
        MMK_CloseAllDynamicApplet();
        MMK_UnLoadAllDynModInfo();
        /* end*/
#endif
        //执行下一步
        MMK_PostMsg(win_id, post_msg_id, &scm_type, sizeof(scm_type));
        //MMIAPICOM_TriggerMMITask();

        break;
        
    case MSG_HANDLE_SDPLUG_IN_STEP_3:
	case MSG_HANDLE_SDPLUG_OUT_STEP_3:
        if(param != PNULL)
		{
			scm_type = *(SCM_SLOT_NAME_E *)param;
		}
        //SCI_TRACE_LOW:"mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_3, scm_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_498_112_2_18_3_4_2_23,(uint8*)"d", scm_type);
        //plug in/plug out step 3              
        
        if(msg_id == MSG_HANDLE_SDPLUG_IN_STEP_3)
        {
			//plug in
			text_id = TXT_COM_SD_PLUGIN;
			//create all default folder in this sd card
			MMISD_CreateDefaultFold(MMISD_GetSDCardType(scm_type));
#ifndef NANDBOOT_SUPPORT 
			//reset bt ftp shared folder
#ifdef BLUETOOTH_SUPPORT
			MMIAPIBT_SaveFileLocation((MMISD_GetSDCardType(scm_type)));
			MMIAPIBT_SetFtpRootFolder((MMISD_GetSDCardType(scm_type)));
#endif
#endif
#ifndef CMCC_UI_STYLE
            MMIAPIMMS_ListInit();
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_InitOTAPUSH();
#endif
#endif
        } 
		else
		{
			text_id = TXT_COM_SD_PLUGOUT;
		}
#ifdef DYNAMIC_MODULE_SUPPORT
        /* begin u盘拔除之后重新load 模块信息 */
        MMK_LoadDynamicModInfo();
        /* end */
#endif
		//show msg
        MMIPUB_OpenAlertWinByTextId(
            PNULL,
            text_id,
            TXT_NULL,
            IMAGE_PUBWIN_WARNING,
            &alert_id,
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL
            );
        MMK_CloseWin(win_id);
        
        break;
        
    default:
        MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 停止当前的操作
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_StopCurOperation(
                            MMI_MESSAGE_ID_E msg_id
                            )
{
    //MMISET_WALLPAPER_SET_T      wallpaper_info = {0};

    //SCI_TRACE_LOW:"mmi_sd_plug.c: StopCurOperation enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_557_112_2_18_3_4_2_24,(uint8*)"");
    
    //停止当前的操作
    
    switch(msg_id)
    {
        //sd plug in message
    case MSG_KEYDOWN_SDCARD_DETECT:
        //停止播放来电铃声、短消息铃声或者闹钟铃声
        //StopAppRing();
#ifdef TCARD_PB_SUPPORT
        if(MMIAPIPB_IsTCardContactExist())
        {
        
            MMIAPIPB_ResortPB();
        }
#endif

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
        MMIAPISET_SetFont( PNULL, FALSE, FALSE );
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT      
        //停止Mp3背景播放
        MMIAPIMP3_ResetAllMp3();
#endif   
#ifdef KURO_SUPPORT
        MMIAPIKUR_ResetAllKur();
#endif

#ifdef  MUSIC_PLAYER_SUPPORT
        // close all music win
        MMIAPIMP3_CloseMyDocumentMusicPlay();
#endif

        //退出图片、音乐、视频文件列表选择窗口
		MMIAPIFMM_CloseSelectFileWin();
        
        //Mp4
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
        MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
        MMIAPIVP_ExitVideoPlayer();        
#endif   
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMIAPISETVWP_ResetAllVideoIsValid();
#endif
#endif

#ifdef MOILE_VIDEO_SUPPORT
        MMIAPIMV_Exit();
#endif
#ifdef EBOOK_SUPPORT
        //电子书
        MMIAPIEBOOK_StopOperaSDPlug();
#endif
        
        //Java
#ifdef JAVA_SUPPORT
        MMIAPIJAVA_StopOperaSDPlug();
#endif
        //停止资源管理器
        MMIAPIFMM_StopOperaSDPlug();
        //停止图片编辑
        MMIAPIPIC_ExitFromPicEditor();
        
        //停止图片浏览器
        MMIAPIPICVIEW_ClosePicViewer();

        //ATV
        MMIAPIATV_Exit();
        
        //MMIDC_CloseStoreWin();
#ifdef SEARCH_SUPPORT
        //处理本地搜索列表中的文件项
        MMIAPISEARCH_AppUpdateInd(MMISEARCH_TYPE_FILE);
#endif
#ifndef NANDBOOT_SUPPORT
#ifdef BROWSER_SUPPORT//MS00217073
        MMIAPIBROWSER_Close();
#endif
#endif
#ifdef MMIEMAIL_SUPPORT
        MMIAPIEMAIL_SyncEmail();
#endif
        break;
        
        //sd plug out message
    case MSG_KEYUP_SDCARD_DETECT:
	case MSG_HANDLE_SD_ERR:
		//12C bug62933,70391
//#ifdef TCARD_PB_SUPPORT
 //       if(MMIAPIPB_IsTCardContactExist())
 //       {
        
            MMIAPIPB_ResortPB();
            MMIAPIPB_AbortPB();
//        }
//#endif

//        //get wallpaper info
//        MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
//
//        switch (wallpaper_info.wallpaper_type)
//        {
//        case MMISET_WALLPAPER_ALBUM:
//        case MMISET_WALLPAPER_DOWNLOAD:
//            s_is_file_wallpaper = TRUE;
//
//            //reset wallpaper
//            MMIAPISET_ResetDefaultWallpaper();
//            break;
//        
//        default:
//            s_is_file_wallpaper = FALSE;
//            break;
//         }

        //停止播放来电铃声、短消息铃声或者闹钟铃声
#ifdef WRE_SUPPORT
        MMIAPIWRE_ExitWre();
#endif // WRE_SUPPORT

#ifdef MCARE_V31_SUPPORT
        McareV31_ExitMcare();
#endif
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
        MMIAPISET_SetFont( PNULL, TRUE, FALSE );
#endif
#ifdef KURO_SUPPORT
        MMIAPIKUR_ResetAllKur();
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT     
        //停止Mp3背景播放
        MMIAPIMP3_ResetAllMp3();
#endif
#ifdef  MUSIC_PLAYER_SUPPORT
        // close all music win
        MMIAPIMP3_CloseMyDocumentMusicPlay();
#endif
        //图片、音乐、视频文件列表选择窗口
//        MMIAPIMULTIM_CloseSelectPictureWin();
 //       MMIAPIFMM_CloseSelectMusicWin();
 //       MMIAPIFMM_CloseSelectMovieWin();
		  MMIAPIFMM_CloseSelectFileWin();
        
        //我的文档
//        MMIAPIMULTIM_StopOperaSDPlug();
#ifdef EBOOK_SUPPORT
        //电子书
        MMIAPIEBOOK_StopOperaSDPlug();
#endif
        //Java
#ifdef JAVA_SUPPORT
        MMIAPIJAVA_StopOperaSDPlug();
#endif
        
        //SD卡管理器
        MMIAPISD_StopOperaSDPlug();
#ifdef MMI_RECORD_SUPPORT        
        //停止录音
        MMIAPIRECORD_Exit();
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
		MMIAPICC_StopCCRecord();
#else
        MMICC_StopRecordInCall();
#endif
        MMIENG_StopENGRecord();
#endif
#ifdef MET_MEX_SUPPORT
        MMIMEX_RecordStop();
#endif
#ifdef FM_SUPPORT
        MMIAPIFMRECORD_Exit();
#endif  
        //停止拍照
        MMIAPIDC_Exit();
        
        //Mp4
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
        MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
        MMIAPIVP_ExitVideoPlayer();
#endif
#endif

#ifdef MOILE_VIDEO_SUPPORT
        MMIAPIMV_Exit();
#endif
        //停止资源管理器
        MMIAPIFMM_StopOperaSDPlug();
		//help vp and audio to close detail win
		MMIAPIFMM_CloseOptionWin();

        //停止图片编辑
        MMIAPIPIC_ExitFromPicEditor();
        
        //停止图片浏览器
        MMIAPIPICVIEW_ClosePicViewer();

		MMIAPISET_CloseValidatePrivacyWin(MMISET_PROTECT_MYDOC);
#ifdef EBOOK_SUPPORT
    	MMIAPISET_CloseValidatePrivacyWin(MMISET_PROTECT_EBOOK);
#endif
	    MMIAPISET_CloseValidatePrivacyWin(MMISET_PROTECT_USB);
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    	MMIAPISET_CloseValidatePrivacyWin(MMISET_PROTECT_AUDIO_PLAYER);
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    	MMIAPISET_CloseValidatePrivacyWin(MMISET_PROTECT_VIDEO_PLAYER);
#endif
#endif
        //ATV
#ifdef ATV_SUPPORT
        MMIAPIATV_Exit();
#endif
#ifdef SEARCH_SUPPORT
        //处理本地搜索列表中的文件项
        MMIAPISEARCH_ExitSearching(MMISEARCH_TYPE_FILE);
#endif
#ifndef NANDBOOT_SUPPORT
#ifdef BROWSER_SUPPORT//MS00217073
        MMIAPIBROWSER_Close();
//#ifdef BROWSER_SUPPORT_DORADO
        MMIAPIBROWSER_SDCardPlugOut();
//#endif        
#endif
#endif        

#ifdef MMIEMAIL_SUPPORT
        MMIAPIEMAIL_SyncEmail();
#endif
	#ifdef ASP_SUPPORT
        //ASP
        MMIAPIASP_SDCardPlugOut();
    #endif 

#ifdef MMI_MEMO_SUPPORT
        MMIAPIMEMO_Exit();
#endif    
        break;
        
    default:
        break;
    }
#ifdef MMI_RECORD_SUPPORT
    MMIAPIRECORD_CloseMenu();
#endif
#ifdef BLUETOOTH_SUPPORT
	MMIAPIBT_CloseStorageMenu();
#endif
#ifdef  CAMERA_SUPPORT
	MMIAPIDC_CloseStorageMenu();
#endif
    //SCI_TRACE_LOW:"mmi_sd_plug.c: StopCurOperation exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_752_112_2_18_3_4_3_25,(uint8*)"");
}

/*****************************************************************************/
//  Description : register or unregister file system of this sd card
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RegisterOneFileSystem(
							 SCM_SLOT_NAME_E slot_type,
                             BOOLEAN         is_register//IN:True:register; FALSE:unregister
                             )
{
    BOOLEAN is_success = FALSE;
    
    //SCI_TRACE_LOW:"mmi_sd_plug.c: RegisterOneFileSystem enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_766_112_2_18_3_4_3_26,(uint8*)"");
	if(is_register)
	{
		//初始化SD卡，注册文件系统
        if(TRUE == MMIAPISD_IsSDPlugIn(MMISD_GetSDCardType(slot_type)))
        {
            if (MMIAPIENG_GetIsCardLogOn()
                && !MMIAPISD_IsUsbLogOn())
            {
                is_success = MMIAPISD_OpenCardLog();
            }
            else
            {
                is_success = MMISD_InitSDCard(slot_type);
            }
            //SCI_TRACE_LOW:"RegisterOneFileSystem: MMISD_InitSDCard is_success = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_781_112_2_18_3_4_3_27,(uint8*)"d", is_success);
        }
	}
	else
	{
        //退出SD卡使用，unregister file system

        if (MMIAPIENG_GetIsCardLogOn()&& !MMIAPISD_IsUsbLogOn())
        {
			is_success = MMIAPISD_CloseCardLog();
        }
        else
        {
			is_success = MMIAPISD_StopSDCard(MMISD_GetSDCardType(slot_type)); 
        }        
        //SCI_TRACE_LOW:"RegisterOneFileSystem: MMIAPISD_StopSDCard is_success = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_796_112_2_18_3_4_3_28,(uint8*)"d", is_success);
	}
    
    //SCI_TRACE_LOW:"mmi_sd_plug.c: RegisterFileSystem exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_799_112_2_18_3_4_3_29,(uint8*)"");

    return is_success;
}

/*****************************************************************************/
//  Description : SD 卡热插拔初始化
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMISD_SDPlugInit(
                      BOOLEAN   is_sd_in,
					  uint32    scm_slot
                      )
{
	//SCM_SLOT_NAME_E slot_type = SCM_SLOT_0;

    //SCI_TRACE_LOW:"MMISD_SDPlugInit, is_sd_in = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_PLUG_815_112_2_18_3_4_3_30,(uint8*)"d", is_sd_in);
	if(scm_slot < SCM_MAX_SLOT_NO)
	{
		s_delayed_devices[scm_slot] = FALSE;
		s_delayed_devices_msg[scm_slot] = 0;

		s_sdplug_down_devices[scm_slot] = is_sd_in;
	}
}
#endif
