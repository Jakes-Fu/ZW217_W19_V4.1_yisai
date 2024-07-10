#include "sci_types.h"
#include "os_api.h"

#include "sprd_fts_log.h"

#include "wifisupp_api.h"
#include "wifi_api.h"
#include "app_tcp_if.h"
#include "chnl_wifi.h"
#include "sci_log.h"

LOCAL int s_netid_wifi = -1;
LOCAL WIFISUPP_SCAN_AP_INFO_T s_ap_info = {0};
LOCAL int s_ap_find = 0;
LOCAL int s_dhcp_res = -1;

#define WIFI_ALL_RETRY (10)
#define WIFI_ON_RETRY  (5)
#define WIFI_SCAN_RETRY  (5)
#define WIFI_CONNECT_RETRY  (5)

int wifi_hal_getNetId();
int chnl_wifi_isConnect();
LOCAL void dhcp_cb(int32 res,        /* DHCP result - 0: OK; else - error */
    const TCPIP_NETIF_IPADDR_T* addr_ptr,   /* ip addresses pointer */
    TCPIP_IPADDR_T              dhcp_addr,  /* DHCP server address pointer */
    TCPIP_NETID_T               netid )    /* net interface ID */
{
    ENG_LOG("%s: res = %d", __FUNCTION__, res);
    s_dhcp_res = res;
}

#define  DHCP_TIMEOUT (30)
PUBLIC int dhcp()
{
    int32 ret = 0;
    int timeout = DHCP_TIMEOUT;
    int netId = 0;

    ENG_LOG("dhcp cancel");
    s_dhcp_res = -1;
    netId = wifi_hal_getNetId();
    ENG_LOG("%s: netId = %x", __FUNCTION__, netId);
    sci_dhcp_cancel(netId);
    ENG_LOG("dhcp start");
    ret = sci_dhcp_request(dhcp_cb, DHCP_TIMEOUT*1000, netId);
    if (ret != 0){
        ENG_LOG("async dhcp fail!!!");
        return -1;
    }

    while(timeout-- >0)    {
        if(s_dhcp_res == 0){
            ENG_LOG("s_dhcp_res == 0, break!");
            ret = 0;
            break;
        }
        ENG_LOG("dhcp wait count: %d", timeout);
        SCI_Sleep(1000);
        ret = -1;
    }

    if (ret == 0){
        ENG_LOG("dhcp ok!!!!!");
    }else{
        ENG_LOG("dhcp fail!!!!!");
        sci_dhcp_cancel(netId);
    }

    return ret;
}

int wifi_hal_getNetId()
{
    return s_netid_wifi;
}

int wifi_hal_on()
{
    int ret;
    WIFISUPP_ON_RESULT_T on_result = {0};
    int retry = WIFI_ON_RETRY;

    do{
        s_ap_find = 0;
        memset(&s_ap_info, 0, sizeof(s_ap_info));

        on_result.wifi_on_result = WIFISUPP_RESULT_SUCC;
        ENG_LOG("Turn on Wi-Fi\n");
        ret = wifi_sta_open();
        if(chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTED || chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTING)
            return -1;
        if (ret) {
              ENG_LOG("Failed to turn on Wi-Fi!\n");
              on_result.wifi_on_result = WIFISUPP_RESULT_FAIL;
             SCI_Sleep(1000);
               continue;
        }
        on_result.net_id = wifi_get_netid_by_name(CONFIG_WIFI_STA_DRV_NAME);
        s_netid_wifi = on_result.net_id;
        if (!on_result.net_id) {
           ENG_LOG("Failed to get net_id!\n");
           on_result.wifi_on_result = WIFISUPP_RESULT_FAIL;
           SCI_Sleep(1000);
           continue;
        }

        break;
    }while(retry-- > 0);

    ENG_LOG("Wi-Fi on! netId = %x\n", s_netid_wifi);
    return ret;
}

int wifi_hal_off()
{
    int ret;
    WIFISUPP_OFF_RESULT_T off_result = {0};

    s_ap_find = 0;
    memset(&s_ap_info, 0, sizeof(s_ap_info));

    off_result.wifi_off_result = WIFISUPP_RESULT_SUCC;
    ENG_LOG("Turn off Wi-Fi\n");
    ret = wifi_sta_close();
    if (ret) {
    	ENG_LOG("Failed to turn off Wi-Fi!\n");
    	off_result.wifi_off_result = WIFISUPP_RESULT_FAIL;
    	return ret;
    }

    ENG_LOG("Wi-Fi off\n");
    return ret;
}

int wifi_hal_scan(char* ssid)
{
    struct wifi_scan_res *res;
    WIFISUPP_RESULT_E scan_result;
    int ret;
    int i;
    int retry = WIFI_SCAN_RETRY;

    s_ap_find = 0;

    do{
        ENG_LOG("Wi-Fi scan\n");
        ret = wifi_sta_scan(NULL);
        if(chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTED || chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTING)
            return WIFISUPP_RESULT_FAIL;
        if (ret) {
            ENG_LOG("Failed to scan!\n");
            ret = WIFISUPP_RESULT_FAIL;

            SCI_Sleep(1000);
            continue;
        }

        res = wifi_sta_scan_results();
        ENG_LOG("Wi-Fi scan done\n");

        for (i = 0; i < res->nr_bss; i++) {
            struct wifi_bss_info *bss = &res->bsses[i];
            WIFISUPP_SCAN_AP_INFO_T ap_info;

            memset(&ap_info, 0, sizeof(ap_info));
            ap_info.ssid.ssid_len = bss->ssid_len;
            memcpy(ap_info.ssid.ssid, bss->ssid, bss->ssid_len);
            memcpy(ap_info.bssid_info.bssid, bss->bssid, WIFI_MAC_ADDR_LEN);

            ap_info.channel = bss->channel;
            ap_info.signal_qua = bss->rssi;

            ap_info.encryp_protocol = convert_security_type(bss->security);
            ap_info.pairwise_cipher = convert_cipher(bss->pairwise_cipher);
            ap_info.group_cipher = convert_cipher(bss->group_cipher);
            ap_info.credential_type = convert_key_mgmt(bss->key_mgmt);
            ENG_LOG("ssid = %s, bssid = %d, encryp_protocol = %d", ap_info.ssid.ssid, ap_info.bssid_info.bssid, ap_info.encryp_protocol);
            if(0 == strcmp(ap_info.ssid.ssid, ssid)){
                s_ap_find = 1;
                memcpy(&s_ap_info, &ap_info, sizeof(WIFISUPP_SCAN_AP_INFO_T));
                break;
            }else{
                //SCI_Sleep(1000);
            }
        }
        if(s_ap_find == 1){
            break;
        }
    }while(retry-- > 0);

    scan_result = WIFISUPP_RESULT_SUCC;

    return ret;
}

int wifi_hal_connect(WIFISUPP_SSID_CONFIG_T *conn_req_ptr)
{
    struct wifi_config conf = { 0 };
    WIFISUPP_CONNECT_RESULT_T connect_result = {0};
    int ret;
    int retry = WIFI_CONNECT_RETRY;

    if (conn_req_ptr->ssid.ssid_len <= WIFI_MAX_SSID_LEN) {
        memcpy(conf.ssid, conn_req_ptr->ssid.ssid, conn_req_ptr->ssid.ssid_len);
    }

    memcpy(conf.bssid, conn_req_ptr->bssid_info.bssid, WIFI_MAC_ADDR_LEN);

    if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_OPENSYS) {
        conf.security = WIFI_SECURITY_OPEN;
    } else if ((conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA)
        || (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA2) 
        ||  (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA_WPA2)) {
        if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA)
            conf.security = WIFI_SECURITY_WPA_PSK;
        if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA2  || conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA_WPA2)
            conf.security = WIFI_SECURITY_WPA2_PSK;
        memcpy(conf.passphrase,conn_req_ptr->credential.wpa_credential.credential_info.wpa_psk_info.psk, WIFI_MAX_PSPHR_LEN);
    } else if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WAPI) {
        conf.security = WIFI_SECURITY_WAPI_PSK;
        memcpy(conf.passphrase,conn_req_ptr->credential.wapi_credential.credential_info.wapi_psk_info.psk, WIFI_MAX_PSPHR_LEN);
    }
    /*else if (conn_req_ptr->encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WEP)
    [>TODO<]*/

    ret = wifi_sta_set_conf(&conf);
    if(chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTED || chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTING)
        return WIFISUPP_RESULT_FAIL;

    if (ret) {
        ENG_LOG("Failed to set config!\n");
        connect_result.result = WIFISUPP_RESULT_FAIL;
        return ret;
    }

    do{
        ENG_LOG("Wi-Fi connect\n");
        ret = wifi_sta_connect();
        if(chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTED || chnl_wifi_isConnect() == WIFI_STATUS_DISCONNECTING)
            return WIFISUPP_RESULT_FAIL;

        if (ret) {
            ENG_LOG("Failed to connect!\n");
            connect_result.result = WIFISUPP_RESULT_FAIL;
            SCI_Sleep(1000);
        } else {
            struct wifi_signal *signal = wifi_sta_get_signal();

            ENG_LOG("Wi-Fi connected\n");
            connect_result.result = WIFISUPP_RESULT_SUCC;
            memcpy(connect_result.ssid.ssid, conn_req_ptr->ssid.ssid, conn_req_ptr->ssid.ssid_len);
            connect_result.ssid.ssid_len = conn_req_ptr->ssid.ssid_len;
            memcpy(connect_result.bssid_info.bssid, conn_req_ptr->bssid_info.bssid, WIFI_MAC_ADDR_LEN);
            if (!signal){
            	ENG_LOG("Failed to get signal!\n");
            }else{
            	connect_result.rssi = signal->signal;
            }
            break;
        }
    }while(retry-- > 0);

    return ret;
}

int wifi_hal_disconnect(WIFISUPP_DISCONNECT_REQ_INFO_T *disconn_req_ptr)
{
    WIFISUPP_DISCONNECT_RESULT_T disc_result = {0};
    int ret;

    s_ap_find = 0;
    memset(&s_ap_info, 0, sizeof(s_ap_info));

    ENG_LOG("Wi-Fi disconnect\n");
    ret = wifi_sta_disconnect();
    if (ret) {
        ENG_LOG("Failed to disconnect!\n");
        disc_result.result = WIFISUPP_RESULT_FAIL;
    } else {
        ENG_LOG("Wi-Fi disconnected\n");
        disc_result.result = WIFISUPP_RESULT_SUCC;
    }

    return ret;
}

int chnl_wifi_hal_autoconnect(char* ssid, char* pwd)
{
    WIFISUPP_SSID_CONFIG_T req = {0};
    int ret = 0;

    ENG_LOG("%s: ssid = %s, pwd = %s", __FUNCTION__, ssid, pwd);

    for(int i = 0; i < WIFI_ALL_RETRY; i++){

        ret = 0;

        ENG_LOG("close wifi first");
        wifi_sta_disconnect();
        wifi_sta_close();

        if(0 != wifi_hal_on()){
            ENG_LOG("wifi_hal_on fail!");
            ret = -1;
            SCI_Sleep(3000);
            continue;
        }

        if(0 != wifi_hal_scan(ssid)){
            ENG_LOG("wifi_hal_scan fail!");
            ret = -2;
            SCI_Sleep(3000);
            continue;
        }

        memcpy(req.bssid_info.bssid, s_ap_info.bssid_info.bssid, WIFI_MAC_ADDR_LEN);
        memcpy(req.ssid.ssid, ssid, strlen(ssid));
        req.ssid.ssid_len = strlen(ssid);
        req.encryp_protocol = s_ap_info.encryp_protocol;
        if ((req.encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA)
        || (req.encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA2) 
        ||  (req.encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WPA_WPA2)) {
            memcpy(req.credential.wpa_credential.credential_info.wpa_psk_info.psk, pwd, strlen(pwd));
        } else if (req.encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WAPI) {
           memcpy(req.credential.wapi_credential.credential_info.wapi_psk_info.psk, pwd, strlen(pwd));
        }

        if(0 != wifi_hal_connect(&req)){
            ENG_LOG("wifi_hal_connect fail!");
            ret = -1;
            SCI_Sleep(3000);
            continue;
        }

        if (0 != dhcp()){
            ENG_LOG("dhcp fail!!!");
            ret = -4;
            SCI_Sleep(3000);
            continue;
        }

        break;
    }

    return ret;
}

int chnl_wifi_hal_disconnect()
{
    int netId = wifi_hal_getNetId();
    ENG_LOG("%s: netId = %x", __FUNCTION__, netId);
    sci_dhcp_cancel(netId);
    wifi_hal_disconnect(NULL);
    wifi_hal_off();
    return 0;
}
