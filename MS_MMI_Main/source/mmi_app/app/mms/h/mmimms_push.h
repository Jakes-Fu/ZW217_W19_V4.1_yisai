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
 * �ӿڷ��ص�AP������,����NOT_PUSH_AP��MMIPUSH_MMS_AP,
 * �������͵Ķ���Ҫ��SMS��Ϣ�彻��WAP��������� 	
 */
typedef enum
{
	MMIPUSH_NOT_AP = -1,		/* ����PUSH��Ϣ */
	MMIPUSH_ANY_AP,			/* ����WAPӦ�� */
	MMIPUSH_SIA_AP,			/* PUSH SIA  */
	MMIPUSH_WML_AP,			/* WML�û����� */
	MMIPUSH_WTA_AP,			/* WTA�û����� */
	MMIPUSH_MMS_AP,			/* MMS�û����� */
	MMIPUSH_SYNCML_ALERT_AP,	/* SyncML AlertӦ�� */
	MMIPUSH_LOCATION_AP,		/* Location�û����� */
	MMIPUSH_SYNCML_DM_AP,		/* SyncML Device ManagementӦ�� */
	MMIPUSH_DRM_AP,			/* DRM(Digital Rights Management)�û����� */
	MMIPUSH_EMN_AP,			/* EMN(Email Notification)�û����� */
	MMIPUSH_WV_AP,				/* WV(Wireless Village)�û����� */	
    MMIPUSH_MAX_AP
}MMIPUSH_AP_E;

typedef enum
{
    MMIMMS_PUSH_NEW,    
    MMIMMS_PUSH_NEW_NOT_NOTIFY,     
    MMIMMS_PUSH_NEW_FULLSPACE,    
    MMIMMS_PUSH_NEW_FULLNUM,
    MMIMMS_PUSH_INVALID_SIZE, 
    MMIMMS_PUSH_SFS_ERROR,          //�޷������ļ�
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
//  Description : �ж��Ƿ����
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsExpire(
                              uint32   date_second,
                              uint32   expiry_hour
                              );

/*****************************************************************************/
//  Description : ����PUSH��Ϣ��Ӧ��AP 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIPUSH_AP_E MMIMMS_PushDispatch(uint8 *pushpdu_ptr, uint32 pdu_len);

/*****************************************************************************/
//  Description : ��ȡָ����unitvar��ֵ 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_PushSMSGetUnitVarValue(uint8* pdu_ptr, uint8* offset_ptr);

/*****************************************************************************/
//  Description : �ص���������Ҫ����push��Ϣ���� 
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


