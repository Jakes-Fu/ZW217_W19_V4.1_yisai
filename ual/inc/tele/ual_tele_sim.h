/*****************************************************************************
** Copyright 2022 Unisoc(Shanghai) Technologies Co.,Ltd                      *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*****************************************************************************
** File Name:      ual_tele_sim.h
** Author:
** Description:
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                   NAME                       DESCRIPTION
** 2022/08/01       Jichuan.Zhang             Create
******************************************************************************/
#ifndef _UAL_TELE_SIM_H_
#define _UAL_TELE_SIM_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ual_tele_common.h"
#include "ual_common.h"
#include "ual_cms.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define UAL_TELE_SIM_IMSI_MAX_LEN 15 //imsi max  length
#define UAL_TELE_SIM_GID_MAX_LEN 32 //gid max  length
#define UAL_TELE_SIM_SPN_MAX_LEN 16 //spn max  length
#define UAL_TELE_SIM_OPN_MAX_LEN 60 //opn max  length
#define UAL_TELE_SIM_ICCID_MAX_LEN 20 //iccid max  length
#define UAL_TELE_SIM_PIN_PUK_MAX_LEN 8//pin puk max  length
#define UAL_TELE_SIM_PIN_PUK_MIN_LEN 4//pin puk min  length
#define UAL_TELE_SIM_MSISDN_MAX_LEN 40 //msisdn max  length
#define UAL_TELE_SIM_ECC_MAX_LEN            10
#define UAL_TELE_SIM_ECC_MAX_NUM            12
#define UAL_TELE_SIM_ECC_ALPHA_ID_MAX_LEN   25

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/* API result*/
typedef enum
{
    UAL_TELE_SIM_RES_SUCCESS = UAL_TELE_RES_SUCCESS,
    UAL_TELE_SIM_RES_PARAM_ERROR = UAL_MODULE_TELE_SIM << 16,
    UAL_TELE_SIM_RES_ERROR,
    UAL_TELE_SIM_RES_MAX
} ual_tele_sim_result_e;

//SIM Service MSG
typedef enum
{
    MSG_UAL_TELE_SIM_START = UAL_MODULE_TELE_SIM << 16,
    MSG_UAL_TELE_SIM_STATUS_IND,
    MSG_UAL_TELE_SIM_PLUG_IN_IND,
    MSG_UAL_TELE_SIM_PLUG_OUT_IND,
    MSG_UAL_TELE_SIM_PIN_PUK_IND,
    MSG_UAL_TELE_SIM_OPERATE_PIN_CNF,
    MSG_UAL_TELE_SIM_OPN_SPN_IND,
    MSG_UAL_TELE_NETWORK_STATUS_IND,
}ual_tele_sim_msg_e;

/* SIM status*/
typedef enum
{
    UAL_TELE_SIM_STATUS_READY = 0,      // SIM OK
    UAL_TELE_SIM_STATUS_PIN_BLOCKED,    //SIM PIN LOCKED
    UAL_TELE_SIM_STATUS_PUK_BLOCKED,    //SIM LUK LOCKED
    UAL_TELE_SIM_STATUS_SIM_LOCKED,     //SIM LOCKED
    UAL_TELE_SIM_STATUS_NO_SIM,         //NO SIM
    UAL_TELE_SIM_STATUS_NOT_INITED,     // SIM NOT INITED
    UAL_TELE_SIM_STATUS_NOT_READY,      //sim当前不可用
    UAL_TELE_SIM_STATUS_MAX
} ual_tele_sim_status_e;

typedef enum
{
    UAL_TELE_SIM_PIN_REQ_TYPE_VERIFY,
    UAL_TELE_SIM_PIN_REQ_TYPE_CHANGE,
    UAL_TELE_SIM_PIN_REQ_TYPE_DISABLE,
    UAL_TELE_SIM_PIN_REQ_TYPE_ENABLE,
    UAL_TELE_SIM_PIN_REQ_TYPE_UNBLOCK
}ual_tele_sim_pin_req_type_e;

typedef enum
{
    UAL_TELE_SIM_PIN1 = 0x01,
    UAL_TELE_SIM_PIN2 = 0x02
}ual_tele_sim_pin_indication_e;

typedef enum
{
    UAL_TELE_SIM_GID1 = 0x01,
    UAL_TELE_SIM_GID2 = 0x02
}ual_tele_sim_gid_type_e;

typedef enum
{
    UAL_TELE_SIM_PIN_REQ_OK,
    UAL_TELE_SIM_PIN_WRONG_REQ_TYPE,            //the PIN request type is wrong
    UAL_TELE_SIM_PIN_ERROR_WITH_BLOCKED,        //the PIN operation error and the PIN been blocked
    UAL_TELE_SIM_PIN_ERROR_CONTRADICT_STATUS,   //the PIN status is contradict to the PIN operation
    UAL_TELE_SIM_PIN_ERROR_NOT_INITIALIZED,     //the PIN still not been initialized
    UAL_TELE_SIM_PIN_ERROR_PIN_FAIL,            //do not fulfill the access condition of the PIN operation
    UAL_TELE_SIM_PIN_ERROR_NOT_SUPPORT,         //this function not support to do
    UAL_TELE_SIM_PIN_ERROR_UNKNOW               //unknow PIN operation error
}ual_tele_sim_pin_req_result_e;

typedef struct
{
    boolean is_plmn_display;
    char spn_str[UAL_TELE_SIM_SPN_MAX_LEN+1];
}ual_tele_sim_spn_t;

typedef struct
{
    char        ecc_code[UAL_TELE_SIM_ECC_MAX_LEN+1];
    boolean     alpha_id_flag;
    uint8       alpha_id_len;
    uint8       alpha_id[UAL_TELE_SIM_ECC_ALPHA_ID_MAX_LEN+1];
    boolean     ecc_category_flag;
    uint8       ecc_category;
}ual_tele_sim_ecc_num_t;

typedef struct
{
    uint8                       validate_num;
    ual_tele_sim_ecc_num_t      call_codes[UAL_TELE_SIM_ECC_MAX_NUM];
}ual_tele_sim_ecc_info_t;

typedef struct
{
    boolean     pin_verified;
    boolean     pin_blocked;     //PIN blocked or not
    uint8       false_remain;    //the number of false presentations remaining
}ual_tele_sim_pin_status_t;

/* SIM state ind info */
typedef struct
{
    uint32 sim_id;//ual_tele_sim_id_e
    uint32 sim_status;//ual_tele_sim_status_e
} ual_tele_sim_status_ind_t;

/* SIM plug in ind info */
typedef struct
{
    uint32 sim_id;//ual_tele_sim_id_e
} ual_tele_sim_plug_in_ind_t;

/* SIM plug out ind */
typedef struct
{
    uint32 sim_id;//ual_tele_sim_id_e
} ual_tele_sim_plug_out_ind_t;

/* SIM pin puk ind info */
typedef struct
{
    uint32                        pin_type;//ual_tele_sim_pin_indication_e
    boolean                       pin_is_blocked; // PIN1 or blocked PIN1
    uint8                         remain_times;   // the value is attempt times of PIN while pin_is_blocked's value is FALSE; and the value is attempt times of PUK while pin_is_blocked's value is TRUE
    uint32                        sim_id;//ual_tele_sim_id_e
} ual_tele_sim_pin_puk_ind_t;

typedef struct
{
    uint32    pin_operate_result;//ual_tele_sim_pin_req_result_e
    uint32    pin_function_type;    // Verify, change, disable, enable, or unblock. //ual_tele_sim_pin_req_type_e
    uint32    pin_type;            // pin1 or pin2 //ual_tele_sim_pin_indication_e
    ual_tele_sim_pin_status_t        pin_status;            // Number of remaining retries and pin status
    ual_tele_sim_pin_status_t        puk_status;    // Number of remaining retries and puk status
    uint32    sim_id;//ual_tele_sim_id_e
}ual_tele_sim_operate_pin_cnf_t;

/* SIM opn info */
typedef struct
{
    char opn_string[UAL_TELE_SIM_OPN_MAX_LEN+1];
    char spn_string[UAL_TELE_SIM_SPN_MAX_LEN+1];
    boolean is_opn_need;                           //网络名称是否需要显示opn
    boolean is_spn_need;                           //网络名称是否需要显示spn
    boolean is_plmn_display;                       //网络名称是否需要显示plmn
    boolean is_add_ci;                             //网络名称是否需要增加ci(country initials)
    uint32 sim_id;//ual_tele_sim_id_e
}ual_tele_sim_opn_spn_ind_t;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :ual_tele_sim_init
// Parameter: [In] none
//            [Out] none
//            [Return]none
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
void ual_tele_sim_init(void);


/*****************************************************************************/
//  Description :ual_tele_sim_register
// Parameter: [In] p_callback
//            [Out] p_handle
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_register(_ual_cms_client_register_callback p_callback, uint32 *p_handle);

/*****************************************************************************/
//  Description :ual_tele_data_unregister
// Parameter: [In] handle
//            [Out] none
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_unregister(uint32 handle);

/*****************************************************************************/
//  Description :获取SIM 卡状态
// Parameter: [In] sim id
//            [Out] none
//            [Return]ual_tele_sim_status_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_status_e ual_tele_sim_get_sim_status(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
//  Description :获取SIM 卡IMSI 信息
// Parameter: [In] sim id
//            [Out] imsi_str
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_imsi(ual_tele_sim_id_e sim_id,char imsi_str[UAL_TELE_SIM_IMSI_MAX_LEN+1]);

/*****************************************************************************/
//  Description :获取SIM 卡GID 信息
// Parameter: [In] sim id
//            [In] gid_type:gid 类型
//            [Out] gid_str
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:若返回UAL_TELE_SIM_RES_ERROR，代表:
//          1. sim卡未ready时调用
//          2. sim卡GID1服务器未开启
//          3. sim卡GID1文件不存在
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_gid(ual_tele_sim_id_e sim_id,ual_tele_sim_gid_type_e gid_type,char gid_str[UAL_TELE_SIM_GID_MAX_LEN+1]);

/*****************************************************************************/
//  Description :获取SIM 卡SPN 信息
// Parameter: [In] sim id
//            [Out] p_spn
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_spn(ual_tele_sim_id_e sim_id,ual_tele_sim_spn_t* p_spn);

/*****************************************************************************/
//  Description :获取SIM 卡ICCID 信息
// Parameter: [In] sim id
//            [Out] iccid_str
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_iccid(ual_tele_sim_id_e sim_id,char iccid_str[UAL_TELE_SIM_ICCID_MAX_LEN+1]);

/*****************************************************************************/
//  Description :获取sim卡本机号码
// Parameter: [In] sim id
//            [Out] msisdn_str
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_msisdn(ual_tele_sim_id_e sim_id,char msisdn_str[UAL_TELE_SIM_MSISDN_MAX_LEN+1]);

/*****************************************************************************/
//  Description :获取SIM 卡OPN 信息
// Parameter: [In] sim id
//            [Out] opn_str
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_opn(ual_tele_sim_id_e sim_id,char opn_str[UAL_TELE_SIM_OPN_MAX_LEN+1]);

/*****************************************************************************/
//  Description :获取SIM 卡的hplmn 信息
// Parameter: [In] sim id
//            [Out] p_plmn
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_sim_hplmn(ual_tele_sim_id_e sim_id,ual_tele_plmn_info_t* p_hplmn);

/*****************************************************************************/
//  Description :从SIM 卡中读取emerge call相关信息
// Parameter: [In] sim id
//            [Out] p_ecc_info
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_get_ecc_info(ual_tele_sim_id_e sim_id, ual_tele_sim_ecc_info_t* p_ecc_info);

/*****************************************************************************/
//  Description :解锁SIM 卡PIN 码
// Parameter: [In] sim id
//            [In] pin_type
//            [In] pin_str
//            [Out] none
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note: 返回值仅为接口调用返回值。
//            若pin解锁失败，会继续上报MSG_UAL_TELE_SIM_PIN_PUK_IND消息
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_unlock_pin(
                                        ual_tele_sim_id_e sim_id,
                                        ual_tele_sim_pin_indication_e pin_type,
                                        char pin_str[UAL_TELE_SIM_PIN_PUK_MAX_LEN+1]);

/*****************************************************************************/
//  Description :输入PUK 码解锁SIM 卡并设置新的PIN 码
// Parameter: [In] sim id
//            [In] pin_type
//            [In] puk_str:输入的pin码
//            [In]pin_str:设置的新pin码
//            [Out] none
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note: 返回值仅为接口调用返回值。
//            若puk解锁失败，会继续上报MSG_UAL_TELE_SIM_PIN_PUK_IND消息
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_input_puk_and_new_pin(
                                                    ual_tele_sim_id_e sim_id,
                                                    ual_tele_sim_pin_indication_e pin_type,
                                                    const char puk_str[UAL_TELE_SIM_PIN_PUK_MAX_LEN+1],
                                                    const char pin_str[UAL_TELE_SIM_PIN_PUK_MAX_LEN+1]);

/*****************************************************************************/
//  Description :pin 相关操作
// Parameter: [In] sim id
//            [In] operate_type:pin操作类型
//            [In] pin_type:pin类型
//            [In] pin_str
//            [In] exist_pin_str
//            [Out] none
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:调用该接口后，pin处理结果会通过异步消息MSG_UAL_TELE_SIM_PIN_FUNC_CNF返回。
//           pin操作请求类型为change/unblock时，exist_pin_str不能为空。
//           change请求时，exist_pin_str代表新pin码。unblock请求时，exist_pin_str代表puk码。
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_operate_pin (
                                ual_tele_sim_id_e sim_id,
                                ual_tele_sim_pin_req_type_e operate_type,
                                ual_tele_sim_pin_indication_e  pin_type,
                                char pin_str[UAL_TELE_SIM_PIN_PUK_MAX_LEN+1],
                                char exist_pin_str[UAL_TELE_SIM_PIN_PUK_MAX_LEN+1]);

/*****************************************************************************/
//  Description :SIM卡上电
// Parameter: [In] sim id
//            [In] is_init: 是否初始化电话本和短信。
//            [Out] none
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_power_on(ual_tele_sim_id_e sim_id,boolean is_init);

/*****************************************************************************/
//  Description :SIM卡下电
// Parameter: [In] sim id
//            [Out] none
//            [Return]ual_tele_sim_result_e
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
ual_tele_sim_result_e ual_tele_sim_power_off(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
//  Description :SIM卡是否为USIM卡
// Parameter: [In] sim id
//            [Out] none
//            [Return]boolean
//  Author:Jichuan.Zhang
//  Note:
/*****************************************************************************/
boolean ual_tele_sim_is_usim(ual_tele_sim_id_e sim_id);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _UAL_TELE_SIM_H_

