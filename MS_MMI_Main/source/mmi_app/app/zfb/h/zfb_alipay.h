#ifndef __ZFB_ALIPAY_H__
#define __ZFB_ALIPAY_H__

#include "zfb_common.h"

#define ZFB_ALIPAY_BIND_CODE_MAX_LEN 100
#define ZFB_ALIPAY_PAY_CODE_MAX_LEN 19
#define ZFB_ALIPAY_OBTAIN_CODE_MAX_LEN 41
#define ZFB_ALIPAY_LOGON_ID_MAX_LEN 100
#define ZFB_ALIPAY_NICK_NAME_MAX_LEN 100

#define ZFB_SYS_CHECK_START_TIME 1629694409

typedef struct
{
    SIGNAL_VARS
    uint8 *        str;
    uint32         len; 
} ZFB_PAY_TASK_SIG_T;

typedef enum _ZFB_APIPAY_INIT_ERR_e
{
    ZFB_APIPAY_INIT_SUCCESS,
    ZFB_APIPAY_INIT_ERR_TIME,
    ZFB_APIPAY_INIT_ERR_NET,
    ZFB_APIPAY_INIT_ERR_ID2,
    ZFB_APIPAY_INIT_ERR_OTP,
    ZFB_APIPAY_INIT_ERR_ALIPAY,
    ZFB_APIPAY_INIT_ERR_OTHER,
    ZFB_APIPAY_INIT_ERR_SIM,
    ZFB_APIPAY_INIT_ERR_MAX
}ZFB_APIPAY_INIT_ERR_E;

typedef enum _ZFB_APIPAY_BINDSTATUS_CFG_e
{
    ZFB_APIPAY_BINDSTATUS_BINDED,
    ZFB_APIPAY_BINDSTATUS_UNBIND,
    ZFB_APIPAY_BINDSTATUS_ERR_CODE,
    ZFB_APIPAY_BINDSTATUS_ERR_NET,
    ZFB_APIPAY_BINDSTATUS_ERR_INIT,
    ZFB_APIPAY_BINDSTATUS_MAX
}ZFB_APIPAY_BINDSTAUS_CFG_E;

typedef enum _ZFB_APIPAY_BINDING_STATUS_e
{
    ZFB_APIPAY_BINDING_SUCCESS,
    ZFB_APIPAY_BINDING_PROCCESS,
    ZFB_APIPAY_BINDING_FAIL,
    ZFB_APIPAY_BINDING_ERR_NET,
    ZFB_APIPAY_BINDING_MAX
}ZFB_APIPAY_BINDING_STATUS_E;

typedef enum _ZFB_APIPAY_PAYCODE_CFG_e
{
    ZFB_APIPAY_GETPAYCODE_SUCCESS,
    ZFB_APIPAY_GETPAYCODE_FAIL,
    ZFB_APIPAY_GETPAYCODE_ERR_NET,
    ZFB_APIPAY_GETPAYCODE_ERR,
    ZFB_APIPAY_GETPAYCODE_MAX
}ZFB_APIPAY_PAYCODE_CFG_E;


typedef enum _ZFB_APIPAY_OBTAINCODE_CFG_e
{
    ZFB_APIPAY_GETOBTAINCODE_SUCCESS,
    ZFB_APIPAY_GETOBTAINCODE_FAIL,
    ZFB_APIPAY_GETOBTAINCODE_ERR_NET,
    ZFB_APIPAY_GETOBTAINCODE_ERR,
    ZFB_APIPAY_GETOBTAINCODE_MAX
}ZFB_APIPAY_OBTAINCODE_CFG_E;

typedef enum _ZFB_APIPAY_LOGON_ID_CFG_e
{
    ZFB_APIPAY_GET_LOGON_ID_SUCCESS,
    ZFB_APIPAY_GET_LOGON_ID_NONE,
    ZFB_APIPAY_GET_LOGON_ID_FAIL,
    ZFB_APIPAY_GET_LOGON_ID_ERR_NET,
    ZFB_APIPAY_GET_LOGON_ID_MAX
}ZFB_APIPAY_LOGON_ID_CFG_E;

typedef enum _ZFB_APIPAY_NICK_NAME_CFG_e
{
    ZFB_APIPAY_GET_NICK_NAME_SUCCESS,
    ZFB_APIPAY_GET_NICK_NAME_NONE,
    ZFB_APIPAY_GET_NICK_NAME_FAIL,
    ZFB_APIPAY_GET_NICK_NAME_ERR_NET,
    ZFB_APIPAY_GET_NICK_NAME_MAX
}ZFB_APIPAY_NICK_NAME_CFG_E;

typedef enum _ZFB_APIPAY_STATUS_e
{
    ZFB_APIPAY_STATUS_NONE,
    ZFB_APIPAY_STATUS_INIT,
    ZFB_APIPAY_STATUS_BIND,
    ZFB_APIPAY_STATUS_PAY,
    ZFB_APIPAY_STATUS_OBTAIN,
    ZFB_APIPAY_STATUS_UNBIND,
    ZFB_APIPAY_STATUS_MAX
}ZFB_APIPAY_STATUS_E;

extern ZFB_APIPAY_STATUS_E g_zfb_alipay_status;

extern BOOLEAN  APP_SendSigTo_Pay(ZFB_PAY_TASK_SIG_E sig_id, const char * data_ptr,uint32 data_len);

extern void ZFBPay_TaskCreate(void);
extern void ZFBPay_TaskClose(void);
extern BLOCK_ID ZFBPay_TaskGetID(void);
extern int ZFBPay_Init(uint8 type);
extern BOOLEAN ZFBPay_Init_Restart(void);
extern BOOLEAN ZFBPay_Init_Reset(void);
extern BOOLEAN ZFBPay_IsInit(void);
extern BOOLEAN  ZFBPay_GetBindStatus(BOOLEAN is_force);
extern BOOLEAN  ZFBPay_BindingStart(void);
extern BOOLEAN  ZFBPay_QueryBindResult(void);
extern BOOLEAN  ZFBPay_QueryBindStop(void);
extern BOOLEAN  ZFBPay_GetPayCode(BOOLEAN is_force);
extern BOOLEAN  ZFBPay_QueryPayResult(void);
extern BOOLEAN  ZFBPay_QueryPayStop(void);
extern BOOLEAN  ZFBPay_GetObtainCode(BOOLEAN is_force);
extern BOOLEAN  ZFBPay_GetLogonID(void);
extern BOOLEAN  ZFBPay_GetNickName(void);

extern char g_zfb_alipay_bind_code[ZFB_ALIPAY_BIND_CODE_MAX_LEN+1];
extern int  g_zfb_alipay_bind_code_len;
extern char g_zfb_alipay_pay_code[ZFB_ALIPAY_PAY_CODE_MAX_LEN+1];
extern int  g_zfb_alipay_pay_code_len;
extern char g_zfb_alipay_obtain_code[ZFB_ALIPAY_OBTAIN_CODE_MAX_LEN+1];
extern int  g_zfb_alipay_obtain_code_len;
extern uint8 g_zfb_cur_pay_amount[32][14];
extern uint32 g_zfb_cur_pay_result;
extern char g_zfb_alipay_logon_id[ZFB_ALIPAY_LOGON_ID_MAX_LEN+1];
extern int  g_zfb_alipay_logon_id_len;
extern char g_zfb_alipay_nick_name[ZFB_ALIPAY_NICK_NAME_MAX_LEN+1];
extern int  g_zfb_alipay_nick_name_len;

#endif
