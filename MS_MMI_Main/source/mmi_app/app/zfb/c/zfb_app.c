/****************************************************************************
** File Name:      mmicdma_app.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 tlt, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef  _MMI_ZFB_APP_C_
#define _MMI_ZFB_APP_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "zfb_app.h"
#include "mmicc_export.h"
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
#include "IN_message.h"
#include "socket_types.h"
#include "cJSON.h"

MMI_APPLICATION_T   		g_zfb_app;
static BOOLEAN s_zfb_app_init = FALSE;
static uint8 s_zfb_app_pay_init_timer_id = 0;
static uint8 s_zfb_app_pay_init_timers = 0;
static BOOLEAN s_zfb_app_pay_is_initing = FALSE;
static BOOLEAN s_zfb_app_pay_is_init_ok = FALSE;

ZFB_APP_STATE_E      g_zfb_app_status = ZFB_APP_STATE_NULL;

void MMIZFB_Pay_Init_TimeStop(void);
void MMIZFB_Pay_Init_TimeStart(uint16 time_s);

BOOLEAN MMIZFB_PayTime_Check(void)
{
    uint32 cur_time = ZFBGetSystemTime();
#if defined(MMI_UPDATE_TIME_SUPPORT)
    if(cur_time < ZFB_SYS_CHECK_START_TIME)
    {
        if(TRUE == ZFB_Net_IsAllow())
        {
            MMIAPISET_ResetUpdateTimeDefault();
            //MMIAPIAUT_SetIsFirstQuerySNTPFlag(TRUE);
            MMIAPIPHONE_GetNitzTime(0);
        }
        return FALSE;
    }
#endif
    return TRUE;
}

static void MMIZFB_Pay_Init_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{

    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_START,PNULL,0);
}

void MMIZFB_Pay_Init_TimeStart(uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 30;
    }

    if(0 != s_zfb_app_pay_init_timer_id)
    {
        MMK_StopTimer(s_zfb_app_pay_init_timer_id);
        s_zfb_app_pay_init_timer_id = 0;
    }
    
    s_zfb_app_pay_init_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        MMIZFB_Pay_Init_TimeOut, 
                                                                        0, 
                                                                        FALSE);
}

void MMIZFB_Pay_Init_TimeStop(void)
{
    if(0 != s_zfb_app_pay_init_timer_id)
    {
        MMK_StopTimer(s_zfb_app_pay_init_timer_id);
        s_zfb_app_pay_init_timer_id = 0;
    }
}

BOOLEAN MMIZFB_Pay_Init(void)
{
    SCI_TRACE_LOW("MMIZFB_Pay_Init Start");
    s_zfb_app_pay_init_timers = 0;
    MMIZFB_Pay_Init_TimeStart(5);
    return FALSE;
}

LOCAL BOOLEAN  MMIZFB_Handle_AlipayInitStart(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    BOOLEAN ret = FALSE;
    if(s_zfb_app_pay_is_initing == TRUE)
    {
        if(p_getdata!= PNULL && p_getdata->str != PNULL)
        {
            SCI_FREE(p_getdata->str);
        }
        return TRUE;
    }
    s_zfb_app_pay_is_initing = TRUE;

    if(ZFB_DB_INIT_OK_Get())
    {
        if(MMIZFB_PayTime_Check())
        {
            ret = TRUE;
            ZFBPay_Init(0);
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_ERR_TIME);
        }
    }
    else
    {
        if(FALSE == ZFB_SIM_Exsit())
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_ERR_SIM);
            if(p_getdata!= PNULL && p_getdata->str != PNULL)
            {
                SCI_FREE(p_getdata->str);
            }
            return FALSE;
        }
        
        if(TRUE == ZFB_Net_IsAllow())
        {
            if(MMIZFB_PayTime_Check())
            {
                ret = TRUE;
                ZFBPay_Init(0);
            }
            else
            {
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_ERR_TIME);
            }
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_ERR_NET);
        }
    }
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
    return ret;
}

LOCAL BOOLEAN  MMIZFB_Handle_AlipayInitOver(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL && p_getdata->len != ZFB_APIPAY_INIT_SUCCESS)
    {
        ZFBAPP_TRACE("MMIZFB_Net_Handle_AlipayInitOver FAIL %d",p_getdata->len);
        g_zfb_app_status = ZFB_APP_STATE_ERR;
        if(p_getdata->len == ZFB_APIPAY_INIT_ERR_TIME 
            || p_getdata->len == ZFB_APIPAY_INIT_ERR_NET
            )
        {
            MMIZFB_Pay_Init_TimeStart(10);
        }
        else if(p_getdata->len == ZFB_APIPAY_INIT_ERR_SIM)
        {
        }
        else
        {
            s_zfb_app_pay_init_timers++;
            if( s_zfb_app_pay_init_timers < 4)
            {
                //MMIZFB_Pay_Init_TimeStart(5);
            }
        }
        if(p_getdata->len == ZFB_APIPAY_INIT_ERR_NET)
        {
            MMIZFB_Net_API_Close();
        }
        
        MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR_INIT);
    }
    else
    {
        ZFBAPP_TRACE("MMIZFB_Net_Handle_AlipayInitOver OK");
        g_zfb_app_status = ZFB_APP_STATE_INIT;
        MMIZFB_Pay_Init_TimeStop();
        MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_INIT);
    }
    s_zfb_app_pay_is_initing = FALSE;
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

LOCAL BOOLEAN  MMIZFB_Handle_AlipayGetBindStatusCFG(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL)
    {
        ZFBAPP_TRACE("MMIZFB_Handle_AlipayGetBindStatusCFG %d",p_getdata->len);
        if(ZFB_APIPAY_BINDSTATUS_BINDED == p_getdata->len)
        {
            if(FALSE == ZFB_DB_INIT_OK_Get())
            {
                ZFB_DB_INIT_OK_Set();
            }
            g_zfb_app_status = ZFB_APP_STATE_PAY;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_PAY);
        }
        else if(ZFB_APIPAY_BINDSTATUS_UNBIND == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_BIND;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_BIND);
        }
        else if(ZFB_APIPAY_BINDSTATUS_ERR_CODE == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR_BIND);
            //ZFBPay_Init_Restart();
        }
        else if(ZFB_APIPAY_BINDSTATUS_ERR_INIT == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR_INIT);
        }
        else if(ZFB_APIPAY_BINDSTATUS_ERR_NET== p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR);
            MMIZFB_Net_API_Close();
        }
        else
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR);
        }
    }
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

LOCAL BOOLEAN  MMIZFB_Handle_BindingStart(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    MMIZFB_SendCallBackMsg(MSG_ZDB_BINDING_START);
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

LOCAL BOOLEAN  MMIZFB_Handle_BindingOver(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);

    if(p_getdata != NULL)
    {
        if(ZFB_APIPAY_BINDING_SUCCESS == p_getdata->len)
        {
            if(FALSE == ZFB_DB_INIT_OK_Get())
            {
                ZFB_DB_INIT_OK_Set();
            }
            MMIZFB_SendCallBackMsg(MSG_ZDB_BINDING_OK);
        }
        else
        {
            MMIZFB_SendCallBackMsg(MSG_ZDB_BINDING_ERROR);
        }
    }

    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

LOCAL BOOLEAN  MMIZFB_Handle_AlipayGetPayCodeCFG(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL)
    {
        ZFBAPP_TRACE("MMIZFB_Handle_AlipayGetPayCodeCFG %d",p_getdata->len);
        if(ZFB_APIPAY_GETPAYCODE_SUCCESS == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_PAY;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_PAY);
        }
        else if(ZFB_APIPAY_GETPAYCODE_FAIL == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_BIND;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR_PAY);
        }
        else if(ZFB_APIPAY_GETPAYCODE_ERR_NET == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR);
            MMIZFB_Net_API_Close();
        }
        else
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR);
        }
    }
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

LOCAL BOOLEAN  MMIZFB_Handle_PayingOver(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);

    if(p_getdata != NULL && p_getdata->str == NULL && p_getdata->len > 0)
    {
        g_zfb_cur_pay_result = p_getdata->len;
        MMIZFB_SendCallBackMsg(MSG_ZDB_PAYING_ERROR);
    }
    else
    {
        if(p_getdata != NULL && p_getdata->str != NULL && p_getdata->len > 0)
        {
            SCI_MEMCPY(g_zfb_cur_pay_amount,p_getdata->str,p_getdata->len);
        }
        MMIZFB_SendCallBackMsg(MSG_ZDB_PAYING_OK);
    }

    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

LOCAL BOOLEAN  MMIZFB_Handle_AlipayGetObtainCodeCFG(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL)
    {
        ZFBAPP_TRACE("MMIZFB_Handle_AlipayGetObtainCodeCFG %d",p_getdata->len);
        if(ZFB_APIPAY_GETOBTAINCODE_SUCCESS == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_OBTAIN;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_OBTAIN);
        }
        else if(ZFB_APIPAY_GETOBTAINCODE_FAIL == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_BIND;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR_OBTAIN);
        }
        else if(ZFB_APIPAY_GETOBTAINCODE_ERR_NET == p_getdata->len)
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR);
            MMIZFB_Net_API_Close();
        }
        else
        {
            g_zfb_app_status = ZFB_APP_STATE_ERR;
            MMIZFB_SendCallBackMsg(MSG_ZDB_STATUS_ERR);
        }
    }
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}


LOCAL BOOLEAN  MMIZFB_Handle_AlipayGetLogonIDCFG(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL)
    {
        ZFBAPP_TRACE("MMIZFB_Handle_AlipayGetLogonIDCFG %d",p_getdata->len);
        if(ZFB_APIPAY_GET_LOGON_ID_SUCCESS == p_getdata->len)
        {
            MMIZFB_SendCallBackMsg(MSG_ZDB_LOGON_ID_OK);
        }
        else
        {
            MMIZFB_SendCallBackMsg(MSG_ZDB_LOGON_ID_ERROR);
        }
    }
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

LOCAL BOOLEAN  MMIZFB_Handle_AlipayGetNickNameCFG(DPARAM param)
{
    MMI_ZFB_SIG_T * pp_getdata = (MMI_ZFB_SIG_T *)param;
    MMI_ZFB_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL)
    {
        ZFBAPP_TRACE("MMIZFB_Handle_AlipayGetNickNameCFG %d",p_getdata->len);
        if(ZFB_APIPAY_GET_LOGON_ID_SUCCESS == p_getdata->len)
        {
            MMIZFB_SendCallBackMsg(MSG_ZDB_NICK_NAME_OK);
        }
        else
        {
            MMIZFB_SendCallBackMsg(MSG_ZDB_NICK_NAME_ERROR);
        }
    }
    
    if(p_getdata!= PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}
static MMI_RESULT_E  HandleZFBAppMsg (PWND app_ptr, 
								 uint16 msg_id, 
								 DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    if(MMI_RESULT_TRUE == MMIZFB_Net_Handle_AppMsg(app_ptr,msg_id,param))
    {
        return MMI_RESULT_TRUE;
    }
    switch(msg_id)
    {                       
        case ZFB_APP_SIGNAL_CHECK_TIME:
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_INIT_START:
                MMIZFB_Handle_AlipayInitStart(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_INIT_CFG:
                MMIZFB_Handle_AlipayInitOver(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_GETBINDSTATUS_CFG:
                MMIZFB_Handle_AlipayGetBindStatusCFG(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_BINDING_START:
               MMIZFB_Handle_BindingStart(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_BINDING_OVER:
               MMIZFB_Handle_BindingOver(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_PAYCODE_CFG:
                MMIZFB_Handle_AlipayGetPayCodeCFG(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER:
               MMIZFB_Handle_PayingOver(param);
            break;
                        
        case ZFB_APP_SIGNAL_ALIPAY_OBTAINCODE_CFG:
                MMIZFB_Handle_AlipayGetObtainCodeCFG(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_LOGON_ID_CFG:
                MMIZFB_Handle_AlipayGetLogonIDCFG(param);
            break;
            
        case ZFB_APP_SIGNAL_ALIPAY_NICK_NAME_CFG:
                MMIZFB_Handle_AlipayGetNickNameCFG(param);
            break;
            
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}
void MMIZFB_AppInit(void)
{
    g_zfb_app.ProcessMsg     = HandleZFBAppMsg;
    s_zfb_app_init = TRUE;
    MMIZFB_Net_APP_Init();
    ZFB_DB_Init();
    ZFBPay_TaskCreate();
}

BOOLEAN MMIZFB_AppIsInit(void)
{
    return s_zfb_app_init;
}

void MMIZFB_InitModule(void)
{
    MMIZFB_RegMenuGroup();
    MMIZFB_RegNv();
    MMIZFB_RegWinIdNameArr();
}

BOOLEAN  MMIZFB_SendSigTo_APP(ZFB_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_ZFB_SIG_T * psig = PNULL;
    if(s_zfb_app_init == FALSE)
    {
        return FALSE;
    }
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOCA(data_len);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("MMIZFB_SendSigTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_ZFB_SIG_T),SCI_IdentifyThread());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}


PUBLIC void ZFB_HTTPSendTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    if (len < 128)
    {
        SCI_TraceLow("[HTTP] Send:%s",string);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[HTTP] Send:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZFB_HTTPRcvTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    if (len < 128)
    {
        SCI_TraceLow("[HTTP] Rcv:%s",string);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[HTTP] Rcv:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZFB_ID2TraceExt(const char *string, int len)
{
#ifndef RELEASE_INFO
    if (len < 128)
    {
        SCI_TraceLow("[ID2]:%s",string);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[ID2]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

void ZFB_ID2Trace(const char *pszFmt, ...)
{
#ifndef RELEASE_INFO
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif

    if (tmp_len >= 0)
    {
        ZFB_ID2TraceExt(buf,tmp_len);
    }
    va_end(va_args);
#endif
}
static uint8 Nibble2HexChar(uint8 bNibble)
{
    uint8  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

static uint32 U8_To_Hex(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

PUBLIC void ZFB_HexTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("alipayu8:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("alipayu8:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZFB_HexTraceID2Set(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[ID2]:[KVSET]%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[ID2]:[KVSET]%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZFB_HexTraceID2Get(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[ID2]:[KVGET]%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[ID2]:[KVGET]%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}
int  ZFB_Trace_CR_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    int  is_cr = FALSE;
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == 0x0D || *(*pCmd + i) == 0x0A)
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
    
    if( (i+1) < *pLen && bHaveNextComma && *(*pCmd + i) == 0x0D && *(*pCmd + i + 1) == 0x0A)
    {
        is_cr = TRUE;
    }
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)
        {
            wMaxStrLen = 100;
        }
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            if(is_cr)
            {
                *pCmd += (EndIndex + 2);
                *pLen -= (EndIndex + 2);
            }
            else
            {
                *pCmd += (EndIndex + 1);
                *pLen -= (EndIndex + 1);
            }
        }
    }
    return(bRtn);
}

PUBLIC void ZFB_TraceCR(const char *data_str, int data_len)
{
#ifndef RELEASE_INFO
    int ret = 0;
    uint8 * pTime = NULL;
    uint32 con_len = 0;
    char tmp_str[128] = {0};
    
    if(data_str == NULL || data_len == 0)
    {
        return;
    }
    
    pTime = (uint8 *)data_str;
    con_len = data_len;
    while(ret != -1)
    {
        ret = ZFB_Trace_CR_GetNextPara(&pTime,&con_len,tmp_str,127);
        if(ret > 0)
        {
            SCI_TraceLow("alipaysoc:%s ",tmp_str);
        }
    }

    return;
#endif
}

PUBLIC void ZFB_AlipayTraceExt(const char *string, int len)
{
#ifndef RELEASE_INFO
    if (len < 128)
    {
        SCI_TraceLow("[alipay]:%s",string);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[alipay]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZFB_AlipayTraceCR(const char *data_str, int data_len)
{
#ifndef RELEASE_INFO
    int ret = 0;
    uint8 * pTime = NULL;
    uint32 con_len = 0;
    char tmp_str[128] = {0};
    
    if(data_str == NULL || data_len == 0)
    {
        return;
    }
    
    pTime = (uint8 *)data_str;
    con_len = data_len;
    while(ret != -1)
    {
        ret = ZFB_Trace_CR_GetNextPara(&pTime,&con_len,tmp_str,127);
        if(ret > 0)
        {
            ZFB_AlipayTraceExt(tmp_str,ret);
        }
    }
    return;
#endif
}

void ZFB_AlipayTrace(const char *pszFmt, ...)
{
#ifndef RELEASE_INFO
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        ZFB_AlipayTraceExt(buf,tmp_len);
    }
    va_end(va_args);
#endif
}

PUBLIC void ZFB_cJsonTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    if (len < 128)
    {
        SCI_TraceLow("[JSON]:%s",string);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[JSON]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZFB_cJsonTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[JSON][U8]:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[JSON][U8]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}
#endif
