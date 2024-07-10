/*****************************************************************************
** File Name:      mmicc_id.h                                                *
** Author:                                                                   *
** Date:           2006/09/26                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe cc win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/26     kelly.li        Create                                    *
******************************************************************************/

#ifndef _MMICC_ID_H_
#define _MMICC_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef CMCC_UI_STYLE
#define MMI_CALLLOG_SUPPORT_MULTI_DELETE
#endif


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    MMI_CC_WIN_ID_START = (MMI_MODULE_CC << 16),

#include "mmicc_id.def"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_dial_cl_contact_id.def"
#endif
    MMICC_MAX_WIN_ID
}MMICC_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMICC_CTRL_ID_START = MMICC_MAX_WIN_ID,//(MMI_MODULE_CC << 16),
//cc watch
    WATCHCC_BACK_BUTTON_CTRL_ID,
    WATCHCC_DIAL_BUTTON_CTRL_ID,
    WATCHCC_HANGUP_BUTTON_CTRL_ID,
    WATCHCC_HANGUP_DISABLE_BUTTON_CTRL_ID,

#ifdef ADULT_WATCH_SUPPORT
    WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID,
    WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID,
    WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID,

    WATCHCC_MUTE_DISABLE_BUTTON_CTRL_ID,
    WATCHCC_MUTE_BUTTON_CTRL_ID,
    WATCHCC_UNMUTE_BUTTON_CTRL_ID,

    WATCHCC_VOLUME_BUTTON_CTRL_ID,
    WATCHCC_DIALPAD_BUTTON_CTRL_ID,
#endif
    WATCHCC_CALLSTATE_LABEL_CTRL_ID,
    WATCHCC_CALLTIME_LABEL_CTRL_ID,

	WATCH_DIALPAD_EDITBOX_CTRL_ID,
	//dialpad button
	WATCH_DIALPAD_NUM1_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM2_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM3_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM4_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM5_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM6_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM7_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM8_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM9_BUTTON_CTRL_ID,
	WATCH_DIALPAD_ASTERISK_BUTTON_CTRL_ID,
	WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID,
	WATCH_DIALPAD_POUND_BUTTON_CTRL_ID,
	WATCH_DIALPAD_DIAL_BUTTON_CTRL_ID,
	WATCH_DIALPAD_SYMBOL_BUTTON_CTRL_ID,
	WATCH_DIALPAD_DELETE_BUTTON_CTRL_ID,

	
//cc
    MMICC_DIALNUMWIN_EDITBOX_CTRL_ID,	// the edit control of CC DIAL Window
    MMICC_MENUWIN_MENU_CTRL_ID,         //  the menu control of CC MENU Window
    MMICC_CALLLISTWIN_LISTBOX_CTRL_ID,  //  the list control of CC LIST Window
    MMICC_MULTICALL_LIST_CTRL_ID, // list of all calls in multiparty
    MMICC_HOLDMENUWIN_MENU_CTRL_ID,
	MMICC_SEND_VOICE_MENU_CTRL_ID,		//The menu control for bluet ooth send voice

    //cc mo mt win
    MMICC_CALLING_TEXT_FORM_CTRL_ID,    //form include four text label
    MMICC_CALLING_STATE_LABEL_CTRL_ID,  //display sim card & call state
    MMICC_CALLING_NAME_LABEL_CTRL_ID,   //display call name if saved in pb
    MMICC_CALLING_NUMBER_LABEL_CTRL_ID, //display call number
    MMICC_CALLING_LOCAT_LABEL_CTRL_ID,  //display call location

    MMICC_CALLING_ANIM_FORM_CTRL_ID,    //form include state anim & photo anim
    MMICC_CALLING_STATE_ANIM_CTRL_ID,   //display anim means mo or mt
    MMICC_PHOTO_ANIM_CTRL_ID,           //display photo anim control id

    //cc connected win
    MMICC_CONNECTED_INFO_FORM_CTRL_ID,  //
    MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID, 

    MMICC_CONNECTED_TEXT_CHILD_FORM_CTRL_ID,//
    MMICC_CONNECTED_STATE_LABEL_CTRL_ID,  //display sim card & call state
    MMICC_CONNECTED_NAME_LABEL_CTRL_ID,   //display call name if saved in pb
    MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, //display call time
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    MMICC_CONNECTED_TIME_LABEL_CTRL_ID, //display call time
    MMICC_CONNECTED_SIM_LABEL_CTRL_ID,  //display sim card
#endif    
    MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID,  //display call location
    MMICC_CONNECTED_REDIAL_TIMES_LABEL_CTRL_ID,  //display call location
    MMICC_CONNECTED_ASP_TIME_LABEL_CTRL_ID,  //display asp record time
    
    MMICC_CONNECTED_HOLD_PHOTO_ANIM_CTRL_ID, 
    MMICC_CONNECTED_HOLD_STATE_LABEL_CTRL_ID,  //display sim card & call state
    MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID,   //display call name if saved in pb
    MMICC_CONNECTED_HOLD_NUMBER_LABEL_CTRL_ID, //display call time
    
    MMICC_MULTI_CALL_FORM_CTRL_ID,
    #ifdef MMI_CALLCONTROL_MINI_SUPPORT
    MMICC_MULTI_CALL_LINE_CTRL_ID,
    #endif
    MMICC_MULTI_ACTIVE_CALL_CHILD_FORM_CTRL_ID,
    MMICC_MULTI_ACTIVE_CALL_PHOTO_ANIM_CTRL_ID, 
    MMICC_MULTI_ACTIVE_CALL_CHILD_CHILD_FORM_CTRL_ID,
    MMICC_MULTI_ACTIVE_CALL_INFO_LABEL_CTRL_ID,
    MMICC_MULTI_ACTIVE_CALL_NAME_LABEL_CTRL_ID,    
    MMICC_MULTI_ACTIVE_CALL_TIME_LABEL_CTRL_ID,
    MMICC_MULTI_ACTIVE_CALL_LOCATION_LABEL_CTRL_ID,
    
    MMICC_MULTI_HOLD_CALL_CHILD_FORM_CTRL_ID,
    MMICC_MULTI_HOLD_CALL_PHOTO_ANIM_CTRL_ID,
    MMICC_MULTI_HOLD_CALL_CHILD_CHILD_FORM_CTRL_ID,
    MMICC_MULTI_HOLD_CALL_INFO_LABEL_CTRL_ID,
    MMICC_MULTI_HOLD_CALL_NAME_LABEL_CTRL_ID,
    MMICC_MULTI_HOLD_CALL_TIME_LABEL_CTRL_ID,
    MMICC_MULTI_HOLD_CALL_LOCATION_LABEL_CTRL_ID,

    MMICC_MULTI_SWAP_CALL_CHILD_FORM_CTRL_ID,
    MMICC_DISCONNECTED_PHOTO_ANIM_CTRL_ID, 
#ifdef MMI_CALLCONTROL_MINI_SUPPORT

    MMICC_BOTTOM_PHOTO_ANIM_CTRL_ID,
#endif  
    MMICC_CONNECTED_BUTTON_FORM_CTRL_ID,    
    MMICC_CONNECTED_BUTTON_CHILD_FORM_0_CRTL_ID,
    MMICC_CONNECTED_BUTTON_CHILD_FORM_1_CRTL_ID,

    MMICC_BUTTON_HANDFREE_CTRL_ID, //
    MMICC_BUTTON_DIALPANEL_CTRL_ID, 
    MMICC_BUTTON_MUTE_CTRL_ID,
    MMICC_BUTTON_RECORD_CTRL_ID,
    MMICC_BUTTON_HOLD_CTRL_ID,
    MMICC_BUTTON_PB_CTRL_ID,
    MMICC_BUTTON_SILENCE_CTRL_ID,
    MMICC_BUTTON_HANGUP_CTRL_ID,
    MMICC_BUTTON_ANSWER_CTRL_ID,
    MMICC_BUTTON_NEW_CALL_CTRL_ID,
    MMICC_BUTTON_MERGE_CALLS_CTRL_ID,
    MMICC_BUTTON_SWAP_CALLS_CTRL_ID,
    MMICC_BUTTON_RETRIVE_CALLS_CTRL_ID,
    MMICC_BUTTON_ASP_CTRL_ID,

    MMICC_BUTTON_PDA_HOLD_CALL_CTRL_ID,
    MMICC_BUTTON_PDA_ACTIVE_CALL_CTRL_ID,
    MMICC_BUTTON_PDA_SWAP_CALL_CTRL_ID, 
    MMICC_BUTTON_PDA_OPTION_CTRL_ID,

    MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID, 
    MMICC_BUTTON_PDA_END_CALL_CTRL_ID,
    
    MMICC_CONNECTED_PDA_HOLD_LABEL_CTRL_ID,
    MMICC_CONNECTED_PDA_ACTIVE_LABEL_CTRL_ID,
    MMICC_CONNECTED_PDA_SWAP_LABEL_CTRL_ID, 
    MMICC_CONNECTED_PDA_SIM_LABEL_CTRL_ID,
    MMICC_CONNECTED_PDA_OPTION_LABEL_CTRL_ID,

    MMICC_MT_LOCK_TIPS_LABEL_CTRL_ID,
    MMICC_MT_LOCK_ANSWER_LABEL_CTRL_ID,
    MMICC_MT_LOCK_REJECT_LABEL_CTRL_ID,
    MMICC_MT_LOCK_UNLOCK_LABEL_CTRL_ID,
//  processing & disconnected win
    MMICC_PROCESSING_FORM_CTRL_ID,
    MMICC_PROCESSING_TEXT_LABEL_0_CTRL_ID,
    MMICC_PROCESSING_TEXT_LABEL_1_CTRL_ID,
    MMICC_PROCESSING_ANIM_CTRL_ID,

#if defined(MMI_SPEEDDIAL_SUPPORT)
//speed dial	
    MMISPEEDDIAL_LISTBOX_CTRL_ID,      //  the list control of LIST Window
    MMISPEEDDIAL_POPMENU_CTRL_ID,
    MMISPEEDDIAL_EDITBOX_CTRL_ID,
    MMISPEEDDIAL_VIEW_RICHTEXT_CTRL_ID,
#endif

//cl
    WATCHCL_CALLLOG_LIST_CTRL_ID,

    MMICL_MAIN_MENU_CTRL_ID,                        // CALL LOG下的Menu控件
    MMICL_LOG_LIST_CTRL_ID,                         // CALL LOG下的已接，未接和已拨电话的list控件
    MMICL_DIALED_LIST_CTRL_ID,                         // CALL LOG下list控件
	MMICL_RECEIVED_LIST_CTRL_ID,                         // CALL LOG下list控件
	MMICL_MISSED_LIST_CTRL_ID,                         // CALL LOG下list控件
#ifdef BT_DIALER_SUPPORT
    MMICL_BT_DIALED_LIST_CTRL_ID,                      // CALL LOG下list控件
    MMICL_BT_RECEIVED_LIST_CTRL_ID,                    // CALL LOG下list控件
    MMICL_BT_MISSED_LIST_CTRL_ID,                      // CALL LOG下list控件
#ifdef MMI_PDA_SUPPORT
    MMICL_BT_ALL_LIST_CTRL_ID,
#endif
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
	MMICL_REFUSED_LIST_CTRL_ID,                         // CALL LOG下list控件
#endif
 
    MMICL_CALL_TIMES_MENU_CTRL_ID,  //CL 相同号码不同时间的现实
    MMICL_CALL_TIMES_DETAIL_CTRL_ID,
    MMICL_CALL_TIMES_LIST_OPT_CTRL_ID, //delete option

#ifdef MMI_PDA_SUPPORT
    MMICL_LOG_LIST_MENUKEY_CTRL_ID,
    MMICL_ALL_LIST_CTRL_ID,
	/*add for istyle test*/
    MMICALLLOG_FORM_CTRL_ID,
	MMICALLLOG_SOFTKEY_CTRL_ID,
#endif
    MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID,
    MMICL_ALL_LOG_LIST_DROPLIST_FORM_CTRL_ID,
    MMICL_ALL_LOG_LIST_FORM_CTRL_ID,
    MMICL_ALL_LOG_FORM_CTRL_ID,
    MMICL_TOOLBAR_ALLLOG_CTRL_ID,

    MMICL_CALL_TIMES_FORM_CTRL_ID,
    MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID,
    MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID,
    MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID,
    MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID,
    MMICL_CALL_TIMES_LIST_FORM_CTRL_ID,
    MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,
    MMICL_CALL_TIMES_BTN_CALL_CTRL_ID,
    MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID,
    MMICL_CALL_TIMES_BTN_MSG_CTRL_ID,
    MMICL_CALL_TIMES_BTN_DEL_CTRL_ID,


    MMICL_LOG_LIST_MENU_CTRL_ID,                    // CALL LOG下的已接，未接和已拨电话的list下一级的menu控件
    MMICL_REFUSED_LOG_LIST_MENU_CTRL_ID,            // CALL LOG下的拒接电话的list下一级的menu控件

	MMICL_LOG_TAB_CTRL_ID,		// used to show call log: dialed, received, missed, refused.

    MMICL_LOG_LIST_DETAIL_CTRL_ID,  // CALL LOG下的已接，未接和已拨电话的详情richtext控件
    
    MMICL_CALL_TIMER_CTRL_ID,   // CALL LOG下的记录通话时间的richtext控件

    MMICL_COSTS_MAX_EDIT_CTRL_ID,                   // 通话计费下的 ACM 话费编辑框 
    MMICL_COSTS_MAX_LABEL_CTRL_ID,                  // 通话计费下的 ACM 话费编辑框
    MMICL_COSTS_CURRENCY_EDIT_CTRL_ID,              // 通话计费下的 话费货币单位编辑框
    MMICL_COSTS_RATE_EDIT_CTRL_ID,                  // 通话计费下的 话费率编辑框

    MMICL_COSTS_CURRENCY_FORM_CTRL_ID,
    MMICL_COSTS_CURRENCY_LABEL_CTRL_ID,             // 通话计费下的 话费货币单位标签
    MMICL_COSTS_CURRENCY_VAL_LABEL_CTRL_ID,
    MMICL_COSTS_RATE_FORM_CTRL_ID,
    MMICL_COSTS_RATE_LABEL_CTRL_ID,                 // 通话计费下的 话费率标签
    MMICL_COSTS_RATE_VAL_LABEL_CTRL_ID,

    MMICL_CALL_COSTS_CTRL_ID,
    MMICL_CALL_COSTS_UPDATE_CTRL_ID,

    MMICC_RINGVOL_LEFT_BUTTON_CTRL_ID,              //通话音量调节，左按扭
    MMICC_RINGVOL_RIGHT_BUTTON_CTRL_ID,             //通话音量调节，右按扭

//#ifdef MMI_MULTI_SIM_SYS_DUAL
    MMICL_COM_SELECT_SIM_CTRL_ID,					 //sim选择
    MMICL_LOG_LIST_DETAIIL_SIM_HEAD_CTRL_ID,        //sim选择
    MMICL_LOG_LIST_DETAIIL_SIM_CTRL_ID,             //sim选择
    MMICC_COM_SELECT_SIM_CTRL_ID,                      //呼叫时选择sim卡
    MMICC_SPEED_DIAL_SIM_CTRL_ID,                      //Speed dial选择sim卡
    MMICL_TIMER_SIM2_LC_NAME_CTRL_ID,
    MMICL_TIMER_SIM2_LC_TIME_CTRL_ID,
    MMICL_TIMER_SIM2_RC_NAME_CTRL_ID,
    MMICL_TIMER_SIM2_RC_TIME_CTRL_ID,
    MMICL_TIMER_SIM2_DC_NAME_CTRL_ID,
    MMICL_TIMER_SIM2_DC_TIME_CTRL_ID,
    MMICL_TIMER_SIM2_AC_NAME_CTRL_ID,
    MMICL_TIMER_SIM2_AC_TIME_CTRL_ID,

//#endif
    MMICC_EMERGENCY_PHONENUM_CTRL_ID,               //紧急呼叫界面
#ifdef MV_SUPPORT
    MMICC_MAGIC_VOICE_MAIN_CTRL_ID,
    MMICC_MAGIC_VOICE_ITEM_CTRL_ID,
#endif
    MMICL_TOOLBAR_ALLLIST_CTRL_ID,
    MMICL_TOOLBAR_DIALED_CTRL_ID,
    MMICL_TOOLBAR_RECEIVED_CTRL_ID,
    MMICL_TOOLBAR_REFUSED_CTRL_ID,
    MMICL_TOOLBAR_MISSED_CTRL_ID,
    MMICL_TOOLBAR_SEARCH_MATCH_CTRL_ID,
#ifdef BT_DIALER_SUPPORT
    MMICL_TOOLBAR_BT_ALLLIST_CTRL_ID,
    MMICL_TOOLBAR_BT_DIALED_CTRL_ID,
    MMICL_TOOLBAR_BT_RECEIVED_CTRL_ID,
    MMICL_TOOLBAR_BT_MISSED_CTRL_ID,
#endif
#ifdef MMI_GPRS_SUPPORT
    MMICL_SELECT_SIM_FOR_GPRS_CTRL_ID,
    MMICL_GPRS_FLOWRATE_CTRL_ID,
#endif

    MMICL_CALL_TIME_POP_LIST_CTRL_ID,
    MMICL_PB_DETAIL_CALLLOG_LIST_CTRL_ID,
    MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID,
    MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,
    MMICL_ALL_LOG_PB_DETAIL_LIST_CTRL_ID,
    MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID,
    MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID,
    MMICL_PB_DETAIL_SELECT_TITLE_LABEL_CTRL_ID,
    MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID,
    MMICL_PB_DETAIL_SELECT_LIST_CTRL_ID,
    MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,

#ifdef CMCC_UI_STYLE
    MMICC_EMERGENCY_WIN_MENU_CTRL_ID, 
#endif
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
    MMICL_CHILD_SELECT_LIST_CTRL_ID,
#endif

#ifdef ADULT_WATCH_SUPPORT
#include "watch_dial_cl_contact_control_id.def"
#endif
    MMICC_MAX_CTRL_ID
}CC_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISET_ID_H_
