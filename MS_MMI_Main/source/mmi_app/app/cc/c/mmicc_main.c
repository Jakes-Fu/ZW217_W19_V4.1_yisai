/****************************************************************************
** File Name:      mmicc_main.c                                  *
** Author:     samboo.shen                                                            *
** Date:           2/6/2012                                             *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2/2012        samboo.shen         Create
****************************************************************************/
#define _MMICC_MAIN_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cc_trc.h"
#ifdef __cplusplus
extern   "C"
{
#endif
    
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_applet_table.h"
#include "mmi_appmsg.h"
#include "mmk_app.h"

#include "mmi_default.h"
#include "mmiphone_export.h"

#include "mmicc_internal.h"
#include "mmicc_audio.h"
#include "mmicl_internal.h"
#ifdef MOTION_SENSOR_TYPE_OPTICAL
#include "mmk_opsensor.h"
#endif
#include "ual_device.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//global variable declare
MMI_APPLICATION_T			g_cc_app;      // g_cc_app的实体定义

LOCAL uint32 s_device_register_handle = 0; //监听device的handle
LOCAL uint8 s_thermal_close_timer = 0;
LOCAL BOOLEAN s_is_closing_business = FALSE;

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register CC module applet info
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void);

/*****************************************************************************/
//  Description : the process message function of the CC
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  CCApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    );

/*****************************************************************************/
//  Description : reg cc MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Initialize(MMICC_INSTANCE_T *self, DPARAM param);

/*****************************************************************************/
//  Description : reg cc MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Finalize(MMICC_INSTANCE_T *self, DPARAM param);

/*****************************************************************************/
//  Description : HandleHeadsetButtonUp
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonUp(void);

/*****************************************************************************/
//  Description : HandleHeadsetButtonLong
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonLong(void);

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                               *
**--------------------------------------------------------------------------*/

LOCAL MMICC_APPLET_T *s_cc_applet_instance = PNULL; /* Temp*/

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmicc_applet_array[] = 
{
    {   
        GUID_NAME_DEF(SPRD_CC_APPLET_ID), 
        sizeof(MMICC_APPLET_T), 
        CCApplet_HandleEvent , 
#ifndef MMI_RES_LOW_COST
    	IMAGE_SECMENU_ICON_CALL,
#else
    	IMAGE_NULL, 
#endif    	
        TXT_TELEPHONE
    },
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmicc_applet_info = 
{
    (CAF_STATIC_APPLET_T*)s_mmicc_applet_array,
    ARR_SIZE(s_mmicc_applet_array)
};

/*****************************************************************************/
// 	Description : register www module applet info
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_CC, &g_mmicc_applet_info );
}

/*****************************************************************************/
//  Description : the process message function of the CC
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  CCApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMICC_APPLET_T* cc_applet_ptr = (MMICC_APPLET_T*)iapplet_ptr;
#ifdef BT_DIALER_SUPPORT
    BT_STATUS bt_status = BT_ERROR;
#endif

    if(MSG_TIMER != msg_id)
    {
        //SCI_TRACE_LOW:"ccapp Applet_HandleEvent msg_id = 0x%04x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_123_112_2_18_2_10_33_290,(uint8*)"d",msg_id);
    }
    
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_cc_applet_instance = cc_applet_ptr;
        CC_Initialize((&(cc_applet_ptr->cc_instance)), param);
        MMK_SetAppletState(cc_applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_DISABLE_RETURN_IDLE, TRUE);
        //MMICC_ConstructAudioHandle();//bug2066958,此部分内容在ual tele内完成
#ifdef MOTION_SENSOR_TYPE_OPTICAL
        SCI_TRACE_LOW("optical sensor IN CC state start");
        MMK_OpenOPsensor(VIRTUAL_WIN_ID);
#endif
        break;
        
    case MSG_APPLET_STOP: 
        CC_Finalize((&(cc_applet_ptr->cc_instance)), param);
        s_cc_applet_instance = PNULL;
        //MMICC_DestroyAudioHandle();
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
#ifdef MOTION_SENSOR_TYPE_OPTICAL
        SCI_TRACE_LOW("optical sensor IN CC state stop");
        MMK_CloseOPsensor(VIRTUAL_WIN_ID);
#endif

#ifdef MMI_ETWS_SUPPORT
        if( TRUE == MMIAPISMSCB_GetIsNeedRedisplay() )
        {
            MMIAPISMSCB_ShowNewETWSPrompt();
        }
#endif

        break;

    case MSG_APPLET_SUSPEND:
        MMK_SetAppletState(cc_applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
#ifdef MMI_CALL_MINIMIZE_SUPPORT         
        MMICC_GetAppletInstance()->cc_instance.is_minimize = TRUE;
#endif
        break;

#ifdef MMI_CALL_MINIMIZE_SUPPORT 
    case MSG_APPLET_RESUME:
        MMICC_GetAppletInstance()->cc_instance.is_minimize = FALSE;
        break;
#endif        

    case MSG_APPLET_SWITCH:
        MMK_SetAppletState(cc_applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_BG_HIDE_WIN, FALSE);        
        break;

#ifdef MMI_CALL_MINIMIZE_SUPPORT
    case MSG_CC_RELEASE_ALL_CALL:
        MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);
        break;
#endif        

    case MSG_TIMER:
        if (s_cc_applet_instance->cc_instance.call_time_count_timer == *(uint8*)param)
        {
            MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_TIMER, param);
        }
        /*else if (s_cc_applet_instance->cc_instance.dtmf_sending_timer == *(uint8*)param)
        {
            MMIAPICC_StopDtmf(MN_DUAL_SYS_MAX);
        }*/
        break;

    case MSG_APP_RED:
#if defined(KEYPAD_TYPE_FOUR_KEY_UNIVERSE) && defined(MMI_CALL_MINIMIZE_SUPPORT)//@tao.xue  renwei
        if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) && MMK_IsFocusWin(MMICC_STATUS_WIN_ID))
        {
            MMIAPICC_HideCCApplet();
            MMK_ReturnIdleWin();
        }
        else
        {
            MMK_DispMsgToWin(msg_id, param);
        }
#else
        //不能马上退回IdleWin
#ifdef BT_DIALER_SUPPORT   
        if(MMICC_GetBtCallFlag())
        {
            bt_status = MMIAPIBT_HfuHangup();
            if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
            {
                SCI_TRACE_LOW("ccapp.c:CC_BtCallClipInd() MMIAPIBT_HfuHangup fail");
            }
        }
        else
#endif
        {
            MMIAPICC_ReleaseCallByRedkey();	
        }
#endif       
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYUP_FLIP:
        {
            BOOLEAN is_flip_answer = MMIAPISET_GetIsFlipAnswer(MMIAPICC_GetCallDualSys());//@fen.xie MS00175075
            MMIDEFAULT_HandleFlipKey(TRUE);
            if(is_flip_answer)
            {
                MMIAPICC_StopMtCallPlayingRing();
                if(!MMICC_IsCurrentCallIDValid() || MMICC_IsExistOperatedCall() )
                {
                    //can not do other handle in this state
                    break;
                }
                if(MMICC_GetCurrentCallStatus()== CC_INCOMING_CALL_STATE)
                {
                    if(!MMICC_AnswerCall())			
                    {
                        //????
                    }
                    //MMK_CloseWin( MMICC_ANIMATION_WIN_ID );
                }
            }		     
        }
        break;
#endif

    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        //Answer the Phone
        if (MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID))
        {
            MMK_SendMsg(MMICC_ANIMATION_WIN_ID, msg_id, NULL);
        }
        break;

    case MSG_CC_STOP_APPLET:
        if (!MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
            && !MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
            && !MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))//for STK hold redial
        {
            MMICC_StopCCApplet();
        }
        break;

    case (MMISRVAUD_EARPHONE_EVENT_CLK + KEY_HEADSET_BUTTIN):  // 中键单击
#ifdef BT_DIALER_SUPPORT   
            if(MMICC_GetBtCallFlag())
            {
                if (CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
                {
                    MMICC_AnswerCall();
                }
                else 
                {
                    bt_status = MMIAPIBT_HfuHangup();
                    if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
                    {
                        SCI_TRACE_LOW("ccapp.c:CCApplet_HandleEvent() KEY_HEADSET_BUTTIN MMIAPIBT_HfuHangup fail");
                    }
                }
            }
            else
            {
#endif
                HandleHeadsetButtonUp();    
#ifdef BT_DIALER_SUPPORT  
            }
#endif
            break;  

    case (MMISRVAUD_EARPHONE_EVENT_LONG + KEY_HEADSET_BUTTIN): // 中键长按
#ifdef BT_DIALER_SUPPORT   
        if(MMICC_GetBtCallFlag())
        {
            if (CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            {
                MMICC_AnswerCall();
            }
            else 
            {
                bt_status = MMIAPIBT_HfuHangup();
                if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
                {
                    SCI_TRACE_LOW("ccapp.c:CCApplet_HandleEvent() KEY_HEADSET_BUTTIN long MMIAPIBT_HfuHangup fail");
                }
            }
        }
        else
        {
#endif
            HandleHeadsetButtonLong();
#ifdef BT_DIALER_SUPPORT  
        }
#endif
        break;
 
    case (MMISRVAUD_EARPHONE_EVENT_DBLCLK + KEY_HEADSET_BUTTIN): // 中键双击
         //do nothing
         break;
        

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 关闭当前的通话界面
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
LOCAL void CC_ThermalCloseBusiness(uint8 timer_id, uint32 param)
{
    if(0 != s_thermal_close_timer)
    {
        SCI_TRACE_LOW("Thermal:cc release all call");

        MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
        if(MMK_IsOpenWin( WATCHCC_THERMAL_WIN_ID ))
        {
            MMK_CloseWin(WATCHCC_THERMAL_WIN_ID);
        }
        s_is_closing_business = FALSE;
        MMK_StopTimer(s_thermal_close_timer);
        s_thermal_close_timer=0;
    }
}

/*****************************************************************************/
//  Description: cc监听ual device模块消息
//  Parameter: [In] msg //ual device上来的消息
//             [Out] None
//             [Return] none
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_HandleDeviceMsg(ual_cms_msg_t msg)
{
    SCI_TRACE_LOW("msg.msg_id = %d.", msg.msg_id);
    switch (msg.msg_id)
    {
        case MSG_UAL_DEVICE_THERMAL_CLOSE_BUSINESS:
        {
            SCI_TRACE_LOW("Thermal:receive MSG_UAL_DEVICE_THERMAL_CLOSE_BUSINESS in cc");
            if((0 < MMICC_GetCallNum())&&(FALSE == s_is_closing_business))//have calling
            {
                s_is_closing_business = TRUE;
                WatchCC_Thermal_Win_Enter();
                s_thermal_close_timer = MMK_CreateTimerCallback(MMI_3SECONDS, CC_ThermalCloseBusiness, NULL, FALSE);
            }
            break;
        }
        case MSG_UAL_DEVICE_THERMAL_REOPEN_BUSINESS:
        {
            SCI_TRACE_LOW( "Thermal:receive MSG_UAL_DEVICE_THERMAL_REOPEN_BUSINESS in cc");
            break;
        }
        default:
            break;
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : init the cc application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMIAPICC_Init(void)
{
#ifdef SIM_PLUG_IN_SUPPORT 
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
#endif
    //define call back function and component type of CC
    g_cc_app.ProcessMsg = MMICC_HandlePsMsg;
    //    g_cc_app.component_type = CT_APPLICATION;
    
    //初始化全局变量
    MMICC_Init_Global();
    //s_is_open_auto_dial_win = FALSE;
    
#ifdef SIM_PLUG_IN_SUPPORT     
    sim_plug_notify_info.module_id = MMI_MODULE_CC;
    sim_plug_notify_info.notify_func = MMICC_HandleSIMPlugNotifyCCFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif

#if defined(IKEYBACKUP_SUPPORT)
    //cl如果未插卡，不会调用mmicl_init
    MMICL_BackupRegister();
#endif
    //register ual device
    ual_device_register(CC_HandleDeviceMsg, &s_device_register_handle);
}

/*****************************************************************************/
//  Description : reg cc, cl menu group and nv group
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_InitModule(void)
{
    MMICC_RegMenuGroup();
    MMICC_RegNv();
    MMICC_RegWinIdNameArr();
    RegAppletInfo();
}

/*****************************************************************************/
//  Description : CC_Initialize
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Initialize(MMICC_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"ccapp CC_Initialize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_227_112_2_18_2_10_33_291,(uint8*)"");
#ifndef MMI_GUI_STYLE_TYPICAL
    SCI_MEMSET(&self->cc_win_button_order, 0x00, sizeof(self->cc_win_button_order));
    self->cc_display_type = CC_DISPLAY_NONE;
#ifndef MMI_CALLCONTROL_MINI_SUPPORT    
    self->mmicc_disconnect_timer_for_display = 0;
#endif
    self->mmicc_move_unlock_icon_timer = 0;
#else
    self->current_win_button_order_ptr = PNULL;
#endif

#if !defined(TOUCH_PANEL_SUPPORT)
    self->cc_win_focus_button_type = 0;
#endif

    SCI_MEMSET(&self->autodial_info, 0x00, sizeof(self->autodial_info));
    SCI_MEMSET(&self->call_repeat_info, 0x00, sizeof(self->call_repeat_info));
    self->mmicc_disconnect_timer = 0;
    self->dtmf_timer_id = 0;
    self->call_end_hint_timer = 0;
    self->is_play_call_end_hint = FALSE;
    SCI_MEMSET(&self->sim_status, 0x00, sizeof(self->sim_status));    
    self->waiting_call_ring_timer = 0;
    self->call_time_count_timer = 0;
    self->dtmf_sending_timer = 0;
    self->is_need_light_on_cc_win = FALSE;
    self->is_dtmf_switch = TRUE;
    self->call_end_index = 0;
#ifdef MMI_CALL_MINIMIZE_SUPPORT    
    self->is_minimize = FALSE;
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : cc_Finalize
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Finalize(MMICC_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"ccapp CC_Finalize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_263_112_2_18_2_10_33_292,(uint8*)"");
    if(MMI_NONE_TIMER_ID !=  self->call_time_count_timer )
    {
        MMK_StopTimer( self->call_time_count_timer );
        self->call_time_count_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->call_end_hint_timer)
    {
        MMK_StopTimer( self->call_end_hint_timer );
        self->call_end_hint_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->mmicc_disconnect_timer)
    {
        MMK_StopTimer( self->mmicc_disconnect_timer );
        self->mmicc_disconnect_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->dtmf_sending_timer)
    {
        MMK_StopTimer( self->dtmf_sending_timer );
        self->dtmf_sending_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->dtmf_timer_id)
    {
        MMK_StopTimer( self->dtmf_timer_id );
        self->dtmf_timer_id = MMI_NONE_TIMER_ID;
    }
#ifndef MMI_GUI_STYLE_TYPICAL
    if(MMI_NONE_TIMER_ID !=  self->mmicc_move_unlock_icon_timer)
    {
        MMK_StopTimer( self->mmicc_move_unlock_icon_timer );
        self->mmicc_move_unlock_icon_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->mmicc_anim_unlock_icon_timer)
    {
        MMK_StopTimer( self->mmicc_anim_unlock_icon_timer );
        self->mmicc_anim_unlock_icon_timer = MMI_NONE_TIMER_ID;
    }
#ifndef MMI_CALLCONTROL_MINI_SUPPORT    
    if(MMI_NONE_TIMER_ID !=  self->mmicc_disconnect_timer_for_display)
    {
        MMK_StopTimer( self->mmicc_disconnect_timer_for_display );
        self->mmicc_disconnect_timer_for_display = MMI_NONE_TIMER_ID;
    }
#endif    

#endif
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Get CC Applet Instance
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:可以返回PNULL，调用函数需要做判断
/*****************************************************************************/
PUBLIC MMICC_APPLET_T *MMICC_GetAppletInstance(void)
{
    return s_cc_applet_instance;
}

/*****************************************************************************/
//  Description : Start cc applet 
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StartCCApplet(void)
{
    MMI_APPLET_START_T start ={0};
    uint32 user_data = 0;
    //SCI_TRACE_LOW:"ccapp StartCCApplet"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_289_112_2_18_2_10_33_293,(uint8*)"");
    start.guid = SPRD_CC_APPLET_ID;
    start.state = MMI_APPLET_STATE_TASK_MANAGER;
    start.param_ptr = &user_data;
    start.size_of_param = sizeof(uint32);
    MMK_StartApplet( &start );
}

/*****************************************************************************/
//  Description : Stop cc applet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StopCCApplet(void)
{

    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    
    //SCI_TRACE_LOW:"ccapp Stop cc applet"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_305_112_2_18_2_10_33_294,(uint8*)"");


    if (PNULL != cc_applet_ptr)
    {
        MMK_CloseApplet(cc_applet_ptr->caf_applet.app_handle);
    }
}

/*****************************************************************************/
//  Description :close codec handle & audio handle when powering off for normally play poweroff audio or video
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in power off procede
/*****************************************************************************/
PUBLIC void MMIAPICC_QuitForPowerOff ( void )
{
    MMICC_EnableVoiceCodec(FALSE);
    MMICC_DestroyVoiceCodec();
    MMICC_DestroyAudioHandle();
    MMICC_StopCCApplet();
}


/*****************************************************************************/
//  Description : MMIAPICC_SendMsgToApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_SendMsgToApplet(MMI_MESSAGE_ID_E msg_id, DPARAM param_ptr)
{

    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    MMI_HANDLE_T cc_handle = PNULL;
    
    //SCI_TRACE_LOW:"MMIAPICC_SendMsgToApplet msg:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_324_112_2_18_2_10_33_295,(uint8*)"d", msg_id);

    if (PNULL != cc_applet_ptr)
    {
        cc_handle = cc_applet_ptr->caf_applet.app_handle;
        MMK_SendMsg(cc_handle, msg_id, PNULL);
    }
}
/*****************************************************************************/
//  Description : HandleHeadsetButtonUp
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonUp(void)
{    
    if(CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
    {
        MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
    }
    else if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) // just only 1 incoming call
    {
        MMICC_AnswerCall();//MMIAPICC_ConnectCallReq();
    }
    else if(CC_WAITING_STATE == MMICC_GetCurrentCallStatus()) //waiting
    {
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            //do nothing
        }
        else
        {
            MMICC_HoldAndAnswerCW();
        }
    }
    else if(CC_CALL_STATE == MMICC_GetCurrentCallStatus())
    {
#if 0    
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
        }
        else
#endif            
        {
            MMICC_ReleaseActiveCall();//MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
        }
    }
    else if(CC_HOLD_STATE == MMICC_GetCurrentCallStatus())
    {
    	MMICC_ReleaseHoldCall();
    }
        
}

/*****************************************************************************/
//  Description : HandleHeadsetButtonLong
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonLong(void)
{
    if(CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
    {
        MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
    }
    else if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) // just only 1 incoming call
    {
        MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
    }
    else if(CC_WAITING_STATE == MMICC_GetCurrentCallStatus()) //waiting
    {
#if 0    
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
        }
        else
#endif            
        {
            MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
        }
    }
    else if(((CC_CALL_STATE == MMICC_GetCurrentCallStatus()) || (CC_HOLD_STATE == MMICC_GetCurrentCallStatus())))
    {
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);
        }
        else
        {
            MMICC_HoldCall();
        }
    }
}
#ifdef MMI_CALL_MINIMIZE_SUPPORT            
/*****************************************************************************/
//  Description : MMIAPICC_ActiveCCApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 让cc  applet获得焦点，返回值TRUE：代表cc applet存在，FALSE：表示applet不存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ActiveCCApplet(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    MMI_HANDLE_T cc_handle = PNULL;
    BOOLEAN result = FALSE;

    if (PNULL != cc_applet_ptr)
    {
        cc_handle = cc_applet_ptr->caf_applet.app_handle;
        if (PNULL != cc_handle)
        {
            if(!MMICC_GetAppletInstance()->cc_instance.is_minimize)
            {
                SCI_TRACE_LOW("ccapp active applet fail as not min");
                result = FALSE;
            }
            else if(MMK_GrabWinFocusByApplet(cc_handle))
            {
                //MMICC_GetAppletInstance()->cc_instance.is_minimize = FALSE;
                result = TRUE;
            }
        }
    }   
    //SCI_TRACE_LOW:"ccapp ActiveCC result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_358_112_2_18_2_10_33_296,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : MMIAPICC_HideCCApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 让cc  applet隐藏，返回值TRUE：代表cc applet存在，FALSE：表示applet不存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_HideCCApplet(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    MMI_HANDLE_T cc_handle = PNULL;
    BOOLEAN result = FALSE;

    if (PNULL != cc_applet_ptr)
    {
        cc_handle = cc_applet_ptr->caf_applet.app_handle;
        if (PNULL != cc_handle)
        {
            if(MMK_HideWinByApplet(cc_handle))
            {
                //MMICC_GetAppletInstance()->cc_instance.is_minimize = TRUE;
                result = TRUE;
            }
        }
    }   
    //SCI_TRACE_LOW:"ccapp HideCC result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_384_112_2_18_2_10_33_297,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : get cc applet handle
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsMinimize(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    BOOLEAN is_minimize = FALSE;

    if(PNULL != cc_applet_ptr)
    {
        is_minimize = cc_applet_ptr->cc_instance.is_minimize;
    }

    return is_minimize;
}

/*****************************************************************************/
//  Description : get cc applet handle
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPICC_GetCCAppletHandle(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    if(PNULL == cc_applet_ptr)
    {
        return 0;
    }
    else
    {
        return cc_applet_ptr->caf_applet.app_handle;
    }
}
#endif//MMI_CALL_MINIMIZE_SUPPORT
    
#ifdef   __cplusplus
    }
#endif
    
