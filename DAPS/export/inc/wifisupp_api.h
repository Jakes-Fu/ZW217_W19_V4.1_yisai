/*****************************************************************************
** File Name:      wifisupp_api.h                                            *
** Author:         juan.zhang                                                *
** Date:           10/29/2009                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used for wifi supplicant SAP definition and  *
**                declaration                                                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/29/2009     juan.zhang          Create                                 *
******************************************************************************/

#ifndef _WIFISUPP_API_H_
#define _WIFISUPP_API_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#include "cfl_wchar.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define WIFISUPP_BSSID_LEN              6               //the length of BSSID in bytes
#define WIFISUPP_SSID_MAX_LEN           32              //the MAX length of SSID in bytes
#define WIFISUPP_WEP_IV_LEN             3               //the length of IV in WEP
#define WIFISUPP_WEP_64BITS_KEY_LEN     8               //the length of 64bits WEP key
#define WIFISUPP_WEP_128BITS_KEY_LEN    16              //the length of 128bits WEP key
#define WIFISUPP_WPA_PSK_LEN            64              //the length of WPA PSK,最后1位永远为0,作为终止判断标志
#define WIFISUPP_WAPI_PSK_LEN           64              //the length of WAPI PSK,最后1位永远为0,作为终止判断标志
#define WIFISUPP_MAX_FILE_PATH_LEN      SFS_MAX_PATH    //the MAX file path supported
#define WIFISUPP_WEP_KEY_ID_MAX         4               //the max WEP key ID

#define WIFISUPP_MAX_USERNAME_LEN       32
#define WIFISUPP_MAX_PSW_LEN            32

#define WIFISUPP_LOG_ON                 1
#define WIFISUPP_LOG_OFF                0

/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
typedef enum
{
    WIFISUPP_NETWORK_MODE_INFRASTRUCTURE,   //infrastructure
    WIFISUPP_NETWORK_MODE_ADHOC,            //ad-hoc
    WIFISUPP_NETWORK_MODE_MAX
} WIFISUPP_NETWORK_MODE_E;

typedef enum
{
    WIFISUPP_RESULT_SUCC,           //success
    WIFISUPP_RESULT_FAIL,           //fail
    WIFISUPP_RESULT_NOT_FOUND,      //the AP is not found
    WIFISUPP_RESULT_TIMEOUT,        //operation time out
    WIFISUPP_RESULT_RESTRICTED,     //connection is restricted
    WIFISUPP_RESULT_MAX
} WIFISUPP_RESULT_E;

typedef enum
{
    WIFISUPP_ENCRYP_PROTOCOL_OPENSYS,   //open system
    WIFISUPP_ENCRYP_PROTOCOL_WEP,       //WEP
    WIFISUPP_ENCRYP_PROTOCOL_WPA,       //WPA
    WIFISUPP_ENCRYP_PROTOCOL_WPA2,      //WPA2
    WIFISUPP_ENCRYP_PROTOCOL_WAPI,      //WAPI
#ifdef WLAN_EAP_SUPPORT
    WIFISUPP_ENCRYP_PROTOCOL_EAP,      //WAPI
#endif /* WLAN_EAP_SUPPORT */

    WIFISUPP_ENCRYP_PROTOCOL_MAX
}WIFISUPP_ENCRYP_PROTOCOL_E;

/*BEGIN: added by George.Liu 01526, 2010/8/23  MS:00196819*/
typedef enum
{
    WIFISUPP_CIPHER_TKIP,       //TKIP
    WIFISUPP_CIPHER_CCMP,       //CCMP
    WIFISUPP_CIPHER_WEP,
    WIFISUPP_CIPHER_SMS4,       //WAPI SMS-4加密
    WIFISUPP_CIPHER_MAX
} WIFISUPP_CIPHER_E;

/*END:   added by George.Liu 01526, 2010/8/23  MS:00196819*/

typedef enum
{
    WIFISUPP_WEP_KEY_TYPE_64BITS,       //64bits_type WEP Key
    WIFISUPP_WEP_KEY_TYPE_128BITS,      //128bits_type WEP Key
    WIFISUPP_WEP_KEY_TYPE_MAX
}WIFISUPP_WEP_KEY_TYPE_E;

typedef enum
{
    WIFISUPP_WPA_CREDENTIAL_TYPE_PSK,   //WPA using PSK
    WIFISUPP_WPA_CREDENTIAL_TYPE_EAP,   //WPA using EAP
    WIFISUPP_WPA_CREDENTIAL_TYPE_MAX
}WIFISUPP_WPA_CREDENTIAL_TYPE_E;

typedef enum
{
    WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK,      //预共享密钥
    WIFISUPP_WAPI_CREDENTIAL_TYPE_CER,      //WAPI证书模式
//  WIFISUPP_WAPI_CREDENTIAL_TYPE_RADIUS,   //使用Radius服务器进行认证(EAP认证)  
    WIFISUPP_WAPI_CREDENTIAL_TYPE_MAX
} WIFISUPP_WAPI_CREDENTIAL_TYPE_E;

typedef enum
{
    WIFISUPP_CREDENTIAL_TYPE_PSK,      //预共享密钥
    WIFISUPP_CREDENTIAL_TYPE_RADIUS,   //使用Radius服务器进行认证(EAP认证)  
    WIFISUPP_CREDENTIAL_TYPE_CER,      //WAPI证书模式
    WIFISUPP_CREDENTIAL_TYPE_MAX
} WIFISUPP_CREDENTIAL_TYPE_E;

typedef enum
{
    WIFISUPP_WPA_EAP_TYPE_TLS,          //EAP using TLS
    WIFISUPP_WPA_EAP_TYPE_TTLS,         //EAP using TTLS
    WIFISUPP_WPA_EAP_TYPE_PEAP,         //EAP using PEAP
    WIFISUPP_WPA_EAP_TYPE_LEAP,         //EAP using LEAP
    WIFISUPP_WPA_EAP_TYPE_SIM,          //EAP using SIM
    WIFISUPP_WPA_EAP_TYPE_AKA,          //EAP using AKA
    WIFISUPP_WPA_EAP_TYPE_FAST,         //EAP using FAST
    WIFISUPP_WPA_EAP_TYPE_MAX
} WIFISUPP_WPA_EAP_TYPE_E;

typedef enum
{
    WIFISUPP_INNER_CREDENTIAL_TYPE_CHAP,        //CHAP credential
    WIFISUPP_INNER_CREDENTIAL_TYPE_PAP,         //PAP credential
    WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP,      //MSCHAP credential
    WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP_V2,   //MACHAP_V2 credential
    WIFISUPP_INNER_CREDENTIAL_TYPE_GTC,         //GTC credential
    WIFISUPP_INNER_CREDENTIAL_TYPE_MAX
} WIFISUPP_INNER_CREDENTIAL_TYPE_E;

typedef enum
{
    WIFIDRV_COMMAND_POWER_SAVE = 0x0101,
    WIFIDRV_COMMAND_SET_ROAMING_MODE = 0x0102,
} WIFISUPP_WIFIDRV_COMMAND_TYPE_E;

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint8 ssid[WIFISUPP_SSID_MAX_LEN];
    uint8 ssid_len;
} WIFISUPP_SSID_T;  //SSID

typedef struct
{
    uint8 bssid[WIFISUPP_BSSID_LEN];
} WIFISUPP_BSSID_T; //BSSID

typedef struct
{
    WIFISUPP_SSID_T                 ssid;
    WIFISUPP_BSSID_T                bssid_info;
    int32                           signal_qua;         //signal quality: RSSI
    uint32                          noise;              //SNR: signal noise ratio
    uint32                          beacon_interval;    //the BEACON interval
    uint32                          channel;            //the channel using
    WIFISUPP_ENCRYP_PROTOCOL_E      encryp_protocol;    //the protocol used by encryption    
    uint32                          rate;               //the rate
    WIFISUPP_NETWORK_MODE_E         network_mode;       //network mode
    WIFISUPP_CIPHER_E               pairwise_cipher;
    WIFISUPP_CIPHER_E               group_cipher;
    WIFISUPP_CREDENTIAL_TYPE_E      credential_type;    //AKM类型
} WIFISUPP_SCAN_AP_INFO_T;  //the AP info scanned

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_SCAN_AP_INFO_T     ap_info;
} WIFISUPP_SCAN_IND_T;  //scan indication when new ap is scanned

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_RESULT_E           scan_result;
} WIFISUPP_SCAN_CNF_T;  //a scan is finished in a period

typedef struct
{
    uint8   psk[WIFISUPP_WPA_PSK_LEN];
}WIFISUPP_WPA_PSK_T;    //WPA Pre_shared Key

typedef struct WIFISUPP_WAPI_PSK_tag
{
    uint8   psk[WIFISUPP_WAPI_PSK_LEN];
} WIFISUPP_WAPI_PSK_T;   //WAPI Pre_shared Key

typedef struct
{
    BOOLEAN     is_use_file;
    CFLWCHAR    certificate_file_path[WIFISUPP_MAX_FILE_PATH_LEN];  //the tls certificate file path, this MAY NOT available when using buffer
    uint8       *certificate_buf_ptr;   //the tls certificate buffer, this MAY NOT available when using file
    uint32      certificate_buf_len;    //the tls certificate buffer length, this MAY NOT available when using file
} WIFISUPP_WPA_EAP_TLS_T;   //EAP_TLS information

typedef struct
{
    WIFISUPP_INNER_CREDENTIAL_TYPE_E    inner_credential_type;
    char                                username_arr[WIFISUPP_MAX_USERNAME_LEN+1];
    char                                psw_arr[WIFISUPP_MAX_PSW_LEN+1];
} WIFISUPP_WPA_EAP_TTLS_PEAP_T; //EAP_TTLS/EAP_PEAP information

typedef struct
{
    char    username_arr[WIFISUPP_MAX_USERNAME_LEN+1];
    char    psw_arr[WIFISUPP_MAX_PSW_LEN+1];
}WIFISUPP_WPA_EAP_LEAP_T;   //EAP_LEAP information

typedef struct
{
    BOOLEAN     is_use_file;
    CFLWCHAR    pac_file_path[WIFISUPP_MAX_FILE_PATH_LEN];//the FAST PAC file path, this MAY NOT available when using buffer
    uint8       *pac_buf_ptr;   //the FAST PAC buffer, this MAY NOT available when using file
    uint32      pac_buf_len;    //the FAST PAC buffer length, this MAY NOT available when using file
    char        username_arr[WIFISUPP_MAX_USERNAME_LEN+1];
    char        psw_arr[WIFISUPP_MAX_PSW_LEN+1];
}WIFISUPP_WPA_EAP_FAST_T;   //EAP_FAST information

typedef struct
{
    WIFISUPP_SSID_T     ssid;
}WIFISUPP_DISCONNECT_REQ_INFO_T;    //information in the request for disconnecting a connection

typedef struct WIFISUPP_SCAN_REQ_INFO_tag
{
    WIFISUPP_SSID_T     ssid;
} WIFISUPP_SCAN_REQ_INFO_T;

typedef struct
{
    WIFISUPP_WPA_EAP_TYPE_E eap_type;
    union
    {
        WIFISUPP_WPA_EAP_TLS_T          tls_info;
        WIFISUPP_WPA_EAP_TTLS_PEAP_T    ttls_info;
        WIFISUPP_WPA_EAP_TTLS_PEAP_T    peap_info;
        WIFISUPP_WPA_EAP_LEAP_T         leap_info;
        WIFISUPP_WPA_EAP_FAST_T         fast_info;
    }eap_info;
} WIFISUPP_WPA_EAP_T; //WPA EAP information

typedef struct
{
    uint8    *as_certificate_buf_ptr;    //证书颁发者的证书，里面包括证书颁发者的public key等信息
    uint32    as_certificate_buf_len; 
    uint8    *asue_certificate_buf_ptr;  //用户证书
    uint32    asue_certificate_buf_len; 
} WIFISUPP_WAPI_CER_T;

typedef struct
{
    WIFISUPP_WPA_CREDENTIAL_TYPE_E  credential_type;
    union
    {
        WIFISUPP_WPA_PSK_T  wpa_psk_info;
        WIFISUPP_WPA_EAP_T  wpa_eap_info;
    }credential_info;
} WIFISUPP_WPA_CREDENTIAL_T;    //WPA credential information

typedef struct WIFISUPP_WAPI_CREDENTIAL_tag
{
    WIFISUPP_WAPI_CREDENTIAL_TYPE_E  credential_type;
    union
    {
        WIFISUPP_WAPI_PSK_T  wapi_psk_info;    
        WIFISUPP_WAPI_CER_T  wapi_cer_info;
    } credential_info;
} WIFISUPP_WAPI_CREDENTIAL_T;

typedef struct
{
    WIFISUPP_WEP_KEY_TYPE_E     key_type;
    BOOLEAN                     is_open_mode;
    uint32                      key_in_use;
    union
    {   
        uint8   key_64bits_arr[WIFISUPP_WEP_KEY_ID_MAX][WIFISUPP_WEP_64BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN];
        uint8   key_128bits_arr[WIFISUPP_WEP_KEY_ID_MAX][WIFISUPP_WEP_128BITS_KEY_LEN - WIFISUPP_WEP_IV_LEN];
    }key;
} WIFISUPP_WEP_CREDENTIAL_T;    //WEP credential information

typedef struct
{
    WIFISUPP_SSID_T             ssid;
    WIFISUPP_BSSID_T            bssid_info;
    WIFISUPP_NETWORK_MODE_E     network_mode;
    WIFISUPP_ENCRYP_PROTOCOL_E  encryp_protocol;
    uint32                      channel;
    union
    {
        WIFISUPP_WEP_CREDENTIAL_T   wep_credential;
        WIFISUPP_WPA_CREDENTIAL_T   wpa_credential;
        WIFISUPP_WAPI_CREDENTIAL_T  wapi_credential;
    }credential;
    
    WIFISUPP_CIPHER_E           pairwise_cipher;
    WIFISUPP_CIPHER_E           group_cipher;
} WIFISUPP_SSID_CONFIG_T;   //the config of a SSID

typedef struct
{
    WIFISUPP_RESULT_E   result;
    WIFISUPP_SSID_T     ssid;
    WIFISUPP_BSSID_T    bssid_info;
    uint32              channel;
    int32               rssi;
} WIFISUPP_CONNECT_RESULT_T;    //connect result

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_CONNECT_RESULT_T   connect_result;
} WIFISUPP_CONNECT_CNF_T;   //connect confirmation

typedef struct 
{
    WIFISUPP_RESULT_E   result;
    WIFISUPP_SSID_T     ssid;
} WIFISUPP_DISCONNECT_RESULT_T; //disconnect result

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_DISCONNECT_RESULT_T    disconnect_result;
} WIFISUPP_DISCONNECT_CNF_T;    //disconnect confirmation

typedef struct
{
    WIFISUPP_SSID_T     ssid;
} WIFISUPP_DISCONNECT_INFO_T;   //disconnect information

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_DISCONNECT_INFO_T      disc_info;
} WIFISUPP_DISCONNECT_IND_T;    //disconnect indication

typedef struct
{
    WIFISUPP_SSID_T     ssid;
    WIFISUPP_BSSID_T    new_bssid_info;
    uint32              channel;
} WIFISUPP_BSSID_CHANGED_INFO_T;    //new bssid information

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_BSSID_CHANGED_INFO_T   bssid_changed_info;
} WIFISUPP_BSSID_CHANGED_IND_T; //BSSID changed indication

typedef struct
{
    WIFISUPP_SSID_T     ssid;
    WIFISUPP_BSSID_T    bssid_info;
    int32               new_signal_qua;
} WIFISUPP_RSSI_CHANGED_INFO_T; //RSSI information

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_RSSI_CHANGED_INFO_T    rssi_changed_info;
} WIFISUPP_RSSI_CHANGED_IND_T;  //RSSI changed indication

typedef struct
{
    WIFISUPP_RESULT_E   wifi_on_result;
    uint32              net_id;             //the net id got from TCP/IP
    BOOLEAN             is_support_roam;    //wether support roam
    BOOLEAN             is_support_protocol[WIFISUPP_ENCRYP_PROTOCOL_MAX];  //the encrypt protocol supported
    BOOLEAN             is_support_eap[WIFISUPP_WPA_EAP_TYPE_MAX];          //the EAP methods supported
    BOOLEAN             is_support_inner_eap[WIFISUPP_INNER_CREDENTIAL_TYPE_MAX];   //the inner eap methos supported
} WIFISUPP_ON_RESULT_T; // wifi on result

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_ON_RESULT_T    on_result;
} WIFISUPP_ON_CNF_T;    //wifi on confirmation

typedef struct 
{
    WIFISUPP_RESULT_E   wifi_off_result;
} WIFISUPP_OFF_RESULT_T;    //wifi off result

typedef struct
{
    _SIGNAL_VARS
    WIFISUPP_OFF_RESULT_T   off_result;
} WIFISUPP_OFF_CNF_T;   //wifi off confirmation

typedef struct
{
    WIFISUPP_WIFIDRV_COMMAND_TYPE_E  command_id;
    uint32  para1;
    uint32  para2;
    uint32  para3;
} WIFISUPP_COMMAND_INFO_T;

#ifdef ZDT_WIFI_SUPPORT
#ifdef ZDT_PLAT_YX_SUPPORT_CY
#define ZDT_WIFI_DATA_VALID_SUM 30
#elif defined(ZDT_PLAT_YX_SUPPORT_LZ)
#define ZDT_WIFI_DATA_VALID_SUM 5
#else
#define ZDT_WIFI_DATA_VALID_SUM 8
#endif
typedef struct ZDT_WIFI_DATA_t
{
    uint16 idx;
    uint32 channel;
    uint8 ssid_len;
    int32 rssi;
    uint8 security_enabled;
    uint8 bss_type;
    uint8 bssid[WIFISUPP_BSSID_LEN];
    uint8 ssid[32];
} ZDT_WIFI_DATA_T;

extern ZDT_WIFI_DATA_T g_zdt_wifi_data[ZDT_WIFI_DATA_VALID_SUM];
extern ZDT_WIFI_DATA_T g_zdt_wifi_cur_data;

extern uint8  ZDT_WIFI_Get_DataNum(void);

#endif

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
//Description : To Open WiFi
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_OnReq(BLOCK_ID thread_id);

/****************************************************************************/
//Description : To Close WiFi
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_OffReq(void);

/****************************************************************************/
//Description : To scan reachable APs
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_ScanReq(WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr);

/****************************************************************************/
//Description : To connect AP, Suppliant will connect and associate with the
//              AP automatically.
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_ConnectReq(WIFISUPP_SSID_CONFIG_T *connect_info_ptr);

/****************************************************************************/
//Description : To disconnect AP with the indicated SSID
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_DisconnectReq(WIFISUPP_DISCONNECT_REQ_INFO_T *disconnect_info_ptr);

/****************************************************************************/
//Description : To Initiate WiFi
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_Init(void);

/****************************************************************************/
//Description : 获取WiFi芯片的mac地址(同步)
//Global resource dependence : 
//Author: George.Liu
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_GetMac(uint8 mac_addr[6]);

/****************************************************************************/
//Description : LOG开关
//Global resource dependence : 
//Author: George.Liu
//Note: 
/****************************************************************************/
PUBLIC void WIFISUPP_SetLogSwitch(unsigned int is_log);
PUBLIC unsigned int WIFISUPP_GetLogSwitch(void);

/****************************************************************************/
//Description : 与第三方驱动的同步命令接口
//Global resource dependence : 
//Author: George.Liu
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN  WIFISUPP_Command(WIFISUPP_COMMAND_INFO_T *command_info_ptr);

#ifdef   __cplusplus
    }
#endif


#endif 


