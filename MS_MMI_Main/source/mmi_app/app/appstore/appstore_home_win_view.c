/*****************************************************************************
** File Name:      appstore_home_win_view.c                                  *
** Author:         xiuyun.wang                                               *
** Date:           08/12/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app  store view function      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       xiuyun.wang          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_common.h"
#include "appstore_home_win_view.h"
#include "window_parse.h"
#include "mmi_text.h"
#include "guiwin.h"
#include "watch_common.h"
#include "watch_common_list.h"
#include "guilistbox.h"
#include "mmidisplay_color.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define APPSTORE_LIST_ITEM_CONTENT_0  (0)
#define APPSTORE_LIST_ITEM_CONTENT_3  (3)
#define APPSTORE_LIST_ITEM_CONTENT_4  (4)

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL uint16 s_app_num = 0;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : APPSTORE_HOME_FULL_PAINT
//  Parameter: [In] ctrl_id
//             [In] win_id
//             [Out] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL void HandleHomeWinFullPaint( MMI_CTRL_ID_T ctrl_id,MMI_WIN_ID_T win_id )
{
    uint16 idx = 0;
    uint16 cur_selection = 0;
    int32  list_Offset = 0;
    GUIANIM_FILE_INFO_T icon_info = {0};
    MMI_STRING_T str1 = {0};
    MMI_STRING_T str2 = {0};
    MMI_BUTTON_T button_info1 = {0};
    MMI_BUTTON_T button_info2 = {0};
    //Background color
    WATCHCOM_DisplayBackground( win_id );

    //get selected item
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

    //get top offset
    GUILIST_GetTopItemOffset(ctrl_id,&list_Offset);

    //remove list item
    GUILIST_RemoveAllItems(ctrl_id);

    //get app total num
    s_app_num=(uint16)APPSTORE_GetAppTotalNum();

    //Set the max item of list
    GUILIST_SetMaxItem(ctrl_id, s_app_num, TRUE);

    //初始化
    for(idx = 0; idx < s_app_num; idx++)
    {
        WatchCOM_ListItem_Draw_1Icon_2Str_2Button_Dyna(ctrl_id,icon_info,str1,str2,button_info1,button_info2,idx);
    }

    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

    //set top offset
    GUILIST_SetTopItemOffset(ctrl_id,list_Offset);

}

/*****************************************************************************/
//  Description : APPSTORE_HOME_OPEN_WINDOW
//  Parameter: [In] ctrl_id
//             [In] win_id
//             [Out] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL void HandleHomeWinOpenWin( MMI_CTRL_ID_T ctrl_id,MMI_WIN_ID_T win_id )
{
    GUI_COLOR_T font_color = MMI_WHITE_COLOR;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T          list_rect = WATCHLIST_RECT;
#endif
//原形表盘list空一段距离再画以避免显示不全
#ifdef SCREEN_SHAPE_CIRCULAR
        GUILIST_SetRect(APPSTORE_HOME_LIST_CTRL_ID,&list_rect);
#endif
    //set title font color
    GUIWIN_SetTitleFontColor(win_id,font_color);

    //set the active control
    MMK_SetAtvCtrl( win_id, ctrl_id );
}

/*****************************************************************************/
//  Description : APPSTORE_HOME_CTL_LIST_NEED_ITEM_DATA
//  Parameter: [In] ctrl_id
//             [In] item_index
//             [Out] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL void HandleHomeWinNeedItemData( MMI_CTRL_ID_T ctrl_id,uint16 item_index )
{
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_list_item_data = PNULL;

    //Get List Item Data
    APPSTORE_Data_GetListItemData(item_index,&p_list_item_data);
    if(PNULL == p_list_item_data)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:Get List Item Data fail");
        return;     //coverity bug1941690
    }

    //draw list ctrl--one icon, one string, one string, one button, one button
    WatchCOM_ListItem_Draw_1Icon_2Str_2Button_Dyna(ctrl_id,p_list_item_data->icon_info,p_list_item_data->str1,p_list_item_data->str2,p_list_item_data->button_info1,p_list_item_data->button_info2,item_index);
}

/*****************************************************************************/
//  Description : APPSTORE_HOME_CTL_LIST_NEED_ITEM_CONTENT_TRUE
//  Parameter: [In] ctrl_id
//             [In] item_index
//             [In] item_content_index
//             [Out] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL void HandleHomeWinNeedItemContent( MMI_CTRL_ID_T ctrl_id,uint16 item_index,uint16 item_content_index )
{
    GUIITEM_CONTENT_T item_content = {0};
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_list_item_data = PNULL;

    //Get List Item Data
    APPSTORE_Data_GetListItemData(item_index,&p_list_item_data);
    if(PNULL == p_list_item_data)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:Get List Item Data fail");
        return;     //coverity bug1941689
    }

    TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:item_content_index:%d",item_content_index);
    switch(item_content_index)
    {
        case APPSTORE_LIST_ITEM_CONTENT_0:
        {
            item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_content.item_data.anim_path_ptr = &(p_list_item_data->icon_info);
            break;
        }
        case APPSTORE_LIST_ITEM_CONTENT_3:
        {
            item_content.item_data_type    = GUIITEM_DATA_ANIM_PATH;
            item_content.item_data.anim_path_ptr = &(p_list_item_data->button_info1.image_info);
            break;
        }
        case APPSTORE_LIST_ITEM_CONTENT_4:
        {
            item_content.item_data_type    = GUIITEM_DATA_ANIM_PATH;
            item_content.item_data.anim_path_ptr = &(p_list_item_data->button_info2.image_info);
            break;
        }

        default:
        {
            TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:parameter error！item_content_index:%d",item_content_index);
            return;
        }
    }
    GUILIST_SetItemContent(ctrl_id, &item_content, item_index, item_content_index);
}

/*****************************************************************************/
//  Description : APPSTORE_HOME_CTL_LIST_NEED_ITEM_CONTENT
//  Parameter: [In] item_index
//             [In] item_content_index
//             [Out] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL void HandleHomeWinItemContentClick( uint16 item_index,uint16 item_content_index )
{
    TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:item_content_index:%d",item_content_index);
    switch(item_content_index)
    {
        case APPSTORE_LIST_ITEM_CONTENT_3:
        {
            //APP Store HomeWin Flow模块提供的处理左侧button（处理安装/更新）的函数
            APPSTORE_Flow_HandleHomeWinButton1(item_index);
            break;
        }
        case APPSTORE_LIST_ITEM_CONTENT_4:
        {
            //APP Store HomeWin Flow模块提供的处理右侧button（处理安装/更新）的函数
            APPSTORE_Flow_HandleHomeWinButton2(item_index);
            break;
        }
        default:
        {
            TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:parameter error！item_content_index:%d",item_content_index);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : Handle App Store Window
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAppStoreHomeWinMsg(MMI_WIN_ID_T       win_id,
                                                          MMI_MESSAGE_ID_E   msg_id,
                                                          DPARAM             param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = APPSTORE_HOME_LIST_CTRL_ID;
    TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:enter win succ，msg_id = 0x%x",msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUILIST_SetSyncDecodeAnim(ctrl_id,TRUE);
            GUILIST_SetZoomAnim(ctrl_id,TRUE);
            GUILIST_SetDefaultIconEx(ctrl_id,NULL,NULL);
            HandleHomeWinOpenWin( ctrl_id,win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            HandleHomeWinFullPaint( ctrl_id,win_id );
            break;
        }
        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *p_item_data = (GUILIST_NEED_ITEM_DATA_T*)param;
            if(PNULL == p_item_data)
            {
                result = MMI_RESULT_FALSE;
                TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:param error");
            }
            else
            {
                HandleHomeWinNeedItemData( ctrl_id,p_item_data->item_index);
            }
            break;
        }
        case MSG_CTL_LIST_ITEM_CONTENT_CLICK:
        {
            GUILIST_ITEM_CONTENT_CLICK_T *p_item_content = (GUILIST_ITEM_CONTENT_CLICK_T*)param;
            if(PNULL == p_item_content)
            {
                result = MMI_RESULT_FALSE;
                TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:param error");
            }
            else
            {
                HandleHomeWinItemContentClick( p_item_content->item_index,p_item_content->item_content_index);
            }
            break;
        }
        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            GUILIST_NEED_ITEM_CONTENT_T *p_item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL == p_item_content)
            {
                result = MMI_RESULT_FALSE;
                TRACE_APPSTORE("[APPSTORE_HOME_WIN_VIEW]:param error");
            }
            else
            {
                HandleHomeWinNeedItemContent( ctrl_id,p_item_content->item_index,p_item_content->item_content_index);
            }
            break;
        }
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}

WINDOW_TABLE( MMISET_APPSTORE_HOME_WIN_TAB ) =
{
    WIN_ID( APPSTORE_HOME_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE( TXT_MAINMENU_LIST_APPSTORE ),
#endif
    WIN_FUNC( (uint32)HandleAppStoreHomeWinMsg ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, APPSTORE_HOME_LIST_CTRL_ID),
    END_WIN
};

/*****************************************************************************/
//  Description : APPSTORE_Home_Win_Enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_Home_Win_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_APPSTORE_HOME_WIN_TAB, PNULL );
}
