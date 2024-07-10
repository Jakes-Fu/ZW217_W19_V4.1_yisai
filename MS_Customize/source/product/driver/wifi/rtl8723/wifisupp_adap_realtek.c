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


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "cfl_mem.h"
#include "wifisupp_internal.h"
#include "rtl8723_wifi_adapter.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//customized signals are added here
typedef enum
{
    ATHEROS_MESSAGE_START = WIFISUPP_SIG_CUSTOM_START,
    ATHEROS_MESSAGE_MAX = WIFISUPP_SIG_CUSTOM_END
}ATHEROS_MESSAGE_E;

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
LOCAL BOOLEAN Rtl8723WifiOn(void);
LOCAL BOOLEAN Rtl8723WifiOff(void);
LOCAL BOOLEAN Rtl8723WifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr);
LOCAL BOOLEAN Rtl8723WifiConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr);
LOCAL BOOLEAN Rtl8723WifiDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr);
LOCAL void Rtl8723HandleCusmsg(uint32 msg_id, void *msg_body_ptr);
LOCAL BOOLEAN Rtl8723WifiGetConnectionInfo(
                                       WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr //[OUT]
                                       );

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
SUPPADAP_CUSTOM_INFO_T g_suppadap_realtek_info = 
{
    {
        Rtl8723WifiOn,
        Rtl8723WifiOff,
        Rtl8723WifiScan,
        Rtl8723WifiConnect,
        Rtl8723WifiDisconnect,
        Rtl8723HandleCusmsg,
        Rtl8723WifiGetConnectionInfo,
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


extern BOOLEAN rtl8723_wifi_on(void); 
extern BOOLEAN rtl8723_wifi_off(void); 
extern BOOLEAN rtl8723_wifi_scan(WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr);
extern BOOLEAN rtl8723_wifi_connect(WIFISUPP_SSID_CONFIG_T *connect_info_ptr);
extern BOOLEAN rtl8723_wifi_disconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconnect_info_ptr);
extern void rtl8723_handle_cus_msg(uint32 msg_id, void *msg_body_ptr);
extern BOOLEAN rtl8723_get_connection_info(WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr);

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
LOCAL BOOLEAN Rtl8723WifiOn(void)
{
	//SCI_TRACE_LOW:"&&&&&&&&&Rtl8723WifiOn()\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_ADAP_REALTEK_116_112_2_18_0_36_19_2251,(uint8*)"");
	return rtl8723_wifi_on();
}

/****************************************************************************/
//Description : wifi off
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Rtl8723WifiOff(void)
{
	//SCI_TRACE_LOW:"&&&&&&&&&Rtl8723WifiOff()\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_ADAP_REALTEK_126_112_2_18_0_36_19_2252,(uint8*)"");
	return rtl8723_wifi_off();
}

/****************************************************************************/
//Description : scan
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Rtl8723WifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr)
{
	//SCI_TRACE_LOW:"&&&&&&&&&Rtl8723WifiScan()\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_ADAP_REALTEK_136_112_2_18_0_36_19_2253,(uint8*)"");
	return rtl8723_wifi_scan(scan_req_ptr);
}

/****************************************************************************/
//Description : connect 
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Rtl8723WifiConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr)
{
	//SCI_TRACE_LOW:"&&&&&&&&&Rtl8723WifiConnect()\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_ADAP_REALTEK_146_112_2_18_0_36_19_2254,(uint8*)"");
	return rtl8723_wifi_connect(conn_req_ptr);
}

/****************************************************************************/
//Description : disconnect
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Rtl8723WifiDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr)
{
	//SCI_TRACE_LOW:"&&&&&&&&&Rtl8723WifiDisconnect()\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_ADAP_REALTEK_156_112_2_18_0_36_19_2255,(uint8*)"");
    	return rtl8723_wifi_disconnect(disconn_req_ptr);
}

/****************************************************************************/
//Description : to handle the customarized message
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void Rtl8723HandleCusmsg(uint32 msg_id, void *msg_body_ptr)
{
	//SCI_TRACE_LOW:"&&&&&&&&&Rtl8723HandleCusmsg()\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_ADAP_REALTEK_166_112_2_18_0_36_19_2256,(uint8*)"");
	//return;
	rtl8723_handle_cus_msg(msg_id,msg_body_ptr);
}

/****************************************************************************/
//Description : get the current connected AP's information
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Rtl8723WifiGetConnectionInfo(
                                       WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr //[OUT]
                                       )
{
	//SCI_TRACE_LOW:"&&&&&&&&&Rtl8723WifiGetConnectionInfo()\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_ADAP_REALTEK_179_112_2_18_0_36_19_2257,(uint8*)"");
	//return 0;
	return rtl8723_get_connection_info(connection_info_ptr);
}


