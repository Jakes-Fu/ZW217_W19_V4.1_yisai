/****************************************************************************
** File Name:      mmicc_internal.h                                        *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the mmi cc internal api   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 9/2006        bruce.chi         Create
****************************************************************************/
#ifndef  _MMICC_INTERNAL_H_
#define  _MMICC_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmicc_export.h"
#include "mmicc_position.h"
#include "mmi_custom_define.h"
#include "mmicl_export.h"
#include "cafapplet.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmisrvrecord_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMICC_PHONE_NUM_MAX_LEN					(MMIPB_BCD_NUMBER_MAX_LEN << 1)


#define CC_INVALID_ID						255		//非法ID
#define CC_TIMER_OUT						1000	//通话计时TIMER周期
#define CC_DIAL_TIMER_OUT					10000	//拨号TIMER周期
#define CC_DISCONNECT_TIMER_OUT_FOR_DISPLAY				1200	//DISCONNECT TIMER周期
#define CC_DISCONNECT_TIMER_OUT				2000	//DISCONNECT TIMER周期
#define CC_VIBRATE_TIMER_OUT                1000    //振动
#define CC_HOUR_TO_SECOND					3600	//小时换算为秒
#define CC_MINUTE_TO_SECOND					60		//分钟换算为秒
#define CC_INVALID_DTMF_VALUE				0		//非法DTMF值

#define CC_REDIAL_TIMER_OUT                 5000     //失败后重拨间隔时间

#define CC_EMERGENCY_CALL_NUM_MAX_LEN       3        //紧急呼叫号码最大长度

//@samboo vt回铃音临时处理
#define MMI_NT_RING_DIS_ID 5
#define MMI_NT_RING_CON_ID 3

#define CC_HOT_BUTTON_ROW_MAX 2
#define CC_HOT_BUTTON_COLUMN_MAX 3

#define CC_DTMF_SENDING_TIMER_OUT 1000
#define CC_MAX_MPTY_NUM 5

#define CC_CALL_NUM_MAX_LEN_EX       6        //紧急呼叫号码最大长度,LOCAL setting可能会超过3位
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/



//在自动重拨设置下记录当前异常CC断开的号码,时间等
typedef struct 
{
    BOOLEAN is_blacklisted;                     //是否被列入黑名单
    uint8   tele_num[CC_MAX_TELE_NUM_LEN];      //呼叫的号码
    uint8   tele_len;                           //呼叫的号码长度
    uint32  failed_time;                        //最后一次呼叫失败放入黑名单时的时间
}CC_AUTO_CALL_REPEAT_INFO_T;

//自动重拨失败次数超过限制的号码黑名单
typedef struct
{
    BOOLEAN                         is_full;    //黑名单列表是否已满
    MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause; //呼叫断开原因
    CC_AUTO_CALL_REPEAT_INFO_T      autocall_blacklist[CC_MAX_AUTOCALL_BLACKLIST]; //自动重拨失败黑名单
}CC_AUTO_CALL_REPEAT_BLACKLIST_T;

typedef enum
{
    CC_HOLDMENU_NODE_ROOT,		//root
#ifdef MV_SUPPORT
    CC_HOLDMENU_NODE_MAGICVOICE,
#endif
    CC_HOLDMENU_NODE_SHUTTLE,	//hold,retrieve
    CC_HOLDMENU_NODE_PB,		//hold,retrieve
    CC_HOLDMENU_NODE_SMS,		//hold,retrieve
	CC_HOLDMENU_NODE_CL,		//CL
    CC_HOLDMENU_NODE_RECORD,	//hold,retrieve
    CC_HOLDMENU_NODE_HANDFREE,  //hold,retrieve
    CC_HOLDMENU_NODE_MUTE,		//hold,retrieve
    //CC_HOLDMENU_NODE_SPEAKER,	//hold,retrieve
    CC_HOLDMENU_NODE_BLUETOOTH,  //hold,retrieve
    CC_HOLDMENU_NODE_BT_CONNECT_HF, //hsp new freature
    CC_HOLDMENU_NODE_HANDHOLD,  //hold,retrieve
    CC_HOLDMENU_NODE_RELEASE_SPECIFIED_CALL, 
    CC_HOLDMENU_NODE_RELEASE_ALL,
    CC_HOLDMENU_NODE_RELEASE_ACTIVE,
    CC_HOLDMENU_NODE_RELEASE_HOLD,
    CC_HOLDMENU_NODE_BUILDMPTY,
    CC_HOLDMENU_NODE_SPLIT,
    CC_HOLDMENU_NODE_DTMF,
    CC_HOLDMENU_NODE_BROWSER,    //to web page
    CC_HOLDMENU_NODE_STK,    //to stk //for orange
    CC_HOLDMENU_NODE_ECT_TRANSFER, //ECT
    CC_HOLDMENU_NODE_END_CALLS,
    CC_HOLDMENU_NODE_VOLUME,
    CC_HOLDMENU_NODE_VIEW_MULTICALL
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    ,CC_HOLDMENU_NODE_NEW_CALL,
    CC_HOLDMENU_NODE_MERGE_CALLS
#endif
}CC_HOLDMENU_NODE_E;

typedef enum
{
    CC_DISC_CAUSE_NORMAL,
    CC_DISC_CAUSE_USER_BUSY,
    CC_DISC_CAUSE_USER_NOT_RESPOND,
    CC_DISC_CAUSE_ERROR_NUMBER,
    CC_DISC_CAUSE_CALL_REJECTED,
    CC_DISC_CAUSE_RES_UNAV,
    CC_DISC_CAUSE_HIDE_ID,
    CC_DISC_CAUSE_SERVICE_UNAV,
    CC_DISC_CAUSE_SERVICE_NOT_IMP,
    CC_DISC_CAUSE_INVALID_MES,
    CC_DISC_CAUSE_PROTOCOL_ERROR,
    CC_DISC_CAUSE_NETWORK_ERROR,
    CC_DISC_CAUSE_BEING_EXCEED_LIMIT
}CC_MN_CALL_DISC_CAUSE_E;

typedef enum
{
    MMICC_MO_START_IND,
    MMICC_MT_START_IND,
    MMICC_MO_CONNECTED_IND,
    MMICC_MT_CONNECTED_IND,
    MMICC_DISCONNECTED_IND,
    MMICC_DISCONNECTED_CNF,
    MMICC_HOLD_SUC_IND,
    MMICC_HOLD_FAIL_IND,
    MMICC_CALL_AFTER_HOLD_FAIL_IND,
    MMICC_RETRIEVE_SUC_IND,
    MMICC_RETRIEVE_FAIL_IND,
    MMICC_BUILD_MPTY_SUC_IND,
    MMICC_BUILD_MPTY_FAIL_IND,
    MMICC_SPLIT_MPTY_SUC_IND,
    MMICC_SPLIT_MPTY_FAIL_IND,
#ifdef MMI_VOLTE_SUPPORT
    MMICC_VOLTE_CALL_IND, //update volte icon in cc UI
#endif
    MMICC_UPDATESTATUS_TYPE_MAX
}MMICC_UPDATESTATUS_TYPE_E; //call status type

typedef struct
{
    MN_DUAL_SYS_E dual_sys;//current sim
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 1];//phone number
    uint8 tele_len;//phone number wstr_len
    MMI_STRING_T *disp_ptr;//disp ptr
    MMI_STRING_T *name_ptr;//name ptr
    CC_CALL_SIM_TYPE_E call_sim_type;//call  sim type
    uint32 call_type; //call type
    MN_SUBADDR_T* sub_addr; //分机号
}CC_MAKE_CALL_INFO;

typedef enum
{
    CC_BUTTON_HANDFREE,
    CC_BUTTON_DIALPANEL,
    CC_BUTTON_MUTE,
    CC_BUTTON_RECORD,
    CC_BUTTON_HOLD,
    CC_BUTTON_PB,
#if !defined(MMI_GUI_STYLE_TYPICAL)
    CC_BUTTON_SILENCE,
    CC_BUTTON_HANGUP,
    CC_BUTTON_ANSWER,
    CC_BUTTON_NEW_CALL,
    CC_BUTTON_MERGE_CALLS,
    CC_BUTTON_SWAP_CALLS,
    CC_BUTTON_RETRIEVE_CALLS,
#endif
#ifdef ASP_SUPPORT 
    CC_BUTTON_ASP,
#endif
    CC_BUTTON_MAX
}CC_CC_BUTTON_E;

#if !defined(MMI_GUI_STYLE_TYPICAL)
typedef enum
{
    CC_DISPLAY_NONE,
    CC_DISPLAY_MT_LOCK,                            //显示屏锁状态(来电)
    CC_DISPLAY_HOT_BUTTONS,             //显示热键状态(来电/去电/通话中)
    CC_DISPLAY_DTMF_PANEL,                //显示DTMF拨号盘状态(来电/去电/通话中)
    CC_DISPLAY_TYPE_MAX
}CC_DISPLAY_TYPE_E;			//用户对来电的操作
#endif

typedef struct
{
    uint8  num; //auto dial times num
    uint8  num_max; //auto dial times max
    //CC_CALL_MODE_E mode; //auto dial type
} CC_AUTODIAL_INFO;

typedef struct 
{
#ifndef MMI_GUI_STYLE_TYPICAL
    CC_CC_BUTTON_E cc_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX];
    CC_DISPLAY_TYPE_E   cc_display_type;//cc界面显示状态:用以区分锁屏、热键、DTMF拨号盘这三个显示单元。
    uint8 mmicc_disconnect_timer_for_display;   //挂断窗口闪烁效果计时器
    uint8 mmicc_move_unlock_icon_timer;   //来电锁屏icon移动计时器
    uint8 mmicc_anim_unlock_icon_timer;   //来电锁屏方向指示动画计时器
    uint8 unlock_anim_frame; //来电锁屏方向指示动画帧数
#else //MMI_GUI_STYLE_TYPICAL
    CC_CC_BUTTON_E (*current_win_button_order_ptr)[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX];
#endif //MMI_GUI_STYLE_TYPICAL

#if !defined(TOUCH_PANEL_SUPPORT)
    CC_CC_BUTTON_E cc_win_focus_button_type;//通话窗口获得焦点的button
#endif

    CC_AUTODIAL_INFO autodial_info;
    CC_AUTO_CALL_REPEAT_BLACKLIST_T call_repeat_info;//记录自动重拨黑名单的结构体
    uint8 mmicc_disconnect_timer;   //挂断窗口控制计时器
    uint8 dtmf_timer_id; //start dtmf timer id
    uint8 call_end_hint_timer; //一通电话挂断时，播放嘟嘟音的定时器
    BOOLEAN is_play_call_end_hint ;//挂断电话，是否播放挂断音
    BOOLEAN is_original_phone_lock ;	//进行通话之前的锁屏状态(使用于锁屏来电及锁屏紧急拨号)
    MMI_STRING_T  sim_status; //sim卡状态：主要用于显示
    uint8 waiting_call_ring_timer; //呼叫等待时，间隔播放tone音的timer
    uint8 call_time_count_timer; //call time timer
    uint8 dtmf_sending_timer; //dtmf sending timer
    BOOLEAN is_need_light_on_cc_win;//是否首次进入CC界面，此时需要打开背光
    BOOLEAN is_dtmf_switch; // dtmf switch in menu
    uint8 call_end_index;//记录挂断电话的索引值
#ifdef MMI_CALL_MINIMIZE_SUPPORT    
    BOOLEAN is_minimize; //是否被最小化
#endif    
}MMICC_INSTANCE_T;

typedef struct 
{
    CAF_APPLET_T caf_applet;
    MMICC_INSTANCE_T cc_instance;
}MMICC_APPLET_T;   //vt applet定义

#ifdef CMCC_UI_STYLE
typedef enum
{
    CC_EMERGENCY_MENU_NODE_ROOT,
    CC_EMERGENCY_MENU_NODE_HANDFREE,   //免提
    CC_EMERGENCY_MENU_NODE_MUTE,  //静音
    CC_EMERGENCY_MENU_NODE_HUNG_UP,  //结束
    CC_EMERGENCY_MENU_NODE_MAX
}CC_EMERGENCY_MENU_NODE_E;
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
// 	Description : the process message function of the CC application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICC_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : init the cc global
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMICC_Init_Global(void);

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify CC
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleSIMPlugNotifyCCFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_RegNv(void);

/*****************************************************************************/
// 	Description : Register cc menu group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_RegMenuGroup(void);


/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_RegWinIdNameArr(void);

/*****************************************************************************/
//  Description : Get CC Applet Instance
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:可以返回PNULL，调用函数需要做判断
/*****************************************************************************/
PUBLIC MMICC_APPLET_T *MMICC_GetAppletInstance(void);

/*****************************************************************************/
//  Description : Start cc applet 
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StartCCApplet(void);

/*****************************************************************************/
//  Description : Stop cc applet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StopCCApplet(void);

/*****************************************************************************/
// 	Description : emergency call win msg
//	Global resource dependence : 
//  Author:bownzhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleEmergencyCallWinMsg(
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      );


/*****************************************************************************/
// 	Description : 获取当前呼叫的类型
//	Global resource dependence : 
//  Author:xiongxiaoyan 
//	Note:20060404
/*****************************************************************************/
PUBLIC CC_CALL_CONTEXT_T* MMICC_GetCallText(void);

/*****************************************************************************/
// 	Description : update cc status win
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_UpdateStatusWin(void);

/*****************************************************************************/
//  Description : get current call number
//  Global resource dependence : none
//  Author: baokun
//  Note: 仅供cc audio模块使用
/*****************************************************************************/
PUBLIC void MMICC_GetCurrentCallNumber(MMIPB_BCD_NUMBER_T *bcd_number_ptr, BOOLEAN is_remove_ip_head, BOOLEAN is_want_mo_call);

/*****************************************************************************/
// 	Description : 获取当前通话时间信息
//	Global resource dependence : 
//  Author:xiongxiaoyan
//	Note:获取当前通话时间信息
/*****************************************************************************/
PUBLIC uint32 MMICC_GetCurrentCallTime(void);

/*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_CALL_STATE_E	MMICC_GetCurrentCallStatus(void);

 /*****************************************************************************/
// 	Description : judge whether allowed to send dtmf when in outgoing or incoming call
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: not for connected state
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAllowSendDtmf(void);

/*****************************************************************************/
// 	Description : judge whether allowed to add new call
//	Global resource dependence : 
//  Author : tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAllowAddNewCall(void);

/*****************************************************************************/
// 	Description : judge whether allowed to send dtmf when in outgoing or incoming call
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: not for connected state
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsSendingStoredDtmf(void);

 /*****************************************************************************/
// 	Description : get call num
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetCallNum(void);

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsCurrentCallIDValid(void);

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetCurrentCallIndex(void);
#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetLastActiveCallIndex(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetLastActiveCallIndex(void);
#endif
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: num_str_ptr must be allocated or be fixed length array, 
//   and the size must be more than MMICC_PHONE_NUM_MAX_LEN + 2
//   num_str_ptr & num_len_ptr [IN]
/*****************************************************************************/
PUBLIC void MMICC_GetWantCallNumStr(uint8* num_str_ptr, uint8* num_len_ptr);

  /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: return value means if the call specified str exists
//   spec_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetWantCallSpecifiedStr(MMI_STRING_T* spec_str_ptr);
  
 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: return value means if the call name exists
//   name_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetWantCallNameStr(MMI_STRING_T* name_str_ptr);

  /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: return value means if the call name exists
//   name_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsWantCallExist(void);

  /*****************************************************************************/
// 	Description : judge whether allowed to send dtmf when in outgoing or incoming call
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: not for connected state
/*****************************************************************************/
PUBLIC void MMICC_ClearWantCallInfo(void);

    /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMICC_GetWantCallDualSys(void);

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: num_str_ptr must be allocated or be fixed length array, 
//   and the size must be more than MMICC_PHONE_NUM_MAX_LEN + 2
//   num_str_ptr & num_len_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetCallNumStrByIndex(uint8* num_str_ptr, uint8* num_len_ptr, uint8 call_index);

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMICC_GetPbNumByIndex(uint8 call_index);

 
 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: return value means if the call name exists
//   name_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetCallNameStrByIndex(MMI_STRING_T* name_str_ptr, uint8 call_index);

 /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_CALL_STATE_E	MMICC_GetCallStatusByIndex(uint8 call_index);

  /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_OPERATED_BY_USER_E	MMICC_GetCallOperatedByIndex(uint8 call_index);

  /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMICC_IsCallIndexInMPTY(uint8 call_index);

      /*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8	MMICC_GetCallDirection(uint8 call_index);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_DTMF_STATE_T* MMICC_GetStoredDtmfInfo(void);

/*****************************************************************************/
// 	Description : process when user hang up the call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseCallReq(MN_DUAL_SYS_E dual_sys, uint8 call_index,MN_CALL_DISCONNECT_CALL_CAUSE_E cause);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseActiveCall(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseHoldCall(void);


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_AnswerCall(void);

/*****************************************************************************/
// 	Description : Hold the active call for preparing  for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_HoldAndAnswerCW(void);

/*****************************************************************************/
// 	Description : Release the active call for preparing for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseAndAnswerCW(void);

/*****************************************************************************/
// 	Description : 是否存在正在建立mpty，分离mpty，断开等call
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:2004.11.26
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistOperatedCall(void);

/*****************************************************************************/
// 	Description : 909 problem, call disconnect msg is coming, but the voice  is being
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleDownlinkChanel(void);

/*****************************************************************************/
// 	Description : making redial call , video call or normal call
//	Global resource dependence : s_call_text
//  Author:samboo.shen
//	Note:2007.7.27
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICC_MakeRedialCall( MN_DUAL_SYS_E dual_sys,uint8 *tele_num, uint8 tele_len);

/*****************************************************************************/
// 	Description : process to build MPTY call for accepting the incoming call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_BuildMPTYCall(
                                          void
                                          );

/*****************************************************************************/
// 	Description : process to build MPTY call for accepting the incoming call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_BuildMPTYAndAnswerCW(
    void
    );

/*****************************************************************************/
// 	Description : process hold the call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_HoldCall(
                              void
                              );


/*****************************************************************************/
// 	Description : process to spilt call from MPTY 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SplitSpecCall(
    uint8 call_index
    );

/*****************************************************************************/
// 	Description : is dtmf
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_IsDTMF(uint8* dtmf_str, uint8 dtmf_len);

/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMICC_ClearStoredDtmf(void);

/*****************************************************************************/
// 	Description : send valid dtmf
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendDTMFStr(uint8* dtmf_str, uint8 dtmf_len);

/*****************************************************************************/
//  Description :ShowCallDiscCause
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC CC_MN_CALL_DISC_CAUSE_E MMICC_ShowCallDiscCause(
    MN_CALL_DISCONNECT_CALL_CAUSE_E cause
    );
    
#ifdef ASP_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_AnswerASPCall(void);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint32	MMICC_GetCallTimeCount(void);

/*****************************************************************************/
// 	Description : Process Make Call
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ProcessMakeCall(
    MN_DUAL_SYS_E dual_sys,
    CC_MAKE_CALL_INFO *make_call_info_ptr
    );

/*****************************************************************************/
// 	Description : check is exist outgoing call or not
//	Global resource dependence : s_call_context
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistOutgoingCall(void);

/*****************************************************************************/
// 	Description : check is exist incomming call or not
//	Global resource dependence : s_call_context
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistIncommingCall(void);


/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveMPTY(void);

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistHoldMPTY(void);

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveCall(void);

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistHoldCall(void);

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveAndHoldCalls(void);

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistMPTY(void);

/*****************************************************************************/
//  Description :judge whether the mpty can be setup
//  Global resource dependence : none
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMptySetupAvailable(void);

/*****************************************************************************/
//  Description :judge whether the mpty can be split
//  Global resource dependence : none
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMptySplitAvailable(void);

/*****************************************************************************/
// 	Description : search the call which state is pointed by param
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC uint8   MMICC_FindStateCall(
                            CC_CALL_STATE_E  state 
                            );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetCallTimeCount(uint32 count);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMICC_GetIsNewMissedCall(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetIsNewMissedCall(BOOLEAN is_new_missed_call);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StartDtmfTimerId(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StopDtmfTimerId(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StartDtmfSendingTimer(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StopDtmfSendingTimer(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StarCallTimingTimer(void);

/*****************************************************************************/
// 	Description : switch s_call_context.dtmf_enable
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_SetDtmfSwitch ( BOOLEAN is_enable );

/*****************************************************************************/
// 	Description : write disconnect call cause to nv
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_SetRepeatInfoCause(MN_CALL_DISCONNECT_CALL_CAUSE_E cause);

/*****************************************************************************/
// Description : set auto dial times max
//Global resource dependence : 
//  Author: samboo.shen
//Note:
/*****************************************************************************/
PUBLIC void MMICC_SetAutoDialTimesMax(uint8 times_max);

/*****************************************************************************/
// 	Description : the operation after call absolute end, including the disconnected win closed
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleOperAfterCallEnd(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : tao.xue
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetIsPlayCallEndHintFlag(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : tao.xue
//	Note:   
/*****************************************************************************/
PUBLIC void MMICC_SetIsPlayCallEndHintFlag(BOOLEAN is_enable);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:   
/*****************************************************************************/
PUBLIC void MMICC_ClearAutoRedialInfo(void);

/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendSingleDtmf( uint32 msg_id);

/*****************************************************************************/
//  Description :update a call status text (hold, active)
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.8
/*****************************************************************************/
PUBLIC void MMICC_UpdateCurrentCallStatus(BOOLEAN is_flash);

/*****************************************************************************/
//  Description :update a call processing text (releasing, holding, spliting, etc)
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.8
/*****************************************************************************/
PUBLIC void MMICC_UpdateCurrentCallProcessing(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:   
/*****************************************************************************/
PUBLIC void MMICC_OpenDisconnectWin(
                                   uint8							call_index,
                                   MN_CALL_DISCONNECT_CALL_CAUSE_E	cause,
                                   uint32 call_time
                                   );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_OpenSelectSimWin(ADD_DATA add_data_ptr);

/*****************************************************************************/
// 	Description : to open cc menu
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void  MMICC_OpenCCMenu(void);

/*****************************************************************************/
//  Description : start vibrate for connect call prompt
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_VibrateForConnectPrompt(void);

/*****************************************************************************/
// 	Description : UPDATE CALL STATUS
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_UpdateCallStatusDisplay(MMICC_UPDATESTATUS_TYPE_E type);

/*****************************************************************************/
// 	Description : 是否允许ECT转接
//	Global resource dependence : 
//  Author:
//	Note:
//  Parameter: TRUE: 允许进行ECT转接； FALSE：不允许进行ECT转接
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ECTIsPermit(void);

/*****************************************************************************/
// 	Description : process ect transfer
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ECTTransfer(void);

/*****************************************************************************/
//  Discription: Handle ECT alert
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICC_HandleECTAlertWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            );

/*****************************************************************************/
//  Description : open success window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_OpenAlertSuccessWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : open warning window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMICC_OpenAlertWarningWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description :open fail window by text_id 
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_OpenAlertFailWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description :open waiting window by text_id 
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMICC_OpenAlertWaitingWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsCurrentCallMute(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_SetCurrentCallMute(BOOLEAN is_mute);

/*****************************************************************************/
//  Description :
//  Global resource dependence : s_calllog_type
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMICC_GetCallLogType(void);

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : get whether exist  waiting call bt connetion
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetBtConnWaitCallFlag(void);

/*****************************************************************************/
// 	Description : set whether exist  waiting call bt connetion
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetBtConnWaitCallFlag(BOOLEAN wating_call_flag);

/*****************************************************************************/
// 	Description : get whether exist  bt call
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetBtCallFlag(void);
/*****************************************************************************/
// 	Description : set whether exist  bt call
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetBtCallFlag(BOOLEAN bt_call_flag);
#endif
#ifdef MMI_CSP_SUPPORT
  /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetWantCallDualLine(void);

 /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8	MMICC_GetCallLineByIndex(uint8 call_index);

 /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8	MMICC_GetCurrentCallLine(uint8 call_index);

#endif

#if defined(CALL_REC_FILE_SEPERATE)
PUBLIC MN_DUAL_SYS_E MMICC_GetCurrCallDualsys(void);
#endif

#if !defined(MMI_GUI_STYLE_TYPICAL)
/*****************************************************************************/
//  Description :SET PUBWIN TITLE
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetAlertWinTitleTextId(MMI_TEXT_ID_T text_id);

#endif

#ifdef MV_SUPPORT
/*****************************************************************************/
// 	Description : open magic voice when MMICC_STATUS_WIN_ID open if switch is on
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void OpenMagicVoice(void);
/*****************************************************************************/
// 	Description : set magic voice parameter when MMICC_STATUS_WIN_ID open
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void SetMagicVoiceParam(void);
/*****************************************************************************/
// 	Description : close magic voice when call is disconnected
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void CloseMagicVoice(void);
/*****************************************************************************/
// 	Description : magic voice needs to open later(when call has been connected)
//	Global resource dependence : 
//  Author : fen.xie
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMagicVoiceOpenLater(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_SetIsNeedOpenMv(BOOLEAN is_need_open);

/*****************************************************************************/
// 	Description : when handfree or bluetooth active, can't use magic voice
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsCanUseMagicVoice(
                        BOOLEAN is_for_open_select_win/*IN,TRUE:判断是否需要打开魔音选择窗口*/
                                                      /*FALSE:是否需要打开魔音效果*/
                        )
;
/*****************************************************************************/
// 	Description : whether magic voice is working
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsMagicVoiceWorking(void);
/*****************************************************************************/
// 	Description : reset magic voice value in setting window with nv value
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void ResetMagicVoiceValue(void);

/*****************************************************************************/
// 	Description : reset magic voice value(s_mv_data) used in cc
//	Global resource dependence : 
//  Author : fen.xie
//	Note:MS00170872
/*****************************************************************************/
PUBLIC void ResetCCMagicVoiceValue(void);

/*****************************************************************************/
// 	Description : open main selection window when incoming or outgoing
//	Global resource dependence : 
//  Author : fen.xie
//	Note:   MS00185659
/*****************************************************************************/
PUBLIC void MMICC_OpenMVBeforeCallConnected(ADD_DATA call_info_ptr);

#endif

/*****************************************************************************/
//  Description : Select SIM API of CC app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
/*
PUBLIC MMI_RESULT_E MMICC_SelectSIMFunc(PROCESSMSG_FUNC	MMICCSelectSIMWinCallback,
							MMISET_APPEND_SIM_TYPE_E append_type,
							ADD_DATA		add_data
							);

*/
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : to open cc emergency menu
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void  MMICC_OpenEmergency_OptionMenu(void);
#endif
#ifdef MMI_ETWS_SUPPORT
PUBLIC void  MMIAPICC_PlayNewCallRing(MN_DUAL_SYS_E dual_sys);
#endif

#ifdef MMI_RECORD_SUPPORT

/*****************************************************************************/
//  Description : start record in call
//  Parameter: [In] None
//             [Out] None
//             [Return] Result
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMICC_StartRecordInCall(void);

/*****************************************************************************/
//  Description : stop record in call
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StopRecordInCall(void);

/*****************************************************************************/
//  Description : if is in recording
//  Parameter: [In] None
//             [Out] None
//             [Return] true or false
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsRecording(void);

#endif

#ifdef MMI_VOLTE_SUPPORT
/*****************************************************************************/
//  Description : check is current call path is 4G
//  Global resource dependence :
//  Author: yuming.yang
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsVolteNetworkStatus(void);
#endif
/*****************************************************************************/
// 	Description : open query window, enable ps to do ecc or not?
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_OpenQueryWinForNoPSEcc(MMI_TEXT_ID_T text_id);
/*****************************************************************************/
// 	Description : get whether exist available ps
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsNeedPowerOnPs(uint8 *tele_num, uint8 tele_len);
/*****************************************************************************/
// 	Description : enable network to implement ecc
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_EnableNetworkForECC(void);

/*****************************************************************************/
//  Description : not send dtmf
//  Global resource dependence :
//  Author:mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_CancelStringDtmf(void);

/*****************************************************************************/
//  Description : send  dtmf
//  Global resource dependence :
//  Author:mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendStringDtmf(void);

#endif
