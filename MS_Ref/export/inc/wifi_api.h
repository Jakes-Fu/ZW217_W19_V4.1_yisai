/*
* Copyright (c) 2020, Unisoc Communications Inc. All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __WIFI_API_H__
#define __WIFI_API_H__

#include "os_api.h"

#define WIFI_MAC_ADDR_LEN   (6)
#define WIFI_MAX_SSID_LEN 	(32)
#define AT_WIFI_SENTEST_MAX_CNT 10

typedef enum
{
    UAL_WIFI_ENCRYP_PROTOCOL_OPENSYS, 
    UAL_WIFI_ENCRYP_PROTOCOL_WEP, 
    UAL_WIFI_ENCRYP_PROTOCOL_WPA, 
    UAL_WIFI_ENCRYP_PROTOCOL_WPA2,
    UAL_WIFI_ENCRYP_PROTOCOL_WPA_WPA2,
    UAL_WIFI_ENCRYP_PROTOCOL_WPA2_PSK_SHA256,
    UAL_WIFI_ENCRYP_PROTOCOL_WPA3_SAE,
    UAL_WIFI_ENCRYP_PROTOCOL_WPA2_WPA3_SAE,
    UAL_WIFI_ENCRYP_PROTOCOL_WPA3_OWE,
    UAL_WIFI_ENCRYP_PROTOCOL_WPA3_WAPI,
    UAL_WIFI_ENCRYP_PROTOCOL_WPA3_EAP,
    UAL_WIFI_ENCRYP_PROTOCOL_MAX
} ual_wifi_encrypt_protocol_e;

typedef enum
{
    UAL_WIFI_NETWORK_MODE_INFRASTRUCTURE, 
    UAL_WIFI_NETWORK_MODE_ADHOC, 
    UAL_WIFI_NETWORK_MODE_MAX
} ual_wifi_network_mode_e;

typedef enum
{
    /*supp 反馈*/
    WIFI_SUPP_EVENT_OPEN_OK,//开启wifi成功
    WIFI_SUPP_EVENT_OPEN_FAIL,//开启wifi失败
    WIFI_SUPP_EVENT_START_SCAN_OK,//开启扫描成功
    WIFI_SUPP_EVENT_START_SCAN_FAIL,//开启扫描失败
    WIFI_SUPP_EVENT_STOP_SCAN_OK,//关闭扫描成功
    WIFI_SUPP_EVENT_STOP_SCAN_FAIL,///关闭扫描失败
    WIFI_SUPP_EVENT_CLOSE_CNF,//关闭wifi成功
    /*supp上报*/
    WIFI_SUPP_EVENT_SCAN_AP_INFO_IND,//SCAN AP数据
    WIFI_SUPP_EVENT_SCAN_AP_INFO_CNF,//SCAN AP上报结束
    WIFI_SUPP_EVENT_MAX
}wifi_supp_event_e;

enum
{
    WIFI_MAIN_OPEN_CMD = 0,
    WIFI_MAIN_CLOSE_CMD = 1,
    WIFI_MAIN_SCAN_CMD = 2,
    WIFI_MAIN_SCAN_START_RSP = 3,
    WIFI_MAIN_SCAN_RSP = 4,
    WIFI_MAIN_SCAN_STOP_RSP = 5,
    WIFI_MAIN_SENSETIVITY_TEST_ON = 6,
    WIFI_MAIN_SENSETIVITY_TEST = 7,
    WIFI_MAIN_SENSETIVITY_TEST_OFF = 8,
    WIFI_MAIN_SENSETIVITY_TEST_RESULT = 9
};

typedef struct _UAL_WIFI_MSG_Tag
{
    uint32 msg_id;
    uint32 body_size;
    void*  body_ptr;
}UAL_WIFI_MSG_T;

typedef void(* UAL_WIFI_CALLBACK)(const UAL_WIFI_MSG_T *msg);


typedef struct WIFI_SIGNAL_T
{
    SIGNAL_VARS//signal
    void *SigP;//operate 
}WIFI_SIGNAL_T;

typedef struct _C_F_W_WIFISENTEST_PARAM
{
    uint8 mode;       //0: signle mod 1:con mod
    uint8 ch_id;      // channel no
    uint16 loop_time; //unit ms,  one loop duration
    uint16 max_loop;  //max loop cnt
    uint16 resvered;
} C_F_W_WIFISENTEST_PARAM;

typedef struct _C_F_W_WIFISENTEST_RES
{
    uint8 mode;                             //0: signle mod 1:con mod
    uint8 ch_id;                            // channel no
    uint16 loop_cnt;                        // for con mod,  executed loop cnt
    uint16 max_loop;                        // for con mod, max loop cnt
    uint16 result[AT_WIFI_SENTEST_MAX_CNT]; // test result (beacon frame counter), for single mod, the result fixed in u16_result[0]
    uint16 resvered;
} C_F_W_WIFISENTEST_RES;

typedef struct _C_F_W_WIFISCAN_AP_RESULT
{
    uint32 bssid_low;  ///< mac address low
    uint16 bssid_high; ///< mac address high
    uint8 channel;     ///< channel id
    int8 rssival;      ///< signal strength
    uint16 reserved;
    uint16 ssidlen; // ssid len, unit byte
    uint8 ssid[32];
} C_F_W_WIFISCAN_AP_RESULT;

typedef struct _WIFISCAN_AP_RESULT
{
    uint16 NumAP;
    C_F_W_WIFISCAN_AP_RESULT *p_wifiscan_result;   //the proactive command data buf
}WIFISCAN_AP_RESULT;

struct wifi_bss_info {
	char ssid[WIFI_MAX_SSID_LEN + 1];
	unsigned int ssid_len;
	char bssid[WIFI_MAC_ADDR_LEN];
	unsigned int security;
	unsigned char band;
	unsigned char channel;
	signed char rssi;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
};

struct wifi_scan_res {
	struct wifi_bss_info *bsses;
	unsigned int nr_bss;
};

typedef struct _WIFISCAN_REQ
{
    BOOLEAN wswitch;
    uint16 nChNum;
    uint16 nMaxNodeNum;
    uint16 nCh[13];
} WIFISCAN_REQ;

void wifi_sta_init(UAL_WIFI_CALLBACK wifi_callback);
void wifi_sta_test_init(void);
int wifi_sta_open(void);
int wifi_sta_close(void);
int wifi_sta_is_open(void);
int wifi_sta_scan_start(WIFISCAN_REQ scan_req);
int wifi_sta_scan_all_chn_start(void);
int wifi_sta_scan_stop(void);
struct wifi_scan_res *wifi_sta_scan_results(void);
int wifi_sta_scan_is_lte_busy(void);
uint32 wifi_main_get_thread_id(void);
int wifi_sensetivity_test_on(void);
int wifi_sensetivity_test(C_F_W_WIFISENTEST_PARAM param);
int wifi_sensetivity_test_off(void);
int wifi_sensetivity_get_result(void);
C_F_W_WIFISENTEST_RES wifi_sensetivity_read_result(void);
int iwnpi_cmd_handler(int argc, char **argv, char *iwnpi_ret);
int wifi_eut_hdlr(char *diag_cmd, char *at_rsp);
int wifi_sigtest_hdlr(char *at_cmd, char *at_rsp);

#endif /* __WIFI_API_H__ */
