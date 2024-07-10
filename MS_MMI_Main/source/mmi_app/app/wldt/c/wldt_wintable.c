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


#define  WLDT_TEST_MENU_LEN  20
#define  WLDT_TEST_MENU_NUM  5
const uint8 wldt_test_menu_text[WLDT_TEST_MENU_NUM][WLDT_TEST_MENU_LEN]= 
{
    "Trace Del",
    "Trace Save",
    "Log Enable",
    "Log Disable",
    "SCI_ASSERT"

};

LOCAL MMI_RESULT_E HandleWLDT_TestMainMenuWinMsg (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      );

WINDOW_TABLE( MMIWLDT_TEST_MENU_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleWLDT_TestMainMenuWinMsg),    
    WIN_ID( MMIWLDT_TEST_MENU_WIN_ID ),
    WIN_TITLE(TXT_WLDT_TEST),
    //WIN_STATUSBAR,
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIWLDT_TEST_MENU_CTRL_ID),
    END_WIN
};

BOOLEAN MMIWLDT_OpenTestMenuWin(void)
{
    MMK_CreateWin((uint32*)MMIWLDT_TEST_MENU_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description : to handle the message of atcmd test 
//	Global resource dependence : 
//  Author:jianshengqi
//	Note: 2006/3/19 
/*****************************************************************************/
//extern uint32 test_main_lcd_id[20];
//extern uint32 test_sub_lcd_id[20];

LOCAL MMI_RESULT_E  HandleWLDT_TestMainMenuWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMIWLDT_TEST_MENU_CTRL_ID;
    uint16                    node_id = 0;
    static BOOLEAN            s_need_update   = FALSE;
  //  MMISET_VALIDATE_PRIVACY_RETURN_T    result_info = {MMISET_PROTECT_CL, FALSE};    
#if defined(DISABLE_STATUSBAR_DISPLAY) 
    GUI_RECT_T                          title_rect =   {0, 0, 239, 23};
#else
    GUI_RECT_T                          title_rect = {0, 18, 239, 18 + 23};
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 i;
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            MMI_STRING_T		    item_str = {0};
            uint8				copy_len = 0;
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};

            
            if (MMK_IsIncludeStatusBar(win_id))
            {
                GUIWIN_SetTitleRect(win_id, title_rect);   
            }  
            
            GUILIST_SetMaxItem(ctrl_id, WLDT_TEST_MENU_NUM,FALSE);   
            
            for ( i = 0; i < WLDT_TEST_MENU_NUM; i++ )
            {
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
                item_t.item_data_ptr = &item_data;
                copy_len = SCI_STRLEN(wldt_test_menu_text[i]);
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = IMAGE_SIM1;
                item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = buff;
                GUI_GB2UCS(item_data.item_content[1].item_data.text_buffer.wstr_ptr, wldt_test_menu_text[i],copy_len );
                item_data.item_content[1].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, copy_len);
                                   
                item_data.softkey_id[0] = TXT_COMMON_OK;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
                
                GUILIST_AppendItem(ctrl_id, &item_t);      
            }
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }

        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {
        node_id = GUILIST_GetCurItemIndex(  ctrl_id );
        node_id++;
        switch( node_id )
        {
#ifndef WIN32
            case  1:
                    #ifdef FILE_LOG_SUPPORT
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_DEL_ALL, 0,0);
                    #endif
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                
            case  2:
                    #ifdef FILE_LOG_SUPPORT
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_SAVE, 0,0);
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_AT_SAVE, 0,0);
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_BUF_SAVE, 0,0);
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_SOCKET_SAVE, 0,0);
                    #endif
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                
            case  3:
                {
                    REFPARAM_SetEnableArmLogFlag(TRUE);
                    #ifdef FILE_LOG_SUPPORT
                        TraceUser_Init();
                    #endif
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
                break;
                
            case  4:
                    REFPARAM_SetEnableArmLogFlag(FALSE);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                                
            case  5:
                    SCI_ASSERT(0);
                break;
#endif
            default:
                break;
        }
    }          
        break;
       
    case MSG_CTL_CANCEL:
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
#endif
