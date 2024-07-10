/*****************************************************************************
** File Name:      hello_test_editbox                                        *
** Author:                                                                   *
** Date:           5/8/2009                                                  *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 5/2009         James.Zhang       Create                                   *
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
#include "guilistbox.h"
#include "guiedit.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

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
//  Description : Editbox控件测试主窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestEditboxMainWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               );

/*****************************************************************************/
//  Description : Text Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTextEditboxWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               );

/*****************************************************************************/
//  Description : Phonenum Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestPhonenumEditboxWinMsg(
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM           param
                                                   );

/*****************************************************************************/
//  Description : Numlist Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestNumlistEditboxWinMsg(
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM           param
                                                  );

/*****************************************************************************/
//  Description : Digital Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestDigitalEditboxWinMsg(
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM           param
                                                  );

/*****************************************************************************/
//  Description : Password Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestPasswordEditboxWinMsg(
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM           param
                                                   );

/*****************************************************************************/
//  Description : Date Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestDateEditboxWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               );

/*****************************************************************************/
//  Description : Time Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTimeEditboxWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               );

/*****************************************************************************/
//  Description : Ip address Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestIpAddressEditboxWinMsg(
                                                    MMI_WIN_ID_T     win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM           param
                                                    );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// editbox控件测试主窗口
WINDOW_TABLE( MMI_TEST_EDITBOX_MAIN_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_EDITBOX_MAIN_WIN_ID ),
    WIN_FUNC((uint32)HandleTestEditboxMainWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_EDITBOX),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_TEST_EDITBOX_LIST_CTRL_ID),
    END_WIN
}; 

//text editbox
WINDOW_TABLE( MMI_TEST_TEXT_EDITBOX_WIN_TAB ) = 
{
    WIN_ID(MMI_TEST_TEXT_EDITBOX_WIN_ID),
    WIN_TITLE(TXT_HELLO_TEST_TEXT_EDITBOX),
    WIN_FUNC((uint32)HandleTestTextEditboxWinMsg),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(100, MMI_TEST_TEXT_EDITBOX_CTRL_ID),
    END_WIN
};

//phone num editbox
WINDOW_TABLE( MMI_TEST_PHONENUM_EDITBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_PHONENUM_EDITBOX_WIN_ID ),
    WIN_TITLE( TXT_HELLO_TEST_PHONENUM_EDITBOX ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),//设置背景
    WIN_FUNC((uint32) HandleTestPhonenumEditboxWinMsg ),    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PHONENUM_CTRL(100,MMI_TEST_PHONENUM_EDITBOX_CTRL_ID),
    END_WIN
};

//num list editbox
WINDOW_TABLE( MMI_TEST_NUMLIST_EDITBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_NUMLIST_EDITBOX_WIN_ID ),
    WIN_TITLE( TXT_HELLO_TEST_NUMLIST_EDITBOX ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32) HandleTestNumlistEditboxWinMsg ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_LIST_CTRL(100,10,MMI_TEST_NUMLIST_EDITBOX_CTRL_ID),
    END_WIN
};

//digital editbox
WINDOW_TABLE( MMI_TEST_DIGITAL_EDITBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_DIGITAL_EDITBOX_WIN_ID ),
    WIN_TITLE( TXT_HELLO_TEST_DIGITAL_EDITBOX ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32) HandleTestDigitalEditboxWinMsg ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(100,MMI_TEST_DIGITAL_EDITBOX_CTRL_ID),
    END_WIN
};

//password editbox
WINDOW_TABLE( MMI_TEST_PASSWORD_EDITBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_PASSWORD_EDITBOX_WIN_ID ),
    WIN_TITLE(TXT_HELLO_TEST_PASSWORD_EDITBOX),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC( (uint32)HandleTestPasswordEditboxWinMsg ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN), 
    CREATE_EDIT_PASSWORD_CTRL(100,MMI_TEST_PASSWORD_EDITBOX_CTRL_ID),
    END_WIN
};

//date editbox
WINDOW_TABLE( MMI_TEST_DATE_EDITBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_DATE_EDITBOX_WIN_ID ),
    WIN_TITLE(TXT_HELLO_TEST_DATE_EDITBOX),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC( (uint32)HandleTestDateEditboxWinMsg ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN), 
    CREATE_EDIT_DATE_CTRL(MMI_TEST_DATE_EDITBOX_CTRL_ID),
    END_WIN
};

// time editbox
WINDOW_TABLE( MMI_TEST_TIME_EDITBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_TIME_EDITBOX_WIN_ID ),
    WIN_TITLE(TXT_HELLO_TEST_TIME_EDITBOX),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC( (uint32)HandleTestTimeEditboxWinMsg ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN), 
    
    CREATE_EDIT_TIME_CTRL(MMI_TEST_TIME_EDITBOX_CTRL_ID),
    END_WIN
};

// ipaddress editbox
WINDOW_TABLE( MMI_TEST_IPADDRESS_EDITBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_IPADDRESS_EDITBOX_WIN_ID ),
    WIN_TITLE(TXT_HELLO_TEST_IPADDRESS_EDITBOX),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC( (uint32)HandleTestIpAddressEditboxWinMsg ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_IP_CTRL(MMI_TEST_IPADDRESS_EDITBOX_CTRL_ID),
    END_WIN
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建Editbox测试主窗口
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateEditboxMainWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_EDITBOX_MAIN_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : apend edit box main list item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendEditBoxMainListItem(
                                     MMI_CTRL_ID_T    ctrl_id,
                                     MMI_TEXT_ID_T    text_id,
                                     uint32           user_data
                                     )
{
    GUILIST_ITEM_T          item_t     = {0};
    GUILIST_ITEM_DATA_T     item_data  = {0};
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT;
    item_t.user_data     = user_data;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Description : Editbox控件测试主窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestEditboxMainWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_EDITBOX_LIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //设置list acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        //设置list最大item
        GUILIST_SetMaxItem( ctrl_id, 10, FALSE );
        //append item, edit类型做为user data传入, 用于标识item
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_TEXT_EDITBOX, GUIEDIT_TYPE_TEXT );
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_PHONENUM_EDITBOX, GUIEDIT_TYPE_PHONENUM );
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_DIGITAL_EDITBOX, GUIEDIT_TYPE_DIGITAL );
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_PASSWORD_EDITBOX, GUIEDIT_TYPE_PASSWORD );
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_NUMLIST_EDITBOX, GUIEDIT_TYPE_LIST );
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_DATE_EDITBOX, GUIEDIT_TYPE_DATE );
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_TIME_EDITBOX, GUIEDIT_TYPE_TIME );
        AppendEditBoxMainListItem( ctrl_id, TXT_HELLO_TEST_IPADDRESS_EDITBOX, GUIEDIT_TYPE_IP );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            //根据list当前索引获得当前item的指针
            const GUILIST_ITEM_T* item_ptr = GUILIST_GetItemPtrByIndex( ctrl_id, GUILIST_GetCurItemIndex( ctrl_id ) );
            
            //根据append时传入的user data, 进入指定的editbox测试窗口
            switch( item_ptr->user_data )
            {
            case GUIEDIT_TYPE_TEXT:
                MMK_CreateWin((uint32*)MMI_TEST_TEXT_EDITBOX_WIN_TAB, PNULL);
                break;
            case GUIEDIT_TYPE_PHONENUM:
                MMK_CreateWin((uint32*)MMI_TEST_PHONENUM_EDITBOX_WIN_TAB, PNULL);
                break;
            case GUIEDIT_TYPE_DIGITAL:
                MMK_CreateWin((uint32*)MMI_TEST_DIGITAL_EDITBOX_WIN_TAB, PNULL);
                break;
            case GUIEDIT_TYPE_PASSWORD:
                MMK_CreateWin((uint32*)MMI_TEST_PASSWORD_EDITBOX_WIN_TAB, PNULL);
                break;
            case GUIEDIT_TYPE_LIST:
                MMK_CreateWin((uint32*)MMI_TEST_NUMLIST_EDITBOX_WIN_TAB, PNULL);
                break;
                
            case GUIEDIT_TYPE_DATE:
                MMK_CreateWin((uint32*)MMI_TEST_DATE_EDITBOX_WIN_TAB, PNULL);
                break;
            case GUIEDIT_TYPE_TIME:
                MMK_CreateWin((uint32*)MMI_TEST_TIME_EDITBOX_WIN_TAB, PNULL);
                break;
            case GUIEDIT_TYPE_IP:
                MMK_CreateWin((uint32*)MMI_TEST_IPADDRESS_EDITBOX_WIN_TAB, PNULL);
                break;
            default:
                break;
            }
        }
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
//  Description : Text Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTextEditboxWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_TEXT_EDITBOX_CTRL_ID;
    MMI_STRING_T  string  = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        MMI_GetLabelTextByLang( TXT_HELLO_TEST_TEXT_EDITBOX, &string );
        
        //设置edit的字符串
        GUIEDIT_SetString( ctrl_id, string.wstr_ptr, string.wstr_len );
        
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        //获得edit的字符串
        GUIEDIT_GetString( ctrl_id, &string );
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
//  Description : Phonenum Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestPhonenumEditboxWinMsg(
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM           param
                                                   )
{
    MMI_RESULT_E  recode     = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id    = MMI_TEST_PHONENUM_EDITBOX_CTRL_ID;
    MMI_STRING_T  string     = {0};
    wchar         test_str[] = { '1', '2', '3', 0 };
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //设置edit的字符串
        GUIEDIT_SetString( ctrl_id, test_str, MMIAPICOM_Wstrlen( test_str ) );
        
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //获得edit的字符串
        GUIEDIT_GetPhoneNumString( ctrl_id, &string );
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
//  Description : Numlist Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestNumlistEditboxWinMsg(
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM           param
                                                  )
{
    MMI_RESULT_E  recode     = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id    = MMI_TEST_NUMLIST_EDITBOX_CTRL_ID;
    MMI_STRING_T  string     = {0};
    wchar         test_str[] = { '1', '2', '3', 0 };
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //设置edit的字符串
        GUIEDIT_AddListItem(ctrl_id,test_str,MMIAPICOM_Wstrlen( test_str ));
        
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
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
//  Description : Digital Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestDigitalEditboxWinMsg(
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM           param
                                                  )
{
    MMI_RESULT_E  recode     = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id    = MMI_TEST_DIGITAL_EDITBOX_CTRL_ID;
    MMI_STRING_T  string     = {0};
    wchar         test_str[] = { '1', '2', '3', 0 };
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:       
        //设置edit的字符串
        GUIEDIT_SetString( ctrl_id, test_str, MMIAPICOM_Wstrlen( test_str ) );
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        //获得edit的字符串
        GUIEDIT_GetString( ctrl_id, &string );
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
//  Description : Password Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestPasswordEditboxWinMsg(
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM           param
                                                   )
{
    MMI_RESULT_E  recode     = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id    = MMI_TEST_PASSWORD_EDITBOX_CTRL_ID;
    MMI_STRING_T  string     = {0};
    wchar         test_str[] = { '1', '2', '3', 0 };
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:        
        //设置edit的字符串
        GUIEDIT_SetString( ctrl_id, test_str, MMIAPICOM_Wstrlen( test_str ) );
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        //获得edit的字符串
        GUIEDIT_GetString( ctrl_id, &string );
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
//  Description : Date Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestDateEditboxWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_DATE_EDITBOX_CTRL_ID;
    uint16        year    = 2009;
    uint8         month   = 1;
    uint8         day     = 1;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //设置日期
        GUIEDIT_SetDate(ctrl_id,year,month,day);
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //获得日期
        GUIEDIT_GetDate( ctrl_id, &year, &month, &day );
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
//  Description : Time Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestTimeEditboxWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_TIME_EDITBOX_CTRL_ID;
    uint8         hour    = 18;
    uint8         minute  = 18;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //设置时间
        GUIEDIT_SetTime( ctrl_id, hour, minute,0);
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //获得时间
        GUIEDIT_GetTime( ctrl_id, &hour, &minute, PNULL);
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
//  Description : Ip address Editbox控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestIpAddressEditboxWinMsg(
                                                    MMI_WIN_ID_T     win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM           param
                                                    )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_IPADDRESS_EDITBOX_CTRL_ID;
    uint32        ip_addr = ( 192 << 24 ) | ( 168 << 16 ) | ( 1 << 8 ) | 100;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //设置ip
        GUIEDIT_SetIP( ctrl_id, ip_addr );
        //设置edit acitve
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //获得ip
        ip_addr = GUIEDIT_GetIP(ctrl_id);
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

#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
