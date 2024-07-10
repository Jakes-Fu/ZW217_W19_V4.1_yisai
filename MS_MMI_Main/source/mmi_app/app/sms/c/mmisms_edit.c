/****************************************************************************
** File Name:      mmisms_delete.c                                          *
** Author:         liming deng                                                        *
** Date:           8/23/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2007        liming.deng      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_edit.h"
#include "mmisms_delete.h"
#include "mmisms_send.h"
#include "mmisms_id.h"
#include "mmisms_order.h"
#include "mmipub.h"
#include "mmisms_save.h"
#include "mmisms_set.h"
#include "gui_ucs2b_converter.h"
#include "mmismsapp_interface.h"
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS的全局变量
extern const MMISMS_EMOJI_INFO_T s_emoji_image_text_convert_table[];

LOCAL MMISMS_EMOJI_DISPLAY_CONTENT_T *s_edit_convert_str = PNULL;
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleEditMoSmsSuccess(
                                  MN_DUAL_SYS_E         dual_sys,
                                  MN_SMS_RECORD_ID_T    record_id,    //IN: the SMS position 
                                  MN_SMS_STORAGE_E        storage,    //IN: where the short message is saved 
                                  MN_SMS_STATUS_E        status        //IN: the SMS state 
                                  );


/*****************************************************************************/
//     Description : save edit status to order list
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void UpdateEditStatusToOrderList(
                                       MN_DUAL_SYS_E         dual_sys,
                                       BOOLEAN                is_exist_addr,    //IN:
                                       MN_SMS_RECORD_ID_T    record_id,        //IN:
                                       MN_SMS_STORAGE_E        storage,        //IN:
                                       MN_SMS_STATUS_E        status            //IN:
                                       );

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleEditMoSmsSuccess(
                                  MN_DUAL_SYS_E         dual_sys,
                                  MN_SMS_RECORD_ID_T    record_id,    //IN: the SMS position 
                                  MN_SMS_STORAGE_E        storage,    //IN: where the short message is saved 
                                  MN_SMS_STATUS_E        status        //IN: the SMS state 
                                  )
{
    MMISMS_OPER_STATUS_E    oper_status        = MMISMS_GetCurrentOperStatus();
    MMISMS_UPDATE_INFO_T    *update_info_ptr= PNULL;
    BOOLEAN                    is_exist_err    = FALSE;
    BOOLEAN                    is_exist_addr    = FALSE;
    
    //SCI_TRACE_LOW:"MMISMS: HandleEditMoSmsSuccess record_id = %d, storage = %d, status = %d, oper_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_73_112_2_18_2_52_36_40,(uint8*)"dddd", record_id, storage, status, oper_status);
    switch (oper_status)
    {
    case MMISMS_UPDATE_SENDFAIL_SMS:
        is_exist_addr = TRUE;
        break;
        
    case MMISMS_UPDATE_NOSEND_SMS:
        is_exist_addr = FALSE;
        break;
        
    default:
        is_exist_err = TRUE;
        break;
    }
    
    // save to the order list.
    UpdateEditStatusToOrderList(
        dual_sys,
        is_exist_addr,
        record_id,
        storage,
        status
        );
    
    if (is_exist_err)
    {
        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);            
        return;
    }
    else
    {        
        update_info_ptr    = &(g_mmisms_global.update_info);
        
        if (update_info_ptr->current_num < update_info_ptr->update_num)
        {
            //continue to update the next SMS
            if (MMISMS_NO_ERR != MMISMS_UpdateNextSms( 
                dual_sys,
                g_mmisms_global.edit_content.dest_info.update_dest_addr, 
                GET_HEAD_MAX_NUM(&g_mmisms_global.edit_content),
                (uint8)(GET_CURRENT_SEQ_NUM(&g_mmisms_global.edit_content) + 1), 
                status,
                PNULL, FALSE
                ) 
                )
            {
                MMISMS_DisplayUpdatePrompt(TRUE);
				MMISMSAPP_HandleReplaceCnf(record_id,
                    storage,
                    TRUE);
            }
        }
        else if ((update_info_ptr->current_num == update_info_ptr->update_num)
            && (
            (update_info_ptr->total_num > update_info_ptr->update_num) || 
            (MMISMS_UPDATE_NOSEND_SMS == MMISMS_GetCurrentOperStatus() && (update_info_ptr->current_num < MMISMS_SPLIT_MAX_NUM) && (update_info_ptr->order_id_arr[update_info_ptr->current_num] != PNULL))
            )
            )
        {
            // have finish update and need to delete the other SMS
            if (!MMISMS_DeletePointedSMSById(update_info_ptr->order_id_arr[update_info_ptr->current_num]))
            {
                MMISMS_DisplayUpdatePrompt(TRUE);
				MMISMSAPP_HandleReplaceCnf(record_id,
                   storage,
                   TRUE);
            }
        }
        else 
        {
            MMISMS_ClearSaveInfo();
            MMISMS_DisplayUpdatePrompt(TRUE);            
            MMISMS_CloseEditWin();
            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID); 
			MMISMSAPP_HandleReplaceCnf(record_id,
                storage,
                TRUE);                 
        }
    }
}

/*****************************************************************************/
//     Description : save edit status to order list
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void UpdateEditStatusToOrderList(
                                       MN_DUAL_SYS_E         dual_sys,
                                       BOOLEAN                is_exist_addr,    //IN:
                                       MN_SMS_RECORD_ID_T    record_id,        //IN:
                                       MN_SMS_STORAGE_E        storage,        //IN:
                                       MN_SMS_STATUS_E        status            //IN:
                                       )
{
    BOOLEAN is_ucs2 = FALSE;
    uint8 seq_num = 0;
    uint8 content_len = 0;
    wchar content[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};   
    MMISMS_SAVE_INFO_T *save_info_ptr   = &(g_mmisms_global.save_info);
    MMISMS_UPDATE_INFO_T *update_info_ptr = &(g_mmisms_global.update_info);    
    MMISMS_STATE_T order_info  = {0};
    
    //SCI_TRACE_LOW:"MMISMS: UpdateEditStatusToOrderList(%d, %d, %d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_168_112_2_18_2_52_37_41,(uint8*)"dddd",is_exist_addr, record_id, storage, status);
    
    if ( 1 == g_mmisms_global.save_info.max_num
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
	     || !MMISMS_GetConcatenatedSMS()
#endif
        )
    {
        seq_num = 1;
        order_info.flag.is_concatenate_sms = FALSE;
        order_info.longsms_max_num = 1;
        order_info.longsms_seq_num = 1;
    }
    else
    {
        seq_num = save_info_ptr->seq_num;        
        order_info.flag.is_concatenate_sms = TRUE; 
        order_info.head_ref = save_info_ptr->ref_num;            // the reference number of SMS
        order_info.longsms_max_num = save_info_ptr->max_num;       //长短消息的最大sms数目;
        order_info.longsms_seq_num = save_info_ptr->seq_num;    //长短消息的每条sms的sequence number
    }
    
    order_info.number.bcd_number = save_info_ptr->number;
    
    is_ucs2 = MMISMS_InterceptMsgContent(
        seq_num,
        MMIMESSAGE_DIAPLAY_LEN,
        &content_len,
        content,
        PNULL
        );
    //insert the new MO message to the order list
    order_info.record_id = record_id;
    order_info.flag.dual_sys  = dual_sys;
    order_info.time = MMISMS_GetCurTime();
    
    //set the status flag according the storage and status
    order_info.flag.is_ucs2 = is_ucs2;
    order_info.flag.storage = storage;    
    order_info.flag.mo_mt_type = MMISMS_TranslateMNStatus2SFStatus(status, MMISMS_MO_TYPE, is_exist_addr); 
    order_info.flag.msg_type = MMISMS_TYPE_SMS;

    if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
    {
        order_info.flag.folder_type = MMISMS_FOLDER_SECURITY;
    }
    
    if(MMISMS_CurMsgIsLocked())
    {
        order_info.flag.is_locked = 0x1;
    }
    else
    {
        order_info.flag.is_locked = 0x0;
    }

    order_info.display_content.content_len = MIN(content_len,MMIMESSAGE_DIAPLAY_LEN);
    
    if(is_ucs2)
    {
        MMISMS_ConvertSmsContentToDisplayContent(&order_info.display_content, 
                                                 (uint8 *)content,
                                                 content_len * sizeof(wchar), 
                                                 MN_SMS_UCS2_ALPHABET); 
    }
    else         
    {
        MMI_WSTRNCPY(order_info.display_content.content,
                     MMIMESSAGE_DIAPLAY_LEN,
                     content,
                     content_len,
                      order_info.display_content.content_len);      
    }    
  

    if ((MMISMS_UPDATE_NOSEND_SMS == MMISMS_GetCurrentOperStatus() && update_info_ptr->current_num < MMISMS_SPLIT_MAX_NUM && update_info_ptr->order_id_arr[update_info_ptr->current_num] != PNULL && update_info_ptr->current_num < update_info_ptr->total_num)
    || (MMISMS_UPDATE_NOSEND_SMS != MMISMS_GetCurrentOperStatus() &&  update_info_ptr->current_num < update_info_ptr->total_num))
    {
        // update the selected position's SMS.
        order_info.flag.is_receive_all = FALSE;
        MMISMS_UpdateSmsInfoInOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num], &order_info);
        update_info_ptr->current_num++;
    }
    else
    {
        MMISMS_InsertSmsToOrder(&order_info);
        MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_ADD);    
        update_info_ptr->current_num++;
    }
}

/*****************************************************************************/
//     Description : to handle ps signal of APP_MN_EDIT_SMS_CNF
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleEditSMSCnf(
                                    DPARAM param
                                    )
{
    APP_MN_EDIT_SMS_CNF_T    *sig_ptr    = (APP_MN_EDIT_SMS_CNF_T*)param;
    
    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleEditSMSCnf sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_272_112_2_18_2_52_37_42,(uint8*)"");
        return;
    }    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleEditSMSCnf status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_275_112_2_18_2_52_37_43,(uint8*)"d", sig_ptr->status);
    switch (sig_ptr->status)
    {
    case MN_SMS_FREE_SPACE:
    case MN_SMS_MT_READED:
    case MN_SMS_MT_TO_BE_READ:
    case MN_SMS_MT_SR_READED:                // status report received and read
    case MN_SMS_MT_SR_TO_BE_READ:            // status report received to be read
        break;
        
    default:
        if ( MN_SMS_OPERATE_SUCCESS == sig_ptr->cause )
        {
            HandleEditMoSmsSuccess(
                sig_ptr->dual_sys,
                sig_ptr->record_id,
                sig_ptr->storage, 
                sig_ptr->status
                );
  
        }
        else
        {
            // 提示保存失败
            MMISMS_ClearSaveInfo();
            MMISMS_DisplayUpdatePrompt(FALSE);
            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);    
            MMISMSAPP_HandleReplaceCnf(sig_ptr->record_id,
                sig_ptr->storage,
                TRUE);
        }

        
        break;
    }
}

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InterceptMsgContent(                        //RETURN: TRUE: is ucs2
                                          uint8    seq_num,                //IN:
                                          uint8    max_len,                //IN:
                                          uint8    *intercpet_len_ptr,        //OUT:
                                          wchar    *intercpet_content_ptr,    //OUT:
                                          MMISMS_SEND_T *send_info_ptr
                                          )
{
    BOOLEAN      is_ucs2   = FALSE;

    uint16       start_pos = 0;
    int32        i = 0;
    uint16       max_convert_len = 0;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
   
    if (PNULL != send_info_ptr)
    {
        edit_content_ptr = &send_info_ptr->send_content;
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InterceptMsgContent PNULL !=  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_328_112_2_18_2_52_37_44,(uint8*)"");
    }
    else 
    {
        edit_content_ptr = &g_mmisms_global.edit_content;
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InterceptMsgContent PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_333_112_2_18_2_52_37_45,(uint8*)"");
    }
    if (PNULL == intercpet_len_ptr || PNULL == intercpet_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InterceptMsgContent intercpet_len_ptr intercpet_content_ptr send_message = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_337_112_2_18_2_52_37_46,(uint8*)"");
        return FALSE;
    }
   
    is_ucs2 = (MN_SMS_UCS2_ALPHABET == edit_content_ptr->send_message.alphabet) ? TRUE : FALSE;

    if(MMISMS_IsR8Language(edit_content_ptr->send_message.language) &&
       (MN_SMS_8_BIT_ALPHBET != edit_content_ptr->send_message.alphabet))        
    {
        g_mmisms_global.is_r8 = TRUE;
        g_mmisms_global.is_single_shift = edit_content_ptr->send_message.is_single_shift;
    }
    else
    {
        g_mmisms_global.is_r8 = FALSE;
        g_mmisms_global.is_single_shift = FALSE;
    }

    
    // count the start pos
    //start_pos = (uint16)((seq_num - 1) * split_len);
    for (i = 0; i < (int32)seq_num - 1; i++)
    {
        start_pos = start_pos + edit_content_ptr->send_message.split_sms_length[i];
    }
    if(is_ucs2)
    {
        max_convert_len = MIN(2*max_len,(edit_content_ptr->send_message.length - start_pos));
    }
    else
    {
        max_convert_len = MIN(max_len,(edit_content_ptr->send_message.length - start_pos));

    }
    SCI_TRACE_LOW("mmisms start_pos = %d,msg_len = %d,seq_num = %d",start_pos,edit_content_ptr->send_message.length,seq_num);

    MMISMS_ConvertValidDataToDispStr(
                                       intercpet_content_ptr,
                                       (uint16*)intercpet_len_ptr,
                                       edit_content_ptr->send_message.data + start_pos,
                                       max_convert_len,
                                       edit_content_ptr->send_message.alphabet,
                                       edit_content_ptr->send_message.language,
                                       TRUE,
                                       edit_content_ptr->send_message.is_single_shift
                                    );
	SCI_TRACE_LOW("MMISMS_InterceptMsgContent intercpet_len= %d",intercpet_len_ptr);

    if (MN_SMS_DEFAULT_ALPHABET == edit_content_ptr->send_message.alphabet)
    {
        if(g_mmisms_global.is_r8) 
        {
#if defined(__BIG_ENDIAN) || defined(__BigEndian)            
            is_ucs2 = TRUE;
#endif
            g_mmisms_global.is_r8 = FALSE;
        }
    }
    


    return (is_ucs2);
}

/*****************************************************************************/
//     Description : intercept class0 msg
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InterceptClass0MsgContent(                        //RETURN: TRUE: is ucs2
                                          uint8    seq_num,                //IN:
                                          uint8    max_len,                //IN:
                                          uint8    *intercpet_len_ptr,        //OUT:
                                          wchar    *intercpet_content_ptr    //OUT:
                                          )
{
    BOOLEAN        is_ucs2   = FALSE;
    uint16       max_convert_len = 0;
    
    if (PNULL == intercpet_len_ptr || PNULL == intercpet_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InterceptClass0MsgContent intercpet_len_ptr intercpet_content_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_461_112_2_18_2_52_37_47,(uint8*)"");
        return FALSE;
    }
    
    is_ucs2 = (MN_SMS_UCS2_ALPHABET == g_mmisms_global.class0_msg.read_content.alphabet) ? TRUE : FALSE;

    if(is_ucs2)
    {
        max_convert_len = MIN(2*max_len,g_mmisms_global.class0_msg.read_content.length);
    }
    else
    {
        max_convert_len = MIN(max_len,g_mmisms_global.class0_msg.read_content.length);
    }
    
    MMISMS_ConvertValidDataToDispStr(
                                       intercpet_content_ptr,
                                       (uint16*)intercpet_len_ptr,
                                       g_mmisms_global.class0_msg.read_content.data,
                                       max_convert_len,
                                       g_mmisms_global.class0_msg.read_content.alphabet,
                                       g_mmisms_global.class0_msg.read_content.language,
                                       TRUE,
                                       g_mmisms_global.class0_msg.is_single_shift
                                    );
    

    return (is_ucs2);
}

/*****************************************************************************/
//     Description : to get current time 
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetCurTime(void)
{
    uint32                        sc_time = 0;
    SCI_TIME_T                    time = {0};
    SCI_DATE_T                    date = {0};
    
    TM_GetSysTime(&time);
    TM_GetSysDate(&date);
    
    sc_time = MMIAPICOM_Tm2Second( 
        time.sec,
        time.min,
        time.hour,
        date.mday,
        date.mon,
        date.year
        );
    return sc_time;
}

/*****************************************************************************/
//    Description : MMISMS_ConvertToWstr
//    Global resource dependence :
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ConvertToWstr(UTF16 ch, wchar *data)
{
    uint16 high = 0;
    uint16 low = 0;

    high = UNICODE_TO_HIGH_SURROGATE(ch);
    low = UNICODE_TO_LOW_SURROGATE(ch);

    data[0] = high;
    data[1] = low;
}

/*****************************************************************************/
//    Description : to save the content which user input
//    Global resource dependence : g_mmisms_global
//    Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMessageContent( 
                                     uint16    length,            //IN:
                                     wchar     *content_ptr,    //IN:
                                     BOOLEAN   is_need_packet,
                                     MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr
                                     )
{
    MMISMS_CONTENT_T    *sms_content_ptr = PNULL;
    BOOLEAN is_ucs2 = FALSE;
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    MMISMS_ALPHABET_MANAGEMENT_E alpha_management = MMISMS_ALPHABET_MANAGEMENT_DEFAULT;
#endif
    wchar *convert_content_ptr = PNULL; 
    uint32 i = 0, j = 0;
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
    BOOLEAN is_r8 = FALSE;
    BOOLEAN is_single_shift = FALSE;
    uint8 *content_r8_ptr = PNULL;
    wchar convert_data[MMISMS_MAX_UCS2_MESSAGE_LEN + 1] = {0};
    uint16 convert_len = 0;
    wchar emoji[2] ={0};
    uint16 emoji_item_num = 0;

    if (PNULL == content_ptr || PNULL == edit_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMSR8: MMISMS_SetMessageContent content_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_587_112_2_18_2_52_38_48,(uint8*)"");
        return;
    }
    content_r8_ptr = SCI_ALLOCAZ( ( MMISMS_R8_MESSAGE_LEN + 1 )*sizeof(wchar) );
    if(PNULL == content_r8_ptr)
    {
        return;
    }

    for(i = 0; i < length; i++)
    {
        if(content_ptr[i] >= 0xE000 && content_ptr[i] <= 0xE2EF)
        {
            emoji_item_num = MMISMS_GetEmojiItemNum();
            for(j = 0;j < emoji_item_num; j++)
            {
                if(content_ptr[i] == s_emoji_image_text_convert_table[j].data)
                {
                    MMISMS_ConvertToWstr(s_emoji_image_text_convert_table[j].code, emoji);
                    MMIAPICOM_Wstrncat(convert_data, emoji, 2);
                    break;
                }
            }
        }
        else
        {
            MMIAPICOM_Wstrncat(convert_data, &content_ptr[i], 1);
        }
    }

    convert_len = MMIAPICOM_Wstrlen(convert_data);
    length = convert_len;

    convert_content_ptr = SCI_ALLOCAZ((length + 1)*sizeof(wchar));

    if (PNULL == convert_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_SetMessageContent No memory!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_601_112_2_18_2_52_38_49,(uint8*)"");

        SCI_FREE(content_r8_ptr);

        return;
    }

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    alpha_management = MMISMS_GetAlphabetManagement();

    if(!is_need_packet)
    {
        alpha_management = MMISMS_ALPHABET_MANAGEMENT_AUTO;
    }
    switch (alpha_management)
    {
    case MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET:        
        for (i = 0; i < length; i++)
        {
            convert_content_ptr[i] = MMISMS_ConvertChar2Alphbet(convert_data[i]);
        }
        break;

    case MMISMS_ALPHABET_MANAGEMENT_8BIT:
        for (i = 0; i < length; i++)
        {
            if (!MMIAPICOM_IsUcs2Char(convert_data[i]))
            {
                convert_content_ptr[i] = convert_data[i];
            }
            else
            {
                convert_content_ptr[i] = 0x20;
            }
        }
        break;
        
    default:
        MMI_WSTRNCPY(convert_content_ptr, length, convert_data, length, length);
        break;
    }
#else
    MMI_WSTRNCPY(convert_content_ptr, length, convert_data, length, length);
#endif

    SCI_MEMSET(&edit_content_ptr->send_message, 0, sizeof(MMISMS_CONTENT_T));
    sms_content_ptr = &(edit_content_ptr->send_message);

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    if (MMISMS_ALPHABET_MANAGEMENT_8BIT != alpha_management && MMISMS_ALPHABET_MANAGEMENT_UCS2 != alpha_management)
#endif
    {
        is_ucs2 = !MMIAPICOM_IsDefaultString(convert_content_ptr, length);

        if (is_ucs2)
        {
            language = MMISMS_R8_DecodeContentForLanguage(convert_content_ptr, length,&is_single_shift);
        }

        //SCI_TRACE_LOW:"MMISMSR8: MMISMS_SetMessageContent language=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_654_112_2_18_2_52_38_50,(uint8*)"d", language);

        sms_content_ptr->language = language;
        sms_content_ptr->is_single_shift = is_single_shift;
            
        if(MMISMS_IsR8Language(language))       
        {
            sms_content_ptr->length = MIN(length, MMISMS_R8_MESSAGE_LEN);
            is_ucs2 = FALSE;
            is_r8 = TRUE;
            length = MMISMS_NationalR82default(convert_content_ptr,content_r8_ptr,sms_content_ptr->length,language);      
        }
    }
    
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    if (MMISMS_ALPHABET_MANAGEMENT_AUTO == alpha_management)
#endif
    {
        sms_content_ptr->alphabet = (is_ucs2) ? MN_SMS_UCS2_ALPHABET : MN_SMS_DEFAULT_ALPHABET;
    }
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    else
    {
        switch (alpha_management)
        {
        case MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET:
            sms_content_ptr->alphabet = MN_SMS_DEFAULT_ALPHABET;
            break;

        case MMISMS_ALPHABET_MANAGEMENT_8BIT:
            sms_content_ptr->alphabet = MN_SMS_8_BIT_ALPHBET;
            break;

        case MMISMS_ALPHABET_MANAGEMENT_UCS2:
            sms_content_ptr->alphabet = MN_SMS_UCS2_ALPHABET;
            break;

        default:
            sms_content_ptr->alphabet = (is_ucs2) ? MN_SMS_UCS2_ALPHABET : MN_SMS_DEFAULT_ALPHABET;
            break;
        }
    }

    //SCI_TRACE_LOW:"MMISMS: MMISMS_SetMessageContent length=%d,alphabet=%d,is_need_packet=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_697_112_2_18_2_52_38_51,(uint8*)"ddd", length, sms_content_ptr->alphabet, is_need_packet);
#else
    //SCI_TRACE_LOW:"MMISMS: MMISMS_SetMessageContent length=%d,is_ucs2=%d,is_need_packet=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_699_112_2_18_2_52_38_52,(uint8*)"ddd", length, is_ucs2, is_need_packet);
#endif

    if(0 != length)
    {
        if (MN_SMS_DEFAULT_ALPHABET == sms_content_ptr->alphabet)
        {
            uint8 *ascii_content = PNULL;
            uint8 *default_data = PNULL;
            uint16 data_length = 0;
            
            ascii_content = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN * 2);
            if(PNULL == ascii_content)
            {
                SCI_FREE(content_r8_ptr);

//#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
                SCI_FREE(convert_content_ptr);
//#endif

                return;
            }         
            //SCI_MEMSET(ascii_content,0,MMISMS_MAX_MESSAGE_LEN);
            
            default_data = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN * 2);
            if(PNULL == default_data)
            {
                SCI_FREE(content_r8_ptr);
            
                SCI_FREE(ascii_content);

//#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
                SCI_FREE(convert_content_ptr);
//#endif

                return;
            } 
            SCI_MEMSET(default_data,0,MMISMS_MAX_MESSAGE_LEN * 2);

            sms_content_ptr->length = MIN(length, MMISMS_MAX_MESSAGE_LEN);

            if(is_r8)
            {
                SCI_MEMCPY(default_data,
                                     content_r8_ptr,
                                     sms_content_ptr->length);
                data_length = sms_content_ptr->length;
                sms_content_ptr->length = MIN(data_length, MMISMS_MAX_MESSAGE_LEN);
                SCI_MEMCPY(
                                        sms_content_ptr->data,
                                        default_data,
                                        sms_content_ptr->length 
                                        );
            }
            else
            {
                if (!is_need_packet)
                {
                    MMI_WSTRNTOSTR((uint8 *)ascii_content,
                                            MMISMS_MAX_MESSAGE_LEN,
                                            convert_content_ptr,
                                            MMISMS_MAX_MESSAGE_LEN,
                                            sms_content_ptr->length);
                    SCI_MEMCPY(
                        sms_content_ptr->data,
                        ascii_content,
                        sms_content_ptr->length 
                        );
                }
                else
                {
                    data_length = MMIAPICOM_Wchar2default(
//#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
                                convert_content_ptr,
// #else
//                                 content_ptr,
// #endif
                                ascii_content,
                                sms_content_ptr->length
                                );          

                    sms_content_ptr->length = MIN(data_length, MMISMS_MAX_MESSAGE_LEN);

                    SCI_MEMCPY(
                        sms_content_ptr->data,
                        ascii_content,
                        sms_content_ptr->length 
                        ); 
                }

            }

            SCI_FREE(ascii_content);

            SCI_FREE(default_data);
        }
        else if(MN_SMS_8_BIT_ALPHBET == sms_content_ptr->alphabet)
        {
            sms_content_ptr->length = MIN(length, MMISMS_SPLIT_MAX_NUM * MMISMS_MAX_8BIT_SPLIT_LEN);

            MMI_WSTRNTOSTR(
                sms_content_ptr->data,
                MMISMS_SPLIT_MAX_NUM * MMISMS_MAX_8BIT_SPLIT_LEN,
//#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
                convert_content_ptr,
// #else
//                 content_ptr,
// #endif
                MMISMS_SPLIT_MAX_NUM * MMISMS_MAX_8BIT_SPLIT_LEN,
                sms_content_ptr->length
                );
        }
        else
        {
            sms_content_ptr->length = MIN(length<<1, MMISMS_MAX_UCS2_MESSAGE_LEN);

//#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
            SCI_MEMCPY(
                sms_content_ptr->data,
                (uint8 *)convert_content_ptr,
                sms_content_ptr->length
                );
// #else
//             SCI_MEMCPY(
//                 sms_content_ptr->data,
//                 (uint8 *)content_ptr,
//                 sms_content_ptr->length
//                 );
// #endif
        }
        
        //SCI_TRACE_LOW:"MMISMS: MMISMS_SetMessageContent: SCI_MEMCPY length = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_830_112_2_18_2_52_38_53,(uint8*)"d", sms_content_ptr->length);
    }

    SCI_TRACE_BUF("MMISMS: MMISMS_SetMessageContent msg: ", sms_content_ptr->data, sms_content_ptr->length);

    SCI_FREE(content_r8_ptr);

//#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    SCI_FREE(convert_content_ptr);
//#endif
}

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InterceptReadMsgContent(                        //RETURN: TRUE: is ucs2
                                              MMISMS_OPER_STATUS_E oper_status,
                                              uint8    seq_num,
                                              uint8    max_len,                //IN:
                                              uint8    *intercpet_len_ptr,        //OUT:
                                              wchar    *intercpet_content_ptr    //OUT:
                                              )
{
    BOOLEAN        is_ucs2   = FALSE;

    uint16       start_pos = 0;
    int32        i = 0;
    uint16       max_convert_len = 0;
    
    if (PNULL == intercpet_len_ptr || PNULL == intercpet_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InterceptClass0MsgContent intercpet_len_ptr intercpet_content_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_EDIT_860_112_2_18_2_52_38_54,(uint8*)"");
        return FALSE;
    }    
    is_ucs2 = (MN_SMS_UCS2_ALPHABET ==  g_mmisms_global.read_msg.read_content.alphabet) ? TRUE : FALSE;

    for (i = 0; i < (int32)seq_num - 1; i++)
    {
        start_pos = start_pos + g_mmisms_global.read_msg.read_content.split_sms_length[i];
    }
    SCI_TRACE_LOW("mmisms start_pos = %d,msg_len = %d,seq_num = %d",start_pos,g_mmisms_global.read_msg.read_content.length,seq_num);
    if(is_ucs2)
    {
        max_convert_len = MIN(2*max_len,(g_mmisms_global.read_msg.read_content.length - start_pos));
    }
    else
    {
        max_convert_len = MIN(max_len,(g_mmisms_global.read_msg.read_content.length - start_pos));
    }
    if(MN_SMS_UCS2_ALPHABET == g_mmisms_global.read_msg.read_content.alphabet)
    {
        max_len *= sizeof(wchar);
        (*intercpet_len_ptr) = (uint8)MIN(max_len,  g_mmisms_global.read_msg.read_content.length - start_pos);

        //copy the valid data to the param
        SCI_MEMCPY(
            intercpet_content_ptr, 
            &g_mmisms_global.read_msg.read_content.data[start_pos],
            (*intercpet_len_ptr)
            );    
        (*intercpet_len_ptr) /= sizeof(wchar);
    }
    else
    {
        MMISMS_ConvertValidDataToDispStr(
                                       intercpet_content_ptr,
                                       (uint16*)intercpet_len_ptr,
                                       g_mmisms_global.read_msg.read_content.data + start_pos,
                                       max_convert_len,
                                       g_mmisms_global.read_msg.read_content.alphabet,
                                       g_mmisms_global.read_msg.read_content.language,
                                       TRUE,
                                       g_mmisms_global.read_msg.read_content.is_single_shift
                                    );
    }
    
    return (is_ucs2);
}

