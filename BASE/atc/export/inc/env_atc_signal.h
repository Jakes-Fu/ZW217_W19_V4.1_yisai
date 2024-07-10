/******************************************************************************
 ** File Name:      env_atc_signal.h                                          *
 ** Author:         Ivan Ren                                                  *
 ** DATE:           07/02/2003                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define atc received signals              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/02/2003     Ivan Ren          Create.                                  *
 ******************************************************************************/

#ifndef _ENV_ATC_SIGNAL_H
#define _ENV_ATC_SIGNAL_H

#include "tasks_id.h"
#ifdef MODEM_PLATFORM
//#include "mn_type.h"


#ifndef _RTOS
#define SIGNAL_VARS
#endif


#define ATC_MAX_RSP_STRLEN_FOR_UEIT   300
#define ATC_MAX_RESULT_CODE_LEN_FOR_UEIT   20

/*******************************************************************************
 **                      atc message structure                                 *
 ******************************************************************************/
#ifdef _MUX_ENABLE_
typedef struct
{
    SIGNAL_VARS
    uint32         len;
    uint8           link_id;
    uint8           align[3];
} ATC_MUX_RECV_NEW_AT_T ;

#else/*_MUX_ENABLE_*/
typedef struct
{
    SIGNAL_VARS
    uint32         len;
} SIO_RECV_NEW_AT_T ;
#endif

typedef struct
{
    SIGNAL_VARS
    uint32         len;
    uint8         *str;
    uint8          link_id;
} AT_WRITE_RSP_T ;

typedef struct
{
    SIGNAL_VARS
    uint16      rsp_str_len;
    uint8       rsp_str[ATC_MAX_RSP_STRLEN_FOR_UEIT];
} ATC_UEIT_ATCMD_RSP_IND_T;


typedef struct
{
    SIGNAL_VARS
    uint16      result_code_len;
    uint8       result_code[ATC_MAX_RESULT_CODE_LEN_FOR_UEIT];
} ATC_UEIT_ATCMD_RESULT_CODE_T;
#endif /*MODEM_PLATFORM*/

/*******************************************************************************
 **                      atc message list                                      *
 ******************************************************************************/
#ifdef _MUX_ENABLE_
typedef enum
{
    ATC_MUX_RECV_NEW_AT  = ((P_ATC << 8) | 1) ,
    ATC_CALL_EXP_IND = ((P_ATC << 8) | 2) ,
    ATC_STSF_EXP_IND = ((P_ATC << 8) | 3) ,
    ATC_STSF_EXP_IND1,
    ATC_STSF_EXP_IND2,
    ATC_STSF_EXP_IND3,
    ATC_POWEROFF_EXP_IND ,
    ATC_STKINIT_EXP_IND,
    ATC_NET_RING_EXP_IND,
    ATC_RSSI_READ_EXP_IND,
    ATC_BT_STATUS_IND,
    ATC_BT_SEARCH_DEVICE_IND,
    ATC_CHR_CAP_IND ,
    ATC_CHR_CHARGE_END_IND,
    ATC_CHR_CHARGE_START_IND,
    ATC_CHR_WARNING_IND,
    ATC_CHR_SHUTDOWN_IND,
    ATC_CHR_FINISH_IND,
    ATC_CHR_DISC_IND,
    ATC_CHR_FAULT_IND,
    ATC_WRITE_RSP_REQ,
    ATC_MUX_LINK_SETUP_REQ,
    ATC_MUX_LINK_RELEASE_REQ,
    ATC_MUX_CLOSE_REQ,
    ATC_CALL_DTMF_EXP_IND,
    ATC_UEIT_ATCMD_RSP_IND,
    ATC_UEIT_ATCMD_RESULT_CODE,
    ATC_PPP_ACTIVE_PDP_REQ,
    ATC_CELL_PERIOD_EXP_IND,
#ifdef FOTA_SUPPORT_ADUPS    
	ATC_ADUPS_INIT_MSG_IND,
	ATC_ADUPS_AT_MSG_IND,
	ATC_ADUPS_TIMER_MSG_IND,
#endif
    AT_BT_GATT_SERVER_REG_READ_RES,       
    AT_BT_GATT_SERVER_REG_WRITE_RES,  
    AT_BT_GATT_SERVER_DEREG_WRITE_RES, 
    AT_BT_GATT_SERVER_SERVICE_READ_RES,
    AT_BT_GATT_SERVER_SERVICE_ADD_WRITE_RES,
    AT_BT_GATT_SERVER_SERVICE_DEL_WRITE_RES,
    AT_BT_GATT_SERVER_CHAR_WRITE_RES, 
    AT_BT_GATT_SERVER_DESC_WRITE_RES, 
    AT_BT_GATT_SERVER_SERVICE_START_WRITE_RES,
    AT_BT_GATT_SERVER_SERVICE_STOP_WRITE_RES,
    AT_BT_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES,
    AT_BT_GATT_SERVER_REQ_RECIEVE_IND, 
    AT_BT_GATT_SERVER_RSP_WRITE_RES,
    AT_BT_GATT_SERVER_CONN_READ_RES,
    AT_BT_GATT_SERVER_DISCONN_WRITE_RES,

    END_ATC_SIGNAL
} ATC_MN_SIGNAL_E;

#else

typedef enum
{
    SIO_RECV_NEW_AT  = ((P_ATC << 8) | 1) ,
    ATC_CALL_EXP_IND = ((P_ATC << 8) | 2) ,
    ATC_STSF_EXP_IND = ((P_ATC << 8) | 3) ,
    ATC_STSF_EXP_IND1,
    ATC_STSF_EXP_IND2,
    ATC_STSF_EXP_IND3,
    ATC_POWEROFF_EXP_IND ,
    ATC_STKINIT_EXP_IND,
    ATC_NET_RING_EXP_IND,
    ATC_RSSI_READ_EXP_IND,
    ATC_BT_STATUS_IND,
    ATC_BT_SEARCH_DEVICE_IND,
    ATC_CHR_CAP_IND ,
    ATC_CHR_CHARGE_END_IND,
    ATC_CHR_CHARGE_START_IND,
    ATC_CHR_WARNING_IND,
    ATC_CHR_SHUTDOWN_IND,
    ATC_CHR_FINISH_IND,
    ATC_CHR_DISC_IND,
    ATC_CHR_FAULT_IND,
    ATC_WRITE_RSP_REQ,
    ATC_CALL_DTMF_EXP_IND,
    ATC_UEIT_ATCMD_RSP_IND,
    ATC_UEIT_ATCMD_RESULT_CODE,
    ATC_CELL_PERIOD_EXP_IND,
#ifdef FOTA_SUPPORT_ADUPS    
    ATC_ADUPS_INIT_MSG_IND,
	ATC_ADUPS_AT_MSG_IND,
	ATC_ADUPS_TIMER_MSG_IND,
#endif
    AT_BT_GATT_SERVER_REG_READ_RES,       
    AT_BT_GATT_SERVER_REG_WRITE_RES,  
    AT_BT_GATT_SERVER_DEREG_WRITE_RES, 
    AT_BT_GATT_SERVER_SERVICE_READ_RES,
    AT_BT_GATT_SERVER_SERVICE_ADD_WRITE_RES,
    AT_BT_GATT_SERVER_SERVICE_DEL_WRITE_RES,
    AT_BT_GATT_SERVER_CHAR_WRITE_RES, 
    AT_BT_GATT_SERVER_DESC_WRITE_RES, 
    AT_BT_GATT_SERVER_SERVICE_START_WRITE_RES,
    AT_BT_GATT_SERVER_SERVICE_STOP_WRITE_RES,
    AT_BT_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES,
    AT_BT_GATT_SERVER_REQ_RECIEVE_IND, 
    AT_BT_GATT_SERVER_RSP_WRITE_RES,
    AT_BT_GATT_SERVER_CONN_READ_RES,
    AT_BT_GATT_SERVER_DISCONN_WRITE_RES,

    END_ATC_SIGNAL

} ATC_MN_SIGNAL_E;
#endif
#endif
