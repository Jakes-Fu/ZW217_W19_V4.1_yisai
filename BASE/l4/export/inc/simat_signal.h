/******************************************************************************
 ** File Name:      simat_signal.h                                            *
 ** Author:         Raistlin.kong                                             *
 ** Date:           10/19/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file of defines signals of the sim     *
 **                 application layer               						  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2001.10        Raistlin.kong    Create									  *
 ** 2003.1         Karin.Li         Update									  *
 ******************************************************************************/
 
 
#ifndef _SIMAT_SIGNAL_H_
#define _SIMAT_SIGNAL_H_

#ifdef __cplusplus
    extern "C"
    {
#endif


//----------------------all layer interface signal----------------------------
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SETUP_EVENT_LIST_CNF_T cnf;
}SIMAT_SIG_SETUP_EVENT_LIST_CNF_T;


//--------------------------MMI interface signal------------------------------
//the DISPLAY TEXT proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_DISPLAY_TEXT_CNF_T cnf;
}SIMAT_SIG_DISPLAY_TEXT_CNF_T;

//the GET INKEY proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_GET_INKEY_CNF_T cnf;
}SIMAT_SIG_GET_INKEY_CNF_T;

//the GET INPUT proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_GET_INPUT_CNF_T cnf;
}SIMAT_SIG_GET_INPUT_CNF_T;

//the PLAY TONE proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_PLAY_TONE_CNF_T cnf;
}SIMAT_SIG_PLAY_TONE_CNF_T;

//the SETUP MENU proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SETUP_MENU_CNF_T cnf;
}SIMAT_SIG_SETUP_MENU_CNF_T;

//the SELECT ITEM proactive command serivce confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SELECT_ITEM_CNF_T cnf;
}SIMAT_SIG_SELECT_ITEM_CNF_T;

//the PROVIDE LOCATION INFORMATION(Get data time and time zone) proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_GET_DT_TZ_CNF_T cnf;
}SIMAT_SIG_GET_DT_TZ_CNF_T;

//the PROVIDE LOCATION INFORMATION(Get Language ME Selected) proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_GET_LANG_CNF_T cnf;
}SIMAT_SIG_GET_LANG_CNF_T;

typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_GET_IMEI_CNF_T cnf;
}SIMAT_SIG_GET_IMEI_CNF_T;

//the SET IDLE MODE TEXT proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SETUP_IDLE_TEXT_CNF_T cnf;
}SIMAT_SIG_SETUP_IDLE_TEXT_CNF_T;

//the MENU SELECTION indicator signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_MENU_SELECTION_IND_T ind;
}SIMAT_SIG_MENU_SELECTION_IND_T;


#ifdef PRODUCT_DM
typedef struct
{
     SIGNAL_VARS
     SIMAT_MENU_SELECTION_IND_T ind;
}SIMAT_SIG_LOCAL_MENU_SELECTION_IND_T;
#endif


//the user activity event indication signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
}SIMAT_SIG_USER_ACTIVITY_IND_T;

//the idle screen available indication signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
}SIMAT_SIG_IDLE_SCREEN_AVA_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
}SIMAT_SIG_LOCAL_USER_ACTIVITY_IND_T;

//the idle screen available indication signal
typedef struct
{
    SIGNAL_VARS
}SIMAT_SIG_LOCAL_IDLE_SCREEN_AVA_IND_T;
#endif

//the language selection indication signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_LANG_SELECTION_IND_T lang_ind;
}SIMAT_SIG_LANG_SELECTION_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_LANG_SELECTION_IND_T lang_ind;
}SIMAT_SIG_LOCAL_LANG_SELECTION_IND_T;

//the indication of SMS control
typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_SMS_CONTROL_IND_T sms_control_ind; //the location infomation
}SIMAT_SIG_SMS_CONTROL_IND_T;
#endif

//-----------------------------------SMS interface signal-------------------------

//the SEND SMS proactive command service request signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SEND_SMS_CNF_T cnf;
}SIMAT_SIG_SEND_SMS_CNF_T;

//the SMS-PP data download request signal from the SMS layer
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SMS_PP_IND_T pp_ind;
}SIMAT_SIG_SMS_PP_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_SMS_PP_IND_T pp_ind;
}SIMAT_SIG_LOCAL_SMS_PP_IND_T;
#endif

//the SMS-CB data download indication signal from the SMS layer
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SMS_CB_IND_T cb_ind;
}SIMAT_SIG_SMS_CB_IND_T;

#ifdef PRODUCT_DM
//the SMS-CB data download indication signal from the SMS layer
typedef struct
{
    SIGNAL_VARS
    SIMAT_SMS_CB_IND_T cb_ind;
}SIMAT_SIG_LOCAL_SMS_CB_IND_T;
#endif

//the MO Short message controled by SIM request signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SMS_CONTROL_IND_T sms_ind;
}SIMAT_SIG_MO_SMS_CONTROL_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_SMS_CONTROL_IND_T sms_ind;
}SIMAT_SIG_LOCAL_MO_SMS_CONTROL_IND_T;
#endif

//------------------------------------------SS interface signal-------------------------------

//the SEND SS proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SEND_SS_CNF_T cnf;
}SIMAT_SIG_SEND_SS_CNF_T;

//the SEND USSD proactive command service confrim signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    kal_uint16          sim_sw;
#endif
    SIMAT_SEND_USSD_CNF_T cnf;
}SIMAT_SIG_SEND_USSD_CNF_T;

//the MO SS control by SIM request signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SS_CONTROL_IND_T ss_ind;
}SIMAT_SIG_MO_SS_CONTROL_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_SS_CONTROL_IND_T ss_ind;
}SIMAT_SIG_LOCAL_MO_SS_CONTROL_IND_T;
#endif

//the MO USSD control by SIM request signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_USSD_CONTROL_IND_T ussd_ind;
}SIMAT_SIG_MO_USSD_CONTROL_IND_T;


#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_USSD_CONTROL_IND_T ussd_ind;
}SIMAT_SIG_LOCAL_MO_USSD_CONTROL_IND_T;
#endif

//-----------------------------------CC interface signal--------------------------------
//the SET UP CALL proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SETUP_CALL_CNF_T cnf;
}SIMAT_SIG_SETUP_CALL_CNF_T;

//the SEND DTMF proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SEND_DTMF_CNF_T cnf;
}SIMAT_SIG_SEND_DTMF_CNF_T;

//the call contorl by SIM request signal
typedef struct
{
#ifndef PRODUCT_DM
	SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_CC_IND_T cc_ind;
}SIMAT_SIG_MO_CC_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_CC_IND_T cc_ind;
}SIMAT_SIG_LOCAL_MO_CC_IND_T;
#endif

//thye call control by SIM confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_CC_RSP_T cc_rsp;
}SIMAT_SIG_MO_CC_RSP_T;

//the MT call event indication signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_MT_CALL_IND_T ind;
}SIMAT_SIG_MT_CALL_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_MT_CALL_IND_T ind;
}SIMAT_SIG_LOCAL_MT_CALL_IND_T;
#endif

//the call connected event indication signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_CALL_CON_IND_T ind;
}SIMAT_SIG_CALL_CON_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_CALL_CON_IND_T ind;
}SIMAT_SIG_LOCAL_CALL_CON_IND_T;
#endif

//the call disconnected event indication signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_CALL_DISCON_IND_T ind;
}SIMAT_SIG_CALL_DISCON_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_CALL_DISCON_IND_T ind;
}SIMAT_SIG_LOCAL_CALL_DISCON_IND_T;
#endif


//---------------------------------------RR or MM  interface signal--------------------------
//the PROVIDE LOCATION INFORMATION(get local informaiton) proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_LAI_CNF_T cnf;
}SIMAT_SIG_GET_LAI_CNF_T;

//the location status event indication signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_LOCATION_STATUS_IND_T ind;
}SIMAT_SIG_LOCATION_STATUS_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    SIGNAL_VARS
    SIMAT_LOCATION_STATUS_IND_T ind;
}SIMAT_SIG_LOCAL_LOCATION_STATUS_IND_T;
#endif

//the PROVIDE LOCATION INFORATION(Get NMR and BCCH list)proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_NMR_BA_CNF_T cnf;
}SIMAT_SIG_GET_NMR_BA_CNF_T;

//the PROVIDE LOCATION INFORATION(Get TIMING ADVANCE)proactive command service request signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_TIMING_AD_CNF_T cnf;
}SIMAT_SIG_GET_TIMING_AD_CNF_T;

//the REFRESH proactive command service confirm signal
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_REFRESH_CNF_T cnf;
}SIMAT_SIG_REFRESH_CNF_T;


typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_LAUNCH_BROWSER_CNF_T cnf;
}SIMAT_SIG_LAUNCH_BROWSER_CNF_T;

typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_SEND_DATA_CNF_T cnf;
}SIMAT_SIG_SEND_DATA_CNF_T;
typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_RECEIVE_DATA_CNF_T cnf;
}SIMAT_SIG_RECEIVE_DATA_CNF_T;


typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_OPEN_CHANNEL_CNF_T cnf;
}SIMAT_SIG_OPEN_CHANNEL_CNF_T;

typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_CLOSE_CHANNEL_CNF_T cnf;
}SIMAT_SIG_CLOSE_CHANNEL_CNF_T;

typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_GET_CHANNEL_STATUS_CNF_T cnf;
}SIMAT_SIG_CHANNEL_STATUS_CNF_T;

typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_DATA_AVAILABLE_IND_T ind;
}SIMAT_SIG_DATA_AVAILABLE_IND_T;

typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_CHANNEL_STATUS_IND_T ind;
}SIMAT_SIG_CHANNEL_STATUS_IND_T;

typedef struct
{
#ifndef PRODUCT_DM
    SIGNAL_VARS
#else
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
#endif
    SIMAT_BROWSER_TERMINATION_IND_T ind;
}SIMAT_SIG_BROWSER_TERMINATION_IND_T;

#ifdef PRODUCT_DM
typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_GET_ACCESS_TECH_IND_T ind;
}SIMAT_SIG_GET_ACCESS_TECH_IND_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_RETRIEVE_MULTIMEDIA_MSG_CNF_T cnf;
}SIMAT_SIG_RETRIEVE_MULTIMEDIA_MSG_CNF_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_SUBMIT_MULTIMEDIA_MSG_CNF_T cnf;
}SIMAT_SIG_SUBMIT_MULTIMEDIA_MSG_CNF_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_DISPLAY_MULTIMEDIA_MSG_CNF_T cnf;
}SIMAT_SIG_DISPLAY_MULTIMEDIA_MSG_CNF_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_DATA_AVAILABLE_IND_T ind;
}SIMAT_SIG_LOCAL_DATA_AVAILABLE_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_CHANNEL_STATUS_IND_T ind;
}SIMAT_SIG_LOCAL_CHANNEL_STATUS_IND_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_NET_SEARCHER_MODE_IND_T ind;
}SIMAT_SIG_NET_SEARCHER_MODE_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_SIG_NET_SEARCHER_MODE_IND_T ind;
}SIMAT_SIG_LOCAL_NET_SEARCHER_MODE_IND_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_BROWSER_STATUS_IND_T ind;
}SIMAT_SIG_BROWSER_STATUS_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_BROWSER_STATUS_IND_T ind;
}SIMAT_SIG_LOCAL_BROWSER_STATUS_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_BROWSER_TERMINATION_IND_T ind;
}SIMAT_SIG_LOCAL_BROWSER_TERMINATION_IND_T;



typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_CARD_READ_STATUS_IND_T  ind;
}SIMAT_SIG_CARD_READER_STATUS_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_CARD_READ_STATUS_IND_T ind;
}SIMAT_SIG_LOCAL_CARD_READER_STATUS_IND_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_LOCAL_CONNECTION_IND_T  ind;
}SIMAT_SIG_LOCAL_CONNECTION_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_LOCAL_CONNECTION_IND_T ind;
}SIMAT_SIG_LOCAL_LOCAL_CONNECTION_IND_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_MMS_TRANS_STATUS_IND_T  ind;
}SIMAT_SIG_MMS_TRANS_STATUS_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_MMS_TRANS_STATUS_IND_T ind;
}SIMAT_SIG_LOCAL_MMS_TRANS_STATUS_IND_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_MMS_NOTI_DOWNLOAD_IND_T  ind;
}SIMAT_SIG_MMS_NOTI_DOWNLOAD_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_MMS_NOTI_DOWNLOAD_IND_T ind;
}SIMAT_SIG_LOCAL_MMS_NOTI_DOWNLOAD_IND_T;

//the location status event indication signal
typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_ACCESS_TECHNOLOGY_IND_T ind;
}SIMAT_SIG_ACCESS_TECH_CHANGE_IND_T;


typedef struct
{
    SIGNAL_VARS
    SIMAT_ACCESS_TECHNOLOGY_IND_T ind;
}SIMAT_SIG_LOCAL_ACCESS_TECH_CHANGE_IND_T;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_ACCESS_CNF_T cnf;
}SIMAT_SIG_GET_ACCESS_CNF_T;

typedef struct
{
    SIGNAL_VARS
    msg_type msg_id;
}SIMAT_SIG_LOCAL_TIMER_EXPIRE_IND_T;

typedef enum
{
    SIMAT_TIMER_1 = 0x01,
    SIMAT_TIMER_2,
    SIMAT_TIMER_3,
    SIMAT_TIMER_4,
    SIMAT_TIMER_5,
    SIMAT_TIMER_6,
    SIMAT_TIMER_7,
    SIMAT_TIMER_8
}SIMAT_TIMER_ID_E;

typedef struct
{
    kal_uint8           ref_count;
    l4_common_hdr_struct header;
    SIMAT_TIMER_ID_E timer_id;
}SIMAT_SIG_TIMER_EXPIRE_IND_T;

#endif

//the SMS-PP data download request signal from the SMS layer
typedef struct
{
    SIGNAL_VARS
    SIMAT_AT_CMD_ENVELOPE_IND_T envelope_ind;
}SIMAT_SIG_AT_CMD_ENVELOPE_IND_T;

//the SMS-PP data download request signal from the SMS layer
typedef struct
{
    SIGNAL_VARS
    SIMAT_AT_CMD_TERMINAL_RES_T resp;
}SIMAT_SIG_AT_CMD_TERMINAL_RES_T;

#ifdef __cplusplus
    }
#endif
#endif  //_SIMAT_SIGNAL_H_
