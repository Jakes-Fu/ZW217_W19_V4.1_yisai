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
#ifndef  _MMI_ZDTHTTP_API_H_    
#define  _MMI_ZDTHTTP_API_H_   

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
#include "http_api.h"

#ifdef FILE_LOG_SUPPORT
#define ZDT_HTTP_TRACE			SCI_TRACE_LOW
#else
#define ZDT_HTTP_TRACE            SCI_TRACE_LOW
#endif

#define  HTTP_IMEI_LEN 15
#define  HTTP_IMSI_LEN 15
#define  HTTP_ICCID_LEN 20

typedef int (*HTTPRCVHANDLER) (BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id);

typedef struct _ZDT_HTTP_RCV_DATA_t
{
    uint8          * pRcv;
    uint32         len;
} ZDT_HTTP_RCV_DATA_T;

typedef struct _ZDT_HTTP_RCV_NODE_t
{
       ZDT_HTTP_RCV_DATA_T  data;
       struct _ZDT_HTTP_RCV_NODE_t * next_ptr;
} ZDT_HTTP_RCV_NODE_T;


typedef struct
{
    BOOLEAN is_get;
    uint8 * ip_str;
    uint8 * str;
    uint32 str_len;
    uint8 * file_fullname;
    uint32 file_len;
    uint16 type;
    uint8 times;
    uint8 priority;
    uint32 timeout;
    HTTPRCVHANDLER rcv_handle;
}ZDT_HTTP_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZDT_HTTP_DATA_T * p_http_data;
} ZDT_HTTP_SIG_T;

typedef struct MMI_GPRS_RSP_t
{
    BOOLEAN  need_stop; 	// Ê£Óà·¢ËÍ´ÎÊý
    BOOLEAN  is_sucess;
    uint32   err_id;
} ZDT_HTTP_RSP_T;

typedef struct
{
	SIGNAL_VARS
       ZDT_HTTP_RSP_T rsp;
} ZDT_HTTP_RSP_SIG_T ;

typedef struct _ZDT_HTTP_DATA_LINK_t
{
    ZDT_HTTP_DATA_T  * p_data;
    struct _ZDT_HTTP_DATA_LINK_t * next_ptr;
} ZDT_HTTP_DATA_LINK_NODE_T;

#ifdef ZDT_CYHEALTH_SUPPORT
#define ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH "D:\\UrlFile\\healthbase64.jpg"
#define ZDT_HTTP_HEALTH_URL_FULLPACH "D:\\UrlFile\\health.png"

#define ZDT_HTTP_NUCLEIC_URL_FULLPACH "D:\\UrlFile\\nucleic.png"

#define ZDT_HTTP_ACC_URL_FULLPACH "D:\\UrlFile\\acc.png"

#define ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH_W L"D:\\UrlFile\\healthbase64.jpg"
#define ZDT_HTTP_HEALTH_URL_FULLPACH_W L"D:\\UrlFile\\health.png"

#define ZDT_HTTP_NUCLEIC_URL_FULLPACH_W L"D:\\UrlFile\\nucleic.png"

#define ZDT_HTTP_ACC_URL_FULLPACH_W L"D:\\UrlFile\\acc.png"

#define ZDT_HTTP_HEALTH_TYPE1_FULLPACH "D:\\Type1File\\health.png"

#define ZDT_HTTP_NUCLEIC_TYPE1_FULLPACH "D:\\Type1File\\nucleic.png"

#define ZDT_HTTP_ACC_TYPE1_FULLPACH "D:\\Type1File\\acc.png"

#define ZDT_HTTP_HEALTH_TYPE1_FULLPACH_W L"D:\\Type1File\\health.png"

#define ZDT_HTTP_NUCLEIC_TYPE1_FULLPACH_W L"D:\\Type1File\\nucleic.png"

#define ZDT_HTTP_ACC_TYPE1_FULLPACH_W L"D:\\Type1File\\acc.png"
#endif

PUBLIC BOOLEAN  MMIZDT_HTTP_SendSigTo_APP(ZDTHTTP_APP_SIG_E sig_id, ZDT_HTTP_DATA_T * p_http_data);

PUBLIC BOOLEAN MMIZDT_HTTP_Init(void);
PUBLIC BOOLEAN MMIZDT_HTTP_AppSend(BOOLEAN is_get,char * ip_str,uint8 *str,uint32 str_len,uint16 type,uint8 priority,uint8 repeat_times,uint32 timeout,uint8 *file_fullname,uint32 file_len, HTTPRCVHANDLER rcv_handle);
PUBLIC BOOLEAN MMIZDT_HTTP_IsSending(void);
PUBLIC BOOLEAN MMIZDT_HTTP_AllowClose(void);

PUBLIC BOOLEAN  MMIZDT_HTTP_Handle_Send(DPARAM param);
PUBLIC BOOLEAN  MMIZDT_HTTP_Handle_ReSend(DPARAM param);
PUBLIC BOOLEAN  MMIZDT_HTTP_Handle_Rcv(DPARAM param);
PUBLIC BOOLEAN  MMIZDT_HTTP_Handle_SendOver(DPARAM param);

PUBLIC uint16  MMIHTTP_Net_GetIMEI(MN_DUAL_SYS_E dual_sys,char * imei_ptr);
PUBLIC uint16  MMIHTTP_Get_SIM_IMSI(MN_DUAL_SYS_E dual_sys,char * imsi_ptr);
PUBLIC uint16  MMIHTTP_Get_SIM_ICCID(MN_DUAL_SYS_E dual_sys,char * iccid_ptr);
PUBLIC uint16  MMIHTTP_Get_IMSI_Str(MN_IMSI_T imsi,char * imsi_ptr);
PUBLIC int MMIZDT_HTTP_GetlCurAllUrl(uint8 **ppStr);

PUBLIC BOOLEAN ZDT_HTTP_RCV_Add(uint8 * pRcv, uint32 rcv_len);
PUBLIC BOOLEAN ZDT_HTTP_RCV_AddExt(uint8 * pRcv, uint32 rcv_len);
PUBLIC BOOLEAN  ZDT_HTTP_RCV_Get(ZDT_HTTP_RCV_DATA_T * pUDPData);

//zmt add start
PUBLIC uint8 *makeSignString(uint8 *app_id,uint8 *app_secret,uint8 *time_stamp);
PUBLIC uint8 *makeBaseQueryUrlString(uint8 *app_id,uint8 *app_secret);
PUBLIC uint8 *makeHttpHeaderString(uint8 *app_id,uint8 *app_secret);
//zmt add end

extern char         g_http_phone_imei[HTTP_IMEI_LEN+1];
extern char         g_http_phone_imei_2[HTTP_IMEI_LEN+1];
extern char         g_http_sim_imsi[HTTP_IMSI_LEN+1];
extern char         g_http_sim_iccid[HTTP_ICCID_LEN+1];

#ifdef __cplusplus
    }
#endif

#endif
