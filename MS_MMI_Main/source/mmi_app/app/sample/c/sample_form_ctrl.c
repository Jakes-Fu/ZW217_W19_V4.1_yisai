/*****************************************************************************
** File Name:      sample_form_ctrl.c                                        *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

#define _SAMPLE_FORM_CTRL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "guiform.h"
#include "guianim.h"
#include "guitext.h"
#include "guilabel.h"
#include "guiwin.h"
#include "window_parse.h"
#include "sample_id.h"
#include "mmi_id.h"
#include "mmidisplay_data.h"
#include "mmicc_image.h"
#include "guisetlist.h"
#include "guidropdownlist.h"
#include "sample_image.h"
#include "mmiwallpaper_image.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
/*****************************************************************************/
// 	Description : handle form win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestFormWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : handle form win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestFormWinMsg1(
                                         MMI_WIN_ID_T       win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         );

/*****************************************************************************/
// 	Description : set form param
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void SetFormParam(void);

/*****************************************************************************/
// 	Description : draw line on anim
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void DrawLineOnAnim(
                          GUIANIM_OWNER_DRAW_T  *owner_draw_ptr
                          );

/*****************************************************************************/
// 	Description : set form title and softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void SetTitileAndSoftkey(
                               MMI_CTRL_ID_T    ctrl_id,
                               MMI_HANDLE_T     win_handle
                               );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetlistTestWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param
                                            );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildlistTestWinMsg(
                                              MMI_WIN_ID_T		win_id, 	
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM				param
                                              );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void  SetlistTest(
                        MMI_WIN_ID_T win_id
                        );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetTestDropdownlist(void);

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//form control window
WINDOW_TABLE( MMI_TEST_FORM_WIN_TAB ) = 
{
    WIN_ID(MMI_TEST_FORM_WIN_ID),
	WIN_FUNC((uint32)HandleTestFormWinMsg),
    WIN_TITLE(TXT_NAME),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_TEST_FORM_CTRL_ID),
        CHILD_ANIM_CTRL(TRUE,FALSE,MMI_TEST_FORM_ANIM_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL1_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,FALSE,MMI_TEST_FORM_LABEL3_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_EDIT_TIME_CTRL(TRUE,MMI_TEST_FORM_EDIT_TEXT_CTRL2_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_ANIM_CTRL(FALSE,TRUE,MMI_TEST_FORM_ANIM3_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_TEST_FORM_CTRL1_ID,MMI_TEST_FORM_CTRL_ID),
            CHILD_ANIM_CTRL(FALSE,TRUE,MMI_TEST_FORM_ANIM1_CTRL_ID,MMI_TEST_FORM_CTRL1_ID),
            CHILD_SETLIST_CTRL(TRUE,MMI_TEST_SETLIST_CTRL_ID,MMI_TEST_FORM_CTRL1_ID),
        CHILD_TEXT_CTRL(TRUE,MMI_TEST_FORM_TEXT1_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        //CHILD_TEXT_CTRL(TRUE,MMI_TEST_FORM_TEXT2_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        //CHILD_TEXT_CTRL(TRUE,MMI_TEST_FORM_TEXT3_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMI_TEST_FORM_LABEL2_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,300,MMI_TEST_FORM_EDIT_TEXT_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_TEST_FORM_CTRL2_ID,MMI_TEST_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,300,MMI_TEST_FORM_EDIT_TEXT_CTRL1_ID,MMI_TEST_FORM_CTRL2_ID),
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_TEST_FORM_CTRL3_ID,MMI_TEST_FORM_CTRL2_ID),
                CHILD_ANIM_CTRL(FALSE,TRUE,MMI_TEST_FORM_ANIM2_CTRL_ID,MMI_TEST_FORM_CTRL3_ID),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_TEST_BUTTON_LEFT_OFF,MMI_TEST_FORM_BUTTON1_CTRL_ID,MMI_TEST_FORM_CTRL3_ID),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_TEST_BUTTON_RIGHT_OFF,MMI_TEST_FORM_BUTTON2_CTRL_ID,MMI_TEST_FORM_CTRL3_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMI_TEST_DROPDOWNLIST_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

//form control window
WINDOW_TABLE( MMI_TEST_FORM_WIN_TAB1 ) = 
{
    WIN_ID(MMI_TEST_FORM_WIN_ID),
	WIN_FUNC((uint32)HandleTestFormWinMsg1),
    WIN_TITLE(TXT_NAME),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_TEST_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL1_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,TRUE,MMI_TEST_FORM_LABEL3_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

// set alarm ON after set time
WINDOW_TABLE( SETLIST_TEST_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSetlistTestWinMsg ),
        WIN_ID( MMI_TEST_SETLIST_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_SETLIST_CTRL(MMI_TEST_SETLIST_CTRL_ID), 
        END_WIN
};

// set alarm ON after set time
WINDOW_TABLE( CHILDLIST_TEST_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleChildlistTestWinMsg ),
        WIN_ID( MMI_TEST_CHILDLIST_WIN_ID ),
        WIN_STATUSBAR,
        WIN_TITLE(TXT_NAME),

        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_TEST_FORM_CTRL_ID),

        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL1_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL2_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL3_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL4_CTRL_ID,MMI_TEST_FORM_CTRL_ID),

        CHILD_LIST_CTRL(TRUE,GUILIST_RADIOLIST_E,MMI_TEST_CTRL_1_ID,MMI_TEST_FORM_CTRL_ID),

        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL5_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL6_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL7_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL8_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL9_CTRL_ID,MMI_TEST_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_TEST_FORM_LABEL10_CTRL_ID,MMI_TEST_FORM_CTRL_ID),

        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        
        END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : creat form win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIHELLO_CreateFormWin(void)
{
    MMK_CreateWin((uint32*)CHILDLIST_TEST_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIHELLO_CreateSetlistWin(void)
{
    MMK_CreateWin((uint32*)SETLIST_TEST_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetlistTestWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param
                                            )
{
    MMI_RESULT_E      recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T        box = {0,0, 239, 399};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetlistTest(win_id); 
        MMK_SetAtvCtrl( win_id,  MMI_TEST_SETLIST_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, box, 0xffff);
        break;

    case MSG_NOTIFY_MIDSK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void  SetlistTest(
                        MMI_WIN_ID_T win_id
                        )
{
    MMI_CTRL_ID_T           current_ctrl_id = MMI_TEST_SETLIST_CTRL_ID;
    GUISETLIST_ITEM_INFO_T  item_info = {0};
    wchar *str_ptr1 = L"item1"; 
    wchar *str_ptr2 = L"item2"; 
    wchar *str_ptr3 = L"item3"; 
    wchar *str_ptr4 = L"item4"; 
    
    
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT|GUISETLIST_ITEM_STATE_IMAGE;
    item_info.str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr1);
    item_info.str_info.wstr_ptr = str_ptr1;
    
    GUISETLIST_AddItem(current_ctrl_id, &item_info);
    
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr2);
    item_info.str_info.wstr_ptr = str_ptr2;
    GUISETLIST_AddItem(current_ctrl_id, &item_info);
    
    item_info.str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr3);
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_ptr = str_ptr3;
    GUISETLIST_AddItem(current_ctrl_id, &item_info);
    
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    item_info.str_info.wstr_ptr = str_ptr4;
    GUISETLIST_AddItem(current_ctrl_id, &item_info);
    
    GUIWIN_SetTitleTextId(win_id, TXT_NAME, FALSE); 

    return;
}

#define MMI_HELLO_DROPDOWNLIST_NUM      10
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetTestDropdownlist(void)
{
    uint32                  i = 0;
    MMI_CTRL_ID_T           ctrl_id = MMI_TEST_DROPDOWNLIST_CTRL_ID;
    MMI_STRING_T            item_arry[MMI_HELLO_DROPDOWNLIST_NUM] ={0};

    for ( i = 0; i < MMI_HELLO_DROPDOWNLIST_NUM; i++ )
    {
        wchar  wstr[16] = {0};
        char   str[16]  = {0};
        uint16 str_len  = 0;
        
        str_len = sprintf( str, "test item %d", i );
        
        MMI_STRNTOWSTR( wstr, 16, str, 16, str_len );
        
        MMI_WSTRNCPY( item_arry[i].wstr_ptr, DROPDOWNLIST_STRING_MAX_NUM, wstr, str_len, str_len );
        
        item_arry[i].wstr_len = str_len;
    }
    GUIDROPDOWNLIST_SetListState(MMI_TEST_DROPDOWNLIST_CTRL_ID, GUILIST_STATE_NEED_TITLE|GUILIST_STATE_NEED_SOFTKEY, TRUE);
    //设置item
    GUIDROPDOWNLIST_AppendItemArray( ctrl_id, item_arry, MMI_HELLO_DROPDOWNLIST_NUM );
    
    //设置当前选中的item
    GUIDROPDOWNLIST_SetCurItemIndex( ctrl_id, 0 );
}

/*****************************************************************************/
// 	Description : handle form win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestFormWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetFormParam();
        SetlistTest(win_id);
        SetTestDropdownlist();
        break;

    case MSG_CTL_OK:
        GUIFORM_SetChildDisplay(MMI_TEST_FORM_CTRL_ID,
            MMI_TEST_FORM_EDIT_TEXT_CTRL_ID,
            GUIFORM_CHILD_DISP_HIDE);
        break;

    case MSG_CTL_CANCEL:
        GUIFORM_SetChildDisplay(MMI_TEST_FORM_CTRL_ID,
            MMI_TEST_FORM_EDIT_TEXT_CTRL_ID,
            GUIFORM_CHILD_DISP_NORMAL);
        break;

    case MSG_APP_1:
        GUILABEL_SetVisible(MMI_TEST_FORM_LABEL1_CTRL_ID,FALSE,TRUE);
        break;
        
    case MSG_APP_2:
        GUILABEL_SetVisible(MMI_TEST_FORM_LABEL1_CTRL_ID,TRUE,TRUE);
        break;

    case MSG_APP_3:
        {
            const wchar     text_str[] = L"women";
            MMI_STRING_T    label_str = {0};

            label_str.wstr_ptr = (wchar *)text_str;
            label_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(text_str);

            GUILABEL_SetText(MMI_TEST_FORM_LABEL1_CTRL_ID,&label_str,TRUE);
        }
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        //set title and softkey
        SetTitileAndSoftkey(*(MMI_CTRL_ID_T *)param,win_id);
        break;

    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set form param
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void SetFormParam(void)
{
    const wchar             file_name[] = L"D:\\Photos\\bg.gif";
    const wchar             file_name1[] = L"D:\\Photos\\0.jpg";
    const wchar             file_name2[] = L"D:\\Photos\\2.gif";
    const wchar             text_str1[] = L"123456";
    const wchar             text_str2[] = L"D:\\Photos\\123jpgwoejegfioegihefsfdgreklgjirjegihrhgerhguirhuighruihurgherihguidjkdsfakljdjgkjdgjkjkdsgjkldjksgkjdgjklgdjkldgjklgdjkjkdgjklgdjklgdjkldfsdakfjeiowifoejgjjdgkjldgjkldgjkldgjkldgjgdkkljdgjklgiueguhgfeigfehabcdefghijjjjlkjghkakgldjgiii";
    const wchar             text_str3[] = L"abcdefg";
    SCI_TIME_T	    		sys_time = {0};
    MMI_STRING_T            label_str = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIFORM_CHILD_WIDTH_T   width = {0};

    width.type = GUIFORM_CHILD_WIDTH_PERCENT;
    width.add_data = 30;
    GUIFORM_SetChildWidth(MMI_TEST_FORM_CTRL1_ID,
        MMI_TEST_FORM_ANIM1_CTRL_ID,
        &width);

    width.type = GUIFORM_CHILD_WIDTH_PERCENT;
    width.add_data = 20;
    GUIFORM_SetChildWidth(MMI_TEST_FORM_CTRL2_ID,
        MMI_TEST_FORM_EDIT_TEXT_CTRL1_ID,
        &width);

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMI_TEST_FORM_ANIM_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
#ifndef MMI_LOW_MEMORY_RESOURCE
    data_info.img_id = IMAGE_WALLPAPER_1;
#else
    data_info.img_id = IMAGE_THEME_BG;
#endif
    file_info.full_path_wstr_ptr = &file_name1;
    file_info.full_path_wstr_len = (uint16)MMIAPICOM_Wstrlen(file_name);

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

    //set bg anim
    GUIANIM_SetParam(&control_info,&data_info,&file_info,&display_info);

    control_info.ctrl_id = MMI_TEST_FORM_ANIM1_CTRL_ID;

    data_info.img_id = IMAGE_BT_SEARCH_STOP;

    file_info.full_path_wstr_ptr = &file_name1;
    file_info.full_path_wstr_len = (uint16)MMIAPICOM_Wstrlen(file_name1);

    //set anim
    GUIANIM_SetParam(&control_info,&data_info,&file_info,&display_info);

    //set owner draw
    GUIANIM_SetOwnerDraw(MMI_TEST_FORM_ANIM1_CTRL_ID,DrawLineOnAnim);

    control_info.ctrl_id = MMI_TEST_FORM_ANIM2_CTRL_ID;

    data_info.img_id = IMAGE_BT_SEARCH_STOP;

    file_info.full_path_wstr_ptr = &file_name1;
    file_info.full_path_wstr_len = (uint16)MMIAPICOM_Wstrlen(file_name1);

    //set anim
    GUIANIM_SetParam(&control_info,&data_info,&file_info,&display_info);

    control_info.ctrl_id = MMI_TEST_FORM_ANIM3_CTRL_ID;

    file_info.full_path_wstr_ptr = &file_name1;
    file_info.full_path_wstr_len = (uint16)MMIAPICOM_Wstrlen(file_name1);

    //set anim
    GUIANIM_SetParam(&control_info,PNULL,&file_info,&display_info);

    //set time
    TM_GetSysTime(&sys_time);
    GUIEDIT_SetTime(MMI_TEST_FORM_EDIT_TEXT_CTRL2_ID,sys_time.hour,sys_time.min,0);
    
    //set text
    GUITEXT_SetString(MMI_TEST_FORM_TEXT1_CTRL_ID,
       (wchar *)text_str2,
       (uint16)MMIAPICOM_Wstrlen(text_str2),
       FALSE);

//    GUITEXT_SetString(MMI_TEST_FORM_TEXT2_CTRL_ID,
//        (wchar *)text_str3,
//        (uint16)MMIAPICOM_Wstrlen(text_str3),
//        FALSE);

//    GUITEXT_SetString(MMI_TEST_FORM_TEXT3_CTRL_ID,
//        (wchar *)text_str3,
//        (uint16)MMIAPICOM_Wstrlen(text_str3),
//        FALSE);

    //set label
    GUILABEL_SetIcon(MMI_TEST_FORM_LABEL1_CTRL_ID,IMAGE_MAINMENU_ICON_DCD);
    label_str.wstr_ptr = (wchar *)text_str1;
    label_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(text_str1);
    GUILABEL_SetText(MMI_TEST_FORM_LABEL1_CTRL_ID,&label_str,FALSE);

    //set label
    label_str.wstr_ptr = (wchar *)text_str2;
    label_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(text_str2);
    GUILABEL_SetText(MMI_TEST_FORM_LABEL2_CTRL_ID,&label_str,FALSE);

    //set label
    label_str.wstr_ptr = (wchar *)text_str3;
    label_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(text_str3);
    GUILABEL_SetText(MMI_TEST_FORM_LABEL3_CTRL_ID,&label_str,FALSE);

    //set active control
    MMK_SetAtvCtrl(MMI_TEST_FORM_WIN_ID,MMI_TEST_FORM_ANIM3_CTRL_ID);
}

/*****************************************************************************/
// 	Description : draw line on anim
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void DrawLineOnAnim(
                          GUIANIM_OWNER_DRAW_T  *owner_draw_ptr
                          )
{
    //draw line
    LCD_DrawThickLine(&owner_draw_ptr->lcd_dev,
        owner_draw_ptr->display_x,
        owner_draw_ptr->display_y,
        owner_draw_ptr->display_rect.right,
        owner_draw_ptr->display_rect.bottom,
        MMI_DARK_RED_COLOR,
        2);
}

/*****************************************************************************/
// 	Description : set form title and softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void SetTitileAndSoftkey(
                               MMI_CTRL_ID_T    ctrl_id,
                               MMI_WIN_ID_T     win_id
                               )
{
    ctrl_id = MMK_GetActiveCtrlId(win_id);
    switch (ctrl_id)
    {
    case MMI_TEST_FORM_LABEL1_CTRL_ID:
        //set title
        GUIWIN_SetTitleTextId(win_id,TXT_NAME,TRUE);

        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,TRUE);
        break;

    case MMI_TEST_FORM_ANIM1_CTRL_ID:
        //set title
        GUIWIN_SetTitleTextId(win_id,TXT_COMMON_NUMBER,TRUE);

        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,STXT_SAVE,STXT_RETURN,TRUE);
        break;

    case MMI_TEST_FORM_EDIT_TEXT_CTRL_ID:
        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,STXT_SAVE,STXT_RETURN,TRUE);
        break;

    case MMI_TEST_DROPDOWNLIST_CTRL_ID:
        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,STXT_SAVE,STXT_RETURN,TRUE);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : handle form win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestFormWinMsg1(
                                         MMI_WIN_ID_T       win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    GUIFORM_CHILD_WIDTH_T   width = {0};
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        width.type = GUIFORM_CHILD_WIDTH_FIXED;
        width.add_data = 70;
        GUIFORM_SetChildWidth(MMI_TEST_FORM_CTRL_ID,
            MMI_TEST_FORM_LABEL1_CTRL_ID,
            &width);
        GUIFORM_SetChildWidth(MMI_TEST_FORM_CTRL_ID,
            MMI_TEST_FORM_LABEL3_CTRL_ID,
            &width);

        //GUIFORM_SetAlign(MMI_TEST_FORM_CTRL_ID,GUIFORM_CHILD_ALIGN_LEFT);
        GUIFORM_SetChildAlign(MMI_TEST_FORM_CTRL_ID,
            MMI_TEST_FORM_LABEL1_CTRL_ID,
            GUIFORM_CHILD_ALIGN_LEFT);

        //set active control
        MMK_SetAtvCtrl(MMI_TEST_FORM_WIN_ID,MMI_TEST_FORM_LABEL1_CTRL_ID);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void AppendlistItem(
                          MMI_CTRL_ID_T ctrl_id,
                          MMI_STRING_T *str_ptr
                          )
{
    GUILIST_ITEM_T      item_t      =   {0};
    GUILIST_ITEM_DATA_T item_data   =   {0};
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;

    item_data.item_content[0].item_data.text_buffer.wstr_len = str_ptr->wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = str_ptr->wstr_ptr;
    
    /*
    // 测试代码，如果屏蔽了MMIALM_AddTestListCtrlChild的调用，可以打开这段code，测试子list控件
    GUILIST_AppendItem(ctrl_id, &item_t);
    */
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void  ChildlistTest(
                        MMI_WIN_ID_T win_id
                        )
{
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_CTRL_1_ID;
    MMI_STRING_T  str_info = {0};
    wchar *str_ptr1 = L"item1"; 
    wchar *str_ptr2 = L"item2"; 
    wchar *str_ptr3 = L"item3"; 
    wchar *str_ptr4 = L"item4"; 
    wchar *str_ptr5 = L"item5"; 
    wchar *str_ptr6 = L"item6"; 
    wchar *str_ptr7 = L"item7"; 
    wchar *str_ptr8 = L"item8"; 
    
    GUILIST_SetMaxItem(ctrl_id,10,FALSE);
    // GUILIST_SetDisable3D(ctrl_id, TRUE);  // 停止3D
    // GUILIST_SetSlideState(ctrl_id, FALSE); // 停止滑动
    
    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr1);
    str_info.wstr_ptr = str_ptr1;
    
    GUILABEL_SetText(MMI_TEST_FORM_LABEL1_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);
    
    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr2);
    str_info.wstr_ptr = str_ptr2;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL2_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);
    
    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr3);
    str_info.wstr_ptr = str_ptr3;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL3_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);
    
    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    str_info.wstr_ptr = str_ptr4;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL4_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);

    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    str_info.wstr_ptr = str_ptr5;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL5_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);

    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    str_info.wstr_ptr = str_ptr6;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL6_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);

    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    str_info.wstr_ptr = str_ptr7;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL7_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);

    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    str_info.wstr_ptr = str_ptr8;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL8_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);

    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    str_info.wstr_ptr = str_ptr8;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL9_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);

    str_info.wstr_len = MMIAPICOM_Wstrlen(str_ptr4);
    str_info.wstr_ptr = str_ptr8;
    GUILABEL_SetText(MMI_TEST_FORM_LABEL10_CTRL_ID,&str_info,FALSE);
    AppendlistItem(ctrl_id, &str_info);

    MMIALM_AddTestListCtrlChild(ctrl_id);
    return;
}

LOCAL BOOLEAN s_is_full_paint = FALSE;

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildlistTestWinMsg(
                                              MMI_WIN_ID_T		win_id, 	
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM				param
                                              )
{
    MMI_RESULT_E      recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T        box = {0,0, 239, 399};
    GUIFORM_CHILD_HEIGHT_T height = {100, GUIFORM_CHILD_HEIGHT_FIXED};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        ChildlistTest(win_id); 
        if (s_is_full_paint)
        {
            GUIFORM_SetChildHeight(MMI_TEST_FORM_CTRL_ID, MMI_TEST_CTRL_1_ID, &height);
        }
        MMK_SetAtvCtrl( win_id,  MMI_TEST_CTRL_1_ID);
        break;

    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, box, 0xffff);
        break;

    case MSG_APP_LEFT:
        s_is_full_paint = !s_is_full_paint;
        break;

    case MSG_NOTIFY_MIDSK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

