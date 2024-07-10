/****************************************************************************************
** File Name:      wifisupp_api.c                                                       *
** Author:         juan.zhang                                                           *
** Date:           2009.10.29                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is used for wifi supplicant API functions' implementation  *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.10       juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "cfl_mem.h"
#include "wifisupp_api.h"
#include "wifisupp_internal.h"
#include "wifi_drv.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
unsigned int g_wifi_log_switch = 1; /*ON:1, OFF:0*/

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL uint32 SerializeConnectReq(
                                 WIFISUPP_SIG_CONNECT_REQ_T *req_ptr, 
                                 WIFISUPP_SIG_CONNECT_REQ_T **serialized_req_pptr //[OUT]
                                 );

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
PUBLIC void WIFISUPP_SetLogSwitch(unsigned int is_log)
{
    g_wifi_log_switch = is_log;
}

PUBLIC unsigned int WIFISUPP_GetLogSwitch(void)
{
    return g_wifi_log_switch;
}

/****************************************************************************/
//Description : To Initiate WiFi
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_Init(void)
{
#ifdef WIN32
    //SUPPMAIN_CreatThread();
#else
    SUPPMAIN_CreatThread();
#endif
    return TRUE;
}

/****************************************************************************/
//Description : To Open WiFi
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_OnReq(BLOCK_ID thread_id)
{
    WIFISUPP_SIG_ON_REQ_T   *sig_ptr = 0;
    uint32                  sig_size = 0;
    BLOCK_ID                sender_thread_id = SCI_IdentifyThread();
    
    if(0==thread_id || SCI_INVALID_BLOCK_ID==thread_id)
    {
        //SCI_TRACE_LOW:"WIFISUPP WIFISUPP_OnReq, invalid thread id!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_85_112_2_18_0_36_22_2279,(uint8*)"");
        return FALSE;
    }
    sig_size = sizeof(WIFISUPP_SIG_ON_REQ_T);
    SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_ON_REQ, sig_size, sender_thread_id);
    if(0 == sig_ptr)//lint !e774
    {
        //SCI_TRACE_LOW:"WIFISUPP WIFISUPP_OnReq, alloc sig_ptr fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_92_112_2_18_0_36_22_2280,(uint8*)"");
        return FALSE;      
    }
    sig_ptr->app_thread_id = thread_id;
#ifdef WIN32
    SCI_SEND_SIGNAL(sig_ptr, /*SUPPMAIN_GetWifiThreadId()*/ 13);
#else
    SCI_SEND_SIGNAL(sig_ptr, SUPPMAIN_GetWifiThreadId());
#endif
    return TRUE;
}

/****************************************************************************/
//Description : To Close WiFi
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_OffReq(void)
{
    xSignalHeader   sig_ptr = 0;
    uint32          sig_size = 0;
    BLOCK_ID        sender_thread_id = SCI_IdentifyThread();
    
    sig_size = sizeof(xSignalHeaderRec);
    SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_OFF_REQ, sig_size, sender_thread_id);
    if(0 == sig_ptr)//lint !e774
    {
        //SCI_TRACE_LOW:"WIFISUPP WIFISUPP_OffReq, alloc sig_ptr fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_114_112_2_18_0_36_22_2281,(uint8*)"");
        return FALSE;      
    }
#ifdef WIN32
    SCI_SEND_SIGNAL(sig_ptr, /*SUPPMAIN_GetWifiThreadId()*/12);
#else
    SCI_SEND_SIGNAL(sig_ptr, SUPPMAIN_GetWifiThreadId());
#endif
    return TRUE;
}

/****************************************************************************/
//Description : To scan reachable APs
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_ScanReq(WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr)
{
    WIFISUPP_SIG_SCAN_REQ_T   *sig_ptr = 0;
    uint32                     sig_size = 0;

    if(PNULL == scan_info_ptr)
    {
        //SCI_TRACE_LOW:"WIFI:error:SUPP:%s scan_info_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_132_112_2_18_0_36_22_2282,(uint8*)"s", __func__);
        return FALSE;
    }
    
    sig_size = sizeof(WIFISUPP_SIG_SCAN_REQ_T);
    SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_SCAN_REQ, sig_size, SCI_IdentifyThread());
    if(PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"WIFI:error:SUPP:%s alloc sig_ptr fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_140_112_2_18_0_36_22_2283,(uint8*)"s", __func__);
        return FALSE;      
    }

    sig_ptr->scan_info = *scan_info_ptr;
#ifdef WIN32
    SCI_SEND_SIGNAL(sig_ptr, /*SUPPMAIN_GetWifiThreadId()*/1);
#else
    SCI_SEND_SIGNAL(sig_ptr, SUPPMAIN_GetWifiThreadId());
#endif
    return TRUE;
}

/****************************************************************************/
//Description : To connect AP, Suppliant will connect and associate with the
//              AP automatically.
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_ConnectReq(WIFISUPP_SSID_CONFIG_T *connect_info_ptr)
{
    WIFISUPP_SIG_CONNECT_REQ_T  req_input = {0};
    WIFISUPP_SIG_CONNECT_REQ_T  *sig_ptr = 0;
    uint32                      sig_size = 0;
    BLOCK_ID                    sender_thread_id = SCI_IdentifyThread();

    if(connect_info_ptr == 0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WIFISUPP_ConnectReq, invalid input!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_164_112_2_18_0_36_22_2284,(uint8*)"");
        return FALSE;
    }
    req_input.ssid_config = *connect_info_ptr;
    sig_size = SerializeConnectReq(&req_input, &sig_ptr);
    if(0 == sig_ptr)
    {
        //SCI_TRACE_LOW:"WIFISUPP WIFISUPP_ConnectReq, alloc sig_ptr fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_171_112_2_18_0_36_22_2285,(uint8*)"");
        return FALSE;      
    }
    ((xSignalHeader)(sig_ptr))->SignalSize = sig_size;
    ((xSignalHeader)(sig_ptr))->SignalCode = WIFISUPP_SIG_CONNECT_REQ;
    ((xSignalHeader)(sig_ptr))->Sender     = sender_thread_id;
#ifdef WIN32
    SCI_SEND_SIGNAL(sig_ptr, /*SUPPMAIN_GetWifiThreadId()*/12);
#else
    SCI_SEND_SIGNAL(sig_ptr, SUPPMAIN_GetWifiThreadId());
#endif
    return TRUE;
}

/****************************************************************************/
//Description : To disconnect AP with the indicated SSID
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_DisconnectReq(WIFISUPP_DISCONNECT_REQ_INFO_T *disconnect_info_ptr)
{
    WIFISUPP_SIG_DISCONNECT_REQ_T   *sig_ptr = 0;
    uint32                          sig_size = 0;
    BLOCK_ID                        sender_thread_id = SCI_IdentifyThread();

    if(disconnect_info_ptr == 0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WIFISUPP_DisconnectReq, invalid input!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_194_112_2_18_0_36_22_2286,(uint8*)"");
        return FALSE;
    }
    sig_size = sizeof(WIFISUPP_SIG_DISCONNECT_REQ_T);
    SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_DISCONNECT_REQ, sig_size, sender_thread_id);
    if(0 == sig_ptr)//lint !e774
    {
        //SCI_TRACE_LOW:"WIFISUPP WIFISUPP_DisconnectReq, alloc sig_ptr fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_201_112_2_18_0_36_22_2287,(uint8*)"");
        return FALSE;      
    }

    sig_ptr->disc_info = *disconnect_info_ptr;
#ifdef WIN32
    SCI_SEND_SIGNAL(sig_ptr, /*SUPPMAIN_GetWifiThreadId()*/12);
#else
    SCI_SEND_SIGNAL(sig_ptr, SUPPMAIN_GetWifiThreadId());	
#endif
    return TRUE;
}

/****************************************************************************/
//Description : 
//Global resource dependence : 
//Author: juan.zhang
//Note: Modified by George.Liu
/****************************************************************************/
LOCAL uint32 SerializeConnectReq(                                
                                 WIFISUPP_SIG_CONNECT_REQ_T *req_ptr, 
                                 WIFISUPP_SIG_CONNECT_REQ_T **serialized_req_pptr //[OUT]
                                    )
{
    WIFISUPP_SIG_CONNECT_REQ_T  *serialized_ptr = 0;
    WIFISUPP_SSID_CONFIG_T      *config_ptr = 0;
    uint32                      serialized_size = sizeof(WIFISUPP_SIG_CONNECT_REQ_T);
    uint8                       *input_tls_cert_buf_ptr = 0, *serialized_tls_cert_buf_ptr = 0;
    uint8                       input_tls_cert_buf_len = 0;
    uint8                       *input_fast_pac_buf_ptr = 0, *serialized_fast_pac_buf_ptr = 0;
    uint8                       input_fast_pac_buf_len = 0;
    uint8                       *input_as_cert_buf_ptr = 0, *serialized_as_cert_buf_ptr = 0;
    uint32                       input_as_cert_buf_len = 0;
    uint8                       *input_asue_cert_buf_ptr = 0, *serialized_asue_cert_buf_ptr = 0;
    uint32                       input_asue_cert_buf_len = 0;
    
    if(0 == req_ptr || 0 == serialized_req_pptr)
    {
        return 0;
    }
    config_ptr = &req_ptr->ssid_config;
    if(config_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA ||
        config_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA2)
    {
        if(config_ptr->credential.wpa_credential.credential_type == 
            WIFISUPP_WPA_CREDENTIAL_TYPE_EAP)
        {
            if(config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_type == 
                WIFISUPP_WPA_EAP_TYPE_TLS)
            {
                if(!config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.tls_info.is_use_file)
                {
                    input_tls_cert_buf_ptr = config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.tls_info.certificate_buf_ptr;
                    input_tls_cert_buf_len = config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.tls_info.certificate_buf_len;
                    
                    serialized_size += input_tls_cert_buf_len;
                    serialized_ptr = CFL_ALLOC(serialized_size);
                    if(0 == serialized_ptr)
                    {
                        //SCI_TRACE_LOW:"WIFISUPP SerializeConnectReq tls using buffer alloc fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_255_112_2_18_0_36_22_2288,(uint8*)"");
                        return 0;
                    }
                    CFL_MEMSET(serialized_ptr, 0, serialized_size);
                    *serialized_ptr = *req_ptr;
                    serialized_tls_cert_buf_ptr = 
                        serialized_ptr->ssid_config.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.tls_info.certificate_buf_ptr;
                    serialized_tls_cert_buf_ptr = (uint8*)serialized_ptr+sizeof(WIFISUPP_SIG_CONNECT_REQ_T);
                    CFL_MEMCPY(
                        serialized_tls_cert_buf_ptr,
                        input_tls_cert_buf_ptr,
                        input_tls_cert_buf_len
                        );
                }
                else
                {
                    //do not need serialization
                    serialized_ptr = CFL_ALLOC(serialized_size);
                    if(0 == serialized_ptr)
                    {
                        //SCI_TRACE_LOW:"WIFISUPP SerializeConnectReq tls using file alloc fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_275_112_2_18_0_36_22_2289,(uint8*)"");
                        return 0;
                    }
                    CFL_MEMSET(serialized_ptr, 0, serialized_size);
                    *serialized_ptr = *req_ptr;
                }
            }
            else if(config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_type ==
                        WIFISUPP_WPA_EAP_TYPE_FAST)
            {
                if(!config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.fast_info.is_use_file)
                {
                    input_fast_pac_buf_ptr = config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.fast_info.pac_buf_ptr;
                    input_fast_pac_buf_len = config_ptr->credential.wpa_credential.credential_info.wpa_eap_info.eap_info.fast_info.pac_buf_len;

                    serialized_size += input_fast_pac_buf_len;
                    serialized_ptr = CFL_ALLOC(serialized_size);
                    if(0 == serialized_ptr)
                    {
                        //SCI_TRACE_LOW:"WIFISUPP SerializeConnectReq fast using buffer alloc fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_294_112_2_18_0_36_22_2290,(uint8*)"");
                        return 0;
                    }
                    
                    CFL_MEMSET(serialized_ptr, 0, serialized_size);
                    *serialized_ptr = *req_ptr;

                    serialized_fast_pac_buf_ptr = 
                        serialized_ptr->ssid_config.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.fast_info.pac_buf_ptr;
                    serialized_fast_pac_buf_ptr = (uint8*)serialized_ptr+sizeof(WIFISUPP_SIG_CONNECT_REQ_T);

                    CFL_MEMCPY(
                        serialized_fast_pac_buf_ptr,
                        input_fast_pac_buf_ptr,
                        input_fast_pac_buf_len
                        );
                }
                else
                {
                    //do not need serialization
                    serialized_ptr = CFL_ALLOC(serialized_size);
                    if(0 == serialized_ptr)
                    {
                        //SCI_TRACE_LOW:"WIFISUPP SerializeConnectReq fast using file alloc fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_317_112_2_18_0_36_23_2291,(uint8*)"");
                        return 0;
                    }
                    CFL_MEMSET(serialized_ptr, 0, serialized_size);
                    *serialized_ptr = *req_ptr;
                }
            }
            else
            {
                //do not need serialization
                serialized_ptr = CFL_ALLOC(serialized_size);
                if(0 == serialized_ptr)
                {
                    //SCI_TRACE_LOW:"WIFISUPP SerializeConnectReq other eap alloc fail"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_330_112_2_18_0_36_23_2292,(uint8*)"");
                    return 0;
                }
                CFL_MEMSET(serialized_ptr, 0, serialized_size);
                *serialized_ptr = *req_ptr;
            }
        }
        else //credential_type == WIFISUPP_WPA_CREDENTIAL_TYPE_PSK
        {
            //do not need serialization
            serialized_ptr = CFL_ALLOC(serialized_size);
            if(0 == serialized_ptr)
            {
                //SCI_TRACE_LOW:"WIFISUPP SerializeConnectReq wpa psk alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_343_112_2_18_0_36_23_2293,(uint8*)"");
                return 0;
            }
            CFL_MEMSET(serialized_ptr, 0, serialized_size);
            *serialized_ptr = *req_ptr;
        }
    }
    else if ((WIFISUPP_ENCRYP_PROTOCOL_WAPI == config_ptr->encryp_protocol) && 
    (WIFISUPP_WAPI_CREDENTIAL_TYPE_CER == config_ptr->credential.wapi_credential.credential_type))
    {
        input_as_cert_buf_ptr = config_ptr->credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr;
        input_as_cert_buf_len = config_ptr->credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_len;
        input_asue_cert_buf_ptr = config_ptr->credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr;
        input_asue_cert_buf_len = config_ptr->credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_len;

        serialized_size = serialized_size + input_as_cert_buf_len + input_asue_cert_buf_len;
        serialized_ptr = CFL_ALLOC(serialized_size);
        if(PNULL == serialized_ptr)
        {
            //SCI_TRACE_LOW:"WIFI:error:SUPP:%s as using buffer alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_362_112_2_18_0_36_23_2294,(uint8*)"s", __func__);
            return 0;
        }
        
        CFL_MEMSET(serialized_ptr, 0, serialized_size);
        /*原有结构*/
        *serialized_ptr = *req_ptr;
        /*根证书*/
        serialized_as_cert_buf_ptr   = (uint8 *)serialized_ptr + sizeof(WIFISUPP_SIG_CONNECT_REQ_T);
        CFL_MEMCPY(serialized_as_cert_buf_ptr, input_as_cert_buf_ptr, input_as_cert_buf_len);
        serialized_ptr->ssid_config.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr = serialized_as_cert_buf_ptr;
        /*用户证书*/
        serialized_asue_cert_buf_ptr = (uint8 *)serialized_as_cert_buf_ptr + input_as_cert_buf_len;
        CFL_MEMCPY(serialized_asue_cert_buf_ptr, input_asue_cert_buf_ptr, input_asue_cert_buf_len);
        serialized_ptr->ssid_config.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr = serialized_asue_cert_buf_ptr;
    }
    else  //encryp_protocol != WPA/WAPI
    {
        //do not need serialization
        serialized_ptr = CFL_ALLOC(serialized_size);
        if(0 == serialized_ptr)
        {
            //SCI_TRACE_LOW:"WIFISUPP SerializeConnectReq wep alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_384_112_2_18_0_36_23_2295,(uint8*)"");
            return 0;
        }
        CFL_MEMSET(serialized_ptr, 0, serialized_size);
        *serialized_ptr = *req_ptr;
    }
    
    *serialized_req_pptr = serialized_ptr;
    return serialized_size;
}

/****************************************************************************/
//Description : 
//Global resource dependence : 
//Author: George.Liu
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_GetMac(uint8 mac_addr[6])
{
    WIFI_ADDR_T addr = {0};
    uint32      ret  = 0;
#ifndef WIN32
#ifndef WIFI_SUPPORT_UNISOC_RX
    ret = WIFI_GetNvParam(&addr, 0);
    if (WIFI_RET_OK != ret) 
    {
        //SCI_TRACE_LOW:"WIFI:error:Supp:GetMac failed %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_407_112_2_18_0_36_23_2296,(uint8*)"d", ret);
        return FALSE;
    }
#endif
#endif

    memcpy(mac_addr, addr.address, 6); 
    
    //SCI_TRACE_LOW:"WIFI:info:Supp:GetMAC is %x-%x-%x-%x-%x-%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_API_414_112_2_18_0_36_23_2297,(uint8*)"dddddd", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    return TRUE;
}


/****************************************************************************/
//Description : 
//Global resource dependence : 
//Author: George.Liu
//Note: 
/****************************************************************************/
extern PUBLIC BOOLEAN WifiCommand(WIFISUPP_COMMAND_INFO_T *command_info_ptr);
PUBLIC BOOLEAN  WIFISUPP_Command(WIFISUPP_COMMAND_INFO_T *command_info_ptr)
{
#ifdef WIN32
     return TRUE;//WifiCommand(command_info_ptr);
#else
    return WifiCommand(command_info_ptr);
#endif
}


