/******************************************************************************
 ** File Name:      atc_gsm_sms.h                                             *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           24/09/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file declares the functions that handle the AT       *
 **                 command regarding SMS(short message service)              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/09/2002     Xueli.lv         Create.                                   *
 ******************************************************************************/

#ifndef _ATC_GSM_SMS_H
#define _ATC_GSM_SMS_H


#include "atc.h"

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
{
#endif

/*****************************************************************************
**                    macro variables
******************************************************************************/
#define  ATC_MIN_SM_LEN             0x08    //according to 3GPP TS 23.040/9.2.2.2
#define  ATC_MAX_SM_LEN             0XA5    //according to 3GPP TS 23.040/9.2.2.2

#define  MT_IND_DEFAULT_VAL         0x00
#define  BM_IND_DEFAULT_VAL         0x00
#define  DS_IND_DEFAULT_VAL         /* 0X00 */ 0x01  /* cr147624 default value is report cds*/
#define  BFR_IND_DEFAULT_VAL        0x00


#define  ATC_SMS_7BIT_LEN           /* 160 */320
#define  ATC_SMS_8BIT_LEN           /* 140 */280
#define  ATC_SMS_UCS2_LEN           /* 140 */280


#define ATC_SMS_MINUTES_VAL         143
#define ATC_SMS_HOURS_VAL           167
#define ATC_SMS_DAYS_VAL            196
#define ATC_SMS_WEEKS_VAL           255

// the AT+CSMP parameter
#define ATC_SMS_FO_DEFAULT_VAL      0X13
#define ATC_SMS_VP_DEFAULT_VAL      143
#define ATC_SMS_PID_DEFAULT_VAL     0

#define ATC_MESSAGE_SIZE            251

#define   AT_SMS_STORAGE_ME             0
#define   AT_SMS_STORAGE_SM             1
#define   AT_SMS_STORAGE_SR             2

#define   AT_RETURN_SESULT_CODE     3

#define  ATC_MAX_PDU_LENGTH         0xA0
#define  ATC_MAX_DIGITAL            41

#ifdef WIN32
#define  ATC_CMMS_TIMER_VALUE       10000
#else

#define  ATC_CMMS_TIMER_VALUE       5000 /*4000   */ /* 1-5 seconds */

#endif

#define  ATC_MAX_SMS_ADDRESS_LEN_FOR_TE   ((MN_MAX_ADDR_BCD_LEN*2)*4+1)
#define  ATC_MAX_SMS_DATA_LEN_FOR_TE   (MN_SMS_MAX_USER_VALID_DATA_LENGTH*4+1)

/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/
typedef enum
{
    ATC_SMS_UNREAD,    // The received message unread(new)
    ATC_SMS_READ,      // The received message read
    ATC_SMS_UNSENT,    // the stored message unsent
    ATC_SMS_SENT,      // the stored message sent
    ATC_SMS_ALL,        // all of the message
    ATC_SMS_MAX_STATUS
} ATC_SMS_STATE_E;

typedef enum
{
    ATC_DEL_INDEX,
    ATC_DEL_ALL_READ,
    ATC_DEL_ALL_READ_SENT,
    ATC_DEL_ALL_READ_SENT_UNSENT,
    ATC_DEL_ALL
} ATC_SMS_DEL_STATE_E;

enum
{
    ATC_MT_NO_DELIVER,
    ATC_MT_CMTI,
    ATC_MT_CMT_2,
    ATC_MT_CMT_3
};
typedef enum
{
    ATC_STORAGE_ME,     // The message memory storage is "ME"
    ATC_STORAGE_SM,     // The message memory storage is "SM"
    ATC_STORAGE_MAX
    //  ATC_SOTRAGE_TL      // The message memory storage is "Template"
} ATC_SMS_STORAGE_E;

typedef enum
{
    ATC_CMMS_DISABLE = 0x00,
    ATC_CMMS_KEEP_ENABLED,
    ATC_CMMS_ENABLED
} ATC_CMMS_VALUE;

typedef struct
{
    uint8               mode;    /* controls the processing of unsolicited result codes specified within this command
                                 ** the default setting is 3.
                                 */
    uint8               mt_ind;  /* sets the result code indication routing for SMS-DELIVERs,
                                 ** 0:No SMS-DELIVER indications are routed to the TE Default setting
                                 ** 1:using the +CMTI result code 3:using the +CMT result code.
                                 */
    uint8               bm_ind;  /* sets the result code indication routing for CBMs
                                 ** 0: Store message to ¡°BM¡±. No CBM indications are routed to the TE Default setting
                                 ** 2: New CBMs are routed directly to the TE, using the +CBM result code
                                 */
    uint8               ds_ind;  /* sets the result code indication routing for SMS-STATUS-REPORTs.
                                 ** 0£ºNo SMS-STATUS-REPORTs are routed to the TE Default setting
                                 ** 1£ºSMS-STATUS-REPORTs are routed to the TE, using the +CDS result code
                                 */
    uint8               bfr_ind;
    MN_SMS_STORAGE_E    mem1;    /* Memory from which messages are read and deleted
                                 ** the current version only supports "SM"
                                 */
    MN_SMS_STORAGE_E    mem2;    /* Memory to which writing and sending options are made (see AT+CMSS and AT+CMGW).
                                 ** The current version only supports "SM"
                                 */
    MN_SMS_STORAGE_E    mem3;    /* Memory to which received SMs are preferred to be stored
                                 ** The current version only supports "SM"
                                 */
    BOOLEAN             mo_flag; /* true: Mobile originated message is supported
                                 ** false: mobile originated message is not supported
                                 */
    BOOLEAN             mt_flag; /* true: mobile terminated message is supported
                                 ** false: mobile terminated message is not supported
                                 */
    BOOLEAN             bm_flag; /* true: the broadcast message is supported
                                 ** false: the broadcast message is not supported
                                 */
    BOOLEAN             msg_format; /* FALSE: PDU mode
                                    ** true: text mode
                                    */
    BOOLEAN             service_flag; /* FALSE: sms at commands are compatible with gsm07.05 phase 2 version
                                      ** TRUE:  sms at commands are compatible with gsm07.05 phase 2+ version
                                      */
    BOOLEAN             csdh_flag; /* FALSE: do not show header value
                                   ** TRUE:  show header value
                                   */
    int32               sm_len;
    BOOLEAN             suss_flag;
    ATC_CMMS_VALUE      cmms_value;
    BOOLEAN             cmms_flag;
    SCI_TIMER_PTR       cmms_timer;
} ATC_SMS_CONFIG_T;

typedef struct
{
    uint8               fo;    // the first octet of the TPDU
    uint8               vp;    // valid period
    uint8               dcs;   // data code schema
    uint8               pid;   // protocol indicator
} ATC_SMS_NV_CONFIG_T;


typedef  struct ATC_SMS_RECORD_tag
{
    int32                        index;
    BOOLEAN                      dest_num_flag;
    MN_CALLED_NUMBER_T           dest_num;
    uint8                        toda;
    uint16                       cmd_id;
    ATC_SMS_STATE_E              state;
    uint8                        length;
    MN_SMS_RECORD_ID_T           cmgl_idex;
    ATC_SMS_DEL_STATE_E          cmgd_state;
    BOOLEAN                      more_sms_flag;
} ATC_SMS_RECORD_T;


#define ATC_CNMA_TIMER_VALUE     15000    /* cnma timer value, the same as sms module SMS_TR2M_VAL,see block_rl_types.pr*/

typedef struct
{
    uint16 cnma_index;
    uint8   cnma_n;
    uint8   cnma_length;
    SCI_TIMER_PTR cnma_timer ;
    BOOLEAN   is_waiting_for_cnma;  /* 3gpp 27.005 3.4.4
                                                   If ME does not get acknowledgement within required time
                                                   (network timeout), ME should respond as specified in 3GPP TS 24.011
                                                   [6] to the network.  ME/TA shall automatically disable routing to TE
                                                   by setting both <mt> and <ds> values of +CNMI to zero.*/
} ATC_SMS_CNMA_T;

typedef struct
{
    BOOLEAN is_enable;
    MN_CB_MSG_ID_MUSTER_T  cb_msg_id;
}ATC_CB_MSG_ID_MUSTER_T;

extern ATC_SMS_CNMA_T          s_sms_cnma[MN_SYS_NUMBER];
extern void AtcCNMATimerExpired(
    uint32 lparam   //the function's input parameter
);

/*****************************************************************************/
//  Description : This function handles the AT+CMGL command
//                AT+CMGL=<type>
//             <type>       0         rec unread
//                          1         rec read
//                          2         sto unread
//                          3         sto read
//                          4         all message
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGL);

/*****************************************************************************/
//  Description : This function handles the AT+CSMS command.this command is used
//                to set message service
//                AT+CSMS=<service>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSMS);

/*****************************************************************************/
//  Description : This function handles the AT+CSDH command.this command is used
//                to give additional information on text mode result code.
//                AT+CSDH=<value>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSDH);

/*****************************************************************************/
//  Description : This function handles the AT+CRES command.this command is used
//                to restore the parameters specified in the at+csca & at+csmp
//                AT+CRES
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCRES);

/*****************************************************************************/
//  Description : This function handles the AT+CSAS command.this command is used
//                to save the SMS configuration profile
//                AT+CSAS
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSAS);


/*****************************************************************************/
//  Description : This function handles the AT+CPMS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPMS);


/*****************************************************************************/
//  Description : This function handles the AT+CPMS command This command is used
//                to set or read the parameters of sending SMS
//                AT+CSMP=<fo>,<pid>,<vp>,<dcs>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSMP);

/*****************************************************************************/
//  Description : This function handles the AT+CMGF command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGF);

/*****************************************************************************/
//  Description : This function handles the AT+CMGR command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGR);

/*****************************************************************************/
//  Description : This function handles the AT+CSCA command to set the short
//                message service centre.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSCA);

/*****************************************************************************/
//  Description : This function handles the AT+CNMI command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCNMI);

/*****************************************************************************/
//  Description : This function handles the AT+CMGD command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGD);

/*****************************************************************************/
//  Description : This function handles the AT+CMGS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGS);

/*****************************************************************************/
//  Description : This function handles the AT+CMGW command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGW);


/*****************************************************************************/
//  Description : This function handles the AT+CMGC command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGC);

/*****************************************************************************/
//  Description : This function handles the AT+CMSS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMSS);

/*****************************************************************************/
//  Description : This function handles the AT+CNMA command.
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCNMA);

/*****************************************************************************/
//  Description : This function handles the PDU send from the SIO.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsPdu(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *pdu_ptr  // pointer to the given pdu buffer
);

extern ATC_STATUS ATC_ProcessReadSmsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_READ_SMS_CNF_T    *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the PDU send from the SIO.
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsStatusPdu(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *pdu_ptr  // pointer to the given pdu buffer
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_IND signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsInd(
    ATC_CONFIG_T       *atc_config_ptr,
    APP_MN_SMS_IND_T   *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_UPDATE_SMS_STATUS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUpdateSmsCnf(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_UPDATE_SMS_STATUS_CNF_T    *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_DELETE_ALL_SMS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       george.yuan 2009-06-21
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDeleteAllSmsCnf(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_DELETE_ALL_SMS_CNF_T    *sig_ptr
);

/*****************************************************************************/
//  Description : This function initializes the static variables for SMS module
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern void ATC_InitSmsConfig(ATC_CONFIG_T *atc_config_ptr);

/*****************************************************************************/
//  Description : This function handles the AT+CMGL command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGL);
/*****************************************************************************/
//  Description : This function handles the AT+CPMS command This command is used
//                to set or read the parameters of sending SMS
//                AT+CSMP=<fo>,<pid>,<vp>,<dcs>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSMP);
/*****************************************************************************/
//  Description : This function get the specific storage's total amount and used
//                amount.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN GetStorageInfo(
    MN_DUAL_SYS_E   dual_sys,
    MN_SMS_STORAGE_E   storage,     // in--get the where store sms's status, ME or SIM
    MN_SMS_RECORD_ID_T *total_ptr,  // out--the max number of sms can saved in ME or SIM
    MN_SMS_RECORD_ID_T *used_ptr,
    BOOLEAN            *unread_flag
);
/*****************************************************************************/
//  Description : This function handles the AT+CSAS command.this command is used
//                to save the SMS configuration profile
//                AT+CSAS
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSAS);

/*****************************************************************************/
//  Description : This function handles the AT+CSMS command.this command is used
//                to set message service
//                AT+CSMS=<service>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSMS);

/*****************************************************************************/
//  Description : This function handles the AT+CSDH command.this command is used
//                to give additional information on text mode result code.
//                AT+CSDH
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSDH);

/*****************************************************************************/
//  Description : This function handles the AT+CRES command.this command is used
//                to restore the parameters specified in the at+csca & at+csmp
//                AT+CRES
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCRES);
/*****************************************************************************/
//  Description : This function handles the AT+SMSC command. This command is used
//                to change the specific SMS status: from READ to unread; for SENT
//                to UNSENT.
//  AT+SMSC=<loc>,<status>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSMSC);

/*****************************************************************************/
//  Description : This function handles the AT+SUSS command. This command is used
//                to set unchange the sms status
//  AT+SMSC=<loc>,<status>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSUSS);

/*****************************************************************************/
//  Description : This function handles the AT+CSCB command. This command is used
//                to select which type CBM to be received by the ME.
//  AT+CSCB=[<mode>[,<mids>[,<dcss>]]]
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSCB);

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handle the APP_MN_SMSCB_MSG_PAGES_IND signal
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessSmscbMsgPagesInd(
    ATC_CONFIG_T                 *atc_config_ptr,    //IN:
    APP_MN_SMSCB_MSG_PAGES_IND_T *sig_ptr            //IN:
);
#endif

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_MEM_FULL_IND signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsReadyInd(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_SMS_READY_IND_T            *sig_ptr
);


/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_MEM_FULL_IND signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsFullInd(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_SMS_MEM_FULL_IND_T            *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_STATUS_REPORT_IND signal.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsStatusReportInd(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_SMS_STATUS_REPORT_IND_T    *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_UPDATE_SMS_STATUS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessWriteSmsCnf(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_WRITE_SMS_CNF_T            *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSendSmsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_SEND_SMS_CNF_T    *sig_ptr
);

/*****************************************************************************/
//  Description : This function handles the APP_MN_UPDATE_SMS_STATUS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessWriteSmsCnf(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_WRITE_SMS_CNF_T            *sig_ptr
);

/*****************************************************************************/
//  Description : This function restore the s_sms_config.service_flag
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void ATC_RestoreSmsConfig(MN_DUAL_SYS_E  dual_sys, ATC_CONFIG_T   *atc_config_ptr);


/*****************************************************************************/
//  Description : This function handles the AT+CMMS command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMMS);

/*****************************************************************************/
//  Description : This function handles the +CMMS timer expire
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
void ATC_ProcessCMMSTimeout(uint32 lparam);

/**********************************************************************
//  Description:
//      the funciton to start CMMS timer
//  Global resource dependence :
//  Author: Pizer.Fan
//  Note:
***********************************************************************/
void ATC_ActiveCMMSTimer(BOOLEAN act_flg);

/*****************************************************************************/
//  Description:
//      the function to start SPSMS function
//  Global resource dependence :
//  Author: Bruce.Jiang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSMSFULL);

	
/*****************************************************************************/
//  Description : This function handles the APP_MN_SET_SMS_RECEIVE_FUNC_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Bruce.Jiang
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSetSmsRcvFunCnf(
    ATC_CONFIG_T       *atc_config_ptr,
    APP_MN_SET_SMS_RECEIVE_FUNC_CNF_T   *sig_ptr
);


#ifdef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handles the AT+SCMGS command 
//  Global resource dependence : g_rsp_str
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMGS );

/*****************************************************************************/
//  Description : This function handles the AT+SCMGW command 
//                AT+CMGW=<length/destination address>,<state>,<sms string>
//                state: 0   "REC UNREAD"
//                       1   "REC READ"
//                       2   "STO UNSEND"
//                       3   "STO SENT"
//  Global resource dependence : g_rsp_str
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMGW );
#endif

/*****************************************************************************/
//  Description : This function converts MN_SMS_CAUSE_E to ATC_SMS_ERR_CODE
//  Global resource dependence :
//  Author:       andy.huang
//  Note:
/*****************************************************************************/
ATC_SMS_ERR_CODE_E ATC_ConvMnSmsCause2AtcSmsCause(   //return: ATC_SMS_ERR_CODE_E, refer to 3GPP TS 27.005/3.2.5
    MN_SMS_CAUSE_E input_mn_sms_err                  //IN: mn sms error code
);

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function used to initialize sms cell broadcast parameters
//  Global resource dependence :
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
extern void ATC_InitSmsCellBroadcastPara(
    ATC_CONFIG_T *atc_config_ptr, 
    BOOLEAN is_all_init, 
    MN_DUAL_SYS_E dual_sys
    );
#endif
/*****************************************************************************/
//  Description : This function handle sms module when receive sim error 
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
void ATC_SmsSimRemovedInd(
        MN_DUAL_SYS_E dual_sys,
        ATC_CONFIG_T     *atc_config_ptr
        );

/*****************************************************************************/
//  Description : This function reset sms sending status 
//  Global resource dependence :
//  Author:Haoming.Lu
//  Note:
/*****************************************************************************/
extern void ATC_ResetSmsSendingStatus(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
