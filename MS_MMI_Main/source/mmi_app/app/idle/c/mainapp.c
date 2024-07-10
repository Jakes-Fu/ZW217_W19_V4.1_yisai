/****************************************************************************
** File Name:      manapp.c                                                *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
** 
****************************************************************************/
#define _MAIN_APP_C_  


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "mmiidle_export.h"
#include "mmi_image.h"
#include "guicommon.h"
#include "mmi_text.h"
#include "tb_dal.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "guimenu.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_call_export.h"
#include "mmiset_display.h"
#include "mmisms_export.h"
#include "mmicl_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmistk_export.h"
#include "mmicc_export.h"
#include "mmiphone_export.h"
#include "mmienvset_export.h"
#include "mmi_appmsg.h"
#include "mmiidle_subwintab.h"
#include "guimsgbox.h"
#include "guilabel.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "mmiudisk_export.h"
#include "mmimms_export.h"
#include "mmiparse_export.h"
#include "mmimsd_export.h"
#include "mmi_textfun.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmisd_export.h"
#include "mmiacc.h"
#include "guiblock.h"
#include "mmifm_export.h"
#include "mmitv_out.h"
#include "guibutton.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
#include "mmicalendar_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmipb_export.h"
#include "mmipub.h"
#include "mmiss_export.h"
//#include "mmiebook.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#include "guistatusbar.h"
#include "mmialarm_id.h"
#include "mmisms_id.h"
#include "mms_image.h"
#include "mmiset_id.h"
#include "mmicc_id.h"
#include "mmiota_image.h"
#include "mmibt_export.h"
//#include "mmiset_nv.h"
#include "mmiset_text.h"
#include "mmimp3_export.h"
#include "mn_type.h"
#include "mn_api.h"
#include "mmipb_id.h"
#include "mmimp3_id.h"
#include "mmiacc_id.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "block_mem_def.h"
#include "block_mem.h"
#include "mmicountedtime_export.h"
#include "mmi_wallpaper_export.h"
#include "mmifmm_export.h"
#include "mmidc_export.h"
#include "mmischedule_export.h"

#include "mmi_mainmenu_export.h"
#ifdef MMI_SPRITE_ENABLE
#include "mmiss_sprite.h"
#endif
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_msg.h"
#include "guiwin.h"
#ifdef MMI_MOCOR_SAMPLE
#include "sample_export.h"
#endif
#include "ui_layer.h"
#include "guires.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#include "guistring.h"
#include "mmipdp_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef DCD_SUPPORT
#include "mmidcd_export.h"
#endif
#include "guitab.h"
#include "mmidm_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef FLASH_SUPPORT
#include "mmiflash_export.h"
#endif
#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT)
#include "mmiwidget.h"
#include "mmiwidget_export.h"
#endif
#if defined QBTHEME_SUPPORT
#include "mmiqbtheme_export.h"
#endif
#if (defined QBTHEME_SUPPORT)
#include "mmiqbtheme_idleexport.h"
#endif

#ifdef PUSH_EMAIL_SUPPORT  
#include "mmimail_export.h"
#endif

#ifdef WEATHER_SUPPORT
#include "mmiweather.h"
#endif

#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif

#include "mmk_gsensor.h"
#include "mmiset_func.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmibt_text.h"

#include "mmisms_read.h"

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#ifdef MMI_TASK_MANAGER
#include "mmi_applet_manager.h"
#endif

#include "mmiidle_cstyle.h"
#include "mmiidle_statusbar.h"
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
#include "mmicc_export.h"
#endif

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_utility.h"
#endif

#ifdef SXH_APPLE_SUPPORT
#include "apple_export.h"
#endif

#include "mmi_autotest.h"
#ifdef VIDEO_PLAYER_SUPPORT       
#include "mmivp_export.h"
#endif

#include "watch_launcher_main.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/


// MAIN screen icon 
#define MMI_IDLE_VIBRA_DEFAULT_TIME 1500  //idle下长按#切换情景模式时振动时长
//the number display rect
//#define MMI_MAINAPP_DIALNUMBER_RECT     {MMI_MAINAPP_DIALNUMBER_RECT_LEFT, MMI_MAINAPP_DIALNUMBER_RECT_TOP, MMI_MAINAPP_DIALNUMBER_RECT_RIGHT, MMI_MAINAPP_DIALNUMBER_RECT_BOTTOM}
//#define MMI_IDLE_CALE_MAX_LEN   136  //idle日程名称最大长度

//#define MMI_IDLE_SUSP_POINTS_WIDTH  10//"..."显示所占的宽度


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// main application modual
MMI_APPLICATION_T   g_main_app;   
const MMI_WIN_ID_T  g_idle_win = MAIN_IDLE_WIN_ID;       // idle窗口的id
LOCAL uint8         g_animal_time_id = 0;    //动画时钟

#ifndef FLASH_SUPPORT
LOCAL BOOLEAN               s_idlewin_is_open; //idle窗口是否打开
#endif

//LOCAL uint8 s_sim_error_time_id = 0;    // no sim or sim errror窗口定时器，用来显示正在充电

#if defined(MMI_INDIAN_SOS_SUPPORT) && defined(MMI_PDA_SUPPORT)
LOCAL uint8 s_sos_idle_key_pressed_count = 0;
LOCAL uint8 s_sos_idle_count_timer_id = 0;
#endif

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start Hard icon for tp
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void StartHdShortCutBarIcon(void);

/*****************************************************************************/
//  Description : the process message function of the show window message
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IdleWin_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle idle message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIdleMsg(
                         MMI_WIN_ID_T         win_id,
                         MMI_MESSAGE_ID_E     msg_id,
                         DPARAM               param
                         );

#if defined(MMI_INDIAN_SOS_SUPPORT) && defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : handle the logic of sos mode entry 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleSOSEntryType(uint8 timer_id, uint32 param);
#endif

#ifdef QQ_SUPPORT
/*****************************************************************************/
//  Description :InitIdleQQState
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  void InitIdleQQState(void);
#endif
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
/*****************************************************************************/
//  Description : to handle  add blacklist open setting query window
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFlyModeCloseQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
/*****************************************************************************/
//  Description :飞行模式设置的窗口处理函数
//  Global resource dependence : 
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyMode(void);
#endif
/**---------------------------------------------------------------------------*
 **                         difination                                        *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : display the main and sub idle soft icon 
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IdleWinIsOpen(void)
{
#ifndef FLASH_SUPPORT
    return (s_idlewin_is_open);
#else
	return MMIAPIFlash_FlashIsVisiable();
#endif
}

/*****************************************************************************/
//  Description : start the idle timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartAnimTimer(void)
{
    MMISET_WALLPAPER_SET_T      wallpaper_info = {0};

    if( (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))  && (MMIDEFAULT_GetFlipStatus()) )
    {//动画时钟
        if(0 == g_animal_time_id)
        {
            //get wallpaper info
            MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
            if(MMISET_WALLPAPER_FIXED_ANIM == wallpaper_info.wallpaper_type)
            {
                g_animal_time_id= MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, PHONE_ANIM_TIME_INTERVAL, FALSE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : stop the idle timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopAnimTimer(void)
{
   //动画时间 
    if (0 < g_animal_time_id )
    {
        MMK_StopTimer( g_animal_time_id );
        g_animal_time_id=0;
    }
}

#if defined(MMI_INDIAN_SOS_SUPPORT) && defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : handle the logic of sos mode entry 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleSOSEntryType(uint8 timer_id, uint32 param)
{
	BOOLEAN is_reset_now = (BOOLEAN)param;

	SCI_TRACE_LOW("[SOS] HandleSOSEntryType IN: s_sos_idle_count_timer_id = %d, s_sos_idle_key_pressed_count = %d", s_sos_idle_count_timer_id,s_sos_idle_key_pressed_count);
	if(!is_reset_now)
	{
		if(s_sos_idle_count_timer_id)
		{
			if(MMISET_SOS_ENTRY_MAGIC_TICK == (++s_sos_idle_key_pressed_count))
			{
				s_sos_idle_key_pressed_count = 0;
				MMK_StopTimer(s_sos_idle_count_timer_id);  
				s_sos_idle_count_timer_id = 0;

				MMIAPISET_StartSOSProcedures();
			}
		}
		else
		{
			s_sos_idle_key_pressed_count++;
			s_sos_idle_count_timer_id = MMK_CreateTimerCallback(MMISET_SOS_ENTRY_TIME_LIMIT, HandleSOSEntryType, TRUE, FALSE);
		}
	}
	else//if(is_reset_now)
	{
		s_sos_idle_key_pressed_count = 0;
		if(s_sos_idle_count_timer_id)
		{
			MMK_StopTimer(s_sos_idle_count_timer_id);  
			s_sos_idle_count_timer_id = 0;
		}
	}
	SCI_TRACE_LOW("[SOS] HandleSOSEntryType OUT: s_sos_idle_count_timer_id = %d, s_sos_idle_key_pressed_count = %d", s_sos_idle_count_timer_id,s_sos_idle_key_pressed_count);
}
#endif

/*****************************************************************************/
//  Description : update idle 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_MAIN_UpdateIdle(void)
{
    if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMIIDLE_SendMsgToIdleWin(MSG_IDLE_UPDATE_MP3, PNULL);
    }     
}

/*****************************************************************************/
//  Description : 全键盘下，某些界面按下数字/字母共享键，转为对应的数字消息
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIIDLE_GetNumMsgId(uint16 orign_msg_id)
{
    uint16 corresponding_msg_id = orign_msg_id;
    switch (orign_msg_id)
    {
        case MSG_KEYUP_R:
                corresponding_msg_id = MSG_KEYUP_1;
                break;
                
        case MSG_KEYUP_T:
                corresponding_msg_id = MSG_KEYUP_2;
                break;
                
        case MSG_KEYUP_Y:
                corresponding_msg_id = MSG_KEYUP_3;
                break;
                
        case MSG_KEYUP_U:
                corresponding_msg_id = MSG_KEYUP_STAR;
                break;
                
        case MSG_KEYUP_F:
                corresponding_msg_id = MSG_KEYUP_4;
                break;
                
        case MSG_KEYUP_G:
                corresponding_msg_id = MSG_KEYUP_5;
                break;
                
        case MSG_KEYUP_H:
                corresponding_msg_id = MSG_KEYUP_6;
                break;
                
        case MSG_KEYUP_J:
                corresponding_msg_id = MSG_KEYUP_HASH;
                break;
                
        case MSG_KEYUP_V:
                corresponding_msg_id = MSG_KEYUP_7;
                break;
                
        case MSG_KEYUP_B:
                corresponding_msg_id = MSG_KEYUP_8;
                break;
                
        case MSG_KEYUP_N:
                corresponding_msg_id = MSG_KEYUP_9;
                break;
                
        case MSG_KEYUP_M:
                corresponding_msg_id = MSG_KEYUP_0;
                break;

        case MSG_KEYDOWN_R:
                corresponding_msg_id = MSG_KEYDOWN_1;
                break;
                
        case MSG_KEYDOWN_T:
                corresponding_msg_id = MSG_KEYDOWN_2;
                break;
                
        case MSG_KEYDOWN_Y:
                corresponding_msg_id = MSG_KEYDOWN_3;
                break;
                
        case MSG_KEYDOWN_U:
                corresponding_msg_id = MSG_KEYDOWN_STAR;
                break;
                
        case MSG_KEYDOWN_F:
                corresponding_msg_id = MSG_KEYDOWN_4;
                break;
                
        case MSG_KEYDOWN_G:
                corresponding_msg_id = MSG_KEYDOWN_5;
                break;
                
        case MSG_KEYDOWN_H:
                corresponding_msg_id = MSG_KEYDOWN_6;
                break;
                
        case MSG_KEYDOWN_J:
                corresponding_msg_id = MSG_KEYDOWN_HASH;
                break;
                
        case MSG_KEYDOWN_V:
                corresponding_msg_id = MSG_KEYDOWN_7;
                break;
                
        case MSG_KEYDOWN_B:
                corresponding_msg_id = MSG_KEYDOWN_8;
                break;
                
        case MSG_KEYDOWN_N:
                corresponding_msg_id = MSG_KEYDOWN_9;
                break;
                
        case MSG_KEYDOWN_M:
                corresponding_msg_id = MSG_KEYDOWN_0;
                break;

        case MSG_KEYLONG_R:
                corresponding_msg_id = MSG_KEYLONG_1;
                break;
                
        case MSG_KEYLONG_T:
                corresponding_msg_id = MSG_KEYLONG_2;
                break;
                
        case MSG_KEYLONG_Y:
                corresponding_msg_id = MSG_KEYLONG_3;
                break;
                
        case MSG_KEYLONG_U:
                corresponding_msg_id = MSG_KEYLONG_STAR;
                break;
                
        case MSG_KEYLONG_F:
                corresponding_msg_id = MSG_KEYLONG_4;
                break;
                
        case MSG_KEYLONG_G:
                corresponding_msg_id = MSG_KEYLONG_5;
                break;
                
        case MSG_KEYLONG_H:
                corresponding_msg_id = MSG_KEYLONG_6;
                break;
                
        case MSG_KEYLONG_J:
                corresponding_msg_id = MSG_KEYLONG_HASH;
                break;
                
        case MSG_KEYLONG_V:
                corresponding_msg_id = MSG_KEYLONG_7;
                break;
                
        case MSG_KEYLONG_B:
                corresponding_msg_id = MSG_KEYLONG_8;
                break;
                
        case MSG_KEYLONG_N:
                corresponding_msg_id = MSG_KEYLONG_9;
                break;
                
        case MSG_KEYLONG_M:
                corresponding_msg_id = MSG_KEYLONG_0;
                break;

        default:
                break;
    }

    return corresponding_msg_id;
}

/*****************************************************************************/
//  Description : the process message function of the main application
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/
MMI_RESULT_E MAIN_HandleSysMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E    recode  = MMI_RESULT_FALSE;

    switch(msg_id)
    {
    case MISC_MSG_RTC_SEC:                
            if (MMIDEFAULT_IsBacklightOn()
                && MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())             
            {
                MMIIDLE_SendMsgToIdleWin( MSG_IDLE_UPDATE_TIME_SECOND, PNULL);
                SCI_TRACE_LOW("MAINAPP MAIN_HandleSysMsg MISC_MSG_RTC_SEC time == %d", SCI_GetTickCount());
            }
        break;

    case MISC_MSG_RTC_MIN:
        SCI_TRACE_LOW("MAINAPP MAIN_HandleSysMsg MISC_MSG_RTC_HOUR time == %d", SCI_GetTickCount());
        if (MMIDEFAULT_IsBacklightOn())
        {
            
            if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
            {
                MMIIDLE_SendMsgToIdleWin( MSG_IDLE_UPDATE_DATETIME, PNULL);
            }
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                MMIAPIKL_SendHourOrMinuteInd();
            }
#endif
#ifndef SUBLCD_SIZE_NONE
            //刷新小屏的显示，时间分
            MMISUB_UpdateDisplaySubLcd();
#endif
        }
    
        // 更新状态栏的时间
        MAIN_SetStbDispTime();
        // dcd相关
        MMIAPIDCD_HandleSysMsg(app_ptr, msg_id, param);

#if defined(MMI_MSD_SUPPORT)
        MMIAPIMSD_MinuteCheck();
#endif
#ifdef HERO_ENGINE_SUPPORT  
		{
			#if !defined(WIN32)
			extern void hero_im_heart_beat();
			hero_im_heart_beat();
			#endif
		}
#endif
        break;

    case MISC_MSG_RTC_HOUR:
        SCI_TRACE_LOW("MAINAPP MAIN_HandleSysMsg MISC_MSG_RTC_HOUR time == %d", SCI_GetTickCount());

        if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
        {
            MMIIDLE_SendMsgToIdleWin(MSG_IDLE_UPDATE_DATETIME, PNULL);
        }
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIAPIKL_SendHourOrMinuteInd();
        }
#endif
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，时间小时
        MMISUB_UpdateDisplaySubLcd();
#endif
#if defined(MMI_MSD_SUPPORT)
        MMIAPIMSD_HourCheck();
#endif
        break;

    default:
        break;
    }

    return (recode);
}
/*****************************************************************************/
//  Description : the process message function of the show window message
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN s_idle_started = FALSE;

LOCAL MMI_RESULT_E IdleWin_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            recode              = MMI_RESULT_TRUE;
    static uint8            s_vibra_timer_id    = 0;
    GUI_LCD_DEV_INFO        lcd_dev_info        = {GUI_LCD_0, GUI_BLOCK_0};
//    MMISET_IDLE_DISPLAY_T   *idle_mode          = PNULL;
    static BOOLEAN          s_is_first_power_on = TRUE;
//#if  (defined MMI_GRID_IDLE_SUPPORT) || (defined MMIWIDGET_SUPPORT) || (defined MMIQBTHEME_SUPPORT)  
//#else
//    static BOOLEAN          s_is_timer_on       = FALSE;
//#endif 
//#if  (defined MMI_GRID_IDLE_SUPPORT) || (defined MMIWIDGET_SUPPORT) || MMIQBTHEME_SUPPORT  
//#else
    //static BOOLEAN          s_is_timer_on       = FALSE;
//#endif 
    LOCAL BOOLEAN           s_is_down_red_key   = FALSE;  //是否按了red key
    static BOOLEAN          s_is_need_set_time  = TRUE;
 
    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
        
    lcd_dev_info.lcd_id     = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   = GUI_BLOCK_MAIN;
 //   idle_mode = MMIAPISET_GetIdleDisplayMode();

    //IDLE界面，全键盘工程，默认需对共用的数字按键进行消息转换。
    if ((MSG_KEYDOWN_Q <= msg_id && msg_id <= MSG_KEYDOWN_EXCLAMATION)
        || (MSG_KEYUP_Q <= msg_id && msg_id <= MSG_KEYUP_EXCLAMATION)        
        || (MSG_KEYLONG_Q <= msg_id && msg_id <= MSG_KEYLONG_EXCLAMATION))
    {
        msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
    }
        
    //SCI_TRACE_LOW:"[idle] IdleWin_HandleMsg msg_id 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINAPP_585_112_2_18_2_23_34_3,(uint8*)"d", msg_id);
    switch(msg_id)
    {
#if defined(MMI_INDIAN_SOS_SUPPORT) && defined(MMI_PDA_SUPPORT)
	case MSG_KEYUP_POWER:
		HandleSOSEntryType(0, FALSE);
		break;
#endif

    case MSG_GSENSOR:
        {
#ifdef VIDEO_PLAYER_SUPPORT            
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION            
            if((uint32 )param == MMK_GSENSOR_STATUS_LEFT_SIDE || (uint32 )param == MMK_GSENSOR_STATUS_RIGHT_SIDE\
                ||(uint32 )param == MMK_GSENSOR_STATUS_UP_SIDE || (uint32 )param == MMK_GSENSOR_STATUS_DOWN_SIDE)
            {
                if(MMIAPIVP_IsOpenPlayerWin())
                {
                    MMIAPIVP_SetVPWinSupportAllAngle();
                }
            }
#endif
#endif
            switch( (uint32 )param)
            {
            //[6800H]需求说了——G-Sensor 只准切到下一首，不切前一首.......
            case MMK_GSENSOR_STATUS_LEFT:
            case MMK_GSENSOR_STATUS_RIGHT:
                if(MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_NEXT_SONG))
                {
#ifdef KURO_SUPPORT
                    if(MMIAPIKUR_IsPlayerPlaying())
                    {
                        MMIAPIKUR_GoToPreOrNextKur(FALSE);
                    }
#ifdef MMI_AUDIO_PLAYER_SUPPORT
                    else if(MMIAPIMP3_IsPlayerPlaying())
                    {
                        MMIAPIMP3_GoToPreOrNextMp3(FALSE);
                        
                        MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, FALSE);
                    }                    
#endif
#else
#ifdef MMI_AUDIO_PLAYER_SUPPORT
                    if(MMIAPIMP3_IsPlayerPlaying())
                    {
                        MMIAPIMP3_GoToPreOrNextMp3(FALSE);
                        
                        MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, FALSE);
                    }                    
#endif                   
#endif

                }

                if(MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_REPLACE_WALLPAPER))
                {
                    if(MMK_IsFocusWin(win_id) 
                    #ifdef  MMIWIDGET_SUPPORT
						&& !MMIAPIWIDGET_IsTpMovingPage()
			        #endif
					&& (!MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_NEXT_SONG)
					     ||(!MMIAPIMP3_IsPlayerActive()
#ifdef KURO_SUPPORT
					&& !MMIAPIKUR_IsPlayerActive()
#endif
)
					    ) 
				      )
                    {
#if defined MMI_WALLPAPER_SUPPORT
                        MMIAPIIDLE_Change_Wallpaper();
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
                        if(MMK_IsFocusWin(win_id) && MMIAPIIDLE_IsLiveWallpaper())
                        {
                        MMIAPILIVEWALLPAPER_Play(win_id);//NEWMS00178618
                        }
#endif                 
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
						if (MMK_IsFocusWin(win_id) && MMIAPISETVWP_IsOpen())
						{
							MMIAPISETVWP_StopVideoWallpaper();
#ifdef  VIDEOTHUMBNAIL_SUPPORT            
                                                MMIAPISETVWP_DisplayThumbnail();
#endif        
							MMIAPISETVWP_SwitchToNext();
                        }
#endif                        
                    }
                }
                break;
                
#ifdef MMI_RES_ORIENT_BOTH
            case MMK_GSENSOR_STATUS_LEFT_SIDE:
                if ( MMISET_IsSensorOpen( MMISET_SENSOR_TYPE_LANDSCAPE_270 ) )//cr227614 maryxiao
                {
					MMIAPIATV_HandleSensorMSG();
                    MMK_SetScreenAngle( LCD_ANGLE_270 );
                }
                break;
            case MMK_GSENSOR_STATUS_RIGHT_SIDE:
                if ( MMISET_IsSensorOpen( MMISET_SENSOR_TYPE_LANDSCAPE_90 ) )
                {
					MMIAPIATV_HandleSensorMSG();
                    MMK_SetScreenAngle( LCD_ANGLE_90 );
                }
                break;
            case MMK_GSENSOR_STATUS_UP_SIDE:
            	if ( MMISET_IsSensorOpen( MMISET_SENSOR_TYPE_LANDSCAPE_270 ) || MMISET_IsSensorOpen( MMISET_SENSOR_TYPE_LANDSCAPE_180) || MMISET_IsSensorOpen( MMISET_SENSOR_TYPE_LANDSCAPE_90))
                {
					MMIAPIATV_HandleSensorMSG();
                    MMK_SetScreenAngle( LCD_ANGLE_0 );
                }
                break;
            case MMK_GSENSOR_STATUS_DOWN_SIDE:
                if ( MMISET_IsSensorOpen( MMISET_SENSOR_TYPE_LANDSCAPE_180 ) )
                {
					MMIAPIATV_HandleSensorMSG();
                    MMK_SetScreenAngle( LCD_ANGLE_180 );
                }
                break;
#endif
            default:
                break;
            } 
        }
        break;

#ifdef SXH_APPLE_SUPPORT
//#ifdef __APPLE_ADDSMS__
	case APPLE_BASE2_MSG_ID:
		{
			AppleIDLEMsgInd(param); /*lint !e718 !e746*/
		}
		break;
//#endif
#endif
    case MSG_OPEN_WINDOW:	
        //fixed idle win reenter
        if (s_idle_started)
        {
            return recode;
        }
        s_idle_started = TRUE;
        MMIZDT_NV_SetFirstPWON(0); //added by bao 如果异常重启不需要开机铃声
        MMITHEME_InitMenuItemCache();
#ifdef DPHONE_SUPPORT
	#ifndef WIN32
	if (GPIO_GetHookState())
	{
		MMIDEFAULT_SetHookStates(TRUE);
	}
	GPIO_EnableHookInt(TRUE);
	#endif
#endif

#ifdef MMI_DEBUGLAYER_SUPPORT        
        MMIAPICOM_CreateDebugLayer();
#endif  

//#ifdef MMI_RES_ORIENT_BOTH     
        //MMK_OpenGsensor( win_id );
        MMISET_UpdateCommonGSensor();
//#endif

#ifdef MMI_SPRITE_ENABLE
        MMIAPIML_CreateLayerTest();  
#endif
        s_is_down_red_key = FALSE;

#ifdef MMI_SPRITE_ENABLE
        MMIIDLE_InitSprite();
#endif
        MMIDEFAULT_StartLcdBackLightTimer();
        MMIDEFAULT_StartAutoKeylockTimer();
        StartHdShortCutBarIcon();
        //充电时钟启动
#ifndef FLASH_SUPPORT
        s_idlewin_is_open = TRUE;
#endif
        //StartIdleTimer();

        //动画时钟启动
        //g_animal_time_id = 0 ;

        MMIAPISTK_CnfIdleScrAva();
        MMIAPISTK_CnfSimUserAct();

#ifdef QBTHEME_SUPPORT
        if(STYLE_IS_SUPPORT_QBTHEME)
        {
			MMIAPIQBTHEME_StyleStart();
		}
#endif
        //creat wallpaper ctrl
        //MMIAPIIDLE_CreatWallpaperCtrl();
        // StatusBarInit();
        GUIWIN_CreateStbDyn(win_id); // 动态创建状态栏
        
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_EVENT, TRUE);
#ifndef MMI_IDLE_MINI_SUPPORT        
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);//需求状态栏加半透黑色背景，idle统一处理了
#else        
        GUIWIN_SetStbBgIsDesplay(win_id, FALSE);//需求状态栏加半透黑色背景，idle统一处理了 
#endif        
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        MAIN_SetStbTimeVisible(TRUE);

        //MMIDLE_RestartSsTimer(win_id);

        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
#ifdef MMIWIDGET_SUPPORT
        if(MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
        {
        MMIAPIWIDGET_CreateIdleWidget();
        }
#endif

#ifdef MMI_GRID_IDLE_SUPPORT
        if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
        {
       MMIAPIGRID_CreateIdle();
        }
#endif
        
#if defined QBTHEME_SUPPORT
        if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
        {
            //call 
           // MMIAPIQBTHEME_StyleStart(); //白屏，提前
            MMIAPIQBTHEME_CreateIdle();
        }
#endif


#ifdef MMI_PDA_SUPPORT
       MMIIDLE_CreateIStyleMenu( win_id );
#endif

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT 
        if(MMIAPIIDLE_IsLiveWallpaper())
        {
            MMI_LivewallpaperResFoldInit();
            MMIAPILIVEWALLPAPER_Init();
        }
#endif

       HandleIdleMsg(win_id,msg_id,param);
#ifdef JAVA_SUPPORT_IA
	   MMIAPIJAVA_JBlendiaAjmsInit();
#endif

#ifdef MMI_LDN_SUPPORT
    MMIAPIPHONE_CheckLdnReadyInd();
#endif    
//多卡的情况下，将PS的启动延后至Ilde Win Open的时候
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        // TODO: 改为调用MMIAPIPHONE_PowerOnPsFull
        MMIAPIPHONE_PowerOnPsEx();
#endif

/* BEGIN: Modified by George.Liu 01526, 2010/8/2   PN:MergeToMw1029forCustom */
#ifdef MMI_WIFI_SUPPORT
    MMIAPIWIFI_Init();
#endif
/* END:   Modified by George.Liu 01526, 2010/8/2   PN:MergeToMw1029forCustom */

#ifdef MMIEMAIL_SUPPORT
//  MMIAPIEMAIL_SyncEmail();
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
        MMIMAIN_PowerOnAddStbScrollItem();
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
        if (MMIAPISET_IsNeedResetDateTime())
        {
#ifndef WIN32
#if defined(MMI_UPDATE_TIME_SUPPORT)
            MMIAPISET_RestoreDateTimeFactorySetting();
            MMIAPISET_SetNeedResetDateTime(FALSE);//bug1579813
#endif
#endif
        }
#endif

#ifdef OD_GAME_SUPPORT
	od_game_init();
#endif

        WatchLAUNCHER_Enter();
        MMK_PostMsg(win_id,MSG_IDLE_USB,PNULL,PNULL); //判断是否有延时的usb
        break;
#ifdef DCD_SUPPORT
	case MSG_NOTIFY_TAB_NEED_ITEM_CONTENT:
	case MSG_NOTIFY_TAB_SWITCH: 		
	case MSG_DCD_TAB_UPDATE:
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
        break;
#endif
    case MSG_FULL_PAINT:
#ifdef MMI_SPRITE_ENABLE        
        MMIAPIML_DrawLayerTest();
#endif
        if(s_is_first_power_on)
        {
            s_is_first_power_on = FALSE;
           // temp_for_call
           // if(MMIAPIENG_GetChipTestMode() == MMIENG_CHIP_TEST_SETTING_ON)
           // {
           //     MMIENGAPI_StartChipTest();
           // }
        }       
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，和主屏同步,主要是彩信
        MMISUB_UpdateDisplaySubLcd();  
#endif
        HandleIdleMsg(win_id,msg_id,param);

        //MMIAPIMMS_IdleWinDownloadMMS(); 

#ifdef MMI_SPRITE_ENABLE
        MMIIDLE_ShowSprite();
#endif
#ifdef DPHONE_SUPPORT

#else
        // 有可能上面的窗口是半透窗口，它的刷新会导致此窗口的刷新，所以加
        // s_is_need_set_time来判断是否要弹日期设置窗口
        if (s_is_need_set_time && MMIAPISET_IsNeedResetDateTime()) //判断是否需要设置时间和日期
        { 
            s_is_need_set_time = FALSE;
            MMIAPIPHONE_SetEventDelaied(MMI_SET_DATA_DELAY);

        }
#endif        
        //回到Idle后判断是否要还插着USB
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {
            MMK_PostMsg(win_id,MSG_IDLE_USB,PNULL,PNULL); //判断是否有延时的usb
        }
        MMIAPIDM_HandleDmIdleOption();
        break;      
        
    case MSG_IDLE_USB:  
        MMIAPIPHONE_ResumeAllDelaiedEvent();
//        if (MMIAPIUdisk_IsUSBDelay())
//        {        
//            MMIAPIUdisk_OpenUsbOperWindow();
//            MMIAPIUdisk_SetUSBDelay(FALSE);     
//         }
        break;
#ifndef UI_MULTILAYER_SUPPORT
    case MSG_CTL_ANIM_GIF_UPDATE:   
#endif        
    case MSG_CTL_ANIM_DISPLAY_IND:
        {
            GUIANIM_DISPLAY_IND_T   display_ind = *(GUIANIM_DISPLAY_IND_T *)param;
            //close waiting win and prompt set wallpaper
#if defined MMI_WALLPAPER_SUPPORT
            MMIAPIFMM_CloseWaitSetWallpaper(display_ind.result);  
#endif
            if(display_ind.result == 0)
            {
                //GUIANIM_UpdateDisplay(display_ind.ctrl_handle);

                //MMK_SendMsg(display_ind.ctrl_handle,  MSG_CTL_PAINT, 0 );
            }


#ifdef MMI_ETWS_SUPPORT
        if( TRUE == MMIAPISMSCB_GetIsNeedRedisplay() )
        {
            MMIAPISMSCB_ShowNewETWSPrompt();
        }
        else
#endif
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
            }
        }
        break;  

    case MSG_KEYDOWN_HEADSET_DETECT:
    case MSG_KEYUP_HEADSET_DETECT:
        recode = MMI_RESULT_FALSE;
        break;

    case MSG_IDLE_UPDATE_MP3:
    case MSG_IDLE_UPDATE_NETWORK:
    // 耳机插入和耳机拔出        
        if(MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle()
            || MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle())
        {
            if (MMIDEFAULT_IsBacklightOn() && MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
            }
        }
        else
        {
            GUIWIN_UpdateStb();        
        }
        HandleIdleMsg( win_id,  msg_id,  param);
        break;
    //刷新小图标 
    case MSG_IDLE_UPDATE_ICON:
        if ( (MMIDEFAULT_IsBacklightOn()) && (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId()) )
        {          
            MMK_SendMsg( IDLEWIN_STATUSBAR_CTRL_ID,MSG_CTL_PAINT,PNULL);
        }
        break;
    //震动启动处理
    case MSG_IDLE_VIBRA:
        MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
        if (s_vibra_timer_id != 0)
        {
            MMK_StopTimer(s_vibra_timer_id);
            s_vibra_timer_id = 0;
        }
        s_vibra_timer_id = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, MMI_IDLE_VIBRA_DEFAULT_TIME,FALSE);
        break;

        
    case MSG_APP_OK:
#ifdef MMI_MOCOR_SAMPLE
        MMIAPISAMPLE_CreateHelloSprdWin();
        break;
#endif
    
    case MSG_CTL_OK:
#if defined (DUALMODE_QUICK_SWITCH) && defined(MMI_DUALMODE_ENABLE)//用来工厂测试用，一键切换TD/GSM模式
        {
            static uint32 network_switch_times = 0;
            
            SCI_TRACE_LOW("MAINAPP IdleWin_HandleMsg network == %d", network_switch_times);

            if (network_switch_times % 2)//先切TD，然后GSM
            {
                MMIAPISET_SetNetworkType(MN_GMMREG_RAT_GPRS);
            }
            else
            {
                MMIAPISET_SetNetworkType(MN_GMMREG_RAT_3G);
            }
            
            network_switch_times++;
        }
#else
#if defined CMCC_UI_STYLE && defined MMI_SPRITE_ENABLE     
        if (!MMIAPIML_HandleLayerTest(win_id,msg_id,param))   
#endif
        {
            HandleIdleMsg(win_id,msg_id,param);
        }
#endif
        break;
      
    case MSG_APP_MENU:
        HandleIdleMsg(win_id, msg_id, param);
        break;

    case MSG_APP_RIGHT:
    case MSG_APP_LEFT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_LONG: 
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
       
#ifdef MMI_SPRITE_ENABLE        
        if (!MMIAPIML_HandleLayerTest(win_id,msg_id,param))
#endif            
        {
            HandleIdleMsg(win_id,msg_id,param);
        }
        break;

    case MSG_APP_CANCEL:
#if defined MMI_GRID_IDLE_SUPPORT || defined QBTHEME_SUPPORT
        if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()   
            || MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle()
            ) 
        {
            HandleIdleMsg(win_id,msg_id,param);
            break;
        }
#endif
    /* fall through */
    //lint -fallthrough
    case MSG_CTL_CANCEL:    //右软键
    #ifdef CMCC_UI_STYLE
        //MO一键上网标记在web键
        //MMISET_EnterMonternetWin();
        MMIAPIMENU_CreatMainMenu(); //进入主菜单
    #elif defined(BAR_PHONE_WIDGET) 
        if(MMISET_IDLE_STYLE_GRID != MMIAPISET_GetIdleStyle())
        {
            MMK_SendMsg(win_id,MSG_LOSE_FOCUS,PNULL);
            MMISET_SetIdleStyle( MMISET_IDLE_STYLE_GRID );
            MMK_SendMsg(win_id,MSG_GET_FOCUS,PNULL);
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);          
        }
    #else
#ifdef MMI_PDA_SUPPORT
        if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
#endif
        {
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT

            if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))

            {
                uint16  sim_sys[MMI_DUAL_SYS_MAX] = {0};
	            uint32 sim_num = 0;
	            const char* s_emergency_call = CC_CALL_EMERGENCY_CALL;	       

                if(MMIAPISET_GetFlyMode())
                {
                    MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_IDLE_FLYMODE_QUERY,IMAGE_PUBWIN_WARNING,PNULL,HandleFlyModeCloseQueryWinMsg,PNULL);        
                }
                else
                {
                    sim_num = MMIAPIPHONE_GetPSReadyNum(sim_sys, MMI_DUAL_SYS_MAX);
	                MMIAPICC_MakeCall((MN_DUAL_SYS_E)sim_sys[0],
	                                            (uint8 *)s_emergency_call,
	                                            strlen(s_emergency_call),
	                                            PNULL,
	                                            PNULL,
	                                            CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,
	                                            PNULL);
                }              
            }
	        else
#endif             
                {
                    MMIAPISET_OpenFuncKeyWin(win_id, MMISET_FUNCKEY_RIGHT);
                }

        }
#endif
        break;

#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/04
    case MSG_APP_SMS:
		MMIAPISMS_OpenMainMenuWin();			
		break;
#endif 
    //一键上网
    case MSG_APP_AT:
#ifdef BROWSER_SUPPORT
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.url_ptr = PNULL;
            entry_param.user_agent_ptr = PNULL;

            MMIAPIBROWSER_Entry(&entry_param);
        }
#endif
        break;    
    
    //绿键已拨电话
    case MSG_APP_GREEN:        
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        {
            uint8           exist_sim_count = 0; 
            MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;

            for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
                if (SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(dual_sys))
                {
                    exist_sim_count++;
                    break;
                }
            }

            if (0 == exist_sim_count)
            {
                MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,(uint8*)"112", 3, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);

                break;
            }

            MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
        }
        break;  

    case MSG_KEYDOWN_TV:
#ifdef CMMB_SUPPORT
		MMIAPIMTV_OpenMTVPlayer();	
#else
        MMIAPIATV_OpenATV();
#endif			
        break;
#ifdef TAKESCREENPHOTO_SUPPORT
    case MSG_KEYLONG_CAMREA:
#ifdef  CAMERA_SUPPORT    
        if(MMIDEFAULT_GetFlipStatus())
        {
            MMIAPIDC_OpenPhotoWin();
        }
        else
#endif
        {
            MMIDEFAULT_TurnOnBackLight();
            //MMIAPIMP3_HandleCameraKey();
        }
        break;
#else
    case MSG_APP_CAMERA:
#ifdef  CAMERA_SUPPORT    
        if(MMIDEFAULT_GetFlipStatus())
        {
            MMIAPIDC_OpenPhotoWin();
        }
        else
#endif
        {
            MMIDEFAULT_TurnOnBackLight();
            //MMIAPIMP3_HandleCameraKey();
        }
        break;

#ifdef  CAMERA_SUPPORT  
    case MSG_KEYLONG_CAMREA:
        MMIAPIDC_OpenPhotoWin();
        break;
#endif
#endif
        //失去焦点
    case MSG_BACKLIGHT_TURN_OFF:
    case MSG_LOSE_FOCUS:
        if ( !MMK_IsFocusWin( MAIN_MAINMENU_WIN_ID ))
        {
            MMITHEME_DestroyMenuItemCache();
        }
        
        SCI_TRACE_LOW("MAINAPP IdleWin_HandleMsg MSG_LOSE_FOCUS start time == %d", SCI_GetTickCount());
        HandleIdleMsg(win_id,msg_id,param);
        SCI_TRACE_LOW("MAINAPP IdleWin_HandleMsg MSG_LOSE_FOCUS end time == %d", SCI_GetTickCount());
        
        MAIN_SetStbTimeVisible(TRUE);
        s_is_down_red_key = FALSE;
        MAIN_SetAllStbAppCallbackValid(FALSE);
//        MMIAPIIDLESS_StopTimer();
        StopAnimTimer();
#if defined MMI_COMMON_IDLE_SUPPORT
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        MMIIDLECOM_StopIdleMp3Timer();
#endif
#endif
        break;
    
    //重获焦点
    case MSG_BACKLIGHT_TURN_ON:
    case MSG_GET_FOCUS:
        #ifdef UPB_IDLE_SUPPORT
        UPB_DestoryAnim();
        #endif
        MMITHEME_InitMenuItemCache();

#ifdef MMI_PDA_SUPPORT
        MMIIDLE_CreateIStyleMenu( win_id );
#endif
        //SCI_TRACE_LOW("[MEM] %d KB-- %d KB U=%d KB",SCI_GetSystemSpaceTotalAvalidMem()/1024, SCI_GetHeapTotalSpace(DYNAMIC_HEAP_APP_MEMORY)/1024,ZDT_File_GetMemFree(FALSE));
        SCI_TRACE_LOW("MAINAPP IdleWin_HandleMsg MSG_GET_FOCUS start time == %d", SCI_GetTickCount());
        HandleIdleMsg(win_id,msg_id,param);
        SCI_TRACE_LOW("MAINAPP IdleWin_HandleMsg MSG_GET_FOCUS end time == %d", SCI_GetTickCount());
        
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_EVENT, TRUE);
#ifndef MMI_IDLE_MINI_SUPPORT        
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);//需求状态栏加半透黑色背景，idle统一处理了
#else        
        GUIWIN_SetStbBgIsDesplay(win_id, FALSE);//需求状态栏加半透黑色背景，idle统一处理了
#endif        
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        MAIN_SetStbTimeVisible(TRUE);
        
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);

        //get wallpaper info
        MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
        switch (wallpaper_info.wallpaper_type)
        {
        case MMISET_WALLPAPER_ALBUM:
        case MMISET_WALLPAPER_DOWNLOAD:                         
#ifdef DRM_SUPPORT
#if defined MMI_WALLPAPER_SUPPORT
                if (MMIAPIIDLE_IsExistWallpaperFile(PNULL))
                {
                    MMIMULTIM_AS_WALLPAPER_INFO wallpaper_file_ptr = {0};
                    MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);
                    if (MMIAPIFMM_IsDrmFileBySuffix(wallpaper_file_ptr.full_path_name,
                        wallpaper_file_ptr.full_path_len))
                    {
                        //检查是否过期了，如果过期了，设置成默认的图片
                        if (MMIAPIDRM_IsRightsExpired(SFS_INVALID_HANDLE, 
                            wallpaper_file_ptr.full_path_name, DRM_PERMISSION_DISPLAY))
                        {
                            MMIAPISET_ResetDefaultWallpaper();                       
                        }
                    }
                }
#endif
#endif
            //judge wallpaper
#if defined MMI_WALLPAPER_SUPPORT
            if (MMIAPIIDLE_CheckWallPaperValid())
            {
                //judge wallpaper is overwrite etc. update
                if (!MMIAPISET_UpdateWallpaper())
                {
                    if (MMITHEME_IsDynaAllocWallpaperBuf())
                    {
                        //update wallpaper
                        MMIAPIIDLE_SetWallpaper(FALSE);
                    }
                }
            }
            else
#endif
            {
                //reset wallpaper
                MMIAPISET_ResetDefaultWallpaper();
            }
            break;
            
        default:
            break;
        }

        MAIN_SetAllStbAppCallbackValid(TRUE);
        MMIAPISMS_CheckSMSState(FALSE);
#if defined(SEARCH_SUPPORT)&&defined(MMI_ISTYLE_SUPPORT)
        if (MMIAPIMENU_IsInIsyleSearch())
        {
            if (MMIAPISEARCH_IsNeedSearchAgain())
            {
                MMIAPISEARCH_ResetAllIsNeedRestart();
                MMK_PostMsg(win_id, MSG_NOTIFY_EDITBOX_UPDATE_STRNUM, PNULL, 0);
            }
        }
        else
#endif
        {
            MMIAPISET_SetIsNeedValidatePrivacy(TRUE);
        }

        //MMIDLE_RestartSsTimer(win_id);
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIAPIKL_LockPhone();
        }
#endif
        if (MMIDEFAULT_GetFlipStatus())
        {
            // 如果Flip处于Open状态才StartTimer
            StartAnimTimer();
        }
        else
        {   
            StopAnimTimer();
        }
        MMIAPISTK_CnfIdleScrAva();
        MMIAPISTK_CnfSimUserAct();
#ifdef DPHONE_SUPPORT
        if(msg_id != MSG_BACKLIGHT_TURN_ON)
            MMIDEFAULT_SetHandFreeStatesAndLed(FALSE);    //Modified by yanqi.2010-Apr-07 for 直接拨号接通后按RedKey导致没返回待机界面
#endif

#if defined MMI_AUTOTEST_SUPPORT && defined ENG_SUPPORT        
        MMI_TraceAutoTest( (const uint8*)"TEST MODULE:returned to idle", strlen("TEST MODULE:returned to idle") );
#endif
        break;
        
    case MSG_APP_RED:
    case MSG_KEYDOWN_HANG:    
        s_is_down_red_key = TRUE;
        HandleIdleMsg(win_id,msg_id,param);
        recode = MMI_RESULT_FALSE;
        break;

    case MSG_KEYLONG_RED:      
         recode = HandleIdleMsg(win_id,msg_id,param);
         break;        
        
    case MSG_KEYUP_RED:
    case MSG_KEYUP_HANG:
        if ((s_is_down_red_key) &&
        (MMIAPISET_GetRedKeyLock()))
        {
            //close all light and enter deep sleep
#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT        
            MMISEPC_CloseAllLight();
#else
            MMIDEFAULT_CloseAllLight();
#endif
        }
        else
        {
#ifdef MMI_PDA_SUPPORT
            if(s_is_down_red_key)
            {
                HandleIdleMsg(win_id,msg_id,param);
            }
#endif
            //MMIDLE_RestartSsTimer(win_id);
            recode = MMI_RESULT_FALSE;
        }
        break;       

#ifdef FLIP_PHONE_SUPPORT    
    //关盖
    case MSG_KEYDOWN_FLIP:   
        StopAnimTimer();
#if defined MMI_COMMON_IDLE_SUPPORT
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        MMIIDLECOM_StopIdleMp3Timer();
#endif
#endif
        recode = MMI_RESULT_FALSE;
        break;

    //开盖
    case MSG_KEYUP_FLIP:
        // 启动Timer，就是为了解决FlipDown状态下不启动TImer 
        StartAnimTimer();
#if defined MMI_COMMON_IDLE_SUPPORT
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        MMIIDLECOM_StartIdleMp3Timer();
#endif
#endif
		recode = MMI_RESULT_FALSE;
        break;
#endif     

    //rtc 的中断消息
    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_IDLE_UPDATE_TIME_SECOND:
        HandleIdleMsg(win_id,msg_id,param);
        break;
        
    //时钟事件
    case MSG_TIMER:
        //震动时钟
        if (s_vibra_timer_id == *((uint8 *) param))
        {
            //震动完成
            //SCI_TRACE_LOW:"IdleWin_HandleMsg:received timer msg!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINAPP_1396_112_2_18_2_23_35_6,(uint8*)"");
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
            s_vibra_timer_id =0;
        }
        //动画时钟
        else if (g_animal_time_id == *(uint8*)param )
        {
            //SCI_TRACE_LOW:"IdleWin_HandleMsg: handle the anim wallpater timer!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINAPP_1403_112_2_18_2_23_35_7,(uint8*)"");

            //动画刷新
            if( (MMIDEFAULT_IsBacklightOn()) && (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId()) )
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }

            StopAnimTimer();
            StartAnimTimer();
        }
        //Mp3 name
#if defined MMI_COMMON_IDLE_SUPPORT
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        else if (MMIIDLECOM_GetMp3NameTimerId() == *(uint8*)param )
        {
            if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
            {
                MMIIDLECOM_StopIdleMp3Timer();
                MMIIDLECOM_UpdateMp3NameStartIndex();
                MMIIDLECOM_StartIdleMp3Timer();
                MMK_PostMsg(win_id,MSG_IDLE_UPDATE_MP3,PNULL,0);
            }
        }
#endif        
#endif
        //Screen saver 
//         else if (MMIAPIIDLESS_IsScreenSaverTimerId(param))
//         {
//             MMIAPIIDLESS_StopTimer();
//             if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
//             {      
// //#if !defined MMI_PDA_SUPPORT
// //                if((!MMIAPIMP3_IsPlayerActive() && !MMIAPIKUR_IsPlayerActive()&& !MMIAPIFM_IsFmActive()))
// //#endif
//                 {                    
//                     if(MMIAPIIDLESS_IsScreenSaverMp4Type())
//                     {
//                         MMIAPISET_StopAllRing(FALSE);
//                     }
//                     MMIAPIIDLESS_OpenScreenSaver();
//                 }
//             }
//         }
#if defined MMI_COMMON_IDLE_SUPPORT
#ifdef MMI_IDLE_STKTEXT_SUPPORT 
        else if (MMIIDLECOM_GetStkTextInfo()->idle_display_timer == *(uint8*)param)
        {
            if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
            {
                MMK_PostMsg(win_id, MSG_STK_SET_UP_IDLE_MODE, PNULL, 0);
            }
        }
#endif
#endif
        else
        {
            //没处理的时间
            HandleIdleMsg(win_id,msg_id,param);
            recode = MMI_RESULT_FALSE;
        }
        break;
  
#if 0
    case MSG_KEYDOWN_UPSIDE:
        if(MMIAPIKUR_IsPlayerPlaying())
        {
            MMIAPIKUR_SetKurVolume(TRUE);
        }

#ifdef FM_SUPPORT
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {
            MMIAPIFM_AdjustVolume(1);
        }
#endif
        break;

    case MSG_KEYDOWN_DOWNSIDE:
        //在mp3背景播放，不管开盖盒盖，短侧键调节mp3音量

        if(MMIAPIKUR_IsPlayerPlaying())
        {
            MMIAPIKUR_SetKurVolume(FALSE);
        }

#ifdef FM_SUPPORT
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {
            MMIAPIFM_AdjustVolume(-1);
        }
#endif

        break;
#endif

    //进输入号码窗口
    case MSG_KEYUP_HASH: 
        msg_id = MSG_KEYDOWN_HASH;
        //lint -fallthrough
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
        MMIAPIIDLE_OpenDialWin(msg_id, TRUE);
        break; 
        
    case MSG_KEYLONG_HASH:
        //MMIDLE_RestartSsTimer(win_id);
        
        if (MMIAPIENG_GetIQModeStatus())
        {
            MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
            break;
        }
        else if (!MMIAPITVOUT_GetStartingStatus())
        {
                     //cr244608 2011-5-23
            uint8 active_mode = MMIAPIENVSET_GetActiveModeId();
            switch (active_mode)
            {
            case MMIENVSET_STANDARD_MODE:  // 普通环境
            case MMIENVSET_INSIDE_MODE:      // 车内环境
            case MMIENVSET_NOISY_MODE:       // 户外环境
            case MMIENVSET_USER_MODE1:
            case MMIENVSET_USER_MODE2:
            case MMIENVSET_USER_MODE3:
            #ifndef DPHONE_SUPPORT
            case MMIENVSET_USER_MODE4:
            case MMIENVSET_USER_MODE5:
            #endif
                active_mode = MMIENVSET_SILENT_MODE;
                break;
                
            case MMIENVSET_SILENT_MODE:      // 安静环境
                active_mode = MMIENVSET_MEETING_MODE;
                MMIIDLE_SendMsgToIdleWin(MSG_IDLE_VIBRA, PNULL);
                break;
                
            case MMIENVSET_MEETING_MODE:     // 会议环境
                active_mode = MMIENVSET_STANDARD_MODE;
                break;
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	                
            case MMIENVSET_EARPHONE_MODE:
            case MMIENVSET_BLUETOOTH_MODE:
                break;
#endif
            default:
                //SCI_TRACE_LOW:"IdleWin_HandleMsg MSG_KEYLONG_HASH!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINAPP_1553_112_2_18_2_23_36_8,(uint8*)"");
                break;
            }
            if(active_mode != MMIAPIENVSET_GetActiveModeId())
            {
                MMIAPIENVSET_SetActiveModeForIdle(active_mode);
                MMIENVSET_SetPreActiveMode(active_mode);
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            MMIAPITVOUT_Close(); //close tv out, MSG_KEYLONG_HASH message is handled here, not in MMK_HandlePublicKey() function.          
        }
        break; 
        
#ifdef MMI_CSP_SUPPORT
    case MSG_KEYLONG_STAR:
        MMIAPISET_SetLineInfo();
#if 0        
        //need to handle multi sim
        if(MMISET_IsAlsValid(MN_DUAL_SYS_1))
        {
            if(MMISET_LINE_IS_LOCK == MMISET_GetLineLockStatus(MN_DUAL_SYS_1))
            {
                //prompt not allowed
                MMIPUB_OpenAlertWarningWin(TXT_LINE_LOCK);
            }
            else
            {
                MMISET_LINE_TYPE_E line = (0 == MMIAPISET_GetCallLineInfo(MN_DUAL_SYS_1))?MMISET_LINE_2:MMISET_LINE_1;
                //set line
                MMIAPISET_SetSelectLine(MN_DUAL_SYS_1, line);
            }
        }
#endif        
        break;
#endif
    case MSG_APP_STAR:
        MMIAPIIDLE_OpenDialWin(msg_id,TRUE);
        break;


    case MSG_CLOSE_WINDOW:
        StopAnimTimer();
        
#ifndef FLASH_SUPPORT
        s_idlewin_is_open = FALSE;
#endif
		#ifdef JAVA_SUPPORT_IA
		MMIAPIJAVA_JBlendiaAjmsFinalize();
		#endif

        MMK_CloseGsensor( win_id );
        break;
#if defined TOUCH_PANEL_SUPPORT && defined MMI_SPRITE_ENABLE
    case MSG_TP_PRESS_MOVE:
        if (MMIIDLE_PressMoveSprite(MMK_GET_TP_X(param), MMK_GET_TP_Y(param)))
        {
            MMIIDLE_UpdatePressedSprite();
        }
        break;              
#endif 

   case MSG_PRE_FULL_PAINT:
       HandleIdleMsg(win_id,msg_id,param);
       break;

    case MSG_LCD_SWITCH:
#ifdef MMIWIDGET_WEATHER_ANIM
    case MSG_WEATHER_NEED_ANIM:
    case MSG_WEATHER_FORBID_ANIM:
#endif
        HandleIdleMsg(win_id,msg_id,param);
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
        break;

    //双击耳机键，呼出最近一次拨出电话
    case MSG_APP_HEADSET_BUTTON: 
        MMK_NeedKbdClkNotifyOnce();
        break;

     case MSG_KEY_DBLCLK:
        MMIAPICL_MakeCallByRecordIndex(MMICL_CALL_DIALED,0);
        break;
        
       
   default:
#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
       if ( MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle() )
       {
           recode = MMIIDLE_IStyleHandleMsg(win_id,msg_id,param);
       }
       else
#endif
       {
           HandleIdleMsg(win_id,msg_id,param);
           recode = MMI_RESULT_FALSE;
       }
       break;
    }
    
#ifdef MMI_VIDEOWALLPAPER_SUPPORT    
    if ( MMI_RESULT_TRUE == MMIAPISETVWP_HandleVideoWallPaper(win_id, msg_id, param) )
    {
        recode = MMI_RESULT_TRUE;
    }
#endif

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMIAPILIVEWALLPAPER_HandleEvent(win_id, msg_id, param);
#endif

    return (recode);
}

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIIDLE_GetMainAppFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_GetMainAppFgInfo(LW_FG_INFO *lw_fg_info_ptr)
{
    MMISET_IDLE_STYLE_E idle_style = MMISET_IDLE_STYLE_COMMON;


    if(PNULL == lw_fg_info_ptr)
    {
        return;
    }
    idle_style = MMIAPISET_GetIdleStyle();

    switch(idle_style)
    {
#ifdef MMI_GRID_IDLE_SUPPORT
        case MMISET_IDLE_STYLE_GRID:
            MMIAPIGRID_GetMainAppFgInfo(lw_fg_info_ptr);//获得GRID风格下动态壁纸的信息
            break;
#endif
#if defined QBTHEME_SUPPORT
        case MMISET_IDLE_QBTHEME:
            MMIAPIQBTHEME_GetMainAppFgInfo(lw_fg_info_ptr);//获得GRID风格下动态壁纸的信息
            break;
#endif
#if defined MMI_ISTYLE_SUPPORT
        case MMISET_IDLE_ISTYLE:
            lw_fg_info_ptr->disp_dev_info.lcd_id = 0;
            lw_fg_info_ptr->disp_dev_info.block_id = 0;
            lw_fg_info_ptr->lw_fg_pos_info.rc = MMITHEME_GetFullScreenRect();
            lw_fg_info_ptr->lw_fg_pos_info.x = 0;
            lw_fg_info_ptr->lw_fg_pos_info.y = 0;
            //MMIISTYLE_GetMainAppFgInfo();//获得iphone风格下动态壁纸的信息
            break;
#endif			
        default:
            lw_fg_info_ptr->disp_dev_info.lcd_id = 0;
            lw_fg_info_ptr->disp_dev_info.block_id = 0;
            lw_fg_info_ptr->lw_fg_pos_info.rc = MMITHEME_GetFullScreenRect();
            lw_fg_info_ptr->lw_fg_pos_info.x = 0;
            lw_fg_info_ptr->lw_fg_pos_info.y = 0;
            break;
    }
}
#endif
/*****************************************************************************/
//  Description : init the main application
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/

#ifdef FOTA_SUPPORT_CMIOT
LOCAL uint8 CheckFotaVer_timer_id = 0; 
extern int g_setting_upgrade_dl_percent;
extern char g_setting_upgrade_new_ver[150];
extern uint8 g_setting_upgrade_status;
extern void FOTA_upgrade_Alert(void);

LOCAL void CheckFotaVerTimerFunc(
                                uint8 timer_id,
                                uint32 param
                                )
{
        if(ZDT_SIM_Exsit())
        {
      		MMIAPISET_FotaFirstCheck();
        }
        SCI_TRACE_LOW("xx StartCheckFotaVerTimer ");
}


LOCAL void MMIZDT_StartCheckFotaVerTimer()
{
    if(CheckFotaVer_timer_id != 0)
    {
        MMK_StopTimer(CheckFotaVer_timer_id);
    }
    CheckFotaVer_timer_id =  MMK_CreateTimerCallback(60*1000, CheckFotaVerTimerFunc, 0, FALSE);
}

PUBLIC void MMIZDT_StopCheckFotaVerTimer()
{
    if(CheckFotaVer_timer_id != 0)
    {
        MMK_StopTimer(CheckFotaVer_timer_id);
    }
}
#endif

void MMIAPIIDLE_init(void)
{
    MMISET_CALL_RING_TYPE_E     call_ring_type = MMISET_CALL_RING;
    uint32 i = 0;
#ifndef FLASH_SUPPORT    
    s_idlewin_is_open = FALSE;
#endif
    MMIIDLE_InitWinInfo();

    //set s_idlewin_info.is_shake
    //get call ring type
    call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, CALL_RING_TYPE);

#ifdef MMI_GPRS_SUPPORT
    MMIAPIPDP_HandleIdleEvent();
#endif
//    MMIAPIPDP_SubscribeEvent(MMI_MODULE_COMMON, 
//                                                 HandlePdpEvent,
//                                                 MMIPDP_NOTIFY_EVENT_MAX,
//                                                  s_pdp_event);
    //SCI_TRACE_LOW:"MMAPIIDLE_init:call_ring_type is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINAPP_1710_112_2_18_2_23_36_9,(uint8*)"d",call_ring_type);
    switch (call_ring_type)
    {
    case MMISET_CALL_RING:
        MMIIDLE_GetIdleWinInfo()->is_shake = FALSE;
        MMIIDLE_GetIdleWinInfo()->is_silent = FALSE;
        break;

    case MMISET_CALL_VIBRA:
    case MMISET_CALL_VIBRA_BEFORE_RING:
    case MMISET_CALL_VIBRA_AND_RING:
        MMIIDLE_GetIdleWinInfo()->is_shake = TRUE;
        MMIIDLE_GetIdleWinInfo()->is_silent = FALSE;
        break;

    case MMISET_CALL_SILENT:
        MMIIDLE_GetIdleWinInfo()->is_shake = FALSE;
        MMIIDLE_GetIdleWinInfo()->is_silent = TRUE;
        break;

    default:
        //SCI_PASSERT(0,("MMIAPIIDLE_init: call_ring_type is error!"));
        //SCI_TRACE_LOW:"MMIAPIIDLE_init: call_ring_type is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINAPP_1732_112_2_18_2_23_36_10,(uint8*)"");
        break;
    }

    //set battery
    MMIIDLE_GetIdleWinInfo()->batlevel = MMIAPIPHONE_GetBatCapacity();

    //set call forward status
    SCI_MEMSET(MMIIDLE_GetIdleWinInfo()->is_forward, FALSE, (sizeof(BOOLEAN)*MMI_DUAL_SYS_MAX));

    //init line and read nv
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIIDLE_GetIdleWinInfo()->line_state[i] = MMIAPISET_GetCallLineInfo(i);
    }
    // set the main applicaiton function and the type
    g_main_app.ProcessMsg     = MAIN_HandleSysMsg;
    // 初始化状态栏控件
    GUIWIN_InitStbData(MMI_WIN_ICON_MAX);
    MMIDILE_StatusBarInit(); // 初始化状态栏数据

#ifdef MONOCHROME_LCD_SUPPORT  // dphone要关闭透明显示功能
    IMG_Turnoff_Transparent(TRUE);   
	IMG_EnableCustomerTransparentColor(TRUE,0xF81F);   
	IMG_Turnoff_Transparent(FALSE);   
#endif    

#ifdef FOTA_SUPPORT_CMIOT
    //MMIZDT_StartCheckFotaVerTimer(); //去掉开机检测
#endif

}


#ifdef MAINLCD_SIZE_128X64
/*******************************************************************
*      Description: Get idle win ID
*	Global resource dependence
*     Author:  alex
*     Note:
*/
PUBLIC MMI_HANDLE_T MAIN_GetIdleWinId(void)
{
	return MAIN_IDLE_WIN_ID;
}
#endif

/*****************************************************************************/
//  Description : open idle win
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenIdleWin(void)
{
    if( !MMK_IsOpenWin( MAIN_IDLE_WIN_ID ) )
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MAIN_IDLE_WIN_ID;
#ifdef FLASH_SUPPORT
        win_create.func = MMIAPIFlashWin_HandleMsg;
#else
        win_create.func = IdleWin_HandleMsg;
#endif
        win_create.win_priority = WIN_LOWEST_LEVEL;
        win_create.softicon_state = 0x127E;
        
        win_create.window_style = WS_NO_DEFAULT_STYLE|WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG;//idle窗口以anim控件为背景, 先画anim后画窗口
        
        //MMK_OpenWindow( s_idle_win_handle );
        MMK_CreateWindow( &win_create );
#ifdef LOW_MEMORY_SUPPORT
        MMK_SetWinMoveStyle(MAIN_IDLE_WIN_ID, MOVE_FORBIDDEN);        
#endif
    }
}


/*****************************************************************************/
//  Description : start the timer of no sim or sim error etc window
//  Global resource dependence : 
//  Author: Jassmine.meng
//  Note: 
/*****************************************************************************/
//TIMER_OPERATE_RESULT MAIN_StopIdleTimer(void)
//{
//    TIMER_OPERATE_RESULT recode = TIMER_INVALIDATE;
//
//    if (0 != s_sim_error_time_id)
//    {
//        if (!MMIIDLE_GetIdleWinInfo()->is_charge)
//        {
//            MMK_StopTimer(s_sim_error_time_id);
//            s_sim_error_time_id = 0;
//
//            recode = TIMER_STOP_SUCCESS;
//        }
//        else
//        {
//            recode = TIMER_STOP_FAILURE;
//        }
//    }
//    else
//    {
////        SCI_TRACE_LOW("MAIN_StopIdleTimer: the sim error timer has stoped!");
//        recode = TIMER_HAVE_STOPED;
//    }
//
//    return (recode);
//}

/*****************************************************************************/
//  Description : 点亮或关闭背光时应用需要做的处理
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
void MMIAPIIDLE_HandleLCDBacklight(
                            BOOLEAN is_light // [IN] TRUE: 点亮背光; FALSE: 关闭背光
                            )
{
    if (is_light)
    {
        // 启动相关的Timer，如果有必要，需要判断Timer所处的窗口是否处于Focus
        // 在这里启动Timer时，请使用MMK_CreateWinTimer函数

        // 启动Idle窗口中的Timer
     //    StartIdleTimer();
#if defined MMI_COMMON_IDLE_SUPPORT
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        MMIIDLECOM_StartIdleMp3Timer();
#endif
#endif
        StartAnimTimer();
#if defined(MMI_DUALMODE_ENABLE) && !defined (WIN32)
        //亮屏时Enable
        MNPHONE_SetTdMeasurementState(MNPHONE_TD_MEAS_ENABLE);
#endif
    }
    else
    {
        // 关闭相关的Timer

        // 关闭Idle窗口中的Timer
//         MustStopIdleTimer(); 
#if defined MMI_COMMON_IDLE_SUPPORT
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        MMIIDLECOM_StopIdleMp3Timer();
#endif
#endif
        StopAnimTimer();
#ifndef SUBLCD_SIZE_NONE
        //关闭小屏的Timer
        MMISUB_StopSubLcdTimer();
#endif
        //关闭sim error界面的Timer
        //MAIN_StopIdleTimer();
#if defined(MMI_DUALMODE_ENABLE) && !defined (WIN32)
        //暗屏时Disable
        MNPHONE_SetTdMeasurementState(MNPHONE_TD_MEAS_DISABLE);
#endif
    }
#ifdef BROWSER_SUPPORT
    MMIAPIBROWSER_CheckTimerDisable();
#endif
}

/*****************************************************************************/
//  Description : start Hard icon for tp
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void StartHdShortCutBarIcon(void)
{
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
    MMIAPIIDLE_StartHdShortCutBarIcon();
#endif
}
/*****************************************************************************/
//  Description : handle idle message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIdleMsg(
                         MMI_WIN_ID_T         win_id,
                         MMI_MESSAGE_ID_E     msg_id,
                         DPARAM               param
                         )
{
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    //get idle style
    idle_style = MMIAPISET_GetIdleStyle();
    switch (idle_style)
    {
#if defined MMI_COMMON_IDLE_SUPPORT
    case MMISET_IDLE_STYLE_COMMON:
        res = MMIIDLE_CommonHandleMsg(win_id,msg_id,param);
        break;
#endif
#if defined (MMI_SMART_IDLE_SUPPORT_SMALL_ICON)||defined (MMI_SMART_IDLE_SUPPORT_LARGE_ICON)
    case MMISET_IDLE_STYLE_SMART:
        res = MMIIDLE_SmartHandleMsg(win_id,msg_id,param);
        break;
#endif

#ifdef MMIWIDGET_SUPPORT
    case MMISET_IDLE_STYLE_WIDGET: 
        res = MMIAPIWIDGET_HandleMsg(win_id,msg_id,param);
        break;
#endif

#ifdef MMI_GRID_IDLE_SUPPORT
    case MMISET_IDLE_STYLE_GRID: 
        
        res = MMIAPIGRID_HandleMsg(win_id,msg_id,param);
        break;
#endif

#ifdef QBTHEME_SUPPORT
    case MMISET_IDLE_QBTHEME:      
        MMIAPIQBIDLE_HandleMsg(win_id,msg_id,param);
        break;
#endif

#if (defined MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
    case MMISET_IDLE_ISTYLE:      
        res = MMIIDLE_IStyleHandleMsg( win_id, msg_id, param );
        break;
#endif

    default:
        res = MMI_RESULT_FALSE;
        //SCI_TRACE_LOW:"HandleIdleMsg:idle_style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINAPP_1948_112_2_18_2_23_36_11,(uint8*)"d",idle_style);
        break;
    }

    return res;
}

#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : open schedule window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenScheduleWin(
                                    MMI_WIN_ID_T    win_id
                                    )
{
    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
}
#endif
//#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsChargeConnected(void)
{
	if(MMIIDLE_GetIdleWinInfo()->is_charge)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

/*****************************************************************************/
//  Description : handle idle plmn message.
//  Global resource dependence : 
//  Author: Michael.shi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenSelNetWorkWin(MN_DUAL_SYS_E dual_sys )
{    
#ifdef MMI_DUALMODE_ENABLE
    MMIAPISET_OpenSelNetWorkWin(dual_sys);
#else
    if (MMIAPIPHONE_IsSimAvailable(dual_sys))
    {
        MMIAPISET_OpenSelNetWorkWin(dual_sys);
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatusII(dual_sys);
    }    
#endif
}

//#endif


#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
/*****************************************************************************/
//  Description : to handle  add blacklist open setting query window
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFlyModeCloseQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
	uint16  sim_sys[MMI_DUAL_SYS_MAX] = {0};
	uint32 sim_num = 0;
    const char* s_emergency_call = CC_CALL_EMERGENCY_CALL;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id,TXT_IDLE_FLYMODE_DISABLE,STXT_CANCEL,FALSE);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:           
	 SetFlyMode();
	 /*sim_num = MMIAPIPHONE_GetPSReadyNum(sim_sys, MMI_DUAL_SYS_MAX);
	 MMIAPICC_MakeCall((MN_DUAL_SYS_E)sim_sys[0],
                                        (uint8 *)s_emergency_call,
                                        strlen(s_emergency_call),
                                        PNULL,
                                        PNULL,
                                        CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,
                                        PNULL);*/
        //关闭询问窗口
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //关闭询问窗口
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description :飞行模式设置的窗口处理函数
//  Global resource dependence : 
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyMode(void)
{
	BOOLEAN    is_fly_mode_on  = MMIAPISET_GetFlyMode();
	uint16     i               = 0;
	BOOLEAN is_ps_deactive = FALSE;
	if(is_fly_mode_on)
	{	
		is_ps_deactive = TRUE;
		for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
		{
			if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
			{
				is_ps_deactive = FALSE;
				break;
			}
		}
		if (is_ps_deactive)
		{
			is_fly_mode_on = !is_fly_mode_on;
			
			MMIAPISET_SetFlyMode(is_fly_mode_on);
			
			MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
			
			// 激活协议栈
			MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
			MMIAPIMMS_RestartAutodownMMS();
		}
		else
		{
			MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
			
			//MMK_CloseWin(win_id);
			return;
		}
	}
	else
	{
		SCI_TRACE_LOW("mmicc_app: SetFlyMode: is_fly_mode_on = %d", is_fly_mode_on);
	}
}
#endif
/*Edit by script, ignore 5 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527
