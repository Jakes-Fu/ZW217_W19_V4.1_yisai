#ifndef _ALIPAY_COMMOM_H_
#define _ALIPAY_COMMOM_H_

#ifdef WIN32
#include "alipay_iot_type.h"
#else
#include "stdint.h"
#include "stdbool.h"
#endif

typedef enum {
    RV_OK = 0,
     RV_WRITE_ERROR,
     RV_READ_ERROR,
     RV_DEL_ERROR,
     RV_NOT_FOUND,
     RV_IO_ERROR,
     RV_NOT_INITIALISED,
     RV_NETWORK_ERROR,
     RV_ECC_GENERATE_ERROR,
     RV_ECC_SHARE_ERROR,
     RV_SE_INFO_ERROR,//10
     RV_SE_RESET_ERROR,
     RV_SE_GET_STATUS_ERROR,
     RV_SE_SAVE_ERROR,
     RV_SE_GENCODE_ERROR,
     RV_BUF_TOO_SHORT,
     RV_ENCRYPTION_ERRO,
     RV_DECRYPTION_ERRO,
     RV_WRONG_TIMESTAMP,
     RV_WRONG_PARAM,
     RV_PRODECT_MODEL_ERROR,//20
     RV_NOT_INPROCESSING,
     RV_SEMAPHORE_CREATE_ERROR,
     RV_SERVER_TIMEOUT,
     RV_BINDING_DATA_FORMAT_ERROR,
     RV_PROCESSING,//25
     RV_SERVER_FAIL_ERROR,
     RV_PB_PACK_ERROR,
     RV_ID2_ERROR,
     RV_JS_ERROR,
     RV_MEM_ERROR,//30
     RV_UNKNOWN = 99
} retval_e;

typedef enum {
    ALIPAY_PAYMENT_STATUS_SUCCESS          = 0,
	ALIPAY_PAYMENT_STATUS_FAIL             = 1,
	ALIPAY_PAYMENT_STATUS_NOTHING          = 2,
	ALIPAY_PAYMENT_STATUS_UNBIND_BY_USER   = 3,
	ALIPAY_PAYMENT_STATUS_DISABLED_BY_USER = 4,
    ALIPAY_PAYMENT_STATUS_UNKNOWN          = 5
} alipay_payment_status_e; 
typedef enum {
    ALIPAY_FAIL_RES_AMOUNT_LIMIT       = 0,  //超出设置的支付额度
	ALIPAY_FAIL_RES_BALANCE_NOT_ENOUGH = 1,  //零花钱不足，请存入
	ALIPAY_FAIL_RES_COMMON_FAIL        = 2,  //支付失败
} alipay_scan_pay_fail_res_e; 

typedef enum{
    prd_type_watch=0, //成人手表
    prd_type_b6t,   //成人手环
    prd_type_card,  //成人卡片
    prd_type_qzwt,  //儿童手表
    prd_type_qzb6t, //儿童手环
    prd_type_qzcrd, //儿童卡片
    prd_type_invalid
}prd_type_e;
#endif
