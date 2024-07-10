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
#include "id2_client.h"
#include "cJSON.h"

static BLOCK_ID zfbpay_taskid=SCI_INVALID_BLOCK_ID;
static BOOLEAN s_zfb_alipay_is_initing = FALSE;
static BOOLEAN s_zfb_alipay_is_init_ok = FALSE;
static BOOLEAN s_zfb_alipay_id2_is_init_ok = FALSE;
static BOOLEAN s_zfb_alipay_id2_is_geting_otp = FALSE;

static uint8 g_zfb_query_bind_timer_id = 0;
static uint8 g_zfb_query_pay_timer_id = 0;

ZFB_APIPAY_STATUS_E g_zfb_alipay_status = ZFB_APIPAY_STATUS_NONE;

uint8 g_zfb_cur_pay_amount[32][14] = {0};
uint32 g_zfb_cur_pay_result = 0;

char g_zfb_alipay_bind_code[ZFB_ALIPAY_BIND_CODE_MAX_LEN+1] = {0};
int  g_zfb_alipay_bind_code_len = 0;
char g_zfb_alipay_pay_code[ZFB_ALIPAY_PAY_CODE_MAX_LEN+1] = {0};
int  g_zfb_alipay_pay_code_len = 0;
char g_zfb_alipay_obtain_code[ZFB_ALIPAY_OBTAIN_CODE_MAX_LEN+1] = {0};
int  g_zfb_alipay_obtain_code_len = 0;
char g_zfb_alipay_logon_id[ZFB_ALIPAY_LOGON_ID_MAX_LEN+1] = {0};
int  g_zfb_alipay_logon_id_len = 0;
char g_zfb_alipay_nick_name[ZFB_ALIPAY_NICK_NAME_MAX_LEN+1] = {0};
int  g_zfb_alipay_nick_name_len = 0;

#ifdef WIN32
const uint8 alipay_amount[448]=
{
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x00,0x03,0x80,0x01,0x80,0x00,
     0x00,0x00,0x00,0x00,0x01,0x80,0x06,0x60,0x00,0x0c,0x70,0x06,0x60,0x00,0x00,0x00,
     0x00,0x00,0x1f,0x80,0x0c,0x30,0x00,0x18,0x18,0x0c,0x30,0x00,0x00,0x00,0x00,0x00,
     0x01,0x80,0x18,0x18,0x00,0x30,0x08,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x01,0x80,
     0x18,0x18,0x00,0x30,0x0c,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x30,0x18,
     0x00,0x30,0x0c,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x30,0x0c,0x00,0x30,
     0x0c,0x30,0x0c,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x30,0x0c,0x00,0x38,0x08,0x30,
     0x0c,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x30,0x0c,0x00,0x1c,0x18,0x30,0x0c,0x00,
     0x00,0x00,0x00,0x00,0x01,0x80,0x70,0x0c,0x00,0x0f,0x30,0x70,0x0c,0x00,0x00,0x00,
     0x00,0x00,0x01,0x80,0x70,0x0c,0x00,0x07,0xc0,0x70,0x0c,0x00,0x00,0x00,0x00,0x00,
     0x01,0x80,0x70,0x0c,0x00,0x0d,0xe0,0x70,0x0c,0x00,0x00,0x00,0x00,0x00,0x01,0x80,
     0x70,0x0c,0x00,0x18,0x70,0x70,0x0c,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x70,0x0c,
     0x00,0x30,0x38,0x70,0x0c,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x30,0x0c,0x00,0x20,
     0x1c,0x30,0x0c,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x30,0x0c,0x00,0x60,0x0c,0x30,
     0x0c,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x30,0x1c,0x00,0x60,0x0c,0x30,0x1c,0x00,
     0x00,0x00,0x00,0x00,0x01,0x80,0x38,0x18,0x00,0x60,0x0c,0x38,0x18,0x00,0x00,0x00,
     0x00,0x00,0x01,0x80,0x18,0x18,0x00,0x20,0x0c,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
     0x01,0x80,0x18,0x30,0x60,0x30,0x08,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x01,0x80,
     0x0c,0x30,0x60,0x18,0x10,0x0c,0x30,0x00,0x00,0x00,0x00,0x00,0x1f,0xf8,0x07,0xc0,
     0x60,0x0f,0xe0,0x07,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
#endif

BOOLEAN  APP_SendSigTo_Pay(ZFB_PAY_TASK_SIG_E sig_id, const char * data_ptr,uint32 data_len)
{
    uint8 * pstr = NULL;
    ZFB_PAY_TASK_SIG_T * psig = PNULL;
    BLOCK_ID cur_taskID=ZFBPay_TaskGetID();
    
    if(SCI_INVALID_BLOCK_ID == cur_taskID)
    {
        ZFBAPP_TRACE("APP_SendSigTo_Pay Err Task sig_id=0x%x,data_len=%d",sig_id,data_len);
        return FALSE;
    }
    
    if(data_len != 0 && data_ptr != NULL)
    {
        pstr = SCI_ALLOC_APPZ(data_len);//free it in gps task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("APP_SendTo_ATC Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZFB_PAY_TASK_SIG_T), SCI_IdentifyThread());
    psig->len = data_len;
    psig->str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, cur_taskID);
    return TRUE;
}


static void ZFB_QueryBind_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_QUERY_BIND_RESULT,PNULL,0);
}

void ZFB_QueryBind_TimeStart(uint16 time_ms)
{
    if(time_ms == 0)
    {
        time_ms = 200;
    }

    if(0 != g_zfb_query_bind_timer_id)
    {
        MMK_StopTimer(g_zfb_query_bind_timer_id);
        g_zfb_query_bind_timer_id = 0;
    }
    
    g_zfb_query_bind_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        ZFB_QueryBind_TimeOut, 
                                                                        0, 
                                                                        FALSE);
}

void ZFB_QueryBind_TimeStop(void)
{
    if(0 != g_zfb_query_bind_timer_id)
    {
        MMK_StopTimer(g_zfb_query_bind_timer_id);
        g_zfb_query_bind_timer_id = 0;
    }
}

static void ZFB_QueryPay_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_QUERY_PAY_RESULT,PNULL,0);
}

void ZFB_QueryPay_TimeStart(uint16 time_ms)
{
    if(time_ms == 0)
    {
        time_ms = 200;
    }

    if(0 != g_zfb_query_pay_timer_id)
    {
        MMK_StopTimer(g_zfb_query_pay_timer_id);
        g_zfb_query_pay_timer_id = 0;
    }
    
    g_zfb_query_pay_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        ZFB_QueryPay_TimeOut, 
                                                                        0, 
                                                                        FALSE);
}

void ZFB_QueryPay_TimeStop(void)
{
    if(0 != g_zfb_query_pay_timer_id)
    {
        MMK_StopTimer(g_zfb_query_pay_timer_id);
        g_zfb_query_pay_timer_id = 0;
    }
}

int  ZFB_ID2_Init_Step1_Start(void)
{
    int ret = RV_UNKNOWN;
    ret = zfb_id2_client_init();
    ZFBAPP_TRACE("ZFB_ID2_Init_Step1_Start s_zfb_alipay_id2_is_init_ok=%d,ret=%d",s_zfb_alipay_id2_is_init_ok,ret);
    if(RV_OK == ret)
    {
        ZFBAPP_TRACE("zfb_id2_client_init OK !!!");
        s_zfb_alipay_id2_is_init_ok = TRUE;
        APP_SendSigTo_Pay(SIG_ZFB_INIT_ALIPAY,PNULL,0);
    }
    else if(1 == ret)
    {
        s_zfb_alipay_id2_is_geting_otp = TRUE;
        ZFBAPP_TRACE("zfb_id2_client_init getting !!!");
    }
    else if(2 == ret)
    {
        ZFBAPP_TRACE("zfb_id2_client_init reset otp !!!");
        APP_SendSigTo_Pay(SIG_ZFB_INIT_ID2_STEP1,PNULL,0);
    }
    else
    {
        ZFBAPP_TRACE("zfb_id2_client_init FAIL=%d !!!",ret);
        APP_SendSigTo_Pay(SIG_ZFB_INIT_ALIPAY_OVER,PNULL,ZFB_APIPAY_INIT_ERR_ID2);
    }
    return ret;
}

int  ZFB_ID2_Init_Step2_Start(void)
{
    retval_e ret = RV_UNKNOWN;
    ret = zfb_id2_client_cleanup();
    ZFBAPP_TRACE("ZFB_ID2_Init_Step2_Start s_zfb_alipay_id2_is_init_ok=%d,ret=%d",s_zfb_alipay_id2_is_init_ok,ret);
    if(RV_OK == ret)
    {
        ZFBAPP_TRACE("zfb_id2_client_cleanup OK !!!");
        s_zfb_alipay_id2_is_init_ok = TRUE;
        APP_SendSigTo_Pay(SIG_ZFB_INIT_ALIPAY,PNULL,0);
    }
    else
    {
        ZFBAPP_TRACE("zfb_id2_client_cleanup Fail=%d !!!",ret);
        APP_SendSigTo_Pay(SIG_ZFB_INIT_ALIPAY_OVER,PNULL,ZFB_APIPAY_INIT_ERR_ID2);
    }
    
    return TRUE;
}

static int ZFB_Alipay_Init_CB(BOOLEAN is_ok,void *pParam)
{
    uint32 cur_type = 0;
    if(pParam != PNULL)
    {
        cur_type = pParam;
    }
    ZFBAPP_TRACE("ZFB_Alipay_Init_CB is_ok = %d,cur_type=%d",is_ok,cur_type);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_INIT_START,PNULL,cur_type);
    }
    else
    {
        if(ZFB_DB_INIT_OK_Get())
        {
            APP_SendSigTo_Pay(SIG_ZFB_INIT_START,PNULL,cur_type);
            return 1;
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_ERR_NET);
        }
    }
    return is_ok;
}

static BOOLEAN ZFB_Alipay_Init(uint8 type)
{
    uint32 cur_time = ZFBGetSystemTime();
    ZFBAPP_TRACE("ZFB_Alipay_Init type = %d",type);
    if(cur_time < ZFB_SYS_CHECK_START_TIME)
    {
        SCI_TRACE_LOW("ZFB_Alipay_Init Err Time");
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_ERR_TIME);
        return FALSE;
    }

    SCI_TRACE_LOW("ZFB_Alipay_Init Start");
    if(MMIZFB_Net_API_Open(type,0,0,0,ZFB_Alipay_Init_CB))
    {
        return TRUE;
    }
    else
    {
        if(ZFB_DB_INIT_OK_Get())
        {
            APP_SendSigTo_Pay(SIG_ZFB_INIT_START,PNULL,type);
            return TRUE;
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_ERR_NET);
        }
    }
    return FALSE;
}

int ZFBPay_Init(uint8 type)
{    
    ZFB_Alipay_Init(type);
    return 0;
}

BOOLEAN ZFBPay_Init_Restart(void)
{
    ZFB_Alipay_Init(1);
    return TRUE;
}

BOOLEAN ZFBPay_Init_Reset(void)
{
    ZFB_Alipay_Init(2);
    return TRUE;
}

BOOLEAN ZFBPay_IsInit(void)
{
    return s_zfb_alipay_is_init_ok;
}

int ZFBPay_GetBindStatus_CB(BOOLEAN is_ok,void *pParam)
{
    ZFBAPP_TRACE("ZFB_Alipay_QueryPayStatus_CB is_ok = %d",is_ok);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_BIND_STATUS,PNULL,0);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_GETBINDSTATUS_CFG,PNULL,ZFB_APIPAY_BINDSTATUS_ERR_NET);
    }
    return is_ok;
}

BOOLEAN  ZFBPay_GetBindStatus(BOOLEAN is_force)
{
    static uint32 s_last_get_bind_code_time = 0;
    uint32 cur_ms = SCI_GetTickCount();
    if(is_force || cur_ms - s_last_get_bind_code_time > 3000 || cur_ms < s_last_get_bind_code_time)
    {
        s_last_get_bind_code_time = cur_ms;
        SCI_TRACE_LOW("ZFBPay_GetBindStatus");
        if(MMIZFB_Net_API_Open(0,0,0,0,ZFBPay_GetBindStatus_CB))
        {
            return TRUE;
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_GETBINDSTATUS_CFG,PNULL,ZFB_APIPAY_BINDSTATUS_ERR_NET);
        }
    }
    return FALSE;
}

BOOLEAN  ZFBPay_BindingStart(void)
{
    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_BINDING_START,PNULL,0);
    return FALSE;
}


int ZFBPay_QueryBindResult_CB(BOOLEAN is_ok,void *pParam)
{
    ZFBAPP_TRACE("ZFBPay_QueryBindResult_CB is_ok = %d",is_ok);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_QUERY_BIND_RESULT,PNULL,0);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_BINDING_OVER,PNULL,ZFB_APIPAY_BINDING_ERR_NET);
    }
    return is_ok;
}
BOOLEAN  ZFBPay_QueryBindResult(void)
{
    SCI_TRACE_LOW("ZFBPay_QueryBindResult");
    if(MMIZFB_Net_API_Open(0,0,0,0,ZFBPay_QueryBindResult_CB))
    {
        return TRUE;
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_BINDING_OVER,PNULL,ZFB_APIPAY_BINDING_ERR_NET);
    }
    return FALSE;
}

BOOLEAN  ZFBPay_QueryBindStop(void)
{
    SCI_TRACE_LOW("ZFBPay_QueryBindStop");
    APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_QUERY_BIND_STOP,PNULL,0);
    return FALSE;
}

static int ZFBPay_GetPayCode_CB(BOOLEAN is_ok,void *pParam)
{
    ZFBAPP_TRACE("ZFB_Alipay_CheckPayStatus_CB is_ok = %d",is_ok);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_PAYCODE,PNULL,0);
    }
    else
    {
        if(ZFB_DB_INIT_OK_Get())
        {
            APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_PAYCODE,PNULL,0);
            return 1;
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYCODE_CFG,PNULL,ZFB_APIPAY_GETPAYCODE_ERR_NET);
        }
    }
    return is_ok;
}

BOOLEAN  ZFBPay_GetPayCode(BOOLEAN is_force)
{
    static uint32 s_last_get_pay_code_time = 0;
    uint32 cur_ms = SCI_GetTickCount();
    if(is_force || cur_ms - s_last_get_pay_code_time > 3000 || cur_ms < s_last_get_pay_code_time)
    {
        s_last_get_pay_code_time = cur_ms;
        SCI_TRACE_LOW("ZFB_Alipay_CheckPayStatus");
        if(MMIZFB_Net_API_Open(0,0,0,0,ZFBPay_GetPayCode_CB))
        {
            return TRUE;
        }
        else
        {
            if(ZFB_DB_INIT_OK_Get())
            {
                APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_PAYCODE,PNULL,0);
                return TRUE;
            }
            else
            {
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYCODE_CFG,PNULL,ZFB_APIPAY_GETPAYCODE_ERR_NET);
            }
        }
    }

    return FALSE;
}

static int ZFBPay_QueryPayResult_CB(BOOLEAN is_ok,void *pParam)
{
    ZFBAPP_TRACE("ZFBPay_QueryPayResult_CB is_ok = %d",is_ok);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_QUERY_PAY_RESULT,PNULL,0);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,PNULL,RV_NETWORK_ERROR);
    }
    return is_ok;
}
BOOLEAN  ZFBPay_QueryPayResult(void)
{
    ZFBAPP_TRACE("ZFBPay_QueryPayResult");
    if(MMIZFB_Net_API_Open(0,0,0,0,ZFBPay_QueryPayResult_CB))
    {
        return TRUE;
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,PNULL,RV_NETWORK_ERROR);
    }
    return FALSE;
}

BOOLEAN  ZFBPay_QueryPayStop(void)
{
    SCI_TRACE_LOW("ZFBPay_QueryPayStop");
    APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_QUERY_PAY_STOP,PNULL,0);
    return FALSE;
}

static int ZFBPay_GetObtainCode_CB(BOOLEAN is_ok,void *pParam)
{
    //ZFBAPP_TRACE("ZFB_Alipay_CheckPayStatus_CB is_ok = %d",is_ok);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_OBTAINCODE,PNULL,0);
    }
    else
    {
        if(ZFB_DB_INIT_OK_Get())
        {
            APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_OBTAINCODE,PNULL,0);
            return 1;
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_OBTAINCODE_CFG,PNULL,ZFB_APIPAY_GETOBTAINCODE_ERR_NET);
        }
    }
    return is_ok;
}

BOOLEAN  ZFBPay_GetObtainCode(BOOLEAN is_force)
{
    static uint32 s_last_get_obtain_code_time = 0;
    uint32 cur_ms = SCI_GetTickCount();
    if(is_force || cur_ms - s_last_get_obtain_code_time > 3000 || cur_ms < s_last_get_obtain_code_time)
    {
        s_last_get_obtain_code_time = cur_ms;
        //SCI_TRACE_LOW("ZFBPay_GetObtainCode");
        if(MMIZFB_Net_API_Open(0,0,0,0,ZFBPay_GetObtainCode_CB))
        {
            return TRUE;
        }
        else
        {
            if(ZFB_DB_INIT_OK_Get())
            {
                APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_OBTAINCODE,PNULL,0);
                return TRUE;
            }
            else
            {
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_OBTAINCODE_CFG,PNULL,ZFB_APIPAY_GETOBTAINCODE_ERR_NET);
            }
        }
    }
    return FALSE;
}

static int ZFBPay_GetLogonID_CB(BOOLEAN is_ok,void *pParam)
{
    //ZFBAPP_TRACE("ZFBPay_GetLogonID_CB is_ok = %d",is_ok);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_LOGON_ID,PNULL,0);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_LOGON_ID_CFG,PNULL,ZFB_APIPAY_GET_LOGON_ID_ERR_NET);
    }
    return is_ok;
}

BOOLEAN  ZFBPay_GetLogonID(void)
{
    if(MMIZFB_Net_API_Open(0,0,0,0,ZFBPay_GetLogonID_CB))
    {
        return TRUE;
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_LOGON_ID_CFG,PNULL,ZFB_APIPAY_GET_LOGON_ID_ERR_NET);
    }
    return FALSE;
}

static int ZFBPay_GetNickName_CB(BOOLEAN is_ok,void *pParam)
{
    //ZFBAPP_TRACE("ZFBPay_GetLogonID_CB is_ok = %d",is_ok);
    if(is_ok)
    {
        APP_SendSigTo_Pay(SIG_ZFB_ALIPAY_GET_NICK_NAME,PNULL,0);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_NICK_NAME_CFG,PNULL,ZFB_APIPAY_GET_NICK_NAME_ERR_NET);
    }
    return is_ok;
}

BOOLEAN  ZFBPay_GetNickName(void)
{
    if(MMIZFB_Net_API_Open(0,0,0,0,ZFBPay_GetNickName_CB))
    {
        return TRUE;
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_NICK_NAME_CFG,PNULL,ZFB_APIPAY_GET_NICK_NAME_ERR_NET);
    }
    return FALSE;
}

static BOOLEAN ZFB_Task_HandleBindGetStatus(void)
{
    char bind_code[ZFB_ALIPAY_BIND_CODE_MAX_LEN+1] = {0};
    int bind_len = ZFB_ALIPAY_BIND_CODE_MAX_LEN;
    BOOLEAN ret = FALSE;
    retval_e bind_ret;

    #ifdef WIN32
        ret = TRUE;
    #else
        ret = alipay_get_bind_status();
    #endif
    //SCI_TRACE_LOW("ZFB_Task_HandleBindGetStatus ret=%d",ret);
    if(ret)
    {
        g_zfb_alipay_status = ZFB_APIPAY_STATUS_PAY;
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_GETBINDSTATUS_CFG,PNULL,ZFB_APIPAY_BINDSTATUS_BINDED);
    }
    else
    {
        if(g_zfb_alipay_logon_id_len > 0)
        {
            g_zfb_alipay_logon_id_len = 0;
        }
        if(g_zfb_alipay_nick_name_len > 0)
        {
            g_zfb_alipay_nick_name_len = 0;
        }
    #ifdef WIN32
        bind_ret = RV_OK;
        bind_len = strlen("ALIPAY_QZWT#0#BDT_2103f71290e5bd1b361be6c27b7be3d013241be9db4#ECELL_8910#3.0.0");
        SCI_MEMCPY(bind_code,"ALIPAY_QZWT#0#BDT_2103f71290e5bd1b361be6c27b7be3d013241be9db4#ECELL_8910#3.0.0",bind_len);
    #else
        bind_ret = alipay_get_binding_code(bind_code,&bind_len);
    #endif
        if(bind_ret == RV_OK && bind_len > 0)
        {
            SCI_MEMSET(g_zfb_alipay_bind_code,0,sizeof(g_zfb_alipay_bind_code));
            SCI_MEMCPY(g_zfb_alipay_bind_code,bind_code,bind_len);
            g_zfb_alipay_bind_code_len = bind_len;
            g_zfb_alipay_status = ZFB_APIPAY_STATUS_BIND;
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_GETBINDSTATUS_CFG,PNULL,ZFB_APIPAY_BINDSTATUS_UNBIND);
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_GETBINDSTATUS_CFG,PNULL,ZFB_APIPAY_BINDSTATUS_ERR_CODE);
        }
        //SCI_TRACE_LOW("alipay_get_binding_code bind_ret=%d, code[%d]=%s",bind_ret,bind_len,g_zfb_alipay_bind_code);
    }
    
    return ret;
}

static BOOLEAN ZFB_Task_HandleQueryBindResult(void)
{
    BOOLEAN result = FALSE;
    retval_e ret = RV_UNKNOWN;
    uint16 check_times = 1;
    while(check_times)
    {
        check_times--;
    #ifdef WIN32
        ret = RV_PROCESSING;
    #else
        ret = alipay_query_binding_result();
    #endif
        //ZFBAPP_TRACE("alipay_query_binding_result ret=%d !!!",ret);
        if(ret == RV_OK)
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_BINDING_OVER,PNULL,ZFB_APIPAY_BINDING_SUCCESS);
            result = TRUE;
            break;
        }
        else if(ret == RV_PROCESSING || ret == RV_NETWORK_ERROR)
        {
            continue;
        }
        else
        {
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_BINDING_OVER,PNULL,ZFB_APIPAY_BINDING_FAIL);
            result = TRUE;
            break;
        }
    }
    if(result == FALSE)
    {
        ZFB_QueryBind_TimeStart(100);
    }
    return result;
}

static BOOLEAN ZFB_Task_HandlePayCodeGet(void)
{
    char pay_code[ZFB_ALIPAY_PAY_CODE_MAX_LEN+1] = {0};
    int pay_len = ZFB_ALIPAY_PAY_CODE_MAX_LEN;
    BOOLEAN ret = FALSE;
    retval_e pay_ret;
    
    #ifdef WIN32
        pay_ret = RV_OK;
        pay_len = strlen("283071037576849083");
        SCI_MEMCPY(pay_code,"283071037576849083",pay_len);
    #else
        pay_ret = alipay_get_paycode(pay_code,&pay_len);
    #endif
    if(pay_ret == RV_OK && pay_len > 0)
    {
        SCI_MEMSET(g_zfb_alipay_pay_code,0,sizeof(g_zfb_alipay_pay_code));
        SCI_MEMCPY(g_zfb_alipay_pay_code,pay_code,pay_len);
        g_zfb_alipay_pay_code_len = pay_len;
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYCODE_CFG,PNULL,ZFB_APIPAY_GETPAYCODE_SUCCESS);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYCODE_CFG,PNULL,ZFB_APIPAY_GETPAYCODE_FAIL);
    }
    
    //SCI_TRACE_LOW("alipay_get_paycode pay_ret=%d, paycode[%d]=%s",pay_ret,pay_len,pay_code);
    
    return ret;
}

static BOOLEAN ZFB_Task_HandleQueryPayResult(void)
{
    BOOLEAN result = FALSE;
    uint16 check_times = 1;
    retval_e ret = RV_UNKNOWN;
    int status=RV_UNKNOWN;
    uint32 recode = 0;
    char amount[32][14] = {0};
    while(check_times)
    {
        check_times--;
    #ifdef WIN32
        ret = RV_OK;
        status = ALIPAY_PAYMENT_STATUS_NOTHING;
    #else
        ret = alipay_query_order_result(&status, amount);
    #endif
        //ZFBAPP_TRACE("ZFB_Task_HandleQueryPayResult ret=%d status=%d !!!",ret,status);
        if(ret == RV_OK)
        {
            if(ALIPAY_PAYMENT_STATUS_SUCCESS == status)
            {
                #ifdef WIN32
                    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,alipay_amount,448);
                #else
                    MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,amount,sizeof(amount));
                #endif
                
                result = TRUE;
                break;
            }
            else if(ALIPAY_PAYMENT_STATUS_NOTHING == status)
            {
                //ZFB_QueryPay_TimeStart(200);
            }
            else if(ALIPAY_PAYMENT_STATUS_UNBIND_BY_USER == status)
            {
                g_zfb_alipay_pay_code_len = 0;
                ZFB_DB_INIT_OK_Delelte();
                recode = status | 0xFF000000;
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,PNULL,recode);
                result = TRUE;
                break;
            }
            else if(ALIPAY_PAYMENT_STATUS_DISABLED_BY_USER == status)
            {
                g_zfb_alipay_pay_code_len = 0;
                ZFB_DB_INIT_OK_Delelte();
                recode = status | 0xFF000000;
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,PNULL,recode);
                result = TRUE;
                break;
            }
            else
            {
                g_zfb_alipay_pay_code_len = 0;
                ZFB_DB_INIT_OK_Delelte();
                recode = status | 0xFF000000;
                MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,PNULL,recode);
                result = TRUE;
                break;
            }
        }
        else if(ret == RV_PROCESSING || ret == RV_NETWORK_ERROR || ret == RV_JS_ERROR)
        {
            //ZFB_QueryPay_TimeStart(100);
        }
        else
        {
            g_zfb_alipay_pay_code_len = 0;
            ZFB_DB_INIT_OK_Delelte();
            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_PAYING_OVER,PNULL,ret);
            result = TRUE;
            break;
        }
    }
    if(result == FALSE)
    {
        ZFB_QueryPay_TimeStart(100);
    }
    return result;
}

static BOOLEAN ZFB_Task_HandleObtainCodeGet(void)
{
    char obtain_code[ZFB_ALIPAY_OBTAIN_CODE_MAX_LEN+1] = {0};
    int obtain_len = ZFB_ALIPAY_OBTAIN_CODE_MAX_LEN;
    BOOLEAN ret = FALSE;
    retval_e obtain_ret;
    
    #ifdef WIN32
        obtain_ret = RV_OK;
        obtain_len = strlen("2103f71290e5bd1b361283071037576849083be3");
        SCI_MEMCPY(obtain_code,"2103f71290e5bd1b361283071037576849083be3",obtain_len);
    #else
        obtain_ret = alipay_get_collectioncode(obtain_code,&obtain_len);
    #endif
    if(obtain_ret == RV_OK && obtain_len > 0)
    {
        SCI_MEMSET(g_zfb_alipay_obtain_code,0,sizeof(g_zfb_alipay_obtain_code));
        SCI_MEMCPY(g_zfb_alipay_obtain_code,obtain_code,obtain_len);
        g_zfb_alipay_obtain_code_len = obtain_len;
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_OBTAINCODE_CFG,PNULL,ZFB_APIPAY_GETOBTAINCODE_SUCCESS);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_OBTAINCODE_CFG,PNULL,ZFB_APIPAY_GETOBTAINCODE_FAIL);
    }
    
    SCI_TRACE_LOW("alipay_get_collectioncode pay_ret=%d, paycode[%d]=%s",obtain_ret,obtain_len,obtain_code);
    
    return ret;
}

static BOOLEAN ZFB_Task_HandleLogonIDGet(void)
{
    char logon_id[ZFB_ALIPAY_LOGON_ID_MAX_LEN+1] = {0};
    int logon_id_len = ZFB_ALIPAY_LOGON_ID_MAX_LEN;
    BOOLEAN ret = FALSE;
    retval_e logon_ret;
    
    #ifdef WIN32
        logon_ret = RV_OK;
        logon_id_len = strlen("boo***@msn.com");
        SCI_MEMCPY(logon_id,"boo***@msn.com",logon_id_len);
    #else
        logon_ret = alipay_get_logon_ID(logon_id,&logon_id_len);
    #endif
    if(logon_ret == RV_OK && logon_id_len > 0)
    {
        SCI_MEMSET(g_zfb_alipay_logon_id,0,sizeof(g_zfb_alipay_logon_id));
        SCI_MEMCPY(g_zfb_alipay_logon_id,logon_id,logon_id_len);
        g_zfb_alipay_logon_id_len = logon_id_len;
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_LOGON_ID_CFG,PNULL,ZFB_APIPAY_GET_LOGON_ID_SUCCESS);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_LOGON_ID_CFG,PNULL,ZFB_APIPAY_GET_LOGON_ID_FAIL);
    }
    
    SCI_TRACE_LOW("alipay_get_logon_ID logon_ret=%d, id[%d]=%s",logon_ret,logon_id_len,logon_id);
    ZFB_AlipayTraceExt(logon_id,logon_id_len);
    ZFB_HexTraceU8(logon_id,logon_id_len);
    return ret;
}

static BOOLEAN ZFB_Task_HandleNickNameGet(void)
{
    char nick_name[ZFB_ALIPAY_NICK_NAME_MAX_LEN+1] = {0};
    int nick_name_len = ZFB_ALIPAY_NICK_NAME_MAX_LEN;
    BOOLEAN ret = FALSE;
    retval_e nick_ret;
    
    #ifdef WIN32
        nick_ret = RV_OK;
        nick_name_len = strlen("coluco");
        SCI_MEMCPY(nick_name,"coluco",nick_name_len);
    #else
        nick_ret = alipay_get_nick_name(nick_name,&nick_name_len);
    #endif
    if(nick_ret == RV_OK && nick_name_len > 0)
    {
        SCI_MEMSET(g_zfb_alipay_nick_name,0,sizeof(g_zfb_alipay_nick_name));
        SCI_MEMCPY(g_zfb_alipay_nick_name,nick_name,nick_name_len);
        g_zfb_alipay_nick_name_len = nick_name_len;
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_NICK_NAME_CFG,PNULL,ZFB_APIPAY_GET_NICK_NAME_SUCCESS);
    }
    else
    {
        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_NICK_NAME_CFG,PNULL,ZFB_APIPAY_GET_NICK_NAME_FAIL);
    }
    
    SCI_TRACE_LOW("alipay_get_nick_name nick_ret=%d, id[%d]=%s",nick_ret,nick_name_len,nick_name);
    ZFB_AlipayTraceExt(nick_name,nick_name_len);
    ZFB_HexTraceU8(nick_name,nick_name_len);
    
    return ret;
}


void ZFB_Alipay_TaskEntry(uint32 argc, void *argv)
{	
    TCPIP_SOCKET_T  so = 0;
    uint32 err = 0;
    zfbpay_taskid=SCI_IdentifyThread();
    ZFBAPP_TRACE("ZFBPay_TaskEntry, zfbpay_taskid=0x%x", zfbpay_taskid);
    
    while(1)
    {
    	xSignalHeaderRec*   sig_ptr = 0; 
    	sig_ptr = SCI_GetSignal( zfbpay_taskid); 
    	switch(sig_ptr->SignalCode)
    	{                                        
                case SIG_ZFB_INIT_START:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        ZFBAPP_TRACE("SIG_ZFB_INIT_START s_zfb_alipay_is_initing=%d, id2_init=%d,alipay_init=%d",s_zfb_alipay_is_initing,s_zfb_alipay_id2_is_init_ok,s_zfb_alipay_is_init_ok);
                        if(s_zfb_alipay_is_initing == TRUE)
                        {
                            ZFBAPP_TRACE("SIG_ZFB_INIT_START is initing");
                            break;
                        }
                        
                        if(gs_sig->len == 1)
                        {
                            s_zfb_alipay_id2_is_init_ok = FALSE;
                            s_zfb_alipay_is_init_ok = FALSE;
                        }
                        else if(gs_sig->len == 2)
                        {
                            ZFB_DB_INIT_OK_Delete_1();
                            s_zfb_alipay_id2_is_init_ok = FALSE;
                            s_zfb_alipay_is_init_ok = FALSE;
                        }
                        if(s_zfb_alipay_is_init_ok)
                        {
                            MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,ZFB_APIPAY_INIT_SUCCESS);
                        }
                        else
                        {
                            s_zfb_alipay_is_initing = TRUE;
                            s_zfb_alipay_id2_is_geting_otp = FALSE;
                            ZFB_ID2_Init_Step1_Start();
                        }
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                        break;
                        
                case SIG_ZFB_INIT_ID2_STEP1:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        
                        int ret2 = RV_UNKNOWN;
                        uint8 is_prov = 0;
                        ZFBAPP_TRACE("SIG_ZFB_INIT_ID2_STEP1 s_zfb_alipay_id2_is_init_ok=%d",s_zfb_alipay_id2_is_init_ok);
                        if(s_zfb_alipay_id2_is_geting_otp == FALSE)
                        {
                            ZFB_ID2_Init_Step1_Start();
                        }
                        else
                        {
                            ZFBAPP_TRACE("ZFB alipay_pre_init is_getting !!!");
                        }
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
            case SIG_ZFB_INIT_ID2_GET_OTP_CFG:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        s_zfb_alipay_id2_is_geting_otp = FALSE;
                        if(gs_sig->len >  0)
                        {
                            APP_SendSigTo_Pay(SIG_ZFB_INIT_ID2_STEP2,PNULL,0);
                        }
                        else
                        {
                            APP_SendSigTo_Pay(SIG_ZFB_INIT_ALIPAY_OVER,PNULL,ZFB_APIPAY_INIT_ERR_OTP);
                        }
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    } 
                    break;
                    
            case SIG_ZFB_INIT_ID2_STEP2:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        ZFB_ID2_Init_Step2_Start();
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
            case SIG_ZFB_INIT_ALIPAY:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        retval_e ret = RV_UNKNOWN;
                        #ifdef WIN32
                        ret = RV_OK;
                        #else
                        ret = alipay_pre_init();
                        #endif
                        if(RV_OK == ret)
                        {
                            ZFBAPP_TRACE("alipay_pre_init OK !!!");
                            s_zfb_alipay_is_init_ok = TRUE;
                            APP_SendSigTo_Pay(SIG_ZFB_INIT_ALIPAY_OVER,PNULL,ZFB_APIPAY_INIT_SUCCESS);
                        }
                        else
                        {
                            ZFBAPP_TRACE("alipay_pre_init Fail=%d !!!",ret);
                            APP_SendSigTo_Pay(SIG_ZFB_INIT_ALIPAY_OVER,PNULL,ZFB_APIPAY_INIT_ERR_ALIPAY);
                        }
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;

            case SIG_ZFB_INIT_ALIPAY_OVER:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        if(gs_sig->len == ZFB_APIPAY_INIT_SUCCESS)
                        {
                            g_zfb_alipay_status = ZFB_APIPAY_STATUS_INIT;
                        }
                        s_zfb_alipay_is_initing = FALSE;
                        MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_CFG,PNULL,gs_sig->len);
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;

             case SIG_ZFB_ALIPAY_GET_BIND_STATUS:
                        ZFB_Task_HandleBindGetStatus();
                    break;
                    
             case SIG_ZFB_ALIPAY_QUERY_BIND_RESULT:
                 {
                     ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                     ZFB_Task_HandleQueryBindResult();
                     if(gs_sig->len >  0 && gs_sig->str!= NULL)
                     {
                         SCI_FREE(gs_sig->str);
                     }
                 }
                 break;
                 
            case SIG_ZFB_ALIPAY_QUERY_BIND_STOP:
                {
                    ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                    ZFBAPP_TRACE("SIG_ZFB_ALIPAY_QUERY_BIND_STOP ZFB_QueryBind_TimeStop");
                    ZFB_QueryBind_TimeStop();                       
                    if(gs_sig->len >  0 && gs_sig->str!= NULL)
                    {
                        SCI_FREE(gs_sig->str);
                    }
                }
                break;
                    
             case SIG_ZFB_ALIPAY_GET_PAYCODE:
                        ZFB_Task_HandlePayCodeGet();
                break;
                
            case SIG_ZFB_ALIPAY_QUERY_PAY_RESULT:
                {
                    ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                    ZFB_Task_HandleQueryPayResult();
                    if(gs_sig->len >  0 && gs_sig->str!= NULL)
                    {
                        SCI_FREE(gs_sig->str);
                    }
                }
                break;
                    
                case SIG_ZFB_ALIPAY_QUERY_PAY_STOP:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        ZFBAPP_TRACE("SIG_ZFB_ALIPAY_PAYING_CHECK_STOP");
                        ZFB_QueryPay_TimeStop();
                        //alipay_query_flow_break();
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZFB_ALIPAY_GET_OBTAINCODE:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        ZFB_Task_HandleObtainCodeGet();
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZFB_ALIPAY_GET_LOGON_ID:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        ZFB_Task_HandleLogonIDGet();
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                case SIG_ZFB_ALIPAY_GET_NICK_NAME:
                    {
                        ZFB_PAY_TASK_SIG_T *  gs_sig = (ZFB_PAY_TASK_SIG_T*)sig_ptr;
                        ZFB_Task_HandleNickNameGet();
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                default:
                    break;
    	}
    	SCI_FREE(sig_ptr);
    }
}

void ZFBPay_TaskCreate(void)
{
      ZFBAPP_TRACE("ZFBPay_TaskCreate, zfbpay_taskid=0x%x", zfbpay_taskid);
	if(zfbpay_taskid == SCI_INVALID_BLOCK_ID)
	{
		zfbpay_taskid = SCI_CreateThread("ZFBPAYTHREAD",
			"ZFBPAY_QUEUE",
			ZFB_Alipay_TaskEntry,
			NULL,
			NULL,
			1024*50,  //300 bao modefied for mem  cust
			100,
			TX_SYSTEM_NORMALE,
			SCI_PREEMPT,
			SCI_AUTO_START
			);
        
		ZFBAPP_TRACE("ZFBPay_TaskCreate, zfbpay_taskid=0x%x", zfbpay_taskid);
	}
}

void ZFBPay_TaskClose(void)
{
    ZFBAPP_TRACE("ZFBPay_TaskClose zfbpay_taskid=0x%x",zfbpay_taskid);
    if(SCI_INVALID_BLOCK_ID != zfbpay_taskid)
    {
        SCI_TerminateThread(zfbpay_taskid);
        SCI_DeleteThread(zfbpay_taskid);
        zfbpay_taskid = SCI_INVALID_BLOCK_ID;       
    }
}

BLOCK_ID ZFBPay_TaskGetID(void)
{
	return zfbpay_taskid;
}

#endif
