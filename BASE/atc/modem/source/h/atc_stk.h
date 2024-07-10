/******************************************************************************
 ** File Name:      atc_stk.h                                             *
 ** Author:         Liuhan                                                    *
 ** DATE:           12/08/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file declares the functions that handle the AT command
 **                 regarding SS(supplement service)
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/08/2003     Liuhan         Create.                                     *
******************************************************************************/
#ifndef _ATC_STK_H
#define _ATC_STK_H

#include "sci_types.h"
#include "atc.h"
#include "simat_outport.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
{
#endif

/**---------------------------------------------------------------------------*
**                         Constant declaration                                     *
**---------------------------------------------------------------------------*/
//The constand which is according to <mode>
#define ATC_STK_DEACTIVATE            0
#define ATC_STK_ACTIVATE              1
#define ATC_STK_CONFIGURE             2

//The following is constant of command type.
#define ATC_SETUP_MENU                0
#define ATC_DISPLAY_TEXT              1
#define ATC_GET_INKEY                 2
#define ATC_GET_INPUT                 3
#define ATC_SETUP_CALL                4
#define ATC_PLAY_TONE                 5//@hanjun.liu
#define ATC_SELECT_ITEM               6
#define ATC_REFRESH                    7
#define ATC_SEND_SS                   8//@hanjun.liu
#define ATC_SEND_SMS                  9
#define ATC_SEND_USSD                 10//@hanjun.liu
#define   ATC_SETUP_EVENT_LIST           11
#define   ATC_SETUP_IDLE_MODE_TEXT  12
#define   ATC_SEND_DTMF                        13
#define ATC_BACK_MOVE                 95
#define ATC_BEYOND_ABILITIE           96
#define ATC_UNABLE_PROCESS            97
#define ATC_NO_RESPONSE               98
#define ATC_END_SESSION               99
#define ATC_STK_INVALID               0xFFFF

//The following is status of stk.
#define ATC_STATUS_SETUPMENU                0
#define ATC_STATUS_DISPLAYTEXT              1
#define ATC_STATUS_GETINKEY                 2
#define ATC_STATUS_GETINPUT                 3
#define ATC_STATUS_SETUPCALL                4
#define ATC_STATUS_PLAYTONE                 5//@hanjun.liu
#define ATC_STATUS_SELECTITEM               6
#define ATC_STATUS_REFRESH                7
#define ATC_STATUS_SENDSS                   8//@hanjun.liu
#define ATC_STATUS_SENDSMS                  9
#define ATC_STATUS_SENDUSSD                 10//@hanjun.liu
#define   ATC_STATUS_SETUPEVENTLIST         11
#define   ATC_STATUS_SETUPIDLEMODETEXT      12
#define   ATC_STATUS_SENDDTMF               13
#define   ATC_STATUS_SENDDATA               15
#define   ATC_STATUS_RECEIVEDATA            16
#define   ATC_STATUS_LAUNCHBROWSER          17
#define   ATC_STATUS_OPENCHANNEL            18
#define   ATC_STATUS_CLOSECHANNEL           19
#define   ATC_STATUS_GETCHANNELSTATUS       20
#define   ATC_STATUS_LANGNOTIFY       21
#define   ATC_STATUS_PROVIDELOCALINFO       22

#define ATC_STATUS_INVAKID_CMD                   0xff

//The following is event download type
#define ATC_EVENT_USERACT                           0
#define ATC_EVENT_IDLESCRAVA                      1
#define ATC_EVENT_BROWSERTERMINATION    2
#define ATC_EVENT_LANGSEL                           3
//The following is result value in setupmenu
#define ATC_RESULT_SELECTITEM               1
#define ATC_RESULT_HELPINFO                 2

//The following is result value in select item
#define ATC_ST_USERBORT                     0
#define ATC_ST_SELECTITEM                   1
#define ATC_ST_HELPINFO                     2
#define ATC_ST_RETURNBACK                   3

#define ATC_DATASTR_LENGTH                  4

#define STK_MAX_INFO_STR_LEN                255   //20
#define STK_MAX_MENU_ITEM_NUM               20
#define STK_MAX_PROACT_CMD_LEN           500
#define STK_MAX_STR_LEN                     255
#define STK_MAX_UCS2_STR_LEN                280
#define STK_MAX_ASCII_STR_LEN               140

//Define the length of string which user input at get inkey.
#define STK_GETINKEY_CNF_ASCI_LEN           1
#define STK_GETINKEY_CNF_UCS2_LEN           2

//Define the length of terminal profile.
#define STK_PROFILE_RESULT_LEN          40
#define STK_PROFILE_CHAR_LEN            30
//The following is command stag
#define ATC_CMD_INIT                     0
#define ATC_CMD_STGI                     1
#define ATC_CMD_STIN                     2
#define ATC_CMD_STGR                     3

//The following is result value of get inkey and get input.
#define ATC_USER_END_SSESSION            0
#define ATC_USER_RESPONSE                1

//The following is result value in setup call
#define ATC_REFUSE_CALL                     0
#define ATC_ACCEPT_CALL                     1

//The following is number of prompting string
#define AT_STK_STR_SENDSMS                  0
#define AT_STK_STR_SENDSS                   1
#define AT_STK_STR_SENDUSSD                 2
#define AT_STK_STR_MAX                      3

//The following is number of prompting string
#define AT_STK_TRP_MIN_VAL                  0
#define AT_STK_TRP_MAX_VAL                  1
#define AT_STK_TRP_NUM_MAX                  2

#define AT_STK_DTMF_PAUSE_TIME              3000
#define AT_STK_TELE_NUM_MAX_LEN             80

#define AT_DIALBCD_0                 0x0
#define AT_DIALBCD_1                 0x1
#define AT_DIALBCD_2                 0x2
#define AT_DIALBCD_3                 0x3
#define AT_DIALBCD_4                 0x4
#define AT_DIALBCD_5                 0x5
#define AT_DIALBCD_6                 0x6
#define AT_DIALBCD_7                 0x7
#define AT_DIALBCD_8                 0x8
#define AT_DIALBCD_9                 0x9
#define AT_DIALBCD_STAR                 0xA
#define AT_DIALBCD_HASH                 0xB
#define AT_DIALBCD_PAUSE                0xC
#define AT_DIALBCD_WILD                 0xD
#define AT_DIALBCD_EXPANSION            0xE
#define AT_DIALBCD_FILLER               0xF

#define AT_STK_MAX_REDIAL_DURATION 180000 //180s =3min,to prevent the too long redial

enum
{
    STK_DEFAULT_ALPHABET   = 0x00,         // handle as unpacked format in SMS default alphabet
    STK_PACKED_FORMAT      = 0xF4,         // packed format in SMS default alphabet
    STK_UNPACKED_FORMAT    = 0x04,        // unpacked format in SMS default alphabet
    STK_UCS2_ALPHABET      = 0x08         // UCS2(16bit)
};

typedef enum
{
    ST_UICC_AS_SIMCARD_PARAM   = 0,
    ST_TERMINAL_PROFILE_BEF_REFRESH_PARAM,
    ST_FETCH_BEF_ME_CAP_CHK_PARAM,
    ST_UNKNOW_PARAM
} ATC_CONFIG_ST_PARAM_TYPE_E;

typedef enum
{
    ST_ACTION_ACTIVE_PS = 0,
    ST_ACTION_POWERON_SIM,
    ST_ACTION_SET_EXT_PROFILE,
    ST_ACTION_TRIG_READ_AUTH_FILE
} ATC_STK_ACTION_TYPE_E;

typedef enum
{
    ATC_STK_CALL_SUCC,
    ATC_STK_CALL_FAIL_DISC,
    ATC_STK_CALL_FAIL_ERROR     
}ATC_STK_SETUP_CALL_FAIL_TYPE_E;

typedef struct atc_stk_param_string
{
    uint8 cmd_num;
    const uint8 *str_ptr;
} ATC_STK_STRING_T;

#ifdef ONTIM_PRJ
typedef enum
{
    ATC_SIM_REFRESH_FILE_UPDATE = 0,
    ATC_SIM_REFRESH_INIT = 1,
    ATC_SIM_REFRESH_RESET = 2,
    ATC_SIM_REFRESH_UNKOWN
} ATC_REFRESH_TYPE_E;

typedef struct
{
    BOOLEAN is_files_exist; //is the file list object exist in the command
    SIMAT_REFRESH_TYPE_E refresh_type;  //the refresh command type.
    SIMAT_REFRESH_FILE_LIST_T    file_list;  //the file list object,exist when the refresh type is 
}ATC_REFRESH_FILE_T;
#endif

/**---------------------------------------------------------------------------*
**                         Struct declaration                                     *
**---------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN              is_support_stk;       //is SIM card support STK
    uint8                cur_status;           //current signal
    uint8                menu_item_num;        //the number of menu item
    SIMAT_SETUP_MENU_T   setup_menu;   //data about setup menu signal
    SIMAT_DO_ITEM_ID     menu_item_id[STK_MAX_MENU_ITEM_NUM];//id of every menu item

    union
    {
        SIMAT_DISPLAY_TEXT_T display_text; //data about display text signal
        SIMAT_GET_INKEY_T    get_inkey;    //data about get inkey signal
        SIMAT_GET_INPUT_T    get_input;    //data about get input signal
        SIMAT_SETUP_CALL_T   setup_call;   //data about about setup call signal
        SIMAT_SELECT_ITEM_T  select_item;  //data about select item signal
        SIMAT_SEND_SM_T      send_sms;     //data about send SMS signal

        SIMAT_SEND_SS_T      send_ss;      //data about send SS signal
        SIMAT_SEND_USSD_T    send_ussd;    //data about send USSD signal
        SIMAT_PLAY_TONE_T    play_tone;    //data about play tone signal
        SIMAT_SETUP_EVENT_LIST_T   setup_event_list;//data about setup event list signal
        SIMAT_SETUP_IDLE_TEXT_T     setup_idle_text;//data about setup idle mode text
        SIMAT_SEND_DTMF_T              send_dtmf;//data about send dtmf signal
        SIMAT_REFRESH_T      refresh;    //data about refresh signal
        SIMAT_SEND_DATA_T                   send_data;      //data about send data signal
        SIMAT_RECEIVE_DATA_T                receive_data;   //data about receive data signal
        SIMAT_LAUNCH_BROWSER_T              launch_browser; //data about launch browser signal
        SIMAT_OPEN_CHANNEL_RELATED_GPRS_T   open_channel;   //data about open channel signal
        SIMAT_CLOSE_CHANNEL_T               close_channel;  //data about close channel signal
        SIMAT_LOCAL_INFO_TYPE_E             pro_local_info;
    } item;
}
STK_SINAL_T;

typedef struct
{
    union
    {
        APP_MN_SIMAT_DISPLAY_TEXT_IND_T display_text;
        APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T setup_idle_text;
        APP_MN_SIMAT_SETUP_MENU_IND_T setup_menu;
    } signal;
} STK_SIGNAL_CACHE_T;

#define STK_MAX_CACHE_SIGNAL_NUM 15

typedef struct
{
    int read;
    int write;
    int size;
    STK_SIGNAL_CACHE_T sig_list[STK_MAX_CACHE_SIGNAL_NUM];
} STK_SIGNAL_CACHE_LIST_T;


typedef struct
{
    uint8               call_index; //the call index to sent dtmf
    BOOLEAN             is_dtmf;    //if there is dtmf
    uint8               dtmf_len;   //dtmf string len
    uint8               dtmf_index; //to be sent index
    uint8               dtmf_str[AT_STK_TELE_NUM_MAX_LEN]; //dtmf string
    MN_CALLED_NUMBER_T  call_num;   //call number
}ATC_STK_CALL_INFO_T;

typedef struct
{
    BOOLEAN     is_need_redial_type;            //whether the setup call type is need redial type
    BOOLEAN     is_first_call_attempt_finished; //whether the first call setup attempt finished
    BOOLEAN     is_max_redial_duration_exp;     //whether the max redial duration timer expired
    BOOLEAN     is_redial_ongoing;              //whether the redial is ongoing
    BOOLEAN     is_self_disc_for_exp;           //the redial call is disconnect for max redial duration is expired
    uint8       curr_redial_cnt;                //current redial cnt
    uint8       max_redial_cnt;                 //max redial cnt
    uint16      raw_apdu_len;                   //raw apdu len
    uint8*      raw_apdu_ptr;                   //raw apdu ptr
    MN_CALL_DISCONNECT_CALL_CAUSE_E fail_cause; //call fail cause
}ATC_STK_REDIAL_INFO_T;

/*****************************************************************************/
//  Description : ATC_InitSimStkVar
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
extern void ATC_InitSimStkVar(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : This function initializes the static variables of phonebook
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void ATC_InitStkVar(
    ATC_CONFIG_T        *atc_config_ptr
);
/*****************************************************************************/
//  Description : This function initializes the static variables of phonebook
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void ATC_StkSimOff(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : This function check and handle what user input for terminal
//                profile
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_CheckAndHandleConfig(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T   *atc_config_ptr,
    char *config_char_ptr
);

/*****************************************************************************/
//  Description : This function handles the STK signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSTKSignal(
    ATC_CONFIG_T             *atc_config_ptr,
    xSignalHeaderRec *sig_ptr
);
/*****************************************************************************/
//  Description : This function handle the call signals
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void  ATC_StkProcessCallSignals(
    BOOLEAN IsCallSucc,
    MN_DUAL_SYS_E dual_sys,
    uint8   call_index,
    ATC_STK_SETUP_CALL_FAIL_TYPE_E  fail_type,
    MN_CALL_DISCONNECT_CALL_CAUSE_E fail_cause
);

/*****************************************************************************/
//  Description : This function handles the AT^STSF command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STSF);

/*****************************************************************************/
//  Description : This function handles the AT^STIN command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STIN);

/*****************************************************************************/
//  Description : This function handles the AT^STGI command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STGI);
/*****************************************************************************/
//  Description : This function handles the AT+ESATCALLSETUP command
//  Global resource dependence : g_rsp_str
//  Author:       bin.li
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessESATCALLSETUP);
/*****************************************************************************/
//  Description : This function handles the data send from the SIO in get input
//                 and get inkey.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern uint32 ATC_ProcessSioStkData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr  // pointer to the given pdu buffer
);

/*****************************************************************************/
//  Description : This function handles the AT^STGR command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STGR);

/*****************************************************************************/
//  Description : This function handles the AT+STED command
//  Global resource dependence : g_rsp_str
//  Author:       WangHui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSTED);
/*****************************************************************************/
//  Description : This function handles the AT+ESATCAPREQ command
//  Global resource dependence : g_rsp_str
//  Author:       binli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessESATCAPREQ);

/*****************************************************************************/
//  Description : This function handle the ATC_STSF_EXP_IND signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessTimerEXp (
    ATC_CONFIG_T       *atc_config_ptr
);
/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_STKProcessSendSmsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_SEND_SMS_CNF_T    *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:       binli
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_STKProcessSendUssdCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_USSD_SERVICE_CNF_T    *sig_ptr
);
/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:       binli
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_STKProcessSendSsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    ATC_SS_SERVICE_CNF_TYPE_E service_cnf_type,
    void    *sig_ptr
);
/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:       binli
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_STKProcessSendSsRegPwdCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_REGISTER_PASSWORD_CNF_T    *sig_ptr
);


/*****************************************************************************/
//  Description : This function handles the AT+ESATPROFILE command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessESATPROFILE);

/*****************************************************************************/
//  Description : This function handles the AT+ESATENVECMD command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessESATENVECMD);

/*****************************************************************************/
//  Description : This function handles the AT+ESATTERMINAL command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessESATTERMINAL);

/*****************************************************************************/
//  Description : This function handles the AT+SPSTPARAM command
//  Global resource dependence : g_rsp_str
//  Author: minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSTPARAM);

/*****************************************************************************/
//  Description : This function handles the AT+SPSTACTION command.
//                   This command is used to execute encryption card operation.
//  Global resource dependence : g_rsp_str
//  Author:       minqian.qian
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSTACTION);

/*****************************************************************************/
//  Description : This function handles the AT+SPUSATPROFILE command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUSATPROFILE );

/*****************************************************************************/
//  Description : This function handles the AT+SPUSATENVECMD command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUSATENVECMD );

/*****************************************************************************/
//  Description : This function handles the AT+SPUSATTERMINAL command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUSATTERMINAL );

/*****************************************************************************/
//  Description : This function handles the AT+SPSTPARAM command
//  Global resource dependence : g_rsp_str
//  Author: minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPSTPARAM );

/*****************************************************************************/
//  Description : This function handles the AT+SPSTACTION command.
//                   This command is used to execute encryption card operation.
//  Global resource dependence : g_rsp_str
//  Author:       minqian.qian
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPSTACTION );

/*****************************************************************************/
//  Description : This function handles the AT+ESATCAPREQ command
//  Global resource dependence : g_rsp_str
//  Author:       binli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUSATCAPREQ );

/*****************************************************************************/
//  Description : This function handles the AT+ESATCALLSETUP command
//  Global resource dependence : g_rsp_str
//  Author:       bin.li
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUSATCALLSETUP );

/**********************************************************************
//  Description:
//     stop the dtmf timer when call disconnected
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
extern void ATC_StkStopDtmfTimer(MN_DUAL_SYS_E dual_sys,uint8 call_index);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
