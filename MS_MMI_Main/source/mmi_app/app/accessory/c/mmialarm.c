/****************************************************************************
** File Name:      mmialarm.c                              
** Author:                                                                 
** Date:            2004/08/24 
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to describe functions of alarm
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 08/24/2004     taul.zhu          Create
** 06/02/2006     bruce.chi         modify
** 08/10/2006     liqing.peng       modify -- code review
****************************************************************************/

/************************************************************************/
//闹钟、自动开关机、日程都用事件表示，事件的结构: MMIACC_SMART_EVENT_T
//                                              (mmiacc_event.h)
//
//所有事件记录存储在NV里，每一个事件都有一个Id, Id从0开始计，
//闹钟事件的ID: EVENT_ALARM_0, EVENT_ALARM_1, EVENT_ALARM_2
//自动开关机事件的ID: EVENT_AUTO_POWERON, EVENT_AUTO_POWEROFF
//日程事件的ID: EVENT_SCH_0 -- EVENT_SCH_MAX(99)
//
//事件的基本属性说明如下:
//
//is_valid: 只用于NV存储，表示某个NV位置是否已经被使用
//          (每个事件记录都保存在一个NV位置)
//
//is_on: 事件是否被激活（也就是闹钟是否开启，日程到时是否需要提醒）
//
//type: 事件类型(闹钟、自动开机、自动关机、日程),对于闹钟、自动开关机事件
//      手机菜单中没有日期设置，对于日程事件手机菜单中有日期设置
//
//fre_mode: 模式(一次、每天、每周、每月等),对于闹钟、自动开关机事件模式有一次、
//          每天、每周、自定义；对于日程事件模式有一次、每天、每周、每月、每年
//
//fre: 和fre_mode配合使用，当模式是每周或者自定义时有效，用一个字节8bit表示
//      一周中有哪几天事件应该发生，一个bit表示一周中的一天
//
//year,mon,day,hour,minute: 用存放一个事件下一次将要发生的日期时间,它是通过
//      上面的参数(is_on, type, fre_mode, fre)计算得出的；例如设置一个模式
//      是每天的闹钟时，会自动根据当时的日期和时间计算下一次发生的日期和时间，
//      可能是当天的晚些时候，也可能是第二天；当这个闹钟到点启闹时，因为模式是
//      每天，它不会被关闭，而是计算下一次发生的日期和时间
//
//
//
//操作事件的函数主要的有下面几个：
//MMIALM_Init，手机开机时初始化全局变量，开机后，在调用下面其他函数之前调用
//
//MMIALM_GeEvent(笔误，应该是MMIALM_GetEvent)，通过Id获得一个事件结构
//MMIALM_ModifyEvent，修改一个事件
//MMIALM_AddOneEvent，增加一个事件
//MMIALM_DelOneEvent，删除一个事件
//MMIALM_DelEvents，删除一组事件
//
//MMIALM_UpdateEvents，当手机的时间重新设置后，需要调用这个函数来更新所有的
//                    事件记录
//
//MMIALM_HasActiveEvent，查看当前是否有激活的事件(时间到后会提醒的)，用于
//          Idle界面下，判断是否显示闹钟图标时使用
//
//MMIALM_GetArrivedEvent, 当MMI收到闹铃消息时，调用这个函数，获得和当时日期
//      时间相匹配的事件，然后更新事件(删除或者计算下一次发生的时间，修改事件) 
/************************************************************************/

/*****************************************************************************************************/
//闹钟到时MMI收到MISC_MSG_RTC_ALARM消息,调用MMIALM_StartAlarm函数, 按照以下
//三个流程之一进行:
//1. MMIAPIALM_StartAlarm -> OpenAlarmExpiredWin (正常情况)
//2. MMIAPIALM_StartAlarm -> PushDelayedEvent -------->(由于手机正在进行其他重要操作,需要等待操作结束再响应) 
//                     -> MMIAPIALM_CheckAndStartDelayedAlarm -> OpenAlarmExpiredWin
//3. MMIAPIALM_StartAlarm -> PushDelayedEvent -------->(由于手机正在进行其他重要操作,需要等待操作结束再响应) 
//                     -> MMIALM_PopupExpiredMsg
/*****************************************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "mmi_app_accessory_trc.h"
#include "tb_dal.h"
#include "window_parse.h"
//#include "win.def"
#include "mmk_app.h"
#include "guitext.h"
#include "guilcd.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmicc_export.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#ifdef DPHONE_SUPPORT
//#include "mmicalc_export.h"  //masked this line for pclint
#else
#include "mmicalc_export.h"  
#endif
#include "mmiwclk_export.h"
#include "mmiphone_export.h"
#include "mmialarm_export.h"
//#include "mmiacc_winmain.h"

//#include "mmigame_export.h"
#include "guimsgbox.h"
#include "mmidisplay_data.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiidle_subwintab.h"
#include "mmi_textfun.h"
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmiidle_export.h"
#include "mmiudisk_export.h"
#include "mmi_appmsg.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmischedule_export.h"
#include "mmi_solarlunar.h"
#include "mmialarm_id.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
//#include "mmialarm_menutable.h"
#include "mmialarm_nv.h"
#include "mmialarm_anim.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "guilabel.h"
#include "guibutton.h"
#include "mmipub.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef MAINLCD_SIZE_128X64
#include "mmialarm_position.h"
#endif
#include "mmiudisk_internal.h"
//#include "mmist_export.h"
#include "mmicountedtime_export.h"//@zhaohui, counted time
#include "mmiudisk_export.h"
#include "mmi_mainmenu_export.h"
#include "mmisd_export.h"
#include "mmifmm.h"
#include "mmicc_id.h"
#include "mmifmm_export.h"
#include "mmisms_export.h"
//#include "mmimpeg4_dc.h"
//#include "mmiset_wintab.h"
#include "mmimms_export.h"
#include "mmipicview_export.h"
#include "mmistk_export.h"
#include "guires.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmialarm_service.h"
//#include "mmiset_display.h"
#include "mmiset_id.h"
#ifdef PIM_SUPPORT
#include "vcalendar_parse.h"
#endif /* PIM_SUPPORT */

#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#include "guiform.h"
#include "guisetlist.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
//#include "mmiset_set_ring.h"

#ifdef FM_SUPPORT
#include "mmifm_export.h"
#include "mmifm_internal.h"
#endif
#include "gpio_prod_api.h"
#ifdef MMI_AZAN_SUPPORT
#include "mmiazan_export.h"
#endif
#include "mmi_text.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmidc_export.h"
#if defined (MMI_PDA_SUPPORT) || defined (MMI_ALARM_DEL_ADD_SUPPORT)
#include "Mmialarm_menutable.h"
#endif

#if defined(IM_ENGINE_SOGOU)
#include "wdp_update_wintab.h"
#include "mmiim_nv.h"
#endif
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget_id.h"
#include "mmiwidget_anim.h"
#include "mmiwidget_image.h"
#include "mmiwidget_text.h"
#include "mmiwidget_export.h"
#endif
#include "guictrl_api.h"
#include "mmiacc_image.h"
#ifdef SCREENSAVER_SUPPORT
#include "mmiss_export.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mmialarm_internal.h"
#include "guitab.h"
#include "mmiacc_id.h"
#include "mmiidle_statusbar.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef MMI_RECORD_SUPPORT
#include "watch_record_api.h"
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
#include "watch_alarm_main.h"
#include "watch_commonwin_export.h"
#endif

#include "watch_common.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define ALERT_RING_DURATION                 54000   // about one minute
#define BACKGROUND_ALERT_RING_DURATION      10000   // 大约震动两次

// 定义延时宏,延时5分钟
#define ALM_DELAY_TIME_SEC          300

#define ALM_EVENT_TIME_STR_LEN      5

#define ALM_POWEROFF_DELAY_SEC        30//关机动画时来闹钟,该闹钟被延时30秒

//for event ring info save use
#define MMIALM_RING_INFO_FILE_NAME_LEN   20
#define MMIALM_RING_INFO_FILE_SUFFIX  "_Ring.ini"

#define WEEKS_STR_BUFF_LEN  100

#ifdef DPHONE_SUPPORT
#define ALM_MODE_LIST      7
#define MMI_LISTBOX_LEFT      0 


#define ALM_EDIT_FOCUS_LINE_COLOR   MMI_BLACK_COLOR
#define ALM_EDIT_CLEAN_LINE_COLOR   MMI_WHITE_COLOR
#endif

#if defined(MMI_ALARM_MINI_SUPPORT) && defined(MMI_GUI_STYLE_MINISCREEN)
#ifdef  MAINLCD_SIZE_128X160
  //DE 引擎下的特殊设置，仅能设置成3行样式
  #if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)
    #define ALARM_MODE_LEFT    5
    #define ALARM_MODE_RIGHT  108
    #define ALARM_ICON_LEFT   105
  #else
    #define ALARM_MODE_LEFT    46
    #define ALARM_MODE_RIGHT  120
    #define ALARM_ICON_LEFT   105
  #endif
#elif defined MAINLCD_SIZE_176X220
//DE下的配置
#define ALARM_MODE_LEFT   (MMI_MAINSCREEN_WIDTH*1/3+MMI_LIST_ICON_LEFT_SPACE*2)
#define ALARM_ICON_LEFT   (MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH-5)
#define ALARM_MODE_RIGHT  (ALARM_ICON_LEFT-2)

#else
#define ALARM_MODE_LEFT   (MMI_MAINSCREEN_WIDTH*1/3+MMI_LIST_ICON_LEFT_SPACE*2)
#define ALARM_ICON_LEFT   (MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH-3)
#define ALARM_MODE_RIGHT  (ALARM_ICON_LEFT-2)
#endif
#endif
#define MMI_EDITWIN_FORM_RECT DP2PX_RECT(36,20,204,179)
/**--------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **--------------------------------------------------------------------------*/
#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL THEMELIST_ITEM_STYLE_T          s_my_style = {0};
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 
typedef struct _RADIO_BUTTON_TAG_
{
    BOOLEAN is_sel;
    BOOLEAN is_focus;
    GUI_POINT_T point;
    GUI_RECT_T rect;
    MMI_TEXT_ID_T text_id;
    MMI_IMAGE_ID_T image_id[3]; // 选中/未选中/焦点
    ALARM_CONTROL_ID_E ctrl_id;
}RADIO_BUTTON_T;

LOCAL BOOLEAN s_weeks_button_is_selected[ALM_WEEK_DAY_NUM]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
LOCAL MMISRV_HANDLE_T s_alarm_play_handle = 0;
#ifdef MMI_ETWS_SUPPORT
LOCAL uint8               s_line_num     = 0;
LOCAL MMI_STRING_T s_alarm_str1 = {0};
LOCAL MMI_STRING_T s_alarm_str2 = {0};
LOCAL MMI_IMAGE_ID_T    s_icon       = IMAGE_NULL;
LOCAL MMI_WIN_PRIORITY_E s_win_priority = WIN_ONE_LEVEL;
LOCAL BOOLEAN s_is_need_ring_and_vib = TRUE;
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
#define CLK_ROTATE_PIC_NUM 2

#ifdef MAINLCD_SIZE_320X480
#define CLK_CENTER_X  171
#define CLK_TIME_X   100
#define CLK_TIME_Y   30
#define CLK_DATE_X   117
#define CLK_DATE_Y   77 
#elif defined MAINLCD_SIZE_240X400
#define CLK_CENTER_X  137
#define CLK_TIME_X   60
#define CLK_TIME_Y   30
#define CLK_DATE_X   77
#define CLK_DATE_Y   77 
#else
#define CLK_CENTER_X  136
#define CLK_TIME_X   60
#define CLK_TIME_Y   0//30
#define CLK_DATE_X   77
#define CLK_DATE_Y   0//77
#endif

typedef struct {
    int32 angel;
    GUI_POINT_T  rotate_point;
    GUI_POINT_T  center_point;
}CLK_ROTATE_INFO_T;

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
LOCAL CLK_ROTATE_INFO_T  s_rotate_info_arr[CLK_ROTATE_PIC_NUM]=
{
    {
        359,
        {0,300},
        {154, 271}
    },
    {
        1,
        {250,300},
        {154, 271}
    }
};
#endif

LOCAL int s_pic_index = 0;
LOCAL uint8 s_clk_timer_id = 0;

#endif
#ifdef DPHONE_SUPPORT
LOCAL uint8                 s_edit_active_ctrl = 0;

// 编辑窗口active控件
LOCAL const uint32          s_edit_active_ctrl_attr[] =
{
  //  MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,
    MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
    MMIALM_EDIT_RING_TEXT_CTRL_ID,
    MMIALM_EDIT_MODE_SETLIST_CTRL_ID
};

LOCAL MMI_TEXT_ID_T s_mode_week_list_arr[ALM_MODE_WEEK_NUM] =
{
    TXT_SUNDAY, 
    TXT_MONDAY, 
    TXT_TUESDAY,        
    TXT_WEDNESDAY,
    TXT_THURSDAY,
    TXT_FRIDAY, 
    TXT_SATURDAY
};
//注意:正常情况下，focus焦点框的坐标范围一定要在所定义的label的坐标外面，不能有重叠；
LOCAL GUI_RECT_T focus_rec[3] =
{
    {ALM_EDIT_X2 - 1, ALM_EDIT_Y1 - 1, ALM_EDIT_X3 + 1, ALM_EDIT_Y1 + ALM_EDIT_HIGHT}, //lint !e778 ,  1st line for alarm time, ALM_EDIT_Y1 + 1 是为了显示完整的一个框，否则最顶上的线条会被遮盖。
    {ALM_EDIT_X2 - 1, ALM_EDIT_Y2 - 1, ALM_EDIT_X3 + 1, ALM_EDIT_Y2 + ALM_EDIT_HIGHT},// second line for ring type,ALM_EDIT_Y2 - 1 是为了把铃声两个汉字显示完整而不被遮盖.
    {ALM_EDIT_X2 - 1, ALM_EDIT_Y3 - 1, ALM_EDIT_X3 + 1, ALM_EDIT_Y3 + ALM_EDIT_HIGHT} // 3rd line for alarm mode: once,everyday or every week;
};  //lint !e778
#endif
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            
#ifdef ZDT_VIDEOCHAT_SUPPORT
extern BOOLEAN g_is_inVideo ; //视频通话中
extern BOOLEAN g_is_activeVideo ;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Open Alarm window 
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL void OpenAlarmAliveWin(
                             MMI_WIN_PRIORITY_E win_priority
                             );

/*****************************************************************************/
//  Description : 从wday向后找到第一个匹配的，返回offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek(
                      uint8 wday,
                      uint8 fre
                      );

/*****************************************************************************/
//  Description : 从wday后一天向后找到第一个匹配的，返回offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek2(
                       uint8 wday,
                       uint8 fre
                       );

/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : the process message function of the ALARM
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmMainWinMsg(
                                         MMI_WIN_ID_T       win_id,     
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );             

/*****************************************************************************/
//  Description : pop up a msg 
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleAlarmExpiredWinMsg(
//                                              MMI_WIN_ID_T       win_id, 
//                                              MMI_MESSAGE_ID_E   msg_id,     
//                                              DPARAM             param
//                                              );                     

/*****************************************************************************/
//  Description : increase current date by one
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL  void IncreaseDateByOne(
                              SCI_DATE_T  *pdate_ptr
                              );

/*****************************************************************************/
//  Description : 比较event，是否和当前日期时间匹配
//  Global resource dependence : 
//  Author: bin.ji
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsMatchEvent(
                           SCI_DATE_T                   first_date, 
                           SCI_TIME_T                   first_time, 
                           const MMIACC_SMART_EVENT_FAST_TABLE_T   *event_ptr
                           );


/*****************************************************************************/
//  Description : compare date and time to another group, find smaller(earlier)
//  Global resource dependence : 
//  Author: taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL int16 CompareDateTime(
                            SCI_ALARM_T first, 
                            SCI_ALARM_T second
                            );

/*****************************************************************************/
//  Description : push ID of current dealyed event to stack to save 
//                when a call is active
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL void PushDelayedEvent(
                            void
                            );

/*****************************************************************************/
//  Description : ClearDelayedEvent
//  Global resource dependence : 
//  Author:        liqing.peng
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void ClearDelayedEvent(void);

#ifdef DPHONE_SUPPORT
LOCAL MMI_RESULT_E  HandleAlarmPowerOnOffEditWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM           param
                                       );
#endif

/*****************************************************************************/
//  Description : HandleAutoPowerOnOffWinMsg
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnOffWinMsg( 
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : auto power on prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnExpired(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : auto power off prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOffExpired(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : open alarm expired win according to the alarm type
//  Global resource dependence : 
//  Author:      lin.lin
//  Note:            
/*****************************************************************************/
LOCAL BOOLEAN OpenAlarmExpiredWin(
                                  MMIACC_SMART_EVENT_E    type
                                  );

/*****************************************************************************/
//  Description : select ring of alarm
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmRingWinMsg(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : select fixed ring tone
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmFixedRingWinMsg(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : Stop ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   
/*****************************************************************************/
LOCAL void StopRingOrVib(BOOLEAN is_close_ring_timer);

/*****************************************************************************/
//  Description : Start ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   
/*****************************************************************************/
LOCAL void StartRingOrVib(void);

/*****************************************************************************/
//  Description : Start ring for ring or vibrate/ring
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   leon.wang add
/*****************************************************************************/
LOCAL void StartRingForRingOrVib(uint8 opt_type);

/*****************************************************************************/
//  Description : SetNextAlarmTimeByCurrent
//  Global resource dependence : 
//  RETURN :  
//  Author:  liqing.peng
//  Note:  根据event_ptr是否开启、类型(闹钟、自动开关机、日程)、
//         模式(一次、每天、每周等)、时间日期，
//         计算event_ptr下一次发生的时间
/*****************************************************************************/
LOCAL BOOLEAN SetNextAlarmTimeByCurrent(
                                        MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr         //in & out
                                        );

/*****************************************************************************/
//  Description : find latest active smart event, and set to alarm according to 
//                current SYS time and date
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindLatestEventToSet(void);

/*****************************************************************************/
//  Description : handle alarm edit windows messages
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );
#ifdef SCREEN_SHAPE_CIRCULAR
/*****************************************************************************/
//  Description : handle alarm mode edit windows messages
//  Global resource dependence :
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerOnOffModeEditWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );
#endif
/*****************************************************************************/
// Description : IncreaseTimeBySecond
// increase current time by seconds inputed.
// Global resource dependence : 
// Author:        taul.zhu
// Note:   
/*****************************************************************************/
LOCAL  void IncreaseTimeBySecond(
                                 SCI_DATE_T *pdate,
                                 SCI_TIME_T *ptime,
                                 uint16      sec
                                 );

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetSameTimeSchEvent(
                                 const MMIACC_SMART_EVENT_FAST_TABLE_T *latest_event,
                                 uint16 index,
                                 uint16 start_index,
                                 MMIACC_SCH_LIST_T *p_sch_list
                                 );

/*****************************************************************************/
//  Description : handle past event win msg
//  Global resource dependence : 
//  Author:       xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastEventWinMsg(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description : Open bypast sch event win(打开过期事件窗口)
//  Global resource dependence : 
//  Author: xiaoqing.lu 
//  Note:   
/*****************************************************************************/
LOCAL void OpenPastSchEventWin(
                               MMI_WIN_PRIORITY_E win_priority,
                               uint16 event_id
                               );

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneSchEventInList(
                                     uint16 index, // IN
                                     MMIACC_SCH_LIST_T *p_sch_list       // IN OUT:
                                     );

/*****************************************************************************/
//  Description : DisplayTheRingName
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void DisplayTheRingName(void);

/*****************************************************************************/
//  Description : close delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN CloseDelayAlarm(
                              uint16 event_id, 
                              MMIACC_SMART_EVENT_FAST_TABLE_T event,
                              BOOLEAN    is_on
                              );
/*****************************************************************************/
//  Description : set delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarmTime(
                                uint16 event_id, 
                                MMIACC_SMART_EVENT_T event,
                                BOOLEAN    is_auto
                                );

/*****************************************************************************/
//  Description : find if the alarm is set
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAlarmSet(void);

/*****************************************************************************/
//  Description : Start timer about alarm
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void StartAlarmTimer(
                           uint8 *timer_id,
                           uint32 time_out,
                           BOOLEAN is_period
                           );

/*****************************************************************************/
//  Description : to append two line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            );

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
/*****************************************************************************/
//  Description : append three line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendThreeLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            );
#endif

/*****************************************************************************/
//  Description : 设置延时闹钟
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarm(
                            uint16 event_id, 
                            MMIACC_SMART_EVENT_T event,
                            BOOLEAN    is_on
                            );

/*****************************************************************************/
//  Description : get the delayed event of event_id
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
//LOCAL MMIACC_SMART_EVENT_FAST_TABLE_T *GetDelayEvent(
//                                                     uint16 event_id
//                                                     );

/*****************************************************************************/
//  Description : 检查并启动过期事件(常在闹钟,自动开机等事件结束时调用)
//  Global resource dependence : 
//  Author: zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void CheckandStartExpiredEvent( void );

/*****************************************************************************/
// Description : 创建alarm目录
// Global resource dependence :
// Author:zhaohui
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitAlarmFolder(void);

/*****************************************************************************/
//  Description : 根据event_id映射成对应该记录的ring信息
//  Global resource
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetRingInfoFileName(
                                  uint16 event_id,  
                                  wchar *u16filename_ptr,//OUT
                                  uint16 u16filename_max_len
                                  );

/*****************************************************************************/
//  Description : 在铃声id信息文件中读取铃声id的路径信息  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN ReadRingInfoFileName(
                                   uint16 event_id,
                                   MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                   );

/*****************************************************************************/
//  Description : 把ring_ptr保存到根据event_id映射成的铃声id信息文件中 
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN SaveRingInfoToFile(
                                 uint16 event_id,
                                 const MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                 );
/*****************************************************************************/
// 	Description : save time
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimeEditWin(void);                                 

/*****************************************************************************/
//  Description : 删除铃声id信息或铃声id文件  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteRingInfoFile(
                                 uint16 event_id                        
                                 );

/*****************************************************************************/
//  Description : modify one event 
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN ModifyEvent(
                          uint16                    event_id, //in
                          MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr,  //in,out
                          BOOLEAN is_set_alarm
                          );

/*****************************************************************************/
//  Description : memset 0 to &s_event_fast_table[event_id],but hold s_event_fast_table[event_id].service_id
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void CleanEventFastTable(uint16 event_id);

/*****************************************************************************/
//  Description : add item for setlist
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  AddItemSetlist(void);

/*****************************************************************************/
//  Description : process the MIDSK message
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
// LOCAL void EditWinMIDSKProcess(
//                                 MMI_WIN_ID_T win_id //in
//                                 );
                                
/*****************************************************************************/
//  Description : set title for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTitleEditWin(
                                MMI_WIN_ID_T win_id //in
                                );

/*****************************************************************************/
//  Description : set name ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetNameParamEditWin(void);

/*****************************************************************************/
//  Description : set time ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTimeParamEditWin(void);

/*****************************************************************************/
//  Description : set ring ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetRingParamEditWin(void);

/*****************************************************************************/
//  Description : set mode ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetModeParamEditWin(void);

/*****************************************************************************/
//  Description : set  weeks ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksParamEditWin(MMI_WIN_ID_T      win_id);

/*****************************************************************************/
//  Description : set  weeks display
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksDisplayEditWin(void);

/*****************************************************************************/
//  Description : save name
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveNameEditWin(void);



/*****************************************************************************/
//  Description : save mode
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  SaveModeEditWin(void);

// LOCAL void AppendWeeksListItem(
//                              MMI_CTRL_ID_T    ctrl_id //in
//                              );

/*****************************************************************************/
//  Description : save the select weeks
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
// LOCAL uint16 SaveSelectWeeks(
//                                                MMI_CTRL_ID_T    ctrl_id //in
//                                                );

/*****************************************************************************/
//  Description : display Alarm window
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAlarmWin(
                          MMI_WIN_ID_T win_id, 
                          MMIPUB_INFO_T    *win_info_ptr
                          );
                          
/*****************************************************************************/
//  Description : past auto power on prompt window
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastAutoPowerOn(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );
/*****************************************************************************/
//  Description : handle alarm edit windows penok messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditPENOKMsg(
                                        MMI_HANDLE_T        win_id,
                                          uint32 ctrl_id
                                          );

/*****************************************************************************/
//  Description : handle alarm edit windows FormSwitchActive messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditFormSwitchActiveMsg(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id
                                          );

/*****************************************************************************/
//  Description : Set EditWin Button bg
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id);


/*****************************************************************************/
//  Description : save fre
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveFreEditWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void setAlarmEditSelectWeeksSoftkey(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id,
                                            BOOLEAN is_update);

/*****************************************************************************/
//  Description : 根据ID判断是否是日程事件
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
//LOCAL BOOLEAN IsSCHEvent(uint16 event_id);

/*****************************************************************************/
//  Description : 频率编辑窗口消息处理函数
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFMEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : 设置频率编辑窗口控件参数
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetFMEditCtrlParam(void);

/*****************************************************************************/
//  Description : change float string to integer value
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:
//  Note: 
/*****************************************************************************/
 LOCAL int16 AlarmConvertFloatStrToInt(const MMI_STRING_T *str_ptr, int32 mul_scale);

/*****************************************************************************/
//  Description : 判断闹钟是否需要在后台提醒
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsNeedBackgroundPrompt(void);

#if  defined DPHONE_SUPPORT || defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : form time str by time
//  Global resource dependence : 
//  Author:        zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void MMIALM_FormTimeStrbyTime(
                      uint8 hour,//in
                      uint8 min,//in
                      char* time_str,//out
                      uint16 time_str_max_len//in
                      );
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

/*****************************************************************************/
//  Description : to handle set time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );


/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditFmFreMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

#endif

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Discription: RecordAlarmRingCallBack
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:录音设置为铃声的callback
//
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordAlarmRingCallBack(void);
#endif

#if defined(SCH_ASSISTANT_WIDGET)
LOCAL MMI_RESULT_E HandleSchPOPAlertWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJumpClockEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );
#endif

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
/*****************************************************************************/
//Description :   ALARM OPT
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmMainOptWin(
                                           MMI_WIN_ID_T    win_id,   // 窗口ID
                                           MMI_MESSAGE_ID_E   msg_id,   // 消息ID
                                           DPARAM             param     // 消息参数
                                           );

/*****************************************************************************/
//Description :   GetAlarmRealId
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL uint16  GetAlarmRealId(uint16 id);

LOCAL uint16  FindAlarmUnuseId(void);

#endif
//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// LOCAL void SetListItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index);
// #endif

/*****************************************************************************/
//  Description : 用于关机闹钟起闹时初始化lcd背景
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
//LOCAL void initLcdBavkground(void);

/*---------------------------------------------------------------------------*/
/*                          GLOBE DATA DECLARE                               */
/*---------------------------------------------------------------------------*/
/*MMI_TEXT_ID_T s_mode_week_list_arr[ALM_MODE_WEEK_NUM] =
{
    TXT_SUNDAY, 
    TXT_MONDAY, 
    TXT_TUESDAY,        
    TXT_WEDNESDAY,
    TXT_THURSDAY,
    TXT_FRIDAY, 
    TXT_SATURDAY
};*/

#define CLOCK_REFRESH_SEC_TIME    100
#define CLOCK_REFRESH_MIN_TIME    6000
#if !defined MMI_GUI_STYLE_TYPICAL
ALARM_CLOCK_TYPE_E s_alm_clk_type = ALM_CLOCK_TYPE_1;
LOCAL uint8	s_refresh_timer_id = 0;
#endif

const uint8 s_week_mask[ALM_MODE_WEEK_NUM] = {MASK_B0, MASK_B1, MASK_B2, MASK_B3, MASK_B4, MASK_B5, MASK_B6};

 const MMI_TEXT_ID_T s_alm_week_text_id[ALM_WEEK_DAY_NUM] =
{
    TXT_SHORT_SUNDAY,
    TXT_SHORT_MONDAY,
    TXT_SHORT_TUESDAY,
    TXT_SHORT_WEDNESDAY,
    TXT_SHORT_THURSDAY,
    TXT_SHORT_FRIDAY,
    TXT_SHORT_SATURDAY
};
/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                               */
/*---------------------------------------------------------------------------*/
LOCAL MMIACC_SCH_LIST_T    s_bypast_list = {0}; // 记录过期事件
// edit窗口button控件的位置
LOCAL const uint8 s_RTC_month_info[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LOCAL const uint8 s_RTC_month_info_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

LOCAL EVENT_CURRENT_DATA_T  s_current_event_info = {0, 0, {0}};/*lint !e651*/
LOCAL MMIACC_SMART_EVENT_T  s_arrived_event = {0};  // 到达事件
LOCAL MMIACC_SMART_EVENT_T  s_poweroff_event = {0}; // 关机事件

LOCAL MMIACC_SMART_EVENT_FAST_TABLE_T   s_event_fast_table[MMIACC_EVENT_MAX_NUM] = {0};//快表,定义成数组,记录了闹钟/日程/开关机等事件的基本信息

LOCAL uint8                 s_event_timer_id = 0;
LOCAL uint8                 s_past_event_timer_id = 0;
LOCAL BOOLEAN               s_is_event_delayed = FALSE;
LOCAL uint16                s_open_event_id = 0;//上次open_win的时候的alarm_id，0-2
LOCAL uint16                s_arrived_event_id = 0;  //只记录当前闹钟的alarm_id,0-2
//LOCAL BOOLEAN               is_week_day_switch = FALSE;
LOCAL GUI_POINT_T           s_last_point = {0};
LOCAL uint8                 s_30seconds_timer_id = 0; 
LOCAL uint8                 s_3seconds_timer_id = 0; 
LOCAL uint8                 s_1seconds_timer_id = 0; 
LOCAL uint8                 s_30seconds_count = 30;  // 计数
LOCAL uint8                 s_vibrator_timer_id = 0;  
LOCAL BOOLEAN               is_vibrator_on = FALSE;  
LOCAL BOOLEAN               s_alarm_is_interrupted = FALSE;  
LOCAL uint16                s_RefCount = 0; //alarm内存分配时使用的引用计数,以避免多次分配等问题

LOCAL uint16                s_past_event_id=0;

LOCAL BOOLEAN               is_ring_on = FALSE;
LOCAL ALM_RING_TYPE_E       s_cur_alarm_ring_type=ALM_FIX_RING;

// 由于该数组中仅text_id被使用过，故使用s_alarm_mode_text来代替
// 每周模式时，当个button按钮的值,
// LOCAL RADIO_BUTTON_T s_radio_but[ALM_MODE_NUM] = 
// {
//     {
//         FALSE,
//         FALSE,
//         {0, 0},
//         {0, 0,  0,0},
//         TXT_ALARM_MODE_ONCE,
//         {IMAGE_RADIO_SELECTED_ICON, IMAGE_RADIO_UNSELECTED_ICON, IMAGE_CHECK_SELECTED_ICON},
//         MMIALM_EDIT_MODE_LIST1_CTRL_ID
//     },
//     {
//         FALSE,
//         FALSE,
//         {0, 0},
//         {0, 0,  0, 0},
//         TXT_ALARM_MODE_EVERYDAY,
//         {IMAGE_RADIO_SELECTED_ICON, IMAGE_RADIO_UNSELECTED_ICON, IMAGE_CHECK_SELECTED_ICON},
//         MMIALM_EDIT_MODE_LIST2_CTRL_ID
//     },
//     {
//         FALSE,
//         FALSE,
//         {0, 0},
//         {0, 0,  0, 0},
//         TXT_ALARM_MODE_WEEK,
//         {IMAGE_RADIO_SELECTED_ICON, IMAGE_RADIO_UNSELECTED_ICON, IMAGE_CHECK_SELECTED_ICON},
//         MMIALM_EDIT_MODE_LIST3_CTRL_ID
//     }
// };
LOCAL MMI_TEXT_ID_T s_alarm_mode_text[ALM_MODE_NUM]=
{
    TXT_ALARM_MODE_ONCE,
    TXT_ALARM_MODE_EVERYDAY,
    TXT_ALARM_MODE_WEEK
};

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// main alarm window
WINDOW_TABLE( ALARM_MAINWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleAlarmMainWinMsg ),    
    WIN_ID( MMIALM_MAINWIN_ID ),
#if defined(MMI_ALARM_MINI_SUPPORT) || (!defined(WORLD_CLOCK_SUPPORT) && !defined(STOPWATCH_SUPPORT) && !defined(COUNTEDTIME_SUPPORT))
    WIN_TITLE(TXT_ALARM_CLOCK),
#endif
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#else
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif    
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_LISTALL_CTRL_ID),
    END_WIN
}; 

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
//创建操作菜单(新建、编辑、删除) 
WINDOW_TABLE( MMIALM_OPTWIN_TAB ) =
{
    WIN_FUNC((uint32)HandleAlarmMainOptWin ),
    WIN_ID( MMIALM_OPTWIN_ID ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_ALARM_OPT,MMIALM_OPT_MENU_CTRL_ID),
    END_WIN   
};
#endif

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( ALARM_RINGWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmRingWinMsg ),    
    WIN_ID( MMIALM_RINGLIST_WIN_ID ),
    //WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    //WIN_TITLE(TXT_RING),
    CREATE_POPMENU_CTRL(MENU_ALARM_SETTING_RING, MMIALM_RINGLIST_CTRL_ID),
    END_WIN
};
#else
// set alarm ring
WINDOW_TABLE( ALARM_RINGWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmRingWinMsg ),    
    WIN_ID( MMIALM_RINGLIST_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_RING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIALM_RINGLIST_CTRL_ID),
    END_WIN
}; 
#endif
// set alarm fixed ring
WINDOW_TABLE( ALARM_FIXEDRINGWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmFixedRingWinMsg ),    
    WIN_ID( MMIALM_FIXEDRINGLIST_WIN_ID ),
    WIN_TITLE(TXT_RING_FIXED),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_STATUSBAR,
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIALM_FIXEDRINGLIST_CTRL_ID),
     END_WIN
}; 

//自动开关机设置
WINDOW_TABLE( AUTOPOWER_MAINWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAutoPowerOnOffWinMsg ),  
    WIN_ID( MMIAUTOPOWER_WIN_ID ),
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
#endif
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_AUTOPOWER_LIST_CTRL_ID),
    #ifdef DPHONE_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_EDIT, STXT_RETURN),
    #else
    #ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(STXT_SELECT, TXT_EDIT, STXT_RETURN),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    #endif
    END_WIN
}; 
WINDOW_TABLE( ALARM_EDITWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleAlarmEditWinMsg), 
    WIN_ID(MMIALM_EDIT_WIN_ID),
#ifdef DPHONE_SUPPORT
        //  WIN_STATUSBAR, //masked this for cr221626
        //注意:如果不需要title,也不能做WIN_TITLE(TXT_NULL),否则当paint title控件时,lcd的第一行会有1个像素的title背景;
        //这样可能会遮盖您所要画第0行的线条;
       //  WIN_TITLE(TXT_NULL), /*note:如果不mask这行,第一次进入闹钟编辑界面时,第一个焦点框的最顶上的线条会被遮盖.*/
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

        // 1st line--time
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,  MMIALM_EDIT_TITLE_LABEL_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID),

        // second line--ring type
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIALM_EDIT_RING_LABEL_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIALM_EDIT_RING_TEXT_CTRL_ID),
                    
        // 3rdline--alarm mode
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,  MMIALM_EDIT_MODE_LABEL_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MMIALM_MODELIST_CTRL_ID),

#else   
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    //WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_NAME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_NAME_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_TIME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#endif

//#ifdef MMI_PDA_SUPPORT
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIALM_EDIT_RING_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, MMIALM_EDIT_RING_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_RING_TYPE_CTRL_ID,MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID),
#ifndef MMI_RECORD_SUPPORT
        CHILD_BUTTON_CTRL(FALSE, IMAGE_DROPDOWNLIST_ARROW_PR, MMIALM_EDIT_RING_BUTTON_CTRL_ID, MMIALM_EDIT_RING_FORM_CTRL_ID),
#else
        CHILD_BUTTON_CTRL(TRUE, IMG_SCH_RECORD_ICON, MMIALM_EDIT_RING_BUTTON_CTRL_ID, MMIALM_EDIT_RING_FORM_CTRL_ID),       
#endif
//#else
//    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_RING_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
//        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
//        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_RING_TYPE_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
//#endif

//#ifndef MMI_PDA_SUPPOR
//        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_RING_TEXT_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
//#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIALM_EDIT_MODE_FORM_CTRL_ID, MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIALM_EDIT_MODE_FORM2_CTRL_ID, MMIALM_EDIT_MODE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIALM_EDIT_MODE_LABEL_CTRL_ID, MMIALM_EDIT_MODE_FORM2_CTRL_ID),
            CHILD_SETLIST_CTRL(TRUE, MMIALM_EDIT_MODE_SETLIST_CTRL_ID, MMIALM_EDIT_MODE_FORM2_CTRL_ID),
        CHILD_BUTTON_CTRL(FALSE, IMAGE_DROPDOWNLIST_ARROW_PR, MMIALM_EDIT_MODE_BUTTON_CTRL_ID, MMIALM_EDIT_MODE_FORM_CTRL_ID),
#else
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_MODE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
#endif
#ifdef CMCC_UI_STYLE
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_RING_DUR_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_DUR_LABLE_CTRL_ID,MMIALM_EDIT_RING_DUR_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID,MMIALM_EDIT_RING_DUR_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_VIBRATE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_VIBRATE_LABLE_CTRL_ID,MMIALM_EDIT_VIBRATE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID,MMIALM_EDIT_VIBRATE_FORM_CTRL_ID),
#endif

    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_EDITWIN_CHILD_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON0_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON1_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON2_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON3_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON4_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON5_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON6_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),  
 #endif
                
    END_WIN
}; 
#ifdef SCREEN_SHAPE_CIRCULAR
WINDOW_TABLE( POWERONOFF_MODE_EDIT_WIN) =
{
    WIN_FUNC((uint32) HandlePowerOnOffModeEditWinMsg),
    WIN_ID( POWERONOFF_MODE_EDIT_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_ALARM_EDIT_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, POWERONOFF_REPEAT_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef DPHONE_SUPPORT
WINDOW_TABLE( DPHONE_ALARM_POWERONOFF_EDIT_WIN) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmPowerOnOffEditWinMsg),
    WIN_ID( DPHONE_ALARM_POWERONOFF_EDIT_WIN_ID),
    WIN_TITLE(TXT_TIME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TIME_CTRL(MMIALM_SETTIME_CTRL_ID), 
    END_WIN
}; 
#else
WINDOW_TABLE(POWERONOFF_EDIT_WIN_TAB) = 
{
    WIN_ID(POWERONOFF_EDIT_WIN_ID),
    WIN_FUNC((uint32) HandleAlarmEditWinMsg),  
#if  !defined MMI_GUI_STYLE_TYPICAL
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR 
    WIN_STATUSBAR,
#endif
#endif
#ifndef SCREEN_SHAPE_CIRCULAR
    WIN_TITLE(TXT_NULL),
#endif
#ifdef MMI_PDA_SUPPORT
    //WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif


    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_NAME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_NAME_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_TIME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_RING_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIALM_EDIT_RING_TYPE_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_RING_TEXT_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_MODE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
#ifndef SCREEN_SHAPE_CIRCULAR
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
#else
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_SELECT_LABEL_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
#endif
    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_EDITWIN_CHILD_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON0_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON1_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON2_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON3_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON4_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON5_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON6_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),                       
                
    END_WIN
}; 
#endif

WINDOW_TABLE( ALARM_FM_EDITWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleFMEditWinMsg), 
    WIN_ID(MMIALM_FM_EDIT_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_ALARM_FM_RING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMI_PDA_SUPPORT
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_SELECT_FM_LIST_CTRL_ID),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_LIST_FORM_CTRL_ID,MMI_FM_EDITWIN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID,MMI_FM_EDITWIN_LIST_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID,MMI_FM_EDITWIN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMI_FM_EDITWIN_FRE_LABEL_CTRL_ID,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_FM_MAX_LEN,MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID),          
#endif
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMIALM_NAMEWIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditNameMsg),    
    WIN_ID(MMIALM_NAMEWIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_TEXT_CTRL(MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};
WINDOW_TABLE( MMIALM_TIMEWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditTimeMsg ),    
    WIN_ID( MMIALM_TIMEWIN_ID ),
    WIN_TITLE( TXT_TIME ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIALM_TIME_FORM_CTRL_ID),
    CHILD_EDIT_TOUCH_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_TIME_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

WINDOW_TABLE(MMIALM_EDITFMFRE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditFmFreMsg),    
    WIN_ID(MMIALM_EDITFMFRE_WIN_ID),
#ifndef MMI_GUI_STYLE_MINISCREEN    
    WIN_TITLE(TXT_ALARM_CUSTOM_FM),
#else
     WIN_TITLE(TXT_ALARM_CUSTOM_FM_TEXT),
#endif
    CREATE_EDIT_TEXT_CTRL(MMIALM_FM_MAX_LEN,MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};
#endif

#if defined(SCH_ASSISTANT_WIDGET)
WINDOW_TABLE(MMISCH_POP_ALERT_WIN_TAB)=
{
    WIN_FUNC((uint32)HandleSchPOPAlertWinMsg),
    WIN_ID(MMISCH_POP_ALERT_ID),
    WIN_TITLE(TXT_WIDGET_SCHEDULE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMISCH_POP_ALERT_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISCH_POP_ALERT_LIST_CTRL_ID, MMISCH_POP_ALERT_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMISCH_POP_ALERT_ANIM_CTRL_ID, MMISCH_POP_ALERT_FORM_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
WINDOW_TABLE( MMIALM_JUMP_CLOCK_TAB ) = 
{
    WIN_ID( MMIALM_ALIVEWIN_ID ),
    WIN_FUNC((uint32)HandleJumpClockEventAliveWinMsg),  
    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_CLOSE, TXT_NULL, TXT_ALARM_DELAY),
     END_WIN
};
#endif

/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/
//pengliqing, 2006-07-28, about alarm, schedule, auto power on/off module Kernel Functions, start

/*****************************************************************************/
//  Discription: modify one alarm event
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIALARM_ModifyOneAlarm( uint16 event_id, MMIACC_SMART_EVENT_FAST_TABLE_T* event_ptr )
{
    if( PNULL == event_ptr )
    {
        SCI_TRACE_LOW("event_ptr is PNULL!!");
        return FALSE;
    }

    if( event_id > ALM_ALARM_NUM )
    {
        SCI_TRACE_LOW("error event_id %u", event_id);
        return FALSE;
    }

    s_event_fast_table[event_id].is_valid = TRUE;
    event_ptr->type = SMART_EVENT_ALARM;

    ModifyEvent( event_id, event_ptr, TRUE );

    return TRUE;
}

/*****************************************************************************/
//  Discription: get one unused alarm event id
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIALARM_GetOneUnusedAlarm( uint16* event_id_ptr )
{
    uint16 index = 0;

    if( PNULL == event_id_ptr )
    {
        return FALSE;
    }
    for( index = 0; index < ALM_ALARM_NUM; index++ )
    {
        if( FALSE == s_event_fast_table[index].is_valid )
        {
            *event_id_ptr = index;
            return TRUE;
        }
    }
    return FALSE; // no unused alarm
}

/*****************************************************************************/
//  Discription: turn off one alarm by event id
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_CloseOneAlarm( uint16 event_id )
{
    BOOLEAN reCode = FALSE;
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = NULL;

    if( event_id > ALM_ALARM_NUM )
    {
        SCI_TRACE_LOW("error event_id %u", event_id);
        return;
    }

    //alarm
    MMIAPIALM_FormInteEventInfo( event_id, &s_current_event_info.event );
    s_current_event_info.id = event_id;

    if( TRUE == s_current_event_info.event.event_fast_info.is_on )
    {
        s_current_event_info.event.event_fast_info.is_on = FALSE;
        reCode = MMIAPIALM_SaveInteEventInfo( s_current_event_info.id, &(s_current_event_info.event) );
        if( FALSE == reCode )
        {
            SCI_TRACE_LOW("MMIALARM_CloseOneAlarm MMIAPIALM_SaveInteEventInfo fail!!");
        }
    }

    //delay alarm related
    event_delay_ptr = GetDelayEvent( s_current_event_info.id );

    if ( TRUE == event_delay_ptr->is_on )
    {
        SetDelayAlarm( s_current_event_info.id, s_current_event_info.event, FALSE );
    }
}

/*****************************************************************************/
//  Discription: turn ON one alarm by event id
//  Global resource dependence: none
//  Author: qi.liu1
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_OpenOneAlarm( uint16 event_id )
{
    BOOLEAN reCode = FALSE;
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = NULL;

    if( event_id > ALM_ALARM_NUM )
    {
        SCI_TRACE_LOW("MMIALARM_OpenOneAlarm:error event_id %u", event_id);
        return;
    }

    //alarm
    MMIAPIALM_FormInteEventInfo( event_id, &s_current_event_info.event );
    s_current_event_info.id = event_id;

    if( FALSE == s_current_event_info.event.event_fast_info.is_on )
    {
        s_current_event_info.event.event_fast_info.is_on = TRUE;
        reCode = MMIAPIALM_SaveInteEventInfo( s_current_event_info.id, &(s_current_event_info.event) );
        if( FALSE == reCode )
        {
            SCI_TRACE_LOW("MMIALARM_OpenOneAlarm MMIAPIALM_SaveInteEventInfo fail!!");
        }
    }

    //delay alarm related
    event_delay_ptr = GetDelayEvent( s_current_event_info.id );

    if ( TRUE == event_delay_ptr->is_on )
    {
        SetDelayAlarm( s_current_event_info.id, s_current_event_info.event, FALSE );
    }
}

/*****************************************************************************/
//  Discription: delete one alarm by event id
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_DeleteOneAlarm( uint16 event_id )
{
    if( event_id > ALM_ALARM_NUM )
    {
        SCI_TRACE_LOW("error event_id %u", event_id);
        return;
    }

    //first close
    MMIALARM_CloseOneAlarm( event_id );

    if( TRUE == s_event_fast_table[event_id].is_valid )
    {
        s_event_fast_table[event_id].is_valid = FALSE;
        MMINV_WRITE( MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table );
    }
}

/*****************************************************************************/
//  Discription: delete all alarm
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_DeleteAllAlarm()
{
    uint8 i=0;
    for(i=0; i<ALM_ALARM_NUM; i++)
    {
        MMIALARM_DeleteOneAlarm(i);
    }

}

/*****************************************************************************/
//  Description : get one event
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC const MMIACC_SMART_EVENT_FAST_TABLE_T *MMIALM_GeEvent (//外部文件引用s_event_fast_table的接口
                                                              uint16 event_id
                                                              )
{

    //SCI_PASSERT(MMIACC_EVENT_MAX_NUM > event_id, ("MMIALM_GeEvent: the event_id is %d",event_id));/*assert verified*/
    if(MMIACC_EVENT_MAX_NUM <= event_id)
    {
        //SCI_TRACE_LOW:"MMIALM_GeEvent: the event_id is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1449_112_2_18_2_0_52_12,(uint8*)"d",event_id);
        return PNULL;
    }
    return &s_event_fast_table[event_id];
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : 获取实际提醒时间
//	Global resource dependence : 
//  Author:  
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TM_T MMIALM_GetSchTime(
                                            uint16 year,
                                            uint8 mon,
                                            uint8 day,
                                            uint8 hour,
                                            uint8 minute,
                                            MMISCH_REMIND_TYPE_E remind_type,
                                            BOOLEAN is_get_remind_time
                                            )
{
    uint32          temp_seconds = 0;
    uint32          res_seconds = 0;
    MMI_TM_T    tm = {0};
    
    tm.tm_year = year;
    tm.tm_mon = mon;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    
    switch(remind_type)
    {
        case MMISCH_REMIND_FIVE_MIN: //提前5分钟
            temp_seconds = 5*60;
            break;
            
        case MMISCH_REMIND_FIFTEEN_MIN: //提前15分钟
            temp_seconds = 15*60;
            break;
            
        case MMISCH_REMIND_ONE_HOUR: //提前1小时
            temp_seconds = 60*60;
            break;
            
        case MMISCH_REMIND_ONE_DAY: //提前一天
            temp_seconds = 24*60*60;
            break;
            
        case MMISCH_REMIND_ONE_WEEK: //提前一周
            temp_seconds = 7*24*60*60;
            break;
            
        default: //准时
            break;
    }

    if(temp_seconds > 0)
    {
        res_seconds = MMIAPICOM_Tm2Second(0, minute, hour, day, mon, year);

        if(is_get_remind_time)
        {
            //获得实际提醒时间
            if(res_seconds > temp_seconds)
            {
                res_seconds -= temp_seconds;
            }
        }
        else
        {
            res_seconds += temp_seconds;
        }
        
        tm = MMIAPICOM_Second2Tm(res_seconds);
    }

    return tm;
}
#endif

/*****************************************************************************/
//  Description : 设置闹钟服务
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL void SetAlarmService(
                           uint16 event_id,
                           BOOLEAN is_set_alarm
                           )
{
//     SCI_ALARM_T alm_time = {0};
    MMIALMSVC_EVENT_REG_INFO_T event_info = {0};
    
#ifdef CMCC_UI_STYLE
    MMI_TM_T    time = MMIALM_GetSchTime(
                                    s_event_fast_table[event_id].year, 
                                    s_event_fast_table[event_id].mon, 
                                    s_event_fast_table[event_id].day, 
                                    s_event_fast_table[event_id].hour, 
                                    s_event_fast_table[event_id].minute, 
                                    s_event_fast_table[event_id].remind_type, 
                                    TRUE);

    event_info.callback = MMIAPIALM_StartAlarm;
    event_info.event_type = ALMSVC_EVENT_SYSTEM;

    event_info.event_time.hour = time.tm_hour;
    event_info.event_time.min = time.tm_min;
    event_info.event_time.year = time.tm_year;
    event_info.event_time.mon = time.tm_mon;
    event_info.event_time.mday = time.tm_mday;
    event_info.event_time.sec = 0;
#else
    event_info.callback = MMIAPIALM_StartAlarm;
    event_info.event_type = ALMSVC_EVENT_SYSTEM;

    event_info.event_time.hour = s_event_fast_table[event_id].hour;
    event_info.event_time.min = s_event_fast_table[event_id].minute;
    event_info.event_time.year = s_event_fast_table[event_id].year;
    event_info.event_time.mon = s_event_fast_table[event_id].mon;
    event_info.event_time.mday = s_event_fast_table[event_id].day;
    event_info.event_time.sec = 0;
#endif
    if (MMIALMSVC_IsValidSvc(s_event_fast_table[event_id].service_id))
    {
        MMIALMSVC_UpdateServcice(&event_info, s_event_fast_table[event_id].service_id, s_event_fast_table[event_id].is_on, is_set_alarm); // 更新服务
    }
    else
    {
        s_event_fast_table[event_id].service_id = MMIALMSVC_RegService(&event_info, s_event_fast_table[event_id].is_on, is_set_alarm); // 注册服务
        //s_event_fast_table[event_id].service_id = s_event_fast_table[event_id].service_id;
    }
}


/*****************************************************************************/
//  Description : modify one event 
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN ModifyEvent(
                          uint16                    event_id, //in
                          MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr,  //in,out
                          BOOLEAN is_set_alarm
                          )
{
    BOOLEAN   result = FALSE;
    MMIALMSVC_ID_T      service_id = s_event_fast_table[event_id].service_id;
    //SCI_ASSERT(NULL != event_ptr);/*assert verified*/
    if(NULL == event_ptr)
    {
        return result;
    }
    
    if(MMIACC_EVENT_MAX_NUM <= event_id)
    {
        //SCI_TRACE_LOW:"ModifyEvent: the event_id is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1509_112_2_18_2_0_52_13,(uint8*)"d",event_id);
        return result;
    }

    if(s_event_fast_table[event_id].is_valid)
    {
        //根据event_ptr是否开启、类型(闹钟、自动开关机、日程)、模式(一次、每天、每周等)、时间日期，
        //计算event_ptr下一次发生的时间
        SetNextAlarmTimeByCurrent(event_ptr);

        SCI_MEMCPY(&s_event_fast_table[event_id], (uint8 *)event_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

        // 设置服务
        MMIALMSVC_FreeAlmRegService(service_id, is_set_alarm);//CR136024
        SetAlarmService(event_id, is_set_alarm);

        FindLatestEventToSet();
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : modify one event 
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_ModifyEvent(
                                  uint16                    event_id, //in
                                  MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr  //in,out
                                  )
{
    return ModifyEvent(event_id, event_ptr, TRUE);
}

/*****************************************************************************/
//  Description : modify one event content name
//  Global resource dependence : 
//  Author:     xiaohua.liu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_ModifyEventName(
                                  uint16                    event_id, //in
                                  MMIACC_SMART_EVENT_CONTENT_T    *event_content_ptr  //in,out
                                  )
{
	if(PNULL == event_content_ptr)
	{
		return FALSE;
	}
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, event_content_ptr);  
	return TRUE;
}
/*****************************************************************************/
//  Description : add one event
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_AddOneEvent(
                           MMIACC_SMART_EVENT_T *event_ptr,
                           uint16               *index_ptr//out
                            )
{
    BOOLEAN result = FALSE;
    MMIALMSVC_ID_T      service_id = {0};
    uint16  i = 0;

    if (PNULL == event_ptr)
    {
        //MMI_ASSERT_WARNING("NULL != event_ptr");/*assert verified*/
        //SCI_TRACE_LOW:"MMIALM_AddOneEvent NULL == event_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1560_112_2_18_2_0_53_14,(uint8*)"");
        return FALSE;/*lint !e527 */
    }
    //SCI_TRACE_LOW:"MMIALM_AddOneEvent enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1563_112_2_18_2_0_53_15,(uint8*)"");

    for(i=EVENT_SCH_0; i<=EVENT_SCH_MAX; i++)
    {
        if(!s_event_fast_table[i].is_valid)
        {
            break;
        }
    }

    if(i <= EVENT_SCH_MAX)
    {
        result = TRUE;

        event_ptr->event_fast_info.is_valid = TRUE;

        //根据event_ptr是否开启、类型(闹钟、自动开关机、日程)、模式(一次、每天、每周等)、时间日期，
        //计算event_ptr下一次发生的时间
        SetNextAlarmTimeByCurrent(&event_ptr->event_fast_info);
        
        //保存快表
        SCI_MEMCPY((uint8 *)(&s_event_fast_table[i]), (uint8 *)&event_ptr->event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
        //保存内容表
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_ptr->event_content_info);  
        //保存ring info
        if (ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type
#ifdef FM_SUPPORT
            ||ALM_FM_RING == event_ptr->event_content_info.ring_type
#endif
            ||ALM_RECORD_RING == event_ptr->event_content_info.ring_type
            )
        {
            result = SaveRingInfoToFile(i,&event_ptr->event_ring_info);  
        }    
        service_id = s_event_fast_table[i].service_id;
        MMIALMSVC_FreeAlmRegService(service_id, TRUE);//CR136024
        // 设置闹钟服务
        SetAlarmService(i, TRUE);

        FindLatestEventToSet();
        if(NULL != index_ptr)
        {
            *index_ptr = i;
        }
        
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
        MMIVCAL_UpdatePimStatus(i-EVENT_SCH_0+1,VCAL_RECENT_ADD);
#endif /* HS_PIM_VCAL */
    }

    return result;

}

/*****************************************************************************/
//  Description : delete one event
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelOneEvent(
                           uint16 event_id
                           )
{
    BOOLEAN result = FALSE;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
        
    if(EVENT_SCH_0 <= event_id && EVENT_SCH_MAX >= event_id && s_event_fast_table[event_id].is_valid)
    {
        MMIALMSVC_ID_T      service_id = s_event_fast_table[event_id].service_id;
        //清除快表上的事件
        SCI_MEMSET(&s_event_fast_table[event_id], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

        //清除内容表上的事件
        SCI_MEMSET(&event_content, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T));
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &event_content);
        
        //删除铃声文件
        DeleteRingInfoFile(event_id);
        // 删除一个服务之后，要更新闹钟
        MMIALMSVC_UnRegService(service_id, TRUE);

        FindLatestEventToSet();

        result = TRUE;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
        MMIVCAL_UpdatePimStatus(event_id-EVENT_SCH_0+1,VCAL_RECENT_DELETE);
#endif /* HS_PIM_VCAL */
#ifdef CALENDAR_SUPPORT
        MMIAPISCH_DeleteUnreadEvent(event_id);
#endif
    }
    else
    {
        result = FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : delete event cnf
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
PUBLIC void MMIALM_DeleteCnf(void)
{ 
    MMK_PostMsg(MMISCH_DELETE_ANIM_WIN_ID , MSG_SCH_LIST_DEL_ALL, PNULL, 0);
}

/*****************************************************************************/
//  Description : delete first valid events
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelFirstValidEvent(void)
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    MmiSignalS *signal = PNULL; 

    if(MMIALM_GetAlmInterruptFlag())
    {
        //delete be interrupted, need stop delete
        return result;
    }
    for(i= EVENT_SCH_0; i<= EVENT_SCH_MAX; i++)
    {
        //find first valid event
        if(s_event_fast_table[i].is_valid)
        {
            //find it, delete it
            result = MMIALM_DelOneEvent(i);
            if(result)
            {
                //send delete cnf
                //creat signal
                MMI_CREATE_SIGNAL(signal,(uint16)MSG_SET_DEL_ALARMSET_CNF,sizeof(MmiSignalS),P_APP);
                //SCI_ASSERT(PNULL != signal);/*assert verified*/
                //send signal
                MMI_SEND_SIGNAL(signal, P_APP);
            }
            break;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : find if exist valid event
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_IsExistValidEvent(void)
{
    BOOLEAN result = FALSE;
    uint32  i = 0;  

    for(i= EVENT_SCH_0; i<= EVENT_SCH_MAX; i++)
    {
        //find first valid event
        if(s_event_fast_table[i].is_valid)
        {
            //find it, delete it
            result = FALSE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : delete events
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelEvents(
                                uint16 start_event,
                                uint16 end_event
                                )
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    
    //SCI_PASSERT(start_event <= end_event, ("MMIALM_DelEvents: start_event > end_event")); /*assert verified*/
    if(start_event > end_event)
    {
        //SCI_TRACE_LOW:"MMIALM_DelEvents start_event > end_event!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1742_112_2_18_2_0_53_16,(uint8*)"");
        return FALSE;
    }
    
    //清除内容表上的事件
    SCI_MEMSET(&event_content, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T));

    if((EVENT_SCH_0 <= start_event && EVENT_SCH_MAX >= start_event) &&
        (EVENT_SCH_0 <= end_event && EVENT_SCH_MAX >= end_event) )
    {
        for(i=start_event; i<=end_event; i++)
        {
            if(s_event_fast_table[i].is_valid)
            {
                MMIALMSVC_ID_T      service_id = s_event_fast_table[i].service_id;
                SCI_MEMSET(&s_event_fast_table[i], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

                //清除内容表上的事件
                MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);

                //删除铃声文件表
                DeleteRingInfoFile(i);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
                MMIVCAL_UpdatePimStatus(i-EVENT_SCH_0+1,VCAL_RECENT_DELETE);
#endif /* HS_PIM_VCAL */
                // 循环删除，不需要每次都更新闹钟
                MMIALMSVC_UnRegService(service_id, FALSE);
            }
        }
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

        // 全部删除了服务之后，要更新闹钟服务
        MMIALMSVC_SetAlmService();
        
        FindLatestEventToSet();
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}


/*****************************************************************************/
//  Description : update events when date and time setting changes
//  Global resource dependence : 
//  Author:      liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIALM_UpdateEvents(void)
{
    uint32      i = 0;
    BOOLEAN     result = FALSE;
    SCI_DATE_T              cur_date = {0};
    SCI_TIME_T              cur_time = {0};
    SCI_DATE_T              second_date = {0};
    SCI_TIME_T              second_time = {0};
    MMIALMSVC_ID_T      service_id = {0};

    //SCI_TRACE_LOW:"MMIALM_UpdateEvents enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1801_112_2_18_2_0_53_17,(uint8*)"");
#ifdef MMI_AZAN_SUPPORT
    MMIAPIAZAN_StopRepeatWinTimer();
#endif

    for(i=0; i<MMIACC_EVENT_MAX_NUM; i++)
    {
        if(s_event_fast_table[i].is_valid && s_event_fast_table[i].is_on)
        {
            if(SetNextAlarmTimeByCurrent(&s_event_fast_table[i]))
            {
                if (EVENT_ALARM_DELAY_0 <= i && i<=EVENT_ALARM_DELAY_MAX)/*lint !e685 !e568*/
            {
                TM_GetSysDate(&cur_date);
                TM_GetSysTime(&cur_time);
                second_time.hour = s_event_fast_table[i].hour;
                second_time.min = s_event_fast_table[i].minute;
                second_time.sec = 0;
                if ( (ALM_MODE_ONCE==s_event_fast_table[i].fre_mode)
                    &&(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time) != CMP_FIRST_SMALL))
                {
                    s_event_fast_table[i].is_on = FALSE;
                }
            }
                result = TRUE;
                //SCI_TRACE_LOW:"MMIALM_UpdateEvents enter! i=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1826_112_2_18_2_0_53_18,(uint8*)"d",i);
            }
            service_id = s_event_fast_table[i].service_id;
            MMIALMSVC_FreeAlmRegService(service_id, FALSE);//CR136024
            // 设置闹钟服务
            SetAlarmService(i, FALSE);
        }
    else if(s_event_fast_table[i].is_valid && (!s_event_fast_table[i].is_on))
    {
        if (i<=EVENT_ALARM_MAX) // && EVENT_ALARM_0 <= i  /*lint !e685 !e568*/
        {
            TM_GetSysDate(&cur_date);
            TM_GetSysTime(&cur_time);
            second_time.hour = s_event_fast_table[i].hour;
            second_time.min = s_event_fast_table[i].minute;
            second_time.sec = 0;
            second_date.year = s_event_fast_table[i].year;
            second_date.mon  = s_event_fast_table[i].mon;
            second_date.mday = s_event_fast_table[i].day;
            if ((MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
                &&(ALM_MODE_ONCE==s_event_fast_table[i].fre_mode)
                && s_event_fast_table[i+EVENT_ALARM_DELAY_0].is_on)
            {
                s_event_fast_table[i].is_on = TRUE;
                s_event_fast_table[i+EVENT_ALARM_DELAY_0].is_on=FALSE;
                result = TRUE;
                service_id = s_event_fast_table[i].service_id;
                MMIALMSVC_FreeAlmRegService(service_id, FALSE);//CR136024
                SetAlarmService(i, FALSE);
            }
        }
    }
    }

    if (result)//快表被修改
    {
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
        // 开启闹钟
        MMIALMSVC_SetAlmService();
    }

    //
    FindLatestEventToSet();
}

/*****************************************************************************/
//  Description : init alarm
//  Global resource dependence : 
//  Author:      liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_Init(void)//该函数在开机时调用
{
    uint16                  fast_nv_return = 0;
#if 1//defined(ADULT_WATCH_SUPPORT) || !defined (SCREEN_SHAPE_CIRCULAR)//解决w217在关机状态下无法起闹
    uint16                  content_nv_return = 0;
    uint32                  i = 0;
#endif
    //uint16                  nv_return = 0;
    //BOOLEAN                 is_set = FALSE;  
    BOOLEAN                 is_events_updated = FALSE;

    MMIACC_SMART_EVENT_CONTENT_T   event_content = {0};

    // init alarm flag
    /*MMINV_READ(MMINV_ACC_ALARM_FLAG, &is_set, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
        is_set = FALSE;
        MMINV_WRITE(MMINV_ACC_ALARM_FLAG, &is_set);
    }*/

    SCI_MEMSET(s_event_fast_table,0x00,(sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T)*MMIACC_EVENT_MAX_NUM));

    MMINV_READ(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table, fast_nv_return);
#if 0 //defined (SCREEN_SHAPE_CIRCULAR) && !defined(ADULT_WATCH_SUPPORT)  //解决w217在关机状态下无法起闹
    //W217儿童圆 -- donothing, 闹钟初始化时为:无闹钟
#else
    for (i=0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        s_event_fast_table[i].service_id = 0; // 初始化的时候id都是0.这是个动态ID，每次开机都会不同
        MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content, content_nv_return);

        if ((MN_RETURN_SUCCESS != fast_nv_return)||(MN_RETURN_SUCCESS != content_nv_return))
        {
#if defined(ADULT_WATCH_SUPPORT)
            event_content.ring = ALM_DEFAULT_ALARM_RINGTONG;         // default is music No.13 

            s_event_fast_table[i].hour = 8;
            s_event_fast_table[i].minute = 0;
            if ( i<= EVENT_ALARM_DELAY_MAX) 
            {
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_ALARM;
                //for cr122785,将闹钟设置每天改为单次
                s_event_fast_table[i].fre_mode = ALM_MODE_ONCE;
                s_event_fast_table[i].fre = 0x3e; // 表示周一到周五选中
                event_content.ring_type = ALM_FIX_RING;
#ifdef CMCC_UI_STYLE
                event_content.is_vibrate = FALSE;
#endif
                event_content.ring = 1;  
                event_content.w_length = 0;
            }
            //@Lin.Lin, add for auto power on/off, start
            else if(EVENT_AUTO_POWERON == i)
#else
            if(EVENT_AUTO_POWERON == i)
#endif
            {
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AUTOPOWERON;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e; // 表示周一到周五选中
            }
            else if(EVENT_AUTO_POWEROFF == i)
            {
                s_event_fast_table[i].hour = 0;
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AUTOPOWEROFF;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e; // 表示周一到周五选中
            }
            //@Lin.Lin, add for auto power on/off, end
            if ((i >= EVENT_SCH_0)&&(i <= EVENT_SCH_MAX))//cr137019
            {
                s_event_fast_table[i].type = SMART_EVENT_SCH;
            }
#ifdef MMI_AZAN_SUPPORT
            if ((i >= EVENT_AZAN_ALARM_1)&&(i <= EVENT_AZAN_ALARM_6))
            {               
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AZAN;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x7f; 
            }
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
            if (i == EVENT_FM_TIMER_RECORD)
            {               
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].is_on = FALSE;
                s_event_fast_table[i].type = SMART_FM_TIMER_RECORD;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e; 
            }
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
            if (i == EVENT_SOGOU_UPDATE)
            {
                VOCABULARY_UPDATE_FREQUENCY_E freq = VOCABULARY_UPDATE_FREQUENCY_E_ONE;
                SCI_TIME_T time;
                if (MN_RETURN_SUCCESS != MMI_ReadNVItem(IMNV_SOGOU_UPDATE_FREQUENCY, &freq))
                {
                    freq = VOCABULARY_UPDATE_FREQUENCY_E_NEVER;
                }
                if (MN_RETURN_SUCCESS != MMI_ReadNVItem(IMNV_SOGOU_UPDATE_TIME, &time))
                {           
                    SCI_MEMSET(&time, 0, sizeof(SCI_TIME_T));
                }
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].is_on = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_SOGOU_UPDATE;
                switch (freq)
                {
                case VOCABULARY_UPDATE_FREQUENCY_E_ONE:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_SEVEN:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYWEEK;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERY15DAYS;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_THIRTY:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYMONTH;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_NEVER:
                default:
                    s_event_fast_table[i].fre_mode = ALM_MODE_NO;
                    s_event_fast_table[i].is_on = FALSE;
                    break;
                }
                s_event_fast_table[i].fre = 0x7F; 
                s_event_fast_table[i].hour = time.hour;
                s_event_fast_table[i].minute = time.min;
            }
#endif
            MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);
            is_events_updated = TRUE;
        }
        else
        {
            if( s_event_fast_table[i].is_valid )
            {
//                 SCI_ALARM_T alm_time = {0};
                MMIALMSVC_EVENT_REG_INFO_T event_info = {0};
                event_info.callback = MMIAPIALM_StartAlarm;
                event_info.event_type = ALMSVC_EVENT_SYSTEM;
                event_info.event_time.hour = s_event_fast_table[i].hour;
                event_info.event_time.min = s_event_fast_table[i].minute;
                event_info.event_time.year = s_event_fast_table[i].year;
                event_info.event_time.mon = s_event_fast_table[i].mon;
                event_info.event_time.mday = s_event_fast_table[i].day;

                if( ALM_FIX_RING == event_content.ring_type && MMISET_MAX_RING_ID < event_content.ring )
                {
                    event_content.ring = ALM_DEFAULT_ALARM_RINGTONG;
                    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);
                    is_events_updated = TRUE;
                }
                // 注册服务，并记录服务的ID

				if (s_event_fast_table[i].is_on)
				{
					s_event_fast_table[i].service_id = MMIALMSVC_RegService(&event_info, s_event_fast_table[i].is_on, FALSE);
					is_events_updated = TRUE;
				}
            }
        }
    }
#endif
    if (is_events_updated)
    {
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
    }
    MAIN_SetIdleAlarmState(IsAlarmSet());
    
#ifndef WIN32
    if(RESTART_BY_ALARM != POWER_GetRestartCondition())
    {
        // 开启闹钟
        MMIALMSVC_SetAlmService();
        FindLatestEventToSet();
        //        MMIALM_UpdateEvents();
    }
#endif
#ifdef IKEYBACKUP_SUPPORT
    MMIAPISCH_ikeybackup_register();
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : memset 0 to &s_event_fast_table[event_id],but hold s_event_fast_table[event_id].service_id
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void CleanEventFastTable(uint16 event_id)
{
    MMIALMSVC_ID_T      temp_service_id = 0;
    
    temp_service_id = s_event_fast_table[event_id].service_id;  
    SCI_MEMSET(&s_event_fast_table[event_id], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
    s_event_fast_table[event_id].service_id = temp_service_id;
}

/*****************************************************************************/
//  Description : restore factory setting of alarm
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_RestoreFactorySetting(void)//清除自动开关机,闹钟/日程保留
{
    //BOOLEAN     is_set = FALSE;
    uint32      i = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    MMIALMSVC_ID_T      temp_service_id = 0;   
//    SCI_TRACE_LOW("MMIAPIALM_ClearUserData");

    //clear all NV, do in setting

    //reset alarm flag
    //MMINV_WRITE(MMINV_ACC_ALARM_FLAG, &is_set);

    //reset alarm records
    for (i = EVENT_SCH_0;i<=EVENT_SCH_MAX;i++)
    {
        temp_service_id = s_event_fast_table[i].service_id;
        SCI_MEMSET(&s_event_fast_table[i], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[i].type = SMART_EVENT_SCH;
        // 循环删除，不需要每次都更新闹钟
        MMIALMSVC_UnRegService(temp_service_id, FALSE);
    }
//    SCI_MEMSET(&s_event_fast_table[i], 0, (sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T) * MMIACC_EVENT_MAX_NUM));

    SCI_MEMSET(&event_content, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T));
#if defined (SCREEN_SHAPE_CIRCULAR) && !defined(ADULT_WATCH_SUPPORT)
        //W217儿童圆 -- donothing, 闹钟初始化时为:无闹钟
        SCI_MEMSET(s_event_fast_table,0x00,(sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T)*MMIACC_EVENT_MAX_NUM));
        //bug 2139233 add for auto power on/off, start
        for (i=0; i< MMIACC_EVENT_MAX_NUM; i++)
        {
            if(EVENT_AUTO_POWERON == i)
            {
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AUTOPOWERON;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e; // 表示周一到周五选中
            }
            else if(EVENT_AUTO_POWEROFF == i)
            {
                s_event_fast_table[i].hour = 0;
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AUTOPOWEROFF;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e; // 表示周一到周五选中
            }
        }
        //add for auto power on/off, end
#else
    for(i = 0; i < (MMIACC_ALARM_NUM + MMIACC_DELAY_ALARM_NUM); i++)
    {
        CleanEventFastTable(i); 
        s_event_fast_table[i].hour = 8;
        s_event_fast_table[i].is_valid = TRUE;
        s_event_fast_table[i].type = SMART_EVENT_ALARM;
        //for cr122785,将闹钟设置每天改为单次
        s_event_fast_table[i].fre_mode = ALM_MODE_ONCE; 
        s_event_fast_table[i].fre = 0x3e; // 表示周一到周五选中                
        event_content.ring_type = ALM_FIX_RING;
#ifdef CMCC_UI_STYLE
        event_content.is_vibrate = FALSE;
#endif
        event_content.ring = 1;
        event_content.w_length = 0;
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);
        temp_service_id = s_event_fast_table[i].service_id;
        MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
        SetAlarmService(i, FALSE);
    }

    //reset auto power on/off
    i = EVENT_AUTO_POWERON;
    CleanEventFastTable(i);
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].hour = 8;
    s_event_fast_table[i].type = SMART_EVENT_AUTOPOWERON;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;                
    s_event_fast_table[i].fre = 0x3e;    
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);
        
    i = EVENT_AUTO_POWEROFF;
    CleanEventFastTable(i);         
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].hour = 0;
    s_event_fast_table[i].type = SMART_EVENT_AUTOPOWEROFF;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;      
    s_event_fast_table[i].fre = 0x3e;    
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);

#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    i = EVENT_FM_TIMER_RECORD;
    CleanEventFastTable(i);         
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].is_on = FALSE;
    s_event_fast_table[i].hour = 8;
    s_event_fast_table[i].type = SMART_FM_TIMER_RECORD;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;      
    s_event_fast_table[i].fre = 0x3e;
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    i = EVENT_SOGOU_UPDATE;
    CleanEventFastTable(i);
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].is_on = FALSE;
    s_event_fast_table[i].hour = 0;
    s_event_fast_table[i].minute = 0;
    s_event_fast_table[i].type = SMART_EVENT_SOGOU_UPDATE;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;      
    s_event_fast_table[i].fre = 0x7F;
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);
#endif
#endif

    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table); 

    MMIALMSVC_SetAlmService();

    FindLatestEventToSet();
}

/*****************************************************************************/
//  Description : check whether there's active alarm
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN  MMIALM_HasActiveEvent ( void)
{
    return IsAlarmSet();
}

/*****************************************************************************/
//  Description : get valid event match with current date and time
//  Global resource dependence : 
//  RETURN :  if event found, return TRUE
//  Author:   taul.zhu
//  Note: 当MMI收到闹铃消息时，获得和当时日期时间相匹配的事件，然后更新事件(删除或者
//      计算下一次发生的时间，修改事件)
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_GetArrivedEvent(
                              MMIACC_SMART_EVENT_T *event_ptr,
                              uint16               *event_id
                              )
{
    uint16                      i              = 0; 
    BOOLEAN                     result         = FALSE;
    SCI_DATE_T                  cur_date       = {0};
    SCI_TIME_T                  cur_time       = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T           event_mod      = {0};
    const MMIACC_SMART_EVENT_FAST_TABLE_T     *cur_event_ptr = NULL;

    if (PNULL == event_ptr || PNULL == event_id)
    {
        //MMI_ASSERT_WARNING("PNULL != event_ptr, NULL != event_id");/*assert verified*/
        //SCI_TRACE_LOW:"MMIALM_GetArrivedEvent PNULL == event_ptr || PNULL == event_id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2186_112_2_18_2_0_54_19,(uint8*)"");
        return FALSE;/*lint !e527*/
    }

    // if alarm after ACTIVE MODE is support !
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

    // find first coming EVENT, just find ONE
    for (i = 0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        cur_event_ptr = MMIALM_GeEvent(i);

        if (cur_event_ptr->is_valid && 
            cur_event_ptr->is_on && 
            IsMatchEvent(cur_date, cur_time, cur_event_ptr) )
        {   
            MMIAPIALM_FormInteEventInfo(i,event_ptr);
            MMIAPIALM_GetRingInfo(i, event_ptr);
            MMI_MEMCPY(
                (void*)&event_mod, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                (void*)cur_event_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

            //此时若是在关机动画界面,则延时30s
            if (MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID))
            {
                SCI_ALARM_T             latest_alarm    = {0};

                latest_alarm.year = cur_event_ptr->year;
                latest_alarm.mon = cur_event_ptr->mon;
                latest_alarm.mday = cur_event_ptr->day;
                latest_alarm.hour = cur_event_ptr->hour;
                latest_alarm.min = cur_event_ptr->minute;
                latest_alarm.sec  = ALM_POWEROFF_DELAY_SEC;//半分钟左右

                if (TM_SetAlarm(latest_alarm)!= ERR_TM_NONE)
                {
                }
                //SCI_TRACE_LOW:"mmialarm.c MMIALM_GetArrivedEvent when poweroff"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2225_112_2_18_2_0_54_20,(uint8*)"");
                break;
            }  
            
            if (SMART_EVENT_ALARM == event_mod.type ||
                SMART_EVENT_AUTOPOWERON == event_mod.type ||
                SMART_EVENT_AUTOPOWEROFF == event_mod.type 
#ifdef MMI_FM_TIMER_RECORD_SUPPORT                
                ||SMART_FM_TIMER_RECORD == event_mod.type 
#endif
                )
            {   
#if 0 //modified by shipeng.lv at sprd for cr149822
//对于一次的闹钟也应该关闭
                //cr149955 will be resolved after cr149822 resolved.
                //shipeng.lv add at sprd. no code is modified for cr149955.
                //check cr149955 after modified code is merged.
                if (ALM_MODE_ONCE == event_mod.fre_mode)
                    &&(SMART_EVENT_AUTOPOWERON == event_mod.type ||
                    SMART_EVENT_AUTOPOWEROFF == event_mod.type)
                )
#else
                if (ALM_MODE_ONCE == event_mod.fre_mode)
#endif
                {
                    event_mod.is_on = FALSE;
                }

                MMIALM_ModifyEvent(i, &event_mod);

                if (SMART_EVENT_AUTOPOWERON == event_mod.type)
                {
                    PHONE_STARTUP_CONDITION_E   e_startup_cond = STARTUP_NONE;
                    
                    e_startup_cond = MMIAPIPHONE_GetStartUpCondition();

                    if(e_startup_cond == STARTUP_NORMAL)
                    {
                        continue;//开机情况下起自动开机,不做起闹
                    }
                }
            }
            else
            {
                if(ALM_MODE_ONCE == event_mod.fre_mode)
                {
                    /*event_mod.is_valid = FALSE;
                    event_mod.is_on = FALSE;
                    MMIALM_DelOneEvent(i);*/
                    SCI_MEMCPY((uint8 *)(&s_event_fast_table[i]), (uint8 *)&event_mod, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
                    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
                }
                else
                {
                    MMIALM_ModifyEvent(i, &event_mod);
                }
            }

            *event_id = i;
            result = TRUE;
            break;
        }
    } 
   
    return result;    
}

/*****************************************************************************/
//  Description : find if the alarm is set
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAlarmSet(void)
{
    BOOLEAN                     is_alarm        = FALSE;
    uint32                      i               = 0;    
    const MMIACC_SMART_EVENT_FAST_TABLE_T     *cur_event_ptr  = NULL;
    const MMIACC_SMART_EVENT_FAST_TABLE_T     *cur_event_delay_ptr  = NULL;
    
    for (i = 0; i< MMIACC_ALARM_NUM; i++)
    {
        cur_event_ptr = MMIALM_GeEvent(i);
        cur_event_delay_ptr = GetDelayEvent(i);
    
        if ((SMART_EVENT_ALARM == cur_event_ptr->type && cur_event_ptr->is_on && cur_event_ptr->is_valid)
        ||(SMART_EVENT_ALARM == cur_event_delay_ptr->type && cur_event_delay_ptr->is_on && cur_event_delay_ptr->is_valid)
            )
        {
            is_alarm = TRUE;
            break;
        }
    }

    return (is_alarm);
}

/*****************************************************************************/
//  Description : find latest active smart event, and set to alarm according to 
//                current SYS time and date
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindLatestEventToSet(void)
{
    BOOLEAN                 result          = TRUE;
    uint32                  i               = 0;    
    //uint32                  event_id        = 0;    
    //BOOLEAN                   is_set          = FALSE;
    SCI_DATE_T              cur_date        = {0};
    SCI_TIME_T              cur_time        = {0};
//     SCI_ALARM_T             latest_alarm    = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T       event           = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T       latest_event    = {0}; 
    MMIACC_SMART_EVENT_FAST_TABLE_T       cur_point       = {0};
    BOOLEAN                 is_alarm        = FALSE;

//    SCI_TRACE_LOW("mmialarm.c:FindLatestEventToSet() enter");

    SCI_MEMSET( &event, 0, sizeof( MMIACC_SMART_EVENT_FAST_TABLE_T));   
    SCI_MEMSET( &latest_event, 0, sizeof( MMIACC_SMART_EVENT_FAST_TABLE_T));   
    
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    cur_point.year = cur_date.year;    //  initial value to first compare
    cur_point.mon = cur_date.mon;
    cur_point.day = cur_date.mday;
    cur_point.hour = cur_time.hour;
    cur_point.minute = cur_time.min;
    latest_event.year = 0xFFFF;

    for(i = 0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        event = s_event_fast_table[i];

        if (event.is_valid && event.is_on)
        {
            //得到当前时间之后最近的起闹时间
            if (MMIAPIALM_CompareTwoEvent(&event, &cur_point) == CMP_SECOND_SMALL &&
                MMIAPIALM_CompareTwoEvent(&latest_event, &event) == CMP_SECOND_SMALL)
            {
                //event_id = i;
                latest_event = event;
            }
        }
    }  

    if(0xffff == latest_event.year)    // not find valid event 
    {
        // TM_DisableAlarm(); 
        //is_set = FALSE;
        result = FALSE;
        // 设置闹钟服务
        //MMIALMSVC_SetAlmService();
    }
    else
    {
//         SCI_ALARM_T alm_time = {0};
//         MMIALMSVC_EVENT_REG_INFO_T event_info = {0};

//         latest_alarm.year = latest_event.year;
//         latest_alarm.mon = latest_event.mon;
//         latest_alarm.mday = latest_event.day;
//         latest_alarm.hour = latest_event.hour;
//         latest_alarm.min = latest_event.minute;
//         latest_alarm.sec  = 0;
        
//         event_info.callback = MMIAPIALM_StartAlarm;
//         event_info.event_type = ALMSVC_EVENT_SYSTEM;
//         
//         event_info.event_time.hour = latest_event.hour;
//         event_info.event_time.min = latest_event.minute;
//         event_info.event_time.year = latest_event.year;
//         event_info.event_time.mon = latest_event.mon;
//         event_info.event_time.mday = latest_event.day;
//         event_info.event_time.sec = 0;

//         if (MMIALMSVC_IsValidSvc(latest_event.service_id))
//         {
//             MMIALMSVC_UpdateServcice(&event_info, latest_event.service_id, TRUE); // 更新服务
//         }
//         else
//         {
//             latest_event.service_id = MMIALMSVC_RegService(&event_info, latest_event.is_on); // 注册服务
//             s_event_fast_table[event_id].service_id = latest_event.service_id;
//         }
        
//         if (TM_SetAlarm(latest_alarm)!= ERR_TM_NONE)
//         {
//         }
//        is_set = TRUE;
    }

    //MMINV_WRITE(MMINV_ACC_ALARM_FLAG, &is_set);
    is_alarm = IsAlarmSet();
    MAIN_SetIdleAlarmState(is_alarm);

    return result;
}

/*****************************************************************************/
//  Description : SetNextAlarmTimeByCurrent
//  Global resource dependence : 
//  Author:  liqing.peng
//  Note:  根据event_ptr是否开启、类型(闹钟、自动开关机、日程)、
//         模式(一次、每天、每周等)、时间日期，
//         计算event_ptr下一次发生的时间
/*****************************************************************************/
LOCAL BOOLEAN SetNextAlarmTimeByCurrent(
                                        MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr         //in & out
                                        )
{
    SCI_ALARM_T             first = {0};
    SCI_ALARM_T             second = {0};
    BOOLEAN                 result = FALSE;
    SCI_DATE_T              second_date = {0};
    SCI_TIME_T              second_time = {0};
    SCI_DATE_T              cur_date = {0};
    SCI_TIME_T              cur_time = {0};
    uint32                  offset_day = 0;
    uint32                  i = 0;
    uint16                  year = 0;
    uint8                   month = 0;
    uint8                   day = 0;
    SCI_DATE_T              event_start_date = {0};
    SCI_DATE_T              event_end_date = {0};
    
    if (!(event_ptr->is_valid && event_ptr->is_on))
    {
        return FALSE;
    }
#ifdef CALENDAR_SUPPORT    
    if(SMART_EVENT_SCH == event_ptr->type)
    {
        if(!MMICALENDAR_IsDateValid(event_ptr->year, 
            event_ptr->mon, 
            event_ptr->day))
        {
            return FALSE;
        }
    }
#endif
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    
    second_time.hour = event_ptr->hour;
    second_time.min = event_ptr->minute;
    second_time.sec = 0;
    
    switch(event_ptr->type)
    {
    case SMART_EVENT_ALARM:
    case SMART_EVENT_AUTOPOWERON:
    case SMART_EVENT_AUTOPOWEROFF:
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    case SMART_FM_TIMER_RECORD:
#endif
        switch(event_ptr->fre_mode)
        {
        case ALM_MODE_ONCE:
        case ALM_MODE_EVERYDAY:
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
            {
                event_ptr->year   = cur_date.year;
                event_ptr->mon    = cur_date.mon;
                event_ptr->day    = cur_date.mday;
                result = TRUE;
            }
            else
            {
                if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                {
                    //                    SCI_TRACE_LOW("SetNextAlarmTimeByCurrent() 2099!");
                    result = FALSE;
                }
                else
                {
                    IncreaseDateByOne(&cur_date);
                    event_ptr->year   = cur_date.year;
                    event_ptr->mon    = cur_date.mon;
                    event_ptr->day    = cur_date.mday;
                    result = TRUE;
                }
                
            }
            break;
            
        case ALM_MODE_EVERYWEEK:
            //SCI_PASSERT(0 != (0x7f & event_ptr->fre), ("SetNextAlarmTimeByCurrent: event_ptr->fre is %d",event_ptr->fre));/*assert verified*/

            if(0 == (0x7f & event_ptr->fre))
            {
                //SCI_TRACE_LOW:"SetNextAlarmTimeByCurrent: event_ptr->fre is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2512_112_2_18_2_0_55_21,(uint8*)"d",event_ptr->fre);
                break;
            }
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
            {
                offset_day = RoundWeek(cur_date.wday, event_ptr->fre);
            }
            else
            {
                offset_day = RoundWeek2(cur_date.wday, event_ptr->fre);
            }
            
            for (i=0; i <offset_day; i++)
            {
                if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                {
                    break;
                }
                IncreaseDateByOne(&cur_date);
            }
            if (i == offset_day)
            {
                event_ptr->year   = cur_date.year;
                event_ptr->mon    = cur_date.mon;
                event_ptr->day    = cur_date.mday;
                result = TRUE;
            }
            else
            {
                //                SCI_TRACE_LOW("SetNextAlarmTimeByCurrent() 2099!");
                result = FALSE;
            }                
            break;
            
        default:
            break;
        }
        break;
        
        case SMART_EVENT_SCH:
#ifdef MMI_AZAN_SUPPORT
        case SMART_EVENT_AZAN:
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
        case SMART_EVENT_SOGOU_UPDATE:
#endif
            second_date.year = event_ptr->year;
            second_date.mon  = event_ptr->mon;
            second_date.mday = event_ptr->day;
            event_start_date.year = event_ptr->start_year;
            event_start_date.mon = event_ptr->start_mon;
            event_start_date.mday = event_ptr->start_day;
            
            event_end_date.year = event_ptr->end_year;
            event_end_date.mon = event_ptr->end_mon;
            event_end_date.mday = event_ptr->end_day;
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, event_start_date, second_time) == CMP_FIRST_SMALL)//cur_date < event_start_date的情况
            {
                event_ptr->year   = event_start_date.year;
                event_ptr->mon    = event_start_date.mon;
                event_ptr->day    = event_start_date.mday;
                result = TRUE;
            }
            else if ( (SMART_EVENT_SCH==event_ptr->type) && (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, event_end_date, second_time) != CMP_FIRST_SMALL))
            {
                result = TRUE; // 已经过期了
            }
            else//event_start_date < cur_date < event_date的情况,或event_date < cur_date 的情况
            {       
                //event date is expired
                switch(event_ptr->fre_mode)
                {
                case ALM_MODE_ONCE:
                    if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL)
                    {
                        result = TRUE;
                    }
                    else
                    {
                        if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                        {
                            result = FALSE;
                        }
                        else
                        {
                            result = TRUE;
                        }
                    }
                    break;
                    
                case ALM_MODE_EVERYDAY:
                    if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
                    {
                        event_ptr->year   = cur_date.year;
                        event_ptr->mon    = cur_date.mon;
                        event_ptr->day    = cur_date.mday;
                        result = TRUE;
                    }
                    else
                    {
                        if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                        {
                            result = FALSE;
                        }
                        else
                        {
                            IncreaseDateByOne(&cur_date);
                            event_ptr->year   = cur_date.year;
                            event_ptr->mon    = cur_date.mon;
                            event_ptr->day    = cur_date.mday;
                            result = TRUE;
                        }
                    }
                    break;
                    
                case ALM_MODE_EVERYWEEK:
                    if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
                    {
                        offset_day = RoundWeek(cur_date.wday, event_ptr->fre);
                    }
                    else
                    {
                        offset_day = RoundWeek2(cur_date.wday, event_ptr->fre);
                    }
                    
                    for (i=0; i <offset_day; i++)
                    {
                        if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                        {
                            break;
                        }
                        IncreaseDateByOne(&cur_date);
                    }
                    if (i == offset_day)
                    {
                        event_ptr->year   = cur_date.year;
                        event_ptr->mon    = cur_date.mon;
                        event_ptr->day    = cur_date.mday;
                        result = TRUE;
                    }
                    else
                    {
                        //                    SCI_TRACE_LOW("SetNextAlarmTimeByCurrent() 2099!");
                        result = FALSE;
                    }  
                    break;
                    
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
                case ALM_MODE_EVERY15DAYS:
                        second_date.year = cur_date.year;
                        second_date.mon  = cur_date.mon;
                        second_date.mday = event_ptr->day;
#ifdef CALENDAR_SUPPORT
                        if((event_ptr->day <= (MMICALENDAR_GetMonthday(cur_date.year, cur_date.mon)-15) &&
                           (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)))
#else
                        if(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
#endif
                        {
                            event_ptr->year   = cur_date.year;
                            event_ptr->mon    = cur_date.mon;
                        }
                        else
                        {
                            year = cur_date.year;
                            month = cur_date.mon;
                            day = event_ptr->day;
#ifdef CALENDAR_SUPPORT
                            MMICALENDAR_MoveDaysForward(year, month, day, 15, &year, &month, &day);
#endif
                            event_ptr->year = year;
                            event_ptr->mon = month;
                            event_ptr->day = day;
                        }
                        result = TRUE;
                        break;
#endif
                case ALM_MODE_EVERYMONTH:
                    //check current month has the day,  and time in this month hasn't expire
                    second_date.year = cur_date.year;
                    second_date.mon  = cur_date.mon;
                    second_date.mday = event_ptr->day;
#ifdef CALENDAR_SUPPORT                    
                    if((event_ptr->day <= MMICALENDAR_GetMonthday(cur_date.year, cur_date.mon) &&
                        (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL)))
#else
                    if(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL)
#endif
                    {
                        //start from this month
                        event_ptr->year   = cur_date.year;
                        event_ptr->mon    = cur_date.mon;
                    }
                    else
                    {
                        //start from next month
                        year = cur_date.year;
                        month = cur_date.mon;
                        day = event_ptr->day;
#ifdef CALENDAR_SUPPORT
                        MMICALENDAR_MoveDaysForward(year, month, day, 
                            MMICALENDAR_GetMonthday(year, month),
                            &year, &month, &day);
#endif
                        if(day == event_ptr->day)
                        {
                            //next month has the day
                            event_ptr->year = year;
                            event_ptr->mon = month;
                        }
                        else
                        {
                            //next month hasn't the day, move next month again
                            event_ptr->year = year;
                            event_ptr->mon = month+1;
                        }
                    }
                    
                    result = TRUE;
                    break;
                    
                case ALM_MODE_EVERYYEAR:
                    //check current year has the day, and time in this year hasn't expire
                    second_date.year = cur_date.year;
                    second_date.mon  = event_ptr->mon;
                    second_date.mday = event_ptr->day; 
                    
                    if((2 != event_ptr->mon || 29 > event_ptr->day || MMIAPICOM_IsLeapYear(cur_date.year)) &&
                        (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL))
                    {
                        //start from this year
                        event_ptr->year = cur_date.year;
                    }
                    else
                    {
                        //start from next year
                        if(2 == event_ptr->mon && 29 == event_ptr->day)
                        {
                            //leap year
                            event_ptr->year = (cur_date.year+4)/4*4;
                            if(!MMIAPICOM_IsLeapYear(event_ptr->year))
//                             {
//                                 event_ptr->year = event_ptr->year;
//                             }
//                             else
                            {
                                event_ptr->year = event_ptr->year+4;
                            }
                        }
                        else
                        {
                            //normal year
                            event_ptr->year = cur_date.year+1;
                        }
                    }
                    
                    result = TRUE;
                    break;
                    
                default:
                    break;
            }
        }
        
        // 判断是否过期
        first.year  = event_ptr->year;
        first.mon  = event_ptr->mon;
        first.mday = event_ptr->day;
        first.hour = 0;
        first.min = 0;
        first.sec = 0;          // ignore second when compare to event
        
        second.year = event_ptr->end_year;
        second.mon = event_ptr->end_mon;
        second.mday = event_ptr->end_day;
        second.hour = 0;
        second.min  = 0;
        second.sec = 0;        // ignore second 
        
        if(CompareDateTime(first, second) == CMP_SECOND_SMALL)
        {
            event_ptr->year = cur_date.year;
            event_ptr->mon = cur_date.mon;
            event_ptr->day = cur_date.mday;
        }
        break;
        
    default:
        break;
    }
    
    if(result)
    {
        //SCI_TRACE_LOW:"SetNextAlarmTimeByCurrent(), success, year = %d, month = %d, day = %d, hour = %d, minute = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2795_112_2_18_2_0_55_22,(uint8*)"ddddd", event_ptr->year, event_ptr->mon, event_ptr->day, event_ptr->hour, event_ptr->minute);
    }
    else
    {
        //SCI_TRACE_LOW:"SetNextAlarmTimeByCurrent(), fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2799_112_2_18_2_0_55_23,(uint8*)"");
    }
    
    return result;
}
//pengliqing, 2006-07-28, about alarm, schedule, auto power on/off module Kernel Functions, end

/*****************************************************************************/
//  Description : SetNextAlarmTimeByCurrent
//  Global resource dependence : 
//  Author:  liqing.peng
//  Note:  根据event_ptr是否开启、类型(闹钟、自动开关机、日程)、
//         模式(一次、每天、每周等)、时间日期，
//         计算event_ptr下一次发生的时间
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_SetNextAlarmTimeByCurrent(
                                        MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr         //in & out
                                        )
{
    return SetNextAlarmTimeByCurrent(event_ptr);
}

/*****************************************************************************/
//  Description : open alarm main window
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenMainWin(void)
{
    MMK_CreateWin((uint32 *)ALARM_MAINWIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : open auto power on/off main window
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUTOPOWER_OpenMainWin(void)
{
    MMK_CreateWin((uint32 *)AUTOPOWER_MAINWIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : 从wday向后找到第一个匹配的，返回offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek(
                      uint8 wday,
                      uint8 fre
                      )
{
    uint8   offset = 0;
    uint32  i = 0;

    for (i = wday; i < ALM_MODE_WEEK_NUM; i++)
    {
        if (fre & s_week_mask[i])
        {
            break;
        }
        offset++;
    }
    if (i < ALM_MODE_WEEK_NUM)
    {
        return offset;
    }
    for (i = 0; i < wday; i++)
    {
        if (fre & s_week_mask[i])
        {
            break;
        }
        offset++;
    }
    return offset;
}

/*****************************************************************************/
//  Description : 从wday后一天向后找到第一个匹配的，返回offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek2(
                      uint8 wday,
                      uint8 fre
                      )
{
    if (6 != wday)
    {
        return (RoundWeek((uint8)(wday+1), fre)+1);
    }
    else
    {
        return (RoundWeek(0, fre)+1);
    }
}

/*****************************************************************************/
//  Description : CompareDateTime
//         compare date and time to another group, find smaller(earlier)
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL int16 CompareDateTime(
                            SCI_ALARM_T first, 
                            SCI_ALARM_T second
                            )
{
    if(first.year > second.year)
    {
        return CMP_SECOND_SMALL;
    }
    else if(first.year < second.year)
    {
        return CMP_FIRST_SMALL;
    }
    else
    {
        if(first.mon > second.mon)
        {
            return CMP_SECOND_SMALL;
        }
        else if(first.mon < second.mon)
        {
            return CMP_FIRST_SMALL;
        }
        else
        {
            if(first.mday > second.mday)
            {
                return CMP_SECOND_SMALL;
            }
            else if(first.mday < second.mday)
            {
                return CMP_FIRST_SMALL;
            }
            else
            {
                if(first.hour > second.hour)
                {
                    return CMP_SECOND_SMALL;
                }
                else if(first.hour < second.hour)
                {
                    return CMP_FIRST_SMALL;
                }
                else
                {
                    if(first.min >second.min)
                    {
                        return CMP_SECOND_SMALL;
                    }
                    else if(first.min <second.min)
                    {
                        return CMP_FIRST_SMALL;
                    }
                    else
                    {
                        return CMP_EQUAL;
                    }
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : compare two event, find smaller with date and time
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note: 比较的两个event都要包括完整的date和time
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_CompareTwoEvent(
                const MMIACC_SMART_EVENT_FAST_TABLE_T *latest_event_ptr,
                const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 
                ) 
{
   SCI_ALARM_T first, second;

    first.year  = latest_event_ptr->year;
    first.mon  = latest_event_ptr->mon;
    first.mday = latest_event_ptr->day;
    first.hour = latest_event_ptr->hour;
    first.min = latest_event_ptr->minute;
    first.sec = 0;          // ignore second when compare to event

    second.year = event_ptr->year;
    second.mon = event_ptr->mon;
    second.mday = event_ptr->day;
    second.hour = event_ptr->hour;
    second.min  = event_ptr->minute;
    second.sec = 0;        // ignore second 
    
    return CompareDateTime(first, second);
}

/*****************************************************************************/
//  Description : 比较event，是否和当前日期时间匹配
//  Global resource dependence :  
//  Author: bin.ji
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsMatchEvent(
                           SCI_DATE_T                   cur_date, 
                           SCI_TIME_T                   cur_time, 
                           const MMIACC_SMART_EVENT_FAST_TABLE_T   *event_ptr
                           )
{
    BOOLEAN     result = FALSE;
    MMI_TM_T    time  = {0};
    if (PNULL == event_ptr)
    {
        //MMI_ASSERT_WARNING("NULL != event_ptr");/*assert verified*/
        //SCI_TRACE_LOW:"IsMatchEvent:NULL == event_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3002_112_2_18_2_0_56_24,(uint8*)"");
        return FALSE;/*lint !e527*/
    }
    time.tm_year = event_ptr->year;
    time.tm_mon = event_ptr->mon;
    time.tm_mday = event_ptr->day;
    time.tm_hour = event_ptr->hour;
    time.tm_min = event_ptr->minute;
#ifdef CMCC_UI_STYLE
    time = MMIALM_GetSchTime(
                                    event_ptr->year, 
                                    event_ptr->mon, 
                                    event_ptr->day, 
                                    event_ptr->hour, 
                                    event_ptr->minute, 
                                    event_ptr->remind_type, 
                                    TRUE);
#endif
    
    if(time.tm_year == cur_date.year &&
       time.tm_mon == cur_date.mon &&
       time.tm_mday == cur_date.mday &&
       time.tm_hour == cur_time.hour &&
       time.tm_min == cur_time.min)
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 比较两个日期时间
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_CompareDateAndTime(
                                SCI_DATE_T first_date, 
                                SCI_TIME_T first_time, 
                                SCI_DATE_T second_date, 
                                SCI_TIME_T second_time 
                                )
{
    SCI_ALARM_T     first = {0};
    SCI_ALARM_T     second = {0};

    first.year  = first_date.year;
    first.mon  = first_date.mon;
    first.mday = first_date.mday;
    first.hour = first_time.hour;
    first.min = first_time.min;
    first.sec = 0;          // ignore second when compare to event

    second.year = second_date.year;
    second.mon = second_date.mon;
    second.mday = second_date.mday;
    second.hour = second_time.hour;
    second.min  = second_time.min;
    second.sec = 0;        // ignore second 
    
    return CompareDateTime(first, second);
}

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetSameTimeSchEvent(
                                 const MMIACC_SMART_EVENT_FAST_TABLE_T *latest_event, // IN: 比较的event
                                 uint16 event_id,                    // IN: event的id
                                 uint16 start_id,                    // IN: start id
                                 MMIACC_SCH_LIST_T *p_sch_list       // OUT:
                                 )
{
    uint16                  i                   = 0; 
    uint16                  num                 = 0;
    MMIACC_SMART_EVENT_FAST_TABLE_T    event                = {0};

    //SCI_PASSERT(event_id < MMIACC_EVENT_MAX_NUM && start_id < MMIACC_EVENT_MAX_NUM, ("GetSameTimeSchEvent: event_id is %d, start_id is %d",event_id,start_id));  /*assert verified*/    
    if(event_id >= MMIACC_EVENT_MAX_NUM || start_id >= MMIACC_EVENT_MAX_NUM)
    {
        return num;
    }
    
    //SCI_ASSERT(NULL != latest_event);/*assert verified*/
    //SCI_ASSERT(NULL != p_sch_list); /*assert verified*/
    if(NULL == latest_event||NULL == p_sch_list)
    {
        return num;
    }

    SCI_MEMSET((uint8 *)p_sch_list, 0, sizeof(MMIACC_SCH_LIST_T));

    for(i = start_id; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        SCI_MEMCPY((uint8 *)&event, (uint8 *)&s_event_fast_table[i], sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

        if (event.is_valid && event.is_on && i != event_id)
        {
            if (MMIAPIALM_CompareTwoEvent(latest_event, &event) == CMP_EQUAL)
            {
                num++;
                p_sch_list->entry_id[p_sch_list->total] = i;
                p_sch_list->total++;
            }
        }
    }  
    
    return num;
}

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneSchEventInList(
                                     uint16 index, // IN
                                     MMIACC_SCH_LIST_T *p_sch_list       // IN OUT:
                                     )
{
    uint16                  i                   = 0; 

    //SCI_ASSERT(NULL != p_sch_list); /*assert verified*/
    if (PNULL == p_sch_list)
    {
        return  FALSE;
    }
    
    if(index>=p_sch_list->total)
    {
        return FALSE;
    }

    if(index == p_sch_list->total - 1)
    {
        p_sch_list->entry_id[index] = 0;
        p_sch_list->total--;
    }
    else
    {
        for(i = index; i < p_sch_list->total - 1; i++)
        {
            p_sch_list->entry_id[i] = p_sch_list->entry_id[i + 1];
        }
        p_sch_list->total--;
        p_sch_list->entry_id[p_sch_list->total] = 0;
    }
    
    return TRUE;
}

/*****************************************************************************/
// Description : IncreaseTimeBySecond
// increase current time by seconds inputed.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:   
/*****************************************************************************/
LOCAL  void IncreaseTimeBySecond(
                                 SCI_DATE_T *pdate,  // inout
                                 SCI_TIME_T *ptime,  // inout
                                 uint16      sec            // in
                                 )
{
    uint16 i = 0;

    i = ptime->sec + sec;
    if (i < 60)
    {
        ptime->sec = i;
    }
    else
    {
        // ptime->sec = i - 60;
        ptime->sec = i%60;
        ptime->min += i/60;

        ptime->hour += ptime->min/60;
        ptime->min = ptime->min%60;

        for (i = 0; i < ptime->hour/24; i++)
        {
            IncreaseDateByOne(pdate);
        }
        ptime->hour = ptime->hour%24;
    }
}

/*****************************************************************************/
//  Description : increase current date by one
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL  void IncreaseDateByOne(
                              SCI_DATE_T  *pdate_ptr
                              )
{
    if (pdate_ptr->mon<1 ||pdate_ptr->mon>12)
    {
        //SCI_TRACE_LOW:"IncreaseDateByOne error! mon=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3181_112_2_18_2_0_56_25,(uint8*)"d",pdate_ptr->mon);
        return;
    }
    if(MMIAPICOM_IsLeapYear((pdate_ptr->year)))
    {
        if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon = 1;
            pdate_ptr->year++; 
        }
        else if(pdate_ptr->mday == s_RTC_month_info_leap[pdate_ptr->mon-1])
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon++;
        }
        else
        {
            pdate_ptr->mday++;
        }
    }
    else        // not leap year
    {
        if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon = 1;
            pdate_ptr->year++;
        }
        else if(pdate_ptr->mday == s_RTC_month_info[pdate_ptr->mon-1])
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon++;
        }
        else
        {
            pdate_ptr->mday++;
        }
    }

    if (pdate_ptr->year >MMICOM_DATE_MAX_YEAR)
    {
        pdate_ptr->year = MMICOM_DATE_MIN_YEAR;       // when 2099, return 1980
    }
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : start timer
//	Global resource dependence : 
//  Author:       maryxiao
//	Note: 
/*****************************************************************************/
// LOCAL uint8 CreateClockTimer(MMI_WIN_ID_T  win_id, ALARM_CLOCK_TYPE_E alarm_type)
// {
//     uint16 time_out= 0;
// 
//     switch(alarm_type)
//     {
//     case ALM_CLOCK_TYPE_1:
//         time_out = CLOCK_REFRESH_SEC_TIME;
//         break;
//     case ALM_CLOCK_TYPE_2:
//          time_out = CLOCK_REFRESH_MIN_TIME;
//         break;
//     default:
//         time_out = CLOCK_REFRESH_SEC_TIME;
//         break;
//     }
//     return MMK_CreateWinTimer(win_id, time_out, TRUE);
// }

/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: maryxiao
//  Note:
/*****************************************************************************/
// LOCAL void StopClockTimer(void)
// {
//     if (0 != s_refresh_timer_id )
//     {
//         MMK_StopTimer(s_refresh_timer_id );
//         s_refresh_timer_id  = 0;
//     }
//     else
//     {
//         //SCI_TRACE_LOW:"[StopClockTimer]: StopTimer: the timer has stopped!"
//         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3264_112_2_18_2_0_56_26,(uint8*)"");
//     }
// }
#endif
/*****************************************************************************/
//  Description : the process message function of the ALARM
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmMainWinMsg( 
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    uint16                      i               = 0;        
    MMI_RESULT_E                recode          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               current_ctrl_id = MMIALM_LISTALL_CTRL_ID;
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = NULL;
//#if !defined MMI_GUI_STYLE_TYPICAL
//    SCI_TIME_T      sys_time = {0};
//    uint16          width = 0;
//    uint16          height = 0;
//    GUI_RECT_T      rect_info =MMITHEME_GetClientRect();
//#endif
                
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:      //create check list box
//#if !defined MMI_GUI_STYLE_TYPICAL
       // if(!MMITHEME_IsMainScreenLandscape())//only display when not landscape
      //  {
      //      s_refresh_timer_id = CreateClockTimer(win_id, s_alm_clk_type);
      //  }       
//#endif        
        GUILIST_SetMaxItem( current_ctrl_id, ALM_ALARM_NUM, FALSE );//max item 3

        for (i = 0; i< ALM_ALARM_NUM; i++)
        {
            uint16 event_id = (uint16)( EVENT_ALARM_0 + i );
            const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent(event_id);

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
            AppendThreeLineListItem( current_ctrl_id,
                TXT_NULL,
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                FALSE );
#else
            AppendTwoLineListItem( current_ctrl_id,
                TXT_NULL,
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                FALSE );
				//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 	     	SetListItemStyle(current_ctrl_id, i);
// #endif
#endif
        }
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;
        
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_TIMER:
        if((*(uint8*)param) == s_refresh_timer_id)
        {
            MMK_UpdateScreen();
        }
        break;
#endif        

    case MSG_LOSE_FOCUS:
// #if !defined MMI_GUI_STYLE_TYPICAL
//  //       StopClockTimer();
// #endif
        break;
        
    case MSG_GET_FOCUS:  
// #if !defined MMI_GUI_STYLE_TYPICAL
//         //s_refresh_timer_id = CreateClockTimer(win_id, s_alm_clk_type);
// #endif        
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:	
	/*	if(!MMITHEME_IsMainScreenLandscape())//only display when not landscape
        {
            GUI_POINT_T point;
             GUI_RECT_T      rect_clock =MMITHEME_GetClientRect();
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            GUIRES_GetImgWidthHeight(&width, &height, s_alm_clock_arr[s_alm_clk_type].bg_img_id, win_id);
            rect_clock.top = rect_clock.top + MMI_STATUSBAR_HEIGHT;//+ MMITHEME_GetWinTitleHeight();//+ MMI_STATUSBAR_HEIGHT; 
            rect_clock.bottom = rect_clock.top+height;
            if((point.y>=rect_clock.top) &&(point.y<=rect_clock.bottom))
            {
                s_alm_clk_type++;
                if(s_alm_clk_type == ALM_CLOCK_TYPE_MAX)
                {
                    s_alm_clk_type = ALM_CLOCK_TYPE_1;
                }
                StopClockTimer();
                s_refresh_timer_id = CreateClockTimer(win_id, s_alm_clk_type);
                GUIRES_GetImgWidthHeight(&width, &height, s_alm_clock_arr[s_alm_clk_type].bg_img_id, win_id);
                rect_info.top = rect_info.top + height+ MMI_STATUSBAR_HEIGHT;//+ MMITHEME_GetWinTitleHeight();//+ MMI_STATUSBAR_HEIGHT; 
                GUILIST_SetRect(current_ctrl_id, &rect_info);
                MMK_UpdateScreen();
            }
        }*/		
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
 
     case MSG_LCD_SWITCH: 
     #if 0
        if(!MMITHEME_IsMainScreenLandscape())//only display when not landscape
        {
            //少图，不应该用widget的图
            GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WIDGET_ACLOCK, win_id);
            rect_info.top = rect_info.top + height + MMI_STATUSBAR_HEIGHT;//+ MMITHEME_GetWinTitleHeight();//+ MMI_STATUSBAR_HEIGHT; 
            GUILIST_SetRect(current_ctrl_id, &rect_info);
        }
      #endif
         break;
#ifdef TOUCH_PANEL_SUPPORT
     case MSG_CTL_LIST_CHECK_CONTENT:
         {
             // save select
             s_current_event_info.id = GUILIST_GetCurItemIndex(current_ctrl_id);

             MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
             event_delay_ptr = GetDelayEvent(s_current_event_info.id);
             
             s_current_event_info.type = SMART_EVENT_ALARM;
             s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
             
             if (s_current_event_info.event.event_fast_info.is_on ||event_delay_ptr->is_on)
             {
                 s_current_event_info.event.event_fast_info.is_on = FALSE;
             }
             else
             {
                 s_current_event_info.event.event_fast_info.is_on = TRUE;
             }
             // 关闭延时闹钟
             SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
             // 修改闹钟设置
             MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);      
             MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); 
         }
         break;
#endif

     case MSG_FULL_PAINT:
        {
            BOOLEAN is_update = TRUE;
            
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
            uint16 cur_index = GUILIST_GetCurItemIndex(current_ctrl_id);
            is_update = FALSE;
            GUILIST_RemoveAllItems(current_ctrl_id);
#endif
         for ( i = 0; i < ALM_ALARM_NUM; i++ )
         {
             uint16 event_id = (uint16)( EVENT_ALARM_0 + i );
             const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
             AppendThreeLineListItem( current_ctrl_id,
                 TXT_NULL,
                 s_alarm_mode_text[event_ptr->fre_mode],
                 event_id,
                 i,
                 is_update);
#else
             AppendTwoLineListItem( current_ctrl_id,
                TXT_NULL,
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                is_update );
				//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 			SetListItemStyle(current_ctrl_id, i);
// #endif
#endif
            }
            
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
         GUILIST_SetCurItemIndex(current_ctrl_id, cur_index);
#endif
        }
         break;
         
     case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_APP_WEB:
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
        //开启、关闭
        {
            uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(current_ctrl_id));
            
            if(ALM_ALARM_NUM > id)
            {
                s_current_event_info.id = id;
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                event_delay_ptr = GetDelayEvent(s_current_event_info.id);

                s_current_event_info.type = SMART_EVENT_ALARM;
                s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;

                if (s_current_event_info.event.event_fast_info.is_on ||event_delay_ptr->is_on)
                {
                    s_current_event_info.event.event_fast_info.is_on = FALSE;
#ifdef CALENDAR_SUPPORT
                     MMIAPISCH_DeleteUnreadEvent(s_current_event_info.id);
#endif

                }
                else
                {
                    s_current_event_info.event.event_fast_info.is_on = TRUE;
                }
                // 关闭延时闹钟
                SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
                // 修改闹钟设置
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);		
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);	
            }
        }
#else
         s_current_event_info.id = GUILIST_GetCurItemIndex(current_ctrl_id);
         //         SCI_MEMCPY((uint8 *)&s_current_event_info.event, 
         //                    (uint8 *)MMIALM_GeEvent((uint16)(EVENT_ALARM_0+s_current_event_info.id)), 
         //                    sizeof(MMIACC_SMART_EVENT_T));
         MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event); 
         if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
         {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
         }
         s_current_event_info.type = SMART_EVENT_ALARM;
         s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
         
         MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
#ifdef CALENDAR_SUPPORT
         MMIAPISCH_DeleteUnreadEvent(s_current_event_info.id);
#endif
#endif
         break;

    case MSG_APP_OK:
    case MSG_CTL_OK:        // goes to setting menu of alarm
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
        // save select
        MMK_CreateWin((uint32 *)MMIALM_OPTWIN_TAB, PNULL);
#else
        s_current_event_info.id = GUILIST_GetCurItemIndex(current_ctrl_id);
//         SCI_MEMCPY((uint8 *)&s_current_event_info.event, 
//                    (uint8 *)MMIALM_GeEvent((uint16)(EVENT_ALARM_0+s_current_event_info.id)), 
//                    sizeof(MMIACC_SMART_EVENT_T));
        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
        event_delay_ptr = GetDelayEvent(s_current_event_info.id);

        s_current_event_info.type = SMART_EVENT_ALARM;
        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;

        if (s_current_event_info.event.event_fast_info.is_on ||event_delay_ptr->is_on)
        {
        s_current_event_info.event.event_fast_info.is_on = FALSE;
        }
        else
        {
        s_current_event_info.event.event_fast_info.is_on = TRUE;
        }
        // 关闭延时闹钟
        SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
        // 修改闹钟设置
        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);       
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); 
#endif
        break;

    case MSG_CLOSE_WINDOW:
// #if !defined MMI_GUI_STYLE_TYPICAL
//         //StopClockTimer();
// #endif
        break;

//如果clock中仅剩alarm,则不响应tab切换
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYREPEAT_LEFT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID,  GUITAB_SWITCH_PRE);
        }
        break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID, GUITAB_SWITCH_NEXT);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(!MMK_IsChildWin(win_id))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMK_CloseParentWin(MMI_TIMER_PARENT_WIN_ID);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : get the delayed event of event_id
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T *GetDelayEvent(uint16 event_id)
{
    /*switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        event_id = EVENT_ALARM_DELAY_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        event_id = EVENT_ALARM_DELAY_1;
        break;

    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        event_id = EVENT_ALARM_DELAY_2;
        break;
        
    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        event_id = EVENT_ALARM_DELAY_3;
        break;
        
    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        event_id = EVENT_ALARM_DELAY_4;
        break;
        
    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        event_id = EVENT_ALARM_DELAY_5;
        break;
            
    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        event_id = EVENT_ALARM_DELAY_6;
        break;
            
    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        event_id = EVENT_ALARM_DELAY_7;
        break;
            
    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        event_id = EVENT_ALARM_DELAY_8;
        break;
            
    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        event_id = EVENT_ALARM_DELAY_9;
        break;

    default:
        break;
    }*/
    if(event_id<=EVENT_ALARM_MAX)
    {
        event_id=event_id+EVENT_ALARM_DELAY_0;
    }

    return &s_event_fast_table[event_id];
}

/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T    temp_str = {0};
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_HANDLE_T    win_handle=0;
    uint16          event_id=0;
    //static BOOLEAN is_operate_backLight=FALSE;
    if (PNULL != win_info_ptr)
    {   
        win_info_ptr->win_id = win_id;
    }

    win_handle=MMK_ConvertIdToHandle(win_id);
#ifdef MMI_ETWS_SUPPORT
    if( MMIALM_ALIVEWIN_ID == win_id )
    {
        SCI_TRACE_LOW("HandleEventAliveWinMsg, MMIALM_ALIVEWIN_ID");
    }
    else if(MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id )
    {
        SCI_TRACE_LOW("HandleEventAliveWinMsg, MMIALM_ALIVEWIN_DURING_ETWS_ID");
    }
#endif

    //SCI_TRACE_LOW:" HandleEventAliveWinMsg, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3609_112_2_18_2_0_57_27,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef SCREEN_SHAPE_CIRCULAR
        {
            GUI_FONT_T  tempFont    = WATCH_DEFAULT_BIG_FONT;
            GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;
            GUITEXT_SetFont( ADULT_WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID, &tempFont, &fontColor );
        }
#endif
        s_open_event_id = s_arrived_event_id;//CR192240
        if (SMART_EVENT_ALARM == s_arrived_event.event_fast_info.type)
        {
#ifdef MMI_ALARM_MINI_SUPPORT
            MMIPUB_SetWinSoftkey(MMIALM_ALIVEWIN_ID, TXT_CLOSE, TXT_ALARM_DELAY_MINI, FALSE);
#else
            MMIPUB_SetWinSoftkey(MMIALM_ALIVEWIN_ID, TXT_CLOSE, TXT_ALARM_DELAY, FALSE);
#endif
            MMIPUB_SetWinTitleTextId(win_id,TXT_ALARM_CLOCK,FALSE);
        }
        else if(SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
        {
            MMIPUB_SetWinTitleTextId(win_id,TXT_IDLE_CALENDAR,FALSE);
        }
        //刷新小屏显示，显示闹钟等事件信息
        if (SMART_EVENT_ALARM ==s_arrived_event.event_fast_info.type) // assigne default text                
        {                
            MMI_GetLabelTextByLang(TXT_ALARM_CLOCK, &temp_str);                
        }
        else                
        {                
            temp_str.wstr_len = s_arrived_event.event_content_info.w_length;                 
            temp_str.wstr_ptr = s_arrived_event.event_content_info.w_content;                 
        } 

#ifdef MMI_ETWS_SUPPORT
        if( TRUE == s_is_need_ring_and_vib)
        {
#endif
            if((MMK_GetWinPriority(win_handle)==WIN_LOWEST_LEVEL)
                ||(MMIAPICC_IsInState(CC_IN_CALL_STATE)))//后台提醒情况
            {
                StartAlarmTimer(&s_event_timer_id, BACKGROUND_ALERT_RING_DURATION, FALSE);
            }
            else
            {

#ifdef CMCC_UI_STYLE
                if(s_arrived_event.event_content_info.ring_dur > 0)
                {
                    StartAlarmTimer(&s_event_timer_id, s_arrived_event.event_content_info.ring_dur*60*1000, FALSE);//ALERT_RING_DURATION
                }
                else
                {
                    StartAlarmTimer(&s_event_timer_id, ALERT_RING_DURATION, FALSE);
                }
#else
                StartAlarmTimer(&s_event_timer_id, ALERT_RING_DURATION, FALSE);
#endif
            }

            StartRingOrVib();//U盘使用中的情况也在该函数中处理

#ifdef MMI_ETWS_SUPPORT
        }
        else
        {
            s_is_need_ring_and_vib = TRUE;
        }
#endif

#ifdef SCREEN_SHAPE_CIRCULAR
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, PNULL);
#else
		recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
#endif

        //以下是点亮屏幕的操作,需放在MMIPUB_HandleWaitWinMsg之后处理
        if ( STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
        {
            if(/*MMIDEFAULT_IsAllowBackLightTurnOff()&&*/ MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
            {
                //MMIDEFAULT_TurnOnBackLight();
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                //is_operate_backLight=TRUE;
            }
        }
        else
        {
            //MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            //is_operate_backLight=TRUE;
        }
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_ALARM,&temp_str);  
#endif     
        MMK_WinGrabFocus(MMK_ConvertIdToHandle(win_id)); 
        break;

    case MSG_FULL_PAINT:
        {        

#ifdef MMI_ETWS_SUPPORT
           if( MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id)
            {
                break;
            }
#endif

            if(PNULL != win_info_ptr)
            {
                DisplayAlarmWin(win_id,win_info_ptr);
            }
        }
        break;
        
    case MSG_GET_FOCUS:
        if(MMK_GetWinPriority(win_handle)==WIN_LOWEST_LEVEL)//后台提醒情况下不用做任何事。
        {//修改详情参考CR122782的comments
            break;
        }
        if (s_event_timer_id ==0)
        {
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
            }
#endif
        }
        else
        {
            if ( STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
            {
                if(/*MMIDEFAULT_IsAllowBackLightTurnOff()&&*/ MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
                {
                    MMIDEFAULT_TurnOnBackLight();
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                    //is_operate_backLight=TRUE;
                }
            }
            else
            {
                MMIDEFAULT_TurnOnBackLight();
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                //is_operate_backLight=TRUE;
            }
//修改详情参考CR122782的comments
#ifdef MMI_KEY_LOCK_SUPPORT
            if(MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(win_id,MSG_ALM_START_RING,PNULL,0);//解决来电挂断铃声起不来问题
            }
            else
#endif
            {
                StartRingOrVib();
            }
        }
        break;
    case MSG_ALM_START_RING:
        StartRingOrVib();
        break;

    case MSG_LOSE_FOCUS:
//修改详情参考CR122782的comments
        StopRingOrVib(FALSE);
        //SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, TRUE);
        //if(is_operate_backLight)
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            //is_operate_backLight=FALSE;
        }
        break;
#ifdef  SCREEN_SHAPE_CIRCULAR
        //close alarm
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_PROMPTWIN_OK:
#ifdef MMI_ETWS_SUPPORT
        if( MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id)
        {
            break;
        }
#endif
#endif
    //case MSG_KEYDOWN_FLIP://合盖应该和按停止键同样处理,@zhaohui,cr102341      
        if (s_arrived_event_id < EVENT_SCH_0)
        {
            if(s_arrived_event_id<=EVENT_ALARM_MAX) //&&s_arrived_event_id>=EVENT_ALARM_0 /*lint !e685 !e568*/
            {
                event_id = s_arrived_event_id;
            }
            else if(s_arrived_event_id>=EVENT_ALARM_DELAY_0&&s_arrived_event_id<=EVENT_ALARM_DELAY_MAX)
            {
               event_id = s_arrived_event_id-EVENT_ALARM_DELAY_0; 
            }
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_DeleteUnreadEvent(event_id);
#endif            
            StopRingOrVib(TRUE);
            CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);
            if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE)&& !MMIAPIMMS_IsSendingOrRecving())
                &&(0==s_bypast_list.total)
                )
            {
                CheckandStartExpiredEvent();
                //这种情况下不能MMK_CloseWin(win_id);否则刚打开的自动关机窗口又关掉了；
                //先调用MMK_CloseWin，后调用CheckandStartExpiredEvent是可以的，只是 会引起CR225729 。
            }
            else
            {
                CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO
#ifndef SCREEN_SHAPE_CIRCULAR
                if (PNULL != win_info_ptr)//因为日程widget也要调用这个函数，所以加入保护，防止widget子窗口被关闭
                {
                    MMK_CloseWin(win_id);
                }
#else
                    MMK_CloseWin(win_id);
#endif
            }

            
        }
        break;
        
    case MSG_TIMER:
        if (s_vibrator_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            if (is_vibrator_on)
            {
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ALARM);
                is_vibrator_on = FALSE;
            }
            else
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
                is_vibrator_on = TRUE;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
        }
        else if (s_event_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            // 停止闹钟
            if(is_ring_on)
            {
                StopRingOrVib(TRUE);
            }
            else
            {
                MMK_StopTimer(s_event_timer_id);
                s_event_timer_id = 0;
            }
            
            if(s_vibrator_timer_id!=0)
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            is_vibrator_on = FALSE;
#ifdef CALENDAR_SUPPORT            
            MMIAPISCH_AddUnreadEvent(s_arrived_event_id);
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                if(MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)||MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
                }
            }
#endif
            // 设置延时的闹钟
            SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, TRUE);
            //            MMK_CloseWin(win_id);
            
            //CheckandStartExpiredEvent();
            //if(is_operate_backLight)
            if(MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
            {
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                //is_operate_backLight=FALSE;
			}
            //shipeng.lv end
#ifdef SCREEN_SHAPE_CIRCULAR
            MMK_CloseWin(win_id);
#else
            if (PNULL != win_info_ptr)//因为日程widget也要调用这个函数，所以加入保护，防止widget子窗口被关闭
            {//修改详情参考CR122782的comments
                // if((MMK_GetWinPriority(win_handle) !=WIN_LOWEST_LEVEL) && (!MMIAPICC_IsInState(CC_IN_CALL_STATE)))//后台提醒情况
                // {
                      MMK_CloseWin(win_id);
                // }
            }
#endif

            //CheckandStartExpiredEvent();
            if ( STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition()) // bug 2119552
            {
                //destroy闹铃界面
                MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

                if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
                {
                    //POWER_PowerOff()之前关闭背光
                    MMIDEFAULT_TurnOffBackLight();
                    MMIDEFAULT_SetKeybadBackLight(FALSE);

                    // 关机
                    POWER_PowerOff();
                }
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP://合盖应该和按停止键同样处理,@zhaohui,cr102341
        StopRingOrVib(TRUE);
        CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);
        if (PNULL != win_info_ptr)//因为日程widget也要调用这个函数，所以加入保护，防止widget子窗口被关闭
        {
            MMK_CloseWin(win_id);
        }
        CheckandStartExpiredEvent();
        recode = MMI_RESULT_FALSE;
        break;      
#endif
        
    case MSG_CLOSE_WINDOW:

#ifdef MMI_ETWS_SUPPORT
        if( MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id)
        {
            MMIAPIALM_OpenAlarmDialogWin();
            break;
        }
#endif

        if(s_event_timer_id != 0)
        {//CR192240
			MMIACC_SMART_EVENT_T open_event = {0};
			MMIAPIALM_FormInteEventInfo(s_open_event_id,&open_event);
            SetDelayAlarmTime(s_open_event_id, open_event, TRUE);
			s_open_event_id = 0;
        }
        StopRingOrVib(TRUE);
        //if(is_operate_backLight)
        if(MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
        {
            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            //is_operate_backLight=FALSE;
        }
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏显示，关闭显示闹钟等事件信息
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_ALARM,PNULL);
#endif

#ifdef SCREEN_SHAPE_CIRCULAR
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, PNULL);
#else
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
#endif

        MMIAPICT_CheckandStartDelayedCoutimeEvent();//@zhaohui,counted time,弹出延迟的倒计时到提示
        break;

    case MSG_KEYDOWN_RED:
    case MSG_KEYUP_POWER:
        StopRingOrVib(TRUE);
        SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, FALSE);
        
        if (
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_IsPhoneLocked()||
#endif
            (s_bypast_list.total > 0)||
            (s_poweroff_event.event_fast_info.is_valid)||
            MMIAPICT_CheckDelayedCoutimeEvent()||
            MMK_IsOpenWin(MMISET_FLY_MODE_OPEN_PHONE_WIN_ID)
            )//锁键或有延时事件，则只关闭当前窗口
        {
            recode = MMI_RESULT_TRUE;   
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }

        if (MMI_RESULT_TRUE == recode||MSG_KEYUP_POWER==msg_id)
        {
            if (PNULL != win_info_ptr)//因为日程widget也要调用这个函数，所以加入保护，防止widget子窗口被关闭
            {
                MMK_CloseWin(win_id);
            }
        }

        CheckandStartExpiredEvent();
        break;
#ifdef  SCREEN_SHAPE_CIRCULAR
        //snooze alarm
        case MSG_CTL_OK:
        case MSG_APP_OK:
#else
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYUP_UPSIDE:
    case MSG_KEYUP_DOWNSIDE:
    case MSG_KEYLONG_UPSIDE:
    case MSG_KEYLONG_DOWNSIDE:
    case MSG_KEYPRESSUP_DOWNSIDE:
    case MSG_KEYPRESSUP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
#endif
        StopRingOrVib(TRUE);
        SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, FALSE);
#ifdef CALENDAR_SUPPORT
        MMIAPISCH_DeleteUnreadEvent(s_arrived_event_id);
#endif      
#if !defined(ADULT_WATCH_SUPPORT) && defined(SCREEN_SHAPE_CIRCULAR)
        WatchPUB_Alert_Note_Win(WATCHALM_DELAY_NOTE_WIN_ID,TXT_ALARM_DELAY_NOTE);
#endif

        // send a msg to vertual win 
        //if ( STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
        //{
        //  MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
        //}
        //else
        //{
            //CheckandStartExpiredEvent();
        //}
        //MMK_CloseWin(win_id);
        if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
                &&(0==s_bypast_list.total)
            )
        {
            CheckandStartExpiredEvent();
            //这种情况下不能MMK_CloseWin(win_id);否则刚打开的自动关机窗口又关掉了；
            //先调用MMK_CloseWin，后调用CheckandStartExpiredEvent是可以的，只是 会引起CR225729 。
        }
        else
        {
            CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO


#ifndef SCREEN_SHAPE_CIRCULAR
            if (PNULL != win_info_ptr)//因为日程widget也要调用这个函数，所以加入保护，防止widget子窗口被关闭
            {
                MMK_CloseWin(win_id);
            }
#else
                MMK_CloseWin(win_id);
#endif

        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_KEYUP_FLIP:
        MMIDEFAULT_HandleFlipKey(TRUE);
        break;
#endif

    default:
#ifdef SCREEN_SHAPE_CIRCULAR
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, PNULL);
#else
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
#endif
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : Stop ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//	Note:   
/*****************************************************************************/
LOCAL void StopRingOrVib(BOOLEAN is_close_ring_timer)
{    
    if (0 != s_vibrator_timer_id )
    {
        MMK_StopTimer(s_vibrator_timer_id);
        s_vibrator_timer_id = 0;
    }

    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ALARM);
    if(is_close_ring_timer)
    {
        if (0 != s_event_timer_id )
        {
            MMK_StopTimer(s_event_timer_id);
            s_event_timer_id = 0;
        }
    }

    if(!is_ring_on)
    {
        return ;
    }

    if (ALM_FIX_RING == s_cur_alarm_ring_type)
    {
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
    }
    else if(ALM_DOWNLOAD_RING == s_cur_alarm_ring_type
        ||ALM_RECORD_RING == s_cur_alarm_ring_type)
    {
        MMIAPISET_StopAppRing();
    }
#ifdef FM_SUPPORT
	else
	{
        MMISRVAUD_Stop(MMIAPIALARM_GetPlayHandle());
		MMIAPIFM_Stop();
        MMIAPIALARM_FreePlaySrv();
	}
#endif
    //WATCHRECORD_ClockEnd();
    is_ring_on=FALSE;
}

/*****************************************************************************/
//  Description : Start timer about alarm
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void StartAlarmTimer(
                           uint8 *timer_id,
                           uint32 time_out,
                           BOOLEAN is_period
                           )
{
    //SCI_ASSERT(timer_id != NULL);/*assert verified*/
    if (NULL == timer_id)
    {
        return;
    }
    
    if (0 != *timer_id)
    {
        MMK_StopTimer(*timer_id);
        *timer_id = 0;
    }
    *timer_id = MMK_CreateTimer(time_out, is_period);
}

/*****************************************************************************/
//  Description : Start ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   
/*****************************************************************************/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
LOCAL void StartRingOrVib(void)
{
//    uint8           ring_vol = 0;
    uint8           opt_type = MMIAPIENVSET_GetActiveModeOptValue(0,ALARM_RING_TYPE); /*lint !e64*/
#ifdef CMCC_UI_STYLE
    BOOLEAN         is_need_vibrate = FALSE;
#endif
//     MMIACC_SMART_EVENT_CONTENT_T	event_content = {0};
    
    //SCI_TRACE_LOW:"mmialarm.c: ring type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4024_112_2_18_2_0_58_28,(uint8*)"d", opt_type);

#ifdef CMCC_UI_STYLE 
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
        ) 
    {
        return;
    }
#endif

#ifdef MET_MEX_SUPPORT
	if(1==MMIMex_Phs_HookResult(MEX_MSG_ID_WILL_PLAY_ALARMTONE, NULL, 0, NULL, MEX_HOOK_MSG_FROM_ALARM))
	{
		mex_log(1, "hook almTone");
		//mex application registered to hook this message
		return;
	}
#endif /* MET_MEX_SUPPORT */

    is_ring_on=TRUE;
    
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef JAVA_SUPPORT
        ||MMIAPIJAVA_IsFocusJAVARunWin()
#endif
//NEWMS00220222
#if defined(BROWSER_SUPPORT) && defined(LOW_MEMORY_SUPPORT_32X16)  
        || MMIAPIBROWSER_IsRunning()   
#endif
#ifdef WRE_SUPPORT
        ||WRE_IsFocusRunWin()
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
		|| MMIAPIATV_IsRecording()
        ) //cr63879
    {
        if((MMISET_MSG_RING == opt_type)||(MMISET_MSG_VIBRA_AND_RING == opt_type))
        {
            opt_type = MMISET_MSG_VIBRA;
        }        
    }

    else if ((MMIAPIFMM_IsFileOption()) ||
             (MMIAPIPICVIEW_IsDeleteFiles()))
    {
        if(MMISET_MSG_RING == opt_type
            || MMISET_MSG_VIBRA_AND_RING == opt_type)
        {
            opt_type = MMISET_MSG_VIBRA;
        }
    }
#ifdef MMI_RECORD_SUPPORT
    WATCHRECORD_ClockStart();
#endif
    switch (opt_type)
    {
    case MMISET_MSG_RING: //ring  
#ifdef MMI_DUALMODE_ENABLE
#ifdef CMCC_UI_STYLE
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_ALARM) && s_arrived_event.event_content_info.is_vibrate)
        {
            is_need_vibrate = TRUE;
        }
#endif
#endif
        StartRingForRingOrVib(MMISET_MSG_RING);
#ifdef CMCC_UI_STYLE
        if(is_need_vibrate)
        {
            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
            if (0 != s_vibrator_timer_id )
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_3SECONDS, FALSE);
            is_vibrator_on = TRUE;
        }
 #endif
        break;

    case MMISET_MSG_VIBRA: //vibration
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_ALARM))
        {
            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
            if (0 != s_vibrator_timer_id )
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
            is_vibrator_on = TRUE;
        }

        break;

    case MMISET_MSG_SILENT: //silent     
        MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_ALARM);
        break;
    
    case MMISET_MSG_VIBRA_AND_RING: // ring&vibration
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_ALARM))
        {
            StartRingForRingOrVib(MMISET_MSG_VIBRA_AND_RING);
            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
            if (0 != s_vibrator_timer_id )
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
            is_vibrator_on = TRUE;
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Start ring for ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   leon.wang add
/*****************************************************************************/
LOCAL void StartRingForRingOrVib(uint8 opt_type)
{
    uint8   ring_vol = 0;
    uint16  ringId = 0;

    if (ALM_FIX_RING == s_arrived_event.event_content_info.ring_type)
    {
        ringId = MMIAPIENVSET_GetFixAlarmRingId( MMIAPIENVSET_GetActiveModeId() );
        MMIAPISET_PlayRingById(TRUE,ringId,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
        s_cur_alarm_ring_type=ALM_FIX_RING;
    }
    else if(ALM_DOWNLOAD_RING == s_arrived_event.event_content_info.ring_type
            ||ALM_RECORD_RING == s_arrived_event.event_content_info.ring_type)
    {               
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
            s_cur_alarm_ring_type=ALM_FIX_RING;
        }
        else
        {
            if(MMIAPIFMM_IsFileExist(s_arrived_event.event_ring_info.dring_name_arr,
                                   s_arrived_event.event_ring_info.dring_name_len))
            {
                ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
                if(0 < ring_vol)
                {
                    //reset audio mode
                    MMIAPISET_SetAudioMode(TRUE);
                    
                    if(!MMIAPIFMM_PlayMusicFile(MMISET_RING_TYPE_ALARM,FALSE,
                                            s_arrived_event.event_ring_info.dring_name_arr,
                                            s_arrived_event.event_ring_info.dring_name_len,
                                            0xffff,
                                            0,
                                            ring_vol,
                                            NULL))
                    {
                        MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
                        s_cur_alarm_ring_type=ALM_FIX_RING;
                    }
                    else
                    {
                        s_cur_alarm_ring_type=s_arrived_event.event_content_info.ring_type;
                    }
                    //reset audio mode
                    MMIAPISET_SetAudioMode(FALSE);
                }
            }
            else
            {
                //s_arrived_event.event_content_info.ring_type = ALM_FIX_RING;
                //s_arrived_event.event_content_info.ring = 1;
                MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
                s_cur_alarm_ring_type=ALM_FIX_RING;
            }
        }
    }
#ifdef FM_SUPPORT
    else
    {               
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);       
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
            s_cur_alarm_ring_type=ALM_FIX_RING;
        }
        else
        {
            MMI_STRING_T  str_ptr={0};
            str_ptr.wstr_ptr=s_arrived_event.event_ring_info.dring_name_arr;
            str_ptr.wstr_len=s_arrived_event.event_ring_info.dring_name_len;
            if( MMIAPIALM_IsValidFM(s_arrived_event.event_ring_info.dring_name_arr,s_arrived_event.event_ring_info.dring_name_len)
	                   && (MMIFM_CheckHeadsetStatusWithoutPrompt()) && (!MMIAPISET_GetFlyMode()))
            {
	            //SCI_TRACE_LOW:"mmialarm.c MMIAPIFM_Play"
                if(MMISET_MSG_RING==opt_type)
                {
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4141_112_2_18_2_0_58_29,(uint8*)"");
                }
                else
                {
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4246_112_2_18_2_0_58_30,(uint8*)"");
                }
                MMIAPIALARM_ReqPlaySrv();
                MMIAPIFM_Play(MMIAPIALARM_GetPlayHandle(),(uint16)AlarmConvertFloatStrToInt(&str_ptr, 10));
				s_cur_alarm_ring_type=ALM_FM_RING;
	        }
            else
            {
                //s_arrived_event.event_content_info.ring_type = ALM_FIX_RING;
                //s_arrived_event.event_content_info.ring = 1;
                MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
                s_cur_alarm_ring_type=ALM_FIX_RING;
            }
        }
    }
#endif
}
/*****************************************************************************/
//  Description : pop up a msg 
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleAlarmExpiredWinMsg(
//                                              MMI_WIN_ID_T       win_id,     
//                                              MMI_MESSAGE_ID_E   msg_id,
//                                              DPARAM             param
//                                              )
// {
//     MMI_RESULT_E    recode = MMI_RESULT_TRUE;
// 
//     //SCI_TRACE_LOW:"mmialarm.c HandleAlarmExpiredWinMsg() msg_id = %x"
//     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4287_112_2_18_2_0_58_31,(uint8*)"d", msg_id);
//     switch(msg_id)
//     {
//     case MSG_GET_FOCUS:
//         MMIDEFAULT_TurnOnBackLight();
//         break;
// 
//     default:
//         break;
//     }
// 
//     recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
//     
//     return recode;
// }

/*****************************************************************************/
//  Description : start delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarmTime(
                                uint16 event_id, 
                                MMIACC_SMART_EVENT_T event,
                                BOOLEAN    is_auto
                                )
{
    BOOLEAN    result   = TRUE;
    SCI_DATE_T cur_date = {0};
    SCI_TIME_T cur_time = {0};
    uint16 delay_event_id = 0;
    MMIALMSVC_ID_T temp_service_id = 0;

    /*switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        delay_event_id = EVENT_ALARM_DELAY_0;
        event_id = EVENT_ALARM_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        delay_event_id = EVENT_ALARM_DELAY_1;
        event_id = EVENT_ALARM_1;
        break;

    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        delay_event_id = EVENT_ALARM_DELAY_2;
        event_id = EVENT_ALARM_2;
        break;

    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        delay_event_id = EVENT_ALARM_DELAY_3;
        event_id = EVENT_ALARM_3;
        break;

    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        delay_event_id = EVENT_ALARM_DELAY_4;
        event_id = EVENT_ALARM_4;
        break;
    
    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        delay_event_id = EVENT_ALARM_DELAY_5;
        event_id = EVENT_ALARM_5;
        break;
    
    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        delay_event_id = EVENT_ALARM_DELAY_6;
        event_id = EVENT_ALARM_6;
        break;
    
    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        delay_event_id = EVENT_ALARM_DELAY_7;
        event_id = EVENT_ALARM_7;
        break;
    
    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        delay_event_id = EVENT_ALARM_DELAY_8;
        event_id = EVENT_ALARM_8;
        break;
    
    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        delay_event_id = EVENT_ALARM_DELAY_9;
        event_id = EVENT_ALARM_9;
        break;
        
    default:
        // 设置下一个日程
        FindLatestEventToSet();
        return FALSE;
        //break;
    }*/
    if(event_id<=EVENT_ALARM_MAX) //&&event_id>=EVENT_ALARM_0 /*lint !e685 !e568*/
    {
        delay_event_id = event_id+EVENT_ALARM_DELAY_0;
        event_id = event_id;
    }
    else if(event_id>=EVENT_ALARM_DELAY_0&&event_id<=EVENT_ALARM_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id-EVENT_ALARM_DELAY_0; 
    }
    else
    {
		temp_service_id = s_event_fast_table[event_id].service_id;
        FindLatestEventToSet();
		MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
        return FALSE;
    }
    

    // 判断是否手动延时
    if (is_auto)// 自动延时
    {
        event.event_fast_info.delay_num++;
    }
    else // 手动延时 设置延时次数为0
    {
        event.event_fast_info.delay_num = 0;
    }

    if (MMIALM_DELAY_MAX_NUM < event.event_fast_info.delay_num)
    {
        event.event_fast_info.is_valid = FALSE;
        event.event_fast_info.is_on = FALSE;
        if (ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode)
        {
            s_event_fast_table[event_id].is_on = FALSE;//关闭延时闹钟的同时,将该延时的原闹钟也关闭
        }
        result = FALSE;
     temp_service_id = s_event_fast_table[delay_event_id].service_id;   
        SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id独立对应与自己的event_id        
    }
    else
    {
        event.event_fast_info.is_on = TRUE;

        TM_GetSysDate(&cur_date);
        TM_GetSysTime(&cur_time);
        cur_date.year = event.event_fast_info.year;
        cur_date.mon = event.event_fast_info.mon;
        cur_date.mday = event.event_fast_info.day;
        cur_time.hour = event.event_fast_info.hour;
        cur_time.min = event.event_fast_info.minute;
        cur_time.sec = 0;
    
        // 计算延时后的时间
        IncreaseTimeBySecond(&cur_date, &cur_time, ALM_DELAY_TIME_SEC);
        //s_event_fast_table[event_id].is_on = TRUE;
        //SCI_MEMCPY(&s_event_fast_table[event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        
        temp_service_id = s_event_fast_table[delay_event_id].service_id;
        SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id独立对应与自己的event_id
    
        s_event_fast_table[delay_event_id].year = cur_date.year;
        s_event_fast_table[delay_event_id].mon = cur_date.mon;
        s_event_fast_table[delay_event_id].day = cur_date.mday;
        s_event_fast_table[delay_event_id].hour = cur_time.hour;
        s_event_fast_table[delay_event_id].minute = cur_time.min; 

      if (ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode)
        {
            s_event_fast_table[event_id].is_on = FALSE;
        }
    };
    // 保存
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_event_id, &event.event_content_info);  

    /*if (ALM_DOWNLOAD_RING == event.event_content_info.ring_type
        ||ALM_FM_RING == event.event_content_info.ring_type)
    {
        SaveRingInfoToFile(delay_event_id,&event.event_ring_info);  
    } */   

    // 设置服务
    MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
    SetAlarmService(delay_event_id, TRUE);
    // 设置最近的闹钟
    FindLatestEventToSet();

    return result;
}

/*****************************************************************************/
//  Description : 从wday后一天向后找到第一个匹配的，返回offset
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  Note:   
/*****************************************************************************/
PUBLIC uint8 MMIAPIALM_RoundWeek(
                              uint8 wday,
                              uint8 fre,
                              uint32 type
                              )
{
    uint8 offset_day = 0;

    switch(type) 
    {
    case CMP_FIRST_SMALL:
        offset_day = RoundWeek(wday, fre);
        break;

    default:
        offset_day = RoundWeek2(wday, fre);
        break;
    }
    
    return offset_day;
}

/*****************************************************************************/
//  Description : increase current date by one
//  Global resource dependence : 
//  Author:        xiaoqinglu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_IncreaseDateByOne(
                                     SCI_DATE_T  *pdate_ptr
                                     )
{
    //SCI_ASSERT(NULL != pdate_ptr);/*assert verified*/
    if(PNULL == pdate_ptr)
    {
        return;
    }
    
    IncreaseDateByOne(pdate_ptr);
}

/*****************************************************************************/
//  Description : close delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN CloseDelayAlarm(
                              uint16 event_id, 
                              MMIACC_SMART_EVENT_FAST_TABLE_T event,
                              BOOLEAN    is_on
                              )
{
    BOOLEAN    result   = TRUE;
    uint16 delay_event_id = 0;
    MMIALMSVC_ID_T temp_service_id = 0;

   /* switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        delay_event_id = EVENT_ALARM_DELAY_0;
        event_id = EVENT_ALARM_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        delay_event_id = EVENT_ALARM_DELAY_1;
        event_id = EVENT_ALARM_1;
        break;
        
    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        delay_event_id = EVENT_ALARM_DELAY_2;
        event_id = EVENT_ALARM_2;
        break;
        
    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        delay_event_id = EVENT_ALARM_DELAY_3;
        event_id = EVENT_ALARM_3;
        break;
        
    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        delay_event_id = EVENT_ALARM_DELAY_4;
        event_id = EVENT_ALARM_4;
        break;
        
    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        delay_event_id = EVENT_ALARM_DELAY_5;
        event_id = EVENT_ALARM_5;
        break;
        
    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        delay_event_id = EVENT_ALARM_DELAY_6;
        event_id = EVENT_ALARM_6;
        break;
        
    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        delay_event_id = EVENT_ALARM_DELAY_7;
        event_id = EVENT_ALARM_7;
        break;
        
    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        delay_event_id = EVENT_ALARM_DELAY_8;
        event_id = EVENT_ALARM_8;
        break;
        
    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        delay_event_id = EVENT_ALARM_DELAY_9;
        event_id = EVENT_ALARM_9;
        break;

    default:
        // 设置下一个日程
        FindLatestEventToSet();
        return FALSE;
        //break;
    }*/
    
    if(event_id<=EVENT_ALARM_MAX) //&&event_id>=EVENT_ALARM_0 /*lint !e685 !e568*/
    {
        delay_event_id = event_id+EVENT_ALARM_DELAY_0;
        event_id = event_id;
    }
    else if(event_id>=EVENT_ALARM_DELAY_0&&event_id<=EVENT_ALARM_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id-EVENT_ALARM_DELAY_0; 
    }
    else
    {
        FindLatestEventToSet();
		temp_service_id = s_event_fast_table[event_id].service_id;
		MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
        return FALSE;
    }

    if (event.is_valid) // 当开启的时候才关闭，如果是关闭状态，就没有必要进行下面的操作。
    {
        event.is_on = is_on;
        event.is_valid = FALSE;
    
        temp_service_id = s_event_fast_table[delay_event_id].service_id;
        SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id独立对应与自己的event_id
        
        if (ALM_MODE_ONCE == s_event_fast_table[event_id].fre_mode)
        {
            s_event_fast_table[event_id].is_on = FALSE;//关闭延时闹钟的同时,将该延时的原闹钟也关闭
        }
#if 0 //modify by shipeng.lv at sprd 2009.08.28 for cr149822
//关闭延时闹钟没必要连原来的闹钟关闭
//对于一次性的闹钟，在MMIALM_StartAlarm中已经关闭
//通过以下方式验证:
//一次的闹钟爆发后关闭时列表中关闭，
//一次的闹钟爆发时延时后列表中显示开启状态，
//多次的闹钟爆发后延时或关闭时列表中显示开启
        if ((!is_on)&&(ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode))
        {
            s_event_fast_table[event_id].is_on = FALSE;//关闭延时闹钟的同时,将该延时的原闹钟也关闭
        }
#endif
        // 保存
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
		MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
        // 设置服务
        SetAlarmService(delay_event_id, TRUE);
        // 设置最近的闹钟
        FindLatestEventToSet();
    }
    return result;
}

/*****************************************************************************/
//  Description : start alarm when interrupt of alarm is coming
//  Global resource dependence : 
//  Author:        taul.zhu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_StartAlarm(
                                 MMIALMSVC_ID_T svc_id, 
                                 MMIALMSVC_EVENT_TYPE_E svc_type
                                 )
{
    BOOLEAN    result = FALSE;
    uint32     i = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T  *event_ptr = NULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T        event_mod = {0};
    MMIACC_SCH_LIST_T           bypast_list = {0};
    BOOLEAN                     is_set_bypast = FALSE;

//  SCI_TRACE_LOW("MMIAPIALM_StartAlarm()");

    s_arrived_event_id = 0;
    s_arrived_event.event_fast_info.is_on = FALSE;

    ClearDelayedEvent();

    result = MMIALM_GetArrivedEvent(&s_arrived_event, &s_arrived_event_id); //find matched event

    if (result) //  got a arrived event
    {
        GetSameTimeSchEvent(&s_arrived_event.event_fast_info, s_arrived_event_id, EVENT_SCH_0, &s_bypast_list);
        {//处理延时事件中的自动开关机事件

            for (i=0;i<s_bypast_list.total;i++)
            {
                // 判断延时事件中是否有EVENT_AUTO_POWERON事件,
                if (EVENT_AUTO_POWERON == s_bypast_list.entry_id[i])//开机情况下起自动开机,则不做处理
                {
                    PHONE_STARTUP_CONDITION_E   e_startup_cond = STARTUP_NONE;
                    
                    e_startup_cond = MMIAPIPHONE_GetStartUpCondition();

                    if(e_startup_cond == STARTUP_NORMAL)
                    {
                        DeleteOneSchEventInList(i, &s_bypast_list);
                        i--;
                    }
                }
                // 判断延时事件中是否有EVENT_AUTO_POWEROFF事件,
                else if (EVENT_AUTO_POWEROFF == s_bypast_list.entry_id[i])
                {
                    MMIAPIALM_FormInteEventInfo(EVENT_AUTO_POWEROFF,&s_poweroff_event);
                    DeleteOneSchEventInList(i, &s_bypast_list);
                }
            }

        }

        //如果有两个以上事件在同一时间发生，只取一个提示
        SCI_MEMSET(((uint8*)&bypast_list), 0, sizeof(MMIACC_SCH_LIST_T));
        GetSameTimeSchEvent(&s_arrived_event.event_fast_info, s_arrived_event_id, s_arrived_event_id, &bypast_list); // 首先取得同时发生的所有事件
        for (i = 0; i < bypast_list.total; i++) // 修改每一个事件
        {
            event_ptr = MMIALM_GeEvent(bypast_list.entry_id[i]); // 得到事件的数据

            MMI_MEMCPY(
                    (void*)&event_mod, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                    (void*)event_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                    sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T)); // copy
            // 一次的闹钟,日程,开关机,直接关闭
            if (ALM_MODE_ONCE == event_mod.fre_mode)
            {
                if (SMART_EVENT_ALARM == event_mod.type ||
                    SMART_EVENT_AUTOPOWERON == event_mod.type ||
                    SMART_EVENT_AUTOPOWEROFF == event_mod.type
 #ifdef MMI_FM_TIMER_RECORD_SUPPORT                   
                    || SMART_FM_TIMER_RECORD == event_mod.type
 #endif
                    )
                {
                    event_mod.is_on = FALSE;
                }       
            }
            
            if (ModifyEvent(bypast_list.entry_id[i], &event_mod, FALSE))// 计算该事件的下一次发生时间,如果是单次,则直接关闭
            {
                is_set_bypast = TRUE;
            }
        }
        if (is_set_bypast) // 上面循环修改的时候，没有设置闹钟，这里统一设置，否则速度会很慢
        {
            MMIALMSVC_SetAlmService();
        }

        //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, find event result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4751_112_2_18_2_0_59_32,(uint8*)"d", result);


        if ( MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
			|| MMIAPICC_IsInState(CC_IN_CALL_STATE)
 #ifdef MMI_FM_TIMER_RECORD_SUPPORT
            || ((MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef MMI_RECORD_SUPPORT
                 ||MMIAPIRECORD_IsRecording()
#endif
                 ||MMIAPIFM_IsFmRecordActive()
                 ||MMIAPIDC_IsOpened())
           && (SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type))
 #endif
            //|| MMIAPIUDISK_IsFormatWaitWinOpened()
            //|| MMIAPISD_IsFormatWaitWinOpened()
            ///|| MMIAPISMS_IsSendingSMS()
            /* added by plum peng for v1 cr to v2 */
           // || MMIAPISTK_IsInSTK()
//#ifdef VT_SUPPORT
//            || MMIAPIVT_IsVtCalling()
//#endif

#ifdef ZDT_VIDEOCHAT_SUPPORT
            ||  (g_is_activeVideo == TRUE)  //视频通话中延迟闹钟事件
            ||  (g_is_inVideo == TRUE)

#endif	
            )
        {   
            // delay processing alarm
            //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, delay alarm win pop"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4773_112_2_18_2_0_59_33,(uint8*)"");
            PushDelayedEvent(); 
            s_arrived_event.event_fast_info.is_on = TRUE;    
            SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, FALSE);//CR192240
        }
        else if (
                (MMIAPICC_IsInState(CC_IN_CALL_STATE) 
                && (s_arrived_event.event_fast_info.type == SMART_EVENT_AUTOPOWEROFF))
                || (MMIAPIMMS_IsSendingOrRecving() 
                && (s_arrived_event.event_fast_info.type == SMART_EVENT_AUTOPOWEROFF))
        )
        {
            /*延时事件记录在s_arrived_event中,但由于此时在自动关机被延时期间,s_arrived_event可能会被起来的闹钟,
            日程等事件冲掉,因此特将被延时的自动关机事件保存至s_poweroff_event中*/
            MMIAPIALM_FormInteEventInfo(EVENT_AUTO_POWEROFF,&s_poweroff_event);
        }       
#ifdef MMI_ETWS_SUPPORT
        else if(TRUE == MMIAPISMSCB_IsETWSAlert())
        {
            ClearDelayedEvent();
            SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, FALSE);
        }
#endif
        else
        {   
            //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, open alarm win at once"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4790_112_2_18_2_0_59_34,(uint8*)"");
            MMIALM_SetAlmInterruptFlag(TRUE);
            result = OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, the alarm is not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4797_112_2_18_2_0_59_35,(uint8*)"");
        MMIALMSVC_SetAlmService();
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            MMIDEFAULT_TurnOffBackLight();
            POWER_PowerOff();
        }
    }

    return result;
}


/*****************************************************************************/
//  Description : open alarm expired win according to the alarm type
//  Global resource dependence : 
//  Author:      lin.lin
//  Note:            
/*****************************************************************************/
LOCAL BOOLEAN OpenAlarmExpiredWin(
                                  MMIACC_SMART_EVENT_E    type
                                  )
{
    BOOLEAN                     recode = TRUE;
    PHONE_STARTUP_CONDITION_E   e_startup_cond = STARTUP_NONE;
    
    e_startup_cond = MMIAPIPHONE_GetStartUpCondition();

    //SCI_TRACE_LOW:"mmialarm.c: OpenAlarmExpiredWin(), type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4823_112_2_18_2_1_0_36,(uint8*)"d", type);
    ClearDelayedEvent();

    switch(type)
    {
    case SMART_EVENT_AUTOPOWERON:
        if((e_startup_cond == STARTUP_CHARGE) ||
           (e_startup_cond == STARTUP_ALARM))
        {
            if (MMK_IsOpenWin(PHONE_STARTUP_CHARGE_WIN_ID)) 
            {
                MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
            }       
//            MMIDEFAULT_TurnOnBackLight();

            if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID)) 
            {
                MMK_CloseWin(MMIALM_ALIVEWIN_ID);
            }

            OpenAlarmAliveWin(WIN_ONE_LEVEL);
            // 自动开机时清除自动关机的数据
            SCI_MEMSET(&s_poweroff_event, 0, sizeof(MMIACC_SMART_EVENT_T));
        }
        else // 开机状态下不响应开机事件,判断是否同时还有有关机事件，日程事件等
        {
            CheckandStartExpiredEvent();
        }
        break;

    case SMART_EVENT_AUTOPOWEROFF:
        if ((e_startup_cond != STARTUP_CHARGE) &&
            (e_startup_cond != STARTUP_ALARM))
        {
//            MMIDEFAULT_TurnOnBackLight();

            if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID)) 
            {
                MMK_CloseWin(MMIALM_ALIVEWIN_ID);
            }

            OpenAlarmAliveWin(WIN_ONE_LEVEL);
        }
        else
        {
            MMIALMSVC_SetAlmService();
            if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
            {
                MMIDEFAULT_TurnOffBackLight();
                POWER_PowerOff();
            }
            recode = FALSE;
        }
        break;

    default:
#ifdef CMCC_UI_STYLE
//在直播当中，不管收到闹钟还是日程提醒异常事件，不仅要有震动提示，而且当退出直播时，需要弹出该异常事件，所以这里必须设置为延迟事件；
#ifdef VIDEO_PLAYER_SUPPORT  
        if (MMIAPIVP_IsLiving())  //add for NEWMS00173193
        {
            SCI_TRACE_LOW("MMIALM_StartAlarm, delay alarm win pop");
            PushDelayedEvent(); 
            s_arrived_event.event_fast_info.is_on = TRUE;    
        }
#endif
#endif
        if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID)) 
        {
            MMK_CloseWin(MMIALM_ALIVEWIN_ID);
        }       
        if(IsNeedBackgroundPrompt())
        {
            OpenAlarmAliveWin(WIN_LOWEST_LEVEL);
        }
        else        
        {
            OpenAlarmAliveWin(WIN_ONE_LEVEL);
        }
        break;
    }

    return recode; 
}

/***********************************************************************************
 Name:          MMIALM_IsAlarmExpired
 Description:   if an alarm/event is waiting for occuring
 Input:     
 Return:         True if  an alarm is waiting for prompt
***********************************************************************************/
PUBLIC BOOLEAN MMIALM_IsAlarmExpired(void)
{
    return s_is_event_delayed;
}

/*****************************************************************************/
//  Description : open alarm expired win according to the alarm type
//  Global resource dependence : 
//  Author:      lin.lin
//  Note:            
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenAlarmExpiredWin(void)
{
    OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
}

/*****************************************************************************/
//  Description : check to see if there is a delayed alarm and activate it
//  Global resource dependence : 
//  Author:        taul.zhu
//  RETRUN:  TRUE:  delayed alarm is activated
//  Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_CheckAndStartDelayedAlarm(void)
{

    //SCI_TRACE_LOW:"MMIAPIALM_CheckAndStartDelayedAlarm(), is_delayed = %d, is_on = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4924_112_2_18_2_1_0_37,(uint8*)"dd", s_is_event_delayed, s_arrived_event.event_fast_info.is_on);

#ifdef WIN32
    s_arrived_event.event_fast_info.is_on = TRUE;
#endif    

    if (s_is_event_delayed && s_arrived_event.event_fast_info.is_on)        // delayed event is valid
    {
        MMK_PostMsg((MMI_WIN_ID_T)VIRTUAL_WIN_ID, MSG_SET_OPENALARM_EXPIRED_WIN, PNULL, 0);
        //OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
        s_arrived_event.event_fast_info.is_on = FALSE;
        
        return TRUE;
    }
    else if (s_poweroff_event.event_fast_info.is_valid)
    {
        MMI_MEMCPY(
            (void*)&s_arrived_event, sizeof(MMIACC_SMART_EVENT_T),
            (void*)&s_poweroff_event, sizeof(MMIACC_SMART_EVENT_T),
            sizeof(MMIACC_SMART_EVENT_T));
        s_arrived_event_id = EVENT_AUTO_POWEROFF;
        MMK_PostMsg((MMI_WIN_ID_T)VIRTUAL_WIN_ID, MSG_SET_OPENALARM_EXPIRED_WIN, PNULL, 0);
        //OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
        SCI_MEMSET(&s_poweroff_event, 0, sizeof(MMIACC_SMART_EVENT_T));
        return TRUE;        
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : PushDelayedEvent
//         push ID of current dealyed event to stack to save when a call is active
//         currently, this stack only contains one flag 
//  Global resource dependence : 
//  Author:        taul.zhu
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void PushDelayedEvent(
                            void
                            )
{
    s_is_event_delayed = TRUE;
}

/*****************************************************************************/
//  Description : ClearDelayedEvent
//  Global resource dependence : 
//  Author:        liqing.peng
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void ClearDelayedEvent(void)
{
    s_is_event_delayed = FALSE;
}

/*****************************************************************************/
//  Description : get current ring type of  two alarm: 0 and 1
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIALM_GetRingId(
                        uint16   alarm_id
                        )
{
    uint16                  nv_return = 0;
    MMIACC_SMART_EVENT_CONTENT_T event = {0};

    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+alarm_id, &event, nv_return);

    if( MMIAPIUDISK_UdiskIsRun() && (MMISET_RING_FIXED_DEFAUT_NUM < event.ring)) //Set default ring
    {
        event.ring = ALM_DEFAULT_ALARM_RINGTONG;
    }
    else if(event.ring_type == ALM_DOWNLOAD_RING
#ifdef FM_SUPPORT
        ||event.ring_type == ALM_FM_RING
#endif
        ||event.ring_type == ALM_RECORD_RING)
    {
        event.ring = MMISET_MAX_RING_ID;
    }

    return event.ring;
}

#ifdef DPHONE_SUPPORT
LOCAL MMI_RESULT_E  HandleAlarmPowerOnOffEditWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   current_ctrl_id = MMIALM_SETTIME_CTRL_ID;
    MMISET_TIME_T   time = {0};
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:  
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
        GUIEDIT_SetTimeStyle(current_ctrl_id,PNULL,&time_style,PNULL,FALSE);
        GUIEDIT_SetFont(current_ctrl_id, MMI_SET_TIME_FONT);
        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            GUIEDIT_SetTime(current_ctrl_id,
            s_current_event_info.event.event_fast_info.hour,
            s_current_event_info.event.event_fast_info.minute,
            0);
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            GUIEDIT_SetTime(current_ctrl_id, sys_time.hour, sys_time.min, 0); 
        }
        MMK_SetAtvCtrl(win_id,  current_ctrl_id);

        //SCI_ASSERT(s_current_event_info.id <= EVENT_AUTO_POWEROFF);/*assert verified*/
        break;

        case MSG_FULL_PAINT:
        break;
        case MSG_CTL_OK:        // completed
        case MSG_APP_OK:       
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIEDIT_GetTime(current_ctrl_id, &time.time_hour, &time.time_minute, PNULL);
            if ( MMIAPICOM_CheckTimeIsValid(time))
            {
                s_current_event_info.event.event_fast_info.hour = time.time_hour; 
                s_current_event_info.event.event_fast_info.minute = time.time_minute;
                s_current_event_info.event.event_fast_info.is_on = TRUE;
                // 关闭延时闹钟
                SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
                // 提示窗口
                if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MEMORY_FULL);
                }
            }
            else // time invalid
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
            }
            MMK_CloseWin(win_id);
            break;
        case MSG_CTL_CANCEL:
        case MSG_KEYDOWN_CANCEL:
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
//  Description : HandleAutoPowerOnOffWinMsg
//  Global resource dependence : 
//  Author:        lin.lin
//   RETRUN: 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnOffWinMsg( 
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    uint16                      i                = 0;
    MMI_CTRL_ID_T               ctrl_id          = MMIALM_AUTOPOWER_LIST_CTRL_ID;
    MMI_RESULT_E                recode           = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T               text_label[]     = {TXT_AUTOPOWER_ON, TXT_AUTOPOWER_OFF};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem( ctrl_id, ALM_AUTOPOWER_NUM, FALSE );//max item 2
        //GUILIST_SetMaxSelectedItem( ctrl_id, ALM_AUTOPOWER_NUM );
        //GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_EDIT_ICON, 1, FALSE);
#endif
        
        for ( i = 0; i < ALM_AUTOPOWER_NUM; i++ )
        {
            uint16 event_id = (uint16)( EVENT_AUTO_POWERON + i );
            const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );
            
            AppendTwoLineListItem( ctrl_id,
                text_label[i],
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                FALSE );
				//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 			SetListItemStyle(ctrl_id, i);
// #endif
        }
        
        MMK_SetAtvCtrl( win_id, ctrl_id );
        GUIWIN_SetTitleTextId( win_id, TXT_AUTO_POWERONOFF, FALSE );
        break;
    
    case MSG_FULL_PAINT:
#ifdef SCREEN_SHAPE_CIRCULAR
        WATCHCOM_DisplayBackground(win_id);
#endif
        for ( i = 0; i < ALM_AUTOPOWER_NUM; i++ )
        {
            uint16 event_id = (uint16)( EVENT_AUTO_POWERON + i );
            const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );
            
            AppendTwoLineListItem( ctrl_id,
                text_label[i],
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                TRUE );
				//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 			SetListItemStyle(ctrl_id, i);
// #endif
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifndef MMI_GUI_STYLE_TYPICAL  
     case MSG_CTL_LIST_CHECK_CONTENT:
#endif
        {
            uint16 cur_idx = GUILIST_GetCurItemIndex(MMIALM_AUTOPOWER_LIST_CTRL_ID);
			BOOLEAN is_need_full_paint = TRUE;
            switch(cur_idx)
            {
            case TYPE_AUTO_POWERON:
            case TYPE_AUTO_POWEROFF:
                if(TYPE_AUTO_POWERON == cur_idx)
                {
                    s_current_event_info.id = EVENT_AUTO_POWERON;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWERON;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWERON;
                }
                else
                {
                    s_current_event_info.id = EVENT_AUTO_POWEROFF;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWEROFF;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWEROFF;                    
                }
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event);
                if (s_current_event_info.event.event_fast_info.is_on)
                {
                s_current_event_info.event.event_fast_info.is_on = FALSE;
                }
                else
                {	//CR177677:判断另一方是否开启，若开启，看时间是否一致，若一致，给出提示。
					SCI_TRACE_LOW("[MMIALARM.C][HandleAutoPowerOnOffWinMsg] s_current_event_info.id = %d",s_current_event_info.id);
					if (EVENT_AUTO_POWERON == s_current_event_info.id)
					{
						const MMIACC_SMART_EVENT_FAST_TABLE_T   *poweroff_event = {0};
						MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
						poweroff_event = MMIALM_GeEvent(EVENT_AUTO_POWEROFF);
						if (poweroff_event->is_on)
						{//若开启
							if((poweroff_event->minute == s_current_event_info.event.event_fast_info.minute)
								&&(poweroff_event->hour == s_current_event_info.event.event_fast_info.hour))
							{
								MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
								MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
							    is_need_full_paint = FALSE;
							}
						}

					}
					else if(EVENT_AUTO_POWEROFF == s_current_event_info.id)
					{
						const MMIACC_SMART_EVENT_FAST_TABLE_T   *poweron_event = {0};
						MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
						poweron_event = MMIALM_GeEvent(EVENT_AUTO_POWERON);
						if (poweron_event->is_on)
						{//若开启
							if((poweron_event->minute == s_current_event_info.event.event_fast_info.minute)
								&&(poweron_event->hour == s_current_event_info.event.event_fast_info.hour))
							{
								MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
								MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
							    is_need_full_paint = FALSE;
							}
						}
					}

					//FALSE == s_current_event_info.event.event_fast_info.is_on && ！=
					if (is_need_full_paint)
					{
						s_current_event_info.event.event_fast_info.is_on = TRUE;
					}

                }
                if (is_need_full_paint)
                {
                    // 关闭延时闹钟
                    SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
                    // 修改闹钟设置
                    MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);   
                }
                break;

            default:
                break;
            }
        }
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        {
            uint16 cur_idx = GUILIST_GetCurItemIndex(MMIALM_AUTOPOWER_LIST_CTRL_ID);
            switch(cur_idx)
            {
            case TYPE_AUTO_POWERON:
            case TYPE_AUTO_POWEROFF:
                if(TYPE_AUTO_POWERON == cur_idx)
                {
                    s_current_event_info.id = EVENT_AUTO_POWERON;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWERON;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWERON;
                }
                else
                {
                    s_current_event_info.id = EVENT_AUTO_POWEROFF;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWEROFF;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWEROFF;                    
                }
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);  
                MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event);
#ifdef DPHONE_SUPPORT
                MMK_CreateWin((uint32*)DPHONE_ALARM_POWERONOFF_EDIT_WIN, PNULL);
#else
                MMK_CreateWin((uint32*)POWERONOFF_EDIT_WIN_TAB, PNULL);    
#endif
                break;

            default:
                break;
            }
        }
        break;            

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;    

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;

}


/*****************************************************************************/
//  Description : auto power on prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnExpired(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
//#ifndef SCREEN_SHAPE_CIRCULAR
     MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    win_info_ptr->win_id = win_id;
//#endif
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:  
          // 开启窗口定时器
          StartAlarmTimer(&s_3seconds_timer_id, MMI_3SECONDS, FALSE);
          //GUIWIN_SetSoftkeyTextId(MMIALM_ALIVEWIN_ID,  STXT_OK, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE);
//#ifndef SCREEN_SHAPE_CIRCULAR
          recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
//#endif
          MMIDEFAULT_TurnOnBackLight(); 
          break;

        case MSG_APP_OK:
        case MSG_CTL_OK :
            /*SCI_TRACE_LOW("mmialarm.c: HandleAutoPowerOnExpired() MSG_CTL_OK "); 
            MMK_CloseWin(win_id);
            if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
            {
               MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
            }
            MMIAPIPHONE_PowerReset();*/
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
            break;
                            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //POWER_PowerOff()之前关闭背光
            /*MMIDEFAULT_TurnOffBackLight();

             POWER_PowerOff();*/
            break;
        
        case MSG_KEYLONG_RED:
            MMK_CloseWin(win_id);
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
            MMIAPIPHONE_PowerReset();
            break;

       case MSG_TIMER:                  
            if (s_3seconds_timer_id == *((uint8 *) param))
            {
                GUIWIN_UpdateTitle(win_id);
                MMK_CloseWin(win_id);

                if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
                {
                   MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
                }
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
                MMIAPIPHONE_PowerReset();
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
           break;
           
       case MSG_GET_FOCUS:
//            MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
            break;

        case MSG_FULL_PAINT:
            //recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            //initLcdBavkground();
//#ifndef SCREEN_SHAPE_CIRCULAR
            DisplayAlarmWin(win_id,win_info_ptr);
//#endif
            break;

        default:
            if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
                 (MSG_KEYUP_FLIP == msg_id ))//|| MSG_CTL_OK == msg_id))
            {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
                if (MSG_KEYUP_FLIP == msg_id)
                {
                    MMIDEFAULT_HandleFlipKey(TRUE);  
                }
#endif                
            }else
            {
#ifndef SCREEN_SHAPE_CIRCULAR
                recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
#endif
            }
            break;
    }    

    return recode;
}

/*****************************************************************************/
//  Description : auto power off prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOffExpired( 
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
     static MMIACC_SMART_EVENT_T *poweroff=PNULL;

     MMI_STRING_T        tip_str   = {0};
 #ifndef SCREEN_SHAPE_CIRCULAR
    MMIPUB_INFO_T   *win_info_ptr = MMK_GetWinAddDataEx(win_id);
    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    win_info_ptr->win_id = win_id;
#endif
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:
        if (0 == s_30seconds_count)
        {
            s_30seconds_count = 30;
        }
        StartAlarmTimer(&s_1seconds_timer_id, MMI_1SECONDS, TRUE);
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);        
		poweroff=SCI_ALLOC_APP(sizeof(MMIACC_SMART_EVENT_T));
		if(PNULL!=poweroff)
		{
            MMI_MEMCPY(
            (void*)poweroff, sizeof(MMIACC_SMART_EVENT_T),
            (void*)&s_arrived_event, sizeof(MMIACC_SMART_EVENT_T),
            sizeof(MMIACC_SMART_EVENT_T));
        }
        //MMIAPIPB_InterruptPB();
        //MMIAPIPICVIEW_StopDeleteFiles();
        MMIPUB_SetWinSoftkey(MMIALM_ALIVEWIN_ID, TXT_COMMON_OK, STXT_CANCEL, FALSE);
        break;

    case MSG_GET_FOCUS:
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
       // StartAlarmTimer(&s_1seconds_timer_id, MMI_1SECONDS, TRUE);
        break;
        
    case MSG_LOSE_FOCUS:
       /* if(0 != s_1seconds_timer_id)
        {
            MMK_StopTimer(s_1seconds_timer_id);
            s_1seconds_timer_id = 0;
        }*/
        break;

    case MSG_TIMER:
        if (s_30seconds_timer_id == *((uint8 *) param))
        {
            if(MMK_IsFocusWin(win_id))
            {
#ifdef FM_SUPPORT
           	   MMIAPIFM_StopRecord();
#endif            
                GUIWIN_UpdateTitle(win_id);
                MMK_ReturnIdleWin();
                MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
                MMIAPIPHONE_PowerOffEx();
            }
            else
            {
                if(0 != s_1seconds_timer_id)
                {
                    MMK_StopTimer(s_1seconds_timer_id);
                    s_1seconds_timer_id = 0;
                }
                MMI_GetLabelTextByLang(TXT_POWER_OFF, &tip_str);
#ifdef SCREEN_SHAPE_CIRCULAR
                GUILABEL_SetText(WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID, &tip_str, TRUE);
#else
                MMIPUB_UpdateWaitWinText(TRUE, 1, &tip_str,PNULL,PNULL, MMIALM_ALIVEWIN_ID);
#endif
                MMK_WinGrabFocus(win_id);
            }
        }
        else if (s_1seconds_timer_id == *((uint8 *) param))
        {
            uint8               second_num   = 0;
            uint8               num_str[32]  = {0};
            uint8               char_num_len = 0;
            //uint8             ucs2_num[32] = {0};
            uint8               line_num     = 0;
            char                time_str[MMIALM_TIME_STR_12HOURS_LEN+1]  = {0};  //10:34
            wchar               *tmp_str2    = PNULL;
            SCI_TIME_T          past_time    = {0};
            MMI_STRING_T        tmp_str_t1   = {0};
            MMI_STRING_T        tmp_str_t2   = {0};
            MMI_STRING_T        alarm_str[5] = {0};
          
            s_30seconds_count--;
            second_num = s_30seconds_count;
            
            past_time.hour = s_arrived_event.event_fast_info.hour;
            past_time.min = s_arrived_event.event_fast_info.minute;
            
            //响应的都为当前时间，不应该去读取NV的时间
            MMIAPISET_FormatTimeStrByTime(past_time.hour,past_time.min,(uint8*)time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
            alarm_str[0].wstr_len = strlen(time_str);
            alarm_str[0].wstr_ptr = SCI_ALLOC_APP((alarm_str[0].wstr_len+1+20)*sizeof(wchar));
            SCI_MEMSET(alarm_str[0].wstr_ptr,0,((alarm_str[0].wstr_len + 1) * sizeof(wchar)));
            MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

            MMI_GetLabelTextByLang(TXT_ALARM_WILL_TO, &tmp_str_t1); 
            MMI_GetLabelTextByLang(TXT_ALARM_SHUT_DOWN, &tmp_str_t2);
            
            {
                MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
                MMIAPISET_GetLanguageType(&lang_type);

#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT) || (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
                if(
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
                   MMISET_LANGUAGE_SIMP_CHINESE == lang_type
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
                   || MMISET_LANGUAGE_TRAD_CHINESE == lang_type 
#endif
                  )
                {

                    sprintf((char*)num_str, "%02d", (int)second_num);
                }
                else
#endif
                {
                    sprintf((char*)num_str, " %02d", (int)second_num);//非中文需加空格
                }
            }

            char_num_len = strlen((char*)num_str);
            
                
            alarm_str[2].wstr_len = tmp_str_t1.wstr_len + tmp_str_t2.wstr_len + char_num_len;
            tmp_str2 = SCI_ALLOC_APP( ( alarm_str[2].wstr_len + 1 ) * sizeof(wchar) );
            if(PNULL != tmp_str2)
            {
                SCI_MEMSET(tmp_str2, 0, (( alarm_str[2].wstr_len + 1 ) * sizeof(wchar)) );

                MMI_WSTRNCPY( tmp_str2, tmp_str_t1.wstr_len, tmp_str_t1.wstr_ptr, tmp_str_t1.wstr_len, tmp_str_t1.wstr_len );
                MMI_STRNTOWSTR( tmp_str2+tmp_str_t1.wstr_len, char_num_len, (char*)num_str, char_num_len, char_num_len );/*lint !e64*/
                MMI_WSTRNCPY( tmp_str2 + tmp_str_t1.wstr_len + char_num_len, tmp_str_t2.wstr_len,
                    tmp_str_t2.wstr_ptr, tmp_str_t2.wstr_len, tmp_str_t2.wstr_len );

                alarm_str[2].wstr_ptr = tmp_str2;
                line_num = 3;
#ifdef SCREEN_SHAPE_CIRCULAR
                alarm_str[0].wstr_len = alarm_str[0].wstr_len + alarm_str[2].wstr_len;
                MMIAPICOM_Wstrcat(alarm_str[0].wstr_ptr, alarm_str[2].wstr_ptr);
                GUITEXT_SetString(WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID, alarm_str[0].wstr_ptr, alarm_str[0].wstr_len, TRUE);
#else
                MMIPUB_UpdateWaitWinText(TRUE, line_num, &alarm_str[0],PNULL,&alarm_str[2], MMIALM_ALIVEWIN_ID);
#endif
            }
            GUIWIN_UpdateTitle(win_id);
            if (0 == s_30seconds_count)
            {
                if(MMK_IsFocusWin(win_id))
                {
#ifdef FM_SUPPORT
                    MMIAPIFM_StopRecord();
#endif
                    MMIAPIMMS_ExitMMS();
                    MMK_ReturnIdleWin();
                    MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
                    MMIAPIPHONE_PowerOffEx();
                }
                else
                {
                    if(0 != s_1seconds_timer_id)
                    {
                        MMK_StopTimer(s_1seconds_timer_id);
                        s_1seconds_timer_id = 0;
                    }
                    MMI_GetLabelTextByLang(TXT_POWER_OFF, &tip_str);
#ifdef SCREEN_SHAPE_CIRCULAR
                    GUILABEL_SetText(WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID, &tip_str, TRUE);
#else
                    MMIPUB_UpdateWaitWinText(TRUE, 1, &tip_str,PNULL,PNULL, MMIALM_ALIVEWIN_ID);
#endif
                    MMK_WinGrabFocus(win_id);
                }
            }
            
            if(PNULL != tmp_str2)
            {
                SCI_FREE(tmp_str2);
                tmp_str2 = PNULL;
            }
 
            if (PNULL != alarm_str[0].wstr_ptr)
            {
                SCI_FREE(alarm_str[0].wstr_ptr);
                alarm_str[0].wstr_ptr= PNULL;
            }
        }
        else
        {
             recode = MMI_RESULT_FALSE;
        }
        break;


    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
        s_30seconds_count=0;
        //SCI_TRACE_LOW:"mmialarm.c: HandleAutoPowerOffExpired() MSG_CTL_OK "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_5540_112_2_18_2_1_1_38,(uint8*)"");
        MMK_ReturnIdleWin();
        MMIAPIPHONE_PowerOffEx();
        break;

	case MSG_KEYDOWN_RED:        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYUP_POWER:
        s_30seconds_count=0;
        if(s_bypast_list.total > 0)
        {
            OpenPastSchEventWin(WIN_ONE_LEVEL, s_bypast_list.entry_id[0]);
            DeleteOneSchEventInList(0, &s_bypast_list);
        }
        else
        {
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//zhaohui,counted time
        }
        MMK_CloseWin(win_id);
        if(MSG_KEYUP_POWER==msg_id)
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
                  
    case MSG_FULL_PAINT:
       // recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
#ifndef SCREEN_SHAPE_CIRCULAR
      DisplayAlarmWin(win_id,win_info_ptr);
#endif
        break;            

    case MSG_CLOSE_WINDOW:
        if(0 != s_1seconds_timer_id)
        {
            MMK_StopTimer(s_1seconds_timer_id);
            s_1seconds_timer_id = 0;
        }
        if(0 != s_30seconds_count)//中途被其他事件打断
        {
            if(PNULL!=poweroff)
            {
                MMI_MEMCPY(
                (void*)&s_poweroff_event, sizeof(MMIACC_SMART_EVENT_T),
                (void*)poweroff, sizeof(MMIACC_SMART_EVENT_T),
                sizeof(MMIACC_SMART_EVENT_T));
            }
            else
            {
                s_30seconds_count=0;
            }
        }
        if(PNULL!=poweroff)
        {
            SCI_FREE(poweroff);
			poweroff=PNULL;
		}
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);  
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    default:
        if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
             (MSG_KEYUP_FLIP == msg_id ))//|| MSG_CTL_OK == msg_id))
        {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
            if (MSG_KEYUP_FLIP == msg_id)
            {
                MMIDEFAULT_HandleFlipKey(TRUE);  
            }
#endif            
        }else
        {
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        }
        break;
    }    

    return recode;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : select alarm ring
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmRingWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIALM_RINGLIST_CTRL_ID;
    uint32                  item_id = 0;
    MULTIM_SELECT_RETURN_T  *win_param = NULL;
    MMIFMM_SELECT_INFO_T    select_info = {{MMIFMM_MUSIC_ALL,0},0,0,PNULL,0,-1};
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    GUIMENU_BUTTON_INFO_T    button_info={0};
    GUIMENU_TITLE_INFO_T      title_info={0};
    uint32 group_id=MENU_ALARM_SETTING_RING;
    MMI_STRING_T title={0};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		//if alarm ring is download ring, check whether file exists
        if((ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type || ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type) 
            &&!MMIAPIFMM_IsFileExist(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type &&
           !MMIAPIALM_IsValidFM(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#endif
        button_info.is_static=TRUE;
        button_info.group_id=group_id;
        button_info.button_style=GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id,&button_info);

        title_info.group_id=group_id;
        title_info.is_static=TRUE;
        MMI_GetLabelTextByLang(TXT_RING,&title);
        title_info.title_ptr=&title;
        GUIMENU_SetMenuTitleEx(ctrl_id,&title_info);
            
        if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type)
        {
            pop_item_info.menu_id=ID_ALARM_RING_DOWNLOAD;
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type)
        {

            pop_item_info.menu_id=ID_ALARM_RING_FM;
        }
#endif
        else if(ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type)
        {

            pop_item_info.menu_id=ID_ALARM_RING_RECORD;
        }
        else
        {
             pop_item_info.menu_id=ID_ALARM_RING_FIX;
        }
        //set pop-up menu status
        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id   = ctrl_id;
        pop_item_info.group_id  = group_id;
        GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &item_id);
        switch(item_id)
        {
        case ID_ALARM_RING_FIX:
            MMK_CreatePubListWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL);  
            break;

        case ID_ALARM_RING_DOWNLOAD:
            select_info.ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);

#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif

            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }
            if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type)
            {
                select_info.ticked_file_wstr = s_current_event_info.event.event_ring_info.dring_name_arr;
                select_info.ticked_file_len= s_current_event_info.event.event_ring_info.dring_name_len;
                select_info.win_id = win_id;  
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
            }
           if (!MMIAPIFMM_OpenSelectMusicWin(&select_info))
            {
                pop_item_info.menu_id=ID_ALARM_RING_FIX;
                pop_item_info.is_static = TRUE;
                pop_item_info.ctrl_id   = ctrl_id;
                pop_item_info.group_id  = group_id;
                GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
                limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
        case ID_ALARM_RING_FM:
            {
                if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                {
                    pop_item_info.menu_id=ID_ALARM_RING_FIX;
                    pop_item_info.is_static = TRUE;
                    pop_item_info.ctrl_id   = ctrl_id;
                    pop_item_info.group_id  = group_id;
                    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
                else
                {
                    MMK_CreateWin((uint32*)ALARM_FM_EDITWIN_TAB, PNULL); 
                }
            }
            break;
#ifdef MMI_RECORD_SUPPORT
         case ID_ALARM_RING_RECORD:
            {
                RecordAlarmRingCallBack();
            }
            break;
#endif
        default:
            break;
        }
        break;

    case MSG_MULTIM_SELECTED_RETURN:
        win_param = (MULTIM_SELECT_RETURN_T *)param;

        s_current_event_info.event.event_content_info.ring_type = ALM_DOWNLOAD_RING;
        s_current_event_info.event.event_ring_info.dring_name_len = win_param->file_name_len;

        SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
        MMI_WSTRNCPY(s_current_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                   win_param->file_name_ptr, win_param->file_name_len, win_param->file_name_len);

        s_current_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
//        CloseDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
//        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);
        //lizhen:此处不需要弹出"已完成"
//        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(win_id);//@zhaohui,108296
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;     
}
#else
/*****************************************************************************/
//  Description : select alarm ring
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmRingWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIALM_RINGLIST_CTRL_ID;
    uint16                  item_id = 0;
    MULTIM_SELECT_RETURN_T  *win_param = NULL;
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, ALM_RING_TYPE_MAX, FALSE);
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
        GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
#endif
        //按照
        MMIAPISET_AppendListItemByTextId(TXT_RING_FIXED,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId(TXT_COMMON_MORE_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#ifdef FM_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_ALARM_FM_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
//@leon.wang modify for QQVGA(128x160)
#ifdef MMI_RECORD_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_ALARM_RECORD_RING_TYPE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
        //if alarm ring is download ring, check whether file exists
        if((ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type || ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type)
            &&!MMIAPIFMM_IsFileExist(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type &&
           !MMIAPIALM_IsValidFM(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#endif

        item_id = (uint16)s_current_event_info.event.event_content_info.ring_type;
        if(ALM_RING_TYPE_MAX<=item_id)
        {
            item_id = 0;
        }
        GUILIST_SetCurItemIndex(ctrl_id, item_id);
        GUILIST_SetSelectedItem(ctrl_id, item_id, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        item_id = GUILIST_GetCurItemIndex(ctrl_id);
        switch( item_id)
        {
        case ALM_FIX_RING:
            MMK_CreateWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL); 
            break;

        case ALM_DOWNLOAD_RING:
            select_info.ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);

#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif

            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }
            if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type)
            {
                select_info.ticked_file_wstr = s_current_event_info.event.event_ring_info.dring_name_arr;
                select_info.ticked_file_len= s_current_event_info.event.event_ring_info.dring_name_len;
                select_info.win_id = win_id;  
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
            }
            if (!MMIAPIFMM_OpenSelectMusicWin(&select_info))
            {
                GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
                GUILIST_SetSelectedItem(ctrl_id, 1, FALSE);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
                limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
#ifdef FM_SUPPORT
        case ALM_FM_RING:
            {
				if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
				else
				{
                    MMK_CreateWin((uint32*)ALARM_FM_EDITWIN_TAB, (ADD_DATA)&(s_current_event_info.event));
				}
            }
            break; 
#endif

#ifdef MMI_RECORD_SUPPORT
        case ALM_RECORD_RING:
            {
                RecordAlarmRingCallBack(); 
            }
            break;
#endif
        default:
            break;
        }
        break;

    case MSG_MULTIM_SELECTED_RETURN:
        win_param = (MULTIM_SELECT_RETURN_T *)param;

        s_current_event_info.event.event_content_info.ring_type = ALM_DOWNLOAD_RING;
        s_current_event_info.event.event_ring_info.dring_name_len = win_param->file_name_len;

        SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
        MMI_WSTRNCPY(s_current_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                   win_param->file_name_ptr, win_param->file_name_len, win_param->file_name_len);

        s_current_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
//        CloseDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
//        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);
        //lizhen:此处不需要弹出"已完成"
//        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(win_id);//@zhaohui,108296
        break;

    case MSG_GET_FOCUS:
        item_id = (uint16)s_current_event_info.event.event_content_info.ring_type;
        if(ALM_RING_TYPE_MAX<=item_id)
        {
            item_id = 0;
        }
        GUILIST_SetCurItemIndex(ctrl_id, item_id);
        GUILIST_SetSelectedItem(ctrl_id, item_id, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;     
}

#endif
/*****************************************************************************/
//  Description : select fixed ring
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmFixedRingWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIALM_FIXEDRINGLIST_CTRL_ID;
//    uint16          i = 0;
    uint16          cur_select = 0;
    uint16          ring_id = 0;
    uint8           ring_vol=0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISRVAUD_ReqVirtualHandle("FIX ALARM SELECT RING", MMISRVAUD_PRI_NORMAL);
#ifdef DPHONE_SUPPORT
        //if alarm ring is download ring, check whether file exists
        if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type &&
           !MMIAPIFMM_IsFileExist(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type &&
        !MMIAPIALM_IsValidFM(s_current_event_info.event.event_ring_info.dring_name_arr,
                        s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#endif
#endif
        GUILIST_SetMaxItem(ctrl_id,MMISET_RING_FIXED_DEFAUT_NUM, FALSE );//max item 8
        MMIAPISET_AppendFixedItems(
                                    TXT_RING,
                                    MMISET_RING_FIXED_DEFAUT_NUM,
                                    TXT_COMMON_OK,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_RADIO
                                    );
        //for(i=0; i<MMISET_RING_FIXED_DEFAUT_NUM; i++)
        //{
            //GUILIST_ChangeIconId(ctrl_id,i,IMAGE_SELECT_TICK_OFF_PIC,IMAGE_NULL);
        //}
        
        if(ALM_FIX_RING == s_current_event_info.event.event_content_info.ring_type)
        {
            ring_id = s_current_event_info.event.event_content_info.ring;
            cur_select = ring_id-1;
            //GUILIST_ChangeIconId(ctrl_id,cur_select,IMAGE_SELECT_TICK_ON_PIC,IMAGE_NULL);
        }
        else
        {
            ring_id = 1;
            cur_select = 0;
        }

        GUILIST_SetCurItemIndex(ctrl_id, cur_select);
        GUILIST_SetSelectedItem(ctrl_id, cur_select, TRUE);
        ////GUILIST_UpdateListBox(ctrl_id);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMIALM_FIXEDRINGLIST_CTRL_ID:
                MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
                cur_select = GUILIST_GetCurItemIndex(ctrl_id);

                ring_id = cur_select+1;
                ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
                if (0 == ring_vol)
                {
                    ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
                }
                MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break; 
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break; 
            default:
                break;
            }

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        cur_select = GUILIST_GetCurItemIndex(ctrl_id);

        ring_id = cur_select+1;
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;
#endif

    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:  
        if ( MMK_IsSingleClick(msg_id, param))
        {
            break;
        }
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);

        cur_select = GUILIST_GetCurItemIndex(ctrl_id);
        ring_id = cur_select +1;
        s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
        s_current_event_info.event.event_content_info.ring = ring_id;
        //MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);

//@zhaohui,cr108296,lizhen:此处不需要弹出"已完成"
//      MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

#ifdef DPHONE_SUPPORT
#else
        MMK_CloseWin( MMIALM_RINGLIST_WIN_ID ); 
#endif        
        MMK_CloseWin( win_id );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMISRVAUD_FreeVirtualHandle("FIX ALARM SELECT RING"); 
        break;        

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode; 
}

//#else


/*****************************************************************************/
//  Description : Open Alarm window 
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL void OpenAlarmAliveWin(
                             MMI_WIN_PRIORITY_E win_priority
                             )
{
    uint8               line_num     = 0;
    uint8               second_num   = 30;
    uint8               char_num_len = 0;
    uint8               num_str[32]  = {0};
    //uint8             ucs2_num[32] = {0};
    //uint8               *tmp_str  = PNULL;
    wchar               *tmp_str2    = PNULL;
    uint8               date_str[MMISET_DATE_STR_LEN] = {0}; //2005.06.30
    char                time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]  = {0};  //10:34
    char                date_unicode_str[11*2+2] = {0}; //2005.06.30
    char                time_unicode_str[MMIALM_TIME_STR_12HOURS_LEN*2 + 2 + 20]  = {0};  //10:34
//    uint16              event_id     = 0;
    SCI_DATE_T          sys_date     = {0};
    SCI_TIME_T          sys_time     = {0};
    MMI_STRING_T        alarm_str[5] = {0};
    MMI_STRING_T        tmp_str_t1   = {0};
    MMI_STRING_T        tmp_str_t2   = {0};
    MMI_IMAGE_ID_T      icon         = IMAGE_NULL;
    MMIPUB_HANDLE_FUNC  func         = NULL;
    MMI_TEXT_ID_T           txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10,};
#ifdef SCREEN_SHAPE_CIRCULAR
	WATCH_SOFTKEY_TEXT_ID_T softkey_text = {0};
#endif
   /* MMI_TEXT_ID_T           txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                                TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                                TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};*///CR196314
    //get current system date
    TM_GetSysDate(&sys_date);

    //get current system time
    TM_GetSysTime(&sys_time);
#ifdef SCREENSAVER_SUPPORT
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {        
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif
    if(MMK_IsOpenWin(PHONE_STARTUP_ALARM_WIN_ID))
    {
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
    }
    
    sys_date.mday=s_arrived_event.event_fast_info.day;
    sys_date.mon=s_arrived_event.event_fast_info.mon;
    sys_date.year=s_arrived_event.event_fast_info.year;
    sys_time.hour=s_arrived_event.event_fast_info.hour;
    sys_time.min=s_arrived_event.event_fast_info.minute;
    
    MMIAPISET_FormatDateStrByDateStyle(sys_date.year, sys_date.mon, sys_date.mday,'.',date_str,MMISET_DATE_STR_LEN);

    MMIAPISET_FormatTimeStrByTime(sys_time.hour,sys_time.min,(uint8*)time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
    
    if (SMART_EVENT_ALARM ==s_arrived_event.event_fast_info.type) // (0  ==s_arrived_event.length)    // assigne default text                
    {   
        alarm_str[0].wstr_len = strlen(time_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*) time_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/
            
        if (0 == s_arrived_event.event_content_info.w_length)
        {
            if (s_arrived_event_id <= EVENT_ALARM_MAX)//&&s_arrived_event_id>=EVENT_ALARM_0 /*lint !e685 !e568*/
            {
                MMI_GetLabelTextByLang(txt_alarm_id[s_arrived_event_id],&alarm_str[1]);
           }
            else if(s_arrived_event_id>=EVENT_ALARM_DELAY_0&&s_arrived_event_id <= EVENT_ALARM_DELAY_MAX)
            {
                MMI_GetLabelTextByLang(txt_alarm_id[s_arrived_event_id-EVENT_ALARM_DELAY_0],&alarm_str[1]);//CR196314
            }
            
        }
        else
        {       
            alarm_str[1].wstr_len = s_arrived_event.event_content_info.w_length;                 
            alarm_str[1].wstr_ptr = s_arrived_event.event_content_info.w_content;  
        }
        line_num = 2;
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT            
        MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_ALARM);
#endif
    }
    else if ( SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*)time_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_AUTOPOWER_ON, &alarm_str[1]); 
        line_num = 2;
    }
    else if ( SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*)time_unicode_str;
         SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_ALARM_WILL_TO, &tmp_str_t1); 
        MMI_GetLabelTextByLang(TXT_ALARM_SHUT_DOWN, &tmp_str_t2);
        
        {
            MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
            MMIAPISET_GetLanguageType(&lang_type);
            if (0 != s_30seconds_count)
            {
                second_num = s_30seconds_count;
            }
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT) || (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
            if(
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
                MMISET_LANGUAGE_SIMP_CHINESE == lang_type
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
             || MMISET_LANGUAGE_TRAD_CHINESE == lang_type 
#endif
             )
            {

                sprintf((char*)num_str, "%02d", (int)second_num);
            }
            else
#endif
            {
                sprintf((char*)num_str, " %02d", (int)second_num);//非中文需加空格
            }
        }
        
        char_num_len = strlen((char*)num_str);

        alarm_str[2].wstr_len = tmp_str_t1.wstr_len + tmp_str_t2.wstr_len + char_num_len;
        tmp_str2 = SCI_ALLOC_APP( ( alarm_str[2].wstr_len + 1 ) * sizeof(wchar) );
        if(PNULL != tmp_str2)
        {
            SCI_MEMSET( tmp_str2, 0, (( alarm_str[2].wstr_len + 1 ) * sizeof(wchar)) );

            MMI_WSTRNCPY( tmp_str2, tmp_str_t1.wstr_len, tmp_str_t1.wstr_ptr, tmp_str_t1.wstr_len, tmp_str_t1.wstr_len );
            MMI_STRNTOWSTR( tmp_str2+tmp_str_t1.wstr_len, char_num_len, (char*)num_str, char_num_len, char_num_len );/*lint !e64*/
            MMI_WSTRNCPY(tmp_str2 + tmp_str_t1.wstr_len + char_num_len, tmp_str_t2.wstr_len, 
                tmp_str_t2.wstr_ptr, tmp_str_t2.wstr_len, tmp_str_t2.wstr_len );
            alarm_str[2].wstr_ptr = tmp_str2;
        }

        line_num = 3;
    } 
#ifdef MMI_AZAN_SUPPORT
    else if (SMART_EVENT_AZAN == s_arrived_event.event_fast_info.type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
        alarm_str[0].wstr_ptr = (wchar*)time_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr, 0, ((alarm_str[0].wstr_len + 1) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/
        MMI_GetLabelTextByLang(STR_AZAN_ALARM, &alarm_str[1]); 
        line_num = 2;
    }
#endif /*MMI_AZAN_SUPPORT*/ 
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    else if(SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type)
    {
        //do nothing;
    }
#endif
    else
    {       
        //日程
        alarm_str[0].wstr_len = strlen((char*)date_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*)date_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,date_str,strlen(date_str),strlen(date_str));/*lint !e64*/

        alarm_str[1].wstr_len = strlen(time_str);
        //tmp_str2 = SCI_ALLOC_APP(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar) );
        //MMI_STRNTOWSTR(tmp_str2,alarm_str[1].wstr_len,time_str,strlen(time_str),strlen(time_str));
        alarm_str[1].wstr_ptr = (wchar*)time_unicode_str;
        SCI_MEMSET(alarm_str[1].wstr_ptr,0,(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[1].wstr_ptr,alarm_str[1].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/
        alarm_str[2].wstr_len = s_arrived_event.event_content_info.w_length;                 
        alarm_str[2].wstr_ptr = s_arrived_event.event_content_info.w_content;   
        line_num = 3;
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT            
        MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_SCHEDULE);
#endif
    } 
    
    if (SMART_EVENT_ALARM ==s_arrived_event.event_fast_info.type ||
        SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type ||
        SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type
#ifdef MMI_AZAN_SUPPORT
        ||(SMART_EVENT_AZAN == s_arrived_event.event_fast_info.type)
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
        ||(SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type)
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
        || (SMART_EVENT_SOGOU_UPDATE == s_arrived_event.event_fast_info.type)
#endif
    ) 
    {
        icon = IMAGE_CLOCK_ALARM_ALERT;
    }
    else if (SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
    {
        icon = IMAGE_CLOCK_CALENDAR_ALERT;
    }
    else
    {
        //SCI_PASSERT(0, ("OpenAlarmAliveWin: Invalid s_arrived_event.type is %d",s_arrived_event.event_fast_info.type)); /*assert verified*/ 
        //SCI_TRACE_LOW:"OpenAlarmAliveWin error: Invalid s_arrived_event.type  is %d:"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6419_112_2_18_2_1_3_39,(uint8*)"d",s_arrived_event.event_fast_info.type);
        if (PNULL != tmp_str2)
        {
            SCI_FREE(tmp_str2);
            tmp_str2 = PNULL;
        }
        return;        
    }
    MMIDEFAULT_TurnOnBackLight();//修改详情参考CR122782的comments
    if (SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type)
    {
        //initLcdBavkground();
        func = (MMIPUB_HANDLE_FUNC )HandleAutoPowerOnExpired;
#if 0//def SCREEN_SHAPE_CIRCULAR
        alarm_str[0].wstr_len = alarm_str[0].wstr_len + alarm_str[1].wstr_len;
        MMIAPICOM_Wstrcat(alarm_str[0].wstr_ptr, alarm_str[1].wstr_ptr);
        WatchCOM_QueryWin_2Btn_Enter(MMIALM_ALIVEWIN_ID,
                                    &alarm_str[0],
                                    PNULL,
                                    PNULL,
                                    PNULL,
                                    PNULL,
                                    softkey_text,
                                    func);
#else
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_NONE,func);
#endif
    }
    else if (SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type)
    {
        func = (MMIPUB_HANDLE_FUNC )HandleAutoPowerOffExpired;
#ifdef SCREEN_SHAPE_CIRCULAR
        alarm_str[0].wstr_len = alarm_str[0].wstr_len + alarm_str[2].wstr_len;
        MMIAPICOM_Wstrcat(alarm_str[0].wstr_ptr, alarm_str[2].wstr_ptr);
        WatchCOM_QueryWin_2Btn_Enter(MMIALM_ALIVEWIN_ID,
                                    &alarm_str[0],
                                    PNULL,
                                    PNULL,
                                    res_common_ic_confirm,
                                    res_common_ic_cancel,
                                    softkey_text,
                                    func);
#else
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],PNULL,&alarm_str[2],
        MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_OKCANCEL,func);
#endif
        /*if( SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type)
        {
            GUIWIN_CreateTitleDyna(MMIALM_ALIVEWIN_ID, TXT_AUTOPOWER_ON);
        }
        else if( SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type)
        {
            GUIWIN_CreateTitleDyna(MMIALM_ALIVEWIN_ID, TXT_AUTOPOWER_OFF);
        }*/
    }
    else if (SMART_EVENT_ALARM == s_arrived_event.event_fast_info.type)
    {
#if defined MMI_ALARM_JUMP_CLOCK_SUPPORT && defined MMI_PDA_SUPPORT
        if(IsNeedBackgroundPrompt()
            #if defined JAVA_SUPPORT
            || MMIAPIJAVA_IsJavaRuning()
            #endif
            #if defined BROWSER_SUPPORT
            || MMIAPIBROWSER_IsRunning()
            #endif
            #if defined WRE_SUPPORT
            || MMIWRE_Actived()
            #endif
            )/*lint !e774*/
        {
            func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
            MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2], 
              MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_CUSTOMER, func); 
        }
        else
        {
            MMK_CreateWin((uint32*)MMIALM_JUMP_CLOCK_TAB, PNULL);
        }
#else   
#ifdef MMI_GUI_STYLE_MINISCREEN     
        if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
        {	
			MMIAPIPHONE_SetStartUpCondition(STARTUP_ALARM);
        }
#endif
        func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
#ifdef MMI_ETWS_SUPPORT
        if(TRUE == MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
        {
            // Copy alarm str1, alarm str2, icon, line num, priority
            if( PNULL != s_alarm_str1.wstr_ptr )
            {
                SCI_FREE(s_alarm_str1.wstr_ptr);
            }
            s_alarm_str1.wstr_ptr = SCI_ALLOCAZ(alarm_str[1].wstr_len * sizeof(wchar));
            if (PNULL == s_alarm_str1.wstr_ptr)
            {
                SCI_TRACE_LOW("MMIALARM:alloc s_alarm_str1.wstr_ptr failed");
                return;
            }
            SCI_MEMSET(s_alarm_str1.wstr_ptr , 0, alarm_str[1].wstr_len* sizeof(wchar));

            MMIAPICOM_Wstrncpy(s_alarm_str1.wstr_ptr, alarm_str[1].wstr_ptr, alarm_str[1].wstr_len );
            s_alarm_str1.wstr_len = alarm_str[1].wstr_len;

            if( PNULL != s_alarm_str2.wstr_ptr )
            {
                SCI_FREE(s_alarm_str2.wstr_ptr);
            }
            s_alarm_str2.wstr_ptr = SCI_ALLOCAZ(alarm_str[0].wstr_len * sizeof(wchar));
            if (PNULL == s_alarm_str2.wstr_ptr)
            {
                SCI_TRACE_LOW("MMIALARM:alloc s_alarm_str2.wstr_ptr failed");
                return;
            }
            SCI_MEMSET(s_alarm_str2.wstr_ptr , 0, alarm_str[0].wstr_len* sizeof(wchar));

            MMIAPICOM_Wstrncpy(s_alarm_str2.wstr_ptr, alarm_str[0].wstr_ptr, alarm_str[0].wstr_len );
            s_alarm_str2.wstr_len = alarm_str[0].wstr_len;

            s_line_num = line_num;
            s_icon = icon;
            s_win_priority = win_priority;

            s_is_need_ring_and_vib = TRUE;

            MMIPUB_OpenWaitWin( line_num,PNULL,PNULL,PNULL,
             MMIALM_ALIVEWIN_DURING_ETWS_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_DIALOG_DISMISS, func);
        }
        else
#endif
       {
#ifdef SCREEN_SHAPE_CIRCULAR
#ifdef ADULT_WATCH_SUPPORT
            AdultWatchCOM_QueryWin_2Btn_Enter(MMIALM_ALIVEWIN_ID,
                                                &alarm_str[0],
                                                PNULL,
                                                PNULL,
                                                res_aw_alarm_ic_snooze,
                                                res_aw_alarm_ic_close,
                                                HandleEventAliveWinMsg);
#else
          {
              WATCH_SOFTKEY_TEXT_ID_T softkey = {0};
              WatchCOM_QueryWin_2Btn_Enter(MMIALM_ALIVEWIN_ID,
                                           &alarm_str[0],
                                           PNULL,
                                           PNULL,
                                           res_aw_alarm_ic_snooze,
                                           res_aw_alarm_ic_close,
                                           softkey,
                                           HandleEventAliveWinMsg);
              WATCHCOM_CloseAudioOrVieo();
         }
#endif

#else
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
          MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_CUSTOMER, func);
#endif
       }
#endif
    }
#ifdef MMI_AZAN_SUPPORT
    else if (SMART_EVENT_AZAN == s_arrived_event.event_fast_info.type)
    {
        MMIAPIAZAN_OpenAlarmExpiredWin(FALSE);
    }
#endif /*MMI_AZAN_SUPPORT*/
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    else if (SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type)
    {
        MMIAPIFM_StartFMTimerRecord();
    }
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    else if (SMART_EVENT_SOGOU_UPDATE == s_arrived_event.event_fast_info.type)
    {
        //SCI_TRACE_LOW:"SOGOU Update event coming"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6491_112_2_18_2_1_3_40,(uint8*)"");
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
        {
            MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
        }
#ifdef MMI_WIFI_SUPPORT
        else if (MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#else
        else if (MMIAPIPHONE_GetSimOkNum(PNULL, 0) == 0 || MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#endif
        {
            // 不具备更新网络条件，不弹出更新提示
        }
        else
        {
            MMIAPIIM_SogouAutoUpdateWordRemind();
        }
    }
#endif
    else
    {
#if defined(SCH_ASSISTANT_WIDGET) && defined(MMI_GRID_IDLE_SUPPORT)
        if(MMIAPIWIDGET_IsScheduleOnTop())
        {
            MMK_SendMsg(WIDGET_SCHEDULE_WIN_ID, MSG_SCH_EVENT_ARRIVE, &s_arrived_event);
        }
        else
        {
            //if(WIN_LOWEST_LEVEL == win_priority)//低优先级，没有提示
            //{
                func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
                MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
                MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_ONE, func);   
            //}
            //else
            //{
            //    MMK_CreatePubFormWin((uint32 *)MMISCH_POP_ALERT_WIN_TAB, PNULL);
            //}
        }
#else
         func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
#ifdef MMI_ETWS_SUPPORT
        if(TRUE == MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
        {
            // Copy alarm str1, alarm str2, icon, line num, priority
            if( PNULL != s_alarm_str1.wstr_ptr )
            {
                SCI_FREE(s_alarm_str1.wstr_ptr);
            }
            s_alarm_str1.wstr_ptr = SCI_ALLOCAZ(alarm_str[1].wstr_len * sizeof(wchar));
            if (PNULL == s_alarm_str1.wstr_ptr)
            {
                SCI_TRACE_LOW("MMIALARM:alloc s_alarm_str1.wstr_ptr failed");
                return;
            }
            SCI_MEMSET(s_alarm_str1.wstr_ptr , 0, alarm_str[1].wstr_len* sizeof(wchar));

            MMIAPICOM_Wstrncpy(s_alarm_str1.wstr_ptr, alarm_str[1].wstr_ptr, alarm_str[1].wstr_len );
            s_alarm_str1.wstr_len = alarm_str[1].wstr_len;

            if( PNULL != s_alarm_str2.wstr_ptr )
            {
                SCI_FREE(s_alarm_str2.wstr_ptr);
            }
            s_alarm_str2.wstr_ptr = SCI_ALLOCAZ(alarm_str[0].wstr_len * sizeof(wchar));
            if (PNULL == s_alarm_str2.wstr_ptr)
            {
                SCI_TRACE_LOW("MMIALARM:alloc s_alarm_str2.wstr_ptr failed");
                return;
            }
            SCI_MEMSET(s_alarm_str2.wstr_ptr , 0, alarm_str[0].wstr_len* sizeof(wchar));

            MMIAPICOM_Wstrncpy(s_alarm_str2.wstr_ptr, alarm_str[0].wstr_ptr, alarm_str[0].wstr_len );
            s_alarm_str2.wstr_len = alarm_str[0].wstr_len;

            s_line_num = line_num;
            s_icon = icon;
            s_win_priority = win_priority;

            s_is_need_ring_and_vib = TRUE;

            MMIPUB_OpenWaitWin( line_num,PNULL,PNULL,PNULL,
             MMIALM_ALIVEWIN_DURING_ETWS_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_DIALOG_DISMISS, func);
        }
        else
#endif
        {
            MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_ONE, func);
        }
#endif

    }
    if (PNULL != tmp_str2)
    {
        SCI_FREE(tmp_str2);
        tmp_str2 = PNULL;
    }
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
// Description : open alarm dialog
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenAlarmDialogWin(void)
{
    MMIPUB_HANDLE_FUNC  func         = NULL;

    func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
    s_is_need_ring_and_vib = FALSE;

    MMIPUB_OpenAlarmDialogWin(s_line_num, &s_alarm_str1, &s_alarm_str2,PNULL,
        MMIALM_ALIVEWIN_ID, s_icon,PNULL,s_win_priority,MMIPUB_SOFTKEY_DIALOG_ALARM, func);

}
#endif

/*****************************************************************************/
//  Description : handle past event win msg
//  Global resource dependence : 
//  Author:       xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastEventWinMsg(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E                            recode = MMI_RESULT_TRUE;    
    MMIPUB_INFO_T*                          win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    static uint8                            s_timer_id = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T*  past_event = PNULL;

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    win_info_ptr->win_id = win_id;

    //SCI_TRACE_LOW:"HandlePastEventWinMsg, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6565_112_2_18_2_1_3_41,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW: 

        past_event  = MMIALM_GeEvent(s_past_event_id);
        s_arrived_event_id = s_past_event_id;
        MMIAPIALM_FormInteEventInfo(s_arrived_event_id,&s_arrived_event);
        MMIAPIALM_GetRingInfo(s_arrived_event_id, &s_arrived_event);

        if (SMART_EVENT_ALARM == past_event->type)
        {
            MMIPUB_SetWinTitleTextId(win_id,TXT_ALARM_CLOCK,FALSE);
        }
        else if(SMART_EVENT_SCH == past_event->type)
        {
            MMIPUB_SetWinTitleTextId(win_id,TXT_IDLE_CALENDAR,FALSE);
        }
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
        GUIWIN_SetSoftkeyTextId(MMIALM_BYPAST_SCH_WIN_ID,  TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE); 
        GUIWIN_SeSoftkeytButtonIconId(MMIALM_BYPAST_SCH_WIN_ID, IMAGE_NULL, 1, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(MMIALM_BYPAST_SCH_WIN_ID,  TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE); 
#endif

        StartAlarmTimer(&s_past_event_timer_id, ALERT_RING_DURATION, FALSE);
        StartAlarmTimer(&s_timer_id, 100, FALSE);
        StartRingOrVib();//CR162626
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);

        break;

    case MSG_FULL_PAINT:
       // recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        DisplayAlarmWin(win_id,win_info_ptr);
        break;
        
    case MSG_GET_FOCUS: 
        recode = MMIPUB_HandleWaitWinMsg(win_id, MSG_FULL_PAINT, PNULL);
        if (0 == s_past_event_timer_id )
        {
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
            }
#endif
        }
        MMIDEFAULT_TurnOnBackLight();//CR162626
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        break;
        
    case MSG_TIMER:
        if (s_past_event_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            
            MMK_StopTimer(s_past_event_timer_id);
            s_past_event_timer_id = 0;
            
            //MMK_CloseWin(win_id);
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_AddUnreadEvent(s_past_event_id);
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                if(MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
                }
            }
#endif
            MMK_CloseWin(win_id);
            CheckandStartExpiredEvent();
        }
        else if(s_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            MMK_StopTimer(s_timer_id);
            s_timer_id = 0;
            if(!MMK_IsFocusWin(win_id))
            {
                MMK_WinGrabFocus(win_id);
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }
        }
        break;
        
    case MSG_CLOSE_WINDOW:
        if (0 != s_past_event_timer_id )
        {
            MMK_StopTimer(s_past_event_timer_id);
            s_past_event_timer_id = 0;
        }
        StopRingOrVib(FALSE);//CR162626
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
        
    case MSG_KEYDOWN_RED:
    case MSG_KEYLONG_RED:
    //case MSG_KEYDOWN_FLIP:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);// 先关闭，再打开
#ifdef CALENDAR_SUPPORT
        MMIAPISCH_DeleteUnreadEvent(s_past_event_id);
#endif
		
        CheckandStartExpiredEvent();
        break;
        
    default:
        if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
            (MSG_KEYUP_FLIP == msg_id || MSG_CTL_OK == msg_id))
        {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
            if (MSG_KEYUP_FLIP == msg_id)
            {
                MMIDEFAULT_HandleFlipKey(TRUE);  
            }
#endif            
        }
        else
        {
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        }
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : Open bypast sch event win(打开过期事件窗口)
//  Global resource dependence : 
//  Author: xiaoqing.lu 
//  Note:   
/*****************************************************************************/
LOCAL void OpenPastSchEventWin(
                               MMI_WIN_PRIORITY_E win_priority,
                               uint16 event_id
                               )
{
    SCI_DATE_T          past_date = {0};
    SCI_TIME_T          past_time = {0};
    MMI_STRING_T        alarm_str[5] = {0};
    MMI_IMAGE_ID_T      icon = IMAGE_NULL;
    uint8                date_str[MMISET_DATE_STR_LEN] = {0}; //2005.06.30
    char                time_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};  //10:34
    wchar                date_unicode_str[11+1] = {0}; //2005.06.30
    wchar                time_unicode_str[MMIALM_TIME_STR_12HOURS_LEN+1]  = {0};  //"%02d:%02d PM"   
    MMIPUB_HANDLE_FUNC  func = NULL;
    uint8               line_num = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T   *past_event = {0};
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    uint16                          nv_return = 0;

    s_past_event_id=event_id;

    past_event = MMIALM_GeEvent(event_id);

    /*past_date.year = s_arrived_event.event_fast_info.year;
    past_date.mon = s_arrived_event.event_fast_info.mon;
    past_date.mday = s_arrived_event.event_fast_info.day;
    past_time.hour = s_arrived_event.event_fast_info.hour;
    past_time.min = s_arrived_event.event_fast_info.minute;
    past_time.sec = 0;*/

    past_date.year = past_event->year;
    past_date.mon = past_event->mon;
    past_date.mday = past_event->day;
    past_time.hour = past_event->hour;
    past_time.min = past_event->minute;
    past_time.sec = 0;
    
    //响应的都为当前时间，不应该去读取NV的时间
    MMIAPISET_FormatDateStrByDateStyle(past_date.year, past_date.mon, past_date.mday,'.',date_str,MMISET_DATE_STR_LEN);

    MMIAPISET_FormatTimeStrByTime(past_time.hour,past_time.min,(uint8*)time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
    
    if (SMART_EVENT_ALARM ==past_event->type)
    {    
        alarm_str[0].wstr_len = strlen(time_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = time_unicode_str;        
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

       // alarm_str[0].str_ptr = (uint8*)time_str;
       // alarm_str[0].length = strlen(time_str);
       // alarm_str[0].is_ucs2 = FALSE;
        MMI_GetLabelTextByLang(TXT_ALARM_CLOCK, &alarm_str[1]);                
        line_num = 2;
    }
    else if ( SMART_EVENT_AUTOPOWERON == past_event->type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = time_unicode_str;   
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_AUTOPOWER_ON, &alarm_str[1]); 
        line_num = 2;
    }
    else if ( SMART_EVENT_AUTOPOWEROFF == past_event->type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = time_unicode_str;  
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_AUTOPOWER_OFF, &alarm_str[1]); 
        line_num = 2;
    } 
    else
    {     
        alarm_str[0].wstr_len = strlen((char*)date_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = date_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,date_str,strlen(date_str),strlen(date_str));/*lint !e64*/

        alarm_str[1].wstr_len = strlen(time_str);
//        alarm_str[1].wstr_ptr = SCI_ALLOC_APP(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[1].wstr_ptr = time_unicode_str;
        SCI_MEMSET(alarm_str[1].wstr_ptr,0,(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[1].wstr_ptr,alarm_str[1].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/
            
//         alarm_str[2].wstr_len = past_event->w_length;       
//         alarm_str[2].wstr_ptr = past_event->w_content;   /*lint !e605 */

        MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &event_content,nv_return);
        alarm_str[2].wstr_len = event_content.w_length;       
        alarm_str[2].wstr_ptr = event_content.w_content;    /*lint !e605 */

        line_num = 3;
    } 
    
    if (SMART_EVENT_ALARM ==past_event->type ||
        SMART_EVENT_AUTOPOWERON == past_event->type ||
        SMART_EVENT_AUTOPOWEROFF == past_event->type) 
    {
        icon = IMAGE_CLOCK_ALARM_ALERT;
    }
    else if (SMART_EVENT_SCH == past_event->type
#ifdef MMI_AZAN_SUPPORT
        ||(SMART_EVENT_AZAN == past_event->type)
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
      ||(SMART_FM_TIMER_RECORD == past_event->type)
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
      || (SMART_EVENT_SOGOU_UPDATE == past_event->type)
#endif
    )
    {
        icon = IMAGE_CLOCK_CALENDAR_ALERT;
    }
    else
    {
        // SCI_PASSERT(0, ("OpenPastSchEventWin: Invalid past_event->type is %d",past_event->type));/*assert verified*/        
        //SCI_TRACE_LOW:"OpenPastSchEventWin error: Invalid past_event->type is %d:"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6792_112_2_18_2_1_4_42,(uint8*)"d",past_event->type);
        return;
    }

    if (SMART_EVENT_AUTOPOWERON == past_event->type)
    {
        func = (MMIPUB_HANDLE_FUNC)HandlePastAutoPowerOn;
        if(MMK_IsOpenWin(MMIALM_BYPAST_SCH_WIN_ID))
        {
            MMK_CloseWin(MMIALM_BYPAST_SCH_WIN_ID);
        }
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_BYPAST_SCH_WIN_ID,icon,PNULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,func);    
    }
    else if (SMART_EVENT_AUTOPOWEROFF == past_event->type)
    {
    
        func = (MMIPUB_HANDLE_FUNC)HandlePastEventWinMsg;
        if(MMK_IsOpenWin(MMIALM_BYPAST_SCH_WIN_ID))
        {
            MMK_CloseWin(MMIALM_BYPAST_SCH_WIN_ID);
        }
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_BYPAST_SCH_WIN_ID,icon,PNULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,func);    
    }
#ifdef MMI_AZAN_SUPPORT
    else if (SMART_EVENT_AZAN == past_event->type)
    {
        MMIAPIAZAN_OpenAlarmExpiredWin(FALSE);
    }
#endif /*MMI_AZAN_SUPPORT*/
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    else if (SMART_FM_TIMER_RECORD == past_event->type)
    {
            MMIAPIFM_StartFMTimerRecord();
    }
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    else if (SMART_EVENT_SOGOU_UPDATE == past_event->type)
    {
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
        {
            MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
        }
#ifdef MMI_WIFI_SUPPORT
        else if ( MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#else
        else if (MMIAPIPHONE_GetSimOkNum(PNULL, 0) == 0 
                    || MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#endif
        {
            // 不具备更新网络条件，不弹出更新提示
        }
        else
        {
            MMIAPIIM_SogouAutoUpdateWordRemind();
        }
    }
#endif
    else
    {
        func = (MMIPUB_HANDLE_FUNC)HandlePastEventWinMsg;
        if(MMK_IsOpenWin(MMIALM_BYPAST_SCH_WIN_ID))
        {
            MMK_CloseWin(MMIALM_BYPAST_SCH_WIN_ID);
        }
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_BYPAST_SCH_WIN_ID,icon,PNULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE, func);    
    }

}

/*****************************************************************************/
//  Description : intial Alarm module  
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_InitModule(void)
{
    MMIALM_RegMenuGroup();
    MMIALM_RegAlarmNv();
    MMIALARM_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : return alarm expired win id  
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIALM_GetAlarmPromptWinId(void)
{
    return MMIALM_ALIVEWIN_ID;
}

/*****************************************************************************/
//  Description : get first alarm ring, for setting use
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_GetFirstAlarmRing(
                              BOOLEAN       *is_fixed,
                              uint16        *ring_id,
                              uint16        *file_name_len,
                              wchar         *file_name_ptr
                              )
{
    uint16                          nv_return = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event = {0};
    MMIACC_SMART_EVENT_RING_T       event_ring = {0};

    //SCI_ASSERT(NULL != is_fixed); /*assert verified*/
    //SCI_ASSERT(NULL != ring_id); /*assert verified*/
    //SCI_ASSERT(NULL != file_name_len); /*assert verified*/
    //SCI_ASSERT(NULL != file_name_ptr); /*assert verified*/
    if(NULL == is_fixed || NULL == ring_id ||NULL == file_name_len ||NULL == file_name_ptr)
    {
        return;
    }
    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+EVENT_ALARM_0, &event, nv_return);

    if(ALM_FIX_RING == event.ring_type)
    {
        *is_fixed = TRUE;
        *ring_id = event.ring;
    }
    else
    {
        //需要fs相关操作
        *is_fixed = FALSE;

        if (!ReadRingInfoFileName(EVENT_ALARM_0,&event_ring))//保护处理
        {
            *is_fixed = TRUE;
            *ring_id = event.ring;
            //SCI_TRACE_LOW:"mmialarm.c MMIAPIALM_GetFirstAlarmRing ReadRingInfoFileName fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6923_112_2_18_2_1_4_43,(uint8*)"");
            
            return;
        }
        *file_name_len = event_ring.dring_name_len;
        MMI_WSTRNCPY( file_name_ptr, MMIFILE_FULL_PATH_MAX_LEN, 
                   event_ring.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN, MMIFILE_FULL_PATH_MAX_LEN);
    }
}

/*****************************************************************************/
//  Description : DisplayTheRingName
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void DisplayTheRingName(void)
{
    //wchar              ring_str[MMIFILE_FILE_NAME_MAX_LEN+1]  = {0};
    wchar              file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    wchar               *ring_name_ptr                         = NULL;
    uint16              file_name_len                          = 0;
    uint16              fixed_ringid                           = 0;
    uint16              ring_name_len                          = 0;
    MMI_STRING_T        ring                                   = {0};
    MMI_STRING_T        text            = {0};
#ifdef FM_SUPPORT    
    wchar fm[3]={'F','M',0};
#endif
#ifdef DRM_SUPPORT
    BOOLEAN     drm_permission = TRUE;
    if (MMIAPIFMM_IsDrmFileBySuffix(s_current_event_info.event.event_ring_info.dring_name_arr,
        s_current_event_info.event.event_ring_info.dring_name_len))
    {
        //检查是否过期了，如果过期了，设置成默认的图片
        if (MMIAPIDRM_IsRightsExpired(SFS_INVALID_HANDLE, 
            s_current_event_info.event.event_ring_info.dring_name_arr, DRM_PERMISSION_PLAY))
        {
            drm_permission = FALSE; 
            SCI_TRACE_LOW("DisplayTheRingName: drm_perssion set as false\n");
        }
    }
#endif
//     MMI_TEXT_ID_T       ring_text                              = TXT_NULL;

    fixed_ringid = s_current_event_info.event.event_content_info.ring;

    ring_name_ptr = s_current_event_info.event.event_ring_info.dring_name_arr;

    ring_name_len = s_current_event_info.event.event_ring_info.dring_name_len;

    ring.wstr_ptr = file_name;        
    
//@zhaohui.cr112262,应只有ALM_DOWNLOAD_RING和ALM_FIX_RING两种情况  
    if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type
        ||ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type)
    {
        //SCI_ASSERT(NULL != ring_name_ptr); /*assert verified*/
        if (MMIAPIUDISK_UdiskIsRun())//@zhaohui,cr111342
        {
             ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
             MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
             GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
             
        }
        else if(!MMIAPIFMM_IsFileExist(ring_name_ptr, ring_name_len)
#ifdef DRM_SUPPORT
            || !drm_permission
#endif
            )
        {
            //铃声文件不存在时重新设置铃声
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

            ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
            MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
            GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
        }
        else
        {
            MMIAPIFMM_SplitFullPath(ring_name_ptr, ring_name_len,
                                  NULL, NULL, NULL, NULL,
                                  file_name, &file_name_len);

            ring.wstr_len = file_name_len;
            ring.wstr_ptr = file_name;
        }
        
    }
#ifdef FM_SUPPORT
    else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type)
    {
        //SCI_ASSERT(NULL != ring_name_ptr); /*assert verified*/
        if (MMIAPIUDISK_UdiskIsRun())//@zhaohui,cr111342
        {
             ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
             MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
             GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
        }
        else if(!MMIAPIALM_IsValidFM(ring_name_ptr, ring_name_len))
        {
            //铃声文件不存在时重新设置铃声
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

            ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
            MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
            GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
        }
        else
        {
            //ring.wstr_len = ring_name_len;
            //ring.wstr_ptr = ring_name_ptr;
            MMI_STRING_T ring_str={0};
#ifndef FM_SUPPORT_NONE
            wchar*channel_name=PNULL;
#endif
            ring_str.wstr_len=ring_name_len;
            ring_str.wstr_ptr = ring_name_ptr;
#ifndef FM_SUPPORT_NONE
            channel_name=MMIAPIFM_GetChannelName(AlarmConvertFloatStrToInt(&ring_str,10));
#endif
#ifndef FM_SUPPORT_NONE
            if(PNULL!=channel_name)
            {
                ring.wstr_len = MMIAPICOM_Wstrlen(channel_name);
                ring.wstr_ptr = channel_name; 
            }
            else
#endif
            {
                MMIAPICOM_Wstrncpy(ring.wstr_ptr, fm, 2);
                MMIAPICOM_Wstrcat(ring.wstr_ptr, ring_name_ptr);
                ring.wstr_len=MMIAPICOM_Wstrlen(ring.wstr_ptr);
            }
        }
        
    }
#endif  //FM_SUPPORT
    else
    {
        ring.wstr_len = MMIAPISET_GetRingName(fixed_ringid, file_name, MMIFILE_FILE_NAME_MAX_LEN);
    }
//#ifdef MMI_PDA_SUPPORT
    GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &ring, FALSE);
//#else
//    GUILABEL_SetText(MMIALM_EDIT_RING_TEXT_CTRL_ID, &ring, FALSE);
//#endif
}

/*****************************************************************************/
//  Description : add item for setlist
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  AddItemSetlist(void)
{
    MMI_CTRL_ID_T           current_ctrl_id = MMIALM_EDIT_MODE_SETLIST_CTRL_ID;
//     uint32 text_id[3]=
//     {
//         TXT_ALARM_MODE_ONCE,
//         TXT_ALARM_MODE_EVERYDAY,
//         TXT_ALARM_MODE_WEEK
//     };
    uint32 loop=0;

    
    for(loop=0;loop<3;loop++)
    {
        GUISETLIST_AddItemById(current_ctrl_id, s_alarm_mode_text[loop]);
    }
    
    return;
}

/*****************************************************************************/
//  Description : process the MIDSK message
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
// LOCAL void EditWinMIDSKProcess(
//                                 MMI_WIN_ID_T win_id //in
//                                 )
// {
// //  uint32 setlist_index=0;
//     MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
//     
//     switch(ctrl_id)
//     {
//     case MMIALM_EDIT_RING_TYPE_CTRL_ID:
//         MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);
//         break;
//     case MMI_EDITWIN_BUTTON0_CTRL_ID:
//     case MMI_EDITWIN_BUTTON1_CTRL_ID:
//     case MMI_EDITWIN_BUTTON2_CTRL_ID:
//     case MMI_EDITWIN_BUTTON3_CTRL_ID:
//     case MMI_EDITWIN_BUTTON4_CTRL_ID:
//     case MMI_EDITWIN_BUTTON5_CTRL_ID:
//     case MMI_EDITWIN_BUTTON6_CTRL_ID:
//             if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
//             {
//                 s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
//             }
//             else
//             {
//                 s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
//             }
// #ifndef MMI_PDA_SUPPORT  
//             setAlarmEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
// #endif
//             SetEditWinButtonBg(win_id,ctrl_id);
//             break;
//     default:
//         break;
//     }
// }

/*****************************************************************************/
//  Description : set title for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTitleEditWin(
                                MMI_WIN_ID_T win_id //in
                                )
{
    switch(s_current_event_info.id) 
    {
    case EVENT_AUTO_POWERON:
        GUIWIN_SetTitleTextId(win_id,TXT_AUTOPOWER_ON,FALSE);
        break;
    case EVENT_AUTO_POWEROFF:
        GUIWIN_SetTitleTextId(win_id,TXT_AUTOPOWER_OFF,FALSE);
        break;            
    default:
        GUIWIN_SetTitleTextId(win_id,TXT_ALARM_CLOCK,FALSE);
        break;
    }
     
}

/*****************************************************************************/
//  Description : set name ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetNameParamEditWin(void)
{
    MMI_STRING_T  name_str        = {0};
    MMI_STRING_T  name_str1      = {0};
    MMI_STRING_T  text                = {0};
    MMI_TEXT_ID_T txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10};
    MMI_TEXT_ID_T txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                        TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                        TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        GUIFORM_SetStyle(MMIALM_EDIT_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        MMI_GetLabelTextByLang(STXT_MAIN_MESSAGE, &text);
        GUILABEL_SetText(MMIALM_EDIT_NAME_LABEL_CTRL_ID, &text, FALSE);

        MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str1);
        if (!MMIAPICOM_CompareTwoWstrExt(name_str1.wstr_ptr,
                                            name_str1.wstr_len,
                                            s_current_event_info.event.event_content_info.w_content,
                                            s_current_event_info.event.event_content_info.w_length,
                                            TRUE)
            ||(0 == s_current_event_info.event.event_content_info.w_length) )
        {
            if (s_current_event_info.id <= EVENT_ALARM_MAX) //&&s_current_event_info.id>=EVENT_ALARM_0 /*lint !e685 !e568*/
            {
                MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str);
            }
            else if(s_current_event_info.id>=EVENT_ALARM_DELAY_0&&s_current_event_info.id <= EVENT_ALARM_DELAY_MAX)
            {
                MMI_GetLabelTextByLang(txt_alarm_id_1[s_current_event_info.id-EVENT_ALARM_DELAY_0],&name_str);
            }
        }
        else
        {
            name_str.wstr_ptr = s_current_event_info.event.event_content_info.w_content;
            name_str.wstr_len = s_current_event_info.event.event_content_info.w_length;
        }
#ifdef MMI_PDA_SUPPORT
        GUILABEL_SetText(MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID, &name_str,FALSE);
#else
        GUIEDIT_SetString(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len);
#endif
    }   

     
}

/*****************************************************************************/
//  Description : set time ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTimeParamEditWin(void)
{
    
#ifndef MMI_PDA_SUPPORT
    GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
//  MMISET_TIME_T   time = {0};
    MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;
#endif
    MMI_STRING_T        title_text            = {0};

    GUIFORM_SetStyle(MMIALM_EDIT_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_TIME, &title_text);
    GUILABEL_SetText(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, &title_text, FALSE);

#ifdef MMI_PDA_SUPPORT
    {
        uint8               time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]    = {0};

        MMI_STRING_T        text            = {0};

        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            MMIALM_FormTimeStrbyTime(s_current_event_info.event.event_fast_info.hour,
                s_current_event_info.event.event_fast_info.minute,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            MMIALM_FormTimeStrbyTime(sys_time.hour,
                sys_time.min,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }

        text.wstr_len = 10;
        text.wstr_ptr = SCI_ALLOC_APP((text.wstr_len +1)* sizeof(wchar));
        SCI_MEMSET(text.wstr_ptr,0,((text.wstr_len +1)* sizeof(wchar)));
        MMI_STRNTOWSTR(text.wstr_ptr,10,(uint8*)time_str,10,10);

        GUILABEL_SetText(MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID, &text, FALSE);
        if (PNULL != text.wstr_ptr)
        {
            SCI_FREE(text.wstr_ptr);
            text.wstr_ptr = PNULL;
        }

    }
#else
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
    GUIEDIT_SetTimeStyle(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
    //GUIEDIT_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_SET_TIME_FONT);
#ifdef SCREEN_SHAPE_CIRCULAR
    GUIEDIT_SetIm(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, GUIIM_TYPE_SYS, GUIIM_TYPE_SYS);
#endif
    if (s_current_event_info.event.event_fast_info.is_valid )
    {
        GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                            s_current_event_info.event.event_fast_info.hour, 
                            s_current_event_info.event.event_fast_info.minute,
                            0);
    }
    else
    {
        SCI_TIME_T      sys_time = {0};
        TM_GetSysTime(&sys_time);
        GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                            sys_time.hour, 
                            sys_time.min,
                            0);
    }
#endif
}

/*****************************************************************************/
//  Description : set ring ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetRingParamEditWin(void)
{
    MMI_STRING_T        text            = {0};
    
    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        GUIFORM_SetStyle(MMIALM_EDIT_RING_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        MMI_GetLabelTextByLang(TXT_RING, &text);
        GUILABEL_SetText(MMIALM_EDIT_RING_LABEL_CTRL_ID, &text, FALSE); // text
/*#ifndef MMI_PDA_SUPPORT
        if(ALM_DOWNLOAD_RING==s_current_event_info.event.event_content_info.ring_type)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_MORE_RING, &text);
        }
        else if(ALM_FM_RING==s_current_event_info.event.event_content_info.ring_type)
        {
            MMI_GetLabelTextByLang(TXT_ALARM_FM_RING, &text);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
        }
        GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
#endif*/
        DisplayTheRingName();
    }
}

#ifdef  CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : set VIBRATE ctrl param for edit win
//	Global resource dependence : 
//  Author: panvs
//	Note:   
/*****************************************************************************/
LOCAL void SetVibrateParamEditWin(void)
{
    MMI_STRING_T text = {0};
    uint16 index = 0;

    GUIFORM_SetStyle(MMIALM_EDIT_VIBRATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_RING_VIBRA, &text);
    GUILABEL_SetText(MMIALM_EDIT_VIBRATE_LABLE_CTRL_ID, &text, FALSE);
    
    GUISETLIST_AddItemById(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID, TXT_CLOSE);
    GUISETLIST_AddItemById(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID, TXT_OPEN);

    if(s_current_event_info.event.event_content_info.is_vibrate)
    {
        index = 1;
    }
    else
    {
        index = 0;
    }
    
    GUISETLIST_SetCurIndex(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID, index );    
    GUISETLIST_SetFontAlign(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID,ALIGN_LVMIDDLE);

}
#endif

/*****************************************************************************/
//  Description : set mode ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetModeParamEditWin(void)
{
    MMI_STRING_T        text            = {0};
    MMI_STRING_T        mode_text       = {0};
//  GUIFONT_REGION_STYLE_T  text_region = {0};
//#ifdef MMI_PDA_SUPPORT
//    uint16 len=0;
//    wchar buff[100]={0};
//#endif
    GUIFORM_SetStyle(MMIALM_EDIT_MODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMI_GUI_STYLE_MINISCREEN
    GUIFORM_SetStyle(MMIALM_EDIT_MODE_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    
    MMI_GetLabelTextByLang(TXT_ALARM_EDIT_MODE, &text);
    GUILABEL_SetText(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &text, FALSE);
    //text_region.length = text.wstr_len;
    //text_region.start = 0;
    //text_region.state = GUISTR_STATE_UNDERLINE;
    //GUILABEL_SetTextRegion(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &text_region, 1);
//#ifdef MMI_PDA_SUPPORT
 //   len=GetFreModeString(s_current_event_info.event.event_fast_info.fre_mode,buff,100);
//    text.wstr_ptr=buff;
//    text.wstr_len=len;
//    GUILABEL_SetText(MMIALM_EDIT_MODE_SETLIST_CTRL_ID, &text, FALSE);
//#else
#ifndef SCREEN_SHAPE_CIRCULAR
    GUILIST_SetCurItemIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,
                            s_current_event_info.event.event_fast_info.fre_mode);
    GUISETLIST_SetTitleTextId(MMIALM_EDIT_MODE_SETLIST_CTRL_ID, TXT_ALARM_EDIT_MODE);
#else
    SCI_TRACE_LOW("SetModeParamEditWin  mode = %d", s_current_event_info.event.event_fast_info.fre_mode);
    if(ALM_MODE_ONCE == s_current_event_info.event.event_fast_info.fre_mode)
    {
        MMI_GetLabelTextByLang(TXT_ALARM_MODE_ONCE, &mode_text);
    }
    else if(ALM_MODE_EVERYDAY == s_current_event_info.event.event_fast_info.fre_mode)
    {
        MMI_GetLabelTextByLang(TXT_ALARM_MODE_EVERYDAY, &mode_text);
    }
    else if(ALM_MODE_EVERYWEEK == s_current_event_info.event.event_fast_info.fre_mode)
    {
        MMI_GetLabelTextByLang(TXT_ALARM_MODE_WEEK, &mode_text);
    }
    GUILABEL_SetText(MMIALM_EDIT_MODE_SELECT_LABEL_CTRL_ID, &mode_text, FALSE);
#endif
//#endif
}

/*****************************************************************************/
//  Description : set  weeks ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksParamEditWin(MMI_WIN_ID_T      win_id)
{
    uint8 day_num=0;
    uint8   week_day =0;
//  MMI_STRING_T string={0};
    uint32 ctrl_id[]={  MMI_EDITWIN_BUTTON0_CTRL_ID,
                        MMI_EDITWIN_BUTTON1_CTRL_ID,
                        MMI_EDITWIN_BUTTON2_CTRL_ID,
                        MMI_EDITWIN_BUTTON3_CTRL_ID,
                        MMI_EDITWIN_BUTTON4_CTRL_ID,
                        MMI_EDITWIN_BUTTON5_CTRL_ID,
                        MMI_EDITWIN_BUTTON6_CTRL_ID,
                    };
    GUIFORM_CHILD_WIDTH_T  width={0}; 
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};
    uint16 hor_space = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;
    GUI_BORDER_T border = {0};
	uint16 week_day_image_adjust = 0;
#ifdef MMI_GUI_STYLE_MINISCREEN
	uint16 space_sub = MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH-1;
#else
	uint16 space_sub = 1;
#endif
   
    bg.bg_type=GUI_BG_IMG;
    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif

	GUIFORM_SetStyle(MMI_EDITWIN_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_CLOCK_ALARM_WEEK_ENABLED, win_id);

    width.type=GUIFORM_CHILD_WIDTH_FIXED;
    width.add_data = (MMI_MAINSCREEN_WIDTH- space_sub * 2) / ALM_WEEK_DAY_NUM;
    GUIFORM_SetSpace(MMI_EDITWIN_CHILD_FORM_CTRL_ID, &hor_space, PNULL);    
    GUIFORM_SetMargin(MMI_EDITWIN_CHILD_FORM_CTRL_ID,0);  
    GUIFORM_PermitChildFont(MMI_EDITWIN_CHILD_FORM_CTRL_ID, FALSE);    
    border.type = GUI_BORDER_NONE;
    IGUICTRL_SetBorder(MMK_GetCtrlPtr(MMI_EDITWIN_CHILD_FORM_CTRL_ID), &border);
    
    for(day_num = 0; day_num < ALM_WEEK_DAY_NUM; day_num++)   
    {
		if ((width.add_data * ALM_WEEK_DAY_NUM + week_day_image_adjust) < (MMI_MAINSCREEN_WIDTH- space_sub * 2))
		{
			width.add_data = width.add_data + 1;
			week_day_image_adjust++;
		}
        week_day = MMIAPISET_GetWeekDayByOffset(day_num );
// #ifdef MAINLCD_DEV_SIZE_128X160 //CR133106
//         GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_TOP);
// #else
        GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_HVMIDDLE);
// #endif
        GUIBUTTON_SetTextId(ctrl_id[day_num],s_alm_week_text_id[ week_day ]);
        if(0!=(s_current_event_info.event.event_fast_info.fre & s_week_mask[week_day]) ) 
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;
            s_weeks_button_is_selected[day_num]=TRUE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;
            s_weeks_button_is_selected[day_num]=FALSE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &disabled_font);
        }
        GUIBUTTON_SetBg(ctrl_id[day_num],&bg);
        GUIBUTTON_PermitBorder(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetOwnerDraw(ctrl_id[day_num],ButtonOwnerDrawFun);
        bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        GUIBUTTON_SetRunSheen(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetPressedBg(ctrl_id[day_num],&bg);        
        GUIFORM_SetChildWidth(MMI_EDITWIN_CHILD_FORM_CTRL_ID,ctrl_id[day_num],&width);
		width.add_data = (MMI_MAINSCREEN_WIDTH - space_sub * 2) / ALM_WEEK_DAY_NUM;
    }
}

/*****************************************************************************/
//  Description : set  weeks display
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksDisplayEditWin(void)
{
    uint32 setlist_index=0;

    GUIFORM_SetCircularHandleLeftRight(MMI_EDITWIN_CHILD_FORM_CTRL_ID,TRUE);

    setlist_index=GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID);
    if(ALM_MODE_EVERYWEEK!=setlist_index)
    {
        
        GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID, 
                            //MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID, 
                            MMI_EDITWIN_CHILD_FORM_CTRL_ID,
                            GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        
        GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID, 
                            //MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,
                            MMI_EDITWIN_CHILD_FORM_CTRL_ID,
                            GUIFORM_CHILD_DISP_NORMAL);
        
    }
}

/*****************************************************************************/
//  Description : save name
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveNameEditWin(void)
{
    MMI_STRING_T        str        = {0};
    MMIACC_SMART_EVENT_CONTENT_T*event_content_ptr=PNULL;
    MMI_STRING_T  name_str      = {0};
    MMI_TEXT_ID_T txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                        TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                        TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10};
    MMI_TEXT_ID_T txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                        TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                        TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    event_content_ptr = &s_current_event_info.event.event_content_info;
    GUIEDIT_GetString(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, &str);
    if (MMIAPISET_IsIncludeReturnChar(str.wstr_ptr,str.wstr_len))
    {
        MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_TITLE);
        return FALSE;
    }
                
    if(0==str.wstr_len)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMM_NO_TITLE);
        return FALSE;
    }
    if(s_current_event_info.id <= EVENT_ALARM_MAX) //&&s_current_event_info.id>=EVENT_ALARM_0 /*lint !e685 !e568*/
    {
        MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str);
    }
    else if(s_current_event_info.id>=EVENT_ALARM_DELAY_0&&s_current_event_info.id <= EVENT_ALARM_DELAY_MAX)
    {
        MMI_GetLabelTextByLang(txt_alarm_id_1[s_current_event_info.id-EVENT_ALARM_DELAY_0],&name_str);
    }
    if(0==MMIAPICOM_CompareTwoWstrExt(name_str.wstr_ptr,
                                            name_str.wstr_len,
                                            str.wstr_ptr,
                                            str.wstr_len,
                                            TRUE))
    {
        /*if(0==MMIAPICOM_CompareTwoWstrExt(event_content_ptr->w_content,
                                            event_content_ptr->w_length,
                                            str.wstr_ptr,
                                            str.wstr_len,
                                            TRUE))
        {
            return TRUE ; 
        }*/
        event_content_ptr->w_length=0;
        SCI_MEMSET(event_content_ptr->w_content,0,(sizeof(wchar))*(MMIACC_EVENT_TEXT_MAX_LEN+1));
        return TRUE ; 
    }
    
    event_content_ptr->w_length = str.wstr_len;
    MMI_WSTRNCPY(
    event_content_ptr->w_content, MMIACC_EVENT_TEXT_MAX_LEN,
    str.wstr_ptr, str.wstr_len,
    str.wstr_len);

    return TRUE ;
}


/*****************************************************************************/
//  Description : save time
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimeEditWin(void)
{
    MMISET_TIME_T   time = {0};
    
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
     MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else	
	GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
    if ( MMIAPICOM_CheckTimeIsValid(time))
    {
        s_current_event_info.event.event_fast_info.hour = time.time_hour; 
        s_current_event_info.event.event_fast_info.minute = time.time_minute;
    }
    else   // time invalid
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
        return FALSE;
    }
    return TRUE ;
}

/*****************************************************************************/
//  Description : save mode
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  SaveModeEditWin(void)
{
//  int32 loop=0;
//  uint16      mode = 0;
#ifdef SCREEN_SHAPE_CIRCULAR
    uint32 setlist_index = GUILIST_GetCurItemIndex(POWERONOFF_REPEAT_LIST_CTRL_ID);
#else
    uint32 setlist_index=GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID);
#endif
    switch(setlist_index)
    {
        case 0:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;
        case 1:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
            break;
        case 2:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            /*if(0==s_current_event_info.event.event_fast_info.fre )//不允许一天也不选。会死机的。
            {
                for(loop=1;loop<ALM_WEEK_DAY_NUM-1;loop++)//没选的话默认选周一~  周五
                {
                    mode |= s_week_mask[loop];
                }

                s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
            }*/
            break;
        default:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;
            
    }
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : set RingDur ctrl param for edit win
//	Global resource dependence : 
//  Author: panvs
//	Note:   
/*****************************************************************************/
LOCAL void SetRingDurParamEditWin(void)
{
    MMI_STRING_T text = {0};
    uint16 index = 0;
    uint32 i = 0;
    GUISETLIST_ITEM_INFO_T item_info = {0};
    wchar wstr[50] = {0};
    char   str[5][3]  = {{'1','\0','\0'},{'5','\0','\0'},{'1','0','\0'},{'1','5','\0'},{'3','0','\0'}};
    uint16 str_len  = 0;
        
    GUIFORM_SetStyle(MMIALM_EDIT_RING_DUR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_RING_DUR_TIME, &text);
    GUILABEL_SetText(MMIALM_EDIT_RING_DUR_LABLE_CTRL_ID, &text, FALSE);

    //set string
    SCI_MEMSET(&text, 0, sizeof(MMI_STRING_T));
    MMI_GetLabelTextByLang(TXT_TIME_MINUTE, &text);
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_ptr = wstr;
    for(i=0; i<5; i++)
    {
        SCI_MEMSET(wstr, 0, sizeof(wstr));
        
        str_len = strlen(str[i]);
        MMI_STRNTOWSTR(wstr, 50, (uint8*)str[i], str_len, str_len);
        
        MMI_WSTRNCPY(&wstr[str_len], 50-str_len, text.wstr_ptr, text.wstr_len, text.wstr_len);
        item_info.str_info.wstr_len = text.wstr_len+str_len;
        
        GUISETLIST_AddItem(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID,&item_info);
    }
    
    switch(s_current_event_info.event.event_content_info.ring_dur)
    {
        case 1:
            index = 0;
            break;
        case 5:
            index = 1;
            break;
        case 10:
            index = 2;
            break;
        case 15:
            index = 3;
            break;
        case 30:
            index = 4;
            break;
        default:
            index = 0;
            break;
    }
    
    GUISETLIST_SetCurIndex(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID, index);    
    GUISETLIST_SetTitleTextId(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID, TXT_RING_DUR_TIME);
    GUISETLIST_SetFontAlign(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
}

/*****************************************************************************/
// 	Description : save RingDur
//	Global resource dependence : 
//  Author: panvs
//	Note:   
/*****************************************************************************/
LOCAL void  SaveRingDurEditWin(void)
{
    uint32 index=GUISETLIST_GetCurIndex(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID);

    switch(index)
    {
        case 0:
            s_current_event_info.event.event_content_info.ring_dur = 1;
            break;
        case 1:
            s_current_event_info.event.event_content_info.ring_dur = 5;
            break;
        case 2:
            s_current_event_info.event.event_content_info.ring_dur = 10;
            break;
        case 3:
            s_current_event_info.event.event_content_info.ring_dur = 15;
            break;
        case 4:
            s_current_event_info.event.event_content_info.ring_dur = 30;
            break;
        default:
            s_current_event_info.event.event_content_info.ring_dur = 1;
            break;
    }
}
#endif

/*****************************************************************************/
//  Description : Append the item of weeks list
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
// LOCAL void AppendWeeksListItem(
//                                                MMI_CTRL_ID_T    ctrl_id //in
//                                                )
// {
//         uint8 day_num=0;
//     uint8   week_day =0;
//     GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
//        GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
// 
//     //add items to listbox
//     for(day_num = 0; day_num < ALM_WEEK_DAY_NUM; day_num++)   
//     {
//         week_day = MMIAPISET_GetWeekDayByOffset(day_num );
//         //MMIAPISET_AppendListItemByTextId(s_alm_week_text_id[ week_day ], STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_CHECK );
//     
//         item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
//         item_t.item_data_ptr = &item_data;
// 
//         item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
//         item_data.item_content[0].item_data.text_id = s_alm_week_text_id[ week_day ];
// 
//         //item_data.softkey_id[0] = STXT_OK;
//         //item_data.softkey_id[1] = STXT_SELECT;
//         //item_data.softkey_id[2] = STXT_RETURN;
//     
//             GUILIST_AppendItem( ctrl_id, &item_t );  
// 
//         if ((s_current_event_info.event.event_fast_info.fre & s_week_mask[week_day]) != 0) 
//         {
//             GUILIST_SetSelectedItem(ctrl_id, day_num, TRUE);
//         }
//     }  
// }

/*****************************************************************************/
//  Description : save the select weeks
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
// LOCAL uint16 SaveSelectWeeks(
//                                                MMI_CTRL_ID_T    ctrl_id //in
//                                                )
// {
//     uint16      cur_selection[ALM_WEEK_DAY_NUM]   =  {0};
//     uint16      cur_num       =   0;
//     uint8   week_day =0;
//     int32 loop=0;
// //  uint32      i = 0; 
//     uint16      mode = 0;
//     
//     cur_num=GUILIST_GetSelectedItemIndex(ctrl_id, cur_selection, ALM_WEEK_DAY_NUM);
// 
//     if(0==cur_num)//不允许一项也不选，否则保存闹钟时会死机
//     {
//         return cur_num;
//     }
// 
//     for(loop=0;loop<cur_num;loop++)
//     {
//         week_day = MMIAPISET_GetWeekDayByOffset(cur_selection[loop]);
//         mode |= s_week_mask[week_day];
//     }
// 
//     if (mode != s_current_event_info.event.event_fast_info.fre) 
//     {
//         s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
//     }
//     
//     return cur_num;
// }
#if  defined DPHONE_SUPPORT || defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : form time str by time
//  Global resource dependence : 
//  Author:        zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void MMIALM_FormTimeStrbyTime(
                      uint8 hour,//in
                      uint8 min,//in
                      char* time_str,//out
                      uint16 time_str_max_len//in
                      )
{
    MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_12HOURS;

    //SCI_ASSERT(PNULL != time_str);  /*assert verified*/
    //SCI_ASSERT(time_str_max_len >= MMIALM_TIME_STR_12HOURS_LEN + 1);  /*assert verified*/
    if(PNULL == time_str || time_str_max_len < (MMIALM_TIME_STR_12HOURS_LEN + 1))
    {
        return;
    }
    
    time_type = MMIAPISET_GetTimeDisplayType();
    if (MMISET_TIME_12HOURS == time_type)
    {
        if ((12 > hour))//AM
        {
            if ((1 > hour))//00:00-1:00
            {
                hour = (uint8)(hour + 12);
            }
            sprintf( (char*)time_str,"%02d:%02dAM",hour,min);
        }
        else//PM
        {
            if ((12 <= hour) && (13 > hour))//12:00-13:00
            {
            }
            else
            {
                hour = (uint8)(hour - 12);
            }
            sprintf( (char*)time_str,"%02d:%02dPM",hour,min);
        }
    }
    else
    {
        sprintf( (char*)time_str, "%02d:%02d", hour, min );
    }
}
#endif

#ifdef DPHONE_SUPPORT
LOCAL void DisplayAlarmTime(void)
{
    uint8               time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]    = {0};

    MMI_STRING_T        text            = {0};

        //时间      
        MMI_GetLabelTextByLang(TXT_TIME, &text);
        GUILABEL_SetText(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, &text, FALSE);

        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            MMIALM_FormTimeStrbyTime(s_current_event_info.event.event_fast_info.hour,
                s_current_event_info.event.event_fast_info.minute,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            MMIALM_FormTimeStrbyTime(sys_time.hour,
                sys_time.min,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }

        text.wstr_len = 10;
        text.wstr_ptr = SCI_ALLOC_APP((text.wstr_len +1)* sizeof(wchar));
        SCI_MEMSET(text.wstr_ptr,0,((text.wstr_len +1)* sizeof(wchar)));
        MMI_STRNTOWSTR(text.wstr_ptr,10,(uint8*)time_str,10,10);

        GUILABEL_SetText(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &text, FALSE);
        if (PNULL != text.wstr_ptr)
        {
            SCI_FREE(text.wstr_ptr);
            text.wstr_ptr = PNULL;
        }
   //     GUILABEL_SetBackgroundColor(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_WINDOW_BACKGROUND_COLOR);
        // end
}

LOCAL void DisplayAlarmRingType(void)
{
    MMI_STRING_T        text            = {0};

    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        // begin
        MMI_GetLabelTextByLang(TXT_RING, &text);
        GUILABEL_SetText(MMIALM_EDIT_RING_LABEL_CTRL_ID, &text, FALSE); // text

        // begin
        DisplayTheRingName();
    }
    else
    {
    
    }
}

LOCAL void DisplayAlarmMode(void)
{
    MMI_STRING_T        text            = {0};

    MMI_GetLabelTextByLang(TXT_ALARM_EDIT_MODE, &text);
    
    GUILABEL_SetText(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &text, FALSE);
    

    if (s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_ONCE)
        {
                MMI_GetLabelTextByLang(TXT_ALARM_MODE_ONCE, &text);
        }
    else if (s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_EVERYDAY)
        {
                MMI_GetLabelTextByLang(TXT_ALARM_MODE_EVERYDAY, &text);
        }
    else  if (s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_EVERYWEEK)
        {
                MMI_GetLabelTextByLang(TXT_ALARM_MODE_WEEK, &text);
        }  

     GUILABEL_SetText(MMIALM_MODELIST_CTRL_ID, &text, FALSE);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:zhangwp
//  Note:
/*****************************************************************************/
LOCAL void AppendAlarmModeListBoxItem(
                                               MMI_CTRL_ID_T    ctrl_id,
                                               MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr  //in
                                               )
{
//    int32               i               =   0;

    //add items to listbox
    MMIAPISET_AppendListItemByTextId(TXT_ALARM_MODE_ONCE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_ALARM_MODE_EVERYDAY, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_ALARM_MODE_WEEK, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
   
    switch(event_ptr->fre_mode)
    {
    case ALM_MODE_ONCE:
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_ONCE, TRUE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYDAY, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYWEEK, FALSE);
        break;

    case ALM_MODE_EVERYDAY:
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_ONCE, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYDAY, TRUE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYWEEK, FALSE);
        break;

    case ALM_MODE_EVERYWEEK:
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_ONCE, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYDAY, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYWEEK, TRUE);
        break;
        
    default:
        break;
    }  
    GUILIST_SetCurItemIndex(ctrl_id,event_ptr->fre_mode); //add by sandywan
}


/*****************************************************************************/
//  Description :set startup mode(sunday to saturday ) of alarm 
//  Global resource dependence : 
//  Author: zhangwp
//  Note: 
/*****************************************************************************/
LOCAL void AppendAlarmWeekDayListBoxItem(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    uint16      max_item = ALM_MODE_LIST;
    uint16      i = 0;

    //set list max item and selected max item
    GUILIST_SetMaxItem(ctrl_id,max_item,FALSE);
    GUILIST_SetMaxSelectedItem(ctrl_id,max_item);

    //append item
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[0],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[1],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[2],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[3],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[4],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[5],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[6],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);

    for (i=0; i<max_item;i++)
    {
        if (0x00000001 == ((s_current_event_info.event.event_fast_info.fre>>i) & 0x00000001))
        {
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id,i,TRUE);
        }
    }
}

/*****************************************************************************/
//  Description : save current frequency setting
//  Global resource dependence : 
//  Author:        zhangwp
//  Note:  
/*****************************************************************************/
LOCAL void UpdateAlarmMode(
                           uint16   *mode_ptr, 
                           uint16   old, 
                           uint16   total
                           )
{
    uint32      i = 0; 
    uint16      mode = 0;
    
    //SCI_ASSERT(PNULL != mode_ptr); /*assert verified*/
    //SCI_ASSERT(total<=ALM_MODE_LIST); /*assert verified*/
    if(PNULL == mode_ptr ||total > ALM_MODE_LIST)
    {
        return;
    }
    
    mode = 0;
    for (i=0; i<total; i++)
    {
        mode |= s_week_mask[mode_ptr[i]];
    }

    if (mode != old) 
    {
        s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
    }
}


/*****************************************************************************/
//  Description : set time of alarm and set status to ON and save to EFS
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmTimeWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   current_ctrl_id = MMIALM_SETTIME_CTRL_ID;
    MMISET_TIME_T   time = {0};
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:  
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
        GUIEDIT_SetTimeStyle(current_ctrl_id,PNULL,&time_style,PNULL,FALSE);
        GUIEDIT_SetFont(current_ctrl_id, MMI_SET_TIME_FONT);
        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            GUIEDIT_SetTime(
            current_ctrl_id,
            s_current_event_info.event.event_fast_info.hour,
            s_current_event_info.event.event_fast_info.minute,
            0);
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            GUIEDIT_SetTime(
            current_ctrl_id,
            sys_time.hour,
            sys_time.min,
            0); 
        }
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);

        //SCI_ASSERT(s_current_event_info.id <= EVENT_AUTO_POWEROFF); /*assert verified*/
        GUIWIN_SetTitleTextId(win_id, TXT_TIME, FALSE); // for cr40327
        break;

        case MSG_FULL_PAINT:
        break;

        case MSG_CTL_OK:        // completed
        case MSG_APP_OK:       
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        GUIEDIT_GetTime(current_ctrl_id, &time.time_hour, &time.time_minute, PNULL);
        if ( MMIAPICOM_CheckTimeIsValid(time))
        {
            s_current_event_info.event.event_fast_info.hour = time.time_hour; 
            s_current_event_info.event.event_fast_info.minute = time.time_minute;
            MMK_CloseWin(win_id);
    
            s_current_event_info.event.event_fast_info.is_on = TRUE;
            // 关闭延时闹钟
            SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
            // 提示窗口
            //@zhaohui,cr108847
            if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
            {
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_MEMORY_FULL);
            }   
            break;
        
        }
        else   // time invalid
        {
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
        }
        break;

   
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

// set alarm ON after set time
WINDOW_TABLE( ALARM_TIMEWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmTimeWinMsg ),
    WIN_ID( MMIALM_TIMEWIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

   /* CREATE_EDIT_TIME_CTRL(
                    0, MMI_CLIENT_RECT_TOP, 
                    MMI_MAINSCREEN_RIGHT_MAX_PIXEL,
                    MMI_CLIENT_RECT_BOTTOM,
                    MMIALM_SETTIME_CTRL_ID
                    ), */
    CREATE_EDIT_TIME_CTRL(MMIALM_SETTIME_CTRL_ID), 
     END_WIN
}; 
/*****************************************************************************/
//  Description :set startup mode(sunday to saturday ) of alarm and save to EFS
//  Global resource dependence : 
//  Author:        ZHANGWP
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmWeekDayModeWinMsg(
                                                  MMI_WIN_ID_T      win_id, 
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  DPARAM            param
                                                  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    uint16              sel_items[ALM_MODE_LIST] = {0};
    uint16              sel_count = 0;
    MMI_CTRL_ID_T       current_ctrl_id = MMIALM_MODSEL_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      //create check list box
            AppendAlarmWeekDayListBoxItem(current_ctrl_id);
            if (0 == s_current_event_info.event.event_fast_info.fre)
            {
                s_current_event_info.event.event_fast_info.fre = 0x3e;
            }

            // read info of alarm , set check flag to indicate if the alarm is setting
            GUILIST_SetMaxSelectedItem(current_ctrl_id, ALM_MODE_LIST);
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;

    case MSG_CTL_CANCEL:  //MSG_CTL_OK:     
            sel_count = GUILIST_GetSelectedItemIndex(current_ctrl_id,sel_items, ALM_MODE_LIST);
            //  save current settings of ferquency to EFS
            if (0 == sel_count)         // no day selected
            {
                MMIPUB_OpenAlertWarningWin(TXT_MUST_SELECT_ONE);
            }
            else
            {
                UpdateAlarmMode(sel_items, s_current_event_info.event.event_fast_info.fre, sel_count);
                s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

                //      MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                MMK_CloseWin(win_id);
            }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

WINDOW_TABLE( ALARM_WEEKDAYMODEWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmWeekDayModeWinMsg ),    
    WIN_ID( MMIALM_WEEKDAYMODEWIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E,MMIALM_MODSEL_CTRL_ID),
     END_WIN
}; 

/*****************************************************************************/
//  Description :set startup mode(frequency)
//  Global resource dependence : 
//  Author:        zhangwp
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmModeWinMsg(
                                          MMI_WIN_ID_T win_id,  
                                          MMI_MESSAGE_ID_E msg_id,      
                                          DPARAM param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    uint16                  item_id = 0;


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIALM_MODELIST_CTRL_ID, 3, FALSE);//max item 3
        AppendAlarmModeListBoxItem(MMIALM_MODELIST_CTRL_ID,&s_current_event_info.event.event_fast_info);
        MMK_SetAtvCtrl(win_id,MMIALM_MODELIST_CTRL_ID);
        break;

    case MSG_NOTIFY_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:        
        item_id = GUILIST_GetCurItemIndex(MMIALM_MODELIST_CTRL_ID);
        s_current_event_info.event.event_fast_info.fre_mode = (MMIACC_MODE_TYPE_E)item_id;
        switch( item_id)
        {
            case ALM_MODE_ONCE:
                s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
                s_current_event_info.event.event_fast_info.fre = MASK_B7; //meanse only once
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

           //     MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                MMK_CloseWin(win_id);
                break;

            case ALM_MODE_EVERYDAY:
                s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
                s_current_event_info.event.event_fast_info.fre = 0x7f; 
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

            //    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                MMK_CloseWin(win_id);
                break;

            case ALM_MODE_EVERYWEEK:
                MMK_CreateWin((uint32*)ALARM_WEEKDAYMODEWIN_TAB, PNULL);
                MMK_CloseWin(win_id);
                break;

            default:
                break;
        }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;            
}

WINDOW_TABLE( ALARM_MODEWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmModeWinMsg ),    
    WIN_ID( MMIALM_MODEWIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIALM_MODELIST_CTRL_ID ),
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 



/*****************************************************************************/
//  Description : EditWinDrawFocusRect 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color
                                )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);

    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}


/*****************************************************************************/
//  Description : EditWinCleanFocus
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinCleanFocus(
                             uint8          index,
                             MMI_WIN_ID_T   win_id
                             )
{
 //   uint8            i            = 0;
//    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

//    MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);
    
    switch(index) 
    {
    case 0:  //ring time
        {
            if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
            {
          //      IGUICTRL_T *ctrl_ptr = NULL;

             //   GUILABEL_GetRect(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,&rect);
         //       ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
           /*        GUIRES_DisplayImg(PNULL,
                &rect,
                &rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);*/
                //                GUILABEL_SetTextScroll(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, FALSE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_WHITE_COLOR/*该颜色需改*/);
            GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
#else
            //  LCD_FillRect(&lcd_dev_info, focus_rec[0], MMI_WHITE_COLOR);
            //     GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
            EditWinDrawFocusRect(focus_rec[0], ALM_EDIT_CLEAN_LINE_COLOR); 
#endif

              //  IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);
            }
        }
        break;
    
    case 1: //ring type
        {
         //   IGUICTRL_T *ctrl_ptr = NULL;

        //    ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_RING_TEXT_CTRL_ID);
          /*       GUIRES_DisplayImg(PNULL,
            &rect,
            &rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);*/
            //            GUILABEL_SetTextScroll(MMIALM_EDIT_RING_TEXT_CTRL_ID, FALSE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_RING_TEXT_CTRL_ID, MMI_WHITE_COLOR/*该颜色需改*/);
            GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);

#else
            //  LCD_FillRect(&lcd_dev_info, focus_rec[1], MMI_WHITE_COLOR);
            //GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);

            EditWinDrawFocusRect(focus_rec[1], ALM_EDIT_CLEAN_LINE_COLOR); 
#endif

         //   IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT, NULL);
        }
        break;

    case 2: //alarm mode
        {
            if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
            {
            //    IGUICTRL_T *ctrl_ptr = NULL;

                //       ctrl_ptr = MMK_GetCtrlPtr(MMIALM_MODELIST_CTRL_ID);

                /*    GUIRES_DisplayImg(PNULL,
                &rect,
                &rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);*/
                //                GUILABEL_SetTextScroll(MMIALM_MODELIST_CTRL_ID, FALSE);
#if 0
                GUILABEL_SetBackgroundColor(MMIALM_MODELIST_CTRL_ID, MMI_WHITE_COLOR/*该颜色需改*/);
                GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
#else
                //      LCD_FillRect(&lcd_dev_info, focus_rec[2], MMI_WHITE_COLOR);
                //      GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
                EditWinDrawFocusRect(focus_rec[2], ALM_EDIT_CLEAN_LINE_COLOR); 
#endif
                //   IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT, NULL);
            }
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : EditWinDrawFocus 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinDrawFocus(
                            uint8           index,
                            MMI_WIN_ID_T    win_id
                            )
{
    //时间，铃声and alarm mode 输入框需要有白色线框
    MMI_TEXT_ID_T   middlesoft_id   = TXT_NULL;
//    GUI_RECT_T rect = {ALM_EDIT_X2-1, ALM_EDIT_Y1+1, ALM_EDIT_X3, ALM_EDIT_Y1 + ALM_EDIT_HIGHT};

//    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    #if 0 // if  make a frame for every item, need add this part;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR); /*this is for ring time.*/

    if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
    rect.top = ALM_EDIT_Y0 - 1;
    rect.bottom = ALM_EDIT_Y0 + ALM_EDIT_HIGHT;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR);//this is for alarm name, don't care.
  
    rect.top = ALM_EDIT_Y2;
    rect.bottom = ALM_EDIT_Y2 + ALM_EDIT_HIGHT ;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR);//this is for ring type

    rect.top = ALM_EDIT_Y3;
    rect.bottom = ALM_EDIT_Y3 + ALM_EDIT_HIGHT ;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR);//this is for alarm mode.
    }
#endif

    switch(index) 
    {
        case 0: //alarm time
        {
            //   IGUICTRL_T *ctrl_ptr = NULL;

            //    ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
            //            GUILABEL_SetTextScroll(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, TRUE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_BLACK_COLOR/*该颜色需改*/);
            GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);

#else
            //    LCD_FillRect(&lcd_dev_info, focus_rec[0], MMI_BLACK_COLOR);
            //     GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);

            EditWinDrawFocusRect(focus_rec[0], ALM_EDIT_FOCUS_LINE_COLOR); 
#endif
            //     IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);
            middlesoft_id = TXT_EDIT; 
        }
        break;

        case 1: // alarm ring
        {
            if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
            {
            //    IGUICTRL_T *ctrl_ptr = NULL;

            //  ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_RING_TEXT_CTRL_ID);
            //                GUILABEL_SetTextScroll(MMIALM_EDIT_RING_TEXT_CTRL_ID, TRUE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_RING_TEXT_CTRL_ID, MMI_BLACK_COLOR/*该颜色需改*/);
            GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
#else
            //  LCD_FillRect(&lcd_dev_info, focus_rec[1], MMI_BLACK_COLOR);
            //  GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);

            EditWinDrawFocusRect(focus_rec[1], ALM_EDIT_FOCUS_LINE_COLOR); 
#endif

            //    IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);
            middlesoft_id = STXT_SELECT;        
            }
        }
        break;

        case 2: //alarm mode
        {
         //  IGUICTRL_T *ctrl_ptr = NULL;
                    
        //   ctrl_ptr = MMK_GetCtrlPtr(MMIALM_MODELIST_CTRL_ID);
            //                GUILABEL_SetTextScroll(MMIALM_MODELIST_CTRL_ID, TRUE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_MODELIST_CTRL_ID, MMI_BLACK_COLOR/*该颜色需改*/);
            GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
#else
       //     LCD_FillRect(&lcd_dev_info, focus_rec[2], MMI_BLACK_COLOR);
        //    GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
            EditWinDrawFocusRect(focus_rec[2], ALM_EDIT_FOCUS_LINE_COLOR); 
#endif

          //  IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);

        middlesoft_id = STXT_SELECT;        
        }
        break;

        default:
        break;
    }
//更新软键栏
GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, middlesoft_id, STXT_RETURN, TRUE);
}


/*****************************************************************************/
//  Description : EditWinKeyWebDown 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinKeyWebDown(
                             MMI_MESSAGE_ID_E   msg_id,
                             MMI_WIN_ID_T       win_id
                             )
{
//    uint8 index = 0;

    switch(s_edit_active_ctrl)
    {   
    case 0: // time
        // s_edit_active_ctrl = 0;
        if (MSG_KEYDOWN_WEB == msg_id)
        {
            MMK_CreateWin((uint32*)ALARM_TIMEWIN_TAB, PNULL);
        }
        break;
        
    case 1: // ring type
        //s_edit_active_ctrl = 1;
        if (MSG_KEYDOWN_WEB == msg_id)
        {
#ifdef DPHONE_SUPPORT
            MMK_CreateWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);//@zhaohui,cr108296
#endif
        }
        break;

    case 2: // alarm mode
        //s_edit_active_ctrl = 2;
        if (MSG_KEYDOWN_WEB == msg_id)
        {
        MMK_CreateWin((uint32 *)ALARM_MODEWIN_TAB, PNULL);
        }
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : HandleAlarmEditCtrlId 处理编辑窗口的控件
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void HandleAlarmEditCtrlId(
                                 MMI_MESSAGE_ID_E   msg_id,
                                 MMI_WIN_ID_T       win_id
                                 )
{
    switch(s_edit_active_ctrl)
    {
    case 0: //time
        //s_edit_active_ctrl = 0;
        if (MSG_KEYUP_WEB != msg_id)
        {
            MMK_CreateWin((uint32*)ALARM_TIMEWIN_TAB, PNULL);
        }
        break;
    
    case 1: // ring type
        //s_edit_active_ctrl = 1;
        if (MSG_KEYUP_WEB != msg_id)
        {
#ifdef DPHONE_SUPPORT
            MMK_CreateWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);//@zhaohui,cr108296
#endif
        }
        break;
        
    case 2: // alarm mode
        //s_edit_active_ctrl = 2;
        if (MSG_KEYUP_WEB != msg_id)
        {
        MMK_CreateWin((uint32 *)ALARM_MODEWIN_TAB, PNULL);
        }
        break;

    default:
        break;
    }

}

LOCAL void EditWinKeyDown(MMI_WIN_ID_T      win_id)
{
        EditWinCleanFocus(s_edit_active_ctrl,win_id);

        if(s_edit_active_ctrl == 2)
        {
            s_edit_active_ctrl = 0;
        }
        else
        {
            s_edit_active_ctrl++;
        }

        EditWinDrawFocus(s_edit_active_ctrl,win_id);   
        MMK_SetAtvCtrlEx(win_id, s_edit_active_ctrl_attr[s_edit_active_ctrl], FALSE);
        //   MMK_SendMsg( s_edit_active_ctrl_attr[s_edit_active_ctrl], MSG_CTL_PAINT, 0 );
}

LOCAL void EditWinKeyUp(MMI_WIN_ID_T      win_id)
{
        EditWinCleanFocus(s_edit_active_ctrl,win_id);

        if(s_edit_active_ctrl == 0)
        {
            s_edit_active_ctrl = 2;
        }
        else
        {
            s_edit_active_ctrl--;
        }

        EditWinDrawFocus(s_edit_active_ctrl,win_id);    
        MMK_SetAtvCtrlEx(win_id, s_edit_active_ctrl_attr[s_edit_active_ctrl], FALSE);
        //MMK_SendMsg( s_edit_active_ctrl_attr[s_edit_active_ctrl], MSG_CTL_PAINT, 0 );
      
}

/*****************************************************************************/
//  Description : handle alarm edit windows messages
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    GUI_RECT_T          bk_rect         = MMI_CLIENT_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;

     MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
            {
            GUI_RECT_T rect = {0};
            
// 1st line--alarm time
            rect.left = ALM_EDIT_X1;
            rect.top = ALM_EDIT_Y1;
            rect.right = ALM_EDIT_X2 - 2;
            rect.bottom = ALM_EDIT_Y1 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, &rect, FALSE);
            rect.left = ALM_EDIT_X2;
            rect.top = ALM_EDIT_Y1;
            rect.right = ALM_EDIT_X3;
            rect.bottom = ALM_EDIT_Y1 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &rect, FALSE);
// second line--ring type          
            rect.left = ALM_EDIT_X1;
            rect.top = ALM_EDIT_Y2;
            rect.right = ALM_EDIT_X2 - 2;
            rect.bottom = ALM_EDIT_Y2 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_RING_LABEL_CTRL_ID, &rect, FALSE);
            rect.left = ALM_EDIT_X2;
            rect.top = ALM_EDIT_Y2;
            rect.right = ALM_EDIT_X3;
            rect.bottom = ALM_EDIT_Y2 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_RING_TEXT_CTRL_ID, &rect, FALSE);

// 3rd line--alarm mode
            rect.left = ALM_EDIT_X1;
            rect.top = ALM_EDIT_Y3;
            rect.right = ALM_EDIT_X2 - 2;
            rect.bottom = ALM_EDIT_Y3 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &rect, FALSE);
            rect.left = ALM_EDIT_X2;
            rect.top = ALM_EDIT_Y3;
            rect.right = ALM_EDIT_X3;
            rect.bottom = ALM_EDIT_Y3 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_MODELIST_CTRL_ID, &rect, FALSE);

            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);

            MMK_SetAtvCtrl(win_id, s_edit_active_ctrl_attr[s_edit_active_ctrl]);
            }
        break;

    case MSG_FULL_PAINT:
      //  IMG_EnableTransparentColor(TRUE);
        GUIRES_DisplayImg(PNULL,
            &bk_rect,
            &bk_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        //SCI_ASSERT((s_current_event_info.id <= EVENT_ALARM_DELAY_2)||(s_current_event_info.id == EVENT_AUTO_POWERON)||(s_current_event_info.id == EVENT_AUTO_POWEROFF)); /*assert verified*/
     
        DisplayAlarmTime();
        DisplayAlarmRingType();
    //    IMG_EnableTransparentColor(FALSE); 
        DisplayAlarmMode();
        EditWinDrawFocus(s_edit_active_ctrl,win_id);
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        EditWinKeyWebDown(msg_id,win_id);
        HandleAlarmEditCtrlId(msg_id,win_id);
        break;

    case MSG_KEYDOWN_UP:
        EditWinKeyUp(win_id);
        break;
        
    case MSG_KEYDOWN_DOWN:
           EditWinKeyDown(win_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        s_current_event_info.event.event_fast_info.is_on = TRUE;
#ifndef WIN32
        //SCI_TRACE_LOW:"%s  %d  Event %d is set to on!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_8646_112_2_18_2_1_8_44,(uint8*)"sdd",__func__,__LINE__,s_current_event_info.id);
#endif
        // 关闭延时闹钟
        SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
        // 提示窗口
        //@zhaohui,cr108847
        if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
        {
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_MEMORY_FULL);
        }   
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        s_edit_active_ctrl = 0;
        MMK_CloseWin(MMIAUTOPOWER_OPTWIN_ID);
        break;
        
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
              }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#else
/*****************************************************************************/
//  Description : handle alarm edit windows messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id =MMIALM_EDIT_MODE_SETLIST_CTRL_ID;
//#ifdef MMI_PDA_SUPPORT
    uint16                  img_width  = 0;
    uint16                  img_height = 0;
    GUIFORM_CHILD_WIDTH_T   button_width = {0};
    GUIFORM_CHILD_HEIGHT_T  button_height = {0};
#ifdef MMI_RECORD_SUPPORT
    GUI_BG_T bg={0};
#endif
	BOOLEAN is_need_save = TRUE;
//#endif
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T  form_rect = MMI_EDITWIN_FORM_RECT;
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_PDA_SUPPORT
		GUIFORM_SetCircularHandleUpDown(MMI_EDITWIN_FORM_CTRL_ID,TRUE);
#endif
        GUIFORM_SetType(MMI_EDITWIN_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        GUISETLIST_SetFontAlign(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
        //GUIEDIT_SetAlign(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);
        GUIFORM_SetStyle(MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIAPICTRL_SetState( MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUICTRL_STATE_DISABLE_TP,TRUE ); 
        //NEWMS00222350
        GUIEDIT_SetHandleRedKey(FALSE, MMIALM_EDIT_NAME_EDITBOX_CTRL_ID);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DROPDOWNLIST_ARROW_PR, win_id);
        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildWidth(MMIALM_EDIT_MODE_FORM_CTRL_ID, MMIALM_EDIT_MODE_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_MODE_FORM_CTRL_ID, MMIALM_EDIT_MODE_BUTTON_CTRL_ID, &button_height);
#ifndef MMI_RECORD_SUPPORT
        GUIFORM_SetChildWidth(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_height);
#else
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_SCH_RECORD_ICON, win_id);
        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildWidth(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_height);
        GUIBUTTON_SetCallBackFunc(MMIALM_EDIT_RING_BUTTON_CTRL_ID, RecordAlarmRingCallBack);
        GUIBUTTON_SetRunSheen(MMIALM_EDIT_RING_BUTTON_CTRL_ID,FALSE);
        bg.bg_type=GUI_BG_IMG;
        bg.img_id=IMG_SCH_RECORD_ICON;
        GUIBUTTON_SetPressedBg(MMIALM_EDIT_RING_BUTTON_CTRL_ID,&bg);
#endif

        
        SetTitleEditWin(win_id) ;
        AddItemSetlist();
#ifndef MMI_PDA_SUPPORT
        //GUIFORM_SetType(MMI_EDITWIN_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        SetNameParamEditWin();
        SetTimeParamEditWin();
        GUIEDIT_SetAlign(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
        SetModeParamEditWin();
        SetWeeksParamEditWin(win_id); 
        SetWeeksDisplayEditWin();
#ifdef CMCC_UI_STYLE
        SetRingDurParamEditWin();
        SetVibrateParamEditWin();
#endif

#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
		GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#endif
#endif
        if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF!= s_current_event_info.id)
        {
            #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_NAME_EDITBOX_CTRL_ID);
            #else
            MMK_SetAtvCtrl(win_id, MMI_EDITWIN_FORM_CTRL_ID);
            #endif
        }
        else
        {
            #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
            #else
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TIME_FORM_CTRL_ID);
            #endif
            GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
            GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        }
#if defined(MMI_ALARM_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)
#ifdef TOUCH_PANEL_SUPPORT
        GUIEDIT_SetIm(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,
                      ((GUIIM_TYPE_T)GUIIM_TYPE_ENGLISH|(GUIIM_TYPE_T)GUIIM_TYPE_ABC|(GUIIM_TYPE_T)GUIIM_TYPE_DIGITAL|(GUIIM_TYPE_T)GUIIM_TYPE_SMART),
                      GUIIM_TYPE_ABC);
       // GUIEDIT_SetImInput(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, (GUIIM_INPUT_TYPE_T)GUIIM_INPUT_TYPE_QWERT, (GUIIM_INPUT_TYPE_T)GUIIM_INPUT_TYPE_ALL);
		//去掉原因:控件去掉这个函数了
#endif
#endif
        break;

    case MSG_FULL_PAINT:
#ifdef SCREEN_SHAPE_CIRCULAR
        WATCHCOM_DisplayBackground(win_id);
        CTRLFORM_SetRect(MMI_EDITWIN_FORM_CTRL_ID, &form_rect);
#endif
#ifdef MMI_PDA_SUPPORT
         if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID))
         {
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
            MMI_STRING_T str={0};
            str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,s_current_event_info.event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
            str.wstr_ptr=buff;
            GUISETLIST_SetDispText(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,&str,FALSE);
         }
        SetNameParamEditWin();
        SetTimeParamEditWin();
#endif
        SetRingParamEditWin();
        break;
    case MSG_GET_FOCUS:
#ifdef SCREEN_SHAPE_CIRCULAR
        SetModeParamEditWin();
#endif
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        //EditWinMIDSKProcess(win_id);
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        HandleAlarmEditPENOKMsg(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        HandleAlarmEditPENOKMsg(win_id,ctrl_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
         ctrl_id = MMK_GetActiveCtrlId(win_id);
         HandleAlarmEditFormSwitchActiveMsg(win_id,ctrl_id);
         break;

#ifdef MMI_RECORD_SUPPORT
	case MMI_MSG_RECORDTORING:
		if(PNULL != param)
		{       
			MMI_STRING_T        ring_name = {0};
			
            ring_name.wstr_ptr = (wchar*)param;
            ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
            SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
            s_current_event_info.event.event_ring_info.dring_name_len = ring_name.wstr_len;
            MMIAPICOM_Wstrncpy(s_current_event_info.event.event_ring_info.dring_name_arr, ring_name.wstr_ptr, ring_name.wstr_len);
            SCI_MEMSET(ring_name.wstr_ptr,0,ring_name.wstr_len * sizeof(wchar));
            
            MMIAPIFMM_SplitFullPath(s_current_event_info.event.event_ring_info.dring_name_arr, s_current_event_info.event.event_ring_info.dring_name_len, PNULL, PNULL, PNULL, PNULL, ring_name.wstr_ptr, &ring_name.wstr_len);

            s_current_event_info.event.event_content_info.ring_type = ALM_RECORD_RING;
		}        
        DisplayTheRingName();
        MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
	    break;
#endif
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
		//CR177677:判断另一方是否开启，若开启，看时间是否一致，若一致，给出提示。
        if (EVENT_AUTO_POWERON == s_current_event_info.id)
		{
			const MMIACC_SMART_EVENT_FAST_TABLE_T   *poweroff_event = {0};
			MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
			poweroff_event = MMIALM_GeEvent(EVENT_AUTO_POWEROFF);
			if (poweroff_event->is_on)
			{//若开启
				MMISET_TIME_T   time = {0};
#ifdef  MMI_PDA_SUPPORT
                time.time_hour = s_current_event_info.event.event_fast_info.hour;
				time.time_minute = s_current_event_info.event.event_fast_info.minute;
#else
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
				MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else	
				GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
#endif
				if((poweroff_event->minute == time.time_minute)
					&&(poweroff_event->hour == time.time_hour))
				{
					MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
					is_need_save = FALSE;
				}
			}
		}
		else if(EVENT_AUTO_POWEROFF == s_current_event_info.id)
		{
			const MMIACC_SMART_EVENT_FAST_TABLE_T   *poweron_event = {0};
			MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
			poweron_event = MMIALM_GeEvent(EVENT_AUTO_POWERON);
			if (poweron_event->is_on)
			{//若开启
				MMISET_TIME_T   time = {0};
#ifdef  MMI_PDA_SUPPORT
                time.time_hour = s_current_event_info.event.event_fast_info.hour;
				time.time_minute = s_current_event_info.event.event_fast_info.minute;
#else				
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
				MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else	
				GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
#endif
				if((poweron_event->minute == time.time_minute)
					&&(poweron_event->hour == time.time_hour))
				{
					MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
				    is_need_save = FALSE;
				}
			}
		}
		if(is_need_save)
		{
#ifndef  MMI_PDA_SUPPORT
        //save name
        if (EVENT_AUTO_POWERON != s_current_event_info.id 
            && EVENT_AUTO_POWEROFF!= s_current_event_info.id)
        {
            if(!SaveNameEditWin())
            {
                break;
            }
        }
        //save time
        if(!SaveTimeEditWin())
        {
            break;
        }
#endif
        //save mode
#ifndef SCREEN_SHAPE_CIRCULAR
        SaveModeEditWin();
#endif
        if(s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_EVERYWEEK)
        {
            if(!SaveFreEditWin())
            {
                break;
            }
        }

#ifdef CMCC_UI_STYLE
        SaveRingDurEditWin();
        //save vibrate
        if(0 == GUISETLIST_GetCurIndex(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID))
        {
            s_current_event_info.event.event_content_info.is_vibrate = FALSE;
        }
        else
        {
            s_current_event_info.event.event_content_info.is_vibrate = TRUE;
        }
#endif

        s_current_event_info.event.event_fast_info.is_on = TRUE;
        // 关闭延时闹钟
        SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
        // 提示窗口
        //@zhaohui,cr108847
        if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
        {
#ifdef FM_SUPPORT
            if(s_current_event_info.event.event_content_info.ring_type!=ALM_FM_RING)
            {
#if 0 //#ifndef MMI_PDA_SUPPORT
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
            else
#endif
            {
 #ifdef MMI_FM_NEED_HEADSET
                MMIPUB_OpenAlertWarningWin(TXT_FM_TIMER_SAVE_PROMPT);
 #endif
            }
        }
        else
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
            MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        }   
        MMK_CloseWin(win_id);
		}
            break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
    break;

        case MSG_CLOSE_WINDOW:
        MMK_CloseWin(MMIAUTOPOWER_OPTWIN_ID);
        break;
        
    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if(MMIALM_EDIT_MODE_SETLIST_CTRL_ID==ctrl_id)
        {
            SetWeeksDisplayEditWin();
        }
    
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
/*****************************************************************************/
//  Description : handle alarm mode edit windows messages
//  Global resource dependence :
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerOnOffModeEditWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = POWERONOFF_REPEAT_LIST_CTRL_ID ;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};

            GUILIST_SetMaxItem( list_id, 3, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);

            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;

            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;

            item_data.item_content[0].item_data.text_id = TXT_ALARM_MODE_ONCE;
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ALARM_MODE_EVERYDAY;
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ALARM_MODE_WEEK;
            GUILIST_AppendItem( list_id, &item_t );

            GUILIST_SetSelectedItem( list_id, s_current_event_info.event.event_fast_info.fre_mode, TRUE );
            GUILIST_SetCurItemIndex(list_id, s_current_event_info.event.event_fast_info.fre_mode);
            MMK_SetAtvCtrl(win_id, list_id);
            break;
        }

        case MSG_FULL_PAINT:
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            //save mode
            SaveModeEditWin();
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            SaveModeEditWin();
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}
#endif
/*****************************************************************************/
//  Description : if alarm alive win is opened
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsAliveWinOpened(void)
{
    if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID))
    {
    return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : append two line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    MMI_TEXT_ID_T           txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10,
                                                TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                                TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                                TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    MMIACC_SMART_EVENT_T    smart_event = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint8 index=0;
    uint8 list_sk_index = 0;

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
#endif
//配置各项在list item中的位置，方便变动;
//对应数据:icon/txt_alarm_id/alarm_mode/event_time
#if  !defined MMI_GUI_STYLE_TYPICAL
#ifdef MMI_ALARM_MINI_SUPPORT
#if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)
    uint8 indexArray[4]= {3, 0, 2, 1};
#else
    uint8 indexArray[4]= {1, 0, 3, 2};
#endif
#else
    uint8 indexArray[4]= {1, 2, 3, 0};
#endif
#else
    uint8 indexArray[4]= {0, 1, 3, 2};
#endif
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = GetDelayEvent( event_id );
    //SCI_ASSERT(PNULL!=event_delay_ptr);  /*assert verified*/    
    if(PNULL == event_delay_ptr)
    {
        return;
    }

    MMIAPIALM_FormInteEventInfo(event_id,&smart_event);

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    if(!smart_event.event_content_info.is_add && event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
    {
        return;//通过标题来判断是否是用户新建的
    }
#endif
    
#ifdef DPHONE_SUPPORT
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;     //add this line for cr225118
#else
#if  !defined MMI_GUI_STYLE_TYPICAL
	//cr132925 
// #if defined(MMI_ALARM_MINI_SUPPORT) && defined(MMI_GUI_STYLE_MINISCREEN)
// 	item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
// #else
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL;
// #endif
#else
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT;
#endif
#endif 
#if  !defined MMI_GUI_STYLE_TYPICAL
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
#endif
    item_t.item_data_ptr = &item_data;

// #if  !defined MMI_GUI_STYLE_TYPICAL
//     index=1;
// #else
//     index=0;
// #endif
    index = indexArray[0];
    item_data.item_content[index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[index].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;

    if (smart_event.event_fast_info.is_on)
    {
            item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
            //item_t.item_state = GUIITEM_STATE_SELECTED;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
                item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
                //item_t.item_state = GUIITEM_STATE_SELECTED;
        }
    }

//    index++;
// #if  !defined MMI_GUI_STYLE_TYPICAL
//     index=2;
// #else
//     index=1;
// #endif  
    index = indexArray[1];
    if (event_id <= EVENT_ALARM_MAX)
    {
        MMI_STRING_T name_str = {0};
        MMI_GetLabelTextByLang(txt_alarm_id[event_id],&name_str);
        if ((0 == smart_event.event_content_info.w_length)
            || !MMIAPICOM_CompareTwoWstrExt(
                name_str.wstr_ptr,
                name_str.wstr_len,
                smart_event.event_content_info.w_content,
                smart_event.event_content_info.w_length,
                TRUE)
            )
        {
            item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[index].item_data.text_id = txt_alarm_id[event_id];
        }
        else
        {
                item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[index].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;
                
                item_data.item_content[index].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
//              MMI_WSTRNCPY(item_data.item_content[1].item_data.text_buffer.wstr,
//                     item_data.item_content[1].item_data.text_buffer.wstr_len,
//                     smart_event.event_content_info.w_content,
//                     smart_event.event_content_info.w_length,
//                     smart_event.event_content_info.w_length);
        }
        
    }
    else 
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_1;
    }

    index = indexArray[2];
#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
    if(TXT_ALARM_MODE_WEEK!=text_id_2)
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_2; 
    }
    else
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPIALARM_GetFreModeString(smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
        item_data.item_content[index].item_data.text_buffer.wstr_ptr = buff;
    }
#else
    item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[index].item_data.text_id = text_id_2;
#endif
// #if  !defined MMI_GUI_STYLE_TYPICAL
//     index=0;
// #else
//     index=2;
// #endif
    index = indexArray[3];
    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    MMIAPIALARM_GetEventTimeString( &smart_event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
    
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = temp_wstr;
    item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    if(event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
    {
        item_data.softkey_id[0] = STXT_OPTION;
        list_sk_index = 1;
    }
    else
    {
        list_sk_index = 0;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
        item_data.softkey_id[1] = TXT_EDIT;
#endif
    }
        
#else
    list_sk_index = 0;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
    item_data.softkey_id[1] = TXT_EDIT;
#endif
#endif
    item_data.softkey_id[list_sk_index] = TXT_COMMON_ENABLE_ON;

    if (smart_event.event_fast_info.is_on )
    {
        item_data.softkey_id[list_sk_index] = TXT_CLOSE;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
            item_data.softkey_id[list_sk_index] = TXT_CLOSE;
        }
    }
    item_data.softkey_id[2] = STXT_RETURN;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
/*****************************************************************************/
//  Description : append three line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendThreeLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    MMI_TEXT_ID_T           txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10,
                                                TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                                TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                                TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    MMIACC_SMART_EVENT_T    smart_event = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint8 index=0;
    uint8 list_sk_index = 0;
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = GetDelayEvent( event_id );
    //SCI_ASSERT(PNULL!=event_delay_ptr);  /*assert verified*/    
    if(PNULL == event_delay_ptr)
    {
        return;
    }

    MMIAPIALM_FormInteEventInfo(event_id,&smart_event);

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    if(!smart_event.event_content_info.is_add && event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
    {
        return;//通过标题来判断是否是用户新建的
    }
#endif
    
    item_t.item_style    = GUIITEM_STYLE_THREE_LINE_TEXT_ICON;

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    if (event_id <= EVENT_ALARM_MAX)
    {
        MMI_STRING_T name_str = {0};
        MMI_GetLabelTextByLang(txt_alarm_id[event_id],&name_str);
        if (!MMIAPICOM_CompareTwoWstrExt(
            name_str.wstr_ptr,
            name_str.wstr_len,
            smart_event.event_content_info.w_content,
            smart_event.event_content_info.w_length,
            TRUE)
            ||(0 == smart_event.event_content_info.w_length))
        {
            item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[index].item_data.text_id = txt_alarm_id[event_id];
        }
        else
        {
                item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[index].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;
                
                item_data.item_content[index].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
//              MMI_WSTRNCPY(item_data.item_content[1].item_data.text_buffer.wstr,
//                     item_data.item_content[1].item_data.text_buffer.wstr_len,
//                     smart_event.event_content_info.w_content,
//                     smart_event.event_content_info.w_length,
//                     smart_event.event_content_info.w_length);
        }
        
    }
    else 
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_1;
    }

    index++;

    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    MMIAPIALARM_GetEventTimeString( &smart_event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
    
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = temp_wstr;
    item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);

    index++;

    if(TXT_ALARM_MODE_WEEK!=text_id_2)
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_2; 
    }
    else
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPIALARM_GetFreModeString(smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
        item_data.item_content[index].item_data.text_buffer.wstr_ptr = buff;
    }

    index++;

    item_data.item_content[index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[index].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;

    if (smart_event.event_fast_info.is_on)
    {
            item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
            //item_t.item_state = GUIITEM_STATE_SELECTED;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
                item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
                //item_t.item_state = GUIITEM_STATE_SELECTED;
        }
    }
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    list_sk_index = 1;
#else
    list_sk_index = 0;
#endif

    item_data.softkey_id[list_sk_index] = TXT_COMMON_ENABLE_ON;
    if (smart_event.event_fast_info.is_on )
    {
        item_data.softkey_id[list_sk_index] = TXT_CLOSE;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
            item_data.softkey_id[list_sk_index] = TXT_CLOSE;
        }
    }

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    item_data.softkey_id[0] = STXT_OPTION;
#else
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
    item_data.softkey_id[1] = TXT_EDIT;
#endif
#endif
    item_data.softkey_id[2] = STXT_RETURN;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
#endif

/*****************************************************************************/
//  Description : get event timer string
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALARM_GetEventTimeString(
                              const MMIACC_SMART_EVENT_FAST_TABLE_T* event_ptr,
                              BOOLEAN                     is_valid,
                              wchar*                      str_ptr,
                              uint16                      str_len
                              )
{
    uint8 * tmp_str = PNULL;
    uint16 tmp_length = 0;
//     MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_12HOURS;
// 
//     time_type = MMIAPISET_GetTimeDisplayType();

    //SCI_PASSERT(PNULL != str_ptr && ALM_EVENT_TIME_STR_LEN <= str_len, ("MMIAPIALARM_GetEventTimeString: str_len is %d",str_len));/*assert verified*/
    if(PNULL == str_ptr || ALM_EVENT_TIME_STR_LEN > str_len)
    {
        //SCI_TRACE_LOW:"MMIAPIALARM_GetEventTimeString:PNULL == str_ptr || ALM_EVENT_TIME_STR_LEN > str_len"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9249_112_2_18_2_1_9_45,(uint8*)"");
        return;
    }
    
    tmp_length = str_len;
    tmp_str = SCI_ALLOC_APP(tmp_length);
    if(PNULL == tmp_str)
    {
        return;
    }
    SCI_MEMSET(tmp_str,0,tmp_length);

    if ( is_valid )
    {
        MMIAPISET_FormatTimeStrByTime(event_ptr->hour,event_ptr->minute, tmp_str,MMIALM_TIME_STR_12HOURS_LEN+1);
    }
    else
    {
        SCI_TIME_T   sys_time = {0};

        TM_GetSysTime( &sys_time );
        MMIAPISET_FormatTimeStrByTime(sys_time.hour,sys_time.min, tmp_str,MMIALM_TIME_STR_12HOURS_LEN+1);
    }
    MMI_STRNTOWSTR(str_ptr,str_len, (char*)tmp_str,tmp_length,tmp_length);/*lint !e64*/

    SCI_FREE(tmp_str);
    tmp_str = PNULL;
}

/*****************************************************************************/
//  Description : close delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarm(
                            uint16 event_id, 
                            MMIACC_SMART_EVENT_T event,
                            BOOLEAN    is_on
                            )
{
    BOOLEAN    result   = TRUE;
    uint16 delay_event_id = FALSE;
    MMIALMSVC_ID_T temp_service_id = 0;

    /*switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        delay_event_id = EVENT_ALARM_DELAY_0;
        event_id = EVENT_ALARM_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        delay_event_id = EVENT_ALARM_DELAY_1;
        event_id = EVENT_ALARM_1;
        break;
        
    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        delay_event_id = EVENT_ALARM_DELAY_2;
        event_id = EVENT_ALARM_2;
        break;
        
    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        delay_event_id = EVENT_ALARM_DELAY_3;
        event_id = EVENT_ALARM_3;
        break;
        
    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        delay_event_id = EVENT_ALARM_DELAY_4;
        event_id = EVENT_ALARM_4;
        break;
        
    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        delay_event_id = EVENT_ALARM_DELAY_5;
        event_id = EVENT_ALARM_5;
        break;
        
    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        delay_event_id = EVENT_ALARM_DELAY_6;
        event_id = EVENT_ALARM_6;
        break;
        
    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        delay_event_id = EVENT_ALARM_DELAY_7;
        event_id = EVENT_ALARM_7;
        break;
        
    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        delay_event_id = EVENT_ALARM_DELAY_8;
        event_id = EVENT_ALARM_8;
        break;
        
    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        delay_event_id = EVENT_ALARM_DELAY_9;
        event_id = EVENT_ALARM_9;
        break;

    default:
        return FALSE;
        //break;
    }*/
    if(event_id<=EVENT_ALARM_MAX) //&&event_id>=EVENT_ALARM_0 /*lint !e685 !e568*/
    {
        delay_event_id = event_id+EVENT_ALARM_DELAY_0;
        event_id = event_id;
    }
    else if(event_id>=EVENT_ALARM_DELAY_0&&event_id<=EVENT_ALARM_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id-EVENT_ALARM_DELAY_0; 
    }
    else
    {
        return FALSE;
    }

    event.event_fast_info.is_on = is_on;
    event.event_fast_info.is_valid = FALSE; 

    temp_service_id = s_event_fast_table[delay_event_id].service_id;
    SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
    s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id独立对应与自己的event_id

    if ((!is_on)&&(ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode))
    {
        s_event_fast_table[event_id].is_on = FALSE;//关闭延时闹钟的同时,将该延时的原闹钟也关闭
    }
    // 保存
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_event_id, &event.event_content_info);  

    /*if (ALM_DOWNLOAD_RING == event.event_content_info.ring_type
        ||ALM_FM_RING == event.event_content_info.ring_type)
    {
        SaveRingInfoToFile(delay_event_id,&event.event_ring_info);  
    } */   

    return result;
}

#ifdef ZDT_PLAT_YX_SUPPORT
static BOOLEAN g_is_alarm_win_is_open = FALSE;
BOOLEAN MMIALARM_IsActiveWin(void)
{
    return g_is_alarm_win_is_open;
}

BOOLEAN SetDelayAlarmExt(
                            uint16 event_id,
                            MMIACC_SMART_EVENT_T *event_ptr,
                            BOOLEAN    is_on
                            )
{
    return SetDelayAlarm(event_id,*event_ptr,is_on);
}
#endif
/*****************************************************************************/
//  Description : Set alarm ring by file full path name.
//  Global resource dependence : 
//  Author: xingdong.li
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_SetAlarmRing(
                                     uint16 alarm_id,
                                     const wchar *full_path_ptr,
                                     uint16 full_path_len        //len in WORD
                                     )
{
    MMIACC_SMART_EVENT_RING_T       event_ring = {0};
    uint16                          nv_return = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    uint16                          delay_alarm_id = alarm_id + MMIACC_ALARM_NUM;
    BOOLEAN                         result = TRUE;  
    
    //SCI_PASSERT(alarm_id <= EVENT_ALARM_MAX && PNULL != full_path_ptr && 0 != full_path_len,("MMIAPIALARM_SetAlarmRing: wrong param"));/*assert verified*/
    if(alarm_id > EVENT_ALARM_MAX || PNULL == full_path_ptr || 0 == full_path_len)
    {
        //SCI_TRACE_LOW:"MMIAPIALARM_SetAlarmRing:alarm_id > EVENT_ALARM_MAX || PNULL == full_path_ptr || 0 == full_path_len!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9419_112_2_18_2_1_9_46,(uint8*)"");
        return FALSE;
    }

    ReadRingInfoFileName(alarm_id,&event_ring);    
    SCI_MEMSET(event_ring.dring_name_arr, 0, sizeof(event_ring.dring_name_arr));
    event_ring.dring_name_len = full_path_len;
    MMI_WSTRNCPY(
        event_ring.dring_name_arr, sizeof(event_ring.dring_name_arr),
        full_path_ptr, full_path_len,
        full_path_len
        );
    result = SaveRingInfoToFile(alarm_id,&event_ring);

    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+alarm_id, &event_content, nv_return);
    event_content.ring_type = ALM_DOWNLOAD_RING;    
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+alarm_id, &event_content);

    if (result
        && s_event_fast_table[delay_alarm_id].is_on 
        && s_event_fast_table[delay_alarm_id].is_valid)
    {
        ReadRingInfoFileName(delay_alarm_id,&event_ring);    
        SCI_MEMSET(event_ring.dring_name_arr, 0, sizeof(event_ring.dring_name_arr));
        event_ring.dring_name_len = full_path_len;
        MMI_WSTRNCPY(
            event_ring.dring_name_arr, sizeof(event_ring.dring_name_arr),
            full_path_ptr, full_path_len,
            full_path_len
            );
        result = SaveRingInfoToFile(delay_alarm_id,&event_ring);

        MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_alarm_id, &event_content, nv_return);
        event_content.ring_type = ALM_DOWNLOAD_RING;    
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_alarm_id, &event_content);    
    }

    if(MMK_IsOpenWin(MMIALM_EDIT_WIN_ID)
//#ifdef MMI_RECORD_SUPPORT        
//        &&MMIAPIRECORD_IsMainWinOpened()  //暂放，待合入
//#endif 
        )
    {
        if(MMK_IsOpenWin(MMIALM_RINGLIST_WIN_ID))
		{
			MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
		}
        if(s_current_event_info.id == alarm_id)
        {
            MMI_STRING_T        ring_name = {0};
			
            ring_name.wstr_ptr = (wchar*)full_path_ptr;
            ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
            SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
            s_current_event_info.event.event_ring_info.dring_name_len = ring_name.wstr_len;
            MMIAPICOM_Wstrncpy(s_current_event_info.event.event_ring_info.dring_name_arr, ring_name.wstr_ptr, ring_name.wstr_len);
            SCI_MEMSET(ring_name.wstr_ptr,0,ring_name.wstr_len * sizeof(wchar));
            
            MMIAPIFMM_SplitFullPath(s_current_event_info.event.event_ring_info.dring_name_arr, s_current_event_info.event.event_ring_info.dring_name_len, PNULL, PNULL, PNULL, PNULL, ring_name.wstr_ptr, &ring_name.wstr_len);
//#ifdef MMI_RECORD_SUPPORT 
            //s_current_event_info.event.event_content_info.ring_type = ALM_RECORD_RING;
//#endif
        }       
    }

    return result;
}

/*****************************************************************************/
//  Description : 检查并启动过期事件(常在闹钟,自动开机等事件结束时调用)
//  Global resource dependence : 
//  Author: zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void CheckandStartExpiredEvent( void )
{
//cr137019
    if(s_bypast_list.total > 0)
    {
        uint16 event_id = s_bypast_list.entry_id[0];

        DeleteOneSchEventInList(0, &s_bypast_list);

        OpenPastSchEventWin(WIN_ONE_LEVEL, event_id);
    }
    else if((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE) && !MMIAPIMMS_IsSendingOrRecving())
    {
        MMI_MEMCPY(
            (void*)&s_arrived_event, sizeof(MMIACC_SMART_EVENT_T),
            (void*)&s_poweroff_event, sizeof(MMIACC_SMART_EVENT_T),
            sizeof(MMIACC_SMART_EVENT_T));
        s_arrived_event_id = EVENT_AUTO_POWEROFF;
        OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
        SCI_MEMSET(&s_poweroff_event, 0, sizeof(MMIACC_SMART_EVENT_T));
    }
    else if ( STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
    {
        MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
    }
}

/*save event ring info to fs*/
/*****************************************************************************/
// Description : 创建alarm目录
// Global resource dependence :
// Author:zhaohui
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitAlarmFolder(void)
{
    wchar            full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16           full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar            *file_dev_ptr = PNULL;
    uint16           file_dev_len = 0;
    BOOLEAN          result = FALSE;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
    
    dev = MMIAPIFMM_GetFirstValidDevice();
    file_dev_ptr = MMIAPIFMM_GetDevicePath(dev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(dev);
    if((PNULL == file_dev_ptr) || (0 == file_dev_len))
    {
        return FALSE;
    }
    
    if(MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
        MMIMULTIM_DIR_ALARM, MMIMULTIM_DIR_ALARM_LEN, 
        NULL, 0, 
        full_path_name, &full_path_len))
    {
        if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            result = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
        else
        {
            result = TRUE;
        }
    }
    //SCI_TRACE_LOW:"InitAlarmFolder result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9527_112_2_18_2_1_10_47,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 根据event_id映射成对应该记录的ring信息
//  Global resource
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetRingInfoFileName(
                                     uint16 event_id,  
                                     wchar *u16filename_ptr,//OUT
                                     uint16 u16filename_max_len
                                     )
{
    BOOLEAN         ret = FALSE;
    uint8           u8filename[MMIALM_RING_INFO_FILE_NAME_LEN +1] = {0};
    wchar           wstr_filename[MMIALM_RING_INFO_FILE_NAME_LEN +1] = {0};
    uint32          u32index = event_id;//(group_id << 16) + entry_id;
//  FILE_DEV_E_T    file_dev = FS_UDISK;
    uint16          full_path_len = u16filename_max_len;
    uint16          wstr_len  = 0;
    wchar           *device_name_ptr = PNULL;
    uint16           device_name_len = 0;   
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
   
    if(PNULL != u16filename_ptr)
    {
        //ring id informatio file name
        sprintf( (char*)u8filename,"%ld%s",u32index,MMIALM_RING_INFO_FILE_SUFFIX);
        
        dev = MMIAPIFMM_GetFirstValidDevice();
        device_name_ptr = MMIAPIFMM_GetDevicePath(dev);
        device_name_len = MMIAPIFMM_GetDevicePathLen(dev);
        
        if((PNULL == device_name_ptr) || (0 == device_name_len))
        {
            return FALSE;
        }
    
        wstr_len = MIN(strlen( (char*)u8filename), MMIALM_RING_INFO_FILE_NAME_LEN); /*lint !e666*/
        MMI_STRNTOWSTR(wstr_filename, MMIALM_RING_INFO_FILE_NAME_LEN, (char*)u8filename, MMIALM_RING_INFO_FILE_NAME_LEN, wstr_len);/*lint !e64*/
        if (!MMIAPIFMM_CombineFullPath(
                                device_name_ptr,
                                device_name_len,
                                MMIMULTIM_DIR_ALARM,
                                MMIMULTIM_DIR_ALARM_LEN,
                                wstr_filename,
                                wstr_len,
                                (wchar *)u16filename_ptr,  
                                &full_path_len
                              )
        )
        {
            //SCI_TRACE_LOW:"mmialarm.c GetRingInfoFileName MMIAPIFMM_CombineFullPath fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9580_112_2_18_2_1_10_48,(uint8*)"");
            return ret;
        }

        if(full_path_len <= MMIFILE_FILE_NAME_MAX_LEN)
        {
            u16filename_ptr[full_path_len] = 0;
        }

        ret = TRUE;
    }
    //SCI_TRACE_LOW:"GetRingInfoFileName event_id %d, ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9591_112_2_18_2_1_10_49,(uint8*)"dd",event_id, ret);
    return ret;
    
}

/*****************************************************************************/
//  Description : 把ring_ptr保存到根据event_id映射成的铃声id信息文件中 
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN SaveRingInfoToFile(
                                           uint16 event_id,
                                           const MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                           )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint32 write_size = 0;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(PNULL != ring_ptr)
    {
        if(GetRingInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            if(InitAlarmFolder())
            {
                //打开文件
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_WriteFile(
                        file_handle, 
                        ring_ptr, 
                        sizeof(MMIACC_SMART_EVENT_RING_T), 
                        &write_size, 
                        NULL
                        );
                    //SCI_TRACE_LOW:"MMIALM_SaveRingIDInfoToFile file_ret %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9630_112_2_18_2_1_10_50,(uint8*)"d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }

            }
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : 在铃声id信息文件中读取铃声id的路径信息  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN ReadRingInfoFileName(
                                        uint16 event_id,
                                        MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                        )
{
    BOOLEAN ret = FALSE;
    wchar u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 read_size = 0;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(PNULL != ring_ptr)
    {
        if(GetRingInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
            
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_ret =  MMIAPIFMM_ReadFile(
                    file_handle, 
                    ring_ptr, 
                    sizeof(MMIACC_SMART_EVENT_RING_T), 
                    &read_size, 
                    NULL
                    );
                //SCI_TRACE_LOW:"ReadRingInfoFileName file_ret %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9676_112_2_18_2_1_10_51,(uint8*)"d",file_ret);
                if(SFS_ERROR_NONE == file_ret && (read_size > 0) /*&& (ring_ptr->file_name_len > 0) && (ring_ptr->file_name_len <= MMIFILE_FILE_NAME_MAX_LEN)*/)
                {
                    ret = TRUE;
                }
                MMIAPIFMM_CloseFile(file_handle);
            }
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : 删除铃声id信息或铃声id文件  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteRingInfoFile(
                                    uint16 event_id                     
                )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(GetRingInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN +1)))
    {
        file_ret = MMIAPIFMM_DeleteFile(u16filename, PNULL);
        //SCI_TRACE_LOW:"DeleteRingInfoFile file_ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9704_112_2_18_2_1_10_52,(uint8*)"d",file_ret);
        if(SFS_NO_ERROR == file_ret)
        {
            ret = TRUE;
        }
    }

    return ret; 
}

/*****************************************************************************/
//  Description : get the integrity event info by event_id
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIALM_FormInteEventInfo(
                            uint16 event_id,//in
                            MMIACC_SMART_EVENT_T* integrity_event_ptr//out
                            )//得到完整的事件信息
{
    MMIACC_SMART_EVENT_CONTENT_T*   event_content_ptr = PNULL;
    MMIACC_SMART_EVENT_RING_T*      event_ring_ptr = PNULL;
    uint16                  nv_return = 0;

    //SCI_ASSERT(PNULL != integrity_event_ptr); /*assert verified*/
    if(PNULL == integrity_event_ptr)
    {
        return;
    }
    event_content_ptr = SCI_ALLOC_APPZ(sizeof(MMIACC_SMART_EVENT_CONTENT_T));
    //SCI_ASSERT(PNULL != event_content_ptr);/*assert verified*/
    SCI_MEMSET( event_content_ptr, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T) );

    event_ring_ptr = SCI_ALLOC_APPZ(sizeof(MMIACC_SMART_EVENT_RING_T));
    //SCI_ASSERT(PNULL != event_ring_ptr);/*assert verified*/
    SCI_MEMSET( event_ring_ptr, 0, sizeof(MMIACC_SMART_EVENT_RING_T) );

    SCI_MEMCPY((uint8 *)&integrity_event_ptr->event_fast_info, 
               (uint8 *)MMIALM_GeEvent((uint16)(EVENT_ALARM_0+event_id)), 
               sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, event_content_ptr, nv_return);
    SCI_MEMCPY((uint8 *)&integrity_event_ptr->event_content_info, 
               (uint8 *)event_content_ptr, 
               sizeof(MMIACC_SMART_EVENT_CONTENT_T));   

//     if (ReadRingInfoFileName(event_id,event_ring_ptr))
//    {
//        SCI_MEMCPY((uint8 *)&integrity_event_ptr->event_ring_info, 
//                   (uint8 *)event_ring_ptr, 
//                   sizeof(MMIACC_SMART_EVENT_RING_T));
//    }
//    else
//    {//若存储器不存在,或无法读取,则
//        if (ALM_DOWNLOAD_RING == integrity_event_ptr->event_content_info.ring_type)
//        {
//            integrity_event_ptr->event_content_info.ring_type = ALM_FIX_RING;
//            integrity_event_ptr->event_content_info.ring = 1;
//            MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &integrity_event_ptr->event_content_info);
//        }
//     }

    SCI_FREE(event_content_ptr);
    event_content_ptr = PNULL;

    SCI_FREE(event_ring_ptr);
    event_ring_ptr = PNULL;
}

/*****************************************************************************/
//  Description : Save the integrity event info 
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_SaveInteEventInfo(
                            uint16 event_id,//in
                            MMIACC_SMART_EVENT_T* integrity_event_ptr//in
                            )
{
    BOOLEAN   result = FALSE;

    //SCI_ASSERT(PNULL != integrity_event_ptr); /*assert verified*/   
    if(PNULL == integrity_event_ptr)
    {
        return FALSE;
    }
    s_event_fast_table[event_id].is_valid = TRUE;
    result = MMIALM_ModifyEvent(event_id, &integrity_event_ptr->event_fast_info);
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &integrity_event_ptr->event_content_info);  

    if (ALM_DOWNLOAD_RING == integrity_event_ptr->event_content_info.ring_type
        ||ALM_RECORD_RING == integrity_event_ptr->event_content_info.ring_type
#ifdef FM_SUPPORT
        ||ALM_FM_RING == integrity_event_ptr->event_content_info.ring_type
#endif
        )
    {
		if (!MMIAPIUDISK_UdiskIsRun())
		{
			result = SaveRingInfoToFile(event_id,&integrity_event_ptr->event_ring_info); 
		}   
    }    
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
    MMIVCAL_UpdatePimStatus(event_id-EVENT_SCH_0+1,VCAL_RECENT_REPLACE);
#endif /* HS_PIM_VCAL */

    return result;
}



/*****************************************************************************/
//  Description : get total times of the arrived alarm and the arrived calender 
//  Global resource dependence : 
//  Author:      jianhui.luo
//  Note:            
/*****************************************************************************/
PUBLIC uint32 MMIAPIALM_GetTotalTimesOfArrivedEvent(void)
{
    /*if(MMIAPIALM_IsAliveWinOpened())
    {  
        return 1;
    }
    else
    {
         return 0;
    } */
#ifdef CALENDAR_SUPPORT
    return MMIAPISCH_GetTotalTimesOfUnreadEvent();
#endif
}

/*****************************************************************************/
//  Description :interrupt alarm delete 
//  Global resource dependence : 
//  Author:      
//  Note:            
/*****************************************************************************/
PUBLIC void MMIALM_SetAlmInterruptFlag(BOOLEAN  flag)
{
    s_alarm_is_interrupted = flag;
    if(flag)
    {
        //interrup delete, close waitting win
        MMK_CloseWin(MMISCH_DELETE_ANIM_WIN_ID);
    }
    //SCI_TRACE_LOW:"[MMIALM] MMIALM_Interrupt s_alarm_is_interrupted %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9834_112_2_18_2_1_10_53,(uint8*)"d", s_alarm_is_interrupted);
}

/*****************************************************************************/
//  Description :interrupt alarm delete 
//  Global resource dependence : 
//  Author:      
//  Note:            
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_GetAlmInterruptFlag(void)
{
    //SCI_TRACE_LOW:"[MMIALM] MMIALM_GetAlmInterruptFlag s_alarm_is_interrupted %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9843_112_2_18_2_1_10_54,(uint8*)"d", s_alarm_is_interrupted);
    return s_alarm_is_interrupted;
}

/*****************************************************************************/
// Description : get ring info
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_GetRingInfo(uint16 event_id, MMIACC_SMART_EVENT_T *event_info_ptr)
{
    BOOLEAN result=TRUE;
    if(PNULL == event_info_ptr)
    {
        return result;
    }
    if(ALM_DOWNLOAD_RING == event_info_ptr->event_content_info.ring_type
        ||ALM_RECORD_RING == event_info_ptr->event_content_info.ring_type
#ifdef FM_SUPPORT
        ||ALM_FM_RING == event_info_ptr->event_content_info.ring_type
#endif
        )
    {
        if(event_id>=EVENT_ALARM_DELAY_0&&event_id<=EVENT_ALARM_DELAY_MAX)
        {
            event_id=event_id-MMIACC_ALARM_NUM;
        }
        if (!ReadRingInfoFileName(event_id, &event_info_ptr->event_ring_info))
        {//若存储器不存在,或无法读取,则
            //event_info_ptr->event_content_info.ring_type = ALM_FIX_RING;
            //event_info_ptr->event_content_info.ring = 1;
            //MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &event_info_ptr->event_content_info);
            result=FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : display Alarm window
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAlarmWin(
                          MMI_WIN_ID_T win_id, 
                          MMIPUB_INFO_T    *win_info_ptr
                          )
{
    MMIPUB_HandleWaitWinMsg(win_id, MSG_FULL_PAINT, PNULL);
//     GUI_POINT_T         dis_point = {0};
//     uint16          image_width = 0;
//     uint16          image_height = 0;
//     GUI_RECT_T      disp_rect = {0};
//     GUI_RECT_T      image_rect = {0};
// 
// 
//     //display wait win background image
//     dis_point.x = win_info_ptr->pubwin_theme.bg_display_rect.left;
//     dis_point.y = win_info_ptr->pubwin_theme.bg_display_rect.top;
//     GUIRES_DisplayImg(&dis_point,
//         PNULL,
//         PNULL,
//         win_info_ptr->win_id,
//         win_info_ptr->pubwin_theme.bg_id,
//         &win_info_ptr->pubwin_theme.bg_lcd_dev);
// 
//     IMG_EnableTransparentColor(TRUE);
//     if (0 != win_info_ptr->pubwin_theme.anim_bg_image_id)
//     {
//         //display wait win background image
//         dis_point.x = win_info_ptr->pubwin_theme.anim_bg_display_rect.left;
//         dis_point.y = win_info_ptr->pubwin_theme.anim_bg_display_rect.top;
//         GUIRES_DisplayImg(&dis_point,
//             PNULL,
//             PNULL,
//             win_info_ptr->win_id,
//             win_info_ptr->pubwin_theme.anim_bg_image_id,
//             &win_info_ptr->pubwin_theme.bg_lcd_dev);
//     }
// 
//     if (0 != win_info_ptr->prompt_image_id)
//     {
//         //display alert image
//         dis_point.x = win_info_ptr->pubwin_theme.prompt_display_rect.left;
//         dis_point.y = win_info_ptr->pubwin_theme.prompt_display_rect.top;
//         GUIRES_DisplayImg(&dis_point,
//             PNULL,
//             PNULL,
//             win_info_ptr->win_id,
//             win_info_ptr->prompt_image_id,
//             &win_info_ptr->pubwin_theme.bg_lcd_dev);
//     }
// 
//     IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : past auto power on prompt window
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastAutoPowerOn(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:  
          // 开启窗口定时器
          StartAlarmTimer(&s_3seconds_timer_id, MMI_3SECONDS, FALSE);
          //GUIWIN_SetSoftkeyTextId(MMIALM_BYPAST_SCH_WIN_ID,  STXT_OK, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE);  
          recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
          MMIDEFAULT_TurnOnBackLight(); 
          break;

        case MSG_APP_OK:
        case MSG_CTL_OK :
            break;
        case MSG_KEYLONG_RED:
            //SCI_TRACE_LOW:"mmialarm.c: HandlePastAutoPowerOn() MSG_CTL_OK "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9961_112_2_18_2_1_11_55,(uint8*)"");
            MMK_CloseWin(win_id);
            if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
            {
               MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
            }
            MMIAPIPHONE_PowerReset();
            break;
                            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            /*MMIDEFAULT_TurnOffBackLight();

             POWER_PowerOff();*/
            break;
        
        /*case MSG_KEYLONG_RED:
            MMK_CloseWin(win_id);

            MMIAPIPHONE_PowerReset();
            break;*/

       case MSG_TIMER:                  
            if (s_3seconds_timer_id == *((uint8 *) param))
            {
                GUIWIN_UpdateTitle(win_id);
                MMK_CloseWin(win_id);

                if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
                {
                   MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
                }
                
                MMK_StopTimer(s_3seconds_timer_id);
                s_3seconds_timer_id = 0;
                
                MMIAPIPHONE_PowerReset();
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
           break;

        case MSG_FULL_PAINT:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            break;
            
        case MSG_CLOSE_WINDOW:
            if (0 != s_3seconds_timer_id)
            {
                MMK_StopTimer(s_3seconds_timer_id);
                s_3seconds_timer_id = 0;
            }
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            break;

        default:
            if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
                 (MSG_KEYUP_FLIP == msg_id ))//|| MSG_CTL_OK == msg_id))
            {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
                if (MSG_KEYUP_FLIP == msg_id)
                {
                    MMIDEFAULT_HandleFlipKey(TRUE);  
                }
#endif                
            }else
            {
                recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            }
            break;
    }    

    return recode;
}

/*****************************************************************************/
//  Description : handle alarm edit windows penok messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditPENOKMsg(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id
                                          )
{
    switch(ctrl_id)
    {
        case MMIALM_EDIT_RING_TYPE_CTRL_ID:
            MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);
            break;
#ifdef MMI_PDA_SUPPORT
        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
            break;
        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_APP_CANCEL,  PNULL);
            break;
        case MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubEditWin((uint32 *)MMIALM_NAMEWIN_TAB, PNULL);
            break;
        case MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID:
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
            {
                SCI_TIME_T      time_info = {0};
                if (s_current_event_info.event.event_fast_info.is_valid )
                {
                    time_info.hour=s_current_event_info.event.event_fast_info.hour;
                    time_info.min = s_current_event_info.event.event_fast_info.minute;
                }
                else
                {
        	        TM_GetSysTime(&time_info);
                    
                }
                MMIAPIALARM_OpenAlmClockWin(time_info, SaveTimeEditWin);
            }
          // MMK_CreateWin((uint32*)MMIALM_JUMP_CLOCK_TAB, PNULL);
           
#else
			MMK_CreatePubFormWin((uint32 *)MMIALM_TIMEWIN_TAB, PNULL);
#endif
            break;
#endif
        case MMI_EDITWIN_BUTTON0_CTRL_ID:
        case MMI_EDITWIN_BUTTON1_CTRL_ID:
        case MMI_EDITWIN_BUTTON2_CTRL_ID:
        case MMI_EDITWIN_BUTTON3_CTRL_ID:
        case MMI_EDITWIN_BUTTON4_CTRL_ID:
        case MMI_EDITWIN_BUTTON5_CTRL_ID:
        case MMI_EDITWIN_BUTTON6_CTRL_ID:
            if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
            {
                s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
            }
            else
            {
                s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
            }
#ifndef MMI_PDA_SUPPORT
            setAlarmEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
            SetEditWinButtonBg(win_id,ctrl_id);
#ifdef MMI_PDA_SUPPORT
         SaveFreEditWin();
         if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID))
         {
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
            MMI_STRING_T str={0};
            str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,s_current_event_info.event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
            str.wstr_ptr=buff;
            GUISETLIST_SetDispText(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,&str,TRUE);
         }
#endif
            break;
#ifdef SCREEN_SHAPE_CIRCULAR
        case MMIALM_EDIT_MODE_SELECT_LABEL_CTRL_ID:
        {
            MMK_CreateWin((uint32 *)POWERONOFF_MODE_EDIT_WIN, PNULL);
        }
#endif
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : handle alarm edit windows FormSwitchActive messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditFormSwitchActiveMsg(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id
                                          )
{
    switch(ctrl_id)
    {
        case MMIALM_EDIT_RING_TYPE_CTRL_ID:
        case MMIALM_EDIT_MODE_SETLIST_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id,  IMAGE_COMMON_SOFTKEY_SELECTED_ICON, 1, TRUE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_SELECT, STXT_RETURN, TRUE);
#endif
#endif
            break;
        case MMI_EDITWIN_BUTTON0_CTRL_ID:
        case MMI_EDITWIN_BUTTON1_CTRL_ID:
        case MMI_EDITWIN_BUTTON2_CTRL_ID:
        case MMI_EDITWIN_BUTTON3_CTRL_ID:
        case MMI_EDITWIN_BUTTON4_CTRL_ID:
        case MMI_EDITWIN_BUTTON5_CTRL_ID:
        case MMI_EDITWIN_BUTTON6_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
            setAlarmEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
            SetEditWinButtonBg(win_id,ctrl_id);
            break;
        case MMIALM_EDIT_NAME_EDITBOX_CTRL_ID:
            break;
        default:
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
#endif
#endif
            SetEditWinButtonBg(win_id,ctrl_id);
            break;
    }
}

/*****************************************************************************/
//  Description : Set EditWin Button bg
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id)
{
    uint32 i=0;
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};

    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif
    bg.bg_type=GUI_BG_IMG;
    for(i=0;i<ALM_WEEK_DAY_NUM;i++)
    {
        if(s_weeks_button_is_selected[i])
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;            
            GUIBUTTON_SetFont(MMI_EDITWIN_BUTTON0_CTRL_ID+i, &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;            
            GUIBUTTON_SetFont(MMI_EDITWIN_BUTTON0_CTRL_ID+i, &disabled_font);
        }
#ifndef MMI_PDA_SUPPORT
        if(ctrl_id==MMI_EDITWIN_BUTTON0_CTRL_ID+i)
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        }
#endif
        GUIBUTTON_SetBg(MMI_EDITWIN_BUTTON0_CTRL_ID+i, &bg);
        GUIBUTTON_Update(MMI_EDITWIN_BUTTON0_CTRL_ID+i);
    }
    
}

/*****************************************************************************/
//  Description : save fre
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveFreEditWin(void)
{
//  uint16      cur_num       =   0;
    uint8   week_day =0;
    int32 loop=0;
//  uint32      i = 0; 
    uint16      mode = 0;
    
    for(loop=0;loop<ALM_WEEK_DAY_NUM;loop++)
    {
        week_day = MMIAPISET_GetWeekDayByOffset(loop);
        if(s_weeks_button_is_selected[loop])
        {
            mode |= s_week_mask[week_day];
        }
    }

    if (mode != s_current_event_info.event.event_fast_info.fre) 
    {
        s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
    }

    if(0==s_current_event_info.event.event_fast_info.fre)
    {
        MMIPUB_OpenAlertWarningWin(TXT_ALARM_SELECT_ONE_DAY);
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32 ctrl_id=MMI_EDITWIN_BUTTON0_CTRL_ID;
    GUI_RECT_T rect={0};//max_rect与owner_draw_ptr->display_rect相交区域，也即图片最后显示区域
    GUI_RECT_T max_rect={0};//图片可以显示的最大区域
    GUI_RECT_T img_rect={0};//图片裁剪区
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T    win_id=0;
    uint16 image_w=0;
    uint16 iamge_h=0;

    ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
    win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id=MMK_GetWinId(win_handle);
    GUIRES_GetImgWidthHeight(&image_w, &iamge_h, IMAGE_CLOCK_ALARM_WEEK_MARK, win_id);

    max_rect.left=owner_draw_ptr->display_x;
    max_rect.top=owner_draw_ptr->display_y;
    max_rect.right=max_rect.left+image_w;
    max_rect.bottom=max_rect.top+iamge_h;
    
    if(!GUI_IntersectRect(&rect,max_rect,owner_draw_ptr->display_rect))
    {//没有相交区域，返回
        return ;
    }

    img_rect.left=rect.left-max_rect.left;
    img_rect.top=rect.top-max_rect.top;
    img_rect.right=img_rect.left+(rect.right-rect.left);
    img_rect.bottom=img_rect.top+(rect.bottom-rect.top);

    if(GUI_IsInvalidRect(img_rect)||GUI_IsInvalidRect(rect))//无效矩形不要画了
    {
        return ;
    }
    
    if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
    {
        GUIRES_DisplayImg(PNULL, &rect, &img_rect, win_handle, IMAGE_CLOCK_ALARM_WEEK_MARK, &owner_draw_ptr->lcd_dev);
    }
}
//for widget
PUBLIC void  MMIAPIALARM_OpenEditAlarmWinbyEventid(uint16 event_id)
{
         s_current_event_info.id = event_id;
         MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event); 
         if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
         {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
         }
         s_current_event_info.type = SMART_EVENT_ALARM;
         s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;      
		 if (MMK_IsOpenWin(MMIALM_EDIT_WIN_ID))
		 {
			 MMK_CloseWin(MMIALM_EDIT_WIN_ID);
		 }
      MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
#ifdef CALENDAR_SUPPORT
      MMIAPISCH_DeleteUnreadEvent(s_current_event_info.id);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void setAlarmEditSelectWeeksSoftkey(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id,
                                            BOOLEAN is_update)
{
    if(ctrl_id>=MMI_EDITWIN_BUTTON0_CTRL_ID && ctrl_id<=MMI_EDITWIN_BUTTON6_CTRL_ID)
    {
        if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_CANCEL_ICON, 1, is_update);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_CANCEL, STXT_RETURN, is_update);
#endif
        }
        else
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_SELECTED_ICON, 1, is_update);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_SELECT, STXT_RETURN, is_update);
#endif
        }
    }
}

/*****************************************************************************/
//  Description : 根据ID判断是否是日程事件
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
// LOCAL BOOLEAN IsSCHEvent(uint16 event_id)
// {
//     if (event_id <EVENT_SCH_0||event_id >EVENT_SCH_MAX)
//     {
//         return FALSE;
//     }
//      return TRUE;
// }
#ifdef FM_SUPPORT//CR132759
/*****************************************************************************/
//  Description : change float string to integer value
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author: 
//  Note: //CR132759 liyan.zhu pack. local->pubilc
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_ConvertFloatStrToInt(const MMI_STRING_T *str_ptr, int32 mul_scale)
{
	return AlarmConvertFloatStrToInt(str_ptr,mul_scale);
}
#endif
/*****************************************************************************/
//  Description : change float string to integer value
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int16 AlarmConvertFloatStrToInt(const MMI_STRING_T *str_ptr, int32 mul_scale)
{
    BOOLEAN result = TRUE;
    int32  i = 0;
    int16  ret =0;
    uint16 num_len = 0;
    int16  point_index =0;
    uint8  num_buf[MMIALM_CHANNEL_NAME_LEN + 1] = {0};
    uint8  decimal_buf[MMIALM_CHANNEL_NAME_LEN + 1] = {0};
    
    if(str_ptr != PNULL && str_ptr->wstr_ptr != PNULL)
    {
        num_len = MIN(MMIALM_CHANNEL_NAME_LEN, str_ptr->wstr_len);/*lint !e64*/
        MMI_WSTRNTOSTR((char *)num_buf, MMIALM_CHANNEL_NAME_LEN, str_ptr->wstr_ptr, MMIALM_CHANNEL_NAME_LEN, num_len);/*lint !e64*/
        //get point postion
        for(i =0; i< num_len; i++)
        {
            if('.' == num_buf[i])
            {
                point_index = i;
                break;
            }
        }
        
        for(i = point_index +1; i < num_len; i++)
        {
             if( ( '0' > num_buf[i] ) || ( '9' < num_buf[i] )) 
             {
                 result = FALSE;
                 break;
             }
        }
        
        if(result)
        {
            if(0 != point_index && (point_index + 1 < num_len))
            {
                strncpy(decimal_buf, &num_buf[point_index +1], 1);/*lint !e64*/
            }
            if(point_index > 0)
            {
                //no point
                num_buf[point_index] = 0;
            }
            ret = atoi(num_buf)*mul_scale;/*lint !e64*/
            ret += atoi(decimal_buf);/*lint !e64*/
        }
        else
        {
            ret = 0;
        }
    }
    //SCI_TRACE_LOW:"[MMIFM] ConvertFloatStrToInt ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_10395_112_2_18_2_1_11_56,(uint8*)"d", ret);
    return ret;
}
/*****************************************************************************/
//  Description : 判断是否是一个有效频率值
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsValidFM(    const wchar *fre,     //in
                            uint16       fre_len       //in, 双字节为单位
                            )
{
    BOOLEAN result=TRUE;
    int i=0;
    int dot_pos=0;
    BOOLEAN is_exist_dot=FALSE;
    int16 ret=0;
    MMI_STRING_T str={0};
    str.wstr_ptr=fre;   /*lint !e605*/
    str.wstr_len=fre_len;
    
    if(fre_len>MMIALM_FM_MAX_LEN ||fre_len==0 || PNULL == fre)
    {
        return FALSE;
    }

    for(i=0;i<fre_len;i++)
    {
        if((fre[i]<'0'||fre[i]>'9')&&fre[i]!='.')
        {
            return FALSE;
        }
        if(fre[i]=='.')
        {
            dot_pos=i;
            is_exist_dot=TRUE;
        }
    }
    if(is_exist_dot&&(dot_pos != (fre_len -2)))//只有1位的小数的判断
    {//CR141079
        return FALSE;
    }
    ret=AlarmConvertFloatStrToInt(&str,10);
    if(!(MMIALM_FREQ_RANGE_LOW <= ret && ret <= MMIALM_FREQ_RANGE_HIGH))
    {
        return FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 频率编辑窗口消息处理函数
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFMEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
#ifdef FM_SUPPORT
#ifdef MMI_PDA_SUPPORT
    uint16 s_cur_list_index=0;
#else
    MMI_STRING_T str={0};
    MMI_CTRL_ID_T ctrl_id =MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID;
#endif
	MMIACC_SMART_EVENT_T *detail_win_param = (MMIACC_SMART_EVENT_T *) MMK_GetWinAddDataPtr(win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetFMEditCtrlParam();
#ifdef MMI_PDA_SUPPORT
        MMK_SetAtvCtrl(win_id, MMIALM_SELECT_FM_LIST_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id, MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        s_cur_list_index = GUILIST_GetCurItemIndex(MMIALM_SELECT_FM_LIST_CTRL_ID);
        if(0==s_cur_list_index)
        {
            MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
            *w_id = MMIALM_FM_EDIT_WIN_ID;
            MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
        }
        else if(1==s_cur_list_index)
        {
           MMK_CreatePubEditWin((uint32 *)MMIALM_EDITFMFRE_WIN_TAB, PNULL); 
        }
#else
        ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        if(ctrl_id==MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID)
        {
            MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
            *w_id = MMIALM_FM_EDIT_WIN_ID;
            MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_MENU:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if(ctrl_id==MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID)
        {
            MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
            *w_id = MMIALM_FM_EDIT_WIN_ID;
            MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
        }
        else if(ctrl_id==MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID)
        {
            GUIEDIT_GetString(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, &str);
            if(MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))
            {
                int32 iloop = 0, dot_index = 0;
                BOOLEAN exist_dot = FALSE;

                //因存在点号而点号后面没有数据，导致数据非法，在后面添加0
                //其实应该在函数MMIALM_IsValidFM中处理的，但是这样就修改了原函数
                //的功能，此处有一个地方注意，在点号后直接添加0，
                //等于将编辑器添加了一个字符0，但这里应该不会有什么影响吧
                for(iloop = 0; iloop < str.wstr_len && iloop < MMIALM_FM_MAX_LEN; iloop ++)
                {
                    if((uint16)str.wstr_ptr[iloop] == '.')
                    {
                        exist_dot = TRUE;
                        dot_index = iloop;
                        //break;        //防止存在点号和后续数据，依然被处理了
                    }
                }

                if(exist_dot && (dot_index + 1 == iloop))   //点号为最后一位，添加0
                {
                    str.wstr_ptr[dot_index + 1] = '0';
                    str.wstr_len += 1;
                }
                else if(exist_dot == FALSE)     //没有点号，需要添加点号和0
                {
                    str.wstr_ptr[str.wstr_len] = '.';
                    str.wstr_ptr[str.wstr_len + 1] = '0';
                    str.wstr_len += 2;
                }
            
                detail_win_param->event_content_info.ring_type = ALM_FM_RING;
                detail_win_param->event_ring_info.dring_name_len = str.wstr_len;

                SCI_MEMSET(detail_win_param->event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
                MMI_WSTRNCPY(detail_win_param->event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                str.wstr_ptr, str.wstr_len, str.wstr_len);
                detail_win_param->event_content_info.ring = MMISET_MAX_RING_ID;
                
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
				MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
            }
        }
        break;
#endif
    case MSG_ALM_SELECTED_FM_RETURN:
        {
            char freq_str[MMIALM_FREQ_MAX_LEN+1]={0};
            MMI_STRING_T str_t = {0};
            wchar wfreq_str[MMIALM_FREQ_MAX_LEN+1]={0};
            MMIFM_CHANNEL_T *win_param = (MMIFM_CHANNEL_T *)param;
            sprintf((char *)freq_str,"%d.%d",win_param->freqency/10, win_param->freqency%10);
            str_t.wstr_len = MIN(strlen(freq_str), MMIALM_FREQ_MAX_LEN);    /*lint !e666*/
            str_t.wstr_ptr = wfreq_str;
            MMI_STRNTOWSTR(str_t.wstr_ptr, MMIALM_FREQ_MAX_LEN, freq_str, MMIALM_FREQ_MAX_LEN, str_t.wstr_len);/*lint !e64*/
            if(MMIAPIALM_IsValidFM(str_t.wstr_ptr, str_t.wstr_len))
            {
                detail_win_param->event_content_info.ring_type = ALM_FM_RING;
                detail_win_param->event_ring_info.dring_name_len = str_t.wstr_len;

                SCI_MEMSET(detail_win_param->event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
                MMI_WSTRNCPY(detail_win_param->event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                str_t.wstr_ptr, str_t.wstr_len, str_t.wstr_len);
                detail_win_param->event_content_info.ring = MMISET_MAX_RING_ID;
                
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
				MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
#endif  //FM_SUPPORT
    return recode;
}
#ifdef FM_SUPPORT//CR132759
/*****************************************************************************/
//  Description : 设置频率编辑窗口控件参数
//  Global resource dependence : 
//  Author: liyanzhu
//  Note:   cr132759 liyan.zhu modify local->public
/*****************************************************************************/
PUBLIC void MMIAPIALM_SetFMEditCtrlParam(void)
{
	SetFMEditCtrlParam();
}
/*****************************************************************************/
//  Description : 对外接口：打开ALARM_FM_EDITWIN_TAB
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:   cr132759 liyan.zhu modify local->public
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenFMEditWin(void* param_ptr)
{
	MMK_CreateWin((uint32*)ALARM_FM_EDITWIN_TAB, (ADD_DATA)param_ptr);
}
#endif
/*****************************************************************************/
//  Description : 设置频率编辑窗口控件参数
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   cr132759 liyan.zhu modify local->public
/*****************************************************************************/
LOCAL void SetFMEditCtrlParam(void)
{
#ifdef MMI_PDA_SUPPORT
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data_info = {0};/*lint !e64*/

    GUILIST_SetMaxItem(MMIALM_SELECT_FM_LIST_CTRL_ID, 2, FALSE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data_info;
    item_data_info.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data_info.item_content[0].item_data.text_id = TXT_COMM_CHANNEL_LIST;
    GUILIST_AppendItem( MMIALM_SELECT_FM_LIST_CTRL_ID, &item_t );

#ifndef MMI_GUI_STYLE_MINISCREEN
    item_data_info.item_content[0].item_data.text_id = TXT_ALARM_CUSTOM_FM;
#else
    item_data_info.item_content[0].item_data.text_id = TXT_ALARM_CUSTOM_FM_TEXT;
#endif
    GUILIST_AppendItem( MMIALM_SELECT_FM_LIST_CTRL_ID, &item_t );
    
#else
    wchar  dot_str[] = {'.'};
    GUIFORM_SetType(MMI_FM_EDITWIN_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    GUIFORM_SetStyle(MMI_FM_EDITWIN_LIST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMI_FM_EDITWIN_FRE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID,TXT_COMM_CHANNEL_LIST,FALSE);
#ifndef MMI_GUI_STYLE_MINISCREEN    
    GUILABEL_SetTextById(MMI_FM_EDITWIN_FRE_LABEL_CTRL_ID,TXT_ALARM_CUSTOM_FM,FALSE);
#else
    GUILABEL_SetTextById(MMI_FM_EDITWIN_FRE_LABEL_CTRL_ID,TXT_ALARM_CUSTOM_FM_TEXT,FALSE);
#endif
    GUIEDIT_SetIm(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
    GUIEDIT_ConfigImCustomKey(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
    GUIEDIT_SetImTag(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, GUIIM_TAG_DOT_NUM);
#ifdef  MMI_GUI_STYLE_MINISCREEN      
    GUIEDIT_SetDefaultStringById(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, TXT_ALARM_CUSTOM_FM_FREQ);
#endif
#endif
}

/*****************************************************************************/
//  Description : 判断闹钟是否需要在后台提醒
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsNeedBackgroundPrompt(void)
{
    BOOLEAN result =FALSE;

	if (MMIAPIATV_IsRecording()
#ifdef VIDEO_PLAYER_SUPPORT
		|| MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
		|| MMIAPIMV_IsLiving()
#endif
#if defined(CMCC_UI_STYLE)
        || (MMIAPICC_IsInState(CC_IN_CALL_STATE))
#endif
		)
	{
		result = TRUE;
	}
	
     return result;
}

/*****************************************************************************/
//  Description : 获取闹钟名称
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC  void MMIAPIALM_GetAlarmName(uint16 index,MMI_STRING_T* str_ptr)
{
    MMIACC_SMART_EVENT_T    smart_event = {0};
    uint16 event_id = (uint16)( EVENT_ALARM_0 + index );
    MMI_TEXT_ID_T           txt_alarm_id[] = {  TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,
                                                TXT_ALARM_CLOCK10};
    MMI_STRING_T name_str = {0};
    
    MMIAPIALM_FormInteEventInfo(event_id,&smart_event);

    if (event_id <= EVENT_ALARM_MAX)
    {
        MMI_GetLabelTextByLang(txt_alarm_id[event_id],&name_str);
        if (0 == smart_event.event_content_info.w_length)
        {
            str_ptr->wstr_ptr = name_str.wstr_ptr;
            str_ptr->wstr_len = name_str.wstr_len;
        }
        else
        {
            if(PNULL != str_ptr->wstr_ptr)
            {
                MMI_WSTRNCPY(
                        str_ptr->wstr_ptr, str_ptr->wstr_len,
                        smart_event.event_content_info.w_content, 
                        smart_event.event_content_info.w_length,
                        smart_event.event_content_info.w_length
                        );
                
                str_ptr->wstr_len= smart_event.event_content_info.w_length;
            }
        }       
    }
    else
    {
    }
}

/*****************************************************************************/
//  Description : 用于关机闹钟起闹时初始化lcd背景
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
/*LOCAL void initLcdBavkground(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T               rect = { 0};
    uint16 w=0;
    uint16 h=0;
    if ( (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition()) || (STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition()))
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&w,&h);
        rect.right =w-1;
        rect.bottom =h-1;    
        LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR); 
    }
}*/
#ifdef MMI_MOCOR_SAMPLE
/*****************************************************************************/
//  Description : sample code 测试所用，请不要删除
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIALM_AddTestListCtrlChild(MMI_CTRL_ID_T ctrl_id)
{
    uint16 i = 0; 

    GUILIST_SetMaxItem( ctrl_id, ALM_ALARM_NUM, FALSE );

    for (i = 0; i< ALM_ALARM_NUM; i++)
    {
        uint16 event_id = (uint16)( EVENT_ALARM_0 + i );
        const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );
        
        AppendTwoLineListItem( ctrl_id,
            TXT_NULL,
            s_alarm_mode_text[event_ptr->fre_mode],
            event_id,
            i,
            FALSE );
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 	 	SetListItemStyle(ctrl_id, i);
// #endif
    }
}
#endif
#ifdef MMI_AZAN_SUPPORT
/*****************************************************************************/
// Description : get event table
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T * MMIAPIALM_GetEventFastTab(void)
{
    return s_event_fast_table;
}
/*****************************************************************************/
// Description : find latest event
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_FindLatestEventToSet(void)
{
    return FindLatestEventToSet();
}
#endif

/*****************************************************************************/
// Description : MMIAPIALM_CheckandStartExpiredEvent
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_CheckandStartExpiredEvent(void)
{
     CheckandStartExpiredEvent();
}

/*****************************************************************************/
// Description : MMIAPIALM_IsAlarmActive
// Global resource dependence : 
// Author:  renyi.hu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsAlarmActive(void)
{
    BOOLEAN result=FALSE;
    if(MMK_IsFocusWin(MMIALM_ALIVEWIN_ID)
        &&(s_event_timer_id!=0)) 
    {
        result=TRUE;
    }
    return result;
}
//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   Modifed by leon.wang on 2012.5.28 for Reducing code size
/*****************************************************************************/
PUBLIC uint16 MMIAPIALARM_GetFreModeString(MMIACC_MODE_TYPE_E  fre_mode,uint8 fre,wchar* buff,uint16 buff_max_len)
{
    MMI_STRING_T name_str = {0};
    uint16       len = 0;

#ifndef MMI_ALARM_MINI_SUPPORT
    if(0!=fre && ALM_MODE_EVERYWEEK==fre_mode)
    {
        uint8   week_day = 0;
        wchar   space[2] = {0x20,0};
        uint32  day_num  = 0;

        for(day_num = 0; day_num < ALM_WEEK_DAY_NUM; day_num++)   
        {
            week_day = MMIAPISET_GetWeekDayByOffset(day_num);
            if(0!=(fre & s_week_mask[week_day]) ) 
            {
                MMI_GetLabelTextByLang(s_alm_week_text_id[week_day], &name_str);
                if((MMIAPICOM_Wstrlen(buff)+name_str.wstr_len+1)<buff_max_len)
                {
                    MMIAPICOM_Wstrncat(buff, name_str.wstr_ptr, name_str.wstr_len); 
                    MMIAPICOM_Wstrncat(buff, space, 1); 
                }
            }
        }
    }
    else
#endif
    {
        if(ALM_MODE_NUM>fre_mode)
        {    
            MMI_GetLabelTextByLang(s_alarm_mode_text[fre_mode], &name_str);
            MMIAPICOM_Wstrncpy(buff, name_str.wstr_ptr, MIN(name_str.wstr_len, buff_max_len));
        }
    }

    len=MMIAPICOM_Wstrlen(buff);
    return len;
}
//#endif

/*******************************************************************/
//  Interface:      MMIALM_TimeUpdateCallback
//  Description :   MMIALM_TimeUpdateCallback
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIALM_TimeUpdateCallback(uint32 old_time, uint32 new_time)
{
    //update alarm event
    MMIALM_UpdateEvents();
    
    //set alarm icon in idle
    if (MMIALM_HasActiveEvent())
    {
        MAIN_SetIdleAlarmState(TRUE);
    }
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMIALM_EDIT_NAME_EDITBOX_CTRL_ID;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
         {
            MMI_STRING_T  name_str        = {0};
            MMI_STRING_T  name_str1      = {0};
            MMI_STRING_T  text                = {0};
            wchar           forbid_char_value[]= {0x0d, 0x0a};
            MMI_TEXT_ID_T txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                    TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                    TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10};
            MMI_TEXT_ID_T txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                            TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                            TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};

			if (s_current_event_info.id>=EVENT_ALARM_0&&s_current_event_info.id <= EVENT_ALARM_MAX)/*lint !e685 !e568*/
			{
				MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str1);//coverity 12728
			}
            if (!MMIAPICOM_CompareTwoWstrExt(name_str1.wstr_ptr,
                                                name_str1.wstr_len,
                                                s_current_event_info.event.event_content_info.w_content,
                                                s_current_event_info.event.event_content_info.w_length,
                                                TRUE)
                ||(0 == s_current_event_info.event.event_content_info.w_length) )
            {
                if (s_current_event_info.id>=EVENT_ALARM_0&&s_current_event_info.id <= EVENT_ALARM_MAX)/*lint !e685 !e568*/
                {
                    MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str);
                }
                else if(s_current_event_info.id>=EVENT_ALARM_DELAY_0&&s_current_event_info.id <= EVENT_ALARM_DELAY_MAX)
                {
                    MMI_GetLabelTextByLang(txt_alarm_id_1[s_current_event_info.id-EVENT_ALARM_DELAY_0],&name_str);
                }
            }
            else
            {
                name_str.wstr_ptr = s_current_event_info.event.event_content_info.w_content;
                name_str.wstr_len = s_current_event_info.event.event_content_info.w_length;
            }

            GUIEDIT_SetString(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len);
            MMI_GetLabelTextByLang(STXT_MAIN_MESSAGE,&text);
            GUIWIN_SetTitleText(win_id, text.wstr_ptr, text.wstr_len, FALSE);
            GUIEDIT_ConfigDisableChar(ctrl_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            MMK_SetAtvCtrl(win_id,ctrl_id);

       }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveNameEditWin())
        {
            MMK_CloseWin(win_id);
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : HandleEditTimeMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
    MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIFORM_IsSlide(MMIALM_TIME_FORM_CTRL_ID,FALSE);
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
            GUIEDIT_SetTimeStyle(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
            //GUIEDIT_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_SET_TIME_FONT);

            if (s_current_event_info.event.event_fast_info.is_valid )
            {
                GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                                    s_current_event_info.event.event_fast_info.hour, 
                                    s_current_event_info.event.event_fast_info.minute,
                                    0);
            }
            else
            {
                SCI_TIME_T      sys_time = {0};
                TM_GetSysTime(&sys_time);
                GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                                    sys_time.hour, 
                                    sys_time.min,
                                    0);
            }
            MMK_SetAtvCtrl(win_id,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveTimeEditWin())
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
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditFmFreMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
#ifdef FM_SUPPORT //pclint
    MMI_CTRL_ID_T            ctrl_id = MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID;
    wchar  dot_str[] = {'.'};
    MMI_STRING_T str={0};

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
        GUIEDIT_ConfigImCustomKey(ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_DOT_NUM);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            GUIEDIT_GetString(ctrl_id, &str);
            if(MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))
            {
                s_current_event_info.event.event_content_info.ring_type = ALM_FM_RING;
                s_current_event_info.event.event_ring_info.dring_name_len = str.wstr_len;

                SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
                MMI_WSTRNCPY(s_current_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                str.wstr_ptr, str.wstr_len, str.wstr_len);
                s_current_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
                
                MMK_CloseWin(MMIALM_FM_EDIT_WIN_ID);
                MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
            }
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
#endif
    return result;
}
#endif

#if defined(SCH_ASSISTANT_WIDGET) && defined(MMI_GRID_IDLE_SUPPORT)
LOCAL void LoadCurSchList(MMI_CTRL_ID_T  ctrl_id)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T     item_data= {0};
    uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_1[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_2[GUILIST_STRING_MAX_NUM] = {0};
    
    item_info.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL;
    item_info.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = s_arrived_event.event_content_info.image_id;
    
    item_data.item_content[1].item_data_type = item_data.item_content[2].item_data_type 
        = item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    MMIAPISET_FormatDateStrByDateStyle(s_arrived_event.event_fast_info.start_year,
        s_arrived_event.event_fast_info.start_mon,s_arrived_event.event_fast_info.start_day,
        '/',tmp_item,GUILIST_STRING_MAX_NUM);

    item_data.item_content[1].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);

    item_data.item_content[1].item_data.text_buffer.wstr_ptr = tmp_wstr_1;

    MMI_STRNTOWSTR( tmp_wstr_1,
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    
	//time
    SCI_MEMSET(tmp_item,0,strlen((char*)tmp_item));/*lint !e666*/
	MMIAPISET_FormatTimeStrByTime(
							 s_arrived_event.event_fast_info.hour,
							 s_arrived_event.event_fast_info.minute,
							 tmp_item,
							 GUILIST_STRING_MAX_NUM
							 );
    item_data.item_content[2].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);

    item_data.item_content[2].item_data.text_buffer.wstr_ptr = tmp_wstr_2;

    MMI_STRNTOWSTR( tmp_wstr_2,
        item_data.item_content[2].item_data.text_buffer.wstr_len,
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    if (0!= s_arrived_event.event_content_info.w_length)
    {
        item_data.item_content[3].item_data.text_buffer.wstr_len = s_arrived_event.event_content_info.w_length;

        item_data.item_content[3].item_data.text_buffer.wstr_ptr = s_arrived_event.event_content_info.w_content;
    }
    else
    {
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[3].item_data.text_id = TXT_BLANK;
    }
    GUILIST_AppendItem(ctrl_id, &item_info);
}
#endif

/*****************************************************************************/
//  Discription: MMIAPIALARM_GetALARMTimerID
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:获得alarm TIMER的id，给日程widget使用
//
/*****************************************************************************/
PUBLIC uint8 MMIAPIALARM_GetALARMTimerID(void)
{
    return s_event_timer_id;
}

#if defined(SCH_ASSISTANT_WIDGET) && defined(MMI_GRID_IDLE_SUPPORT)
LOCAL MMI_RESULT_E HandleSchPOPAlertWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T form_ctrl = MMISCH_POP_ALERT_FORM_CTRL_ID;
    MMI_CTRL_ID_T list_ctrl = MMISCH_POP_ALERT_LIST_CTRL_ID;
    MMI_CTRL_ID_T anim_ctrl = MMISCH_POP_ALERT_ANIM_CTRL_ID;
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    uint16        anim_height = 0;
    uint16        anim_width = 0;
    MMI_HANDLE_T  win_handle = 0;
    uint16        list_height = 0;
    uint16        list_width= 0;
    
    win_handle = MMK_ConvertIdToHandle(win_id);

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUIRES_GetImgWidthHeight(&anim_width, &anim_height, IMAGE_WIDGET_SCHEDULE_ASSISTANT, win_id);
    GUIRES_GetImgWidthHeight(&list_width,&list_height,IMAGE_WIDGET_SCHEDULE_TEXT_BG,win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIANIM_CTRL_INFO_T     anim_ctrl_info = {0};         
            GUIANIM_DISPLAY_INFO_T  anim_display_info = {0}; 
        	GUIANIM_DATA_INFO_T     anim_data_info = {0};
        	GUIFORM_CHILD_WIDTH_T   anim_ctrl_width  = {0};
        	GUIFORM_CHILD_HEIGHT_T  list_ctrl_height = {0};
        	GUI_RECT_T              list_rect = {0};

        	anim_data_info.anim_id = ANIM_WIDGET_SCHEDULE_ASSISTANT;
            anim_ctrl_info.is_ctrl_id = TRUE;
            anim_ctrl_info.ctrl_id = anim_ctrl;
            anim_ctrl_info.ctrl_ptr = PNULL;
            anim_display_info.is_syn_decode = TRUE;
            anim_display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            anim_ctrl_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            anim_ctrl_width.add_data = anim_width;

            list_ctrl_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            list_ctrl_height.add_data= list_height;
            
            GUIFORM_PermitChildBg(form_ctrl,FALSE);
    		GUILIST_SetUserBg(list_ctrl,TRUE);
    		GUILIST_SetBgImage(list_ctrl, IMAGE_WIDGET_SCHEDULE_TEXT_BG, FALSE);
    		GUILIST_SetListState(list_ctrl, GUILIST_STATE_SPLIT_LINE, FALSE);
    		GUILIST_SetNeedHiLightBar(list_ctrl,FALSE);
    		GUILIST_SetMaxItem(list_ctrl, 1, FALSE);    
            GUIANIM_SetParam(&anim_ctrl_info,&anim_data_info,PNULL,&anim_display_info);
            GUIFORM_PermitChildBorder(form_ctrl, FALSE);
            GUIFORM_SetChildWidth(form_ctrl, anim_ctrl, &anim_ctrl_width);
            GUIAPICTRL_GetRect(list_ctrl, &list_rect);
            GUIFORM_SetChildHeight(form_ctrl, list_ctrl, &list_ctrl_height);
            LoadCurSchList(list_ctrl);
            recode = HandleEventAliveWinMsg(win_id, msg_id, param); 
        }
        break;   
    case MSG_FULL_PAINT:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYUP_POWER:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        MMIAPIALARM_DealWithCancelOprate();
        break;      

    default:
        recode = HandleEventAliveWinMsg(win_id, msg_id, param); 
        break;
    }
    return recode;
}
#endif

#ifdef ATEST_SUPPORT
/*****************************************************************************/
//  Description : get the Next valid Alarm Time
//  Global resource dependence : 
//  Author:  lin.chen
//  Note:   
/*****************************************************************************/
PUBLIC void ATESTALM_GetLatestAlarmTime(SCI_ALARM_T  *s_alm)
{
	MMIACC_SMART_EVENT_FAST_TABLE_T       event           = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T       latest_event    = {0}; 
    MMIACC_SMART_EVENT_FAST_TABLE_T       cur_point       = {0};    
	SCI_TIME_T  cur_time	=	{0}; 
	SCI_DATE_T  cur_date	=	{0}; 
	uint32		i			=	0;
	uint32		event_id	=	0;
    
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    cur_point.year = cur_date.year; 
    cur_point.mon = cur_date.mon;
    cur_point.day = cur_date.mday;
    cur_point.hour = cur_time.hour;
    cur_point.minute = cur_time.min;
    latest_event.year = 0xFFFF;
	
	for(i = 0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        event = s_event_fast_table[i];
        if (event.is_valid && event.is_on)
        {
            if (MMIAPIALM_CompareTwoEvent(&event, &cur_point) == CMP_SECOND_SMALL &&
                MMIAPIALM_CompareTwoEvent(&latest_event, &event) == CMP_SECOND_SMALL)
            {
                event_id = i;
                latest_event = event;
            }
        }
    }
	s_alm->year = latest_event.year;
	s_alm->mon = latest_event.mon;
	s_alm->mday = latest_event.day;
	s_alm->hour = latest_event.hour;
	s_alm->min = latest_event.minute;
}

#endif	//ATEST_SUPPORT
/*****************************************************************************/
//  Description : Request alarm play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_ReqPlaySrv(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;

    if(s_alarm_play_handle != 0)
    {
        res = TRUE;
    }
    else
    {
        req.pri = MMISRVAUD_PRI_NORMAL;
        audio_srv.info.fm.type = MMISRVAUD_TYPE_FM;//后期请换成alarm的type
        audio_srv.volume = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
		if (0 == audio_srv.volume)//CR200011
        {
            audio_srv.volume = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        audio_srv.all_support_route = MMISRVAUD_ROUTE_EARFREE|MMISRVAUD_ROUTE_SPEAKER;
        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(handle > 0)
        {
            s_alarm_play_handle = handle;
            res = TRUE;
        }
    }
    SCI_TRACE_LOW("[fm]MMIAPIALARM_ReqPlaySrv:alarm_handle %d",s_alarm_play_handle);
    return res;
}
/*****************************************************************************/
//  Description : Get alarm play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAPIALARM_GetPlayHandle(void)
{
    return s_alarm_play_handle;
}
/*****************************************************************************/
//  Description : Free alarm play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIALARM_FreePlaySrv(void)
{
    if(s_alarm_play_handle != 0)
    {
        MMISRVMGR_Free(s_alarm_play_handle);
        s_alarm_play_handle = 0;
    }
}
/*****************************************************************************/
//  Discription: MMIAPIALARM_DealWithCancelOprate
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:处理关闭日程起闹的时间，同HandleEventAliveWinMsg中的MSG_APP_CANCEL要做的事情
//
/*****************************************************************************/
PUBLIC void MMIAPIALARM_DealWithCancelOprate(void)
{
    StopRingOrVib(TRUE);
    SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, FALSE);
#ifdef CALENDAR_SUPPORT
    MMIAPISCH_DeleteUnreadEvent(s_arrived_event_id);
#endif
    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
    if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
            &&(0==s_bypast_list.total)
        )
    {
        CheckandStartExpiredEvent();
        //这种情况下不能MMK_CloseWin(win_id);否则刚打开的自动关机窗口又关掉了；
        //先调用MMK_CloseWin，后调用CheckandStartExpiredEvent是可以的，只是 会引起CR225729 。
    }
    else
    {
        CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO        
    }
}
/*****************************************************************************/
//  Discription: MMIAPIALARM_HandleEventAliveMSG
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:HandleEventAliveWinMsg的封装，供外部使用
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E   MMIAPIALARM_HandleEventAliveMSG(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param 
                                                     )
{
    return HandleEventAliveWinMsg(win_id,msg_id,param);
}

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :录音 后 的 处 理  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AlarmRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr)
{
    //MMIFILE_FILE_INFO_T file_info = {0};
    static MMI_STRING_T    str_record_file_path = {0};
    uint32          size_of_param= 0;

    if (PNULL == data_info_ptr) 
    {
        SCI_TRACE_LOW("[MMIALM]OpenRecord data_info_ptr is pnull");
        return; 
    } 

    if (MMIRECORD_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            str_record_file_path.wstr_ptr=(wchar*)SCI_ALLOCA((data_info_ptr->file_name_len+1)*sizeof(wchar));
            if(str_record_file_path.wstr_ptr==PNULL)
            {
                return ;
            }
            SCI_MEMSET(str_record_file_path.wstr_ptr,0,(data_info_ptr->file_name_len+1)*sizeof(wchar));
            MMIAPICOM_Wstrncpy(str_record_file_path.wstr_ptr, data_info_ptr->file_name, data_info_ptr->file_name_len);
            str_record_file_path.wstr_len = data_info_ptr->file_name_len;
            size_of_param = (str_record_file_path.wstr_len+1)*sizeof(wchar);
            MMK_PostMsg(MMIALM_EDIT_WIN_ID,  MMI_MSG_RECORDTORING, (DPARAM)str_record_file_path.wstr_ptr,size_of_param);
            SCI_FREE(str_record_file_path.wstr_ptr);
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIALM]OpenRecord result is not succ");
    }

    return ;
}

/*****************************************************************************/
//  Discription: RecordAlarmRingCallBack
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:录音设置为铃声的callback
//
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordAlarmRingCallBack(void)
{
    MMIRECORD_WORK_PARAM_T parameter={0};

    parameter.max_file_size = 0;
    parameter.max_file_time = 0;
    parameter.record_support_type = (uint32)MMIRECORD_WORK_FORMAT_WAV|(uint32)MMIRECORD_WORK_FORMAT_AMR|(uint32)MMIRECORD_WORK_FORMAT_MP3;
    parameter.work_callback = AlarmRecordWorkCallBack;
    if(MMIAPIRECORD_OpenMainWin(&parameter) != MMIRECORD_WORK_SUCCESS)
    {
        SCI_TRACE_LOW("[MMIALM]OpenRecord alarm record error!");
    }
    return MMI_RESULT_TRUE;
}
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJumpClockEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_STRING_T            temp_str = {0};
    uint16 event_id=0;
    //uint16      width = 0; 
    //uint16      height = 0; 
#if defined MAINLCD_SIZE_240X400 || defined MAINLCD_SIZE_320X480
    GUI_POINT_T point;
#endif
    GUI_RECT_T   win_rect ={0};
    SCI_TIME_T  sys_time ={0};
    SCI_DATE_T  sys_date = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		if (SMART_EVENT_ALARM == s_arrived_event.event_fast_info.type)
		{
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
			GUIWIN_SetSoftkeyTextId(win_id, TXT_CLOSE, TXT_NULL,TXT_ALARM_DELAY, FALSE);
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id, TXT_CLOSE, TXT_NULL,TXT_ALARM_DELAY, FALSE);
#endif
            // GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_CLOSE,TXT_NULL, FALSE);
		}
        	
		//刷新小屏显示，显示闹钟等事件信息
        if (SMART_EVENT_ALARM ==s_arrived_event.event_fast_info.type) // assigne default text                
        {                
            MMI_GetLabelTextByLang(TXT_ALARM_CLOCK, &temp_str);                
        }
        else                
        {                
            temp_str.wstr_len = s_arrived_event.event_content_info.w_length;                 
            temp_str.wstr_ptr = s_arrived_event.event_content_info.w_content;                 
        } 
        
        
        StartAlarmTimer(&s_event_timer_id, ALERT_RING_DURATION, FALSE);
        
        StartRingOrVib();//U盘使用中的情况也在该函数中处理
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);//禁止横屏
        s_clk_timer_id = MMK_CreateWinTimer(win_id, 100, TRUE);

        //以下是点亮屏幕的操作,需放在MMIPUB_HandleWaitWinMsg之后处理
        if ( STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
        {
            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        else
        {
            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_ALARM,&temp_str);    
        break;	
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;           
            default:
                break;
            }
        }
        break;       
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_FULL_PAINT:
        {
            
            //GUI_POINT_T point={0};
            MMK_GetWinRect(win_id, &win_rect);

            TM_GetSysTime(&sys_time);

            TM_GetSysDate(&sys_date);
   
            GUIRES_DisplayImg(PNULL,
		        &win_rect,
		        &win_rect,
		        win_id,
		        IMG_JUMP_WIN_BG,
		        &lcd_dev_info);
            sys_date.mday=s_arrived_event.event_fast_info.day;
	        sys_date.mon=s_arrived_event.event_fast_info.mon;
	        sys_date.year=s_arrived_event.event_fast_info.year;
	        sys_time.hour=s_arrived_event.event_fast_info.hour;
	        sys_time.min=s_arrived_event.event_fast_info.minute;            
            MMIAPIALARM_DisplayAlmClock(win_id, sys_time, win_rect, s_rotate_info_arr[s_pic_index].angel);
#if defined MAINLCD_SIZE_240X400 || defined MAINLCD_SIZE_320X480
            TM_GetSysTime(&sys_time);

            TM_GetSysDate(&sys_date);
            point.x = CLK_TIME_X;
            point.y = CLK_TIME_Y;
            MMIAPIALARM_DrawTime(sys_time, point);
            point.x = CLK_DATE_X;
            point.y = CLK_DATE_Y;
            MMIAPIALARM_DrawDate(sys_date, point); 
#endif
        }
        break;
        
    case MSG_GET_FOCUS:
        if(s_clk_timer_id == 0)
        {
        
            s_clk_timer_id = MMK_CreateWinTimer(win_id, 100, TRUE);
        }       
        if (s_event_timer_id ==0)
        {
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
	        {
	            MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
	        }
#endif
		}
		else
		{
			if ( STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
	        {
                    MMIDEFAULT_TurnOnBackLight();
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            }
            else
            {
                MMIDEFAULT_TurnOnBackLight();
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            }
#ifdef MMI_KEY_LOCK_SUPPORT
            if(MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(win_id,MSG_ALM_START_RING,PNULL,0);//解决来电挂断铃声起不来问题
            }
            else
#endif
            {
                StartRingOrVib();
            }
        }
        break;
    case MSG_ALM_START_RING:
        StartRingOrVib();
        break;
     
    case MSG_LOSE_FOCUS:
        if(s_clk_timer_id != 0)
        {
        
            MMK_StopTimer(s_clk_timer_id);
            s_clk_timer_id = 0;
        }
        StopRingOrVib(FALSE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_PROMPTWIN_OK:    
        if (s_arrived_event_id < EVENT_SCH_0)
        {
            if(s_arrived_event_id>=EVENT_ALARM_0&&s_arrived_event_id<=EVENT_ALARM_MAX)/*lint !e685 !e568*/
            {
                event_id = s_arrived_event_id;
            }
            else if(s_arrived_event_id>=EVENT_ALARM_DELAY_0&&s_arrived_event_id<=EVENT_ALARM_DELAY_MAX)
            {
               event_id = s_arrived_event_id-EVENT_ALARM_DELAY_0; 
            }
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_DeleteUnreadEvent(event_id);
#endif            
            StopRingOrVib(TRUE);
            CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);
            if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
                &&(0==s_bypast_list.total)
                )
            {
                CheckandStartExpiredEvent();
                //这种情况下不能MMK_CloseWin(win_id);否则刚打开的自动关机窗口又关掉了；
                //先调用MMK_CloseWin，后调用CheckandStartExpiredEvent是可以的，只是 会引起CR225729 。
            }
            else
            {
                CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO
                MMK_CloseWin(win_id);
            }

            
        }
        break;
        
    case MSG_TIMER:
        if (s_vibrator_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            if (is_vibrator_on)
            {
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ALARM);
                is_vibrator_on = FALSE;
            }
            else
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
                is_vibrator_on = TRUE;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
        }
        else if (s_event_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            // 停止闹钟
            if(is_ring_on)
            {
                StopRingOrVib(TRUE);
            }
            else
            {
                MMK_StopTimer(s_event_timer_id);
                s_event_timer_id = 0;
            }
            
            if(s_vibrator_timer_id!=0)
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            is_vibrator_on = FALSE;
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_AddUnreadEvent(s_arrived_event_id);
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                if(MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)||MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
                }
            }
#endif
            // 设置延时的闹钟
            SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, TRUE);
            
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            
            MMK_CloseWin(win_id);
            CheckandStartExpiredEvent();
        }
        else if (s_clk_timer_id == *((uint8 *) param))
        {
            s_pic_index =(s_pic_index+1)%CLK_ROTATE_PIC_NUM;
            MMK_UpdateScreen();   
        }            
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP://合盖应该和按停止键同样处理,@zhaohui,cr102341
        StopRingOrVib(TRUE);
        CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);
        MMK_CloseWin(win_id);
        CheckandStartExpiredEvent();
        recode = MMI_RESULT_FALSE;
        break; 
#endif
        
    case MSG_CLOSE_WINDOW:
        if(s_event_timer_id != 0)
        {
            SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, TRUE);
        }
        if(s_clk_timer_id != 0)
        {
        
            MMK_StopTimer(s_clk_timer_id);
            s_clk_timer_id = 0;
        }      
        StopRingOrVib(TRUE);
        
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        //刷新小屏显示，关闭显示闹钟等事件信息
		MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_ALARM,PNULL);
        MMIAPICT_CheckandStartDelayedCoutimeEvent();//@zhaohui,counted time,弹出延迟的倒计时到提示
        break;
        
    case MSG_KEYDOWN_RED:
    case MSG_KEYUP_POWER:
        StopRingOrVib(TRUE);
        SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, FALSE);
        
        if (
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_IsPhoneLocked()||
#endif
            (s_bypast_list.total > 0)||
            (s_poweroff_event.event_fast_info.is_valid)||
            MMIAPICT_CheckDelayedCoutimeEvent()||
            MMK_IsOpenWin(MMISET_FLY_MODE_OPEN_PHONE_WIN_ID)
            )//锁键或有延时事件，则只关闭当前窗口
        {
            recode = MMI_RESULT_TRUE;   
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }

        if (MMI_RESULT_TRUE == recode||MSG_KEYUP_POWER==msg_id)
        {
            MMK_CloseWin(win_id);
        }

        CheckandStartExpiredEvent();
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYUP_UPSIDE:
    case MSG_KEYUP_DOWNSIDE:
    case MSG_KEYLONG_UPSIDE:
    case MSG_KEYLONG_DOWNSIDE:
    case MSG_KEYPRESSUP_DOWNSIDE:
    case MSG_KEYPRESSUP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        StopRingOrVib(TRUE);
        SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, FALSE);

        
#ifdef CALENDAR_SUPPORT       
        MMIAPISCH_DeleteUnreadEvent(s_arrived_event_id);
#endif       
        if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
                &&(0==s_bypast_list.total)
            )
        {
            CheckandStartExpiredEvent();
            //这种情况下不能MMK_CloseWin(win_id);否则刚打开的自动关机窗口又关掉了；
            //先调用MMK_CloseWin，后调用CheckandStartExpiredEvent是可以的，只是 会引起CR225729 。
        }
        else
        {
            CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO
            MMK_CloseWin(win_id);
        }
        break;
        
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_KEYUP_FLIP:
        MMIDEFAULT_HandleFlipKey(TRUE);  
        break;
#endif
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
/*****************************************************************************/
//Description :   ALARM OPT
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmMainOptWin(
                                           MMI_WIN_ID_T    win_id,   // 窗口ID
                                           MMI_MESSAGE_ID_E   msg_id,   // 消息ID
                                           DPARAM             param     // 消息参数
                                           )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIALM_OPT_MENU_CTRL_ID;    
    uint16 total_alm_num = GUILIST_GetTotalItemNum(MMIALM_LISTALL_CTRL_ID);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(0 == total_alm_num)
        {
            GUIMENU_SetItemGrayed(ctrl_id, MENU_ALARM_OPT, ID_ALM_OPT_EDIT, TRUE);
            GUIMENU_SetItemGrayed(ctrl_id, MENU_ALARM_OPT, ID_ALM_OPT_DEL, TRUE);
        }
        else if(ALM_ALARM_NUM <= total_alm_num)
        {
            GUIMENU_SetItemGrayed(ctrl_id, MENU_ALARM_OPT, ID_ALM_OPT_ADD, TRUE);
        }        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32      group_id    =   0 ;
            uint32      menu_id     =   0;
            GUIMENU_GetId(ctrl_id,&group_id,&menu_id);

            switch(menu_id)
            {
            case ID_ALM_OPT_ADD:
                {
                    uint16 id = FindAlarmUnuseId();
                    
                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        
                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);	
                        if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
                        {
                            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
                            s_current_event_info.event.event_content_info.ring = 1;
                        }

                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_content_info.is_add = TRUE;
                        MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
                    }
                }
                break;

            case  ID_ALM_OPT_EDIT:
                {
                    uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                    
                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;

                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);	
                        if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
                        {
                            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
                            s_current_event_info.event.event_content_info.ring = 1;
                        }
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;

                        MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
                    }
                }
                break;

            case ID_ALM_OPT_DEL: 
                {
                    uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                    uint16 total_num = 0;
                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;

                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.is_on = FALSE;
                        s_current_event_info.event.event_content_info.w_length = 0;
                        s_current_event_info.event.event_content_info.is_add = FALSE;
                        SetDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
                        MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);	
                        GUILIST_RemoveItem(MMIALM_LISTALL_CTRL_ID, GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                        total_num = GUILIST_GetTotalItemNum(MMIALM_LISTALL_CTRL_ID);
                        if(0 == total_num)
                        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
							GUIWIN_SetSoftkeyTextId(MMIALM_MAINWIN_ID, 
								STXT_OPTION, 
								TXT_NULL, 
								STXT_RETURN,
                                                    FALSE);
							GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
                            GUIWIN_SetSoftkeyTextId(MMIALM_MAINWIN_ID, 
                                                    STXT_OPTION, 
                                                    TXT_NULL, 
                                                    STXT_RETURN,
                                                    FALSE);
#endif
                        }
                        MMK_PostMsg(MMIALM_MAINWIN_ID, MSG_FULL_PAINT, PNULL, 0);	
                    }
                }
                break;

            default:
                break;
            } 
            MMK_CloseWin( win_id );    
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
} 

/*****************************************************************************/
//Description :   GetAlarmRealId
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL uint16  GetAlarmRealId(uint16 id)
{
    uint16 i = 0;
    uint16 num = 0;
    
    for ( i = 0; i < ALM_ALARM_NUM; i++ )
    {
        MMIACC_SMART_EVENT_T smart_event = {0};

        MMIAPIALM_FormInteEventInfo(i, &smart_event);
        if(smart_event.event_content_info.is_add)
        {
            if(id == num)
            {
                return i;
            }
            num++;
        }
    }

    return ALM_ALARM_NUM;
}

LOCAL uint16  FindAlarmUnuseId(void)
{
    uint16 i = 0;
    
    for ( i = 0; i < ALM_ALARM_NUM; i++ )
    {
        MMIACC_SMART_EVENT_T smart_event = {0};

        MMIAPIALM_FormInteEventInfo(i, &smart_event);
        if(!smart_event.event_content_info.is_add)
        {
            return i;
        }
    }

    return ALM_ALARM_NUM;
}
#endif//CMCC_UI_STYLE
//CR132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// LOCAL void SetListItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index)
// {
// #if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)&&(!defined MMI_THEME_LIST_MINI_SUPPORT)
//     THEMELIST_ITEM_STYLE_T* pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_THREE_LINE_TEXT_ICON);
// #else
//     THEMELIST_ITEM_STYLE_T* pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL);
// #endif
// 	
// 	if (pitem_style)
// 	{	
//         SCI_MEMCPY(&s_my_style, pitem_style, sizeof(THEMELIST_ITEM_STYLE_T));
// #if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)
//         s_my_style.content[0].rect.left = s_my_style.content[0].rect_focus.left = ALARM_MODE_LEFT;
//         s_my_style.content[1].rect.left = s_my_style.content[1].rect_focus.left = ALARM_MODE_LEFT;
//         s_my_style.content[2].rect.left = s_my_style.content[2].rect_focus.left = ALARM_MODE_LEFT;
//         s_my_style.content[0].rect.right = s_my_style.content[0].rect_focus.right = ALARM_MODE_RIGHT;
//         s_my_style.content[1].rect.right = s_my_style.content[1].rect_focus.right = ALARM_MODE_RIGHT;
//         s_my_style.content[2].rect.right = s_my_style.content[2].rect_focus.right = ALARM_MODE_RIGHT;
//         s_my_style.content[3].rect.left = s_my_style.content[3].rect_focus.left = ALARM_ICON_LEFT;
//         s_my_style.height = 50;
//         s_my_style.content[0].rect.top = s_my_style.content[0].rect_focus.top = 0;
//         s_my_style.content[0].rect.bottom = s_my_style.content[0].rect_focus.bottom = s_my_style.content[0].rect.top + 16;
//         s_my_style.content[1].rect.top = s_my_style.content[1].rect_focus.top = s_my_style.content[0].rect.bottom + 1;
//         s_my_style.content[1].rect.bottom = s_my_style.content[1].rect_focus.bottom = s_my_style.content[1].rect.top + 16;
//         s_my_style.content[2].rect.top = s_my_style.content[2].rect_focus.top = s_my_style.content[1].rect.bottom + 1;
//         s_my_style.content[2].rect.bottom = s_my_style.content[2].rect_focus.bottom = s_my_style.content[2].rect.top + 16;
//         s_my_style.content[3].rect.top = s_my_style.content[3].rect_focus.top = s_my_style.content[1].rect.top;
//         s_my_style.content[3].rect.bottom = s_my_style.content[3].rect_focus.bottom = s_my_style.content[1].rect.bottom;
// 		s_my_style.content[0].scale_left = s_my_style.content[0].scale_right =
// 			s_my_style.content[1].scale_left = s_my_style.content[1].scale_right = 
// 			s_my_style.content[2].scale_left = s_my_style.content[2].scale_right =
// 			s_my_style.content[3].scale_left = s_my_style.content[3].scale_right = 0;
// 
// #else
// 		s_my_style.content[3].rect.left = s_my_style.content[3].rect_focus.left = ALARM_MODE_LEFT;
// 		s_my_style.content[3].rect.right = s_my_style.content[3].rect_focus.right = ALARM_MODE_RIGHT;
// 		s_my_style.content[1].rect.left = s_my_style.content[1].rect_focus.left = ALARM_ICON_LEFT;
// 		s_my_style.content[0].scale_left = s_my_style.content[0].scale_right =
// 			s_my_style.content[1].scale_left = s_my_style.content[1].scale_right = 
// 			s_my_style.content[2].scale_left = s_my_style.content[2].scale_right =
// 			s_my_style.content[3].scale_left = s_my_style.content[3].scale_right = 0;
// 
// #endif
// 		GUILIST_SetItemStyleEx(ctrl_id, index, &s_my_style);
// 	}
// }
// #endif 

/*Edit by script, ignore 7 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527


/*Edit by script, ignore 13 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527

PUBLIC uint8 GetAlarmTotalNum()
{ 
	uint8 i = 0, num = 0;
	for ( i = 0; i < ALM_ALARM_NUM; i++ )
	{
		MMIACC_SMART_EVENT_T smart_event = {0};
		uint8 event_id = EVENT_ALARM_0 + i;
		MMIAPIALM_FormInteEventInfo(event_id, &smart_event);
		//if(smart_event.event_content_info.is_add)
		{ 
			num++;
		}
	}
	return num;
}

//[edit repeat list]
const uint8 s_week_mask_frm_mon[ALM_MODE_WEEK_NUM] = {MASK_B1, MASK_B2, MASK_B3, MASK_B4, MASK_B5, MASK_B6, MASK_B0};
//[end]
PUBLIC void GetRepeatModeString(
                            MMI_STRING_T *p_repeat_str,
                            MMIACC_MODE_TYPE_E fre_mode,
                            uint8 fre_mask
                            )
{
    MMI_STRING_T txt_specday_temp = {0};
    MMI_STRING_T txt_specday_temp1 = {0};
    MMI_STRING_T txt_comma_space = {0};
    MMI_STRING_T res_strid = {0};
    wchar comma_and_space[] = {',',' ',0};
    MMI_TEXT_ID_T edit_repeat_arr[ALM_MODE_WEEK_NUM] =
    {
        STR_ALARM_REPEAT_MO_EXT01,
        STR_ALARM_REPEAT_TU_EXT01,
        STR_ALARM_REPEAT_WE_EXT01,
        STR_ALARM_REPEAT_TH_EXT01,
        STR_ALARM_REPEAT_FR_EXT01,
        STR_ALARM_REPEAT_SA_EXT01,
        STR_ALARM_REPEAT_SU_EXT01
    };

    txt_comma_space.wstr_ptr = comma_and_space;
    txt_comma_space.wstr_len = MMIAPICOM_Wstrlen(comma_and_space);

    switch(fre_mode)
    {
        case ALM_MODE_ONCE:
            MMI_GetLabelTextByLang(STR_ILIST_NOT_REPEATED,p_repeat_str);
            break;
        case ALM_MODE_EVERYDAY:
            MMI_GetLabelTextByLang(STR_ALARM_LIST_RP_EVD_EXT02,p_repeat_str);
            break;
        case ALM_MODE_EVERYWEEK:
            if(fre_mask == MASK_WEEKDAY)
            {
                MMI_GetLabelTextByLang(STR_ALARM_LIST_RP_WKD_EXT02,p_repeat_str);
            }
            else if(fre_mask == MASK_WEEKEND)
            {
                MMI_GetLabelTextByLang(STR_LIST_REPEAT_WEEKENDS_EXT01,p_repeat_str);
            }
            else
            {
                int i = 0;
                BOOLEAN is_first_time = TRUE;
                for (i = 0; i < ALM_MODE_WEEK_NUM; i++)
                {
                    if (fre_mask & s_week_mask_frm_mon[i])
                    {
                        MMI_GetLabelTextByLang(edit_repeat_arr[i],&res_strid);
                        if(is_first_time)
                        {
                            MMI_STRING_T empty_str = {0};
                            MMIAPICOM_CatTwoString(&txt_specday_temp1,&empty_str,&res_strid);
                            is_first_time = FALSE;
                        }
                        else
                        {
                            MMIAPICOM_CatTwoString(&txt_specday_temp1,&txt_comma_space,&res_strid);
                        }
                        MMIAPICOM_CatTwoString(&txt_specday_temp,p_repeat_str,&txt_specday_temp1);
                        MMIAPICOM_DestroyString(*p_repeat_str);
                        MMIAPICOM_DestroyString(txt_specday_temp1);
                        p_repeat_str->wstr_ptr = SCI_ALLOCA((txt_specday_temp.wstr_len + 1) * sizeof(wchar));
                        p_repeat_str->wstr_len = txt_specday_temp.wstr_len;
                        SCI_MEMSET(p_repeat_str->wstr_ptr,0,(p_repeat_str->wstr_len + 1) * sizeof(wchar));
                        MMIAPICOM_Wstrcpy(p_repeat_str->wstr_ptr,txt_specday_temp.wstr_ptr);
                        MMIAPICOM_DestroyString(txt_specday_temp);
                    }
                }
            }
            break;
        default:
            break;
    }
}
