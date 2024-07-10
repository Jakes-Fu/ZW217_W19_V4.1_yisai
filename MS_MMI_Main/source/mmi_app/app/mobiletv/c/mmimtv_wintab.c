/*****************************************************************************
** File Name:      mmimtv_wintab.c                                           *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv window table            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#ifdef CMMB_SUPPORT
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "mmk_type.h"
#include "guistatusbar.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guiwin.h"
#include "mmiudisk_export.h"
//#include "mmieng_uitestwin.h"
#include "mmieng_export.h"
//#include "mmiaudio_ctrl.h"
#include "mmimtv_export.h"
#include "mmimtv_id.h"
#include "mmimtv_image.h"
#include "mmimtv_main.h"
#include "mmimbbms_main.h"
#include "mmimtv_position.h"
#include "mmimtv_osd_display.h"
#include "mmi_appmsg.h"
#include "lcd_backlight.h"
#include "chng_freq.h"
#include "freq_cfg.h"
#include "mmiphone_export.h"

#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          #win_id

LOCAL const uint8 mmimtv_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "mmimtv_id.def"    
};
#undef WIN_ID_DEF


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E	msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(void);

/*****************************************************************************/
//  Description : init display param 
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitDisplayParam(void);


/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
LOCAL WINDOW_TABLE(MMIMTV_MAIN_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleMainWinMsg),
    WIN_ID(MMIMTV_MAIN_WIN_ID),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_90|WIN_SUPPORT_ANGLE_270 ),
    //CREATE_SOFTKEY_CTRL(TXT_NULL, TXT_NULL, TXT_NULL, GUISOFTKEY_STYLE_NULL, MMICOMMON_SOFTKEY_CTRL_ID),
    END_WIN
};



/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : open MTV player
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_OpenMTVPlayer(void)
{
    //if(!MMIMTV_IsMTVOpen())
    //{
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_MTV);
#endif    
        //GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,LCD_DIRECT_ROT_90);
        return MMK_CreateWin((uint32*)MMIMTV_MAIN_WIN_TAB, (ADD_DATA)PNULL);
    //}
    //else
    //{
    //    return FALSE;
    //}
}

/*****************************************************************************/
// 	Description : 供MBBMS使用
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_OpenMTVPlayerForMBBMS(void)
{
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_MTV);
#endif 
//    GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,LCD_DIRECT_ROT_90);
    return MMK_CreateWin((uint32*)MMIMTV_MAIN_WIN_TAB, (ADD_DATA)PNULL);
}

/*****************************************************************************/
// 	Description : register MTV win id name array
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_MTV, (const uint8**)mmimtv_id_name_arr);
}

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(
                                    MMI_WIN_ID_T        win_id, //window id
                                    MMI_MESSAGE_ID_E	msg_id, //the identify of message
                                    DPARAM              param   //the param of the message
                                    )
{
    MMI_RESULT_E recode	= MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO dev_info = {0};
    AUDIO_DEVICE_MODE_TYPE_E audio_mode = 0;
    GUI_BG_T  bg_info = {0};
    GUI_BOTH_RECT_T both_rect = {{0, 0, MMIMTV_LCD_HEIGHT - 1, MMI_STATUSBAR_HEIGHT-1}, {0, 0, MMIMTV_LCD_HEIGHT - 1, MMI_STATUSBAR_HEIGHT-1}};
    MMISRV_HANDLE_T audio_handle = MMIMTV_GetAudioHandle();
    MMIMTV_VOLUME_E ring_vol     = MMIMTV_GetVolume();
    
	msg_id = MMIAPICOM_ConvertKeyMsg(msg_id);
    //SCI_TRACE_LOW("[MMIMTV]: HandleMainWinMsg, msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_TurnOnBackLight(); 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        
        if(0 == audio_handle)
        {
            MMK_CloseWin(win_id);
            recode = MMI_RESULT_FALSE;
        }

        GUIWIN_CreateStbDyn(win_id);
        //GUIWIN_SetStbBgIsDesplay(win_id, FALSE)
        GUIWIN_SetStbBothRect(win_id, &both_rect);
        bg_info.bg_type = GUI_BG_COLOR;
        bg_info.color = MMIMTV_TRANSPARENT_COLOR;
        GUIWIN_SetStbBg(win_id, bg_info);
        
        //init osd block and display etc
        SetBlockType();
        InitDisplayParam();
        
        //set audio etc
        //MMIAPISET_GetDeviceMode(&audio_mode);
		if(MMIMTV_IsMute())
		{
            CMMB_Mute();
	        //MMIAUDIO_SetMute(TRUE);
	        ring_vol = 0;
		}	
		else
		{
			CMMB_UnMute();		
        	//MMIAUDIO_SetMute(FALSE);
		}
		if((ring_vol != MMISRVAUD_GetVolume(audio_handle)) && (audio_handle > 0))
		{
		    MMISRVAUD_SetVolume(audio_handle, ring_vol);
		}
        //MMIAUDIO_SetVolume(audio_mode, MMIMTV_GetVolume());
        MMIMTV_SetBrightness(MMIMTV_GetBrightness());
        
        if(MMIMBBMS_IsNeedPlayService())
        {
            MMIMBBMS_SetIsNeedPlayService(FALSE);
            
            //连接等待场景
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PLAY_CONNECTING);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
            MMIMBBMS_StartPlay();
        }
        else
        {
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        break;

//     case MSG_LCD_SWITCH:
//         if(LCD_DIRECT_NORMAL == GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID))
//         {
//             GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,LCD_DIRECT_ROT_90);
//         }
//         else if(LCD_DIRECT_ROT_180 == GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID))
//         {
//             GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,LCD_DIRECT_ROT_270);
//         }
//         break;
    case MSG_FULL_PAINT:
        if ( !MMK_IsFocusWin( win_id ) )
        {
            GUI_FillRect(&dev_info, MMITHEME_GetFullScreenRect(), MMIMTV_TRANSPARENT_COLOR);
        }
		break;
    case MSG_GET_FOCUS:
        MMIDEFAULT_TurnOnBackLight(); 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        SetBlockType();
        
        //MMIAPISET_GetDeviceMode(&audio_mode);
		if(MMIMTV_IsMute())
		{
            CMMB_Mute();
	        //MMIAUDIO_SetMute(TRUE);
	        ring_vol = 0;
		}	
		else
		{
			CMMB_UnMute();		
        	//MMIAUDIO_SetMute(FALSE);
		}
        //MMIAUDIO_SetVolume(audio_mode, MMIMTV_GetVolume());
        if((ring_vol != MMISRVAUD_GetVolume(audio_handle)) && (audio_handle > 0))
        {
            MMISRVAUD_SetVolume(audio_handle, ring_vol);
        }
        MMIMTV_SetBrightness(MMIMTV_GetBrightness());

#ifdef ENG_SUPPORT
        if(MMIMBBMS_IsCurrentServiceAudioTyle()
            && !MMIAPIENG_IsAudioServiceEnable())
        {
            //SCI_TRACE_LOW:"[MMIMBBMS]: MSG_GET_FOCUS cannot play audio service!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_WINTAB_267_112_2_18_2_40_53_887,(uint8*)"");
            break;//切换至音频节目播放，音频节目不支持，返回播放界面不作播放处理
        }
#endif		
        MMIMTV_HandleFocusMsg(msg_id);
        
        if(MMIMBBMS_IsNeedPlayService())
        {
            //SCI_TRACE_LOW:"[MMIMBBMS]: MSG_GET_FOCUS is need play service "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_WINTAB_275_112_2_18_2_40_53_888,(uint8*)"");
            MMIMBBMS_SetIsNeedPlayService(FALSE);
            
            //播放前确认service_id是否有效
            if(MMIMBBMS_GetPlayInfo()->service_id == MMIMBBMS_INVALID_SERVICE_ID)
            {
                //SCI_TRACE_LOW:"[MMIMBBMS]: service_id is INVALID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_WINTAB_281_112_2_18_2_40_53_889,(uint8*)"");
                break;
            }
            
            //连接等待场景
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PLAY_CONNECTING);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
            if(MMIMBBMS_SUBSCRIBE_FROM_PLAY == MMIMBBMS_GetSubscribeFromType())
            {
                //播放触发的订购，完成后需重走播放流程
                MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_NONE);
                MMIMBBMS_PlayServiceByIndex(MMIMBBMS_GetPlayingServiceIndex());
            }
            else
            {
                MMIMBBMS_StartPlay();
            }
        }
        else
        {
            //显示频道号
            if(OSD_IDLE == MMIMTV_GetFocusOSDScene() || OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
            {
                MMIMTV_DisplayServiceNumber();
            }
            
            if(MMIMBBMS_IsNeedResumePlay())
            {
                //SCI_TRACE_LOW:"[MMIMBBMS]: MSG_GET_FOCUS is need resume play"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_WINTAB_311_112_2_18_2_40_53_890,(uint8*)"");
                MMIMBBMS_ResetResumePlayFlag();
                MMIMBBMS_StartPlay();
            }
        }
        break;
        
    case MSG_LOSE_FOCUS:
		
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
		MMIMBBMS_StopPlay();
        MMIMBBMS_SetIsNeedPlayService(TRUE);
		
        GUIBLOCK_ClearByColor(MMIMTV_TRANSPARENT_COLOR, &dev_info);
        GUIBLOCK_ResetMainType();
        MMIMTV_HandleFocusMsg(msg_id);
        break;
        
    case MSG_TIMER:
        MMIMTV_HandleTimerMsg(*(uint8*)param);
        break;
        
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_HandleMsg(MMIMTV_APP_WEB, param);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_HandleMsg(MMIMTV_APP_OK, param);
        break;
        
    case MSG_APP_LEFT:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_HandleMsg(MMIMTV_APP_LEFT, param);
        break;
        
    case MSG_APP_RIGHT:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_HandleMsg(MMIMTV_APP_RIGHT, param);
        break;
        
    case MSG_APP_UP:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_HandleMsg(MMIMTV_APP_UP, param);
        break;
        
    case MSG_APP_DOWN:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_HandleMsg(MMIMTV_APP_DOWN, param);
        break;

    case MSG_CTL_EDITBOX_UPDATE_STRNUM: //手动搜索场景softkey更新 “删除”“返回”
        MMIMTV_HandleMsg(MMIMTV_CTL_EDITBOX_UPDATE_STRNUM, param);
        break;
		
	case MSG_CTL_LIST_MOVEDOWN:
	case MSG_CTL_LIST_MOVEUP:
	case MSG_CTL_LIST_MOVEBOTTOM:
	case MSG_CTL_LIST_MOVETOP:
		MMIMTV_HandleMsg(MMIMTV_CTL_LISTBOX_INDEX_MOVED, param);
		break;
        
    case MSG_NOTIFY_TAB_SWITCH: //频道切换栏
        //reset switch play tab timer
        MMIMTV_ResetSwitchPlayTimer();
        break;
        
    case MSG_APP_0: //数字键切换频道支持
        MMIMTV_HandleMsg(MMIMTV_APP_0, param);
        break;
    case MSG_APP_1:
        MMIMTV_HandleMsg(MMIMTV_APP_1, param);
        break;
    case MSG_APP_2:
        MMIMTV_HandleMsg(MMIMTV_APP_2, param);
        break;
    case MSG_APP_3:
        MMIMTV_HandleMsg(MMIMTV_APP_3, param);
        break;
    case MSG_APP_4:
        MMIMTV_HandleMsg(MMIMTV_APP_4, param);
        break;
    case MSG_APP_5:
        MMIMTV_HandleMsg(MMIMTV_APP_5, param);
        break;    
    case MSG_APP_6:
        MMIMTV_HandleMsg(MMIMTV_APP_6, param);
        break;     
    case MSG_APP_7:
        MMIMTV_HandleMsg(MMIMTV_APP_7, param);
        break;
    case MSG_APP_8:
        MMIMTV_HandleMsg(MMIMTV_APP_8, param);
        break;
    case MSG_APP_9:
        MMIMTV_HandleMsg(MMIMTV_APP_9, param);
        break;
        
    case MSG_APP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_TurnUpVolume();
        break;
        
    case MSG_APP_DOWNSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_TurnDownVolume();
        break;
        
    case MSG_APP_CAMERA:
        MMIMTV_HandleMsg(MMIMTV_APP_CAMERA, param);
        break;
        
    case MSG_KEYREPEAT_STAR:
    case MSG_KEYREPEAT_HASH:
    case MSG_APP_STAR:
    case MSG_APP_HASH:
        MMIMTV_ResetDisappearTimer();
        if (MMICOM_RETURN_INCREASE ==MMIAPICOM_IsIncrease(msg_id))
        {
            if(MMIMTV_GetFocusOSDScene()==OSD_ESG_LIST)
            {
                MMIMTV_HandleMsg(MMIMTV_APP_HASH, param);
            }
            else
            {
                MMIMTV_TurnUpVolume();
            }
        }
        else
        {

            if(MMIMTV_GetFocusOSDScene()==OSD_ESG_LIST)
            {
                MMIMTV_HandleMsg(MMIMTV_APP_STAR, param);
            }
            else
            {
                MMIMTV_TurnDownVolume();
            }
        }
        break;

        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        MMIMTV_ResetDisappearTimer();
        MMIMTV_HandleTPMsg(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_RED:
        if(OSD_MANUAL_SEARCH == MMIMTV_GetFocusOSDScene())
        {
            //编辑框下，RED键返回上级菜单
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_SEARCH_MENU);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        else
        {
            MMK_CloseWin(win_id);
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIMTV_HandleMsg(MMIMTV_APP_CANCEL, param);
        break;
        
    case MSG_MTV_ESG_SUC_CNF:
    case MSG_MTV_ESG_ERR_IND:
        MMIMTV_HandlePSMsg(PNULL,msg_id,param);
        break;

#ifdef MBBMS_SUPPORT
        //MBBMS发起播放流程失败(MSK更新失败等)，CMMB关闭横屏连接等待场景
    case MSG_APP_MBBMS_PLAY_PROCESS_ERROR:
        if(OSD_PLAY_CONNECTING == MMIMTV_GetFocusOSDScene())
        {
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_SHORTCUT_ICON);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        break;
#endif
        
    case MSG_CLOSE_WINDOW:
        MMIMBBMS_ReleaseBgOSDLayer();
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        GUIBLOCK_ResetMainType();
        break;

        //关机时需要关闭mtv, 否则会显示透明
    case MSG_KEYLONG_RED:
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        recode = MMI_RESULT_FALSE;
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(void)
{    
    GUI_COLOR_T color_key= MMIMTV_TRANSPARENT_COLOR;
    uint32 alpha = MMIMTV_GetTransparencyValue(MMIMTV_GetTransparency());

    GUIBLOCK_SetType(alpha,color_key,GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev());    
}

/*****************************************************************************/
//  Description : init display param 
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitDisplayParam(void)
{
    uint16 w, h;
    DPLAYER_DISPLAY_PARAM_T display_param = {0};
    
    GUILCD_GetLogicWidthHeight(0, &w, &h);
    //SCI_TRACE_LOW:"MMIMBBMS InitDisplayParam: w %d, h %d, angle %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_WINTAB_546_112_2_18_2_40_53_891,(uint8*)"ddd", w, h, GUILCD_GetLogicAngle(0));
    
    display_param.disp_rect.x = 0;
    display_param.disp_rect.y = 0;
    display_param.disp_rect.dx = h;
    display_param.disp_rect.dy = w;
    
    display_param.target_rect.x = 0;
    display_param.target_rect.y = 0;
    display_param.target_rect.dx = h;
    display_param.target_rect.dy = w;
    
    display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
    display_param.RotateAngle = GUILCD_GetLogicAngle(0);
    display_param.audioSupport = 1;
    display_param.videoSupport = 1;
    display_param.LcdId = 0;
    display_param.LcdBlkId = 0;
    display_param.b_support_seek = 0;
    
    MTVSERVICE_SetDisplayParam(&display_param);
}

#endif


/*Edit by script, ignore 1 case. Thu Apr 26 19:01:00 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
