/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_DATETIME_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "guires.h"
#include "mmk_timer.h"
#include "guisetlist.h"
#if defined(MMI_UPDATE_TIME_SUPPORT)
#include "mmiaut_export.h"
#endif
#include "mmiset_wintab.h"
#include "mmiset_image.h"
#include "mmiset_func.h"
#include "guitext.h"
#include "guiwin.h"
#include "mmiset_nv.h"
#include "mmiidle_statusbar.h"
#ifdef MMI_PDA_SUPPORT
#include "mmiset_wintab.h"
#endif
#include "mmiidle_subwintab.h"
#ifdef MMI_PHONESET_MINI_SUPPORT
#include "guictrl_api.h"
#endif
#ifdef DYNAMIC_WATCHFACE_SUPPORT
#include "watchface_view.h"
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#define  MMISET_DATE_TIME_TYPE_BY_WIN  0


#if !defined MMI_GUI_STYLE_TYPICAL
LOCAL uint8 s_time_refresh_timer_id = 0;
#endif


#if defined(MMI_UPDATE_TIME_SUPPORT)
LOCAL     SCI_TIME_T                  s_sys_time = {0}; //remember sys time
LOCAL     SCI_DATE_T                  s_sys_date = {0};//remember sys date
#endif/*MMI_UPDATE_TIME_SUPPORT*/

#define     MMIIDLE_DATE_LEN                        12
LOCAL MMISET_DATE_DISPLAY_TYPE_E s_date_display_type = MMISET_DEFAULT_DATE_YMD;

LOCAL BOOLEAN s_is_user_update_time = FALSE;

/*****************************************************************************/
//  Description :refresh keyboard time when system time changed
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void RefreshKeyboardLight(void);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetPdaTimeLabel(BOOLEAN is_enter_from_setting);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void UpdateStbDispTime(BOOLEAN is_enter_from_setting);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetPdaDateLabel(BOOLEAN is_enter_from_setting);
#endif
/*****************************************************************************/
//  Description : set time_setting param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetTimeSettingParam(void);

/*****************************************************************************/
//  Description : set date setting param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetDateSettingParam(void);

/*****************************************************************************/
//  Description : set time display param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetTimeDisplayTypeParam(void);

/*****************************************************************************/
//  Description : set date display param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetDateDisplayTypeParam(void);

/*****************************************************************************/
//  Description : save time date setting
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveTimeDatesetting(void);
#if defined(MMI_UPDATE_TIME_SUPPORT)

/*****************************************************************************/
//  Description : Set aut menu name 
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SetAutLabelParam(void);

/*****************************************************************************/
//  Description : get if time modified when exit set time&date window 
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTimeModified(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : get if date modified when exit set time&date window 
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsDateModified(MMI_CTRL_ID_T ctrl_id);
// 
// /*****************************************************************************/
// //  Description : 手动更新时间启动
// //  Global resource dependence : 
// //  Author:xiaoming.ren 
// //  Note: 
// /*****************************************************************************/
// LOCAL void  StartManualUpdateSystemTime(void);

#ifdef MMI_SNTP_SUPPORT //support manual update
/*****************************************************************************/
//  Description : to handle update time window message
//  Global resource dependence : 
//  Author:Dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetUpdateTimeWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : 处理自动更新时间
//  Global resource dependence : 
//  Author:xiaoming.ren 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoUpdateTimeWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );
/*****************************************************************************/
// 	Description : 处理打开设置时间/日期窗口时，是否关闭"自动更新时间"功能
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCloseAutoUpdateQueryWindow( 
                                                    MMI_WIN_ID_T     win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param
                                                    );

/*****************************************************************************/
//  Description :处理打开自动更新时间确认窗口消息函数
//  Global resource dependence : current select item id
//  Author:Dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenAutoUpdateQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );

#endif


/*****************************************************************************/
//  Description :打开自动更新时间确认窗口
//  Global resource dependence : current select item id
//  Author:Dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void MMISET_OpenAutoUpdateTimeQueryWin(BOOLEAN is_auto_update_time);

/*****************************************************************************/
// 	Description : 等待窗口处理
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:
/*****************************************************************************/
// LOCAL MMI_RESULT_E HandleUpdateSystemTimeWaitWin(
//                                                  MMI_WIN_ID_T win_id,
//                                                  MMI_MESSAGE_ID_E msg_id,
//                                                  DPARAM param
//                                                 );

/*****************************************************************************/
// 	Description : to handle update time help function,display the help infor
//	Global resource dependence : 
//   Author:  dave.ruan
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleUpdateTimeHelpWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param);

#endif/*MMI_UPDATE_TIME_SUPPORT*/


/*****************************************************************************/
//  Description :在时间日期界面修改时间显示风格时，当前界面的time edit同时更新
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void ChangeTimeEditDispaly(uint8 index);

/*****************************************************************************/
//  Description :在时间日期界面修改日期显示风格时，当前界面的date同时更新
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void ChangeDateEditDispaly(uint8 index);

/*****************************************************************************/
//  Description : 设置输入时间或者日期窗口的背景色，image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetTimeDateBackground(
                                 GUI_POINT_T dis_point,
                                 uint32    icon_id,
                                 MMI_WIN_ID_T win_id
                                 );

/*****************************************************************************/
//  Description : 设置时间时，显示系统时间
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySystemTime(
                             MMI_CTRL_ID_T     ctrl_id,
                             BOOLEAN  is_enter_form_setting//是否由setting菜单进入时间设置窗口
                             );

/*****************************************************************************/
//  Description : 设置输入的时间为系统时间
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTime(
                      MMI_CTRL_ID_T     ctrl_id
                      );
/*****************************************************************************/
//  Description : to handle set time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );
/*****************************************************************************/
//  Description : to handle set date
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDateWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/*****************************************************************************/
//  Description : 设置日期时，显示系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySystemDate(
                             MMI_CTRL_ID_T     ctrl_id,
                             BOOLEAN   is_enter_from_setting//是否从设置菜单进入设置日期窗口
                             );

/*****************************************************************************/
//  Description : 设置输入的日期为系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDate(
                      MMI_CTRL_ID_T     ctrl_id
                      );
/*****************************************************************************/
//  Description : to handle time and date setting window message
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeDateWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
                                            
#ifdef MMISET_DATE_TIME_TYPE_BY_WIN                                            
/*****************************************************************************/
//  Description : 处理时间显示格式
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeDisplayTypeWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   );
#endif

/*****************************************************************************/
//  Description : 添加日期显示格式选择的radio list的item
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void AppendDateDisplayTypeListItem(
                                         MMI_CTRL_ID_T      ctrl_id
                                         );

/*****************************************************************************/
//  Description : set date display type 
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void SetDateDisplayType(
                              MMISET_DATE_DISPLAY_TYPE_E date_display_type
                              );

#ifdef MMISET_DATE_TIME_TYPE_BY_WIN
/*****************************************************************************/
//  Description : 处理日期显示格式
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDateDisplayTypeWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   );
#endif

/*****************************************************************************/
//  Description : 等待窗口的处理函数
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeWaitingWinMsg(
                                               MMI_WIN_ID_T win_id,            
                                               MMI_MESSAGE_ID_E msg_id,        
                                               DPARAM param
                                               );  
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : 等待窗口的处理函数
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDateWaitingWinMsg(
                                               MMI_WIN_ID_T win_id,            
                                               MMI_MESSAGE_ID_E msg_id,        
                                               DPARAM param
                                               );                                                                                                  
                                            
/*****************************************************************************/                                            
#endif
// the window for phone setting 
WINDOW_TABLE( MMISET_SET_TIME_DATE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSetTimeDateWindow ),    
    WIN_ID( MMISET_SET_TIME_DATE_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //CREATE_MENU_CTRL(MENU_SET_TIME_DATE_OPT, MMISET_SET_TIME_DATE_MENU_CTRL_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_TIME_DATE_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_TIME_CHILD_FORM_CTRL_ID,MMISET_SET_TIME_DATE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SET_TIME_LABEL_CTRL_ID, MMISET_SET_TIME_CHILD_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMISET_SET_TIME_EDIT_CTRL_ID, MMISET_SET_TIME_CHILD_FORM_CTRL_ID),
#else
    CHILD_EDIT_TIME_CTRL(TRUE, MMISET_SET_TIME_EDIT_CTRL_ID, MMISET_SET_TIME_CHILD_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_DATE_CHILD_FORM_CTRL_ID,MMISET_SET_TIME_DATE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SET_DATE_LABEL_CTRL_ID, MMISET_SET_DATE_CHILD_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMISET_SET_DATE_EDIT_CTRL_ID, MMISET_SET_DATE_CHILD_FORM_CTRL_ID),
#else
    CHILD_EDIT_DATE_CTRL(TRUE, MMISET_SET_DATE_EDIT_CTRL_ID, MMISET_SET_DATE_CHILD_FORM_CTRL_ID),
#endif
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_TIME_FORMAT_CHILD_FORM_CTRL_ID,MMISET_SET_TIME_DATE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SET_TIME_DISPLAY_LABEL_CTRL_ID, MMISET_SET_TIME_FORMAT_CHILD_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE, MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID, MMISET_SET_TIME_FORMAT_CHILD_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_DATE_FORMAT_CHILD_FORM_CTRL_ID,MMISET_SET_TIME_DATE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SET_DATE_DISPLAY_LABEL_CTRL_ID, MMISET_SET_DATE_FORMAT_CHILD_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE, MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID, MMISET_SET_DATE_FORMAT_CHILD_FORM_CTRL_ID),

#if defined(MMI_UPDATE_TIME_SUPPORT)
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_UPDATE_TIME_CTRL_ID,MMISET_SET_TIME_DATE_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_AUTO_UPDATE_TIME_LABEL_CTRL_ID,MMISET_SET_UPDATE_TIME_CTRL_ID),
#ifndef MMI_SNTP_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SET_AUTO_UPDATE_TIME_CTRL_ID,MMISET_SET_UPDATE_TIME_CTRL_ID),
#endif
#endif

    END_WIN
};

// the window for set time
WINDOW_TABLE( MMISET_SET_TIME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetTimeWindow ),    
        WIN_ID( MMISET_SET_TIME_WIN_ID ),
        WIN_TITLE( TXT_SET_TIME ),
#ifdef MMI_PDA_SUPPORT
       // WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_TIME_FORM_CTRL_ID),
    
        CHILD_EDIT_TOUCH_TIME_CTRL(TRUE,MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID,MMISET_TIME_FORM_CTRL_ID),
#else
        CREATE_EDIT_TIME_CTRL(MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID),        
#endif
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};


// the window for set date
WINDOW_TABLE( MMISET_SET_DATE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetDateWindow ),    
        WIN_ID( MMISET_SET_DATE_WIN_ID ),
        WIN_TITLE( TXT_SET_DATE ),
#ifdef MMI_PDA_SUPPORT
        //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DATE_FORM_CTRL_ID),
    
        CHILD_EDIT_TOUCH_DATE_CTRL(TRUE,MMISET_SET_DATE_DATEEDITBOX_CTRL_ID,MMISET_DATE_FORM_CTRL_ID),        
#else
        CREATE_EDIT_DATE_CTRL(MMISET_SET_DATE_DATEEDITBOX_CTRL_ID),       
#endif
         WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#ifdef MMISET_DATE_TIME_TYPE_BY_WIN
//时间显示格式设置窗口
WINDOW_TABLE(MMISET_SET_TIME_DISPLAY_TYPE_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSetTimeDisplayTypeWindow ),
        WIN_ID(MMISET_SET_TIME_DISPLAY_TYPE_WIN_ID),
        WIN_TITLE( TXT_SET_TIME_DISPLAY_TYPE),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_TIME_DISPLAY_TYPE_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//日期显示格式设置窗口
WINDOW_TABLE(MMISET_SET_DATE_DISPLAY_TYPE_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSetDateDisplayTypeWindow ),
        WIN_ID(MMISET_SET_DATE_DISPLAY_TYPE_WIN_ID),
        WIN_TITLE( TXT_SET_DATE_DISPLAY_TYPE),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_DATE_DISPLAY_TYPE_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
//更新时间设置
#ifdef MMI_SNTP_SUPPORT
WINDOW_TABLE( MMISET_SET_UPDATE_TIME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetUpdateTimeWindow),
    WIN_ID(MMISET_SET_UPDATE_TIME_WIN_ID),
    WIN_TITLE( TXT_SET_UPDATE_TIME_SET ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if  !defined MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_UPDATE_TIME_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_UPDATE_TIME_OPT, MMISET_SET_UPDATE_TIME_CTRL_ID),
#endif
    END_WIN
};

// 自动更新时间窗口
WINDOW_TABLE( MMISET_SET_ATUO_UPDATE_TIME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetAutoUpdateTimeWindow),
    WIN_ID(MMISET_SET_AUTO_UPDATE_TIME_WIN_ID),
    WIN_TITLE( TXT_SET_AUTO_UPDATE_TIME ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_AUTO_UPDATE_TIME_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//使用说明窗口
WINDOW_TABLE( MMISET_SET_UPDATE_TIME_HELP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleUpdateTimeHelpWinMsg ),    
    WIN_ID( MMISET_SET_UPDATE_TIME_HELP_WIN_ID ),
    WIN_TITLE(TXT_SET_UPDATE_TIME_HELP),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
    CREATE_TEXT_CTRL(MMISET_SET_UPDATE_TIME_MSGBOX_CTRL_ID),
    END_WIN
}; 
#endif
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetPdaTimeLabel(BOOLEAN is_enter_from_setting)
{
    MMI_STRING_T title = {0};
    SCI_TIME_T                  sys_time = {0};
    unsigned char        time_buf[10] ={0};
    wchar       time_wchar_buf[10]={0};
    uint16      len = 0 ;
    MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_24HOURS;

    /*if (MMIAPISET_IsNeedResetDateTime())
    {
        sys_time.hour = 0;
        sys_time.min = 0;
        sys_time.sec = 0;
#ifndef WIN32
        MMIAPICOM_SetSysTime(sys_time);
#endif
    }*/
#if defined(MMI_UPDATE_TIME_SUPPORT)
    TM_GetSysTime(&sys_time);
    s_sys_time.sec = sys_time.sec;
    s_sys_time.min = sys_time.min;
    s_sys_time.hour = sys_time.hour;
#endif

    if(is_enter_from_setting)
    {
        time_type = GUISETLIST_GetCurIndex(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID);
    }
    else
    {
        time_type = MMIAPISET_GetTimeDisplayType();
    }
    if (MMISET_TIME_12HOURS == time_type)
    {
        MMIAPISET_GetTimeStr12(time_buf);
    }
    else
    {
        MMIAPISET_GetTimeStr24(time_buf);
    }
    len =strlen((char*)time_buf);
    MMI_STRNTOWSTR(time_wchar_buf, len, time_buf, len, len);
    title.wstr_len = len;
    title.wstr_ptr = time_wchar_buf; 
    GUILABEL_SetText(MMISET_SET_TIME_EDIT_CTRL_ID, &title, FALSE);
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void UpdateStbDispTime(BOOLEAN is_enter_from_setting)
{
    //MMI_STRING_T        title = {0};
    SCI_TIME_T          sys_time = {0};
    unsigned char       time_buf[10] ={0};
    wchar               time_wchar_buf[10]={0};
    uint16              len = 0 ;

    MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_24HOURS;

    /*if (MMIAPISET_IsNeedResetDateTime())
    {
        sys_time.hour = 0;
        sys_time.min = 0;
        sys_time.sec = 0;
#ifndef WIN32
        MMIAPICOM_SetSysTime(sys_time);
#endif
    }*/
#if defined(MMI_UPDATE_TIME_SUPPORT)
    TM_GetSysTime(&sys_time);
    s_sys_time.sec = sys_time.sec;
    s_sys_time.min = sys_time.min;
    s_sys_time.hour = sys_time.hour;
#endif

    if(is_enter_from_setting)
    {
        time_type = GUISETLIST_GetCurIndex(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID);
    }
    else
    {
        time_type = MMIAPISET_GetTimeDisplayType();
    }
    if (MMISET_TIME_12HOURS == time_type)
    {
        MMIAPISET_GetTimeStr12(time_buf);
    }
    else
    {
        MMIAPISET_GetTimeStr24(time_buf);
    }
    len =strlen((char*)time_buf);
    MMI_STRNTOWSTR(time_wchar_buf, len, time_buf, len, len);

    if(MAIN_SetStbDispTimeinfo(time_wchar_buf,len))
    {
        GUIWIN_UpdateStb();
    }
}
/*****************************************************************************/
//  Description : set time setting param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetTimeSettingParam(void)
{
    MMI_STRING_T title = {0};
    GUIFORM_SetStyle(MMISET_SET_TIME_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMI_PDA_SUPPORT
    SetPdaTimeLabel(FALSE);
#endif
    MMI_GetLabelTextByLang(TXT_SET_TIME, &title);
    GUILABEL_SetText(MMISET_SET_TIME_LABEL_CTRL_ID, &title, FALSE);
#ifndef MMI_PDA_SUPPORT    
	SCI_TRACE_LOW("[mmiset_datetime.c]  SetTimeSettingParam");
    DisplaySystemTime(MMISET_SET_TIME_EDIT_CTRL_ID,FALSE);
#endif
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetPdaDateLabel(BOOLEAN is_enter_from_setting)
{
    MMI_STRING_T title = {0};
    char        date_buf[20] ={0};
    wchar       date_wchar_buf[20]={0};
    SCI_DATE_T                  sys_date = {0};
    MMISET_DATE_DISPLAY_TYPE_E   date_type = MMISET_DATE_YMD;
   
    /*if (MMIAPISET_IsNeedResetDateTime())
    {
		sys_date.mday = 1;
        sys_date.mon = 1;
        sys_date.wday = 2;
        sys_date.year = 1980;
#ifndef WIN32
        MMIAPICOM_SetSysData(sys_date);
        //SCI_TRACE_LOW:"DisplaySystemDate sys_date.year = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_642_112_2_18_2_50_47_130,(uint8*)"d",sys_date.year);
#endif
    }*/

        //get current system date
     TM_GetSysDate(&sys_date);

    
#if defined(MMI_UPDATE_TIME_SUPPORT)
    s_sys_date.mday = sys_date.mday;
    s_sys_date.mon = sys_date.mon;
    s_sys_date.wday = sys_date.wday;
    s_sys_date.year = sys_date.year;
#endif
	if(is_enter_from_setting)
    {
        date_type = GUISETLIST_GetCurIndex(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID);
    }
    else
    {
        date_type = MMIAPISET_GetDateDisplayType();
    }
    switch (date_type)
    {
    case MMISET_DATE_YMD:
        sprintf(date_buf, "%04d.%02d.%02d",sys_date.year, sys_date.mon, sys_date.mday);
        break;
        
    case MMISET_DATE_MDY:
        sprintf(date_buf, "%02d.%02d.%04d",sys_date.mon, sys_date.mday, sys_date.year);
        break;
        
    case MMISET_DATE_DMY:
        sprintf(date_buf, "%02d.%02d.%04d",sys_date.mday, sys_date.mon, sys_date.year);
        break;
        
    default:
        sprintf(date_buf, "%04d.%02d.%02d",sys_date.year, sys_date.mon, sys_date.mday);
        break;
    }
    
    MMI_STRNTOWSTR(date_wchar_buf, 10, date_buf, 10, 10);/*lint !e64*/
    title.wstr_len = 10;
    title.wstr_ptr = date_wchar_buf; 
    GUILABEL_SetText(MMISET_SET_DATE_EDIT_CTRL_ID, &title, FALSE);
}
#endif
/*****************************************************************************/
//  Description : set date setting param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetDateSettingParam(void)
{
    MMI_STRING_T title = {0};
     GUIFORM_SetStyle(MMISET_SET_DATE_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMI_PDA_SUPPORT
     SetPdaDateLabel(FALSE);
#endif

    MMI_GetLabelTextByLang(TXT_SET_DATE, &title);
    GUILABEL_SetText(MMISET_SET_DATE_LABEL_CTRL_ID, &title, FALSE);
#ifndef MMI_PDA_SUPPORT
    DisplaySystemDate(MMISET_SET_DATE_EDIT_CTRL_ID,FALSE);
   // GUIEDIT_SetFont(MMISET_SET_DATE_EDIT_CTRL_ID,MMI_DEFAULT_BIG_FONT);//for cr 116713 maryxiao
#endif
}

/*****************************************************************************/
//  Description : set time display param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetTimeDisplayTypeParam(void)
{
    MMI_STRING_T title = {0};
    uint32 loop = 0;
    MMISET_TIME_DISPLAY_TYPE_E time_display_type = MMISET_TIME_12HOURS;
    uint32 text_id[MMISET_MAX_TIME_DISPLAY_TYPE]=
    {
        TXT_COMM_12_HOURS,
        TXT_COMM_24_HOURS
    };
  
   // GUIFORM_SetStyle(MMISET_SET_TIME_DISPLAY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMISET_SET_TIME_FORMAT_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_TIME_DISPLAY_TYPE, &title);
    GUILABEL_SetText(MMISET_SET_TIME_DISPLAY_LABEL_CTRL_ID, &title, FALSE);
#if  !defined MMI_GUI_STYLE_TYPICAL
    	GUISETLIST_SetFontAlign(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID,ALIGN_LEFT);
#endif
    for(loop=0;loop<MMISET_MAX_TIME_DISPLAY_TYPE;loop++)
    {
        GUISETLIST_AddItemById(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID, text_id[loop]);
    }
    time_display_type = MMIAPISET_GetTimeDisplayType();
    GUISETLIST_SetCtrlState(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID, time_display_type);
    GUISETLIST_SetTitleTextId(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID,TXT_SET_TIME_DISPLAY_TYPE);   
}

/*****************************************************************************/
//  Description : set date display param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetDateDisplayTypeParam(void)
{
    MMI_STRING_T title = {0};
    uint32 loop = 0;
    MMISET_DATE_DISPLAY_TYPE_E      date_display_type= MMISET_DATE_YMD;
    uint32 text_id[MMISET_DATE_DISPLAY_TYPE_MAX]=
    {
        TXT_SET_DATE_YMD,
        TXT_SET_DATE_MDY,
        TXT_SET_DATE_DMY
    };
  
   // GUIFORM_SetStyle(MMISET_SET_DATA_DISPLAY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMISET_SET_DATE_FORMAT_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_DATE_DISPLAY_TYPE, &title);
    GUILABEL_SetText(MMISET_SET_DATE_DISPLAY_LABEL_CTRL_ID, &title, FALSE);
#if  !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID,ALIGN_LEFT);
#endif  
    for(loop=0;loop<MMISET_DATE_DISPLAY_TYPE_MAX;loop++)
    {
        GUISETLIST_AddItemById(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID, text_id[loop]);
    }
    date_display_type = MMIAPISET_GetDateDisplayType();
    GUISETLIST_SetCtrlState(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID, date_display_type);
    GUISETLIST_SetTitleTextId(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID,TXT_SET_DATE_DISPLAY_TYPE);   
}
/*****************************************************************************/
//  Description : save time date setting
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveTimeDatesetting(void)
{
    MMISET_DATE_DISPLAY_TYPE_E type = MMISET_DATE_YMD;
    uint16            cur_selection = 0;
    //设置日期格式
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID);    
    if (0 == cur_selection)
    {
        type = MMISET_DATE_YMD;
    }
    else if (1 == cur_selection)
    {
        type = MMISET_DATE_MDY;
    }
    else
    {
        type = MMISET_DATE_DMY;
    }         
    SetDateDisplayType(type);    
    //设置时间格式
    MMISET_SetSelectTimeDisplyType(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID);   
    //保存时间
#ifndef MMI_PDA_SUPPORT
	SCI_TRACE_LOW("[mmiset_datetime.c] SaveTimeDatesetting");
    SetTime(MMISET_SET_TIME_EDIT_CTRL_ID);
#endif
    MAIN_SetStbDispTime();
    //保存日期
#ifndef MMI_PDA_SUPPORT
    SetDate(MMISET_SET_DATE_EDIT_CTRL_ID);
#endif
    //更新闹钟
    MMIAPISET_UpdateAlarmSet();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示，刷新时间
    MMISUB_UpdateDisplaySubLcd(); 
#endif
    RefreshKeyboardLight();
}

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
//  Description : Set auto update time label menu name 
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SetAutLabelParam(void)
{
    MMI_STRING_T aut_displayname = {0};
       
    GUIFORM_SetStyle(MMISET_SET_UPDATE_TIME_CTRL_ID,GUIFORM_STYLE_UNIT);

#ifdef MMI_SNTP_SUPPORT //support manual update
    MMI_GetLabelTextByLang(TXT_SET_UPDATE_TIME_SET, &aut_displayname);
    GUILABEL_SetText(MMISET_AUTO_UPDATE_TIME_LABEL_CTRL_ID, &aut_displayname, FALSE);
#else    
    {
        MMI_STRING_T text_str  = {0};

        MMI_GetLabelTextByLang(TXT_SET_AUTO_UPDATE_TIME, &aut_displayname);
        GUILABEL_SetText(MMISET_AUTO_UPDATE_TIME_LABEL_CTRL_ID, &aut_displayname, FALSE);
        //text 1
        if(MMIAPISET_GetIsAutoUpdateTime()) 
        {
            MMI_GetLabelTextByLang(TXT_OPEN, &text_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_CLOSE, &text_str);
        }
        //auto update time setting
        GUILABEL_SetText(MMISET_SET_AUTO_UPDATE_TIME_CTRL_ID, &text_str, TRUE);
    }
#endif
}

/*****************************************************************************/
//  Description : get if time modified when exit set time&date window 
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTimeModified(MMI_CTRL_ID_T ctrl_id)
{
    uint8           hour        =   0;
    uint8           minute      =   0;
    BOOLEAN         result      = FALSE;
    
    //get time
    GUIEDIT_GetTime(ctrl_id,&hour,&minute,PNULL);
       
    if((s_sys_time.hour != hour) || (s_sys_time.min != minute))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    SCI_MEMSET(&s_sys_time,0x00,sizeof(SCI_TIME_T));
    
    return result;
}


/*****************************************************************************/
//  Description : get if date modified when exit set time&date window 
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsDateModified(MMI_CTRL_ID_T ctrl_id)
{
    uint8           month       =   0;
    uint8           day         =   0;
    uint16          year        =   0;
    BOOLEAN         result      = TRUE;
    
    //get date
    GUIEDIT_GetDate(ctrl_id,&year,&month,&day);
    
       
    if((s_sys_date.year != year)||(s_sys_date.mon != month) || (s_sys_date.mday != day))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    SCI_MEMSET(&s_sys_date,0x00,sizeof(SCI_DATE_T));
    
    return result;
}
#if  !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : Init Pda Update Time Settings Ctrl
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaUpdateTimeSettingsCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_UPDATE_TIME_CTRL_ID;
    MMI_STRING_T text_str  = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUILIST_POS_INFO_T pos_info = {0};

    GUILIST_GetCurPosInfo(ctrl_id, &pos_info);
   
    //text 1
    GUILIST_SetMaxItem(ctrl_id, 3, FALSE );
    if(MMIAPISET_GetIsAutoUpdateTime()) 
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    //auto update time setting
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_AUTO_UPDATE_TIME,text_str,image_id,ID_SET_AUTO_UPDATE_TIME);

    //manual update menu
    AppendPdaSettingsItem1Line(ctrl_id,TXT_SET_UPDATE_SYSTEM_TIME,ID_SET_UPDATE_SYSTEM_TIME);

    //help
    AppendPdaSettingsItem1Line(ctrl_id,TXT_SET_UPDATE_TIME_HELP,ID_SET_UPDATE_TIME_HELP);

    GUILIST_SetCurPosInfo(ctrl_id, &pos_info);
}
#endif
#ifdef MMI_SNTP_SUPPORT //support manual update
/*****************************************************************************/
//  Description : to handle update time window message
//  Global resource dependence : 
//  Author:Dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetUpdateTimeWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T         group_id    =   0;
#endif
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISET_SET_UPDATE_TIME_CTRL_ID;
    uint16  index  = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        s_is_user_update_time = TRUE;

        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_FULL_PAINT:
    #if  !defined MMI_GUI_STYLE_TYPICAL
        InitPdaUpdateTimeSettingsCtrl();
    #endif
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    #if !defined MMI_GUI_STYLE_TYPICAL
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);//get user data
    #else
        GUIMENU_GetId(MMISET_SET_UPDATE_TIME_CTRL_ID,&group_id,&menu_id);
    #endif
        switch (menu_id)
        {
            case ID_SET_AUTO_UPDATE_TIME:/*自动更新时间设置*/
    #if !defined MMI_GUI_STYLE_TYPICAL
                /*打开时,弹出提示窗口:开机和联网的时候会自动更新时间*/
                MMISET_OpenAutoUpdateTimeQueryWin(!MMIAPISET_GetIsAutoUpdateTime());/*lint !e730*/
    #else            
                MMK_CreateWin((uint32*)MMISET_SET_ATUO_UPDATE_TIME_WIN_TAB,PNULL);           
    #endif
                break;
            case ID_SET_UPDATE_SYSTEM_TIME:/*启动手动更新时间*/
				SCI_TRACE_LOW("[mmiset_datetime.c] ID_SET_UPDATE_SYSTEM_TIME update system time by manual");
                MMIAPIAUT_StartManualUpdateSystemTime(); 
                break;
            case ID_SET_UPDATE_TIME_HELP:
                MMK_CreateWin((uint32 *)MMISET_SET_UPDATE_TIME_HELP_WIN_TAB, NULL);
                break;
                    
            default:
                break;
        }
        break;
 
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
    case MSG_CLOSE_WINDOW:
         s_is_user_update_time = FALSE;
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;  
}

/*****************************************************************************/
//  Description :处理打开自动更新时间确认窗口消息函数
//  Global resource dependence : current select item id
//  Author:Dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenAutoUpdateQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //first enable auto update time flag ,then execute manual update system time
        MMIAPISET_SetIsAutoUpdateTime(TRUE);//enable the flag

        //IAPIWCLK_OpenMainWin();
        MMIPUB_OpenAlertSuccessWin(TXT_OPEN_AUTO_UPDATE_TIME_PROMPT);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
#endif

/*****************************************************************************/
//  Description :打开自动更新时间确认窗口
//  Global resource dependence : current select item id
//  Author:Dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void MMISET_OpenAutoUpdateTimeQueryWin(BOOLEAN is_auto_update_time)
{
    //MMI_WIN_ID_T query_win_id = MMISET_ACTIVE_AUTO_UPDATE_QUERY_WIN_ID;
#ifdef MMI_SNTP_SUPPORT //support manual update
    MMI_HANDLE_T  auto_update_win_id = MMISET_SET_UPDATE_TIME_WIN_ID;
#else
    MMI_HANDLE_T  auto_update_win_id = MMISET_SET_TIME_DATE_WIN_ID;
#endif
    
    if(is_auto_update_time)
    {
        //MMIIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_OPEN_AUTO_UPDATE_TIME_QUERY,
        //                            IMAGE_PUBWIN_QUERY, &query_win_id,
        //                            HandleOpenAutoUpdateQueryWindow, (uint32)NULL);
        MMIAPISET_SetIsAutoUpdateTime(TRUE);//enable the flag
        
        if(MMIAPIAUT_GetIsNeedUpdateTimeByNet())
        {
            if(MMIAPIPHONE_SetSysTimeWithNitzAndTickTime())
            {
                //update status bar even if it does not popup success
                MMIAPISET_ShowUpdateTimeMsgWin(TRUE,FALSE);
                
                /*重置update_time flag,防止频繁更新时间*/
                MMIAPIAUT_SetIsNeedUpdateTimeByNet(FALSE); 
            }
            else
            {
#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
                MMIAPIPHONE_SetAttachNitzTimeFinish(FALSE);  //set fasle temp
                MMIAPIPHONE_StartGetNitzTime();
#endif				
            }
        }
        else
        {
            MMIPUB_OpenAlertSuccessWin(TXT_OPEN_AUTO_UPDATE_TIME_PROMPT);
        }
    }
    else
    {
        MMIAPISET_SetIsAutoUpdateTime(FALSE);
        //refresh menu list
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SET_AUTO_UPDATE_TIME,TXT_SIM_DEACTIVATED,
         //                                       IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    MMK_SendMsg(auto_update_win_id, MSG_FULL_PAINT, PNULL);

}

/*****************************************************************************/
//  Description : 处理自动更新时间
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoUpdateTimeWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_AUTO_UPDATE_TIME_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMISET_AppendOpenCloseListItem(MMISET_SET_AUTO_UPDATE_TIME_CTRL_ID,MMIAPISET_GetIsAutoUpdateTime);
        MMK_SetAtvCtrl(win_id,MMISET_SET_AUTO_UPDATE_TIME_CTRL_ID);
        break;
	
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(MMISET_SET_AUTO_UPDATE_TIME_CTRL_ID, &cur_selection, 1);
        
        /*打开时,弹出提示窗口:开机和联网的时候会自动更新时间*/
        MMISET_OpenAutoUpdateTimeQueryWin(0 == cur_selection ? TRUE:FALSE);

        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : 处理打开设置时间/日期窗口时，是否关闭"自动更新时间"功能询问窗口
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCloseAutoUpdateQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //关闭自动更新时间功能,打开对应窗口(时间/日期设置窗口)
        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        
        MMIAPISET_SetIsAutoUpdateTime(FALSE);//disable the flag
        if ((win_info_ptr != PNULL) && ((uint32 *)(win_info_ptr->user_data) != PNULL))
        {
#if defined(MMI_PDA_SUPPORT)
        MMK_CreatePubFormWin((uint32 *)(win_info_ptr->user_data), PNULL);
#else
            MMK_CreateWin((uint32 *)(win_info_ptr->user_data), PNULL);
#endif
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if (MMIAPISET_IsNeedResetDateTime())
        {
            /*取消对DateTime的Reset,因此需要设置DateTime为出厂值,待从网络更新时间*/
            MMIAPISET_SetNeedResetDateTime(FALSE);
            MMIAPISET_RestoreDateTimeFactorySetting();
        }
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : to handle update time help function,display the help infor
//	Global resource dependence : 
//   Author:  dave.ruan
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleUpdateTimeHelpWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T help_string_ptr = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMI_GetLabelTextByLang(TXT_SET_UPDATE_TIME_HELP_STR, &help_string_ptr);
        GUITEXT_SetString(MMISET_SET_UPDATE_TIME_MSGBOX_CTRL_ID,help_string_ptr.wstr_ptr,help_string_ptr.wstr_len,FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_SET_UPDATE_TIME_MSGBOX_CTRL_ID); 
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
// 	Description : 更新时间结束后提示用户
//	Global resource dependence : 
//    Author: dave.ruan
//	Note: 手动更新成功会提示，自动更新成功不提示
/*****************************************************************************/
PUBLIC void MMIAPISET_ShowUpdateTimeMsgWin(BOOLEAN is_success, BOOLEAN is_manual_update)
{
    MMI_WIN_ID_T result_win_id = MMISET_UPDATE_TIME_RESULT_WIN_ID;
    MMI_WIN_ID_T wait_win_id  = MMISET_UPDATE_SYSTEM_TIME_WAITING_WIN_ID;

    if (is_success)
    {
        /*提示时间已更新*/
#if !defined MMI_GUI_STYLE_TYPICAL
        if (is_manual_update ||MMK_IsOpenWin(wait_win_id))
#endif        
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SET_DTAE_AND_TIME_UPDATED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&result_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        
        /*关闭等待窗口*/
        MMIAPIAUT_CloseManualUpdateWaitWin();
       
        //更新成功后不需要reset;防止处于idle时被flash UI 冲掉
        if(MMIAPISET_IsNeedResetDateTime())
        {
            MMIAPISET_SetNeedResetDateTime(FALSE);
        }
        MMIIDLE_SendMsgToIdleWin( MSG_IDLE_UPDATE_DATETIME, PNULL);//cr132485
        //  更新状态栏的时间
        MAIN_SetStbDispTime();//update status bar
        //  更新表盘日期
#ifdef DYNAMIC_WATCHFACE_SUPPORT
        WATCHFACE_UpdateDate();
#endif
        /*if in time&date setting window reset the ctrl id display*/
        MMIAPISET_RefreshTimeDateEditCtrl();
    }
    else
    {
        /*关闭等待窗口*/
        MMIAPIAUT_CloseManualUpdateWaitWin();
    }
}

/*****************************************************************************/
// 	Description : 获取关闭自动更新窗口的id
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_GetCloseAutoUpdateQueryWinID(void)
{
    MMI_WIN_ID_T query_win_id = MMISET_CLOSE_AUTO_UPDATE_QUERY_WIN_ID;

    return query_win_id;
}

/*****************************************************************************/
// 	Description : refresh time date edit ctrl id after time date update 
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_RefreshTimeDateEditCtrl(void)
{
    if(MMK_IsOpenWin(MMISET_SET_TIME_DATE_WIN_ID)) 
    {
#ifdef MMI_PDA_SUPPORT  
        SetPdaTimeLabel(FALSE);
        SetPdaDateLabel(FALSE);
#else
		SCI_TRACE_LOW("[mmiset_datetime.c] MMIAPISET_RefreshTimeDateEditCtrl");
        DisplaySystemTime(MMISET_SET_TIME_EDIT_CTRL_ID,TRUE);//CR181282
        DisplaySystemDate(MMISET_SET_DATE_EDIT_CTRL_ID,TRUE);//CR181282
    //    GUIEDIT_SetFont(MMISET_SET_DATE_EDIT_CTRL_ID,MMI_DEFAULT_BIG_FONT);//for cr 116713 maryxiao
#endif
    }

}
#endif/*MMI_UPDATE_TIME_SUPPORT*/


/*****************************************************************************/
//  Description :在时间日期界面修改时间显示风格时，当前界面的time edit同时更新
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void ChangeTimeEditDispaly(uint8 index)
{
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    
    switch(index)
    {
        case 0:
            time_style = GUIEDIT_TIME_STYLE_12;
            break;

        case 1:
            time_style = GUIEDIT_TIME_STYLE_24;
            break;

        default:
            break;
    }
#ifdef MMI_PDA_SUPPORT
    SetPdaTimeLabel(TRUE);
#else
    GUIEDIT_SetTimeStyle(MMISET_SET_TIME_EDIT_CTRL_ID,PNULL,&time_style,PNULL,TRUE);
#endif
}
/*****************************************************************************/
//  Description :在时间日期界面修改日期显示风格时，当前界面的date同时更新
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void ChangeDateEditDispaly(uint8 index)
{
    GUIEDIT_DATE_STYLE_E date_style = GUIEDIT_DATE_STYLE_NULL;

    switch(index)
    {
        case 0:
            date_style = GUIEDIT_DATE_STYLE_YMD;
            break;

        case 1:
            date_style = GUIEDIT_DATE_STYLE_MDY;
            break;

        case 2:
            date_style = GUIEDIT_DATE_STYLE_DMY;
            break;
            
        default:
            break;
    }
#ifdef MMI_PDA_SUPPORT
    SetPdaDateLabel(TRUE);
#else
    GUIEDIT_SetDateStyle(MMISET_SET_DATE_EDIT_CTRL_ID, PNULL, &date_style, TRUE);
#endif
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : handle idle tips timer msg
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/

LOCAL void HandleTimeRefreshTimer(
                               uint8    timer_id,
                               uint32   param
                               )
{
    if (s_time_refresh_timer_id == timer_id)
    {
        MMK_SendMsg(MMISET_SET_TIME_DATE_WIN_ID, MSG_FULL_PAINT,PNULL);
    }
    s_time_refresh_timer_id = MMK_CreateTimerCallback(100,
                            HandleTimeRefreshTimer,
                            PNULL,
                            FALSE);
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 设置状态栏日期
//  Global resource dependence : 
//  Author:wei.ren
//  Note: 
/*****************************************************************************/
LOCAL void UpdateStbDispDate(void)
{
#ifdef PDA_UI_DROPDOWN_WIN
    SCI_DATE_T                  sys_date                        ={0};
    uint16                      date_wstr_len                   = 0;
    uint32                      cur_selection                   = 0;
    char                        sym_str[2]                      ={0};
    GUIWIN_STBDATA_TEXT_T       date_info                       ={0};
    uint8                       date_str[MMIIDLE_DATE_LEN]      ={0};
    uint16                      date_wstr_str[MMIIDLE_DATE_LEN] ={0};
    
    sym_str[0] = '-';
    
    TM_GetSysDate(&sys_date);

    if(!(1 <= sys_date.mon && 12 >= sys_date.mon))
    {
        //SCI_TRACE_LOW:"Waring, UpdateStbDispDate: month = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_1392_112_2_18_2_50_48_131,(uint8*)"d", sys_date.mon);
        return ;
    }
    if(!(1 <= sys_date.mday && 31 >= sys_date.mday))
    {
        //SCI_TRACE_LOW:"Waring,UpdateStbDispDate: date = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_1397_112_2_18_2_50_48_132,(uint8*)"d", sys_date.mday);
        return ;
    }
    
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID);

    switch(cur_selection)
    {
        case MMISET_DATE_YMD:
            sprintf((char*) date_str, "%04d%s%02d%s%02d", sys_date.year, sym_str,sys_date.mon, sym_str, sys_date.mday);
            break;
        case MMISET_DATE_MDY:
            sprintf((char*) date_str, "%02d%s%02d%s%04d", sys_date.mon, sym_str, sys_date.mday, sym_str, sys_date.year);
            break;
        case MMISET_DATE_DMY:
            sprintf((char*) date_str, "%02d%s%02d%s%04d", sys_date.mday, sym_str, sys_date.mon, sym_str, sys_date.year);
            break;
        default:
            sprintf((char*) date_str, "%04d%s%02d%s%02d", sys_date.year, sym_str,sys_date.mon, sym_str, sys_date.mday);
            break;
    }
 
    date_wstr_len           = strlen((char*)date_str);
    date_wstr_len           = MIN(date_wstr_len, MMIIDLE_DATE_LEN);

    MMI_STRNTOWSTR(date_wstr_str,MMIIDLE_DATE_LEN, date_str,MMIIDLE_DATE_LEN, date_wstr_len);

    date_info.font_color   = MMI_GRAY_WHITE_COLOR;
    date_info.align        = ALIGN_BOTTOM; 
    date_info.font_type    = MMI_STATUS_FONT_TYPE;
    date_info.is_display   = TRUE;
    date_info.wstr_len     = MMIAPICOM_Wstrlen(date_wstr_str);
    
    SCI_MEMCPY(date_info.wstr_ptr, date_wstr_str, 
        sizeof(wchar)*MIN(date_info.wstr_len, MMIIDLE_DATE_LEN));
    
    GUIWIN_SetStbItemText(MMI_WIN_TEXT_DATE, &date_info);
#endif

}
#endif
/*****************************************************************************/
//  Description : to handle time and date setting window message
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeDateWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;  
    MMI_CTRL_ID_T               ctrl_id     =   MMISET_SET_TIME_EDIT_CTRL_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMISET_SET_TIME_DATE_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        #ifndef MMI_PDA_SUPPORT
		//fixed by hongjun.jia for  NEWMS00205395,
        GUIEDIT_SetAlign(MMISET_SET_TIME_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMISET_SET_DATE_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        #endif
#endif     
#ifndef MMI_PDA_SUPPORT
		GUIFORM_SetCircularHandleUpDown(MMISET_SET_TIME_DATE_FORM_CTRL_ID,TRUE);
#endif
        SetTimeDisplayTypeParam();
        SetTimeSettingParam();
        SetDateSettingParam();
        SetDateDisplayTypeParam();
#if defined(MMI_UPDATE_TIME_SUPPORT)
        SetAutLabelParam();
#endif      
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id); 
#ifdef MMI_PDA_SUPPORT              
#if !defined MMI_GUI_STYLE_TYPICAL
        s_time_refresh_timer_id = MMK_CreateTimerCallback(100,
                                HandleTimeRefreshTimer,
                                PNULL,
                                FALSE);
#endif
#endif                                
        GUIWIN_SetTitleTextId(win_id,TXT_SET_TIME_DATE,FALSE);
        break;
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_GET_FOCUS:
        s_time_refresh_timer_id = MMK_CreateTimerCallback(100,
                                HandleTimeRefreshTimer,
                                PNULL,
                                FALSE);
        break;
#endif
#endif        
    case MSG_FULL_PAINT:
#if defined(MMI_UPDATE_TIME_SUPPORT)
        SetAutLabelParam();
#endif 
#ifdef MMI_PDA_SUPPORT
        SetPdaTimeLabel(TRUE);    
        SetPdaDateLabel(TRUE);
        UpdateStbDispTime(TRUE);
        UpdateStbDispDate();
#endif

        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
        if(MSG_CTL_PENOK == msg_id)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
        switch (ctrl_id)
        {
//#ifdef MMI_PDA_SUPPORT
        case MMISET_SET_TIME_EDIT_CTRL_ID:
            MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_TIME_WIN_TAB, TRUE);
            break;
            
        case MMISET_SET_DATE_EDIT_CTRL_ID:
            MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_DATE_WIN_TAB, TRUE);
            break;
//#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
            //更新时间设置
        case MMISET_AUTO_UPDATE_TIME_LABEL_CTRL_ID:
#ifdef MMI_SNTP_SUPPORT //support manual update
            MMK_CreateWin((uint32*)MMISET_SET_UPDATE_TIME_WIN_TAB,PNULL); 
#else
#if !defined MMI_GUI_STYLE_TYPICAL
            /*打开时,弹出提示窗口:联网的时候会自动更新时间*/
            MMISET_OpenAutoUpdateTimeQueryWin(!MMIAPISET_GetIsAutoUpdateTime());/*lint !e730*/
#else            
            MMK_CreateWin((uint32*)MMISET_SET_ATUO_UPDATE_TIME_WIN_TAB,PNULL);           
#endif
#endif            
            break;
#endif 
          

        default:
            break;
        }
        break;  
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#if defined(MMI_UPDATE_TIME_SUPPORT)
#ifndef MMI_PDA_SUPPORT
        if (MMIAPISET_GetIsAutoUpdateTime()&&(IsTimeModified(MMISET_SET_TIME_EDIT_CTRL_ID) || IsDateModified(MMISET_SET_DATE_EDIT_CTRL_ID)))//提示用户选择:是否关闭自动更新时间功能
        {
            MMIPUB_OpenQueryWinByTextId(TXT_SET_MODIFY_DATE_TIME_TIMEZONE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
        }
#endif
#endif  /*MMI_UPDATE_TIME_SUPPORT*/

#ifndef MMI_PDA_SUPPORT /* 非PDA 关窗口时保存时间 */
        SaveTimeDatesetting();
        if (MMIAPISET_IsNeedResetDateTime())
        {
            MMIAPISET_SetNeedResetDateTime(FALSE);
        }  
#endif
        MMK_CloseWin(win_id);
        break;
      
    case MSG_CTL_SETLIST_SWITCH:
        {
            uint8  time_index = 0;
            uint8  date_index = 0;
            time_index = GUISETLIST_GetCurIndex(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID);
            date_index = GUISETLIST_GetCurIndex(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID);
            ChangeTimeEditDispaly(time_index);
            ChangeDateEditDispaly(date_index);
        }
        break;
#if defined(MMI_UPDATE_TIME_SUPPORT)
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        SCI_TRACE_LOW("HandleSetTimeDateWindow SetIsAutoUpdateTime is false");
        MMIAPISET_SetIsAutoUpdateTime(FALSE);/*disable the auto update time flag*/
        
        //继续处理各菜单项
        SaveTimeDatesetting();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
#endif /*MMI_UPDATE_TIME_SUPPORT*/

    case MSG_APP_RED:
#ifdef MMI_PDA_SUPPORT
        {
            MMISET_DATE_DISPLAY_TYPE_E type = MMISET_DATE_YMD;
            uint16            cur_selection = 0;
            //设置日期格式
            cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID);    
            if (0 == cur_selection)
            {
                type = MMISET_DATE_YMD;
            }
            else if (1 == cur_selection)
            {
                type = MMISET_DATE_MDY;
            }
            else
            {
                type = MMISET_DATE_DMY;
            }         
            SetDateDisplayType(type);    
            //设置时间格式
            MMISET_SetSelectTimeDisplyType(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID);  
        }
#endif
        recode = MMI_RESULT_FALSE;
        break;
        
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_LOSE_FOCUS:
        {
            if (0 <s_time_refresh_timer_id)
            {
                MMK_StopTimer(s_time_refresh_timer_id);
                s_time_refresh_timer_id = 0;
            }
            
            SaveTimeDatesetting();//crNEWMS00180799
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            if (0 <s_time_refresh_timer_id)
            {
                MMK_StopTimer(s_time_refresh_timer_id);
                s_time_refresh_timer_id = 0;
            }
            SaveTimeDatesetting();
        }
        break;
#endif
#endif        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}


/*****************************************************************************/
//  Description : 等待窗口的处理函数
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeWaitingWinMsg(
                                               MMI_WIN_ID_T win_id,            
                                               MMI_MESSAGE_ID_E msg_id,        
                                               DPARAM param
                                               )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    if (MSG_SET_UPDATE_ALARMSET == msg_id)
    {
        MMIAPISET_UpdateAlarmSet();
        MMK_PostMsg(MMISET_SET_TIME_WIN_ID, MSG_SET_UPDATE_ALARMSET_DONE, PNULL, 0);
    }
    else if (MSG_APP_RED != msg_id && MSG_APP_CANCEL!= msg_id)
    {
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : to handle set time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
#if defined (MMI_GUI_STYLE_TYPICAL) 
    GUI_POINT_T     point = {0};
    uint16           font_height = GUIFONT_GetHeight(MMI_SET_TIME_FONT);
#endif
#ifdef MAINLCD_SIZE_128X64 /* lint_lai */
#else   
#if defined (MMI_GUI_STYLE_TYPICAL) 
    GUI_RECT_T      client_rect = MMITHEME_GetClientRect();
#endif
//#ifndef MMI_PHONESET_MINI_SUPPORT
    uint16          icon_width = 0;     /*lint !e529 */
    uint16          icon_height = 0;    /*lint !e529 */
//#endif
#if defined (MMI_GUI_STYLE_TYPICAL) 
    GUI_BOTH_RECT_T  both_rect = {0};
    GUI_BOTH_RECT_T  both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif
#endif 
    BOOLEAN    is_enter_from_setting  = (BOOLEAN)(uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
       icon_width = 0;     /*lint !e529 */
       icon_height = 0;    /*lint !e529 */
//#if defined MMI_GUI_STYLE_TYPICAL
#ifdef MAINLCD_SIZE_128X64 //zss modify begin

#else     
    #if defined (MMI_GUI_STYLE_TYPICAL)
        #ifdef MMI_PHONESET_MINI_SUPPORT
        both_rect.h_rect.left = both_rect.v_rect.left = MMI_TIME_EDIT_LEFT;
        both_rect.h_rect.top = (both_client_rect.h_rect.bottom - font_height) / 2 + both_client_rect.h_rect.top;
        both_rect.h_rect.bottom = both_rect.h_rect.top + font_height;
        both_rect.h_rect.right = both_client_rect.h_rect.right - MMI_DATE_EDIT_LEFT;
        both_rect.v_rect.top = (both_client_rect.v_rect.bottom - font_height) / 2 + both_client_rect.v_rect.top;
        both_rect.v_rect.bottom = both_rect.v_rect.top + font_height;
        both_rect.v_rect.right = both_client_rect.v_rect.right - MMI_DATE_EDIT_LEFT;
        GUIAPICTRL_SetBothRect(MMISET_SET_DATE_DATEEDITBOX_CTRL_ID, &both_rect);//yongsheng.wang modify
        #else
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height,IMAGE_SET_TIME_ICON, win_id);
        both_rect.h_rect.left = both_rect.v_rect.left = MMI_TIME_EDIT_LEFT;
		if(both_client_rect.h_rect.bottom >both_client_rect.h_rect.top + icon_height + font_height)
		{
			both_rect.h_rect.top = (both_client_rect.h_rect.bottom - both_client_rect.h_rect.top-  icon_height - font_height)/3 + both_client_rect.h_rect.top;
		}
		else
		{
			both_rect.h_rect.top = both_client_rect.h_rect.top;
		}
        both_rect.h_rect.bottom = both_rect.h_rect.top + font_height;
        both_rect.h_rect.right = both_client_rect.h_rect.right - MMI_TIME_EDIT_LEFT;
		if(both_client_rect.v_rect.bottom > both_client_rect.v_rect.top +  icon_height + font_height)
		{
			both_rect.v_rect.top = (both_client_rect.v_rect.bottom - both_client_rect.v_rect.top-  icon_height - font_height)/3 + both_client_rect.v_rect.top;
		}
		else
		{
			both_rect.v_rect.top = both_client_rect.v_rect.top;
		}
        both_rect.v_rect.bottom = both_rect.v_rect.top + font_height;
        both_rect.v_rect.right = both_client_rect.v_rect.right - MMI_TIME_EDIT_LEFT;
        GUIAPICTRL_SetBothRect(MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID, &both_rect);
        #endif
    #else
        GUIFORM_IsSlide(MMISET_TIME_FORM_CTRL_ID,FALSE);
    #endif
#endif //zss modify end   

//#else
        MMK_SetAtvCtrl(win_id,MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID);

        DisplaySystemTime(MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID,is_enter_from_setting);
        break;
        
    case MSG_FULL_PAINT:
//#ifdef MMI_GUI_STYLE_TYPICAL

#ifdef MAINLCD_SIZE_128X64 //zss modify begin

#else       
    #if defined (MMI_GUI_STYLE_TYPICAL) //arvin zhang add for setDate window display bg_img and ctrl
        #ifdef MMI_PHONESET_MINI_SUPPORT
		point.x = client_rect.left + (client_rect.right ) /2;
		if(client_rect.bottom > client_rect.top + font_height)
		{
			point.y = (client_rect.bottom - client_rect.top - font_height)/2 + client_rect.top + font_height + 1;
		}
		else
		{
			point.y = client_rect.top + font_height + 1;
		}
		SetTimeDateBackground(point,PNULL,win_id);
        #else
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height,IMAGE_SET_TIME_ICON, win_id);
		point.x = client_rect.left + (client_rect.right - icon_width) /2;
		if(client_rect.bottom > client_rect.top + icon_height + font_height)
		{
			point.y = (client_rect.bottom - client_rect.top- icon_height - font_height)/2 + client_rect.top + font_height + 1;
		}
		else
		{
			point.y = client_rect.top + font_height + 1;
		}
		SetTimeDateBackground(point,IMAGE_SET_TIME_ICON,win_id);
	#endif
	#endif
#endif //zss modify end
        
//#endif
        break;       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:

        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if (SetTime(MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID))//time is valid
                {
                    //  更新状态栏的时间
                    MAIN_SetStbDispTime();
                    MMIAPISET_UpdateAlarmSet();
                    MMK_PostMsg(win_id, MSG_SET_UPDATE_ALARMSET_DONE, PNULL, 0);
                }
                else
                {            
                    MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
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
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

        if (SetTime(MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID))//time is valid
        {
            //  更新状态栏的时间
            MAIN_SetStbDispTime();
            MMIAPISET_UpdateAlarmSet();
            MMK_PostMsg(win_id, MSG_SET_UPDATE_ALARMSET_DONE, PNULL, 0);
        }
        else
        {            
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
        }
        
        break;
        
    case MSG_SET_UPDATE_ALARMSET_DONE:
        {
            if (MMIAPISET_IsNeedResetDateTime())
            {  
                MMIAPISET_SetNeedResetDateTime(FALSE);
            }
            MMK_CloseWin(win_id); 
#ifndef SUBLCD_SIZE_NONE            
            //刷新小屏的显示，刷新时间
            MMISUB_UpdateDisplaySubLcd(); 
#endif
            RefreshKeyboardLight();
#ifdef MMI_PDA_SUPPORT
            SetPdaTimeLabel(TRUE);
#endif
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);      
        break;
        
    case MSG_CLOSE_WINDOW:     
        //  更新状态栏的时间
        MAIN_SetStbDispTime();
        if (MMIAPISET_IsNeedResetDateTime())
        {  
            MMIAPISET_SetNeedResetDateTime(FALSE);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 设置输入时间或者日期窗口的背景色，image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetTimeDateBackground(
                                 GUI_POINT_T dis_point,
                                 uint32    icon_id,
                                 MMI_WIN_ID_T win_id
                                 )
{

#ifdef  MAINLCD_SIZE_128X64     
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T bg_rect = {0, 0, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT};

    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);

#else    
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T          bg_rect         =   MMITHEME_GetClientRect();
    MMI_IMAGE_ID_T      bg_id           =   0;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

#if defined MAINLCD_SIZE_240X320 //cr254220
    bg_id        = MMITHEME_GetCommonLightBg().img_id;//IMAGE_COMMON_BG_LIGHT;
#else
    bg_id       = IMAGE_COMMON_BG;
#endif

    GUIRES_DisplayImg(PNULL,
        &bg_rect,
        &bg_rect,
        win_id,
        bg_id,
        &lcd_dev_info);
#ifndef MMI_PHONESET_MINI_SUPPORT
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        icon_id,
        &lcd_dev_info);
#endif
#endif          
}

/*****************************************************************************/
//  Description : 设置时间时，显示系统时间
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySystemTime(
                             MMI_CTRL_ID_T     ctrl_id,
                             BOOLEAN is_enter_from_setting//是否由设置菜单进入时间设置窗口
                             )
{
    SCI_TIME_T                  sys_time = {0};
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    //get time type
    if(is_enter_from_setting)
    {
        time_type = GUISETLIST_GetCurIndex(MMISET_SET_TIME_DISPLAY_SETLIST_CTRL_ID);
    }
    else
    {
        time_type = MMIAPISET_GetTimeDisplayType();
    }
    if (MMISET_TIME_12HOURS == time_type)
    {
        time_style = GUIEDIT_TIME_STYLE_12;
    }
    else
    {
        time_style = GUIEDIT_TIME_STYLE_24;
    }
    
    //set time display style

    GUIEDIT_SetTimeStyle(MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
#ifndef MMI_PDA_SUPPORT
	SCI_TRACE_LOW("[mmiset_datetime.c] DisplaySystemTime");
    GUIEDIT_SetTimeStyle(MMISET_SET_TIME_EDIT_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
#endif
  //  GUIEDIT_SetFont(MMISET_SET_TIME_TIMEEDITBOX_CTRL_ID, MMI_SET_TIME_FONT);//for cr 116713 maryxiao

    
    /*if (MMIAPISET_IsNeedResetDateTime())
    {
        sys_time.hour = 0;
        sys_time.min = 0;
        sys_time.sec = 0;
#ifndef WIN32
        MMIAPICOM_SetSysTime(sys_time);
#else	//WIN32
        TM_GetSysTime(&sys_time);
#endif
    }
    else*/
    {    
        //get current system time
        TM_GetSysTime(&sys_time);
    }
    
#if defined(MMI_UPDATE_TIME_SUPPORT)
    s_sys_time.sec = sys_time.sec;
    s_sys_time.min = sys_time.min;
    s_sys_time.hour = sys_time.hour;
#endif

    GUIEDIT_SetTime(ctrl_id,sys_time.hour,sys_time.min,0);
}

/*****************************************************************************/
//  Description : 设置输入的时间为系统时间
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTime(
                      MMI_CTRL_ID_T     ctrl_id
                      )
{
    uint8           hour        =   0;
    uint8           minute      =   0;
    BOOLEAN         result      =   TRUE;
    MMISET_TIME_T   time        =   {0};
    SCI_TIME_T      sys_time    =   {0};

	//get time
    GUIEDIT_GetTime(ctrl_id,&hour,&minute,PNULL);
#ifdef MMI_PDA_SUPPORT
    SetPdaTimeLabel(TRUE);
#else
	SCI_TRACE_LOW("[mmiset_datetime.c] SetTime");
    GUIEDIT_SetTime(MMISET_SET_TIME_EDIT_CTRL_ID, hour, minute, 0);
#endif

    time.time_hour = hour;
    time.time_minute = minute;
    
    //check time if is valid
    if (MMIAPICOM_CheckTimeIsValid(time))
    {
		sys_time.sec = 0;
        sys_time.min = minute;
        sys_time.hour = hour;
        MMIAPICOM_SetSysTime(sys_time);
        //        UpdateAlarmSet();
	}
    else
    {
        //SCI_TRACE_LOW:"SetTime:hour is %d,minute is %d is invalid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_1995_112_2_18_2_50_49_133,(uint8*)"dd",hour,minute);
        result = FALSE;
    }   
    return (result);
}
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : 等待窗口的处理函数
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDateWaitingWinMsg(
                                               MMI_WIN_ID_T win_id,            
                                               MMI_MESSAGE_ID_E msg_id,        
                                               DPARAM param
                                               )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    if (MSG_SET_UPDATE_ALARMSET == msg_id)
    {
        
        MMIAPISET_UpdateAlarmSet();
        MMK_PostMsg(MMISET_SET_DATE_WIN_ID, MSG_SET_UPDATE_ALARMSET_DONE, PNULL, 0);
    }
    else if (MSG_APP_RED != msg_id && MSG_APP_CANCEL!= msg_id)
    {
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    }
    
    return (result);
}
#endif
/*****************************************************************************/
//  Description : to handle set date
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDateWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
#if defined (MMI_GUI_STYLE_TYPICAL) 
    GUI_POINT_T     point = {0};
    uint16           font_height = GUIFONT_GetHeight(MMI_SET_DATE_FONT);
#endif
#ifdef MAINLCD_SIZE_128X64 /* lint_lai */
#else   
#if defined (MMI_GUI_STYLE_TYPICAL) 
    GUI_RECT_T      client_rect = MMITHEME_GetClientRect();
#endif
//#ifndef MMI_PHONESET_MINI_SUPPORT
    uint16          icon_width = 0;     /*lint !e529 */
    uint16          icon_height = 0;    /*lint !e529 */
//#endif
#if defined (MMI_GUI_STYLE_TYPICAL) 
    GUI_BOTH_RECT_T  both_rect = {0};
    GUI_BOTH_RECT_T  both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif 
#endif 
    BOOLEAN    is_enter_from_setting  = (BOOLEAN)(uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        icon_width = 0;     /*lint !e529 */
        icon_height = 0;    /*lint !e529 */
//#if defined MMI_GUI_STYLE_TYPICAL
#ifdef MAINLCD_SIZE_128X64 //zss modify begin

#else     
    #if defined (MMI_GUI_STYLE_TYPICAL) 
        #ifdef MMI_PHONESET_MINI_SUPPORT
        both_rect.h_rect.left = both_rect.v_rect.left = MMI_TIME_EDIT_LEFT;
        both_rect.h_rect.top = (both_client_rect.h_rect.bottom - font_height) / 2 + both_client_rect.h_rect.top;
        both_rect.h_rect.bottom = both_rect.h_rect.top + font_height;
        both_rect.h_rect.right = both_client_rect.h_rect.right - MMI_DATE_EDIT_LEFT;
        both_rect.v_rect.top = (both_client_rect.v_rect.bottom - font_height) / 2 + both_client_rect.v_rect.top;
        both_rect.v_rect.bottom = both_rect.v_rect.top + font_height;
        both_rect.v_rect.right = both_client_rect.v_rect.right - MMI_DATE_EDIT_LEFT;
        GUIAPICTRL_SetBothRect(MMISET_SET_DATE_DATEEDITBOX_CTRL_ID, &both_rect);//yongsheng.wang modify
        #else
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height,IMAGE_SET_DATE_ICON, win_id);
        both_rect.h_rect.left = both_rect.v_rect.left = MMI_TIME_EDIT_LEFT;
        both_rect.h_rect.top = (both_client_rect.h_rect.bottom -  icon_height - font_height)/3 + both_client_rect.h_rect.top;
        both_rect.h_rect.bottom = both_rect.h_rect.top + font_height;
        both_rect.h_rect.right = both_client_rect.h_rect.right - MMI_DATE_EDIT_LEFT;
        both_rect.v_rect.top = (both_client_rect.v_rect.bottom -  icon_height - font_height)/3 + both_client_rect.v_rect.top;
        both_rect.v_rect.bottom = both_rect.v_rect.top + font_height;
        both_rect.v_rect.right = both_client_rect.v_rect.right - MMI_DATE_EDIT_LEFT;
        GUIAPICTRL_SetBothRect(MMISET_SET_DATE_DATEEDITBOX_CTRL_ID, &both_rect);
        #endif
    #else
        GUIFORM_IsSlide(MMISET_DATE_FORM_CTRL_ID,FALSE);
    #endif
#endif   
//#else
        MMK_SetAtvCtrl(win_id,MMISET_SET_DATE_DATEEDITBOX_CTRL_ID);
        DisplaySystemDate(MMISET_SET_DATE_DATEEDITBOX_CTRL_ID,is_enter_from_setting);
        break;
        
    case MSG_FULL_PAINT:
//#if !defined MMI_GUI_STYLE_TYPICAL
//#else//not the pda support
#ifdef MAINLCD_SIZE_128X64 //zss modify begin

#else   
    #if defined (MMI_GUI_STYLE_TYPICAL) 
        #ifdef MMI_PHONESET_MINI_SUPPORT
		point.x = client_rect.left + (client_rect.right ) /2;
		if(client_rect.bottom > client_rect.top + font_height)
		{
			point.y = (client_rect.bottom - client_rect.top - font_height)/2 + client_rect.top + font_height + 1;
		}
		else
		{
			point.y = client_rect.top + font_height + 1;
		}
		SetTimeDateBackground(point,PNULL,win_id);
        #else
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height,IMAGE_SET_DATE_ICON, win_id);
        point.x = client_rect.left + (client_rect.right - icon_width) /2 ;
        point.y = (client_rect.bottom -  icon_height - font_height)/2 + client_rect.top + font_height + 1;
        SetTimeDateBackground(point,IMAGE_SET_DATE_ICON,win_id);
        #endif
    #endif
#endif //zss modify end     
       
//#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if (SetDate(MMISET_SET_DATE_DATEEDITBOX_CTRL_ID))//date is valid
                {
                    if (!MMIAPISET_IsNeedResetDateTime())
                    {
                        MMIAPISET_UpdateAlarmSet();
                        MMK_PostMsg(win_id, MSG_SET_UPDATE_ALARMSET_DONE, PNULL, 0);
                    }   
                    else
                    {
                        MMK_CloseWin(win_id);
                    }
            
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
            
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
        if (SetDate(MMISET_SET_DATE_DATEEDITBOX_CTRL_ID))//date is valid
        {
            if (!MMIAPISET_IsNeedResetDateTime())
            {
                MMIAPISET_UpdateAlarmSet();
                MMK_PostMsg(win_id, MSG_SET_UPDATE_ALARMSET_DONE, PNULL, 0);
            }   
            else
            {
                MMK_CloseWin(win_id);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
            
        }
        
        break;
        
    case MSG_SET_UPDATE_ALARMSET_DONE:
        {
#ifdef MMI_PDA_SUPPORT
            SetPdaDateLabel(TRUE);
#endif 
            MMK_CloseWin(win_id);
#ifndef SUBLCD_SIZE_NONE
            //刷新小屏的显示，刷新时间
            MMISUB_UpdateDisplaySubLcd(); 
#endif            
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);      
        break;
        
    case MSG_CLOSE_WINDOW:
        if (MMIAPISET_IsNeedResetDateTime())
        {
            MMIAPISET_SetTime();
#ifdef MMI_PDA_SUPPORT
            MMIAPISET_SetNeedResetDateTime(FALSE);
#endif
        }  
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 设置日期时，显示系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySystemDate(
                             MMI_CTRL_ID_T     ctrl_id,
                             BOOLEAN   is_enter_from_setting//是否从设置菜单进入设置日期窗口
                             )
{
    SCI_DATE_T                  sys_date = {0};
    GUIEDIT_DATE_STYLE_E        date_style = GUIEDIT_DATE_STYLE_NULL;
    MMISET_DATE_DISPLAY_TYPE_E  date_type = MMISET_DATE_YMD;

	if(is_enter_from_setting)
    {
        date_type = GUISETLIST_GetCurIndex(MMISET_SET_DATE_DISPLAY_SETLIST_CTRL_ID);
    }
    else
    {
        date_type = MMIAPISET_GetDateDisplayType();
    }
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_2215_112_2_18_2_50_50_134,(uint8*)"d",date_type);
        break;
    }
    
    //set date style

    GUIEDIT_SetDateStyle(ctrl_id,PNULL,&date_style,FALSE);
    
    //GUIEDIT_SetFont(ctrl_id, MMI_SET_DATE_FONT);//for cr 116713 maryxiao
    
    /*if (MMIAPISET_IsNeedResetDateTime())
    {
	    sys_date.mday = 1;
        sys_date.mon = 1;
        sys_date.wday = 2;
        sys_date.year = 1980;
#ifndef WIN32
        MMIAPICOM_SetSysData(sys_date);
        //SCI_TRACE_LOW:"DisplaySystemDate sys_date.year = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_2233_112_2_18_2_50_50_135,(uint8*)"d",sys_date.year);
#endif
    }
    else*/
    {
        //get current system date
        TM_GetSysDate(&sys_date);
    }
    
#if defined(MMI_UPDATE_TIME_SUPPORT)
    s_sys_date.mday = sys_date.mday;
    s_sys_date.mon = sys_date.mon;
    s_sys_date.wday = sys_date.wday;
    s_sys_date.year = sys_date.year;
#endif

    GUIEDIT_SetDate(ctrl_id,sys_date.year,sys_date.mon,sys_date.mday);

}

/*****************************************************************************/
//  Description : 设置输入的日期为系统日期
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDate(
                      MMI_CTRL_ID_T     ctrl_id
                      )
{
    uint8           month       =   0;
    uint8           day         =   0;
    uint16          year        =   0;
    BOOLEAN         result      = TRUE;
    MMISET_DATE_T   date        =   {0};
    SCI_DATE_T      sys_date    =   {0};
    
    //get date
    GUIEDIT_GetDate(ctrl_id,&year,&month,&day);
#ifdef MMI_PDA_SUPPORT
    SetPdaDateLabel(TRUE);
#else    
    GUIEDIT_SetDate(MMISET_SET_DATE_EDIT_CTRL_ID, year, month, day);
#endif

    
    date.date_day = day;
    date.date_month = month;
    date.date_year = year;
    
    //check date if is valid
    if (MMIAPICOM_CheckDateIsValid(date))
    {
        sys_date.year = year;
        sys_date.mon = month;
        sys_date.mday = day;
        //SCI_TRACE_LOW:"SetDate sys_date.year = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_2287_112_2_18_2_50_50_136,(uint8*)"d",sys_date.year);
        //TM_SetSysDate(sys_date);
        MMIAPICOM_SetSysData(sys_date);
        //        UpdateAlarmSet();
    }
    else
    {
        //SCI_TRACE_LOW:"SetDate:year is %d,month is %d and day is %d is invalid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DATETIME_2294_112_2_18_2_50_50_137,(uint8*)"ddd",year,month,day);
        result = FALSE;
    }  
    return (result);
}


/*****************************************************************************/
//  Description : MMIAPISET_SetTime
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTime(void)
{
    MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_TIME_WIN_TAB, FALSE);
}

/*****************************************************************************/
//  Description : MMIAPISET_SetDate
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDate(void)
{
    MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_DATE_WIN_TAB, FALSE);
}


/*****************************************************************************/
//  Description : 打开时间/日期设置窗口
//                     判断"自动更新时间"功能是否开启
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenTimeOrDateWindow(uint32* win_table_ptr, BOOLEAN is_enter_from_setting)
{
#if defined(MMI_UPDATE_TIME_SUPPORT)
    MMI_WIN_ID_T query_win_id = MMISET_CLOSE_AUTO_UPDATE_QUERY_WIN_ID;
#endif/*MMI_UPDATE_TIME_SUPPORT*/

    if(PNULL == win_table_ptr)
    {
        return;//do nothing
    }
    
#if defined(MMI_UPDATE_TIME_SUPPORT)
    if (MMIAPISET_GetIsAutoUpdateTime())
    {
        if (MMIAPISET_IsNeedResetDateTime())
        {
            /*开机后需要重新设置时间*/
            /*开机自动更新时间功能打开，则不进入时间设置窗口*/
            MMIAPISET_SetNeedResetDateTime(FALSE);
//#ifndef WIN32
//            MMIAPISET_RestoreDateTimeFactorySetting();
//#endif
        }
        else
        {
            /*自动更新打开时，修改时间则需要提示用户关闭自动更新功能*/
            MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_SET_MODIFY_DATE_TIME_TIMEZONE,
                                          IMAGE_PUBWIN_QUERY, &query_win_id,
                                          HandleCloseAutoUpdateQueryWindow, (uint32)win_table_ptr);
        }
    }
    else
#endif /*MMI_UPDATE_TIME_SUPPORT*/        
    {
#ifdef MMI_PDA_SUPPORT
        MMK_CreatePubFormWin( win_table_ptr, (ADD_DATA)is_enter_from_setting);
#else
        MMK_CreateWin( win_table_ptr, (ADD_DATA)is_enter_from_setting);
#endif
    }
}

#ifdef MMISET_DATE_TIME_TYPE_BY_WIN
/*****************************************************************************/
//  Description : 时间显示格式
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTimeDisplayTypeWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        //设置列表项数
        GUILIST_SetMaxItem(MMISET_SET_TIME_DISPLAY_TYPE_CTRL_ID,2, FALSE );//max item 2
        //显示列表
        MMISET_AppendTimeDisplayTypeListItem(MMISET_SET_TIME_DISPLAY_TYPE_CTRL_ID);
        //激活控件
        MMK_SetAtvCtrl(win_id,MMISET_SET_TIME_DISPLAY_TYPE_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        //设置选中的时间显示格式为当前显示格式
        MMISET_SetSelectTimeDisplyType(MMISET_SET_TIME_DISPLAY_TYPE_CTRL_ID);       
        // 当重新设置了时间格式的时候,需要重新设置状态栏上的显示时间
        MAIN_SetStbDispTime();
        //成功
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        //关闭当前窗口
        MMK_CloseWin( win_id );
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
#endif

/*****************************************************************************/
//  Description : get the type of date display type
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_DISPLAY_TYPE_E MMIAPISET_GetDateDisplayType(void)
{
    SCI_TRACE_LOW("MMIAPISET_GetDateDisplayType %d",s_date_display_type);
    return s_date_display_type;
}
/*****************************************************************************/
//  Description : init date display type
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_InitDateDisplayType(void)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_DATE_DISPLAY_TYPE, &s_date_display_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_date_display_type= MMISET_DEFAULT_DATE_YMD;
        MMINV_WRITE(MMINV_SET_DATE_DISPLAY_TYPE, &s_date_display_type);
    }
    
}

/*****************************************************************************/
//  Description : Is User Update Time
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:TRUE or FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsUserUpdateTime(void)
{
    SCI_TRACE_LOW(" MMISET_IsUserUpdateTime s_is_user_update_time = %d ",s_is_user_update_time);
    return s_is_user_update_time;
}

/*****************************************************************************/
//  Description : 添加日期显示格式选择的radio list的item
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void AppendDateDisplayTypeListItem(
                                         MMI_CTRL_ID_T      ctrl_id
                                         )
{
    MMISET_DATE_DISPLAY_TYPE_E      date_display_type   =   MMISET_DATE_YMD;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_DATE_YMD, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_DATE_MDY, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId( TXT_SET_DATE_DMY, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    
    date_display_type = MMIAPISET_GetDateDisplayType();
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, (uint16)date_display_type, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, (uint16)date_display_type);
}
/*****************************************************************************/
//  Description : set date display type 
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void SetDateDisplayType(
                              MMISET_DATE_DISPLAY_TYPE_E date_display_type
                              )
{
    s_date_display_type = date_display_type;
    MMINV_WRITE(MMINV_SET_DATE_DISPLAY_TYPE, &s_date_display_type);
}  

#ifdef MMISET_DATE_TIME_TYPE_BY_WIN
/*****************************************************************************/
//  Description : 处理日期显示格式
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDateDisplayTypeWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:       
        //设置列表项数
        GUILIST_SetMaxItem(MMISET_SET_DATE_DISPLAY_TYPE_CTRL_ID,MMISET_DATE_DISPLAY_TYPE_MAX, FALSE );//max item 3
        //显示列表
        AppendDateDisplayTypeListItem(MMISET_SET_DATE_DISPLAY_TYPE_CTRL_ID);
        //激活控件
        MMK_SetAtvCtrl(win_id,MMISET_SET_DATE_DISPLAY_TYPE_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMISET_DATE_DISPLAY_TYPE_E type = MMISET_DATE_YMD;
            //设置选中的日期显示格式为当前显示格式
            GUILIST_GetSelectedItemIndex(MMISET_SET_DATE_DISPLAY_TYPE_CTRL_ID,&cur_selection,sizeof(cur_selection));
            
            if (0 == cur_selection)
            {
                type = MMISET_DATE_YMD;
            }
            else if (1 == cur_selection)
            {
                type = MMISET_DATE_MDY;
            }
            else
            {
                type = MMISET_DATE_DMY;
            }          
            
            SetDateDisplayType(type);       
            //成功
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            //关闭当前窗口
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
#endif
/*****************************************************************************/
//  Description :refresh keyboard time when system time changed
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void RefreshKeyboardLight(void)
{
    MMISET_KEYBOARD_LIGHT_SET_T     keyboard_light_info     =   {0};
    SCI_TIME_T                      sys_time                =   {0};
    
    MMIAPISET_GetKeyBoardLightTime(&keyboard_light_info);
    //get current system time
    TM_GetSysTime(&sys_time); 
    if (MMISET_KEYBOARD_LIGHT_CLOSE == keyboard_light_info.keyboard_type)
    {
        MMIDEFAULT_SetKeybadBackLight(FALSE);
    }
    else if (MMISET_KEYBOARD_LIGHT_AUTO == keyboard_light_info.keyboard_type 
        &&(MMISET_OPEN_KEYBOARD_LIGHT_MIN_HOUR <= sys_time.hour && sys_time.hour < MMISET_OPEN_KEYBOARD_LIGHT_MAX_HOUR))
    {
        if (MMISET_OPEN_KEYBOARD_LIGHT_MIN_HOUR == sys_time.hour && sys_time.min == 0)
        {
            MMIDEFAULT_SetKeybadBackLight(TRUE); 
        }
        else
        {
            MMIDEFAULT_SetKeybadBackLight(FALSE);
        }
    }
    else
    {
        MMIDEFAULT_SetKeybadBackLight(TRUE);
    }   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_CreateDateTimeWin(void)
{
    MMK_CreateWin((uint32*)MMISET_SET_TIME_DATE_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : set date display type 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDateDisplayType(MMISET_DATE_DISPLAY_TYPE_E date_display_type)
{
    SetDateDisplayType(date_display_type);
}


/*Edit by script, ignore 9 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
