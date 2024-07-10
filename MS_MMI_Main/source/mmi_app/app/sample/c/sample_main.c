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
#include "sample_test_ctrl.h"
#include "sample_tab_ctrl.h"
#include "guistring.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef struct  
{
    CAF_APPLET_T caf_applet;
    uint32       test_num;

}SAMPLE_APPLET_T;
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
//  Description : the process message function of applet
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  SampleAppletHandleEvent(    
                                      IAPPLET_T*          iapplet_ptr,
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM              param
                                      );

/*****************************************************************************/
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelloSprdWinMsg(
                                         MMI_HANDLE_T     win_handle, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : test menu win
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestMenuWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );

/*****************************************************************************/
//  Description : popupmenu test window
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestPopMenuWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           );

/*****************************************************************************/
//  Description : 写出测试提示
//  Global resource dependence : 
//  Author: lianxiang.zhou
//  Note: 
/*****************************************************************************/
LOCAL void DrawTestTips(GUI_LCD_DEV_INFO *lcd_info_ptr, GUI_POINT_T position);


/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
//显示字符串
WINDOW_TABLE( MMI_HELLO_SPRD_WIN_TAB ) = 
{
    WIN_ID(MMI_HELLO_SPRD_WIN_ID),
    WIN_FUNC((uint32)HandleHelloSprdWinMsg),
    WIN_TITLE(TXT_HELLO_TITLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


//显示字符串
WINDOW_TABLE( MMI_TEST_MENU_WIN_TAB ) = 
{
    WIN_ID(MMI_TEST_MENU_WIN_ID),
    WIN_FUNC((uint32)HandleTestMenuWinMsg),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_HELLO_TABLE, MMI_TEST_MENU_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// option menu
WINDOW_TABLE(MMITEST_OPT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleTestPopMenuWinMsg),
    WIN_ID(MMI_TEST_PUPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_TEST_POPUPMENU_TABLE,MMI_TEST_PUPMENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmisample_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_SAMPLE_APPLET_ID), sizeof(SAMPLE_APPLET_T), SampleAppletHandleEvent, 0 },
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T s_mmisample_applet_info = 
{
    s_mmisample_applet_array,
    ARR_SIZE(s_mmisample_applet_array)
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register sample module applet info
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_SAMPLE, &s_mmisample_applet_info );
}

/*****************************************************************************/
//  Description : the process message function of applet
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  SampleAppletHandleEvent(    
                                      IAPPLET_T*          iapplet_ptr,
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM              param
                                      )
{
    MMI_RESULT_E     result     = MMI_RESULT_FALSE;
    SAMPLE_APPLET_T* applet_ptr = (SAMPLE_APPLET_T*)iapplet_ptr;
    LOCAL uint32 test_num = 0;
    
    switch (msg_id)
    {
        //applet启动
    case MSG_APPLET_START:
        {
            uint32 start_param = 0;
            MMI_WINDOW_TABLE_CREATE_T create = {0};

            //获得start applet时传入的param
            start_param = *(uint32*)param;

            //窗口需要和applet绑定
            create.applet_handle = applet_ptr->caf_applet.app_handle;
            create.win_table_ptr = MMI_HELLO_SPRD_WIN_TAB;

            applet_ptr->test_num = test_num++;

            MMK_CreateWinTable( &create );
        }
        break;
        
    case MSG_APPLET_STOP:
        break;

    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : draw line
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void HelloDrawLine(void)
{
    GUI_POINT_T point1 = {1, 80};
    GUI_POINT_T point2 = {100, 160};
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    LCD_DrawLine(&lcd_dev_info, point1.x, point1.y, point2.x, point2.y, MMI_BLACK_COLOR);
    //LCD_DrawThickLine(&lcd_dev_info, point1.x, point1.y, point2.x, point2.y, MMI_BLACK_COLOR,5);
}

/*****************************************************************************/
//  Description : draw rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void HelloDrawRect(void)
{
    GUI_RECT_T box = {1, 80, 100, 160};
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    LCD_DrawRect(&lcd_dev_info, box, MMI_BLACK_COLOR);
}

/*****************************************************************************/
//  Description : fill rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void HelloFillRect(void)
{
    GUI_RECT_T box = {1, 80, 100, 160};
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    LCD_FillRect(&lcd_dev_info, box, MMI_YELLOW_COLOR);
}

/*****************************************************************************/
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelloSprdWinMsg(
                                         MMI_HANDLE_T     win_handle, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    //根据win_handle获得applet的指针
    SAMPLE_APPLET_T* applet_ptr = (SAMPLE_APPLET_T*)MMK_GetAppletPtrByWin( win_handle );
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;

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
            GUISTR_STYLE_T      text_style      = {0};
            GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
            
            GUI_POINT_T start_point = {1,30};
            GUI_RECT_T rect = {0,0,239,319};
            wchar w_text[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', 0 };
            uint16 len = MMIAPICOM_Wstrlen(w_text);
            MMI_STRING_T text_str = {0};
            // GUI_RECT_T box = {0,100,30,319};

            wchar w_text1[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D','H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D','H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D','H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D','H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', 0 };

             GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);

            MMIRES_GetText( TXT_HELLO_SPRD, win_handle, &text_str );
            
            text_style.font = SONG_FONT_16;
            text_style.font_color = MMI_BLACK_COLOR;
            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)&start_point,
                (const MMI_STRING_T     *)&text_str,
                (GUISTR_STYLE_T         *)&text_style,
                state
                );
            
            start_point.x = 1;
            start_point.y = 50;
            GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_handle,
                HELLO_SPRD_TEST, &lcd_dev_info);

            len = MMIAPICOM_Wstrlen(w_text1);

            HelloFillRect();
            HelloDrawLine();
            HelloDrawRect();

            //...
            start_point.x = 40;
            start_point.y = MMI_TITLE_HEIGHT + 7;
            
            DrawTestTips(&lcd_dev_info, start_point);
        }
        break;

    case MSG_APP_CANCEL:
        {
// #ifdef TEST_MULTI_APPLET //测试多个applet重入, 使用MMK_GetAppletPtrByGuid遍历所有guid相同的applet
//             SAMPLE_APPLET_T* find_applet_ptr = MMK_GetAppletPtrByGuid( SPRD_SAMPLE_APPLET_ID, 0, TRUE );
// 
//             //查找系统内所有guid为SPRD_SAMPLE_APPLET_ID的applet
//             while ( PNULL != find_applet_ptr )
//             {
//                 find_applet_ptr = MMK_GetAppletPtrByGuid( SPRD_SAMPLE_APPLET_ID, find_applet_ptr->caf_applet.app_handle, TRUE );
//             }
// 
//             MMK_CloseWin(win_handle);
// #else
            MMK_CloseApplet( applet_ptr->caf_applet.app_handle );
//#endif
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;

    case MSG_APP_OK:
        MMK_CreateWin((uint32*)MMI_TEST_MENU_WIN_TAB, PNULL);
        break;
        
    case MSG_APP_UP:
        {
            uint16  nv_return = 0;
            BOOLEAN flag = FALSE;
            GUI_POINT_T start_point = {1,80};
            GUI_RECT_T rect = {1,80,239,100};
            uint16 len = 0;//
            GUISTR_STYLE_T      text_style      = {0};
            GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
            MMI_STRING_T text_str = {0};

            nv_return = MMI_ReadNVItem(MMINV_HELLO_FLAG, &flag);

            GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);

            if (!flag)
            {
                wchar w_text[] = {'F', 'A', 'L', 'S', 'E', 0};
                len = MMIAPICOM_Wstrlen(w_text);

                text_str.wstr_len = len;
                text_str.wstr_ptr = w_text;
                text_style.font = SONG_FONT_16;
                text_style.font_color = MMI_BLACK_COLOR;
                
                GUISTR_DrawTextToLCDSingleLine( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_POINT_T      *)&start_point,
                    (const MMI_STRING_T     *)&text_str,
                    (GUISTR_STYLE_T         *)&text_style,
                    state
                    );

                flag = TRUE;
                MMI_WriteNVItem(MMINV_HELLO_FLAG, &flag);
            }
            else
            {
                wchar w_text[] = {'T', 'R', 'U', 'E', 0};
                len = MMIAPICOM_Wstrlen(w_text);

                text_str.wstr_len = len;
                text_str.wstr_ptr = w_text;
                text_style.font = SONG_FONT_16;
                text_style.font_color = MMI_BLACK_COLOR;
                
                GUISTR_DrawTextToLCDSingleLine( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_POINT_T      *)&start_point,
                    (const MMI_STRING_T     *)&text_str,
                    (GUISTR_STYLE_T         *)&text_style,
                    state
                    );

                flag = FALSE;
                MMI_WriteNVItem(MMINV_HELLO_FLAG, &flag);
            }
        }
        break;

    case MSG_APP_DOWN:
        MMIHELLO_CreateListWin();
        break;

    case MSG_APP_1:
        MMIHELLO_CreateTextboxWin();
        break;

    case MSG_APP_2:
        MMIHELLO_CreateButtonWin();
        break;

    case MSG_APP_3:
        MMIHELLO_CreateTabWin();
        break;
        
    case MSG_APP_4:
        MMIHELLO_CreateAnimWin();
        break;

    case MSG_APP_5:
        MMIHELLO_CreateIconlistWin();
        break;

    case MSG_APP_6:
        MMIHELLO_CreateEditboxMainWin();
        break;
    
    case MSG_APP_7:
        MMIHELLO_CreateDropdownlistWin();
        break;
        
    case MSG_APP_8:
        MMIHELLO_CreateRichtextWin();
        break;

    case MSG_APP_9:
        MMIHELLO_CreateImWin();
        break;
        
    case MSG_APP_0:
        MMIHELLO_CreateFormWin();
        break;
        
    case MSG_APP_HASH:
        MMIHELLO_CreateSetlistWin();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : popupmenu test window
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestPopMenuWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_PUPMENU_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
        break;

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
//  Description : test menu win
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestMenuWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_MENU_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
#if 0        
        MMK_CreateWin( (uint32 *)MMITEST_OPT_WIN_TAB, PNULL );

//         MMIPOPMENU_CreateStatic(
//             MMI_TEST_PUPMENU_WIN_ID ,
//             MMI_TEST_PUPMENU_CTRL_ID,
//             MENU_TEST_POPUPMENU_TABLE,
//             HandleTestPopMenuWinMsg,
//             PNULL
//             );
        break;

    case MSG_CTL_CANCEL:
#endif
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
//  Description : 写出测试提示
//  Global resource dependence : 
//  Author: lianxiang.zhou
//  Note: 
/*****************************************************************************/
LOCAL void DrawTestTips(GUI_LCD_DEV_INFO *lcd_info_ptr, GUI_POINT_T position)
{
    GUI_FONT_T font = SONG_FONT_16;
    GUI_COLOR_T color = MMI_BLACK_COLOR;
    GUI_POINT_T pos = position;
    uint16 delta_y = GUI_GetFontHeight(font, UNICODE_HANZI) + 2;
    LOCAL wchar str_ok[] = L"Ok - Menu";
    LOCAL wchar str_up[] = L"Up - NV";
    LOCAL wchar str_down[] = L"Down - List";
    LOCAL wchar str_1[] = L"1 - Textbox";
    LOCAL wchar str_2[] = L"2 - Button";
    LOCAL wchar str_3[] = L"3 - Tab";
    LOCAL wchar str_4[] = L"4 - Anim";
    LOCAL wchar str_5[] = L"5 - Iconlist";
    LOCAL wchar str_6[] = L"6 - Editbox";
    LOCAL wchar str_7[] = L"7 - Dropdownlist";
    LOCAL wchar str_8[] = L"8 - Richtext";
    LOCAL wchar str_9[] = L"9 - Imput Method";
    LOCAL wchar str_10[] = L"0 - Form";
    wchar *str[] = {str_ok, str_up, str_down, str_1, str_2, str_3, str_4, str_5, str_6, str_7, str_8, str_9,str_10};
    int32 i = 0;
    
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
    MMI_STRING_T text_str = {0};
    
    //SCI_ASSERT(PNULL != lcd_info_ptr); /*assert verified*/
    if(PNULL == lcd_info_ptr)
    {
	    return;
	}
	
    for(i=0; i<sizeof(str)/sizeof(str[0]); i++)
    {
        text_str.wstr_len = MMIAPICOM_Wstrlen(str[i]);
        text_str.wstr_ptr = str[i];
        text_style.font = font;
        text_style.font_color = color;

        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)lcd_info_ptr,
            (const GUI_POINT_T      *)&pos,
            (const MMI_STRING_T     *)&text_str,
            (GUISTR_STYLE_T         *)&text_style,
            state
            );

        pos.y += delta_y;
    }
    
    return;
}



/*****************************************************************************/
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISAMPLE_CreateHelloSprdWin(void)
{
    MMI_APPLET_START_T start ={0};
    uint32 start_param = 0xabcd;
    
    start.guid = SPRD_SAMPLE_APPLET_ID;
    start.param_ptr = &start_param;
    start.size_of_param = sizeof(uint32);
    
    MMK_StartApplet( &start );

    //MMK_CreateWin((uint32*)MMI_HELLO_SPRD_WIN_TAB, PNULL);
}


/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISAMPLE_HELLO_ModuleInit(void)
{
    MMIHELLO_RegMenuGroup();
    MMI_RegHelloNv();
    RegAppletInfo();
}
#endif

