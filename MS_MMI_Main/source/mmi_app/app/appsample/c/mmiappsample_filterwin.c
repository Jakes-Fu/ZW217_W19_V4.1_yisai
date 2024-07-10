/*****************************************************************************
 ** File Name:   mmiappsample_filterwin.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe filter win sample code 
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
#include "mmiappsample_menutable.h"

#include "mmi_pubmenuwin.h"
#include "window_parse.h"
#include "mmk_msg.h"

#include "guilistbox.h"

typedef struct
{
    uint32 input_type;
    uint16 sel_index;
    MMIAPPSAMPLE_FILTER_T filter;
}APPSAMPLE_FILTERWINDATA_T;

/******************************************************************************
//  Description : win handler
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFilterWinMsg(MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                               );

LOCAL void GenerateFilter(APPSAMPLE_FILTERWINDATA_T *windata_p);


//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_FILTER_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleFilterWinMsg),
    WIN_ID( MMIAPPSAMPLE_FILTER_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_MAIN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_FILTERWIN_LIST_CTRL_ID),
    END_WIN
};

/******************************************************************************
//  Description : filter win entry
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
PUBLIC void EnterAppSampleFilterWin(void)
{
    MMK_CreateWin((uint32*)MMIAPPSAMPLE_FILTER_WIN_TAB, PNULL);
    return;
}

/******************************************************************************
//  Description : filter win menu init handler
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL void FilterWin_MenuInitHandler(MMIPUB_MENU_NOTIFY_T *param )
{

}


/******************************************************************************
//  Description : filter win menu ok handler
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL void FilterWin_MenuOKHandler(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 group_id, menu_id;

    if ( PNULL == param )
    {
        MMIAPPSAMPLE_RaiseError(0);
        return;
    }
    group_id = param->group_id;
    menu_id = param->menu_id;
    switch (menu_id)
    {
        case ID_APPSAMPLE_WINDATA:
//            EnterAppSampleWindataWin();
            break;
        case ID_APPSAMPLE_EVENT:
            EnterAppSampleEventWin();
            break;
        default:
            break;
    }
}

/******************************************************************************
//  Description : filter win create filter
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL void GenerateFilter(APPSAMPLE_FILTERWINDATA_T *windata_p)
{
    if (windata_p != PNULL)
    {
        MMIAPPSAMPLE_RaiseError(1);
        return;
    }

}



/******************************************************************************
//  Description : open ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E FilterWin_OpenCtrlFunc(MMI_WIN_ID_T win_id)  //OpenCtrl
{
    APPSAMPLE_FILTERWINDATA_T *win_data_p =PNULL;

    win_data_p = (APPSAMPLE_FILTERWINDATA_T*)SCI_ALLOC_APPZ(sizeof(APPSAMPLE_FILTERWINDATA_T));
    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(1);
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    MMK_SetWinUserData(win_id, (ADD_DATA)(win_data_p));
    return MMI_RESULT_TRUE;
}

/******************************************************************************
//  Description : close ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E FilterWin_CloseCtrlFunc(MMI_WIN_ID_T win_id) //Close Ctrl
{
    APPSAMPLE_FILTERWINDATA_T *win_data_p = (APPSAMPLE_FILTERWINDATA_T*)MMK_GetWinUserData(win_id);
    if (win_data_p != PNULL)
    {
        SCI_FREE(win_data_p);
        return MMI_RESULT_TRUE;
    }
    return MMI_RESULT_FALSE;
}

const MMI_TEXT_ID_T filter_list_textid[] =
{
    TXT_APPSAMPLE_FILTER1,
    TXT_APPSAMPLE_FILTER2,
    TXT_APPSAMPLE_FILTER3
};
/*
RES_ADD_STRING(TXT_APPSAMPLE_FILTER1,"1st of Age < 10,Sex=MALE")
RES_ADD_STRING(TXT_APPSAMPLE_FILTER2,"2nd of Age > 10,Sex=MALE")
RES_ADD_STRING(TXT_APPSAMPLE_FILTER3,"3rd of all")

const APPSAMPLE_FILTERWINDATA_T filter[][]=
{
{
{TRUE,VALUE_SMALLER,10},
{TRUE,VALUE_EQUAL,0},
},
{
{TRUE,VALUE_BIGGER,10},
{TRUE,VALUE_EQUAL,0},
},
{
{FALSE,VALUE_EQUAL,0},    
{FALSE,VALUE_EQUAL,0},
},
};
*/

/******************************************************************************
//  Description : list view ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E FilterWin_OpenViewFunc(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FILTERWIN_LIST_CTRL_ID;
    uint8 i = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    GUILIST_SetMaxItem(list_ctrl, 3, FALSE);
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    //set list item
    for ( i = 0; i < 3; i++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = filter_list_textid[i] ;
        item_t.user_data = (uint32)i;
        GUILIST_AppendItem( list_ctrl, &item_t );

    }
    MMK_SetAtvCtrl(win_id, list_ctrl);
    return MMI_RESULT_TRUE;
}


/******************************************************************************
//  Description : win handler
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleFilterWinMsg(MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_FILTERWIN_LIST_CTRL_ID;
    APPSAMPLE_FILTERWINDATA_T *win_data_p = (APPSAMPLE_FILTERWINDATA_T*)MMK_GetWinUserData(win_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (!FilterWin_OpenCtrlFunc(win_id))
            {
                break;
            }
            FilterWin_OpenViewFunc(win_id);

            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_CTL_PENOK:
        case MSG_CTL_LIST_LONGOK:
            //Open Menu
            {
                uint16 cur_sel = GUILIST_GetCurItemIndex(list_ctrl);
                MMIPUB_MENU_PARAM_T menuparam = {0};

                win_data_p->sel_index = cur_sel;

                GenerateFilter(win_data_p);
                menuparam.group_id = MENU_APPSAMPLE_POPUP_MEMU;
                menuparam.win_id = MMIAPPSAMPLE_COMMENU_WIN_ID;
                menuparam.ctrl_id = MMIAPPSAMPLE_MAINWIN_POPMENU_CTRL_ID;

                menuparam.dst_handle = win_id;
                menuparam.func.OnMenuOk = FilterWin_MenuOKHandler;
                menuparam.func.OnMenuInit = FilterWin_MenuInitHandler;
                MMIPUB_OpenPubMenuWin( &menuparam );
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            FilterWin_CloseCtrlFunc(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

