/*****************************************************************************
** File Name:      mmiwidget_mp3.c                                        *
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

#define _MMIWIDGET_NEWSMS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#if (defined MMIWIDGET_SUPPORT)

#include "mmi_app_widget_trc.h"
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

#include "mmisms_text.h"
//#include "Mmiset.h"
#include "mmiset_export.h"
#include "Mmisms_api.h"
#include "mmisms_id.h"
#include "mmisms_export.h"
#include "Mmisms_app.h"
#include "mmisms_read.h"
//#include "mmisms_internal.h"
#include "mmiwidget_position.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/



#define MMIWIDGET_NEWSMS_TEXT_FONT          SONG_FONT_12

#define MMIWIDGET_NEWSMS_TEXT_COLOR         MMI_BLACK_COLOR



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
//点中区域的标识
typedef enum
{
    MMIWIDGET_NEWSMS_HIT_NONE,        
    MMIWIDGET_NEWSMS_HIT_OPEN,       
    MMIWIDGET_NEWSMS_HIT_CANCEL,      
    MMIWIDGET_NEWSMS_HIT_MAX
}MMIWIDGET_NEWSMS_HIT_TYPE_E;
//点中区域的标识
LOCAL uint32 s_widget_newsms_hit_type = MMIWIDGET_NEWSMS_HIT_NONE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E  HandleWidgetNewSmsWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   );
LOCAL void MMIWIDGET_NewSms_DisplayNewSmsPanel(
										   MMI_WIN_ID_T     win_id
										      );
LOCAL void MMIWIDGET_NewSms_GetNewSmsTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color,
                                                                         GUI_ALIGN_E	font_align									   
										   );
LOCAL MMI_RESULT_E HandleNewSmsTpPressUp(MMI_WIN_ID_T win_id, DPARAM param);
LOCAL MMI_RESULT_E  HandleNewSmsTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
LOCAL void  MMIWIDGET_NewSms_DisplaySmsDetail(MMI_WIN_ID_T win_id,
	                                                                              const GUI_LCD_DEV_INFO *dev_info_ptr,
	                                                                              GUI_POINT_T start_point); //相对坐标

LOCAL uint16 GetNewSmsNum(void);

LOCAL MMI_RESULT_E  DisplayNewSmsValid(void);

LOCAL void MMIWIDGET_NewSms_EnterNewSmsNoPrivacyProtect(void);
LOCAL void ResetWidgetSms();
/**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_newsms_widget =
{
        MMIWIDGET_NEWSMS_ID, 
		HandleWidgetNewSmsWinMsg, 
		DisplayNewSmsValid,
		ResetWidgetSms,
		PNULL,		
		WIDGET_NEWSMS_WIN_ID, 
		IMAGE_WIDGET_NEWSMS_SHORTCUT, 
		IMAGE_WIDGET_NEWSMS_BG, 
		TXT_WIDGET_NEWSMS,
		50,//    75, 
		50,//    50
		TRUE,
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL void ResetWidgetSms()
{
	s_widget_newsms_hit_type = MMIWIDGET_NEWSMS_HIT_NONE;
}

LOCAL MMI_RESULT_E  DisplayNewSmsValid(void)
{
    if (GetNewSmsNum() > 0)
    {
        return MMI_RESULT_TRUE;
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

extern PUBLIC void SetPromptWinId(MMI_HANDLE_T win_id);

LOCAL MMI_RESULT_E  HandleWidgetNewSmsWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;	
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    
    //dual_sys = MMIAPISMS_GetSimIdOfNewMsg();	//no use code
    switch (msg_id)
    {     
	case MSG_OPEN_WINDOW:
        SetPromptWinId(win_id);
		//MMISMS_PlayNewSMSRing(dual_sys);
		break;
	case MSG_WIDGET_TP_PRESS_DOWN:
		recode = HandleNewSmsTpPressDown(win_id, param);
		break;	
        
    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//关振动
        MMIAPIENVSET_SetVibStatus(FALSE);
        MMIENVSET_StopOnetimeVibTimer();
        break;
		
	case MSG_WIDGET_TP_PRESS_UP:
		recode = HandleNewSmsTpPressUp(win_id, param);
		break;
    case MSG_TIMER:
        
        if (MMIENVSET_IsOnetimeVibTimerOut(param))
        {
            if (MMIAPIENVSET_GetVibStatus())
            {
                MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//关振动
                MMIAPIENVSET_SetVibStatus(FALSE);
                MMIENVSET_StartOnetimeVibTimer();
            }
            else
            {
                MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_MSG);
                MMIAPIENVSET_SetVibStatus(TRUE);
                MMIENVSET_StartOnetimeVibTimer();
            }
        }
        break;

	case MSG_LCD_SWITCH:
        ResetWidgetSms();
        break;
        
	case MSG_FULL_PAINT:			
		MMIWIDGET_NewSms_DisplayNewSmsPanel(win_id);
		break;
		
	default:
		recode = MMI_RESULT_FALSE;
		break;
    }
    
    return recode;        
}


LOCAL void MMIWIDGET_NewSms_GetNewSmsTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color,
                                                                         GUI_ALIGN_E	font_align									   
										   )
{
    //SCI_ASSERT(PNULL != style_ptr);
    
    if (PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NewSms_GetNewSmsTextStyle PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_NEWSMS_225_112_2_18_3_8_22_220,(uint8*)"");
        return;
    }
	
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = font_align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
}

LOCAL uint16 GetNewSmsNum(void)
{ 
    return MMIAPISMS_GetAllUnreadMsgCount();
}

LOCAL void MMIWIDGET_NewSms_DisplayNewSmsPanel(
										   MMI_WIN_ID_T     win_id
										   )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    // UILAYER_HANDLE_T layer_handle = 0;
    GUI_POINT_T      point={0};
    GUISTR_STYLE_T text_style = {0};
    GUI_RECT_T text_rect = {0};	
    GUI_RECT_T open_rect = MMIWIDGET_NEWSMS_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_NEWSMS_CANCEL_RECT;	
    //
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 new_msg_num[6] = {0};
    wchar disp_num[10] = {0};	
    MMI_STRING_T prompt_str = {0};	
    MMI_STRING_T tmp_prompt = {0};	
    GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;		
    uint16 to_be_read_num = GetNewSmsNum();
    GUISTR_INFO_T    tmp_str_info={0};
	
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
			IMAGE_WIDGET_NEWSMS_BG,
			&lcd_dev_info
			);
		
		open_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, open_rect);
		canecl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, canecl_rect);
		GUIRES_DisplayImg(
			PNULL,
			&open_rect,
			PNULL,
			win_id,
			IMAGE_WIDGET_NEWSMS_LEFT_BUTTON_OFF,
			&lcd_dev_info
			);	
		GUIRES_DisplayImg(
			PNULL,
			&canecl_rect,
			PNULL,
			win_id,
			IMAGE_WIDGET_NEWSMS_RIGHT_BUTTON_OFF,
			&lcd_dev_info
			);			
		//TP down 后图片的更换			
        switch (s_widget_newsms_hit_type)
        {
		case MMIWIDGET_NEWSMS_HIT_OPEN:
			GUIRES_DisplayImg(
				PNULL,
				&open_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_NEWSMS_LEFT_BUTTON_ON,
				&lcd_dev_info
				);
			break;
		case MMIWIDGET_NEWSMS_HIT_CANCEL:
			GUIRES_DisplayImg(
				PNULL,
				&canecl_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_NEWSMS_RIGHT_BUTTON_ON,
				&lcd_dev_info
				);
			break;
		default:
			break;			
        }
		MMIWIDGET_NewSms_GetNewSmsTextStyle(&text_style, MMIWIDGET_NEWSMS_TEXT_FONT, MMITHEME_GetDefaultWidgetThemeColor(),ALIGN_HVMIDDLE);			
		//画OPEN的文本
		MMI_GetLabelTextByLang(TXT_COMM_OPEN, &tmp_prompt);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&open_rect,
			&open_rect,
			&tmp_prompt,	   
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
			); 
		//画cancel的文本
		MMI_GetLabelTextByLang(STXT_CANCEL, &tmp_prompt);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&canecl_rect,
			&canecl_rect,
			&tmp_prompt,	   
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
			); 			
		//显示几新信息
		prompt_str.wstr_len = 0;
		prompt_str.wstr_ptr = PNULL;
		
		// if there is no new message, return
		if (0 == to_be_read_num)
		{
			return;
		}
		MMIWIDGET_NewSms_GetNewSmsTextStyle(&text_style, MMIWIDGET_NEWSMS_TEXT_FONT, MMIWIDGET_NEWSMS_TEXT_COLOR,ALIGN_HVMIDDLE);		        
		disp_num_len= sprintf((char *)new_msg_num, "%d", to_be_read_num);
		MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8*)new_msg_num, disp_num_len, disp_num_len);
		
		// the total string wstr_len
		disp_str_len =  disp_num_len  + 1;   
		// allocate memory
		prompt_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
		SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
		
		MMI_WSTRNCPY(prompt_str.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);
		
		prompt_str.wstr_len = disp_num_len;
		
		GUISTR_GetStringInfo(&text_style, &prompt_str, text_state,&tmp_str_info);
		text_rect.left = MMIWIDGET_NEWSMS_LEFT_SPACE;
		text_rect.top = MMIWIDGET_NEWSMS_TOP_SPACE;
		text_rect.right = text_rect.left + tmp_str_info.width -1;
		text_rect.bottom = text_rect.top + 	 tmp_str_info.height -1;
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
		
		if (to_be_read_num > 1)
		{
			MMI_GetLabelTextByLang(TXT_SMS_NEW_MSGS, &tmp_prompt);
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_SMS_NEW_MSG, &tmp_prompt);
		}
		GUISTR_GetStringInfo(&text_style, &tmp_prompt, text_state,&tmp_str_info);	
		text_rect.top =   text_rect.bottom + MMIWIDGET_NEWSMS_LINE_SPACE;	  
		text_rect.right = text_rect.left + tmp_str_info.width -1;	 
        text_rect.bottom = text_rect.top + tmp_str_info.height -1;
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
		point.y =   text_rect.bottom + MMIWIDGET_NEWSMS_SENTENCE_SPACE;
		point = MMI_ConvertWinPoint(MMICOM_WINPOS_DISP2WIN, win_id, point); 
		//获取最新一条短信的信息
		MMIWIDGET_NewSms_DisplaySmsDetail(win_id,&lcd_dev_info,point);
		
		if (PNULL != prompt_str.wstr_ptr)
		{
			SCI_FREE(prompt_str.wstr_ptr);
			prompt_str.wstr_ptr = PNULL;
		}		
        
    }
}

LOCAL MMI_RESULT_E  HandleNewSmsTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T open_rect = MMIWIDGET_NEWSMS_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_NEWSMS_CANCEL_RECT;
    // GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    if (GUI_PointIsInRect(point, open_rect))
    {
        s_widget_newsms_hit_type = MMIWIDGET_NEWSMS_HIT_OPEN;
        MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//关振动
        MMIAPIENVSET_SetVibStatus(FALSE);
        MMIENVSET_StopOnetimeVibTimer();
    }
    else if (GUI_PointIsInRect(point, canecl_rect))
    {
        s_widget_newsms_hit_type = MMIWIDGET_NEWSMS_HIT_CANCEL;
        MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//关振动
        MMIAPIENVSET_SetVibStatus(FALSE);
        MMIENVSET_StopOnetimeVibTimer();
    }
    else
    {
        s_widget_newsms_hit_type = MMIWIDGET_NEWSMS_HIT_NONE;
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}
LOCAL MMI_RESULT_E HandleNewSmsTpPressUp(MMI_WIN_ID_T win_id, DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_RECT_T open_rect = MMIWIDGET_NEWSMS_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_NEWSMS_CANCEL_RECT;   
	GUI_POINT_T point = {0};	
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);	
    if (GUI_PointIsInRect(point, open_rect)) 
    {
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_SMS, MMIWIDGET_NewSms_EnterNewSmsNoPrivacyProtect);
    }
    else if (GUI_PointIsInRect(point, canecl_rect))
     {
		if (MMIWIDGET_NEWSMS_HIT_CANCEL == s_widget_newsms_hit_type)
		{
			MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_NEWSMS_ID);
		}
     }
     else
     {
	      recode = MMI_RESULT_FALSE;
     }
	
	s_widget_newsms_hit_type = MMIWIDGET_NEWSMS_HIT_NONE;
	return recode;
}

/*
LOCAL uint8 MMIWIDGET_NewSms_GetOriginNumByPos(                        //RETURN: the length of num_ptr
                                      MN_DUAL_SYS_E     *dual_sys_ptr,
                                      uint8                *num_ptr,        //OUT:
                                      uint8                 max_num_len    //IN:                                          
                                      )
{
    uint8 len = 0;
    uint8 i = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
	//  g_mmisms_global.operation.cur_order_index = 0;
    cur_order_id = MMIAPISMS_GetFirstUnreadMsgId();
 
    if (PNULL == cur_order_id)
    {
        *dual_sys_ptr = MN_DUAL_SYS_MAX;
        return 0;
    }

    //SCI_ASSERT( PNULL != num_ptr && dual_sys_ptr != PNULL);
    //SCI_ASSERT( PNULL != num_ptr );

    if (PNULL == num_ptr || PNULL == dual_sys_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NewSms_GetOriginNumByPos PNULL == num_ptr || PNULL == dual_sys_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_NEWSMS_514_112_2_18_3_8_23_221,(uint8*)"");
        return len;
    }
    
    if (cur_order_id->flag.msg_type == MMISMS_TYPE_SMS)
    {
        len = MMIAPICOM_GenDispNumber(
            MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton),
            MIN(MN_MAX_ADDR_BCD_LEN, cur_order_id->number.bcd_number.number_len),
            cur_order_id->number.bcd_number.number,
            num_ptr,
            max_num_len
            );        
    }
    else
    {
        len = strlen((char *)cur_order_id->number.sender);
        //changed by minghu cr128630
        for (i = 0; i < len; i++)
        {
            if (MMIAPICOM_IsTelephoneNum(*(cur_order_id->number.sender+i)))
            {
                *(num_ptr + i) = *(cur_order_id->number.sender + i);
            }
            else
            {
                len = i;
                break;
            }
        }
        *(num_ptr + len) = 0;
    }
    *dual_sys_ptr = (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys;
	
    return (len);
}
LOCAL void MMIWIDGET_NewSms_GetOriginTimeAndDate(
                                        wchar    *date_ptr,        //OUT:
                                        wchar    *time_ptr        //OUT:
                                        )
{
    uint8 len = 0;
    MN_SMS_TIME_STAMP_T    time_stamp  = {0};
    MMI_TM_T mmi_time_stamp = {0};
    uint32 year = 0;
    uint8 date[20] = {0};
    uint8 time[20] = {0};
    MMISMS_ORDER_ID_T   cur_order_id = PNULL;


    if (PNULL == date_ptr || PNULL == time_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NewSms_GetOriginTimeAndDate PNULL == date_ptr || PNULL == time_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_NEWSMS_566_112_2_18_3_8_23_222,(uint8*)"");
        return;
    }
    
    cur_order_id = MMIAPISMS_GetFirstUnreadMsgId();
    
    if (PNULL == cur_order_id)
    {
        return;
    }
    
    mmi_time_stamp = MMIAPICOM_Second2Tm(cur_order_id->time);
    time_stamp.day = mmi_time_stamp.tm_mday;
    time_stamp.hour = mmi_time_stamp.tm_hour;
    time_stamp.minute = mmi_time_stamp.tm_min;
    time_stamp.month = mmi_time_stamp.tm_mon;
    time_stamp.second = mmi_time_stamp.tm_sec;
    time_stamp.timezone = 0;
    year = mmi_time_stamp.tm_year;

    
    //SCI_ASSERT(PNULL != date_ptr);
    //SCI_ASSERT(PNULL != time_ptr);

    MMIAPISET_FormatDateStrByDateStyle(year,time_stamp.month, time_stamp.day,'/',date,20);

    len = strlen((char*)date);

    MMI_STRNTOWSTR(date_ptr, len, (const uint8*)date, len, len);
    
    MMIAPISET_FormatTimeStrByTime(time_stamp.hour, time_stamp.minute,time,20);

    len =  (uint16)strlen((char*)time);

    MMI_STRNTOWSTR(time_ptr, len, (const uint8*)time, len, len);
}
*/

LOCAL void  MMIWIDGET_NewSms_DisplaySmsDetail(MMI_WIN_ID_T win_id,
											  const GUI_LCD_DEV_INFO *dev_info_ptr,
											  GUI_POINT_T start_point)  //相对坐标
{
	// GUI_RECT_T win_rect = {0};
	MMI_STRING_T string_prefix = {0};
	uint8 number[30 + 1] = {0};
	uint8 number_len = 0;	 
	MMI_STRING_T name_string = {0};
	MMI_STRING_T date_string = {0};
	MMI_STRING_T time_string = {0};	  
	BOOLEAN find_result = FALSE;	  
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;	  
	wchar content1_str[30+1] = {0};
	wchar content2_str[30+1] = {0};	 
	GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;	
	GUISTR_INFO_T  str_info = {0};
    GUI_POINT_T  point =start_point;
	GUI_POINT_T  lcd_point ={0};
    GUI_RECT_T    name_rect = {0};	 
	GUI_POINT_T  temp_point ={0};	
	MMIWIDGET_NewSms_GetNewSmsTextStyle(&text_style, MMIWIDGET_NEWSMS_TEXT_FONT, MMIWIDGET_NEWSMS_TEXT_COLOR,ALIGN_LEFT);	  
	// 名称:
	MMI_GetLabelTextByLang(TXT_COMMON_DETAIL_FROM, &string_prefix);
	
    lcd_point = MMI_ConvertWinPoint(MMICOM_WINPOS_WIN2DISP, win_id, point);
    temp_point =  lcd_point;
    GUISTR_DrawTextToLCDSingleLine((const GUI_LCD_DEV_INFO *)dev_info_ptr, (const GUI_POINT_T      *)&lcd_point, 
		(const MMI_STRING_T     *)&string_prefix,
		&text_style,
		text_state
		);	
	
//    number_len = MMIWIDGET_NewSms_GetOriginNumByPos(
    number_len = MMIAPISMS_GetOriginNumByPos(    
		&dual_sys,
		number,
		30
		);
    //查找该号码是否存在PB中，存在则显示名称 
    name_string.wstr_ptr = (wchar*)SCI_ALLOC_APP((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
    name_string.wstr_len = MMISMS_PBNAME_MAX_LEN;
    SCI_MEMSET(name_string.wstr_ptr, 0x00, (MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
    
    find_result = MMIAPISMS_GetNameByNumberFromPB(number, &name_string, MMISMS_PBNAME_MAX_LEN);
	
    if (!find_result) //没有在电话本中找到相应纪录则显示号码
    {
        name_string.wstr_len = MMISMS_PBNAME_MAX_LEN;
		
        MMI_STRNTOWSTR(name_string.wstr_ptr,
			name_string.wstr_len,
			(const uint8*)number,
			number_len,
			number_len);
		
        name_string.wstr_len = number_len;
    }	
	GUISTR_GetStringInfo(&text_style, &string_prefix, text_state,&str_info);		
	//计算显示来短信号码或者姓名的区域
	name_rect.top = point.y;
	
	name_rect.left = point.x+ str_info.width;
	//根据背景图片算区域的右下位置
	GUIRES_GetImgWidthHeight(&name_rect.right,PNULL, IMAGE_WIDGET_NEWSMS_BG, win_id);/*lint !e64*/
	name_rect.right = name_rect.right - MMIWIDGET_NEWSMS_LEFT_SPACE;
	//根据字体的高度重新调整
	name_rect.bottom = name_rect.top +str_info.height;
	
	name_rect=  MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, name_rect);
	//display name
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)dev_info_ptr,
		&name_rect,
		&name_rect,
		&name_string,	   
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);
	
	
    lcd_point.y = lcd_point.y +str_info.height +MMIWIDGET_NEWSMS_LINE_SPACE;
    lcd_point.x = temp_point.x;
    MMI_GetLabelTextByLang(TXT_COMMON_TIME, &string_prefix);   
    GUISTR_DrawTextToLCDSingleLine(
		(const GUI_LCD_DEV_INFO *)dev_info_ptr,
		(const GUI_POINT_T      *)&lcd_point,
		&string_prefix,
		&text_style,
		text_state
		);	
	
	GUISTR_GetStringInfo(&text_style, &string_prefix, text_state,&str_info);	
	lcd_point.x  += str_info.width;		 
    SCI_MEMSET(content1_str, 0x00, sizeof(content1_str));
    SCI_MEMSET(content1_str, 0x00, sizeof(content2_str));
	
    // get original date and time
//    MMIWIDGET_NewSms_GetOriginTimeAndDate(
    MMIAPISMS_GetOriginTimeAndDate(
		content1_str,	// date
		content2_str	// time
		);
	
    // Date
    date_string.wstr_ptr = content1_str;
    date_string.wstr_len = MMIAPICOM_Wstrlen(content1_str);
    time_string.wstr_ptr = content2_str;
    time_string.wstr_len = MMIAPICOM_Wstrlen(content2_str);
	
    GUISTR_DrawTextToLCDSingleLine(
		(const GUI_LCD_DEV_INFO *)dev_info_ptr,
		(const GUI_POINT_T      *)&lcd_point,
		&date_string,
		&text_style,
		text_state
		);
	GUISTR_GetStringInfo(&text_style, &date_string, text_state,&str_info);	
    lcd_point.y  = lcd_point.y +str_info.height+MMIWIDGET_NEWSMS_LINE_SPACE;
    GUISTR_DrawTextToLCDSingleLine(
		(const GUI_LCD_DEV_INFO *)dev_info_ptr,
		(const GUI_POINT_T      *)&lcd_point,
		&time_string,
		&text_style,
		text_state
		);	
    if (name_string.wstr_ptr != PNULL)
    {
        SCI_FREE(name_string.wstr_ptr);
        name_string.wstr_ptr = PNULL;
    }
	
}
LOCAL void MMIWIDGET_NewSms_EnterNewSmsNoPrivacyProtect(void)
{
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
#ifdef MMI_SMS_CHAT_SUPPORT  
        //MMIAPISMS_ReadNewSms(MMISMS_BOX_CHAT);
        MMIAPISMS_OpenMainMenuWin();
        return TRUE;
#endif
#else
    //如果只有一条新信息，就去读这条信息
    //多条新信息的就进入短信列表窗口
    MMIAPISMS_EnterSmsBox( MMISMS_BOX_MT ); 
    
    if (1 == GetNewSmsNum() 
        && ( MMIAPISMS_GetFirstUnreadMsgId() != PNULL ))
    {
        MMK_PostMsg(MMISMS_MSGBOX_MAIN_WIN_ID,
            MMISMS_MSG_READ_MSG,
            PNULL,
            0
            );
    }
#endif
}
#endif
