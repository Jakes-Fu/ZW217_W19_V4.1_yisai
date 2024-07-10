/****************************************************************************************
** File Name:      wifisupp_adap_none.c                                                 *
** Author:         juan.zhang                                                           *
** Date:           2009.10.29                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This is the file that a custom supplicant to start with              *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.10       juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "cfl_mem.h"
#include "wifisupp_internal.h"
#include "uni8910_wifi.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//customized signals are added here
typedef enum
{
    NONE_MESSAGE_START = WIFISUPP_SIG_CUSTOM_START,
    NONE_MESSAGE_MAX = WIFISUPP_SIG_CUSTOM_END
}NONE_MESSAGE_E;

typedef enum
{
    WIFI_OFF = 0,
    WIFI_ON = 1,
    WIFI_SCANING = 2
}WIFI_STATUS_E;
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
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN Uni8910WifiOn(void);
LOCAL BOOLEAN Uni8910WifiOff(void);
LOCAL BOOLEAN Uni8910WifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr);


/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
SUPPADAP_CUSTOM_INFO_T g_suppadap_unisoc_info = 
{
    {
        Uni8910WifiOn,
        Uni8910WifiOff,
        Uni8910WifiScan,
        PNULL,
        PNULL,
        PNULL,
        PNULL,
        PNULL,
    },  //SUPPADAP_REG_OPER_FUNC_T
    FALSE,//is_support_roam
    {
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_OPENSYS
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_WEP
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_WPA
        FALSE, //WIFISUPP_ENCRYP_PROTOCOL_WPA2
        FALSE  //WIFISUPP_ENCRYP_PROTOCOL_WAPI
    },  //is_support_protocol
    {
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_TLS
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_TTLS
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_PEAP
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_LEAP
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_SIM
        FALSE,   //WIFISUPP_WPA_EAP_TYPE_AKA
        FALSE    //WIFISUPP_WPA_EAP_TYPE_FAST
    },  //is_support_eap
    {
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_CHAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_PAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP
        FALSE,   //WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP_V2
        FALSE    //WIFISUPP_INNER_CREDENTIAL_TYPE_GTC
    }//is_support_inner_eap
};

extern BOOLEAN uni8910_wifi_on(void); 
extern BOOLEAN uni8910_wifi_off(void); 
extern BOOLEAN uni8910_wifi_scan(WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr);
/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//Description : wifi on
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Uni8910WifiOn(void)
{
    SCI_TRACE_LOW:("&&&&&&&&&uni8910WifiOn()\n");
    return uni8910_wifi_on();
}

/****************************************************************************/
//Description : wifi off
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Uni8910WifiOff(void)
{
    SCI_TRACE_LOW:("&&&&&&&&&uni8910WifiOff()\n");
    return uni8910_wifi_off();
}

/****************************************************************************/
//Description : scan
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN Uni8910WifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr)
{
    SCI_TRACE_LOW:("&&&&&&&&&uni8910Wifiscan()\n");
    return uni8910_wifi_scan(scan_req_ptr);
}

char uni8910_release_version[64] = "WIFI_UNISOC8910";
char * WIFI_GetVersion(void)
{   
    return uni8910_release_version;
}
void iperf_test_start(char* param)
{
}


/*---------------------------------------------------------------------*/
/* mmi -- wifi_supplicant*/

#define SCAN_NUM_MAX WF_AP_MAX
extern int wifi8910_init_flag;
WIFI_STATUS_E wifirx_stat = WIFI_OFF;
extern wifiApInfo_t *aps;
unsigned char uni8910_wifi_on()
{
    WIFISUPP_ON_RESULT_T on_result = {0};
    on_result.wifi_on_result = WIFISUPP_RESULT_FAIL;

    SCI_TRACE_LOW("uni8910wifirx,uni8910_wifi_on,stat=%d,tick=%d",wifirx_stat,SCI_GetTickCount());

    if(!wifi8910_init_flag)
    {
        on_result.wifi_on_result = WIFISUPP_RESULT_FAIL;
        SCI_TRACE_LOW("uni8910wifirx,uni8910_wifi_on,res=%d",on_result.wifi_on_result);
        SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &on_result);
        return FALSE;
    }
    if(wifirx_stat == WIFI_OFF)
    {
        wifirx_stat = WIFI_ON;
        drvWifiOpen();
    }
    on_result.wifi_on_result = WIFISUPP_RESULT_SUCC;


    SUPPMAIN_NotifyApp(WIFISUPP_SIG_ON_CNF, &on_result);
    SCI_TRACE_LOW("uni8910wifirx,uni8910_wifi_on,res=%d,stat=%d,tick=%d",on_result.wifi_on_result,wifirx_stat,SCI_GetTickCount());
    return TRUE;
}

unsigned char uni8910_wifi_off()
{
    WIFISUPP_OFF_RESULT_T off_result = {0};

    off_result.wifi_off_result = WIFISUPP_RESULT_SUCC;
    SCI_TRACE_LOW("uni8910wifirx,uni8910_wifi_off,stat=%d,tick=%d",wifirx_stat,SCI_GetTickCount());
    if(wifirx_stat == WIFI_ON)
    {
        drvWifiClose();
        wifirx_stat = WIFI_OFF;
        off_result.wifi_off_result = WIFISUPP_RESULT_SUCC;
    }
    else if(wifirx_stat == WIFI_OFF)
    {
        off_result.wifi_off_result = WIFISUPP_RESULT_SUCC;
    }
    else
    {
        off_result.wifi_off_result = WIFISUPP_RESULT_FAIL;
    }

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_OFF_CNF, &off_result);
    SCI_TRACE_LOW("uni8910wifirx,uni8910_wifi_off,res=%d,stat=%d,tick=%d",off_result.wifi_off_result, wifirx_stat,SCI_GetTickCount());
    if(off_result.wifi_off_result == WIFISUPP_RESULT_SUCC)
        return TRUE;
    else
        return FALSE;
}

//check Little-Endian OR Big-Endian
static int check()  
{
    union check  
    {  
        int i;
        char ch;
    }c;
    c.i  =1;
    return (c.ch == 1);
}
static void i16toa(uint8* mac, uint16 high)
{
    char* chPtr = (char*)&high;
    {
        mac[1] = chPtr[0];
        mac[0] = chPtr[1];
    }
}
static void i16toa_big(uint8* mac, uint16 high)
{
    char* chPtr = (char*)&high;

    {
        mac[0] = chPtr[0];
        mac[1] = chPtr[1];
    }
}
static void i32toa(uint8* mac, uint32 low)
{
    char* chPtr = (char*)&low;
    {
        mac[3] = chPtr[0];
        mac[2] = chPtr[1];
        mac[1] = chPtr[2];
        mac[0] = chPtr[3];
    }
}
static void i32toa_big(uint8* mac, uint32 low)
{
    char* chPtr = (char*)&low;
    {
        mac[0] = chPtr[0];
        mac[1] = chPtr[1];
        mac[2] = chPtr[2];
        mac[3] = chPtr[3];
    }
}
static void macFill(uint8 *mac, uint16 high, uint32 low)
{
    i16toa(mac, high);
    i32toa(mac+2, low);
}

static void macFill_big(uint8 *mac, uint16 high, uint32 low)
{
    i16toa_big(mac, high);
    i32toa_big(mac+2, low);
}
unsigned char uni8910_wifi_scan(WIFISUPP_SCAN_REQ_INFO_T* scan_info_ptr)
{
    WIFISUPP_SCAN_AP_INFO_T ap_info = {0};
    WIFISUPP_RESULT_E scan_result = 0;

    SCI_TRACE_LOW("uni8910wifirx,uni8910_wifi_scan,stat=%d,tick=%d", wifirx_stat,SCI_GetTickCount());
    if(wifirx_stat == WIFI_ON)
    {
        wifiScanRequest_t req;
        wifirx_stat = WIFI_SCANING;

        req.aps = aps;
        req.max = SCAN_NUM_MAX;
        req.found = 0;
#ifdef ZDT_PLAT_YX_SUPPORT
        req.maxtimeout = 240;//120 multiply of 120ms is better
#else
        req.maxtimeout = 120;//multiply of 120ms is better
#endif
        drvWifiScanAllChannel(&req);
        if (req.found != 0)
        {
            uint32 i ;
            SCI_TRACE_LOW("uni8910wifirx,found ap num=%d",req.found);
            if(check())
            {
                for (i = 0; i < req.found; i++)
                {
                    wifiApInfo_t *apRes = &req.aps[i];
                    //SCI_TRACE_LOW("uni8910wifirx,found ap - {mac address : %04x%08lx, rssival: %i dBm, channel: %u}",
                    //             apRes->bssid_high, apRes->bssid_low, apRes->rssival, apRes->channel);
                    macFill((uint8 *)(ap_info.bssid_info.bssid), apRes->bssid_high, apRes->bssid_low);
                    ap_info.signal_qua = apRes->rssival+WIFI_RSSI_CALIB_OFFSET;
                    if(ap_info.signal_qua >= 0)
                        ap_info.signal_qua = -8;
                    ap_info.channel = apRes->channel;
                    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_IND, &ap_info);
                }
            }
            else
            {
                for (i = 0; i < req.found; i++)
                {
                    wifiApInfo_t *apRes = &req.aps[i];
                    //SCI_TRACE_LOW("uni8910wifirx,found ap - {mac address : %04x%08lx, rssival: %i dBm, channel: %u}",
                    //             apRes->bssid_high, apRes->bssid_low, apRes->rssival, apRes->channel);
                    macFill_big((uint8 *)(ap_info.bssid_info.bssid), apRes->bssid_high, apRes->bssid_low);
                    ap_info.signal_qua = apRes->rssival+WIFI_RSSI_CALIB_OFFSET;
                    if(ap_info.signal_qua >= 0)
                        ap_info.signal_qua = -8;
                    ap_info.channel = apRes->channel;
                    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_IND, &ap_info);
                }
            }
        }
        else
        {
            SCI_TRACE_LOW( "uni8910wifirx,wifi scan not found ap\n");
        }

        if(req.found)
            scan_result = WIFISUPP_RESULT_SUCC;
        else
            scan_result = WIFISUPP_RESULT_NOT_FOUND;
        wifirx_stat = WIFI_ON;
    }
    else
    {
        scan_result = WIFISUPP_RESULT_FAIL;
    }

    SUPPMAIN_NotifyApp(WIFISUPP_SIG_SCAN_CNF, &scan_result);
    SCI_TRACE_LOW("uni8910wifirx,uni8910_wifi_scan,res=%d,stat=%d,tick=%d",scan_result, wifirx_stat,SCI_GetTickCount());
    if(scan_result == WIFISUPP_RESULT_SUCC)
        return TRUE;
   else
        return FALSE;
}

