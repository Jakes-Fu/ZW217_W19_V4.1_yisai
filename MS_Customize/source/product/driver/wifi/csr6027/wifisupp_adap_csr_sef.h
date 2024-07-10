#ifndef WIFISUPP_ADAP_CSR_SEF_H__
#define WIFISUPP_ADAP_CSR_SEF_H__
/****************************************************************************************
** File Name:      wifisupp_adap_csr_sef.c                                                  *
** Author:         lasse.bigum                                                          *
** Date:           2009.12.15                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    CSR Synergy WiFi Supplicant implementation for Spreadtrum platform   *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.12        lasse.bigum      Create                                               *
**                                                                                      *
*****************************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
/* Platform includes */
#include "sci_api.h"
#include "wifisupp_internal.h"

/* CSR includes */
#include "csr_types.h"

/* Wifi libs and types */
#include "csr_wifi_sme_prim.h"
#include "csr_wifi_sme_lib.h"
#include "csr_wifi_nme_prim.h"
#include "csr_wifi_nme_lib.h"

// Enable the use of NME auto-connect
//#define CSR_WIFI_NME_AUTOCONNECT

#define RSSI_POLL_INTERVAL_MS 6000

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//customized signals are added here
#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define CSR_DEFAULT_STACK_SIZE    (4 * 1024)
#define CSR_DEFAULT_QUEUE_NUM     (50)  // Was 200, Bruce recommended to set this to 50
#define CSR_DEFAULT_PRIORITY      (30)   //30
#define CSR_HIGH_PRIORITY         (16)   //16
#endif

/* Uniquely specified queue id for CSR Synergy WiFi supplicant */
#define CSR_WIFI_SUPPLICANT_IFACEQUEUE (CsrSchedQid) 0x7801

extern CsrBool csrWifiEnableStrictComplianceMode;

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    CSR_WIFI_SUPP_MESSAGE_START = WIFISUPP_SIG_CUSTOM_START,
    CSR_WIFI_SUPP_MESSAGE_MAX = WIFISUPP_SIG_CUSTOM_END
} CSR_WIFI_SUPP_MESSAGE_E;

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct {
    _SIGNAL_VARS    // This is just a macro that expands to the necessary parameters that a signal *must* have
    void *message;
} CsrMessageSignalType;

typedef struct {
    CsrBool wifiOn; // Indicates if wifi is on or not
    WIFISUPP_SSID_CONFIG_T *connectReq;  // Store currently pending connection request
    CsrBool sharedConnect; // Flag for indicating if the connection attempt is the first or second
    WIFISUPP_DISCONNECT_RESULT_T *disconnectReq;  // Store currently pending disconnect request

    CsrWifiSmeConnectionInfoGetCfm *currentConnectionInfo;  // Store information on current connection
    CsrWifiSmeConnectionQualityInd *currentConnectionQuality; // Store current connection quality

    //WIFISUPP_AUTOSCAN_REQ_INFO_T *autoScan_info_ptr;
#ifdef CSR_WIFI_NME_AUTOCONNECT
    /* Data needed for ProfileOrderSet - auto-connect feature */
    CsrUint8 profileIdentitysCount;
    CsrWifiNmeProfileIdentity *profileIdentitys;
#endif
    WIFISUPP_SCAN_REQ_INFO_T *scan_req_info_ptr;
    WIFISUPP_COMMAND_INFO_T *command_info_ptr;
    void * recvMessage;
} csrWifiSupplicantInstanceData;

CsrUint32 getRate(CsrUint16 informationElementsLength, CsrUint8 *informationElements);
CsrUint8 getWepKeyIndex(WIFISUPP_SSID_CONFIG_T *ssidConfig);

typedef void (*CsrWifiSmeStateHandlerType)(csrWifiSupplicantInstanceData *instanceData);

typedef enum {
	RSSI_POLLING_TIMER_DEACTIVE = 0,
	RSSI_POLLING_TIMER_INIT,
	RSSI_POLLING_TIMER_ACTIVE,
	RSSI_POLLING_TIMER_CANCEL
}CsrRssiPollTimerStatus;

#endif

