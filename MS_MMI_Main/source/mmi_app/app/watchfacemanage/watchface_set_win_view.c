/*****************************************************************************
** File Name:      watchface_set_win_view.c                                  *
** Author:         bin.wang1                                                 *
** Date:           08/17/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define watch face set view function  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       bin.wang1            Create                                  *
******************************************************************************/
#ifdef WATCHFACE_MANAGE_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "guiwin.h"
#include "watch_common.h"
#include "watch_common_list.h"
#include "guilistbox.h"

#include "watchface_set_win_view.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define WATCHFACE_LIST_ITEM_CONTENT_0  (0)
#define WATCHFACE_LIST_ITEM_CONTENT_3  (3)
#define WATCHFACE_LIST_ITEM_CONTENT_4  (4)

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : HandleSetWinOpenWin
//  Parameter: [In] win_id, ctrl_id
//             [Out] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL void HandleSetWinOpenWin( MMI_CTRL_ID_T ctrl_id, MMI_WIN_ID_T win_id )
{
    GUI_COLOR_T     font_color = MMI_WHITE_COLOR;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T          list_rect = WATCHLIST_RECT;
#endif
    TRACE_WATCHFACE(" enter ");

    //原形表盘list空一段距离再画以避免显示不全
#ifdef SCREEN_SHAPE_CIRCULAR
    GUILIST_SetRect(WATCH_FACE_SET_LIST_CTRL_ID,&list_rect);
#endif

    //set title font color
    GUIWIN_SetTitleFontColor(win_id,font_color);

    //set the active control
    MMK_SetAtvCtrl( win_id, ctrl_id );

    //同步刷新
    GUILIST_SetSyncDecodeAnim( ctrl_id, TRUE );

    //无图片资源时什么都不显示
    GUILIST_SetDefaultIconEx( ctrl_id, NULL, NULL );

    //无图片资源时什么都不显示
    GUILIST_SetZoomAnim( ctrl_id, TRUE );

    //设置滚动
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );
}

/*****************************************************************************/
//  Description : HandleSetWinFullPaint
//  Parameter: [In] win_id, ctrl_id
//             [Out] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL void HandleSetWinFullPaint( MMI_CTRL_ID_T ctrl_id, MMI_WIN_ID_T win_id )
{
    uint16              index = 0;
    uint16              cur_selection = 0;
    int32               list_Offset = 0;
    uint16              total_num = 0;
    MMI_STRING_T        str1 = {0};
    MMI_STRING_T        str2 = {0};
    MMI_BUTTON_T        button_info1 = {0};
    MMI_BUTTON_T        button_info2 = {0};
    GUIANIM_FILE_INFO_T icon_info = {0};

    TRACE_WATCHFACE(" enter ");

    //get selected item
    cur_selection = GUILIST_GetCurItemIndex( ctrl_id );

    //get top offset
    GUILIST_GetTopItemOffset( ctrl_id, &list_Offset );

    //remove list item
    GUILIST_RemoveAllItems( ctrl_id );


    total_num = WATCHFACE_Set_Data_GetListItemNum();

    //Set the max item of list
    GUILIST_SetMaxItem( ctrl_id, total_num, TRUE );

    for( index = 0; index < total_num; index++ )
    {
        WatchCOM_ListItem_Draw_1Icon_2Str_2Button_Dyna(
                                                ctrl_id,
                                                icon_info,
                                                str1,
                                                str2,
                                                button_info1,
                                                button_info2,
                                                index);
    }

    //set selected item
    GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE) ;

    //set current item
    GUILIST_SetCurItemIndex( ctrl_id, cur_selection );

    //set top offset
    GUILIST_SetTopItemOffset( ctrl_id,list_Offset );

}

/*****************************************************************************/
//  Description : HandleSetWinFullPaint
//  Parameter: [In] item_index, ctrl_id
//             [Out] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL void HandleSetWinNeedItemData( MMI_CTRL_ID_T ctrl_id,uint16 item_index )
{
    WATCHFACE_STATUS_RES_E res = WATCHFACE_STATUS_RES_SUCCESS;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_list_item_data = PNULL;

    TRACE_WATCHFACE(" enter ");

    //Get List Item Data
    res = WATCHFACE_Set_Data_GetListItemData( item_index, &p_list_item_data );

    if( (WATCHFACE_STATUS_RES_SUCCESS != res) || (PNULL == p_list_item_data) )
    {
        return;
    }

    //draw list ctrl--one icon, one string, one string, one button, one button
    WatchCOM_ListItem_Draw_1Icon_2Str_2Button_Dyna(
                                            ctrl_id,
                                            p_list_item_data->icon_info,
                                            p_list_item_data->str1,
                                            p_list_item_data->str2,
                                            p_list_item_data->button_info1,
                                            p_list_item_data->button_info2,
                                            item_index);
}

/*****************************************************************************/
//  Description : HandleSetWinFullPaint
//  Parameter: [In] item_content_index, item_index, ctrl_id
//             [Out] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL void HandleSetWinNeedItemContent( MMI_CTRL_ID_T ctrl_id, uint16 item_index, uint16 item_content_index )
{
    GUIITEM_CONTENT_T       item_content = { 0 };
    WATCHFACE_STATUS_RES_E  res = WATCHFACE_STATUS_RES_SUCCESS;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_list_item_data = PNULL;

    TRACE_WATCHFACE(" enter ");

    //Get List Item Data
    res = WATCHFACE_Set_Data_GetListItemData( item_index, &p_list_item_data );//bug id 1941703 CID 420807
    if( (WATCHFACE_STATUS_RES_SUCCESS != res) || (PNULL == p_list_item_data) )
    {
        return;
    }

    switch( item_content_index )
    {
        case WATCHFACE_LIST_ITEM_CONTENT_0:
        {
            item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_content.item_data.anim_path_ptr = &(p_list_item_data->icon_info);
            break;
        }
        case WATCHFACE_LIST_ITEM_CONTENT_3:
        {
            item_content.item_data_type    = GUIITEM_DATA_ANIM_PATH;
            item_content.item_data.anim_path_ptr = &(p_list_item_data->button_info1.image_info);
            break;
        }
        case WATCHFACE_LIST_ITEM_CONTENT_4:
        {
            item_content.item_data_type    = GUIITEM_DATA_ANIM_PATH;
            item_content.item_data.anim_path_ptr = &(p_list_item_data->button_info2.image_info);
            break;
        }

        default:
        {
            TRACE_WATCHFACE("parameter error！item_content_index:%d", item_content_index);
            break;
        }
    }

    GUILIST_SetItemContent(ctrl_id, &item_content, item_index, item_content_index);
}

/*****************************************************************************/
//  Description : HandleSetWinFullPaint
//  Parameter: [In] item_content_index, item_index,
//             [Out] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL void HandleSetWinItemContentClick( uint16 item_index,uint16 item_content_index )
{
    TRACE_WATCHFACE(" enter ");

    switch( item_content_index )
    {
        case WATCHFACE_LIST_ITEM_CONTENT_3:
        {
            //Watch face set win Flow模块提供的处理左侧button（设置为当前）的函数
            WATCHFACE_Set_Flow_HandleListItemButton1( item_index );
            break;
        }
        case WATCHFACE_LIST_ITEM_CONTENT_4:
        {
            //Watch face set win Flow模块提供的处理右侧button（删除）的函数
            WATCHFACE_Set_Flow_HandleListItemButton2( item_index );
            break;
        }
        default:
        {
            TRACE_WATCHFACE("parameter error！item_content_index:%d",item_content_index);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : Handle watch face set window
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWatchFaceSetWinMsg(
                                         MMI_WIN_ID_T       win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    result =  MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = WATCH_FACE_SET_LIST_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            HandleSetWinOpenWin( ctrl_id, win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            HandleSetWinFullPaint( ctrl_id, win_id );
            break;
        }

        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *p_item_data = (GUILIST_NEED_ITEM_DATA_T*)param;

            if( PNULL == p_item_data )
            {
                result = MMI_RESULT_FALSE;
                TRACE_WATCHFACE("param error");
            }
            else
            {
                HandleSetWinNeedItemData( ctrl_id, p_item_data->item_index);
            }
            break;

        }

        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            GUILIST_NEED_ITEM_CONTENT_T *p_item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;

            if( PNULL == p_item_content )
            {
                result = MMI_RESULT_FALSE;
                TRACE_WATCHFACE("param error");
            }
            else
            {
                HandleSetWinNeedItemContent( ctrl_id, p_item_content->item_index, p_item_content->item_content_index);
            }
            break;
        }

        case MSG_CTL_LIST_ITEM_CONTENT_CLICK:
        {
            GUILIST_ITEM_CONTENT_CLICK_T *p_item_content = (GUILIST_ITEM_CONTENT_CLICK_T*)param;

            if(PNULL == p_item_content)
            {
                result = MMI_RESULT_FALSE;
                TRACE_WATCHFACE("param error");
            }
            else
            {
                HandleSetWinItemContentClick( p_item_content->item_index, p_item_content->item_content_index);
            }
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            WATCHFACE_Set_Data_DestroyWinData();
            break;
        }

        default:
        {
            TRACE_WATCHFACE(" msg_id 0x%x ", msg_id);
            result = MMI_RESULT_FALSE;
            break;
        }
    }

    return result;
}

/**--------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/

WINDOW_TABLE( WATCHFACE_SET_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleWatchFaceSetWinMsg ),
    WIN_ID( WATCHFACE_SET_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_MAINMENU_LIST_WATCH_SET),
#endif
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, WATCH_FACE_SET_LIST_CTRL_ID ),
    END_WIN
};

/****************************************************************************/
//  Description : enter watch face settings window
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void WATCHFACE_Set_Win_Enter( void )
{
    TRACE_WATCHFACE(" enter ");

    MMK_CreateWin((uint32*)WATCHFACE_SET_WIN_TAB, PNULL);
}
#endif
