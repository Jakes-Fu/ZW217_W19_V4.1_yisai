/*****************************************************************************
** File Name:      watch_set_bt_ble_manage.c                                 *
** Author:         chunjuan.liang                                            *
** Date:           03/26/2021                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2021      chunjuan.liang            Creat
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
#include "bbm_export.h"
#include "mmieng_export.h"
#include "watch_common_list.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define BBM_MAX_NUM        (6)

typedef enum
{
    BBM_LIST_INDEX_ON,
    BBM_LIST_INDEX_AUTO,
    BBM_LIST_INDEX_SLAVER,
    BBM_LIST_INDEX_BLE,
    BBM_LIST_INDEX_DIALER,
    BBM_LIST_INDEX_PHONE,
    BBM_LIST_INDEX_MAX
}BBM_LIST_INDEX_E;

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION DEFINITION                        *
**---------------------------------------------------------------------------*/

LOCAL void BBMFullPaint(MMI_WIN_ID_T win_id);

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                    *
**---------------------------------------------------------------------------*/
LOCAL MMI_TEXT_ID_T BBMGetRoleText(BBM_CURRENT_DETAIL_STATE detail_state)
{
    MMI_TEXT_ID_T txt_role = TXT_NULL;

    TRACE_APP_BBM("detail_state= %d",detail_state);
    switch(detail_state)
    {
        case BBM_CURRENT_DETAIL_STATE_BLE:
            txt_role = TXT_SET_BT_ROLE_BLE;
            break;
        case BBM_CURRENT_DETAIL_STATE_BTDIALER:
            txt_role = TXT_SET_BT_ROLE_DIALER;
            break;
        case BBM_CURRENT_DETAIL_STATE_BTPHONE:
            txt_role = TXT_SET_BT_ROLE_PHONE;
            break;
        case BBM_CURRENT_DETAIL_STATE_IS_SWITCHING:
            txt_role = TXT_SET_BT_SWITCHING;
            break;
        default:
            break;
    }
    return txt_role;
}
/*****************************************************************************/
//  Description:BBM open Window
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendBBMListItem(MMI_CTRL_ID_T list_ctrl_id)
{
    BBM_MODE_E         bt_mode = BBM_MODE_AUTO;
    uint16          list_index = BBM_MODE_PHONE;
    uint8                    i = 0;
    MMI_STRING_T     list_text = {0};
    MMI_IMAGE_ID_T     imageId = IMAGE_SWITCH_OFF_SELECTED;
    MMI_CTRL_ID_T      ctrl_id = MMISET_BBM_LIST_CTRL_ID;
    int32           listOffset = 0;
    MMI_STRING_T     role_text = {0};
    BOOLEAN           is_bt_on = FALSE;
    MMI_IMAGE_ID_T    image_id = 0;
    MMI_STRING_T     string[2] = {0};
    uint32             text_id = 0;

    // get top item offset
    GUILIST_GetTopItemOffset( ctrl_id, &listOffset );
    // get current select item index
    list_index = GUILIST_GetCurItemIndex( ctrl_id );
    GUILIST_RemoveAllItems( ctrl_id );

    GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_SPLIT_LINE, TRUE);//需要分割线

    /**********************************************/
        /*1.BT on/off*/
    /**********************************************/
    is_bt_on = Bbm_GetBTOn();
    TRACE_APP_BBM("is_bt_on = %d",is_bt_on);
    image_id = is_bt_on ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    text_id = is_bt_on ? TXT_COMMON_OPEN : TXT_COMMON_CLOSE;
    MMI_GetLabelTextByLang(TXT_BLUETOOTH, &string[0]);
    MMI_GetLabelTextByLang(text_id, &string[1]);

    AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line(ctrl_id,string[0], string[1], image_id);

    bt_mode = Bbm_GetBTMode();
    TRACE_APP_BBM("bt mode = %d",bt_mode);

    /**********************************************/
        /*2.Auto*/
    /**********************************************/
    MMI_GetLabelTextByLang(TXT_AUTO,&list_text);
    //调用BBM接口，如果BT形态为auto，显示两行，下面一行显示BTPhone/dialer/ble/switching,如果不是auto,显示一行并灰化
    if(BBM_MODE_AUTO == bt_mode)
    {
        BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_MAX;
        list_index = BBM_LIST_INDEX_AUTO;
        imageId = IMAGE_AW_RADIO_SELECTED_ICON;
        current_detail_state = BbmGetCurrentDetailState();
        TRACE_APP_BBM("auto mode detail state:%d\n",current_detail_state);
        text_id = BBMGetRoleText(current_detail_state);
        MMI_GetLabelTextByLang(text_id,&role_text);
        if(is_bt_on)//如果蓝牙总开关关闭，显示单行
        {
            AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line(list_ctrl_id,list_text,role_text,imageId);//auto
        }
        else
        {
            imageId = IMAGE_AW_RADIO_UNSELECTED_ICON;
            AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,list_text,imageId);//auto
        }
    }
    else
    {
        imageId = IMAGE_AW_RADIO_UNSELECTED_ICON;
        AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,list_text,imageId);//auto
    }

    /**********************************************/
        /*3.Slaver(BLE/Dialer)*/
    /**********************************************/
    MMI_GetLabelTextByLang(TXT_SET_BT_ROLE_SLAVER,&list_text);
    imageId = IMAGE_AW_RADIO_UNSELECTED_ICON;
    //如果BT形态为slaver,显示两行，下面一行显示ble/dialer,如果没有选中，显示一行，并灰化
    if(BBM_MODE_SLAVER == bt_mode)
    {
        BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_MAX; 
        current_detail_state = BbmGetCurrentDetailState();
        TRACE_APP_BBM("slaver mode detail state:%d\n",current_detail_state);
        list_index = BBM_LIST_INDEX_SLAVER;
        imageId = IMAGE_AW_RADIO_SELECTED_ICON;
        text_id = BBMGetRoleText(current_detail_state);
        MMI_GetLabelTextByLang(text_id,&role_text);
        if(is_bt_on)//如果蓝牙总开关关闭，显示单行
        {
            AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line(list_ctrl_id,list_text,role_text,imageId);//slaver  
        }
        else
        {
            imageId = IMAGE_AW_RADIO_UNSELECTED_ICON;
            AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,list_text,imageId);//slaver
        }
    }
    else
    {
        AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,list_text,imageId);
    }

    /**********************************************/
        /*4.BLE*/
    /**********************************************/
    MMI_GetLabelTextByLang(TXT_SET_BT_ROLE_BLE,&list_text);
    imageId = IMAGE_AW_RADIO_UNSELECTED_ICON;

    if(BBM_MODE_BLE == bt_mode)
    {
        imageId = IMAGE_AW_RADIO_SELECTED_ICON;
        list_index = BBM_LIST_INDEX_BLE;
    }
    AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,list_text,imageId);

    /**********************************************/
        /*5.Dialer*/
    /**********************************************/
    MMI_GetLabelTextByLang(TXT_SET_BT_ROLE_DIALER,&list_text);
    imageId = IMAGE_AW_RADIO_UNSELECTED_ICON;
    if(BBM_MODE_DIALER== bt_mode)
    {
        imageId = IMAGE_AW_RADIO_SELECTED_ICON;
        list_index = BBM_LIST_INDEX_DIALER;
    }
    AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,list_text,imageId);

    /**********************************************/
        /*6.Phone*/
    /**********************************************/
    MMI_GetLabelTextByLang(TXT_SET_BT_ROLE_PHONE,&list_text);
    imageId = IMAGE_AW_RADIO_UNSELECTED_ICON;
    if(BBM_MODE_PHONE == bt_mode)
    {
        imageId = IMAGE_AW_RADIO_SELECTED_ICON;
        list_index = BBM_LIST_INDEX_PHONE;
    }
    AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,list_text,imageId);

    if(FALSE == is_bt_on)
    {
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_AUTO,TRUE);//item auto,set gray
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_BLE,TRUE);//item ble,set gray
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_SLAVER,TRUE);//item slaver,set gray
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_DIALER,TRUE);//item dilaer,set gray
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_PHONE,TRUE);//item phone,set gray
    }
    if((TRUE == MMIENG_IsMonkeyTestMode()) || (FALSE == MMIAPIENG_GetBBMViewVisible()))
    {
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_AUTO,TRUE);//item auto,set gray
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_BLE,TRUE);//item ble,set gray
        GUILIST_SetItemGray(list_ctrl_id,BBM_MODE_SLAVER,TRUE);//item slaver,set gray
    }
    GUILIST_SetCurItemIndex(list_ctrl_id,list_index);

    GUILIST_SetTopItemOffset(list_ctrl_id,listOffset);
}
/*****************************************************************************/
//  Description:BBM open Window
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BBMOpenWin(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T  list_ctrl_id = MMISET_BBM_LIST_CTRL_ID;

    MMK_SetAtvCtrl( win_id, list_ctrl_id );

    GUILIST_SetMaxItem( list_ctrl_id, BBM_MAX_NUM, FALSE );

}

/*****************************************************************************/
//  Description:BBM full paint Window
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BBMFullPaint(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T  list_ctrl_id = MMISET_BBM_LIST_CTRL_ID;

    AppendBBMListItem(list_ctrl_id);

    GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_SPLIT_LINE, TRUE);//需要分割线
}
/*****************************************************************************/
//  Description:BBMStartBT
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BBMStartBT(void)
{
    if (!ual_bt_dev_get_is_on() && (UAL_BT_STATUS_SUCCESS != ual_bt_dev_open()))
    {
        uint32 count = 0;

        /* Wait for BT to be ON, 20s at most */
        while (!ual_bt_dev_get_is_on() && (count++ < 200))
        {
            SCI_Sleep(100);
        }
    }
}
/*****************************************************************************/
//  Description:BBMStopBT
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BBMStopBT(void)
{
    if (ual_bt_dev_get_is_on() && (UAL_BT_STATUS_SUCCESS != ual_bt_dev_close()))
    {
        /* Wait for BT to be OFF */
        while (ual_bt_dev_get_is_on())
        {
            SCI_Sleep(100);
        }
    }
}

/*****************************************************************************/
//  Description:BBM web 
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BBMAppOK(MMI_WIN_ID_T win_id)
{
    uint16 list_idx = GUILIST_GetCurItemIndex(MMISET_BBM_LIST_CTRL_ID);

    uint8  pre_bt_role  = UAL_BT_PHONE;

    TRACE_APP_BBM("index = %d",list_idx);

    switch(list_idx)
    {
        case BBM_LIST_INDEX_ON:
        {
            BOOLEAN is_on = TRUE;
            is_on = Bbm_GetBTOn();
            if(FALSE == is_on)
            {
                BbmOpenBt();
            }
            else
            {
                BbmCloseBt();
            }
        }
            break;
        case BBM_LIST_INDEX_AUTO:
        {
            BBM_MODE_E bbm_mode = BBM_MODE_AUTO;
            //set nv
            Bbm_SetBTMode( bbm_mode);
            BbmSwitchToAutoMode();
        }
            break;
        case BBM_LIST_INDEX_SLAVER:
        {
            BBM_MODE_E bbm_mode = BBM_MODE_SLAVER;
            //set nv
            Bbm_SetBTMode( bbm_mode);
            BbmSwitchToSlaverMode();
        }
            break;
        case BBM_LIST_INDEX_BLE:
        {
            BBM_MODE_E bbm_mode = BBM_MODE_BLE;
            //set nv
            Bbm_SetBTMode( bbm_mode);
            BbmSwitchToBleMode();
        }
            break;
        case BBM_LIST_INDEX_DIALER:
        {
            BBM_MODE_E bbm_mode = BBM_MODE_DIALER;
            //set nv
            Bbm_SetBTMode( bbm_mode);
            BbmSwitchToBtDialerMode();
        }
            break;
        case BBM_LIST_INDEX_PHONE:
        {
            BBM_MODE_E bbm_mode = BBM_MODE_PHONE;
            //set nv
            Bbm_SetBTMode( bbm_mode);
            BbmSwitchToBTPhoneMode();
        }
            break;
        default:
            TRACE_APP_BBM("index error");
            break;
    }
    GUILIST_SetCurItemIndex(MMISET_BBM_LIST_CTRL_ID,list_idx);
    MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
}

/*****************************************************************************/
//  Description:BBM cancel
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BBMAppCancel(MMI_WIN_ID_T win_id)
{
    MMK_CloseWin(win_id);
}

/*****************************************************************************/
//  Description:produrce of BBMMain Window
//  Paramter:
//          [In]:win_id msg_id param
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdultSetBBMWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E result =  MMI_RESULT_TRUE;
    TRACE_APP_BBM("msg_id = %08x", msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_BBM("MSG_OPEN_WINDOW");
            BBMOpenWin(win_id);
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_BBM("MSG_CLOSE_WINDOW");
            break;
        }
        case MSG_FULL_PAINT:
        {
            TRACE_APP_BBM("MSG_FULL_PAINT");
            BBMFullPaint(win_id);
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
            TRACE_APP_BBM("MSG_APP_WEB");
            BBMAppOK(win_id);
        }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            TRACE_APP_BBM("MSG_APP_CANCEL");
            BBMAppCancel(win_id);
            break;
        }
        default:
            result =  MMI_RESULT_FALSE;
            break;
    }

    return result;
}

/*win table*/
WINDOW_TABLE( ADULTSET_BBM_WIN_TAB ) =
{
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SET_BT_ROLE_MANAGEMENT),
    WIN_FUNC( (uint32)HandleAdultSetBBMWindow ),
    WIN_ID( MMISET_BBM_WIN_ID ),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISET_BBM_LIST_CTRL_ID ),
    END_WIN
};

/*---------------------------------------------------------------------------*
**                          PUBLIC FUNCTION DEFINITION                       *
**---------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : enter BBM main window
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:
/****************************************************************************/
PUBLIC void AdultSet_BBM_Enter( void )
{
    MMK_CreateWin( (uint32*)ADULTSET_BBM_WIN_TAB, PNULL);
}

