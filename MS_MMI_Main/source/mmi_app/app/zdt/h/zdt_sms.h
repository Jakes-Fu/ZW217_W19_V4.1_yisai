/****************************************************************************
** File Name:      mmiphs_app.h                                                *
** Author:                                                                 *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshengqi         Create
** 
****************************************************************************/
#ifndef  _MMI_ZDT_SMS_H_    
#define  _MMI_ZDT_SMS_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "zdt_common.h"

typedef enum
{
    ZDT_SMS_MSG = 1,
    ZDT_GPRS_SMS_MSG,
    ZDT_SMS_GPRS_MSG,
    ZDT_SMS_SEND_TYPE_MAX
}ZDT_SMS_SEND_TYPE_E;

typedef struct MMI_ZDT_SMS_DATA_t
{
    uint8* msg; 	// 需要发送的数据
    uint8* num; 	// 需要发送的数据
    uint16 msg_len;
    uint16 num_len;
    uint16 send_type;
    uint8 dcs;
    uint8 dual_sys;
    uint8 times;
    uint8 is_delay;
} MMI_ZDT_SMS_DATA_T;

typedef struct
{
    SIGNAL_VARS
    MMI_ZDT_SMS_DATA_T * p_sms_data;
} MMI_ZDT_SMS_SIG_T ;

typedef struct _ZDT_SMS_DATA_LINK_t
{
    MMI_ZDT_SMS_DATA_T  * p_data;
    struct _ZDT_SMS_DATA_LINK_t * next_ptr;
} ZDT_SMS_DATA_LINK_NODE_T;

typedef enum
{
	ZDT_SMS_SERVER_NULL,
	ZDT_SMS_SERVER_HEAD,
	ZDT_SMS_SERVER_TAIL,
}ZDTSMSServerPos;

MMI_RESULT_E  MMIZDT_SMS_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param);

BOOLEAN  MMIZDT_SMS_Rcv_IsValid(MN_DUAL_SYS_E dual_sys,uint8 *sms_num_ptr, uint8 sms_num_len,uint8 * sms_data_ptr, uint32 sms_data_len,BOOLEAN is_ucs2);

BOOLEAN  MMIZDT_SMS_IsSending(void);
BOOLEAN  MMIZDT_SMS_AppSendType(ZDT_SMS_SEND_TYPE_E send_type,MN_DUAL_SYS_E dual_sys,uint8 * msg_str,uint16 msg_len,uint8 * num_str,uint16 num_len,uint8 dcs,uint16 repeat_times);
BOOLEAN  MMIZDT_SMS_AppSend(MN_DUAL_SYS_E dual_sys,uint8 * msg_str,uint16 msg_len,uint8 * num_str,uint16 num_len,uint8 dcs,uint16 repeat_times);
BOOLEAN  MMIZDT_SMS_AppReSend(void);
BOOLEAN  ZDT_SMS_UCS_is_All_Ascii(uint8*  src_ptr,uint16  src_len);

#ifdef ZDT_GPS_SUPPORT
BOOLEAN  MMIZDT_GPS_SMS_Send_Loc(BOOLEAN loc_ok);
#endif
#ifdef ZDT_PRODUCT_Z805
BOOLEAN  MMIZDT_SOS_SMS_Send(void);
#endif
#ifdef __cplusplus
    }
#endif

#endif
