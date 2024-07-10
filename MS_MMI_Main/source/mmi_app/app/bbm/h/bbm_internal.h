/****************************************************************************
** File Name:     bbm_internal.h                                           *
** Author:                                                                 *
** Date:           1/19/2021                                               *
** Copyright:      2021Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    this file is for bbm internal                           *
**                                                                         *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 1/2021        changli.jiang         Create
** 
****************************************************************************/
#ifndef  _BBM_INTERNAL_H_    
#define  _BBM_INTERNAL_H_
#include "sci_types.h"
#include "ual_bt.h"

//internel message
typedef enum
{
    /*������Ϣ��bbm����״̬*/
    MSG_BBM_BLE_CONNECT_SUCCESS,//BLE���ӳɹ�
    MSG_BBM_BLE_CONNECT_FAIL,//BLE����ʧ��
    MSG_BBM_BLE_CONNECT_TIME_OUT,//BLE connect timeout
    MSG_BBM_BLE_INCOMMING_CALL,//BLE֪ͨ���绰
    MSG_BBM_SWITCH_TO_BLE,//bbm slaver and auto mode is need switch ble or bt dialer 
    MSG_BBM_SWITCH_TO_BT_DIALER,//bbm slaver and auto mode is need switch ble or bt dialer
    MSG_BBM_BLE_PASSIVE_DISCONNECTED,// ble �����Ͽ�
    MSG_BBM_BLE_DISCONNECTED_SUCESS_CNF,//CNF���ϲ� �����Ͽ�
    MSG_BBM_BLE_DISCONNECTED_FAIL_CNF,//CNF���ϲ� �����Ͽ�
    MSG_BBM_BLE_CLEAR_BOUND_INFO,//ble���
    MSG_BBM_BT_DIALER_CONNECTED_SUCCESS,//btdial���ӵĳɹ�
    MSG_BBM_BT_DIALER_CONNECTED_FAIL,//bt dialer ����ʧ��
    MSG_BBM_BT_DIALER_DISCONNECT_SUCESS,//bt dial�Ͽ��ɹ�,�����˱����Ͽ�����Ϣ
    MSG_BBM_BT_DIALER_DISCONNECT_FAIL,//bt dialer �Ͽ�ʧ��
    MSG_BBM_BT_PHONE_OPEN_FAIL,//bt phone ��ʧ��
    MSG_BBM_BT_PHONE_OPEN_SUCESS,// bt phone �򿪳ɹ�
    MSG_BBM_BT_PHONE_CLOSE_SUCESS,//bt phone�رճɹ�
    MSG_BBM_BT_PHONE_CLOSE_FAIL,//bt phone �ر�ʧ��
    MSG_BBM_PHONE_SEARCHED,//bbm��Ϊphone��̬�Ѿ������������Զ��ֻ���
    /*���µ���Ϣ��Ϊbbmģʽ֮���л�*/
    MSG_BBM_SWITCH_TO_AUTO_MODE,
    MSG_BBM_SWITCH_TO_SLAVER_MODE,
    MSG_BBM_SWITCH_TO_BTPHONE_MODE,
    MSG_BBM_SWITCH_TO_BTDIALER_MODE,
    MSG_BBM_SWITCH_TO_BLE_MODE,
    MSG_BBM_CLOSE_ALL_MODE,
    MSG_BBM_MAX,
}BBM_MSG_E;
/*ble api for bbm begin*/
/*******************************************************************************************/
//  Description : api of connect ble
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_connect_ble(void);
/*******************************************************************************************/
//  Description : api of disconnect ble
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_disconnect_ble(void);
/*******************************************************************************************/
//  Description : disconnect ble and connet bt dialer
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_ble_to_btdailer(void);
/*******************************************************************************************/
//  Description : disconnect bt dialer and connet ble
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_btdialer_to_ble(void);
/*******************************************************************************************/
//  Description : disconnect ble and connect bt phone
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_ble_to_btphone(void);
/*******************************************************************************************/
//  Description : disconnect btphone and connect ble
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_btphone_to_ble(void);
/*******************************************************************************************/
//  Description : disconnect btphone and bt dialer and connect ble
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_other_device_to_ble(void);
/*******************************************************************************************/
//  Description : disconnect btphone ble and connect bt dialer
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_other_device_to_btdialer(void);
/*******************************************************************************************/
//  Description : disconnect ble and btidaler and connect btphone
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_switch_other_device_to_btphone(void);
/*******************************************************************************************/
//  Description : bbm main process for every mode,it is every mode's entry
//  Param in: BBM_MSG_E:msg_id
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC void BbmMainProcess(BBM_MSG_E msg_id);

PUBLIC void BbmNotifyBbmUser(uint32 msg_id,void *param);

/****************************************************************************/
//  Description : BBM_StartSearchTargetDev
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:����Ŀ���豸����Ϊ������Ŀ���豸���ֻ������Խ�service��Ϊhandfree gategway
/****************************************************************************/
PUBLIC void Bbm_StartSearchBoundDevice(void);

/****************************************************************************/
//  Description : BBM_IsSearchedTargetDev
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:���豸�Ƿ�������
/****************************************************************************/
PUBLIC BOOLEAN Bbm_BoundDeviceIsSearched(void);

/****************************************************************************/
//  Description : Bbm_OpenClassicBtPhone
//  Global resource dependence :
//  Author:changli.jiang
//  Note:open classic BT
/****************************************************************************/
PUBLIC ual_bt_status_e Bbm_OpenClassicBtPhone(void);

/*******************************************************************************************/
//  Description : close all device
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN bbm_close_all_device(void);
#endif
