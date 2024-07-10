/*************************************************************************
 ** File Name:      mmidm_export.h                                       *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/02                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about dm function     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/02     jian.ma          Create.                              *
*************************************************************************/

#ifndef _MMI_DM_EXPORT_H_    
#define  _MMI_DM_EXPORT_H_        

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisms_app.h"
/**----------------------------------------------------------------------*
 **                         Compiler Flag                                *
 **----------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**----------------------------------------------------------------------*
 **                         MACRO DEFINITION                             *
 **----------------------------------------------------------------------*/


/**----------------------------------------------------------------------*
 **                         TYPE AND STRUCT                              *
 **----------------------------------------------------------------------*/
typedef enum 
{
     MMIDM_PDP_STATUS_NONE,  //未连接     
     MMIDM_PDP_STATUS_START, //开始连接 
     MMIDM_PDP_STATUS_CONNECT,//已连接
     MMIDM_PDP_STATUS_ABORT //取消连接
}MMIDM_PDP_STATUS_E;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
// 	Description : set the simdm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/ 
PUBLIC void MMIAPIDM_SetMainSim(MN_DUAL_SYS_E sim);

/*****************************************************************************/
// 	Description : get the sim dm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/  
PUBLIC MN_DUAL_SYS_E MMIAPIDM_GetMainSim(void);

/*****************************************************************************/
//  Discription: This fucntion is used to open the dm main menu win  
//  
//  Global resource dependence: None
//  Author: jian.ma 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIDM_OpenMainMenuWin(void);
								
/*****************************************************************************/
//  Discription: This function used to process msg comeback form dm
//  Global resource dependence: none 
//  Author: jian.ma 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIDM_Init(void);

/*****************************************************************************/
// 	Description : intial dm module  
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_InitModule(void);

/*****************************************************************************/
//  Discription: handle sms MT ind
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/	
PUBLIC  BOOLEAN MMIAPIDM_HandleSmsMtInd(
                                       MN_DUAL_SYS_E dual_sys,
                                       uint8   *number_ptr,
                                       uint16   num_len,
                                       APP_SMS_USER_DATA_T	sms_user_data
                                       );

/*****************************************************************************/
// 	Description : handle dm message
//	Global resource dependence : none
//  Author:jian.ma 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_HandleDmMessage(DPARAM     sig_data_ptr);

/*****************************************************************************/
// 	Description : reset dm factory setting
//	Global resource dependence : none
//  Author:jianhui.luo 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_ResetFactorySetting(void);

/*****************************************************************************/
// 	Description : get fumo update timer id
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_IsDmTimerId(uint32 timer_id);

/*****************************************************************************/
// 	Description : handle fumo update timer 
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleDmTimer(uint8 timer_id, uint32 param);


/*****************************************************************************/
//  Description : handle socmo callback
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleScomoCallback(int suiteID,
                                         uint16*version,
                                         uint16 version_len,
                                         uint16* name,
                                         uint16 name_len);

/*****************************************************************************/
// 	Description : 判断当前SIM卡跟IMEI号是否已经注册，若是，那么就不用再次发送自注册短信，若否，
//			则发送如下格式的自注册短信:
//			IMEI:IMEI number/manufacture/mode/sw version
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_SendSelfRegisterSMS(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : get pdp status
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_PDP_STATUS_E MMIAPIDM_GetPdpActiveStatus(void);

/*****************************************************************************/
//  Description :handle dm rx change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandDmRxChange(uint8 rx_level);

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandDmCallClose(void);

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandWapClose(void);

/*****************************************************************************/
//  Description :handle network change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleNetChange(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :hand set lock
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_GetHandsetLock(void);

/*****************************************************************************/
// 	Description : to handle dm idle option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleDmIdleOption(void);
/*****************************************************************************/
// 	Description : to handle dm idle option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDM_SetReg(BOOLEAN enable);

/*****************************************************************************/
// 	Description : get dm version
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC char* MMIAPIDM_GetVersion(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif
