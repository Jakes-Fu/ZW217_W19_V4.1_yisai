/******************************************************************************
 ** File Name:      atc_gsm.h                                                 *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           12/08/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/08/2002     Xueli.lv         Create.                                   *
 ** 15/07/2003     Liuhan           Modify.                                   *
 ******************************************************************************/
#ifndef _ATC_GSM_H
#define _ATC_GSM_H

#include "atc.h"
//@Ivan.yin add for ATD*99#
#include "atc_plus_gprs.h"

/*******************************************************************************
 **                              Macro declaration                             *
 ******************************************************************************/
#define ATC_CLIP_INTERVAL           5*1000 // 5 seconds

#define ATC_COPS_UNKNOWN_STATE      0
#define ATC_COPS_AVAILABLE_STATE    1
#define ATC_COPS_CURRENT_STATE      2
#define ATC_COPS_FORBIDDEN_STATE    3

#define ATC_MAX_OPER_LEN            0X20
#define ATC_MAX_TEL_LENGTH          40 

#define ATC_PLMN_LEN            3
#define ATC_MAX_FULL_OP_NAME    32
#define ATC_MAX_SHORT_OP_NAME   12
#define ATC_MAX_COUNTRY_NAME    20

/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/
typedef   int8      OPER_STRING_T[ATC_MAX_OPER_LEN];
typedef   int8      TEL_STRING_T[ATC_MAX_TEL_LENGTH];

typedef struct
{
    BOOLEAN  param_used[6];
    uint32 pdp_id[MAX_PDP_CONTEXT_COUNT];
} ATC_GPRS_DIAL_REQ_T;

typedef struct
{
    BOOLEAN             calling_num_present;
    MN_CALLING_NUMBER_T calling_num;
} ATC_CALLING_NUM_T;


typedef enum
{
    PLMN_FORMAT_FULL_NAME = 0x00,
    PLMN_FORMAT_SHORT_NAME,
    PLMN_FORMAT_NUMBERIC
} ATC_PLMN_FORMAT_E;

typedef enum
{
    PLMN_TYPE_FULL_NAME = PLMN_FORMAT_FULL_NAME,
    PLMN_TYPE_SHORT_NAME = PLMN_FORMAT_SHORT_NAME,
    PLMN_TYPE_NUMBERIC = PLMN_FORMAT_NUMBERIC,
    PLMN_TYPE_MNC_MCC,
    PLMN_TYPE_MAX
} ATC_PLMN_TYPE_E;

typedef struct
{
    unsigned char  PLMNNumber_data[ATC_PLMN_LEN];                   /* PLMN number code */
    unsigned char  full_operator_name[ATC_MAX_FULL_OP_NAME+1];      /* full operator name */
    unsigned char  short_operator_name[ATC_MAX_SHORT_OP_NAME+1];    /* short operator name */
    unsigned char  country_name[ATC_MAX_COUNTRY_NAME+1];            /* country name */
    unsigned short mcc_data;                                        /* mcc value */
    unsigned short mnc_data;                                        /* mnc value */
} ATC_PLMN_DETAILED_INFO_T;

/*****************************************************************************/
//  Description : This function handle the ATA command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessA(
                ATC_CONFIG_T        * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATD command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessD( // return the handle result
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the AT+SSMP command, This command is used
//                to send the singal with max power. 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSMP );

/*****************************************************************************/
//  Description : This function handle the AT+CMUX command 
//  Global resource dependence : none
//  Author:      Wang hui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMUX );

/*****************************************************************************/
//  Description : This function handle the AT+SCMUX command 
//  Global resource dependence : none
//  Author:      Feng.shi
//  Note: param1=0 means close mux, =3 means open mux and link 0 is sim1 AT, 
//        link 1 is sim2 AT, link 2 is ppp link.
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMUX );

/*****************************************************************************/
//  Description : This function handles the AT+CMOD command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMOD);

/*****************************************************************************/
//  Description : This function handles the AT+CHUP command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCHUP);

/*****************************************************************************/
//  Description : This function handle the AT+VTS command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessVTS( // return the handle result
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the APP_MN_SETUP_IND signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSetupInd(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_SETUP_IND_T  *sig_ptr         // point to the APP_MN_SETUP_IND signal
                );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handle the APP_MN_SETUP_COMPLETE_IND signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSetupCompleteInd(
                ATC_CONFIG_T                 *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_SETUP_COMPLETE_IND_T  *sig_ptr
                );
#endif

/*****************************************************************************/
//  Description : This function handle the APP_MN_ALERTING_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
 extern ATC_STATUS ATC_ProcessAlertingInd(
               ATC_CONFIG_T            *atc_config_ptr, 
               APP_MN_ALERTING_IND_T   *sig_ptr
               );

/*****************************************************************************/
//  Description : This function handle the ATH command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessH(
                ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T   *cur_cmd_info
                );
/*****************************************************************************/
//  Description : This function handle the AT+COPS command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCOPS(
                ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T   *cur_cmd_info
                );

/*****************************************************************************/
//  Description : This function handle the ATC_CALL_EXP_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessCallExpInd (
                ATC_CONFIG_T            *atc_config_ptr
                );
/*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_START_IND signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCallStartInd(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_CALL_START_IND_T  *sig_ptr         // point to the APP_MN_CALL_START_IND signal
                );
                
/*****************************************************************************/
//  Description : This function handles the AT+CEER. This command is used to 
//  get the cause of call release when the last call set up.
//  Global resource dependence : none
//  Author: Xueli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCEER);

/*****************************************************************************/
//  Description : This function handles the AT+SDMUT command.This command is used
//                to mute downlink of phone.
//  AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSDMUT );

/*****************************************************************************/
//  Description : This function handle the AT+SSMP command, This command is used
//                to send the singal with max power. 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSMP );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_PLMN_LIST_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPlmnListCnf(
                            ATC_CONFIG_T                   *atc_config_ptr,
                            APP_MN_PLMN_LIST_CNF_T         *sig_ptr
                            );

/*****************************************************************************/
//  Description : This function handles the APP_MN_PLMN_SELECT_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPlmnSelCnf(
                               ATC_CONFIG_T                   *atc_config_ptr,
                               APP_MN_PLMN_SELECT_CNF_T       *sig_ptr);    

/*****************************************************************************/
//  Description : This function checks the plmn string.
//  Global resource dependence : none
//  Author:      sunny
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckPlmnStr(
    uint8          length,
    uint8          *str
);
#endif

/*****************************************************************************/
//  Description : This function handle the ATH command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessH );    

/*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_HELD_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/

extern ATC_STATUS  ATC_ProcessCallDiscInd(
                    ATC_CONFIG_T            *atc_config_ptr,
                    xSignalHeaderRec        *sig_ptr
                    );          

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_ACMMAX_IND signal. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessAcmInd(
                  ATC_CONFIG_T          *atc_config_ptr
                  );            
#endif

#if 0
/*****************************************************************************/
//  Description : This function handle the AT+VTS command, This command is used
//                to define the tone duration. 
//  AT+VTD=<n>
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVTD );

/*****************************************************************************/
//  Description : This function handles the AT+CEER. This command is used to 
//  get the cause of call release when the last call set up.
//  Global resource dependence : none
//  Author: Xueli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCEER);

/*****************************************************************************/
//  Description : This function handle the AT+S32K command, This command is used
//                to set the sleep mode. 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS32K );
#endif
#endif
