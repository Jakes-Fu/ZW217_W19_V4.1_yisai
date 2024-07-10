/*****************************************************************************
** File Name:      mmisms_timermsg.c                                                
** Author:         hongbing.ju   
** Date:           2011/04/26
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to implement sms_timermsg function 
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/04/26     hongbing.ju      Create                                    *
******************************************************************************/
#include "mmi_app_sms_trc.h"
#ifdef MMI_TIMERMSG_SUPPORT

/**---------------------------------------------------------------------------*
**                         INCLUDE FILES                                     *
**---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
//#include "win.def"
#include "mmk_app.h"
#include "mmi_text.h"
//#include "sci_types.h"
#include "guistatusbar.h"
#include "guisoftkey.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmi_theme.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "mmialarm_export.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmimp3_export.h"
#include "mmi_solarlunar.h"
#include "mmialarm_id.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "guidropdownlist.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmiacc_event.h"
#include "mmicalendar_export.h"
#include "mmi_mainmenu_export.h"
#include "guitext.h"
#include "mmiset_wintab.h"
#include "guibutton.h"
#include "guirichtext.h"
#include "guires.h"
#include "guiedit.h"
#include "mmiset_display.h"
#include "guiform.h"
#include "guisetlist.h"
#include "mmi_solarlunar.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
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
#include "mmialarm_service.h"
#include "mmicc_export.h"
#include "mmipdp_export.h"
#include "mmiussd_export.h"
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmimms_id.h"
#endif                    
/**---------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define TIMERMSG_DROP_NUM MMISMS_TIMERMSG_MODE_MAX_TYPE
#define TIMERMSG_WEEK_DAY_NUM 7
#define TIMERMSG_BIG_MONTH_DAY 31
#define TIMERMSG_SCH_DECEMBER 12
#define TIMERMSG_QUERY_YEAR_TOP MMICOM_QUERY_MAX_YEAR    //可查询的最晚年份
#define TIMERMSG_CHECK_TIMER_MSG_DIRECT_TIME_OUT 1000
#define TIMERMSG_CHECK_TIMER_MSG_TIME_OUT 5000

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL Variable DECLARE                        */
/*---------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS的全局变量

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 

/*---------------------------------------------------------------------------*/
/*                          LOCAL VARIABLE DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_mmisms_weeks_is_selected[TIMERMSG_WEEK_DAY_NUM];
LOCAL MMINV_SMS_TIMERMSG_T s_mmisms_timermsg_data[MMISMS_TIMERMSG_MAX_ITEM];
LOCAL uint8 s_mmisms_timer_sms_timer_id = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get if start_date <= cur_date <= iend_date
//	Global resource dependence : 
//  Author:  zhaohui
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN TIMERMSG_IsInDateRange(     
                            SCI_DATE_T start_date, 
                            SCI_DATE_T end_date,
                            SCI_DATE_T cur_date
                         );



/*****************************************************************************/
//     Description : view and edit timer message
//    Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditTimerMsgWinMsg( 
                                       MMI_WIN_ID_T        win_id, 
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM                param
                                       );
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : view and edit dead line
//    Global resource dependence : 
//  Author:  rong.gu 
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsTimerMsgDeadlineWindow( 
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               );

/*****************************************************************************/
//  Description : 设置DEADLINE 日期时，显示DEADLINE 日期
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void DisplayTimerMsgDeadline(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T     ctrl_id );

/*****************************************************************************/
//     Description : view and edit timer message
//    Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsTimerMsgDateWindow( 
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               );

/*****************************************************************************/
//  Description : 设置日期时，显示系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTimerMsgDate(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T     ctrl_id );

/*****************************************************************************/
//     Description : view and edit timer message
//    Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsTimerMsgTimeWindow( 
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               );

/*****************************************************************************/
//  Description : 设置日期时，显示系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTimerMsgTime(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T     ctrl_id );
#endif
/*****************************************************************************/
// 	Description : CompareDate
//         compare a date to another , find smaller(earlier)
//	Global resource dependence : 
//  Author:        hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL uint16 CompareDate(
                         SCI_DATE_T first, 
                         SCI_DATE_T second
                         );

/*****************************************************************************/
// 	Description : if the deadline is earlier then the date ,it is not a valid deadline
//	Global resource dependence : 
//  Author:        hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsValidDeadline(
                              SCI_DATE_T deadline, 
                              SCI_DATE_T date
                              );

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinDateParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgPdaDateParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgPdaTimeParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  rong.gu
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgPdaDeadlineParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);
#endif

/*****************************************************************************/
// 	Description : set time param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinTimeParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

/*****************************************************************************/
// 	Description : set freq type param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinFreqTypeParam(void);

/*****************************************************************************/
// 	Description : set deadline  param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinDeadlineParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

/*****************************************************************************/
// 	Description : set week list param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinWeeklistParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_WIN_ID_T        win_id);

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinWeeklistDisplay(void);

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinDeadlineDisplay(void );

/*****************************************************************************/
//  Description : 设置SETLIST 的数据
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL void SetFreqSetList(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

/*****************************************************************************/
//  Description : save the frequency mode
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL void SaveTimerMsgEditWinFreqMode(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

/*****************************************************************************/
//  Description : save the date
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimerMsgEditWinDate(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : save the time
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimerMsgEditWinTime(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : save the deadline time
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimerMsgEditWinDeadlineDate(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : handle TimerMsg edit windows FormSwitchActive messages
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void  HandleTimerMsgEditFormSwitchActiveMsg(
                                            MMI_HANDLE_T win_id,
                                            uint32 ctrl_id
                                          );

/*****************************************************************************/
// 	Description : Set EditWin Button bg
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void TimerMsgSetEditWinButtonBg(MMI_HANDLE_T win_id, uint32 ctrl_id);

/*****************************************************************************/
// 	Description : save fre
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN TimerMsgSaveFreEditWin(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr);

/*****************************************************************************/
// 	Description : handle TimerMsg edit windows penok messages
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void HandleTimerMsgEditPENOKMsg(
                                            MMI_HANDLE_T win_id,
                                            uint32 ctrl_id
                                          );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void TimerMsgButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditSelectWeeksSoftkey(
                                            MMI_HANDLE_T win_id,
                                            uint32 ctrl_id,
                                            BOOLEAN is_update);

/*****************************************************************************
// 	Description : get a free nv storage for saving smart event
// 	Global resource dependence : none
// 	Author: 
// 	Note:
*****************************************************************************/
LOCAL uint16 TIMERMSG_GetNVIndex(void);

/*****************************************************************************
// 	Description : update nv storge for smart event
// 	Global resource dependence : sort table index and sms record id
// 	Author: 
// 	Note:
*****************************************************************************/
LOCAL uint16 TIMERMSG_UpdateNVItem(MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************/
// 	Description : HandleTimerMsgEditWinWebKeyMsg
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void HandleTimerMsgEditWinWebKeyMsg(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : init a timer SMS record
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void TIMERMSG_InitNewTimerMsg(
                                                                           uint16 year,
                                                                           uint8 month,
                                                                           uint8 day,
                                                                           MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr
                                                                           );

/*****************************************************************************/
//  Description : Init Content Notify Info
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TIMERMSG_InitContentNotifyInfo(void);

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

/***************************************************************************/
// Description:Handle Check Timer Msg Timeout
// Global resource dependence : none
// Author: 
// Note:
//*************************************************************************/
LOCAL void HandleCheckTimerMsgTimeout(uint8 timer_id, uint32 param);

/**---------------------------------------------------------------------------*
**                         CONSTANT DEFINITION                               *
**---------------------------------------------------------------------------*/
//编辑定时短消息的内容
WINDOW_TABLE( TIMERMSG_EDIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleEditTimerMsgWinMsg ),    
    WIN_ID( MMISMS_TIMERMSG_EDIT_WIN_ID ), 
    WIN_TITLE(TXT_SMS_TIMER_SETTINGS),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_TIME_FORM_CTRL_ID,MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_TIMERMSG_TIME_TITLE_LABEL_CTRL_ID,MMISMS_TIMERMSG_TIME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID, MMISMS_TIMERMSG_TIME_FORM_CTRL_ID),
#else
    CHILD_EDIT_TIME_CTRL(TRUE, MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID, MMISMS_TIMERMSG_TIME_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_DATE_FORM_CTRL_ID,MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_TIMERMSG_DATE_TITLE_LABEL_CTRL_ID,MMISMS_TIMERMSG_DATE_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID, MMISMS_TIMERMSG_DATE_FORM_CTRL_ID),
#else
    CHILD_EDIT_DATE_CTRL(TRUE, MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID, MMISMS_TIMERMSG_DATE_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_FREQ_FORM_CTRL_ID,MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_TIMERMSG_FREQ_TITLE_LABEL_CTRL_ID,MMISMS_TIMERMSG_FREQ_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID,MMISMS_TIMERMSG_FREQ_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID,MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISMS_TIMERMSG_EDITWIN_BUTTON1_CTRL_ID,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISMS_TIMERMSG_EDITWIN_BUTTON2_CTRL_ID,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISMS_TIMERMSG_EDITWIN_BUTTON3_CTRL_ID,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISMS_TIMERMSG_EDITWIN_BUTTON4_CTRL_ID,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISMS_TIMERMSG_EDITWIN_BUTTON5_CTRL_ID,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISMS_TIMERMSG_EDITWIN_BUTTON6_CTRL_ID,MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID,MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_TIMERMSG_DEADLINE_TITLE_LABEL_CTRL_ID,MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID, MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID),  
#else
    CHILD_EDIT_DATE_CTRL(TRUE,MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID,MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID),
#endif
    
#ifdef MMI_PDA_SUPPORT
     CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_COMMON_OK, TXT_NULL,MMICOMMON_SOFTKEY_CTRL_ID,MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID),
 #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif    
    END_WIN
}; 
#ifdef MMI_PDA_SUPPORT
// the window for set date
WINDOW_TABLE( MMISMS_TIMERMSG_DEADLINE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSmsTimerMsgDeadlineWindow ),    
        WIN_ID( MMISMS_TIMERMSG_DEADLINE_WIN_ID ),
        WIN_TITLE( TXT_CALENDAR_EDIT_DEADLINE ),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_DEADLINE_EDIT_FORM_CTRL_ID),    
        CHILD_EDIT_TOUCH_DATE_CTRL(TRUE,MMISMS_TIMERMSG_DEADLINE_DEADLINEEDITBOX_CTRL_ID,MMISMS_TIMERMSG_DEADLINE_EDIT_FORM_CTRL_ID),

        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for set date
WINDOW_TABLE( MMISMS_TIMERMSG_DATE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSmsTimerMsgDateWindow ),    
        WIN_ID( MMISMS_TIMERMSG_DATE_WIN_ID ),
        WIN_TITLE( TXT_START_DATE ),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_DATE_EDIT_FORM_CTRL_ID),    
        CHILD_EDIT_TOUCH_DATE_CTRL(TRUE,MMISMS_TIMERMSG_DATE_DATEEDITBOX_CTRL_ID,MMISMS_TIMERMSG_DATE_EDIT_FORM_CTRL_ID),

         WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};


// the window for set time
WINDOW_TABLE( MMISMS_TIMERMSG_TIME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSmsTimerMsgTimeWindow ),    
        WIN_ID( MMISMS_TIMERMSG_TIME_WIN_ID ),
        WIN_TITLE( TXT_TIMERMSG_SEND_TIME ),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_TIMERMSG_TIME_EDIT_FORM_CTRL_ID),    
        CHILD_EDIT_TOUCH_TIME_CTRL(TRUE,MMISMS_TIMERMSG_TIME_TIMEEDITBOX_CTRL_ID,MMISMS_TIMERMSG_TIME_EDIT_FORM_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif


/**---------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
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
// 	Description : HandleTimerMsgEditWinWebKeyMsg
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void HandleTimerMsgEditWinWebKeyMsg(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
    
    switch(ctrl_id)
    {
    case MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON1_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON2_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON3_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON4_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON5_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON6_CTRL_ID:
        if(s_mmisms_weeks_is_selected[ctrl_id-MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID])
        {
            s_mmisms_weeks_is_selected[ctrl_id-MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID] = FALSE;
        }
        else
        {
            s_mmisms_weeks_is_selected[ctrl_id-MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID] = TRUE;
        }

        SetTimerMsgEditSelectWeeksSoftkey(win_id, ctrl_id, TRUE);
        TimerMsgSetEditWinButtonBg(win_id,ctrl_id);
        break;

    case MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID:
    case MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID:
    case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
        {
            MMI_NOTIFY_T notify = {0};

            notify.src_handle = MMK_ConvertIdToHandle(ctrl_id);
            notify.src_id = ctrl_id;
            notify.code = FALSE;

            MMK_PostMsg(win_id, MSG_CTL_PENOK, &notify, sizeof(notify));
        }
        break;

    default:
        break;
    }
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgPdaDateParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{  
    MMI_STRING_T title = {0};
    uint8        date_buf[20] ={0};
    wchar       date_wchar_buf[20]={0};
    
    if (PNULL == event_fast_info_ptr)
    {
        return;
    }

    GUIFORM_SetStyle(MMISMS_TIMERMSG_DATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);    
    GUILABEL_SetTextById(MMISMS_TIMERMSG_DATE_TITLE_LABEL_CTRL_ID, TXT_START_DATE, FALSE);

    MMIAPISET_FormatDateStrByDateStyle(event_fast_info_ptr->year, event_fast_info_ptr->mon, event_fast_info_ptr->day, '.', date_buf, 20);

    MMI_STRNTOWSTR(date_wchar_buf, 10, date_buf, 10, 10);/*lint !e64*/
    title.wstr_len = 10;
    title.wstr_ptr = date_wchar_buf; 
    GUILABEL_SetText(MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID, &title, FALSE);
}

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgPdaTimeParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{    
    MMI_STRING_T        title = {0};
    unsigned char       time_buf[10] ={0};
    wchar               time_wchar_buf[10]={0};
    uint16              len = 0 ;

    if (PNULL == event_fast_info_ptr)
    {
        return;
    }
    
    GUIFORM_SetStyle(MMISMS_TIMERMSG_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISMS_TIMERMSG_TIME_TITLE_LABEL_CTRL_ID, TXT_TIMERMSG_SEND_TIME,FALSE);

    MMIAPISET_FormatTimeStrByTime(event_fast_info_ptr->hour, event_fast_info_ptr->minute, time_buf, 10);
    len =strlen((char*)time_buf);
    MMI_STRNTOWSTR(time_wchar_buf, len, time_buf, len, len);
    title.wstr_len = len;
    title.wstr_ptr = time_wchar_buf; 
    GUILABEL_SetText(MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID, &title, FALSE);
}

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  rong.gu
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgPdaDeadlineParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{  
    MMI_STRING_T title = {0};
    uint8        date_buf[20] ={0};
    wchar       date_wchar_buf[20]={0};
    
    if (PNULL == event_fast_info_ptr)
    {
        return;
    }

    GUIFORM_SetStyle(MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);    
    GUILABEL_SetTextById(MMISMS_TIMERMSG_DEADLINE_TITLE_LABEL_CTRL_ID, TXT_CALENDAR_EDIT_DEADLINE, FALSE);

    MMIAPISET_FormatDateStrByDateStyle(event_fast_info_ptr->end_year, event_fast_info_ptr->end_mon, event_fast_info_ptr->end_day, '.', date_buf, 20);

    MMI_STRNTOWSTR(date_wchar_buf, 10, date_buf, 10, 10);/*lint !e64*/
    title.wstr_len = 10;
    title.wstr_ptr = date_wchar_buf; 
    GUILABEL_SetText(MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID, &title, FALSE);
}
#endif

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinDateParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{    
    MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
    GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;
    
    if (PNULL == event_fast_info_ptr)
    {
        return;
    }

    GUIFORM_SetStyle(MMISMS_TIMERMSG_DATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    GUILABEL_SetTextById(MMISMS_TIMERMSG_DATE_TITLE_LABEL_CTRL_ID, TXT_START_DATE, FALSE);

    date_display_type = MMIAPISET_GetDateDisplayType();

    if (MMISET_DATE_YMD == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_YMD;
    }
    else if (MMISET_DATE_MDY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_MDY;
    }
    else if (MMISET_DATE_DMY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_DMY;
    }

    GUIEDIT_SetDateStyle(MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID,PNULL,&date_display_style,FALSE);
    GUIEDIT_SetDate(MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID,
                    event_fast_info_ptr->year,
                    event_fast_info_ptr->mon,
                    event_fast_info_ptr->day);
}

/*****************************************************************************/
// 	Description : set time param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinTimeParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{    
    GUIEDIT_TIME_STYLE_E time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_12HOURS;
    
    if (PNULL == event_fast_info_ptr)
    {
        return;
    }
    
    GUIFORM_SetStyle(MMISMS_TIMERMSG_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMISMS_TIMERMSG_TIME_TITLE_LABEL_CTRL_ID, TXT_TIMERMSG_SEND_TIME,FALSE);

    time_type = MMIAPISET_GetTimeDisplayType();

    if (MMISET_TIME_12HOURS == time_type)
    {
        time_style = GUIEDIT_TIME_STYLE_12;
    }
    else
    {
        time_style = GUIEDIT_TIME_STYLE_24;
    }        

    //set time display style
    GUIEDIT_SetTimeStyle(MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID,PNULL,&time_style,PNULL,FALSE);

    GUIEDIT_SetTime(
                MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID,
                event_fast_info_ptr->hour,
                event_fast_info_ptr->minute,
                0);
}

/*****************************************************************************/
// 	Description : set freq type param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinFreqTypeParam(void)
{        
    GUIFORM_SetStyle(MMISMS_TIMERMSG_FREQ_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISMS_TIMERMSG_FREQ_TITLE_LABEL_CTRL_ID, TXT_ALARM_EDIT_MODE,FALSE);

#ifdef MMISMS_TIMERMSG_ONCE_SUPPORT//只做了单次
    GUIFORM_SetChildDisplay(MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID, 
                        MMISMS_TIMERMSG_FREQ_FORM_CTRL_ID, 
                        GUIFORM_CHILD_DISP_HIDE);
#endif
}

/*****************************************************************************/
// 	Description : set deadline  param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinDeadlineParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{    
    MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
    GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;

    if (PNULL == event_fast_info_ptr)
    {
        return;
    }

    GUILABEL_SetTextById(MMISMS_TIMERMSG_DEADLINE_TITLE_LABEL_CTRL_ID, TXT_CALENDAR_EDIT_DEADLINE,FALSE);

    date_display_type = MMIAPISET_GetDateDisplayType();
    if (MMISET_DATE_YMD == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_YMD;
    }
    else if (MMISET_DATE_MDY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_MDY;
    }
    else if (MMISET_DATE_DMY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_DMY;
    }

    GUIEDIT_SetDateStyle(MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID, PNULL, &date_display_style, FALSE);       
    GUIEDIT_SetDate(MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID,
                event_fast_info_ptr->end_year,
                event_fast_info_ptr->end_mon,
                event_fast_info_ptr->end_day);   
}

/*****************************************************************************/
// 	Description : set week list param
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinWeeklistParam(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_WIN_ID_T        win_id)
{
    uint8 day_num = 0;
    uint8 week_day = 0;
    uint32 ctrl_id[]={MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID,
                          MMISMS_TIMERMSG_EDITWIN_BUTTON1_CTRL_ID,
                          MMISMS_TIMERMSG_EDITWIN_BUTTON2_CTRL_ID,
                          MMISMS_TIMERMSG_EDITWIN_BUTTON3_CTRL_ID,
                          MMISMS_TIMERMSG_EDITWIN_BUTTON4_CTRL_ID,
                          MMISMS_TIMERMSG_EDITWIN_BUTTON5_CTRL_ID,
                          MMISMS_TIMERMSG_EDITWIN_BUTTON6_CTRL_ID,
                    };
    GUIFORM_CHILD_WIDTH_T  width={0}; 
    GUI_BG_T bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};
    uint16 hor_space = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;
    GUI_BORDER_T border = {0};

    if (PNULL == event_fast_info_ptr)
    {
        return;
    }
    
    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#else
    disabled_font.font=SONG_FONT_16;
#endif

    bg.bg_type = GUI_BG_IMG;
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_CLOCK_ALARM_WEEK_ENABLED, win_id);

    width.type=GUIFORM_CHILD_WIDTH_FIXED;
    width.add_data = image_width;
    GUIFORM_SetSpace(MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID, &hor_space, PNULL);
    GUIFORM_SetMargin(MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID, 0);
    GUIFORM_PermitChildFont(MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID, FALSE);    
    border.type = GUI_BORDER_NONE;
    IGUICTRL_SetBorder(MMK_GetCtrlPtr(MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID), &border);

    for (day_num = 0; day_num < TIMERMSG_WEEK_DAY_NUM; day_num++)   
    {
        week_day = MMIAPISET_GetWeekDayByOffset(day_num );
        GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_HVMIDDLE);
        GUIBUTTON_SetTextId(ctrl_id[day_num],s_alm_week_text_id[ week_day ]);
        if (0 != (event_fast_info_ptr->fre & s_week_mask[week_day]) ) 
        {
            bg.img_id = IMAGE_CLOCK_ALARM_WEEK_ENABLED;
            s_mmisms_weeks_is_selected[day_num] = TRUE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;
            s_mmisms_weeks_is_selected[day_num] = FALSE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &disabled_font);
        }
        GUIBUTTON_SetBg(ctrl_id[day_num],&bg);
        GUIFORM_SetChildWidth(MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID, ctrl_id[day_num], &width);
        GUIBUTTON_PermitBorder(ctrl_id[day_num], FALSE);
        GUIBUTTON_SetOwnerDraw(ctrl_id[day_num], TimerMsgButtonOwnerDrawFun);
        bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        GUIBUTTON_SetRunSheen(ctrl_id[day_num], FALSE);
        GUIBUTTON_SetPressedBg(ctrl_id[day_num], &bg);
    }
}

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinWeeklistDisplay(void)
{
    uint32 setlist_index = 0;
    
    setlist_index=GUISETLIST_GetCurIndex(MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID);

    if (MMISMS_TIMERMSG_MODE_EVERYWEEK != setlist_index)
    {
        GUIFORM_SetChildDisplay(MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID, 
                            MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID, 
                            GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        
        GUIFORM_SetChildDisplay(MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID, 
                            MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID, 
                            GUIFORM_CHILD_DISP_NORMAL);
    }
}

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditWinDeadlineDisplay(void )
{
    uint32 setlist_index = 0;
    setlist_index = GUISETLIST_GetCurIndex(MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID);

    GUIFORM_SetStyle(MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    if (MMISMS_TIMERMSG_MODE_ONCE == setlist_index)
    {
        GUIFORM_SetChildDisplay(MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID, 
            MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID, 
            GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        GUIFORM_SetChildDisplay(MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID, 
            MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID, 
            GUIFORM_CHILD_DISP_NORMAL);
    }
}

/*****************************************************************************/
//  Description : 设置SETLIST 的数据
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL void SetFreqSetList(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{
    uint32 i = 0;
    MMISMS_TIMERMSG_MODE_TYPE_E index = MMISMS_TIMERMSG_MODE_ONCE;
    MMI_TEXT_ID_T text_arr[TIMERMSG_DROP_NUM] = {
                                                                                        TXT_ALARM_MODE_ONCE,
                                                                                        TXT_ALARM_MODE_EVERYDAY,
                                                                                        TXT_ALARM_MODE_WEEK,
                                                                                        TXT_ALARM_MODE_MONTH,
                                                                                        TXT_ALARM_MODE_YEAR
                                                                                        };

    if (PNULL == event_fast_info_ptr)
    {
        return;
    }

    //初始化下拉列表
    if (!event_fast_info_ptr->is_on)
    {
        //index = MMISMS_TIMERMSG_MODE_SEND_NO;
    }
    else
    {
        switch(event_fast_info_ptr->fre_mode)
        {
        case ALM_MODE_ONCE:
            index = MMISMS_TIMERMSG_MODE_ONCE;
            break;

        case ALM_MODE_EVERYDAY:
            index = MMISMS_TIMERMSG_MODE_EVERYDAY;
            break;

        case ALM_MODE_EVERYWEEK:
            index = MMISMS_TIMERMSG_MODE_EVERYWEEK;
            break;

        case ALM_MODE_EVERYMONTH:
            index = MMISMS_TIMERMSG_MODE_EVERYMONTH;;
            break;

        case ALM_MODE_EVERYYEAR:
            index = MMISMS_TIMERMSG_MODE_EVERYYEAR;
            break;

        default:
            break;
        }
    }

    for (i = 0; i < TIMERMSG_DROP_NUM; i++)
    {
        GUISETLIST_AddItemById(MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID, text_arr[i]);
    }

    GUISETLIST_SetCurIndex(MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID, (uint32)index);
    GUISETLIST_SetTitleTextId(MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID, TXT_ALARM_EDIT_MODE);
}

/*****************************************************************************/
//  Description : save the frequency mode
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL void SaveTimerMsgEditWinFreqMode(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{
    if (PNULL == event_fast_info_ptr)
    {
        return;
    }
    
    event_fast_info_ptr->is_on = TRUE;
    switch(GUISETLIST_GetCurIndex(MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID))
    {
    case MMISMS_TIMERMSG_MODE_ONCE:
        event_fast_info_ptr->fre_mode = ALM_MODE_ONCE;
        break;

    case MMISMS_TIMERMSG_MODE_EVERYDAY:
        event_fast_info_ptr->fre_mode  = ALM_MODE_EVERYDAY;
        break;

    case MMISMS_TIMERMSG_MODE_EVERYWEEK:
        event_fast_info_ptr->fre_mode  = ALM_MODE_EVERYWEEK;
        break;

    case MMISMS_TIMERMSG_MODE_EVERYMONTH:
        event_fast_info_ptr->fre_mode  = ALM_MODE_EVERYMONTH;
        break;

    case MMISMS_TIMERMSG_MODE_EVERYYEAR:
        event_fast_info_ptr->fre_mode  = ALM_MODE_EVERYYEAR;
        break;

    default:
        break;
    }  
}
/*****************************************************************************/
//  Description : save the date
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimerMsgEditWinDate(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T ctrl_id)
{
    MMISET_DATE_T date = {0};

    if (PNULL == event_fast_info_ptr)
    {
        return FALSE;
    }
    
    GUIEDIT_GetDate(ctrl_id, 
                    &date.date_year,
                    &date.date_month,
                    &date.date_day);
    if (MMIAPICOM_CheckDateIsValid(date))
    {
        event_fast_info_ptr->start_year = date.date_year;
        event_fast_info_ptr->start_mon = date.date_month;
        event_fast_info_ptr->start_day = date.date_day;
        event_fast_info_ptr->year = date.date_year;
        event_fast_info_ptr->mon = date.date_month;
        event_fast_info_ptr->day = date.date_day;
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : save the time
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimerMsgEditWinTime(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T ctrl_id)
{
    MMISET_TIME_T time = {0};

    if (PNULL == event_fast_info_ptr)
    {
        return FALSE;
    }
    
    GUIEDIT_GetTime(ctrl_id, 
                    &time.time_hour, 
                    &time.time_minute,
                    PNULL);

    if (MMIAPICOM_CheckTimeIsValid(time))
    {
        event_fast_info_ptr->hour = time.time_hour;
        event_fast_info_ptr->minute = time.time_minute;
    }
    else 
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : save the deadline time
//  Global resource dependence : 
//  Author:hongbing.ju
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimerMsgEditWinDeadlineDate(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T ctrl_id)
{
    MMISET_DATE_T date = {0};
    SCI_DATE_T second_date = {0};
    SCI_DATE_T deadline = {0};

    if (PNULL == event_fast_info_ptr)
    {
        return FALSE;
    }

    GUIEDIT_GetDate(ctrl_id, 
                    &date.date_year,
                    &date.date_month,
                    &date.date_day);

    if (MMIAPICOM_CheckDateIsValid(date))
    {
        event_fast_info_ptr->end_year = date.date_year;
        event_fast_info_ptr->end_mon = date.date_month;
        event_fast_info_ptr->end_day = date.date_day;            

        //when deadline earlier than event time,alert!
        second_date.year = event_fast_info_ptr->year;
        second_date.mon  = event_fast_info_ptr->mon;
        second_date.mday = event_fast_info_ptr->day;    
        deadline.year = event_fast_info_ptr->end_year;
        deadline.mon= event_fast_info_ptr->end_mon;
        deadline.mday= event_fast_info_ptr->end_day;

        if (!IsValidDeadline(deadline, second_date))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
            return FALSE;
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : view and edit timer SMS
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditTimerMsgWinMsg(
                                       MMI_WIN_ID_T        win_id, 
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = &g_mmisms_global.curr_timer_mode_node.event_fast_info;
    MMI_CTRL_ID_T ctrl_id = 0;
    SCI_DATE_T cur_date = {0};
    SCI_TIME_T cur_time = {0};
    SCI_DATE_T second_date = {0};
    SCI_TIME_T second_time = {0};
    SCI_DATE_T start_date = {0};
    SCI_TIME_T start_time = {0};
    SCI_DATE_T deadline = {0};
    SCI_DATE_T min_date = {0};
    SCI_TIME_T  min_time = {0};

    //SCI_TRACE_LOW:"HandleEditTimerMsgWinMsg: msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_1355_112_2_18_2_53_40_476,(uint8*)"d",msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        TM_GetSysDate(&cur_date);
        #ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID, GUIFORM_TYPE_TP);
        GUISETLIST_SetFontAlign(MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID, ALIGN_LEFT);
        #endif
        if (!MMISMS_GetIsTimerMode())
        {
            TIMERMSG_InitNewTimerMsg(cur_date.year,
                cur_date.mon,
                cur_date.mday,
                event_fast_info_ptr);
        }
        SetFreqSetList(event_fast_info_ptr);
#ifdef MMI_PDA_SUPPORT
        SetTimerMsgPdaDateParam(event_fast_info_ptr);
        SetTimerMsgPdaTimeParam(event_fast_info_ptr);
        SetTimerMsgEditWinFreqTypeParam();
        SetTimerMsgPdaDeadlineParam(event_fast_info_ptr);
        //SetTimerMsgEditWinDeadlineParam(event_fast_info_ptr);
        SetTimerMsgEditWinWeeklistParam(event_fast_info_ptr, win_id);
        SetTimerMsgEditWinDeadlineDisplay();
        SetTimerMsgEditWinWeeklistDisplay();
#else
        SetTimerMsgEditWinDateParam(event_fast_info_ptr);
        SetTimerMsgEditWinTimeParam(event_fast_info_ptr);
        SetTimerMsgEditWinFreqTypeParam();
        SetTimerMsgEditWinDeadlineParam(event_fast_info_ptr);
        SetTimerMsgEditWinWeeklistParam(event_fast_info_ptr, win_id);
        SetTimerMsgEditWinDeadlineDisplay();
        SetTimerMsgEditWinWeeklistDisplay();
#endif        
        MMK_SetAtvCtrl(win_id, MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_GET_FOCUS:
        SetTimerMsgPdaDateParam(event_fast_info_ptr);
        SetTimerMsgPdaTimeParam(event_fast_info_ptr);
        SetTimerMsgPdaDeadlineParam(event_fast_info_ptr);
        break;
#endif    

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
            
    case MSG_CTL_SETLIST_SWITCH:
        SetTimerMsgEditWinDeadlineDisplay();
        SetTimerMsgEditWinWeeklistDisplay();
        break;
        
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        HandleTimerMsgEditFormSwitchActiveMsg(win_id, ctrl_id);
        break;
    
    case MSG_CTL_OK:
    case MSG_APP_OK:        
#ifndef MMI_PDA_SUPPORT
        if (!SaveTimerMsgEditWinTime(event_fast_info_ptr,MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID))
        {
            break;
        }
        if (!SaveTimerMsgEditWinDate(event_fast_info_ptr,MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID))
        {
            break;
        }
#endif

        SaveTimerMsgEditWinFreqMode(event_fast_info_ptr);     
        
        if (ALM_MODE_EVERYWEEK == event_fast_info_ptr->fre_mode)
        {
            if (!TimerMsgSaveFreEditWin(event_fast_info_ptr))
            {
                break;
            }
        }
        
        if (ALM_MODE_ONCE != event_fast_info_ptr->fre_mode && ALM_MODE_NO != event_fast_info_ptr->fre_mode)
        {
#ifndef MMI_PDA_SUPPORT
            if (!SaveTimerMsgEditWinDeadlineDate(event_fast_info_ptr, MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID))                        
            {
                break;
            }
#endif
        }
        else
        {
            event_fast_info_ptr->end_year = event_fast_info_ptr->year;
            event_fast_info_ptr->end_mon = event_fast_info_ptr->mon;
            event_fast_info_ptr->end_day = event_fast_info_ptr->day;
        }
        
        event_fast_info_ptr->start_year = event_fast_info_ptr->year;
        event_fast_info_ptr->start_mon  = event_fast_info_ptr->mon;
        event_fast_info_ptr->start_day = event_fast_info_ptr->day;		

        TM_GetSysDate(&cur_date);
        TM_GetSysTime(&cur_time);

        second_date.year = event_fast_info_ptr->year;
        second_date.mon  = event_fast_info_ptr->mon;
        second_date.mday = event_fast_info_ptr->day;
        second_time.hour = event_fast_info_ptr->hour;
        second_time.min = event_fast_info_ptr->minute;
        second_time.sec = 0;
        start_date.year = event_fast_info_ptr->start_year;
        start_date.mon = event_fast_info_ptr->start_mon;
        start_date.mday = event_fast_info_ptr->start_day;

        min_date.year =MMICOM_DATE_MIN_YEAR;
        min_date.mon = 1;
        min_date.mday = 1;

        if (CMP_FIRST_SMALL == MMIAPIALM_CompareDateAndTime(start_date,start_time,min_date,min_time))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);

            break;
        }

        if (ALM_MODE_ONCE == event_fast_info_ptr->fre_mode)
        {
            // 判断时间是否过期
            if (CMP_FIRST_SMALL == MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time))
            {
                deadline.year = event_fast_info_ptr->end_year;
                deadline.mon= event_fast_info_ptr->end_mon;
                deadline.mday= event_fast_info_ptr->end_day;
                second_date.year = event_fast_info_ptr->year;
                second_date.mon  = event_fast_info_ptr->mon;
                second_date.mday = event_fast_info_ptr->day;

                if (IsValidDeadline(deadline, second_date))
                {			
                    MMISMS_SetIsTimerMode(TRUE); 

                    #ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);  
                    #else
                    MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                    #endif
                    
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
            }
        }
        else if (ALM_MODE_EVERYWEEK == event_fast_info_ptr->fre_mode)
        {
            uint8 i = 0;
            uint8 offset_day = 0;
            SCI_DATE_T temp_date = {0};
            
            if (0 == event_fast_info_ptr->fre)
            {
                MMIPUB_OpenAlertWarningWin(TXT_ALARM_SELECT_ONE_DAY);                    
                break;
            }                
            // 比较的日期判断时间是否过期
            second_date.year = event_fast_info_ptr->year;
            second_date.mon  = event_fast_info_ptr->mon;
            second_date.mday = event_fast_info_ptr->day;
            second_time.hour = event_fast_info_ptr->hour;
            second_time.min = event_fast_info_ptr->minute;
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
            {                   
                
                temp_date.year = event_fast_info_ptr->start_year;
                temp_date.mon = event_fast_info_ptr->start_mon;
                temp_date.mday = event_fast_info_ptr->start_day;
#ifdef CALENDAR_SUPPORT
                temp_date.wday = MMICALENDAR_GetWeekday(temp_date.year, temp_date.mon, temp_date.mday);
#endif                
                if (MMIAPIALM_CompareDateAndTime(temp_date, cur_time, temp_date, second_time) <= CMP_FIRST_SMALL)
                {
                    offset_day = MMIAPIALM_RoundWeek(temp_date.wday, event_fast_info_ptr->fre, CMP_FIRST_SMALL);
                }
                else
                {
                    offset_day = MMIAPIALM_RoundWeek(temp_date.wday, event_fast_info_ptr->fre, CMP_SECOND_SMALL);
                }
                
                for (i = 0; i <offset_day; i++)
                {
                    if ((TIMERMSG_QUERY_YEAR_TOP == temp_date.year) && (TIMERMSG_SCH_DECEMBER == temp_date.mon) && (TIMERMSG_BIG_MONTH_DAY == temp_date.mday))
                    {
                        break;
                    }
                    
                    MMIAPIALM_IncreaseDateByOne(&temp_date);
                }
                
                // 计算后的日期
                event_fast_info_ptr->day = temp_date.mday;
                event_fast_info_ptr->year = temp_date.year;
                event_fast_info_ptr->mon = temp_date.mon;
                
                event_fast_info_ptr->start_year = temp_date.year;
                event_fast_info_ptr->start_mon  = temp_date.mon;
                event_fast_info_ptr->start_day = temp_date.mday;
                
                deadline.year = event_fast_info_ptr->end_year;
                deadline.mon= event_fast_info_ptr->end_mon;
                deadline.mday= event_fast_info_ptr->end_day;
                
                second_date.year = temp_date.year;
                second_date.mon  = temp_date.mon;
                second_date.mday = temp_date.mday;
                if (IsValidDeadline(deadline, second_date))
                {			
                    MMISMS_SetIsTimerMode(TRUE); 
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
#else
                    MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
#endif
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
            }
        }
        else
        {
            deadline.year = event_fast_info_ptr->end_year;
            deadline.mon= event_fast_info_ptr->end_mon;
            deadline.mday= event_fast_info_ptr->end_day;
            second_date.year = event_fast_info_ptr->year;
            second_date.mon  = event_fast_info_ptr->mon;
            second_date.mday = event_fast_info_ptr->day;
            second_time.hour = event_fast_info_ptr->hour;
            second_time.min = event_fast_info_ptr->minute;

            if (CMP_FIRST_SMALL != MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time))
            {
                BOOLEAN result = FALSE;

                result = MMIAPIALM_SetNextAlarmTimeByCurrent(event_fast_info_ptr);

                if (!result || event_fast_info_ptr->year > TIMERMSG_QUERY_YEAR_TOP)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);

                    break;
                }
            }

            if (CMP_SECOND_SMALL == CompareDate(deadline, second_date))
            {	
                MMISMS_SetIsTimerMode(TRUE); 
                #ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                #else
                MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                #endif
                MMK_CloseWin(win_id);
            }
            else if (CMP_EQUAL == CompareDate(deadline, second_date))
            {
                if (CMP_FIRST_SMALL == MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time))
                {
                    MMISMS_SetIsTimerMode(TRUE); 
                    #ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                    #else
                    MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                    #endif
                    MMK_CloseWin(win_id);
                }                
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
            }
        }
        break;
        
    case MSG_CTL_MIDSK:          
    case MSG_APP_WEB:
        HandleTimerMsgEditWinWebKeyMsg(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        #ifdef MMI_PDA_SUPPORT
        if(ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
        {
            SaveTimerMsgEditWinFreqMode(event_fast_info_ptr);
            
            if (ALM_MODE_EVERYWEEK == event_fast_info_ptr->fre_mode)
            {
                if (!TimerMsgSaveFreEditWin(event_fast_info_ptr))
                {
                    break;
                }
            }
            
            if (ALM_MODE_ONCE != event_fast_info_ptr->fre_mode && ALM_MODE_NO != event_fast_info_ptr->fre_mode)
            {
#ifndef MMI_PDA_SUPPORT
                if (!SaveTimerMsgEditWinDeadlineDate(event_fast_info_ptr ,MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID))
                {
                    break;
                }
#endif
            }
            else
            {
                event_fast_info_ptr->end_year = event_fast_info_ptr->year;
                event_fast_info_ptr->end_mon = event_fast_info_ptr->mon;
                event_fast_info_ptr->end_day = event_fast_info_ptr->day;
            }
            
            event_fast_info_ptr->start_year = event_fast_info_ptr->year;
            event_fast_info_ptr->start_mon  = event_fast_info_ptr->mon;
            event_fast_info_ptr->start_day = event_fast_info_ptr->day;		
            
            TM_GetSysDate(&cur_date);
            TM_GetSysTime(&cur_time);
            
            second_date.year = event_fast_info_ptr->year;
            second_date.mon  = event_fast_info_ptr->mon;
            second_date.mday = event_fast_info_ptr->day;
            second_time.hour = event_fast_info_ptr->hour;
            second_time.min = event_fast_info_ptr->minute;
            second_time.sec = 0;
            start_date.year = event_fast_info_ptr->start_year;
            start_date.mon = event_fast_info_ptr->start_mon;
            start_date.mday = event_fast_info_ptr->start_day;
            
            min_date.year =MMICOM_DATE_MIN_YEAR;
            min_date.mon = 1;
            min_date.mday = 1;
            //SCI_TRACE_LOW:"HandleEditTimerMsgWinMsg:  MSG_CTL_PENOK event_fast_info_ptr->fre_mode = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_1713_112_2_18_2_53_41_477,(uint8*)"d", event_fast_info_ptr->fre_mode);
            
            if (CMP_FIRST_SMALL == MMIAPIALM_CompareDateAndTime(start_date,start_time,min_date,min_time))
            {
                MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
                
                break;
            }
            
            if (ALM_MODE_ONCE == event_fast_info_ptr->fre_mode)
            {
                // 判断时间是否过期
                if (CMP_FIRST_SMALL == MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time))
                {
                    deadline.year = event_fast_info_ptr->end_year;
                    deadline.mon= event_fast_info_ptr->end_mon;
                    deadline.mday= event_fast_info_ptr->end_day;
                    second_date.year = event_fast_info_ptr->year;
                    second_date.mon  = event_fast_info_ptr->mon;
                    second_date.mday = event_fast_info_ptr->day;
                    
                    if (IsValidDeadline(deadline, second_date))
                    {			
                        MMISMS_SetIsTimerMode(TRUE); 
                        #ifdef MMIMMS_SMS_IN_1_SUPPORT
                        MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                        #else
                        MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                        #endif
                        MMK_CloseWin(win_id);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
                }
            }
            else if (ALM_MODE_EVERYWEEK == event_fast_info_ptr->fre_mode)
            {
                uint8 i = 0;
                uint8 offset_day = 0;
                SCI_DATE_T temp_date = {0};
               
                if (0 == event_fast_info_ptr->fre)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_ALARM_SELECT_ONE_DAY);                    
                    break;
                }                
                // 比较的日期判断时间是否过期
                second_date.year = event_fast_info_ptr->year;
                second_date.mon  = event_fast_info_ptr->mon;
                second_date.mday = event_fast_info_ptr->day;
                second_time.hour = event_fast_info_ptr->hour;
                second_time.min = event_fast_info_ptr->minute;
                if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
                {                   

                    temp_date.year = event_fast_info_ptr->start_year;
                    temp_date.mon = event_fast_info_ptr->start_mon;
                    temp_date.mday = event_fast_info_ptr->start_day;
#ifdef CALENDAR_SUPPORT
                    temp_date.wday = MMICALENDAR_GetWeekday(temp_date.year, temp_date.mon, temp_date.mday);
#endif
                    if (MMIAPIALM_CompareDateAndTime(temp_date, cur_time, temp_date, second_time) <= CMP_FIRST_SMALL)
                    {
                        offset_day = MMIAPIALM_RoundWeek(temp_date.wday, event_fast_info_ptr->fre, CMP_FIRST_SMALL);
                    }
                    else
                    {
                        offset_day = MMIAPIALM_RoundWeek(temp_date.wday, event_fast_info_ptr->fre, CMP_SECOND_SMALL);
                    }
                    
                    for (i = 0; i <offset_day; i++)
                    {
                        if ((TIMERMSG_QUERY_YEAR_TOP == temp_date.year) && (TIMERMSG_SCH_DECEMBER == temp_date.mon) && (TIMERMSG_BIG_MONTH_DAY == temp_date.mday))
                        {
                            break;
                        }
                        
                        MMIAPIALM_IncreaseDateByOne(&temp_date);
                    }
                    
                    // 计算后的日期
                    event_fast_info_ptr->day = temp_date.mday;
                    event_fast_info_ptr->year = temp_date.year;
                    event_fast_info_ptr->mon = temp_date.mon;
                    
                    event_fast_info_ptr->start_year = temp_date.year;
                    event_fast_info_ptr->start_mon  = temp_date.mon;
                    event_fast_info_ptr->start_day = temp_date.mday;
                    
                    deadline.year = event_fast_info_ptr->end_year;
                    deadline.mon= event_fast_info_ptr->end_mon;
                    deadline.mday= event_fast_info_ptr->end_day;

                    second_date.year = temp_date.year;
                    second_date.mon  = temp_date.mon;
                    second_date.mday = temp_date.mday;
                    if (IsValidDeadline(deadline, second_date))
                    {			
                        MMISMS_SetIsTimerMode(TRUE); 
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                        MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
#else
                        MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
#endif
                        MMK_CloseWin(win_id);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
                } 
            }
            else
            {
                deadline.year = event_fast_info_ptr->end_year;
                deadline.mon= event_fast_info_ptr->end_mon;
                deadline.mday= event_fast_info_ptr->end_day;
                second_date.year = event_fast_info_ptr->year;
                second_date.mon  = event_fast_info_ptr->mon;
                second_date.mday = event_fast_info_ptr->day;
                second_time.hour = event_fast_info_ptr->hour;
                second_time.min = event_fast_info_ptr->minute;
                
                if (CMP_FIRST_SMALL != MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time))
                {
                    BOOLEAN result = FALSE;
                    
                    result = MMIAPIALM_SetNextAlarmTimeByCurrent(event_fast_info_ptr);
                    
                    if (!result || event_fast_info_ptr->year > TIMERMSG_QUERY_YEAR_TOP)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
                        
                        break;
                    }
                }
                
                if (CMP_SECOND_SMALL == CompareDate(deadline, second_date))
                {	
                    MMISMS_SetIsTimerMode(TRUE); 
                    #ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                    #else
                    MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                    #endif
                    MMK_CloseWin(win_id);
                }
                else if (CMP_EQUAL == CompareDate(deadline, second_date))
                {
                    if (CMP_FIRST_SMALL == MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time))
                    {
                        MMISMS_SetIsTimerMode(TRUE); 
                        #ifdef MMIMMS_SMS_IN_1_SUPPORT
                        MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                        #else
                        MMK_PostMsg(MMISMS_EDIT_SMS_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                        #endif
                        MMK_CloseWin(win_id);
                    }                
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
                }
            }
            break;
        }
        else if (ctrl_id ==MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID)
        {
            MMK_CreatePubFormWin((uint32 *)MMISMS_TIMERMSG_TIME_WIN_TAB, PNULL);
        }
        else if (ctrl_id ==  MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID)
        {
            MMK_CreatePubFormWin((uint32 *)MMISMS_TIMERMSG_DATE_WIN_TAB, PNULL);
        }  
        else if(ctrl_id ==  MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID)
        {
            MMK_CreatePubFormWin((uint32 *)MMISMS_TIMERMSG_DEADLINE_WIN_TAB, PNULL);
        }
        else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
        {
            MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
        }
        else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
        {
            MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
        }
        #endif
        
        HandleTimerMsgEditPENOKMsg(win_id, ctrl_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : view and edit deadline timer message
//    Global resource dependence : 
//  Author: rong.gu 
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsTimerMsgDeadlineWindow( 
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = &g_mmisms_global.curr_timer_mode_node.event_fast_info;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_IsSlide(MMISMS_TIMERMSG_DEADLINE_EDIT_FORM_CTRL_ID,FALSE);        
        MMK_SetAtvCtrl(win_id,MMISMS_TIMERMSG_DEADLINE_DEADLINEEDITBOX_CTRL_ID);
        DisplayTimerMsgDeadline(event_fast_info_ptr, MMISMS_TIMERMSG_DEADLINE_DEADLINEEDITBOX_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if (SaveTimerMsgEditWinDeadlineDate(event_fast_info_ptr, MMISMS_TIMERMSG_DEADLINE_DEADLINEEDITBOX_CTRL_ID))//date is valid
                {
                    MMK_CloseWin(win_id);            
                }
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);      
                break; 
            default:
                break;
            }
        } 
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        if (SaveTimerMsgEditWinDeadlineDate(event_fast_info_ptr, MMISMS_TIMERMSG_DEADLINE_DEADLINEEDITBOX_CTRL_ID))
        {
            MMK_CloseWin(win_id);
        }
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


/*****************************************************************************/
//     Description : view and edit timer message
//    Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsTimerMsgDateWindow( 
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = &g_mmisms_global.curr_timer_mode_node.event_fast_info;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_IsSlide(MMISMS_TIMERMSG_DATE_EDIT_FORM_CTRL_ID,FALSE);        
        MMK_SetAtvCtrl(win_id,MMISMS_TIMERMSG_DATE_DATEEDITBOX_CTRL_ID);
        DisplayTimerMsgDate(event_fast_info_ptr, MMISMS_TIMERMSG_DATE_DATEEDITBOX_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if (SaveTimerMsgEditWinDate(event_fast_info_ptr, MMISMS_TIMERMSG_DATE_DATEEDITBOX_CTRL_ID))//date is valid
                {
                    MMK_CloseWin(win_id);            
                }
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);      
                break; 
            default:
                break;
            }
        } 
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        if (SaveTimerMsgEditWinDate(event_fast_info_ptr, MMISMS_TIMERMSG_DATE_DATEEDITBOX_CTRL_ID))
        {
            MMK_CloseWin(win_id);
        }
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

/*****************************************************************************/
//  Description : 设置DEADLINE 日期时，显示DEADLINE 日期
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void DisplayTimerMsgDeadline(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T     ctrl_id )
{
    GUIEDIT_DATE_STYLE_E        date_style = GUIEDIT_DATE_STYLE_NULL;
    MMISET_DATE_DISPLAY_TYPE_E  date_type = MMISET_DATE_YMD;    
    
    date_type = MMIAPISET_GetDateDisplayType();
    switch (date_type)
    {
    case MMISET_DATE_YMD:
        date_style = GUIEDIT_DATE_STYLE_YMD;
        break;
        
    case MMISET_DATE_MDY:
        date_style = GUIEDIT_DATE_STYLE_MDY;
        break;
        
    case MMISET_DATE_DMY:
        date_style = GUIEDIT_DATE_STYLE_DMY;
        break;
        
    default:
        //SCI_TRACE_LOW:"DisplaySystemDate:date_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2085_112_2_18_2_53_41_478,(uint8*)"d",date_type);
        break;
    }    
    //set date style
    GUIEDIT_SetDateStyle(ctrl_id,PNULL,&date_style,FALSE);
    GUIEDIT_SetDate(ctrl_id,event_fast_info_ptr->end_year,event_fast_info_ptr->end_mon,event_fast_info_ptr->end_day);
}


/*****************************************************************************/
//  Description : 设置日期时，显示系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTimerMsgDate(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T     ctrl_id )
{
    GUIEDIT_DATE_STYLE_E        date_style = GUIEDIT_DATE_STYLE_NULL;
    MMISET_DATE_DISPLAY_TYPE_E  date_type = MMISET_DATE_YMD;    
    
    date_type = MMIAPISET_GetDateDisplayType();
    switch (date_type)
    {
    case MMISET_DATE_YMD:
        date_style = GUIEDIT_DATE_STYLE_YMD;
        break;
        
    case MMISET_DATE_MDY:
        date_style = GUIEDIT_DATE_STYLE_MDY;
        break;
        
    case MMISET_DATE_DMY:
        date_style = GUIEDIT_DATE_STYLE_DMY;
        break;
        
    default:
        //SCI_TRACE_LOW:"DisplaySystemDate:date_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_TIMERMSG_2119_112_2_18_2_53_41_479,(uint8*)"d",date_type);
        break;
    }    
    //set date style
    GUIEDIT_SetDateStyle(ctrl_id,PNULL,&date_style,FALSE);
    GUIEDIT_SetDate(ctrl_id,event_fast_info_ptr->year,event_fast_info_ptr->mon,event_fast_info_ptr->day);
}

/*****************************************************************************/
//  Description : 设置日期时，显示系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTimerMsgTime(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr, MMI_CTRL_ID_T     ctrl_id )
{
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    time_type = MMIAPISET_GetTimeDisplayType();    
    if (MMISET_TIME_12HOURS == time_type)
    {
        time_style = GUIEDIT_TIME_STYLE_12;
    }
    else
    {
        time_style = GUIEDIT_TIME_STYLE_24;
    }    
    //set time display style    
    GUIEDIT_SetTimeStyle(ctrl_id,PNULL,&time_style,PNULL,FALSE);    

    GUIEDIT_SetTime(ctrl_id,event_fast_info_ptr->hour,event_fast_info_ptr->minute,0);
}

/*****************************************************************************/
//     Description : view and edit timer message
//    Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsTimerMsgTimeWindow( 
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = &g_mmisms_global.curr_timer_mode_node.event_fast_info;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_IsSlide(MMISMS_TIMERMSG_TIME_EDIT_FORM_CTRL_ID,FALSE);        
        MMK_SetAtvCtrl(win_id,MMISMS_TIMERMSG_TIME_TIMEEDITBOX_CTRL_ID);
        DisplayTimerMsgTime(event_fast_info_ptr,  MMISMS_TIMERMSG_TIME_TIMEEDITBOX_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if (SaveTimerMsgEditWinTime(event_fast_info_ptr, MMISMS_TIMERMSG_TIME_TIMEEDITBOX_CTRL_ID))//date is valid
                {
                    MMK_CloseWin(win_id);            
                }
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);      
                break; 
            default:
                break;
            }
        } 
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        if (SaveTimerMsgEditWinTime(event_fast_info_ptr, MMISMS_TIMERMSG_TIME_TIMEEDITBOX_CTRL_ID))
        {
            MMK_CloseWin(win_id);
        }
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

/*****************************************************************************/
// 	Description : init a timer SMS record
//	Global resource dependence : 
//  Author:  hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void TIMERMSG_InitNewTimerMsg(
                                                                           uint16 year,
                                                                           uint8 month,
                                                                           uint8 day,
                                                                           MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr
                                                                           )
{
    SCI_TIME_T cur_time = {0};
    SCI_DATE_T date = {0};  

    if (PNULL == event_fast_info_ptr)
    {
        return;
    }
    
    TM_GetSysTime(&cur_time);
    
    event_fast_info_ptr->is_valid = TRUE;
    event_fast_info_ptr->is_on = TRUE;

    date.year = year;
    date.mon = month;
    date.mday = day;

    //新建日程时,默认时间是当前时间+1小时
    if (MMICOM_24HOUR_MAX_HOUR - 1 == cur_time.hour )
    {
        cur_time.hour = 0;

        MMIAPIALM_IncreaseDateByOne(&date);//date ++
    }
    else
    {
        cur_time.hour ++;
    }

    event_fast_info_ptr->start_year = date.year;
    event_fast_info_ptr->start_mon = date.mon;
    event_fast_info_ptr->start_day = date.mday; 
    event_fast_info_ptr->year = date.year;
    event_fast_info_ptr->mon = date.mon;
    event_fast_info_ptr->day = date.mday;
    event_fast_info_ptr->end_year = TIMERMSG_QUERY_YEAR_TOP;
    event_fast_info_ptr->end_mon = TIMERMSG_SCH_DECEMBER;
    event_fast_info_ptr->end_day = TIMERMSG_BIG_MONTH_DAY;
    event_fast_info_ptr->fre = 0x3e; // 表示周一到周五选中
    
    event_fast_info_ptr->hour = cur_time.hour;
    event_fast_info_ptr->minute = cur_time.min;
    
    event_fast_info_ptr->type = SMART_EVENT_SCH;
    event_fast_info_ptr->fre_mode = ALM_MODE_ONCE;
}

/*****************************************************************************/
// 	Description : CompareDate
//         compare a date to another , find smaller(earlier)
//	Global resource dependence : 
//  Author:        hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL uint16 CompareDate(
                         SCI_DATE_T first, 
                         SCI_DATE_T second
                         )
{
    MMISMS_TIMERMSG_ALM_CMP_E result = MMISMS_TIMERMSG_CMP_EQUAL;
    
    if (first.year > second.year)
    {
        result = MMISMS_TIMERMSG_CMP_SECOND_SMALL;
    }
    else if (first.year < second.year)
    {
        result = MMISMS_TIMERMSG_CMP_FIRST_SMALL;
    }
    else
    {
        if (first.mon > second.mon)
        {
            result = MMISMS_TIMERMSG_CMP_SECOND_SMALL;
        }
        else if (first.mon < second.mon)
        {
            result = MMISMS_TIMERMSG_CMP_FIRST_SMALL;
        }
        else
        {
            if (first.mday > second.mday)
            {
                result = MMISMS_TIMERMSG_CMP_SECOND_SMALL;
            }
            else if (first.mday < second.mday)
            {
                result = MMISMS_TIMERMSG_CMP_FIRST_SMALL;
            }    
            else
            {
                result = MMISMS_TIMERMSG_CMP_EQUAL;
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : if the deadline is earlier then the date ,it is not a valid deadline
//	Global resource dependence : 
//  Author:        hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsValidDeadline(
                              SCI_DATE_T deadline, 
                              SCI_DATE_T date
                              )
{
    BOOLEAN result = TRUE;
    
    if (MMISMS_TIMERMSG_CMP_FIRST_SMALL == CompareDate(deadline, date))
    {
        result = FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : get if start_date <= cur_date <= iend_date
//    Global resource dependence : 
//  Author:  zhaohui
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN TIMERMSG_IsInDateRange(     
                            SCI_DATE_T start_date, 
                            SCI_DATE_T end_date,
                            SCI_DATE_T cur_date
                         )
{
    if (((MMISMS_TIMERMSG_CMP_FIRST_SMALL == CompareDate(start_date, cur_date)) 
        &&(MMISMS_TIMERMSG_CMP_FIRST_SMALL == CompareDate(cur_date, end_date)))
        ||
        (MMISMS_TIMERMSG_CMP_EQUAL == CompareDate(start_date, cur_date)
        ||MMISMS_TIMERMSG_CMP_EQUAL == CompareDate(cur_date, end_date)))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : handle TimerMsg edit windows FormSwitchActive messages
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void HandleTimerMsgEditFormSwitchActiveMsg(
                                            MMI_HANDLE_T win_id,
                                            uint32 ctrl_id
                                          )
{
    switch(ctrl_id)
    {
    case MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON1_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON2_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON3_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON4_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON5_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON6_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
        SetTimerMsgEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
        TimerMsgSetEditWinButtonBg(win_id,ctrl_id);
        break;

    default:
#ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
#endif
        TimerMsgSetEditWinButtonBg(win_id,ctrl_id);
        break;
    }
}

/*****************************************************************************/
// 	Description : Set EditWin Button bg
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void TimerMsgSetEditWinButtonBg(MMI_HANDLE_T win_id, uint32 ctrl_id)
{
    GUI_BG_T bg={0};
    uint32 i = 0;
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};

    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#else
    disabled_font.font=SONG_FONT_16;
#endif        
    bg.bg_type = GUI_BG_IMG;
    for (i = 0; i < TIMERMSG_WEEK_DAY_NUM; i++)
    {
        if (s_mmisms_weeks_is_selected[i])
        {
            bg.img_id = IMAGE_CLOCK_ALARM_WEEK_ENABLED;
            GUIBUTTON_SetFont(MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID+i, &enabled_font);
        }
        else
        {
            bg.img_id = IMAGE_CLOCK_ALARM_WEEK_DISABLED;
            GUIBUTTON_SetFont(MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID+i, &disabled_font);
        }

        GUIBUTTON_SetBg(MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID + i, &bg);
        GUIBUTTON_Update(MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID + i);
    }
}

/*****************************************************************************/
// 	Description : save fre
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN TimerMsgSaveFreEditWin(MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr)
{
    uint8 week_day =0;
    int32 loop = 0;
    uint16 mode = 0;
    
    for (loop = 0; loop < TIMERMSG_WEEK_DAY_NUM; loop++)
    {
        week_day = MMIAPISET_GetWeekDayByOffset(loop);

        if (s_mmisms_weeks_is_selected[loop])
        {
            mode |= s_week_mask[week_day];
        }
    }

    if (mode != event_fast_info_ptr->fre) 
    {
        event_fast_info_ptr->fre = mode & 0x00ff;
    }

    if (0 == event_fast_info_ptr->fre)
    {
        MMIPUB_OpenAlertWarningWin(TXT_ALARM_SELECT_ONE_DAY);

        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle TimerMsg edit windows penok messages
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void  HandleTimerMsgEditPENOKMsg(
                                            MMI_HANDLE_T win_id,
                                            uint32 ctrl_id
                                          )
{
    switch(ctrl_id)
    {
    case MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON1_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON2_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON3_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON4_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON5_CTRL_ID:
    case MMISMS_TIMERMSG_EDITWIN_BUTTON6_CTRL_ID:
        if(s_mmisms_weeks_is_selected[ctrl_id - MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID])
        {
            s_mmisms_weeks_is_selected[ctrl_id - MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID] = FALSE;
        }
        else
        {
            s_mmisms_weeks_is_selected[ctrl_id - MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID] = TRUE;
        }

        SetTimerMsgEditSelectWeeksSoftkey(win_id,ctrl_id, TRUE);
        TimerMsgSetEditWinButtonBg(win_id, ctrl_id);
        break;

    default:
        break;
    }
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void TimerMsgButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32 ctrl_id = MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID;
    GUI_RECT_T rect = {0};//max_rect与owner_draw_ptr->display_rect相交区域，也即图片最后显示区域
    GUI_RECT_T max_rect = {0};//图片可以显示的最大区域
    GUI_RECT_T img_rect = {0};//图片裁剪区
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T win_id=0;
    uint16 image_w = 0;
    uint16 iamge_h = 0;

    ctrl_id = MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
    win_handle = MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id = MMK_GetWinId(win_handle);
    GUIRES_GetImgWidthHeight(&image_w, &iamge_h, IMAGE_CLOCK_ALARM_WEEK_MARK, win_id);

    max_rect.left = owner_draw_ptr->display_x;
    max_rect.top = owner_draw_ptr->display_y;
    max_rect.right = max_rect.left + image_w;
    max_rect.bottom = max_rect.top + iamge_h;
    
    if (!GUI_IntersectRect(&rect,max_rect, owner_draw_ptr->display_rect))
    {//没有相交区域，返回
        return;
    }

    img_rect.left = rect.left-max_rect.left;
    img_rect.top = rect.top-max_rect.top;
    img_rect.right = img_rect.left+(rect.right-rect.left);
    img_rect.bottom = img_rect.top+(rect.bottom-rect.top);

    if (GUI_IsInvalidRect(img_rect)||GUI_IsInvalidRect(rect))//无效矩形不要画了
    {
        return ;
    }
    
    if (s_mmisms_weeks_is_selected[ctrl_id-MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID])
    {
        GUIRES_DisplayImg(PNULL, &rect, &img_rect, win_handle, IMAGE_CLOCK_ALARM_WEEK_MARK, &owner_draw_ptr->lcd_dev);
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
LOCAL void SetTimerMsgEditSelectWeeksSoftkey(
                                            MMI_HANDLE_T win_id,
                                            uint32 ctrl_id,
                                            BOOLEAN is_update)
{
    if (ctrl_id >= MMISCH_EDITWIN_BUTTON0_CTRL_ID && ctrl_id <= MMISCH_EDITWIN_BUTTON6_CTRL_ID)
    {
        if (s_mmisms_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID])
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_CANCEL, STXT_RETURN, is_update);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_SELECT, STXT_RETURN, is_update);
        }
    }
}

/*****************************************************************************/
// 	Description : create a window according to timer msg index
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note: if index == MMISMS_TIMERMSG_MAX_ITEM create a timer msg interface with current time
//        otherwise   create a timer msg interface with timer msg info from NV
/*****************************************************************************/
PUBLIC void MMISMS_CreateTimerMsgWin(void)
{
    if (MMK_IsOpenWin(MMISMS_TIMERMSG_EDIT_WIN_ID))
    {
        MMK_CloseWin(MMISMS_TIMERMSG_EDIT_WIN_ID);
    }

    MMK_CreateWin((uint32 *)TIMERMSG_EDIT_WIN_TAB, PNULL);    
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

        MMISMS_SaveAsDraft(MMISMS_GetCurSendDualSys());

        return TRUE;
    }

    return FALSE;
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
                    MMISMS_SetOperStatus(MMISMS_READ_FOR_TIMERMSG);

                    MMISMS_SetIsTimerMsg(TRUE);

                    MMISMS_ReadyReadSms();

                    // to read the pointed SMS
                    if (MMISMS_AppReadSms(cur_order_id))
                    {
                        MMI_GetLabelTextByLang(TXT_SMS_TIMER_MESSAGE, &prompt_str);
                        
                        MMISMS_GetNameInfo(cur_order_id, &name, MMISMS_PBNAME_MAX_LEN);
                        
                        MMIPUB_OpenWaitWin(2,&prompt_str,&name,PNULL,MMISMS_TIMERMSG_WAIT_WIN_ID,IMAGE_NULL,
                            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleTimerMsgWaitWinMsg);
                    }                  
                    break;
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
#endif


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
