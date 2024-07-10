/*****************************************************************************
** File Name:      mmiidle_func.c                                            *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Creat
******************************************************************************/

#define _MMIIDLE_FUNC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#include "mmiidle_func.h"

#include "mmiacc_event.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "mmi_theme.h"
#if defined MMI_SMART_IDLE_SUPPORT || defined MMI_COMMON_IDLE_SUPPORT ||defined BAR_PHONE_WIDGET 
#include "mmischedule_export.h"
#endif
#include "mmi_modu_main.h"
#include "mmialarm_export.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmistk_export.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#include "guirichtext.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//time length

#define MMIIDLE_GET_STR_DISPLAY_LEN_OFFSET 12

#define IDLE_STK_TIMER_PERIOD       150 //idle下stk text显示的定时器周期

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL IDLE_STK_TEXT_INFO    s_idle_stk_text = {0};

LOCAL IDLE_WIN_INFO_T       s_idlewin_info; // Idle窗口的相关Information
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
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DrawStkIdleModeText(uint32 block_id);
#endif
/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            );

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description :to handle STK Idle Text Win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStkIdleTextWinMsg(
                                                                              MMI_WIN_ID_T win_id, 
                                                                              MMI_MESSAGE_ID_E msg_id, 
                                                                              DPARAM param
                                                                              );
#endif

#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON|| defined MMI_COMMON_IDLE_SUPPORT ||defined BAR_PHONE_WIDGET
#ifdef CALENDAR_SUPPORT 
/*****************************************************************************/
//  Description : display schedule
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplaySchedule(const GUI_LCD_DEV_INFO *lcd_dev_ptr,uint8 direction, uint16 max_len, GUI_POINT_T  *dis_point,GUI_FONT_T font_type)
{
    BOOLEAN                 is_alloc = FALSE;
    uint8                   time_str[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_wstr[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_str_len = 0;   
    uint16                  event_id = 0;
    uint16                  schedule_wstr_len = 0;
    uint16                  schedule_full_wstr_len  = 0;
    uint16                  display_max_len = 0;
    MMI_STRING_T            name_str = {0};
    MMI_STRING_T            schedule_str = {0};
    MMI_STRING_T            schedule_event_str = {0};
    MMI_STRING_T            schedule_full_event_str = {0};
    MMIACC_SMART_EVENT_T*   schedule_event_ptr = PNULL;
    wchar*                  event_str_ptr = PNULL;
    wchar*                  full_str_ptr = PNULL;//[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    uint16                  full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    uint16                  event_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    BOOLEAN                 is_dis_suspension = FALSE;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T            full_str = {0};
    MMI_STRING_T            event_str = {0};
    //get schedule
    if (!MMIAPISCH_OpenEventToday(&event_id))
    {
        //今日无日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOCALENDAR,&schedule_str);
    }
    else if (MMIAPISCH_IsAllPastEventToday(&event_id))
    {
        //今日无更多日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOMORECALENDAR,&schedule_str);
    }
    else
    {
        //日程
        MMI_GetLabelTextByLang(TXT_IDLE_CALENDAR,&name_str);

        //get schedule
        schedule_event_ptr = (MMIACC_SMART_EVENT_T*)SCI_ALLOCA(sizeof(MMIACC_SMART_EVENT_T));
        //SCI_ASSERT(PNULL != schedule_event_ptr);
        if(PNULL == schedule_event_ptr)
        {
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_138_112_2_18_2_24_7_54,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_event_ptr, 0, sizeof(MMIACC_SMART_EVENT_T) );
        MMIAPIALM_FormInteEventInfo(event_id,schedule_event_ptr);

        //timer
		MMIAPISET_FormatTimeStrByTime(
			schedule_event_ptr->event_fast_info.hour,
			schedule_event_ptr->event_fast_info.minute,
			time_str,
			MMIIDLE_TIME_LEN
			);
        time_str_len = strlen((char *)time_str);
        MMI_STRNTOWSTR(time_wstr,MMIIDLE_TIME_LEN,
            time_str,MMIIDLE_TIME_LEN,
            time_str_len);

        //get schedule string len
        schedule_str.wstr_len = name_str.wstr_len + time_str_len + 1 + 1;//两个空格
        
        //copy schedule string
        schedule_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_str.wstr_ptr )
        {
            SCI_FREE(schedule_event_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_164_112_2_18_2_24_7_55,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_str.wstr_ptr,0,((schedule_str.wstr_len+1)*sizeof(wchar)));
        
        schedule_event_str.wstr_len = schedule_event_ptr->event_content_info.w_length;
        schedule_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_event_str.wstr_ptr)
        {
            SCI_FREE(schedule_event_ptr);
            SCI_FREE(schedule_str.wstr_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_176_112_2_18_2_24_7_56,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_event_str.wstr_ptr,0,((schedule_event_str.wstr_len+1)*sizeof(wchar)));
        is_alloc = TRUE;
        if (MMI_DIRECTION_RIGHT==direction)
        {
            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                time_wstr,time_str_len,
                time_str_len);
			schedule_str.wstr_ptr[time_str_len] = ' ';//CR165770
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr+time_str_len + 1,schedule_str.wstr_len-time_str_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);
        }
        else
        {
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);
            schedule_str.wstr_ptr[name_str.wstr_len] = ' ';//cr165770
            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len + 1,schedule_str.wstr_len-name_str.wstr_len,
                time_wstr,MMIIDLE_TIME_LEN,
                time_str_len);
        }
        
	//	schedule_str.wstr_ptr[name_str.wstr_len+time_str_len + 1]= ' ';//cr173732
        MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len+time_str_len + 1,schedule_str.wstr_len-name_str.wstr_len-time_str_len,L" ",1,1);/*lint !e64*///CR165770
        //copy event
        MMI_WSTRNCPY(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,
            schedule_event_ptr->event_content_info.w_content,schedule_event_ptr->event_content_info.w_length,
            schedule_event_ptr->event_content_info.w_length);

        event_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        if(PNULL == event_str_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
            SCI_FREE(schedule_event_str.wstr_ptr);
            SCI_FREE(schedule_event_ptr);
          
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == event_str_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_219_112_2_18_2_24_7_57,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET( event_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        MMIIDLE_GetDisPlayString(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,max_len-display_max_len,font_type,event_str_ptr,&event_len);//MARYXIAO CR230653
        SCI_FREE(schedule_event_ptr);
        SCI_FREE(schedule_event_str.wstr_ptr);
    }
    //display string max len
    display_max_len = max_len;
    full_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    //SCI_ASSERT(PNULL != full_str_ptr);
    if(PNULL == full_str_ptr)
    {
        if(PNULL != schedule_str.wstr_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
        }
       
        if(PNULL != event_str_ptr)
        {
            SCI_FREE(event_str_ptr);
        }
        
        //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == full_str_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_244_112_2_18_2_24_7_58,(uint8*)"");
        return 0;
    }
    
    if(event_str_ptr != PNULL)
    {
        event_str.wstr_ptr = event_str_ptr;
        event_str.wstr_len = event_len;
        schedule_full_event_str.wstr_len = schedule_str.wstr_len + event_str.wstr_len;//拼接
        schedule_full_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_full_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_full_event_str.wstr_ptr)
        {
            if(PNULL != schedule_str.wstr_ptr)
            {
                SCI_FREE(schedule_str.wstr_ptr);
            }
 
            SCI_FREE(event_str_ptr);
            
            SCI_FREE(full_str_ptr);
            
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_full_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_265_112_2_18_2_24_7_59,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_full_event_str.wstr_ptr,0,((schedule_full_event_str.wstr_len+1)*sizeof(wchar)));
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len - event_str.wstr_len,schedule_str.wstr_ptr,schedule_str.wstr_len,schedule_str.wstr_len);/*lint !e64*/
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr + schedule_str.wstr_len,event_str.wstr_len,event_str.wstr_ptr,event_str.wstr_len,event_str.wstr_len);/*lint !e64*/
    }

    SCI_MEMSET( full_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);

    if(event_str_ptr != PNULL)
    {    
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
    else
    {
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_str.wstr_ptr,schedule_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
    MMIIDLE_GetIdleTextStyle(&text_style, font_type, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR));

    //get right softkey text length
    full_str.wstr_ptr = full_str_ptr;
    full_str.wstr_len = full_len;
    schedule_full_wstr_len = GUISTR_GetStringWidth(&text_style,&full_str,text_state);
    schedule_wstr_len = GUISTR_GetStringWidth(&text_style,&schedule_str,text_state);
    //CR132520
   // if (!is_dis_suspension && MMI_DIRECTION_HMIDDLE == direction)
   // {
	if ((max_len > schedule_full_wstr_len)&&(MMI_DIRECTION_HMIDDLE == direction))
	{
		dis_point->x = (max_len - schedule_full_wstr_len)/2; 
	}
               
   // }
    
    //display string
    MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_POINT_T *)dis_point,
        font_type,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
        full_str.wstr_ptr,
        full_str.wstr_len
        );

//     MMIIDLE_DisplayIdleText(
//         (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
//         (const GUI_POINT_T *)dis_point,
//         font_type,
//         MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//         schedule_str.wstr_ptr,
//         schedule_str.wstr_len
//         );

//     if(event_str_ptr != PNULL)
//     {
//         dis_point->x = dis_point->x + schedule_wstr_len;
//         MMIIDLE_DisplayIdleText(
//             (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
//             (const GUI_POINT_T *)dis_point,
//             font_type,
//             MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//             event_str_ptr,
//             event_len
//             );
//     }
	//end for NEWMS00222937
    
    if(PNULL!= event_str_ptr) 
    {   
        SCI_FREE(event_str_ptr);
    }

    SCI_FREE(full_str_ptr);
    
    //free memory
    if (is_alloc&&PNULL!= schedule_str.wstr_ptr)
    {
        SCI_FREE(schedule_str.wstr_ptr);
    }
    
    SCI_FREE(schedule_full_event_str.wstr_ptr);
    
    return schedule_wstr_len;    
   
}

PUBLIC uint16 MMIIDLE_DisplayScheduleEx(
                                              GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                              GUI_RECT_T      *rect_ptr,
                                              GUI_FONT_T      font_type
                                             )

{
    BOOLEAN                 is_alloc = FALSE;
    uint8                   time_str[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_wstr[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_str_len = 0;   
    uint16                  event_id = 0;
    uint16                  display_max_len = 0;
    uint16                  max_len = MMITHEME_GetClientRectWidth();
    MMI_STRING_T            name_str = {0};
    MMI_STRING_T            schedule_str = {0};
    MMI_STRING_T            schedule_event_str = {0};
    MMI_STRING_T            schedule_full_event_str = {0};
    MMIACC_SMART_EVENT_T*   schedule_event_ptr = PNULL;
    wchar*                  event_str_ptr = PNULL;
    wchar*                  full_str_ptr = PNULL;//[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    uint16                  full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    uint16                  event_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    BOOLEAN                 is_dis_suspension = FALSE;
    MMI_STRING_T            event_str = {0};
    //MMI_DIRECTION_SET_E     direction = MMI_DIRECTION_LEFT;
    uint16                  wstr_width = 0;
    //get schedule
    if (!MMIAPISCH_OpenEventToday(&event_id))
    {
        //今日无日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOCALENDAR,&schedule_str);
    }
    else if (MMIAPISCH_IsAllPastEventToday(&event_id))
    {
        //今日无更多日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOMORECALENDAR,&schedule_str);
    }
    else
    {
        //日程
        MMI_GetLabelTextByLang(TXT_IDLE_CALENDAR,&name_str);

        //get schedule
        schedule_event_ptr = (MMIACC_SMART_EVENT_T*)SCI_ALLOCA(sizeof(MMIACC_SMART_EVENT_T));
        //SCI_ASSERT(PNULL != schedule_event_ptr);
        if(PNULL == schedule_event_ptr)
        {
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_138_112_2_18_2_24_7_54,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_event_ptr, 0, sizeof(MMIACC_SMART_EVENT_T) );
        MMIAPIALM_FormInteEventInfo(event_id,schedule_event_ptr);

        //timer
		MMIAPISET_FormatTimeStrByTime(
			schedule_event_ptr->event_fast_info.hour,
			schedule_event_ptr->event_fast_info.minute,
			time_str,
			MMIIDLE_TIME_LEN
			);
        time_str_len = strlen((char *)time_str);
        MMI_STRNTOWSTR(time_wstr,MMIIDLE_TIME_LEN,
            time_str,MMIIDLE_TIME_LEN,
            time_str_len);

        //get schedule string len
        schedule_str.wstr_len = name_str.wstr_len + time_str_len +1;
        
        //copy schedule string
        schedule_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_str.wstr_ptr )
        {
            SCI_FREE(schedule_event_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_164_112_2_18_2_24_7_55,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_str.wstr_ptr,0,((schedule_str.wstr_len+1)*sizeof(wchar)));
        
        schedule_event_str.wstr_len = schedule_event_ptr->event_content_info.w_length;
        schedule_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_event_str.wstr_ptr)
        {
            SCI_FREE(schedule_event_ptr);
            SCI_FREE(schedule_str.wstr_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_176_112_2_18_2_24_7_56,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_event_str.wstr_ptr,0,((schedule_event_str.wstr_len+1)*sizeof(wchar)));
        is_alloc = TRUE;
        /*if (MMI_DIRECTION_RIGHT==direction)
        {
            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                time_wstr,time_str_len,
                time_str_len);
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr+time_str_len,schedule_str.wstr_len-time_str_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);
        }
        else*/
        {
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);

            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len,schedule_str.wstr_len-name_str.wstr_len,
                time_wstr,MMIIDLE_TIME_LEN,
                time_str_len);
        }

        MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len+time_str_len,schedule_str.wstr_len-name_str.wstr_len-time_str_len,L" ",1,1);/*lint !e64*/
        //copy event
        MMI_WSTRNCPY(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,
            schedule_event_ptr->event_content_info.w_content,schedule_event_ptr->event_content_info.w_length,
            schedule_event_ptr->event_content_info.w_length);

        event_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        if(PNULL == event_str_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
            SCI_FREE(schedule_event_str.wstr_ptr);
            SCI_FREE(schedule_event_ptr);
          
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == event_str_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_219_112_2_18_2_24_7_57,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET( event_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        MMIIDLE_GetDisPlayString(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,max_len-display_max_len,font_type,event_str_ptr,&event_len);//MARYXIAO CR230653
        SCI_FREE(schedule_event_ptr);
        SCI_FREE(schedule_event_str.wstr_ptr);
    }
    //display string max len
    display_max_len = max_len - rect_ptr->left;
    full_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    //SCI_ASSERT(PNULL != full_str_ptr);
    if(PNULL == full_str_ptr)
    {
        if(PNULL != schedule_str.wstr_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
        }
       
        if(PNULL != event_str_ptr)
        {
            SCI_FREE(event_str_ptr);
        }
        
        //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == full_str_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_244_112_2_18_2_24_7_58,(uint8*)"");
        return 0;
    }
    
    if(event_str_ptr != PNULL)
    {
        event_str.wstr_ptr = event_str_ptr;
        event_str.wstr_len = event_len;
        schedule_full_event_str.wstr_len = schedule_str.wstr_len + event_str.wstr_len;//拼接
        schedule_full_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_full_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_full_event_str.wstr_ptr)
        {
            if(PNULL != schedule_str.wstr_ptr)
            {
                SCI_FREE(schedule_str.wstr_ptr);
            }
 
            SCI_FREE(event_str_ptr);
            
            SCI_FREE(full_str_ptr);
            
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_full_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_265_112_2_18_2_24_7_59,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_full_event_str.wstr_ptr,0,((schedule_full_event_str.wstr_len+1)*sizeof(wchar)));
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len - event_str.wstr_len,schedule_str.wstr_ptr,schedule_str.wstr_len,schedule_str.wstr_len);/*lint !e64*/
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr + schedule_str.wstr_len,event_str.wstr_len,event_str.wstr_ptr,event_str.wstr_len,event_str.wstr_len);/*lint !e64*/
    }

    SCI_MEMSET( full_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);

    if(event_str_ptr != PNULL)
    {    
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
    else
    {
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_str.wstr_ptr,schedule_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
        
    //display string
    wstr_width = MMIIDLE_DisplayLineTextInRect(
            lcd_dev_ptr,
            rect_ptr,
            font_type,
            MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
            full_str_ptr, 
            full_len
            );
    
//	    wstr_width = MMIIDLE_DisplayLineTextInRect(
//	            lcd_dev_ptr,
//	            rect_ptr,
//	            font_type,
//	            MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//	            schedule_str.wstr_ptr, 
//	            schedule_str.wstr_len
//	            );
//	            
//	
//	    if(event_str_ptr != PNULL)
//	    {
//	        rect_ptr->left = rect_ptr->left + wstr_width;
//	 
//	        //display string
//	        wstr_width = MMIIDLE_DisplayLineTextInRect(
//	                lcd_dev_ptr,
//	                rect_ptr,
//	                font_type,
//	                MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//	                event_str_ptr, 
//	                event_len
//	                );
//	    }
	//end for NEWMS00222937
    
    if(PNULL!= event_str_ptr) 
    {   
        SCI_FREE(event_str_ptr);
    }

    SCI_FREE(full_str_ptr);
    
    //free memory
    if (is_alloc&&PNULL!= schedule_str.wstr_ptr)
    {
        SCI_FREE(schedule_str.wstr_ptr);
    }
    
    SCI_FREE(schedule_full_event_str.wstr_ptr);
    
    return wstr_width;    
   
}
#endif
/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            )
{
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"GetIdleTextStyle ERROR! PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_352_112_2_18_2_24_7_60,(uint8*)"");
        return;
    }
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
}

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleText(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             )
{
    GUISTR_STYLE_T  text_style = {0};/*lint !e64*/
    MMI_STRING_T    text = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T      disp_rect = {0};
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          str_pixel_width = 0;

    GUILCD_GetLogicWidthHeight(lcd_dev_ptr->lcd_id, &lcd_width, &lcd_height);

    MMIIDLE_GetIdleTextStyle(&text_style, font, font_color);

    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = lcd_width - 1;
    disp_rect.bottom  = disp_rect.top + GUIFONT_GetHeight(text_style.font) +1;
    
    text.wstr_len = wstr_len;
    text.wstr_ptr = wstr_ptr;
    //display date
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_RECT_T      *)&disp_rect,
        (const GUI_RECT_T      *)&disp_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

    str_pixel_width = GUISTR_GetStringWidth(&text_style,&text,text_state);

    return (str_pixel_width);
}
#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON
/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleTextEx(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len,
                             GUISTR_STATE_T  text_state
                             )
{
    GUISTR_STYLE_T  text_style = {0};/*lint !e64*/
    MMI_STRING_T    text = {0};
    GUI_RECT_T      disp_rect = {0};
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          str_pixel_width = 0;

    GUILCD_GetLogicWidthHeight(lcd_dev_ptr->lcd_id, &lcd_width, &lcd_height);

    GetIdleTextStyle(&text_style, font, font_color);

    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = lcd_width - 1;
    disp_rect.bottom  = disp_rect.top + GUIFONT_GetHeight(text_style.font) +1;
    
    text.wstr_len = wstr_len;
    text.wstr_ptr = wstr_ptr;
    //display date
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_RECT_T      *)&disp_rect,
        (const GUI_RECT_T      *)&disp_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

    str_pixel_width = GUISTR_GetStringWidth(&text_style,&text,text_state);

    return (str_pixel_width);
}
#endif
/*****************************************************************************/
//  Description : get display string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_GetDisPlayString(wchar const * src_str,     //in
                            uint16  src_len,     //in
                            uint16  max_display_len,//in
                            GUI_FONT_T font_type, //font
                            wchar * des_str,     //out
                            uint16* des_len      //out
                            )
{    
    uint16                  wstr_len= 0;
    uint8                   temp_str [MMIIDLE_TIME_LEN] = {0};
    wchar                   temp_wstr[MMIIDLE_TIME_LEN] = {0};
    uint16                  temp_str_len = 0;
    uint16                  temp_wstr_width = 0;
    BOOLEAN                 is_dis_suspension = FALSE;
    uint16                  src_str_len = src_len;
    uint16                  max_des_len = *des_len;

    SCI_MEMSET(des_str,0x00,(sizeof(wchar)*(*des_len)));

    //get suspension points
    sprintf((char *)temp_str,"...");
    temp_str_len = strlen((char *)temp_str);
    MMI_STRNTOWSTR(temp_wstr,MMIIDLE_TIME_LEN,
        temp_str,MMIIDLE_TIME_LEN,
        temp_str_len);

    temp_wstr_width = MMIAPICOM_GetStrWidth(temp_wstr,
                &(temp_str_len),
                0,
                max_display_len,
                font_type);
    wstr_len = src_str_len;
    MMIAPICOM_GetStrWidth(src_str,
                    &(src_str_len),
                    0,
                    max_display_len,
                    font_type);

    //超过一行时,末尾显示"..."的处理
    if (wstr_len > src_str_len)
    {
        is_dis_suspension = TRUE;
        //获得去除"..."后的显示宽度
        MMIAPICOM_GetStrWidth(src_str,
                    &(src_str_len),
                    0,
                    //这个函数计算不是很准确，为了避免显示不全，只有将显示区域减小一点了。
                    (max_display_len - temp_wstr_width-MMIIDLE_GET_STR_DISPLAY_LEN_OFFSET),
                    font_type);
    }
    MMI_WSTRNCPY(des_str,*des_len,src_str,src_str_len,src_str_len);

    *des_len = src_str_len;
    if (is_dis_suspension)
    {
        MMI_WSTRNCPY(des_str+src_str_len,max_des_len-src_str_len,temp_wstr,temp_str_len,temp_str_len);
        *des_len +=temp_str_len;
    }
    return is_dis_suspension;
}

PUBLIC uint16 MMIIDLE_DisplayLineTextInRect(
                              GUI_LCD_DEV_INFO *lcd_dev_ptr,
                              GUI_RECT_T      *rect_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             )
{
    GUISTR_STYLE_T  text_style = {0};/*lint !e64*/
    MMI_STRING_T    text = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T      disp_rect = *rect_ptr;
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          str_pixel_width = 0;

    GUILCD_GetLogicWidthHeight(lcd_dev_ptr->lcd_id, &lcd_width, &lcd_height);

    MMIIDLE_GetIdleTextStyle(&text_style, font, font_color);
    text_style.align = ALIGN_LVMIDDLE;

    text.wstr_len = wstr_len;
    text.wstr_ptr = wstr_ptr;
    //display date
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_RECT_T      *)&disp_rect,
        (const GUI_RECT_T      *)&disp_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

    str_pixel_width = GUISTR_GetStringWidth(&text_style,&text,text_state);

    return (str_pixel_width);
}

#endif //defined MMI_SMART_IDLE_SUPPORT || defined MMI_COMMON_IDLE_SUPPORT
/*****************************************************************************/
//  Description : init win Info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_InitWinInfo(void)
{
    SCI_MEMSET(((void*)&s_idlewin_info), 0, sizeof(IDLE_WIN_INFO_T));
}
/*****************************************************************************/
//  Description : get idle info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC IDLE_WIN_INFO_T* MMIIDLE_GetIdleWinInfo(void)
{
    return &s_idlewin_info;
}
/*****************************************************************************/
//  Description : display the main and sub idle soft icon 
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_SendMsgToIdleWin(
                               MMI_MESSAGE_ID_E msg_id,
                               DPARAM            param_ptr
                               )
{
    if (MMIAPIIDLE_IdleWinIsOpen() && MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        return MMK_SendMsg(MAIN_IDLE_WIN_ID, msg_id, param_ptr);
    }
    else
    {
        return (FALSE);
    }
}
/*****************************************************************************/
//  Description : need show soft icon and network name according to setting
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(MN_DUAL_SYS_E dual_sys)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_set = MMIAPISET_GetMultiSysSetting();

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"NeedShowIconAccordingToDualSysSetting ERROR! MMI_DUAL_SYS_MAX <= dual_sys,dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_567_112_2_18_2_24_7_61,(uint8*)"d",dual_sys);
        return FALSE;
    }
    if (e_dualsys_set & (0x01 << dual_sys))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            )
{
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"GetIdleTextStyle ERROR! PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_592_112_2_18_2_24_8_62,(uint8*)"");
        return;
    }

    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
}
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_DrawStkIdleModeText(uint32 block_id)
{
    return DrawStkIdleModeText(block_id);
}

/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DrawStkIdleModeText(uint32 block_id)
{
    MMI_STRING_T idle_text = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_IDLE_DISPLAY_T idle_disp_style = {0};
    GUI_COLOR_T text_color = 0;
    GUI_FONT_T text_font = SONG_FONT_8;
    GUISTR_STYLE_T text_style = {0};/*lint !e64*/
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
    GUI_RECT_T disp_rect = {0};
    GUI_POINT_T point = {0};
    BOOLEAN is_display = FALSE;
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;
    BOOLEAN is_icon_exist = FALSE;
    GUIIMG_BITMAP_T *icon_bmp_ptr = PNULL;
    BOOLEAN is_self_explanatory = FALSE;
#ifdef WIN32
	wchar test_stk[] ={'a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','\0'};
    MMI_STRING_T test_stk_str ={0};
	s_idle_stk_text.stk_idle_str.wstr_ptr = test_stk;
	s_idle_stk_text.stk_idle_str.wstr_len = 168;
	s_idlewin_info.is_display_stk_idle_text = TRUE;
	s_idle_stk_text.is_scroll = TRUE;
#endif
    idle_style = MMIAPISET_GetIdleStyle();

    lcd_dev_info.block_id = block_id;

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_STK_IDLE_MODE);

    text_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_STK);
    text_font = idle_disp_style.text_font;

    disp_rect.left = idle_disp_style.text_frame_x;
    disp_rect.top = idle_disp_style.text_frame_top;
    disp_rect.right = disp_rect.left + MMI_MAINSCREEN_IDLE_STK_WIDTH - 1;
    disp_rect.bottom = disp_rect.top + GUI_GetFontHeight(text_font, UNICODE_HANZI)+1 ;

    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    if (s_idlewin_info.is_display_stk_idle_text
        && PNULL != s_idle_stk_text.stk_idle_str.wstr_ptr
        && 0 != s_idle_stk_text.stk_idle_str.wstr_len)
    {
        //display icon
        icon_bmp_ptr = (GUIIMG_BITMAP_T *)MMIAPISTK_GetIdleTextIcon(&is_icon_exist, &is_self_explanatory);

        if (is_icon_exist && PNULL != icon_bmp_ptr && PNULL != icon_bmp_ptr->bit_ptr)//??Icon
        {
            GUI_POINT_T dis_point = {0};
            GUI_RECT_T show_rect = {0};

            if(point.x < (icon_bmp_ptr->img_width + MMI_IDLE_SIM_ICON_INTERVAL))
            {
                point.x = icon_bmp_ptr->img_height + MMI_IDLE_SIM_ICON_INTERVAL;
            }

            dis_point.x = point.x - icon_bmp_ptr->img_width;
            dis_point.y = point.y;

            show_rect.left = dis_point.x;
            show_rect.top = dis_point.y;
            show_rect.right = dis_point.x + icon_bmp_ptr->img_width;
            show_rect.bottom = dis_point.y + icon_bmp_ptr->img_height;

            GUIIMG_DisplayBmp(FALSE, &show_rect, &dis_point, icon_bmp_ptr, &lcd_dev_info);
        }

        if (!is_icon_exist || !is_self_explanatory)
        {
            if (s_idle_stk_text.str_index > s_idle_stk_text.stk_idle_str.wstr_len - 1)
            {
                s_idle_stk_text.str_index = 0;
            }

            idle_text.wstr_ptr = s_idle_stk_text.stk_idle_str.wstr_ptr + s_idle_stk_text.str_index;
            idle_text.wstr_len = s_idle_stk_text.stk_idle_str.wstr_len - s_idle_stk_text.str_index;
            
            GetIdleTextStyle(&text_style, text_font, text_color);

            if (s_idle_stk_text.is_scroll)
            {
                MAIN_StopIdleSTKTextTimer();
                MAIN_StartIdleSTKTextTimer();
            }
            else
            {
                text_style.align = ALIGN_HMIDDLE;
            }

            GUISTR_DrawTextToLCDInRect( 
                                                            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                                            (const GUI_RECT_T *)&disp_rect,
                                                            (const GUI_RECT_T *)&disp_rect,
                                                            (const MMI_STRING_T *)&idle_text,
                                                            &text_style,
                                                            text_state,
                                                            GUISTR_TEXT_DIR_AUTO
                                                            );

            if (s_idle_stk_text.is_scroll && s_idle_stk_text.str_index < s_idle_stk_text.stk_idle_str.wstr_len - 1)
            {
                s_idle_stk_text.str_index ++;
            }
            else
            {
                s_idle_stk_text.str_index = 0;
            }
        }

        is_display = TRUE;
    }

    return is_display;
}

/*****************************************************************************/
//  Description : start stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StartIdleSTKTextTimer(void)
{
    BOOLEAN result = FALSE;

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        if ((0 == s_idle_stk_text.idle_display_timer) &&
            (s_idle_stk_text.is_scroll))
        {
            s_idle_stk_text.idle_display_timer = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, IDLE_STK_TIMER_PERIOD, FALSE);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : stop stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StopIdleSTKTextTimer(void)
{
    BOOLEAN result = FALSE;

    if (0 != s_idle_stk_text.idle_display_timer)
    {
        MMK_StopTimer(s_idle_stk_text.idle_display_timer);
        s_idle_stk_text.idle_display_timer = 0;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get stk text info
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_STK_TEXT_INFO* MMIIDLECOM_GetStkTextInfo(void)
{
    return &s_idle_stk_text;
}

/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_MAIN_SetIdleModeText(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *stk_idle_str)
{
    MMI_IDLE_DISPLAY_T idle_disp_style = {0};
    
    if (MMI_DUAL_SYS_MAX<=dual_sys || PNULL == stk_idle_str)
    {
        //SCI_TRACE_LOW:"MMIAPIIDLE_MAIN_SetIdleModeText ERROR! dual_sys =%d, PNULL == stk_idle_str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_785_112_2_18_2_24_8_63,(uint8*)"d",dual_sys);
        return;
    }
    //SCI_TRACE_LOW:"MMIAPIIDLE_MAIN_SetIdleModeText dual_sys=%d, len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_788_112_2_18_2_24_8_64,(uint8*)"dd", dual_sys, stk_idle_str->wstr_len);

    MAIN_StopIdleSTKTextTimer();

    MMIIDLE_GetIdleWinInfo()->is_display_stk_idle_text = FALSE;
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_STK_IDLE_TEXT);
#endif

    s_idle_stk_text.is_scroll = FALSE;
    s_idle_stk_text.str_index = 0;

    //free memory
    if (PNULL != s_idle_stk_text.stk_idle_str.wstr_ptr)
    {
        SCI_FREE(s_idle_stk_text.stk_idle_str.wstr_ptr);
        s_idle_stk_text.stk_idle_str.wstr_ptr = PNULL;
    }

    if (0 != stk_idle_str->wstr_len)
    {
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_STK_IDLE_TEXT);
#endif

        s_idle_stk_text.dual_sys = dual_sys;
        MMIIDLE_GetIdleWinInfo()->is_display_stk_idle_text = TRUE;
        s_idle_stk_text.stk_idle_str.wstr_len = stk_idle_str->wstr_len;

        s_idle_stk_text.stk_idle_str.wstr_ptr = SCI_ALLOC_APP((s_idle_stk_text.stk_idle_str.wstr_len + 1) * sizeof(wchar));
        SCI_MEMSET(s_idle_stk_text.stk_idle_str.wstr_ptr, 0x00, ((s_idle_stk_text.stk_idle_str.wstr_len + 1) * sizeof(wchar)));
        MMI_WSTRNCPY(s_idle_stk_text.stk_idle_str.wstr_ptr,
                                s_idle_stk_text.stk_idle_str.wstr_len,
                                stk_idle_str->wstr_ptr,
                                stk_idle_str->wstr_len,
                                stk_idle_str->wstr_len);

        idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_STK_IDLE_MODE);

        s_idle_stk_text.str_width = GUI_CalculateStringPiexlNum(s_idle_stk_text.stk_idle_str.wstr_ptr,
                                                                                            s_idle_stk_text.stk_idle_str.wstr_len,
                                                                                            idle_disp_style.text_font,
                                                                                            0);

        if (MMI_MAINSCREEN_IDLE_STK_WIDTH < s_idle_stk_text.str_width)
        {
            s_idle_stk_text.is_scroll = TRUE;
        }
        else
        {
            s_idle_stk_text.is_scroll = FALSE;
        }
    }

    //inform idle win display text
    if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_STK_SET_UP_IDLE_MODE, PNULL, 0);
    }
}
#endif
#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description :to handle STK Idle Text Win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStkIdleTextWinMsg(
                                                                              MMI_WIN_ID_T win_id, 
                                                                              MMI_MESSAGE_ID_E msg_id, 
                                                                              DPARAM param
                                                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIIDLE_STK_IDLE_TEXT_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);//not return
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);    
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT
        {
            uint32 src_id = ((MMI_NOTIFY_T *)param)->src_id;

            if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == src_id)
            {
                MMK_CloseWin(win_id);

                break;
            }
        }
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :Create Stk Idle Text Win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CreateStkIdleTextWin(void)
{
    MMI_STRING_T title = {0};
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T create = {0};
    GUIRICHTEXT_INIT_DATA_T init_data = {0};
    //void* ctrl_ptr = PNULL;
    MMI_HANDLE_T win_id = MMIIDLE_STK_IDLE_TEXT_WIN_ID;
    BOOLEAN is_exist_stk_name = FALSE;
    BOOLEAN is_icon_exist = FALSE;
    GUIIMG_BITMAP_T *icon_bmp_ptr = PNULL;
    BOOLEAN is_self_explanatory = FALSE;

    MMK_CloseWin(win_id);

    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = win_id;
    win_create.func = HandleStkIdleTextWinMsg;
    win_create.win_priority = WIN_ONE_LEVEL;
    
    MMK_CreateWindow(&win_create);

#ifdef MMI_PDA_SUPPORT
    GUIWIN_CreateStbDyn(win_id);
#endif

    {
        //create softkey
        GUISOFTKEY_INIT_DATA_T softkey_data = {0};
        MMI_CONTROL_CREATE_T softkey_create = {0};
        MMI_CTRL_ID_T softkey_ctrl_id = 0;

#ifdef MMI_PDA_SUPPORT
        softkey_data.leftsoft_id = TXT_NULL;
        softkey_data.midsoft_id = STXT_EXIT;
        softkey_data.rightsoft_id = TXT_NULL;
#else
        softkey_data.leftsoft_id = TXT_NULL;
        softkey_data.midsoft_id = TXT_NULL;
        softkey_data.rightsoft_id = STXT_RETURN;
#endif
        softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
        softkey_create.ctrl_id = softkey_ctrl_id;
        softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
        softkey_create.parent_win_handle = win_id;
        softkey_create.init_data_ptr = &softkey_data;

#ifdef MMI_PDA_SUPPORT
        MMK_SetWinDisplayStyleState( win_id, WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE, TRUE );
#endif

        MMK_CreateControl( &softkey_create );
        MMK_SetWinSoftkeyCtrlId(win_id, softkey_ctrl_id);
    } 
    
    //set title
    GUIWIN_CreateTitleDyna(win_id, TXT_NULL);

    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);

    create.ctrl_id = MMIIDLE_STK_IDLE_TEXT_CTRL_ID;
    create.guid = SPRD_GUI_RICHTEXT_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;
    //ctrl_ptr = 
    MMK_CreateControl( &create );

    //Set title
    is_exist_stk_name = MMIAPISTK_GetStkName(s_idle_stk_text.dual_sys, &title);

    if (!is_exist_stk_name)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_MAIN_STK, &title);
    }
    
    GUIWIN_SetTitleText(win_id, title.wstr_ptr, title.wstr_len, FALSE);


    icon_bmp_ptr = (GUIIMG_BITMAP_T *)MMIAPISTK_GetIdleTextIcon(&is_icon_exist, &is_self_explanatory);

    if (is_icon_exist && PNULL != icon_bmp_ptr && PNULL != icon_bmp_ptr->bit_ptr)//Icon
    {
        item_data.text_type = GUIRICHTEXT_TEXT_NONE;
        item_data.img_type = GUIRICHTEXT_IMAGE_BUF;
        item_data.is_bit_map = TRUE;
        item_data.img_data.buf.buf_ptr = (uint8 *)icon_bmp_ptr->bit_ptr;
        item_data.img_data.buf.size = icon_bmp_ptr->img_width * icon_bmp_ptr->img_height * 2;
        item_data.img_data.buf.width = icon_bmp_ptr->img_width;
        item_data.img_data.buf.height = icon_bmp_ptr->img_height;
        item_data.img_data.buf.height = icon_bmp_ptr->img_height;
        item_data.img_set_size = TRUE;
        item_data.img_setting_width = icon_bmp_ptr->img_width;
        item_data.img_setting_height = icon_bmp_ptr->img_height;
        item_data.img_set_align = TRUE;
        item_data.img_setting_align = GUIRICHTEXT_IMG_ALIGN_MIDDLE;

        GUIRICHTEXT_AddItem(
            MMIIDLE_STK_IDLE_TEXT_CTRL_ID,
            &item_data,
            &index
            );
    }

    if (!is_icon_exist || !is_self_explanatory)
    {
        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.text_data.buf.str_ptr = s_idle_stk_text.stk_idle_str.wstr_ptr;
        item_data.text_data.buf.len = s_idle_stk_text.stk_idle_str.wstr_len;

        GUIRICHTEXT_AddItem(
            MMIIDLE_STK_IDLE_TEXT_CTRL_ID,
            &item_data,
            &index
            );
    }

    return TRUE;
}
#endif



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
