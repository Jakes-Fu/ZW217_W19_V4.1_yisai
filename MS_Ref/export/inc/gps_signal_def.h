/******************************************************************************
 ** File Name:      gps_signal_def.h                                          *
 ** Author:         CUIFANG DENG                                              *
 ** Date:           05/31/2017                                               *
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
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/31/2017     cuifang.deng      Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#if !defined(_GPS_SIGNAL_DEF_H)
#define _GPS_SIGNAL_DEF_H

#include "sci_types.h"
#include "os_api.h"
#include "env_gps_signal.h"
//#include "mn_type.h"
//#include "mn_gps_type.h"

#define SAT_NUM 32
#define NMEA_PACKET_DATA_LEN 3072
#define LIBGPS_WIFI_AP_NODE_MAC_ADDRLEN (6)
#define LIBGPS_AGNSS_WIFI_AP_NODE_MAX   (10)



//for gnss bit mask
#define CPMODE_GPS_MASK 0x1  //00001
#define CPMODE_BD2_MASK 0x2  //00010
#define CPMODE_GLO_MASK 0x4  //00100
#define CPMODE_B1C_MASK 0x8  //01000
#define CPMODE_GAL_MASK 0x10 //10000
#define CPMODE_DEF_MASK 0x0  // Need to be configured again
#define CPMODE_ALL_MASK (CPMODE_GPS_MASK | CPMODE_BD2_MASK | CPMODE_GLO_MASK | CPMODE_B1C_MASK | CPMODE_GAL_MASK)




typedef enum
{
    GNSS_MODE_UNKOWN = 0,
    GNSS_MODE_COLD_START,
    GNSS_MODE_WARM_START,
    GNSS_MODE_HOT_START,
} GNSS_StartMode_e;


typedef enum
{
    LIBGPS_NOT_RUN,
    LIBGPS_NOT_FIX,
    LIBGPS_2D_FIX,
    LIBGPS_3D_FIX
} LIBGPS_STATUS_E;

typedef enum
{
    LIBGPS_HOT_MODE,
    LIBGPS_COLD_MODE,
    LIBGPS_WARM_MODE,
    LIBGPS_FACTORY_MODE
} LIBGPS_RESET_MODE_E,
    LIBGPS_START_MODE_E;
	
typedef enum
{
	LIBGPS_SETUP_DATA_SOCKET,
	LIBGPS_RELEASE_DATA_SOCKET
} LIBGPS_DATA_SOCKET_E;


typedef enum
{
    LIBGPS_START_GPS_BD2_GALILEO = (CPMODE_GPS_MASK | CPMODE_BD2_MASK | CPMODE_GAL_MASK), //19
    LIBGPS_START_GPS = CPMODE_GPS_MASK,                                                   //GPS only                        1
    LIBGPS_START_BDS = CPMODE_BD2_MASK,                                                   //BeiDou Only                     2
    LIBGPS_START_GLONASS = CPMODE_GLO_MASK,                                               //GLONASS  only         4
    LIBGPS_START_B1C = CPMODE_B1C_MASK,                                                   //B1C only                            8
    LIBGPS_START_GALILEO = CPMODE_GAL_MASK,                                               //GALILEO  only	     16
    LIBGPS_START_GPS_BDS = (CPMODE_GPS_MASK | CPMODE_BD2_MASK),                           //GPS+BeiDou                3
    LIBGPS_START_GPS_GLONASS = (CPMODE_GPS_MASK | CPMODE_GLO_MASK),                       //GPS+Glonass     5
    LIBGPS_START_GPS_B1C = (CPMODE_GPS_MASK | CPMODE_B1C_MASK),                           //9
    LIBGPS_START_GPS_GALILEO = (CPMODE_GPS_MASK | CPMODE_GAL_MASK),                       //17
    //LIBGPS_START_BD2_GLONASS = (CPMODE_BD2_MASK|CPMODE_GLO_MASK),                    //6
    LIBGPS_START_BD2_GALILEO = (CPMODE_BD2_MASK | CPMODE_GAL_MASK),                                                 //18
    LIBGPS_START_B1C_GALILEO = (CPMODE_B1C_MASK | CPMODE_GAL_MASK),                                                 //24
    LIBGPS_START_GLONASS_GALILEO = (CPMODE_GLO_MASK | CPMODE_GAL_MASK),                                             //20
    LIBGPS_START_GPS_B1C_GLONASS = (CPMODE_GPS_MASK | CPMODE_B1C_MASK | CPMODE_GLO_MASK),                           //13
    LIBGPS_START_GPS_B1C_GALILEO = (CPMODE_GPS_MASK | CPMODE_B1C_MASK | CPMODE_GAL_MASK),                           //25
    LIBGPS_START_GPS_GLONASS_GALILEO = (CPMODE_GPS_MASK | CPMODE_GLO_MASK | CPMODE_GAL_MASK),                       //21
    LIBGPS_START_GPS_B1C_GLONASS_GALILEO = (CPMODE_GPS_MASK | CPMODE_B1C_MASK | CPMODE_GLO_MASK | CPMODE_GAL_MASK), //29
    MN_GPS_GNSS_MODE_MAX
} LIBGPS_GNSS_MODE_E;
	

// following signal only have signal header
typedef xSignalHeaderRec SIGNALS_NO_PARA_T;



typedef struct
{
    GNSS_StartMode_e startMode;
    uint8 useAgnss;
} LIBGPS_STARTMODE_AGNSS_T;

typedef struct
{
    SIGNAL_VARS
    LIBGPS_STARTMODE_AGNSS_T gps_startmode;
} LIBGPS_STARTMODE_AGNSS_IND_T;


typedef struct
{
    SIGNAL_VARS
    uint8 type; //bit0: START_GPS     bit1: START_BDS      bit2: START_GLONASS
    LIBGPS_START_MODE_E start_mode;
} LIBGPS_START_REQ_T;

typedef struct
{
    SIGNAL_VARS
    LIBGPS_RESET_MODE_E reset_mode;
} LIBGPS_RESET_REQ_T;
typedef struct
{
    SIGNAL_VARS
    LIBGPS_DATA_SOCKET_E dataSocket_type;
} LIBGPS_DATA_SOCKET_IND_T;
typedef struct
{
    SIGNAL_VARS
    LIBGPS_GNSS_MODE_E gnss_mode;
} LIBGPS_SET_GNSS_MODE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    LIBGPS_STATUS_E status;
} LIBGPS_STATUS_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint16 type; // value 0- disable ouput;  BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint32 time_interval;
} LIBGPS_OUTPUT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 dual_sys;
    uint32 netid; //value 5~11.   0:invalid
} LIBGPS_PDP_INFO_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 dual_sys;
    unsigned char is_report_location_info;
    unsigned char is_report_satellite_info;
} LIBGPS_REPORT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 dual_sys;
    uint8 type;
} LIBGPS_ASSERT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    unsigned char is_ok;
} LIBGPS_DOWNLOAD_CNF_T,
    LIBGPS_INIT_CNF_T,
    LIBGPS_START_CNF_T,
    LIBGPS_STOP_CNF_T,
    LIBGPS_RESET_CNF_T,
    LIBGPS_OUTPUT_CNF_T,
    LIBGPS_POWEROFF_CNF_T,
    LIBGPS_SET_GNSS_MODE_CNF_T,
    LIBGPS_DEL_AID_DATA_CNF_T,
    LIBGPS_PDP_INFO_CNF_T,
    LIBGPS_RUN_CNF_T,
    LIBGPS_REPORT_CNF_T;

typedef SIGNALS_NO_PARA_T LIBGPS_ERR_IND_T;

typedef struct
{
    SIGNAL_VARS
    LIBGPS_STATUS_E fix_status; //¶¨Î»×´Ì¬
} LIBGPS_READ_STATUS_CNF_T;

typedef struct
{
    uint16 wYear;
    uint16 wMonth;
    uint16 wDay;
    uint16 wHour;
    uint16 wMinute;
    uint16 wSecond;
} LIBGPS_TimeStamp;

typedef struct
{
    float longitude;     // ¾­¶È
    float latitude;      // Î³¶È
    float altitude;      // ¸ß¶È
    LIBGPS_TimeStamp utc_time;  // ÊÀ½çÊ±¼ä
    uint32 ttff_time;    // µÚÒ»´Î¶¨Î»ËùÓÃÊ±¼ä£¬µ¥Î»s
    uint8 satellite_num; // ÎÀÐÇÊýÄ¿
    float speed;         // ¶ÔµØËÙ¶È
    float course;        // ¶ÔµØ·½Ïò
    uint16 clockdrift;
    SCI_TICK_TIME_T m_nSystemTimeStamp; // RTC or any other system time
    uint32 m_nUTCTime;                  // second
    uint8 m_nUncertaintySemiMajor;
    uint8 m_nUncertaintySemiMinor;
    float m_nBearing;            // increments of 1 measured clockwise from the North.
    uint16 m_nDirection;         // UP=0, Down=1
    float m_nHorizontalVelocity; // m/s
    float m_nVerticalVelocity;   // m/s
    uint32 fix_flag;
    float m_nHorizontalAccuracy;
    float m_nPDOP; // Î»ÖÃ¾«¶ÈÒò×Ó
    float m_nHDOP; // Ë®Æ½¾«¶ÈÒò×Ó
    float m_nVDOP; // ´¹Ö±¾«¶ÈÒò×Ó
} LIBGPS_INFO_T;

typedef struct
{
    uint16 m_nSatelliteIdentifier; // SV PRN
    uint16 m_cn0;                  // 2CN0ÐÅÔë±È
    uint16 m_nElevation;           // Ñö½Ç
    uint16 m_nAzimuth;             // ·½Î»½Ç
    uint8 m_IsUsed;
} LIBGPS_SATELLITE_INFO_T;

typedef struct
{
    uint8 satellite_num;
    LIBGPS_SATELLITE_INFO_T satellite_info[SAT_NUM];
} LIBGPS_SATELLITE_INFO_LIST_T;

typedef struct
{
    SIGNAL_VARS
    LIBGPS_INFO_T gps_info;
    LIBGPS_SATELLITE_INFO_LIST_T satellite_list_info;
} LIBGPS_READ_INFO_CNF_T;

typedef struct
{
    SIGNAL_VARS
    LIBGPS_INFO_T gps_info;
} LIBGPS_LOCATION_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    LIBGPS_SATELLITE_INFO_LIST_T satellite_list_info;
} LIBGPS_SATELLITE_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    uint16 length;
    uint16 type;                           // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8 nmea_data[NMEA_PACKET_DATA_LEN]; // NMEA packet buffer
} LIBGPS_OUTPUT_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    float longitude;    //¾­¶È
    float latitude;     //Î³¶È
    LIBGPS_TimeStamp utc_time; // ÊÀ½çÊ±¼ä
    uint32 ttff_time;   //µÚÒ»´Î¶¨Î»ËùÓÃÊ±¼ä£¬µ¥Î»s
    uint16 cn0;
} LIBGPS_FIX_IND_T;

typedef struct
{
	uint8 ap_macaddr[LIBGPS_WIFI_AP_NODE_MAC_ADDRLEN];
	int16 signal_level;
} LIBGPS_APNODE_T;

typedef struct
{
	uint8 ap_num;
	LIBGPS_APNODE_T ap_node[LIBGPS_AGNSS_WIFI_AP_NODE_MAX];
} LIBGPS_APNODE_INFO_T;

typedef struct
{
    SIGNAL_VARS
	LIBGPS_APNODE_INFO_T apnode_info;
} LIBGPS_APNODE_REQ_T;


#endif /* end of _GPS_SIGNAL_DEF_H */
//#endif
