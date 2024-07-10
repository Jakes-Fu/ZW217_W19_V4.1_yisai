/******************************************************************************
 ** File Name:      atc_gsm_ss.h                                              *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           10/09/2002                                                *
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
 ** 10/09/2002     Xueli.lv         Create.                                   *
 ** 07/15/2003     Liuhan           Modify                                    *
 ******************************************************************************/
#ifndef _ATC_GSM_SS_H
#define _ATC_GSM_SS_H

#include "atc.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*****************************************************************************
**                    macro variables
******************************************************************************/
#define AT_CLCK_PSW_LEN         0X04

#define AT_CLCK_VOICE           0X01
//@Shijun.cui CR22510
#define AT_PSW_MAX_LEN          0X08
#define AT_PSW_MIN_LEN          0X04
//end CR22510

#define AT_CUSD_DISABLE         0X0
#define AT_CUSD_ENABLE          0X01
#define AT_CUSD_CANCEL          0X02

#define AT_CUSD_SMSALPHABET     0X0
#define AT_CUSD_UCS2ALPHABET    0X01

#define AT_STATUS_ACTIVE        1
#define AT_STATUS_NOT_ACTIVE    0

#define AT_CUSD_NOSTEPMORE      0X0
#define AT_CUSD_STEPMORE        0X01
#define AT_CUSD_CANCELUSSD      0X02
#define AT_CUSD_NOSUPPORT       0X04

#define ATC_SS_PWD_LEN          4   //the max length of the password
#define ATC_SS_CODE_LEN         6

#define ATC_SS_MAX_NO_REPLY_TIME                30
#define ATC_SS_MIN_NO_REPLY_TIME                5
/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/

typedef enum
{
    ATC_UNCONDITIONAL,
    ATC_MOBILE_BUSY,
    ATC_NO_REPLY,
    ATC_NOT_REACHABLE,
    ATC_ALL_CALL_FORWARDED,
    ATC_ALL_CONDITIONAL_FORWARDED
} ATC_CCFC_REASON_E;

typedef enum
{
    ATC_DISABLE,
    ATC_ENABLE,
    ATC_QUERY_STATUS,
    ATC_REGISTRATION,
    ATC_ERASURE
} ATC_OPERATION_MODE_E;

typedef enum
{
    ATC_VOICE = 0x01,              // voice service
    ATC_DATA = 0x02,               // data service
    ATC_FAX = 0x04,                // fax service
    ATC_ALL_CLASSES = 0xF7       // all the tele service except SMS 
} ATC_CLASS_E;

typedef struct
{
    int16          cmd_id;
} ATC_SS_CONTEXT_T;

typedef enum
{
    ATC_CLCK_DISABLE,
    ATC_CKCK_ENABLE,
    ATC_CLCK_QUERY_STATUS
} ATC_CLCK_OPERATION_MODE_E;

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function initializes the static variables for SS module
//  Global resource dependence : 
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern void ATC_InitSsVar( void );
#endif

/*****************************************************************************/
//  Description : This function handle the AT+CLIP command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:  Modify by liuhan       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLIP );

/*****************************************************************************/
//  Description : This function handle the AT+CCFC command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note: Modify by liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCFC );

/*****************************************************************************/
//  Description : This function handle the AT+CCWA command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note: Modify by liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCWA );

/*****************************************************************************/
//  Description : This function handle the AT+CLCK command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLCK );

/*****************************************************************************/
//  Description : This function handle the AT+CPWD command
//  This command is used to change the password
//            AT+CPWD=<fac>,<oldpin>,<newpin> 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPWD );        

/*****************************************************************************/
//  Description : This function handle the AT+COLP command 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCOLP );

/*****************************************************************************/
//  Description : This function handle the AT+CAOC command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCAOC );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_GET_PASSWORD_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessGetPswInd(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_GET_PASSWORD_IND_T      *sig_ptr
                      );

/*****************************************************************************/
//  Description : This function handles the APP_MN_REGISTER_PASSWORD_CNF signal 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:         
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPwdCnf(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_REGISTER_PASSWORD_CNF_T      *sig_ptr
                      );

/*****************************************************************************/
//  Description : This function handles the APP_MN_RELEASE_SS_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRelCnf(
                    ATC_CONFIG_T               *atc_config_ptr,
                    APP_MN_RELEASE_SS_IND_T    *sig_ptr);

/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_CHV_FUNC_CNF signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChvCnf(
                  ATC_CONFIG_T                   *atc_config_ptr,
                  APP_MN_SIM_PIN_FUNC_CNF_T      *sig_ptr
                  );
#endif

/*****************************************************************************/
//  Description : This function handle the AT+CHLD command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCHLD );

/*****************************************************************************/
//  Description : This function handle the AT+CLCC command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLCC );

/*****************************************************************************/
//  Description : This function handle the AT+CCUG command 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCUG );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_FORWARD_IND signal and 
//                sends the +CSSI: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessForwardInd(
            ATC_CONFIG_T              *atc_config_ptr,
            APP_MN_CALL_FORWARD_IND_T *sig_ptr
            );

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_BARRED_IND signal and 
//                sends the +CSSI: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessBarredInd(
                ATC_CONFIG_T              *atc_config_ptr,
                APP_MN_CALL_BARRED_IND_T  *sig_ptr
                );

/*****************************************************************************/
//  Description : This function handles the APP_MN_FORWARD_STATUS_IND signal and 
//                sends the +CSSI: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessForwardStatusInd(
                        ATC_CONFIG_T                   *atc_config_ptr,
                        APP_MN_FORWARD_STATUS_IND_T    *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_HELD_IND signal and 
//                sends the +CSSU: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessHeldInd(
                  ATC_CONFIG_T              *atc_config_ptr,
                  APP_MN_CALL_HELD_IND_T    *sig_ptr
                  );

 /*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_HELD_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessHeldCnf(
                    ATC_CONFIG_T            *atc_config_ptr,
                    APP_MN_CALL_HELD_CNF_T  *sig_ptr
                    );

/*****************************************************************************/
//  Description : This function handle the APP_MN_SPLIT_MPTY_CNF signal 
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessSplitMptyCnf(
                    ATC_CONFIG_T            *atc_config_ptr,
                    APP_MN_SPLIT_MPTY_CNF_T  *sig_ptr
                    );

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_RETRIEVED_IND signal and 
//                sends the +CSSU: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRetrievedInd(
                    ATC_CONFIG_T                *atc_config_ptr,
                    APP_MN_CALL_RETRIEVED_IND_T *sig_ptr
                    );

/*****************************************************************************/
//  Description : This function handles the APP_MN_BUILD_MPTY_IND signal and 
//                sends the +CSSU: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessBuildMptyInd(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_BUILD_MPTY_IND_T  *sig_ptr
                );

/*****************************************************************************/
//  Description : This function handles the APP_MN_REGISTER_SS_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRegisterCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_REGISTER_SS_CNF_T *sig_ptr
                );

/*****************************************************************************/
//  Description : This function handles the APP_MN_INTERROGATE_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessInterCnf(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_INTERROGATE_CNF_T *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handles the APP_MN_DEACTIVATE_SS_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDeactCnf(
                       ATC_CONFIG_T               *atc_config_ptr,
                       APP_MN_DEACTIVATE_SS_CNF_T *sig_ptr
                       );

/*****************************************************************************/
//  Description : This function handles the APP_MN_ACTIVATE_SS_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessActCnf(
                        ATC_CONFIG_T                *atc_config_ptr,
                        APP_MN_ACTIVATE_SS_CNF_T    *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handles the APP_MN_ACTIVATE_SS_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessEraseCnf(
                        ATC_CONFIG_T              *atc_config_ptr,
                        APP_MN_ERASE_SS_CNF_T     *sig_ptr
                        );
#endif

/*****************************************************************************/
//  Description : This function handles the APP_MN_RELEASE_SS_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRelCnf(
                    ATC_CONFIG_T               *atc_config_ptr,
                    APP_MN_RELEASE_SS_IND_T    *sig_ptr);

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_PUCT_INFO_CNF signal 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPuctCnf(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_PUCT_INFO_CNF_T *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_IND 
//                       signal to response the function 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUssdInd(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_USSD_IND_T *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_NOTIFY_IND  signal   
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUssdNotifyInd(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_USSD_NOTIFY_IND_T *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_SERVICE_CNF signal 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUssdServiceCnf(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_USSD_SERVICE_CNF_T *sig_ptr
                        );
#endif

/*****************************************************************************/
//  Description : This function handles the APP_MN_SETUP_COMPLETE_IND,
//  APP_MN_SETUP_COMPLETE_CNF signal to response the function 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/

extern ATC_STATUS ATC_ProcessSetupCompleteSignal(
                        ATC_CONFIG_T             *atc_config_ptr,
                        xSignalHeaderRec *sig_ptr
                        );
/*****************************************************************************/
//  Description : This function handles the APP_MN_GET_PASSWORD_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessGetPswInd(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_GET_PASSWORD_IND_T      *sig_ptr
                      );

/*****************************************************************************/
//  Description : This function handles the APP_MN_REGISTER_PASSWORD_CNF signal 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:         
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPwdCnf(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_REGISTER_PASSWORD_CNF_T      *sig_ptr
                      );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_BUILD_MPTY_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessBuildMptyCnf(
                        ATC_CONFIG_T              *atc_config_ptr,
                        APP_MN_BUILD_MPTY_CNF_T   *sig_ptr
                        );

/*****************************************************************************/
//  Description : This function handles the APP_MN_ECT_INVOKE_CNF signal
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessEctInvokeCnf(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_ECT_INVOKE_CNF_T   *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_RETRIEVED_CNF signal.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCallRetrievCnf(
                         ATC_CONFIG_T                  *atc_config_ptr,
                         APP_MN_CALL_RETRIEVED_CNF_T   *sig_ptr
                         );

/*****************************************************************************/
//  Description : This function checks if there is held calls
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN   CheckPresentHeldCall(
                ATC_CALLS_STATE_T   *call_state_ptr,
                uint8               *call_id
                );                                           

/*****************************************************************************/
//  Description : This function Release all held calls present
//  Global resource dependence : none
//    Author:       Elvis Xu
//  Note:
/*****************************************************************************/
BOOLEAN   RelPresentHeldCalls(
                            MN_DUAL_SYS_E dual_sys,
                ATC_CALLS_STATE_T   *call_state_ptr
                );
#endif

/*****************************************************************************/
//  Description : This function check if there is a waiting call
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN CheckPresentWaitCall(
                    ATC_CALLS_STATE_T   *call_state_ptr,
                    uint8               *call_id
                    );

/*****************************************************************************/
//  Description : This function set user_determined user busy for a waiting call
//  Global resource dependence : none
//    Author:       Elvis Xu
//  Note:
/*****************************************************************************/
BOOLEAN SetUserBusyForWaitCall(
                                   MN_DUAL_SYS_E dual_sys,
                    ATC_CALLS_STATE_T   *call_state_ptr
                    );
#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function check if there is an active call
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN CheckPresentActiveCall(
                    ATC_CALLS_STATE_T   *call_state_ptr,
                    uint8               *call_id
                    );                  

/*****************************************************************************/
//  Description : This function release all active calls present
//  Global resource dependence : none
//    Author:       Elvis Xu
//  Note:
/*****************************************************************************/
BOOLEAN RelPresentActiveCalls(
                                   MN_DUAL_SYS_E dual_sys,
                    ATC_CALLS_STATE_T   *call_state_ptr
                    );
#endif

/*****************************************************************************/
//  Description : This function handle the SS request through ATD command  
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_CheckSSStr(
            ATC_CONFIG_T   * atc_config_ptr,
            uint8          *str_ptr,
            uint8          length
            );

/*****************************************************************************/
//  Description : This function handle the AT+CUSD command 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCUSD );

/*****************************************************************************/
//  Description : This function handle the AT+CLIR command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note: Modify by liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLIR );

#if 0
/*****************************************************************************/
//  Description : This function handle the AT+CSSN command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSSN );

/*****************************************************************************/
//  Description : This function handle the AT+COLP command 
//  Global resource dependence : none
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCOLR );

/*****************************************************************************/
//  Description : This function handle the AT+CPUC command 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPUC );

/*****************************************************************************/
//  Description : This function handle the AT+CACM command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCACM );

/*****************************************************************************/
//  Description : This function handle the AT+CAMM command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCAMM );

/*****************************************************************************/
//  Description : This function handles the AT+SAC command, This command is used
//  to abort the current SS service of PLMN searching.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSAC );
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
