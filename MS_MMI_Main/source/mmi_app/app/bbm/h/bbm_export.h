/****************************************************************************
** File Name:     bbm_export.h                                             *
** Author:                                                                 *
** Date:           1/19/2021                                               *
** Copyright:      2021Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
**                 bbm is ble and claasic bt(bt dialer and btphone) manager*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 1/2021        changli.jiang         Create
** 
****************************************************************************/
#ifndef  _BBM_EXPORT_H_
#define  _BBM_EXPORT_H_
#include "sci_types.h"
#include "ual_bt.h"
#include "bbm_internal.h"

#define BLE_USER_NAME_LEN      (32)//注册ble user的名称不能大于32

//it is for msg who use bbm
typedef enum
{
    BBM_MSG_BT_ROLE_SWITCHING,//notify user bt role is switching    
    BBM_MSG_BT_SWITCH_FAIL,//notify user bt role switch error
    BBM_MSG_BT_SWITCH_SUCESS,//notify user bt role switch sucess    
    BBM_MSG_BT_BLE_CLEAR_BOUND_SUCESS,//notify user bt clase sucess
    BBM_MSG_BT_BLE_CONNECT_SUCESS,//notify user bt clase sucess    
    BBM_MSG_BT_REPORT_BLE_NAME,//notify user ble name
    BBM_MSG_BT_OPEN_SUCESS,//notify user bt open sucess
    BBM_MSG_BT_OPEN_FAIL,//notify user bt open fail
    BBM_MSG_BT_CLOSE_SUCESS,//notify user bt clase sucess
    BBM_MSG_BT_CLOSE_FAIL,//notify user bt close fail
    BBM_MSG_BT_CONNECT_DEVICE_SUCESS,
    BBM_MSG_BT_DEVICE_DISCONNECTED,
}BBM_EXPORT_MSG_E;

typedef enum
{
    BBM_RETURN_CODE_START,//bbm对外枚举起始
    BBM_RETURN_SUCESS,//bbm注册成功
    BBM_RETURN_REGISTER_REPEATED,//bbm user重复定义
    BBM_RETURN_PARAM_ERROR,//bbm 参数错误
    BBM_RETURN_MODE_ERROR,//bbm mode error,should not use bt dialer make call
    BBM_RETURN_OHTER_ERROR,//bbm 其他错误
}BBM_RETURN_TYPE_E;

//for bbm there is base state no matter in what mode,below is detail base state
typedef enum
{
    BBM_CURRENT_DETAIL_STATE_BLE,
    BBM_CURRENT_DETAIL_STATE_BTDIALER,
    BBM_CURRENT_DETAIL_STATE_BTPHONE,
    BBM_CURRENT_DETAIL_STATE_IS_SWITCHING,
    BBM_CURRENT_DETAIL_STATE_MAX,
}BBM_CURRENT_DETAIL_STATE;

typedef enum
{
    BBM_MODE_AUTO = 1,
    BBM_MODE_SLAVER,
    BBM_MODE_BLE,
    BBM_MODE_DIALER,
    BBM_MODE_PHONE,
}BBM_MODE_E;

typedef struct
{
    wchar bt_name[UAL_BT_DEV_NAME_MAX_LEN];//对端设备名称
    uint8 ble_addr[MAX_BLE_ADDRESS_LEN];//ble 蓝牙地址
    uint8 classic_addr[UAL_BT_DEV_ADDRESS_SIZE];//classic 蓝牙地址
}BBM_BOUND_REMOTE_DEV_INFO_T;

typedef BOOLEAN (* Bbm_CallBack )(uint32 msg_id,void *param);

/*******************************************************************************************/
//  Description : bbm user should use this api to reg,especially should reg callback,in order
//                to notify user some information
//  Param out:BOOLEAN true:reg sucess,false reg fail
//  Param in: user_name:it is user string
//  Param in: user_callback:in order to notify user to process some msg
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmRegister(const char* user_name,Bbm_CallBack user_callback );
/*******************************************************************************************/
//  Description : bbm user if not want to receive bbm message,should use api deregister
//  Param out:BOOLEAN true:dereg sucess,false dereg fail
//  Param in: user_name:it is user string
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmUnregister(const char* user_name);
/*******************************************************************************************/
//  Description : As btphone and bt dial managuage,should provide current bt role,if is not
//                bt phone,it is dialer role,and should use BbmSwitchToBtDialer mode function
//  Param out:BOOLEAN true:is bt phone,false:it is not bt phone
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN BbmCurrentRoleIsBtPhone(void);
/*******************************************************************************************/
//  Description : use to inform bbm auto mode switch to bt dialer
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBtDialer(void);
/*******************************************************************************************/
//  Description : use to inform bbm auto mode switch to ble
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBle(void);
/*******************************************************************************************/
//  Description : use to inform bbm to switch to bt dialer as one slaver role
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBtDialerMode(void);
/*******************************************************************************************/
//  Description : use to inform bbm to switch to ble as one slaver role
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBleMode(void);
/*******************************************************************************************/
//  Description : use to inform bbm to switch to BtPhone
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToBTPhoneMode(void);
/*******************************************************************************************/
//  Description : use to inform bbm to open auto mode
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToAutoMode(void);
/*******************************************************************************************/
//  Description : use to inform bbm to open slaver mode
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmSwitchToSlaverMode(void);
/*******************************************************************************************/
//  Description : use to get curremt mode
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_CURRENT_DETAIL_STATE BbmGetCurrentDetailState(void);
/*******************************************************************************************/
//  Description : use to abtain current bt is connect to btphone
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN BbmIsConnectWithRemotePhone(void);
/*******************************************************************************************/
//  Description : open bt
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC void BbmOpenBt(void);
/*******************************************************************************************/
//  Description : close bt
//  Param out: void
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC void BbmCloseBt(void);
/*******************************************************************************************/
//  Description : use to init bbm module this init should be after ual init
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmInit(void);

/****************************************************************************/
//  Description : Bbm_GetBTOn
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取蓝牙开关状态，返回值为TRUE,蓝牙打开，返回值为FALSE,蓝牙关闭
/****************************************************************************/
PUBLIC BOOLEAN Bbm_GetBTOn(void);

/****************************************************************************/
//  Description : Bbm_SetBTOn
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置蓝牙开关状态，TRUE,蓝牙打开，FALSE,蓝牙关闭
/****************************************************************************/
PUBLIC void Bbm_SetBTOn(BOOLEAN is_on);

/****************************************************************************/
//  Description : Bbm_GetBTMode
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取蓝牙模式，共有auto/slaver/ble/dialer/phone 五种模式
/****************************************************************************/
PUBLIC BBM_MODE_E Bbm_GetBTMode(void);
/****************************************************************************/
//  Description : Bbm_SetBTMode
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置蓝牙模式，共有auto/slaver/ble/dialer/phone 五种模式
/****************************************************************************/
PUBLIC void Bbm_SetBTMode(BBM_MODE_E mode);
/****************************************************************************/
//  Description : Bbm_SetBTBoundDevName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:写绑定设备名称
/****************************************************************************/
PUBLIC void Bbm_SetBTBoundDevName(wchar *p_name,uint16 name_len);
/****************************************************************************/
//  Description : Bbm_SetBTBoundDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:写绑定设备地址的信息，调用该接口时，传入bt name和地址(因为远端设备经典和BLE地址不一样，但是名称一样)
/****************************************************************************/
PUBLIC void Bbm_SetBTBoundDevAddr(uint8 *p_addr,BOOLEAN is_classic);
/****************************************************************************/
//  Description : Bbm_GetBTBoundDevInfo
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取绑定设备信息的接口，包括设备名称，设备ble 地址和classic地址
/****************************************************************************/
PUBLIC void Bbm_GetBTBoundDevInfo(BBM_BOUND_REMOTE_DEV_INFO_T *p_dev_info);
/****************************************************************************/
//  Description : Bbm_GetBTBoundDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取绑定设备信息地址,is_classic:TRUE,获取经典蓝牙地址，FALSE,获取BLE蓝牙地址
/****************************************************************************/
PUBLIC void Bbm_GetBTBoundDevAddr(uint8 *p_addr,uint16 addr_len,BOOLEAN is_classic);
/****************************************************************************/
//  Description : Bbm_GetBTBoundDevName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取绑定设备名称
/****************************************************************************/
PUBLIC void Bbm_GetBTBoundDevName(wchar *p_name,uint16 name_len);
/****************************************************************************/
//  Description : Bbm_GetBTLocalName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取BLE local device name,若开启经典蓝牙，ble和经典蓝牙一致，关闭经典蓝牙从nv中获取
/****************************************************************************/
PUBLIC void Bbm_GetBTLocalName(wchar *p_name,uint16 name_len);
/****************************************************************************/
//  Description : Bbm_SetBTLocalName
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置本地蓝牙设备名称
/****************************************************************************/
PUBLIC void Bbm_SetBTLocalName(wchar *p_name);
/****************************************************************************/
//  Description:Bbm_ClearBTBoundDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:清除对端蓝牙地址，is_classic = TRUE为清除经典蓝牙地址，is_classic = FALSE
//  清除对端BLE地址
/****************************************************************************/
PUBLIC void Bbm_ClearBTBoundDevAddr(BOOLEAN is_classic);

/*******************************************************************************************/
//  Description : use to inform bbm auto mode switch to bt dialer
//  Param out: this api is directly result
//  Global resource dependence :
//  Author: changli.jiang
//  Note:
/*********************************************************************************************/
PUBLIC BBM_RETURN_TYPE_E BbmCloseAllMode(void);

#endif
