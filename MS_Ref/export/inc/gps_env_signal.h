/******************************************************************************
 ** File Name:      gps_env_signal.h                                           *
 ** Author:         cuifang.deng                                               *
 ** Date:           06/01/2017                                               *
 ** Copyright:      Copyright 2001-2017 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define MN received signals                                                       
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 06/01/2017    cuifang.deng      Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#ifndef _GPS_ENV_SIGNAL_H
#define _GPS_ENV_SIGNAL_H

//#include "env_ps_signal.h"
#include "env_mn_signal.h"

typedef enum
{
    LIBGPS_APP_MN_GPS_DOWNLOAD_REQ = END_MN_SIGNAL+20,
    LIBGPS_APP_MN_GPS_INIT_REQ,
    LIBGPS_APP_MN_GPS_START_REQ,
    LIBGPS_APP_MN_GPS_STOP_REQ,
    LIBGPS_APP_MN_GPS_RESET_REQ,
    LIBGPS_APP_MN_GPS_READ_STATUS_REQ,
    LIBGPS_APP_MN_GPS_READ_INFO_REQ,
    LIBGPS_APP_MN_GPS_OUTPUT_REQ,
    LIBGPS_APP_MN_GPS_POWEROFF_REQ,
    LIBGPS_APP_MN_GPS_PDP_INFO_REQ,
    LIBGPS_APP_MN_GPS_REPORT_REQ,
    LIBGPS_APP_MN_GPS_ASSERT_REQ,
    LIBGPS_APP_MN_GPS_SET_GNSS_MODE_REQ,
    LIBGPS_APP_MN_GPS_DEL_AID_DATA_REQ,
    LIBGPS_DOWNLOAD_REQ,
    LIBGPS_INIT_REQ,//5190
    LIBGPS_START_REQ,//5191
    LIBGPS_STOP_REQ,
    LIBGPS_RESET_REQ,
    LIBGPS_READ_STATUS_REQ,
    LIBGPS_READ_INFO_REQ,
    LIBGPS_OUTPUT_REQ,
    LIBGPS_POWEROFF_REQ,
    LIBGPS_PDP_INFO_REQ, //23
    LIBGPS_REPORT_REQ,
    LIBGPS_ASSERT_REQ,//5200
    LIBGPS_DOWNLOAD_CNF,
    LIBGPS_INIT_CNF,
    LIBGPS_START_CNF,
    LIBGPS_STOP_CNF,
    LIBGPS_RESET_CNF,
    LIBGPS_READ_STATUS_CNF,
    LIBGPS_READ_INFO_CNF,
    LIBGPS_OUTPUT_CNF,
    LIBGPS_OUTPUT_INFO_IND,
    LIBGPS_POWEROFF_CNF,//5210
    LIBGPS_ERR_IND,
    LIBGPS_FIX_IND,
    LIBGPS_REPORT_CNF,
    LIBGPS_LOCATION_INFO_IND,
    LIBGPS_SATELLITE_INFO_IND,
    LIBGPS_SET_GNSS_MODE_REQ,
    LIBGPS_SET_GNSS_MODE_CNF,
    MSENSOR_GPS_SMD_INT,
    LIBGPS_DATA_SOCKET_IND,
    LIBGPS_WATCHDOG_REQ,//5220
    LIBGPS_DEL_AID_DATA_REQ,
    LIBGPS_DEL_AID_DATA_CNF,
    LIBGPS_STARTMODE_AGNSS_IND,
    LIBGPS_PDP_INFO_CNF, //49
    LIBGPS_RUN_REQ, //5225
    LIBGPS_RUN_CNF,
    LIBGPS_APNODE_REQ,
    LIBGPS_END_SIGNAL

} libgps_signal_id_t;

#endif
//#endif
