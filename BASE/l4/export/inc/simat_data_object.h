/******************************************************************************
 ** File Name:      simat_data_object.h                                       *
 ** Author:         Raistlin.kong                                             *
 ** Date:           09/19/2001                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file defines the data type of the      *
 **			        SIMPLE-TLV data object.ref to section 12 "SIMPLE_TLV data *
 **                 object" GSM 11.14										  *                                                      
 ******************************************************************************
 
 

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2001.9        Raislin.Kong    Create									  *
 ** 2007.9        Bin.Li          modify for usat							  *
 ** 2008.3        Pizer.Fan       2.5g (dual sim) and 3g integration		  *
 ******************************************************************************/
 

#ifndef _SIMAT_DATA_OBJECT_H_
#define _SIMAT_DATA_OBJECT_H_
//guisen.deng; change the struct SIMAT_DO_TEXT_STRING_T 
#define MN_MAX_SIMAT_DATA_OBJECT_LEN  255  //the max  lenght of the array in SIMAT_DO_TEXT_STRING_T 

#ifndef PRODUCT_DM
#define MN_MAX_SIMAT_ITEM_NUM         50   //the max  number of Items in a STK data object
#else
#define MN_MAX_SIMAT_ITEM_NUM         255   //the max  number of Items in a STK data object
#endif

#define MN_MAX_SIMAT_CAUSE_LENGTH         32   //the max  number of Items in a STK data object
#define MN_SIMAT_CB_PAGE_LEN          88  //the fixed length of CB message


#define MN_SIMAT_URL_LEN                                           152  
#define MN_SIMAT_FILE_PATH_LEN                                     40  
#define MN_SIMAT_FILE_PATH_NUM                                     10  
#define MN_SIMAT_SUB_ADDR_LEN                                       40  
#define MN_SIMAT_OTHER_ADDR_LEN                                  40  
#ifndef PRODUCT_DM
#define MN_SIMAT_BEAR_LEN                                                50  
#else
#define MN_SIMAT_BEAR_LEN                                                10  
#endif
#define MN_SIMAT_NETWORK_ACCESS_NAME_LEN                40  
#define MN_SIMAT_CHANNEL_DATA_LEN                              255 
#define MN_SIMAT_DEVICE_FILTER_LEN                              60 
#define MN_SIMAT_SERVICE_SEARCHER_LEN                       60 
#define MN_SIMAT_ATTIBUTE_INFO_LEN                             60 
#define MN_SIMAT_SERVICE_RECORD_LEN                           100
#define MN_SIMAT_FRAME_RELATIVE_SIZE_LEN                  16
#define MN_SIMAT_MULTIMEDIA_MSGREF_LEN                     10
#define MN_SIMAT_MULTIMEDIA_MSGID_LEN                        20
#define MN_SIMAT_MM_CONTENT_DATA_LEN                         200
#define MN_SIMAT_MULTIMEDIA_MSG_TRANS_STATUS_LEN   10
#define MN_SIMAT_FILE_LIST_LEN                                     255
#define MN_SIMAT_BROWSER_STATUS_LEN                        20
#define MN_SIMAT_BEAR_QOS_LEN                        20

#define MN_SIMAT_MAX_RPDU_DATA_LEN                                 256
#define MN_SIMAT_MAX_TEXT_ATTRIBUTE_LEN                     4  
#define MN_SIMAT_MAX_TEXT_ATTRIBUTE_LIST_LEN             60    

#ifdef __cplusplus
    extern "C"
    {
#endif

#include "sim_type.h"

/*typedef struct
{
    uint8   len;     //the dialling number string length
    NUMBER_TYPE_E   ton;    //the nubmer type of the address
    NUMBER_PLAN_E   npi;    //the number plan of the address
    uint8*  dn_ptr;  //the bcd array of the dialling number
}SIMAT_DO_ADDRESS_T;*/

typedef struct
{
    MN_NUMBER_TYPE_E  ton;//the nubmer type of the address
    MN_NUMBER_PLAN_E  npi;//the number plan of the address
    uint16			  len;//the dialling number string length
    uint8			  dn_ptr[MN_MAX_ADDR_BCD_LEN];//the bcd array of the dialling number
}SIMAT_DO_ADDRESS_T; 

typedef enum 
{
    SIMAT_BEAR_TYPE_CSD = 0x01,          
    SIMAT_BEAR_TYPE_GPRS = 0x02,       
    SIMAT_BEAR_TYPE_GPRS_3G = 0x02,    /* for gprs or 3g qos*/   
    SIMAT_BEAR_TYPE_DEFALT_BEAR = 0x03,
    SIMAT_BEAR_TYPE_LOCAL_LINK_INDEP = 0x04,
    SIMAT_BEAR_TYPE_BLUE_TOOTH = 0x05,
    SIMAT_BEAR_TYPE_IRDA = 0x06,
    SIMAT_GBEAR_TYPE_RS232 = 0x07,
    SIMAT_BEAR_TYPE_TIA_EIA = 0x08,

    SIMAT_BEAR_TYPE_3GPP_IWLAN = 0x0A, 
    SIMAT_BEAR_TYPE_USB = 0x10, 
	SIMAT_BEAR_TYPE_MAX     // Reserved, user can't use it.
}SIMAT_BEAR_TYPE_E;

typedef enum 
{        
    SIMAT_PACKET_DATA_PROTOCAL_IP = 0x02,       
    SIMAT_PACKET_DATA_PROTOCAL_RESERVED
}SIMAT_PACKET_DATA_PROTOCAL_TYPE_E;

typedef enum
{
    SIMAT_CHANNEL_MODE_NO_FURTHER_INFO=0,
    SIMAT_CHANNEL_MODE_NOT_USED1,
    SIMAT_CHANNEL_MODE_NOT_USED2,
    SIMAT_CHANNEL_MODE_NOT_USED3,
    SIMAT_CHANNEL_MODE_NOT_USED4,
    SIMAT_CHANNEL_MODE_LINK_DROPPED,
    SIMAT_CHANNEL_MODE_RFU
}SIMAT_CHANNEL_MODE_INFO_E;

typedef enum 
{
    SIMAT_BEAR_IDENTITY_TYPE_TECH_INDEP= 0x00,          
    SIMAT_BEAR_IDENTITY_TYPE_BLUETOOTH = 0x01,
    SIMAT_BEAR_IDENTITY_TYPE_IRDA = 0x02,
    SIMAT_BEAR_IDENTITY_TYPE_RS232 = 0x03,
    SIMAT_BEAR_IDENTITY_TYPE_USB = 0x04,
	SIMAT_BEAR_IDENTITY_TYPE_OTHER     // Reserved, user can't use it.
}SIMAT_BEAR_IDENTITY_TYPE_E;

typedef enum 
{
    SIMAT_LAYOUT_TYPE_HORIZONTAL = 0x00,          
    SIMAT_LAYOUT_TYPE_VERTICAL = 0x01,
	SIMAT_LAYOUT_TYPE_OTHER     // Reserved, user can't use it.
}SIMAT_LAYOUT_TYPE_E;

typedef enum 
{
    SIMAT_ADDRESS_TYPE_IPV4 = 0x21,         
    SIMAT_ADDRESS_TYPE_IPV6 = 0x57,   
	SIMAT_ADDRESS_TYPE_OTHER     // Reserved, user can't use it.
}SIMAT_ADDRESS_TYPE_E;

typedef enum 
{
    SIMAT_REMOTE_ADDRESS_CODE_TYPE_IEEE802 = 0x01,         
    SIMAT_REMOTE_ADDRESS_CODE_TYPE_IRDA = 0x02,   
	SIMAT_REMOTE_ADDRESS_CODE_TYPE_OTHER     // Reserved, user can't use it.
}SIMAT_REMOTE_ADDRESS_CODE_TYPE_E;

typedef enum 
{
    SIMAT_TRANS_LEVEL_TYPE_UDP_CLIENT = 0x01,         
    SIMAT_TRANS_LEVEL_TYPE_TCP_CLIENT = 0x02,   
    SIMAT_TRANS_LEVEL_TYPE_TCP_SERVER = 0x03,   
    SIMAT_TRANS_LEVEL_TYPE_OTHER     // Reserved, user can't use it.
}SIMAT_TRANS_LEVEL_TYPE_E;

typedef enum
{
    SIMAT_BATTERY_STATE_BATTERY_VERY_LOW = 0x0,
    SIMAT_BATTERY_STATE_BATTERY_LOW,
    SIMAT_BATTERY_STATE_BATTERY_AVERAGE,
    SIMAT_BATTERY_STATE_BATTERY_GOOD,
    SIMAT_BATTERY_STATE_BATTERY_FULL,
    SIMAT_BATTERY_STATE_NOT_APPLICABLE = 0xFE,
    SIMAT_BATTERY_STATE_UNKNOWN = 0xFF
}SIMAT_BATTERY_STATE_E;

typedef enum
{
    SIMAT_BROWSER_IDENTITY_DEFAULT = 0,
    SIMAT_BROWSER_IDENTITY_WML,
    SIMAT_BROWSER_IDENTITY_HTML,
    SIMAT_BROWSER_IDENTITY_XHTML,
    SIMAT_BROWSER_IDENTITY_CHTML,
    SIMAT_BROWSER_IDENTITY_RFU
}SIMAT_BROWSER_IDENTITY_TYPE_E;

typedef enum
{
    SIMAT_BROWSER_TERMINATION_BY_TERMINAL = 0x0,
    SIMAT_BROWSER_TERMINATION_ERROR,
    SIMAT_BROWSER_TERMINATION_OTHER
}SIMAT_BROWSER_TERMINATION_CAUSE_E;

typedef enum
{
    SIMAT_CHANNEL_MODE_UICC = 0x0,
    SIMAT_CHANNEL_MODE_OTHER
}SIMAT_CHANNEL_MODE_E;



typedef enum
{
    SIMAT_CHANNEL_TCP_STATE_CLOSED = 0x0,
    SIMAT_CHANNEL_TCP_STATE_LISTEN,
    SIMAT_CHANNEL_TCP_STATE_ESTABLISH,
    SIMAT_CHANNEL_TCP_STATE_OTHER
}SIMAT_CHANNEL_TCP_STATE_E;

typedef enum
{
    SIMAT_ACCESS_TECHNOLOGY_TYPE_GSM = 0x0,
    SIMAT_ACCESS_TECHNOLOGY_TYPE_TIA_EIA_553,
    SIMAT_ACCESS_TECHNOLOGY_TYPE_TIA_EIA_136,
    SIMAT_ACCESS_TECHNOLOGY_TYPE_UTRAN,
    SIMAT_ACCESS_TECHNOLOGY_TYPE_TETRA,
    SIMAT_ACCESS_TECHNOLOGY_TYPE_TIA_EIA_95,
    SIMAT_ACCESS_TECHNOLOGY_TYPE_TIA_EIA_IS_2000,
    SIMAT_ACCESS_TECHNOLOGY_TYPE_OTHER
}SIMAT_ACCESS_TECHNOLOGY_TYPE_E;


typedef enum
{
    SIMAT_NET_SEARCHER_MODE_MANU = 0x0,
    SIMAT_NET_SEARCHER_MODE_AUTO,
    SIMAT_NET_SEARCHER_MODE_RFU
}SIMAT_NET_SEARCHER_MODE_E;


typedef enum
{
    SIMAT_CHANNEL_LINK_ESTABLISH_ON_DEMAND = 0x0,
    SIMAT_CHANNEL_LINK_ESTABLISH_IMMEDIATE,
    SIMAT_CHANNEL_LINK_ESTABLISH_OTHER
}SIMAT_CHANNEL_LINK_ESTABLISH_TYPE_E;
typedef enum
{
    SIMAT_CHANNEL_RECONNECT_NO_AUTOMATIC = 0x0,
    SIMAT_CHANNEL_RECONNECT_AUTOMATIC,
    SIMAT_CHANNEL_RECONNECT_OTHER
}SIMAT_CHANNEL_RECONNECT_TYPE_E;
typedef enum
{
    SIMAT_CHANNEL_NO_BACKGROUND = 0x0,
    SIMAT_CHANNEL_LINK_ESTABLISH_IMMEDIATE_BACKGROUND,
    SIMAT_CHANNEL_BACKGROUND_OTHER
}SIMAT_CHANNEL_BACKGROUND_TYPE_E;

typedef enum
{
    SIMAT_MULTI_MEDIA_MSG_DISPLAY_NORMAL = 0x0,
    SIMAT_MULTI_MEDIA_MSG_DISPLAY_HIGH,
    SIMAT_MULTI_MEDIA_MSG_DISPLAY_PRIORITY_OTHER
}SIMAT_MULTI_MEDIA_MSG_DISPLAY_PRIORITY_E;

typedef enum
{
    SIMAT_CLEAR_AFTER_DELAY = 0x0,
    SIMAT_WAIT_FOR_USER_CLEAR,
    SIMAT_MULTI_MEDIA_MSG_CLEAR_OTHER
}SIMAT_MULTI_MEDIA_MSG_CLEAR_MODE_E;

typedef enum
{
    SIMAT_EN_MMI_OK,
    SIMAT_EN_MMI_OK_WITH_PC,
    SIMAT_EN_MMI_SIM_BUSY,
    SIMAT_EN_MMI_OK_WITH_DATA,
    SIMAT_EN_MMI_WARNING_NO_INFORMATION,
    SIMAT_EN_MMI_ERROR_MEM_UNCHANGE,
    SIMAT_EN_MMI_ERROR_MEM_CHANGED,
    SIMAT_EN_MMI_ERROR_INSTRUCTION_CODE,
    SIMAT_EN_MMI_ERROR_LC_NOT_SUPPORT,
    SIMAT_EN_MMI_ERROR_SECURITY_NOT_SUPPORT,
    SIMAT_EN_MMI_ERROR_WITH_DATA,
    SIMAT_EN_MMI_CARD_NOT_READY,
    SIMAT_EN_MMI_CARD_NOT_SUPPORT,
    SIMAT_EN_MMI_ERROR_UNKNOW
}SIMAT_ENVELOPE_RESULT_TO_MMI_E;
  
//#define MN_MAX_SIMAT_DATA_OBJECT_LEN    256

typedef struct
{
    uint8   len;  //the byte number of the alpha id        
    uint8   id_ptr[MN_MAX_SIMAT_DATA_OBJECT_LEN];    //the alpha id array
}SIMAT_DO_ALPHA_ID_T;

typedef enum 
{
    //Standard supervisory tones:
    SIMAT_DIAL_TONE = 0x01,            //Dial tone
    SIMAT_CALLED_SUBSCRIBER_BUSY_TONE, //Called subscriber busy
    SIMAT_CONGESTION_TONE,             //Congestion
    SIMAT_RADIO_PATH_ACK_TONE,         //Radio path acknowledge
    SIMAT_RADIO_PATH_NOT_AVAIL_TONE,   //Radio path not available / Call dropped
    SIMAT_ERROR_SPECIAL_INFO_TONE,     //Error / Special infomation
    SIMAT_CALL_WAITING_TONE,           //Call waiting tone
    SIMAT_RINGING_TONE,                //Ringing tone
    //ME proprietary tones:
    SIMAT_GENERAL_BEEP_TONE = 0x10,    //General beep
    SIMAT_POSITIVE_ACK_TONE,           //Positive acknowledgement tone
    SIMAT_NEGATIVE_ACK_ERROR_TONE,     //Negative acknowledgement or error tone

    // User could add new at here.
    // ......
    
	SIMAT_TONE_ID_MAX     // Reserved, user can't use it.
} SIMAT_TONE_ID_E;

typedef struct
{
#ifndef PRODUCT_DM
    uint8   len;        //the byte number of the called party sub address 
#else
    uint8   no_cps_ptr;
#endif
    uint8*  cps_ptr; //the called party sub address
}SIMAT_DO_CPS_T;

#define SIMAT_MAX_CCP_BYTE_NUM 14

typedef struct
{
    uint8   len;        //the byte number of the capability configration parameters
    uint8   ccp_ptr[SIMAT_MAX_CCP_BYTE_NUM];  //the CCP array
}SIMAT_DO_CCP_T;

typedef struct
{
    uint8   len;       //the Cell Boradcast length,must be 88 
    uint8   page_arr[MN_SIMAT_CB_PAGE_LEN];  //the cell broad cast page array
}SIMAT_DO_CB_PAGE_T;


typedef struct
{
    uint32 duration;        //the time duration,the time unit is millisencond
}SIMAT_DO_DURATION_T;

typedef uint8    SIMAT_DO_ITEM_ID;

typedef struct DEF_SIMAT_DO_ITEM_T
{
    uint8 item_data_object_num; //number of sub-menus
    uint8 item_data_object_buf[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    uint8 item_data_object_info[MN_MAX_SIMAT_ITEM_NUM][3];//three-dimension array, storing every sub-menu's 
	                                                      //ID,length and the starting position in the above buf
}SIMAT_DO_ITEM_T;

typedef struct
{
    uint8 id;
    uint8 len;
    uint8 item_content_entry_buf[MN_MAX_SIMAT_DATA_OBJECT_LEN];	 
}SIMAT_ITEMTEMPLATE_T;

typedef struct
{
    uint8   min_len;    //the mnimum legnth of response
    uint8   max_len;    //the maximum length of response
}SIMAT_DO_RESPONSE_LEN_T;

typedef struct
{
    uint8   len;        //the length of the SMS TPDU
    uint8   tpdu_ptr[MN_SMS_SIM_SMS_LEN];   //the TPDU array
}SIMAT_DO_SMS_TPDU_T;  


typedef struct
{
    uint8   len;    //the text string length
    uint8   dcs;    //the data coding scheme of the text string
    uint8   text_ptr[MN_MAX_SIMAT_DATA_OBJECT_LEN]; //the text string array
}SIMAT_DO_TEXT_STRING_T;

//??? should be defined by the MMI
typedef enum
{
    SIMAT_TONE_DIAL = 0x01,
    SIMAT_TONE_CALLED_SUBSCRIBER_BUSY,
    SIMAT_TONE_CONGESTION,
    SIMAT_TONE_RADIO_PATH_ACK,
    SIMAT_TONE_CALL_DROPPED,
    SIMAT_TONE_ERROR,
    SIMAT_TONE_CALL_WAITING,
    SIMAT_TONE_RINGING,
    SIMAT_TONE_GENERAL_BEEP = 0x10,
    SIMAT_TONE_POSITIVE_ACK,
    SIMAT_TONE_NEGATIVE_ACK
}SIMAT_TONE_E;


typedef struct
{
    SIMAT_TONE_E tone;  //the tone type
}SIMAT_DO_TONE_T;

typedef struct
{
    uint8   len;        //the ussd string length
    uint8   dcs;        //the data coding scheme of the USSD string
    uint8*  str_ptr;   //the ussd string
}SIMAT_DO_USSD_STRING_T;

#define SIMAT_MAX_FILE_NUM_IN_LIST  40
#define SIMAT_MAX_FILE_DATA_IN_LIST  255

typedef struct
{
    uint8   file_num;   //the file number in the file list
    SIM_FILE_NAME_E file_arr[SIMAT_MAX_FILE_NUM_IN_LIST];   //the file array
}SIMAT_DO_FILE_LIST_T;

/* correct refresh function */
typedef struct
{
    uint8   file_num;   //the file number in the file list
    uint16 file_data_len;
    uint8  file_data_arr[SIMAT_MAX_FILE_DATA_IN_LIST];   //the file array
}SIMAT_REFRESH_FILE_LIST_T;


#define SIMAT_MCC_MNC_BCD_LEN  3 
typedef struct
{
    uint16  lac;    //the local area code
    uint16  cell_id;    //the cell id
    uint8   mcc_mnc_arr[SIMAT_MCC_MNC_BCD_LEN]; //the MCC and MNC bcd code array
}SIMAT_DO_LOCATION_INFO_T;

#define SIMAT_IMEI_LEN      8
typedef struct
{
    uint8   imei_arr[SIMAT_IMEI_LEN];   //the imei array
}SIMAT_DO_IMEI_T;

#define SIMAT_IMEISV_LEN      9
typedef struct
{
    uint8   imeisv_arr[SIMAT_IMEISV_LEN];   //the imeisv array
}SIMAT_DO_IMEISV_T;


//the length of the Network measurement result
#define SIMAT_NMR_LEN   16
typedef struct
{
    uint8   nmr[SIMAT_NMR_LEN]; //the network measurement result
}SIMAT_DO_NMR_T;

typedef struct
{
    uint8   len;    //the default text length
    uint8   dcs;    //the data coding scheme of the default text
    uint8   text_ptr[MN_MAX_SIMAT_DATA_OBJECT_LEN]; //the default text
}SIMAT_DO_DEFAULT_TEXT_T;


typedef struct
{
    uint8   len;    //the next action indictor list length
    uint8  next_action_list[MN_MAX_SIMAT_ITEM_NUM];    //the action indicator array
}SIMAT_DO_NEXT_ACTION_ID_T;


typedef enum
{
    SIMAT_EVENT_MT_CALL = 0x0,
    SIMAT_EVENT_CALL_CON,
    SIMAT_EVENT_CALL_DISCON,
    SIMAT_EVENT_LOCATION_STATUS,
    SIMAT_EVENT_USER_ACTIVITY,
    SIMAT_EVENT_IDLE_SCREEN_AVIALBLE,
    SIMAT_EVENT_CARD_READ_STATUS,
    SIMAT_EVENT_LAN_SEL,
    SIMAT_EVENT_BROESWER_TERMINATION,
    SIMAT_EVENT_DATA_AVAILABLE,
    SIMAT_EVENT_CHANNEL_STATUS,
    SIMAT_EVENT_ACCESS_TECHNOLOGY_CHANHE,
    SIMAT_EVENT_DISPLAY_PARA_CHANGE,
    SIMAT_EVENT_LOCAL_CONNECTION,
    SIMAT_EVENT_NET_SEARCHER_MODE_CHANGE,
    SIMAT_EVENT_BROWSER_STATUS,
    SIMAT_EVENT_FRAME_INFO_CHANGE,
    SIMAT_EVENT_REVERSED
}SIMAT_EVENT_E;


typedef struct
{
    uint8   num;    //the number of the event in the list
    SIMAT_EVENT_E  events_arr[SIMAT_EVENT_REVERSED];    //the event list nubmer,should convert from the uint8 to SIMAT_EVENT_E
}SIMAT_DO_EVENT_LIST_T;


typedef struct
{
    uint8   len;        //the cause length
    uint8   cause_arr[MN_MAX_SIMAT_CAUSE_LENGTH];  //the define
}SIMAT_DO_CAUSE_T;


//???defined by the MM
typedef enum
{
    SIMAT_LS_NORMAL_SERVICE,
    SIMAT_LS_LIMITED_SERVICE,
    SIMAT_LS_NO_SERVICE
}SIMAT_LOCATION_STATUS_E;

typedef struct
{
    SIMAT_LOCATION_STATUS_E status; //the location status indicate the current service state of the MS
}SIMAT_DO_LOCATION_STATUS_T;

#define SIMAT_MAX_TI_NUM    5
typedef struct
{
    uint8   ti_len;     //the transaction id list length
    uint8   tis_ptr[SIMAT_MAX_TI_NUM];  //the transaction id list
}SIMAT_DO_TI_LIST_T;

#define SIMAT_MAX_BCCH_NUM 16
typedef struct
{
    uint8   len;    //the BCCH channel list array length
    uint8   bcch_list[SIMAT_MAX_BCCH_NUM];    //the BCCH channel list
}SIMAT_DO_BCCH_LIST_T;

typedef struct
{
    BOOLEAN is_self_explanatory;    //is the icon self explanatory.
    uint8   icon_id;    //the icon id in the EFimg
}SIMAT_DO_ICON_ID_T;


typedef struct
{
    uint8   num;  //the icon id list length       
    BOOLEAN is_self_explanatory; //is the icon self explanatory?  
    uint8  icon_ids[MN_MAX_SIMAT_ITEM_NUM];   //array storing the icon ids
}SIMAT_DO_ICON_LIST_T;

typedef struct
{
    uint8   text_attribute[MN_SIMAT_MAX_TEXT_ATTRIBUTE_LEN];    //the text attibute
}SIMAT_DO_TEXT_ATTRIBUTE_T;

typedef struct
{
    uint8   num;  //the text attibute list length       
    uint8  text_attribute_list[MN_SIMAT_MAX_TEXT_ATTRIBUTE_LIST_LEN];   //array storing the text attibute list 
}SIMAT_DO_TEXT_ATTRIBUTE_LIST_T;

typedef uint8    SIMAT_DO_FRAME_ID;

//the multicard relative data object will define only when the SIMAT
//support the class A
//ifdef SIMAT_CLASS_A

//endif  //SIMAT_CLASS_A

#define SIMAT_DT_TZ_LEN     7
typedef struct
{
    uint8   dt_tz_arr[SIMAT_DT_TZ_LEN];
}SIMAT_DO_DT_TZ_T;

//there are data object that relative to AT command which can only be 
//used when the SIMAT support the SIMAT class B
//ifdef SIMAT_CLASS_B
//endif  //SIMAT_CLASS_B

typedef enum
{
    SIMAT_BCR_ALTERNATE = 0x01,
    SIMAT_BCR_SEQUENTIAL = 0x03
}SIMAT_BCR_INDICATOR_E;

typedef struct
{
    SIMAT_BCR_INDICATOR_E bcr_indicator;
}SIMAT_DO_BCR_INDICATOR_T;

typedef struct
{
    uint8   len;    //the length of the immediate response 
    uint8   dtmf[MN_MAX_SIMAT_DATA_OBJECT_LEN];   //the pointer to the DTMF string
}SIMAT_DO_DTMF_T;

#define MN_SIMAT_LANGE_LEN  2
typedef struct
{
    uint8 lang[MN_SIMAT_LANGE_LEN];    //the language
}SIMAT_DO_LANG_T;

typedef struct
{
    BOOLEAN is_me_idle;     //whether the ME is in idle status
    uint8   timing_ad;      //the timing advance value
}SIMAT_DO_TIMING_AD_T;

//the terminal response and also the tag of the coresponding terminal response
typedef enum
{
    SIMAT_TR_SUCCESS = 0x0,
    SIMAT_TR_PATRIAL_COM,
    SIMAT_TR_MISS_INFO,
    SIMAT_TR_REFRESH_EF_READ_ADD,
    SIMAT_TR_ICON_FAIL_DISPLAY,
    SIMAT_TR_MODIFY_BY_CC_OF_SIM,
    SIMAT_TR_LIMITED_SERVICE,
    SIMAT_TR_WITH_MODIFICATION,
    SIMAT_TR_TERMINATED_BY_USER = 0x10,
    SIMAT_TR_BACK_MOVE,
    SIMAT_TR_NO_RESPONSE,
    SIMAT_TR_HELP_INFO,
    SIMAT_TR_USSD_SS_TERMINATED_BY_USER,
    SIMAT_TR_ME_UNABLE = 0x20,
    SIMAT_TR_NE_UNABLE,
    SIMAT_TR_USER_DENY_CALL,
    SIMAT_TR_USER_CLEAR_CALL,
    SIMAT_TR_TIMER_STATE_CONTRADICTION,
    SIMAT_TR_TEMP_PROBLEM_IN_CC,
    SIMAT_TR_LAUNCH_BROWSER_ERR,
    SIMAT_TR_BEYOND_ME_CAP = 0x30,
    SIMAT_TR_TYPE_NOT_UNDERSTOOD,
    SIMAT_TR_DATA_NOT_UNDERSTOOD,
    SIMAT_TR_NUM_NOT_KNOW,
    SIMAT_TR_SS_ERROR,
    SIMAT_TR_SMS_ERROR,
    SIMAT_TR_REQUIRE_V_MISSING,
    SIMAT_TR_USSD_ERROR,
    SIMAT_TR_MULTI_CARD_COMMAND_ERROR,
    SIMAT_TR_PERM_PROBLEM_INN_CC,
    SIMAT_TR_BEAR_INDEPEND_ERROR,
    SIMAT_TR_NOT_DISPOSE_ACESS_TECH_ERROR
}SIMAT_GENERAL_PC_RESULT_E;

typedef struct
{
    SIMAT_GENERAL_PC_RESULT_E gen_result;   //the general result
    uint8  add_ptr[MN_MAX_SIMAT_DATA_OBJECT_LEN];    //the pointer point to the addtion information on the result
    uint8   len;    //the length of the addtion information
}SIMAT_DO_RESULT_T;

typedef struct
{
    uint8   id_len; 
    uint8   browser_id;
}SIMAT_BROWSER_IDENTITY_T;

typedef struct
{
    uint8   url_len; 
    uint8   url[MN_SIMAT_URL_LEN];
}SIMAT_URL_T;

typedef struct
{
    uint8   bear_len; 
    uint8   bear[MN_SIMAT_BEAR_LEN];
}SIMAT_BEAR_T;

/*
typedef struct
{
    SIMAT_BEAR_TYPE_E bear_type;
    uint8 precedence;
    uint8 delay;
    uint8 reliability;
    uint8 peak;
    uint8 mean;
    SIMAT_PACKET_DATA_PROTOCAL_TYPE_E pdp_type;
}SIMAT_BEAR_DESC_T;
*/
typedef struct
{
    SIMAT_BEAR_TYPE_E bear_type;
    uint16 bear_len;
    uint8 bear_desc_qos[MN_SIMAT_BEAR_QOS_LEN];
}SIMAT_BEAR_DESC_T;

typedef struct
{
    uint8   path_len; 
    uint8   file_path[MN_SIMAT_FILE_PATH_LEN];
}SIMAT_PROVISION_FILE_PATH_T;

typedef struct
{
    uint8   addr_len; 
    uint8   sub_addr[MN_SIMAT_SUB_ADDR_LEN];
}SIMAT_SUB_ADDRESS_T;

typedef struct
{
    uint8   addr_len; 
    SIMAT_ADDRESS_TYPE_E addr_type;
    uint8   other_addr[MN_SIMAT_SUB_ADDR_LEN];
}SIMAT_OTHER_ADDRESS_T;

typedef struct
{
    uint8  name_len; 
    uint8   net_access_name[MN_SIMAT_NETWORK_ACCESS_NAME_LEN];
}SIMAT_NET_ACCESS_NAME_T;

typedef struct
{
    uint8   addr_len; 
    SIMAT_REMOTE_ADDRESS_CODE_TYPE_E addr_code_type;
    uint8   remote_addr[MN_SIMAT_SUB_ADDR_LEN];
}SIMAT_REMOTE_ENTITY_ADDRESS_T;

typedef struct
{
    SIMAT_TRANS_LEVEL_TYPE_E trans_level_type;
    uint16 port;
}SIMAT_TRANS_LEVEL_T;

typedef struct
{
    uint8 data_len;
    uint8  data[MN_SIMAT_CHANNEL_DATA_LEN];
}SIMAT_CHANNEL_DATA_T;

typedef struct
{
    uint8 state_len;
    uint8  state_data[MN_SIMAT_BROWSER_STATUS_LEN];
}SIMAT_BROWSER_STATUS_T;

typedef struct
{
    uint8 local_bearer_identifier;
    uint8 data_len;
    uint8  device_filter[MN_SIMAT_DEVICE_FILTER_LEN];
}SIMAT_DEVICE_FILTER_T;

typedef struct
{
    uint8 local_bearer_identifier;
    uint8 data_len;
    uint8  service_searcher[MN_SIMAT_SERVICE_SEARCHER_LEN];
}SIMAT_SERVICE_SEARCHER_T;

typedef struct
{
    uint8 local_bearer_identifier;
    uint8 data_len;
    uint8  attibute_info[MN_SIMAT_SERVICE_SEARCHER_LEN];
}SIMAT_ATTIBUTE_INFOMATION_T;

typedef struct
{
    uint8 data_len;
    SIMAT_BEAR_IDENTITY_TYPE_E local_bear_identity;
    uint8 service_id;
    uint8  service_record[MN_SIMAT_SERVICE_RECORD_LEN];
}SIMAT_SERVICE_RECORD_T;


typedef struct
{
    uint8 data_len;
    SIMAT_LAYOUT_TYPE_E layout_type;
    uint8  frame_relative_size[MN_SIMAT_FRAME_RELATIVE_SIZE_LEN];
}SIMAT_FRAME_LAYOUT_T;

typedef struct
{
    uint8 data_len;
    uint8  muiltimedia_msgref[MN_SIMAT_MULTIMEDIA_MSGREF_LEN];
}SIMAT_MULTIMEDIA_MSGREF_T;

typedef struct
{
    uint8 data_len;
    uint8  muiltimedia_msg_id[MN_SIMAT_MULTIMEDIA_MSGID_LEN];
}SIMAT_MULTIMEDIA_MSG_IDENTITY_T;

typedef struct
{
    uint8 data_len;
    uint8  muiltimedia_msg_trans_status[MN_SIMAT_MULTIMEDIA_MSG_TRANS_STATUS_LEN];
}SIMAT_MULTIMEDIA_MSG_TRANS_STATUS_T;


typedef struct
{
    uint8 data_len;
    uint8  mm_content_data[MN_SIMAT_MM_CONTENT_DATA_LEN];
}SIMAT_MM_CONTENT_ID_T;

#define PLMNACT_MAX_NUM   8

typedef struct
{
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
}SIMAT_PLMN_T;

typedef struct
{
    BOOLEAN      is_plmn_exist;
    SIMAT_PLMN_T plmn_list[PLMNACT_MAX_NUM];
	uint16       acess_tech_id[PLMNACT_MAX_NUM];
	uint16       plmn_list_num;
}SIMAT_PLMNWACT_INFO_T;

/*+Bug 590293*/
#define SIMAT_MAX_AID_DATA_LEN 16

typedef struct
{
    uint8 aid_len;
    uint8 aid_data[SIMAT_MAX_AID_DATA_LEN];
}SIMAT_AID_T;
/*-Bug 590293*/

#ifdef __cplusplus
    }
#endif
#endif  //_SIMAT_DATA_OBJECT_H_
