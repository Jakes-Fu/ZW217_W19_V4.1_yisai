/*****************************************************************************
 ** File Name:   mmiappsample_funcwinc.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: funcWinC sample code
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
    uint16 input_index;
    uint16 cur_sel;
    MMI_HANDLE_T    src_handle;
    FUNCWINCALLBACK callback;
} MMIAPPSAMPLE_FUNCWINMODEC_DATA_T;


/******************************************************************************
//  Description : win handler
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinModeCWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                     );


//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_FUNCWIN_MODEC_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleFuncWinModeCWinMsg),
    WIN_ID( MMIAPPSAMPLE_FUNCWINMODEC_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_FUNCWINLIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_FUNCWINMODEC_LIST_CTRL_ID),
    END_WIN
};


/******************************************************************************
//  Description : funcwinC entry
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFuncWinModeCWin(MMI_HANDLE_T src_handle, uint16 sel_index, FUNCWINCALLBACK callback)
{
    MMI_HANDLE_T win_handle;
    MMIAPPSAMPLE_FUNCWINMODEC_DATA_T *win_data_p = PNULL;

    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE EnterAppSampleFuncWinModeCWin");

    win_data_p = (MMIAPPSAMPLE_FUNCWINMODEC_DATA_T*)SCI_ALLOCAZ(sizeof(MMIAPPSAMPLE_FUNCWINMODEC_DATA_T));

    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(0x00);
        return;
    }
    win_data_p->input_index = sel_index;
    win_data_p->src_handle = src_handle;
    win_data_p->callback = callback;
    win_handle  = MMK_CreateWin((uint32*)MMIAPPSAMPLE_FUNCWIN_MODEC_WIN_TAB, PNULL);

    if (win_handle == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(0x02);
        SCI_FREE(win_data_p);
        return;
    }
    MMK_SetWinUserData(win_handle, win_data_p);

    return;
}

/******************************************************************************
//  Description : funcwinC open ctrl
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E FuncWinModeC_OpenCtrlFunc(MMI_WIN_ID_T win_id) //OpenCtrl
{
    MMIAPPSAMPLE_FUNCWINMODEC_DATA_T *win_data_p = PNULL;

    win_data_p = (MMIAPPSAMPLE_FUNCWINMODEC_DATA_T*)MMK_GetWinUserData(win_id);
    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(0x01);
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    win_data_p->cur_sel = win_data_p->input_index;

    MMK_SetWinUserData(win_id, (ADD_DATA)(win_data_p));     //Set Windata
    MMK_SetWinDirty(win_id, TRUE);                          //Set Windirty

    return MMI_RESULT_TRUE;
}

/******************************************************************************
//  Description : funcwinC close ctrl
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void FuncWinModeC_CloseCtrlFunc(MMI_WIN_ID_T win_id)    //CloseCtrl
{
    MMIAPPSAMPLE_FUNCWINMODEC_DATA_T *win_data_p = (MMIAPPSAMPLE_FUNCWINMODEC_DATA_T*)MMK_GetWinUserData(win_id);

    if (win_data_p != PNULL)
    {
        SCI_FREE(win_data_p);
        return ;
    }
    return ;
}

/******************************************************************************
//  Description : funcwinC list view ctrl
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void FuncWinModeC_ListViewFunc(MMI_WIN_ID_T win_id)     //View func
{
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FUNCWINMODEC_LIST_CTRL_ID;
    MMIAPPSAMPLE_FUNCWINMODEC_DATA_T *win_data_p = (MMIAPPSAMPLE_FUNCWINMODEC_DATA_T*)MMK_GetWinUserData(win_id);

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

    GUILIST_SetCurItemIndex(list_ctrl, win_data_p->cur_sel);
    //Set Windirty
    MMK_SetWinDirty(win_id, FALSE);
}


/******************************************************************************
//  Description : win handler
//   Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinModeCWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                     )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FUNCWINMODEC_LIST_CTRL_ID;
    MMIAPPSAMPLE_FUNCWINMODEC_DATA_T *win_data_p = (MMIAPPSAMPLE_FUNCWINMODEC_DATA_T*)MMK_GetWinUserData(win_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (!FuncWinModeC_OpenCtrlFunc(win_id))
            {
                break;
            }

            break;

        case MSG_FULL_PAINT:
            if (MMK_GetWinDirty(win_id))
            {
                FuncWinModeC_ListViewFunc(win_id);
            }
            break;

        case MSG_CTL_PENOK:
            win_data_p->cur_sel = GUILIST_GetCurItemIndex(list_ctrl);
            if (win_data_p->callback != PNULL)
            {
                win_data_p->callback(win_data_p->src_handle, win_data_p->cur_sel);
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            FuncWinModeC_CloseCtrlFunc(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

