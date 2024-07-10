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
#ifndef  _MMI_ZFB_NET_H_    
#define  _MMI_ZFB_NET_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "zfb_common.h"

#define  ZFB_IMEI_LEN 15
#define  ZFB_IMSI_LEN 15
#define  ZFB_ICCID_LEN 20

typedef struct _ZFB_ICCD
{
    char num[ZFB_ICCID_LEN];
} ZFB_ICCID;

typedef int (*ZFBNETCBHANDLER) (BOOLEAN is_ok,void *pParam);

typedef struct _ZFB_NET_LINK_DATA_t
{
    uint8 type;
    uint8 priority;
    uint8 repeat_times;
    uint32 timeout;
    ZFBNETCBHANDLER cb_handle;
} ZFB_NET_LINK_DATA_T;

typedef struct _ZFB_NET_LINK_t
{
       ZFB_NET_LINK_DATA_T *  data;
       struct _ZFB_NET_LINK_t * next_ptr;
} ZFB_NET_LINK_NODE_T;

typedef struct _ZFB_NET_DATA_t
{
    uint16 type;
    void* data_p;
} ZFB_NET_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZFB_NET_DATA_T * p_data;
} ZFB_NET_DATA_SIG_T;

extern char*         g_zfb_phone_imei;
extern char         g_zfb_phone_imei_1[ZFB_IMEI_LEN+1];
extern char         g_zfb_phone_imei_2[ZFB_IMEI_LEN+1];
extern char         g_zfb_sim_imsi[ZFB_IMSI_LEN+1];
extern char         g_zfb_sim_iccid[ZFB_ICCID_LEN+1];

PUBLIC uint32 MMIZFB_Net_GetNetID(void);
PUBLIC BOOLEAN MMIZFB_Net_PdpStateGet(void);
PUBLIC BOOLEAN MMIZFB_Net_IsActiving(void);
PUBLIC BOOLEAN MMIZFB_Net_IsClosing(void);
PUBLIC BOOLEAN MMIZFB_Net_IsInit(void);
PUBLIC BOOLEAN MMIZFB_Net_Init(void);
PUBLIC BOOLEAN MMIZFB_Net_Reset(void);
PUBLIC BOOLEAN MMIZFB_Net_Open(void);
PUBLIC BOOLEAN MMIZFB_Net_Close(void);
PUBLIC BOOLEAN MMIZFB_Net_IsValid(void);

PUBLIC MN_DUAL_SYS_E MMIZFB_Net_GetActiveSys(void);
PUBLIC void MMIZFB_Net_SetActiveSys(MN_DUAL_SYS_E set_sys);

PUBLIC MMI_RESULT_E MMIZFB_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

PUBLIC BOOLEAN MMIZFB_Net_APP_Init(void);
PUBLIC BOOLEAN MMIZFB_Net_GPRS_IsOn(void);
PUBLIC uint32 MMIZFB_Net_API_Open(uint8 type,uint8 priority,uint8 repeat_times,uint32 timeout,ZFBNETCBHANDLER cb_handle);
PUBLIC BOOLEAN MMIZFB_Net_API_Close(void);


#ifdef __cplusplus
    }
#endif

#endif
