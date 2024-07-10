/****************************************************************************
** File Name:      watch_sms_notifyconnection.c                                    *
** Author:         qi.liu1                                             *
** Date:           21/1/2021                                                *
** Copyright:      2021 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe main SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 02/2021      qi.liu1                  Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "watch_sms_main.h"
#include "watch_sms_presetlist.h"
#include "watch_sms_message.h"
#include "mmicom_trace.h"
#include "ctrllist_export.h"
#include "watch_common_list.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_menutable.h"
#include "mmisms_order.h"
#include "mmimms_export.h"
#include "mmisms_messagecountwin.h"
#include "mmisms_messageboxwin.h"
#include "mmiudisk_export.h"
#include "mmisms_appcontrol.h"
#include "mmisms_showsmswin.h"
#include "watch_sms_notifyconnect.h"
#include "mmisms_receive.h"
#include "mmisms_app.h"
#include "mmisms_export.h"
#include "watch_notifycenter.h"
#include "watch_notifycenter_id.h"
//#include "watch_sms_message.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS��ȫ�ֱ���
/**--------------------------------------------------------------------------*
 **                        STATIC DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : FindUnreadLongSMSFirstOrderId
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return] 
//  Author: qi.liu1
//  Note:  ͨ��ĳ�������Ŵ����new_order_id���ҵ������order_id
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T FindUnreadSMSRootOrderId(MMISMS_ORDER_ID_T  new_order_id);

/*****************************************************************************/
//  Description : UpdateNotifyHandle
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ����δ�������notify_handle
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T UpdateNotifyHandle(MMISMS_ORDER_ID_T  new_order_id,uint32 notify_handle);

/*****************************************************************************/
//  Description : MotifySMSNotifyHandle
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ͨ��cur_order_id��ȡnotify_handle
/*****************************************************************************/
//LOCAL uint32 GetNotifyHandleByOrderID(MMISMS_ORDER_ID_T  new_order_id);

/*****************************************************************************/
//  Description : GetOrderIdByNotifyHandle
//  Parameter: [In] notify_handle
//             [Out] find_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ͨ��notify_handle��ȡcur_order_id
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T GetOrderIdByNotifyHandle(uint32  notify_handle);
/*****************************************************************************/
//  Description : MotifySMSNotifyHandle
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ����δ�������notify_handle
/*****************************************************************************/
LOCAL void MotifySMSNotifyHandle(MMISMS_ORDER_ID_T  new_order_id, uint32 notify_handle);

LOCAL void ReadNotifySMSCallback(uint32 notify_handle);
  /*****************************************************************************/
 //  Description : EnterNotifySMSAppCallback
 //  Parameter: [In] None
 //             [Out] None
 //  Author: qi.liu1
 //  Note: Notification Enter SMS App
 /*****************************************************************************/
 LOCAL void EnterNotifySMSAppCallback(uint32 notify_handle);

 /*****************************************************************************/
//  Description : NotificationNewEventCallback
//  Parameter: [In] event_info_ptr
//             [Out] None
//  Author: qi.liu1
//  Note: when a new SMS come, do this callback
/*****************************************************************************/
LOCAL void NotificationNewEventCallback(MMIEVENT_INFO_T *event_info_ptr)
{
#if 1
    MMI_STRING_T subject = {0};//���Ÿ�Ҫ����
    MMI_STRING_T time = {0};//����ʱ��
    MMI_STRING_T name = {0}; //��ʾ�Ķ��ź������ϵ��
    MMISMS_READMSG_CONTENT_T content_ptr = {0};//������ϸ����2
    MMISMS_ORDER_ID_T  finded_order_id = PNULL;
    NOTIFYCENTER_NOTIFY_INFO_T sms_notify_info = {0};
    MMISMS_READ_MSG_T   read_result;
    MMISMS_NEW_SMS_INFO_T *new_sms_info_ptr = PNULL;
    MMISMS_STATE_T *cur_order_id = PNULL;
    SCI_DATE_T cur_date = {0};
    MMI_TM_T sms_time = {0};
    uint8 date_str[MMISET_DATE_STR_LEN + 1] = {0};
    uint8 time_string[MMIMESSAGE_DIAPLAY_LEN] = {0};
    wchar temp_arr1[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr2[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr3[MMISMS_PBNAME_MAX_LEN + 1] = {0};

    subject.wstr_ptr = temp_arr1;
    time.wstr_ptr    = temp_arr2;
    name.wstr_ptr    = temp_arr3;

    new_sms_info_ptr = (MMISMS_NEW_SMS_INFO_T *)event_info_ptr->param_ptr;
    if(PNULL == new_sms_info_ptr)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback new_sms_info_ptr = PNULL!");
        return;
    }

    TRACE_APP_MESSAGES("SMSNotify:NotificationNewEventCallback enter!");

    cur_order_id = new_sms_info_ptr->cur_chat_order_id;
    if(PNULL == cur_order_id)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback cur_order_id = PNULL!");
        return;
    }
    //���϶�������
    if ( MMISMS_TYPE_SMS == cur_order_id->flag.msg_type )
    {
        // 1.��ȡ����/��ϵ������
        MMISMS_GetNameInfo(cur_order_id, &name, MMISMS_PBNAME_MAX_LEN);

        // 2.���Ͻ��ն��Ÿ�Ҫ����
        finded_order_id = FindUnreadSMSRootOrderId(cur_order_id);//may be long sms
        if(PNULL == finded_order_id)
        {
            SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback record_id=%d,storage=%d finded_order_id=PNULL!",cur_order_id->record_id,cur_order_id->flag.storage);
            finded_order_id = cur_order_id;
        }
        subject.wstr_len  = MIN((cur_order_id->display_content.content_len), MMIMESSAGE_DIAPLAY_LEN);   
        MMI_WSTRNCPY(
            subject.wstr_ptr,
            MMIMESSAGE_DIAPLAY_LEN,
            finded_order_id->display_content.content, 
            subject.wstr_len,
            subject.wstr_len
            );
        TRACE_APP_MESSAGES("subject.length is %d", subject.wstr_len);

        // 3.���Ͻ��ն���ʱ��
        TM_GetSysDate(&cur_date);
        sms_time = MMIAPICOM_Second2Tm(cur_order_id->time);
        if ((sms_time.tm_mday == cur_date.mday)
        && (sms_time.tm_mon == cur_date.mon)
        && (sms_time.tm_year == cur_date.year))
        {
            MMIAPISET_FormatTimeStrByTime(sms_time.tm_hour, sms_time.tm_min, time_string, MMISET_DATE_STR_LEN);
            time.wstr_len = sprintf((char *)time_string, "%s", time_string);
        }
        else
        {
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

        // 4.��ȡ������ϸ���� (������)
        content_ptr.content_string.wstr_ptr = SCI_ALLOCAZ( ( MMISMS_R8_MESSAGE_LEN + 1 )*sizeof(wchar) );
        content_ptr.content_string.wstr_len = MMISMS_R8_MESSAGE_LEN;
        if(PNULL == content_ptr.content_string.wstr_ptr)
        {
            SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback content_string.wstr_ptr=PNULL!");
            return;
        }
        read_result = MMISMS_GetmsgResult();
        //�õ���ϸ��������
        MMISMS_GetReadSmsDispContent(&read_result,content_ptr.content_string.wstr_ptr,&content_ptr.content_string.wstr_len);
    }
    else
    {
        SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback SMS msg_type=%d",cur_order_id->flag.msg_type);
    }

    //����ǳ����ţ��ȴ�������Ϻ�֪ͨ��Notification
    if(cur_order_id->flag.is_concatenate_sms)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback max_num=%d,seq_num=%d",cur_order_id->longsms_max_num,cur_order_id->longsms_seq_num);
        //�����Ž������
        if(cur_order_id->longsms_max_num ==cur_order_id->longsms_seq_num)
        {
            MN_SMS_STORAGE_E     storage = 0;
            MN_SMS_RECORD_ID_T   record_id = 0;
            uint16 notify_handle = 0;//temp

            //�ҵ������ŵĵ�һ��Order Id
            finded_order_id = FindUnreadSMSRootOrderId(cur_order_id);
            if(PNULL == finded_order_id)
            {
                SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback receive record_id=%d finded_order_id=PNULL!",cur_order_id->record_id);
                finded_order_id = cur_order_id;
            }

	        sms_notify_info.notify_image = res_aw_info_mms;
	        SCI_MEMCPY(sms_notify_info.app_name_wstr,name.wstr_ptr,(((name.wstr_len > NOTIFYCENTER_APPNAME_LEN) ? NOTIFYCENTER_APPNAME_LEN : name.wstr_len)*sizeof(wchar)));
	        SCI_MEMCPY(sms_notify_info.title_wstr,name.wstr_ptr,(((name.wstr_len > NOTIFYCENTER_APPNAME_LEN) ? NOTIFYCENTER_APPNAME_LEN : name.wstr_len)*sizeof(wchar)));
	        SCI_MEMCPY(sms_notify_info.short_desc_wstr,subject.wstr_ptr,(((subject.wstr_len > NOTIFYCENTER_SHORTDESC_LEN) ? NOTIFYCENTER_SHORTDESC_LEN : subject.wstr_len)*sizeof(wchar)));
	        SCI_MEMCPY(sms_notify_info.time_wstr, time.wstr_ptr, (((time.wstr_len > NOTIFYCENTER_TIME_LEN) ? NOTIFYCENTER_TIME_LEN : time.wstr_len)*sizeof(wchar)));
	        SCI_MEMCPY(sms_notify_info.detail_desc_wstr,content_ptr.content_string.wstr_ptr,
				      (((content_ptr.content_string.wstr_len > NOTIFYCENTER_DETAILDESC_LEN) ? NOTIFYCENTER_DETAILDESC_LEN : content_ptr.content_string.wstr_len)*sizeof(wchar)));
	        sms_notify_info.cb_NotifyEnterApp = EnterNotifySMSAppCallback;
	        sms_notify_info.cb_NotifyRead = ReadNotifySMSCallback;
            //֪ͨNotification
	        notify_handle = NotifyCenter_Add(&sms_notify_info);
            MMK_SendMsg(NOTIFYLIST_WIN_ID,MSG_FULL_PAINT,PNULL);
            //��notify_handle����ű�ʶrecord_id��
            MotifySMSNotifyHandle(finded_order_id, notify_handle);
        }
        else
        {
            //������û�н������
            SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback long sms not receive ALL!");
        }
    }
    else //�������ͨ���ţ�ֱ��֪ͨ��Notification
    {
        uint16 notify_handle = 0;
        sms_notify_info.notify_image = res_aw_info_mms;
        SCI_MEMCPY(sms_notify_info.app_name_wstr,name.wstr_ptr,(((name.wstr_len > NOTIFYCENTER_APPNAME_LEN) ? NOTIFYCENTER_APPNAME_LEN : name.wstr_len)*sizeof(wchar)));
        SCI_MEMCPY(sms_notify_info.title_wstr,name.wstr_ptr,(((name.wstr_len > NOTIFYCENTER_APPNAME_LEN) ? NOTIFYCENTER_APPNAME_LEN : name.wstr_len)*sizeof(wchar)));
        SCI_MEMCPY(sms_notify_info.short_desc_wstr,subject.wstr_ptr,(((subject.wstr_len > NOTIFYCENTER_SHORTDESC_LEN) ? NOTIFYCENTER_SHORTDESC_LEN : subject.wstr_len)*sizeof(wchar)));
        SCI_MEMCPY(sms_notify_info.time_wstr, time.wstr_ptr, (((time.wstr_len > NOTIFYCENTER_TIME_LEN) ? NOTIFYCENTER_TIME_LEN : time.wstr_len)*sizeof(wchar)));
        SCI_MEMCPY(sms_notify_info.detail_desc_wstr,content_ptr.content_string.wstr_ptr,
			      (((content_ptr.content_string.wstr_len > NOTIFYCENTER_DETAILDESC_LEN) ? NOTIFYCENTER_DETAILDESC_LEN : content_ptr.content_string.wstr_len)*sizeof(wchar)));
        sms_notify_info.cb_NotifyEnterApp = EnterNotifySMSAppCallback;
        sms_notify_info.cb_NotifyRead = ReadNotifySMSCallback;
        SCI_TRACE_LOW("SMSNotify:NotificationNewEventCallback Normal sms");
		//֪ͨNotification
        notify_handle = NotifyCenter_Add(&sms_notify_info);
        MMK_SendMsg(NOTIFYLIST_WIN_ID,MSG_FULL_PAINT,PNULL);
        MotifySMSNotifyHandle(cur_order_id, notify_handle);
    }
#endif
}

 /*****************************************************************************/
//  Description : NotificationReadEventCallback
//  Parameter: [In] event_info_ptr
//             [Out] None
//  Author: qi.liu1
//  Note: SMS APP�����ŵ� callback
/*****************************************************************************/
LOCAL void NotificationReadEventCallback(MMIEVENT_INFO_T *event_info_ptr)
{
#if 1
    NOTIFYCENTER_RESULT_E ret = NOTIFYCENTER_ERROR;
    MMISMS_NEW_SMS_INFO_T *new_sms_info_ptr = PNULL;
    uint32 notify_handle = 0;

    new_sms_info_ptr = (MMISMS_NEW_SMS_INFO_T *)event_info_ptr->param_ptr;

    if(PNULL == new_sms_info_ptr)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationReadEventCallback new_sms_info_ptr = PNULL!");
        return;
    }

    if(PNULL == new_sms_info_ptr->cur_chat_order_id)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationReadEventCallback record_id=%d cur_chat_order_id = PNULL!",new_sms_info_ptr->record_id);
        return;
    }

    //ͨ��cur_order_id ��ȡnotify_handle
    notify_handle = WatchSMS_GetNotifyHandleByOrderID(new_sms_info_ptr->cur_chat_order_id);
    //delete notification
    ret = NotifyCenter_Delete(notify_handle);

	if(NOTIFYCENTER_SUCCESS != ret)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationReadEventCallback delete Error!");
    }
#endif
}

/*****************************************************************************/
//  Description : NotificationReadEventCallback
//  Parameter: [In] event_info_ptr
//             [Out] None
//  Author: qi.liu1
//  Note: SMS APP�����ŵ� callback
/*****************************************************************************/
LOCAL void NotificationDeleteEventCallback(MMIEVENT_INFO_T *event_info_ptr)
{
#if 1
    NOTIFYCENTER_RESULT_E ret = NOTIFYCENTER_ERROR;
    //MMISMS_NEW_SMS_INFO_T *new_sms_info_ptr = PNULL;
    uint32 notify_handle = 0;
    uint32 *data_change_ptr = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;

    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("SMSNotify:NotificationDeleteEventCallback event_info_ptr = PNULL!");
       return;
    }

    data_change_ptr = (uint32 *)event_info_ptr->param_ptr;
    if(PNULL == data_change_ptr)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationDeleteEventCallback data_change_ptr=PNULL!");
        return;
    }
    notify_handle = *data_change_ptr;
    if(0 == notify_handle)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationDeleteEventCallback notify_handle=0!");
        return;
    }

    //delete notification
    ret = NotifyCenter_Delete(notify_handle);
	if(NOTIFYCENTER_SUCCESS != ret)
    {
        SCI_TRACE_LOW("SMSNotify:NotificationDeleteEventCallback delete Error!");
    }
#endif
}
/*****************************************************************************/
//  Description : WatchSMS_Notification_Init
//  Parameter: [In] None
//             [In] None
//             [Out] None
//             [Return] handle
//  Author: qi.liu1
//  Note:��ʼ��,�����¶�����Ϣ
/*****************************************************************************/
PUBLIC void WatchSMS_Notification_Init(void)
{
    SCI_TRACE_LOW("SMSNotify:WatchSMS_Notification_Init enter!");
    //�����¶���EVENT
    MMIFW_SetEventListener(SMS_NEWSMS_EVENT, NotificationNewEventCallback, TRUE);
    //����APP������EVENT
    MMIFW_SetEventListener(SMS_READSMS_EVENT, NotificationReadEventCallback, TRUE);
    //EventListener --Delete
    MMIFW_SetEventListener(SMS_DELETESMS_EVENT, NotificationDeleteEventCallback, TRUE);
}

/*****************************************************************************/
//  Description : FindUnreadSMSRootOrderId
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ͨ��ĳ��(��)���Ŵ����new_order_id���ҵ������(��һ��)order_id
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T FindUnreadSMSRootOrderId(MMISMS_ORDER_ID_T  new_order_id)
{
    MMISMS_ORDER_ID_T  finded_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;

    if(PNULL == new_order_id)
    {
        SCI_TRACE_LOW("SMSNotify:FindUnreadSMSRootOrderId new_order_id = PNULL!");
        return finded_order_id;
    }

    SCI_TRACE_LOW("SMSNotify:FindUnreadSMSRootOrderId new record_id=%d",new_order_id->record_id);
    cur_order_id = MMISMS_GetFirstUnreadSmsId();//s_mmisms_order.mt_unread_head_ptr;
    while (cur_order_id != PNULL)
    {
        finded_order_id = cur_order_id;
        cur_long_order_id = cur_order_id; //�����Ż���ͨ����
        while (cur_long_order_id != PNULL)
        {
            if((cur_long_order_id->record_id== new_order_id->record_id)&&
                (cur_long_order_id->flag.storage == new_order_id->flag.storage))
            {
                SCI_TRACE_LOW("SMSNotify:FindUnreadSMSRootOrderId record_id=%d Find!",cur_long_order_id->record_id);
                return finded_order_id;
            }
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    SCI_TRACE_LOW("SMSNotify:FindUnreadSMSRootOrderId record_id=%d not Find!",new_order_id->record_id);
    return finded_order_id;
}

/*****************************************************************************/
//  Description : MotifySMSNotifyHandle
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ����δ�������notify_handle
/*****************************************************************************/
LOCAL void MotifySMSNotifyHandle(MMISMS_ORDER_ID_T  new_order_id, uint32 notify_handle)
{
    MMISMS_ORDER_ID_T  finded_order_id = PNULL;

    SCI_TRACE_LOW("SMSNotify:MotifySMSNotifyHandle notify_handle=%d",notify_handle);
    if(0 == notify_handle)
    {
        SCI_TRACE_LOW("SMSNotify:MotifySMSNotifyHandle notify_handle Error!");
        return;
    }
    if(PNULL == new_order_id)
    {
        SCI_TRACE_LOW("SMSNotify:MotifySMSNotifyHandle new_order_id = PNULL!");
        return;
    }

    finded_order_id = FindUnreadSMSRootOrderId(new_order_id);
    if(PNULL != finded_order_id)
    {
        finded_order_id->notify_handle = notify_handle;
    }
    else
    {
        SCI_TRACE_LOW("SMSNotify:MotifySMSNotifyHandle record_id=%d finded_order_id = PNULL!",new_order_id->record_id);
    }
    SCI_TRACE_LOW("SMSNotify:MotifySMSNotifyHandle storage = %d,record_id = %d,notify_handle =%d",new_order_id->flag.storage,new_order_id->record_id,notify_handle);
}

/*****************************************************************************/
//  Description : MotifySMSNotifyHandle
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ͨ��cur_order_id��ȡnotify_handle
/*****************************************************************************/
PUBLIC uint32 WatchSMS_GetNotifyHandleByOrderID(MMISMS_ORDER_ID_T  new_order_id)
{
    MMISMS_ORDER_ID_T  finded_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    uint32 notify_handle = 0;

    if(PNULL == new_order_id)
    {
        SCI_TRACE_LOW("SMSNotify:GetNotifyHandleByOrderID new_order_id = PNULL!");
        return notify_handle;
    }

    finded_order_id = FindUnreadSMSRootOrderId(new_order_id);

    if(PNULL == finded_order_id)
    {
        SCI_TRACE_LOW("SMSNotify:GetNotifyHandleByOrderID finded_order_id = PNULL!,record_id = %d, notify_handle = %d!",new_order_id->record_id,notify_handle);
        return notify_handle;
    }

    notify_handle = finded_order_id->notify_handle;

    SCI_TRACE_LOW("SMSNotify:GetNotifyHandleByOrderID record_id = %d, notify_handle = %d!",new_order_id->record_id,notify_handle);
    return notify_handle;
}

/*****************************************************************************/
//  Description : GetOrderIdByNotifyHandle
//  Parameter: [In] notify_handle
//             [Out] find_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  ͨ��notify_handle��ȡcur_order_id
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T GetOrderIdByNotifyHandle(uint32  notify_handle)
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;

    if(0 == notify_handle)
    {
        SCI_TRACE_LOW("SMSNotify:GetOrderIdByNotifyHandle notify_handle = PNULL!");
        return cur_order_id;
    }

    cur_order_id = MMISMS_GetFirstUnreadSmsId();
    while (cur_order_id != PNULL)
    {
        if(notify_handle == cur_order_id->notify_handle)
        {
            SCI_TRACE_LOW("SMSNotify:GetOrderIdByNotifyHandle record_id=%d Find!",cur_order_id->record_id);
            return cur_order_id;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    SCI_TRACE_LOW("SMSNotify:GetOrderIdByNotifyHandle notify_handle=%d not Find!",notify_handle);
    return cur_order_id;
}

#if 1
/*****************************************************************************/
//  Description : ReadNotifySMSCallback
//  Parameter: [In] notify_handle
//             [Out] 
//             [Return]
//  Author: qi.liu1
//  Note:  notification read sms callback
/*****************************************************************************/
LOCAL void ReadNotifySMSCallback(uint32 notify_handle)
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    NOTIFYCENTER_RESULT_E ret = NOTIFYCENTER_ERROR;

    g_mmisms_global.chat_oper_order_id = GetOrderIdByNotifyHandle(notify_handle);
    if(PNULL == g_mmisms_global.chat_oper_order_id)
    {
        SCI_TRACE_LOW("SMSNotify:WatchSMS_ReadNotifySMSCallback notify_handle = %d ,cur_order_id = PNULL!",notify_handle);
        return;
    }

    // to modify the status from to_be_read to READ
    MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS); //bug id:1646632
    MNSMS_UpdateSmsStatusEx(MN_SYS_SIM_1, g_mmisms_global.chat_oper_order_id->flag.storage, g_mmisms_global.chat_oper_order_id->record_id, MN_SMS_MT_READED);
    MMISMS_SetNeedUpdateInfo();

    if(g_mmisms_global.chat_oper_order_id)
	{
        //g_mmisms_global.chat_oper_order_id->flag.is_need_update = TRUE;
        cur_long_order_id = g_mmisms_global.chat_oper_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_need_update = TRUE;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
	}

    //Update Unread SMS Order list
    MMISMS_UpdateUnreadOrder();
    SCI_MEMSET(&g_mmisms_global.chat_oper_order_id,0x0,sizeof(MMISMS_ORDER_ID_T));
}
#endif

 /*****************************************************************************/
//  Description : EnterNotifySMSAppCallback
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note: Notification Enter SMS App
/*****************************************************************************/
LOCAL void EnterNotifySMSAppCallback(uint32 notify_handle)
{
    WatchSMS_MessageWin_Enter();
}

