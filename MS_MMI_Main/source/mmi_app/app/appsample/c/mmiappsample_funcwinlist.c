/*****************************************************************************
 ** File Name:   mmiappsample_funcwinlist.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe funcwin list sample code
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/


#include "mmiappsample_internal.h"
#include "mmiappsample_text.h"
#include "mmiappsample_id.h"
#include "mmiappsample_image.h"

#include "srvsample_export.h"

#include "mmi_pubmenuwin.h"
#include "window_parse.h"
#include "mmk_msg.h"

#include "guilistbox.h"

typedef struct
{
    uint16 cursel_index;
} MMIAPPSAMPLE_FUNCWINLIST_DATA_T;


/******************************************************************************
//  Description : win handler
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinListWinMsg(MMI_WIN_ID_T win_id,
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param
                                                    );


//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_FUNCWINLIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleFuncWinListWinMsg),
    WIN_ID( MMIAPPSAMPLE_FUNCWINLIST_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_FUNCWINLIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_FUNCWINLIST_LIST_CTRL_ID),
    END_WIN
};


/******************************************************************************
//  Description : funcwinlist win entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFuncWinListWin(void)
{
    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE EnterAppSampleFuncWinListWin");

    MMK_CreateWin((uint32*)MMIAPPSAMPLE_FUNCWINLIST_WIN_TAB, PNULL);
    return;
}


/******************************************************************************
//  Description : Open ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E FuncWinList_OpenCtrlFunc(MMI_WIN_ID_T win_id) //OpenCtrl
{

    MMIAPPSAMPLE_FUNCWINLIST_DATA_T *win_data_p = PNULL;

    win_data_p = (MMIAPPSAMPLE_FUNCWINLIST_DATA_T*)SCI_ALLOCAZ(sizeof(MMIAPPSAMPLE_FUNCWINLIST_DATA_T));

    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(0x01);
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    MMK_SetWinUserData(win_id, (ADD_DATA)(win_data_p));     //Set Windata

    MMK_SetWinDirty(win_id, TRUE);

    return MMI_RESULT_TRUE;
}

/******************************************************************************
//  Description : close ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void FuncWinList_CloseCtrlFunc(MMI_WIN_ID_T win_id)    //CloseCtrl
{
    MMIAPPSAMPLE_FUNCWINLIST_DATA_T *win_data_p = (MMIAPPSAMPLE_FUNCWINLIST_DATA_T*)MMK_GetWinUserData(win_id);

    if (win_data_p != PNULL)
    {
        SCI_FREE(win_data_p);
        return ;
    }

    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE FuncWinList_CloseCtrlFunc win_data_p = PNULL");

    return ;
}

const MMI_TEXT_ID_T list_entry_id[] =
{
    TXT_APPSAMPLE_FUNCWIN_MODEA,
    TXT_APPSAMPLE_FUNCWIN_MODEB,
    TXT_APPSAMPLE_FUNCWIN_MODEC,
};


/******************************************************************************
//  Description : list view ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void FuncWinList_ListViewFunc(MMI_WIN_ID_T win_id)     //View func
{
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FUNCWINLIST_LIST_CTRL_ID;
    MMIAPPSAMPLE_FUNCWINLIST_DATA_T *win_data_p = (MMIAPPSAMPLE_FUNCWINLIST_DATA_T*)MMK_GetWinUserData(win_id);

    uint8 i = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

//    MMI_STRING_T        show_textstr = {0};
//    wchar               list_text_wchar[20] = {0};
//    char                list_text_char[20] = {0};


    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(0x01);
        MMK_CloseWin(win_id);
        return;
    }

    GUILIST_SetMaxItem(list_ctrl, 3, FALSE);
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    //set list item
    for ( i = 0; i < 3; i++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = list_entry_id[i] ;
        item_t.user_data = (uint32)i;
        GUILIST_AppendItem( list_ctrl, &item_t );

    }
    MMK_SetAtvCtrl(win_id, list_ctrl);

    GUILIST_SetCurItemIndex(list_ctrl, win_data_p->cursel_index);
    //Set Windirty
    MMK_SetWinDirty(win_id, FALSE);
}

/******************************************************************************
//  Description : change list index
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
void FuncWinList_ChangeListIndex(MMI_HANDLE_T win_handle, uint16 sel_index)
{
    MMIAPPSAMPLE_FUNCWINLIST_DATA_T *win_data_p = PNULL;
    win_data_p = (MMIAPPSAMPLE_FUNCWINLIST_DATA_T*)MMK_GetWinUserData(win_handle);

    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(0x01);
        MMK_CloseWin(win_handle);
        return ;
    }
    win_data_p->cursel_index = sel_index;
    MMK_SetWinDirty(win_handle, TRUE);

    MMK_PostMsg(win_handle, MSG_FULL_PAINT, PNULL, 0);

    return;
}

/******************************************************************************
//  Description : entry func win of selected index
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void  EntryFuncWinMode(MMI_WIN_ID_T win_id, uint16 cursel_index)
{
    if (cursel_index == 0)
    {
        EnterAppSampleFuncWinModeAWin();
    }
    else if (cursel_index == 1)
    {
        EnterAppSampleFuncWinModeBWin(cursel_index);
    }
    else if (cursel_index == 2)
    {
        EnterAppSampleFuncWinModeCWin(win_id, cursel_index, FuncWinList_ChangeListIndex);
    }
    else
    {
        MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE EntryFuncWinMode error select index");
    }

}


/******************************************************************************
//  Description : win handler
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinListWinMsg(MMI_WIN_ID_T win_id,
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param
                                                    )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FUNCWINLIST_LIST_CTRL_ID;
    MMIAPPSAMPLE_FUNCWINLIST_DATA_T *win_data_p = (MMIAPPSAMPLE_FUNCWINLIST_DATA_T*)MMK_GetWinUserData(win_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (!FuncWinList_OpenCtrlFunc(win_id))
            {
                break;
            }

            break;

        case MSG_FULL_PAINT:
            if (MMK_GetWinDirty(win_id))
            {
                FuncWinList_ListViewFunc(win_id);
            }
            break;

        case MSG_CTL_PENOK:
            win_data_p->cursel_index = GUILIST_GetCurItemIndex(list_ctrl);
            EntryFuncWinMode(win_id, win_data_p->cursel_index);

            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            FuncWinList_CloseCtrlFunc(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

