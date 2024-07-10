/*****************************************************************************
** File Name:      mmiwidget_autoanswer_log.c                                *
** Author:                                                                   *
** Date:           2010.11                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010.11        cheng.luo              Creat
******************************************************************************/

#define _MMIWIDGET_AUTOANSWER_LOG_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"

#if (defined MMIWIDGET_SUPPORT) 
#include "mmi_app_widget_trc.h"
#ifdef ASP_SUPPORT

#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "guibutton.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiwidget_position.h"

#include "mmiasp_export.h"
#include "mmisms_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIDGET_AUTOANSWER_LOG_TEXT_FONT          SONG_FONT_12

#define MMIWIDGET_AUTOANSWER_LOG_TEXT_COLOR         MMI_BLACK_COLOR

#define MMIWIDGET_AUTOANSWER_LOG_TIME_LEN           9

#define MMIWIDGET_AUTOANSWER_LOG_DATE_LEN           11

#define MMIWIDGET_NEW_MSG_MAX_BYTE                  6

#define MMIWIDGET_ONLY_ONE_RECORD                   1

#define MMIASP_MAX_NAME_LEN                         (((MMIPB_MAX_STRING_LEN)+1)*2)

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//点中区域的标识
typedef enum
{
    MMIWIDGET_AUTOANSWER_LOG_HIT_NONE,        
    MMIWIDGET_AUTOANSWER_LOG_HIT_OPEN,       
    MMIWIDGET_AUTOANSWER_LOG_HIT_CANCEL,      
    MMIWIDGET_AUTOANSWER_LOG_HIT_MAX
}MMIWIDGET_AUTOANSWER_LOG_HIT_TYPE_E;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint32 s_widget_autoanswer_log_hit_type = MMIWIDGET_AUTOANSWER_LOG_HIT_NONE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle tp press down
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoAnswerLogTpPressDown(
                                                  MMI_WIN_ID_T win_id, 
                                                  DPARAM param
                                                  );

/*****************************************************************************/
//  Description : handle tp press up
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoAnswerLogTpPressUp(
                                                MMI_WIN_ID_T win_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//  Description : display auto answer log widget
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_DisplayAutoAnswerLogPanel(
                                               MMI_WIN_ID_T     win_id
                                               );

/*****************************************************************************/
//  Description : handle widget message
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetAutoAnswerLogWinMsg(
                                                    MMI_WIN_ID_T        win_id,        //IN:
                                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                                    DPARAM              param          //IN:
                                                    );

/*****************************************************************************/
//  Description : widget callback (*DisplayWidgetCondition) 
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DisplayAutoAnswerLogValid(void);

/*****************************************************************************/
//  Description : widget callback (*ResetChildWinItem)
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void  ResetWidgetAutoAnswerLog(void);


PUBLIC const MMIWIDGET_ITEM_INFO_T g_autoanswer_log_widget =
{
        MMIWIDGET_AUTOANSWER_LOG_ID, 
		HandleWidgetAutoAnswerLogWinMsg, 
		DisplayAutoAnswerLogValid,
		ResetWidgetAutoAnswerLog,
		PNULL,
		WIDGET_AUTOANSWER_LOG_WIN_ID, 
		IMAGE_WIDGET_AUTOANSWER_LOG_SHORTCUT, 
		IMAGE_WIDGET_AUTOANSWER_LOG_BG, 
        TXT_WIDGET_ASP,		
		80,
		80,
		TRUE,
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle widget message
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetAutoAnswerLogWinMsg(
                                                    MMI_WIN_ID_T        win_id,      //IN:
                                                    MMI_MESSAGE_ID_E    msg_id,      //IN:
                                                    DPARAM              param        //IN:
                                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;	
    
    switch (msg_id)
    {     
    case MSG_OPEN_WINDOW:
        break;
    case MSG_WIDGET_TP_PRESS_DOWN:
        recode = HandleAutoAnswerLogTpPressDown(win_id, param);
        break;			
        
    case MSG_WIDGET_TP_PRESS_UP:
        recode = HandleAutoAnswerLogTpPressUp(win_id, param);
        break;
        
    case MSG_FULL_PAINT:			
        MMIWIDGET_DisplayAutoAnswerLogPanel(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;    
}

/*****************************************************************************/
//  Description : widget callback (*DisplayWidgetCondition) 
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DisplayAutoAnswerLogValid()
{
    if (0 < MMIAPIASP_GetNewRecordNum())
    {
        return MMI_RESULT_TRUE;
    }
    
    return MMI_RESULT_FALSE;
}

/*****************************************************************************/
//  Description : widget callback (*ResetChildWinItem)
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void  ResetWidgetAutoAnswerLog()
{
    s_widget_autoanswer_log_hit_type = MMIWIDGET_AUTOANSWER_LOG_HIT_NONE;
}

/*****************************************************************************/
//  Description : get widget display text style
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_GetAutoAnswerLogTextStyle(
                                               GUISTR_STYLE_T *style_ptr,
                                               GUI_FONT_T     font,
                                               GUI_COLOR_T    font_color,
                                               GUI_ALIGN_E	  font_align									   
                                               )
{
    if (PNULL != style_ptr)
    {
        style_ptr->effect 	= FONT_EFFECT_REVERSE_EDGE; 
        style_ptr->angle 	= ANGLE_0;
        style_ptr->align 	= font_align;
        style_ptr->font 	= font;
        style_ptr->font_color = font_color;
    }
    
}

/*****************************************************************************/
//  Description : display the record detail
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void  MMIWIDGET_DisplayAutoAnswerLogDetail(
                                                 MMI_WIN_ID_T win_id,
                                                 const GUI_LCD_DEV_INFO *dev_info_ptr,
                                                 GUI_POINT_T start_point
                                                 )
{
    MMI_STRING_T 	string_prefix = {0};
    GUISTR_INFO_T 	str_info = {0};
    GUI_POINT_T  	point = start_point;
    GUI_POINT_T  	lcd_point ={0};
    GUI_POINT_T  	temp_point ={0};
    GUI_RECT_T      name_rect = {0};	 	  
    GUISTR_STYLE_T 	text_style = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;	
    
    wchar           name_arr[MMIASP_MAX_NAME_LEN+1]  = {0};
    MMI_STRING_T    name_str                         = {0};
    uint8           time_arr[MMIASP_MAX_NAME_LEN+1] = {0};
    uint8           date_arr[MMIASP_MAX_NAME_LEN+1] = {0};
    wchar           date_wstr[MMIASP_MAX_NAME_LEN+1] = {0};
    wchar           time_wstr[MMIASP_MAX_NAME_LEN+1] = {0};
    //uint16          node_index = 0;
    MMIASP_MSG_INFO_T   record_info = {0};
    
    MMI_STRING_T    time_str = {0};
    MMI_STRING_T    date_str = {0};
    
    name_str.wstr_ptr = name_arr;
    name_str.wstr_len = 0;
    
    MMIWIDGET_GetAutoAnswerLogTextStyle(&text_style, 
        MMIWIDGET_AUTOANSWER_LOG_TEXT_FONT, 
        MMIWIDGET_AUTOANSWER_LOG_TEXT_COLOR, 
        ALIGN_LEFT);	  

    MMI_GetLabelTextByLang(TXT_COMMON_DETAIL_FROM, &string_prefix);
    
    lcd_point = MMI_ConvertWinPoint(MMICOM_WINPOS_WIN2DISP, win_id, point);	
    //记录此时屏的位置，为时间标题标题对齐
    temp_point =  lcd_point;
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)dev_info_ptr,
        (const GUI_POINT_T      *)&lcd_point,
        &string_prefix,
        &text_style,
        text_state
        );
    
    if (MMIAPIASP_GetLastestUnreadRecord(&record_info ,PNULL))
    {
        // get name
        if(MMIAPIASP_GetNameByNumber(record_info.call_num, &name_str))
        { 
            //SCI_TRACE_LOW:"MMIASP this number has find a name "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_AUTOANSWER_LOG_283_112_2_18_3_7_53_174,(uint8*)"");
        }
        else
        {
            //MMIAPICOM_StrToWstr(record_info.call_num, name_str.wstr_ptr, record_info.call_num, );
            MMI_STRNTOWSTR((wchar*)name_str.wstr_ptr, MMIASP_MAX_NAME_LEN, 
                record_info.call_num, MMIPB_MAX_STRING_LEN, 
                strlen((char*)record_info.call_num));
        }
        name_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(name_str.wstr_ptr);
        
        GUISTR_GetStringInfo(&text_style, &string_prefix, text_state, &str_info);		
        
        name_rect.top = point.y;
        name_rect.left = point.x + str_info.width;
        GUIRES_GetImgWidthHeight(&name_rect.right, PNULL, /*lint !e64*/
            IMAGE_WIDGET_AUTOANSWER_LOG_BG, win_id);
        name_rect.right = name_rect.right - MMIWIDGET_AUTOANSWER_LOG_LEFT_SPACE;
        name_rect.bottom = name_rect.top + str_info.height;       
        name_rect=  MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, name_rect);

        //display name
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)dev_info_ptr,
            &name_rect,
            &name_rect,
            &name_str,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        // get date time
        MMIAPISET_FormatDateStrByDateStyle(record_info.date_info.sys_date.year, 
            record_info.date_info.sys_date.mon, 
            record_info.date_info.sys_date.mday, 
            '/', date_arr, MMIWIDGET_AUTOANSWER_LOG_DATE_LEN + 1);
        //MMIAPICOM_StrToWstr(date, date_wstr);
        MMI_STRNTOWSTR((wchar*)date_wstr, MMIASP_MAX_NAME_LEN, date_arr, MMIASP_MAX_NAME_LEN, strlen((char*)date_arr));
        date_str.wstr_ptr = date_wstr;
        date_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(date_wstr);
        
        MMIAPISET_FormatTimeStrByTime(record_info.date_info.sys_time.hour, 
            record_info.date_info.sys_time.min,
            time_arr, 
            MMIWIDGET_AUTOANSWER_LOG_TIME_LEN +1);
        //MMIAPICOM_StrToWstr(time, time_wstr);
        MMI_STRNTOWSTR(time_wstr, MMIASP_MAX_NAME_LEN, time_arr, MMIASP_MAX_NAME_LEN, strlen((char*)time_arr));
        time_str.wstr_ptr = time_wstr;
        time_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(time_wstr);
        
        lcd_point.y = lcd_point.y + str_info.height + MMIWIDGET_AUTOANSWER_LOG_LINE_SPACE;
        lcd_point.x = temp_point.x;
        MMI_GetLabelTextByLang(TXT_COMMON_TIME, &string_prefix);  
        
        // display prefix
        GUISTR_DrawTextToLCDSingleLine(
            (const GUI_LCD_DEV_INFO *)dev_info_ptr,
            (const GUI_POINT_T      *)&lcd_point,
            &string_prefix,
            &text_style,
            text_state
            );	
        
        GUISTR_GetStringInfo(&text_style, &string_prefix, text_state, &str_info);	
        lcd_point.x  += str_info.width;		 
        
        // display data
        GUISTR_DrawTextToLCDSingleLine(
            (const GUI_LCD_DEV_INFO *)dev_info_ptr,
            (const GUI_POINT_T      *)&lcd_point,
            &date_str,
            &text_style,
            text_state
            );

        GUISTR_GetStringInfo(&text_style, &date_str, text_state,&str_info);	
        lcd_point.y  += str_info.height +MMIWIDGET_AUTOANSWER_LOG_LINE_SPACE;
        // display time
        GUISTR_DrawTextToLCDSingleLine(
            (const GUI_LCD_DEV_INFO *)dev_info_ptr,
            (const GUI_POINT_T      *)&lcd_point,
            &time_str,
            &text_style,
            text_state
            );			
    }
}

/*****************************************************************************/
//  Description : display auto answer log widget
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_DisplayAutoAnswerLogPanel(
                                               MMI_WIN_ID_T     win_id
                                               )
{
    GUI_RECT_T win_rect         = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_POINT_T     point       = {0};
    GUISTR_STYLE_T  text_style  = {0};
    GUI_RECT_T      text_rect   = {0};	
    GUI_RECT_T open_rect = MMIWIDGET_AUTOANSWER_LOG_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_AUTOANSWER_LOG_CANCEL_RECT;
    
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 new_msg_num[MMIWIDGET_NEW_MSG_MAX_BYTE] = {0};
    wchar disp_num[MMIWIDGET_NEW_MSG_MAX_BYTE * 2] = {0};	
    MMI_STRING_T prompt_str = {0};	
    MMI_STRING_T tmp_prompt = {0};
    
    GUISTR_INFO_T    str_info={0};
    GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;	
    
    uint32 new_msg_count = MMIAPIASP_GetNewRecordNum();
    
    if (0 == new_msg_count)
    {
        return;
    }
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);  
    
    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        //显示背景图
        point.x = win_rect.left;
        point.y = win_rect.top;
        
        GUIRES_DisplayImg(
            &point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WIDGET_AUTOANSWER_LOG_BG,
            &lcd_dev_info
            ); 
        
        open_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, open_rect);
        canecl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, canecl_rect);
        GUIRES_DisplayImg(
            PNULL,
            &open_rect,
            PNULL,
            win_id,
            IMAGE_WIDGET_AUTOANSWER_LOG_LEFT_BUTTON_OFF,
            &lcd_dev_info
            );	 
        GUIRES_DisplayImg(
            PNULL,
            &canecl_rect,
            PNULL,
            win_id,
            IMAGE_WIDGET_CALLLOG_RIGHT_BUTTON_OFF,
            &lcd_dev_info
            );	
        //TP down 后图片的更换			
        switch (s_widget_autoanswer_log_hit_type)
        {
        case MMIWIDGET_AUTOANSWER_LOG_HIT_OPEN:
            GUIRES_DisplayImg(
                PNULL,
                &open_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_AUTOANSWER_LOG_LEFT_BUTTON_ON,
                &lcd_dev_info
                );
            break;
        case MMIWIDGET_AUTOANSWER_LOG_HIT_CANCEL:
            GUIRES_DisplayImg(
                PNULL,
                &canecl_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_AUTOANSWER_LOG_RIGHT_BUTTON_ON,
                &lcd_dev_info
                );
            break;
        default:
            break;			
        }
        
        // text
        MMIWIDGET_GetAutoAnswerLogTextStyle(&text_style, 
            MMIWIDGET_AUTOANSWER_LOG_TEXT_FONT, 
            MMITHEME_GetDefaultWidgetThemeColor(),
            ALIGN_HVMIDDLE);
        // open
        MMI_GetLabelTextByLang(TXT_COMM_OPEN, &tmp_prompt);		
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &open_rect,
            &open_rect,
            &tmp_prompt,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO); 
        
        // cancel
        MMI_GetLabelTextByLang(STXT_CANCEL, &tmp_prompt);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &canecl_rect,
            &canecl_rect,
            &tmp_prompt,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO);
        
        //显示count		
        MMIWIDGET_GetAutoAnswerLogTextStyle(&text_style,
            MMIWIDGET_AUTOANSWER_LOG_TEXT_FONT, 
            MMIWIDGET_AUTOANSWER_LOG_TEXT_COLOR, 
            ALIGN_HVMIDDLE);
        disp_num_len= sprintf((char *)new_msg_num, "%d", new_msg_count);
        MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8*)new_msg_num, disp_num_len, disp_num_len);
        disp_str_len =  disp_num_len + 1;
        
        // allocate memory
        prompt_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
        SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        MMI_WSTRNCPY(prompt_str.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);		
        prompt_str.wstr_len = disp_num_len;
        
        GUISTR_GetStringInfo(&text_style, &prompt_str, text_state,&str_info);
        text_rect.left = MMIWIDGET_AUTOANSWER_LOG_LEFT_SPACE;
        text_rect.top = MMIWIDGET_AUTOANSWER_LOG_TOP_SPACE;
        text_rect.right = text_rect.left + str_info.width - 1;
        text_rect.bottom = text_rect.top + str_info.height - 1;
        text_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, text_rect);			
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &prompt_str,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            ); 
		if (new_msg_count > 1)
		{
			MMI_GetLabelTextByLang(TXT_MMIASP_NEW_MULT_MSGS, &tmp_prompt);
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_MMIASP_NEW_MSG, &tmp_prompt);
		}
        GUISTR_GetStringInfo(&text_style, &tmp_prompt, text_state, &str_info);	
        text_rect.top = text_rect.bottom + MMIWIDGET_AUTOANSWER_LOG_LINE_SPACE;	  
        text_rect.right = text_rect.left + str_info.width -1;	 
        text_rect.bottom = text_rect.top + str_info.height -1;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &tmp_prompt,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        point.x =  text_rect.left;
        point.y =   text_rect.bottom + MMIWIDGET_AUTOANSWER_LOG_SENTENCE_SPACE;
        point = MMI_ConvertWinPoint(MMICOM_WINPOS_DISP2WIN, win_id, point);
        
        //获取最新一条记录的信息
        MMIWIDGET_DisplayAutoAnswerLogDetail(win_id, &lcd_dev_info, point);
    }
    
    if (PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr = PNULL;
    }	
}

/*****************************************************************************/
//  Description : handle tp press down
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutoAnswerLogTpPressDown(
                                                   MMI_WIN_ID_T      win_id,      //IN:
                                                   DPARAM            param        //IN:
                                                   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T open_rect = MMIWIDGET_AUTOANSWER_LOG_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_AUTOANSWER_LOG_CANCEL_RECT;
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if (GUI_PointIsInRect(point, open_rect))
    {
        s_widget_autoanswer_log_hit_type = MMIWIDGET_AUTOANSWER_LOG_HIT_OPEN;
    }
    else if (GUI_PointIsInRect(point, canecl_rect))
    {
        s_widget_autoanswer_log_hit_type = MMIWIDGET_AUTOANSWER_LOG_HIT_CANCEL;
    }
    else
    {
        s_widget_autoanswer_log_hit_type = MMIWIDGET_AUTOANSWER_LOG_HIT_NONE;
        recode = MMI_RESULT_FALSE;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle tp press up
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoAnswerLogTpPressUp(
                                                MMI_WIN_ID_T win_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_RECT_T open_rect = MMIWIDGET_AUTOANSWER_LOG_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_AUTOANSWER_LOG_CANCEL_RECT;
    GUI_POINT_T point = {0};
    uint16 node_index = 0;
    uint16 new_msg_count = MMIAPIASP_GetNewRecordNum();
    
    if (0 == new_msg_count)
    {
        return MMI_RESULT_FALSE;
    }
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if (GUI_PointIsInRect(point, open_rect))
    {
        if (MMIWIDGET_ONLY_ONE_RECORD != new_msg_count)
        {
            recode = MMIAPIASP_EnterAsp();
        }
        else
        {
            if (MMIAPIASP_GetLastestUnreadRecord(PNULL, &node_index))
            {
                recode = MMIAPIASP_OpenPlayWinFromExternal(node_index);
            }
        }
    }
    else if(GUI_PointIsInRect(point, canecl_rect))
    {
        if (MMIWIDGET_AUTOANSWER_LOG_HIT_CANCEL == s_widget_autoanswer_log_hit_type)
        {
            MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_AUTOANSWER_LOG_ID);
        }			
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }
    
    s_widget_autoanswer_log_hit_type = MMIWIDGET_AUTOANSWER_LOG_HIT_NONE;
    
    return recode;
}

#endif
#endif
