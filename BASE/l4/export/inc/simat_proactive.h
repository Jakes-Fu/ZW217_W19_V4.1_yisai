/******************************************************************************
 ** File Name:      simat_proactive.h                                         *
 ** Author:         Raistlin.kong                                             *
 ** Date:           07/19/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file have the SIMAT proactive SIM      *
 **                 command relative data and macro define		              *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2001.9        Raislin.Kong    Create									  *
 ** 2007.9        Bin.Li          modify for usat						      *
 ** 2008.3        Pizer.Fan       2.5g (dual sim) and 3g integration		  *
 ******************************************************************************/
 

#ifndef _SIMAT_PROACTIVE_H_
#define _SIMAT_PROACTIVE_H_
#ifdef __cplusplus
    extern "C"
    {
#endif

#ifndef PRODUCT_DM
#include "dal_audio.h"
#endif

//get the tag value from a uint8 data byte
#define GET_TAG_VALUE(uint8_byte)           ((uint8_byte) & 0x7f)
#define IS_COMPREHESION(uint8_byte)         ((BOOLEAN)(((uint8_byte) & 0x80) == 0x80))
#define MAKE_COMPREHESION(uint8_byte)       ((uint8_byte) |= 0x80)



//the additional information for the ME problem
typedef enum
{
    SIMAT_MP_NO_SPECIFIC_CAUSE = 0x0,   //no specific cause can be given
    SIMAT_MP_SCREEN_BUSY,               //screen is busy
    SIMAT_MP_BUSY_ON_CALL,              //ME currently busy on call
    SIMAT_MP_BUSY_ON_SS,                //ME currently busy on SS transaction
    SIMAT_MP_NO_SERVICE,                //no service
    SIMAT_MP_ACC_BAR,                   //access control class bar
    SIMAT_MP_RR_NOT_GRANTED,            //radio resource no granted
    SIMAT_MP_NOT_IN_SPEECH_CALL,        //not in speech call
    SIMAT_MP_BUSY_ON_USSD,              //ME currently busy on USSD transaction
    SIMAT_MP_BUSY_ON_DTMF               //ME currently busy on SEND DTMF command
}SIMAT_ME_PROBLEM_E;

//addtional information for the network problem
typedef enum
{
    SIMAT_NP_NO_SPECIFIC_CAUSE = 0x0    //no specifi cause can be given
}SIMAT_NE_PROBLEM_E;

//additional information for ss problem
typedef enum
{
    SIMAT_SSP_NO_SPECIFIC_CAUSE = 0x0   //no specific cause can be given
}SIMAT_SS_PROBLEM_E;

//additional information for SMS problem
typedef enum
{
    SIMAT_SMSP_NO_SPECIFIC_CAUSE = 0x0  //no specific cause can be given
}SIMAT_SMS_PROBLEM_E;

//additional information for USSD problem
typedef enum
{
    SIMAT_USSDP_NO_SPECIFIC_CAUSE = 0x0 //no specific cause can be given
}SIMAT_USSD_PROBLEM_E;

typedef enum
{
    SIMAT_CS_CONTROL_NO_SPECIFIC_CAUSE = 0x0,//no specific cause can be given
    SIMAT_CS_CONTROL_ACTION_NOT_ALLOWED_CAUSE,
    SIMAT_CS_CONTROL_TYPE_CHANGED_CAUSE
}SIMAT_CALL_SMS_CONTROL_PROBLEM_E;

typedef enum
{
    SIMAT_LB_NO_SPECIFIC_CAUSE = 0x0,//no specific cause can be given
    SIMAT_LB_BEAR_UNAVAILABLE_CAUSE,
    SIMAT_LB_BROWSER_UNAVAILABLE_CAUSE,
    SIMAT_LB_ME_UNABLE_READ_CAUSE
}SIMAT_LAUNCH_BROWSER_PROBLEM_E;

typedef enum
{
    SIMAT_BI_NO_SPECIFIC_CAUSE = 0x0,//no specific cause can be given
    SIMAT_BI_CHANNEL_UNAVAILABLE_CAUSE,
    SIMAT_BI_CHANNEL_CLOSED_CAUSE,
    SIMAT_BI_CHANNEL_ID_INVALID_CAUSE,
    SIMAT_BI_BUFFER_SIZE_INVALID_CAUSE,
    SIMAT_BI_SECURITY_ERROR_CAUSE,
    SIMAT_BI_INTERFACE_LEVEL_INVALID_CAUSE
}SIMAT_BEARER_INDEPENDENT_PROBLEM_E;

//the structure of the DISPLAY TEXT proactive command
typedef struct
{
    BOOLEAN high_priority;  //the display command high proority or not
    BOOLEAN user_clear;     //wait for the user to clear the message or clear the message
                            //after a while
    BOOLEAN is_icon_exist;  //does the ICON id exist
    BOOLEAN is_ir;          //is this command a immediate response
    SIMAT_DO_TEXT_STRING_T  text_str;   //the text string
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identifier
}SIMAT_DISPLAY_TEXT_T;

//the get inkey type
typedef enum
{
    SIMAT_DIGIT_ONLY,   //the inkey is digit(0-9,*,#)
    SIMAT_YES_NO,       //the inkey is yes(0x1)/no(0x0)
    SIMAT_SMS_ALPHABET, //the inkey is a SMS alphabet
    SIMAT_UCS2_ALPHABET //the inkey is ucs2 alphabet
}SIMAT_INKEY_TYPE_E;

//the structure of the GET INKEY proactive command
typedef struct
{
    BOOLEAN help_info;      //TRUE:help information available,FALSE help information not avialable
    BOOLEAN is_icon_id;  //does the ICON id exist
    SIMAT_INKEY_TYPE_E  inkey_type; //the getinky type
    SIMAT_DO_TEXT_STRING_T  text_str;   //the text string
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identifier
}SIMAT_GET_INKEY_T;

typedef enum
{
    SIMAT_INPUT_DIGIT_ONLY,     //the input is be digit(0-9,*,#)
    SIMAT_INPUT_SMS_ALPHABET,  //the input is unpacked sms alphabet
    SIMAT_INPUT_UCS2_ALPHABET,  //the input is ucs2 alphabet
    SIMAT_INPUT_DUAL_DIGIT      //the input is dual byte digit
}SIMAT_INPUT_TYPE_E;

//the structure of the GET INPUT proactive command
typedef struct
{
    BOOLEAN is_packed;  //the input is packed sms or unpacked sms,
    BOOLEAN input_echo;     //TRUE:ME echo the user input on the display
                            //FALSE:user input should not be revealed in any way(by show with'*' or other)
    BOOLEAN help_info;      //TRUE:help information available,FALSE help information not avialable
    BOOLEAN is_default_text;    //is the deafult text string supplyed by the SIM
    BOOLEAN is_icon_id;  //does the ICON id exist
    SIMAT_INPUT_TYPE_E input_type;  //the input type
    SIMAT_DO_TEXT_STRING_T  text_str;   //the text string
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identifier
    SIMAT_DO_RESPONSE_LEN_T response_len;   //the user input length scope
    SIMAT_DO_DEFAULT_TEXT_T   default_text;   //the string for the ME to display,as default text string offered by the SIM
}SIMAT_GET_INPUT_T;

//the structure of the PLAY TONE proactive command
typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities exist
    BOOLEAN is_tone_exist;  //is the tone object exist in the command
    BOOLEAN is_duration_exist;  //is the play tone duration exist in the command
    BOOLEAN is_icon_id;              //does the ICON id exist
    SIMAT_DO_ALPHA_ID_T  alpha_id;   //the alpha identities
    SIMAT_TONE_ID_E  tone;   //the tone to be played,is it not exist,then play the "general beep"
    uint32 duration;   //the duraiton to play the tone,if not exist,the duration is decided by the maufacture
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identifier
}SIMAT_PLAY_TONE_T;

//the structure of the SETUP MENU proactive command
typedef struct
{
    BOOLEAN help_info;      //TRUE:help information avaliable,FALSE:no help information
    BOOLEAN is_action_id;   //is the Items next action indicator object exist in the command
    BOOLEAN is_icon_id;     //is the icon idnetities object extist in the command
    BOOLEAN is_icon_list;   //is the cion idnetifier list object exist in the command
    BOOLEAN is_select_prefer;       //FALSE,no seclection preference,TRUE,selection using soft key preferred
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha id object
    SIMAT_DO_ITEM_T    item; //the struct storing the information of all sub-menus
    SIMAT_DO_NEXT_ACTION_ID_T   next_action;    //the next action indicator list object
    SIMAT_DO_ICON_ID_T  icon_id;    //the icon idnetities object
    SIMAT_DO_ICON_LIST_T icon_list; //the Item icon identifier list
}SIMAT_SETUP_MENU_T;

//the structure of the SELECT ITEM proactive command
typedef struct
{
    BOOLEAN speci_type;     //the presenttaion type is specified or not in the command
    BOOLEAN data_option;    //meaning only when the speci_type is true TRUE:the presentaion 
                            //is a choice of data values,FALSE:the presentaion is a navigation choice
    BOOLEAN help_info;      //the help information avialable or not
    BOOLEAN is_alpha_id;       //is the alpha id object exist in the command
    BOOLEAN is_action_id;   //is the Items next action indicator object exist in the command
    BOOLEAN is_icon_id;     //is the icon idnetities object extist in the command
    BOOLEAN is_icon_list;   //is the cion idnetifier list object exist in the command
    BOOLEAN is_item_id;     //is the item identifier data object exist in the proactive command
    BOOLEAN is_select_prefer;       //FALSE,no seclection preference,TRUE,selection using soft key preferred
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha id object
    SIMAT_DO_ITEM_T    item; //the struct storing the information of all sub-menus
    SIMAT_DO_NEXT_ACTION_ID_T   next_action;    //the next action indicator list object
    SIMAT_DO_ITEM_ID    item_id;    //the item identifier data object
    SIMAT_DO_ICON_ID_T  icon_id;    //the icon idnetities object
    SIMAT_DO_ICON_LIST_T icon_list; //the Item icon identifier list
}SIMAT_SELECT_ITEM_T;

//the structure of the SETUP IDLE MODE TEXT proactive command
typedef struct
{
    BOOLEAN is_icon_id;     //si the icon identities object exist in the command
    SIMAT_DO_TEXT_STRING_T  text_string;    //the text string object
    SIMAT_DO_ICON_ID_T  icon_id;    //the icon identities object
}SIMAT_SETUP_IDLE_TEXT_T;

//the structure of the LANGUAGE NOTIFICATION proactive command
typedef struct
{
    BOOLEAN specific_lang_noti; //TRUE:specific language notification,and the Language object will exist
                                //FALSE:none-specific language notification,the language object will not exist
    SIMAT_DO_LANG_T lang;   //currently used language.
}SIMAT_LANG_NOTIFICATION_T;

typedef struct
{
    BOOLEAN is_text_attribute;     //is the text attribute object extist in the command
    BOOLEAN is_text_attribute_list;   //is the text attribute list object exist in the command
    SIMAT_DO_TEXT_ATTRIBUTE_T text_attribute;    //the text attribute object
    SIMAT_DO_TEXT_ATTRIBUTE_LIST_T text_attribute_list;    //the text attribute list object
}SIMAT_TEXT_ATTRIBUTE_T;

//-------------------------------SMS relative command----------------------
//the structure of the SEND SHORT MSSSAGE proactive command
typedef struct
{
    BOOLEAN is_packet_IND;	//true:sms packeting by ME required,false:packeting not required
    BOOLEAN is_alpha_id;    //is the alpha identifier object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exit in the command
    BOOLEAN is_address;     //is the address object exist in the command
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha identifier object
    MN_CALLED_NUMBER_T  address;    //the address object
    MN_L3_MSG_UNIT_T tpdu;   //the TPDU object
    SIMAT_DO_ICON_ID_T icon_id;    //the icon idntities object
}SIMAT_SEND_SM_T;
 

//the structure of the SEND SS proactive command
typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identifier object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exit in the command
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha identifier object
    SIMAT_DO_SS_STRING_T  ss;   //the SS string object
    SIMAT_DO_ICON_ID_T  icon_id;    //the icon idntities object
}SIMAT_SEND_SS_T;

//the structure of the SEND USSD proactive command
typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identifier object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exit in the command
    SIMAT_DO_ALPHA_ID_T alpha_id;   //the alpha identifier object
    MN_SS_USSD_DATA_T  ussd;   //the USSD string object
    SIMAT_DO_ICON_ID_T  icon_id;    //the icon idntities object
}SIMAT_SEND_USSD_T;



//the set up call type
typedef enum
{
    SIMAT_STC_FREE,         //set up call,but only if not current busy on another call
    SIMAT_STC_FREE_REDIAL,  //set up call,but only if not current busy on another call,with redial
    SIMAT_STC_HOLD,         //set up call,putting call other calls(if any)on hold
    SIMAT_STC_HOLD_REDIAL,  //set up call,putting call other calls(if any)on hold,with redial
    SIMAT_STC_DISCON,       //set up call,disconnecting all other call(if any)
    SIMAT_STC_DISCON_REDIAL,    //set up call,disconnecting all other call(if any),with redial    
    SIMAT_STC_RESERVED      //reserved
}SIMAT_SETUP_CALL_TYPE_E;

//the structure of the SET UP CALL proactive command
typedef struct
{
    BOOLEAN is_ccp;        
	BOOLEAN is_cps;         
    BOOLEAN is_duration;    
    SIMAT_SETUP_CALL_TYPE_E call_type;  
    MN_CALLED_NUMBER_T  address;       
    SIMAT_DO_CCP_T      ccp;            
    MN_SUBADDR_T cps;       
    uint32 duration;   
 } SIMAT_SETUP_CALL_PARAM_T;

typedef struct
{
    BOOLEAN is_uc_alpha_id;    
    BOOLEAN is_uc_icon_id;    
	BOOLEAN is_cs_alpha_id; 
    BOOLEAN is_cs_icon_id; 
    SIMAT_DO_ALPHA_ID_T uc_alpha_id;    
    SIMAT_DO_ICON_ID_T  uc_icon_id; 
    SIMAT_DO_ALPHA_ID_T cs_alpha_id;    
	SIMAT_DO_ICON_ID_T  cs_icon_id;    
	SIMAT_SETUP_CALL_PARAM_T call_parameter;
}SIMAT_SETUP_CALL_T; 

//the structure of the SEND DTMF COMMAND proactive command
typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_DO_DTMF_T         dtmf;       //the DTMF string object of the command
}SIMAT_SEND_DTMF_T;


//the refresh type
typedef enum
{
    SIMAT_RF_INITIAL_FULL_FILE_CHANGE,  //SIM initialization and full file change notification
    SIMAT_RF_FILE_CHANGE,               //SIM file change notification
    SIMAT_RF_INITIAL_FILE_CHANGE,       //SIM initialization and file change notification
    SIMAT_RF_INITIAL,                   //SIM initialization
    SIMAT_RF_RESET,                     //SIM reset
/* 2.5g(dual sim)/3g integration */
    SIMAT_RF_3G_APPLICATIN_RESET, 
    SIMAT_RF_3G_SESSION_RESET,               
    SIMAT_RF_RESERVED                   //0x07 to 0xff is reversed vlaue
}SIMAT_REFRESH_TYPE_E;

/* correct refresh function */
//the structure of the REFRESH proactive command
typedef struct
{
    BOOLEAN is_files_exist; //is the file list object exist in the command
    SIMAT_REFRESH_TYPE_E refresh_type;  //the refresh command type.
    #ifdef PRODUCT_DM
        SIMAT_REFRESH_FILE_LIST_T    file_list;  //the file list object,exist when the refresh type is 
                                                 //"SIM file change" and "sim initialization and file change"
    #else
#ifdef _SUPPORT_IPC_
        SIMAT_REFRESH_FILE_LIST_T    file_list;
#else
        SIMAT_DO_FILE_LIST_T    file_list;  //the file list object,exist when the refresh type is 
                                            //"SIM file change" and "sim initialization and file change"
#endif
    #endif
}SIMAT_REFRESH_T;


//the local information type
typedef enum
{
    SIMAT_LI_LOCATION_INFO, //the location information(MCC,MNC,LACm and Cell identity)
    SIMAT_LI_IMEI,  //IMEI of the ME
    SIMAT_LI_NMR,   //network measurement results
    SIMAT_LI_DT_TZ, //data,time and time zone
    SIMAT_LI_LANG,  //language setting
    SIMAT_LI_TIMING_AD, //timing advance
/* 2.5g(dual sim)/3g integration */
    SIMAT_LI_ACCESS_TECH,
    SIMAT_ESN_OF_TERMIANL,
    SIMAT_IMSISV_OF_TERMINAL,
    SIMAT_SEARCH_MODE,
    SIMAT_CHARGE_STATE_OF_BATTERY,
    SIMAT_MEID_OF_TERMINAL,
    SIMAT_CURRENT_WSID,
    SIMAT_LI_RESERVED   //reserved
}SIMAT_LOCAL_INFO_TYPE_E;

//the structure of the PROVIDE LOCAL INFORMATION proactive command
typedef struct
{
    SIMAT_LOCAL_INFO_TYPE_E info_type;  //the required local information type
}SIMAT_PROVIDE_LOCAL_INFO_T;

//the structure of the SET UP EVENT LIST proactive command
typedef struct
{
    SIMAT_DO_EVENT_LIST_T   event_list; //the event list object
}SIMAT_SETUP_EVENT_LIST_T;

typedef struct
{
    SIMAT_DO_ALPHA_ID_T alpha_id;  
}SIMAT_CC_TO_MMI_IND_T;

/* 2.5g(dual sim)/3g integration */
//the lauch borowser type
typedef enum
{
    SIMAT_LAUNCH_BROWSER_NOT_LAUCHED = 0, 
    SIMAT_LAUNCH_BROWSER_NOT_USED1, 
    SIMAT_LAUNCH_BROWSER_USE_EXIST, 
    SIMAT_LAUNCH_BROWSER_CLOSE_OLD_NEW_BROWSER, 
    SIMAT_LAUNCH_BROWSER_NOT_USED2,
    SIMAT_LAUNCH_BROWSER_TYPE_RFU
}SIMAT_LAUNCH_BROWSER_TYPE_E;



/* binli add usat proactive command data struct */
//the structure of the GET READ STATUS proactive command
typedef struct
{
    BOOLEAN is_browser_id;    //is the browser identifier object exist in the command 
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_gate_way;     //is the text_str object exist in the command
    BOOLEAN is_bear;     //is the bear object exist in the command
    SIMAT_LAUNCH_BROWSER_TYPE_E launch_browser_type;
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_DO_DEFAULT_TEXT_T gate_way;
    SIMAT_BROWSER_IDENTITY_TYPE_E browser_id;
    SIMAT_URL_T url;
    SIMAT_BEAR_T bear;
    uint8 file_path_num;
    SIMAT_PROVISION_FILE_PATH_T provision_file_path[MN_SIMAT_FILE_PATH_NUM];
}SIMAT_LAUNCH_BROWSER_T;

//the structure of the OPEN CHANNEL proactive command
typedef struct
{

    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_sub_address;     //is the sub_address object exist in the command
    BOOLEAN is_other_address;     //is the sub_address object exist in the command
    BOOLEAN is_text_str_login;     //is the text_str object exist in the command
    BOOLEAN is_text_str_pwd;     //is the text_str object exist in the command
    BOOLEAN is_duration1;     //is the duration1 object exist in the command
    BOOLEAN is_duration2;     //is the duration2 object exist in the command
    BOOLEAN is_trans_level;     //is the duration2 object exist in the command
    BOOLEAN is_data_dest_addr;     //is the duration2 object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_DO_DEFAULT_TEXT_T text_str_login;
    SIMAT_DO_DEFAULT_TEXT_T text_str_pwd;
    MN_CALLED_NUMBER_T  address;    //the address object
    SIMAT_SUB_ADDRESS_T sub_address;
    uint32 duration1;
    uint32 duration2;
    SIMAT_BEAR_DESC_T bear_desc;
    uint16 buf_size;
    SIMAT_OTHER_ADDRESS_T other_address;
    SIMAT_TRANS_LEVEL_T trans_level;
    SIMAT_OTHER_ADDRESS_T data_dest_address;
}SIMAT_OPEN_CHANNEL_CS_BEAR_T;


typedef struct
{

    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_sub_address;     //is the sub_address object exist in the command
    BOOLEAN is_other_address;     //is the sub_address object exist in the command
    BOOLEAN is_text_str_login;     //is the text_str object exist in the command
    BOOLEAN is_text_str_pwd;     //is the text_str object exist in the command
    BOOLEAN is_duration1;     //is the duration1 object exist in the command
    BOOLEAN is_duration2;     //is the duration2 object exist in the command
    BOOLEAN is_trans_level;     //is the duration2 object exist in the command
    BOOLEAN is_network_access_name;     //is the text_str object exist in the command
    BOOLEAN is_data_dest_addr;     //is the duration2 object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_DO_DEFAULT_TEXT_T text_str_login;
    SIMAT_DO_DEFAULT_TEXT_T text_str_pwd;
    SIMAT_SUB_ADDRESS_T sub_address;
    uint32 duration1;
    uint32 duration2;
    SIMAT_BEAR_DESC_T bear_desc;
    uint16 buf_size;
    uint8 network_access_name[MN_SIMAT_NETWORK_ACCESS_NAME_LEN];
    SIMAT_OTHER_ADDRESS_T other_address;
    SIMAT_TRANS_LEVEL_T trans_level;
    SIMAT_OTHER_ADDRESS_T data_dest_address;
}SIMAT_OPEN_CHANNEL_PCkDATA_SERVICE_BEAR_T;

typedef struct
{

    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_sub_address;     //is the sub_address object exist in the command
    BOOLEAN is_remote_address;     //is the sub_address object exist in the command
    BOOLEAN is_text_str_pwd;     //is the text_str object exist in the command
    BOOLEAN is_duration1;     //is the duration1 object exist in the command
    BOOLEAN is_duration2;     //is the duration2 object exist in the command
    BOOLEAN is_trans_level;     //is the duration2 object exist in the command
    BOOLEAN is_network_access_name;     //is the text_str object exist in the command
    BOOLEAN is_data_dest_addr;     //is the duration2 object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_DO_DEFAULT_TEXT_T text_str_pwd;
    SIMAT_SUB_ADDRESS_T sub_address;
    uint32 duration1;
    uint32 duration2;
    SIMAT_BEAR_DESC_T bear_desc;
    uint16 buf_size;
    uint8 network_access_name[MN_SIMAT_NETWORK_ACCESS_NAME_LEN];
    SIMAT_REMOTE_ENTITY_ADDRESS_T remote_address;
    SIMAT_TRANS_LEVEL_T trans_level;
    SIMAT_OTHER_ADDRESS_T data_dest_address;
}SIMAT_OPEN_CHANNEL_LOCAL_BEAR_T;

typedef struct
{

    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_other_address;     //is the sub_address object exist in the command
    BOOLEAN is_text_str_login;     //is the text_str object exist in the command
    BOOLEAN is_text_str_pwd;     //is the text_str object exist in the command
    BOOLEAN is_duration1;     //is the duration1 object exist in the command
    BOOLEAN is_duration2;     //is the duration2 object exist in the command
    BOOLEAN is_trans_level;     //is the duration2 object exist in the command
    BOOLEAN is_data_dest_addr;     //is the duration2 object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_DO_DEFAULT_TEXT_T text_str_login;
    SIMAT_DO_DEFAULT_TEXT_T text_str_pwd;
    uint32 duration1;
    uint32 duration2;
    SIMAT_BEAR_DESC_T bear_desc;
    uint16 buf_size;
    SIMAT_OTHER_ADDRESS_T other_address;
    SIMAT_TRANS_LEVEL_T trans_level;
    SIMAT_OTHER_ADDRESS_T data_dest_address;
}SIMAT_OPEN_CHANNEL_DEFALT_BEAR_T;

typedef struct
{

    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_other_address;     //is the sub_address object exist in the command
    BOOLEAN is_text_str_login;     //is the text_str object exist in the command
    BOOLEAN is_text_str_pwd;     //is the text_str object exist in the command
    BOOLEAN is_trans_level;     //is the duration2 object exist in the command
    BOOLEAN is_data_dest_addr;     //is the duration2 object exist in the command
    BOOLEAN is_net_access_name;
    SIMAT_CHANNEL_LINK_ESTABLISH_TYPE_E link_establish_type;
    SIMAT_CHANNEL_RECONNECT_TYPE_E reconnect_type; 
    SIMAT_CHANNEL_BACKGROUND_TYPE_E background_mode; 
    SIMAT_NET_ACCESS_NAME_T net_access_name;
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_DO_DEFAULT_TEXT_T text_str_login;
    SIMAT_DO_DEFAULT_TEXT_T text_str_pwd;
    SIMAT_BEAR_DESC_T bear_desc;
    uint16 buf_size;
    SIMAT_OTHER_ADDRESS_T other_address;
    SIMAT_TRANS_LEVEL_T trans_level;
    SIMAT_OTHER_ADDRESS_T data_dest_address;
}SIMAT_OPEN_CHANNEL_RELATED_GPRS_T;

typedef struct
{

    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_trans_level;     //is the duration2 object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    uint16 buf_size;
    SIMAT_TRANS_LEVEL_T trans_level;
}SIMAT_OPEN_CHANNEL_SERVER_MODE_T;

typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    uint8 channel_id;
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
}SIMAT_CLOSE_CHANNEL_T;

typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    uint8 channel_data_len;
    uint8 channle_id;
}SIMAT_RECEIVE_DATA_T;

typedef struct
{
    BOOLEAN is_send_immediate;
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    uint8 channel_id;
    SIMAT_CHANNEL_DATA_T channel_data;
}SIMAT_SEND_DATA_T;

typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    BOOLEAN is_device_filter;     //is the device_filter object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_SERVICE_SEARCHER_T service_searcher;
    SIMAT_DEVICE_FILTER_T  device_filter;
}SIMAT_SERVICE_SEACHER_T;


typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_ATTIBUTE_INFOMATION_T attibute_info;
}SIMAT_GET_SERVICE_INFO_T;

typedef struct
{
    BOOLEAN is_trans_level;    //is the trans_level object exist in the command
    SIMAT_TRANS_LEVEL_T trans_level;
    SIMAT_SERVICE_RECORD_T service_record;
}SIMAT_DECLARE_SERVICE_T;

typedef struct
{
    BOOLEAN is_defalt_frame_id;    //is the trans_level object exist in the command
    uint8 frame_id;
    uint8 default_frame_id;
    SIMAT_FRAME_LAYOUT_T frame_layout;
}SIMAT_SET_FRAME_T;

typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_msg_id;     //is the msg_id object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_MULTIMEDIA_MSGREF_T msg_ref;
    SIMAT_MULTIMEDIA_MSG_IDENTITY_T msg_id;
    SIMAT_DO_FILE_LIST_T mms_receive_file;
    SIMAT_MM_CONTENT_ID_T mm_content_id;
}SIMAT_RETRIVE_MULTINEDIA_MESSAGE_T;

typedef struct
{
    BOOLEAN is_alpha_id;    //is the alpha identities object exist in the command
    BOOLEAN is_msg_id;     //is the msg_id object exist in the command
    BOOLEAN is_icon_id;     //is the icon identities object exist in the command
    SIMAT_DO_ALPHA_ID_T     alpha_id;   //the alpha identities object
    SIMAT_DO_ICON_ID_T      icon_id;    //the icon identites object
    SIMAT_MULTIMEDIA_MSG_IDENTITY_T msg_id;
    SIMAT_DO_FILE_LIST_T mms_submission_file;
}SIMAT_SUBMIT_MULTINEDIA_MESSAGE_T;

typedef struct
{
    SIMAT_MULTIMEDIA_MSG_IDENTITY_T msg_id;
    SIMAT_DO_FILE_LIST_T mms_submission_file;
    SIMAT_MULTI_MEDIA_MSG_DISPLAY_PRIORITY_E priority;
    SIMAT_MULTI_MEDIA_MSG_CLEAR_MODE_E clear_mode;
}SIMAT_DISPLAY_MULTINEDIA_MESSAGE_T;

typedef struct
{
    BOOLEAN is_setup_menu; //whether the proactive command is SETUP MENU
    uint16 data_len;    //the data length
    uint8 data_arr[MN_SIMAT_MAX_RPDU_DATA_LEN]; //the data forward to at layer
}SIMAT_FWD_PC_T;

/* -cr110036 */
#ifdef __cplusplus
    }
#endif

#endif  //_SIMAT_PROACTIVE_H_
