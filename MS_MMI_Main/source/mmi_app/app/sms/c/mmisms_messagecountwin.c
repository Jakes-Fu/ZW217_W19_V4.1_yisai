/****************************************************************************
** File Name:      mmisms_messagecountwin.c                                    *
** Author:         liming deng                                             *
** Date:           08/14/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012        rong.gu      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmisms_messagecountwin.h"
#ifndef MMI_GUI_STYLE_TYPICAL 
#include "guilistbox.h"
#else
#include "guirichtext.h"
#endif
#include "mmisms_text.h"
#include "mmicom_string.h"
#include "mmisms_id.h"
#include "mmi_appmsg.h"
#include "mmiset_export.h"
#include "guictrl_api.h"
    
#ifdef MMS_SUPPORT
#include "mmimms_export.h"
#endif
#include "mmisms_order.h"
#include "mmisms_showsmswin.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISMS_MEMSTATE_MAX_LEN 20
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

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
//     Description : set SMS memory state to string 
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
#ifndef MMI_GUI_STYLE_TYPICAL 
LOCAL void SetSmsMemState2String(
                                MMI_CTRL_ID_T ctrl_id,
                                GUILIST_ITEM_DATA_T *item_data,
                                uint16 *index,
                                MN_DUAL_SYS_E dual_sys,
                                BOOLEAN is_nv
                                );
#else
LOCAL void SetSmsMemState2String(
                                MMI_CTRL_ID_T ctrl_id,
                                GUIRICHTEXT_ITEM_T *item_data,
                                uint16 *index,
                                MN_DUAL_SYS_E dual_sys,
                                BOOLEAN is_nv
                                );
#endif

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
/*****************************************************************************/
//  Description : add mms richtext items
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AddMailMemStateItems(
                                 MMI_CTRL_ID_T ctrl_id,
#ifndef MMI_GUI_STYLE_TYPICAL 
                                 GUILIST_ITEM_DATA_T *item_data,
#else
                                 GUIRICHTEXT_ITEM_T *item_data,
#endif
                                 uint16 *index); 
#endif

/*****************************************************************************/
//     Description : 5.10 display content window message handler
//    Global resource dependence : 
//  Author:Bruce.Chi
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E Content_HandleWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM                param
                                        );
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
// for showing SMS memory space
WINDOW_TABLE(MMIMESSAGE_CONTENT_WIN) = 
{
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
#endif
    WIN_TITLE(TXT_SMS_MESSAGESPACE),
#ifdef MONOCHROME_LCD_SUPPORT 
#else			
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
#endif 
    WIN_FUNC((uint32) Content_HandleWinMsg),    
    WIN_ID(MMISMS_MEMORY_STATE_WIN_ID),
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_MESSAGE_CONTENT_TEXT_CTRL_ID),
#ifndef MMI_PDA_SUPPORT    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),    
#endif    
#else
    CREATE_RICHTEXT_CTRL(MMISMS_MESSAGE_CONTENT_TEXT_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMessageCountChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMISMS_MEMORY_STATE_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms MTBox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {          
        MMK_PostMsg(win_id, MMISMS_MSG_UPDATEMENU, PNULL, 0);
    }    
}



#ifndef  MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : set message list item
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void SetMessageListItem(void)
{  
    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/
    MMI_CTRL_ID_T list_ctrl_id = MMISMS_MESSAGE_CONTENT_TEXT_CTRL_ID;

    MMI_STRING_T string_prefix = {0};
    wchar item_text[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint32 i = 0;
    uint16 index;
    //load log list
    GUILIST_SetMaxItem(list_ctrl_id, 100, FALSE );//max item 20
    GUILIST_RemoveAllItems(list_ctrl_id);

    GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, TRUE); 

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    MMI_GetLabelTextByLang(TXT_SMS_SMS_SPACE, &string_prefix);
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUILIST_STRING_MAX_NUM);;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = item_text;
    item_t.item_data_ptr = &item_data;
    GUILIST_AppendItem( list_ctrl_id, &item_t );
    
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        SetSmsMemState2String(list_ctrl_id, &item_data,&index, i, FALSE);
    }
    
    SetSmsMemState2String(list_ctrl_id, &item_data ,&index, MN_DUAL_SYS_1, TRUE); //nv
    
#ifdef MMS_SUPPORT //added by taiping.lai 2010/12/28
    //mms memory
    MMIAPIMMS_SetMMSMemState(list_ctrl_id, &item_data, &index);
#endif

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    //ÏÔÊ¾139ÓÊÏäÈÝÁ¿
    AddMailMemStateItems(list_ctrl_id, &item_data, &index);   
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
    
}
/*****************************************************************************/
//     Description : 5.10 display content window message handler
//    Global resource dependence : 
//  Author:Bruce.Chi
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E Content_HandleWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM                param
                                        )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl_id = MMISMS_MESSAGE_CONTENT_TEXT_CTRL_ID;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    LOCAL BOOLEAN s_need_update = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);    
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMessageCountChangeCallBack, TRUE); 
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        break;
        
    case MSG_FULL_PAINT:      
        SetMessageListItem();
        break;
        
    case MMISMS_MSG_NEEDUPDATEMENU:
        s_need_update = TRUE;
        break;
        
    case MSG_GET_FOCUS:
        if (s_need_update)
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            s_need_update = FALSE;
        }
        break;
    case MMISMS_MSG_UPDATEMENU:
        if(MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        s_need_update = TRUE;
        break;

    case MSG_CTL_CANCEL:
        // close the window
        MMK_CloseWin(win_id);
        break;

    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }

    return err_code;
}
#else
/*****************************************************************************/
//     Description : 5.10 display content window message handler
//    Global resource dependence : 
//  Author:Bruce.Chi
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E Content_HandleWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM                param
                                        )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_MESSAGE_CONTENT_TEXT_CTRL_ID;
    LOCAL BOOLEAN s_need_update = FALSE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2010/12/28
     {
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        GUI_RECT_T rect={0, 0, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1};
				
        GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
        Edit_SetMemoryStateContent(ctrl_id);
        GUIRICHTEXT_SetLineSpace(ctrl_id, MMISMS_DETAIL_LINE_SPACE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
	}
#else
        //set label window text
        Edit_SetMemoryStateContent(ctrl_id);

        GUIRICHTEXT_SetLineSpace(ctrl_id, MMISMS_DETAIL_LINE_SPACE);

        //GUIRICHTEXT_SetScroll(ctrl_id, TRUE);

        //set sofkey
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);

        MMK_SetAtvCtrl(win_id, ctrl_id);
#endif
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMessageCountChangeCallBack, TRUE); 
        break;

    case MMISMS_MSG_NEEDUPDATEMENU:
        s_need_update = TRUE;
        break;

    case MSG_GET_FOCUS:
        if (s_need_update)
        {
            GUIRICHTEXT_DeleteAllItems(ctrl_id);

            //set label window text
            Edit_SetMemoryStateContent(ctrl_id);

            s_need_update = FALSE;
        }
        break;

    case MMISMS_MSG_UPDATEMENU:
        GUIRICHTEXT_DeleteAllItems(ctrl_id);

        //set label window text
        Edit_SetMemoryStateContent(ctrl_id);

        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_CTL_CANCEL:
        // close the window
        MMK_CloseWin(win_id);
        break;

    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }

    return err_code;
}
#endif

/*****************************************************************************/
// 	Description : set SMS memory state to string 
//	Global resource dependence : 
//  Author: Tracy Zhang
//	Note: 
/*****************************************************************************/
#ifndef MMI_GUI_STYLE_TYPICAL
LOCAL void SetSmsMemState2String(
                                 MMI_CTRL_ID_T ctrl_id,
                                 GUILIST_ITEM_DATA_T *item_data,
                                 uint16 *index,
                                 MN_DUAL_SYS_E dual_sys,
                                 BOOLEAN is_nv
                                 )
{
    uint8 mem_string[MMISMS_MEMSTATE_MAX_LEN] = {0};
    wchar mem_wstring[MMISMS_MEMSTATE_MAX_LEN + 1] = {0};
    MMI_TEXT_ID_T	text_id = TXT_NULL;
    MMI_STRING_T string_prefix = {0};
    MMI_STRING_T mem_status = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_str = {0};
#endif
    uint16 wstr_len = 0;
    uint16 sms_max_num = 0;
    uint16 sms_used_num = 0;
    uint16 sms_ori_used_num = 0;
    wchar item_text[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_CTRL_ID_T list_ctrl_id = MMISMS_MESSAGE_CONTENT_TEXT_CTRL_ID;
    GUILIST_ITEM_T item_t = {0};

    if (PNULL == item_data)
    {
        //SCI_TRACE_LOW:"MMISMS:SetSmsMemState2String item_data index = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_4082_112_2_18_2_54_28_721,(uint8*)"");
        return;
    }

    if (is_nv)
    {
        text_id = TXT_PHONE;
        MMI_GetLabelTextByLang(text_id, &string_prefix);
    }
    else
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        sim_str = MMIAPISET_GetSimName(dual_sys);
        string_prefix.wstr_len = sim_str.wstr_len;
        string_prefix.wstr_ptr = sim_str.wstr_arr;
#else
        text_id = TXT_COMM_SIM;
        MMI_GetLabelTextByLang(text_id, &string_prefix);
#endif
    }

    wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUILIST_STRING_MAX_NUM);
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t.item_data_ptr = item_data;
    
    item_data->item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
   
    item_data->item_content[0].item_data.text_buffer.wstr_len = string_prefix.wstr_len;
    item_data->item_content[0].item_data.text_buffer.wstr_ptr = string_prefix.wstr_ptr;
    
    // get max and used num in SMS storage.
    MMISMS_GetSMSUsedAndMax(dual_sys, is_nv, &sms_max_num, &sms_used_num, &sms_ori_used_num);

    mem_status.wstr_ptr = mem_wstring;
    mem_status.wstr_len = sprintf((char *)mem_string, "%d/%d", sms_ori_used_num, sms_max_num);

    MMI_STRNTOWSTR(mem_status.wstr_ptr,
                                MMISMS_MEMSTATE_MAX_LEN,
                                (const uint8 *)mem_string,
                                MMISMS_MEMSTATE_MAX_LEN,
                                mem_status.wstr_len);

    wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUILIST_STRING_MAX_NUM);
    
    item_data->item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
   
    item_data->item_content[1].item_data.text_buffer.wstr_len = mem_status.wstr_len;
    item_data->item_content[1].item_data.text_buffer.wstr_ptr = mem_status.wstr_ptr;

    GUILIST_AppendItem( list_ctrl_id, &item_t );    

    if (!is_nv)
    {
        MMI_STRING_T unloaded_prefix = {0};
        uint16 unloaded_sms = 0;
        GUILIST_ITEM_DATA_T item_data_undownload                       = {0};
        GUILIST_ITEM_T item_t_undownload = {0};
        
        item_t_undownload.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
        item_t_undownload.item_data_ptr = &item_data_undownload;
    
        if (sms_used_num > sms_ori_used_num)
        {
            unloaded_sms = sms_used_num - sms_ori_used_num;

            wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUILIST_STRING_MAX_NUM);
            item_data_undownload.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data_undownload.item_content[0].item_data.text_buffer.wstr_len  = wstr_len;
            item_data_undownload.item_content[0].item_data.text_buffer.wstr_ptr = item_text;
            
            text_id = TXT_SMS_UNLOADED_SMS;
            MMI_GetLabelTextByLang(text_id, &unloaded_prefix);

            wstr_len = MMIAPICOM_CopyStr2Buf(item_text + wstr_len, &unloaded_prefix, GUILIST_STRING_MAX_NUM);
            item_data_undownload.item_content[0].item_data.text_buffer.wstr_len += wstr_len;

            mem_status.wstr_ptr = mem_wstring;
            mem_status.wstr_len = sprintf((char *)mem_string, "%d/%d", unloaded_sms, sms_max_num);

            MMI_STRNTOWSTR(mem_status.wstr_ptr,
                                        MMISMS_MEMSTATE_MAX_LEN,
                                        (const uint8 *)mem_string,
                                        MMISMS_MEMSTATE_MAX_LEN,
                                        mem_status.wstr_len);

            //wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &mem_status, GUILIST_STRING_MAX_NUM);
            item_data_undownload.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data_undownload.item_content[1].item_data.text_buffer.wstr_len = mem_status.wstr_len;
            item_data_undownload.item_content[1].item_data.text_buffer.wstr_ptr= mem_status.wstr_ptr;
            GUILIST_AppendItem( list_ctrl_id, &item_t_undownload );   
             
        }
    }      
}
#else
LOCAL void SetSmsMemState2String(
                                 MMI_CTRL_ID_T ctrl_id,
                                 GUIRICHTEXT_ITEM_T *item_data,
                                 uint16 *index,
                                 MN_DUAL_SYS_E dual_sys,
                                 BOOLEAN is_nv
                                 )
{
    uint8 mem_string[MMISMS_MEMSTATE_MAX_LEN] = {0};
    wchar mem_wstring[MMISMS_MEMSTATE_MAX_LEN + 1] = {0};
    MMI_TEXT_ID_T	text_id = TXT_NULL;
    MMI_STRING_T string_prefix = {0};
    MMI_STRING_T colon_string = {0};
    MMI_STRING_T mem_status = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_str = {0};
#endif
    uint16 wstr_len = 0;
    uint16 sms_max_num = 0;
    uint16 sms_used_num = 0;
    uint16 sms_ori_used_num = 0;
    wchar colon_str[] = {':', 0};
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};

    if (PNULL == item_data || PNULL == index)
    {
        //SCI_TRACE_LOW:"MMISMS:SetSmsMemState2String item_data index = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_4194_112_2_18_2_54_28_722,(uint8*)"");
        return;
    }

    if (is_nv)
    {
        text_id = TXT_PHONE;
        MMI_GetLabelTextByLang(text_id, &string_prefix);
    }
    else
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        sim_str = MMIAPISET_GetSimName(dual_sys);
        string_prefix.wstr_len = sim_str.wstr_len;
        string_prefix.wstr_ptr = sim_str.wstr_arr;
#else
        text_id = TXT_COMM_SIM;
        MMI_GetLabelTextByLang(text_id, &string_prefix);
#endif
    }

    wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data->text_data.buf.len = wstr_len;
    item_data->text_data.buf.str_ptr = item_text;

    //add ":"
    colon_string.wstr_ptr = (wchar *)colon_str;
    colon_string.wstr_len = 1;
    wstr_len = MMIAPICOM_CopyStr2Buf(item_text + item_data->text_data.buf.len, &colon_string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data->text_data.buf.len += wstr_len;
    

    GUIRICHTEXT_AddItem(ctrl_id, item_data, index);

    // get max and used num in SMS storage.
    MMISMS_GetSMSUsedAndMax(dual_sys, is_nv, &sms_max_num, &sms_used_num, &sms_ori_used_num);

    mem_status.wstr_ptr = mem_wstring;
    mem_status.wstr_len = sprintf((char *)mem_string, "%d/%d", sms_ori_used_num, sms_max_num);

    MMI_STRNTOWSTR(mem_status.wstr_ptr,
                                MMISMS_MEMSTATE_MAX_LEN,
                                (const uint8 *)mem_string,
                                MMISMS_MEMSTATE_MAX_LEN,
                                mem_status.wstr_len);

    wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &mem_status, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data->text_data.buf.str_ptr = item_text;
    item_data->text_data.buf.len = wstr_len;
    GUIRICHTEXT_AddItem(ctrl_id, item_data, index);

    if (!is_nv)
    {
        MMI_STRING_T unloaded_prefix = {0};
        uint16 unloaded_sms = 0;

        if (sms_used_num > sms_ori_used_num)
        {
            unloaded_sms = sms_used_num - sms_ori_used_num;

            wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
            item_data->text_data.buf.len = wstr_len;
            item_data->text_data.buf.str_ptr = item_text;

            //add "TXT_SMS_UNLOADED_SMS"
            text_id = TXT_SMS_UNLOADED_SMS;
            MMI_GetLabelTextByLang(text_id, &unloaded_prefix);

            wstr_len = MMIAPICOM_CopyStr2Buf(item_text + item_data->text_data.buf.len, &unloaded_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
            item_data->text_data.buf.len += wstr_len;

            GUIRICHTEXT_AddItem(ctrl_id, item_data, index);

            mem_status.wstr_ptr = mem_wstring;
            mem_status.wstr_len = sprintf((char *)mem_string, "%d/%d", unloaded_sms, sms_max_num);

            MMI_STRNTOWSTR(mem_status.wstr_ptr,
                                        MMISMS_MEMSTATE_MAX_LEN,
                                        (const uint8 *)mem_string,
                                        MMISMS_MEMSTATE_MAX_LEN,
                                        mem_status.wstr_len);

            wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &mem_status, GUIRICHTEXT_TEXT_MAX_LEN);
            item_data->text_data.buf.len = wstr_len;
            item_data->text_data.buf.str_ptr = item_text;
            GUIRICHTEXT_AddItem(ctrl_id, item_data, index);
        }
    }
}
#endif

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : add mms richtext items
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AddMailMemStateItems(
                                 MMI_CTRL_ID_T ctrl_id,
                                 GUILIST_ITEM_DATA_T *item_data,
                                 uint16 *index)
{

    uint8 mem_string[20 + 1] = {0};
    wchar mem_wstring[20 + 1]= {0};
    uint32 mail_num = 0;
    uint32 mail_max_num = 0;
    MMI_STRING_T mem_status = {0};
    
    MMI_STRING_T string_prefix = {0};
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
      
    //sms memory
    MMI_GetLabelTextByLang(TXT_MAIL_MEMORY, &string_prefix);
    item_t.item_data_ptr = item_data;
    
    item_data->item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;   
    item_data->item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);;
    item_data->item_content[0].item_data.text_buffer.wstr_ptr = item_text;
#ifdef MMI_OTA_SUPPORT
    mail_num = MMIAPIOTA_GetOTATotalNum();
#endif
    mail_max_num = MMIOTA_CONFIG_INFO_MAX;

    mail_num = mail_GetTotalBoxMailNum();
    mail_max_num = MAIL_BOX_MAIL_MAX_NUM;
    
    mem_status.wstr_len  = sprintf((char *)mem_string, "%ld/%ld", mail_num, mail_max_num);
    
    MMI_STRNTOWSTR(mem_wstring, 20, (const uint8 *)mem_string, 20, mem_status.wstr_len);

    mem_status.wstr_ptr = mem_wstring;

    item_data->item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;   
    item_data->item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);;
    item_data->item_content[1].item_data.text_buffer.wstr_ptr = item_text;
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    GUILIST_AppendItem( ctrl_id, &item_t );
}
#else
/*****************************************************************************/
//  Description : add mms richtext items
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AddMailMemStateItems(
                                 MMI_CTRL_ID_T ctrl_id,
                                 GUIRICHTEXT_ITEM_T *item_data,
                                 uint16 *index)
{

    uint8 mem_string[20 + 1] = {0};
    wchar mem_wstring[20 + 1]= {0};
    uint32 mail_num = 0;
    uint32 mail_max_num = 0;
    MMI_STRING_T mem_status = {0};
    
    MMI_STRING_T string_prefix = {0};
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    //sms memory
    MMI_GetLabelTextByLang(TXT_MAIL_MEMORY, &string_prefix);

    item_data->text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data->text_data.buf.str_ptr = item_text;
    GUIRICHTEXT_AddItem(ctrl_id, item_data, index);
#ifdef MMI_OTA_SUPPORT
    mail_num = MMIAPIOTA_GetOTATotalNum();
#endif
    mail_max_num = MMIOTA_CONFIG_INFO_MAX;

    mail_num = mail_GetTotalBoxMailNum();
    mail_max_num = MAIL_BOX_MAIL_MAX_NUM;
    
    mem_status.wstr_len  = sprintf((char *)mem_string, "%ld/%ld", mail_num, mail_max_num);
    
    MMI_STRNTOWSTR(mem_wstring, 20, (const uint8 *)mem_string, 20, mem_status.wstr_len);

    mem_status.wstr_ptr = mem_wstring;
    
    item_data->text_data.buf.str_ptr = mem_status.wstr_ptr;    
    item_data->text_data.buf.len = mem_status.wstr_len;
    GUIRICHTEXT_AddItem(ctrl_id, item_data, index);
    
}
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#endif

/*****************************************************************************/
//  Description : add mms richtext items
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterMessageCountWin(void)
{
    MMK_CreateWin((uint32 *)MMIMESSAGE_CONTENT_WIN, PNULL);
}
