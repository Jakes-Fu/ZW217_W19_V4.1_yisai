/*****************************************************************************
 ** File Name:   mmiappsample_windatawin.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe windata sample code
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
    uint16 data_idx[SRVSAMPLE_MAX_RECORD_NUM];
    uint16 cursel_index;
    uint16 read_value;
} MMIAPPSAMPLE_WINDATAWIN_DATA_T;


/******************************************************************************
//  Description : win handler
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleWindataWinMsg(MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                );


//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_WINDATA_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleWindataWinMsg),
    WIN_ID( MMIAPPSAMPLE_WINDATA_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_FUNCWINLIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_WINDATAWIN_LIST_CTRL_ID),
    END_WIN
};


/******************************************************************************
//  Description : windata funcwin entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleWindataWin(MMIAPPSAMPLE_FILTER_T filter)
{
    MMK_CreateWin((uint32*)MMIAPPSAMPLE_WINDATA_WIN_TAB, PNULL);
    return;
}

/******************************************************************************
//  Description : build list data
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL BOOLEAN WindataWin_buildList(MMIAPPSAMPLE_WINDATAWIN_DATA_T *win_data_p)
{
    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(1);
        return FALSE;
    }

    if (SAMPLESRV_ReadAll(win_data_p->data_idx, SRVSAMPLE_MAX_RECORD_NUM) == 0)
    {
        MMIAPPSAMPLE_RaiseError(1);
        return FALSE;
    }
    return TRUE;

}

LOCAL BOOLEAN Windata_ReadContentOK(MMI_WIN_ID_T win_id)
{

    return TRUE;
}


/******************************************************************************
//  Description : open ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E WindataWin_OpenCtrlFunc(MMI_WIN_ID_T win_id) //OpenCtrl
{
    //MMIAPPSAMPLE_WINDATAWIN_DATA_T *win_data_p = (MMIAPPSAMPLE_WINDATAWIN_DATA_T*)MMK_GetWinAddDataPtr(win_id);
    MMIAPPSAMPLE_WINDATAWIN_DATA_T *win_data_p = PNULL;

    win_data_p = (MMIAPPSAMPLE_WINDATAWIN_DATA_T*)SCI_ALLOCAZ(sizeof(MMIAPPSAMPLE_WINDATAWIN_DATA_T));

    if (win_data_p == PNULL)
    {
        MMIAPPSAMPLE_RaiseError(1);
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    if (!WindataWin_buildList(win_data_p))
    {
        MMIAPPSAMPLE_RaiseError(1);
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    MMK_SetWinUserData(win_id, (ADD_DATA)(win_data_p));     //Set Windata
    MMIAPPSAMPLE_RegisterDateChangeEvent(win_id, 1);        //Register Event
    MMK_SetWinDirty(win_id, TRUE);                          //Set Windirty

    return MMI_RESULT_TRUE;
}

/******************************************************************************
//  Description : close ctrl
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void WindataWin_CloseCtrlFunc(MMI_WIN_ID_T win_id)    //CloseCtrl
{
    MMIAPPSAMPLE_WINDATAWIN_DATA_T *win_data_p = (MMIAPPSAMPLE_WINDATAWIN_DATA_T*)MMK_GetWinUserData(win_id);

    MMIAPPSAMPLE_UnRegisterDateChangeEvent(1);          //Unregister event
    if (win_data_p != PNULL)
    {
        SCI_FREE(win_data_p);
        return ;
    }
    return ;
}

/******************************************************************************
//  Description : list view
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void WindataWin_ListViewFunc(MMI_WIN_ID_T win_id)     //View func
{
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_WINDATAWIN_LIST_CTRL_ID;
    MMIAPPSAMPLE_WINDATAWIN_DATA_T *win_data_p = (MMIAPPSAMPLE_WINDATAWIN_DATA_T*)MMK_GetWinUserData(win_id);

    uint8 i = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T        show_textstr = {0};
    wchar               list_text_wchar[20] = {0};
    char                list_text_char[20] = {0};
    uint16 age = 0;

    if (win_data_p == PNULL)
    {
        //Raise Error
        MMK_CloseWin(win_id);
        return;
    }

    GUILIST_SetMaxItem(list_ctrl, SRVSAMPLE_MAX_RECORD_NUM, FALSE);
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    //set list item
    for ( i = 0; i < SRVSAMPLE_MAX_RECORD_NUM; i++ )
    {
        if (!SAMPLESRV_ReadAgebyIndex(i, &age))
        {
            age = 0;
        }
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        sprintf(list_text_char, "age = %d", age);
        MMIAPICOM_StrToWstr((const uint8*)list_text_char, show_textstr.wstr_ptr );
        SCI_MEMSET(&show_textstr, 0, sizeof(show_textstr));
        show_textstr.wstr_ptr  = list_text_wchar;
        show_textstr.wstr_len = MMIAPICOM_Wstrlen(show_textstr.wstr_ptr);

        item_data.item_content[0].item_data.text_buffer.wstr_ptr = show_textstr.wstr_ptr ;
        item_data.item_content[0].item_data.text_buffer.wstr_len = show_textstr.wstr_len ;

        item_t.user_data = (uint32)i;
        GUILIST_AppendItem( list_ctrl, &item_t );

    }
    MMK_SetAtvCtrl(win_id, list_ctrl);

    GUILIST_SetCurItemIndex(list_ctrl, win_data_p->cursel_index);
    //Set Windirty
    MMK_SetWinDirty(win_id, FALSE);
}

LOCAL BOOLEAN readCallback(SRV_CALLBACK_STRUCT_T* callback_data)
{
    MMIAPPSAMPLE_WINDATAWIN_DATA_T *win_data_p = PNULL;
    if (callback_data == PNULL ||callback_data->user_data == PNULL)
    {
        return;
    }

    win_data_p=(MMIAPPSAMPLE_WINDATAWIN_DATA_T*)MMK_GetWinUserData(callback_data->user_data); 
    
    //do something
    MMK_SetWinDirty(callback_data->user_data,TRUE);
    //win_data_p->data_idx[win_data_p->cursel_index] = *((uint16*)callback_data->opreation_cb);
	 win_data_p->cursel_index  = 0;
    MMK_PostMsg(callback_data->user_data,MSG_FULL_PAINT,PNULL,0);

	return TRUE;
}

/******************************************************************************
//  Description : win handler
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleWindataWinMsg(MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl = MMIAPPSAMPLE_WINDATAWIN_LIST_CTRL_ID;
    MMIAPPSAMPLE_WINDATAWIN_DATA_T *win_data_p = (MMIAPPSAMPLE_WINDATAWIN_DATA_T*)MMK_GetWinUserData(win_id);


    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (!WindataWin_OpenCtrlFunc(win_id))
            {
                break;
            }

            break;

        case MSG_FULL_PAINT:
            if (MMK_GetWinDirty(win_id))
            {
                WindataWin_buildList(win_data_p);
                WindataWin_ListViewFunc(win_id);
            }
            break;

        case MSG_CTL_PENOK:
#if 0
			{
            uint16 cur_sel = GUILIST_GetCurItemIndex(list_ctrl);
            uint16 age = 0;
            if (SAMPLESRV_ReadAgebyIndex(cur_sel, &age))
            {
                win_data_p->cursel_index = cur_sel; //save cur sel to windata
                SAMPLESRV_EditOneAgebyIndex(win_data_p->cursel_index, age + 3);
            }
        }
#endif
        break;
		case MSG_APP_OK:
		case MSG_CTL_MIDSK:
        case MSG_CTL_PENLONGOK:
        {
            win_data_p->cursel_index = GUILIST_GetCurItemIndex(list_ctrl);
            SAMPLESRV_AysnReadOnebyIndex(win_data_p->cursel_index,readCallback,&win_data_p->read_value,win_id);
        }
        break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            WindataWin_CloseCtrlFunc(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

