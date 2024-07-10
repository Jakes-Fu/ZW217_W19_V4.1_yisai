/*****************************************************************************
** File Name:      mmigrid_alarm.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009    hongbo.lan           Creat
******************************************************************************/

#define _MMIGRID_ALARM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_ALARM
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"

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
#include "mmk_tp.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif

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
#define   MMIWIDGET_ALARM_MARGIN                 4
#define   MMIWIDGET_ALARM_MAX_NUM                ALM_WIDGET_ALARM_NUM

#if defined (MAINLCD_SIZE_240X400)
#elif defined(MAINLCD_SIZE_240X320)
#elif defined(MAINLCD_SIZE_320X480)
#else
#endif

LOCAL uint32       s_is_widget_alarm_hit  = WIDGET_ALARM_BUTTON_INVALIDATE_ID;  //tp down 所在switch 的index，255非法
LOCAL uint32       s_widget_alarm_down_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;//tp down 所在item 的index，255非法

LOCAL GUI_RECT_T GetOneAlarmRect(MMI_WIN_ID_T win_id,int32 index );
LOCAL GUI_RECT_T GetAlarmIconRect(MMI_WIN_ID_T win_id, int32 index );
LOCAL GUI_RECT_T GetAlarmLineRect(MMI_WIN_ID_T win_id,int32 index );
LOCAL GUI_RECT_T GetAlarmNameTimeInfoRect(MMI_WIN_ID_T win_id,int32 index );
LOCAL GUI_RECT_T GetAlarmFrequencyInfoRect(MMI_WIN_ID_T win_id,int32 index );
LOCAL GUI_RECT_T GetAlarmSwitchRect(MMI_WIN_ID_T win_id,int32 index );

LOCAL void DisplayAlarmIcon(MMI_WIN_ID_T win_id, int32 index );
LOCAL void DisplayAlarmLine(MMI_WIN_ID_T win_id, int32 index );
LOCAL void DisplayAlarmSwitch(MMI_WIN_ID_T win_id, int32 index );

/*****************************************************************************/
// Description : Display grid alarm panel.
// Global resource dependence : 
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DisplayAlarmPanel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : Get alarm text style.
// Global resource dependence : 
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void GetAlarmtTextStyle(GUISTR_STYLE_T *style_ptr,GUI_FONT_T font, GUI_COLOR_T  font_color);

/*****************************************************************************/
// Description : handle grid alarm tp press down message.
// Global resource dependence : 
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGridAlarmTpDown(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
// Description : handle grid alarm tp press up message.
// Global resource dependence : 
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGridAlarmTpUp(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
// Description : modify alarm on/off status.
// Global resource dependence : 
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void ModifyAlarmEventbyEventid(uint16 event_id, BOOLEAN is_on);

/*****************************************************************************/
//  Description : Handle grid alarm window Msg
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGridAlarmWinMsg(MMI_WIN_ID_T        win_id,        //IN:
											MMI_MESSAGE_ID_E    msg_id,        //IN:
											DPARAM              param        //IN:
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

PUBLIC const MMIWIDGET_ITEM_INFO_T g_alarm_grid =
{
	MMIWIDGET_ALARM_ID, 
	HandleGridAlarmWinMsg, 
	PNULL, 
	ResetWidgetAlarm,
	PNULL,
	MMI_GRID_ALARM_WIN_ID, 
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
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void ResetWidgetAlarm()
{
	s_is_widget_alarm_hit = WIDGET_ALARM_BUTTON_INVALIDATE_ID;
	s_widget_alarm_down_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;	  
}

/*****************************************************************************/
//  Description :  get one alarm item rect, all item num = 3
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetOneAlarmRect(MMI_WIN_ID_T win_id,int32 index )
{
    uint16 height = 0;
    GUI_RECT_T win_rect = {0};    
    GUI_RECT_T item_rect = {0};
	MMK_GetWinRect(win_id, &win_rect);
    win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_DISP2WIN, win_id, win_rect);
	item_rect = win_rect;
	height = (win_rect.bottom - win_rect.top + 1) / MMIWIDGET_ALARM_MAX_NUM;
	item_rect.top = win_rect.top + height*index;
	item_rect.bottom = item_rect.top + height - 1;
	return item_rect;
}

LOCAL GUI_RECT_T GetAlarmIconRect(MMI_WIN_ID_T win_id, int32 index )
{    
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;    
    uint16 img_height = 0;
    item_rect = GetOneAlarmRect(win_id, index);
    item_rect.left += MMIWIDGET_ALARM_MARGIN;
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  IMAGE_WIDGET_ALARM_ALARM, win_id);
    item_rect.right = item_rect.left + img_width;   
	return item_rect;
}

LOCAL GUI_RECT_T GetAlarmLineRect(MMI_WIN_ID_T win_id,int32 index )
{
    GUI_RECT_T item_rect1 = {0};
    GUI_RECT_T item_rect2 = {0};
    GUI_RECT_T item_rect = {0};

    
    item_rect1 = GetAlarmIconRect(win_id, index);
    item_rect2 = GetAlarmSwitchRect(win_id, index);
    item_rect = item_rect1;
    item_rect.left = item_rect1.right + MMIWIDGET_ALARM_MARGIN;
    item_rect.right = item_rect2.left - MMIWIDGET_ALARM_MARGIN;
	return item_rect;
}

LOCAL GUI_RECT_T GetAlarmNameTimeInfoRect(MMI_WIN_ID_T win_id,int32 index )
{
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;    
    uint16 img_height = 0;
    
    item_rect = GetAlarmLineRect(win_id, index);    
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  IMAGE_WIDGET_ALARM_LINE, win_id);
    item_rect.bottom = item_rect.top + (item_rect.bottom - item_rect.top + 1 - img_height)/2 - 1;
    item_rect.left += MMIWIDGET_ALARM_MARGIN;
    item_rect.right -= MMIWIDGET_ALARM_MARGIN;
	return item_rect;
}

LOCAL GUI_RECT_T GetAlarmFrequencyInfoRect(MMI_WIN_ID_T win_id,int32 index )
{
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;    
    uint16 img_height = 0;
    
    item_rect = GetAlarmLineRect(win_id, index);    
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  IMAGE_WIDGET_ALARM_LINE, win_id);
    item_rect.top = item_rect.bottom - (item_rect.bottom - item_rect.top + 1 - img_height)/2 + 1;
    item_rect.left += MMIWIDGET_ALARM_MARGIN;
    item_rect.right -= MMIWIDGET_ALARM_MARGIN;
	return item_rect;
}


LOCAL GUI_RECT_T GetAlarmSwitchRect(MMI_WIN_ID_T win_id,int32 index )
{
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;    
    uint16 img_height = 0;
    item_rect = GetOneAlarmRect(win_id, index);
    item_rect.right -= MMIWIDGET_ALARM_MARGIN;
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  IMAGE_WIDGET_ALARM_ON, win_id);
    item_rect.left = item_rect.right - img_width;    
	return item_rect;
}

LOCAL void DisplayAlarmIcon(MMI_WIN_ID_T win_id, int32 index )
{
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;    
    uint16 img_height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);		
    item_rect = GetAlarmIconRect(win_id, index);
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  IMAGE_WIDGET_ALARM_ALARM, win_id);
    item_rect = GUI_GetCenterRect( item_rect, img_width, img_height);
    item_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, item_rect);
    GUIRES_DisplayImg(
                      PNULL,
                      &item_rect,
                      PNULL,
                      win_id,
                      IMAGE_WIDGET_ALARM_ALARM,
                      &lcd_dev_info
                      );
}
LOCAL void DisplayAlarmLine(MMI_WIN_ID_T win_id, int32 index )
{
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;    
    uint16 img_height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);		
    item_rect = GetAlarmLineRect(win_id, index);
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  IMAGE_WIDGET_ALARM_LINE, win_id);
    item_rect = GUI_GetCenterRect( item_rect, img_width, img_height);
    item_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, item_rect);
    GUIRES_DisplayImg(
                      PNULL,
                      &item_rect,
                      PNULL,
                      win_id,
                      IMAGE_WIDGET_ALARM_LINE,
                      &lcd_dev_info
                      );

}
LOCAL void DisplayAlarmSwitch(MMI_WIN_ID_T win_id, int32 index )
{
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;    
    uint16 img_height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
	const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 	= PNULL;
	const MMIACC_SMART_EVENT_FAST_TABLE_T *event_delay_ptr = PNULL;
	uint16 event_id	  = 0; 
	uint16 delay_event_id = 0;
	MMI_IMAGE_ID_T  img_id = 0;
	event_id        = (uint16)(EVENT_ALARM_0 + index);
	delay_event_id  = (uint16)(EVENT_ALARM_DELAY_0 + index);	
	event_ptr       = MMIALM_GeEvent(event_id);
	event_delay_ptr = MMIALM_GeEvent(delay_event_id);
	if(event_ptr== PNULL)
	{
	    //SCI_TRACE_LOW:"MMIGIRD_ALARM DisplayAlarmSwitch event_ptr = 0"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_ALARM_311_112_2_18_3_6_53_0,(uint8*)"");
	    return;
	}
	if(event_delay_ptr== PNULL)
	{
	    //SCI_TRACE_LOW:"MMIGIRD_ALARM DisplayAlarmSwitch event_delay_ptr = 0"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_ALARM_316_112_2_18_3_6_53_1,(uint8*)"");
	    return;
	}
    if (!event_ptr->is_on && !event_delay_ptr->is_on)
    {
        img_id = IMAGE_WIDGET_ALARM_OFF;     
    }
    else
    {
        img_id = IMAGE_WIDGET_ALARM_ON;      
    }
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);		
    item_rect = GetAlarmSwitchRect(win_id, index);
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  img_id, win_id);
    item_rect = GUI_GetCenterRect( item_rect, img_width, img_height);
    item_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, item_rect);
    GUIRES_DisplayImg(
                      PNULL,
                      &item_rect,
                      PNULL,
                      win_id,
                      img_id,
                      &lcd_dev_info
                      );
}

/*****************************************************************************/
//  Description : Handle grid alarm window Msg
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGridAlarmWinMsg(MMI_WIN_ID_T        win_id,        //IN:
										  MMI_MESSAGE_ID_E    msg_id,        //IN:
										  DPARAM            param        //IN:
										  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

	switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_FULL_PAINT:
        DisplayAlarmPanel(win_id);
        break;
        
    case MSG_WIDGET_TP_PRESS_DOWN:
        recode = HandleGridAlarmTpDown(win_id,param);
        break;
       
    case MSG_WIDGET_TP_PRESS_UP:
        recode = HandleGridAlarmTpUp(win_id,param);
        break;        
        
    case  MSG_WIDGET_PAGE_SLIDE_START:
        ResetWidgetAlarm();
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_APP_WEB:
        MMIAPIALM_OpenMainWin();
        break;
    case MSG_LCD_SWITCH:
        ResetWidgetAlarm();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;        
}

LOCAL void DisplayAlarmInfo(MMI_WIN_ID_T win_id, int32 index )
{
	GUI_RECT_T disp_rect = {0};
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T    temp_str= {0};
	MMI_STRING_T    label_str= {0};	
	uint32        fre_mode_text_id[] = {TXT_ALARM_MODE_ONCE, TXT_ALARM_MODE_EVERYDAY, TXT_ALARM_MODE_WEEK};	
	wchar temp_wstr[MMIWIDGET_ALARM_TEXT_MAX_LEN + 1] = {0};

	uint16 event_id	  = 0; 
	uint16 delay_event_id = 0;
	const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 	= PNULL;
	const MMIACC_SMART_EVENT_FAST_TABLE_T *event_delay_ptr = PNULL;
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ALIGN_BY_DIR \
                |GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;	
    MMIACC_SMART_EVENT_T    smart_event = {0};
			                    
	GetAlarmtTextStyle(&text_style, MMIWIDGET_ALARM_TEXT_FONT, MMIWIDGET_ALARM_TEXT_COLOR);	

    event_id        = (uint16)(EVENT_ALARM_0 + index);
    delay_event_id  = (uint16)(EVENT_ALARM_DELAY_0 + index);	
    event_ptr       = MMIALM_GeEvent(event_id);
    event_delay_ptr = MMIALM_GeEvent(delay_event_id);
    
    if(event_ptr== PNULL)
    {
        //SCI_TRACE_LOW:"MMIGIRD_ALARM DisplayAlarmSwitch event_ptr = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_ALARM_419_112_2_18_3_6_53_2,(uint8*)"");
        return;
    }
    if(event_delay_ptr== PNULL)
    {
        //SCI_TRACE_LOW:"MMIGIRD_ALARM DisplayAlarmSwitch event_delay_ptr = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_ALARM_424_112_2_18_3_6_53_3,(uint8*)"");
        return;
    }    
    //display alaram name.		
    /*
    MMIAPIALM_FormInteEventInfo(event_id,&smart_event);
    if (0 != smart_event.event_content_info.w_length)
    {
    label_str[event_id].wstr_ptr = smart_event.event_content_info.w_content;
    label_str[event_id].wstr_len = smart_event.event_content_info.w_length;
    }
    
      GUISTR_GetStringInfo(&text_style, &label_str[event_id], text_state,&str_info);			
      GUISTR_DrawTextToLCDInRect(
      (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
      &disp_rect,
      &disp_rect,
      &label_str[event_id],	   
      &text_style,
      text_state,
      0
      );
    */
    
    //if (!event_ptr->is_on && !event_delay_ptr->is_on)
    //{
    //    return;
    //}
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);	
    
    //display alarm time.
    MMIAPIALARM_GetEventTimeString(event_ptr, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );	
    temp_str.wstr_ptr = temp_wstr;
    temp_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
    disp_rect = GetAlarmNameTimeInfoRect(win_id, index);
    disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);
    text_style.align = ALIGN_BOTTOM;
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &disp_rect,
        &disp_rect,
        &temp_str,	   
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    //display alarm frequence.
    if(ALM_MODE_ONCE == event_ptr->fre_mode)
    {
        MMI_GetLabelTextByLang(fre_mode_text_id[ALM_MODE_ONCE], &label_str);				 
    }
    else if(ALM_MODE_EVERYDAY == event_ptr->fre_mode)
    {
        MMI_GetLabelTextByLang(fre_mode_text_id[ALM_MODE_EVERYDAY], &label_str);					 
    }
    else if(ALM_MODE_EVERYWEEK == event_ptr->fre_mode)
    {
        //MMI_GetLabelTextByLang(fre_mode_text_id[ALM_MODE_EVERYWEEK], &label_str);	
        
        MMIAPIALM_FormInteEventInfo(event_id,&smart_event);
        SCI_MEMSET(temp_wstr, 0 , MMIWIDGET_ALARM_TEXT_MAX_LEN*sizeof(wchar));
        label_str.wstr_len = MMIAPIALARM_GetFreModeString(smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre,temp_wstr,GUILIST_STRING_MAX_NUM+1);
        label_str.wstr_ptr = temp_wstr;
    }	
    
    disp_rect = GetAlarmFrequencyInfoRect(win_id, index);
    disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);
    text_style.align = ALIGN_TOP;

    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &disp_rect,
        &disp_rect,
        &label_str,	   
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );	
    
}

/*****************************************************************************/
//  Description :  display grid alarm panel.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DisplayAlarmPanel(MMI_WIN_ID_T win_id)
{
    int32 i = 0;
	GUI_RECT_T win_rect = {0};	
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	
    MMK_GetWinRect(win_id, &win_rect);    
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);     
    if(!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }
    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&lcd_dev_info))
    {
        UILAYER_ClearRect(&lcd_dev_info, win_rect);        
    }
    GUIRES_DisplayImg(
                      PNULL,
                      &win_rect,
                      PNULL,
                      win_id,
                      IMAGE_WIDGET_ALARM_BG,
                      &lcd_dev_info
                      );
    
    for(i = 0; i< MMIWIDGET_ALARM_MAX_NUM ; i++)
    {
        DisplayAlarmIcon( win_id,  i );
        DisplayAlarmLine( win_id,  i );
        DisplayAlarmInfo(   win_id, i );
        DisplayAlarmSwitch( win_id,  i );
    }        
}


/*****************************************************************************/
//  Description : get grid alarm text style.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetAlarmtTextStyle(GUISTR_STYLE_T *style_ptr,
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
    style_ptr->align = ALIGN_LVMIDDLE;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
	style_ptr->line_space = 0;    
    style_ptr->edge_alpha = 0x40;
}

/*****************************************************************************/
//  Description: handle grid alarm tp down message. 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGridAlarmTpDown(MMI_WIN_ID_T        win_id,        //IN:
										  DPARAM            param        //IN:
										  )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_POINT_T  point  = {0};
	GUI_RECT_T button_rect = MMIWIDGET_ALARM_BUTTON_RECT; 
    GUI_RECT_T item_rect = {0};	
    int32 i = 0;
    
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
	    
	for(i = 0; i < MMIWIDGET_ALARM_MAX_NUM; i++)
	{
	    button_rect = GetAlarmSwitchRect( win_id,  i);	
	    item_rect = GetOneAlarmRect( win_id,  i);	
    	if(GUI_PointIsInRect(point, button_rect))
    	{
            s_widget_alarm_down_button = i;
            break;
    	}
    	else if (GUI_PointIsInRect(point, item_rect))
    	{
            s_is_widget_alarm_hit = i;
            break;
    	}
   }
	return recode;
}

/*****************************************************************************/
//  Description: handle grid alarm tp up message. 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGridAlarmTpUp(MMI_WIN_ID_T win_id, DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_FALSE;
	GUI_POINT_T point = {0};
	
	uint16 event_id	  = 0; 
	uint16 delay_event_id = 0;
	MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 	= PNULL;
	MMIACC_SMART_EVENT_FAST_TABLE_T *event_delay_ptr = PNULL;

	GUI_RECT_T button_rect = {0};
	GUI_RECT_T item_rect = {0};

	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);	
	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    if(s_widget_alarm_down_button != WIDGET_ALARM_BUTTON_INVALIDATE_ID)
    {
        button_rect = GetAlarmSwitchRect( win_id,  s_widget_alarm_down_button);  
    	if(GUI_PointIsInRect(point, button_rect))
    	{    	    
            event_id        = (uint16)(EVENT_ALARM_0 + s_widget_alarm_down_button);
            delay_event_id  = (uint16)(EVENT_ALARM_DELAY_0 + s_widget_alarm_down_button); 
            event_ptr       = (MMIACC_SMART_EVENT_FAST_TABLE_T *)MMIALM_GeEvent(event_id);
            event_delay_ptr = (MMIACC_SMART_EVENT_FAST_TABLE_T *)MMIALM_GeEvent(delay_event_id);
            if(event_ptr== PNULL)
            {
                //SCI_TRACE_LOW:"MMIGIRD_ALARM HandleGridAlarmTpUp event_ptr = 0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_ALARM_628_112_2_18_3_6_54_4,(uint8*)"");
                return recode;
            }
            if(event_delay_ptr== PNULL)
            {
                //SCI_TRACE_LOW:"MMIGIRD_ALARM HandleGridAlarmTpUp event_delay_ptr = 0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_ALARM_633_112_2_18_3_6_54_5,(uint8*)"");
                return recode;
            }
            
            if (!event_ptr->is_on && !event_delay_ptr->is_on)
            {
                ModifyAlarmEventbyEventid(s_widget_alarm_down_button, TRUE); /*lint !e730*/
            }
            
            else
            {
                ModifyAlarmEventbyEventid(s_widget_alarm_down_button, FALSE); /*lint !e730*/
            }
            
            DisplayAlarmPanel(win_id);
    	}    	
    }
    if(s_is_widget_alarm_hit != WIDGET_ALARM_BUTTON_INVALIDATE_ID)
    {
	    item_rect = GetOneAlarmRect( win_id,  s_is_widget_alarm_hit);	    
    	if(GUI_PointIsInRect(point, item_rect))
    	{
		    MMIAPIALARM_OpenEditAlarmWinbyEventid(s_is_widget_alarm_hit);
		}
    }

	s_widget_alarm_down_button = WIDGET_ALARM_BUTTON_INVALIDATE_ID;	
	s_is_widget_alarm_hit =WIDGET_ALARM_BUTTON_INVALIDATE_ID;
    recode = MMI_RESULT_TRUE;
	return recode;	
}

/*****************************************************************************/
//  Description: modify alarm on/off status.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void ModifyAlarmEventbyEventid(uint16 event_id, BOOLEAN is_on)
{
    uint16 nor_event_id   = EVENT_ALARM_0 + event_id;
    uint16 delay_event_id = EVENT_ALARM_DELAY_0 + event_id;
	MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr       = PNULL;
	MMIACC_SMART_EVENT_FAST_TABLE_T *delay_event_ptr = PNULL;

    event_ptr = (MMIACC_SMART_EVENT_FAST_TABLE_T*)MMIALM_GeEvent( nor_event_id );
	delay_event_ptr = (MMIACC_SMART_EVENT_FAST_TABLE_T*)MMIALM_GeEvent(delay_event_id);
	
	event_ptr->is_on = is_on;
	delay_event_ptr->is_on = FALSE;
	
	MMIALM_ModifyEvent(event_id, event_ptr);
	MMIALM_ModifyEvent(delay_event_id, delay_event_ptr);
}     
#endif
