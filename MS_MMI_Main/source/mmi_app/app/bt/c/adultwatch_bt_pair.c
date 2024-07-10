/*****************************************************************************
** File Name:      adultwatch_bt_pair.c                                      *
** Author:         chunjuan.liang                                            *
** Date:           04/20/2021                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2021      chunjuan.liang            Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guires.h"
#include "guictrl_api.h"

#include "mmicom_trace.h"
#include "mmiset_image.h"

#include "mmiset_export.h"
#include "mmibt_nv.h"
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
#include "ual_bt.h"
#include "mmi_nv.h"

#include "mmibt_func.h"
#include "mmibt_app.h"
#include "mmibt_image.h"
#include "mmibt_text.h"
#include "mmiset_export.h"
#include "mmiset_text.h"
#include "watch_common_list.h"
#include "adultwatch_bt_win.h"
#include "mmieng_export.h"
#include "watch_commonwin_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define BT_PAIR_LIST_MAX_NUM                    (3)//当前窗口中list的最大条数
#define BT_PAIR_ADDR_VIEW_MAX_LEN               (25)

#define BT_SLAVER_DISCONNECT_TIMER_OUT          (100)
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/

typedef enum 
{
    BT_PAIR_TYPE_BLE = 0,
    BT_PAIR_TYPE_DIALER,
    BT_PAIR_TYPE_PHONE,
    BT_PAIR_TYPE_MAX
}WATCH_BT_PAIR_ITEM_TYPE;

typedef struct BT_PAIR_ITEM_DATA_T
{
    WATCH_BT_PAIR_ITEM_TYPE type;
    uint32 fun_cb;
    uint16 index;
}BT_PAIR_ITEM_DATA_T;

LOCAL BT_PAIR_ITEM_DATA_T s_pair_data[BT_PAIR_LIST_MAX_NUM] = {0};//UI中每个list数据
LOCAL uint32 s_pair_item_num = 0;//list中append的数目

//pair item callback function
typedef void ( *BT_PAIR_ITEM_FUNC )( void );

LOCAL uint8  s_bt_ble_disconnect_timer    = 0;//断开ble的timer id

LOCAL uint8  s_bt_dialer_disconnect_timer = 0;//断开dialer的timer id

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTIONS                                   *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description  : HandleDialerPairQueryWin
//  Paramter:
//          [In]:win id,msg id 
//          [Out]:None
//          [Return]:result
//  Author: chunjuan.liang
//  Note:取消配对时，弹出窗体
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialerPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param );

/****************************************************************************/
/***********************Ble Reference API bdegin*****************************/
/****************************************************************************/
/*****************************************************************************/
#ifdef BBM_ROLE_SUPPORT
LOCAL BOOLEAN  Bbm_Ble_CallBack(uint32 msg_id,void *param)
{
    BOOLEAN ret_val = TRUE;
    TRACE_APP_BBM("msg_id = %d",msg_id);
    switch(msg_id)
    {
        case BBM_MSG_BT_BLE_CLEAR_BOUND_SUCESS:
        {
            TRACE_APP_BT("BBM_MSG_BLE_CLEAR_BOUND_INFO");
            if(MMK_IsOpenWin(ADULTWATCH_BT_PAIR_WIN_ID))
            {
                MMK_SendMsg(ADULTWATCH_BT_PAIR_WIN_ID,MSG_FULL_PAINT,PNULL);
            }
        }
            break;
        case BBM_MSG_BT_BLE_CONNECT_SUCESS:
        {
            TRACE_APP_BT("BBM_MSG_BT_BLE_CONNECT_SUCESS");
            if(MMK_IsOpenWin(ADULTWATCH_BT_PAIR_WIN_ID))
            {
                MMK_SendMsg(ADULTWATCH_BT_PAIR_WIN_ID,MSG_FULL_PAINT,PNULL);
            }
        }
            break;
        default:
            ret_val = FALSE;
            TRACE_APP_BT("other msg");
            break;
    }
    return ret_val;
}
/*****************************************************************************/
//  Description : BTBleCancelPair
//  Global resource dependence : none
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BTBleCancelPair(uint8 timer_id, uint32 param)
{
    TRACE_APP_BT("enter");
    if(0 != s_bt_ble_disconnect_timer)
    {
        MMK_StopTimer(s_bt_ble_disconnect_timer);
        s_bt_ble_disconnect_timer = 0;

        if(UAL_BT_BLE_IDLE == ual_bt_ble_get_current_ble_status())//if ble is in connect state,disconnect ble
        {
            TRACE_APP_BT("disconnect ble");
            ual_bt_ble_clear_bound_info();
        }
        else
        {
            TRACE_APP_BT("start timer");
            s_bt_ble_disconnect_timer = MMK_CreateTimerCallback(BT_SLAVER_DISCONNECT_TIMER_OUT, BTBleCancelPair, 0, FALSE);
        }
    }
    TRACE_APP_BT("exit");
}
/*****************************************************************************/
//  Description  : HandleBleCancelPairQueryWin
//  Paramter:
//          [In]:win id,msg id 
//          [Out]:None
//          [Return]:result
//  Author: chunjuan.liang
//  Note:取消配对时，弹出窗体
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBleCancelPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E             ret = MMI_RESULT_TRUE;
    ual_bt_status_e return_value = UAL_BT_STATUS_SUCCESS;
    uint16                   idx = GUILIST_GetCurItemIndex(MMIBT_ADULTWACHT_PAIR_LIST_CTRL_ID);
    ual_bt_status_e      del_ret = UAL_BT_STATUS_SUCCESS;

    TRACE_APP_BT("msg_id =%x",msg_id);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        case MSG_APP_WEB:
        {
            uint8  ble_addr[MAX_BLE_ADDRESS_LEN] = {0};
            TRACE_APP_BT("MSG_NOTIFY_CANCEL");

            MMK_CloseWin(win_id);
            Bbm_GetBTBoundDevAddr(ble_addr,MAX_BLE_ADDRESS_LEN,FALSE);

            TRACE_APP_BT("ble addr %x,%x,%x,%x,%x,%x",ble_addr[0],ble_addr[1],ble_addr[2],
                                                      ble_addr[3],ble_addr[4],ble_addr[5]);
            if(UAL_BT_BLE_IDLE == ual_bt_ble_get_current_ble_status())//if ble is in connect state,disconnect ble
            {
                TRACE_APP_BT("unbound ble");
                ual_bt_ble_clear_bound_info();
            }
            else
            {
                bbm_disconnect_ble();//discomnect ble
                // TODO:delete device?
                s_bt_ble_disconnect_timer = MMK_CreateTimerCallback(BT_SLAVER_DISCONNECT_TIMER_OUT, BTBleCancelPair, 0, FALSE);
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

LOCAL void HandleBlePairAction(void)
{
    MMI_STRING_T         txt_tip = {0};

    MMI_IMAGE_ID_T  img_cancel = IMAGE_SET_COM_AW_BTN_CANCEL;
    MMI_IMAGE_ID_T  img_ok     = IMAGE_SET_COM_AW_BTN_CONFIRM;

    uint8 bound_dev_addr[MAX_BLE_ADDRESS_LEN] = {0};
    BBM_BOUND_REMOTE_DEV_INFO_T  bound_dev_info =  {0};

    uint8       str_addr[MAX_BLE_ADDRESS_LEN] = {0};
    uint16   addr_len = 0;
    // TODO: bbm switch to bt dialer
    BbmSwitchToBleMode();
    Bbm_GetBTBoundDevInfo(&bound_dev_info);

    addr_len = sizeof(bound_dev_info.ble_addr);

    TRACE_APP_BT("addr_len = %d,addr=%x,%x,%x,%x,%x,%x",addr_len,bound_dev_info.ble_addr[0],bound_dev_info.ble_addr[1],bound_dev_info.ble_addr[2],
                                                                 bound_dev_info.ble_addr[3],bound_dev_info.ble_addr[4],bound_dev_info.ble_addr[5]);

    SCI_MEMSET(str_addr,0,MAX_BLE_ADDRESS_LEN);

    if(0 == memcmp(str_addr,bound_dev_info.ble_addr,MAX_BLE_ADDRESS_LEN))
    {
        TRACE_APP_BT("request pair");
        MMI_GetLabelTextByLang(STXT_PAIR, &txt_tip);
        AdultWatchCOM_QueryWin_2Btn_Enter(ADULTWATCH_BBM_PAIR_REQUEST_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,HandleDialerPairQueryWin);
    }
    else
    {
        TRACE_APP_BT("cancel pair");
        MMI_GetLabelTextByLang(TXT_BT_CANCEL_PAIR, &txt_tip);
        AdultWatchCOM_QueryWin_2Btn_Enter(ADULTWATCH_BT_CANCEL_PAIR_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,HandleBleCancelPairQueryWin);
    }
}

LOCAL void AppendBlePairItem(MMI_CTRL_ID_T list_ctrl_id)
{
    MMI_STRING_T ble_name = {0};
    MMI_STRING_T pair_status = {0};
    MMI_STRING_T dev_name = {0};
    MMI_STRING_T dev_addr = {0};
    uint16       wstr_addr[BT_PAIR_ADDR_VIEW_MAX_LEN] = {0};
    uint8        str_addr[BT_PAIR_ADDR_VIEW_MAX_LEN] = {0};
    uint16       addr_len = 0;

    BT_PAIR_ITEM_DATA_T pair_data_info = {0};
    uint16 list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);
    uint32 user_data = 0;

    BBM_BOUND_REMOTE_DEV_INFO_T bound_dev_info = {0};

    Bbm_GetBTBoundDevInfo(&bound_dev_info);
    
    MMI_GetLabelTextByLang(TXT_SET_BT_ROLE_BLE,&ble_name);

    addr_len = SCI_STRLEN(bound_dev_info.ble_addr);
    TRACE_APP_BT("ble addr len =%d,ble addr=%x,%x,%x,%x,%x,%x",addr_len,bound_dev_info.ble_addr[5],bound_dev_info.ble_addr[4],bound_dev_info.ble_addr[3],
                                                                     bound_dev_info.ble_addr[2],bound_dev_info.ble_addr[1],bound_dev_info.ble_addr[0]);

    dev_name.wstr_ptr = bound_dev_info.bt_name;
    dev_name.wstr_len = MMIAPICOM_Wstrlen(dev_name.wstr_ptr);
    TRACE_APP_BT("ble name len = %d",MMIAPICOM_Wstrlen(dev_name.wstr_ptr));
    
    sprintf(str_addr, "%02x:%02x:%02x:%02x:%02x:%02x",bound_dev_info.ble_addr[5],bound_dev_info.ble_addr[4],bound_dev_info.ble_addr[3],
                                                      bound_dev_info.ble_addr[2],bound_dev_info.ble_addr[1],bound_dev_info.ble_addr[0]);

    TRACE_APP_BT("str addr len= %d",SCI_STRLEN(str_addr));
    
    MMI_STRNTOWSTR(wstr_addr,BT_PAIR_ADDR_VIEW_MAX_LEN,str_addr,BT_PAIR_ADDR_VIEW_MAX_LEN,strlen(str_addr));
    dev_addr.wstr_ptr = wstr_addr;
    dev_addr.wstr_len = MMIAPICOM_Wstrlen(dev_addr.wstr_ptr);
    TRACE_APP_BT("wstr addr len= %d",dev_addr.wstr_len);
    SCI_MEMSET(str_addr,0,BT_PAIR_ADDR_VIEW_MAX_LEN);
    if(0 == memcmp(str_addr,bound_dev_info.ble_addr,addr_len))
    {
        SCI_MEMSET(&dev_name,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&dev_addr,0,sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_BT_UNPAIR,&pair_status);
    }
    else
    {
        MMI_GetLabelTextByLang(STXT_PAIRED,&pair_status);
    }

    AdultWatchCOM_ListItem_Draw_4Str_3line(list_ctrl_id,ble_name,pair_status,dev_name,dev_addr);

    pair_data_info.type = BT_PAIR_TYPE_BLE;
    pair_data_info.fun_cb = (uint32)HandleBlePairAction;
    s_pair_data[s_pair_item_num] = pair_data_info;
    user_data = (uint32)&s_pair_data[s_pair_item_num];
    GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);

    s_pair_item_num++;

    TRACE_APP_BT("exit s_pair_item_num=%d.",s_pair_item_num);
}
/****************************************************************************/
/***********************Ble Reference API end********************************/
/****************************************************************************/

/****************************************************************************/
/***********************Dialer Reference API begin***************************/
/****************************************************************************/
#ifdef UAL_BT_HFU_SUPPORT
/****************************************************************************/
//  Description : WATCHBT_HFUConnect
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:hfu connect，当前hfu只能连接一个
/****************************************************************************/
PUBLIC BOOLEAN WATCHBT_ConnectBoundDialerDev(void)
{
    uint8 bt_bond_dialer_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
    uint8 bt_addr_null[UAL_BT_DEV_ADDRESS_SIZE] = {0};

    Bbm_GetBTBoundDevAddr(bt_bond_dialer_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);
    if(0 == memcmp(bt_addr_null,bt_bond_dialer_addr,UAL_BT_DEV_ADDRESS_SIZE))
    {
        TRACE_APP_BT("bond addr is null");
        return FALSE;
    }
    TRACE_APP_BT("[dev]bond addr =%d,%d,%d,%d,%d,%d",bt_bond_dialer_addr[0],bt_bond_dialer_addr[1],bt_bond_dialer_addr[2],
                                                  bt_bond_dialer_addr[3],bt_bond_dialer_addr[4],bt_bond_dialer_addr[5]);
#ifdef UAL_BT_HFU_SUPPORT
    TRACE_APP_BT("[dev]BT_HfuGetConnStatus = %d",ual_bt_hfu_get_connect_status());
    if(TRUE == ual_bt_hfu_get_connect_status())
    {
        TRACE_APP_BT("[dev]hfu has connected,do nothing");
    }
   else
    {
        TRACE_APP_BT("[dev]to connect hfu");
        ual_bt_hfu_connect(bt_bond_dialer_addr);
    }
#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : BTDialerCancelPair
//  Global resource dependence : none
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL void BTDialerCancelPair(uint8 timer_id, uint32 param)
{
    ual_bt_status_e del_ret = UAL_BT_STATUS_MAX;
    uint8 dialer_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
    Bbm_GetBTBoundDevAddr(dialer_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);

    TRACE_APP_BT("enter");
    if(0 != s_bt_dialer_disconnect_timer)
    {
        TRACE_APP_BT("stop timer");
        MMK_StopTimer(s_bt_dialer_disconnect_timer);
        s_bt_dialer_disconnect_timer = 0;

        if(!ual_bt_hfu_get_connect_status())//if hfu connect,disconnect hfu
        {
            TRACE_APP_BT("unbound HFU");
            del_ret = ual_bt_dev_delete_paired_dev(dialer_addr);
        }
        else
        {
            TRACE_APP_BT("start timer,to disconnect hfu");
            s_bt_dialer_disconnect_timer = MMK_CreateTimerCallback(BT_SLAVER_DISCONNECT_TIMER_OUT, BTDialerCancelPair, 0, FALSE);
        }
    }
}

/*****************************************************************************/
//  Description  : HandleDialerCancelPairQueryWin
//  Paramter:
//          [In]:win id,msg id 
//          [Out]:None
//          [Return]:result
//  Author: chunjuan.liang
//  Note:取消配对时，弹出窗体
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialerCancelPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E             ret = MMI_RESULT_TRUE;
    ual_bt_status_e return_value = UAL_BT_STATUS_SUCCESS;
    uint16                   idx = GUILIST_GetCurItemIndex(MMIBT_ADULTWACHT_PAIR_LIST_CTRL_ID);
    ual_bt_status_e      del_ret = UAL_BT_STATUS_SUCCESS;

    TRACE_APP_BT("msg_id =%x",msg_id);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        case MSG_APP_WEB:
        {
            uint8  dialer_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
            TRACE_APP_BT("MSG_NOTIFY_CANCEL");

            MMK_CloseWin(win_id);
            Bbm_GetBTBoundDevAddr(dialer_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);

            TRACE_APP_BT("dialer addr %x,%x,%x,%x,%x,%x",dialer_addr[0],dialer_addr[1],dialer_addr[2],
                                                         dialer_addr[3],dialer_addr[4],dialer_addr[5]);
            if(!ual_bt_hfu_get_connect_status())//if hfu connect,disconnect hfu
            {
                TRACE_APP_BT("disconnect HFU");
                del_ret = ual_bt_dev_delete_paired_dev(dialer_addr);
                TRACE_APP_BT("del_ret=%d",del_ret);
            }
            else
            {
                ual_bt_hfu_disconnect();//wait stack message
                // TODO:delete device?
                s_bt_dialer_disconnect_timer = MMK_CreateTimerCallback(BT_SLAVER_DISCONNECT_TIMER_OUT, BTDialerCancelPair, 0, FALSE);
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
//  Description  : HandleDialerPairQueryWin
//  Paramter:
//          [In]:win id,msg id 
//          [Out]:None
//          [Return]:result
//  Author: chunjuan.liang
//  Note:取消配对窗口，
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialerPairQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E             ret = MMI_RESULT_TRUE;

    TRACE_APP_BT("MSG ID =%x",msg_id);

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        case MSG_APP_WEB:
        case MSG_NOTIFY_OK:
        {
            TRACE_APP_BT("MSG_NOTIFY_CANCEL/OK");
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

LOCAL void HandleDialerPairAction(void)
{
    MMI_STRING_T         txt_tip = {0};

    MMI_IMAGE_ID_T  img_cancel = IMAGE_SET_COM_AW_BTN_CANCEL;
    MMI_IMAGE_ID_T  img_ok     = IMAGE_SET_COM_AW_BTN_CONFIRM;

    uint8 bound_dev_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
    uint8       str_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
    uint16   addr_len = 0;
    // TODO: bbm switch to bt dialer
#ifdef BBM_ROLE_SUPPORT
    BbmSwitchToBtDialerMode();
#else
    ual_bt_dev_enable_dialer();
#endif
    Bbm_GetBTBoundDevAddr(bound_dev_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);

    addr_len = sizeof(bound_dev_addr);

    TRACE_APP_BT("addr_len = %d,addr=%x,%x,%x,%x,%x,%x",addr_len,bound_dev_addr[0],bound_dev_addr[1],bound_dev_addr[2],
                                                                 bound_dev_addr[3],bound_dev_addr[4],bound_dev_addr[5]);

    SCI_MEMSET(str_addr,0,UAL_BT_DEV_ADDRESS_SIZE);

    if(0 == memcmp(str_addr,bound_dev_addr,UAL_BT_DEV_ADDRESS_SIZE))
    {
        TRACE_APP_BT("request pair");
        MMI_GetLabelTextByLang(STXT_PAIR, &txt_tip);
        AdultWatchCOM_QueryWin_2Btn_Enter(ADULTWATCH_BBM_PAIR_REQUEST_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,HandleDialerPairQueryWin);
    }
    else
    {
        TRACE_APP_BT("cancel pair");
        MMI_GetLabelTextByLang(TXT_BT_CANCEL_PAIR, &txt_tip);
        AdultWatchCOM_QueryWin_2Btn_Enter(ADULTWATCH_BT_CANCEL_PAIR_WIN_ID,&txt_tip,PNULL,PNULL,img_cancel,img_ok,HandleDialerCancelPairQueryWin);
    }
}

LOCAL void AppendDialerPairItem(MMI_CTRL_ID_T list_ctrl_id)
{
    MMI_STRING_T dialer_name = {0};
    MMI_STRING_T pair_status = {0};
    MMI_STRING_T    dev_name = {0};
    MMI_STRING_T    dev_addr = {0};
    uint8       str_addr[BT_PAIR_ADDR_VIEW_MAX_LEN] = {0};
    wchar      wstr_addr[BT_PAIR_ADDR_VIEW_MAX_LEN] = {0};
    uint16      addr_len = 0;

    BT_PAIR_ITEM_DATA_T pair_data_info = {0};
    uint16 list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);
    uint32 user_data = 0;

    BBM_BOUND_REMOTE_DEV_INFO_T bound_dev_info = {0};
    Bbm_GetBTBoundDevInfo(&bound_dev_info);

    
    MMI_GetLabelTextByLang(TXT_SET_BT_ROLE_DIALER,&dialer_name);

    addr_len = SCI_STRLEN(bound_dev_info.classic_addr);

    TRACE_APP_BT("addr_len = %d,addr=%x,%x,%x,%x,%x,%x",addr_len,bound_dev_info.classic_addr[5],bound_dev_info.classic_addr[4],bound_dev_info.classic_addr[3],
                                                        bound_dev_info.classic_addr[2],bound_dev_info.classic_addr[1],bound_dev_info.classic_addr[0]);

    TRACE_APP_BT("name len = %d",MMIAPICOM_Wstrlen(bound_dev_info.bt_name));
    dev_name.wstr_ptr = bound_dev_info.bt_name;
    dev_name.wstr_len = MMIAPICOM_Wstrlen(dev_name.wstr_ptr);

    SCI_MEMSET(wstr_addr,0,sizeof(wchar)*(BT_PAIR_ADDR_VIEW_MAX_LEN));

    sprintf(str_addr, "%02x:%02x:%02x:%02x:%02x:%02x",bound_dev_info.classic_addr[5],bound_dev_info.classic_addr[4],bound_dev_info.classic_addr[3],
                                                        bound_dev_info.classic_addr[2],bound_dev_info.classic_addr[1],bound_dev_info.classic_addr[0]);
    TRACE_APP_BT("str_addr = %d",SCI_STRLEN(str_addr));
    
    MMI_STRNTOWSTR(wstr_addr,BT_PAIR_ADDR_VIEW_MAX_LEN,str_addr,BT_PAIR_ADDR_VIEW_MAX_LEN,strlen(str_addr));
    dev_addr.wstr_ptr = wstr_addr;
    dev_addr.wstr_len = MMIAPICOM_Wstrlen(dev_addr.wstr_ptr);
    TRACE_APP_BT("dev_addr.wstr_len = %d",dev_addr.wstr_len);

    SCI_MEMSET(str_addr,0,BT_PAIR_ADDR_VIEW_MAX_LEN);
    if(0 == memcmp(str_addr,bound_dev_info.classic_addr,addr_len))
    {
        SCI_MEMSET(&dev_name,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&dev_addr,0,sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_BT_UNPAIR,&pair_status);
    }
    else
    {
        MMI_GetLabelTextByLang(STXT_PAIRED,&pair_status);
    }

    AdultWatchCOM_ListItem_Draw_4Str_3line(list_ctrl_id,dialer_name,pair_status,dev_name,dev_addr);

    pair_data_info.type          = BT_PAIR_TYPE_DIALER;
    pair_data_info.fun_cb        = (uint32)HandleDialerPairAction;
    s_pair_data[s_pair_item_num] = pair_data_info;
    user_data = (uint32)&s_pair_data[s_pair_item_num];
    GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);
    s_pair_item_num++;
    
    GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);

    TRACE_APP_BT("exit s_pair_item_num=%d.",s_pair_item_num);
}
#endif
#endif
/****************************************************************************/
/************************Dialer Reference API end****************************/
/****************************************************************************/

/****************************************************************************/
/************************Phone  Reference API begin**************************/
/****************************************************************************/
LOCAL void HandlePhoneTxtAction(void)
{
#ifdef BBM_ROLE_SUPPORT
    BbmSwitchToBTPhoneMode();
#else
    ual_bt_dev_enable_phone();
#endif
    TRACE_APP_BT("enter phone win");
    AdultBT_MainWin_Enter();
}

LOCAL void AppendPhoneTxtItemToList(MMI_CTRL_ID_T list_ctrl_id)
{
    MMI_STRING_T             txt_phone = {0};
    BT_PAIR_ITEM_DATA_T item_data_info = {0};
    uint32                   user_data = 0;
    uint16                  list_index = CTRLLIST_GetTotalItemNum(list_ctrl_id);

    MMI_GetLabelTextByLang(TXT_SET_BT_ROLE_PHONE,&txt_phone);
    //append phone item
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign(list_ctrl_id,txt_phone);

    //set callback
    item_data_info.type = BT_PAIR_TYPE_PHONE;
    item_data_info.fun_cb = (uint32)HandlePhoneTxtAction;
    s_pair_data[s_pair_item_num] = item_data_info;
    user_data = (uint32)&s_pair_data[s_pair_item_num];
    GUILIST_SetItemUserData(list_ctrl_id, list_index, &user_data);

    s_pair_item_num++;

}
#ifdef BBM_ROLE_SUPPORT
/*****************************************************************************/
//  Description : ResetToTargetMode
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: chunjuan.liang
//  Note:退出窗口时，回到相应的模式
/*****************************************************************************/
LOCAL void ResetToTargetMode(void)
{
#ifdef BBM_ROLE_SUPPORT
        BBM_MODE_E  bt_mode = Bbm_GetBTMode();
#endif

    TRACE_APP_BT("enter,bt_mode=%d",bt_mode);
    switch(bt_mode)
    {
        case BBM_MODE_AUTO:
            BbmSwitchToAutoMode();
            break;
        case BBM_MODE_SLAVER:
            BbmSwitchToSlaverMode();
            break;
        case BBM_MODE_BLE:
            BbmSwitchToBleMode();
            break;
        case BBM_MODE_DIALER:
            BbmSwitchToBtDialerMode();
            break;
        case BBM_MODE_PHONE:
            BbmSwitchToBTPhoneMode();
            break;
        default:
            break;
    }
    TRACE_APP_BT("exit");
}
#endif
/****************************************************************************/
/************************Phone  Reference API end****************************/
/****************************************************************************/
LOCAL void BTPairOpenWin(MMI_WIN_ID_T win_id)
{

    MMI_CTRL_ID_T           listCtrlId =  MMIBT_ADULTWACHT_PAIR_LIST_CTRL_ID;

    MMK_SetAtvCtrl( win_id, listCtrlId );

    GUILIST_SetMaxItem( listCtrlId, BT_PAIR_LIST_MAX_NUM, FALSE );

}
LOCAL void BTPairFullPaint(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T           listCtrlId =  MMIBT_ADULTWACHT_PAIR_LIST_CTRL_ID;
    uint16 focus_index = GUILIST_GetCurItemIndex(listCtrlId);
    int32           listOffset = 0;

    TRACE_APP_BT("enter.");
    // get top item offset
    GUILIST_GetTopItemOffset( listCtrlId, &listOffset );
    //remove all list items
    GUILIST_RemoveAllItems(listCtrlId);

    GUILIST_SetMaxItem( listCtrlId, BT_PAIR_LIST_MAX_NUM, FALSE );

    s_pair_item_num = 0;
#ifdef BBM_ROLE_SUPPORT
    AppendBlePairItem(listCtrlId);
    AppendDialerPairItem(listCtrlId);
#endif
    AppendPhoneTxtItemToList(listCtrlId);

    //set focus index
    GUILIST_SetCurItemIndex(listCtrlId, focus_index);
    GUILIST_SetListState(listCtrlId, GUILIST_STATE_SPLIT_LINE, TRUE);//需要分割线

    GUILIST_SetTopItemOffset(listCtrlId,listOffset);

    if((TRUE == MMIENG_IsMonkeyTestMode()) || (FALSE == MMIAPIENG_GetBBMViewVisible()))
    {
        GUILIST_SetItemGray(listCtrlId,0,TRUE);//set item ble pair gray
    }
    TRACE_APP_BT("exit.");
}

LOCAL void BTPairCloseWin(MMI_WIN_ID_T win_id)
{
    ual_bt_dev_status_e bt_status = UAL_BT_DEV_STATUS_INIT;
    bt_status = ual_bt_dev_get_status();

    if(UAL_BT_DEV_STATUS_SEARCHING == bt_status)
    {
        TRACE_APP_BT("cancel search");
        ual_bt_dev_cancel_search();
    }
#ifdef BBM_ROLE_SUPPORT
    // TODO:切换到进入窗口之前的mode
    ResetToTargetMode();
    BbmUnregister("BT APP");
#endif
}

LOCAL void BTPairAppOK( MMI_WIN_ID_T win_id)
{
    uint16              index = 0;
    uint32              userData = 0;
    BOOLEAN             reCode = FALSE;
    BT_PAIR_ITEM_FUNC itemFunc = PNULL;
    MMI_CTRL_ID_T       listCtrlId =  MMIBT_ADULTWACHT_PAIR_LIST_CTRL_ID;

    BT_PAIR_ITEM_DATA_T *item_data = (BT_PAIR_ITEM_DATA_T *)userData;

    TRACE_APP_BT("enter.");

    index = GUILIST_GetCurItemIndex( listCtrlId );
    reCode = GUILIST_GetItemData( listCtrlId, index, (uint32*)(&userData) );
    if( FALSE == reCode )
    {
        TRACE_APP_BT("GUILIST_GetItemData fail!!!");
        return;
    }

    item_data = (BT_PAIR_ITEM_DATA_T *)userData;

    if(PNULL != item_data)
    {
        itemFunc = (BT_PAIR_ITEM_FUNC)(item_data->fun_cb);
    }
    else
    {
        TRACE_APP_BT("item data is null");
    }

    TRACE_APP_BT("itemFunc = %0x.", itemFunc);
    if( PNULL != itemFunc )
    {
        (itemFunc)();
    }

    TRACE_APP_BT("exit.");
}

/*****************************************************************************/
//  Description:produrce of Bt Pair Window
//  Paramter:
//          [In]:win_id msg_id param
//          [Out]:
//          [Return]:
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdultWatchBTPairWindow(
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
            BTPairOpenWin(win_id);
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_BT("MSG_CLOSE_WINDOW");
            
            BTPairCloseWin(win_id);
            break;
        }
        case MSG_FULL_PAINT:
        {
            TRACE_APP_BT("MSG_FULL_PAINT");
            BTPairFullPaint(win_id);
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
            BTPairAppOK(win_id);
        }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            TRACE_APP_BT("MSG_APP_CANCEL");
            MMK_CloseWin(win_id);
            break;
        }
        default:
            result =  MMI_RESULT_FALSE;
            break;
    }

    return result;
}

/*win table*/
WINDOW_TABLE( ADULTWATCH_BT_PAIR_WIN_TAB ) =
{
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_BT_DEVICE_LIST),
    WIN_FUNC( (uint32)HandleAdultWatchBTPairWindow ),
    WIN_ID( ADULTWATCH_BT_PAIR_WIN_ID ),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIBT_ADULTWACHT_PAIR_LIST_CTRL_ID ),
    END_WIN
};

/*---------------------------------------------------------------------------*
**                          PUBLIC FUNCTION DEFINITION                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: BTPair_FreshWin
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note:包括BLE和dialer配对
//
/*****************************************************************************/
PUBLIC void BTPair_FreshWin(void)
{
    if(MMK_IsOpenWin(ADULTWATCH_BT_PAIR_WIN_ID))
    {
        BTPairFullPaint(ADULTWATCH_BT_PAIR_WIN_ID);
    }
}

/****************************************************************************/
//  Description : enter adult bluetooth main window
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:
/****************************************************************************/
PUBLIC void AdultBT_Pair_Enter( void )
{
    if(TRUE == Bbm_GetBTOn())
    {
#ifdef BBM_ROLE_SUPPORT
        BbmRegister("BT APP",Bbm_Ble_CallBack);
#endif
        MMK_CreateWin( (uint32*)ADULTWATCH_BT_PAIR_WIN_TAB, PNULL);
    }
    else
    {
        TRACE_APP_BT("BT close");
    }
}
