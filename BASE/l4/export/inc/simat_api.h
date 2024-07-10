/******************************************************************************
 ** File Name:      simat_api.h                                               *
 ** Author:         Raistlin.kong                                             *
 ** Date:           02/18/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file that define the signals related   *
 **				    with short message                                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raistlin.kong    Create									  *
 ** 2003.1         Karin.Li         Update                                    *
 ** 2007.10        Bin.Li           add usat                                  *
 ** 2008.3         Pizer.Fan        2.5g (dual sim) and 3g integration        *
 ******************************************************************************/

#ifndef _SIMAT_API_H_
#define _SIMAT_API_H_
#include "simat_data_object.h"
#include "simat_proactive.h"
#include "state_machine.h"   //need by SIGNAL_ACTION_E

#ifdef __cplusplus
	extern "C"
	{
#endif


#define SIMAT_MAX_INPUT_LEN     200
/* 2.5g(dual sim)/3g integration */
#define SIMAT_BROWSER_STATE_LEN    20
#define SIMAT_FRAME_INFO_DATA_LEN    40
#define SIMAT_MULTIMEDIA_MSG_NOTIFY_DATA_LEN    60

#define SIMAT_MAX_APDU_DATA_LEN  256

typedef uint8 SIMAT_OPER_SOURCE_T;
#define SIMAT_OPER_NULL 0           //operation is from mmi user,not from stk
#define SIMAT_OPER_SETUP_CALL 1     //operation is from stk setup call
#define SIMAT_OPER_SEND_SMS 2       //operation is from stk send sms
#define SIMAT_OPER_SEND_SS 3        //operation is from stk send ss
#define SIMAT_OPER_SEND_USSD 4      //operation is from stk send ussd

//the confirm structure of the time advance from the sim application toolkit
//implementation layer
typedef struct
{
    SIMAT_DO_TIMING_AD_T    timing_ad;  //the timing advance
    SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_TIMING_AD_CNF_T;
/* 2.5g(dual sim)/3g integration */
typedef struct
{
    SIMAT_ACCESS_TECHNOLOGY_TYPE_E    access_type;  //the timing advance
    SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_ACCESS_CNF_T;

//the confirm structure of the network mesurement result
typedef struct
{
    SIMAT_DO_NMR_T  nmr;
    SIMAT_DO_BCCH_LIST_T    bcch_list;
    SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_NMR_BA_CNF_T;

//the confirm structure of the location information
typedef struct
{
	SIMAT_DO_LOCATION_INFO_T lai;   //the local information
    SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_LAI_CNF_T;

//the indication structure of the location status
typedef struct
{
	BOOLEAN is_local_info;      //is the location information data object exist in the event
    MN_PHONE_CURRENT_PLMN_INFO_T local_info;
}SIMAT_LOCATION_STATUS_IND_T;

//the indication structure of the call disconectted
typedef struct
{
    BOOLEAN is_cause;   //is the cause data object exist in the event
    BOOLEAN is_near_end_disconnection;//is near end disconnection or not
    SIMAT_DO_TI_LIST_T transaction_id;
    SIMAT_DO_CAUSE_T cause;
}SIMAT_CALL_DISCON_IND_T;

//the indication structure of the call connect
typedef struct
{
    BOOLEAN is_near_end_connect;//is connecting at the near end(an MT call)or not
	SIMAT_DO_TI_LIST_T ti_list;
}SIMAT_CALL_CON_IND_T;

//the indication structure of the MT call
typedef struct
{
    BOOLEAN is_address;         //is the addresss data object exist in the event
    BOOLEAN is_cps;             //is the cps data object exist in the event
    SIMAT_DO_TI_LIST_T transaction_id;
    MN_CALLED_NUMBER_T address;
    MN_SUBADDR_T  cps;
}SIMAT_MT_CALL_IND_T;

//the confirmation structure of send DTMF
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_SEND_DTMF_CNF_T;

typedef enum
{
    SIMAT_SIMC_ADDRESS,     //the address
    SIMAT_SIMC_SS_STRING,   //the ss string
    SIMAT_SIMC_USSD_STRING  //the ussd string
}SIMAT_SIMC_DATA_TYPE_E;

/* 2.5g(dual sim)/3g integration */
#ifndef PRODUCT_DM
typedef union
#else
typedef struct
#endif
{
    MN_CALLED_NUMBER_T  add;
    SIMAT_DO_SS_STRING_T       ss;
    MN_SS_USSD_DATA_T     ussd;
}SIMAT_SIMC_DATA_U;

//if the call control change the SETUP CALL,SEND SS,SEND USSD into another
//active,this structure is to save the information of the active call control make.
typedef struct
{
	BOOLEAN is_simc_data;   //is the SIM control data exist
    BOOLEAN is_ccp1;        //is the capability configuration parameter 1 exist
    BOOLEAN is_ccp2;        //is the capability configuration parameter 2 exist,
                            //ccp1 present is the premise of the present of the ccp2
    BOOLEAN is_cps;         //is the called patry subaddress
    BOOLEAN is_alpha_id;    //is the alpha identifier exist
/* 2.5g(dual sim)/3g integration */
    uint8 no_add_data;             //the addition data length,only valide when the SEND SS command succeed
    uint8* add_data;                //the pointer to the addtion data
    SIMAT_SIMC_DATA_TYPE_E data_type;   //the type of the SIMC data
    SIMAT_SIMC_DATA_U simc_data;    //the data of the SIM control
    SIMAT_DO_CCP_T  ccp1;   //the capability configuration parameter 1
    SIMAT_DO_CCP_T  ccp2;   //the capability configuration parameter 2
    SIMAT_DO_CPS_T  cps;    //the called party subaddress
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha identifier
    SIMAT_DO_BCR_INDICATOR_T    bc_repeat;  //he BC repeat indicator,only present when BOTH the ccp1 and ccp2 exist
    //the below is the result informaiton of the acitve call control
    SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
    SIMAT_NE_PROBLEM_E  ne_info;    //the Additional information for Nework problem.it's needed when the general
                                    //result is "Network currently unalbe to process command"
    SIMAT_SS_PROBLEM_E  ss_info;    //the additional information for the SS problem.it's needed when the general
                                    //result is "SS return error"
    SIMAT_USSD_PROBLEM_E    ussd_info;  //the additional information for the USSD problem.it's needed when the general
                                        //result is "USSD return error"
    MN_SS_USSD_DATA_T ussd_return; //the USSD return result,only valid when the SEND USSD commad succeed
}SIMAT_CC_OPERATE_RESULT_T;

//the confirmation structure of setup call
typedef struct
{
    BOOLEAN is_command_changed;     //is the command has been changed into another command(SNED SS,SEND USSD)
	SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
    /* +CR111385 */
    uint8  ne_info;    //the Additional information for Nework problem.it's needed when the general
                                    //result is "Network currently unalbe to process command"
    uint8  ss_info;    //the additional information for the SS problem.it's needed when the general
                                    //result is "SS return error"
    /* -CR111385 */
    SIMAT_CC_OPERATE_RESULT_T cc_result;    //the cc result valide only when the set up call comand has be change into
                                            //another command by the call contorl
}SIMAT_SETUP_CALL_CNF_T;

//the confirmation of send USSD
typedef struct
{
    BOOLEAN is_command_changed;     //is the command has been changed into another command(SET UP CALL,SEND SS)
	SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
    uint8  ne_info;    //the Additional information for Nework problem.it's needed when the general
                                    //result is "Network currently unalbe to process command"
    uint8    ussd_info;  //the additional information for the USSD problem.it's needed when the general
                                        //result is "USSD return error"
    MN_SS_USSD_DATA_T ussd_return; //the USSD return result,only valid when the SEND USSD commad succeed
    SIMAT_CC_OPERATE_RESULT_T cc_result;    //the cc result valide only when the set up call comand has be change into
                                            //another command by the call contorl
}SIMAT_SEND_USSD_CNF_T;

//the confirmation of send SS
typedef struct
{
    //uint8   buf_id;  //the share buf number asociated with for this service
    BOOLEAN is_command_changed;     //is the command has been changed into another command(SET UP CALL,SEND USSD)
/* 2.5g(dual sim)/3g integration */
    uint8  no_add_data;             //the addition data length,only valide when the SEND SS command succeed
    uint8* add_data;                //the pointer to the addtion data
	SIMAT_GENERAL_PC_RESULT_E  result; //general result
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
    uint8  ne_info;    //the Additional information for Nework problem.it's needed when the general
                                    //result is "Network currently unalbe to process command"
    uint8  ss_info;    //the additional information for the SS problem.it's needed when the general
                                    //result is "SS return error"
    SIMAT_CC_OPERATE_RESULT_T cc_result;    //the cc result valide only when the set up call comand has be change into
                                            //another command by the call contorl
}SIMAT_SEND_SS_CNF_T;

//the confirmation of send SMS
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
    uint8  ne_info;    //the Additional information for Nework problem.it's needed when the general
                                    //result is "Network currently unalbe to process command"
    uint8 sms_info;   //the additional information for SMS problem.it's needed when general result
                                    //is "SMS RP-ERROR"
    SIMAT_CALL_SMS_CONTROL_PROBLEM_E cc_info; //Additional information for interaction with call control or MO SM control.
                                      //it is needed when general result is "interaction with call control by SIM or MO short message control by SIM, permanent problem".
}SIMAT_SEND_SMS_CNF_T;

//the indication of language selection
typedef struct
{
	 SIMAT_DO_LANG_T lang;  //the language is a pair of alpha-numberic characters,defined in ISO639
}SIMAT_LANG_SELECTION_IND_T;

//the indication of menu selection
typedef struct
{
	BOOLEAN is_help_IND;    //is the menu seletion including the help information request
    uint8 menu_id;          //the identifier of the menu being selected
}SIMAT_MENU_SELECTION_IND_T;

//the confirm of setup idle text
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_SETUP_IDLE_TEXT_CNF_T;

//the confirm of get IMEI
typedef struct
{
	SIMAT_DO_IMEI_T imei;
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_GET_IMEI_CNF_T;

//the confirm of get language
typedef struct
{
	SIMAT_DO_LANG_T lang;  //the language is a pair of alpha-numberic characters,defined in ISO639
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_GET_LANG_CNF_T;

//the confirm of get date-time and time-zone
typedef struct
{
	SIMAT_DO_DT_TZ_T dt_tz;   //the data time and time zone information coded as GSM 04.08
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_GET_DT_TZ_CNF_T;

//the confirm of select item
typedef struct
{
	uint8   item_id;        //the identifier of the selected item
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_SELECT_ITEM_CNF_T;

//the confirm of setup menu
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_SETUP_MENU_CNF_T;

//the confirm of play tone
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_PLAY_TONE_CNF_T;

//the confirm of get input
typedef struct
{
	SIMAT_DO_TEXT_STRING_T text_str;       //the text string that the user input and
                                        //empacked as the command required
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_GET_INPUT_CNF_T;

//the confirm of get inkey
typedef struct
{
	SIMAT_DO_TEXT_STRING_T text_str;       //the text string that the user input and
                                        //empacked as the command required
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
    uint32  duration; //ipc required, no internal requeired now
}SIMAT_GET_INKEY_CNF_T;

//the confirm of display text
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_DISPLAY_TEXT_CNF_T;

//the confirm of setup event list
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_SETUP_EVENT_LIST_CNF_T;

//the confirmation of send SMS
typedef struct
{
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
}SIMAT_RETRIEVE_MULTIMEDIA_MSG_CNF_T;
typedef struct
{
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
}SIMAT_SUBMIT_MULTIMEDIA_MSG_CNF_T;
typedef struct
{
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
}SIMAT_DISPLAY_MULTIMEDIA_MSG_CNF_T;

//the confirm of refresh
typedef struct
{
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_REFRESH_CNF_T;

//the Call control and MO SMS control by SIM result
typedef enum
{
    SIMAT_SIMC_ALLOWED = 0x0,              //Allowed,no modification
    SIMAT_SIMC_NOT_ALLOWED,          //not allowed
    SIMAT_SIMC_ALLOWED_AND_MODIFIED, //allowed with modification
    SIMAT_SIMC_BUSY,        //the SIM application toolkit is busy
    SIMAT_SIMC_ERROR        //the SIM is error when processing the call control
}SIMAT_SIM_CONTROL_RESULT_E;



//the indication structure of call control
typedef struct
{
	BOOLEAN is_ccp1;    //is the Capability configuration parameter 1 exist
    BOOLEAN is_cps;     //is the called party subaddress exist
    BOOLEAN is_ccp2;    //is the capability configuration parameter 2 exist
    SIMAT_SIMC_DATA_TYPE_E data_type;  //the content is ADDRESS or SS or USSD
    MN_CALLED_NUMBER_T  add; //the call address
    MN_SS_USSD_DATA_T  ussd;   //the USSD string object
    SIMAT_DO_SS_STRING_T  ss;   //the SS string object
    SIMAT_DO_CCP_T  ccp1;   //the capability configuration parameter 1
    SIMAT_DO_CCP_T  ccp2;   //the capability configuration parameter 2
    MN_PHONE_CURRENT_PLMN_INFO_T   location_info;  //the location information
    MN_SUBADDR_T  cps;    //the called party subaddress
}SIMAT_CC_IND_T;

//the response structure of call control
typedef struct
{
	BOOLEAN is_simc_data;   //is the SIM control data exist
    BOOLEAN is_ccp1;        //is the capability configuration parameter 1 exist
    BOOLEAN is_ccp2;        //is the capability configuration parameter 2 exist,
                            //ccp1 present is the premise of the present of the ccp2
    BOOLEAN is_cps;         //is the called patry subaddress
/*  2.5g(dual sim)/3g integration */
    BOOLEAN is_alpha_id;    //is the alpha identifier exist

    SIMAT_SIM_CONTROL_RESULT_E result; //the result of the sim control
    SIMAT_SIMC_DATA_TYPE_E data_type;   //the type of the SIMC data
    SIMAT_SIMC_DATA_U simc_data;    //the data of the SIM control
    SIMAT_DO_CCP_T  ccp1;   //the capability configuration parameter 1
    SIMAT_DO_CCP_T  ccp2;   //the capability configuration parameter 2
    MN_SUBADDR_T  cps;    //the called party subaddress
/*  2.5g(dual sim)/3g integration */
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha identifier

    SIMAT_DO_BCR_INDICATOR_T    bc_repeat;  //he BC repeat indicator,only present when BOTH the ccp1 and ccp2 exist
}SIMAT_CC_RSP_T;

typedef struct
{
    SIMAT_CC_RSP_T          cc_rsp;      //mo cc cntrl rsp
    SIMAT_ENVELOPE_RESULT_TO_MMI_E env_result;     //contains the envelop result
}SIMAT_CC_CTL_TO_MMI_IND_T;

//the indication structure of USSD control
typedef struct
{
    uint8   buf_id;  //the share buf number asociated with for this service
	BOOLEAN is_ccp1;    //is the Capability configuration parameter 1 exist
    BOOLEAN is_cps;     //is the called party subaddress exist
    BOOLEAN is_ccp2;    //is the capability configuration parameter 2 exist
    MN_SS_USSD_DATA_T ussd; //the ussd string
    SIMAT_DO_CCP_T  ccp1;   //the capability configuration parameter 1
    SIMAT_DO_CCP_T  ccp2;   //the capability configuration parameter 2
    SIMAT_DO_LOCATION_INFO_T   location_info;  //the location information
    SIMAT_DO_CPS_T  cps;    //the called party subaddress
}SIMAT_USSD_CONTROL_IND_T;

//the response structure of USSD control
typedef struct
{
    uint8   buf_id;  //the share buf number asociated with for this service
	BOOLEAN is_simc_data;   //is the SIM control data exist
    BOOLEAN is_ccp1;        //is the capability configuration parameter 1 exist
    BOOLEAN is_ccp2;        //is the capability configuration parameter 2 exist,
                            //ccp1 present is the premise of the present of the ccp2
    BOOLEAN is_cps;         //is the called patry subaddress
    BOOLEAN is_alpha_id;    //is the alpha identifier exist
    SIMAT_SIM_CONTROL_RESULT_E result; //the result of the sim control
    SIMAT_SIMC_DATA_TYPE_E data_type;   //the type of the SIMC data
    SIMAT_SIMC_DATA_U simc_data;    //the data of the SIM control
    SIMAT_DO_CCP_T  ccp1;   //the capability configuration parameter 1
    SIMAT_DO_CCP_T  ccp2;   //the capability configuration parameter 2
    SIMAT_DO_CPS_T  cps;    //the called party subaddress
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha identifier
    SIMAT_DO_BCR_INDICATOR_T    bc_repeat;  //he BC repeat indicator,only present when BOTH the ccp1 and ccp2 exist
}SIMAT_USSD_CONTROL_RSP_T;

//the indication of SS control
typedef struct
{
    uint8   buf_id;  //the share buf number asociated with for this service
	BOOLEAN is_ccp1;    //is the Capability configuration parameter 1 exist
    BOOLEAN is_cps;     //is the called party subaddress exist
    BOOLEAN is_ccp2;    //is the capability configuration parameter 2 exist
    SIMAT_DO_SS_STRING_T    ss; //the ss string
    SIMAT_DO_CCP_T  ccp1;   //the capability configuration parameter 1
    SIMAT_DO_CCP_T  ccp2;   //the capability configuration parameter 2
    SIMAT_DO_LOCATION_INFO_T   location_info;  //the location information
    SIMAT_DO_CPS_T  cps;    //the called party subaddress
}SIMAT_SS_CONTROL_IND_T;

//the response of SS control
typedef struct
{
    uint8   buf_id;  //the share buf number asociated with for this service
	BOOLEAN is_simc_data;   //is the SIM control data exist
    BOOLEAN is_ccp1;        //is the capability configuration parameter 1 exist
    BOOLEAN is_ccp2;        //is the capability configuration parameter 2 exist,
                            //ccp1 present is the premise of the present of the ccp2
    BOOLEAN is_cps;         //is the called patry subaddress
    BOOLEAN is_alpha_id;    //is the alpha identifier exist
    SIMAT_SIM_CONTROL_RESULT_E result; //the result of the sim control
    SIMAT_SIMC_DATA_TYPE_E data_type;   //the type of the SIMC data
    SIMAT_SIMC_DATA_U simc_data;    //the data of the SIM control
    SIMAT_DO_CCP_T  ccp1;   //the capability configuration parameter 1
    SIMAT_DO_CCP_T  ccp2;   //the capability configuration parameter 2
    SIMAT_DO_CPS_T  cps;    //the called party subaddress
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha identifier
    SIMAT_DO_BCR_INDICATOR_T    bc_repeat;  //he BC repeat indicator,only present when BOTH the ccp1 and ccp2 exist
}SIMAT_SS_CONTROL_RSP_T;

//the indication of SMS control
typedef struct
{
	MN_CALLED_NUMBER_T  dest_sc;    //the RP_Destination_Address of the service center
    MN_CALLED_NUMBER_T  dest_me;    //the TP_Destination_Address of the ME is poposing to send the SM
    MN_PHONE_CURRENT_PLMN_INFO_T location_info; //the location infomation
}SIMAT_SMS_CONTROL_IND_T;


//the response of SMS control
typedef struct
{
	BOOLEAN is_dest_sc;     //is the destination Service Center address exist
    BOOLEAN is_dest_me;     //is the Destination ME address exist
    SIMAT_SIM_CONTROL_RESULT_E   result;     //the MO short message control result
    MN_CALLED_NUMBER_T  dest_sc;    //the destination service center
    MN_CALLED_NUMBER_T  dest_me;    //the destination ME address
}SIMAT_SMS_CONTROL_RSP_T;

typedef struct
{
	BOOLEAN is_address;     //is the address data object exist in the download request
    MN_CALLED_NUMBER_T  address;    //the address data object holds the RP originating address of the service centre
    MN_L3_MSG_UNIT_T     tpdu;   //the SMS tpdu
}SIMAT_SMS_PP_IND_T;

#define SIMAT_MAX_SIM_PP_ACK_LEN    128
typedef struct
{
    uint8   len;        //the acknowledgement length
    uint8   ack_arr[SIMAT_MAX_SIM_PP_ACK_LEN];  //the acknowledgement data
}SIMAT_SMS_PP_ACK_T;
/* +cr110036 , 2.5g(dual sim)/3g integration */
typedef struct
{
    BOOLEAN                 is_alpha_id;    //is sim provide alpha id or not
    SIMAT_DO_ALPHA_ID_T     alpha_id;       //text string from SIM card
    SIMAT_SMS_CONTROL_RSP_T cntrl_rsp;      //mo sms cntrl rsp
    SIMAT_ENVELOPE_RESULT_TO_MMI_E env_result;     //contains the envelop result
}SIMAT_SMS_CTL_TO_MMI_IND_T;

/* -cr110036 */
//the SMS-PP data download result
typedef enum
{
    SIMAT_SMSPP_OK,     //the SMS-PP data download success
    SIMAT_SMSPP_OK_WITH_ACK,    //the SMS-PP data download success with acknowledgement for the network
    SIMAT_SMSPP_BUSY,   //the Application toolkit busy
    SIMAT_SMSPP_ERROR,  //SIM data download error
    SIMAT_SMSPP_ERROR_WITH_MESSAGE  //sim data download error with a message for the network
}SIMAT_SMS_PP_RESULT_E;

typedef struct
{
	SIMAT_SMS_PP_ACK_T ack;         //the SIM acknowledgement,shoud be send back to the network
    SIMAT_SMS_PP_RESULT_E result;   //the SMS-PP data download result
}SIMAT_SMS_PP_RSP_T;

typedef struct
{
	SIMAT_DO_CB_PAGE_T cb_page;     //the cell broadcast page
}SIMAT_SMS_CB_IND_T;

//the result of the terminal profile operation
typedef enum
{
    SIMAT_PD_OK,
    SIMAT_PD_ERROR,
    SIMAT_PD_CARD_NOT_READY,
    SIMAT_PD_CARD_NOT_SUPPORT
}SIMAT_PROFILE_DOWNLOAD_RESULT_E;

typedef struct
{
    SIMAT_PROFILE_DOWNLOAD_RESULT_E result;   //the profile download result
}SIMAT_PROFILE_DOWNLOAD_CNF_T;

//the response signal struction of the sms data download ind
#define STK_MAX_SMS_PP_ACK_LEN    128
typedef struct
{
    uint8   len;        //the acknowledgement length
    uint8   ack_arr[STK_MAX_SMS_PP_ACK_LEN];  //the acknowledgement data
}STK_SMS_PP_ACK_T;

//the SMS-PP data download result
typedef enum
{
    STK_SMSPP_OK,     //the SMS-PP data download success
    STK_SMSPP_OK_WITH_ACK,    //the SMS-PP data download success with acknowledgement for the network
    STK_SMSPP_BUSY,   //the Application toolkit busy
    STK_SMSPP_ERROR,  //SIM data download error
    STK_SMSPP_ERROR_WITH_MESSAGE  //sim data download error with a message for the network
}STK_SMS_PP_RESULT_E;

typedef struct
{
    SIGNAL_VARS
	STK_SMS_PP_ACK_T ack;         //the SIM acknowledgement,shoud be send back to the network
    STK_SMS_PP_RESULT_E result;   //the SMS-PP data download result
}STK_SMS_PP_CNF_T;



#if 0 // already defined above
//the confirm of refresh
typedef struct
{
	SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_REFRESH_CNF_T;
#endif

typedef enum
{
    MN_SIMFILE_START = 0,
    MN_SIMFILE_ECC,
    MN_SIMFILE_ELP,
    MN_SIMFILE_LP,
    MN_SIMFILE_PHASE,
    MN_SIMFILE_AD,
    MN_SIMFILE_SST,
    MN_SIMFILE_IMSI,
    MN_SIMFILE_ACC,
    MN_SIMFILE_PLMNsel,
    MN_SIMFILE_PLMNWACT,
    MN_SIMFILE_OPLMNWACT,
    MN_SIMFILE_HPLMN,
    MN_SIMFILE_LOCI,
    MN_SIMFILE_KC,
    MN_SIMFILE_BCCH,
    MN_SIMFILE_FPLMN,
    MN_SIMFILE_SoLSA,
    MN_SIMFILE_CBMID,
    MN_SIMFILE_DCK,
    MN_SIMFILE_NIA,
    MN_SIMFILE_ICCID,
    MN_SIMFILE_SMSP,
    MN_SIMFILE_SMSS,
    MN_SIMFILE_SMS,//SMS should be read asap
    MN_SIMFILE_FDN,//FDN should be read asap
    MN_SIMFILE_CPHSINFO,
    MN_SIMFILE_CFFLAG,
    MN_SIMFILE_ONSTRING,
    MN_SIMFILE_ONSTHORT,
    MN_SIMFILE_DYN_FLAG,
    MN_SIMFILE_DYN_LINE2_FLAG,
    MN_SIMFILE_CSP_LINE2,
    MN_SIMFILE_ACM_LINE2,
    MN_SIMFILE_CSP,
    MN_SIMFILE_IMG,
    MN_SIMFILE_ACMmax,
    MN_SIMFILE_SPDI,    // Service Provider Display Information
    MN_SIMFILE_PNN,
    MN_SIMFILE_OPL,
    MN_SIMFILE_SPNI,
    MN_SIMFILE_PNNI,
    MN_SIMFILE_LND,
    MN_SIMFILE_MSISDN,
    MN_SIMFILE_ADN,
    MN_SIMFILE_SDN,
    MN_SIMFILE_EST,
    MN_SIMFILE_SPN,
    MN_SIMFILE_END // END of sim files to be readed
}MN_INI_SIMFILE_E;

typedef struct {

	BOOLEAN is_ok;				//whether success
	BOOLEAN is_file_changed;    //whether file changed
	BOOLEAN	flag_arr[MN_SIMFILE_END];//changed files table
}SIMAT_REFRESH_RESULT_T;

/* +cr110036 , 2.5g(dual sim)/3g integration */
/* bin.li add for usat */
typedef struct
{
    BOOLEAN is_sub_addess;
    BOOLEAN is_cap_config_para1;
    BOOLEAN is_cap_config_para2;
    BOOLEAN is_cb_repeat_indicator;
    MN_CALLED_NUMBER_T address;
    SIMAT_SUB_ADDRESS_T sub_address;
    SIMAT_LOCATION_STATUS_IND_T location_status;
    SIMAT_SETUP_CALL_T cap_config_para1;
    SIMAT_SETUP_CALL_T cap_config_para2;
    uint8 cb_repeat_indicator;
}SIMAT_CALL_CTL_BY_NAA_IND_T;

#define SIMAT_CARD_READER_IS_REMOVABLE           0x01
#define SIMAT_CARD_READER_IS_PRESENT                0x02
#define SIMAT_CARD_READER_PRESENT_IS_ID1        0x04
#define SIMAT_CARD_IS_PRESENT                              0x08
#define SIMAT_CARD_IS_POWERED                             0x10
typedef struct
{
    uint8 identity; /* 0-7 */
    uint8 status_mask;
}SIMAT_CARD_READ_STATUS_IND_T;

typedef struct
{
    SIMAT_BROWSER_TERMINATION_CAUSE_E  termination_cause;
}SIMAT_BROWSER_TERMINATION_IND_T;

typedef struct
{
    uint8 channel_id;                            /* 1-7 available */
    SIMAT_CHANNEL_MODE_INFO_E mode_info;
    BOOLEAN is_link_establish;
}SIMAT_CHANNEL_STATUS_T;

typedef struct
{
    uint8 data_len;
    SIMAT_CHANNEL_STATUS_T channel_status;
}SIMAT_DATA_AVAILABLE_IND_T;


typedef struct
{
    BOOLEAN is_other_address;
    BOOLEAN is_bear_desc;
    SIMAT_CHANNEL_STATUS_T  channel_status;
    SIMAT_OTHER_ADDRESS_T other_address;
    SIMAT_BEAR_DESC_T bear_desc;
    uint8 channel_id;
}SIMAT_CHANNEL_STATUS_IND_T;

typedef struct
{
    SIMAT_ACCESS_TECHNOLOGY_TYPE_E access_type;
}SIMAT_ACCESS_TECHNOLOGY_IND_T;

typedef struct
{
    uint8 display_para[3];
}SIMAT_DISPLAY_PARA_IND_T;

typedef struct
{
       BOOLEAN is_trans_level;
	BOOLEAN is_remote_address;
	BOOLEAN is_other_address;
       SIMAT_TRANS_LEVEL_T trans_level;
       SIMAT_SERVICE_RECORD_T service_record;
	SIMAT_REMOTE_ENTITY_ADDRESS_T remote_address;
	SIMAT_OTHER_ADDRESS_T other_address;
}SIMAT_LOCAL_CONNECTION_IND_T;

typedef struct
{
    SIMAT_NET_SEARCHER_MODE_E searcher_mode;
}SIMAT_NET_SEARCHER_MODE_IND_T;

typedef struct
{
    uint8 data_len;
    uint8 state_data[SIMAT_BROWSER_STATE_LEN];
}SIMAT_BROWSER_STATUS_IND_T;


typedef struct
{
    uint8 data_len;
    uint8 default_frame_id;
    uint8 frame_info_data[SIMAT_FRAME_INFO_DATA_LEN];
}SIMAT_FRAME_INFOMATION_CHANGED_IND_T;


typedef struct
{
    BOOLEAN is_trans_status;
    SIMAT_DO_FILE_LIST_T    file_list;  //the file list object,exist when the refresh type is
    SIMAT_MULTIMEDIA_MSG_IDENTITY_T multimedia_msgid;
    SIMAT_MULTIMEDIA_MSG_TRANS_STATUS_T trans_status;
}SIMAT_MMS_TRANS_STATUS_IND_T;


typedef struct
{
      BOOLEAN is_last_envelop;
      uint8 data_len;
      uint8 msg_notify_data[SIMAT_MULTIMEDIA_MSG_NOTIFY_DATA_LEN];
}SIMAT_MMS_NOTI_DOWNLOAD_IND_T;
/* -cr110036 */

#if 0 // already defined above
/* bin.li add for usat */
typedef struct
{
    BOOL is_sub_addess;
    BOOL is_cap_config_para1;
    BOOL is_cap_config_para2;
    BOOL is_cb_repeat_indicator;
    MN_CALLED_NUMBER_T address;
    SIMAT_SUB_ADDRESS_T sub_address;
    SIMAT_LOCATION_STATUS_IND_T location_status;
    SIMAT_SETUP_CALL_T cap_config_para1;
    SIMAT_SETUP_CALL_T cap_config_para2;
    uint8 cb_repeat_indicator;
}SIMAT_CALL_CTL_BY_NAA_IND_T;

#define SIMAT_CARD_READER_IS_REMOVABLE           0x01
#define SIMAT_CARD_READER_IS_PRESENT                0x02
#define SIMAT_CARD_READER_PRESENT_IS_ID1        0x04
#define SIMAT_CARD_IS_PRESENT                              0x08
#define SIMAT_CARD_IS_POWERED                             0x10
typedef struct
{
    uint8 identity; /* 0-7 */
    uint8 status_mask;
}SIMAT_CARD_READ_STATUS_IND_T;

typedef struct
{
    uint8 channel_id;                            /* 1-7 available */
    SIMAT_CHANNEL_MODE_INFO_E mode_info;
    BOOL is_link_establish;
}SIMAT_CHANNEL_STATUS_T;

typedef struct
{
    uint8 data_len;
    SIMAT_CHANNEL_STATUS_T channel_status;
}SIMAT_DATA_AVAILABLE_IND_T;


typedef struct
{
    BOOL is_other_address;
    BOOL is_bear_desc;
    SIMAT_CHANNEL_STATUS_T  channel_status;
    SIMAT_OTHER_ADDRESS_T other_address;
    SIMAT_BEAR_DESC_T bear_desc;
    uint8 channel_id;
}SIMAT_CHANNEL_STATUS_IND_T;

typedef struct
{
    SIMAT_ACCESS_TECHNOLOGY_TYPE_E access_type;
}SIMAT_ACCESS_TECHNOLOGY_IND_T;

typedef struct
{
    uint8 display_para[3];
}SIMAT_DISPLAY_PARA_IND_T;

typedef struct
{
       BOOL is_trans_level;
	BOOL is_remote_address;
	BOOL is_other_address;
       SIMAT_TRANS_LEVEL_T trans_level;
       SIMAT_SERVICE_RECORD_T service_record;
	SIMAT_REMOTE_ENTITY_ADDRESS_T remote_address;
	SIMAT_OTHER_ADDRESS_T other_address;
}SIMAT_LOCAL_CONNECTION_IND_T;

typedef struct
{
    SIMAT_NET_SEARCHER_MODE_E searcher_mode;
}SIMAT_NET_SEARCHER_MODE_IND_T;

typedef struct
{
    uint8 data_len;
    uint8 state_data[SIMAT_BROWSER_STATE_LEN];
}SIMAT_BROWSER_STATUS_IND_T;


typedef struct
{
    uint8 data_len;
    uint8 default_frame_id;
    uint8 frame_info_data[SIMAT_FRAME_INFO_DATA_LEN];
}SIMAT_FRAME_INFOMATION_CHANGED_IND_T;


typedef struct
{
    BOOL is_trans_status;
    SIMAT_DO_FILE_LIST_T    file_list;  //the file list object,exist when the refresh type is
    SIMAT_MULTIMEDIA_MSG_IDENTITY_T multimedia_msgid;
    SIMAT_MULTIMEDIA_MSG_TRANS_STATUS_T trans_status;
}SIMAT_MMS_TRANS_STATUS_IND_T;


typedef struct
{
      BOOLEAN is_last_envelop;
      uint8 data_len;
      uint8 msg_notify_data[SIMAT_MULTIMEDIA_MSG_NOTIFY_DATA_LEN];
}SIMAT_MMS_NOTI_DOWNLOAD_IND_T;
#endif


typedef BOOLEAN SIMAT_SMS_CB_RES;

typedef struct
{
     SIMAT_ACCESS_TECHNOLOGY_TYPE_E access_tech_type;
}SIMAT_GET_ACCESS_TECH_IND_T;

typedef struct
{
     SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
     SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
     uint8 channel_data_len;
}SIMAT_SEND_DATA_CNF_T;

typedef struct
{
     SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
     SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
     SIMAT_CHANNEL_DATA_T channel_data;
     uint8 channel_data_len;
}SIMAT_RECEIVE_DATA_CNF_T;

typedef struct
{
     SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
     SIMAT_LAUNCH_BROWSER_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
}SIMAT_LAUNCH_BROWSER_CNF_T;

typedef struct
{
     BOOLEAN is_bear_desc;
     BOOLEAN is_buf_size;
     BOOLEAN is_other_address;
     BOOLEAN is_channel_status;
     uint8 channel_id;
     SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
     SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the genernal result is
                                    //"ME currently can not process command"
     SIMAT_BEAR_DESC_T bear_desc;
     uint16 buf_size;
     SIMAT_OTHER_ADDRESS_T other_address;
     SIMAT_CHANNEL_STATUS_T channel_status;
}SIMAT_OPEN_CHANNEL_CNF_T;

typedef struct
{
     uint8 channel_id;
     SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
     SIMAT_BEARER_INDEPENDENT_PROBLEM_E  mp_info;    //the ME problem add information,used when the general result is
                                    //"ME currently can not process command"
}SIMAT_CLOSE_CHANNEL_CNF_T;

typedef struct
{
    SIMAT_GENERAL_PC_RESULT_E result;   //the general result of the proactive command
    SIMAT_ME_PROBLEM_E  mp_info;    //the ME problem add information,used when the general result is
                                    //"ME currently can not process command"
    SIMAT_CHANNEL_STATUS_T channel_status;
    uint8 channel_id;
}SIMAT_GET_CHANNEL_STATUS_CNF_T;

typedef struct
{
	uint8   apdu_len;
	uint8   apdu[SIMAT_MAX_APDU_DATA_LEN];
}SIMAT_AT_CMD_ENVELOPE_IND_T,
SIMAT_AT_CMD_TERMINAL_RES_T;

//---------------------------call back function---------------------------------
/**********************************************************************
//    Description:
//      the sim application toolkit use this call back function to communicate
//    with the sim application toolkit MMI implementation layer.the communicating
//    item including the below:poacitve command request,data download result,call
//    control(including USSD,SS,SMS control) result and so on.
//    Global resource dependence :
//    Author:Raistlin.kong
//    Note:You can rewrite this function to make the communicating way the
//    seem to your own way.
***********************************************************************/
void SIMAT_CommunicateCallBack(
                                uint32 taskid,
                                uint32 msg_type,
                                void *argv
                                );

//This call back function used in simat_main.c
/**********************************************************************
//    Description:
//      the sim application toolkit use this call back function to communicate
//    with the sim application toolkit MMI implementation layer.the communicating
//    item including the below:poacitve command request,data download result,call
//    control(including USSD,SS,SMS control) result and so on.
//    Global resource dependence :
//    Author:Raistlin.kong
//    Note:You can rewrite this function to make the communicating way the
//    seem to your own way.
***********************************************************************/
void SIMAT_CallBack(
                   uint32 taskid,
                   uint32 msg_type,
                   void *argv
                   );

//-----------------------------share memory fucntion-----------------------------

/**********************************************************************
//    Description:
//      Close a Share memory for laterly use
//    Global resource dependence :NO
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMAT_CloseShareBufEx(
                                uint8 card_id,
                                uint8 buf_id   //the buf id in the share mem
                                );

//----------------------------------------------API functions--------------------
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    time advance as the confirm result of the provide location information
//    proactive command
//    Global resource dependence :
//    Author:Raistlin.kong
//    Note:
***********************************************************************/
void SIMAT_CnfTimeAdvance(
                                SIMAT_TIMING_AD_CNF_T* pta  //in:the timing advance cnf
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    network measurement result and ba list as the confirm result of the
//    provide location information
//    proactive command
//    Global resource dependence :
//    Author:Raistlin.kong
//    Note:
***********************************************************************/
void SIMAT_CnfNMRBA(
                                SIMAT_NMR_BA_CNF_T* pnmr    //in:the nmr and ba confirm
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    network measurement result and ba list as the confirm result of the
//    provide location information
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfAccess
(
    SIMAT_ACCESS_CNF_T* access_cnf    //in:the nmr and ba confirm
);

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    Location Information as the confirm result of the provide location
//    information
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfLAI(
                                SIMAT_LAI_CNF_T* plai    //in:the lai confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    Location Status as the indication result of the provide location
//    Status
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndLocationStatus(
                                SIMAT_LOCATION_STATUS_IND_T* plocsta    //in:the location status indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    call disconnected event as the indication result of the provide
//    call disconnected event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndCallDiscon(
                                SIMAT_CALL_DISCON_IND_T* pcaldis    //in:the call disconnected event indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    call connected event as the indication result of the provide
//    call connected event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndCallCon(
                                SIMAT_CALL_CON_IND_T* pcalcon    //in:the call connected event indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MT call event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMTCall(
                                SIMAT_MT_CALL_IND_T* pmtcal    //in:the MT call event indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MO call control as the indication result of the provide MO call
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMOCC(
                                SIMAT_CC_IND_T* pmocc    //in:the MO call control indication
                                );
/**********************************************************************
//    Description:
//      MMI call this API function to send the MO call control as the 
//    indication result of the provide MO call control indication
//    proactive command
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
void SIMAT_IndMMIMOCC(
                SIMAT_CC_IND_T* pmocc    //in:the MO call control indication
                );
  
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    DTMF as the confirm result of the provide DTMF confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendDtmf(
                                SIMAT_SEND_DTMF_CNF_T* pdtmf    //in:the DTMF confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup call as the confirm result of the provide setup call confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetupCall(
                                SIMAT_SETUP_CALL_CNF_T* psetcall    //in:the setup call confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup call as the confirm result of the provide setup call confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfRetrieveMultiMediaMsg(
                                SIMAT_RETRIEVE_MULTIMEDIA_MSG_CNF_T* retrieve_multimedia_msg_ptr    //in:the setup call confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup call as the confirm result of the provide setup call confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSubmitMultiMediaMsg(
                                SIMAT_SUBMIT_MULTIMEDIA_MSG_CNF_T* submit_multimedia_msg_ptr    //in:the setup call confirm
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup call as the confirm result of the provide setup call confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfDisplayMultiMediaMsg(
                                SIMAT_DISPLAY_MULTIMEDIA_MSG_CNF_T* display_multimedia_msg_ptr    //in:the setup call confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    USSD control as the indication result of the provide USSD
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndUSSDControl(
                                SIMAT_USSD_CONTROL_IND_T* pussdc    //in:the USSD control indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SS control as the indication result of the provide SS
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSSControl(
                                SIMAT_SS_CONTROL_IND_T* pssc    //in:the SS control indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    Location Status as the indication result of the provide location
//    Status
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndAccessTechChange
(
    SIMAT_ACCESS_TECHNOLOGY_IND_T* access_tech_change_ptr    //in:the location status indication
);

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    send USSD as the confirm result of the provide send USSD confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendUssd(
                                SIMAT_SEND_USSD_CNF_T* psussd    //in:the send ussd confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    send SS as the confirm result of the provide send SS confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendSs(
                                SIMAT_SEND_SS_CNF_T* psss    //in:the send ss confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SMS control as the indication result of the provide SMS
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSMSControl(
                                SIMAT_SMS_CONTROL_IND_T* psmsc    //in:the SMS control indication
                                );

/**********************************************************************
//    Description:
//      MMI call this API function to send the SMS control as the 
//    indication result of the provide SMS control indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMMISMSControl(
                            SIMAT_SMS_CONTROL_IND_T* psmsc    //in:the SMS control indication
                            );
  
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SMS cell broadcast as the indication result of the provide SMS
//    cell broadcast indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSMSCB(
                                SIMAT_SMS_CB_IND_T* psmscb    //in:the SMS cell broadcast indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SMS PP as the indication result of the provide SMS
//    PP indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSMSPP(
                                SIMAT_SMS_PP_IND_T* psmspp   //in:the SMS PP indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SMS PP as the indication result of the provide SMS
//    PP indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndNormalSmsPP(
                    SIMAT_SMS_PP_IND_T* psmspp   //in:the SMS PP indication
                    );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    send SMS as the confirm result of the provide send SMS confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendSms(
                                SIMAT_SEND_SMS_CNF_T* pssms    //in:the send sms confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    language selection as the indication result of the provide language
//    selection indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndLangSel(
                                SIMAT_LANG_SELECTION_IND_T* plangsel   //in:the language selection indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    idle screen available as the indication result of the provide
//    idle screen available indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndIdleScrAva( void );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    user activity event as the indication result of the provide
//    user activity event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndUserAct( void );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    menu selection as the indication result of the provide menu
//    selection indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMenuSel(
                                SIMAT_MENU_SELECTION_IND_T* pmenusel   //in:the menu selection indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup idle text as the confirm result of the provide setup idle text
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetIdleText(
                                SIMAT_SETUP_IDLE_TEXT_CNF_T* psit    //in:the setup idle text confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get IMEI as the confirm result of the provide get IMEI
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetImei(
                                SIMAT_GET_IMEI_CNF_T* pgimei    //in:the get IMEI confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get language as the confirm result of the provide get language
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetLang(
                                SIMAT_GET_LANG_CNF_T* pglang    //in:the get language confirm
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    refresh as the confirm result of the provide get language
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfRefresh(
                                SIMAT_REFRESH_CNF_T* prefresh   //in:the refresh confirm
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get date-time and time-zone as the confirm result of the provide get
//    date-time and time-zone confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetDttz(
                                SIMAT_GET_DT_TZ_CNF_T* pgdttz    //in:the get date-time and time-zone confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    select item as the confirm result of the provide select item
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSelItem(
                                SIMAT_SELECT_ITEM_CNF_T* psitem    //in:the select item confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup menu as the confirm result of the provide setup menu
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetMenu(
                                SIMAT_SETUP_MENU_CNF_T* psmenu    //in:the setup menu confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    play tone as the confirm result of the provide play tone
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfPlayTone(
                                SIMAT_PLAY_TONE_CNF_T* pptone    //in:the play tone confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get input as the confirm result of the provide get input
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetInput(
                                SIMAT_GET_INPUT_CNF_T* pginput   //in:the get input confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get inkey as the confirm result of the provide get inkey
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetInkey(
                                SIMAT_GET_INKEY_CNF_T* pginkey   //in:the get inkey confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    display text as the confirm result of the provide display text
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfDisplayText(
                                SIMAT_DISPLAY_TEXT_CNF_T* pdtext   //in:the diaplay text confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup event list as the confirm result of the provide setup event
//    list confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetEventList(
                                SIMAT_SETUP_EVENT_LIST_CNF_T* pselist   //in:the setup event list confirm
                                );

/**********************************************************************
//    Description:
//      set the task id of the STK-MMI
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetMMITaskID(
                                uint32  task_id //the STK-MMI task id
                                );

/**********************************************************************
//    Description:
//       call this fucntion to redo the terminal profile
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_TerminalProfile(
                                uint8* pprofile,    //in:the pointer point to the profile data buf
                                uint16 data_len     //the length of the validate profile data
                                );

/**********************************************************************
//    Description:
//      judge whether need pass the call information to the SIM card when
//    a MO occurs.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern BOOLEAN SIMAT_IsNeedCallControl(void);

/**********************************************************************
//    Description:
//      judge whether need pass the short message information to the SIM card when
//    a MO short message occurs.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern BOOLEAN SIMAT_IsNeedSMSControl(void);

/**********************************************************************
//    Description:
//      Pack the 8 bit sms alphabet data into 7 bit sms alphabet
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint16 SIMAT_PackSmsAlphabet(   //return value:the length of the output data
                                uint8*  pbuf,    //in:the input data buffer
                                uint16  data_len,   //the length of the data input
                                uint8*  po_buf, //out:the output buf
                                uint16  max_len //the max legnth of the output buf
                                );

/**********************************************************************
//    Description:
//      Unpack the 7 bit sms alphabet data into 8 bit sms alphabet
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint16 SIMAT_UnpackSmsAlphabet( //return value:the length of the output data
                                uint8*  pbuf,    //in:the input data buffer
                                uint16  data_len,   //the length of the data input
                                uint8*  po_buf, //out:the output buf
                                uint16  max_len //the max legnth of the output buf
                                );



/**********************************************************************
//    Description:
//      Initialize the sim application toolkit
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern void InitSIMATEx(uint8 card_id);

/*****************************************************************************/
// 	Description : entry point of mnstk module
//	Global resource dependence :
//  Author:	Raislin.kong
//	Note:
/*****************************************************************************/

extern SIGNAL_ACTION_E STK_Module(
         MN_DUAL_SYS_E card_id,
         xSignalHeaderRec *psig   // input signal value
         );

/* +cr110036 , 2.5g(dual sim)/3g integration */
/**********************************************************************
//    Description:
//      judge whether need pass the call information to the SIM card when
//    a MO occurs.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern BOOLEAN SIMAT_IsNeedCallControl(void);
/* -cr110036 */

/**********************************************************************
//    Description:
//      The function to collect the SIM Application Toolkit facilities
//    that are supported by the ME and make up the TEMINAL PROFILE data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern void SIMAT_GetMeProfile(
                                uint8* profile,     //out:the Teminal profile data
                                uint16* profile_len  //in/out:when in,the length of the buf that to save the teminal profile
                                                    //out,the really terminal profile data length
                                );

//----------------------------------------------API functions--------------------
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    time advance as the confirm result of the provide location information
//    proactive command
//    Global resource dependence :
//    Author:Raistlin.kong
//    Note:
***********************************************************************/
void SIMAT_CnfTimeAdvanceEx(
                                uint8 card_id,
                                SIMAT_TIMING_AD_CNF_T* pta  //in:the timing advance cnf
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    network measurement result and ba list as the confirm result of the
//    provide location information
//    proactive command
//    Global resource dependence :
//    Author:Raistlin.kong
//    Note:
***********************************************************************/
void SIMAT_CnfNMRBAEx(
								uint8 card_id,
                                SIMAT_NMR_BA_CNF_T* pnmr    //in:the nmr and ba confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    Location Information as the confirm result of the provide location
//    information
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfLAIEx(
                                uint8 card_id,
                                SIMAT_LAI_CNF_T* plai    //in:the lai confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    Location Status as the indication result of the provide location
//    Status
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndLocationStatusEx(
                                uint8 card_id,
                                SIMAT_LOCATION_STATUS_IND_T* plocsta    //in:the location status indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    call disconnected event as the indication result of the provide
//    call disconnected event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndCallDisconEx(
                                uint8 card_id,
                                SIMAT_CALL_DISCON_IND_T* pcaldis    //in:the call disconnected event indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    call connected event as the indication result of the provide
//    call connected event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndCallConEx(
                                uint8 card_id,
                                SIMAT_CALL_CON_IND_T* pcalcon    //in:the call connected event indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MT call event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMTCallEx(
                                uint8 card_id,
                                SIMAT_MT_CALL_IND_T* pmtcal    //in:the MT call event indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MO call control as the indication result of the provide MO call
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMOCCEx(
                                uint8 card_id,
                                SIMAT_CC_IND_T* pmocc    //in:the MO call control indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    DTMF as the confirm result of the provide DTMF confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendDtmfEx(
                                uint8 card_id,
                                SIMAT_SEND_DTMF_CNF_T* pdtmf    //in:the DTMF confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup call as the confirm result of the provide setup call confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetupCallEx(
                                uint8 card_id,
                                SIMAT_SETUP_CALL_CNF_T* psetcall    //in:the setup call confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    USSD control as the indication result of the provide USSD
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndUSSDControlEx(
                                uint8 card_id,
                                SIMAT_USSD_CONTROL_IND_T* pussdc    //in:the USSD control indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SS control as the indication result of the provide SS
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSSControlEx(
                                uint8 card_id,
                                SIMAT_SS_CONTROL_IND_T* pssc    //in:the SS control indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    send USSD as the confirm result of the provide send USSD confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendUssdEx(
                                uint8 card_id,
                                SIMAT_SEND_USSD_CNF_T* psussd    //in:the send ussd confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    send SS as the confirm result of the provide send SS confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendSsEx(
                                uint8 card_id,
                                SIMAT_SEND_SS_CNF_T* psss    //in:the send ss confirm
                                );

/**********************************************************************
//    Description:
//      judge whether need pass the call information to the SIM card when
//    a MO occurs.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMAT_IsNeedCallControlEx(uint8 card_id);

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SMS control as the indication result of the provide SMS
//    control indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSMSControlEx(
                                uint8 card_id,
                                SIMAT_SMS_CONTROL_IND_T* psmsc    //in:the SMS control indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SMS cell broadcast as the indication result of the provide SMS
//    cell broadcast indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSMSCBEx(
                                uint8 card_id,
                                SIMAT_SMS_CB_IND_T* psmscb    //in:the SMS cell broadcast indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    SMS PP as the indication result of the provide SMS
//    PP indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndSMSPPEx(
                                uint8 card_id,
                                SIMAT_SMS_PP_IND_T* psmspp   //in:the SMS PP indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    send SMS as the confirm result of the provide send SMS confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendSmsEx(
                                uint8 card_id,
                                SIMAT_SEND_SMS_CNF_T* pssms    //in:the send sms confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    language selection as the indication result of the provide language
//    selection indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndLangSelEx(
                                uint8 card_id,
                                SIMAT_LANG_SELECTION_IND_T* plangsel   //in:the language selection indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    idle screen available as the indication result of the provide
//    idle screen available indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndIdleScrAvaEx(uint8 card_id);

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    user activity event as the indication result of the provide
//    user activity event indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndUserActEx(uint8 card_id);

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    menu selection as the indication result of the provide menu
//    selection indication
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMenuSelEx(
                                uint8 card_id,
                                SIMAT_MENU_SELECTION_IND_T* pmenusel   //in:the menu selection indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup idle text as the confirm result of the provide setup idle text
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetIdleTextEx(
                                uint8 card_id,
                                SIMAT_SETUP_IDLE_TEXT_CNF_T* psit    //in:the setup idle text confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get IMEI as the confirm result of the provide get IMEI
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetImeiEx(
                                uint8 card_id,
                                SIMAT_GET_IMEI_CNF_T* pgimei    //in:the get IMEI confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get language as the confirm result of the provide get language
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetLangEx(
                                uint8 card_id,
                                SIMAT_GET_LANG_CNF_T* pglang    //in:the get language confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get date-time and time-zone as the confirm result of the provide get
//    date-time and time-zone confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetDttzEx(
                                uint8 card_id,
                                SIMAT_GET_DT_TZ_CNF_T* pgdttz    //in:the get date-time and time-zone confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    select item as the confirm result of the provide select item
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSelItemEx(
                                uint8 card_id,
                                SIMAT_SELECT_ITEM_CNF_T* psitem    //in:the select item confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup menu as the confirm result of the provide setup menu
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetMenuEx(
                                uint8 card_id,
                                SIMAT_SETUP_MENU_CNF_T* psmenu    //in:the setup menu confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    play tone as the confirm result of the provide play tone
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfPlayToneEx(
                                uint8 card_id,
                                SIMAT_PLAY_TONE_CNF_T* pptone    //in:the play tone confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get input as the confirm result of the provide get input
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetInputEx(
                                uint8 card_id,
                                SIMAT_GET_INPUT_CNF_T* pginput   //in:the get input confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get inkey as the confirm result of the provide get inkey
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetInkeyEx(
                                uint8 card_id,
                                SIMAT_GET_INKEY_CNF_T* pginkey   //in:the get inkey confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    display text as the confirm result of the provide display text
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfDisplayTextEx(
                                uint8 card_id,
                                SIMAT_DISPLAY_TEXT_CNF_T* pdtext   //in:the diaplay text confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    setup event list as the confirm result of the provide setup event
//    list confirm
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetEventListEx(
                                uint8 card_id,
                                SIMAT_SETUP_EVENT_LIST_CNF_T* pselist   //in:the setup event list confirm
                                );

/**********************************************************************
//    Description:
//      set the task id of the STK-MMI
//    proactive command
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSetMMITaskIDEx(
                                uint8 card_id,
                                uint32  task_id //the STK-MMI task id
                                );

/**********************************************************************
//    Description:
//      judge whether need pass the call information to the SIM card when
//    a MO occurs.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern BOOLEAN SIMAT_IsNeedCallControlEx( uint8 card_id );

/**********************************************************************
//    Description:
//      judge whether need pass the short message information to the SIM card when
//    a MO short message occurs.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern BOOLEAN SIMAT_IsNeedSMSControlEx(uint8 card_id);
/*****************************************************************************/
// 	Description : send stk refresh request to the sim
//	Global resource dependence : none
//  Author:       yongxia
//	Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_RefreshEx(
		 uint8 card_index,
		 SIMAT_REFRESH_T refresh // the refresh type and the refresh file
		 );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    refresh result as the confirm result of refresh command
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMAT_CnfRefreshEx(
								uint8 card_id,
                                SIMAT_REFRESH_CNF_T* prefresh   //in:the refresh confirm
                                );
                                
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    AT envelope request to the sim 
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
void SIMAT_IndATCmdEnvelopeEx(
                                uint8 card_id,
                                SIMAT_AT_CMD_ENVELOPE_IND_T *penv //in:the Envelope data
                                );
                                
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    AT terminal request to the sim 
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
void SIMAT_TerminalResponseEx(
                                uint8 card_id,
                                SIMAT_AT_CMD_TERMINAL_RES_T *prsp //in:the terminal response data
                                );                                
                               
/* +cr110036 , 2.5g(dual sim)/3g integration */
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get access technology as the confirm result of the provide get access technology
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
void SIMAT_GetAccessTechInd(
                                SIMAT_GET_ACCESS_TECH_IND_T* access_tech_ptr    //in:the get IMEI confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MT call event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndBrowserTermination(
                                SIMAT_BROWSER_TERMINATION_IND_T* browser_termination_ptr    //in:the MT call event indication
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    browser status event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndMmsTransferStatus(
                                SIMAT_MMS_TRANS_STATUS_IND_T* mms_trans_status_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    browser status event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndCardReaderStatusEvent(
                                SIMAT_CARD_READ_STATUS_IND_T* card_reader_status_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    browser status event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndBrowserStatusEvent(
                                SIMAT_BROWSER_STATUS_IND_T* browser_status_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MT channel status event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndChannelStatusEvent(
                                SIMAT_CHANNEL_STATUS_IND_T* channel_status_ptr
                                );
 /**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MT call event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndDataAvailable(
                                SIMAT_DATA_AVAILABLE_IND_T* data_availabe_ptr    //in:the MT call event indication
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    result of the GetChannelStatus confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note: Added Dual SIM support by Eddie.Wang on 12/07/2010
***********************************************************************/
void SIMAT_CnfGetChannelStatusEx(
                                uint8 card_id,
                                SIMAT_GET_CHANNEL_STATUS_CNF_T* get_channel_status_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    result of the CloseChannel confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note: Added Dual SIM support by Eddie.Wang on 12/07/2010
***********************************************************************/
void SIMAT_CnfCloseChannelEx(
                                uint8 card_id,
                                SIMAT_CLOSE_CHANNEL_CNF_T* close_channel_ptr
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    result of the OpenChannel confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note: Added Dual SIM support by Eddie.Wang on 12/07/2010
***********************************************************************/
void SIMAT_CnfOpenChannelEx(
                                uint8 card_id,
                                SIMAT_OPEN_CHANNEL_CNF_T* open_channel_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    result of the ReceiveData confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note: Added Dual SIM support by Eddie.Wang on 12/07/2010
***********************************************************************/
void SIMAT_CnfReceiveDataEx(
                                uint8 card_id,
                                SIMAT_RECEIVE_DATA_CNF_T* receive_data_ptr    //in:the get IMEI confirm
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    result of the  SendData confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note: Added Dual SIM support by Eddie.Wang on 12/07/2010
***********************************************************************/
void SIMAT_CnfSendDataEx(
                                uint8 card_id,
                                SIMAT_SEND_DATA_CNF_T* send_data_ptr    //in:the get IMEI confirm
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    result of the launch browser
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMAT_CnfLaunchBrowserEx
							(	uint8 card_id,
								SIMAT_LAUNCH_BROWSER_CNF_T* launch_browser_ptr
							);

/**********************************************************************
//    Description:
//       call this fucntion to redo the terminal profile download
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_TerminalProfileEx(
                             uint8 card_id,
                             uint8* pprofile,    //in:the pointer point to the profile data buf
                             uint16 data_len     //the length of the validate profile data
                             );
                             
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to decode the stk file list data object(for at modem)
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMAT_DecodeFileListEx(
                                uint8 data_len,                    //the data len of the file list data object
                                uint8 *file_arr,                   //the input file list data object,begin from the tag
                                SIMAT_DO_FILE_LIST_T *file_list
                                );  

/* -cr110036 */
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get access tech as the confirm result of the provide get access tech
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfGetChannelStatus(
                                SIMAT_GET_CHANNEL_STATUS_CNF_T* get_channel_status_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get access tech as the confirm result of the provide get access tech
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfCloseChannel(
                                SIMAT_CLOSE_CHANNEL_CNF_T* close_channel_ptr
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get access tech as the confirm result of the provide get access tech
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfOpenChannel
(
    SIMAT_OPEN_CHANNEL_CNF_T* open_channel_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get access tech as the confirm result of the provide get access tech
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfLaunchBrowser
(
                                SIMAT_LAUNCH_BROWSER_CNF_T* lauch_browser_ptr
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get access tech as the confirm result of the provide get access tech
//    confirm
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfReceiveData(
                                SIMAT_RECEIVE_DATA_CNF_T* receive_data_ptr    //in:the get IMEI confirm
                                );
/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    get access tech as the confirm result of the provide get access tech
//    confirm
//    proactive command
//    Global resourcSIMAT_CnfSendDatae dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_CnfSendData(
                                SIMAT_SEND_DATA_CNF_T* send_data_ptr    //in:the get IMEI confirm
                                );

/**********************************************************************
//    Description:
//      the mmi  implementation layer call this API function to refresh sim
//    proactive command
//    Global none  dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_Refresh(
		 SIMAT_REFRESH_T refresh // the refresh type and the refresh file
		 );

/* +CR142255 */
/**********************************************************************
//    Description:
//      judge whether need pass the short message information to the SIM card when
//    a MO short message occurs.
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
BOOLEAN SIMAT_IsNeedSMSDownload(void);
/* -CR142255 */

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the
//    MT call event as the indication result of the provide MT call
//    event indication
//    proactive command
//    Global resource dependence :
//    Author:Bin.Li
//    Note:
***********************************************************************/
void SIMAT_IndNwSrchModeChange(
                                SIMAT_NET_SEARCHER_MODE_IND_T* nw_search_mode
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the data available event proactive command
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMAT_IndDataAvailableEx(
                                uint8 card_id,
                                SIMAT_DATA_AVAILABLE_IND_T* data_availabe_ptr    //in:
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send the channel status event
//proactive command
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMAT_IndChannelStatusEx(
                                uint8 card_id,
                                SIMAT_CHANNEL_STATUS_IND_T* channel_status_ptr    //in:
                                );

/**********************************************************************
//    Description:
//      the sim implementation layer call this API function to send
//    browser termination event download the the sim card
//    event indication
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
void SIMAT_IndBrowserTerminationEx(
                                 uint8 card_id,
                                 SIMAT_BROWSER_TERMINATION_IND_T* browser_termination_ptr    //in:the browser termination event indication
                                );

/*****************************************************************************/
// 	Description : trig event EV_MN_APP_SIMAT_EVENT_REFRESH_CNF
//	Global resource dependence : none
//  Author:       Yongxia.Zhang
//	Note:
/*****************************************************************************/
void SIMAT_SendEventRefreshCnfEx(
         MN_DUAL_SYS_E  dual_sys,
         BOOLEAN        is_success,
         BOOLEAN        is_file_changed,
         BOOLEAN	    *file_flag_arr
         );

/*****************************************************************************/
// 	Description : set the operation source of setup call/send sms/send ss/ussd
//	Global resource dependence : none
//  Author:       Yongxia.Zhang
//	Note:
/*****************************************************************************/
extern void SIMAT_SetOperSourceEx(
         MN_DUAL_SYS_E  dual_sys,
         SIMAT_OPER_SOURCE_T       oper_source
         );
         
/*****************************************************************************/
// 	Description : get the operation source of setup call/send sms/send ss/ussd
//	Global resource dependence : none
//  Author:       Yongxia.Zhang
//	Note:
/*****************************************************************************/
extern SIMAT_OPER_SOURCE_T SIMAT_GetOperSourceEx(
         MN_DUAL_SYS_E  dual_sys
         );

/*****************************************************************************/
// 	Description : get ccp params
//	Global resource dependence : none
//  Author:       xiaonian.li
//	Note:
/*****************************************************************************/
extern void SIMAT_GetCcpParamsEx(
         MN_DUAL_SYS_E  dual_sys,
         SIMAT_DO_CCP_T *ccp_ptr
         );

/*****************************************************************************/
//  Description : Dphone refresh sim request
//  Global resource dependence : none
//  Author:Jacky.ying
//  Note:
/*****************************************************************************/
void SIMAT_DphoneRefreshEx( 
              MN_DUAL_SYS_E   dual_sys,
              SIMAT_REFRESH_TYPE_E refresh_type
              );

/*****************************************************************************/
//  Description : DM_BASE structure adaptive in two structure.
//  Global resource dependence : none
//  Author:wuding.yang
//  Note:
/*****************************************************************************/
void SIMAT_TranslateRefreshStructure(
            MN_DUAL_SYS_E  dual_sys,
            SIMAT_REFRESH_FILE_LIST_T *in_ptr,
            SIMAT_DO_FILE_LIST_T *out_ptr);

/*****************************************************************************/
//  Description : Is STK active?
//  Global resource dependence : none
//  Author:wuding.yang
//  Note:
/*****************************************************************************/
BOOLEAN SIMAT_IsSTKStatusActive(MN_DUAL_SYS_E  dual_sys);
void SIMAT_ActiveSTKStatus(MN_DUAL_SYS_E  dual_sys);

//this include must be end of this header file
#include "simat_api_td.h" 

#ifdef __cplusplus
	}
#endif

#endif	//_SIMAT_API_H_
