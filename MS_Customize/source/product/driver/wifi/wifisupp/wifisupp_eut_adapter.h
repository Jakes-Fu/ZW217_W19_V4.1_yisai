/*****************************************************************************
** File Name:      wifisupp_eut_adapter.h                                    *
** Author:         bruce.chi                                                 *
** Date:           5/18/2010                                                 *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used for wifi eut adapter definition         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 5/18/2010     bruce.chi         Create                                     *
******************************************************************************/

#ifndef _WIFISUPP_EUT_ADAPTER_H_
#define _WIFISUPP_EUT_ADAPTER_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define WIFISUPP_COMMAND_MAX_LEN    1024
#define WIFI_MAC_ADDR_LEN           6

/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/

typedef void *WIFISUPP_EUT_HANDLE_T;

typedef unsigned char WIFISUPP_BOOL;
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Global variables                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
//Description : enter eut mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTEnterMode(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_enter);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTEnterModeCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description : set channel 1~13
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetChannel(WIFISUPP_EUT_HANDLE_T handle, long channel);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetChannelCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description : set data ratio, 1Mbps, 2Mbps, 5.5, 11. 6, 9, 12, 18, 24, 36, 48, 54Mbps
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetDataRatio(WIFISUPP_EUT_HANDLE_T handle, long ratio);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetDataRatioCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description : set wifi mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetMode(WIFISUPP_EUT_HANDLE_T handle, WIFI_MODE_E mode);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetModeCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description : set tx factor
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTxFactor(WIFISUPP_EUT_HANDLE_T handle, long pcl, long tx_factor);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTxFactorCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description :  set pcl
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetPCL(WIFISUPP_EUT_HANDLE_T handle, long pcl);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetPCLCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description : set tx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTx(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_tx_on, int tx_packet_mode);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTxCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description : set rx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetRx(WIFISUPP_EUT_HANDLE_T handle, int rx_command);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetRxCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

/****************************************************************************/
//Description : read rx total packets and error packets
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadRxVal(WIFISUPP_EUT_HANDLE_T handle);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadRxValCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success, uint32 total_packets, uint32 err_packets);

#ifdef WIFI_VENDOR_REALTEK
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTCusData(WIFISUPP_EUT_HANDLE_T handle, uint* pBuf);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTCusDataCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success, uint *pBuf);
#endif

#ifdef WIFI_SUPPORT_AR6302
/****************************************************************************/
//Description : set crystal cap value in wifi chip
//Global resource dependence : 
//Author: George.Liu
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetCrystalCap(WIFISUPP_EUT_HANDLE_T handle, uint8 szCrystalCap[]);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetCrystalCapCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadCalData(WIFISUPP_EUT_HANDLE_T handle);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadCalDataCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success, const char* pBuf);

PUBLIC WIFISUPP_BOOL WIFISUPP_EUTWriteCalData(WIFISUPP_EUT_HANDLE_T handle, const char* pBuf);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTWriteCalDataCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success);

#endif


#ifdef WIFI_VENDOR_CSR
/****************************************************************************/
//Description : support CSR uniTest tools
//Global resource dependence : 
//Author: George.Liu
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadRegister(WIFISUPP_EUT_HANDLE_T handle, uint32 addr, uint32 *pData);
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTWriteRegister(WIFISUPP_EUT_HANDLE_T handle, uint32 addr, uint32 Data);

#endif

#ifdef   __cplusplus
    }
#endif


#endif 




