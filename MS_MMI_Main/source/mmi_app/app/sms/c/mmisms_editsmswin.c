/*****************************************************************************
 ** File Name:   mmisms_editsmswin.c
 ** Author:      sam.hua
 ** Date:        21/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe SMS EDIT Win
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  21/aug/2012      sam.hua       written
 ****************************************************************************/
/*
//There are three different layout for sms edit win
defined MAINLCD_SIZE_128X64     ---Mode one

not defined MAINLCD_SIZE_128X64 and and not MMI_GUI_STYLE_TYPICAL
not defined MAINLCD_SIZE_128X64 and and MMI_GUI_STYLE_TYPICAL


*/
#ifndef MMIMMS_SMS_IN_1_SUPPORT


#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmisms_internal.h"
#include "mmisms_mainmenuwin.h"
#include "mmisms_messagecountwin.h"
#include "mmisms_messageboxwin.h"
#include "mmisms_commonui.h"
#include "mmisms_nv.h"
#include "mmisms_send.h"
#include "mmidisplay_data.h"

#include "mmisms_export.h"
#include "mmisms_edit.h"
#include "mmisms_set.h"
#include "mmisms_read.h"
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
#include "wdp_update_wintab.h"
#endif
#include "mmisms_order.h"
#include "mmisms_appcontrol.h"
#include "mmismsapp_interface.h"
/** -------------------------------------------------------------------------*/
//                          Types and defines                                //
/** -------------------------------------------------------------------------*/

#if (defined MAINLCD_SIZE_240X400)||(defined MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_128X160) || defined(MAINLCD_SIZE_176X220)
#define MMISMS_EDIT_CONTACT_ICON_WIDTH  19
#define MMISMS_EDIT_CONTACT_ICON_WIDTH_H  10
#else
#define MMISMS_EDIT_CONTACT_ICON_WIDTH  15
#define MMISMS_EDIT_CONTACT_ICON_WIDTH_H  10
#endif

#ifdef MMI_PDA_SUPPORT
#define MMISMS_EDIT_TEXT_NUM_SPACE  10
#else
#define MMISMS_EDIT_TEXT_NUM_SPACE  20
#endif
#define MMISMS_CHAT_TEXT_MARGIN_LEFT  15
#define MMISMS_CHAT_TEXT_MARGIN_RIGHT 5
#define MMISMS_CHAT_TEXT_MARGIN_TOP  8
#define MMISMS_CHAT_TEXT_MARGIN_BOTTOM  8

//begin 20101202
#ifdef MAINLCD_SIZE_128X64
#define EDIT_IM_ICON_HEIGHT 16
#define EDIT_TEXT_HEIGHT 18
#endif
//end 20101202


typedef struct
{
    BOOLEAN s_is_add_contact;
    MN_DUAL_SYS_E dual_sys;                                 //卡
    MMI_HANDLE_T  win_handle;
    wchar         edit_string[MMISMS_MAX_MESSAGE_LEN + 1];    //edit_string
    uint32        edit_strlen;
    MMISMS_DEST_ADDR_INFO_T dest_info;                      //dest infomation
    MMI_HANDLE_T  content_handle;
    MMI_HANDLE_T  number_handle;
    BOOLEAN       is_msg_sent;
    BOOLEAN       is_msg_saved;
    BOOLEAN       is_replaced_msg;
    BOOLEAN       is_timer_msg;
    
    BOOLEAN       is_edit_mode;
    BOOLEAN       is_blank_address;
    MMISMS_READ_MSG_T read_msg;
    MMISMS_ORDER_ID_T cur_order_id;    
} MMISMS_EDITSMSWIN_DATA_T;

//#ifdef MMI_PDA_SUPPORT
extern MMISMS_GLOBAL_T g_mmisms_global; 
//#endif 

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
extern char      *g_map_push_message_ptr;
extern uint32    push_message_data_len;
extern BOOLEAN   s_is_map_allow_sync;
#endif  

/** -------------------------------------------------------------------------*/
//                          LOCAL FUNCTIONS                                 //
/** -------------------------------------------------------------------------*/

#ifdef MMI_SMS_SYN_SUPPORT
/*****************************************************************************/
//  Description : send sms by bt
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
LOCAL void MMISMS_SendSmsByBtSend(uint8 *data, uint32 data_len);
#endif

/*****************************************************************************/
//  Description :sms edit win
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSmsWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//     Description : to handle the message of edit menu win
//    Global resource dependence :
//  Author:louis.wei
//  Modify: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditMenuWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN
);

//begin 20101202
#ifdef MAINLCD_SIZE_128X64
/*****************************************************************************/
//  Description :sms edit number win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSmsNumberWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
#endif

/*****************************************************************************/
//     Description : add the contact information to the SMS edit window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddContactInfo2Editbox(MMIPB_HANDLE_T handle, MMI_HANDLE_T    ctrl_handle);

/*****************************************************************/
//  Description :短信编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsSelectContactCallback(MMIPB_HANDLE_T handle);


/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNumberFromEditbox(MMISMS_DEST_ADDR_INFO_T* dest_info,
                                   MMI_CTRL_ID_T ctrl_id,     //IN:
                                   BOOLEAN  is_delete_invalid
                                  );

/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNumberFromPb(
    MMISMS_DEST_ADDR_INFO_T* dest_info,
    MMIPB_HANDLE_T handle,
    MMI_MESSAGE_ID_E    msg_id    //IN:
);

/*****************************************************************************/
//     Description : load number to editbox
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddNumber2Edit(MMISMS_DEST_ADDR_INFO_T* dest_info,
                          MMI_CTRL_ID_T    ctrl_id
                         );


#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
//     Description : load timer info to editbox
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddTimer2Label(
    MMI_CTRL_ID_T ctrl_id,
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr
);

/*****************************************************************************/
//  Description : set Timer SMS display
//  Global resource dependence :
//  Author:  renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetTimerMsgFreqDateDisplay(void);

/*****************************************************************************/
//  Description : set Timer SMS label param
//  Global resource dependence :
//  Author:  renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetTimerMsgStartLabelParam(void);

/*****************************************************************************/
//  Description : set Timer SMS label param
//  Global resource dependence :
//  Author:  renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetTimerMsgLabelParam(void);
#endif
#ifdef MMI_CONCATENATED_SMS_SEND_QUERY_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of send query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);
/*****************************************************************************/
//     Description : open the SMS send query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void OpenSendQueryWin(uint16 page_num);
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
#endif

/******************************************************************************
//  Description : send operate of edit win
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL BOOLEAN EditSMSWin_SendSMS(MMISMS_EDITSMSWIN_DATA_T* windata);

/******************************************************************************
//  Description : Update Windata,and save it to global variables for send operation
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSendData(MMISMS_EDITSMSWIN_DATA_T* windata_p);

/******************************************************************************
//  Description : Update Windata,and save it to global variables for save operation
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSaveData(MMISMS_EDITSMSWIN_DATA_T* windata_p);

/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL void SMS_SAVE(MN_DUAL_SYS_E dual_sys, MMISMS_EDITSMSWIN_DATA_T* windata_p);


/** -------------------------------------------------------------------------*/
//                          WINTAB DEFINES                                  //
/** -------------------------------------------------------------------------*/

#ifdef MAINLCD_SIZE_128X64
WINDOW_TABLE( MMISMS_EDIT_SMS_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleEditSmsWinMsg),
    WIN_ID(MMISMS_EDIT_SMS_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),

    //  CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMISMS_EDIT_SMS_SENDTO_CTRL_ID,MMISMS_EDIT_SMS_NUM_CTRL_ID),
    //  CHILD_EDIT_LIST_CTRL(TRUE,MMISMS_DEST_ADDR_MAX_NUM,MMISMS_PBNUM_MAX_LEN,MMISMS_EDIT_SMS_PHONENUM_CTRL_ID,MMISMS_EDIT_SMS_NUM_CTRL_ID),
    //  CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMISMS_EDIT_SMS_TEXT_CTRL_ID,MMISMS_EDITWIN_EDITCTRL_ID),
    CREATE_EDIT_TEXT_CTRL(MMISMS_MAX_MESSAGE_LEN, MMISMS_EDIT_SMS_TEXT_CTRL_ID),

    END_WIN
};
WINDOW_TABLE( MMISMS_EDIT_SMS_NUMBER_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleEditSmsNumberWinMsg),
    WIN_ID(MMISMS_EDIT_SMS_NUMBER_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_MAIN_PB, TXT_NULL, STXT_RETURN),

    CREATE_EDIT_LIST_CTRL(MMISMS_DEST_ADDR_MAX_NUM, MMISMS_PBNUM_MAX_LEN, MMISMS_EDIT_SMS_PHONENUM_CTRL_ID),

    END_WIN
};
#else
WINDOW_TABLE( MMISMS_EDIT_SMS_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleEditSmsWinMsg),
    WIN_ID(MMISMS_EDIT_SMS_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_TITLE(TXT_SMS_NEW_SMS),
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
#else
#ifdef MMI_SMS_MINI_SUPPORT
    WIN_TITLE(TXT_SMS_NEW_SMS),
#else
    WIN_TITLE(TXT_SMS_WRITE),
#endif
#endif

#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_EDITWIN_EDITCTRL_ID),

#ifdef MMI_TIMERMSG_SUPPORT
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMISMS_TIMER_SMS_CTRL_ID, MMISMS_EDITWIN_EDITCTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMISMS_EDIT_TIMER_CTRL_ID, MMISMS_TIMER_SMS_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE, MMISMS_EDIT_TIMER_FREQDATE_CTRL_ID, MMISMS_TIMER_SMS_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISMS_EDIT_SMS_NUM_CTRL_ID, MMISMS_EDITWIN_EDITCTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL, MMISMS_EDIT_SMS_SENDTO_CTRL_ID, MMISMS_EDIT_SMS_NUM_CTRL_ID),
#else
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMISMS_EDIT_SMS_SENDTO_CTRL_ID, MMISMS_EDIT_SMS_NUM_CTRL_ID),
#endif
    CHILD_EDIT_LIST_CTRL(TRUE, MMISMS_DEST_ADDR_MAX_NUM, MMISMS_PBNUM_MAX_LEN, MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, MMISMS_EDIT_SMS_NUM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMISMS_MAX_MESSAGE_LEN, MMISMS_EDIT_SMS_TEXT_CTRL_ID, MMISMS_EDITWIN_EDITCTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_SEND, TXT_NULL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_EDITWIN_EDITCTRL_ID),
#endif

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMISMS_SMS_SINATURE_CTRL_ID, MMISMS_EDITWIN_EDITCTRL_ID),
    CHILD_EDIT_TEXT_CTRL(FALSE,
    MMISMS_MAX_SIGNATURE_NUMBER_LEN,
    MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID,
    MMISMS_SMS_SINATURE_CTRL_ID),
#endif
    END_WIN
};

#endif


WINDOW_TABLE( MMISMS_EDITOPTION_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleEditMenuWinMsg),
    WIN_ID(MMISMS_EDITOPTION_WIN_ID),
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_SMS_WRITE_OPT, MMISMS_WRITEOPTION_MENU_CTRL_ID),
#else
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SMS_WRITE_OPT, MMISMS_WRITEOPTION_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

/** -------------------------------------------------------------------------*/
//                          Function definition                              //
/** -------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Entry SMS Edit Win
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_EnterSmsEditWin(MMISMS_EDITSMSWIN_DATA_T    *windata_p)
{
    MMI_HANDLE_T    win_handle = PNULL;

    if (windata_p == PNULL)
    {
        return FALSE;
    }
    // open edit window
    if (!MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
    {
        win_handle = MMK_CreateWin((uint32 *)MMISMS_EDIT_SMS_WIN_TAB, PNULL);
    }
    else
    {
        MMK_WinGrabFocus(MMISMS_EDIT_SMS_WIN_ID);
        win_handle = MMK_ConvertIdToHandle( MMISMS_EDIT_SMS_WIN_ID );
    }
    windata_p->win_handle = win_handle;
    MMK_SetWinUserData(win_handle, windata_p);

    return TRUE;
}

/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL void SMS_SEND(MN_DUAL_SYS_E dual_sys, MMI_HANDLE_T  win_handle)
{
    MMISMS_EDITSMSWIN_DATA_T* windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinUserData(win_handle);

    if(windata_p == PNULL)
    {
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif

#ifdef MMI_TIMERMSG_SUPPORT
    if (windata_p->is_timer_msg)
    {
        if (MMISMS_TIMERMSG_CheckTimerSend(TRUE))
        {
            //Save it to sendfail box
            SMS_SAVE(dual_sys,windata_p);
            return;
        }    
    }
#endif    
    MMISMSAPP_SendSmsWithData(dual_sys,
            FALSE,
            &windata_p->dest_info,
            windata_p->edit_string,
            windata_p->edit_strlen,
            PNULL
            );


}

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//  Description : BT SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_BtSendItem(
                              BOOLEAN       is_resend
                              )
{
    MMI_CTRL_ID_T ctrl_text_id = MMISMS_EDIT_SMS_TEXT_CTRL_ID;
    MMI_STRING_T edit_string = {0};
    uint8 bt_num_count = 0;
    uint32                service_type = 0;
    BT_ADDRESS            dev_addr = {0};
    
    SCI_TRACE_LOW("[MMISMS] case MSG_BT_SEND_ITEM");
#ifndef WIN32
    MMIBT_IsConnectDevice(&dev_addr);
    service_type = BT_GetPairedDeviceService(&dev_addr);
    
    if (!(BT_SERVICE_MAP_SERVER & service_type))
    {
        SCI_TRACE_LOW("[MMISMS] MSG_BT_SEND_ITEM remote device not support MAP!");
        MMIAPISMS_SetSendAddrCount(0);
        MMIAPISMS_SetIsSendMultiSms(FALSE);
        //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
        MMISMS_OpenAlertWarningWin(TXT_MAP_NOT_SUPPORT);
    }
    else
#endif
    {
        if(!is_resend)
        {
            bt_num_count = MMIAPISMS_GetSendAddrCount();
            if(bt_num_count == 0)
            {
                if(PNULL != g_map_push_message_ptr)
                {
                    SCI_FREE(g_map_push_message_ptr);
                    g_map_push_message_ptr = PNULL;
                }
                
                GetNumberFromEditbox(&g_mmisms_global.edit_content.dest_info,MMISMS_EDIT_SMS_PHONENUM_CTRL_ID,FALSE);
                
                bt_num_count = g_mmisms_global.edit_content.dest_info.dest_list.number;
                MMIAPISMS_SetSendAddrCount(bt_num_count);
                
                GUIEDIT_GetString(ctrl_text_id, &edit_string);
                if (0 == edit_string.wstr_len)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_EMPTY_CONTENT);
                    MMK_CloseWin(MMISMS_EDITOPTION_WIN_ID);
                    
                    return;
                }
                else
                {
                    // to save the information
                    MMISMS_SetMessageContent(edit_string.wstr_len, edit_string.wstr_ptr, FALSE, &g_mmisms_global.edit_content);
                }
            }
        }
        else
        {
            bt_num_count = MMIAPISMS_GetSendAddrCount();
        }
        
        if(bt_num_count >= 1)
        {
            SCI_TRACE_LOW("MMISMS: send_message.alphabet = %d", g_mmisms_global.edit_content.send_message.alphabet);

            g_map_push_message_ptr = MMIAPISMS_ComposeMAPPushBody(
                (uint8*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[bt_num_count - 1]), 
                strlen((char*)g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[bt_num_count - 1]),
                (uint8*)g_mmisms_global.edit_content.send_message.data, g_mmisms_global.edit_content.send_message.length,
                g_mmisms_global.edit_content.send_message.alphabet);
            push_message_data_len = strlen(g_map_push_message_ptr);
            
            SCI_TRACE_LOW("MMISMS: MGS_BT_PUSH_MESSAGE START");
            
            s_is_map_allow_sync = TRUE;
            MMIAPISMS_SetCurOperIsPush(TRUE);
            
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT) && defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
            MMIAPISMS_CreateSmsSyncTimer();
            MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_MAP);
#endif
            if(MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
            {
                StartMapMessageSync(MMISMS_EDIT_SMS_WIN_ID);
            }
            else
            {
                StartMapMessageSync(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        }
    }
}
#endif

/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL void SMS_SAVE(MN_DUAL_SYS_E dual_sys, MMISMS_EDITSMSWIN_DATA_T* windata_p)
{
    SMS_SAVE_OPERATE_DATA_T op_data={0};

    op_data.dual_sys = dual_sys;
    op_data.is_needLoadSaved = FALSE;
    op_data.sms_addr = &windata_p->dest_info;
    op_data.save_strlen = windata_p->edit_strlen;
    op_data.save_string_ptr = windata_p->edit_string;
    op_data.is_timer_msg =windata_p->is_timer_msg;
    if (windata_p->is_replaced_msg)
    {
        MMISMSAPP_ReplaceSms(op_data,PNULL,PNULL,PNULL);
    }
    else
    {
        MMISMSAPP_SaveSms(op_data,PNULL,PNULL,PNULL);
    }
}


#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T win_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());
    MMISMS_EDITSMSWIN_DATA_T* windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinUserData(win_handle);

    if (windata_p == PNULL)
    {
        return recode;
    }

    if (isReturnOK)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif
        windata_p->dual_sys = dual_sys;
        windata_p->is_msg_sent = TRUE;      //Sms is sent,don't need to save
        SMS_SEND(windata_p->dual_sys, windata_p->win_handle);
        //MMK_CloseWin(windata_p->win_handle);    //msg is sent, close edit win
    }
    return recode;
}

/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SaveSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T win_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());
    MMISMS_EDITSMSWIN_DATA_T* windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinUserData(win_handle);

    if (windata_p == PNULL)
    {
        return recode;
    }

    if (isReturnOK)
    {
        windata_p->dual_sys = dual_sys;
        SMS_SAVE(windata_p->dual_sys, windata_p);
        MMK_CloseWin(windata_p->win_handle);
    }
    return recode;
}
#endif

/*****************************************************************************/
//  Description : Save SMS operation of edit sms win
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/\
LOCAL void EditSMSWin_SaveSMS(MMISMS_EDITSMSWIN_DATA_T    *windata_p)
{
    BuildSaveData(windata_p);
#ifdef MMI_TIMERMSG_SUPPORT
    if (windata_p->is_timer_msg)
    {
        //Convert to normal draft
        windata_p->is_timer_msg =FALSE;
        MMISMS_SetIsTimerMode(FALSE);
    }
#endif
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!windata_p->is_replaced_msg)
    {
        // uint32 i = 0;
        MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim;
        major_select_sim = MMIAPISET_GetMajorSimCard(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
        if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
        {
            windata_p->dual_sys = MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
        }
        else
        {
            if (MMISMS_MEHaveEnoughSpace()
//                    || ((MMIAPIPHONE_GetSimCardOkNum(PNULL, 0) == 0)
//#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                     //   || (MMISMS_BOX_SECURITY == MMISMS_GetCurBoxType()) //Never used
//#endif
//                       )
               )
            {
                windata_p->dual_sys = MN_DUAL_SYS_1;
            }
			else if( (MMIAPIPHONE_GetSimCardOkNum(PNULL, 0) == 0) 
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
				|| (MMISMS_BOX_SECURITY == MMISMS_GetCurBoxType())
				
#endif
				)
			{
				//prompt: no free space
				MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
            }
            else
            {
                MMISMS_SelectSIMFunc(SaveSelectSimCallback, (ADD_DATA)windata_p->win_handle, TRUE);
                return;
            }
        }
    }
#else
    windata_p->dual_sys = MN_DUAL_SYS_1;
#endif
    SMS_SAVE(windata_p->dual_sys, windata_p);
    //MMK_CloseWin(windata_p->win_handle);
}

/******************************************************************************
//  Description : send operate of edit win
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL BOOLEAN EditSMSWin_SendSMS(MMISMS_EDITSMSWIN_DATA_T    *windata_p)
{
#ifdef MMI_CONCATENATED_SMS_SEND_QUERY_SUPPORT
    uint16 page_num = 0;
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint16 sim_num = 0;
    uint16 sim_ok = 0;

    sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
	SCI_TRACE_LOW("[MMISMS] EditSMSWin_SendSMS sim_num = %d", sim_num);
#endif

    

    if (windata_p == PNULL)
    {
        return FALSE;
    }
    // send all message
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (0 == sim_num || TRUE == MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (1 == sim_num)
    {
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        MMISMS_SelectSIMFunc(SendSelectSimCallback, (ADD_DATA)windata_p->win_handle, TRUE);
        return TRUE;
#else
#ifdef MMI_CONCATENATED_SMS_SEND_QUERY_SUPPORT
        GUIEDIT_GetLeftNumInTextEditBox(windata_p->content_handle, &page_num);

        if (page_num > 1)
        {
            OpenSendQueryWin(page_num);
        }
        else
#endif
        {
            windata_p->dual_sys = (MN_DUAL_SYS_E)sim_ok;

            SMS_SEND(windata_p->dual_sys, windata_p->win_handle);
            //MMK_CloseWin(windata_p->win_handle);
        }
#endif
    }
    else
    {
#ifdef MMI_CONCATENATED_SMS_SEND_QUERY_SUPPORT
        GUIEDIT_GetLeftNumInTextEditBox(windata_p->content_handle, &page_num);

        if (page_num > 1)
        {
            OpenSendQueryWin(page_num);
        }
        else
#endif
        {
            if (windata_p->dual_sys == MN_DUAL_SYS_MAX)
            {
                MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim;
                major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
                if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                {
                    windata_p->dual_sys =  MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
                }
                else
                {
#ifdef MMI_SMS_FIX_SEND_SIM
                    if (MMISMS_GetFixSIMSendingOnOFF())
                    {
                        windata_p->dual_sys = MMISMS_GetFixSIMSendingSIMSYS();
                    }
                    else
#endif
                    {
                        MMISMS_SelectSIMFunc(SendSelectSimCallback, (ADD_DATA)windata_p->win_handle, TRUE);
                        return TRUE;
                    }

                }
            }

            SMS_SEND(windata_p->dual_sys, windata_p->win_handle);
            //MMK_CloseWin(windata_p->win_handle);
        }
    }
    return TRUE;
#else
    if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1) || TRUE == MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    {
#ifdef MMI_CONCATENATED_SMS_SEND_QUERY_SUPPORT
        if (page_num > 1)
        {
            OpenSendQueryWin(page_num);
        }
        else
#endif
        {
            SMS_SEND(MN_DUAL_SYS_1, windata_p->win_handle);
            //MMK_CloseWin(windata_p->win_handle);
        }
    }
    return TRUE;
#endif
}

/******************************************************************************
//  Description : Update Windata,and save it to global variables for save
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSaveData(MMISMS_EDITSMSWIN_DATA_T* windata_p)
{
    MMI_STRING_T        edit_string = {0};

    if (windata_p == PNULL)
    {
        return;
    }

    //Get Current Number and current content by control handle
    GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);

    GUIEDIT_GetString(windata_p->content_handle, &edit_string);

    windata_p->edit_strlen = edit_string.wstr_len;
    SCI_MEMCPY(windata_p->edit_string, edit_string.wstr_ptr, windata_p->edit_strlen * sizeof(wchar));
    return;
}



/******************************************************************************
//  Description : Update Windata,and save it to global variables.
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSendData(MMISMS_EDITSMSWIN_DATA_T* windata_p)
{
    MMI_STRING_T        edit_string = {0};

    if (windata_p == PNULL)
    {
        return;
    }


    //Get Current Number and current content by control handle
    GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);

    GUIEDIT_GetString(windata_p->content_handle, &edit_string);


#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    if (MMISMS_AllowAutoSignature())
    {
        MMI_STRING_T sig_string = {0};
        MMI_STRING_T display_string = {0};
        wchar        signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
        uint16       signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;


#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        signature_wstr[0] = 0x0a00;
#else
        signature_wstr[0] = 0x000a;
#endif
        signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr + 1) + 1;

        sig_string.wstr_ptr = signature_wstr;
        sig_string.wstr_len = signature_str_len;

        display_string.wstr_ptr = windata_p->edit_string;
        if (PNULL == display_string.wstr_ptr)
        {
            //SCI_TRACE_LOW:"HandleEditSmsWinMsg alloc mem fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_26001_112_2_18_2_55_14_843, (uint8*)"");
            return;
        }

        MMIAPICOM_MergeTwoString(
            &edit_string,
            &sig_string,
            &display_string,
            MMISMS_MAX_MESSAGE_LEN
        );

        windata_p->edit_strlen = display_string.wstr_len;
    }
    else
#endif
    {
        windata_p->edit_strlen = edit_string.wstr_len;
        SCI_MEMCPY(windata_p->edit_string, edit_string.wstr_ptr, windata_p->edit_strlen * sizeof(wchar));
    }
}


#ifdef MMI_CONCATENATED_SMS_SEND_QUERY_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of send query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = 0;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint16 sim_num = 0;
    uint16 sim_ok = 0;
#endif

    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

            if (0 == sim_num)
            {
                MMIPHONE_AlertSimNorOKStatus();
            }
            else if (1 == sim_num)
            {
                dual_sys = (MN_DUAL_SYS_E)sim_ok;

                SMS_SEND(dual_sys, MMISMS_EDIT_SMS_WIN_ID);
                //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
            }
            else
            {
                dual_sys = MMISMS_GetDestDualSys();
                if (MMISMS_GetDestDualSys() == MN_DUAL_SYS_MAX)
                {
                    MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
                    if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                    {
                        dual_sys = MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
                    }
                    else
                    {
#ifdef MMI_SMS_FIX_SEND_SIM

                        if (MMISMS_GetFixSIMSendingOnOFF())
                        {
                            dual_sys = (MN_DUAL_SYS_E)MMISMS_GetFixSIMSendingSIMSYS();
                        }
                        else
#endif
                        {
                            MMISMS_SelectSIMFunc(MultiSendSelectSimCallback, (ADD_DATA)MMIVC_SMS_SEND_MAX, TRUE);
                            break;
                        }
                    }
                }
                SMS_SEND(dual_sys, MMISMS_EDIT_SMS_WIN_ID);
                //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);

            }
#else
            if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
            {
                MMIPHONE_AlertSimNorOKStatus();
            }
            else
            {
                SMS_SEND(MN_DUAL_SYS_1, MMISMS_EDIT_SMS_WIN_ID);
                //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
            }
#endif

            MMK_CloseWin(win_id);
            break;

        case MSG_SMS_SELECTRESULT:
            dual_sys = MMISMS_GetCurSendDualSys();
            SMS_SEND(dual_sys, MMISMS_EDIT_SMS_WIN_ID);
            //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
        }

    return (recode);
}

/*****************************************************************************/
//     Description : open the SMS send query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void OpenSendQueryWin(uint16 page_num)
{
    uint8               disp_str_len   = 0;
    uint8               disp_num_len   = 0;
    uint8               send_msg_num[6] = {0};
    wchar               disp_num[10] = {0};
    const wchar         blank[5]={' ','\0'};
    MMI_STRING_T        prompt_str1 = {0};
    MMI_STRING_T        prompt_str2 = {0};
    MMI_STRING_T        tmp_prompt = {0};
    MMI_WIN_ID_T        query_win_id = MMISMS_QUERY_WIN_ID;
    MMI_STRING_T        display_prompt_str ={0};
    MMK_CloseWin(query_win_id);
    
    MMI_GetLabelTextByLang(TXT_SMS_SEND_QUERY_HEADR, &prompt_str1);
    
    disp_num_len = sprintf((char *)send_msg_num, "%ld ", page_num);
    
    MMI_STRNTOWSTR(disp_num, disp_num_len, send_msg_num, disp_num_len, disp_num_len);
    
    MMI_GetLabelTextByLang(TXT_SMS_SEND_QUERY_TAIL, &tmp_prompt);
    
    // the total string wstr_len
    disp_str_len =  MMIAPICOM_Wstrlen(blank) + disp_num_len + tmp_prompt.wstr_len;
    display_prompt_str.wstr_len =prompt_str1.wstr_len +disp_str_len;
    // allocate memory
    prompt_str2.wstr_ptr = SCI_ALLOC_APPZ((disp_str_len + 1) * sizeof(wchar));
    display_prompt_str.wstr_ptr = SCI_ALLOC_APPZ((display_prompt_str.wstr_len +1)*sizeof(wchar));
    MMI_WSTRNCPY(display_prompt_str.wstr_ptr,display_prompt_str.wstr_len,prompt_str1.wstr_ptr,display_prompt_str.wstr_len,prompt_str1.wstr_len);
    //copy blank
    MMI_WSTRNCPY(prompt_str2.wstr_ptr, 
        disp_str_len, 
        blank, 
        disp_str_len, 
        MMIAPICOM_Wstrlen(blank));
    
    prompt_str2.wstr_len = MMIAPICOM_Wstrlen(blank);
    // copy the num
    MMI_WSTRNCPY(
        &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]), 
        disp_str_len, 
        disp_num, 
        disp_str_len, 
        disp_num_len);
    
    prompt_str2.wstr_len += disp_num_len;
    
    // copy the prompt string
    MMI_WSTRNCPY(
        &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]),
        disp_str_len,
        tmp_prompt.wstr_ptr,
        disp_str_len,
        tmp_prompt.wstr_len
        );
    prompt_str2.wstr_len += tmp_prompt.wstr_len;
    
    MMIAPICOM_Wstrncat(display_prompt_str.wstr_ptr,prompt_str2.wstr_ptr,prompt_str2.wstr_len);
    if (PNULL != prompt_str2.wstr_ptr)
    {
        SCI_FREE(prompt_str2.wstr_ptr);
    }
    MMIPUB_OpenQueryWinByTextPtr(&display_prompt_str,PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleSendQueryWinMsg );
    
    if (PNULL != display_prompt_str.wstr_ptr)
    {
        SCI_FREE(display_prompt_str.wstr_ptr);
    }
}
#endif

/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL BOOLEAN EditSMSWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_EDITSMSWIN_DATA_T *windata_p = PNULL;
    uint8   addr_array[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
    uint8   addr_len= 0;     
    MMI_STRING_T    edit_content_str = {0};


    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    
    if (PNULL == callback_data
        ||callback_data->opreation_cb==PNULL 
        ||callback_data->user_data==PNULL)
    { 
        SCI_TRACE_LOW("MMISMS callback_data pnull");
        return FALSE;
    }
    windata_p = (MMISMS_EDITSMSWIN_DATA_T *)callback_data->user_data;

    if (callback_data->result == FALSE)
    {
        SCI_FREE(windata_p);
        return TRUE;
    }

    //Get Read result and convert address and content
    addr_len =  MMISMS_GetCurNumByOrder(windata_p->cur_order_id,
            &windata_p->dual_sys,
            addr_array,
            MMISMS_MAX_USERNUMBER_LEN + 2
            );

    if ( 0 < addr_len && !windata_p->is_blank_address)
    {
        MMISMS_SetAddressToMessage(addr_array, addr_len, &windata_p->dest_info.dest_list);
    }

    MMISMS_GetSMSStringFromContent(&windata_p->read_msg.read_content,
                    &edit_content_str);
  
    SCI_MEMCPY(windata_p->edit_string,edit_content_str.wstr_ptr,sizeof(wchar)*edit_content_str.wstr_len);
    
    windata_p->edit_strlen = MMIAPICOM_Wstrlen(windata_p->edit_string);                 

    if (PNULL != edit_content_str.wstr_ptr)
    {
        SCI_FREE(edit_content_str.wstr_ptr);
        edit_content_str.wstr_ptr = PNULL;
    } 
    //Open EditSMS Win

    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));
    if (!MMISMS_EnterSmsEditWin(windata_p))
    {
        SCI_FREE(windata_p);
        return TRUE;
    }

    return TRUE;
}


/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_EnterEditSMSWinForForward(MMISMS_ORDER_ID_T order_id)
{
//    MMI_CTRL_ID_T ctrl_id = MMISMS_EDIT_SMS_TEXT_CTRL_ID;
    MMISMS_EDITSMSWIN_DATA_T    *windata_p = PNULL;
	MMI_STRING_T	prompt_str = {0};

    if (order_id == PNULL)
    {
        return;
    }

    windata_p = SCI_ALLOCAZ(sizeof(MMISMS_EDITSMSWIN_DATA_T));
    if (PNULL == windata_p)
    {
        return;
    }
    windata_p->is_edit_mode = TRUE;
    windata_p->cur_order_id = order_id;
    windata_p->is_blank_address= TRUE;

    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);

    MMISMSAPP_ReadOneSms(
                order_id,
                EditSMSWinReadSmsCallback,
                &windata_p->read_msg,   //action data
                (void*)windata_p);

}



/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_EnterEditSMSWinByOrderID(MMISMS_ORDER_ID_T order_id)
{
//    MMI_CTRL_ID_T ctrl_id = MMISMS_EDIT_SMS_TEXT_CTRL_ID;
    MMISMS_EDITSMSWIN_DATA_T    *windata_p = PNULL;
	MMI_STRING_T	prompt_str = {0};

    if (order_id == PNULL)
    {
        return;
    }

    windata_p = SCI_ALLOCAZ(sizeof(MMISMS_EDITSMSWIN_DATA_T));
    if (PNULL == windata_p)
    {
        return;
    }
    windata_p->is_edit_mode = TRUE;
    windata_p->cur_order_id = order_id;

    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);

    MMISMSAPP_ReadOneSms(
                order_id,
                EditSMSWinReadSmsCallback,
                &windata_p->read_msg,   //action data
                (void*)windata_p);

}

/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_SendGroupSmsEx(
    MN_DUAL_SYS_E   dual_sys,           //IN:
    MMI_STRING_T    *sms_init_ptr,        //IN:
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr
)
{
    MMISMS_EDITSMSWIN_DATA_T    *windata_p = PNULL;
    windata_p = SCI_ALLOCAZ(sizeof(MMISMS_EDITSMSWIN_DATA_T));
    if (PNULL == windata_p)
    {
        return;
    }

#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_SetIsTimerMode(FALSE); //Clear Timer Mode
#endif

    if (!MMISMS_EnterSmsEditWin(windata_p))
    {
        SCI_FREE(windata_p);
        return;
    }
    
    if (dest_info_ptr != PNULL)
    {
        SCI_MEMCPY(&windata_p->dest_info,dest_info_ptr,sizeof(MMISMS_DEST_ADDR_INFO_T));
    }

    //本卡回复
    if (MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS)
            && dual_sys < MMI_DUAL_SYS_MAX
            && MMIAPIPHONE_IsSimAvailable(dual_sys))
    {
        windata_p->dual_sys = dual_sys;
    }
    else
    {
        windata_p->dual_sys = MN_DUAL_SYS_MAX;
    }

    if (MMISMS_MsgIsInDraftBox())
    {
        windata_p->is_replaced_msg = TRUE;  //Msg should be replaced instead of 
        windata_p->dual_sys = MMISMS_GetCurReadMsgDualSys();
    }

    // set init SMS content to edit_box
    if (PNULL != sms_init_ptr)
    {
        //有编辑的短消息内容
        windata_p->edit_strlen = MIN(MMISMS_MAX_MESSAGE_LEN, sms_init_ptr->wstr_len);
        SCI_MEMCPY(windata_p->edit_string, sms_init_ptr->wstr_ptr, windata_p->edit_strlen * sizeof(wchar));

        if (sms_init_ptr->wstr_len > MMISMS_MAX_MESSAGE_LEN)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SMS_TOO_LONG_MSG, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        }
    }
}

/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_WriteNewMessageEx(
    MN_DUAL_SYS_E   dual_sys,           //IN:
    MMI_STRING_T    *sms_init_ptr,        //IN:
    BOOLEAN            need_load_saved,    //IN:    TRUE, load the saved content
    uint8            *dest_addr_ptr,        //IN:
    uint8            dest_addr_len        //IN:
)
{
//    MMI_CTRL_ID_T ctrl_id = MMISMS_EDIT_SMS_TEXT_CTRL_ID;
    MMISMS_EDITSMSWIN_DATA_T    *windata_p = PNULL;

    windata_p = SCI_ALLOCAZ(sizeof(MMISMS_EDITSMSWIN_DATA_T));
    if (PNULL == windata_p)
    {
        return;
    }

#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_SetIsTimerMode(FALSE); //Clear Timer Mode
    MMISMS_SetIsTimerMsgReplace(FALSE);
#endif

    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));
    if (!MMISMS_EnterSmsEditWin(windata_p))
    {
        SCI_FREE(windata_p);
        return;
    }

    // set destination address
    MMISMS_ClearDestNum();
    if ( (0 < dest_addr_len) && (PNULL != dest_addr_ptr) )
    {
        MMISMS_SetAddressToMessage(dest_addr_ptr, dest_addr_len, &windata_p->dest_info.dest_list);
    }

    //本卡回复
    if (MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS)
            &&( dual_sys < MMI_DUAL_SYS_MAX)
            && MMIAPIPHONE_IsSimAvailable(dual_sys))
    {
        SCI_TRACE_LOW("MMISMS: MMIAPISET_GetAnswerWithOriginalSimType");
        windata_p->dual_sys = dual_sys;
        MMISMS_SetDestDualSys(dual_sys); // add for cr120003
    }
    else
    {
        SCI_TRACE_LOW("MMISMS: MN_DUAL_SYS_MAX");
        windata_p->dual_sys = MN_DUAL_SYS_MAX;
        MMISMS_SetDestDualSys(MN_DUAL_SYS_MAX);  // add for cr120003
    }

    if (MMISMS_MsgIsInDraftBox())
    {
        SCI_TRACE_LOW("MMISMS: MMISMS_MsgIsInDraftBox");
        windata_p->is_replaced_msg = TRUE;  //Msg should be replaced instead of 
        //windata_p->dual_sys = MMISMS_GetCurReadMsgDualSys(); fix cr147577
    }


    // set init SMS content to edit_box
    if (PNULL != sms_init_ptr)
    {
        //有编辑的短消息内容
        windata_p->edit_strlen = MIN(MMISMS_MAX_MESSAGE_LEN, sms_init_ptr->wstr_len);
        SCI_MEMCPY(windata_p->edit_string, sms_init_ptr->wstr_ptr, windata_p->edit_strlen * sizeof(wchar));

        if (sms_init_ptr->wstr_len > MMISMS_MAX_MESSAGE_LEN)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SMS_TOO_LONG_MSG, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        }
    }
    else
    {
#ifdef MMI_SMS_MINI_SUPPORT
        if (need_load_saved)//该分支给设置完字体大小后重新创建编辑窗口加载内容使用
        {
            MMI_STRING_T    sms_string={0};
            
            sms_string.wstr_ptr = windata_p->edit_string;
            if (MMISMS_LoadStoreddMsgBuffer(&sms_string))
            {
                windata_p->edit_strlen = sms_string.wstr_len;
            }    
        }
#endif        
    }

}


#ifdef MAINLCD_SIZE_128X64
/*****************************************************************************/
//  Description : set sms edit rect
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetEditSmsRect(MMI_HANDLE_T win_id,MMI_HANDLE_T ctrl_id)
{
    GUI_RECT_T edit_text_rect = {0};
    GUI_RECT_T title_rect = {0};

    edit_text_rect.left = 0;
    edit_text_rect.right = edit_text_rect.left + MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    edit_text_rect.top = EDIT_IM_ICON_HEIGHT;
    edit_text_rect.bottom = edit_text_rect.top + EDIT_TEXT_HEIGHT;

    GUIEDIT_SetRect(ctrl_id, &edit_text_rect);

    title_rect.right = title_rect.left +  MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    title_rect.bottom = EDIT_IM_ICON_HEIGHT;
    GUIWIN_SetTitleRect(win_id, title_rect);
    GUIWIN_SetTitleBackground(win_id, 0,  MMI_WHITE_COLOR);
}

/*****************************************************************************/
//  Description :sms edit content win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSmsWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T edit_string = {0};
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T font_group_id = MENU_SMS_FONT_MENU_OPT;
    MMISMS_FONT_E font_size = MMISMS_GetFontSize();/*lint !e64 */
#endif
    MMISMS_EDITSMSWIN_DATA_T*   windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinUserData(win_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:

            windata_p->content_handle = MMK_GetCtrlHandleByWin(win_id, MMISMS_EDIT_SMS_TEXT_CTRL_ID);

            //edit处理红键消息
            GUIEDIT_SetHandleRedKey(FALSE, windata_p->content_handle);

            GUIEDIT_SetSoftkey(windata_p->content_handle, 1, 0, TXT_SEND, TXT_NULL, PNULL);

            // set font color
            GUIEDIT_SetFontColor(windata_p->content_handle, MMI_BLACK_COLOR);

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            // set font size
            GUIEDIT_SetFont(windata_p->content_handle, MMISMS_GetWinFont());//added by taiping.lai 2011/01/02 解决编辑汉字时显示异常
#endif

            GUIEDIT_SetStyle(windata_p->content_handle, GUIEDIT_STYLE_SINGLE);

            // set rect
            SetEditSmsRect(win_id,windata_p->content_handle);


#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            GUIEDIT_SetFont(windata_p->content_handle, MMISMS_GetWinFont());
#endif

            MMISMS_SetEditTextProperty(windata_p->content_handle);
            GUIEDIT_SetString(windata_p->content_handle, windata_p->edit_string,windata_p->edit_strlen);

            MMK_SetAtvCtrl(win_id, windata_p->content_handle);

            break;

        case MSG_GET_FOCUS:
#ifdef MMI_TIMERMSG_SUPPORT
            windata_p->is_timer_msg = MMISMS_GetIsTimerMode();
            break;
#endif
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_SK_SKB_ICON_PRESSED:
            MMK_CreateWin((uint32 *)MMISMS_EDIT_SMS_NUMBER_WIN_TAB, windata_p);
            break;

        case MMI_SELECT_CONTACT_CNF:    //added by taiping.lai 2011/01/18 添加联系人信息处理
            if (windata_p->s_is_add_contact)
            {
                AddContactInfo2Editbox((MMIPB_HANDLE_T)param, windata_p->content_handle);
                windata_p->s_is_add_contact = FALSE;
            }
            break;

        case MSG_FULL_PAINT:
            {
                GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
                GUI_RECT_T rect = {0};

                rect.left = 0;
                rect.right = rect.left + MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
                rect.top = EDIT_IM_ICON_HEIGHT;
                rect.bottom = MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;

                GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
            }
#ifdef MMI_TIMERMSG_SUPPORT
            windata_p->is_timer_msg = MMISMS_GetIsTimerMode();
#endif
            break;

#ifdef MMI_TIMERMSG_SUPPORT
        case MMISMS_MSG_UPDATE_TIMER:
            SetTimerMsgLabelParam();
            SetTimerMsgFreqDateDisplay();
            break;
#endif

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
        
            MMK_CreateWin((uint32 *)MMISMS_EDITOPTION_WIN_TAB, windata_p);
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            switch (font_size)
            {
                case MMISMS_FONT_BIG:
                    menu_id = MMISMS_MENU_BIG_FONT_ITEM_ID;
                    break;

                case MMISMS_FONT_NORMAL:
                    menu_id = MMISMS_MENU_NORMAL_FONT_ITEM_ID;
                    break;

                case MMISMS_FONT_SMALL:
                    menu_id = MMISMS_MENU_SMALL_FONT_ITEM_ID;
                    break;

                default:
                    menu_id = MMISMS_MENU_NORMAL_FONT_ITEM_ID;
                    break;
            }

            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = MMISMS_WRITEOPTION_MENU_CTRL_ID;
            pop_item_info.menu_id = menu_id;
            pop_item_info.group_id = font_group_id;

            GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
#endif

#ifdef MMI_TIMERMSG_SUPPORT
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = MMISMS_WRITEOPTION_MENU_CTRL_ID;
            pop_item_info.menu_id = MMISMS_GetIsTimerMode() ? MMISMS_MENU_ON_ITEM_ID : MMISMS_MENU_OFF_ITEM_ID;
            pop_item_info.group_id = MENU_SMS_TIMER_MESSAGE_MENU_OPT;

            GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
#endif

            break;

        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_PROMPTWIN_OK:          //TXT_SMS_ASKSAVE_DRAFT
            EditSMSWin_SaveSMS(windata_p);
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;
        case MSG_APP_RED:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            if (windata_p !=PNULL)
            {
                SCI_FREE(windata_p);
            }                
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : set sms edit list rect
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetEditSmsNumberRect(MMI_HANDLE_T win_id,MMI_HANDLE_T ctrl_id)
{
    GUI_RECT_T edit_list_rect = {0};
    GUI_RECT_T title_rect = {0};

    edit_list_rect.right = edit_list_rect.left + MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    edit_list_rect.top = EDIT_IM_ICON_HEIGHT;
    edit_list_rect.bottom = MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;

    GUIEDIT_SetRect(ctrl_id, &edit_list_rect);

    title_rect.right = title_rect.left + MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    title_rect.bottom = EDIT_IM_ICON_HEIGHT;
    GUIWIN_SetTitleRect(win_id, title_rect);
    GUIWIN_SetTitleBackground(win_id, 0,  MMI_WHITE_COLOR);
}
/*****************************************************************************/
//  Description :sms edit number win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSmsNumberWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    MMI_STRING_T edit_string = {0};
    uint16 max_selected_num = 0;
    uint16 num_count = 0;
    MMISMS_EDITSMSWIN_DATA_T* windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinAddDataPtr(win_id);

    if (windata_p == PNULL)
    {
        recode = MMI_RESULT_FALSE;
        return recode;
    }

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            //edit处理红键消息
            windata_p->number_handle = MMK_GetCtrlHandleByWin(win_id, MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);

            AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);

            GUIEDIT_SetHandleRedKey(FALSE, windata_p->number_handle);

            GUIEDIT_SetSoftkey(windata_p->number_handle, 1, 0, TXT_SEND, TXT_NULL, PNULL);

            // set font color
            GUIEDIT_SetFontColor(windata_p->number_handle, MMI_BLACK_COLOR);

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            // set font size
            GUIEDIT_SetFont(windata_p->number_handle, MMISMS_GetWinFont());//added by taiping.lai 2011/01/03 输入发送号码显示字体过小
#endif

            // set rect
            SetEditSmsNumberRect(win_id,windata_p->number_handle);
            MMK_SetAtvCtrl(win_id, windata_p->number_handle);
            break;

        case MMI_SELECT_CONTACT_CNF:
            // from PHONEBOOK, call the API to get the phone number.
            if (GetNumberFromPb(&windata_p->dest_info, (MMIPB_HANDLE_T)param, msg_id))
            {
                // update the edit box
                AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
                MMK_SetAtvCtrl(win_id, windata_p->number_handle);
            }
            break;

        case MMISMS_MSG_UPDATE_NUMBER:
            AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
            break;

            // send
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_SK_SKB_ICON_PRESSED:
            BuildSendData(windata_p);

            // open the input number window
            if (0 < windata_p->dest_info.dest_list.number)
            {
                EditSMSWin_SendSMS(windata_p);
            }
            else
            {
                num_count = GUIEDIT_GetListItemNum(windata_p->number_handle);
                if (num_count == 0)
                {
                    max_selected_num = MMISMS_DEST_ADDR_MAX_NUM -  windata_p->dest_info.dest_list.number;

                    MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        max_selected_num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSmsNumSelectContactCallback
                    );
                }
            }
            break;

        //choose from pb
        case MSG_CTL_OK:
        case MSG_APP_OK:
            GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);
            max_selected_num = MMISMS_DEST_ADDR_MAX_NUM -  windata_p->dest_info.dest_list.number;

            MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_CHECK, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_NUMBER,
                max_selected_num,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                EditSmsNumSelectContactCallback
            );
            break;

        case MSG_CTL_CANCEL:
            if (!(GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, FALSE)) &&
                    (edit_string.wstr_len == 0))
            {
                // close the edit window
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
            }
            break;

        case MSG_PROMPTWIN_OK:
            MMK_CloseWin(win_id);
            break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;

        case MSG_APP_RED:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}
#else
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :sms edit win
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSmsWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_text_id = MMISMS_EDIT_SMS_TEXT_CTRL_ID;
    MMI_STRING_T edit_string = {0};
// no use
#if defined(MMI_TIMERMSG_SUPPORT) || defined(MMI_SMS_FONT_SETTING_SUPPORT)
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
#endif
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    uint16 max_selected_num = 0;
//#endif
// no use
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T font_group_id = MENU_SMS_FONT_MENU_OPT;
    MMISMS_FONT_E font_size = MMISMS_GetFontSize();/*lint !e64 */
#endif
    uint16  v_space = MMISMS_EDIT_TEXT_NUM_SPACE;
    MMISMS_EDITSMSWIN_DATA_T*   windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinUserData(win_id);

    //begin 20101202
#ifdef MAINLCD_SIZE_128X64
    GUI_RECT_T client_rect = {0, 15, 127, 30};
#endif
    //end 20101202

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:

            windata_p->content_handle =  MMK_GetCtrlHandleByWin(win_id, MMISMS_EDIT_SMS_TEXT_CTRL_ID);
            windata_p->number_handle = MMK_GetCtrlHandleByWin(win_id, MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);

            //form child不显示背景
            GUIFORM_PermitChildBg(MMISMS_EDITWIN_EDITCTRL_ID, FALSE);
            GUIFORM_PermitChildFont(MMISMS_EDITWIN_EDITCTRL_ID, FALSE);

            GUIFORM_PermitChildBg(MMISMS_EDIT_SMS_NUM_CTRL_ID, FALSE);

            //edit处理红键消息
            GUIEDIT_SetHandleRedKey(FALSE, MMISMS_EDIT_SMS_TEXT_CTRL_ID);
            GUIEDIT_SetHandleRedKey(FALSE, MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);

            //FORM控件不显示边框
            GUIEDIT_PermitBorder(ctrl_text_id, FALSE);
            GUIEDIT_PermitBorder(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, FALSE);

            //edit设置线宽
            GUIEDIT_SetDividingLine(ctrl_text_id, 1, MMI_DARK_GRAY_COLOR);
            GUIEDIT_SetDividingLine(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, 1, MMI_DARK_GRAY_COLOR);

            //设置号码框和文本框的相对位置根据语言自动调整
            GUIFORM_IsAutoDir(MMISMS_EDIT_SMS_NUM_CTRL_ID, TRUE);

            //设置号码框和文本框的间距
            GUIFORM_SetChildSpace(MMISMS_EDITWIN_EDITCTRL_ID, MMISMS_EDIT_SMS_TEXT_CTRL_ID, PNULL, &v_space);

            //set edit height
            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
            GUIFORM_SetChildHeight(MMISMS_EDITWIN_EDITCTRL_ID, MMISMS_EDIT_SMS_TEXT_CTRL_ID, &child_height);

#ifndef MMI_PDA_SUPPORT
            GUILABEL_SetIcon(MMISMS_EDIT_SMS_SENDTO_CTRL_ID, IMAGE_SMS_EDIT_TO);
#endif

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
            if (MMISMS_GetAutoSignatureFlag())
            {
                GUIFORM_SetChildDisplay(MMISMS_EDITWIN_EDITCTRL_ID,
                                        MMISMS_SMS_SINATURE_CTRL_ID,
                                        GUIFORM_CHILD_DISP_NORMAL);

                GUIEDIT_SetHandleRedKey(FALSE, MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID);

                GUIEDIT_PermitBorder(MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID, FALSE);
                //edit设置线宽
                GUIEDIT_SetDividingLine(MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID, 1, MMI_DARK_GRAY_COLOR);

                //设置号码框和文本框的间距
                GUIFORM_SetChildSpace(MMISMS_SMS_SINATURE_CTRL_ID, MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID,
                                      PNULL, PNULL);

                //set edit height
                child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
                GUIFORM_SetChildHeight(MMISMS_SMS_SINATURE_CTRL_ID, MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID, &child_height);
            }
            else
            {
                GUIFORM_SetChildDisplay(MMISMS_EDITWIN_EDITCTRL_ID,
                                        MMISMS_SMS_SINATURE_CTRL_ID,
                                        GUIFORM_CHILD_DISP_HIDE);
            }
#endif

#ifdef MMI_TIMERMSG_SUPPORT
            //设置收件人图标和号码框的比例
            child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            child_width.add_data = MMISMS_EDIT_CONTACT_ICON_WIDTH;
            GUIFORM_SetChildWidth(MMISMS_TIMER_SMS_CTRL_ID, MMISMS_EDIT_TIMER_CTRL_ID, &child_width);
            GUILABEL_SetIcon(MMISMS_EDIT_TIMER_CTRL_ID, IMAGE_SMS_TIMER_ICON);
            SetTimerMsgStartLabelParam();
            SetTimerMsgFreqDateDisplay();
#endif

            //设置收件人图标和号码框的比例

            child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            if (MMITHEME_IsMainScreenLandscape())
            {
                child_width.add_data = MMISMS_EDIT_CONTACT_ICON_WIDTH_H;
            }
            else
            {
                child_width.add_data = MMISMS_EDIT_CONTACT_ICON_WIDTH;
            }
            GUIFORM_SetChildWidth(MMISMS_EDIT_SMS_NUM_CTRL_ID, MMISMS_EDIT_SMS_SENDTO_CTRL_ID, &child_width);
#ifdef MMI_PDA_SUPPORT
            {
                GUI_BG_T bg = {0};
                bg.bg_type = GUI_BG_IMG;
                bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
                GUIBUTTON_SetBg(MMISMS_EDIT_SMS_SENDTO_CTRL_ID, &bg);

                bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_PRESSED_BG;
                GUIBUTTON_SetPressedBg(MMISMS_EDIT_SMS_SENDTO_CTRL_ID, &bg);

                bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
                GUIBUTTON_SetFg(MMISMS_EDIT_SMS_SENDTO_CTRL_ID, &bg);

                bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
                GUIBUTTON_SetPressedFg(MMISMS_EDIT_SMS_SENDTO_CTRL_ID, &bg);
                GUIBUTTON_SetRunSheen(MMISMS_EDIT_SMS_SENDTO_CTRL_ID, FALSE);
                GUIBUTTON_PermitBorder(MMISMS_EDIT_SMS_SENDTO_CTRL_ID,  FALSE);
            }
#endif
            AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            GUIEDIT_SetFont(ctrl_text_id, MMISMS_GetWinFont());
            GUIEDIT_SetFont(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, MMISMS_GetWinFont());
#endif
    
            MMISMS_SetEditTextProperty(ctrl_text_id);            
            GUIEDIT_SetString(windata_p->content_handle, windata_p->edit_string,windata_p->edit_strlen);
            
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
            /*auto signature*/
            if (MMISMS_GetAutoSignatureFlag())
            {
                wchar signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
                uint8 signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;

                signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr);

                if (MMISMS_BOX_SENDSUCC != MMISMS_GetCurBoxType() && MMISMS_BOX_SENDFAIL != MMISMS_GetCurBoxType())
                {
                    GUIEDIT_SetString(MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID, signature_wstr, signature_str_len);
                    //                GUIEDIT_SetMaxLen(ctrl_text_id, MMISMS_MAX_MESSAGE_LEN - signature_str_len - 1);
                }
            }
#endif

#ifndef MMI_PDA_SUPPORT
            GUIEDIT_SetSoftkey(ctrl_text_id, 1, 0, TXT_SEND, TXT_NULL, PNULL);
            GUIEDIT_SetSoftkey(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, 1, 0, STXT_MAIN_PB, TXT_NULL, PNULL);
#endif

            if ( windata_p->dest_info.dest_list.number > 0)
            {
                MMK_SetAtvCtrl(win_id, windata_p->content_handle);
            }
            else
            {
                MMK_SetAtvCtrl(win_id, windata_p->number_handle);
            }

#ifdef MAINLCD_SIZE_128X64
            //begin 20101202
            GUIEDIT_SetFontColor(MMISMS_EDIT_SMS_TEXT_CTRL_ID, MMI_BLACK_COLOR);
            GUIEDIT_SetFontColor(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, MMI_BLACK_COLOR);
            GUIEDIT_SetStyle(MMISMS_EDIT_SMS_TEXT_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetFont(MMISMS_EDIT_SMS_TEXT_CTRL_ID, SONG_FONT_10);
            GUIEDIT_SetFont(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, SONG_FONT_10);

            GUIEDIT_SetRect(MMISMS_EDIT_SMS_TEXT_CTRL_ID, &client_rect);
            GUIEDIT_SetRect(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, &client_rect);
            //end 20101202
#endif
            break;
        case MSG_GET_FOCUS:
#ifdef MAINLCD_SIZE_128X64
            GUIEDIT_SetStyle(MMISMS_EDIT_SMS_TEXT_CTRL_ID, GUIEDIT_STYLE_SINGLE);
#endif

            break;

        case MSG_FULL_PAINT:
            {            
                MMI_CTRL_ID_T focus_ctrl_id = MMK_GetActiveCtrlId(win_id);
                if (MMISMS_EDIT_SMS_PHONENUM_CTRL_ID != focus_ctrl_id)
                {
                    GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);
                    AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
                }
            }
#ifdef MMI_TIMERMSG_SUPPORT
            windata_p->is_timer_msg = MMISMS_GetIsTimerMode();
#endif
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case  MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T     ctrl_id = 0;
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if (ctrl_id == MMISMS_EDIT_SMS_SENDTO_CTRL_ID)
            {
                GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);
                max_selected_num = MMISMS_DEST_ADDR_MAX_NUM -  windata_p->dest_info.dest_list.number;
                AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
                /*MMIAPIPB_OpenNumberList(
                MMIPB_LIST_FOR_SMS,
                MMIPB_LIST_CHECK,
                max_selected_num,
                PNULL
                );*/
                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    max_selected_num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSmsSelectContactCallback
                );
            }
            else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                // to open the receiver window
                // to get the content which the user input

                BuildSendData(windata_p);

                // open the input number window
                if (0 < windata_p->dest_info.dest_list.number)
                {
                    EditSMSWin_SendSMS(windata_p);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }
            }
#ifdef MMI_TIMERMSG_SUPPORT            
            else if(ctrl_id == MMISMS_EDIT_TIMER_CTRL_ID)
            {   
                if (MMISMS_GetIsTimerMode())
                {
                    BuildSendData(windata_p);
                    MMISMS_EnterTimerMsgMainWin();                              
                }
            }
#endif            
        }
        break;
#endif
#ifndef MMI_PDA_SUPPORT
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_SK_SKB_ICON_PRESSED:
        {
            MMI_CTRL_ID_T focus_ctrl_id = MMK_GetActiveCtrlId(win_id);
            if (MMISMS_EDIT_SMS_TEXT_CTRL_ID == focus_ctrl_id)
            {
                BuildSendData(windata_p);

                // open the input number window
                if (0 <  windata_p->dest_info.dest_list.number)
                {
                    windata_p->dual_sys = MMISMS_GetDestDualSys();
                    EditSMSWin_SendSMS(windata_p);
                }
                else
                {
                    uint16 num_count = 0;
                    num_count = GUIEDIT_GetListItemNum(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);
                    if (num_count == 0)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                    }
                }
            }
            else if (MMISMS_EDIT_SMS_PHONENUM_CTRL_ID == focus_ctrl_id)
            {
                GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);
                max_selected_num = MMISMS_DEST_ADDR_MAX_NUM -  windata_p->dest_info.dest_list.number;
                AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    max_selected_num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSmsSelectContactCallback
                );
            }
        }
        break;
#endif

        case MMI_SELECT_CONTACT_CNF:
            if (windata_p->s_is_add_contact)
            {
                AddContactInfo2Editbox((MMIPB_HANDLE_T)param, windata_p->content_handle);
                MMK_SetAtvCtrl(win_id, windata_p->content_handle);
                windata_p->s_is_add_contact = FALSE;
            }
            else
            {
                // from PHONEBOOK, call the API to get the phone number.
                if (GetNumberFromPb(&windata_p->dest_info, (MMIPB_HANDLE_T)param, msg_id))
                {
                    // update the edit box
                    AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
                    MMK_SetAtvCtrl(win_id, windata_p->number_handle);
                }
            }
            break;
#ifdef MMI_PDA_SUPPORT
        case MMI_CL_MULTI_SELECT_LIST_RETURN:
            if (MMISMS_GetNumberFromCalllog(msg_id))
            {
                // update the edit box
				SCI_MEMCPY(&windata_p->dest_info,&g_mmisms_global.edit_content.dest_info,sizeof(MMISMS_DEST_ADDR_INFO_T));
				
                AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
                MMK_SetAtvCtrl(win_id, windata_p->number_handle);
            }
            break;
#endif
        case MMISMS_MSG_UPDATE_NUMBER:
            AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
            break;

#ifdef MMI_TIMERMSG_SUPPORT
        case MMISMS_MSG_UPDATE_TIMER:
            SetTimerMsgLabelParam();
            SetTimerMsgFreqDateDisplay();
            break;
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
            MMK_CreateWin((uint32 *)MMISMS_EDITOPTION_WIN_TAB, windata_p);

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            switch (font_size)
            {
                case MMISMS_FONT_BIG:
                    menu_id = MMISMS_MENU_BIG_FONT_ITEM_ID;
                    break;

                case MMISMS_FONT_NORMAL:
                    menu_id = MMISMS_MENU_NORMAL_FONT_ITEM_ID;
                    break;

                case MMISMS_FONT_SMALL:
                    menu_id = MMISMS_MENU_SMALL_FONT_ITEM_ID;
                    break;

                default:
                    menu_id = MMISMS_MENU_NORMAL_FONT_ITEM_ID;
                    break;
            }

            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = MMISMS_WRITEOPTION_MENU_CTRL_ID;
            pop_item_info.menu_id = menu_id;
            pop_item_info.group_id = font_group_id;

            GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
#endif

#ifdef MMI_TIMERMSG_SUPPORT
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = MMISMS_WRITEOPTION_MENU_CTRL_ID;
            pop_item_info.menu_id = MMISMS_GetIsTimerMode() ? MMISMS_MENU_ON_ITEM_ID : MMISMS_MENU_OFF_ITEM_ID;
            pop_item_info.group_id = MENU_SMS_TIMER_MESSAGE_MENU_OPT;

            GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
#endif

            break;

        case MSG_CTL_CANCEL:
#ifdef MMI_SMS_MINI_SUPPORT
            // no prompt for the new function continue to write
            MMK_CloseWin(win_id);
#else
            SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
            GUIEDIT_GetString(ctrl_text_id, &edit_string);

            if (!(GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, FALSE)) &&
                    (0 == edit_string.wstr_len))
            {
                // close the edit window
                MMK_CloseWin(win_id);
            }
            else
            {
#if 0  // for CR126694
                MMIPUB_OpenQueryWinByTextId(TXT_SMS_ASKSAVE_DRAFT, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
#else
                MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
#endif
            }
#endif
            break;

        case MSG_PROMPTWIN_OK:      //TXT_EXIT_WITHOUT_SAVE
#if 0  // for CR126694
            EditSMSWin_SaveSMS(windata_p);
#endif
            MMIPUB_CloseQuerytWin(PNULL);            
            MMK_CloseWin(win_id);
            break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);  
#if 0  // for CR126694
            MMK_CloseWin(win_id);
#endif
            break;

        case MSG_APP_RED:
            SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
            GUIEDIT_GetString(ctrl_text_id, &edit_string);
            
            if (GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, FALSE) ||  
            (edit_string.wstr_len > 0))
            {
                EditSMSWin_SaveSMS(windata_p);
            }
            recode = MMI_RESULT_FALSE;
            //MMK_CloseWin(win_id);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMode())
            {
                GUI_RECT_T  form_rect = {0};
                MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
                SCI_MEMCPY(&para, (MMI_MULTI_KEY_TP_MSG_PARAM_T*)param, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
                GUICTRL_GetRect( MMK_GetCtrlPtr(MMISMS_TIMER_SMS_CTRL_ID), &form_rect);
                if (GUI_PointIsInRect(para.cur_tp_point, form_rect))
                {
                    BuildSendData(windata_p);
                    MMISMS_EnterTimerMsgMainWin();
                }
            }
#endif
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_CLOSE_WINDOW:
#ifdef MMI_SMS_MINI_SUPPORT
            SCI_MEMSET( &edit_string, 0, sizeof(MMI_STRING_T));
            GUIEDIT_GetString(ctrl_text_id, &edit_string);

            MMISMS_SaveStoredMsgdBuffer(edit_string);
#endif
            if (windata_p !=PNULL)
            {
                SCI_FREE(windata_p);
            }                

            break;

#ifdef MMI_SMS_SYN_SUPPORT
	case MSG_BT_SPP_SEND_ITEM:
		{ 
			uint8 sms_data[MMISMS_MAX_MESSAGE_LEN] = {65, 84, 77, 83, 69, 44};
			uint8 sms_content[MMISMS_MAX_MESSAGE_LEN];
			uint8 sms_addr[11] = {0};
			GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr = PNULL;
			uint8 bt_num_count = 0;
			uint8 i = 0;
			uint32 data_len = 0;
			uint8 bt_end[2] = {13, 10};
			uint8 qian_byte = 0;
			uint8 bai_byte = 0;
			uint8 ge_byte =0;
			
			SCI_TRACE_LOW("[MMISMS] case MSG_BT_SPP_SEND_ITEM");
			
			bt_num_count = GUIEDIT_GetListItemNum(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);
			
			if (0 != bt_num_count)
			{
				num_str_ptr = SCI_ALLOC_APP(bt_num_count*sizeof(GUIEDIT_LIST_ITEM_STR_T));
				
				if (PNULL != num_str_ptr)
				{
					GUIEDIT_GetListItemString(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID,bt_num_count,num_str_ptr);
					
					for(i = 0; i < num_str_ptr->str_len; i++)
					{
						sms_addr[i] = ((uint8 *)num_str_ptr->str_ptr)[i * 2];
					}
				} 
				else
				{
					SCI_TRACE_LOW("Alloc mem failed");
					
					break;
				}
				
				
			} 
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
				
				MMK_CloseWin(win_id);
				
				break;
			}
			
			GUIEDIT_GetString(ctrl_text_id, &edit_string);
			
			SCI_TRACE_LOW("MMISMS: content_len = %d", edit_string.wstr_len);
			
			if (0 == edit_string.wstr_len)
			{
				MMIPUB_OpenAlertWarningWin(TXT_SMS_EMPTY_CONTENT);
				
				SCI_FREE(num_str_ptr);
				
				MMK_CloseWin(win_id);
				
				break;
			}
			
			sms_content[0] = 44;
			
			if (edit_string.wstr_len >= 100)
			{
				qian_byte = edit_string.wstr_len/100;
				bai_byte = edit_string.wstr_len/10%10;
				ge_byte = edit_string.wstr_len%10;
				
				sms_content[1] = qian_byte + 48;
				sms_content[2] = bai_byte + 48;
				sms_content[3] = ge_byte + 48;
				sms_content[4] = 44;
				
			}
			else if (edit_string.wstr_len >= 10)
			{
				bai_byte = edit_string.wstr_len/10;
				ge_byte = edit_string.wstr_len%10;
				
				sms_content[1] = bai_byte + 48;
				sms_content[2] = ge_byte + 48;
				sms_content[3] = 44;
			}
			else
			{
				sms_content[1] = edit_string.wstr_len + 48;
				sms_content[2] = 44;
			}
			
			
			//compose a package include command, number, content length, content and end flag
			
			
			if (edit_string.wstr_len >= 100)
			{
				SCI_MEMCPY(&sms_content[5], (uint8 *)edit_string.wstr_ptr, edit_string.wstr_len*2);
			}
			else if (edit_string.wstr_len >= 10)
			{
				SCI_MEMCPY(&sms_content[4], (uint8 *)edit_string.wstr_ptr, edit_string.wstr_len*2);
			}
			else
			{
				SCI_MEMCPY(&sms_content[3], (uint8 *)edit_string.wstr_ptr, edit_string.wstr_len*2);
			}
			
			
			SCI_MEMCPY(&sms_data[6], sms_addr, i);
			
			if (edit_string.wstr_len >= 100)
			{
				SCI_MEMCPY(&sms_data[6 + i], sms_content, edit_string.wstr_len * 2 + 5);
				
				data_len = edit_string.wstr_len * 2 + 5 + num_str_ptr->str_len + 6;
			}
			else if (edit_string.wstr_len >= 10)
			{
				SCI_MEMCPY(&sms_data[6 + i], sms_content, edit_string.wstr_len * 2 + 4);
				
				data_len = edit_string.wstr_len * 2 + 4 + num_str_ptr->str_len + 6;
			}
			else
			{
				SCI_MEMCPY(&sms_data[6 + i], sms_content, edit_string.wstr_len * 2 + 3);
				
				data_len = edit_string.wstr_len * 2 + 3 + num_str_ptr->str_len + 6;
			}    
			
			SCI_MEMCPY(&sms_data[data_len], bt_end, 2);
			
			data_len = data_len + 2;
			
			SCI_TRACE_LOW("MMISMS: Bt Send, length = %d", data_len);
			
			if(MMIAPIBT_IsConnectDevice())
			{
				MMI_STRING_T    prompt_str = {0};
				
				SCI_TRACE_LOW("MMISMS: BT is Connecting");
				
				MMISMS_SendSmsByBtSend(sms_data, data_len); 
				
				MMI_GetLabelTextByLang(TXT_SENDING, &prompt_str);
				
				MMIAPISMS_CreateSmsSyncTimer();
				MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_SPP);
				
				MMIPUB_OpenWaitWin(3, PNULL, &prompt_str, PNULL, MMISMS_SEND_ANIM_WIN_ID,IMAGE_NULL,
					ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER, PNULL);
			}
			else
			{
				SCI_TRACE_LOW("MMISMS: BT is disconnected");
			}
			
			SCI_FREE(num_str_ptr);
			
			MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
            }
			
            break;
#endif
            
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        
    case MSG_BT_SEND_ITEM:
            {
                MMISMS_BtSendItem(FALSE);
            }

            break;
    case MSG_BT_MSG_SYNC_CNF:
        {
            SCI_TRACE_LOW("[MMISMS] MSG_BT_MSG_SYNC_CNF");
            HandleBTMsgSyncCfn(win_id);
        }
        break;
    
    case MSG_BT_MAP_REGISTER_SUCCESS:
        {
            SCI_TRACE_LOW("[MMISMS] push message MSG_BT_MSG_REGISTER_SUCCESS");
            //HandleBTMapRegisterSuccess(win_id);
        }
        break;
        
    case MSG_BT_MAP_CONNECT_SUCCESS:
        {
            SCI_TRACE_LOW("[MMISMS] MSG_BT_MAP_CONNECT_SUCCESS");
            HandleBTMapConnectSuccess(win_id);
        }
        break;

    case MSG_BT_SET_FOLDER_SUCCESS:
        {
            SCI_TRACE_LOW("[MMISMS] MSG_BT_SET_FOLDER_SUCCESS");
            HandleBTMapSetFolderSuccess(win_id);
        }
        break;

    case MSG_BT_PUSH_SUCCESS:
        {
            uint8 count = 0;
            
            SCI_TRACE_LOW("[MMISMS] MSG_BT_PUSH_SUCCESS");
            
            count = MMIAPISMS_GetSendAddrCount();
            if(count > 1)
            {
                MMISMS_CloseMapWaitWin();
                MMIAPISMS_SetSendAddrCount(count - 1);
                
                MMISMS_BtSendItem(FALSE);
                //MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MSG_BT_SEND_ITEM, PNULL);
            }
            else
            {
                MMIAPISMS_SetIsSendMultiSms(FALSE);
                MMIAPISMS_SetSendAddrCount(0);
                MMISMS_CloseMapWaitWin();
                
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT) && defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
                MMIAPISMS_StopSmsSyncTimer();
                MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_NONE);
#endif
                if(MMISMS_BOX_NOSEND == g_mmisms_global.operation.cur_type)
                {
                    MMISMS_DeleteReqToMN(g_mmisms_global.operation.cur_type, MMISMS_GetCurOperationOrderId(), MMISMS_DELETE_MSG);
                    MMISMS_DelBtSmsInfoInOrder(MMISMS_GetCurOperationOrderId());
                    MMISMS_HandleOrderChangeCallBack();
                }
                
                MMIAPISMS_SetIsUpdateOutbox(TRUE);
                MMISMS_CloseEditWin();
            }
        }
        break;
#endif

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}
#else
/*****************************************************************************/
//  Description :sms edit win
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSmsWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_text_id = MMISMS_EDIT_SMS_TEXT_CTRL_ID;
    MMI_STRING_T edit_string = {0};
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    MMI_CTRL_ID_T focus_ctrl_id = MMK_GetActiveCtrlId(win_id);
    uint16 max_selected_num = 0;
    // MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;
    // MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    uint16 num_count = 0;
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T font_group_id = MENU_SMS_FONT_MENU_OPT;
    MMISMS_FONT_E font_size = MMISMS_GetFontSize();/*lint !e64 */
#endif
    uint16  v_space = MMISMS_EDIT_TEXT_NUM_SPACE;

    MMISMS_EDITSMSWIN_DATA_T*   windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinUserData(win_id);
    //begin 20101202
#ifdef MAINLCD_SIZE_128X64
    GUI_RECT_T client_rect = {0, 15, 127, 30};
#endif
    //end 20101202

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            windata_p->content_handle =  MMK_GetCtrlHandleByWin(win_id, MMISMS_EDIT_SMS_TEXT_CTRL_ID);
            windata_p->number_handle = MMK_GetCtrlHandleByWin(win_id, MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);

            //form child不显示背景
            GUIFORM_PermitChildBg(MMISMS_EDITWIN_EDITCTRL_ID, FALSE);
            GUIFORM_PermitChildFont(MMISMS_EDITWIN_EDITCTRL_ID, FALSE);

            //edit处理红键消息
            GUIEDIT_SetHandleRedKey(FALSE, MMISMS_EDIT_SMS_TEXT_CTRL_ID);
            GUIEDIT_SetHandleRedKey(FALSE, MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);

            //FORM控件不显示边框
            GUIEDIT_PermitBorder(ctrl_text_id, FALSE);
            GUIEDIT_PermitBorder(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, FALSE);

            //edit设置线宽
            GUIEDIT_SetDividingLine(ctrl_text_id, 1, MMI_DARK_GRAY_COLOR);
            GUIEDIT_SetDividingLine(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, 1, MMI_DARK_GRAY_COLOR);

            //设置号码框和文本框的相对位置根据语言自动调整
            GUIFORM_IsAutoDir(MMISMS_EDIT_SMS_NUM_CTRL_ID, TRUE);

            //设置号码框和文本框的间距
            GUIFORM_SetChildSpace(MMISMS_EDITWIN_EDITCTRL_ID, MMISMS_EDIT_SMS_TEXT_CTRL_ID, PNULL, &v_space);

#ifdef MMI_TIMERMSG_SUPPORT
            //设置收件人图标和号码框的比例
            child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            child_width.add_data = MMISMS_EDIT_CONTACT_ICON_WIDTH;
            GUIFORM_SetChildWidth(MMISMS_TIMER_SMS_CTRL_ID, MMISMS_EDIT_TIMER_CTRL_ID, &child_width);
            GUILABEL_SetIcon(MMISMS_EDIT_TIMER_CTRL_ID, IMAGE_SMS_TIMER_ICON);
            SetTimerMsgStartLabelParam();
            SetTimerMsgFreqDateDisplay();
#endif

            //设置收件人图标和号码框的比例
            child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            child_width.add_data = MMISMS_EDIT_CONTACT_ICON_WIDTH;
            GUIFORM_SetChildWidth(MMISMS_EDIT_SMS_NUM_CTRL_ID, MMISMS_EDIT_SMS_SENDTO_CTRL_ID, &child_width);
#ifndef MMI_PDA_SUPPORT
            GUILABEL_SetIcon(MMISMS_EDIT_SMS_SENDTO_CTRL_ID, IMAGE_SMS_EDIT_TO);
#endif

            AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
            GUIEDIT_SetString(windata_p->content_handle, windata_p->edit_string,windata_p->edit_strlen);
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            GUIEDIT_SetFont(ctrl_text_id, MMISMS_GetWinFont());
            GUIEDIT_SetFont(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, MMISMS_GetWinFont());
#endif
            MMISMS_SetEditTextProperty(ctrl_text_id);            
            
#ifndef MMI_PDA_SUPPORT
            GUIEDIT_SetSoftkey(ctrl_text_id, 1, 0, TXT_SEND, TXT_NULL, PNULL);
            GUIEDIT_SetSoftkey(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, 1, 0, STXT_MAIN_PB, TXT_NULL, PNULL);
#endif

            if ( windata_p->dest_info.dest_list.number > 0)
            {
                MMK_SetAtvCtrl(win_id, windata_p->content_handle);
            }
            else
            {
                MMK_SetAtvCtrl(win_id, windata_p->number_handle);
            }


#ifdef MAINLCD_SIZE_128X64
            //begin 20101202

            GUIEDIT_SetFontColor(MMISMS_EDIT_SMS_TEXT_CTRL_ID, MMI_BLACK_COLOR);
            GUIEDIT_SetFontColor(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, MMI_BLACK_COLOR);
            GUIEDIT_SetStyle(MMISMS_EDIT_SMS_TEXT_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetFont(MMISMS_EDIT_SMS_TEXT_CTRL_ID, SONG_FONT_10);
            GUIEDIT_SetFont(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, SONG_FONT_10);

            GUIEDIT_SetRect(MMISMS_EDIT_SMS_TEXT_CTRL_ID, &client_rect);
            GUIEDIT_SetRect(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, &client_rect);
            //end 20101202
#endif            
            break;

        case MSG_GET_FOCUS:
#ifdef MAINLCD_SIZE_128X64        
            GUIEDIT_SetStyle(MMISMS_EDIT_SMS_TEXT_CTRL_ID, GUIEDIT_STYLE_SINGLE);
#endif
#ifdef MMI_TIMERMSG_SUPPORT 
            windata_p->is_timer_msg = MMISMS_GetIsTimerMsg();
#endif
            break;

#ifdef MMI_PDA_SUPPORT
        case  MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T     ctrl_id = 0;
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if (ctrl_id == MMISMS_EDIT_SMS_SENDTO_CTRL_ID)
            {
                GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);
                max_selected_num = MMISMS_DEST_ADDR_MAX_NUM -  windata_p->dest_info.dest_list.number;
                AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);

                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    max_selected_num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSmsSelectContactCallback
                );
            }
            else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                BuildSendData(windata_p);
                // open the input number window
                if (0 <  windata_p->dest_info.dest_list.number)
                {
                    EditSMSWin_SendSMS(windata_p);

                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }
            }
        }
        break;
#endif

        case MMI_SELECT_CONTACT_CNF:
            if (windata_p->s_is_add_contact)
            {
                AddContactInfo2Editbox((MMIPB_HANDLE_T)param, windata_p->content_handle);
                windata_p->s_is_add_contact = FALSE;
            }
            else
            {
                // from PHONEBOOK, call the API to get the phone number.
                if (GetNumberFromPb(&windata_p->dest_info, (MMIPB_HANDLE_T)param, msg_id))
                {
                    // update the edit box
                    AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
                    MMK_SetAtvCtrl(win_id, windata_p->number_handle);
                }
            }
            break;

        case MMISMS_MSG_UPDATE_NUMBER:
            AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);
            break;

#ifdef MMI_TIMERMSG_SUPPORT
        case MMISMS_MSG_UPDATE_TIMER:
            SetTimerMsgLabelParam();
            SetTimerMsgFreqDateDisplay();
            break;
#endif
#ifndef MMI_PDA_SUPPORT
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_SK_SKB_ICON_PRESSED:
            if (MMISMS_EDIT_SMS_TEXT_CTRL_ID == focus_ctrl_id)
            {

                BuildSendData(windata_p);
                // open the input number window
                if (0 <  windata_p->dest_info.dest_list.number)
                {
                    EditSMSWin_SendSMS(windata_p);
                }
                else
                {
                    num_count = GUIEDIT_GetListItemNum(windata_p->number_handle);
                    if (num_count == 0)
                    {
                        //MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                        //Add by huang fengming for cr148999
                        max_selected_num = MMISMS_DEST_ADDR_MAX_NUM - windata_p->dest_info.dest_list.number;

                        MMIAPIPB_OpenMainWin(
                            MMIPB_ALL_CONTACT,//
                            MMIPB_LIST_CHECK, //IN: 号码列表的类型
                            MMIPB_LIST_FIELD_NUMBER,
                            max_selected_num,//如果是多选时，此项限制了多选的最大个数
                            PNULL,
                            EditSmsSelectContactCallback
                        );
                    }
                }
            }
            else
            {
                GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, TRUE);
                max_selected_num = MMISMS_DEST_ADDR_MAX_NUM -  windata_p->dest_info.dest_list.number;
                AddNumber2Edit(&windata_p->dest_info, windata_p->number_handle);

                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    max_selected_num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSmsSelectContactCallback
                );

                //AddNumber2Edit(focus_ctrl_id);
            }
            break;
#endif

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
            MMK_CreateWin((uint32 *)MMISMS_EDITOPTION_WIN_TAB, windata_p);

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            switch (font_size)
            {
                case MMISMS_FONT_BIG:
                    menu_id = MMISMS_MENU_BIG_FONT_ITEM_ID;
                    break;

                case MMISMS_FONT_NORMAL:
                    menu_id = MMISMS_MENU_NORMAL_FONT_ITEM_ID;
                    break;

                case MMISMS_FONT_SMALL:
                    menu_id = MMISMS_MENU_SMALL_FONT_ITEM_ID;
                    break;

                default:
                    menu_id = MMISMS_MENU_NORMAL_FONT_ITEM_ID;
                    break;
            }

            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = MMISMS_WRITEOPTION_MENU_CTRL_ID;
            pop_item_info.menu_id = menu_id;
            pop_item_info.group_id = font_group_id;

            GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
#endif

#ifdef MMI_TIMERMSG_SUPPORT
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = MMISMS_WRITEOPTION_MENU_CTRL_ID;
            pop_item_info.menu_id = MMISMS_GetIsTimerMode() ? MMISMS_MENU_ON_ITEM_ID : MMISMS_MENU_OFF_ITEM_ID;
            pop_item_info.group_id = MENU_SMS_TIMER_MESSAGE_MENU_OPT;

            GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
#endif

            break;

        case MSG_CTL_CANCEL:
            SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
            GUIEDIT_GetString(ctrl_text_id, &edit_string);

            if (!(GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, FALSE)) &&
                    (0 == edit_string.wstr_len))
            {
                // close the edit window
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
            }
            break;

        case MSG_PROMPTWIN_OK:      //TXT_EXIT_WITHOUT_SAVE
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            EditSMSWin_SaveSMS(windata_p);
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_RED:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:            
            if (windata_p !=PNULL)
            {
                SCI_FREE(windata_p);
            }                
        
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}

#endif

#endif


/*****************************************************************************/
//     Description : to handle the message of edit menu win
//    Global resource dependence :
//  Author:louis.wei
//  Modify: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditMenuWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_WRITEOPTION_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    // MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;
//    MMI_CTRL_ID_T ctrl_text_id = MMISMS_EDIT_SMS_TEXT_CTRL_ID;
//    MMI_STRING_T edit_string = {0};
#ifdef MMI_PDA_SUPPORT
    MMI_STRING_T menu_string = {0};
#endif
#ifdef MAINLCD_SIZE_128X64 /* lint_lai */
#else
    uint16 num_count = 0;
//    uint16 max_selected_num = 0;
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
#endif
    MMISMS_EDITSMSWIN_DATA_T*   windata_p = (MMISMS_EDITSMSWIN_DATA_T*)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"MMISMS: HandleEditMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_5478_112_2_18_2_54_31_734, (uint8*)"d", msg_id);

    if (windata_p == PNULL)
    {
        return recode;
    }

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMode())
            {
                MMI_GetLabelTextByLang(TXT_SMS_CLOSE_TIMER_MESSAGE, &menu_string);
                GUIMENU_SetStaticItem(
                    MMISMS_WRITEOPTION_MENU_CTRL_ID,    // [in]
                    MENU_SMS_WRITE_OPT,   // [in]
                    MMISMS_MENU_TIMER_MESSAGE_ITEM_ID,    // [in]
                    &menu_string,  // [in] PNULL表示不设置文本
                    0     // [in] 0或者IMAGE_NULL表示不设置图片
                );
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_SMS_OPEN_TIMER_MESSAGE, &menu_string);
                GUIMENU_SetStaticItem(
                    MMISMS_WRITEOPTION_MENU_CTRL_ID,    // [in]
                    MENU_SMS_WRITE_OPT,   // [in]
                    MMISMS_MENU_TIMER_MESSAGE_ITEM_ID,    // [in]
                    &menu_string,  // [in] PNULL表示不设置文本
                    0     // [in] 0或者IMAGE_NULL表示不设置图片
                );
            }
#endif
#endif

#ifdef MMI_SMS_SYN_SUPPORT
        if(MMIAPIBT_IsConnectDevice())
        {
            MMIAPICOM_EnableGrayed(win_id, MENU_SMS_WRITE_OPT, MMISMS_MENU_BT_SEND_ITEM_ID, FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id, MENU_SMS_WRITE_OPT, MMISMS_MENU_BT_SEND_ITEM_ID, TRUE);
        }
#endif
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        
        if(MMIAPIBT_IsConnectDevice())
        {
            MMIAPICOM_EnableGrayed(win_id, MENU_SMS_WRITE_OPT, MMISMS_MENU_BT_SEND_ITEM_ID, FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id, MENU_SMS_WRITE_OPT, MMISMS_MENU_BT_SEND_ITEM_ID, TRUE);
        }
#endif

            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

#ifdef MMI_PDA_SUPPORT   //关闭菜单
        case MSG_GET_FOCUS:
#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMode())
            {
                MMI_GetLabelTextByLang(TXT_SMS_CLOSE_TIMER_MESSAGE, &menu_string);
                GUIMENU_SetStaticItem(
                    MMISMS_WRITEOPTION_MENU_CTRL_ID,    // [in]
                    MENU_SMS_WRITE_OPT,   // [in]
                    MMISMS_MENU_TIMER_MESSAGE_ITEM_ID,    // [in]
                    &menu_string,  // [in] PNULL表示不设置文本
                    0     // [in] 0或者IMAGE_NULL表示不设置图片
                );
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_SMS_OPEN_TIMER_MESSAGE, &menu_string);
                GUIMENU_SetStaticItem(
                    MMISMS_WRITEOPTION_MENU_CTRL_ID,    // [in]
                    MENU_SMS_WRITE_OPT,   // [in]
                    MMISMS_MENU_TIMER_MESSAGE_ITEM_ID,    // [in]
                    &menu_string,  // [in] PNULL表示不设置文本
                    0     // [in] 0或者IMAGE_NULL表示不设置图片
                );
            }
#endif
            break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
        case MSG_CTL_OK:
        case MSG_APP_OK:
#endif
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            switch (menu_id)
            {

#ifdef MMI_SMS_SYN_SUPPORT
        case MMISMS_MENU_BT_SEND_ITEM_ID:
            { 
		  MMI_STRING_T edit_string = {0};
                uint8 sms_data[MMISMS_MAX_MESSAGE_LEN] = {65, 84, 77, 83, 69, 44};
                uint8 sms_content[MMISMS_MAX_MESSAGE_LEN];
                uint8 sms_addr[11];
                GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr = PNULL;
                uint8 num_count = 0;
                uint8 i = 0;
                uint32 data_len = 0;
                uint8 bt_end[2] = {13, 10};
                uint8 qian_byte = 0;
                uint8 bai_byte = 0;
                uint8 ge_byte =0;
		  num_count = GUIEDIT_GetListItemNum(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);

                if (0 != num_count)
                {
                    num_str_ptr = SCI_ALLOC_APP(num_count*sizeof(GUIEDIT_LIST_ITEM_STR_T));

                    if (PNULL != num_str_ptr)
                    {
                        GUIEDIT_GetListItemString(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID,num_count,num_str_ptr);
                        
                        for(i = 0; i < num_str_ptr->str_len; i++)
                        {
                            sms_addr[i] = ((uint8 *)num_str_ptr->str_ptr)[i * 2];
                        }
                    } 
                    else
                    {
                        SCI_TRACE_LOW("Alloc mem failed");

                        break;
                    }
                    
                    
                } 
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                    
                    MMK_CloseWin(win_id);

                    break;
                }
		  
		  GUIEDIT_GetString(windata_p->content_handle, &edit_string);
                
                SCI_TRACE_LOW("MMISMS: content_len = %d", edit_string.wstr_len);
                
                if (0 == edit_string.wstr_len)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_EMPTY_CONTENT);

                    if (PNULL != num_str_ptr)
                    {
                        SCI_FREE(num_str_ptr);
                    } 
                    
                    MMK_CloseWin(win_id);
                    
                    break;
                }
                
                sms_content[0] = 44;
		  if (edit_string.wstr_len >= 100)
                {
                    qian_byte = edit_string.wstr_len/100;
                    bai_byte = edit_string.wstr_len/10%10;
                    ge_byte = edit_string.wstr_len%10;

                    sms_content[1] = qian_byte + 48;
                    sms_content[2] = bai_byte + 48;
                    sms_content[3] = ge_byte + 48;
                    sms_content[4] = 44;
                    
                }
                else if (edit_string.wstr_len >= 10)
                {
                    bai_byte = edit_string.wstr_len/10;
                    ge_byte = edit_string.wstr_len%10;
                    
                    sms_content[1] = bai_byte + 48;
                    sms_content[2] = ge_byte + 48;
                    sms_content[3] = 44;
                }
                else
                {
                    sms_content[1] = edit_string.wstr_len + 48;
                    sms_content[2] = 44;
                }
		  //compose a package include command, number, content length, content and end flag


                if (edit_string.wstr_len >= 100)
                {
                    SCI_MEMCPY(&sms_content[5], (uint8 *)edit_string.wstr_ptr, edit_string.wstr_len*2);
                }
                else if (edit_string.wstr_len >= 10)
                {
                    SCI_MEMCPY(&sms_content[4], (uint8 *)edit_string.wstr_ptr, edit_string.wstr_len*2);
                }
                else
                {
                    SCI_MEMCPY(&sms_content[3], (uint8 *)edit_string.wstr_ptr, edit_string.wstr_len*2);
                }
                

                SCI_MEMCPY(&sms_data[6], sms_addr, i);

                if (edit_string.wstr_len >= 100)
                {
                    SCI_MEMCPY(&sms_data[6 + i], sms_content, edit_string.wstr_len * 2 + 5);

                    data_len = edit_string.wstr_len * 2 + 5 + num_str_ptr->str_len + 6;
                }
                else if (edit_string.wstr_len >= 10)
                {
                    SCI_MEMCPY(&sms_data[6 + i], sms_content, edit_string.wstr_len * 2 + 4);
                    
                    data_len = edit_string.wstr_len * 2 + 4 + num_str_ptr->str_len + 6;
                }
                else
                {
                    SCI_MEMCPY(&sms_data[6 + i], sms_content, edit_string.wstr_len * 2 + 3);

                    data_len = edit_string.wstr_len * 2 + 3 + num_str_ptr->str_len + 6;
                }    
		  SCI_MEMCPY(&sms_data[data_len], bt_end, 2);

                data_len = data_len + 2;

                SCI_TRACE_LOW("MMISMS: Bt Send, length = %d", data_len);

                if(MMIAPIBT_IsConnectDevice())
                {
                    MMI_STRING_T    prompt_str = {0};

                    SCI_TRACE_LOW("MMISMS: BT is Connecting");

                    MMISMS_SendSmsByBtSend(sms_data, data_len); 
                    
                    MMI_GetLabelTextByLang(TXT_SENDING, &prompt_str);

                    MMIPUB_OpenWaitWin(3, PNULL, &prompt_str, PNULL, MMISMS_SEND_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER, PNULL);
                }
                else
                {
                    SCI_TRACE_LOW("MMISMS: BT is disconnected");
                }

                if (PNULL != num_str_ptr)
                {
                    SCI_FREE(num_str_ptr);
                }               

                MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
            }
	     //
            break;
#endif
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
                case MMISMS_MENU_BT_SEND_ITEM_ID:
                {
                    MMISMS_BtSendItem(FALSE);
                    //MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MSG_BT_SEND_ITEM, PNULL);
                }
                break;
#endif

                case MMISMS_MENU_SIM1_SEND_ITEM_ID:
#ifdef MAINLCD_SIZE_128X64
                    MMK_CreateWin((uint32 *)MMISMS_EDIT_SMS_NUMBER_WIN_TAB, windata_p);
#else
                    BuildSendData(windata_p);
                    // open the input number window
                    if (0 <  windata_p->dest_info.dest_list.number)
                    {
                        EditSMSWin_SendSMS(windata_p);
                    }
                    else
                    {
                        num_count = GUIEDIT_GetListItemNum(windata_p->number_handle);
                        if (num_count == 0)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                        }
                    }
#endif
                    break;

                case MMISMS_MENU_SAVE_AS_DRAFT_ITEM_ID:    // save the message to draft box
                    EditSMSWin_SaveSMS(windata_p);
                    MMK_CloseWin(windata_p->win_handle);
                    break;

                case MMISMS_MENU_ADD_CONTACTINFO_ITEM_ID:
                    //SCI_TRACE_LOW:"MMISMS: HandleEditMenuWinMsg call MMIAPIPB_OpenNumberList"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_5747_112_2_18_2_54_31_735, (uint8*)"");
                    MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_RADIO, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        1,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        //EditSelectContactCallback
                        EditSmsSelectContactCallback
                    );
                    windata_p->s_is_add_contact = TRUE;

                    break;

                case MMISMS_MENU_ADD_PHIZ_ITEM_ID:
                    MMISMS_OpenPhizWin(MMISMS_EDIT_SMS_TEXT_CTRL_ID);
                    break;
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
                case MMISMS_MENU_INPUTMETHOD_SETTING_ITEM_ID:
                    MMIAPISET_GetLanguageType(&lang_type);
                    {
#if defined(MMIIM_USE_KB_SOGOU)
                        MMIAPIIM_SGGetImeSkin();
#endif
                        MMIAPIIM_OpenSogouIMSetInputModeWin();
                    }
                    break;
#endif

                case MMISMS_MENU_OPT_FREQWORD_ITEM_ID:
                    MMISMS_CreateSelectWordWin(windata_p->content_handle);
                    break;

#ifdef SMS_EXTRACT_URLORST_SUPPORT   //No Use Menu
                case MMISMS_MENU_EXTRACT_URL:
                case MMISMS_MENU_EXTRACT_STADDR:
                {
                    uint32 total_num = 0;
                    MMI_TEXT_ID_T text_id = TXT_NULL;

                    if (MMISMS_MENU_EXTRACT_URL == menu_id)
                    {
                        total_num = MMISMS_ExtractURLOrST(TRUE);
                        text_id = TXT_SMS_NO_URL;
                    }
                    else
                    {
                        total_num = MMISMS_ExtractURLOrST(FALSE);
                        text_id = TXT_SMS_NO_STADDR;
                    }
                    if (0 < total_num)
                    {
                        MMK_CreateWin((uint32 *)MMISMS_EXTRACT_URLORST_LIST_TAB, PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(text_id);
                    }
                }
                break;
#endif

#ifdef MMI_TIMERMSG_SUPPORT
#ifdef MMI_PDA_SUPPORT
                case MMISMS_MENU_TIMER_MESSAGE_ITEM_ID:
                    if (MMISMS_GetIsTimerMode())
                    {
                        MMISMS_SetIsTimerMode(FALSE);
                        MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                    }
                    else
                    {
                        BuildSendData(windata_p);
                        MMISMS_EnterTimerMsgMainWin();
                    }
                    break;
#endif
                case MMISMS_MENU_ON_ITEM_ID:
                    BuildSendData(windata_p);
                    MMISMS_EnterTimerMsgMainWin();
                    break;

                case MMISMS_MENU_OFF_ITEM_ID:
                    MMISMS_SetIsTimerMode(FALSE);
                    MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                    break;
#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
                case MMISMS_MENU_BIG_FONT_ITEM_ID:
                    MMISMS_SetFontSize( MMISMS_FONT_BIG );
                    break;

                case MMISMS_MENU_NORMAL_FONT_ITEM_ID:
                    MMISMS_SetFontSize( MMISMS_FONT_NORMAL );
                    break;

                case MMISMS_MENU_SMALL_FONT_ITEM_ID:
                    MMISMS_SetFontSize( MMISMS_FONT_SMALL);
                    break;
#endif

                case MMISMS_MENU_EXIT_ITEM_ID:
                    BuildSaveData(windata_p);
                    if (!(GetNumberFromEditbox(&windata_p->dest_info, windata_p->number_handle, FALSE)) &&
                            (windata_p->edit_strlen== 0))
                    {
                        // close the edit window                         
                        MMK_CloseWin(windata_p->win_handle);                        
                    }
                    else
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
                    }
                    break;

                default:
                    break;
            }
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //case MSG_LOSE_FOCUS:
            // close the option window
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/** -------------------------------------------------------------------------*/
//                          LOCAL FUNCTIONS APIS                               //
/** -------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : add the contact information to the SMS edit window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddContactInfo2Editbox(MMIPB_HANDLE_T handle, MMI_HANDLE_T    ctrl_handle)
{
    MMI_STRING_T contact_str = {0};
    BOOLEAN is_right = TRUE;

    contact_str.wstr_ptr = SCI_ALLOC_APP((MMISMS_MAX_MESSAGE_LEN + 1) * sizeof(wchar));
    SCI_MEMSET(contact_str.wstr_ptr, 0, ((MMISMS_MAX_MESSAGE_LEN + 1)*sizeof(wchar)));

    // get the detail information from the phone_book
    MMISMS_GetContactInfo((uint32)handle,
                          &contact_str,
                          MMISMS_MAX_MESSAGE_LEN
                         );

    is_right = GUIEDIT_InsertString(
                   ctrl_handle,
                   contact_str.wstr_ptr,
                   contact_str.wstr_len
               );
    if (!is_right)
    {
        // prompt the error information
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
    }

    if (PNULL != contact_str.wstr_ptr)
    {
        SCI_FREE(contact_str.wstr_ptr);
        contact_str.wstr_ptr = PNULL;
    }
}

/*****************************************************************/
//  Description :短信编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);
}



/*****************************1************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************1************************************************/
LOCAL BOOLEAN GetNumberFromEditbox(MMISMS_DEST_ADDR_INFO_T* dest_info,
                                   MMI_CTRL_ID_T ctrl_id,     //IN:
                                   BOOLEAN  is_delete_invalid
                                  )
{
    BOOLEAN result = FALSE;
    uint8 dest_addr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint16 num_count = 0;
    uint16 i = 0;
    GUIEDIT_LIST_ITEM_STR_T *num_str_ptr = PNULL;

    if (dest_info == PNULL)
    {
        return result;
    }
    //MMISMS_ClearDestNum();
    SCI_MEMSET(dest_info, 0, sizeof(MMISMS_DEST_ADDR_INFO_T));

    //get number count
    num_count = GUIEDIT_GetListItemNum(ctrl_id);
    if (0 < num_count)
    {
        //alloc
        num_str_ptr = SCI_ALLOC_APP(num_count * sizeof(GUIEDIT_LIST_ITEM_STR_T));

        //get all phone number
        GUIEDIT_GetListItemString(ctrl_id, num_count, num_str_ptr);

        //set dest
        for (i = 0; i < num_count; i++)
        {
            if (MMISMS_PBNUM_MAX_LEN < num_str_ptr[i].str_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
            }
            else if (!is_delete_invalid)
            {
                if (num_str_ptr[i].is_valid)
                {
                    if (MMISMS_DEST_ADDR_MAX_NUM >= num_count)
                    {
                        SCI_MEMSET(dest_addr, 0, sizeof(dest_addr));
                        MMI_WSTRNTOSTR((uint8 *)dest_addr, MMISMS_PBNUM_MAX_LEN + 2,
                                       num_str_ptr[i].str_ptr, num_str_ptr[i].str_len,
                                       num_str_ptr[i].str_len);

                        MMISMS_SetAddressToMessage((uint8 *)dest_addr, (uint8)strlen((char *)dest_addr), &dest_info->dest_list);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_INVALID_NUMBER);
                }
            }
            else
            {
                if (MMISMS_DEST_ADDR_MAX_NUM >= num_count)
                {
                    SCI_MEMSET(dest_addr, 0, sizeof(dest_addr));
                    MMI_WSTRNTOSTR((uint8 *)dest_addr, MMISMS_PBNUM_MAX_LEN + 2,
                                   num_str_ptr[i].str_ptr, num_str_ptr[i].str_len,
                                   num_str_ptr[i].str_len);

                    MMISMS_SetAddressToMessage((uint8 *)dest_addr, (uint8)strlen((char *)dest_addr), &dest_info->dest_list);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
                }
            }
        }
        SCI_FREE(num_str_ptr);

        result = TRUE;
    }

    return (result);
}


/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNumberFromPb(MMISMS_DEST_ADDR_INFO_T* dest_info,
                              MMIPB_HANDLE_T handle,
                              MMI_MESSAGE_ID_E    msg_id    //IN:
                             )
{
    int16 i = 0;
    uint16 count = MMIAPIPB_GetSelectCount(handle);
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    BOOLEAN result = TRUE;
    MMIPB_BCD_NUMBER_T    number = {0};

    //SCI_TRACE_LOW:"MMISMS: GetNumberFromPb msg_id = 0x%x, count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_17616_112_2_18_2_54_57_791, (uint8*)"dd", msg_id, count);

    if (dest_info== PNULL)
    {
        return FALSE;
    }

    if ((MMI_SELECT_CONTACT_CNF == msg_id))
    {
        if (MMISMS_DEST_ADDR_MAX_NUM < count)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
        }
        for (i = 0; i < MIN(MMISMS_DEST_ADDR_MAX_NUM, count); i++)
        {
            SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));

            // read the phone number from phonebook one by one
            MMIAPIPB_GetSelectNumber(handle, &number, MMIPB_BCD_NUMBER_MAX_LEN, i);

            max_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, number.number_len);

            tele_len = MMIAPICOM_GenDispNumber(
                           MMIPB_GetNumberTypeFromUint8(number.npi_ton),
                           max_len,
                           number.number,
                           tele_num,
                           MMISMS_NUMBER_MAX_LEN + 2
                       );

            MMISMS_SetAddressToMessage(tele_num, tele_len, &dest_info->dest_list);
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//     Description : load number edit box
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddNumber2Edit(MMISMS_DEST_ADDR_INFO_T* dest_info,
                          MMI_CTRL_ID_T    ctrl_id
                         )
{
    uint8 num_count = 0;
    if (dest_info == PNULL)
    {
        return;
    }
    num_count = dest_info->dest_list.number;

    //显示目的号码信息
    if (0 < num_count)
    {
        uint16   i = 0;
        uint16   actual_count = 0;
        wchar   *number_wstr_ptr = PNULL;
        GUIEDIT_LIST_ITEM_T *list_item_ptr = PNULL;

        GUIEDIT_ClearAllStr(ctrl_id);

        list_item_ptr = SCI_ALLOC_APP(sizeof(GUIEDIT_LIST_ITEM_T) * num_count);
        SCI_MEMSET(list_item_ptr, 0, (sizeof(GUIEDIT_LIST_ITEM_T) * num_count));

        for (i = 0; i < num_count; i++)
        {
            uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
            uint8 num_len = 0;
            MMIPB_BCD_NUMBER_T black_bcd_num = {0};
            MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
            BOOLEAN find_result = FALSE;
            wchar *number_name_wstr = PNULL;
            MMI_STRING_T number_name_addr = {0};

            SCI_MEMCPY( number, dest_info->dest_list.dest_addr[i], MMISMS_PBNUM_MAX_LEN + 1);
            num_len = strlen((char*)number);
            if (num_len > 0)
            {
                number_wstr_ptr = SCI_ALLOC_APP(((MMISMS_PBNUM_MAX_LEN + 2) + 1) * sizeof(wchar));
                SCI_MEMSET(number_wstr_ptr, 0x00, (((MMISMS_PBNUM_MAX_LEN + 2) + 1) * sizeof(wchar)));
                MMI_STRNTOWSTR(number_wstr_ptr, (MMISMS_PBNUM_MAX_LEN + 2),
                               (const uint8 *)number, (MMISMS_PBNUM_MAX_LEN + 2), num_len);
                MMIAPICOM_GenPartyNumber(number, num_len, &party_num);
                black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
                number_name_wstr = SCI_ALLOC_APP((MMIPB_NAME_MAX_LEN  + 1) * sizeof(wchar));
                SCI_MEMSET(number_name_wstr, 0x00, ((MMIPB_NAME_MAX_LEN + 1) * sizeof(wchar)));
                number_name_addr.wstr_ptr = number_name_wstr;
                find_result = MMIAPIPB_GetNameByNumber(&black_bcd_num, &number_name_addr, MMIPB_NAME_MAX_LEN, FALSE);

                if (find_result && 0 < number_name_addr.wstr_len)
                {
                    //如果匹配名称，则显示名称，不能够修改了
                    list_item_ptr[actual_count].is_object = TRUE;

                    list_item_ptr[actual_count].display_str_ptr = number_name_addr.wstr_ptr;
                    list_item_ptr[actual_count].display_str_len = number_name_addr.wstr_len;
                    list_item_ptr[actual_count].user_str_ptr = number_wstr_ptr;
                    list_item_ptr[actual_count].user_str_len = num_len;
                }
                else
                {
                    list_item_ptr[actual_count].is_object = FALSE;
                    list_item_ptr[actual_count].display_str_ptr = number_wstr_ptr;
                    list_item_ptr[actual_count].display_str_len = num_len;
                    list_item_ptr[actual_count].user_str_ptr = PNULL;
                    list_item_ptr[actual_count].user_str_len = 0;
                    SCI_FREE(number_name_wstr);
                    number_name_wstr = PNULL;
                }

                actual_count++;
            }
        }

        GUIEDIT_AddListItemArray(ctrl_id, list_item_ptr, actual_count);

        for (i = 0; i < num_count; i++)
        {
            if (PNULL != list_item_ptr[i].display_str_ptr)
            {
                SCI_FREE(list_item_ptr[i].display_str_ptr);
            }
            if (PNULL != list_item_ptr[i].user_str_ptr)
            {
                SCI_FREE(list_item_ptr[i].user_str_ptr);
            }
        }

        SCI_FREE(list_item_ptr);
    }
}

#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
//     Description : load timer info to editbox
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddTimer2Label(
    MMI_CTRL_ID_T ctrl_id,
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr
)
{
#ifdef MMISMS_TIMERMSG_ONCE_SUPPORT//只做了单次
    char str[32] = {0};
    uint8 len = 0;
    wchar *wstr_ptr = PNULL;
    MMI_STRING_T label_string = {0};
    SCI_DATE_T cur_date = {0};
    uint8 time_str[MMISET_DATE_STR_LEN + 1] = {0};
    uint8 date_str[MMISET_DATE_STR_LEN + 1] = {0};
    BOOLEAN is_display_year = FALSE;
    GUI_FONT_T font = SONG_FONT_18;
    GUI_COLOR_T font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_FONT);

    if (PNULL == event_fast_info_ptr)
    {
        return;
    }

    TM_GetSysDate(&cur_date);

    if (event_fast_info_ptr->year != cur_date.year)
    {
        is_display_year = TRUE;
    }

    MMIAPISET_FormatDateStrByDateEx(event_fast_info_ptr->year,
                                    event_fast_info_ptr->mon,
                                    event_fast_info_ptr->day,
                                    '.',
                                    date_str,
                                    MMISET_DATE_STR_LEN,
                                    is_display_year);

    MMIAPISET_FormatTimeStrByTime(event_fast_info_ptr->hour, event_fast_info_ptr->minute, time_str, MMISET_DATE_STR_LEN);

    len = sprintf(str, "%s %s", time_str, date_str);

    label_string.wstr_len = len;

    wstr_ptr = SCI_ALLOCA((label_string.wstr_len + 1) * sizeof(wchar));

    if (PNULL == wstr_ptr)
    {
        return;
    }

    label_string.wstr_ptr = wstr_ptr;

    SCI_MEMSET(label_string.wstr_ptr, 0x00, ((label_string.wstr_len + 1) * sizeof(wchar)));

    MMI_STRNTOWSTR(label_string.wstr_ptr, label_string.wstr_len, str, len, len);

    GUITEXT_SetString(ctrl_id, label_string.wstr_ptr, label_string.wstr_len, FALSE);

    GUITEXT_SetFont(ctrl_id, &font, &font_color);

    SCI_FREE(wstr_ptr);
#else
    char str[32] = {0};
    uint8 len = 0;
    wchar *wstr_ptr = PNULL;
    MMI_STRING_T fre_mode_string = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T label_string = {0};
    SCI_DATE_T cur_date = {0};
    uint8 time_str[MMISET_DATE_STR_LEN + 1] = {0};
    uint8 date_str[MMISET_DATE_STR_LEN + 1] = {0};
    BOOLEAN is_display_year = FALSE;
    GUI_FONT_T font = SONG_FONT_18;
    GUI_COLOR_T font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_FONT);

    if (PNULL == event_fast_info_ptr)
    {
        return;
    }

    switch (event_fast_info_ptr->fre_mode)
    {
        case ALM_MODE_ONCE:
            text_id = TXT_ALARM_MODE_ONCE;
            break;

        case ALM_MODE_EVERYDAY:
            text_id = TXT_ALARM_MODE_EVERYDAY;
            break;

        case ALM_MODE_EVERYWEEK:
            text_id = TXT_ALARM_MODE_WEEK;
            break;

        case ALM_MODE_EVERYMONTH:
            text_id = TXT_ALARM_MODE_MONTH;
            break;

        case ALM_MODE_EVERYYEAR:
            text_id = TXT_ALARM_MODE_YEAR;
            break;

        default:
            break;
    }

    MMI_GetLabelTextByLang(text_id, &fre_mode_string);

    TM_GetSysDate(&cur_date);

    if (event_fast_info_ptr->year != cur_date.year)
    {
        is_display_year = TRUE;
    }

    MMIAPISET_FormatDateStrByDateEx(event_fast_info_ptr->year,
                                    event_fast_info_ptr->mon,
                                    event_fast_info_ptr->day,
                                    '.',
                                    date_str,
                                    MMISET_DATE_STR_LEN,
                                    is_display_year);

    MMIAPISET_FormatTimeStrByTime(event_fast_info_ptr->hour, event_fast_info_ptr->minute, time_str, MMISET_DATE_STR_LEN);

    len = sprintf(str, ";\n%s %s", time_str, date_str);

    label_string.wstr_len = fre_mode_string.wstr_len + len;

    wstr_ptr = SCI_ALLOCA((label_string.wstr_len + 1) * sizeof(wchar));

    if (PNULL == wstr_ptr)
    {
        return;
    }

    label_string.wstr_ptr = wstr_ptr;

    SCI_MEMSET(label_string.wstr_ptr, 0x00, ((label_string.wstr_len + 1) * sizeof(wchar)));

    MMI_WSTRNCPY(label_string.wstr_ptr, label_string.wstr_len, fre_mode_string.wstr_ptr, fre_mode_string.wstr_len, fre_mode_string.wstr_len);

    MMI_STRNTOWSTR(label_string.wstr_ptr + fre_mode_string.wstr_len, len, (uint8 *)str, len, len);

    GUITEXT_SetString(ctrl_id, label_string.wstr_ptr, label_string.wstr_len, FALSE);

    GUITEXT_SetFont(ctrl_id, &font, &font_color);

    SCI_FREE(wstr_ptr);
#endif
}

/*****************************************************************************/
//  Description : set Timer SMS display
//  Global resource dependence :
//  Author:  renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetTimerMsgFreqDateDisplay(void)
{
    GUIFORM_SetStyle(MMISMS_TIMER_SMS_CTRL_ID, GUIFORM_STYLE_UNIT);

    if (!MMISMS_GetIsTimerMode())
    {
        GUIFORM_SetChildDisplay(MMISMS_EDITWIN_EDITCTRL_ID,
                                MMISMS_TIMER_SMS_CTRL_ID,
                                GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        GUIFORM_SetChildDisplay(MMISMS_EDITWIN_EDITCTRL_ID,
                                MMISMS_TIMER_SMS_CTRL_ID,
                                GUIFORM_CHILD_DISP_NORMAL);
    }
}

/*****************************************************************************/
//  Description : set Timer SMS label param
//  Global resource dependence :
//  Author:  renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetTimerMsgStartLabelParam(void)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;

    GUIFORM_SetStyle(MMISMS_TIMER_SMS_CTRL_ID, GUIFORM_STYLE_UNIT);
    MMISMS_SetCurOrderArrIndex(0);
    order_id = MMISMS_GetCurOperationOrderId();

    if (PNULL != order_id)
    {
        event_fast_info_ptr = MMISMS_TIMERMSG_GetCurEvent();

        if (PNULL != event_fast_info_ptr)
        {
            AddTimer2Label(MMISMS_EDIT_TIMER_FREQDATE_CTRL_ID, event_fast_info_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set Timer SMS label param
//  Global resource dependence :
//  Author:  renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetTimerMsgLabelParam(void)
{
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = MMISMS_TIMERMSG_GetCurEvent();

    GUIFORM_SetStyle(MMISMS_TIMER_SMS_CTRL_ID, GUIFORM_STYLE_UNIT);

    if (PNULL != event_fast_info_ptr)
    {
        AddTimer2Label(MMISMS_EDIT_TIMER_FREQDATE_CTRL_ID, event_fast_info_ptr);
    }
}
#endif
#endif
