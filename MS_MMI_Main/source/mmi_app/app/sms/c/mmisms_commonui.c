/****************************************************************************
** File Name:      mmisms_chatreadwin.c                                    *
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
#include "watch_commonwin_export.h"
//#include "watch_sms_messagebox.h"
#include "mmisms_commonui.h"
#include "mmisms_id.h"

#ifdef MMS_SUPPORT
#include "mmimms_text.h"
#include "mmimms_export.h"
#include "mmimms_id.h"
#include "mmiota_id.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#include "mmienvset_export.h"
#include "mmicc_export.h"
#include "mmisms_messagecountwin.h"
#include "mmiudisk_export.h"
#include "mmi_event_api.h"
#include "mmisms_read.h" 
#ifdef MMI_SMS_CHAT_SUPPORT
#include "mmisms_chatreadwin.h"
#endif
#include "mmiota_export.h"
#include "mmidc_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmiset_export.h"
#include "mmienvset_internal.h"
#ifdef SCREENSAVER_SUPPORT
#include "mmiss_export.h"
#endif
#include "mmiidle_statusbar.h"
//#include "mmischedule_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
//#include "mmisms_messageboxwin.h"
#include "mmieng_export.h"

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
#include "Mmibrowser_export.h"
#endif
#endif
#include "mmisms_send.h"
#include "mmisms_set.h"
#include "mmialarm_export.h"
//#include "mmisms_appcontrol.h"
#include "mmisms_order.h"

#ifdef CALENDAR_SUPPORT
#include "mmischedule_export.h"
#endif
#include "mmisms_appcontrol.h"
#include "mmismsapp_interface.h"
#include "mmisms_messageboxwin.h"
#include "mmialarm_id.h"
#include "mmisms_edit.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMISMS_PORTRAIT_SUPPORT
#define SMS_LIST_SMS_PB_PHOTO_ICON 0
#define SMS_LIST_SMS_STATUS_ICON 1
#define SMS_LIST_SMS_PHONE_NAME_ICON 2
#define SMS_LIST_SMS_DATE_ICON 3
#define SMS_LIST_SMS_SIM_ICON 4
#define SMS_LIST_CONTENT_SIM_ICON 5
#else
#define SMS_LIST_SMS_STATUS_ICON 0
#define SMS_LIST_SMS_PHONE_NAME_ICON 1
#define SMS_LIST_SMS_DATE_ICON 2
#define SMS_LIST_CONTENT_SIM_ICON 3
#endif
#endif
#define MMISMS_VIBRA_TIMEROUT        3000
//END
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

#ifdef  SXH_APPLE_SUPPORT
extern BOOLEAN AppleIsRun(void);/*lint !e18*/
#endif  

#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_HANDLE_T g_s_prompt_win_handle = 0;
#if defined(MMI_VCARD_SUPPORT)
LOCAL uint16 *s_new_vcard_name_ptr = PNULL;//vcard名称
#endif

#ifdef MMI_VCALENDAR_SUPPORT
LOCAL uint16 *s_new_vcalendar_name_ptr = PNULL;//vcard名称
#endif
LOCAL BOOLEAN s_sms_is_continue_mp3 = FALSE; //sms是否继续播放mp3
MMISMS_BOX_TYPE_E s_box_type = MMISMS_BOX_NONE;//sms信箱类型
MMISMS_OPERATE_TYPE_E g_oper_type = MMISMS_OPERATE_NONE;//短消息操作类型
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//LOCAL uint16 s_all_del_msg_num = 0;
//LOCAL BOOLEAN s_mms_is_sendrecv = FALSE;
//LOCAL BOOLEAN s_is_mms_del = FALSE;

//extern MMISMS_GLOBAL_T  g_mmisms_global; 
extern uint8  g_sms_retry_timer;
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get Small Icon by sim index
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSimSmallIcon(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSmallIconByStatus(MMISMS_STATE_T *sms_state_ptr);
#endif

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void ShowNewMsgPrompt(void);

/*****************************************************************************/
//     Description : to play msg ring
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void PlaySMSRing(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void SMS_ValidatePrivacyReadNewSms(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//     Description : to STOP the vibrator
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void RingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//  Description : is vibra new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNewMsgNeedVibra(void);

/*****************************************************************************/
//     Description : 开机初始化时，进入短消息的等待窗口处理函数
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitingWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       );

/*****************************************************************************/
//     Description : delete one sms call back
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_DeleteOneSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data);

#ifdef MMI_SMS_MOVE_SUPPORT 
/*****************************************************************************/
//     Description : delete one sms call back
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_MoveOneSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data);

/*****************************************************************************/
//  Description : to handle the message of move waiting window    
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleMoveWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );
#endif

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of move waiting window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleCopyWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );
/*****************************************************************************/
//     Description : delete one sms call back
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_CopyOneSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data);

#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to form the item for list_box
//    Global resource dependence : nsms1_state_ptr
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void FormListBoxItem(
                           MMISMS_STATE_T    *sms_state_ptr,     //IN:
                           MMI_CTRL_ID_T     ctrl_id,
                           uint16            index
                           )
{
    //GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_STRING_T name = {0}; 
    MMI_STRING_T time = {0};
    uint8 time_string[MMIMESSAGE_DIAPLAY_LEN] = {0};
    MMI_STRING_T subject = {0};
    MMI_TM_T sms_time = {0};
    SCI_DATE_T cur_date = {0};
    uint16 list_index = 0;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle( ctrl_id );
    wchar temp_arr1[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr2[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr3[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    //BOOLEAN is_display_year = FALSE;
    uint8 date_str[MMISET_DATE_STR_LEN + 1] = {0};
#ifdef MMI_MMS_LIST_DISPLAY_MULTI_PEOPLE
    wchar sender_wstr[MMISMS_SENDER_DISPLAY_STRING_MAX + 2] = {0};
#endif
    MMISMS_EMOJI_DISPLAY_CONTENT_T *display_data = PNULL;
    uint16 len = 0, i = 0;

    subject.wstr_ptr = temp_arr1;
    time.wstr_ptr    = temp_arr2;
    name.wstr_ptr    = temp_arr3;
        
    MMISMS_GetNameInfo(sms_state_ptr, &name, MMISMS_PBNAME_MAX_LEN);

    if ((sms_state_ptr->flag.mo_mt_type == MMISMS_MT_SR_HAVE_READ) ||
        (sms_state_ptr->flag.mo_mt_type ==MMISMS_MT_SR_TO_BE_READ) )
    {
        MMI_GetLabelTextByLang(TXT_SMS_STATUSREPORT, &subject);
        subject.wstr_len = MIN(MMISMS_PBNAME_MAX_LEN, subject.wstr_len);
    }
    else
    {
        display_data = (MMISMS_EMOJI_DISPLAY_CONTENT_T *)SCI_ALLOCA(sizeof(MMISMS_EMOJI_DISPLAY_CONTENT_T ));
        if (PNULL == display_data)
        {
            SCI_TRACE_LOW("[MMISMS] FormListBoxItem alloc display_data fail!");
            return;
        }

        SCI_MEMSET(display_data, 0x00, sizeof(MMISMS_EMOJI_DISPLAY_CONTENT_T));

        if(MMISMS_IsContainEmoji(sms_state_ptr->display_content.content, sms_state_ptr->display_content.content_len, display_data))
        {
            for (i = 0; i < display_data->length; i++)
            {
                if(PNULL != display_data->item[i].data)
                {
                    MMIAPICOM_Wstrcat(subject.wstr_ptr, &display_data->item[i].data);
                }
                else
                {
                    len = MMIAPICOM_Wstrlen(subject.wstr_ptr);
                    subject.wstr_ptr[len] = (wchar*)display_data->item[i].code;
                }
            }

            subject.wstr_len = (uint16)MMIAPICOM_Wstrlen(subject.wstr_ptr);
        }
        else
        {
            subject.wstr_len = sms_state_ptr->display_content.content_len;
            if(subject.wstr_len != 0)
            {

                MMI_WSTRNCPY(
                    subject.wstr_ptr,
                    MMIMESSAGE_DIAPLAY_LEN,
                    sms_state_ptr->display_content.content, 
                    subject.wstr_len,
                    subject.wstr_len
                    );
            }
        }
        //SCI_TRACE_LOW:"subject.length is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3429_112_2_18_2_54_0_629,(uint8*)"d",subject.wstr_len);
    }
    
    TM_GetSysDate(&cur_date);
    sms_time = MMIAPICOM_Second2Tm(sms_state_ptr->time);
    if ((sms_time.tm_mday == cur_date.mday)
        && (sms_time.tm_mon == cur_date.mon) 
        && (sms_time.tm_year == cur_date.year))
    {
        MMIAPISET_FormatTimeStrByTime(sms_time.tm_hour, sms_time.tm_min, time_string, MMISET_DATE_STR_LEN);
        time.wstr_len = sprintf((char *)time_string, "%s", time_string);
    }
    else
    {
/*      if (sms_time.tm_year != cur_date.year)
        {
            is_display_year = TRUE;
        }
*/        
        MMIAPISET_FormatDateStrByDateEx(sms_time.tm_year,
            sms_time.tm_mon,
            sms_time.tm_mday,
            '/',
            date_str,
            MMISET_DATE_STR_LEN,
            FALSE);
        time.wstr_len = sprintf((char *)time_string, "%s", date_str);
    }
    MMI_STRNTOWSTR(time.wstr_ptr,
                                MMIMESSAGE_DIAPLAY_LEN,
                                (uint8 *)time_string,
                                time.wstr_len,
                                time.wstr_len
                                );

    list_index = 0;
    
#ifdef MMI_SMS_CHAT_SUPPORT
    if(MMISMS_CHATBOX_LISTBOX_CTRL_ID == ctrl_id)
    {
#ifdef MMISMS_PORTRAIT_SUPPORT
        item_data.item_content[list_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
#else
        MMI_SMS_CHAT_STATUS_T chat_status = MMISMS_FindChatSMSNumStatusInList(index);

        item_data.item_content[list_index].item_data_type = GUIITEM_DATA_IMAGE_ID;

        if (chat_status.is_unread_sms && chat_status.is_unread_mms)//未读短彩
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_MMS_ICON;
        }
        else if (!chat_status.is_unread_sms && chat_status.is_unread_mms)//未读彩信
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_NOT_READ_MMS_ICON;
        }
        else if (chat_status.is_unread_sms && !chat_status.is_unread_mms)//未读短信
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_ICON;
        }
        else if (chat_status.is_exist_sms && chat_status.is_exist_mms)//短彩同时存在
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_READ_SMS_MMS_ICON;
        }
        else if (!chat_status.is_exist_sms && chat_status.is_exist_mms)//只有彩信
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_READ_MMS_ICON;
        }
        else//只有短信
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_READ_SMS_ICON;
        }
#endif
    }
    else
#endif        
    {
#ifdef MMISMS_PORTRAIT_SUPPORT
        item_data.item_content[SMS_LIST_SMS_PB_PHOTO_ICON].item_data_type = GUIITEM_DATA_ANIM_PATH;
#endif
        item_data.item_content[SMS_LIST_SMS_STATUS_ICON].item_data_type = GUIITEM_DATA_IMAGE_ID;
    
        if (MMISMS_TYPE_MMS == sms_state_ptr->flag.msg_type &&
           (   MMISMS_MT_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type 
            || MMISMS_MT_NOT_NOTIFY == sms_state_ptr->flag.mo_mt_type
            || MMISMS_MT_NEED_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type))
        {//为下载彩信
            if (MMIAPIMMS_IsProcessMMSById(sms_state_ptr->record_id))//如果是正在下载
            {
                item_data.item_content[SMS_LIST_SMS_STATUS_ICON].item_data.image_id = sms_state_ptr->flag.is_locked? IMAGE_MMS_NOW_DOWNLOAD_INBOX_LOCK :IMAGE_MMS_NOW_DOWNLOAD_INBOX;
            }
            else if (MMIAPIMMS_IsMMSInDownloadQueue(sms_state_ptr->record_id))//如果是在下载等待中
            {
                 item_data.item_content[SMS_LIST_SMS_STATUS_ICON].item_data.image_id = sms_state_ptr->flag.is_locked? IMAGE_MMS_WAITING_DOWNLOAD_INBOX_LOCK :IMAGE_MMS_WAITING_DOWNLOAD_INBOX;
            }
            else//正常未下载和下载失败彩信通知
            {
                item_data.item_content[SMS_LIST_SMS_STATUS_ICON].item_data.image_id = MMISMS_GetIconByStatus(sms_state_ptr);
            }
        }    
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        else if(MMISMS_TYPE_MAIL == sms_state_ptr->flag.msg_type)
        {
            item_data.item_content[SMS_LIST_SMS_STATUS_ICON].item_data.image_id = MMISMS_GetIconByStatus(sms_state_ptr);
        }
#endif
        else
        {
            item_data.item_content[SMS_LIST_SMS_STATUS_ICON].item_data.image_id = MMISMS_GetIconByStatus(sms_state_ptr);
        }

#ifndef MMISMS_PORTRAIT_SUPPORT
        if ((MMISMS_MO_DRAFT != sms_state_ptr->flag.mo_mt_type
            || MN_SMS_STORAGE_SIM == sms_state_ptr->flag.storage)
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            && MMISMS_TYPE_SMS == sms_state_ptr->flag.msg_type
#endif
            )
        {
#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
            item_data.item_content[SMS_LIST_SMS_STATUS_ICON].icon_id = GetSmallIconByStatus(sms_state_ptr);
#else
            item_data.item_content[SMS_LIST_SMS_STATUS_ICON].icon_id = GetSimSmallIcon(sms_state_ptr->flag.dual_sys);
#endif
        }
#endif
    }

    list_index++;

#ifdef MMISMS_PORTRAIT_SUPPORT
#ifdef MMI_SMS_CHAT_SUPPORT    
    if(MMISMS_CHATBOX_LISTBOX_CTRL_ID != ctrl_id)
#endif
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        item_data.item_content[SMS_LIST_SMS_SIM_ICON].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        if (MMISMS_MO_DRAFT != sms_state_ptr->flag.mo_mt_type)
        {
            if (sms_state_ptr->flag.dual_sys<MMI_DUAL_SYS_MAX )
            {
                item_data.item_content[SMS_LIST_SMS_SIM_ICON].item_data.image_id = MMIAPISET_GetSimIcon(sms_state_ptr->flag.dual_sys); 
            }
            else
            {
                item_data.item_content[SMS_LIST_SMS_SIM_ICON].item_data.image_id = (MMI_IMAGE_ID_T)0;
            }
        }
        else
        {
            if (MN_SMS_STORAGE_SIM == sms_state_ptr->flag.storage)
            {
                if (sms_state_ptr->flag.dual_sys<MMI_DUAL_SYS_MAX )
                {
                    item_data.item_content[SMS_LIST_SMS_SIM_ICON].item_data.image_id = MMIAPISET_GetSimIcon(sms_state_ptr->flag.dual_sys); 
                }
                else
                {
                    item_data.item_content[SMS_LIST_SMS_SIM_ICON].item_data.image_id = (MMI_IMAGE_ID_T)0;
                }
            }
        }
        list_index++;
#endif
    }
#endif

#ifdef MAINLCD_SIZE_128X64 //@CR229251 2011.02.23
  if( (ctrl_id == MMISMS_MTBOX_LISTBOX_CTRL_ID) ||
      (ctrl_id == MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID) ) 
  {
    // set value to item list
    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = name.wstr_len;
    
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = name.wstr_ptr;

    list_index++;
  }
  else
  {
    // set value to item list
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = subject.wstr_len;

    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;

    list_index++;
    
  }
#else
    // set value to item list
#ifdef MMI_SMS_CHAT_SUPPORT
    if(MMISMS_CHATBOX_LISTBOX_CTRL_ID == ctrl_id)
    {
        item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[list_index].item_data.text_buffer.wstr_len = name.wstr_len;
        
        item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = name.wstr_ptr;

        list_index++;
        
        // set value to item list    
        item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[list_index].item_data.text_buffer.wstr_len = time.wstr_len;

        item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = time.wstr_ptr;

        list_index++;

        // set value to item list
        item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[list_index].item_data.text_buffer.wstr_len = subject.wstr_len;

        item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;
        
        list_index++;
    }
    else
#endif		
    {
        item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
#ifdef MMI_MMS_LIST_DISPLAY_MULTI_PEOPLE
        if (MMISMS_TYPE_MMS == sms_state_ptr->flag.msg_type)
        {
            MMI_STRING_T    sender_info = {0};
            wchar           sender_info_wstr[MMISMS_NUMBER_MAX_LEN + 2] = {0};
            wchar           sender_array[MMISMS_SENDER_DISPLAY_MAX][MMISMS_NUMBER_MAX_LEN + 1] = {0};
            uint16          num = 0;
            wchar           *wchar_ptr = PNULL;
            uint16          sender_len = 0;

            num = MMIAPIMMS_GetSendNumberArray(sms_state_ptr->record_id, sender_array, sizeof(sender_array)/sizeof(sender_array[0]));

            sender_info.wstr_ptr = sender_info_wstr;
            wchar_ptr = sender_wstr;

            if (num > 0 && num <= MMISMS_SENDER_DISPLAY_MAX)
            {
                uint8     temp[MMISMS_NUMBER_MAX_LEN * 3 + 1] = {0};
                int32     i = 0;
                uint16    len = 0;

                for (i = 0; i < num; i++)
                {
                    SCI_MEMSET(temp, 0, sizeof(temp));
                    SCI_MEMSET(sender_info_wstr, 0, sizeof(sender_info_wstr));

                    len = MMIAPICOM_Wstrlen(sender_array[i]);

                    GUI_WstrToUTF8(temp, MMISMS_NUMBER_MAX_LEN * 3, sender_array[i], len);

                    if(MMISMS_GetNameByNumberFromPB(temp, &sender_info, MMISMS_NUMBER_MAX_LEN))
                    {
                        if (sender_len + sender_info.wstr_len >= MMISMS_SENDER_DISPLAY_STRING_MAX)
                        {
                            break;
                        }

                        MMIAPICOM_Wstrncpy(wchar_ptr, sender_info.wstr_ptr, sender_info.wstr_len);

                        wchar_ptr += sender_info.wstr_len;
                        *wchar_ptr = ';';
                        wchar_ptr += 1;
                        sender_len += (sender_info.wstr_len + 1);
                    }
                    else
                    {
                        if (sender_len + len >= MMISMS_SENDER_DISPLAY_STRING_MAX)
                        {
                            break;
                        }
                        MMIAPICOM_Wstrncpy(wchar_ptr, sender_array[i], len);

                        wchar_ptr +=len;
                        *wchar_ptr = ';';
                        wchar_ptr += 1;
                        sender_len += (len + 1);
                    }
                }
                item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_len = sender_len;
                item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_ptr = sender_wstr;
            }
            else
            {
                item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_ptr = name.wstr_ptr;
                item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_len = name.wstr_len;
            }
        }
        else
        {
            item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_ptr = name.wstr_ptr;
            item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_len = name.wstr_len;
        }
#else
        item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_len = name.wstr_len;
        
        item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_ptr = name.wstr_ptr;
        
#endif
        // set value to item list    
        item_data.item_content[SMS_LIST_SMS_DATE_ICON].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[SMS_LIST_SMS_DATE_ICON].item_data.text_buffer.wstr_len = time.wstr_len;

        item_data.item_content[SMS_LIST_SMS_DATE_ICON].item_data.text_buffer.wstr_ptr = time.wstr_ptr;


        // set value to item list
        item_data.item_content[SMS_LIST_CONTENT_SIM_ICON].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[SMS_LIST_CONTENT_SIM_ICON].item_data.text_buffer.wstr_len = subject.wstr_len;

        item_data.item_content[SMS_LIST_CONTENT_SIM_ICON].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;
    }
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_PORTRAIT_SUPPORT
    if (MMISMS_CHATBOX_LISTBOX_CTRL_ID == ctrl_id)
    {  
        const GUILIST_ITEM_T *list_item_ptr = PNULL;        
        list_item_ptr = GUILIST_GetItemPtrByIndex(list_handle,index);  //form link head的时候已经查找过未读了，不需再查找,比较下style
        if(list_item_ptr != 0 && GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON == list_item_ptr->item_style)
        {
            item_data.item_content[list_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_ICON;
        }
    }
    list_index++;    
#endif
#endif 
    // set soft_key
    item_data.softkey_id[LEFT_BUTTON]      = STXT_OPTION;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;

    GUILIST_SetItemData( list_handle, &item_data, index );

    if (PNULL != display_data)
    {
        SCI_FREE(display_data);
    }
}
#else
/*****************************************************************************/
//     Description : to form the item for list_box
//    Global resource dependence : nsms1_state_ptr
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void FormListBoxItem(
                           MMISMS_STATE_T    *sms_state_ptr,     //IN:
                           MMI_CTRL_ID_T     ctrl_id,
                           uint16            index
                           )
{
    //GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_STRING_T name = {0}; 
    MMI_STRING_T time = {0};
    uint8 time_string[MMIMESSAGE_DIAPLAY_LEN] = {0};
    MMI_STRING_T subject = {0};
    MMI_TM_T sms_time = {0};
    SCI_DATE_T cur_date = {0};
    uint16 list_index = 0;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle( ctrl_id );
    wchar temp_arr1[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr2[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr3[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
	MMISMS_BOX_TYPE_E cur_box_type = MMISMS_BOX_NONE;
    BOOLEAN ret = FALSE;
    MMI_STRING_T bt_num_name = {0};
#endif

    subject.wstr_ptr = temp_arr1;
    time.wstr_ptr    = temp_arr2;
    name.wstr_ptr    = temp_arr3;
        
    MMISMS_GetNameInfo(sms_state_ptr, &name, MMIMESSAGE_DIAPLAY_LEN);
    
    if ((sms_state_ptr->flag.mo_mt_type == MMISMS_MT_SR_HAVE_READ) || 
        (sms_state_ptr->flag.mo_mt_type ==MMISMS_MT_SR_TO_BE_READ) )
    {
        MMI_GetLabelTextByLang(TXT_SMS_STATUSREPORT, &subject);        
        subject.wstr_len = MIN(MMISMS_PBNAME_MAX_LEN, subject.wstr_len);
    }
    else
    {       
        subject.wstr_len  = MIN((sms_state_ptr->display_content.content_len), MMIMESSAGE_DIAPLAY_LEN);
        
        MMI_WSTRNCPY(
            subject.wstr_ptr,
            MMIMESSAGE_DIAPLAY_LEN,
            sms_state_ptr->display_content.content, 
            subject.wstr_len,
            subject.wstr_len
            );
        //SCI_TRACE_LOW:"subject.length is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3689_112_2_18_2_54_1_630,(uint8*)"d",subject.wstr_len);
    }
    TM_GetSysDate(&cur_date);

    sms_time = MMIAPICOM_Second2Tm(sms_state_ptr->time);

    if ((sms_time.tm_mday == cur_date.mday)
        && (sms_time.tm_mon == cur_date.mon) 
        && (sms_time.tm_year == cur_date.year))
    {
        time.wstr_len = sprintf((char *)time_string, "%d:%02d", sms_time.tm_hour, sms_time.tm_min);
    }
    else
    {
        time.wstr_len = sprintf((char *)time_string, "%d/%d", sms_time.tm_mon, sms_time.tm_mday);
    }

    MMI_STRNTOWSTR(time.wstr_ptr,
                                MMIMESSAGE_DIAPLAY_LEN,
                                (uint8 *)time_string,
                                time.wstr_len,
                                time.wstr_len
                                );

    list_index = 0;
    
#ifdef MMI_SMS_CHAT_SUPPORT    
    if(MMISMS_CHATBOX_LISTBOX_CTRL_ID == ctrl_id)
    {
        item_data.item_content[list_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
    }
    else
#endif        
    {
        item_data.item_content[list_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
    
        if (MMISMS_TYPE_MMS == sms_state_ptr->flag.msg_type &&
           (   MMISMS_MT_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type 
            || MMISMS_MT_NOT_NOTIFY == sms_state_ptr->flag.mo_mt_type
            || MMISMS_MT_NEED_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type))
        {//为下载彩信
            if (MMIAPIMMS_IsProcessMMSById(sms_state_ptr->record_id))//如果是正在下载
            {
                item_data.item_content[list_index].item_data.image_id = sms_state_ptr->flag.is_locked? IMAGE_MMS_NOW_DOWNLOAD_INBOX_LOCK :IMAGE_MMS_NOW_DOWNLOAD_INBOX;
            }
            else if (MMIAPIMMS_IsMMSInDownloadQueue(sms_state_ptr->record_id))//如果是在下载等待中
            {
                 item_data.item_content[list_index].item_data.image_id = sms_state_ptr->flag.is_locked? IMAGE_MMS_WAITING_DOWNLOAD_INBOX_LOCK :IMAGE_MMS_WAITING_DOWNLOAD_INBOX;
            }
            else//正常未下载和下载失败彩信通知
            {
                item_data.item_content[list_index].item_data.image_id = MMISMS_GetIconByStatus(sms_state_ptr);
            }
        }    
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        else if(MMISMS_TYPE_MAIL == sms_state_ptr->flag.msg_type)
        {
            item_data.item_content[list_index].item_data.image_id = MMISMS_GetIconByStatus(sms_state_ptr);
        }
#endif
        else
        {
            item_data.item_content[list_index].item_data.image_id = MMISMS_GetIconByStatus(sms_state_ptr);
        }
    }

    list_index++;

#ifdef MMI_SMS_CHAT_SUPPORT    
    if(MMISMS_CHATBOX_LISTBOX_CTRL_ID != ctrl_id)
#endif
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        if (MMISMS_MO_DRAFT != sms_state_ptr->flag.mo_mt_type)
        {
            if (sms_state_ptr->flag.dual_sys<MMI_DUAL_SYS_MAX )
            {
                item_data.item_content[list_index].item_data.image_id = MMIAPISET_GetSimIcon(sms_state_ptr->flag.dual_sys); 
            }
            else
            {
                item_data.item_content[list_index].item_data.image_id = (MMI_IMAGE_ID_T)0;
            }
        }
        else
        {
            if (MN_SMS_STORAGE_SIM == sms_state_ptr->flag.storage)
            {
                if (sms_state_ptr->flag.dual_sys<MMI_DUAL_SYS_MAX )
                {
                    item_data.item_content[list_index].item_data.image_id = MMIAPISET_GetSimIcon(sms_state_ptr->flag.dual_sys); 
                }
                else
                {
                    item_data.item_content[list_index].item_data.image_id = (MMI_IMAGE_ID_T)0;
                }
            }
        }
        list_index++;
#endif
    }

#ifdef MAINLCD_SIZE_128X64 //@CR229251 2011.02.23
  if( (ctrl_id == MMISMS_MTBOX_LISTBOX_CTRL_ID) ||
      (ctrl_id == MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID) ) 
  {
    // set value to item list
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = name.wstr_len;
    
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = name.wstr_ptr;

    list_index++;
  }
  else
  {
    // set value to item list
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = subject.wstr_len;

    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;

    list_index++;
    
  }
#else
    // set value to item list
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        ret = MMISMS_GetNameInfoByRecordId(sms_state_ptr->record_id, &(bt_num_name.wstr_ptr), (uint32*)(&(bt_num_name.wstr_len)));
	
		if( TRUE == ret)
		{
			item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_len = bt_num_name.wstr_len;
			item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_ptr = bt_num_name.wstr_ptr;
		}
		else
        {
        item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_len = name.wstr_len;
        
        item_data.item_content[SMS_LIST_SMS_PHONE_NAME_ICON].item_data.text_buffer.wstr_ptr = name.wstr_ptr;
        }
#else
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = name.wstr_len;
    
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = name.wstr_ptr;
#endif

    list_index++;
    
    // set value to item list    
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = time.wstr_len;

    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = time.wstr_ptr;

    list_index++;

    // set value to item list
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = subject.wstr_len;

    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;

    list_index++;
#endif

#ifdef MMI_SMS_CHAT_SUPPORT   
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
 
    if (MMISMS_CHATBOX_LISTBOX_CTRL_ID == ctrl_id)
    {
        MMI_SMS_CHAT_STATUS_T chat_status = FindChatSMSNumStatus(index);

        if (chat_status.is_unread_sms || chat_status.is_unread_mms)
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_ICON;
        }
    }
    list_index++;    
#endif 
    // set soft_key
    item_data.softkey_id[LEFT_BUTTON]      = STXT_OPTION;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;

    GUILIST_SetItemData( list_handle, &item_data, index );
}
#endif

/*****************************************************************************/
//     Description : to append the info to list_box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void AppendListItem(
                          wchar             *string_ptr,        //IN:
                          uint8                string_len,            //IN:
                          MMI_CTRL_ID_T        ctrl_id,            //IN:
                          MMI_TEXT_ID_T        left_softkey_id,    //IN:
                          MMI_TEXT_ID_T        mid_softkey_id,    //IN:Randy add new param @10-27 
                          uint16            pos,                //IN:
                          MMISMS_LIST_ITEM_TYPE_E type
                          )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    if (PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AppendListItem string_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_17251_112_2_18_2_54_56_789,(uint8*)"");
        return;
    }    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;

    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
//     MMI_WSTRNCPY( 
//                 item_data.item_content[0].item_data.text_buffer.wstr,
//                 GUILIST_STRING_MAX_NUM,
//                 string_ptr, 
//                 item_data.item_content[0].item_data.text_buffer.wstr_len,
//                 item_data.item_content[0].item_data.text_buffer.wstr_len
//                 );

    switch( type )
    {
    case MMISMS_APPEND_LIST_ITEM_TYPE:
        GUILIST_AppendItem( ctrl_id, &item_t );
        break;
    case MMISMS_REPLACE_LIST_ITEM_TYPE:
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
        break;
    case MMISMS_INSERT_LIST_ITEM_TYPE:
        GUILIST_InsertItem( ctrl_id, &item_t, pos );
        break;
    case MMISMS_SET_LIST_ITEM_TYPE:
        GUILIST_SetItemData(ctrl_id,&item_data, pos);
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//     Description : Set Empty to the list box   
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMS_SetEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,        //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: for different position, the empty string is different.
                             MMI_TEXT_ID_T    left_softkey_id    //IN:
                             )
{
    MMI_STRING_T empty_str = {0};

    // need append empty string to the list box
    // get empty string
    MMI_GetLabelTextByLang(empty_txt_id, &empty_str);
    AppendListItem(
                    empty_str.wstr_ptr,
                    (uint8)empty_str.wstr_len,
                    ctrl_id,
                    left_softkey_id,
                    TXT_NULL,
                    0,
                    MMISMS_SET_LIST_ITEM_TYPE
                    );
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetListItemData(MMISMS_BOX_TYPE_E box_type, 
                                    MMI_CTRL_ID_T     ctrl_id,
                                    uint16            index
                                    )
{
    //GUILIST_ITEM_T item_info = {0};
    //GUILIST_ITEM_DATA_T item_data = {0};
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    MMISMS_STATE_T sms_order_info ={0};

#ifdef MMI_SMS_CHAT_SUPPORT    
    if(MMISMS_BOX_CHAT == box_type)
    {
        current_order_id = MMISMS_GetChatLinkedArrayPtrIdInList(index);
    }
    else
#endif        
    {
        current_order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type ,index); 
    }
    
    if (current_order_id != PNULL)
    {  
        MMISMS_GetSmsInfoFromOrder(current_order_id, &sms_order_info);        
        //SCI_MEMSET(&item_info, 0, sizeof(GUILIST_ITEM_T)); 
		
        FormListBoxItem(            
            &sms_order_info,
            ctrl_id,
            index
            );        

        if (sms_order_info.flag.is_marked)
        {
            GUILIST_SetTextListMarkable(ctrl_id, TRUE);
            GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
        }
    }
    else
    {
#ifndef MMI_PDA_SUPPORT
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        if((MMISMS_BOX_SECURITY == box_type)
#ifdef MMI_SMS_CHAT_SUPPORT 
           || (MMISMS_BOX_CHAT == box_type)
#endif
		  )
        {
            MMISMS_SetEmptyStr2List(ctrl_id, TXT_BLANK, STXT_OPTION);    
        }
        else
#endif            
        {
            MMISMS_SetEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);    
        }       
#endif        
    }
}

/*****************************************************************************/
//  Description : Get Small Icon by sim index
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSimSmallIcon(MN_DUAL_SYS_E dual_sys)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }

    return (MMI_IMAGE_ID_T)(IMAGE_SMS_SIM_CARD1+(uint32)dual_sys);
#else
    return IMAGE_SMS_SIM_CARD;
#endif
}

#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSmallIconByStatus(MMISMS_STATE_T *sms_state_ptr)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;

    if (PNULL == sms_state_ptr)
    {
        return IMAGE_NULL;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    dual_sys = sms_state_ptr->flag.dual_sys;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }

    if (sms_state_ptr->flag.storage == MN_SMS_STORAGE_SIM)
    {
        return (MMI_IMAGE_ID_T)(IMAGE_SMS_SIM_CARD1+(uint32)dual_sys);
    }
    else
    {
        return (MMI_IMAGE_ID_T)(IMAGE_SMS_ME_CARD1+(uint32)dual_sys);
    }
#else
    if (sms_state_ptr->flag.storage == MN_SMS_STORAGE_SIM)
    {
        return IMAGE_SMS_SIM_CARD;
    }
    else
    {
        return IMAGE_NULL;
    }
#endif
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of SMS box window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetSoftkeyAfterMarkOpera(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,BOOLEAN *is_mark_all)
{
    uint16 item_num = 0;
    uint16 sel_item_num = 0;
    sel_item_num = GUILIST_GetSelectedItemNum(ctrl_id);
    item_num = GUILIST_GetTotalItemNum(ctrl_id);

    if(sel_item_num == item_num)
    {
        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_SELECT_ALL, TXT_DELETE,STXT_CANCEL,TRUE);     
        *is_mark_all = TRUE;
    }
    else
    {
        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,TRUE); 
        *is_mark_all = FALSE;
    }
    
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle mark all
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_HandleMarkAll(BOOLEAN is_mark_all,MMI_WIN_ID_T win_id,MMI_CTRL_ID_T cur_ctrl_id,MMISMS_BOX_TYPE_E cur_box_type,uint32 *marked_sms_num)
{
    MMI_TEXT_ID_T left_id = 0;
    
    if(is_mark_all)
    {                              
       left_id = TXT_MARK_ALL;
       MMISMS_CancelMarkAllItem(cur_ctrl_id,cur_box_type,marked_sms_num,FALSE);
    }
    else
    {
       left_id = TXT_CANCEL_SELECT_ALL;
       MMISMS_MarkAllItem(cur_ctrl_id,cur_box_type,marked_sms_num,FALSE);
    }
    
    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, left_id, TXT_DELETE,STXT_CANCEL,TRUE);     
    if (MMK_IsFocusWin(win_id))
    {
      MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
    }
    return !is_mark_all;
}


/*****************************************************************************/
//     Description : 
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_TYPE_E box_type, GUIFORM_CHILD_DISPLAY_E display_type)
{
    MMI_CTRL_ID_T parent_ctrl_id = 0;

    switch (box_type)
    {
    case MMISMS_BOX_MT:
        parent_ctrl_id = MMISMS_MTBOX_FORM_CTRL_ID;
        break;

    case MMISMS_BOX_SENDSUCC:
        parent_ctrl_id = MMISMS_SENDSUCCBOX_FORM_CTRL_ID;
        break;

    case MMISMS_BOX_SENDFAIL:
        parent_ctrl_id = MMISMS_SENDFAILBOX_FORM_CTRL_ID;
        break;

    case MMISMS_BOX_NOSEND:
        parent_ctrl_id = MMISMS_DRAFTBOX_FORM_CTRL_ID;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        parent_ctrl_id = MMISMS_SECURITYBOX_FORM_CTRL_ID;
        break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT    
    case MMISMS_BOX_CHAT:
        parent_ctrl_id = MMISMS_CHATBOX_FORM_CTRL_ID;
        break;
#endif

    default:
        break;
    }

    GUIFORM_SetChildDisplay(parent_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, display_type);
}
#endif
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleDialMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_MAX;
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;
    if(PNULL != cur_order_id)
    {
        msg_type = cur_order_id->flag.msg_type;
    }


    if((cur_order_id != PNULL && 
        ((MMISMS_TYPE_SMS == msg_type 
            && MN_NUM_TYPE_ALPHANUMERIC == MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton)
            )||(MMISMS_TYPE_WAP_PUSH == msg_type || MMISMS_TYPE_WAP_OTA == msg_type 
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
			|| MMISMS_TYPE_MAIL == msg_type
#endif
			)||((0 == menu_enable_ptr->marked_num && (MMISMS_TYPE_WAP_PUSH == msg_type || MMISMS_TYPE_WAP_OTA == msg_type)) 
                || (0 < menu_enable_ptr->marked_num )))) || (PNULL == cur_order_id)
        )
    {		 
#ifdef MMI_SMS_CHAT_SUPPORT    
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, MENU_SMS_CHAT_MAIN_OPT, MMISMS_MENU_CALL_ITEM_ID, TRUE); 
#endif
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, MENU_SMS_MTLIST_OPT, MMISMS_MENU_CALL_ITEM_ID, TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_DIAL_ITEM_ID, TRUE);
#if defined(VT_SUPPORT)        
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_VIDEOCALL_ITEM_ID, TRUE);
#endif

#if defined(MMI_IP_CALL_SUPPORT)        
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_IPDIAL_ITEM_ID, TRUE);
#endif
    }
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleAnswerMsgMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_MAX;
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;	

    if(PNULL != cur_order_id)
    {
       msg_type = cur_order_id->flag.msg_type;
    }

    if((cur_order_id != PNULL && 
        ((MMISMS_TYPE_SMS == msg_type 
            && MN_NUM_TYPE_ALPHANUMERIC == MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton)
            )||(menu_enable_ptr->marked_num > 0 && !MMISMS_IsHaveValidMsgToReply())
                ||(0 == menu_enable_ptr->marked_num && (MMISMS_TYPE_WAP_PUSH == msg_type || MMISMS_TYPE_WAP_OTA == msg_type)) 
                ))
         ||(PNULL == cur_order_id)        
        )
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, MENU_SMS_MTLIST_OPT, MMISMS_MENU_ANSWER_ITEM_ID, TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, MENU_SMS_MTLIST_UNLOCK_OPT, MMISMS_MENU_ANSWER_ITEM_ID, TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_ANSWER_SMS_ITEM_ID, TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_ANSWER_MMS_ITEM_ID, TRUE);
    }
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_ANSWER_MMS_ITEM_ID, TRUE);
    }
}
/*****************************************************************************/
//     Description : handle common menu enable
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleCommMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;

    if(PNULL == cur_order_id)
    {
        return;
    }

    if( menu_enable_ptr->marked_num > 0)
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id,MMISMS_MENU_EDIT_ITEM_ID,TRUE);
    }

    if(MN_SMS_STORAGE_SIM == menu_enable_ptr->cur_order_id->flag.storage)
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_LOCK_ITEM_ID, TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_UNLOCK_ITEM_ID, TRUE);
    }
}
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleDeleteMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_MAX;
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;
    uint32 avail_del_num = 0;    
	BOOLEAN is_process_mms = FALSE;
    avail_del_num = MMISMS_GetSmsBoxDeleteAvailNum(menu_enable_ptr->box_type);
    if(cur_order_id != PNULL)
    {
        msg_type = cur_order_id->flag.msg_type;
		is_process_mms = MMIAPIMMS_IsProcessMMSById(cur_order_id->record_id);
    }
    
    if(((MMISMS_TYPE_WAP_PUSH == msg_type 
            || MMISMS_TYPE_WAP_OTA == msg_type
            || MMISMS_TYPE_MMS == msg_type
            || MMISMS_TYPE_MMS_OTA == msg_type
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT            
            || MMISMS_TYPE_MAIL == msg_type
#endif            
        ) && MMIAPIUDISK_UdiskIsRun())
        ||((0 == avail_del_num) && menu_enable_ptr->marked_num > 0)
        ||(cur_order_id != PNULL && (0 == menu_enable_ptr->marked_num) && cur_order_id->flag.is_locked)
        ||(cur_order_id != PNULL && (0 == menu_enable_ptr->marked_num) && is_process_mms)

        #ifdef PUSH_EMAIL_3_IN_1_SUPPORT 
        || (cur_order_id != PNULL && mail_GetMailSendingFlag((uint32)mail_getMsgIdByRecordId(cur_order_id->record_id)))
        #endif
        )
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_DEL_ITEM_ID, TRUE); 
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_DELBOX_ALL_ITEM_ID, TRUE);        
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id, menu_enable_ptr->group_id, MMISMS_MENU_DELBOX_SAMENUM_ITEM_ID, TRUE);  
    }

    if (menu_enable_ptr->marked_num > 0)
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_DELBOX_SAMENUM_ITEM_ID,TRUE);
    }
}
           
#ifdef MMI_SMS_MOVE_SUPPORT 
/*****************************************************************************/
//    Description : handle move menu enable
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleMoveMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_MAX;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;
    uint16 sms_max_num,sms_used_num;
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;

    uint16 sim_ok_num = 0;
    if(cur_order_id != PNULL)
    {
        msg_type = cur_order_id->flag.msg_type;
        folder_type = cur_order_id->flag.folder_type;
    }
       
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    if( MMISMS_FOLDER_SECURITY == folder_type )
    {        
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_SECURITY_ITEM_ID,TRUE);     
    }
    else
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_PREVIEW_ITEM_ID,TRUE);               
    }
#endif  

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    if(((MMISMS_TYPE_WAP_PUSH == msg_type 
            || MMISMS_TYPE_WAP_OTA == msg_type
            || MMISMS_TYPE_MMS == msg_type
            || MMISMS_TYPE_MMS_OTA == msg_type
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT            
            || MMISMS_TYPE_MAIL == msg_type
#endif            
        ) && MMIAPIUDISK_UdiskIsRun()))
    {
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_SECURITY_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_PREVIEW_ITEM_ID,TRUE);  
    }
#endif

    MMISMS_GetSMSMemory(MN_DUAL_SYS_1, TRUE,&sms_max_num,&sms_used_num);
    sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(PNULL, 0);
    
    if(((0 == MMISMS_GetMarkedStorageMsgNum(menu_enable_ptr->box_type,MN_SMS_STORAGE_ME))
       && (sms_max_num == sms_used_num)) || (sim_ok_num < 1))
    {        
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID,TRUE);
    }

    if(menu_enable_ptr->marked_num > 0) 
    {
        if(0 == MMISMS_GetMarkedStorageMsgNum(menu_enable_ptr->box_type,MN_SMS_STORAGE_ME))
        {
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID,TRUE);  
        }
        if(0 == MMISMS_GetMarkedStorageMsgNum(menu_enable_ptr->box_type,MN_SMS_STORAGE_SIM))
        {     
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_MOVETOME_ITEM_ID,TRUE); 
        }
    }
    else
    {    
        if(PNULL == cur_order_id)
        {
            return;
        }
//         if(MN_SMS_STORAGE_SIM == cur_order_id->flag.storage)
//         {
//             MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID,TRUE);
//         }
        else if(MN_SMS_STORAGE_ME == cur_order_id->flag.storage)
        {
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_MOVETOME_ITEM_ID,TRUE);
        }
        if((MMISMS_TYPE_WAP_PUSH == msg_type 
                || MMISMS_TYPE_WAP_OTA == msg_type
                || MMISMS_TYPE_MMS == msg_type
                || MMISMS_TYPE_MMS_OTA == msg_type
    #ifdef PUSH_EMAIL_3_IN_1_SUPPORT            
                || MMISMS_TYPE_MAIL == msg_type
    #endif
         ) || (cur_order_id->flag.is_locked)
        )
        {
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID,TRUE);
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_MOVETOME_ITEM_ID,TRUE);
        }
    }
}
#endif

#ifdef MMI_SMS_COPY_SUPPORT 
/*****************************************************************************/
//    Description : handle mms chat item opt
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleCopyMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_MAX;
    uint16 sms_max_num,sms_used_num;
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;
    uint16 sim_ok_num = 0;
    if(cur_order_id != PNULL)
    {
        msg_type = cur_order_id->flag.msg_type;
    }
        //标记的全为SIM消息且nv中无空间，"移动到安全信箱"灰化
    MMISMS_GetSMSMemory(MN_DUAL_SYS_1, TRUE,&sms_max_num,&sms_used_num);
    sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(PNULL, 0);
    
    if(sms_max_num == sms_used_num) //NV无空间拷贝,SIM卡需要选卡，暂不考虑
    {
#ifdef MMI_SMS_COPY_SUPPORT
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOME_ITEM_ID,TRUE);
#endif
    }

    if(sim_ok_num <= 1 && 0 < MMISMS_GetMarkedStorageMsgNum(menu_enable_ptr->box_type,MN_SMS_STORAGE_SIM) && 0 == MMISMS_GetMarkedStorageMsgNum(menu_enable_ptr->box_type,MN_SMS_STORAGE_ME))
    {
#ifdef MMI_SMS_COPY_SUPPORT
        MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID,TRUE);
#endif
    }

    if(menu_enable_ptr->marked_num > 0) 
    {
        if(0 == MMISMS_GetMarkedStorageMsgNum(menu_enable_ptr->box_type,MN_SMS_STORAGE_ME))
        {
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID,TRUE);  
        }
        if(0 == MMISMS_GetMarkedStorageMsgNum(menu_enable_ptr->box_type,MN_SMS_STORAGE_SIM))
        {     
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOME_ITEM_ID,TRUE); 
        }  
    }
    else
    {
        if(cur_order_id != PNULL && MN_SMS_STORAGE_ME == cur_order_id->flag.storage) //ME中不拷贝
        {
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOME_ITEM_ID,TRUE);
        }
//         if (PNULL != cur_order_id && MN_SMS_STORAGE_SIM == cur_order_id->flag.storage)
//         {
//             MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID,TRUE);
//         }
        if(msg_type != MMISMS_TYPE_SMS) //只拷贝短信
        {                             
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOME_ITEM_ID,TRUE);
            MMIAPICOM_EnableGrayed(menu_enable_ptr->win_id,menu_enable_ptr->group_id,MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID,TRUE);
        }
    }
}
#endif


/*****************************************************************************/
//    Description : handle mms chat item opt
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSendMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_MAX;
    MMI_WIN_ID_T          win_id = menu_enable_ptr->win_id;
    uint32 group_id = menu_enable_ptr->group_id;
    BOOLEAN is_process_mms = FALSE;
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;
    if(cur_order_id)
    {
        is_process_mms = MMIAPIMMS_IsProcessMMSById(cur_order_id->record_id);
    }

    if(cur_order_id != PNULL)
    {
        msg_type = cur_order_id->flag.msg_type;
    }
    if(cur_order_id != PNULL && (MMISMS_SENDSTATUS_WAITING == cur_order_id->backsend_status || MMISMS_SENDSTATUS_SENDING == cur_order_id->backsend_status))
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_SIM1_SEND_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_DEL_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_DELBOX_ALL_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARK_ITEM_ID,TRUE);     

    }
    if(menu_enable_ptr->marked_num > 0)
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_SIM1_SEND_ITEM_ID,TRUE);
    }
    else
    {
        if((
                MMISMS_TYPE_MMS == msg_type
    #ifdef PUSH_EMAIL_3_IN_1_SUPPORT            
                || MMISMS_TYPE_MAIL == msg_type
    #endif            
            ) && MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_SIM1_SEND_ITEM_ID,TRUE);
        }
    
        if(MMISMS_TYPE_MMS == msg_type && is_process_mms)
        {
            MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_SIM1_SEND_ITEM_ID,TRUE);
        }
    }
}

/*****************************************************************************/
//    Description : handle mms chat item opt
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleForwardMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_MAX;
    MMI_WIN_ID_T          win_id = menu_enable_ptr->win_id;
    uint32 group_id = menu_enable_ptr->group_id;
    MMISMS_ORDER_ID_T cur_order_id = menu_enable_ptr->cur_order_id;

    if(cur_order_id != PNULL)
    {
        msg_type = cur_order_id->flag.msg_type;
    }
    if(menu_enable_ptr->marked_num > 0)
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_FORWARD_ITEM_ID,TRUE);
    }
    else
    {
        if((
                MMISMS_TYPE_MMS == msg_type
    #ifdef PUSH_EMAIL_3_IN_1_SUPPORT            
                || MMISMS_TYPE_MAIL == msg_type
    #endif            
            ) && MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_FORWARD_ITEM_ID,TRUE);
        }
    
        if(MMISMS_TYPE_WAP_PUSH == msg_type 
                    || MMISMS_TYPE_WAP_OTA == msg_type
                    || MMISMS_TYPE_MMS_OTA == msg_type)
        {
            MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_FORWARD_ITEM_ID,TRUE);
        }
    }
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//    Description : handle mark menu enable gray
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleChatMarkMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{  
    MMI_WIN_ID_T          win_id = menu_enable_ptr->win_id;
    uint32 group_id = menu_enable_ptr->group_id;
    if(0 == menu_enable_ptr->marked_num)
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CHAT_CANCEL_MARK_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CANCLE_MARKALL_ITEM_ID,TRUE);
    }
    if(menu_enable_ptr->cur_order_id != PNULL && menu_enable_ptr->cur_order_id->flag.is_marked)
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CHAT_OPT_MARK_ITEM_ID,TRUE);
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CHAT_CANCEL_MARK_ITEM_ID,TRUE);
    }

    if(menu_enable_ptr->marked_num ==  MMISMS_GetBoxTotalNumberFromOrder(menu_enable_ptr->box_type)) //已经mark全部
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARKALL_ITEM_ID,TRUE);
    }    
}
#endif

/*****************************************************************************/
//    Description : handle mark menu enable gray
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleMarkMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{  
    MMI_WIN_ID_T          win_id = menu_enable_ptr->win_id;
    uint32 group_id = menu_enable_ptr->group_id;
    uint i = 0;
    if(0 == menu_enable_ptr->marked_num)
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CANCEL_MARK_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CANCLE_MARKALL_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CANCEL_MARK_NUM_ITEM_ID,TRUE);
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        if(MMIAPIPHONE_IsSimCardOk(i))
        {
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)        
            if(menu_enable_ptr->box_type != MMISMS_BOX_SECURITY)
#endif                
            {
                if(MMISMS_BOX_MT == menu_enable_ptr->box_type)
                {
//#if 0 // for 133698
                    if(PNULL == MMISMS_FindValidMTSIMSMS(i) || (menu_enable_ptr->marked_num ==  MMISMS_GetBoxTotalNumberFromOrder(menu_enable_ptr->box_type)))
//#endif
                    {
                        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARK_SIM1_SMS_ITEM_ID+((MMI_MENU_ID_T)i -MN_DUAL_SYS_1),TRUE);
                    }
                }
                else
                {
//#if 0 // for 133698
                    if(PNULL == MMISMS_FindValidMOSIMSMS(menu_enable_ptr->box_type,i) || (menu_enable_ptr->marked_num ==  MMISMS_GetBoxTotalNumberFromOrder(menu_enable_ptr->box_type)))
//#endif
                    {
                        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARK_SIM1_SMS_ITEM_ID+((MMI_MENU_ID_T)i -MN_DUAL_SYS_1),TRUE);
                    } 
                }
            }
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARK_SIM1_SMS_ITEM_ID+((MMI_MENU_ID_T)i -MN_DUAL_SYS_1),TRUE);
        }
    }
#else    
    if(!MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1))
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARK_SIM_SMS_ITEM_ID,TRUE);
    }
#endif 
    if(menu_enable_ptr->cur_order_id != PNULL && menu_enable_ptr->cur_order_id->flag.is_marked)
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_OPT_MARK_ITEM_ID,TRUE);
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_CANCEL_MARK_ITEM_ID,TRUE);
    }

    if(menu_enable_ptr->marked_num ==  MMISMS_GetBoxTotalNumberFromOrder(menu_enable_ptr->box_type)) //已经mark全部
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARKALL_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARK_NUM_ITEM_ID,TRUE);
#ifdef MMI_MULTI_SIM_SYS_SINGLE          
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_MARK_SIM_SMS_ITEM_ID,TRUE);
#endif
    }    
}

/*****************************************************************************/
//    Description : handle mark menu enable gray
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleAddPBMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMI_WIN_ID_T          win_id = menu_enable_ptr->win_id;
    uint32 group_id = menu_enable_ptr->group_id;
    if( menu_enable_ptr->marked_num > 0
	    ||( menu_enable_ptr->cur_order_id != PNULL && 
        (
            MMISMS_TYPE_WAP_PUSH == menu_enable_ptr->cur_order_id->flag.msg_type
            ||MMISMS_TYPE_MMS_OTA == menu_enable_ptr->cur_order_id->flag.msg_type
            || MMISMS_TYPE_WAP_OTA == menu_enable_ptr->cur_order_id->flag.msg_type
        )))
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_ADDNUMTOPB_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID,TRUE);
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT         
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID,TRUE);
#endif
#ifdef MMI_BLACKLIST_SUPPORT
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID,TRUE);
#endif
    }

}

/*****************************************************************************/
//    Description : handle mark menu enable gray
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSortMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr)
{
    MMI_WIN_ID_T          win_id = menu_enable_ptr->win_id;
    uint32 group_id = menu_enable_ptr->group_id;
    if( menu_enable_ptr->marked_num > 0)
    {
        MMIAPICOM_EnableGrayed(win_id, group_id,MMISMS_MENU_SORT_ITEM_ID,TRUE);
    }
}

#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC  void MMISMS_SetPushMsgListItemData(
                                    MMI_CTRL_ID_T     ctrl_id,
                                    uint16            index
                                    )
{
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    MMISMS_STATE_T sms_order_info ={0};
      
    current_order_id = MMISMS_GetPushMsgLinkedArrayPtrId(index);  //根据index，进入相应的wap push
    
    if (current_order_id != PNULL)
    {  
        MMISMS_GetSmsInfoFromOrder(current_order_id, &sms_order_info);        
        //SCI_MEMSET(&item_info, 0, sizeof(GUILIST_ITEM_T)); 
		
        FormListBoxItem(            
            &sms_order_info,
            ctrl_id,
            index
            );        

        if (sms_order_info.flag.is_marked)
        {
            GUILIST_SetTextListMarkable(ctrl_id, TRUE);
            GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
        }
    }
}
#endif


/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_RED:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        // only for cf141306 temp solution
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
#ifdef MMI_SMS_CHAT_SUPPORT 
        if(MMISMS_READ_CHAT_NUM_SMS == MMISMS_GetCurrentOperStatus())
        {
            MMISMS_InsertUnhandledSmsToOrder();
//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)			
            MMISMS_HandleOrderChangeCallBack();
#endif
            //SCI_TRACE_LOW:"MMISMS_HandleWaitWinMsg:insert unhandle sms"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6455_112_2_18_2_54_33_743,(uint8*)"");
        }
#endif
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}



//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleBtWaitWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED:
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;

    default:
        recode = MMISMS_HandleBtWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : set anim in chat list
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DrawListBoxItemWithIconData(
    uint16           item_index,     //in:item索引
    uint16           content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T    ctrl_id,         //in:control id
    MMISMS_BOX_TYPE_E box_type
)
{
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
    uint32 custom_photo_name_len = 0;
    uint32 photo_file_size = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};
    BOOLEAN result = FALSE;
    MMISMS_ORDER_ID_T current_order_id = PNULL;

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    if (MMISMS_BOX_MT == box_type)
    {
        current_order_id = MMISMS_GetPushMsgLinkedArrayPtrId(item_index);
    }
    else
    {
        current_order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type, item_index);
    }
#else
    current_order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type, item_index);
#endif

    if (PNULL == current_order_id)
    {
        return result;
    }

    if (current_order_id->flag.msg_type == MMISMS_TYPE_SMS)
    {
        pb_num.number_len = current_order_id->number.bcd_number.number_len;
        pb_num.npi_ton = current_order_id->number.bcd_number.npi_ton;
        if (pb_num.number_len > 0)
        {
            SCI_MEMCPY(&pb_num.number, current_order_id->number.bcd_number.number, pb_num.number_len);
        }
    }
    else
    {
        MMISMS_MMSNumberToBcd((char*)current_order_id->number.sender, pb_num.number, &pb_num.number_len);
    }

    if (MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num, custom_photo_name, &custom_photo_name_len, &photo_file_size))
    {
        item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
        item_content.item_data.anim_path_ptr = &anim_path;
        anim_path.full_path_wstr_ptr = custom_photo_name;
        anim_path.full_path_wstr_len = custom_photo_name_len;
        result = GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);
    }
    else
    {
        GUILIST_SetDefaultIcon(ctrl_id, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
    }

    return result;
}
#endif
/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenDelteQueryWin(MMISMS_DELETE_TYPE_E delete_type,
                                     BOOLEAN is_delete_push,
                                     MMISMS_ORDER_ID_T order_id,
                                     MMISMS_BOX_TYPE_E box_type,
                                     MMIPUB_HANDLE_FUNC handle_func)
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 delete_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T tmp_prompt = {0};

    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2+1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    uint32 delete_sms_num = 0;

    SCI_TRACE_LOW("[MMISMS] MMISMS_OpenDelteQueryWin delete_type = %d, is_delete_push = %d, order_id = %d, box_type = %d", delete_type, is_delete_push, order_id, box_type);

    if(MMISMS_DELETE_PART == delete_type)
    {
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        if(is_delete_push)
        {
            delete_sms_num = MMISMS_GetPushMsgDeleteAvailNum();
        }
        else
        {
            delete_sms_num = MMISMS_GetSmsBoxDeleteAvailNum(box_type);
        }
#else
        delete_sms_num = MMISMS_GetSmsBoxDeleteAvailNum(box_type);
#endif

        MMI_GetLabelTextByLang(TXT_DELETE, &prompt_str1);
        
        disp_num_len = sprintf((char *)delete_msg_num, "%ld ", delete_sms_num);

        MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)delete_msg_num, disp_num_len, disp_num_len);

        if (delete_sms_num > 1)
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEMS, &tmp_prompt);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEM, &tmp_prompt);
        }

        // the total string wstr_len
        disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
        
        // allocate memory
        prompt_str2.wstr_ptr = SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
        SCI_MEMSET(prompt_str2.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        // copy the num
        MMI_WSTRNCPY(prompt_str2.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);

        prompt_str2.wstr_len = disp_num_len;
        
        // copy the prompt string
        MMI_WSTRNCPY(
            &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]),
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr, 
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
            );
        prompt_str2.wstr_len += tmp_prompt.wstr_len;

        if (0 < delete_sms_num)
        {
            MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, handle_func);
        }
        else
        {
            MMIPUB_OpenAlertWinByTextPtr(PNULL, PNULL, &prompt_str2, IMAGE_PUBWIN_WARNING, &query_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        if (PNULL != prompt_str2.wstr_ptr)
        {
            SCI_FREE(prompt_str2.wstr_ptr);
            prompt_str2.wstr_ptr = PNULL;
        }
    }
    else if(MMISMS_DELETE_ALL == delete_type)
    {
        MMI_GetLabelTextByLang(TXT_SMS_DELALL_MESSAGE, &prompt_str1);
        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, handle_func);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SMS_DELETE_MESSAGE, &prompt_str1);
        if(PNULL == order_id)
        {
        SCI_TRACE_LOW("mmisms order_id == PNULL when delete");
        return;
        }
        if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
        {
            prompt_str2.wstr_len = order_id->display_content.content_len;
            prompt_str2.wstr_ptr = order_id->display_content.content;
        }
        else
        {
            MMI_WSTRNCPY(str,
                         MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                         order_id->display_content.content,
                         MMISMS_QUERY_STRING_MAX_NUM,
                         MMISMS_QUERY_STRING_MAX_NUM);

            MMI_STRNTOWSTR( str + MMISMS_QUERY_STRING_MAX_NUM,
                            MMISMS_ELLIPSE_STR_LEN,
                            (const uint8 *)ellipse_str,
                            MMISMS_ELLIPSE_STR_LEN,
                            MMISMS_ELLIPSE_STR_LEN);
            prompt_str2.wstr_ptr = str;
            prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
        }

        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, handle_func);

    }
}
#ifdef MMI_SMS_COPY_SUPPORT
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :set copy menu title
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCopyMenuTitle(void)
{
    MMI_TEXT_ID_T      text_id[5] = 
                            {
                                TXT_INBOX,
                                TXT_SMS_SENTBOX,  
                                TXT_COMM_OUTBOX,  
                                TXT_COMM_DRAFT,
                                #ifdef MMI_SMS_SECURITYBOX_SUPPORT
                                TXT_SMS_SECURITY
                                #endif
                            };
    
    uint16 index = 0;
    GUIMENU_TITLE_INFO_T title_info = {0};
    MMI_STRING_T title_text = {0};
	index = MMISMS_GetCurBoxType() - 1;     
    MMI_GetLabelTextByLang(text_id[index] , &title_text);
    title_info.group_id = MENU_SMS_COPY_OPT;
    title_info.title_ptr = &title_text;    
    title_info.is_static = TRUE;
    GUIMENU_SetMenuTitleEx(MMISMS_MARK_MENU_OPT_MENU_CTRL_ID, &title_info);
}
#endif

/*****************************************************************************/
//  Description : open the SMS move query window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenCopyQueryWinMsg(uint32 menu_id,MMISMS_BOX_TYPE_E box_type,uint32 marked_sms_num,MMISMS_ORDER_ID_T  cur_order_id,MMIPUB_HANDLE_FUNC handle_func)
{   
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 delete_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T tmp_prompt = {0};
    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    uint32 copy_sms_num = 0;
    uint32 copy_mark_num = 0;

    if (0 == marked_sms_num)
    {
        MMISMS_SetCurOrderMarked(cur_order_id,TRUE);
    }  
        
    if(MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID == menu_id)
    {
        copy_sms_num = MMISMS_GetMarkedStorageMsgNumByDualsys(box_type,MN_SMS_STORAGE_SIM,MMISMS_GetCurSaveDualSys());
        copy_mark_num = MMISMS_GetMarkedStorageMsgNum(box_type,MN_SMS_STORAGE_ALL);

        copy_sms_num = copy_mark_num - copy_sms_num;
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_SIM);
    }
    else if(MMISMS_MENU_BOX_COPYTOME_ITEM_ID == menu_id)
    {
        copy_sms_num = MMISMS_GetMarkedStorageMsgNum(box_type,MN_SMS_STORAGE_SIM);
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_ME);
    }

    if (marked_sms_num > 0)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_COPY, &prompt_str1);
        
        disp_num_len = sprintf((char *)delete_msg_num, "%ld ", copy_sms_num);

        MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)delete_msg_num, disp_num_len, disp_num_len);

        if (copy_sms_num > 1)
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEMS, &tmp_prompt);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEM, &tmp_prompt);
        }

        // the total string wstr_len
        disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
        
        // allocate memory
        prompt_str2.wstr_ptr = SCI_ALLOCAZ((disp_str_len+1)*sizeof(wchar));
        if(PNULL == prompt_str2.wstr_ptr)
        {
            return;
        }
        //SCI_MEMSET(prompt_str2.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        // copy the num
        MMI_WSTRNCPY(prompt_str2.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);

        prompt_str2.wstr_len = disp_num_len;
        
        // copy the prompt string
        MMI_WSTRNCPY(
            &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]),
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr, 
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
            );
        prompt_str2.wstr_len += tmp_prompt.wstr_len;

        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, handle_func);
        if (PNULL != prompt_str2.wstr_ptr)
        {
            SCI_FREE(prompt_str2.wstr_ptr);
            prompt_str2.wstr_ptr = PNULL;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_COPY, &prompt_str1);
        order_id = cur_order_id;
        if(PNULL == order_id)
        {
            SCI_TRACE_LOW("mmisms cur order pnull");
            return;
        }
        if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
        {
            prompt_str2.wstr_len = order_id->display_content.content_len;
            prompt_str2.wstr_ptr = order_id->display_content.content;
        }
        else
        {
            MMI_WSTRNCPY(str,
                         MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                         order_id->display_content.content,
                         MMISMS_QUERY_STRING_MAX_NUM,
                         MMISMS_QUERY_STRING_MAX_NUM);

            MMI_STRNTOWSTR(str + MMISMS_QUERY_STRING_MAX_NUM,
                           MMISMS_ELLIPSE_STR_LEN,
                           (const uint8 *)ellipse_str,
                           MMISMS_ELLIPSE_STR_LEN,
                           MMISMS_ELLIPSE_STR_LEN);
            prompt_str2.wstr_ptr = str;
            prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
        }
       
        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, handle_func);
    }    
}
#endif

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : open the SMS move query window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenMoveQueryWinMsg(uint32 menu_id,MMISMS_BOX_TYPE_E box_type,uint32 marked_sms_num,MMISMS_ORDER_ID_T cur_order_id,MMIPUB_HANDLE_FUNC handle_func)
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 delete_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T tmp_prompt = {0};
    MMISMS_ORDER_ID_T order_id = cur_order_id;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    uint32 move_sms_num = 0;
    uint32 move_mark_num = 0;

    if (0 == marked_sms_num)
    {
        MMISMS_SetCurOrderMarked(cur_order_id,TRUE);
    }  
        
    if(MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID == menu_id)
    {
        move_sms_num = MMISMS_GetMarkedStorageMsgNumByDualsys(box_type,MN_SMS_STORAGE_SIM,MMISMS_GetCurSaveDualSys());
        move_mark_num = MMISMS_GetMarkedStorageMsgNum(box_type,MN_SMS_STORAGE_ALL);
        move_sms_num = move_mark_num - move_sms_num;
        MMISMS_SetMoveOperStatus(MMISMS_MOVE_SMS_TO_SIM);
    }
    else if(MMISMS_MENU_BOX_MOVETOME_ITEM_ID == menu_id)
    {
        move_sms_num = MMISMS_GetMarkedStorageMsgNum(box_type,MN_SMS_STORAGE_SIM);
        MMISMS_SetMoveOperStatus(MMISMS_MOVE_SMS_TO_ME);
    }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)    
    else if(MMISMS_MENU_BOX_SECURITY_ITEM_ID == menu_id)
    {       
        MMISMS_SetMoveOperStatus(MMISMS_MOVE_SMS_TO_SECURITY);
        move_sms_num = MMISMS_GetSmsBoxMoveAvailNum(box_type);
    }
#endif    
    else if(MMISMS_MENU_BOX_PREVIEW_ITEM_ID == menu_id)
    {
        MMISMS_SetMoveOperStatus(MMISMS_MOVE_SMS_FROM_SECURITY);
        move_sms_num = MMISMS_GetSmsBoxMoveAvailNum(box_type);
    }
    

    if (marked_sms_num > 0)
    {
        MMI_GetLabelTextByLang(TXT_MOVE, &prompt_str1);
        
        disp_num_len = sprintf((char *)delete_msg_num, "%ld ", move_sms_num);

        MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)delete_msg_num, disp_num_len, disp_num_len);

        if (move_sms_num > 1)
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEMS, &tmp_prompt);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEM, &tmp_prompt);
        }

        // the total string wstr_len
        disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
        
        // allocate memory
        prompt_str2.wstr_ptr = SCI_ALLOCAZ((disp_str_len+1)*sizeof(wchar));
        if(PNULL == prompt_str2.wstr_ptr)
        {
            return;
        }
        //SCI_MEMSET(prompt_str2.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        // copy the num
        MMI_WSTRNCPY(prompt_str2.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);

        prompt_str2.wstr_len = disp_num_len;
        
        // copy the prompt string
        MMI_WSTRNCPY(
            &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]),
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr, 
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
            );
        prompt_str2.wstr_len += tmp_prompt.wstr_len;

        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, handle_func);
        if (PNULL != prompt_str2.wstr_ptr)
        {
            SCI_FREE(prompt_str2.wstr_ptr);
            prompt_str2.wstr_ptr = PNULL;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MOVE, &prompt_str1);
        if(PNULL == order_id)
        {
            SCI_TRACE_LOW("mmisms cur order pnull");
            return;
        }
        if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
        {
            prompt_str2.wstr_len = order_id->display_content.content_len;
            prompt_str2.wstr_ptr = order_id->display_content.content;
        }
        else
        {
            MMI_WSTRNCPY(str,
                         MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                         order_id->display_content.content,
                         MMISMS_QUERY_STRING_MAX_NUM,
                         MMISMS_QUERY_STRING_MAX_NUM);

            MMI_STRNTOWSTR(str + MMISMS_QUERY_STRING_MAX_NUM,
                           MMISMS_ELLIPSE_STR_LEN,
                           (const uint8 *)ellipse_str,
                           MMISMS_ELLIPSE_STR_LEN,
                           MMISMS_ELLIPSE_STR_LEN);
            prompt_str2.wstr_ptr = str;
            prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
        }
       
        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, handle_func);
    }
}
#endif


/*****************************************************************************/
//     Description : to handle the new message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleNewMsgWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
//    MMISMS_READ_MSG_T read_msg = {0};
    dual_sys = MMISMS_GetSimIdOfNewMsg();
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //如果屏保被点亮中，则关闭屏保窗口
        if (MMK_IsOpenWin(MMIMAIN_SHOWTIME_WIN_ID))
        {
            MMK_CloseWin(MMIMAIN_SHOWTIME_WIN_ID);
        }
        break;

    case MSG_FULL_PAINT:
        if (
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_IsPhoneLocked() ||
#endif
            MMISMS_IsInSMSWin()
           || MMIAPIMMS_IsInMMSWin()
#ifdef MMI_OTA_SUPPORT
           || MMIAPIOTA_IsInOTAWin()
#endif
#ifdef CAMERA_SUPPORT
           && !MMIAPIDC_IsRecording()
#endif
           )
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
#ifdef CMCC_TEST_FLAG
            //CMCC入库测试修改
            if(MMIAPIDC_IsOpened())
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
            }
            else
#endif
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_READ, STXT_EXIT, FALSE);
            }
        }

        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_SMS_PLAY_RING:
        MMISMS_PlayNewSMSRing(dual_sys);
        break;

    case MSG_TIMER:
        //SCI_TRACE_LOW:"MMISMS_HandleNewMsgWin %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_19422_112_2_18_2_55_1_799,(uint8*)"d", MMIAPIENVSET_GetVibStatus());
        if (MMIENVSET_IsOnetimeVibTimerOut(param))
        {
            if (MMIAPIENVSET_GetVibStatus())
            {
                MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//关振动
                MMIAPIENVSET_SetVibStatus(FALSE);
                MMIENVSET_StartOnetimeVibTimer();
            }
            else
            {
                MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_MSG);
                MMIAPIENVSET_SetVibStatus(TRUE);
                MMIENVSET_StartOnetimeVibTimer();
            }
        }
        else
        {
            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        }
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef MMI_ETWS_SUPPORT
        MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
        MMIAPISET_StopAllRing(FALSE);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_NONE);
#endif
        if (
#ifdef MMI_KEY_LOCK_SUPPORT
              !(MMIAPIKL_IsPhoneLocked() || 
#endif
              MMISMS_IsInSMSWin()   || 
              MMIAPIMMS_IsInMMSWin() 
#ifdef MMI_OTA_SUPPORT
              ||MMIAPIOTA_IsInOTAWin()
#endif
#ifdef CAMERA_SUPPORT
              && !MMIAPIDC_IsRecording()
#endif
           )
        {
#if defined(MMI_SMSCB_SUPPORT)
            if ( MMISMS_CB_NEWMSG_WIN_ID == win_id )
            {
                MMIAPISMSCB_OpenCBList();

                MMK_CloseWin(win_id);
            }
            else
#endif
            {
                if (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS) &&
                    !MMISMS_IsInSMSWin())
                {
                    MMK_CloseWin(win_id);
                    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, (MMISET_PRIVACY_CALLBACK_PFUNC)SMS_ValidatePrivacyReadNewSms, (void*)&win_id, sizeof(win_id));
                }
                else
                {
                    if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                    {
                        MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                    }
                    if(MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
                    {
                        MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
                    }
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
#ifdef MMI_SMS_CHAT_SUPPORT
                    MMISMS_EnterSmsBox( MMISMS_BOX_CHAT );
#endif
#else
                    MMISMS_EnterSmsBox( MMISMS_BOX_MT );
#endif
                    if ((1 == MMISMS_GetTobeReadSmsCount(MN_DUAL_SYS_MAX) 
                        && ( MMISMS_GetFirstUnreadSmsId() != PNULL ))
                        )
                    {
                      #ifndef MMI_PDA_SUPPORT  
                        uint16 unread_msg_index = MMISMS_GetMTUnDownloadMsgCount(MN_DUAL_SYS_MAX);

                        //排序顺序为未下载彩信，新信息，新彩信，其他MT信息
                        GUILIST_SetCurItemIndex(MMISMS_MTBOX_LISTBOX_CTRL_ID, unread_msg_index);
                        MMK_PostMsg(MMISMS_MSGBOX_MAIN_WIN_ID,
                            MMISMS_MSG_READ_MSG,
                            PNULL,
                            0
                            );
                     #else	
                     MMI_STRING_T     prompt_str = {0};
                     MMISMS_ORDER_ID_T order_id = PNULL;
                     MMISMS_ORDER_ID_T cur_order_id = PNULL;
                     int32 i = 0;
         #ifdef MMI_TIMERMSG_SUPPORT
                     if (MMISMS_GetIsTimerMsg())
                     {
                         MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);        
                         break;
                     }
         #endif
#ifdef MMI_SMS_CHAT_SUPPORT 
                     MMISMS_SetCurType(MMISMS_BOX_CHAT);		 
                     order_id = MMISMS_GetChatUnreadOrderId();
#else
			order_id = MMISMS_GetFirstUnreadMsgId();
#endif
                     if(order_id == PNULL || order_id->flag.msg_type != MMISMS_TYPE_SMS)
                     {
                         //SCI_TRACE_LOW:"mmisms: order_id is PNULL"
                         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_19519_112_2_18_2_55_1_800,(uint8*)"");
                         break;
                     }
                     MMISMS_SetSMSUpdateInfo(order_id);                     
                     MMISMS_InitCurOperationOrderId();                       
                     
                     if (order_id->flag.is_concatenate_sms) //is long sms
                     {
                         cur_order_id = order_id;
                         while (cur_order_id != PNULL)
                         {
                             MMISMS_SetCurOrderId(i,cur_order_id); //设置当前order_id到全局变量数组
                             cur_order_id  = cur_order_id->next_long_sms_ptr;
                             i++;
                         }
                     }
                     else //is normal sms
                     {
                         MMISMS_SetCurOrderId(0,order_id); //设置当前order_id到全局变量数组 
                     }  
                     
                     cur_order_id = MMISMS_GetCurOperationOrderId();
#ifdef MMI_SMS_CHAT_SUPPORT 
                     if(0 == MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_CHAT))
                     {
                         break;
                     }

                     MMISMS_EnterSMSChatReadWin(cur_order_id);
//                            MMK_CreateWin((uint32 *)MMISMS_SHOW_CHAT_SMS_WIN_TAB, PNULL);
#else
                     if(0 == MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_MT))
                     {
                         break;
                     }
                     
                     //MMK_CreateWin((uint32 *)MMISMS_SHOWMSG_WIN_TAB/*MMISMS_SHOW_CHAT_SMS_WIN_TAB*/,PNULL);
                     read_msg = MMISMS_GetReadResult();
                     MMISMS_EnterShowSmsWin(read_msg);
#endif
                     MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
                     MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);            
                    #endif        
                    }

                    MMK_CloseWin(win_id);

                }
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    case MSG_APP_RED:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMK_CloseWin(win_id);
        }
        else
#endif
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_CLOSE_WINDOW:
#ifdef MMI_ETWS_SUPPORT
        if(!MMIAPISMSCB_GetETWSAlertFlag())
        {
            MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);
            MMIENVSET_StopOnetimeVibTimer();
            MMIAPIENVSET_SetVibStatus(FALSE);
            MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
        }
        else
        {
            MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
        }

        MMIAPISET_StopAllRing(FALSE);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_NONE);
#else
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
        MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);
        MMIENVSET_StopOnetimeVibTimer();
        MMIAPIENVSET_SetVibStatus(FALSE);

        MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
#endif

        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_LOSE_FOCUS:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIAPIKL_SetNewSMSStatus();//设置新信息状态
        }
#endif
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
}


/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void ShowNewMsgPrompt(void)
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 new_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    uint16 to_be_read_num = MMISMS_GetAllUnreadMsgCount();
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_STRING_T prompt_str = {0};
    MMI_STRING_T tmp_prompt = {0};
    uint32 time_out = 0;
    MMI_WIN_ID_T alert_win_id = MMISMS_NEWMSG_WIN_ID;
    MN_DUAL_SYS_E dual_sys = MMISMS_GetSimIdOfNewMsg();
    
    //init the string, which will be displayed
    prompt_str.wstr_len = 0;
    prompt_str.wstr_ptr = PNULL;

    // if there is no new message, return
    if (0 == to_be_read_num)
    {
        return;
    }
        
    disp_num_len= sprintf((char *)new_msg_num, "%d", to_be_read_num);
    MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)new_msg_num, disp_num_len, disp_num_len);

    if (to_be_read_num > 1)
    {
        MMI_GetLabelTextByLang(TXT_SMS_NEW_MSGS, &tmp_prompt);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SMS_NEW_MSG, &tmp_prompt);
    }
    // the total string wstr_len
    disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
    
    // allocate memory
    prompt_str.wstr_ptr = SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
    SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));

    MMI_WSTRNCPY(prompt_str.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);

    prompt_str.wstr_len = disp_num_len;

    // copy the prompt string
    MMI_WSTRNCPY(
                &(prompt_str.wstr_ptr[prompt_str.wstr_len]),
                tmp_prompt.wstr_len,
                tmp_prompt.wstr_ptr, 
                tmp_prompt.wstr_len,
                tmp_prompt.wstr_len
                );
    prompt_str.wstr_len += tmp_prompt.wstr_len;
    
    if (MMK_IsFocusWin(MMISMS_NEWMSG_WIN_ID))
    {
        // set to window
        MMIPUB_SetAlertWinTextByPtr(alert_win_id,&prompt_str,PNULL,TRUE);
#ifdef MMI_PDA_SUPPORT
        MMK_UpdateScreen();
#endif
        // play the ring
        MMK_PostMsg(alert_win_id, MSG_SMS_PLAY_RING, PNULL, 0);
    }
    else
    {
        MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);

        // 窗口: 提示用户新短消息
#ifdef SCREENSAVER_SUPPORT
        if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
        {
            MMIAPIIDLESS_CloseScreenSaver();
        }
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
        MMIAPIKL_SetNewSMSStatus();
#endif
        if(MMISMS_IsForbitNewMsgWin(FALSE))
        {
            win_priority = WIN_LOWEST_LEVEL;
        }
        else
        {
            win_priority = WIN_ONE_LEVEL;
        }

#if 0
#ifdef VT_SUPPORT
        if (MMIAPIVT_IsVtConnecting())
        {
            win_priority = WIN_ONE_LEVEL;
        }
#endif
        
        if(MMIAPIMTV_IsPlaying())
        {
            win_priority = WIN_ONE_LEVEL;
        }
        
#ifdef BROWSER_SUPPORT
        if(MMIAPIBROWSER_IsActive()
#ifdef VIDEO_PLAYER_SUPPORT
            ||(MMIAPIVP_IsFocusPlayerWin() && !MMIAPIVP_IsLiving())
#endif
#ifdef PUSH_EMAIL_SUPPORT
            ||MMIAPIMAIL_IsActive()
#endif /* PUSH_EMAIL_SUPPORT  */
            )
#else
#ifdef VIDEO_PLAYER_SUPPORT
        if(MMIAPIVP_IsFocusPlayerWin() && !MMIAPIVP_IsLiving())
#else
        if (0)
#endif
#endif
                {
                    win_priority = WIN_ONE_LEVEL;
                    //MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleNewMsgWin);
                }
#ifdef JAVA_SUPPORT
        else if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) ||
                 MMIAPIJAVA_IsFocusJAVARunWin())
#else
        else if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
#endif

                {
                    win_priority = WIN_ONE_LEVEL;
                    //MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleNewMsgWin);
                }        
                else
                {
                    //MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleNewMsgWin);
                }
#endif

#if defined MMIWIDGET_SUPPORT //&& !defined(FLASH_SUPPORT)
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
        {
            MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_NEWSMS_ID, FALSE, TRUE);            
        }

        if(MMK_IsFocusWin(MAIN_IDLE_WIN_ID) && (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()))
        {
        }
        else
#endif
//         if (MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle()
//                 || MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle()
//                 || WIN_ONE_LEVEL == win_priority)
        {
            if(MMISMS_IsInSMSWin()
               || MMIAPIMMS_IsInMMSWin()
#ifdef MMI_OTA_SUPPORT
               || MMIAPIOTA_IsInOTAWin()
#endif
#ifdef CAMERA_SUPPORT
               && !MMIAPIDC_IsRecording()
#endif
               )
            {
                MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,MMISMS_HandleNewMsgWin);
                 g_s_prompt_win_handle = (MMI_HANDLE_T)alert_win_id;
            }
            else
            {
#ifdef CMCC_TEST_FLAG
                //CMCC入库测试修改
                if(MMIAPIDC_IsOpened())
                {
                    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,MMISMS_HandleNewMsgWin);
                }
                else
#endif
                {
                    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleNewMsgWin);
                }

                g_s_prompt_win_handle = (MMI_HANDLE_T)alert_win_id; 
            }
            MMIPUB_SetWinTitleTextId(MMISMS_NEWMSG_WIN_ID, TXT_COMMON_PROMPT, FALSE);
        }
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked() && MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
        {
            MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNREAD_MESSAGE,PNULL,0);
        }
#endif
        MMISMS_PlayNewSMSRing(dual_sys);
    }

    if (PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//     Description : 触发监听新短信事件的所有回调函数
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
LOCAL void TriggerNewSMSCallback(uint16 to_be_read_num)
{
	MMIEVENT_INFO_T event_info = {0};

	event_info.event_type = SMS_NEWSMS_EVENT;
	event_info.param_ptr = (void*)&to_be_read_num;
	MMIFW_TriggerEvent(&event_info);
}

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewMsgPrompt(
                             MN_DUAL_SYS_E  dual_sys,
                             BOOLEAN    is_play_ring    //IN:
                             )
{
    uint16 to_be_read_num = MMISMS_GetTobeReadSmsCount(MN_DUAL_SYS_MAX);
    
#ifdef MET_MEX_SUPPORT
    if(MMIMEX_IsSysRunning())
    {// mex application is running, let mex do the process
        return;
    }
#endif
    // save the flag
    MMISMS_SetNewSMSRingFlag(is_play_ring);
    MMISMS_SetSimIdOfNewMsg(dual_sys);

    // if there is no new message, return
    if (0 == to_be_read_num)
    {
        return;
    }
#ifdef  SXH_APPLE_SUPPORT
    if (AppleIsRun())
    {
        MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
        return;
    }
#endif  
    // set SMS status in idle
    MAIN_SetMsgState(TRUE);
    
/*
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
    MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MESSAGE);
#endif
*/
    //TriggerNewSMSCallback(to_be_read_num);

#ifdef MRAPP_SUPPORT
    if(!MMIMRAPP_GetNewSmsAction())
    {
        return;
    }
#endif
#ifdef HERO_ENGINE_SUPPORT 
		if(!MMIHEROAPP_GetNewSmsAction())
		{
			return;
		}
#endif

    ShowNewMsgPrompt();

    MMIAPIENG_VibrateWinGetFocus();
}

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void SetPromptWinId(MMI_HANDLE_T win_id)
{
    g_s_prompt_win_handle = win_id;
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void SMS_ValidatePrivacyReadNewSms(BOOLEAN is_permitted, DPARAM param_ptr)
{
#if defined(MMI_SMSCB_SUPPORT)
    MMI_WIN_ID_T win_id = 0;
#endif
    if (is_permitted)
    {
#if defined(MMI_SMSCB_SUPPORT)    
        if (PNULL != param_ptr)
        {
            win_id = *(MMI_WIN_ID_T*)param_ptr;
        }

        if ( MMISMS_CB_NEWMSG_WIN_ID == win_id )
        {
            MMIAPISMSCB_OpenCBList();
        }
        else
#endif
        {
#ifdef MMI_SMS_CHAT_SUPPORT
            MMISMS_EnterSmsBox(MMISMS_BOX_CHAT);
#else
            MMISMS_EnterSmsBox(MMISMS_BOX_MT);
#endif

            if (1 == MMISMS_GetTobeReadSmsCount(MN_DUAL_SYS_MAX) 
                && ( MMISMS_GetFirstUnreadSmsId() != PNULL ))
            {
                uint16 unread_msg_index = MMISMS_GetMTUnDownloadMsgCount(MN_DUAL_SYS_MAX);

                //排序顺序为未下载彩信，新信息，新彩信，其他MT信息
                GUILIST_SetCurItemIndex(MMISMS_MTBOX_LISTBOX_CTRL_ID, unread_msg_index);

                MMK_PostMsg(MMISMS_MSGBOX_MAIN_WIN_ID,
                    MMISMS_MSG_READ_MSG,
                    PNULL,
                    0
                    );
            }
        }
    }
}

/*****************************************************************************/
//     Description : to play msg ring
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void PlaySMSRing(MN_DUAL_SYS_E dual_sys)
{
    MMISET_MSG_RING_TYPE_E ring_type = MMISET_MSG_RING;
    
    ring_type = (MMISET_MSG_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(dual_sys, MSG_RING_TYPE);
// #ifdef CAMERA_SUPPORT
//     if ( MMIAPIDC_IsRecording())
//     {
//         return;
//     }
// #endif   //DV录制时来短信有震动提示 bug132739
    if(IsNewMsgNeedVibra())
    {
        SCI_TRACE_LOW("PlaySMSRing oil ring_type=%d",ring_type);
#ifndef ADULT_WATCH_SUPPORT
        if ((MMISET_MSG_RING == ring_type)||(MMISET_MSG_VIBRA_AND_RING == ring_type))
        {
            ring_type = MMISET_MSG_VIBRA;
        }
#else
        if (MMISET_MSG_VIBRA_AND_RING == ring_type)
        {
            ring_type = MMISET_MSG_VIBRA;
        }
        else if(MMISET_MSG_RING == ring_type)//bugid:1633296
        {
            ring_type = MMISET_MSG_SILENT;
        }
#endif
    }

    //SCI_TRACE_LOW:"PlaySMSRing ring_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_20102_112_2_18_2_55_2_807,(uint8*)"d",ring_type);
    
    switch( ring_type )
    {
    case MMISET_MSG_RING:
        s_sms_is_continue_mp3 = TRUE;
        MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG,RingCallBack);
        break;

    case MMISET_MSG_VIBRA:
        //if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_MSG))
        if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_MSG))
        {
            MMIAPISET_StopAllVibrator();
            //MMIAUDIO_Vibrate(MMISMS_VIBRA_TIMEROUT, FALSE);
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
        }
        break;

    case MMISET_MSG_SILENT:
        //MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_MSG);
        break;

    case MMISET_MSG_VIBRA_AND_RING:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_MSG))
        {
            s_sms_is_continue_mp3 = TRUE;
            if ( 0 == MMIAPIENVSET_GetActiveModeOptValue(dual_sys, MSG_RING_VOL) )
            {
                //MMIAUDIO_Vibrate(MMISMS_VIBRA_TIMEROUT, FALSE);
                MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
            }
            else
            {
                //SCI_TRACE_LOW:"mary MMISET_MSG_VIBRA_AND_RING"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_20136_112_2_18_2_55_2_808,(uint8*)"");
                
                MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//maryxiao cr231219
                MMIENVSET_StopOnetimeVibTimer();
                MMIAPIENVSET_SetVibStatus(FALSE);
                MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG, RingCallBack);
                // 开启震动
                MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_MSG);
                MMIENVSET_StartWinOnetimeVibTimer(g_s_prompt_win_handle); // MMIENVSET_StartWinOnetimeVibTimer(g_s_prompt_win_handle);
                MMIAPIENVSET_SetVibStatus(TRUE);
                //MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
            }
        }
        break;

    default:
        break;
    }
}
#if defined(MMI_VCARD_SUPPORT)
/*****************************************************************************/
// Description : to handle the new Vcard message window
//               (参考函数:MMISMS_HandleNewMsgWin)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
MMI_RESULT_E MMISMS_HandleNewVcardMsgWin(MMI_WIN_ID_T win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM param)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    uint16 *name_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint16 name_len = 0;
    
    dual_sys = MMISMS_GetSimIdOfNewMsg();

    switch (msg_id)
    {
    case MSG_FULL_PAINT:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
#endif
        {
            MMIPUB_SetWinSoftkey(win_id, STXT_DETAIL, STXT_EXIT, FALSE);
        }
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_SMS_PLAY_RING:
        MMISMS_PlayNewSMSRing(dual_sys);
        break;

    case MSG_SMS_VCARD_FILE_NAME:
        name_ptr = (uint16 *)param;

        if (PNULL != s_new_vcard_name_ptr) //free memory
        {
            SCI_FREE(s_new_vcard_name_ptr);
            s_new_vcard_name_ptr = PNULL;
        }

        name_len = MMIAPICOM_Wstrlen((const wchar *)name_ptr); //name len
        //SCI_TRACE_LOW:"MMISMS_HandleNewVcardMsgWin name_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_22519_112_2_18_2_55_7_820,(uint8*)"d", name_len);

        s_new_vcard_name_ptr = SCI_ALLOCAZ((name_len + 1) * sizeof(wchar)); //alloc memory
        if (PNULL != s_new_vcard_name_ptr)
        {
            //SCI_MEMSET(s_new_vcard_name_ptr, 0x00, ((name_len + 1) * sizeof(wchar)));
            SCI_MEMCPY(s_new_vcard_name_ptr, name_ptr, name_len * sizeof(wchar)); //copy name
        }
        break;

        case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {
#if defined MMI_VCARD_SUPPORT
            MMIAPIPB_OpenVcardFileContactWin((uint16 *)s_new_vcard_name_ptr);
#endif
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMK_CloseWin(win_id);
        }
        else
#endif
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {
            if (PNULL != s_new_vcard_name_ptr)
            {
                SCI_FREE(s_new_vcard_name_ptr);
                s_new_vcard_name_ptr = PNULL;
            }
        }

        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_LOSE_FOCUS:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
#if defined(MMI_VCARD_SUPPORT)
            MMIAPIKL_SetNewVcardStatus();//设置新Vcard状态
#endif
        }
#endif
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;
}
#endif

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : to handle the new Vcalendar message window
//               (参考函数:MMISMS_HandleNewMsgWin)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
MMI_RESULT_E MMISMS_HandleNewVcalendarMsgWin(MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    LOCAL uint16 *name_ptr = PNULL;
    uint16 **p = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint16 name_len = 0;
    
    dual_sys = MMISMS_GetSimIdOfNewMsg();

    switch (msg_id)
    {
    case MSG_FULL_PAINT:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
#endif
        {
            MMIPUB_SetWinSoftkey(win_id, STXT_DETAIL, STXT_EXIT, FALSE);
        }
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_SMS_PLAY_RING:
        MMISMS_PlayNewSMSRing(dual_sys);
        break;

    case MSG_SMS_VCARD_FILE_NAME:
        p = param;
        name_ptr = (uint16 *)*p;

        if(PNULL==name_ptr)
        {
           break; 
        }

        if (PNULL != s_new_vcalendar_name_ptr) //free memory
        {
            SCI_FREE(s_new_vcalendar_name_ptr);
            s_new_vcalendar_name_ptr = PNULL;
        }

        name_len = MMIAPICOM_Wstrlen((const wchar *)name_ptr); //name len
        //SCI_TRACE_LOW:"MMISMS_HandleNewVcalendarMsgWin name_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_22651_112_2_18_2_55_7_821,(uint8*)"d", name_len);

        s_new_vcalendar_name_ptr = SCI_ALLOCAZ((name_len + 1) * sizeof(wchar)); //alloc memory
        if (PNULL != s_new_vcalendar_name_ptr)
        {
            //SCI_MEMSET(s_new_vcalendar_name_ptr, 0x00, ((name_len + 1) * sizeof(wchar)));
            SCI_MEMCPY(s_new_vcalendar_name_ptr, name_ptr, name_len * sizeof(wchar)); //copy name
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {
            if (PNULL != name_ptr)
            {
#ifdef CALENDAR_SUPPORT
                MMIAPISCH_ReadVcalendarFile((uint16 *)name_ptr);
#endif
            }
            else if (PNULL != s_new_vcalendar_name_ptr)
            {
#ifdef CALENDAR_SUPPORT
                MMIAPISCH_ReadVcalendarFile((uint16 *)s_new_vcalendar_name_ptr);
#endif
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMK_CloseWin(win_id);
        }
        else
#endif
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
        if (PNULL != name_ptr)
        {
            SCI_FREE(name_ptr);
            name_ptr = PNULL;
        }
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {
            if (PNULL != s_new_vcalendar_name_ptr)
            {
                SCI_FREE(s_new_vcalendar_name_ptr);
                s_new_vcalendar_name_ptr = PNULL;
            }
        }

        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_LOSE_FOCUS:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIAPIKL_SetNewVcalendarStatus();
        }
#endif
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;
}
#endif


/*****************************************************************************/
//     Description : to STOP the vibrator
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void RingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"RingCallBack ENTER RingCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_20070_112_2_18_2_55_2_806,(uint8*)"");
    MMIAPISET_AlertRingPlayResult(result, param);

    MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//maryxiao cr231219
    MMIENVSET_StopOnetimeVibTimer();
    MMIAPIENVSET_SetVibStatus(FALSE);
    //MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
    
    if (s_sms_is_continue_mp3)
    {
        s_sms_is_continue_mp3 = FALSE;
    }
    SCI_TRACE_LOW("[RingCallBack]result:%d",result);
    if(MMISRVAUD_REPORT_RESULT_SUCESS == result)   //表示短信提示音播放完毕   //bug2093978
    {
        if(MMK_IsOpenWin(MMIALM_ALIVEWIN_ID))  //如果闹钟起闹窗打开，继续播放闹钟起闹声音
        {
            SCI_TRACE_LOW("[RingCallBack]continue alarm ring");
            MMK_SendMsg(MMIALM_ALIVEWIN_ID, MSG_GET_FOCUS, PNULL);
        }
    }
}
/*****************************************************************************/
//     Description : play new message ring
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_PlayNewSMSRing(MN_DUAL_SYS_E dual_sys)
{
#ifdef JAVA_SUPPORT
    if (MMIAPIJAVA_IsFocusJAVARunWin()
#ifdef WRE_SUPPORT
        ||WRE_IsFocusRunWin()
#endif
        )    //$discuss
    {
        //MMIAUDIO_Vibrate(MMISMS_VIBRA_TIMEROUT, FALSE);
        MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
    }
    else
#endif
    {
#ifdef WRE_SUPPORT
        if(WRE_IsFocusRunWin())
        {
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
        }
        else
#endif
        {
            if (MMIAPICC_IsInState(CC_IN_CALL_STATE) && !MMIAPIUDISK_UdiskIsRun())
            {
                //SCI_TRACE_LOW:"SMS: PlayNewSMSRing vt mode"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_19809_112_2_18_2_55_1_803,(uint8*)"");
                if (MMIAPICC_IsVideoCall())    //$discuss
                {
                    //MMIAUDIO_Vibrate(MMISMS_VIBRA_TIMEROUT, FALSE);
                    MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
                }
                else
                {        
                    //SCI_TRACE_LOW:"MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_19817_112_2_18_2_55_1_804,(uint8*)"");
                    MMIAPISET_PlayRing(dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_MSG_IN_CALL,PNULL);
                }
            }
            else
            {
                PlaySMSRing(dual_sys);
            }
        }
    }
}


/*****************************************************************************/
//  Description : is vibra new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNewMsgNeedVibra(void)
{
    if (!MMISMS_IsPermitPlayNewMsgRing()  
        //|| MMIAPIUDISK_UdiskIsRun() 
#ifdef JAVA_SUPPORT
        ||(MMIAPIJAVA_IsJavaRuning() && AUDIO_GetIfAudioWork())// MMIAPIJAVA_IsFocusJAVARunWin()//maryxiao cr231358
#endif
#ifdef WRE_SUPPORT
		||WRE_IsFocusRunWin()
#endif
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//     Description : play new message ring
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_PlayNewSMSRing(MN_DUAL_SYS_E dual_sys)
{    
    if (MMISMS_GetNewSMSRingFlag())
    {
#ifdef JAVA_SUPPORT
        if(MMIAPIJAVA_IsJavaRuning()
#ifdef WRE_SUPPORT
            ||WRE_IsFocusRunWin()
#endif
            )// MMIAPIJAVA_IsFocusJAVARunWin())    //maryxiao cr231358
        {
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
        }
        else
#endif
        {
#ifdef WRE_SUPPORT
            if(WRE_IsFocusRunWin())
            {
                MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISMS_VIBRA_TIMEROUT);
            }
            else
#endif
            {
                if (MMIAPICC_IsInState(CC_IN_CALL_STATE) && !MMIAPIUDISK_UdiskIsRun())
                {
                    
                    //SCI_TRACE_LOW:"MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_19769_112_2_18_2_55_1_802,(uint8*)"");
                    MMIAPISET_PlayRing(dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_MSG_IN_CALL,PNULL);
                }
                else
                {
                    PlaySMSRing(dual_sys);
                }
            }
        }
    }
}



/*****************************************************************************/
//     Description : to append the info to list_box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_AppendListItem(
                          wchar             *string_ptr,        //IN:
                          uint8                string_len,            //IN:
                          MMI_CTRL_ID_T        ctrl_id,            //IN:
                          MMI_TEXT_ID_T        left_softkey_id,    //IN:
                          MMI_TEXT_ID_T        mid_softkey_id,    //IN:Randy add new param @10-27 
                          uint16            pos,                //IN:
                          MMISMS_LIST_ITEM_TYPE_E type
                          )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    if (PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AppendListItem string_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_17251_112_2_18_2_54_56_789,(uint8*)"");
        return;
    }    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;

    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
//     MMI_WSTRNCPY( 
//                 item_data.item_content[0].item_data.text_buffer.wstr,
//                 GUILIST_STRING_MAX_NUM,
//                 string_ptr, 
//                 item_data.item_content[0].item_data.text_buffer.wstr_len,
//                 item_data.item_content[0].item_data.text_buffer.wstr_len
//                 );

    switch( type )
    {
    case MMISMS_APPEND_LIST_ITEM_TYPE:
        GUILIST_AppendItem( ctrl_id, &item_t );
        break;
    case MMISMS_REPLACE_LIST_ITEM_TYPE:
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
        break;
    case MMISMS_INSERT_LIST_ITEM_TYPE:
        GUILIST_InsertItem( ctrl_id, &item_t, pos );
        break;
    case MMISMS_SET_LIST_ITEM_TYPE:
        GUILIST_SetItemData(ctrl_id,&item_data, pos);
        break;
    default:
        break;
    }
}


/*****************************************************************************/
//     Description : open waiting window when the sms is initial
//    Global resource dependence : 
//  Author: liming.deng
/*****************************************************************************/
PUBLIC void MMISMS_OpenInitWaitingWindow(MMISMS_OPERATE_TYPE_E oper_type)
{
    MMI_STRING_T waiting_text = {0};
    g_oper_type = oper_type;
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &waiting_text);
    MMIPUB_OpenWaitWin(1, &waiting_text, PNULL, PNULL, MMISMS_WAITING_WIN_ID, IMAGE_NULL,
        ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleWaitingWinMsg);
}

/*****************************************************************************/
//     Description : 开机初始化时，进入短消息的等待窗口处理函数
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitingWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//     Description : set box type
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetBoxType(MMISMS_BOX_TYPE_E box_type)
{
    s_box_type = box_type;
}
/*****************************************************************************/
//     Description : 开机初始化完毕后，关闭等待窗口，并进入相应的操作。
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
void MMISMS_CloseWaitingWin(void)
{
    if (MMK_IsOpenWin(MMISMS_WAITING_WIN_ID))
    {
        MMK_CloseWin(MMISMS_WAITING_WIN_ID);
        switch(g_oper_type)
        { 
        case MMISMS_WRITE_NEW_SMS:            
            MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, FALSE, PNULL, 0);
            break;

        case MMISMS_OPEN_BOX:
            MMISMS_OpenSmsBox(s_box_type);
            break;

        case MMISMS_VIEW_MEMORY:
            MMISMS_EnterMessageCountWin();
            break;

        default:
            MMIAPISMS_ExitFromSMS();
            MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_SMS, MMISMS_OpenMainMenuWindowExt);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : Open Alert Wanring Win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenAlertWarningWin(MMI_TEXT_ID_T text_id)
{
    MMIPUB_CloseAlertWin();

    MMIPUB_OpenAlertWarningWin(text_id);
}


/*****************************************************************************/
//     Description : is in sms moudle window
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsInSMSWin(void)
{
    MMI_WIN_ID_T win_id = 0;
    BOOLEAN is_in_sms_win = FALSE;

    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        is_in_sms_win = TRUE;
    }

    if (!is_in_sms_win)
    {
        for (win_id = MMISMS_START_WIN_ID; win_id < MMISMS_END_WIN_ID; win_id++)
        {            
            if (MMISMS_MSGBOX_WIN_ID != win_id && MMK_IsOpenWin(win_id))
            {
                //SCI_TRACE_LOW:"MMISMS_IsInSMSWin :win_id is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_23040_112_2_18_2_55_8_825,(uint8*)"d",win_id);
                is_in_sms_win = TRUE;
                break;
            }
        }
    }

    return is_in_sms_win;

}
/*****************************************************************************/
//     Description : judge whether can play new message alert ring
//    Global resource dependence : 
//  Author: bruce.chi
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsPermitPlayNewMsgRing(void)
{
    return MMIDEFAULT_IsPermitPlayNewMsgRing();
}


/*****************************************************************************/
//     Description : open the SMS sending window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSendingWindow(
                             MN_DUAL_SYS_E  dual_sys,
                             MMI_STRING_T    *prompt_ptr        //IN:
                             )
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
    MMI_STRING_T sim_string_ptr = {0};
#endif
    uint8 addr_id = 0;
    MMI_STRING_T name = {0};
    wchar name_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    
    name.wstr_ptr = name_arr;
    edit_content_ptr = MMISMS_GetEditContentInfoPtr();
    addr_id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
    
    if (strlen((char *)edit_content_ptr->dest_info.dest_list.dest_addr[addr_id]) > 0)  
    {
        if(!MMISMS_GetNameByNumberFromPB(edit_content_ptr->dest_info.dest_list.dest_addr[addr_id], &name, MMISMS_PBNAME_MAX_LEN) )
        {
            name.wstr_len = strlen((char *)edit_content_ptr->dest_info.dest_list.dest_addr[addr_id]);
            name.wstr_len = MIN(MMISMS_PBNAME_MAX_LEN, name.wstr_len);

            MMI_STRNTOWSTR( name.wstr_ptr,
                            MMISMS_PBNAME_MAX_LEN,
                            (const uint8 *)edit_content_ptr->dest_info.dest_list.dest_addr[addr_id],
                            strlen((char *)edit_content_ptr->dest_info.dest_list.dest_addr[addr_id]),
                            name.wstr_len);
        }
        //MMISMS_SetEditContentInfo(edit_content);
    }

    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    sim_name = MMIAPISET_GetSimName(dual_sys);

    sim_string_ptr.wstr_len = sim_name.wstr_len;
    sim_string_ptr.wstr_ptr = sim_name.wstr_arr;
    MMIPUB_OpenWaitWin(3, &sim_string_ptr, prompt_ptr, &name, MMISMS_SEND_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,(MMIPUB_HANDLE_FUNC)MMISMS_HandleSendWaitWinMsg);
#else
    MMIPUB_OpenWaitWin(2,prompt_ptr,&name,PNULL,MMISMS_SEND_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,(MMIPUB_HANDLE_FUNC)MMISMS_HandleSendWaitWinMsg);
#endif
}


/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
MMI_RESULT_E MMISMS_HandleSendWaitWinMsg(
                                         MMI_WIN_ID_T        win_id,    //IN:
                                         MMI_MESSAGE_ID_E    msg_id,    //IN:
                                         DPARAM                param    //IN:
                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
    MMI_STRING_T sim_string_ptr = {0};
#endif
    uint8 addr_id = 0;
    MMI_STRING_T prompt_str = {0};
    MMI_STRING_T name = {0};
    wchar name_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint8 seq_num = 0;
    uint8 total_num = 0;
    MMISMS_SEND_T *send_info_ptr = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id,TXT_NULL,STXT_CANCEL,FALSE);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
#ifndef MMI_DUALMODE_ENABLE
        MNSMS_CancelMoSmsEx(MMISMS_GetCurSendDualSys());
        MMK_SendMsg(win_id,MMISMS_MSG_UPDATE_NUMBER, PNULL);
        //SCI_TRACE_LOW:"MMISMS_HandleSendWaitWinMsg: Send cancel! dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6266_112_2_18_2_54_33_739,(uint8*)"d",MMISMS_GetCurSendDualSys());
#ifdef MMI_SMS_MINI_SUPPORT
        {            
            MMISMS_SEND_T *send_message_ptr = PNULL;
            MMISMS_UPDATE_INFO_T *update_info_ptr = PNULL;
            MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
            MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
            MN_SMS_RECORD_ID_T record_id = 0;
            
            send_message_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
            if(PNULL != send_message_ptr)
            {
//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)			
                MMISMS_STATE_T old_sms_state = {0};
#endif
                SCI_TRACE_LOW("MMISMS_HandleSendWaitWinMsg : NULL != send_message_ptr");
                update_info_ptr = &send_message_ptr->mosend_update_data.mosend_update_info;
                
                // get the storage and record_id
                MMISMS_GetPointedSMSMemInfo(
                    update_info_ptr->order_id_arr[update_info_ptr->current_num],
                    &dual_sys,
                    &storage, 
                    &record_id
                    );
//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
                MMISMS_GetSmsInfoFromOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num], &old_sms_state)
                if((MMISMS_MO_DRAFT != old_sms_state.flag.mo_mt_type) && (MMISMS_MO_SEND_FAIL != old_sms_state.flag.mo_mt_type))
                {
                    MMISMS_DelSmsInfoInOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num]);  //根据order_id_arr找到删除的form中的index
                    MMISMS_HandleOrderChangeCallBack();
                }
#else
                MMISMS_DelSmsInfoInOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num]);  //根据order_id_arr找到删除的form中的index                
#endif                
            }
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);                
        }
#endif
#else
        MMISMS_ClearDestNum();
        MMISMS_CloseEditWin();
        MMK_CloseWin(win_id);
#endif        
        break;
    
    case MMISMS_MSG_UPDATE_NUMBER:
        send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
        if (PNULL == send_info_ptr)
        {
            //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSendWaitWinMsg send_info_ptr = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6278_112_2_18_2_54_33_740,(uint8*)"");
            break;
        }
#ifndef MMI_DUALMODE_ENABLE
        if(MMISMS_GetDeliverCancel())
        {
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
            MMIPUB_SetWaitWinText(TRUE,1, &prompt_str,PNULL,PNULL,MMISMS_SEND_ANIM_WIN_ID);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, TXT_NULL, TRUE);
        }
        else
        {
            name.wstr_ptr = name_arr;

            MMI_GetLabelTextByLang(TXT_SENDING, &prompt_str);

            addr_id = send_info_ptr->send_content.dest_info.dest_list.dest_addr_id;
            if (strlen((char *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id]) > 0)  
            {
                if(!MMISMS_GetNameByNumberFromPB(send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id], &name, MMISMS_PBNAME_MAX_LEN) )
                {
                    name.wstr_len  = SCI_STRLEN((char *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id]);
                    name.wstr_len = MIN(MMISMS_PBNAME_MAX_LEN, name.wstr_len );
                    
                    MMI_STRNTOWSTR( name.wstr_ptr,
                                    MMISMS_PBNAME_MAX_LEN,
                        (const uint8 *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id],
                        strlen((char *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id]),
                                    name.wstr_len);
                    }
            }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            sim_name = MMIAPISET_GetSimName(MMISMS_GetCurSendDualSys());
            sim_string_ptr.wstr_len = sim_name.wstr_len;
            sim_string_ptr.wstr_ptr = sim_name.wstr_arr;
              
            MMIPUB_SetWaitWinText(TRUE,3, &sim_string_ptr, &prompt_str, &name,MMISMS_SEND_ANIM_WIN_ID);
#else
            MMIPUB_SetWaitWinText(TRUE,2, &prompt_str,&name,PNULL,MMISMS_SEND_ANIM_WIN_ID);
#endif
        }
#else
        name.wstr_ptr = name_arr;

        MMI_GetLabelTextByLang(TXT_SENDING, &prompt_str);

        addr_id = send_info_ptr->send_content.dest_info.dest_list.dest_addr_id;
        if (strlen((char *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id]) > 0)  
        {
            if(!MMISMS_GetNameByNumberFromPB(send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id], &name, MMISMS_PBNAME_MAX_LEN) )
            {
                name.wstr_len  = SCI_STRLEN((char *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id]);
                name.wstr_len = MIN(MMISMS_PBNAME_MAX_LEN, name.wstr_len );

                MMI_STRNTOWSTR( name.wstr_ptr,
                                MMISMS_PBNAME_MAX_LEN,
                                (const uint8 *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id],
                                strlen((char *)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id]),
                                name.wstr_len);
                }
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        sim_name = MMIAPISET_GetSimName(MMISMS_GetCurSendDualSys());
        sim_string_ptr.wstr_len = sim_name.wstr_len;
        sim_string_ptr.wstr_ptr = sim_name.wstr_arr;
          
        MMIPUB_SetWaitWinText(TRUE,3, &sim_string_ptr, &prompt_str, &name,MMISMS_SEND_ANIM_WIN_ID);
#else
        MMIPUB_SetWaitWinText(TRUE,2, &prompt_str,&name,PNULL,MMISMS_SEND_ANIM_WIN_ID);
#endif
#endif
        break;

    case MSG_TIMER:
        if(PNULL == (uint8*)param)
        {
            break;
        }
        send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
        if (PNULL == send_info_ptr)
        {
            //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSendWaitWinMsg send_info_ptr = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6361_112_2_18_2_54_33_741,(uint8*)"");
            break;
        }
        //SCI_TRACE_LOW:"MMISMS_HandleSendWaitWinMsg:Stop g_sms_retry_timer  %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6364_112_2_18_2_54_33_742,(uint8*)"d",send_info_ptr->resend_count);

        //重发

        if( g_sms_retry_timer == *(uint8*)param)
        {
            if (((0 < send_info_ptr->send_content.tpdu_head.length)
                && (MMISMS_HEAD_8BIT_IEI == send_info_ptr->send_content.tpdu_head.user_header_arr[MMISMS_HEAD_IEI_POS])
                && !MMISMS_IsR8Language(send_info_ptr->send_content.send_message.language))
                || ((MMISMS_R8_MAX_NORMAL_HEAD_LEN < send_info_ptr->send_content.tpdu_head.length)
                && (MMISMS_HEAD_8BIT_IEI == send_info_ptr->send_content.tpdu_head.user_header_arr[MMISMS_HEAD_IEI_POS])
                && MMISMS_IsR8Language(send_info_ptr->send_content.send_message.language)))
            {
                seq_num = GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1;
                total_num = GET_HEAD_MAX_NUM(&send_info_ptr->send_content);
            }
            else
            {
                seq_num = 1;
                total_num = 1;
            }
            // send the current message to the same destination
            MMISMS_SendMsgBySeqNum(MMISMS_GetCurSendDualSys(),total_num,seq_num,TRUE, send_info_ptr); 
            if (send_info_ptr->send_content.is_need_save//需要保存
                && (MN_SMS_NO_STORED == send_info_ptr->send_save_result.save_storage) ) //没有收到write_cnf
            {                
                send_info_ptr->send_save_result.is_finished = FALSE;
            }

            if (MMISMS_GetDeliverCancel())
            {
                send_info_ptr->send_save_result.send_result = MMISMS_SEND_FAIL;
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (0 != g_sms_retry_timer)
        {
            MMK_StopTimer(g_sms_retry_timer);
            g_sms_retry_timer = 0;
        }
        MMIAPIALM_CheckAndStartDelayedAlarm();
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(FALSE);
#endif
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
        MMISMS_SetIsAnswerSms(FALSE);
#endif
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : to display send err win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DisplaySendErr(
                                 MN_DUAL_SYS_E         dual_sys,
                                 MMISMS_OPER_ERR_E    oper_err    //IN:
                                 )
{
    switch (oper_err)
    {
    case MMISMS_NO_ERR:
        //打开消息发送窗口提示正在发送消息
        {
            MMI_STRING_T    prompt_str = {0};
            
            MMI_GetLabelTextByLang(TXT_SENDING, &prompt_str);
            
#ifdef MMI_SMS_MINI_SUPPORT
            if(!MMK_IsOpenWin(MMIPUB_ALERT_WIN_ID))
            {
                MMISMS_OpenSendingWindow(dual_sys, &prompt_str);
            }
#endif
        }
        break;
        
    case MMISMS_NO_SC_ADDRESS:
        //打开提示窗口：短消息中心未设置
		WatchPUB_Alert_Note_Win(MMISMS_NOTE_WIN_ID,TXT_SMS_NO_SC);

#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;

    case MMISMS_FDN_SCA_FAILED:
        MMIPUB_OpenAlertWarningWin(TXT_SMS_SC_NOT_IN_FDN);
        
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif        
        break;

    case MMISMS_FDN_DEST_NUM_FAILED:
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);

#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif                
        break;
        
    case MMISMS_NO_ENOUGH_SPACE:
        //打开提示窗口：存储空间不足
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);

#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;

    case MMISMS_SYSTEM_BUSY:
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMISMS_CALLING_BUSY:
        MMIAPICC_OpenSIMCallingPrompt();
        break;
#endif

    case MMISMS_SENDLIST_FULL:
        break;
        
    case MMISMS_NO_DEST_ADDRESS:
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);

#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;
        
    default:
        SCI_TRACE_LOW("MMISMS: DisplaySendErr oper_err = %d", oper_err);
        break;
    }
}

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : Set Empty to the list box   
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMS_AppendEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,        //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: for different position, the empty string is different.
                             MMI_TEXT_ID_T    left_softkey_id    //IN:
                             )
{
    MMI_STRING_T empty_str = {0};

    // need append empty string to the list box
    // get empty string
    MMI_GetLabelTextByLang(empty_txt_id, &empty_str);
    MMISMS_AppendListItem(
                    empty_str.wstr_ptr,
                    (uint8)empty_str.wstr_len,
                    ctrl_id,
                    left_softkey_id,
                    TXT_NULL,
                    0,
                    MMISMS_APPEND_LIST_ITEM_TYPE
                    );
}
#endif



/*****************************************************************************/
//     Description : DelPushCbf
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL int32 DelPushCbf(BOOLEAN result,void *param)
{
    uint32 record_id = (uint32)(param);
    MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH,record_id);
    if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
    {
        MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
    }
    //MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL,0);    
    MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
    return 0;    
}

/*****************************************************************************/
//     Description : DelOtaCbf
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL int32 DelOtaCbf(BOOLEAN result,void *param)
{
    uint32 record_id = (uint32)(param);
    MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA,record_id);
    if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
    {
        MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
    }
    //MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL, 0);    
    MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
    return 0;    
}
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
/*****************************************************************************/
//     Description : DelMailCbf
//    Global resource dependence : none
//  Author:sun
//    Note:
/*****************************************************************************/
PUBLIC void Mail_DelMailCbf(BOOLEAN result,int param)
{
    int record_id = param;
    MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MAIL,record_id);
    //如果查看邮件窗口打开，则关闭该窗口    
    if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
    {
        MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
    }
    //MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL, 0);    
    MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
}
#endif//PUSH_EMAIL_3_IN_1_SUPPORT

/*****************************************************************************/
//     Description : delete one sms call back
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_DeleteOneSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
    return MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT,PNULL, 0); 
}

/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleDelWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	MMISMS_DELWAIT_WIN_DATA *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_DELWAIT_WIN_DATA *)MMK_GetWinUserData(win_id);
    
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms del message data is pnull");
        return recode;        
    }
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelWaitWinMsg msg_id = 0x%x", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:    
        win_data_ptr->all_del_msg_num = 0;
        win_data_ptr->is_mms_del = FALSE;
        win_data_ptr->mms_is_sendrecv = FALSE;
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL,0); //刷新窗口
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MMISMS_MSG_DELETE_NEXT: 
        {    
            MMISMS_ORDER_ID_T cur_del_order = PNULL; 

            SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelWaitWinMsg MMISMS_MSG_DELETE_NEXT");

            cur_del_order = MMISMS_GetCurDeleteData(&win_data_ptr->opearte_data_info);
            if(PNULL == cur_del_order) //删完了
            {   
//                if(PNULL != win_data_ptr)
                {
                    MMISMS_DELETE_DATE_INFO data_info = {0};
                    data_info.all_del_msg_num = win_data_ptr->all_del_msg_num;
                    data_info.is_mms_del = win_data_ptr->is_mms_del;
                    data_info.mms_is_sendrecv = win_data_ptr->mms_is_sendrecv;
                    win_data_ptr->del_call_back(win_data_ptr->user_data,&data_info);
                }
                //for 126318
                //MMK_CloseWin(win_id);
                break;
            }  

            if (MMISMS_TYPE_MMS == cur_del_order->flag.msg_type && MMIAPIMMS_IsProcessMMSById(cur_del_order->record_id))
            {
                win_data_ptr->mms_is_sendrecv = TRUE;
            }        

            SCI_TRACE_LOW("MMISMS:MMISMS_HandleDelWaitWinMsg msg_type:%d, record_id:%d", cur_del_order->flag.msg_type, cur_del_order->record_id);
            switch(cur_del_order->flag.msg_type)
            {
                 case MMISMS_TYPE_SMS:
                     MMISMSAPP_DeleteSms(cur_del_order,MMISMS_DeleteOneSmscallback,PNULL,PNULL);
                     win_data_ptr->all_del_msg_num++;
                     break;
                     
                 case  MMISMS_TYPE_MMS:
                     if (MMIAPIUDISK_UdiskIsRun())
                     {                     
                         //MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL,0); 
                         MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
                     }
                     else
                     {
                         if(MMIAPIMMS_DeleteMMS(cur_del_order->record_id,MMISMS_DELETE_ANIM_WIN_ID))
                         {   
                             win_data_ptr->is_mms_del = TRUE;
                             //MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL,0); 
                             MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
                         }     
                         else
                         {
                            MMISMS_DELETE_DATE_INFO data_info = {0};
                            data_info.all_del_msg_num = win_data_ptr->all_del_msg_num;                    
                            data_info.is_mms_del = win_data_ptr->is_mms_del;
                            data_info.mms_is_sendrecv = win_data_ptr->mms_is_sendrecv;
                            win_data_ptr->del_call_back(win_data_ptr->user_data,&data_info);
                          }
                         
                         win_data_ptr->all_del_msg_num++;
                     } 
                     break;
                 case MMISMS_TYPE_MMS_OTA:
                 case MMISMS_TYPE_WAP_OTA:   
                     if (MMIAPIUDISK_UdiskIsRun())
                     {
                         //MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL, 0); 
                         MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
                     }
                     else
                     {
                #ifdef MMI_OTA_SUPPORT
                         MMIAPIOTA_DelOTAByID(cur_del_order->record_id, DelOtaCbf, (void *)cur_del_order->record_id);
                #endif
                         win_data_ptr->all_del_msg_num++;
                     }
                     break;
                     
                 case MMISMS_TYPE_WAP_PUSH:    
                    if (MMIAPIUDISK_UdiskIsRun())
                    {
                        //MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL, 0); 
                        MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
                    }
                    else
                    {
        #ifdef MMI_OTA_SUPPORT
                        MMIAPIOTA_DelPushByID(cur_del_order->record_id, DelPushCbf, (void *)cur_del_order->record_id);
        #endif
                        win_data_ptr->all_del_msg_num++;
                    } 
                    break;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
                case MMISMS_TYPE_MAIL:
                    if (MMIAPIUDISK_UdiskIsRun())
                    {
                        //MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL, 0); 
                        MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
                    }
                    else
                    {
                        if(Mail_DelMailByMsgId(cur_del_order->record_id) == TRUE)
                        {
                            Mail_DelMailCbf(TRUE, cur_del_order->record_id);
                        }
                        else
                        {
                            //MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL,0);
                            //MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL, 0); 
                            MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT); 
                        }
                        win_data_ptr->all_del_msg_num++;
                    }         
                    break;
#endif//PUSH_EMAIL_3_IN_1_SUPPORT  
                default:
                    break;
                }
        }
        break;

    case MSG_CLOSE_WINDOW:                       
        SCI_FREE(win_data_ptr);        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
//        if(PNULL != win_data_ptr)
        {
            MMISMS_DELETE_DATE_INFO data_info = {0};
            data_info.all_del_msg_num = win_data_ptr->all_del_msg_num;
            data_info.is_mms_del = win_data_ptr->is_mms_del;
            data_info.mms_is_sendrecv = win_data_ptr->mms_is_sendrecv;
            win_data_ptr->del_call_back(win_data_ptr->user_data,&data_info);
        }        
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterDelWaitWin(MMISMS_OPERATE_DATA_INFO_T *data_info_ptr,MMISMS_DEL_CALLBACK del_call_back,void*  user_data)
{
    MMISMS_DELWAIT_WIN_DATA *del_message_data = PNULL;
    MMI_STRING_T prompt_str = {0};
    MMI_HANDLE_T win_handle = PNULL;

    SCI_TRACE_LOW("[MMISMS] MMISMS_EnterDelWaitWin");

    if(PNULL == data_info_ptr )
    {
        return;
    }
    del_message_data = SCI_ALLOCAZ(sizeof(MMISMS_DELWAIT_WIN_DATA));
    if(PNULL == del_message_data)
    {
        return;
    }
    
    SCI_MEMCPY(&del_message_data->opearte_data_info,data_info_ptr,sizeof(MMISMS_OPERATE_DATA_INFO_T));
    del_message_data->del_call_back = del_call_back;
    del_message_data->user_data = user_data;
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);             
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleDelWaitWinMsg);   
    win_handle = MMK_GetWinHandle( MMK_GetFirstAppletHandle(), MMISMS_DELETE_ANIM_WIN_ID );
	if(win_handle)
	{
		MMK_SetWinUserData(win_handle, del_message_data);
	}
}


#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterMoveWaitWin(MMISMS_MOVE_DATA_INFO_T *data_info_ptr,MMISMS_MOVE_CALLBACK move_call_back,void*  user_data)
{
    MMISMS_MOVEWAIT_WIN_DATA *move_message_ptr = PNULL;
    MMI_STRING_T prompt_str = {0};
    MMI_HANDLE_T win_handle = PNULL;
        
    if(PNULL == data_info_ptr )
    {
        return;
    }
    move_message_ptr = SCI_ALLOCAZ(sizeof(MMISMS_MOVEWAIT_WIN_DATA));
    if(PNULL == move_message_ptr)
    {
        return;
    }
    
    SCI_MEMCPY(&move_message_ptr->move_data_info,data_info_ptr,sizeof(MMISMS_MOVE_DATA_INFO_T));
    move_message_ptr->move_call_back = move_call_back;
    move_message_ptr->user_data = user_data;
    MMISMS_SetMoveAllFlag(TRUE);
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);             
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_MOVE_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleMoveWaitWinMsg);   
    win_handle = MMK_GetWinHandle( MMK_GetFirstAppletHandle(), MMISMS_MOVE_ANIM_WIN_ID );
    if(win_handle)
    {
        MMK_SetWinUserData(win_handle, move_message_ptr);
    }
}

/*****************************************************************************/
//     Description : delete one sms call back
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_MoveOneSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
    MMISMS_MOVEWAIT_WIN_DATA *win_data_ptr = (MMISMS_MOVEWAIT_WIN_DATA *)MMK_GetWinUserData(MMISMS_MOVE_ANIM_WIN_ID);
    SCI_TRACE_LOW("mmisms move one sms call back");
    if(MMISMS_NO_ERR == callback_data->cause)
    {    
        return MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0); 
    }
    else
    {
        if(PNULL != win_data_ptr)
        {
            MMISMS_MOVE_RESULT_INFO data_info = {0};
            data_info.ret_value = callback_data->cause;
            data_info.all_move_msg_num = win_data_ptr->all_move_msg_num;
            data_info.is_mms_move = win_data_ptr->is_mms_move;
            data_info.mms_is_sendrecv = win_data_ptr->mms_is_sendrecv;
            win_data_ptr->move_call_back(win_data_ptr->user_data,&data_info);
        }
        MMK_CloseWin(MMISMS_MOVE_ANIM_WIN_ID);
        return TRUE;
    }    
}


/*****************************************************************************/
//  Description : to handle the message of move waiting window    
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleMoveWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	MMISMS_MOVEWAIT_WIN_DATA *win_data_ptr = PNULL;
    SMS_MOVE_OPERATE_DATA_T move_opt_data = {0};
    win_data_ptr = (MMISMS_MOVEWAIT_WIN_DATA *)MMK_GetWinUserData(win_id);
    
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms del message data is pnull");
        return recode;        
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:    
        win_data_ptr->all_move_msg_num = 0;
        win_data_ptr->is_mms_move = FALSE;
        win_data_ptr->mms_is_sendrecv = FALSE;
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        MMK_PostMsg(win_id, MMISMS_MSG_MOVE_NEXT,PNULL,0);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MMISMS_MSG_MOVE_NEXT: 
        {    
            MMISMS_ORDER_ID_T cur_move_order = PNULL;
            //SMS_MOVE_OPERATE_DATA_T move_data = PNULL;
            MMISMS_OPER_ERR_E ret_value = MMISMS_NO_ERR;
            cur_move_order = MMISMS_GetCurMoveData(&win_data_ptr->move_data_info,&ret_value);
            if(PNULL == cur_move_order) //move 完成
            {                   
//                if(PNULL != win_data_ptr)
                {
                    MMISMS_MOVE_RESULT_INFO data_info = {0};
                    data_info.ret_value = ret_value;
                    data_info.all_move_msg_num = win_data_ptr->all_move_msg_num;
                    data_info.is_mms_move = win_data_ptr->is_mms_move;
                    data_info.mms_is_sendrecv = win_data_ptr->mms_is_sendrecv;
                    win_data_ptr->move_call_back(win_data_ptr->user_data,&data_info);
                }
                MMK_CloseWin(win_id);
                break;
            }  
                   
            switch(cur_move_order->flag.msg_type)
            {
             case MMISMS_TYPE_SMS:
                 //MMISMS_GetSmsInfoFromOrder(first_pos,&sms_order_info);
                 //MMISMS_SetCurOrderId(g_mmisms_global.operation.cur_order_index, first_pos);    
                 if (cur_move_order->flag.mo_mt_type == MMISMS_MT_SR_TO_BE_READ
                 || cur_move_order->flag.mo_mt_type == MMISMS_MT_SR_HAVE_READ)
                 {
//                     if(PNULL != win_data_ptr)
                     {
                         MMISMS_MOVE_RESULT_INFO data_info = {0};
                         data_info.all_move_msg_num = win_data_ptr->all_move_msg_num;
                         data_info.is_mms_move = win_data_ptr->is_mms_move;
                         data_info.mms_is_sendrecv = win_data_ptr->mms_is_sendrecv;
                         data_info.ret_value = MMISMS_FINISH_OPER_ERR;
                         win_data_ptr->move_call_back(win_data_ptr->user_data,&data_info);
                     }        
                     MMK_CloseWin(win_id);
                     return recode;
                 }
                 move_opt_data.move_order_id = cur_move_order;
                 move_opt_data.oper_status = win_data_ptr->move_data_info.oper_status;
                 win_data_ptr->all_move_msg_num++;  
                 MMISMSAPP_MoveSms(move_opt_data,MMISMS_MoveOneSmscallback,PNULL,PNULL);  //win_data_ptr would be freed
                 break;
                 
             case MMISMS_TYPE_MMS:      
  	             if (MMIAPIUDISK_UdiskIsRun())
                 {
                     cur_move_order->flag.is_marked = FALSE;
                     MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0); 
     		         break;
                 }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                 if (MMISMS_FOLDER_SECURITY == cur_move_order->flag.folder_type)
                 {
                     MMIAPIMMS_MoveSecurityMMS(cur_move_order->record_id, FALSE);
                     cur_move_order->flag.folder_type = FALSE;
                 }
                 else
                 {
                     MMIAPIMMS_MoveSecurityMMS(cur_move_order->record_id, TRUE);
                     cur_move_order->flag.folder_type = MMISMS_FOLDER_SECURITY;
                     cur_move_order->flag.is_search = FALSE;
                 } 
                 MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0); 
                 win_data_ptr->all_move_msg_num++;
#endif
                 break;
            case MMISMS_TYPE_MMS_OTA:
            case MMISMS_TYPE_WAP_OTA:
    	         if (MMIAPIUDISK_UdiskIsRun())
                 {
                    cur_move_order->flag.is_marked = FALSE;
                    MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0); 
    		        break;
                 }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                 if (MMISMS_FOLDER_SECURITY == cur_move_order->flag.folder_type)
                 {
#ifdef MMI_OTA_SUPPORT
                     MMIAPIMMS_MoveSecurityOTA(cur_move_order->record_id, FALSE);
#endif
                     cur_move_order->flag.folder_type = FALSE;
                 }
                 else
                 {
#ifdef MMI_OTA_SUPPORT
                     MMIAPIMMS_MoveSecurityOTA(cur_move_order->record_id, TRUE);
#endif
                     cur_move_order->flag.folder_type = MMISMS_FOLDER_SECURITY;
                     cur_move_order->flag.is_search = FALSE;
                 }
                 MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0); 
                 win_data_ptr->all_move_msg_num++;
#endif
                break;

            case MMISMS_TYPE_WAP_PUSH: 
    	        if (MMIAPIUDISK_UdiskIsRun())
                {
                   cur_move_order->flag.is_marked = FALSE;
                   MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0); 
    		       break;
                }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                if (MMISMS_FOLDER_SECURITY == cur_move_order->flag.folder_type)
                {
#ifdef MMI_OTA_SUPPORT
                    MMIAPIMMS_MoveSecurityPush(cur_move_order->record_id, FALSE);
#endif
                    cur_move_order->flag.folder_type = FALSE;
                }
                else
                {
#ifdef MMI_OTA_SUPPORT
                    MMIAPIMMS_MoveSecurityPush(cur_move_order->record_id, TRUE);
#endif
                    cur_move_order->flag.folder_type = MMISMS_FOLDER_SECURITY;
                    cur_move_order->flag.is_search = FALSE;
                }
                MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0); 
                win_data_ptr->all_move_msg_num++;
#endif
            break;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support 
            case MMISMS_TYPE_MAIL:      
                if (MMISMS_FOLDER_SECURITY == cur_move_order->flag.folder_type)
                {
                    mail_MoveMailToSecurityBox(cur_move_order->record_id, FALSE);
                    cur_move_order->flag.folder_type = FALSE;
                }
                else
                {
                    mail_MoveMailToSecurityBox(cur_move_order->record_id, TRUE);
                    cur_move_order->flag.folder_type = MMISMS_FOLDER_SECURITY;
                    cur_move_order->flag.is_search = FALSE;
                } 
                MMK_PostMsg(MMISMS_MOVE_ANIM_WIN_ID, MMISMS_MSG_MOVE_NEXT,PNULL,0);
                win_data_ptr->all_move_msg_num++;
                break;
#endif//PUSH_EMAIL_3_IN_1_SUPPORT   
            default:
                break;
            }
        }
        break;

    case MSG_CLOSE_WINDOW:                       
        SCI_FREE(win_data_ptr);        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
//        if(PNULL != win_data_ptr)
        {
            MMISMS_MOVE_RESULT_INFO data_info = {0};
            data_info.all_move_msg_num = win_data_ptr->all_move_msg_num;
            data_info.is_mms_move = win_data_ptr->is_mms_move;
            data_info.mms_is_sendrecv = win_data_ptr->mms_is_sendrecv;
            data_info.ret_value = MMISMS_FINISH_OPER;
            win_data_ptr->move_call_back(win_data_ptr->user_data,&data_info);
        }        
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }    
    return (recode);    
}
#endif


#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterCopyWaitWin(MMISMS_COPY_DATA_INFO_T *data_info_ptr,MMISMS_COPY_CALLBACK copy_call_back,void*  user_data)
{
    MMISMS_COPYWAIT_WIN_DATA *copy_message_data = PNULL;
    MMI_STRING_T prompt_str = {0};
    MMI_HANDLE_T win_handle = PNULL;
        
    if(PNULL == data_info_ptr)
    {
        return;
    }
    copy_message_data = SCI_ALLOCAZ(sizeof(MMISMS_COPYWAIT_WIN_DATA));
    if(PNULL == copy_message_data)
    {
        return;
    }
    
    SCI_MEMCPY(&copy_message_data->copy_data_info,data_info_ptr,sizeof(MMISMS_COPY_DATA_INFO_T));
    copy_message_data->copy_call_back = copy_call_back;
    copy_message_data->user_data = user_data;
    MMISMS_SetMoveAllFlag(TRUE);
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);             
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_COPY_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleCopyWaitWinMsg);   
    win_handle = MMK_GetWinHandle( MMK_GetFirstAppletHandle(), MMISMS_COPY_ANIM_WIN_ID );
	if(win_handle)
	{
		MMK_SetWinUserData(win_handle, copy_message_data);
	}
}


/*****************************************************************************/
//     Description : delete one sms call back
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_CopyOneSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
    MMISMS_COPYWAIT_WIN_DATA *win_data_ptr = MMK_GetWinUserData(MMISMS_COPY_ANIM_WIN_ID);
    if(MMISMS_NO_ERR == callback_data->cause)
    {    
        return MMK_PostMsg(MMISMS_COPY_ANIM_WIN_ID, MMISMS_MSG_COPY_NEXT,PNULL,0); 
    }
    else
    {
        if(PNULL != win_data_ptr)
        {
            MMISMS_COPY_RESULT_INFO data_info = {0};
            data_info.ret_value = callback_data->cause;
            win_data_ptr->copy_call_back(win_data_ptr->user_data,&data_info);
        }
        MMK_CloseWin(MMISMS_COPY_ANIM_WIN_ID);
        return TRUE;
    }    
}

/*****************************************************************************/
//  Description : to handle the message of move waiting window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleCopyWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	MMISMS_COPYWAIT_WIN_DATA *win_data_ptr = PNULL;
    SMS_COPY_OPERATE_DATA_T copy_opt_data = {0};
    win_data_ptr = (MMISMS_COPYWAIT_WIN_DATA *)MMK_GetWinUserData(win_id);
    
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms del message data is pnull");
        return recode;        
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        MMK_PostMsg(win_id, MMISMS_MSG_COPY_NEXT,PNULL,0);               
        break;
        
    case MMISMS_MSG_COPY_NEXT: 
        {
            MMISMS_ORDER_ID_T cur_copy_order = PNULL;
            //SMS_MOVE_OPERATE_DATA_T move_data = PNULL;
            MMISMS_OPER_ERR_E ret_value = MMISMS_NO_ERR;
            cur_copy_order = MMISMS_GetCurCopyData(&win_data_ptr->copy_data_info,&ret_value);
            if(PNULL == cur_copy_order) //move 完成
            {                   
//                if(PNULL != win_data_ptr)
                {
                    MMISMS_COPY_RESULT_INFO data_info = {0};
                    data_info.ret_value = ret_value;
                    win_data_ptr->copy_call_back(win_data_ptr->user_data,&data_info);
                }
                MMK_CloseWin(win_id);
                break;
            }
    
            switch (cur_copy_order->flag.msg_type)
            {
               case MMISMS_TYPE_SMS:
               {
                   if (cur_copy_order->flag.mo_mt_type == MMISMS_MT_SR_TO_BE_READ
                       || cur_copy_order->flag.mo_mt_type == MMISMS_MT_SR_HAVE_READ)
                   {
                       MMISMS_COPY_RESULT_INFO data_info = {0};
                       data_info.ret_value = MMISMS_FINISH_OPER_ERR;
                       win_data_ptr->copy_call_back(win_data_ptr->user_data,&data_info);
                       MMK_CloseWin(win_id);
                       return recode;
                   }
    
                   copy_opt_data.copy_order_id = cur_copy_order;
                   MMISMSAPP_CopySms(copy_opt_data,MMISMS_CopyOneSmscallback,PNULL,PNULL);         
               }
               break;
               
               default:
                   break;
            }			
        }
        break;

    case MSG_CLOSE_WINDOW:
        SCI_FREE(win_data_ptr);    
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
//        if(PNULL != win_data_ptr)
        {
            MMISMS_COPY_RESULT_INFO data_info = {0};
            data_info.ret_value = MMISMS_FINISH_OPER;
            win_data_ptr->copy_call_back(win_data_ptr->user_data,&data_info);
        } 
                
        //SCI_TRACE_LOW:"copy cancel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24333_112_2_18_2_55_11_833,(uint8*)"");
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}
#endif

#define WATCHPOPWIN
#ifdef WATCHPOPWIN
/*---------------------------------------------------------------------------*
**                          WATCH-NOTE WAIT QUERY WIN                                 *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :Open note win
//  Parameter: [In] win_id
//             [In] text_id
//             [In] userFunc
//             [Out] NULL 
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenNoteWin(
                       MMI_WIN_ID_T            win_id,  
                       MMI_TEXT_ID_T           text_id,    
                       PROCESSMSG_FUNC    userFunc
                       )
{
    MMI_STRING_T text_str = {0};
    MMI_IMAGE_ID_T imageId = IMAGE_SMS_NOTE_BG;
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL,NULL,NULL};
    
    // if SMS box is opened, close it at first.
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMI_GetLabelTextByLang(text_id,&text_str);
#ifdef SCREEN_SHAPE_CIRCULAR
#ifdef ADULT_WATCH_SUPPORT
    Adult_WatchCOM_NoteWin_1Line_Toast_Enter( win_id,&text_str,PNULL,userFunc);
#endif
#else
    WatchCOM_NoteWin_1Line_Enter( win_id,&text_str,NULL,softkey_text,userFunc);
#endif
}

/*****************************************************************************/
//  Description :Open full screen note win
//  Parameter: [In] win_id
//             [In] text_id
//             [In] image_id
//             [In] userFunc
//             [Out] message info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenFullScreenNoteWin( 
                       MMI_WIN_ID_T          win_id,
                       MMI_TEXT_ID_T         text_id,    
                       MMI_IMAGE_ID_T       image_id,    
                       PROCESSMSG_FUNC   userFunc
                       )
{
    MMI_STRING_T text_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    MMI_GetLabelTextByLang(text_id,&text_str);
    WatchCOM_NoteWin_FullScreen_Enter( win_id,0,&text_str,image_id,0,softkey_text,userFunc );
}

/*****************************************************************************/
//  Description :Open waiting win
//  Parameter: [In] win_id
//             [In] image_id
//             [In] image_id
//             [In] userFunc
//             [Out] message info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenFullScreenWaitingWin( 
                       MMI_WIN_ID_T       win_id,
                       MMI_TEXT_ID_T      text_id,    
                       MMI_IMAGE_ID_T    image_id,   
                       PROCESSMSG_FUNC userFunc
                       )
{
    MMI_STRING_T text_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    MMI_GetLabelTextByLang(text_id,&text_str);
    WatchCOM_WaitingWin_FullScreen_Enter( win_id,&text_str,image_id,softkey_text,userFunc );
}

/*****************************************************************************/
//  Description :Open delete query win
//  Parameter: [In] win_id
//             [In] text_id
//             [In] left_image_id
//             [In] right_image_id
//             [In] userFunc
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenFullScreenQueryWin(
                       MMI_WIN_ID_T       win_id,
                       MMI_TEXT_ID_T      text_id,    
                       MMI_IMAGE_ID_T    left_image_id,   
                       MMI_IMAGE_ID_T    right_image_id,   
                       PROCESSMSG_FUNC userFunc)
{
    MMI_STRING_T text_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }  
    MMI_GetLabelTextByLang(text_id,&text_str);
#ifdef  ADULT_WATCH_SUPPORT
    AdultWatchCOM_QueryWin_2Btn_Enter( win_id,&text_str,0,0,left_image_id,right_image_id,userFunc );
#else
    WatchCOM_QueryWin_2Btn_Enter( win_id,&text_str,0,0,left_image_id,right_image_id,softkey_text,userFunc );
#endif
}
#endif
 
