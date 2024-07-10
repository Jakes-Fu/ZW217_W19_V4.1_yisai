/******************************************************************************
** File Name:      atc_eng.h                                                  *
** Author:         Lisa.Lin                                             *
** DATE:           24/04/2008                                                *
** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file contains the functions that handle the AT command
**                 for engineering mode,mainly NV Operation
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** ------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                               *
** 24/04/2008     Lisa.Lin         Create.                                   *
******************************************************************************/
#ifndef _ATC_ENG_H
#define _ATC_ENG_H

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
{
#endif
/*****************************************************************************
**                    macro variables
******************************************************************************/
#define ATC_MAX_SPN_LEN 16
#define ATC_MAX_GSM_CELL_NUM                10
#define ATC_MAX_GSM_CELL_INFO_LEN           41
/*****************************************************************************
**                    enum define
******************************************************************************/
typedef enum
{
    FTA_TEST = 0x00,      //FTA TEST
    INTEGRALITY_TEST, //integrality test
    RW_TEST, //real network
    AOC_TEST,//Invalid parameter unsupport
    GSIM_SUPPORT_TEST,  //GSM SIM Support
    PS_VERSION_TEST,    //R4 or R5
    TRAFFIC_CLASS_TEST, //interactive,background
    SERVICE_TYPE_TEST, //CS ONLY,PS ONly,PS and CS Both
    CELL_CAPABILITY_TEST,/* mbms_support: (1<<0)
                       hsdpa_support:(1<<1)
                       hsupa_support:(1<<2)
                       edge_support: (1<<3)*/
    WAKEUP_CONDITION_TEST,
    DSP_LOG_TEST,
    FBAND_TEST,
    TEST_ITEM_MAX
} TEST_ITEM;

typedef enum
{
    RAT_AUTO_MODE = 2,
    RAT_GSM_ONLY = 13,
    RAT_WCDMA_ONLY = 14,
    RAT_TD_ONLY = 15,
    RAT_NO_CHANGE = 16
} AT_Network_Mode;

typedef enum
{
    PRAT_AUTO_MODE = 0,
    PRAT_GSM_PERFERED,
    PRAT_3G_PERFERED,
    PRAT_NO_CHANGE,
    PRAT_HUAWEI_GSM_PERFERED
} AT_Perfered_RAT;

typedef enum
{
    ROAMING_NOT_SUPPORT = 0,
    ROAMING_SUPPORT,
    ROAMING_NO_CHANGE
} AT_Roaming_Service;

typedef enum
{
    DOMAIN_CS_ONLY = 0,
    DOMAIN_PS_ONLY,
    DOMAIN_CS_PS,
    DOMAIN_ANY,
    DOMAIN_NO_CHANGE
} AT_Service_Domain;

typedef enum
{
    UNKNOWN_MODE = 0,
    SIM_MODE,
    USIM_MODE
} AT_SimType;

typedef enum
{
    ECMMB_OPER_POWER_OFF = 0,
    ECMMB_OPER_POWER_ON,
    ECMMB_OPER_ENTER_SPI,
    ECMMB_OPER_ENTER_UNKNOWN = 0xFF
} AT_Eccmb_Oper_E;

typedef enum
{
    ECMMB_STS_POWER_OFF = 0,
    ECMMB_STS_POWER_ON,
    ECMMB_STS_ENTER_SPI,
    ECMMB_STS_HARDWARE_ERROR,
    ECMMB_STS_ENTER_UNKNOWN = 0xFF
} AT_Ecmmb_Sts_E;

/*the type of information string which is received from MMI*/
typedef enum
{
    DUMMYMMI_RSP_WIN = 0,           //operation window information
    DUMMYMMI_RSP_MESSAGE,           //message box information
    DUMMYMMI_RSP_BROWSERTITLE,      //browser title information
    DUMMYMMI_RSP_BROWSERCURSOR,     //browser cursor information
    DUMMYMMI_RSP_SOFTKEY,           //LEFT/RIGTH softkey information
    DUMMYMMI_RSP_IMAGEID,           //image id information
    DUMMYMMI_RSP_FOCUSWIN,          //current focus window
    DUMMYMMI_RSP_WINSTRING,         //current window context information
    DUMMYMMI_RSP_BACKLIGHT,         //current backlight state
    DUMMYMMI_RSP_MAX
} AT_DUMMYMMI_RSP_TYPE_E;

typedef enum
{
    SGMR_SN = 0,
    SGMR_IMEI,
    SGMR_SV,
    SGMR_CALI,
    SGMR_CALI_VAL,
    SCMR_CHECK_PHASE,
    SGMR_C_IMEI
} ATC_SGMR_TYPE_E;

typedef enum
{
    OP_READ = 0,
    OP_WRITE = 1
} SGMR_OPER_E;

typedef enum _ATC_SPN_NAME_CODE_SCHEM_E
{
    ATC_SPN_NAME_7BIT_ALPHA,
    ATC_SPN_NAME_UCS2,
    ATC_SPN_NAME_NUM
} ATC_SPN_NAME_CODE_SCHEM_E;

typedef enum _ATC_UPLMN_MODE_E
{
    ATC_UPLMN_GET,
    ATC_UPLMN_SET,
    ATC_UPLMN_MAX
} ATC_UPLMN_MODE_E;

typedef struct
{
    uint16  cmd_id;
    uint16  arfcn;
    uint16  gainpower;
    uint8  ms_band;
    BOOLEAN  is_ps_active;    //to store the ps state(active or deactive) before max power transmission    
    MN_DUAL_SYS_E dual_sys;
} ATC_POWER_INF_T;

typedef struct
{
    uint8            index;
    uint8            mode;
    MN_PLMN_T        plmn;
    MN_GMMREG_RAT_E  rat;
} ATC_UPLMN_INF_T;

typedef enum _ATC_BT_MODE
{
    ATC_BT_MODE_EUT,
    ATC_BT_MODE_CH,
    ATC_BT_MODE_TX,
    ATC_BT_MODE_TXPWR,
    ATC_BT_MODE_RX,
    ATC_BT_MODE_RXPACKCOUNT,
    ATC_MAX_BT_MODE_NUM
} ATC_BT_MODE_E;

#define ATC_MAX_DIAG_MODE_LEN       20
#define ATC_MAX_DIAG_ACT_LEN        5
typedef struct
{
    uint8        cmd_type;
    uint8        act_value[ATC_MAX_DIAG_ACT_LEN]; // operation action string for uint8, uint16,uint32 para
    uint8        mode_len;
    uint8        mode[ATC_MAX_DIAG_MODE_LEN];
} ATC_DIAG_PARAM_T;

/*****************************************************************************/
//  Description : This function handles the AT+SPPSRATE command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPPSRATE);

/*****************************************************************************/
//  Description : This function handles the AT^SYSINFO command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SYSINFO);

/*****************************************************************************/
//  Description : This function handles the AT+SPCOMDEBUG command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPCOMDEBUG);

/*****************************************************************************/
//  Description : This function handles the ATC^SPN command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SPN);

/*****************************************************************************/
//  Description : This function handles the ATC+ARMLOG command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessARMLOG);

/*****************************************************************************/
//  Description : This function handles the ATC+UDISK command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessUDISK);

/*****************************************************************************/
//  Description : This function handles the AT+ECMMB command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessECMMB);

/*****************************************************************************/
//  Description : This function handles the AT+ECMMBSTS command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessECMMBSTS);

/*****************************************************************************/
//  Description : This function handles the AT+ECMMBEER command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessECMMBEER);

/*****************************************************************************/
//  Description : This function handles the AT+USBCT command
//  Global resource dependence :
//  Author:       xia.wei
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processstar_USBCT);

/*****************************************************************************/
//  Description : This function initializes the static variables for ENG module
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
void ATC_InitEngVar(void);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : This function is a callback function, it receives and
//                      handles the message which is sent by MMI.
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
extern int32  ATC_ProcessAutoTestInfoFromMMI(
    const uint8   *str_ptr,     //the string type refer to AT+SPDUMMYKPDRSP
    uint32        str_len
);

/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPD command
//                dummy keypad operation
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPD) ;
/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPDRSP command
//                get dummy keypad operation response
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPDRSP) ;

/*****************************************************************************/
//  Description : This function handle the AT+SPCEER command
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPCEER);

/*****************************************************************************/
//  Description : This function handle the AT+SPCHIPTYPE command
//                Get chip type
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPCHIPTYPE);

/*****************************************************************************/
//  Description : This function handles the AT+SPEQPARA command
//                      Get(Set) eq para from(to) RAM or FALSH
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPEQPARA);

/*****************************************************************************/
//  Description : This function handle the AT+SPAUTODLOADER command
//                 Automatic downloader
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPREF);

/*****************************************************************************/
//  Description : This function handles the AT+SCPFRE command.This command is used
//  to set or get frequency value.
//  Global resource dependence : g_rsp_str
//  Author:       minqian.qian
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSCPFRE);

/*****************************************************************************/
//  Description : This function handle the AT+SL1MON command 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSL1MON );

/*****************************************************************************/
//  Description : This function handle the AT+SADC command 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSADC );

/*****************************************************************************/
//  Description : This function handle the AT+SGSIMG command to Get Sim image
//  Global resource dependence : none
//  Author:   elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSGSIMG);

/*****************************************************************************/
//  Description : This function handle the AT+SDRMOD command 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSDRMOD);

/*****************************************************************************/
//  Description : This function handle the AT+SGMR command 
//  Global resource dependence : none
//  Author:
//  Note: parameter - dual_sys, read/write, type, string
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSGMR);

/*****************************************************************************/
//  Description : This function handle the AT+SFPL command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSFPL);

/*****************************************************************************/
//  Description : This function handle the AT+SEPL command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSEPL);

/*****************************************************************************/
//  Description : This function handle the AT+SPGSMFRQ command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPGSMFRQ);

/*****************************************************************************/
//  Description : This function handle the AT+SCDSLEEP command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSCDSLEEP);

/*****************************************************************************/
//  Description : This function handle the AT+SUSB command to inform usb connection status
//  Global resource dependence : none
//  Author: Elvis xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSUSB);

/*****************************************************************************/
//  Description : This function handle the AT+STPTEST command to transmit the given power
//  Global resource dependence : none
//  Author: Elvis xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSTPTEST);

/*****************************************************************************/
//  Description : This function used to start the test of GSM given power Transmission
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_StartGsmTxPwrTest(ATC_CONFIG_T *atc_config_ptr);

/*****************************************************************************/
//  Description : This function handles the AT+SSWAPCOM command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSSWAPCOM);

/*****************************************************************************/
//  Description : This function handles the AT+SASYNC command to set async mode
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSASYNC);

/*****************************************************************************/
//  Description : This function handles the AT+SPTEST command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPTEST);

/*****************************************************************************/
//  Description : This function handles the AT+SPUPLMN command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPUPLMN);

/*****************************************************************************/
//  Description : This function handles the AT+SPID command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPID);

/*****************************************************************************/
//  Description : This function handles the UPLMN info
//  Global resource dependence :
//  Author: Elvis.Xu
//  Note:
/*****************************************************************************/
void ATC_InitUplmnCfgInfo(MN_DUAL_SYS_E  dual_sys);

/*****************************************************************************/
//  Description : This function handles the AT^CARDMODE command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_CARDMODE);

/*****************************************************************************/
//  Description : This function handles the ATC^SYSCONFIG command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SYSCONFIG);

/*****************************************************************************/
//  Description : This function handles the AT+SPDSP command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDSP);

/*****************************************************************************/
//  Description : This function handles the AT+SPFRQ command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFRQ);

/*****************************************************************************/
//  Description : This function handles the AT+SPDIAG command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDIAG);

/*****************************************************************************/
//  Description : This function handles the AT+SPBTTEST command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPBTTEST);
#ifndef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function is used to read command line from diag command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
extern void ATC_ReadDiagCmdLine (
    uint8 *data_ptr,       // Output argument, point to the buf which is alloced
    uint length,           // The length of cmd line which should be read from
    uint *read_length_ptr  // The actual read length
);
#endif
/*****************************************************************************/
//  Description : This function is used to get the parameters of diag command
//  Global resource dependence : none
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_GetDiagParams(
    uint8            *in_ptr,
    uint8             in_len,
    ATC_DIAG_PARAM_T *param_ptr
);

 /*****************************************************************************/
//  Description : This function handles the AT+SSIMDETPIN command to set async mode
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSSIMDETPIN);

/*****************************************************************************/
//  Description : This function handles the AT+SGETIQ command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
extern AT_CMD_FUNC(ATC_ProcessSGETIQ);

#ifdef   __cplusplus
}
#endif
#endif
