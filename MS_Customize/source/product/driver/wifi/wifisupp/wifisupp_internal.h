/*****************************************************************************
** File Name:      wifisupp_internal.h                                       *
** Author:         juan.zhang                                                *
** Date:           10/29/2009                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used for wifi supplicant internal definition *
**                and declaration                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/29/2009     juan.zhang          Create                                 *
******************************************************************************/

#ifndef _WIFISUPP_INTERNAL_H_
#define _WIFISUPP_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "in_message.h"
#include "wifisupp_api.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#define __func__ __FILE__
#endif
/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/
//To open WiFi
typedef BOOLEAN (*SUPPADAP_ON_FUNC)(void); 

//To close WiFi
typedef BOOLEAN (*SUPPADAP_OFF_FUNC)(void); 

typedef BOOLEAN (*SUPPADAP_SCAN_FUNC)(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr); 

//To connect to an AP
typedef BOOLEAN (*SUPPADAP_CONNECT_FUNC)(WIFISUPP_SSID_CONFIG_T *connect_info_ptr);

//To disconnect an AP
typedef BOOLEAN (*SUPPADAP_DISCONNECT_FUNC)(WIFISUPP_DISCONNECT_REQ_INFO_T *disconnect_info_ptr);

//To Handle the customarized message
typedef void (*SUPPADAP_HANDLE_CUS_MSG_FUNC)(uint32 msg_id, void *msg_body_ptr);

//To get current connected AP's information.
typedef BOOLEAN (*SUPPADAP_GET_CONNECTION_INFO_FUNC)(WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr);

typedef BOOLEAN (*SUPPADAP_COMMAND_FUNC)(WIFISUPP_COMMAND_INFO_T *command_info_ptr);

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
    SUPPADAP_ON_FUNC                    wifi_on_func;                   //To open WiFi
    SUPPADAP_OFF_FUNC                   wifi_off_func;                  //To close WiFi
    SUPPADAP_SCAN_FUNC                  wifi_scan_func;                 //To initiate a scanning if no scanning is running and get the scan results by corresponding CNF.  
    SUPPADAP_CONNECT_FUNC               wifi_connect_func;              //To connect to an AP
    SUPPADAP_DISCONNECT_FUNC            wifi_disconnect_func;           //To disconnect an AP      
    SUPPADAP_HANDLE_CUS_MSG_FUNC        wifi_handle_cus_msg_func;       //To Handle the customarized message
    SUPPADAP_GET_CONNECTION_INFO_FUNC   wifi_get_connection_info_func;  //To get current connected AP's information.
    SUPPADAP_COMMAND_FUNC               wifi_command_func;              //call command for wifi drv
} SUPPADAP_REG_OPER_FUNC_T;

typedef struct
{
    SUPPADAP_REG_OPER_FUNC_T    wifi_oper_funcs;
    BOOLEAN                     is_support_roam;
    BOOLEAN                     is_support_protocol[WIFISUPP_ENCRYP_PROTOCOL_MAX];
    BOOLEAN                     is_support_eap[WIFISUPP_WPA_EAP_TYPE_MAX];
    BOOLEAN                     is_support_inner_eap[WIFISUPP_INNER_CREDENTIAL_TYPE_MAX];
} SUPPADAP_CUSTOM_INFO_T;

typedef struct 
{
    _SIGNAL_VARS
    BLOCK_ID    app_thread_id;
}WIFISUPP_SIG_ON_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    WIFISUPP_SSID_CONFIG_T  ssid_config;
}WIFISUPP_SIG_CONNECT_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    WIFISUPP_DISCONNECT_REQ_INFO_T  disc_info;
}WIFISUPP_SIG_DISCONNECT_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    WIFISUPP_SCAN_REQ_INFO_T  scan_info;
} WIFISUPP_SIG_SCAN_REQ_T;

/*---------------------------------------------------------------------------*/
/*                          Global variables                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
//Description : to send signal to app
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void SUPPMAIN_NotifyApp(
                            uint32      msg_id, 
                            void        *msg_info_ptr
                         );

/****************************************************************************/
//Description : to get the wifi supplicant thread ID
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BLOCK_ID SUPPMAIN_GetWifiThreadId(void);

/****************************************************************************/
//Description : to get the wifi APP thread ID
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BLOCK_ID SUPPMAIN_GetAppThreadId(void);

/****************************************************************************/
//Description : to creat wifi supplicant thread
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void SUPPMAIN_CreatThread(void);


#ifdef   __cplusplus
    }
#endif


#endif 

