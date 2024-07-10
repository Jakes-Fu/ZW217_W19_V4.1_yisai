#ifdef WRE_SUPPORT
#ifndef     _MMIWRE_DIAL_H_
#define     _MMIWRE_DIAL_H_

typedef enum WRE_WIFI_STATUS
{
    WRE_WIFI_STATUS_OFF,
    WRE_WIFI_STATUS_ON,
    WRE_WIFI_STATUS_CONNECTED,
    WRE_WIFI_STATUS_MAX
}WRE_WIFI_STATUS_E;

typedef enum
{
    WRE_MMIPDP_INTERFACE_GPRS, /*GPRS链路*/
    WRE_MMIPDP_INTERFACE_WIFI, /*WiFi链路*/
    WRE_MMIPDP_INTERFACE_MAX
} WRE_MMIPDP_INTERFACE_E;

typedef enum
{
    WREPDP_APP_MSG_INVALID,
    WREPDP_ACTIVE_CNF,
    WREPDP_DEACTIVE_CNF,
    WREPDP_DEACTIVE_IND,
    WREPDP_ACTIVE_TIMOUT_IND
}WREPDP_APP_MSG_E;

typedef enum
{
    WREPDP_RESULT_SUCC,
    WREPDP_RESULT_FAIL
}WREPDP_RESULT_E;

typedef enum
{
    WRE_DUAL_SYS_1 = 0,
    WRE_DUAL_SYS_2 = 1,
    WRE_DUAL_SYS_3 = 2,
    WRE_DUAL_SYS_4 = 3,
    WRE_DUAL_SYS_MAX
}WRE_DUAL_SYS_E;

#define     WRE_GPRS_DIAL_ACTIVE      1
#define     WRE_GPRS_DIAL_DEACTIVE    2
#define     WRE_WIFI_DIAL_ACTIVE      3
#define     WRE_WIFI_DIAL_DEACTIVE    4


/* 连接方式设定名的最大长度*/
#define WRE_CONNECTION_MAX_LINKMODENAME_LEN  28

/*设置的最大数量10*/
#define WRE_CONNECTION_MAX_SETTING_NUM   10

/*网关地址的最大长度*/
#define WRE_CONNECTION_MAX_GATEWAY_LEN    15

/*apn的最大程度*/
#define WRE_CONNECTION_MAX_APN_LEN        30
typedef struct
{
    uint32              app_handler;
    uint32              nsapi;   /*原来的值是uint8,为了支持wifi，扩展为uint32*/
    int                 msg_id;
    int                 result;
}WREPDP_CNF_INFO_T;


typedef void (*WREPDP_MSG_CALLBACK)(WREPDP_CNF_INFO_T *msg_ptr);

typedef struct
{
    uint32                          app_handler;        //app handler
    uint8 	                         iLinkIndex;          //link index
    WRE_DUAL_SYS_E                  dual_sys;           //dual sim choice
    uint32                          priority;           //the smaller the number, the higher the priority
    WREPDP_MSG_CALLBACK             handle_msg_callback;//callback handle for dealing pdp manager message
    WRE_MMIPDP_INTERFACE_E          ps_interface;
}WREPDP_ACTIVE_INFO_T;

 
WRE_WIFI_STATUS_E WREWIFI_GetStatus(void);
DWORD WREWIFI_GetNetId(void);
void WREWIFI_OpenListWin(void);
BOOLEAN WREWIFI_Deactive(uint32 hdial);
BOOLEAN WREPDP_Active(WREPDP_ACTIVE_INFO_T *app_info_ptr);
BOOLEAN WREPDP_GetDualSimStatus(BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr);
int WRE_WifiConnect(void *ptr);
BOOL Wifi_PostMessage(void *hHandle, uint32 Msg, uint32 wParam, long lParam);
void WRE_MSG_Charge_Handle(uint32 msg);
void WRE_MSG_USB_Handle(uint32 msg);
uint8 WREPDP_GetConnectionLinkSettingNum(WRE_DUAL_SYS_E dual_sys);//add by qingjun.yu 20110.06.02
BOOL WREPDP_GetConnectionLinkSettingDetailByIndex(CONNECT_LINKSETTING_DETAIL *linkDetail, uint8 iLinkIndex, WRE_DUAL_SYS_E dual_sys);//add by qingjun.yu 20110.06.02
int WREPDP_GetDefaultConnectLinkIndex(DIAL_NET_TYPE NetType);
#endif
#endif
