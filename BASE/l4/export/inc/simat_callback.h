/******************************************************************************
 ** File Name:      simat_callback.h                                          *
 ** Author:         Raistlin.kong                                             *
 ** Date:           01/12/2003                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    you can define your own message structure that to be used *
 **				    in your owner implemetation of the SIM application toolkit*
 **                 and also used in the call back function of SIMAT.         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2003.1        Raistlin.kong    Create                                     *
 ******************************************************************************/
 


#ifndef _SIMAT_CALLBACK_H_
#define _SIMAT_CALLBACK_H_

#ifdef __cplusplus
	extern "C"
	{
#endif

#define SIMAT_MAX_APDU_DATA_LEN 256

typedef  struct SIMAT_RAW_APDU_T 
{
    uint16 data_len;    //the data length
    uint8   data_arr[SIMAT_MAX_APDU_DATA_LEN]; //the data
}SIMAT_RAW_APDU_T;


//the DISPLAY TEXT proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_DISPLAY_TEXT_T com;   //the  proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
    SIMAT_TEXT_ATTRIBUTE_T text_attribute; 
}APP_MN_SIMAT_DISPLAY_TEXT_IND_T;

//the GET INKEY proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_GET_INKEY_T com;  //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
    SIMAT_TEXT_ATTRIBUTE_T text_attribute; 
}APP_MN_SIMAT_GET_INKEY_IND_T;

//the GET INPUT proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_GET_INPUT_T com;  //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
    SIMAT_TEXT_ATTRIBUTE_T text_attribute; 
}APP_MN_SIMAT_GET_INPUT_IND_T;

//the MORE TIME proactive command service request signal
typedef struct
{
    SIGNAL_VARS
	//SIMAT_MORE_TIME_T com;  //the proactive command
	MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_MORE_TIME_IND_T;


//the PLAY TONE proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_PLAY_TONE_T com;  //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
    SIMAT_DO_ICON_ID_T icon_id; //cr117196
    BOOLEAN is_icon_exist;      //cr117196
    SIMAT_TEXT_ATTRIBUTE_T text_attribute; 
}APP_MN_SIMAT_PLAY_TONE_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_REFRESH_T com;
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_REFRESH_IND_T;

//the SETUP MENU proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SETUP_MENU_T com; //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
    SIMAT_TEXT_ATTRIBUTE_T text_attribute; 
}APP_MN_SIMAT_SETUP_MENU_IND_T;

//the SELECT ITEM proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SELECT_ITEM_T com;  //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
    SIMAT_TEXT_ATTRIBUTE_T text_attribute;  //text attribute info
}APP_MN_SIMAT_SELECT_ITEM_IND_T;

//the SEND SMS proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SEND_SM_T    com;    //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_SEND_SMS_IND_T;

//the SEND SS proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SEND_SS_T com;    //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_SEND_SS_IND_T;

//the SEND USSD proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SEND_USSD_T   com;    //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_SEND_USSD_IND_T;

//the SET UP CALL proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SETUP_CALL_T  com;    //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_SETUP_CALL_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_LOCAL_INFO_TYPE_E com;
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T;

//the SET UP EVENT LIST proactive service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SETUP_EVENT_LIST_T com;
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}SIMAT_SIG_SETUP_EVENT_LIST_REQ_T;


//the SET IDLE MODE TEXT proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SETUP_IDLE_TEXT_T com;  //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
    SIMAT_TEXT_ATTRIBUTE_T text_attribute;  //text attribute info
}APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T;

//the SEND DTMF proactive command service request signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SEND_DTMF_T   com;    //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_SEND_DTMF_IND_T;

//the LANGUAGE NOTIFY proactive command service indicator signal(the proactive do not need return value)
typedef struct
{
    SIGNAL_VARS
    SIMAT_LANG_NOTIFICATION_T com;    //the proactive command
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_LANG_NOTIFY_IND_T;

//thye call control by SIM confirm signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_CC_RSP_T  com;
    MN_DUAL_SYS_E dual_sys;
   SIMAT_ENVELOPE_RESULT_TO_MMI_E env_result;     //contains the envelop result
}APP_MN_SIMAT_CC_TO_MMI_IND_T;

//the MO SS control by SIM confirm signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SS_CONTROL_RSP_T ss_rsp;
    MN_DUAL_SYS_E dual_sys;
}SIMAT_SIG_MO_SS_CONTROL_RSP_T;

//the MO USSD control by SIM confirm signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_USSD_CONTROL_RSP_T ussd_rsp;
    MN_DUAL_SYS_E dual_sys;
}SIMAT_SIG_MO_USSD_CONTROL_RSP_T;


//the MO Short message controled by SIM confirm signal
typedef struct
{
    SIGNAL_VARS
    SIMAT_SMS_CTL_TO_MMI_IND_T com;
    MN_DUAL_SYS_E dual_sys;
}APP_MN_SIMAT_SMS_CTL_TO_MMI_IND_T;


//the SMS-PP data download confirm signal from the SMS layer
typedef struct
{
    SIGNAL_VARS
    SIMAT_SMS_PP_RSP_T pp_rsp;
    MN_DUAL_SYS_E dual_sys;
}SIMAT_SIG_SMS_PP_RSP_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_SMS_CB_RES is_dl_success;  //is the CB sms download success or not
    MN_DUAL_SYS_E dual_sys;
}SIMAT_SIG_SMS_CB_RSP_T;

//the refresh  confirm signal
typedef struct
{
    SIGNAL_VARS
//#ifdef PRODUCT_DM
//    SIMAT_REFRESH_RESULT_T refresh_result;
//#else
	SIMAT_REFRESH_RESULT_T	result;
//#endif
	MN_DUAL_SYS_E	dual_sys;
}APP_MN_SIMAT_REFRESH_CNF_T;
typedef struct
{
	SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
}APP_MN_SIMAT_SESSION_TERMINATED_IND_T;

/* +cr110036 , 2.5g(dual sim)/3g integration */
/* Bin.Li add for usat */
typedef struct
{
    SIGNAL_VARS
    SIMAT_SEND_DATA_T  send_data;
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_SEND_DATA_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_RECEIVE_DATA_T  receive_data;
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_RECEIVE_DATA_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_LAUNCH_BROWSER_T  com;
	MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_LAUNCH_BROWSER_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_OPEN_CHANNEL_RELATED_GPRS_T  open_channel;
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_OPEN_CHANNEL_IND_T;
typedef struct
{
    SIGNAL_VARS
    SIMAT_CLOSE_CHANNEL_T  close_channel;
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_CLOSE_CHANNEL_IND_T;
typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    SIMAT_RAW_APDU_T raw_apdu;
}APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_DISPLAY_MULTINEDIA_MESSAGE_T  display_multimedia_msg;
    SIMAT_RAW_APDU_T raw_apdu;
    MN_DUAL_SYS_E dual_sys;
}APP_MN_SIMAT_DISPLAY_MULTIMEDIA_MSG_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_RETRIVE_MULTINEDIA_MESSAGE_T retrieve_multimedia_msg;
	SIMAT_RAW_APDU_T raw_apdu;
	MN_DUAL_SYS_E dual_sys;
}APP_MN_SIMAT_RETRIEVE_MULTIMEDIA_MSG_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_SUBMIT_MULTINEDIA_MESSAGE_T  submit_multimedia_msg;
    SIMAT_RAW_APDU_T raw_apdu;
    MN_DUAL_SYS_E dual_sys;
}APP_MN_SIMAT_SUBMIT_MULTIMEDIA_MSG_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_FWD_PC_T com; //the proactive command
    MN_DUAL_SYS_E dual_sys;
}APP_MN_SIMAT_FWD_PC_IND_T; 

/* -cr110036 */

typedef struct
{
    SIGNAL_VARS
    SIMAT_CC_RSP_T  cc_rsp;
}APP_MN_SIMAT_CC_MSG_RSP_T;
#if 0
typedef struct
{
    SIGNAL_VARS
    SIMAT_REFRESH_RESULT_T refresh_result;
}APP_MN_SIMAT_REFRESH_CNF_T;
#endif
/* +cr119425 newms */
typedef struct
{
    SIGNAL_VARS
    SIMAT_STATUS_WORD_T  status_word;
}APP_MN_SIMAT_TERMINAL_RESPONSE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    SIMAT_STATUS_WORD_T  status_word;
}APP_MN_SIMAT_ENVELOP_CNF_T;
/* -cr119425 newms */
#ifdef __cplusplus
	}
#endif
#endif
