/*****************************************************************************
** File Name:      mmisms_timermsg_control.c                                                
** Author:         hongbing.ju   
** Date:           2012/08/03
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to implement sms_timermsg_control function 
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/08/03     jixin.xu      Create                                    *
******************************************************************************/
#include "mmi_app_sms_trc.h"
#ifdef MMI_TIMERMSG_SUPPORT

/**---------------------------------------------------------------------------*
**                         INCLUDE FILES                                     *
**---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "window_parse.h"
//#include "mmk_app.h"
//#include "mmi_text.h"
//#include "guistatusbar.h"
//#include "guisoftkey.h"
//#include "guilistbox.h"
//#include "guilabel.h"
//#include "guidropdownlist.h"
//#include "mmi_image.h"
//#include "guitext.h"
//#include "guibutton.h"
//#include "guirichtext.h"
//#include "guires.h"
//#include "guiedit.h"
//#include "guiform.h"
//#include "guisetlist.h"
//#include "guilcd.h"
//#include "mmidisplay_data.h"
//#include "guifont.h"
//#include "mmi_theme.h"
//#include "mmienvset_export.h"
//#include "mmialarm_service.h"
//#include "mmi_appmsg.h"
//#include "mmimp3_export.h"
//#include "mmiset_wintab.h"
//#include "mmiset_display.h"
#include "mmiset_export.h"
#include "mmialarm_export.h"
#include "mmi_menutable.h"
#include "mmi_solarlunar.h"
#include "mmialarm_id.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmiacc_event.h"
#include "mmicalendar_export.h"
#include "mmi_mainmenu_export.h"
#include "mmi_solarlunar.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_app.h"
#include "mmisms_send.h"
#include "mmisms_read.h"
#include "mmisms_save.h"
#include "mmisms_edit.h"
#include "mmisms_timermsg.h"

#include "mmischedule_export.h"
#include "mmicc_export.h"
#include "mmipdp_export.h"
#include "mmiussd_export.h"
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmimms_id.h"
#endif 
#include "mmisms_appcontrol.h"
#include "mmismsapp_interface.h"
/**---------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define TIMERMSG_CHECK_TIMER_MSG_DIRECT_TIME_OUT 1000
#define TIMERMSG_CHECK_TIMER_MSG_TIME_OUT 5000

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL Variable DECLARE                        */
/*---------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS的全局变量

/*---------------------------------------------------------------------------*/
/*                          LOCAL VARIABLE DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMINV_SMS_TIMERMSG_T s_mmisms_timermsg_data[MMISMS_TIMERMSG_MAX_ITEM];//存放在nv中的数据。暂时不放到windata里
LOCAL uint8 s_mmisms_timer_sms_timer_id = 0;//srv层用
 
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************
// 	Description : get a free nv storage for saving smart event
// 	Global resource dependence : none
// 	Author: 
// 	Note:
*****************************************************************************/
LOCAL uint16 TIMERMSG_GetNVIndex(void);

/*****************************************************************************/
//  Description : Init Content Notify Info
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TIMERMSG_InitContentNotifyInfo(void);

/***************************************************************************/
// Description:Handle Check Timer Msg Timeout
// Global resource dependence : none
// Author: 
// Note:
//*************************************************************************/
LOCAL void HandleCheckTimerMsgTimeout(uint8 timer_id, uint32 param);

/*****************************************************************************
// 	Description : update nv storge for smart event
// 	Global resource dependence : sort table index and sms record id
// 	Author: 
// 	Note:
*****************************************************************************/
LOCAL uint16 TIMERMSG_UpdateNVItem(MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************/
//  Description : Cancel All Alarm
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TIMERMSG_CancelAllAlarm(void);

/*****************************************************************************/
//     Description : Update Alarm
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void TIMERMSG_UpdateAlarm(uint16 timer_index);

/*****************************************************************************/
// Description : is sms oper busy
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSmsOperBusy(void);


/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleTimerMsgWaitWinMsg(
                                             MMI_WIN_ID_T        win_id,    //IN:
                                             MMI_MESSAGE_ID_E    msg_id,    //IN:
                                             DPARAM                param    //IN:
                                             );

/**---------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************
// 	Description : get a free nv storage for saving smart event
// 	Global resource dependence : none
// 	Author: 
// 	Note:
*****************************************************************************/
LOCAL uint16 TIMERMSG_GetNVIndex(void)
{
    uint16 i = 0;

    for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
    {
        if (!s_mmisms_timermsg_data[i].flag.is_in_use)
        {
            return i;
        }
    }

    return MMISMS_TIMERMSG_MAX_ITEM;
}

/*****************************************************************************/
//  Description : Init Content Notify Info
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TIMERMSG_InitContentNotifyInfo(void)
{
    BOOLEAN event_flag = FALSE;
    uint32 sys_time = {0};
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
    BOOLEAN bret = FALSE;
    uint32 i = 0;
    
    sys_time = MMIAPICOM_GetCurTime();
    
    TIMERMSG_CancelAllAlarm();
    
    for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
    {
        MMINV_SMS_TIMERMSG_T *cursor = &s_mmisms_timermsg_data[i];
    
        if (cursor->flag.is_in_use)
        {
            uint32 task_time = 0;

            task_time = MMIAPICOM_Tm2Second(0,
                                                                    cursor->fast_table.minute,
                                                                    cursor->fast_table.hour,
                                                                    cursor->fast_table.day,
                                                                    cursor->fast_table.mon,
                                                                    cursor->fast_table.year);

            if (task_time > sys_time)
            {
                SCI_MEMSET(&content_event_info, 0x00, sizeof(content_event_info));
            
                content_event_info.event_type = ALMSVC_EVENT_OTHER;
                content_event_info.event_time.year = cursor->fast_table.year;
                content_event_info.event_time.mon = cursor->fast_table.mon;
                content_event_info.event_time.mday = cursor->fast_table.day;
                content_event_info.event_time.hour = cursor->fast_table.hour;
                content_event_info.event_time.min = cursor->fast_table.minute;
                content_event_info.event_time.sec = 0;
                content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMISMS_TIMERMSG_ContentNotifyCallback;
                
                //SCI_TRACE_LOW:"TIMERMSG_InitContentNotifyInfo %d-%d-%d %d:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_3108_112_2_18_2_53_44_491,(uint8*)"ddddd", content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,content_event_info.event_time.hour,content_event_info.event_time.min);
                
                //SCI_TRACE_LOW:"TIMERMSG_InitContentNotifyInfo before update cursor->event_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_3110_112_2_18_2_53_44_492,(uint8*)"d", cursor->event_id);
                
                if (cursor->event_id > 0)
                {
                    /* 最后一个参数表示是否立即启动ALM SERVICE， 放到循环后统一开启 */
                    bret = MMIALMSVC_UpdateServcice(&content_event_info, cursor->event_id, TRUE, TRUE);
                    
                    if (!bret)
                    {
                        cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
                    }
                }
                else
                {
                    cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
                }
                
                //SCI_TRACE_LOW:"TIMERMSG_InitContentNotifyInfo after update cursor->event_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_3127_112_2_18_2_53_44_493,(uint8*)"d", cursor->event_id);
                            
                event_flag = TRUE;
            }
        }
    }

    if (event_flag)
    {
        MMIALMSVC_SetAlmService();
    }

    MMI_WriteNVItem(MMINV_SMS_TIMERMSG, s_mmisms_timermsg_data);
    
    return;
}


/***************************************************************************/
// Description:Handle Check Timer Msg Timeout
// Global resource dependence : none
// Author: 
// Note:
//*************************************************************************/
LOCAL void HandleCheckTimerMsgTimeout(uint8 timer_id, uint32 param)
{
    if (0 != s_mmisms_timer_sms_timer_id
        && s_mmisms_timer_sms_timer_id == timer_id)
    {
        MMK_StopTimer(s_mmisms_timer_sms_timer_id);

        s_mmisms_timer_sms_timer_id = 0;

        MMISMS_TIMERMSG_ContentNotifyCallback(0, ALMSVC_EVENT_OTHER);
    }
}

/*****************************************************************************
// 	Description : update nv storge for smart event
// 	Global resource dependence : sort table index and sms record id
// 	Author: 
// 	Note:
*****************************************************************************/
LOCAL uint16 TIMERMSG_UpdateNVItem(MMISMS_STATE_T *order_info_ptr)
{
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = MMISMS_TIMERMSG_GetCurEvent();
    uint16 timer_index = 0;

    if (PNULL == event_fast_info_ptr || PNULL == order_info_ptr)
    {
        return MMISMS_TIMERMSG_MAX_ITEM;
    }

    for (timer_index = 0; timer_index < MMISMS_TIMERMSG_MAX_ITEM; timer_index++)
    {
        if (s_mmisms_timermsg_data[timer_index].flag.is_in_use
            && s_mmisms_timermsg_data[timer_index].record_id == order_info_ptr->record_id
            && s_mmisms_timermsg_data[timer_index].flag.storage == order_info_ptr->flag.storage
            && s_mmisms_timermsg_data[timer_index].flag.dual_sys == order_info_ptr->flag.dual_sys
            && s_mmisms_timermsg_data[timer_index].flag.mo_mt_type == order_info_ptr->flag.mo_mt_type)
        {
            break;
        }
    }

    if (MMISMS_TIMERMSG_MAX_ITEM == timer_index)
    {
        timer_index = TIMERMSG_GetNVIndex();
    }

    if (MMISMS_TIMERMSG_MAX_ITEM != timer_index)
    {
        s_mmisms_timermsg_data[timer_index].record_id = order_info_ptr->record_id;
        s_mmisms_timermsg_data[timer_index].flag.storage = order_info_ptr->flag.storage;
        s_mmisms_timermsg_data[timer_index].flag.dual_sys = order_info_ptr->flag.dual_sys;
        s_mmisms_timermsg_data[timer_index].flag.mo_mt_type = order_info_ptr->flag.mo_mt_type;
        s_mmisms_timermsg_data[timer_index].flag.is_in_use = TRUE;
        SCI_MEMCPY(&s_mmisms_timermsg_data[timer_index].fast_table, event_fast_info_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
    }

    MMI_WriteNVItem(MMINV_SMS_TIMERMSG, s_mmisms_timermsg_data);

    return timer_index;
}

/*****************************************************************************/
//  Description : Cancel All Alarm
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TIMERMSG_CancelAllAlarm(void)
{
    uint32 i = 0;

    for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
    {
        MMINV_SMS_TIMERMSG_T *cursor = &s_mmisms_timermsg_data[i];

        if (cursor->flag.is_in_use && cursor->event_id > 0)
        {
            MMIALMSVC_UnRegService(cursor->event_id, TRUE);
            cursor->event_id = 0;
        }
    }
}

/*****************************************************************************/
//     Description : Update Alarm
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void TIMERMSG_UpdateAlarm(uint16 timer_index)
{
    uint32 sys_time = {0};
    uint32 task_time = 0;
    BOOLEAN bret = FALSE;
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};

    if (timer_index < MMISMS_TIMERMSG_MAX_ITEM)
    {
        MMINV_SMS_TIMERMSG_T *cursor = &s_mmisms_timermsg_data[timer_index];

        sys_time = MMIAPICOM_GetCurTime();

        task_time = MMIAPICOM_Tm2Second(0,
                                                                cursor->fast_table.minute,
                                                                cursor->fast_table.hour,
                                                                cursor->fast_table.day,
                                                                cursor->fast_table.mon,
                                                                cursor->fast_table.year);

        if (task_time > sys_time)
        {
            SCI_MEMSET(&content_event_info, 0x00, sizeof(content_event_info));
        
            content_event_info.event_type = ALMSVC_EVENT_OTHER;
            content_event_info.event_time.year = cursor->fast_table.year;
            content_event_info.event_time.mon = cursor->fast_table.mon;
            content_event_info.event_time.mday = cursor->fast_table.day;
            content_event_info.event_time.hour = cursor->fast_table.hour;
            content_event_info.event_time.min = cursor->fast_table.minute;
            content_event_info.event_time.sec = 0;
            content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMISMS_TIMERMSG_ContentNotifyCallback;
            
            //SCI_TRACE_LOW:"TIMERMSG_UpdateAlarm %d-%d-%d %d:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_3205_112_2_18_2_53_44_494,(uint8*)"ddddd", content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,content_event_info.event_time.hour,content_event_info.event_time.min);
            
            //SCI_TRACE_LOW:"TIMERMSG_UpdateAlarm before update cursor->event_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_3207_112_2_18_2_53_44_495,(uint8*)"d", cursor->event_id);
            
            if (cursor->event_id > 0)
            {
                /* 最后一个参数表示是否立即启动ALM SERVICE， 放到循环后统一开启 */
                bret = MMIALMSVC_UpdateServcice(&content_event_info, cursor->event_id, TRUE, TRUE);
                
                if (!bret)
                {
                    cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
                }
            }
            else
            {
                cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
            }
            
            //SCI_TRACE_LOW:"TIMERMSG_UpdateAlarm after update cursor->event_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_3224_112_2_18_2_53_44_496,(uint8*)"d", cursor->event_id);
        }

        MMI_WriteNVItem(MMINV_SMS_TIMERMSG, s_mmisms_timermsg_data);
    }
}

/*****************************************************************************/
// Description : is sms oper busy
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSmsOperBusy(void)
{
    BOOLEAN is_sms_sending = MMK_IsOpenWin(MMISMS_SEND_ANIM_WIN_ID);
    BOOLEAN is_sms_deleting = MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID);
    BOOLEAN is_sms_saving = MMK_IsOpenWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
    BOOLEAN is_sms_reading = MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID);
#ifdef MMI_SMS_MOVE_SUPPORT      
    BOOLEAN is_sms_moving = MMK_IsOpenWin(MMISMS_MOVE_ANIM_WIN_ID);
#endif
#ifdef MMI_SMS_COPY_SUPPORT      
    BOOLEAN is_sms_copying = MMK_IsOpenWin(MMISMS_COPY_ANIM_WIN_ID);
#endif    
#if defined(MMI_SMS_EXPORT_SUPPORT)     
    BOOLEAN is_sms_exporting = MMK_IsOpenWin(MMISMS_EXPORT_ANIM_WIN_ID);
#endif
    //SCI_TRACE_LOW:"MMISMS: IsSmsOperBusy is_sms_sending=%d, is_sms_deleting=%d, is_sms_saving=%d, is_sms_reading=%d, is_sms_moving=%d, is_sms_exporting=%d, is_sms_copying=%d"
//    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2797_112_2_18_2_53_43_484,(uint8*)"ddddddd",is_sms_sending, is_sms_deleting, is_sms_saving, is_sms_reading, is_sms_moving, is_sms_exporting, is_sms_copying); //not show log

    if (is_sms_sending
        || is_sms_deleting
        || is_sms_saving
        || is_sms_reading
#ifdef MMI_SMS_MOVE_SUPPORT 		
        || is_sms_moving
#endif
#ifdef MMI_SMS_COPY_SUPPORT 		
        || is_sms_copying
#endif	
#if defined(MMI_SMS_EXPORT_SUPPORT) 	
	    || is_sms_exporting
#endif		
        )
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************
// 	Description : init of this module
// 	Global resource dependence : none
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_Init(void)
{
    MMI_ReadNVItem(MMINV_SMS_TIMERMSG, s_mmisms_timermsg_data);

    TIMERMSG_InitContentNotifyInfo();
}

/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_GetCurMsgNode(void)
{
    if (!MMISMS_GetIsTimerMsg())
    {
        return PNULL;
    }

    return &g_mmisms_global.curr_timer_msg_node.event_fast_info;
}

/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_GetCurEvent(void)
{
    if (!MMISMS_GetIsTimerMode())
    {
        return PNULL;
    }

    return &g_mmisms_global.curr_timer_mode_node.event_fast_info;
}

/*****************************************************************************
// 	Description : get current smart event instance
// 	Global resource dependence : current global timer msg index
// 	Author: jixin.xu
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_Win_GetCurEvent(void)
{
    return &g_mmisms_global.curr_timer_mode_node.event_fast_info;
}

/*****************************************************************************/
// 	Description : proc for timer msg after sending or saving
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_PostProc(MMISMS_STATE_T *order_info_ptr)
{
    if (PNULL == order_info_ptr)
    {
        return;
    }

    if (MMISMS_IsOrderOk())
    {
        if (MMISMS_GetIsTimerMode())
        {
            MMISMS_TIMERMSG_DeleteTimerMsg(order_info_ptr);

            if (MMISMS_BOX_SENDFAIL == MMISMS_TIMERMSG_GetCurrBoxType()
                && MMISMS_MO_SEND_FAIL == order_info_ptr->flag.mo_mt_type)
            {
                if ( 0 == order_info_ptr->flag.is_concatenate_sms
                    || 1 == order_info_ptr->longsms_seq_num)
                {
                    uint16 timer_index = 0;

                    timer_index = TIMERMSG_UpdateNVItem(order_info_ptr);

                    if (timer_index < MMISMS_TIMERMSG_MAX_ITEM)
                    {
                        TIMERMSG_UpdateAlarm(timer_index);
                    }
                }
            }
        }
    }
}

/*****************************************************************************/
// 	Description : check sending timer msg 
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note: if timer msg is ahead of current time, save the msg to send box
//        otherwise send the message directly   
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_CheckTimerSend(BOOLEAN is_send)
{
    if (MMISMS_GetIsTimerMode())
    {
        MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = MMISMS_TIMERMSG_GetCurEvent();
        MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_SENDFAIL;
        uint32 sys_time = {0};
        uint32 task_time = 0;
        uint16 i = 0;
        uint16 timer_sum = 0;

        if (PNULL == event_fast_info_ptr)
        {
            return FALSE;
        }

        sys_time = MMIAPICOM_GetCurTime();

        task_time = MMIAPICOM_Tm2Second(0,
                                                                event_fast_info_ptr->minute,
                                                                event_fast_info_ptr->hour,
                                                                event_fast_info_ptr->day,
                                                                event_fast_info_ptr->mon,
                                                                event_fast_info_ptr->year);

        if (task_time <= sys_time)
        {
            MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);

            return TRUE;
        }

        for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
        {
            if (s_mmisms_timermsg_data[i].flag.is_in_use)
            {
                timer_sum++;
            }
        }

        //modify send fail box
        if (MMISMS_MsgIsInSendFailBox())
        {
            MMIACC_SMART_EVENT_FAST_TABLE_T *cur_event_fast_info_ptr = PNULL;
            MMISMS_ORDER_ID_T order_id = PNULL;

            order_id = MMISMS_GetCurOperationOrderId();

            cur_event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);

            if (PNULL != cur_event_fast_info_ptr && timer_sum > 0)
            {
                timer_sum--;
            }
        }

        if (MMISMS_TIMERMSG_MAX_ITEM == timer_sum)
        {
            MMIPUB_OpenAlertWarningWin(TXT_TIMERMSG_FULL);

            return TRUE;
        }

        if (!is_send)
        {
            box_type = MMISMS_BOX_NOSEND;
        }

        MMISMS_TIMERMSG_SetCurrBoxType(box_type);

//        MMISMS_SaveAsDraft(MMISMS_GetCurSendDualSys());

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_CheckTimerMSG(BOOLEAN is_direct)
{
    //SCI_TRACE_LOW:"MMISMS: MMISMS_TIMERMSG_CheckTimerMSG s_mmisms_timer_sms_timer_id=%d, is_direct=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2838_112_2_18_2_53_43_485,(uint8*)"dd", s_mmisms_timer_sms_timer_id, is_direct);

    if (0 != s_mmisms_timer_sms_timer_id)
    {
        MMK_StopTimer(s_mmisms_timer_sms_timer_id);

        s_mmisms_timer_sms_timer_id = 0;
    }

    if (is_direct)
    {
        s_mmisms_timer_sms_timer_id = MMK_CreateTimerCallback(TIMERMSG_CHECK_TIMER_MSG_DIRECT_TIME_OUT, HandleCheckTimerMsgTimeout, NULL, FALSE);
    }
    else
    {
        s_mmisms_timer_sms_timer_id = MMK_CreateTimerCallback(TIMERMSG_CHECK_TIMER_MSG_TIME_OUT, HandleCheckTimerMsgTimeout, NULL, FALSE);
    }

    return TRUE;
}

/*****************************************************************************
// 	Description : Set current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_SetCurEvent(MMIACC_SMART_EVENT_FAST_TABLE_T* event_fast_info_ptr)
{
    if (PNULL != event_fast_info_ptr && MMISMS_GetIsTimerMode())
    {
        SCI_MEMCPY(&g_mmisms_global.curr_timer_mode_node.event_fast_info, event_fast_info_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
    }
}

/*****************************************************************************
// 	Description : get smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_GetEventByOrder(MMISMS_STATE_T *order_info_ptr)
{
   uint16 i = 0;

    if (PNULL != order_info_ptr)
    {
        for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
        {
            if (s_mmisms_timermsg_data[i].flag.is_in_use
                && s_mmisms_timermsg_data[i].record_id == order_info_ptr->record_id
                && s_mmisms_timermsg_data[i].flag.storage == order_info_ptr->flag.storage
                && s_mmisms_timermsg_data[i].flag.dual_sys == order_info_ptr->flag.dual_sys
                && s_mmisms_timermsg_data[i].flag.mo_mt_type == order_info_ptr->flag.mo_mt_type)
            {
                return &s_mmisms_timermsg_data[i].fast_table;
            }
        }
    }

    return PNULL;
}

/*****************************************************************************
// 	Description : Get Timer Msg By Index
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMINV_SMS_TIMERMSG_T* MMISMS_TIMERMSG_GetTimerMsgByIndex(uint16 timer_index)
{
    if (MMISMS_TIMERMSG_MAX_ITEM > timer_index)
    {
        return &s_mmisms_timermsg_data[timer_index];
    }

    return PNULL;
}

/*****************************************************************************
// 	Description : set box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_TYPE_E box_type)
{
    if (MMISMS_GetIsTimerMode())
    {
        g_mmisms_global.curr_timer_mode_node.box_type = box_type;
    }
}

/*****************************************************************************
// 	Description : get box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_TIMERMSG_GetCurrBoxType(void)
{
    if (MMISMS_GetIsTimerMode())
    {
        return g_mmisms_global.curr_timer_mode_node.box_type;
    }

    return MMISMS_BOX_NOSEND;
}

/*****************************************************************************
// 	Description : get a free nv storage for saving smart event
// 	Global resource dependence : none
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_DeleteTimerMsg(MMISMS_STATE_T *order_info_ptr)
{
    uint16 i = 0;

    if (PNULL == order_info_ptr)
    {
        return FALSE;
    }

    for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
    {
        if (s_mmisms_timermsg_data[i].flag.is_in_use
            && s_mmisms_timermsg_data[i].record_id == order_info_ptr->record_id
            && s_mmisms_timermsg_data[i].flag.storage == order_info_ptr->flag.storage
            && s_mmisms_timermsg_data[i].flag.dual_sys == order_info_ptr->flag.dual_sys
            && s_mmisms_timermsg_data[i].flag.mo_mt_type == order_info_ptr->flag.mo_mt_type)
        {
            if (0 != s_mmisms_timermsg_data[i].event_id)
            {
                MMIALMSVC_UnRegService(s_mmisms_timermsg_data[i].event_id, TRUE);
            }

            SCI_MEMSET(&s_mmisms_timermsg_data[i], 0x00, sizeof(MMINV_SMS_TIMERMSG_T));

            MMI_WriteNVItem(MMINV_SMS_TIMERMSG, s_mmisms_timermsg_data);

            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_SetIsTimerMsgReplace(BOOLEAN is_timer_msg_replace)
{
    SCI_TRACE_LOW("MMISMS:MMISMS_SetIsTimerMsgReplace:MMISMS_SetIsTimerMsgReplace = %d",is_timer_msg_replace);  
    g_mmisms_global.is_timer_msg_replace = is_timer_msg_replace;
}
      
/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsTimerMsgReplace(void)
{
    SCI_TRACE_LOW("MMISMS:MMISMS_GetIsTimerMsgReplace:MMISMS_GetIsTimerMsgReplace = %d",g_mmisms_global.is_timer_msg_replace);  
    return g_mmisms_global.is_timer_msg_replace;
}
      
/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_SetIsTimerMode(BOOLEAN is_timer_mode)
{
    //SCI_TRACE_LOW:"MMISMS:MMISMS_SetIsTimerMode: g_mmisms_global.is_timer_mode =%d, is_timer_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2601_112_2_18_2_53_43_480,(uint8*)"dd", g_mmisms_global.is_timer_mode, is_timer_mode);

    g_mmisms_global.is_timer_mode = is_timer_mode;

    if (!is_timer_mode)
    {
        SCI_MEMSET(&g_mmisms_global.curr_timer_mode_node, 0x00, sizeof(MMISMS_TIMERMSG_NODE))
    }
}
                  
/*****************************************************************************/
// 	Description : get timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsTimerMode(void)
{
    //SCI_TRACE_LOW:"MMISMS:MMISMS_GetIsTimerMode: g_mmisms_global.is_timer_mode =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2617_112_2_18_2_53_43_481,(uint8*)"d", g_mmisms_global.is_timer_mode);

    return g_mmisms_global.is_timer_mode;
}

/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_SetIsTimerMsg(BOOLEAN is_timer_msg)
{
    //SCI_TRACE_LOW:"MMISMS:MMISMS_SetIsTimerMsg: g_mmisms_global.is_timer_msg =%d, is_timer_msg=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2628_112_2_18_2_53_43_482,(uint8*)"dd", g_mmisms_global.is_timer_msg, is_timer_msg);

    g_mmisms_global.is_timer_msg = is_timer_msg;


    if (!is_timer_msg)
    {
        SCI_MEMSET(&g_mmisms_global.curr_timer_msg_node, 0x00, sizeof(MMISMS_TIMERMSG_NODE))
    }
}

/*****************************************************************************/
// 	Description : get timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsTimerMsg(void)
{
    //SCI_TRACE_LOW:"MMISMS:MMISMS_GetIsTimerMsg: g_mmisms_global.is_timer_msg =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2645_112_2_18_2_53_43_483,(uint8*)"d", g_mmisms_global.is_timer_msg);

    return g_mmisms_global.is_timer_msg;
}

/*****************************************************************************/
//     Description : 检查Timer Msg是否有效
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_CheckTimerMsgValid(void)
{
    MMISMS_CheckTimerMsgValid();

    MMI_WriteNVItem(MMINV_SMS_TIMERMSG, s_mmisms_timermsg_data);
}


LOCAL BOOLEAN TimerMsgReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMI_STRING_T msg_content = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 number [MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint16 num_len = 0;
    MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;

    // set the init SMS content
    MMISMS_GetSMSContent(&msg_content);

    MMISMS_SetMessageContent(msg_content.wstr_len, msg_content.wstr_ptr, TRUE, &g_mmisms_global.edit_content);

    num_len = MMISMS_GetOriginNum(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
    MMISMS_ClearDestNum();

    MMISMS_SetAddressToMessage(number, num_len, &g_mmisms_global.edit_content.dest_info.dest_list);

    MMISMS_SetSaveFlag(TRUE);
    MMISMS_SetCurSendDualSys(dual_sys);

    send_err_code = MMISMS_WintabSendTimerSMS(FALSE);

    return TRUE;
}


/*****************************************************************************/
// Description : Content Notify Callback
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_ContentNotifyCallback(MMIALMSVC_ID_T svc_id, MMIALMSVC_EVENT_TYPE_E svc_type)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    uint32 second = 0;
    uint32 i = 0;
    BOOLEAN is_need_update_nv = FALSE;
    BOOLEAN is_need_reset_timer_sms = TRUE;
    MMI_STRING_T   prompt_str = {0};
    MMI_STRING_T name = {0}; 
    wchar temp_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    BOOLEAN result = FALSE;

    name.wstr_ptr = temp_arr;

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    second = MMIAPICOM_Tm2Second(0, 
                                                        sys_time.min,
                                                        sys_time.hour,
                                                        sys_date.mday,
                                                        sys_date.mon,
                                                        sys_date.year);

    //SCI_TRACE_LOW:"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback svc_id=%d, svc_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2888_112_2_18_2_53_43_486,(uint8*)"dd", svc_id, svc_type);

    for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
    {
        MMINV_SMS_TIMERMSG_T *cursor = &s_mmisms_timermsg_data[i];
    
        if (cursor->flag.is_in_use
            /*&& (0 == svc_id || cursor->event_id == svc_id)*/)
        {
            uint32 task_time = 0;

            task_time = MMIAPICOM_Tm2Second(0,
                                                                    cursor->fast_table.minute,
                                                                    cursor->fast_table.hour,
                                                                    cursor->fast_table.day,
                                                                    cursor->fast_table.mon,
                                                                    cursor->fast_table.year);

            if (task_time <= second)
            {
                MMISMS_ORDER_ID_T cur_order_id = PNULL;
                MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)cursor->flag.dual_sys;
                BOOLEAN is_fly_mode = MMIAPISET_GetFlyMode();
                BOOLEAN is_order_ok = MMIAPISMS_IsOrderOk();
                BOOLEAN is_sms_sending = MMIAPISMS_IsSendingSMS();
                MN_DUAL_SYS_E cc_dual_sys = MMIAPICC_GetCallingSIM();
#ifdef MMI_GPRS_SUPPORT
                MN_DUAL_SYS_E pdp_dual_sys = MMIAPIPDP_GetPdpLinkSIM();
#endif
                MN_DUAL_SYS_E ussd_dual_sys = MMIAPIUSSD_GetUssdSIM();
                MN_DUAL_SYS_E ss_dual_sys = MMIAPISET_GetSSSIM();
                MMISMS_OPER_STATUS_E cur_oper_status = MMISMS_GetCurrentOperStatus();
                BOOLEAN is_cs_available = MMIAPIPHONE_IsCSAvail(dual_sys);

#ifdef MMI_GPRS_SUPPORT
                //SCI_TRACE_LOW:"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback dual_sys=%d, is_fly_mode=%d, is_order_ok=%d, is_sms_sending=%d, cc=%d, pdp=%d, ussd=%d, ss=%d, cur_oper_status=%d, is_cs_available=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2924_112_2_18_2_53_43_487,(uint8*)"dddddddddd",dual_sys, is_fly_mode, is_order_ok, is_sms_sending, cc_dual_sys, pdp_dual_sys, ussd_dual_sys, ss_dual_sys, cur_oper_status, is_cs_available);
#else
                //SCI_TRACE_LOW:"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback dual_sys=%d, is_fly_mode=%d, is_order_ok=%d, is_sms_sending=%d, cc=%d, ussd=%d, ss=%d, cur_oper_status=%d, is_cs_available=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2927_112_2_18_2_53_43_488,(uint8*)"ddddddddd",dual_sys, is_fly_mode, is_order_ok, is_sms_sending, cc_dual_sys, ussd_dual_sys, ss_dual_sys, cur_oper_status, is_cs_available);
#endif

                MMIALMSVC_UnRegService(cursor->event_id, TRUE);

                if ((MMI_DUAL_SYS_MAX != cc_dual_sys && dual_sys != cc_dual_sys)
#ifdef MMI_GPRS_SUPPORT
                    || (MMI_DUAL_SYS_MAX != pdp_dual_sys && dual_sys != pdp_dual_sys)
#endif
                    || (MMI_DUAL_SYS_MAX != ussd_dual_sys && dual_sys != ussd_dual_sys)
                    || (MMI_DUAL_SYS_MAX != ss_dual_sys && dual_sys != ss_dual_sys)
                )
                {
                    continue;
                }

                if (is_fly_mode
                    || !is_order_ok
                    || is_sms_sending
                    || (MMISMS_READ_FOR_TIMERMSG == cur_oper_status)
                    || !is_cs_available
                    || IsSmsOperBusy()
                )
                {
                    if (MMISMS_READ_FOR_TIMERMSG == cur_oper_status)
                    {
                        is_need_reset_timer_sms = FALSE;
                    }

                    //SCI_TRACE_LOW:"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback not allow send sms now"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2956_112_2_18_2_53_43_489,(uint8*)"");

                    break;
                }

                result = TRUE;

                MMISMS_SetCurOperationOrderIdByTimerMsg(cursor);
                cur_order_id = MMISMS_GetCurOperationOrderId();

                SCI_MEMSET(&g_mmisms_global.curr_timer_msg_node.event_fast_info, 0x00, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T))
                SCI_MEMCPY(&g_mmisms_global.curr_timer_msg_node.event_fast_info, &cursor->fast_table, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T))

                if (ALM_MODE_ONCE == cursor->fast_table.fre_mode)
                {
                    SCI_MEMSET(cursor, 0x00, sizeof(MMINV_SMS_TIMERMSG_T));
                }
                else
                {
                    BOOLEAN is_next_set = TRUE;
                    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
                    uint32 second_time = 0;

                    MMIAPIALM_SetNextAlarmTimeByCurrent(&cursor->fast_table);

                    second_time = MMIAPICOM_Tm2Second(0,
                                                                            cursor->fast_table.minute,
                                                                            cursor->fast_table.hour,
                                                                            cursor->fast_table.day,
                                                                            cursor->fast_table.mon,
                                                                            cursor->fast_table.year);

                    if (second_time <= task_time)
                    {
                        g_mmisms_global.curr_timer_msg_node.event_fast_info.fre_mode = ALM_MODE_ONCE;

                        is_next_set = FALSE;
                    }

                    if (is_next_set)
                    {
                        content_event_info.event_type = ALMSVC_EVENT_OTHER;
                        content_event_info.event_time.year = cursor->fast_table.year;
                        content_event_info.event_time.mon = cursor->fast_table.mon;
                        content_event_info.event_time.mday = cursor->fast_table.day;
                        content_event_info.event_time.hour = cursor->fast_table.hour;
                        content_event_info.event_time.min = cursor->fast_table.minute;
                        content_event_info.event_time.sec = 0;
                        content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMISMS_TIMERMSG_ContentNotifyCallback;
                        
                        //SCI_TRACE_LOW:"MMISMS_TIMERMSG_ContentNotifyCallback %d-%d-%d %d:%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_3010_112_2_18_2_53_43_490,(uint8*)"ddddd", content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,content_event_info.event_time.hour,content_event_info.event_time.min);

                        cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
                    }
                    else
                    {
                        SCI_MEMSET(cursor, 0x00, sizeof(MMINV_SMS_TIMERMSG_T));
                    }
                }

                is_need_update_nv = TRUE;
                is_need_reset_timer_sms = FALSE;

                if (PNULL != cur_order_id)
                {
                    if(MMISMSAPP_ReadTimerSms(cur_order_id,
                            TimerMsgReadSmsCallback,
                            PNULL,
                            PNULL))
                    {
                        MMI_GetLabelTextByLang(TXT_SMS_TIMER_MESSAGE, &prompt_str);
                        
                        MMISMS_GetNameInfo(cur_order_id, &name, MMISMS_PBNAME_MAX_LEN);
                        
                        MMIPUB_OpenWaitWin(2,&prompt_str,&name,PNULL,MMISMS_TIMERMSG_WAIT_WIN_ID,IMAGE_NULL,
                            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleTimerMsgWaitWinMsg);

                    }
                }
            }
        }
    }

    if (is_need_update_nv)
    {
        MMI_WriteNVItem(MMINV_SMS_TIMERMSG, s_mmisms_timermsg_data);
    }

    if (is_need_reset_timer_sms)
    {
        MMISMS_SetIsTimerMsg(FALSE);
    }

    return result;
}

/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleTimerMsgWaitWinMsg(
                                             MMI_WIN_ID_T        win_id,    //IN:
                                             MMI_MESSAGE_ID_E    msg_id,    //IN:
                                             DPARAM                param    //IN:
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifndef MMI_DUALMODE_ENABLE
    MMI_STRING_T prompt_str = {0};
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id,TXT_NULL,STXT_CANCEL,FALSE);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        if(MMISMS_READ_FOR_TIMERMSG == MMISMS_GetCurrentOperStatus())
        {
            MMISMS_InsertUnhandledSmsToOrder();
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_ADD);    
            MMK_CloseWin( win_id );
            //SCI_TRACE_LOW:"MMISMS_HandleWaitWinMsg:insert unhandle sms"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_30815_112_2_18_2_55_24_870,(uint8*)"");
        }
        else if(MMISMS_SEND_SMS == MMISMS_GetCurrentOperStatus()
            || MMISMS_SEND_SAVE_SMS == MMISMS_GetCurrentOperStatus())
        {
#ifndef MMI_DUALMODE_ENABLE
            MNSMS_CancelMoSmsEx(MMISMS_GetCurSendDualSys());
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
            MMIPUB_SetWaitWinText(TRUE,1, &prompt_str,PNULL,PNULL,win_id);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, TXT_NULL, TRUE);
            //SCI_TRACE_LOW:"MMISMS_HandleSendWaitWinMsg: Send cancel! dual_sys = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_30825_112_2_18_2_55_24_871,(uint8*)"d",MMISMS_GetCurSendDualSys());
#else
            MMISMS_ClearDestNum();
            MMK_CloseWin(win_id);
#endif 
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMISMS_SetIsTimerMsg(FALSE);
        MMISMS_TIMERMSG_CheckTimerMSG(FALSE);
        if (MMISMS_BOX_SENDFAIL == MMISMS_GetCurOperationType())
        {
            MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMK_CloseWin(MMISMS_COM_SELECT_SIM_WIN_ID);
#endif
        }
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

#endif
