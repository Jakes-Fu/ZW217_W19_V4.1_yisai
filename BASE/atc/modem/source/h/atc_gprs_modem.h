/******************************************************************************
 ** File Name:      atc_gprs_modem.h                                          *
 ** Author:         Hyman.wu                                                  *
 ** DATE:           14/10/2004                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains atc functionss about GPRS data transfer*
 ******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                *
 ** 14/10/2004     Hyman.wu         Create.                                    *
 ******************************************************************************/
#ifndef _ATC_GPRS_MODEM_H_
#define  _ATC_GPRS_MODEM_H_

#include "socket_types.h"
#include "in_message.h"

#define  MAX_TX_BUF_LEN               1024
#define  MAX_RX_BUF_LEN               400   
#define  MAX_SEND_DATA_LENGTH         1024
#define  MAX_DATACONF_NUM             10
#define  MAX_UDP_SEND_LENGTH          1096 //shijun modify for TFTP client cr35475
#define  UDP_DATACONF                 1
#define  TCP_DATACONF                 2
#define  ATDATALEN                    500

#define  default_apn                   "CMNET"
#define  default_cid                   1
#define  default_netid                 5     //default nsapi value = 5 when default cid =1
#define  MAX_PORT_NUM                  65535

#define  ATC_MAX_IP_STR_LEN            15  //just for IPV4
#define  ATC_MAX_NETWORK_ADDR_LEN      255

enum echo
{
    HEX_ECHO = 0,
    TEXT_ECHO
};

enum mode
{
    AUTO_RETURN = 0,
    READ_RETURN
};

#ifdef _SCM21_ATC_DEV
typedef enum _MODEM_IP_STATE_E
{
    ATC_MODEM_IP_INITIAL,
    ATC_MODEM_IP_START,
    ATC_MODEM_IP_CONFIG,
    ATC_MODEM_IP_GPRSACT,
    ATC_MODEM_IP_STATUS,
    ATC_MODEM_IP_PROCESSING,
    ATC_MODEM_IP_CONNECT_OK,
    ATC_MODEM_IP_CLOSING,
    ATC_MODEM_IP_CLOSED,
    ATC_MODEM_IP_GPRSDEACT
}MODEM_IP_STATE_E;
typedef enum _CLIENT_STATE_E
{
    INITIAL,
    CONNECTING,
    REMOTE_CLOSING,
    CLOSING,
    CLOSED
}CLIENT_STATE_E;
typedef enum _SERVER_STATE_E
{
    OPENING,
    LISTENING,
    S_CLOSING
}SERVER_STATE_E;
#endif //_SCM21_ATC_DEV

enum modem_state_e
{
    ATC_MODEM_IDLE,
    ATC_MODEM_SET,
    ATC_MODEM_ACTIVE,
    ATC_MODEM_SEND    // add by longting.zhao
};

//for  socket
struct at_socket_info
{
    uint8 sock_index_id;   // add by longting.zhao
    uint8 link_id;
    uint16 lport;
    uint16 fport;
    uint32 fhost;
    //void *socket;
    TCPIP_SOCKET_T socket;
    uint32 type;
    uint32 state;
    TCPIP_HOST_HANDLE sock_request_id;   // add by longting.zhao    
    char *addr_str;
    char ip_addr_str[ATC_MAX_NETWORK_ADDR_LEN];
#ifdef _SCM21_ATC_DEV
	MODEM_IP_STATE_E modem_state; //add by gang.li for SCM21 modem
#endif //_SCM21_ATC_DEV
};

typedef struct 
{
    int32  data_len;
    int32  unsend_len;
    char   buf[MAX_TX_BUF_LEN];   
}ATC_TX_BUF_T;

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
void IPOPEN_ProcessActPdpContextCnf(ATC_CONFIG_T *atc_config_ptr,APP_MN_GPRS_T *sig_ptr);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+NETAPN, set net access
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessNETAPN(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+SOCKET, create SOCKET 
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessSOCKET(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPOPEN, creat connect 
//              between MT and server
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPOPEN(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPSEND, send data 
//  Global resource dependence : ????
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPSEND( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPCLOSE, close socKet 
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPCLOSE (   // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPDACTIVE, execute deattachment
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPDEACT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/*****************************************************************************/
//  Description : This function handles Gprs data from the SIO. 
//  Global resource dependence : g_rsp_str
//  Author:      
//  Note:
/*****************************************************************************/
uint32 ATC_ProcessSioData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *pdu_ptr,  // pointer to the given pdu buffer
                uint16          param_len
                );


/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIPMUX, set pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR:
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessCIPMUX( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIPSTART,  
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessCIPSTART(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);


#endif /* _ATC_GPRS_MODEM_H_ */
