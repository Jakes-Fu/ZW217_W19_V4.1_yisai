/****************************************************************************
** File Name:      bbm_control.c                                             *
** Author:                                                                   *
** Date:           01/19/21                                                  *
** Copyright:      2021 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the bbm control part        *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/19/21        changli.jiang    create
*****************************************************************************/
#include "mmi_link.h"
#include "mmi_string.h"
#include "mmicom_trace.h"
#include "bbm_internal.h"
#include "bbm_export.h"
#include "mmicc_export.h"
#include "mmiphone_export.h"

#define BLE_CONNECT_TIMEOUT    (3*60*1000)

typedef enum
{
    BBM_STATE_IDLE,//空闲状态
    BBM_BLE_CONNECTTING,//BLE正在连接的状态
    BBM_BLE_CONNECTTED,//BLE连接
    BBM_BT_DIAL_CONNECTTING,//BT dial连接中
    BBM_BT_DIAL_CONNECTED,//bt dial已经连接
    BBM_BT_PHONE_OPENNING,//bt phone 打开中
    BBM_BT_PHONE_OPENNED,//bt phone 已经打开
    BBM_DETAIL_STATE_MAX,
}BBM_DETAIL_STATE_E;
typedef enum
{
    BBM_AUTO_MODE,
    BBM_SLAVER_MODE,
    BBM_BT_PHONE_MODE,
    BBM_BT_DIALER_MODE,
    BBM_BLE_MODE,
    BBM_MODE_STATE_MAX,
}BBM_MODE_STATE_E;
//bbm user info manage
typedef struct
{
    char user_name[BLE_USER_NAME_LEN];
    Bbm_CallBack bbm_user_callback;
}BBM_USER_INFO_T;

LOCAL MMI_LINK_NODE_T *g_bbm_user_list_ptr = PNULL;

BBM_DETAIL_STATE_E g_bbm_auto_mode_detail_state     = BBM_DETAIL_STATE_MAX;//auto mode state define
BBM_DETAIL_STATE_E g_bbm_slaver_mode_detail_state   = BBM_DETAIL_STATE_MAX;//slaver mode state define
BBM_DETAIL_STATE_E g_bbm_ble_mode_detail_state      = BBM_DETAIL_STATE_MAX;//ble mode state define
BBM_DETAIL_STATE_E g_bbm_btdialer_mode_detail_state = BBM_DETAIL_STATE_MAX;//bt dialer state define
BBM_DETAIL_STATE_E g_bbm_btphone_mode_detail_state  = BBM_DETAIL_STATE_MAX;//bt phone state define


BBM_MODE_STATE_E g_bbm_mode_state = BBM_MODE_STATE_MAX;
// timer define
#define RETRY_CONNECNT_BLE_TIMEROUT (500)//500ms 之后重新尝试连接ble
LOCAL uint8 g_retry_connect_ble_timer_id = 0;//ble retry timer

#define RETRY_CONNECNT_BTDIALER_TIMEROUT (500)//500ms 之后重新尝试连接btdialer
LOCAL uint8 g_retry_connect_bt_dialer_timer_id = 0;//bt dilaer retry timer

#define RETRY_CONNECNT_BTPHONE_TIMEROUT (500)//500ms 之后重新尝试连接btphone
LOCAL uint8 g_retry_connect_bt_phone_timer_id = 0;//bt phone retry timer

#define CHECK_PHONE_IS_SEARCHED_TIMEROUT (300)//每300mscheck一次对端设备是否找到
LOCAL uint8 g_check_remote_phone_is_searched_timer_id = 0;

LOCAL void BbmStopCheckPhoneIsSearchedTimer(void)
{
    if(0 != g_check_remote_phone_is_searched_timer_id)
    {
        MMK_StopTimer(g_check_remote_phone_is_searched_timer_id);
        g_check_remote_phone_is_searched_timer_id = 0;
    }
}
LOCAL void BBM_HandleCheckRemotePhoneIsSearchedTimer(uint8 timer_id,uint32 param)
{
    if(g_check_remote_phone_is_searched_timer_id == timer_id)
    {
        BOOLEAN is_phone_searched = FALSE;
        //调用API phone is searched
        is_phone_searched = Bbm_BoundDeviceIsSearched();
        if(TRUE == is_phone_searched)
        {
            BbmMainProcess(MSG_BBM_PHONE_SEARCHED);
        }
        else
        {
            TRACE_APP_BBM("is_phone_searched false!,phone is not searched!\n");
        }
    }
}
LOCAL void BbmStartCheckRemotePhoneIsSearchedTimer(void)
{
    if(0 != g_check_remote_phone_is_searched_timer_id)
    {
        MMK_StopTimer(g_check_remote_phone_is_searched_timer_id);
        g_check_remote_phone_is_searched_timer_id = 0;
    }
    g_check_remote_phone_is_searched_timer_id = MMK_CreateTimerCallback(CHECK_PHONE_IS_SEARCHED_TIMEROUT,BBM_HandleCheckRemotePhoneIsSearchedTimer,PNULL,TRUE);
}
LOCAL void BbmStopRetryConnectBleTimer(void)
{
    if(0 != g_retry_connect_ble_timer_id)
    {
        MMK_StopTimer(g_retry_connect_ble_timer_id);
        g_retry_connect_ble_timer_id = 0;
    }
}
LOCAL void BBM_HandleRetryConnectBleTimer(uint8 timer_id,uint32 param)
{
    if(g_retry_connect_ble_timer_id == timer_id)
    {
        MMK_StopTimer(g_retry_connect_ble_timer_id);
        g_retry_connect_ble_timer_id = 0;
        //connect ble
        bbm_connect_ble();
        TRACE_APP_BBM("retry connect ble timeout!\n");
    }
}

LOCAL void BbmStartRetryConnectBleTimer(void)
{
    TRACE_APP_BBM("start retry connect ble timer!\n");
    if(0 != g_retry_connect_ble_timer_id)
    {
        MMK_StopTimer(g_retry_connect_ble_timer_id);
        g_retry_connect_ble_timer_id = 0;
    }
   g_retry_connect_ble_timer_id =  MMK_CreateTimerCallback(RETRY_CONNECNT_BLE_TIMEROUT,BBM_HandleRetryConnectBleTimer,PNULL,FALSE);
}

LOCAL void BbmStopRetryConnectBtDialerTimer(void)
{
    if(0 != g_retry_connect_bt_dialer_timer_id)
    {
        MMK_StopTimer(g_retry_connect_bt_dialer_timer_id);
        g_retry_connect_bt_dialer_timer_id = 0;
    }
}
LOCAL void BBM_HandleRetryConnectBtDialerTimer(uint8 timer_id,uint32 param)
{
    if(g_retry_connect_bt_dialer_timer_id == timer_id)
    {
        MMK_StopTimer(g_retry_connect_bt_dialer_timer_id);
        g_retry_connect_bt_dialer_timer_id = 0;
        // TODO: connect bt dialer
    }
}

LOCAL void BbmStartRetryConnectBtDialerTimer(void)
{
    if(0 != g_retry_connect_bt_dialer_timer_id)
    {
        MMK_StopTimer(g_retry_connect_bt_dialer_timer_id);
        g_retry_connect_bt_dialer_timer_id  = 0;
    }
    g_retry_connect_bt_dialer_timer_id = MMK_CreateTimerCallback(RETRY_CONNECNT_BTDIALER_TIMEROUT,BBM_HandleRetryConnectBtDialerTimer,PNULL,FALSE);
}

LOCAL void BbmStopRetryConnectBtPhoneTimer(void)
{
    if(0 != g_retry_connect_bt_phone_timer_id)
    {
        MMK_StopTimer(g_retry_connect_bt_phone_timer_id);
        g_retry_connect_bt_phone_timer_id = 0;
    }
}
LOCAL void BBM_HandleRetryConnectBtPhoneTimer(uint8 timer_id,uint32 param)
{
    if(g_retry_connect_bt_phone_timer_id == timer_id)
    {
        ual_bt_status_e bt_status = UAL_BT_STATUS_MAX;
        MMK_StopTimer(g_retry_connect_bt_phone_timer_id);
        g_retry_connect_bt_phone_timer_id = 0;
        // open bt phone
        bt_status = Bbm_OpenClassicBtPhone();        
        if(UAL_BT_STATUS_SUCCESS == bt_status)
        {
            // notify bbm control bt open sucess
            BbmMainProcess(MSG_BBM_BT_PHONE_OPEN_SUCESS);
            TRACE_APP_BBM("begin enable btphone sucess!\n");
        }    
        else if(UAL_BT_STATUS_PENDING == bt_status)
        {
            TRACE_APP_BBM("begin enable bt phone pending for msg!\n");
        }
        else
        {
            TRACE_APP_BBM("begin enable btphone fail!\n");
        }
    }
}

LOCAL void BbmStartRetryConnectBtPhoneTimer(void)
{
    if(0 != g_retry_connect_bt_phone_timer_id)
    {
        MMK_StopTimer(g_retry_connect_bt_phone_timer_id);
        g_retry_connect_bt_phone_timer_id = 0;
    }
    g_retry_connect_bt_phone_timer_id = MMK_CreateTimerCallback(RETRY_CONNECNT_BTPHONE_TIMEROUT,BBM_HandleRetryConnectBtPhoneTimer,PNULL,FALSE);
}

LOCAL BOOLEAN SearchBbmUserInfo(
                                        MMI_LINK_NODE_T const* node_ptr,
                                        uint32 condition1, 
                                        uint32 condition2
                                    )
{
    if(0 == MMIAPICOM_Stricmp((const char *)condition1,(const char *)((BBM_USER_INFO_T*)node_ptr->data)->user_name))
    {
        TRACE_APP_BBM("have find user!\n");
        return TRUE;
    }
    return FALSE;
}

LOCAL BOOLEAN JudgeIfUserHasRegister(char *user_name)
{    
    MMI_LINK_NODE_T *deregister_user_node_ptr = PNULL;
    
    TRACE_APP_BBM("user_name:%s\n",user_name);
    
    deregister_user_node_ptr = MMILINK_SearchNode(g_bbm_user_list_ptr, TRUE, SearchBbmUserInfo, \
        (uint32)user_name, 0); 
    if(PNULL != deregister_user_node_ptr)
    {
        return TRUE;
    }
    else
    {
        TRACE_APP_BBM("this user has not regiserted!\n");
        return FALSE;
    }
}
/*******************************************************************************************/
//  Description : use to inform all user information
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC void BbmNotifyBbmUser(uint32 msg_id,void *param)
{
    MMI_LINK_NODE_T *temp_node_ptr = PNULL;
    temp_node_ptr = g_bbm_user_list_ptr;
    while(PNULL != temp_node_ptr)
    {
       if(PNULL != ((BBM_USER_INFO_T*)temp_node_ptr->data)->bbm_user_callback)
       {
            ((BBM_USER_INFO_T*)temp_node_ptr->data)->bbm_user_callback(msg_id,param);
       }
       else
       {
            TRACE_APP_BBM("bb_user_callback is NULL!\n");
       }
       temp_node_ptr = temp_node_ptr->next_ptr;
       if(g_bbm_user_list_ptr == temp_node_ptr)
       {
            TRACE_APP_BBM("bbm has notify all user!\n");
            break;
       }
    }
}

LOCAL void BbmAutoModeProcess(BBM_MSG_E msg_id)
{
    TRACE_APP_BBM("enter g_bbm_auto_mode_detail_state:%d,msg_id:%d\n",g_bbm_auto_mode_detail_state,msg_id);
    switch(g_bbm_auto_mode_detail_state)
    {
        case BBM_BLE_CONNECTTING:
        {
            switch(msg_id)
            {
                case MSG_BBM_SWITCH_TO_BT_DIALER:
                {
                    //强制停止ble，直接连接btdialer
                    g_bbm_auto_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    bbm_switch_ble_to_btdailer();
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BLE_CONNECT_SUCCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BLE;
                    // 1:switch state
                    g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTED;
                    //notify user
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                case MSG_BBM_BLE_CONNECT_FAIL:   
                case MSG_BBM_BLE_CONNECT_TIME_OUT:
                {
                    // 1:open bt phone
                    // 2:power on ps
                    bbm_switch_ble_to_btphone();
                    //1 power on modem
                    MMIAPIPHONE_PowerOnModem();
                    g_bbm_auto_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    break;
            }
        }
        break;
        case BBM_BLE_CONNECTTED:
        {
            switch(msg_id)
            {
                case MSG_BBM_BLE_INCOMMING_CALL:
                case MSG_BBM_SWITCH_TO_BT_DIALER:
                {
                    // 1:close ble
                    // 2:call reconnect btdial api                    
                    g_bbm_auto_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    bbm_switch_ble_to_btdailer();
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BLE_PASSIVE_DISCONNECTED://被动断开
                {
                    // 1:open bt phone
                    // 2:power on ps
                    bbm_switch_ble_to_btphone();
                    //1 power on modem
                    MMIAPIPHONE_PowerOnModem();
                    g_bbm_auto_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                break;
            }
        }
        break;
        case BBM_BT_DIAL_CONNECTTING:
        {
            switch(msg_id)
            {
                case MSG_BBM_BT_DIALER_CONNECTED_SUCCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BTDIALER;
                    g_bbm_auto_mode_detail_state = BBM_BT_DIAL_CONNECTED;
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                case MSG_BBM_BT_DIALER_CONNECTED_FAIL:
                {
                    //先通知btdialer切换失败，再告诉角色切换中
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_FAIL,PNULL);
                    bbm_switch_btdialer_to_ble();
                    g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                break;
            }
        }
        break;
        case BBM_BT_DIAL_CONNECTED:
        {
            switch(msg_id)
            {
                case MSG_BBM_SWITCH_TO_BLE:
                case MSG_BBM_BT_DIALER_DISCONNECT_SUCESS:
                {
                    // 1:stop btdial:
                    // 2:open ble
                    bbm_switch_btdialer_to_ble();
                    g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                break;
            }
        }
        break;
        case BBM_BT_PHONE_OPENNING:
        {
            switch(msg_id)
            {
                case MSG_BBM_BT_PHONE_OPEN_FAIL:
                {
                    // 1 start timer
                    // 2 continue open bt phone
                    BbmStartRetryConnectBtPhoneTimer();
                    g_bbm_auto_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BT_PHONE_OPEN_SUCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BTPHONE;
                    // 1 start timer
                    // 2 定时检查设备是否在的接口
                    //开始搜索
                    Bbm_StartSearchBoundDevice();
                    BbmStartCheckRemotePhoneIsSearchedTimer();
                    g_bbm_auto_mode_detail_state = BBM_BT_PHONE_OPENNED;
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                default:
                break;
            }
        }
        break;
        case BBM_BT_PHONE_OPENNED:
        {
            switch(msg_id)
            {
                case MSG_BBM_PHONE_SEARCHED:
                    // 1:is in cc call
                    // 2:if in cc search timer should not stop
                    // 3:if not in cc call,stop bt phone search timer
                    // 4:change state
                    if(TRUE == MMIAPICC_IsInState(CC_IN_CALL_STATE))
                    {
                        TRACE_APP_BBM("current is in cc call!");
                    }
                    else
                    {
                        // stop timer
                        BbmStopCheckPhoneIsSearchedTimer();
                        // TODO:停止搜索的接口先不做
                        ual_bt_dev_cancel_search();
                        bbm_switch_btphone_to_ble();
                        //1 power off modem
                        MMIAPIPHONE_PowerOffModem();
                        TRACE_APP_BBM("current is Not in cc call!");
                        g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    break;
                default:
                    break;
            }
        }
        break;

	default:
	    break;

    }
}
/*******************************************************************************************/
//  Description : bbm ble mode process
//  Param in: BBM_MSG_E:msg_id
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
LOCAL void BbmBleModeProcess(BBM_MSG_E msg_id)
{
    TRACE_APP_BBM("ble mode state:%d,msg_id:%d\n",g_bbm_ble_mode_detail_state,msg_id);    
    switch(g_bbm_ble_mode_detail_state)
    {
        case BBM_BLE_CONNECTTING:
        {
            switch(msg_id)
            {
                case MSG_BBM_BLE_CONNECT_SUCCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BLE;
                    g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTED;
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                case MSG_BBM_BLE_CONNECT_FAIL:
                case MSG_BBM_BLE_CONNECT_TIME_OUT:
                    {
                        // TODO:先起timer，统一大家的流程，先不用set adv policy的接口
                        // 1 起timer 尝试连接,暂时先不起timer，使用set adv policy的接口测试
                        BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_FAIL,PNULL);
                        BbmStartRetryConnectBleTimer();
                        g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    break;
                default:
                    TRACE_APP_BBM("ble mode ble connectting state should not process msgid:%d\n",msg_id);
                    break;
            }
        }
        break;
        case BBM_BLE_CONNECTTED:
        {
            switch(msg_id)
            {
                case MSG_BBM_BLE_PASSIVE_DISCONNECTED:
                case MSG_BBM_BLE_DISCONNECTED_SUCESS_CNF:
                {
                    // 1:起 timer 尝试连接，暂时不起timer，先使用set adv policy的接口
                    BbmStartRetryConnectBleTimer();
                    g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BLE_INCOMMING_CALL:
                {
                    TRACE_APP_BBM("in comming call!");
                    g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTED;
                }
                break;
                default:
                    TRACE_APP_BBM("ble mode ble connect state should not process msg:%d",msg_id);
                break;
            }
        }
        break;
        default:
        {
            TRACE_APP_BBM("ble mode state:%d is wrong!\n",g_bbm_ble_mode_detail_state);
        }
        break;
    }

}
/*******************************************************************************************/
//  Description : bbm bt dialer process
//  Param in: BBM_MSG_E:msg_id
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
LOCAL void BbmBTDialerModeProcess(BBM_MSG_E msg_id)
{
    TRACE_APP_BBM("bt dialer mode state:%d,msg_id:%d\n",g_bbm_btdialer_mode_detail_state,msg_id);
    switch(g_bbm_btdialer_mode_detail_state)
    {
        case BBM_BT_DIAL_CONNECTTING:
        {
            switch(msg_id)
            {
                case MSG_BBM_BT_DIALER_CONNECTED_SUCCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BTDIALER;
                    g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTED;
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                // TODO:如果长时间不回消息直接timerout??再尝试连
                case MSG_BBM_BT_DIALER_CONNECTED_FAIL:
                {
                    // start timer to retry connect
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_FAIL,PNULL);
                    BbmStartRetryConnectBtDialerTimer();
                    g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("msg id:%d,should not process in dialer connectting state!\n");
                    break;
            }
        }
        break;
        case BBM_BT_DIAL_CONNECTED:
        {
            switch(msg_id)
            {
                case MSG_BBM_BT_DIALER_DISCONNECT_SUCESS:
                {
                    // start timer to retry connect
                    BbmStartRetryConnectBtDialerTimer();
                    g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("msg id:%d should not process in dialer connect state!\n");
                break;
            }
        }
        break;
        default:
            TRACE_APP_BBM("bt dialer mode state:%d is wrong!",g_bbm_btdialer_mode_detail_state);
            break;
    }
}
/*******************************************************************************************/
//  Description : bbm Slaver mode process
//  Param in: BBM_MSG_E:msg_id
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
LOCAL void BbmSlaverModeProcess(BBM_MSG_E msg_id)
{
    TRACE_APP_BBM("slaver mode state:%d,msg_id:%d\n",g_bbm_slaver_mode_detail_state,msg_id);
    switch(g_bbm_slaver_mode_detail_state)
    {
        case BBM_BLE_CONNECTTING:
        {
            switch(msg_id)
            {
                case MSG_BBM_SWITCH_TO_BT_DIALER:
                {
                    //强制停止BLE开启BTDIALER                    
                    g_bbm_slaver_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    bbm_switch_ble_to_btdailer();
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BLE_CONNECT_SUCCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BLE;
                    // 1:switch state
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTED;
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                case MSG_BBM_BLE_CONNECT_FAIL:
                case MSG_BBM_BLE_CONNECT_TIME_OUT:
                {
                    // TODO: 1:开启timer 先尝试再广播，另外，暂时不用policy flag 代替是否有效
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_FAIL,PNULL);
                    BbmStartRetryConnectBleTimer();
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("slaver mode ble connecting state should not process msgid:%d",msg_id);
                    break;
            }
        }
        break;
        case BBM_BLE_CONNECTTED:
        {
            switch(msg_id)
            {
                case MSG_BBM_BLE_INCOMMING_CALL:
                case MSG_BBM_SWITCH_TO_BT_DIALER:
                {
                    // 1:close ble
                    // 2:call reconnect btdial api  
                    bbm_switch_ble_to_btdailer();
                    g_bbm_slaver_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BLE_PASSIVE_DISCONNECTED://被动断开
                case MSG_BBM_BLE_DISCONNECTED_SUCESS_CNF:
                {
                    // 1:open ble
                    // TODO:1:开启timer先尝试再广播，另外，暂时不用policy flag代替是否有效
                    BbmStartRetryConnectBleTimer();
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("slaver mode ble connected state should not process msgid:%d",msg_id);
                break;
            }
        }
        break;
        case BBM_BT_DIAL_CONNECTTING:
        {
            switch(msg_id)
            {
                case MSG_BBM_BT_DIALER_CONNECTED_SUCCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BTDIALER;
                    g_bbm_slaver_mode_detail_state = BBM_BT_DIAL_CONNECTED;
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                case MSG_BBM_BT_DIALER_CONNECTED_FAIL:
                {
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_FAIL,PNULL);
                    bbm_switch_btdialer_to_ble();
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("slaver mode btdialer conectting state should not process msgid:%d",msg_id);
                break;
            }
        }
        break;
        case BBM_BT_DIAL_CONNECTED:
        {
            switch(msg_id)
            {
                case MSG_BBM_SWITCH_TO_BLE:
                case MSG_BBM_BT_DIALER_DISCONNECT_SUCESS:
                {
                    // 1:stop btdial:
                    // 2:open ble
                    bbm_switch_btdialer_to_ble();
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("slaver mode btdialer connected state should not process msgid:%d",msg_id);
                break;
            }
        }
        break;
        default:
            TRACE_APP_BBM("slaver mode state:%d is wrong!\n",g_bbm_slaver_mode_detail_state);
        break;
    }
}
/*******************************************************************************************/
//  Description : bbm bt phone mode process
//  Param in: BBM_MSG_E:msg_id
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
LOCAL void BbmBtPhoneModeProcess(BBM_MSG_E msg_id)
{
    TRACE_APP_BBM("btphone mode state:%d,msg_id:%d\n",g_bbm_btphone_mode_detail_state,msg_id);
    switch(g_bbm_btphone_mode_detail_state)
    {
        case BBM_BT_PHONE_OPENNING:
        {
            switch(msg_id)
            {
                case MSG_BBM_BT_PHONE_OPEN_SUCESS:
                {
                    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_BTPHONE;
                    // 1:switch state
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNED;
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_SUCESS,&current_detail_state);
                }
                break;
                case MSG_BBM_BT_PHONE_OPEN_FAIL:
                {
                    // 1 start tiemr
                    BbmNotifyBbmUser(BBM_MSG_BT_SWITCH_FAIL,PNULL);
                    BbmStartRetryConnectBtPhoneTimer();
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("btphone mode phone openning state should not process msgid:%d",msg_id);
                    break;
            }
        }
        break;
        case BBM_BT_PHONE_OPENNED:
        {
            switch(msg_id)
            {
                case MSG_BBM_BT_PHONE_CLOSE_SUCESS:
                {
                    // 1 start timer
                    BbmStartRetryConnectBtPhoneTimer();
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BT_PHONE_CLOSE_FAIL:
                {
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNED;
                }
                break;
                default:
                    TRACE_APP_BBM("btphone mode phone_openned state should not process msgid:%d",msg_id);
                    break;
            }
        }
        break;
        default:
            TRACE_APP_BBM("btphone mode state:%d is wrong!\n",g_bbm_btphone_mode_detail_state);
            break;
            
    }
}
/*******************************************************************************************/
//  Description : bbm main process for every mode,it is every mode's entry
//  Param in: BBM_MSG_E:msg_id
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC void BbmMainProcess(BBM_MSG_E msg_id)
{
    TRACE_APP_BBM("enter g_bbm_mode_state:%d\n",g_bbm_mode_state);
    switch(g_bbm_mode_state)
    {
        case BBM_MODE_STATE_MAX:
        {
            switch(msg_id)
            {
                case MSG_BBM_CLOSE_ALL_MODE:
                {
                    TRACE_APP_BBM("cur state is NOT in any mode!\n");
                }
                break;
                case MSG_BBM_SWITCH_TO_AUTO_MODE:
                {
                    //1 power off modem
                    //MMIAPIPHONE_PowerOffModem();
                    //尝试连接ble
                    bbm_switch_other_device_to_ble();
                    // max state
                    g_bbm_mode_state = BBM_AUTO_MODE;
                    //auto mode detail state
                    g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_SLAVER_MODE:
                {
                    //1 power off modem
                    //MMIAPIPHONE_PowerOffModem();
                    //尝试连接ble
                    bbm_switch_other_device_to_ble();
                    // max state
                    g_bbm_mode_state = BBM_SLAVER_MODE;
                    //slaver mode detail state
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    //notify user bt is switching
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTPHONE_MODE:
                {
                    //1 power on modem
                    MMIAPIPHONE_PowerOnModem();
                    //尝试连接btphone
                    bbm_switch_other_device_to_btphone();
                    // max state
                    g_bbm_mode_state = BBM_BT_PHONE_MODE;
                    //btphone mode detail state
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTDIALER_MODE:
                {
                    //1 power off modem
                    //MMIAPIPHONE_PowerOffModem();
                    //尝试连接btdialer
                    bbm_switch_other_device_to_btdialer();
                    // max state
                    g_bbm_mode_state = BBM_BT_DIALER_MODE;
                    //bt dialer mode detail state
                    g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BLE_MODE:
                {
                    //1 power off modem
                    //MMIAPIPHONE_PowerOffModem();
                    //尝试连接btdialer
                    bbm_switch_other_device_to_ble();
                    // max state
                    g_bbm_mode_state = BBM_BLE_MODE;
                    //bt dialer mode detail state
                    g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    TRACE_APP_BBM("MAX MODE STATE Should not process this msg!\n");
                    break;
            }
        }
        break;
        case BBM_AUTO_MODE:
        {
            switch(msg_id)
            {
                case MSG_BBM_CLOSE_ALL_MODE:
                {
                    //1 power on modem，关闭之后是不起modem，起modem的原则是和模式相关
                    //MMIAPIPHONE_PowerOnModem();
                    //关闭所有设备
                    bbm_close_all_device();
                    TRACE_APP_BBM("auto close all mode!\n");
                    g_bbm_mode_state = BBM_MODE_STATE_MAX;
                    //notify user
                    //BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_AUTO_MODE:
                {
                    bbm_switch_other_device_to_ble();
                    TRACE_APP_BBM("BBM mode is already auto mode!");
                    g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_AUTO_MODE;
                }
                break;
                case MSG_BBM_SWITCH_TO_SLAVER_MODE:
                {
                    if(BBM_BLE_CONNECTTED ==g_bbm_auto_mode_detail_state)
                    {
                        g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTED;
                    }else if(BBM_BLE_CONNECTTING == g_bbm_auto_mode_detail_state)
                    {
                        g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                        // TODO:暂时此处回复状态切换
                    }else if(BBM_BT_DIAL_CONNECTTING == g_bbm_auto_mode_detail_state)
                    {
                        g_bbm_slaver_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    }else if(BBM_BT_DIAL_CONNECTED == g_bbm_auto_mode_detail_state)
                    {
                        g_bbm_slaver_mode_detail_state = BBM_BT_DIAL_CONNECTED;
                    }
                    else
                    {
                        BOOLEAN is_sucess = FALSE;
                        //1 power off modem
                        MMIAPIPHONE_PowerOffModem();
                        // stop ble/btdialer/btphone connect ble
                        is_sucess = bbm_switch_other_device_to_ble();
                        g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    g_bbm_mode_state = BBM_SLAVER_MODE;
                }
                break;
                case MSG_BBM_SWITCH_TO_BTPHONE_MODE:
                {
                    //当前automode切换到btphonemode，如果automode已经是btphone的opening或者openned， 
                    // 则切换到btphonemode时不需要再断开重连，直接切换状态
                    //1 power on modem
                    MMIAPIPHONE_PowerOnModem();
                    if(BBM_BT_PHONE_OPENNING == g_bbm_auto_mode_detail_state)
                    {
                        g_bbm_btphone_mode_detail_state = g_bbm_auto_mode_detail_state;
                    }
                    else if(BBM_BT_PHONE_OPENNED == g_bbm_auto_mode_detail_state)
                    {
                        g_bbm_btphone_mode_detail_state = g_bbm_auto_mode_detail_state;
                    }
                    else
                    {
                        // stop ble/btdialer/btphone connect btphone
                        bbm_switch_other_device_to_btphone();
                        g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    g_bbm_mode_state = BBM_BT_PHONE_MODE;
                }
                break;
                case MSG_BBM_SWITCH_TO_BTDIALER_MODE:
                {
                    //当前automode切换到btdialer mode，如果automode已经是btdialer的opening或者openned， 
                    // 则切换到btdilaermode时不需要再断开重连，直接切换状态
                    if((BBM_BT_DIAL_CONNECTTING == g_bbm_auto_mode_detail_state)
                       ||(BBM_BT_DIAL_CONNECTED == g_bbm_auto_mode_detail_state))
                    {
                        g_bbm_btdialer_mode_detail_state = g_bbm_auto_mode_detail_state;
                    }
                    else
                    {
                        if((BBM_BT_PHONE_OPENNING ==g_bbm_auto_mode_detail_state)
                            ||(BBM_BT_PHONE_OPENNED == g_bbm_auto_mode_detail_state))
                        {
                            //1 power off modem
                            MMIAPIPHONE_PowerOffModem();
                        }
                        //stop ble/btdialer/btphone connect btdialer
                        bbm_switch_other_device_to_btdialer();
                        g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    g_bbm_mode_state = BBM_BT_DIALER_MODE;
                }
                break;
                case MSG_BBM_SWITCH_TO_BLE_MODE:
                {
                    if((BBM_BLE_CONNECTTING == g_bbm_auto_mode_detail_state)
                        ||(BBM_BLE_CONNECTTED == g_bbm_auto_mode_detail_state))
                    {
                        g_bbm_ble_mode_detail_state = g_bbm_auto_mode_detail_state;
                    }
                    else
                    {
                        if((BBM_BT_PHONE_OPENNING ==g_bbm_auto_mode_detail_state)
                            ||(BBM_BT_PHONE_OPENNED == g_bbm_auto_mode_detail_state))
                        {
                            //1 power off modem
                            MMIAPIPHONE_PowerOffModem();
                        }
                        //stop ble/btdialer/btphone connect ble
                        bbm_switch_other_device_to_ble();
                        g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    g_bbm_mode_state = BBM_BLE_MODE;
                }
                break;
                default:
                    BbmAutoModeProcess(msg_id);
                    break;
            }
        }
        break;
        case BBM_SLAVER_MODE:
        {
            switch(msg_id)
            {
                case MSG_BBM_CLOSE_ALL_MODE:
                {
                    //1 power on modem
                    //MMIAPIPHONE_PowerOnModem();
                    //关闭所有设备
                    bbm_close_all_device();
                    TRACE_APP_BBM("slaver close all mode!\n");
                    g_bbm_mode_state = BBM_MODE_STATE_MAX;
                    //BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_AUTO_MODE:
                {
                    if(BBM_BLE_CONNECTTED ==g_bbm_slaver_mode_detail_state)
                    {
                        g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTED;
                    }else if(BBM_BLE_CONNECTTING == g_bbm_slaver_mode_detail_state)
                    {
                        g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    }else if(BBM_BT_DIAL_CONNECTTING == g_bbm_slaver_mode_detail_state)
                    {
                        g_bbm_auto_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    }else if(BBM_BT_DIAL_CONNECTED == g_bbm_slaver_mode_detail_state)
                    {
                        g_bbm_auto_mode_detail_state = BBM_BT_DIAL_CONNECTED;
                    }
                    else
                    {
                        //stop ble/btdialer/btphone connect ble
                        bbm_switch_other_device_to_ble();
                        g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    g_bbm_mode_state = BBM_AUTO_MODE;
                }
                break;
                case MSG_BBM_SWITCH_TO_SLAVER_MODE:
                {
                    bbm_switch_other_device_to_ble();
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_SLAVER_MODE;
                    TRACE_APP_BBM("BBM mode is already slaver mode!");
                }
                break;
                case MSG_BBM_SWITCH_TO_BTPHONE_MODE:
                {
                    //1 power on modem
                    MMIAPIPHONE_PowerOnModem();
                    // stop ble/btdialer/ connect btphone
                    bbm_switch_other_device_to_btphone();
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    g_bbm_mode_state =BBM_BT_PHONE_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTDIALER_MODE:
                {
                    if((BBM_BT_DIAL_CONNECTTING == g_bbm_slaver_mode_detail_state)
                       ||(BBM_BT_DIAL_CONNECTED == g_bbm_slaver_mode_detail_state))
                    {
                        g_bbm_btdialer_mode_detail_state = g_bbm_slaver_mode_detail_state;
                    }
                    else
                    {
                        //stop ble/btdialer connect btdialer
                        bbm_switch_other_device_to_btdialer();
                        g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    g_bbm_mode_state = BBM_BT_DIALER_MODE;
                }
                break;
                case MSG_BBM_SWITCH_TO_BLE_MODE:
                {
                    if((BBM_BLE_CONNECTTING == g_bbm_slaver_mode_detail_state)||(BBM_BLE_CONNECTTED == g_bbm_slaver_mode_detail_state))
                    {
                        g_bbm_ble_mode_detail_state = g_bbm_slaver_mode_detail_state;
                    }
                    else
                    {
                        //stop ble/btdialer/btphone connect ble
                        bbm_switch_other_device_to_ble();
                        g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                        BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                    }
                    g_bbm_mode_state = BBM_BLE_MODE;
                }
                break;
                default:
                    BbmSlaverModeProcess(msg_id);
                    break;
            }
        }
        break;
        case BBM_BT_PHONE_MODE:
        {
            switch(msg_id)
            {
                case MSG_BBM_CLOSE_ALL_MODE:
                {
                    //1 modem not need to power
                    //关闭所有设备
                    bbm_close_all_device();
                    TRACE_APP_BBM("bt phone close all mode!\n");
                    g_bbm_mode_state = BBM_MODE_STATE_MAX;
                    //BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_AUTO_MODE:
                {
                    //1 power off modem
                    MMIAPIPHONE_PowerOffModem();
                    //断开BTPHONE开BLE
                    bbm_switch_other_device_to_ble();
                    g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_AUTO_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_SLAVER_MODE:
                {
                    //1 power off modem
                    MMIAPIPHONE_PowerOffModem();
                    //断开BTPHONE开BLE
                    bbm_switch_other_device_to_ble();
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_SLAVER_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTPHONE_MODE:
                {
                    // this should not power on modem,但需要在开启btphonemode
                    bbm_switch_other_device_to_btphone();
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    g_bbm_mode_state = BBM_BT_PHONE_MODE;
                    TRACE_APP_BBM("BBM mode is already btphone mode!");
                }
                break;
                case MSG_BBM_SWITCH_TO_BTDIALER_MODE:
                {
                    //1 power off modem
                    MMIAPIPHONE_PowerOffModem();
                   // 断开其他开BTDIALER
                   bbm_switch_other_device_to_btdialer();
                   g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                   g_bbm_mode_state = BBM_BT_DIALER_MODE;
                   BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BLE_MODE:
                {
                    //1 power off modem
                    MMIAPIPHONE_PowerOffModem();
                    // 断开BTPHONE开BLE
                    bbm_switch_other_device_to_ble();
                    g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_BLE_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    BbmBtPhoneModeProcess(msg_id);
                    break;
            }
        }
        break;
        case BBM_BT_DIALER_MODE:
        {
            switch(msg_id)
            {
                case MSG_BBM_CLOSE_ALL_MODE:
                {
                    //关闭所有设备
                    //1 power on modem
                    //MMIAPIPHONE_PowerOnModem();
                    bbm_close_all_device();
                    TRACE_APP_BBM("bt dialer close all mode!\n");
                    g_bbm_mode_state = BBM_MODE_STATE_MAX;
                    //BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_AUTO_MODE:
                {
                    //断开BTDIALER开BLE
                    bbm_switch_other_device_to_ble();
                    g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_AUTO_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_SLAVER_MODE:
                {
                    //断开BTDIALER开BLE
                    bbm_switch_other_device_to_ble();
                    g_bbm_slaver_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_SLAVER_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTPHONE_MODE:
                {
                    //1 power on modem
                    MMIAPIPHONE_PowerOnModem();
                    //断开BTDIALER开BTPHONE
                    bbm_switch_other_device_to_btphone();
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    g_bbm_mode_state = BBM_BT_PHONE_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTDIALER_MODE:
                {
                    bbm_switch_other_device_to_btdialer();
                    g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    g_bbm_mode_state = BBM_BT_DIALER_MODE;
                    TRACE_APP_BBM("BBM mode is already btdialer mode!");
                }
                break;
                case MSG_BBM_SWITCH_TO_BLE_MODE:
                {
                    //断开BTDIALER开BLE
                    bbm_switch_other_device_to_ble();
                    g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                    g_bbm_mode_state = BBM_BLE_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    BbmBTDialerModeProcess(msg_id);
                    break;
            }
        }
        break;
        case BBM_BLE_MODE:
        {
            switch(msg_id)
            {
                case MSG_BBM_CLOSE_ALL_MODE:
                {
                    //1 power on modem
                    //MMIAPIPHONE_PowerOnModem();
                    //关闭所有设备
                    bbm_close_all_device();
                    TRACE_APP_BBM("ble close all mode!\n");
                    g_bbm_mode_state = BBM_MODE_STATE_MAX;
                    //BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_BLE_CLEAR_BOUND_INFO:
                {
                    // stop ble retry timer
                    BbmStopRetryConnectBleTimer();
                    g_bbm_ble_mode_detail_state = BBM_DETAIL_STATE_MAX;
                }
                break;
                case MSG_BBM_SWITCH_TO_AUTO_MODE:
                //获取当前状态，如果是连着ble则进入auto mode的连接态，否则的话连接BLE
                {
                    if(BBM_DETAIL_STATE_MAX == g_bbm_ble_mode_detail_state)
                    {
                        bbm_switch_other_device_to_ble();
                        g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    }
                    else
                    {
                        g_bbm_auto_mode_detail_state = g_bbm_ble_mode_detail_state;
                    }                    
                    g_bbm_mode_state = BBM_AUTO_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_SLAVER_MODE:
                {
                    
                    if(BBM_DETAIL_STATE_MAX == g_bbm_ble_mode_detail_state)
                    {
                        bbm_switch_other_device_to_ble();
                        g_bbm_auto_mode_detail_state = BBM_BLE_CONNECTTING;
                    }
                    else
                    {
                        g_bbm_slaver_mode_detail_state = g_bbm_ble_mode_detail_state;
                    }
                    g_bbm_mode_state = BBM_SLAVER_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTPHONE_MODE:
                {
                    //1 power on modem
                    MMIAPIPHONE_PowerOnModem();
                    //断开BLE开BTPHONE
                    bbm_switch_other_device_to_btphone();
                    g_bbm_btphone_mode_detail_state = BBM_BT_PHONE_OPENNING;
                    g_bbm_mode_state = BBM_BT_PHONE_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BTDIALER_MODE:
                {
                    //断开BLE开BTDIALER
                    bbm_switch_other_device_to_btdialer();
                    g_bbm_btdialer_mode_detail_state = BBM_BT_DIAL_CONNECTTING;
                    g_bbm_mode_state = BBM_BT_DIALER_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                case MSG_BBM_SWITCH_TO_BLE_MODE:
                {               
                    if(BBM_DETAIL_STATE_MAX == g_bbm_ble_mode_detail_state)
                    {
                        bbm_switch_other_device_to_ble();
                        g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                    }
                    else
                    {   
                        bbm_switch_other_device_to_ble();
                        TRACE_APP_BBM("BBM mode is already ble mode!");
                        g_bbm_ble_mode_detail_state = BBM_BLE_CONNECTTING;
                    }                    
                    g_bbm_mode_state = BBM_BLE_MODE;
                    BbmNotifyBbmUser(BBM_MSG_BT_ROLE_SWITCHING,PNULL);
                }
                break;
                default:
                    BbmBleModeProcess(msg_id);
                    break;
            }
        }
        break;
        default:
            TRACE_APP_BBM("state is wrong!");
        break;
    }
}
/*******************************************************************************************/
//  Description : As btphone and bt dial managuage,should provide current bt role,if is not
//                bt phone,it is dialer role,and should use BbmSwitchToBtDialer mode function
//  Param out:BOOLEAN true:is bt phone,false:it is not bt phone
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN BbmCurrentRoleIsBtPhone(void)
{
    BOOLEAN is_bt_phone = FALSE;    
    TRACE_APP_BBM("mode state:%d,detail state:%d\n",g_bbm_mode_state,g_bbm_auto_mode_detail_state);
    
    if(BBM_AUTO_MODE ==g_bbm_mode_state)
    {
        if((BBM_BT_PHONE_OPENNING == g_bbm_auto_mode_detail_state) ||(BBM_BT_PHONE_OPENNED == g_bbm_auto_mode_detail_state))
        {
            is_bt_phone = TRUE;
        }
        else
        {
            is_bt_phone = FALSE;
        }
    }
    else if(BBM_BT_PHONE_MODE == g_bbm_mode_state)
    {
        is_bt_phone = TRUE;
    }
    else if(BBM_MODE_STATE_MAX == g_bbm_mode_state)//关bt时，若当前是phont模式或者auto模式下的phone，需要返回TRUE
    {
        BBM_MODE_E bbm_mode = BBM_MODE_AUTO;
        bbm_mode = Bbm_GetBTMode();
        TRACE_APP_BBM("bbm_mode:%d\n",bbm_mode);
        if(BBM_MODE_PHONE == bbm_mode)
        {
            is_bt_phone = TRUE;
        }
        else if(BBM_MODE_AUTO == bbm_mode)//quto mode下是有开btphone的可能的，当前g_bbm_auto_mode_detail_state是phoneopenning
        {
            if((BBM_BT_PHONE_OPENNING == g_bbm_auto_mode_detail_state) ||(BBM_BT_PHONE_OPENNED == g_bbm_auto_mode_detail_state))
            {
                is_bt_phone = TRUE;
            }
            else
            {
                is_bt_phone = FALSE;
            }
        }
        else
        {
            is_bt_phone = FALSE;
        }
    }
    else
    {
        is_bt_phone = FALSE;
    }
    TRACE_APP_BBM("is_bt_phone:%d\n",is_bt_phone);
    return is_bt_phone;
}
/*******************************************************************************************/
//  Description : use to inform bbm to switch to bt dialer as one slaver role
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBtDialerMode(void)
{
    //    
    //BBM_MODE_E bbm_mode = BBM_MODE_DIALER;
    TRACE_APP_BBM("enter");
    BbmMainProcess(MSG_BBM_SWITCH_TO_BTDIALER_MODE);
    //set nv
    //Bbm_SetBTMode( bbm_mode);
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to inform bbm to switch to ble as one slaver role
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBleMode(void)
{
    //    
    //BBM_MODE_E bbm_mode = BBM_MODE_BLE;
    TRACE_APP_BBM("enter\n");
    BbmMainProcess(MSG_BBM_SWITCH_TO_BLE_MODE);
    //set nv
    //Bbm_SetBTMode( bbm_mode);
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to inform bbm to switch to BtPhone
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBTPhoneMode(void)
{
    //
    //BBM_MODE_E bbm_mode = BBM_MODE_PHONE;
    TRACE_APP_BBM("enter\n");
    BbmMainProcess(MSG_BBM_SWITCH_TO_BTPHONE_MODE);
    //set nv
    //Bbm_SetBTMode( bbm_mode);
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to inform bbm to open auto mode
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToAutoMode(void)
{
    //BBM_MODE_E bbm_mode = BBM_MODE_AUTO;
    TRACE_APP_BBM("enter\n");
    BbmMainProcess(MSG_BBM_SWITCH_TO_AUTO_MODE);
    // set nv
    //Bbm_SetBTMode( bbm_mode);
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to inform bbm to open slaver mode
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToSlaverMode(void)
{
    //BBM_MODE_E bbm_mode = BBM_MODE_SLAVER;
    TRACE_APP_BBM("enter\n");
    BbmMainProcess(MSG_BBM_SWITCH_TO_SLAVER_MODE);
    //set nv
    //Bbm_SetBTMode( bbm_mode);
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to inform bbm auto mode switch to ble
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBle(void)
{
    TRACE_APP_BBM("bbm mode:%d\n",g_bbm_mode_state);
    if(BBM_AUTO_MODE ==g_bbm_mode_state)
    {
        BbmAutoModeProcess(MSG_BBM_SWITCH_TO_BLE);
    }
    else if(BBM_SLAVER_MODE == g_bbm_mode_state)
    {
        BbmSlaverModeProcess(MSG_BBM_SWITCH_TO_BLE);
    }
    else if(BBM_BLE_MODE == g_bbm_mode_state)
    {
        return BBM_RETURN_SUCESS;
    }
    else
    {
        TRACE_APP_BBM("current state is not auto and slaver mode,should not switch to btdialer!\n");
        return BBM_RETURN_MODE_ERROR;
    }
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to inform bbm auto mode switch to bt dialer
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBtDialer(void)
{
    TRACE_APP_BBM("bbm mode:%d\n",g_bbm_mode_state);
    if((BBM_AUTO_MODE ==g_bbm_mode_state) || (BBM_SLAVER_MODE == g_bbm_mode_state)
       ||(BBM_BT_DIALER_MODE == g_bbm_mode_state))
    {
        BbmMainProcess(MSG_BBM_SWITCH_TO_BT_DIALER);
    }
    else
    {
        TRACE_APP_BBM("current state is not auto and slaver and btdialer mode,should not switch to btdialer!\n");
        return BBM_RETURN_MODE_ERROR;
    }
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to inform bbm auto mode switch to bt dialer
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmCloseAllMode(void)
{
    TRACE_APP_BBM("bbm mode:%d\n",g_bbm_mode_state);
    BbmMainProcess(MSG_BBM_CLOSE_ALL_MODE);
}
/*******************************************************************************************/
//  Description : use to get curremt mode
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_CURRENT_DETAIL_STATE BbmGetCurrentDetailState(void)
{
    BBM_CURRENT_DETAIL_STATE current_detail_state = BBM_CURRENT_DETAIL_STATE_MAX;
    TRACE_APP_BBM("enter,mode state:%d,auto mode state:%d,ble mode:%d,bt dialer mode:%d,phone mode:%d,slaver mode:%d\n",
                g_bbm_mode_state,g_bbm_auto_mode_detail_state,g_bbm_ble_mode_detail_state,
                g_bbm_btdialer_mode_detail_state,g_bbm_btphone_mode_detail_state,g_bbm_slaver_mode_detail_state);
    switch(g_bbm_mode_state)
    {
        case BBM_AUTO_MODE:
        {
            if(BBM_BLE_CONNECTTED ==g_bbm_auto_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BLE;
            }
            else if(BBM_BT_DIAL_CONNECTED == g_bbm_auto_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BTDIALER;
            }
            else if(BBM_BT_PHONE_OPENNED == g_bbm_auto_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BTPHONE;
            }
            else
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_IS_SWITCHING;
            }
        }
        break;
        case BBM_SLAVER_MODE:
        {
            if(BBM_BLE_CONNECTTED ==g_bbm_slaver_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BLE;
            }
            else if(BBM_BT_DIAL_CONNECTED == g_bbm_slaver_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BTDIALER;
            }
            else
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_IS_SWITCHING;
            }
        }
        break;
        case BBM_BT_PHONE_MODE:
        {
            if(BBM_BT_PHONE_OPENNED ==g_bbm_btphone_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BTPHONE;
            }
            else
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_IS_SWITCHING;
            }
        }
        break;
        case BBM_BT_DIALER_MODE:
        {
            if(BBM_BT_DIAL_CONNECTED ==g_bbm_btdialer_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BTDIALER;
            }
            else
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_IS_SWITCHING;
            }
        }
        break;
        case BBM_BLE_MODE:
        {
            if(BBM_BLE_CONNECTTED ==g_bbm_ble_mode_detail_state)
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_BLE;
            }
            else
            {
                current_detail_state = BBM_CURRENT_DETAIL_STATE_IS_SWITCHING;
            }
        }
        break;
        case BBM_MODE_STATE_MAX:
        {
            current_detail_state = BBM_CURRENT_DETAIL_STATE_MAX;
        }
        break;
        default:
            TRACE_APP_BBM("cur mode is wrong!\n");
            break;
    }
    TRACE_APP_BBM("return state:%d\n",current_detail_state);
    return current_detail_state;
}
/*******************************************************************************************/
//  Description : bbm user should use this api to reg,especially should reg callback,in order
//                to notify user some information
//  Param out:BOOLEAN true:reg sucess,false reg fail
//  Param in: user_name:it is user string
//  Param in: user_callback:in order to notify user to process some msg
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmRegister(const char* user_name,Bbm_CallBack user_callback )
{
    uint8 user_name_len = 0;
    BOOLEAN is_repeat = FALSE;
    BBM_USER_INFO_T* ptr_bbm_user_info = PNULL;
    TRACE_APP_BBM("Enter\n");
    // 入参判断
    if((PNULL == user_name) || (PNULL == user_callback))
    {
        TRACE_APP_BBM("param is error!\n");
        return BBM_RETURN_PARAM_ERROR;
    }
    if(PNULL != g_bbm_user_list_ptr)
    {
        char temp_user_name[BLE_USER_NAME_LEN] = {0};
        int  user_name_length = strlen(user_name);
        SCI_MEMCPY(temp_user_name,user_name,MIN(user_name_length,BLE_USER_NAME_LEN));
        // 判断当前user是否已经注册了
        is_repeat = JudgeIfUserHasRegister(temp_user_name);
        if(TRUE == is_repeat)
        {        
            TRACE_APP_BBM("user is registered\n");
            return BBM_RETURN_REGISTER_REPEATED;
        }
    }
    else
    {
        TRACE_APP_BBM("there is no use register bbm now!\n");
    }
    user_name_len = strlen(user_name);
    //当前user name的长度判断
    if(user_name_len > BLE_USER_NAME_LEN)
    {
        TRACE_APP_BBM("user_name_len:%d\n",user_name_len);
        return BBM_RETURN_PARAM_ERROR;
    }
    else
    {
        TRACE_APP_BBM("user_name_len:%d\n",user_name_len);
    }
    // malloc当前节点的node
    ptr_bbm_user_info = SCI_ALLOC_APP(sizeof(BBM_USER_INFO_T));
    if(PNULL != ptr_bbm_user_info)
    {
        SCI_MEMSET(ptr_bbm_user_info,0X0,sizeof(BBM_USER_INFO_T));
        //将user name赋值给node节点
        SCI_MEMCPY(ptr_bbm_user_info->user_name,user_name,user_name_len); 
        ptr_bbm_user_info->bbm_user_callback = user_callback;
        
        if(PNULL == g_bbm_user_list_ptr)
        {
            g_bbm_user_list_ptr = MMILINK_CreateHead((uint32)ptr_bbm_user_info);
        }
        else
        {
            MMI_LINK_NODE_T *temp_node_ptr = MMILINK_CreateNode((uint32)ptr_bbm_user_info);
            MMILINK_AddNodeBeforeBaseNode(g_bbm_user_list_ptr,temp_node_ptr);
        }
    }
    else
    {
        TRACE_APP_BBM("malloc ptr_bbm_user_info error!\n");
        return BBM_RETURN_OHTER_ERROR;
    }
    return BBM_RETURN_SUCESS;
}

LOCAL void DestroyUnregisterNode(
                          MMI_LINK_NODE_T const * node_ptr
                          )
{
    BBM_USER_INFO_T *ptr_bbm_user_info = PNULL;

    TRACE_APP_BBM("DestroyUnregisterNode!");
    if (node_ptr == PNULL)
    {
        return;
    }
    ptr_bbm_user_info = (BBM_USER_INFO_T*)node_ptr->data;

    if (ptr_bbm_user_info != PNULL)
    {
        TRACE_APP_BBM("destory unregiser node:%s\n",ptr_bbm_user_info->user_name);
        SCI_FREE(ptr_bbm_user_info);
    }
}

/*******************************************************************************************/
//  Description : bbm user if not want to receive bbm message,should use api deregister
//  Param out:BOOLEAN true:dereg sucess,false dereg fail
//  Param in: user_name:it is user string
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmUnregister(const char* user_name)
{
    MMI_LINK_NODE_T *deregister_user_node_ptr = PNULL;
    
    TRACE_APP_BBM("user_name:%s\n",user_name);
    if(PNULL == user_name)
    {
        TRACE_APP_BBM("user_name is PNULL!\n");
        return BBM_RETURN_PARAM_ERROR;
    }
    deregister_user_node_ptr = MMILINK_SearchNode(g_bbm_user_list_ptr, TRUE, SearchBbmUserInfo, \
        (uint32)user_name, 0); 
    
    if(PNULL != deregister_user_node_ptr)
    {
        g_bbm_user_list_ptr = MMILINK_DestroyNode(deregister_user_node_ptr,g_bbm_user_list_ptr,DestroyUnregisterNode);
    }
    else
    {
        TRACE_APP_BBM("unregister_user_node_ptr!\n");
        return BBM_RETURN_PARAM_ERROR;
    }
    return BBM_RETURN_SUCESS;
}
/*******************************************************************************************/
//  Description : use to abtain current bt is connect to btphone
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN BbmIsConnectWithRemotePhone(void)
{
    BBM_CURRENT_DETAIL_STATE current_state = BBM_CURRENT_DETAIL_STATE_MAX;
    current_state = BbmGetCurrentDetailState();
    if(BBM_CURRENT_DETAIL_STATE_IS_SWITCHING == current_state)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
