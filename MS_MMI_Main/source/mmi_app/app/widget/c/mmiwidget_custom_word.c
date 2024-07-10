/*****************************************************************************
** File Name:      mmiwidget_custom_word.c                                        *
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

#define _MMIWIDGET_CUSTOM_WORD_C_

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
#include "mmiwidget_image.h"
#include "guibutton.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"

#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"

//#include"Mmiset_menutable.h"
//#include "Mmiset.h"
#include "mmiset_export.h"
//#include "mmiset_wintab.h"
#include "mmi_menutable.h"

#include "Mmk_app.h"
//#include "Mmiset_nv.h"
//#include "mmiset_display.h"
#include "mmiset_text.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIDGET_CUSTOM_WORD_TEXT_FONT         SONG_FONT_20

#define MMIWIDGET_CUSTOM_WORD_TEXT_COLOR        MMITHEME_GetDefaultWidgetThemeColor()

#define MMIWIDGET_CUSTOM_WORD_MARGIN            2
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
LOCAL MMI_RESULT_E  HandleWidgetCustomWordWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   );
LOCAL void MMIWIDGET_CustomWord_DisplayCustomWord(
										   MMI_WIN_ID_T     win_id,
										   MMI_STRING_T *str_ptr);
LOCAL void MMIWIDGET_CustomWord_GetCustomWordTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   );
LOCAL MMI_RESULT_E  DisplayCustomWordValid();
/**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_custom_word_widget =
{
        MMIWIDGET_CUSTOM_WORD_ID, 
		HandleWidgetCustomWordWinMsg, 
		DisplayCustomWordValid,
		PNULL,
		PNULL,
		WIDGET_CUSTOM_WORD_WIN_ID, 
		IMAGE_WIDGET_CUSTOM_WORD_SHORTCUT, 
		IMAGE_WIDGET_CUSTOM_WORD_BG, 
		TXT_WIDGET_CUSTOM_WORD,
		50,//    75, 
		50,//    50
		TRUE,
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E  DisplayCustomWordValid()
{
   	MMISET_IDLE_DISPLAY_T   *idle_set_ptr = PNULL;

    	//get display setting info
    	idle_set_ptr = MMIAPISET_GetIdleDisplayMode();

	if(idle_set_ptr->is_show_idle_words)
	{
		return MMI_RESULT_TRUE;	
	}
	else
	{
		return MMI_RESULT_FALSE;
	}
}

LOCAL MMI_RESULT_E  HandleWidgetCustomWordWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISET_IDLE_DISPLAY_T   *display_info   =   PNULL;
    MMI_STRING_T            custom_word     =   {0};
    // MN_RETURN_RESULT_E          return_value        =   MN_RETURN_FAILURE;
    // MMISET_IDLE_DISPLAY_T       idle_display_info   =   {0};	
    switch (msg_id)
    {     
	case MSG_WIDGET_TP_PRESS_UP:
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
		  MMIAPISET_CreateCustomWordEditWin();
#endif
#endif
		 break;		
	case MSG_GET_FOCUS:
		 MMIAPISET_SetDisplaytIdleMode();
		 break;
	case MSG_FULL_PAINT:
	       display_info = MMIAPISET_GetIdleDisplayMode();
	       custom_word.wstr_ptr =   display_info->idle_words;
	       custom_word.wstr_len =  display_info->wstr_len;	
		MMIWIDGET_CustomWord_DisplayCustomWord(win_id, &custom_word);
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
    }
    
    return recode;        
}


LOCAL void MMIWIDGET_CustomWord_GetCustomWordTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   )
{
    //SCI_ASSERT(PNULL != style_ptr);
    
    if (PNULL == style_ptr)
    {
        return;
    }
	
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_HVMIDDLE;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
}

LOCAL void MMIWIDGET_CustomWord_DisplayCustomWord(
										   MMI_WIN_ID_T     win_id,
										   MMI_STRING_T *str_ptr)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    // UILAYER_HANDLE_T layer_handle = 0;
    GUI_POINT_T      point={0};
    GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T     tempStr = {0};
	
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;	
    MMIWIDGET_CustomWord_GetCustomWordTextStyle(&text_style, MMIWIDGET_CUSTOM_WORD_TEXT_FONT, MMIWIDGET_CUSTOM_WORD_TEXT_COLOR);	
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
		
        //ÏÔÊ¾±³¾°Í¼
        point.x = win_rect.left;
        point.y = win_rect.top;
		
        win_rect.left += MMIWIDGET_CUSTOM_WORD_MARGIN;
        win_rect.right -= MMIWIDGET_CUSTOM_WORD_MARGIN;
		
		GUIRES_DisplayImg(
		&point,
		PNULL,
		PNULL,
		win_id,
		IMAGE_WIDGET_CUSTOM_WORD_BG,
		&lcd_dev_info
		);
		if(0 == str_ptr->wstr_len)
		{
			MMI_GetLabelTextByLang(TXT_SET_CUSTOM_WORD, &tempStr);
			GUISTR_DrawTextToLCDInRect(
				   (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				   &win_rect,
				   &win_rect,
				   &tempStr,	   
				   &text_style,
				   text_state,
				   GUISTR_TEXT_DIR_AUTO
				   );
		}
		else
		{
	           //display string
	           GUISTR_DrawTextToLCDInRect(
				   (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				   &win_rect,
				   &win_rect,
				   str_ptr,	   
				   &text_style,
				   text_state,
				   GUISTR_TEXT_DIR_AUTO
				   );
		}
        
    }
	
	
}




#endif

