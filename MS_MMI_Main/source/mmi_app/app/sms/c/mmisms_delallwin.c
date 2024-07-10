/****************************************************************************
** File Name:      mmisms_delallwin.c                                          *
** Author:         liming deng                                                        *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        minghu.mao       Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#if defined(MMI_SMS_DELALL_SUPPORT)
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmipub.h"
#include "guitab.h"

#include "mmisms_edit.h"
#include "mmisms_delete.h"
#include "mmisms_send.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmimms_id.h"
#include "mmimms_text.h"
#include "mmiota_export.h"
#include "mmiota_id.h"
#include "mmisms_image.h"
#include "mmisms_order.h"
#include "mmisms_save.h"
#include "mmisms_set.h"
#include "mmisms_export.h"
#include "guictrl_api.h"
#include "mmimms_export.h"
#include "mmiudisk_export.h"
#include "mmisms_securitywin.h"
#include "mmisms_commonui.h"
#include "mmisms_appcontrol.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#define MMISMS_DELMSG_BOX_ITEM_NUM      6
#else
#define MMISMS_DELMSG_BOX_ITEM_NUM      5
#endif

#define MMISMS_DELMSG_TYPE_ITEM_NUM     4
#define MMISMS_DELMSG_MSGNUM_STR_LEN    10 
#define MMISMS_ONE_K_LEN                1024
#define MMISMS_SIZE_FLOAT_A             0.100000
#define MMISMS_SIZE_FLOAT_B             0.000000
#define MMISMS_MEMSTATE_DELMSG_MAX_LEN  30

typedef enum
{
     MMISMS_DELMSG_CHILD_MSGBOX,
     MMISMS_DELMSG_CHILD_MSGTYPE,
     MMISMS_DELMSG_CHILD_SIM,     
     MMISMS_DELMSG_CHILD_MAX
} MMISMS_DELMSG_CHILD_TYPE_E;

typedef enum
{
     MMISMS_DELMSG_INBOX,
     MMISMS_DELMSG_SENDSUCCBOX,
     MMISMS_DELMSG_OUTBOX,
     MMISMS_DELMSG_DRAFTBOX,
     MMISMS_DELMSG_SECURITYBOX,
     MMISMS_DELMSG_BOX_ALL,
     MMISMS_DELMSG_BOX_MAX
} MMISMS_DELMSG_BOX_TYPE_E;

typedef enum
{
     MMISMS_DELMSG_TYPE_SMS,
     MMISMS_DELMSG_TYPE_MMS,
     MMISMS_DELMSG_TYPE_OTAPUSH,
     MMISMS_DELMSG_TYPE_WAPPUSH,
     MMISMS_DELMSG_TYPE_MAX
} MMISMS_DELMSG_MSGTYPE_TYPE_E;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    MMISMS_DELMSG_CHILD_TYPE_E child_type; 
    MMISMS_DELMSG_BOX_TYPE_E box_type;
    MMISMS_DELMSG_MSGTYPE_TYPE_E msg_type;
}MMISMS_DELMSG_GLOBLE_T;

typedef struct
{
    MMISMS_DELMSG_GLOBLE_T sms_delmsg_all_info;
    uint16 all_del_msg_num;
    BOOLEAN mms_is_sendrecv;
    BOOLEAN is_mms_del;
} MMISMS_DELALLMSG_WIN_PARAM_T;

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle the message of message del all box window 
//  Global resource dependence : 
//  Author:liming.deng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgDelAllWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description : to handle the message of message del all child box window 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBoxDelChildWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description : to handle the message of message del all child box window 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgTypeDelChildWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//     Description : add message delete all list items
//    Global resource dependence : 
//  Author: minghu.mao
//    Note: 
/*****************************************************************************/
LOCAL void  AddMessageDelAllChildLists(BOOLEAN is_replace);

/*****************************************************************************/
//  Description : add box delete all list items.
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  AddBoxDelAllChildLists(BOOLEAN is_replace);

/*****************************************************************************/
//  Description : add box delete all list items data.
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetBoxDelListItemData(MMISMS_DELMSG_BOX_TYPE_E box_type);

/*****************************************************************************/
//  Description : get box number
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void GetBoxDelNumString(
                              MMI_STRING_T        *string_ptr,    //OUT:
                              MMISMS_BOX_TYPE_E box_type        //IN:
                              );

/*****************************************************************************/
//  Description : add msg type delete all list items data.
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMsgTypeListItemData(MMISMS_DELMSG_MSGTYPE_TYPE_E msg_type);

/*****************************************************************************/
//  Description : get msg type number
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void GetMsgTypeNumString(
                               MMI_STRING_T        *string_ptr,        //OUT:
                               MMISMS_DELMSG_MSGTYPE_TYPE_E msg_type,  //IN:
                               uint16 *msg_num_ptr                     //OUT
                               );

/*****************************************************************************/
//     Description : add all tabs and all child windows to parent window.
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL void AddDelAllMsgChildWindows(void);

/*****************************************************************************/
//  Description :security pwd window call back
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleDelMsgSecurityPWDResult(uint32 validate_result);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jixin.xu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelAllWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE(MMISMS_MSGDEL_ALL_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleMsgDelAllWindow),    
    WIN_ID( MMISMS_MSGDEL_ALL_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_CLEAR, TXT_NULL, STXT_RETURN ),
#ifdef MMI_GUI_STYLE_TYPICAL
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMISMS_MSGDEL_ALL_CTRL_ID),
#else
	CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMISMS_MSGDEL_ALL_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISMS_BOXDEL_ALL_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleBoxDelChildWinMsg),    
    WIN_ID( MMISMS_BOXDEL_ALL_CHILD_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_CLEAR, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_BOXDEL_ALL_CHILD_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_TAB ) = 
{
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT  
    WIN_TITLE( TXT_SMS_MSG_DELALL ),
#endif
    WIN_FUNC((uint32)HandleMsgTypeDelChildWinMsg),    
    WIN_ID( MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_CLEAR, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_MSGTYPEDEL_ALL_CHILD_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle tab del all msg window message
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgDelAllWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_DELALLMSG_WIN_PARAM_T * param_to_create_delallmsg_win = (MMISMS_DELALLMSG_WIN_PARAM_T *)MMK_GetWinUserData(win_id);

    if (PNULL == param_to_create_delallmsg_win)
    {
        return MMI_RESULT_TRUE;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISMS_MSGDEL_ALL_CTRL_ID);
        AddDelAllMsgChildWindows();
        param_to_create_delallmsg_win->sms_delmsg_all_info.child_type = MMISMS_DELMSG_CHILD_MSGBOX; 
        break;

    case MSG_NOTIFY_TAB_SWITCH://fullpait
        if(0 == GUITAB_GetCurSel(MMISMS_MSGDEL_ALL_CTRL_ID))
        {
            param_to_create_delallmsg_win->sms_delmsg_all_info.child_type = MMISMS_DELMSG_CHILD_MSGBOX; 
        }   
        else
        {
            param_to_create_delallmsg_win->sms_delmsg_all_info.child_type = MMISMS_DELMSG_CHILD_MSGTYPE; 
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        if(0 == GUITAB_GetCurSel(MMISMS_MSGDEL_ALL_CTRL_ID))
        {
            MMK_PostMsg(MMISMS_BOXDEL_ALL_CHILD_WIN_ID, MSG_PROMPTWIN_OK, PNULL, 0);
        }   
        else
        {
            MMK_PostMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MSG_PROMPTWIN_OK, PNULL, 0);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        if(0 == GUITAB_GetCurSel(MMISMS_MSGDEL_ALL_CTRL_ID))
        {
            MMK_PostMsg(MMISMS_BOXDEL_ALL_CHILD_WIN_ID, MSG_PROMPTWIN_CANCEL, PNULL, 0);
        }   
        else
        {
            MMK_PostMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MSG_PROMPTWIN_CANCEL, PNULL, 0);
        }
        break;
    case MMISMS_MSG_UPDATELIST:
        MMK_PostMsg(MMISMS_BOXDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL, 0);
        MMK_PostMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL, 0);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(win_id);            
        break;
        
    case MSG_CLOSE_WINDOW:
        DelAllWinCloseCtrlFunc(win_id);
        break; 
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : add all tabs and all child windows to parent window.
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AddDelAllMsgChildWindows(void)
{
    MMI_STRING_T str_text = {0};

    MMI_GetLabelTextByLang(TXT_SMS_MSG_DEL_CLEARBOX , &str_text);
    GUITAB_SetMaxItemNum(MMISMS_MSGDEL_ALL_CTRL_ID, 2);

    GUITAB_AppendSimpleItem(
        MMISMS_MSGDEL_ALL_CTRL_ID,
        &str_text,
        IMAGE_SMS_INBOX_TAB_ON_ICON,
        IMAGE_SMS_INBOX_TAB_OFF_ICON);
    GUITAB_AddChildWindow(
        MMISMS_MSGDEL_ALL_CTRL_ID,
        MMISMS_BOXDEL_ALL_CHILD_WIN_ID, 
        0);

    MMI_GetLabelTextByLang(TXT_SMS_MSG_DEL_CLEARMSG, &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_MSGDEL_ALL_CTRL_ID,
        &str_text,
        IMAGE_SMS_SENDFAILBOX_TAB_ON_ICON,
        IMAGE_SMS_SENDFAILBOX_TAB_OFF_ICON);

    GUITAB_AddChildWindow(
        MMISMS_MSGDEL_ALL_CTRL_ID,
        MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID,
        1);

    GUITAB_SetCurSel(MMISMS_MSGDEL_ALL_CTRL_ID, 0);
}


/*****************************************************************************/
//     Description : handle delete finish call back
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MessageBoxDelFinishCallback(void *user_data,MMISMS_DELETE_DATE_INFO *data_info_ptr) 
{
    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
    MMK_CloseWin(MMICMSBRW_NEWPUSH_WIN_ID);
	MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
    
    MMISMS_SetDelAllFlag(FALSE);
    if (data_info_ptr->is_mms_del)
    {//Save mms file 
        MMIAPIMMS_SaveListInfo();
    }            
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    //提示删除了多少信息
    if(0 == data_info_ptr->all_del_msg_num)
    {
        if(data_info_ptr->mms_is_sendrecv)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SMS_MSG_DEL_MMSDOWNLOAD);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
        }
    }
            
    //根据type发送消息刷新
    MMK_PostMsg(MMISMS_BOXDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL,0);
    MMK_PostMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL,0);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    MAIN_SetMsgState(TRUE);
    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleBoxDelDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMISMS_BOXDEL_ALL_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms chatbox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {  
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }    
}

/*****************************************************************************/
//  Description : to handle tab del all msg child window message
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxDelChildWinMsg(
                                           MMI_WIN_ID_T        win_id,        //IN:
                                           MMI_MESSAGE_ID_E    msg_id,        //IN:
                                           DPARAM            param        //IN:
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_BOXDEL_ALL_CHILD_CTRL_ID;
    uint16 selected_index = 0;
    uint16 msg_num = 0;
    uint16 index = 0;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_DELALLMSG_WIN_PARAM_T *param_to_create_delallmsg_win = (MMISMS_DELALLMSG_WIN_PARAM_T *)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELALLWIN_387_112_2_18_2_52_31_14,(uint8*)"d", msg_id);
    if (PNULL == param_to_create_delallmsg_win)
    {
        return MMI_RESULT_TRUE;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(ctrl_id,&list_both_rect);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        GUILIST_SetMaxItem(ctrl_id, MMISMS_DELMSG_BOX_ITEM_NUM, TRUE);
        // load items to list box
        AddBoxDelAllChildLists(FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleBoxDelDateChangeCallBack, TRUE); 
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_CLEAR, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:HandleBoxDelChildWinMsg need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELALLWIN_407_112_2_18_2_52_31_15,(uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            SetBoxDelListItemData((MMISMS_DELMSG_BOX_TYPE_E)index);
        }
        break; 

    case MSG_GET_FOCUS:    
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        param_to_create_delallmsg_win->sms_delmsg_all_info.box_type = (MMISMS_DELMSG_BOX_TYPE_E)selected_index;
        break;

    case MSG_FULL_PAINT:
        if(MMK_GetWinDirty(win_id))
        {
            AddBoxDelAllChildLists(TRUE);    
            MMK_SetWinDirty(win_id, FALSE);            
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT    
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T prompt_str   = {0};
            uint16 security_num = 0;

            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            param_to_create_delallmsg_win->sms_delmsg_all_info.box_type = (MMISMS_DELMSG_BOX_TYPE_E)selected_index;
            msg_num = MMISMS_GetBoxTotalNumberFromOrder(selected_index + 1);
            security_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_DELMSG_SECURITYBOX + 1);
            if (0 < msg_num)
            {
                MMI_GetLabelTextByLang(TXT_DELALL, &prompt_str);
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                if (MMISMS_DELMSG_SECURITYBOX == selected_index || 
                    (MMISMS_DELMSG_BOX_ALL == selected_index && 0 < security_num))
                {//安全信箱输入密码
                    MMIAPISMS_OpenValidateSecruityBox(HandleDelMsgSecurityPWDResult);
                }
                else
#endif
                {
                    MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
                }
            }            
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        //打开一个删除的等待窗口（使用sms的删除窗口id），然后一个一个删除，每次删除一个，知道删除完成，
        //如果是短消息使用cnf删除，彩信的话使用消息传递，同时这个地方启动删除第一条
        //OpenDelAllWaitingWin(MMISMS_DELMSG_CHILD_MSGBOX, param_to_create_delallmsg_win);
        {
        MMISMS_OPERATE_DATA_INFO_T data_info = {0};
        data_info.is_del_all = TRUE;
        data_info.box_type = param_to_create_delallmsg_win->sms_delmsg_all_info.box_type + 1;
        MMISMS_EnterDelWaitWin(&data_info,MMISMS_MessageBoxDelFinishCallback,0);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMISMS_MSGDEL_ALL_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleBoxDelDateChangeCallBack, FALSE); 
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description :security pwd window call back
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleDelMsgSecurityPWDResult(uint32 validate_result)	
{	
    if (validate_result)		//If check is ok
    {	
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        if (MMK_IsOpenWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID))
#else
            if (MMK_IsOpenWin(MMISMS_MSGDEL_ALL_WIN_ID))
#endif 
            {
                MMI_STRING_T prompt_str   = {0};
                MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
                
                MMI_GetLabelTextByLang(TXT_DELALL, &prompt_str);
                MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
            }
    }
    else
    {
		//do nothing is ok
	}
	return TRUE;
	
}  //end of HandleSecurityPWDResult



/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgTypeDelDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms chatbox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {  
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }    
}

/*****************************************************************************/
//  Description : to handle tab del all msg child window message
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgTypeDelChildWinMsg(
                                               MMI_WIN_ID_T        win_id,        //IN:
                                               MMI_MESSAGE_ID_E    msg_id,        //IN:
                                               DPARAM            param        //IN:
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_MSGTYPEDEL_ALL_CHILD_CTRL_ID;
    uint16 selected_index = 0;
    uint16 msg_num = 0;
    uint16 index = 0;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    MMISMS_DELALLMSG_WIN_PARAM_T * param_to_create_delallmsg_win = (MMISMS_DELALLMSG_WIN_PARAM_T *)MMK_GetWinUserData(win_id);
#else
    MMISMS_DELALLMSG_WIN_PARAM_T *param_to_create_delallmsg_win = (MMISMS_DELALLMSG_WIN_PARAM_T *)MMK_GetWinAddDataPtr(win_id);
#endif
    if (PNULL == param_to_create_delallmsg_win)
    {
        return recode;
    }
    //SCI_TRACE_LOW:"MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELALLWIN_538_112_2_18_2_52_31_16,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(ctrl_id,&list_both_rect);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        GUILIST_SetMaxItem(ctrl_id, MMISMS_DELMSG_TYPE_ITEM_NUM, TRUE );//max item 
        // load items to list box
        AddMessageDelAllChildLists(FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMsgTypeDelDateChangeCallBack, TRUE); 
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_CLEAR, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:HandleBoxDelChildWinMsg need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELALLWIN_558_112_2_18_2_52_32_17,(uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            SetMsgTypeListItemData((MMISMS_DELMSG_MSGTYPE_TYPE_E)index);
        }
        break;
        
    case MSG_LOSE_FOCUS:
        break;

    case MSG_GET_FOCUS:
        //GUILIST_ValidCurItemPos(ctrl_id);
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        param_to_create_delallmsg_win->sms_delmsg_all_info.msg_type = (MMISMS_DELMSG_MSGTYPE_TYPE_E)selected_index;
        break;

    case MSG_FULL_PAINT:
        if(MMK_GetWinDirty(win_id))
        { 
            AddMessageDelAllChildLists(TRUE);
            MMK_SetWinDirty(win_id,FALSE);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif    
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB: 
        {
            MMI_STRING_T prompt_str   = {0};

            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            param_to_create_delallmsg_win->sms_delmsg_all_info.msg_type = (MMISMS_DELMSG_MSGTYPE_TYPE_E)selected_index;
            GetMsgTypeNumString(PNULL, (MMISMS_DELMSG_MSGTYPE_TYPE_E)selected_index, &msg_num);
            if(0 < msg_num)
            {
                MMI_GetLabelTextByLang(TXT_DELALL, &prompt_str);
                MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
            }            
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        //OpenDelAllWaitingWin(MMISMS_DELMSG_CHILD_MSGTYPE, param_to_create_delallmsg_win);
        {
            MMISMS_OPERATE_DATA_INFO_T data_info = {0};
            data_info.is_del_all = TRUE;
            data_info.is_del_msg_type = TRUE;
            if(MMISMS_DELMSG_TYPE_SMS == param_to_create_delallmsg_win->sms_delmsg_all_info.msg_type)
            {
                data_info.msg_type = MMISMS_TYPE_SMS;
            }
            else if(MMISMS_DELMSG_TYPE_MMS == param_to_create_delallmsg_win->sms_delmsg_all_info.msg_type)
            {
                data_info.msg_type = MMISMS_TYPE_MMS;
            }
            else if(MMISMS_DELMSG_TYPE_WAPPUSH == param_to_create_delallmsg_win->sms_delmsg_all_info.msg_type)
            {
                data_info.msg_type = MMISMS_TYPE_WAP_PUSH;
            }
            else
            {
                data_info.msg_type = MMISMS_TYPE_MMS_OTA;
            }
            //data_info.box_type = param_to_create_delallmsg_win->sms_delmsg_all_info.box_type + 1;
            MMISMS_EnterDelWaitWin(&data_info,MMISMS_MessageBoxDelFinishCallback,0);
        }
        
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT      
        MMK_CloseParentWin(MMISMS_MSGDEL_ALL_WIN_ID);
#endif        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        DelAllWinCloseCtrlFunc(win_id);
#endif
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMsgTypeDelDateChangeCallBack, FALSE); 
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : add box delete all list items.
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  AddBoxDelAllChildLists(BOOLEAN is_replace)
{
    GUILIST_ITEM_T list_item_t    = {0};/*lint !e64*/
    uint16 index                  = 0;
    MMI_CTRL_ID_T ctrl_id         = MMISMS_BOXDEL_ALL_CHILD_CTRL_ID;
#ifndef MMI_GUI_STYLE_TYPICAL
    list_item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
#else    
    list_item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;
#endif    
    for (index = 0; index < MMISMS_DELMSG_BOX_ITEM_NUM; index++)
    {
        if (is_replace)
        {
            GUILIST_ReplaceItem(ctrl_id, &list_item_t, index);
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &list_item_t);
        }        
    }
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : add box delete all list items data.
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetBoxDelListItemData(MMISMS_DELMSG_BOX_TYPE_E box_type)
{
    MMI_STRING_T box_name_string = {0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_CTRL_ID_T ctrl_id  = MMISMS_BOXDEL_ALL_CHILD_CTRL_ID;
    wchar number_wch[MMISMS_MEMSTATE_DELMSG_MAX_LEN + 1] = {0};
    MMI_STRING_T num_string = {0};

    num_string.wstr_ptr = number_wch;    
    switch(box_type) 
    {
    case MMISMS_DELMSG_INBOX:
        MMI_GetLabelTextByLang(TXT_INBOX, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_MT);
        break;
    case MMISMS_DELMSG_OUTBOX:
        MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_SENDFAIL);
        break;
    case MMISMS_DELMSG_DRAFTBOX:
        MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_NOSEND);
        break;
    case MMISMS_DELMSG_SENDSUCCBOX:
        MMI_GetLabelTextByLang(TXT_SMS_SENTBOX, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_SENDSUCC);
        break;

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_DELMSG_SECURITYBOX:
        MMI_GetLabelTextByLang(TXT_SMS_SECURITY, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_SECURITY);
        break;
#endif

    case MMISMS_DELMSG_BOX_ALL:
        MMI_GetLabelTextByLang(TXT_SMS_MSG_DEL_ALLBOXMSG, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_ALL);
        break;
    default:
        return;
    }

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = box_name_string.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = box_name_string.wstr_ptr;

    if (MMISMS_DELMSG_SECURITYBOX != box_type)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = num_string.wstr_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = num_string.wstr_ptr;
    }

    GUILIST_SetItemData(ctrl_id,&item_data, box_type);   
}

#else
/*****************************************************************************/
//  Description : add box delete all list items data.
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetBoxDelListItemData(MMISMS_DELMSG_BOX_TYPE_E box_type)
{
    MMI_STRING_T box_name_string = {0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_CTRL_ID_T ctrl_id  = MMISMS_BOXDEL_ALL_CHILD_CTRL_ID;
    wchar number_wch[MMISMS_MEMSTATE_DELMSG_MAX_LEN + 1] = {0};
    MMI_STRING_T num_string = {0};
    uint16 index = 0;
    uint16 msg_num = 0;   

    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    num_string.wstr_ptr = number_wch;    
    switch(box_type) 
    {
    case MMISMS_DELMSG_INBOX:
        MMI_GetLabelTextByLang(TXT_INBOX, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_MT);
        msg_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_MT);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SECMENU_ICON_MESSAGE_INBOX;
        index++;
        break;
    case MMISMS_DELMSG_OUTBOX:
        MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_SENDFAIL);
        msg_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SENDFAIL);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SECMENU_ICON_MESSAGE_OUTBOX;
        index++;
        break;
    case MMISMS_DELMSG_DRAFTBOX:
        MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_NOSEND);
        msg_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_NOSEND);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SECMENU_ICON_MESSAGE_DRAFTBOX;
        index++;
        break;
    case MMISMS_DELMSG_SENDSUCCBOX:
        MMI_GetLabelTextByLang(TXT_SMS_SENTBOX, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_SENDSUCC);
        msg_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SENDSUCC);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SECMENU_ICON_MESSAGE_SENT;
        index++;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_DELMSG_SECURITYBOX:
        MMI_GetLabelTextByLang(TXT_SMS_SECURITY, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_SECURITY);
        msg_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SECURITY);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SECMENU_ICON_MESSAGE_SECURITY;
        index++;
        break;
#endif
    case MMISMS_DELMSG_BOX_ALL:
        MMI_GetLabelTextByLang(TXT_SMS_MSG_DEL_ALLBOXMSG, &box_name_string);
        GetBoxDelNumString(&num_string, MMISMS_BOX_ALL);
        msg_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_ALL);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = (MMI_IMAGE_ID_T)IMAGE_SECMENU_ICON_MESSAGE_INBOX;
        index++;
        break;
    default:
        return;
    }
    if (0 < msg_num)
    {
        item_data.softkey_id[0] = TXT_COMMON_CLEAR;
    }
    else
    {
        item_data.softkey_id[0] = TXT_NULL;
    }
    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[index].item_data.text_buffer.wstr_len = box_name_string.wstr_len;
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = box_name_string.wstr_ptr;
    index++;

    if (MMISMS_DELMSG_SECURITYBOX != box_type)
    {
        item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[index].item_data.text_buffer.wstr_len = num_string.wstr_len;
        item_data.item_content[index].item_data.text_buffer.wstr_ptr = num_string.wstr_ptr;
        index++;
    }

    GUILIST_SetItemData(ctrl_id,&item_data, box_type);   
}
#endif
/*****************************************************************************/
//  Description : get box number
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void GetBoxDelNumString(
                              MMI_STRING_T        *string_ptr,    //OUT:
                              MMISMS_BOX_TYPE_E box_type          //IN:
                              )
{
    uint16 total_num     = 0;
    uint16 unread_num    = 0;
    uint8 num_state[MMISMS_DELMSG_MSGNUM_STR_LEN] = {0};

    if ( MMISMS_BOX_MT == box_type)
    {
        total_num = MMISMS_GetBoxTotalNumberFromOrder(box_type);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        unread_num = MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_MAX) ;
#else
        unread_num = MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_1);
#endif
        unread_num += MMISMS_GetUndownloadMMSNumInOrder();

        string_ptr->wstr_len = sprintf((char *)num_state, "%d/%d", unread_num, total_num);
    }
    else
    {
        total_num = MMISMS_GetBoxTotalNumberFromOrder(box_type);
        string_ptr->wstr_len = sprintf((char *)num_state, "%d", total_num);
    }
    MMI_STRNTOWSTR(string_ptr->wstr_ptr, string_ptr->wstr_len, (const uint8 *)num_state, string_ptr->wstr_len, string_ptr->wstr_len);
}



#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : add message delete all list items
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void  AddMessageDelAllChildLists(BOOLEAN is_replace)
{
    GUILIST_ITEM_T list_item_t    = {0};/*lint !e64*/
    uint16 index                  = 0;
    MMI_CTRL_ID_T ctrl_id         = MMISMS_MSGTYPEDEL_ALL_CHILD_CTRL_ID;

    list_item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    for (index = 0; index < MMISMS_DELMSG_TYPE_ITEM_NUM; index++)
    {
        if (is_replace)
        {
            GUILIST_ReplaceItem(ctrl_id, &list_item_t, index);
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &list_item_t);
        }        
    }
}
#else
/*****************************************************************************/
//  Description : add message delete all list items
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  AddMessageDelAllChildLists(BOOLEAN is_replace)
{
    GUILIST_ITEM_T list_item_t    = {0};/*lint !e64*/
    uint16 index                  = 0;
    MMI_CTRL_ID_T ctrl_id         = MMISMS_MSGTYPEDEL_ALL_CHILD_CTRL_ID;

    list_item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;
    for (index = 0; index < MMISMS_DELMSG_TYPE_ITEM_NUM; index++)
    {
        if (is_replace)
        {
            GUILIST_ReplaceItem(ctrl_id, &list_item_t, index);
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &list_item_t);
        }        
    }
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : add msg type delete all list items data.
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMsgTypeListItemData(MMISMS_DELMSG_MSGTYPE_TYPE_E msg_type)
{
    MMI_STRING_T type_name_string = {0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_CTRL_ID_T ctrl_id         = MMISMS_MSGTYPEDEL_ALL_CHILD_CTRL_ID;
    wchar number_wch[MMISMS_MEMSTATE_DELMSG_MAX_LEN + 1] = {0};
    MMI_STRING_T num_string = {0};
    uint16 msg_num = 0;

    //SCI_TRACE_LOW:"SetMsgTypeListItemData msg_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELALLWIN_917_112_2_18_2_52_33_18,(uint8*)"d", msg_type);
    num_string.wstr_ptr = number_wch;
    switch(msg_type) 
    {
    case MMISMS_DELMSG_TYPE_SMS:
        MMI_GetLabelTextByLang(TXT_SMS, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_SMS, &msg_num);
        break;
    case MMISMS_DELMSG_TYPE_MMS:
        MMI_GetLabelTextByLang(TXT_MMS, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_MMS, &msg_num);
        break;
    case MMISMS_DELMSG_TYPE_OTAPUSH:
        MMI_GetLabelTextByLang(TXT_COMM_PUSH_MSG, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_OTAPUSH, &msg_num);
        break;
    case MMISMS_DELMSG_TYPE_WAPPUSH:
        MMI_GetLabelTextByLang(TXT_SMS_MSG_DEL_OTA_MSG, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_WAPPUSH, &msg_num);
        break;
    default:
        return;
    }

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = type_name_string.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = type_name_string.wstr_ptr;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = num_string.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = num_string.wstr_ptr;

    GUILIST_SetItemData(ctrl_id,&item_data, msg_type);
}


#else
/*****************************************************************************/
//  Description : add msg type delete all list items data.
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMsgTypeListItemData(MMISMS_DELMSG_MSGTYPE_TYPE_E msg_type)
{
    MMI_STRING_T type_name_string = {0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_CTRL_ID_T ctrl_id         = MMISMS_MSGTYPEDEL_ALL_CHILD_CTRL_ID;
    wchar number_wch[MMISMS_MEMSTATE_DELMSG_MAX_LEN + 1] = {0};
    MMI_STRING_T num_string = {0};
    uint16 index = 0;
    uint16 msg_num = 0;

    //SCI_TRACE_LOW:"SetMsgTypeListItemData msg_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELALLWIN_968_112_2_18_2_52_33_19,(uint8*)"d", msg_type);
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    num_string.wstr_ptr = number_wch;
    switch(msg_type) 
    {
    case MMISMS_DELMSG_TYPE_SMS:
        MMI_GetLabelTextByLang(TXT_SMS, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_SMS, &msg_num);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SMS_READ_SMS_ICON;
        index++;
        break;
    case MMISMS_DELMSG_TYPE_MMS:
        MMI_GetLabelTextByLang(TXT_MMS, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_MMS, &msg_num);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SMS_READ_MMS_ICON;
        index++;
        break;
    case MMISMS_DELMSG_TYPE_OTAPUSH:
        MMI_GetLabelTextByLang(TXT_COMM_PUSH_MSG, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_OTAPUSH, &msg_num);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SMS_WAPPUSH_READED_ICON;
        index++;
        break;
    case MMISMS_DELMSG_TYPE_WAPPUSH:
        MMI_GetLabelTextByLang(TXT_SMS_MSG_DEL_OTA_MSG, &type_name_string);
        GetMsgTypeNumString(&num_string, MMISMS_DELMSG_TYPE_WAPPUSH, &msg_num);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[index].item_data.image_id = IMAGE_SMS_WAPOTA_READED_ICON;
        index++;
        break;
    default:
        return;
    }
    if (0 < msg_num)
    {
        item_data.softkey_id[0] = TXT_COMMON_CLEAR;
    }
    else
    {
        item_data.softkey_id[0] = TXT_NULL;
    }
    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[index].item_data.text_buffer.wstr_len = type_name_string.wstr_len;
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = type_name_string.wstr_ptr;
    index++;

    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[index].item_data.text_buffer.wstr_len = num_string.wstr_len;
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = num_string.wstr_ptr;
    index++;

    GUILIST_SetItemData(ctrl_id,&item_data, msg_type);
}
#endif
/*****************************************************************************/
//  Description : get msg type number
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void GetMsgTypeNumString(
                               MMI_STRING_T        *string_ptr,        //OUT:
                               MMISMS_DELMSG_MSGTYPE_TYPE_E msg_type,  //IN:
                               uint16 *msg_num_ptr                     //OUT
                               )
{
    uint32 msg_num = 0;
    uint8 num_ch[MMISMS_DELMSG_MSGNUM_STR_LEN] = {0};
    uint32 mms_used_size = 0;
    uint32 mms_total_size = 0;
    wchar mms_size_wch[MMISMS_MEMSTATE_DELMSG_MAX_LEN + 1] = {0};
    uint8 mms_size_ch[MMISMS_MEMSTATE_DELMSG_MAX_LEN + 1] = {0};
    
    if (PNULL == msg_num_ptr)
    {
        return;
    }
    //SCI_TRACE_LOW:"GetMsgTypeNumString msg_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELALLWIN_1047_112_2_18_2_52_33_20,(uint8*)"d", msg_type);
    switch(msg_type) 
    {
    case MMISMS_DELMSG_TYPE_SMS:
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        msg_num = (uint32)MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_ALL) 
            - (MMIAPIMMS_GetMMSTotalNumber() - (uint32)MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_MMS)) 
#ifdef MMI_OTA_SUPPORT
	     - ((uint32)MMIAPIOTA_GetPushTotalNum() 
#endif
            - (uint32)MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_WAP_PUSH)          
#ifdef MMI_OTA_SUPPORT
		) 
            - ((uint32)MMIAPIOTA_GetOTATotalNum() 
#endif
            - (uint32)MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_WAP_OTA)
#ifdef MMI_OTA_SUPPORT
	)
#endif
            - (uint32)MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SECURITY);

#else
        msg_num = (uint32)MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_ALL) 
            - (MMIAPIMMS_GetMMSTotalNumber() - (uint32)MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_MMS)) 
#ifdef MMI_OTA_SUPPORT
	     - ((uint32)MMIAPIOTA_GetPushTotalNum() 
#endif
            - (uint32)MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_WAP_PUSH)
#ifdef MMI_OTA_SUPPORT
		) 
            - ((uint32)MMIAPIOTA_GetOTATotalNum() 
#endif
            - (uint32)MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_WAP_OTA)
#ifdef MMI_OTA_SUPPORT
            );
#else
	      ;
#endif
#endif
        break;
    case MMISMS_DELMSG_TYPE_MMS:
        msg_num = MMIAPIMMS_GetMMSTotalNumber() - MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_MMS);
        MMIAPIMMS_GetTotalSizeAndAllSize(&mms_used_size, &mms_total_size);
        _snprintf((void *)mms_size_ch, MMISMS_MEMSTATE_DELMSG_MAX_LEN ,"/%.1fM",(float)mms_used_size/(MMISMS_ONE_K_LEN * MMISMS_ONE_K_LEN));
        if (((float)mms_used_size/(MMISMS_ONE_K_LEN * MMISMS_ONE_K_LEN)) < MMISMS_SIZE_FLOAT_A 
            && ((float)mms_used_size/(MMISMS_ONE_K_LEN * MMISMS_ONE_K_LEN)) > MMISMS_SIZE_FLOAT_B)
        {
            _snprintf((void*)mms_size_ch, MMISMS_MEMSTATE_DELMSG_MAX_LEN,"/%.1fM",0.1);
        }
        MMI_STRNTOWSTR(mms_size_wch, MMISMS_MEMSTATE_DELMSG_MAX_LEN, mms_size_ch, 
            MMISMS_MEMSTATE_DELMSG_MAX_LEN, strlen((char*)mms_size_ch));
        break;
#ifdef MMI_OTA_SUPPORT
    case MMISMS_DELMSG_TYPE_OTAPUSH:
        msg_num = MMIAPIOTA_GetPushTotalNum() - MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_WAP_PUSH);
        break;
#endif
    case MMISMS_DELMSG_TYPE_WAPPUSH:
#ifdef MMI_OTA_SUPPORT
        msg_num = MMIAPIOTA_GetOTATotalNum() - MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_TYPE_WAP_OTA);
#endif
		break;
    default:
        return;
    }
    *msg_num_ptr = msg_num;
    if (PNULL == string_ptr)
    {
        return;
    }    
    string_ptr->wstr_len = sprintf((char *)num_ch, "%d", msg_num);
    MMI_STRNTOWSTR(string_ptr->wstr_ptr, string_ptr->wstr_len, (const uint8 *)num_ch, string_ptr->wstr_len, string_ptr->wstr_len);   
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jixin.xu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelAllWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{

    MMISMS_DELALLMSG_WIN_PARAM_T *win_data_p = (MMISMS_DELALLMSG_WIN_PARAM_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : to enter the del all msg window
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterDelAllWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMI_HANDLE_T win_handle = 0;
    MMISMS_DELALLMSG_WIN_PARAM_T *param_to_create_delallmsg_win = PNULL;

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT   //纯聊天界面不区分信箱概念
    if (MMK_IsOpenWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID))
    {
        MMK_CloseWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID);
    }
#else
    if (MMK_IsOpenWin(MMISMS_MSGDEL_ALL_WIN_ID))
    {
        MMK_CloseWin(MMISMS_MSGDEL_ALL_WIN_ID);
        MMK_CloseWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID);
        MMK_CloseWin(MMISMS_BOXDEL_ALL_CHILD_WIN_ID);
    }//子窗口要关闭吗?
#endif
    param_to_create_delallmsg_win = (MMISMS_DELALLMSG_WIN_PARAM_T *)SCI_ALLOC_APPZ(sizeof (MMISMS_DELALLMSG_WIN_PARAM_T));
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT   //纯聊天界面不区分信箱概念
    //SetDelMsgChildType(MMISMS_DELMSG_CHILD_MSGTYPE);
    if (PNULL != param_to_create_delallmsg_win)
    {
        param_to_create_delallmsg_win->sms_delmsg_all_info.child_type = MMISMS_DELMSG_CHILD_MSGTYPE;
    }
    win_handle = MMK_CreateWin((uint32*)MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_TAB, PNULL);
#else
    win_handle = MMK_CreateParentWin((uint32 *)MMISMS_MSGDEL_ALL_WIN_TAB, PNULL);
    MMK_CreateChildWin(MMISMS_MSGDEL_ALL_WIN_ID, (uint32 *)MMISMS_BOXDEL_ALL_CHILD_WIN_TAB, (ADD_DATA)param_to_create_delallmsg_win);
    MMK_CreateChildWin(MMISMS_MSGDEL_ALL_WIN_ID, (uint32 *)MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_TAB, (ADD_DATA)param_to_create_delallmsg_win);
    MMK_SetChildWinFocus(MMISMS_MSGDEL_ALL_WIN_ID, MMISMS_BOXDEL_ALL_CHILD_WIN_ID);
#endif
    MMK_SetWinUserData(win_handle, (void*)param_to_create_delallmsg_win);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (param_to_create_delallmsg_win);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;
}

#endif


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
