/*****************************************************************************
** File Name:      mmiwidget_countedtime.c                                        *
** Author:                                                                   *
** Date:           08/01/2011                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/01/2011        nan.ji              Creat
******************************************************************************/

#define _MMIWIDGET_COUNTEDTIME_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#if defined MMI_GRID_COUNTEDTIME
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiwidget_text.h"
#include "mmiwidget_position.h"
#include "guiform.h"
//#include "time.h"
#include "mmicountedtime_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmipub.h"
#include "guictrl_api.h"
#include "mmiidle_export.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif
#include "mmi_timer_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/


typedef enum
{
	ARRIVE_END,          /*!<运行结束*/
	TIMENULL_END,        /*!<输入空结束*/
	END_MAX            /*!<保留位*/
} COUNTEDTIME_END_E;

typedef void    (*SET_STATE)(void *ptr);
/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
SCI_TIME_T s_ct_current_time = {0};//启动后记录的倒计时当前时间
APP_POSITION_E s_app_get_focus_to_run = APP_NOT_RUN;
uint8 s_countedtime_time_id = 0;//倒记时的timer
COUNTEDTIME_STATE_E s_is_countedtime_running = STATE_STOP;
LOCAL SCI_TIME_T s_ct_set_time = {10,0,0};//进入设置界面时看到的时间，记录为最近设置的时间值 
LOCAL uint8 s_last_sys_time_second = {0};//启动后记录的倒计时当前时间
LOCAL uint8 s_pubwin_time_id = 0; 
LOCAL BOOLEAN s_is_focus = FALSE;
/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Handle Countedtime Win Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetCountedtimeWinMsg(
												MMI_WIN_ID_T        win_id,        //IN:
												MMI_MESSAGE_ID_E    msg_id,        //IN:
												DPARAM            param        //IN:
												);
/*****************************************************************************/
// Description : Display Countedtime Widget
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL void DisplayCountedtime(
							MMI_WIN_ID_T win_id,
							DPARAM    param
							);

/*****************************************************************************/
//  Description : to handle set time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetTime(
                      MMI_CTRL_ID_T     ctrl_id
                      );

/*****************************************************************************/
//  Description : start timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StartSecTimer(void);
/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopSecTimer(void);

/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ShowCountedTimeArrive(COUNTEDTIME_END_E style);
/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispCoutime(void);
/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_GetTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   );
/*****************************************************************************/
//  Description : save state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/		
LOCAL void Savestate(void);
/*****************************************************************************/
//  Description : resume state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/		
LOCAL void Resumestate(void);
/*****************************************************************************/
//  Description : resume state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/	
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
/*****************************************************************************/
//  Description : resume state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/	
LOCAL void  HandleTpPressLongUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
/*****************************************************************************/
//  Description : resume state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/	
LOCAL void StateStop(void *ptr);
/*****************************************************************************/
//  Description : resume state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StateRunning(void *ptr);	

LOCAL MMI_RESULT_E HandleTPLongDown(      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                );
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_countedtime_widget =
{
	MMIWIDGET_COUNTEDTIME_ID, 
	HandleWidgetCountedtimeWinMsg, 
	PNULL, 
	PNULL,
	PNULL,
	WIDGET_COUNTEDTIME_WIN_ID, 
	IMAGE_WIDGET_COUNTEDTIME__SHORTCUT, 
	IMAGE_WIDGET_COUNTEDTIME_BG,
	TXT_TOOLS_COUNTEDTIME,
	80,//    75, 
	80,//    75
	FALSE
};
//状态回调函数
const   SET_STATE  s_state[STATE_CANCEL]    = {         
        StateStop,
		StateRunning,
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : Handle Widget Countedtime Win Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetCountedtimeWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		{
            GUIEDIT_INIT_DATA_T            init_data   = {0};
			MMI_CONTROL_CREATE_T           ctrl_create = {0};
			SCI_TIME_T                     sys_time    = s_ct_set_time;
			GUIEDIT_TIME_STYLE_E           time_style  = GUIEDIT_TIME_STYLE_TIMER;
			GUIEDIT_TIME_DISPLAY_STYLE_E   display_style_ptr= GUIEDIT_TIME_DISPLAY_STYLE_HMS; //may PNULL
            init_data.type                = GUIEDIT_TYPE_TOUCH_TIME;
			ctrl_create.ctrl_id           = ctrl_id;
			ctrl_create.guid              = SPRD_GUI_EDITBOX_ID;
			ctrl_create.init_data_ptr     = &init_data;
			ctrl_create.parent_win_handle = win_id;
			MMK_CreateControl(&ctrl_create);

			GUIEDIT_SetTimeStyle(ctrl_create.ctrl_id,PNULL,&time_style,&display_style_ptr,FALSE);
			GUIEDIT_SetTime(ctrl_create.ctrl_id,sys_time.hour,sys_time.min,sys_time.sec);	
			GUIEDIT_EnbaleAnim(ctrl_create.ctrl_id, TRUE);
			
		    GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, FALSE );
        }
        break;  

    case MSG_GRID_ADD_WIDGET:  
        {                           
            GUI_RECT_T   edit_rect   = MMIWIDGET_COUNTEDTIME_EDIT_RECT;
            GUI_RECT_T   win_rect    = {0};
            MMK_GetWinRect(win_id, &win_rect);
            edit_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, edit_rect);
            GUIEDIT_SetRect(ctrl_id,&edit_rect);
            
            MMK_SetAtvCtrl(win_id,MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID);
			//MMK_WinGrabFocus(win_id);
            //MMK_SetWinHookFunc(win_id, HandleTPLongDown);  
        }
        break;
        
    case MSG_WIDGET_MODIFY_RECT:
        break;

    case MSG_LOSE_FOCUS:
		Savestate();
	//	MMK_WinGrabFocus(MMK_GetParentWinHandle(win_id));//CR167818
		break;

                
	case MSG_GRID_REMOVE_WIDGET:
		StopSecTimer();
        //MMK_SetWinHookFunc(win_id, 0);
		s_is_countedtime_running = STATE_CANCEL;
        break;

    case MSG_APP_WEB:
#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
        MMIAPI_OpenTimerWin(ID_COUNTDOWN);
#endif
        break;

    case MSG_WIDGET_TP_PRESS_DOWN:
        {
			MMK_SetChildWinFocus(MMK_GetParentWinHandle( win_id), win_id);
            recode = MMI_RESULT_FALSE;
        }
        break;
                
    case MSG_WIDGET_TP_PRESS_LONG_DOWN:        
        {
            //enter sliding state ,disalbe TP
		    GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, TRUE );
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_WIDGET_TP_PRESS_UP:
		recode = HandleTpPressUp(win_id,param);
		break;
        
    case MSG_FULL_PAINT:
		DisplayCountedtime(win_id,NULL);
        break;

    case MSG_GET_FOCUS:
		Resumestate();
		break;

    case MSG_WIDGET_TP_PRESS_LONG_UP:
      	HandleTpPressLongUp(win_id,param);  
		break;

    case MSG_WIDGET_PAGE_SLIDE_START:
		GUIEDIT_EnbaleAnim(ctrl_id,FALSE);
	    GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
		//MMK_WinGrabFocus(win_id);//cr167818
		break;

    case MSG_TIMER:
		if((*(uint8*)param) == s_pubwin_time_id )
		{		
		//	ShowCountedTimeArrive(ARRIVE_END);
            MMIAPICT_StartCountedTimeArrive();
			MMK_StopTimer(s_pubwin_time_id);
			s_pubwin_time_id = 0;
			GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, FALSE );
            GUIEDIT_SetTime(ctrl_id,0,0,0);
		}
		break;


    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}


/*****************************************************************************/
// Description : Display Countedtime Widget
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL void DisplayCountedtime(
                       MMI_WIN_ID_T win_id,
					   DPARAM    param
                       )
{
    GUI_RECT_T              win_rect     = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    GUISTR_STYLE_T          text_style   = {0};
    GUISTR_STATE_T          text_state   = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;	
    GUI_RECT_T		        playbutton_rect = MMIWIDGET_COUNTEDTIME_BUTTON_RECT;
	MMI_STRING_T            tempStr      = {0};
	MMI_CTRL_ID_T           ctrl_id      = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;


    GUIEDIT_EnbaleAnim(ctrl_id, FALSE);
    GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);


    MMIWIDGET_GetTextStyle(&text_style, 24, MMI_BLACK_COLOR);
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    MMK_SetCtrlLcdDevInfo(ctrl_id, &lcd_dev_info);
	
	if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
		UILAYER_ClearRect(&lcd_dev_info,win_rect);
		GUIRES_DisplayImg(
			PNULL,
			&win_rect,
			PNULL,
			win_id,
			IMAGE_WIDGET_COUNTEDTIME_BG,
			&lcd_dev_info
			);
		playbutton_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, playbutton_rect);
		if (STATE_STOP == s_is_countedtime_running || STATE_CANCEL == s_is_countedtime_running)
		{
			GUIRES_DisplayImg(
				PNULL,
				&playbutton_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_COUNTEDTIME_BUTTON_OFF,
				&lcd_dev_info
				);
			MMI_GetLabelTextByLang(TXT_COMMON_START, &tempStr);
			GUISTR_DrawTextToLCDInRect(
				(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				&playbutton_rect,
				&playbutton_rect,
				&tempStr,	   
				&text_style,
				text_state,
				GUISTR_TEXT_DIR_AUTO
				);
		}
		else if(STATE_RUNNING == s_is_countedtime_running)
		{
			GUIRES_DisplayImg(
				PNULL,
				&playbutton_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_COUNTEDTIME_BUTTON_ON,
				&lcd_dev_info
				);
			MMI_GetLabelTextByLang(STXT_STOP, &tempStr);
			GUISTR_DrawTextToLCDInRect(
				(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				&playbutton_rect,
				&playbutton_rect,
				&tempStr,	   
				&text_style,
				text_state,
				GUISTR_TEXT_DIR_AUTO
				);			
		}
	}
}

LOCAL MMI_RESULT_E HandleTPLongDown(      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                )
{
    MMI_RESULT_E	recode          = MMI_RESULT_FALSE;
    MMI_WIN_ID_T  parent_handle = 0;
    
    if( msg_id == MSG_TP_PRESS_LONG )
    {
        parent_handle = MMK_GetParentWinHandle(win_id);
        MMK_SendMsg(parent_handle,  MSG_TP_PRESS_DOWN, param);
        MMK_SendMsg(parent_handle,  MSG_TP_PRESS_LONG, param);
    	recode  = MMI_RESULT_FALSE;
    }
    else if( msg_id == MSG_TP_PRESS_UP )
    {
        parent_handle = MMK_GetParentWinHandle(win_id);
        MMK_SendMsg(parent_handle,  msg_id, param);
    	recode  = MMI_RESULT_FALSE;
    }
    return recode;

}
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E	recode          = MMI_RESULT_TRUE;
    GUI_POINT_T		point           = {0};
    GUI_RECT_T		playbutton_rect = MMIWIDGET_COUNTEDTIME_BUTTON_RECT;
	MMI_CTRL_ID_T       ctrl_id = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if (GUI_PointIsInRect(point, playbutton_rect))
    {
		if(STATE_RUNNING == s_is_countedtime_running)
		{
			StopSecTimer();
            s_is_countedtime_running = STATE_STOP;
			GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, FALSE );
		}
		else if (STATE_STOP == s_is_countedtime_running || STATE_CANCEL == s_is_countedtime_running)
		{
			SetTime(ctrl_id);//得到edit的时间
			if (0 == s_ct_current_time.hour && 0 == s_ct_current_time.min && 0 == s_ct_current_time.sec)
			{
				ShowCountedTimeArrive(TIMENULL_END);
				return recode;
			}
			StartSecTimer();
			GUIEDIT_EnbaleAnim(ctrl_id, TRUE);
			GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, TRUE );//禁止tp
			s_is_countedtime_running = STATE_RUNNING;
#ifdef PDA_UI_DROPDOWN_WIN
			MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_COUNTEDTIME);
#endif 
		}
    }
	else
	{
        recode = MMI_RESULT_FALSE;
	}
    return recode;
}
/*****************************************************************************/
//  Description : 设置输入的时间为倒记时开始的时间
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTime(
                      MMI_CTRL_ID_T     ctrl_id
                      )
{
    uint8           hour   = 0, minute = 0, sec = 0;
    BOOLEAN         result = TRUE;
    MMISET_TIME_T   time   = {0};

    //get time from ctrl
    GUIEDIT_GetTime(ctrl_id,&hour,&minute,&sec);

    time.time_hour = hour;
    time.time_minute = minute;

    //check time if is valid
    if (MMIAPICOM_CheckTimeIsValid(time))//@zhaohui,cr108071
    {
        s_ct_current_time.sec = sec;
        s_ct_current_time.min = minute;
        s_ct_current_time.hour = hour;
    }
    else
    {
        result = FALSE;
    }
    return (result);
}
/*****************************************************************************/
//  Description : start timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StartSecTimer(void)
{ 
    if (0 == s_countedtime_time_id )
    {
		s_app_get_focus_to_run = APP_IN_IDLE;
        s_countedtime_time_id = MMK_CreateTimerCallback(1000,  MMIAPICT_HandleTimer, NULL, TRUE);  
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIWIDGET]: StartTimer: the timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_COUNTEDTIME_503_112_2_18_3_8_1_188,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopSecTimer(void)
{
	if (0 != s_countedtime_time_id)
	{
        s_app_get_focus_to_run = APP_NOT_RUN;
		MMK_StopTimer(s_countedtime_time_id);
		s_countedtime_time_id = 0;
	}
    else
    {
        //SCI_TRACE_LOW:"[MMIWIDGET]: StopTimer: the timer has stopped!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_COUNTEDTIME_520_112_2_18_3_8_1_189,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_HandleTimer(uint8 timer_id)
{
    SCI_TIME_T        sys_time        = {0};

	TM_GetSysTime(&sys_time);

	if (sys_time.sec == s_last_sys_time_second)
	{
		return;
	}
	if (s_countedtime_time_id == timer_id)
	{
		//时间减1处理
		if (s_ct_current_time.sec <60)
		{
			s_ct_current_time.sec -- ;
			if (((s_ct_current_time.sec == 0)||(s_ct_current_time.sec == 0xff) ) && (s_ct_current_time.min == 0) && (s_ct_current_time.hour == 0))//时间到
			{   
#if defined MMI_ISTYLE_SUPPORT 			
                if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
                {
                    ShowCountedTimeArrive(ARRIVE_END);
                }
                else
#endif					
                {
                    s_ct_current_time.sec = 0;
                    SendMsgtoDispCoutime();
                    GUIAPICTRL_SetState(MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID, GUICTRL_STATE_DISABLE_TP, FALSE );
                    s_pubwin_time_id = MMK_CreateWinTimer(WIDGET_COUNTEDTIME_WIN_ID, 500, FALSE);
                    StopSecTimer();
                }
                return;
			}
			else if ((s_ct_current_time.sec == 0xff) && (s_ct_current_time.min <= 59))//@zhaohui,cr110181
			{
				s_ct_current_time.sec = 59;
				s_ct_current_time.min --;
				if ((s_ct_current_time.min == 0xff) && (s_ct_current_time.hour>0) && (s_ct_current_time.hour <= 23))//@zhaohui,cr110181
				{
					s_ct_current_time.min = 59;
					s_ct_current_time.hour --;
				}
			}
#if defined MMI_GRID_IDLE_SUPPORT
                if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
                {
                    if(MMIGRID_IsForbidPaint(MMIWIDGET_COUNTEDTIME_ID))
                    {
                        return;
                    }
                }
#endif
                
#if defined QBTHEME_SUPPORT
                if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
                {
                    if(MMIQBIDLE_IsForbidPaint(MMIWIDGET_COUNTEDTIME_ID))
                    {
                        return;
                    }
                }
#endif
        	
			SendMsgtoDispCoutime();
		}

	}
}

/*****************************************************************************/
//  Description : to show counted time arrive prompt
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowCountedTimeArrive(COUNTEDTIME_END_E style)
{
    MMI_WIN_ID_T           alert_win_id   = WIDGET_COUNTEDTIME_TIMEEND_WIN_ID;//倒计时到窗口
    MMI_STRING_T           prompt_str1    = {0};

    SCI_TIME_T             time           = {0};//从系统获得的时间
    uint8                  timestr[8*2+1] = {0};//用于显示时间的buffer
    MMI_STRING_T           prompt_str2    = {0};
    MMI_WIN_PRIORITY_E     win_priority   = WIN_LOWEST_LEVEL;
    uint32                 time_out       = 0;
	if (ARRIVE_END == style)
	{
		MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME_ARRIVE/*倒计时时间到*/,&prompt_str1);
		/*需要换图片文字*/
	}        
	else if (TIMENULL_END == style)
	{
		MMI_GetLabelTextByLang(TXT_WIDGET_COUNTEDTIME_INPUT/*请输入时间*/,&prompt_str1);
	}

    //prompt2
    //init the string, which will be displayed
    prompt_str2.wstr_len  = 0;
    prompt_str2.wstr_len = PNULL;

    TM_GetSysTime(&time);

    MMIAPISET_FormatTimeStrByTime(time.hour,time.min,timestr,(8*2+1));
    prompt_str2.wstr_len = strlen((char *)timestr);
    
    //allocate memory
    prompt_str2.wstr_ptr = SCI_ALLOC_APP((prompt_str2.wstr_len+1)*sizeof(wchar));
	if(PNULL == prompt_str2.wstr_ptr)
	{
		return;
	}

    //prompt_str2.is_ucs2 = FALSE;/*不知此处该如何处理*/
    //copy the num
    MMI_STRNTOWSTR(
                prompt_str2.wstr_ptr,
                prompt_str2.wstr_len,
                timestr,  
                prompt_str2.wstr_len,
                prompt_str2.wstr_len
                );

    win_priority = WIN_ONE_LEVEL;

    MMIDEFAULT_TurnOnBackLight();
    //@zhaohui,cr108565
	if (ARRIVE_END == style)
	{
		MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str1,&prompt_str2,IMAGE_PUBWIN_SUCCESS,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,MMIAPICT_HandleCountedTimeArriveWin);
	}
	else if (TIMENULL_END == style)
	{
		MMIPUB_OpenAlertWinByTextPtr(PNULL,&prompt_str1,&prompt_str2,IMAGE_PUBWIN_WARNING,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,NULL);
	}
    //free memory
    if (PNULL != prompt_str2.wstr_ptr)
    {
        SCI_FREE(prompt_str2.wstr_ptr);
        prompt_str2.wstr_ptr = PNULL;
    }
}


/*****************************************************************************/
//  Description : SendMsgtoDispCoutime
//  Global resource dependence : 
//  Author:       zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispCoutime(void)
{
	GUI_RECT_T              win_rect     = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
	GUI_RECT_T              edit_rect    = MMIWIDGET_COUNTEDTIME_EDIT_RECT;
	MMI_CTRL_ID_T           ctrl_id      = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;
	SCI_TIME_T              sys_time     = {0};
	MMI_WIN_ID_T            win_id       = WIDGET_COUNTEDTIME_WIN_ID;
	if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
	{
		if (MMIAPIIDLE_IdleWinIsOpen())
		{
			if (s_is_focus)
			{
				MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
				MMK_SetCtrlLcdDevInfo(ctrl_id, &lcd_dev_info);
				MMK_GetWinRect(win_id, &win_rect);
				edit_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, edit_rect);
				GUIEDIT_SetRect(ctrl_id, &edit_rect);
				GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
				GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, TRUE );
				GUIEDIT_EnbaleAnim(ctrl_id, TRUE);
				s_is_countedtime_running = STATE_RUNNING;
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}
		}
	}
	TM_GetSysTime(&sys_time);			
	s_last_sys_time_second = sys_time.sec;
    return TRUE;
}
/*****************************************************************************/
//  Description : 获得字体
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_GetTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   )
{
    if (PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_GetTextStyle style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_COUNTEDTIME_731_112_2_18_3_8_2_190,(uint8*)"");
        return;
    }
  	
    style_ptr->effect = 0;//FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_HVMIDDLE;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 2;    
}

/*****************************************************************************/
//  Description : save state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/		
LOCAL void Savestate(void)
{	
	MMI_CTRL_ID_T   ctrl_id = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;

	s_is_focus = FALSE;	
	GUIEDIT_EnbaleAnim(ctrl_id, FALSE);
	GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
	GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, TRUE );
}

/*****************************************************************************/
//  Description : save state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/		
LOCAL void Resumestate(void)
{
	MMI_CTRL_ID_T           ctrl_id      = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;
	s_is_focus = TRUE;

	if (APP_IN_MENU == s_app_get_focus_to_run &&STATE_RUNNING == s_is_countedtime_running)
	{
		StopSecTimer();
		s_app_get_focus_to_run = APP_IN_IDLE;
		StartSecTimer();
	}

	if (STATE_STOP == s_is_countedtime_running)
	{
		SetTime(ctrl_id);//得到edit的时间
		GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
		GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, FALSE );
	}
	else if (STATE_RUNNING == s_is_countedtime_running)
	{
		if(0 == s_ct_current_time.hour && 0 == s_ct_current_time.min && 0 == s_ct_current_time.sec)
		{
		    GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
			s_is_countedtime_running = STATE_STOP;
			GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, FALSE );
			return;
		}
		GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, TRUE );
	    GUIEDIT_EnbaleAnim(ctrl_id, FALSE);
		GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
        GUIEDIT_EnbaleAnim(ctrl_id, TRUE);
	}
	else if(STATE_CANCEL == s_is_countedtime_running )
	{
		s_ct_current_time = s_ct_set_time;
		GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
		GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, FALSE );
		s_is_countedtime_running = STATE_STOP;
	}
}
/*****************************************************************************/
//  Description :handle longpress up
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/	 
LOCAL void  HandleTpPressLongUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
   (*s_state[s_is_countedtime_running])(PNULL); 
}
/*****************************************************************************/
//  Description :stop state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/	  
LOCAL void StateStop(void *ptr)
{
	MMI_CTRL_ID_T    ctrl_id              = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;

	GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, FALSE );	
	GUIEDIT_EnbaleAnim(ctrl_id, TRUE);
}
/*****************************************************************************/
//  Description : running state.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/	
LOCAL void StateRunning(void *ptr)
{
	MMI_CTRL_ID_T    ctrl_id              = MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID;

	s_is_countedtime_running = STATE_RUNNING;
	GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP, TRUE );	
	GUIEDIT_EnbaleAnim(ctrl_id, FALSE);
	GUIEDIT_SetTime(ctrl_id, s_ct_current_time.hour, s_ct_current_time.min, s_ct_current_time.sec);
	GUIEDIT_EnbaleAnim(ctrl_id, TRUE);

}
#endif

