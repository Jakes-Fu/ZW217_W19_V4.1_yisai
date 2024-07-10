#ifndef _CHNL_WIFI_H_
#define _CHNL_WIFI_H_

#include "sci_types.h"
#include "os_api.h"

enum{
    MSG_ID_WIFI_CONNECT,
    MSG_ID_WIFI_DISCONNECT,

    MSG_ID_NONE
};

enum{
    WIFI_STATUS_NONE,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_DISCONNECTING,
    WIFI_STATUS_DISCONNECTED,

    WIFI_STATUS_TOTAL,
};

PUBLIC void chnl_wifi_init(int type);
PUBLIC void chnl_wifi_connect();
PUBLIC void chnl_wifi_disconnect();
PUBLIC int   chnl_wifi_isConnect();
PUBLIC void chnl_wifi_close();
#ifndef FT_WIFI_SUPPORT_CUSTOM_XTC
PUBLIC void chnl_wifi_sock_start();
#endif
#endif
