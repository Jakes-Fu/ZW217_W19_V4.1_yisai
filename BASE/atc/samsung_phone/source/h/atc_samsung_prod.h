/******************************************************************************
 ** File Name:      atc_samsung_prod.c                                                     *
 ** Author:         Elvis.xu                                             *
 ** Date:           05/10/2012                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains all commands related with Samsung production line tool     *
 ******************************************************************************/
#ifndef _ATC_SAMSUNG_PROD_H_
#define _ATC_SAMSUNG_PROD_H_
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#include "atc.h"

#define  ATC_IMEI_LENGTH                    15
#define ATC_PRODUCTION_LINE_CMD_MAX_LEN       50

#define  ATC_RSSI_READ_INTERVAL    200
#define  ATC_MAX_BAND_NUM     4

#define  ATC_LOOP_TEST_DELAY_TIME 100

#define ATC_RESPONSE_MSG(atc_config_ptr, msg)   sprintf((char *)g_rsp_str, "+CME Error:%s", msg)

typedef struct
{
    uint16  cmd_id;
    uint16  arfcn;
    uint16  gainpower;
    uint8  ms_band;
    BOOLEAN  is_ps_active;    //to store the ps state(active or deactive) before max power transmission    
    MN_DUAL_SYS_E dual_sys;
    BOOLEAN sim_insert[MN_DUAL_SYS_MAX];
}ATC_POWER_INF_T;

typedef struct
{
    uint8 mode;
    uint16 voice_format;
}ATC_LOOP_TEST_INFO_T;

typedef struct
{
    uint8        param_len;
    uint8        param[ATC_PRODUCTION_LINE_CMD_MAX_LEN];
}ATC_PRODUCTION_LINE_PARAM_T;

typedef struct
{
    uint8 id;
    uint8 result_code;
}ATC_FACTORY_ITEM_T;

typedef struct
{
    uint8 id[3];
    uint8 result_code;
}ATC_FACTORY_ITEM_RSP_T;

typedef enum
{
    ATC_READ_IMEI = 1,
    ATC_WRITE_IMEI_1 = 2,
    ATC_WRITE_IMEI_2 = 4,
    ATC_OPERATION_INVALID
}ATC_IMEI_OPERATION_TYPE;

typedef enum
{
    ATC_EGSM900,
    ATC_DCS1800,
    ATC_PCS1900 = 3,
    ATC_GSM850,
    ATC_INVALID_BAND
}ATC_BAND_E;


/*****************************************************************************/
//  Description : This function handle the AT+HEADINFO command 
//                 headinfo for phone information
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessHEADINFO);

/*****************************************************************************/
//  Description : This function handle the AT+SYSSLEEP command 
//                 set phone to sleep/wakeup
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSYSSLEEP);

/*****************************************************************************/
//  Description : This function handle the AT+FCEPTEST command 
//                 Ear phone test
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCEPTEST);

/*****************************************************************************/
//  Description : This function handle the AT+TEMPTEST command 
//                 Temperature test
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessTEMPTEST);

/*****************************************************************************/
//  Description : This function handle the AT+SPKSTEST command 
//                 Speaker test & ringtone play
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPKSTEST);

/*****************************************************************************/
//  Description : This function handle the AT+KEYSHORT command 
//                 check whether key is pressed or not
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessKEYSHORT);

/*****************************************************************************/
//  Description : This function handle the AT+IMEITEST command 
//                 IMEI read write 
//  Global resource dependence : none
//  Author:longting zhao
//  Note: the input IMEI value should be 14 digits, not including the last check digit,
//           the check digit should be calculated and appended to the 15th digit.
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessIMEITEST);

/*****************************************************************************/
//  Description : This function handle the AT+RSTVERIF command 
//                 Factory Reset Verify
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessRSTVERIF);

/*****************************************************************************/
//  Description : This function handle the AT+GETTESTNV command 
//                 Get Test status in NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGETTESTNV);

/*****************************************************************************/
//  Description : This function handle the AT+SETTESTNV command 
//                 Set test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/          
AT_CMD_FUNC(ATC_ProcessSETTESTNV);

/*****************************************************************************/
//  Description : This function handle the AT+GETFULLHISTNV command 
//                 Get all history test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/          
AT_CMD_FUNC(ATC_ProcessGETFULLHISTNV);

/*****************************************************************************/
//  Description : This function handle the AT+LOGERASE command 
//                 Erase history test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/          
AT_CMD_FUNC(ATC_ProcessLOGERASE);

/*****************************************************************************/
//  Description : This function handle the AT+GETFULLTESTNV command 
//                 Get full test NV value
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGETFULLTESTNV);

/*****************************************************************************/
//  Description : This function handle the AT+LOOPTEST command 
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessLOOPTEST);

/*****************************************************************************/
//  Description : This function handle the AT+BATTTEST command 
//                    battery status test
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessBATTTEST);

/*****************************************************************************/
//  Description : This function handle the AT+MAXPOWER command
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessMAXPOWER);

/*****************************************************************************/
//  Description : This function handle the AT+READRSSI command
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessREADRSSI);

/*****************************************************************************/
//  Description : This function handle the AT+LOCKREAD command
//  Global resource dependence : g_rsp_str
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessLOCKREAD);
/*****************************************************************************/
//  Description : This function handle the AT+LVOFLOCK command
//  Global resource dependence : g_rsp_str
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessLVOFLOCK);
/*****************************************************************************/
//  Description : This function handle the AT+DETALOCK command
//  Global resource dependence : g_rsp_str
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessDETALOCK);

/*****************************************************************************/
// Description : This function handle the AT+KSTRINGB command 
//                 Key string block
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessKSTRINGB);

/*****************************************************************************/
// Description : This function handle the AT+MSLSECUR command 
//                 MSL security 
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMSLSECUR);

/*****************************************************************************/
// Description : This function handle the AT+CALIDATE command 
//                 calibration date
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCALIDATE);

/*****************************************************************************/
// Description : This function handle the AT+SIMDETEC command 
//                 Detect if sim card exists
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSIMDETEC);

/*****************************************************************************/
// Description : This function handle the AT+SERIALNO command 
//                 Serial number read/write
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSERIALNO);

/*****************************************************************************/
// Description : This function handle the AT+FACTORST command 
//                 Factory Reset
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFACTORST);

/*****************************************************************************/
// Description : This function handle the AT+AKSEEDNO command 
//                 AK Seed Number read write 
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessAKSEEDNO);

/*****************************************************************************/
// Description : This function handle the AT+MISCD command 
//                 get the status of SMCD
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMISCD);

/*****************************************************************************/
// Description : This function handle the AT+EMEMTEST command 
//                 get the status of external memory
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessEMEMTEST);

/*****************************************************************************/
// Description : This function handle the AT+FCMPTEST command 
//                 start/stop play mp3
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCMPTEST);

/*****************************************************************************/
// Description : This function handle the AT+FCBTTEST command 
//                 switch BT function, search device and give research result within 10 seconds
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCBTTEST);

/*****************************************************************************/
// Description : This function handle the AT+SETFDATA command 
//                 set the value of the index passed as first parameter
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSETFDATA);

/*****************************************************************************/
// Description : This function handle the AT+SETFULLFDATA command 
//                 set the data of the all structure members
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSETFULLFDATA);

/*****************************************************************************/
// Description : This function handle the AT+GETFULLFDATA command 
//                 get the data of the all structure members
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGETFULLFDATA);

/*****************************************************************************/
// Description : This function handle the AT+GETFDATA command 
//                 get the value of the structure member passed as the parameter
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGETFDATA);

/*****************************************************************************/
//  Description : This function used to start the test of GSM Max Power Transmission
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_StartGsmTxPwrTest(ATC_CONFIG_T *atc_config_ptr);

/*****************************************************************************/
//  Description : This function used to start the test of GSM Receive Power
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_StartGsmRxPwrTest(ATC_CONFIG_T *atc_config_ptr);

//  Description : This function used to start the test of GSM Receive Power
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_RssiReadExpInd(ATC_CONFIG_T *atc_config_ptr);

/*****************************************************************************/
//  Description : This function used to get BT status: On/Off
//  Global resource dependence : g_rsp_str
//  Author: longting.zhao
//  Note:
/*****************************************************************************/
void ATC_GetBtStatus(ATC_CONFIG_T *atc_config_ptr);

/*****************************************************************************/
//  Description : This function used to get BT Searched device count
//  Global resource dependence : g_rsp_str
//  Author: longting.zhao
//  Note:
/*****************************************************************************/
void ATC_GetBtDeviceCnt(ATC_CONFIG_T *atc_config_ptr);

/*****************************************************************************/
//  Description : This function is used to handle looptest functionalities,
//                     when receving APP_MN_PS_POWER_OFF_CNF
//  Global resource dependence : AT+LOOPTEST is executed
//  Author:       longting zhao
//  Note:
/*****************************************************************************/
void ATC_LoopTestPsOffCnf(ATC_CONFIG_T *atc_config_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif/*_ATC_SAMSUNG_PROD_H_*/
