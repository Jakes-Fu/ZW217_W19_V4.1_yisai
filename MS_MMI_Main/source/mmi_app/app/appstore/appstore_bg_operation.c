/*****************************************************************************
** File Name:      appstore_bg_operation.c                                   *
** Author:         donglin.su                                                *
** Date:           11/1/2021                                                 *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to deal with command from phone apk     *
**                 function                                                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/1/2021      donglin.su       Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "appstore_common.h"
#ifdef UAL_BT_BLE_DEV_SUPPORT
#include "ual_bt_ble_dev.h"
#endif
#ifdef UAL_BT_BLE_PRIVATE_SUPPORT
#include "ual_bt_ble_private.h"
#endif
#include "appstore_bg_operation.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL uint32   s_appstore_handle = 0;//接收ble private 模块的消息handle

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
#ifdef UAL_BT_BLE_PRIVATE_SUPPORT
/*****************************************************************************/
//  Description : HandleBleMsg
//  Parameter: [In]  ual_cms_msg_t:msg id and msg body
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:handle ble private message
/*****************************************************************************/
LOCAL  BOOLEAN HandleBleMsg(ual_cms_msg_t param);
#endif

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : APPSTORE_BG_RegisterBLE
//  Parameter: [In]  none
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:init watchface handle ble private message module
/*****************************************************************************/
PUBLIC BOOLEAN APPSTORE_BG_RegisterBLE(void)
{
    BOOLEAN ret = TRUE;
#ifdef UAL_BT_BLE_PRIVATE_SUPPORT
    ual_bt_ble_result_e app_private_ret = UAL_BT_BLE_RESULT_START;
    //register ble private msg

    app_private_ret = ual_bt_ble_private_register(HandleBleMsg, &s_appstore_handle);
    if(UAL_BT_BLE_PRIVATE_RESULT_SUCESS == app_private_ret)
    {
        TRACE_APPSTORE("watchface register ble private sucess!\n");
        ret = TRUE;
    }
    else
    {
        TRACE_APPSTORE("watchface register ble private fail!\n");
        ret = FALSE;
    }
#endif
    return ret;
}

#ifdef UAL_BT_BLE_PRIVATE_SUPPORT
/*****************************************************************************/
//  Description : HandleBleMsg
//  Parameter: [In]  ual_cms_msg_t:msg id and msg body
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:handle ble private message
/*****************************************************************************/
LOCAL  BOOLEAN HandleBleMsg(ual_cms_msg_t param)
{
    BOOLEAN ret = TRUE;
    char*  p_command[5] = {"APPINS",//安装APP
                           "APPUPD",//更新APP
                           "APPDEL",//删除APP
                           "APPINF",};//获得所有APP信息
    uint8  received_command[APPSTORE_CMD_LEN_MAX+1] = {0};
    char*  p_command_response[2] = {"00","01"};//失败和成功
    uint16 all_app_num = 0;
    uint8  app_id[APPSTORE_APP_ID_LEN_MAX] = {0};
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
    APPSTORE_APP_LIST_T* p_app_list = PNULL;
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    LOCAL  uint8 s_app_index = 0;
    LOCAL  uint8 s_received_msg_id[APPSTORE_MSG_ID_LEN_MAX+1] = {0};
    LOCAL  ual_bt_ble_private_message_data_t* s_p_private_message = PNULL;

    switch(param.msg_id)
    {
        case UAL_BT_BLE_PRIVATE_MSG_RECEIVE_DATA_CNF:
        {
            s_p_private_message = (ual_bt_ble_private_message_data_t*)param.p_body;
            if(PNULL == s_p_private_message)
            {
                TRACE_APPSTORE("[APPSTORE_BG]:appstore private_message is null");
                ret = FALSE;
                break;
            }

            if( UAL_BT_BLE_PRIVATE_APPSTORE == s_p_private_message->msg_type)
            {
                TRACE_APPSTORE("[APPSTORE_BG]:appstore data is msg len = %d,\n",s_p_private_message->msg_len);
                TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                if(0 >= s_p_private_message->msg_len)
                {
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore data is false");
                    ret = FALSE;
                }
                else
                {
                    SCI_MEMSET(s_received_msg_id,0x00,APPSTORE_MSG_ID_LEN_MAX);
                    SCI_MEMCPY(s_received_msg_id,&(s_p_private_message->msg_data[0]),APPSTORE_MSG_ID_LEN_MAX);
                    SCI_MEMCPY(received_command,&(s_p_private_message->msg_data[APPSTORE_MSG_ID_LEN_MAX]),APPSTORE_CMD_LEN_MAX);
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore msgid_get is %s",s_received_msg_id);
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore command_get is %s",received_command);
                    if( 0 == SCI_MEMCMP(p_command[0],received_command,APPSTORE_CMD_LEN_MAX))//installed app
                    {
                        p_app_list = APPSTORE_GetFullAppList();
                        if(PNULL == p_app_list)  //coverity bug1941697
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:p_app_list is null");
                            ret = FALSE;
                            break;
                        }
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore command is APPINS");
                        SCI_MEMCPY(app_id,
                            &(s_p_private_message->msg_data[APPSTORE_MSG_ID_LEN_MAX+APPSTORE_CMD_LEN_MAX + APPSTORE_URL_LEN_MAX]),
                            APPSTORE_APP_ID_LEN_MAX);
                        TRACE_APPSTORE("[APPSTORE_BG]:app id is %s", app_id);
                        APPSTORE_GetAppNodeByAppId(app_id, *p_app_list, &p_app_node);
                        if((PNULL == p_app_node)||(PNULL == p_app_node->p_app_id))
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:get app node is null");
                            ret = FALSE;
                            SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[0]);
                            TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                            ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                            break;
                        }
                        APPSTORE_InstallAPP(p_app_node);
                        SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[1]);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                        s_p_private_message->msg_len = SCI_STRLEN(s_p_private_message->msg_data);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data len is %d\n",s_p_private_message->msg_len);
                        ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                    }
                    else if( 0 == SCI_MEMCMP(p_command[1],received_command,APPSTORE_CMD_LEN_MAX))//update app
                    {
                        p_app_list = APPSTORE_GetFullAppList();
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore command is APPUDP");
                        if(PNULL == p_app_list)  //coverity bug1941697
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:p_app_list is null");
                            ret = FALSE;
                            break;
                        }

                        SCI_MEMCPY(app_id,
                            &(s_p_private_message->msg_data[APPSTORE_MSG_ID_LEN_MAX+APPSTORE_CMD_LEN_MAX + APPSTORE_URL_LEN_MAX]),
                            APPSTORE_APP_ID_LEN_MAX);
                        TRACE_APPSTORE("[APPSTORE_BG]:app id is %s", app_id);
                        APPSTORE_GetAppNodeByAppId(app_id, *p_app_list, &p_app_node);
                        if((PNULL == p_app_node)||(PNULL == p_app_node->p_app_id))
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:get app node is null");
                            ret = FALSE;
                            SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[0]);
                            TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                            ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                            break;
                        }
                        APPSTORE_UpdateAPP(p_app_node);
                        SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[1]);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                        s_p_private_message->msg_len = SCI_STRLEN(s_p_private_message->msg_data);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data len is %d\n",s_p_private_message->msg_len);
                        ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                    }
                    else if( 0 == SCI_MEMCMP(p_command[2],received_command,APPSTORE_CMD_LEN_MAX))//delete app
                    {
                        p_app_list = APPSTORE_GetFullAppList();
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore command is APPDEL");
                        if(PNULL == p_app_list)  //coverity bug1941697
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:p_app_list is null");
                            ret = FALSE;
                            break;
                        }
                        SCI_MEMCPY(app_id, &(s_p_private_message->msg_data[APPSTORE_MSG_ID_LEN_MAX + APPSTORE_CMD_LEN_MAX]),
                            APPSTORE_APP_ID_LEN_MAX);
                        TRACE_APPSTORE("[APPSTORE_BG]:app id is %s", app_id);
                        APPSTORE_GetAppNodeByAppId(app_id, *p_app_list, &p_app_node);

                        if((PNULL == p_app_node)||(PNULL == p_app_node->p_app_id))
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:get app node is null");
                            ret = FALSE;
                            SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[0]);
                            TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                            ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                            break;
                        }
                        TRACE_APPSTORE("[APPSTORE_BG]:app id is %s", p_app_node->p_app_id);

                        APPSTORE_UninstallAPP(p_app_node);
                        SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[1]);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                        s_p_private_message->msg_len = SCI_STRLEN(s_p_private_message->msg_data);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data len is %d\n",s_p_private_message->msg_len);
                        ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                    }
                    else if( 0 == SCI_MEMCMP(p_command[3],received_command,APPSTORE_CMD_LEN_MAX))//get all app info
                    {
                        result = APPSTORE_GetInstalledAppList(&p_app_list);
                        if(PNULL == p_app_list)  //coverity bug1941689
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:get all app info p_app_list is null");
                            ret = FALSE;
                            break;
                        }

                        if(APPSTORE_RES_SUCC != result)
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:APPSTORE_GetInstalledAppList error! result = %d",result);
                            ret = FALSE;
                            break;
                        }

                        TRACE_APPSTORE("[APPSTORE_BG]:appstore command is APPINF");
                        s_app_index = 0;
                        all_app_num = p_app_list->app_num;
                        if(0 == all_app_num)
                        {
                            SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(s_p_private_message->msg_data,"%s%s%02d%02d",s_received_msg_id,p_command_response[1],
                                s_app_index, all_app_num);
                            TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                            ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                            if(PNULL != p_app_list)
                            {
                                APPSTORE_FreeAppList(p_app_list);
                                p_app_list = PNULL;
                            }
                            break;
                        }
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore all_app_num is %d",all_app_num);
                        APPSTORE_GetAppNodeFromAppList(s_app_index,*p_app_list,&p_app_node);
                        if((PNULL == p_app_node)||(PNULL == p_app_node->p_app_id))
                        {
                            TRACE_APPSTORE("[APPSTORE_BG]:get app node is null");
                            ret = FALSE;
                            SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[0]);
                            TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,len = %d\n",s_p_private_message->msg_data, s_p_private_message->msg_len);
                            ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                            if(PNULL != p_app_list)
                            {
                                APPSTORE_FreeAppList(p_app_list);
                                p_app_list = PNULL;
                            }
                            break;
                        }
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore s_app_index = %d,appstore id is %s",s_app_index,p_app_node->p_app_id);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore app_version = %d",p_app_node->app_version_code);
                        SCI_MEMSET(app_id, 0, APPSTORE_APP_ID_LEN_MAX);
                        SCI_MEMCPY(app_id, p_app_node->p_app_id, SCI_STRLEN(p_app_node->p_app_id));
                        SCI_MEMSET(s_p_private_message->msg_data,0x00,sizeof(uint8)*UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(s_p_private_message->msg_data,"%s%s%02d%02d%02d%s",s_received_msg_id,p_command_response[1],
                            s_app_index,all_app_num,p_app_node->app_version_code,app_id);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                        s_p_private_message->msg_len = SCI_STRLEN(s_p_private_message->msg_data);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data len is %d\n",s_p_private_message->msg_len);
                        ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                        s_app_index = 1;
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                        if(PNULL != p_app_list)
                        {
                            APPSTORE_FreeAppList(p_app_list);
                            p_app_list = PNULL;
                        }
                    }
                    else
                    {
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore command is ERROR");
                        SCI_MEMSET(s_p_private_message->msg_data,0x00,sizeof(s_p_private_message->msg_data));
                        sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[0]);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                        ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                    }
                }
            }
            else
            {
                TRACE_APPSTORE("[APPSTORE_BG]:appstore should not process current private data!\n");
            }
            break;
        }

        case UAL_BT_BLE_PRIVATE_MSG_SEND_DATA_CNF:
        {
            TRACE_APPSTORE("[APPSTORE_BG]:appstore UAL_BT_BLE_PRIVATE_MSG_SEND_DATA_CNF\n");
            if( s_app_index > 0 )
            {
                result = APPSTORE_GetInstalledAppList(&p_app_list);
                if(PNULL == p_app_list)  //coverity bug1941689
                {
                    TRACE_APPSTORE("[APPSTORE_BG]:UAL_BT_BLE_PRIVATE_MSG_SEND_DATA_CNF p_app_list is null");
                    ret = FALSE;
                    break;
                }

                if(APPSTORE_RES_SUCC != result)
                {
                    TRACE_APPSTORE("[APPSTORE_BG]:APPSTORE_GetInstalledAppList error! result = %d",result);
                    ret = FALSE;
                    break;
                }
                all_app_num = p_app_list->app_num;
                if(PNULL == s_p_private_message)
                {
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore private_message is null");
                    ret = FALSE;
                    if(PNULL != p_app_list)
                    {
                        APPSTORE_FreeAppList(p_app_list);
                        p_app_list = PNULL;
                    }
                    break;
                }
                TRACE_APPSTORE("[APPSTORE_BG]:appstore command is APPINF!");
                if( s_app_index < all_app_num )
                {
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore command is APPINF!");
                    SCI_MEMSET(s_p_private_message->msg_data,0x00,sizeof(uint8)*UAL_MAX_PRIVATE_DATA_LEN);
                    APPSTORE_GetAppNodeFromAppList(s_app_index,*p_app_list,&p_app_node);
                    if((PNULL == p_app_node)||(PNULL == p_app_node->p_app_id))
                    {
                        TRACE_APPSTORE("[APPSTORE_BG]:get app node is null");
                        ret = FALSE;
                        SCI_MEMSET(s_p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(s_p_private_message->msg_data,"%s%s",s_received_msg_id,p_command_response[0]);
                        TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                        ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                        if(PNULL != p_app_list)
                        {
                            APPSTORE_FreeAppList(p_app_list);
                            p_app_list = PNULL;
                        }
                        break;
                    }
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore s_app_index=%d,p_app-id=%s",s_app_index,p_app_node->p_app_id);
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore app_version = %d",p_app_node->app_version_code);
                    SCI_MEMSET(app_id, 0, APPSTORE_APP_ID_LEN_MAX);
                    SCI_MEMCPY(app_id, p_app_node->p_app_id, SCI_STRLEN(p_app_node->p_app_id));
                    sprintf(s_p_private_message->msg_data,"%s%s%02d%02d%02d%s",s_received_msg_id,p_command_response[1],
                        s_app_index,all_app_num,p_app_node->app_version_code,p_app_node->p_app_id);
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,\n",s_p_private_message->msg_data);
                    s_p_private_message->msg_type = UAL_BT_BLE_PRIVATE_APPSTORE;
                    s_p_private_message->msg_len = SCI_STRLEN(s_p_private_message->msg_data);
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore data len is %d\n",s_p_private_message->msg_len);
                    ual_bt_ble_private_send_data(s_p_private_message,s_p_private_message->msg_len + APPSTORE_SEND_MSG_HEAD_LEN);
                    s_app_index++;
                    TRACE_APPSTORE("[APPSTORE_BG]:appstore data is %s,s_app_index=%d\n",s_p_private_message->msg_data,s_app_index);
                    if(PNULL != p_app_list)
                    {
                        APPSTORE_FreeAppList(p_app_list);
                        p_app_list = PNULL;
                    }
                }
                else
                {
                    s_app_index = 0;
                }
            }
            else
            {
                TRACE_APPSTORE("[APPSTORE_BG]:appstore s_appidx is %d", s_app_index );
            }
            break;
        }

        default:
            TRACE_APPSTORE("[APPSTORE_BG]:msg_id:%d should not process\n",param.msg_id);
            ret = FALSE;
            break;

    }

    return ret;
}
#endif
