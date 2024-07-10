/*****************************************************************************
** File Name:        mmigps_api.h                                            *
** Author:           xiuyun.wang                                             *
** Date:             02/25/2020                                              *
** Copyright:                                                                *
** Description:    This file is used to define gps api                       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/25/2020           xiuyun.wang           Create                         *
******************************************************************************/
#ifndef _MMIGPS_API_H_
#define _MMIGPS_API_H_
#include "sig_code.h"
#include "mn_gps_type.h"
#include "mmipdp_export.h"

#ifdef WIN32
#define	__FUNCTION__	"function unknown"
#endif

typedef enum
{
 GPS_IDLE_STATE,
 GPS_DOWNLOAD_STATE,
 GPS_INIT_READY_STATE,
 GPS_STARTING_STATE,
 GPS_STARTED_STATE,
 GPS_STOPED_STATE,
}MMIGPS_STATUS_EUM;

typedef enum
{
    MMIGPS_REQUEST_TYPE_SMSGPS_REPORT,
    MMIGPS_REQUEST_TYPE_ENG, 
    MMIGPS_REQUEST_TYPE_MAX,
}MMIGPS_REQUEST_TYPE_E;

typedef enum
{
    MMIGPS_START_TYPE_NONE = 0,
    MMIGPS_START_TYPE_GPS = MNGPS_START_GPS,
    MMIGPS_START_TYPE_BDS = MNGPS_START_BDS,
    MMIGPS_START_TYPE_GPS_BDS = MNGPS_START_GPS_BDS,
    MMIGPS_START_TYPE_GPS_GLONASS = MNGPS_START_GPS_GLONASS,
}MMIGPS_SATRT_TYPE_E;
typedef struct _MMIGPS_START_GPS_PARAM_T_
{
    MMIGPS_REQUEST_TYPE_E request_type;
    MMIGPS_SATRT_TYPE_E   start_type;
    MNGPS_START_MODE_E    start_mode;//if start type is MMIGPS_START_TYPE_NONE, start mode can set to 0
}MMIGPS_START_GPS_PARAM_T;
typedef enum 
{
    MMIGPS_RES_SUCC,
    MMIGPS_RES_STATE_WRONG,
    MMIGPS_RES_PARAM_WRONG,
    MMIGPS_RES_MAX
}MMIGPS_RES_E;
typedef BOOLEAN (* GPS_REQUEST_CALLBACK) (APP_MN_GPS_SIGNAL_E gps_sig_id,void* gps_info);

/*****************************************************************************/
//  Description : download GPS Module Handle Msg For Obtaining Ps Message
//  Parameter : 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_Download(void);
/*****************************************************************************/
//  Description : init gps module
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Init(void);

/*****************************************************************************/
//  Description : power off gps
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_PowerOff(void);
/*****************************************************************************/
//  Description : Start Gps request
//  Parameter In: reuqest_type(0:SMSGPS_REPORT,1:ENG TEST)
//                call_back: register for gps wrapper,and handle gps message 
//            OUT:TRUE:
//                FALSE:
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Start(MMIGPS_START_GPS_PARAM_T* start_param,GPS_REQUEST_CALLBACK call_back);
/*****************************************************************************/
//  Description : request for stop Gps
//  Parameter : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Stop(void);
/*****************************************************************************/
//  Description : get gps current status
//  Parameter : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_STATUS_EUM MMIGPS_CurrentStatus_Get(void);
/*****************************************************************************/
//  Description : set gnss mode for gps
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_GnssMode_Set(MN_GPS_GNSS_MODE_E app_gnss_mode);
/*****************************************************************************/
//  Description : handle data socket
//  Global resource dependence :
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_DataSocketHandle(void *param);
/*****************************************************************************/
//  Description : active pdp with pdp type
//  Global resource dependence :
//  pdp_type: gprs or wifi
//  pdp_callback: callback for setup pdp result
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetupPdp(MMIPDP_INTERFACE_E pdp_type, MMIPDP_MSG_CALLBACK pdp_callback);
/*****************************************************************************/
//  Description : delete aid data for gps test
//  Global resource dependence :
//  [IN]: none
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_AidData_Delete(void);
/*****************************************************************************/
//  Description : release pdp
//  Global resource dependence :
//  [IN]: none
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_ReleasePdp(void);
/*****************************************************************************/
//  Description : registe callback for gps test
//  Global resource dependence :
//  [IN]: call_back: call back funtion for receive msg
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_RegisterCallBack(GPS_REQUEST_CALLBACK call_back);
/*****************************************************************************/
//  Description : open gps
//  Global resource dependence :
//  RETRUN:
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Open(void);
/*****************************************************************************/
//  Description : close gps
//  Global resource dependence :
//  RETRUN:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_Close(void);
/*****************************************************************************/
//  Description : get gps on/off state
//  Global resource dependence :
//  RETRUN:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGPS_OnOffState_Get(void);

#endif
