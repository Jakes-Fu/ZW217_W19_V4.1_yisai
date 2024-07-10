/*****************************************************************************
 ** File Name:   mmiappsample_funcwina.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe FuncWin A Sample code
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

/********************************************************************
//  Description :funcWinA win handler
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinModeAWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                     );

//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_FUNCWIN_MODEA_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleFuncWinModeAWinMsg),
    WIN_ID( MMIAPPSAMPLE_FUNCWINMODEA_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_FUNCWIN_MODEA),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_FUNCWINMODEA_LIST_CTRL_ID),
    END_WIN
};

/********************************************************************
//  Description :funcWinA Win Entry
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void EnterAppSampleFuncWinModeAWin(void)
{
    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE EnterAppSampleFuncWinModeAWin");

    MMK_CreateWin((uint32*)MMIAPPSAMPLE_FUNCWIN_MODEA_WIN_TAB, PNULL);
    return;
}

/********************************************************************
//  Description :funcWinA Win Open
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL MMI_RESULT_E FuncWinModeAWin_OpenCtrlFunc(MMI_WIN_ID_T win_id) //OpenCtrl
{

    MMK_SetWinDirty(win_id, TRUE);                          //Set Windirty

    return MMI_RESULT_TRUE;
}

/********************************************************************
//  Description :funcWinA win close
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL void FuncWinModeAWin_CloseCtrlFunc(MMI_WIN_ID_T win_id)    //CloseCtrl
{
    return ;
}

/********************************************************************
//  Description :funcWinA list view
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL void FuncWinModeAWin_ListViewFunc(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FUNCWINMODEA_LIST_CTRL_ID;

    uint8 i = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
//    MMI_STRING_T        show_textstr = {0};
//    wchar               list_text_wchar[20] = {0};
//    char                list_text_char[20] = {0};


    GUILIST_SetMaxItem(list_ctrl, 1, FALSE);
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    //set list item
    for ( i = 0; i < 1; i++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_APPSAMPLE_FUNCWIN_MODEA;
        item_t.user_data = (uint32)i;
        GUILIST_AppendItem( list_ctrl, &item_t );
    }
    MMK_SetAtvCtrl(win_id, list_ctrl);

    //Set Windirty
    MMK_SetWinDirty(win_id, FALSE);
}

/********************************************************************
//  Description :funcWinA win handler
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFuncWinModeAWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                     )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (!FuncWinModeAWin_OpenCtrlFunc(win_id))
            {
                break;
            }

            break;

        case MSG_FULL_PAINT:
            if (MMK_GetWinDirty(win_id))
            {
                FuncWinModeAWin_ListViewFunc(win_id);
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            FuncWinModeAWin_CloseCtrlFunc(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

