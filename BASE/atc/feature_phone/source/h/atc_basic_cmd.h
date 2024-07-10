/******************************************************************************
 ** File Name:      atc_basic_cmd.h                                           *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           20/08/2002                                                *
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
 ** 20/08/2002     Xueli.lv         Create.                                   *
 ** 15/07/2003     Liuhan           Modify.                                   *
 ******************************************************************************/

#ifndef _ATC_BASIC_CMD_H
#define _ATC_BASIC_CMD_H

#include "atc.h"
#include "stdio.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/******************************* macro variables*******************************/
#define AT_SIDE_TONE0                    0
#define AT_SIDE_TONE1                    1
#define AT_SIDE_TONE2                    2
#define AT_SIDE_TONE3                    3
#define AT_SIDE_TONE4                    4
#define AT_SIDE_TONE5                    5
#define AT_SIDE_TONE6                    6
#define AT_SIDE_TONE7                    7
#define AT_SIDE_TONE8                    8
#define AT_SIDE_TONE9                    9
#define AT_SIDE_TONEA                   10
#define AT_SIDE_TONEF                   15

#define AT_CCLK_STR_LEN                 17

// The following macros define the supported baudrate
#define AT_BAUD_1200                  1200
#define AT_BAUD_2400                  2400
#define AT_BAUD_4800                  4800
#define AT_BAUD_9600                  9600
#define AT_BAUD_19200                19200
#define AT_BAUD_38400                38400
#define AT_BAUD_57600                57600
#define AT_BAUD_115200              115200
#define AT_BAUD_230400              230400
#define AT_BAUD_460800              460800

// define the max value for time yy/mm/dd,hh:mm:ss
#define AT_MAX_MONTH                    12
#define AT_MAX_DAY                      31
#define AT_MAX_HOUR                     23
#define AT_MAX_MINUTE                   59
#define AT_MAX_SECOND                   59

// The uint duration 100ms
#define AT_SPEAKER_UNIT_DURATION       100
#define AT_SPEAKER_MAX_GAIN             15
#define AT_DEFAULT_WDTMF_GAIN            9

// for AT+CRMP command
#define AT_CRMP_VOICE_CALL               0
#define AT_CRMP_DATA_CALL                1
#define AT_CRMP_FAX_CALL                 2
#define AT_CRMP_SMS                      3
#define AT_CRMP_SMS_REPEAT               1

#define AT_MAX_SIDET_LEV                 5
#define AT_INVALID_RXQUAL               99
#define AT_RXLEV_ZERO                    0

#define AT_CLCK_ALL_BAR_SVR      0X00
#define AT_CLCK_BAR_OUTGOING     0X01
#define AT_CLCK_BAOC             0X02
#define AT_CLCK_BOIC             0X03
#define AT_CLCK_BOIC_EXHC        0X04
#define AT_CLCK_BAR_INCOMING     0X05
#define AT_CLCK_BAIC             0X06
#define AT_CLCK_BICROAM          0X07

#define AT_MIX_MODE              1   // The current mode is mix mode
#define AT_AT_MODE               0   // The current mode is AT mode

// for the battery charge
#define AT_STOP_CHR        0
#define AT_START_CHR       1
#define AT_GET_BATT_VOL    2
#define AT_SET_BATT_PARM   3

#define AT_IPR_MAX         10
#define ATC_POWEROFF_INTERVAL    5*1000   // 5 seconds

#ifndef _ULTRA_LOW_CODE_
// @Shijun.cui 2005-07-12 CR27598
#ifdef _ATC_ONLY
  #define AT_NORMAL_RING_NUM       5
  #define AT_SMS_RING_NUM          4
#else
  #define AT_NORMAL_RING_NUM       5
  #define AT_SMS_RING_NUM          4
#endif // _ATC_ONLY
// end CR27598
#else
  #define AT_NORMAL_RING_NUM       1
  #define AT_SMS_RING_NUM          1
#endif /* _ULTRA_LOW_CODE_ */

#define AT_SPEAKER_GAIN_LEVEL    6


#define ATC_SNVM_NUM            0
#define ATC_SNVM_STRING         1

#define ATC_REF_PARAMETER       1
#define ATC_IMEI                2
//#define ATC_LOG_SWITCH          3
//#define ATC_DEEP_SLEEP          4

//for GPIO define
#define AT_GPIO_SET_DIRECTION    0
#define AT_GPIO_GET_DIRECTION    1
#define AT_GPIO_SET_VALUE        0
#define AT_GPIO_GET_VALUE        1

//for AT+SADM command
typedef enum
{
    ATC_GET_AUD_MODE_NUM = 0,       //  0  :get audio mode total number
    ATC_GET_AUD_MODE_NAME,           //  1  :get audio mode name
    ATC_GET_AUD_MODE_DATA,           //  2  :get audio data from memory
    ATC_SET_AUD_MODE_DATA,           //  3  :set audio data into memory
    ATC_SET_AUD_DEV_MODE,             //  4  :set audio mode
    ATC_SET_AUD_MODE_NV_DATA,       //  5  :set audio mode parameters into flash
    ATC_GET_AUD_MODE_NV_DATA,       //  6  :get audio mode parameters from flash
    ATC_GET_AUD_ARM_MODE_NUM,             //  7  :get audio mode total number
    ATC_GET_AUD_ARM_MODE_NAME,           //  8  :get audio mode name
    ATC_GET_AUD_ARM_MODE_DATA,           //  9  :get audio data from memory
    ATC_SET_AUD_ARM_MODE_DATA,           //  10  :set audio data into memory
    ATC_SET_AUD_ARM_MODE_NV_DATA,    //  11  :set audio mode parameters into flash
    ATC_GET_AUD_ARM_MODE_NV_DATA     //  12  :get audio mode parameters from flash
} ATC_AUD_MODE_OPT_TYPE_E;


#define ATC_SPEAKER              1
#define ATC_PIN1                 0
#define ATC_PIN2                 1
#define ATC_PUK1                 2
#define ATC_PUK2                 3

#define AT_STOP_CHR        0
#define AT_START_CHR       1
#define AT_GET_BATT_VOL    2
#define AT_SET_BATT_PARM   3


#define AT_AUD_VOL_DEFAULT_VAL                        4 //3
#define AT_AUD_VOL_SPEAKER_MAX                        9 //7

#define AT_AUD_VOL_SIDET_TONE_MAX                    32768
#define AT_AUD_VOL_DIGITAL_HANDSET_UP_MAX            65535
#define AT_AUD_VOL_DIGITAL_HANDSET_DOWN_MAX          65535
#define AT_AUD_VOL_DIGITAL_HANDFREE_UP_MAX           65535
#define AT_AUD_VOL_DIGITAL_HANDFREE_DOWN_MAX         65535
#define AT_AUD_VOL_PGA_HANDSET_UP_MAX                11
#define AT_AUD_VOL_PGA_HANDSET_DOWN_MAX              11
#define AT_AUD_VOL_PGA_HANDFREE_UP_MAX               11
#define AT_AUD_VOL_PGA_HANDFREE_DOWN_MAX             11

#define AT_AUD_VOL_HANDSET_UL_EQ_INDEX               32
#define AT_AUD_VOL_HANDSET_UL_EQ_MAX                 65535
#define AT_AUD_VOL_HANDFREE_UL_EQ_INDEX              32
#define AT_AUD_VOL_HANDFREE_UL_EQ_MAX                65535
#define AT_AUD_VOL_HANDSET_DL_EQ_INDEX               32
#define AT_AUD_VOL_HANDSET_DL_EQ_MAX                 65535
#define AT_AUD_VOL_HANDFREE_DL_EQ_INDEX              32
#define AT_AUD_VOL_HANDFREE_DL_EQ_MAX                65535
#define AT_AUD_ECHO_TEST_MAX                         1
#define AT_AUD_ECHO_TEST_FREQ_MIN                    250
#define AT_AUD_ECHO_TEST_FREQ_MAX                    3400
#define AT_AUD_VOL_HANDSET_DIGITAL_SCALE_UP_MAX      65535
#define AT_AUD_VOL_HANDSET_DIGITAL_SCALE_DOWN_MAX    65535
#define AT_AUD_VOL_HANDFREE_DIGITAL_SCALE_UP_MAX     65535
#define AT_AUD_VOL_HANDFREE_DIGITAL_SCALE_DOWN_MAX   65535


#define ATC_SNVM_READ            0
#define ATC_SNVM_ADD             1
#define ATC_SNVM_REPLACE         2
#define ATC_SNVM_GETSIZE         3
#define ATC_SNVM_DELETE          4

#define ATC_MAX_BT_ADDRESS_STRING_LEN 20

#define ATC_MAX_BT_NAME_STRING_LEN 164 //BT_DEVICE_NAME_SIZE = 41

#define ATC_BT_NAME_ASCII 0 //the bt name encode in ascii
#define ATC_BT_NAME_UCS2 1 //the bt name encode in ucs2

/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/
#ifdef WIN32
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned long int   uint32_t;
#endif
typedef enum
{
    ATC_CREG_DISABLE,        // disable network registration unsolicited result code
    ATC_CREG_ENABLE,         // enable network registration unsolicited result code +CREG: <stat>
    ATC_CREG_AND_INFO        // enable network registration and location information unsolicited result code +CREG:<stat>,[<lac>,<ci>]
} ATC_CREG_TYPE_E;

typedef enum
{
    ATC_NO_OPERATE,
    ATC_RESET_ACM_UNIT,
    ATC_SET_ACM_MAX
} ATC_AOC_OPERATE_E;

typedef struct
{
    OPER_STATUS_E            oper_status;
    uint16 lac;
    uint16 cell_id;
}ATC_CREG_STATE_T;

typedef enum
{
    ATC_GET_ENHA_MODE_NUM = 0,      //  0  :get audio enha mode total number
    ATC_GET_ENHA_MODE_DATA,         //  1  :get audio enha data from memory
    ATC_SET_ENHA_MODE_DATA,         //  2  :set audio enha data into memory
    ATC_GET_ENHA_MODE_NV_DATA,      //  3  :get audio enha mode parameters from flash
    ATC_SET_ENHA_MODE_NV_DATA       //  4  :set audio enha mode parameters into flash
}ATC_ENHA_MODE_OPT_TYPE_E;

typedef struct
{
    uint16  cmd_id;
    uint16  arfcn;
    uint16  gainpower;
    uint8  ms_band;
    BOOLEAN  is_ps_active;    //to store the ps state(active or deactive) before max power transmission
    MN_DUAL_SYS_E dual_sys;
}ATC_POWER_INF_T;

typedef struct
{
    uint8 mode;
    uint16 voice_format;
}ATC_LOOP_TEST_INFO_T;
/* +add for lvve function */
typedef enum
{
    ATC_GET_LEVE_MODE_NUM = 0,       //  0  :get audio mode total number
    ATC_GET_LEVE_MODE_NAME,          //  1  :get audio mode name
    ATC_GET_LEVE_MODE_DATA,          //  2  :get audio data from memory
    ATC_SET_LEVE_MODE_DATA,           //  3  :set audio mode
    ATC_GET_LEVE_MODE_NV_DATA,       //  4  :get audio mode parameters from flash
    ATC_SET_LEVE_MODE_NV_DATA,       //  5  :set audio mode parameters into flash
} ATC_LEVE_MODE_OPT_TYPE_E;
/* -add for lvve function */

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
#define ATC_MAX_DIAG_ACT_LEN        16
typedef struct
{
    uint8        cmd_type;
    uint8        act_value[ATC_MAX_DIAG_ACT_LEN]; // operation action string for uint8, uint16,uint32 para
    uint8        mode_len;
    uint8        mode[ATC_MAX_DIAG_MODE_LEN];
} ATC_DIAG_PARAM_T;

typedef struct
{
    uint8        cmd_type;
    uint8        mode_len;
    uint8        mode[ATC_MAX_DIAG_MODE_LEN];
    uint8        act_len[4];
    uint8        act_value[4][ATC_MAX_DIAG_ACT_LEN]; // operation action string for uint8, uint16,uint32 para
} ATC_DIAG_PARAM_EX_T;

typedef enum _ATC_FM_MODE
{
    ATC_FM_TESTMODE,
    ATC_FM_VOLUME,
    ATC_FM_MUTE,
    ATC_FM_TUNE,
    ATC_FM_SEEK,
    ATC_FM_RREG,
    ATC_FM_WREG,
    ATC_FM_STATUS,
    ATC_FM_IQ,
    ATC_FM_DATACAPTURE,
    ATC_FM_SHORTANT,
    ATC_MAX_FM_MODE_NUM
} ATC_FM_MODE_E;

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

#define MAX_DUMMY_KPD_RSP_LEN      256       //max rsp info length
#define ATC_MAX_GSM_CELL_INFO_LEN  41

typedef enum
{
    ATC_POWER_OFF_SIM_PS = 0,
    ATC_POWER_ON_SIM_ONLY,
    ATC_POWER_ON_SIM_NORMAL,
    ATC_POWER_OFF_SIM,
    ATC_POWER_ON_PS,
    ATC_POWER_OFF_PS
} ATC_SFUN_TYPE_E;

typedef enum
{
    ATC_FUN_DEACT_SIM_PS = 0,
    ATC_FUN_ACT_SIM_PS,
    ATC_FUN_ACT_SIM,
    ATC_FUN_DEACT_SIM,
    ATC_FUN_ACT_PS,
    ATC_FUN_DEACT_PS
} ATC_FUN_STATUS_E;

typedef enum
{
    SV_LTE_TEST_MODE = 0X00,
    TD_LTE_ONLY_MODE,
    LTEFDD_ONLY_MODE,
    TD_LTEFDD_LTE_DUAL_MODE,
    LTE_IDD_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE,
    TD_LTE_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE,
    TD_LTE_OR_LTE_FDD_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE,
    TD_LTE_OR_TD_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE,
    TD_LTE_OR_LTEFDD_OR_TD_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE,
    TD_LTE_OR_LTEFDD_OR_TD_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE,
    GSM_ONLY_MODE,
    W_ONLY_MODE,
    TD_ONLY_MODE,
    TG_DUAL_MODE,
    WG_DUAL_MODE,
    GSM_ONLY_AND_MASTER_CARD,
    M_GSM_PREFER_IN_WG_DUAL_MODE,
    WCDMA_OR_TD_ONLY_MODE,
    WCDMA_ONLY_MASTER_CARD_MODE,
    TD_ONLY_MASTER_CARD_MODE,
    TG_OR_WG_MASTER_CARD_MODE,
    LTE_AND_GSM_MODE,
    LTE_AND_WCDMA_MODE = 0x18,
    NO_SIM_MODE = 0xFE,
    TG_DUAL_MODE_OR_WG_DUAL_MODE,
} TESTMODEM_MODE;


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
    WAKEUP_CONDITION_TEST, // DSP-CTA network test selection
    DSP_LOG_TEST, // Dsp log
    FBAND_TEST, // Main frequency or secondary configuration for FBAND
    IS_VT_MT_CALL_SUPPORT_TEST = 0x0D,
    TRACE_TASK_SWITCH_TEST, // Open/close TRACE TASK
    TRACE_LEVEL_TEST, // Open/close TRACE LEVEL
    EMERGENCE_ACTIVATE_PS_FOR_PSIT_TEST = 0x17, // Emergence activate PS only for psit
    CLOSE_AUDIO_TEST, // In FPGA call testing, Need to close the audio, so add AT to control the Audio
    DISABLE_ENABLE_MAX_POWER_TEST, // In normal mode (PS power on), need to open max power in GSM, so add interface to notify grr to disable/enable max power
    AGPS_TEST, // AGPS default QoS timeout
    PS_TASK_POINT_SWITCH_TEST, // PS Task Point Switch
    VOICE_CODEC_SWITCH_TEST, // Voice codec switch
    IMEI_IN_LTE, // imei in LTE
    IMEI_IN_2G_OR_3G_TEST, // imei in 2/3G
    IMEISV_IN_LTE_TEST, // imeisv in LTE
    IMEISV_IN_2G_OR_3G_TEST, // imeisv in 2/3G
    CONTROL_ENG_MODE_SWITCH_TEST, // Control ENG mode switch
    OPEN_LTE_FEATURE_IN_NON_SIM_TEST, // For instrument test,open lte feature in non_sim state
    IMS_TEST_FOR_SIP_MESSAGE_TEST, // ims test for sip message
    LINK_ID_SETTING_TEST, // Link ID setting
    TEST_ITEM_MAX
} TEST_ITEM;

typedef int32 (* ATC_MMI_RECEIVER_FPTR)(
                const uint8* str_ptr,    // point to the string
                uint32 str_len     // received string length
                );
/*****************************************************************************/
//  Description : This function handle the ATQ command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessQ(
                ATC_CONFIG_T        * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATV command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessV(
                ATC_CONFIG_T        * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATV command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCMEE(
                ATC_CONFIG_T        * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );



/*****************************************************************************/
//  Description : This function handle the AT+CREG command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCREG(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATS0 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS0(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the AT+CSQ command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCSQ(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the AT+CGMI command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCGMI(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the AT+CGSN command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCGSN(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the AT+CGMM command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCGMM(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the AT+CGMR command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCGMR(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the APP_MN_NETWORK_STATUS_IND signal and
//                send the unsolicited result code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessNetworkStatusInd(
                   ATC_CONFIG_T                  *atc_config_ptr,
                   APP_MN_NETWORK_STATUS_IND_T   *sig_ptr
                   );

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_Tun_Data(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_Data(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_Tun_NVData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_NVData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );

/*****************************************************************************/
//  Description : This function handle the AT+CSCS command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSCS );

/*****************************************************************************/
//  Description : This function handle the AT+CPIN command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPIN );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_AOC_INCREASE_ACM_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessIncreaseAcmInd(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_AOC_MODIFIED_IND_T      *sig_ptr
                      );

/*****************************************************************************/
//  Description : This function handles the ATCMD_READW_CNF signal and send the
//                  result code to SIO
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
PUBLIC ATC_STATUS ATC_ProcessReadDSPCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_READ_DSP_MEM_CNF_T    *sig_ptr
                );

/*****************************************************************************/
//  Description : This function handles the APP_MN_SCELL_RSSI_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRssiInd(
                          ATC_CONFIG_T                  *atc_config_ptr,
                          APP_MN_SCELL_RSSI_IND_T       *sig_ptr);

/*****************************************************************************/
//  Description : This function handles the ATC_CHR_CAP_IND signal.And inform the
//    DTE the current battery level
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChrCapInd(
                    ATC_CONFIG_T                  *atc_config_ptr,
                    xSignalHeaderRec              *sig_ptr
                    );

/*****************************************************************************/
//  Description : This function handles the ATC_CHR_CHARGE_START_IND signal.Inform
//    that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChrStartInd(
                    ATC_CONFIG_T                  *atc_config_ptr,
                    xSignalHeaderRec              *sig_ptr
                    );

/*****************************************************************************/
//  Description : This function detects a proper sim card whose pdp context  is able to be activated
//  Global resource dependence : s_atc_pdp_act_sys_cap
//  Author:       Elivs Xu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_DetectSimCardForPdpAct(
                                                        ATC_CONFIG_T   * atc_config_ptr,
                                                        MN_DUAL_SYS_E * current_sys);
#endif

/*****************************************************************************/
//  Description : This function handle the AT command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessAT );

/*****************************************************************************/
//  Description : This function handle the ATE command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessE );

/*****************************************************************************/
//  Description : This function handle the ATE command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessZ );

/*****************************************************************************/
// 	Description : This function handle the ATX command
//	Global resource dependence : none
//	Author:      longting.zhao
//	Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessX);

/*****************************************************************************/
//  Description : This function handle the AT+XX command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessXX );

/*****************************************************************************/
//  Description : This function handle the AT+CCID command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCID );

/*****************************************************************************/
//  Description : This function handle the AT+CCLK command
//  Global resource dependence : none
//  Author:       xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCLK );
/*****************************************************************************/
//  Description : This function handle the AT+VGR command
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVGR );
/*****************************************************************************/
//  Description : This function handle the AT+SSEA command
//                AT+SSEA=<value>
//                       <value>:0 -- 8
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSEA );

 /* +add for lvve function */
/*****************************************************************************/
//  Description : This function handle the AT+VGR command
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SSEA
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSSEAData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16            param_len
                );

/*****************************************************************************/
//  Description : This function handle the AT+CFUN command
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCFUN );

/*****************************************************************************/
//  Description : This function handle the AT+CIND command
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCIND );

/*****************************************************************************/
//  Description : This function handle the AT+CCED command
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCED );

/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_NOT_READY_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessNotReadyInd(
                       ATC_CONFIG_T                    *atc_config_ptr,
                       APP_MN_SIM_NOT_READY_IND_T      *sig_ptr
                       );

/*****************************************************************************/
//  Description : This function handle the AT## command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSHARP_SHARP );

/*****************************************************************************/
//  Description : This function handle the AT+WTONE command
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTONE );

/*****************************************************************************/
//  Description : This function handle the AT+ASSERT command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessASSERT );

/*****************************************************************************/
//  Description : This function handles the AT+WIND command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSIND );

/*****************************************************************************/
//  Description : This function handles the AT+SDTMF command
//                AT+SDTMF is used to play the DTMF tone on the current speaker
//                AT+SDTMF=<mode>,<dtmf>,<gain>,<duartion>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSDTMF );

/*****************************************************************************/
//  Description : This function handles the AT+SCDM command AT+WCDM is used to
//                  select a melody to be played for a new incoming call.
//  AT+WCDM=<melody>,<player>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCDM );

/*****************************************************************************/
//  Description : This function handle the AT+SSAM command
//  Global resource dependence : none
//  Author:  Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSAM );

/*****************************************************************************/
//  Description : This function handle the AT+SSAP command
//  Global resource dependence : none
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSAP );

/*****************************************************************************/
//  Description : This function handle the AT+SADM command
//  Global resource dependence : none
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSADM );


/*****************************************************************************/
//  Description : This function handle the AT+SPLO command£¬Preferred operator
//    list
//  Global resource dependence : g_rsp_str
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPLO );
#if 0
/*****************************************************************************/
//  Description : This function handles the AT+CRSL command.This command is used
//                  to set or get the sound level of the ringer on incoming call.
//  AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRSL );
#endif
/*****************************************************************************/
//  Description : This function handles the AT+CMUT command.This command is used
//                  to mute the micrphone input.
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMUT );

/*****************************************************************************/
//  Description : This function handles the AT+CIMI command. This command is used
//                  to get the IMSI of the SIM card.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCIMI );

/*****************************************************************************/
//  Description : This function handleS the AT+CPOF command. This command is used
//  to power off the module
//  AT+CPOF
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPOF );

/*****************************************************************************/
//  Description : This function handleS the AT+CPOL command. This command is used
//  to power off the module
//  AT+CPOL
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPOL );

/*****************************************************************************/
//  Description : This function handles the AT+SBRA command, This command is used
//  to enable the ATC
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSBRA );

/*****************************************************************************/
//  Description : This function handles the AT+SBAND command, This command is used
//  to select the band in EGSM900/DCS1800/PCS1900.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSBAND );

#ifdef _ATC_ONLY
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SNVM
//  Global resource dependence :
//  Author:       SHIJUN
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSNVMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr  // pointer to the data
                );
#endif

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SRWD
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSRWDData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr  // pointer to the data
                );

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:new added by king.li for cr103703
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADMNVData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );

/*****************************************************************************/
//  Description : This function handles the AT+SRWD command, This command is used
//  to manage the NV items.
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSRWD );

/*****************************************************************************/
//  Description : This function handles the AT+SUPS command.This command is used
//  to enable the log output.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSUPS );

/*****************************************************************************/
//  Description : This function handles the AT+SCPFRE command.This command is used
//  to set or get frequency value.
//  Global resource dependence : g_rsp_str
//  Author:       king.li
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCPFRE );

/*****************************************************************************/
//  Description : This function handles the AT+SSYS command.This command is used
//  to set or get the operated sim id .
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSYS );
/*****************************************************************************/
//  Description : This function handles the AT+SCINF command.This command is used
//  to get some information about the serving and neighbour cells .
//  Global resource dependence : g_rsp_str
//  Author:       elvis.xu
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCINF );



/*****************************************************************************/
//  Description : This function handle the AT+SPREF command
//                   Automatic downloader
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPREF);

/*****************************************************************************/
//  Description : This function handle the AT+SYTIN command
//  Global resource dependence : none
//  Author:shuyan.hu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSYTIN);


/*****************************************************************************/
//  Description : This function handle the AT+SFTIN command
//  Global resource dependence : none
//  Author:shuyan.hu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSFTIN);


/*****************************************************************************/
//  Description : This function handles the APP_MN_SET_BAND_CNF
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSetBandCnf(
                ATC_CONFIG_T                *atc_config_ptr,
                APP_MN_SET_BAND_CNF_T       *sig_ptr
                );

/*****************************************************************************/
//  Description : This function handle the AT+SIME command
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note: This is for hisilicon modem project
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSIME );

/*****************************************************************************/
//  Description : This function handles the AT+PSORGMOD command.This command is used
//  to set audio mode. This is used for AP+BB scenario. If AP connect only with BB's
//  receiver, and AP want to use this physical channel as handon/handfree/BT earphone,
//  it will use this function to set audio param to DSP without change physical channel.
//  handon/handfree/BT should have the same device name in the NV item.
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSORGMOD);

/*****************************************************************************/
//  Description : This function handle the APP_MN_SYNC_IND signal
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessMnSyncInd(
                    ATC_CONFIG_T            *atc_config_ptr,
                    APP_MN_SYNC_IND_T  *sig_ptr
                    );

/*****************************************************************************/
//  Description : This function handle the AT+SPENHA command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPENHA );
/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_READY_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessReadyInd(
                          ATC_CONFIG_T                  *atc_config_ptr,
                          APP_MN_SIM_READY_IND_T        *sig_ptr);

/*****************************************************************************/
//  Description : This function handles the APP_MN_DETACH_GSM_CNF signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPowerOffCnf(
                            ATC_CONFIG_T                  *atc_config_ptr,
                            APP_MN_DEACTIVE_PS_CNF_T     *sig_ptr
                            );


/*****************************************************************************/
//  Description : This function handles the APP_MN_PREFER_PLMN_CHANGE_CNF at the
//                   AT+CPOL
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPreferPlmnChangCnf(
                                                ATC_CONFIG_T                    *atc_config_ptr,
                                                APP_MN_PREFER_PLMN_CHANGE_CNF_T *sig_ptr
                                                );

/*****************************************************************************/
//  Description : This function handles the ATC_CHR_CHARGE_END_IND signal.Inform
//    that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChrEndInd(
                    ATC_CONFIG_T                  *atc_config_ptr,
                    xSignalHeaderRec              *sig_ptr
                    );


#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
/*****************************************************************************/
//  Description : This function initializes the static variables
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern void ATC_InitBasicVar(void);

/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_NOT_READY_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessNotReadyInd(
                       ATC_CONFIG_T                    *atc_config_ptr,
                       APP_MN_SIM_NOT_READY_IND_T      *sig_ptr
                       );

/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_GET_PIN_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPinInd(
                        ATC_CONFIG_T                   *atc_config_ptr,
                        APP_MN_SIM_GET_PIN_IND_T       *sig_ptr);
#endif

/*****************************************************************************/
//  Description : This function handles the ATC_ProcessPowerOnCnf signal
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPsPowerOnCnf(ATC_CONFIG_T *atc_config_ptr,
                    xSignalHeaderRec              *sig_ptr,
                    uint8 bExpect);

/*****************************************************************************/
//  Description : This function handles the ATC_ProcessPowerOnCnf signal
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPsPowerOffCnf(ATC_CONFIG_T                  *atc_config_ptr,
                    xSignalHeaderRec              *sig_ptr,
                    uint8 bExpect);


/*****************************************************************************/
//  Description : This function handles the APP_MN_CRSM_CNF signal in
//    deleting only one item.
//  Global resource dependence :
//  Author:       WangHui
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCrsmCnf(
                ATC_CONFIG_T         *atc_config_ptr,
                APP_MN_CRSM_CNF_T    *sig_ptr
                );

 #ifdef AUDIO_PARAM_ARM
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author: chong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioARMSADMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author:  chong.liu
//  Note: new added for the function to read&write audio parameters into NV
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioARMSADMNVData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );

#endif/*AUDIO_PARAM_ARM*/

#ifdef _NDS_EXT_CMD_
/*****************************************************************************/
//  Description : is sim OK?
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern BOOLEAN  DMSIM_SimIsReady( // TRUE: sim is ready
              MN_DUAL_SYS_E dual_sys
              );
#endif
/*****************************************************************************/
//  Description : This function handle the AT&C command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_C );

/*****************************************************************************/
//  Description : This function handle the AT&D command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_D );
/*****************************************************************************/
//  Description : This function handle the AT&F command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_F );

/*****************************************************************************/
//  Description : This function handle the ATS7 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS7(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//   Description : This function handle the AT+SPAUDVOLTYPE command
//  Global resource dependence : g_rsp_str
//  Author:  chong.liu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPAUDVOLTYPE);

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
//  Description : This function handle the AT+CPAS command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPAS );

/*****************************************************************************/
//  Description : This function handle the AT+GMM command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGMM );

/*****************************************************************************/
//  Description : This function handle the AT+FCLASS command
//  Global resource dependence : g_rsp_str
//  Author:  Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCLASS);

/*****************************************************************************/
//  Description : This function handles the AT_CLS command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSHARP_CLS);

/*****************************************************************************/
//  Description : This function handles the AT+GCI command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGCI);

/*****************************************************************************/
//  Description : This function handles the AT+I1 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI1);

/*****************************************************************************/
//  Description : This function handles the AT+I2 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI2);

/*****************************************************************************/
//  Description : This function handles the AT+I3 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI3);

/*****************************************************************************/
//  Description : This function handles the AT+I4 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI4);

/*****************************************************************************/
//  Description : This function handles the AT+I5 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI5);

/*****************************************************************************/
//  Description : This function handles the AT+I6 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI6);

/*****************************************************************************/
//  Description : This function handles the AT+I7 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI7);
/*****************************************************************************/
//  Description : This function initializes the static variables for ENG module
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
void ATC_InitEngVar(void);
#ifdef AUTO_TEST_SUPPORT
/*****************************************************************************/
//  Description : This function is used convert string data to number
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ConvertStringtoUint16(uint8 *str_ptr, uint16 length, uint16 *num_ptr);

/*****************************************************************************/
//  Description : register callback funtion for dummy mmi keypad response.
//                It is used by ATC.
//  Global resource dependence : none
//  Author: minqian.qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E ATC_RegisterMMIKpdRspFun(
                        ATC_MMI_RECEIVER_FPTR      callback_ptr
                        );
/*****************************************************************************/
/* 	Description : Auto Poweron Flag */
/*	Global resource dependence : none */
/*  Author:Joe.Ni */
/*	Note: */
/*****************************************************************************/
extern void ATC_REFPARAM_SetAutoPowerOnFlag(uint8 mode);
/*****************************************************************************/
/* 	Description : Get Auto Poweron Flag */
/*	Global resource dependence : none */
/*  Author:Joe.Ni */
/*	Note: */
/*****************************************************************************/
extern uint8 ATC_REFPARAM_GetAutoPowerOnFlag(void);
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
#endif
/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPD command
//                dummy keypad operation
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPD);

/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPDRSP command
//                get dummy keypad operation response
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPDRSP) ;

/*****************************************************************************/
//  Description : This function handles the AT+SPAUTOPOWERON command.
//                This command is used to set or get auto poweron flag.
//  Global resource dependence : g_rsp_str
//  Author:       Joe.Ni
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPAUTOPOWER );

/*****************************************************************************/
//  Description : This function handle the AT+CRC command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRC );

/*****************************************************************************/
//  Description : This function handles the AT+CRSM command.
//  Global resource dependence : g_rsp_str
//  Author:       tracy.zhang
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRSM );



/*****************************************************************************/
//  Description : This function handles the AT+CSCLK command.This command is used
//  to set or get deep sleep mode.
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPMMODE);

#if 0
/*****************************************************************************/
//  Description : This function handle the AT&D command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_D);

/*****************************************************************************/
//  Description : This function handle the AT&C command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_C );

/*****************************************************************************/
//  Description : This function handle the ATW command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessW );

/*****************************************************************************/
//  Description : This function handle the AT&K command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_K );

/*****************************************************************************/
//  Description : This function handle the ATS95 command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS95 );

/*****************************************************************************/
//  Description : This function handle the AT%%% command
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessTHREE_PERCENT );

/*****************************************************************************/
//  Description : This function handleS the AT&W command
//              AT&W is used to save the current configuration to the nv
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_W );


/*****************************************************************************/
//  Description : This function handleS the AT+SBCM command. This command is used
//  to manage battery charge.
//  AT+SBCM=<mode>,<chargeind>,<battLevMax>,<battLevMin>,<TPulseInCharge>,<TPulseOutCharge>,
//    <BattIntRes>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSBCM );

/*****************************************************************************/
//  Description : This function handle the ATS2 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS2(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/*****************************************************************************/
//  Description : This function handle the ATS3 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS3(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATS4 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS4(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATS5 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS5(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATS6 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS6(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );


/*****************************************************************************/
//  Description : This function handle the ATS8 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS8(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the ATS10 command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessS10(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handle the AT+VGT command
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVGT );

/*****************************************************************************/
//  Description : This function handle the AT+SPEAKER command
//                AT+SPEAKER=<value>,<value>
//                       <value>:0 -- 1
//  Global resource dependence : none
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPEAKER );

/*****************************************************************************/
//  Description : This function handle the AT+ECHO command
//  Global resource dependence : none
//  Author:     Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessECHO );

/*****************************************************************************/
//  Description : This function handle the AT+CICB command
//    this command is used to set the type of incoming calls
//               AT+CICB=<mode>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCICB );

/*****************************************************************************/
//  Description : This function handle the AT+IPR command
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessIPR );

/*****************************************************************************/
//  Description : This function handles the AT+CRMP command AT+WDTMF is used to
//                play melody
//  AT+CRMP=<call type>,<volume>,<type>,<index>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRMP );

/*****************************************************************************/
//  Description : This function handle the AT+CR command
//                  set if its GPRS type before show connect
//  Global resource dependence : none
//  Author:       xu.xiaogang
//  Note:
//  History:
//        xu.xiaogang 2003-9-26 create
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCR(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description : This function handles the AT+SNVM command, This command is used
//    to manage the NV items.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSNVM );

/*****************************************************************************/
//  Description : This function handles the AT+SBOOT command.This command is used
//  to set the spreadtrum module to BOOT mode.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli.lv
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSBOOT );

/*****************************************************************************/
//  Description : This function handles the AT+SLOG command.This command is used
//  to enable the log output.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSLOG );

/*****************************************************************************/
//  Description : This function handles the AT+SSGF command.This command is used
//  to set or get GPIO function.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSGF );

/*****************************************************************************/
//  Description : This function handles the AT+SSGS command.This command is used
//  to set or get GPIO value.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSGS );

/*****************************************************************************/
//  Description : This function handle the AT+SCREG command
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCREG );

/*****************************************************************************/
//  Description : This function handle the AT+CIEV command
//  Global resource dependence : none
//  Author:
//  Note: this command set the rssi report level
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCIEV );

/*****************************************************************************/
//  Description : This function handle the AT+GSMSTATUS command
//                 Automatic downloader
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGSMSTATUS );

/*****************************************************************************/
//  Description : This function handle the AT+FIRBOOT command
//                 Automatic downloader
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessFIRBOOT );
/*****************************************************************************/
//  Description : This function handle the AT+VGSMSW command
//                 Automatic downloader
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVGSMSW );
/*****************************************************************************/
//  Description : This function handle the AT+POSI command
//                 Automatic downloader
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPOSI );
#endif

/*****************************************************************************/
//  Description : This function handles the AT+SPFMMODE command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFMMODE);

/*****************************************************************************/
//  Description : This function handles the AT+SPFMTUNE command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFMTUNE);

/*****************************************************************************/
//  Description : This function handles the AT+SPDSP command to read/write RF register
//  Global resource dependence :
//                      P0	P1	            P2	            P3
//  Reg read	        0	RF reg address	X               X
//  Reg write           1   RF reg address	RF reg value    X
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessRWRFREG);

/*****************************************************************************/
//  Description : This function handles the AT+SPFMTEST command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFMTEST);

/*****************************************************************************/
//  Description : This function handle the AT+SPUSIMTEST command
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPUSIMTEST);

/*****************************************************************************/
//  Description : This function handles the AT+ETSRESET command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
//This at command used to factory reset (erase running NV) under the calibration mode.
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessETSRESET);

/*****************************************************************************/
//  Description : This function handle the AT+SPDATAENCRYPT  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPDATAENCRYPT );

/*****************************************************************************/
//  Description : This function handle the AT+SPSIMLOCKDATAWRITE  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPSIMLOCKDATAWRITE );

/*****************************************************************************/
//  Description : This function handle the AT+SPSIMLOCKCHECK  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSIMLOCKCHECK);

/*****************************************************************************/
//  Description : This function handle the AT+SPSIMLOCKIMEI  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPSIMLOCKIMEI );

/*****************************************************************************/
//  Description : This function handle the AT+SPDATAENCRYPT  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPHUK );

/*****************************************************************************/
//  Description : This function handle the AT+SPSIMLOCKDATAWRITE  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPFACAUTHPUBKEY );

/*****************************************************************************/
//  Description : This function handle the AT+SPSIMLOCKIMEI  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPIDENTIFYSTART );

/*****************************************************************************/
//  Description : This function handle the AT+SPSIMLOCKIMEI  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPIDENTIFYEND );



extern ATC_STATUS ATC_ProcessSimlockData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *pdu_ptr  // pointer to the given pdu buffer
);


/*****************************************************************************/
//  Description : This function handles the AT+SPBTCTRL command
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPBTCTRL);

/*****************************************************************************/
//  Description :convert the atc input string 12345678 to uint32 0x12345678
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
BOOLEAN ConvertStringtoUint32(uint8 *str_ptr, uint8 length, uint32 *num_ptr);

/*****************************************************************************/
//  Description :convert the atc input bt address string to bt address
//  For example :atc input string 11:22:33:44:55:66,convert to bt address 0x112233445566
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
BOOLEAN ConvertStringtoBtAddr(uint8 *str_ptr, uint8 length, uint8 *addr);

/*****************************************************************************/
//  Description :convert the bt address to atc output string
//  For example :the bt address 0x112233445566,convert to atc output string 11:22:33:44:55:66
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
void ConvertBtAddrtoString(uint8 *addr,uint8 *str_ptr);

/*****************************************************************************/
//  Description :convert the bt name double byte ascii 0030 to ascii 30
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint8* ConvertWstrToStr(const uint16* src,uint8* dst);

/*****************************************************************************/
//  Description :convert the bt name ascii 30 to double byte ascii 0030
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16* ConvertStrToWstr(const uint8* src,uint16* dst);

/*****************************************************************************/
//  Description :convert the bt name to string
//              bt name is encoded in UCS2,if it is double byte ascii,convert to ascii,else convert to string
//              for example: 1) UCS2 004D004F0043004F0052,convert to MOCOR
//                           2) UCS2 5F204E09,convert to string 5F204E09
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
void ConvertBtNametoString(uint16 *name,uint8 name_arr_len,uint8 *name_str_ptr,uint8 name_str_arr_len);

/*****************************************************************************/
//  Description :convert the string to bt name
//              bt name is encoded in UCS2.
//              if the atc input bt name is ascii,convert to double byte ascii(UCS2);
//              if the atc input bt name is UCS2 string,convert to UCS2
//              for example: 1) atc input bt name is MOCOR,convert to UCS2 004D004F0043004F0052
//                           2) atc input bt name is string 5F204E09,convert to UCS2 5F204E09
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
void ConvertStringtoBtName(uint8 type,uint8 *str,uint8 str_len,uint16 *name,uint8 name_arr_len);

/*****************************************************************************/
//  Description : This function handles the AT+SPBLETEST command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPBLETEST);

/*****************************************************************************/
//  Description : This function handle the AT+FOTA command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessFOTA );

/*****************************************************************************/
//  Description : This function handles the AT+SCWTEST command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSCWTEST);

/*****************************************************************************/
//  Description : This function handles the AT^SCFG command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
//  This at command used to CW function
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SCFG);

extern ATC_STATUS ATC_ProcessNVGV(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );


extern ATC_STATUS ATC_ProcessNVPC(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );



extern ATC_STATUS ATC_Processxor_SSIT(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

extern AT_CMD_FUNC(ATC_ProcessCACCP);

extern AT_CMD_FUNC(ATC_ProcessCAVQE);

extern AT_CMD_FUNC(ATC_ProcessCAPRE);

extern AT_CMD_FUNC(ATC_ProcessCAPOST);

extern AT_CMD_FUNC(ATC_ProcessCAWTF);

extern AT_CMD_FUNC(ATC_ProcessCAIET);

extern AT_CMD_FUNC(ATC_ProcessCADTF);

extern AT_CMD_FUNC(ATC_ProcessCAVCT);

extern AT_CMD_FUNC(ATC_ProcessCAVIMS);

extern AT_CMD_FUNC(ATC_ProcessSPSELMODE);

extern AT_CMD_FUNC(ATC_ProcessRRCREL);

extern AT_CMD_FUNC(ATC_ProcessIMSEN);

extern AT_CMD_FUNC(ATC_ProcessSPIMEI);

extern AT_CMD_FUNC(ATC_ProcessSFUN);

extern AT_CMD_FUNC(ATC_ProcessSPTESTMODEM);

extern AT_CMD_FUNC(ATC_ProcessSPTEST);

extern AT_CMD_FUNC(ATC_ProcessSPDATA);

extern AT_CMD_FUNC(ATC_ProcessCEMODE);

extern AT_CMD_FUNC(ATC_ProcessCANXP);

extern AT_CMD_FUNC(ATC_Processxor_SYSCONFIG);

extern AT_CMD_FUNC(ATC_ProcessSPLTEDUMMYPARA);

extern AT_CMD_FUNC(ATC_ProcessRRTMPARAM);

extern AT_CMD_FUNC(ATC_ProcessGRRLTEFREQ);

extern AT_CMD_FUNC(ATC_ProcessL1PARAM);

extern AT_CMD_FUNC(ATC_ProcessRRDMPARAM);

extern AT_CMD_FUNC(ATC_ProcessSPCLEANINFO);

extern AT_CMD_FUNC(ATC_ProcessXCAPRTURI);

extern AT_CMD_FUNC(ATC_ProcessBSF);

extern AT_CMD_FUNC(ATC_ProcessCISRVCC);

extern AT_CMD_FUNC(ATC_ProcessSPCHKSD);

extern AT_CMD_FUNC(ATC_ProcessEMMCDDRSIZE);

extern AT_CMD_FUNC(ATC_ProcessCHANGERAT);

extern AT_CMD_FUNC(ATC_ProcessFACNETINFO);

extern AT_CMD_FUNC(ATC_ProcessFACNETINFOSTA);

extern AT_CMD_FUNC(ATC_ProcessSPRNDIS);

extern AT_CMD_FUNC(ATC_ProcessCHECKOPERAIN);

extern AT_CMD_FUNC(ATC_ProcessAUDCH);

extern AT_CMD_FUNC(ATC_ProcessSPWIQ);

extern AT_CMD_FUNC(ATC_ProcessIMSNV);

extern AT_CMD_FUNC(ATC_ProcessSPADCVS);

extern AT_CMD_FUNC(ATC_ProcessPEINFO);

extern AT_CMD_FUNC(ATC_ProcessWIFI);

extern AT_CMD_FUNC(ATC_ProcessARMLOG);

extern AT_CMD_FUNC(ATC_ProcessSDRMOD);

extern AT_CMD_FUNC(ATC_ProcessCALIBINFO);

extern AT_CMD_FUNC(ATC_ProcessCGNSSST);

extern AT_CMD_FUNC(ATC_ProcessSPGPSTEST);

/*****************************************************************************/
//  Description: This function handles the AT+NETIF command
//                    used for show Net Interface Information .
//  Example:     AT+NETIF=net info;
//  Author:       Bangyu.Liu
/*****************************************************************************/
extern AT_CMD_FUNC(ATC_ProcessNETIF);

/*****************************************************************************/
//  Description: This function handles the AT+MTU command
//               used for seting MTU.
//  Example:     AT+MTU=V4,net name,1024  ;set this net IPv4 mtu to 1024
//               AT+MTU=V6,net name,1024  ;set this net IPv6 mtu to 1024
//               Use "AT+NETIF=net info" to get net interface name
/*****************************************************************************/
extern AT_CMD_FUNC(ATC_ProcessMTU);

/*****************************************************************************/
//  Description : This function handle the AT+DAPSIPERF  command
//  Global resource dependence : none
//  Author:  Xirui.Zhang
//  Note:
/*****************************************************************************/
extern AT_CMD_FUNC(ATC_ProcessDAPSIPERF);

/*****************************************************************************/
//  Description: This function handles the AT+TRAFFIC command
//               used for query traffic.
//  Example:     AT+TRAFFIC="sim"--query traffic
//                    AT+TRAFFIC="clear sim"--clear last traffic
/*****************************************************************************/
extern AT_CMD_FUNC(ATC_ProcessTRAFFIC);

PUBLIC uint32 ATC_ProcessSioSADCVSData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr,  // pointer to the data
    uint16          param_len
);

PUBLIC uint32 ATC_ProcessSioSADCVSNVData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr,  // pointer to the data
    uint16          param_len
);


extern AT_CMD_FUNC(ATC_ProcessSPADCVS);

extern AT_CMD_FUNC(ATC_ProcessPEINFO);

PUBLIC uint32 ATC_ProcessSioSADCVSData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr,  // pointer to the data
    uint16          param_len
);

PUBLIC uint32 ATC_ProcessSioSADCVSNVData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr,  // pointer to the data
    uint16          param_len
);

/*****************************************************************************/
//  Description: This function handles the AT+PING command
//               used for ping test.
//  Example:     AT+PING=V4,www.baidu.com
//               AT+PING=V6,ipv6.ustb.edu.cn
//  Author:      qin.he
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessPING);

/*****************************************************************************/
//  Description: This function handles the AT+SACK command
//               used for ping test.
//  Example:     AT+SACK=1, 100 //1:sync start test; 100: test timeout: 100s
//                     AT+SACK=2, 100 //2:Async start test; 100: test timeout: 100s
//                     AT+SACK=0  // stop test
//  Author:      qin.he
/*****************************************************************************/
ATC_STATUS ATC_ProcessSACK(
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/*****************************************************************************/
//  Description: This function handles the AT+SOCKET command
//               used for ping test.
//  Example:     AT+SOCKET=1, "loopback", 5 //1:sync start test; loopback/gprs: use loopback/gprs netif; 5: send interval is 5s
//                     AT+SOCKET=2, "loopback", 5 //1:Async start test; loopback/gprs: use loopback/gprs netif; 5: send interval is 5s
//                     AT+SOCKET=0  // stop test
//  Author:      qin.he
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSOCKET);

/*****************************************************************************/
//  Description: This function handles the AT+HTTP command
//               used for ping test.
//  Example:     AT+HTTP="https://www.baidu.com"
//  Author:      qin.he
/*****************************************************************************/
ATC_STATUS ATC_ProcessHTTP(
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );



AT_CMD_FUNC(ATC_ProcessMQTTCONN);

/*****************************************************************************/
// Description: This function handles the AT+MQTTPUB command
//                      used for publish a MQTT message on topic
// Example:     AT+MQTTPUB=<topic>,<message>, <qos>, <duplicate>, <retain>
//             topic--topic name
//             message--message of topic
//             qos--quality of mqtt service. Range of [0-2].
//             duplicate--not used, duplicate of MQTT. Range of [0-1].
//             retain--not used, retain of MQTT. Range of [0-1].
// Author:      hongqun.cheng
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMQTTPUB);

/*****************************************************************************/
// Description: This function handles the AT+MQTTSUBUNSUB command
//                      used for subscribe or unsubscribe a MQTT topic
// Example:     AT+MQTTSUBUNSUB=<topic>,<sub flag>, [qos]
//             topic--topic name
//             sub flag--Range of (0,1). 1-subscribe topic, 0-unsubscribe topic.
//             qos--quality of mqtt service. Be default, when flag is 0. Range of [0-2].
// Author:      hongqun.cheng
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMQTTSUBUNSUB);

/*****************************************************************************/
// Description: This function handles the AT+MQTTDISCONN command
//                      used for disconnect the MQTT connection
// Example:     AT+MQTTDISCONN
// Author:      hongqun.cheng
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMQTTDISCONN);

/*****************************************************************************/
// Description: This function handles the COAP_GET command
//                      used for create coap client and handle GET request
// Example:     AT+COAP_GET=<url>,<cmdline>, [timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOAP_GET);

/*****************************************************************************/
// Description: This function handles the COAP_POST command
//                      used for create coap client and handle POST request
// Example:     AT+COAP_POST=<url>,<cmdline>,<data> ,[timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             data -- user data to post
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOAP_POST);

/*****************************************************************************/
// Description: This function handles the COAP_PUT command
//                      used for create coap client and handle PUT request
// Example:     AT+COAP_PUT=<url>,<cmdline>,<data> ,[timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             data -- user data to put
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOAP_PUT);

/*****************************************************************************/
// Description: This function handles the COAP_DELETE command
//                      used for create coap client and handle DELETE request
// Example:     AT+COAP_DELETE=<url>,<cmdline>, [timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOAP_DELETE);

AT_CMD_FUNC(ATC_ProcessTESTLWM2M);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
