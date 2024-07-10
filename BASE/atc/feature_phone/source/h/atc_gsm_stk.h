/******************************************************************************
 ** File Name:      atc_gsm_STK.h                                             *
 ** Author:         Liuhan                                                    *
 ** DATE:           12/08/2003                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file declares the functions that handle the AT       *
 **                 command regarding SS(supplement service)                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/08/2003     Liuhan           Create.                                   *
******************************************************************************/
#if 0
#ifndef _ATC_GSM_STK_H
#define _ATC_GSM_STK_H

#include "sci_types.h"
#include "atc.h"
#include "simat_outport.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

/**---------------------------------------------------------------------------*
 **                         Constant declaration                              *
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
#define ATC_SEND_SS                   8//@hanjun.liu
#define ATC_SEND_SMS                  9
#define ATC_SEND_USSD                 10//@hanjun.liu
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
#define ATC_STATUS_SENDSS                   8//@hanjun.liu
#define ATC_STATUS_SENDSMS                  9
#define ATC_STATUS_SENDUSSD                 10//@hanjun.liu
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
#define STK_MAX_STR_LEN                     255
#define STK_MAX_UCS2_STR_LEN                280
#define STK_MAX_ASCII_STR_LEN               140

//Define the length of string which user input at get inkey.
#define STK_GETINKEY_CNF_ASCI_LEN           1
#define STK_GETINKEY_CNF_UCS2_LEN           2

//Define the length of terminal profile.
#define STK_PROFILE_RESULT_LEN          5
#define STK_PROFILE_CHAR_LEN            10

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

enum 
{
    STK_DEFAULT_ALPHABET   = 0x00,         // handle as unpacked format in SMS default alphabet
    STK_PACKED_FORMAT      = 0xF4,         // packed format in SMS default alphabet
    STK_UNPACKED_FORMAT    = 0x04,        // unpacked format in SMS default alphabet
    STK_UCS2_ALPHABET      = 0x08         // UCS2(16bit)    
};
 /**---------------------------------------------------------------------------*
 **                         Struct declaration                                     *
 **---------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN              is_support_stk;       //is SIM card support STK
    uint8                cur_status;           //current signal
    uint8                menu_item_num;        //the number of menu item
    SIMAT_DO_ITEM_ID     menu_item_id[STK_MAX_MENU_ITEM_NUM];//id of every menu item
    SIMAT_SETUP_MENU_T   setup_menu;   //data about setup menu signal
    
    union 
    {
        SIMAT_DISPLAY_TEXT_T display_text; //data about display text signal
        SIMAT_GET_INKEY_T    get_inkey;    //data about get inkey signal
        SIMAT_GET_INPUT_T    get_input;    //data about get input signal
        SIMAT_SETUP_CALL_T   setup_call;   //data about about setup call signal
        SIMAT_SELECT_ITEM_T  select_item;  //data about select item signal
        SIMAT_SEND_SM_T      send_sms;     //data about send SMS signal
        //@hanjun.liu
        SIMAT_SEND_SS_T      send_ss;      //data about send SS signal
        SIMAT_SEND_USSD_T    send_ussd;    //data about send USSD signal
        SIMAT_PLAY_TONE_T    play_tone;    //data about play tone signal
    } item;
}
STK_SINAL_T;
typedef struct
{
    uint8   mainmenu_item_num;
    uint8   title_str_len;
    uint8   mainmenu_title_string[STK_MAX_INFO_STR_LEN];
    uint8   item_str_len[STK_MAX_MENU_ITEM_NUM];
    uint8   mainmenu_item_string[STK_MAX_MENU_ITEM_NUM][STK_MAX_INFO_STR_LEN];
    SIMAT_DO_ITEM_ID     mainmenu_item_id[STK_MAX_MENU_ITEM_NUM];
}
STK_MANIMENU_T;

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
//  Description : This function check and handle what user input for terminal
//                profile 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_CheckAndHandleConfig(
                                        ATC_CONFIG_T   *atc_config_ptr,
                                        char *config_char_ptr
                                       );

#ifndef _ULTRA_LOW_CODE_
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
                                       MN_DUAL_SYS_E  dual_sys,
                                       BOOLEAN IsCallSucc
                                       );
#endif

/*****************************************************************************/
//  Description : This function handles the AT+STSF command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTSF );

/*****************************************************************************/
//  Description : This function handles the AT+STIN command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTIN );

/*****************************************************************************/
//  Description : This function handles the AT+STSF command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTGI );
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
//  Description : This function handles the AT+STSF command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTGR );

/*****************************************************************************/
//  Description : This function handles the AT+PSSTKE command.This command is used
//  as STK Envelope Cmd. transparent display mode.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKE);

/*****************************************************************************/
//  Description : This function handles the AT+PSSTKR command.This command is used
//  as STK Response Cmd. transparent display mode.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKR);

/*****************************************************************************/
//  Description : This function handles the AT+PSSTKI command.This command is used
//  as STK setup menu display Cmd. transparent display mode.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKI);

/*****************************************************************************/
//  Description : This function handles the AT+STKREFRESH command.This command is used
//  refresh STK sim files etc.
//  Global resource dependence : 
//  Author:      Elvis Xu 
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTKREFRESH);

/*****************************************************************************/
//  Description : This function handles the AT+PSSTKSMS command, which used to send sms under 
//                      transparent display mode
//  Global resource dependence : 
//  Author:       Elvis Xu
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKSMS);

#ifndef _ULTRA_LOW_CODE_
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
//  Description : This function handles the APP_MN_SIMAT_REFRESH_CNF message and send
//  info to APP.
//  Global resource dependence : none
//  Author:      Elvis Xu 
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSIMRefreshCnf(
                ATC_CONFIG_T    *atc_config_ptr,
                APP_MN_SIMAT_REFRESH_CNF_T    *sig_ptr
                );

/*****************************************************************************/
//  Description : This function handle the STK SMS received from SIO
//  Global resource dependence : 
//  Author:       Elvis Xu
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSioStkSmsData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *pdu_ptr  // pointer to the given pdu buffer
                );
#endif
/*****************************************************************************/
//  Description : This function handles the STK signal 
//  Global resource dependence : none
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSIMATFwdInd(
                        ATC_CONFIG_T       *atc_config_ptr,
                        xSignalHeaderRec   *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handle the call signals 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void  ATC_StkProcessCallSignals(
                                       MN_DUAL_SYS_E  dual_sys,
                                       BOOLEAN IsCallSucc
                                     );
                                                                                                                            
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif
