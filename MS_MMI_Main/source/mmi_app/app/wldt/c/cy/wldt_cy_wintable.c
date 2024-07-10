/****************************************************************************
** File Name:      mmiphsapp_wintab.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef _MMIWLDT_WINTAB_C_
#define _MMIWLDT_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_common.h"

#include "wldt_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"

#define WLDT_UI_DEVICE_WIDTH  MMI_MAINSCREEN_WIDTH
#define WLDT_UI_DEVICE_HEIGHT  MMI_MAINSCREEN_HEIGHT
#define WLDT_UI_TITLE_Y	  (0)
#define WLDT_UI_TITLE_HEIGHT   40
#define WLDT_UI_ITEM_WIDTH   100
#define WLDT_UI_ITEM_HEIGHT   40
#define WLDT_UI_OFFSET_X	  (10)
#define WLDT_UI_OFFSET_Y	  (20)
#define WLDT_UI_TEXT_BG_COLOR	  0x094B
#define WLDT_UI_TEXT_BG_FORCUS_COLOR	  MMI_YELLOW_COLOR
#define WLDT_UI_OFFSET_X_2	  (15)
#define WLDT_UI_ITEM_WIDTH_2   60

LOCAL MMI_RESULT_E MmiWldt_OpenClassMenuWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

LOCAL MMI_RESULT_E MmiWldt_OpenClassSingleWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

LOCAL MMI_RESULT_E MmiWldt_OpenClassMultipleWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

LOCAL MMI_RESULT_E MmiWldt_OpenClassJudgeWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

                            
WINDOW_TABLE(MMIWLDT_WATCH_CLASS_MENU_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)MmiWldt_OpenClassMenuWinHandleMsg),
        //win_id
        WIN_ID(MMIWLDT_WATCH_CLASS_MENU_WIN_ID),
        //窗口的标题
        //WIN_TITLE(TXT_WLDT_TEST),//
        WIN_HIDE_STATUS,
	 END_WIN
};
WINDOW_TABLE(MMIWLDT_WATCH_CLASS_SINGLE_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)MmiWldt_OpenClassSingleWinHandleMsg),
        //win_id
        WIN_ID(MMIWLDT_WATCH_CLASS_SINGLE_WIN_ID),
        //窗口的标题
        //WIN_TITLE(TXT_WLDT_TEST),//
        WIN_HIDE_STATUS,
	 END_WIN
};
WINDOW_TABLE(MMIWLDT_WATCH_CLASS_MULTIPLE_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)MmiWldt_OpenClassMultipleWinHandleMsg),
        //win_id
        WIN_ID(MMIWLDT_WATCH_CLASS_MULTIPLE_WIN_ID),
        //窗口的标题
        //WIN_TITLE(TXT_WLDT_TEST),//
        WIN_HIDE_STATUS,
	 END_WIN
};
WINDOW_TABLE(MMIWLDT_WATCH_CLASS_JUDGE_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)MmiWldt_OpenClassJudgeWinHandleMsg),
        //win_id
        WIN_ID(MMIWLDT_WATCH_CLASS_JUDGE_WIN_ID),
        //窗口的标题
        //WIN_TITLE(TXT_WLDT_TEST),//
        WIN_HIDE_STATUS,
	 END_WIN
};

BOOLEAN MMIWLDT_OpenClassWin(void)
{
     MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_MENU_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIWLDT_OpenClassWinFocus(void)
{
    if(MMK_IsOpenWin(MMIWLDT_WATCH_CLASS_SINGLE_WIN_ID))
    {
        MMK_WinGrabFocus(MMIWLDT_WATCH_CLASS_SINGLE_WIN_ID);
    }    
    else if(MMK_IsOpenWin(MMIWLDT_WATCH_CLASS_MULTIPLE_WIN_ID))
    {
        MMK_WinGrabFocus(MMIWLDT_WATCH_CLASS_MULTIPLE_WIN_ID);
    }    
    else if(MMK_IsOpenWin(MMIWLDT_WATCH_CLASS_JUDGE_WIN_ID))
    {
        MMK_WinGrabFocus(MMIWLDT_WATCH_CLASS_JUDGE_WIN_ID);
    }    
    else if(MMK_IsOpenWin(MMIWLDT_WATCH_CLASS_MENU_WIN_ID))
    {
        MMK_WinGrabFocus(MMIWLDT_WATCH_CLASS_MENU_WIN_ID);
    }
    else
    {
         MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_MENU_WIN_TAB,PNULL);
    }    
    return TRUE;
}

BOOLEAN is_press_A = FALSE;
BOOLEAN is_press_B = FALSE;
BOOLEAN is_press_C = FALSE;
BOOLEAN is_press_D = FALSE;
BOOLEAN is_press_E = FALSE;
BOOLEAN is_press_F = FALSE;
BOOLEAN is_press_YES = FALSE;
BOOLEAN is_press_NO = FALSE;
static uint8 is_press_index = 0;
static uint8 is_press_type_index = 0;

LOCAL void MmiWldt_ClassSingleKeySelect(
                                uint8 index
                                )
{
    is_press_A = FALSE;
    is_press_B = FALSE;
    is_press_C = FALSE;
    is_press_D = FALSE;
    if(index == 1)
    {
        is_press_A = TRUE;
    }
    else if(index == 2)
    {
        is_press_B = TRUE;
    }
    else if(index == 3)
    {
        is_press_C = TRUE;
    }
    else if(index == 4)
    {
        is_press_D = TRUE;
    }
}

LOCAL void MmiWldt_ClassSingleKeyUp(
                                MMI_WIN_ID_T    win_id
                                )
{
    is_press_index++;
    if(is_press_index > 4)
    {
        is_press_index = 1;
    }
    MmiWldt_ClassSingleKeySelect(is_press_index);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

LOCAL void MmiWldt_ClassSingleKeyOK(
                                MMI_WIN_ID_T    win_id
                                )
{

    //提交
    if(is_press_A == TRUE || is_press_B == TRUE ||is_press_C==TRUE || is_press_D ==TRUE)
    {
        uint16 result = 0;
        if(is_press_A)
        {
            result |= 0x0001;
        }
        else if(is_press_B)
        {
            result |= 0x0002;
        }
        else if(is_press_C)
        {
            result |= 0x0004;
        }
        else if(is_press_D)
        {
            result |= 0x0008;
        }
        MMIWLDT_HTTP_Send(result);
        is_press_A = FALSE;
        is_press_B = FALSE;
        is_press_C = FALSE;
        is_press_D = FALSE;
        is_press_index = 0;
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
}

 LOCAL void MmiWldt_ClassSingleKeyDown(
                                MMI_WIN_ID_T    win_id
                                )
{
    if(is_press_index > 0)
    {
        is_press_index--;
    }
    if(is_press_index == 0)
    {
        is_press_index = 4;
    }
    MmiWldt_ClassSingleKeySelect(is_press_index);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

LOCAL void MmiWldt_ClassSingleTPUp(
                                GUI_POINT_T     *point_ptr,
                                MMI_WIN_ID_T    win_id
                                )
{
    //WLDT_LOG("[CLASS],MmiWldt_ClassSingleTPUp=point_ptr->x=%d,,point_ptr->y=%d",point_ptr->x,point_ptr->y);          
    if( (point_ptr->x >= 10)
		&& (point_ptr->x <= 110)
		&& (point_ptr->y >= 60)
		&& (point_ptr->y <= 100)
		)//A
    {
		if(is_press_A == TRUE)
		{
			is_press_A = FALSE;
		}
		else
		{
			is_press_A = TRUE;
		}
		is_press_B = FALSE;
		is_press_C = FALSE;
		is_press_D = FALSE;
    }
    else if( (point_ptr->x >= 130)
		&& (point_ptr->x <= 220)
		&& (point_ptr->y >= 60)
		&& (point_ptr->y <= 100)
		)//B
    {
		if(is_press_B == TRUE)
		{
			is_press_B = FALSE;
		}
		else
		{
			is_press_B = TRUE;
		}
		is_press_A = FALSE;
		is_press_C = FALSE;
		is_press_D = FALSE;
    }
    else if( (point_ptr->x >= 10)
		&& (point_ptr->x <= 110)
		&& (point_ptr->y >= 120)
		&& (point_ptr->y <= 160)
		)//C
    {
		if(is_press_C == TRUE)
		{
			is_press_C = FALSE;
		}
		else
		{
			is_press_C = TRUE;
		}
		is_press_A = FALSE;
		is_press_B = FALSE;
		is_press_D = FALSE;
    }
    else if( (point_ptr->x >= 130)
		&& (point_ptr->x <= 220)
		&& (point_ptr->y >= 120)
		&& (point_ptr->y <= 160)
		)//D
    {
		is_press_A = FALSE;
		is_press_B = FALSE;
		is_press_C = FALSE;
		if(is_press_D == TRUE)
		{
			is_press_D = FALSE;
		}
		else
		{
			is_press_D = TRUE;
		}
    }
    else if( (point_ptr->x >= 0)&& (point_ptr->x <= 120)&& (point_ptr->y >= 197)&& (point_ptr->y <=240))
    {
        //返回
	 if(is_press_A == TRUE || is_press_B == TRUE ||is_press_C==TRUE || is_press_D ==TRUE)
	 {
		is_press_A = FALSE;
		is_press_B = FALSE;
		is_press_C = FALSE;
		is_press_D = FALSE;
	 }
       else
       {
		MMK_CloseWin(win_id);
		return;
       }
    }
    else if( (point_ptr->x >= 120)&& (point_ptr->x <= 240)&& (point_ptr->y >= 197)&& (point_ptr->y <=240))
    {
        //提交
	 if(is_press_A == TRUE || is_press_B == TRUE ||is_press_C==TRUE || is_press_D ==TRUE)
	 {
            uint16 result = 0;
            if(is_press_A)
            {
                result = 0x0001;
            }
            else if(is_press_B)
            {
                result = 0x0002;
            }
            else if(is_press_C)
            {
                result = 0x0004;
            }
            else if(is_press_D)
            {
                result = 0x0008;
            }
            MMIWLDT_HTTP_Send(result);
		is_press_A = FALSE;
		is_press_B = FALSE;
		is_press_C = FALSE;
		is_press_D = FALSE;
            //MMK_CloseWin(win_id);
		return;
	 }
    }
	MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
} 

void MmiWldt_OpenClassSingle_Show(MMI_WIN_ID_T    win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   full_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_32;
    GUI_COLOR_T normal_color =MMI_WHITE_COLOR;
    GUI_COLOR_T forcus_color =MMI_DARK_RED_COLOR;
    GUI_COLOR_T normal_bg_color =WLDT_UI_TEXT_BG_COLOR;
    GUI_COLOR_T forcus_bg_color =WLDT_UI_TEXT_BG_FORCUS_COLOR;
    uint16 title_wstr[10] = {0x5355,0x9009,0x9898,0x0};  //  4E92 52A8 7B54 9898 5361 
    uint16 A_wstr[3] = {0x0041,0x0};  
    uint16 B_wstr[3] = {0x0042,0x0}; 
    uint16 C_wstr[3] = {0x0043,0x0};
    uint16 D_wstr[3] = {0x0044,0x0};
    uint16 OK_wstr[3] = {0x63D0,0x4EA4,0x0};  
    uint16 Cancel_wstr[3] = {0x8FD4,0x56DE,0x0};  
    
    GUI_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = WLDT_UI_DEVICE_WIDTH-1;
    rect.bottom = WLDT_UI_DEVICE_HEIGHT-1;
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_rect = rect;
    cur_rect.top    = WLDT_UI_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_TITLE_HEIGHT-1;

    //LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
    cur_str_t.wstr_ptr = title_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //A
    cur_rect.left = WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;
    text_style.font = SONG_FONT_36;
    if(is_press_A)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
        text_style.font_color = forcus_color;
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
        text_style.font_color = normal_color;
    }
    cur_str_t.wstr_ptr = A_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    //B
    cur_rect.left    = WLDT_UI_DEVICE_WIDTH/2+WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 

    if(is_press_B)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
        text_style.font_color = forcus_color;
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = B_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    
    //C
    cur_rect.left = WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;
    
    if(is_press_C)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
        text_style.font_color = forcus_color;
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = C_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //D
    cur_rect.left    = WLDT_UI_DEVICE_WIDTH/2+WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 

    if(is_press_D)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
        text_style.font_color = forcus_color;
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = D_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //返回
    cur_rect.left = WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.bottom = WLDT_UI_DEVICE_HEIGHT-3;
    cur_rect.top    = cur_rect.bottom-WLDT_UI_ITEM_HEIGHT+1; 
    text_style.font_color = normal_color;
#if 0
    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);

    cur_str_t.wstr_ptr = Cancel_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
#endif
    //提交
    cur_rect.left    = WLDT_UI_DEVICE_WIDTH/2+WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.bottom = WLDT_UI_DEVICE_HEIGHT-3;
    cur_rect.top    = cur_rect.bottom-WLDT_UI_ITEM_HEIGHT+1; 

    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);

    cur_str_t.wstr_ptr = OK_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    return;
}

LOCAL MMI_RESULT_E MmiWldt_OpenClassSingleWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T client_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
 GUISTR_STYLE_T          text_style = {0};
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T            text_tip1 = {0};
    MMI_STRING_T            text_tip2 = {0};	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 is_press_A = FALSE;
		 is_press_B = FALSE;
		 is_press_C = FALSE;
		 is_press_D = FALSE;
		 is_press_E = FALSE;
		 is_press_F = FALSE;
             is_press_index = 0;
        break;

	case MSG_FULL_PAINT:
		MmiWldt_OpenClassSingle_Show(win_id);
		break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            MmiWldt_ClassSingleTPUp(&point,win_id);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif

    case MSG_KEYDOWN_DOWN:
            MmiWldt_ClassSingleKeyDown(win_id);
        break;
        
    case MSG_KEYDOWN_UP:
            MmiWldt_ClassSingleKeyUp(win_id);
        break;
        
    case MSG_KEYDOWN_OK:
            MmiWldt_ClassSingleKeyOK(win_id);
        break;
        
    case MSG_GET_FOCUS: 
        break;    
    case MSG_LOSE_FOCUS:        
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;

}

LOCAL void MmiWldt_ClassMultipleKeySelect(
                                MMI_WIN_ID_T    win_id
                                )
{
    if(is_press_index == 1)
    {
        if(is_press_A)
        {
            is_press_A = FALSE;
        }
        else
        {
            is_press_A = TRUE;
        }
    }
    else if(is_press_index == 2)
    {
        if(is_press_B)
        {
            is_press_B = FALSE;
        }
        else
        {
            is_press_B = TRUE;
        }
    }
    else if(is_press_index == 3)
    {
        if(is_press_C)
        {
            is_press_C = FALSE;
        }
        else
        {
            is_press_C = TRUE;
        }
    }
    else if(is_press_index == 4)
    {
        if(is_press_D)
        {
            is_press_D = FALSE;
        }
        else
        {
            is_press_D = TRUE;
        }
    }
    else if(is_press_index == 5)
    {
        if(is_press_E)
        {
            is_press_E = FALSE;
        }
        else
        {
            is_press_E = TRUE;
        }
    }
    else if(is_press_index == 6)
    {
        if(is_press_F)
        {
            is_press_F = FALSE;
        }
        else
        {
            is_press_F = TRUE;
        }
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

LOCAL void MmiWldt_ClassMultipleKeyUp(
                                MMI_WIN_ID_T    win_id
                                )
{
    is_press_index++;
    if(is_press_index > 6)
    {
        is_press_index = 1;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

LOCAL void MmiWldt_ClassMultipleKeyOK(
                                MMI_WIN_ID_T    win_id
                                )
{
    //提交
    if(is_press_A == TRUE || is_press_B == TRUE ||is_press_C==TRUE || is_press_D ==TRUE ||is_press_E==TRUE || is_press_F ==TRUE)
    {
        uint16 result = 0;
        if(is_press_A)
        {
            result |= 0x0001;
        }
        if(is_press_B)
        {
            result |= 0x0002;
        }
        if(is_press_C)
        {
            result |= 0x0004;
        }
        if(is_press_D)
        {
            result |= 0x0008;
        }
        if(is_press_E)
        {
            result |= 0x0010;
        }
        if(is_press_F)
        {
            result |= 0x0020;
        }
        MMIWLDT_HTTP_Send(result);
        is_press_A = FALSE;
        is_press_B = FALSE;
        is_press_C = FALSE;
        is_press_D = FALSE;
        is_press_E = FALSE;
        is_press_F = FALSE;
        is_press_index = 0;
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    return;
}

 LOCAL void MmiWldt_ClassMultipleKeyDown(
                                MMI_WIN_ID_T    win_id
                                )
{
    if(is_press_index > 0)
    {
        is_press_index--;
    }
    if(is_press_index == 0)
    {
        is_press_index = 6;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}


 LOCAL void MmiWldt_ClassMultipleTPUp(
								 GUI_POINT_T	 *point_ptr,
								 MMI_WIN_ID_T	 win_id
								 )
{
    //WLDT_LOG("[CLASS],MmiWldt_ClassMultipleTPUp=point_ptr->x=%d,,point_ptr->y=%d",point_ptr->x,point_ptr->y);          
    if( (point_ptr->x >= 15)
		&& (point_ptr->x <= 75)
		&& (point_ptr->y >= 60)
		&& (point_ptr->y <= 100)
		)//A
    {
		if(is_press_A == TRUE)
		{
			is_press_A = FALSE;
		}
		else
		{
			is_press_A = TRUE;
		}
    }
    else if( (point_ptr->x >= 90)
		&& (point_ptr->x <= 150)
		&& (point_ptr->y >= 60)
		&& (point_ptr->y <= 100)
		)//B
    {
		if(is_press_B == TRUE)
		{
			is_press_B = FALSE;
		}
		else
		{
			is_press_B = TRUE;
		}
    }
    else if( (point_ptr->x >= 165)
		&& (point_ptr->x <= 225)
		&& (point_ptr->y >= 60)
		&& (point_ptr->y <= 100)
		)//C
    {
		if(is_press_C == TRUE)
		{
			is_press_C = FALSE;
		}
		else
		{
			is_press_C = TRUE;
		}
    }
    else if( (point_ptr->x >= 15)
		&& (point_ptr->x <= 75)
		&& (point_ptr->y >= 120)
		&& (point_ptr->y <= 160)
		)//D
    {
		if(is_press_D == TRUE)
		{
			is_press_D = FALSE;
		}
		else
		{
			is_press_D = TRUE;
		}
    }
   else if( (point_ptr->x >= 90)
		&& (point_ptr->x <= 150)
		&& (point_ptr->y >= 120)
		&& (point_ptr->y <= 160)
		)//E
    {
		if(is_press_E == TRUE)
		{
			is_press_E = FALSE;
		}
		else
		{
			is_press_E = TRUE;
		}
    }
    else if( (point_ptr->x >= 165)
		&& (point_ptr->x <= 225)
		&& (point_ptr->y >= 120)
		&& (point_ptr->y <= 160)
		)//F
    {
		if(is_press_F == TRUE)
		{
			is_press_F = FALSE;
		}
		else
		{
			is_press_F = TRUE;
		}
    }
    else if( (point_ptr->x >= 0)&& (point_ptr->x <= 120)&& (point_ptr->y >= 197)&& (point_ptr->y <=240))
    {
        //返回
	 if(is_press_A == TRUE || is_press_B == TRUE ||is_press_C==TRUE || is_press_D ==TRUE ||is_press_E==TRUE || is_press_F ==TRUE)
	 {
		is_press_A = FALSE;
		is_press_B = FALSE;
		is_press_C = FALSE;
		is_press_D = FALSE;
		is_press_E = FALSE;
		is_press_F = FALSE;
	 }
       else
       {
		MMK_CloseWin(win_id);
		return;
       }
    }
    else if( (point_ptr->x >= 120)&& (point_ptr->x <= 240)&& (point_ptr->y >= 197)&& (point_ptr->y <=240))
    {
        //提交
	 if(is_press_A == TRUE || is_press_B == TRUE ||is_press_C==TRUE || is_press_D ==TRUE ||is_press_E==TRUE || is_press_F ==TRUE)
	 {
            uint16 result = 0;
            if(is_press_A)
            {
                result |= 0x0001;
            }
            if(is_press_B)
            {
                result |= 0x0002;
            }
            if(is_press_C)
            {
                result |= 0x0004;
            }
            if(is_press_D)
            {
                result |= 0x0008;
            }
            if(is_press_E)
            {
                result |= 0x0010;
            }
            if(is_press_F)
            {
                result |= 0x0020;
            }
            MMIWLDT_HTTP_Send(result);
		is_press_A = FALSE;
		is_press_B = FALSE;
		is_press_C = FALSE;
		is_press_D = FALSE;
		is_press_E = FALSE;
		is_press_F = FALSE;
            //MMK_CloseWin(win_id);
		return;
	 }
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
} 

void MmiWldt_OpenClassMultiple_Show(MMI_WIN_ID_T    win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   full_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_32;
    GUI_COLOR_T normal_color =MMI_WHITE_COLOR;
    GUI_COLOR_T forcus_color =MMI_DARK_RED_COLOR;
    GUI_COLOR_T normal_bg_color =WLDT_UI_TEXT_BG_COLOR;
    GUI_COLOR_T forcus_bg_color =WLDT_UI_TEXT_BG_FORCUS_COLOR;
    uint16 title_wstr[10] = {0x591A,0x9009,0x9898,0x0};  //  4E92 52A8 7B54 9898 5361 
    uint16 A_wstr[3] = {0x0041,0x0};  
    uint16 B_wstr[3] = {0x0042,0x0}; 
    uint16 C_wstr[3] = {0x0043,0x0};
    uint16 D_wstr[3] = {0x0044,0x0};
    uint16 E_wstr[3] = {0x0045,0x0};
    uint16 F_wstr[3] = {0x0046,0x0};
    uint16 OK_wstr[3] = {0x63D0,0x4EA4,0x0};  
    uint16 Cancel_wstr[3] = {0x8FD4,0x56DE,0x0};  
    
    GUI_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = WLDT_UI_DEVICE_WIDTH-1;
    rect.bottom = WLDT_UI_DEVICE_HEIGHT-1;
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_rect = rect;
    cur_rect.top    = WLDT_UI_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_TITLE_HEIGHT-1;

    //LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
    cur_str_t.wstr_ptr = title_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //A
    cur_rect.left = WLDT_UI_OFFSET_X_2; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH_2-1; 
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;
    text_style.font = SONG_FONT_36;
    if(is_press_index == 1)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
    }
    if(is_press_A)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }
    cur_str_t.wstr_ptr = A_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    //B
    cur_rect.left    = cur_rect.right+WLDT_UI_OFFSET_X_2; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH_2-1; 
    if(is_press_index == 2)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
    }
    if(is_press_B)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = B_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //C
    cur_rect.left    = cur_rect.right+WLDT_UI_OFFSET_X_2; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH_2-1; 
    if(is_press_index == 3)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
    }
    if(is_press_C)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = C_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    //D
    cur_rect.left = WLDT_UI_OFFSET_X_2; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH_2-1; 
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;
    if(is_press_index == 4)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
    }
    if(is_press_D)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = D_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //E
    cur_rect.left    = cur_rect.right+WLDT_UI_OFFSET_X_2; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH_2-1; 
    if(is_press_index == 5)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
    }
    if(is_press_E)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = E_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //F
    cur_rect.left    = cur_rect.right+WLDT_UI_OFFSET_X_2; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH_2-1; 
    if(is_press_index == 6)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
    }
    if(is_press_F)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = F_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //返回
    cur_rect.left = WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.bottom = WLDT_UI_DEVICE_HEIGHT-3;
    cur_rect.top    = cur_rect.bottom-WLDT_UI_ITEM_HEIGHT+1; 
    text_style.font_color = normal_color;
    #if 0
    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);

    cur_str_t.wstr_ptr = Cancel_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    #endif
    
    //提交
    cur_rect.left    = WLDT_UI_DEVICE_WIDTH/2+WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.bottom = WLDT_UI_DEVICE_HEIGHT-3;
    cur_rect.top    = cur_rect.bottom-WLDT_UI_ITEM_HEIGHT+1; 

    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);

    cur_str_t.wstr_ptr = OK_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    return;
}

LOCAL MMI_RESULT_E MmiWldt_OpenClassMultipleWinHandleMsg(
		 MMI_WIN_ID_T		win_id,
		 MMI_MESSAGE_ID_E	msg_id,
		 DPARAM 			param
		 )
 {
	 MMI_RESULT_E				 recode 		 =	 MMI_RESULT_TRUE;
	 GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	 GUI_RECT_T client_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
	 //GUI_RECT_T img_button_left_rect = {MMIWLDT_SET_LEFT_BUTTON_LEFT, MMIWLDT_SET_BUTTON_BG_TOP, MMIWLDT_SET_LEFT_BUTTON_LEFT + MMIWLDT_SET_BUTTON_WIDTH -1, MMIWLDT_SET_BUTTON_BG_TOP + MMIWLDT_SET_BUTTON_HEIGHT-1};
	 //GUI_RECT_T img_button_right_rect = {MMIWLDT_SET_RIGHT_BUTTON_LEFT, MMIWLDT_SET_BUTTON_BG_TOP, MMIWLDT_SET_RIGHT_BUTTON_LEFT + MMIWLDT_SET_BUTTON_WIDTH -1, MMIWLDT_SET_BUTTON_BG_TOP + MMIWLDT_SET_BUTTON_HEIGHT-1};  
	 GUISTR_STYLE_T 		 text_style = {0};
	 GUISTR_STATE_T 		 text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
	 MMI_STRING_T			 text_tip1 = {0};
	 MMI_STRING_T			 text_tip2 = {0};	 
	 switch(msg_id)
	 {
	 case MSG_OPEN_WINDOW:
		 is_press_A = FALSE;
		 is_press_B = FALSE;
		 is_press_C = FALSE;
		 is_press_D = FALSE;
		 is_press_E = FALSE;
		 is_press_F = FALSE;
             is_press_index = 0;
		 break;
 
	 case MSG_FULL_PAINT:
		 MmiWldt_OpenClassMultiple_Show(win_id);
		 break;
 
#ifdef TOUCH_PANEL_SUPPORT
	 case MSG_TP_PRESS_DOWN:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
		 }
		 break;
		 
	 case MSG_TP_PRESS_UP:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
			 MmiWldt_ClassMultipleTPUp(&point,win_id);
		 }
		 break;
		 
	 case MSG_TP_PRESS_MOVE:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
		 }
		 break;
	 case MSG_TP_PRESS_LONG:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
		 }
		 break;
#endif
    case MSG_KEYDOWN_DOWN:
            MmiWldt_ClassMultipleKeyDown(win_id);
        break;
        
    case MSG_KEYDOWN_UP:
            MmiWldt_ClassMultipleKeyUp(win_id);
        break;
        
    case MSG_KEYDOWN_OK:
            MmiWldt_ClassMultipleKeyOK(win_id);
        break;
        
    case MSG_KEYDOWN_GREEN:
            MmiWldt_ClassMultipleKeySelect(win_id);
        break;
        
	 case MSG_GET_FOCUS: 
	        break;    
	 case MSG_LOSE_FOCUS:        
	        break;
	 case MSG_CTL_CANCEL:
	 case MSG_APP_CANCEL:
		 MMK_CloseWin(win_id);
		 break;
 
	 default:
		 recode = MMI_RESULT_FALSE;
		 break;
	 }
	 return recode;
 
 }

LOCAL void MmiWldt_ClassJudgeKeyUp(
                                MMI_WIN_ID_T    win_id
                                )
{
    if(is_press_YES)
    {
        is_press_YES = FALSE;
        is_press_NO = TRUE;
    }
    else if(is_press_NO)
    {
        is_press_YES = TRUE;
        is_press_NO = FALSE;
    }
    else
    {
        is_press_YES = TRUE;
        is_press_NO = FALSE;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

LOCAL void MmiWldt_ClassJudgeKeyOK(
                                MMI_WIN_ID_T    win_id
                                )
{
    //提交
 if(is_press_YES == TRUE || is_press_NO == TRUE)
 {
        uint16 result = 0;
        if(is_press_YES)
        {
            result = 0x0200;
        }
        else if(is_press_NO)
        {
            result = 0x0100;
        }
        MMIWLDT_HTTP_Send(result);
        is_press_YES = FALSE;
        is_press_NO = FALSE;
       MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
	return;
 }
}

 LOCAL void MmiWldt_ClassJudgeKeyDown(
                                MMI_WIN_ID_T    win_id
                                )
{
    if(is_press_YES)
    {
        is_press_YES = FALSE;
        is_press_NO = TRUE;
    }
    else if(is_press_NO)
    {
        is_press_YES = TRUE;
        is_press_NO = FALSE;
    }
    else
    {
        is_press_YES = FALSE;
        is_press_NO = TRUE;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

 LOCAL void MmiWldt_ClassJudgeTPUp(
                                GUI_POINT_T     *point_ptr,
                                MMI_WIN_ID_T    win_id
                                )
{
    //WLDT_LOG("[CLASS],MmiWldt_ClassJudgeTPUp=point_ptr->x=%d,,point_ptr->y=%d",point_ptr->x,point_ptr->y);          
    if( (point_ptr->x >= 0)
		&& (point_ptr->x <= 240)
		&& (point_ptr->y > WLDT_UI_TITLE_HEIGHT)
		&& (point_ptr->y <= 110)
		)//YES
    {
		if(is_press_YES == TRUE)
		{
			is_press_YES = FALSE;
		}
		else
		{
			is_press_YES = TRUE;
		}
		is_press_NO = FALSE;
    }
    else if( (point_ptr->x >= 0)
		&& (point_ptr->x <= 240)
		&& (point_ptr->y > 110)
		&& (point_ptr->y <= 170)
		)//NO
    {
		if(is_press_NO == TRUE)
		{
			is_press_NO = FALSE;
		}
		else
		{
			is_press_NO = TRUE;
		}
		is_press_YES = FALSE;
    }
    else if( (point_ptr->x >= 0)&& (point_ptr->x <= 120)&& (point_ptr->y >= 197)&& (point_ptr->y <=240))
    {
        //返回
	 if(is_press_YES == TRUE || is_press_NO == TRUE)
	 {
		is_press_YES = FALSE;
		is_press_NO = FALSE;
	 }
       else
       {
		MMK_CloseWin(win_id);
		return;
       }
    }
    else if( (point_ptr->x >= 120)&& (point_ptr->x <= 240)&& (point_ptr->y >= 197)&& (point_ptr->y <=240))
    {
        //提交
	 if(is_press_YES == TRUE || is_press_NO == TRUE)
	 {
            uint16 result = 0;
            if(is_press_YES)
            {
                result = 0x0200;
            }
            else if(is_press_NO)
            {
                result = 0x0100;
            }
            MMIWLDT_HTTP_Send(result);
		is_press_YES = FALSE;
		is_press_NO = FALSE;
            //MMK_CloseWin(win_id);
		return;
	 }
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
} 

void MmiWldt_OpenClassJudge_Show(MMI_WIN_ID_T    win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   full_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_32;
    GUI_COLOR_T normal_color =MMI_WHITE_COLOR;
    GUI_COLOR_T forcus_color =MMI_DARK_RED_COLOR;
    GUI_COLOR_T normal_bg_color =WLDT_UI_TEXT_BG_COLOR;
    GUI_COLOR_T forcus_bg_color =WLDT_UI_TEXT_BG_FORCUS_COLOR;
    uint16 title_wstr[10] = {0x5224,0x65AD,0x9898,0x0};  //  4E92 52A8 7B54 9898 5361 
    uint16 yes_wstr[3] = {0x5BF9,0x0};  
    uint16 no_wstr[3] = {0x9519 ,0x0}; 
    uint16 OK_wstr[3] = {0x63D0,0x4EA4,0x0};  
    uint16 Cancel_wstr[3] = {0x8FD4,0x56DE,0x0};  
    
    GUI_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = WLDT_UI_DEVICE_WIDTH-1;
    rect.bottom = WLDT_UI_DEVICE_HEIGHT-1;
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_rect = rect;
    cur_rect.top    = WLDT_UI_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_TITLE_HEIGHT-1;

    //LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
    cur_str_t.wstr_ptr = title_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //Yes
    cur_rect.left = WLDT_UI_OFFSET_X; 
    cur_rect.right = WLDT_UI_DEVICE_WIDTH-WLDT_UI_OFFSET_X; 
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;
    text_style.font = SONG_FONT_36;
    if(is_press_YES)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
        text_style.font_color = forcus_color;
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
        text_style.font_color = normal_color;
    }
    cur_str_t.wstr_ptr = yes_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    

    
    //C
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;
    
    if(is_press_NO)
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, forcus_bg_color);
        text_style.font_color = forcus_color;
    }
    else
    {
        LCD_FillRect(&lcd_dev_info, cur_rect, normal_bg_color);
        text_style.font_color = normal_color;
    }

    cur_str_t.wstr_ptr = no_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    //返回
    cur_rect.left = WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.bottom = WLDT_UI_DEVICE_HEIGHT-3;
    cur_rect.top    = cur_rect.bottom-WLDT_UI_ITEM_HEIGHT+1; 
    text_style.font_color = normal_color;
#if 0
    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);

    cur_str_t.wstr_ptr = Cancel_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
#endif
    //提交
    cur_rect.left    = WLDT_UI_DEVICE_WIDTH/2+WLDT_UI_OFFSET_X; 
    cur_rect.right = cur_rect.left+WLDT_UI_ITEM_WIDTH-1; 
    cur_rect.bottom = WLDT_UI_DEVICE_HEIGHT-3;
    cur_rect.top    = cur_rect.bottom-WLDT_UI_ITEM_HEIGHT+1; 

    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);

    cur_str_t.wstr_ptr = OK_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    return;
}

LOCAL MMI_RESULT_E MmiWldt_OpenClassJudgeWinHandleMsg(
		 MMI_WIN_ID_T		win_id,
		 MMI_MESSAGE_ID_E	msg_id,
		 DPARAM 			param
		 )
 {
	 MMI_RESULT_E				 recode 		 =	 MMI_RESULT_TRUE;
	 GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	 GUI_RECT_T client_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
	 //GUI_RECT_T img_button_left_rect = {MMIWLDT_SET_LEFT_BUTTON_LEFT, MMIWLDT_SET_BUTTON_BG_TOP, MMIWLDT_SET_LEFT_BUTTON_LEFT + MMIWLDT_SET_BUTTON_WIDTH -1, MMIWLDT_SET_BUTTON_BG_TOP + MMIWLDT_SET_BUTTON_HEIGHT-1};
	 //GUI_RECT_T img_button_right_rect = {MMIWLDT_SET_RIGHT_BUTTON_LEFT, MMIWLDT_SET_BUTTON_BG_TOP, MMIWLDT_SET_RIGHT_BUTTON_LEFT + MMIWLDT_SET_BUTTON_WIDTH -1, MMIWLDT_SET_BUTTON_BG_TOP + MMIWLDT_SET_BUTTON_HEIGHT-1};  
	 GUISTR_STYLE_T 		 text_style = {0};
	 GUISTR_STATE_T 		 text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
	 MMI_STRING_T			 text_tip1 = {0};
	 MMI_STRING_T			 text_tip2 = {0};	 
	 switch(msg_id)
	 {
	 case MSG_OPEN_WINDOW:
		 is_press_YES = FALSE;
		 is_press_NO = FALSE;
		 break;
 
	 case MSG_FULL_PAINT:
		 MmiWldt_OpenClassJudge_Show(win_id);		 
		 break;
 
#ifdef TOUCH_PANEL_SUPPORT
	 case MSG_TP_PRESS_DOWN:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
		 }
		 break;
		 
	 case MSG_TP_PRESS_UP:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
			 MmiWldt_ClassJudgeTPUp(&point,win_id);
		 }
		 break;
		 
	 case MSG_TP_PRESS_MOVE:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
		 }
		 break;
	 case MSG_TP_PRESS_LONG:
		 {
			 GUI_POINT_T   point = {0};
			 point.x = MMK_GET_TP_X(param);
			 point.y = MMK_GET_TP_Y(param);
		 }
		 break;
#endif
    case MSG_KEYDOWN_DOWN:
            MmiWldt_ClassJudgeKeyDown(win_id);
        break;
        
    case MSG_KEYDOWN_UP:
            MmiWldt_ClassJudgeKeyUp(win_id);
        break;
        
    case MSG_KEYDOWN_OK:
            MmiWldt_ClassJudgeKeyOK(win_id);
        break;
        
        case MSG_GET_FOCUS: 
               break;    
        case MSG_LOSE_FOCUS:        
               break;
	 case MSG_CTL_CANCEL:
	 case MSG_APP_CANCEL:
		 MMK_CloseWin(win_id);
		 break;
 
	 default:
		 recode = MMI_RESULT_FALSE;
		 break;
	 }
	 return recode;
 
 }

LOCAL void MmiWldt_ClassMenuKeyUp(
                                MMI_WIN_ID_T    win_id
                                )
{
    if(is_press_type_index > 0)
    {
        is_press_type_index--;
    }
    if(is_press_type_index == 0)
    {
        is_press_type_index = 3;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

LOCAL void MmiWldt_ClassMenuKeyOK(
                                MMI_WIN_ID_T    win_id
                                )
{

    //提交
    if(is_press_type_index == 1)
    {
	   MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_SINGLE_WIN_TAB, PNULL);
    }
    else if(is_press_type_index == 2)
    {
	   MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_MULTIPLE_WIN_TAB, PNULL);
    }
    else if(is_press_type_index == 3)
    {
	   MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_JUDGE_WIN_TAB, PNULL); 
    }
}

 LOCAL void MmiWldt_ClassMenuKeyDown(
                                MMI_WIN_ID_T    win_id
                                )
{
    is_press_type_index++;
    if(is_press_type_index > 3)
    {
        is_press_type_index = 1;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

void MmiWldt_OpenClassMenu_Show(MMI_WIN_ID_T    win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   full_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_32;
    uint16 title_wstr[10] = {0x4E92,0x52A8,0x7B54,0x9898,0x5361,0x0};  //  4E92 52A8 7B54 9898 5361 
    uint16 s1_wstr[10] = {0x5355,0x9009,0x9898,0x0};  //  5355 9009 9898 
    uint16 s2_wstr[10] = {0x591A,0x9009,0x9898,0x0};  //  591A 9009 9898 
    uint16 s3_wstr[10] = {0x5224,0x65AD,0x9898,0x0};  //  5224 65AD 9898 
    GUI_COLOR_T normal_color =MMI_WHITE_COLOR;
    GUI_COLOR_T forcus_color =MMI_DARK_RED_COLOR;
    GUI_COLOR_T normal_bg_color =WLDT_UI_TEXT_BG_COLOR;
    GUI_COLOR_T forcus_bg_color =WLDT_UI_TEXT_BG_COLOR;
    
    GUI_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = WLDT_UI_DEVICE_WIDTH-1;
    rect.bottom = WLDT_UI_DEVICE_HEIGHT-1;
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = normal_color;

    cur_rect = rect;
    cur_rect.top    = WLDT_UI_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_TITLE_HEIGHT-1;

    //LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
    cur_str_t.wstr_ptr = title_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    cur_rect.left = WLDT_UI_OFFSET_X; 
    cur_rect.right = WLDT_UI_DEVICE_WIDTH-WLDT_UI_OFFSET_X; 
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, cur_rect, WLDT_UI_TEXT_BG_COLOR);

    cur_str_t.wstr_ptr = s1_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    if(is_press_type_index == 1)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, cur_rect, WLDT_UI_TEXT_BG_COLOR);

    cur_str_t.wstr_ptr = s2_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    if(is_press_type_index == 2)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    cur_rect.top    = cur_rect.bottom+WLDT_UI_OFFSET_Y; 
    cur_rect.bottom = cur_rect.top + WLDT_UI_ITEM_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, cur_rect, WLDT_UI_TEXT_BG_COLOR);

    cur_str_t.wstr_ptr = s3_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    if(is_press_type_index == 3)
    {
        text_style.font_color = forcus_color;
    }
    else
    {
        text_style.font_color = normal_color;
    }
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    //MMITHEME_UpdateRect();
    return;
}
 
LOCAL MMI_RESULT_E MmiWldt_OpenClassMenuWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T client_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
    GUISTR_STYLE_T          text_style = {0};
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T            text_tip1 = {0};
    MMI_STRING_T            text_tip2 = {0};	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
             is_press_type_index = 1;
		//WLDT_CY_SetNetOpen();
        break;

	case MSG_FULL_PAINT:
                MmiWldt_OpenClassMenu_Show(win_id);
		break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
		if( (point.y > WLDT_UI_TITLE_HEIGHT) && (point.y <= 110)
			&& (point.x > 0)&& (point.x < 240)
			)
		{    
		   MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_SINGLE_WIN_TAB, PNULL);
		}
		 if( (point.y > 110) && (point.y <= 170)
			&& (point.x > 0)&& (point.x < 240)
			)
		{
		   MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_MULTIPLE_WIN_TAB, PNULL);
		}
		 if( (point.y > 170) && (point.y < 240)
			&& (point.x > 0)&& (point.x < 240)
			)
		{
		   MMK_CreateWin((uint32*)MMIWLDT_WATCH_CLASS_JUDGE_WIN_TAB, PNULL); 
		}   
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif
    case MSG_KEYDOWN_DOWN:
            MmiWldt_ClassMenuKeyDown(win_id);
        break;
        
    case MSG_KEYDOWN_UP:
            MmiWldt_ClassMenuKeyUp(win_id);
        break;
        
    case MSG_KEYDOWN_OK:
            MmiWldt_ClassMenuKeyOK(win_id);
        break;
        
    case MSG_GET_FOCUS: 
        break;
        
    case MSG_LOSE_FOCUS:        
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        //WLDT_CY_SetNetClose();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;

}
#endif
