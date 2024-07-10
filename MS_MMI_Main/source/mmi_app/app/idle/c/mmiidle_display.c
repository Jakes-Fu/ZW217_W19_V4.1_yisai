/*****************************************************************************
** File Name:      mmiidle_display.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2009        Jassmine              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"

#define _MMIIDLE_DISPLAY_C_

#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON
#include "mmi_appmsg.h"
#include "mmiidle_display.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_text.h"
#include "mmiset_shortcut.h"
#include "mmiacc_event.h"
#include "mmischedule_export.h"
#include "mmicountedtime_export.h"
#include "mmifm_export.h"
#include "mmimp3_export.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "guires.h"
#include "mmiidle_position.h"
#include "mmiidle_export.h"
#ifndef CMCC_UI_STYLE
#include "mmi_mainmenu_export.h"
#endif
#include "mmialarm_export.h"
#include "guistring.h"
#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif
#include "ui_layer.h"
#include "mmifm_export.h"
#include "mmipicview_text.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_text.h"
#endif
#ifdef MMI_WIFI_SUPPORT
#include "mmiconnection_text.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_text.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_text.h"
#endif
#include "guistatusbar.h"
#include "mmi_wallpaper_export.h"
#ifdef CMCC_UI_STYLE
#include "mmipb_export.h"
#endif
#ifdef SCREENSAVER_SUPPORT
#include "mmiss_export.h "
#endif
#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)     
#include "mmiwifi_export.h"
#include "mmiwlan_text.h"
#endif
#ifdef MCARE_V31_SUPPORT
#include "Mcare_img_id.h"
#include"Mcare_txt_id.h"
#endif
#include "mmiidle_func.h"
#include "mmistk_export.h"
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
#include "mmicc_export.h"
#endif
#include "mmienvset_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmi_timer_export.h"
#include "mmisms_export.h"
#include "mmiidle_statusbar.h"
#include "mmisms_app.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//idle font
#ifdef MAINLCD_SIZE_320X480
#define MMIIDLE_TEXT_FONT       MMI_DEFAULT_NORMAL_FONT
#elif defined(MAINLCD_SIZE_176X220)
#define MMIIDLE_TEXT_FONT       SONG_FONT_14
#else
#define MMIIDLE_TEXT_FONT       MMI_DEFAULT_BIG_FONT
#endif

#define MMIIDLE_TIPS_TEXT_FONT       MMI_DEFAULT_SMALL_FONT

#define MMIIDLE_DATE_INTERVAL       10
#define MMIIDLE_WEEK_INTERVAL       10


#define MMIDILE_TIME_RECT_TOP     (MMI_STATUSBAR_HEIGHT + 1)

#define MMIDILE_TIME_POS_X     10
#define MMIDILE_TIME_POS_Y     (MMIDILE_TIME_RECT_TOP + 2)

#define MMIDILE_PROMPT_LINE_TEXT_POS_X     30

#define MMIDILE_TIME_COLON_POS_Y    MMIDILE_TIME_POS_Y
#define MMIDILE_TIME_COLON_AM_Y     MMIDILE_TIME_POS_Y
#define MMIDILE_DATE_POS_Y          MMIDILE_TIME_POS_Y
#define MMIDILE_SOFTKEY_HEIGHT      MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT

#define MMIDILE_SHORTCUT_ITEM_WIDTH    55
#define MMIDILE_SHORTCUT_ITEM_WIDTH_H    61
#define MMIDILE_SHORTCUT_HIRIZONAL_MARGIN   10
#define MMIDILE_SHORTCUT_HIGHLIGHT_OFFSET   10

#define MMIIDLE_SHORTCUT_NUM    (5) //MAX NUM

#ifdef MAINLCD_SIZE_176X220
#define MMIIDLE_DATE_DIS_POINT  {66,MMIDILE_DATE_POS_Y}
//week
#define MMIIDLE_WEEK_DIS_POINT  {135,MMIDILE_DATE_POS_Y}
#define MMIIDLE_PROMPT_TEXT_POS_GAP 2

#define MMIIDLE_TIME_DATE_LINE_HEIGHT  24

#define MMIIDLE_LINE1_X     0
#define MMIIDLE_LINE1_Y     50

#define MMIIDLE_PROMPT_LINE_HEIGHT  24

#define MMIIDLE_SHORTCUT_FOCUS_TOP   39//24
#define MMIIDLE_SHORTCUT_FOCUS_HEIGHT   22

#elif defined MAINLCD_SIZE_240X320

#define MMIDILE_WEEK_POS_Y         30 // 42

#define MMIIDLE_DATE_DIS_POINT  {110,MMIDILE_DATE_POS_Y}
//week
#define MMIIDLE_WEEK_DIS_POINT  {204,MMIDILE_WEEK_POS_Y}
#define MMIIDLE_PROMPT_TEXT_POS_GAP 1

#define MMIIDLE_TIME_LINE_HEIGHT  30
#define MMIIDLE_DATE_LINE_HEIGHT  20

#define MMIIDLE_TIME_DATE_LINE_HEIGHT  50

#ifdef MMI_MULTI_SIM_SYS_TRI
#define MMIIDLE_PROMPT_LINE_HEIGHT  24
#else
#define MMIIDLE_PROMPT_LINE_HEIGHT  30
#endif

#define MMIIDLE_LINE1_X     0
#define MMIIDLE_LINE1_Y     50

#define MMIIDLE_SHORTCUT_FOCUS_TOP      (MMI_MAINSCREEN_HEIGHT - 40 -60)
#define MMIIDLE_SHORTCUT_FOCUS_HEIGHT   60


#elif defined MAINLCD_SIZE_128X64

#define MMIDILE_DATE_POS_Y          31//267

#define MMIIDLE_DATE_DIS_POINT  {110,MMIDILE_DATE_POS_Y}
//week
#define MMIIDLE_WEEK_DIS_POINT  {204,MMIDILE_DATE_POS_Y}
#define MMIIDLE_PROMPT_TEXT_POS_GAP 1

#define MMIIDLE_TIME_DATE_LINE_HEIGHT  24
#define MMIIDLE_PROMPT_LINE_HEIGHT 24
#define MMIIDLE_LINE1_X     0
#define MMIIDLE_LINE1_Y     50

#define MMIIDLE_SHORTCUT_FOCUS_TOP      47//29
#define MMIIDLE_SHORTCUT_FOCUS_HEIGHT   40

#elif defined MAINLCD_SIZE_240X400


#define MMIDILE_DATE_POS_Y          28//347

/*end for dcd by wz for dcd*/
#define MMIIDLE_DATE_DIS_POINT  {110,MMIDILE_DATE_POS_Y}
//week
#define MMIIDLE_WEEK_DIS_POINT  {204,MMIDILE_DATE_POS_Y}
#define MMIIDLE_PROMPT_TEXT_POS_GAP 1

#define MMIIDLE_TIME_DATE_LINE_HEIGHT  40
#define MMIIDLE_PROMPT_LINE_HEIGHT  40

#define MMIIDLE_LINE1_X     0
#define MMIIDLE_LINE1_Y     50

#define MMIIDLE_SHORTCUT_FOCUS_TOP    47//29
#define MMIIDLE_SHORTCUT_FOCUS_HEIGHT   60

#elif defined MAINLCD_SIZE_320X480

#define MMIDILE_DATE_POS_Y          40//347

/*end for dcd by wz for dcd*/
#define MMIIDLE_DATE_DIS_POINT  {185,MMIDILE_DATE_POS_Y}
//week
#define MMIIDLE_WEEK_DIS_POINT  {280,MMIDILE_DATE_POS_Y}
#define MMIIDLE_PROMPT_TEXT_POS_GAP 1
#define MMIIDLE_ICON_WIDTH      26

#define MMIIDLE_TIME_DATE_LINE_HEIGHT  40
#define MMIIDLE_PROMPT_LINE_HEIGHT  40

#define MMIIDLE_LINE1_X     0
#define MMIIDLE_LINE1_Y     50

#define MMIIDLE_SHORTCUT_FOCUS_TOP      65//57//29
#define MMIIDLE_SHORTCUT_FOCUS_HEIGHT   40

#else


#define MMIDILE_DATE_POS_Y          40//347

/*end for dcd by wz for dcd*/
#define MMIIDLE_DATE_DIS_POINT  {185,MMIDILE_DATE_POS_Y}
//week
#define MMIIDLE_WEEK_DIS_POINT  {280,MMIDILE_DATE_POS_Y}
#define MMIIDLE_PROMPT_TEXT_POS_GAP 1
#define MMIIDLE_ICON_WIDTH      26

#define MMIIDLE_TIME_DATE_LINE_HEIGHT  24
#define MMIIDLE_PROMPT_LINE_HEIGHT  40

#define MMIIDLE_LINE1_X     0
#define MMIIDLE_LINE1_Y     50
#define MMIIDLE_PROMPT_LINE_HEIGHT  24

#define MMIIDLE_SHORTCUT_FOCUS_TOP      65//57//29
#define MMIIDLE_SHORTCUT_FOCUS_HEIGHT   40

#endif

//date string max len
#define MMIIDLE_DATE_MAX_LEN    16  //2008-11-20


#define MMIIDLE_SOFTKEY_POS_Y_INTERVAL    3//2 

#define MMIIDLE_TIPS_V_SPACE        1
#define MMIIDLE_TIPS_H_SPACE        9
#define MMIIDLE_TIPS_TIMER_PERIOD   1000
//shortcut icon 
 /*lint -save -e785 */


LOCAL void EnterWebWin( void);
LOCAL void EnterWriteSMSWin( void);

LOCAL const MMIIDLE_SHORTCUT_T  s_shortcut_id[] = {
                                    {IMAGE_IDLE_SHORTCUT_MESSAGE,TXT_COMMUNICATION_MESSAGE, 
                                    IMAGE_IDLE_SHORTCUT_MESSAGE_FOCUSED, EnterWriteSMSWin},
                                    {IMAGE_IDLE_SHORTCUT_MP3,TXT_AUDIO ,
                                    IMAGE_IDLE_SHORTCUT_MP3_FOCUSED, MMIAPIMP3_OpenMp3Player},/*lint !e64*/
                                    {IMAGE_IDLE_SHORTCUT_WEB,TXT_COMMUNICATION_BROWSER, 
                                    IMAGE_IDLE_SHORTCUT_WEB_FOCUSED, EnterWebWin},
                                    {IMAGE_IDLE_SHORTCUT_ALARM,TXT_ALARM_CLOCK, 
                                    IMAGE_IDLE_SHORTCUT_ALARM_FOCUSED, MMIAPIALM_OpenMainWin},                                    
                                    {IMAGE_IDLE_SHORTCUT_PROFILES,TXT_SETTINGS_ENVIONMENT, 
                                    IMAGE_IDLE_SHORTCUT_PROFILES_FOCUSED, MMIENVSET_OpenMainMenuWindow},
                                    
                            #if 0   //def CMCC_UI_STYLE
                                    {IMAGE_IDLE_SHORTCUT_FETION,TXT_NET_FETION},    //飞信
                                    //{IMAGE_IDLE_SHORTCUT_MC,TXT_COMMON_MUSIC},      //音乐随身听
                                    {IMAGE_IDLE_SHORTCUT_CALENDAR,TXT_IDLE_CALENDAR},
                                    {IMAGE_IDLE_SHORTCUT_MAP,TXT_MOBILE_MAP},//xuefangjiang add 20101104
                            #endif
                                    };
/*lint -restore */

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//time hour : minute AM/PM
#ifdef MAINLCD_SIZE_176X220  
LOCAL const GUI_POINT_T       s_time_dis_point[] = {{2,MMIDILE_TIME_POS_Y},
                                              {14,MMIDILE_TIME_POS_Y},
                                              {26,MMIDILE_TIME_COLON_POS_Y},
                                              {32,MMIDILE_TIME_POS_Y},
                                              {44,MMIDILE_TIME_POS_Y},
                                              {56,MMIDILE_TIME_COLON_AM_Y}};
#elif defined(MAINLCD_SIZE_320X480)
LOCAL const GUI_POINT_T       s_time_dis_point[] = {{2,MMIDILE_TIME_POS_Y},
                                              {21,MMIDILE_TIME_POS_Y},
                                              {42,MMIDILE_TIME_POS_Y},
                                              {48,MMIDILE_TIME_POS_Y},
                                              {67,MMIDILE_TIME_POS_Y},
                                              {88,MMIDILE_TIME_POS_Y}};

#else
LOCAL const GUI_POINT_T       s_time_dis_point[] = {{10,MMIDILE_TIME_POS_Y},
                                              {30,MMIDILE_TIME_POS_Y},
                                              {53,MMIDILE_TIME_POS_Y},
                                              {60,MMIDILE_TIME_POS_Y},
                                              {80,MMIDILE_TIME_POS_Y},
                                              {100,MMIDILE_TIME_POS_Y}};
#endif

//line info
LOCAL MMIIDLE_LINE_INFO_T   s_line_info = {0,
                                           {MMIIDLE_LINE_NONE},
                                           MMIIDLE_LINE_SHORTCUT,
                                           MMIIDLE_SHORTCUT_FOCUS_1};

//display rect
LOCAL GUI_RECT_T        s_display_rect[MMIIDLE_TP_MAX] = {0};
LOCAL MMIIDLE_TP_TYPE_E  s_tp_down_type = MMIIDLE_TP_NONE;
//个性词语超长滚动偏移
LOCAL uint16 s_welcome_words_scroll_offset = 0;

LOCAL uint8             s_idle_tips_timer_id = 0;
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO  s_idle_tips_layer_handle = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_smart_idle_paint_layer_handle = {0,UILAYER_NULL_HANDLE};
#endif

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : display idle exception wallpaper
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySmartStyleIdle(
                                 MMI_WIN_ID_T win_id
                                 );

/*****************************************************************************/
//  Description : init smart style idle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitSmartStyleIdle(void);

/*****************************************************************************/
//  Description : get display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDisplayRect(
                                MMIIDLE_TP_TYPE_E  tp_type
                                );
LOCAL GUI_RECT_T GetTimeDateRect(void);

LOCAL GUI_RECT_T GetPromptLineRect(int32 i);
LOCAL uint32 GetPromptLineNum(void);

LOCAL GUI_RECT_T GetShortcutRect(void);

LOCAL GUI_RECT_T GetSoftkeyRect(void);
/*****************************************************************************/
//  Description : set display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetDisplayRect(
                          GUI_RECT_T           rect,
                          MMIIDLE_TP_TYPE_E    tp_type
                          );

/*****************************************************************************/
//  Description : display idle time
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTime(
                       MMI_WIN_ID_T win_id
                       );

/*****************************************************************************/
//  Description : display date
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDate(
                       MMI_WIN_ID_T win_id
                       );

/*****************************************************************************/
//  Description : display week
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWeek(void);

/*****************************************************************************/
//  Description : get shortcut num
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint32 GetShortcutNum(void);

/*****************************************************************************/
//  Description : display shortcut
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayShortCut(
                           BOOLEAN                  is_all,
                           MMI_WIN_ID_T             win_id,
                           MMIIDLE_SHORTCUT_FOCUS_E shortcut_type
                           );

/*****************************************************************************/
//  Description : get line number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLineNum(void);

/*****************************************************************************/
//  Description : get line type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIIDLE_LINE_TYPE_E GetLineType(
                                      uint16    line_index
                                      );

/*****************************************************************************/
//  Description : set line type and add line number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLineTypeAndNum(
                             MMIIDLE_LINE_TYPE_E    line_type
                             );

/*****************************************************************************/
//  Description : reset line number and type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetLineNumAndType(
                               BOOLEAN  is_show_words
                               );

/*****************************************************************************/
//  Description : get focus line type 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIIDLE_LINE_TYPE_E GetFocusLineType(
                                           MMIIDLE_SHORTCUT_FOCUS_E    *shortcut_ptr   //may PNULL
                                           );

/*****************************************************************************/
//  Description : set focus line type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFocusLineType(
                            MMIIDLE_LINE_TYPE_E         line_focus_type,//in
                            MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus  //in
                            );

/*****************************************************************************/
//  Description : get focus line index 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFocusLineIndex(void);

/*****************************************************************************/
//  Description : get line index 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLineIndex(
                          MMIIDLE_LINE_TYPE_E   line_type
                          );

/*****************************************************************************/
//  Description : display netname
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayNetname(
                          BOOLEAN               is_only_update, //是否显示指定的信息
                          MMI_WIN_ID_T          win_id,
                          MMIIDLE_LINE_TYPE_E   sim_type
                          );

/*****************************************************************************/
//  Description : display line and select image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayLineAndSelImg(
                                uint16                  line_index,
                                MMI_WIN_ID_T            win_id,
                                MMIIDLE_LINE_TYPE_E     line_type,
                                const GUI_LCD_DEV_INFO  *lcd_dev_ptr
                                );

/*****************************************************************************/
//  Description : get current item info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T GetCurItemPos(
                                uint16                  line_index, //from 0 start
                                MMIIDLE_PROMPT_POS_E    prompt_pos
                                );

LOCAL GUI_RECT_T GetCurItemRect(
                                uint16                  line_index, //from 0 start
                                MMIIDLE_PROMPT_POS_E    prompt_pos
                                );
LOCAL GUI_RECT_T GetCurItemImgDisplayRect(
                                            GUI_RECT_T              src_rect,
                                            MMI_WIN_ID_T            win_id,
                                            MMI_IMAGE_ID_T          img_id
                                            );
/*****************************************************************************/
//  Description : set prompt line display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPromptDisplayRect(
                                uint16              line_index, //from 0 start
                                MMIIDLE_TP_TYPE_E   tp_type
                                );

/*****************************************************************************/
//  Description : display schedule
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySchedule(
                           BOOLEAN          is_only_update, //是否只显示
                           MMI_WIN_ID_T     win_id
                           );

/*****************************************************************************/
//  Description : display count down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#ifdef COUNTEDTIME_SUPPORT
LOCAL BOOLEAN DisplayCountDown(
                               BOOLEAN          is_only_update, //是否只显示
                               MMI_WIN_ID_T     win_id
                               );
#endif
/*****************************************************************************/
//  Description : display mp3 name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayMp3Name(
                             BOOLEAN          is_only_update, //是否只显示
                             MMI_WIN_ID_T     win_id
                             );

/*****************************************************************************/
//  Description : display fm name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#ifdef FM_SUPPORT 
LOCAL BOOLEAN DisplayFmName(
                            BOOLEAN         is_only_update, //是否只显示
                            MMI_WIN_ID_T    win_id
                            );
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : display words
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWords(
                        BOOLEAN         is_only_update, //是否只显示
                        const wchar     *words_ptr,
                        uint16          words_len,
                        MMI_WIN_ID_T    win_id
                        );
#endif
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : display stk idle text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayStkIdleText(
                          BOOLEAN               is_only_update,
                          MMI_WIN_ID_T          win_id
                          );
#endif
/*****************************************************************************/
//  Description : display DCD
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
#ifdef MMI_ENABLE_DCD
LOCAL void DisplayDcd(
                                     BOOLEAN          is_only_update, //是否只显示
                                     MMI_WIN_ID_T     win_id
                                   );
#endif


/*****************************************************************************/
//  Description : display wlan
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)     
LOCAL void DisplayWlan(
                                     BOOLEAN          is_only_update, //是否只显示
                                     MMI_WIN_ID_T     win_id
                                   );
#endif

/*****************************************************************************/
//  Description : display softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySoftkey(MMI_WIN_ID_T    win_id, GUI_LCD_DEV_INFO const *lcd_dev_ptr);

/*****************************************************************************/
//  Description : open the focus item window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenFocusItemWin(
                            MMI_WIN_ID_T    win_id
                            );

/*****************************************************************************/
//  Description : switch focus line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SwitchFocusLine(
                           BOOLEAN          is_increase, //in
                           MMI_WIN_ID_T     win_id
                           );

/*****************************************************************************/
//  Description : switch focus shortcut icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SwitchFocusShortCut(
                               BOOLEAN          is_increase, //in
                               MMI_WIN_ID_T     win_id
                               );

/*****************************************************************************/
//  Description : reset focus to shortcut default icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetFocusToShortCut(
                                MMI_WIN_ID_T    win_id
                                );

/*****************************************************************************/
//  Description : open the prompt window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenPromptWin(
                         MMIIDLE_LINE_TYPE_E    focus_line_type,
                         MMI_WIN_ID_T           win_id
                         );
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : smart idle handle tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SmartIdleHandleTpDown(
                                 GUI_POINT_T  tp_point,
                                 MMI_WIN_ID_T win_id
                                 );

/*****************************************************************************/
//  Description : smart idle handle tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SmartIdleHandleTpUp(
                               GUI_POINT_T  tp_point,
                               MMI_WIN_ID_T win_id
                               );

/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIIDLE_TP_TYPE_E GetTpType(
                                  GUI_POINT_T   tp_point
                                  );
#endif
/*****************************************************************************/
//  Description : update point line or shortcut
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdateSmartStyleIdle(
                                BOOLEAN         is_shortcut,//is switch in shortcut
                                uint16          prev_index, //previous index,from 0
                                uint16          cur_index,  //current index,from 0
                                MMI_WIN_ID_T    win_id
                                );


/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateSmartIdlePaintLayer(
                              MMI_WIN_ID_T      win_id
                              );

/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetSmartIdlePaintLcdDev(void);

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseSmartIdlePaintLayer(void);

                             
/*****************************************************************************/
//  Description : create idle tips layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreatIdleTipsLayer(
                              MMI_WIN_ID_T      win_id
                              );

/*****************************************************************************/
//  Description : get idle tips max width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetIdleTipsMaxWidth(void);

/*****************************************************************************/
//  Description : get idle tips height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetIdleTipsHeight(void);

/*****************************************************************************/
//  Description : release idle tips layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ReleaseIdleTipsLayer(void);

/*****************************************************************************/
//  Description : display idle tips layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayIdleTipsLayer(
                                MMI_WIN_ID_T    win_id
                                );

/*****************************************************************************/
//  Description : get idle current shortcut tips text id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetIdleTipsTextId(
                                      MMIIDLE_SHORTCUT_FOCUS_E  shortcut_focus
                                      );

/*****************************************************************************/
//  Description : get idle current shortcut tips rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetIdleTipsRect(
                                 uint16                     str_width,
                                 MMIIDLE_SHORTCUT_FOCUS_E   shortcut_focus
                                 );

/*****************************************************************************/
//  Description : start idle tips timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartIdleTipsTimer(void);

/*****************************************************************************/
//  Description : idle tips timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopIdleTipsTimer(void);

/*****************************************************************************/
//  Description : handle idle tips timer msg
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIdleTipsTimer(
                               uint8    timer_id,
                               uint32   param
                               );

/*****************************************************************************/
//  Description : hide idle tips
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HideIdleTipsLayer(void);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
LOCAL uint8  GetTimeLineOffset( void)
{
    MMISET_IDLE_DISPLAY_T*  idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
    uint8                   time_line_offset = 0;
    
    if (idle_set_ptr->visible_flag.item_flag.is_show_time_and_date)
    {
        time_line_offset = 1;
    }
    
    return time_line_offset;
}

/*****************************************************************************/
//  Description : to enter web win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterWebWin( void)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = PNULL;
    entry_param.user_agent_ptr = PNULL;
    
    MMIAPIBROWSER_Entry(&entry_param);
#endif
}

LOCAL void EnterWriteSMSWin( void)
{
    //send sms
    MMIAPISMS_ClearDestNum();
    MMIAPISMS_WriteNewMessage(MMI_DUAL_SYS_MAX, PNULL, PNULL, PNULL );
}
/*****************************************************************************/
//  Description : display idle exception wallpaper
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySmartStyleIdle(
                                 MMI_WIN_ID_T win_id
                                 )
{
    MMISET_IDLE_DISPLAY_T* idle_set_ptr = 0;
    uint32 max_line = GetPromptLineNum();


 

#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_smart_idle_paint_layer_handle))
    {
        UILAYER_Clear(&s_smart_idle_paint_layer_handle);
    }
#endif
    //reset focus line 
    if ( MMIIDLE_LINE_NONE == s_line_info.line_focus_type)
    {
        SetFocusLineType(MMIIDLE_LINE_SHORTCUT,MMIIDLE_SHORTCUT_FOCUS_1);
        //MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
    }

    //get display setting info
    idle_set_ptr = MMIAPISET_GetIdleDisplayMode();

    //init
    InitSmartStyleIdle();

#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD))
    //set line num and type are 0
    ResetLineNumAndType(idle_set_ptr->visible_flag.item_flag.is_show_idle_words);
#endif
#endif

    if (idle_set_ptr->visible_flag.item_flag.is_show_time_and_date)
    {     
        if(MMIIDLE_LINE_TIME == GetFocusLineType(0))
        {
            GUI_RECT_T rect = GetTimeDateRect();
            LCD_FillRoundedRect(GetSmartIdlePaintLcdDev(),  rect, rect,
            MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
        }
        //display time
        DisplayTime(win_id);
        //display date
        DisplayDate(win_id);
        //display week
        DisplayWeek();

        //add line num
        SetLineTypeAndNum(MMIIDLE_LINE_TIME);
    }
    
    if (idle_set_ptr->visible_flag.item_flag.is_show_network_name 
                && GetLineNum() < max_line)
    {
        //display net name
        DisplayNetname(FALSE,win_id,MMIIDLE_LINE_SIM1);
    }
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL                                                             
    if (idle_set_ptr->visible_flag.item_flag.is_show_network_name2
                && GetLineNum() < max_line)
    {
        //display net name
        DisplayNetname(FALSE,win_id,MMIIDLE_LINE_SIM2);
    }  
#endif
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD                                                          
    if (idle_set_ptr->visible_flag.item_flag.is_show_network_name3
                && GetLineNum() < max_line)
    {
        //display net name
        DisplayNetname(FALSE,win_id,MMIIDLE_LINE_SIM3);
    }  
#endif
#if  defined MMI_MULTI_SIM_SYS_QUAD                                                           
    if (idle_set_ptr->visible_flag.item_flag.is_show_network_name4
                && GetLineNum() < max_line)
    {
        //display net name
        DisplayNetname(FALSE,win_id,MMIIDLE_LINE_SIM4);
    }
#endif
    
    //display schedule
    if (idle_set_ptr->visible_flag.item_flag.is_show_today_schedule
        &&GetLineNum() < max_line )
    {
        DisplaySchedule(FALSE,win_id);
    }
    
#ifdef COUNTEDTIME_SUPPORT
    if (GetLineNum() < max_line && idle_set_ptr->visible_flag.item_flag.is_show_countdown)
    {
        DisplayCountDown(FALSE,win_id);
    }
#endif

    if(GetLineNum() < max_line )
    {
#ifdef MMI_AUDIO_PLAYER_SUPPORT         
        //display mp3 or kuro
        if (MMIAPIMP3_IsPlayerActive() 
            && idle_set_ptr->visible_flag.item_flag.is_show_mp3)
        {
#ifdef FM_SUPPORT 
            if ((FM_PLAYING == MMIAPIFM_GetStatus()) 
                && idle_set_ptr->visible_flag.item_flag.is_show_fm)
            {
                //display fm
                DisplayFmName(FALSE,win_id);
            }
            else
#endif
            {
                DisplayMp3Name(FALSE,win_id);
            }
          
        }  
        else
#endif 
        {
#ifdef FM_SUPPORT  
            if ((FM_STOP != MMIAPIFM_GetStatus()) 
                && idle_set_ptr->visible_flag.item_flag.is_show_fm)
            {
                //display fm
                DisplayFmName(FALSE,win_id);
            }
#endif
        }
    }
    
    //display words
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT 
    if (idle_set_ptr->visible_flag.item_flag.is_show_idle_words && GetLineNum() < max_line )
    {
        DisplayWords(FALSE,idle_set_ptr->idle_words,idle_set_ptr->wstr_len,win_id);
    }
#endif	
#ifdef MMI_IDLE_STKTEXT_SUPPORT
    if (idle_set_ptr->visible_flag.item_flag.is_show_stk_idle_text  && GetLineNum() < max_line )
    {
        //display stk idle text
        if (GetLineNum() < max_line)
        {
            DisplayStkIdleText(FALSE, win_id);
        }
    }
#endif
    
    //added by @jun.hu, 2009/7/23
#ifdef MMI_ENABLE_DCD
    {
        if (MMIAPIDCD_IsIdleDisplay())
        {
            DisplayDcd(FALSE,win_id);
        }
    }
#endif

    //display wlan
#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)     
    if (idle_set_ptr->visible_flag.item_flag.is_show_wlan_option
        &&!MMIAPISET_GetFlyMode()
        &&GetLineNum() < max_line )
    {
        DisplayWlan(FALSE,win_id);
    }
#endif

    //display shortcut
    DisplayShortCut(TRUE,win_id,MMIIDLE_SHORTCUT_FOCUS_1);

    //display softkey
    DisplaySoftkey(win_id, GetSmartIdlePaintLcdDev());   
}

/*****************************************************************************/
//  Description : init smart style idle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitSmartStyleIdle(void)
{
    SCI_MEMSET(s_display_rect,0,sizeof(s_display_rect));
}

/*****************************************************************************/
//  Description : get display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDisplayRect(
                                MMIIDLE_TP_TYPE_E  tp_type
                                )
{
    GUI_RECT_T error_point = {0};
    if ((MMIIDLE_TP_NONE >= tp_type) || (MMIIDLE_TP_MAX <= tp_type))
    {
        //SCI_TRACE_LOW:"GetDisplayRect ERROR! tp_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_966_112_2_18_2_23_59_31,(uint8*)"d",tp_type);
        return error_point;
    }
    else
    {
        return (s_display_rect[tp_type]);
    }
}

/*****************************************************************************/
//  Description : get time date rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTimeDateRect(void)
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    rect.top =  MMIDILE_TIME_RECT_TOP;
    rect.bottom = rect.top + MMIIDLE_TIME_DATE_LINE_HEIGHT;
    return rect;   
}

/*****************************************************************************/
//  Description : get prompt line rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetPromptLineRect(int32 i)
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();    
    int16          y_start_offset = GetTimeDateRect().bottom + 1;
    
    MMISET_IDLE_DISPLAY_T * idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
    
    if (idle_set_ptr->visible_flag.item_flag.is_show_time_and_date)
    {
        y_start_offset = GetTimeDateRect().bottom + 1;
        i--;
    }
    else
    {
        y_start_offset = GetTimeDateRect().top;
    }
    rect.top =  y_start_offset + MMIIDLE_PROMPT_LINE_HEIGHT*i;
    rect.bottom = rect.top + MMIIDLE_PROMPT_LINE_HEIGHT;
    return rect;   
}

/*****************************************************************************/
//  Description : get prompt line rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPromptLineNum(void)
{
    uint32 line_num = 0;
    GUI_RECT_T time_rect = GetTimeDateRect();   
    GUI_RECT_T shortcut_rect =  GetShortcutRect();
    MMISET_IDLE_DISPLAY_T * idle_set_ptr = MMIAPISET_GetIdleDisplayMode();

    if (idle_set_ptr->visible_flag.item_flag.is_show_time_and_date)
    {
        line_num = 1 + (shortcut_rect.top - time_rect.bottom) / MMIIDLE_PROMPT_LINE_HEIGHT;
    }
    else
    {
        line_num = (shortcut_rect.top - time_rect.top) / MMIIDLE_PROMPT_LINE_HEIGHT;
    }
    return line_num ;
}

/*****************************************************************************/
//  Description : get shortcut rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetShortcutRect(void)
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();   
    rect.bottom = GetSoftkeyRect().top + 1;
    rect.top = rect.bottom - MMIIDLE_SHORTCUT_FOCUS_HEIGHT;
    return rect;   
}

/*****************************************************************************/
//  Description : get softkey rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSoftkeyRect(void)
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();   
    rect.top = rect.bottom - MMIDILE_SOFTKEY_HEIGHT - 8;
    return rect;   
}
/*****************************************************************************/
//  Description : set display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetDisplayRect(
                          GUI_RECT_T           rect,
                          MMIIDLE_TP_TYPE_E    tp_type
                          )
{
    if ((MMIIDLE_TP_NONE >= tp_type) || (MMIIDLE_TP_MAX <= tp_type))
    {
        //SCI_TRACE_LOW:"SetDisplayRect ERROR! tp_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_986_112_2_18_2_23_59_32,(uint8*)"d",tp_type);
        return;
    }
    else
    {
        //set rect
        s_display_rect[tp_type].left   = rect.left;
        s_display_rect[tp_type].top    = rect.top;
        s_display_rect[tp_type].right  = rect.right;
        s_display_rect[tp_type].bottom = rect.bottom;
    }
}

/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTime(
                       MMI_WIN_ID_T win_id
                       )
{
    BOOLEAN                     is_12hour       = FALSE;
    BOOLEAN                     is_pm           = FALSE;
    uint16                      img_width       = 0;
    uint16                      img_height      = 0;
    //int16                       offset          = 0;
    uint32                      i               = 0;
    SCI_TIME_T                  sys_time        = {0};
    GUI_RECT_T                  dis_rect        = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info     = *GetSmartIdlePaintLcdDev();
    MMISET_TIME_DISPLAY_TYPE_E  time_type       = MMISET_TIME_12HOURS;
    MMI_IMAGE_ID_T              last_icon_id    = IMAGE_NULL;
    //GUI_RECT_T                  time_date_rect   = GetTimeDateRect();
    //get system time0
    TM_GetSysTime(&sys_time);

    //get time type
    time_type = MMIAPISET_GetTimeDisplayType();
    if (MMISET_TIME_12HOURS == time_type)
    {
        is_12hour = TRUE;
        if ((MMISET_TIME_PM_HOUR > sys_time.hour))//AM
        {
            if ((MMISET_TIME_1_HOUR > sys_time.hour))//00:00-1:00
            {
                sys_time.hour = (uint8)(sys_time.hour + MMISET_TIME_PM_HOUR);
            }
        }
        else//PM
        {
            is_pm = TRUE;
            if ((MMISET_TIME_PM_HOUR <= sys_time.hour) && (MMISET_TIME_13_HOUR > sys_time.hour))//12:00-13:00
            {
            }
            else
            {
                sys_time.hour = (uint8)(sys_time.hour-MMISET_TIME_PM_HOUR);
            }
        }
    }

    
    //get image width and height
    GUIRES_GetImgWidthHeight(&img_width,&img_height,IMAGE_IDLE_TIME_0,win_id);    
   
    //display time hour
    GUIRES_DisplayImg((GUI_POINT_T *)&s_time_dis_point[i++],PNULL,PNULL,win_id,(IMAGE_IDLE_TIME_0+sys_time.hour/10),&lcd_dev_info);
    GUIRES_DisplayImg((GUI_POINT_T *)&s_time_dis_point[i++],PNULL,PNULL,win_id,(IMAGE_IDLE_TIME_0+sys_time.hour%10),&lcd_dev_info);
    //display time colon
    GUIRES_DisplayImg((GUI_POINT_T *)&s_time_dis_point[i++],PNULL,PNULL,win_id,IMAGE_IDLE_TIME_COLON,&lcd_dev_info);
    //display time minute
    GUIRES_DisplayImg((GUI_POINT_T *)&s_time_dis_point[i++],PNULL,PNULL,win_id,(IMAGE_IDLE_TIME_0+sys_time.min/10),&lcd_dev_info);
    last_icon_id = (IMAGE_IDLE_TIME_0+sys_time.min%10);
    GUIRES_DisplayImg((GUI_POINT_T *)&s_time_dis_point[i++],PNULL,PNULL,win_id,last_icon_id,&lcd_dev_info);

    if (is_12hour)
    {
        //display Am/Pm
        if (is_pm)
        {
            last_icon_id = IMAGE_IDLE_TIME_PM;
        }
        else
        {
            last_icon_id = IMAGE_IDLE_TIME_AM;
        }
        GUIRES_DisplayImg((GUI_POINT_T *)&s_time_dis_point[i++],PNULL,PNULL,win_id,last_icon_id,&lcd_dev_info);
    }


    //set display rect
    dis_rect.left   = s_time_dis_point[0].x;
    dis_rect.top    = s_time_dis_point[0].y;
    dis_rect.right  = s_time_dis_point[i-1].x + img_width - 1;
    dis_rect.bottom = s_time_dis_point[i-1].y + img_height - 1;

    //set time rect
    SetDisplayRect(dis_rect,MMIIDLE_TP_TIME);
}




/*****************************************************************************/
//  Description : display date
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDate(
                       MMI_WIN_ID_T win_id
                       )
{
    uint8                       data_str[MMIIDLE_DATE_MAX_LEN]  = {0};
    wchar                       data_wstr[MMIIDLE_DATE_MAX_LEN] = {0};
    uint16                      data_wstr_len                   = 0;
    uint16                      str_width                       = 0;
    GUI_RECT_T                  dis_rect                        = {0};
    GUI_LCD_DEV_INFO             *lcd_dev_ptr                    = GetSmartIdlePaintLcdDev();

    //get system date
    MMIAPISET_GetCurrentDateStr(data_str,'-',MMIIDLE_DATE_MAX_LEN);

    data_wstr_len = strlen((char*)data_str);

    if(0 == data_wstr_len)
    {
        //SCI_TRACE_LOW:"DisplayDate ERROR! data_wstr_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_1117_112_2_18_2_23_59_33,(uint8*)"d",data_wstr_len);
        return;
    }

    //convert string
    MMI_STRNTOWSTR(data_wstr,MMIIDLE_DATE_MAX_LEN,
        data_str,MMIIDLE_DATE_MAX_LEN,
        data_wstr_len);
        
    //set display rect
    dis_rect = GetTimeDateRect();   
    //dis_rect = GetDisplayRect(MMIIDLE_TP_TIME);

    dis_rect.top =  GetDisplayRect(MMIIDLE_TP_TIME).bottom + 2;
    dis_rect.left = GetDisplayRect(MMIIDLE_TP_TIME).left;    
    
    str_width = MMIIDLE_DisplayLineTextInRect(
        lcd_dev_ptr,
        &dis_rect,
        SONG_FONT_17,//SONG_FONT_12
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_DATE),
        data_wstr, 
        data_wstr_len
        );    
    

    dis_rect.right  = dis_rect.left + str_width - 1;
    //set date rect
    SetDisplayRect(dis_rect,MMIIDLE_TP_DATE);
}

/*****************************************************************************/
//  Description : display week
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWeek(void)
{
    uint16              str_width   = 0;
    SCI_DATE_T          sys_date    = {0};
    GUI_RECT_T          dis_rect    = {0};
    MMI_STRING_T        week_str    = {0};
    MMI_TEXT_ID_T       week_text[] = {TXT_IDLE_SHORT_SUNDAY,
                                       TXT_IDLE_SHORT_MONDAY,
                                       TXT_IDLE_SHORT_TUESDAY,
                                       TXT_IDLE_SHORT_WEDNESDAY,
                                       TXT_IDLE_SHORT_THURSDAY,
                                       TXT_IDLE_SHORT_FRIDAY,
                                       TXT_IDLE_SHORT_SATURDAY};
    GUI_LCD_DEV_INFO *lcd_dev_ptr = GetSmartIdlePaintLcdDev();

    //get system date
    TM_GetSysDate(&sys_date);

    //get week str
    MMI_GetLabelTextByLang(week_text[sys_date.wday],&week_str);

    dis_rect =  GetDisplayRect(MMIIDLE_TP_DATE);

    dis_rect.left = dis_rect.right + MMIIDLE_WEEK_INTERVAL;    
    dis_rect.right = GetTimeDateRect().right;
    
    str_width = MMIIDLE_DisplayLineTextInRect(
        lcd_dev_ptr,
        &dis_rect,
        SONG_FONT_17,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_WEEK),
        week_str.wstr_ptr, 
        week_str.wstr_len
        );

    dis_rect.right  = dis_rect.left + str_width - 1;

    //set week rect
    SetDisplayRect(dis_rect,MMIIDLE_TP_WEEK);
}

/*****************************************************************************/
//  Description : get shortcut num
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint32 GetShortcutitemWidth(void)
{
    uint32 width = 0;
    if(MMITHEME_IsMainScreenLandscape())
    {
        width = MMIDILE_SHORTCUT_ITEM_WIDTH_H;
    }
    else
    {
        width = MMIDILE_SHORTCUT_ITEM_WIDTH;
    }
    return width;
}

/*****************************************************************************/
//  Description : get shortcut num
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint32 GetShortcutNum(void)
{
    uint16 screen_width = 0;
    uint16 shortcut_width = GetShortcutitemWidth();
    
    GUILCD_GetLogicWidthHeight( 0, &screen_width, 0);
    return (screen_width/shortcut_width);   //MMIIDLE_SHORTCUT_NUM
}

/*****************************************************************************/
//  Description : display shortcut
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayShortCut(
                           BOOLEAN                  is_all,
                           MMI_WIN_ID_T             win_id,
                           MMIIDLE_SHORTCUT_FOCUS_E shortcut_type
                           )
{
    uint8                       i               = 0;
    MMI_IMAGE_ID_T              shortcut_img_id = IMAGE_NULL;
    GUI_LCD_DEV_INFO     const  *lcd_dev_ptr    = GetSmartIdlePaintLcdDev();
//    MMISET_IDLE_SHORTCUT_T      idle_shortcut   = {0};/*lint !e64*/
    MMIIDLE_LINE_TYPE_E         focus_line_type = GetFocusLineType(PNULL);
    GUI_RECT_T 					icon_total_display_rect = {0};
	GUI_RECT_T 					icon_display_rect[MMIIDLE_SHORTCUT_NUM] ={0};		
	GUI_RECT_T 					icon_display_rect_normal[MMIIDLE_SHORTCUT_NUM] ={0};	
    GUI_RECT_T 					icon_total_display_rect_normal = {0};
	GUI_RECT_T					center_rect = {0};
    uint16                      icon_width = 0;
    uint16                      icon_height = 0;
    uint32                      shortcut_num = GetShortcutNum();
    uint32                      shortcut_item_width = GetShortcutitemWidth();

    icon_total_display_rect = GetShortcutRect();
    
	GUI_CreateMatrixRect(&icon_total_display_rect,MMIDILE_SHORTCUT_HIRIZONAL_MARGIN,
	        0, 0, 0, 1, shortcut_num,icon_display_rect);

    icon_total_display_rect_normal = icon_total_display_rect;
    icon_total_display_rect_normal.top = icon_total_display_rect.top + MMIDILE_SHORTCUT_HIGHLIGHT_OFFSET;
	GUI_CreateMatrixRect(&icon_total_display_rect_normal,MMIDILE_SHORTCUT_HIRIZONAL_MARGIN,
	        0, 0, 0, 1, shortcut_num,icon_display_rect_normal);
  
    
    if(s_line_info.line_num == GetLineIndex(focus_line_type)
            && MMIIDLE_LINE_SHORTCUT != focus_line_type)
    {
        focus_line_type = MMIIDLE_LINE_SHORTCUT;
        SetFocusLineType(focus_line_type, MMIIDLE_SHORTCUT_FOCUS_1);
    }

    shortcut_type = s_line_info.shortcut_focus ;    
    //get shortcut image id
    //idle_shortcut = MMIAPISET_GetIdleShortcut();

    if(MMIIDLE_LINE_SHORTCUT == focus_line_type)
    {      
        icon_total_display_rect.left -= shortcut_item_width * (shortcut_num - 1 - (uint32)shortcut_type); /*lint !e737 */
		icon_total_display_rect.bottom = GetSoftkeyRect().bottom;
        GUIRES_DisplayImg(PNULL,&icon_total_display_rect,PNULL,win_id,IMAGE_IDLE_SHORTCUT_BACKGROUND_FOCUSED,lcd_dev_ptr);
    }
    else
    {
		icon_total_display_rect.bottom = GetSoftkeyRect().bottom;
        GUIRES_DisplayImg(PNULL,&icon_total_display_rect,PNULL,win_id,IMAGE_IDLE_SHORTCUT_BACKGROUND,lcd_dev_ptr);
    }

    //display shortcut icon

    for (i=0; i<shortcut_num; i++) 
    {            
        shortcut_img_id = s_shortcut_id[i].img_id;            
        GUIRES_GetImgWidthHeight(&icon_width,&icon_height,shortcut_img_id,win_id);
        center_rect = GUI_GetCenterRect(icon_display_rect_normal[i],icon_width,icon_height);
        if(MMIIDLE_LINE_SHORTCUT == focus_line_type && shortcut_type == i)
        {
            shortcut_img_id = s_shortcut_id[i].focus_img_id;            
            GUIRES_GetImgWidthHeight(&icon_width,&icon_height,shortcut_img_id,win_id);
            center_rect = GUI_GetCenterRect(icon_display_rect[i],icon_width,icon_height);
        }
        
        GUIRES_DisplayImg(PNULL,&center_rect,PNULL,win_id,shortcut_img_id,lcd_dev_ptr);
     }

    
    //add line num
    SetLineTypeAndNum(MMIIDLE_LINE_SHORTCUT);
    
    for (i=0; i<(shortcut_num); i++)
    {
        //set shortcut rect
        SetDisplayRect(icon_display_rect[i],MMIIDLE_TP_SHORTCUT1+i);/*lint !e64*/
    }
 
}

/*****************************************************************************/
//  Description : get line number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLineNum(void)
{
    return (s_line_info.line_num);
}

/*****************************************************************************/
//  Description : get line type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIIDLE_LINE_TYPE_E GetLineType(
                                      uint16    line_index
                                      )
{
    if (MMIIDLE_ITEM_MAX_NUM <= line_index)
    {
        //SCI_TRACE_LOW:"GetLineType ERROR! line_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_1397_112_2_18_2_24_0_34,(uint8*)"d",line_index);
        return s_line_info.line_type[MMIIDLE_ITEM_MAX_NUM - 1];
    }
    else
    {
        return (s_line_info.line_type[line_index]);
    }
}

/*****************************************************************************/
//  Description : set line type and add line number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLineTypeAndNum(
                             MMIIDLE_LINE_TYPE_E    line_type
                             )
{
    //set line type
    
    uint32 max_line = GetPromptLineNum() + 1;   //add shortcut line
    if(max_line > s_line_info.line_num)
    {
        s_line_info.line_type[s_line_info.line_num] = line_type;

        //add line number
        s_line_info.line_num++;
    }
}

#if 0
/*****************************************************************************/
//  Description : judge if mp3,  kur all stopped
//  Global resource dependence : 
//  Author: maryxiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsIdleMediaStoped(void)
{
	BOOLEAN result =TRUE;
#ifdef FM_SUPPORT
	MMIFM_STATUS_E  fm_status = FM_PLAYING;
#endif

	if(!MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_MP3))
		{
			result = FALSE;
		}
#ifdef KURO_SUPPORT
	if(!MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_KURO))
		{
			result = FALSE;
		}
#endif
#ifdef FM_SUPPORT
	fm_status = MMIAPIFM_GetStatus();
	if((FM_PLAYING == fm_status) || (FM_SUSPENDED == fm_status))
		{
			result = FALSE;
		}
#endif
	return  result;
}
#endif

/*****************************************************************************/
//  Description : reset line number and type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetLineNumAndType(
                               BOOLEAN  is_show_words
                               )
{
    uint32  i  = 0;
    
    s_line_info.line_num = 0;

    for (i=0; i<MMIIDLE_ITEM_MAX_NUM; i++)
    {
        s_line_info.line_type[i] = MMIIDLE_LINE_NONE;
    }
}

/*****************************************************************************/
//  Description : get focus line type 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIIDLE_LINE_TYPE_E GetFocusLineType(
                                           MMIIDLE_SHORTCUT_FOCUS_E    *shortcut_ptr   //may PNULL
                                           )
{
    MMIIDLE_LINE_TYPE_E     focus_line_type     = s_line_info.line_focus_type;

    if (PNULL != shortcut_ptr)
    {
        *shortcut_ptr = s_line_info.shortcut_focus;
    }

    return (focus_line_type);
}

/*****************************************************************************/
//  Description : set focus line type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFocusLineType(
                            MMIIDLE_LINE_TYPE_E         line_focus_type,//in
                            MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus  //in
                            )
{
    s_line_info.line_focus_type = line_focus_type;
    if (MMIIDLE_LINE_SHORTCUT == line_focus_type)
    {
        s_line_info.shortcut_focus = shortcut_focus;
    }
}

/*****************************************************************************/
//  Description : get focus line index 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFocusLineIndex(void)
{
    uint16  i   = 0;

    for (i=0; i< s_line_info.line_num; i++)
    {
        if (s_line_info.line_type[i] == s_line_info.line_focus_type)
        {
            break;
        }
    }
    if (i == s_line_info.line_num)
    {
        i = 0;
    }
    return (i);
}

/*****************************************************************************/
//  Description : get line index 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLineIndex(
                          MMIIDLE_LINE_TYPE_E   line_type
                          )
{
    uint16  i = 0;

    for (i=0; i< s_line_info.line_num; i++)
    {
        if (s_line_info.line_type[i] == line_type)
        {
            break;
        }
    }

    return (i);
}

/*****************************************************************************/
//  Description : display net name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayNetname(
                          BOOLEAN               is_only_update, //是否显示指定的信息
                          MMI_WIN_ID_T          win_id,
                          MMIIDLE_LINE_TYPE_E   sim_type
                          )
{
    BOOLEAN             is_fly_mode                                 = FALSE;
    wchar               opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
    uint32              i                                           = 0;
    GUI_RECT_T          rect                                        = {0};
    MMI_STRING_T        netname_str                                 = {0};
    GUI_LCD_DEV_INFO  *lcd_dev_ptr                             = GetSmartIdlePaintLcdDev();
    uint16              line_index                                  = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    wchar *sim_deactivated_str = PNULL;
    
    const MMI_IMAGE_ID_T sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_SIM
#else
        IMAGE_IDLE_PROMPT_SIM1,
        IMAGE_IDLE_PROMPT_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_SIM4
#endif
#endif
    };

    const MMI_IMAGE_ID_T no_sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_NO_SIM
#else
        IMAGE_IDLE_PROMPT_NO_SIM1,
        IMAGE_IDLE_PROMPT_NO_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_NO_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_NO_SIM4
#endif
#endif
    };

    const MMI_IMAGE_ID_T gray_sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_GRAY_SIM
#else
        IMAGE_IDLE_PROMPT_GRAY_SIM1,
        IMAGE_IDLE_PROMPT_GRAY_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_GRAY_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_GRAY_SIM4
#endif
#endif
    };
 
    //set display net name
  
        if (is_only_update)
        {
            //get line index
            line_index = GetLineIndex(sim_type);
        }
        else
        {
            //get line number
            line_index = GetLineNum();
        }

        //display line and select image
        DisplayLineAndSelImg(line_index,win_id,sim_type,lcd_dev_ptr);/*lint !e64*/


        //get icon point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);

        i = (uint32)sim_type - (uint32)MMIIDLE_LINE_SIM1;
        //display sim icon
        if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(i))
            {
                image_id = sim_img_id[i];
            }
            else
            {
                image_id = gray_sim_img_id[i];
            }
        }
        else
        {
            image_id = no_sim_img_id[i];
        }
        rect = GetCurItemImgDisplayRect(rect,  win_id, image_id);
        GUIRES_DisplayImg(0,
            &rect,
            PNULL,
            win_id,
            image_id,
            lcd_dev_ptr);

        //get fly mode is open
        is_fly_mode = MMIAPISET_GetFlyMode();

        //display string
        if (is_fly_mode)
        {
            MMI_GetLabelTextByLang(TXT_SET_FLY_MODE,&netname_str);
        }
        else if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(i))//待机
            {
                SCI_MEMSET(&netname_str, 0x00, sizeof(netname_str));
                netname_str.wstr_ptr = (wchar *)opn_spn_name;
                SCI_MEMSET(netname_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
            
                MMIAPIPHONE_GetSimAndServiceString(i, &netname_str);
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else
            {
                MMISET_SIM_NAME_T sim_name = {0};
                MMI_STRING_T sim_deactivated = {0};

                sim_name = MMIAPISET_GetSimName(i);

                MMI_GetLabelTextByLang(TXT_SIM_DEACTIVATED, &sim_deactivated);

                //此处不判断是否需要加入空格，统一都多分配一个空格的存储空间(2bytes)    
                sim_deactivated_str = (wchar *)SCI_ALLOC_APP(((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));
                SCI_MEMSET(sim_deactivated_str, 0x00, ((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));

                MMI_WSTRNCPY(sim_deactivated_str,
                                            (sim_deactivated.wstr_len + sim_name.wstr_len + 1),
                                            sim_name.wstr_arr,
                                            sim_name.wstr_len,
                                            sim_name.wstr_len);

                // ASCII 码数据，两个字符串中间追加一个空格。
                if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr,sim_name.wstr_len)
                    && MMIAPICOM_IsASCIIString(sim_deactivated.wstr_ptr ,sim_name.wstr_len))
                {
                    MMIAPICOM_Wstrcat(sim_deactivated_str, L" "); 
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len + 1,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    
                    netname_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    netname_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len + 1 ;
                }
                else
                {
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len ,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    netname_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    netname_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len ;
                }
            }
#endif
        }
        else
        {

#ifndef MMI_IDLE_MINI_SUPPORT
            MMI_GetLabelTextByLang(TXT_INSERT_SIM, &netname_str);
#else
            MMI_GetLabelTextByLang(TXT_INSERT_SIM_MINI, &netname_str); 
#endif                
            
        }
        //get text point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);

        MMIIDLE_DisplayLineTextInRect(
            lcd_dev_ptr,
            &rect,
            MMIIDLE_TEXT_FONT,
            MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_NETWORKNAME),
            netname_str.wstr_ptr, 
            netname_str.wstr_len
            );

        if (PNULL != sim_deactivated_str)
        {
            SCI_FREE(sim_deactivated_str);
            sim_deactivated_str = PNULL;
        }

        if (!is_only_update)
        {
            //set sim rect
            SetPromptDisplayRect(line_index, MMIIDLE_TP_LINE2 + sim_type - MMIIDLE_LINE_SIM1);/*lint !e64*/

            //add line number
            SetLineTypeAndNum(sim_type);/*lint !e64*/
        }    
}

/*****************************************************************************/
//  Description : display line and select image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayLineAndSelImg(
                                uint16                  line_index,
                                MMI_WIN_ID_T            win_id,
                                MMIIDLE_LINE_TYPE_E     line_type,
                                const GUI_LCD_DEV_INFO  *lcd_dev_ptr
                                )
{
    MMIIDLE_LINE_TYPE_E     focus_line_type = GetFocusLineType(PNULL);
	GUI_RECT_T              rect = MMITHEME_GetFullScreenRect();

    //set item index,except shortcut
    rect = GetPromptLineRect(line_index);

    //display select image
    if (line_type == focus_line_type)
    {
        //display line
        GUIRES_DisplayImg(0,
            &rect,
            0,
            win_id,
            IMAGE_IDLE_PROMPT_LINE,
            lcd_dev_ptr);       

   
        //display select bar
//        LCD_FillRoundedRect(lcd_dev_ptr,
//            rect,
//            rect,
//            MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    }
}

/*****************************************************************************/
//  Description : get current item info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T GetCurItemPos(
                                uint16                  line_index, //from 0 start
                                MMIIDLE_PROMPT_POS_E    prompt_pos
                                )
{
    GUI_POINT_T     pos_point   =   {0};
    GUI_RECT_T      rect = GetPromptLineRect(line_index);

    switch (prompt_pos)
    {
    case MMIIDLE_PROMPT_POS_ICON:
        pos_point.x = rect.left + 6;
        pos_point.y = rect.top + 1;

        break;

    case MMIIDLE_PROMPT_POS_TEXT:
         pos_point.x = rect.left + 30;
         pos_point.y = rect.top + 1;
        break;

    default:
        //SCI_TRACE_LOW:"GetCurItemPos:prompt_pos %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_2005_112_2_18_2_24_1_38,(uint8*)"d",prompt_pos);
        break;
    }

    return (pos_point);
}

LOCAL GUI_RECT_T GetCurItemRect(
                                uint16                  line_index, //from 0 start
                                MMIIDLE_PROMPT_POS_E    prompt_pos
                                )
{
    GUI_RECT_T      rect = GetPromptLineRect(line_index);

    switch (prompt_pos)
    {
    case MMIIDLE_PROMPT_POS_ICON:
        rect.left += MMIDILE_TIME_POS_X;
        break;

    case MMIIDLE_PROMPT_POS_TEXT:
        rect.left += MMIDILE_PROMPT_LINE_TEXT_POS_X;
        break;

    default:
        break;
    }

    return (rect);
}

LOCAL GUI_RECT_T GetCurItemImgDisplayRect(
                                            GUI_RECT_T              src_rect,
                                            MMI_WIN_ID_T            win_id,
                                            MMI_IMAGE_ID_T          img_id
                                            )
{
    GUI_RECT_T          rect = {0};
    uint16              img_heigth = 0;
    uint16              img_width = 0;
    GUIRES_GetImgWidthHeight(&img_width, &img_heigth,  img_id,  win_id);
    rect = GUI_GetCenterRect( src_rect, img_width, img_heigth);    
    rect.left = src_rect.left;
    return (rect);
}

/*****************************************************************************/
//  Description : set prompt line display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPromptDisplayRect(
                                uint16              line_index, //from 0 start
                                MMIIDLE_TP_TYPE_E   tp_type
                                )
{
    GUI_RECT_T  dis_rect   = GetPromptLineRect(line_index);

    //set shortcut rect
    SetDisplayRect(dis_rect,tp_type);
}

/*****************************************************************************/
//  Description : display schedule
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySchedule(
                           BOOLEAN          is_only_update, //是否只显示
                           MMI_WIN_ID_T     win_id
                           )
{
    uint16                  line_index = 0;
    GUI_RECT_T               rect = {0};
    GUI_LCD_DEV_INFO   *lcd_dev_ptr = GetSmartIdlePaintLcdDev();
    //get line index
    if (is_only_update)
    {
        line_index = GetLineIndex(MMIIDLE_LINE_SCHEDULE);
    }
    else
    {
        line_index = GetLineNum();
    }

    //display line and select image
    DisplayLineAndSelImg(line_index,win_id,MMIIDLE_LINE_SCHEDULE,lcd_dev_ptr);

    //get icon point
    rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);

    rect = GetCurItemImgDisplayRect(rect,  win_id, IMAGE_IDLE_PROMPT_SCHEDULE);

    //display sim icon
    GUIRES_DisplayImg(0,
        &rect,
        PNULL,
        win_id,
        IMAGE_IDLE_PROMPT_SCHEDULE,
        lcd_dev_ptr);

    //get text point
    rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);
#ifdef CALENDAR_SUPPORT
    MMIIDLE_DisplayScheduleEx(lcd_dev_ptr, &rect, MMIIDLE_TEXT_FONT);
#endif
    if (!is_only_update)
    {
        //set schedule rect
        SetPromptDisplayRect(line_index,MMIIDLE_TP_LINE2+line_index-GetTimeLineOffset());/*lint !e64*/

        //add line number
        SetLineTypeAndNum(MMIIDLE_LINE_SCHEDULE);
    }
}

#ifdef COUNTEDTIME_SUPPORT

/*****************************************************************************/
//  Description : display count down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayCountDown(
                               BOOLEAN          is_only_update, //是否只显示
                               MMI_WIN_ID_T     win_id
                               )
{
    BOOLEAN             result                      = FALSE;
    uint8               time_str[MMIIDLE_COUNT_TIME_LEN]  = {0};
    uint16              time_wstr[MMIIDLE_COUNT_TIME_LEN] = {0};
    uint16              time_str_len                = 0;
    SCI_TIME_T          time                        = {0};
    GUI_RECT_T          rect                        = {0};
    MMI_STRING_T        name_str                    = {0};
    MMI_STRING_T        count_down_str              = {0};
    GUI_LCD_DEV_INFO  *lcd_dev_ptr             = GetSmartIdlePaintLcdDev();
    uint16              line_index = 0;

    result = MMIAPICT_IsShowIdleCoutime();
    
    if (result)
    {
        //get line index
        if (is_only_update)
        {
            line_index = GetLineIndex(MMIIDLE_LINE_COUNT_DOWN);
        }
        else
        {
            line_index = GetLineNum();
        }

        //display line and select image
        DisplayLineAndSelImg(line_index,win_id,MMIIDLE_LINE_COUNT_DOWN,lcd_dev_ptr);

        //get icon point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);
        rect = GetCurItemImgDisplayRect(rect,  win_id, IMAGE_IDLE_PROMPT_COUNT_DOWN);

        //display icon
        GUIRES_DisplayImg(0,
            &rect,
            PNULL,
            win_id,
            IMAGE_IDLE_PROMPT_COUNT_DOWN,
            lcd_dev_ptr);

        //倒计时
        MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME,&name_str);

        //get count time
        time = MMIAPICT_GetCurTime();

        //time xx:xx
//        if (MMIAPICT_IsLessThanOnemin())
//        {
            //小于1分钟
//            sprintf((char *)time_str," %02d:%02d ",time.min,time.sec);
//        }
//        else
//        {
            //大于1分钟
//            sprintf((char *)time_str," %02d:%02d ",time.hour,time.min);
//        }
        sprintf((char *)time_str," %02d:%02d:%02d ",time.hour,time.min,time.sec);
        time_str_len = strlen((char *)time_str);
        MMI_STRNTOWSTR(time_wstr,MMIIDLE_COUNT_TIME_LEN,
            time_str,MMIIDLE_COUNT_TIME_LEN,
            time_str_len);

        //get count down string len
        count_down_str.wstr_len = name_str.wstr_len + time_str_len;

        //copy count down string
        count_down_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((count_down_str.wstr_len+1)*sizeof(wchar));
        SCI_MEMSET(count_down_str.wstr_ptr,0,((count_down_str.wstr_len+1)*sizeof(wchar)));

        //copy name
        MMI_WSTRNCPY(count_down_str.wstr_ptr,count_down_str.wstr_len,
            name_str.wstr_ptr,name_str.wstr_len,
            name_str.wstr_len);

        //copy time
        MMI_WSTRNCPY(count_down_str.wstr_ptr+name_str.wstr_len,count_down_str.wstr_len-name_str.wstr_len,
            time_wstr,MMIIDLE_COUNT_TIME_LEN,
            time_str_len);

        //get text point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);

        //display string
        MMIIDLE_DisplayLineTextInRect(
                lcd_dev_ptr,
                &rect,
                MMIIDLE_TEXT_FONT,
                MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_COUNTTIME),
                count_down_str.wstr_ptr, 
                count_down_str.wstr_len
                );

        //free memory
        SCI_FREE(count_down_str.wstr_ptr);

        if (!is_only_update)
        {
            //set count down rect
            SetPromptDisplayRect(line_index,MMIIDLE_TP_LINE2+line_index-GetTimeLineOffset());/*lint !e64*/

            //add line number
            SetLineTypeAndNum(MMIIDLE_LINE_COUNT_DOWN);
        }
    }

    return (result);
}
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : display mp3 name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayMp3Name(
                             BOOLEAN          is_only_update, //是否只显示
                             MMI_WIN_ID_T     win_id
                             )
{
    BOOLEAN                         result                              = FALSE;
    uint16                          display_max_len                     = 0;
    GUI_RECT_T                      rect                                = {0};
    MMI_STRING_T                    mp3_str                             = {0};
    MMI_IMAGE_ID_T                  icon_id                             = IMAGE_IDLE_PROMPT_PLAY;
    GUI_LCD_DEV_INFO           *lcd_dev_ptr                        = GetSmartIdlePaintLcdDev();
    MMIIDLE_LINE_TYPE_E             idle_line_type                      = MMIIDLE_LINE_MP3;
    MMI_THEME_FONT_COLOR_INDEX_E    idle_theme_type                     = MMI_THEME_IDLE_MP3;
    uint16                          line_index = 0;
    wchar                           full_str[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    uint16                          full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
	GUI_RECT_T              mainlcd_rect = MMITHEME_GetFullScreenRect();


    if(MMIAPIMP3_IsPlayerPlaying() )
    {
        icon_id = IMAGE_IDLE_PROMPT_PLAY;
        result = TRUE;
    }
    else if(MMIAPIMP3_IsPlayerActive() )
    {
        icon_id = IMAGE_IDLE_PROMPT_PAUSE;
        result = TRUE;
    }
    if (result)
    {
        //get line index
        if (is_only_update)
        {
            if (MMIAPIMP3_IsPlayerActive())
            {
                line_index = GetLineIndex(MMIIDLE_LINE_MP3);
            }
        }
        else
        {
            line_index = GetLineNum();
        }

        //get mp3 name
        mp3_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
        SCI_MEMSET(mp3_str.wstr_ptr,0,((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        if (MMIAPIMP3_IsPlayerActive())
        {
            MMIAPIMP3_GetCurMp3Name(&mp3_str, MMIMP3_NAME_MAX_LEN);
        }
#endif
        //display line and select image
        DisplayLineAndSelImg(line_index,win_id,idle_line_type,lcd_dev_ptr);

        //get icon point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);
        rect = GetCurItemImgDisplayRect(rect,  win_id, icon_id);

        //display icon
        GUIRES_DisplayImg(0,
            &rect,
            PNULL,
            win_id,
            icon_id,
            lcd_dev_ptr);
        
        //get text point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);
        
        //display string max len
        display_max_len = mainlcd_rect.right-(rect.left+3)-MMIIDLE_MARGIN_WIDTH-1;
        
        MMIIDLE_GetDisPlayString(mp3_str.wstr_ptr,mp3_str.wstr_len,display_max_len,MMIIDLE_TEXT_FONT,full_str,&full_len);

        //display string
        MMIIDLE_DisplayLineTextInRect(
                lcd_dev_ptr,
                &rect,
                MMIIDLE_TEXT_FONT,
                MMITHEME_GetCurThemeFontColor(idle_theme_type),
                full_str,
                full_len
                );

        //free memory
        SCI_FREE(mp3_str.wstr_ptr);

        if (!is_only_update)
        {
            //set mp3 rect
            SetPromptDisplayRect(line_index,MMIIDLE_TP_LINE2+line_index-GetTimeLineOffset());/*lint !e64*/

            //add line number
            SetLineTypeAndNum(idle_line_type);
        }
    }

    return (result);
}
#endif

#ifdef FM_SUPPORT  

/*****************************************************************************/
//  Description : display fm name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayFmName(
                            BOOLEAN         is_only_update, //是否只显示
                            MMI_WIN_ID_T    win_id
                            )
{
    BOOLEAN             result                              = FALSE;
    uint16              line_index                          = 0;
    uint16              display_max_len                     = 0;
    GUI_RECT_T          rect                                = {0};
    MMI_STRING_T        name_str                            = {0};
    MMI_STRING_T        frequency_str                       = {0};
    MMI_STRING_T        fm_str                              = {0};
    MMIFM_STATUS_E      fm_status                           = FM_PLAYING;
    MMI_IMAGE_ID_T      icon_id                             = IMAGE_IDLE_PROMPT_PLAY;
    GUI_LCD_DEV_INFO  *lcd_dev_ptr                     = GetSmartIdlePaintLcdDev();
    wchar               full_str[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    uint16              full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
	GUI_RECT_T              mainlcd_rect = MMITHEME_GetFullScreenRect();

    //get fm status
    fm_status = MMIAPIFM_GetStatus();
    switch (fm_status)
    {
    case FM_PLAYING:
        icon_id = IMAGE_IDLE_PROMPT_PLAY;
        result = TRUE;
        break;

    case FM_SUSPENDED:
        icon_id = IMAGE_IDLE_PROMPT_PAUSE;
        result = TRUE;
        break;

    case FM_STOP:
        result = FALSE;
        break;

    default:
        //SCI_TRACE_LOW:"DisplayFmName:fm_status %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_2393_112_2_18_2_24_2_42,(uint8*)"d",fm_status);
        break;
    }

    if (result)
    {
        //get line index
        if (is_only_update)
        {
            line_index = GetLineIndex(MMIIDLE_LINE_FM);
        }
        else
        {
            line_index = GetLineNum();
        }

        //display line and select image
        DisplayLineAndSelImg(line_index,win_id,MMIIDLE_LINE_FM,lcd_dev_ptr);

        //get icon point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);

        rect = GetCurItemImgDisplayRect(rect,  win_id, icon_id);
        //display icon
        GUIRES_DisplayImg(0,
            &rect,
            PNULL,
            win_id,
            icon_id,
            lcd_dev_ptr);

        //FM
        MMI_GetLabelTextByLang(TXT_FM,&name_str);

        //get frequency
        frequency_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((FMM_SEARCH_FILENAME_MAXLEN+1)*sizeof(wchar));
        SCI_MEMSET(frequency_str.wstr_ptr,0,((FMM_SEARCH_FILENAME_MAXLEN+1)*sizeof(wchar)));
        MMIAPIFM_GetCurFreqName(&frequency_str,FMM_SEARCH_FILENAME_MAXLEN);

        //get fm string len
        fm_str.wstr_len = name_str.wstr_len + frequency_str.wstr_len;

        //copy fm string
        fm_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((fm_str.wstr_len+1)*sizeof(wchar));
        SCI_MEMSET(fm_str.wstr_ptr,0,((fm_str.wstr_len+1)*sizeof(wchar)));

        //copy name
        MMI_WSTRNCPY(fm_str.wstr_ptr,fm_str.wstr_len,
            name_str.wstr_ptr,name_str.wstr_len,
            name_str.wstr_len);

        //copy frequency
        MMI_WSTRNCPY(fm_str.wstr_ptr+name_str.wstr_len,fm_str.wstr_len-name_str.wstr_len,
            frequency_str.wstr_ptr,FMM_SEARCH_FILENAME_MAXLEN,
            frequency_str.wstr_len);

        //get text point
        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);

        //display string max len
        display_max_len = mainlcd_rect.right-(rect.left+1)-MMIIDLE_MARGIN_WIDTH;
        
        MMIIDLE_GetDisPlayString(fm_str.wstr_ptr,fm_str.wstr_len,display_max_len,MMIIDLE_TEXT_FONT,full_str,&full_len);

        //display string
        MMIIDLE_DisplayLineTextInRect(
                lcd_dev_ptr,
                &rect,
                MMIIDLE_TEXT_FONT,
                MMI_DEFAULT_TEXT_COLOR,
                full_str,
                full_len
                );
        //free memory
        SCI_FREE(frequency_str.wstr_ptr);
        SCI_FREE(fm_str.wstr_ptr);

        if (!is_only_update)
        {
            //set fm rect
            SetPromptDisplayRect(line_index,MMIIDLE_TP_LINE2+line_index-GetTimeLineOffset());/*lint !e64*/

            //add line number
            SetLineTypeAndNum(MMIIDLE_LINE_FM);
        }
    }

    return (result);
}
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT 
/*****************************************************************************/
//  Description : display words
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWords(
                        BOOLEAN         is_only_update, //是否只显示
                        const wchar     *words_ptr,
                        uint16          words_len,
                        MMI_WIN_ID_T    win_id
                        )
{
    uint16              line_index      = 0;
    GUI_RECT_T          rect =  {0};
	MMI_IMAGE_ID_T      icon_id                             = IMAGE_IDLE_PROMPT_WORDS;//ADD BY MARY
    GUI_LCD_DEV_INFO  *lcd_dev_ptr = GetSmartIdlePaintLcdDev();
    uint16              display_max_len                     = 0;
    wchar               full_str[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    uint16              full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
	GUI_RECT_T          mainlcd_rect = MMITHEME_GetFullScreenRect();
    MMI_STRING_T        idle_words={0};
      
    if(PNULL == words_ptr)
    {
        //SCI_TRACE_LOW:"DisplayWords ERROR! PNULL == words_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_2513_112_2_18_2_24_2_44,(uint8*)"");
        return;
    }

    //个性词语内容为空时显示默认个性词语
    if(words_len== 0)
    {
	MMI_GetLabelTextByLang(TXT_SET_CUSTOM_WORD, &idle_words);	
	words_ptr  = idle_words.wstr_ptr;
	words_len  = idle_words.wstr_len;
    }
    
    
    //get line index
    if (is_only_update)
    {
        line_index = GetLineIndex(MMIIDLE_LINE_WORDS);
    }
    else
    {
        line_index = GetLineNum();
    }

    //display line and select image
    DisplayLineAndSelImg(line_index,win_id,MMIIDLE_LINE_WORDS,lcd_dev_ptr);
  rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);
  rect = GetCurItemImgDisplayRect(rect,  win_id, icon_id);

  //display icon
  GUIRES_DisplayImg(0,
      &rect,
      PNULL,
      win_id,
      icon_id,
      lcd_dev_ptr); // add by mary


    //get text point
    rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);
    //display string max len
    display_max_len = mainlcd_rect.right - rect.left -MMIIDLE_MARGIN_WIDTH-1;
    
    MMIIDLE_GetDisPlayString(words_ptr,words_len,display_max_len,MMIIDLE_TEXT_FONT,full_str,&full_len);
    //display string
    MMIIDLE_DisplayLineTextInRect(
        lcd_dev_ptr,
        &rect,
        MMIIDLE_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_IDLEWORD),
        full_str, 
        full_len
        );
    if (!is_only_update)
    {
        //set words rect
        SetPromptDisplayRect(line_index,MMIIDLE_TP_LINE2+line_index-GetTimeLineOffset());/*lint !e64*/

        //add line number
        SetLineTypeAndNum(MMIIDLE_LINE_WORDS);
    }
}
#endif
#ifdef MMI_ENABLE_DCD

/*****************************************************************************/
//  Description : active or deactive dcd ctrl
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_ActiveDcdCtrl(
                                  MMI_WIN_ID_T     win_id,
                                  BOOLEAN          is_active   //激活or去激活
                                  )
{
    if (is_active)
    {
        MMK_WinInactiveCtrl(win_id, FALSE);
        MMIAPIDCD_SetIdleTabColorAndActive(TRUE);
    }
    else
    {
        MMIAPIDCD_SetIdleTabColorAndActive(FALSE);
        MMK_WinInactiveCtrl(win_id, TRUE);
    }      
}

/*****************************************************************************/
//  Description : display dcd
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayDcd(
                      BOOLEAN          is_only_update, //是否只显示
                      MMI_WIN_ID_T     win_id
                      )
{
    
    uint16       line_index   = 0;
    GUI_RECT_T   dcd_rect   = MMIAPIDCD_GetFocusCtrlRect();
    MMIIDLE_LINE_TYPE_E focus_line_type = GetFocusLineType(PNULL);
 
    //get line index
    if (is_only_update)
    {
        line_index = GetLineIndex(MMIIDLE_LINE_DCD);
    }
    else
    {
        line_index = GetLineNum();
        SetDisplayRect(dcd_rect, MMIIDLE_TP_LINE2+(line_index)-GetTimeLineOffset());/*lint !e64*/
        SetLineTypeAndNum(MMIIDLE_LINE_DCD);
    }
       
    if (MMIIDLE_LINE_DCD == focus_line_type)
    {
        MMIAPIIDLE_ActiveDcdCtrl(win_id, TRUE);
    }
    else
    {
        MMIAPIIDLE_ActiveDcdCtrl(win_id, FALSE);
    }      
}

#endif


/*****************************************************************************/
//  Description : display WLAN
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)     
LOCAL void DisplayWlan(
                                     BOOLEAN          is_only_update, //是否只显示
                                     MMI_WIN_ID_T     win_id
                                   )
{
    uint16                          line_index                          = 0;
    GUI_RECT_T                      rect = 0;
    MMI_IMAGE_ID_T                  icon_id                             = IMAGE_IDLE_PROMPT_WLAN;
    GUI_LCD_DEV_INFO const          *lcd_dev_ptr                        = GetSmartIdlePaintLcdDev();
    MMIIDLE_LINE_TYPE_E             idle_line_type                      = MMIIDLE_LINE_WLAN;
    MMI_THEME_FONT_COLOR_INDEX_E    idle_theme_type                     = MMI_THEME_IDLE_IDLEWORD;
    uint16                          line_index = 0;
    MMI_STRING_T     str_ptr = {0};
    //get line index
    if (is_only_update)
    {
        line_index = GetLineIndex(MMIIDLE_LINE_WLAN);
    }
    else
    {
        line_index = GetLineNum();
    }

    //display line and select image
    DisplayLineAndSelImg(line_index,win_id,idle_line_type,lcd_dev_ptr);

    //get icon point
    rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);
    rect = GetCurItemImgDisplayRect(rect,  win_id, icon_id);

    //display icon
    GUIRES_DisplayImg(,
        &rect,
        PNULL,
        win_id,
        icon_id,
        lcd_dev_ptr);
    
    //get text point
    rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);
    
    if (MMIWIFI_STATUS_OFF != MMIAPIWIFI_GetStatus()) 
    {  
         MMI_GetLabelTextByLang(TXT_COMM_HAS_OPEN,&str_ptr);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMM_HAS_CLOSE,&str_ptr);
    }
    
    //display string
    MMIIDLE_DisplayLineTextInRect(
            (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
            &rect,
            MMIIDLE_TEXT_FONT,
            MMITHEME_GetCurThemeFontColor(idle_theme_type),
            str_ptr.wstr_ptr,
            str_ptr.wstr_len
            );
    
    if (!is_only_update)
    {
        SetPromptDisplayRect(line_index,MMIIDLE_TP_LINE2+line_index-GetTimeLineOffset());/*lint !e64*/

        //add line number
        SetLineTypeAndNum(idle_line_type);
    }

}
#endif

/*****************************************************************************/
//  Description : display softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySoftkey(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO const *lcd_dev_ptr)
{
    uint8                   font_height     = 0;
    uint16                  str_width       = 0;
    uint16                  str_height      = 0;
    GUI_RECT_T              dis_rect        = {0};
    GUI_POINT_T             dis_point       = {0};
    MMI_STRING_T            sk_str          = {0};
    MMI_TEXT_ID_T           left_sk_text    = TXT_NULL;
    MMI_TEXT_ID_T           right_sk_text   = TXT_NULL;   
    uint16                  display_max_len = 0;
    //wchar                   full_str[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    wchar                   *full_str = PNULL;
    uint16                  full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
	GUI_RECT_T              mainlcd_rect = MMITHEME_GetFullScreenRect();
    uint16                  mid_icon_width = 0;
    uint16                  mid_icon_height = 0;
    MMI_IMAGE_ID_T          mid_icon   = 0;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T            full_str_t = {0};
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;

    idle_style = MMIAPISET_GetIdleStyle();
#ifdef CMCC_UI_STYLE
#ifndef MMI_IDLE_MINI_SUPPORT
    mid_icon = IMAGE_IDLE_MIDDLE_SOFTKEY_ICON;
#endif
#else
#ifndef MMI_IDLE_MINI_SUPPORT
    if(MMISET_IDLE_STYLE_SMART == idle_style)
    {
        mid_icon = IMAGE_IDLE_MIDDLE_SOFTKEY_ICON;    
    }
#ifdef MMIWIDGET_SUPPORT
    else if(MMISET_IDLE_STYLE_WIDGET == idle_style)
    {
        mid_icon = IMAGE_WIDGET_IDLE_MIDDLE_SOFTKEY_ICON;
    }
#endif    
#endif
#endif
    GUIRES_GetImgWidthHeight(&mid_icon_width, &mid_icon_height,mid_icon , win_id);
    //get font height
	display_max_len = ((mainlcd_rect.right - mid_icon_width)/2);

    font_height = GUI_GetFontHeight(MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,UNICODE_HANZI);

    //get softkey text
    MMIAPISET_GetFSMainTextId(&left_sk_text,&right_sk_text);
    
    MMI_GetLabelTextByLang(left_sk_text,&sk_str);
    
    full_str = SCI_ALLOC_APP(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));

    MMIIDLE_GetDisPlayString(sk_str.wstr_ptr,sk_str.wstr_len,display_max_len,MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,full_str,&full_len);

    //set left softkey display point
    dis_point.x = MMIIDLE_MARGIN_WIDTH;
    dis_point.y = mainlcd_rect.bottom - font_height - MMIIDLE_MARGIN_WIDTH - MMIIDLE_SOFTKEY_POS_Y_INTERVAL;

    //display string
    str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_POINT_T *)&dis_point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );

    //get string width and height
    str_height = GUI_GetStringHeight(MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,sk_str.wstr_ptr,sk_str.wstr_len);

    //set display rect
    dis_rect.left   = 0;
    dis_rect.top    = dis_point.y;
    dis_rect.right  = dis_rect.left + str_width - 1;
    dis_rect.bottom = dis_rect.top + str_height - 1;

    //set left softkey rect
    SetDisplayRect(dis_rect,MMIIDLE_TP_LEFT_SK);
#ifndef MMI_IDLE_MINI_SUPPORT
    //get middle softkey display point
#ifdef CMCC_UI_STYLE
    dis_point.x = (mainlcd_rect.right>>1 )- mid_icon_width/2;
#else
    dis_point.x = (mainlcd_rect.right - mid_icon_width)>>1;
#endif

    dis_point.y = mainlcd_rect.bottom - mid_icon_height - MMIIDLE_MARGIN_WIDTH;

    //display middle softkey icon
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            mid_icon,
            lcd_dev_ptr);

     //set display rect     
    dis_rect.left   = dis_point.x;
    dis_rect.top    = dis_point.y;
    dis_rect.right  = dis_point.x + mid_icon_width;
    dis_rect.bottom = dis_rect.top + mid_icon_height - 1;


    //set middle softkey rect
    SetDisplayRect(dis_rect,MMIIDLE_TP_MIDDLE_SK);
#endif
//#endif

    //get right softkey text
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
        MMI_GetLabelTextByLang(TXT_SOS,&sk_str);
    }
    else
#endif
	{
    	MMI_GetLabelTextByLang(right_sk_text,&sk_str);
	}

    MMIIDLE_GetIdleTextStyle(&text_style, MMI_FULLSCREEN_SOFTKEY_TEXT_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY));

    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
    full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    MMIIDLE_GetDisPlayString(sk_str.wstr_ptr,sk_str.wstr_len,display_max_len,MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,full_str,&full_len);

    //get right softkey text length
    full_str_t.wstr_ptr = full_str;
    full_str_t.wstr_len = full_len;
    str_width = GUISTR_GetStringWidth(&text_style,&full_str_t,text_state);

    //set right softkey display point
    dis_point.x = MAX((mainlcd_rect.right - str_width - MMIIDLE_MARGIN_WIDTH),(dis_rect.right+1));
    dis_point.y = mainlcd_rect.bottom - font_height - MMIIDLE_MARGIN_WIDTH - MMIIDLE_SOFTKEY_POS_Y_INTERVAL;
    
    //display string
    MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_POINT_T *)&dis_point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );

    //set display rect
    dis_rect.left   = dis_point.x;
    dis_rect.top    = dis_point.y;
    dis_rect.right  = mainlcd_rect.right;
    dis_rect.bottom = dis_rect.top + str_height - 1;
    
    SCI_FREE(full_str);
    full_str = PNULL;
    
    //set right softkey rect
    SetDisplayRect(dis_rect,MMIIDLE_TP_RIGHT_SK);
}


/*****************************************************************************/
//  Description : handle smart idle message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_SmartHandleMsg(
                                   MMI_WIN_ID_T         win_id,
                                   MMI_MESSAGE_ID_E     msg_id,
                                   DPARAM               param
                                   )
{
#if defined TOUCH_PANEL_SUPPORT
    GUI_POINT_T              tp_point    =   {0};
    LOCAL BOOLEAN            s_is_smart_process_tp_down   = FALSE;  //是否先处理了 tp down key
#endif
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    case MSG_GET_FOCUS:  
#ifdef UI_MULTILAYER_SUPPORT        
        CreatIdleTipsLayer(win_id);
        CreateSmartIdlePaintLayer(win_id);
#endif     
        break;

    case MSG_FULL_PAINT:
        DisplaySmartStyleIdle(win_id);
#ifdef MAINLCD_SIZE_240X320
        MAIN_SetStbDispTime(); 
#endif   
        break;

    case MSG_LCD_SWITCH:
#ifdef UI_MULTILAYER_SUPPORT        
        //hide idle tips
        //HideIdleTipsLayer();
#endif
        break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        //stop idle tips timer
        StopIdleTipsTimer();
#ifdef UI_MULTILAYER_SUPPORT        
        ReleaseIdleTipsLayer();
        ReleaseSmartIdlePaintLayer();
#endif        
        break;
        
    case MSG_APP_OK:
#ifdef CMCC_UI_STYLE
        //open current focus item
        OpenFocusItemWin(win_id);
#else
#ifdef BAR_PHONE_WIDGET 
        MMIAPIMENU_CreatMainMenu();          
#else
        MMIAPISET_OpenFuncKeyWin(win_id, MMISET_FUNCKEY_LEFT);
#endif
#endif
        break;

    case MSG_APP_WEB:
#ifdef CMCC_UI_STYLE
        //MO一键上网标记在web键
        MMIAPISET_EnterMonternetWin();
#else
        //open current focus item
        OpenFocusItemWin(win_id);
#endif
        break;

    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        //MMIDLE_RestartSsTimer(win_id);
        SwitchFocusLine(FALSE,win_id);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        //MMIDLE_RestartSsTimer(win_id);
        SwitchFocusLine(TRUE,win_id);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        //MMIDLE_RestartSsTimer(win_id);
        SwitchFocusShortCut(FALSE,win_id);
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        //MMIDLE_RestartSsTimer(win_id);
        SwitchFocusShortCut(TRUE,win_id);
        break;
        
    case MSG_APP_RED:
        ResetFocusToShortCut(win_id);
        break;
        
#if defined TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        s_is_smart_process_tp_down = TRUE;
        //get tp point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        SmartIdleHandleTpDown(tp_point,win_id);
        break;
        
    case MSG_TP_PRESS_UP:
        if(!s_is_smart_process_tp_down)
            break;
        
        s_is_smart_process_tp_down = FALSE;
        
        //get tp point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        SmartIdleHandleTpUp(tp_point,win_id);
        break;
#endif

    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_STK_SET_UP_IDLE_MODE: 
    case MSG_IDLE_UPDATE_COUNTEDTIME:
    case MSG_IDLE_UPDATE_MP3:   
    case MSG_WIDGET_MP3_SWITCH:
#ifdef UI_MULTILAYER_SUPPORT 
if(MMK_IsFocusWin(win_id))
    {
        DisplaySmartStyleIdle(win_id);
    }
#else        
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#endif
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : open the focus item window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenFocusItemWin(
                            MMI_WIN_ID_T    win_id
                            )
{
    MMIIDLE_LINE_TYPE_E         focus_line_type = MMIIDLE_LINE_NONE;
    MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus  = MMIIDLE_SHORTCUT_FOCUS_1;

    //get focus line type
    focus_line_type = GetFocusLineType(&shortcut_focus);
    switch (focus_line_type)
    {
    case MMIIDLE_LINE_NONE:
        //SCI_TRACE_LOW:"OpenFocusItemWin:focus_line_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_3093_112_2_18_2_24_3_48,(uint8*)"d",focus_line_type);
        break;

    case MMIIDLE_LINE_SHORTCUT:
        switch (shortcut_focus)
        {
        case MMIIDLE_SHORTCUT_FOCUS_1:
        case MMIIDLE_SHORTCUT_FOCUS_2:        
        case MMIIDLE_SHORTCUT_FOCUS_3:
        case MMIIDLE_SHORTCUT_FOCUS_4:
        case MMIIDLE_SHORTCUT_FOCUS_5:
            s_shortcut_id[shortcut_focus].func();
            break;

        default:
            break;
        }
        break;

    case MMIIDLE_LINE_TIME:
    case MMIIDLE_LINE_SIM1:
    case MMIIDLE_LINE_SIM2:
    case MMIIDLE_LINE_SIM3:
    case MMIIDLE_LINE_SIM4:
    case MMIIDLE_LINE_SCHEDULE:
    case MMIIDLE_LINE_COUNT_DOWN:
#if !((defined(MAINLCD_SIZE_176X220)||defined(MAINLCD_SIZE_128X160)) && defined(DCD_SUPPORT))	
    case MMIIDLE_LINE_WORDS: 
#endif		
    case MMIIDLE_LINE_MP3:
    case MMIIDLE_LINE_FM:
#ifdef DCD_SUPPORT 
    case MMIIDLE_LINE_DCD:
#endif
#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)     
    case MMIIDLE_LINE_WLAN:
#endif
        OpenPromptWin(focus_line_type,win_id);
        break;
        
    default:
        //SCI_TRACE_LOW:"OpenFocusItemWin:focus_line_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_3148_112_2_18_2_24_3_49,(uint8*)"d",focus_line_type);
        break;
    }
}

/*****************************************************************************/
//  Description : switch focus line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SwitchFocusLine(
                           BOOLEAN          is_increase, //in
                           MMI_WIN_ID_T     win_id
                           )
{
    uint16                      line_num            = 0;
    uint16                      focus_line_index    = 0;
    MMIIDLE_LINE_TYPE_E         focus_line_type     = MMIIDLE_LINE_NONE;
    MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus      = MMIIDLE_SHORTCUT_FOCUS_1;
    uint16                      prev_focus_index = 0;

    GetFocusLineType(&shortcut_focus);

    //get line num
    line_num = GetLineNum();
    if (1 < line_num)
    {
        //get focus line index
        focus_line_index = GetFocusLineIndex();

        //set previous focus line index
        prev_focus_index = focus_line_index;

        if (is_increase)
        {
            if (line_num > (focus_line_index+1))
            {
                focus_line_index++;
            }
            else
            {
                focus_line_index = 0;
            }
        }
        else
        {
            if (0 < focus_line_index)
            {
                focus_line_index--;
            }
            else
            {
                focus_line_index = line_num - 1;
            }
        }

        //get line type by line index
        focus_line_type = GetLineType(focus_line_index);

        
        //jump time line,for it can not be hightlight
        if(MMIIDLE_LINE_TIME == focus_line_type)
        {
            if (is_increase)
            {
                focus_line_index++;
            }
            else
            {
                focus_line_index = line_num - 1;
            }
        }

        focus_line_type = GetLineType(focus_line_index);

        //set focus line type
        SetFocusLineType(focus_line_type,shortcut_focus);

        //update
        UpdateSmartStyleIdle(FALSE,prev_focus_index,focus_line_index,win_id);
#ifdef UI_MULTILAYER_SUPPORT
        //display idle tips
        DisplayIdleTipsLayer(win_id);
#endif
    }
}

#ifdef MMI_ENABLE_DCD
/*****************************************************************************/
//  Description : 如果当前的焦点在DCD上，则切换到SHORTCUT1上去，此仅供外部调用.
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_DCDFocusToShortcut(void)
{
    MMIIDLE_LINE_TYPE_E   focus_line_type = MMIIDLE_LINE_NONE;
    MMISET_IDLE_STYLE_E   idle_style  = MMISET_IDLE_STYLE_COMMON;
    MMIIDLE_SHORTCUT_FOCUS_E    cur_shortcut = MMIIDLE_SHORTCUT_FOCUS_1;
    uint16        focus_line_index    = 0;
    
    idle_style = MMIAPISET_GetIdleStyle();
    if(MMISET_IDLE_STYLE_SMART != idle_style)
    {
       return;
    }
    //get focus line index
    focus_line_index = GetFocusLineIndex();
    //get line type by line index
    focus_line_type = GetLineType(focus_line_index);
    if(MMIIDLE_LINE_DCD == focus_line_type)
    {
       focus_line_index = 0;
       //get line type by line index
       focus_line_type = GetLineType(focus_line_index);
       SetFocusLineType(focus_line_type, cur_shortcut);
    }

}
/*****************************************************************************/
//  Description : 把当前IDLE的焦点在DCD上
//  Global resource dependence : 
//  Author: jun.hu
//  Note:  这里加上是因为用户在IDLE界面触摸笔点击DCD时，TAB键处理了事件，并发了个MSG_NOTIFY_TAB_SWITCH
//         消息上来，idle界面没有截获触摸笔消息，故在这个消息里面处理....
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_FocusToDCDLine(void)
{
    MMIIDLE_LINE_TYPE_E   focus_line_type = MMIIDLE_LINE_NONE;
    MMISET_IDLE_STYLE_E   idle_style  = MMISET_IDLE_STYLE_COMMON;
    MMIIDLE_SHORTCUT_FOCUS_E shortcut_focus = MMIIDLE_SHORTCUT_FOCUS_1;
    uint16        pre_line_index    = 0;
    uint16        cur_line_index    = 0;

    idle_style = MMIAPISET_GetIdleStyle();
    if(MMISET_IDLE_STYLE_SMART == idle_style)
    {
        focus_line_type = GetFocusLineType(&shortcut_focus);
        if(MMIIDLE_LINE_DCD != focus_line_type)
        {
            //get focus line index
            pre_line_index = GetFocusLineIndex();
            SetFocusLineType(MMIIDLE_LINE_DCD, shortcut_focus);
            cur_line_index = GetFocusLineIndex();
            UpdateSmartStyleIdle(FALSE, pre_line_index, cur_line_index, MAIN_IDLE_WIN_ID);
        }
    }
    
}
#endif
/*****************************************************************************/
//  Description : switch focus shortcut icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SwitchFocusShortCut(
                               BOOLEAN          is_increase, //in
                               MMI_WIN_ID_T     win_id
                               )
{
    uint16                      prev_focus_index = 0;
    uint16                      cur_focus_index = 0;
    MMIIDLE_LINE_TYPE_E         focus_line_type = MMIIDLE_LINE_NONE;
    MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus  = MMIIDLE_SHORTCUT_FOCUS_1;
    uint32                      shortcut_num = GetShortcutNum();

    //get focus line type
    focus_line_type = GetFocusLineType(&shortcut_focus);
    if (MMIIDLE_LINE_SHORTCUT == focus_line_type)
    {
        //set previous focus index
        prev_focus_index = (uint16)shortcut_focus;

        if (is_increase)
        {
            if (shortcut_num - 1 > shortcut_focus)
            {
                shortcut_focus++;
            }
            else
            {
                shortcut_focus = MMIIDLE_SHORTCUT_FOCUS_1;
            }
        }
        else
        {
            if (0 < shortcut_focus)
            {
                shortcut_focus--;
            }
            else
            {
                shortcut_focus = shortcut_num - 1;
            }
        }

        //set current focus index
        cur_focus_index = (uint16)shortcut_focus;
    }

    //set focus line type
    SetFocusLineType(focus_line_type,shortcut_focus);

    //update
    UpdateSmartStyleIdle(TRUE,prev_focus_index,cur_focus_index,win_id);
#ifdef UI_MULTILAYER_SUPPORT
    //display idle tips
    DisplayIdleTipsLayer(win_id);
#endif
}

/*****************************************************************************/
//  Description : reset focus to shortcut default icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetFocusToShortCut(
                                MMI_WIN_ID_T    win_id
                                )
{
    BOOLEAN                     is_shortcut = FALSE;
    uint16                      prev_focus_index = 0;
    uint16                      cur_focus_index = 0;
    MMIIDLE_LINE_TYPE_E         focus_line_type = MMIIDLE_LINE_NONE;
    MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus  = MMIIDLE_SHORTCUT_FOCUS_1;

    //get focus line type
    focus_line_type = GetFocusLineType(&shortcut_focus);
    if (MMIIDLE_LINE_SHORTCUT == focus_line_type)
    {
        is_shortcut = TRUE;

        //set previous focus index
        prev_focus_index = (uint16)shortcut_focus;
    }
    else
    {
        //set previous focus index
        prev_focus_index = GetFocusLineIndex();
    }

    if ((MMIIDLE_LINE_SHORTCUT == focus_line_type) &&
        (MMIIDLE_SHORTCUT_FOCUS_1 == shortcut_focus))
    {
        //do nothing
    }
    else
    {
        //set focus line type
        SetFocusLineType(MMIIDLE_LINE_SHORTCUT,MMIIDLE_SHORTCUT_FOCUS_1);

        if (MMIIDLE_LINE_SHORTCUT == focus_line_type)
        {
            //set current focus index
            cur_focus_index = (uint16)MMIIDLE_SHORTCUT_FOCUS_1;
        }
        else
        {
            //set current focus index
            cur_focus_index = GetFocusLineIndex();
        }

        //update
        UpdateSmartStyleIdle(is_shortcut,prev_focus_index,cur_focus_index,win_id);
#ifdef UI_MULTILAYER_SUPPORT
        //display idle tips
        DisplayIdleTipsLayer(win_id);
#endif
    }
}

/*****************************************************************************/
//  Description : open the prompt window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenPromptWin(
                         MMIIDLE_LINE_TYPE_E    focus_line_type,
                         MMI_WIN_ID_T           win_id
                         )
{
    //get line type
    switch (focus_line_type)
    {
    case MMIIDLE_LINE_TIME:        
#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
        MMIAPI_OpenTimerWin(0);
#endif
        break;
    case MMIIDLE_LINE_SIM1:
#ifdef MMI_DUALMODE_ENABLE
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        MMIAPISET_OpenSelNetWorkWin(MN_DUAL_SYS_1);
#else
        MMIAPISET_OpenMultiSimSettingsWindow();
#endif
#else        
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        if (MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
        {
            MMIAPISET_OpenSelNetWorkWin(MN_DUAL_SYS_1);

        }
        else
        {
            MMIAPIPHONE_AlertSimNorOKStatusII(MN_DUAL_SYS_1);
        }
#else
        MMIAPISET_OpenMultiSimSettingsWindow();
#endif //MMI_MULTI_SIM_SYS_SINGLE
#endif //MMI_DUALMODE_ENABLE
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMIIDLE_LINE_SIM2:
        MMIAPISET_OpenMultiSimSettingsWindow();
        break;
        
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIIDLE_LINE_SIM3:
        MMIAPISET_OpenMultiSimSettingsWindow();
        break;
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIIDLE_LINE_SIM4:
        MMIAPISET_OpenMultiSimSettingsWindow();
        break;
#endif
#endif

#ifdef CALENDAR_SUPPORT
    case MMIIDLE_LINE_SCHEDULE:
        MMIAPIIDLE_OpenScheduleWin(win_id);
        break;
#endif		
    case MMIIDLE_LINE_COUNT_DOWN:
        MMIAPICT_OpenCoutimeRunningWin();
       break;

    case MMIIDLE_LINE_MP3:
 #ifdef MMI_AUDIO_PLAYER_SUPPORT       
        MMIAPIMP3_OpenMp3Player();
 #endif
        break;

    case MMIIDLE_LINE_FM:
        MMIAPIFM_OpenMainWin();
        break;

    case MMIIDLE_LINE_WORDS:
        MMIAPISET_OpenSetIdleDisWin();
        break;

#ifdef DCD_SUPPORT 
    case MMIIDLE_LINE_DCD:
          MMIAPIDCD_ExternalOpenWin(FALSE);
        break;
#endif

#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)     
    case MMIIDLE_LINE_WLAN:
        if(MMIWIFI_STATUS_OFF != MMIAPIWIFI_GetStatus())
        {
            MMIAPIWIFI_SetInfo(FALSE);
            MMIAPIWIFI_Off();
        }
        else
        {
            MMIAPIWIFI_On(TRUE);   
        }
        break;
#endif

    case MMIIDLE_LINE_STK_IDLE_TEXT:
        break;

    default:
        //SCI_TRACE_LOW:"OpenPromptWin:focus_line_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_3489_112_2_18_2_24_4_50,(uint8*)"d",focus_line_type);
        break;
    }
}
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : smart idle handle tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SmartIdleHandleTpDown(
                                 GUI_POINT_T  tp_point,
                                 MMI_WIN_ID_T win_id
                                 )
{
    BOOLEAN                     is_shortcut = FALSE;
    uint16                      prev_index = 0;
    uint16                      cur_index = 0;   
    MMIIDLE_LINE_TYPE_E         focus_line_type = MMIIDLE_LINE_NONE;
    MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus  = MMIIDLE_SHORTCUT_FOCUS_1;

    //get tp type
    s_tp_down_type = GetTpType(tp_point);
    switch (s_tp_down_type)
    {
    case MMIIDLE_TP_NONE:
    case MMIIDLE_TP_TIME:
    case MMIIDLE_TP_DATE:
    case MMIIDLE_TP_WEEK:
    case MMIIDLE_TP_LEFT_SK:
    case MMIIDLE_TP_RIGHT_SK:
    case MMIIDLE_TP_MIDDLE_SK:
        break;

    case MMIIDLE_TP_SHORTCUT1:
    case MMIIDLE_TP_SHORTCUT2:
    case MMIIDLE_TP_SHORTCUT3:
    case MMIIDLE_TP_SHORTCUT4:
    case MMIIDLE_TP_SHORTCUT5:
    case MMIIDLE_TP_SHORTCUT6:
        //get previous focus line type
        focus_line_type = GetFocusLineType(&shortcut_focus);
        if (MMIIDLE_LINE_SHORTCUT == focus_line_type)
        {
            is_shortcut = TRUE;
            prev_index = (uint16)shortcut_focus;
        }
        else
        {
            prev_index = GetFocusLineIndex();
        }
        
        //set focus line type and shortcut focus
        focus_line_type = MMIIDLE_LINE_SHORTCUT;
        shortcut_focus = (uint32)MMIIDLE_SHORTCUT_FOCUS_1+((uint32)s_tp_down_type-MMIIDLE_TP_SHORTCUT1);/*lint !e64*/
        
        //set focus shortcut
        SetFocusLineType(focus_line_type,shortcut_focus);

        //get current focus line type
        focus_line_type = GetFocusLineType(&shortcut_focus);
        if (is_shortcut)
        {
            cur_index = (uint16)shortcut_focus;
        }
        else
        {
            cur_index = GetFocusLineIndex();
        }

        //update
        UpdateSmartStyleIdle(is_shortcut,prev_index,cur_index,win_id);
#ifdef UI_MULTILAYER_SUPPORT        
        //display idle tips
        DisplayIdleTipsLayer(win_id);
#endif
        break;

    case MMIIDLE_TP_LINE2:
    case MMIIDLE_TP_LINE3:
    case MMIIDLE_TP_LINE4:
    case MMIIDLE_TP_LINE5:
    case MMIIDLE_TP_LINE6:
        //get previous focus line index
        prev_index = GetFocusLineIndex();

        //set focus line type and shortcut focus
        focus_line_type = GetLineType((uint16)((uint32)s_tp_down_type - MMIIDLE_TP_LINE2 + GetTimeLineOffset()));
        shortcut_focus = MMIIDLE_SHORTCUT_FOCUS_1;

        //set focus shortcut
        SetFocusLineType(focus_line_type,shortcut_focus);

        //get current focus line index
        cur_index = GetFocusLineIndex();

        //update
        UpdateSmartStyleIdle(FALSE,prev_index,cur_index,win_id);
        break;

    default:
        //SCI_TRACE_LOW:"SmartIdleHandleTpDown:tp_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_3588_112_2_18_2_24_4_51,(uint8*)"d",s_tp_down_type);
        break;
    }
}

/*****************************************************************************/
//  Description : smart idle handle tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SmartIdleHandleTpUp(
                               GUI_POINT_T  tp_point,
                               MMI_WIN_ID_T win_id
                               )
{
    uint16                  shortcut_index  = 0;
    uint16                  line_index      = 0;
    MMIIDLE_TP_TYPE_E       tp_type         = MMIIDLE_TP_NONE;
    MMIIDLE_LINE_TYPE_E     focus_line_type = MMIIDLE_LINE_NONE;

    //get tp type
    tp_type = GetTpType(tp_point);
    if (s_tp_down_type != tp_type)
    {
        s_tp_down_type = MMIIDLE_TP_NONE;
        return;
    }

    switch (tp_type)
    {
    //case MMIIDLE_TP_NONE:
//    case MMIIDLE_TP_SHORTCUT1:
//#ifdef CMCC_UI_STYLE
//        //open phonebook
//        MMIAPIPB_OpenListAllGroupWin();
//#else
//        //open mainmenu
//        MMIAPIMENU_CreatMainMenu();
//#endif
//        break;

    case MMIIDLE_TP_TIME:
	 MMIAPISET_SetTime();
        //MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_TIME_WIN_TAB, FALSE);
        break;

    case MMIIDLE_TP_DATE:
    case MMIIDLE_TP_WEEK:
	 MMIAPISET_SetDate();
        //MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_DATE_WIN_TAB, FALSE);
        break;

    case MMIIDLE_TP_SHORTCUT1:
    case MMIIDLE_TP_SHORTCUT2:
    case MMIIDLE_TP_SHORTCUT3:
    case MMIIDLE_TP_SHORTCUT4:
    case MMIIDLE_TP_SHORTCUT5:
    case MMIIDLE_TP_SHORTCUT6:
        shortcut_index = (uint16)(MMIIDLE_SHORTCUT_FOCUS_1 + ((uint32)tp_type - MMIIDLE_TP_SHORTCUT1)); //lint !e778
        if(shortcut_index < (sizeof(s_shortcut_id)/sizeof(s_shortcut_id[0])))
        {
            s_shortcut_id[shortcut_index].func();
        }
        break;

    case MMIIDLE_TP_LINE2:
    case MMIIDLE_TP_LINE3:
    case MMIIDLE_TP_LINE4:
    case MMIIDLE_TP_LINE5:
    case MMIIDLE_TP_LINE6:
        line_index = (uint16)((uint32)tp_type - MMIIDLE_TP_LINE2)+GetTimeLineOffset();

        focus_line_type = GetLineType(line_index);
        OpenPromptWin(focus_line_type,win_id);
        break;
#ifdef CMCC_UI_STYLE
    case MMIIDLE_TP_LEFT_SK://open current focus item
        OpenFocusItemWin(win_id);
        break;

    case MMIIDLE_TP_MIDDLE_SK:
        MMIAPISET_EnterMonternetWin();
        break;
#else
    case MMIIDLE_TP_LEFT_SK:


#ifdef BAR_PHONE_WIDGET 
            MMIAPIMENU_CreatMainMenu(); //进入主菜单
#else
            MMIAPISET_OpenFuncKeyWin(win_id,MMISET_FUNCKEY_LEFT);
#endif            

        break;

    case MMIIDLE_TP_MIDDLE_SK://open current focus item
        OpenFocusItemWin(win_id);
        break;
#endif

    case MMIIDLE_TP_RIGHT_SK:
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
        if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
        {
            uint16 sim_sys[MMI_DUAL_SYS_MAX] = {0};
            uint32 sim_num = 0;
            const char* s_emergency_call = "112";

            sim_num = MMIAPIPHONE_GetPSReadyNum(sim_sys, MMI_DUAL_SYS_MAX);

            MMIAPICC_MakeCall((MN_DUAL_SYS_E)sim_sys[0],
                                            (uint8 *)s_emergency_call,
                                            strlen(s_emergency_call),
                                            PNULL,
                                            PNULL,
                                            CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,
                                            PNULL);
        }
        else
#endif	
#ifdef BAR_PHONE_WIDGET 
        {
            MMISET_SetIdleStyle( MMISET_IDLE_STYLE_GRID );
            MMK_SendMsg(win_id,MSG_GET_FOCUS,PNULL);
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
#else
       {
        MMIAPISET_OpenFuncKeyWin(win_id,MMISET_FUNCKEY_RIGHT);
	   }
#endif        
        break;

    default:
        //SCI_TRACE_LOW:"SmartIdleHandleTpUp:tp_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DISPLAY_3717_112_2_18_2_24_5_52,(uint8*)"d",tp_type);
        break;
    }
    s_tp_down_type = MMIIDLE_TP_NONE;

}

/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIIDLE_TP_TYPE_E GetTpType(
                                  GUI_POINT_T   tp_point
                                  )
{
    uint32                  i               = 0;
    MMIIDLE_TP_TYPE_E       tp_type         = MMIIDLE_TP_NONE;

    for (i=0; i<MMIIDLE_TP_MAX; i++)
    {
        if (GUI_PointIsInRect(tp_point,s_display_rect[i]))
        {
            tp_type = (MMIIDLE_TP_TYPE_E)i;
            break;
        }
    }

    return (tp_type);
}
#endif
/*****************************************************************************/
//  Description : update point line or shortcut
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdateSmartStyleIdle(
                                BOOLEAN         is_shortcut,//is switch in shortcut
                                uint16          prev_index, //previous index,from 0
                                uint16          cur_index,  //current index,from 0
                                MMI_WIN_ID_T    win_id
                                )
{        
    //update
    MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);    
}


#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create idle tips layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreatIdleTipsLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    int16   layer_width = 0;
    int16   layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_idle_tips_layer_handle))    
    {
        //get tips layer width
        layer_width = GetIdleTipsMaxWidth();

        //get tips layer height
        layer_height = GetIdleTipsHeight();

        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = layer_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(
            &create_info,
            &s_idle_tips_layer_handle);

        //set color key
        UILAYER_SetLayerColorKey(&s_idle_tips_layer_handle,TRUE,UILAYER_TRANSPARENT_COLOR);
    }
}


/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateSmartIdlePaintLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    uint16   layer_width = 0;
    uint16   layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_smart_idle_paint_layer_handle))    
    {
        //get tips layer width height
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&layer_width,&layer_height);    

        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = layer_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(
            &create_info,
            &s_smart_idle_paint_layer_handle);

        //set color key
        UILAYER_SetLayerColorKey(&s_smart_idle_paint_layer_handle,TRUE,UILAYER_TRANSPARENT_COLOR);
        
        append_layer.lcd_dev_info = s_smart_idle_paint_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    if (UILAYER_IsLayerActive(&s_smart_idle_paint_layer_handle))
    {
        UILAYER_Clear(&s_smart_idle_paint_layer_handle);
    }

}

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseSmartIdlePaintLayer(void)
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_smart_idle_paint_layer_handle))    
    {
        UILAYER_RELEASELAYER(&s_smart_idle_paint_layer_handle);/*lint !e506 !e774 !e831*/
    }
}
#endif
/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetSmartIdlePaintLcdDev(void)
{
    GUI_LCD_DEV_INFO            *lcd_dev_info_ptr = PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_smart_idle_paint_layer_handle))
    {
        lcd_dev_info_ptr = &s_smart_idle_paint_layer_handle;
    }
    else
#endif
    {
        lcd_dev_info_ptr = MMITHEME_GetDefaultLcdDev();/*lint !e605*/
    }
    return lcd_dev_info_ptr;
}

/*****************************************************************************/
//  Description : get idle tips max width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetIdleTipsMaxWidth(void)
{
    int16       tips_width = 0;
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,
        &lcd_width,
        &lcd_height);

    //tips max width
    tips_width = (int16)MAX(lcd_width,lcd_height);

    return (tips_width);
}

/*****************************************************************************/
//  Description : get idle tips height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetIdleTipsHeight(void)
{
    uint8           font_height = 0;
    int16           tips_height = 0;
    uint16          img_height = 0;
    MMI_WIN_ID_T    win_id = MAIN_IDLE_WIN_ID;

    //get font height
    font_height = GUI_GetFontHeight(MMIIDLE_TEXT_FONT,UNICODE_HANZI);

    GUIRES_GetImgWidthHeight(PNULL,&img_height,IMAGE_IDLE_KEYLOCK_TIPS_BG,win_id);

    //tips height
    tips_height = (int16)MAX(img_height,font_height);

    return (tips_height);
}
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : release idle tips layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ReleaseIdleTipsLayer(void)
{
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_idle_tips_layer_handle))    
    {
        UILAYER_RELEASELAYER(&s_idle_tips_layer_handle);/*lint !e506 !e774 !e831*/
    }
}

/*****************************************************************************/
//  Description : display idle tips layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayIdleTipsLayer(
                                MMI_WIN_ID_T    win_id
                                )
{
    uint16                      str_width = 0;
    GUI_RECT_T                  tips_rect = {0};
    MMI_STRING_T                tips_string = {0};
    MMI_TEXT_ID_T               text_id = 0;
    GUISTR_STYLE_T              str_style = {0};
    GUISTR_STATE_T              str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    MMIIDLE_LINE_TYPE_E         focus_line_type = MMIIDLE_LINE_NONE;
    MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus = MMIIDLE_SHORTCUT_FOCUS_1;
    UILAYER_APPEND_BLT_T        append_layer = {0};
    GUI_LCD_DEV_INFO            *layer_handle_ptr = &s_idle_tips_layer_handle;

    //get focus line type
    focus_line_type = GetFocusLineType(&shortcut_focus);

    //stop idle tips timer
    StopIdleTipsTimer();

    if (UILAYER_IsLayerActive(layer_handle_ptr))
    {
        if (MMIIDLE_LINE_SHORTCUT == focus_line_type)
        {
            //get tips text id
            //text_id = GetIdleTipsTextId(shortcut_focus);
             text_id = s_shortcut_id[shortcut_focus].txt_id;
            //get string
            MMI_GetLabelTextByLang(text_id,&tips_string);

            //set string style
            str_style.font       = MMIIDLE_TIPS_TEXT_FONT;
            str_style.font_color = MMI_DEFAULT_TEXT_COLOR;
            str_style.align      = ALIGN_HVMIDDLE;

            //get string width
            str_width = GUISTR_GetStringWidth(&str_style,
                &tips_string,
                str_state);

            //get tips rect
            tips_rect = GetIdleTipsRect(str_width,shortcut_focus);

            //set layer position
            UILAYER_SetLayerPosition(layer_handle_ptr,
                tips_rect.left,
                tips_rect.top);

            //clear
            UILAYER_Clear(layer_handle_ptr);

            //blt
            append_layer.lcd_dev_info = *layer_handle_ptr;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);

            //display bg image
            GUIRES_DisplayImg(PNULL,
                &tips_rect,
                PNULL,
                win_id,
                IMAGE_IDLE_KEYLOCK_TIPS_BG,
                layer_handle_ptr);

            //display text
            GUISTR_DrawTextToLCDInRect(layer_handle_ptr,
                &tips_rect,
                &tips_rect,
                &tips_string,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO);

            //start tips timer
            StartIdleTipsTimer();
        }
        else
        {
            //hide idle tips
            HideIdleTipsLayer();
        }
    }
}
#endif
/*****************************************************************************/
//  Description : get idle current shortcut tips text id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetIdleTipsTextId(
                                      MMIIDLE_SHORTCUT_FOCUS_E  shortcut_focus
                                      )
{
    MMI_TEXT_ID_T               text_id = TXT_NULL;

    if (shortcut_focus < (sizeof(s_shortcut_id)/sizeof(s_shortcut_id[0])))
    {
        text_id = s_shortcut_id[shortcut_focus].txt_id;
    }

    return (text_id);
}

/*****************************************************************************/
//  Description : get idle current shortcut tips rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetIdleTipsRect(
                                 uint16                     str_width,
                                 MMIIDLE_SHORTCUT_FOCUS_E   shortcut_focus
                                 )
{
    GUI_RECT_T      tips_rect = {0};
    GUI_RECT_T 	    all_shortcut_rect = {0};
    GUI_RECT_T 	    shortcut_rect[MMIIDLE_SHORTCUT_NUM] = {0};
    uint32          shortcut_num = GetShortcutNum();
    

    //set all shortcut rect
    all_shortcut_rect = GetShortcutRect();

    //get shortcut rect  
    GUI_CreateMatrixRect(&all_shortcut_rect,MMIDILE_SHORTCUT_HIRIZONAL_MARGIN,
            0,0,0,1,shortcut_num,shortcut_rect);

    //add space
    str_width = (uint16)(str_width + MMIIDLE_TIPS_H_SPACE*2);

    //get tips rect
    tips_rect.bottom = all_shortcut_rect.top - MMIIDLE_TIPS_V_SPACE;
    tips_rect.top    = tips_rect.bottom - GetIdleTipsHeight() ;
    tips_rect.left   = shortcut_rect[shortcut_focus].left;
    tips_rect.right  = (int16)(tips_rect.left + str_width - 1);

    return (tips_rect);
}

/*****************************************************************************/
//  Description : start idle tips timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartIdleTipsTimer(void)
{
    s_idle_tips_timer_id = MMK_CreateTimerCallback(MMIIDLE_TIPS_TIMER_PERIOD,
                                HandleIdleTipsTimer,
                                PNULL,
                                FALSE);
}

/*****************************************************************************/
//  Description : idle tips timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopIdleTipsTimer(void)
{
    if (0 <s_idle_tips_timer_id)
    {
        MMK_StopTimer(s_idle_tips_timer_id);
        s_idle_tips_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle idle tips timer msg
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIdleTipsTimer(
                               uint8    timer_id,
                               uint32   param
                               )
{
#ifdef UI_MULTILAYER_SUPPORT    
    if (s_idle_tips_timer_id == timer_id)
    {
        //hide idle tips
        HideIdleTipsLayer();
    }
#endif
    GUI_INVALID_PARAM(param);/*lint !e522*/
}
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : hide idle tips
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HideIdleTipsLayer(void)
{
    GUI_RECT_T          layer_rect = {0};
    GUI_LCD_DEV_INFO    *layer_handle_ptr = &s_idle_tips_layer_handle;
    
    if (UILAYER_IsLayerActive(layer_handle_ptr))
    {
        //get layer
        UILAYER_RemoveBltLayer(layer_handle_ptr);

        //set layer rect
        layer_rect.right  = (int16)(GetIdleTipsMaxWidth() - 1);
        layer_rect.bottom = (int16)(GetIdleTipsHeight() - 1);
        
        MMITHEME_StoreUpdateRect(layer_handle_ptr,layer_rect);
    }
}
#endif
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : display stk idle text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayStkIdleText(
                          BOOLEAN               is_only_update, //?????????
                          MMI_WIN_ID_T          win_id
                          )
{
    uint16              line_index = 0;
    GUI_RECT_T          rect = {0};
    uint16              display_max_len = 0;
    GUI_LCD_DEV_INFO   *lcd_dev_ptr = GetSmartIdlePaintLcdDev();
    IDLE_WIN_INFO_T *idle_info_ptr = MMIIDLE_GetIdleWinInfo();
    IDLE_STK_TEXT_INFO *stk_text_ptr = MMIIDLECOM_GetStkTextInfo();
    MMI_STRING_T idle_text = {0};
    BOOLEAN is_icon_exist = FALSE;
    GUIIMG_BITMAP_T *icon_bmp_ptr = PNULL;
    BOOLEAN is_self_explanatory = FALSE;

    if (PNULL == idle_info_ptr
        || PNULL == stk_text_ptr)
    {
        return;
    }
    
    if (!idle_info_ptr->is_display_stk_idle_text
        || PNULL == stk_text_ptr->stk_idle_str.wstr_ptr
        || 0 == stk_text_ptr->stk_idle_str.wstr_len)
    {
        return;
    }
    
    //get line index
    if (is_only_update)
    {
        line_index = GetLineIndex(MMIIDLE_LINE_STK_IDLE_TEXT);
    }
    else
    {
        line_index = GetLineNum();
    }

    //SCI_ASSERT(0 < line_index);
    

    //display line and select image
    DisplayLineAndSelImg(line_index,win_id,MMIIDLE_LINE_STK_IDLE_TEXT,lcd_dev_ptr);

    //display icon
    icon_bmp_ptr = (GUIIMG_BITMAP_T *)MMIAPISTK_GetIdleTextIcon(&is_icon_exist, &is_self_explanatory);

    if (is_icon_exist && PNULL != icon_bmp_ptr && PNULL != icon_bmp_ptr->bit_ptr)//??Icon
    {
        GUI_RECT_T show_rect = {0};

        rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);
        show_rect = GUI_GetCenterRect(rect, icon_bmp_ptr->img_width, icon_bmp_ptr->img_height);
        show_rect.left = rect.left;
        show_rect.right = rect.right;

        GUIIMG_DisplayBmp(FALSE, &show_rect, 0, icon_bmp_ptr, lcd_dev_ptr);
    }

    //display string
    if (!is_icon_exist || !is_self_explanatory)
    {
        if (is_icon_exist && PNULL != icon_bmp_ptr && PNULL != icon_bmp_ptr->bit_ptr)//??ICON
        {
            rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_TEXT);
        }
        else
        {
            rect = GetCurItemRect(line_index,MMIIDLE_PROMPT_POS_ICON);
        }

        display_max_len = MMI_MAINSCREEN_WIDTH-(rect.left+1)-MMIIDLE_MARGIN_WIDTH-1;

        if (stk_text_ptr->str_index > stk_text_ptr->stk_idle_str.wstr_len - 1)
        {
            stk_text_ptr->str_index = 0;
        }

        idle_text.wstr_ptr = stk_text_ptr->stk_idle_str.wstr_ptr + stk_text_ptr->str_index;
        idle_text.wstr_len = stk_text_ptr->stk_idle_str.wstr_len - stk_text_ptr->str_index;

        if (stk_text_ptr->is_scroll)
        {
            MAIN_StopIdleSTKTextTimer();
            MAIN_StartIdleSTKTextTimer();
        }

        MMIIDLE_DisplayLineTextInRect(
            lcd_dev_ptr,
            &rect,
            MMIIDLE_TEXT_FONT,
            MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_NETWORKNAME),
            idle_text.wstr_ptr, 
            idle_text.wstr_len
            );


        if (stk_text_ptr->is_scroll && stk_text_ptr->str_index < stk_text_ptr->stk_idle_str.wstr_len - 1)
        {
            stk_text_ptr->str_index ++;
        }
        else
        {
            stk_text_ptr->str_index = 0;
        }
    }

    if (!is_only_update)
    {
        //set words rect
        SetPromptDisplayRect(line_index,MMIIDLE_TP_LINE2+line_index);

        //add line number
        SetLineTypeAndNum(MMIIDLE_LINE_STK_IDLE_TEXT);
    }
}
#endif
#endif //#if defined MMI_SMART_IDLE_SUPPORT


/*Edit by script, ignore 2 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527
