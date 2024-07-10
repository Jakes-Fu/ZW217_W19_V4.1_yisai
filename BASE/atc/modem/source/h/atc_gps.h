/******************************************************************************
 ** File Name:      atc_gps.h                                                 *
 ** Author:         cuifang.deng                                                 *
 ** DATE:           07/06/2017                                                *
 ** Copyright:      Copyright 2001-2017 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/06/2017     Xueli.lv         Create.                                   *
 ******************************************************************************/

#ifndef _ATC_GPS_H
#define _ATC_GPS_H

#include "sci_types.h"
#include "os_api.h"
#include "atc.h"

/*******************************************************************************
 **                              Macro declaration                             *
 ******************************************************************************/


/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/
 typedef enum
{
    ATC_SPGPSPWR = 1, 
    ATC_SPGPSFIX,
    ATC_SPGPSSTATUS,
    ATC_SPGPSINF,
    ATC_SPGPSOUT,  
    ATC_SPGPSRST, 
    ATC_SPGPSPDP, 
    ATC_SPGPSTTFF,
    ATC_SPGPSASSERT,
    ATC_QGNSSC,    
    ATC_QGNSSRD,    
    ATC_QGNSSCMD,
    ATC_CGNSPWR,    
    ATC_CGNSAID,    
    ATC_CGNSCMD, 
    ATC_CGNSINF   
} ATC_GPS_CMD_TYPE_E;

 typedef enum
{
    ATC_NMEA_NONE = 0, 
    ATC_NMEA_GGA   = 1, 
    ATC_NMEA_GLL    = 2,
    ATC_NMEA_GSA   = 4,
    ATC_NMEA_GSV   = 8,
    ATC_NMEA_RMC   = 16,  
    ATC_NMEA_VTG   = 32,
    ATC_NMEA_ALL   = 255
} ATC_GPS_NMEA_TYPE_E;

typedef enum
{
    ATC_FIX_TIMEOUT = 0, 
    ATC_FIX_TRACKING   = 1, 
    ATC_FIX_INTERVAL   = 2,
    ATC_FIX_TEST
} ATC_GPS_TIME_TYPE_E;

typedef struct
{
    ATC_GPS_CMD_TYPE_E      gps_cmd_type;
    ATC_GPS_NMEA_TYPE_E     nmea_type;
    uint32 reset_mode;            
    uint16 receive_type;
    uint32 fix_times;
    uint32 test_times;
    uint32 fix_timeout;   
    uint32 tracking_time;
    uint32 interval_time;
    float   ref_longitude;
    float   ref_latitude;
} ATC_GPS_CONFIG_T;

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSPWR command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSPWR );

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSRST command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSRST );


/*****************************************************************************/
//  Description : This function handles the AT+SPGPSSTATUS command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSSTATUS );

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSINF command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSINF );

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSOUT command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSOUT );

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSFIX command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSFIX );

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSPDP command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSPDP );

/*****************************************************************************/
//  Description : This function handles the AT+QGNSSC command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessQGNSSC );

/*****************************************************************************/
//  Description : This function handles the AT+QGNSSRD command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessQGNSSRD );

/*****************************************************************************/
//  Description : This function handles the AT+QGNSSCMD command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessQGNSSCMD );

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSTTFF command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSTTFF );

/*****************************************************************************/
//  Description : This function handles the AT+CGNSPWR command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGNSPWR );

/*****************************************************************************/
//  Description : This function handles the AT+CGNSAID command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGNSAID );

/*****************************************************************************/
//  Description : This function handles the AT+CGNSCMD command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGNSCMD );

/*****************************************************************************/
//  Description : This function handles the AT+CGNSINF command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGNSINF );

/*****************************************************************************/
//  Description : This function handle asynchronous gps after receive
//                asynchronous events
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void ATC_ProcessGpsEvent(ATC_CONFIG_T *atc_config_ptr,xSignalHeaderRec *sig_ptr);

/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void  ATC_ProcessGpsFixTimeout(
    uint32 lparam
);

/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void  ATC_ProcessGpsTrackingTimeout(
    uint32 lparam
);

/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void  ATC_ProcessGpsIntervalTimeout(
    uint32 lparam
);

/*****************************************************************************/
//  Description : This function handles timer 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
LOCAL void ATC_GpsStartTimer(MN_DUAL_SYS_E dual_sys, uint32 time_type, uint32 timer_value);

/*****************************************************************************/
//  Description : This function handles timer 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
LOCAL void ATC_GpsStopTimer(MN_DUAL_SYS_E dual_sys, uint32 time_type);

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSASSERT command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSASSERT );

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSLTEOPEN command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note: open or close the Ephemeris prediction function
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSLTEOPEN );

#endif/*_ATC_GPS_H*/
