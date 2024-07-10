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
 ** 12/10/2008     Pizer.Fan        Add +TRACE,+OFF,+RESET.                   *
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
#define ATC_MAX_FULL_OP_NAME    50 //32
#define ATC_MAX_SHORT_OP_NAME   50 //14 //12 //604 1 ?
#define ATC_MAX_COUNTRY_NAME    20

#define ATC_MAX_DTMF_ENTRY 50

/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/
typedef   int8      OPER_STRING_T[ATC_MAX_OPER_LEN];
typedef   int8      TEL_STRING_T[ATC_MAX_TEL_LENGTH];

typedef struct
{
    BOOLEAN             calling_num_present;
    MN_CALLING_NUMBER_T calling_num;
} ATC_CALLING_NUM_T;

typedef struct
{
    BOOLEAN              called_num_present;
    BOOLEAN              is_buffer_call;  //only for multi-party call, to dial new call after previous one(s) held
 #ifdef _MUX_ENABLE_
    uint8                buffer_link_id;
 #endif
    MN_CALL_TYPE_E       call_type;
    MN_CALL_CLIR_E       call_clir;
    MN_CALLED_NUMBER_T   called_num;
} ATC_LAST_CALLED_NUM_T;

typedef enum
{
    ATC_CPOL_READ,
    ATC_CPOL_TEST
} ATC_CPOL_CMD_E;

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

typedef struct 
{
    MN_PLMN_T              cur_plmn;
    MN_NETWORK_NAME_T      full_name;
    MN_NETWORK_NAME_T      short_name;
} ATC_PLMN_INFO_T;

typedef struct
{
    uint16      duration;                   /* duration between two dtmf keys*/
    uint8       callid;
    uint8       count;
    uint8       link_id;
} ATC_DTMF_INFO_T;


typedef struct
{
    uint16 duration[ATC_MAX_DTMF_ENTRY];         /* duration between two dtmf keys*/
    uint8 read_index;
    uint8 count_of_cur_duration;
    BOOLEAN is_wait_stop_dtmf_cnf;  //for the dtmf timer
}ATC_DTMF_DURATION_T;

/*****************************************************************************/
//  Description : This function handle the ATA command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessA(
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
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
//  Description : This function handle the AT+VTS command, This command is used
//                to define the tone duration.
//  AT+VTD=<n>
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessVTD);

/*****************************************************************************/
//  Description : This function get the dtmf timer active or not
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_IsDTMFTimerActive(void);
/*****************************************************************************/
//  Description : This function reset dtmf timer
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ResetDTMFTimer(void);

/*****************************************************************************/
//  Description : This function reset the s_atc_dtmf variable
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
extern void ATC_ResetDTMFVar(MN_DUAL_SYS_E   dual_sys);

/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
extern void  ATC_ProcessDTMFTimerCallback(
    uint32 lparam
);
/*****************************************************************************/
//  Description : This function handles the ATC_CALL_DTMF_EXP_IND signal
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessDTMFTimerExpInd(
    ATC_CONFIG_T         *atc_config_ptr,
    ATC_DTMF_TIMER_EXP_IND_T *sig_ptr
);
/*****************************************************************************/
//  Description : This function handle APP_MN_START_DTMF_CNF
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessStartDTMFCnf(
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    APP_MN_START_DTMF_CNF_T  *sig_ptr    // point to the APP_MN_START_DTMF_CNF signal
);
/*****************************************************************************/
//  Description : This function handle APP_MN_STOP_DTMF_CNF
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessStopDTMFCnf(
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    APP_MN_STOP_DTMF_CNF_T  *sig_ptr    // point to the APP_MN_STOP_DTMF_CNF signal
);

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
//  Description : This function handle the AT+EVTS command
//  Global resource dependence : none
//  Author:       Yi Jin
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessEVTS( // return the handle result
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
//  Description : This function handle the AT+CPLS command
//  Global resource dependence : none
//  Author:       sunny
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCPLS(
    ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T   *cur_cmd_info
);

/*****************************************************************************/
//  Description : This function handle the AT+CPOL command
//  Global resource dependence : none
//  Author:       sunny
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCPOL(
    ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T   *cur_cmd_info
);
/*****************************************************************************/
//  Description : This function handle the ATC_CALL_EXP_IND signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessCallExpInd(
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
//  Description : This function handle the APP_MN_EMG_NUM_LIST_IND signal.
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
extern void ATC_ProcessEmgNumListInd(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    APP_MN_EMG_NUM_LIST_IND_T *sig_ptr        // point to the APP_MN_EMG_NUM_LIST_IND signal
);

/*****************************************************************************/
//  Description : This function handles the AT+CEER. This command is used to
//      get the cause of call release when the last call set up.
//  Global resource dependence : none
//  Author: Xueli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCEER);

/*****************************************************************************/
//  Description : This function handles the AT+SDMUT command.This command is used
//                to mute downlink of phone.
//      AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSDMUT);

/*****************************************************************************/
//  Description : This function handles the AT+CEER. This command is used to
//      get the cause of call release when the last call set up.
//  Global resource dependence : none
//  Author: Xueli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCEER);

/*****************************************************************************/
//  Description : This function handles the APP_MN_PREFER_PLMN_CHANGE_CNF at the
//                 AT+CPOL
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPreferPlmnChangCnf(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_PREFER_PLMN_CHANGE_CNF_T         *sig_ptr
);

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
//  Description : This function handle the ATH command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessH);

/*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_DISCONNECTED_IND or APP_MN_CALL_ERR_IND signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/

extern ATC_STATUS  ATC_ProcessCallDiscInd(
    ATC_CONFIG_T            *atc_config_ptr,
    xSignalHeaderRec        *sig_ptr
);


/*****************************************************************************/
//  Description : This function handles the APP_MN_ACMMAX_IND signal.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessAcmInd(
    ATC_CONFIG_T          *atc_config_ptr,
    APP_MN_ACMMAX_IND_T  *sig_ptr 
);

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handle the AT*SPNTRG command
//  Global resource dependence : none
//  Author:       peng.chen
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processstar_PSNTRG);

/*****************************************************************************/
//  Description : This function handle the AT+S32K command, This command is used
//                to set the sleep mode.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessS32K);

/*****************************************************************************/
//  Description : This function handles the AT+CBST command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCBST);

/*****************************************************************************/
//  Description : This function handles the AT+CMOD command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMOD);

/*****************************************************************************/
//  Description : This function handles the ATDL command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessDL);

/*****************************************************************************/
//  Description : This function handles the AT+CHUP command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCHUP);

/*****************************************************************************/
//  Description : This function handles the ATC+CSTA command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSTA);

/*****************************************************************************/
//  Description : This function handles the AT+CHUPVT command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCHUPVT);

/*****************************************************************************/
//  Description : This function handles the AT+TRACE command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessTRACE);

/*****************************************************************************/
//  Description : This function handles the AT+OFF command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessOFF);

/*****************************************************************************/
//  Description : This function handles the AT+RESET command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessRESET);

/*****************************************************************************/
//  Description : This function handle the AT+SPAUTO command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPAUTO);


/*****************************************************************************/
//  Description : This function handle the AT+SSMP command, This command is used
//                to send the singal with max power.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSSMP);

/*****************************************************************************/
//  Description : This function handle the AT+XLEMA command
//  Global resource dependence : none
//  Author:  vicky.wang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessXLEMA);

#endif

/*****************************************************************************/
//  Description : This function handle the AT+CMUX command
//  Global resource dependence : none
//  Author:      Wang hui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMUX);

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handle the AT+SCMUX command 
//  Global resource dependence : none
//  Author:      Feng.shi
//  Note: param1=0 means close mux, =3 means open mux and link 1 is sim1 AT, 
//        link 2 is sim2 AT,...,  link 5 is ppp link.
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMUX );
#endif

/*****************************************************************************/
//  Description : This function handle the AT+CMER command
//  Global resource dependence : g_rsp_str
//  Author:       peng.chen
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMER);

/*****************************************************************************/
//  Description : This function handle the AT+SMMSWAP command for swapping between single sim mux
//                      mode and multi sims mux mode
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSMMSWAP);

/*****************************************************************************/
//  Description : This function query plmn detailed info
//  Global resource dependence :
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
const ATC_PLMN_DETAILED_INFO_T *ATC_GetPlmnDetailedInfo(
    ATC_PLMN_TYPE_E QueryType,
    unsigned char  *QueryData);

/*****************************************************************************/
//  Description : This function restore the gsm  configs
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
extern void ATC_RestoreGsmConfig(
    ATC_CONFIG_T         *atc_config_ptr,
    MN_DUAL_SYS_E   dual_sys
);

/*****************************************************************************/
//  Description : This function handle the APP_MN_PROGRESS_IND signal
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:         
/*****************************************************************************/
ATC_STATUS ATC_ProcessProgressInd(
    ATC_CONFIG_T            *atc_config_ptr,
    APP_MN_PROGRESS_IND_T   *sig_ptr
);

/*****************************************************************************/
//  Description : This function handle the APP_MN_DISC_INFO_IND signal
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:         
/*****************************************************************************/
ATC_STATUS ATC_ProcessDiscInfoInd(
    ATC_CONFIG_T            *atc_config_ptr,
    APP_MN_DISC_INFO_IND_T   *sig_ptr
);

/*****************************************************************************/
//  Description : This function handle the APP_MN_DISCONNECTING_IND signal
//  Global resource dependence : none
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDisconnectingInd(
    ATC_CONFIG_T         *atc_config_ptr,
    APP_MN_DISCONNECTING_IND_T    *sig_ptr
);

extern void BuildPSNTRGResponse(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T   *atc_config_ptr ,
    const MN_PHONE_CURRENT_PLMN_INFO_T    *register_status_ptr
    );

/*****************************************************************************/
//  Description : This function checks the plmn string.
//  Global resource dependence : none
//  Author:      sunny
//  Note:
/*****************************************************************************/
extern BOOLEAN CheckPlmnStr(
    uint8          length,
    uint8          *str
);


#ifdef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function used to handle the response when GPRS service 
//                was suspended.
//  Global resource dependence : none
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
void ATC_HandleGprsSuspendRsp(ATC_CONFIG_T *atc_config_ptr);

#endif
#endif/*_ATC_GSM_H*/