/*****************************************************************************
** File Name:      sample_im.c                                               *
** Author:                                                                   *
** Date:           2009/07/21                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to test input method ctrl               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 7/2009         Lianxiang.Zhou    Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "std_header.h"

#ifdef MMI_MOCOR_SAMPLE

#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "sample_main.h"
#include "guifont.h"
#include "mmi_textfun.h"
#include "sample_text.h"
#include "sample_image.h"
#include "sample_menutable.h"
#include "sample_id.h"
#include "sample_nv.h"
#include "mmitheme_pos.h"
#include "guires.h"

#include "guiim.h"
#include "guistring.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define SAMPLE_IM_BUFFER_LEN (24)

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : im ctrl测试的回调函数
//  Global resource dependence :
//  Author: Lianxiang.Zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestImWinMsg(
                                      MMI_WIN_ID_T     win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM           param
                                      );


LOCAL void ShowTips(wchar *str_ptr);
LOCAL void UpdateInput(GUIIM_NOTIFY_PARAM_T *data_ptr);
/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// im控件测试窗口
WINDOW_TABLE( MMI_TEST_IM_WIN_TAB ) =
{
    WIN_ID( MMI_TEST_RICHTEXT_WIN_ID ),
    WIN_FUNC((uint32)HandleTestImWinMsg ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建im测试窗口
//  Global resource dependence :
//  Author: Lianxiang.Zhou
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateImWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_IM_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : im控件测试窗口的回调函数
//  Global resource dependence :
//  Author: Lianxiang.Zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestImWinMsg(
                                      MMI_WIN_ID_T     win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM           param
                                      )
{
    static IGUICTRL_T *im_handle = PNULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T full_rect = MMITHEME_GetClientRect();
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        if(PNULL != im_handle)
        {
            MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T *)im_handle));
            im_handle = PNULL;
        }
        break;

    case MSG_FULL_PAINT:
    case MSG_NOTIFY_IM_PAINT:
    {
        MMI_STRING_T string = {0};
        GUI_POINT_T disp_point = {0};// 1 ,MMI_TITLE_HEIGHT + 10};
        wchar tip_txt[] = L"Press OK to Test Input Method Controler.";
        GUISTR_STYLE_T      text_style      = {0};
        GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
        
		disp_point.x = 1;
		disp_point.y = MMI_TITLE_HEIGHT + 10;

        string.wstr_ptr = tip_txt;
        string.wstr_len = MMIAPICOM_Wstrlen(tip_txt);
        GUI_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);

        text_style.font = SONG_FONT_12;
        text_style.font_color = MMI_GRAY_WHITE_COLOR;
        GUISTR_DrawTextToLCDSingleLine( 
                            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                            (const GUI_POINT_T      *)&disp_point,
                            (const MMI_STRING_T     *)&string,
                            (GUISTR_STYLE_T         *)&text_style,
                            state
                            );
        
        break;
    }

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_OK:
        if(PNULL == im_handle)
        {
            MMI_CONTROL_CREATE_T create = {0};
            GUIIM_INIT_PARAM_T init_param = {0};

            init_param.input_type = GUIIM_INPUT_TYPE_TOUCHPEN;
            init_param.im_type_set = GUIIM_TYPE_SET_ALL;
            init_param.im_type = GUIIM_TYPE_DEFAULT;
            init_param.cap_mode = GUIIM_MODE_LEAD_UPPER;
            //init_param.kernel_param.custom_data = ??
            
            create.ctrl_id = 0;
            create.guid = SPRD_GUI_IM_ID;
            create.parent_win_handle = win_id;
            create.parent_ctrl_handle = 0;
            create.init_data_ptr = &init_param;
            
            im_handle = MMK_CreateControl( &create );

            MMK_SetActiveCtrl(GUICTRL_GetCtrlHandle((IGUICTRL_T *)im_handle), FALSE);

            ShowTips(L"The im ctrl is created!");
        }
        else
        {
            ShowTips(L"The im ctrl is already created!");
        }

        break;

    case MSG_NOTIFY_IM_COMMIT:
        UpdateInput(param);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


LOCAL void ShowTips(wchar *str_ptr)
{
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
    MMI_STRING_T        string = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T tip_rect = {0};

	tip_rect.left = 10;
	tip_rect.top = MMI_TITLE_HEIGHT + 50 + 10;
	tip_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 1;
	tip_rect.bottom = MMI_TITLE_HEIGHT + 50 + 10 + 60;

    GUI_FillRect(&lcd_dev_info, tip_rect, MMI_BACKGROUND_COLOR);
    
    
    string.wstr_ptr = str_ptr;
    string.wstr_len = MMIAPICOM_Wstrlen(str_ptr);
    
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    text_style.font_color = MMI_GRAY_WHITE_COLOR;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&tip_rect,       //the fixed display area
        (const GUI_RECT_T      *)&tip_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        state,
        -1
        );

    return;
}

LOCAL void UpdateInput(GUIIM_NOTIFY_PARAM_T *data_ptr)
{
    GUI_FONT_T const font = MMI_DEFAULT_SMALL_FONT;
    GUI_LCD_DEV_INFO const lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T tip_rect = {0};
    static wchar buffer[SAMPLE_IM_BUFFER_LEN + 1] = {0};
    static size_t buffer_len = 0;
    GUI_RECT_T underline_disp_rect = {0};
    GUI_POINT_T dummy_point = {0};
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
    MMI_STRING_T        string = {0};

	tip_rect.left = 10;
	tip_rect.top = MMI_TITLE_HEIGHT + 50 + 10;
	tip_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 10;
	tip_rect.bottom = MMI_TITLE_HEIGHT + 50 + 10 + 60;

    if(data_ptr->commit_len)
    {
        // -1留点缓冲，简化处理
        if(buffer_len + data_ptr->commit_len > SAMPLE_IM_BUFFER_LEN - 1)
        {
            buffer[0] = '\0';
            buffer_len = 0;
        }
        
        MMIAPICOM_Wstrcat(buffer, data_ptr->commit);
        buffer_len += data_ptr->commit_len;
        
        GUI_FillRect(&lcd_dev_info, tip_rect, MMI_BACKGROUND_COLOR);
        string.wstr_ptr = buffer;
        string.wstr_len = MMIAPICOM_Wstrlen(buffer_len);
        
        text_style.font = font;
        text_style.font_color = MMI_GRAY_WHITE_COLOR;
        
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&tip_rect,       //the fixed display area
            (const GUI_RECT_T      *)&tip_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T     *)&string,
            (GUISTR_STYLE_T         *)&text_style,
            state,
            -1
            );
    }

    if(data_ptr->underline_len)
    {
        if(buffer_len + data_ptr->underline_len > SAMPLE_IM_BUFFER_LEN)
        {
            buffer[0] = '\0';
            buffer_len = 0;

            GUI_FillRect(&lcd_dev_info, tip_rect, MMI_BACKGROUND_COLOR);
        }

        underline_disp_rect = tip_rect;
        underline_disp_rect.left += GUI_GetStringWidth(font, buffer, buffer_len);
        underline_disp_rect.right = underline_disp_rect.left + GUI_GetStringWidth(font, data_ptr->underline, data_ptr->underline_len);
        underline_disp_rect.bottom = underline_disp_rect.top + GUI_GetFontHeight(font, UNICODE_ASCII);

        state |= GUISTR_STATE_ALIGN;
        text_style.align = ALIGN_LEFT;
        state |= GUISTR_STATE_EFFECT;
        text_style.effect = FONT_EFFECT_REVERSE_EDGE;
        string.wstr_len = data_ptr->underline_len;
        string.wstr_ptr = data_ptr->underline;
        text_style.font = font;
        text_style.font_color = MMI_GRAY_WHITE_COLOR;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&underline_disp_rect,       //the fixed display area
            (const GUI_RECT_T      *)&underline_disp_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T     *)&string,
            (GUISTR_STYLE_T         *)&text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }

    return;
}



#endif

