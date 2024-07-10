#if 0
#ifndef RTL_WIFI_API_H
#define RTL_WIFI_API_H

extern BOOLEAN rtl8723_wifi_on(void); 
extern BOOLEAN rtl8723_wifi_off(void); 
extern BOOLEAN rtl8723_wifi_scan(void); 
extern BOOLEAN rtl8723_wifi_connect(PARA_WIFISUPP_SSID_CONFIG_T *connect_info_ptr);
extern BOOLEAN rtl8723_wifi_disconnect(PARA_WIFISUPP_DISCONNECT_REQ_INFO_T *disconnect_info_ptr);
//extern void atheros_handle_cus_msg(uint32 msg_id, void *msg_body_ptr);
extern BOOLEAN rtl8723_get_connection_info(PARA_WIFISUPP_SCAN_AP_INFO_T *connection_info_ptr);

#endif
#endif