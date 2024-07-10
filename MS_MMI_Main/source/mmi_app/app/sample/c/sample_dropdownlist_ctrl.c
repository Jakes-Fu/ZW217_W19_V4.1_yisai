/*****************************************************************************
** File Name:      hello_test_dropdownlist                                   *
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
#include "guidropdownlist.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMI_HELLO_DROPDOWNLIST_NUM      10
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
//  Description : Dropdownlist控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestDropdownlistWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// dropdownlist控件测试窗口
WINDOW_TABLE( MMI_TEST_DROPDOWNLIST_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_DROPDOWNLIST_WIN_ID ),
    WIN_FUNC((uint32)HandleTestDropdownlistWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_DROPDOWNLIST),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_DROPDOWNLIST_CTRL(3, GUIDNLIST_SHOW_DOWNWARD, MMI_TEST_DROPDOWNLIST_CTRL_ID),
    END_WIN
}; 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建dropdownlist测试窗口
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateDropdownlistWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_DROPDOWNLIST_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : Dropdownlist控件测试窗口的回调函数
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestDropdownlistWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_DROPDOWNLIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T        item_arry[MMI_HELLO_DROPDOWNLIST_NUM] ={0};
            uint32 i = 0;
            GUI_RECT_T          dropdownlist_rect = {40,60,219,85};
            
            GUIDROPDOWNLIST_SetRect(ctrl_id,&dropdownlist_rect);
            
            //设置dropdownlist acitve
            MMK_SetAtvCtrl( win_id, ctrl_id );
            
            for ( i = 0; i < MMI_HELLO_DROPDOWNLIST_NUM; i++ )
            {
                wchar  wstr[16] = {0};
                char   str[16]  = {0};
                uint16 str_len  = 0;

                str_len = sprintf( str, "test item %d", i );

                MMI_STRNTOWSTR( wstr, 16, str, 16, str_len );

                MMI_WSTRNCPY( item_arry[i].wstr_len, DROPDOWNLIST_STRING_MAX_NUM, wstr, str_len, str_len );

                item_arry[i].wstr_len = str_len;
            }

            //设置item
            GUIDROPDOWNLIST_AppendItemArray( ctrl_id, item_arry, MMI_HELLO_DROPDOWNLIST_NUM );
            
            //设置当前选中的item
            GUIDROPDOWNLIST_SetCurItemIndex( ctrl_id, 0 );
        }
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
            uint16 index = 0;
            
            //获得当前的索引
            index = GUIDROPDOWNLIST_GetCurItemIndex( ctrl_id );
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
#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
