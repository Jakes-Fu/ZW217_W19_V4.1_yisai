/****************************************************************************
** File Name:      mmibt_inquireddevice.h                                        *
** Author:         Wenming feng                                            *
** Date:           7/20/2012                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the paired device list     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 7/20/2012    yuantao.xu          Create                                  *
**                                                                         *
****************************************************************************/
/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmibt_app.h"
#include "mmifmm_export.h"
#include "mmibt_id.h"
#include "mmibt_nv.h"
#include "bt_abs.h"
#include "mmi_filemgr.h"
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
 typedef struct mmibt_pin_input_param_tag
{
    BT_DEVICE_INFO    pin_request_device_info;
    BTFUNCWINCALLBACK callbackfunc;
}MMIBT_PIN_INPUT_PARAM_T;
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: when paired device successful should modify inquired device info
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void ModifyInquiredDeviceInfo(uint16  delete_device_index);
/*****************************************************************************/
//  Discription: get inquired device name info 
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetInqDevName(
								BT_MSG_T *msg_body_ptr
								);
/*****************************************************************************/
//  Discription: get device pair complete confirm (success or failure)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairComCnf(BT_MSG_T *msg_body_ptr);
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void OpenSearchDeviceWaitWin(
                                   uint32 service_type
                                   );
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void SetSelfPair(BOOLEAN  is_self_pair);
/*****************************************************************************/
//  Discription: get the input pin code request
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_GetInputPinReq(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC  void MMIBT_NumbericPasskeyCheck(void);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC  void MMIBT_CancelNumricPair(void);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC  BT_DEVICE_INFO GetPinRequestInfo(void);
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void SetIsPinCancel(BOOLEAN  is_pin_cancel);
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN IsPinCancel(void);
/*****************************************************************************/
//  Discription: open pin input win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E OpenPinInputWin(BT_DEVICE_INFO *param_ptr,BTFUNCWINCALLBACK call_back);
/*****************************************************************************/
//  Discription: open pin input win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void InputPairedPin(MMI_HANDLE_T win_handle,const wchar *str_ptr,int str_len);

/*****************************************************************************/
//  Discription: get earphone device pair complete confirm (success or failure)
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairComIND(BT_MSG_T *msg_body_ptr);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_StopOpenWaitTimer(void);
