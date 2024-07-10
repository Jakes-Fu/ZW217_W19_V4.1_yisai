/*****************************************************************************
** File Name:      hellosprd.c                                               *
** Author:                                                                   *
** Date:           4/01/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 4/2009         xiaoqing.lu       Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "std_header.h"

#ifdef MMI_MOCOR_SAMPLE

#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmi_textfun.h"
#include "sample_text.h"
#include "sample_image.h"
#include "sample_menutable.h"
#include "sample_id.h"
#include "mmiacc_text.h"
#include "sample_nv.h"
#include "guitab.h"
#include "guistring.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMI_HELLO_LIST_NUM      3
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
//  Description : Tab控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTabWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       );

/*****************************************************************************/
//  Description : Tab控件测试窗口的回调函数, TAB1 子窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTab1ChildWindow(
                                             MMI_WIN_ID_T     win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM           param
                                             );

/*****************************************************************************/
//  Description : Tab控件测试窗口的回调函数, TAB1 子窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTab2ChildWindow(
                                             MMI_WIN_ID_T     win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM           param
                                             );

/*****************************************************************************/
// 	Description : 增加Tab信息
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void  AppendTabInfo(void);

/*****************************************************************************/
//  Description : 在Tab控件的子窗口显示一个字符串
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void DisplayString(
                         uint16 *wstr_ptr, 
                         uint16 len
                         );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// TAB控件父窗口
WINDOW_TABLE( MMI_SAMPLE_TABCTRL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTabWinMsg),    
    WIN_ID( MMI_TEST_TAB_WIN_ID),

    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMI_TEST_TAB_CTRL_ID),

    END_WIN
};

// TAB1子窗口
WINDOW_TABLE( MMI_SAMPLE_TAB1_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTab1ChildWindow),    
    WIN_ID( MMI_TEST_TAB1_CHILD_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// TAB2子窗口
WINDOW_TABLE( MMI_SAMPLE_TAB2_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTab2ChildWindow),    
    WIN_ID( MMI_TEST_TAB2_CHILD_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Tab控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTabWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
//     GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
//     GUI_RECT_T          rect = {0, 0, 239, 319};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_TAB_CTRL_ID);
        AppendTabInfo();
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        //GUIRES_DisplayImg(PNULL, &rect, &rect, win_id, IMAGE_COMMON_BG, &lcd_dev_info);
        break;

    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}


/*****************************************************************************/
//  Description : 在Tab控件的子窗口显示一个字符串
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void DisplayString(
                         uint16 *wstr_ptr, 
                         uint16 len
                         )
{
    GUI_POINT_T         start_point     = {1,140};
    GUI_RECT_T          rect            = {0};
    MMI_STRING_T        text_str        = {0};
    GUI_LCD_DEV_INFO	lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
    
	rect.left = 0;
	rect.top = MMI_TAB_CHILD_WINDOW_TOP;
	rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
	rect.top = MMI_CLIENT_RECT_BOTTOM;

    text_str.wstr_ptr = wstr_ptr;
    text_str.wstr_len = len;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    text_style.font = MMI_BLACK_COLOR;
    text_style.font_color = SONG_FONT_16;
    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&start_point,
        (const MMI_STRING_T     *)&text_str,
        (GUISTR_STYLE_T         *)&text_style,
        state
        );
}

/*****************************************************************************/
//  Description : Tab控件测试窗口的回调函数, TAB1 子窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTab1ChildWindow(
                                             MMI_WIN_ID_T     win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM           param
                                             )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUISOFTKEY_IMG_T    img_info = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        {
            wchar w_text[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', '1', 0 };
            uint16 len = MMIAPICOM_Wstrlen(w_text);

            DisplayString(w_text, len);
        }
        break;

    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMI_TEST_TAB_WIN_ID);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Tab控件测试窗口的回调函数, TAB1 子窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTab2ChildWindow(
                                             MMI_WIN_ID_T     win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM           param
                                             )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUISOFTKEY_IMG_T    img_info = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        {
            wchar w_text[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', '2', 0 };
            uint16 len = MMIAPICOM_Wstrlen(w_text);

            DisplayString(w_text, len);
        }
        break;

    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMI_TEST_TAB_WIN_ID);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : 增加Tab信息
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void  AppendTabInfo(void)
{
    MMI_STRING_T    str_text;
    
    // 设置最大选项卡数
    GUITAB_SetMaxItemNum(MMI_TEST_TAB_CTRL_ID, 2);

    // 获取选项卡文本信息
    MMI_GetLabelTextByLang(TXT_HELLO_TEST_TAB1_TEXT, &str_text);
    // 设置Item选项内容
    GUITAB_AppendSimpleItem(
        MMI_TEST_TAB_CTRL_ID,
        &str_text,
        IMAGE_TAB_SELECT1,
        IMAGE_TEST_TAB1_RELEASED);
    // 增加item子窗口
    GUITAB_AddChildWindow(MMI_TEST_TAB_CTRL_ID, MMI_TEST_TAB1_CHILD_WIN_ID, 0);

    // 获取选项卡文本信息
    MMI_GetLabelTextByLang(TXT_HELLO_TEST_TAB2_TEXT, &str_text);
    // 设置选项卡内容
    GUITAB_AppendSimpleItem(
        MMI_TEST_TAB_CTRL_ID,
        &str_text,
        IMAGE_TAB_SELECT1,
        IMAGE_TEST_TAB2_RELEASED);
    // 增加Item子窗口
    GUITAB_AddChildWindow(MMI_TEST_TAB_CTRL_ID, MMI_TEST_TAB2_CHILD_WIN_ID, 1);

    // 设置当前项
    GUITAB_SetCurSel(MMI_TEST_TAB_CTRL_ID, 0);
    // 设置焦点窗口
    MMK_SetChildWinFocus(MMI_TEST_TAB_WIN_ID, MMI_TEST_TAB1_CHILD_WIN_ID);
}

/*****************************************************************************/
//  Description : 创建TAb测试窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIHELLO_CreateTabWin(void)
{
    MMK_CreateParentWin((uint32*)MMI_SAMPLE_TABCTRL_WIN_TAB, PNULL);
    MMK_CreateChildWin(MMI_TEST_TAB_WIN_ID, (uint32*)MMI_SAMPLE_TAB1_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMI_TEST_TAB_WIN_ID, (uint32*)MMI_SAMPLE_TAB2_CHILD_WIN_TAB, NULL);
}

#endif
