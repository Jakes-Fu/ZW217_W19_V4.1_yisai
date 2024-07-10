/****************************************************************************************
** File Name:      wifisupp_adap_none.c                                                 *
** Author:         juan.zhang                                                           *
** Date:           2009.10.29                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This is the file that a custom supplicant to start with              *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.10       juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/

#ifndef WIN32
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "cfl_mem.h"
#include "wifisupp_internal.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//customized signals are added here
typedef enum
{
    NONE_MESSAGE_START = WIFISUPP_SIG_CUSTOM_START,
    NONE_MESSAGE_MAX = WIFISUPP_SIG_CUSTOM_END
}NONE_MESSAGE_E;

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
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN NoneWifiOn(void);
LOCAL BOOLEAN NoneWifiOff(void);

LOCAL BOOLEAN NoneWifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr);

LOCAL BOOLEAN NoneWifiConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr);
LOCAL BOOLEAN NoneWifiDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr);
LOCAL void NoneHandleCusmsg(uint32 msg_id, void *msg_body_ptr);
LOCAL BOOLEAN NoneWifiGetConnectionInfo(
                                       WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr //[OUT]
                                       );
/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
SUPPADAP_CUSTOM_INFO_T g_suppadap_none_info = 
{
    {
        NoneWifiOn,
        NoneWifiOff,
        NoneWifiScan,
        NoneWifiConnect,
        NoneWifiDisconnect,
        NoneHandleCusmsg,
        NoneWifiGetConnectionInfo,
        PNULL,
    },  //SUPPADAP_REG_OPER_FUNC_T
    TRUE,//is_support_roam
    {
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_OPENSYS
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_WEP
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_WPA
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_WPA2
        FALSE  //WIFISUPP_ENCRYP_PROTOCOL_WAPI
    },  //is_support_protocol
    {
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_TLS
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_TTLS
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_PEAP
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_LEAP
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_SIM
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_AKA
        FALSE    //WIFISUPP_WPA_EAP_TYPE_FAST
    },  //is_support_eap
    {
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_CHAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_PAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP_V2
        FALSE    //WIFISUPP_INNER_CREDENTIAL_TYPE_GTC
    }//is_support_inner_eap
};

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//Description : wifi on
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN NoneWifiOn(void)
{
    WIFISUPP_ON_RESULT_T on_result = {0};
    
    on_result.wifi_on_result = WIFISUPP_RESULT_SUCC;

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &on_result);
    return TRUE;
}

/****************************************************************************/
//Description : wifi off
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN NoneWifiOff(void)
{
    WIFISUPP_OFF_RESULT_T off_result = {0};

    off_result.wifi_off_result = WIFISUPP_RESULT_SUCC;
    SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &off_result);
    return TRUE;
}

/****************************************************************************/
//Description : scan
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN NoneWifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr)
{
    WIFISUPP_SCAN_AP_INFO_T ap_info = {0};
    WIFISUPP_RESULT_E scan_result = 0;

    //SSID
    _snprintf((char *)ap_info.ssid.ssid, sizeof(ap_info.ssid.ssid) - 1, "%s", "TestSSID");
    ap_info.ssid.ssid_len = strlen((char *)ap_info.ssid.ssid);
    
    //BSSID
    _snprintf((char *)ap_info.bssid_info.bssid, sizeof(ap_info.bssid_info.bssid) - 1, "%s", "A1:B2:C3:D4:E5:F6");

    //channel
    ap_info.channel = 6;

    //rssi
    ap_info.signal_qua = 0;

    //rate : 54M
    ap_info.rate = 54 * 1024 * 1024;

    //ecrypt method
    ap_info.encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_WPA2;

    //send to app
    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_IND, &ap_info);
    scan_result = WIFISUPP_RESULT_SUCC;
    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_CNF, &scan_result);

    return TRUE;
}
/****************************************************************************/
//Description : connect 
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN NoneWifiConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr)
{
    WIFISUPP_CONNECT_RESULT_T connect_result = {0};

    connect_result.result = WIFISUPP_RESULT_SUCC;
    //SSID
    _snprintf((char *)connect_result.ssid.ssid, sizeof(connect_result.ssid.ssid) - 1, "%s", "TestSSID");
    connect_result.ssid.ssid_len = strlen((char *)connect_result.ssid.ssid);

    //BSSID
    _snprintf((char *)connect_result.bssid_info.bssid, sizeof(connect_result.bssid_info.bssid) - 1, "%s", "A1:B2:C3:D4:E5:F6");

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_CONNECT_CNF, &connect_result);
    return TRUE;
}

/****************************************************************************/
//Description : disconnect
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN NoneWifiDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr)
{
    WIFISUPP_DISCONNECT_RESULT_T disc_result = {0};

    //result
    disc_result.result = WIFISUPP_RESULT_SUCC;
    
    //SSID
    _snprintf((char *)disc_result.ssid.ssid, sizeof(disc_result.ssid.ssid) - 1, "%s", "TestSSID");
    disc_result.ssid.ssid_len = strlen((char *)disc_result.ssid.ssid);

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_DISCONNECT_CNF, &disc_result);
    return TRUE;
}

/****************************************************************************/
//Description : to handle the customarized message
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void NoneHandleCusmsg(uint32 msg_id, void *msg_body_ptr)
{
	;
}

/****************************************************************************/
//Description : get the current connected AP's information
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN NoneWifiGetConnectionInfo(
                                       WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr //[OUT]
                                       )
{
    return TRUE;
}
#endif