/****************************************************************************
** File Name:      mmiaut_export.h 
** Author:                                                                  
** Date:           2010/11/01
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is mmi auto update time api inc file.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 11/01/2010        xiaoming             Create
** 2012/01/20       dave.ruan             modify
****************************************************************************/
#ifndef MMI_AUTO_UPDATE_TIME_API_H
#define MMI_AUTO_UPDATE_TIME_API_H
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicom_time.h"
#include "mmiconnection_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiaut_internal.h"
#include "mmiwclk_export.h"
#include "mmipdp_export.h"

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
#define MMIAUT_MAX_DIRECT_LINK_SETTING_NUM	   8             //设置的直连账户8
#define MMIAUT_11SECOND			11000           //reactive timer 5sec, Qos timer 5sec, attach for timer must be longer than 10sec
#define MMIAUT_5SECOND			5000
#define MMIAUT_1SECOND			1000

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 设置系统时间
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIAUT_SetSysTime(uint32 second);

/*****************************************************************************/
//  Description : 关闭等待窗口
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIAUT_CloseManualUpdateWaitWin(void);

/*****************************************************************************/
// 	Description : 是否正在手动更新时间
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_IsManualUpdateTimeRunning(void);

/*****************************************************************************/
// 	Description : get need update time enable
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_GetNeedUpdateTimeEnable(void);

/*****************************************************************************/
// 	Description : set 是否处理本次的网络时间将其更新到系统时间上
//	Global resource dependence : s_update_time_by_netstatusind
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_SetIsNeedUpdateTimeByNet(BOOLEAN is_need_update_time);

/*****************************************************************************/
// 	Description : get s_update_time_by_netstatusind
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_GetIsNeedUpdateTimeByNet(void);

/*****************************************************************************/
// 	Description : 自动更新时间相关参数初始化
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_AutoUpdateTimeInit(void);

#ifdef MMI_NET_TIME_SUPPORT
/*****************************************************************************/
// 	Description : 从网络信息header中提取时间信息，然后设置到系统
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_GetTimeFromRecvData(const char* header_buf, int32 content_length);
#endif

#ifdef MMI_SNTP_SUPPORT
/*****************************************************************************/
// 	Description : MMIAPIAUT_ManualUpdateTimeHandleSNTPcnf
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:获取MM infor 后的处理，只有自动处理，不会手动更新
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIAUT_ManualUpdateTimeHandleSNTPcnf( MMI_MESSAGE_ID_E msg_id,void * param);

/*****************************************************************************/
// 	Description : MMIAPIAUT  Start Manual Update System Time
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:开始使用SNTP手动更新时间
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIAUT_StartManualUpdateSystemTime( void);
#endif

/*****************************************************************************/
//  Description : get  timezone value by MCC
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   local_timezone_ptr is  in 15 minutes
//              mcc is from local network service
/*****************************************************************************/
PUBLIC void MMIAPIAUT_GetTimezoneByMcc(uint16 mcc, int8* local_timezone_ptr);

#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
/*****************************************************************************/
//  Description : retry pdp dual sys process
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_PdpRetryProcess(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// 	Description : set auto update time active gprs for NITZ
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_UpdateNitzTime(MN_DUAL_SYS_E dual_sys);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //MMI_AUTO_UPDATE_TIME_API_H


