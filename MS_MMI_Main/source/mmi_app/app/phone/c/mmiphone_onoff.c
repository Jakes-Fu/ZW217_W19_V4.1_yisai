/*****************************************************************************
** File Name:      mmiphone_onoff.c                                          *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe phone power on/off         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create                                    *
******************************************************************************/

#define _MMIPHONE_ONOFF_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#include "mmiphone_onoff.h"
#include "mmiphone_internal.h"
#include "mmiphone_export.h"
#include "window_parse.h"
#include "guiblock.h"
#include "mmiidle_subwintab.h"
#include "mmiidle_export.h"
#include "mmi_id.h"
#ifdef TOUCH_PANEL_SUPPORT
#include "tp_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
//#include "mmienvset.h"
#include "mmienvset_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmipub.h"
#include "mmisd_export.h"
#ifdef FLASH_SUPPORT
#include "Mmiacc_id.h"
#endif
#include "run_mode.h"
#include "mmiset_image.h"
#include "watch_commonwin_export.h"
#include "mmialarm_image.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define ALM_30SEC_TIME                   30000

#if defined(ZDT_W18_COMMON_UI_POWERONOFF_RING)
#ifndef  WIN32
#define PHONE_STARTUP_DURING_TIME           3500//12000    // 以毫秒为单位的值  // Bug 1321510,1334016
#else
#define PHONE_STARTUP_DURING_TIME           1000//12000    // 以毫秒为单位的值
#endif
#define PHONE_POWER_OFF_DURING_TIME         3500//12000   // Bug 1321510,1334016
#else
#ifndef  WIN32
#define PHONE_STARTUP_DURING_TIME           1200//12000    // 以毫秒为单位的值  // Bug 1321510,1334016
#else
#define PHONE_STARTUP_DURING_TIME           1000//12000    // 以毫秒为单位的值
#endif
#define PHONE_POWER_OFF_DURING_TIME         1200//12000   // Bug 1321510,1334016
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL PHONE_STARTUP_CONDITION_E   s_startup_condition;    // 记录开机的原因
//LOCAL BOOLEAN       s_is_play_default = TRUE;   //TRUE:play default animation,FALSE:play file
LOCAL BOOLEAN   s_is_play_power_on_mp4; //TRUE:power on mp4, FALSE: power off MP4
LOCAL BOOLEAN   s_is_playing_mp4 = FALSE; //TRUE:playing mp4 as poweron/off anim, FALSE: NONE
LOCAL uint8 s_30sec_timer_id = 0; 
LOCAL uint8 s_phone_startup_timer_id = 0;// 正常开机时开机界面的持续时间的ID号   
#if defined(MMI_POWERONOFFMP4_ENABLE)
LOCAL uint8 * s_poweronoff_buffer_ptr = PNULL;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
extern BOOLEAN g_is_sim_only_check_status[MMI_DUAL_SYS_MAX];
#endif
extern MMISET_PWD_OPERATE_TYPE_E g_mmiset_pwd_type;
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 处理正常开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNormalStartupWindow(
                                             MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             );
/*****************************************************************************/
//  Description : 处理闹铃启动开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmStartupWindow (
                                             MMI_WIN_ID_T    win_id,    // STARTUP ALARM窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             );

/*****************************************************************************/
//  Description : 关机闹钟后，是否开机询问窗口处理函数
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartOrNotWindow( 
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : play power on mp4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayPowerOnOffMP4(BOOLEAN is_power_on);

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : HandleVideoPowerOnOffMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleVideoPowerOnOffMsg(VP_CALLBACK_T *msg_ptr);
#endif
#ifndef MMI_POWERONOFFMP4_ENABLE
/*****************************************************************************/
//  Description : PlayDefaultPowerOnOffAnimation
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayDefaultPowerOnOffAnimation(BOOLEAN is_power_on_off);    //TRUE:power on, FALSE:power off
#endif
/*****************************************************************************/
//  Description : call back function of PlayPowerOnOffMP4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
#ifdef VIDEO_PLAYER_SUPPORT
void PlayPowerOnOffMP4CallBack(VP_PLAY_END_RESULT_E result);
#else
void PlayPowerOnOffMP4CallBack(void);
#endif


/*****************************************************************/
//  Description :play more power on or off animation
//  Global resource dependence : none
//  Author: jian.ma
//  Note: 
/*****************************************************************/
LOCAL BOOLEAN PlayMorePowerOnOffAnimation(
                                          BOOLEAN is_power_on_off,  //TRUE:power on, FALSE:power off
                                          MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info
                                          );
/*****************************************************************************/
//  Description : 处理关机界面
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffWindow(
                                        MMI_WIN_ID_T     win_id,    // STARTUP NORMAL窗口的ID
                                        MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                        DPARAM              param       // 相应消息的参数
                                        );
// the window for startup normal

WINDOW_TABLE( PHONE_STARTUP_NORMAL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_THREE_LEVEL ),
    WIN_FUNC( (uint32)HandleNormalStartupWindow ),    
    WIN_ID( PHONE_STARTUP_NORMAL_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_HWICON),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};

// the window for startup alarm
WINDOW_TABLE( PHONE_STARTUP_ALARM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleAlarmStartupWindow ),    
    WIN_ID( PHONE_STARTUP_ALARM_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

// the window for power off
WINDOW_TABLE( PHONE_POWER_OFF_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_THREE_LEVEL ),
    WIN_FUNC( (uint32)HandlePowerOffWindow ),
//    WIN_STYLE(WS_DISABLE_LCD_SWITCH),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_ID( PHONE_POWER_OFF_WIN_ID ),
    WIN_STYLE(WS_DISABLE_HWICON),
    WIN_HIDE_STATUS,
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};
PUBLIC BOOLEAN MMIAPIPHONE_IsPoweringOff(void)
{
    return MMK_IsOpenWin(PHONE_POWER_OFF_WIN_ID);
}
/*****************************************************************************/
//  Description : 闹铃启动开机界面的初始化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_AlarmInit(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    // 记录开机方式
    s_startup_condition = STARTUP_ALARM;
    MMIDEFAULT_InitDefault(FALSE); //关机闹钟响，lcd背光默认是关闭的
    
    if(!MMIALMSVC_StartService()/*MMIAPIALM_StartAlarm(FALSE)*/)
    {
        //关机
        MMIDEFAULT_TurnOffBackLight();
        POWER_PowerOff();
    }
#ifndef SUBLCD_SIZE_NONE
    else
    {
        //设置小屏显示是关机状态，刷新字屏显示关机充电
        MMISUB_SetMsStatus(FALSE);
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_ALARM,PNULL);
        MMISUB_IsPermitUpdate(TRUE);
    }
#endif
}


/*****************************************************************************/
//  Description : ASSERT启动的初始化,直接进入idle界面
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_AssertInit(void)
{
    // 记录开机方式,虽然是ASSERT开机，但是除开机流程不一样之外，其余都一样的
    s_startup_condition = STARTUP_NORMAL;//STARTUP_ASSERT;
#ifndef SUBLCD_SIZE_NONE
    MMISUB_IsPermitUpdate(TRUE);
#endif
    // 进入idle界面
	#if defined(W217_UWS6121EG_NOGNSS_PATCH_20231019)
    //MMIAPIIDLE_OpenIdleWin();
    MMIAPI_SetStartUpState(MMI_STARTUP_STATE_ANIMATION);
    MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_ANIMATION_FINISH);
	#else
    MMIAPIIDLE_OpenIdleWin();
	#endif

    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
}


/*****************************************************************************/
//     Description : 正常开机校准后界面的初始化
//    Global resource dependence : 
//  Author: Robert
//    Note:
/*****************************************************************************/
void MMIAPIPHONE_NormalInitAfterCoordinate(void)
{

    MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_COORDINATE_FINISH);

#if 0//for StartupCtrl
    if (MMIAPISET_GetPowerOnPwdStatus())//开机密码开启
    {
        g_mmiset_pwd_type = MMISET_PWD_NORMAL;         
        
        //open input power on password window
        MMIAPISET_OpenPowerOnPwdWin();
    }
    else
    {
        if (MMIAPISET_GetFlyModeSetPowerOn())
        {
            MMIAPISET_SetFlyModeOpenPhone();
        }
        else
        {
            MMIAPIPHONE_NormalInitAfterPwd();
        }
    }
#endif
}

/*****************************************************************************/
//  Description : init a virtual window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_VirtualWinInit(void)
{
    MMI_APPLET_START_T start ={0};
    
    start.guid = SPRD_IDLE_APPLET_ID;
    
    MMK_StartApplet( &start );
}

/*****************************************************************************/
//  Description : 正常开机界面的初始化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_NormalInit(void)
{
    // 记录开机方式
    s_startup_condition = STARTUP_NORMAL;
#ifndef SUBLCD_SIZE_NONE    
    //子屏开机图画,不允许刷新   SUBLCD刷新必须在初始化之后
    MMISUB_DisplayIdlePic();
    MMISUB_IsPermitUpdate(TRUE);
#endif    
#ifdef MMI_SIM_LOCK_SUPPORT
    if(!MNPHONE_SIMLockDataIntegrityCheck())
    {         
        /*sim lock data error,the ME alert "sim Locked" then power off*/
        MMIPHONE_SIMLockedOpenShutOffWin();
        return;
    }
    else
    {
        MN_SIM_LOCK_STATUS_T* sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
        
        /*初始化SIM Lock Data*/
        SCI_MEMSET(sim_unlock_data,0x00,sizeof(MN_SIM_LOCK_STATUS_T));
        MMIAPIPHONE_UpdateSIMUnlockData();
    }
#endif        

#ifndef WIN32
    if (TRUE == RM_GetCalibrationPostMode())
    {
        uint8 i = 0;
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MMIPHONE_SetSimStatus(i, SIM_NOT_INITED);

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
            MNPHONE_PowerOnSimEx(i, FALSE);
#else

#ifdef UAL_TELE_SUPPORT
            ual_tele_sim_power_on(i, TRUE);
#else
            MNPHONE_PowerOnSimEx(i, TRUE);
#endif

#endif
        }
        SCI_TRACE_LOW("MMIAPIPHONE_NormalInit,Enter calibratiion post mode");
        return;
    }
    else
    {
        SCI_TRACE_LOW("MMIAPIPHONE_NormalInit,  not calibratiion post mode");
    }

#endif
    MMIAPI_StartupCtrl(MMI_STARTUP_NORMAL_INIT_ENTER);

#if 0//for StartupCtrl
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
    // 判定是否第一次开机， 是打开校准窗口
    if (MMIAPITP_IsPowerOnFirst() && !POWER_IsAutoTestMode())
    {
        MMIAPITP_COORDINATE_OpenWin(TRUE);
    }
    else
#endif
    {        
        if (MMIAPISET_GetPowerOnPwdStatus())//开机密码开启
        {
            g_mmiset_pwd_type = MMISET_PWD_NORMAL;         
            
            //open input power on password window
            MMIAPISET_OpenPowerOnPwdWin();
        }
        else
        {
            if (MMIAPISET_GetFlyModeSetPowerOn())
            {
                MMIAPISET_SetFlyModeOpenPhone();
            }
            else
            {
                MMIAPIPHONE_NormalInitAfterPwd();
            }
        }
    }
#endif
}

/*****************************************************************************/
//  Description : 正常开机输入开机密码后界面的初始化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_NormalInitAfterPwd(void)
{    
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        g_is_sim_only_check_status[i] = TRUE;
    }
#ifndef SUBLCD_SIZE_NONE
    //子屏开机图画,不允许刷新
    MMISUB_DisplayIdlePic();
    MMISUB_IsPermitUpdate(TRUE);
#endif
    MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, FALSE);
#else
    MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);
#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
    MMIAPIPHONE_QuerySimisPlugIn(); //支持软件热插拔的时候，开机后起timer查询sim卡是否存在
#endif
#ifndef SUBLCD_SIZE_NONE
    //子屏开机图画,不允许刷新
    MMISUB_DisplayIdlePic();
    MMISUB_IsPermitUpdate(TRUE);
#endif    
    // temp_for_call
    //MMIAPIIDLE_OpenIdleWin();  // Bug 1321510,1334016

// 打开正常开机窗口
    MMIAPIPHONE_OpenNormalStartUpWin();  // Bug 1321510,1334016
#endif
}


/*****************************************************************************/
//  Description : get phone start up condition
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:  
/*****************************************************************************/
PUBLIC PHONE_STARTUP_CONDITION_E MMIAPIPHONE_GetStartUpCondition(void)
{
    return s_startup_condition;
}

/*****************************************************************************/
//  Description : set phone start up condition
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetStartUpCondition(PHONE_STARTUP_CONDITION_E e_start_cond)
{
    if(e_start_cond >= STARTUP_MAX)
    {
        //SCI_TRACE_LOW:"[POWERONOFF]:MMIAPIPHONE_IsNetworkSelecting e_start_cond = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_381_112_2_18_2_45_25_223,(uint8*)"d",e_start_cond);
        return ;
    }
    
    s_startup_condition = e_start_cond;
}
/*****************************************************************************/
//  Description : 处理正常开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNormalStartupWindow(
                                             MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             )
{   
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
        {
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
            GUI_RECT_T   rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};

            LCD_FillRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, rect, MMI_WHITE_COLOR);    
       }
#endif 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIDEFAULT_TurnOnBackLight();
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
#endif
        //s_is_play_default = TRUE;
#ifndef MMI_POWERONOFFMP4_ENABLE
        s_is_play_power_on_mp4 = TRUE; 
        SCI_MEMSET(&power_on_off_anim_info,0,sizeof( MMISET_POWER_ON_OFF_ANIM_INFO_T));
        MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, TRUE);

        //SCI_TRACE_LOW:"[POWERONOFF]:HandleNormalStartupWindow type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_421_112_2_18_2_45_25_224,(uint8*)"d",power_on_off_anim_info.anim_type);

        if (MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
        {
            PlayDefaultPowerOnOffAnimation(TRUE);
        }
        else if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type) 
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
        {
            if(!MMIAPIFMM_IsFileExist(power_on_off_anim_info.more_anim_info.name_wstr,power_on_off_anim_info.more_anim_info.name_wstr_len))
            {
                PlayDefaultPowerOnOffAnimation(TRUE);
            }
            else
            {    
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                {
                    PlayMorePowerOnOffAnimation(TRUE,power_on_off_anim_info);
                }
            }
        }
#endif
        if (s_phone_startup_timer_id != 0)
        {
            MMK_StopTimer(s_phone_startup_timer_id);
            s_phone_startup_timer_id = 0;
        }
        s_phone_startup_timer_id = MMK_CreateTimer(PHONE_STARTUP_DURING_TIME, FALSE);
        PlayPowerOnOffMP4(TRUE); 
        break;

    case MSG_TIMER:
        if (s_phone_startup_timer_id == *((uint8 *) param))
        { 
            s_phone_startup_timer_id = 0;
            
            // 检查Alarm的情况
            MMIAPIALM_CheckAndStartDelayedAlarm();            
            
            // 关闭本窗口
            //MMK_CloseWin(win_id);
            
            SCI_MEMSET(&power_on_off_anim_info, 0, sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T));
            MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, TRUE);
#ifdef VIDEO_PLAYER_SUPPORT 
            if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type)
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
            {
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                {
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                }
                else
                {
#ifdef MMI_POWERONOFFMP4_ENABLE
                    MMIAPIVP_ExitPlay();
#else
                    //if(s_is_play_default)//cr151078
                    //{
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                    //}
#endif
                }
            }
            else if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
            {

#ifdef MMI_POWERONOFFMP4_ENABLE
                MMIAPIVP_ExitPlay();
#else
                PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
#endif

            }
#else
            PlayPowerOnOffMP4CallBack();
#endif
            // 打开IDLE界面 
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
     
    case MSG_LOSE_FOCUS: 
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
#ifdef VIDEO_PLAYER_SUPPORT
         MMIAPIVP_ExitPlay();
#endif
        //cr124655, 当开机窗口被打断，需要中止开机铃声
        MMIAPISET_StopRing(MMISET_RING_TYPE_POWER);
        //MMIAPIIDLE_OpenIdleWin();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
        // 关闭开机窗口定时器
        if( 0 != s_phone_startup_timer_id )
        {
            // 检查Alarm的情况
            MMIAPIALM_CheckAndStartDelayedAlarm();
            MMK_StopTimer(s_phone_startup_timer_id);
            s_phone_startup_timer_id = 0;
            //MMIAPIIDLE_OpenIdleWin();
        }
#ifdef VIDEO_PLAYER_SUPPORT
        MMIAPIVP_ExitPlay();
#endif
#if defined(MMI_POWERONOFFMP4_ENABLE)
		if (PNULL != s_poweronoff_buffer_ptr)
		{
			SCI_FREE(s_poweronoff_buffer_ptr);
			s_poweronoff_buffer_ptr = PNULL;
		}
#endif
        //cr124655, 当开机窗口被打断，需要中止开机铃声
        MMIAPISET_StopRing(MMISET_RING_TYPE_POWER);
        MMIAPIPHONE_SetPowerAnimMpeg4Flag(FALSE);
#ifdef MSDC_CARD_SUPPORT        
        MMIAPISD_CheckAllDelayedDevice();
#endif
        //MMIAPIPHONE_ResumeAllDelaiedEvent();
//        if (MMIAPIUdisk_IsUSBDelay())
//        {        
//            MMIAPIUdisk_OpenUsbOperWindow();
//            MMIAPIUdisk_SetUSBDelay(FALSE);     
//         }
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMI_RestoreFreq();
        MMIAPI_StartupCtrl( MMI_STARTUP_EVENT_ANIMATION_FINISH );
        break;
       
#ifdef LCD_SLIDE_SUPPORT
    case MSG_KEYDOWN_SLIDE:
    case MSG_KEYUP_SLIDE:
#endif     
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_RED:
    case MSG_KEYLONG_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 处理闹铃启动开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmStartupWindow (
                                             MMI_WIN_ID_T    win_id,    // STARTUP ALARM窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_STRING_T     display_str = {0};

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (s_30sec_timer_id != 0)
        {
            MMK_StopTimer(s_30sec_timer_id);
            s_30sec_timer_id = 0;
        }
        s_30sec_timer_id = MMK_CreateWinTimer(win_id, ALM_30SEC_TIME, FALSE);
        break;
        
    case MSG_FULL_PAINT:
        //设置闹钟背景图
        if(MMK_IsOpenWin(MMIPUB_QUERY_WIN_ID))
        {
            MMK_CloseWin(MMIPUB_QUERY_WIN_ID);
        }
        //询问是否开机!!
        MMI_GetLabelTextByLang( TXT_STARTUP_OR_NOT, &display_str );
#ifndef ADULT_WATCH_SUPPORT
        {
            WATCH_SOFTKEY_TEXT_ID_T softkey = {0};
            WatchCOM_QueryWin_2Btn_Enter(MMIPUB_QUERY_WIN_ID,
                                          &display_str,
                                          PNULL,
                                          PNULL,
                                          res_aw_alarm_ic_close,
                                          IMAGE_SET_COM_BTN_POWERON,
                                          softkey,
                                          HandleStartOrNotWindow);
        }
#else
        {
            AdultWatchCOM_QueryWin_2Btn_Enter(MMIPUB_QUERY_WIN_ID,
                                              &display_str,
                                              PNULL,
                                              PNULL,
                                              IMAGE_SET_COM_AW_BTN_CANCEL,
                                              IMAGE_SET_COM_AW_BTN_CONFIRM,
                                              HandleStartOrNotWindow);
        }
#endif
        break;
        
    case MSG_TIMER:
        if (s_30sec_timer_id == *((uint8 *)param))
        {
            MMK_StopTimer(s_30sec_timer_id);
            s_30sec_timer_id = 0;
            
            //关闭询问窗口
            MMIPUB_CloseQuerytWin(PNULL);
            
            //destroy闹铃界面
            MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
            
            if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
            {
                //POWER_PowerOff()之前关闭背光
                MMIDEFAULT_TurnOffBackLight();
                MMIDEFAULT_SetKeybadBackLight(FALSE);
                
                // 关机
                POWER_PowerOff();
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_PROMPTWIN_OK:
    case MSG_KEYLONG_RED:
        MMK_StopTimer(s_30sec_timer_id);
        s_30sec_timer_id = 0;
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        
        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
        
        // 开机
        MMIAPIPHONE_PowerReset();
        // 记录开机方式
        MMIAPIPHONE_SetStartUpCondition(STARTUP_NORMAL);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
    case MSG_APP_RED:
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.    
    case MSG_APP_FLIP:
#endif    
        MMK_StopTimer(s_30sec_timer_id);
        s_30sec_timer_id = 0;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
        if(MSG_APP_FLIP == msg_id)
        {
            MMIDEFAULT_HandleFlipKey(FALSE);
        }
#endif
        
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        
        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
        
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            //POWER_PowerOff()之前关闭背光
            MMIDEFAULT_TurnOffBackLight();
            MMIDEFAULT_SetKeybadBackLight(FALSE);
            
            // 关机
            POWER_PowerOff();
        }
        break;
    case MSG_CLOSE_WINDOW:
        if(0!=s_30sec_timer_id)
        {
            MMK_StopTimer(s_30sec_timer_id);
            s_30sec_timer_id = 0;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 关机闹钟后，是否开机询问窗口处理函数
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartOrNotWindow( 
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_KEYLONG_RED:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        
        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
        
        // 开机
        MMIAPIPHONE_PowerReset();
        // 记录开机方式
        MMIAPIPHONE_SetStartUpCondition(STARTUP_NORMAL);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        
        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
        
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            // 关机
            MMIDEFAULT_TurnOffBackLight();
            MMIDEFAULT_SetKeybadBackLight(FALSE);
            POWER_PowerOff();
        }
        break;
#endif

#ifdef SCREEN_SHAPE_CIRCULAR
    //|√|
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    {
        //phone on
        MMK_StopTimer(s_30sec_timer_id);
        s_30sec_timer_id = 0;
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);

        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

        // 开机
        MMIAPIPHONE_PowerReset();
        // 记录开机方式
        MMIAPIPHONE_SetStartUpCondition(STARTUP_NORMAL);
        break;
    }
    //|x|
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {
        //cancel
        MMK_StopTimer(s_30sec_timer_id);
        s_30sec_timer_id = 0;

        //关闭询问窗口
        //MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(MMIPUB_QUERY_WIN_ID);

        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            //POWER_PowerOff()之前关闭背光
            MMIDEFAULT_TurnOffBackLight();
            MMIDEFAULT_SetKeybadBackLight(FALSE);

            // 关机
            POWER_PowerOff();
        }
        break;
    }
#endif
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 处理关机界面
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffWindow(
                                        MMI_WIN_ID_T     win_id,    // STARTUP NORMAL窗口的ID
                                        MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                        DPARAM              param       // 相应消息的参数
                                        )
{   
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static uint8    s_power_off_timer_id = 0;             // 关机界面持续时间的ID号
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
#endif
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/12
        {
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
            GUI_RECT_T   rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};

            LCD_FillRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, rect, MMI_WHITE_COLOR);    
       }
#endif 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIAPISET_StopAllRing(TRUE);  
        //s_is_play_default = TRUE;
#ifndef MMI_POWERONOFFMP4_ENABLE
        s_is_play_power_on_mp4 = FALSE; 
        SCI_MEMSET(&power_on_off_anim_info,0,sizeof( MMISET_POWER_ON_OFF_ANIM_INFO_T));
        MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, FALSE);
        if (MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
        {
            PlayDefaultPowerOnOffAnimation(FALSE);
        }
        else if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type) 
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
        {
            if(!MMIAPIFMM_IsFileExist(power_on_off_anim_info.more_anim_info.name_wstr,power_on_off_anim_info.more_anim_info.name_wstr_len))
            {
                PlayDefaultPowerOnOffAnimation(FALSE);
            }
            else
            {    
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                {
                    PlayMorePowerOnOffAnimation(FALSE,power_on_off_anim_info);
                }
            }

        }
#endif
        // 开启窗口定时器
        if (s_power_off_timer_id != 0)
        {
            MMK_StopTimer(s_power_off_timer_id);
            s_power_off_timer_id = 0;
        }
        s_power_off_timer_id = MMK_CreateTimer(PHONE_POWER_OFF_DURING_TIME, FALSE);
        break;
        
    case MSG_FULL_PAINT:

        #ifdef VIDEO_PLAYER_SUPPORT 
        MMIAPIVP_ExitPlay();//andy.he_crNEWMS00116228
        #endif
        
        PlayPowerOnOffMP4(FALSE);
        break;

    case MSG_TIMER:
        if (s_power_off_timer_id == *((uint8 *) param))
        {
            // 关闭关机窗口定时器
            MMK_StopTimer(s_power_off_timer_id);
            s_power_off_timer_id = 0;                    
            SCI_MEMSET(&power_on_off_anim_info, 0, sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T));
            MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, FALSE);
#ifdef VIDEO_PLAYER_SUPPORT               
            if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type)
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
            {
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                {
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                }
                else
                {
#ifdef MMI_POWERONOFFMP4_ENABLE
                    MMIAPIVP_ExitPlay();
#else
                    //if(s_is_play_default)//cr151078
                    //{
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                    //}
#endif
                }

            }
            else if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
            {
#ifdef MMI_POWERONOFFMP4_ENABLE
                MMIAPIVP_ExitPlay();
#else
                PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
#endif
            }
#else
            PlayPowerOnOffMP4CallBack();
#endif
            
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_GET_FOCUS:  
        MMK_CloseWin(win_id);        
        break;
        
    case MSG_LOSE_FOCUS: 
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
#ifdef VIDEO_PLAYER_SUPPORT
       MMIAPIVP_ExitPlay();
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
        // 关闭关机窗口定时器
        if (s_power_off_timer_id != 0)
        {
            MMK_StopTimer(s_power_off_timer_id);
            s_power_off_timer_id = 0;
        }

#ifdef VIDEO_PLAYER_SUPPORT
        MMIAPIVP_ExitPlay();
#endif
#if defined(MMI_POWERONOFFMP4_ENABLE)
		if (PNULL != s_poweronoff_buffer_ptr)
		{
			SCI_FREE(s_poweronoff_buffer_ptr);
			s_poweronoff_buffer_ptr = PNULL;
		}
#endif
        //关机
        MMIDEFAULT_TurnOffBackLight();
        MMIDEFAULT_SetKeybadBackLight(FALSE);
        //any way
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        //为了消除耳机模式情况下出现的pu音
        MMIAPISET_StopAllRing(TRUE);
        SCI_Sleep(2000);
#ifdef ENG_SUPPORT
        MMIAPIENG_SaveLastTime();
#endif
        POWER_PowerOff();
        break;
        
#ifdef LCD_SLIDE_SUPPORT
    case MSG_KEYDOWN_SLIDE:
    case MSG_KEYUP_SLIDE:
#endif
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_RED:
    case MSG_KEYLONG_RED:
	case MSG_KEYUP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Play mpeg4 as power on off animation.
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: Modified by apple.zhang
/*****************************************************************************/
LOCAL BOOLEAN PlayPowerOnOffMP4(BOOLEAN is_power_on_off)    //TRUE:power on, FALSE:power off
{
#ifdef VIDEO_PLAYER_SUPPORT   
    MMIAUD_RING_DATA_INFO_T         file_info = {0};/*lint !e64*/ 
    uint8                           power_midi_volume = MMISET_VOL_ZERO;
    VP_PLAY_PARAM_T                     play_param  = {0};
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
    GUI_RECT_T                     full_rect = {0};
   

    s_is_play_power_on_mp4 = is_power_on_off;
    //SCI_TRACE_LOW:"[POWERONOFF]:PlayPowerOnOffMP4:is_power_on=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_970_112_2_18_2_45_26_225,(uint8*)"d", is_power_on_off);
    
    SCI_MEMSET(&file_info, 0, sizeof(MMIAUD_RING_DATA_INFO_T));  
    SCI_MEMSET(&power_on_off_anim_info,0,sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T));
    
    MMIAPISET_GetPowerOnOffAnimInfo(
        &power_on_off_anim_info, 
        is_power_on_off
        );
    
    if(0 == MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1,POWER_RING_SWITCH))
    {
        power_midi_volume = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_POWER);
    }
    MMIAPIPHONE_SetPowerAnimMpeg4Flag(TRUE);

    SCI_MEMSET(&play_param,0,sizeof(VP_PLAY_PARAM_T));
    play_param.dual_sys = MN_DUAL_SYS_1;
    play_param.is_random_play = FALSE;
    play_param.mode = VP_REPEAT_PLAY_MODE_CLOSE;
    play_param.url_ptr = PNULL;
    play_param.display_param.audioSupport = 1;
    play_param.display_param.videoSupport = 1;
    play_param.is_scal_up = TRUE;
    play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 
    full_rect = MMITHEME_GetFullScreenRect();

    if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
    {
        play_param.display_param.RotateAngle = LCD_ANGLE_90;
        play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
    }
    else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
    {
        play_param.display_param.RotateAngle = LCD_ANGLE_270;
        play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
    }
    else
    {
        play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
    }
                
    

    play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
    play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
    play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
    play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;

    play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;

    switch (power_on_off_anim_info.anim_type)
    {
    case MMISET_POWER_ON_OFF_ANIM_DEFAULT:
#if defined(MMI_POWERONOFFMP4_ENABLE)
        MMI_GetRingInfo(OPEN_MP4, &file_info); 
        play_param.play_type = VP_PLAY_FROM_BUFFER;
        play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_3GP;
        play_param.video_buf_info.video_buf_len = file_info.data_len;
        play_param.video_buf_info.video_buf_ptr = SCI_ALLOCA(file_info.data_len);
		if (PNULL == play_param.video_buf_info.video_buf_ptr)
		{
			PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
			break;
		}
		SCI_MEMSET(play_param.video_buf_info.video_buf_ptr,0,file_info.data_len);
		SCI_MEMCPY(play_param.video_buf_info.video_buf_ptr,file_info.data_ptr,file_info.data_len);
		s_poweronoff_buffer_ptr = play_param.video_buf_info.video_buf_ptr;    
        play_param.vol = power_midi_volume;
        if(!MMIAPIVP_Play(&play_param, HandleVideoPowerOnOffMsg, 0))
        {
            PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
        }
#endif
        break;


    case MMISET_POWER_ON_OFF_ANIM_MORE:
    case MMISET_POWER_ON_OFF_PIC_MORE:
        if(MMIAPIFMM_IsFileExist(power_on_off_anim_info.more_anim_info.name_wstr,power_on_off_anim_info.more_anim_info.name_wstr_len))
        {   
            uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
            uint16 suffix_len = MMIFMM_FILENAME_LEN;
            MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                power_on_off_anim_info.more_anim_info.name_wstr_len, 
                PNULL, PNULL, suffix_wstr, &suffix_len);
            if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
            {
                PlayMorePowerOnOffAnimation(is_power_on_off,power_on_off_anim_info);
            }
            else
            {
                play_param.play_type = VP_PLAY_FROM_FILE;
                play_param.full_path_len = power_on_off_anim_info.more_anim_info.name_wstr_len;
                play_param.full_path_ptr = power_on_off_anim_info.more_anim_info.name_wstr; 
                play_param.vol = power_midi_volume;
                //s_is_play_default = FALSE;
                if(!MMIAPIVP_Play(&play_param, HandleVideoPowerOnOffMsg, 0))
                {
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                }  
            }
        }
        else
        {
            #if defined(MMI_POWERONOFFMP4_ENABLE)
            MMI_GetRingInfo(OPEN_MP4, &file_info); 
            play_param.play_type = VP_PLAY_FROM_BUFFER;
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_3GP;
            play_param.video_buf_info.video_buf_len = file_info.data_len;
			play_param.video_buf_info.video_buf_ptr = SCI_ALLOCA(file_info.data_len);
			if (PNULL == play_param.video_buf_info.video_buf_ptr)
			{
				PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
				break;
			}
			SCI_MEMSET(play_param.video_buf_info.video_buf_ptr,0,file_info.data_len);
			SCI_MEMCPY(play_param.video_buf_info.video_buf_ptr,file_info.data_ptr,file_info.data_len);
			s_poweronoff_buffer_ptr = play_param.video_buf_info.video_buf_ptr;    
              
            play_param.vol = power_midi_volume;
            if(!MMIAPIVP_Play(&play_param, HandleVideoPowerOnOffMsg, 0))
            {
                PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
            }            
            #endif
        }
        break;

    default:
        //SCI_TRACE_LOW:"[POWERONOFF]:PlayPowerOnOffMP4:anim_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1086_112_2_18_2_45_27_226,(uint8*)"d", power_on_off_anim_info.anim_type);
        break;
    }
    
#endif
    return TRUE;

}
#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : call back function of PlayPowerOnOffMP4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
void PlayPowerOnOffMP4CallBack(VP_PLAY_END_RESULT_E result)
{   
    //SCI_TRACE_MID:"mmiphone_onoff.c, PlayPowerOnMP4CallBack result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1101_112_2_18_2_45_27_227,(uint8*)"d",result);

    /*是否处理待定*/
    //*
#if defined(MMI_POWERONOFFMP4_ENABLE)
	if (PNULL != s_poweronoff_buffer_ptr)
	{
		SCI_FREE(s_poweronoff_buffer_ptr);
		s_poweronoff_buffer_ptr = PNULL;
	}
#endif
    if(VP_PLAY_END_ERROR == result)
    {
        MMIAPISET_RetPowerOnOffAnimInfo(s_is_play_power_on_mp4);
    }
    
    if (s_is_play_power_on_mp4)
    {        
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        
        if(!MMIAPIIDLE_IdleWinIsOpen())
        {
            #ifdef FLASH_SUPPORT
            if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
               MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
               MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)||//puk 码
               MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID)||//防盗
               MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID)//USB
           #ifdef MMI_SIM_LOCK_SUPPORT
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
               )
            {
                if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
                    MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
                    MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)//puk 码
           #ifdef MMI_SIM_LOCK_SUPPORT
                           ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
                           ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
                    )
                {
                    //如果需要输入pin码，那么此处将不进入IDLE界面
                    //由pin码输入确认后在进入IDLE
                    MMIAPIPHONE_SetOpenIdleFlag(TRUE);
                }
                
                if(MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID))//防盗
                {
                   MMIAPIPHONE_SetScrDelCallBack(MMIMSD_INIT_INPUT_PSW_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForLock);  
                }
                if(MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))//USB
                {
                   MMIAPIPHONE_SetScrDelCallBack(MMIUDISK_USB_OPER_SELECT_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForUsb);  
                }
            }
            else
            {
               //MMIAPIIDLE_OpenIdleWin();//无特殊窗口，正常进入IDLE
               MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_ANIMATION_FINISH);
            }
            #else
            //MMIAPIIDLE_OpenIdleWin();
            MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_ANIMATION_FINISH);
            #endif
        }
        
        MMI_FreeRes(VIRTUAL_WIN_ID);//释放开始动画资源
    }
    else
    {
        MMIDEFAULT_TurnOffBackLight();
        MMIDEFAULT_SetKeybadBackLight(FALSE);
        MMK_CloseWin(PHONE_POWER_OFF_WIN_ID);
    }
    //MMI_Enable3DMMI(TRUE);
}

/*****************************************************************************/
//  Description : HandleVideoPowerOnOffMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleVideoPowerOnOffMsg(VP_CALLBACK_T *msg_ptr)
{
    //VP_PLAY_END_IND_T  *play_end_ind_ptr = PNULL;
    VP_PLAY_END_RESULT_E play_end_result = 0;
    
    if (PNULL == msg_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"[POWERONOFF]:HandleVideoPowerOnOffMsg msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1185_112_2_18_2_45_27_228,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {  
    case VP_STOP_IND: 
        play_end_result = VP_PLAY_END_END_SUCCESS;       
        PlayPowerOnOffMP4CallBack(play_end_result);
        break;   
  
    default:
        break;
    }
    
    
}
#else
/*****************************************************************************/
//  Description : call back function of PlayPowerOnOffMP4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
void PlayPowerOnOffMP4CallBack(void)
{       
#if defined(MMI_POWERONOFFMP4_ENABLE)
	if (PNULL != s_poweronoff_buffer_ptr)
	{
		SCI_FREE(s_poweronoff_buffer_ptr);
		s_poweronoff_buffer_ptr = PNULL;
	}
#endif
    if (s_is_play_power_on_mp4)
    {        
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        
        if(!MMIAPIIDLE_IdleWinIsOpen())
        {
            #ifdef FLASH_SUPPORT
            if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
               MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
               MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)||//puk 码
               MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID)||//防盗
               MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID)//USB
           #ifdef MMI_SIM_LOCK_SUPPORT
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
               )
            {
                if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
                    MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
                    MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)//puk 码
           #ifdef MMI_SIM_LOCK_SUPPORT
                       ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
                       ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
                    )
                {
                    //如果需要输入pin码，那么此处将不进入IDLE界面
                    //由pin码输入确认后在进入IDLE
                    MMIAPIPHONE_SetOpenIdleFlag(TRUE);
                }
                
                        if(MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID))//防盗
                        {
                           MMIAPIPHONE_SetScrDelCallBack(MMIMSD_INIT_INPUT_PSW_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForLock);  
                        }
                    if(MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))//USB
                        {
                           MMIAPIPHONE_SetScrDelCallBack(MMIUDISK_USB_OPER_SELECT_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForUsb);  
                        }
            }
            else
            {
               //MMIAPIIDLE_OpenIdleWin();//无特殊窗口，正常进入IDLE
               MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_ANIMATION_FINISH);
            }
            #else
            //MMIAPIIDLE_OpenIdleWin();
            MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_ANIMATION_FINISH);
            #endif
        }
        
        MMI_FreeRes(VIRTUAL_WIN_ID);//释放开始动画资源
    }
    else
    {        
        MMIDEFAULT_TurnOffBackLight();
        MMIDEFAULT_SetKeybadBackLight(FALSE);
        MMK_CloseWin(PHONE_POWER_OFF_WIN_ID);
    }
    //MMI_Enable3DMMI(TRUE);
}
#endif


#ifndef MMI_POWERONOFFMP4_ENABLE
/*****************************************************************************/
//  Description : PlayDefaultPowerOnOffAnimation
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayDefaultPowerOnOffAnimation(BOOLEAN is_power_on_off)    //TRUE:power on, FALSE:power off
{
//#ifdef LOW_MEMORY_SUPPORT
    GUI_LCD_DEV_INFO                lcd_dev_info = {0};
    GUI_RECT_T                      rect = MMITHEME_GetFullScreenRect();
//#endif
    MMI_WIN_ID_T                    win_id = 0;
    GUIANIM_INIT_DATA_T             anim_init = {0};
    MMI_CONTROL_CREATE_T            anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T             ctrl_info = {0};
    GUIANIM_DATA_INFO_T             data_info = {0};
    GUIANIM_DISPLAY_INFO_T          display_info = {0};
    uint8 active_mode_value =MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1,POWER_RING_SWITCH);
    //SCI_TRACE_LOW:"[POWERONOFF]:PlayDefaultPowerOnOffAnimation active_mode_value=%d is_power_on_off=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1285_112_2_18_2_45_27_229,(uint8*)"dd",active_mode_value,is_power_on_off);

    /* if power off, we should stop all the sound, and do not resume it. 
        so we request a virtual handl to stop them, and do not free the virtual handle */
    if(!is_power_on_off)
    {
        MMISRVAUD_ReqVirtualHandle("POWER OFF", MMISRVAUD_PRI_NORMAL);
    }
    if(active_mode_value==0)
    {
#ifdef CMCC_UI_STYLE
        if(is_power_on_off)
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
        }
        else
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER_OFF,PNULL);
        }
#else        
		//added by bao 此处增加判断，只在正常开关机才有铃声，其它情况下不播放开机声音
        if(1 == MMIZDT_GetFirstPWON())
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
        }
#endif
    }
    
    ctrl_info.is_ctrl_id = TRUE;

//#ifdef LOW_MEMORY_SUPPORT
    //先填充黑色，再播放动画小图标
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
//#endif
    
    if(is_power_on_off)
    {
        ctrl_info.ctrl_id = PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID;
#ifdef ADULT_WATCH_SUPPORT
        data_info.anim_id = IMAGE_POWER_ON_ANIM;
#else
        data_info.img_id = IMAGE_POWER_ON_PIC;  // Bug 1321510,1334016
#endif
        win_id = PHONE_STARTUP_NORMAL_WIN_ID;
    }
    else
    {
        ctrl_info.ctrl_id = PHONE_POWER_OFF_ANIMATEBOX_CTRL_ID;
#if defined(ZDT_W18_COMMON_UI_POWERONOFF_PIC)// W18 POWER OFF PIC same as ON
        data_info.img_id = IMAGE_POWER_ON_PIC;  // Bug 1321510,1334016
#else
        data_info.img_id = IMAGE_POWER_OFF_PIC;  // Bug 1321510,1334016
#endif
        win_id = PHONE_POWER_OFF_WIN_ID;
    }
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;

    //set anim control init param
    anim_init.both_rect.h_rect = anim_init.both_rect.v_rect = MMITHEME_GetFullScreenRect();
// Bug 1321510,1334016 start
    anim_init.both_rect.h_rect.bottom = anim_init.both_rect.h_rect.right;
    anim_init.both_rect.v_rect.bottom = anim_init.both_rect.v_rect.right;
// Bug 1321510,1334016 end

    //creat anim
    anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
    anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
    anim_ctrl.init_data_ptr     = &anim_init;
    anim_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&anim_ctrl);
    
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
#ifdef ADULT_WATCH_SUPPORT
    GUIANIM_PlayAnim(anim_ctrl.ctrl_id);
#endif
    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : open normal start up window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenNormalStartUpWin(void)
{
    if (!MMK_IsOpenWin(MAIN_IDLE_WIN_ID))
    {
        MMK_CreateWin((uint32*)PHONE_STARTUP_NORMAL_WIN_TAB,PNULL);
    }
}
/*****************************************************************************/
//  Description : Set the mpeg4 playing flag as power anim.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetPowerAnimMpeg4Flag(
                    BOOLEAN is_playing
                    )
{
    s_is_playing_mp4 = is_playing;
}

/*****************************************************************************/
//  Description : Set the mpeg4 playing flag as power anim.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPowerAnimMpeg4Flag(
                    void
                    )
{
    return s_is_playing_mp4;
}

/*****************************************************************************/
//  Description : get power on status when mp4 is playing
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPowerOnStatus(void)
{
    return (s_is_play_power_on_mp4);
}

/*****************************************************************/
//  Description :play more power on or off animation
//  Global resource dependence : none
//  Author: jian.ma
//  Note: 
/*****************************************************************/
LOCAL BOOLEAN PlayMorePowerOnOffAnimation(
                                          BOOLEAN is_power_on_off,  //TRUE:power on, FALSE:power off
                                          MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info
                                          )  
{
    MMI_WIN_ID_T                    win_id = 0;
    GUIANIM_INIT_DATA_T             anim_init = {0};
    MMI_CONTROL_CREATE_T            anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T             ctrl_info = {0};
    GUIANIM_FILE_INFO_T             file_info = {0};
    GUIANIM_DISPLAY_INFO_T          display_info = {0};
    GUI_LCD_DEV_INFO                lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T                      rect = MMITHEME_GetFullScreenRect();//{0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};

    if(MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1,POWER_RING_SWITCH)==0)
    {
#ifdef CMCC_UI_STYLE
        if(is_power_on_off)
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
        }
        else
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER_OFF,PNULL);
        }
#else        
        MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
#endif
    }
    
    ctrl_info.is_ctrl_id = TRUE;

    LCD_FillRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, rect, MMI_WHITE_COLOR);
    
    file_info.full_path_wstr_ptr = power_on_off_anim_info.more_anim_info.name_wstr;
    file_info.full_path_wstr_len = power_on_off_anim_info.more_anim_info.name_wstr_len;

    if(is_power_on_off)
    {
        ctrl_info.ctrl_id = PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID;
        win_id = PHONE_STARTUP_NORMAL_WIN_ID;
    }
    else
    {
        ctrl_info.ctrl_id = PHONE_POWER_OFF_ANIMATEBOX_CTRL_ID;
        win_id = PHONE_POWER_OFF_WIN_ID;
    }
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;

    //set anim control init param
    anim_init.both_rect.h_rect = anim_init.both_rect.v_rect = MMITHEME_GetFullScreenRect();
// Bug 1321510,1334016 start
    anim_init.both_rect.h_rect.bottom = anim_init.both_rect.h_rect.right;
    anim_init.both_rect.v_rect.bottom = anim_init.both_rect.v_rect.right;
// Bug 1321510,1334016 end

    //creat anim
    anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
    anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
    anim_ctrl.init_data_ptr     = &anim_init;
    anim_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&anim_ctrl);
    
    GUIANIM_SetParam(&ctrl_info,PNULL,&file_info,&display_info);
    GUIANIM_SetDefaultIcon(anim_ctrl.ctrl_id, IMAGE_NULL, IMAGE_NULL);

    return TRUE;
}
