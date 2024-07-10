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

// for AT+CBC command
#define AT_CBC_NO_CHARGE                 0
#define AT_CBC_CHARGE                    1
#define AT_CBC_NO                        2

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

/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/
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

typedef enum
{
    ATC_GET_ENHA_MODE_NUM = 0,      //  0  :get audio enha mode total number
    ATC_GET_ENHA_MODE_DATA,         //  1  :get audio enha data from memory
    ATC_SET_ENHA_MODE_DATA,         //  2  :set audio enha data into memory
    ATC_GET_ENHA_MODE_NV_DATA,      //  3  :get audio enha mode parameters from flash
    ATC_SET_ENHA_MODE_NV_DATA       //  4  :set audio enha mode parameters into flash
}ATC_ENHA_MODE_OPT_TYPE_E;

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

typedef struct
{
    OPER_STATUS_E            oper_status;
    uint16 lac;
    uint16 cell_id;
}ATC_CREG_STATE_T;

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function initializes the static variables 
//  Global resource dependence : 
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern void ATC_InitBasicVar(void);
#endif

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
AT_CMD_FUNC( ATC_ProcessSPADLVVE );

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPADLVVE
//  Global resource dependence : g_rsp_str
//  Author: chong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADLVVEData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPADLVVE
//  Global resource dependence : g_rsp_str
//  Author: chong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADLVVENVData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );
/* -add for lvve function */

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

AT_CMD_FUNC( ATC_ProcessSADMDSP ); 

/*****************************************************************************/
//  Description : This function handle the AT+SPLO command£¬Preferred operator
//    list 
//  Global resource dependence : g_rsp_str
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPLO );

/*****************************************************************************/
//  Description : This function handles the AT+CRSL command.This command is used
//                  to set or get the sound level of the ringer on incoming call.
//  AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRSL );

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

#ifndef _ULTRA_LOW_CODE_
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
//  Description : This function handles the AT+SPEQPARA command  
//                        Get(Set) eq para from(to) RAM or FALSH
//  Global resource dependence : 
//  Author:     elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPEQPARA) ;

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
//  Description : This function handles the AT+SLOG command.This command is used
//  to enable the log output.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSLOG );

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

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_SCELL_RSSI_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRssiInd(
                          ATC_CONFIG_T                  *atc_config_ptr,
                          APP_MN_SCELL_RSSI_IND_T       *sig_ptr);
#endif
                          
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

#ifndef _ULTRA_LOW_CODE_
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
#endif
                    
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
                                                                                                                                                            
#ifndef _ULTRA_LOW_CODE_
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
                    xSignalHeaderRec              *sig_ptr);

/*****************************************************************************/
//  Description : This function handles the ATC_ProcessPowerOnCnf signal
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPsPowerOffCnf(ATC_CONFIG_T                  *atc_config_ptr, 
                    xSignalHeaderRec              *sig_ptr);
                    
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

#ifndef _ULTRA_LOW_CODE_
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

PUBLIC uint32 ATC_ProcessSioDSPSADMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );

PUBLIC uint32 ATC_ProcessSioDSPSADMNVData(               
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                );
#endif/*AUDIO_PARAM_ARM*/

#ifdef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function handles the midi pdu file   
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDownFile(
                ATC_CONFIG_T     *atc_config_ptr,
                uint8            *param_ptr,
                uint16            param_len
                );

/*****************************************************************************/
//  Description : This function handles the midi pdu file   
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note: add for Gionee
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDownFileWithChk(
                ATC_CONFIG_T     *atc_config_ptr,
                uint8            *param_ptr,
                uint16            param_len
                );

/********************************************************************************/
#endif

/*****************************************************************************/
//  Description : This function handle the AT+CPAS command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPAS );

/*****************************************************************************/
//  Description : This function handles the AT+CMER command
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMER);

/*****************************************************************************/
//  Description : This function handles the AT+CVHU command
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCVHU);

/*****************************************************************************/
//  Description : This function handle the AT+SPAUDVOLTYPE command
//  Global resource dependence : g_rsp_str
//  Author:  chong.liu
//  Note: 
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPAUDVOLTYPE );

/*****************************************************************************/
//  Description : This function handle the AT&F command 
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_F );

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

#if 0
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
//  Description : This function handle the AT+CBC command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCBC );

/*****************************************************************************/
//  Description : This function handle the AT+CRC command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCRC(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

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
//  Description : This function handles the AT+CRSM command.
//  Global resource dependence : g_rsp_str
//  Author:       tracy.zhang
/*****************************************************************************/ 
AT_CMD_FUNC( ATC_ProcessCRSM );

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
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
