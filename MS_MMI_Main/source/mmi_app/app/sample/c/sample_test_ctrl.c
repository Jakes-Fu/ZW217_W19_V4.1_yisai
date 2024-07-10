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
#include "sample_main.h"
#include "guifont.h"
#include "mmi_textfun.h"
#include "sample_text.h"
#include "sample_image.h"
#include "sample_menutable.h"
#include "sample_id.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "sample_nv.h"
#include "guilistbox.h"
#include "guibutton.h"
#include "guilabel.h"

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
//  Description : test list win
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestListWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param
                                        );

/*****************************************************************************/
//  Description : append list item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(
                          MMI_CTRL_ID_T    ctrl_id,
                          MMI_IMAGE_ID_T   image_id,
                          MMI_TEXT_ID_T    text_id,
                          MMI_TEXT_ID_T	   left_softkey_id,
                          MMI_TEXT_ID_T	   right_softkey_id
                          );

/*****************************************************************************/
//  Description : Textbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTextboxWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           );

/*****************************************************************************/
//  Description : Button控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestButtonWinMsg(
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM           param
                                          );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

// 列表控件测试窗口
WINDOW_TABLE( MMI_TEST_LIST_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_LIST_WIN_ID ),
    WIN_FUNC((uint32)HandleTestListWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_LIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMI_TEST_LIST_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

// 文本显示框控件测试窗口
WINDOW_TABLE( MMI_TEST_TEXTBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_TEXTBOX_WIN_ID ),
    WIN_FUNC((uint32) HandleTestTextboxWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_TEXTBOX_TITLE),
    CREATE_TEXT_CTRL(MMI_TEST_TEXTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

// Button测试窗口
WINDOW_TABLE( MMI_TEST_BUTTON_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_BUTTON_WIN_ID ),
    WIN_FUNC((uint32) HandleTestButtonWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_BUTTON_TEXT),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MMI_TEST_BUTTON_LABEL1_CTRL_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_TEST_BUTTON_LABEL2_CTRL_ID),

	CREATE_BUTTON_CTRL(IMAGE_TEST_BUTTON_LEFT_OFF, MMI_TEST_BUTTON1_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_TEST_BUTTON_RIGHT_OFF,MMI_TEST_BUTTON2_CTRL_ID),

	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
}; 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 列表控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestListWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_LIST_CTRL_ID;

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_LIST_CTRL_ID);
        GUILIST_SetMaxItem(ctrl_id, MMI_HELLO_LIST_NUM, FALSE);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_CALL, TXT_HELLO_TEST_LIST1, TXT_COMMON_OK, STXT_RETURN);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_PHONE, TXT_HELLO_TEST_LIST2, STXT_SELECT,STXT_RETURN);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_DISPLAY, TXT_HELLO_TEST_LIST3, STXT_ANSWER, STXT_RETURN);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
        GUILIST_GetCurItemIndex(ctrl_id);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
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
//  Description : apend list item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(
                          MMI_CTRL_ID_T    ctrl_id,
                          MMI_IMAGE_ID_T   image_id,
                          MMI_TEXT_ID_T    text_id,
                          MMI_TEXT_ID_T	   left_softkey_id,
                          MMI_TEXT_ID_T	   right_softkey_id
                          )
{
    GUILIST_ITEM_T          item_t     = {0};
    GUILIST_ITEM_DATA_T     item_data  = {0};
    uint32                  item_index = 0;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.text_id = image_id;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem(ctrl_id, &item_t);

    return;
}

/*****************************************************************************/
//  Description : 创建list控件测试窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateListWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_LIST_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : Textbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTextboxWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_TEXTBOX_CTRL_ID;
    MMI_STRING_T        string = {0};

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMI_GetLabelTextByLang(TXT_HELLO_TEST_TEXTBOX_TEXT, &string);
        GUITEXT_SetString(ctrl_id , string.wstr_ptr, string.wstr_len, FALSE);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
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
//  Description : 创建Textbox测试窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateTextboxWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_TEXTBOX_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : left keystoke callback
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LeftButtonCallBack(void)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;
    wchar w_text[] = {'B', 'u', 't', 't', 'o', 'n', '1', 0};
    uint16 len = MMIAPICOM_Wstrlen(w_text);
    MMI_STRING_T        string = {0};

    string.wstr_len = len;
    string.wstr_ptr = w_text;

    GUILABEL_SetText(MMI_TEST_BUTTON_LABEL1_CTRL_ID, &string, TRUE);

	return result;
}

/*****************************************************************************/
//  Description : left keystoke callback
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RightButtonCallBack(void)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;
    wchar w_text[] = {'B', 'u', 't', 't', 'o', 'n', '2', 0};
    uint16 len = MMIAPICOM_Wstrlen(w_text);
    MMI_STRING_T        string = {0};

    string.wstr_len = len;
    string.wstr_ptr = w_text;

    GUILABEL_SetText(MMI_TEST_BUTTON_LABEL2_CTRL_ID, &string, TRUE);

	return result;
}

/*****************************************************************************/
//  Description : Button控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestButtonWinMsg(
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM           param
                                          )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          button_rect1 = {0};
    GUI_RECT_T          button_rect2 = {0};
    GUI_RECT_T          rect = {0, 0, 239, 319};
    
	button_rect1.left = 0;
	button_rect1.top = MMI_TITLE_HEIGHT + 60;
	button_rect1.right = 120;
	button_rect1.bottom = MMI_TITLE_HEIGHT + 60 + 23;

	button_rect2.left = 0;
	button_rect2.top = MMI_TITLE_HEIGHT + 60 + 23 + 5;
	button_rect2.right = 120;
	button_rect2.bottom = MMI_TITLE_HEIGHT + 60 + 23 + 5 + 23;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIBUTTON_SetRect(MMI_TEST_BUTTON1_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMI_TEST_BUTTON2_CTRL_ID,&button_rect2);
		{
			GUI_RECT_T lab_rect = {0};
			
			lab_rect.left = 0;
			lab_rect.top = MMI_TITLE_HEIGHT;
			lab_rect.right = 239;
			lab_rect.bottom = MMI_TITLE_HEIGHT + 25;
			
			GUILABEL_SetRect(MMI_TEST_BUTTON_LABEL1_CTRL_ID, &lab_rect, FALSE);
        }
		{
			GUI_RECT_T lab_rect = {0};

			lab_rect.left = 0;
			lab_rect.top = MMI_TITLE_HEIGHT + 30;
			lab_rect.right = 239;
			lab_rect.bottom = MMI_TITLE_HEIGHT + 55;

			GUILABEL_SetRect(MMI_TEST_BUTTON_LABEL2_CTRL_ID, &lab_rect, FALSE);
        }               
        
        GUIBUTTON_SetCallBackFunc(MMI_TEST_BUTTON1_CTRL_ID, LeftButtonCallBack);
        GUIBUTTON_SetCallBackFunc(MMI_TEST_BUTTON2_CTRL_ID, RightButtonCallBack);
        {
            wchar             w_text[] = L"Button1leftrightober";
            uint16 len = MMIAPICOM_Wstrlen(w_text);

            GUIBUTTON_SetText(MMI_TEST_BUTTON1_CTRL_ID,w_text,len);
        }
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL, &rect, &rect, win_id, IMAGE_COMMON_BG, &lcd_dev_info);
        break;

    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
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
//  Description : 创建Button测试窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateButtonWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_BUTTON_WIN_TAB, PNULL);
}
#endif

