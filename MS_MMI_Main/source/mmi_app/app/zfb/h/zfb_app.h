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
#ifndef  _MMI_ZFB_APP_H_    
#define  _MMI_ZFB_APP_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "zfb_common.h"
#include "zfb_db.h"
#include "zfb_id.h"
#include "zfb_menutable.h"
#include "zfb_nv.h"
#include "zfb_text.h"
#include "zfb_image.h"
#include "zfb_anim.h"
#include "zfb_net.h"
#include "zfb_id2.h"
#include "alipay_common.h"
#include "alipay_payment_sdk.h"
#include "zfb_alipay.h"

#define ZFBAPP_TRACE            SCI_TRACE_LOW


typedef struct
{
       uint8 *        str;
	uint32         len; 
} MMI_ZFB_DATA_T;

typedef struct
{
	SIGNAL_VARS
       MMI_ZFB_DATA_T data;
} MMI_ZFB_SIG_T ;

typedef enum _ZFB_APIPAY_STATE_e
{
    ZFB_APP_STATE_NULL,
    ZFB_APP_STATE_INIT,
    ZFB_APP_STATE_BIND,
    ZFB_APP_STATE_BINDING,
    ZFB_APP_STATE_PAY,
    ZFB_APP_STATE_OBTAIN,
    ZFB_APP_STATE_ERR,
    ZFB_APP_STATE_MAX
}ZFB_APP_STATE_E;

extern BOOLEAN MMIZFB_PayTime_Check(void);
extern void MMIZFB_AppInit(void);
extern void MMIZFB_RegWinIdNameArr(void);
extern void MMIZFB_InitModule(void);
extern void MMIZFB_RegMenuGroup(void);
extern void MMIZFB_RegNv(void);
extern BOOLEAN  MMIZFB_SendSigTo_APP(ZFB_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len);
extern BOOLEAN MMIZFB_SendCallBackMsg(uint16 msg_id);

BOOLEAN MMIZFB_Pay_Init(void);

extern ZFB_APP_STATE_E      g_zfb_app_status;

extern void ZFB_HTTPSendTrace(const char *string, int len);
extern void ZFB_HTTPRcvTrace(const char *string, int len);
extern void ZFB_ID2TraceExt(const char *string, int len);
extern void ZFB_ID2Trace(const char *pszFmt, ...);
extern void ZFB_TraceCR(const char *data_str, int data_len);
extern void ZFB_HexTraceU8(const char *str, int len);
extern void ZFB_AlipayTraceExt(const char *string, int len);
extern void ZFB_AlipayTraceCR(const char *data_str, int data_len);
extern void ZFB_AlipayTrace(const char *pszFmt, ...);

#ifdef __cplusplus
    }
#endif

#endif
