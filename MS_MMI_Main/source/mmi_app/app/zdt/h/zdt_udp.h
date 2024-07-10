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
#ifndef  _MMI_ZDT_UDP_H_    
#define  _MMI_ZDT_UDP_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include "sci_types.h"
#include "mmi_common.h"
#include <stdio.h>
#include "nvitem.h"
#ifdef _RTOS
#include "sci_api.h" /*@tony.yao  replase sci_mem.h with sci_api.h*/
#else
#include "tasks_id.h"
#endif
#include "mn_api.h"
#include "mn_events.h"
#include "mn_type.h"
#include "nv_item_id.h"
#include "sio.h"
#include "sig_code.h"
#include "os_api.h"
#include "mmi_signal_ext.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

typedef enum _ZDT_UDP_TASK_SIGNAL
{
	SIG_ZDT_UDP_START = 0xF401,
	SIG_ZDT_UDP_SEND,
	SIG_ZDT_UDP_SEND_OVER,
	SIG_ZDT_UDP_RCV,
	SIG_ZDT_UDP_MAX
}ZDT_UDP_TASK_SIG_E;

typedef struct
{
    uint8 * str;
    uint16 str_len;
    uint16 port;
    uint32 ip_addr;
    uint8  * ip_str;
}ZDT_UDP_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZDT_UDP_DATA_T * p_udp_data;
} ZDT_UDP_SIG_T;

typedef struct _ZDT_UDP_DATA_LINK_t
{
    ZDT_UDP_DATA_T  * p_data;
    struct _ZDT_UDP_DATA_LINK_t * next_ptr;
} ZDT_UDP_DATA_LINK_NODE_T;


PUBLIC BOOLEAN  MMIZDT_UDP_SendSigTo_APP(ZDT_APP_SIG_E sig_id, ZDT_UDP_DATA_T * p_udp_data);
PUBLIC BOOLEAN  MMIZDT_UDP_SendSigTo_Task(ZDT_UDP_TASK_SIG_E sig_id, ZDT_UDP_DATA_T * p_udp_data);

PUBLIC BOOLEAN MMIZDT_UDP_Init(void);
PUBLIC BOOLEAN MMIZDT_UDP_AppSend(uint8 *str,uint16 str_len,char * ip_str,uint16 port);
PUBLIC BOOLEAN MMIZDT_UDP_IsSending(void);
PUBLIC BOOLEAN MMIZDT_UDP_AllowClose(void);

PUBLIC BOOLEAN  MMIZDT_UDP_Handle_PDPActiveOK(void);
PUBLIC BOOLEAN  MMIZDT_UDP_Handle_PDPActiveFail(void);
PUBLIC BOOLEAN  MMIZDT_UDP_Handle_PDPRespond(void);
PUBLIC MMI_RESULT_E  MMIZDT_UDP_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param);
PUBLIC BOOLEAN MMIZDT_UDP_APP_Init(void);

#ifdef __cplusplus
    }
#endif

#endif
