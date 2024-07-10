#ifndef _DRV_WIFI_
#define _DRV_WIFI_

#include <stdint.h>
#include <stdbool.h>
#include "sci_types.h"
#include "sci_api.h"
#include "sci_log.h"
#include "uni8910_wifi_wcn.h"

#define WIFI_CHANNEL_MAX 13
#define WIFI_SCAN_MIN_INTERVAL (120)
#define WF_AP_MAX 40
#define WIFI_RSSI_CALIB_OFFSET (20)

/**
 * @brief WIFI AP information
 */
typedef struct
{
    uint32 bssid_low;  ///< mac address low
    uint16 bssid_high; ///< mac address high
    uint8 channel;     ///< channel id
    int8 rssival;      ///< signal strength
} wifiApInfo_t;

typedef struct
{
    uint32 max;        ///< set by caller, must room count for store aps
    uint32 found;      ///< set by wifi, actual found ap count
    uint32 maxtimeout; ///< max timeout in milliseconds for each channel
    wifiApInfo_t *aps;   ///< room for aps
} wifiScanRequest_t;

typedef struct
{
    bool scan_all;
    wifiScanRequest_t *req;
    uint32 channel;
    uint32 round;
} scanCtx_t;

typedef struct APP_SIG_tag
{
    SIGNAL_VARS
    scanCtx_t *scanCtx;
}APP_SIG_T;

typedef enum {
    APP_SCAN_MSG = 1,
} APP_MSG_E;

typedef enum {
    IRQ_DISABLED,
    IRQ_ENABLED
} IRQ_STATUS_E;

#endif