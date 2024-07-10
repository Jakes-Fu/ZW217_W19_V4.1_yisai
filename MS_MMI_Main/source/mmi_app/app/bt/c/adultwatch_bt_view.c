/*****************************************************************************
         ** File Name:      adultwatch_bt_view.c                             *
** Author:         xiaotong.su                                               *
** Date:           04/16/2021                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2021      xiaotong.su            Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmicom_trace.h"
#include "mmi_text.h"
#include "guires.h"
#include "guictrl_api.h"
#include "window_parse.h"
#include "mmiset_id.h"
#include "guilistbox.h"
#include "mmiset_text.h"
#include "ual_bt.h"
#include "adultwatch_bt_view.h"
#include "adultwatch_bt_name_win.h"
#include "watch_set_bt_ble_manage.h" //bt control
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
#include "adultwatch_bt_pair.h" //bt pair win
#include "mmi_string.h"
#include "watch_common_list.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define BT_MAX_NUM        (3)

typedef enum
{
    BT_CONTROL,
    BT_PAIR,
    BT_DEVICE_NAME
}ADULTWATCH_BT_ITEM_E;

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION DEFINITION                        *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                    *
**---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:BT append List Item
//  Paramter:
//          [In]:list_ctrl_id
//          [Out]:
//          [Return]:
//  Author: xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void AppendBtListItem(MMI_CTRL_ID_T list_ctrl_id)
{
    uint16          cur_list_index = 0;
    MMI_STRING_T    list_text = {0};
    MMI_CTRL_ID_T   ctrl_id = MMISET_BT_LIST_CTRL_ID;
    wchar           wstr_bt_name[BT_LOCAL_NAME_LEN + 1]={0};
    MMI_STRING_T    bt_local_name = {0};

    // get current select item index
    cur_list_index = GUILIST_GetCurItemIndex( ctrl_id );
    GUILIST_RemoveAllItems( ctrl_id );

    //Bluetooth Control
    MMI_GetLabelTextByLang(TXT_SET_BT_CONTROL,&list_text);
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( list_ctrl_id, list_text );
    //Bluetooth Pair
    MMI_GetLabelTextByLang(TXT_SET_BT_PAIR,&list_text);
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( list_ctrl_id, list_text );
    //Device Name
#ifdef BBM_ROLE_SUPPORT
    Bbm_GetBTLocalName( wstr_bt_name,BT_LOCAL_NAME_LEN);
#else
    ual_bt_dev_get_local_name(wstr_bt_name,BT_LOCAL_NAME_LEN+1);
#endif
    bt_local_name.wstr_ptr = wstr_bt_name;
    bt_local_name.wstr_len = MMIAPICOM_Wstrlen(bt_local_name.wstr_ptr);

    MMI_GetLabelTextByLang(TXT_SET_BT_DEVICE_NAME,&list_text);
    AdultWatchCOM_ListItem_Draw_2Str_2line(list_ctrl_id,list_text,bt_local_name);
    GUILIST_SetCurItemIndex(list_ctrl_id,cur_list_index);

}

/*****************************************************************************/
//  Description:BT open Window
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void BtOpenWin(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T  list_ctrl_id = MMISET_BT_LIST_CTRL_ID;

    MMK_SetAtvCtrl( win_id, list_ctrl_id );
    GUILIST_SetMaxItem( list_ctrl_id, BT_MAX_NUM, FALSE );

}

 /*****************************************************************************/
//  Description:BT full paint Window
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void BtFullPaint(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T  list_ctrl_id = MMISET_BT_LIST_CTRL_ID;

    AppendBtListItem(list_ctrl_id);
}

/*****************************************************************************/
//  Description:BT web
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void BtAppOK(void)
{
    uint16 list_idx = GUILIST_GetCurItemIndex(MMISET_BT_LIST_CTRL_ID);
    TRACE_APP_BT("index = %d",list_idx);

    switch (list_idx)
    {
        case BT_CONTROL:
        {
#ifdef BBM_ROLE_SUPPORT
            AdultSet_BBM_Enter();
#endif
            break;
        }
        case BT_PAIR:
        {

#ifdef BBM_ROLE_SUPPORT
            AdultBT_Pair_Enter();
#endif
            break;
        }
        case BT_DEVICE_NAME:
        {
            Bt_NameWin_Enter();
            break;
        }

        default:
        {
            break;
        }
    }
}

/*****************************************************************************/
//  Description:BT cancel
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void BtAppCancel(MMI_WIN_ID_T win_id)
{
    MMK_CloseWin(win_id);
}

/*****************************************************************************/
//  Description:produrce of BTMain Window
//  Paramter:
//          [In]:win_id msg_id param
//          [Out]:
//          [Return]:
//  Author: xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdultSetBtWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E result =  MMI_RESULT_TRUE;
    TRACE_APP_BT("msg_id = %08x", msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_BT("MSG_OPEN_WINDOW");
            BtOpenWin(win_id);
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_BT("MSG_CLOSE_WINDOW");
            break;
        }
        case MSG_FULL_PAINT:
        {
            TRACE_APP_BT("MSG_FULL_PAINT");
            BtFullPaint(win_id);
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_APP_OK:
        case MSG_APP_WEB:
        {
            TRACE_APP_BT("MSG_APP_WEB");
            BtAppOK();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            TRACE_APP_BT("MSG_APP_CANCEL");
            BtAppCancel(win_id);
            break;
        }
        default:
        {
            result =  MMI_RESULT_FALSE;
            break;
        }
    }

    return result;
}

/*win table*/
WINDOW_TABLE( ADULTWATCH_BT_WIN_TAB ) =
{
	WIN_HIDE_STATUS,
	WIN_TITLE( TXT_BLUETOOTH ),
	WIN_FUNC( (uint32)HandleAdultSetBtWindow ),
	WIN_ID( MMISET_BT_WIN_ID ),
	CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISET_BT_LIST_CTRL_ID ),
	END_WIN
};

/*---------------------------------------------------------------------------*
**                          PUBLIC FUNCTION DEFINITION                       *
**---------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : enter BT main window
//  Global resource dependence :
//  Author xiaotong.su
//  Note:
/****************************************************************************/
PUBLIC void AdultWatchBT_MainWin_Enter( void )
{
    MMK_CreateWin( (uint32*)ADULTWATCH_BT_WIN_TAB, PNULL);
}

