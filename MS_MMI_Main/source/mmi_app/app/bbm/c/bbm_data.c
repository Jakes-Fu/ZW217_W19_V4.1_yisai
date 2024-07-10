/****************************************************************************
** File Name:      bbm_data.c                                                *
** Author:                                                                   *
** Date:           01/19/21                                                  *
** Copyright:      2021 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the bbm data part,data is   *
**                 for control service
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/19/21        changli.jiang    create
*****************************************************************************/
#include "mmi_link.h"
#include "mmi_string.h"
#include "bbm_internal.h"
#include "bbm_export.h"
#include "ual_cms_export.h"
#include "ual_bt_common.h"
#include "ual_bt_ble_dev.h"
#include "ual_bt_ble_private.h"
#include "ual_bt_dev.h"
#include "mmicom_trace.h"
#include "mmi_nv.h"
static BOOLEAN g_is_for_bt_switcher = FALSE;// for bt开关时引起的蓝牙开成功失败，排除中间auto模式
//或模式切换过程中引起的打开及关闭消息，方便用户使用
/*******************************************************************************************/
/********************************btdialer api begin*****************************************/
/*******************************************************************************************/
/****************************************************************************/
//  Description : BBM_StartSearchTargetDev
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:搜索目标设备，因为搜索的目标设备是手机，所以将service设为handfree gategway
/****************************************************************************/
PUBLIC void Bbm_StartSearchBoundDevice(void)
{
    uint32 target_service = UAL_BT_SERVICE_HANDFREE_GATEWAY;
    TRACE_APP_BBM("enter");
    if(BBM_MODE_AUTO == Bbm_GetBTMode()) //bug1611714
    {
        ual_bt_dev_start_search(target_service);
    }
    else
    {
        TRACE_APP_BBM("other mode,no need to search");
    }
}

/****************************************************************************/
//  Description : BBM_IsSearchedTargetDev
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:绑定设备是否被搜索到
/****************************************************************************/
PUBLIC BOOLEAN Bbm_BoundDeviceIsSearched(void)
{
    BOOLEAN                                  ret = FALSE;
    ual_bt_dev_info_t       *p_all_searched_list = PNULL;
    uint8                       searched_dev_num = 0;
    uint8 bound_bt_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};

    uint8 i = 0;

    Bbm_GetBTBoundDevAddr(bound_bt_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);

    p_all_searched_list = ual_bt_dev_get_searched_list();
    searched_dev_num = ual_bt_dev_get_searched_num();
    TRACE_APP_BBM("BBM_IsSearchedTargetDev searched_dev_num=%d",searched_dev_num);
    for(i=0;i < searched_dev_num;i++)
    {
        TRACE_APP_BBM("p_all_searched_list[i]->addr,%d,%d,%d,%d,%d,%d",p_all_searched_list[i].addr[0],p_all_searched_list[i].addr[1],p_all_searched_list[i].addr[2],
                                                                       p_all_searched_list[i].addr[3],p_all_searched_list[i].addr[4],p_all_searched_list[i].addr[5]);
        ret = !memcmp(bound_bt_addr, (p_all_searched_list[i].addr), UAL_BT_DEV_ADDRESS_SIZE);
        if(ret)
        {
            break;
        }
    }
    TRACE_APP_BBM("BBM_IsSearchedTargetDev ret=%d",ret);
    return ret;
}
PUBLIC BOOLEAN Bbm_HandleBtDevMsg(ual_cms_msg_t param)
{
    TRACE_APP_BBM("bbm:Bbm_HandleBtDevMsg msg_id:%x\n",param.msg_id);
    switch(param.msg_id)
    {
        case UAL_APP_BT_DEV_PAIR_IND:
        {
            uint8              bt_role = UAL_BT_DIALER;
            bt_role = ual_bt_get_product_role();
            if(UAL_BT_PHONE == bt_role)
            {
                TRACE_APP_BBM("bt phone passive paired!\n");
                //BbmMainProcess(MSG_BBM_BT_PHONE_OPEN_SUCESS);

            }else if(UAL_BT_DIALER == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_DIALER_CONNECTED_SUCCESS);
                BbmNotifyBbmUser(BBM_MSG_BT_CONNECT_DEVICE_SUCESS,PNULL);
            }
        }
        break;
        case UAL_APP_BT_DEV_CLOSED://关闭成功
        {
            uint8              bt_role = UAL_BT_DIALER;
            bt_role = ual_bt_get_product_role();
            if(UAL_BT_PHONE == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_PHONE_CLOSE_SUCESS);

            }else if(UAL_BT_DIALER == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_DIALER_DISCONNECT_SUCESS);
            }
            // TODO: notify user close sucess
            if(TRUE == g_is_for_bt_switcher)
            {
                BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_SUCESS,PNULL);
                g_is_for_bt_switcher = FALSE;
                if(TRUE == Bbm_GetBTOn()) //bug1613044
                {
                    TRACE_APP_BBM("different ");
                    g_is_for_bt_switcher = TRUE;
                    ual_bt_dev_open();
                }
            }
        }
        break;
        case UAL_APP_BT_DEV_CLOSE_ERR://关闭失败
        {
            uint8              bt_role = UAL_BT_DIALER;
            bt_role = ual_bt_get_product_role();
            if(UAL_BT_PHONE == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_PHONE_CLOSE_FAIL);

            }else if(UAL_BT_DIALER == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_DIALER_DISCONNECT_FAIL);
            }
            // TODO:notify user close fail
            if(TRUE == g_is_for_bt_switcher)
            {
                BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_FAIL,PNULL);
                g_is_for_bt_switcher = FALSE;
            }
        }
        break;
        case UAL_APP_BT_DEV_PAIRED:
        {
            TRACE_APP_BBM("classic bt dev paired sucess\n!");
        }
        break;
        case UAL_APP_BT_DEV_PAIR_ERR:
        {
            uint8              bt_role = UAL_BT_DIALER;
            bt_role = ual_bt_get_product_role();
            if(UAL_BT_PHONE == bt_role)
            {
                TRACE_APP_BBM("classic btphone paired error!\n");
                //BbmMainProcess(MSG_BBM_BT_PHONE_OPEN_FAIL);

            }else if(UAL_BT_DIALER == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_DIALER_CONNECTED_FAIL);
            }
        }
        break;
        case UAL_APP_BT_DEV_OPENED://打开bt phone成功及btdialer成功
        {
            uint8              bt_role = UAL_BT_DIALER;
            bt_role = ual_bt_get_product_role();
            if(UAL_BT_PHONE == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_PHONE_OPEN_SUCESS);
            }else if(UAL_BT_DIALER == bt_role)
            {
                BOOLEAN have_connected_before = FALSE;
                uint8 btdialer_dev_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
                uint8 compare_dev_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
                // TODO: judge current is connect
                Bbm_GetBTBoundDevAddr(btdialer_dev_addr,UAL_BT_DEV_ADDRESS_SIZE,TRUE);
                have_connected_before = MMIAPICOM_Stricmp(btdialer_dev_addr,compare_dev_addr);
                if(0 == have_connected_before)
                {
                    TRACE_APP_BBM("there is never connect bt dialer!\n");
                }
                else
                {
                    TRACE_APP_BBM("bt dialer open sucess!\n");
                    // notify user
                    BbmNotifyBbmUser(BBM_MSG_BT_CONNECT_DEVICE_SUCESS,PNULL);
                    BbmMainProcess(MSG_BBM_BT_DIALER_CONNECTED_SUCCESS);

                }
            }
            // TODO:notify user open sucess
            if(TRUE == g_is_for_bt_switcher)
            {
                BbmNotifyBbmUser(BBM_MSG_BT_OPEN_SUCESS,PNULL);
                g_is_for_bt_switcher = FALSE;
                if(FALSE == Bbm_GetBTOn()) //bug1613044，1634509
                {
                    TRACE_APP_BBM("different");
                    g_is_for_bt_switcher = TRUE;
                    ual_bt_dev_close();
                }
            }
        }
        break;
        case UAL_APP_BT_DEV_OPEN_ERR://打开bt phone 失败
        {
            uint8              bt_role = UAL_BT_DIALER;
            bt_role = ual_bt_get_product_role();
            if(UAL_BT_PHONE == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_PHONE_OPEN_FAIL);

            }else if(UAL_BT_DIALER == bt_role)
            {
                BbmMainProcess(MSG_BBM_BT_DIALER_CONNECTED_FAIL);
            }
            // TODO: notify user open fail
            if(TRUE == g_is_for_bt_switcher)
            {
                BbmNotifyBbmUser(BBM_MSG_BT_OPEN_FAIL,PNULL);
                g_is_for_bt_switcher = FALSE;
            }
        }
        break;
        case UAL_APP_BT_DEV_SEARCH_ONE:
        {
            if(BBM_MODE_AUTO == Bbm_GetBTMode()) //bug1904520
            {
                BOOLEAN is_find = FALSE;
                TRACE_APP_BBM("UAL_APP_BT_DEV_SEARCH_ONE");
                is_find = Bbm_BoundDeviceIsSearched();
                if(is_find)
                {
                    TRACE_APP_BBM("find it,cancel search");
                    BbmMainProcess(MSG_BBM_PHONE_SEARCHED);
                    ual_bt_dev_cancel_search();
                }
                else
                {
                    TRACE_APP_BBM("not find it,continue search");
                }
            }
            else
            {
                TRACE_APP_BBM("other mode,no need to search");
            }
        }
            break;
        case UAL_APP_BT_DEV_SEARCH_COMPLETE:
        {
            BOOLEAN is_find = FALSE;
            TRACE_APP_BBM("UAL_APP_BT_DEV_SEARCH_COMPLETE");
            is_find = Bbm_BoundDeviceIsSearched();
            if(is_find)
            {
                TRACE_APP_BBM("find it");
            }
            else
            {
                TRACE_APP_BBM("not find it");
                Bbm_StartSearchBoundDevice();
            }
        }
        break;
        case UAL_APP_BT_DEV_CANCEL_SEARCHED:
        {
            SCI_TRACE_LOW("UAL_APP_BT_DEV_CANCEL_SEARCHED");
        }
        break;
        default:
            TRACE_APP_BBM("msg_id:%d should not process\n",param.msg_id);
            break;
    }
}
/****************************************************************************/
//  Description : Bbm_OpenClassicBtPhone
//  Global resource dependence :
//  Author:changli.jiang
//  Note:open classic BT
/****************************************************************************/
PUBLIC ual_bt_status_e Bbm_OpenClassicBtPhone(void)
{
    BOOLEAN is_bt_open = FALSE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_SUCCESS;
    TRACE_APP_BBM("open classic btphone");
    is_bt_open = Bbm_GetBTOn();
    if(TRUE == is_bt_open)
    {
        bt_status = ual_bt_dev_enable_phone();
    }
    else
    {
        bt_status = UAL_BT_STATUS_ERROR;
    }
    return bt_status;
}
ual_bt_status_e Bbm_OpenClassicBtDialer(void)
{
    BOOLEAN is_bt_open = FALSE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_SUCCESS;
    TRACE_APP_BBM("open classic btdialer");
    is_bt_open = Bbm_GetBTOn();
    if(TRUE == is_bt_open)
    {
        bt_status = ual_bt_dev_enable_dialer();
    }
    else
    {
        bt_status = UAL_BT_STATUS_ERROR;
    }
    return bt_status;
}

/*******************************************************************************************/
/********************************btdialer api end*******************************************/
/*******************************************************************************************/

/*******************************************************************************************/
/********************************ble api begin**********************************************/
/*******************************************************************************************/
uint8 g_bbm_ble_is_active = FALSE;// ble opera is active
#define ADV_INTERVAL_TIME_MAX  32//800
#define ADV_INTERVAL_TIME_MIN  32//800
PUBLIC BOOLEAN Bbm_HandleBleDevMsg(ual_cms_msg_t param)
{ 
    TRACE_APP_BBM("msg_id:%d\n",param.msg_id);
    switch(param.msg_id)
    {
        case UAL_BT_BLE_DEV_MSG_CONFIG_ADV_PARAM_CNF:
        {
            if(PNULL != param.p_body)
            {
                ual_bt_ble_config_adv_param_cnf_t* config_adv_param_return = PNULL;
                config_adv_param_return = (ual_bt_ble_config_adv_param_cnf_t*)param.p_body;
                if(UAL_BT_BLE_DEV_OPERATE_SUCESS == config_adv_param_return->dev_operate_state)
                {
                    TRACE_APP_BBM("config adv param sucess!\n");
                    if(TRUE == g_bbm_ble_is_active)
                    {
                        uint8 temp_len = 0;
                        uint32 i=0;
                        ual_bt_ble_result_e status = UAL_BT_BLE_RESULT_MAX;
                        uint8 name_len = 0;
                        wchar local_name_buffer[UAL_BT_DEV_NAME_MAX_LEN + 1] = {0};
                        uint8 device_name[UAL_BT_DEV_NAME_MAX_LEN + 1] = {0};
                        // ui to set local ble name
                        ual_bt_ble_set_adv_content_t adv_content = {0};
                        //set local name
                        Bbm_GetBTLocalName(local_name_buffer,UAL_BT_DEV_NAME_MAX_LEN);                        
                        name_len = MMIAPICOM_Wstrlen(local_name_buffer);
                        MMIAPICOM_WstrToStr(local_name_buffer, device_name);
                        if(0 == name_len)
                        {
                            TRACE_APP_BBM("name_len = %d",name_len);                            
                        }
                        for(i=0;i<UAL_BT_DEV_NAME_MAX_LEN;i++)
                        {
                            SCI_TRACE_LOW("device_name[%d]:0x%02x\n",i,device_name[i]);
                        }
                        temp_len = MIN(name_len,MAX_ADV_CONTENT_LEN);
                        adv_content.content_type = 0x09;
                        SCI_MEMCPY(adv_content.content,device_name,temp_len);
                        adv_content.content_len = temp_len;
                        status = ual_bt_ble_set_adv_content(&adv_content);
                        if(UAL_BT_BLE_RESULT_SUCESS == status)
                        {
                            TRACE_APP_BBM("begin set adv content!\n");
                        }
                        else
                        {
                            TRACE_APP_BBM("begin set adv content error!\n");
                        }
                    }
                    else
                    {
                        TRACE_APP_BBM("it is not bbm config ble param!\n");
                    }
                }
                else
                {
                    TRACE_APP_BBM("config adv param fail!\n");
                }
            }
            else
            {
                TRACE_APP_BBM("config adv_param return pbody is PNULL");
            }
        }
        break;
        case UAL_BT_BLE_DEV_MSG_SET_ADV_CONTENT_CNF:
        {
            if(PNULL != param.p_body)
            {          
                ual_bt_ble_set_adv_content_cnf_t* set_adv_content_return = PNULL;
                set_adv_content_return = (ual_bt_ble_set_adv_content_cnf_t*)param.p_body;
                if(UAL_BT_BLE_DEV_OPERATE_SUCESS == set_adv_content_return->dev_operate_state)
                {
                    TRACE_APP_BBM("set adv content sucess");
                    if(TRUE == g_bbm_ble_is_active)
                    {
                        uint16 adv_timeout = 0;
                        ual_bt_ble_result_e status = UAL_BT_BLE_RESULT_MAX; 
                        status = ual_bt_ble_start_adv(adv_timeout);
                        if(UAL_BT_BLE_RESULT_SUCESS == status)
                        {
                            TRACE_APP_BBM("begin start adv sucess!\n");
                        }
                        else
                        {
                            TRACE_APP_BBM("begin start adv error!\n");
                        }
                    }
                    else
                    {
                        TRACE_APP_BBM("it is not bbm set adv content!\n");
                    }
                }
                else
                {
                    TRACE_APP_BBM("set adv content fail");
                }
            }
            else
            {
                TRACE_APP_BBM("p_body is PNULL!\n");
            }
        }
        break;
        case UAL_BT_BLE_DEV_MSG_START_ADV_CNF:
        {
            g_bbm_ble_is_active = FALSE;// no matter sucess or fail it should be false
            if(PNULL != param.p_body)
            {                
                ual_bt_ble_start_adv_cnf_t* start_adv_return = PNULL;
                start_adv_return = (ual_bt_ble_set_adv_content_cnf_t*)param.p_body;
                
                if(UAL_BT_BLE_DEV_OPERATE_SUCESS == start_adv_return->dev_operate_state)
                {
                    TRACE_APP_BBM("start adv sucess!");
                    // TODO: send msg to bbm control 
                    // TODO:notify user
                    if(TRUE == g_is_for_bt_switcher)
                    {
                        BbmNotifyBbmUser(BBM_MSG_BT_OPEN_SUCESS,PNULL);
                        g_is_for_bt_switcher = FALSE;
                    }
                    
                }
                else
                {
                    // TODO: send msg to bbm control
                    TRACE_APP_BBM("start adv fail!\n");
                    if(TRUE == g_is_for_bt_switcher)
                    {
                        BbmNotifyBbmUser(BBM_MSG_BT_OPEN_FAIL,PNULL);
                        g_is_for_bt_switcher = FALSE;
                    }
                }
            }
            else
            {
                TRACE_APP_BBM("p_body is PNULL!\n");
            }
        }
        break;
        case UAL_BT_BLE_DEV_MSG_STOP_ADV_CNF:
        {
            // for bbm stop adv cnf means ble disconnect
            ual_bt_ble_stop_adv_cnf_t *stop_adv_cnf = PNULL;
            stop_adv_cnf = (ual_bt_ble_stop_adv_cnf_t*)param.p_body;
            if(UAL_BT_BLE_DEV_OPERATE_SUCESS == stop_adv_cnf->dev_operate_state)
            {
                TRACE_APP_BBM("adv stop sucess mean for bbm it is disconnect sucess!\n");
                //
                BbmMainProcess(MSG_BBM_BLE_DISCONNECTED_SUCESS_CNF);
                if(TRUE == g_is_for_bt_switcher)
                {
                    // notify user
                    BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_SUCESS,PNULL);
                    g_is_for_bt_switcher = FALSE;
                }
            }
            else
            {
                TRACE_APP_BBM("adv stop fail mean for bbm it is disconnect fail!\n");
                BbmMainProcess(MSG_BBM_BLE_DISCONNECTED_FAIL_CNF);                
                // notify user
                if(TRUE == g_is_for_bt_switcher)
                {
                    BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_FAIL,PNULL);
                    g_is_for_bt_switcher = FALSE;
                }
            }
        }
        break;
        case UAL_BT_BLE_DEV_MSG_START_ADV_TIMEOUT_CNF:
        {
            //for bbm it is ble connect fail, if no remote device connect us            
            BbmMainProcess(MSG_BBM_BLE_CONNECT_FAIL);
            TRACE_APP_BBM("there is no one connect and ble connect fail!\n");
            // TODO:这个暂时不通知open,再开广播时已经通知过了
            #if 0
            if(TRUE == g_is_for_bt_switcher)
            {
                BbmNotifyBbmUser(BBM_MSG_BT_OPEN_FAIL,PNULL);
                g_is_for_bt_switcher = FALSE;
            }
            #endif
        }
        break;
        case UAL_BT_BLE_DEV_MSG_ACTIVE_DISCONNECT_CNF:
        {
            // TODO:
            ual_bt_ble_active_disconnect_cnf_t* active_disconnect_cnf = PNULL;
            active_disconnect_cnf = (ual_bt_ble_active_disconnect_cnf_t*)param.p_body;
            if(UAL_BT_BLE_DEV_OPERATE_SUCESS == active_disconnect_cnf->dev_operate_state)
            {
                // ble disconnect
                BbmNotifyBbmUser(BBM_MSG_BT_DEVICE_DISCONNECTED,PNULL);
                BbmMainProcess(MSG_BBM_BLE_DISCONNECTED_SUCESS_CNF);
                TRACE_APP_BBM("ble actvie disconnect sucess!\n");                
                if(TRUE == g_is_for_bt_switcher)
                {
                    BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_SUCESS,PNULL);
                    g_is_for_bt_switcher = FALSE;
                }
            }
            else
            {
                BbmMainProcess(MSG_BBM_BLE_DISCONNECTED_FAIL_CNF);
                TRACE_APP_BBM("ble active disconnect fail!\n");                
                if(TRUE == g_is_for_bt_switcher)
                {
                    BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_FAIL,PNULL);
                    g_is_for_bt_switcher = FALSE;
                }
            }
        }
        break;
        case UAL_BT_BLE_DEV_MSG_PASSIVE_DISCONNECT_CNF:
        {
            // TODO:
            TRACE_APP_BBM("ble is passive disconnect!\n");
            // notify user
            BbmNotifyBbmUser(BBM_MSG_BT_DEVICE_DISCONNECTED,PNULL);
            BbmMainProcess(MSG_BBM_BLE_PASSIVE_DISCONNECTED);            
            if(TRUE == g_is_for_bt_switcher)
            {
                BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_SUCESS,PNULL);
                g_is_for_bt_switcher = FALSE;
            }
        }
        break;
        case UAL_BT_BLE_DEV_MSG_SET_ADV_POLICY_CNF:
        {
            if(PNULL != param.p_body)
            {
                ual_bt_ble_result_e status = UAL_BT_BLE_RESULT_MAX;
                ual_bt_ble_set_adv_policy_cnf_t* adv_policy_cnf = PNULL;
                adv_policy_cnf = (ual_bt_ble_set_adv_policy_cnf_t*)param.p_body;
                if(UAL_BT_BLE_DEV_OPERATE_SUCESS == adv_policy_cnf->dev_operate_state)
                {
                    // first config param
                    ual_bt_ble_adv_param_t adv_param = {0};
                    adv_param.adv_adv_filter_policy = UAL_BT_BLE_ALLOW_SCAN_CONNECT_ANY;
                    adv_param.adv_display_mode = UAL_BT_BLE_NORMAL_DISCOVERABLE_MODE;
                    adv_param.adv_mode = UAL_BT_BLE_UNDIRECT_SCAN_CONNET_MODE;
                    adv_param.adv_interval_min = ADV_INTERVAL_TIME_MIN;
                    adv_param.adv_interval_max = ADV_INTERVAL_TIME_MAX;
                    status = ual_bt_ble_config_adv_param(&adv_param);
                    if(UAL_BT_BLE_RESULT_SUCESS == status)
                    {
                        TRACE_APP_BBM("begin config adv param sucess!\n");
                    }
                    else
                    {
                        TRACE_APP_BBM("begin config adv param error status:%d\n",status);
                    }
                }
                else
                {
                    TRACE_APP_BBM("set adv policy fail!\n");
                }
            }
            else
            {
                TRACE_APP_BBM("p_body is PNULL!\n");
            }
        }
        break;
        case UAL_BT_BLE_DEV_MSG_REPORT_PAIR_STATUS_CNF:
        break;
        case UAL_BT_BLE_DEV_MSG_CLEAR_BOUND_INFO_CNF:
        {
            Bbm_ClearBTBoundDevAddr(FALSE);
            BbmNotifyBbmUser(BBM_MSG_BT_BLE_CLEAR_BOUND_SUCESS,PNULL);
            if(TRUE == g_is_for_bt_switcher)
            {
                // TODO:此分支暂时不会走到
                BbmNotifyBbmUser(BBM_MSG_BT_CLOSE_SUCESS,PNULL);
                g_is_for_bt_switcher = FALSE;
            }
            BbmMainProcess(MSG_BBM_BLE_CLEAR_BOUND_INFO);
        }
        break;
        case UAL_BT_BLE_DEV_MSG_PASSIVE_CONNECT_CNF://被动连接
        {
           uint8 address[MAX_BLE_ADDRESS_LEN] = {0};
           ual_bt_ble_passive_connect_cnf_t *passive_connect_cnf = PNULL;

           passive_connect_cnf = (ual_bt_ble_passive_connect_cnf_t*)param.p_body;
           SCI_MEMCPY(address,passive_connect_cnf->remote_address.address,MAX_BLE_ADDRESS_LEN);
           //set nv
           Bbm_SetBTBoundDevAddr(address,FALSE);
           //
           BbmNotifyBbmUser(BBM_MSG_BT_CONNECT_DEVICE_SUCESS,PNULL);
           //nodify user ble connect sucess
           BbmNotifyBbmUser(BBM_MSG_BT_BLE_CONNECT_SUCESS,PNULL);
           BbmMainProcess(MSG_BBM_BLE_CONNECT_SUCCESS);
        }
        break;
        case UAL_BT_BLE_DEV_MSG_REPORT_REMOTE_DEVICE_NAME_CNF:
        {
            wchar device_name[MAX_DEVICE_NAME_LEN] ={0};
            ual_bt_ble_remote_device_name_cnf_t* premote_device_name = PNULL;

            premote_device_name = (ual_bt_ble_remote_device_name_cnf_t*)param.p_body;
            MMIAPICOM_StrToWstr(premote_device_name->remote_device_name,device_name);
            Bbm_SetBTBoundDevName(device_name,premote_device_name->remote_device_name_len);

            //notiy ble name for app
            BbmNotifyBbmUser(BBM_MSG_BT_REPORT_BLE_NAME,PNULL);
        }
        break;
        default:            
            SCI_TRACE_LOW("bbm:Bbm_HandleBleDevMsg msg_id:%d should not process\n",param.msg_id);
            break;
    }
}
PUBLIC BOOLEAN Bbm_HandleBlePrivateMsg(ual_cms_msg_t param)
{
    TRACE_APP_BBM("msg_id:%d\n",param.msg_id);
    switch(param.msg_id)
    {
        case UAL_BT_BLE_PRIVATE_MSG_RECEIVE_DATA_CNF:
        {
            uint32 i=0;
            ual_bt_ble_private_message_data_t* private_message = (ual_bt_ble_private_message_data_t*)param.p_body;
            if( UAL_BT_BLE_PRIVATE_INCALL == private_message->msg_type)
            {
                TRACE_APP_BBM("data is incomming call!\n");
                // bbm in comming call
                BbmMainProcess(MSG_BBM_BLE_INCOMMING_CALL);
            }
            else
            {
                TRACE_APP_BBM("bbm should not process current private data!\n");
            }
        }
        break;
        default:            
            TRACE_APP_BBM("msg_id:%d should not process\n",param.msg_id);
            break;
    }
}
/*******************************************************************************************/
//  Description : api of connect ble
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_connect_ble(void)
{
    ual_bt_ble_state_e current_ble_state = UAL_BT_BLE_MAX;
    ual_bt_ble_result_e status = UAL_BT_BLE_RESULT_MAX;
    BOOLEAN is_need_adv = FALSE;
    BOOLEAN is_bt_open = FALSE;
    is_bt_open = Bbm_GetBTOn();
    if(FALSE == is_bt_open)
    {
        return FALSE;
    }
    else
    {
        TRACE_APP_BBM("bt is open!\n");
    }    
    current_ble_state = ual_bt_ble_get_current_ble_status();
    if(UAL_BT_BLE_ADERVISING == current_ble_state)
    {
        TRACE_APP_BBM("begin advering sucess!\n");
        return TRUE;
    }
    else if(UAL_BT_BLE_CONNECTED == current_ble_state)
    {
        BbmMainProcess(MSG_BBM_BLE_CONNECT_SUCCESS);
        return TRUE;
    }
    else if(UAL_BT_BLE_BUSY == current_ble_state)
    {
        // TODO:看测试情况，如果不行，我需要起timer开启BLE
        TRACE_APP_BBM("ble state is busy!\n");
        return FALSE;
    }
    else
    {
        TRACE_APP_BBM("ble status is idle,need start adv!\n");
    }
    // set basic policy before start adv
    status = ual_bt_ble_set_auto_adv(is_need_adv);
    if(UAL_BT_BLE_RESULT_SUCESS == status)
    {
        g_bbm_ble_is_active = TRUE;
        TRACE_APP_BBM("begin set auto adv sucess!");
        return TRUE;
    }
    else
    {
        g_bbm_ble_is_active = FALSE;
        TRACE_APP_BBM("begin set auto adv fail!");
        return FALSE;
    }
}
/*******************************************************************************************/
//  Description : api of disconnect ble
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_disconnect_ble(void)
{
    ual_bt_ble_state_e current_ble_state = UAL_BT_BLE_MAX;
    ual_bt_ble_result_e ble_disconnect_ret = UAL_BT_BLE_RESULT_MAX;
    ual_bt_ble_result_e ble_stop_adv = UAL_BT_BLE_RESULT_MAX;
    TRACE_APP_BBM("bbm begin disconnect ble");
    current_ble_state = ual_bt_ble_get_current_ble_status();
    if(UAL_BT_BLE_ADERVISING == current_ble_state)
    {
        ble_stop_adv = ual_bt_ble_stop_adv();
        if(UAL_BT_BLE_RESULT_SUCESS == ble_stop_adv)
        {
            TRACE_APP_BBM("begin disconnect ble sucess");
            return TRUE;
        }else
        {
            TRACE_APP_BBM("begin disconnect ble fail");
            return FALSE;
        }
    }else if(UAL_BT_BLE_CONNECTED == current_ble_state)
    {
        ble_disconnect_ret = ual_bt_ble_disconnect_ble();
        if(UAL_BT_BLE_RESULT_SUCESS == ble_disconnect_ret)
        {
            TRACE_APP_BBM("begin disconnect ble sucess");
            return TRUE;
        }else
        {
            TRACE_APP_BBM("begin disconnect ble fail");
            return FALSE;
        }
    }
    else if(UAL_BT_BLE_BUSY == current_ble_state)
    {
        // TODO:看测试情况，如果不行，我需要起timer关闭
        TRACE_APP_BBM("ble state is busy!\n");
        return FALSE;
    }
    else
    {
        TRACE_APP_BBM("ble state is idle Not Need to close!\n");
        return TRUE;
    }
}
/*******************************************************************************************/
/********************************ble api end************************************************/
/*******************************************************************************************/

/*******************************************************************************************/
/********************************common api begin*******************************************/
/*******************************************************************************************/
uint32 g_ble_dev_handle = 0;//接收ble dev模块的消息handle
uint32 g_ble_private_handle = 0;//接收ble private 模块的消息handle
uint32 g_bt_dev_handle = 0;//接收bt dev 模块的消息handle
/*******************************************************************************************/
//  Description : open bt
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC void BbmOpenBt(void)
{
    //switch auto mode
    //set nv    
    BBM_MODE_E bt_mode = BBM_MODE_AUTO;
    BOOLEAN is_bt_on = TRUE;
    TRACE_APP_BBM("open bt and switch to auto mode!\n");
    g_is_for_bt_switcher = TRUE;
    Bbm_SetBTOn(is_bt_on);
    bt_mode = Bbm_GetBTMode();
    if(BBM_MODE_AUTO == bt_mode)
    {
        BbmSwitchToAutoMode();
    }
    else if(BBM_MODE_SLAVER == bt_mode)
    {
        BbmSwitchToSlaverMode();
    }
    else if(BBM_MODE_BLE == bt_mode)
    {
        BbmSwitchToBleMode();
    }
    else if(BBM_MODE_DIALER == bt_mode)
    {
        BbmSwitchToBtDialerMode();
    }
    else if(BBM_MODE_PHONE == bt_mode)
    {
        BbmSwitchToBTPhoneMode();
    }
    else
    {
        TRACE_APP_BBM("bbm mode is wrong!\n");
    }
}
/*******************************************************************************************/
//  Description : close bt
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC void BbmCloseBt(void)
{    
    BOOLEAN is_bt_on = FALSE;
    TRACE_APP_BBM("close bt!\n");    
    Bbm_SetBTOn(is_bt_on);
    g_is_for_bt_switcher = TRUE;
    BbmCloseAllMode();
}
/*******************************************************************************************/
//  Description : disconnect ble and connet bt dialer
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_ble_to_btdailer(void)
{
    BOOLEAN ret = FALSE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_MAX;
    ret = bbm_disconnect_ble();
    if(TRUE == ret)
    {
        TRACE_APP_BBM("begin disconnect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin disconnect ble fail!");
    }
    // TODO:open bt dialer
    bt_status = Bbm_OpenClassicBtDialer();
    if(UAL_BT_STATUS_SUCCESS == bt_status)
    {
        // bbm connect sucess
        BbmMainProcess(MSG_BBM_BT_DIALER_CONNECTED_SUCCESS);
        TRACE_APP_BBM("begin enable btdialer sucess!\n");
    }
    else if(UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin enable btdialer pending for msg!");
    }
    else
    {
        TRACE_APP_BBM("begin enable btdialer fail!\n");
    }
    return TRUE;
}
/*******************************************************************************************/
//  Description : disconnect bt dialer and connet ble
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_btdialer_to_ble(void)
{
    BOOLEAN ble_ret = FALSE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_MAX;
    // TODO:暂时不判断接口返回值
    //关闭btdialer
    bt_status = ual_bt_dev_close();
    if(UAL_BT_STATUS_SUCCESS == bt_status)
    {
        TRACE_APP_BBM("begin stop btdialer sucess!\n");
    }
    else if(UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin stop btdialer pending for msg!\n");
    }
    else
    {
        TRACE_APP_BBM("begin stop btdialer fail!\n");
    }
    //开启ble
    ble_ret = bbm_connect_ble();
    if(TRUE == ble_ret)
    {
        TRACE_APP_BBM("begin connect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin connect ble fail!");
    }
    return TRUE;
}
/*******************************************************************************************/
//  Description : disconnect ble and connect bt phone
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_ble_to_btphone(void)
{
    BOOLEAN ret = FALSE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_MAX;
    ret = bbm_disconnect_ble();
    if(TRUE == ret)
    {
        TRACE_APP_BBM("begin  disconnect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin disconnect ble fail!");
    }
    // TODO:open bt phone
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
    return TRUE;
}
/*******************************************************************************************/
//  Description : disconnect btphone and connect ble
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_btphone_to_ble(void)
{
    BOOLEAN ble_ret = FALSE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_MAX;
    // TODO:暂时不判断接口返回值
    //关闭btphone
    bt_status = ual_bt_dev_close();
    if((UAL_BT_STATUS_SUCCESS == bt_status))
    {
        TRACE_APP_BBM("begin stop btphone sucess!\n");
    }
    else if(UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin stop btphone pending for msg!\n");
    }
    else
    {
        TRACE_APP_BBM("begin stop btphone fail!\n");
    }
    //开启ble
    ble_ret = bbm_connect_ble();
    if(TRUE == ble_ret)
    {
        TRACE_APP_BBM("begin connect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin connect ble fail!");
    }
    return TRUE;
}
/*******************************************************************************************/
//  Description : disconnect btphone and bt dialer and connect ble
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_other_device_to_ble(void)
{
    BOOLEAN ble_ret = TRUE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_MAX;
    // TODO:暂时不判断接口返回值
    // close btdialer
    // close bt phone
    bt_status = ual_bt_dev_close();//此接口是关闭经典bt的任何设备
    if((UAL_BT_STATUS_SUCCESS == bt_status) || (UAL_BT_STATUS_PENDING == bt_status))
    {
        TRACE_APP_BBM("begin stop classic bt sucess!\n");
    }
    else
    {
        TRACE_APP_BBM("begin stop classic bt fail!\n");
    }
    //开启ble
    ble_ret = bbm_connect_ble();
    if(TRUE == ble_ret)
    {
        TRACE_APP_BBM("begin connect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin connect ble fail!");
        ble_ret = FALSE;
    }
    return ble_ret;
}
/*******************************************************************************************/
//  Description : disconnect btphone ble and connect bt dialer
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_other_device_to_btdialer(void)
{
    BOOLEAN ret = FALSE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_SUCCESS;
    ret = bbm_disconnect_ble();
    if(TRUE == ret)
    {
        TRACE_APP_BBM("begin disconnect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin disconnect ble fail!");
    }
    ual_bt_dev_disable_phone();
    //bt_status = ual_bt_dev_close();//此接口是关闭经典bt的任何设备
    #if 0
    if(UAL_BT_STATUS_SUCCESS == bt_status)
    {
        TRACE_APP_BBM("begin close classic bt sucess!\n");
    }
    else if (UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin close classic bt pending for msg");
    }
    else
    {
        TRACE_APP_BBM("begin close classic bt fail!\n");
    }
#endif
    // TODO:open bt dialer
    bt_status = Bbm_OpenClassicBtDialer();
    if(UAL_BT_STATUS_SUCCESS == bt_status)
    {
        //bbm connect sucess        
        BbmMainProcess(MSG_BBM_BT_DIALER_CONNECTED_SUCCESS);
        TRACE_APP_BBM("begin enable bt dialer sucess!\n");
    }
    else if(UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin enable bt dilaer pending for msg\n");
    }
    else
    {
        TRACE_APP_BBM("begin enable bt dialer fail!\n");
    }
    return TRUE;
}
/*******************************************************************************************/
//  Description : disconnect ble and btidaler and connect btphone
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_other_device_to_btphone(void)
{
    BOOLEAN ret = TRUE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_SUCCESS;
    ret = bbm_disconnect_ble();
    if(TRUE == ret)
    {
        TRACE_APP_BBM("begin disconnect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin disconnect ble fail!");
    }
    ual_bt_dev_disable_dialer();
    //bt_status = ual_bt_dev_close();//此接口是关闭经典bt的任何设备
    #if 0//pclinck
    if(UAL_BT_STATUS_SUCCESS == bt_status)
    {
        TRACE_APP_BBM("begin close classic bt sucess!\n");
    }
    else if(UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin close classic bt pending for msg");
    }
    else
    {
        TRACE_APP_BBM("begin close classic bt fail!\n");
    }
    #endif
    // open bt phone
    bt_status = Bbm_OpenClassicBtPhone();
    if(UAL_BT_STATUS_SUCCESS == bt_status)
    {
        // notify bbm
        BbmMainProcess(MSG_BBM_BT_PHONE_OPEN_SUCESS);
        TRACE_APP_BBM("begin enable bt phone sucess!\n");
    }
    else if(UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin enable bt phone pending for msg!\n");
    }
    else
    {
        TRACE_APP_BBM("begin enable bt phone fail!\n");
        ret = FALSE;
    }
    return ret;
}
/*******************************************************************************************/
//  Description : close all device
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_close_all_device(void)
{
    BOOLEAN ret = TRUE;
    ual_bt_status_e bt_status = UAL_BT_STATUS_MAX;    
    //close ble
    //close bt dialer
    //close bt phone
    ret = bbm_disconnect_ble();
    if(TRUE == ret)
    {
        TRACE_APP_BBM("begin disconnect ble sucess!");
    }
    else
    {
        TRACE_APP_BBM("begin disconnect ble fail!");
    }
    bt_status = ual_bt_dev_close();//此接口是关闭经典bt的任何设备
    if(UAL_BT_STATUS_SUCCESS == bt_status)
    {
        TRACE_APP_BBM("begin close classic bt sucess!\n");
    }
    else if(UAL_BT_STATUS_PENDING == bt_status)
    {
        TRACE_APP_BBM("begin close classic bt pending for msg");
    }
    else
    {
        TRACE_APP_BBM("begin close classic bt fail!\n");
    }
}
// TODO: 1:auto mode's detail state 2: get current mode
/*******************************************************************************************/
//  Description : use to init bbm module this init should be after ual init
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmInit(void)
{
    ual_bt_ble_private_result_e ble_private_ret = UAL_BT_BLE_PRIVATE_RESULT_START;
    ual_bt_ble_result_e ble_dev_ret = UAL_BT_BLE_RESULT_START;
    BOOLEAN  bt_dev_ret = FALSE;
    BBM_MODE_E bt_mode = BBM_MODE_AUTO;
    TRACE_APP_BBM("bbm:BbmInit");
    //注册bledev 接口
    ble_dev_ret = ual_bt_ble_dev_register(Bbm_HandleBleDevMsg,&g_ble_dev_handle);
    if(UAL_BT_BLE_RESULT_SUCESS == ble_dev_ret)
    {
        TRACE_APP_BBM("bbm:register ble dev sucess!\n");
    }
    else
    {
        TRACE_APP_BBM("bbm:register ble dev fail!\n");
    }
    //注册private接口
    ble_private_ret = ual_bt_ble_private_register(Bbm_HandleBlePrivateMsg,&g_ble_private_handle);
    if(UAL_BT_BLE_PRIVATE_RESULT_SUCESS == ble_private_ret)
    {
        TRACE_APP_BBM("bbm:register ble private sucess!\n");
    }
    else
    {
        TRACE_APP_BBM("bbm:register ble private fail!\n");
    }
    //注册bt dev接口    
    bt_dev_ret = ual_bt_dev_register(Bbm_HandleBtDevMsg,&g_bt_dev_handle);   
    if(TRUE == bt_dev_ret)
    {
        TRACE_APP_BBM("bbm:register bt dev sucess!\n");
    }
    else
    {
        TRACE_APP_BBM("bbm:register bt dev fail!\n");
    }    
    // TODO:begin slaver mode
    bt_mode = Bbm_GetBTMode();
    if(BBM_MODE_AUTO == bt_mode)
    {
        BbmSwitchToAutoMode();
    }
    else if(BBM_MODE_SLAVER == bt_mode)
    {
        BbmSwitchToSlaverMode();
    }
    else if(BBM_MODE_BLE == bt_mode)
    {
        BbmSwitchToBleMode();
    }
    else if(BBM_MODE_DIALER == bt_mode)
    {
        BbmSwitchToBtDialerMode();
    }
    else if(BBM_MODE_PHONE == bt_mode)
    {
        BbmSwitchToBTPhoneMode();
    }
    else
    {
        TRACE_APP_BBM("bbm mode is wrong!\n");
    }
}
/*******************************************************************************************/
/********************************common api end*********************************************/
/*******************************************************************************************/

/*******************************************************************************************/
/********************************nv api begin***********************************************/
/*******************************************************************************************/

/****************************************************************************/
//  Description : Bbm_GetBTOn
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取蓝牙开关状态，返回值为TRUE,蓝牙打开，返回值为FALSE,蓝牙关闭
/****************************************************************************/
PUBLIC BOOLEAN Bbm_GetBTOn(void)
{
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
    BOOLEAN                     is_on = FALSE;

    MMINV_READ(WACHT_BT_ONOFF,&is_on,return_value);
    TRACE_APP_BBM("return_value=%d",return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(WACHT_BT_ONOFF,&is_on);
    }
    TRACE_APP_BBM("is_on=%d",is_on);
    return is_on;
}

/****************************************************************************/
//  Description : Bbm_SetBTOn
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置蓝牙开关状态，TRUE,蓝牙打开，FALSE,蓝牙关闭
/****************************************************************************/
PUBLIC void Bbm_SetBTOn(BOOLEAN is_on)
{
    TRACE_APP_BBM("is_on=%d",is_on);
    MMINV_WRITE(WACHT_BT_ONOFF,&is_on);
}
/****************************************************************************/
//  Description : Bbm_GetBTMode
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取蓝牙模式，共有auto/slaver/ble/dialer/phone 五种模式
/****************************************************************************/
PUBLIC BBM_MODE_E Bbm_GetBTMode(void)
{
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
    BBM_MODE_E                   mode = BBM_MODE_AUTO;

    MMINV_READ(WATCHBBM_MODE,&mode,return_value);
    TRACE_APP_BBM("return_value=%d",return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        mode = BBM_MODE_PHONE;
        MMINV_WRITE(WATCHBBM_MODE,&mode);
    }
    TRACE_APP_BBM("mode=%d",mode);
    return mode;
}

/****************************************************************************/
//  Description : Bbm_SetBTMode
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置蓝牙模式，共有auto/slaver/ble/dialer/phone 五种模式
/****************************************************************************/
PUBLIC void Bbm_SetBTMode(BBM_MODE_E mode)
{
    TRACE_APP_BBM("mode=%d",mode);
    MMINV_WRITE(WATCHBBM_MODE,&mode);
}
/****************************************************************************/
//  Description : Bbm_SetBTBoundDevName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:写绑定设备名称
/****************************************************************************/
PUBLIC void Bbm_SetBTBoundDevName(wchar *p_name,uint16 name_len)
{
    BBM_BOUND_REMOTE_DEV_INFO_T bound_dev_info = {0};
    MN_RETURN_RESULT_E        return_value = MN_RETURN_FAILURE;

    if(PNULL == p_name)
    {
        TRACE_APP_BBM("p_name is null");
        return;
    }    
    //first read nv
    MMINV_READ(WATCH_BOUND_DEVICE_INFO,&bound_dev_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&bound_dev_info,0,sizeof(BBM_BOUND_REMOTE_DEV_INFO_T)); 
        MMINV_WRITE(WATCH_BOUND_DEVICE_INFO,&bound_dev_info);
    }
    //then write
    TRACE_APP_BBM("name_len=%d",name_len);

    SCI_MEMSET(bound_dev_info.bt_name,0,sizeof(wchar)*UAL_BT_DEV_NAME_MAX_LEN);//bug1620796
    MMIAPICOM_Wstrncpy(bound_dev_info.bt_name,p_name,MIN(name_len,UAL_BT_DEV_NAME_MAX_LEN));

    MMINV_WRITE(WATCH_BOUND_DEVICE_INFO,&bound_dev_info);
}

/****************************************************************************/
//  Description : Bbm_SetBTBoundDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:写绑定设备地址的信息，调用该接口时，传入bt name和地址(因为远端设备经典和BLE地址不一样，但是名称一样)
/****************************************************************************/
PUBLIC void Bbm_SetBTBoundDevAddr(uint8 *p_addr,BOOLEAN is_classic)
{
    BBM_BOUND_REMOTE_DEV_INFO_T bound_dev_info = {0};
    uint16    remote_addr_len = 0;
    MN_RETURN_RESULT_E        return_value = MN_RETURN_FAILURE;

    if(PNULL == p_addr)
    {
        TRACE_APP_BBM("p_addr is null");
        return;
    }
    //first read nv
    MMINV_READ(WATCH_BOUND_DEVICE_INFO,&bound_dev_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&bound_dev_info,0,sizeof(BBM_BOUND_REMOTE_DEV_INFO_T)); 
        MMINV_WRITE(WATCH_BOUND_DEVICE_INFO,&bound_dev_info);
    }
    //then set addr
    remote_addr_len = strlen(p_addr);
    TRACE_APP_BBM("remote_addr_len=%d",remote_addr_len);

    if(TRUE == is_classic)
    {
        SCI_MEMCPY(bound_dev_info.classic_addr,p_addr,MIN(remote_addr_len,UAL_BT_DEV_ADDRESS_SIZE));
    }
    else
    {
        SCI_MEMCPY(bound_dev_info.ble_addr,p_addr,MIN(remote_addr_len,MAX_BLE_ADDRESS_LEN));
    }

    MMINV_WRITE(WATCH_BOUND_DEVICE_INFO,&bound_dev_info);
}
/****************************************************************************/
//  Description:Bbm_ClearBTBoundDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:清除对端蓝牙地址，is_classic = TRUE为清除经典蓝牙地址，is_classic = FALSE
//  清除对端BLE地址
/****************************************************************************/
PUBLIC void Bbm_ClearBTBoundDevAddr(BOOLEAN is_classic)
{
    BBM_BOUND_REMOTE_DEV_INFO_T bound_dev_info = {0};
    MN_RETURN_RESULT_E        return_value = MN_RETURN_FAILURE;

    //first read nv
    MMINV_READ(WATCH_BOUND_DEVICE_INFO,&bound_dev_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&bound_dev_info,0,sizeof(BBM_BOUND_REMOTE_DEV_INFO_T)); 
        MMINV_WRITE(WATCH_BOUND_DEVICE_INFO,&bound_dev_info);
    }

    TRACE_APP_BBM("is classic = %d",is_classic);
    if(TRUE == is_classic)
    {
        SCI_MEMSET(bound_dev_info.classic_addr,0,UAL_BT_DEV_ADDRESS_SIZE);
    }
    else
    {
        SCI_MEMSET(bound_dev_info.ble_addr,0,MAX_BLE_ADDRESS_LEN);
    }
    TRACE_APP_BBM("class addr= %x,%x,%x,%x,%x,%x",bound_dev_info.classic_addr[0],bound_dev_info.classic_addr[1],bound_dev_info.classic_addr[2],
                                                  bound_dev_info.classic_addr[3],bound_dev_info.classic_addr[4],bound_dev_info.classic_addr[5]);
    TRACE_APP_BBM("ble addr= %x,%x,%x,%x,%x,%x",bound_dev_info.ble_addr[0],bound_dev_info.ble_addr[1],bound_dev_info.ble_addr[2],
                                                bound_dev_info.ble_addr[3],bound_dev_info.ble_addr[4],bound_dev_info.ble_addr[5]);

    MMINV_WRITE(WATCH_BOUND_DEVICE_INFO,&bound_dev_info);
}
/****************************************************************************/
//  Description : Bbm_GetBTBoundDevInfo
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取绑定设备信息的接口，包括设备名称，设备ble 地址和classic地址
/****************************************************************************/
PUBLIC void Bbm_GetBTBoundDevInfo(BBM_BOUND_REMOTE_DEV_INFO_T *p_dev_info)
{
    MN_RETURN_RESULT_E        return_value = MN_RETURN_FAILURE;

    BBM_BOUND_REMOTE_DEV_INFO_T bound_dev_info = {0};

    if(PNULL == p_dev_info)
    {
        TRACE_APP_BBM("p_conn_addr is null");
        return;
    }

    MMINV_READ(WATCH_BOUND_DEVICE_INFO,&bound_dev_info,return_value);
    TRACE_APP_BBM("return_value=%d",return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&bound_dev_info,0,sizeof(BBM_BOUND_REMOTE_DEV_INFO_T)); 
        MMINV_WRITE(WATCH_BOUND_DEVICE_INFO,&bound_dev_info);
    }

    MMIAPICOM_Wstrncpy(p_dev_info->bt_name,bound_dev_info.bt_name,UAL_BT_DEV_NAME_MAX_LEN);
    SCI_MEMCPY(p_dev_info->classic_addr,bound_dev_info.classic_addr,UAL_BT_DEV_ADDRESS_SIZE);
    SCI_MEMCPY(p_dev_info->ble_addr,bound_dev_info.ble_addr,MAX_BLE_ADDRESS_LEN);
}
/****************************************************************************/
//  Description : Bbm_GetBTBoundDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取绑定设备信息地址,is_classic:TRUE,获取经典蓝牙地址，FALSE,获取BLE蓝牙地址
/****************************************************************************/
PUBLIC void Bbm_GetBTBoundDevAddr(uint8 *p_addr,uint16 addr_len,BOOLEAN is_classic)
{
    MN_RETURN_RESULT_E                    return_value = MN_RETURN_FAILURE;

    BBM_BOUND_REMOTE_DEV_INFO_T         bound_dev_info = {0};

    if(PNULL == p_addr)
    {
        TRACE_APP_BBM("p_conn_addr  param is null");
        return;
    }
    TRACE_APP_BBM("addr_len=%d",addr_len);
    MMINV_READ(WATCH_BOUND_DEVICE_INFO,&bound_dev_info,return_value);
    TRACE_APP_BBM("return_value=%d",return_value);

    if(TRUE == is_classic)
    {
        SCI_MEMCPY(p_addr,bound_dev_info.classic_addr,MIN(addr_len,UAL_BT_DEV_ADDRESS_SIZE));
    }
    else
    {
        SCI_MEMCPY(p_addr,bound_dev_info.ble_addr,MIN(addr_len,MAX_BLE_ADDRESS_LEN));
    }
    TRACE_APP_BBM("bound addr=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",p_addr[0],p_addr[1],p_addr[2],
                                                             p_addr[3],p_addr[4],p_addr[5]);
}
/****************************************************************************/
//  Description : Bbm_GetBTBoundDevName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取绑定设备名称
/****************************************************************************/
PUBLIC void Bbm_GetBTBoundDevName(wchar *p_name,uint16 name_len)
{
    MN_RETURN_RESULT_E           return_value = MN_RETURN_FAILURE;

    BBM_BOUND_REMOTE_DEV_INFO_T bound_dev_info = {0};

    uint16                    remote_name_len = 0;

    if(PNULL == p_name)
    {
        TRACE_APP_BBM("p_name is null");
        return;
    }
    TRACE_APP_BBM("name_len = %d",name_len);

    MMINV_READ(WATCH_BOUND_DEVICE_INFO,&bound_dev_info,return_value);
    TRACE_APP_BBM("return_value=%d",return_value);
    remote_name_len = MMIAPICOM_Wstrlen(bound_dev_info.bt_name);
    TRACE_APP_BBM("remote_name_len = %d",remote_name_len);
    MMIAPICOM_Wstrncpy(p_name,bound_dev_info.bt_name,MIN(remote_name_len,name_len));
}

/****************************************************************************/
//  Description : Bbm_GetBTLocalName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取BLE local device name,若开启经典蓝牙，ble和经典蓝牙一致，关闭经典蓝牙 
/****************************************************************************/
PUBLIC void Bbm_GetBTLocalName(wchar *p_name,uint16 name_len)
{
    MN_RETURN_RESULT_E         return_value = MN_RETURN_FAILURE;
    wchar bt_name[UAL_BT_DEV_NAME_MAX_LEN] = {0};
    uint16                   local_name_len = 0;
    if(PNULL == p_name)
    {
        TRACE_APP_BBM("p_name is null");
        return;
    }
    TRACE_APP_BBM("name_len = %d",name_len);

    MMINV_READ(WATCH_BT_LOCAL_NAME,bt_name,return_value);
    if(MN_RETURN_FAILURE == return_value)
    {
        //读失败默认用6130E
        uint8 local_name[] =
        {
            '6', '1', '3', '0', 'E','\0',
        };
        TRACE_APP_BBM("read local name fail!");
        MMIAPICOM_StrToWstr(local_name,bt_name);
        Bbm_SetBTLocalName(bt_name);
        ual_bt_dev_set_local_name(bt_name);//bug1606618
    }
    local_name_len = MMIAPICOM_Wstrlen(bt_name);
    TRACE_APP_BBM("local_name_len = %d",local_name_len);
    MMIAPICOM_Wstrncpy(p_name,bt_name,MIN(local_name_len,name_len));
}
/****************************************************************************/
//  Description : Bbm_SetBTLocalName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置本地BT 设备名称
/****************************************************************************/
PUBLIC void Bbm_SetBTLocalName(wchar *p_name)
{
    uint16                   local_name_len = 0;
    wchar ble_name[UAL_BT_DEV_NAME_MAX_LEN] = {0};
    if(PNULL == p_name)
    {
        TRACE_APP_BBM("p_name is null");
        return;
    }

    local_name_len = MMIAPICOM_Wstrlen(p_name);
    TRACE_APP_BBM("local_name_len = %d",local_name_len);

    local_name_len = MIN(local_name_len,UAL_BT_DEV_NAME_MAX_LEN);

    MMIAPICOM_Wstrncpy(ble_name,p_name,local_name_len);
    MMINV_WRITE(WATCH_BT_LOCAL_NAME,ble_name);
}
/*******************************************************************************************/
/********************************nv api end*************************************************/
/*******************************************************************************************/

