/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

****************************************************************************/

#include <stdarg.h>

#include "os_api.h"
#include "csr_rel_ver.h"

/*lint -e838*/
// Using platform build system, so need to define it here also
#define SCI_CSR_LOG_ENABLE

#ifdef SCI_CSR_LOG_ENABLE
#define DEBUG_BUFFER_SIZE       (1000)
static char CsrLogBuffer[DEBUG_BUFFER_SIZE] = ""; /*lint !e785 */

/* buffer[len - 1] = '\0'; line removes \n from the formated string */
#define FORMAT_TRACE(buffer, buf_len, fmt)       \
{                                                \
    unsigned int len = 0;                        \
    va_list args;                                \
    va_start(args, fmt);                         \
    len = vsnprintf((buffer),                    \
                    (buf_len),                   \
                    fmt,                         \
                    args);                       \
    va_end(args);                                \
    if (len >= (buf_len))                        \
    {                                            \
        (buffer)[(buf_len) - 1] = '\0';          \
    }                                            \
    else                                         \
    {                                            \
        (buffer)[len] = '\0';                    \
    }                                            \
}


#if defined(WIFI_SUPPORT_CSR6030)
static char CsrLogPrefix[] = "CSR6030 "CSR_RELEASE_VERSION;
#else
static char CsrLogPrefix[] = "CSR6030 InvVer";
#endif

#endif

void CsrDebugTraceLow(const char* const format, ...)
{
#ifdef SCI_CSR_LOG_ENABLE
    FORMAT_TRACE(CsrLogBuffer, DEBUG_BUFFER_SIZE, format);/*lint !e64 -e628 */
    SCI_TRACE_LOW("%s:%s", CsrLogPrefix, CsrLogBuffer); /*lint !e64 */
#endif
}

/*
 Sme Logging Categories 
#define TR_SME                      0x0000
#define TR_CORE                     0x0001
#define TR_UNIFI_DRIVER_FSM         0x0002
#define TR_MIB_ACCESS_FSM           0x0003
#define TR_MIB_ACCESS               0x0004
#define TR_SME_CONFIGURATION_FSM    0x0005
#define TR_SCAN                     0x0006
#define TR_SCAN_STORAGE             0x0007
#define TR_LINK_QUALITY             0x0008
#define TR_HIP_PROXY                0x0009
#define TR_SECURITY                 0x000A
#define TR_NETWORK_SELECTOR_FSM     0x000B
#define TR_CXN_MGR                  0x000C
#define TR_SME_MEASUREMENTS_FSM     0x900D
#define TR_POWER_MGR                0x000E
#define TR_COEX                     0x000F
#define TR_DOT11N                   0x0010
#define TR_REGDOM                   0x0011
#define TR_QOS                      0x0012
#define TR_DBG                      0x0013
#define TR_PAYLOAD_MGR              0x0014
#define TR_IE_ACCESS                0x0015
#define TR_IE_PRINT                 0x0016
#define TR_HIP_AUTO                 0x0017
#define TR_ROUTER_SAP               0x0018

#define TR_PAL_MGR_FSM              0x0019
#define TR_PAL_LM_LINK_FSM          0x001A
#define TR_PAL_DM_FSM               0x001B
#define TR_PAL_COEX_FSM             0x001C
#define TR_PAL_LM_HIP_FSM           0x001D
#define TR_PAL_DAM                  0x001E

#define TR_CCX                      0x0020
#define TR_CCX_LTST                 0x0021
#define TR_CCX_IAPP                 0x0022
#define TR_DEVICE_CONFIG_INFO_FSM   0x0023
#define TR_SME_CONTROL              0x0024
#define TR_PEER_MGR                 0x0025
#define TR_HIP_SAP                  0x0026
#define TR_AP_MANAGER_FSM           0x0027
#define TR_AP_CONN_MANAGER_FSM      0x0028
#define TR_AP_SEC_MANAGER_FSM       0x0029
#define TR_AP_PEER_FSM              0x0030
#define TR_AP_COMMON                0x0031
#define TR_P2P_MANAGER_FSM          0x0032
#define TR_AP_RES_MANAGER_FSM       0x0033
#define TR_AP_BLOCK_ACK_TX_FSM      0x0034
#define TR_AP_BLOCK_ACK_RX_FSM      0x0035

#define TR_PMSGUTILS                0x0036
#define TR_80211_MSG_ENCODE         0x0037
#define TR_80211_MSG_DECODE         0x0038
*/
/* For Security log 
#define CSR_SECURITY_LOG_ID 0x12FF0005
	
Security Logging Categories 
#define TR_SECURITY_LIB              0x0000
#define TR_SECURITY_WAPI             0x0001
#define TR_SECURITY_EAP              0x0002
#define TR_SECURITY_PEAP             0x0003
#define TR_SECURITY_TLS              0x0004
#define TR_SECURITY_TTLS             0x0005
#define TR_SECURITY_SIMAKA           0x0006
#define TR_SECURITY_FAST             0x0007
#define TR_SECURITY_LEAP             0x0008
#define TR_SECURITY_HANDSHAKE        0x0009
#define TR_SECURITY_WPS              0x000a
*/

void CsrTraceSmeLog(int id, unsigned short sub, const char * const format,...)
{
#ifdef SCI_CSR_LOG_ENABLE
#define LOG_CSR_WIFI_SME_LOG_ID 0x1202FFFF
#define LOG_CSR_SECURITY_LOG_ID 0x12FF0005
#define DEFAULT_LOG_LEVEL 0x000C //TR_CXN_MGR

	if ((id == LOG_CSR_WIFI_SME_LOG_ID && sub <= DEFAULT_LOG_LEVEL) || 
		id == LOG_CSR_SECURITY_LOG_ID)
	{
    	FORMAT_TRACE(CsrLogBuffer, DEBUG_BUFFER_SIZE, format);/*lint !e64 -e628 */
    	SCI_TRACE_LOW("%s:%s", CsrLogPrefix, CsrLogBuffer); /*lint !e64 */
	}
#undef LOG_CSR_WIFI_SME_LOG_ID
#undef LOG_CSR_SECURITY_LOG_ID
#endif
}

void CsrDebugTraceHigh(const char* const format, ...)
{
#ifdef SCI_CSR_LOG_ENABLE
    FORMAT_TRACE(CsrLogBuffer, DEBUG_BUFFER_SIZE, format);/*lint !e64 */
    SCI_TRACE_LOW("%s:%s", CsrLogPrefix, CsrLogBuffer); /*lint !e64 */
#endif
}


void csr_trace_simple(int id, const char* const format, ...)
{
#ifdef SCI_CSR_LOG_ENABLE
    FORMAT_TRACE(CsrLogBuffer, DEBUG_BUFFER_SIZE, format);/*lint !e64 */
    SCI_TRACE_LOW("%s:%s", CsrLogPrefix, CsrLogBuffer); /*lint !e64 */
#endif
}

