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
    }
#endif

/*****************************************************************************
**                    macro variables
******************************************************************************/
#define  ATC_MIN_SM_LEN             0x09
#define  ATC_MAX_SM_LEN             0XA0

#ifdef _MUX_REDUCED_MODE_
#define  MT_IND_DEFAULT_VAL         0x02
#else
#define  MT_IND_DEFAULT_VAL         0x00
#endif
#define  BM_IND_DEFAULT_VAL         0x00
#define  DS_IND_DEFAULT_VAL         0X00
#define  BFR_IND_DEFAULT_VAL        0x00

#define  ATC_SMS_7BIT_LEN           160
#define  ATC_SMS_8BIT_LEN           140
#define  ATC_SMS_UCS2_LEN           140

#define ATC_SMS_MINUTES_VAL         143
#define ATC_SMS_HOURS_VAL           167
#define ATC_SMS_DAYS_VAL            196
#define ATC_SMS_WEEKS_VAL           255

// the AT+CSMP parameter
#define ATC_SMS_FO_DEFAULT_VAL      0X13
#define ATC_SMS_VP_DEFAULT_VAL      143
#define ATC_SMS_PID_DEFAULT_VAL     0

#ifdef ATC_ORANGE_ENABLE
    #define   AT_SMS_STORAGE_ME             0
#else
    #define   AT_SMS_STORAGE_BM             0
#endif
#define   AT_SMS_STORAGE_SM             1
#define   AT_SMS_STORAGE_SR             2

#define   AT_RETURN_SESULT_CODE     3

#define  ATC_MAX_PDU_LENGTH         0xA0
#define  ATC_MAX_DIGITAL            41

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
}ATC_SMS_DEL_STATE_E;

enum
{
    ATC_MT_NO_DELIVER,
    ATC_MT_CMTI,                   
    ATC_MT_CMT                    
};


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
    MN_SMS_STORAGE_E   mem1;    /* Memory from which messages are read and deleted
                                 ** the current version only supports "SM"
                                 */
    MN_SMS_STORAGE_E   mem2;    /* Memory to which writing and sending options are made (see AT+CMSS and AT+CMGW).
                                 ** The current version only supports "SM"
                                 */ 
    MN_SMS_STORAGE_E   mem3;    /* Memory to which received SMs are preferred to be stored
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
    BOOLEAN             sms_is_ready;
    int32               sm_len;
    BOOLEAN             suss_flag;

 
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
AT_CMD_FUNC( ATC_ProcessCMGL );

/*****************************************************************************/
//  Description : This function handles the AT+CSMS command.this command is used
//                to set message service
//                AT+CSMS=<service> 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSMS );

/*****************************************************************************/
//  Description : This function handles the AT+CSDH command.this command is used
//                to give additional information on text mode result code.
//                AT+CSDH=<value>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSDH );

/*****************************************************************************/
//  Description : This function handles the AT+CRES command.this command is used
//                to restore the parameters specified in the at+csca & at+csmp
//                AT+CRES
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRES );

/*****************************************************************************/
//  Description : This function handles the AT+CSAS command.this command is used
//                to save the SMS configuration profile
//                AT+CSAS 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSAS );


/*****************************************************************************/
//  Description : This function handles the AT+CPMS command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPMS );


/*****************************************************************************/
//  Description : This function handles the AT+CPMS command This command is used 
//                to set or read the parameters of sending SMS
//                AT+CSMP=<fo>,<pid>,<vp>,<dcs>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSMP );

/*****************************************************************************/
//  Description : This function handles the AT+CMGF command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGF );

/*****************************************************************************/
//  Description : This function handles the AT+CMGR command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGR );

/*****************************************************************************/
//  Description : This function handles the AT+CSCA command to set the short 
//                message service centre. 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSCA);

/*****************************************************************************/
//  Description : This function handles the AT+CNMI command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCNMI );

/*****************************************************************************/
//  Description : This function handles the AT+CMGD command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGD );

/*****************************************************************************/
//  Description : This function handles the AT+CMGS command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGS );

/*****************************************************************************/
//  Description : This function handles the AT+CMGW command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGW );

/*****************************************************************************/
//  Description : This function handles the AT+CMGC command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGC );

/*****************************************************************************/
//  Description : This function handles the AT+CMSS command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMSS );

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


#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
/*****************************************************************************/
//  Description : This function handles the PDU send from the SIO. 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsPdu(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *pdu_ptr            // pointer to the given pdu buffer
                );

/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and send the 
//                result code to SIO
//  Global resource dependence : 
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessReadSmsCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_READ_SMS_CNF_T    *sig_ptr
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
//  Description : This function initializes the static variables for SMS module
//  Global resource dependence : 
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern void ATC_InitSmsConfig( void );

/*****************************************************************************/
//  Description : This function get the specific storage's total amount and used 
//                amount.
//  Global resource dependence : 
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN GetStorageInfo(
                                    MN_DUAL_SYS_E dual_sys,
                     MN_SMS_STORAGE_E   storage,     // in--get the where store sms's status, ME or SIM
                     MN_SMS_RECORD_ID_T *total_ptr,  // out--the max number of sms can saved in ME or SIM
                     MN_SMS_RECORD_ID_T *used_ptr,
                     BOOLEAN            *unread_flag    
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

#endif

 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
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
#endif

/*****************************************************************************/
//  Description : This function handles the AT+CMGL command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGL );
/*****************************************************************************/
//  Description : This function handles the AT+CPMS command This command is used 
//                to set or read the parameters of sending SMS
//                AT+CSMP=<fo>,<pid>,<vp>,<dcs>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSMP );

/*****************************************************************************/
//  Description : This function handles the AT+CSAS command.this command is used
//                to save the SMS configuration profile
//                AT+CSAS 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSAS );

/*****************************************************************************/
//  Description : This function handles the AT+CSMS command.this command is used
//                to set message service
//                AT+CSMS=<service> 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSMS );

/*****************************************************************************/
//  Description : This function handles the AT+CSDH command.this command is used
//                to give additional information on text mode result code.
//                AT+CSDH
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSDH );

/*****************************************************************************/
//  Description : This function handles the AT+CRES command.this command is used
//                to restore the parameters specified in the at+csca & at+csmp
//                AT+CRES
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRES );
/*****************************************************************************/
//  Description : This function handles the AT+SMSC command. This command is used 
//                to change the specific SMS status: from READ to unread; for SENT
//                to UNSENT.
//  AT+SMSC=<loc>,<status> 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSMSC );

/*****************************************************************************/
//  Description : This function handles the AT+CSCB command. This command is used 
//                to select which type CBM to be received by the ME.
//  AT+CSCB=<mode>,<mids>,<dcss>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSCB );

/*****************************************************************************/
//  Description : This function handles the AT+SFSMSM command 
//                AT+SFSMSM=<length/destination address>,<state>
//                state: 0   "REC UNREAD"
//                       1   "REC READ"
//                       2   "STO UNSEND"
//                       3   "STO SENT"
//  Global resource dependence : g_rsp_str
//  Author:       Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSFSMSM );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_HELD_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessSmscbMsgInd(
                ATC_CONFIG_T            *atc_config_ptr,
                APP_MN_SMSCB_MSG_IND_T  *sig_ptr
                );
#endif

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
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
//  Description : This function restore the s_sms_config.service_flag
//  Global resource dependence : 
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void ATC_RestoreSmsConfig(MN_DUAL_SYS_E dual_sys);
#endif

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

AT_CMD_FUNC( ATC_ProcessCMMS );

#if 0
/*****************************************************************************/
//  Description : This function handles the AT+SMSC command. This command is used 
//                to set unchange the sms status
//  AT+SMSC=<loc>,<status> 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSUSS );

/*****************************************************************************/
//  Description : This function handles the AT+SCMGS command 
//  Global resource dependence : g_rsp_str
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMGS );

/*****************************************************************************/
//  Description : This function handles the AT+SCMGW command 
//  Global resource dependence : g_rsp_str
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMGW );
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
