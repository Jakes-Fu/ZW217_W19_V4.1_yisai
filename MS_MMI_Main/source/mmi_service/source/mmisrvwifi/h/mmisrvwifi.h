/*****************************************************************************
** File Name:      mmisrvwifi.h                                              *
** Author:         bin.wang1                                                 *
** Date:           2020.07.24                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
** 1. �ӿڰ�ǰ̨��̨��Ϊ���ף�ǰ̨�û��ɼ�����Ҫ�û�ͬ�����£���̨�߼�       *
**    ����������                                                             *
** 2. ��������ϢҲ��Ϊǰ̨�ͺ�̨���ף�Ϊ����ϣ�������Ϣ���ǲ��ŷ�ʽ         *
**    �������û����͡�ǰ̨�ɺ��Ժ�̨��Ϣ����̨�ɺ���ǰ̨��Ϣ                 *
** 3. �����û����ر�ʱWIFI�������ر�                                         *
** 4. ��Ҫ�û��̵߳���SCI_RegisterTaskMsg����SCI_RegisterMsgע��             *
**    SRV_WIFI_SERVICE_CNF_E�¼���ע��ʱCallback�봫���ָ�룬����service��  *
**    ��������Ϣ���ƣ�����Ϊͬ������                                         *
** 5. �û��̻߳���յ�SRV_WIFI_SIG_ON_CNF����Ϣ����Ϣ���ݽṹ��֮ǰ��ͬ      *
** 6. Ŀǰ�����Ƕ��߳�ͬ��connect����                                        *
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
//  Note: ��ʼͳ�ƻ������ݰ�
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_StartCalcWakeupPacket( uint32 time_range,  uint32 wake_times );

/****************************************************************************/
//  Description : MMISRVWIFI_StopCalcWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note: ֹͣͳ�ƻ������ݰ�
/****************************************************************************/
PUBLIC BOOLEAN MMISRVWIFI_StopCalcWakeupPacket( void );

#endif
