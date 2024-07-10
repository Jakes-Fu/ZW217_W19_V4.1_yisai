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

#include "csr_wifi_mib_repository.h"

#include "wifisupp_adap_csr_sef.h"

/*lint -e835*/
/*lint -e838*/
void CsrWifiSuppSmeActivateCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeWifiOnCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeWifiOffCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeWifiOffIndHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmePermanentMacAddressGetCfmHandler(csrWifiSupplicantInstanceData * instData);
#if 0
void CsrWifiSuppSmeScanConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeScanConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
#endif
void CsrWifiSuppSmePowerConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmePowerConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeRoamingConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeRoamingConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeScanFullCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeScanResultsGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeConnectionInfoGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeConnectionQualityIndHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeDisconnectCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeHostConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeHostConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeEventMaskSetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeIbssStationIndHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeLinkQualityGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeMediaStatusIndHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppSmeVersionsGetCfmHandler(csrWifiSupplicantInstanceData * instData);
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
void CsrWifiSmeMibConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSmeMibConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSmeMibGetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSmeMibSetCfmHandler(csrWifiSupplicantInstanceData * instData);
#endif

void CsrWifiSuppNmeProfileSetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppNmeProfileOrderSetCfmHandler(csrWifiSupplicantInstanceData * instData);
void CsrWifiSuppNmeProfileConnectCfmHandler(csrWifiSupplicantInstanceData * instData);

extern CsrCharString CsrWiFiRelVer[128];
static CsrBool first_ver_get = TRUE;
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
/*
unifiCsrOnlyPatchIntFunction.27 ::= 0
#unifi_mib mode set 1.3.6.1.4.1.22164.1.2.3.2.1.1.3.27 type int 0
CsrUint8 binarray[] = {
    0x55, 0x44, 0x4d, 0x49, 0x01, 0x00, 0x19, 0x00, 0x00, 0x00, 0x17, 0x00, 
    0x30, 0x15, 0x06, 0x10, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x02, 0x03, 0x02, 0x01, 0x01, 0x03, 0x1b, 0x02, 0x01, 0x00, 
    };        
unifiKeepAliveTime ::= 0
#unifi_mib mode set 1.3.6.1.4.1.22164.1.1.4.1.21 type int 0
CsrUint8 binarray[] = {
    0x55, 0x44, 0x4d, 0x49, 0x01, 0x00, 0x16, 0x00, 0x00, 0x00, 0x14, 0x00, 
    0x30, 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x01, 0x04, 0x01, 0x15, 0x02, 0x01, 0x00,
    };
*/
const CsrUint8 unifiCsrOnlyPatchIntFunctionp27Equal0[] = {
    0x30, 0x15, 0x06, 0x10, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x02, 0x03, 0x02, 0x01, 0x01, 0x03, 0x1b, 0x02, 0x01, 0x00
    };
const CsrUint8 unifiKeepAliveTimeEqual0[] = {
    0x30, 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x01, 0x04, 0x01, 0x15, 0x02, 0x01, 0x00
    };
/*
const CsrUint8 unifiCsrOnlyPatchIntFunctionp27EqualDefault[] = {
    0x30, 0x17, 0x06, 0x10, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x02, 0x03, 0x02, 0x01, 0x01, 0x03, 0x1b, 0x02, 0x03, 0x00, 0xff, 0xff
    };
const CsrUint8 unifiKeepAliveTimeEqual30[] = {
    0x30, 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x01, 0x04, 0x01, 0x15, 0x02, 0x01, 0x1e
    };
const CsrUint8 unifiCsrOnlyPatchIntFunctionp27[] = {
    0x30, 0x14, 0x06, 0x10, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x02, 0x03, 0x02, 0x01, 0x01, 0x03, 0x1b, 0x05, 0x00
    };     
const CsrUint8 unifiKeepAliveTime[] = {
    0x30, 0x11, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 
    0x01, 0x01, 0x04, 0x01, 0x15, 0x05, 0x00
    };
*/
extern CsrBool csrWifiAnritsuTestMode;
extern CsrUint32 csrWifiAnritsuCurrentRate;
#endif

extern CsrRssiPollTimerStatus CsrRSSITimerBeSet;
extern int wifi_supp_is_on;
extern CsrUint32 *csrGlobalIpEtherContext;
extern void CsrDebugTraceLow(const char* format, ...);
extern void CsrWifiOffVarsDeInit(void);

const CsrWifiSmeStateHandlerType CsrWifiSuppSmeHandlers[CSR_WIFI_SME_PRIM_UPSTREAM_COUNT] =
{
        CsrWifiSuppSmeActivateCfmHandler,       /* CSR_WIFI_SME_ACTIVATE_CFM                         */
        NULL,       /* CSR_WIFI_SME_ADHOC_CONFIG_GET_CFM                 */
        NULL,       /* CSR_WIFI_SME_ADHOC_CONFIG_SET_CFM                 */
        NULL,       /* CSR_WIFI_SME_ASSOCIATION_COMPLETE_IND             */
        NULL,       /* CSR_WIFI_SME_ASSOCIATION_START_IND                */
        NULL,       /* CSR_WIFI_SME_BLACKLIST_CFM                        */
        NULL,       /* CSR_WIFI_SME_CALIBRATION_DATA_GET_CFM             */
        NULL,       /* CSR_WIFI_SME_CALIBRATION_DATA_SET_CFM             */
        NULL,       /* CSR_WIFI_SME_CCX_CONFIG_GET_CFM                   */
        NULL,       /* CSR_WIFI_SME_CCX_CONFIG_SET_CFM                   */
        NULL,       /* CSR_WIFI_SME_COEX_CONFIG_GET_CFM                  */
        NULL,       /* CSR_WIFI_SME_COEX_CONFIG_SET_CFM                  */
        NULL,       /* CSR_WIFI_SME_COEX_INFO_GET_CFM                    */
        NULL,       /* CSR_WIFI_SME_CONNECT_CFM                          */
        NULL,       /* CSR_WIFI_SME_CONNECTION_CONFIG_GET_CFM            */
        CsrWifiSuppSmeConnectionInfoGetCfmHandler,       /* CSR_WIFI_SME_CONNECTION_INFO_GET_CFM              */
        CsrWifiSuppSmeConnectionQualityIndHandler,       /* CSR_WIFI_SME_CONNECTION_QUALITY_IND               */
        NULL,       /* CSR_WIFI_SME_CONNECTION_STATS_GET_CFM             */
        NULL,       /* CSR_WIFI_SME_DEACTIVATE_CFM                       */
        CsrWifiSuppSmeDisconnectCfmHandler,       /* CSR_WIFI_SME_DISCONNECT_CFM                       */
        CsrWifiSuppSmeEventMaskSetCfmHandler,       /* CSR_WIFI_SME_EVENT_MASK_SET_CFM                   */
        CsrWifiSuppSmeHostConfigGetCfmHandler,       /* CSR_WIFI_SME_HOST_CONFIG_GET_CFM                  */
        CsrWifiSuppSmeHostConfigSetCfmHandler,       /* CSR_WIFI_SME_HOST_CONFIG_SET_CFM                  */
        CsrWifiSuppSmeIbssStationIndHandler,       /* CSR_WIFI_SME_IBSS_STATION_IND                     */
        NULL,       /* CSR_WIFI_SME_KEY_CFM                              */
        CsrWifiSuppSmeLinkQualityGetCfmHandler, /* CSR_WIFI_SME_LINK_QUALITY_GET_CFM                 */
        CsrWifiSuppSmeMediaStatusIndHandler,       /* CSR_WIFI_SME_MEDIA_STATUS_IND                     */
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
        CsrWifiSmeMibConfigGetCfmHandler,       /* CSR_WIFI_SME_MIB_CONFIG_GET_CFM                   */
        CsrWifiSmeMibConfigSetCfmHandler,       /* CSR_WIFI_SME_MIB_CONFIG_SET_CFM                   */
        CsrWifiSmeMibGetCfmHandler,       /* CSR_WIFI_SME_MIB_GET_CFM                          */
        NULL,       /* CSR_WIFI_SME_MIB_GET_NEXT_CFM                     */
        CsrWifiSmeMibSetCfmHandler,       /* CSR_WIFI_SME_MIB_SET_CFM                          */
#else
        NULL,       /* CSR_WIFI_SME_MIB_CONFIG_GET_CFM                   */
        NULL,       /* CSR_WIFI_SME_MIB_CONFIG_SET_CFM                   */
        NULL,       /* CSR_WIFI_SME_MIB_GET_CFM                          */
        NULL,       /* CSR_WIFI_SME_MIB_GET_NEXT_CFM                     */
        NULL,       /* CSR_WIFI_SME_MIB_SET_CFM                          */
#endif
        NULL,       /* CSR_WIFI_SME_MIC_FAILURE_IND                      */
        NULL,       /* CSR_WIFI_SME_MULTICAST_ADDRESS_CFM                */
        NULL,       /* CSR_WIFI_SME_PACKET_FILTER_SET_CFM                */
        CsrWifiSuppSmePermanentMacAddressGetCfmHandler,       /* CSR_WIFI_SME_PERMANENT_MAC_ADDRESS_GET_CFM        */
        NULL,       /* CSR_WIFI_SME_PMKID_CANDIDATE_LIST_IND             */
        NULL,       /* CSR_WIFI_SME_PMKID_CFM                            */
        CsrWifiSuppSmePowerConfigGetCfmHandler,       /* CSR_WIFI_SME_POWER_CONFIG_GET_CFM                 */
        CsrWifiSuppSmePowerConfigSetCfmHandler,       /* CSR_WIFI_SME_POWER_CONFIG_SET_CFM                 */
        NULL,       /* CSR_WIFI_SME_REGULATORY_DOMAIN_INFO_GET_CFM       */
        NULL,       /* CSR_WIFI_SME_ROAM_COMPLETE_IND                    */
        NULL,       /* CSR_WIFI_SME_ROAM_START_IND                       */
        CsrWifiSuppSmeRoamingConfigGetCfmHandler,       /* CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM               */
        CsrWifiSuppSmeRoamingConfigSetCfmHandler,       /* CSR_WIFI_SME_ROAMING_CONFIG_SET_CFM               */
        NULL,       /* CSR_WIFI_SME_SCAN_CONFIG_GET_CFM                  */
        NULL,       /* CSR_WIFI_SME_SCAN_CONFIG_SET_CFM                  */
        CsrWifiSuppSmeScanFullCfmHandler,       /* CSR_WIFI_SME_SCAN_FULL_CFM                        */
        NULL,       /* CSR_WIFI_SME_SCAN_RESULT_IND                      */
        NULL,       /* CSR_WIFI_SME_SCAN_RESULTS_FLUSH_CFM               */
        CsrWifiSuppSmeScanResultsGetCfmHandler,       /* CSR_WIFI_SME_SCAN_RESULTS_GET_CFM                 */
        NULL,       /* CSR_WIFI_SME_SME_CONFIG_GET_CFM                   */
        NULL,       /* CSR_WIFI_SME_SME_CONFIG_SET_CFM                   */
        NULL,       /* CSR_WIFI_SME_STATION_MAC_ADDRESS_GET_CFM          */
        NULL,       /* CSR_WIFI_SME_TSPEC_IND                            */
        NULL,       /* CSR_WIFI_SME_TSPEC_CFM                            */
        CsrWifiSuppSmeVersionsGetCfmHandler,       /* CSR_WIFI_SME_VERSIONS_GET_CFM                     */
        NULL,       /* CSR_WIFI_SME_WIFI_FLIGHTMODE_CFM                  */
        CsrWifiSuppSmeWifiOffIndHandler,       /* CSR_WIFI_SME_WIFI_OFF_IND                         */
        CsrWifiSuppSmeWifiOffCfmHandler,       /* CSR_WIFI_SME_WIFI_OFF_CFM                         */
        CsrWifiSuppSmeWifiOnCfmHandler,       /* CSR_WIFI_SME_WIFI_ON_CFM                          */
        NULL,       /* CSR_WIFI_SME_RESTRICTED_ACCESS_ENABLE_CFM         */
        NULL,       /* CSR_WIFI_SME_RESTRICTED_ACCESS_DISABLE_CFM        */
        NULL,       /* CSR_WIFI_SME_CLOAKED_SSIDS_SET_CFM                */
        NULL,       /* CSR_WIFI_SME_CLOAKED_SSIDS_GET_CFM                */
};

const CsrWifiSmeStateHandlerType CsrWifiSuppNmeHandlers[CSR_WIFI_NME_PRIM_UPSTREAM_COUNT] =
{
        CsrWifiSuppNmeProfileSetCfmHandler,       /* CSR_WIFI_NME_PROFILE_SET_CFM                      */
        NULL,       /* CSR_WIFI_NME_PROFILE_DELETE_CFM                   */
        NULL,       /* CSR_WIFI_NME_PROFILE_DELETE_ALL_CFM               */
        CsrWifiSuppNmeProfileOrderSetCfmHandler,       /* CSR_WIFI_NME_PROFILE_ORDER_SET_CFM                */
        CsrWifiSuppNmeProfileConnectCfmHandler,       /* CSR_WIFI_NME_PROFILE_CONNECT_CFM                  */
        NULL,       /* CSR_WIFI_NME_WPS_CFM                              */
        NULL,       /* CSR_WIFI_NME_WPS_CANCEL_CFM                       */
        NULL,       /* CSR_WIFI_NME_CONNECTION_STATUS_GET_CFM            */
        NULL,       /* CSR_WIFI_NME_CERTIFICATE_VALIDATE_IND             */
        NULL,       /* CSR_WIFI_NME_PROFILE_UPDATE_IND                   */
        NULL,       /* CSR_WIFI_NME_PROFILE_DISCONNECT_IND               */
};

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
    CsrUint16 current = 0;
    CsrBool wpaSupported = FALSE;

    if ((scanResult->capabilityInformation & 0x0010) == 0)
    {
        return WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;
    }

    *p_cred_type = WIFISUPP_CREDENTIAL_TYPE_PSK;

    /* Can the info elements for WPA/WPA2 data */
    while(current < scanResult->informationElementsLength)
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
    CsrUint8 ii = 0;
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
    CsrUint16 ieOffset = 0;
    CsrUint8 rate = 0;

    /* IE format is     [ ID | Length | Information ]
       Number of octets:   1      1       Length */
    while (ieOffset < informationElementsLength)
    {
        CsrUint8 ieId = informationElements[ieOffset];
        CsrUint8 ieLen = informationElements[ieOffset + 1];
        if (ieId == 1)
        { /* 1 == IE_ID_SUPPORTED_RATES */
            CsrUint8 rateIterator;
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
            CsrUint8 rateIterator;
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

#ifdef CSR_WIFI_NME_AUTOCONNECT
static CsrBool findNmeProfileInSet(csrWifiSupplicantInstanceData *instData, WIFISUPP_SSID_CONFIG_T *connectInfo)
{
    CsrBool apInList = FALSE;
    CsrUint8 ii = 0;

    for (ii = 0; ii < instData->profileIdentitysCount; ii++)
    {
        if (instData->profileIdentitys[ii].ssid.length == connectInfo->ssid.ssid_len &&
            CsrMemCmp(instData->profileIdentitys[ii].ssid.ssid, connectInfo->ssid.ssid, connectInfo->ssid.ssid_len) == 0)
        {
            apInList = TRUE;
            break;
        }
    }

    return apInList;
}
#endif

/* State event functions */
void CsrWifiSuppSmeActivateCfmHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiSmeActivateCfm *prim = (CsrWifiSmeActivateCfm *) instData->recvMessage;
    CsrDebugTraceLow("CSR_WIFI_SME_ACTIVATE_CFM prim received - status: %d!!", prim->status);

    if (instData->wifiOn)
    {
        /* SR 21745 */
        extern void CsrWifiOn (void);
        CsrDebugTraceLow("CsrWifiSuppSmeActivateCfmHandler - turning WiFi back on after WIFI_OFF_IND");
        CsrWifiOn();
    }
}

extern SUPPADAP_CUSTOM_INFO_T g_suppadap_csr_info;

void CsrWifiSuppSmeWifiOnCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeWifiOnCfm *prim = (CsrWifiSmeWifiOnCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_WIFI_ON_CFM prim received - status: %d - net_id: 0x%08X", prim->status, *csrGlobalIpEtherContext);

    if(prim->status != CSR_WIFI_SME_STATUS_SUCCESS)
    {
        CsrWifiOffVarsDeInit();
    }
    
    if (instData->wifiOn == FALSE)
    {
        WIFISUPP_ON_RESULT_T resultData;

        if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
        {
            resultData.wifi_on_result = WIFISUPP_RESULT_SUCC;
            resultData.net_id = *csrGlobalIpEtherContext;
            resultData.is_support_roam = TRUE;

            // Store that WiFi is on, so that we can turn it on again if we receive a WIFI_OFF_IND
            instData->wifiOn = TRUE;

            /* Copy support from instance data */
            CsrMemCpy(resultData.is_support_protocol, g_suppadap_csr_info.is_support_protocol, WIFISUPP_ENCRYP_PROTOCOL_MAX * sizeof(BOOLEAN));
            CsrMemCpy(resultData.is_support_eap, g_suppadap_csr_info.is_support_eap, WIFISUPP_WPA_EAP_TYPE_MAX * sizeof(BOOLEAN));
            CsrMemCpy(resultData.is_support_inner_eap, g_suppadap_csr_info.is_support_inner_eap, WIFISUPP_INNER_CREDENTIAL_TYPE_MAX * sizeof(BOOLEAN));

            CsrWifiSmeScanConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
            CsrWifiSmePermanentMacAddressGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);

            CsrWifiSmeEventMaskSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
                                          CSR_WIFI_SME_INDICATIONS_MASK_WIFIOFF |
                                          CSR_WIFI_SME_INDICATIONS_MASK_CONNECTIONQUALITY |
                                          CSR_WIFI_SME_INDICATIONS_MASK_MEDIASTATUS |
                                          CSR_WIFI_SME_INDICATIONS_MASK_IBSSSTATION);

            CsrWifiSmeHostConfigGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
            
            CsrWifiSmeVersionsGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
        }
        else
        {
            resultData.wifi_on_result = WIFISUPP_RESULT_FAIL;
        }
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &resultData);
    }
}


/* Start of Handler functions */
void CsrWifiSuppSmeWifiOffCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeWifiOffCfm *prim = (CsrWifiSmeWifiOffCfm *) instData->recvMessage;
    WIFISUPP_OFF_RESULT_T resultData;

    CsrDebugTraceLow("CSR_WIFI_SME_WIFI_OFF_CFM prim received - status: %d!", prim->status);

    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        resultData.wifi_off_result = SCI_SUCCESS;
    }
    else
    {
        resultData.wifi_off_result = SCI_ERROR;
    }
    instData->wifiOn = FALSE;
    SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &resultData);
}

void CsrWifiSuppSmeWifiOffIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeWifiOffInd *prim = (CsrWifiSmeWifiOffInd *) instData->recvMessage;
    WIFISUPP_OFF_RESULT_T resultData;

    CsrDebugTraceLow("===== CSR_WIFI_SME_WIFI_OFF_IND prim received - reason: %d!!", prim->reason);

    if (prim->reason == CSR_WIFI_SME_CONTROL_INDICATION_ERROR)
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

void CsrWifiSuppSmePermanentMacAddressGetCfmHandler(csrWifiSupplicantInstanceData * instData)
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

#if 0
void CsrWifiSuppSmeScanConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeScanConfigSetCfm *prim = (CsrWifiSmeScanConfigSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_SCAN_CONFIG_SET_CFM prim received - status: %d", prim->status);
}

void CsrWifiSuppSmeScanConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeScanConfigGetCfm *prim = (CsrWifiSmeScanConfigGetCfm *) instData->recvMessage;


    CsrDebugTraceLow("[Randy] CSR_WIFI_SME_SCAN_CONFIG_GET_CFM autonomus scan : %d, interval : %d",
                     instData->autoScan_info_ptr->is_enable,
                     instData->autoScan_info_ptr->interval);

    CsrDebugTraceLow("[Randy] CSR_WIFI_SME_SCAN_CONFIG_GET_CFM scanCfg : %d",
                     prim->scanCfg[0x03].intervalSeconds
                     );

    CsrDebugTraceLow("CSR_WIFI_SME_SCAN_CONFIG_GET_CFM prim received - status: %d. maxResults: %d, highRssi: %d, lowRssi: %d, deltaRssi: %d, highSnr: %d, lowSnr: %d, deltaSnr: %d",
                     prim->status,
                     prim->maxResults,
                     prim->highRssiThreshold,
                     prim->lowRssiThreshold,
                     prim->deltaRssiThreshold,
                     prim->highSnrThreshold,
                     prim->lowSnrThreshold,
                     prim->deltaSnrThreshold);
	prim->disableAutonomousScans = (~instData->autoScan_info_ptr->is_enable)&0x1;
	prim->scanCfg[0x03].intervalSeconds= instData->autoScan_info_ptr->interval;
//    prim->disableAutonomousScans->scanCfg[unifi_NotConnected].intervalSeconds 
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        CsrDebugTraceLow("[wifi] CsrWifiSuppSmeScanConfigGetCfmHandler, status: %d, disableAutonomousScans: %d", prim->status, prim->disableAutonomousScans);
        CsrWifiSmeScanConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
                                       prim->scanCfg,
                                       prim->disableAutonomousScans,	//test for autoscan
//										TRUE,		// original
                                       prim->maxResults,
                                       prim->highRssiThreshold,
                                       prim->lowRssiThreshold,
                                       prim->deltaRssiThreshold,
                                       prim->highSnrThreshold,
                                       prim->lowSnrThreshold,
                                       prim->deltaSnrThreshold,
                                       prim->passiveChannelListCount,
                                       prim->passiveChannelList);
    }
}
#endif

void CsrWifiSuppSmeVersionsGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrCharString tmp[32+1];
    
    CsrWifiSmeVersionsGetCfm *prim = (CsrWifiSmeVersionsGetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM prim received - status: %d", prim->status);
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        /*
        CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM prim received - chip ID: %#x, chip version: %#x",
            prim->chipId,
            prim->chipVersion);
        CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM prim received - firmware build: %u, firmware patch: %u, firmware HIP: %#06x",
            prim->firmwareBuild,
            prim->firmwarePatch,
            prim->firmwareHip);  
        CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM prim received - driver build: %u, driver HIP: %#06x",
            prim->driverBuild,
            prim->driverHip);        
        CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM prim received - sme build: %u, sme variant: %u, sme HIP: %#06x, sme ID string:%s",
            prim->smeBuild,
            prim->smeVariant,
            prim->smeHip,
            prim->smeIdString);
        */
        if (first_ver_get)
        {
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
            first_ver_get = FALSE;
        }
        CsrDebugTraceLow("CSR_WIFI_SME_VERSIONS_GET_CFM - CsrWiFiRelVer: %s", CsrWiFiRelVer);
    }
}

#ifdef CSR_WIFI_ANRITSU_TEST_MODE
void CsrWifiSmeMibConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeMibConfigGetCfm *prim = (CsrWifiSmeMibConfigGetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_MIB_CONFIG_GET_CFM prim received - status: %d", prim->status);
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        /*
        CsrDebugTraceLow("CSR_WIFI_SME_MIB_CONFIG_GET_CFM prim received - unifiFixMaxTxDataRate: %d, unifiFixTxDataRate: %d, dot11RtsThreshold: %d, dot11FragmentationThreshold: %d, dot11CurrentTxPowerLevel: %d",
            prim->unifiFixMaxTxDataRate,
            prim->unifiFixTxDataRate,
            prim->dot11RtsThreshold,
            prim->dot11FragmentationThreshold,
            prim->dot11CurrentTxPowerLevel);
        */
        prim->unifiFixTxDataRate = csrWifiAnritsuCurrentRate;
        prim->unifiFixMaxTxDataRate = 1;
        prim->dot11RtsThreshold = 3000;
        CsrDebugTraceLow("CsrWifiSmeMibConfigGetCfmHandler, unifiFixMaxTxDataRate: %d, unifiFixTxDataRate: %d, dot11RtsThreshold: %d",
            prim->unifiFixMaxTxDataRate,
            prim->unifiFixTxDataRate,
            prim->dot11RtsThreshold);
        CsrWifiSmeMibConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE,
            prim->unifiFixMaxTxDataRate,
            prim->unifiFixTxDataRate,
            prim->dot11RtsThreshold,
            prim->dot11FragmentationThreshold,
            prim->dot11CurrentTxPowerLevel);        
    }
    else
        csrWifiAnritsuTestMode = FALSE;
}

void CsrWifiSmeMibConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeMibConfigSetCfm *prim = (CsrWifiSmeMibConfigSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_MIB_CONFIG_SET_CFM prim received - status: %d", prim->status);
    if (prim->status != CSR_WIFI_SME_STATUS_SUCCESS)
        csrWifiAnritsuTestMode = FALSE;
}

void CsrWifiSmeMibGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrUint16 i;
    
    CsrWifiSmeMibGetCfm *prim = (CsrWifiSmeMibGetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_MIB_GET_CFM prim received - status: %d", prim->status);
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        CsrDebugTraceLow("CSR_WIFI_SME_MIB_GET_CFM prim received - mibAttributeLength: %d", prim->mibAttributeLength);
        for (i = 0; i < prim->mibAttributeLength; i++)
            CsrDebugTraceLow("CSR_WIFI_SME_MIB_GET_CFM prim received - mibAttribute[%d] = 0x%x", i, prim->mibAttribute[i]);
    }
    else
        csrWifiAnritsuTestMode = FALSE;
}

void CsrWifiSmeMibSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    /*
    CsrUint8 *mibAttribute1 = NULL;
    CsrUint8 *mibAttribute2 = NULL;
    CsrUint16 mibAttributeLength1;
    CsrUint16 mibAttributeLength2;
    */
    CsrWifiSmeMibSetCfm *prim = (CsrWifiSmeMibSetCfm *) instData->recvMessage;
    
    CsrDebugTraceLow("CSR_WIFI_SME_MIB_SET_CFM prim received - status: %d", prim->status);
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        /*
        mibAttributeLength1 = sizeof(unifiCsrOnlyPatchIntFunctionp27);
        mibAttributeLength2 = sizeof(unifiKeepAliveTime);
        mibAttribute1 = CsrPmemAlloc(mibAttributeLength1);
        mibAttribute2 = CsrPmemAlloc(mibAttributeLength2);
        CsrMemCpy(mibAttribute1, unifiCsrOnlyPatchIntFunctionp27, mibAttributeLength1);
        CsrMemCpy(mibAttribute2, unifiKeepAliveTime, mibAttributeLength2);
        CsrDebugTraceLow("CsrWifiSmeMibSetCfmHandler, mibAttributeLength1: %d, mibAttributeLength2: %d",
            mibAttributeLength1,
            mibAttributeLength2);
        CsrWifiSmeMibGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, mibAttributeLength1, mibAttribute1);
        CsrWifiSmeMibGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, mibAttributeLength2, mibAttribute2);
        */
    }
    else
        csrWifiAnritsuTestMode = FALSE;
}
#endif

void CsrWifiSuppSmePowerConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
    CsrUint8 *mibAttribute1 = NULL;
    CsrUint8 *mibAttribute2 = NULL;
    CsrUint16 mibAttributeLength1;
    CsrUint16 mibAttributeLength2;
#endif
        
    CsrWifiSmePowerConfigSetCfm *prim = (CsrWifiSmePowerConfigSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_POWER_CONFIG_SET_CFM prim received - status: %d", prim->status);
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {            
        if (csrWifiAnritsuTestMode)
        {
            mibAttributeLength1 = sizeof(unifiCsrOnlyPatchIntFunctionp27Equal0);
            mibAttributeLength2 = sizeof(unifiKeepAliveTimeEqual0);
            mibAttribute1 = CsrPmemAlloc(mibAttributeLength1);
            mibAttribute2 = CsrPmemAlloc(mibAttributeLength2);
            CsrMemCpy(mibAttribute1, unifiCsrOnlyPatchIntFunctionp27Equal0, mibAttributeLength1);
            CsrMemCpy(mibAttribute2, unifiKeepAliveTimeEqual0, mibAttributeLength2);        
            CsrDebugTraceLow("CsrWifiSmePowerConfigSetCfmHandler, mibAttributeLength1: %d, mibAttributeLength2: %d",
                mibAttributeLength1,
                mibAttributeLength2);
            CsrWifiSmeMibSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, mibAttributeLength1, mibAttribute1);
            CsrWifiSmeMibSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, mibAttributeLength2, mibAttribute2);
        }
    }
    else
        csrWifiAnritsuTestMode = FALSE;
#endif
}

void CsrWifiSuppSmePowerConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmePowerConfigGetCfm *prim = (CsrWifiSmePowerConfigGetCfm *) instData->recvMessage;

    // Workaround MMI send commands with the same pars at once
    /*
    CsrDebugTraceLow("CSR_WIFI_SME_POWER_CONFIG_GET_CFM target - command_id: %d, para1: %d",
        instData->command_info_ptr->command_id,
        instData->command_info_ptr->para1);
    */
    CsrDebugTraceLow("CSR_WIFI_SME_POWER_CONFIG_GET_CFM target - state: %d", instData->command_info_ptr->para1);
    CsrDebugTraceLow("CSR_WIFI_SME_POWER_CONFIG_GET_CFM prim received - status: %d, powerSaveLevel: %d",
        prim->status,
        prim->powerSaveLevel);
    if (instData->command_info_ptr->para1)
    {
        prim->powerSaveLevel = CSR_WIFI_SME_POWER_SAVE_LEVEL_LOW;
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
        // Temporary use power save ON to determine Anritsu test mode ON
        csrWifiAnritsuTestMode = TRUE;
#endif
    }
    else
    {
        prim->powerSaveLevel = CSR_WIFI_SME_POWER_SAVE_LEVEL_MED;
#ifdef CSR_WIFI_ANRITSU_TEST_MODE
        // Temporary use power save OFF to determine Anritsu test mode OFF
        csrWifiAnritsuTestMode = FALSE;
#endif
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
}

void CsrWifiSuppSmeRoamingConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeRoamingConfigSetCfm *prim = (CsrWifiSmeRoamingConfigSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_ROAMING_CONFIG_SET_CFM prim received - status: %d", prim->status);
}

void CsrWifiSuppSmeRoamingConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeRoamingConfigGetCfm *prim = (CsrWifiSmeRoamingConfigGetCfm *) instData->recvMessage;

    // Workaround MMI send commands with the same pars at once
    /*
    CsrDebugTraceLow("CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM target - command_id: %d, para1: %d",
        instData->command_info_ptr->command_id,
        instData->command_info_ptr->para1);
    */
    CsrDebugTraceLow("CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM target - state: %d", instData->command_info_ptr->para2);
    CsrDebugTraceLow("CSR_WIFI_SME_ROAMING_CONFIG_GET_CFM prim received - status: %d, disableRoamScans: %d",
        prim->status,
        prim->disableRoamScans);
    // Workaround MMI send commands with the same pars at once
    // if (instData->command_info_ptr->para1)
    if (instData->command_info_ptr->para2)
        prim->disableRoamScans = FALSE;
    else
        prim->disableRoamScans = TRUE;
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
}

void CsrWifiSuppSmeScanFullCfmHandler(csrWifiSupplicantInstanceData * instData)
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
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
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

void CsrWifiSuppSmeScanResultsGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrUint16 start = 0,
                   end = 0,
                   i;
    CsrBool found = FALSE;
    WIFISUPP_SCAN_CNF_T resultScanCnf;
    WIFISUPP_SCAN_AP_INFO_T *resultData = NULL;

    CsrWifiSmeScanResultsGetCfm * prim = (CsrWifiSmeScanResultsGetCfm *) instData->recvMessage;

    resultScanCnf.scan_result = WIFISUPP_RESULT_FAIL;
    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
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

void CsrWifiSuppSmeConnectionInfoGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeConnectionInfoGetCfm *prim = (CsrWifiSmeConnectionInfoGetCfm *) instData->recvMessage;
    instData->currentConnectionInfo = prim;

    CsrDebugTraceLow("CSR_WIFI_SME_CONNECTION_INFO_CFM prim received status: %d - ssid: %.*s", prim->status, prim->ssid.length, prim->ssid.ssid);

    // Set to NULL to avoid prim from being freed
    instData->recvMessage = NULL;
}

void CsrWifiSuppSmeConnectionQualityIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeConnectionQualityInd *prim = (CsrWifiSmeConnectionQualityInd *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_CONNECTION_QUALITY_IND prim received - rssi: %d - snr: %d",
                     prim->unifiRssi,
                     prim->unifiSnr);

    if (instData->currentConnectionInfo)
    {
        static WIFISUPP_RSSI_CHANGED_INFO_T resultData;

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

        // Store pointer and clear recvMessage pointer as it gets freed later on otherwise
        // We need the rssi value for later 
        CsrPmemFree(instData->currentConnectionQuality);
        instData->currentConnectionQuality = prim;
        instData->recvMessage = NULL;
// TODO: Still needs to be implemented, currently memory is corrupted when this is called?!
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_RSSI_CHANGED_IND, &resultData);
    }
}

void CsrWifiSuppSmeDisconnectCfmHandler(csrWifiSupplicantInstanceData * instData)
{  
    CsrWifiSmeDisconnectCfm *prim = (CsrWifiSmeDisconnectCfm *) instData->recvMessage;

    // SR30032: debug and fix assert here
    CsrDebugTraceLow("*****%s: BEGIN", __func__);

    // SR30032: debug and fix assert here
    if (instData->disconnectReq == NULL)
    {
        CsrDebugTraceLow("*****%s: Unexpected instData->disconnectReq == NULL", __func__);
        if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
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

    CsrDebugTraceLow("CSR_WIFI_SME_DISCONNECT_CFM prim received status: %d - tried to disconnect from: %.*s",
                     prim->status,
                     instData->disconnectReq->ssid.ssid_len,
                     instData->disconnectReq->ssid.ssid);

    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
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
#if 0
    CsrWifiSmeDisconnectCfm *prim = (CsrWifiSmeDisconnectCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_DISCONNECT_CFM prim received status: %d - tried to disconnect from: %.*s",
                     prim->status,
                     instData->disconnectReq->ssid.ssid_len,
                     instData->disconnectReq->ssid.ssid);

    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        instData->disconnectReq->result = WIFISUPP_RESULT_SUCC;

        /* Free all data associated with this current connection */
        CsrWifiSmeFreeUpstreamMessageContents(CSR_WIFI_SME_PRIM, instData->currentConnectionInfo);
        CsrPmemFree(instData->currentConnectionInfo);
        instData->currentConnectionInfo = NULL;
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
#endif
}

void CsrWifiSuppSmeHostConfigGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeHostConfigGetCfm * prim = (CsrWifiSmeHostConfigGetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_HOST_CONFIG_GET_CFM prim received status: %d, PowerMode: %d, appDataPeriodMs: %d", prim->status, prim->powerMode, prim->applicationDataPeriodMs);

    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        CsrWifiSmeHostConfigSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, CSR_WIFI_SME_HOST_POWER_MODE_POWER_SAVE, prim->applicationDataPeriodMs);
    }
}

void CsrWifiSuppSmeHostConfigSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeHostConfigSetCfm * prim = (CsrWifiSmeHostConfigSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_HOST_CONFIG_SET_CFM prim received status: %d", prim->status);

    if (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)
    {
        /* If CsrWifiSupplicantInitialize() is called with TRUE as the parameter, we want strict mode! */
        if (csrWifiEnableStrictComplianceMode == TRUE)
        {
            // TODO: Implement dynamic toggle for TKIP + DraftN compliance - default is NO
        }
    }
}

void CsrWifiSuppSmeEventMaskSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeEventMaskSetCfm *prim = (CsrWifiSmeEventMaskSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_SME_EVENT_MASK_SET_CFM prim received status: %d", prim->status);
}

void CsrWifiSuppSmeIbssStationIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeIbssStationInd *prim = (CsrWifiSmeIbssStationInd *) instData->recvMessage;
    WIFISUPP_BSSID_CHANGED_INFO_T resultData;

    CsrDebugTraceLow("CSR_WIFI_SME_IBSS_STATION_IND prim received - address: %02X:%02X:%02X:%02X:%02X:%02X - connected: %d",
                     prim->address.a[0], prim->address.a[1], prim->address.a[2], prim->address.a[3], prim->address.a[4], prim->address.a[5],
                     prim->isconnected);

    CsrMemCpy(&resultData.ssid, &instData->currentConnectionInfo->ssid, sizeof(CsrWifiSsid));
    CsrMemCpy(resultData.new_bssid_info.bssid, prim->address.a, WIFISUPP_BSSID_LEN);

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_RSSI_CHANGED_IND, &resultData);

    if (!prim->isconnected)
    { /* Ibss changed, we are no longer connected, free connectInfo */
        WIFISUPP_DISCONNECT_INFO_T disconnectInd;

        CsrMemCpy(&disconnectInd.ssid, &instData->currentConnectionInfo->ssid, sizeof(CsrWifiSsid));
	
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_DISCONNECT_IND, &disconnectInd);
        CsrWifiSmeFreeUpstreamMessageContents(CSR_WIFI_SME_PRIM, instData->currentConnectionInfo);
        CsrPmemFree(instData->currentConnectionInfo);
        instData->currentConnectionInfo = NULL;
    }
}

void CsrWifiSuppSmeLinkQualityGetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeLinkQualityGetCfm *prim = (CsrWifiSmeLinkQualityGetCfm *) instData->recvMessage;
	
    /* if supplicant is under connection process */
    if (instData->connectReq != NULL && CsrRSSITimerBeSet == RSSI_POLLING_TIMER_INIT) 
	{
        WIFISUPP_CONNECT_RESULT_T resultData;
        CsrWifiSmeConnectionInfoGetCfm *connInfo = instData->currentConnectionInfo;        
        resultData.result = WIFISUPP_RESULT_SUCC;   

		CsrDebugTraceLow("CsrWifiSuppSmeLinkQualityGetCfmHandler rssi: %d", prim->unifiRssi);
        if (connInfo != NULL)
        {
            /* sr 23831 */
            CsrMemCpy(&(resultData.ssid), &connInfo->ssid, sizeof(WIFISUPP_SSID_T));
            CsrMemCpy(resultData.bssid_info.bssid, &connInfo->bssid, WIFISUPP_BSSID_LEN);
            resultData.channel = connInfo->channelNumber;
        }
        else
        {
            CsrMemCpy(&(resultData.ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
            CsrMemCpy(resultData.bssid_info.bssid, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);
            resultData.channel = instData->connectReq->channel;
        }
        resultData.rssi = (prim->status == CSR_WIFI_SME_STATUS_SUCCESS)? prim->unifiRssi : -90;
        
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

        CsrMemCpy(&resultData.ssid, &connInfo->ssid, sizeof(WIFISUPP_SSID_T));
        CsrMemCpy(resultData.bssid_info.bssid, &connInfo->bssid, WIFISUPP_BSSID_LEN);
        resultData.new_signal_qua = prim->unifiRssi;

		//CsrDebugTraceLow("CsrWifiSuppSmeLinkQualityGetCfmHandler rssi change: %d", prim->unifiRssi);
		
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_RSSI_CHANGED_IND, &resultData);
	}
}

void CsrWifiSuppSmeMediaStatusIndHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiSmeMediaStatusInd *prim = (CsrWifiSmeMediaStatusInd *) instData->recvMessage;
    
    CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  Status: %d, prim->ssid: %.*s - assocRspResult: %d!!",
         prim->mediaStatus,
         prim->ssid.length,
         prim->ssid.ssid,
         prim->assocRspResult);

    if (prim->mediaStatus == CSR_WIFI_SME_MEDIA_STATUS_DISCONNECTED &&
        instData->disconnectReq == NULL)
    {
        WIFISUPP_DISCONNECT_INFO_T resultData;

        CsrDebugTraceLow("===== CSR_WIFI_SME_MEDIA_STATUS_IND  - Doing disconnect stuff");

        CsrMemCpy(&resultData.ssid, &prim->ssid, sizeof(WIFISUPP_SSID_T));

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
            if (!CsrStrNCmp((CsrCharString *)prim->ssid.ssid, (CsrCharString *)instData->currentConnectionInfo->ssid.ssid, (CsrSize)instData->currentConnectionInfo->ssid.length))
            {
                // Same SSID
                CsrUint16 i;
                CsrBool roam = FALSE;
                for (i = 0; i < sizeof(CsrWifiMacAddress); i++)
                {
                    if (prim->bssid.a[i] != instData->currentConnectionInfo->bssid.a[i])
                    {
                        roam = TRUE;
                        break;
                    }
                }
                if (roam)
                {
                    // Diff BSSID - therefore notify MMI roaming has occurred
                    static WIFISUPP_BSSID_CHANGED_INFO_T resultData;
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
                    SUPPMAIN_NotifyApp(WIFISUPP_SIG_BSSID_CHANGED_IND, &resultData);
                }
            }
        }
        
        CsrWifiSmeConnectionInfoGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
    }
}

void CsrWifiSuppNmeProfileSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiNmeProfileSetCfm *prim = (CsrWifiNmeProfileSetCfm *) instData->recvMessage;

    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_SET_CFM received - status: %d - ssid: %.*s",
                     prim->status,
                     instData->connectReq->ssid.ssid_len,
                     instData->connectReq->ssid.ssid);

    if (prim->status == CSR_WIFI_NME_STATUS_SUCCESS)
    {
        CsrWifiNmeProfileIdentity profile;

        CsrMemCpy(&(profile.ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
//        CsrMemCpy(profile.bssid.a, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);
        CsrMemSet(profile.bssid.a, 0xFF, WIFISUPP_BSSID_LEN);

        CsrWifiNmeProfileConnectReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, profile);
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

void CsrWifiSuppNmeProfileOrderSetCfmHandler(csrWifiSupplicantInstanceData * instData)
{
    CsrWifiNmeProfileOrderSetCfm *prim = (CsrWifiNmeProfileOrderSetCfm *) instData->recvMessage;
    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_ORDER_SET_CFM prim received - status: %d", prim->status);

#ifdef CSR_WIFI_NME_AUTOCONNECT
    if (instData->disconnectReq != NULL)
    {
        CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_ORDER_SET_CFM - calling SmeDisconnectReq");
        CsrWifiSmeDisconnectReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
    }
#endif
}

void CsrWifiSuppNmeProfileConnectCfmHandler(csrWifiSupplicantInstanceData *instData)
{
    CsrWifiNmeProfileConnectCfm *prim = (CsrWifiNmeProfileConnectCfm *) instData->recvMessage;
    WIFISUPP_CONNECT_RESULT_T resultData;
    uint16 i;
	
    CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM prim received -status: %d", prim->status);

    for (i = 0; i < prim->connectAttemptsCount; i++)
    {
        CsrDebugTraceLow("BSSID: %02X:%02X:%02X:%02X:%02X:%02X", prim->connectAttempts[i].bssid.a[0], prim->connectAttempts[i].bssid.a[1], prim->connectAttempts[i].bssid.a[2],
                         prim->connectAttempts[i].bssid.a[3], prim->connectAttempts[i].bssid.a[4], prim->connectAttempts[i].bssid.a[5]);
        CsrDebugTraceLow("Status: %d, Security Error: %d", prim->connectAttempts[i].status, prim->connectAttempts[i].securityError);
    }


	/* Only try Shared if connectReq is empty (since we clear it after trying Shared)
       and if the connect result was a failure (due to AP using Shared, phone using Open */
    if (prim->status == CSR_WIFI_NME_STATUS_ERROR &&
        instData->connectReq != NULL &&
        instData->connectReq->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WEP &&
        instData->sharedConnect == FALSE)
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
        profile.wmmQosCapabilitiesMask = 0;

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
        if (prim->status == CSR_WIFI_NME_STATUS_SUCCESS)
        {
#ifdef CSR_WIFI_NME_AUTOCONNECT
            CsrWifiNmeProfileIdentity *profileIdentitys;
            CsrBool alreadyKnown = FALSE;

            alreadyKnown = findNmeProfileInSet(instData, instData->connectReq);

            CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM - alreadyKnown: %d", alreadyKnown);

            if (!alreadyKnown)
            {
                // Allocate memory for the previous elements + 1 new element
                profileIdentitys = CsrPmemAlloc((instData->profileIdentitysCount + 1) * sizeof(CsrWifiNmeProfileIdentity));
				CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM - alreadyKnown profileIdentitys : %x", profileIdentitys);
                // Copy all the old elements to the new pointer, setup new element, increase count, free old pointer and use new one
				CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM - alreadyKnown profileIdentitysCount : %d", instData->profileIdentitysCount);
                if (instData->profileIdentitysCount == 0)
                {
                    // Zero previous elements, nothing to copy
                }
                else
                {
                    CsrMemCpy(&profileIdentitys[1], instData->profileIdentitys, instData->profileIdentitysCount * sizeof(CsrWifiNmeProfileIdentity));
                }

                // Setup new element with data
                CsrMemCpy(&(profileIdentitys[0].ssid), &(instData->connectReq->ssid), sizeof(WIFISUPP_SSID_T));
                CsrMemCpy(profileIdentitys[0].bssid.a, instData->connectReq->bssid_info.bssid, WIFISUPP_BSSID_LEN);
//                CsrMemSet(&profileIdentitys[0].bssid.a, 0xFF, WIFISUPP_BSSID_LEN);
				//CsrDebugTraceHigh("CSR_WIFI_NME_PROFILE_CONNECT_CFM - alreadyKnown [2] bssid.a : %02X:%02X:%02X:%02X:%02X:%02X", profileIdentitys[0].bssid.a[0],profileIdentitys[0].bssid.a[1],profileIdentitys[0].bssid.a[2],profileIdentitys[0].bssid.a[3],profileIdentitys[0].bssid.a[4],profileIdentitys[0].bssid.a[5]);

				instData->profileIdentitysCount++;
                CsrPmemFree(instData->profileIdentitys);
                instData->profileIdentitys = profileIdentitys;
                profileIdentitys = NULL;
				
				CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM - alreadyKnown [2] profileIdentitysCount : %d", instData->profileIdentitysCount);
            }

            // Make copy of profileIdentitys for use with lib call
            profileIdentitys = CsrPmemAlloc(instData->profileIdentitysCount * sizeof(CsrWifiNmeProfileIdentity));
			CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM - [3] profileIdentitys  %x", profileIdentitys);
            CsrMemCpy(profileIdentitys, instData->profileIdentitys, instData->profileIdentitysCount * sizeof(CsrWifiNmeProfileIdentity));

            CsrDebugTraceLow("CSR_WIFI_NME_PROFILE_CONNECT_CFM prim - OrderSet count: %d, ptr: 0x%08X | first: ssid_len: %d - ssid(%.*s)",
                             instData->profileIdentitysCount,
                             instData->profileIdentitys,
                             instData->profileIdentitys[0].ssid.length,
                             instData->profileIdentitys[0].ssid.length,
                             instData->profileIdentitys[0].ssid.ssid);

            CsrWifiNmeProfileOrderSetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE, instData->profileIdentitysCount, profileIdentitys);
            //profileIdentitys = NULL;
#endif
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
            CsrWifiSmeLinkQualityGetReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
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
