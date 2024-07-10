/*****************************************************************************
 ** File Name:   mmiappsample_funcwinb.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe funcwin B sample
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


#include "window_parse.h"
#include "mmk_msg.h"

#include "guilistbox.h"

typedef struct
{
    uint16 input_index;
    uint16 cur_sel;
} MMIAPPSAMPLE_FUNCWINMODEB_DATA_T;


/******************************************************************************
//  Description : win_handler
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinModeBWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                     );


//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_FUNCWIN_MODEB_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleFuncWinModeBWinMsg),
    WIN_ID( MMIAPPSAMPLE_FUNCWINMODEB_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_FUNCWIN_MODEB),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_FUNCWINMODEB_LIST_CTRL_ID),
    END_WIN
};


/******************************************************************************
//  Description : FuncWin B Entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFuncWinModeBWin(uint16 sel_index)
{
    MMI_HANDLE_T win_handle;
    MMIAPPSAMPLE_FUNCWINMODEB_DATA_T *win_data_p = PNULL;

    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE EnterAppSampleFuncWinModeBWin");

    win_data_p = (MMIAPPSAMPLE_FUNCWINMODEB_DATA_T*)SCI_ALLOCAZ(sizeof(MMIAPPSAMPLE_FUNCWINMODEB_DATA_T));

    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(1);
        return;
    }
    win_data_p->input_index = sel_index;
    win_handle  = MMK_CreateWin((uint32*)MMIAPPSAMPLE_FUNCWIN_MODEB_WIN_TAB, PNULL);

    if (win_handle == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(1);
        SCI_FREE(win_data_p);
        return;
    }
    MMK_SetWinUserData(win_handle, win_data_p);

    return;
}

/******************************************************************************
//  Description : FuncWin B Open ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E FuncWinModeBWin_OpenCtrlFunc(MMI_WIN_ID_T win_id)
{
    MMIAPPSAMPLE_FUNCWINMODEB_DATA_T *win_data_p = PNULL;

    win_data_p = (MMIAPPSAMPLE_FUNCWINMODEB_DATA_T*)MMK_GetWinUserData(win_id);

    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(1);
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    win_data_p->cur_sel = win_data_p->input_index;
    MMK_SetWinDirty(win_id, TRUE);

    return MMI_RESULT_TRUE;
}

/******************************************************************************
//  Description : FuncWin B close ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void FuncWinModeBWin_CloseCtrlFunc(MMI_WIN_ID_T win_id)
{
    return ;
}


/******************************************************************************
//  Description : FuncWin B List view
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void FuncWinModeBWin_ListViewFunc(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FUNCWINMODEB_LIST_CTRL_ID;

    uint8 i = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
//    MMI_STRING_T        show_textstr = {0};
//    wchar               list_text_wchar[20] = {0};
//    char                list_text_char[20] = {0};


    GUILIST_SetMaxItem(list_ctrl, 3, FALSE);
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    //set list item
    for ( i = 0; i < 3; i++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_APPSAMPLE_INPUT_A + i;
        item_t.user_data = (uint32)i;
        GUILIST_AppendItem( list_ctrl, &item_t );
    }
    MMK_SetAtvCtrl(win_id, list_ctrl);


    //Set Windirty
    MMK_SetWinDirty(win_id, FALSE);
}


/********************************************************************
//  Description :funcWinB Win handler
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinModeBWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                     )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FUNCWINMODEB_LIST_CTRL_ID;
    MMIAPPSAMPLE_FUNCWINMODEB_DATA_T *win_data_p = (MMIAPPSAMPLE_FUNCWINMODEB_DATA_T*)MMK_GetWinUserData(win_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (!FuncWinModeBWin_OpenCtrlFunc(win_id))
            {
                break;
            }

            break;

        case MSG_FULL_PAINT:
            if (MMK_GetWinDirty(win_id))
            {
                FuncWinModeBWin_ListViewFunc(win_id);
                GUILIST_SetCurItemIndex(list_ctrl, win_data_p->cur_sel);
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            FuncWinModeBWin_CloseCtrlFunc(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

