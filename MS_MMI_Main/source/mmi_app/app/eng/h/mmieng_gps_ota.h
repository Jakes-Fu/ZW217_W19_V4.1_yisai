#ifndef  _MMIENG_GPS_OTA_H_
#define _MMIENG_GPS_OTA_H_
typedef struct
{
    SIGNAL_VARS
    uint8 *pData;
} APP_GPS_OTA_SENG_DATA_TO_APP_T;

#if ((defined GPS_SUPPORT)&&(defined MMI_ENG_WIFI_SUPPORT))
/*****************************************************************************/
//  Description : Open Gps OTA Test window
//  Parameter :
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIENG_OpenGPSOtaTestWin(void);
#endif
#endif

