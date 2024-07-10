/*****************************************************************************
** File Name:      adultwatch_bt_win.c                                       *
** Author:         chunjuan.liang                                            *
** Date:           01/12/2021                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2021      chunjuan.liang            Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
//#include "std_header.h"
#endif
#include "mmibt_func.h"
#include "mmibt_app.h"
#include "mmibt_image.h"
#include "mmibt_text.h"
#include "mmiset_export.h"
#include "mmibt_nv.h"

#include "guires.h"
#include "guictrl_api.h"

#include "mmicom_trace.h"
#include "mmiset_image.h"

#include "ual_bt.h"
#include "adultwatch_bt_win.h"
#ifdef UAL_BT_HFU_SUPPORT
#include "ual_bt_hfu.h"
#endif
#include "watch_commonwin_export.h"
#include "mmibt_headset.h"
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
#include "adultwatch_bt_pair.h"
#include "adultwatch_bt_pininput_win.h"

#include "watch_common_list.h"//bug1668384

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define BT_MAIN_TITLE_MAX_LEN                   (20)
#ifdef BBM_ROLE_SUPPORT
#define BT_MAIN_LIST_PERMANENT_ITEM_NUM         (0)
#else
#define BT_MAIN_LIST_PERMANENT_ITEM_NUM         (1)
#endif

#define BT_DEFAULT_PIN_CODE                     "1234"

#define BT_TOTAL_ITEM                           (UAL_BT_DEV_MAX_COUNT/*paired device*/+UAL_BT_DEV_MAX_COUNT/*inquire device*/+BT_MAIN_LIST_PERMANENT_ITEM_NUM)


/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL ual_bt_dev_info_ssp_t g_ssp_request_info_ext = {0};//ssp code
LOCAL ual_bt_dev_info_t         g_pin_request_info = {0};//pin code

LOCAL ual_bt_dev_info_t             g_pair_req_dev = {0};//发起配对设备信息，若是audio设备，需要发起连接

LOCAL BOOLEAN                     g_searching_flag = FALSE;//是否在searching状态下进行配对/取消配对

LOCAL uint8 g_removed_headset_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};

typedef enum 
{
    BT_LIST_TYPE_ON_OFF = 0,
    BT_LIST_TYPE_PAIRED,
    BT_LIST_TYPE_SERACH,
    BT_LIST_TYPE_MAX
}WATCH_BT_LIST_ITEM_TYPE;

typedef struct bt_list_item_data_t
{
    WATCH_BT_LIST_ITEM_TYPE type;
    uint32 fun_cb;
    uint16 index;
    uint16 position;// current item's position in searched list
}bt_list_item_data_t;


LOCAL bt_list_item_data_t s_list_data[BT_TOTAL_ITEM] = {0};
LOCAL uint32 s_list_item_num = 0;

//list item callback function
typedef void ( *BT_ITEM_FUNC )( void );


/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleOnOffAction(void);

/*****************************************************************************/
//  Description  : load on/off state from bt  ps, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendOnOffItemToList(MMI_CTRL_ID_T list_ctrl_id);

#ifdef UAL_BT_HFU_SUPPORT
/*****************************************************************************/
//  Description  : load the data of bound paired devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendBoundItemsToList(MMI_CTRL_ID_T list_ctrl_id);
#endif

/*****************************************************************************/
//  Description  : load the data of paired devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendPairedItemsToList(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description  : load the data of searched devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendSearchedItemsToList(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description  : remove all items in list_ctrl, reload all data and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void ReCreateList(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description  : get the image id of device by dev info
//  Paramter:
//          [In]:dev ------ device info
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDevIconByDevInfo(ual_bt_dev_info_t *dev);
#ifdef UAL_BT_HFU_SUPPORT
/****************************************************************************/
//  Description : WATCHBT_HFUConnect
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:hfu connect，当前hfu只能连接一个
/****************************************************************************/
LOCAL BOOLEAN WATCHBT_ConnectBoundDev(void);
/*****************************************************************************/
//  Discription: This function is used to Get the Paired bt dev list
//  Global resource dependence: None
//  Author:chunjuan.liang
//  Note :this function is used to put bound dev in the first position
//  Return:None
/*****************************************************************************/
LOCAL uint16 AdultWatchBT_GetBoundDevIndex(void);
#endif

/*****************************************************************************/
//  Description  : HandleCancelPairQueryWin
//  Paramter:
//          [In]:win id,msg id 
//          [Out]:None
//          [Return]:result
//  Author: chunjuan.liang
//  Note:取消配对时，弹出窗体
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param );

/*****************************************************************************/
//  Description  : produrce of Bt Main Window
//  Paramter:
//          [In]:win_id msg_id param
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdultWatchBTMainWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         );

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION DEFINITION                        *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description  : get the image id of device by dev info
//  Paramter:
//          [In]:dev ------ device info
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDevIconByDevInfo(ual_bt_dev_info_t *dev)
{
    uint16 dev_type = 0;
    uint16 icon_index = 0;
    MMI_IMAGE_ID_T  watch_pair_icon_array[] = {IMAGE_BT_HEADSET, IMAGE_BT_PHONE, IMAGE_BT_LAPTOP};
    if (PNULL == dev)
    {
        TRACE_APP_BT("PNULL == dev");
       return watch_pair_icon_array[icon_index];
    }
    ual_bt_dev_get_dev_type(dev, &dev_type);
    TRACE_APP_BT("dev_type=%d",dev_type);
    if(dev_type & UAL_BT_DEVICE_CLASS_AUDIO)
    {
        icon_index = 0;
    }
    else if(dev_type & UAL_BT_DEVICE_CLASS_PHONE)
    {
        icon_index = 1;
    }
    else
    {
        icon_index = 2;
    }
    return watch_pair_icon_array[icon_index];
}

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
    MMI_GetLabelTextByLang( txt_id, &txt_msg );
    Adult_WatchCOM_NoteWin_1Line_Toast_Enter(ADULTWATCH_BT_NOTE_WIN_ID,&txt_msg,0,PNULL);

    return TRUE;
}
LOCAL BOOLEAN BoundDevIsInPairedList(void)
{
    BOOLEAN ret = FALSE;

    uint8 bond_dev_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};

#ifdef UAL_BT_HFU_SUPPORT
#ifdef BBM_ROLE_SUPPORT
        Bbm_GetBTBoundDevAddr(bond_dev_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);
#else
        WATCHBT_GetBoundDevAddr(bond_dev_addr);
#endif
#endif
    ret = ual_bt_dev_is_in_paired_list(bond_dev_addr);

    TRACE_APP_BT("ret = %d",ret);

    return ret;
}
/*****************************************************************************/
//  Description:process the action of press on the item of [on/off]
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleOnOffAction(void)
{
    BOOLEAN is_bt_on = ual_bt_dev_get_is_on();
    ual_bt_status_e result = UAL_BT_STATUS_SUCCESS;

    TRACE_APP_BT("enter. is_bt_on = %d.", is_bt_on);

    if (!is_bt_on)
    {
        result = ual_bt_dev_open();
        TRACE_APP_BT("ual_bt_dev_open(). result = %d.", result);
        if(UAL_BT_STATUS_PENDING == result)
        {
            //waiting win
        }
        else
        {
            //pop up msg
        }
    }
    else
    {
        result = ual_bt_dev_close();
        TRACE_APP_BT("ual_bt_dev_close(). result = %d.", result);
        if(UAL_BT_STATUS_PENDING == result)
        {
            //waiting win
        }
        else
        {
            //pop up msg
        }
    }
    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description:draw BT On/Off status to ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendOnOffItemToList(MMI_CTRL_ID_T list_ctrl_id)
{
    BOOLEAN is_bt_on = FALSE;
    MMI_IMAGE_ID_T image_id =0;
    uint32 text_id = 0; 
    MMI_STRING_T string[2] = {0};
    uint16 list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);
    uint32 user_data = 0;
    bt_list_item_data_t item_data_info = {0};

    TRACE_APP_BT("enter.");
    is_bt_on = ual_bt_dev_get_is_on();
    TRACE_APP_BT("is_bt_on = %d", is_bt_on);

    image_id = is_bt_on ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON;
    text_id = is_bt_on ? TXT_COMMON_OPEN : TXT_COMMON_CLOSE;
    MMI_GetLabelTextByLang(TXT_BLUETOOTH, &string[0]);
    MMI_GetLabelTextByLang(text_id, &string[1]);

    AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line(list_ctrl_id, string[0], string[1], image_id);

    item_data_info.type = BT_LIST_TYPE_ON_OFF;
    item_data_info.fun_cb = (uint32)HandleOnOffAction;
    s_list_data[s_list_item_num] = item_data_info;
    user_data = &s_list_data[s_list_item_num];
    GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);

    s_list_item_num++;

    TRACE_APP_BT("exit s_list_item_num=%d.",s_list_item_num);
}


/*****************************************************************************/
//  Description: remove all items in list_ctrl, reload all data and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void ReCreateList(MMI_CTRL_ID_T list_ctrl_id)
{
    int32 listOffset = 0;
    uint16 focus_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    TRACE_APP_BT("enter.");

    // get top item offset
    GUILIST_GetTopItemOffset( list_ctrl_id, &listOffset );

    //remove all list items
    GUILIST_RemoveAllItems(list_ctrl_id);

    s_list_item_num = 0;
#ifndef BBM_ROLE_SUPPORT //BBM TODO
    //append on/off item
    AppendOnOffItemToList(list_ctrl_id);
#endif
    //append paired list && searched list when device is on
    if (ual_bt_dev_get_is_on())
    {
        if(UAL_BT_PHONE == ual_bt_get_product_role())//is_phone =1 means phone, is_phone=0 means headset
        {
            AppendPairedItemsToList(list_ctrl_id);
            AppendSearchedItemsToList(list_ctrl_id);
        }
#ifndef BBM_ROLE_SUPPORT //BBM TODO
#ifdef UAL_BT_HFU_SUPPORT
        else if(UAL_BT_DIALER == ual_bt_get_product_role())
        {
            TRACE_APP_BT("dialer mode,no need display search list.");
            AppendBoundItemsToList(list_ctrl_id);
        }
#endif
#endif
    }

    //set focus index
    GUILIST_SetCurItemIndex(list_ctrl_id, focus_index);
    GUILIST_SetTopItemOffset(list_ctrl_id,listOffset);
    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description : redraw all sub ctrls in window
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void GetWinTitle(wchar *title,uint16 *title_len)
{
    MMI_STRING_T      txt_bt = {0};
    MMI_STRING_T   txt_role  = {0};
    wchar      filter_wstr[] = { '-', 0 };
    uint16        filter_len = 1;
    uint16         wchar_len = 0;

    if(PNULL == title || 0 == title_len)
    {
        TRACE_APP_BT("title error");
        return;
    }

    MMI_GetLabelTextByLang(TXT_BLUETOOTH, &txt_bt);

    // TODO:use bbm api
    if(UAL_BT_DIALER == ual_bt_get_product_role())//dialer
    {
        MMI_GetLabelTextByLang(TXT_BT_DIALER_MODE, &txt_role);
    }
    else if(UAL_BT_PHONE == ual_bt_get_product_role())//phone
    {
        MMI_GetLabelTextByLang(TXT_BT_PHONE_MODE, &txt_role);
    }

    MMIAPICOM_Wstrncat(title,txt_bt.wstr_ptr,txt_bt.wstr_len);
    MMIAPICOM_Wstrncat(title,filter_wstr,1);
    MMIAPICOM_Wstrncat(title,txt_role.wstr_ptr,txt_role.wstr_len);
    *title_len = txt_bt.wstr_len + 1 + txt_role.wstr_len;
}

/*****************************************************************************/
//  Description  : process the action of press on the item of seached 
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleSearchedDeviceAction(void)
{
    ual_bt_status_e return_value = UAL_BT_STATUS_SUCCESS;
    uint32 service_type = UAL_BT_SERVICE_ALL;
    uint16 index  = GUILIST_GetCurItemIndex(MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID);
    uint32 paired_cnt = ual_bt_dev_get_paired_dev_count(service_type);

    ual_bt_dev_info_t *p_searched_dev_info = PNULL;
    
    ual_bt_dev_status_e bt_status = UAL_BT_DEV_STATUS_INIT;

    p_searched_dev_info = ual_bt_dev_get_searched_list();

    TRACE_APP_BT("enter. paired_cnt = %d.", paired_cnt);

    if(PNULL == p_searched_dev_info)
    {
        TRACE_APP_BT("PNULL == p_searched_dev_info.");
        return;
    }

    //if connected device in paired list,pop up prompt msg
    if(MMIAPIBT_GetActiveBTHeadset() /*|| BT_GetActivatedHS(&addr)*/) //bug1352187
    {
        PopUpPromptMsg(TXT_ACTIVE_HEADSET_ALERT);
        return;
    }
    bt_status = ual_bt_dev_get_status();
    if(UAL_BT_DEV_STATUS_SEARCHING == bt_status)
    {
        TRACE_APP_BT("cancel search");
        ual_bt_dev_cancel_search();
        g_searching_flag = TRUE;
    }
    else
    {
        uint32 userData = 0;
        bt_list_item_data_t *item_data = PNULL;

        //get list item data
        CTRLLIST_GetItemData(MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID,index,(uint32*)(&userData));

        item_data = (bt_list_item_data_t *)userData;
        if(PNULL == item_data)
        {
            TRACE_APP_BT("PNULL == item_data.");
            return;
        }
        g_searching_flag = FALSE;
        //copy selected device info to g_pin_request_info.
        SCI_MEMSET(&g_pair_req_dev, 0, sizeof(ual_bt_dev_info_t));
        TRACE_APP_BT("index=%d,%x",index);
        SCI_MEMCPY(&g_pair_req_dev, &p_searched_dev_info[item_data->position], sizeof(ual_bt_dev_info_t));//Bug 1949728

        //invoke BT_PairDevice
        TRACE_APP_BT("addr=%x,%x,%x,%x,%x,%x",g_pair_req_dev.addr[0],g_pair_req_dev.addr[1],g_pair_req_dev.addr[2],
                                              g_pair_req_dev.addr[3],g_pair_req_dev.addr[4],g_pair_req_dev.addr[5]);
        return_value = ual_bt_dev_start_pair(g_pair_req_dev.addr);
        TRACE_APP_BT("invoke BT_PairDevice(), return = %d.", return_value);
        if (UAL_BT_STATUS_PENDING != return_value)
        {
            MMI_STRING_T  txt_error = {0};
            MMI_GetLabelTextByLang( TXT_BT_PAIR_FAIL, &txt_error );
            TRACE_APP_BT("invoke BT_PairDevice() failed.");

            //tip error win
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter(ADULTWATCH_BT_NOTE_WIN_ID,&txt_error,0,PNULL);
        }
        else
        {
            MMI_STRING_T txt_wait = {0};
            TRACE_APP_BT("invoke start pair success.");
        
            //tip waiting win
            MMI_GetLabelTextByLang(TXT_BT_PAIR_REQUEST, &txt_wait);
            AdultWatchCOM_WaitingWin_1Line_Enter(ADULTWATCH_BT_PAIR_REQUEST_WIN_ID,&txt_wait,0, PNULL/*HandleCanbeCancelWaitingWinMsg*/);
        }
    }
    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description  : load the data of searched devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendSearchedItemsToList(MMI_CTRL_ID_T list_ctrl_id)
{
    TRACE_APP_BT("enter.");
    while (1)
    {
        uint32 i = 0;
        MMI_STRING_T bt_name = {0};
        MMI_STRING_T bt_status_str  = {0};
        MMI_IMAGE_ID_T dev_icon = 0;
        uint16 list_index = 0;
        bt_list_item_data_t item_data_info = {0};

        uint8  searched_dev_num = 0;
        ual_bt_dev_info_t *p_searched_dev_info = PNULL;

        searched_dev_num = ual_bt_dev_get_searched_num();
        p_searched_dev_info = ual_bt_dev_get_searched_list();

        MMI_GetLabelTextByLang(TXT_SET_BT_AVAILABLE, &bt_status_str);

        if (p_searched_dev_info == PNULL || searched_dev_num == 0)
        {
            TRACE_APP_BT("searched list not valid.");
            break;
        }

        TRACE_APP_BT("searched_dev_num = %d. searched_dev_info_ptr's addr = %0x.",
            searched_dev_num,
            p_searched_dev_info);

        for (i = 0; i < searched_dev_num; i++)
        {
            ual_bt_dev_info_t device_info = {0};
            uint32 user_data = 0;

            //current info
            device_info = p_searched_dev_info[i];

            if(TRUE == ual_bt_dev_is_in_paired_list(device_info.addr))//bug 1592413若该设备在配对列表中，不显示该设备
            {
                TRACE_APP_BT("dev is in paired list");
                continue;
            }

            //get icon
            dev_icon = GetDevIconByDevInfo(&device_info);

            //parse name
            bt_name.wstr_ptr = device_info.name;
            bt_name.wstr_len = MMIAPICOM_Wstrlen(bt_name.wstr_ptr);

            //must set before AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line
            list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);

            //draw list item
            AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line(list_ctrl_id,dev_icon, bt_name, bt_status_str );

            //set callback
            item_data_info.type = BT_LIST_TYPE_SERACH;
            item_data_info.fun_cb = (uint32)HandleSearchedDeviceAction;
            item_data_info.position = i;//Bug 1949728
            s_list_data[s_list_item_num] = item_data_info;
            user_data = &s_list_data[s_list_item_num];
            GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);

            s_list_item_num++;
            
            GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);
        }

        //must break at this point for while(1)
        break;
    }
    TRACE_APP_BT("exit, s_list_item_num=%d.",s_list_item_num);
}

/*****************************************************************************/
//  Description: process the message of MSG_OPEN_WINDOW
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:chunjuan.liang
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleMsgOpenWin( MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T           listCtrlId =  MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID;
    MMI_STRING_T            txtSearch = {0};


    MMK_SetAtvCtrl( win_id, listCtrlId );

    GUILIST_SetMaxItem( listCtrlId, (UAL_BT_DEV_MAX_COUNT/*paired device*/ + UAL_BT_DEV_MAX_COUNT /*inquire device*/+ BT_MAIN_LIST_PERMANENT_ITEM_NUM/*on/off*/), FALSE );
    GUILIST_SetListState( listCtrlId, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );//增加list自动滚动属性

    if(UAL_BT_PHONE == ual_bt_get_product_role())//PHONE MODE
    {
        TRACE_APP_BT("phone mode,to search");
        if(ual_bt_dev_get_is_on())
        {
            ual_bt_dev_start_search(UAL_BT_SERVICE_ALL);
            MMI_GetLabelTextByLang(TXT_COMMON_SEARCHING,&txtSearch);
            AdultWatchCOM_WaitingWin_1Line_Enter(ADULTWATCH_BT_SEARCH_WAIT_WIN_ID,&txtSearch,0,PNULL);
        }
    }
    else
    {
        TRACE_APP_BT("dialer mode,not to search");
    }
}


/*****************************************************************************/
//  Description:process the message of MSG_CLOSE_WINDOW
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleMsgCloseWin(MMI_WIN_ID_T win_id)
{
    // TODO: get status
    // TODO: cancel search?
    ual_bt_dev_status_e bt_status = UAL_BT_DEV_STATUS_INIT;
    bt_status = ual_bt_dev_get_status();

    if(UAL_BT_DEV_STATUS_SEARCHING == bt_status)
    {
        ual_bt_dev_cancel_search();
    }
}

/*****************************************************************************/
//  Description: process the message of MSG_FULL_PAINT
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleMsgFullPaint( MMI_WIN_ID_T win_id)
{
    wchar title[BT_MAIN_TITLE_MAX_LEN] = {0};
    uint16                   title_len = 0;

    TRACE_APP_BT("enter.");
    GetWinTitle(title,&title_len);//get title string

    GUIWIN_SetTitleText(win_id,title,title_len,FALSE);

    ReCreateList(MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID);

    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description:process the message of MSG_APP_OK
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleMsgAppOK( MMI_WIN_ID_T win_id)
{
    uint16              index = 0;
    uint32              userData = 0;
    BOOLEAN             reCode = FALSE;
    BT_ITEM_FUNC        itemFunc = PNULL;
    MMI_CTRL_ID_T       listCtrlId =  MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID;

    bt_list_item_data_t *item_data = (bt_list_item_data_t *)userData;

    TRACE_APP_BT("enter.");

    index = GUILIST_GetCurItemIndex( listCtrlId );
    reCode = GUILIST_GetItemData( listCtrlId, index, (uint32*)(&userData) );
    if( FALSE == reCode )
    {
        TRACE_APP_BT("GUILIST_GetItemData fail!!!");
        return;
    }

    item_data = (bt_list_item_data_t *)userData;

    itemFunc = (BT_ITEM_FUNC)(item_data->fun_cb);

    TRACE_APP_BT("itemFunc = %0x.", itemFunc);
    if( PNULL != itemFunc )
    {
        (itemFunc)();
    }

    TRACE_APP_BT("exit.");
}


/*****************************************************************************/
//  Description:process the message of MSG_APP_CANCEL
//  Paramter:
//          [In]:win_id
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandleMsgAppCancel( MMI_WIN_ID_T win_id )
{
    TRACE_APP_BT("enter.");

    // TODO: search 

    MMK_CloseWin(win_id);

    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description  : get the image id of device by dev info
//  Paramter:
//          [In]:dev ------ device info
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDeviceIconByInfo(ual_bt_dev_info_t *dev)
{
    MMI_IMAGE_ID_T icon = IMAGE_BT_PHONE;
    uint16     dev_type = 0;
    uint16   icon_index = 0;
    MMI_IMAGE_ID_T  watch_pair_icon_array[] = {IMAGE_BT_HEADSET, IMAGE_BT_PHONE, IMAGE_BT_LAPTOP};

    if (PNULL == dev)
    {
        TRACE_APP_BT("PNULL == dev");
        return icon;
    }
    ual_bt_dev_get_dev_type(dev,&dev_type);
    TRACE_APP_BT("dev_type =%d",dev_type);

    if(UAL_BT_DEVICE_CLASS_AUDIO & dev_type)
    {
        icon_index = 0;
    }
    else if(UAL_BT_DEVICE_CLASS_PHONE & dev_type)
    {
        icon_index = 1;
    }
    else if(UAL_BT_DEVICE_CLASS_COMPUTER & dev_type)
    {
        icon_index = 2;
    }
    else
    {
        icon_index = 1;// 当前未知设备类型使用phone图标
    }

   return icon;
}
/*****************************************************************************/
//  Description  : process the action of press on the item of paired 
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void HandlePairedDeviceAction()
{
    MMI_STRING_T         txt_tip = {0};

    MMI_IMAGE_ID_T  img_cancel = IMAGE_SET_COM_AW_BTN_CANCEL;
    MMI_IMAGE_ID_T  img_ok     = IMAGE_SET_COM_AW_BTN_CONFIRM;

    ual_bt_dev_status_e bt_status = UAL_BT_DEV_STATUS_INIT;

    bt_status = ual_bt_dev_get_status();
    if(UAL_BT_DEV_STATUS_SEARCHING == bt_status)
    {
        TRACE_APP_BT("cancel search");
        ual_bt_dev_cancel_search();
        g_searching_flag = TRUE;
    }
    else
    {
        g_searching_flag = FALSE;
        MMI_GetLabelTextByLang(TXT_BT_CANCEL_PAIR, &txt_tip);
        AdultWatchCOM_QueryWin_2Btn_Enter(ADULTWATCH_BT_CANCEL_PAIR_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,HandleCancelPairQueryWin);
    }
}
/*****************************************************************************/
//  Description  : HandleCancelPairQueryWin
//  Paramter:
//          [In]:win id,msg id 
//          [Out]:None
//          [Return]:result
//  Author: chunjuan.liang
//  Note:取消配对时，弹出窗体
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E             ret = MMI_RESULT_TRUE;
    ual_bt_status_e return_value = UAL_BT_STATUS_SUCCESS;
    uint16                   idx = GUILIST_GetCurItemIndex(MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID);
    uint16              pair_idx = 0;
    ual_bt_status_e      del_ret = UAL_BT_STATUS_SUCCESS;

    TRACE_APP_BT("MSG ID =%x",msg_id);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        case MSG_APP_WEB:
        {
            ual_bt_dev_info_t paired_dev = {0};
            uint32          service_type = UAL_BT_SERVICE_ALL;
            uint16              dev_type = 0;
            pair_idx = s_list_data[idx].index + 1;//配对列表中，index从1开始,所以需要+1
            TRACE_APP_BT("enter pair_idx=%d.",pair_idx);

            MMK_CloseWin(win_id);

            ual_bt_dev_get_pair_info(service_type,pair_idx,&paired_dev);

            return_value = ual_bt_dev_get_dev_type(&paired_dev, &dev_type);

            TRACE_APP_BT("dev_type =%d, paired_dev addr %d,%d,%d,%d,%d,%d",dev_type,paired_dev.addr[0],paired_dev.addr[1],paired_dev.addr[2],
                                                         paired_dev.addr[3],paired_dev.addr[4],paired_dev.addr[5]);
            if(dev_type & BT_DEVICE_CLASS_AUDIO)
            {
                if(!MMIBT_DeviceIsConnectedHeadsetEx(paired_dev.addr))
                {
                    TRACE_APP_BT("remove headset dev");
                    del_ret = ual_bt_dev_delete_paired_dev(paired_dev.addr); //bug1355879

                }else
                {
                    TRACE_APP_BT(" close headset ");
                    CloseHeadsetEx(paired_dev.addr);//关闭耳机

                    SCI_MEMSET(g_removed_headset_addr,0,UAL_BT_DEV_ADDRESS_SIZE);
                    SCI_MEMCPY(g_removed_headset_addr,paired_dev.addr,UAL_BT_DEV_ADDRESS_SIZE);
                }
            }
            else
            {
                TRACE_APP_BT("HandlePairedDeviceAction is not audio dev");
                //To do delete pair
                del_ret = ual_bt_dev_delete_paired_dev(paired_dev.addr);
            }
        }
            break;
        case MSG_NOTIFY_OK:
        {
            TRACE_APP_BT("MSG_NOTIFY_OK");
            MMK_CloseWin(win_id);
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
//  Description  : load the data of bound paired devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
#ifdef UAL_BT_HFU_SUPPORT
LOCAL void AppendBoundItemsToList(MMI_CTRL_ID_T list_ctrl_id)
{
    TRACE_APP_BT("enter.");
    while (1)
    {
        uint32 service_type = UAL_BT_SERVICE_ALL;
        uint32 i = 0;
        uint32 paired_items_cnt = ual_bt_dev_get_paired_dev_count(service_type);
        MMI_STRING_T bt_status_str = {0};
        uint32 user_data = 0;
        uint8  bt_paired_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};

        MMI_GetLabelTextByLang( STXT_PAIRED, &bt_status_str);
        TRACE_APP_BT("paired_items_cnt = %d.", paired_items_cnt);

        WATCHBT_GetBoundDevAddr(bt_paired_addr);

        for (i = 0; i < paired_items_cnt; i++)
        {
            ual_bt_dev_info_t dev_info = {0};
            MMI_IMAGE_ID_T    dev_icon = 0;
            MMI_STRING_T      dev_name = {0};
            uint16          list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);
            
            //the index of device begin at 1.
            if (UAL_BT_STATUS_SUCCESS != ual_bt_dev_get_pair_info(service_type, i+1, &dev_info))
            {
                TRACE_APP_BT("invoke BT_GetPairedDeviceInfo error. id = %d.", i);
                continue;
            }
             if(ual_bt_hfu_get_connect_status()) //fix bug1592080
             {
                 MMI_GetLabelTextByLang( TXT_COMMON_CONNECTED, &bt_status_str);
             }

            if(0 == memcmp(bt_paired_addr, dev_info.addr, UAL_BT_DEV_ADDRESS_SIZE))
            {
                TRACE_APP_BT("is dialer bound device.");
                dev_icon = GetDevIconByDevInfo(&dev_info);

                dev_name.wstr_ptr = dev_info.name;
                dev_name.wstr_len = MMIAPICOM_Wstrlen(dev_name.wstr_ptr);
                AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line(list_ctrl_id, dev_icon, dev_name, bt_status_str);
            }
        }

        //must break at this point
        break;
    }
    TRACE_APP_BT("exit.");
}
#endif
/*****************************************************************************/
//  Description  : load the data of paired devices, and redraw it on ui
//  Paramter:
//          [In]:list_ctrl_id ------ list ctrl
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void AppendPairedItemsToList(MMI_CTRL_ID_T list_ctrl_id)
{
    TRACE_APP_BT("enter.");
    while (1)
    {
        uint32 service_type = UAL_BT_SERVICE_ALL;
        uint32 i = 0;
        uint32 paired_items_cnt = ual_bt_dev_get_paired_dev_count(service_type);
        MMI_STRING_T bt_status_str = {0};
        uint32 user_data = 0;
        uint8   paired_device_index[UAL_BT_DEV_MAX_COUNT] = {0};
#ifdef UAL_BT_HFU_SUPPORT
        uint8  bt_bound_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
#endif

        MMI_GetLabelTextByLang( STXT_PAIRED, &bt_status_str);
        TRACE_APP_BT("paired_items_cnt = %d.", paired_items_cnt);

#ifdef UAL_BT_HFU_SUPPORT
#ifdef BBM_ROLE_SUPPORT
        Bbm_GetBTBoundDevAddr(bt_bound_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);
#else
        WATCHBT_GetBoundDevAddr(bt_bound_addr);
#endif
#endif
        for (i = 1; i <= paired_items_cnt; i++)
        {
            ual_bt_dev_info_t dev_info = {0};
            MMI_IMAGE_ID_T    dev_icon = 0;
            MMI_STRING_T      dev_name = {0};
            uint16            list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);
            uint16            dev_type = 0;
            bt_list_item_data_t item_data_info = {0};

            //the index of device begin at 1.
            if (UAL_BT_STATUS_SUCCESS != ual_bt_dev_get_pair_info(service_type, i, &dev_info))
            {
                TRACE_APP_BT("invoke BT_GetPairedDeviceInfo error. id = %d.", i);
                continue;
            }


            dev_icon = GetDevIconByDevInfo(&dev_info);

            dev_name.wstr_ptr = dev_info.name;
            dev_name.wstr_len = MMIAPICOM_Wstrlen(dev_name.wstr_ptr);

            ual_bt_dev_get_dev_type(&dev_info, &dev_type);

            if((dev_type & BT_DEVICE_CLASS_AUDIO) && MMIBT_DeviceIsConnectedHeadsetEx(dev_info.addr))//fix bug1592084
            {
                TRACE_APP_BT("connect.");
                MMI_GetLabelTextByLang(TXT_COMMON_CONNECTED, &bt_status_str);
            }

#ifdef UAL_BT_HFU_SUPPORT
            if(UAL_BT_PHONE == ual_bt_get_product_role())
            {
                if(0 == memcmp(bt_bound_addr, dev_info.addr, UAL_BT_DEV_ADDRESS_SIZE))
                {
                    TRACE_APP_BT("is dialer bound device.");

                    item_data_info.type = BT_LIST_TYPE_PAIRED;  //bug1606731
                    item_data_info.fun_cb = 0;
                    item_data_info.index= list_index + BT_MAIN_LIST_PERMANENT_ITEM_NUM;
                    s_list_data[s_list_item_num] = item_data_info;
                    user_data = &s_list_data[s_list_item_num];
                    GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);
                    s_list_item_num++;
                    AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line(list_ctrl_id, dev_icon, dev_name, bt_status_str);//bug1586597
                    GUILIST_SetItemGray(list_ctrl_id,list_index,TRUE);
                    continue;
                }
            }
#endif
            AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line(list_ctrl_id, dev_icon, dev_name, bt_status_str);

            //to do.paired callback
            item_data_info.type = BT_LIST_TYPE_PAIRED;
            item_data_info.fun_cb = (uint32)HandlePairedDeviceAction;
            item_data_info.index= list_index + BT_MAIN_LIST_PERMANENT_ITEM_NUM;
            s_list_data[s_list_item_num] = item_data_info;
            user_data = &s_list_data[s_list_item_num];
            GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);
            s_list_item_num++;

        }
        
        //must break at this point
        break;
    }
    TRACE_APP_BT("exit, s_list_item_num=%d.",s_list_item_num);
}

/*****************************************************************************/
//  Description  : ClosePairWin
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void ClosePairWin( void )
{
    TRACE_APP_BT("enter");

    if(MMK_IsOpenWin(ADULTWATCH_BT_PAIR_WAITING_WIN_ID))
    {
        MMK_CloseWin(ADULTWATCH_BT_PAIR_WAITING_WIN_ID);
    }

    if(MMK_IsOpenWin(ADULTWATCH_BT_PASSKEY_WIN_ID))
    {
        MMK_CloseWin(ADULTWATCH_BT_PASSKEY_WIN_ID);
    }
}

/*****************************************************************************/
//  Description: CloseToPairWaitingWin
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:配对成功后，关闭waiting窗口
/*****************************************************************************/
LOCAL void CloseToPairWaitingWin( void )
{
    TRACE_APP_BT("enter");

    if(MMK_IsOpenWin(ADULTWATCH_BT_PAIR_REQUEST_WIN_ID))
    {
        MMK_CloseWin(ADULTWATCH_BT_PAIR_REQUEST_WIN_ID);
    }
}

/*****************************************************************************/
//  Description:the win produrce of pin input mode
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
    BT_STATUS  return_value = BT_SUCCESS;

    TRACE_APP_BT("MSG ID =%x",msg_id);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        case MSG_APP_WEB:
        {
            MMI_STRING_T wait_text = {0};
            TRACE_APP_BT("MSG_NOTIFY_CANCEL");
            TRACE_APP_BT("pin addr is %d,%d,%d,%d,%d,%d",g_pin_request_info.addr[0],g_pin_request_info.addr[1],g_pin_request_info.addr[2],
                                                         g_pin_request_info.addr[3],g_pin_request_info.addr[4],g_pin_request_info.addr[5]);
            return_value = ual_bt_dev_input_pinpair(g_pin_request_info.addr,(uint8*)BT_DEFAULT_PIN_CODE,strlen(BT_DEFAULT_PIN_CODE));

            if (UAL_BT_STATUS_PENDING == return_value)
            {
                MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
                //close pin win
                if(MMK_IsOpenWin(ADULTWATCH_BT_PASSKEY_WIN_ID))
                {
                    MMK_CloseWin(ADULTWATCH_BT_PASSKEY_WIN_ID);
                }
                AdultWatchCOM_WaitingWin_1Icon_1Str_Enter(ADULTWATCH_BT_PAIR_WAITING_WIN_ID,&wait_text,PNULL,PNULL);
            }
            else
            {
                 MMK_CloseWin(win_id);
            }
        }
            break;
        case MSG_NOTIFY_OK:
        {
            TRACE_APP_BT("MSG_NOTIFY_OK");
            MMK_CloseWin(win_id);
            ual_bt_dev_cancel_pair(g_pin_request_info.addr);
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
//  Description:open pin mode query win.
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void OpenPinPairQueryWin(ual_bt_dev_info_t device_info)
{
    MMI_STRING_T   txt_pair = {0};
    MMI_STRING_T   txt_tip  = {0};
    wchar      wstr_tip[50] = {0};
    wchar        wstr_pwd[UAL_BT_DEV_SSP_NUM_MAX_LEN] = {0};

    MMI_IMAGE_ID_T  img_cancel = IMAGE_SET_COM_AW_BTN_CANCEL;
    MMI_IMAGE_ID_T  img_ok     = IMAGE_SET_COM_AW_BTN_CONFIRM;

    TRACE_APP_BT("enter.");

    SCI_MEMSET(&g_pin_request_info, 0, sizeof(ual_bt_dev_info_t));
    SCI_MEMCPY(&g_pin_request_info, &device_info,sizeof(ual_bt_dev_info_t));

    MMI_GetLabelTextByLang(STXT_ISPAIRED,&txt_pair);

    MMI_STRNTOWSTR(wstr_pwd,UAL_BT_DEV_SSP_NUM_MAX_LEN, BT_DEFAULT_PIN_CODE,UAL_BT_DEV_SSP_NUM_MAX_LEN,UAL_BT_DEV_SSP_NUM_MAX_LEN);

    MMIAPICOM_Wstrncat(wstr_tip, txt_pair.wstr_ptr, txt_pair.wstr_len);//paired:
    MMIAPICOM_Wstrncat(wstr_tip, wstr_pwd, MMIAPICOM_Wstrlen(wstr_pwd));//1234

    txt_tip.wstr_ptr = wstr_tip;
    txt_tip.wstr_len = MMIAPICOM_Wstrlen(wstr_tip);

    TRACE_APP_BT("ssp addr is %d,%d,%d,%d,%d,%d",device_info.addr[0],device_info.addr[1],device_info.addr[2],device_info.addr[3],device_info.addr[4],device_info.addr[5]);
    AdultWatchCOM_QueryWin_2Btn_Enter(ADULTWATCH_BT_PASSKEY_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,HandlePinPairQueryWin);
    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description : the win produrce of ssp mode
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSspPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
    BT_STATUS  return_value = BT_SUCCESS;
    TRACE_APP_BT("MSG ID =%x",msg_id);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        case MSG_APP_WEB:
        {
            MMI_STRING_T wait_text = {0};
            TRACE_APP_BT("MSG_NOTIFY_CANCEL");
            TRACE_APP_BT("add=%x,%x,%x,%x,%x,%x",g_ssp_request_info_ext.addr[0],g_ssp_request_info_ext.addr[1],g_ssp_request_info_ext.addr[2],
                                              g_ssp_request_info_ext.addr[3],g_ssp_request_info_ext.addr[4],g_ssp_request_info_ext.addr[5]);
            return_value = ual_bt_dev_input_pinpair(g_ssp_request_info_ext.addr,(uint8*)g_ssp_request_info_ext.ssp_numric_key,strlen(g_ssp_request_info_ext.ssp_numric_key));

            if (UAL_BT_STATUS_PENDING == return_value)
            {
                MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
                //close passky win
                if(MMK_IsOpenWin(ADULTWATCH_BT_PASSKEY_WIN_ID))
                {
                    MMK_CloseWin(ADULTWATCH_BT_PASSKEY_WIN_ID);
                }
                AdultWatchCOM_WaitingWin_1Icon_1Str_Enter(ADULTWATCH_BT_PAIR_WAITING_WIN_ID,&wait_text,PNULL,PNULL);
            }
            else
            {
                 MMK_CloseWin(win_id);
            }
        }
            break;
        case MSG_APP_OK:
        case MSG_APP_MENU:
        case MSG_NOTIFY_OK:
        {
            TRACE_APP_BT("MSG_NOTIFY_OK");
            MMK_CloseWin(win_id);
            ual_bt_dev_cancel_pair(g_ssp_request_info_ext.addr);
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
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void OpenSspPairQueryWin(ual_bt_dev_info_ssp_t device_info)
{
    MMI_STRING_T txt_pair = {0};
    MMI_STRING_T txt_tip  = {0};
    wchar        wstr_pwd[UAL_BT_DEV_SSP_NUM_MAX_LEN] = {0};
    wchar      wstr_tip[50] = {0};

    MMI_IMAGE_ID_T  img_cancel = IMAGE_SET_COM_AW_BTN_CANCEL;
    MMI_IMAGE_ID_T  img_ok     = IMAGE_SET_COM_AW_BTN_CONFIRM;

    TRACE_APP_BT("enter.");

    SCI_MEMSET(&g_ssp_request_info_ext, 0, sizeof(ual_bt_dev_info_ssp_t));
    SCI_MEMCPY(&g_ssp_request_info_ext, &device_info,sizeof(ual_bt_dev_info_ssp_t));

    MMI_GetLabelTextByLang(STXT_ISPAIRED,&txt_pair);

    MMI_STRNTOWSTR(wstr_pwd,UAL_BT_DEV_SSP_NUM_MAX_LEN, g_ssp_request_info_ext.ssp_numric_key,UAL_BT_DEV_SSP_NUM_MAX_LEN,UAL_BT_DEV_SSP_NUM_MAX_LEN);
    MMIAPICOM_Wstrncat(wstr_tip, txt_pair.wstr_ptr, txt_pair.wstr_len);
    MMIAPICOM_Wstrncat(wstr_tip, wstr_pwd, MMIAPICOM_Wstrlen(wstr_pwd));

    txt_tip.wstr_ptr = wstr_tip;
    txt_tip.wstr_len = MMIAPICOM_Wstrlen(wstr_tip);

    TRACE_APP_BT("ssp addr is %d,%d,%d,%d,%d,%d",device_info.addr[0],device_info.addr[1],device_info.addr[2],device_info.addr[3],device_info.addr[4],device_info.addr[5]);
    AdultWatchCOM_QueryWin_2Btn_Enter(ADULTWATCH_BT_PASSKEY_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,HandleSspPairQueryWin);
    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description:produrce of Bt Main Window
//  Paramter:
//          [In]:win_id msg_id param
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdultWatchBTMainWindow(
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
            HandleMsgOpenWin(win_id);
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_BT("MSG_CLOSE_WINDOW");
            HandleMsgCloseWin(win_id);
            break;
        }
        case MSG_FULL_PAINT:
        {
            TRACE_APP_BT("MSG_FULL_PAINT");
            HandleMsgFullPaint(win_id);
            break;
        }
        case MSG_KEYDOWN_USER_FUNC:
       {
            MMK_SendMsg(win_id,MSG_APP_DOWN , PNULL);
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
            HandleMsgAppOK(win_id);
        }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            TRACE_APP_BT("MSG_APP_CANCEL");
            HandleMsgAppCancel(win_id);
            break;
        }
        default:
            result =  MMI_RESULT_FALSE;
            break;
    }

    return result;
}


/*win table*/
WINDOW_TABLE( ADULTWATCH_BT_MAIN_WIN_TAB ) =
{
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),
    WIN_FUNC( (uint32)HandleAdultWatchBTMainWindow ),
    WIN_ID( ADULTWATCH_BT_MAIN_WIN_ID ),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID ),
    END_WIN
};

/*---------------------------------------------------------------------------*
**                          PUBLIC FUNCTION DEFINITION                        *
**---------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : enter adult bluetooth main window
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:
/****************************************************************************/
PUBLIC void AdultBT_MainWin_Enter( void )
{
    MMK_CreateWin( (uint32*)ADULTWATCH_BT_MAIN_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : BT_HandleDevMsg
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:处理ual返回ui的消息
/****************************************************************************/
PUBLIC BOOLEAN BT_HandleDevMsg(ual_cms_msg_t param)
{
    BOOLEAN ret = TRUE;

    MMI_STRING_T            txtSearch = {0};

    TRACE_APP_BT("msg_id =%x",param.msg_id);

    switch(param.msg_id)
    {
        case UAL_APP_BT_DEV_OPENED:
        {
            TRACE_APP_BT("UAL_BT_DEV_VIEW_OPENED");
            // UAL TODO :set roam status & operator name
#ifdef BT_BQB_SUPPORT
            MMIAPIBT_SetNetWorkRoamStatus(MMIAPIPHONE_GetIsNetworkRoaming());
            MMIAPIBT_SetOperatorName();	

            MMIAPIBT_SetMyPhoneNumber();
#endif
#ifdef UAL_BT_HFU_SUPPORT
            //auto connect hfu
            if(UAL_BT_DIALER == ual_bt_get_product_role())
            {
                //
#ifdef BBM_ROLE_SUPPORT
                WATCHBT_ConnectBoundDialerDev();
#else
                WATCHBT_ConnectBoundDev();
#endif
                // TODO: create timer 连接失败后，多次进行连接
            }
#endif
           if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                MMK_SendMsg(ADULTWATCH_BT_MAIN_WIN_ID,MSG_FULL_PAINT,PNULL);

                if(UAL_BT_PHONE == ual_bt_get_product_role())//is not slaver mode
                {
                    TRACE_APP_BT("phone mode,to search");
                    if(ual_bt_dev_get_is_on())
                    {
                        ual_bt_dev_start_search(UAL_BT_SERVICE_ALL);
                        MMI_GetLabelTextByLang(TXT_COMMON_SEARCHING,&txtSearch);
                        AdultWatchCOM_WaitingWin_1Line_Enter(ADULTWATCH_BT_SEARCH_WAIT_WIN_ID,&txtSearch,0,PNULL);
                    }
                }
                else
                {
                    TRACE_APP_BT("dialer mode,not to search");
                }
            }
        }
            break;
        case UAL_APP_BT_DEV_CLOSED:
        {
            TRACE_APP_BT("UAL_APP_BT_DEV_CLOSED");
            if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                MMK_SendMsg(ADULTWATCH_BT_MAIN_WIN_ID,MSG_FULL_PAINT,PNULL);
            }
        }
            break;
        case UAL_APP_BT_DEV_OPEN_ERR:
            TRACE_APP_BT("UAL_APP_BT_DEV_OPEN_ERR");
            PopUpPromptMsg(TXT_COMM_ERROR);
            break;
        case UAL_APP_BT_DEV_CLOSE_ERR:
            TRACE_APP_BT("UAL_APP_BT_DEV_CLOSE_ERR");
            PopUpPromptMsg(TXT_COMM_ERROR);
            break;
        case UAL_APP_BT_DEV_SEARCH_ONE:
        {
            TRACE_APP_BT("UAL_APP_BT_DEV_SEARCH_ONE");
            if(MMK_IsOpenWin(ADULTWATCH_BT_SEARCH_WAIT_WIN_ID))
            {
                MMK_CloseWin(ADULTWATCH_BT_SEARCH_WAIT_WIN_ID);
            }
            if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                MMK_SendMsg(ADULTWATCH_BT_MAIN_WIN_ID,MSG_FULL_PAINT,PNULL);
            }
        }
            break;
        case UAL_APP_BT_DEV_SEARCH_COMPLETE:
        {
            TRACE_APP_BT("UAL_APP_BT_DEV_SEARCH_COMPLETE");
            if(MMK_IsOpenWin(ADULTWATCH_BT_SEARCH_WAIT_WIN_ID))
            {
                MMK_CloseWin(ADULTWATCH_BT_SEARCH_WAIT_WIN_ID);
            }
            if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                PopUpPromptMsg(TXT_BT_SEARCH_COM);//搜索完毕
            }
        }
            break;
        case UAL_APP_BT_DEV_CANCEL_SEARCHED:
        {
            TRACE_APP_BT("UAL_APP_BT_DEV_CANCEL_SEARCHED");
            TRACE_APP_BT("g_searching_flag=%d",g_searching_flag);//如果搜索过程中去配对，先取消搜索，再进行配对
            if(TRUE == g_searching_flag)
            {
                if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
                {
                    MMK_SendMsg(ADULTWATCH_BT_MAIN_WIN_ID,MSG_APP_WEB,PNULL);
                }
            }
            else
            {
                PopUpPromptMsg(TXT_BT_SEARCH_COM);//搜索完毕
            }
        }
            break;
        case UAL_APP_BT_DEV_SEARCH_NONE:
        {
            TRACE_APP_BT("UAL_APP_BT_DEV_SEARCH_NONE");
            PopUpPromptMsg(TXT_BT_NO_DEVICE);//没有搜索到设备
        }
            break;
        case UAL_APP_BT_DEV_SEARCH_COMPLETE_ERR:
            TRACE_APP_BT("UAL_APP_BT_DEV_SEARCH_COMPLETE_ERR");
            PopUpPromptMsg(TXT_COMM_ERROR);//搜索出错
            break;
        case UAL_APP_BT_DEV_PAIR_PIN_INPUT:
        {
            ual_bt_dev_info_t dev_info = {0};
            uint16 name_len = 0;
            TRACE_APP_BT("UAL_APP_BT_DEV_PAIR_PIN_INPUT");
            TRACE_APP_BT("param.p_body=%x,param.body_size=%d",param.p_body,param.body_size);

            CloseToPairWaitingWin();//关闭wating窗口

            if(PNULL == param.p_body || 0 == param.body_size)
            {
                TRACE_APP_BT("param error,param.p_body=%x,param.body_size=%d",param.p_body,param.body_size);
                PopUpPromptMsg(TXT_COMM_ERROR);//提示错误
            }
            else
            { 
                dev_info = *(( ual_bt_dev_info_t *) param.p_body);
                MMIDEFAULT_TurnOnBackLight();//1592456

                //OpenPinPairQueryWin(dev_info);
                AdultWatchBT_Open_Pininput_Win(dev_info);

                if(UAL_BT_DIALER == ual_bt_get_product_role())//在Dialer模式下，才会去保存BT name
                {
                    name_len = MMIAPICOM_Wstrlen(dev_info.name);
                    TRACE_APP_BT("bt dialer mode,save BT name,name len = %d",name_len);
#ifdef BBM_ROLE_SUPPORT
                    Bbm_SetBTBoundDevName(dev_info.name,name_len);
#endif
                }
            }
        }
            break;
        case UAL_APP_BT_DEV_PAIR_SSP_NUM:
        {
            ual_bt_dev_info_ssp_t dev_info = {0};

            uint16 name_len = 0;
            TRACE_APP_BT("UAL_APP_BT_DEV_PAIR_SSP_NUM");
            TRACE_APP_BT("param.p_body=%x,param.body_size=%d",param.p_body,param.body_size);

            CloseToPairWaitingWin();//关闭wating窗口

            if(PNULL == param.p_body || 0 == param.body_size)
            {
                TRACE_APP_BT("param error");
                PopUpPromptMsg(TXT_COMM_ERROR);//提示错误
            }
            else
            {
                dev_info = *(( ual_bt_dev_info_ssp_t *) param.p_body);
                MMIDEFAULT_TurnOnBackLight();//1592456
                OpenSspPairQueryWin(dev_info);

                if(UAL_BT_DIALER == ual_bt_get_product_role())//在Dialer模式下，才会去保存BT name
                {
                    name_len = MMIAPICOM_Wstrlen(dev_info.name);
                    TRACE_APP_BT("bt dialer mode,save BT name,name len = %d",name_len);
#ifdef BBM_ROLE_SUPPORT
                    Bbm_SetBTBoundDevName(dev_info.name,name_len);
#endif
                }
            }
        }
            break;
        case UAL_APP_BT_DEV_PAIRED:
        {
            TRACE_APP_BT("UAL_APP_BT_DEV_PAIRED");
            TRACE_APP_BT("param.p_body=%x,param.body_size=%d",param.p_body,param.body_size);

            ClosePairWin();

            CloseToPairWaitingWin();

            if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                MMK_SendMsg(ADULTWATCH_BT_MAIN_WIN_ID,MSG_FULL_PAINT,PNULL);
            }

            //如果配对的设备为耳机，要主动发起连接
            do
            {
                uint16 dev_type = 0;
                ual_bt_dev_get_dev_type(&g_pair_req_dev, &dev_type);
                TRACE_APP_COMMON("UAL_APP_BT_DEV_PAIRED device type = %d",dev_type);
                if(dev_type & BT_DEVICE_CLASS_AUDIO)
                {
                    //headset need open it directly
                    TRACE_APP_BT("UAL_APP_BT_DEV_PAIRED audio type.");

                    TRACE_APP_BT("g_pair_req_dev addr %d,%d,%d,%d,%d,%d",g_pair_req_dev.addr[0],g_pair_req_dev.addr[1],g_pair_req_dev.addr[2],
                                                                        g_pair_req_dev.addr[3],g_pair_req_dev.addr[4],g_pair_req_dev.addr[5]);
                    if(HeadsetOpenIsPermitEx(g_pair_req_dev.addr))
                    {
                        OpenHeadsetEx(g_pair_req_dev.addr);
                    }
                }
            }while(0);
        }
            break;
        case UAL_APP_BT_DEV_PAIR_IND:
        {
            ual_bt_dev_info_t dev_info = {0};
            TRACE_APP_BT("UAL_APP_BT_DEV_PAIR_IND");
            TRACE_APP_BT("param.p_body=%x,param.body_size=%d",param.p_body,param.body_size);
            dev_info = *(( ual_bt_dev_info_t *) param.p_body);

            ClosePairWin();

            if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                MMK_SendMsg(ADULTWATCH_BT_MAIN_WIN_ID,MSG_FULL_PAINT,PNULL);
            }
            TRACE_APP_BT("dev_info.addr=%x,%x,%x,%x,%x,%x",dev_info.addr[0],dev_info.addr[1],dev_info.addr[2],
                                                           dev_info.addr[3],dev_info.addr[4],dev_info.addr[5]);

#ifdef BBM_ROLE_SUPPORT
#ifdef UAL_BT_HFU_SUPPORT
            if(UAL_BT_DIALER == ual_bt_get_product_role())//在Dialer模式下，才会去保存BT addr
            {
                TRACE_APP_BT("dev_info.addr=%x,%x,%x,%x,%x,%x",dev_info.addr[0],dev_info.addr[1],dev_info.addr[2],
                                   dev_info.addr[3],dev_info.addr[4],dev_info.addr[5]);
                TRACE_APP_BT("bt dialer mode,save BT addr,addr len =%d",strlen(dev_info.addr));
                Bbm_SetBTBoundDevAddr(dev_info.addr,TRUE);
            }
            //如果窗口打开，UI显示名称和地址
            BTPair_FreshWin();
#endif
#endif
        }
            break;
        case UAL_APP_BT_DEV_PAIR_ERR:
        {
            TRACE_APP_BT("UAL_APP_BT_DEV_PAIR_ERR");
            CloseToPairWaitingWin();
            ClosePairWin();
            PopUpPromptMsg(TXT_BT_PAIR_FAIL);
        }
            break;
        case UAL_APP_BT_DEV_DELETED:
            TRACE_APP_BT("UAL_APP_BT_DEV_DELETED");
            //if dialer mode,to remove remote bt addr from nv
            if(UAL_BT_DIALER == ual_bt_get_product_role())
            {
                uint8 dev_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
                TRACE_APP_BT("clear bt addr");
#ifdef BBM_ROLE_SUPPORT
                Bbm_ClearBTBoundDevAddr(TRUE);
#else
                WATCHBT_SetBoundDevAddr(dev_addr);
#endif
            }
            //PHONE mode下，关闭窗口
            if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                MMK_CloseWin(ADULTWATCH_BT_MAIN_WIN_ID);
            }
#ifdef BBM_ROLE_SUPPPORT
            //DIALER mode下，关闭窗口
            BTPair_FreshWin();
#endif
            break;
        case UAL_APP_BT_DEV_DELETE_ERR:
            TRACE_APP_BT("UAL_APP_BT_DEV_DELETE_ERR");
            PopUpPromptMsg(TXT_COMM_ERROR);//取消配对失败
            break;
        default:
            TRACE_APP_BT("error msg id error");
            ret = FALSE;
            break;
    }

    return ret;

}
#ifdef UAL_BT_HFU_SUPPORT
/****************************************************************************/
//  Description : WATCHBT_GetConnectDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取绑定设备地址
/****************************************************************************/
PUBLIC void WATCHBT_GetBoundDevAddr(uint8 *p_conn_addr )
{
    MN_RETURN_RESULT_E                    return_value = MN_RETURN_FAILURE;
    uint8            dev_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};

    if(PNULL == p_conn_addr)
    {
        TRACE_APP_BT("p_conn_addr is null");
        return;
    }

    MMINV_READ(MMINV_BT_BOUND_ADDR,dev_addr,return_value);
    TRACE_APP_BT("return_value=%d",return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(dev_addr,0,UAL_BT_DEV_ADDRESS_SIZE); 
        MMINV_WRITE(MMINV_BT_BOUND_ADDR,dev_addr);
    }

    SCI_MEMCPY(p_conn_addr,dev_addr,UAL_BT_DEV_ADDRESS_SIZE);
    TRACE_APP_BT("bound addr=%d,%d,%d,%d,%d,%d",p_conn_addr[0],p_conn_addr[1],p_conn_addr[2],
                                                p_conn_addr[3],p_conn_addr[4],p_conn_addr[5]);
}

/****************************************************************************/
//  Description : WATCHBT_SetConnectDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置绑定设备的地址，第一次配对成功后，开始写
/****************************************************************************/
PUBLIC void WATCHBT_SetBoundDevAddr(uint8 *p_conn_addr )
{
    if(PNULL == p_conn_addr)
    {
        TRACE_APP_BT("p_conn_addr is null");
        return;
    }

    MMINV_WRITE(MMINV_BT_BOUND_ADDR,p_conn_addr);
}
/****************************************************************************/
//  Description : WATCHBT_HFUConnect
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:hfu connect，当前hfu只能连接一个
/****************************************************************************/
LOCAL BOOLEAN WATCHBT_ConnectBoundDev(void)
{
    uint8 bt_bond_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
    uint8 addr_initial[UAL_BT_DEV_ADDRESS_SIZE] = {0}; // Bug 1911403 -pclint

    WATCHBT_GetBoundDevAddr(bt_bond_addr);
    if(0 == memcmp(addr_initial,bt_bond_addr,UAL_BT_DEV_ADDRESS_SIZE))
    {
        TRACE_APP_BT("bond addr is null");
        return FALSE;
    }
    TRACE_APP_BT("[dev]bond addr =%d,%d,%d,%d,%d,%d",bt_bond_addr[0],bt_bond_addr[1],bt_bond_addr[2],
                                                  bt_bond_addr[3],bt_bond_addr[4],bt_bond_addr[5]);
#ifdef UAL_BT_HFU_SUPPORT
    TRACE_APP_BT("[dev]BT_HfuGetConnStatus = %d",ual_bt_hfu_get_connect_status());
    if(TRUE == ual_bt_hfu_get_connect_status())
    {
        TRACE_APP_BT("[dev]hfu has connected,do nothing");
    }
   else
    {
        TRACE_APP_BT("[dev]to connect hfu");
        ual_bt_hfu_connect(bt_bond_addr);
    }
#endif
    return TRUE;
}
/*****************************************************************************/
//  Discription: This function is used to Get the Paired bt dev list
//  Global resource dependence: None
//  Author:chunjuan.liang
//  Note :this function is used to put bound dev in the first position
//  Return:None
/*****************************************************************************/
LOCAL uint16 AdultWatchBT_GetBoundDevIndex(void)
{
    uint16                   i = 0;
    uint16       device_number = 0;
    ual_bt_status_e     status = UAL_BT_STATUS_ERROR;
    ual_bt_dev_info_t dev_info = {0};
    uint32             service = UAL_BT_SERVICE_ALL;

    uint16          bound_index = 0;

    uint32 ual_bt_dev_get_paired = 0;

    uint8 bond_dev_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};

    WATCHBT_GetBoundDevAddr(bond_dev_addr);

    ual_bt_dev_get_paired=ual_bt_dev_get_paired_dev_count(service);
    device_number = MIN(ual_bt_dev_get_paired, UAL_BT_DEV_MAX_COUNT);

    for (i = 1; i <= device_number; i++)
    {
        SCI_MEMSET(&dev_info, 0x00, sizeof(ual_bt_dev_info_t));
        status = ual_bt_dev_get_pair_info(service, i, &dev_info);
        TRACE_APP_BT("i = %d",i);
        if(UAL_BT_STATUS_SUCCESS != status)
        {
            continue;
        }
        if(!memcmp(bond_dev_addr,dev_info.addr,UAL_BT_DEV_ADDRESS_SIZE))
        {
            bound_index = i;
            break;
        }
    }
    TRACE_APP_BT("bound_index = %d",bound_index);
    return bound_index;
}
#endif
PUBLIC void AdultWatch_GetRemoveHeadsetAddr(uint8 *p_headset_addr)
{
    if(PNULL == p_headset_addr)
    {
        TRACE_APP_BT("param is null");
        return;
    }

    SCI_MEMCPY(p_headset_addr,g_removed_headset_addr,UAL_BT_DEV_ADDRESS_SIZE);
}

/****************************************************************************/
//  Description : process ps msg
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN AdultWatchBt_ProcessBtPsMsg(uint16 msg_id, const BT_MSG_T *msg_body_ptr)
{
    BOOLEAN result = FALSE;
    TRACE_APP_COMMON("enter. msg_id = %0X.", msg_id);
    if (msg_body_ptr == NULL)
    {
        TRACE_APP_COMMON("msg_body_ptr == NULL.");
        return FALSE;
    }

    switch(msg_id)
    {
        case MMI_BT_HFAG_SLC_DISCONNECTED:
        case MMI_BT_A2DP_DISCONNECTED:
        case MMI_BT_HFAG_SLC_CONNECTED:
        case MMI_BT_HFAG_SLC_CONNECTED_IND:
        case MMI_BT_A2DP_CONNECTED:
        {
            if(MMK_IsOpenWin(ADULTWATCH_BT_MAIN_WIN_ID))
            {
                //Update UI
                MMK_SendMsg(ADULTWATCH_BT_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
            }

            break;
        }
        default:
            TRACE_APP_COMMON("default.");
            break;
    }

    TRACE_APP_COMMON("exit.");
    result = TRUE;
    return result;
}


