/*****************************************************************************
** File Name:        mmieng_gps_ota.c                                        *
** Author:           xiuyun.wang                                             *
** Date:             06/10/2020                                              *
** Copyright:                                                                *
** Description:    This file is used as gps ota function                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 06/10/2020           xiuyun.wang           Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "window_parse.h"
#include "mmieng_id.h"
#include "mmi_signal_ext.h"
#ifdef GPS_SUPPORT
#include "mmigps_api.h"
#endif
#include "guicommon.h"
#include "socket_api.h"
#include "sig_code.h"
#include "mmicom_trace.h"
#include "mmieng_export.h"
#include "mmiwifi_export.h"
#include "mmieng_text.h"
#include "guilistbox.h"
#include "mmieng_menutable.h"
#include "mmieng_gps_ota.h"
#include "mmk_msg.h"

#define P_GPS_OTA_SOCK_STACK_SIZE     (1024*4)
#define P_GPS_OTA_SOCK_QUEUE_NUM      15
#define P_GPS_OTA_SOCK_TASK_PRIORITY  78
#define IP_ADDR_LEN                   64
#define GPS_OTA_SOCK_DIAG_BUFF_SIZE (100)
#define SOCK_WRITE_RETRY 60

typedef enum{
    APP_GPS_OTA_STATUS_IDLE,
    APP_GPS_OTA_STATUS_SETTING,
    APP_GPS_OTA_STATUS_SET_FINISH
}APP_GPS_OTA_STATUS_E;
LOCAL BLOCK_ID g_gps_ota_sock_task = SCI_INVALID_BLOCK_ID;
TCPIP_SOCKET_T g_sock_srv = TCPIP_SOCKET_INVALID;
TCPIP_SOCKET_T g_sock_diag = TCPIP_SOCKET_ERROR;
LOCAL uint8 g_sock_diag_buffer[GPS_OTA_SOCK_DIAG_BUFF_SIZE] = {0};
APP_GPS_OTA_STATUS_E g_gpsOtaStatus = APP_GPS_OTA_STATUS_IDLE;
#define GPS_OTA_SOCK_PORT (7878)

/**********************internal api define*******************/
LOCAL void EngGpsOTA_HandleDataFromPC(uint8 *pData);

LOCAL BOOLEAN GetWifiIpAddr(char *ip_ptr,uint32 buf_len)
{
    unsigned long ip_u32 = 0;

    ip_u32 = (unsigned long)sci_gethostaddress();
    {
        typedef union
        {
            uint32 whole;
            struct
            {
                uint32 byte0 : 8;
                uint32 byte1 : 8;
                uint32 byte2 : 8;
                uint32 byte3 : 8;
            }all_part;
        }DEPART_T;

        DEPART_T depart = {0};
        depart.whole = ip_u32;

        _snprintf(
            ip_ptr,
            buf_len,
            "%d.%d.%d.%d",
            depart.all_part.byte0,
            depart.all_part.byte1,
            depart.all_part.byte2,
            depart.all_part.byte3
            );
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : fill content to listbox
//  param:IN ctrl_id, list ctrl
//  Author:
//  RETRUN:
//  Note:
/*****************************************************************************/
LOCAL void EngWifi_IPAddrDisplay(MMI_CTRL_ID_T current_ctrl_id)
{
    GUILIST_ITEM_T          item_t    = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    MMI_TEXT_ID_T           text_id = 0;
    MMI_STRING_T temp_ipAddr = {0};
    MMI_STRING_T temp_ipData = {0};
    char ipAddr[IP_ADDR_LEN] = {0};
    wchar temp_IpDataWchar[IP_ADDR_LEN]  = {0};

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    TRACE_APP_ENGINEERMODE("[GPS OTA]");

    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA] wifi is connected");

        //ip address string
        text_id = TXT_ENG_WIFI_IPADDR;
        MMI_GetLabelTextByLang(text_id, &temp_ipAddr);
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer = temp_ipAddr;
        GUILIST_AppendItem(current_ctrl_id,&item_t);

        // ip address data
        if(GetWifiIpAddr(ipAddr, sizeof(ipAddr)))
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] ip address = %s",ipAddr);
        }
        else
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] get ip fail!!");
        }
        MMIAPICOM_StrToWstr(ipAddr,temp_IpDataWchar);
        temp_ipData.wstr_len = MMIAPICOM_Wstrlen(temp_IpDataWchar);
        temp_ipData.wstr_ptr = temp_IpDataWchar;
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer = temp_ipData;
        GUILIST_AppendItem(current_ctrl_id,&item_t);
    }
    else
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA] wifi is not connect");

        //popup please connect wifi
        text_id = TXT_ENG_WIFI_NOT_CONNECT;
        MMI_GetLabelTextByLang(text_id, &temp_ipAddr);
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer = temp_ipAddr;
        GUILIST_AppendItem(current_ctrl_id,&item_t);
    }

}
/*****************************************************************************/
//  Description : send signal to app task
//  Global resource dependence :
//  Author:
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void EngGpsOTA_SendDataToAPPTask( void * pData)
{
    APP_GPS_OTA_SENG_DATA_TO_APP_T *sig_ptr = PNULL;
    MMI_CREATE_SIGNAL(sig_ptr,
        APP_GPS_OTA_SEND_DATA_TO_APP,
        sizeof(APP_GPS_OTA_SENG_DATA_TO_APP_T),
        SCI_IdentifyThread()
        );
    sig_ptr->pData = pData;
    MMI_SEND_SIGNAL(sig_ptr, P_APP);
}

LOCAL int sock_send_data( uint8* buff, int len)
{
    int ret = 0;
    int offset = 0;
    int retry = SOCK_WRITE_RETRY;

    TRACE_APP_ENGINEERMODE("[GPS OTA]: write fd = %x, len = %d", g_sock_diag, len);
    if(PNULL == buff)
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: buff is null");
        return FALSE;
    }
    TRACE_APP_ENGINEERMODE("[GPS OTA]: send string:%s", buff);

    do{
        if(g_sock_diag == TCPIP_SOCKET_ERROR)
            break;

        ret = sci_sock_send(g_sock_diag, buff+offset, len, 0);
        if (ret < 0)
        {
            if(retry-- <=0)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]:error: spipe write fail! break");
                break;
            }
            else
            {
                SCI_Sleep(500);
                TRACE_APP_ENGINEERMODE("[GPS OTA]:error: spipe write fail! retry, error = %d", ret);
            }
        }
        else
        {
            len -= ret;
            offset += ret;
        }

    }while(len > 0);

    TRACE_APP_ENGINEERMODE("[GPS OTA]: send suc data len: %d", offset);
    return offset;
}
LOCAL void ENGGpsOTA_SendDataToPC(uint8 *pCmd, uint8 *pStatus, APP_MN_GPS_READ_INFO_CNF_T *pGps_data)
{
    uint8 response_cmd[GPS_OTA_SOCK_DIAG_BUFF_SIZE*2]= {0};
    uint32 resCmd_len =0;
    int32 i = 0;
    if(PNULL == pCmd)
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: response to pc command is null\n");
        return ;
    }
    if(PNULL == pStatus)
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: response to pc status is null\n");
        return ;
    }
    //montage command
    SCI_MEMCPY(response_cmd,pCmd, strlen(pCmd));//cmd
    strcat(response_cmd, ",");
    strcat(response_cmd, pStatus);

    TRACE_APP_ENGINEERMODE("[GPS OTA]: command :%s\n",response_cmd);
    if(PNULL != pGps_data)
    {
        strcat(response_cmd, ",");
        strcat(response_cmd, "result:");
        resCmd_len = strlen(response_cmd);
        if(0 != pGps_data->satellite_list_info.satellite_num)
        {
            for(i =0; i<pGps_data->satellite_list_info.satellite_num; i++)
            {
                sprintf(response_cmd+resCmd_len, "%d^%d^",pGps_data->satellite_list_info.satellite_info[i].m_nSatelliteIdentifier,
                    pGps_data->satellite_list_info.satellite_info[i].m_cn0);

                resCmd_len = strlen(response_cmd);
            }
        }
        else
        {
            strcat(response_cmd, "fail^");
        }
        TRACE_APP_ENGINEERMODE("[GPS OTA]: response :%s\n",response_cmd);
    }
    else
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: response to pc result is null\n");
    }

    //send data to pc
    sock_send_data(response_cmd, strlen(response_cmd));
}

LOCAL BOOLEAN _EngHandleGpsMessageCallback(APP_MN_GPS_SIGNAL_E gps_sig_id,void* gps_info)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    static APP_MN_GPS_READ_INFO_CNF_T report_gpsinfo = {0};//get information from report
    APP_MN_GPS_READ_INFO_CNF_T read_gpsinfo = {0};
    int i =0;
    TRACE_APP_ENGINEERMODE("[GPS OTA]: gps_sig_id:%d\n",gps_sig_id);
    switch ( gps_sig_id )
    {
        case APP_MN_GPS_INIT_CNF:
        {
            APP_MN_GPS_INIT_CNF_T *pInitcnf = (APP_MN_GPS_INIT_CNF_T *)gps_info;
            if(PNULL == pInitcnf)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]:APP_MN_GPS_INIT_CNF param is null\n");
                break;
            }
            if(TRUE == pInitcnf->is_ok)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]:gps init succ");
            }
            break;
        }
        case APP_MN_GPS_START_CNF:
        {
            APP_MN_GPS_START_CNF_T *pStartInfo = (APP_MN_GPS_START_CNF_T *)gps_info;
            if(PNULL == pStartInfo)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_MN_GPS_START_CNF  pStartInfo is null\n");
                return MMI_RESULT_TRUE;
            }
            if(TRUE == pStartInfo->is_ok)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]:APP_MN_GPS_START_CNF is OK\n");
                ENGGpsOTA_SendDataToPC("cmd:StartGPSLocation", "status:ok", NULL);

            }else
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]:APP_MN_GPS_START_CNF is NOT OK\n");
                ENGGpsOTA_SendDataToPC("cmd:StartGPSLocation", "status:fail", NULL);
            }
            break;
        }
        case APP_MN_GPS_READ_INFO_CNF:
        {
            char test_str[256] = {0};
            APP_MN_GPS_READ_INFO_CNF_T *param_ptr = (APP_MN_GPS_READ_INFO_CNF_T *)gps_info;
            TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_MN_GPS_READ_INFO_CNF\n");
            if(PNULL == param_ptr)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_MN_GPS_READ_INFO_CNF  gps_info is null\n");
                ENGGpsOTA_SendDataToPC("cmd:GetGPSInfo", "status:ok", NULL);
                return MMI_RESULT_FALSE;
            }

            read_gpsinfo.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;
            SCI_MEMCPY(read_gpsinfo.satellite_list_info.satellite_info,
                param_ptr->satellite_list_info.satellite_info,
                sizeof(param_ptr->satellite_list_info.satellite_info));
            TRACE_APP_ENGINEERMODE("[GPS OTA] read_gpsinfo.satellite_list_info.satellite_num = %d ",read_gpsinfo.satellite_list_info.satellite_num);
            for(i=0; i<read_gpsinfo.satellite_list_info.satellite_num; i++)
            {
                sprintf(test_str, "%s: APP_MN_GPS_READ_INFO_CNF satellite info %d,%d,%d,%d,%d", "read_gpsinfo satellite info:",
                read_gpsinfo.satellite_list_info.satellite_info[i].m_nSatelliteIdentifier,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_cn0,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_nElevation,  param_ptr->satellite_list_info.satellite_info[i].m_nAzimuth,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_IsUsed);
                TRACE_APP_ENGINEERMODE("[GPS OTA]:%s",test_str);
            }
            ENGGpsOTA_SendDataToPC("cmd:GetGPSInfo", "status:ok", &read_gpsinfo);
             break;
        }
        case APP_MN_GPS_ERR_IND:
        {
            break;
        }
        case APP_MN_GPS_STOP_CNF:
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA]:APP_MN_GPS_STOP_CNF is OK\n");
            ENGGpsOTA_SendDataToPC("cmd:EndGPSLocation", "status:ok", NULL);
            break;
        }
        case APP_MN_GPS_DEL_AID_DATA_CNF:
        {
            APP_MN_GPS_DEL_AID_DATA_CNF_T *param_ptr = (APP_MN_GPS_DEL_AID_DATA_CNF_T *)gps_info;
            TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_MN_GPS_DEL_AID_DATA_CNF\n");
            if(PNULL == param_ptr)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_MN_GPS_DEL_AID_DATA_CNF  gps_info is null\n");
                ENGGpsOTA_SendDataToPC("cmd:DelGPSAidData", "status:fail", NULL);
                return MMI_RESULT_FALSE;
            }
            if(TRUE == param_ptr->is_ok)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_MN_GPS_DEL_AID_DATA_CNF  delete aid data ok\n");
                ENGGpsOTA_SendDataToPC("cmd:DelGPSAidData", "status:ok", NULL);
            }
            else
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_MN_GPS_DEL_AID_DATA_CNF  delete aid data fail\n");
                ENGGpsOTA_SendDataToPC("cmd:DelGPSAidData", "status:fail", NULL);
            }
            break;
        }
        case APP_GPS_OTA_SEND_DATA_TO_APP:
        {
            uint8 DataFromPC[GPS_OTA_SOCK_DIAG_BUFF_SIZE]= {0};
            APP_GPS_OTA_SENG_DATA_TO_APP_T *pDataFromPC = (APP_GPS_OTA_SENG_DATA_TO_APP_T *)gps_info;
            TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_GPS_OTA_SEND_DATA_TO_APP\n");
            if(NULL == gps_info)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]: APP_GPS_OTA_SEND_DATA_TO_APP, data is null\n");
                break;
            }
            SCI_MEMCPY(DataFromPC, pDataFromPC->pData, strlen(pDataFromPC->pData));
            EngGpsOTA_HandleDataFromPC(DataFromPC);
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            TRACE_APP_ENGINEERMODE("[GPS OTA]: Unknow msg id!\n");
            break;
        }
    }
    return result;
}

LOCAL void EngGpsOTA_HandleDataFromPC(uint8 *pData)
{
    uint8 cmd_delAidData[20]={"cmd:DelGPSAidData"};
    uint8 cmd_startGPS[30] = {"cmd:StartGPSLocation"};
    uint8 cmd_getGPSInof[50] = {"cmd:GetGPSInfo,param:satelliteInfo"};
    uint8 cmd_endGPS[30] = {"cmd:EndGPSLocation"};
    MMIGPS_START_GPS_PARAM_T start_param = {0};

    TRACE_APP_ENGINEERMODE("[GPS OTA] enter, data:%s", pData);
    if(PNULL == pData)
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA] pData is null!!!!");
        return;
    }

    if ( 0 == MMIAPICOM_StrCmp(pData,strlen(pData),cmd_delAidData,strlen(cmd_delAidData)))
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: command gps delete aid data\n");
        MMIGPS_AidData_Delete();
    }
    else if ( 0 == MMIAPICOM_StrCmp(pData,strlen(pData),cmd_startGPS,strlen(cmd_startGPS)))
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: command gps start\n");

        //start gps
        start_param.request_type = MMIGPS_REQUEST_TYPE_ENG;
        start_param.start_type = MMIGPS_START_TYPE_NONE;
        start_param.start_mode = 0;
        MMIGPS_Start(&start_param,_EngHandleGpsMessageCallback);
    }
    else if ( 0 == MMIAPICOM_StrCmp(pData,strlen(pData),cmd_getGPSInof,strlen(cmd_getGPSInof)))
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: get command read gps info\n");
        if (GPS_STARTED_STATE != MMIGPS_CurrentStatus_Get())
        {
            ENGGpsOTA_SendDataToPC("cmd:StartGPSLocation", "status:error", NULL);
        }
        else
        {
            MNGPS_ReadInfoReq(0);   //request gps and location info
        }
    }
    else if ( 0 == MMIAPICOM_StrCmp(pData,strlen(pData),cmd_endGPS,strlen(cmd_endGPS)))
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]: command gps stop\n");
        MMIGPS_Stop();
    }
    else
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA] command from pc is wrong!");
        ENGGpsOTA_SendDataToPC("cmd:error", "status:error", NULL);
    }
}
LOCAL void EngGpsOTA_sock_close(void)
{
    if(g_sock_srv == TCPIP_SOCKET_INVALID)
    {
        sci_sock_socketclose(g_sock_srv);
        g_sock_srv = TCPIP_SOCKET_INVALID;
    }
    if(g_sock_diag != TCPIP_SOCKET_ERROR)
    {
        sci_sock_socketclose(g_sock_diag);
        g_sock_diag = TCPIP_SOCKET_ERROR;
    }
}

LOCAL BOOLEAN EngGpsOTA_sock_open(void)
{
    struct sci_sockaddr addr = {0};
    int addr_len = 0;

    if(g_sock_srv == TCPIP_SOCKET_INVALID)
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]g_sock_srv is invalidate!!!!");
        return FALSE;
    }

    do{
        TRACE_APP_ENGINEERMODE("[GPS OTA] sock accept start: g_sock_srv = 0x%x", g_sock_srv);
        g_sock_diag = sci_sock_accept(g_sock_srv, (struct sci_sockaddrext*)&addr, &addr_len);
        TRACE_APP_ENGINEERMODE("[GPS OTA] sock accept end: accept ret %x, error %d", g_sock_diag, sci_sock_errno(g_sock_diag));
        if (g_sock_diag == TCPIP_SOCKET_ERROR)
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] accept fail!");
            continue;
        }
        else
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] accept ok!");
            return TRUE;
        }
    }while(1);


    return FALSE;
}

LOCAL void EngGpsOTA_Sock_Diag(void)
{
    int ret = 0;
    TRACE_APP_ENGINEERMODE("[GPS OTA]");

    if(EngGpsOTA_sock_open() != TRUE)
    {
        TRACE_APP_ENGINEERMODE("[GPS OTA]open sock diag fail!!!");
        return ;
    }

    while(TRUE)
    {
        int nRd = 0;
        int status;

        SCI_MEMSET(g_sock_diag_buffer, 0, sizeof(g_sock_diag_buffer));

        if(g_sock_diag != TCPIP_SOCKET_ERROR)
        {
            MMIWIFI_STATUS_E status;
            nRd = sci_sock_recv(g_sock_diag, g_sock_diag_buffer, sizeof(g_sock_diag_buffer), 0);
            status =  MMIAPIWIFI_GetStatus();
            if(MMIWIFI_STATUS_CONNECTED != status)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA] wifi is disconnect!!!!break");
            }
        }
        if (nRd > 0)
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA]received string:%s",g_sock_diag_buffer);
            EngGpsOTA_SendDataToAPPTask(g_sock_diag_buffer);

            status =  MMIAPIWIFI_GetStatus();
            if(MMIWIFI_STATUS_CONNECTED != status)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA] wifi is disconnect!!!!break");
                break;
            }
        }
        else
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] read return %d, then reopen",  nRd);
            do{
                EngGpsOTA_sock_close();
                if(EngGpsOTA_sock_open() != TRUE)
                {
                    TRACE_APP_ENGINEERMODE("[GPS OTA] reopen sock diag fail!!!");
                    break;
                }
            }while(0);
        }
    }
#ifndef WIN32
    pc_sock_diag_close();
#endif
}

LOCAL void EngGpsOTA_socket_thread_callback(uint32 argc, void* argv )
{
    int  err_type = 0;
    TCPIP_NETID_T netid = 0;
    struct sci_sockaddr addr = {0};
    int time = 60;
    int ret = -1;
    int reconnect = 0;

    do{
        netid = MMIAPIWIFI_GetNetId();
        if (TCPIP_NETID_NULL == netid)
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] netid is NULL!!!!");
        }

        do{
            if (g_sock_srv == TCPIP_SOCKET_INVALID)
            {
                g_sock_srv = sci_sock_socket(AF_INET, SOCK_STREAM, 0, netid);
            }
            else
            {
                ret = sci_sock_modifynetid(g_sock_srv, netid);
                TRACE_APP_ENGINEERMODE("[GPS OTA] modifynetId return %d, error %d", ret, sci_sock_errno(g_sock_srv));
                break;
            }

            TRACE_APP_ENGINEERMODE("[GPS OTA] g_sock_srv = 0x%x", g_sock_srv);
            if (TCPIP_SOCKET_INVALID != g_sock_srv)
            {
                if (reconnect == 0)
                {
                    addr.ip_addr = 0;
                    addr.family = AF_INET;
                    addr.port = (uint16)htons(GPS_OTA_SOCK_PORT);

                    ret = sci_sock_bind(g_sock_srv, (struct sci_sockaddr*)&addr, sizeof(addr));
                    TRACE_APP_ENGINEERMODE("[GPS OTA] sock bind: %x bind to %s, error %d", g_sock_srv,
                                        TCPIPDEBUG_GetIpPortInfo((struct sockaddr*)&addr), sci_sock_errno(g_sock_srv));
                }
                if (ret >= 0)
                {
                    TRACE_APP_ENGINEERMODE("[GPS OTA]sock listen start: g_sock_srv = 0x%x", g_sock_srv);
                    ret = sci_sock_listen(g_sock_srv, 5);
                    TRACE_APP_ENGINEERMODE("[GPS OTA]sock listen end: ret = %d, error %d", ret, sci_sock_errno(g_sock_srv));
                    if (ret >= 0)
                    {
                        break;
                    }
                }
            }

            SCI_Sleep(1000);
            if (time-- < 0)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA] fail:");
                time = 60;
            }

        }while(1);
    EngGpsOTA_Sock_Diag();
   } while(TRUE);
}

LOCAL void EngGpsOTA_InitSocket(void)
{
    if( SCI_INVALID_BLOCK_ID == g_gps_ota_sock_task )
    {
        g_gps_ota_sock_task = SCI_CreateThread(
            "GPS_OTA_SOCK_THREAD",
            "GPS_OTA_SOCK_THREAD",
            EngGpsOTA_socket_thread_callback,
            NULL,
            NULL,
            P_GPS_OTA_SOCK_STACK_SIZE,
            P_GPS_OTA_SOCK_QUEUE_NUM,
            P_GPS_OTA_SOCK_TASK_PRIORITY,
            SCI_PREEMPT,
            SCI_AUTO_START );
    }
}
LOCAL void HandlePdpCallback(MMIPDP_CNF_INFO_T *msg_ptr)
{
    int simindex = 0;
    if(PNULL == msg_ptr)
    {
        return;
    }

    TRACE_APP_ENGINEERMODE("[GPS OTA] msg_ptr->msg_id = %d.", msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] MMIPDP_ACTIVE_CNF.msg_ptr->result = %d,pdp_id:%d,nsapi:%x", msg_ptr->result,msg_ptr->pdp_id, msg_ptr->nsapi);

            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
            
                g_gpsOtaStatus = APP_GPS_OTA_STATUS_SET_FINISH;
                MMK_SendMsg(MMIENG_TEST_WIFI_IPADDR_WIN_ID, MSG_FULL_PAINT, PNULL);
                EngGpsOTA_InitSocket();
            }
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_GPS);
            }
        }
            break;
        case MMIPDP_DEACTIVE_CNF:
            TRACE_APP_ENGINEERMODE("[GPS OTA] MMIPDP_DEACTIVE_CNF");
            g_gpsOtaStatus = APP_GPS_OTA_STATUS_IDLE;
            break;

        case MMIPDP_DEACTIVE_IND:
            TRACE_APP_ENGINEERMODE("[GPS OTA] MMIPDP_DEACTIVE_IND");
            MMIAPIPDP_Deactive(MMI_MODULE_GPS);
            if( SCI_INVALID_BLOCK_ID != g_gps_ota_sock_task)
            {
                SCI_DeleteThread(g_gps_ota_sock_task);
                g_gps_ota_sock_task = SCI_INVALID_BLOCK_ID;
            }
            g_gpsOtaStatus = APP_GPS_OTA_STATUS_IDLE;
            break;
        default:
            break;
        }
}

LOCAL MMI_RESULT_E EngWiFi_IPAddrWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_RECT_T   client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T   lab_rect = client_rect;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    MMIGPS_RES_E gps_res = MMIGPS_RES_STATE_WRONG;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUILIST_SetMaxItem( ctrl_id, 3, FALSE);
            GUILIST_SetNeedHiLightBar(ctrl_id,FALSE);
            EngWifi_IPAddrDisplay(ctrl_id);
            MMK_SetAtvCtrl( win_id,  ctrl_id);

            //init gps first
            MMIGPS_RegisterCallBack(_EngHandleGpsMessageCallback);

            gps_res = MMIGPS_Open();
            TRACE_APP_ENGINEERMODE("[GPS OTA] gps init res:%d", gps_res);
            break;
        }
        case MSG_FULL_PAINT:
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] MSG_FULL_PAINT");
            if(APP_GPS_OTA_STATUS_IDLE == g_gpsOtaStatus)
            {
                GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL, TXT_ENG_GPS_SETUP_DATA_SOCKET, TXT_NULL, TRUE);
            }
            else if(APP_GPS_OTA_STATUS_SET_FINISH == g_gpsOtaStatus)
            {
                GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL, TXT_ENG_GPS_RELEASE_DATA_SOCKET, TXT_NULL, TRUE);
            }
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            TRACE_APP_ENGINEERMODE("[GPS OTA] g_gpsOtaStatus:%d", g_gpsOtaStatus);
            switch(g_gpsOtaStatus)
            {
                case APP_GPS_OTA_STATUS_IDLE:
                {
                    TRACE_APP_ENGINEERMODE("[GPS OTA] setup wifi pdp");
                    g_gpsOtaStatus = APP_GPS_OTA_STATUS_SETTING;
                    MMIGPS_SetupPdp(MMIPDP_INTERFACE_WIFI, HandlePdpCallback);
                    break;
                }
                case APP_GPS_OTA_STATUS_SET_FINISH:
                {
                    TRACE_APP_ENGINEERMODE("[GPS OTA] release wifi pdp");
                    EngGpsOTA_sock_close();
                    SCI_Sleep(100);//for watting close socket finished
                    g_gpsOtaStatus = APP_GPS_OTA_STATUS_IDLE;
                    MMIGPS_ReleasePdp();
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL, TXT_ENG_GPS_SETUP_DATA_SOCKET, TXT_NULL, TRUE);
                    if( SCI_INVALID_BLOCK_ID != g_gps_ota_sock_task )
                    {
                        SCI_DeleteThread(g_gps_ota_sock_task);
                        g_gps_ota_sock_task = SCI_INVALID_BLOCK_ID;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMIGPS_Close();
            MMK_CloseWin(win_id);
            break;
        }
        default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

WINDOW_TABLE(MMIENG_TEST_WIFI_IPADDR_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngWiFi_IPAddrWinHandleMsg),
    WIN_ID(MMIENG_TEST_WIFI_IPADDR_WIN_ID),
    WIN_TITLE(TXT_ENG_WIFI_IPADDR),
    WIN_SOFTKEY(TXT_NULL, TXT_ENG_GPS_SETUP_DATA_SOCKET, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

PUBLIC int32 MMIENG_OpenGPSOtaTestWin(void)
{
    MMK_CreateWin((uint32 * )MMIENG_TEST_WIFI_IPADDR_TAB, PNULL);
}



