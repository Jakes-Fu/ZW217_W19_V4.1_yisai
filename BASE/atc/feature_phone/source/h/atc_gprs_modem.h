/******************************************************************************
 ** File Name:      atc_gprs_modem.h                                     *
 ** Author:                                                                   *
 ** DATE:           04/07/2011                                                *
 ** Description:    This file contains functions definitions about GPRS ATC.  *
 ******************************************************************************/
#ifndef  _ATC_GPRS_MODEM_H_
#define  _ATC_GPRS_MODEM_H_
#ifdef ATC_SAT_ENABLE
#include  "socket_types.h"
#include  "atc_plus_gprs.h"
#include "in_message.h"

#define  MAX_DATACONF_NUM 10
#define  MAX_RECEIVE_DATA_LENGTH   400
#define  MAX_SEND_DATA_LENGTH  1024
#define  UDP_DATACONF   1
#define  TCP_DATACONF   2
#define  MAX_PORT_NUM   65535
#define  MAX_CONNECT_ID_NUMBER  10
#define  default_apn   "CMNET"
#define  default_cid   1
#define  default_netid   5     //default nsapi value = 5 when default cid =1
#define  MAX_TX_BUF_LEN   1024   
#define  MAX_RX_BUF_LEN    400   

typedef struct 
{
    int32 data_len;
    int32 unsend_len;
    char   buf[MAX_TX_BUF_LEN];   
}ATC_TX_BUF_T;

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
};

enum modem_state_e
{
    IDLE,
    SET,
    ACTIVE,
    SEND    // add by longting.zhao
};


BOOLEAN ATC_SockIDNotProcessDNS(uint8 *sock_id);
#ifdef TCPIP_SUPPORT
void IPOPEN_ProcessActPdpContextCnf(ATC_CONFIG_T *atc_config_ptr,APP_MN_GPRS_T *sig_ptr);
#endif

/******************************************************************************/
//    DESCRIPTION:This function handles command AT+NETAPN, set net access
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessNETAPN(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);
/******************************************************************************/
//    DESCRIPTION:This function handles command AT+SOCKET, create SOCKET 
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessSOCKET(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);
/******************************************************************************/
//    DESCRIPTION:This function handles command AT+IPOPEN, creat connect 
//                between MT and server
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPOPEN(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);
/******************************************************************************/
//    DESCRIPTION:This function handles command AT+IPSEND, send data 
//    Global resource dependence : ????
//    AUTHOR: 
//    Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPSEND( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//    DESCRIPTION:This function handles command AT+IPCLOSE, close socKet 
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPCLOSE (   // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);
/******************************************************************************/
//    DESCRIPTION:This function handles command AT+IPDACTIVE, execute deattachment
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
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
#endif/*ATC_SAT_ENABLE*/
#endif
