/*****************************************************************************
 ** File Name:   mmiappsample_mainWin.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe sample app mainwin 
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

#include "mmi_pubmenuwin.h"
#include "window_parse.h"
#include "mmk_msg.h"

#include "guilistbox.h"



typedef struct
{
    MMI_TEXT_ID_T   list_textid;
    void   (*func)(void);
} APPSAMPLE_MAIN_ENTRY_LIST_T;

/******************************************************************************
//  Description : win handler
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleMainWinMsg(MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                             );


//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_MAIN_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleMainWinMsg),
    WIN_ID( MMIAPPSAMPLE_MAIN_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_MAIN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_MAINWIN_LIST_CTRL_ID),
    END_WIN
};

/******************************************************************************
//  Description : Entry sample app
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleMainWin(CAF_HANDLE_T app_handle)
{
    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE EnterAppSampleMainWin");
    if (app_handle == PNULL)
        {
            return;
        }
    MMK_CreateWinByApplet(app_handle,(uint32*)MMIAPPSAMPLE_MAIN_WIN_TAB, PNULL);
    return;
}

/******************************************************************************
//  Description : open ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E MainWin_OpenCtrlFunc(MMI_WIN_ID_T win_id)    //OpenCtrl
{
    return MMI_RESULT_TRUE;
}

/******************************************************************************
//  Description : close ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E MainWin_CloseCtrlFunc(MMI_WIN_ID_T win_id)   //CloseCtrl
{
    return MMI_RESULT_TRUE;
}

const APPSAMPLE_MAIN_ENTRY_LIST_T list_entry[] =
{
    {TXT_APPSAMPLE_FUNCWINLIST, PNULL},
    {TXT_APPSAMPLE_PUBMENU, PNULL},
    {TXT_APPSAMPLE_PUBMENU, PNULL}
};

/******************************************************************************
//  Description : view ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E MainWin_OpenViewFunc(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_MAINWIN_LIST_CTRL_ID;
    uint8 i = 0;
    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    GUILIST_SetMaxItem(list_ctrl, 3, FALSE);
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    //set list item
    for ( i = 0; i < 3; i++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = list_entry[i].list_textid ;
        item_t.user_data = (uint32)i;
        GUILIST_AppendItem( list_ctrl, &item_t );

    }
    MMK_SetAtvCtrl(win_id, list_ctrl);
    return MMI_RESULT_TRUE;
}

/******************************************************************************
//  Description : win handler
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleMainWinMsg(MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_MAINWIN_LIST_CTRL_ID;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MainWin_OpenCtrlFunc(win_id);
            MainWin_OpenViewFunc(win_id);

            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_CTL_PENOK:
        {
            uint16 cur_sel = GUILIST_GetCurItemIndex(list_ctrl);
            MMIAPPSAMPLE_FILTER_T filter = {0};
            if (cur_sel == 0)
            {
                EnterAppSampleFuncWinListWin();
            }
            else
	            {
                EnterAppSampleWindataWin(filter);
            }
        }
        break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            MainWin_CloseCtrlFunc(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}


