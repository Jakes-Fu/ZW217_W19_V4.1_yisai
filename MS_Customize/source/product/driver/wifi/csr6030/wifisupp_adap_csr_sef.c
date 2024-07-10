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


#ifdef CSR_WIFI_6030
/*
 * Enable support for EAP-SIM and AKA supplicant
 * this should be enabled in the projects files, before product release
 */
//#define CSR_WIFI_SUPPORT_EAP_SIM_AKA_SUPPLICANT
#endif
               

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
/* Platform includes */
//#include "sci_api.h"
#include "wifisupp_internal.h"

/* CSR includes */
#include "csr_types.h"
#include "csr_pmem.h"

/* Wifi libs and types */
#include "csr_wifi_sme_prim.h"
#include "csr_wifi_sme_lib.h"
#include "csr_wifi_nme_prim.h"
#include "csr_wifi_nme_lib.h"

#ifdef CSR_WIFI_6030
#include "tcpip_types.h"
#ifdef MIFI_SUPPORT
#include "mifisupp_api.h"
#endif
#ifdef CSR_WIFI_AP_ENABLE
#include "csr_wifi_nme_ap_lib.h"
#endif
#else
#include "csr_wifi_mib_repository.h"
#endif

#include "wifisupp_adap_csr_sef.h"

/*lint -e835*/
/*lint -e838*/
static void csrWifiSuppSmeActivateCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeWifiOnCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeWifiOffCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeWifiOffIndHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmePermanentMacAddressGetCfmHandler(csrWifiSupplicantInstanceData * instData);
#if 0
static void csrWifiSuppSmeScanConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeScanConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
#endif
static void csrWifiSuppSmePowerConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmePowerConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeRoamingConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeRoamingConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeScanFullCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeScanResultsGetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeConnectionInfoGetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeConnectionQualityIndHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeDisconnectCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeHostConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeHostConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeEventMaskSetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeIbssStationIndHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeLinkQualityGetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeMediaStatusIndHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppSmeVersionsGetCfmHandler(csrWifiSupplicantInstanceData * instData);

static void csrWifiSuppNmeProfileSetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppNmeProfileOrderSetCfmHandler(csrWifiSupplicantInstanceData * instData);
static void csrWifiSuppNmeProfileConnectCfmHandler(csrWifiSupplicantInstanceData * instData);
#ifdef CSR_WIFI_6030
#ifdef CSR_WIFI_SUPPORT_EAP_SIM_AKA_SUPPLICANT
static void csrWifiSuppNmeSimImsiGetIndHandler(csrWifiSupplicantInstanceData *instData);
static void csrWifiSuppNmeSimGsmAuthIndHandler(csrWifiSupplicantInstanceData *instData);
static void csrWifiSuppNmeSimUmtsAuthIndHandler(csrWifiSupplicantInstanceData *instData);
#endif
static void csrWifiSuppNmeEventMaskSetCfmHandler(csrWifiSupplicantInstanceData *instData);
#endif

extern CsrCharString CsrWiFiRelVer[128];
static CsrBool first_ver_get = TRUE;

extern CsrRssiPollTimerStatus CsrRSSITimerBeSet;
extern int wifi_supp_is_on;
#ifdef CSR_WIFI_6030
extern TCPIP_NETID_T csrGlobalIpEtherContext;
#else
extern CsrUint32 *csrGlobalIpEtherContext;
#endif
extern void CsrDebugTraceLow(const char* format, ...);
extern void CsrWifiOffVarsDeInit(void);

#ifdef CSR_WIFI_6030
const CsrWifiSmeStateHandlerType CsrWifiSuppSmeHandlers[CSR_WIFI_SME_PRIM_UPSTREAM_COUNT] =
{
    csrWifiSuppSmeActivateCfmHandler,               /* CSR_WIFI_SME_ACTIVATE_CFM                   */
    NULL,                                           /* CSR_WIFI_SME_ADHOC_CONFIG_GET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_ADHOC_CONFIG_SET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_ASSOCIATION_COMPLETE_IND       */
    NULL,                                           /* CSR_WIFI_SME_ASSOCIATION_START_IND          */
    NULL,                                           /* CSR_WIFI_SME_BLACKLIST_CFM                  */
    NULL,                                           /* CSR_WIFI_SME_CALIBRATION_DATA_GET_CFM       */
    NULL,                                           /* CSR_WIFI_SME_CALIBRATION_DATA_SET_CFM       */
    NULL,                                           /* CSR_WIFI_SME_CCX_CONFIG_GET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_CCX_CONFIG_SET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_COEX_CONFIG_GET_CFM            */
    NULL,                                           /* CSR_WIFI_SME_COEX_CONFIG_SET_CFM            */
    NULL,                                           /* CSR_WIFI_SME_COEX_INFO_GET_CFM              */
    NULL,                                           /* CSR_WIFI_SME_CONNECT_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_CONNECTION_CONFIG_GET_CFM      */
    csrWifiSuppSmeConnectionInfoGetCfmHandler,      /* CSR_WIFI_SME_CONNECTION_INFO_GET_CFM        */
    csrWifiSuppSmeConnectionQualityIndHandler,      /* CSR_WIFI_SME_CONNECTION_QUALITY_IND         */
    NULL,                                           /* CSR_WIFI_SME_CONNECTION_STATS_GET_CFM       */
    NULL,                                           /* CSR_WIFI_SME_DEACTIVATE_CFM                 */
    csrWifiSuppSmeDisconnectCfmHandler,             /* CSR_WIFI_SME_DISCONNECT_CFM                 */
    csrWifiSuppSmeEventMaskSetCfmHandler,           /* CSR_WIFI_SME_EVENT_MASK_SET_CFM             */
    csrWifiSuppSmeHostConfigGetCfmHandler,          /* CSR_WIFI_SME_HOST_CONFIG_GET_CFM            */
    csrWifiSuppSmeHostConfigSetCfmHandler,          /* CSR_WIFI_SME_HOST_CONFIG_SET_CFM            */
    csrWifiSuppSmeIbssStationIndHandler,            /* CSR_WIFI_SME_IBSS_STATION_IND               */
    NULL,                                           /* CSR_WIFI_SME_KEY_CFM                        */
    csrWifiSuppSmeLinkQualityGetCfmHandler,         /* CSR_WIFI_SME_LINK_QUALITY_GET_CFM           */
    csrWifiSuppSmeMediaStatusIndHandler,            /* CSR_WIFI_SME_MEDIA_STATUS_IND               */
    NULL,                                           /* CSR_WIFI_SME_MIB_CONFIG_GET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_MIB_CONFIG_SET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_MIB_GET_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_MIB_GET_NEXT_CFM               */
    NULL,                                           /* CSR_WIFI_SME_MIB_SET_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_MIC_FAILURE_IND                */
    NULL,                                           /* CSR_WIFI_SME_MULTICAST_ADDRESS_CFM          */
    NULL,                                           /* CSR_WIFI_SME_PACKET_FILTER_SET_CFM          */
    csrWifiSuppSmePermanentMacAddressGetCfmHandler, /* CSR_WIFI_SME_PERMANENT_MAC_ADDRESS_GET_CFM  */
    NULL,                                           /* CSR_WIFI_SME_PMKID_CANDIDATE_LIST_IND       */
    NULL,                                           /* CSR_WIFI_SME_PMKID_CFM                      */
    csrWifiSuppSmePowerConfigGetCfmHandler,         /* CSR_WIFI_SME_POWER_CONFIG_GET_CFM           */
    csrWifiSuppSmePowerConfigSetCfmHandler,         /* CSR_WIFI_SME_POWER_CONFIG_SET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_REGULATORY_DOMAIN_INFO_GET_CFM */
    NULL,                                           /* CSR_WIFI_SME_ROAM_COMPLETE_IND              */
    NULL,                                           /* CSR_WIFI_SME_ROAM_START_IND                 */
    csrWifiSuppSmeRoamingConfigGetCfmHandler,       /* CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM         */
    csrWifiSuppSmeRoamingConfigSetCfmHandler,       /* CSR_WIFI_SME_ROAMING_CONFIG_SET_CFM         */
    NULL,                                           /* CSR_WIFI_SME_SCAN_CONFIG_GET_CFM            */
    NULL,                                           /* CSR_WIFI_SME_SCAN_CONFIG_SET_CFM            */
    csrWifiSuppSmeScanFullCfmHandler,               /* CSR_WIFI_SME_SCAN_FULL_CFM                  */
    NULL,                                           /* CSR_WIFI_SME_SCAN_RESULT_IND                */
    NULL,                                           /* CSR_WIFI_SME_SCAN_RESULTS_FLUSH_CFM         */
    csrWifiSuppSmeScanResultsGetCfmHandler,         /* CSR_WIFI_SME_SCAN_RESULTS_GET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_SME_STA_CONFIG_GET_CFM         */
    NULL,                                           /* CSR_WIFI_SME_SME_STA_CONFIG_SET_CFM         */
    NULL,                                           /* CSR_WIFI_SME_STATION_MAC_ADDRESS_GET_CFM    */
    NULL,                                           /* CSR_WIFI_SME_TSPEC_IND                      */
    NULL,                                           /* CSR_WIFI_SME_TSPEC_CFM                      */
    csrWifiSuppSmeVersionsGetCfmHandler,            /* CSR_WIFI_SME_VERSIONS_GET_CFM               */
    NULL,                                           /* CSR_WIFI_SME_WIFI_FLIGHTMODE_CFM            */
    csrWifiSuppSmeWifiOffIndHandler,                /* CSR_WIFI_SME_WIFI_OFF_IND                   */
    csrWifiSuppSmeWifiOffCfmHandler,                /* CSR_WIFI_SME_WIFI_OFF_CFM                   */
    csrWifiSuppSmeWifiOnCfmHandler,                 /* CSR_WIFI_SME_WIFI_ON_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_CLOAKED_SSIDS_SET_CFM          */
    NULL,                                           /* CSR_WIFI_SME_CLOAKED_SSIDS_GET_CFM          */
    NULL,                                           /* CSR_WIFI_SME_WIFI_ON_IND                    */
    NULL,                                           /* CSR_WIFI_SME_SME_COMMON_CONFIG_GET_CFM      */
    NULL,                                           /* CSR_WIFI_SME_SME_COMMON_CONFIG_SET_CFM      */
    NULL,                                           /* CSR_WIFI_SME_INTERFACE_CAPABILITY_GET_CFM   */
    NULL,                                           /* CSR_WIFI_SME_ERROR_IND                      */
    NULL,                                           /* CSR_WIFI_SME_INFO_IND                       */
    NULL,                                           /* CSR_WIFI_SME_CORE_DUMP_IND                  */
    NULL,                                           /* CSR_WIFI_SME_AMP_STATUS_CHANGE_IND          */
    NULL,                                           /* CSR_WIFI_SME_WPS_CONFIGURATION_CFM          */
};

const CsrWifiSmeStateHandlerType CsrWifiSuppNmeHandlers[CSR_WIFI_NME_PRIM_UPSTREAM_COUNT] =
{
    csrWifiSuppNmeProfileSetCfmHandler,             /* CSR_WIFI_NME_PROFILE_SET_CFM                */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_DELETE_CFM             */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_DELETE_ALL_CFM         */
    csrWifiSuppNmeProfileOrderSetCfmHandler,        /* CSR_WIFI_NME_PROFILE_ORDER_SET_CFM          */
    csrWifiSuppNmeProfileConnectCfmHandler,         /* CSR_WIFI_NME_PROFILE_CONNECT_CFM            */
    NULL,                                           /* CSR_WIFI_NME_WPS_CFM                        */
    NULL,                                           /* CSR_WIFI_NME_WPS_CANCEL_CFM                 */
    NULL,                                           /* CSR_WIFI_NME_CONNECTION_STATUS_GET_CFM      */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_UPDATE_IND             */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_DISCONNECT_IND         */
#ifdef CSR_WIFI_SUPPORT_EAP_SIM_AKA_SUPPLICANT
    csrWifiSuppNmeSimImsiGetIndHandler,             /* CSR_WIFI_NME_SIM_IMSI_GET_IND               */
    csrWifiSuppNmeSimGsmAuthIndHandler,             /* CSR_WIFI_NME_SIM_GSM_AUTH_IND               */
    csrWifiSuppNmeSimUmtsAuthIndHandler,            /* CSR_WIFI_NME_SIM_UMTS_AUTH_IND              */
#else
    NULL,                                           /* CSR_WIFI_NME_SIM_IMSI_GET_IND               */
    NULL,                                           /* CSR_WIFI_NME_SIM_GSM_AUTH_IND               */
    NULL,                                           /* CSR_WIFI_NME_SIM_UMTS_AUTH_IND              */
#endif
    NULL,                                           /* CSR_WIFI_NME_WPS_CONFIG_SET_CFM             */                   
    csrWifiSuppNmeEventMaskSetCfmHandler,           /* CSR_WIFI_NME_EVENT_MASK_SET_CFM             */                    
};
#else
const CsrWifiSmeStateHandlerType CsrWifiSuppSmeHandlers[CSR_WIFI_SME_PRIM_UPSTREAM_COUNT] =
{
    csrWifiSuppSmeActivateCfmHandler,               /* CSR_WIFI_SME_ACTIVATE_CFM                   */
    NULL,                                           /* CSR_WIFI_SME_ADHOC_CONFIG_GET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_ADHOC_CONFIG_SET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_ASSOCIATION_COMPLETE_IND       */
    NULL,                                           /* CSR_WIFI_SME_ASSOCIATION_START_IND          */
    NULL,                                           /* CSR_WIFI_SME_BLACKLIST_CFM                  */
    NULL,                                           /* CSR_WIFI_SME_CALIBRATION_DATA_GET_CFM       */
    NULL,                                           /* CSR_WIFI_SME_CALIBRATION_DATA_SET_CFM       */
    NULL,                                           /* CSR_WIFI_SME_CCX_CONFIG_GET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_CCX_CONFIG_SET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_COEX_CONFIG_GET_CFM            */
    NULL,                                           /* CSR_WIFI_SME_COEX_CONFIG_SET_CFM            */
    NULL,                                           /* CSR_WIFI_SME_COEX_INFO_GET_CFM              */
    NULL,                                           /* CSR_WIFI_SME_CONNECT_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_CONNECTION_CONFIG_GET_CFM      */
    csrWifiSuppSmeConnectionInfoGetCfmHandler,      /* CSR_WIFI_SME_CONNECTION_INFO_GET_CFM        */
    csrWifiSuppSmeConnectionQualityIndHandler,      /* CSR_WIFI_SME_CONNECTION_QUALITY_IND         */
    NULL,                                           /* CSR_WIFI_SME_CONNECTION_STATS_GET_CFM       */
    NULL,                                           /* CSR_WIFI_SME_DEACTIVATE_CFM                 */
    csrWifiSuppSmeDisconnectCfmHandler,             /* CSR_WIFI_SME_DISCONNECT_CFM                 */
    csrWifiSuppSmeEventMaskSetCfmHandler,           /* CSR_WIFI_SME_EVENT_MASK_SET_CFM             */
    csrWifiSuppSmeHostConfigGetCfmHandler,          /* CSR_WIFI_SME_HOST_CONFIG_GET_CFM            */
    csrWifiSuppSmeHostConfigSetCfmHandler,          /* CSR_WIFI_SME_HOST_CONFIG_SET_CFM            */
    csrWifiSuppSmeIbssStationIndHandler,            /* CSR_WIFI_SME_IBSS_STATION_IND               */
    NULL,                                           /* CSR_WIFI_SME_KEY_CFM                        */
    csrWifiSuppSmeLinkQualityGetCfmHandler,         /* CSR_WIFI_SME_LINK_QUALITY_GET_CFM           */
    csrWifiSuppSmeMediaStatusIndHandler,            /* CSR_WIFI_SME_MEDIA_STATUS_IND               */
    NULL,                                           /* CSR_WIFI_SME_MIB_CONFIG_GET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_MIB_CONFIG_SET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_MIB_GET_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_MIB_GET_NEXT_CFM               */
    NULL,                                           /* CSR_WIFI_SME_MIB_SET_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_MIC_FAILURE_IND                */
    NULL,                                           /* CSR_WIFI_SME_MULTICAST_ADDRESS_CFM          */
    NULL,                                           /* CSR_WIFI_SME_PACKET_FILTER_SET_CFM          */
    csrWifiSuppSmePermanentMacAddressGetCfmHandler, /* CSR_WIFI_SME_PERMANENT_MAC_ADDRESS_GET_CFM  */
    NULL,                                           /* CSR_WIFI_SME_PMKID_CANDIDATE_LIST_IND       */
    NULL,                                           /* CSR_WIFI_SME_PMKID_CFM                      */
    csrWifiSuppSmePowerConfigGetCfmHandler,         /* CSR_WIFI_SME_POWER_CONFIG_GET_CFM           */
    csrWifiSuppSmePowerConfigSetCfmHandler,         /* CSR_WIFI_SME_POWER_CONFIG_SET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_REGULATORY_DOMAIN_INFO_GET_CFM */
    NULL,                                           /* CSR_WIFI_SME_ROAM_COMPLETE_IND              */
    NULL,                                           /* CSR_WIFI_SME_ROAM_START_IND                 */
    csrWifiSuppSmeRoamingConfigGetCfmHandler,       /* CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM         */
    csrWifiSuppSmeRoamingConfigSetCfmHandler,       /* CSR_WIFI_SME_ROAMING_CONFIG_SET_CFM         */
    NULL,                                           /* CSR_WIFI_SME_SCAN_CONFIG_GET_CFM            */
    NULL,                                           /* CSR_WIFI_SME_SCAN_CONFIG_SET_CFM            */
    csrWifiSuppSmeScanFullCfmHandler,               /* CSR_WIFI_SME_SCAN_FULL_CFM                  */
    NULL,                                           /* CSR_WIFI_SME_SCAN_RESULT_IND                */
    NULL,                                           /* CSR_WIFI_SME_SCAN_RESULTS_FLUSH_CFM         */
    csrWifiSuppSmeScanResultsGetCfmHandler,         /* CSR_WIFI_SME_SCAN_RESULTS_GET_CFM           */
    NULL,                                           /* CSR_WIFI_SME_SME_CONFIG_GET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_SME_CONFIG_SET_CFM             */
    NULL,                                           /* CSR_WIFI_SME_STATION_MAC_ADDRESS_GET_CFM    */
    NULL,                                           /* CSR_WIFI_SME_TSPEC_IND                      */
    NULL,                                           /* CSR_WIFI_SME_TSPEC_CFM                      */
    csrWifiSuppSmeVersionsGetCfmHandler,            /* CSR_WIFI_SME_VERSIONS_GET_CFM               */
    NULL,                                           /* CSR_WIFI_SME_WIFI_FLIGHTMODE_CFM            */
    csrWifiSuppSmeWifiOffIndHandler,                /* CSR_WIFI_SME_WIFI_OFF_IND                   */
    csrWifiSuppSmeWifiOffCfmHandler,                /* CSR_WIFI_SME_WIFI_OFF_CFM                   */
    csrWifiSuppSmeWifiOnCfmHandler,                 /* CSR_WIFI_SME_WIFI_ON_CFM                    */
    NULL,                                           /* CSR_WIFI_SME_RESTRICTED_ACCESS_ENABLE_CFM   */
    NULL,                                           /* CSR_WIFI_SME_RESTRICTED_ACCESS_DISABLE_CFM  */
    NULL,                                           /* CSR_WIFI_SME_CLOAKED_SSIDS_SET_CFM          */
    NULL,                                           /* CSR_WIFI_SME_CLOAKED_SSIDS_GET_CFM          */
};

const CsrWifiSmeStateHandlerType CsrWifiSuppNmeHandlers[CSR_WIFI_NME_PRIM_UPSTREAM_COUNT] =
{
    csrWifiSuppNmeProfileSetCfmHandler,             /* CSR_WIFI_NME_PROFILE_SET_CFM                */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_DELETE_CFM             */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_DELETE_ALL_CFM         */
    csrWifiSuppNmeProfileOrderSetCfmHandler,        /* CSR_WIFI_NME_PROFILE_ORDER_SET_CFM          */
    csrWifiSuppNmeProfileConnectCfmHandler,         /* CSR_WIFI_NME_PROFILE_CONNECT_CFM            */
    NULL,                                           /* CSR_WIFI_NME_WPS_CFM                        */
    NULL,                                           /* CSR_WIFI_NME_WPS_CANCEL_CFM                 */
    NULL,                                           /* CSR_WIFI_NME_CONNECTION_STATUS_GET_CFM      */
    NULL,                                           /* CSR_WIFI_NME_CERTIFICATE_VALIDATE_IND       */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_UPDATE_IND             */
    NULL,                                           /* CSR_WIFI_NME_PROFILE_DISCONNECT_IND         */
};
#endif


/* Utility functions */
#define WPA2_RSN_ID 0x30
#define WAPI_ID     0x44
#define WPA_ID      0xDD
#define WPA_OUI1    0x00
#define WPA_OUI2    0x50
#define WPA_OUI3    0xF2
#define WPA_OUI4    0x01

static WIFISUPP_ENCRYP_PROTOCOL_E getSecurityType(CsrWifiSmeScanResult *scanResult, WIFISUPP_CREDENTIAL_TYPE_E *p_cred_type)
{
    CsrUint32 current = 0;
    CsrBool wpaSupported = FALSE;

    if ((scanResult->capabilityInformation & 0x0010) == 0)
    {
        return WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;
    }

    *p_cred_type = WIFISUPP_CREDENTIAL_TYPE_PSK;

    /* Can the info elements for WPA/WPA2 data */
    while (current < (CsrUint32)scanResult->informationElementsLength)
    {
        /* Check for the WPA2 Info Element */
        if (scanResult->informationElements[current] == WPA2_RSN_ID)
        {
            return WIFISUPP_ENCRYP_PROTOCOL_WPA2;
        }

        if (scanResult->informationElements[current] == WAPI_ID)
        {
            CsrUint8 *ie = &scanResult->informationElements[current];
            const CsrUint8 wapi_akm_cert[4] = {0x00, 0x14, 0x72, 0x01};
            const CsrUint8 wapi_akm_psk[4] = {0x00, 0x14, 0x72, 0x02};
            if (CsrMemCmp(&ie[6], wapi_akm_cert, sizeof(wapi_akm_cert)) == 0)
                *p_cred_type = WIFISUPP_CREDENTIAL_TYPE_CER;                
            else if (CsrMemCmp(&ie[6], wapi_akm_psk, sizeof(wapi_akm_psk)) == 0)
                *p_cred_type = WIFISUPP_CREDENTIAL_TYPE_PSK;                
            return WIFISUPP_ENCRYP_PROTOCOL_WAPI;
        }

        /* Check for the WPA Info Element */
        if (scanResult->informationElements[current]     == WPA_ID &&
            scanResult->informationElements[current + 2] == WPA_OUI1 &&
            scanResult->informationElements[current + 3] == WPA_OUI2 &&
            scanResult->informationElements[current + 4] == WPA_OUI3 &&
            scanResult->informationElements[current + 5] == WPA_OUI4)
        {
            /* Some APs support 'mixed-mode' where both WPA + WPA2 are supported,
               we want to support highest mode, so continue checking for WPA2 */
            wpaSupported = TRUE;;
        }

        current += scanResult->informationElements[current + 1] + 2;
    }

    if (wpaSupported)
    {
        return WIFISUPP_ENCRYP_PROTOCOL_WPA;
    }
    else
    {
        return WIFISUPP_ENCRYP_PROTOCOL_WEP;
    }
}

CsrUint8 getUint8Rate(CsrUint8 rate)
{
    CsrUint32 ii;
    CsrUint8 uint8Rate = 0;

    // Run through the bits from left to right, and add the value to the rate
    for (ii = 0; ii < 7; ii++)
    {
        uint8Rate += (rate & (1 << ii)) ? (1 << ii) : 0;
    }

    return uint8Rate;
}


/* Returns the supported rate of a AP in multiples of 500 kb/s */
CsrUint32 getRate(CsrUint16 informationElementsLength, CsrUint8 *informationElements)
{
    CsrUint32 ieOffset = 0;
    CsrUint8 rate = 0;

    /* IE format is     [ ID | Length | Information ]
       Number of octets:   1      1       Length */
    while (ieOffset < (CsrUint32)informationElementsLength)
    {
        CsrUint32 ieId = (CsrUint32)informationElements[ieOffset];
        CsrUint32 ieLen = (CsrUint32)informationElements[ieOffset + 1];
        if (ieId == 1)
        { /* 1 == IE_ID_SUPPORTED_RATES */
            CsrUint32 rateIterator;
            CsrUint8 nextRate = informationElements[ieOffset + 2];
            rate = getUint8Rate(rate);

            for (rateIterator = 0; rateIterator < ieLen; rateIterator++)
            {
                nextRate = informationElements[ieOffset + 2 + rateIterator];
                nextRate = getUint8Rate(nextRate);

                if (rate < nextRate)
                {
                    rate = nextRate;
                }
            }
        }
        else if (ieId == 50)
        { /* 50 == IE_ID_EXTENDED_SUPPORTED_RATES */
            CsrUint32 rateIterator;
            CsrUint8 nextRate = informationElements[ieOffset + 2];

            // Supported rates are stored in big endian, and MSB indicates optional (0) and mandatory (1), so mask out
            nextRate = getUint8Rate(nextRate);

            for (rateIterator = 0; rateIterator < ieLen; rateIterator++)
            {
                nextRate = informationElements[ieOffset + 2 + rateIterator];
                nextRate = getUint8Rate(nextRate);

                if (rate < nextRate)
                {
                    rate = nextRate;
                }
            }
        }

        /* IE_ID investigated, move ahead to next one */
        if (ieLen > 0)
        {
            ieOffset += ieLen + 2; // Remember to add two for the header
        }
        else
        {
            ieOffset += 2;
        }
    }

    return rate / 2; // Result sent is multiples of 500 kb/s, so divide by 2
}

/* State event functions */
static void csrWifiSuppSmeActivateCfmHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiSmeActivateCfm *prim = (CsrWifiSmeActivateCfm *) instData->recvMessage;
    CsrDebugTraceLow("CSR_WIFI_SME_ACTIVATE_CFM prim received - status: %d!!", prim->status);

#ifdef CSR_WIFI_6030
    /* TODO EK
       fix for AP mode
    */
#endif
    if (instData->wifiOn)
    {
        /* SR 21745 */
        extern void CsrWifiOn (void);
        CsrDebugTraceLow("CsrWifiSuppSmeActivateCfmHandler - turning WiFi back on after WIFI_OFF_IND");
        CsrWifiOn();
    }
}

extern SUPPADAP_CUSTOM_INFO_T g_suppadap_csr_info;

static void csrWifiSuppSmeWifiOnCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeWifiOnCfm *prim = (CsrWifiSmeWifiOnCfm *) instData->recvMessage;

#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("CSR_WIFI_SME_WIFI_ON_CFM prim received - status: %d - net_id: 0x%08X - wifiON: %d", 
        prim->status,
        csrGlobalIpEtherContext,
        instData->wifiOn);
    if (prim->status != CSR_RESULT_SUCCESS)
    {
#else
    CsrDebugTraceLow("CSR_WIFI_SME_WIFI_ON_CFM prim received - status: %d - net_id: 0x%08X", prim->status, *csrGlobalIpEtherContext);
    if(prim->status != CSR_WIFI_SME_STATUS_SUCCESS)
    {
#endif
        CsrWifiOffVarsDeInit();
    }

    if (instData->wifiOn == FALSE)
    {
        WIFISUPP_ON_RESULT_T resultData;

#ifdef CSR_WIFI_6030
        if (prim->status == CSR_RESULT_SUCCESS)
        {
            resultData.net_id = csrGlobalIpEtherContext;
#else
        if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
        {
            resultData.net_id = *csrGlobalIpEtherContext;
#endif        
            resultData.wifi_on_result = WIFISUPP_RESULT_SUCC;
            resultData.is_support_roam = TRUE;

            // Store that WiFi is on, so that we can turn it on again if we receive a WIFI_OFF_IND
            instData->wifiOn = TRUE;

            /* Copy support from instance data */
            CsrMemCpy(resultData.is_support_protocol, g_suppadap_csr_info.is_support_protocol, WIFISUPP_ENCRYP_PROTOCOL_MAX * sizeof(BOOLEAN));
            CsrMemCpy(resultData.is_support_eap, g_suppadap_csr_info.is_support_eap, WIFISUPP_WPA_EAP_TYPE_MAX * sizeof(BOOLEAN));
            CsrMemCpy(resultData.is_support_inner_eap, g_suppadap_csr_info.is_support_inner_eap, WIFISUPP_INNER_CREDENTIAL_TYPE_MAX * sizeof(BOOLEAN));

            CsrWifiSmeScanConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
            CsrWifiSmePermanentMacAddressGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);

#ifdef CSR_WIFI_6030
            CsrWifiSmeEventMaskSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
                                          CSR_WIFI_SME_INDICATIONS_WIFIOFF |
                                          CSR_WIFI_SME_INDICATIONS_CONNECTIONQUALITY |
                                          CSR_WIFI_SME_INDICATIONS_MEDIASTATUS |
                                          CSR_WIFI_SME_INDICATIONS_IBSSSTATION);
            CsrWifiSmeHostConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0);
#else
            CsrWifiSmeEventMaskSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
                                          CSR_WIFI_SME_INDICATIONS_MASK_WIFIOFF |
                                          CSR_WIFI_SME_INDICATIONS_MASK_CONNECTIONQUALITY |
                                          CSR_WIFI_SME_INDICATIONS_MASK_MEDIASTATUS |
                                          CSR_WIFI_SME_INDICATIONS_MASK_IBSSSTATION);
            CsrWifiSmeHostConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
#endif
            CsrWifiSmeVersionsGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
        }
        else
        {
            resultData.wifi_on_result = WIFISUPP_RESULT_FAIL;
        }
#ifdef CSR_WIFI_6030
#ifdef CSR_WIFI_AP_ENABLE
        /*  TODO EK fix fail case for AP mode!!!
            send fail result to UI
        */
        if (instData->bApMode)
        {
            CsrDebugTraceLow("CSR_WIFI_SME_WIFI_ON_CFM: ap cfg");
            CsrWifiNmeApConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, instData->apSuppConfig, instData->apSuppMacConfig);
        }
        else
        {
#endif
            SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &resultData);
#ifdef CSR_WIFI_AP_ENABLE
        }
#endif
#else
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &resultData);
#endif
    }
}


/* Start of Handler functions */
static void csrWifiSuppSmeWifiOffCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeWifiOffCfm *prim = (CsrWifiSmeWifiOffCfm *) instData->recvMessage;
    WIFISUPP_OFF_RESULT_T resultData;

    CsrDebugTraceLow("CSR_WIFI_SME_WIFI_OFF_CFM prim received - status: %d!", prim->status);

    instData->wifiOn = FALSE;
#ifdef CSR_WIFI_6030
    #ifdef MIFI_SUPPORT
    if (instData->bApMode)
    {
        MIFI_SIG_STOP_CNF_INFO_T *pSig = CsrPmemAlloc(sizeof(MIFI_SIG_STOP_CNF_INFO_T));
    
        CsrDebugTraceLow("CSR_WIFI_SME_WIFI_OFF_CFM AP mode");
    
        pSig->result = CSR_RESULT_SUCCESS == prim->status ? WLAN_RESULT_SUCCESS : WLAN_RESULT_FAIL;
        pSig->ssid.length = instData->ssid.length;
        CsrMemCpy(pSig->ssid.ssid, instData->ssid.ssid, instData->ssid.length);
        
        CsrDebugTraceLow("CSR_WIFI_NME_AP_STOP_CFM %d %s", instData->ssid.length, instData->ssid.ssid);
        pSig->ssid.reserve = 0;
        instData->bApMode = FALSE;
        MIFIAPISUPP_StopCnf(pSig);
    }
    else
    #endif
    {
        resultData.wifi_off_result = CSR_RESULT_SUCCESS == prim->status ?  SCI_SUCCESS : SCI_ERROR;
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &resultData);
    }
#else
    {
        resultData.wifi_off_result = CSR_WIFI_SME_STATUS_SUCCESS == prim->status ?  SCI_SUCCESS : SCI_ERROR;
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &resultData);
    }
#endif
}

static void csrWifiSuppSmeWifiOffIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeWifiOffInd *prim = (CsrWifiSmeWifiOffInd *) instData->recvMessage;
    WIFISUPP_OFF_RESULT_T resultData;

    CsrDebugTraceLow("===== CSR_WIFI_SME_WIFI_OFF_IND prim received - reason: %d!!", prim->reason);

#ifdef CSR_WIFI_6030
    if(prim->reason == CSR_WIFI_SME_CONTROL_INDICATION_USER_REQUESTED)
    {
    }
    else if (prim->reason == CSR_WIFI_SME_CONTROL_INDICATION_ERROR)
#else
    if (prim->reason == CSR_WIFI_SME_CONTROL_INDICATION_ERROR)
#endif
    {
        /* Restart WiFi by calling activateReq again */
        CsrWifiSmeActivateReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
    }
    else
    {
        resultData.wifi_off_result = SCI_ERROR;

        SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &resultData); // CNF used?
    }
}

static void csrWifiSuppSmePermanentMacAddressGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmePermanentMacAddressGetCfm *prim = (CsrWifiSmePermanentMacAddressGetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_PERMANENT_MAC_ADDRESS_GET_CFM prim received - addr: 0x%02X:0x%02X:0x%02X:0x%02X:0x%02X:0x%02X",
                     prim->permanentMacAddress.a[0],
                     prim->permanentMacAddress.a[1],
                     prim->permanentMacAddress.a[2],
                     prim->permanentMacAddress.a[3],
                     prim->permanentMacAddress.a[4],
                     prim->permanentMacAddress.a[5]);
}

static void csrWifiSuppSmeVersionsGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrCharString tmp[32+1];
    
    CsrWifiSmeVersionsGetCfm *prim = (CsrWifiSmeVersionsGetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM prim received - status: %d", prim->status);
#ifdef CSR_WIFI_6030
    if (prim->status == CSR_RESULT_SUCCESS)
#else
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
#endif
    {
        if (first_ver_get)
        {
#ifdef CSR_WIFI_6030            
            CsrUInt16ToHex((CsrUint16)prim->versions.chipId, tmp);            
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);            
            CsrUInt16ToHex((CsrUint16)prim->versions.chipVersion, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->versions.firmwareBuild, tmp);            
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);            
            CsrUInt16ToHex((CsrUint16)prim->versions.firmwarePatch, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->versions.firmwareHip, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrStrCat(CsrWiFiRelVer, prim->versions.routerBuild);            
            CsrStrCat(CsrWiFiRelVer, " ");
            CsrUInt16ToHex((CsrUint16)prim->versions.routerHip, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrStrCat(CsrWiFiRelVer, prim->versions.smeBuild);
            CsrStrCat(CsrWiFiRelVer, " ");
            CsrUInt16ToHex((CsrUint16)prim->versions.smeHip, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
#else
            CsrUInt16ToHex((CsrUint16)prim->chipId, tmp);            
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);            
            CsrUInt16ToHex((CsrUint16)prim->chipVersion, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->firmwareBuild, tmp);            
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);            
            CsrUInt16ToHex((CsrUint16)prim->firmwarePatch, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->firmwareHip, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->driverBuild, tmp);            
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);            
            CsrUInt16ToHex((CsrUint16)prim->driverHip, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->smeBuild, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->smeVariant, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
            CsrUInt16ToHex((CsrUint16)prim->smeHip, tmp);
            CsrStrCat(tmp, " ");
            CsrStrCat(CsrWiFiRelVer, tmp);
#endif
            first_ver_get = FALSE;
        }
        CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM - CsrWiFiRelVer: %s", CsrWiFiRelVer);
    }
}

static void csrWifiSuppSmePowerConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{        
    CsrWifiSmePowerConfigSetCfm *prim = (CsrWifiSmePowerConfigSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_POWER_CONFIG_SET_CFM prim received - status: %d", prim->status);
}

static void csrWifiSuppSmePowerConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmePowerConfigGetCfm *prim = (CsrWifiSmePowerConfigGetCfm *) instData->recvMessage;

    // Workaround MMI send commands with the same pars at once
    CsrDebugTraceLow("CSR_WIFI_SME_POWER_CONFIG_GET_CFM target - state: %d", instData->command_info_ptr->para1);
    CsrDebugTraceLow("CSR_WIFI_SME_POWER_CONFIG_GET_CFM prim received - status: %d, powerSaveLevel: %d",
        prim->status,
#ifdef CSR_WIFI_6030
        prim->powerConfig.powerSaveLevel);
#else
        prim->powerSaveLevel);
#endif

#ifdef CSR_WIFI_6030
    if (instData->command_info_ptr->para1)
    {
        prim->powerConfig.powerSaveLevel = CSR_WIFI_SME_POWER_SAVE_LEVEL_LOW;
    }
    else
    {
        prim->powerConfig.powerSaveLevel = CSR_WIFI_SME_POWER_SAVE_LEVEL_AUTO;
    }
    if (prim->status == CSR_RESULT_SUCCESS)
    {
        CsrDebugTraceLow("CsrWifiSmePowerConfigGetCfmHandler, status: %d, powerSaveLevel: %d",
            prim->status,
            prim->powerConfig.powerSaveLevel);
        CsrWifiSmePowerConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
            prim->powerConfig);
    }
#else
    if (instData->command_info_ptr->para1)
    {
        prim->powerSaveLevel = CSR_WIFI_SME_POWER_SAVE_LEVEL_LOW;
    }
    else
    {
        prim->powerSaveLevel = CSR_WIFI_SME_POWER_SAVE_LEVEL_MED;
    }
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        CsrDebugTraceLow("CsrWifiSmePowerConfigGetCfmHandler, status: %d, powerSaveLevel: %d",
            prim->status,
            prim->powerSaveLevel);
        CsrWifiSmePowerConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
            prim->powerSaveLevel,
            prim->listenIntervalBeacons,
            prim->rxDtims,
            prim->d3AutoScanMode);
    }
#endif
}

static void csrWifiSuppSmeRoamingConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeRoamingConfigSetCfm *prim = (CsrWifiSmeRoamingConfigSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_ROAMING_CONFIG_SET_CFM prim received - status: %d", prim->status);
}

static void csrWifiSuppSmeRoamingConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeRoamingConfigGetCfm *prim = (CsrWifiSmeRoamingConfigGetCfm *) instData->recvMessage;

    // Workaround MMI send commands with the same pars at once
    CsrDebugTraceLow("CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM target - state: %d", instData->command_info_ptr->para2);
    CsrDebugTraceLow("CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM prim received - status: %d, disableRoamScans: %d",
        prim->status,
#ifdef CSR_WIFI_6030
        prim->roamingConfig.disableRoamScans);
#else
        prim->disableRoamScans);
#endif
#ifdef CSR_WIFI_6030
    if (instData->command_info_ptr->para2)
    {
        prim->roamingConfig.disableRoamScans = FALSE;
    }
    else
    {
        prim->roamingConfig.disableRoamScans = TRUE;
    }
    if (prim->status == CSR_RESULT_SUCCESS)
    {
        CsrDebugTraceLow("CsrWifiSmeRoamingConfigGetCfmHandler, status: %d, disableRoamScans: %d",
            prim->status,
            prim->roamingConfig.disableRoamScans);
        CsrWifiSmeRoamingConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0, prim->roamingConfig);
    }
#else
    if (instData->command_info_ptr->para2)
    {
        prim->disableRoamScans = FALSE;
    }
    else
    {
        prim->disableRoamScans = TRUE;
    }
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        CsrDebugTraceLow("CsrWifiSmeRoamingConfigGetCfmHandler, status: %d, disableRoamScans: %d",
            prim->status,
            prim->disableRoamScans);
        CsrWifiSmeRoamingConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
            prim->roamingBands,
            prim->lowQualHystWindow,
            prim->apBlockTimeMs,
            prim->roamMonitorPeriodMs,
            prim->roamNumMaxTh,
            prim->disableRoamScans,
            prim->reconnectLimit,
            prim->roamScanCfg);
    }
#endif
}

static void csrWifiSuppSmeScanFullCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeScanFullCfm *prim = (CsrWifiSmeScanFullCfm *) instData->recvMessage;
    static int scan_try_num = 0;
    #define SCAN_TRY_NUM_MAX    (3)

    CsrDebugTraceLow("CSR_WIFI_SME_SCAN_FULL_CFM prim received - status: %d", prim->status);

    scan_try_num ++;
    if (scan_try_num < SCAN_TRY_NUM_MAX) {
        CsrWifiMacAddress bssid;
        CsrMemSet(bssid.a, 0xFF, WIFISUPP_BSSID_LEN);
        CsrWifiSmeScanFullReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0, NULL, bssid, FALSE, 0, 0, 0, NULL, 0, NULL);
        return;
    }

    scan_try_num = 0;
#ifdef CSR_WIFI_6030
    if (prim->status == CSR_RESULT_SUCCESS)
#else
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
#endif
    {
        CsrDebugTraceLow("CSR_WIFI_SME_SCAN_FULL_CFM prim received - status: WIFISUPP_RESULT_SUCC");
        CsrWifiSmeScanResultsGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
    }
    else
    {
        WIFISUPP_SCAN_CNF_T resultData;
        resultData.scan_result = WIFISUPP_RESULT_FAIL;
        CsrDebugTraceLow("CSR_WIFI_SME_SCAN_FULL_CFM prim received - status: WIFISUPP_RESULT_FAIL");
        
        CsrPmemFree(instData->scan_req_info_ptr);
        instData->scan_req_info_ptr = NULL;
        
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_CNF, &resultData);
    }
}

static void csrWifiSuppSmeScanResultsGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrUint32 start = 0,
                   end = 0,
                   i;
    CsrBool found = FALSE;
    WIFISUPP_SCAN_CNF_T resultScanCnf;
    WIFISUPP_SCAN_AP_INFO_T *resultData = NULL;

    CsrWifiSmeScanResultsGetCfm * prim = (CsrWifiSmeScanResultsGetCfm *) instData->recvMessage;

    resultScanCnf.scan_result = WIFISUPP_RESULT_FAIL;
#ifdef CSR_WIFI_6030
    if (prim->status == CSR_RESULT_SUCCESS)
#else
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
#endif
    {
        WIFISUPP_CREDENTIAL_TYPE_E cred_type;
        CsrDebugTraceLow("CSR_WIFI_SME_SCAN_RESULTS_GET_CFM - ssid: %s, length: %d", instData->scan_req_info_ptr->ssid.ssid, instData->scan_req_info_ptr->ssid.ssid_len);
        if (instData->scan_req_info_ptr->ssid.ssid_len != 0)
        {
            // For scan specified AP
            for (i = 0; i < prim->scanResultsCount; i++)
            {
                if (CsrStrCmp((CsrCharString *)&(prim->scanResults[i].ssid.ssid), (CsrCharString *)&(instData->scan_req_info_ptr->ssid.ssid)) == 0)
                {
                    // Found specified AP
                    CsrDebugTraceLow("CSR_WIFI_SME_SCAN_RESULTS_GET_CFM - found %s", prim->scanResults[i].ssid.ssid);
                    found = TRUE;
                    break;
                }
            }
            if (found)
            {
                start = i;
                end = i + 1;
                resultScanCnf.scan_result = WIFISUPP_RESULT_SUCC;
            }
        }
        else
        {
            // For scan full
            CsrDebugTraceLow("CSR_WIFI_SME_SCAN_RESULTS_GET_CFM prim received - status: %d, number of results: %d",
                prim->status,
                prim->scanResultsCount);
            start = 0;
            end = prim->scanResultsCount;
            resultScanCnf.scan_result = WIFISUPP_RESULT_SUCC;
        }
        if (resultScanCnf.scan_result == WIFISUPP_RESULT_SUCC)
        {
            for (i = start; i < end; i++)
            {           
                resultData = CsrPmemAlloc(sizeof(WIFISUPP_SCAN_AP_INFO_T));
                CsrDebugTraceLow("UNIFI: Scan result - ssid: %.*s, rssi: %d, snr: %d, capability: 0x%04X",
                    prim->scanResults[i].ssid.length,
                    prim->scanResults[i].ssid.ssid,
                    prim->scanResults[i].rssi,
                    prim->scanResults[i].snr,
                    prim->scanResults[i].capabilityInformation);
                CsrMemCpy(resultData->ssid.ssid, prim->scanResults[i].ssid.ssid, prim->scanResults[i].ssid.length);
                resultData->ssid.ssid_len = prim->scanResults[i].ssid.length;
                CsrMemCpy(resultData->bssid_info.bssid, prim->scanResults[i].bssid.a, sizeof(CsrWifiMacAddress));
                resultData->signal_qua = prim->scanResults[i].rssi;
                resultData->noise = prim->scanResults[i].snr;
                resultData->beacon_interval = prim->scanResults[i].beaconPeriodTu;
                resultData->channel = prim->scanResults[i].channelNumber;
                resultData->encryp_protocol = getSecurityType(&prim->scanResults[i], &cred_type);
                resultData->credential_type = cred_type;
                CsrDebugTraceLow("UNIFI: encryp_protocol found from getSecurityType is: %d", resultData->encryp_protocol);
                resultData->network_mode = (prim->scanResults[i].bssType == CSR_WIFI_SME_BSS_TYPE_INFRASTRUCTURE ? WIFISUPP_NETWORK_MODE_INFRASTRUCTURE : WIFISUPP_NETWORK_MODE_ADHOC);
                resultData->rate = getRate(prim->scanResults[i].informationElementsLength, prim->scanResults[i].informationElements);
                SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_IND, resultData);
                CsrPmemFree(resultData);
            }
        }
    }
    CsrDebugTraceLow("CSR_WIFI_SME_SCAN_RESULTS_GET_CFM prim received - scan_result: %d", resultScanCnf.scan_result);
    
    CsrPmemFree(instData->scan_req_info_ptr);
    instData->scan_req_info_ptr = NULL;
    
    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_CNF, &(resultScanCnf.scan_result));
}

static void csrWifiSuppSmeConnectionInfoGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
#ifdef CSR_WIFI_6030
    instData->currentConnectionInfo = (CsrWifiSmeConnectionInfoGetCfm *) instData->recvMessage;
	
    CsrDebugTraceLow("CSR_WIFI_SME_CONNECTION_INFO_CFM prim received status: %d InterfaceTag: %d - ssid: %.*s", 
        instData->currentConnectionInfo->status, 
        instData->currentConnectionInfo->interfaceTag, 
        instData->currentConnectionInfo->connectionInfo.ssid.length, 
        instData->currentConnectionInfo->connectionInfo.ssid.ssid);
#else
    CsrWifiSmeConnectionInfoGetCfm *prim = (CsrWifiSmeConnectionInfoGetCfm *) instData->recvMessage;
    instData->currentConnectionInfo = prim;

    CsrDebugTraceLow("CSR_WIFI_SME_CONNECTION_INFO_CFM prim received status: %d - ssid: %.*s", prim->status, prim->ssid.length, prim->ssid.ssid);
#endif
    // Set to NULL to avoid prim from being freed
    instData->recvMessage = NULL;
}

static void csrWifiSuppSmeConnectionQualityIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeConnectionQualityInd *prim = (CsrWifiSmeConnectionQualityInd *) instData->recvMessage;

#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("CSR_WIFI_SME_CONNECTION_QUALITY_IND prim received - rssi: %d - snr: %d",
                     prim->linkQuality.unifiRssi,
                     prim->linkQuality.unifiSnr);
#else
    CsrDebugTraceLow("CSR_WIFI_SME_CONNECTION_QUALITY_IND prim received - rssi: %d - snr: %d",
                     prim->unifiRssi,
                     prim->unifiSnr);
#endif

    if (instData->currentConnectionInfo)
    {
        static WIFISUPP_RSSI_CHANGED_INFO_T resultData;
#ifdef CSR_WIFI_6030
        CsrDebugTraceLow("bssid: %02X:%02X:%02X:%02X:%02X:%02X - ssid: %*s",
                         instData->currentConnectionInfo->connectionInfo.bssid.a[0],
                         instData->currentConnectionInfo->connectionInfo.bssid.a[1],
                         instData->currentConnectionInfo->connectionInfo.bssid.a[2],
                         instData->currentConnectionInfo->connectionInfo.bssid.a[3],
                         instData->currentConnectionInfo->connectionInfo.bssid.a[4],
                         instData->currentConnectionInfo->connectionInfo.bssid.a[5],
                         instData->currentConnectionInfo->connectionInfo.ssid.length,
                         instData->currentConnectionInfo->connectionInfo.ssid.ssid);

        CsrMemCpy(&resultData.ssid, &instData->currentConnectionInfo->connectionInfo.ssid, sizeof(CsrWifiSsid));
        CsrMemCpy(resultData.bssid_info.bssid, instData->currentConnectionInfo->connectionInfo.bssid.a, WIFISUPP_BSSID_LEN);
        resultData.new_signal_qua = prim->linkQuality.unifiRssi;
#else
        CsrDebugTraceLow("bssid: %02X:%02X:%02X:%02X:%02X:%02X - ssid: %*s",
                         instData->currentConnectionInfo->bssid.a[0],
                         instData->currentConnectionInfo->bssid.a[1],
                         instData->currentConnectionInfo->bssid.a[2],
                         instData->currentConnectionInfo->bssid.a[3],
                         instData->currentConnectionInfo->bssid.a[4],
                         instData->currentConnectionInfo->bssid.a[5],
                         instData->currentConnectionInfo->ssid.length,
                         instData->currentConnectionInfo->ssid.ssid);

        CsrMemCpy(&resultData.ssid, &instData->currentConnectionInfo->ssid, sizeof(CsrWifiSsid));
        CsrMemCpy(resultData.bssid_info.bssid, instData->currentConnectionInfo->bssid.a, WIFISUPP_BSSID_LEN);
        resultData.new_signal_qua = prim->unifiRssi;
#endif
        // Store pointer and clear recvMessage pointer as it gets freed later on otherwise
        // We need the rssi value for later 
        CsrPmemFree(instData->currentConnectionQuality);
        instData->currentConnectionQuality = prim;
        instData->recvMessage = NULL;
// TODO: Still needs to be implemented, currently memory is corrupted when this is called?!
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_RSSI_CHANGED_IND, &resultData);
    }
}

static void csrWifiSuppSmeDisconnectCfmHandler(csrWifiSupplicantInstanceData * instData)
{  
    CsrWifiSmeDisconnectCfm *prim = (CsrWifiSmeDisconnectCfm *) instData->recvMessage;

    // SR30032: debug and fix assert here
    CsrDebugTraceLow("*****%s: BEGIN", __func__);

    // SR30032: debug and fix assert here
    if (instData->disconnectReq == NULL)
    {
        CsrDebugTraceLow("*****%s: Unexpected instData->disconnectReq == NULL", __func__);
#ifdef CSR_WIFI_6030
        if (prim->status == CSR_RESULT_SUCCESS)
#else
        if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
#endif
        {
            CsrDebugTraceLow("*****%s: 1st  free all data associated with this current connection", __func__);
            // 1st free all data associated with this current connection
            CsrWifiSmeFreeUpstreamMessageContents(CSR_WIFI_SME_PRIM, instData->currentConnectionInfo);
            if (instData->currentConnectionInfo == NULL)
            {
                CsrDebugTraceLow("*****%s: 1st Unexpected instData->currentConnectionInfo == NULL", __func__);
            }
            else
            {
                CsrDebugTraceLow("*****%s: 1st Expected instData->currentConnectionInfo != NULL", __func__);
                CsrPmemFree(instData->currentConnectionInfo);
                instData->currentConnectionInfo = NULL;
            }
        }
        // SR30032: debug and fix assert here
        CsrDebugTraceLow("*****%s: 1st END", __func__);
        return;
    }
    else
    {

#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("CSR_WIFI_SME_DISCONNECT_CFM prim received status: %d interfaceTag: %d - tried to disconnect from: %.*s",
                     prim->status,
                     prim->interfaceTag,
                     instData->disconnectReq->ssid.ssid_len,
                     instData->disconnectReq->ssid.ssid);   
#else
    CsrDebugTraceLow("CSR_WIFI_SME_DISCONNECT_CFM prim received status: %d - tried to disconnect from: %.*s",
                     prim->status,
                     instData->disconnectReq->ssid.ssid_len,
                     instData->disconnectReq->ssid.ssid);
#endif

#ifdef CSR_WIFI_6030
    if (prim->status == CSR_RESULT_SUCCESS)
#else
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
#endif
    {
        instData->disconnectReq->result = WIFISUPP_RESULT_SUCC;

        // SR30032: debug and fix assert here
        CsrDebugTraceLow("*****%s: 2nd free all data associated with this current connection", __func__);
        // 2nd free all data associated with this current connection
        CsrWifiSmeFreeUpstreamMessageContents(CSR_WIFI_SME_PRIM, instData->currentConnectionInfo);
        if (instData->currentConnectionInfo == NULL)
        {
            CsrDebugTraceLow("*****%s: 2nd Unexpected instData->currentConnectionInfo == NULL", __func__);
        }
        else
        {
            CsrDebugTraceLow("*****%s: 2nd Expected instData->currentConnectionInfo != NULL", __func__);
            CsrPmemFree(instData->currentConnectionInfo);
            instData->currentConnectionInfo = NULL;
        }
    }
    else
    {
        instData->disconnectReq->result = WIFISUPP_RESULT_FAIL;
    }

    CsrRSSITimerBeSet = RSSI_POLLING_TIMER_CANCEL;
    SUPPMAIN_NotifyApp(WIFISUPP_SIG_DISCONNECT_CNF, instData->disconnectReq);

    // Free the disconnect req, as we have no further use for it
    CsrPmemFree(instData->disconnectReq);
    instData->disconnectReq = NULL;

    // SR30032: debug and fix assert here
    CsrDebugTraceLow("*****%s: 2nd END", __func__);

    }
}

static void csrWifiSuppSmeHostConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeHostConfigGetCfm * prim = (CsrWifiSmeHostConfigGetCfm *) instData->recvMessage;

#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("CSR_WIFI_SME_HOST_CONFIG_GET_CFM prim received status: %d, PowerMode: %d, appDataPeriodMs: %d", 
        prim->status,
        prim->hostConfig.powerMode,
        prim->hostConfig.applicationDataPeriodMs);

    if (prim->status == CSR_RESULT_SUCCESS)
    {
        CsrWifiSmeHostConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0, prim->hostConfig);
    }
#else
    CsrDebugTraceLow("CSR_WIFI_SME_HOST_CONFIG_GET_CFM prim received status: %d, PowerMode: %d, appDataPeriodMs: %d", prim->status, prim->powerMode, prim->applicationDataPeriodMs);

    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        CsrWifiSmeHostConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, CSR_WIFI_SME_HOST_POWER_MODE_POWER_SAVE, prim->applicationDataPeriodMs);
    }
#endif
}

static void csrWifiSuppSmeHostConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeHostConfigSetCfm * prim = (CsrWifiSmeHostConfigSetCfm *) instData->recvMessage;

#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("CSR_WIFI_SME_HOST_CONFIG_SET_CFM prim received status: %d InterfaceTag: %d", prim->status, prim->interfaceTag);

    if (prim->status == CSR_RESULT_SUCCESS)
#else
    CsrDebugTraceLow("CSR_WIFI_SME_HOST_CONFIG_SET_CFM prim received status: %d", prim->status);

    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
#endif
    {
        /* If CsrWifiSupplicantInitialize() is called with TRUE as the parameter, we want strict mode! */
        if (csrWifiEnableStrictComplianceMode == TRUE)
        {
            // TODO: Implement dynamic toggle for TKIP + DraftN compliance - default is NO
        }
    }
}

static void csrWifiSuppSmeEventMaskSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeEventMaskSetCfm *prim = (CsrWifiSmeEventMaskSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_EVENT_MASK_SET_CFM prim received status: %d", prim->status);
}

static void csrWifiSuppSmeIbssStationIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeIbssStationInd *prim = (CsrWifiSmeIbssStationInd *) instData->recvMessage;
    WIFISUPP_BSSID_CHANGED_INFO_T resultData;

    CsrDebugTraceLow("CSR_WIFI_SME_IBSS_STATION_IND prim received - address: %02X:%02X:%02X:%02X:%02X:%02X - connected: %d",
                     prim->address.a[0], prim->address.a[1], prim->address.a[2], prim->address.a[3], prim->address.a[4], prim->address.a[5],
                     prim->isconnected);

#ifdef CSR_WIFI_6030
    CsrMemCpy(&resultData.ssid, &instData->currentConnectionInfo->connectionInfo.ssid, sizeof(CsrWifiSsid)); 
#else
    CsrMemCpy(&resultData.ssid, &instData->currentConnectionInfo->ssid, sizeof(CsrWifiSsid));
#endif
    CsrMemCpy(resultData.new_bssid_info.bssid, prim->address.a, WIFISUPP_BSSID_LEN);

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_RSSI_CHANGED_IND, &resultData);

    if (!prim->isconnected)
    { /* Ibss changed, we are no longer connected, free connectInfo */
        WIFISUPP_DISCONNECT_INFO_T disconnectInd;

#ifdef CSR_WIFI_6030
        CsrMemCpy(&disconnectInd.ssid, &instData->currentConnectionInfo->connectionInfo.ssid, sizeof(CsrWifiSsid));
#else
        CsrMemCpy(&disconnectInd.ssid, &instData->currentConnectionInfo->ssid, sizeof(CsrWifiSsid));
#endif

        SUPPMAIN_NotifyApp(WIFISUPP_SIG_DISCONNECT_IND, &disconnectInd);
        CsrWifiSmeFreeUpstreamMessageContents(CSR_WIFI_SME_PRIM, instData->currentConnectionInfo);
        CsrPmemFree(instData->currentConnectionInfo);
        instData->currentConnectionInfo = NULL;
    }
}

static void csrWifiSuppSmeLinkQualityGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeLinkQualityGetCfm *prim = (CsrWifiSmeLinkQualityGetCfm *) instData->recvMessage;

    /* if supplicant is under connection process */
    if (instData->connectReq != NULL && CsrRSSITimerBeSet == RSSI_POLLING_TIMER_INIT) 
    {
        WIFISUPP_CONNECT_RESULT_T resultData;
        CsrWifiSmeConnectionInfoGetCfm *connInfo = instData->currentConnectionInfo;        
        resultData.result = WIFISUPP_RESULT_SUCC;   
#ifdef CSR_WIFI_6030
        CsrDebugTraceLow("CsrWifiSuppSmeLinkQualityGetCfmHandler rssi: %d", prim->linkQuality.unifiRssi);
#else
        CsrDebugTraceLow("CsrWifiSuppSmeLinkQualityGetCfmHandler rssi: %d", prim->unifiRssi);
#endif
        if (connInfo != NULL)
        {
            /* sr 23831 */
#ifdef CSR_WIFI_6030
            CsrMemCpy(&(resultData.ssid), &connInfo->connectionInfo.ssid, sizeof(WIFISUPP_SSID_T));
            CsrMemCpy(resultData.bssid_info.bssid, &connInfo->connectionInfo.bssid, WIFISUPP_BSSID_LEN);
            resultData.channel = connInfo->connectionInfo.channelNumber;
#else
            CsrMemCpy(&(resultData.ssid), &connInfo->ssid, sizeof(WIFISUPP_SSID_T));
            CsrMemCpy(resultData.bssid_info.bssid, &connInfo->bssid, WIFISUPP_BSSID_LEN);
            resultData.channel = connInfo->channelNumber;
#endif
        }
        else
        {
            CsrMemCpy(&(resultData.ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
            CsrMemCpy(resultData.bssid_info.bssid, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);
            resultData.channel = instData->connectReq->channel;
        }
#ifdef CSR_WIFI_6030
        resultData.rssi = (prim->status == CSR_RESULT_SUCCESS)? prim->linkQuality.unifiRssi : -90;
#else
        resultData.rssi = (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)? prim->unifiRssi : -90;
#endif
        
        /* free connectReq */
        CsrPmemFree(instData->connectReq);
        instData->connectReq = NULL;
        instData->sharedConnect = FALSE;

        CsrRSSITimerBeSet = RSSI_POLLING_TIMER_ACTIVE;
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_CONNECT_CNF, &resultData);
    }
    else if(CsrRSSITimerBeSet == RSSI_POLLING_TIMER_ACTIVE)
    {
        CsrWifiSmeConnectionInfoGetCfm *connInfo = instData->currentConnectionInfo;
        static WIFISUPP_RSSI_CHANGED_INFO_T resultData;

#ifdef CSR_WIFI_6030
        CsrMemCpy(&resultData.ssid, &connInfo->connectionInfo.ssid, sizeof(WIFISUPP_SSID_T));
        CsrMemCpy(resultData.bssid_info.bssid, &connInfo->connectionInfo.bssid, WIFISUPP_BSSID_LEN);
        resultData.new_signal_qua = prim->linkQuality.unifiRssi;
        //CsrDebugTraceLow("CsrWifiSuppSmeLinkQualityGetCfmHandler rssi change: %d", prim->linkQuality.unifiRssi);
#else
        CsrMemCpy(&resultData.ssid, &connInfo->ssid, sizeof(WIFISUPP_SSID_T));
        CsrMemCpy(resultData.bssid_info.bssid, &connInfo->bssid, WIFISUPP_BSSID_LEN);
        resultData.new_signal_qua = prim->unifiRssi;
        //CsrDebugTraceLow("CsrWifiSuppSmeLinkQualityGetCfmHandler rssi change: %d", prim->unifiRssi);
#endif

        SUPPMAIN_NotifyApp(WIFISUPP_SIG_RSSI_CHANGED_IND, &resultData);
    }
}

static void csrWifiSuppSmeMediaStatusIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeMediaStatusInd *prim = (CsrWifiSmeMediaStatusInd *) instData->recvMessage;
#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  Status: %d, prim->ssid: %.*s - assocRspResult: %d!!",
         prim->mediaStatus,
         prim->connectionInfo.ssid.length,
         prim->connectionInfo.ssid.ssid,
         prim->connectionInfo.assocRspResult);
#else
    CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  Status: %d, prim->ssid: %.*s - assocRspResult: %d!!",
         prim->mediaStatus,
         prim->ssid.length,
         prim->ssid.ssid,
         prim->assocRspResult);
#endif

    if (prim->mediaStatus == CSR_WIFI_SME_MEDIA_STATUS_DISCONNECTED &&
        instData->disconnectReq == NULL)
    {
        WIFISUPP_DISCONNECT_INFO_T resultData;

        CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  - Doing disconnect stuff");

#ifdef CSR_WIFI_6030
        CsrMemCpy(&resultData.ssid, &prim->connectionInfo.ssid, sizeof(WIFISUPP_SSID_T));
#else
        CsrMemCpy(&resultData.ssid, &prim->ssid, sizeof(WIFISUPP_SSID_T));
#endif
        /* Free all data associated with this current connection */
        CsrWifiSmeFreeUpstreamMessageContents(CSR_WIFI_SME_PRIM, instData->currentConnectionInfo);
        CsrPmemFree(instData->currentConnectionInfo);
        instData->currentConnectionInfo = NULL;

        /* Reset sharedConnect variable as we might need to retry during subsequent connection attempts */
        instData->sharedConnect = FALSE;

        /* SR 27184/27237 */
        if (wifi_supp_is_on)
        {
            CsrRSSITimerBeSet = RSSI_POLLING_TIMER_CANCEL;
            SUPPMAIN_NotifyApp(WIFISUPP_SIG_DISCONNECT_IND, &resultData);
        }
    }
    else if (prim->mediaStatus == CSR_WIFI_SME_MEDIA_STATUS_CONNECTED)
    {
        CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  - Get connection stuff");
        
        if (instData->currentConnectionInfo)
        {
            // Previous connection exist
#ifdef CSR_WIFI_6030
            if (!CsrStrNCmp((CsrCharString *)prim->connectionInfo.ssid.ssid, (CsrCharString *)instData->currentConnectionInfo->connectionInfo.ssid.ssid, (CsrSize)instData->currentConnectionInfo->connectionInfo.ssid.length))
#else
            if (!CsrStrNCmp((CsrCharString *)prim->ssid.ssid, (CsrCharString *)instData->currentConnectionInfo->ssid.ssid, (CsrSize)instData->currentConnectionInfo->ssid.length))
#endif
            {
                // Same SSID
                CsrUint16 i;
                CsrBool roam = FALSE;
                for (i = 0; i < sizeof(CsrWifiMacAddress); i++)
                {
#ifdef CSR_WIFI_6030
                    if (prim->connectionInfo.bssid.a[i] != instData->currentConnectionInfo->connectionInfo.bssid.a[i])
#else
                    if (prim->bssid.a[i] != instData->currentConnectionInfo->bssid.a[i])
#endif
                    {
                        roam = TRUE;
                        break;
                    }
                }
                if (roam)
                {
                    // Diff BSSID - therefore notify MMI roaming has occurred
                    static WIFISUPP_BSSID_CHANGED_INFO_T resultData;
#ifdef CSR_WIFI_6030
                    CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  ROAMING  - Previous ssid: %.*s, bssid: %02X:%02X:%02X:%02X:%02X:%02X, channel: %d",
                        instData->currentConnectionInfo->connectionInfo.ssid.length,
                        instData->currentConnectionInfo->connectionInfo.ssid.ssid,
                        instData->currentConnectionInfo->connectionInfo.bssid.a[0],
                        instData->currentConnectionInfo->connectionInfo.bssid.a[1],
                        instData->currentConnectionInfo->connectionInfo.bssid.a[2],
                        instData->currentConnectionInfo->connectionInfo.bssid.a[3],
                        instData->currentConnectionInfo->connectionInfo.bssid.a[4],
                        instData->currentConnectionInfo->connectionInfo.bssid.a[5],
                        instData->currentConnectionInfo->connectionInfo.channelNumber);
                    CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  ROAMING  - Current ssid: %.*s, bssid: %02X:%02X:%02X:%02X:%02X:%02X, channel: %d",
                        prim->connectionInfo.ssid.length,
                        prim->connectionInfo.ssid.ssid,
                        prim->connectionInfo.bssid.a[0],
                        prim->connectionInfo.bssid.a[1],
                        prim->connectionInfo.bssid.a[2],
                        prim->connectionInfo.bssid.a[3],
                        prim->connectionInfo.bssid.a[4],
                        prim->connectionInfo.bssid.a[5],
                        prim->connectionInfo.channelNumber);
                    CsrMemCpy(&resultData.ssid, &instData->currentConnectionInfo->connectionInfo.ssid, sizeof(CsrWifiSsid));
                    CsrMemCpy(resultData.new_bssid_info.bssid,  prim->connectionInfo.bssid.a, WIFISUPP_BSSID_LEN);
                    resultData.channel = prim->connectionInfo.channelNumber;
#else
                    CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  ROAMING  - Previous ssid: %.*s, bssid: %02X:%02X:%02X:%02X:%02X:%02X, channel: %d",
                        instData->currentConnectionInfo->ssid.length,
                        instData->currentConnectionInfo->ssid.ssid,
                        instData->currentConnectionInfo->bssid.a[0],
                        instData->currentConnectionInfo->bssid.a[1],
                        instData->currentConnectionInfo->bssid.a[2],
                        instData->currentConnectionInfo->bssid.a[3],
                        instData->currentConnectionInfo->bssid.a[4],
                        instData->currentConnectionInfo->bssid.a[5],
                        instData->currentConnectionInfo->channelNumber);
                    CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  ROAMING  - Current ssid: %.*s, bssid: %02X:%02X:%02X:%02X:%02X:%02X, channel: %d",
                        prim->ssid.length,
                        prim->ssid.ssid,
                        prim->bssid.a[0],
                        prim->bssid.a[1],
                        prim->bssid.a[2],
                        prim->bssid.a[3],
                        prim->bssid.a[4],
                        prim->bssid.a[5],
                        prim->channelNumber);
                    CsrMemCpy(&resultData.ssid, &instData->currentConnectionInfo->ssid, sizeof(CsrWifiSsid));
                    CsrMemCpy(resultData.new_bssid_info.bssid,  prim->bssid.a, WIFISUPP_BSSID_LEN);
                    resultData.channel = prim->channelNumber;
#endif
                    SUPPMAIN_NotifyApp(WIFISUPP_SIG_BSSID_CHANGED_IND, &resultData);
                }
            }
        }
        
#ifdef CSR_WIFI_6030
        CsrWifiSmeConnectionInfoGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0);
#else
        CsrWifiSmeConnectionInfoGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
#endif
    }
}

static void csrWifiSuppNmeProfileSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiNmeProfileSetCfm *prim = (CsrWifiNmeProfileSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_SET_CFM received - status: %d - ssid: %.*s",
                     prim->status,
                     instData->connectReq->ssid.ssid_len,
                     instData->connectReq->ssid.ssid);

#ifdef CSR_WIFI_6030
    if (prim->status == CSR_RESULT_SUCCESS)
#else
    if (prim->status == CSR_WIFI_NME_STATUS_SUCCESS)
#endif
    {
        CsrWifiNmeProfileIdentity profile;

        CsrMemCpy(&(profile.ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
//        CsrMemCpy(profile.bssid.a, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);
        CsrMemSet(profile.bssid.a, 0xFF, WIFISUPP_BSSID_LEN);

#ifdef CSR_WIFI_6030
        CsrWifiNmeProfileConnectReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0, profile);
#else
        CsrWifiNmeProfileConnectReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, profile);
#endif
    }
    else
    {
        WIFISUPP_CONNECT_RESULT_T resultData;
        CsrMemCpy(&(resultData.ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
        CsrMemCpy(resultData.bssid_info.bssid, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);

        resultData.result = WIFISUPP_RESULT_FAIL;

        // Clean up after failing to set up profile - no connect attempt
        CsrWifiSmeFreeUpstreamMessageContents(CSR_WIFI_SME_PRIM, instData->currentConnectionInfo);
        CsrPmemFree(instData->currentConnectionInfo);
        instData->currentConnectionInfo = NULL;

        CsrPmemFree(instData->connectReq);
        instData->connectReq = NULL;

        SUPPMAIN_NotifyApp(WIFISUPP_SIG_CONNECT_CNF, &resultData);
    }
}

static void csrWifiSuppNmeProfileOrderSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiNmeProfileOrderSetCfm *prim = (CsrWifiNmeProfileOrderSetCfm *) instData->recvMessage;
#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_ORDER_SET_CFM prim received - status: %d interfaceTag: %d", prim->status, prim->interfaceTag);
#else
    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_ORDER_SET_CFM prim received - status: %d", prim->status);
#endif
}

static void csrWifiSuppNmeProfileConnectCfmHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiNmeProfileConnectCfm *prim = (CsrWifiNmeProfileConnectCfm *) instData->recvMessage;
    WIFISUPP_CONNECT_RESULT_T resultData;
    uint32 i;

#ifdef CSR_WIFI_6030
    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM prim received -status: %d interfaceTag: %d", prim->status, prim->interfaceTag);
#else
    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM prim received -status: %d", prim->status);
#endif

    for (i = 0; i < prim->connectAttemptsCount; i++)
    {
        CsrDebugTraceLow("BSSID: %02X:%02X:%02X:%02X:%02X:%02X", prim->connectAttempts[i].bssid.a[0], prim->connectAttempts[i].bssid.a[1], prim->connectAttempts[i].bssid.a[2],
                         prim->connectAttempts[i].bssid.a[3], prim->connectAttempts[i].bssid.a[4], prim->connectAttempts[i].bssid.a[5]);
        CsrDebugTraceLow("Status: %d, Security Error: %d", prim->connectAttempts[i].status, prim->connectAttempts[i].securityError);
    }


    /* Only try Shared if connectReq is empty (since we clear it after trying Shared)
       and if the connect result was a failure (due to AP using Shared, phone using Open */
    if (
#ifdef CSR_WIFI_6030
        (prim->status == CSR_WIFI_RESULT_SECURITY_ERROR || prim->status == CSR_RESULT_FAILURE)
#else
        prim->status == CSR_WIFI_NME_STATUS_ERROR
#endif
        && instData->connectReq != NULL 
        && instData->connectReq->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WEP 
        && instData->sharedConnect == FALSE)
    {
        /* Copied from wifisupp_adap_csr_sef.c - CsrWifiSuppConnect, try again, just using Shared this time */
        CsrWifiNmeProfile profile;

        CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM using WEP in OPEN failed - retrying with SHARED");

        // Setup NME profile information
        CsrMemCpy(&(profile.profileIdentity.ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
//        CsrMemCpy(profile.profileIdentity.bssid.a, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);
        CsrMemSet(profile.profileIdentity.bssid.a, 0xFF, WIFISUPP_BSSID_LEN);

        profile.bssType = instData->connectReq->network_mode == WIFISUPP_NETWORK_MODE_INFRASTRUCTURE ? CSR_WIFI_NME_BSS_TYPE_INFRASTRUCTURE : CSR_WIFI_NME_BSS_TYPE_ADHOC;
        profile.ccxOptionsMask = 0;
        profile.channelNo = instData->connectReq->channel;
        profile.cloakedSsid = FALSE;
#ifdef CSR_WIFI_6030
        profile.wmmQosInfoMask = 0;
#else
        profile.wmmQosCapabilitiesMask = 0;
#endif
        if (instData->connectReq->credential.wep_credential.key_type == WIFISUPP_WEP_KEY_TYPE_64BITS)
        {
            profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WEP64;
            //profile.credentials.credential.wep64Key.selectedWepKey = getWepKeyIndex(instData->connectReq);
            profile.credentials.credential.wep64Key.selectedWepKey = instData->connectReq->credential.wep_credential.key_in_use + 1;
            //CsrDebugTraceLow("profile.credentials.credential.wep64Key.selectedWepKey is %d", instData->connectReq->credential.wep_credential.key_in_use);
            profile.credentials.credential.wep64Key.wepAuthType = CSR_WIFI_NME_AUTH_MODE_80211_SHARED;  //peishan pm07


            CsrMemCpy(profile.credentials.credential.wep64Key.key1, &instData->connectReq->credential.wep_credential.key.key_64bits_arr[0][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            CsrMemCpy(profile.credentials.credential.wep64Key.key2, &instData->connectReq->credential.wep_credential.key.key_64bits_arr[1][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            CsrMemCpy(profile.credentials.credential.wep64Key.key3, &instData->connectReq->credential.wep_credential.key.key_64bits_arr[2][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            CsrMemCpy(profile.credentials.credential.wep64Key.key4, &instData->connectReq->credential.wep_credential.key.key_64bits_arr[3][0], WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
        }
        else
        {
            profile.credentials.credentialType = CSR_WIFI_NME_CREDENTIAL_TYPE_WEP128;
            //profile.credentials.credential.wep128Key.selectedWepKey = getWepKeyIndex(instData->connectReq);
            profile.credentials.credential.wep128Key.selectedWepKey = instData->connectReq->credential.wep_credential.key_in_use + 1;
            //CsrDebugTraceLow("profile.credentials.credential.wep128Key.selectedWepKey is %d", instData->connectReq->credential.wep_credential.key_in_use);
            profile.credentials.credential.wep128Key.wepAuthType = CSR_WIFI_NME_AUTH_MODE_80211_SHARED;  //peishan pm07


            CsrMemCpy(profile.credentials.credential.wep128Key.key1, &instData->connectReq->credential.wep_credential.key.key_128bits_arr[0][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            CsrMemCpy(profile.credentials.credential.wep128Key.key2, &instData->connectReq->credential.wep_credential.key.key_128bits_arr[1][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            CsrMemCpy(profile.credentials.credential.wep128Key.key3, &instData->connectReq->credential.wep_credential.key.key_128bits_arr[2][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
            CsrMemCpy(profile.credentials.credential.wep128Key.key4, &instData->connectReq->credential.wep_credential.key.key_128bits_arr[3][0], WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN);
        }
        /* End copy */

        CsrDebugTraceLow("Security conf - protocol: %d - credentialType: %d, ", instData->connectReq->encryp_protocol, profile.credentials.credentialType);

        instData->sharedConnect = TRUE;

        /* Connect, using SHARED this time */
        CsrWifiNmeProfileSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, profile);
    }
    else
    {
#ifdef CSR_WIFI_6030
        if (prim->status == CSR_RESULT_SUCCESS)
#else
        if (prim->status == CSR_WIFI_NME_STATUS_SUCCESS)
#endif
        {
            resultData.result = WIFISUPP_RESULT_SUCC;
        }
        else
        {
            resultData.result = WIFISUPP_RESULT_FAIL;
        }

        /*
         * As for the success case, we need to get RSSI value. In failure case, give a failure indication here.
         */
        if (resultData.result == WIFISUPP_RESULT_SUCC)
        {
        	CsrRSSITimerBeSet = RSSI_POLLING_TIMER_INIT;
#ifdef CSR_WIFI_6030
            CsrWifiSmeLinkQualityGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, 0);
#else
            CsrWifiSmeLinkQualityGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
#endif
            return;
        }
        

        CsrMemCpy(&(resultData.ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
        CsrMemCpy(resultData.bssid_info.bssid, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);

        // Connection request completed, free connectReq pointer, not needed anymore
        CsrPmemFree(instData->connectReq);
        instData->connectReq = NULL;
        instData->sharedConnect = FALSE;

        SUPPMAIN_NotifyApp(WIFISUPP_SIG_CONNECT_CNF, &resultData);
    }
}	


#ifdef CSR_WIFI_6030

#ifdef CSR_WIFI_SUPPORT_EAP_SIM_AKA_SUPPLICANT_DUMMY
#include "mn_error.h"
#include "mn_type.h"
// DUMMY defines
#define MN_SPECIAL_ALGO_REQ_LEN     128
#define MN_MAX_IMSI_INFO_ARR_LEN    16
#define MN_MAX_MCC_MNC_ARR_LEN      3
//dummy struct
typedef struct _MN_IMSI_INFO_T
{
    uint8 parity_and_type;
    uint8 imsi_ascii_len;
    uint8 imsi_ascii_val[MN_MAX_IMSI_INFO_ARR_LEN];
    uint8 mcc_ascii[MN_MAX_MCC_MNC_ARR_LEN];
    uint8 mnc_ascii_len;
    uint8 mnc_ascii[MN_MAX_MCC_MNC_ARR_LEN];
} MN_IMSI_INFO_T;

typedef enum
{
    APP_MN_SIM_SPECIAL_ALGO_RESPONSE_SUCCESS,
    APP_MN_SIM_SPECIAL_ALGO_RESPONSE_CONTECT_NOT_SUPPORT,
    APP_MN_SIM_SPECIAL_ALGO_RESPONSE_MAC_FAILURE,
    APP_MN_SIM_SPECIAL_ALGO_RESPONSE_SYNC_FAILURE,
    APP_MN_SIM_SPECIAL_ALGO_RESPONSE_OTHER_FAILURE,
} APP_MN_SIM_SPECIAL_ALGO_RESPONSE_TYPE_E;

//dummy functions
extern ERR_MNPHONE_CODE_E MNPHONE_GetImsiInfoEx(
    MN_DUAL_SYS_E dual_sys,
    MN_IMSI_INFO_T *imsi_info_ptr);

extern ERR_MNPHONE_CODE_E MNSIM_RunSIMSpecialAlgoEx(
    MN_DUAL_SYS_E dual_sys,
    uint32 task_id,
    uint32 algo_index,
    uint8 *rand_ptr,
    uint8 *autn_ptr);
#endif

#ifdef CSR_WIFI_SUPPORT_EAP_SIM_AKA_SUPPLICANT

static void csrWifiSuppNmeSimImsiGetIndHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiNmeSimImsiGetInd *prim = instData->recvMessage;
    CsrResult status;
    CsrCharString *imsi;
    CsrWifiNmeSimCardType cardType;
    MN_DUAL_SYS_E dual = MN_DUAL_SYS_1;
//    MN_IMSI_INFO_T info;

    CsrDebugTraceLow("CSR_WIFI_NME_SIM_IMSI_GET_IND:");

/*
CSR_WIFI_NME_SIM_CARD_TYPE_2G
CSR_WIFI_NME_SIM_CARD_TYPE_3G
CSR_WIFI_NME_SIM_CARD_TYPE_2G3G
*/
/*    if (ERR_MNPHONE_NO_ERR == MNPHONE_GetImsiInfoEx(dual, &info))
    {
        CsrDebugTraceLow("CSR_WIFI_NME_SIM_IMSI_GET_IND: SUCCESS");
        status = CSR_RESULT_SUCCESS;
        cardType = CSR_WIFI_NME_SIM_CARD_TYPE_2G3G;
        imsi = CsrPmemAlloc(info.imsi_ascii_len+1);
        CsrStrNCpy(imsi, info.imsi_ascii_val, info.imsi_ascii_len+1);
    }
    else*/
    {
        CsrDebugTraceLow("CSR_WIFI_NME_SIM_IMSI_GET_IND: FAIL");
        status = CSR_RESULT_FAILURE;
        imsi = NULL;
        cardType = CSR_WIFI_NME_SIM_CARD_TYPE_2G3G;
    }
    CsrWifiNmeSimImsiGetResSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, status, imsi, cardType);
}

static void csrWifiSuppNmeSimGsmAuthIndHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiNmeSimGsmAuthInd *prim = instData->recvMessage;
    CsrResult status; /* CSR_RESULT_SUCCESS or CSR_RESULT_FAILURE */
    CsrUint8 kcsLength; /* Length in Bytes of Kc buffer. Legal values are: 16 or 24 */
    CsrUint8 *kcs;
    CsrUint8 sresLength; /* Length in Bytes of SRES buffer. Legal values are: 8 or 12 */
    CsrUint8 *sres;
    CsrUint32 result;
    extern BLOCK_ID csrWifiMsgHandlerTask;
/*
    EAP-SIM authentication requires 2
    or 3 GSM authentication rounds and therefore 2 or 3 RANDS (GSM Random
    Challenges) are included.

    randsLength - GSM RAND is 16 bytes long hence valid values are 32 (2 RANDS)
                  or 48 (3 RANDs).
*/

    CsrDebugTraceLow("CSR_WIFI_NME_SIM_GSM_AUTH_IND: len:%d %p", prim->randsLength, prim->rands);
//    result = MNSIM_RunSIMSpecialAlgoEx(MN_DUAL_SYS_1, csrWifiMsgHandlerTask, 1, prim->rands, sres);

    CsrDebugTraceLow("CSR_WIFI_NME_SIM_GSM_AUTH_IND: 0x%02X", result);
//    if (APP_MN_SIM_SPECIAL_ALGO_RESPONSE_SUCCESS == result)
//    CsrMemCpy(dest, prim->rands, prim->randsLength);
    CsrPmemFree(prim->rands);
//    Call SPRD SIM Mgr auth ()
/*    if (NO_ERROR)
    {
        status = CSR_RESULT_SUCCESS;
        kcs = CsrPmemAlloc(kcsLength);
        CsrMemCpy(kcs, src, kcsLength);
        sres = CsrPmemAlloc(sresLength);
        CsrMemCpy(sres, src, sresLength);
    }
    else*/
    {
        status = CSR_RESULT_FAILURE;
        kcs = NULL;
        kcsLength = 0;
        sres = NULL;
        sresLength = 0;
    }

    CsrWifiNmeSimGsmAuthResSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, status, kcsLength, kcs, sresLength, sres);
}

static void csrWifiSuppNmeSimUmtsAuthIndHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiNmeSimUmtsAuthInd *prim = instData->recvMessage;
    CsrResult status;
    CsrWifiNmeUmtsAuthResult result;
    CsrUint8 umtsCipherKey[16];
    CsrUint8 umtsIntegrityKey[16];
    CsrUint8 resParameterLength;
    CsrUint8 *resParameter;
    CsrUint8 auts[14];
/*
    EAP-AKA requires one UMTS authentication round and therefore only one 
    RAND and one AUTN values are included
*/
    CsrDebugTraceLow("CSR_WIFI_NME_SIM_UMTS_AUTH_IND: rand %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ",
        prim->rand[0], prim->rand[1], prim->rand[2], prim->rand[3], 
        prim->rand[4], prim->rand[5], prim->rand[6], prim->rand[7], 
        prim->rand[8], prim->rand[9], prim->rand[10], prim->rand[11], 
        prim->rand[12], prim->rand[13], prim->rand[14], prim->rand[15]);
    CsrDebugTraceLow("CSR_WIFI_NME_SIM_UMTS_AUTH_IND: autn %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ",
        prim->autn[0], prim->autn[1], prim->autn[2], prim->autn[3], 
        prim->autn[4], prim->autn[5], prim->autn[6], prim->autn[7], 
        prim->autn[8], prim->autn[9], prim->autn[10], prim->autn[11], 
        prim->autn[12], prim->autn[13], prim->autn[14], prim->autn[15]);

    /* Call SPRD SIM mgr for auth */
    // return to CSR from SPRD
#if 0
    switch ()
    {
        case :
        {   /* Response from the application that received the NME SIM UMTS AUTH IND.
               The values of umtsCipherKey, umtsIntegrityKey, resParameterLength and
               resParameter are only meanigful when result = UMTS_AUTH_RESULT_SUCCESS */
            status = CSR_RESULT_SUCCESS;
            result = CSR_WIFI_NME_UMTS_AUTH_RESULT_SUCCESS;
            CsrMemCpy(umtsCipherKey, CiphSrc, 16);
            CsrMemCpy(umtsIntegrityKey, InteSrc, 16);
            resParameterLength = ;
            resParameter = CsrPmemAlloc(resParameterLength);
            CsrMemCpy(resParameter, ResSrc, resParameterLength);
            break;
        }
        case :
        {   /* The value of auts is only meaningful when result=UMTS_AUTH_RESULT_SYNC_FAIL */
            status = CSR_RESULT_SUCCESS;
            result = CSR_WIFI_NME_UMTS_AUTH_RESULT_SYNC_FAIL;
            CsrMemCpy(auts, autsSrc, 14);
            resParameter = NULL;
            resParameterLength = 0;
            break;
        }
        default:
        {
            status = CSR_RESULT_FAILURE;
            result = CSR_WIFI_NME_UMTS_AUTH_RESULT_REJECT;
            resParameter = NULL;
            resParameterLength = 0;
            break;
        }
    }
#endif
    CsrWifiNmeSimUmtsAuthResSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, status, result, 
        umtsCipherKey, umtsIntegrityKey, resParameterLength, resParameter, auts);
}
#endif

static void csrWifiSuppNmeEventMaskSetCfmHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiNmeEventMaskSetCfm *prim = (CsrWifiNmeEventMaskSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_NME_EVENT_SET_CFM - status: %d", prim->status);

}

#endif

