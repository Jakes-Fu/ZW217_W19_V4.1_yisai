/****************************************************************************
** File Name:      mmisms_voicemailwin.c                                    *
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
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmisms_app.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_voicemail.h"
#include "mmisms_voicemailwin.h"
#include "guirichtext.h"
#include "mmisms_menutable.h"
#include "mmipub.h"
#include "mmicc_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//END

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
    MMISMS_PDA_SMS_VOICEMAIL_INIT,
    MMISMS_PDA_SMS_VOICEMAIL_LINE1,    
    MMISMS_PDA_SMS_VOICEMAIL_LINE2,   
    MMISMS_PDA_SMS_VOICEMAIL_FAX,   
    MMISMS_PDA_SMS_VOICEMAIL_DATA,   
    MMISMS_PDA_SMS_VOICEMAIL_MAX   
}MMISMS_PDA_VOICEMAIL_SETTING_TYPE_E;
#endif

extern MMI_SMS_VMN_T g_cur_vmnum_info; //当前voice mail号码信息

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  OpenVMSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  VoiceMailSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
#endif
/*****************************************************************************/
//     Description : to open the window to show voice mailbox number of line1 ,line2
//                 fax and data
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 2006-02-10
/*****************************************************************************/
LOCAL void OpenVMNumberWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type);

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to open the window to show voice mailbox number of line1 ,line2
//                 fax and data
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 2006-02-10
/*****************************************************************************/
/*LOCAL*/PUBLIC void OpenVMListWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type);//20150618.vm
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaSmsVoiceMailListCtrl(MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaSettingsItem2Line_2Anim(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T imgae_id1,   //图标ID
                                 MMI_IMAGE_ID_T imgae_id2,   //图标ID
                                 uint32         user_data   //用户数据
                                 );

/*****************************************************************************/
// Description : open set vm win
// Global resource dependence : 
// Author:rong.gu
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetVMWinByType(MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr,MMISMS_VMN_RECORD_ID_E vmn_type);

#else
 /*****************************************************************************/
//     Description : to handle the showing voice mail number window
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 2006-02-10
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowVMWinMsg(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );

/*****************************************************************************/
//     Description : to open the window to set the voice mail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMISMS_OpenSetVMWin(void);
#endif

/*****************************************************************************/
//     Description : set voice mail detail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void SetVMDetail(MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T ctrl_id);

#if 0 
// no use
/*****************************************************************************/
//     Description : 对语音信箱号码的操作菜单的响应
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 2006-02-10
/*****************************************************************************/
LOCAL void VoiceMailOperatorMenuCnf(
                                 MMI_CTRL_ID_T    ctrl_id    
                                  );
#endif

/*****************************************************************************/
//     Description : 语音信箱号码界面打开时，等待界面不允许返回
//    Global resource dependence : 
//  Author: kelly.li
//    Note:2006-02-13
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGetVMWaitiingWindow(
                                           MMI_WIN_ID_T         win_id,     // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM                 param        // 相应消息的参数
                                           );

/*****************************************************************************/
//     Description : to handle the edit voice mail window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSVMEditWindow(
                                       MMI_WIN_ID_T        win_id,    //IN:
                                       MMI_MESSAGE_ID_E msg_id,    //IN:
                                       DPARAM            param    //IN:
                                       );
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : open voice mail win
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterShowVmnWins(void);
#endif
/*****************************************************************************/
//     Description : to handle the voicemail setting list
//    Global resource dependence : 
//  Author:rong gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVoiceMailSettingMenuWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM                param
                                                    );

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
// the window for voice mail setting menu

#ifdef MMI_GUI_STYLE_TYPICAL
//the window for showing voice mail number
WINDOW_TABLE( MMISMS_SHOW_VMN_WIN_TAB ) = 
{
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32) HandleShowVMWinMsg ),    
    WIN_ID( MMISMS_SHOW_VMN_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_RICHTEXT_CTRL(MMISMS_VMN_LABEL_CTRL_ID),
    END_WIN
};
#endif/* MMI_PDA_SUPPORT */

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISMS_VOICEMAIL_SETTING_MENUWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleVoiceMailSettingMenuWinMsg),    
    WIN_ID( MMISMS_VOICEMAIL_SETTING_MENU_WIN_ID ),
    WIN_TITLE( TXT_VOICEMAIL ),
#ifndef MMI_PDA_SUPPORT    
    WIN_SOFTKEY(TXT_EDIT, TXT_CALL, STXT_RETURN ),
#endif    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISMS_VM_EDIT_WIN_TAB ) = 
{
    WIN_TITLE( TXT_NULL ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32) HandleSMSVMEditWindow ),    
    WIN_ID( MMISMS_VM_EDIT_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL ),
    CREATE_EDIT_PHONENUM_CTRL(MMISMS_VMNUM_MAX_LEN, MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID),
    END_WIN  
};
#else
WINDOW_TABLE( MMISMS_VOICEMAIL_SETTING_MENUWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleVoiceMailSettingMenuWinMsg),    
    WIN_ID( MMISMS_VOICEMAIL_SETTING_MENU_WIN_ID ),
    WIN_TITLE( TXT_VOICEMAIL ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_MENU_CTRL(MENU_SMS_VOICEMAIL_OPT, MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISMS_VM_EDIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)  HandleSMSVMEditWindow),    
    WIN_TITLE(TXT_NULL),
    WIN_ID( MMISMS_VM_EDIT_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_VM_EDIT_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_VM_FORM_CTRL_ID,MMISMS_VM_EDIT_CTRL_ID),
        //号码title label
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_VM_EDIT_WIN_CTRL_ID,MMISMS_VM_FORM_CTRL_ID),
    
        //号码label
        CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISMS_VMNUM_MAX_LEN,MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID,MMISMS_VM_FORM_CTRL_ID), 
    END_WIN                                
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : open voice mail win
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterShowVmnWin(void)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr = PNULL;

    if(MMK_IsOpenWin(MMISMS_SHOW_VMN_WIN_ID))
    {
        SCI_TRACE_LOW("mmisms main menu is open");
        MMK_CloseWin(MMISMS_SHOW_VMN_WIN_ID);
    }
    
    win_data_ptr = (MMISMS_VOICEMAIL_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_VOICEMAIL_WINDATA_T));

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms malloc error");
        return;
    }
    win_data_ptr->cur_vmnum_info = MMISMS_GetCurVmInfo();
    win_handle  = MMK_CreateWin((uint32*)MMISMS_SHOW_VMN_WIN_TAB, PNULL);

    if(win_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms MMISMS_OpenVoiceMailWin create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_OpenVoiceMailWin create success");
    MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);
    
}
#endif

/*****************************************************************************/
//  Description : open voice mail win
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenVoiceMailWin(void)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr = PNULL;

    if(MMK_IsOpenWin(MMISMS_VOICEMAIL_SETTING_MENU_WIN_ID))
    {
        SCI_TRACE_LOW("mmisms main menu is open");
        return;
    }
    
    win_data_ptr = (MMISMS_VOICEMAIL_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_VOICEMAIL_WINDATA_T));

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms malloc error");
        return;
    }
    win_data_ptr->cur_vmnum_info = MMISMS_GetCurVmInfo();
    win_handle  = MMK_CreateWin((uint32*)MMISMS_VOICEMAIL_SETTING_MENUWIN_TAB, PNULL);

    if(win_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms MMISMS_OpenVoiceMailWin create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_OpenVoiceMailWin create success");
    MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);
    
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaSettingsItem2Line_2Anim(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T imgae_id1,   //图标ID
                                 MMI_IMAGE_ID_T imgae_id2,   //图标ID
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16 index = 0;
    //minghu for cr123283    
    if (IMAGE_NULL == imgae_id1)
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_ANIM_AND_TEXT;
        index = 2;
    }
    else       
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT;
        index = 3;
    }
    
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;    
    item_t.item_state |=GUIITEM_STATE_CONTENT_CHECK;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    if (IMAGE_NULL == imgae_id1)
    {
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = imgae_id2;
    }
    else
    {
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = imgae_id1;
        
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[2].item_data.image_id = imgae_id2;
    }   

    item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = text_str.wstr_ptr;
    item_data.item_content[index].item_data.text_buffer.wstr_len = text_str.wstr_len;

    GUILIST_AppendItem( ctrl_id, &item_t );
  
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaSmsVoiceMailListCtrl(MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr)
{
    MMI_CTRL_ID_T       ctrl_id = MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID;
#if defined(MMI_PDA_SUPPORT) || defined(TOUCH_PANEL_SUPPORT)     
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
#endif
    wchar temp_wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMI_STRING_T text_str= {0};
    uint16 mail_len = 0;
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
#ifdef MMI_CSP_SUPPORT
    BOOLEAN is_als_support = MMISET_IsAlsValid(MMISMS_GetCurVoiceMailDualSys());
#endif
    //set max item
    
    GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_SMS_VOICEMAIL_MAX, FALSE );   
    
    mail_len = MMIAPICOM_GenDispNumber( 
        win_data_ptr->cur_vmnum_info.line1_vmn_info.number_type, 
        win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len, 
        win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num, 
        voice_mail,
        MMISMS_PBNUM_MAX_LEN + 2);
    SCI_TRACE_BUF("zhangqiang test voice_mail",voice_mail,strlen((char *)voice_mail));
    if(mail_len != 0)
    {
    MMI_STRNTOWSTR(temp_wstr,
                            MMISMS_PBNUM_MAX_LEN + 2,
                            (const uint8 *)voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2,
                            mail_len); 
    SCI_TRACE_BUF("zhangqign test temp_wstr",temp_wstr,strlen((char *)voice_mail)*sizeof(wchar));
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = mail_len;
#ifdef TOUCH_PANEL_SUPPORT
    image_id = IMAGE_IDLE_SHORTCUT_DIAL;
#endif
    }
    else
    {
#ifdef MMI_PDA_SUPPORT     
        image_id = IMAGE_NULL;
#endif
    }
#ifdef MMI_CSP_SUPPORT
    if(is_als_support)
    {
        text_id = TXT_LINE1_NUMBER;
    }
    else
#endif
    {
        text_id = TXT_LINE_NUMBER;
    }
    AppendPdaSettingsItem2Line_2Anim(
    ctrl_id,
    TXT_LINE1_NUMBER,
    text_str,
#ifdef MMI_PDA_SUPPORT    
    image_id,
    IMAGE_IDLE_SHORTCUT_WRITE_MSG,
#else
    IMAGE_NULL,
    IMAGE_NULL,
#endif
    (uint32)MMISMS_PDA_SMS_VOICEMAIL_LINE1
    );
#ifdef MMI_CSP_SUPPORT
    if(is_als_support)
    {
        SCI_MEMSET(&text_str, 0 ,sizeof(MMI_STRING_T));
        SCI_MEMSET(voice_mail, 0 ,sizeof(voice_mail));
        mail_len = MMIAPICOM_GenDispNumber( 
            win_data_ptr->cur_vmnum_info.line2_vmn_info.number_type, 
            win_data_ptr->cur_vmnum_info.line2_vmn_info.num_len, 
            win_data_ptr->cur_vmnum_info.line2_vmn_info.bcd_num, 
            voice_mail,
            MMISMS_PBNUM_MAX_LEN + 2);
        if(mail_len != 0)
        {
        MMI_STRNTOWSTR(temp_wstr,
                                MMISMS_PBNUM_MAX_LEN + 2,
                                (const uint8 *)voice_mail,
                                MMISMS_PBNUM_MAX_LEN + 2,
                                mail_len);                                
        text_str.wstr_ptr = temp_wstr;
        text_str.wstr_len = mail_len; 
#if defined(TOUCH_PANEL_SUPPORT)
        image_id = IMAGE_IDLE_SHORTCUT_DIAL;
#endif
        }
        else
        {
#ifdef MMI_PDA_SUPPORT     
             image_id = IMAGE_NULL;
#endif
        }
        AppendPdaSettingsItem2Line_2Anim(
        ctrl_id,
        TXT_LINE2_NUMBER,
        text_str,
#ifdef MMI_PDA_SUPPORT
        image_id,
        IMAGE_IDLE_SHORTCUT_WRITE_MSG,
#else
        IMAGE_NULL,
        IMAGE_NULL,
#endif
        (uint32)MMISMS_PDA_SMS_VOICEMAIL_LINE2
        );
    }
#endif//MMI_CSP_SUPPORT

    SCI_MEMSET(&text_str, 0 ,sizeof(MMI_STRING_T));
    SCI_MEMSET(voice_mail, 0 ,sizeof(voice_mail));
    mail_len = MMIAPICOM_GenDispNumber( 
    win_data_ptr->cur_vmnum_info.fax_vmn_info.number_type, 
    win_data_ptr->cur_vmnum_info.fax_vmn_info.num_len, 
    win_data_ptr->cur_vmnum_info.fax_vmn_info.bcd_num, 
    voice_mail,
    MMISMS_PBNUM_MAX_LEN + 2);
    if(mail_len != 0)
    {
    MMI_STRNTOWSTR(temp_wstr,
                            MMISMS_PBNUM_MAX_LEN + 2,
                            (const uint8 *)voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2,
                            mail_len);                                
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = mail_len;
#ifdef TOUCH_PANEL_SUPPORT
    image_id = IMAGE_IDLE_SHORTCUT_DIAL;
#endif
    }
    else
    {
#ifdef MMI_PDA_SUPPORT     
        image_id = IMAGE_NULL;
#endif
    }
    AppendPdaSettingsItem2Line_2Anim(
    ctrl_id,
    TXT_COMM_FAX_NUMBER,
    text_str,
#ifdef MMI_PDA_SUPPORT    
    image_id,
    IMAGE_IDLE_SHORTCUT_WRITE_MSG,
#else
    IMAGE_NULL,
    IMAGE_NULL,
#endif
    (uint32)MMISMS_PDA_SMS_VOICEMAIL_FAX
    );
#ifdef MMI_CSP_SUPPORT
    if(is_als_support)
    {
        if(!MMISET_IsCphsValid(g_mmisms_global.cur_voice_mail_dual_sys))
        {
            GUILIST_SetItemGray(ctrl_id, 2, TRUE);
        }
    }
    else
    {
        if(!MMISET_IsCphsValid(g_mmisms_global.cur_voice_mail_dual_sys))
        {
            GUILIST_SetItemGray(ctrl_id, 1, TRUE);
        }
    }
#endif
    SCI_MEMSET(&text_str, 0 ,sizeof(MMI_STRING_T));
    SCI_MEMSET(voice_mail, 0 ,sizeof(voice_mail));
    mail_len = MMIAPICOM_GenDispNumber( 
        win_data_ptr->cur_vmnum_info.data_vmn_info.number_type, 
        win_data_ptr->cur_vmnum_info.data_vmn_info.num_len, 
        win_data_ptr->cur_vmnum_info.data_vmn_info.bcd_num, 
        voice_mail,
        MMISMS_PBNUM_MAX_LEN + 2);         
    if(mail_len != 0)    
    {
    MMI_STRNTOWSTR(temp_wstr,
                            MMISMS_PBNUM_MAX_LEN + 2,
                            (const uint8 *)voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2,
                            mail_len);                                
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = mail_len;  
#ifdef TOUCH_PANEL_SUPPORT
    image_id = IMAGE_IDLE_SHORTCUT_DIAL;
#endif
    }
    else
    {
#ifdef MMI_PDA_SUPPORT     
        image_id = IMAGE_NULL;
#endif
    }
    AppendPdaSettingsItem2Line_2Anim(
    ctrl_id,
    TXT_SMS_DATA_NUMBER,
    text_str,
#ifdef MMI_PDA_SUPPORT 
    image_id,
    IMAGE_IDLE_SHORTCUT_WRITE_MSG,
#else
    IMAGE_NULL,
    IMAGE_NULL,
#endif
    (uint32)MMISMS_PDA_SMS_VOICEMAIL_DATA
    );
#ifdef MMI_CSP_SUPPORT
    if(is_als_support)
    {
        if(!MMISET_IsCphsValid(g_mmisms_global.cur_voice_mail_dual_sys))
        {
            GUILIST_SetItemGray(ctrl_id, 3, TRUE);
        }
    }
    else
    {
        if(!MMISET_IsCphsValid(g_mmisms_global.cur_voice_mail_dual_sys))
        {
            GUILIST_SetItemGray(ctrl_id, 2, TRUE);
        }
    }
#endif
}
#endif
/*****************************************************************************/
//     Description : to handle the voicemail setting list
//    Global resource dependence : 
//  Author:rong gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVoiceMailSettingMenuWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM                param
                                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID;
    uint16      cur_index    = 0;
    MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr = (MMISMS_VOICEMAIL_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms voice mail win_data_ptr param PNULL");
        return MMI_RESULT_TRUE;    
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaSmsVoiceMailListCtrl(win_data_ptr);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

 case MSG_NOTIFY_LIST_CHECK_CONTENT:
     {
         
         GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
         GUILIST_ITEM_T*  list_item_ptr      = PNULL;
         
         item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
         cur_index = item_content_ptr->item_index;
         list_item_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
         if(PNULL == list_item_ptr)
         {
             //SCI_TRACE_LOW:"HandleEntryDetailPdaWinMsg list_item_ptr %d"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_31121_112_2_18_2_55_24_872,(uint8*)"d", list_item_ptr);
             break;
         }
         if(item_content_ptr->item_content_index == 1)
         {
             MMISMS_VMN_RECORD_ID_E vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
#ifdef TOUCH_PANEL_SUPPORT            
             if(list_item_ptr->item_data_ptr->item_content[1].item_data.image_id == IMAGE_IDLE_SHORTCUT_DIAL)
             {
#ifdef MMI_CSP_SUPPORT
                 if(MMISET_IsAlsValid(MMISMS_GetCurVoiceMailDualSys()))
                 {
                     vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID;
                 }
                 else
#endif 
                 {
                     if(0 ==cur_index)//对line1的单独处理
                     {
                         vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID;
                     }
                     else
                     {
                         vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID + 1;//在csp不存在时，跳过MMISMS_LINE2_VMN_RECORD_ID = 2
                     }
                 }
                 MMISMS_OpenCallVMWin(MMISMS_GetCurVoiceMailDualSys(),vmn_type);
                 //MMISMS_OpenCallVMWin(MMISMS_GetCurVoiceMailDualSys(),(cur_index + MMISMS_LINE1_VMN_RECORD_ID));
             }
             else if(list_item_ptr->item_data_ptr->item_content[1].item_data.image_id == IMAGE_IDLE_SHORTCUT_WRITE_MSG)
             {
#ifdef MMI_CSP_SUPPORT
                 if(MMISET_IsAlsValid(MMISMS_GetCurVoiceMailDualSys()))
                 {
                     vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID;
                 }
                 else
#endif 
                 {
                     if (0 ==cur_index) //对line1的单独处理
                     {
                         vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID;
                     }
                     else
                     {
                         vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID + 1;//在csp不存在时，跳过MMISMS_LINE2_VMN_RECORD_ID = 2
                     }
                 }
                 MMISMS_OpenSetVMWinByType(win_data_ptr, vmn_type);
                 //MMISMS_OpenSetVMWinByType(win_data_ptr,cur_index + MMISMS_LINE1_VMN_RECORD_ID);
             }
#endif                
         }
         else if(item_content_ptr->item_content_index == 2)
         {
             MMISMS_VMN_RECORD_ID_E vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
             if(0 != cur_index)
             {
#ifdef MMI_CSP_SUPPORT
                 if(MMISET_IsAlsValid(MMISMS_GetCurVoiceMailDualSys()))
                 {
                     vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID;
                 }
                 else
#endif 
                 {
                     vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID + 1;//在csp不存在时，跳过MMISMS_LINE2_VMN_RECORD_ID = 2
                 }
             }
             MMISMS_OpenSetVMWinByType(win_data_ptr, vmn_type);
             // MMISMS_OpenSetVMWinByType(win_data_ptr,cur_index + MMISMS_LINE1_VMN_RECORD_ID);
         }
         else
         {
             
         }
         
     }
        break;

    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
#ifdef MMI_GUI_STYLE_MINISCREEN        
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
#endif
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaSmsVoiceMailListCtrl(win_data_ptr);
#ifdef MMI_GUI_STYLE_MINISCREEN        
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
#endif
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        {
            MMISMS_VMN_RECORD_ID_E vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
            if(0 != cur_index)
            {
#ifdef MMI_CSP_SUPPORT
                if(MMISET_IsAlsValid(MMISMS_GetCurVoiceMailDualSys()))
                {
                    vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID;
                }
                else
#endif 
                {
                    vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID + 1;//??line2?,??line2??????1
                }
            }
            MMISMS_OpenSetVMWinByType(win_data_ptr, vmn_type);
        }
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        {
            MMISMS_VMN_RECORD_ID_E vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
            if(0 != cur_index)
            {
#ifdef MMI_CSP_SUPPORT
                if(MMISET_IsAlsValid(MMISMS_GetCurVoiceMailDualSys()))
                {
                    vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID;
                }
                else
#endif 
                {
                    vmn_type = cur_index + MMISMS_LINE1_VMN_RECORD_ID + 1;//??line2?,??line2??????1
                }
            }
            MMISMS_OpenCallVMWin(MMISMS_GetCurVoiceMailDualSys(),vmn_type);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        SCI_FREE(win_data_ptr);
        break;     
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  VoiceMailSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_FALSE;

    if (isReturnOK)
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        {
            uint16 i = 0;

            MMISMS_InitCurVmInfo();
            MMISMS_SetVmOperType(MMISMS_VMN_OPER_TYPE_SMS_VM_SETTING);
            MMISMS_SetCurVoiceMailDualSys((MN_DUAL_SYS_E)dual_sys);

            //for(i = 0; i < (MMISMS_VMN_TYPE_MAX - 1); i++)//20150618.vm
            {
                OpenVMListWin((MN_DUAL_SYS_E)dual_sys,(MMISMS_LINE1_VMN_RECORD_ID + i));
            }
        }
#else
        MMISMS_SetCurVoiceMailDualSys((MN_DUAL_SYS_E)dual_sys);

        MMISMS_OpenVoiceMailWin();           
#endif            
    }

    return recode;
}

/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  OpenVMSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 vmn_type = (uint32)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());


    if (isReturnOK)
    {
        dual_sys = (MN_DUAL_SYS_E)dual_sys;
            OpenVMNumberWin(dual_sys, (MMISMS_VMN_RECORD_ID_E)vmn_type);
        }

    return recode;
}

#endif

/*****************************************************************************/
//     Description : to open the voice mail main window
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenVoicemailWin(void)
{
    if (MMIAPIPHONE_GetSimAvailableNum(NULL, 0) == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    {
        uint16 sim_sys = MN_DUAL_SYS_1;
        uint32 sim_num = 0;

        sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_num)
        {
            MMISMS_SetCurVoiceMailDualSys((MN_DUAL_SYS_E)sim_sys);
            #ifndef MMI_GUI_STYLE_TYPICAL             
            {
                uint16 i = 0;
                MMISMS_InitCurVmInfo();
                MMISMS_SetVmOperType(MMISMS_VMN_OPER_TYPE_SMS_VM_SETTING);
                //for(i = 0; i < (MMISMS_VMN_TYPE_MAX - 1); i++)//20150618.vm
                {
                    OpenVMListWin((MN_DUAL_SYS_E)sim_sys,(MMISMS_LINE1_VMN_RECORD_ID + i));
                }
            }
            //to do
#else
            MMISMS_OpenVoiceMailWin();
#endif
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(VoiceMailSelectSimCallback,PNULL,TRUE);
        }
#endif
    }
    
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// Description : open set vm win
// Global resource dependence : 
// Author:rong.gu
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetVMWinByType(MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr,MMISMS_VMN_RECORD_ID_E vmn_type)
{
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 mail_len = 0;
    MMI_STRING_T   num_string = {0};
    wchar voice_mail_wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    
    switch( vmn_type ) 
    {
    case MMISMS_LINE1_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.line1_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
        
    case MMISMS_LINE2_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.line2_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.line2_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.line2_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
        
    case MMISMS_FAX_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.fax_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.fax_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.fax_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;

    case MMISMS_DATA_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.data_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.data_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.data_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
        
    default:
        //SCI_TRACE_LOW:" the menu id is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15277_112_2_18_2_54_52_775,(uint8*)"");
        break;
    }
    
    // open the input voice mail number window
    MMK_CreatePubEditWin((uint32 *)MMISMS_VM_EDIT_WIN_TAB, (ADD_DATA)vmn_type);
    
    if (0 < mail_len)
    {
        mail_len = MIN(MMISMS_PBNUM_MAX_LEN, mail_len);

        MMI_STRNTOWSTR(voice_mail_wstr,
                                    MMISMS_PBNUM_MAX_LEN,
                                    (const uint8 *)voice_mail,
                                    MMISMS_PBNUM_MAX_LEN,
                                    mail_len);

        // set the edit box init param
        num_string.wstr_len = mail_len;
        num_string.wstr_ptr = voice_mail_wstr;
        SCI_TRACE_BUF("zhangqiang test MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID's num_string",num_string.wstr_ptr,num_string.wstr_len*sizeof(wchar));
        GUIEDIT_SetString(MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID, num_string.wstr_ptr, num_string.wstr_len);
    }
}


#endif

/*****************************************************************************/
//     Description : set voice mail detail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void SetVMDetail(MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    uint16 index = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T title_str = {0};
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 mail_len = 0;
    wchar voice_mail_wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};

    GUIMENU_GetId(MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
    switch ( menu_id )
    {
    case MMISMS_MENU_LINE_VMN_ITEM_ID:
        text_id = TXT_LINE_NUMBER;
        mail_len = MMIAPICOM_GenDispNumber( 
                        g_cur_vmnum_info.line1_vmn_info.number_type,
                        (uint8)g_cur_vmnum_info.line1_vmn_info.num_len, 
                        g_cur_vmnum_info.line1_vmn_info.bcd_num,
                        voice_mail,
                        MMISMS_PBNUM_MAX_LEN + 2
                        );
        break;
 
#ifdef MMI_CSP_SUPPORT
    case MMISMS_MENU_LINE1_VMN_ITEM_ID:
        text_id = TXT_LINE1_NUMBER;
        mail_len = MMIAPICOM_GenDispNumber( 
                        win_data_ptr->cur_vmnum_info.line1_vmn_info.number_type,
                        (uint8)win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len, 
                        win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num,
                        voice_mail,
                        MMISMS_PBNUM_MAX_LEN + 2
                        );
        break;
        
    case MMISMS_MENU_LINE2_VMN_ITEM_ID:
        text_id = TXT_LINE2_NUMBER;
        mail_len = MMIAPICOM_GenDispNumber( 
                        win_data_ptr->cur_vmnum_info.line2_vmn_info.number_type,
                        (uint8)win_data_ptr->cur_vmnum_info.line2_vmn_info.num_len, 
                        win_data_ptr->cur_vmnum_info.line2_vmn_info.bcd_num,
                        voice_mail,
                        MMISMS_PBNUM_MAX_LEN + 2
                        );
        break;
#endif     
        
    case MMISMS_MENU_FAX_VMN_ITEM_ID:
        text_id = TXT_COMM_FAX_NUMBER;
        mail_len = MMIAPICOM_GenDispNumber( 
                        win_data_ptr->cur_vmnum_info.fax_vmn_info.number_type,
                        (uint8)win_data_ptr->cur_vmnum_info.fax_vmn_info.num_len, 
                        win_data_ptr->cur_vmnum_info.fax_vmn_info.bcd_num,
                        voice_mail,
                        MMISMS_PBNUM_MAX_LEN + 2
                        );
        break;
        
    case MMISMS_MENU_DATA_VMN_ITEM_ID:
        text_id = TXT_SMS_DATA_NUMBER;
        mail_len = MMIAPICOM_GenDispNumber( 
                        win_data_ptr->cur_vmnum_info.data_vmn_info.number_type,
                        (uint8)win_data_ptr->cur_vmnum_info.data_vmn_info.num_len, 
                        win_data_ptr->cur_vmnum_info.data_vmn_info.bcd_num,
                        voice_mail,
                        MMISMS_PBNUM_MAX_LEN + 2
                        );
        break;

    default:
        //SCI_TRACE_LOW:" the menu id is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15447_112_2_18_2_54_53_777,(uint8*)"");
        break;
    }

    GUIRICHTEXT_DeleteAllItems(ctrl_id);

    mail_len = MIN(mail_len, MMISMS_PBNUM_MAX_LEN);

    MMI_STRNTOWSTR(voice_mail_wstr,
                                MMISMS_PBNUM_MAX_LEN,
                                (const uint8 *)voice_mail,
                                MMISMS_PBNUM_MAX_LEN,
                                mail_len);

    title_str.wstr_len = MMIAPICOM_Wstrlen(voice_mail_wstr);
    title_str.wstr_ptr = voice_mail_wstr;

    item_data.img_type = GUIRICHTEXT_IMAGE_RES;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    item_data.img_data.res.id = IMAGE_STORAGE_SIM_ICON;

    if ( 0 == title_str.wstr_len)
    {
        MMI_GetLabelTextByLang(TXT_SMS_VOICEMAIL_EMPTY, &title_str);

        GUIWIN_SetSoftkeyTextId(MMISMS_SHOW_VMN_WIN_ID,  TXT_EDIT, TXT_NULL, STXT_RETURN, FALSE);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(MMISMS_SHOW_VMN_WIN_ID,  TXT_EDIT, TXT_CALL, STXT_RETURN, FALSE);
    }

    item_data.text_data.buf.str_ptr = title_str.wstr_ptr;
    item_data.text_data.buf.len = title_str.wstr_len;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    GUIWIN_SetTitleTextId(MMISMS_SHOW_VMN_WIN_ID, text_id, FALSE);
}

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to open the window to set the voice mail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMISMS_OpenSetVMWin(void)
{
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 mail_len = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_STRING_T   num_string = {0};
    wchar voice_mail_wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    
    GUIMENU_GetId( MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
    switch( menu_id ) 
    {
    case MMISMS_MENU_LINE_VMN_ITEM_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.line1_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.line1_vmn_info.num_len, 
                            g_cur_vmnum_info.line1_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;

#ifdef MMI_CSP_SUPPORT
    case MMISMS_MENU_LINE1_VMN_ITEM_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info->line1_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info->line1_vmn_info.num_len, 
                            g_cur_vmnum_info->line1_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
        
    case MMISMS_MENU_LINE2_VMN_ITEM_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info->line2_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info->line2_vmn_info.num_len, 
                            g_cur_vmnum_info->line2_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
#endif
        
    case MMISMS_MENU_FAX_VMN_ITEM_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info->fax_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info->fax_vmn_info.num_len, 
                            g_cur_vmnum_info->fax_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;

    case MMISMS_MENU_DATA_VMN_ITEM_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info->data_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info->data_vmn_info.num_len, 
                            g_cur_vmnum_info->data_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
        
    default:
        //SCI_TRACE_LOW:" the menu id is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15359_112_2_18_2_54_52_776,(uint8*)"");
        break;
    }
    
    // open the input voice mail number window
    MMK_CreateWin((uint32 *)MMISMS_VM_EDIT_WIN_TAB, PNULL);
    
    if (0 < mail_len)
    {
        mail_len = MIN(MMISMS_PBNUM_MAX_LEN, mail_len);

        MMI_STRNTOWSTR(voice_mail_wstr,
                                    MMISMS_PBNUM_MAX_LEN,
                                    (const uint8 *)voice_mail,
                                    MMISMS_PBNUM_MAX_LEN,
                                    mail_len);

        // set the edit box init param
        num_string.wstr_len = mail_len;
        num_string.wstr_ptr = voice_mail_wstr;
        GUIEDIT_SetString(MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID, num_string.wstr_ptr, num_string.wstr_len);
    }
}

/*****************************************************************************/
//     Description : to handle the showing voice mail number window
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 2006-02-10
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowVMWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 mail_len = 0;
    MMISMS_VMN_RECORD_ID_E vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
    MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr = (MMISMS_VOICEMAIL_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms voice mail win_data_ptr param PNULL");
        return MMI_RESULT_TRUE;    
    }
    
    switch(msg_id)
    {        
    case MSG_OPEN_WINDOW:
        SetVMDetail(win_data_ptr,MMISMS_VMN_LABEL_CTRL_ID);

        MMK_SetAtvCtrl(win_id, MMISMS_VMN_LABEL_CTRL_ID);
        break;

    case MSG_APP_OK:    // 对语音号码进行操作
    case MSG_CTL_OK:
        MMISMS_OpenSetVMWin();
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
        case MMISMS_MENU_LINE_VMN_ITEM_ID:
            mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.line1_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.line1_vmn_info.num_len, 
                            g_cur_vmnum_info.line1_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
            break;

#ifdef MMI_CSP_SUPPORT
        case MMISMS_MENU_LINE1_VMN_ITEM_ID:
            mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.line1_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
            break;
            
        case MMISMS_MENU_LINE2_VMN_ITEM_ID:
            mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.line2_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.line2_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.line2_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
            break;
#endif //MMI_CSP_SUPPORT            
            
        case MMISMS_MENU_FAX_VMN_ITEM_ID:
            mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.fax_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.fax_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.fax_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
            break;
            
        case MMISMS_MENU_DATA_VMN_ITEM_ID:
            mail_len = MMIAPICOM_GenDispNumber( 
                            win_data_ptr->cur_vmnum_info.data_vmn_info.number_type,
                            (uint8)win_data_ptr->cur_vmnum_info.data_vmn_info.num_len, 
                            win_data_ptr->cur_vmnum_info.data_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
            break;

        default:
            //SCI_TRACE_LOW:" the menu id is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15566_112_2_18_2_54_53_778,(uint8*)"");
            break;
        }

        if(mail_len == 0)
        {
            MMISMS_OpenSetVMWin();
        }
        else
        {
            switch( menu_id ) 
            {
                case MMISMS_MENU_LINE_VMN_ITEM_ID:
                    vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
                    break;

#ifdef MMI_CSP_SUPPORT
                case MMISMS_MENU_LINE1_VMN_ITEM_ID:
                    vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
                    break;
                    
                case MMISMS_MENU_LINE2_VMN_ITEM_ID:
                    vmn_type = MMISMS_LINE2_VMN_RECORD_ID;
                    break;
#endif //MMI_CSP_SUPPORT
                    
                case MMISMS_MENU_FAX_VMN_ITEM_ID:
                    vmn_type = MMISMS_FAX_VMN_RECORD_ID;
                    break;

                case MMISMS_MENU_DATA_VMN_ITEM_ID:
                    vmn_type = MMISMS_DATA_VMN_RECORD_ID;
                    break;
                
                default:
                    //SCI_TRACE_LOW:" the menu id is error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15594_112_2_18_2_54_53_779,(uint8*)"");
                    break;
            }
            MMISMS_OpenCallVMWin(MMISMS_GetCurVoiceMailDualSys(), vmn_type);
        }

        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        IMG_EnableTransparentColor(FALSE);
        break;
        
    case MSG_CLOSE_WINDOW:
        if(win_data_ptr)
        {
            SCI_FREE(win_data_ptr);
        }    
        break;
        
    default:            
        recode = MMI_RESULT_FALSE;
        break;
    }

    return (recode);
}
#endif /* MMI_PDA_SUPPORT */
 
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : save voice mail addr to simcard
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 
/*****************************************************************************/
PUBLIC void SaveVMAddr2SimByType(MMI_SMS_VMN_T *cur_vmnum_info,MN_DUAL_SYS_E dual_sys,MMI_STRING_T* num_string, MMISMS_VMN_RECORD_ID_E vmn_type)
{
    MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MN_SMS_VMW_MBNUM_T voimess_addr_info = {0};
    uint8 record_id = 0;
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type = MN_MULTI_SUBSCRIBER_MAX;
    MN_VOICE_MAIL_TYPE_E voice_mail_type = MN_VOICE_MAIL;
	
    MMI_WSTRNTOSTR((uint8 *)voice_mail,
                                MMISMS_PBNUM_MAX_LEN + 2,
                                num_string->wstr_ptr,
                                num_string->wstr_len,
                                MIN(MMISMS_PBNUM_MAX_LEN, num_string->wstr_len));
 
    MMIAPICOM_GenPartyNumber(voice_mail, (int16)MIN(MMISMS_PBNUM_MAX_LEN, num_string->wstr_len), &party_num );

    //set the parameter 
    voimess_addr_info.alpha_id_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN;
    voimess_addr_info.mailbox_number_len = party_num.num_len;
    voimess_addr_info.is_ton_npi_exist = TRUE;
    voimess_addr_info.is_ccp_exist = FALSE;
    voimess_addr_info.ccp_id = 0;
    voimess_addr_info.is_ext_exist = FALSE;
    voimess_addr_info.ton = party_num.number_type;
    voimess_addr_info.npi = MN_NUM_PLAN_ISDN_TELE;
    voimess_addr_info.ext_id = 0;
    SCI_MEMSET( voimess_addr_info.mailbox_number, 0, sizeof(voimess_addr_info.mailbox_number));
    SCI_MEMCPY( voimess_addr_info.mailbox_number, party_num.bcd_num, voimess_addr_info.mailbox_number_len);

    switch ( vmn_type )
    {
    case MMISMS_LINE1_VMN_RECORD_ID:
        record_id = 1;
        cur_vmnum_info->record_id = record_id;
        cur_vmnum_info->line1_vmn_info.num_len = party_num.num_len;
        cur_vmnum_info->line1_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( cur_vmnum_info->line1_vmn_info.bcd_num, 0, sizeof( cur_vmnum_info->line1_vmn_info.bcd_num ));
        SCI_MEMCPY( cur_vmnum_info->line1_vmn_info.bcd_num, party_num.bcd_num,cur_vmnum_info->line1_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_1;
        voice_mail_type = MN_VOICE_MAIL; 

		break;

    case MMISMS_LINE2_VMN_RECORD_ID:
        record_id = 2;
        cur_vmnum_info->record_id = record_id;
        cur_vmnum_info->line2_vmn_info.num_len = party_num.num_len;
        cur_vmnum_info->line2_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( cur_vmnum_info->line2_vmn_info.bcd_num, 0, sizeof( cur_vmnum_info->line2_vmn_info.bcd_num ));
        SCI_MEMCPY( cur_vmnum_info->line2_vmn_info.bcd_num, party_num.bcd_num,cur_vmnum_info->line2_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_2;
        voice_mail_type = MN_VOICE_MAIL;         
		break;

    case MMISMS_FAX_VMN_RECORD_ID:
        record_id = 3;
        cur_vmnum_info->record_id = record_id;
        cur_vmnum_info->fax_vmn_info.num_len = party_num.num_len;
        cur_vmnum_info->fax_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( cur_vmnum_info->fax_vmn_info.bcd_num, 0, sizeof( cur_vmnum_info->fax_vmn_info.bcd_num ));
        SCI_MEMCPY( cur_vmnum_info->fax_vmn_info.bcd_num, party_num.bcd_num,cur_vmnum_info->fax_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_1;
        voice_mail_type = MN_VOICE_MAIL_FAX; 
		break;

    case MMISMS_DATA_VMN_RECORD_ID:
        record_id = 4;
        cur_vmnum_info->record_id = record_id;
        cur_vmnum_info->data_vmn_info.num_len = party_num.num_len;
        cur_vmnum_info->data_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( cur_vmnum_info->data_vmn_info.bcd_num, 0, sizeof( cur_vmnum_info->data_vmn_info.bcd_num ));
        SCI_MEMCPY( cur_vmnum_info->data_vmn_info.bcd_num, party_num.bcd_num,cur_vmnum_info->data_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_1;
        voice_mail_type = MN_VOICE_MAIL_EMAIL; 
		break;

    default:
        //SCI_TRACE_LOW:" the menu id is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15746_112_2_18_2_54_53_781,(uint8*)"");

		break;
    }
    
    //先判断sim卡是否支持cphs协议，若支持则语音信箱的号码存入sim卡
    MMISMS_SetCurVmInfo(cur_vmnum_info);  

	SCI_TRACE_LOW("SaveVMAddr2SimByType, cur_vmnum_info = %d, dual_sys=%d, voice_mail_type = %d",cur_vmnum_info, dual_sys, voice_mail_type);
    MNSMS_SetVoiMessAddrEx (dual_sys, line_type, voice_mail_type, &voimess_addr_info);
}

#else
/*****************************************************************************/
//     Description : save voice mail addr to simcard
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 
/*****************************************************************************/
LOCAL void SaveVMAddr2Sim(MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr,MN_DUAL_SYS_E dual_sys, MMI_STRING_T* num_string)
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MN_SMS_VMW_MBNUM_T voimess_addr_info = {0};
    uint8 record_id = 0;
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type = MN_MULTI_SUBSCRIBER_MAX;
    MN_VOICE_MAIL_TYPE_E voice_mail_type = MN_VOICE_MAIL;
	
    MMI_WSTRNTOSTR((uint8 *)voice_mail,
                                MMISMS_PBNUM_MAX_LEN + 2,
                                num_string->wstr_ptr,
                                num_string->wstr_len,
                                MIN(MMISMS_PBNUM_MAX_LEN, num_string->wstr_len));
 
    MMIAPICOM_GenPartyNumber(voice_mail, (int16)MIN(MMISMS_PBNUM_MAX_LEN, num_string->wstr_len), &party_num );

    //set the parameter 
    voimess_addr_info.alpha_id_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN;
    voimess_addr_info.mailbox_number_len = party_num.num_len;
    voimess_addr_info.is_ton_npi_exist = TRUE;
    voimess_addr_info.is_ccp_exist = FALSE;
    voimess_addr_info.ccp_id = 0;
    voimess_addr_info.is_ext_exist = FALSE;
    voimess_addr_info.ton = party_num.number_type;
    voimess_addr_info.npi = MN_NUM_PLAN_ISDN_TELE;
    voimess_addr_info.ext_id = 0;
    SCI_MEMSET( voimess_addr_info.mailbox_number, 0, sizeof(voimess_addr_info.mailbox_number));
    SCI_MEMCPY( voimess_addr_info.mailbox_number, party_num.bcd_num, voimess_addr_info.mailbox_number_len);
    
    GUIMENU_GetId( MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
    switch ( menu_id )
    {
    case MMISMS_MENU_LINE_VMN_ITEM_ID:
	 record_id = 1;
        win_data_ptr->cur_vmnum_info.record_id = record_id;
        win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len = party_num.num_len;
        win_data_ptr->cur_vmnum_info.line1_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num, 0, sizeof( win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num ));
        SCI_MEMCPY( win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num, party_num.bcd_num,win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_1;
        voice_mail_type = MN_VOICE_MAIL; 
		break;
#ifdef MMI_CSP_SUPPORT
    case MMISMS_MENU_LINE1_VMN_ITEM_ID:
        record_id = 1;
        win_data_ptr->cur_vmnum_info.record_id = record_id;
        win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len = party_num.num_len;
        win_data_ptr->cur_vmnum_info.line1_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num, 0, sizeof( win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num ));
        SCI_MEMCPY( win_data_ptr->cur_vmnum_info.line1_vmn_info.bcd_num, party_num.bcd_num,win_data_ptr->cur_vmnum_info.line1_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_1;
        voice_mail_type = MN_VOICE_MAIL; 
		break;

    case MMISMS_MENU_LINE2_VMN_ITEM_ID:
        record_id = 2;
        win_data_ptr->cur_vmnum_info.record_id = record_id;
        win_data_ptr->cur_vmnum_info.line2_vmn_info.num_len = party_num.num_len;
        win_data_ptr->cur_vmnum_info.line2_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( win_data_ptr->cur_vmnum_info.line2_vmn_info.bcd_num, 0, sizeof( win_data_ptr->cur_vmnum_info.line2_vmn_info.bcd_num ));
        SCI_MEMCPY( win_data_ptr->cur_vmnum_info.line2_vmn_info.bcd_num, party_num.bcd_num,win_data_ptr->cur_vmnum_info.line2_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_2;
        voice_mail_type = MN_VOICE_MAIL;    
		break;
#endif

    case MMISMS_MENU_FAX_VMN_ITEM_ID:
        record_id = 3;
        win_data_ptr->cur_vmnum_info.record_id = record_id;
        win_data_ptr->cur_vmnum_info.fax_vmn_info.num_len = party_num.num_len;
        win_data_ptr->cur_vmnum_info.fax_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( win_data_ptr->cur_vmnum_info.fax_vmn_info.bcd_num, 0, sizeof( win_data_ptr->cur_vmnum_info.fax_vmn_info.bcd_num ));
        SCI_MEMCPY( win_data_ptr->cur_vmnum_info.fax_vmn_info.bcd_num, party_num.bcd_num,win_data_ptr->cur_vmnum_info.fax_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_1;
        voice_mail_type = MN_VOICE_MAIL_FAX; 
		break;

    case MMISMS_MENU_DATA_VMN_ITEM_ID:
        record_id = 4;
        win_data_ptr->cur_vmnum_info.record_id = record_id;
        win_data_ptr->cur_vmnum_info.data_vmn_info.num_len = party_num.num_len;
        win_data_ptr->cur_vmnum_info.data_vmn_info.number_type = party_num.number_type;
        SCI_MEMSET( win_data_ptr->cur_vmnum_info.data_vmn_info.bcd_num, 0, sizeof( win_data_ptr->cur_vmnum_info.data_vmn_info.bcd_num ));
        SCI_MEMCPY( win_data_ptr->cur_vmnum_info.data_vmn_info.bcd_num, party_num.bcd_num,win_data_ptr->cur_vmnum_info.data_vmn_info.num_len);
        line_type = MN_MULTI_SUBSCRIBER_ID_1;
        voice_mail_type = MN_VOICE_MAIL_EMAIL; 
		break;

    default:
        //SCI_TRACE_LOW:" the menu id is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15829_112_2_18_2_54_53_782,(uint8*)"");
        break;
    }
    MMISMS_SetCurVmInfo(&win_data_ptr->cur_vmnum_info);     
    //先判断sim卡是否支持cphs协议，若支持则语音信箱的号码存入sim卡
	SCI_TRACE_LOW("SaveVMAddr2SimByType, win_data_ptr->cur_vmnum_info = %d, dual_sys=%d, voice_mail_type = %d",win_data_ptr->cur_vmnum_info, dual_sys, voice_mail_type);
    MNSMS_SetVoiMessAddrEx (dual_sys, line_type, voice_mail_type, &voimess_addr_info);
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to handle the edit voice mail window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSVMEditWindow(
                                       MMI_WIN_ID_T        win_id,    //IN:
                                       MMI_MESSAGE_ID_E msg_id,    //IN:
                                       DPARAM            param    //IN:
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T num_string = {0};
    MMI_STRING_T wait_text = {0};    
    MMISMS_VMN_RECORD_ID_E  vmn_type = ( uint32 )MMK_GetWinAddDataPtr(win_id);

    MMISMS_VOICEMAIL_WINDATA_T *win_data_ptr = (MMISMS_VOICEMAIL_WINDATA_T *)MMK_GetWinUserData(MMISMS_VOICEMAIL_SETTING_MENU_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms voice mail win_data_ptr param PNULL");
        return MMI_RESULT_TRUE;    
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        switch ( vmn_type )
        {
        case MMISMS_LINE1_VMN_RECORD_ID:
#ifdef MMI_CSP_SUPPORT
            if(MMISET_IsAlsValid(MMISMS_GetCurVoiceMailDualSys()))
            {
                text_id = TXT_LINE1_NUMBER;
            }
            else
#else
            {
                text_id = TXT_LINE_NUMBER;
            }
#endif
            break;
            
#ifdef MMI_CSP_SUPPORT 
        case MMISMS_LINE2_VMN_RECORD_ID:
            text_id = TXT_LINE2_NUMBER;
            break;
#endif //MMI_CSP_SUPPORT  
            
        case MMISMS_FAX_VMN_RECORD_ID:
            text_id = TXT_COMM_FAX_NUMBER;
            break;
            
        case MMISMS_DATA_VMN_RECORD_ID:
            text_id = TXT_SMS_DATA_NUMBER;
            break;

        default:
            break;
        }
        
        GUIWIN_SetTitleTextId(win_id, text_id, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID);
        break;
       
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MMISMS_MSG_UPDATELIST:
        GUIEDIT_GetString(MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID, &num_string);
        if (num_string.wstr_len > 0 && !MMIAPICOM_IsValidNumberStringEx(&num_string))
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);

            break;
        }
       
        SCI_TRACE_LOW("zhangqiang test num_string.wchar_len =%d",num_string.wstr_len);
        if(PNULL!=num_string.wstr_ptr)
        {
            if('+' ==num_string.wstr_ptr[0] && MMISMS_VMNUM_MAX_LEN==num_string.wstr_len)
            {
                num_string.wstr_len +=1;
            }
            SaveVMAddr2SimByType(&win_data_ptr->cur_vmnum_info,MMISMS_GetCurVoiceMailDualSys(),&num_string,vmn_type);
            
            SetVMDetail(win_data_ptr,MMISMS_VMN_LABEL_CTRL_ID);
        }
        else
        {
            MMK_CloseWin(win_id);
            break;
        }
          //进入等待界面,存号码
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleGetVMWaitiingWindow);      
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


#else
/*****************************************************************************/
//     Description : to handle the edit voice mail window
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSVMEditWindow(
                                       MMI_WIN_ID_T        win_id,    //IN:
                                       MMI_MESSAGE_ID_E msg_id,    //IN:
                                       DPARAM            param    //IN:
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T title = {0};
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T num_string = {0};
    MMI_STRING_T wait_text = {0};    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMISMS_VM_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        //设置标签标题
        MMI_GetLabelTextByLang(TXT_INPUTNUM, &title);
        GUILABEL_SetText(MMISMS_VM_EDIT_WIN_CTRL_ID,&title,FALSE);

        GUIMENU_GetId(MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
	case MMISMS_MENU_LINE_VMN_ITEM_ID:
		text_id = TXT_LINE_NUMBER;
		break;
#ifdef MMI_CSP_SUPPORT          
	case MMISMS_MENU_LINE1_VMN_ITEM_ID:
	        text_id = TXT_LINE1_NUMBER;
	    break;
	     
	case MMISMS_MENU_LINE2_VMN_ITEM_ID:
	    text_id = TXT_LINE2_NUMBER;
	    break;
#endif            
            
        case MMISMS_MENU_FAX_VMN_ITEM_ID:
            text_id = TXT_COMM_FAX_NUMBER;
            break;
            
        case MMISMS_MENU_DATA_VMN_ITEM_ID:
            text_id = TXT_SMS_DATA_NUMBER;
            break;

        default:
            break;
        }
        
        GUIWIN_SetTitleTextId(win_id, text_id, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID);
        break;
       
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MMISMS_MSG_UPDATELIST:
        GUIEDIT_GetString(MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID, &num_string);
        if (num_string.wstr_len > 0 && !MMIAPICOM_IsValidNumberStringEx(&num_string))
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);

            break;
        }
        //SaveVMAddr2Sim(MMISMS_GetCurVoiceMailDualSys(), &num_string);

        //SetVMDetail(win_data_ptr,MMISMS_VMN_LABEL_CTRL_ID);

          //进入等待界面,存号码
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleGetVMWaitiingWindow);      
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to open the window to show voice mailbox number of line1 ,line2
//                 fax and data
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 2006-02-10
/*****************************************************************************/
/*LOCAL*/PUBLIC void OpenVMListWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type)//20150618.vm
{
    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type = MN_MULTI_SUBSCRIBER_MAX;
    MN_VOICE_MAIL_TYPE_E voice_mail_type = MN_VOICE_MAIL;
    if (dual_sys >=  MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISMS:OpenVMNumberWin dual_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_5005_112_2_18_2_54_30_730,(uint8*)"");
        return;
    }

	switch(vmn_type)
	{
	case MMISMS_LINE1_VMN_RECORD_ID:
		line_type = MN_MULTI_SUBSCRIBER_ID_1;
		voice_mail_type = MN_VOICE_MAIL;
		break;
		
	case MMISMS_LINE2_VMN_RECORD_ID:
		line_type = MN_MULTI_SUBSCRIBER_ID_2;
		voice_mail_type = MN_VOICE_MAIL;
		break;
		
	case MMISMS_FAX_VMN_RECORD_ID:
		line_type =MN_MULTI_SUBSCRIBER_ID_1;
		voice_mail_type = MN_VOICE_MAIL_FAX;
		break;
		
	case MMISMS_DATA_VMN_RECORD_ID:
		line_type = MN_MULTI_SUBSCRIBER_ID_1;
		voice_mail_type = MN_VOICE_MAIL_EMAIL; 
		break;
		
	default:
		//do nothing
		break;
	}

    if (MMIAPICC_GetSimCallReady(dual_sys))
    {
        MMI_STRING_T wait_text = {0};
		ERR_MNDATAMAG_CODE_E result = ERR_MNSMS_NONE;

		SCI_TRACE_LOW("OpenVMNumberWin-L1860,dual_sys = %d, line_type= %d, voice_mail_type= %d, vmn_type = %d ", dual_sys, line_type, voice_mail_type, vmn_type);
        result = MNSMS_GetVoiMessAddrEx(dual_sys, line_type, voice_mail_type);
        MMISMS_SetVMAddr(dual_sys, vmn_type );
        //SCI_TRACE_LOW:"OpenVMNumberWin-L1860: MNSMS_GetVoiMessAddr()"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_5014_112_2_18_2_54_30_731,(uint8*)"");

        //进入等待界面
//		if (ERR_MNDATAMAG_NO_ERR == result)
		{
			MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
			MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
								ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleGetVMWaitiingWindow);
		} 
// 		else
// 		{
// 			SCI_TRACE_LOW("OpenVMNumberWin-L1860, result= %d ", result);
// 			MMIPUB_OpenAlertWarningWin(TXT_COMMON_WAITING);
// 		}

    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatusII(dual_sys);
    }
}
#endif
/*****************************************************************************/
//     Description : to open the window to show voice mailbox number of line1 ,line2
//                 fax and data
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 2006-02-10
/*****************************************************************************/
LOCAL void OpenVMNumberWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type)
{
    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type = MN_MULTI_SUBSCRIBER_MAX;
    MN_VOICE_MAIL_TYPE_E voice_mail_type = MN_VOICE_MAIL;
    if (dual_sys >=  MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISMS:OpenVMNumberWin dual_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_5036_112_2_18_2_54_30_732,(uint8*)"");
        return;
    }

	switch(vmn_type)
	    {
	        case MMISMS_LINE1_VMN_RECORD_ID:
	            line_type = MN_MULTI_SUBSCRIBER_ID_1;
	            voice_mail_type = MN_VOICE_MAIL;
	            break;
	        case MMISMS_LINE2_VMN_RECORD_ID:
	            line_type = MN_MULTI_SUBSCRIBER_ID_2;
	            voice_mail_type = MN_VOICE_MAIL;
	            break;
	        case MMISMS_FAX_VMN_RECORD_ID:
	            line_type = MN_MULTI_SUBSCRIBER_ID_1;
	            voice_mail_type = MN_VOICE_MAIL_FAX;
	            break;

	        case MMISMS_DATA_VMN_RECORD_ID:
	            line_type = MN_MULTI_SUBSCRIBER_ID_1;
	            voice_mail_type = MN_VOICE_MAIL_EMAIL; 
	            break;

	        default:
	            //do nothing
	            break;
	    }
    if (MMIAPICC_GetSimCallReady(dual_sys))
    {
        MMI_STRING_T wait_text = {0};
		ERR_MNDATAMAG_CODE_E result = ERR_MNSMS_NONE;

        if (MMISMS_VMN_OPER_TYPE_IDLE_DIAL == MMISMS_GetVmOperType())
        {
            uint8 line_info = MMIAPISET_GetCallLineInfo(dual_sys);

            if (1 == line_info)
            {
                vmn_type = MMISMS_LINE2_VMN_RECORD_ID;
            }
            else
            {
                vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
            }
        }

		SCI_TRACE_LOW("OpenVMNumberWin-L1937,dual_sys = %d, line_type= %d, voice_mail_type= %d, vmn_type = %d ", dual_sys, line_type, voice_mail_type, vmn_type);
        
		result = MNSMS_GetVoiMessAddrEx(dual_sys, line_type, voice_mail_type);
        MMISMS_SetVMAddr(dual_sys, vmn_type );
        //SCI_TRACE_LOW:"OpenVMNumberWin-L1937: MNSMS_GetVoiMessAddr()"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_5060_112_2_18_2_54_30_733,(uint8*)"");

//		if (ERR_MNDATAMAG_NO_ERR == result)
		{
			//进入等待界面
			MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
			MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
				ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleGetVMWaitiingWindow);
		} 
// 		else
// 		{
// 			SCI_TRACE_LOW("OpenVMNumberWin-L1937, result= %d ", result);
// 			MMIPUB_OpenAlertWarningWin(TXT_COMMON_WAITING);
// 		}
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatusII(dual_sys);
    }
}

/*****************************************************************************/
//     Description : to open the window to show voice mailbox number of line1 ,line2
//                 fax and data
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 2006-02-10
/*****************************************************************************/
PUBLIC void MMISMS_OpenVMNumberWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type)
{
    // MMI_STRING_T wait_text = {0};
    uint16 sim_ok = 0;
    uint32 sim_ok_num = 0;

    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        OpenVMNumberWin(dual_sys, vmn_type);
    }
    else
    {
        sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);

        if (0 == sim_ok_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_ok_num)
        {
            OpenVMNumberWin((MN_DUAL_SYS_E)sim_ok, vmn_type);
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
             MMISMS_SelectSIMFunc(OpenVMSelectSimCallback,(ADD_DATA)vmn_type,TRUE);
        }
#endif
    }
}


/*****************************************************************************/
//     Description : 语音信箱号码界面打开时，等待界面不允许返回
//    Global resource dependence : 
//  Author: kelly.li
//    Note:2006-02-13
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGetVMWaitiingWindow(
                                           MMI_WIN_ID_T      win_id,     // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM                 param        // 相应消息的参数
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
        
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(MMISMS_COM_SELECT_SIM_WIN_ID);
        break;
#endif

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

#endif