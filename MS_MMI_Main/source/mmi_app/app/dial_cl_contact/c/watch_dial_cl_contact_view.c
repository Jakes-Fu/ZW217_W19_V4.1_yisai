/*****************************************************************************
** File Name:      watch_dial_cl_contact_view.c                              *
** Author:         feiyue.ji                                                 *
** Date:           18/1/2021                                                 *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw dial&calllog&contact interface  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 01/2021               feiyue.ji            Create                         *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------——-----*/
#include "std_header.h"
#include "window_parse.h"
#include "mmicc_id.h"
#include "ctrllist_export.h"
#include "mmitheme_list.h"
// TODO:cc text image,just for test
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "guilistbox.h"
#include "watch_dial_cl_contact_internal.h"
#include "mmicc_export.h"
#include "mmidisplay_color.h"
#include "mmidisplay_font.h"
#include "watch_common_list.h"
#include "watch_cc_view.h"

#define DIALLIST_ITEM_WIDTH DP2PX_VALUE(240)//item的宽
#define DIALLIST_ITEM_HIGHT DP2PX_VALUE(84)//item的高
#define DIALLIST_ITEM_TOP_SPACE DP2PX_VALUE(24)//item的top
#define DIALLIST_ITEM_BOTTOM_SPACE DP2PX_VALUE(12)//item的bottom space

#define DIALLIST_ICON_HIGHT DP2PX_VALUE(48)//icon的高
#define DIALLIST_ICON_WIDTH DP2PX_VALUE(48)//icon的宽
#define DIALLIST_ITEM_LEFT_SPACE ((DIALLIST_ITEM_WIDTH-DIALLIST_ICON_WIDTH)/2)
#define DIALLIST_ITEM_RIGHT_SPACE ((DIALLIST_ITEM_WIDTH-DIALLIST_ICON_WIDTH)/2 + DIALLIST_ICON_WIDTH)

#define WATCH_DIAL_LIST_INDEX 0
#define DIAL_ITEM_COUNT 1

LOCAL int32 offset_y = 0;

//dial的item layout布局
LOCAL THEMELIST_ITEM_STYLE_T s_dial_liststyle = {
    DIALLIST_ITEM_HIGHT, DIALLIST_ITEM_HIGHT,

    {
        {
            0,
            {DIALLIST_ITEM_LEFT_SPACE, DIALLIST_ITEM_TOP_SPACE,  DIALLIST_ITEM_RIGHT_SPACE,
            DIALLIST_ITEM_HIGHT - DIALLIST_ITEM_BOTTOM_SPACE},
            {DIALLIST_ITEM_LEFT_SPACE, DIALLIST_ITEM_TOP_SPACE,  DIALLIST_ITEM_RIGHT_SPACE, DIALLIST_ITEM_HIGHT},
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

LOCAL THEMELIST_ITEM_STYLE_T   s_title_style = {0};

LOCAL void AppendTitleItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T titleStr)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};
    THEMELIST_ITEM_STYLE_T* itemstyle_ptr = THEMELIST_GetStyle(GUIITEM_STYLE_1STR_1LINE_CENTERALIGN_ADULTWATCH);
    uint16 item_index = 0;

    if(PNULL == itemstyle_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("title item style error");
        return;
    }

    item_t.item_style = GUIITEM_STYLE_1STR_1LINE_CENTERALIGN_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = titleStr;
    item_data.item_content[0].font_color_id = MMITHEME_COLOR_BLUE;

    //不画分割线
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
    //不画高亮条
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );

    if(!GUILIST_AppendItem(ctrl_id, &item_t))
    {
        TRACE_APP_DIAL_CL_CONTACT("Append Failed");
    }
    item_index = GUILIST_GetTotalItemNum(ctrl_id)-1;

    SCI_MEMCPY(&s_title_style, itemstyle_ptr, sizeof(THEMELIST_ITEM_STYLE_T));
    s_title_style.content[0].font = ADULTWATCHLIST_ITEM_SMALL_FONT;
    s_title_style.content[0].font_focus = ADULTWATCHLIST_ITEM_SMALL_FONT;
    GUILIST_SetItemStyleEx(ctrl_id, item_index, &s_title_style);
}
/*****************************************************************************/
//  Description : 添加dial的item项
//  Global resource dependence :
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void AppendDialItem(MMI_CTRL_ID_T ctrl_id)
{
    // TODO:后续需要修改图片
    AdultWatchCOM_ListItem_Draw_1Icon_1line(ctrl_id, res_aw_call_ic_dialpad);
    GUILIST_SetItemStyleEx(ctrl_id, WATCH_DIAL_LIST_INDEX, &s_dial_liststyle);
}

/*****************************************************************************/
//  Description : 添加calllog的item项
//  Global resource dependence :
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void AppendCalllogItem(MMI_CTRL_ID_T ctrl_id, CL_VIEW_DATA_INFO_T* view_data_info_ptr)
{
    MMI_STRING_T    titleStr = {0};
    MMI_STRING_T    name = {0};
    MMI_STRING_T    time = {0};
    uint16 i = 0;
    //add title
    // TODO:后续需要修改字串
    MMI_GetLabelTextByLang(TXT_ADULT_WATCH_CALLLOG, &titleStr);
    AppendTitleItem(ctrl_id, titleStr);
    //add calllog content

    if((PNULL == view_data_info_ptr)
        || (0 == view_data_info_ptr->calllog_count))
    {
        TRACE_APP_DIAL_CL_CONTACT("VIEW DATA is NULL!");
        MMI_GetLabelTextByLang(TXT_ADULT_WATCH_CALLLOG_EMPTY, &titleStr);
        AdultWatchCOM_ListItem_Draw_1StrCenterAlign(ctrl_id, titleStr);
    }
    else
    {
        for(i=0; i<view_data_info_ptr->calllog_count; i++)
        {
            GUILIST_ITEM_T          item_t          = {0};
            GUILIST_ITEM_DATA_T     item_data       = {0};
            MMITHEME_COLOR_ID_E     name_color_id = 0;
            uint16 wstr_len = 0;

            if(TRUE == view_data_info_ptr->calllog_view_data_array[i].is_unread_missed_call)
            {
                name_color_id = MMITHEME_COLOR_RED;
            }
            else
            {
                name_color_id = MMITHEME_COLOR_NONE;
            }
            wstr_len = MMIAPICOM_Wstrlen(view_data_info_ptr->calllog_view_data_array[i].datetime_wstr);
            time.wstr_len = MIN(WATCH_DATA_TIME_STR_MAX_LEN, wstr_len);
            time.wstr_ptr = view_data_info_ptr->calllog_view_data_array[i].datetime_wstr;
            name.wstr_ptr = view_data_info_ptr->calllog_view_data_array[i].disp_str;
            name.wstr_len = MMIAPICOM_Wstrlen(name.wstr_ptr);

            item_t.item_style = GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH;
            item_t.item_data_ptr = &item_data;
            item_t.user_data = (uint32)(view_data_info_ptr->calllog_view_data_array[i].number);

            item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id        = view_data_info_ptr->calllog_view_data_array[i].calltype_image_id;

            item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer     = name;
            item_data.item_content[1].font_color_id = name_color_id;

            item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[2].item_data.text_buffer     = time;
            item_data.item_content[2].font_color_id = MMITHEME_COLOR_GRAY;

            //不画分割线
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
            //不画高亮条
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );

            if(!GUILIST_AppendItem(ctrl_id, &item_t))
            {
                TRACE_APP_DIAL_CL_CONTACT("AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line Append Failed");
            }

        }

    }


}

/*****************************************************************************/
//  Description : 添加contact的item项
//  Global resource dependence :
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void AppendContactItem(MMI_CTRL_ID_T ctrl_id, CONTACT_VIEW_DATA_INFO_T* contact_data_info_ptr )
{
    MMI_STRING_T    titleStr = {0};
    MMI_STRING_T    name = {0};
    uint16 i = 0;
    //add title
    // TODO:后续需要修改字串
    MMI_GetLabelTextByLang(TXT_COMMON_CONTACT, &titleStr);
    AppendTitleItem(ctrl_id, titleStr);

    if((PNULL == contact_data_info_ptr)
        || (0 == contact_data_info_ptr->contact_count))
    {
        TRACE_APP_DIAL_CL_CONTACT("VIEW DATA is NULL!");
        // TODO: add empty string item
        MMI_GetLabelTextByLang(TXT_ADULT_WATCH_CONTACT_EMPTRY, &titleStr);
        AdultWatchCOM_ListItem_Draw_1StrCenterAlign(ctrl_id, titleStr);
    }
    else
    {
        //add contact content
        for(i=0; i<contact_data_info_ptr->contact_count; i++)
        {
            GUILIST_ITEM_T          item_t          = {0};
            GUILIST_ITEM_DATA_T     item_data       = {0};
            uint16 wstr_len = 0;
            name.wstr_ptr = contact_data_info_ptr->contact_view_data_array[i].name_str;
            wstr_len = MMIAPICOM_Wstrlen(contact_data_info_ptr->contact_view_data_array[i].name_str);
            name.wstr_len= MIN(wstr_len, DISPLAY_STR_MAX_LEN);

            item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH;
            item_t.item_data_ptr = &item_data;
            item_t.user_data = (uint32)(contact_data_info_ptr->contact_view_data_array[i].number);

            item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id        = res_aw_contact_portrait;

            item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer     = name;

            //不画分割线
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
            //不画高亮条
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );

            if(!GUILIST_AppendItem(ctrl_id, &item_t))
            {
                TRACE_APP_DIAL_CL_CONTACT("Append Failed");
            }

            }

        }
}

/*****************************************************************************/
//  Description : 添加dial calllog contact的item项
//  Global resource dependence :
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void AppendListItem(MMI_CTRL_ID_T ctrl_id, DIAL_CL_CONTACT_VIEW_DATA_T* viewdata_ptr)
{
    if(PNULL == viewdata_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("viewdata_ptr is PNULL!");
        return;
    }
    GUILIST_RemoveAllItems(ctrl_id);
    AppendDialItem(ctrl_id);
    AppendCalllogItem(ctrl_id, viewdata_ptr->calllog_view_data_ptr);
    AppendContactItem(ctrl_id, viewdata_ptr->contact_view_data_ptr);
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );

}

LOCAL void ReloadData(DIAL_CL_CONTACT_VIEW_DATA_T* viewdata_ptr)
{
    uint16        max_item_num = 0;
    MMI_CTRL_ID_T     ctrl_id = WATCH_DIAL_CL_CONTACT_LIST_ID;

    if(PNULL == viewdata_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("param is error!");
    }

    Watch_Dial_Cl_Contact_LoadViewData(viewdata_ptr);
    max_item_num = DIAL_ITEM_COUNT + 2;// 2代表calllog和contact的title


    //calllog 内容的count
    if((PNULL == viewdata_ptr->calllog_view_data_ptr)
        || (0 == viewdata_ptr->calllog_view_data_ptr->calllog_count))
    {
        TRACE_APP_DIAL_CL_CONTACT("calllog is empty!");
        max_item_num = max_item_num + 1;// 1代表empty字串item
    }
    else
    {
        max_item_num = max_item_num +
        viewdata_ptr->calllog_view_data_ptr->calllog_count;
    }

    //contact 内容的count
    if((PNULL == viewdata_ptr->contact_view_data_ptr)
       ||(0 == viewdata_ptr->contact_view_data_ptr->contact_count))
    {
        TRACE_APP_DIAL_CL_CONTACT("contact is empty!");
        max_item_num = max_item_num + 1;// 1代表empty字串item
    }
    else
    {
        max_item_num = max_item_num + viewdata_ptr->contact_view_data_ptr->contact_count;
    }

    GUILIST_SetMaxItem(ctrl_id, max_item_num, FALSE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_EFFECT_STR, TRUE);

    AppendListItem(ctrl_id, viewdata_ptr);
}

LOCAL void Watch_Dial_Cl_ContactWin_CloseWindow(
                                          MMI_WIN_ID_T win_id,
                                          DIAL_CL_CONTACT_VIEW_DATA_T* viewdata_ptr
                                          )
{

    Watch_Dial_Cl_Contact_DestroyViewData(viewdata_ptr);
    //save call log
    Watch_Dial_Cl_Contact_SaveMissedCallData();
}

LOCAL void HandleWebKeyMsg(
                                       MMI_WIN_ID_T win_id,
                                       DIAL_CL_CONTACT_VIEW_DATA_T* viewdata_ptr
                                       )
{
    MMI_CTRL_ID_T     ctrl_id = WATCH_DIAL_CL_CONTACT_LIST_ID;
    uint16 cur_index = 0;
    uint32 usedata = 0;
    uint8 tele_len = 0;
    CC_RESULT_E ret = CC_RESULT_ERROR;

    cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    TRACE_APP_DIAL_CL_CONTACT("cur index:%d", cur_index);

    if(WATCH_DIAL_LIST_INDEX == cur_index)//open dial win
    {
        WatchCC_DialpadWin_Enter();
    }
    else//make call
    {
        CTRLLIST_GetItemData(ctrl_id,cur_index,&usedata);
        if(0 == usedata)
        {
            TRACE_APP_DIAL_CL_CONTACT("no user data");
            return;
        }
        tele_len = strlen((char*)usedata);
        if(0 == tele_len)
        {
            TRACE_APP_DIAL_CL_CONTACT("tele num error len :%d",tele_len);
            return;
        }
        WatchAPICC_DialNumber((uint8*)usedata,tele_len);
    }
}

/*****************************************************************************/
//  Description : handle dial_cl_contact Window messages
//  Global resource dependence :
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleDial_Cl_ContactWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E      result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T     ctrl_id = WATCH_DIAL_CL_CONTACT_LIST_ID;
    DIAL_CL_CONTACT_VIEW_DATA_T* viewdata_ptr = PNULL;

    viewdata_ptr = (DIAL_CL_CONTACT_VIEW_DATA_T*)MMK_GetWinUserData(win_id);
    if(PNULL == viewdata_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("get window adddata fail!");
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            ReloadData(viewdata_ptr);
            offset_y = 0;
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            Watch_Dial_Cl_ContactWin_CloseWindow(win_id,viewdata_ptr);
            break;
        }

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_NOTIFY_PENOK:
        {
            HandleWebKeyMsg(win_id, viewdata_ptr);
            break;
        }
        case MMI_CL_RELOAD_DATA:
        {
            ReloadData(viewdata_ptr);
            CTRLLIST_SetTopItemOffset(ctrl_id,offset_y);
            break;
        }
        case MSG_LOSE_FOCUS:
        {
            CTRLLIST_GetTopItemOffset(ctrl_id, &offset_y);
            //save call log
            Watch_Dial_Cl_Contact_SaveMissedCallData();
            break;
        }
        case MSG_GET_FOCUS://fix bug1594837
        {
            MMK_SendMsg(win_id, MMI_CL_RELOAD_DATA, PNULL);
            break;
        }
        case MSG_KEYDOWN_USER_FUNC://fix bug1594837 1595403 1586984
        {
            MMK_SendMsg(win_id, MSG_APP_DOWN, PNULL);
            break;
        }

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
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
/**---------------------------------------------------------------------------*
**                         win table                               *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( WATCH_DIAL_CL_CONTACT_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleDial_Cl_ContactWinMsg),
    WIN_ID(WATCH_DIAL_CL_CONTACT_WIN_ID),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,WATCH_DIAL_CL_CONTACT_LIST_ID),
    END_WIN
};

PUBLIC void Watch_Dial_Cl_Contact_Win_Enter(void)
{
    DIAL_CL_CONTACT_VIEW_DATA_T* viewdata_ptr = PNULL;
    MMI_HANDLE_T      win_handle = 0;

    win_handle = MMK_CreateWin((uint32 *)WATCH_DIAL_CL_CONTACT_WIN_TAB, PNULL);

    if(0 != win_handle)
    {
        viewdata_ptr = Watch_Dial_Cl_Contact_CreateViewData();
        if(PNULL == viewdata_ptr)
        {
            TRACE_APP_DIAL_CL_CONTACT("create data fail, can not enter window!");
            return;
        }
        MMK_SetWinUserData(win_handle, viewdata_ptr);
    }
    else
    {
        TRACE_APP_DIAL_CL_CONTACT("create  window fail!");
    }

}


