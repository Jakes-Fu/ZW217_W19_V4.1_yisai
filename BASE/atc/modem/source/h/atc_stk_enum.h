#ifndef _ATC_STK_ENUM_H_
#define _ATC_STK_ENUM_H_
/**************************************************************************

Copyright (C) 2001 SPREADTRUM COMMUNICATION CO.
All rights reserved.

This document contains proprietary information blonging to
SPREADTRUM. Passing on and copying this document, use and
communication of its contents is not permitted without prior
writed authorization.

***************************************************************************/

/***************************************************************************

Reversion information:
File Name:    atc_stk_enum.h
Version:      0.1
Author:       Bin.Li
Date:         2009.4
          enum macro define
***************************************************************************/
/***************************************************************************

            Edit Histroy
Date          Name            Description
--------------------------------------------------------------------------
2009.4        Bin.Li    Create
***************************************************************************/

#include "sci_types.h"
#include "simat_outport.h"
#include "sim_type.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define SIMAT_COMMAND_DETAIL_QULIFIER_POS   4   //the command qualifier
#define SIMAT_COMMAND_DETAIL_LEN    5   //the command detail data object buf len
#define SIMAT_COMMAND_DETAIL_L_VALUE    3   //the legnth vluae of command detail data object TLV
#define SIMAT_CD_TIEMR_OEPRATION_MARK   0x03

#define SIMAT_SHARE_BUF_LEN     800
#define SIMAT_SHARE_BUF_NUM     5

#define SIMAT_BTAG_MARK     0x7f
#define SIMAT_BTAG_CR_MARK  0x10
#define SIMAT_PC_INFO_LEN   5
#define SIMAT_MIN_PC_LEN    9       //the command detail and device identities is minimal length 
#define SIMAT_MIN_TLV_LEN   2

//the proactive command type
typedef enum
{
    SIMAT_PC_REFRESH,
    SIMAT_PC_MORE_TIME,
    SIMAT_PC_POLLING_INTERVAL,
    SIMAT_PC_POLLING_OFF,
    SIMAT_PC_SETUP_EVENT_LIST,
    SIMAT_PC_SETUP_CALL,
    SIMAT_PC_SEND_SS,
    SIMAT_PC_SEND_USSD,
    SIMAT_PC_SMS,
    SIMAT_PC_SEND_DTMF,
    SIMAT_PC_LAUNCH_BROWSER,
    SIMAT_PC_PLAY_TONE,
    SIMAT_PC_DISPLAY_TEXT,
    SIMAT_PC_GET_INKEY,
    SIMAT_PC_GET_INPUT,
    SIMAT_PC_SELECT_ITEM,
    SIMAT_PC_SETUP_MENU,
    SIMAT_PC_PROVIDE_LOCATION_INFO,
    SIMAT_PC_TIMER_MANAGEMENT,
    SIMAT_PC_SETUP_IDLE_MODE_TEXT,
    SIMAT_PC_CARD_APDU,
    SIMAT_PC_POWER_ON_CARD,
    SIMAT_PC_POWER_OFF_CARD,
    SIMAT_PC_GET_CARD_STATUS,
    SIMAT_PC_RUN_AT_COMMAND,
    SIMAT_PC_LANG_NOTIFICATION,
    SIMAT_PC_OPEN_CHANNEL,
    SIMAT_PC_CLOSE_CHANNEL,
    SIMAT_PC_RECEIVE_DATA,
    SIMAT_PC_SEND_DATA,
    SIMAT_PC_GET_CHANNEL_STATUS,
    SIMAT_PC_RETRIEVE_MUITIMEDIA_MSG,
    SIMAT_PC_SUBMIT_MUITIMEDIA_MSG,
    SIMAT_PC_DISPLAY_MUITIMEDIA_MSG,

    SIMAT_PC_NUM
}
SIMAT_PC_NAME_E;

typedef enum
{
    SIMAT_DO_DECODE_OK = 0x0,
    SIMAT_DO_CONTENTS_NOT_UNDERSTOOD,
    SIMAT_DO_RFU,
    SIMAT_DO_LENGTH_ERROR,
    SIMAT_DO_ERROR_TAG,
    SIMAT_DO_SERIOUS,
    SIMAT_DO_ERROR_SHARE_MEM
} SIMAT_DO_DECODE_RESULT_E;

typedef enum
{
    SIMAT_DE_KEYPAD = 0x01,
    SIMAT_DE_DISPLAY,
    SIMAT_DE_EARPIECE,
    /*+CR174286*/
    SIMAT_DE_CHANNEL1 = 0x21,
    SIMAT_DE_CHANNEL2,
    SIMAT_DE_CHANNEL3,
    SIMAT_DE_CHANNEL4,
    SIMAT_DE_CHANNEL5,
    SIMAT_DE_CHANNEL6,
    SIMAT_DE_CHANNEL7,
    SIMAT_DE_SIM = 0x81,
    SIMAT_DE_ME,
    SIMAT_DE_NETWORK
} SIMAT_DEVICE_ID_E;
//all the following tag values reference to section 13 "Tag values" of GSM11.14
//the BER-TLV tags in ME to SIM direction
typedef enum
{
    SIMAT_DTAG_SMS_PP = 0xD1,
    SIMAT_DTAG_SMS_CB,
    SIMAT_DTAG_MENU_SELECTION,
    SIMAT_DTAG_CC,
    SIMAT_DTAG_MO_SM_CC,
    SIMAT_DTAG_EVENT_DOWNLOAD,
    SIMAT_DTAG_TIME_EXPIRATION
} SIMAT_ME_SIM_TAG_E;

//the BER-TLV tags in SIM to ME direction
typedef enum
{
    SIMAT_UTAG_PC = 0xD0
} SIMAT_SIM_ME_TAG_E;


//SIMPLE-TLV tags in both directions
typedef enum
{
    SIMAT_BTAG_COMMAND_DETAIL = 0x01,
    SIMAT_BTAG_DEVICE_ID,
    SIMAT_BTAG_RESULT,
    SIMAT_BTAG_DURATION,
    SIMAT_BTAG_ALPHA_ID,
    SIMAT_BTAG_ADDRESS,
    SIMAT_BTAG_CCP,
    SIMAT_BTAG_CPS,
    SIMAT_BTAG_SS_STRING,
    SIMAT_BTAG_USSD_STRING,

    SIMAT_BTAG_SMS_TPDU,
    SIMAT_BTAG_CB_PAGE,
    SIMAT_BTAG_TEXT_STRING,
    SIMAT_BTAG_TONE,
    SIMAT_BTAG_ITEM,
    SIMAT_BTAG_ITEM_ID,
    SIMAT_BTAG_RESPONSE_LEN,
    SIMAT_BTAG_FILE_LIST,
    SIMAT_BTAG_LOCATION_INFO,
    SIMAT_BTAG_IMEI,

    SIMAT_BTAG_HELP_REQ,
    SIMAT_BTAG_NMR,       //the Network Measurement Result
    SIMAT_BTAG_DEFAULT_TEXT,
    SIMAT_BTAG_ITEM_NEXT_ACTION_INDICATOR,
    SIMAT_BTAG_EVENT_LIST,
    SIMAT_BTAG_CAUSE,
    SIMAT_BTAG_LOCATION_STATUS,
    SIMAT_BTAG_TRANSACTION_ID,
    SIMAT_BTAG_BCCH_LIST,
    SIMAT_BTAG_ICON_ID,

    SIMAT_BTAG_ICON_ID_LIST,
    SIMAT_BTAG_CARD_READER_STATUS,
    SIMAT_BTAG_CARD_ATR,
    SIMAT_BTAG_C_APDU,
    SIMAT_BTAG_R_APDU,
    SIMAT_BTAG_TIMER_ID,
    SIMAT_BTAG_TIMER_VALUE,
    SIMAT_BTAG_DT_AND_TZ,       //Data time and time zone
    SIMAT_BTAG_CC_REQ_ACTION,
    SIMAT_BTAG_AT_COMMAND,

    SIMAT_BTAG_AT_RESPONSE,
    SIMAT_BTAG_BC_REPEAT_INDICATOR,
    SIMAT_BTAG_IMMEDIATE_RESPONSE,
    SIMAT_BTAG_DTMF_STRING,
    SIMAT_BTAG_LANGUAGE,
    SIMAT_BTAG_TIME_ADVANCE, /* 2E */

    /*+ bin.li add for usat */
    SIMAT_BTAG_AID,
    SIMAT_BTAG_BROWSER_ID,
    SIMAT_BTAG_URL,
    SIMAT_BTAG_BEARER,
    SIMAT_BTAG_PROVISION_PRE,
    SIMAT_BTAG_BROWSER_TERMINAT_CAUSE,
    SIMAT_BTAG_BEAR_DESC,
    SIMAT_BTAG_CHANNEL_DATA,   /*0x36*/
    SIMAT_BTAG_CHANNEL_DATA_LEN,
    SIMAT_BTAG_CHANNEL_STATUS,
    SIMAT_BTAG_BUFF_SIZE,
    SIMAT_BTAG_CARD_READ_ID,
    SIMAT_BTAG_FILE_UPDATE_INFO,
    SIMAT_BTAG_INTERFACE_TRANS_LEVEL,
    SIMAT_BTAG_NOT_USED,
    SIMAT_BTAG_OTHER_ADDR,
    SIMAT_BTAG_ACCESS_TECH, /* 0x3f */
    SIMAT_BTAG_DISPLAY_PARA,
    SIMAT_BTAG_SERVICE_RECORD,
    SIMAT_BTAG_DEVICE_FILTER,
    SIMAT_BTAG_SERVICE_SEARCH,
    SIMAT_BTAG_ATRIBUTE_INFO,
    SIMAT_BTAG_SERVICE_AVAILABLE,
    SIMAT_BTAG_ESN,
    SIMAT_BTAG_NETWORK_ACCESS_NAME,


    SIMAT_BTAG_CDMA_SMS_TPDU,
    SIMAT_BTAG_REMOTE_ENTITY_ADDR,
    SIMAT_BTAG_I_WLAN_ID,
    SIMAT_BTAG_I_WLAN_ACCESS_STATUS, /*0x4B*/

    SIMAT_BTAG_FILL_1,      /*0x4C*/
    SIMAT_BTAG_FILL_2,      /*0x4D*/
    SIMAT_BTAG_FILL_3,      /*0x4E*/
    SIMAT_BTAG_FILL_4,      /*0x4F*/
    SIMAT_BTAG_TEXT_ATRIBUTE = 0x50,
    SIMAT_BTAG_ITEM_TEXT_ATRIBUTE_LIST,
    SIMAT_BTAG_PDP_ACT_PARA,     /* 0x52*/

    SIMAT_BTAG_FILL_5,       /*0x53*/
    SIMAT_BTAG_FILL_6,       /*0x54*/
    SIMAT_BTAG_FILL_7,      /*0x55*/
    SIMAT_BTAG_FILL_8,      /*0x56*/
    SIMAT_BTAG_FILL_9,          /*0x57*/
    SIMAT_BTAG_FILL_10,      /*0x58*/
    SIMAT_BTAG_FILL_11,       /*0x59*/
    SIMAT_BTAG_FILL_12,      /*0x5A*/
    SIMAT_BTAG_FILL_13,      /*0x5B*/
    SIMAT_BTAG_FILL_14,       /*0x5C*/
    SIMAT_BTAG_FILL_15,      /*0x5D*/
    SIMAT_BTAG_FILL_16,       /*0x5E*/
    SIMAT_BTAG_FILL_17,      /*0x5F*/
    SIMAT_BTAG_FILL_18,      /*0x60*/
    SIMAT_BTAG_FILL_19,      /*0x61*/

    SIMAT_BTAG_IMSISV = 0x62,        /* 0x62*/
    SIMAT_BTAG_BATTERY_STATE,
    SIMAT_BTAG_BROWSER_STATE,
    SIMAT_BTAG_NETWORK_SEARCH_MODE,
    SIMAT_BTAG_FRAME_LAYOUT,
    SIMAT_BTAG_FRAME_INFO,
    SIMAT_BTAG_FRAME_ID,
    SIMAT_BTAG_UTRAN_MEASURE_QUALIFIER,
    SIMAT_BTAG_MULTIMEDIA_MSG_REF,
    SIMAT_BTAG_MULTIMEDIA_MSG_ID,
    SIMAT_BTAG_MULTIMEDIA_MSG_TRANS_STATUS,
    SIMAT_BTAG_MULTIMEDIA_MEID,
    SIMAT_BTAG_MULTIMEDIA_MSG_CONTENT_ID,
    SIMAT_BTAG_MULTIMEDIA_MSG_NOTIFICATION,
    SIMAT_BTAG_LAST_ENVELOP,
    SIMAT_BTAG_REGISTRY_APP_DATA,

    SIMAT_BTAG_UNKNOW
} SIMAT_B_DIRECTION_TAG_E;
//the device identity data object structure
//the timer opration type
typedef enum
{
    SIMAT_START_TIMER = 0,
    SIMAT_DEACTIVE_TIMER,
    SIMAT_GET_TIMER_VALUE,
    SIMAT_TIMER_RFU
} SIMAT_TIMER_OPERATION_E;
//the file types in sim card
//used by msk_api_sim.h(87),MSK_SIM_EncodeEFInfo,for SIM_EF_FILE_INFO_T
typedef struct
{
    SIMAT_DEVICE_ID_E source;
    SIMAT_DEVICE_ID_E destination;
} SIMAT_DO_DEVICE_ID_T;

typedef struct
{
    uint32 timer_value;     //the timer value of millisecond
} SIMAT_DO_TIMER_VALUE_T;

#ifdef __cplusplus
}
#endif

#endif  //_ATC_STK_ENUM_H_


