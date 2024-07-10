/****************************************************************************
** File Name:      mmisms_securitywin.c                                    *
** Author:         liming deng                                             *
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
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_order.h"
#include "guirichtext.h"
#include "mmisms_showsmswin.h"
#include "mmicc_export.h"
#include "mmisms_menutable.h"
#include "mmiota_id.h"

#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#include "mmisms_set.h"
#endif
#include "mmimms_export.h"
#include "mmisms_commonui.h"    
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif    
#include "mmisms_send.h"
#include "mmisms_read.h"
#include "mmicom_extract.h"
#include "mmisms_save.h"
#include "mmiudisk_export.h"
#include "mmisms_appcontrol.h"
#include "mmisms_receive.h"
#include "gui_ucs2b_converter.h"
#include "mmialarm_export.h"
#include "mmiidle_statusbar.h" 
#include "mmisms_internal.h"
#ifdef MMS_SUPPORT
#include "mmimms_text.h"
#include "mmimms_id.h"
#endif
#ifdef MMI_OTA_SUPPORT
#include "mmiota_export.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "Met_mex_export.h"
#endif
#include "mmismsapp_interface.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//END
#ifdef MAINLCD_SIZE_240X400
#define SMS_BUTTONFORM_HEIGHT (48) //58
#else
#define SMS_BUTTONFORM_HEIGHT (58)
#endif



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
   MMI_WIN_ID_T            win_id; 
   uint32                  menu_id;
   MMISMS_READ_MSG_T        read_msg;
   MMISMS_ORDER_ID_T cur_order_id;
#ifdef SNS_SUPPORT
   IMMISns *pMe;
#endif

}MMISMS_SHOWSMS_WINDATA_T;

#ifdef MMI_PDA_SUPPORT
typedef struct
{
    MMI_CTRL_ID_T   ctrl_id;
    MMI_IMAGE_ID_T          bg_img_id;
    MMI_IMAGE_ID_T          pressed_bg_img_id;
    MMI_IMAGE_ID_T          fg_img_id;
    MMI_IMAGE_ID_T          pressed_fg_img_id;        
} MMISMS_BUTTON_T;

LOCAL MMISMS_BUTTON_T s_mtbox_form_button_info[] = 
{
#ifdef TTS_SUPPORT
    {MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_READ_SMS_PDA,IMAGE_SMS_READ_SMS_PDA},   
#endif
    {MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_REPLY_SMS_PDA,IMAGE_SMS_REPLY_SMS_PDA},
    {MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_FORWARD_SMS_PDA,IMAGE_SMS_FORWARD_SMS_PDA},
    {MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_DELETE_SMS_PDA,IMAGE_SMS_DELETE_SMS_PDA},    
};

LOCAL MMISMS_BUTTON_T s_sendbox_form_button_info[] = 
{
    {MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_FORWARD_SMS_PDA,IMAGE_SMS_FORWARD_SMS_PDA},
    {MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_DELETE_SMS_PDA,IMAGE_SMS_DELETE_SMS_PDA},    
};


LOCAL MMISMS_BUTTON_T s_class0_sms_form_button_info[] = 
{
    {MMISMS_CLASS0_BUTTON_FORM_CALL_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_CALL_PDA,IMAGE_SMS_CALL_PDA},
    {MMISMS_CLASS0_BUTTON_FORM_SAVE_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_SAVE_SMS_PDA,IMAGE_SMS_SAVE_SMS_PDA},    
};
#endif

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
extern MMISMS_GLOBAL_T g_mmisms_global;
#endif

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to show the class0 message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowClass0WinMsg(
                                          MMI_WIN_ID_T        win_id,        //IN:
                                          MMI_MESSAGE_ID_E    msg_id,        //IN:
                                          DPARAM            param        //IN:
                                          );
/*****************************************************************************/
//     Description : to handle the show message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSMSWinMsg(
                                       MMI_WIN_ID_T        win_id,        //IN:
                                       MMI_MESSAGE_ID_E msg_id,        //IN:
                                       DPARAM            param        //IN:
                                       );
#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleShowSmsAddSenderToBlackList(
    uint8 *num_array_ptr    //IN:
);
#endif

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinExtractUrlOptOk(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinExtractEmailOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mt box opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mt box opt init
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinMTBoxOptInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle succ box init
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinSUCCBoxOptInit(MMIPUB_MENU_NOTIFY_T *param );

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Description : handle mt box opt init
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinBtMTBoxOptInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle succ box init
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinBtSUCCBoxOptInit(MMIPUB_MENU_NOTIFY_T *param );
#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:jun.hu
//    Note: 
/*****************************************************************************/
LOCAL void SetShowSmsWinSoftKeyWithTTS(MMI_WIN_ID_T    win_id,        //IN:
                                       BOOLEAN         is_need_update // whether update
                                       );
#endif
/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsDeleteCallBack(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsWinDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );



/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinDel(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinForwardSms(void);

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  ShowSmsCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsCopyToSIM(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsCopyToMe(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsCopyQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );


/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsCopyCallBack(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr);

#endif

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  ShowSmsMoveSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsMoveToSIM(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsMoveToMe(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsMoveQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
#if 0
/*****************************************************************************/
//  Description : to handle the message of move waiting window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsMoveWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );
#endif
#endif

/*****************************************************************************/
//     Description : show MT SMS detail
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMtSmsDetailWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM            param
                                           );

/*****************************************************************************/
// 	Description : set mt SMS detail
//	Global resource dependence : 
//  Author: Tracy Zhang
/*****************************************************************************/
LOCAL void SetMtSmsDetail(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr, MMI_CTRL_ID_T ctrl_id);

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence :
//  Author: renwei
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleShowSmsWinSnsInstallWarningWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsWinShareSNS(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr);
#endif

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinClass0OptOk(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinClass0Init(MMIPUB_MENU_NOTIFY_T *param);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get richtext rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetRichtextRect(GUI_RECT_T *rect_ptr);

/*****************************************************************************/
//     Description : to  the show button form
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MTBoxButtonFormInit(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//     Description : to  the show send box button form
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_SendBoxButtonFormInit(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//     Description : to  the show class0 sms button form
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_Class0SmsButtonFormInit(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSMSPenOkMsg(MMI_WIN_ID_T win_id,         //in:
                              MMI_CTRL_ID_T ctrl_id  );


/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsDestroyButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMISMS_BUTTON_T *button_array,uint16 array_len);

/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsConstructButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMISMS_BUTTON_T *button_array,uint16 array_len);

#ifdef TTS_SUPPORT
/*****************************************************************************/
//     Description : set  button form while playing
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_SetTTSPlayingButtonForm(BOOLEAN is_refresh);

/*****************************************************************************/
//     Description : reset  button form after tts stop
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_SetTTSStopButtonForm(void);

#endif
#endif
/*****************************************************************************/
//     Description : open read message 
//    Global resource dependence :
//  Author:rong.gu
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleShowSmsOpenReadMessage(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : open read message 
//    Global resource dependence :
//  Author:rong.gu
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleOpenCurOrder(MMISMS_ORDER_ID_T cur_order_id);

/*****************************************************************************/
//  Description : add sms content item to richtext
//  Global resource dependence : 
//  Author: Kun Yu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_ShowSMSContentRichText(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_only_display,MMISMS_READ_MSG_T *read_msg_ptr);
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( MMISMS_SHOWCLASS0MSG_WIN_TAB ) = 
{
    WIN_PRIO( WIN_ONE_LEVEL ),
    #ifdef MMI_PDA_SUPPORT
    WIN_TITLE( TXT_SMS_READ),
    #else
    WIN_TITLE( TXT_SMS_READ_SMS ),
    #endif
    WIN_FUNC((uint32) HandleShowClass0WinMsg ),    
    WIN_ID( MMISMS_SHOWCLASS0_WIN_ID ),

    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),

    CREATE_RICHTEXT_CTRL(MMISMS_SHOWCLASS0_TEXTBOX_CTRL_ID ),
    END_WIN
};

// the window of 2.2.1 for showing the content of message
WINDOW_TABLE( MMISMS_SHOWMSG_WIN_TAB ) = 
{
    #ifdef MMI_PDA_SUPPORT
    //WIN_STATUSBAR,
    WIN_TITLE( TXT_SMS_READ),
    #else
    WIN_TITLE( TXT_SMS_READ_SMS ),
    #endif
    
    WIN_FUNC((uint32) HandleShowSMSWinMsg ),    
    WIN_ID( MMISMS_SHOWMSG_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_RICHTEXT_CTRL(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID),
    END_WIN
};

// the window for showing MT SMS detail
WINDOW_TABLE(MMISMS_MTSMS_DETAIL_TAB) = 
{
    WIN_TITLE(STXT_DETAIL),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32)HandleMtSmsDetailWinMsg),    
    WIN_ID(MMISMS_MTSMS_DETAIL_WIN_ID),

    CREATE_RICHTEXT_CTRL(MMISMS_MTSMS_DETAIL_TEXT_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    END_WIN
};

/*****************************************************************************/
//     Description : show MT SMS detail
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMtSmsDetailWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM            param
                                           )
{
    MMI_RESULT_E  err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_MTSMS_DETAIL_TEXT_CTRL_ID;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        return err_code;
    }
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set window text
        SetMtSmsDetail(win_data_ptr,ctrl_id);

        GUIRICHTEXT_SetLineSpace(ctrl_id, MMISMS_DETAIL_LINE_SPACE);

        //set sofkey
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        // close the window
        MMK_CloseWin(win_id);
        break;
        
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }

    return err_code;
}

/*****************************************************************************/
// 	Description : set mt SMS detail
//	Global resource dependence : 
//  Author: Tracy Zhang
/*****************************************************************************/
LOCAL void SetMtSmsDetail(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    uint16 index = 0;
    MMI_STRING_T name_string = {0};
    MMI_STRING_T date_string = {0};
    MMI_STRING_T time_string = {0};
    MMI_STRING_T sim_num = {0};
    MMI_STRING_T string_prefix = {0};
    uint8 number[MMISMS_MEMSTATE_LABEL_MAX_LEN + 1] = {0};
    uint8 number_len = 0;
    wchar content1_str[MMISMS_MEMSTATE_LABEL_MAX_LEN+1] = {0};
    wchar content2_str[MMISMS_MEMSTATE_LABEL_MAX_LEN+1] = {0};
    BOOLEAN find_result = FALSE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMISMS_ORDER_ID_T cur_order_id = MMISMS_GetCurOperationOrderId();

    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
#endif

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    // Sender
    if(MMISMS_MO_SEND_SUCC == cur_order_id->flag.mo_mt_type)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_RECIPIENT, &string_prefix);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_DETAIL_FROM, &string_prefix);
    }

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    number_len = MMISMS_GetOriginNum(
                                                            &dual_sys,
                                                            number,
                                                            MMISMS_MEMSTATE_LABEL_MAX_LEN
                                                            );
    //查找该号码是否存在PB中，存在则显示名称 
    name_string.wstr_ptr = SCI_ALLOC_APP((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
    name_string.wstr_len = MMISMS_PBNAME_MAX_LEN;
    SCI_MEMSET(name_string.wstr_ptr, 0x00, (MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
    
    find_result = MMISMS_GetNameByNumberFromPB(number, &name_string, MMISMS_PBNAME_MAX_LEN);

    if (!find_result) //没有在电话本中找到相应纪录则显示号码
    {
        name_string.wstr_len = MMISMS_PBNAME_MAX_LEN;

        MMI_STRNTOWSTR( name_string.wstr_ptr,
                        name_string.wstr_len,
                        (const uint8 *)number,
                        number_len,
                        number_len);

        name_string.wstr_len = number_len;
    }

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &name_string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(content1_str, 0x00, sizeof(content1_str));
    SCI_MEMSET(content1_str, 0x00, sizeof(content2_str));

    // get original date and time
    MMISMS_GetOriginTimeAndDate(
                                content1_str,	// date
                                content2_str	// time
                                );

    // Date
    MMI_GetLabelTextByLang(TXT_COMMON_DATE, &string_prefix);

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    date_string.wstr_ptr = content1_str;
    date_string.wstr_len = MMIAPICOM_Wstrlen(content1_str);

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &date_string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // Time
    MMI_GetLabelTextByLang(TXT_COMMON_TIME, &string_prefix);

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    time_string.wstr_ptr = content2_str;
    time_string.wstr_len = MMIAPICOM_Wstrlen(content2_str);

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &time_string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //显示存储位置
    MMI_GetLabelTextByLang(TXT_COMMON_DETAIL_FILE_POS, &string_prefix);

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    if ( MN_SMS_STORAGE_ME == cur_order_id->flag.storage )
    {
        MMI_GetLabelTextByLang(TXT_PHONE, &sim_num );
    }
    else
    {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        MMI_GetLabelTextByLang(TXT_COMM_SIM, &sim_num );
#else //MMI_MULTI_SIM_SYS_SINGLE
        sim_name = MMIAPISET_GetSimName(win_data_ptr->read_msg.dual_sys);

        sim_num.wstr_len = sim_name.wstr_len;
        sim_num.wstr_ptr = sim_name.wstr_arr;
#endif //MMI_MULTI_SIM_SYS_SINGLE
    
    }

    item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &sim_num, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_text;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    if (name_string.wstr_ptr != PNULL)
    {
        SCI_FREE(name_string.wstr_ptr);
        name_string.wstr_ptr = PNULL;
    }
}



/*****************************************************************************/
//  Description : entre sim box win
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterShowSmsWin(MMISMS_READ_MSG_T read_msg)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    if(MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID)) 
    {
        MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    }
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_SHOWSMS_WINDATA_T));

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms malloc error");
        return;
    }
    SCI_MEMCPY(&win_data_ptr->read_msg,&read_msg,sizeof(MMISMS_READ_MSG_T));
    win_data_ptr->cur_order_id = MMISMS_GetCurOperationOrderId();
    win_handle = MMK_CreateWin((uint32*)MMISMS_SHOWMSG_WIN_TAB, PNULL);
  
    if(PNULL == win_handle)
    {
        SCI_TRACE_LOW("mmisms sms win create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_SHOWMSG_WIN_TAB create success");
    MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);
}


/*****************************************************************************/
//  Description : entre sim box win
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterShowClass0Win(MMISMS_READ_MSG_T read_msg)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    if(MMK_IsOpenWin(MMISMS_SHOWCLASS0_WIN_ID)) 
    {
        MMK_CloseWin(MMISMS_SHOWCLASS0_WIN_ID);
    }
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_SHOWSMS_WINDATA_T));

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms malloc error");
        return;
    }
    SCI_MEMCPY(&win_data_ptr->read_msg,&read_msg,sizeof(MMISMS_READ_MSG_T));
    win_handle = MMK_CreateWin((uint32*)MMISMS_SHOWCLASS0MSG_WIN_TAB, PNULL);
   
    if(PNULL == win_handle)
    {
        SCI_TRACE_LOW("mmisms class0 win create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_SHOW_CHAT_SMS_WIN_TAB create success");
    MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);

}


/*****************************************************************************/
//  Description : add sms content item to richtext
//  Global resource dependence : 
//  Author: Kun Yu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_ShowSMSContentRichText(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_only_display,MMISMS_READ_MSG_T *read_msg_ptr)
{    
    wchar contact_str[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    uint16 nIndex = 0;
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 str_len = 0;
    wchar          time_string[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    wchar    *string = PNULL;
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMI_STRING_T long_sms = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
	MMISMS_BOX_TYPE_E cur_box_type = MMISMS_BOX_NONE;
    BOOLEAN ret = FALSE;
    MMI_STRING_T bt_num_name = {0};
#endif    
	
    GUIRICHTEXT_DeleteAllItems(ctrl_id);	
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    GUIRICHTEXT_SetFont(ctrl_id, MMISMS_GetWinFont());
#endif
    if (MMISMS_GetSMSIsExtract())
    {
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
    }
    else
    {
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
#ifdef DPHONE_SUPPORT 
    item_data.tag_type = GUIRICHTEXT_TAG_NONE;
#else
    if (MMISMS_GetSMSIsExtract())
    {
        item_data.tag_type = GUIRICHTEXT_TAG_PHONENUM;
    }
    else
    {
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
    }
#endif
	
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
	cur_box_type = MMISMS_GetCurBoxType();
	cur_order_id = MMISMS_GetCurOperationOrderId();
	if((0 != MMISMS_GetBoxTotalNumberFromOrder(cur_box_type))
		&& (PNULL != cur_order_id)
		)
	{
        ret = MMISMS_GetNameInfoByRecordId(cur_order_id->record_id, &(bt_num_name.wstr_ptr), (uint32*)(&(bt_num_name.wstr_len)));
	}
#endif
    
    MMISMS_GetReadSmsDispNum(read_msg_ptr,contact_str,&str_len);
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
	if( TRUE == ret )
	{
		item_data.text_data.buf.len = bt_num_name.wstr_len;  //MMIAPICOM_CopyStr2Buf(content_str, &num_name,GUIRICHTEXT_TEXT_MAX_LEN);
		item_data.text_data.buf.str_ptr = bt_num_name.wstr_ptr;
	}
	else
#endif
	{
		item_data.text_data.buf.len = str_len;  //MMIAPICOM_CopyStr2Buf(content_str, &num_name,GUIRICHTEXT_TEXT_MAX_LEN);
		item_data.text_data.buf.str_ptr = contact_str;
	}
    GUIRICHTEXT_AddItem (   //显示联系人
        ctrl_id,
        &item_data,
        &nIndex
        );

    MMISMS_GetReadSmsDispTime(read_msg_ptr,time_string,&str_len);   //显示时间
    item_data.text_data.buf.str_ptr = time_string;
    item_data.text_data.buf.len = str_len;
    item_data.tag_type = GUIRICHTEXT_TAG_NONE;
    GUIRICHTEXT_AddItem (
        ctrl_id,
        &item_data,
        &nIndex
        );

    string = SCI_ALLOCAZ( ( MMISMS_R8_MESSAGE_LEN + 1 )*sizeof(wchar) );
    if(PNULL == string)
    {
        return;
    }
    
    MMISMS_GetReadSmsDispContent(read_msg_ptr,string,&str_len); 
    //bug925155 冗余处理，且造成BT长短信显示错误，删除
#if 0//defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    long_sms = MMIAPISMS_GetLongMessageData();
    if( PNULL != long_sms.wstr_ptr)
	{
		item_data.text_data.buf.str_ptr = long_sms.wstr_ptr;
		item_data.text_data.buf.len = long_sms.wstr_len;
	}
    else
#endif
    //bug925155
	{
		item_data.text_data.buf.str_ptr = string;
		item_data.text_data.buf.len = str_len;
	}
#ifdef DPHONE_SUPPORT 
	item_data.tag_type = GUIRICHTEXT_TAG_NONE;
#else 
    if(MMISMS_GetSMSIsExtract())
    {
        item_data.tag_type = GUIRICHTEXT_TAG_PARSE_PHONENUM;/*lint !e655*/
		
#ifdef STREAMING_HS_SUPPORT
        item_data.tag_type = item_data.tag_type | GUIRICHTEXT_TAG_PARSE_RTSP;/*lint !e655*/
#endif // STREAMING_HS_SUPPORT
		
#ifdef MMS_SUPPORT
        item_data.tag_type = item_data.tag_type | GUIRICHTEXT_TAG_PARSE_EMAIL;/*lint !e655*/
#endif
		
#ifdef BROWSER_SUPPORT
        item_data.tag_type = item_data.tag_type | GUIRICHTEXT_TAG_PARSE_URL;/*lint !e655*/
#endif
    }
	
    else
    {
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
    }
#endif    
    GUIRICHTEXT_AddItem (
        ctrl_id,
        &item_data,
        &nIndex
        );
    SCI_FREE(string);
    string = PNULL;
}

/*****************************************************************************/
//     Description : handle left and right msg in show SMS window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void HandleLeftRightMsgInShowSmsWin(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id        //IN:
                                          )
{
    MMI_CTRL_ID_T ctrl_id = 0;
    uint16 selected_index = 0;
    uint16 total_item = 0;
    MMISMS_ORDER_ID_T cur_order = PNULL;
    MMISMS_BOX_TYPE_E state_box_type = MMISMS_BOX_NONE;
    MMISMS_BOX_TYPE_E sms_box_type = MMISMS_BOX_NONE;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);
    
    if( MMISMS_READ_SMS == MMISMS_GetCurrentOperStatus())
    {
        return;
    }

    ctrl_id = MMISMS_GetCurActiveListCtrlId();
    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
    total_item = GUILIST_GetTotalItemNum(ctrl_id);
    if (1 < total_item)
    {
        if (MSG_APP_LEFT == msg_id)
        {
            if (0 < selected_index)
            {
                selected_index--;
            }
            else
            {
                selected_index = total_item - 1;
            }
        }
        else
        {
            if (selected_index < total_item - 1)
            {
                selected_index++;
            }
            else
            {
                selected_index = 0;
            }
        }
        GUILIST_SetTopItemIndex(ctrl_id, selected_index);
        GUILIST_SetCurItemIndex(ctrl_id, selected_index);
        if(PNULL != win_data_ptr)  
        {
            state_box_type = MMISMS_GetSmsStateBoxtype(win_data_ptr->cur_order_id);
        }
        else
        {
            cur_order = MMISMS_GetCurOperationOrderId();

            state_box_type = MMISMS_GetSmsStateBoxtype(cur_order);
        }
        
        cur_order = MMISMS_GetLinkedArrayPtrIdInList(state_box_type, selected_index);
        
        if(PNULL != win_data_ptr)  
        {
            sms_box_type = MMISMS_GetSmsBoxtype(cur_order);
            if((sms_box_type == MMISMS_BOX_MT || sms_box_type == MMISMS_BOX_SENDSUCC) && (win_data_ptr->cur_order_id != cur_order))
            {
               win_data_ptr->cur_order_id = cur_order;
               HandleShowSmsOpenReadMessage(win_data_ptr);   
            }
        } 
        else
        {
             HandleOpenCurOrder(cur_order);
        }
    }
}
/*****************************************************************************/
//     Description : to show the class0 message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowClass0WinMsg(
                                          MMI_WIN_ID_T        win_id,        //IN:
                                          MMI_MESSAGE_ID_E    msg_id,        //IN:
                                          DPARAM            param        //IN:
                                          )
{
    MMI_RESULT_E ret_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOWCLASS0_TEXTBOX_CTRL_ID;
    //uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/
    GUIRICHTEXT_FOCUS_TYPE_T focus_type = GUIRICHTEXT_FOCUS_NONE;
    wchar *focus_data_wstr_ptr = PNULL;
    uint8 *focus_data_str_ptr = PNULL;
    uint16 index = 0;
#ifdef MMI_PDA_SUPPORT
    GUI_RECT_T text_rect = {0};
#endif
	MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        return ret_code;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_data_ptr->win_id = MMISMS_SHOWCLASS0_WIN_ID;
        MMISMS_PlayNewSMSRing(win_data_ptr->read_msg.dual_sys);
#ifdef MMI_PDA_SUPPORT
        SmsDestroyButtonFormCtrl(win_id,MMISMS_CLASS0_BUTTON_FORM_CTRL_ID,
        s_class0_sms_form_button_info,
        sizeof(s_class0_sms_form_button_info)/sizeof(s_class0_sms_form_button_info[0])
        );
        SmsConstructButtonFormCtrl(win_id,MMISMS_CLASS0_BUTTON_FORM_CTRL_ID,
        s_class0_sms_form_button_info,
        sizeof(s_class0_sms_form_button_info)/sizeof(s_class0_sms_form_button_info[0])
        );
        MMISMS_Class0SmsButtonFormInit(win_id);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMISMS_ShowSMSContentRichText(MMISMS_SHOWCLASS0_TEXTBOX_CTRL_ID,TRUE,&win_data_ptr->read_msg);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_LCD_SWITCH:
        MMISMS_GetRichtextRect(&text_rect);
        GUIRICHTEXT_SetRect(ctrl_id, text_rect);
        break;
#endif

#ifndef MMI_PDA_SUPPORT
    case MSG_FULL_PAINT:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        {
            BOOLEAN is_need_update = FALSE;

            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;

            if (MSG_FULL_PAINT == msg_id)
            {
                is_need_update = FALSE;
            }
            else
            {
                is_need_update = TRUE;
            }
            
            switch(focus_type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_COMMON_NUMBER, STXT_RETURN, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_URL_ADRESS, STXT_RETURN, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_EMAIL_BOX, STXT_RETURN, is_need_update);
                break;

            default:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_NULL, STXT_RETURN, is_need_update);
                break;
            }
        }
        break;
#endif

    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
#endif
            uint32 call_type = CC_CALL_NORMAL_CALL;
            
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(ctrl_id,&index);

            if (focus_type == GUIRICHTEXT_FOCUS_PHONENUM && 0 != index)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
#if 0
// no use
                CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
#endif
                focus_data_str_ptr = SCI_ALLOCA(min_num_len + 1);
                if (PNULL == focus_data_str_ptr)
                {
                    break;
                }
                SCI_MEMSET(focus_data_str_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)focus_data_str_ptr, MMISMS_PBNUM_MAX_LEN, richtext_element.data.str_ptr, richtext_element.data.len, min_num_len);


                MMIAPICC_MakeCall(
                                                                win_data_ptr->read_msg.dual_sys,
                                                                focus_data_str_ptr,
                                                                richtext_element.data.len,
                                                                PNULL,
                                                                PNULL,
                                                                sim_type,
                                                                call_type,                                                                
                                                                PNULL
                                                                );

                SCI_FREE(focus_data_str_ptr);
                focus_data_str_ptr = PNULL;
            }
            else
            {
                uint8 num_len = 0;
                uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
#if 0
// no use                
                CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
#endif

                num_len = MMIAPICOM_GenDispNumber(
                        win_data_ptr->read_msg.address.number_type,
                        MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->read_msg.address.num_len),
                        win_data_ptr->read_msg.address.party_num,
                        number,
                        MMISMS_PBNUM_MAX_LEN + 2
                        );

                if (num_len > 0)
                {
#if 0
// no use                
                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,                                                                    
                                                                    PNULL
                                                                    );
#else
                    MMIAPICC_MakeCall(
                                                                    win_data_ptr->read_msg.dual_sys,
                                                                    number,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,                                                                    
                                                                    PNULL
                                                                    );

#endif
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
#ifdef MMI_PDA_SUPPORT
            if(param == PNULL)
            {
                break;
            }

            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(ctrl_id == MMISMS_CLASS0_BUTTON_FORM_CALL_CTRL_ID)
            {
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
                //MMK_CreateWin((uint32 *)MMISMS_CLASS0_OPTION_POP_WIN_TAB,PNULL);
                {
                    MMIPUB_MENU_PARAM_T menuparam = {0};
                    menuparam.group_id = MENU_SMS_CLASS0_CALL_OPT;
                    menuparam.win_id =   MMISMS_CLASS0_OPTION_POP_WIN_ID;
                    menuparam.ctrl_id =  MMISMS_CLASS0_OPTION_POP_CTRL_ID;    
                    menuparam.dst_handle = win_data_ptr->win_id;
                    menuparam.func.OnMenuOk = HandleShowSMSWinClass0OptOk;   
                    menuparam.func.OnMenuInit = HandleShowSMSWinClass0Init;   
                    MMIPUB_OpenPubMenuWin( &menuparam ); 
                }

#else
                {
                    uint8 num_len = 0;
                    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
                    
                    num_len = MMIAPICOM_GenDispNumber(
                            win_data_ptr->read_msg.address.number_type,
                            MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->read_msg.address.num_len),
                            win_data_ptr->read_msg.address.party_num,
                            number,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );     

                    if (num_len > 0)
                    {
                                        MMIAPICC_MakeCall(
                                                                        win_data_ptr->read_msg.dual_sys,
                                                                        number,
                                                                        num_len,
                                                                        PNULL,
                                                                        PNULL,
                                                                        CC_CALL_SIM_MAX,
                                                                        CC_CALL_NORMAL_CALL,
                                                                        PNULL
                                                                        );
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                    }
                }
#endif
            }
            else if(ctrl_id == MMISMS_CLASS0_BUTTON_FORM_SAVE_CTRL_ID)
            {
                MMISMS_SaveClass0Msg();
                MMK_CloseWin(win_id);
            }
            else
#endif
            {
                GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
                focus_type = richtext_element.type;
                GUIRICHTEXT_GetFocusItemIndex(ctrl_id,&index);

                switch(focus_type) 
                {
                case GUIRICHTEXT_FOCUS_PHONENUM:
                    if (0 == index)
                    {
                        uint8 num_len = 0;

                        focus_data_str_ptr = SCI_ALLOC_APPZ(MMISMS_PBNUM_MAX_LEN + 2);
                        focus_data_wstr_ptr = SCI_ALLOC_APPZ((MMISMS_PBNUM_MAX_LEN + 2)*sizeof(wchar));

                        num_len = MMIAPICOM_GenDispNumber(
                                win_data_ptr->read_msg.address.number_type,
                                MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->read_msg.address.num_len),
                                win_data_ptr->read_msg.address.party_num,
                                focus_data_str_ptr,
                                MMISMS_PBNUM_MAX_LEN + 2
                                );

                        num_len = MIN(num_len, MMISMS_PBNUM_MAX_LEN);

                        MMI_STRNTOWSTR(focus_data_wstr_ptr, num_len, focus_data_str_ptr, num_len, num_len);

                        MMIAPICOM_OpenExtractNumberWin(MMISMS_CLASS0_EXTRACT_NUMBER_POP_WIN_ID, focus_data_wstr_ptr, num_len, win_data_ptr->read_msg.dual_sys);

                        SCI_FREE(focus_data_wstr_ptr);

                        SCI_FREE(focus_data_str_ptr);
                    }
                    else
                    {
                        MMIAPICOM_OpenExtractNumberWin(MMISMS_CLASS0_EXTRACT_NUMBER_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, win_data_ptr->read_msg.dual_sys);
                    }
                    break;

                case GUIRICHTEXT_FOCUS_URL:
                    MMIAPICOM_OpenExtractUrlWin(MMISMS_CLASS0_EXTRACT_URL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, win_data_ptr->read_msg.dual_sys);
                    break;

                case GUIRICHTEXT_FOCUS_EMAIL:
                    MMIAPICOM_OpenExtractUrlWin(MMISMS_CLASS0_EXTRACT_EMAIL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, win_data_ptr->read_msg.dual_sys);
                    break;
             
                case GUIRICHTEXT_FOCUS_NONE:
                    break;

                default:
                    break;
                }
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMISMS_SaveClass0Msg();
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
        SCI_FREE(win_data_ptr);
        break;
        
    default:
        ret_code = MMI_RESULT_FALSE;
        break;
    }
    
    return (ret_code);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : to handle the show message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSMSWinMsg(
                                       MMI_WIN_ID_T        win_id,        //IN:
                                       MMI_MESSAGE_ID_E msg_id,        //IN:
                                       DPARAM            param        //IN:
                                       )
{
    // GUIRICHTEXT_FOCUS_ELEM_T        focus_element = {0};/*lint !e64*/
    MMI_RESULT_E ret_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID;
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    // GUI_POINT_T point = {0};
    // GUI_POINT_T dis_point = {0};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMI_CTRL_ID_T richtext_ctrl_id                       = MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element            = {0};/*lint !e64*/
    GUIRICHTEXT_FOCUS_TYPE_T focus_type                    = GUIRICHTEXT_FOCUS_NONE;
    wchar *focus_num_ptr                                 = PNULL;

    uint8 *number_ptr                                    = PNULL;
    uint16 index = 0;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;    

    GUI_RECT_T richtext_rect = {0};

#ifdef TTS_SUPPORT	
    //+CR243394
    static BOOLEAN    is_tts_played = FALSE;
    //-CR243394
    uint16                  total_item;
    MMI_CTRL_ID_T           cur_active_ctrl_id = 0; 
#endif
	MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        return ret_code;
    }
    
    // GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    //SCI_TRACE_LOW:"MMISMS: HandleShowSMSWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_10967_112_2_18_2_54_43_758,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
#ifdef MMI_PDA_SUPPORT
    case MSG_LCD_SWITCH:
        MMISMS_GetRichtextRect(&richtext_rect);
        GUIRICHTEXT_SetRect(ctrl_id, richtext_rect);
        break;
#endif
    case MSG_OPEN_WINDOW:
        win_data_ptr->win_id = win_id;
		if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
		{
			MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
		}
		if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
		{
			MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
		}
        #ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleRollArrow(win_id, FALSE);
        #else
        GUIWIN_SetTitleRollArrow(win_id, TRUE);
        #endif

        if(win_data_ptr->read_msg.mo_mt_type != MMISMS_MO_SEND_SUCC)
        {
            SmsDestroyButtonFormCtrl(win_id,MMISMS_MTBOX_BUTTON_FORM_CTRL_ID,
            s_mtbox_form_button_info,
            sizeof(s_mtbox_form_button_info)/sizeof(s_mtbox_form_button_info[0])
            );
            SmsConstructButtonFormCtrl(win_id,MMISMS_MTBOX_BUTTON_FORM_CTRL_ID,
            s_mtbox_form_button_info,
            sizeof(s_mtbox_form_button_info)/sizeof(s_mtbox_form_button_info[0])
            );
            MMISMS_MTBoxButtonFormInit(win_id);
        }
        else
        {
            SmsDestroyButtonFormCtrl(win_id,MMISMS_MTBOX_BUTTON_FORM_CTRL_ID,
            s_sendbox_form_button_info,
            sizeof(s_sendbox_form_button_info)/sizeof(s_sendbox_form_button_info[0])
            );
            SmsConstructButtonFormCtrl(win_id,MMISMS_MTBOX_BUTTON_FORM_CTRL_ID,
            s_sendbox_form_button_info,
            sizeof(s_sendbox_form_button_info)/sizeof(s_sendbox_form_button_info[0])
            );
            MMISMS_SendBoxButtonFormInit(win_id);
        }
        if (MMISMS_CurMsgIsLocked())
        {
            GUI_BG_T fg_info = {0};

            fg_info.bg_type = GUI_BG_IMG;
            fg_info.img_id = IMAGE_SMS_DELETE_GRAY_SMS_PDA;
            GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, TRUE, &fg_info, FALSE);
        }        
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        MMISMS_ShowSMSContentRichText(ctrl_id,FALSE,&win_data_ptr->read_msg);
        
#ifdef DPHONE_SUPPORT
        GUIRICHTEXT_SetLineSpace(ctrl_id, MMISMS_DETAIL_LINE_SPACE);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef TTS_SUPPORT
        cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
        //+CR243394
        is_tts_played = FALSE;
        //-CR243394 
#endif
        break;
        
    case MMISMS_MSG_UPDATE_NUMBER:
        MMISMS_ShowSMSContentRichText(ctrl_id,FALSE,&win_data_ptr->read_msg);
        break;

#ifdef MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
#endif
        {
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
            uint32 call_type = CC_CALL_NORMAL_CALL;
            CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

#ifdef VT_SUPPORT
            if(MSG_KEYLONG_GREEN == msg_id)
            {
                call_type = CC_CALL_VIDEO_CALL;
            }
#endif

            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
            
            if(focus_type ==GUIRICHTEXT_FOCUS_PHONENUM && 0 != index)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                
                focus_num_ptr = SCI_ALLOC_APP((min_num_len + 1)*sizeof(wchar));
                SCI_MEMSET(focus_num_ptr, 0, ((min_num_len + 1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,min_num_len+1);
                number_ptr = SCI_ALLOC_APP(min_num_len + 1);
                SCI_MEMSET(number_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)number_ptr, MMISMS_PBNUM_MAX_LEN, focus_num_ptr, richtext_element.data.len, min_num_len);
         
                {
                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number_ptr,
                                                                    richtext_element.data.len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                SCI_FREE(focus_num_ptr);
                SCI_FREE(number_ptr);
                focus_num_ptr = PNULL;
                number_ptr = PNULL;
            }
            else
            {
                num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
                
                if (num_len > 0)
                {
                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }
            }            
        }
        break;
#else
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:   
        {
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX);
            
            uint32 call_type = CC_CALL_NORMAL_CALL;
            
            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
            if(focus_type ==GUIRICHTEXT_FOCUS_PHONENUM && 0 != index)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                    
                focus_num_ptr = SCI_ALLOC_APP((min_num_len + 1)*sizeof(wchar));
                SCI_MEMSET(focus_num_ptr, 0, ((min_num_len + 1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,min_num_len+1);
                number_ptr = SCI_ALLOC_APP(min_num_len + 1);
                SCI_MEMSET(number_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)number_ptr, MMISMS_PBNUM_MAX_LEN, focus_num_ptr, richtext_element.data.len, min_num_len);
         
                {
                    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number_ptr,
                                                                    richtext_element.data.len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                SCI_FREE(focus_num_ptr);
                SCI_FREE(number_ptr);
                focus_num_ptr = PNULL;
                number_ptr = PNULL;
            }
            else
            {
                num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
               
                if (num_len > 0)
                {
                    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }            
            }
        }
        break;
#endif

    case MSG_FULL_PAINT:
#ifdef TTS_SUPPORT        
        /* Bug fix: cr00203127. If in call state, do not play SMS text */
        if( ! MMIAPICC_IsInState(CC_IN_CALL_STATE) && MMISMS_GetTTSInfo())
        {
            /* Bug fix: cr00195085. We should do the judgement by whitch is not the current index 
                of list CTRL, but SMS record_id, because that if we receive a new SMS in this win, the current
                index would be changed */
            BOOLEAN ret = FALSE;
            MMI_STRING_T    msg_content = {0};
            MMITTS_PLAY_PARAM_T play_param = {0};

            if (!is_tts_played)
            {
                //The content is played first time.
                MMISMS_GetSMSContent(&msg_content);
                if(PNULL != msg_content.wstr_ptr)
                {
                    play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
                    play_param.txt_len_in_byte = msg_content.wstr_len * 2;
                    play_param.is_ucs2 = TRUE;
                    play_param.play_times = 1;
                    play_param.tts_vol = MMIAPISET_GetMultimVolume();
                    play_param.ring_type = TTS_RING_TYPE_SMS;
                    play_param.win_handle = MMISMS_SHOWMSG_WIN_ID;
					play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
					play_param.digit_mode = TTS_DIGIT_AUTO;
					play_param.punc_mode = TTS_PUNC_OFF;
                    play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;                    
                    ret = MMIAPITTS_PlayText(&play_param);
#ifdef MMI_PDA_SUPPORT
                    if(ret)
                    {
                        MMISMS_SetTTSPlayingButtonForm(FALSE);
                        // to do 
                    }
#endif
                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
                //+CR243394
                is_tts_played = TRUE;
                //-CR243394                     
            }
        }
#endif        
        
        if (MMISMS_READ_STATUSREPORT == MMISMS_GetMtType())
        {
#ifdef TTS_SUPPORT			
            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_DELETE, STXT_STOP, STXT_RETURN, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_DELETE, TXT_SET_TTS_PLAY, STXT_RETURN, TRUE);
            }
#else
            if (MMISMS_CurMsgIsLocked())
            {
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_REPLY, STXT_RETURN, FALSE);
            }
            else
            {        
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_DELETE, TXT_REPLY, STXT_RETURN, FALSE);
            }
#endif
        }
        else
        {
            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            
            if(focus_type != GUIRICHTEXT_FOCUS_NONE)
            {
                switch(focus_type) 
                {
                case GUIRICHTEXT_FOCUS_PHONENUM:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_URL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_EMAIL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                    break;
                default:                        
                    break;
                }
            }
            
            else
            {
                if (MMISMS_GetCurBoxType() != MMISMS_BOX_SENDSUCC )
                {
#ifdef TTS_SUPPORT
                    SetShowSmsWinSoftKeyWithTTS(win_id, FALSE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_REPLY, STXT_RETURN, FALSE);
#endif
                }
                else
                {
#ifdef TTS_SUPPORT
                    SetShowSmsWinSoftKeyWithTTS(win_id, FALSE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_COMMON_FORWARD, STXT_RETURN, FALSE);
#endif
                }
            }
        }
        break;
        
#ifdef TTS_SUPPORT		
    case MSG_TTS_OVER:
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
        if(GUIRICHTEXT_TAG_NONE == focus_type)
        {
             SetShowSmsWinSoftKeyWithTTS(win_id, TRUE);
        }
        #ifdef MMI_PDA_SUPPORT
        {
            MMISMS_SetTTSStopButtonForm();
        }
        #endif
        break;
#endif

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT 
        if(param != PNULL && HandleSMSPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id))
        {
            //SCI_TRACE_LOW:"sms pen ok has handled"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_11332_112_2_18_2_54_44_759,(uint8*)"");
            break;
        }
#endif
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);

        switch(focus_type) 
        {
            case GUIRICHTEXT_FOCUS_PHONENUM:
			if (MN_NUM_TYPE_ALPHANUMERIC == win_data_ptr->read_msg.address.number_type)  //fixed on bug120542
			{
				return;
			} 
			else
			{
				MMIPUB_MENU_PARAM_T menuparam = {0};
				menuparam.group_id = MENU_SMS_EXTRACT_NUMBER_POP_MENU;
				menuparam.win_id =   MMISMS_EXTRACT_NUMBER_POP_WIN_ID;
				menuparam.ctrl_id =  MMISMS_EXTRACT_NUMBER_POP_CTRL_ID;    
				menuparam.dst_handle = win_data_ptr->win_id;
				menuparam.func.OnMenuOk = HandleShowSMSWinExtractNumberOptOk;       
                MMIPUB_OpenPubMenuWin( &menuparam );
			}
            
                
            
            break;
        case GUIRICHTEXT_FOCUS_URL:
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_EXTRACT_URL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_URL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_URL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinExtractUrlOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam );                 
            }            
            break;
#ifdef MMIEMAIL_SUPPORT
        case GUIRICHTEXT_FOCUS_EMAIL:
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_EXTRACT_EMAIL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_EMAIL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_EMAIL_POP_CTRL_ID;       
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinExtractEmailOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam );                 
            }
            break;
#endif  
        case GUIRICHTEXT_FOCUS_NONE:
#ifdef TTS_SUPPORT
        /*if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        }
        else*/ // 改成和彩信那边一致，都提示内存不足

        if(MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        else
        {
            MMI_STRING_T    msg_content = {0};
            MMITTS_PLAY_PARAM_T play_param = {0};
            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
            {
                MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
            }
            else
            {
                MMISMS_GetSMSContent(&msg_content);
                if(PNULL != msg_content.wstr_ptr)
                {
                    play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
                    play_param.txt_len_in_byte = msg_content.wstr_len * 2;
                    play_param.is_ucs2 = TRUE;
                    play_param.play_times = 1;
                    play_param.tts_vol = MMIAPISET_GetMultimVolume();
                    play_param.ring_type = TTS_RING_TYPE_SMS;
                    play_param.win_handle = MMISMS_SHOWMSG_WIN_ID;
					play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
					play_param.digit_mode = TTS_DIGIT_AUTO;
					play_param.punc_mode = TTS_PUNC_OFF;
					play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;                    
                    MMIAPITTS_PlayText(&play_param);
                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
            }

            SetShowSmsWinSoftKeyWithTTS(win_id, TRUE);
        }
        break;
#else
            //打开写消息窗口
            // get the address of the SMS message 
            num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
  //          MMISMS_SetIsNeedLoadSaved(FALSE);
            
            if (MMISMS_BOX_SENDSUCC != MMISMS_GetCurBoxType())
            {
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
                MMISMS_SetIsAnswerSms(TRUE);
#endif

                //open edit window and set the address to the deliver address
                MMISMS_WriteNewMessage(
                        dual_sys,
                        PNULL,
                        FALSE,
                        number,
                        num_len
                        );
            }
            else
            {
                MMI_STRING_T    msg_content = {0};
                // set the init SMS content
                MMISMS_GetSMSContent(&msg_content);
                    
                // open edit window
                MMISMS_WriteNewMessage(
                                    MN_DUAL_SYS_MAX,
                                    &msg_content,
                                    FALSE,
                                    PNULL,
                                    0
                                    );

                if (PNULL != msg_content.wstr_ptr)
                {
                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
            }
            break;
#endif

        default:                        
            break;
        }
    break;

    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
      
        switch(focus_type) 
        {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
            	break;
            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_NONE:
#ifdef TTS_SUPPORT                
                SetShowSmsWinSoftKeyWithTTS(win_id, TRUE);
#else
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_REPLY, STXT_RETURN, TRUE);
#endif
                break;
            default:                        
                break;
        }
    break;    

    case MSG_PROMPTWIN_OK:
        // open waiting window
        //MMISMS_OperateDelorDelAll(win_data_ptr->cur_order_id,0,ShowSMSWinHandleDelWaitWinMsg);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    case MSG_CLOSE_WINDOW:
#ifdef TTS_SUPPORT		
        MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
#endif	

        MMIPUB_CloseQuerytWin(&query_win_id);
        MMISMS_UpdateUnreadOrder();

        SCI_FREE(win_data_ptr);

        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
#ifdef TTS_SUPPORT		
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
        {        
            GUI_BG_T button = {0};
        
            button.bg_type = GUI_BG_IMG;
            button.img_id  = IMAGE_SMS_READ_SMS_PDA;
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
            #ifdef MMI_PDA_SUPPORT 
            MMISMS_SetTTSStopButtonForm();
            #endif
        }
#endif
        if(win_data_ptr->cur_order_id != PNULL )
        {
            box_type = MMISMS_GetSmsBoxtype(win_data_ptr->cur_order_id);
        }
        
        switch ( box_type )
        {
        case MMISMS_BOX_MT:    

            if ( MMISMS_READ_STATUSREPORT == MMISMS_GetMtType() )
            {
                if (MMISMS_IsUpdateOk() && !MMISMS_CurMsgIsLocked())
                {
                    //询问是否删除？
                    MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
            }
            else
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_MTBOX_OPT;
                menuparam.win_id =   MMISMS_MTBOX_WIN_ID;
                menuparam.ctrl_id =  MMISMS_MTBOX_OPT_MENU_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;       
                menuparam.func.OnMenuInit = HandleShowSMSWinMTBoxOptInit;  
                MMIPUB_OpenPubMenuWin( &menuparam );   
            }
            break;

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
           
            case MMISMS_BT_BOX_MT:   
                if ( MMISMS_READ_STATUSREPORT == MMISMS_GetMtType() )
                {
                    if (MMISMS_IsUpdateOk() && !MMISMS_CurMsgIsLocked())
                    {
                        //???????
                        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                    }
                }
                else
                {
                    //MMK_CreateWin((uint32 *)MMISMS_BT_MTBOX_WIN_TAB,PNULL);
                    MMIPUB_MENU_PARAM_T menuparam = {0};
                    menuparam.group_id = MENU_SMS_BT_MTBOX_OPT;
                    menuparam.win_id =   MMISMS_BT_MTBOX_WIN_ID;
                    menuparam.ctrl_id =  MMISMS_BT_MTBOX_OPT_MENU_CTRL_ID;    
                    menuparam.dst_handle = win_data_ptr->win_id;
                    menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;       
                    menuparam.func.OnMenuInit = HandleShowSMSWinBtMTBoxOptInit;  
                    MMIPUB_OpenPubMenuWin( &menuparam );
                }
                break;
#endif

        case MMISMS_BOX_SENDSUCC:            
            //MMK_CreateWin((uint32 *)MMISMS_SENDSUCCBOX_WIN_TAB,PNULL);
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_SENDSUCC_OPT;
                menuparam.win_id =   MMISMS_SENDSUCCBOX_WIN_ID;
                menuparam.ctrl_id =  MMISMS_SENDSUCCBOX_OPT_MENU_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;   
                menuparam.func.OnMenuInit = HandleShowSMSWinSUCCBoxOptInit;   
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }            
            break;

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        
        case MMISMS_BT_BOX_SENDSUCC:
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                
                menuparam.group_id = MENU_SMS_BT_SENDSUCC_OPT;
                menuparam.win_id =   MMISMS_BT_SENDSUCCBOX_WIN_ID;
                menuparam.ctrl_id =  MMISMS_BT_SENDSUCCBOX_OPT_MENU_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;       
                menuparam.func.OnMenuInit = HandleShowSMSWinSUCCBoxOptInit;  
                MMIPUB_OpenPubMenuWin( &menuparam );
            }
            break;
#endif

            case MMISMS_BOX_SENDFAIL:
            case MMISMS_BOX_NOSEND:
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
            case MMISMS_BT_BOX_SENDFAIL:
            case MMISMS_BT_BOX_NOSEND:
#endif
            default:
                break;
            }

        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
   
        // the MT SMS list box is opened
        MMK_PostMsg(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_UPDATE_LIST, PNULL,0);
        MMK_CloseWin(win_id);

        break;

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_CTL_TITLE_MOVE_NEXT:
    case MSG_CTL_TITLE_MOVE_PREV:
#ifdef TTS_SUPPORT		
        cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
        total_item       = GUILIST_GetTotalItemNum(cur_active_ctrl_id);
        //+CR243394
        if (total_item>1)
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
            is_tts_played = FALSE;
        }
        //-CR243394
#endif        

        if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
        {
            msg_id = MSG_APP_RIGHT;
        }
        else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
        {
            msg_id = MSG_APP_LEFT;
        }

        // the MT SMS list box is opened

        if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID) || MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
        {           
            HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
        }
        break;
   
    default:
        ret_code = MMI_RESULT_FALSE;
        break;
    }
    
    return ret_code;
}
#else
/*****************************************************************************/
//     Description : to handle the show message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSMSWinMsg(
                                       MMI_WIN_ID_T        win_id,        //IN:
                                       MMI_MESSAGE_ID_E msg_id,        //IN:
                                       DPARAM            param        //IN:
                                       )
{
    // GUIRICHTEXT_FOCUS_ELEM_T        focus_element = {0};/*lint !e64*/
    MMI_RESULT_E ret_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID;
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    // GUI_POINT_T point = {0};
    // GUI_POINT_T dis_point = {0};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;

    MMI_CTRL_ID_T richtext_ctrl_id                       = MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element            = {0};/*lint !e64*/
    GUIRICHTEXT_FOCUS_TYPE_T focus_type                    = GUIRICHTEXT_FOCUS_NONE;
    wchar *focus_num_ptr                                 = PNULL;

    uint8 *number_ptr                                    = PNULL;
    uint16 index = 0;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;    
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMI_STRING_T    prompt_str = {0};
    MMISMS_ORDER_ID_T cur_order_id = MMISMS_GetCurOperationOrderId();
    MN_SMS_RECORD_ID_T record_id = 0;
    char handle_number[HANDLE_LEN] = {0};
    uint8 is_extra_msg = 0;
#endif

#ifdef TTS_SUPPORT	
    //+CR243394
    static BOOLEAN    is_tts_played = FALSE;
    //-CR243394
    uint16                  total_item;
    MMI_CTRL_ID_T           cur_active_ctrl_id = 0; 
#endif
	MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        return ret_code;
    }
    
    // GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    //SCI_TRACE_LOW:"MMISMS: HandleShowSMSWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_11828_112_2_18_2_54_45_763,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_data_ptr->win_id = win_id;
		if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
		{
			MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
		}
		if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
		{
			MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
		}

        GUIWIN_SetTitleRollArrow(win_id, TRUE);

        
#ifdef DPHONE_SUPPORT
        GUIRICHTEXT_SetLineSpace(ctrl_id, MMISMS_DETAIL_LINE_SPACE);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
#endif
        MMISMS_ShowSMSContentRichText(ctrl_id,FALSE,&win_data_ptr->read_msg);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef TTS_SUPPORT
        cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
        //+CR243394
        is_tts_played = FALSE;
        //-CR243394 
#endif
        break;
        
    case MMISMS_MSG_UPDATE_NUMBER:
        MMISMS_ShowSMSContentRichText(ctrl_id,FALSE,&win_data_ptr->read_msg);
        break;

#ifdef MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
#endif
        {
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
            uint32 call_type = CC_CALL_NORMAL_CALL;
            CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

#ifdef VT_SUPPORT
            if(MSG_KEYLONG_GREEN == msg_id)
            {
                call_type = CC_CALL_VIDEO_CALL;
            }
#endif
            dual_sys = win_data_ptr->read_msg.dual_sys;
            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
            
            if(focus_type ==GUIRICHTEXT_FOCUS_PHONENUM && 0 != index)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                
                focus_num_ptr = SCI_ALLOC_APP((min_num_len + 1)*sizeof(wchar));
                SCI_MEMSET(focus_num_ptr, 0, ((min_num_len + 1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,min_num_len+1);
                number_ptr = SCI_ALLOC_APP(min_num_len + 1);
                SCI_MEMSET(number_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)number_ptr, MMISMS_PBNUM_MAX_LEN, focus_num_ptr, richtext_element.data.len, min_num_len);
         
                {
                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number_ptr,
                                                                    richtext_element.data.len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                SCI_FREE(focus_num_ptr);
                SCI_FREE(number_ptr);
                focus_num_ptr = PNULL;
                number_ptr = PNULL;
            }
            else
            {
                num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
                
                if (num_len > 0)
                {
                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }
            }            
        }
        break;
#else
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:   
        {
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX);
            
            uint32 call_type = CC_CALL_NORMAL_CALL;
            dual_sys = win_data_ptr->read_msg.dual_sys;
            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
            if(focus_type ==GUIRICHTEXT_FOCUS_PHONENUM && 0 != index)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                    
                focus_num_ptr = SCI_ALLOC_APP((min_num_len + 1)*sizeof(wchar));
                SCI_MEMSET(focus_num_ptr, 0, ((min_num_len + 1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,min_num_len+1);
                number_ptr = SCI_ALLOC_APP(min_num_len + 1);
                SCI_MEMSET(number_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)number_ptr, MMISMS_PBNUM_MAX_LEN, focus_num_ptr, richtext_element.data.len, min_num_len);
         
                {
                    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number_ptr,
                                                                    richtext_element.data.len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                SCI_FREE(focus_num_ptr);
                SCI_FREE(number_ptr);
                focus_num_ptr = PNULL;
                number_ptr = PNULL;
            }
            else
            {
                num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
               
                if (num_len > 0)
                {
                    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                    cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    sim_type,
                                                                    call_type,
                                                                    PNULL
                                                                    );
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }            
            }
        }
        break;
#endif

    case MSG_FULL_PAINT:
#ifdef TTS_SUPPORT        
        /* Bug fix: cr00203127. If in call state, do not play SMS text */
        if( ! MMIAPICC_IsInState(CC_IN_CALL_STATE) && MMISMS_GetTTSInfo())
        {
            /* Bug fix: cr00195085. We should do the judgement by whitch is not the current index 
                of list CTRL, but SMS record_id, because that if we receive a new SMS in this win, the current
                index would be changed */
            BOOLEAN ret = FALSE;
            MMI_STRING_T    msg_content = {0};
            MMITTS_PLAY_PARAM_T play_param = {0};

            if (!is_tts_played)
            {
                //The content is played first time.
                MMISMS_GetSMSContent(&msg_content);
                if(PNULL != msg_content.wstr_ptr)
                {
                    play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
                    play_param.txt_len_in_byte = msg_content.wstr_len * 2;
                    play_param.is_ucs2 = TRUE;
                    play_param.play_times = 1;
                    play_param.tts_vol = MMIAPISET_GetMultimVolume();
                    play_param.ring_type = TTS_RING_TYPE_SMS;
                    play_param.win_handle = MMISMS_SHOWMSG_WIN_ID;
					play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
					play_param.digit_mode = TTS_DIGIT_AUTO;
					play_param.punc_mode = TTS_PUNC_OFF;
                    play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;                    
                    ret = MMIAPITTS_PlayText(&play_param);

                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
                //+CR243394
                is_tts_played = TRUE;
                //-CR243394                     
            }
        }
#endif        
        
        if (MMISMS_READ_STATUSREPORT == MMISMS_GetMtType())
        {
#ifdef TTS_SUPPORT			
            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_DELETE, STXT_STOP, STXT_RETURN, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_DELETE, TXT_SET_TTS_PLAY, STXT_RETURN, TRUE);
            }
#else
            if (MMISMS_CurMsgIsLocked())
            {
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_REPLY, STXT_RETURN, FALSE);
            }
            else
            {        
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_DELETE, TXT_REPLY, STXT_RETURN, FALSE);
            }
#endif
        }
        else
        {
            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
            focus_type = richtext_element.type;
            
            if(focus_type != GUIRICHTEXT_FOCUS_NONE)
            {
                switch(focus_type) 
                {
                case GUIRICHTEXT_FOCUS_PHONENUM:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_URL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_EMAIL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                    break;
                default:                        
                    break;
                }
            }
            
            else
            {
                if (MMISMS_GetCurBoxType() != MMISMS_BOX_SENDSUCC )
                {
#ifdef TTS_SUPPORT
                    SetShowSmsWinSoftKeyWithTTS(win_id, FALSE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_REPLY, STXT_RETURN, FALSE);
#endif
                }
                else
                {
#ifdef TTS_SUPPORT
                    SetShowSmsWinSoftKeyWithTTS(win_id, FALSE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_COMMON_FORWARD, STXT_RETURN, FALSE);
#endif
                }
            }
        }
        break;
        
#ifdef TTS_SUPPORT		
    case MSG_TTS_OVER:
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
        if(GUIRICHTEXT_TAG_NONE == focus_type)
        {
             SetShowSmsWinSoftKeyWithTTS(win_id, TRUE);
        }

        break;
#endif

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        dual_sys = win_data_ptr->read_msg.dual_sys;
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);

        switch(focus_type) 
        {
        case GUIRICHTEXT_FOCUS_PHONENUM:   
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_EXTRACT_NUMBER_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_NUMBER_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_NUMBER_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinExtractNumberOptOk;       
                MMIPUB_OpenPubMenuWin( &menuparam );
            }
            break;
        case GUIRICHTEXT_FOCUS_URL:
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_EXTRACT_URL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_URL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_URL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinExtractUrlOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam );                 
            }
            
            break;
#ifdef MMIEMAIL_SUPPORT
        case GUIRICHTEXT_FOCUS_EMAIL:
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_EXTRACT_EMAIL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_EMAIL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_EMAIL_POP_CTRL_ID;       
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinExtractEmailOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam );                 
            }
            
            break;
#endif
        case GUIRICHTEXT_FOCUS_NONE:
#ifdef TTS_SUPPORT
        /*if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        }
        else*/ // 改成和彩信那边一致，都提示内存不足

        if(MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        else
        {
            MMI_STRING_T    msg_content = {0};
            MMITTS_PLAY_PARAM_T play_param = {0};
            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
            {
                MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
            }
            else
            {
                MMISMS_GetSMSContent(&msg_content);
                if(PNULL != msg_content.wstr_ptr)
                {
                    play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
                    play_param.txt_len_in_byte = msg_content.wstr_len * 2;
                    play_param.is_ucs2 = TRUE;
                    play_param.play_times = 1;
                    play_param.tts_vol = MMIAPISET_GetMultimVolume();
                    play_param.ring_type = TTS_RING_TYPE_SMS;
                    play_param.win_handle = MMISMS_SHOWMSG_WIN_ID;
					play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
					play_param.digit_mode = TTS_DIGIT_AUTO;
					play_param.punc_mode = TTS_PUNC_OFF;
                    play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;                    
                    MMIAPITTS_PlayText(&play_param);
                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
            }

            SetShowSmsWinSoftKeyWithTTS(win_id, TRUE);
        }
        break;
#else
            //打开写消息窗口
            // get the address of the SMS message 
            num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
//            MMISMS_SetIsNeedLoadSaved(FALSE);
#ifdef MMI_SMS_MINI_SUPPORT
            MMISMS_ClearEditContent();
            MMISMS_ClearDestNum();
#endif            
            if (MMISMS_BOX_SENDSUCC != MMISMS_GetCurBoxType())
            {
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
                //g_mmisms_global.is_answer_sms = TRUE;
                MMISMS_SetIsAnswerSms(TRUE);
#endif

                //open edit window and set the address to the deliver address
                MMISMS_WriteNewMessage(
                        dual_sys,
                        PNULL,
                        FALSE,
                        number,
                        num_len
                        );
            }
            else
            {
                MMI_STRING_T msg_content = {0};
                // set the init SMS content
                MMISMS_GetSMSContent(&msg_content);
                    
                // open edit window
                MMISMS_WriteNewMessage(
                                        MN_DUAL_SYS_MAX,
                                    &msg_content,
                                    FALSE,
                                    PNULL,
                                    0
                                    );

                if (PNULL != msg_content.wstr_ptr)
                {
                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
            }
            break;
#endif

        default:                        
            break;
        }
    break;

    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
      
        switch(focus_type) 
        {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
            	break;
            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_NONE:
#ifdef TTS_SUPPORT                
                SetShowSmsWinSoftKeyWithTTS(win_id, TRUE);
#else
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_REPLY, STXT_RETURN, TRUE);
#endif
                break;
            default:                        
                break;
        }
    break;    

    case MSG_PROMPTWIN_OK:
        // open waiting window

        {        
            //MMISMS_OperateDelorDelAll(win_data_ptr->cur_order_id,0,ShowSMSWinHandleDelWaitWinMsg);
            MMIPUB_CloseQuerytWin(&query_win_id);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    case MSG_CLOSE_WINDOW:
#ifdef TTS_SUPPORT		
        MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
#endif	

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        box_type = MMISMS_GetCurBoxType();
        SCI_TRACE_LOW("[MMISMS] HandleShowSMSWinMsg MSG_CLOSE_WINDOW box_type = %d", box_type);

        if ((box_type == MMISMS_BT_BOX_MT)
            || (box_type == MMISMS_BT_BOX_SENDSUCC)
            || (box_type == MMISMS_BT_BOX_SENDFAIL)
            || (box_type == MMISMS_BT_BOX_NOSEND))
        {
#ifndef WIN32

            SCI_TRACE_LOW("[MMISMS] HandleShowSMSWinMsg MSG_CLOSE_WINDOW");
            MMIPUB_CloseQuerytWin(&query_win_id);

            if(MMIAPIBT_IsConnectDevice() && MMIAPIBT_MapIsConnected())
            {
                SCI_TRACE_LOW("[MMISMS] HandleShowSMSWinMsg MMIAPIBT_IsConnectDevice() && MMIAPIBT_MapIsConnected()");
                if(MMISMS_MT_TO_BE_READ == cur_order_id->flag.mo_mt_type)
			    {
                    SCI_TRACE_LOW("[MMISMS] HandleShowSMSWinMsg MMISMS_MT_TO_BE_READ == cur_order_id->flag.mo_mt_type");

                    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
                    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID,IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleConnectWaitingMsg);            
                    MMIPUB_SetWinSoftkey(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
                    
                    record_id = g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]->record_id;
                    SCI_TRACE_LOW("[MMISMS] HandleShowSMSWinMsg record_id = %d", record_id);
                    MMISMS_GetHandleInfoByRecordId(record_id, handle_number, &is_extra_msg);
                    SCI_TRACE_LOW("[MMISMS] HandleShowSMSWinMsg handle_number = %s", handle_number);

                    MMIAPISMS_CreateMapSyncTimer();
                    BT_MapSetMessageStatus(handle_number, BT_MAP_SET_MSG_STATUS_READ, TRUE);
                }
            }            
            else
            {
                MMIAPISMS_HandleMsgReadStatus();
            }
#else
            MMIAPISMS_HandleMsgReadStatus();
#endif
        }
        else
#endif
{
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMISMS_UpdateUnreadOrder();
}

        SCI_FREE(win_data_ptr);
      
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if(win_data_ptr->cur_order_id != PNULL )
        {
            box_type = MMISMS_GetSmsBoxtype(win_data_ptr->cur_order_id);
        }
                
        switch ( box_type )
        {
        case MMISMS_BOX_MT:    
            if ( MMISMS_READ_STATUSREPORT == MMISMS_GetMtType() )
            {
                if (MMISMS_IsUpdateOk() && !MMISMS_CurMsgIsLocked())
                {
                    //询问是否删除？
                    MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
            }
            else
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_MTBOX_OPT;
                menuparam.win_id =   MMISMS_MTBOX_WIN_ID;
                menuparam.ctrl_id =  MMISMS_MTBOX_OPT_MENU_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;       
                menuparam.func.OnMenuInit = HandleShowSMSWinMTBoxOptInit;  
                MMIPUB_OpenPubMenuWin( &menuparam );                
            }

            break;

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        case MMISMS_BT_BOX_MT:
            {
                if ( MMISMS_READ_STATUSREPORT == MMISMS_GetMtType() )
                {
                    if (MMISMS_IsUpdateOk() && !MMISMS_CurMsgIsLocked())
                    {
                        //???????
                        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                    }
                }
                else
                {
                    //MMK_CreateWin((uint32 *)MMISMS_BT_MTBOX_WIN_TAB,PNULL);
                    MMIPUB_MENU_PARAM_T menuparam = {0};
                    menuparam.group_id = MENU_SMS_BT_MTBOX_OPT;
                    menuparam.win_id =   MMISMS_BT_MTBOX_WIN_ID;
                    menuparam.ctrl_id =  MMISMS_BT_MTBOX_OPT_MENU_CTRL_ID;    
                    menuparam.dst_handle = win_data_ptr->win_id;
                    menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;       
                    menuparam.func.OnMenuInit = HandleShowSMSWinMTBoxOptInit;  
                    MMIPUB_OpenPubMenuWin( &menuparam );
                }
            }
            break;
           
#endif

        case MMISMS_BOX_SENDSUCC:            
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_SENDSUCC_OPT;
                menuparam.win_id =   MMISMS_SENDSUCCBOX_WIN_ID;
                menuparam.ctrl_id =  MMISMS_SENDSUCCBOX_OPT_MENU_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;   
                menuparam.func.OnMenuInit = HandleShowSMSWinSUCCBoxOptInit;   
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }

            break;

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        case MMISMS_BT_BOX_SENDSUCC:
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                
                //MMK_CreateWin((uint32 *)MMISMS_BT_SENDSUCCBOX_WIN_TAB,PNULL);
                menuparam.group_id = MENU_SMS_BT_SENDSUCC_OPT;
                menuparam.win_id =   MMISMS_BT_SENDSUCCBOX_WIN_ID;
                menuparam.ctrl_id =  MMISMS_BT_SENDSUCCBOX_OPT_MENU_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleShowSMSWinOptOk;       
                menuparam.func.OnMenuInit = HandleShowSMSWinSUCCBoxOptInit;  
                MMIPUB_OpenPubMenuWin( &menuparam );
            }
            break;
#endif

            case MMISMS_BOX_SENDFAIL:
            case MMISMS_BOX_NOSEND:
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
            case MMISMS_BT_BOX_SENDFAIL:
            case MMISMS_BT_BOX_NOSEND:
#endif
            default:
                break;
            }

        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
   
        // the MT SMS list box is opened
        //MMK_PostMsg(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_UPDATE_LIST, PNULL,0);
        MMK_CloseWin(win_id);

        break;

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_CTL_TITLE_MOVE_NEXT:
    case MSG_CTL_TITLE_MOVE_PREV:
#ifdef TTS_SUPPORT		
        cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
        total_item       = GUILIST_GetTotalItemNum(cur_active_ctrl_id);
        //+CR243394
        if (total_item>1)
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
            is_tts_played = FALSE;
        }
        //-CR243394
#endif        

        if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
        {
            msg_id = MSG_APP_RIGHT;
        }
        else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
        {
            msg_id = MSG_APP_LEFT;
        }
         
        // the MT SMS list box is opened
        //MMK_PostMsg(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_UPDATE_LIST, PNULL,0);
            
        if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID) || MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
        {           
            HandleLeftRightMsgInShowSmsWin(MMISMS_MSGBOX_MAIN_WIN_ID, msg_id);
        }
        else if(MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
        {
            HandleLeftRightMsgInShowSmsWin(MMISMS_SECURITYBOX_CHILD_WIN_ID, msg_id);
        }
        break;

    default:
        ret_code = MMI_RESULT_FALSE;
        break;
    }
    
    return ret_code;
}
#endif

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint8 *number_ptr = PNULL;
    uint8 num_len = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject                 = {0};
    CC_RESULT_E cc_result = 0;
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};
    uint16 index = 0;
    wchar *focus_num_ptr = PNULL;
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    cur_order_id = win_data_ptr->cur_order_id;
    if(PNULL == cur_order_id)
    {
        SCI_TRACE_LOW("mmisms cur_order_id PNULL");
        return;
    }
    
    GUIRICHTEXT_GetFocusElement(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID, &richtext_element);
    GUIRICHTEXT_GetFocusItemIndex(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,&index);
    if(index == 0)
    {
        //num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

        if(MN_NUM_TYPE_ALPHANUMERIC == win_data_ptr->read_msg.address.number_type)
        {
            return;
        }
        else
        {

            num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(win_data_ptr->read_msg.address.number_type),
                                                                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                    win_data_ptr->read_msg.address.num_len),
                                                                    win_data_ptr->read_msg.address.party_num,
                                                                    number,
                                                                    sizeof(number)
                                                                    );
            
        }

        
        number_ptr = SCI_ALLOC_APP(num_len + 1);
        SCI_MEMSET(number_ptr, 0, (num_len + 1));
        SCI_MEMCPY(number_ptr,number,num_len);
    }
    else
    {
        focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
        if (PNULL == focus_num_ptr)
        {
            //SCI_TRACE_LOW:"MMISMS:HandleShowSMSWinMsg focus_num_ptr = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_12170_112_2_18_2_54_46_765,(uint8*)"");
            return;
        }
        //SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
        MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
        number_ptr = SCI_ALLOC_APP(richtext_element.data.len + 1);
        SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
        MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
        SCI_FREE(focus_num_ptr);
        focus_num_ptr = PNULL;
    }
  
    switch (menu_id)
    {
        case MMISMS_MENU_DIAL_ITEM_ID:
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
        case MMISMS_MENU_IPDIAL_ITEM_ID:
            num_len = SCI_STRLEN((char *)number_ptr);
            if (MMISMS_MENU_DIAL_ITEM_ID == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_NORMAL_CALL,
                                PNULL
                            );
            }
            else if (MMISMS_MENU_VIDEOCALL_ITEM_ID == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_VIDEO_CALL,
                                PNULL
                            );
            }
            else
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_IP_CALL,
                                PNULL
                            );
            }
            break;
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:
            MMIAPISMS_AnswerMessage((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, PNULL, number_ptr,
                                    (uint8)(strlen((char *)number_ptr) > MMIMMS_MAX_ADDRESS_LEN ? MMIMMS_MAX_ADDRESS_LEN : strlen((char *)number_ptr)));
            break;
        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
        {
            num_len = strlen((char *)number_ptr);
            //answer mms
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                string_to.wstr_ptr = (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
                SCI_MEMSET(string_to.wstr_ptr, 0x00, (num_len * sizeof(wchar)));
                MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)number_ptr, num_len, num_len);
                string_to.wstr_len = num_len;
                MMIAPIMMS_GetSubjectById(cur_order_id->record_id, &string_subject);
                MMIAPIMMS_AnswerMMS((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, &string_subject, &string_to);
                SCI_FREE(string_to.wstr_ptr);
                string_to.wstr_ptr = PNULL;
            }
        }
        break;

        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            // save to phonebook
            MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);

            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY(
                pb_bcd.number,
                party_num.bcd_num,
                pb_bcd.number_len
            );
            MMIAPIPB_AddContactWin(&pb_bcd, PB_GROUP_ALL, MMIPB_ADD_TYPE_NUMBER);
            //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
        }
        break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            // save to phonebook
            MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);

            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY(
                pb_bcd.number,
                party_num.bcd_num,
                pb_bcd.number_len
            );
            //  MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
            MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
        }
        break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
        case MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:
            HandleShowSmsAddSenderToBlackList((uint8 *)number_ptr);
            break;
#endif

        default :
            break;
    }    
    SCI_FREE(number_ptr);
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinExtractUrlOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    wchar  *url_winparam_ptr              = PNULL;
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_CUSTOM_TEXT_T pb_url = {0};
#endif

    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};

    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    GUIRICHTEXT_GetFocusElement(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID, &richtext_element);
        
    url_winparam_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1) * sizeof(wchar));
    if( PNULL == url_winparam_ptr )
    {
        SCI_TRACE_LOW("mmisms alloc error");
        return;
    }     
    MMIAPICOM_Wstrncpy(url_winparam_ptr, richtext_element.data.str_ptr, richtext_element.data.len);

    switch (menu_id)
    {
        case MMISMS_MENU_EXACT_URL_BROWSE_ITEM_ID:
        {
             
            uint8 *url_ptr = PNULL;
            uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
    
            url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));
    
            SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
    
            GUI_WstrToUTF8(url_ptr, str_len, url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));
    
#ifdef BROWSER_SUPPORT
            {
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    
                entry_param.type = MMIBROWSER_ACCESS_URL;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
#ifdef CMCC_UI_STYLE
                entry_param.is_alert = TRUE;
#endif
                MMIAPIBROWSER_Entry(&entry_param);
            }
#endif
            SCI_FREE(url_ptr);
            url_ptr = PNULL;
          
        }
        break;
        case MMISMS_MENU_EXTRACT_URL_ADDTOBOOKBARK_ITEM_ID:
        {
            uint8 *url_ptr = PNULL;
            uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
    
            url_ptr = SCI_ALLOCAZ(str_len * sizeof(uint8));
            if ( PNULL == url_ptr)
            {
                break;
            }
    
            //SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
    
            GUI_WstrToUTF8(url_ptr, str_len, url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));
    #ifdef BROWSER_SUPPORT
            {
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    
                entry_param.type = MMIBROWSER_ADD_BOOKMARK;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
                MMIAPIBROWSER_Entry(&entry_param);
            }
    #endif
            SCI_FREE(url_ptr);
            url_ptr = PNULL;
        }
        break;
    
    #ifndef PB_SUPPORT_LOW_MEMORY
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            uint16 str_len = (uint16)MMIAPICOM_Wstrlen(url_winparam_ptr);
    
            if (str_len > 0)
            {
                str_len = MIN(str_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));
                MMIAPICOM_Wstrncpy(pb_url.wstr,
                                   url_winparam_ptr,
                                   str_len);
                pb_url.wstr_len = str_len;
                MMIAPIPB_AddContactWin(&pb_url, PB_GROUP_ALL, MMIPB_ADD_TYPE_URL);
            }
        }
        break;
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            uint16 str_len = (uint16)MMIAPICOM_Wstrlen(url_winparam_ptr);
    
            if (str_len > 0)
            {
                str_len = MIN(str_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));
                MMIAPICOM_Wstrncpy(pb_url.wstr,
                                   url_winparam_ptr,
                                   str_len);
                pb_url.wstr_len = str_len;
                // MMIAPIPB_OpenEntryListForEx(&pb_url,MMIPB_ADD_TYPE_URL);
                MMIAPIPB_AddToExistContact(&pb_url, MMIPB_ADD_TYPE_URL, PNULL);
            }
        }
        break;
    #endif
    #endif
    
        default :
            break;
    }
    
    SCI_FREE(url_winparam_ptr);     
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleShowSmsAddSenderToBlackList(
    uint8 *num_array_ptr    //IN:
)
{
    MMIPB_BCD_NUMBER_T    pb_bcd = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};

    MMIAPICOM_GenPartyNumber(num_array_ptr, (uint16)SCI_STRLEN((char *)num_array_ptr), &party_num);
    pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
    SCI_MEMCPY(
        pb_bcd.number,
        party_num.bcd_num,
        pb_bcd.number_len
    );
    MMISMS_AddSenderToBlackList(pb_bcd);
}
#endif
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL; 
    uint32 marked_sms_num = 0;
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    
    switch(menu_id)
    {
        case MMISMS_MENU_DEL_ITEM_ID:
            HandleShowSMSWinDel(win_data_ptr);
            break;
            
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:
            MMISMS_AnswerSms(win_data_ptr->cur_order_id,MMISMS_BOX_NONE,0,&marked_sms_num);
            break;
            
        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
            MMISMS_AnswerMMS(win_data_ptr->cur_order_id,MMISMS_BOX_NONE,0,&marked_sms_num);
            break;
            
        case MMISMS_MENU_FORWARD_ITEM_ID:
            HandleShowSMSWinForwardSms();         
            break;
            
        case MMISMS_MENU_DIAL_ITEM_ID:
#ifdef BT_DIALER_SUPPORT
        case MMISMS_MENU_SIM_DIAL_ITEM_ID:
#endif
            MMISMS_ReplyCall(win_data_ptr->cur_order_id,CC_CALL_NORMAL_CALL);
            break;
#ifdef VT_SUPPORT            
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
            MMISMS_ReplyCall(win_data_ptr->cur_order_id,CC_CALL_VIDEO_CALL);
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case MMISMS_MENU_IPDIAL_ITEM_ID:  
            MMISMS_ReplyCall(win_data_ptr->cur_order_id,CC_CALL_IP_CALL);
            break;
#endif    

#ifdef BT_DIALER_SUPPORT
        case MMISMS_MENU_BT_DIAL_ITEM_ID:
            MMISMS_ReplyCall(win_data_ptr->cur_order_id,CC_CALL_BT_CALL);
            break;
#endif

#ifdef MMI_SMS_MOVE_SUPPORT 
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_MENU_BOX_SECURITY_ITEM_ID:
        case MMISMS_MENU_BOX_PREVIEW_ITEM_ID:
            HandleShowSmsMoveToMe(win_data_ptr);
            break;
#endif            
            
        case MMISMS_MENU_BOX_MOVETOME_ITEM_ID:
            HandleShowSmsMoveToMe(win_data_ptr);         
            break;
            
        case MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID:
            HandleShowSmsMoveToSIM(win_data_ptr);         
            break;    
#endif

#ifdef MMI_SMS_COPY_SUPPORT 
        case MMISMS_MENU_BOX_COPYTOME_ITEM_ID:
            HandleShowSmsCopyToMe(win_data_ptr);         
            break; 

        case MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID:
            HandleShowSmsCopyToSIM(win_data_ptr);         
            break;             
#endif            

        case MMISMS_MENU_DETAIL_ITEM_ID:
            MMK_CreateWin((uint32 *)MMISMS_MTSMS_DETAIL_TAB, PNULL);
            break;

        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
                uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
                MMISMS_GetCurNumByOrder(win_data_ptr->cur_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);
                MMISMS_AddToPbNew(win_data_ptr->cur_order_id,(uint8*)number_arr);
            }
            break;
        
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT    
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
                uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
                MMISMS_GetCurNumByOrder(win_data_ptr->cur_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);
                MMISMS_AddToPbExist(win_data_ptr->cur_order_id,(uint8*)number_arr);
            }
            break;
    #endif    
    
    #if defined(MMI_BLACKLIST_SUPPORT)
        case MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:
            {
                MMIPB_BCD_NUMBER_T    pb_bcd = {0};
                MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
                MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
                uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
                MMISMS_GetCurNumByOrder(win_data_ptr->cur_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);            
                MMIAPICOM_GenPartyNumber(number_arr,(uint16)SCI_STRLEN((char *)number_arr), &party_num);
                pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
                MMISMS_AddSenderToBlackList(pb_bcd);
            }
            break;
    #endif       

#ifdef SNS_SUPPORT
        case MMISMS_MENU_SHARE_TO_SINA_ID:
        case MMISMS_MENU_SHARE_TO_FACEBOOK_ID:
        case MMISMS_MENU_SHARE_TO_TIWTTER_ID:
            HandleShowSmsWinShareSNS(win_data_ptr);
            break;
#endif       

        default:
        break;
    }
}

/*****************************************************************************/
//     Description : handle mt box opt init
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinMTBoxOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL; 
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    uint32 common_group_id = 0;
  
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }  
               
    menu_enable_t.cur_order_id = win_data_ptr->cur_order_id;
    menu_enable_t.win_id = MMISMS_MTBOX_WIN_ID;
    common_group_id = MENU_SMS_MTBOX_OPT;
    
#ifdef MMI_PDA_SUPPORTs
    menu_enable_t.group_id = common_group_id;
#else
    #if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
    #endif
#endif

    MMISMS_HandleDialMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT     
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT     
    menu_enable_t.group_id = MENU_SMS_COPY_OPT;
    MMISMS_HandleCopyMenuEnableGray(&menu_enable_t);
#endif

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);
            
    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);  

    if(MMISMS_FOLDER_SECURITY == MMISMS_CurMsgFolderType())
    {
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        MMIAPICOM_EnableGrayed(MMISMS_MTBOX_WIN_ID,MENU_SMS_SECURITY_BOX_OPT,MMISMS_MENU_BOX_SECURITY_ITEM_ID,TRUE);
#endif
        MMIAPICOM_EnableGrayed(MMISMS_MTBOX_WIN_ID,MENU_SMS_SECURITY_BOX_OPT,MMISMS_MENU_BOX_PREVIEW_ITEM_ID,FALSE);
    }
    else
    {
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        MMIAPICOM_EnableGrayed(MMISMS_MTBOX_WIN_ID,MENU_SMS_SECURITY_BOX_OPT,MMISMS_MENU_BOX_SECURITY_ITEM_ID,FALSE);
#endif
        MMIAPICOM_EnableGrayed(MMISMS_MTBOX_WIN_ID,MENU_SMS_SECURITY_BOX_OPT,MMISMS_MENU_BOX_PREVIEW_ITEM_ID,TRUE);        
    }   
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinClass0OptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    uint16 num_len = 0;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;    
    uint8  number[MMISMS_PBNUM_MAX_LEN+2] = {0};
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    switch(menu_id)
    {
        case MMISMS_MENU_DIAL_ITEM_ID:
        case MMISMS_MENU_VIDEOCALL_ITEM_ID: 
        case MMISMS_MENU_IPDIAL_ITEM_ID:
        num_len = MMIAPICOM_GenDispNumber(
                win_data_ptr->read_msg.address.number_type,
                MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->read_msg.address.num_len),
                win_data_ptr->read_msg.address.party_num,
                number,
                MMISMS_PBNUM_MAX_LEN + 2
                );     
    
        if (num_len > 0)
        {
            CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
    
            if (menu_id == MMISMS_MENU_DIAL_ITEM_ID)
            {
                cc_result = MMIAPICC_MakeCall(
                                                                win_data_ptr->read_msg.dual_sys,
                                                                number,
                                                                num_len,
                                                                PNULL,
                                                                PNULL,
                                                                CC_CALL_SIM_MAX,
                                                                CC_CALL_NORMAL_CALL,
                                                                PNULL
                                                                );
            }
    #ifdef VT_SUPPORT
            else if (menu_id == MMISMS_MENU_VIDEOCALL_ITEM_ID)
            {
                cc_result = MMIAPICC_MakeCall(
                                                                win_data_ptr->read_msg.dual_sys,
                                                                number,
                                                                num_len,
                                                                PNULL,
                                                                PNULL,
                                                                CC_CALL_SIM_MAX,
                                                                CC_CALL_VIDEO_CALL,
                                                                PNULL
                                                                );
            }
    #endif
    #ifdef MMI_IP_CALL_SUPPORT
            else
            {
                cc_result = MMIAPICC_MakeCall(
                                                                win_data_ptr->read_msg.dual_sys,
                                                                number,
                                                                num_len,
                                                                PNULL,
                                                                PNULL,
                                                                CC_CALL_SIM_MAX,
                                                                CC_CALL_IP_CALL,
                                                                PNULL
                                                                );
            }
    #endif
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        }
    
        break;
    
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    
            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(win_data_ptr->read_msg.address.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(win_data_ptr->read_msg.address.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY( 
                pb_bcd.number,  
                win_data_ptr->read_msg.address.party_num, 
                pb_bcd.number_len 
                );
            MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
            //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
        }
        break;
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT            
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    
            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(win_data_ptr->read_msg.address.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(win_data_ptr->read_msg.address.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY( 
                pb_bcd.number,  
                win_data_ptr->read_msg.address.party_num, 
                pb_bcd.number_len 
                );
            //MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
            MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
    
        }
        break;
    #endif
        default:
            break;     
    }
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinClass0Init(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;    
#ifdef MMI_PDA_SUPPORT
    uint16 num_len = 0;
    MMI_STRING_T title_text = {0};
    wchar wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8  number[MMISMS_PBNUM_MAX_LEN+2] = {0};
#endif
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
#ifdef MMI_PDA_SUPPORT
    num_len = MMIAPICOM_GenDispNumber(
    win_data_ptr->read_msg.address.number_type,
    MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->read_msg.address.num_len),
    win_data_ptr->read_msg.address.party_num,
    number,
    MMISMS_PBNUM_MAX_LEN + 2
    );    
    MMI_STRNTOWSTR(wstr, MMISMS_PBNUM_MAX_LEN + 2, (const uint8 *)number, MMISMS_PBNUM_MAX_LEN + 2, num_len);
    title_text.wstr_ptr = wstr;
    title_text.wstr_len = num_len;
    GUIMENU_SetMenuTitle(&title_text , MMISMS_CLASS0_OPTION_POP_CTRL_ID);
#endif
}


/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinSUCCBoxOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;   
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    
    menu_enable_t.cur_order_id = win_data_ptr->cur_order_id;
    menu_enable_t.win_id = MMISMS_SENDSUCCBOX_WIN_ID;
    common_group_id = MENU_SMS_SENDSUCC_OPT;    
#ifdef MMI_PDA_SUPPORTs
    menu_enable_t.group_id = common_group_id;
#else
    #if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
    #endif
#endif
    if(menu_enable_t.group_id != MENU_SMS_MTLIST_ANSWER_OPT)
    {
        menu_enable_t.group_id = common_group_id;
    }
    MMISMS_HandleDialMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT     
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);
    
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinExtractEmailOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint8 num_len = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject                 = {0};
    uint8 *email_ptr = PNULL;
    MMIPB_MAIL_T pb_email = {0};

    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};
        
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    cur_order_id = win_data_ptr->cur_order_id;
    GUIRICHTEXT_GetFocusElement(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID, &richtext_element);
    
    //进入文件管理器界面选择文件

    email_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1) * sizeof(wchar));
    if( PNULL == email_ptr )
    {
        SCI_TRACE_LOW("mmisms alloc error");
        return;
    }     

    MMI_WSTRNTOSTR((uint8 *)email_ptr, richtext_element.data.len , richtext_element.data.str_ptr, richtext_element.data.len, richtext_element.data.len);
    
    switch (menu_id)
    {
        case MMISMS_MENU_EXTRACT_SEND_MMS_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0 && cur_order_id != PNULL)
            {
                num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                string_to.wstr_ptr = (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
                MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                string_to.wstr_len = num_len;
                MMIAPIMMS_GetSubjectById(cur_order_id->record_id, &string_subject);
                MMIAPIMMS_AnswerMMS((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, &string_subject, &string_to);
                SCI_FREE(string_to.wstr_ptr);
                string_to.wstr_ptr = PNULL;
            }
        }
        break;
#ifdef MMIEMAIL_SUPPORT
		case MMISMS_MENU_EXTRACT_SEND_EMAIL_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0 && cur_order_id != PNULL)
            {
                num_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                string_to.wstr_ptr = (wchar*)SCI_ALLOC_APPZ((MMIPB_MAX_MAIL_LEN+1) * sizeof(wchar));
                MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                string_to.wstr_len = num_len;
                MMIAPIEMAIL_EditEmail(&string_to);
                SCI_FREE(string_to.wstr_ptr);
                string_to.wstr_ptr = PNULL;
            }
        }
        break;
#endif
    
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMSET(&pb_email, 0x0, sizeof(MMIPB_MAIL_T));
                MMI_STRNTOWSTR(pb_email.wstr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                pb_email.wstr_len = num_len;
                MMIAPIPB_AddContactWin(&pb_email, PB_GROUP_ALL, MMIPB_ADD_TYPE_EMAIL);
            }
        }
        break;
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMSET(&pb_email, 0x0, sizeof(MMIPB_MAIL_T));
                MMI_STRNTOWSTR(pb_email.wstr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                pb_email.wstr_len = num_len;
                // MMIAPIPB_OpenEntryListForEx(&pb_email,MMIPB_ADD_TYPE_EMAIL);
                MMIAPIPB_AddToExistContact(&pb_email, MMIPB_ADD_TYPE_EMAIL, PNULL);
            }
        }
        break;
    #endif
        default :
            break;
    }
    
    SCI_FREE(email_ptr);    
}

#ifdef TTS_SUPPORT
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:jun.hu
//    Note: 
/*****************************************************************************/
LOCAL void SetShowSmsWinSoftKeyWithTTS(MMI_WIN_ID_T    win_id,        //IN:
                                       BOOLEAN         is_need_update // whether update
                                       )
{
    
    MMI_TEXT_ID_T   leftsoft_id = TXT_NULL;
    if (MMISMS_READ_STATUSREPORT == MMISMS_GetMtType())
    {
        leftsoft_id = TXT_DELETE;
    }
    else
    {
        leftsoft_id = STXT_OPTION;
    }
    // 如果TTS正在播放....
    if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, STXT_STOP, STXT_RETURN, is_need_update);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, TXT_SET_TTS_PLAY, STXT_RETURN, is_need_update);
    }
}
#endif

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinDel(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_OpenDelteQueryWin(MMISMS_DELETE_SINGLE,FALSE,win_data_ptr->cur_order_id,MMISMS_BOX_NONE,HandleShowSmsWinDelQueryWinMsg);
    }    
}

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinForwardSms(void)
{
    MMI_STRING_T    msg_content = {0};
    MMISMS_CloseEditWin();
    // set the init SMS content
    MMISMS_GetSMSContent(&msg_content);
    MMISMS_ClearDestNum(); 
    // open edit window
    MMISMS_WriteNewMessage(
                            MN_DUAL_SYS_MAX,
                            &msg_content,
                            FALSE,
                            PNULL,
                            0
                            );
    
    if (PNULL != msg_content.wstr_ptr)
    {
        SCI_FREE(msg_content.wstr_ptr);
        msg_content.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsDeleteCallBack(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    SCI_TRACE_LOW("mmisms enter HandleShowSmsDeleteCallBack");
    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
    MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
    MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
	MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);

    MMISMS_SetDelAllFlag(FALSE);  
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    
    return TRUE;
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsWinDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_OPERATE_DATA_INFO_T data_info = {0};
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        return recode;
    }
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        //MMISMS_OperateDelorDelAll(win_data_ptr->cur_order_id,0,ShowSMSWinHandleDelWaitWinMsg);
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
#endif
        MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,TRUE);
        data_info.box_type = MMISMS_GetSmsStateBoxtype(win_data_ptr->cur_order_id);        
        MMISMS_EnterDelWaitWin(&data_info,HandleShowSmsDeleteCallBack,(void*)win_data_ptr);
        
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

#ifdef MMI_SMS_COPY_SUPPORT

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  ShowSmsCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    
    if (isReturnOK)
    {
        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
        }
        else
        {  
            if (dual_sys == win_data_ptr->cur_order_id->flag.dual_sys)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_NONE, 0, win_data_ptr->cur_order_id, HandleShowSmsCopyQueryWinMsg);
                
            }
            
        }
    }
    
    return recode;    
}

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsCopyToSIM(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        uint16 dual_sys = 0;
        MN_DUAL_SYS_E copy_dual_sys = MN_DUAL_SYS_1;
   
        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
        MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,TRUE);
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_SIM);
    
        if (0 == sim_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);            
        }
        else if (1 == sim_num)
        {
            if (MN_SMS_STORAGE_SIM == win_data_ptr->cur_order_id->flag.storage)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
                
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
            } 
            else
            {
                for(dual_sys=0;dual_sys<MN_DUAL_SYS_MAX;dual_sys++)
                {
                    if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys))
                    {
                        copy_dual_sys = dual_sys;
                    }
                }
                
                
                MMISMS_SetCurSaveDualSys(copy_dual_sys);
                MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_NONE, 0, win_data_ptr->cur_order_id, HandleShowSmsCopyQueryWinMsg);
            }
            
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(ShowSmsCopySelectSimCallback,PNULL,FALSE);
        }
#endif
    }    
}

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsCopyToMe(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_ME);
        MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,TRUE);
        //MsgBoxOpenCopyQueryWinMsg(win_data_ptr);  
        MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_NONE, 0, win_data_ptr->cur_order_id, HandleShowSmsCopyQueryWinMsg);
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsCopyCallBack(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr)
{
    MMI_RESULT_E ret = FALSE;
    if(PNULL == data_info_ptr)
    {
        return FALSE;
    }
    if(!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }
    MMISMS_SetMoveAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
    SCI_TRACE_LOW("mmisms copy ret value %d",data_info_ptr->ret_value);
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        MMISMS_WriteFolderType();                
        ret = TRUE;
    }
    else if(MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {  
        MMISMS_OpenMsgBox(TXT_NO_SPACE, IMAGE_PUBWIN_WARNING);      
        ret = FALSE;
    }     
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {   
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);     
        ret = FALSE;
    }
    MMISMS_ClearOperInfo();
    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    return ret;
}
/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsCopyQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID; 
    MMISMS_COPY_DATA_INFO_T copy_data_info = {0};
     
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        //MMISMS_OperateCopy(win_data_ptr->cur_order_id,0,HandleShowSmsCopyWaitWinMsg);
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
#endif

        MMISMS_SetMoveAllFlag(TRUE);
        copy_data_info.box_type = MMISMS_GetSmsStateBoxtype(win_data_ptr->cur_order_id);
        copy_data_info.oper_status = MMISMS_GetCurrentOperStatus();
        MMISMS_EnterCopyWaitWin(&copy_data_info,HandleShowSmsCopyCallBack,win_data_ptr); 
        
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

#endif

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  ShowSmsMoveSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);
    
    if(isReturnOK)
    {
        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
        }
        else
        {
            if (dual_sys == win_data_ptr->cur_order_id->flag.dual_sys)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_MOVE);
            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                //MsgBoxOpenMoveQueryWinMsg(win_data_ptr);
                MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_NONE,0,win_data_ptr->cur_order_id,HandleShowSmsMoveQueryWinMsg);
        
            }
        }
    }
    
    return recode;
}


/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsMoveToSIM(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        uint16 dual_sys = 0;
        MN_DUAL_SYS_E move_dual_sys = MN_DUAL_SYS_1;
   
        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
        MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,TRUE);
        MMISMS_SetMoveOperStatus(MMISMS_MOVE_SMS_TO_SIM);
    
        if (0 == sim_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);            
        }
        else if (1 == sim_num)
        {
            if (MN_SMS_STORAGE_SIM == win_data_ptr->cur_order_id->flag.storage)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);
                
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_MOVE);
            } 
            else
            {
                for(dual_sys=0;dual_sys<MN_DUAL_SYS_MAX;dual_sys++)
                {
                    if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys))
                    {
                        move_dual_sys = dual_sys;
                    }
                }
                MMISMS_SetCurSaveDualSys(move_dual_sys);
                MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_NONE,0,win_data_ptr->cur_order_id,HandleShowSmsMoveQueryWinMsg);
            }
            

        }
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(ShowSmsMoveSelectSimCallback,(ADD_DATA)win_data_ptr,FALSE);
            //MMK_CreateWin((uint32 *)MMISMS_MOVE_SELECT_SIM_WIN_TAB, PNULL);
        }
    #endif
    }
}


/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsMoveToMe(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_NONE,0,win_data_ptr->cur_order_id,HandleShowSmsMoveQueryWinMsg);
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsMoveCallBack(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr)
{
    MMI_RESULT_E ret = FALSE;
    if(PNULL == data_info_ptr)
    {
        return FALSE;
    }
    if(!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }
    MMISMS_SetMoveAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
    SCI_TRACE_LOW("mmisms move ret value %d",data_info_ptr->ret_value);
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        MMISMS_WriteFolderType();
        
        MMISMS_ClearOperInfo();
        ret = TRUE;
    }
    else if(MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {  
        MMISMS_OpenMsgBox(TXT_NO_SPACE, IMAGE_PUBWIN_WARNING);
        MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);        
        MMISMS_ClearOperInfo();   
        ret = FALSE;
    }     
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {   
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);  
        MMISMS_ClearOperInfo();     
        ret = FALSE;
    }
    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    return ret;
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSmsMoveQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID; 
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_MOVE_DATA_INFO_T move_data_info = {0};
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:        
        //MMISMS_OperateMove(win_data_ptr->cur_order_id,0,HandleShowSmsMoveWaitWinMsg);                
        move_data_info.box_type = MMISMS_GetSmsStateBoxtype(win_data_ptr->cur_order_id);
        move_data_info.oper_status = MMISMS_GetCurrentMoveOperStatus();
        MMISMS_EnterMoveWaitWin(&move_data_info, HandleShowSmsMoveCallBack, PNULL);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

        MMISMS_SetCurOrderMarked(win_data_ptr->cur_order_id,FALSE);

        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

#endif

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence :
//  Author: renwei
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleShowSmsWinSnsInstallWarningWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    //int            err             = 0;
    int            widget_id       = 0;
    MMI_RESULT_E   recode          = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    widget_id = pubwin_info_ptr->user_data;

    switch (msg_id)
    {
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&win_data_ptr->pMe);

            if (win_data_ptr->pMe)
            {
                IMMISNS_AppInstall(win_data_ptr->pMe, widget_id);

                IMMISNS_Release(win_data_ptr->pMe);
                win_data_ptr->pMe         = PNULL;
            }
            break;
        }
        case MSG_APP_CANCEL:
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }

    return recode;
}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSmsWinShareSNS(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr)
{
    int share_id = 0;

    if (MMISMS_MENU_SHARE_TO_SINA_ID == win_data_ptr->menu_id)
    {
        share_id = MMI_SNS_SINA;
    }
    else if (MMISMS_MENU_SHARE_TO_FACEBOOK_ID == win_data_ptr->menu_id)
    {
        share_id = MMI_SNS_FACEBOOK;
    }
    else
    {
        share_id = MMI_SNS_TIWTTER;
    }

    IMMISNS_CreateObject(&win_data_ptr->pMe);

    if (win_data_ptr->pMe)
    {
        if (IMMISNS_IsFixAppInstalled(win_data_ptr->pMe, share_id) < 0)
        {
            IMMISNS_Release(win_data_ptr->pMe);
            win_data_ptr->pMe = PNULL;

            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(), PNULL, TXT_WHETHER_INSTALL, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_OKCANCEL, HandleShowSmsWinSnsInstallWarningWinMsg, share_id);
        }
        else
        {
            MMI_STRING_T msg_content = {0};

            MMISMS_GetSMSContent(&msg_content);

            if (PNULL != msg_content.wstr_ptr)
            {
                uint32        time_num    = 500;

                IMMISNS_ShareToSns(win_data_ptr->pMe, share_id, SNS_MODULE_SMS , (uint32)&msg_content);
                MMIPUB_OpenAlertWinByTextId(&time_num, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

                SCI_FREE(msg_content.wstr_ptr);
                msg_content.wstr_ptr = PNULL;
            }
        }
    }   
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get richtext rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetRichtextRect(GUI_RECT_T *rect_ptr)
{
    uint16 mainscreen_height          = 0;
    uint16 mainscreen_width           = 0;


    if (PNULL == rect_ptr)
    {
        return FALSE;
    }
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return FALSE;
    }

    rect_ptr->left = 0;
    rect_ptr->top = MMI_TITLE_HEIGHT +MMI_STATUSBAR_HEIGHT - 1;
    rect_ptr->right = mainscreen_width;
    rect_ptr->bottom = mainscreen_height - SMS_BUTTONFORM_HEIGHT - 1;  // 40 BUTTON FORM HEIGHT
    return TRUE;
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSMSPenOkMsg(MMI_WIN_ID_T win_id,         //in:
                              MMI_CTRL_ID_T ctrl_id  )
{

    MMI_CTRL_ID_T cur_ctrl_id = 0;
    MN_DUAL_SYS_E     dual_sys = MN_DUAL_SYS_MAX;
    uint16 cur_index = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_BOX_TYPE_E cur_box_type = MMISMS_BOX_NONE;
    uint8 num_len= 0;
    uint8 number [MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return FALSE;       
    }
    cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
    cur_box_type = MMISMS_GetCurBoxType();
    cur_order_id = MMISMS_GetLinkedArrayPtrIdInList(cur_box_type, cur_index);
        
    switch (ctrl_id)
    {
    #ifdef TTS_SUPPORT
        case MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID:
         if(MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        else
        {
            MMI_STRING_T    msg_content = {0};
            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
            {
                MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
                MMISMS_SetTTSStopButtonForm();
            }
            else
            {
                MMISMS_GetSMSContent(&msg_content);
                if(PNULL != msg_content.wstr_ptr)
                {
                    BOOLEAN ret = FALSE;
                    GUI_BG_T button = {0};
                    MMITTS_PLAY_PARAM_T play_param = {0};
                
                    button.bg_type = GUI_BG_IMG;
                    button.img_id  = IMAGE_SMS_STOP_READ_SMS_PDA;
                    play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
                    play_param.txt_len_in_byte = msg_content.wstr_len * 2;
                    play_param.is_ucs2 = TRUE;
                    play_param.play_times = 1;
                    play_param.tts_vol = MMIAPISET_GetMultimVolume();
                    play_param.ring_type = TTS_RING_TYPE_SMS;
                    play_param.win_handle = MMISMS_SHOWMSG_WIN_ID;
					play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
					play_param.digit_mode = TTS_DIGIT_AUTO;
					play_param.punc_mode = TTS_PUNC_OFF;
                    play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;                    
                    ret = MMIAPITTS_PlayText(&play_param);
                    if(ret)
                    {
                        MMISMS_SetTTSPlayingButtonForm(TRUE);
                    }
                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
            }
        }
        break;
        #endif
        
        case MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID:
            //121788
            {

                uint32 marked_sms_num = 0;

				MMISMS_CloseEditWin();

                if(PNULL != cur_order_id)
                {
                    MMISMS_AnswerSms(cur_order_id,MMISMS_BOX_NONE,0,&marked_sms_num);
                }
            }

//        MMISMS_SetIsNeedLoadSaved(FALSE);

#if 0
        // get the address of the SMS message 
        num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

        //open edit window and set the address to the deliver address
        MMISMS_WriteNewMessage(
                                dual_sys,
                                PNULL,
                                FALSE,
                                number,
                                num_len
                                );
#endif

        break;
        case MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID:                      
        if(PNULL == cur_order_id)
        {
            break;
        }
        SCI_TRACE_LOW("MMISMS: msg_type = %d",cur_order_id->flag.msg_type);
        if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            if(MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {

                MMI_STRING_T    msg_content = {0};
                MMISMS_CloseEditWin();
                // set the init SMS content
                MMISMS_GetSMSContent(&msg_content);
                MMISMS_ClearDestNum(); 
                SCI_TRACE_LOW("MMISMS: MMISMS_WriteNewMessage 4545");
                // open edit window
                MMISMS_WriteNewMessage(
                                        MN_DUAL_SYS_MAX,
                                        &msg_content,
                                        FALSE,
                                        PNULL,
                                        0
                                        );

                if (PNULL != msg_content.wstr_ptr)
                {
                    SCI_FREE(msg_content.wstr_ptr);
                    msg_content.wstr_ptr = PNULL;
                }
            }
        }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.20
        else if (MMISMS_TYPE_MAIL == cur_order_id->flag.msg_type)
        {
            mail_ForwardMailByRecordId((uint32)cur_order_id->record_id);
        }
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
        else //IS MMS, not OTA and PUSH
        {
            MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_SENDBOX_LIST_FARWARD);
            MMIAPIMMS_FarwardMMS((uint32)cur_order_id->record_id);

            //MMIAPIMMS_OpMMS((uint32)cur_order_id->record_id, MMIMMS_OP_FOWARD, win_id);
            //MMK_CloseWin(win_id);
        }
        break;
        
        case   MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID:
            HandleShowSMSWinDel(win_data_ptr);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : to  the show button form
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MTBoxButtonFormInit(MMI_WIN_ID_T win_id)
{
    GUI_BG_T  bg = {0};
    GUI_RECT_T richtext_rect = {0};
    MMISMS_GetRichtextRect(&richtext_rect);             
    GUIRICHTEXT_SetRect(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID ,richtext_rect);
   
    GUIFORM_IsSlide(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, 0);
  
    bg.bg_type = GUI_BG_IMG;                     
#ifdef TTS_SUPPORT
    GUIBUTTON_SetRunSheen(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, FALSE);
#endif    
    GUIBUTTON_SetRunSheen(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, FALSE);     
    
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
    GUIFORM_SetBg(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, &bg);
    GUIFORM_SetMargin(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, 6);

    bg.img_id = IMAGE_SMS_BUTTON_NORMAL;        
    #ifdef TTS_SUPPORT
    GUIBUTTON_SetBg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);
    #endif
    GUIBUTTON_SetBg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);            
    
    bg.img_id = IMAGE_SMS_BUTTON_PRESSED;           
    #ifdef TTS_SUPPORT
    GUIBUTTON_SetPressedBg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);
    #endif
    GUIBUTTON_SetPressedBg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, &bg);
    GUIBUTTON_SetPressedBg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);
    GUIBUTTON_SetPressedBg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);

    bg.img_id =  IMAGE_SMS_READ_SMS_PDA;          
    #ifdef TTS_SUPPORT
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);
    #endif
    
    bg.img_id =  IMAGE_SMS_REPLY_SMS_PDA;          
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, &bg);

    bg.img_id =  IMAGE_SMS_FORWARD_SMS_PDA;          
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);

    bg.img_id =  IMAGE_SMS_DELETE_SMS_PDA;          
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);

 #if 0
    box_type = MMISMS_GetCurBoxType();
    if(box_type == MMISMS_BOX_SENDSUCC)
    {
        GUI_BG_T bg = {0};            
        bg.bg_type = GUI_BG_IMG;
        //bg.img_id = IMAGE_SMS_BUTTON_DISABLE;      
        //GUIBUTTON_SetBg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, &bg);            
        
        bg.img_id =  IMAGE_SMS_REPLY_GRAY_SMS_PDA;          
        GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID,TRUE, &bg,FALSE);       

        bg.img_id = IMAGE_SMS_BUTTON_DISABLE;  
        GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, TRUE, &bg, FALSE);                        
    }
#endif       

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//     Description : to  the show send box button form
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_SendBoxButtonFormInit(MMI_WIN_ID_T win_id)
{
    GUI_BG_T  bg = {0};
    GUI_RECT_T richtext_rect = {0};
    MMISMS_GetRichtextRect(&richtext_rect);        
    GUIRICHTEXT_SetRect(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID, richtext_rect);
 
    GUIFORM_IsSlide(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, 6);
  
    bg.bg_type = GUI_BG_IMG;                     

    GUIBUTTON_SetRunSheen(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, FALSE);     
    
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
    GUIFORM_SetBg(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, &bg);
    GUIFORM_SetMargin(MMISMS_MTBOX_BUTTON_FORM_CTRL_ID, 6);

    bg.img_id = IMAGE_SMS_BUTTON_NORMAL;        

    GUIBUTTON_SetBg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);            
    
    bg.img_id = IMAGE_SMS_BUTTON_PRESSED;           
    GUIBUTTON_SetPressedBg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);
    GUIBUTTON_SetPressedBg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);

    bg.img_id =  IMAGE_SMS_FORWARD_SMS_PDA;          
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, &bg);

    bg.img_id =  IMAGE_SMS_DELETE_SMS_PDA;          
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, &bg);
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//     Description : to  the show class0 sms button form
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_Class0SmsButtonFormInit(MMI_WIN_ID_T win_id)
{
    GUI_BG_T  bg = {0};
    GUI_RECT_T richtext_rect = {0};

    MMISMS_GetRichtextRect(&richtext_rect);            
    GUIRICHTEXT_SetRect(MMISMS_SHOWCLASS0_TEXTBOX_CTRL_ID, richtext_rect);
  
    GUIFORM_IsSlide(MMISMS_CLASS0_BUTTON_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMISMS_CLASS0_BUTTON_FORM_CTRL_ID, 6);
  
    bg.bg_type = GUI_BG_IMG;

    GUIBUTTON_SetRunSheen(MMISMS_CLASS0_BUTTON_FORM_CALL_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMISMS_CLASS0_BUTTON_FORM_SAVE_CTRL_ID, FALSE);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsDestroyButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMISMS_BUTTON_T *button_array,uint16 array_len)
{
    uint16 index = 0;
    if(0 != MMK_GetCtrlHandleByWin(win_id, ctrl_id))
    {
        MMK_DestroyControl(ctrl_id);
        for(index = 0; index < array_len; index++)
        {
            MMK_DestroyDynaCtrl(button_array[index].ctrl_id);
        }
    }
}

/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsConstructButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMISMS_BUTTON_T *button_array,uint16 array_len)
{
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    GUI_BG_T bg = {0};
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    uint16 index = 0;
    GUI_BOTH_RECT_T client_rect = {0};

    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    //create MMIIDLE_DIAL_BUTTON_FORM_CTRL_ID control
    form_init.both_rect.v_rect.left =  client_rect.v_rect.left;
    form_init.both_rect.v_rect.top = client_rect.v_rect.bottom - SMS_BUTTONFORM_HEIGHT + 1;
    form_init.both_rect.v_rect.right = client_rect.v_rect.right;
    form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom;
    
    form_init.both_rect.h_rect.left = client_rect.h_rect.left;
    form_init.both_rect.h_rect.top = client_rect.h_rect.bottom - SMS_BUTTONFORM_HEIGHT + 1;
    form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom;
    form_init.layout_type = GUIFORM_LAYOUT_SBS;    

    form_ctrl.ctrl_id           = ctrl_id;
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);
        
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
    GUIFORM_SetBg(ctrl_id, &bg);
    GUIFORM_SetMargin(ctrl_id, 6);
    //not slide
    GUIFORM_IsSlide(ctrl_id,FALSE);

    for(index = 0; index < array_len; index++)
    {    
        form_child_ctrl.child_handle = button_array[index].ctrl_id;
        form_child_ctrl.guid = SPRD_GUI_BUTTON_ID;
        
        button_init.bg.bg_type = GUI_BG_IMG;
        button_init.bg.img_id  = button_array[index].bg_img_id;
        form_child_ctrl.init_data_ptr = &button_init;
        
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;

        GUIFORM_CreatDynaChildCtrl(win_id, ctrl_id,  &form_child_ctrl);
        
        bg.img_id = button_array[index].pressed_bg_img_id;
        GUIBUTTON_SetPressedBg(button_array[index].ctrl_id, &bg);

        bg.img_id = button_array[index].fg_img_id;
        GUIBUTTON_SetFg(button_array[index].ctrl_id, &bg);

        bg.img_id = button_array[index].pressed_fg_img_id;
        GUIBUTTON_SetPressedFg(button_array[index].ctrl_id, &bg);        
    }
}

#ifdef TTS_SUPPORT
/*****************************************************************************/
//     Description : set  button form while playing
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_SetTTSPlayingButtonForm(BOOLEAN is_refresh)
{
    GUI_BG_T  bg = {0};
    bg.bg_type = GUI_BG_IMG;                         
    bg.img_id = IMAGE_SMS_STOP_READ_SMS_PDA;  
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);
    if(is_refresh)
    {
        GUIBUTTON_Update(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID);
    }
    
    bg.img_id =  IMAGE_SMS_REPLY_GRAY_SMS_PDA;          
    GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID,TRUE, &bg,is_refresh);       
    bg.img_id = IMAGE_SMS_BUTTON_DISABLE;  
    GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, TRUE, &bg, is_refresh);    

    bg.img_id =  IMAGE_SMS_FORWARD_GARY_SMS_PDA;          
    GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID,TRUE, &bg,is_refresh);       
    bg.img_id = IMAGE_SMS_BUTTON_DISABLE;  
    GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, TRUE, &bg, is_refresh);    

    bg.img_id =  IMAGE_SMS_DELETE_GRAY_SMS_PDA;          
    GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID,TRUE, &bg,is_refresh);       
    bg.img_id = IMAGE_SMS_BUTTON_DISABLE;  
    GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, TRUE, &bg, is_refresh);    
               
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//     Description : reset  button form after tts stop
//    Global resource dependence : 
//  Author:rong.gu 
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_SetTTSStopButtonForm(void)
{
    MMISMS_BOX_TYPE_E  box_type = MMISMS_BOX_NONE;     
    GUI_BG_T bg = {0};         
    box_type = MMISMS_GetCurBoxType();
    bg.bg_type = GUI_BG_IMG;         
    bg.img_id = IMAGE_SMS_READ_SMS_PDA; 
    GUIBUTTON_SetFg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);
    GUIBUTTON_SetPressedFg(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID, &bg);
    GUIBUTTON_Update(MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID);    

    GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID, FALSE, PNULL, TRUE);                        
    GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID,FALSE,PNULL,TRUE);       
       
    GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID, FALSE, PNULL, TRUE);                        
    GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID,FALSE,PNULL,TRUE);    
    if(box_type != MMISMS_BOX_SENDSUCC)
    {               
       GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID, FALSE, PNULL, TRUE);                        
       GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID,FALSE,PNULL,TRUE);    
    }
    else
    {   
        bg.bg_type = GUI_BG_IMG;        
        bg.img_id = IMAGE_SMS_BUTTON_DISABLE;  
        GUIBUTTON_SetGrayed(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID,TRUE, &bg,FALSE);             
        bg.img_id =  IMAGE_SMS_REPLY_GRAY_SMS_PDA;          
        GUIBUTTON_SetGrayedFg(MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID,TRUE, &bg,FALSE);              
    }

    return MMI_RESULT_TRUE;
}
#endif
#endif

/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL BOOLEAN ShowSmsWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T richtext_ctrl_id = MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID;
    if (PNULL == callback_data)
    {
        SCI_TRACE_LOW("MMISMS callback_data pnull");
        return FALSE;
    }
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOWMSG_WIN_ID);
    SCI_TRACE_LOW("mmisms enter ShowSmsWinReadSmsCallback");

#ifdef MMS_SUPPORT    
    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
    {
        MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
    }
#endif    
    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);

    MMISMS_ClearOperInfo();
    
    if(win_data_ptr)
    {        
        GUIRICHTEXT_SaveInfo(richtext_ctrl_id);
        
        if(MMISMS_GetSMSIsExtract())
        {
            GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
        }
        else
        {
            GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        }    
        SCI_TRACE_LOW("MMISMS show sms content");
        win_data_ptr->read_msg = MMISMS_GetReadResult();
        MMISMS_ShowSMSContentRichText(richtext_ctrl_id,FALSE,&win_data_ptr->read_msg);
        GUIRICHTEXT_RestoreInfo(richtext_ctrl_id);
    }
    else
    {
        MMISMS_READ_MSG_T read_msg = {0};
        read_msg = MMISMS_GetReadResult();
        SCI_TRACE_LOW("MMISMS enter show sms win");
        MMISMS_EnterShowSmsWin(read_msg);
    }
    return TRUE;
}


/*****************************************************************************/
//     Description : open read message 
//    Global resource dependence :
//  Author:rong.gu
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleShowSmsOpenReadMessage(MMISMS_SHOWSMS_WINDATA_T *win_data_ptr)
{
    MMISMS_BOX_TYPE_E state_box_type = MMISMS_BOX_NONE;
    MMI_STRING_T   prompt_str = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif
    cur_order_id = win_data_ptr->cur_order_id;
    if(PNULL == cur_order_id)
    {
        return;
    }

    state_box_type = MMISMS_GetSmsBoxtype(cur_order_id); 
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);    
    switch(cur_order_id->flag.msg_type)
    {
        case MMISMS_TYPE_SMS:
#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMsg())
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#endif
            if (MMISMS_IsSmsBusy(cur_order_id))
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            if(MMISMS_BOX_NOSEND == state_box_type)
            {
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS);
            }
#endif
            MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
            MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
            SCI_MEMSET(&win_data_ptr->read_msg,0,sizeof(MMISMS_READ_MSG_T));
            MMISMSAPP_ReadOneSms(
                cur_order_id,
                ShowSmsWinReadSmsCallback,
                PNULL,   //action data
                PNULL);
    
            break;
            
        case MMISMS_TYPE_MMS:
			//解决cr MS00150274,U盘使用过程中不能看MMS
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                break;
            }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
            if (MMIAPIBROWSER_IsRunning())
            {
                MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                break;
            }
#endif
#endif			
#ifdef MET_MEX_SUPPORT
            if (MMIMEX_ConflictPromt())
            {
                // mex kernel is running and may result in mem conflict
                break;
            }     
#endif			
            MMISMS_ClearOperInfo();                     
            switch(state_box_type) 
            {                
            case MMISMS_BOX_MT:
            case MMISMS_BOX_SENDSUCC:
            case MMISMS_BOX_SENDFAIL:
                MMIAPIMMS_ReadMMS((uint32)cur_order_id->record_id);
                break;
            case MMISMS_BOX_NOSEND:
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT);
                MMIAPIMMS_DraftEditMMS((uint32)cur_order_id->record_id);                   
                break;
            default:
                break;
            }

            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
            {
                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
            }
            break;
            
        case MMISMS_TYPE_WAP_PUSH:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {

                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT                        
                MMIAPIOTA_WapPushEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
                if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                {
                    MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                }
            }
            break;
            
        case MMISMS_TYPE_WAP_OTA:
        case MMISMS_TYPE_MMS_OTA:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {               
                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
                MMIAPIOTA_OTAEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
                if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                {
                    MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                }
            }
            break;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
        case MMISMS_TYPE_MAIL:
            //读取邮件
            mail_ReadMailByRecordId((uint16)cur_order_id->record_id);
            break;
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
        default: 
            break;
    }
         
}


/*****************************************************************************/
//     Description : open read message 
//    Global resource dependence :
//  Author:rong.gu
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleOpenCurOrder(MMISMS_ORDER_ID_T cur_order_id)
{
    MMISMS_BOX_TYPE_E state_box_type = MMISMS_BOX_NONE;
    MMI_STRING_T   prompt_str = {0};
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif

    if(PNULL == cur_order_id)
    {
        return;
    }

    state_box_type = MMISMS_GetSmsBoxtype(cur_order_id); 
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);    
    switch(cur_order_id->flag.msg_type)
    {
        case MMISMS_TYPE_SMS:
#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMsg())
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#endif
            if (MMISMS_IsSmsBusy(cur_order_id))
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            if(MMISMS_BOX_NOSEND == state_box_type)
            {
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS);
            }
#endif
            MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
            MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
            //SCI_MEMSET(&win_data_ptr->read_msg,0,sizeof(MMISMS_READ_MSG_T));
            MMISMSAPP_ReadOneSms(
                cur_order_id,
                ShowSmsWinReadSmsCallback,
                PNULL,   //action data
                PNULL);    
            break;
            
        case MMISMS_TYPE_MMS:
			//解决cr MS00150274,U盘使用过程中不能看MMS
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                break;
            }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
            if (MMIAPIBROWSER_IsRunning())
            {
                MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                break;
            }
#endif
#endif			
#ifdef MET_MEX_SUPPORT
            if (MMIMEX_ConflictPromt())
            {
                // mex kernel is running and may result in mem conflict
                break;
            }     
#endif			
            MMISMS_ClearOperInfo();                     
            switch(state_box_type) 
            {                
            case MMISMS_BOX_MT:
            case MMISMS_BOX_SENDSUCC:
            case MMISMS_BOX_SENDFAIL:
                MMIAPIMMS_ReadMMS((uint32)cur_order_id->record_id);
                break;
            case MMISMS_BOX_NOSEND:
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT);
                MMIAPIMMS_DraftEditMMS((uint32)cur_order_id->record_id);                   
                break;
            default:
                break;
            }

            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
            {
                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
            }
            break;
            
        case MMISMS_TYPE_WAP_PUSH:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {

                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT                        
                MMIAPIOTA_WapPushEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
                if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                {
                    MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                }
            }
            break;
            
        case MMISMS_TYPE_WAP_OTA:
        case MMISMS_TYPE_MMS_OTA:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {               
                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
                MMIAPIOTA_OTAEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
                if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                {
                    MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                }
            }
            break;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
        case MMISMS_TYPE_MAIL:
            //读取邮件
            mail_ReadMailByRecordId((uint16)cur_order_id->record_id);
            break;
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
        default: 
            break;
    }
         
}

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Description : handle mt box opt init
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinBtMTBoxOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL; 
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    uint32 common_group_id = 0;
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }  
    
    menu_enable_t.cur_order_id = win_data_ptr->cur_order_id;
    menu_enable_t.win_id = MMISMS_BT_MTBOX_WIN_ID;
    common_group_id = MENU_SMS_BT_MTBOX_OPT;
    
#ifdef MMI_PDA_SUPPORTs
    menu_enable_t.group_id = common_group_id;
#else
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
#endif
#endif
    
    MMISMS_HandleDialMenuEnableGray(&menu_enable_t);
    
    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);
    
    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);  
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleShowSMSWinBtSUCCBoxOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_SHOWSMS_WINDATA_T *win_data_ptr = PNULL;   
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleShowSMSWinExtractEmailOptOk param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_SHOWSMS_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }

    menu_enable_t.cur_order_id = win_data_ptr->cur_order_id;
    menu_enable_t.win_id = MMISMS_BT_SENDSUCCBOX_WIN_ID;
    common_group_id = MENU_SMS_BT_SENDSUCC_OPT;

#ifdef MMI_PDA_SUPPORTs
    menu_enable_t.group_id = common_group_id;
#else
    #if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
    #endif
#endif
    if(menu_enable_t.group_id != MENU_SMS_MTLIST_ANSWER_OPT)
    {
        menu_enable_t.group_id = common_group_id;
    }
    MMISMS_HandleDialMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);
    
}
#endif
