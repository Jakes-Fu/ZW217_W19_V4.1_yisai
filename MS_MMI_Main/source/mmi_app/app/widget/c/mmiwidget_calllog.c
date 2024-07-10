/*****************************************************************************
** File Name:      mmiwidget_calllog.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**       Creat
******************************************************************************/

#define _MMIWIDGET_CALLLOG_C_

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
#include "mmicc_text.h" 
#include "mmicc_id.h"
#include "mmicl_internal.h"    
#include "mmisms_read.h"
//#include "Mmiset.h"
#include "mmiset_export.h"
//#include "mmiset_display.h"
#include "mmiwidget_position.h"
#include "mmisms_text.h"
#include "mmiwidget_export.h" 
#include "mmiwidget_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/



#define MMIWIDGET_CALLLOG_TEXT_FONT          SONG_FONT_12

#define MMIWIDGET_CALLLOG_TEXT_COLOR         MMI_BLACK_COLOR

#define     MMIWIDGET_CALLLOG_DISPLAY_TIME_LEN  9

#define     MMIWIDGET_CALLLOG_DISPLAY_DATE_LEN  11


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
    MMIWIDGET_CALLLOG_HIT_NONE,        
    MMIWIDGET_CALLLOG_HIT_OPEN,       
    MMIWIDGET_CALLLOG_HIT_CANCEL,      
    MMIWIDGET_CALLLOG_HIT_MAX
}MMIWIDGET_CALLLOG_HIT_TYPE_E;
//点中区域的标识
LOCAL uint32 s_widget_calllog_hit_type = MMIWIDGET_CALLLOG_HIT_NONE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetCallLogWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/

LOCAL void MMIWIDGET_DisplayCallLogPanel(
										   MMI_WIN_ID_T     win_id
										      );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/

LOCAL void MMIWIDGET_GetCallLogTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color,
                                                                         GUI_ALIGN_E	font_align									   
										   );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleCallLogTpPressUp(MMI_WIN_ID_T win_id, DPARAM param);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleCallLogTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/

LOCAL void  MMIWIDGET_DisplayMissLogDetail(MMI_WIN_ID_T win_id,
										   const GUI_LCD_DEV_INFO *dev_info_ptr,
										   GUI_POINT_T start_point); //相对坐标

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DisplayMissCallValid();
LOCAL void MMIWIDGET_Calllog_EnterWidgetCLNoPrivacyProtect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : reset call log button status.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  ResetWidgetCalllog();
/**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_calllog_widget =
{
        MMIWIDGET_CALLLOG_ID, 
		HandleWidgetCallLogWinMsg, 
      	DisplayMissCallValid,
       	ResetWidgetCalllog,
       	PNULL,
		WIDGET_CALLLOG_WIN_ID, 
		IMAGE_WIDGET_CALLLOG_SHORTCUT, 
		IMAGE_WIDGET_CALLLOG_BG,
        TXT_WIDGET_CALLLOG,		
		//    201, 
		//    71, 
		0,//    75, 
		0,//    50
        TRUE,
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : reset call log button status.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  ResetWidgetCalllog()
{
	s_widget_calllog_hit_type = MMIWIDGET_CALLLOG_HIT_NONE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DisplayMissCallValid()
{
    if (MMIAPICC_GetMissedCallNumber() > 0)
    {
        return MMI_RESULT_TRUE;
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}



/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetCallLogWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;	
	
    switch (msg_id)
    {     
	case MSG_OPEN_WINDOW:
		break;
	case MSG_WIDGET_TP_PRESS_DOWN:
		recode = HandleCallLogTpPressDown(win_id, param);
		break;			
		
	case MSG_WIDGET_TP_PRESS_UP:
		recode = HandleCallLogTpPressUp(win_id, param);
		break;
	
    case MSG_LCD_SWITCH:
        ResetWidgetCalllog();
        break;
        
	case MSG_FULL_PAINT:			
		MMIWIDGET_DisplayCallLogPanel(win_id);
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
LOCAL void MMIWIDGET_GetCallLogTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color,
                                                                         GUI_ALIGN_E	font_align									   
										   )
{
    //SCI_ASSERT(PNULL != style_ptr);
    
    if (PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_GetCallLogTextStyle PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALLLOG_248_112_2_18_3_7_58_185,(uint8*)"");
        return;
    }
	
    style_ptr->effect 	= FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle 	= ANGLE_0;
    style_ptr->align 	= font_align;
    style_ptr->font 	= font;
    style_ptr->font_color = font_color;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_DisplayCallLogPanel(
										 MMI_WIN_ID_T     win_id
										 )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
	//    UILAYER_HANDLE_T layer_handle = 0;
    GUI_POINT_T      point={0};
    GUISTR_STYLE_T text_style = {0};
    GUI_RECT_T text_rect = {0};	
    GUI_RECT_T open_rect = MMIWIDGET_CALLLOG_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_CALLLOG_CANCEL_RECT;	
    //
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 missed_call_num[6] = {0};
    wchar disp_num[10] = {0};	
    MMI_STRING_T prompt_str = {0};	
    MMI_STRING_T tmp_prompt = {0};	
    GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;	
    uint32 miss_call_num = MMIAPICC_GetMissedCallNumber();
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
			IMAGE_WIDGET_CALLLOG_BG,
			&lcd_dev_info
			); 
		
		
		open_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, open_rect);
		canecl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, canecl_rect);
		GUIRES_DisplayImg(
			PNULL,
			&open_rect,
			PNULL,
			win_id,
			IMAGE_WIDGET_CALLLOG_LEFT_BUTTON_OFF,
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
        switch (s_widget_calllog_hit_type)
        {
		case MMIWIDGET_CALLLOG_HIT_OPEN:
			GUIRES_DisplayImg(
				PNULL,
				&open_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_CALLLOG_LEFT_BUTTON_ON,
				&lcd_dev_info
				);
			break;
		case MMIWIDGET_CALLLOG_HIT_CANCEL:
			GUIRES_DisplayImg(
				PNULL,
				&canecl_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_CALLLOG_RIGHT_BUTTON_ON,
				&lcd_dev_info
				);
			break;
		default:
			break;			
        }
		
		//画OPEN的文本
		if(1 == miss_call_num)
		{
			MMI_GetLabelTextByLang(TXT_DIAL, &tmp_prompt);
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_COMM_OPEN, &tmp_prompt);
		}
		MMIWIDGET_GetCallLogTextStyle(&text_style,MMIWIDGET_CALLLOG_TEXT_FONT, MMITHEME_GetDefaultWidgetThemeColor(),ALIGN_HVMIDDLE);		
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
		//显示未接来电个数
		prompt_str.wstr_len = 0;
		prompt_str.wstr_ptr = PNULL;
		
		// if there is no new miss call, return
		if (0 == miss_call_num)
		{
			return;
		}	
		MMIWIDGET_GetCallLogTextStyle(&text_style,MMIWIDGET_CALLLOG_TEXT_FONT, MMIWIDGET_CALLLOG_TEXT_COLOR,ALIGN_HVMIDDLE);			
		disp_num_len= sprintf((char *)missed_call_num, "%d", miss_call_num);
		MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8*)missed_call_num, disp_num_len, disp_num_len);
		disp_str_len =  disp_num_len + 1;
		
		// allocate memory
		prompt_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
		SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
		
		MMI_WSTRNCPY(prompt_str.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);
		
		prompt_str.wstr_len = disp_num_len;
		/*
		// copy the prompt string
		MMI_WSTRNCPY(
		&(prompt_str.wstr_ptr[prompt_str.wstr_len]),
		tmp_prompt.wstr_len,
		tmp_prompt.wstr_ptr, 
		tmp_prompt.wstr_len,
		tmp_prompt.wstr_len
		);
		prompt_str.wstr_len += tmp_prompt.wstr_len;
		*/
		GUISTR_GetStringInfo(&text_style, &prompt_str, text_state,&tmp_str_info);
		text_rect.left = MMIWIDGET_CALLLOG_LEFT_SPACE;
		text_rect.top = MMIWIDGET_CALLLOG_TOP_SPACE;
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
        
        if (1 == miss_call_num)
        {
            MMI_GetLabelTextByLang(TXT_CC_MISS_CALL, &tmp_prompt);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_COMMON_MISS_CALLS, &tmp_prompt);
        }
			  
		GUISTR_GetStringInfo(&text_style, &tmp_prompt, text_state,&tmp_str_info);	
		text_rect.top =   text_rect.bottom + MMIWIDGET_CALLLOG_LINE_SPACE;	  
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
		point.y =   text_rect.bottom + MMIWIDGET_CALLLOG_SENTENCE_SPACE;
		point = MMI_ConvertWinPoint(MMICOM_WINPOS_DISP2WIN, win_id, point);
		//获取最新一条记录的信息
		MMIWIDGET_DisplayMissLogDetail(win_id, &lcd_dev_info,point);
		
        
    }
	
    if (PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr = PNULL;
    }	
	
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallLogTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T open_rect = MMIWIDGET_CALLLOG_OEPN_RECT;
    GUI_RECT_T canecl_rect = MMIWIDGET_CALLLOG_CANCEL_RECT;
   // GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    if (GUI_PointIsInRect(point, open_rect))
    {
        s_widget_calllog_hit_type = MMIWIDGET_CALLLOG_HIT_OPEN;
    }
    else if (GUI_PointIsInRect(point, canecl_rect))
    {
        s_widget_calllog_hit_type = MMIWIDGET_CALLLOG_HIT_CANCEL;
    }
    else
    {
        s_widget_calllog_hit_type = MMIWIDGET_CALLLOG_HIT_NONE;
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void WIDGET_CallLog_ValidatePrivacy(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMI_WIN_ID_T win_id = 0;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            win_id = *(MMI_WIN_ID_T *)param_ptr;
        }
        
        MMIWIDGET_Calllog_EnterWidgetCLNoPrivacyProtect(win_id);  
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCallLogTpPressUp(MMI_WIN_ID_T win_id, DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	// MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
	// MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
	// MMICL_CALL_TYPE_E    call_type = MMICL_CALL_MISSED;
	GUI_RECT_T open_rect = MMIWIDGET_CALLLOG_OEPN_RECT;
	GUI_RECT_T canecl_rect = MMIWIDGET_CALLLOG_CANCEL_RECT;
	GUI_POINT_T point = {0};
	// uint8  record_index = 0;
	// uint32 miss_call_num = MMIAPICC_GetMissedCallNumber();
	
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
	
	
	if (GUI_PointIsInRect(point, open_rect))
	{
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, WIDGET_CallLog_ValidatePrivacy, &win_id, sizeof(win_id));
	}
	else if(GUI_PointIsInRect(point, canecl_rect))
	{
		if (MMIWIDGET_CALLLOG_HIT_CANCEL == s_widget_calllog_hit_type)
		{
			MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_CALLLOG_ID);
		}			
	}
	else
	{
		recode = MMI_RESULT_FALSE;
	}
	
	s_widget_calllog_hit_type = MMIWIDGET_CALLLOG_HIT_NONE;
	return recode;
	
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  MMIWIDGET_DisplayMissLogDetail(MMI_WIN_ID_T win_id,
										   const GUI_LCD_DEV_INFO *dev_info_ptr,
										   GUI_POINT_T start_point) //相对坐标
{
	//     GUI_RECT_T 		win_rect = {0};
	MMI_STRING_T 	string_prefix = {0};
	MMI_STRING_T 	name_string = {0};
	GUISTR_INFO_T 	str_info = {0};
	GUI_POINT_T  		point = start_point;
	GUI_POINT_T  		lcd_point ={0};
	GUI_POINT_T  		temp_point ={0};
	GUI_RECT_T            name_rect = {0};	 	  
	GUISTR_STYLE_T 	text_style = {0};
    GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;	
	BOOLEAN 			find_result =FALSE;
	
	MN_RETURN_RESULT_E          		return_value = MN_RETURN_FAILURE;
	MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
	MMICL_CALL_TYPE_E    			call_type = MMICL_CALL_MISSED;
	uint8  	record_index = 0;
	
    uint8   	tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 		tele_num_len = 0;	
    uint8   	time[MMIWIDGET_CALLLOG_DISPLAY_TIME_LEN + 1 ] = {0};
    wchar   	time_wstr[MMIWIDGET_CALLLOG_DISPLAY_TIME_LEN + 1] = {0};
    uint8   	date[MMIWIDGET_CALLLOG_DISPLAY_DATE_LEN + 1] = {0};
    wchar  	date_wstr[MMIWIDGET_CALLLOG_DISPLAY_DATE_LEN + 1] = {0};
    MMI_TM_T  tm = {0};
    MMI_STRING_T    time_str = {0};
    MMI_STRING_T    date_str = {0};
    uint32 miss_call_num = MMIAPICC_GetMissedCallNumber();	
	
	arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCA(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

	if (PNULL == arraycall_info)
	{
	    //SCI_TRACE_LOW:"MMIWIDGET_DisplayMissLogDetail arraycall_info failed to alloc memory"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALLLOG_599_112_2_18_3_7_59_186,(uint8*)"");
	    return;
	}
	
	//SCI_ASSERT(PNULL != arraycall_info);    
	
	SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
	SCI_MEMSET(&tm,0,sizeof(tm));
	
	MMIWIDGET_GetCallLogTextStyle(&text_style, MMIWIDGET_CALLLOG_TEXT_FONT, MMIWIDGET_CALLLOG_TEXT_COLOR,ALIGN_LEFT);	  
	
	// 名称:
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
	
	
	
	
	if(miss_call_num !=0)
	{
		return_value = MMICL_ReadNV(call_type, arraycall_info);
		record_index  	= 0;//取最新的记录
		
		tele_num_len = MMIAPICOM_GenDispNumber(arraycall_info->call_bcdinfo[record_index].number_type,
            arraycall_info->call_bcdinfo[record_index].number_len,
            arraycall_info->call_bcdinfo[record_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
		
		name_string.wstr_ptr = (wchar*)SCI_ALLOC_APP((PHONE_NUM_MAX_LEN+1)*sizeof(wchar));
		name_string.wstr_len = PHONE_NUM_MAX_LEN;
		SCI_MEMSET(name_string.wstr_ptr, 0x00, (PHONE_NUM_MAX_LEN+1)*sizeof(wchar));
		
		//查找该号码是否存在PB中，存在则显示名称     
		find_result = MMIAPISMS_GetNameByNumberFromPB(tele_num, &name_string, PHONE_NUM_MAX_LEN);
		
		if (!find_result) //没有在电话本中找到相应纪录则显示号码
		{
			name_string.wstr_len = PHONE_NUM_MAX_LEN;
			
			MMI_STRNTOWSTR(name_string.wstr_ptr,
				name_string.wstr_len,
				(const uint8*)tele_num,
				tele_num_len,
				tele_num_len);
			
			name_string.wstr_len =  (uint16)strlen((char*)tele_num);;
		}	
		GUISTR_GetStringInfo(&text_style, &string_prefix, text_state,&str_info);		
		//计算显示来短信号码或者姓名的区域
		name_rect.top = point.y;
		name_rect.left = point.x+ str_info.width;
		//根据背景图片算区域的右下位置
		GUIRES_GetImgWidthHeight(&name_rect.right,PNULL, IMAGE_WIDGET_CALLLOG_BG, win_id);/*lint !e64*/
		name_rect.right = name_rect.right - MMIWIDGET_CALLLOG_LEFT_SPACE;
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
		
		//get time and date
		tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[record_index].call_start_time);/*lint !e64*/
		
        MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,time,MMIWIDGET_CALLLOG_DISPLAY_TIME_LEN + 1);
		time_str.wstr_ptr = time_wstr;
		time_str.wstr_len = (uint16)strlen((char*)time);
		
		MMI_STRNTOWSTR( time_wstr, MMIWIDGET_CALLLOG_DISPLAY_TIME_LEN, (const uint8*)time, time_str.wstr_len, time_str.wstr_len );
		
		MMIAPISET_FormatDateStrByDateStyle(tm.tm_year,tm.tm_mon,tm.tm_mday,'/', date,MMIWIDGET_CALLLOG_DISPLAY_DATE_LEN + 1);
		date_str.wstr_ptr = date_wstr;
		date_str.wstr_len = (uint16)strlen((char*)date);
		MMI_STRNTOWSTR( date_wstr, MMIWIDGET_CALLLOG_DISPLAY_DATE_LEN, (const uint8*)date, date_str.wstr_len, date_str.wstr_len );
		
		
		lcd_point.y = lcd_point.y +str_info.height +MMIWIDGET_CALLLOG_LINE_SPACE;
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
		
		
		GUISTR_DrawTextToLCDSingleLine(
			(const GUI_LCD_DEV_INFO *)dev_info_ptr,
			(const GUI_POINT_T      *)&lcd_point,
			&date_str,
			&text_style,
			text_state
			);
		GUISTR_GetStringInfo(&text_style, &date_str, text_state,&str_info);	
		//单行显示不下，换行显示
		lcd_point.y  += str_info.height +MMIWIDGET_CALLLOG_LINE_SPACE;	
		GUISTR_DrawTextToLCDSingleLine(
			(const GUI_LCD_DEV_INFO *)dev_info_ptr,
			(const GUI_POINT_T      *)&lcd_point,
			&time_str,
			&text_style,
			text_state
			);	
		
	}
	//释放空间
    if (name_string.wstr_ptr != PNULL)
    {
        SCI_FREE(name_string.wstr_ptr);
        name_string.wstr_ptr = PNULL;
    }
	
	SCI_FREE(arraycall_info);
	arraycall_info =PNULL;
}
LOCAL void MMIWIDGET_Calllog_EnterWidgetCLNoPrivacyProtect(MMI_WIN_ID_T win_id)
{
	// MMI_RESULT_E recode = MMI_RESULT_TRUE;
	MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
	MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
	MMICL_CALL_TYPE_E    call_type = MMICL_CALL_MISSED;
	uint8  record_index = 0;
	uint32 miss_call_num = MMIAPICC_GetMissedCallNumber();		
	arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCA(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

	if (PNULL == arraycall_info)
	{
	    //SCI_TRACE_LOW:"MMIWIDGET_Calllog_EnterWidgetCLNoPrivacyProtect arraycall_info failed to alloc memory"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALLLOG_751_112_2_18_3_7_59_187,(uint8*)"");
	    return;
	}
	
	//SCI_ASSERT(PNULL != arraycall_info);    
	
	SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));		
	//如果只有一条记录，拨叫该记录
	//多条记录就进入未接电话列表
	return_value  	= MMICL_ReadNV(call_type, arraycall_info);	
	if( miss_call_num == 1)//电话号码存在
	{
		record_index  	= 0;//取最新的记录
		//get call type
		if (arraycall_info->call_bcdinfo[record_index].isvideocall)
		{
			
			//为完成
		}
		else
		{
			MMIAPICL_MakeCallByRecordIndex(call_type,record_index);
			MMIAPICC_ClearMissedCallNum();
		}
	}
	else if(miss_call_num > 1)
	{
		MMIAPICL_OpenCLWindow();
		if(MMK_IsOpenWin(MMICL_MISSED_CHILD_WIN_ID))
		{
			//成功打开未接来电窗口，清空未接来电数目
			MMIAPICC_ClearMissedCallNum();      
		}
	}
	SCI_FREE(arraycall_info);
	arraycall_info =PNULL;
			
}
#endif

