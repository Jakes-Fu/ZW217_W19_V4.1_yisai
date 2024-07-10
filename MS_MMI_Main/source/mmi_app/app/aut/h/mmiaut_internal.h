/****************************************************************************
** File Name:      mmiaut_internal.h 
** Author:                                                                  
** Date:           2012/01/20
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is mmi nettime api file.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 01/20/2012       dave.ruan            Create
** 
****************************************************************************/
#ifndef _MMIAUT_INTERNAL_H_
#define _MMIAUT_INTERNAL_H_

#ifdef  MMI_UPDATE_TIME_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiset_nv.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

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
#define MMI_DEFAULT_READ_FILE_NAME       { 'E', ':', '\\', 'r', 'x', 'm', '1', '.', 't', 'x', 't', 0 }
#define MMI_DEFAULT_WRITE_FILE_NAME      { 'E', ':', '\\', 'r', 'x', 'm', '2', '.', 't', 'x', 't', 0 }
#define NET_TIME_START_TIPS        "Date:"
#define NET_TIME_END_TIPS          "GMT"
#define NET_CACHE_CONTROL_TIPS          "Cache-Control:"
#define NET_CACHE_CONTROL_TYPE_TIPS          "no-cache"
#define NET_URL_REDIRECT_TIPS          "Moved Temporarily"  //if head returns this tips,ignore its time message
#define NET_URL_NOT_MODIFY_TIPS          "Not Modified"  //if head returns this tips,ignore its time message

#define NET_TIME_DEFAULT_URL        "http://www.google.com"
#define NET_TIME_DEFAULT_HOST       "www.google.com"
#define NET_TIME_DEFAULT_IP         "74.125.71.104"
#define NET_TIME_DEFAULT_PORT       (80)

#define NET_TIME_STRING_LEN        (32) //"Date: Fri, 09 Jul 2010 02:37:17 "
#define NET_TIME_YEAR_LEN          (4)
#define NET_TIME_MON_LEN           (3)
#define NET_TIME_DAY_LEN           (2)
#define NET_TIME_SEC_LEN           (2)
#define NET_CACHE_CONTROL_DISTANCE_LEN          (50)

#define NETTIME_CMP_CURTIME_OFFSET (59) //网络时间和本地时间的容错值，单位s,当时间差超过该值时，则进行网络校时

#define TIME_ZONE_CHINA            (8)
#define TIME_ZONE_MAX              (12.75)
#define TIME_ZONE_MIN              (-12)
#define MMI_DEFAULT_TIME_ZONE      TIME_ZONE_CHINA

#define AUT_LIST_ITEM_SIM_ICON_INDEX 0
#define AUT_LIST_ITEM_SIM_ID_INDEX 1
#define AUT_LIST_ITEM_SIM_SERVICE_NAME_INDEX 2
#define AUT_LIST_ITEM_NETWORK_ACCOUNT_INDEX 3
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMI_PUBWIN_SUCCESS,
    MMI_PUBWIN_FAIL,
    MMI_PUBWIN_WAITING,
    MMI_PUBWIN_QUERY,
    MMI_PUBWIN__MAX
 } MMI_PUBWIN_TYPE_E;

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
// 	Description : 获取系统时间
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMIAUT_GetSysTime(void);

/*****************************************************************************/
// 	Description : 从Socket Buffer中获取网络时间
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_FindNetTimeInNetBuf(char *SocketBuf, uint32 BufLen, uint32 *net_second);

/*****************************************************************************/
// 	Description : 更新网络时间
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAUT_UpdateSysTimeByNetTM(uint32 net_second);

/*****************************************************************************/
// 	Description : 参数复位
//	Global resource dependence : 
//  Author:dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_Reset(void);

/*****************************************************************************/
// 	Description : 自动更新时间相关参数初始化
//	Global resource dependence : 
//  Author:dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_UpdateTimeInit(void);

/*****************************************************************************/
//  Description : get timezone index for SNTP
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   local_timezone is  in seconds
/*****************************************************************************/
PUBLIC uint32 MMIAUT_GetTimezoneIndexForSNTP(uint16 mcc, int32 local_timezone);

/*****************************************************************************/
//  Description : get mcc and timezone value for sntp
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   local_timezone_ptr is  in seconds
//              mcc is from local network service
/*****************************************************************************/
PUBLIC void MMIAUT_GetMccAndTimezone(MN_DUAL_SYS_E dual_sys,uint16* mcc_ptr, int32* local_timezone_ptr);

/*****************************************************************************/
//  Description : active pdp for auto update net time 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAUT_GetSNTPTime(uint8 sntp_server_addr_index);

/*****************************************************************************/
//  Description : active pdp for auto update net time 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_ActivePDPLink(MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item);

/*****************************************************************************/
// 	Description : deactive pdp link
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_DeactivePDPLink(void);

/*****************************************************************************/
// 	Description : 开始手动更新时间的接口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_StartManualUpdateSystemTime(void);

/*****************************************************************************/
//  Description : 打开等待窗口
//  Global resource dependence : 
//  Author:dave.ruan 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUT_OpenManualUpdateWaitWin(void);

/*****************************************************************************/
//  Description : 获取等待窗口的id
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAUT_GetManualUpdateWaitWinID(void);

/*****************************************************************************/
//  Description :   Handle SNTP Cnf Msg,SNTP_GET_TIME_SIG_CNF_T
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 处理SNTP反馈消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_HandleSNTPCnfMsg(MMI_MESSAGE_ID_E msg_id, void *param);

/*****************************************************************************/
// 	Description : Handle Deactive PDP Prompt WinMsg
//	Global resource dependence : 
//    Author: dave.ruan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAUT_ReleaseAllPDPCallback(void);

/*****************************************************************************/
// 	Description : 是否用户取消
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_IsUserCancel(void);

/*****************************************************************************/
// 	Description : 设置用户取消操作标志位
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_SetUserCancelFlag(BOOLEAN user_cancel);

/*****************************************************************************/
// 	Description : get auto update time module id
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAUT_GetModuleID(void);

/*****************************************************************************/
// 	Description : 设置正在手动更新时间标志
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAUT_SetManualUpdateTimeRunning(BOOLEAN is_running);

/*****************************************************************************/
// 	Description : get auto update time Sntp Link Detail Item
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMICONNECTION_SETTING_DETAIL_ITEM_T* MMIAUT_GetSntpLinkDetailItem(void);

/*****************************************************************************/
// 	Description : set auto update time Sntp Link Detail Item
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_SetSntpLinkDetailItem(MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item);

/*****************************************************************************/
// 	Description : Is Need Auto Update Time
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_GetIsNeedAutoUpdateTime(void);

/*****************************************************************************/
// 	Description : Set Is Need Auto Update Time
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_SetIsNeedAutoUpdateTime(BOOLEAN is_need_update);

/*****************************************************************************/
// 	Description : active GPRS
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_ActiveGPRS(MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item, MMIPDP_MSG_CALLBACK pdp_callback);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#endif //MMIAUT_NET_FUN_H


