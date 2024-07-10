/*****************************************************************************
** File Name:      mmisrvwifi.h                                              *
** Author:         bin.wang1                                                 *
** Date:           2020.07.24                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
** 1. 接口按前台后台分为两套，前台用户可见，需要用户同步更新，后台逻辑       *
**    看具体需求                                                             *
** 2. 回馈的消息也分为前台和后台两套，为解耦合，所有消息都是播放方式         *
**    向所有用户发送。前台可忽略后台消息，后台可忽略前台消息                 *
** 3. 所有用户都关闭时WIFI才真正关闭                                         *
** 4. 需要用户线程调用SCI_RegisterTaskMsg或者SCI_RegisterMsg注册             *
**    SRV_WIFI_SERVICE_CNF_E事件，注册时Callback请传入空指针，这样service才  *
**    会启动消息机制，否则为同步调用                                         *
** 5. 用户线程会接收到SRV_WIFI_SIG_ON_CNF等消息，消息数据结构与之前相同      *
** 6. 目前不考虑多线程同步connect场景                                        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2020.07.24     bin.wang1            Create                                *
******************************************************************************/

#ifndef  _MMISRVWIFI_H_
#define  _MMISRVWIFI_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"

#include "wifisupp_api.h"


/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/

typedef enum
{
    SRV_WIFI_SERVICE_START = (WIFI_SERVICE << 8),
    SRV_WIFI_SERVICE_ON_CNF,
    SRV_WIFI_SERVICE_OFF_CNF,
    SRV_WIFI_SERVICE_SCAN_IND,
    SRV_WIFI_SERVICE_SCAN_CNF,
    SRV_WIFI_SERVICE_CONNECT_CNF,
    SRV_WIFI_SERVICE_DISCONNECT_IND,
    SRV_WIFI_SERVICE_DISCONNECT_CNF,
    SRV_WIFI_SERVICE_CALC_IND,
    SRV_WIFI_SERVICE_BACKGROUND_ON_CNF,
    SRV_WIFI_SERVICE_BACKGROUND_OFF_CNF,
    SRV_WIFI_SERVICE_BACKGROUND_SCAN_IND,
    SRV_WIFI_SERVICE_BACKGROUND_SCAN_CNF,
    SRV_WIFI_SERVICE_END
}SRV_WIFI_SERVICE_CNF_E;

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : MMISRVWIFI_Init
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMISRVWIFI_Init( void );

/****************************************************************************/
//  Description : MMISRVWIFI_OnReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_OnReq( void );

/****************************************************************************/
//  Description : MMISRVWIFI_BackgroundOnReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_BackgroundOnReq( void );

/****************************************************************************/
//  Description : MMISRVWIFI_OffReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_OffReq( void );

/****************************************************************************/
//  Description : MMISRVWIFI_BackgroundOffReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_BackgroundOffReq( void );

/****************************************************************************/
//  Description : MMISRVWIFI_ScanReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_ScanReq( WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr );

/****************************************************************************/
//  Description : MMISRVWIFI_BackgroundScanReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_BackgroundScanReq( WIFISUPP_SCAN_REQ_INFO_T *scan_info_ptr );

/****************************************************************************/
//  Description : MMISRVWIFI_ConnectReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_ConnectReq( WIFISUPP_SSID_CONFIG_T *connect_info_ptr );

/****************************************************************************/
//  Description : MMISRVWIFI_DisConnectReq
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_DisconnectReq( WIFISUPP_DISCONNECT_REQ_INFO_T* disconnect_info_ptr );

/****************************************************************************/
//  Description : MMISRVWIFI_StartCalcWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note: 开始统计唤醒数据包
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_StartCalcWakeupPacket( uint32 time_range,  uint32 wake_times );

/****************************************************************************/
//  Description : MMISRVWIFI_StopCalcWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note: 停止统计唤醒数据包
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_StopCalcWakeupPacket( void );

#endif
