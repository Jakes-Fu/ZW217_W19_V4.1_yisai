/*****************************************************************************
** File Name:      mmiesim_hb_connection.c                                   *
** Author:         yonghua.zhang                                             *
** Date:           07/15/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to parse ble private data of HB         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 07/2020               yonghua.zhang            Create                     *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**-------------------------------------------------------------- ------------*/
#include "sci_types.h"
#ifndef UAL_BT_BLE_DEV_SUPPORT_NONE
#include "ual_bt_ble_dev.h"
#include "ual_bt_ble_private.h"
#endif
#include "hb_export.h"
#include "mmicom_trace.h"
#include "os_api.h"
/*****************************************************************************/
//  Description : handle ble connect information
//  Parameter: [In]  parm ble message infor
//             [OUT] NONE
//             [Return] NONE
//  Author:yonghua.zhang
// Note:
/*****************************************************************************/

LOCAL void MMIESIM_HandleBleDevMsg(ual_cms_msg_t param)
{
    TRACE_APP_ESIM_HB("param id ", param.msg_id);
    switch(param.msg_id)
    {
        //Òì³£´¦Àí
        case UAL_BT_BLE_DEV_MSG_ACTIVE_DISCONNECT_CNF:
        case UAL_BT_BLE_DEV_MSG_PASSIVE_DISCONNECT_CNF:
        {

        }
        break;

        default:
            break;
    }
}
/*****************************************************************************/
//  Description : handle ble private message for esim module
//  Parameter: [In]  parm ble message infor
//             [OUT] NONE
//             [Return] NONE
//  Author:yonghua.zhang
// Note:
/*****************************************************************************/

LOCAL void MMIESIM_HandleBlePrivateMsg(ual_cms_msg_t param)
{
    uint8 i=0, j=0, k=0;
    uint8 packet_len = 0;
    uint8 last_packet_len = 0;

    TRACE_APP_ESIM_HB("mg_id:%d\n", param.msg_id);
    switch(param.msg_id)
    {
        case UAL_BT_BLE_PRIVATE_MSG_RECEIVE_DATA_CNF:
        {
            ual_bt_ble_private_message_data_t* private_message = (ual_bt_ble_private_message_data_t*)param.p_body;

            packet_len = private_message->msg_len;
            TRACE_APP_ESIM_HB("msg_type: 0x%x, msg_len:%d\n", private_message->msg_type, packet_len);

            if( UAL_BT_BLE_PRIVATE_ESIM == private_message->msg_type)
            {
                MMIDEFAULT_TurnOnBackLight();
                ESIM_HB_ReceiveBLEData(private_message);
            }
        }
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : register ble callback for esim module
//  Parameter: [In]  NONE
//             [OUT] NONE
//             [Return] NONE
//  Author:yonghua.zhang
// Note:
/*****************************************************************************/

PUBLIC void MMIESIM_HB_BLEInit(void)
{
#ifndef UAL_BT_BLE_DEV_SUPPORT_NONE
    uint32 ble_dev_handle = 0;
    uint32 ble_private_handle = 0;

    ual_bt_ble_dev_register(MMIESIM_HandleBleDevMsg,&ble_dev_handle);
    ual_bt_ble_private_register(MMIESIM_HandleBlePrivateMsg,&ble_private_handle);
#endif
    TRACE_APP_ESIM_HB("register callback to ble");
}


