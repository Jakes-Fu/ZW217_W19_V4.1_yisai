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

#include "zte_stopwatch_id.h"
#include "zte_stopwatch_image.h"
#include "zte_stopwatch_win.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
 typedef enum
{
  STOPWATCH_STATUS_STOP,
  STOPWATCH_STATUS_PAUSE,
  STOPWATCH_STATUS_RUNNING
} STOPWATCH_STATUS_E;

typedef struct  //�����Լ���ʱ������
{
    uint8   dsec;       //one-tenth of the second-[0,9]
    uint8   sec;        // secondsafter the minute - [0,59]
    uint8   min;        // minutesafter the hour - [0,59]
    uint32  hour;       // hours since midnight - [0-...]
}STOPWATCH_TIME_T;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define STOPWATCH_TIME_STRLENGTH        (32)  // X...X:XX:XX.X(Сʱ���7λ)


#define STOPWATCH_TIME_DISP_RECT        DP2PX_RECT(42,84,197,130)
#define STOPWATCH_TIME_DISP_BG_RECT     DP2PX_RECT(42,84,197,130)
#define STOPWATCH_TIME_DISP_FONT_BIG    DP2PX_FONT(SONG_FONT_48)
#define STOPWATCH_TIME_DISP_FONT_SMALL  DP2PX_FONT(SONG_FONT_36)
#define STOPWATCH_BTN_SINGLE_RECT       DP2PX_RECT(91,172,149,230)
#define STOPWATCH_BTN_TWO_LEFT_RECT     DP2PX_RECT(51,172,109,230)
#define STOPWATCH_BTN_TWO_RIGHT_RECT    DP2PX_RECT(130,172,188,230)

#define STOPWATCH_TIMER_INTERVAL        (10)
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL uint64 s_stopwatch_start_time   = 0;    //ÿ�ΰ���start��ʱ�� (��dsecΪ��λ)
LOCAL uint64 s_stopwatch_stop_time    = 0;    //ÿ�ΰ���stopʱ��time2�Ѿ���ʱ������ʱ�� (��dsecΪ��λ)
LOCAL uint64 s_stopwatch_current_time = 0;    //�ܼ�ʱʱ��,��ĿǰΪֹ������ʱ��(��dsecΪ��λ)
LOCAL uint8  s_stopwatch_timeId       = 0;    //����timerID, ��ʱʱ��Ϊ0
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
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_STOP, TRUE,  FALSE);
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_START,  FALSE, FALSE);
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_RESET, TRUE,  FALSE);
    }
    else if(STOPWATCH_STATUS_PAUSE == status)
    {
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_STOP, FALSE, FALSE);
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_START,  TRUE,  FALSE);
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_RESET, TRUE,  FALSE);
    }
    else if(STOPWATCH_STATUS_STOP == status)
    {
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_STOP, FALSE, FALSE);
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_START,  TRUE,  FALSE);
        GUIBUTTON_SetVisible(STOPWATCH_CTRL_RESET, FALSE, FALSE);
    }
    else
    {
        SCI_TRACE_LOW("status = %d",status);
    }
}

LOCAL void StopWatch_GetCurTimeAndFont( uint8 *str_ptr, const uint64 curent_time, GUI_FONT_T *font_ptr )
{
    STOPWATCH_TIME_T    curTime    = {0};
    uint32              timeTemp   = (uint32)(curent_time / 10);   //bug1999095
    uint32              timeSec    = 0;

    if (PNULL == str_ptr)
    {
        SCI_TRACE_LOW("str_ptr is PNULL!");
        return;
    }
    curTime.dsec    = (uint32)timeTemp % 100;
    timeSec         = (uint32)timeTemp / 100;//��secΪ��λ   //bug1999095
    curTime.sec     = timeSec % 60;
    timeSec         = timeSec / 60; //��minΪ��λ
    curTime.min     = timeSec % 60;
    timeSec         = timeSec / 60; //��hourΪ��λ
    curTime.hour    = timeSec;

    if(!curTime.hour)//û�г���1Сʱ,ֻ��ʾ 00:00.0
    {
        *font_ptr = STOPWATCH_TIME_DISP_FONT_BIG; // BUG- 1327280
        sprintf((char*)str_ptr, "%02d:%02d.%02d", curTime.min, curTime.sec,curTime.dsec);   //bug1999095
    }
    else //����1Сʱ������ʾ 0...0:00:00.0
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
    //����ֵ
    rect    = time_rect;

    //��μ��
    if (GUI_IsInvalidRect(rect))
    {
        SCI_TRACE_LOW("the rect is invalid!");
        return;
    }

    if (PNULL == style_ptr)
    {
        SCI_TRACE_LOW("style_ptr is PNULL!");
        return;
    }

    //��ֵ
    StopWatch_GetCurTimeAndFont(timeStr, current_time, &font);// BUG- 1327280
    strLen = strlen((char *)timeStr);

    style_ptr->font = font;// BUG- 1327280

    MMI_STRNTOWSTR(timeTemp, STOPWATCH_TIME_STRLENGTH, timeStr, STOPWATCH_TIME_STRLENGTH, strLen);
    curTime.wstr_ptr = timeTemp;
    curTime.wstr_len = strLen;

    //��ȡ��ǰ�����������ɫ
    style_ptr->font_color = MMI_WHITE_COLOR;

    style_ptr->align = ALIGN_BOTTOM;//ALIGN_HVMIDDLE; //bug1999095 ALIGN_HVMIDDLE ˢ��ʱ��Ļ�ᶶ��

    // display
    GUISTR_DrawTextToLCDInRect(
                                (const GUI_LCD_DEV_INFO *)&lcdDevInfo,
                                (const GUI_RECT_T       *)&rect,       //the fixed display area
                                (const GUI_RECT_T       *)&rect,       //�û�Ҫ���е�ʵ������
                                (const MMI_STRING_T     *)&curTime,
                                style_ptr,
                                state,
                                GUISTR_TEXT_DIR_AUTO
                                );
}

LOCAL void StopWatch_TimeUpdate_CallBack( uint8 timer_id )
{
    SCI_TICK_TIME_T tickTime    = {0};//��ǰʱ�̣�ticktime
    uint32          currentTime = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������

    SCI_TRACE_LOW("timer_id = %d , s_stopwatch_timeId = %d",timer_id,s_stopwatch_timeId);
    if (timer_id == s_stopwatch_timeId)
    {
        if(STOPWATCH_STATUS_RUNNING == s_stopwatch_status)
        {
            SCI_GetTickTime(&tickTime);
            currentTime = tickTime.second * 1000 + tickTime.milliseconds;
            s_stopwatch_current_time  = s_stopwatch_stop_time + currentTime - s_stopwatch_start_time;//�ܼ�ʱʱ��
            if (STOPWATCH_WIN_ID == MMK_GetFocusWinId()||STOPWATCH_WIN_ID == MMK_GetFocusChildWinId())
            {
                if ( STOPWATCH_STATUS_RUNNING == s_stopwatch_status )//ֻ�м�ʱ����ʱ��ˢ��
                {
                    MMK_SendMsg(STOPWATCH_WIN_ID, MSG_ACC_UPDATE_STOPWATCH, PNULL);
                }
                else
                {
                    SCI_TRACE_LOW("s_stopwatch_status = %d",s_stopwatch_status);
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
    GUI_RECT_T  btnRect = STOPWATCH_BTN_SINGLE_RECT;

    if(STOPWATCH_STATUS_STOP == s_stopwatch_status)
    {
        SCI_TRACE_LOW("s_stopwatch_status = %d",s_stopwatch_status);
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

    GUIBUTTON_SetRect(STOPWATCH_CTRL_START, &btnRect);

    StopWatch_SetBtnVisibleStatus(s_stopwatch_status);

    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_COMMON_START,STXT_RETURN,FALSE);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);

}

LOCAL void StopWatch_StartOrPauseTime( MMI_WIN_ID_T win_id )
{
    SCI_TICK_TIME_T tickTime    = {0};//��ǰʱ�̣�ticktime
    uint64          currentTime = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������
    GUI_RECT_T      leftBtnRect = STOPWATCH_BTN_TWO_LEFT_RECT;
    GUI_RECT_T      rightBtnRect= STOPWATCH_BTN_TWO_RIGHT_RECT;
    if(STOPWATCH_STATUS_RUNNING == s_stopwatch_status)
    {
        if( s_stopwatch_timeId != 0)
        {
            MMK_StopTimer(s_stopwatch_timeId);
            s_stopwatch_timeId   = 0;
        }
        s_stopwatch_stop_time   = s_stopwatch_current_time;
        s_stopwatch_status      = STOPWATCH_STATUS_PAUSE;
        GUIBUTTON_SetRect(STOPWATCH_CTRL_RESET, &leftBtnRect);
        GUIBUTTON_SetRect(STOPWATCH_CTRL_START, &rightBtnRect);

        GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_COMMON_PLAY,STXT_RETURN,FALSE);
    }
    else //û��running������£����µ�Ϊstart(������stop���Ǵ�δ��ʼ�����ƴ���)
    {
        SCI_GetTickTime(&tickTime);
        currentTime             = ((uint64)(tickTime.second) * 1000 + (uint64)(tickTime.milliseconds));
        s_stopwatch_start_time  = currentTime;
        s_stopwatch_status      = STOPWATCH_STATUS_RUNNING;
        s_stopwatch_timeId      = MMK_CreateTimerCallback(STOPWATCH_TIMER_INTERVAL,StopWatch_TimeUpdate_CallBack,NULL,TRUE);
        GUIBUTTON_SetRect(STOPWATCH_CTRL_RESET, &leftBtnRect);
        GUIBUTTON_SetRect(STOPWATCH_CTRL_STOP, &rightBtnRect);

        GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_PAUSE,STXT_RETURN,FALSE);
    }
    StopWatch_SetBtnVisibleStatus(s_stopwatch_status);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);

}

LOCAL void WATCHCOM_DisplayStopwatchBackground( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T         disp_rect    = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO   lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

     LCD_FillRect(&lcd_dev_info, disp_rect, MMI_BLACK_COLOR);
    //GUIRES_DisplayImg(PNULL, &disp_rect, PNULL, win_id, res_aw_stopwatch_bg,&lcd_dev_info);

}

LOCAL MMI_RESULT_E HandleMsgStopWatchWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;
    GUI_RECT_T      timeRect    = STOPWATCH_TIME_DISP_RECT;  //time������ָ��
    GUISTR_STYLE_T  timeStyle   = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            //����button����ʾ�ַ�
            if (STOPWATCH_STATUS_RUNNING == s_stopwatch_status)
            {
                GUI_RECT_T leftBtnRect = STOPWATCH_BTN_TWO_LEFT_RECT;
                GUI_RECT_T rightBtnRect = STOPWATCH_BTN_TWO_RIGHT_RECT;
                GUIBUTTON_SetRect(STOPWATCH_CTRL_RESET, &leftBtnRect);
                GUIBUTTON_SetRect(STOPWATCH_CTRL_STOP, &rightBtnRect);

                GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_PAUSE,STXT_RETURN,FALSE);
            }
            else if(STOPWATCH_STATUS_PAUSE == s_stopwatch_status)
            {
                GUI_RECT_T leftBtnRect = STOPWATCH_BTN_TWO_LEFT_RECT;
                GUI_RECT_T rightBtnRect = STOPWATCH_BTN_TWO_RIGHT_RECT;
                GUIBUTTON_SetRect(STOPWATCH_CTRL_RESET, &leftBtnRect);
                GUIBUTTON_SetRect(STOPWATCH_CTRL_START, &rightBtnRect);

                GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET,TXT_COMMON_PLAY,STXT_RETURN,FALSE);
            }
            else if(STOPWATCH_STATUS_STOP == s_stopwatch_status)
            {
                GUI_RECT_T  btnRect = STOPWATCH_BTN_SINGLE_RECT;
                GUIBUTTON_SetRect(STOPWATCH_CTRL_START, &btnRect);

                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_COMMON_START,STXT_RETURN,FALSE);
            }
            else
            {
                SCI_TRACE_LOW("s_stopwatch_status = %d",s_stopwatch_status);
            }

            //����BUTTON����״̬
            StopWatch_SetBtnVisibleStatus(s_stopwatch_status);
        }
        break;

        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayStopwatchBackground(win_id);

            StopWatch_DisplayCurTime(win_id, timeRect, &timeStyle, s_stopwatch_current_time);//û�п�ʼ��ʱʱ����ʾ00:00:00.0
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
            //��ʾʱ���ı�����
            StopWatch_TimeBackgroundDisplay();
            //��ʾ����ʱʱ����Ϣ
            StopWatch_DisplayCurTime(win_id, timeRect, &timeStyle, s_stopwatch_current_time);;
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
            if(STOPWATCH_CTRL_RESET == ((MMI_NOTIFY_T*)param)->src_id)//reset button��ѡ��
            {
                StopWatch_ResetTime(win_id);
            }
            else if(STOPWATCH_CTRL_START == ((MMI_NOTIFY_T*)param)->src_id)//play button��ѡ��
            {
                StopWatch_StartOrPauseTime(win_id);
            }
            else if(STOPWATCH_CTRL_STOP == ((MMI_NOTIFY_T*)param)->src_id)//pause button��ѡ��
            {
                StopWatch_StartOrPauseTime(win_id);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT

        case MSG_KEYDOWN_RED:
            break;

        case MSG_KEYUP_RED:
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
WINDOW_TABLE(STOPWATCH_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(STOPWATCH_WIN_ID),
    WIN_FUNC((uint32)HandleMsgStopWatchWindow),
    CREATE_BUTTON_CTRL(STOPWATCH_RESET, STOPWATCH_CTRL_RESET),
    CREATE_BUTTON_CTRL(STOPWATCH_START,  STOPWATCH_CTRL_START),
    CREATE_BUTTON_CTRL(STOPWATCH_STOP, STOPWATCH_CTRL_STOP),
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
PUBLIC void ZTE_STOPWATCH_OpenMainWin( void )
{
    MMK_CreateWin((uint32 *)STOPWATCH_WIN_TAB,PNULL);
}
