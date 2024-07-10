/*****************************************************************************
** File Name:      mmiphone_internal.h                                          *
** Author:                                                                   *
** Date:           01/2012                                                   *
** Copyright:      20121 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe phone powner on/off         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012        dave.ruan          Create                                    *
******************************************************************************/

#ifndef _MMIPHONE_INTERNAL_H_
#define _MMIPHONE_INTERNAL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmiphone_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMIPHONE_PIN_PUK_EVENT_INPUT = 0,
    MMIPHONE_PIN_PUK_EVENT_READY,
    MMIPHONE_PIN_PUK_EVENT_NOT_READY,
    MMIPHONE_PIN_PUK_EVENT_MAX
}MMIPHONE_PIN_PUK_EVENT_TYPE_E;

typedef struct
{
    BOOLEAN need_handle_pin[MMI_DUAL_SYS_MAX];		//need to handle pin
    BOOLEAN is_handling_pin[MMI_DUAL_SYS_MAX];		//is handling pin
    MN_DUAL_SYS_E   cur_handling_sim_id;					//current handling sim id
    APP_MN_SIM_GET_PIN_IND_T sim_pin_ind[MMI_DUAL_SYS_MAX];	        //save SIM_PIN_IND
}MMIPHONE_HANDLE_PIN_STATUS_T;                                  //add for dual sim

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : power off MS
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_PowerOff(void);

/*****************************************************************************/
// 	Description : 设置sim card状态
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_SetSimStatus(MN_DUAL_SYS_E dual_sys, SIM_STATUS_E sim_status);

/*****************************************************************************/
// 	Description : Get Pin/PUK Remain string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_GetPinPukRemainStr(MN_DUAL_SYS_E dual_sys,
                                                                                    MMI_STRING_T *pin_puk_str_ptr,
                                                                                    MNSIM_PIN_INDICATION_E pin_ind,
                                                                                    BOOLEAN chv_is_blocked);

/*****************************************************************************/
// 	Description : open query power off 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_OpenQueryPowerOff(MMIPHONE_QUERY_POWER_OFF_TYPE_E query_type);

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : is SIM unlocked?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetIsSIMUnlocked(void);

/*****************************************************************************/
// 	Description : is phone need restart?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set is phone need restart
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys, BOOLEAN is_need_restart);

/*****************************************************************************/
//  Description : mmi phone get curr countdown time to restore in NV
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPHONE_GetCurCountdownTime( void );

/*****************************************************************************/
// 	Description : create input sim lock window
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:  
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_CreateInputSimLockWindow(APP_MN_SIM_NOT_READY_IND_T sim_not_ready_ind);

/*****************************************************************************/
// 	Description : get handle sim lock status ptr for multi sim card
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_HANDLE_SIMLOCK_STATUS_T *MMIPHONE_GetHandleSIMLockStatus(void);

/*****************************************************************************/
// 	Description : MMIPHONE SIM Locked Open Shut Off Win
//	Global resource dependence :                                
//      Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SIMLockedOpenShutOffWin (void);
#endif /*MMI_SIM_LOCK_SUPPORT*/

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_GetAciString(PHONE_SERVICE_STATUS_T *service_status,//IN:
                                         MMI_STRING_T *aci_str_ptr, //OUT
                                         uint16 max_aci_len //IN
                                         );

/*****************************************************************************/
// 	Description : cat opn spn display string
//	Global resource dependence : 
//   Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_CatAciOpnSpnString(MMI_STRING_T *aci_str_ptr,//IN 
                                                MMI_STRING_T *opn_str_ptr,//IN:
                                                MMI_STRING_T *spn_str_ptr,//IN:
                                                MMI_STRING_T *network_str_ptr//OUT
                                                );

/*****************************************************************************/
// 	Description : get the status of battery(if it is charging)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetChargeStatus(void);

/*****************************************************************************/
// 	Description : init nitz info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitNITZ(void);

/*****************************************************************************/
// 	Description : Store Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:only update and store network name, do not Parse name string 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_StoreNetworkNameInfo(const APP_MN_NETWORK_INFO_IND_T *network_info_ind);

/*****************************************************************************/
// 	Description : get NV Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_NV_NETWORK_NAME_T* MMIPHONE_GetNVNetworkNameInfo(void);

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETWORK_INFO_IND
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:store, update network name,and update mm nitz time，full name和short name
//          ,与opn状态相同,在处理opn时存NV
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleNetworkInfoInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitNetworkNameInfo(void);

#ifdef MMI_NETWORK_NAME_SUPPORT
/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_NETWORK_NAME_T* MMIPHONE_GetNetworkNameInfo(void);

/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_PNN_T* MMIPHONE_GetPNNInfo(void);

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETNAME_READY_IND
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready index, get the SPN SPNI ONS .etc netname that can read derectly
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleNetNameReadyInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready flag, get the ONS .etc netname that can read derectly
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleCPHSReadyInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_RECORD_NUM_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready index
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMRecordNumCnf(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_RECORD_READ_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:SIM record read confirm
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMRecordReadCnf(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_BINARY_READ_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:read SPNI confirm
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMBinaryReadCnf(
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description :Get Curr SIM PLMN Network Name Num
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is an asynchronism function
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetPNNum(
                           MN_DUAL_SYS_E    dual_sys
                           );

/*****************************************************************************/
//  Description : Get Network Name String
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:network name string include ACI SPN OPN three part
/*****************************************************************************/
PUBLIC void MMIPHONE_GetNetworkNameString( 
                           MN_DUAL_SYS_E   dual_sys,
                           MMI_STRING_T  *  network_name_ptr
                           );

/*****************************************************************************/
//  Description : set the PNN/PNNI  in to MMI
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_SetPNNWithLac(
                                                MN_DUAL_SYS_E   dual_sys,       //IN:
                                                MN_PLMN_T cur_plmn,                 //IN
                                                uint16  lac         //IN
                                                );
#endif

//=================================VOLTE START===============================//

/*****************************************************************************/
//  Description : deal with signal of APP_MN_MODEM_ACTIVATE_PDP_CONTEXT_IND
//  Parameter: [In] param:the param with the signal
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleActivatePdpCtxInd(DPARAM param);

/*****************************************************************************/
//  Description : deal with signal of APP_MN_IMS_STATE_IND
//  Parameter: [In] param:the param with the signal
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleImsStatusInd(DPARAM param);

/*****************************************************************************/
//  Description : Deregister IMS
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_DeregisterIms(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Set Volte config info according with the config file
//  Parameter: [In] dual_sys: the value of the sim card
//             [In] hplmn : the value of plmn
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetVolteConfigInfo(MN_DUAL_SYS_E dual_sys, MN_PLMN_T hplmn);

/*****************************************************************************/
//  Description : Init volte mode info according with the volte config info
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitVolteInfo(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set Volte status
//  Parameter: [In] dual_sys: the value of the sim card
//             [In] is_volte_ready : TRUE :ready,FALSE :not ready;
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetVolteStatus(MN_DUAL_SYS_E dual_sys , BOOLEAN is_volte_ready);


//===================================VOLTE END===============================//

/*****************************************************************************/
//  Description : set current handling sim info
//  Parameter: [In] None
//             [Out] None
//             [Return] s_handle_pin_status.sim_pin_ind[s_handle_pin_status.cur_handling_sim_id]
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APP_MN_SIM_GET_PIN_IND_T MMIPHONE_GetCurHandlingPinSimInfo(void);

/*****************************************************************************/
//  Description : 启机流程的控制函数
//  Parameter: [In]event: 当前完成的事件
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_PinOrPukProccess( MMIPHONE_PIN_PUK_EVENT_TYPE_E event_id);

#ifdef   __cplusplus
    }
#endif

#endif

