/*****************************************************************************
** File Name:      mmiwidget_alarm.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#define _MMIWIDGET_ALARM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#ifdef MMIWIDGET_SUPPORT
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
//#include "mmicc_text.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_position.h"
#include "mmialarm_export.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmiacc_event.h"
#include "mmiacc_id.h"
#include "mmi_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
//#include "guictrl_api.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define WIDGET_ALARM_BUTTON_INVALIDATE_ID   255


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

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
#define   MMIWIDGET_ALARM_TEXT_FONT              SONG_FONT_16	
#define   MMIWIDGET_ALARM_TIME_FONT              SONG_FONT_24	
#define   MMIWIDGET_ALARM_TEXT_COLOR             MMI_WHITE_COLOR								 
#define   MMIWIDGET_ALARM_TEXT_MAX_LEN           60

#define MMIWIDGET_ALARM_NAME_X       23
#define MMIWIDGET_ALARM_NAME_Y       17

#define MMIWIDGET_ALARM_SWITCH_X     120
#define MMIWIDGET_ALARM_SWITCH_Y     14

#define MMIWIDGET_ALARM_TIME_X       23
#define MMIWIDGET_ALARM_TIME_Y       42

#define MMIWIDGET_ALARM_FREQ_X       120
#define MMIWIDGET_ALARM_FREQ_Y       45

#define MMIWIDGET_ALARM_LEFT_ARR_X   7
#define MMIWIDGET_ALARM_LEFT_ARR_Y   32

#define MMIWIDGET_ALARM_RIGHT_ARR_X  183
#define MMIWIDGET_ALARM_RIGHT_ARR_Y  32

#define MMIWIDGET_ALARM_IND_X        0
#define MMIWIDGET_ALARM_IND_Y        71

#define MMIWIDGET_ALARM_FOCUS_STARTX 38
#define MMIWIDGET_ALARM_FOCUS_STARTY 80

LOCAL uint32 s_widget_alarm_focus_idx = 0;
LOCAL uint32 s_widget_alarm_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;

LOCAL GUI_POINT_T s_widget_alarm_down_point = {0};

LOCAL uint32 s_widget_alarm_down_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;

LOCAL void DisplayAlarmPanel(MMI_WIN_ID_T win_id, GUI_POINT_T move_point);

LOCAL void MMIWIDGET_Alarm_GetTextStyle(
                                        GUISTR_STYLE_T *style_ptr,
										GUI_FONT_T     font,
										GUI_COLOR_T    font_color
										);

LOCAL void MMIWIDGET_Alarm_DisplaySlideImg(MMI_WIN_ID_T win_id, GUI_POINT_T  point, const GUI_LCD_DEV_INFO *dev_info_ptr  );

LOCAL MMI_RESULT_E  HandleAlarmTpPressDown(
										   MMI_WIN_ID_T        win_id,        //IN:
										   DPARAM            param        //IN:
										   );
										   
LOCAL MMI_RESULT_E HandleAlarmTpPressMove(MMI_WIN_ID_T win_id, DPARAM param);

LOCAL MMI_RESULT_E HandleAlarmTpPressUp(MMI_WIN_ID_T win_id, DPARAM param);

LOCAL void MMIWIDGET_Alarm_ModifyEventbyEventid(uint16 event_id, BOOLEAN is_on);
LOCAL BOOLEAN MMIWIDGET_Alarm_GetEventStatusbyEventid(uint16 event_id);
LOCAL MMI_RESULT_E DirectlyHandleTpPressUp(uint16 event_id,GUI_POINT_T up_point,uint16 valid_step);
/*****************************************************************************/
//     Description : Handle WidgetClock Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetAlarmWinMsg(
											MMI_WIN_ID_T        win_id,        //IN:
											MMI_MESSAGE_ID_E    msg_id,        //IN:
											DPARAM            param        //IN:
											);

/*****************************************************************************/
//  Description :  reset alarm button status.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ResetWidgetAlarm();
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_alarm_widget =
{
	MMIWIDGET_ALARM_ID, 
	HandleWidgetAlarmWinMsg, 
	PNULL, 
	ResetWidgetAlarm,
	PNULL,
	WIDGET_ALARM_WIN_ID, 
	IMAGE_WIDGET_ALARM_SHORTCUT, 
	IMAGE_WIDGET_ALARM_BG,
    TXT_ALARM_CLOCK,
	50,//    75, 
	50,//    75
	FALSE
};

/*****************************************************************************/
//  Description :  reset alarm button status.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ResetWidgetAlarm()
{
	s_widget_alarm_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetAlarmWinMsg(
											MMI_WIN_ID_T        win_id,        //IN:
											MMI_MESSAGE_ID_E    msg_id,        //IN:
											DPARAM            param        //IN:
											)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};

	switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_FULL_PAINT:
        if(WIDGET_ALARM_BUTTON_INVALIDATE_ID != s_widget_alarm_button && PNULL == param)
        {
            break;
        }
        else
        if (PNULL  != param)
        {
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);	
        }

        DisplayAlarmPanel(win_id, point);
        break;
        
    case MSG_WIDGET_TP_PRESS_DOWN:
        recode = HandleAlarmTpPressDown(win_id,param);
        break;
        
    case MSG_WIDGET_TP_PRESS_UP:
        recode = HandleAlarmTpPressUp(win_id,param);
        break;
        
    case  MSG_WIDGET_TP_PRESS_MOVE:
        recode = HandleAlarmTpPressMove(win_id,param);
        break;

    case MSG_LOSE_FOCUS:
#if (defined MMI_GRID_IDLE_SUPPORT)
        if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
        {
            MMIGRID_DestroyApplicationLayer();
        }
#endif
#if (defined MMIWIDGET_SUPPORT)
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
        {
            MMIWIDGET_DestroyApplicationLayer();
        }
#endif 
        break;

    case MSG_LCD_SWITCH:
        ResetWidgetAlarm();
#if (defined MMI_GRID_IDLE_SUPPORT)
        if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
        {
            MMIGRID_DestroyApplicationLayer();
        }
#endif
#if (defined MMIWIDGET_SUPPORT)
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
        {
           MMIWIDGET_DestroyApplicationLayer();
        }
#endif          
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;        
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayAlarmPanel(MMI_WIN_ID_T win_id, GUI_POINT_T move_point)
{
	GUI_RECT_T win_rect = {0};
	GUI_RECT_T disp_rect = {0};
	GUI_RECT_T temp_win_rect = {0};	
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	GUI_POINT_T      point={0};
	GUI_POINT_T      temp_point={0};	
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T    Temp_str= {0};
	MMI_STRING_T    label_str[ALM_ALARM_NUM]= {0};	
	uint32 i = 0;
	uint32        fre_mode_text_id[] = {TXT_ALARM_MODE_ONCE, TXT_ALARM_MODE_EVERYDAY, TXT_ALARM_MODE_WEEK};	
	MMI_TEXT_ID_T txt_alarm_id[]     = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3, TXT_ALARM_CLOCK4, TXT_ALARM_CLOCK5, TXT_ALARM_CLOCK6};
	wchar temp_wstr[MMIWIDGET_ALARM_TEXT_MAX_LEN + 1] = {0};	
	uint16	 image_width = 0;
	uint16	 image_height = 0;	
	uint16 event_id	  = 0; 
	uint16 delay_event_id = 0;
	GUISTR_INFO_T   str_info ={0};	 
	const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 	= PNULL;
	const MMIACC_SMART_EVENT_FAST_TABLE_T *event_delay_ptr = PNULL;
	GUI_RECT_T button_rect = MMIWIDGET_ALARM_BUTTON_RECT;
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;	
    MMIACC_SMART_EVENT_T    smart_event = {0};
    MMI_IMAGE_ID_T   img_id = 0;
	BOOLEAN is_app_layer = FALSE;

	MMIWIDGET_Alarm_GetTextStyle(&text_style, MMIWIDGET_ALARM_TEXT_FONT, MMIWIDGET_ALARM_TEXT_COLOR);	
	MMK_GetWinRect(win_id, &win_rect);
	disp_rect = win_rect;

#if (defined MMI_GRID_IDLE_SUPPORT)
    if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
    {
        is_app_layer = MMIGRID_GetApplicationLayer(&lcd_dev_info);
    }
#endif
#if defined MMIWIDGET_SUPPORT
    if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
    {
       is_app_layer = MMIWIDGET_GetApplicationLayer(&lcd_dev_info);
    }
#endif

	if(!is_app_layer)
	{
		MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);		
	}
	
	Temp_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MMIWIDGET_ALARM_TEXT_MAX_LEN+1)*sizeof(wchar));
	SCI_MEMSET(Temp_str.wstr_ptr, 0x00, ((MMIWIDGET_ALARM_TEXT_MAX_LEN+1)*sizeof(wchar)));	

	event_id        = (uint16)(EVENT_ALARM_0 + s_widget_alarm_focus_idx);
	delay_event_id  = (uint16)(EVENT_ALARM_DELAY_0 + s_widget_alarm_focus_idx);	
	event_ptr       = MMIALM_GeEvent(event_id);
	event_delay_ptr = MMIALM_GeEvent(delay_event_id);

	if (UILAYER_IsLayerActive(&lcd_dev_info))
	{
		//display alarm background image.
		point.x = disp_rect.left;
		point.y = disp_rect.top;
		
		GUIRES_DisplayImg(
                		  &point,
                		  PNULL,
                		  PNULL,
                		  win_id,
                		  IMAGE_WIDGET_ALARM_BG,
                		  &lcd_dev_info
                		  );
				 
		//display alaram name.		
        MMIAPIALM_FormInteEventInfo(event_id,&smart_event);
		MMI_GetLabelTextByLang(txt_alarm_id[event_id], &label_str[event_id]);

        if (0 != smart_event.event_content_info.w_length)
        {
  		    label_str[event_id].wstr_ptr = smart_event.event_content_info.w_content;
  		    label_str[event_id].wstr_len = smart_event.event_content_info.w_length;
        }
		
		GUISTR_GetStringInfo(&text_style, &label_str[event_id], text_state,&str_info);		 
        disp_rect.left  = win_rect.left + MMIWIDGET_ALARM_NAME_X;
        disp_rect.top = win_rect.top + MMIWIDGET_ALARM_NAME_Y;
		disp_rect.right = win_rect.left + (MMIWIDGET_ALARM_SWITCH_X - MMIWIDGET_ALARM_NAME_X - 10);
		disp_rect.bottom = disp_rect.top + str_info.height -1;
		
		GUISTR_DrawTextToLCDInRect(
                      			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                      			&disp_rect,
                      			&disp_rect,
                      			&label_str[event_id],	   
                      			&text_style,
                      			text_state,
                      			GUISTR_TEXT_DIR_AUTO
                      			);

        //display alarm time.
		SCI_MEMSET(Temp_str.wstr_ptr, 0x00, ((MMIWIDGET_ALARM_TEXT_MAX_LEN+1)*sizeof(wchar)));			   
		MMIAPIALARM_GetEventTimeString(event_ptr, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
		MMI_WSTRNCPY(Temp_str.wstr_ptr, MMIWIDGET_ALARM_TEXT_MAX_LEN, temp_wstr, MMIWIDGET_ALARM_TEXT_MAX_LEN, MMIAPICOM_Wstrlen(temp_wstr));
		
		Temp_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);	

		text_style.font = MMIWIDGET_ALARM_TIME_FONT;		
		GUISTR_GetStringInfo(&text_style, &Temp_str, text_state,&str_info);
		disp_rect.left = win_rect.left + MMIWIDGET_ALARM_TIME_X;
		disp_rect.top  = win_rect.top + MMIWIDGET_ALARM_TIME_Y;			
		disp_rect.right = win_rect.right + (MMIWIDGET_ALARM_FREQ_X - MMIWIDGET_ALARM_TIME_X);
		disp_rect.bottom = disp_rect.top + str_info.height -1;
		
		GUISTR_DrawTextToLCDInRect(
                     			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                     			&disp_rect,
                     			&disp_rect,
                     			&Temp_str,	   
                     			&text_style,
                     			text_state,
                     			GUISTR_TEXT_DIR_AUTO
                     			);

        //display current alarm on / off slider.
		disp_rect.left  = win_rect.left + MMIWIDGET_ALARM_SWITCH_X;
		temp_win_rect.left = disp_rect.left;
		disp_rect.top  = win_rect.top + MMIWIDGET_ALARM_SWITCH_Y;        
		disp_rect.left = temp_win_rect.left;	        
		point.x = temp_win_rect.left;
		point.y = disp_rect.top;

		if (!event_ptr->is_on && !event_delay_ptr->is_on)
		{
            img_id = IMAGE_WIDGET_ALARM_OFF_BG;		
		}
		else
		{
            img_id = IMAGE_WIDGET_ALARM_ON_BG;		
		}
		
		GUIRES_DisplayImg(
            			&point,
            			PNULL,
            			PNULL,
            			win_id,
            			img_id,
            			&lcd_dev_info
            			);
		
		GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_ALARM_ON_BG, win_id);
		temp_win_rect.bottom = disp_rect.top + image_height -1;

        //display on/off status.
		if (!event_ptr->is_on && !event_delay_ptr->is_on)
        {		     		 
            GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_ALARM_SLIDER, win_id);
            point.x = (win_rect.left + button_rect.left);	
            
            if( s_widget_alarm_button == s_widget_alarm_focus_idx)
            {
                temp_point.x = move_point.x;

                if(temp_point.x > (win_rect.left + button_rect.left + image_width ))
                {
                    temp_point.x = (win_rect.left + button_rect.right - image_width );
                }
                else
                {
                    temp_point.x = (win_rect.left + button_rect.left );					 
                }
                temp_point.y = point.y;
                MMIWIDGET_Alarm_DisplaySlideImg(win_id,temp_point,&lcd_dev_info);
            }
            else
                MMIWIDGET_Alarm_DisplaySlideImg(win_id,point,&lcd_dev_info);						           			
        }
		else
        {    
            GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_ALARM_SLIDER, win_id);

            point.x = (win_rect.left + button_rect.right - image_width );
             	
            if( s_widget_alarm_button == s_widget_alarm_focus_idx)
            {
                temp_point.x = move_point.x;
                if(temp_point.x < (win_rect.left + button_rect.right - image_width ))
                {
                    temp_point.x = (win_rect.left + button_rect.left );
                }
                else
                {
                    temp_point.x = (win_rect.left + button_rect.right - image_width );
                }
                temp_point.y = point.y;
                MMIWIDGET_Alarm_DisplaySlideImg(win_id,temp_point,&lcd_dev_info);
            }
            else
                MMIWIDGET_Alarm_DisplaySlideImg(win_id,point,&lcd_dev_info);		            
        }	

		//display alarm frequence.
		if(ALM_MODE_ONCE == event_ptr->fre_mode)
		{
			MMI_GetLabelTextByLang(fre_mode_text_id[ALM_MODE_ONCE], &label_str[event_id]);				 
		}
		else if(ALM_MODE_EVERYDAY == event_ptr->fre_mode)
		{
			MMI_GetLabelTextByLang(fre_mode_text_id[ALM_MODE_EVERYDAY], &label_str[event_id]);					 
		}
		else if(ALM_MODE_EVERYWEEK == event_ptr->fre_mode)
		{
			MMI_GetLabelTextByLang(fre_mode_text_id[ALM_MODE_EVERYWEEK], &label_str[event_id]);					 
		}	

		text_style.font = MMIWIDGET_ALARM_TEXT_FONT;		
		GUISTR_GetStringInfo(&text_style, &label_str[event_id], text_state,&str_info);	
		disp_rect.left  = win_rect.left + MMIWIDGET_ALARM_FREQ_X;		
		disp_rect.right = win_rect.right -1;
		disp_rect.top   = win_rect.top + MMIWIDGET_ALARM_FREQ_Y;
		disp_rect.bottom = disp_rect.top + str_info.height -1;	
		
		GUISTR_DrawTextToLCDInRect(
                    			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    			&disp_rect,
                    			&disp_rect,
                    			&label_str[event_id],	   
                    			&text_style,
                    			text_state,
                    			GUISTR_TEXT_DIR_AUTO
                    			);	

        //display indicator.
        point.x = win_rect.left + MMIWIDGET_ALARM_IND_X;
        point.y = win_rect.top  + MMIWIDGET_ALARM_IND_Y;
        img_id = IMAGE_WIDGET_ALARM_1 + s_widget_alarm_focus_idx;
        
 		GUIRES_DisplayImg(
            			&point,
            			PNULL,
            			PNULL,
            			win_id,
            			img_id,
            			&lcd_dev_info
            			); 

        //display all alarm focus status.
        point.x = win_rect.left + MMIWIDGET_ALARM_FOCUS_STARTX;
        point.y = win_rect.top  + MMIWIDGET_ALARM_FOCUS_STARTY;
        GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_ALARM_OFF, win_id);
        
        for (i = 0; i < ALM_ALARM_NUM; i++)
        {
        	event_id        = (uint16)(EVENT_ALARM_0 + i);
        	delay_event_id  = (uint16)(EVENT_ALARM_DELAY_0 + i);	
        	event_ptr       = MMIALM_GeEvent(event_id);
        	event_delay_ptr = MMIALM_GeEvent(delay_event_id); 

        	if (!event_ptr->is_on && !event_delay_ptr->is_on)
        	{
        	    if (i == s_widget_alarm_focus_idx)
        	    {
        	         img_id = IMAGE_WIDGET_ALARM_OFF_FOCUS;
        	    }
        	    else
        	    {
        	         img_id = IMAGE_WIDGET_ALARM_OFF;
        	    }
        	}
        	else
        	{
        	    if (i == s_widget_alarm_focus_idx)
        	    {
        	         img_id = IMAGE_WIDGET_ALARM_ON_FOCUS;
        	    }
        	    else
        	    {
        	         img_id = IMAGE_WIDGET_ALARM_ON;
        	    }        	
        	}

     		GUIRES_DisplayImg(
                			&point,
                			PNULL,
                			PNULL,
                			win_id,
                			img_id,
                			&lcd_dev_info
                			); 

    	    point.x += image_width + 2;
        }

        //display left/right arrow.
        if (0 == s_widget_alarm_focus_idx)
        {
            img_id = IMAGE_WIDGET_ALARM_LEFT_GRAY;
        }
        else
        {
            img_id = IMAGE_WIDGET_ALARM_LEFT;        
        }

        point.x = win_rect.left + MMIWIDGET_ALARM_LEFT_ARR_X;
        point.y = win_rect.top  + MMIWIDGET_ALARM_LEFT_ARR_Y;
        
 		GUIRES_DisplayImg(
            			&point,
            			PNULL,
            			PNULL,
            			win_id,
            			img_id,
            			&lcd_dev_info
            			);

        if (ALM_ALARM_NUM - 1 == s_widget_alarm_focus_idx)
        {
            img_id = IMAGE_WIDGET_ALARM_RIGHT_GRAY;
        }
        else
        {
            img_id = IMAGE_WIDGET_ALARM_RIGHT;        
        }			

        point.x = win_rect.left + MMIWIDGET_ALARM_RIGHT_ARR_X;
        point.y = win_rect.top  + MMIWIDGET_ALARM_RIGHT_ARR_Y;
        
 		GUIRES_DisplayImg(
            			&point,
            			PNULL,
            			PNULL,
            			win_id,
            			img_id,
            			&lcd_dev_info
            			);
        
	} 
	
	if (Temp_str.wstr_ptr != PNULL)
	{
		SCI_FREE(Temp_str.wstr_ptr);
		Temp_str.wstr_ptr = PNULL;
	}
		
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_Alarm_GetTextStyle(
										GUISTR_STYLE_T *style_ptr,
										GUI_FONT_T     font,
										GUI_COLOR_T    font_color
										)
{
    if (PNULL == style_ptr)
    {
        return;
    }
	
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
	style_ptr->line_space = 0;    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmTpPressDown(
										   MMI_WIN_ID_T        win_id,        //IN:
										   DPARAM            param        //IN:
										   )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_POINT_T  point  = {0};
	GUI_RECT_T button_rect = MMIWIDGET_ALARM_BUTTON_RECT;
	
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
	
	s_widget_alarm_down_point = point;	
	
	if(GUI_PointIsInRect(point, button_rect))
	{
        s_widget_alarm_down_button = s_widget_alarm_focus_idx;
        
#if (defined MMI_GRID_IDLE_SUPPORT)
        if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
        {
            MMIGRID_CreateApplicationLayer(0,0);
        }
#endif
#if defined MMIWIDGET_SUPPORT
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
        {
            MMIWIDGET_CreateApplicationLayer();
        }
#endif  
	}
	else
	{
	    s_widget_alarm_down_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;		
		recode = MMI_RESULT_FALSE;
	}
	return recode;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmTpPressUp(MMI_WIN_ID_T win_id, DPARAM param)
{
    uint16 index = 0;
    uint16 img_width  = 0;
    uint16 img_height = 0;
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_POINT_T point = {0};	
	GUI_RECT_T text_rect   = MMIWIDGET_ALARM_NAME_RECT;
	GUI_RECT_T larrow_rect = MMIWIDGET_ALARM_LARR_RECT;
	GUI_RECT_T rarrow_rect = MMIWIDGET_ALARM_RARR_RECT;
	GUI_RECT_T indic_rect  = MMIWIDGET_ALARM_INDIC_RECT;
    GUI_RECT_T rect        = {0};
	int16 move_step = 0;
	uint16 alarm_on_off_image_width = 0;
	uint16 slide_image_width = 0;	
	uint16 chang_step = 0;
	BOOLEAN dir =FALSE;	 

	
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);	
	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
	
#if (defined MMI_GRID_IDLE_SUPPORT)
    if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
    {
        MMIGRID_DestroyApplicationLayer();
    }
#endif
#if (defined MMIWIDGET_SUPPORT)
    if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
    {
       MMIWIDGET_DestroyApplicationLayer();
    }
#endif 

    if((s_widget_alarm_focus_idx == s_widget_alarm_down_button))
	{   
       GUIRES_GetImgWidthHeight(&alarm_on_off_image_width, PNULL, IMAGE_WIDGET_ALARM_ON_BG, win_id);
	   GUIRES_GetImgWidthHeight(&slide_image_width, PNULL, IMAGE_WIDGET_ALARM_SLIDER, win_id);	

	   chang_step = (alarm_on_off_image_width - slide_image_width)/2;
	   move_step = abs(point.x - s_widget_alarm_down_point.x);

	   if((point.x - s_widget_alarm_down_point.x) > 0)
	   {
		   dir = TRUE;
	   }
	   else if((point.x - s_widget_alarm_down_point.x) < 0)
	   {
		   dir = FALSE;
	   }
	   
	   if(move_step > chang_step)
	   {
		   MMIWIDGET_Alarm_ModifyEventbyEventid(s_widget_alarm_down_button,dir); /*lint !e730*/
	   }
	   else
	   {
	       recode = DirectlyHandleTpPressUp(s_widget_alarm_down_button,point,slide_image_width);
	   }
	}
	else
	if(GUI_PointIsInRect(point, text_rect))
	{
		MMIAPIALARM_OpenEditAlarmWinbyEventid(s_widget_alarm_focus_idx);
	}
	else
	if (GUI_PointIsInRect(point, larrow_rect))
	{
	    s_widget_alarm_focus_idx = (s_widget_alarm_focus_idx > 0) ? (s_widget_alarm_focus_idx - 1) : s_widget_alarm_focus_idx;
	}
	else
	if (GUI_PointIsInRect(point, rarrow_rect))
	{
	    s_widget_alarm_focus_idx = (s_widget_alarm_focus_idx < ALM_ALARM_NUM - 1) ? (s_widget_alarm_focus_idx + 1) : s_widget_alarm_focus_idx;	
	}
	else
	if (GUI_PointIsInRect(point, indic_rect))
	{
	    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_WIDGET_ALARM_OFF, win_id);
	    
        rect.top    = indic_rect.top;
        rect.bottom = indic_rect.bottom;

	    for (index = 0; index < ALM_ALARM_NUM; index++)
	    {
	        rect.left   =  indic_rect.left + index * (img_width + 2);
	        rect.right  =  rect.left + img_width;
	        
	        if (GUI_PointIsInRect(point, rect))
	        {
	            s_widget_alarm_focus_idx = index;
	            break;
	        }
	    }
	}
	else
	{
		recode = MMI_RESULT_FALSE;
	}
	
	s_widget_alarm_down_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;	
	s_widget_alarm_button =WIDGET_ALARM_BUTTON_INVALIDATE_ID;
	return recode;
	
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmTpPressMove(MMI_WIN_ID_T win_id, DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_POINT_T point = {0};
	GUI_RECT_T button_rect = MMIWIDGET_ALARM_BUTTON_RECT;
	GUI_RECT_T  win_rect = {0};
	GUI_LCD_DEV_INFO lcd_dev_info = {0};

	
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	MMK_GetWinRect(win_id, &win_rect);
	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

	//防止拖到另外的BUTTON导致两个在动
	if(abs(point.y - s_widget_alarm_down_point.y) > (button_rect.bottom - button_rect.top)/2)
	{
		return MMI_RESULT_FALSE;
    }

	if(GUI_PointIsInRect(point, button_rect))
	{
	     BOOLEAN  is_app_layer = FALSE;
	     
#if (defined MMI_GRID_IDLE_SUPPORT)
         if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
         {
             is_app_layer = MMIGRID_GetApplicationLayer(&lcd_dev_info);
         }
#endif
#if (defined MMIWIDGET_SUPPORT)
         if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
         {
             is_app_layer = MMIWIDGET_GetApplicationLayer(&lcd_dev_info);
         }
#endif	
		if(is_app_layer)
		{
			UILAYER_Clear(&lcd_dev_info);		
			s_widget_alarm_button =0;	
			MMK_SendMsg(win_id,MSG_FULL_PAINT,param);
		}
		
	}
	else
	{
		s_widget_alarm_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;
		recode = MMI_RESULT_FALSE;
	}
	return recode;
}

LOCAL void MMIWIDGET_Alarm_DisplaySlideImg(MMI_WIN_ID_T win_id, GUI_POINT_T  point, const GUI_LCD_DEV_INFO *dev_info_ptr  )
{	
	GUIRES_DisplayImg(
            		&point,
            		PNULL,
            		PNULL,
            		win_id,
            	    IMAGE_WIDGET_ALARM_SLIDER,
            	    dev_info_ptr
            	    );
}

LOCAL void MMIWIDGET_Alarm_ModifyEventbyEventid(uint16 event_id, BOOLEAN is_on)
{
    uint16 nor_event_id   = EVENT_ALARM_0 + event_id;
    uint16 delay_event_id = EVENT_ALARM_DELAY_0 + event_id;
	MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr       = PNULL;
	MMIACC_SMART_EVENT_FAST_TABLE_T *delay_event_ptr = PNULL;

    event_ptr = (MMIACC_SMART_EVENT_FAST_TABLE_T*)MMIALM_GeEvent( nor_event_id );
	delay_event_ptr = (MMIACC_SMART_EVENT_FAST_TABLE_T*)MMIALM_GeEvent(delay_event_id);
	
	event_ptr->is_on = is_on;
	delay_event_ptr->is_on = is_on;
	
	MMIALM_ModifyEvent(event_id, event_ptr);
	MMIALM_ModifyEvent(delay_event_id, delay_event_ptr);
}

LOCAL BOOLEAN MMIWIDGET_Alarm_GetEventStatusbyEventid(uint16 event_id)
{
	MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 	= PNULL;
	
	event_ptr = MMIALM_GeEvent( event_id );/*lint !e605*/
	
	return event_ptr->is_on;
	
}
/*****************************************************************************/
//  Description : 直接处理UP事件，以最左边或最右边，做一个valid_step长的矩形
// 拖动的距离不在有效范围时，直接点击需要调用此函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DirectlyHandleTpPressUp(uint16 event_id,GUI_POINT_T up_point,uint16 valid_step)
{
	BOOLEAN    event_is_on  = FALSE;	
	MMI_RESULT_E recode     = MMI_RESULT_TRUE;
	GUI_RECT_T button_rect = MMIWIDGET_ALARM_BUTTON_RECT;

	event_is_on = MMIWIDGET_Alarm_GetEventStatusbyEventid(event_id);

	if(event_is_on)
	{
		button_rect.right = button_rect.left + valid_step;
	}
	else
	{
		button_rect.left = button_rect.right - valid_step;
	}

    if(GUI_PointIsInRect(up_point, button_rect))
    {
        MMIWIDGET_Alarm_ModifyEventbyEventid(event_id,!event_is_on); /*lint !e730*/
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }
    
    return recode;	  
}
#endif
