/*****************************************************************************
** File Name:      mmiwidget_netsearch.c                                        *
** Author:                                                                   *
** Date:           07/30/2011                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/30/2011        nan.ji              Creat
******************************************************************************/

#define _MMIWIDGET_NETSEARCH_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_widget_trc.h"
#ifdef BROWSER_SUPPORT_DORADO
#ifdef MMI_WIDGET_NETSEARCH
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
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
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define   MMIWIDGET_NETSEARCH_TEXT_FONT                   SONG_FONT_18

#define   MMIWIDGET_NETSEARCH_TEXT_COLOR                  MMI_BLACK_COLOR
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
LOCAL   BOOLEAN   s_is_focus = FALSE;
LOCAL   char  s_netsearch_browser_url[40] ={"http://m.baidu.com/?from=1214a"};
/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Handle Widget NetSearch Win Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetNetSearchWinMsg(
												MMI_WIN_ID_T        win_id,        //IN:
												MMI_MESSAGE_ID_E    msg_id,        //IN:
												DPARAM            param        //IN:
												);
/*****************************************************************************/
// Description : Display NetSearch Widget
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL void DisplayNetSearch(
							MMI_WIN_ID_T win_id,
							DPARAM    param
							);
/*****************************************************************************/
// Description : Handle Widget NetSearch TpPressUp Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E MMIWIDGET_NetSearch_HandleTpPressUp(
														MMI_WIN_ID_T        win_id,        //IN:
														DPARAM            param        //IN:
														);
/*****************************************************************************/
// Description : Handle Widget NetSearch TpPressDown Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_NetSearch_HandleTpPressDown(
														MMI_WIN_ID_T        win_id,        //IN:
														DPARAM            param        //IN:
														);
/*****************************************************************************/
// Description : Get TextStyle
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_NetSearch_GetNetSearchTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   );

/*****************************************************************************/
// Description :OpenNetSearchWin
// Global resource dependence : 
// Author:juan.wu
// Note: 打开网络搜索窗口
/*****************************************************************************/
LOCAL void OpenNetSearchWin(void);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_netsearch_widget =
{
	MMIWIDGET_NETSEARCH_ID, 
	HandleWidgetNetSearchWinMsg, 
	PNULL, 
	PNULL,
    PNULL,	
	WIDGET_NET_SEARCH_WIN_ID, 
	IMAGE_WIDGET_NETSEARCH_SHORTCUT, 
	IMAGE_WIDGET_NETSEARCH_BG,
	TXT_NETWORK_SEARCHING,
	80,//    75, 
	80,//    75
	FALSE
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : Handle Widget NetSearch Win Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetNetSearchWinMsg(
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

    case MSG_LOSE_FOCUS:
		//s_is_focus = TRUE;
		break;
    case MSG_WIDGET_TP_PRESS_DOWN:
		recode = MMI_RESULT_FALSE;//MMIWIDGET_NetSearch_HandleTpPressDown(win_id,param);
		break;

	case MSG_APP_WEB:
		OpenNetSearchWin();
	    break;
	    
    case MSG_WIDGET_TP_PRESS_UP:
        recode = MMIWIDGET_NetSearch_HandleTpPressUp(win_id,param);	
		break;
        
    case MSG_FULL_PAINT:
        DisplayNetSearch(win_id,param);
        break;
    case MSG_GET_FOCUS:
		//s_is_focus = FALSE;
		break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}
/*****************************************************************************/
// Description : Display NetSearch Widget
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL void DisplayNetSearch(
                       MMI_WIN_ID_T win_id,
					   DPARAM    param
                       )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T img_rect = MMIWIDGET_NET_SEARCH_BUTTON_RECT;
    GUI_RECT_T string_rect = MMIWIDGET_NET_SEARCH_STRING_RECT;
    GUI_RECT_T        lcd_rect = {0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;	

	MMI_STRING_T     tempStr = {0};	
	    lcd_rect = MMITHEME_GetFullScreenRect();
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
		GUIRES_DisplayImg(
			PNULL,
			&win_rect,
			PNULL,
			win_id,
			IMAGE_WIDGET_NETSEARCH_BG,
			&lcd_dev_info
			);  
		
		img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, img_rect);
		if (!s_is_focus)
		{
			GUIRES_DisplayImg(
				PNULL,
				&img_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_NETSEARCH_OFF,
				&lcd_dev_info
				);
		}
		else
		{
			GUIRES_DisplayImg(
				PNULL,
				&img_rect,
				PNULL,
				win_id,
				IMAGE_WIDGET_NETSEARCH_ON,
				&lcd_dev_info
				);
		}
		
		MMIWIDGET_NetSearch_GetNetSearchTextStyle(&text_style, MMIWIDGET_NETSEARCH_TEXT_FONT, MMIWIDGET_NETSEARCH_TEXT_COLOR);
		string_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, string_rect);

		MMI_GetLabelTextByLang(STXT_SEARCH, &tempStr);
		//GUI_FillRect(&lcd_dev_info ,string_rect,0);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&string_rect,
			&string_rect,
			&tempStr,	   
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
			);	
	}
}
/*****************************************************************************/
// Description : Handle Widget NetSearch TpPressUp Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_NetSearch_HandleTpPressUp(
														MMI_WIN_ID_T        win_id,        //IN:
														DPARAM            param        //IN:
														)
{  
	MMI_RESULT_E	recode  = MMI_RESULT_TRUE;
	GUI_RECT_T img_rect = MMIWIDGET_NET_SEARCH_BUTTON_RECT;
	GUI_RECT_T str_rect = MMIWIDGET_NET_SEARCH_STRING_RECT;
	GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
	GUI_POINT_T point   = {0};
	
	img_rect.left  = str_rect.left;
	img_rect.right = str_rect.right;
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	//point = MMI_ConvertWinPoint(MMICOM_WINPOS_WIN2DISP, win_id, point);
	MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, img_rect);
	if (GUI_PointIsInRect(point, img_rect))
	{
		//MMK_SendMsg(win_id,MSG_FULL_PAINT,NULL);
		{
			MMIBROWSER_ENTRY_PARAM entry_param = {0};
			entry_param.type = MMIBROWSER_ACCESS_URL;
			entry_param.dual_sys = MN_DUAL_SYS_MAX;
			entry_param.user_agent_ptr = PNULL;
			entry_param.url_ptr = s_netsearch_browser_url;
			MMIAPIBROWSER_Entry(&entry_param);
		}
		s_is_focus = FALSE;
	}
	else
	{
		recode = MMI_RESULT_FALSE;
	}
    return recode;
}	
/*****************************************************************************/
// Description : Handle Widget NetSearch TpPressDown Msg
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_NetSearch_HandleTpPressDown(
														MMI_WIN_ID_T        win_id,        //IN:
														DPARAM            param        //IN:
														)
{  
	MMI_RESULT_E	recode  = MMI_RESULT_TRUE;
	GUI_RECT_T img_rect = MMIWIDGET_NET_SEARCH_BUTTON_RECT;
	GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
	GUI_POINT_T point   = {0};
	
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	//point = MMI_ConvertWinPoint(MMICOM_WINPOS_WIN2DISP, win_id, point);
	MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, img_rect);
	if (GUI_PointIsInRect(point, img_rect))
	{
        //s_is_focus = TRUE;
	}
	else
	{
		recode = MMI_RESULT_FALSE;
	}
	return recode;
}	
/*****************************************************************************/
// Description :Get TextStyle
// Global resource dependence : 
// Author:nan.ji
// Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_NetSearch_GetNetSearchTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   )
{
    //SCI_ASSERT(PNULL != style_ptr);

    if (PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NetSearch_GetNetSearchTextStyle style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_NETSEARCH_325_112_2_18_3_8_20_218,(uint8*)"");
        return;
    }
  	
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
}
/*****************************************************************************/
// Description :OpenNetSearchWin
// Global resource dependence : 
// Author:juan.wu
// Note: 打开网络搜索窗口
/*****************************************************************************/
LOCAL void OpenNetSearchWin(void)
{
	MMIBROWSER_ENTRY_PARAM entry_param = {0};
	entry_param.type = MMIBROWSER_ACCESS_URL;
	entry_param.dual_sys = MN_DUAL_SYS_MAX;
	entry_param.user_agent_ptr = PNULL;
	entry_param.url_ptr = s_netsearch_browser_url;
	MMIAPIBROWSER_Entry(&entry_param);
	s_is_focus = FALSE;
}		
#endif
#endif



