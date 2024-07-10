/****************************************************************************************
** File Name:      wifieut_main.c                                                       *
** Author:         bruce.chi                                                            *
** Date:           20010.5.18                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is used for wifi supplicant eut functions' implementation  *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2010.5       bruce.chi         Create                                                *
**                                                                                      *
*****************************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "wifisupp_eut.h"
#include "wifisupp_eut_adapter.h"
#include "rtl8723_wifi_adapter.h"
#include "tcpip_types.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//Description : enter eut mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTEnterMode(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_enter)
{
#if 1
	rtl8723_set_eut_mode(is_enter);

	//SCI_TraceLow:"WIFISUPP_EUTEnterMode %d\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_REALTEK_58_112_2_18_0_36_19_2258,(uint8*)"d", is_enter);
	WIFISUPP_EUTEnterModeCnf(handle, 1);
#endif
    return TRUE;
}

/****************************************************************************/
//Description : set channel 1~13
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetChannel(WIFISUPP_EUT_HANDLE_T handle, long channel)
{
#if 1
    rtl8723_set_eut_rxtx_channel(channel);
    WIFISUPP_EUTSetChannelCnf(handle, 1);
#endif
    return TRUE;
}

/****************************************************************************/
//Description : set data ratio, 1Mbps, 2Mbps, 5.5, 11. 6, 9, 12, 18, 24, 36, 48, 54Mbps
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetDataRatio(WIFISUPP_EUT_HANDLE_T handle, long ratio)
{
#if 1
    rtl8723_set_eut_tx_rate(ratio);
    WIFISUPP_EUTSetDataRatioCnf(handle, 1);
#endif
    return TRUE;
}

/****************************************************************************/
//Description : set wifi mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetMode(WIFISUPP_EUT_HANDLE_T handle, WIFI_MODE_E mode)
{
    WIFISUPP_EUTSetModeCnf(handle, 1);
    return TRUE;
}

/****************************************************************************/
//Description : set tx factor
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTxFactor(WIFISUPP_EUT_HANDLE_T handle, long pcl, long tx_factor)
{
#if 1
    //SCI_TRACE_LOW:"WIFISUPP_EUTSetTxFactor pcl=%d, tx_factor=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_REALTEK_107_112_2_18_0_36_19_2259,(uint8*)"dd", pcl, tx_factor);
    
    rtl8723_set_eut_tx_pwr(tx_factor);
    WIFISUPP_EUTSetTxFactorCnf(handle, 1);
#endif
    return TRUE;
}

/****************************************************************************/
//Description :  set pcl
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetPCL(WIFISUPP_EUT_HANDLE_T handle, long pcl)
{
    WIFISUPP_EUTSetPCLCnf(handle, 1);
    return TRUE;
}

/****************************************************************************/
//Description : set tx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTx(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_tx_on, int tx_packet_mode)
{
#if 1
    rtl8723_eut_set_TX_param(is_tx_on, tx_packet_mode);
    WIFISUPP_EUTSetTxCnf(handle, 1);
#endif
    return TRUE;
}

/****************************************************************************/
//Description : set rx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetRx(WIFISUPP_EUT_HANDLE_T handle, int rx_command)
{
#if 1
    rtl8723_eut_set_RX_param(rx_command);
    WIFISUPP_EUTSetRxCnf(handle, 1);
#endif
    return TRUE;
}

/****************************************************************************/
//Description : read rx total packets and error packets
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadRxVal(WIFISUPP_EUT_HANDLE_T handle)
{
    uint32 total_packets = 0;
    uint32 err_packets = 0;
#if 1    
    rtl8723_eut_get_RX_packets(&total_packets, &err_packets);
    total_packets = htonl(total_packets);
    err_packets = htonl(err_packets);
    WIFISUPP_EUTReadRxValCnf(handle, 1, total_packets, err_packets);
#endif
    return TRUE;
}

#ifdef AMY_TODO
extern unsigned int eeprom_data_len;
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadCalData(WIFISUPP_EUT_HANDLE_T handle)
{
    char* pBuf = NULL;
    char ret = 0;
    pBuf = malloc(eeprom_data_len);
    if (pBuf)
        ret = rtl8723_eut_get_cal_data(pBuf, eeprom_data_len);

    WIFISUPP_EUTReadCalDataCnf(handle, ret, pBuf);
    if (pBuf)
        free(pBuf);
    return TRUE;
}

PUBLIC WIFISUPP_BOOL WIFISUPP_EUTWriteCalData(WIFISUPP_EUT_HANDLE_T handle, const char* pBuf)
{
    char ret = 0;
    ret = rtl8723_eut_update_cal_data(pBuf, eeprom_data_len);

    WIFISUPP_EUTWriteCalDataCnf(handle, ret);

    return TRUE;
}
#endif
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTCusData(WIFISUPP_EUT_HANDLE_T handle, uint* pBuf)
{
#if 1
//	uint32 rxok = 0;
//    	uint32 rxfail = 0;
	rtl8723_eut_custom_data(pBuf);

	//SCI_TraceLow:"WIFISUPP_EUTCusData\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_REALTEK_201_112_2_18_0_36_19_2260,(uint8*)"");
	WIFISUPP_EUTCusDataCnf(handle, 1, pBuf);
#endif
    return TRUE;
}
