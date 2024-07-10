#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"

#include "sprd_fts_log.h"
#include "sprd_fts_type.h"

#include "chnl.h"
#include "chnl_wifi.h"
#include "wifisupp_api.h"
#include "dal_power.h"

typedef enum MMIWIFI_STATUS
{
    MMIWIFI_STATUS_OFF,
    MMIWIFI_STATUS_ON,
    MMIWIFI_STATUS_CONNECTED,
    MMIWIFI_STATUS_MAX
} MMIWIFI_STATUS_E;


//engpc thread: cp response to pc tool
#define P_ENGPC_PC_WIFICONN_STACK_SIZE     (1024*4)
#define P_ENGPC_PC_WIFICONN_QUEUE_NUM      15    
#define P_ENGPC_PC_WIFICONN_TASK_PRIORITY  78 //31


LOCAL BLOCK_ID s_engpc_pc_wificonn_task = SCI_INVALID_BLOCK_ID;
LOCAL int s_wifi_connected = WIFI_STATUS_DISCONNECTED;

#define WIFI_CONN_TIMEOUT (5*60)
int wifi_mmi_connect(char* ssid, int len_ssid, char* key, int len_key){
    int timeout = WIFI_CONN_TIMEOUT;

    do{
        if(MMIAPIWIFI_GetStatus() == MMIWIFI_STATUS_CONNECTED){
            break;
        }
        SCI_Sleep(1000);
    }while(timeout-- > 0);

    if(MMIAPIWIFI_GetStatus() == MMIWIFI_STATUS_CONNECTED){
        ENG_LOG("connwifi succ!!!");
        s_wifi_connected = WIFI_STATUS_CONNECTED;
        return 0;
    }else{
        ENG_LOG("connwifi fail!!!");
        s_wifi_connected = WIFI_STATUS_DISCONNECTED;
        return -1;
    }
}

extern int isConnectAp();
extern int loadSsidKey(char* ssid, int ssid_len, char*key, int key_len);
extern int chnl_wifi_hal_autoconnect(char* ssid, char* pwd);
extern int chnl_wifi_hal_disconnect();
extern  int dhcp();
void chnl_sock_init(int type);
#define TEST_AP "qingjun"
#define TEST_PWD "BbatTest"
int wifi_connect(){
    char ssid[64] = {0};
    char key[64] = {0};
    int ret = 0;

    if (loadSsidKey(&ssid, sizeof(ssid), &key, sizeof(key)) != 0){
        ENG_LOG("load ssid && key fail!");
        memcpy(ssid, TEST_AP, strlen(TEST_AP));
        memcpy(key, TEST_PWD, strlen(TEST_PWD));
    }

    ENG_LOG("ssid = %s, key = %s", ssid, key);
    ret = chnl_wifi_hal_autoconnect(ssid, key);
    if (ret != 0){
        s_wifi_connected = WIFI_STATUS_DISCONNECTED;
        ENG_LOG("connect wifi fail: %d", ret);
        return -3;
    }

    s_wifi_connected = WIFI_STATUS_CONNECTED;

    return 0;
}

int wifi_disconnect(){
    chnl_wifi_hal_disconnect();
    s_wifi_connected = WIFI_STATUS_DISCONNECTED;
    return 0;
}

int chnl_wifi_isConnect(){
    return s_wifi_connected;
}

PUBLIC void chnl_wifi_connect()
{
    xSignalHeader   sig_ptr = 0;
    uint32          sig_size = 0;
    BLOCK_ID        sender_thread_id = SCI_IdentifyThread();
    
    sig_size = sizeof(xSignalHeaderRec);
    SCI_CREATE_SIGNAL(sig_ptr, MSG_ID_WIFI_CONNECT, sig_size, sender_thread_id);
    if(0 == sig_ptr)//lint !e774
    {
        ENG_LOG("%s: create signal fail", __FUNCTION__);
        return;
    }

    if(s_engpc_pc_wificonn_task != SCI_INVALID_BLOCK_ID){
        if (s_wifi_connected != WIFI_STATUS_CONNECTING && s_wifi_connected != WIFI_STATUS_CONNECTED){
            SCI_SEND_SIGNAL(sig_ptr, s_engpc_pc_wificonn_task);
            s_wifi_connected = WIFI_STATUS_CONNECTING;
        }
    }
}

PUBLIC void chnl_wifi_disconnect(){
    xSignalHeader   sig_ptr = 0;
    uint32          sig_size = 0;
    BLOCK_ID        sender_thread_id = SCI_IdentifyThread();
    
    sig_size = sizeof(xSignalHeaderRec);
    SCI_CREATE_SIGNAL(sig_ptr, MSG_ID_WIFI_DISCONNECT, sig_size, sender_thread_id);
    if(0 == sig_ptr)//lint !e774
    {
        ENG_LOG("%s: create signal fail", __FUNCTION__);
        return;
    }

    if(s_engpc_pc_wificonn_task != SCI_INVALID_BLOCK_ID){
        if (s_wifi_connected != WIFI_STATUS_DISCONNECTING  && s_wifi_connected != WIFI_STATUS_DISCONNECTED){
            SCI_SEND_SIGNAL(sig_ptr, s_engpc_pc_wificonn_task);
            s_wifi_connected = WIFI_STATUS_DISCONNECTING;
        }
    }
}


extern void chnl_sock_close();
PUBLIC void chnl_wifi_close(){

    s_wifi_connected = WIFI_STATUS_DISCONNECTING;
    wifi_sta_disconnect();
    wifi_sta_close();
    pc_sock_diag_close();
    chnl_sock_close();
    s_wifi_connected = WIFI_STATUS_DISCONNECTED;

}

PUBLIC void Engpc_Chnl_Wifi( uint32 argc, void* argv )
{
    xSignalHeader	sig_ptr	= SCI_NULL;
    BLOCK_ID  task_id = SCI_IdentifyThread();

    for(;;){
            sig_ptr = SCI_GetSignal(task_id);
            SCI_ASSERT(SCI_NULL != sig_ptr);/*assert verified*/

            unsigned short msg_id = sig_ptr->SignalCode;
            int status = chnl_wifi_isConnect();
            ENG_LOG("%s: msg_id = %d, status = %d", __FUNCTION__, msg_id, status);
            if (msg_id == MSG_ID_WIFI_CONNECT){
                if(status == WIFI_STATUS_CONNECTING){
                    int ret = wifi_connect();
                    ENG_LOG("connectWifi return %d", ret);
                }else{
                    ENG_LOG("wifi is already connected");
                }
            }else if(msg_id == MSG_ID_WIFI_DISCONNECT){
                if(status == WIFI_STATUS_DISCONNECTING){
                    wifi_disconnect();
                    ENG_LOG("disconnect");
                }else{
                    ENG_LOG("wifi is already disconnect");
                }
            }else{
                ENG_LOG("%s: unknown sign %d", __FUNCTION__, msg_id);
            }
    }
}

void chnl_daps_start()
{
    int connAp = isConnectAp();
    int mode = POWER_GetResetMode();
    if((connAp == 1 && mode != NORMAL_MODE)){
        extern void DAPS_Start( void );
        DAPS_Start();
    }
}

void chnl_wifi_init(int type){
    int connAp = isConnectAp();
    int mode = POWER_GetResetMode();
#ifdef FT_WIFI_SUPPORT_CUSTOM_XTC
    if(connAp == 1 || type == CHNL_TYPE_DEBUG){
#else
    if((connAp == 1 && mode != NORMAL_MODE) || type == CHNL_TYPE_DEBUG){
#endif
        if( SCI_INVALID_BLOCK_ID == s_engpc_pc_wificonn_task )
        {
            s_engpc_pc_wificonn_task = SCI_CreateThread(
                "ENGPC_PC_WIFICONN_THREAD",
                "ENGPC_PC_WIFICONN_THREAD",
                Engpc_Chnl_Wifi,
                NULL,
                NULL,
                P_ENGPC_PC_WIFICONN_STACK_SIZE,
                P_ENGPC_PC_WIFICONN_QUEUE_NUM,
                P_ENGPC_PC_WIFICONN_TASK_PRIORITY,
                SCI_PREEMPT,
                SCI_AUTO_START );
        }

        SCI_Sleep(1000);
        chnl_wifi_connect();
    }else{
        ENG_LOG("connAp = %d, type = %d, mode = %d", connAp, type, mode);
        return ;
    }
}

#ifndef FT_WIFI_SUPPORT_CUSTOM_XTC
void chnl_wifi_sock_start()
{
    chnl_wifi_init(CHNL_TYPE_DEBUG);
    chnl_sock_init(CHNL_TYPE_DEBUG);
}
#endif
int eng_at_wificonn_func(char *req, char *rsp)
{
    chnl_wifi_init(CHNL_TYPE_DEBUG);
    chnl_sock_init(CHNL_TYPE_DEBUG);
    sprintf(rsp, "+wifi connect ok");
    return strlen(rsp);
}

int eng_at_wifidisconn_func(char *req, char *rsp)
{
    chnl_wifi_close();
    sprintf(rsp, "+wifi disconnect ok");
    return strlen(rsp);
}
int test_eng_diag_wifi_test(char *buf, int len, char *rsp, int rsplen){
    //chnl_wifi_disconnect();
    chnl_wifi_close();
    SCI_Sleep(5*1000);
    chnl_wifi_connect();
}
