/*****************************************************************************
** File Name:      watch_bt_main_win.c                                        *
** Author:         bin.wang1                                                 *
** Date:           04/09/2020                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2020      bin.wang1              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmibt_mainwin.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_image.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "mmiset_export.h"
#include "guitext.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmivcard_export.h"
#include "mmieng_export.h"
#include "mmicl_export.h"
#include "mmiphone_export.h"
#include "mmibt_a2dp.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmibt_nv.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#include "mmi_mainmenu_export.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "mmidc_export.h"
#include "guiedit.h"
#include "mmiidle_export.h"
#include "mmimtv_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmi_autotest.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmieng_id.h"
#include "guictrl_api.h"
#include "mmieng_text.h"
#include "mmienvset_export.h"
#include "mime_type.h"
#include "mmisms_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif
#include "mmikl_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmiidle_statusbar.h"
#include "mmimultim_image.h"
#include "mmisrvaud_api.h"

#ifdef BT_PBAP_SUPPORT
#include "mmipb_export.h"
#include "mmipb_app.h"
#include "mmipb_common.h"
#include "mmipb_text.h"
#include "mmifilearray_export.h"
//#include "mmipb_task.h"
#endif
#include "mmicom_trace.h"
#include "watch_commonwin_export.h"
#include "mmiset_image.h"
#include "mmibt_headset.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define BT_MAIN_LIST_PERMANENT_ITEM_NUM         (1)
#define BT_MAIN_LIST_BT_ITEM_INDEX              (0)

#define BT_SSP_NUM_LEN                          (7)
#define BT_TIP_MAX_LEN                          (50)

#define BT_DEFAULT_PIN_CODE                     "1234"

typedef enum
{
    MMIBT_STATUS_NONE,//该状态表示已打开或已关闭(opened or closed)
    MMIBT_STATUS_OPENING,
    MMIBT_STATUS_CLOSING,
    MMIBT_STATUS_MAX,
}MMIBT_STATUS_ENUM;


/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/

//define the list of devices searched. it,s a good idea to move it to bt app layer.
LOCAL BT_DEVICE_INFO  *g_watch_bt_inq_dev_info = PNULL;

//define the count of devices searched. it,s a good idea to move it to bt app layer.
LOCAL uint8 g_bt_inq_dev_num  = 0;

//define the device to be master pair. for input pin mode
LOCAL BT_DEVICE_INFO  g_pin_request_info = {0};

//define the device to be input pin code for pair.
LOCAL BT_DEVICE_INFO  g_dev_pin_input = {0};

//define the device to be master pair. for ssp mode
LOCAL BT_DEVICE_INFO_EXT  g_pin_request_info_ext = {0};

//is in bt device searching state
LOCAL BOOLEAN searching_flag = FALSE;

//define the device to remove
LOCAL BT_DEVICE_INFO g_remove_paired_dev_info = {0};

LOCAL MMIBT_STATUS_ENUM s_bt_status = MMIBT_STATUS_NONE;

//list item callback function
typedef void ( *BT_ITEM_FUNC )( void );


/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description  : whether the dev in paired list
//  Paramter:
//          [In]:dev1
//          [Out]:None
//          [Return]:BOOLEAN
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDeviceInPairedList(const BT_DEVICE_INFO *dev1);

/*****************************************************************************/
//  Description  : is dev1's addr equal to dev2's addr
//  Paramter:
//          [In]:dev1 dev2
//          [Out]:None
//          [Return]:BOOLEAN
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSameBtAddr(const BT_DEVICE_INFO *dev1, const BT_DEVICE_INFO *dev2);

/*****************************************************************************/
//  Description  : begin to start search device using bluetooth
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:BT_STATUS
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS StartSearchDevice(void);

/*****************************************************************************/
//  Description  : cancel search device
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:BT_STATUS
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS CancelSearchDevice(void);

/*****************************************************************************/
//  Description  : process the action of press on the item of [on/off]
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void HandleOnOffAction();

/*****************************************************************************/
//  Description  : process the action of press on the item of paired 
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void HandlePairedDeviceAction();

/*****************************************************************************/
//  Description  : process the action of press on the item of seached 
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void HandleInquiredDeviceAction();

/*****************************************************************************/
//  Description  : load on/off state from bt  ps, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void AppendOnOffItemToList(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description  : load the data of paired devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void AppendPairedItemsToList(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description  : load the data of searched devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void AppendInquiredItemsToList(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description  : remove all items in list_ctrl, reload all data and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void ReCreateList(MMI_CTRL_ID_T list_ctrl_id);


/*****************************************************************************/
//  Description  : update soft key
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void UpdateSoftKey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description  : redraw all sub ctrls in window
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void UpdateAllWin();

/*****************************************************************************/
//  Description  : get the image id of device by dev info
//  Paramter:
//          [In]:dev ------ device info
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDeviceIconByInfo(BT_DEVICE_INFO *dev);

/*****************************************************************************/
//  Description  : the win produrce of input pin
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputPinPair( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description  : open pin mode query win.
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void OpenPinPairQueryWin(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description  : the win produrce of ssp mode
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSspPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description  : open ssp mode query win.
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void OpenSspPairQueryWin(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description  : process the message of MSG_OPEN_WINDOW
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_OPEN_WINDOW( MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description  : process the message of MSG_CLOSE_WINDOW
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_CLOSE_WINDOW(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description  : process the message of MSG_FULL_PAINT
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_FULL_PAINT( MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description  : process the message of MSG_APP_OK
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_APP_OK( MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description  : process the message of MSG_APP_CANCEL
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_APP_CANCEL( MMI_WIN_ID_T win_id );

/*****************************************************************************/
//  Description  : produrce of Bt Main Window
//  Paramter:
//          [In]:win_id msg_id param
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBTMainWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         );


#ifdef BREAK_LINE_MARK
#endif
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION DEFINITION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description  : pop up prompt msg
//  Paramter:
//          [In]:txt id
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopUpPromptMsg(MMI_TEXT_ID_T txt_id)
{

    MMI_STRING_T txt_msg = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey={TXT_NULL,TXT_NULL,TXT_NULL};
    MMI_GetLabelTextByLang( txt_id, &txt_msg );
    WatchCOM_NoteWin_2Line_Enter(MMIBT_NOTE_WIN_ID,&txt_msg,0,softkey,PNULL);

    TRACE_APP_COMMON("paired list have not headset.");
    return FALSE;
}

/*****************************************************************************/
//  Description  : whether the dev in paired list
//  Paramter:
//          [In]:dev1
//          [Out]:None
//          [Return]:BOOLEAN
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDeviceInPairedList(const BT_DEVICE_INFO *dev1)
{
    uint32 i = 0;
    uint32 service_type = BT_SERVICE_ALL;
    uint32 paired_num = BT_GetPairedDeviceCount(service_type);
    //TRACE_APP_COMMON("enter. paired_num = %d.", paired_num);
    if (dev1 == NULL)
    {
        return FALSE;
    }

    for (i = 0; i < paired_num; i++)
    {
        BT_DEVICE_INFO paired_dev = {0};
        BT_GetPairedDeviceInfo(service_type, i + 1, &paired_dev);
        if (IsSameBtAddr(dev1, &paired_dev))
        {
            TRACE_APP_COMMON("dev is in paired list. the pos is %d.", i+1);
            return TRUE;
        }
    }

    //TRACE_APP_COMMON("dev not in paired list.");
    return FALSE;
}


/*****************************************************************************/
//  Description  : is dev1's addr equal to dev2's addr
//  Paramter:
//          [In]:dev1 dev2
//          [Out]:None
//          [Return]:BOOLEAN
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSameBtAddr(const BT_DEVICE_INFO *dev1, const BT_DEVICE_INFO *dev2)
{
    if (dev1 == NULL || dev2 == NULL)
    {
        return FALSE;
    }

    if (0 == SCI_MEMCMP(&dev1->addr, &dev2->addr, sizeof(BT_ADDRESS)))
    {
        return TRUE;
    }

    return FALSE;
}


/*****************************************************************************/
//  Description  : begin to start search device using bluetooth
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:BT_STATUS
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS StartSearchDevice(void)
{
    BT_STATUS result = BT_SUCCESS;
    TRACE_APP_COMMON("enter.");
    result = BT_SearchDevice(BT_SERVICE_ALL);

    if (result == BT_SUCCESS || result == BT_PENDING)
    {
        TRACE_APP_COMMON("invoke BT_SearchDevice() success.");
        searching_flag = TRUE;
    }
    else
    {
        searching_flag = FALSE;
        TRACE_APP_COMMON("invoke BT_SearchDevice() failed.");
    }

    TRACE_APP_COMMON("exit.");
    return result;
}


/*****************************************************************************/
//  Description  : cancel search device
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:BT_STATUS
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS CancelSearchDevice(void)
{
    BT_STATUS result = BT_SUCCESS;
    TRACE_APP_COMMON("enter.");
    result = BT_CancelSearch();

    if (result == BT_SUCCESS || result == BT_PENDING)
    {
        TRACE_APP_COMMON("success.");
    }
    else
    {
        TRACE_APP_COMMON("failed.");
    }
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : process the action of press on the item of [on/off]
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void HandleOnOffAction()
{
    BOOLEAN is_bt_on = BT_GetState();
    BT_STATUS result = BT_SUCCESS;

    TRACE_APP_COMMON("enter. is_bt_on = %d,s_bt_status = %d.", is_bt_on,s_bt_status);

    if(MMIBT_STATUS_NONE != s_bt_status)
    {
        return;
    }
    if (!is_bt_on)
    {
        result = BT_Start();
        TRACE_APP_COMMON("invoke BT_Start(). result = %d.", result);
        if(BT_PENDING == result)//bug13914545
        {
            s_bt_status = MMIBT_STATUS_OPENING;
        }
    }
    else
    {
        result = BT_Stop();
        TRACE_APP_COMMON("invoke BT_Stop(). result = %d.", result);
        if(BT_PENDING == result)
        {
            s_bt_status = MMIBT_STATUS_CLOSING;
        }
    }
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : process the action of press on the item of paired 
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void HandlePairedDeviceAction()
{
    uint16 idx = GUILIST_GetCurItemIndex(MMIBT_MAIN_LIST_CTRL_ID);
    uint32 service_type = BT_SERVICE_ALL;
    BT_DEVICE_INFO paired_dev = {0};
    BT_STATUS      del_ret = BT_SUCCESS;
    uint16         dev_type = 0;
    BT_STATUS      bt_ret = BT_SUCCESS;
    uint8   paired_device_index[MMIBT_MAX_DEV_ITEMS]= {0};

    TRACE_APP_COMMON("enter index=%d.",idx);


    MMIBT_GetPairedDevList(service_type, paired_device_index);

    BT_GetPairedDeviceInfo(service_type, paired_device_index[idx - 1], &paired_dev);

    TRACE_APP_COMMON("dev_type =%d, paired_dev addr %d,%d,%d,%d,%d,%d",dev_type,paired_dev.addr.addr[0],paired_dev.addr.addr[1],paired_dev.addr.addr[2],paired_dev.addr.addr[3],paired_dev.addr.addr[4],paired_dev.addr.addr[5]);
    bt_ret = BT_GetDeviceType(&paired_dev, &dev_type);
    TRACE_APP_COMMON("HandlePairedDeviceAction device type = %d",dev_type);
    if(dev_type & BT_DEVICE_CLASS_AUDIO)
    {
        if(!MMIBT_DeviceIsConnectedHeadset(&(paired_dev.addr)))
        {
            TRACE_APP_COMMON("HandlePairedDeviceAction open headset ");
            del_ret = BT_RemovePairedDevice(&paired_dev.addr); //bug1355879

        }else
        {
            TRACE_APP_COMMON("HandlePairedDeviceAction close headset ");
            CloseHeadset(&paired_dev.addr);//close headset

            SCI_MEMSET(&g_remove_paired_dev_info,0,sizeof(BT_DEVICE_INFO));
            g_remove_paired_dev_info = paired_dev;

            //recieve disconnect msg,then To do delete pair

        }
    }else
    {
        TRACE_APP_COMMON("HandlePairedDeviceAction is not audio dev  ");
        //To do delete pair
        del_ret = BT_RemovePairedDevice(&paired_dev.addr);
    }

    TRACE_APP_COMMON("exit.del_ret=%d",del_ret);
}


/*****************************************************************************/
//  Description  : process the action of press on the item of seached 
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void HandleInquiredDeviceAction()
{
    BT_STATUS return_value = BT_SUCCESS;
    uint32 service_type = BT_SERVICE_ALL;
    uint16 index  = GUILIST_GetCurItemIndex(MMIBT_MAIN_LIST_CTRL_ID);
    uint32 paired_cnt =BT_GetPairedDeviceCount(service_type);
    BT_ADDRESS addr = {0};

    TRACE_APP_COMMON("enter. paired_cnt = %d.", paired_cnt);
    SCI_MEMSET(&g_pin_request_info, 0, sizeof(BT_DEVICE_INFO));
    if(PNULL == g_watch_bt_inq_dev_info)
    {
        TRACE_APP_COMMON("PNULL == g_watch_bt_inq_dev_info.");
        return;
    }
    if((MMIBT_STATUS_OPENING == s_bt_status)||(MMIBT_STATUS_CLOSING == s_bt_status))//bug1522220
    {
        TRACE_APP_COMMON("s_bt_status error,s_bt_status=%d",s_bt_status);
        return;
    }
    //if connected device in paired list,pop up prompt msg
    if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr)) //bug1352187
    {
        PopUpPromptMsg(TXT_ACTIVE_HEADSET_ALERT);
        return;
    }

    //copy selected device info to g_pin_request_info.
    SCI_MEMCPY(&g_pin_request_info, &g_watch_bt_inq_dev_info[index-1-paired_cnt], sizeof(BT_DEVICE_INFO));

    //invoke BT_PairDevice
    return_value = BT_PairDevice(&g_pin_request_info.addr);

    TRACE_APP_COMMON("invoke BT_PairDevice(), return = %d.", return_value);
    if (BT_PENDING != return_value)
    {
        MMI_STRING_T  txt_error = {0};
        WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
        MMI_GetLabelTextByLang( TXT_ERROR, &txt_error );
        TRACE_APP_COMMON("invoke BT_PairDevice() failed.");

        //tip error win
        WatchCOM_NoteWin_1Line_Enter(MMIBT_NOTE_WIN_ID,&txt_error,0,softkey,PNULL);
    }
    else
    {
        MMI_STRING_T txt_wait = {0};
        WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
        TRACE_APP_COMMON("invoke BT_PairDevice() success.");
        MMIBT_SetSelfPair(TRUE);
        SetIsPinCancel(FALSE);

        //tip waiting win
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &txt_wait);
        WatchCOM_WaitingWin_1Line_Enter(MMIBT_WAIT_WIN_ID,&txt_wait,0,softkey, NULL/*HandleCanbeCancelWaitingWinMsg*/);
    }
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : load on/off state from bt  ps, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void AppendOnOffItemToList(MMI_CTRL_ID_T list_ctrl_id)
{
    TRACE_APP_COMMON("enter.");
    while (1)
    {
        BOOLEAN is_bt_on = FALSE;
        MMI_IMAGE_ID_T image_id =0;
        uint32 text_id = 0; 
        MMI_STRING_T string[2] = {0};
        uint16 list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);
        uint32 user_data = 0;

        is_bt_on = BT_GetState();
        TRACE_APP_COMMON("is_bt_on = %d", is_bt_on);

        image_id = is_bt_on ? IMAGE_CHECK_SELECTED_ICON: IMAGE_CHECK_UNSELECTED_ICON;
        text_id = is_bt_on ? TXT_COMMON_OPEN : TXT_COMMON_CLOSE;
        MMI_GetLabelTextByLang(TXT_BLUETOOTH, &string[0]);
        MMI_GetLabelTextByLang(text_id, &string[1]);

        WatchCOM_ListItem_Draw_2Str_1Icon( list_ctrl_id, string[0], string[1], image_id);
        user_data = (uint32 *)HandleOnOffAction;
        GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);

        //must break at this point
        break;
    }

    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : load the data of paired devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void AppendPairedItemsToList(MMI_CTRL_ID_T list_ctrl_id)
{
    TRACE_APP_COMMON("enter.");
    while (1)
    {
        uint32 service_type = BT_SERVICE_ALL;
        uint32 i = 0;
        uint32 paired_items_cnt = BT_GetPairedDeviceCount(service_type);
        MMI_STRING_T bt_status_str = {0};
        uint32 user_data = 0;
        uint8   paired_device_index[MMIBT_MAX_DEV_ITEMS]= {0};
        MMI_GetLabelTextByLang( STXT_PAIRED, &bt_status_str);
        TRACE_APP_COMMON("paired_items_cnt = %d.", paired_items_cnt);


        MMIBT_GetPairedDevList(BT_SERVICE_ALL, paired_device_index);

        for (i = 0; i < paired_items_cnt; i++)
        {
            BT_DEVICE_INFO dev_info = {0};
            MMI_IMAGE_ID_T dev_icon = 0;
            MMI_STRING_T dev_name = {0};
            uint16 list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);
            
            //the index of device begin at 1.
            if (BT_SUCCESS != BT_GetPairedDeviceInfo(service_type, paired_device_index[i], &dev_info))
            {
                TRACE_APP_COMMON("invoke BT_GetPairedDeviceInfo error. id = %d.", i);
                continue;
            }

            dev_icon = GetDeviceListIcon(&dev_info);

            dev_name.wstr_ptr = dev_info.name;
            dev_name.wstr_len = MMIAPICOM_Wstrlen(dev_name.wstr_ptr);
            WatchCOM_ListItem_Draw_1Icon_2Str(list_ctrl_id, dev_icon, dev_name, bt_status_str);

            //to do.paired callback
            user_data = (uint32 *)HandlePairedDeviceAction;
            GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);
        }
        
        //must break at this point
        break;
    }
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : load the data of searched devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void AppendInquiredItemsToList(MMI_CTRL_ID_T list_ctrl_id)
{
    TRACE_APP_COMMON("enter.");
    while (1)
    {
        uint32 i = 0;
        MMI_STRING_T bt_name = {0};
        MMI_STRING_T bt_status_str  = {0};
        MMI_IMAGE_ID_T dev_icon = 0;
        uint16 list_index = 0;

        MMI_GetLabelTextByLang(TXT_SET_BT_AVAILABLE, &bt_status_str);

        if (g_watch_bt_inq_dev_info == NULL || g_bt_inq_dev_num == 0)
        {
            TRACE_APP_COMMON("inquired list not valid.");
            break;
        }

        TRACE_APP_COMMON("g_bt_inq_dev_num = %d. g_watch_bt_inq_dev_info's addr = %0x.",
            g_bt_inq_dev_num,
            g_watch_bt_inq_dev_info);

        for (i = 0; i < g_bt_inq_dev_num; i++)
        {
            BT_DEVICE_INFO device_info = {0};
            uint32 user_data = 0;

            //current info
            device_info = g_watch_bt_inq_dev_info[i];

            //get icon
            dev_icon = GetDeviceListIcon(&device_info);

            //parse name
            bt_name.wstr_ptr = device_info.name;
            bt_name.wstr_len = MMIAPICOM_Wstrlen(bt_name.wstr_ptr);

            //must set before WatchCOM_ListItem_Draw_1Icon_2Str
            list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);

            //draw list item
            WatchCOM_ListItem_Draw_1Icon_2Str(list_ctrl_id, dev_icon, bt_name, bt_status_str);

            //set callback
            user_data = HandleInquiredDeviceAction;
            GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);
        }

        //must break at this point for while(1)
        break;
    }
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : remove all items in list_ctrl, reload all data and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void ReCreateList(MMI_CTRL_ID_T list_ctrl_id)
{
    uint32 focus_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    TRACE_APP_COMMON("enter.");

    //remove all list items
    GUILIST_RemoveAllItems(list_ctrl_id);

    //append on/off item
    AppendOnOffItemToList(list_ctrl_id);

    //append paired list && searched list when device is on
    if (BT_GetState())
    {
        AppendPairedItemsToList(list_ctrl_id);
        AppendInquiredItemsToList(list_ctrl_id);
    }

    //set focus index
    GUILIST_SetCurItemIndex(list_ctrl_id, focus_index);
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : update soft key
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void UpdateSoftKey(MMI_WIN_ID_T win_id)
{
    if(searching_flag)
    {
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL, STXT_STOP, TRUE);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,STXT_OK, STXT_RETURN, TRUE);
    }
}

/*****************************************************************************/
//  Description  : redraw all sub ctrls in window
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void UpdateAllWin()
{
    if(TRUE == MMK_IsFocusWin(MMIBT_MAIN_WIN_ID)) //Bug 1734902
    {
        MMK_SendMsg(MMIBT_MAIN_WIN_ID,MSG_FULL_PAINT,PNULL);
    }
}

#if 0
PUBLIC void BTMainCancelWaitingWin(void)
{
    MMI_STRING_T txt_cancel = {0};
    WATCH_SOFTKEY_TEXT_ID_T   softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
    MMI_GetLabelTextByLang(TXT_BT_CANCELLING, &txt_cancel);
    WatchCOM_WaitingWin_1Line_Enter(MMIBT_WAIT_WIN_ID,&txt_cancel,0,softkey,HandlePubwinWaitingMsg);  
}
#endif


/*****************************************************************************/
//  Description  : get the image id of device by dev info
//  Paramter:
//          [In]:dev ------ device info
//          [Out]:None
//          [Return]:None
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDeviceIconByInfo(BT_DEVICE_INFO *dev)
{
    MMI_IMAGE_ID_T icon = IMAGE_BT_PC_NO_PAIRE;
    uint16 dev_type = 0;
    uint16 icon_index = 0;
    MMI_IMAGE_ID_T  watch_pair_icon_array[] = {IMAGE_BT_HEADSET_DISCONNECT, IMAGE_BT_HANFREE_PAIRE, IMAGE_BT_PC_PAIRE};
    MMI_IMAGE_ID_T  watch_no_pair_icon_array[] = {IMAGE_BT_WATCH_HEADSET_NO_PAIRE,IMAGE_BT_WATCH_PHONE_NO_PAIRE,IMAGE_BT_WATCH_PC_NO_PAIRE};
    if (PNULL == dev)
    {
        TRACE_APP_COMMON("PNULL == dev");
        return icon;
    }
    BT_GetDeviceType(dev, &dev_type);
    if(dev_type & BT_DEVICE_CLASS_AUDIO)
    {
        icon_index = 0;
    }
    else if(dev_type & BT_DEVICE_CLASS_PHONE)
    {
        icon_index = 1;
    }
    else
    {
        icon_index = 2;
    }
    if(DeviceIsPaired(&(dev->addr)))
    {
        icon = watch_pair_icon_array[icon_index];
    }else
    {
        icon = watch_no_pair_icon_array[icon_index];
    }
}


/*****************************************************************************/
//  Description  : the win produrce of input pin
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputPinPair( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
    BT_STATUS  return_value = BT_SUCCESS;
    MMI_STRING_T wait_text = {0};
    TRACE_APP_COMMON("MSG ID =%d",msg_id);
    MMK_CloseWin(MMIBT_WAIT_WIN_ID);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        case MSG_NOTIFY_MIDSK:
        case MSG_APP_WEB:
        {
            TRACE_APP_COMMON("MSG_NOTIFY_CANCEL");
            return_value = BT_InputPairPin(&g_dev_pin_input.addr,(uint8*)BT_DEFAULT_PIN_CODE,strlen(BT_DEFAULT_PIN_CODE));
            if (BT_PENDING == return_value)
            {
                MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
                MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
            }
            else
            {
                 MMK_CloseWin(win_id);
            }
        }
            break;
        case  MSG_NOTIFY_OK:
        {
            TRACE_APP_COMMON("MSG_NOTIFY_OK");
            MMK_CloseWin(win_id);
            BT_CancelPair(&g_dev_pin_input.addr);
         }
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            ret = MMI_RESULT_FALSE;
            break;
    }

    return ret;
}

/*****************************************************************************/
//  Description  : open pin mode query win.
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void OpenPinPairQueryWin(BT_MSG_T* msg_body_ptr)
{
    MMI_STRING_T   txt_pair  = {0};
    MMI_STRING_T   txt_tip   = {0};
    wchar      wstr_tip[BT_TIP_MAX_LEN]  = {0};
    wchar        wstr_pwd[BT_SSP_NUM_LEN] = {0};

    BT_DEVICE_INFO   device_info = {0};

    WATCH_SOFTKEY_TEXT_ID_T softkey={TXT_NULL,TXT_NULL,TXT_NULL};

    MMI_IMAGE_ID_T  img_cancel = IMAGE_SET_COM_BTN_CANCEL;
    MMI_IMAGE_ID_T  img_ok     = IMAGE_SET_COM_BTN_FIXED;

    TRACE_APP_COMMON("enter.");
    if(PNULL == msg_body_ptr )
    {
        TRACE_APP_COMMON("msg_body_ptr is pnull");
        return;
    }

    if(PNULL == msg_body_ptr->body_ptr)
    {
        TRACE_APP_COMMON("msg_body_ptr->body_ptr is pnull");
        return;
    }

    device_info = *(( BT_DEVICE_INFO *) msg_body_ptr->body_ptr);

    SCI_MEMSET(&g_dev_pin_input, 0, sizeof(BT_DEVICE_INFO));
    SCI_MEMCPY(&g_dev_pin_input, &device_info,sizeof(BT_DEVICE_INFO));

    MMI_GetLabelTextByLang(STXT_ISPAIRED,&txt_pair);

    MMI_STRNTOWSTR(wstr_pwd,BT_SSP_NUM_LEN, BT_DEFAULT_PIN_CODE,BT_SSP_NUM_LEN,BT_SSP_NUM_LEN);

    MMIAPICOM_Wstrncat(wstr_tip, txt_pair.wstr_ptr, txt_pair.wstr_len);//paired:
    MMIAPICOM_Wstrncat(wstr_tip, wstr_pwd, MMIAPICOM_Wstrlen(wstr_pwd));//1234

    txt_tip.wstr_ptr = wstr_tip;
    txt_tip.wstr_len = MMIAPICOM_Wstrlen(wstr_tip);

    TRACE_APP_COMMON("pin addr is %d,%d,%d,%d,%d,%d",device_info.addr.addr[0],device_info.addr.addr[1],device_info.addr.addr[2],device_info.addr.addr[3],
                                                     device_info.addr.addr[4],device_info.addr.addr[5]);
    WatchCOM_QueryWin_2Btn_Enter(MMIBT_PASSKEY_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,softkey,HandleInputPinPair);
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : the win produrce of ssp mode
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSspPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
    BT_STATUS  return_value = BT_SUCCESS;
    MMI_STRING_T wait_text = {0};
    TRACE_APP_COMMON("MSG ID =%d",msg_id);
    MMK_CloseWin(MMIBT_WAIT_WIN_ID);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        //case MSG_NOTIFY_OK:
        case MSG_NOTIFY_MIDSK:
        case MSG_APP_WEB:
        {
            TRACE_APP_COMMON("MSG_NOTIFY_CANCEL");
            //SetIsPinCancel(FALSE);
            return_value = BT_InputPairPin(&g_pin_request_info_ext.addr,(uint8*)g_pin_request_info_ext.ssp_numric_key,strlen(g_pin_request_info_ext.ssp_numric_key));
            if (BT_PENDING == return_value)
            {
                MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
                MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
            }
            else
            {
                 MMK_CloseWin(win_id);
            }
        }
            break;
			
        case  MSG_NOTIFY_OK:
        {
            TRACE_APP_COMMON("MSG_NOTIFY_OK");
            MMK_CloseWin(win_id);
            BT_CancelPair(&g_pin_request_info_ext.addr);
         }
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            ret = MMI_RESULT_FALSE;
            break;
    }

    return ret;
}


/*****************************************************************************/
//  Description  : open ssp mode query win.
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void OpenSspPairQueryWin(BT_MSG_T* msg_body_ptr)
{
    MMI_STRING_T txt_pair = {0};
    MMI_STRING_T txt_tip  = {0};
    wchar        wstr_pwd[BT_SSP_NUM_LEN] = {0};
    wchar      wstr_tip[BT_TIP_MAX_LEN] = {0};
    BT_DEVICE_INFO_EXT device_info = {0};

    MMI_IMAGE_ID_T  img_cancel = IMAGE_SET_COM_BTN_CANCEL;
    MMI_IMAGE_ID_T  img_ok     = IMAGE_SET_COM_BTN_FIXED;
    WATCH_SOFTKEY_TEXT_ID_T softkey={TXT_NULL,TXT_NULL,TXT_NULL};

    TRACE_APP_COMMON("enter.");
    if(PNULL == msg_body_ptr )
    {
        TRACE_APP_COMMON("msg_body_ptr is pnull");
        return;
    }
    if(PNULL == msg_body_ptr->body_ptr)
    {
        TRACE_APP_COMMON("msg_body_ptr->body_ptr is pnull");
        return;
    }

    device_info = *(( BT_DEVICE_INFO_EXT *) msg_body_ptr->body_ptr);

    SCI_MEMSET(&g_pin_request_info_ext, 0, sizeof(BT_DEVICE_INFO_EXT));
    SCI_MEMCPY(&g_pin_request_info_ext, &device_info,sizeof(BT_DEVICE_INFO_EXT));

    MMI_GetLabelTextByLang(STXT_ISPAIRED,&txt_pair);

    MMI_STRNTOWSTR(wstr_pwd, BT_SSP_NUM_LEN, g_pin_request_info_ext.ssp_numric_key,BT_SSP_NUM_LEN,BT_SSP_NUM_LEN);
    MMIAPICOM_Wstrncat(wstr_tip, txt_pair.wstr_ptr, txt_pair.wstr_len);
    MMIAPICOM_Wstrncat(wstr_tip, wstr_pwd, MMIAPICOM_Wstrlen(wstr_pwd));

    txt_tip.wstr_ptr = wstr_tip;
    txt_tip.wstr_len = MMIAPICOM_Wstrlen(wstr_tip);

    TRACE_APP_COMMON("ssp addr is %d,%d,%d,%d,%d,%d",device_info.addr.addr[0],device_info.addr.addr[1],device_info.addr.addr[2],device_info.addr.addr[3],device_info.addr.addr[4],device_info.addr.addr[5]);
    WatchCOM_QueryWin_2Btn_Enter(MMIBT_PASSKEY_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,softkey,HandleSspPairQueryWin);
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : process the message of MSG_OPEN_WINDOW
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_OPEN_WINDOW( MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T listCtrlId =  MMIBT_MAIN_LIST_CTRL_ID;

    MMK_SetAtvCtrl( win_id, listCtrlId );

    if (g_watch_bt_inq_dev_info != NULL)
    {
        SCI_FREE(g_watch_bt_inq_dev_info);
        g_watch_bt_inq_dev_info = NULL;
    }

    g_watch_bt_inq_dev_info = (BT_DEVICE_INFO *)SCI_ALLOCA(sizeof(BT_DEVICE_INFO)*MMIBT_MAX_DEV_ITEMS);
    if (g_watch_bt_inq_dev_info == NULL)
    {
        TRACE_APP_COMMON("g_watch_bt_inq_dev_info == NULL");
    }
    g_bt_inq_dev_num = 0;

    GUILIST_SetMaxItem( listCtrlId, (MMIBT_MAX_DEV_ITEMS + MMIBT_MAX_DEV_ITEMS + BT_MAIN_LIST_PERMANENT_ITEM_NUM), FALSE );
    if(TRUE == BT_GetState())
    {
        StartSearchDevice();
    }
}


/*****************************************************************************/
//  Description  : process the message of MSG_CLOSE_WINDOW
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_CLOSE_WINDOW(MMI_WIN_ID_T win_id)
{
    TRACE_APP_COMMON("enter.");
    if (g_watch_bt_inq_dev_info != NULL)
    {
        SCI_FREE(g_watch_bt_inq_dev_info);
        g_watch_bt_inq_dev_info = NULL;
    }
    g_bt_inq_dev_num = 0;

    if (searching_flag)
    {
        CancelSearchDevice();    
    }

    TRACE_APP_COMMON("exit");
}


/*****************************************************************************/
//  Description  : process the message of MSG_FULL_PAINT
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_FULL_PAINT( MMI_WIN_ID_T win_id)
{
    TRACE_APP_COMMON("enter.");
    ReCreateList(MMIBT_MAIN_LIST_CTRL_ID);
    UpdateSoftKey(win_id);

    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : process the message of MSG_APP_OK
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_APP_OK( MMI_WIN_ID_T win_id)
{
    uint16              index = 0;
    uint32              userData = 0;
    BOOLEAN             reCode = FALSE;
    BT_ITEM_FUNC        itemFunc = PNULL;
    MMI_CTRL_ID_T       listCtrlId =  MMIBT_MAIN_LIST_CTRL_ID;
    TRACE_APP_COMMON("enter.");

    index = GUILIST_GetCurItemIndex( listCtrlId );
    reCode = GUILIST_GetItemData( listCtrlId, index, (uint32*)(&userData) );
    if( FALSE == reCode )
    {
        TRACE_APP_COMMON("GUILIST_GetItemData fail!!!");
        return;
    }

    TRACE_APP_COMMON("itemFunc = %0X.", itemFunc);
    itemFunc = (BT_ITEM_FUNC)userData;
    if ( PNULL != itemFunc )
    {
        (itemFunc)();
    }

    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : process the message of MSG_APP_CANCEL
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL void Handle_MSG_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    BT_STATUS return_value = BT_SUCCESS;
    TRACE_APP_COMMON("enter.");

    if(searching_flag == TRUE)
    {
        CancelSearchDevice();
    }
    else
    {
        MMK_CloseWin(win_id);
    }
    TRACE_APP_COMMON("exit.");
}


/*****************************************************************************/
//  Description  : produrce of Bt Main Window
//  Paramter:
//          [In]:win_id msg_id param
//          [Out]:
//          [Return]:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBTMainWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E result =  MMI_RESULT_TRUE;
    TRACE_APP_COMMON("msg_id = %08x", msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_COMMON("MSG_OPEN_WINDOW");
            Handle_MSG_OPEN_WINDOW(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_COMMON("MSG_CLOSE_WINDOW");
            Handle_MSG_CLOSE_WINDOW(win_id);
            break;
        }

        case MSG_FULL_PAINT:
        {
            TRACE_APP_COMMON("MSG_FULL_PAINT");
            Handle_MSG_FULL_PAINT(win_id);
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
            uint16 index = 0;
            index = GUILIST_GetCurItemIndex( MMIBT_MAIN_LIST_CTRL_ID );

            if(FALSE == searching_flag || (0 == index)/*1355566*/) //bug1352219 whhen status is search,not alllowed to pair
            {
                TRACE_APP_COMMON("MSG_APP_OK");
                Handle_MSG_APP_OK(win_id);
            }
            else if(TRUE == searching_flag) //bug1904726 when status is searching,the search will be canceled first
            {
                TRACE_APP_COMMON("searching...");
                BT_CancelSearch();
                PopUpPromptMsg(TXT_BT_STOP_SEARCH_NOTE); //bug 1355133
            }
            else
            {
                TRACE_APP_COMMON("do nothing");
            }
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            TRACE_APP_COMMON("MSG_APP_CANCEL");
            Handle_MSG_APP_CANCEL(win_id);
            break;
        }
        default:
            result =  MMI_RESULT_FALSE;
            break;
    }

    TRACE_APP_COMMON("msg_id = %08x. end", msg_id);
    return result;
}


/*win table*/
WINDOW_TABLE( MMIBT_MAIN_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleBTMainWindow ),
    WIN_ID( MMIBT_MAIN_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIBT_MAIN_LIST_CTRL_ID ),
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};


/*---------------------------------------------------------------------------*
**                          PUBLIC FUNCTION DEFINITION                                  *
**---------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : enter bluetooth main window
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void BT_MainWin_Enter( void )
{
    MMK_CreateWin( (uint32*)MMIBT_MAIN_WIN_TAB, PNULL );
}

/****************************************************************************/
//  Description : process ps msg
//  Global resource dependence :
//  Author:zhikun.lv
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN WatchBtMain_ProcessBtPsMsg(uint16 msg_id, const BT_MSG_T *msg_body_ptr)
{
    BOOLEAN result = FALSE;
    TRACE_APP_COMMON("enter. msg_id = %0X.", msg_id);
    if (msg_body_ptr == NULL)
    {
        TRACE_APP_COMMON("msg_body_ptr == NULL.");
        return FALSE;
    }
#if 0 //bug1515390
    if (!MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
    {
        TRACE_APP_COMMON("MMIBT_MAIN_WIN_ID not opened.");
        return FALSE;
    }
#endif
    switch(msg_id)
    {
        case MMI_BT_ME_ON_CNF:
        {
            s_bt_status = MMIBT_STATUS_NONE;
            if(BT_SUCCESS == msg_body_ptr->status)
            {
                BT_STATUS search_ret = BT_SUCCESS;
                
                TRACE_APP_COMMON("MMI_BT_ME_ON_CNF success.");

                //update nv should invoke at app.
                BT_UpdateNV();

                //start auto search
                StartSearchDevice();

                UpdateAllWin();
                //update statusbar
                MAIN_SetIdleBlueToothState(ON_AND_VISIBLE);

#ifdef BT_BQB_SUPPORT
                MMIAPIBT_SetNetWorkRoamStatus(MMIAPIPHONE_GetIsNetworkRoaming());
                MMIAPIBT_SetOperatorName();//bug1515678 update operator name
#ifndef BT_DIALER_SUPPORT
                MMIAPIBT_SetMyPhoneNumber();
#endif
#endif
            }
            else
            {
                TRACE_APP_COMMON("MMI_BT_ME_ON_CNF failed.");
            }
            break;
        }

        case MMI_BT_ME_OFF_CNF:
        {
            s_bt_status = MMIBT_STATUS_NONE;
            if(BT_SUCCESS == msg_body_ptr->status)
            {
                TRACE_APP_COMMON("MMI_BT_ME_OFF_CNF success.");

                //update nv should invoke at app.
                BT_UpdateNV();

                searching_flag = FALSE;

                //make all inquired number no valid.
                g_bt_inq_dev_num = 0;

                UpdateAllWin();

			    //update statusbar
			    MAIN_SetIdleBlueToothState(OFF_BLUETOOTH);
            }
            else
            {
                TRACE_APP_COMMON("MMI_BT_ME_OFF_CNF failed.");
            }
            break;
        }

        case MMI_BT_INQ_DEV_NAME:
        {
            TRACE_APP_COMMON("MMI_BT_INQ_DEV_NAME enter.");
            if(BT_SUCCESS == msg_body_ptr->status)
            {
                BT_DEVICE_INFO dev_info ={0};

                if (g_bt_inq_dev_num == MMIBT_MAX_DEV_ITEMS -1)
                {
	           TRACE_APP_COMMON("g_bt_inq_dev_num == MMIBT_MAX_DEV_ITEMS -1.");
                    break;
                }

                if (msg_body_ptr->body_ptr == NULL)
                {
                    TRACE_APP_COMMON("MMI_BT_INQ_DEV_NAME, msg_body_ptr->body_ptr == NULL. break.");
                    break;
                }
                dev_info = *((BT_DEVICE_INFO*)msg_body_ptr->body_ptr);

                if (IsDeviceInPairedList(&dev_info))
                {
                    TRACE_APP_COMMON("MMI_BT_INQ_DEV_NAME, device has in paired list.");
                    break;
                }

                //update data list
                if (g_watch_bt_inq_dev_info != NULL)
                {
                    g_watch_bt_inq_dev_info[g_bt_inq_dev_num] = dev_info;
                    g_bt_inq_dev_num++;

                    UpdateAllWin();

                    TRACE_APP_COMMON("MMI_BT_INQ_DEV_NAME success.");
                }
                
            }
            else
            {
                TRACE_APP_COMMON("MMI_BT_INQ_DEV_NAME failed.");
            }
            break;
        }

        case MMI_BT_INQ_COMP_CNF:
        {
            TRACE_APP_COMMON("MMI_BT_INQ_COMP_CNF enter.");
            if(BT_SUCCESS == msg_body_ptr->status)
            {
                TRACE_APP_COMMON("MMI_BT_INQ_COMP_CNF success.");
            }
            else
            {
                //pop up prompt msg "not allowed search"
                PopUpPromptMsg(TXT_ERROR);
                TRACE_APP_COMMON("MMI_BT_INQ_COMP_CNF failed.");
            }

            searching_flag = FALSE;
            UpdateAllWin();
            break;
        }

        case MMI_BT_INQUIRY_CANCEL:
        {
            TRACE_APP_COMMON("MMI_BT_INQUIRY_CANCEL enter.");
            if(BT_SUCCESS == msg_body_ptr->status)
            {
                TRACE_APP_COMMON("MMI_BT_INQUIRY_CANCEL success.");
            }
            else
            {
                TRACE_APP_COMMON("MMI_BT_INQUIRY_CANCEL failed.");
            }

            searching_flag = FALSE;
            UpdateAllWin();
            break;
        }

        case MMI_BT_DEV_PIN_REQ:
        {
            TRACE_APP_COMMON("MMI_BT_DEV_PIN_REQ enter.");
            if(BT_SUCCESS == msg_body_ptr->status)
            {
                BT_MSG_T temp_msg_body = {0}; // Bug 1911405-PCLINT, Bug 1916276
                TRACE_APP_COMMON("MMI_BT_DEV_PIN_REQ success.");

                temp_msg_body.msg_id = msg_body_ptr->msg_id;
                temp_msg_body.status = msg_body_ptr->status;
                temp_msg_body.body_size = msg_body_ptr->body_size;
                temp_msg_body.body_ptr = (void*)SCI_ALLOC_APPZ((msg_body_ptr->body_size) + 1);
                if(PNULL == temp_msg_body.body_ptr)
                {
                    TRACE_APP_COMMON("temp_msg_body.body_ptr malloc error \r\n");
                    break;
                }

                SCI_MEMCPY(temp_msg_body.body_ptr,msg_body_ptr->body_ptr,msg_body_ptr->body_size);
                OpenPinPairQueryWin(&temp_msg_body);
                SCI_FREE(temp_msg_body.body_ptr);
            }
            else
            {
                TRACE_APP_COMMON("MMI_BT_DEV_PIN_REQ failed.");
            }
            break;
        }

        case MMI_BT_SSP_NUM_IND:
        {
            TRACE_APP_COMMON("MMI_BT_SSP_NUM_IND enter.");
            if(BT_SUCCESS == msg_body_ptr->status)
            {
                BT_MSG_T temp_msg_body = {0}; // Bug 1911405-PCLINT, Bug 1916276
                TRACE_APP_COMMON("MMI_BT_SSP_NUM_IND success.");

                temp_msg_body.msg_id = msg_body_ptr->msg_id;
                temp_msg_body.status = msg_body_ptr->status;
                temp_msg_body.body_size = msg_body_ptr->body_size;
                temp_msg_body.body_ptr = (void*)SCI_ALLOC_APPZ((msg_body_ptr->body_size) + 1);
                if(PNULL == temp_msg_body.body_ptr)
                {
                    TRACE_APP_COMMON("temp_msg_body.body_ptr malloc error \r\n");
                    break;
                }

                SCI_MEMCPY(temp_msg_body.body_ptr,msg_body_ptr->body_ptr,msg_body_ptr->body_size);
                OpenSspPairQueryWin(&temp_msg_body);
                SCI_FREE(temp_msg_body.body_ptr);
            }
            else
            {
                TRACE_APP_COMMON("MMI_BT_SSP_NUM_IND failed.");
            }
            break;
        }
        case MMI_BT_DEV_PAIR_COMPLETE_IND:
        case MMI_BT_DEV_PAIR_COMPLETE:
        {
            TRACE_APP_COMMON("MMI_BT_DEV_PAIR_COMPLETE enter.");

            //close all waiting win
            MMK_CloseWin(MMIBT_PASSKEY_WIN_ID);
            MMK_CloseWin(MMIBT_PAIR_WAIT_WIN_ID);
            MMK_CloseWin(MMIBT_WAIT_WIN_ID);

            MMIBT_SetSelfPair(FALSE);

            if (BT_SUCCESS == msg_body_ptr->status)
            {
                uint32 i = 0;
                int32 find_idx = -1;
                BT_DEVICE_INFO device_info = {0};
                device_info = *(BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
                TRACE_APP_COMMON("MMI_BT_DEV_PAIR_COMPLETE success.");

                // step1. remove device from inquired list
                if (g_bt_inq_dev_num == 0 || g_watch_bt_inq_dev_info == NULL)
                {
                    break;
                }

                //find current paired item in inquired list
                for (i = 0; i < g_bt_inq_dev_num; i++)
                {
                    if (IsSameBtAddr(&device_info, &g_watch_bt_inq_dev_info[i]))
                    {
                        find_idx = i;
                        break;
                    }
                }

                // if find it, remove it
                if (find_idx != -1)
                {
                    for (i = find_idx; i < g_bt_inq_dev_num - 1; i ++)
                    {
                        g_watch_bt_inq_dev_info[i] = g_watch_bt_inq_dev_info[i+1];
                    }
                    g_bt_inq_dev_num--;
                }

                UpdateAllWin();

                //tip paired success win
                do
                {
                    uint16 dev_type = 0;
                    MMI_STRING_T txt_error = {0};
                    WATCH_SOFTKEY_TEXT_ID_T softkey={TXT_NULL,TXT_NULL,TXT_NULL};
                    MMI_GetLabelTextByLang( TXT_SUCCESS, &txt_error );
                    WatchCOM_NoteWin_1Line_Enter(MMIBT_NOTE_WIN_ID,&txt_error,0,softkey,PNULL);
                    BT_GetDeviceType(&g_pin_request_info, &dev_type);
                    TRACE_APP_COMMON("MMI_BT_DEV_PAIR_COMPLETE device type = %d",dev_type);
                    if(dev_type & BT_DEVICE_CLASS_AUDIO)
                    {
                        //headset need open it directly
                        TRACE_APP_COMMON("MMI_BT_DEV_PAIR_COMPLETE adio type.");

                        TRACE_APP_COMMON("pin addr %d,%d,%d,%d,%d,%d",g_pin_request_info.addr.addr[0],g_pin_request_info.addr.addr[1],g_pin_request_info.addr.addr[2],g_pin_request_info.addr.addr[3],g_pin_request_info.addr.addr[4],g_pin_request_info.addr.addr[5]);
                        if(HeadsetOpenIsPermit(&(g_pin_request_info.addr)))
                        {
                            OpenHeadset(&g_pin_request_info.addr);
                        }
                    }
                } while(0);
            }
            else
            {
                //tip paired failed win
                MMI_STRING_T txt_error = {0};
                WATCH_SOFTKEY_TEXT_ID_T softkey={TXT_NULL,TXT_NULL,TXT_NULL};
                MMI_GetLabelTextByLang( TXT_ERROR, &txt_error );
                WatchCOM_NoteWin_1Line_Enter(MMIBT_NOTE_WIN_ID,&txt_error,0,softkey,PNULL);
                TRACE_APP_COMMON("MMI_BT_DEV_PAIR_COMPLETE failed.");
            }
            break;
        }
        case MMI_BT_HFAG_SLC_DISCONNECTED:
        case MMI_BT_A2DP_DISCONNECTED:
        {
            BT_STATUS del_ret = BT_SUCCESS;
            BT_ADDRESS addr = {0};
            if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
            {
                TRACE_APP_COMMON("msg_body_ptr is null");
                break;
            }
            if(MMI_BT_A2DP_DISCONNECTED == msg_id)//fix bug1492118
            {
                BT_A2DP_INFO_T a2dp_info = {0};

                a2dp_info = *(BT_A2DP_INFO_T *)msg_body_ptr->body_ptr;
                SCI_MEMCPY(&(addr.addr), a2dp_info.dev_addr.addr, BT_ADDRESS_SIZE);
            }
            else
            {
                BT_DEVICE_INFO  device = {0};

                device = *(BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
                SCI_MEMCPY(&(addr.addr), device.addr.addr, BT_ADDRESS_SIZE);
            }
            TRACE_APP_COMMON("g_remove_paired_dev_infoaddr %d,%d,%d,%d,%d,%d",g_remove_paired_dev_info.addr.addr[0],g_remove_paired_dev_info.addr.addr[1],g_remove_paired_dev_info.addr.addr[2],g_remove_paired_dev_info.addr.addr[3],g_remove_paired_dev_info.addr.addr[4],g_remove_paired_dev_info.addr.addr[5]);
            TRACE_APP_COMMON("device address %d,%d,%d,%d,%d,%d",addr.addr[0],addr.addr[1],addr.addr[2],addr.addr[3],addr.addr[4],addr.addr[5]);

            if(HeadsetIsClosed(&addr,PNULL))  //bug1352192
            {
                if(!memcmp(&g_remove_paired_dev_info.addr, &addr, sizeof(BT_ADDRESS)))
                {
                    TRACE_APP_COMMON("delete BT headset");
                    del_ret = BT_RemovePairedDevice(&addr);
                    SCI_MEMSET(&g_remove_paired_dev_info,0,sizeof(BT_DEVICE_INFO));
                    TRACE_APP_COMMON("del_ret = %d",del_ret);
                }
            }else
            {
                del_ret = CloseHeadset(&addr);
            }
        }
            break;

        case MMI_BT_DELETE_DEVICE_RES:
        {
            MMI_STRING_T txt_error = {0};
            WATCH_SOFTKEY_TEXT_ID_T softkey={TXT_NULL,TXT_NULL,TXT_NULL};
            if (BT_SUCCESS == msg_body_ptr->status)
            {
                TRACE_APP_COMMON("MMI_BT_DELETE_DEVICE_RES success.");
                UpdateAllWin();
            }else
            {
                TRACE_APP_COMMON("MMI_BT_DELETE_DEVICE_RES fail.");
                //pop up prompt msg "delete fail"
              
                MMI_GetLabelTextByLang( TXT_ERROR, &txt_error );
                WatchCOM_NoteWin_1Line_Enter(MMIBT_NOTE_WIN_ID,&txt_error,0,softkey,PNULL);
                TRACE_APP_COMMON("MMI_BT_DELETE_DEVICE_RES failed.");
            }
        }
            MMK_CloseWin(MMIBT_MAIN_WIN_ID);
            break;

        default:
            TRACE_APP_COMMON("default.");
            break;
    }

    TRACE_APP_COMMON("exit.");
    result = TRUE;
    return result;
}
