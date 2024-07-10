/*****************************************************************************
** File Name:      mmicc_menutable.h                                         *
** Author:         bruce.chi                                                 *
** Date:           2006/09/25                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe cc menu id                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/25     bruce.chi        Create                                    *
******************************************************************************/

#ifndef _MMICC_MENUTABLE_H_
#define _MMICC_MENUTABLE_H_

#include "mmi_module.h"
#include "mmicc_handfree.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/


//Menu ID
typedef enum
{
    ID_CC_MENU_START = (MMI_MODULE_CC << 16),
//cc
    // for cc menu
    ID_CC_HOLD_ANSWER,
    ID_CC_RELEASE_ANSWER,
    ID_CC_NEW_CALL,
    ID_CC_ECT,
    ID_CC_SHUTTLE,        
	ID_CC_RELEASE_CALL,   
	ID_CC_RELEASE_ACTIVE, 
	ID_CC_RELEASE_HOLD,   
	ID_CC_RELEASE_ALL,    
	ID_CC_BUILDMPTY,      
	ID_CC_SPLIT,          
	ID_CC_BUILDMPTY_ANSWER,
    ID_CC_HOLD,
    ID_CC_RECORD,
    ID_HANDFREE_SWITCH,
    ID_CC_HANDHOLD,
    ID_CC_BLUETOOTH,

 	//add for blue tooth send voice begin
	ID_CC_SEND_VOICE,
	ID_SEND_VOICE_TO_PHONE,
	ID_SEND_VOICE_TO_EARPHONE,
	//add for blue tooth send voice end   

    //for mtfw
    ID_MMIMTFW_ADD,
    ID_MMIMTFW_LIST,

	ID_MMIMTFW_DEL,
	ID_MMIMTFW_EDIT,

    ID_SPEEDDIAL_DIAL,
    ID_SPEEDDIAL_IPDIAL,
    ID_SPEEDDIAL_AUDIO_DIAL,
    ID_SPEEDDIAL_VIDEO_DIAL,
    ID_SPEEDDIAL_VIEW,
    ID_SPEEDDIAL_EDIT,
    ID_SPEEDDIAL_EDIT_CONTACT,
    ID_SPEEDDIAL_DEL,
    
//cl
    ID_CL_CALLS_ALL,
    ID_CL_MISSED_CALLS,
    ID_CL_DIALED_CALLS,
    ID_CL_RECEIVED_CALLS,
#if defined(MMI_BLACKLIST_SUPPORT)
    ID_CL_REFUSED_CALLS,
#endif
    ID_CL_CALLLOG_MENUS,
#ifdef ASP_SUPPORT
    ID_CL_ASP,
#endif
    ID_CL_DELETE_ALL_RECORDS,
    ID_CL_CALL_TIMERS,
    ID_CL_CALL_COSTS,                         //通话计费菜单项ID号
	ID_CL_GPRS_FLOWRATE,

    ID_CL_TIMER_LAST_CALL,
    ID_CL_TIMER_RECEIVED_CALLS,
    ID_CL_TIMER_DIALED_CALLS,
    ID_CL_TIMER_ALL_CALLS,
    ID_CL_TIMER_RESET,

	ID_CL_LOG_DETAIL,
    ID_CL_LOG_LIST_SAVE,
    ID_CL_LOG_LIST_DELETE,
    ID_CL_LOG_LIST_DELETE_MULTI,
//#ifdef VT_SUPPORT    
    //ID_CL_LOG_LIST_AUDIOCALL,//@zhaohui add,语音电话
    //ID_CL_LOG_LIST_VIDEOCALL,//@zhaohui add,可视电话
//#else
    ID_CL_LOG_LIST_DIAL,
//#endif
    ID_CL_ADD_TO_PB,
    ID_CL_ADD_TO_EXIST_CONTACT,
    ID_CL_LOG_LIST_SEND,
#if defined(MMI_BLACKLIST_SUPPORT)
    ID_CL_LOG_LIST_REFUSE,
    ID_CL_LOG_LIST_REFUSE_ADD,
    ID_CL_LOG_LIST_REFUSE_REMOVE,
#endif
    ID_CL_DELETE_ALL,

    //ID_CL_LOG_LIST_IPCALL,
    ID_CL_REFUSE_IPCALL,

    ID_CL_SEND_SMS,
    ID_CL_SEND_MMS,

    ID_CL_REFUSE_DELETE,
    ID_CL_REFUSE_CALL,
    ID_CL_REFUSE_SEND,
    ID_CL_REFUSE_DELETE_ALL,
    
    ID_CL_COSTS_TOTAL,                        //总通话费用菜单项ID号
    ID_CL_COSTS_MAX,                          //通话上限菜单项ID号
    ID_CL_COSTS_RATE,                         //通话费率菜单项ID号
    ID_CL_COSTS_RESET,                        //计费器清零菜单项ID号   

    //@fen.xie MS00172793:级联菜单
    ID_CL_DIAL_AUDIOCALL,//@zhaohui add,语音电话
    ID_CL_DIAL_VIDEOCALL,//@zhaohui add,可视电话
    ID_CL_DIAL_IPCALL,
    ID_CL_DIAL_CALL,
    ID_CL_DELETE,
    ID_CL_EDIT_BEFORE_DIAL,
    ID_CL_CONTACT_VIEW,
    ID_CL_MARK,
#ifdef MMI_CL_MINI_SUPPORT
    ID_CL_LOG_LIST_MARK,   //主菜单的ID
    ID_CL_MARK_LIST,
    ID_CL_CANCEL_MARK_LIST,
    ID_CL_MARKALL_LIST,
    ID_CL_CANCLE_MARKALL_LIST,
#endif
#ifdef BT_DIALER_SUPPORT
	ID_CL_DIAL_SIMCALL,
	ID_CL_DIAL_BTCALL,
    ID_CL_LOG_SYNC,
#endif
	ID_CC_MENU_MAX
}MMICC_ID_MENU_E;

#ifdef MMI_ENABLE_HANDFREE
#define ONEITEM_MMI_ENABLE_HANDFREE 1
#else
#define ONEITEM_MMI_ENABLE_HANDFREE 0
#endif


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMICC_MENU_LABEL_START = (MMI_MODULE_CC << 16),

    #include "mmicc_menutable.def"

    MMICC_MENUTABLE_MAX
} MMICC_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_CC_MENUTABLE_H_

