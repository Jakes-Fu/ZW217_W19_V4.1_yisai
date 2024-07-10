#ifndef _MMIBT_HEADSET_H_ /*perl robot add*/
#define _MMIBT_HEADSET_H_ /*perl robot add*/
/****************************************************************************
** File Name:      mmibt_headset.h                                        *
** Author:         yuantao.xu                                            *
** Date:           8/22/2012                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the paired device list     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/22/2012    yuantao.xu          Create                                  *
**                                                                         *
****************************************************************************/
/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
//  Description : open headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void OpenHeadset(const BT_ADDRESS *src_addr);
/*****************************************************************************/
//  Discription: get active earphone result confirm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_OpenHeadsetCnf(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp);
/*****************************************************************************/
//  Discription: get active earphone result confirm
//  Global resource dependence: None
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_OpenHeadsetConfirmIND(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp);
/*****************************************************************************/
//  Discription: get deactive earphone confirm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CloseHeadsetCnf(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp);
/*****************************************************************************/
//  Discription:check the deviece is connected headset
//  Global resource dependence: none 
//  Author: 
//  Note: if addr == NULL, it will check all paired device to find exist Connected Headset,
//        the connected headset will return in  out_addr
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedHeadset(BT_ADDRESS * in_addr);

/*****************************************************************************/
//  Discription:check exist opened headset or not
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenedHeadsetIsExist(BT_DEVICE_INFO *out_dev);
/*****************************************************************************/
//  Discription:  MMIAPIBT_ Open Bt Headset List
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtHeadsetList(void);
/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void handleHeadsetCloseResult(BT_STATUS bt_status, BT_ADDRESS *headset_addr_ptr);
/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS CloseHeadset(const BT_ADDRESS *src_addr);

//lzk mark
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description :  HandleConnectCnfAlertWin
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleConnectCnfAlertWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );

/*****************************************************************************/
//  Discription:check headset all service is close
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetIsClosed(BT_ADDRESS * in_addr, BT_ADDRESS *out_addr);
#endif

#ifdef MMIBT_HEADSET_AUTHORIZE 
/*****************************************************************************/
//  Discription: Set bluetooth device authorize status   
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetAuthorization(BT_MSG_T* msg_body_ptr);
#endif
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Discription: remove headset
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_RemoveHeadset(BT_MSG_T *msg_body,BOOLEAN is_a2dp);
/*****************************************************************************/
//  Discription:check has headset connected
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedHeadsetEx(uint8 *p_bt_addr);

/*****************************************************************************/
//  Discription:close headset
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/
PUBLIC int32 CloseHeadsetEx(uint8 *p_bt_addr);

#endif


/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetOpenIsPermitEx(uint8 *p_bt_addr);

/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/
PUBLIC void OpenHeadsetEx(uint8 *src_addr);

#endif  /*perl robot add*/

