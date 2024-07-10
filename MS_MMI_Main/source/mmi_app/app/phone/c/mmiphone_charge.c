/*****************************************************************************
** File Name:      mmiphone_charge.c                                         *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe phone charge                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create                                    *
******************************************************************************/

#define _MMIPHONE_CHARGE_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#include "mmiphone_charge.h"
#include "window_parse.h"
#include "sig_code.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmicc_export.h"
#include "mmipub.h"

#include "mmiidle_subwintab.h"
#include "mmiidle_export.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmiidle_statusbar.h"
#include "mmi_id.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#include "mmienvset_export.h"
#include "mmiudisk_export.h"
#include "mmidm_export.h"
#include "guires.h"
#include "mmiset_text.h"
#include "dualbat_drvapi.h"
#include "tp_export.h"
#ifdef JAVA_SUPPORT_IA
#include "mmijava_export.h"
#endif
#if defined IM_ENGINE_SOGOU
#include "mmiim_sogou.h"    
#endif
#include "mmiacc_id.h"

#include "watch_launcher_main.h"
#include "watch_charge_win.h"
#ifdef ZDT_APP_SUPPORT
#include "zdt_app.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#ifdef MAINLCD_SIZE_240X320
#define Charge_TIME_UP_MARGIN              12
#elif defined MAINLCD_SIZE_240X400
#define Charge_TIME_UP_MARGIN              15
#elif defined MAINLCD_SIZE_320X480
#define Charge_TIME_UP_MARGIN              19
#else
#define Charge_TIME_UP_MARGIN              6
#endif
#define UPDATETIME_TIMER_10SEC      (10*1000)

// battery capacity
#define MMIPHONE_BTRYCAP_NORMAL_COLOR   0xFE60//0xF4BD
#define MMIPHONE_BTRYCAP_LOWER_COLOR    0xF800
#ifdef MMI_GUI_STYLE_MINISCREEN 
#define MMIPHONE_BERYCAP_FONT_COLOR  0x0000
#define MMIPHONE_BERYCAP_FONT_SIZE   SONG_FONT_14      
#endif
#define MMIPHONE_BTRYCAP_STR_LEN        64
#define MMIPHONE_MAX_BAT_LEVEL_SPAN     100
#define MMIPHONE_CHARGE_INTERVAL        60000
#define MMIPHONE_CHARGE_FULL_CAP        BATTERY_CHARGE_FULL_CAP
#define MMIPHONE_CHARGE_ZERO_CAP        0
#define MMIPHONE_CHARGE_WARNING_CAP_FIFT     15
#define MMIPHONE_CHARGE_WARNING_CAP_TEN     10
#define MMIPHONE_CHARGE_WARNING_CAP_FIVE     5
#define MMIPHONE_CHARGE_TIME                500
#define MMIPHONE_CHARGE_STARTUP_DELAY 4000

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMI_DELAIED_EVENT_E  s_delaied_events[MMI_DELAIED_EVENT_NUM] = {MMI_NONE_DELAY};
LOCAL uint8         s_charge_anim_timer_id = 0;
LOCAL uint8         s_is_charging_timer = 0;
LOCAL uint8         s_charging_updatetime_timer=0;
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
LOCAL uint32        s_pre_battery_cap = 0;
#endif
#if defined (PLATFORM_UWS6121E)
LOCAL uint32        s_charge_status_remain = CHR_MSG_CHARGE_START_IND;
#endif

#ifdef ZDT_PLAT_YX_SUPPORT
uint8        g_low_shudown_battery_cap = 0;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChrCapInd(void);

/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_START_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeStartInd(void);

/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_START_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeEndInd(BOOLEAN need_delay);

/*****************************************************************************/
//  Description : deal with signal of CHR_WARNING_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChrWarningInd(void);

/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_FINISH
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeFinsh(void);

/*****************************************************************************/
//  Description : deal with signal of CHR_MSG_CHARGE_FAULT_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeFaultInd(void);

/*****************************************************************************/
//  Description : deal with signal of CHR_MSG_CHARGE_FAULT_OVERTEMP_IND
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChangeOvertempInd(void);

#ifdef MMI_DUAL_BATTERY_SUPPORT
/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatCapInd(void);

/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_FINISH
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatFinsh(void);

/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatPlugInInd(DPARAM param);

/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatPlugOutInd(DPARAM param);
/*****************************************************************************/
//  Description : deal with signal of DBAT_MSG_SWITCH_BAT_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatSwitchInd(void);

#endif


/*****************************************************************************/
//  Description :  Handle Low Power Ring Win msg.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLowPowerRingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );

/*****************************************************************************/
//  Description : 处理充电启动开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: PHOEBE CHEN
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeStartupWindow (
                                              MMI_WIN_ID_T       win_id,    // STARTUP CHARGE窗口的ID
                                              MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                              DPARAM                param       // 相应消息的参数
                                              );
/*****************************************************************************/
//  Description : 处理充电启动开机界面电量已满的消息(除PS之外)
//  Global resource dependence : 
//  Author: PHOEBE CHEN
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeStartupOKWindow (
                                                MMI_WIN_ID_T     win_id,    // STARTUP CHARGE窗口的ID
                                                MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                                DPARAM              param       // 相应消息的参数
                                                );
/*****************************************************************************/
//  Description : 处理充电窗口
//  Global resource dependence : 
//  Author: PHOEBE CHEN
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtryCapacityWin(
                                         MMI_WIN_ID_T     win_id,   // STARTUP CHARGE窗口的ID
                                         MMI_MESSAGE_ID_E    msg_id,    // 窗口的内部消息ID
                                         DPARAM                 param       // 相应消息的参数
                                         );

/*****************************************************************************/
//  Description : SetBtryCapacityFormParam
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetBtryCapacityFormParam(void);

/*****************************************************************************/
//  Description : DisplayDayTimeEffert
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDayTimeEffert(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : BtryCapacityCtrlCallbackFun
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void BtryCapacityCtrlCallbackFun ( GUIANIM_OWNER_DRAW_T    *draw_ptr);

/*****************************************************************************/
//  Description : DisplayBtryCapacityStr
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void DisplayBtryCapacityStr(void);

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
//  Description : HandleOpenPowerSavQueryWin
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenPowerSavQueryWin(
                                    MMI_WIN_ID_T       win_id,    
                                    MMI_MESSAGE_ID_E      msg_id,    
                                    DPARAM                param      
                                 );
#endif

/*****************************************************************************/
//  Description : LowPower_PowerOff
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 低电量关机
/*****************************************************************************/
LOCAL void LowPower_PowerOff(void);

/*****************************************************************************/
//  Description : CheckLowBatTimerHandle
//  Global resource dependence : 
//  Author:feng.xiao
//  Note: 检查开机时电量，开机画面可能没有关闭时，low bat消息就发过来。为了保证
//        能弹出提示窗口，开启一个定时器查询开机窗口是否close.
/*****************************************************************************/
LOCAL void CheckLowBatTimerHandle(uint8  timer_id, uint32 param);
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
// the window for 关机充电
WINDOW_TABLE( PHONE_STARTUP_CHARGE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_LOWEST_LEVEL ),
    WIN_FUNC( (uint32)HandleChargeStartupWindow ),    
    WIN_ID( PHONE_STARTUP_CHARGE_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

// the window for 关机充电 OK
WINDOW_TABLE( PHONE_STARTUP_CHARGE_OK_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_LOWEST_LEVEL ),
    WIN_FUNC( (uint32)HandleChargeStartupOKWindow ),    
    WIN_ID( PHONE_STARTUP_CHARGE_OK_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

// the window for in charging
WINDOW_TABLE( PHONE_BTRY_CAP_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleBtryCapacityWin),
    WIN_TITLE(TXT_SET_POWER_MANAGEMENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
#ifdef MMI_DUAL_BATTERY_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPHONE_DISPLAY_BTRYCAP_FORM_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMIPHONE_DISPLAY_BTRYCAP_MAIN_FORM_ID,MMIPHONE_DISPLAY_BTRYCAP_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,MMIPHONE_DISPLAY_BTRYCAP_MAIN_FORM_ID),
            CHILD_ANIM_CTRL(FALSE,FALSE,MMIPHONE_DISPLAY_BTRYCAP_IMAGE_ID,MMIPHONE_DISPLAY_BTRYCAP_MAIN_FORM_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMIPHONE_DISPLAY_BTRYCAP_AUX_FORM_ID,MMIPHONE_DISPLAY_BTRYCAP_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPHONE_DISPLAY_BTRYCAP_AUX_LABEL_ID,MMIPHONE_DISPLAY_BTRYCAP_AUX_FORM_ID),
            CHILD_ANIM_CTRL(FALSE,FALSE,MMIPHONE_DISPLAY_BTRYCAP_AUX_IMAGE_ID,MMIPHONE_DISPLAY_BTRYCAP_AUX_FORM_ID),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPHONE_DISPLAY_BTRYCAP_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,MMIPHONE_DISPLAY_BTRYCAP_FORM_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPHONE_DISPLAY_BTRYCAP_IMAGE_ID,MMIPHONE_DISPLAY_BTRYCAP_FORM_ID),
#endif
    WIN_ID(PHONE_STARTUP_BTRY_CAP_WIN_ID),
    END_WIN
};

/*****************************************************************************/
//  Description : 充电启动开机界面的初始化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ChargeInit(void)
{
    BOOLEAN is_charge = TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CHARGE_WIN_ID;

//#ifdef MMI_DUAL_BATTERY_SUPPORT
//    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
//#endif

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
  //dbat_info->module_state状态是由charge.c的消息来更新的，
  //而这时刚开机还没来得及更新，所以这里请不要用MMI_DUAL_BATTERY_SUPPORT宏进行区分，
  //还是用以前单电池的机制is_charge = CHR_IsChargeConnect();来区分
/*#ifdef MMI_DUAL_BATTERY_SUPPORT
    if(DBAT_CONNECT_CHARGING==dbat_info->module_state)
    {
       is_charge = TRUE; 
    }
    else
    {
        is_charge = FALSE;
    }
    //SCI_TRACE_LOW:"MMIAPIPHONE_ChargeInit,is_charge =%d,dbat_info->module_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_307_112_2_18_2_45_19_177,(uint8*)"dd",is_charge,dbat_info->module_state);
#else*/
    is_charge = CHGMNG_IsChargeConnect();
//#endif
    if (is_charge)
    {
        // 记录开机方式
        MMIAPIPHONE_SetStartUpCondition(STARTUP_CHARGE);
        if (NULL == MMK_IsOpenWin(win_id))//bug 2113365
        {
            MMK_CreateTimerCallback(MMIPHONE_CHARGE_STARTUP_DELAY,  WatchLAUNCHER_Charge_Enter, NULL, FALSE);
        }

        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#ifndef SUBLCD_SIZE_NONE
        //设置小屏显示是关机状态，刷新字屏显示关机充电
        MMISUB_SetMsStatus(FALSE);
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_CHARGE_OFF,PNULL);
        MMISUB_IsPermitUpdate(TRUE);
#endif
    }
    else
    {
        //关机
        MMIDEFAULT_TurnOffBackLight();
        POWER_PowerOff();
    }
}
/*****************************************************************************/
//  Description : 处理PHONE APP的charge消息
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_ChargeHandlePsAndRefMsg(
                                     PWND                app_ptr, 
                                     uint16              msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch (msg_id)
    {
#ifdef MMI_DUAL_BATTERY_SUPPORT
    case DBAT_MSG_UPDATE_STATE_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_UPDATE_STATE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_346_112_2_18_2_45_19_178,(uint8*)"");
        //定时上报电池状态的消息
        result = HandleDbatCapInd();
        break;
    case DBAT_MSG_BAT_PLUGIN_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_BAT_PLUGIN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_351_112_2_18_2_45_19_179,(uint8*)"");
        result = HandleDbatPlugInInd(param);
        break;
    case DBAT_MSG_BAT_PLUGOUT_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_BAT_PLUGOUT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_355_112_2_18_2_45_19_180,(uint8*)"");
        result =HandleDbatPlugOutInd(param);
        break;
    case DBAT_MSG_SWITCH_BAT_IND://目前不处理
        result = HandleDbatSwitchInd();
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_SWITCH_BAT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_360_112_2_18_2_45_19_181,(uint8*)"");
        break;
    case DBAT_MSG_CHARGE_START_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_START_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_363_112_2_18_2_45_19_182,(uint8*)"");
        MMIDEFAULT_TurnOnBackLight();
        result = HandleChargeStartInd();// 和单电池公用该接口，里面的接口有修改
        break;
    case DBAT_MSG_WARNING_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_WARNING_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_368_112_2_18_2_45_19_183,(uint8*)"");
        result = HandleChrWarningInd();// 和单电池公用该接口，里面的接口有修改
        break;
    case DBAT_MSG_SHUTDOWN_IND://和单电池处理一致
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_SHUTDOWN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_372_112_2_18_2_45_19_184,(uint8*)"");
#ifdef VT_SUPPORT
        if(MMIAPIVT_IsVtConnecting())
        {
            MMIAPIVT_ReleaseVTCallByRedkey();   
        }
#endif   
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);
        }
        // 关闭所有的窗口
        MMK_ReturnIdleWin();
        // 该消息用于汇报此时电池电量非常低，已经无法支持系统正常运行，应该直接关机。
        // 关机     
        //MMIDEFAULT_TurnOffBackLight();
        
        #ifdef JAVA_SUPPORT_IA  //解决移动mm,低电量关闭，再次开机mm不能启动
            //低电量关机时同步关掉jvm
         MMIAPIJAVA_stopVM_sync();
        #endif
        MMIPHONE_PowerOff();
        break;
    case DBAT_MSG_CHARGE_FINISH:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_FINISH"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_396_112_2_18_2_45_19_185,(uint8*)"");
        HandleDbatFinsh();
        break;
    case DBAT_MSG_CHARGE_DISCONNECT:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_DISCONNECT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_400_112_2_18_2_45_19_186,(uint8*)"");
        MMIDEFAULT_TurnOnBackLight();
        result = HandleChargeEndInd(FALSE);
        break;
    case DBAT_MSG_CHARGE_FAULT:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_FAULT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_405_112_2_18_2_45_19_187,(uint8*)"");
        result = HandleChargeFaultInd();
        break;
#else
    case CHR_MSG_CAP_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_CAP_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_410_112_2_18_2_45_19_188,(uint8*)"");
        // 该消息用于汇报电池电量
        result = HandleChrCapInd();
        break;
        
    case CHR_MSG_CHARGE_START_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_START_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_416_112_2_18_2_45_19_189,(uint8*)"");
#ifdef ZDT_APP_SUPPORT
        ZDT_SetIsBatLow(0);
#endif
        if(MMIAPIPHONE_IsNeedDelayEvent(MMI_CHARGE_PLUG_IN_DELAY))
        {
#ifdef ZDT_APP_SUPPORT
		ZDT_SetIsCharge(TRUE);
#endif
            MAIN_SetIdleChargeState(TRUE);
			MMIAPIPHONE_CleanEventDelaied(MMI_CHARGE_PLUG_OUT_DELAY);
            MMIAPIPHONE_SetEventDelaied(MMI_CHARGE_PLUG_IN_DELAY);
        }
        else
        {
            MMIDEFAULT_TurnOnBackLight();        
            // 该消息用于汇报设备处于充电状态。界面上的电池显示为动画方式。
            result = HandleChargeStartInd();
        }
        break;
        
    case CHR_MSG_CHARGE_END_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_431_112_2_18_2_45_19_190,(uint8*)"");
        if(MMIAPIPHONE_IsNeedDelayEvent(MMI_CHARGE_END_DELAY))
        {
            HandleChargeEndInd(TRUE);
            MMIAPIPHONE_SetEventDelaied(MMI_CHARGE_END_DELAY);
        }
        else
        {
            if (MMIAPIPHONE_GetStartUpCondition() != STARTUP_CHARGE)
            {
            MMIDEFAULT_TurnOnBackLight();
            }
            // 该消息用于汇报设备充电已经完成。
            result = HandleChargeEndInd(FALSE);
        }
        break;
        
    case CHR_MSG_WARNING_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_WARNING_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_446_112_2_18_2_45_19_191,(uint8*)"");
        // 该消息用于汇报此时电池电量低。可以弹出窗口进行提示，并发出警告音。
        //在HandleChrCapInd中统一用百分比处理，因为这个消息在0%以后才会报上来。
        //result = HandleChrWarningInd();
        break;
        
    case CHR_MSG_SHUTDOWN_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_SHUTDOWN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_452_112_2_18_2_45_19_192,(uint8*)"");
        //因为现在的charge电量计算不是用的电压table，而是库仑计，所以电量与对应电压会产生偏差；
        //应Eric.zhou要求将关机操作从cap_ind主动判断修改为此处根据shutdown消息进行关机.
        LowPower_PowerOff();
        break;
        
    case CHR_MSG_CHARGE_FINISH_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FINISH_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_480_112_2_18_2_45_19_193,(uint8*)"");
        // 该消息用于汇报此时充电已满。
        result = HandleChargeFinsh();
        break;
        
    case CHR_MSG_CHARGE_DISCONNECT_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_DISCONNECT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_486_112_2_18_2_45_19_194,(uint8*)"");
        if(MMIAPIPHONE_IsNeedDelayEvent(MMI_CHARGE_PLUG_OUT_DELAY))
        {
            HandleChargeEndInd(TRUE);
			MMIAPIPHONE_CleanEventDelaied(MMI_CHARGE_PLUG_IN_DELAY);
            MMIAPIPHONE_SetEventDelaied(MMI_CHARGE_PLUG_OUT_DELAY);
        }
        else
        {
            if (MMIAPIPHONE_GetStartUpCondition() != STARTUP_CHARGE)
            {
            MMIDEFAULT_TurnOnBackLight();
            }
            // 该消息用于汇报此时充电被断开。
            result = HandleChargeEndInd(FALSE);
        }
        break;

    case CHR_MSG_CHARGE_FAULT_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FAULT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_501_112_2_18_2_45_19_195,(uint8*)"");
        MMIDEFAULT_SetBackLight(TRUE);
        // 在充电电压超过额电电压
        result = HandleChargeFaultInd();
        break;

#if defined (MODEM_SUPPORT_SPRD)
    case CHR_MSG_CHARGE_FAULT_OVERTEMP_IND:
#elif defined (MODEM_SUPPORT_RDA)
case CHR_MSG_CHARGE_OVER_TEMPTURE_IND:
    s_charge_status_remain = CHR_MSG_CHARGE_OVER_TEMPTURE_IND;
    SCI_TRACE_LOW("[charge] MMIPHONE_ChargeHandlePsAndRefMsg s_charge_status_remain = %d", s_charge_status_remain);
#endif

        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FAULT_OVERTEMP_IND"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_501_112_2_18_2_45_19_195_1,(uint8*)"");
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FAULT_OVERTEMP_IND");
        MMIDEFAULT_SetBackLight(TRUE);
        // charging over temp
        result = HandleChangeOvertempInd();
        break;
#endif
    default:
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#ifdef ZDT_APP_SUPPORT
PUBLIC uint8 MMIAPIPHONE_ZDT_GetBatCapacity(void);
#endif
LOCAL MMI_RESULT_E HandleChrCapInd(void)
{
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    uint8          num_str[16] = {0};
    wchar          wnum_str[8] = {0};  
    wchar          wquery_str1[40] = {0};
#endif
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMI_STRING_T    query_text1 = {0};
    MMI_STRING_T    query_text2 = {0};
    MMI_WIN_ID_T    query_win_id = MMI_CHR_POWER_SAVE_QUERY_WIN_ID;
#endif
    static uint32   alert_cap  = 0;

    switch (MMIAPIPHONE_GetStartUpCondition())
    {
    case STARTUP_NORMAL:
        {
            uint8  bat_level = MMIAPIPHONE_GetBatCapacity();
            uint32 bat_cap   = 0;
            CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
#ifdef ZDT_APP_SUPPORT
            if(!ZDT_GetIsCharge())
            {
                uint8 low_l = MMIAPIPHONE_ZDT_GetBatCapacity();
                if(low_l > 0)
                {
                    ZDT_SetIsBatLow(low_l);
                }
            }
#endif

            MAIN_SetIdleBatLevel(bat_level);
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
            MMIAPIBT_SetBatteryCharge(bat_level); //mdy by qing.yu@for bt_bqb
#endif

            if(PNULL == p_chgmng_info)
            {
                SCI_TRACE_LOW("HandleChrCapInd PNULL == p_chgmng_info");
                break;
            }

            bat_cap = p_chgmng_info->bat_remain_cap;
            
            if(MMIPHONE_CHARGE_FULL_CAP == bat_cap)//NEWMS00201875:防止电量为100%，MMI仍然处于充电动画
            {
                HandleChargeFinsh();
            }
#ifdef ZDT_APP_SUPPORT
            else
            {
                ZDT_SetIsBatFull(FALSE);
            }
#endif
            if(MMIIDLE_GetIdleWinInfo()->is_charge)
            {
                alert_cap = 0;
            }
            else
            {

                if(MMIPHONE_CHARGE_WARNING_CAP_FIFT == bat_cap
                    || MMIPHONE_CHARGE_WARNING_CAP_TEN == bat_cap
                    || MMIPHONE_CHARGE_WARNING_CAP_FIVE == bat_cap)//15,10,5提示一次
                {
#if 1
                    if(alert_cap != bat_cap)
                    {
                        HandleChrWarningInd();
                        alert_cap = bat_cap;
                    }
#else
                    WatchLAUNCHER_BatteryLow_Enter();
#endif
                }
                else if(MMIPHONE_CHARGE_ZERO_CAP == bat_cap)//NEWMS00206076
                {
                    //LowPower_PowerOff();
                    break;
                }
                //开机后第一次已经小于 5%
                else if(MMIPHONE_CHARGE_WARNING_CAP_FIVE > bat_cap && 0 == alert_cap)
                {
                    MMK_CreateTimerCallback(1000, CheckLowBatTimerHandle, NULL, FALSE);
//                     HandleChrWarningInd();
                    alert_cap = bat_cap;
                }
            }

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
            {
                MMISET_POWER_SETTING_ITEM_T power_sav_setting = MMIAPISET_GetPowerSavingSetting();
                uint32    ps_setting = 0;//用来转换枚举值

                if(MMIAPISET_GetPowerSavingSwitch())
                {
                    if(MMISET_POWER_SAVING_SETTING_TWE == power_sav_setting.schedule_setting)//20%
                    {
                        ps_setting = 20;                    
                    }
                    else if(MMISET_POWER_SAVING_SETTING_FOURTY == power_sav_setting.schedule_setting)//40%
                    { 
                        ps_setting = 40;                   
                    }
                    else if(MMISET_POWER_SAVING_SETTING_SIXTY == power_sav_setting.schedule_setting)//60%
                    {
                        ps_setting = 60;
                    }
                    
                    if(power_sav_setting.is_active)//cr160707省电模式开启后，电量高于设定值，要关闭省电模式
                    {
                        if(p_chgmng_info->bat_remain_cap > ps_setting)
                        {
                            power_sav_setting.is_active = FALSE;
                            MMIAPISET_SetPowerSavingSetting(power_sav_setting);
                            MMIAPIENVSET_ResetActModeOtherRingSet();
                        }
                    }
                    else
                    {
                        if(p_chgmng_info->bat_remain_cap <= ps_setting)
                        {
                            if (MMIAPISET_IsProgramRunning())
                            {
                                MMI_GetLabelTextByLang(
                                    TXT_BTRY_CAP_LABEL_TEXT, &query_text1);            
                                sprintf((char*)num_str, "%d%s", 
                                    (int)p_chgmng_info->bat_remain_cap, "%");
                                MMI_GetLabelTextByLang(
                                    TXT_CHR_OPEN_POWER_SAVING_QUERY2, &query_text2);
                                MMIAPICOM_StrToWstr(num_str, wnum_str);
                                MMIAPICOM_Wstrncpy(wquery_str1, 
                                    query_text1.wstr_ptr,query_text1.wstr_len);
                                MMIAPICOM_Wstrcat(wquery_str1, wnum_str);
                                query_text1.wstr_ptr = wquery_str1;
                                query_text1.wstr_len = MMIAPICOM_Wstrlen(
                                    wquery_str1);
                                
                                MMIPUB_OpenQueryWinByTextPtr(&query_text1, 
                                    &query_text2, IMAGE_PUBWIN_QUERY, &query_win_id, 
                                    HandleOpenPowerSavQueryWin);
                            }
                            else
                            {
                                MMIAPISET_OpenPowerSettingMode();
                                MMIPUB_OpenAlertWinByTextId(PNULL, 
                                    TXT_SET_POWER_SAVING_IS_STARTED, TXT_NULL, 
                                    IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                                    MMIPUB_SOFTKEY_ONE, PNULL);
                            }
                        }
                    }
                }
            }
#endif    
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
            {
                uint32   pre_battery_cap = MMIAPIPHONE_GetPreBatteryCap();
                
                if(pre_battery_cap != p_chgmng_info->bat_remain_cap)
                {
                    if(MMIAPISET_GetBatteryPercentDisSwitch())
                    {
                        GUIWIN_STBDATA_TEXT_T  text_info = {0};

                        text_info = MMIAPIIDLE_GetBatteryPercentStbTxt();
                        GUIWIN_SetStbItemText(MMI_WIN_ICON_BATTERY_PERCENT,&text_info);
                        GUIWIN_UpdateStb();
                    }
                }
            }
#endif
        //WatchSTB_UpdateBatteryText();
        //WatchSTB_UpdateBatteryIcon();
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#define CHARGE_TIP_DELAY 1000
LOCAL uint8 s_charge_tip_delay_timer = 0;
LOCAL void OpenChargeTipDelayCallback(uint8 timer_id, uint32 param)
{
    if (FALSE == MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))
    {
        //WatchLAUNCHER_ChargeTip_Enter();
        MMIZDT_OpenChargingWin();
    }
    if (0 != s_charge_tip_delay_timer)
    {
        s_charge_tip_delay_timer = 0;
    }
}

/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_START_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeStartInd(void)
{
#if 0
    MMI_WIN_ID_T    alert_win_id = PHONE_CHR_WARNING_WIN_ID;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch (MMIAPIPHONE_GetStartUpCondition())
    {
	case STARTUP_CHARGE:
		//启动idle状态下电池充电动画!
#ifdef ZDT_APP_SUPPORT
		ZDT_SetIsCharge(TRUE);
#endif
        MAIN_SetIdleChargeState(TRUE);
		if (MMK_IsOpenWin(PHONE_STARTUP_CHARGE_OK_WIN_ID))
        {
            MMK_CloseWin(PHONE_STARTUP_CHARGE_OK_WIN_ID);
        }
		MMIAPIPHONE_ChargeInit();
    	break;
    case STARTUP_NORMAL:
        //启动idle状态下电池充电动画!
#ifdef ZDT_APP_SUPPORT
        ZDT_SetIsCharge(TRUE);
#endif
        MAIN_SetIdleChargeState(TRUE);
        if(MMK_IsOpenWin(PHONE_CHR_WARNING_WIN_ID))
        {
            MMK_CloseWin(PHONE_CHR_WARNING_WIN_ID);//maryxiao cr 243401
        }        
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_USB_DEBUG);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CHARGE_START,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
#else 
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CHARGE_START,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
#ifdef ZDT_APP_SUPPORT
		//MMIZDT_OpenChargingWin();
#endif        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
#else
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CHARGE_WIN_ID;

    switch (MMIAPIPHONE_GetStartUpCondition())
    {
        case STARTUP_CHARGE:
        {
            
#ifdef ZDT_APP_SUPPORT// wuxx add for W217
                    ZDT_SetIsCharge(TRUE);
#endif
            if (NULL == MMK_IsOpenWin(win_id))//bug 2113365
            {
                MMK_CreateTimerCallback(MMIPHONE_CHARGE_STARTUP_DELAY,  WatchLAUNCHER_Charge_Enter, NULL, FALSE);
            }
            break;
        }

        case STARTUP_NORMAL:
        {
            // 播放充电提示音
            MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
            uint16          ringId = (uint16)MMIAPIENVSET_GetActiveModeOptValue( dualSys, CHARGE_NOTIFY );
            uint8           volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );

#ifdef ZDT_APP_SUPPORT // wuxx add for W217
                    ZDT_SetIsCharge(TRUE);
#endif

            if( ringId != 0 && volume != 0 ) // 0 mean silent
            {
                MMIAPISET_PlayRingByIdEx( FALSE, ringId, volume, 1, MMISET_RING_TYPE_CHARGE, PNULL );
            }

            //启动idle状态下电池充电动画!
            MAIN_SetIdleChargeState(TRUE);
        #ifndef ZDT_APP_SUPPORT
            WatchLAUNCHER_SetChargePageVisible(TRUE);//bug: 2138727
        #endif
            if (0 == s_charge_tip_delay_timer)
            {
                s_charge_tip_delay_timer = MMK_CreateTimerCallback(CHARGE_TIP_DELAY,  OpenChargeTipDelayCallback, NULL, FALSE);
            }
            break;
        }

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
#endif
}

/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_START_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeEndInd(BOOLEAN need_delay)
{
#if 0
#ifndef MMI_DUAL_BATTERY_SUPPORT
    uint8               bat_level = 0;
#endif
    MMI_WIN_ID_T        alert_win_id = PHONE_CHR_WARNING_WIN_ID;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
#endif

    switch (MMIAPIPHONE_GetStartUpCondition())
    {
    case STARTUP_CHARGE:
	    //停止充电，并设置电池强度
        MAIN_SetIdleChargeState(FALSE);
#ifdef MMI_DUAL_BATTERY_SUPPORT
        MAIN_SetIdleDbatLevel(dbat_info);
#else
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#endif
#ifdef ZDT_APP_SUPPORT
        {
            CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
	        ZDT_SetIsCharge(FALSE);
	        if(MMIPHONE_CHARGE_FULL_CAP == p_chgmng_info->bat_remain_cap)//NEWMS00201875:防止电量为100%，MMI仍然处于充电动画
	        {
	            ZDT_SetIsBatFull(TRUE);
	        }
	        else
	        {
	            ZDT_SetIsBatFull(FALSE);
	        }
	    }
#endif

       // destroy开机(charge)界面
       MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
        
#ifdef MMI_UDISK_MEM_ENABLE
        if (!MMIAPIUDISK_UdiskIsRun())
#endif      
        {
            MMIDEFAULT_TurnOffBackLight();
            POWER_PowerOff();       
        }   
        break;

    case STARTUP_NORMAL:
        
#ifdef ZDT_APP_SUPPORT
{
        CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
        ZDT_SetIsCharge(FALSE);
        if(MMIPHONE_CHARGE_FULL_CAP == p_chgmng_info->bat_remain_cap)//NEWMS00201875:防止电量为100%，MMI仍然处于充电动画
        {
            ZDT_SetIsBatFull(TRUE);
        }
        else
        {
            ZDT_SetIsBatFull(FALSE);
        }
}
#endif
        //停止充电，并设置电池强度
        MAIN_SetIdleChargeState(FALSE);
#ifdef MMI_DUAL_BATTERY_SUPPORT
        MAIN_SetIdleDbatLevel(dbat_info);
#else
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#endif
        if(!need_delay)
        {
            if(MMK_IsOpenWin(PHONE_CHR_WARNING_WIN_ID))
            {
                MMK_CloseWin(PHONE_CHR_WARNING_WIN_ID);//maryxiao cr 243401
            }
            if (!MMIAPIUDISK_UdiskIsRun())
            {        
			#ifdef MMI_PDA_SUPPORT
            	MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CHARGE_PLUGOUT,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
			#else
            	MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CHARGE_PLUGOUT,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
			#endif
            }

        }
#ifdef ZDT_APP_SUPPORT
        //MMK_CloseChargingWin();
#endif        
        break;
        
    case STARTUP_ALARM:
    default:
        //TEMP 不处理
        break;
    }
    
    return (result);
#else
    uint8 bat_level = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch (MMIAPIPHONE_GetStartUpCondition())
    {
        case STARTUP_CHARGE:
        {
	    //停止充电，并设置电池强度
            MAIN_SetIdleChargeState(FALSE);
#ifdef MMI_DUAL_BATTERY_SUPPORT
            MAIN_SetIdleDbatLevel(dbat_info);
#else
            bat_level = MMIAPIPHONE_GetBatCapacity();
            MAIN_SetIdleBatLevel(bat_level);
#endif
#ifdef ZDT_APP_SUPPORT
{
	    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
        ZDT_SetIsCharge(FALSE);
        if(MMIPHONE_CHARGE_FULL_CAP <= p_chgmng_info->bat_remain_cap)//NEWMS00201875:防止电量为100%，MMI仍然处于充电动画
        {
            ZDT_SetIsBatFull(TRUE);
        }
        else
        {
            ZDT_SetIsBatFull(FALSE);
        }
}
#endif
           // destroy开机(charge)界面
           MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);

#ifdef MMI_UDISK_MEM_ENABLE
            if (!MMIAPIUDISK_UdiskIsRun())
#endif
            {
                MMIDEFAULT_TurnOffBackLight();
                POWER_PowerOff();
            }
            break;
        }

        case STARTUP_NORMAL:
        {
#ifdef ZDT_APP_SUPPORT
{
        CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
        ZDT_SetIsCharge(FALSE);
        if(MMIPHONE_CHARGE_FULL_CAP <= p_chgmng_info->bat_remain_cap)//NEWMS00201875:防止电量为100%，MMI仍然处于充电动画
        {
            ZDT_SetIsBatFull(TRUE);
        }
        else
        {
            ZDT_SetIsBatFull(FALSE);
        }
}
#endif
            //停止充电，并设置电池强度
            MAIN_SetIdleChargeState(FALSE);
#ifdef MMI_DUAL_BATTERY_SUPPORT
            MAIN_SetIdleDbatLevel(dbat_info);
#else
            bat_level = MMIAPIPHONE_GetBatCapacity();
            MAIN_SetIdleBatLevel(bat_level);
#endif
        #ifdef ZDT_APP_SUPPORT
            MMK_CloseChargingWin();
        #else
            WatchLAUNCHER_SetChargePageVisible(FALSE);
            WatchLAUNCHER_ChargeTip_Exist();
        #endif
            if (0 != s_charge_tip_delay_timer)
            {
                MMK_StopTimer(s_charge_tip_delay_timer);
                s_charge_tip_delay_timer = 0;
            }
            break;
        }

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
#endif
}


/*****************************************************************************/
//  Description : deal with signal of CHR_WARNING_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChrWarningInd(void)
{
#ifndef MMI_DUAL_BATTERY_SUPPORT
    uint8               bat_level = 0;
#endif
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        alert_win_id = PHONE_CHR_WARNING_WIN_ID;
    MMI_WIN_PRIORITY_E  win_priority = WIN_THREE_LEVEL;
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
#endif

#ifdef SCREEN_SHAPE_CIRCULAR
    WatchLAUNCHER_BatteryLow_Enter();
    return (result);
#endif
    switch (MMIAPIPHONE_GetStartUpCondition())
    {
    case STARTUP_NORMAL:
#ifdef MMI_DUAL_BATTERY_SUPPORT
        MAIN_SetIdleDbatLevel(dbat_info);
#else
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#endif
        if (MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID)
            ||MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
            ||(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
               && MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)))
        {
            break;
        }
       
        if (MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID)
            ||MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID))
        {
            break;
        }
        
#ifndef ZDT_APP_SUPPORT
        MMIDEFAULT_TurnOnBackLight();
#endif        
        // 打开电池电量低界面!
        if(MMK_IsOpenWin(PHONE_CHR_WARNING_WIN_ID))
        {
            MMK_CloseWin(PHONE_CHR_WARNING_WIN_ID);//maryxiao cr 243401
        }        
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CHARGE_WARNING,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,HandleLowPowerRingWin);

        break;
        
    case STARTUP_CHARGE:
        // 不应该收到该消息
        //SCI_TRACE_LOW:"HandleChrWarningInd:s_startup_condition = STARTUP_CHARGE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_765_112_2_18_2_45_20_196,(uint8*)"");
        break;
        
    case STARTUP_ALARM:
        //TEMP 不处理
        break;
        
    default:
        break;
    }
    
    return (result);
}


/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_FINISH
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeFinsh(void)
{
    uint8   bat_level = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

#if 1
#ifdef ZDT_APP_SUPPORT
        ZDT_SetIsBatFull(TRUE);
#endif
    return result;
#endif

    switch (MMIAPIPHONE_GetStartUpCondition())
    {
    case STARTUP_CHARGE:
        if(MMK_IsOpenWin(MMIAPIALM_GetAlarmPromptWinId()))
        {
            MMK_CloseWin(MMIAPIALM_GetAlarmPromptWinId());
        }
        
        if (!MMK_IsOpenWin(PHONE_STARTUP_CHARGE_OK_WIN_ID))
        {
            MMK_CreateWin((uint32*)PHONE_STARTUP_CHARGE_OK_WIN_TAB,PNULL);
        }
        if (MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))
        {
            MMK_CloseWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
        }
        MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
        
        //设置main中的全局变量
        MAIN_SetIdleChargeState(FALSE);
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏显示充电完成
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_CHARGE_FINISH,PNULL);
#endif
        break;
        
    case STARTUP_NORMAL:
        //停止充电，并设置电池强度
        MAIN_SetIdleChargeState(FALSE);
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
        break;
        
    case STARTUP_ALARM:
    default:
        //TEMP 不处理
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : deal with signal of CHR_MSG_CHARGE_FAULT_IND
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeFaultInd(void)
{
#ifndef MMI_DUAL_BATTERY_SUPPORT
    uint8   bat_level = 0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    PHONE_STARTUP_CONDITION_E startup_condition = STARTUP_NONE;
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
#endif

    startup_condition = (PHONE_STARTUP_CONDITION_E)MMIAPIPHONE_GetStartUpCondition();
    
    switch (startup_condition)
    {
    case STARTUP_CHARGE:
        //提示超过额定电压
        MMIPUB_OpenAlertFailWin(TXT_CHARGE_OVER_VOLTAGE);
        
        if (!MMK_IsOpenWin(PHONE_STARTUP_CHARGE_OK_WIN_ID))
        {
            MMK_CreateWin((uint32*)PHONE_STARTUP_CHARGE_OK_WIN_TAB,PNULL);
        }
        
        // destroy开机(charge)界面
        MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
        
        //设置main中的全局变量
        
#ifdef MMI_DUAL_BATTERY_SUPPORT
        MAIN_SetIdleChargeState(FALSE);
        MAIN_SetIdleDbatLevel(dbat_info);
#else
        MAIN_SetIdleChargeState(FALSE);
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#endif
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏显示充电电压过大
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_CHARGE_OVER_VOLTAGE,PNULL);
#endif
        break;
        
    case STARTUP_NORMAL:
        //提示超过额定电压
        MMIPUB_OpenAlertFailWin(TXT_CHARGE_OVER_VOLTAGE);
        
        //停止充电，并设置电池强度
        MAIN_SetIdleChargeState(FALSE);
#ifdef MMI_DUAL_BATTERY_SUPPORT
        MAIN_SetIdleDbatLevel(dbat_info);
#else
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#endif
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏显示充电电压过大
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_CHARGE_OVER_VOLTAGE,PNULL);
#endif
        break;
        
    case STARTUP_ALARM:
        break;

    default:
        //TEMP 不处理
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleChangeOvertempInd
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChangeOvertempInd(void)
{
#ifndef MMI_DUAL_BATTERY_SUPPORT
    uint8   bat_level = 0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    PHONE_STARTUP_CONDITION_E startup_condition = STARTUP_NONE;
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info = DBAT_GetState();
#endif

    startup_condition = (PHONE_STARTUP_CONDITION_E)MMIAPIPHONE_GetStartUpCondition();

    switch (startup_condition)
    {
    case STARTUP_CHARGE:
        //提示超过额定电压
        MMIPUB_OpenAlertFailWin(TXT_CHARGE_OVER_TEMP);

        if (!MMK_IsOpenWin(PHONE_STARTUP_CHARGE_OK_WIN_ID))
        {
            MMK_CreateWin((uint32*)PHONE_STARTUP_CHARGE_OK_WIN_TAB,PNULL);
        }

        // destroy开机(charge)界面
        MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);

        //设置main中的全局变量
#ifdef MMI_DUAL_BATTERY_SUPPORT
        MAIN_SetIdleChargeState(FALSE);
        MAIN_SetIdleDbatLevel(dbat_info);
#else
        MAIN_SetIdleChargeState(FALSE);
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#endif
        break;

    case STARTUP_NORMAL:
        //提示超过额定电压
        MMIPUB_OpenAlertFailWin(TXT_CHARGE_OVER_TEMP);

        //停止充电，并设置电池强度
        MAIN_SetIdleChargeState(FALSE);
#ifdef MMI_DUAL_BATTERY_SUPPORT
        MAIN_SetIdleDbatLevel(dbat_info);
#else
        bat_level = MMIAPIPHONE_GetBatCapacity();
        MAIN_SetIdleBatLevel(bat_level);
#endif
        break;

    case STARTUP_ALARM:
        break;

    default:
        //TEMP 不处理
        break;
    }

    return (result);
}

#ifdef MMI_DUAL_BATTERY_SUPPORT
/*****************************************************************************/
//  Description : deal with signal of DBAT_MSG_SWITCH_BAT_IND
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatSwitchInd(void)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch (MMIAPIPHONE_GetStartUpCondition())
    {
    case STARTUP_NORMAL:
        {
            if(MMIAPIIDLE_IsChangeChargeState())
            {
                MAIN_SetIdleChargeState(TRUE);
            }
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatCapInd(void)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch (MMIAPIPHONE_GetStartUpCondition())
    {
    case STARTUP_NORMAL:
        {
            DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
            MAIN_SetIdleDbatLevel(dbat_info);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : deal with signal of CHR_CHARGE_FINISH
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatFinsh(void)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
    
    switch (MMIAPIPHONE_GetStartUpCondition())
    {
    case STARTUP_CHARGE:
        if(MMK_IsOpenWin(MMIAPIALM_GetAlarmPromptWinId()))
        {
            MMK_CloseWin(MMIAPIALM_GetAlarmPromptWinId());
        }
        
        if (!MMK_IsOpenWin(PHONE_STARTUP_CHARGE_OK_WIN_ID))
        {
            MMK_CreateWin((uint32*)PHONE_STARTUP_CHARGE_OK_WIN_TAB,PNULL);
        }
        
        MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
        
        //设置main中的全局变量
        MAIN_SetIdleChargeState(FALSE);
        MAIN_SetIdleDbatLevel(dbat_info);
        //刷新小屏显示充电完成
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_CHARGE_FINISH,PNULL);
        break;
        
    case STARTUP_NORMAL:
        //停止充电，并设置电池强度
        MAIN_SetIdleChargeState(FALSE);
        MAIN_SetIdleDbatLevel(dbat_info);
        break;
        
    case STARTUP_ALARM:
    default:
        //TEMP 不处理
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatPlugInInd(DPARAM param)
{
    MmiOtherTaskMsgS otherTaskMsg={0};
    DBAT_BAT_E       cur_bat=DBAT_MAX_BAT;
    uint32 bat=0;
    MMI_WIN_ID_T    alert_win_id = PHONE_CHR_WARNING_WIN_ID;
    if(PNULL!=param)
    {
        otherTaskMsg=*((MmiOtherTaskMsgS*)param);
        bat = (uint32)otherTaskMsg.param_ptr;
        if(bat<DBAT_MAX_BAT)
        {
           cur_bat= bat;
        }
        else
        {
            return MMI_RESULT_FALSE;
        }
    }
    else
    {
        return MMI_RESULT_FALSE;
    }

    if(STARTUP_NORMAL==MMIAPIPHONE_GetStartUpCondition())
    {
        DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
        MAIN_SetIdleDbatLevel(dbat_info);
    }
    else if(STARTUP_CHARGE==MMIAPIPHONE_GetStartUpCondition())
    {
        //dbat do something
    }
    
    if(DBAT_MAIN_BAT==cur_bat)
    {
        if(MMK_IsOpenWin(PHONE_CHR_WARNING_WIN_ID))
        {
            MMK_CloseWin(PHONE_CHR_WARNING_WIN_ID);
        } 
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DUAL_BATTERY_PLUGIN_MAIN,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else if(DBAT_AUX_BAT==cur_bat)
    {
        if(MMK_IsOpenWin(PHONE_CHR_WARNING_WIN_ID))
        {
            MMK_CloseWin(PHONE_CHR_WARNING_WIN_ID);
        } 
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DUAL_BATTERY_PLUGIN_AUX,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : deal with signal of CHR_CAP_IND
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDbatPlugOutInd(DPARAM param)
{
    MmiOtherTaskMsgS otherTaskMsg={0};
    DBAT_BAT_E       cur_bat=DBAT_MAX_BAT;
    uint32 bat=0;
    MMI_WIN_ID_T    alert_win_id = PHONE_CHR_WARNING_WIN_ID;
    if(PNULL!=param)
    {
        otherTaskMsg=*((MmiOtherTaskMsgS*)param);
        bat = (uint32)otherTaskMsg.param_ptr;
        if(bat<DBAT_MAX_BAT)
        {
           cur_bat= bat;
        }
        else
        {
            return MMI_RESULT_FALSE;
        }
    }
    else
    {
        return MMI_RESULT_FALSE;
    }

    if(STARTUP_NORMAL==MMIAPIPHONE_GetStartUpCondition())
    {
        DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
        MAIN_SetIdleDbatLevel(dbat_info);
    }
    else if(STARTUP_CHARGE==MMIAPIPHONE_GetStartUpCondition())
    {
        //dbat do something
    }
    
    if(DBAT_MAIN_BAT==cur_bat)
    {
        if(MMK_IsOpenWin(PHONE_CHR_WARNING_WIN_ID))
        {
            MMK_CloseWin(PHONE_CHR_WARNING_WIN_ID);
        } 
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DUAL_BATTERY_PLUGOUT_MAIN,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else if(DBAT_AUX_BAT==cur_bat)
    {
        if(MMK_IsOpenWin(PHONE_CHR_WARNING_WIN_ID))
        {
            MMK_CloseWin(PHONE_CHR_WARNING_WIN_ID);
        } 
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DUAL_BATTERY_PLUGOUT_AUX,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : get capacity level(0-5)
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetBatLevel(uint32 level)
{
    uint8  bat_level = 0;
    
    //获得电量强度
    bat_level = (uint8)(level / PHONE_RX_BAT_LEVEL_SPAN); 
    
    if (PHONE_RX_BAT_DEFAULT_LEVEL < bat_level)
    {
        bat_level = PHONE_RX_BAT_DEFAULT_LEVEL;
    }
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetBatLevel:bat_level=%d,level=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_1130_112_2_18_2_45_21_197,(uint8*)"dd",bat_level,level);
    return (bat_level);
}
#endif

/*****************************************************************************/
//  Description : get capacity level(0-5)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetBatCapacity(void)
{
    uint32  bat_level ;
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    uint32  bat_level_last = 0;
    
    bat_level = p_chgmng_info->bat_remain_cap;
    //获得电量强度
    bat_level_last = (bat_level % PHONE_RX_BAT_LEVEL_SPAN);
    if(bat_level_last >= PHONE_RX_BAT_LEVEL_SPAN/2)//四舍五入
    {
        bat_level_last = 1;
    }
    else
    {
        bat_level_last = 0;
    }
    bat_level = (uint8)((bat_level / PHONE_RX_BAT_LEVEL_SPAN) + bat_level_last); 
    
    if (PHONE_RX_BAT_DEFAULT_LEVEL < bat_level)
    {
        bat_level = PHONE_RX_BAT_DEFAULT_LEVEL;
    }
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetBatCapacity bat_level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_1151_112_2_18_2_45_21_198,(uint8*)"d",bat_level);
    return ((uint8)bat_level);
}
#ifdef ZDT_APP_SUPPORT
PUBLIC uint8 MMIAPIPHONE_ZDT_GetBatCapacity(void)
{
    uint8 res = 0;
    uint32  bat_level ;
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    uint32  bat_level_last = 0;
    if(p_chgmng_info->bat_remain_cap <= 5)
    {
    	res = 1;
    }
    else if(p_chgmng_info->bat_remain_cap <= 10)
    {
        res = 2;
    }
    else if(p_chgmng_info->bat_remain_cap <= 20)
    {
        res = 3;
    }
    return res;
}
#endif

/*****************************************************************************/
//  Description : get the status of battery(if it is charging)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
BOOLEAN MMIPHONE_GetChargeStatus(void)
{
    BOOLEAN     result = TRUE;
#ifndef WIN32
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
    if(PNULL!=dbat_info->p_bat[DBAT_MAIN_BAT] || PNULL!=dbat_info->p_bat[DBAT_AUX_BAT])
    {
        result = TRUE;
    }
    else
    {
       result = FALSE; 
    }
#else
    result = TRUE;
#endif
#endif
    //SCI_TRACE_LOW:"MMIPHONE_GetChargeStatus,result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_1176_112_2_18_2_45_21_199,(uint8*)"d",result);
    return (result);
}


/*****************************************************************************/
//  Description :  Handle Low Power Ring Win msg.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLowPowerRingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"[MMIPHONE]: HandleLowPowerRingWin, msg=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_1193_112_2_18_2_45_21_200,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //发出警告音
        if (MMIAPISET_GetIsOpenVoltageWarning())
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1, FALSE,0,1,MMISET_RING_TYPE_BAT,PNULL);
        }
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPISET_StopRing(MMISET_RING_TYPE_BAT);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopAppRing();
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}


/*****************************************************************************/
//  Description : get event is delay or not
//  Global resource dependence : none
//  Author: 
//  Note: return TRUE: delay, FALSE:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetEventDelaied(MMI_DELAIED_EVENT_E event)
{
    uint32 i = 0;
    BOOLEAN result = FALSE;
    for(i = 0; i < MMI_DELAIED_EVENT_NUM; i++)
    {
        if(s_delaied_events[i] == event)
        {
            result = TRUE;
            break;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetEventDelaied(MMI_DELAIED_EVENT_E event)
{
    uint32 i = 0;

    if(!MMIAPIPHONE_GetEventDelaied(event))
    {
        for(i = 0; i < MMI_DELAIED_EVENT_NUM; i++)
        {
            if(s_delaied_events[i] == MMI_NONE_DELAY)
            {
                s_delaied_events[i] = event;
                break;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_CleanEventDelaied(MMI_DELAIED_EVENT_E event)
{
    uint32 i = 0;
    for(i = 0; i < MMI_DELAIED_EVENT_NUM; i++)
    {
        if(s_delaied_events[i] == event)
        {
            s_delaied_events[i] = MMI_NONE_DELAY;
            break;
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ResumeDelaiedEvent(MMI_DELAIED_EVENT_E event)
{
    switch(event)
    {
    case MMI_USB_DELAY:
        //resume usb plug event
        MMIAPIUdisk_OpenUsbOperWindow();
        break;
    case MMI_CHARGE_PLUG_IN_DELAY:
        //resume charge plug in event
        MMIDEFAULT_TurnOnBackLight();        
        // 该消息用于汇报设备处于充电状态。界面上的电池显示为动画方式。
        HandleChargeStartInd();
        break;
    case MMI_CHARGE_PLUG_OUT_DELAY:
        //resume charge plug out event
        MMIDEFAULT_TurnOnBackLight();
        // 该消息用于汇报此时充电被断开。
        HandleChargeEndInd(FALSE);
        break;
    case MMI_CHARGE_END_DELAY:
        //resume charge end event
        MMIDEFAULT_TurnOnBackLight();
        // 该消息用于汇报此时充电被断开。
        HandleChargeEndInd(FALSE);
        break;
    case MMI_SET_DATA_DELAY:
        MMIAPISET_SetDate(); 
        break;
#if defined IM_ENGINE_SOGOU //sogou输入法初始化比较慢,耗时约3.7s
        case MMI_SOGOU_INIT_DELAY:
        MMIAPIIM_SGInit ();
        MMIAPIPHONE_CleanEventDelaied (MMI_SOGOU_INIT_DELAY);
        break;            
#endif
       
    default:
        break;
    }
    MMIAPIPHONE_CleanEventDelaied(event);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ResumeAllDelaiedEvent(void)
{
    uint32 i = 0;
    for(i = 0; i < MMI_DELAIED_EVENT_NUM; i++)
    {
        MMIAPIPHONE_ResumeDelaiedEvent(s_delaied_events[i]);
    }
}

/*****************************************************************************/
//  Description : check event must delay
//  Global resource dependence : none
//  Author: 
//  Note: return TRUE: delay, FALSE:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsNeedDelayEvent(MMI_DELAIED_EVENT_E event)
{
        BOOLEAN bret = FALSE;
    
    if (MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID)
        ||MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
        ||MMIAPIDM_GetHandsetLock() 
        ||MMIAPITP_Coordinate_IsWinOpen()
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
        )
    {
        bret = TRUE;
    }
    
    return bret;
}


/*****************************************************************************/
//  Description : 处理充电启动开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: PHOEBE CHEN
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeStartupWindow (
                                              MMI_WIN_ID_T       win_id,    // STARTUP CHARGE窗口的ID
                                              MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                              DPARAM                param       // 相应消息的参数
                                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    //static uint8        s_current_charge_step = 0;
    //GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUIANIM_INIT_DATA_T                 anim_init = {0};
    MMI_CONTROL_CREATE_T                anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    uint16 screen_w=0;
    uint16 screen_h=0;
    GUI_RECT_T rect={0};
    GUI_COLOR_T bg_color=0x0861;
    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T anim_rect = MMITHEME_GetFullScreenBothRect();
            uint16 timeimgheight=0;     
#ifndef MMI_GUI_STYLE_MINISCREEN
            GUIRES_GetImgWidthHeight(PNULL,&timeimgheight,IMAGE_CHARGE_TIME_BG,PHONE_STARTUP_CHARGE_WIN_ID);
            anim_rect.v_rect.top = anim_rect.v_rect.top + MMI_STATUSBAR_HEIGHT+timeimgheight;
#else
            GUIRES_GetImgWidthHeight(PNULL,&timeimgheight,IMAGE_IDLE_KEYLOCK_TIME_BG,PHONE_STARTUP_CHARGE_WIN_ID);
            anim_rect.v_rect.top = anim_rect.v_rect.top + timeimgheight;
#ifdef MAINLCD_DEV_SIZE_160X128
			anim_rect.h_rect.top =  anim_rect.h_rect.top + timeimgheight;
#endif
#endif
#ifdef MAINLCD_DEV_SIZE_320X240
            anim_rect.h_rect.top =  anim_rect.h_rect.top + timeimgheight;
#endif
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMI_CHARGE_ANIM_CTRL_ID;
            data_info.img_id = IMAGE_CHARGING;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            anim_init.both_rect = anim_rect;

            //creat anim
            anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
            anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
            anim_ctrl.init_data_ptr     = &anim_init;
            anim_ctrl.parent_win_handle = win_id;
            MMK_CreateControl(&anim_ctrl);
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
#ifdef ZDT_APP_SUPPORT
            ZDT_SetIsCharge(TRUE);
#endif
        }
        break;
    case MSG_FULL_PAINT:
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&screen_w,&screen_h);
        rect.right=screen_w-1;
        rect.bottom=screen_h-1;
#ifdef CMCC_UI_STYLE
        bg_color = MMI_BLACK_COLOR;
#endif

        LCD_FillRect(&lcd_dev_info,rect,bg_color);

        DisplayDayTimeEffert(win_id);

        break;
    case MSG_CTL_ANIM_DISPLAY_IND:
    case MSG_CTL_ANIM_GIF_UPDATE:     
                if (MMK_IsFocusWin(win_id))
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
        break;

    case MSG_CLOSE_WINDOW:
        break;

    case MSG_KEYLONG_RED:
#ifndef MMI_PDA_SUPPORT
        MMK_CloseWin(win_id);
        
        // 开机
        MMIAPIPHONE_PowerReset();
#endif
        break;
        
    case MSG_APP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
#ifndef SUBLCD_SIZE_NONE
    case MSG_GET_FOCUS:
        {
            IDLE_WIN_INFO_T		*idle_info_ptr = MMIIDLE_GetIdleWinInfo();
            MMISUB_UpdateDisplaySubLcd();
            if(idle_info_ptr->is_charge)
            {
                MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_CHARGE_OFF,PNULL);
            }
        }
        break;
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
  

                                            
/*****************************************************************************/
//  Description : 处理充电启动开机界面电量已满的消息(除PS之外)
//  Global resource dependence : 
//  Author: PHOEBE CHEN
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChargeStartupOKWindow (
                                                MMI_WIN_ID_T     win_id,    // STARTUP CHARGE窗口的ID
                                                MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                                DPARAM              param       // 相应消息的参数
                                                )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    //GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    //uint32 i = 0;
    GUIANIM_INIT_DATA_T                 anim_init = {0};
    MMI_CONTROL_CREATE_T                anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    uint16 screen_w=0;
    uint16 screen_h=0;
    GUI_RECT_T rect={0};
    GUI_COLOR_T bg_color = 0x0861;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        #if 0
        dis_point.x = 0;
        dis_point.y = 0;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);

        dis_point.x = 0;
        dis_point.y = 0;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_COMMON_TITLE_BAR,
                &lcd_dev_info);

        dis_point.x = 0;
        dis_point.y = MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_SOFTKEY_BG,
                &lcd_dev_info);
        
        dis_point.x = MMIPHONE_CHARGE_LEVEL_BOX_LEFT;
        dis_point.y = MMIPHONE_CHARGE_LEVEL_BOX_TOP;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_ADJUST_VOL_BLOCK_BG,
                &lcd_dev_info);
        
        dis_point.x = MMIPHONE_CHARGE_BG_LEFT;
        dis_point.y = MMIPHONE_CHARGE_BG_TOP;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_CHARGE_FINISH,
                &lcd_dev_info);
        
        for (i = 0; i < MMIPHONE_CHARGE_MAX_STEPS; i ++)
        {
            dis_point.x = MMIPHONE_CHARGE_LEVEL_BOX_LEFT + (MMIPHONE_CHARGE_MAX_STEPS - 1 - i) * MMIPHONE_CHARGE_LEVEL_BOX_DISTANCE;
            dis_point.y = MMIPHONE_CHARGE_LEVEL_BOX_TOP;
            GUIRES_DisplayImg(&dis_point,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_ADJUST_VOL_BLOCK_FG,
                    &lcd_dev_info);
        }
        #endif
        {
            GUI_BOTH_RECT_T anim_rect = MMITHEME_GetFullScreenBothRect();
            uint16 timeimgheight=0;     
#ifndef MMI_GUI_STYLE_MINISCREEN
            GUIRES_GetImgWidthHeight(PNULL,&timeimgheight,IMAGE_CHARGE_TIME_BG,win_id);
            anim_rect.v_rect.top = anim_rect.v_rect.top + MMI_STATUSBAR_HEIGHT+timeimgheight;
#else
            GUIRES_GetImgWidthHeight(PNULL,&timeimgheight,IMAGE_IDLE_KEYLOCK_TIME_BG,win_id);
            anim_rect.v_rect.top = anim_rect.v_rect.top + timeimgheight;
#endif

            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMI_CHARGE_ANIM_CTRL_ID;
            data_info.img_id = IMAGE_CHARGE_FINISH;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            anim_init.both_rect = anim_rect;

            //creat anim
            anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
            anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
            anim_ctrl.init_data_ptr     = &anim_init;
            anim_ctrl.parent_win_handle = win_id;
            MMK_CreateControl(&anim_ctrl);
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);

            //creat update-timer
            if(s_charging_updatetime_timer!=0)
            {
                MMK_StopTimer(s_charging_updatetime_timer);
                s_charging_updatetime_timer=0;
            }
            s_charging_updatetime_timer=MMK_CreateTimer(UPDATETIME_TIMER_10SEC, TRUE);
           
        }
        break;
    case MSG_FULL_PAINT:
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&screen_w,&screen_h);
        rect.right=screen_w-1;
        rect.bottom=screen_h-1;
#ifdef CMCC_UI_STYLE
        bg_color = MMI_BLACK_COLOR;
#endif
       
        LCD_FillRect(&lcd_dev_info,rect,bg_color);

        DisplayDayTimeEffert(win_id);

        break;

    case MSG_TIMER:
        if (s_charging_updatetime_timer == *((uint8 *) param))
        {
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
        break;
    case MSG_GET_FOCUS:
            if (s_charging_updatetime_timer != 0)
            {
                MMK_StopTimer(s_charging_updatetime_timer);
                s_charging_updatetime_timer = 0;
            }
            s_charging_updatetime_timer = MMK_CreateTimer(UPDATETIME_TIMER_10SEC, TRUE);
        break;
    case MSG_LOSE_FOCUS:
        if (s_charging_updatetime_timer != 0)
        {
            MMK_StopTimer(s_charging_updatetime_timer);
            s_charging_updatetime_timer = 0;
        }
        break;
    case MSG_CLOSE_WINDOW:
        if (s_charging_updatetime_timer != 0)
        {
            MMK_StopTimer(s_charging_updatetime_timer);
            s_charging_updatetime_timer = 0;
        }
        break;
        
    case MSG_KEYLONG_RED:
        MMK_CloseWin(win_id);

        MMIAPISET_StopAppRing();
        
        // 开机
        MMIAPIPHONE_PowerReset();
        break;
        
    case MSG_APP_RED:
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
//  Description : 处理充电窗口
//  Global resource dependence : 
//  Author: PHOEBE CHEN
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtryCapacityWin(
                                         MMI_WIN_ID_T     win_id,   // STARTUP CHARGE窗口的ID
                                         MMI_MESSAGE_ID_E    msg_id,    // 窗口的内部消息ID
                                         DPARAM                 param       // 相应消息的参数
                                         )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    //GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;  
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
        SetBtryCapacityFormParam();
#ifdef MMI_GUI_STYLE_MINISCREEN
        GUIFORM_PermitChildBorder(MMIPHONE_DISPLAY_BTRYCAP_FORM_ID,FALSE);
        GUILABEL_SetFont(MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,MMIPHONE_BERYCAP_FONT_SIZE,MMIPHONE_BERYCAP_FONT_COLOR);
        MMK_SetAtvCtrl(win_id, MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID);
#else
        MMK_SetAtvCtrl(win_id, MMIPHONE_DISPLAY_BTRYCAP_FORM_ID);
        
#endif
        break;
        
    case MSG_FULL_PAINT:
        #if 0
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        //DisplayBtryCapacity();
        //GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
        #endif
        DisplayBtryCapacityStr();
        break;
        
    case MSG_GET_FOCUS:
        if (MMIPHONE_GetChargeStatus())
        {
            if (s_is_charging_timer != 0)
            {
                MMK_StopTimer(s_is_charging_timer);
                s_is_charging_timer = 0;
            }
            s_is_charging_timer = MMK_CreateTimer(MMIPHONE_CHARGE_INTERVAL, FALSE);
        }
        break;
        
    case MSG_LOSE_FOCUS:
        if (s_is_charging_timer != 0)
        {
            MMK_StopTimer(s_is_charging_timer);
            s_is_charging_timer = 0;
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if (s_is_charging_timer != 0)
        {
            MMK_StopTimer(s_is_charging_timer);
            s_is_charging_timer = 0;
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_TIMER:
        if (s_is_charging_timer == *((uint8 *) param))
        {
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open bettry capability win
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenBtryCapWin(void)
{
    MMK_CreateWin((uint32*)PHONE_BTRY_CAP_WIN_TAB, NULL);
}


/*****************************************************************************/
//  Description : start charge timer
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_StartChargeTimer(void)
{
    MMIAPIPHONE_StopChargeTimer();
    s_charge_anim_timer_id = MMK_CreateTimer(MMIPHONE_CHARGE_TIME, FALSE);
}

/*****************************************************************************/
//  Description : stop 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_StopChargeTimer(void)
{
    if (0 !=s_charge_anim_timer_id)
    {
        MMK_StopTimer(s_charge_anim_timer_id);
        s_charge_anim_timer_id = 0;
    }
}

#ifdef MMI_DUAL_BATTERY_SUPPORT
/*****************************************************************************/
//  Description : SetBtryCapacityFormParam
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetBtryCapacityFormParam(void)
{
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};

    data_info.img_id = IMAGE_BATTERY_CAPACIT_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIPHONE_DISPLAY_BTRYCAP_IMAGE_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIPHONE_DISPLAY_BTRYCAP_IMAGE_ID,BtryCapacityCtrlCallbackFun);

    control_info.ctrl_id = MMIPHONE_DISPLAY_BTRYCAP_AUX_IMAGE_ID;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIPHONE_DISPLAY_BTRYCAP_AUX_IMAGE_ID,BtryCapacityCtrlCallbackFun);
    
    DisplayBtryCapacityStr();
        
    //GUIFORM_SetSpace(MMIPHONE_DISPLAY_BTRYCAP_FORM_ID,&form_space,&form_space);
    //GUIFORM_SetMargin(MMIPHONE_DISPLAY_BTRYCAP_FORM_ID,0);
}
#else
/*****************************************************************************/
//  Description : SetBtryCapacityFormParam
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetBtryCapacityFormParam(void)
{
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    uint16                      form_space =   0;

    data_info.img_id = IMAGE_BATTERY_CAPACIT_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIPHONE_DISPLAY_BTRYCAP_IMAGE_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIPHONE_DISPLAY_BTRYCAP_IMAGE_ID,BtryCapacityCtrlCallbackFun);
    
    //GUILABEL_SetTextById(MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,TXT_PHONE_CHARGE_OK,FALSE);
    DisplayBtryCapacityStr();
        
    GUIFORM_SetSpace(MMIPHONE_DISPLAY_BTRYCAP_FORM_ID,&form_space,&form_space);
 
    //GUIFORM_SetMargin(MMIPHONE_DISPLAY_BTRYCAP_FORM_ID,MMITHEME_GetFullScreenRect().bottom/10);
}
#endif


/*****************************************************************************/
//  Description : DisplayBtryCapacityStr
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void DisplayBtryCapacityStr(void)
{
#ifndef MMI_DUAL_BATTERY_SUPPORT
    uint8               bat_level   = 0;
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
#endif
    MMI_STRING_T        text_str    = {0};
    uint8               char_num_len    = 0;
    uint8               num_str[16] = {0};
    wchar               str_context[MMIPHONE_BTRYCAP_STR_LEN + 1] = {0};
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
    if(PNULL==dbat_info->p_bat[DBAT_MAIN_BAT])
    {
        GUIFORM_SetVisible(MMIPHONE_DISPLAY_BTRYCAP_MAIN_FORM_ID, FALSE, FALSE);
    }
    else
    {
        GUIFORM_SetVisible(MMIPHONE_DISPLAY_BTRYCAP_MAIN_FORM_ID, TRUE, FALSE);
        //bat_level=MMIAPIPHONE_GetBatLevel(dbat_info->p_bat[DBAT_MAIN_BAT]->remain_cap);
        //if (MMIPHONE_MAX_BAT_LEVEL_SPAN <= dbat_info->p_bat[DBAT_MAIN_BAT]->remain_cap)
        //{
        //    GUILABEL_SetTextById(MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,TXT_DUAL_BTRY_MAIN_CHARGE_OK,FALSE);
        //}
        //else
        { // 显示电量百分比
            MMI_GetLabelTextByLang(TXT_DUAL_BTRY_MAIN_CAP_LABEL_TEXT, &text_str); 
            if(DBAT_MAIN_BAT == dbat_info->cur_bat)
            {
                sprintf((char*)num_str, "%d%s", (int)dbat_info->chgmng_info.bat_remain_cap, "%");
            }
            else
            {
                sprintf((char*)num_str, "%d%s", (int)dbat_info->p_bat[DBAT_MAIN_BAT]->remain_cap, "%");
            }
            char_num_len = strlen((char*)num_str);

            if( text_str.wstr_len + char_num_len < MMIPHONE_BTRYCAP_STR_LEN )
            {
                MMI_WSTRNCPY( str_context, MMIPHONE_BTRYCAP_STR_LEN, text_str.wstr_ptr, text_str.wstr_len, text_str.wstr_len );
                MMI_STRNTOWSTR( str_context + text_str.wstr_len, MMIPHONE_BTRYCAP_STR_LEN - text_str.wstr_len, (uint8*)num_str, char_num_len, char_num_len );
                
                text_str.wstr_len = text_str.wstr_len + char_num_len;
                text_str.wstr_ptr = str_context;
                GUILABEL_SetText(MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,&text_str,FALSE);
            }
            else
            {
                //SCI_TRACE_LOW:"mmi DisplayBtryCapacityStr exceed str len max"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_1883_112_2_18_2_45_22_201,(uint8*)"");
            }
        }
    }
    
    if(PNULL==dbat_info->p_bat[DBAT_AUX_BAT])
    {
        GUIFORM_SetVisible(MMIPHONE_DISPLAY_BTRYCAP_AUX_FORM_ID, FALSE, FALSE);
    }
    else
    {
        GUIFORM_SetVisible(MMIPHONE_DISPLAY_BTRYCAP_AUX_FORM_ID, TRUE, FALSE);
        //bat_level=MMIAPIPHONE_GetBatLevel(dbat_info->p_bat[DBAT_AUX_BAT]->remain_cap);
        //if (MMIPHONE_MAX_BAT_LEVEL_SPAN <= dbat_info->p_bat[DBAT_AUX_BAT]->remain_cap)
        //{
        //    GUILABEL_SetTextById(MMIPHONE_DISPLAY_BTRYCAP_AUX_LABEL_ID,TXT_DUAL_BTRY_AUX_CHARGE_OK,FALSE);
        //}
        //else
        { // 显示电量百分比
            MMI_GetLabelTextByLang(TXT_DUAL_BTRY_AUX_CAP_LABEL_TEXT, &text_str); 
            if(DBAT_AUX_BAT == dbat_info->cur_bat)
            {
                sprintf((char*)num_str, "%d%s", (int)dbat_info->chgmng_info.bat_remain_cap, "%");
            }
            else
            {
                sprintf((char*)num_str, "%d%s", (int)dbat_info->p_bat[DBAT_AUX_BAT]->remain_cap, "%");
            }
            char_num_len = strlen((char*)num_str);

            if( text_str.wstr_len + char_num_len < MMIPHONE_BTRYCAP_STR_LEN )
            {
                MMI_WSTRNCPY( str_context, MMIPHONE_BTRYCAP_STR_LEN, text_str.wstr_ptr, text_str.wstr_len, text_str.wstr_len );
                MMI_STRNTOWSTR( str_context + text_str.wstr_len, MMIPHONE_BTRYCAP_STR_LEN - text_str.wstr_len, (uint8*)num_str, char_num_len, char_num_len );
                
                text_str.wstr_len = text_str.wstr_len + char_num_len;
                text_str.wstr_ptr = str_context;
                GUILABEL_SetText(MMIPHONE_DISPLAY_BTRYCAP_AUX_LABEL_ID,&text_str,FALSE);
            }
            else
            {
                //SCI_TRACE_LOW:"mmi DisplayBtryCapacityStr exceed str len max"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_1924_112_2_18_2_45_22_202,(uint8*)"");
            }
        }
    }

#else
    
    bat_level = p_chgmng_info->bat_remain_cap;
    //display battery text 
    if (MMIPHONE_MAX_BAT_LEVEL_SPAN <= bat_level)
    {
        GUILABEL_SetTextById(MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,TXT_PHONE_CHARGE_OK,FALSE);
    }
    else
    { // 显示电量百分比
        MMI_GetLabelTextByLang(TXT_BTRY_CAP_LABEL_TEXT, &text_str); 
        sprintf((char*)num_str, "%d%s", (int)bat_level, "%");
        char_num_len = strlen((char*)num_str);

        if( text_str.wstr_len + char_num_len < MMIPHONE_BTRYCAP_STR_LEN )
        {
            MMI_WSTRNCPY( str_context, MMIPHONE_BTRYCAP_STR_LEN, text_str.wstr_ptr, text_str.wstr_len, text_str.wstr_len );
            MMI_STRNTOWSTR( str_context + text_str.wstr_len, MMIPHONE_BTRYCAP_STR_LEN - text_str.wstr_len, (uint8*)num_str, char_num_len, char_num_len );
            
            text_str.wstr_len = text_str.wstr_len + char_num_len;
            text_str.wstr_ptr = str_context;
            GUILABEL_SetText(MMIPHONE_DISPLAY_BTRYCAP_LABEL_ID,&text_str,FALSE);
        }
        else
        {
            //SCI_TRACE_LOW:"mmi DisplayBtryCapacityStr exceed str len max"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_1954_112_2_18_2_45_22_203,(uint8*)"");
        }
    }
#endif
}

/*****************************************************************************/
//  Description : BtryCapacityCtrlCallbackFun
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void BtryCapacityCtrlCallbackFun ( GUIANIM_OWNER_DRAW_T    *draw_ptr)
{
    uint8               bat_level = 0;
    uint16              bat_width = 0;
    uint16              bat_height = 0;
    GUI_RECT_T          lcd_rect = {0};
    GUI_RECT_T          img_rect = {0};
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
#ifdef MMI_DUAL_BATTERY_SUPPORT
    MMI_HANDLE_T        handle1  =  0;
    MMI_HANDLE_T        handle2  =  0;
#endif
    
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
    
    handle1 = MMK_ConvertIdToHandle(MMIPHONE_DISPLAY_BTRYCAP_IMAGE_ID);
    handle2 = MMK_ConvertIdToHandle(MMIPHONE_DISPLAY_BTRYCAP_AUX_IMAGE_ID);

    if(handle1==draw_ptr->ctrl_handle && PNULL!=dbat_info->p_bat[DBAT_MAIN_BAT])
    {
        if(DBAT_MAIN_BAT == dbat_info->cur_bat)
        {
            bat_level = dbat_info->chgmng_info.bat_remain_cap;
        }
        else
        {
            bat_level = dbat_info->p_bat[DBAT_MAIN_BAT]->remain_cap;
        }
    }
    else if(handle2==draw_ptr->ctrl_handle && PNULL!=dbat_info->p_bat[DBAT_AUX_BAT])
    {
        if(DBAT_AUX_BAT == dbat_info->cur_bat)
        {
            bat_level = dbat_info->chgmng_info.bat_remain_cap;
        }
        else
        {
            bat_level = dbat_info->p_bat[DBAT_AUX_BAT]->remain_cap;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"BtryCapacityCtrlCallbackFun,ERROR!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_2007_112_2_18_2_45_23_204,(uint8*)"");
    }
#else
    bat_level = p_chgmng_info->bat_remain_cap;
#endif
    //SCI_TRACE_LOW:"BtryCapacityCtrlCallbackFun,bat_level=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_CHARGE_2012_112_2_18_2_45_23_205,(uint8*)"d",bat_level);
    //display battery image bar
    GUIRES_GetImgWidthHeight(&bat_width,&bat_height, IMAGE_BATTERY_CAPACIT_FG,draw_ptr->win_handle);

    //set lcd rect
    lcd_rect.left   = draw_ptr->display_x;
    lcd_rect.top    = draw_ptr->display_y;
    lcd_rect.right  = (int16)(draw_ptr->display_x + bat_level*bat_width/MMIPHONE_MAX_BAT_LEVEL_SPAN);
    lcd_rect.bottom = (int16)(draw_ptr->display_y + bat_height - 1);

    //get crossed rect is lcd rect
    if (GUI_IntersectRect(&lcd_rect,lcd_rect,draw_ptr->display_rect))
    {
        //set img rect
        img_rect.left   = (int16)(lcd_rect.left - draw_ptr->display_x);
        img_rect.top    = (int16)(lcd_rect.top - draw_ptr->display_y);
        img_rect.right  = (int16)(lcd_rect.right - draw_ptr->display_x);
        img_rect.bottom = (int16)(lcd_rect.bottom - draw_ptr->display_y);

        GUIRES_DisplayImg(PNULL,
            &lcd_rect,
            &img_rect,
            draw_ptr->win_handle,
            IMAGE_BATTERY_CAPACIT_FG,
            &draw_ptr->lcd_dev);
    }
}

/*****************************************************************************/
//  Description :  General  digital clock display.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
 LOCAL void ChargeDisplayDigitalClock(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, MMI_IMAGE_ID_T start_img_id,MMI_WIN_ID_T  win_id)
{
    GUI_POINT_T                 point = {0};
    SCI_TIME_T                  time = {0};
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
    BOOLEAN                     b_am = TRUE;
    uint16                      image_width = 0,image_height = 0;
    //uint16                      am_width = 0,  am_height = 0;
    uint16                      time_total_width = 0;
    uint16                   am_width = 0;
    uint16                   am_height = 0;
    
    TM_GetSysTime(&time);

    //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        if ((12 > time.hour))//AM
        {
            if ((1 > time.hour))//00:00-1:00
            {
                time.hour = time.hour + 12;
            }
        }
        else//PM
        {
            if ((12 <= time.hour) && (13 > time.hour))//12:00-13:00
            {
            }
            else
            {
                time.hour = time.hour-12;
            }
            b_am = FALSE;
        }   
    }

    //计算时间总的宽度，为了居中显示
    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id + 10,win_id);
    time_total_width = time_total_width  + image_width;//colon

#if defined (MMI_GUI_STYLE_MINISCREEN)
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GUIRES_GetImgWidthHeight(&am_width,&am_height, IMAGE_IDLE_TIME_AM,win_id);
        time_total_width = time_total_width  + am_width;//am.pm
    }
#else
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GUIRES_GetImgWidthHeight(&am_width,&am_height, IMAGE_IDLE_KEYLOCK_NUM_AM,win_id);
        time_total_width = time_total_width  + am_width;//am.pm
    }
#endif

    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id,win_id);
    time_total_width = time_total_width + image_width * 4 ;//有四个数字

    point.y = rect.top + (rect.bottom - rect.top - image_height) / 2;
    if(rect.right - rect.left >= time_total_width)
    {
        point.x = rect.left + (rect.right - rect.left - time_total_width)/2;
    }
    else
    {
        point.x = rect.left;
    }

    //HOUR
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,start_img_id + time.hour/10, dev);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,start_img_id + time.hour%10, dev);

    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,start_img_id + 10, dev);

    //COLON IMAGE WIDTH
    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id + 10, win_id);
    //MIN
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,start_img_id + time.min/10, dev);

    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id,win_id);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,start_img_id + time.min%10, dev);
    //AM.PM
#if defined (MMI_GUI_STYLE_MINISCREEN)
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        point.x = point.x + image_width;
        point.y = point.y + image_height - am_height;
        if(b_am )
            GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_IDLE_TIME_AM, dev);
        else
            GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_IDLE_TIME_PM, dev);
    }
#else
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        point.x = point.x + image_width;
        point.y = point.y + image_height - am_height;
        if(b_am )
            GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_IDLE_KEYLOCK_NUM_AM, dev);
        else
            GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_IDLE_KEYLOCK_NUM_PM, dev);
    }
#endif    
}

/*****************************************************************************/
//  Description :  Display date and day
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
 LOCAL void ChargeDisplayDateAndDay(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, GUI_FONT_T font, GUI_ALIGN_E align, GUI_COLOR_T color, GUIFONT_EFFECT_T effect )
{
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T string = {0};
    uint8 datestr[32] = {0};
    wchar datewstr[16] = {0};

    SCI_DATE_T date = {0};
    MMI_TEXT_ID_T week_table[] = {
                                        TXT_SHORT_IDLE_SUNDAY,
                                        TXT_SHORT_IDLE_MONDAY,
                                        TXT_SHORT_IDLE_TUESDAY,
                                        TXT_SHORT_IDLE_WEDNESDAY,
                                        TXT_SHORT_IDLE_THURSDAY,
                                        TXT_SHORT_IDLE_FRIDAY,
                                        TXT_SHORT_IDLE_SATURDAY
                                    };

    MMIAPISET_GetCurrentDateStr(datestr,'-',16);
    MMI_STRNTOWSTR( datewstr, 16, datestr, 16, 16 );    

    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &string);

    MMIAPICOM_Wstrcat(datewstr, L"  ");
    MMIAPICOM_Wstrncat(datewstr, string.wstr_ptr, string.wstr_len);

    string.wstr_ptr = (wchar *)datewstr;
    string.wstr_len = MMIAPICOM_Wstrlen(datewstr);
    
    text_style.font_color = color;
    text_style.font = font;
    text_style.align = align;
    text_style.effect = effect;

    GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)dev,
                    (const GUI_RECT_T *)&rect,
                    (const GUI_RECT_T *)&rect,
                    (const MMI_STRING_T *)&string,
                    &text_style,
                    GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT,
                    GUISTR_TEXT_DIR_AUTO
                    );
}

LOCAL void DisplayDayTimeEffert(MMI_WIN_ID_T  win_id)//andy.he_charge
{
    GUI_LCD_DEV_INFO layer = {0, 0};
    GUI_RECT_T rect = {0};
    GUI_POINT_T pos = {0, 0};
    uint16 img_width = 0, img_height = 0;
    uint16 lcd_width = 0, lcd_height = 0;
   
    /* time bg */
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CHARGE_TIME_BG, win_id);
#else
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_TIME_BG, win_id);
#endif
    pos.x = MAX(0,(lcd_width - img_width)/2);
    pos.y = MMITHEME_GetStatusBarHeight();
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_CHARGE_TIME_BG, &layer);
#else
    GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_IDLE_KEYLOCK_TIME_BG, &layer);
#endif
    /* time */
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CHARGE_DIGIT0, win_id);
#else
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_TIME_0, win_id);
#endif
    rect.top = MMITHEME_GetStatusBarHeight() + Charge_TIME_UP_MARGIN;
    rect.bottom = rect.top + img_height;
    rect.right = lcd_width - 1;
#ifndef MMI_GUI_STYLE_MINISCREEN
    ChargeDisplayDigitalClock(&layer, rect, IMAGE_CHARGE_DIGIT0,win_id);
#else
ChargeDisplayDigitalClock(&layer, rect, IMAGE_IDLE_TIME_0,win_id);
#endif
    /* day and date */
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CHARGE_TIME_BG, win_id);
#else
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_TIME_BG, win_id);
#endif
    rect.top = rect.bottom + 2;
    rect.bottom = MMITHEME_GetStatusBarHeight() + img_height;
    rect.right = lcd_width - 1;
    ChargeDisplayDateAndDay(&layer, rect, MMI_DEFAULT_SMALL_FONT, ALIGN_HVMIDDLE, MMI_DEFAULT_TEXT_COLOR, FONT_EFFECT_NONE);
    
}

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
//  Description : HandleOpenPowerSavQueryWin
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 开启了省电模式时，电量为设定值会打开该询问窗口，询问是否立即开启省电模式
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenPowerSavQueryWin(
                                                MMI_WIN_ID_T       win_id,    
                                                MMI_MESSAGE_ID_E      msg_id,    
                                                DPARAM                param      
                                             )
{
    MMI_RESULT_E result =  MMI_RESULT_TRUE;
    

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef BLUETOOTH_SUPPORT
		if(MMIAPIBT_IsOpenFileWin())
	    {
	        MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
			MMK_CloseWin(win_id);
	    }
		else
#endif
		{
	        MMIAPISET_OpenPowerSettingMode();        
	        MMK_CloseWin(win_id);
		}
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
        //如果用户点取消，不需要打开省电模式，则需要关闭省电模式并提醒用户
        MMK_CloseWin(win_id);
        MMIAPISET_SetPowerSavingSwitch(FALSE);
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SET_POWER_SAVING_IS_CLOSED,
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
            MMIPUB_SOFTKEY_ONE, PNULL);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;

    }
    return result;
}
#endif

#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
/*****************************************************************************/
//  Description : MMIAPIPHONE_SetInitBatteryCap
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetPreBatteryCap(uint32 cap)
{
    s_pre_battery_cap = cap;
}
/*****************************************************************************/
//  Description : MMIAPIPHONE_GetPreBatteryCap
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPreBatteryCap(void)
{
    return s_pre_battery_cap;
}
#endif
/*****************************************************************************/
//  Description : LowPower_PowerOff
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 低电量关机
/*****************************************************************************/
LOCAL void LowPower_PowerOff(void)
{
#ifdef VT_SUPPORT
    if(MMIAPIVT_IsVtConnecting())
    {
        MMIAPIVT_ReleaseVTCallByRedkey();   
    }
#endif   
#ifdef ZDT_PLAT_YX_SUPPORT
    g_low_shudown_battery_cap = 1;
#endif
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);
    }
    // 关闭所有的窗口
    MMK_ReturnIdleWin();    
#ifdef JAVA_SUPPORT_IA  //解决移动mm,低电量关闭，再次开机mm不能启动
        //低电量关机时同步关掉jvm
     MMIAPIJAVA_stopVM_sync();
#endif
#ifdef WRE_SUPPORT
    MMIWRE_StopMM();
#endif
    MMIPHONE_PowerOff();
}

LOCAL uint16 s_check_low_bat_counter = 0;
/*****************************************************************************/
//  Description : CheckLowBatTimerHandle
//  Global resource dependence : 
//  Author:feng.xiao
//  Note: 检查开机时电量，开机画面可能没有关闭时，low bat消息就发过来。为了保证
//        能弹出提示窗口，开启一个定时器查询开机窗口是否close.
/*****************************************************************************/
LOCAL void CheckLowBatTimerHandle(uint8  timer_id, uint32 param)
{
    if (!MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID))
    {
        HandleChrWarningInd();       
    }
    else
    {
        if (s_check_low_bat_counter++ < 60)
        {
            MMK_CreateTimerCallback(1000, CheckLowBatTimerHandle, NULL, FALSE);
        }
    }
}
