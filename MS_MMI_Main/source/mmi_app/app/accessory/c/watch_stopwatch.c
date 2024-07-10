/*****************************************************************************
** File Name:   watch_stopwatch.c                                            *
** Author:      fangfang.yao                                                 *
** Date:        2020/02/20                                                   *
** Copyright:                                                                *
** Description: This file is used to process msg to main window of stop watch*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 02/20/2020           fangfang.yao           Create                        *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "guibutton.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmidisplay_data.h"

#include "os_api.h"
#include "mmk_type.h"

#include "mmi_default.h"
#include "mmiacc_id.h"
#include "mmiacc_position.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmiset_export.h"
#include "mmiacc_nv.h"
#include "guistring.h"

#include "mmi_timer_export.h"

#include "watch_stopwatch.h"
#include "watch_common.h"
#include "mmicom_trace.h"
#include "watch_common_btn.h"
#ifdef ADULT_WATCH_SUPPORT
#include "guires.h"
#endif
/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
 typedef enum
{
  STOPWATCH_STATUS_STOP,
  STOPWATCH_STATUS_PAUSE,
  STOPWATCH_STATUS_RUNNING
} STOPWATCH_STATUS_E;

typedef struct  //定义自己的时间类型
{
    uint8   dsec;       //one-tenth of the second-[0,9]
    uint8   sec;        // secondsafter the minute - [0,59]
    uint8   min;        // minutesafter the hour - [0,59]
    uint32  hour;       // hours since midnight - [0-...]
}STOPWATCH_TIME_T;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define STOPWATCH_TIME_STRLENGTH        (32)  // X...X:XX:XX.X(小时最多7位)
#ifdef ADULT_WATCH_SUPPORT
#define STOPWATCH_TIME_DISP_RECT        DP2PX_RECT(65,96,215,128)
#define STOPWATCH_TIME_DISP_BG_RECT     DP2PX_RECT(65,96,215,128)
#define STOPWATCH_BTN_SINGLE_RECT       DP2PX_RECT(96,144,143,191)
#define STOPWATCH_BTN_TWO_LEFT_RECT     DP2PX_RECT(60,144,107,191)
#define STOPWATCH_BTN_TWO_RIGHT_RECT    DP2PX_RECT(132,144,179,191)
#define STOPWATCH_TIME_DISP_FONT_BIG    DP2PX_FONT(SONG_FONT_36)
#define STOPWATCH_TIME_DISP_FONT_SMALL  DP2PX_FONT(SONG_FONT_24)
#else
#ifdef SCREEN_SHAPE_CIRCULAR
#define STOPWATCH_TIME_DISP_RECT        DP2PX_RECT(40,90,199,130)
#define STOPWATCH_TIME_DISP_BG_RECT     DP2PX_RECT(40,90,199,130)
#define STOPWATCH_BTN_SINGLE_RECT       DP2PX_RECT(96,164,143,211)
#define STOPWATCH_BTN_TWO_LEFT_RECT     DP2PX_RECT(60,164,107,211)
#define STOPWATCH_BTN_TWO_RIGHT_RECT    DP2PX_RECT(132,164,179,211)
#define STOPWATCH_TIME_DISP_FONT_BIG    DP2PX_FONT(SONG_FONT_48)
#define STOPWATCH_TIME_DISP_FONT_SMALL  DP2PX_FONT(SONG_FONT_36)
#else
#define STOPWATCH_TIME_DISP_RECT        {30,15,219,85}
#define STOPWATCH_TIME_DISP_BG_RECT     {0,15,239,85}
#define STOPWATCH_TIME_DISP_FONT_BIG    SONG_FONT_48
#define STOPWATCH_TIME_DISP_FONT_SMALL  SONG_FONT_36
#endif
#endif
#define STOPWATCH_TIMER_INTERVAL        (10)
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL uint64 s_stopwatch_start_time   = 0;    //每次按下start的时刻 (以dsec为单位)
LOCAL uint64 s_stopwatch_stop_time    = 0;    //每次按下stop时，time2已经计时经过的时间 (以dsec为单位)
LOCAL uint64 s_stopwatch_current_time = 0;    //总计时时间,到目前为止经过的时间(以dsec为单位)
LOCAL uint8  s_stopwatch_timeId       = 0;    //码表的timerID, 计时时不为0
LOCAL STOPWATCH_STATUS_E s_stopwatch_status = STOPWATCH_STATUS_STOP;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

LOCAL void StopWatch_TimeBackgroundDisplay( void )
{
    GUI_RECT_T          dispRect    = STOPWATCH_TIME_DISP_BG_RECT;
    GUI_LCD_DEV_INFO    lcdDevInfo  = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    LCD_FillRect(&lcdDevInfo, dispRect, MMI_BLACK_COLOR);
}

LOCAL void StopWatch_SetBtnVisibleStatus( STOPWATCH_STATUS_E status)
{
    if (STOPWATCH_STATUS_RUNNING == status)
    {
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID, TRUE,  FALSE);
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID,  FALSE, FALSE);
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_RESET_BTN_CTRL_ID, TRUE,  FALSE);
    }
    else if(STOPWATCH_STATUS_PAUSE == status)
    {
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID,  TRUE,  FALSE);
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_RESET_BTN_CTRL_ID, TRUE,  FALSE);
    }
    else if(STOPWATCH_STATUS_STOP == status)
    {
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID,  TRUE,  FALSE);
        GUIBUTTON_SetVisible(WATCH_STOPWATCH_RESET_BTN_CTRL_ID, FALSE, FALSE);
    }
    else
    {
        TRACE_APP_STOPWATCH("status = %d",status);
    }
}

LOCAL void StopWatch_GetCurTimeAndFont( uint8 *str_ptr, const uint64 curent_time, GUI_FONT_T *font_ptr )
{
    STOPWATCH_TIME_T    curTime    = {0};
    uint32              timeTemp   = (uint32)(curent_time / 10);   //bug1999095
    uint32              timeSec    = 0;

    if (PNULL == str_ptr)
    {
        TRACE_APP_STOPWATCH("str_ptr is PNULL!");
        return;
    }
    curTime.dsec    = (uint32)timeTemp % 100;
    timeSec         = (uint32)timeTemp / 100;//以sec为单位   //bug1999095
    curTime.sec     = timeSec % 60;
    timeSec         = timeSec / 60; //以min为单位
    curTime.min     = timeSec % 60;
    timeSec         = timeSec / 60; //以hour为单位
    curTime.hour    = timeSec;

    if(!curTime.hour)//没有超过1小时,只显示 00:00.0
    {
        *font_ptr = STOPWATCH_TIME_DISP_FONT_BIG; // BUG- 1327280
        sprintf((char*)str_ptr, "%02d:%02d.%02d", curTime.min, curTime.sec,curTime.dsec);   //bug1999095
    }
    else //超过1小时，则显示 0...0:00:00.0
    {
        *font_ptr = STOPWATCH_TIME_DISP_FONT_SMALL; // BUG- 1327280
        sprintf((char*)str_ptr, "%d:%02d:%02d.%02d", curTime.hour, curTime.min, curTime.sec,curTime.dsec);   //bug1999095
    }
}

LOCAL void StopWatch_DisplayCurTime(
                                            MMI_WIN_ID_T    win_id,
                                            GUI_RECT_T      time_rect,
                                            GUISTR_STYLE_T  *style_ptr,
                                            uint64          current_time
                                            )
{
    uint8               timeStr[STOPWATCH_TIME_STRLENGTH+1]     = {0};
    wchar               timeTemp[STOPWATCH_TIME_STRLENGTH+1]    = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_LCD_DEV_INFO    lcdDevInfo  = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T        curTime     = {0};
    GUI_RECT_T          rect        = {0};
    size_t              strLen      = 0;
    GUI_FONT_T          font        = 0;
    //区域赋值
    rect    = time_rect;

    //入参检测
    if (GUI_IsInvalidRect(rect))
    {
        TRACE_APP_STOPWATCH("the rect is invalid!");
        return;
    }

    if (PNULL == style_ptr)
    {
        TRACE_APP_STOPWATCH("style_ptr is PNULL!");
        return;
    }

    //赋值
    StopWatch_GetCurTimeAndFont(timeStr, current_time, &font);// BUG- 1327280
    strLen = strlen((char *)timeStr);

    style_ptr->font = font;// BUG- 1327280

    MMI_STRNTOWSTR(timeTemp, STOPWATCH_TIME_STRLENGTH, timeStr, STOPWATCH_TIME_STRLENGTH, strLen);
    curTime.wstr_ptr = timeTemp;
    curTime.wstr_len = strLen;

    //获取当前主题的字体颜色
    style_ptr->font_color = MMI_WHITE_COLOR;
#ifdef ADULT_WATCH_SUPPORT
    style_ptr->align = ALIGN_LVMIDDLE; // BUG- 1327163 
#else
    style_ptr->align = ALIGN_BOTTOM;//ALIGN_HVMIDDLE; //bug1999095 ALIGN_HVMIDDLE 刷字时屏幕会抖动
#endif
    // display
    GUISTR_DrawTextToLCDInRect(
                                (const GUI_LCD_DEV_INFO *)&lcdDevInfo,
                                (const GUI_RECT_T       *)&rect,       //the fixed display area
                                (const GUI_RECT_T       *)&rect,       //用户要剪切的实际区域
                                (const MMI_STRING_T     *)&curTime,
                                style_ptr,
                                state,
                                GUISTR_TEXT_DIR_AUTO
                                );
}

LOCAL void StopWatch_TimeUpdate_CallBack( uint8 timer_id )
{
    SCI_TICK_TIME_T tickTime    = {0};//当前时刻，ticktime
    uint32          currentTime = 0;  //将ticktime转化为以dsec为单位的数字

    TRACE_APP_STOPWATCH("timer_id = %d , s_stopwatch_timeId = %d",timer_id,s_stopwatch_timeId);
    if (timer_id == s_stopwatch_timeId)
    {
        if(STOPWATCH_STATUS_RUNNING == s_stopwatch_status)
        {
            SCI_GetTickTime(&tickTime);
            currentTime = tickTime.second * 1000 + tickTime.milliseconds;
            s_stopwatch_current_time  = s_stopwatch_stop_time + currentTime - s_stopwatch_start_time;//总计时时间
            if (WATCH_STOPWATCH_WIN_ID == MMK_GetFocusWinId()||WATCH_STOPWATCH_WIN_ID == MMK_GetFocusChildWinId())
            {
                if ( STOPWATCH_STATUS_RUNNING == s_stopwatch_status )//只有计时进行时才刷新
                {
                    MMK_SendMsg(WATCH_STOPWATCH_WIN_ID, MSG_ACC_UPDATE_STOPWATCH, PNULL);
                }
                else
                {
                    TRACE_APP_STOPWATCH("s_stopwatch_status = %d",s_stopwatch_status);
                }
            }
        }
        else
        {
            if( s_stopwatch_timeId != 0)
            {
                MMK_StopTimer(s_stopwatch_timeId);
                s_stopwatch_timeId   = 0;
            }
        }
    }
}

LOCAL void StopWatch_ResetTime( MMI_WIN_ID_T win_id )
{
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T  btnRect = STOPWATCH_BTN_SINGLE_RECT;
#endif
    if(STOPWATCH_STATUS_STOP == s_stopwatch_status)
    {
        TRACE_APP_STOPWATCH("s_stopwatch_status = %d",s_stopwatch_status);
        return;
    }

    if( s_stopwatch_timeId != 0)
    {
        MMK_StopTimer(s_stopwatch_timeId);
        s_stopwatch_timeId   = 0;
    }
    s_stopwatch_current_time  = 0;
    s_stopwatch_start_time    = 0;
    s_stopwatch_stop_time     = 0;
    s_stopwatch_status        = STOPWATCH_STATUS_STOP;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUIBUTTON_SetRect(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID, &btnRect);
#else
    WatchCOM_Btn_SetPos_1Btn(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID);
#endif
    StopWatch_SetBtnVisibleStatus(s_stopwatch_status);

    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_COMMON_START,STXT_RETURN,FALSE);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);

}

LOCAL void StopWatch_StartOrPauseTime( MMI_WIN_ID_T win_id )
{
    SCI_TICK_TIME_T tickTime    = {0};//当前时刻，ticktime
    uint64          currentTime = 0;  //将ticktime转化为以dsec为单位的数字
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T      leftBtnRect = STOPWATCH_BTN_TWO_LEFT_RECT;
    GUI_RECT_T      rightBtnRect= STOPWATCH_BTN_TWO_RIGHT_RECT;
#endif
    if(STOPWATCH_STATUS_RUNNING == s_stopwatch_status)
    {
        if( s_stopwatch_timeId != 0)
        {
            MMK_StopTimer(s_stopwatch_timeId);
            s_stopwatch_timeId   = 0;
        }
        s_stopwatch_stop_time   = s_stopwatch_current_time;
        s_stopwatch_status      = STOPWATCH_STATUS_PAUSE;
#ifdef SCREEN_SHAPE_CIRCULAR
        GUIBUTTON_SetRect(WATCH_STOPWATCH_RESET_BTN_CTRL_ID, &leftBtnRect);
        GUIBUTTON_SetRect(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID, &rightBtnRect);
#else
        WatchCOM_Btn_SetPos_2Btn( WATCH_STOPWATCH_RESET_BTN_CTRL_ID , WATCH_STOPWATCH_PLAY_BTN_CTRL_ID);
#endif
        GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_COMMON_PLAY,STXT_RETURN,FALSE);
    }
    else //没有running的情况下，则按下的为start(不管是stop还是从未开始，类似处理)
    {
        SCI_GetTickTime(&tickTime);
        currentTime             = ((uint64)(tickTime.second) * 1000 + (uint64)(tickTime.milliseconds));
        s_stopwatch_start_time  = currentTime;
        s_stopwatch_status      = STOPWATCH_STATUS_RUNNING;
        s_stopwatch_timeId      = MMK_CreateTimerCallback(STOPWATCH_TIMER_INTERVAL,StopWatch_TimeUpdate_CallBack,NULL,TRUE);
#ifdef SCREEN_SHAPE_CIRCULAR
        GUIBUTTON_SetRect(WATCH_STOPWATCH_RESET_BTN_CTRL_ID, &leftBtnRect);
        GUIBUTTON_SetRect(WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID, &rightBtnRect);
#else
        WatchCOM_Btn_SetPos_2Btn( WATCH_STOPWATCH_RESET_BTN_CTRL_ID , WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID);
#endif
        GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_PAUSE,STXT_RETURN,FALSE);
    }
    StopWatch_SetBtnVisibleStatus(s_stopwatch_status);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);

}

#ifdef SCREEN_SHAPE_CIRCULAR
LOCAL void WATCHCOM_DisplayStopwatchBackground( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T         disp_rect    = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO   lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    GUIRES_DisplayImg(PNULL, &disp_rect, PNULL, win_id, res_aw_stopwatch_bg,&lcd_dev_info);

}
#endif

LOCAL MMI_RESULT_E HandleMsgStopWatchWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;
    GUI_RECT_T      timeRect    = STOPWATCH_TIME_DISP_RECT;  //time总区域指针
    GUISTR_STYLE_T  timeStyle   = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            //设置button的显示字符
            if (STOPWATCH_STATUS_RUNNING == s_stopwatch_status)
            {
#ifdef SCREEN_SHAPE_CIRCULAR
                GUI_RECT_T leftBtnRect = STOPWATCH_BTN_TWO_LEFT_RECT;
                GUI_RECT_T rightBtnRect = STOPWATCH_BTN_TWO_RIGHT_RECT;
                GUIBUTTON_SetRect(WATCH_STOPWATCH_RESET_BTN_CTRL_ID, &leftBtnRect);
                GUIBUTTON_SetRect(WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID, &rightBtnRect);
#else
                WatchCOM_Btn_SetPos_2Btn( WATCH_STOPWATCH_RESET_BTN_CTRL_ID , WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID);
#endif
                GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_PAUSE,STXT_RETURN,FALSE);
            }
            else if(STOPWATCH_STATUS_PAUSE == s_stopwatch_status)
            {
#ifdef SCREEN_SHAPE_CIRCULAR
                GUI_RECT_T leftBtnRect = STOPWATCH_BTN_TWO_LEFT_RECT;
                GUI_RECT_T rightBtnRect = STOPWATCH_BTN_TWO_RIGHT_RECT;
                GUIBUTTON_SetRect(WATCH_STOPWATCH_RESET_BTN_CTRL_ID, &leftBtnRect);
                GUIBUTTON_SetRect(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID, &rightBtnRect);
#else
                WatchCOM_Btn_SetPos_2Btn( WATCH_STOPWATCH_RESET_BTN_CTRL_ID , WATCH_STOPWATCH_PLAY_BTN_CTRL_ID);
#endif
                GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_COMMON_PLAY,STXT_RETURN,FALSE);
            }
            else if(STOPWATCH_STATUS_STOP == s_stopwatch_status)
            {
#ifdef SCREEN_SHAPE_CIRCULAR
                GUI_RECT_T  btnRect = STOPWATCH_BTN_SINGLE_RECT;
                GUIBUTTON_SetRect(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID, &btnRect);
#else
                WatchCOM_Btn_SetPos_1Btn( WATCH_STOPWATCH_PLAY_BTN_CTRL_ID);
#endif
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_COMMON_START,STXT_RETURN,FALSE);
            }
            else
            {
                TRACE_APP_STOPWATCH("s_stopwatch_status = %d",s_stopwatch_status);
            }

            //设置BUTTON显隐状态
            StopWatch_SetBtnVisibleStatus(s_stopwatch_status);
        }
        break;

        case MSG_FULL_PAINT:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            WATCHCOM_DisplayStopwatchBackground(win_id);
#else
            WATCHCOM_DisplayBackground(win_id);
#endif
            StopWatch_DisplayCurTime(win_id, timeRect, &timeStyle, s_stopwatch_current_time);//没有开始计时时，显示00:00:00.0
        }
        break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        {
            StopWatch_ResetTime(win_id);
        }
        break;

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            StopWatch_StartOrPauseTime(win_id);
        }
        break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

        case MSG_ACC_UPDATE_STOPWATCH:
        {
            //显示时间文本背景
            StopWatch_TimeBackgroundDisplay();
            //显示倒计时时间信息
            StopWatch_DisplayCurTime(win_id, timeRect, &timeStyle, s_stopwatch_current_time);;
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
            if(WATCH_STOPWATCH_RESET_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
            {
                StopWatch_ResetTime(win_id);
            }
            else if(WATCH_STOPWATCH_PLAY_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//play button被选中
            {
                StopWatch_StartOrPauseTime(win_id);
            }
            else if(WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//pause button被选中
            {
                StopWatch_StartOrPauseTime(win_id);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT

        case MSG_KEYDOWN_RED:
        {
            SCI_TRACE_LOW("[HandleMsgStopWatchWindow]:receive MSG_KEYDOWN_RED");
            if(STOPWATCH_STATUS_RUNNING == s_stopwatch_status)
            {
                StopWatch_StartOrPauseTime(win_id);
            }
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            if (STOPWATCH_STATUS_STOP == s_stopwatch_status)
            {
                if( s_stopwatch_timeId != 0)
                {
                    MMK_StopTimer(s_stopwatch_timeId);
                    s_stopwatch_timeId   = 0;
                }
                s_stopwatch_current_time = 0;
                s_stopwatch_start_time   = 0;
                s_stopwatch_stop_time    = 0;
                s_stopwatch_status       = STOPWATCH_STATUS_STOP;
            }
        }
        break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

// window table of stop watch win
WINDOW_TABLE(WATCH_STOPWATCH_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(WATCH_STOPWATCH_WIN_ID),
    WIN_FUNC((uint32)HandleMsgStopWatchWindow),
    CREATE_BUTTON_CTRL(image_watch_stopwatch_reset, WATCH_STOPWATCH_RESET_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(image_watch_stopwatch_play,  WATCH_STOPWATCH_PLAY_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(image_watch_stopwatch_pause, WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
#endif
    END_WIN
};


/*****************************************************************************/
//  Description: enter stop watch window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchStopWatch_MainWin_Enter( void )
{
    MMK_CreateWin((uint32 *)WATCH_STOPWATCH_WIN_TAB,PNULL);
}
