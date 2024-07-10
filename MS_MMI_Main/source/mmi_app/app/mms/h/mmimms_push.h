/****************************************************************************
** File Name:      mmimms_push.h                                            *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_PUSH_H
#define _MMIMMS_PUSH_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "mmimms_file.h"
#include "mmisms_export.h"
//#include "mMmsGlobal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
/*
 * 接口返回的AP类型中,除了NOT_PUSH_AP和MMIPUSH_MMS_AP,
 * 其余类型的都需要将SMS消息体交给WAP浏览器处理 	
 */
typedef enum
{
	MMIPUSH_NOT_AP = -1,		/* 不是PUSH消息 */
	MMIPUSH_ANY_AP,			/* 任意WAP应用 */
	MMIPUSH_SIA_AP,			/* PUSH SIA  */
	MMIPUSH_WML_AP,			/* WML用户代理 */
	MMIPUSH_WTA_AP,			/* WTA用户代理 */
	MMIPUSH_MMS_AP,			/* MMS用户代理 */
	MMIPUSH_SYNCML_ALERT_AP,	/* SyncML Alert应用 */
	MMIPUSH_LOCATION_AP,		/* Location用户代理 */
	MMIPUSH_SYNCML_DM_AP,		/* SyncML Device Management应用 */
	MMIPUSH_DRM_AP,			/* DRM(Digital Rights Management)用户代理 */
	MMIPUSH_EMN_AP,			/* EMN(Email Notification)用户代理 */
	MMIPUSH_WV_AP,				/* WV(Wireless Village)用户代理 */	
    MMIPUSH_MAX_AP
}MMIPUSH_AP_E;

typedef enum
{
    MMIMMS_PUSH_NEW,    
    MMIMMS_PUSH_NEW_NOT_NOTIFY,     
    MMIMMS_PUSH_NEW_FULLSPACE,    
    MMIMMS_PUSH_NEW_FULLNUM,
    MMIMMS_PUSH_INVALID_SIZE, 
    MMIMMS_PUSH_SFS_ERROR,          //无法保存文件
    MMIMMS_PUSH_EXPIRED     = 128,  //MMS_STATUS_VALUE_EXPIRED	
    MMIMMS_PUSH_RETRIVED    = 129,  //MMS_STATUS_VALUE_RETRIVED
    MMIMMS_PUSH_REJECTED    = 130,  //MMS_STATUS_VALUE_REJECTED
    MMIMMS_PUSH_DEFERRED    = 131,  //MMS_STATUS_VALUE_DEFERRED
    MMIMMS_PUSH_UNRECOGNISED= 132,  //MMS_STATUS_VALUE_UNRECOGNISED
    MMIMMS_PUSH_RECV_MAX
}MMIMMS_PUSH_RECV_E;

typedef struct MMIMMS_MSG_RECEIVED_T_tag
{
    int32		        record_id;
    MMS_PUSH_TYPE_E     type;
    MMIMMS_PUSH_RECV_E  push_status;
    uint8 receiver[MMIMMS_MAX_ADDRESS_LEN+1];
}MMIMMS_MSG_RECEIVED_T;

/*****************************************************************************/
//  Description : 判断是否过期
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsExpire(
                              uint32   date_second,
                              uint32   expiry_hour
                              );

/*****************************************************************************/
//  Description : 查找PUSH消息对应的AP 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIPUSH_AP_E MMIMMS_PushDispatch(uint8 *pushpdu_ptr, uint32 pdu_len);

/*****************************************************************************/
//  Description : 提取指定的unitvar的值 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_PushSMSGetUnitVarValue(uint8* pdu_ptr, uint8* offset_ptr);

/*****************************************************************************/
//  Description : 回调函数，主要用于push消息处理 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC int MMIMMS_HandleMMSPushCallback(int status,void *prompt,int status_value);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_NET_H*/


