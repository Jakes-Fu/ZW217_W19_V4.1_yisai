/****************************************************************************************
** File Name:      wifisupp_main.c                                                      *
** Author:         juan.zhang                                                           *
** Date:           2009.10.29                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is used for wifi supplicant signal dispatch                *
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
#include "sci_api.h"
#include "cfl_mem.h"
#include "wifisupp_api.h"
#include "wifisupp_internal.h"
#include "priority_app.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define WIFISUPP_TASK_STACK_SIZE    10240
#define WIFISUPP_TASK_QUEUE_NUM     200
#define WIFISUPP_TASK_PRIORITY      PRI_WIFISUPP_TASK  //26            

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/

typedef struct
{
    BLOCK_ID    supp_thread_id;
    BLOCK_ID    app_thread_id;
} SUPP_INSTANCE_T;


/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
#ifdef WIN32
    extern SUPPADAP_CUSTOM_INFO_T  g_suppadap_none_info;
    static SUPPADAP_CUSTOM_INFO_T  *s_suppadap_custom_info_ptr = &g_suppadap_none_info;
#elif defined WIFI_VENDOR_CSR
    extern SUPPADAP_CUSTOM_INFO_T  g_suppadap_csr_info;
    static SUPPADAP_CUSTOM_INFO_T  *s_suppadap_custom_info_ptr = &g_suppadap_csr_info;
#elif defined WIFI_VENDOR_ATHEROS
    extern SUPPADAP_CUSTOM_INFO_T  g_suppadap_atheros_info;
    static SUPPADAP_CUSTOM_INFO_T  *s_suppadap_custom_info_ptr = &g_suppadap_atheros_info;
#elif defined WIFI_VENDOR_REALTEK
    extern SUPPADAP_CUSTOM_INFO_T  g_suppadap_realtek_info;
    static SUPPADAP_CUSTOM_INFO_T  *s_suppadap_custom_info_ptr = &g_suppadap_realtek_info;
#elif defined WIFI_VENDOR_UNISOC
    extern SUPPADAP_CUSTOM_INFO_T  g_suppadap_unisoc_info;
    static SUPPADAP_CUSTOM_INFO_T  *s_suppadap_custom_info_ptr = &g_suppadap_unisoc_info;
#else 
    static SUPPADAP_CUSTOM_INFO_T  *s_suppadap_custom_info_ptr = PNULL;
#endif


/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
static SUPP_INSTANCE_T      s_wifisupp_instance = {0};

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN WifiOn(BLOCK_ID app_thread_id);
LOCAL BOOLEAN WifiOff(void);
LOCAL BOOLEAN WifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr);
LOCAL BOOLEAN WifiConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr);
LOCAL BOOLEAN WifiDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr);
LOCAL void WifiSuppTaskEntry(uint32 argc, void * argv);
LOCAL void WifiHandleCusMsg(uint32 msg_id, void *msg_body_ptr);

/*LOCAL void WifiDhcpNotice(WIFISUPP_DHCP_NOTICE_IND_T *dhcp_notice_ptr);*/

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//Description : to creat wifi supplicant thread
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void SUPPMAIN_CreatThread(void)
{
    if(0 == s_wifisupp_instance.supp_thread_id)
    {
        s_wifisupp_instance.supp_thread_id = SCI_CreateThread(
                                                "WIFISUPP",
                                                "WIFISUPP_QUEUE",
                                                WifiSuppTaskEntry,
                                                0,0,
                                                WIFISUPP_TASK_STACK_SIZE,
                                                WIFISUPP_TASK_QUEUE_NUM,
                                                WIFISUPP_TASK_PRIORITY,            
                                                SCI_PREEMPT, 
                                                SCI_AUTO_START
                                                );
        //SCI_TRACE_LOW:"WIFISUPP creat thread: %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_104_112_2_18_0_36_25_2328,(uint8*)"d",s_wifisupp_instance.supp_thread_id);
    }
}
/****************************************************************************/
//Description : to send signal to app
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void SUPPMAIN_NotifyApp(
                            uint32      msg_id, 
                            void        *msg_info_ptr
                             )
{
    xSignalHeader   sig_ptr = 0;
    uint16          sig_size = 0;
    BLOCK_ID        sender_thread_id = SCI_IdentifyThread();
    BLOCK_ID        send_to_thread_id = SUPPMAIN_GetAppThreadId();

    if(0 == msg_info_ptr)
    {
        //SCI_TRACE_LOW:"WIFISUPP SUPPADAP_SendSig msg_info_ptr==0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_123_112_2_18_0_36_25_2329,(uint8*)"");
        return;
    }
    if(send_to_thread_id==0 || send_to_thread_id==SCI_INVALID_BLOCK_ID)
    {
        //SCI_TRACE_LOW:"WIFISUPP SUPPADAP_SendSig send_to_thread_id==0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_128_112_2_18_0_36_25_2330,(uint8*)"");
        return;
    }
    switch(msg_id)
    {
    case WIFISUPP_SIG_ON_CNF: //msg_info_ptr: WIFISUPP_ON_RESULT_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_ON_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_134_112_2_18_0_36_25_2331,(uint8*)"");

        sig_size = sizeof(WIFISUPP_ON_CNF_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_ON_CNF, sig_size, sender_thread_id);
        ((WIFISUPP_ON_CNF_T*)sig_ptr)->on_result = *((WIFISUPP_ON_RESULT_T*)msg_info_ptr);
        break;
        
    case WIFISUPP_SIG_OFF_CNF: //msg_info_ptr: WIFISUPP_OFF_RESULT_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_OFF_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_142_112_2_18_0_36_25_2332,(uint8*)"");

        sig_size = sizeof(WIFISUPP_OFF_CNF_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_OFF_CNF, sig_size, sender_thread_id);
        ((WIFISUPP_OFF_CNF_T*)sig_ptr)->off_result = *((WIFISUPP_OFF_RESULT_T*)msg_info_ptr);
        break;

    case WIFISUPP_SIG_SCAN_IND: //msg_info_ptr: WIFISUPP_SCAN_AP_INFO_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_SCAN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_150_112_2_18_0_36_25_2333,(uint8*)"");
        
        sig_size = sizeof(WIFISUPP_SCAN_IND_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_SCAN_IND, sig_size, sender_thread_id);
        CFL_MEMCPY(&((WIFISUPP_SCAN_IND_T*)sig_ptr)->ap_info, msg_info_ptr, sizeof(WIFISUPP_SCAN_AP_INFO_T));
        break;

    case WIFISUPP_SIG_SCAN_CNF: //msg_info_ptr: WIFISUPP_RESULT_E
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_SCAN_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_158_112_2_18_0_36_25_2334,(uint8*)"");
    
        sig_size = sizeof(WIFISUPP_SCAN_CNF_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_SCAN_CNF, sig_size, sender_thread_id);
        ((WIFISUPP_SCAN_CNF_T*)sig_ptr)->scan_result = *((WIFISUPP_RESULT_E*)msg_info_ptr);
        break;

    case WIFISUPP_SIG_CONNECT_CNF: //msg_info_ptr: WIFISUPP_CONNECT_RESULT_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_CONNECT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_166_112_2_18_0_36_25_2335,(uint8*)"");

        sig_size = sizeof(WIFISUPP_CONNECT_CNF_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_CONNECT_CNF, sig_size, sender_thread_id);
        ((WIFISUPP_CONNECT_CNF_T*)sig_ptr)->connect_result = *((WIFISUPP_CONNECT_RESULT_T*)msg_info_ptr);
        break;

    case WIFISUPP_SIG_DISCONNECT_CNF: //msg_info_ptr: WIFISUPP_DISCONNECT_RESULT_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_DISCONNECT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_174_112_2_18_0_36_25_2336,(uint8*)"");

        sig_size = sizeof(WIFISUPP_DISCONNECT_CNF_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_DISCONNECT_CNF, sig_size, sender_thread_id);
        ((WIFISUPP_DISCONNECT_CNF_T*)sig_ptr)->disconnect_result = *((WIFISUPP_DISCONNECT_RESULT_T*)msg_info_ptr);
        break;

    case WIFISUPP_SIG_DISCONNECT_IND: //msg_info_ptr: WIFISUPP_DISCONNECT_INFO_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_DISCONNECT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_182_112_2_18_0_36_26_2337,(uint8*)"");

        sig_size = sizeof(WIFISUPP_DISCONNECT_IND_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_DISCONNECT_IND, sig_size, sender_thread_id);
        ((WIFISUPP_DISCONNECT_IND_T*)sig_ptr)->disc_info = *((WIFISUPP_DISCONNECT_INFO_T*)msg_info_ptr);
        break;

    case WIFISUPP_SIG_BSSID_CHANGED_IND: //msg_info_ptr: WIFISUPP_BSSID_CHANGED_INFO_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_BSSID_CHANGED_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_190_112_2_18_0_36_26_2338,(uint8*)"");

        sig_size = sizeof(WIFISUPP_BSSID_CHANGED_IND_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_BSSID_CHANGED_IND, sig_size, sender_thread_id);
        ((WIFISUPP_BSSID_CHANGED_IND_T*)sig_ptr)->bssid_changed_info = 
                                    *((WIFISUPP_BSSID_CHANGED_INFO_T*)msg_info_ptr);
        break;

    case WIFISUPP_SIG_RSSI_CHANGED_IND: //msg_info_ptr: WIFISUPP_RSSI_CHANGED_INFO_T
        //SCI_TRACE_LOW:"SUPPMAIN_NotifyApp WIFISUPP_SIG_RSSI_CHANGED_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_199_112_2_18_0_36_26_2339,(uint8*)"");
        
        sig_size = sizeof(WIFISUPP_RSSI_CHANGED_IND_T);
        SCI_CREATE_SIGNAL(sig_ptr, WIFISUPP_SIG_RSSI_CHANGED_IND, sig_size, sender_thread_id);
        ((WIFISUPP_RSSI_CHANGED_IND_T*)sig_ptr)->rssi_changed_info = 
                                    *((WIFISUPP_RSSI_CHANGED_INFO_T*)msg_info_ptr);
        break;
    default:
        //SCI_TRACE_LOW:"WIFISUPP CreatWifiMsg, invalid msg id!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_207_112_2_18_0_36_26_2340,(uint8*)"");
        break;
    }
    if(sig_ptr != 0)
    {
        SCI_SEND_SIGNAL(sig_ptr, send_to_thread_id);
    }
}

/****************************************************************************/
//Description : to get the wifi supplicant thread ID
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BLOCK_ID SUPPMAIN_GetWifiThreadId(void)
{
    return s_wifisupp_instance.supp_thread_id;
}

/****************************************************************************/
//Description : to get the wifi APP thread ID
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BLOCK_ID SUPPMAIN_GetAppThreadId(void)
{
    return s_wifisupp_instance.app_thread_id;
}

/****************************************************************************/
//Description : the wifi supplicant thread loop
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void WifiSuppTaskEntry(uint32 argc, void *argv)
{
    xSignalHeaderRec        *sig_ptr = 0;
    BLOCK_ID                task_id = SCI_IdentifyThread();

    for(;;)
    {
        sig_ptr = SCI_GetSignal(task_id);
        //SCI_TRACE_LOW:"WifiSuppTaskEntry, id:%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_246_112_2_18_0_36_26_2341,(uint8*)"d",sig_ptr->SignalCode);

        //handle the customized signal
        if(sig_ptr->SignalCode>=WIFISUPP_SIG_CUSTOM_START &&
            sig_ptr->SignalCode<=WIFISUPP_SIG_CUSTOM_END)
        {
            //SCI_TRACE_LOW:"WIFISUPP task entry: Customized signal received,id:%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_252_112_2_18_0_36_26_2342,(uint8*)"d",sig_ptr->SignalCode);
            WifiHandleCusMsg(sig_ptr->SignalCode, (uint8*)sig_ptr+sizeof(xSignalHeaderRec));
        }
        //handle the fixed signal
        else
        {
            switch(sig_ptr->SignalCode)
            {
            case WIFISUPP_SIG_ON_REQ:
                {
                    WIFISUPP_SIG_ON_REQ_T *on_req_ptr = (WIFISUPP_SIG_ON_REQ_T*)sig_ptr;

                    //SCI_TRACE_LOW:"WIFISUPP task entry: WIFISUPP_SIG_ON_REQ,app:%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_264_112_2_18_0_36_26_2343,(uint8*)"d",on_req_ptr->app_thread_id);
                    WifiOn(on_req_ptr->app_thread_id);
                }
                break;
            case WIFISUPP_SIG_OFF_REQ:
                //SCI_TRACE_LOW:"WIFISUPP task entry: WIFISUPP_SIG_OFF_REQ"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_269_112_2_18_0_36_26_2344,(uint8*)"");
                WifiOff();
                break;
            case WIFISUPP_SIG_SCAN_REQ:
                //SCI_TRACE_LOW:"WIFISUPP task entry: WIFISUPP_SIG_SCAN_REQ"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_273_112_2_18_0_36_26_2345,(uint8*)"");
                {
                    WIFISUPP_SCAN_REQ_INFO_T  *scan_info_ptr = 
                        (WIFISUPP_SCAN_REQ_INFO_T *)((uint8*)sig_ptr + sizeof(xSignalHeaderRec));

                    WifiScan(scan_info_ptr);
                }
                break;
            case WIFISUPP_SIG_CONNECT_REQ:
                {
                    WIFISUPP_SSID_CONFIG_T  *ssid_cfg_ptr = 
                        (WIFISUPP_SSID_CONFIG_T*)((uint8*)sig_ptr+sizeof(xSignalHeaderRec));

                    //SCI_TRACE_LOW:"WIFISUPP task entry: WIFISUPP_SIG_CONNECT_REQ"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_286_112_2_18_0_36_26_2346,(uint8*)"");
                    WifiConnect(ssid_cfg_ptr);
                }
                break;
            case WIFISUPP_SIG_DISCONNECT_REQ:
                {
                    WIFISUPP_DISCONNECT_REQ_INFO_T  *disc_info_ptr = 
                        (WIFISUPP_DISCONNECT_REQ_INFO_T*)((uint8*)sig_ptr+sizeof(xSignalHeaderRec));

                    //SCI_TRACE_LOW:"WIFISUPP task entry: WIFISUPP_SIG_DISCONNECT_REQ"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_295_112_2_18_0_36_26_2347,(uint8*)"");
                    WifiDisconnect(disc_info_ptr);
                }
                break;
            default:
                //SCI_TRACE_LOW:"WIFISUPP task entry: unwanted signal received!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_300_112_2_18_0_36_26_2348,(uint8*)"");
                break;
            }
        }
        CFL_FREE(sig_ptr);
    }
}

/****************************************************************************/
//Description : To call the wifi on function
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiOn(BLOCK_ID app_thread_id)
{
    //SCI_TRACE_LOW:"WifiSupp_main.c::WifiOn called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_314_112_2_18_0_36_26_2349,(uint8*)"");
    if(s_suppadap_custom_info_ptr==0 || 
        s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_on_func==0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiOn, custom supplicant info NOT available!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_318_112_2_18_0_36_26_2350,(uint8*)"");
        return FALSE;
    }
    if(0==app_thread_id || SCI_INVALID_BLOCK_ID==app_thread_id)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiOn, app thread is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_323_112_2_18_0_36_26_2351,(uint8*)"");
        return FALSE;
    }
    if(0==s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_on_func)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiOn, wifi_on_func is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_328_112_2_18_0_36_26_2352,(uint8*)"");
        return FALSE;
    }
    s_wifisupp_instance.app_thread_id = app_thread_id;
    return (*s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_on_func)();
}

/****************************************************************************/
//Description : To call the wifi off function
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiOff(void)
{
    if(s_suppadap_custom_info_ptr==0 || 
        s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_off_func==0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiOff, custom supplicant info NOT available!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_344_112_2_18_0_36_26_2353,(uint8*)"");
        return FALSE;
    }
    if(0==s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_off_func)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiOff, wifi_off_func is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_349_112_2_18_0_36_26_2354,(uint8*)"");
        return FALSE;
    }
    return (*s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_off_func)();
}

/****************************************************************************/
//Description : To call the scan function
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiScan(WIFISUPP_SCAN_REQ_INFO_T *scan_req_ptr)
{
    uint8 essid[WIFISUPP_SSID_MAX_LEN + 1] = {0};

    if(PNULL == scan_req_ptr)
    {
        //SCI_TRACE_LOW:"WIFI:info:SUPP:%s invalid input!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_365_112_2_18_0_36_26_2355,(uint8*)"s", __func__);
        return FALSE;
    }

    //SCI_TRACE_LOW:"WIFI:info:SUPP:%s ssid_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_369_112_2_18_0_36_26_2356,(uint8*)"sd", __func__, scan_req_ptr->ssid.ssid_len);
    SCI_MEMCPY(essid, scan_req_ptr->ssid.ssid, WIFISUPP_SSID_MAX_LEN);
    //SCI_TRACE_LOW:"WIFI:info:SUPP:%s ESSID =%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_371_112_2_18_0_36_26_2357,(uint8*)"ss", __func__, essid);

    if(PNULL == s_suppadap_custom_info_ptr)
    {
        //SCI_TRACE_LOW:"WIFI:error:SUPP:%s custom supplicant info NOT available!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_375_112_2_18_0_36_26_2358,(uint8*)"s", __func__);
        return FALSE;
    }
    
    if(PNULL == s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_scan_func)
    {
        //SCI_TRACE_LOW:"WIFI:error:SUPP:%s wifi_scan_func is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_381_112_2_18_0_36_26_2359,(uint8*)"s", __func__);
        return FALSE;
    }
    
    return (*s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_scan_func)(scan_req_ptr);
}

/****************************************************************************/
//Description : To call the wifi connect function
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiConnect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr)
{
    uint8 essid[WIFISUPP_SSID_MAX_LEN + 1] = {0};

    if(conn_req_ptr == 0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiConnect, invalid input!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_398_112_2_18_0_36_26_2360,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMCPY(essid, conn_req_ptr->ssid.ssid, WIFISUPP_SSID_MAX_LEN);
    //SCI_TRACE_LOW:"WifiConnect: ESSID =%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_403_112_2_18_0_36_26_2361,(uint8*)"s", essid);
    //SCI_TRACE_LOW:"WifiConnect: BSSID: %02X:%02X:%02X:%02X:%02X:%02X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_405_112_2_18_0_36_26_2362,(uint8*)"dddddd", conn_req_ptr->bssid_info.bssid[0],conn_req_ptr->bssid_info.bssid[1],conn_req_ptr->bssid_info.bssid[2],conn_req_ptr->bssid_info.bssid[3],conn_req_ptr->bssid_info.bssid[4],conn_req_ptr->bssid_info.bssid[5]);
    //SCI_TRACE_LOW:"WifiConnect encryp type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_406_112_2_18_0_36_26_2363,(uint8*)"d",conn_req_ptr->encryp_protocol);
    //SCI_TRACE_LOW:"WifiConnect network_mode =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_407_112_2_18_0_36_26_2364,(uint8*)"d",conn_req_ptr->network_mode);
    //SCI_TRACE_LOW:"WifiConnect chanel =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_408_112_2_18_0_36_26_2365,(uint8*)"d",conn_req_ptr->channel);
    //SCI_TRACE_LOW:"WifiConnect pairwise_cipher =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_409_112_2_18_0_36_26_2366,(uint8*)"d",conn_req_ptr->pairwise_cipher);
    //SCI_TRACE_LOW:"WifiConnect group_cipher =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_410_112_2_18_0_36_26_2367,(uint8*)"d",conn_req_ptr->group_cipher);
    if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WEP)
    {
        //SCI_TRACE_LOW:"WifiConnect WEP type =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_413_112_2_18_0_36_26_2368,(uint8*)"d",conn_req_ptr->credential.wep_credential.key_type);
        if (conn_req_ptr->credential.wep_credential.key_type == WIFISUPP_WEP_KEY_TYPE_64BITS)
        {
            //SCI_TRACE_LOW:"WifiConnect WEP 64bit key 1 = %x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_417_112_2_18_0_36_26_2369,(uint8*)"ddddd",conn_req_ptr->credential.wep_credential.key.key_64bits_arr[0][0],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[0][1],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[0][2],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[0][3],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[0][4]);

            //SCI_TRACE_LOW:"WifiConnect WEP 64bit key 2 = %x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_420_112_2_18_0_36_26_2370,(uint8*)"ddddd",conn_req_ptr->credential.wep_credential.key.key_64bits_arr[1][0],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[1][1],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[1][2],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[1][3],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[1][4]);
            
            //SCI_TRACE_LOW:"WifiConnect WEP 64bit key 3 = %x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_423_112_2_18_0_36_26_2371,(uint8*)"ddddd",conn_req_ptr->credential.wep_credential.key.key_64bits_arr[2][0],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[2][1],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[2][2],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[2][3],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[2][4]);

            //SCI_TRACE_LOW:"WifiConnect WEP 64bit key 4 = %x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_426_112_2_18_0_36_26_2372,(uint8*)"ddddd",conn_req_ptr->credential.wep_credential.key.key_64bits_arr[3][0],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[3][1],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[3][2],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[3][3],conn_req_ptr->credential.wep_credential.key.key_64bits_arr[3][4]);
        }
        else
        {
            //SCI_TRACE_LOW:"WifiConnect WEP 128bit key 1 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_434_112_2_18_0_36_26_2373,(uint8*)"ddddddddddddd",conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][0],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][1],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][2],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][3],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][4],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][5],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][6],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][7],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][8],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][9],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][10],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][11],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[0][12]);

            //SCI_TRACE_LOW:"WifiConnect WEP 128bit key 2 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_440_112_2_18_0_36_26_2374,(uint8*)"ddddddddddddd",conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][0],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][1],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][2],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][3],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][4],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][5],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][6],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][7],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][8],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][9],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][10],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][11],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[1][12]);

            //SCI_TRACE_LOW:"WifiConnect WEP 128bit key 3 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_446_112_2_18_0_36_26_2375,(uint8*)"ddddddddddddd",conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][0],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][1],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][2],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][3],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][4],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][5],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][6],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][7],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][8],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][9],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][10],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][11],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[2][12]);
        
            //SCI_TRACE_LOW:"WifiConnect WEP 128bit key 4 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_452_112_2_18_0_36_26_2376,(uint8*)"ddddddddddddd",conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][0],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][1],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][2],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][3],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][4],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][5],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][6],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][7],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][8],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][9],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][10],\
            conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][11],conn_req_ptr->credential.wep_credential.key.key_128bits_arr[3][12]);
        }
    }
    else if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA)
    {
        //SCI_TRACE_LOW:"WifiConnect wpa type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_457_112_2_18_0_36_26_2377,(uint8*)"d",conn_req_ptr->credential.wpa_credential.credential_type);
        //SCI_TRACE_LOW:"WifiConnect wpa =%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_458_112_2_18_0_36_26_2378,(uint8*)"s",conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk);
    }
    else if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA2)
    {
        //SCI_TRACE_LOW:"WifiConnect wpa type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_462_112_2_18_0_36_26_2379,(uint8*)"d",conn_req_ptr->credential.wpa_credential.credential_type);
        //SCI_TRACE_LOW:"WifiConnect wpa =%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_463_112_2_18_0_36_26_2380,(uint8*)"s",conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk);
    }
    else if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WAPI)
    {
        //SCI_TRACE_LOW:"WifiConnect WAPI type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_467_112_2_18_0_36_26_2381,(uint8*)"d",conn_req_ptr->credential.wapi_credential.credential_type);
        if (WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK == conn_req_ptr->credential.wapi_credential.credential_type)
        {
            //SCI_TRACE_LOW:"WifiConnect WAPI psk = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_470_112_2_18_0_36_26_2382,(uint8*)"s", conn_req_ptr->credential.wapi_credential.credential_info.wapi_psk_info.psk);
        }
    }
    else
    {
    
    }

    if(s_suppadap_custom_info_ptr==0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiConnect, custom supplicant info NOT available!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_480_112_2_18_0_36_26_2383,(uint8*)"");
        return FALSE;
    }
    if(0==s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_connect_func)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiConnect, wifi_connect_func is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_485_112_2_18_0_36_26_2384,(uint8*)"");
        return FALSE;
    }
    return (*s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_connect_func)(conn_req_ptr);
}

/****************************************************************************/
//Description : To call the wifi disconnect function
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiDisconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr)
{
    if(disconn_req_ptr == 0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiDisconnect, invalid input!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_499_112_2_18_0_36_26_2385,(uint8*)"");
        return FALSE;
    }
    if(s_suppadap_custom_info_ptr==0 || 
        s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_disconnect_func==0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiDisconnect, custom supplicant info NOT available!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_505_112_2_18_0_36_26_2386,(uint8*)"");
        return FALSE;
    }
    if(0==s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_disconnect_func)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiDisconnect, wifi_connect_func is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_510_112_2_18_0_36_26_2387,(uint8*)"");
        return FALSE;
    }
    return (*s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_disconnect_func)(disconn_req_ptr);
}

/****************************************************************************/
//Description : To handle the wifi customized signal
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void WifiHandleCusMsg(uint32 msg_id, void *msg_body_ptr)
{
    if(s_suppadap_custom_info_ptr==0 || 
        s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_handle_cus_msg_func ==0)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiHandleCusMsg, custom supplicant info NOT available!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_525_112_2_18_0_36_26_2388,(uint8*)"");
        return;
    }
    if(0==s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_handle_cus_msg_func)
    {
        //SCI_TRACE_LOW:"WIFISUPP WifiHandleCusMsg, wifi_handle_cus_msg_func is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_530_112_2_18_0_36_26_2389,(uint8*)"");
        return ;
    }
    (*s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_handle_cus_msg_func)(msg_id, msg_body_ptr);
}

/****************************************************************************/
//Description : 
//Global resource dependence : 
//Author: George.Liu
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN WifiCommand(WIFISUPP_COMMAND_INFO_T *command_info_ptr)
{
    if (PNULL == command_info_ptr)
    {
        //SCI_TRACE_LOW:"WIFI:error:SUPP:%s command_info_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_544_112_2_18_0_36_26_2390,(uint8*)"s", __func__);
        return FALSE;
    }

    if (PNULL == s_suppadap_custom_info_ptr)
    {
        //SCI_TRACE_LOW:"WIFI:error:SUPP:%s s_suppadap_custom_info_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_550_112_2_18_0_36_26_2391,(uint8*)"s", __func__);
        return FALSE;
    }

    if(PNULL == s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_command_func)
    {
        //SCI_TRACE_LOW:"WIFI:error:SUPP:%s wifi_command_func is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_MAIN_556_112_2_18_0_36_26_2392,(uint8*)"s", __func__);
        return FALSE;
    }

    return (*s_suppadap_custom_info_ptr->wifi_oper_funcs.wifi_command_func)(command_info_ptr);
}



