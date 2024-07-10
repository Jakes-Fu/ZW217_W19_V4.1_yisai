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

/*******************************************************************************
** File Name:       ual_tele_ss.h                                              *
** Author:          miao.liu2                                                  *
** Date:            05/09/2022                                                 *
** Description:     This file is used to define ual tele ss export api         *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 05/09/2022          miao.liu2                Create                         *
********************************************************************************/
#ifndef _UAL_TELE_SS_H_
#define _UAL_TELE_SS_H_


/**----------------------------------------------------------------------------*
**                         Include Files                                       *
**-----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         MACRO DEFINITION                                    *
**-----------------------------------------------------------------------------*/
//宏定义
#define UAL_TELE_SS_MAX_ADDR_BCD_LEN         (20)
#define UAL_TELE_SS_MAX_STR_LEN              (255)
#define UAL_TELE_SS_CALLBARRING_PWD_LEN (4)   //the max length of the password
#define UAL_TELE_SS_CALLFORWARD_NUM_MAX_LEN (40) //呼叫转移号码的最大长度


//ss 返回的error值
typedef enum
{
    UAL_TELE_SS_RES_SUCCESS = UAL_TELE_RES_SUCCESS,   //成功
    UAL_TELE_SS_RES_FAIL = UAL_MODULE_TELE_SS <<16,  //失败
    UAL_TELE_SS_RES_PARAM_ERROR,                     //参数错误
    UAL_TELE_SS_RES_NETWORK_ERROR,                   //网络错误
    UAL_TELE_SS_RES_SIM_NOT_READY,                   //SIM 卡未处于待机状态
    UAL_TELE_SS_RES_IS_BUSY,                         //正在进行SS业务
    UAL_TELE_SS_RES_ERROR_MAX
}ual_tele_ss_result_e;

//事件定义
typedef enum
{
    MSG_UAL_TELE_SS_CALLFORWARD_REQUEST_CNF = UAL_MODULE_TELE_SS <<16, //发起呼叫转移请求后的消息上报
    MSG_UAL_TELE_SS_CALLFORWARD_QUERY_CNF,       //发起呼叫转移查询后的消息上报
    MSG_UAL_TELE_SS_CALLWAITING_REQUEST_CNF,    //发起呼叫等待请求后的消息上报
    MSG_UAL_TELE_SS_CALLWAITING_QUERY_CNF,      //发起呼叫等待查询后的消息上报
    MSG_UAL_TELE_SS_CALLBARRING_REQUEST_CNF,    //发起呼叫限制请求后的消息上报
    MSG_UAL_TELE_SS_CALLBARRING_QUERY_CNF,      //发起呼叫限制查询后的消息上报
    MSG_UAL_TELE_SS_CLIR_REQUEST_CNF,           //发起隐藏本机号码请求后的消息上报
    MSG_UAL_TELE_SS_CLIR_QUERY_CNF,             //发起隐藏本机号码查询后的消息上报
    MSG_UAL_TELE_SS_CLIP_REQUEST_CNF,           //发起隐藏对方号码请求后的消息上报
    MSG_UAL_TELE_SS_CLIP_QUERY_CNF,             //发起隐藏对方号码查询后的消息上报
    MSG_UAL_TELE_SS_OTHERS_REQUEST_CNF,         //发起其他类型请求后的消息上报，通过暗码发起请求时上报
    MSG_UAL_TELE_SS_OTHERS_QUERY_CNF,           //发起其他类型查询后的消息上报，通过暗码发起请求时上报
    MSG_UAL_TELE_SS_REGISTER_PASSWORD_REQUEST_CNF,//发起修改密码请求后的消息上报
    MSG_UAL_TELE_SS_RELEASE_REQUEST_CNF,        //发起Release请求后的消息上报
    MSG_UAL_TELE_SS_MAX
} ual_tele_ss_msg_e;

//SS业务的类型
typedef enum
{
    UAL_TELE_SS_TYPE_CALLFORWARD,   //呼叫转移
    UAL_TELE_SS_TYPE_CALLWAITING,   //呼叫等待
    UAL_TELE_SS_TYPE_CALLBARRING,   //呼叫限制
    UAL_TELE_SS_TYPE_CLIR,          //隐藏本机号码
    UAL_TELE_SS_TYPE_CLIP,          //隐藏对方号码
    UAL_TELE_SS_TYPE_MAX
}ual_tele_ss_type_e;

//SS业务的操作类型，包括注册，擦除，激活，去激活，查询，注册密码等
typedef enum
{
    UAL_TELE_SS_OPER_TYPE_REGISTER,           //注册
    UAL_TELE_SS_OPER_TYPE_ERASE,              //擦除
    UAL_TELE_SS_OPER_TYPE_ACTIVATE,           //激活
    UAL_TELE_SS_OPER_TYPE_DEACTIVATE,         //去激活
    UAL_TELE_SS_OPER_TYPE_QUERY,              //查询
    UAL_TELE_SS_OPER_TYPE_REGISTER_PASSWORD,  //注册密码
    UAL_TELE_SS_OPER_TYPE_MAX
}ual_tele_ss_oper_type_e;

//呼叫转移的类型
typedef enum
{
    UAL_TELE_SS_CALLFORWARD_TYPE_ALL,               //所有呼叫转移
    UAL_TELE_SS_CALLFORWARD_TYPE_UNCONDITIONAL,     //无条件呼叫转移
    UAL_TELE_SS_CALLFORWARD_TYPE_ALL_CONDITIONAL,   //所有条件呼叫转移
    UAL_TELE_SS_CALLFORWARD_TYPE_WHEN_BUSY,         //遇忙呼叫转移
    UAL_TELE_SS_CALLFORWARD_TYPE_NO_ANSWER,         //无应答呼叫转移
    UAL_TELE_SS_CALLFORWARD_TYPE_NOT_REACHABLE      //无法到达移动用户上的呼叫转移
}ual_tele_ss_callforward_type_e;

//呼叫限制的类型
typedef enum
{
    UAL_TELE_SS_CALLBARRING_TYPE_ALL_BAR_SERVICE,       //所有呼叫限制
    UAL_TELE_SS_CALLBARRING_TYPE_MO_CALLS,              //禁止呼出业务
    UAL_TELE_SS_CALLBARRING_TYPE_ALL_MO_CALLS,          //禁止所有呼出呼叫
    UAL_TELE_SS_CALLBARRING_TYPE_MO_INT_CALLS,          //禁止所有国际呼出呼叫
    UAL_TELE_SS_CALLBARRING_TYPE_MO_CALLS_INT_ROAM,     //禁止所有除归属PLMN国家外的国际呼出呼叫
    UAL_TELE_SS_CALLBARRING_TYPE_MT_CALLS,              //禁止呼入业务
    UAL_TELE_SS_CALLBARRING_TYPE_ALL_MT_CALLS,          //禁止所有呼入呼叫
    UAL_TELE_SS_CALLBARRING_TYPE_MT_CALLS_ROAM          //当漫游除归属PLMN国家后，禁止所有呼入呼叫
}ual_tele_ss_callbarring_type_e;

typedef enum
{
    UAL_TELE_SS_OPER_RES_ACTIVE,             //成功已经激活
    UAL_TELE_SS_OPER_RES_DEACTIVE,           //成功未激活
    UAL_TELE_SS_OPER_RES_NOT_PROVITION,      //成功未提供
    UAL_TELE_SS_OPER_RES_NOT_REGISTER,       //成功未注册
    UAL_TELE_SS_OPER_RES_BARRING_SUCCESS,    //呼叫限制成功
    UAL_TELE_SS_OPER_RES_PARSE_SUCCESS,      //默认值，成功
    UAL_TELE_SS_OPER_RES_PDP_ACTIVE_FAIL,    //pdp激活失败，申请ss时直接返回的error
    UAL_TELE_SS_OPER_RES_FDN_ONLY,           //只能拨打固定拨号
    UAL_TELE_SS_OPER_RES_NET_NOT_OK,         //网络不好
    UAL_TELE_SS_OPER_RES_PW_ERROR,           //呼叫限制密码错误
    UAL_TELE_SS_OPER_RES_PW_INVALID,         //呼叫限制密码无效
    UAL_TELE_SS_OPER_RES_ACTIVE_FAIL,        //注册/激活失败
    UAL_TELE_SS_OPER_RES_DEACTIVE_FAIL,      //去注册/去激活失败
    UAL_TELE_SS_OPER_RES_PARSE_ERROR,        //从解析底层消息返回的error
    UAL_TELE_SS_OPER_RES_PARSE_REJECT,       //从解析底层消息返回的reject
    UAL_TELE_SS_OPER_RES_MAX
}ual_tele_ss_oper_result_e;

typedef struct
{
    uint8   call_number[UAL_TELE_SS_CALLFORWARD_NUM_MAX_LEN + 1];  //存储的号码
}ual_tele_ss_call_number_t;

typedef struct
{
    uint32        operation_type;    //呼叫转移业务的操作类型 //ual_tele_ss_oper_type_e
    uint32        callforward_type;  //呼叫转移的类型 //ual_tele_ss_callforward_type_e
    ual_tele_ss_call_number_t  forward_num;      //无应答呼叫转移时，转移到哪个号码
    uint8                      no_reply_time;    //无应答呼叫转移的时间,只有无应答呼叫转移时，需要传入此参数
}ual_tele_callforward_param_info_t;

typedef struct
{
    uint32        operation_type;               //呼叫限制业务的操作类型 //ual_tele_ss_oper_type_e
    uint32        callbar_type;                 //呼叫限制的类型 //ual_tele_ss_callbarring_type_e
    uint8         pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1]; //呼叫限制的密码
}ual_tele_callbarring_param_info_t;

typedef struct
{
    uint32        operation_type;    //呼叫等待业务的操作类型 //ual_tele_ss_oper_type_e
}ual_tele_callwaiting_param_info_t;

typedef struct
{
    uint32        operation_type;    //隐藏本机号码业务的操作类型 //ual_tele_ss_oper_type_e
}ual_tele_clir_param_info_t;

typedef struct
{
    uint32        operation_type;    //隐藏对方号码业务的操作类型 //ual_tele_ss_oper_type_e
}ual_tele_clip_param_info_t;


typedef enum
{
    UAL_TELE_SS_OPERATION_CODE_REGISTER              = 10,
    UAL_TELE_SS_OPERATION_CODE_ERASE                 = 11,
    UAL_TELE_SS_OPERATION_CODE_ACTIVATE              = 12,
    UAL_TELE_SS_OPERATION_CODE_DEACTIVATE            = 13,
    UAL_TELE_SS_OPERATION_CODE_INTERROGATE           = 14,
    UAL_TELE_SS_OPERATION_CODE_NOTIFY                = 16,
    UAL_TELE_SS_OPERATION_CODE_REGISTERPASSWORD      = 17,
    UAL_TELE_SS_OPERATION_CODE_GETPASSWORD           = 18,
    UAL_TELE_SS_OPERATION_CODE_PROCUNSTRUCTSS_DATA   = 19,
    UAL_TELE_SS_OPERATION_CODE_FORWORKCHECK_IND      = 38,
    UAL_TELE_SS_OPERATION_CODE_PROCUNSTRUCTSS_REQ    = 59,
    UAL_TELE_SS_OPERATION_CODE_UNSTRUCTSS_REQ        = 60,
    UAL_TELE_SS_OPERATION_CODE_UNSTRUCTSS_NOTIFY     = 61,
    UAL_TELE_SS_OPERATION_CODE_ERASE_CCENTRY         = 77,
    UAL_TELE_SS_OPERATION_CODE_CALL_DEFLECTION       = 117,
    UAL_TELE_SS_OPERATION_CODE_USER_USERSERVICE      = 118,
    UAL_TELE_SS_OPERATION_CODE_ACCESS_REGIST_CCENTRY = 119,
    UAL_TELE_SS_OPERATION_CODE_FORWARDCUG_INFO       = 120,
    UAL_TELE_SS_OPERATION_CODE_SPLIT_MPTY            = 121,
    UAL_TELE_SS_OPERATION_CODE_RETRIEVE_MPTY         = 122,
    UAL_TELE_SS_OPERATION_CODE_HOLD_MPTY             = 123,
    UAL_TELE_SS_OPERATION_CODE_BUILD_MPTY            = 124,
    UAL_TELE_SS_OPERATION_CODE_FORWARD_CHARGE_ADVICE = 125,
    UAL_TELE_SS_OPERATION_CODE_EXPLICIT_CT           = 126,
    UAL_TELE_SS_OPERATION_CODE_LCS_LOC_NOTIFY        = 116,
    UAL_TELE_SS_OPERATION_CODE_LCS_MOLR              = 115
} ual_tele_ss_operation_code_e;

typedef enum
{
    UAL_TELE_SS_SERVICE_ALL_TELESERVICES              = 0,
    UAL_TELE_SS_SERVICE_ALL_SPEECH_TRANS_SERVICES     = 16,
    UAL_TELE_SS_SERVICE_TELEPHONE                     = 17,
    UAL_TELE_SS_SERVICE_EMERGENCYCALL                 = 18,
    UAL_TELE_SS_SERVICE_ALL_DATADCS_SERVICES          = 24, /*servic code of vt in interrogat cnf*/

    UAL_TELE_SS_SERVICE_ALL_SHORT_MSG_SERVICE         = 32,
    UAL_TELE_SS_SERVICE_SHORT_MSG_MTPP                = 33,
    UAL_TELE_SS_SERVICE_SHORT_MSG_MOPP                = 34,

    UAL_TELE_SS_SERVICE_ALL_FACSIMIL_TRANS_SERVICES   = 96,
    UAL_TELE_SS_SERVICE_FACSIMIL3_ALTERNATE_SPEECH    = 97,
    UAL_TELE_SS_SERVICE_AUTO_FACSIMIL3                = 98,
    UAL_TELE_SS_SERVICE_FACSIMIL4                     = 99,

    UAL_TELE_SS_SERVICE_ALL_DATA_TELESERVICES         = 112,
    UAL_TELE_SS_SERVICE_TELESERVICES_EXP_SMS          = 128,

    UAL_TELE_SS_SERVICE_AUXILIARY_SPEECH              = 208,
    UAL_TELE_SS_SERVICE_AUXILIARY_TELEPHONY           = 209
}ual_tele_ss_service_code_e;


typedef struct
{
    uint8    len;
    uint32   ton;
    uint32   npi;
    uint8    ss_ptr[UAL_TELE_SS_MAX_STR_LEN + 1];
}ual_tele_ss_string_t;

// call barring密码
typedef struct
{
    uint8 old_pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1];    //旧密码
    uint8 new_pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1];    //新密码
    uint8 cnf_pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1];    //确认密码
}ual_tele_ss_pwd_info_t;

typedef enum
{
    UAL_TELE_SS_ALL_SERVICE,
    UAL_TELE_SS_BEARER_SERVICE,
    UAL_TELE_SS_TELE_SERVICE
} ual_tele_ss_basic_service_e;

typedef struct
{

    uint32  service_type;   //bearerService or teleService //ual_ss_basic_service_e
    uint16  service_code;   // service code is bearerService code, while service_type = BEARER_SERVICE,ual_tele_ss_service_code_e
} ual_tele_ss_basic_service_t;

typedef struct
{
    uint32          number_type;
    uint32          number_plan;
    uint16          num_len;
    uint8           party_num[UAL_TELE_SS_MAX_ADDR_BCD_LEN];
} ual_tele_ss_number_t;

typedef struct
{
    ual_tele_ss_number_t forward_num;
    ual_tele_ss_number_t cfnry_num;
    ual_tele_ss_number_t dual_sys_forward_num;//用于双卡呼叫转移，存放第一张发起呼叫转移的卡号。
    uint8                no_reply_time;
}ual_tele_ss_register_param_t;

typedef enum
{
    UAL_TELE_SS_CLIR_NOT_PROVISION,          // 未提供CLIR/OIR服务
    UAL_TELE_SS_CLIR_PERMANENT,              // 主叫线路显示永久禁止
    UAL_TELE_SS_CLIR_UNKNOWN,                // 未知
    UAL_TELE_SS_CLIR_TMP_DEFAULT_RESTRICTED, // 主叫线路显示暂时禁止，缺省为显示禁止
    UAL_TELE_SS_CLIR_TMP_DEFAULT_ALLOWED     // 主叫线路显示暂时禁止，缺省为显示允许
}ual_tele_ss_clir_info_e;

typedef struct
{
    uint16                      ss_status;
    boolean                     ss_status_present;
    boolean                     forward_num_present;
    boolean                     no_reply_time_present;
    boolean                     clir_info_present;
    uint8                       no_reply_time;
    ual_tele_ss_number_t        forward_num;
    ual_tele_ss_basic_service_t basic_service;
    ual_tele_ss_clir_info_e     clir_info;
}ual_tele_ss_interrogate_info_t;


typedef enum
{
    UAL_TELE_SS_CODE_CLIP                          = 17,   // calling line identification presentation
    UAL_TELE_SS_CODE_CLIR                          = 18,   // calling line identification restriction
    UAL_TELE_SS_CODE_COLP                          = 19,   // connected line identification presentation
    UAL_TELE_SS_CODE_COLR                          = 20,   // connected line identification restriction
    UAL_TELE_SS_CODE_CNAP                          = 25,   // calling name presentation

    /*SS-Codes '00011010' to '00011111' are reserved for furture: name identification Supplementary Service*/
    UAL_TELE_SS_CODE_ALL_FORWARD                   = 32,   // all call forwarding ss
    UAL_TELE_SS_CODE_CFU                           = 33,   // call forwarding unconditional
    UAL_TELE_SS_CODE_CD                            = 36,   // communication deflection
    UAL_TELE_SS_CODE_CFNL                          = 37,   // communication forwarding on not logged-in
    UAL_TELE_SS_CODE_ALL_COND_FORWARD              = 40,   // all conditional forwarding SS
    UAL_TELE_SS_CODE_CFB                           = 41,   // call forwarding on mobile subscriber busy
    UAL_TELE_SS_CODE_CFNRY                         = 42,   // call forwarding on no reply
    UAL_TELE_SS_CODE_CFNRC                         = 43,   // call forwarding on mobile subscriber not reachable

    UAL_TELE_SS_CODE_CW                            = 65,   // call waiting
    UAL_TELE_SS_CODE_HOLD                          = 66,   // call hold
    UAL_TELE_SS_CODE_CCBS                          = 67,   // completion of called busy subscriber
    UAL_TELE_SS_CODE_MC                            = 69,   // multicall

    UAL_TELE_SS_CODE_MULTIPTY                      = 81,   // multiparty

    UAL_TELE_SS_CODE_CUG                           = 97,   // closed user group

    UAL_TELE_SS_CODE_AOCI                          = 113,  // advice of charge information
    UAL_TELE_SS_CODE_AOCC                          = 114,  // advice of charge charging

    UAL_TELE_SS_CODE_ALL_BAR_SERVICE               = 144,  // all barring service
    UAL_TELE_SS_CODE_BAR_OUTGOING                  = 145,  // barring of outgoing calls
    UAL_TELE_SS_CODE_BAOC                          = 146,  // barring of all outgoing calls
    UAL_TELE_SS_CODE_BOIC                          = 147,  // barring of outgoing international calls
    UAL_TELE_SS_CODE_BOIC_EXHC                     = 148,  // barring of outgoing international calls except those directed to the home PLMN
    UAL_TELE_SS_CODE_BAR_INCOMING                  = 153,  // barring of incoming calls
    UAL_TELE_SS_CODE_BAIC                          = 154,  // barring of all incoming calls
    UAL_TELE_SS_CODE_BICROAM                       = 155,  // barring of incoming calls when roaming outside home PLMN Country
    UAL_TELE_SS_CODE_ACR                           = 156,  /* Anonymous Communication Rejection bug645204*/
    UAL_TELE_SS_CODE_ALL_MOLR                      = 192,  // all Mobile Originating Location Request Classes
    UAL_TELE_SS_CODE_BASIC_SELF_LOC                = 193,  // allow an MS to request its own location
    UAL_TELE_SS_CODE_AUTO_SELF_LOC                 = 194,  // allow an MS to perform self location without interaction with the PLMN for a predetermined period of time
    UAL_TELE_SS_CODE_LOC_TRANS_TO_3RDPARTY         = 195   // allow an MS to request transfer of its location to another LCS client
} ual_tele_ss_code_e;

//发起ss所需要的数据结构
typedef struct
{

    uint32                           ss_oper;//ss业务是激活，查询，还是去激活 //ual_tele_ss_operation_code_e
    uint32                           ss_code;//ss业务是cfu还是MN_SS_CFNRY  等 //ual_tele_ss_code_e
    ual_tele_ss_basic_service_t      basic_service;//查询时为all service,其他一般为telservice
    ual_tele_ss_register_param_t     register_param;//无应答转移,里面可以无应答时间,及转移号码，一般情况下为0
    ual_tele_ss_string_t             ss_string;//一般情况下为0，在idle下是将转移号码赋予这个值
} ual_tele_ss_parameter_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 callforward_type;  //呼叫转移的类型,ual_tele_ss_callforward_type_e
}ual_tele_ss_callforward_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 callforward_type;  //呼叫转移的类型,ual_tele_ss_callforward_type_e
    ual_tele_ss_interrogate_info_t ss_info; //附加业务查询结果结构
}ual_tele_ss_callforward_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_type;           //补充业务的类型,ual_tele_ss_type_e
}ual_tele_ss_callwaiting_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_type;           //补充业务的类型,ual_tele_ss_type_e
    ual_tele_ss_interrogate_info_t ss_info; //附加业务查询结果结构
}ual_tele_ss_callwaiting_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 callbarring_type;  //呼叫限制的类型,ual_tele_ss_callbarring_type_e
}ual_tele_ss_callbarring_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 callbarring_type;  //呼叫限制的类型,ual_tele_ss_callbarring_type_e
    ual_tele_ss_interrogate_info_t ss_info; //附加业务查询结果结构
}ual_tele_ss_callbarring_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_type;           //补充业务的类型,ual_tele_ss_type_e
}ual_tele_ss_clir_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_type;           //补充业务的类型,ual_tele_ss_type_e
    ual_tele_ss_interrogate_info_t ss_info; //附加业务查询结果结构
}ual_tele_ss_clir_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_type;           //补充业务的类型,ual_tele_ss_type_e
}ual_tele_ss_clip_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_type;           //补充业务的类型,ual_tele_ss_type_e
    ual_tele_ss_interrogate_info_t ss_info; //附加业务查询结果结构
}ual_tele_ss_clip_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_code;           //补充业务的类型,ual_tele_ss_code_e
}ual_tele_ss_others_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_code;           //补充业务的类型,ual_tele_ss_code_e
    ual_tele_ss_interrogate_info_t ss_info; //附加业务查询结果结构
}ual_tele_ss_others_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //当前使用卡的sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //呼叫转移请求的结果,ual_tele_ss_oper_result_e
    uint32 ss_code;           //补充业务的类型,ual_tele_ss_code_e
}ual_tele_ss_register_password_request_cnf_t;

typedef struct
{
    boolean         error_present;  // whether gsm cause present in signal
    uint32          error_code;     // release cause
}ual_tele_ss_release_request_cnf_t;

/*******************************************************************************/
//  Description: 注册监听ual tele ss模块的回调函数
//  Parameter: [In] ss_callback:callback
//             [Out] p_handle:tele ss handle
//             [Return] ual_tele_ss_result_e :注册监听函数的返回值
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_register(_ual_cms_client_register_callback ss_callback, uint32 *p_handle);

/*******************************************************************************/
//  Description: 去注册监听ual tele ss模块返回的消息
//  Parameter: [In] handle:注册监听函数时得到的ual tele ss handle
//             [Out] None
//             [Return] ual_tele_ss_result_e :注册监听函数的返回值
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_unregister(uint32 handle);

/*******************************************************************************/
//  Description: 发起呼叫转移请求
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] callforward_param:呼叫转移业务的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :呼叫转移请求的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_callforward(ual_tele_sim_id_e sim_id, ual_tele_callforward_param_info_t callforward_param);

/*******************************************************************************/
//  Description: 查询呼叫转移信息
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] callforward_param:呼叫转移业务的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :呼叫转移查询的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_callforward_info(ual_tele_sim_id_e sim_id, ual_tele_callforward_param_info_t callforward_param);

/*******************************************************************************/
//  Description: 关闭所有呼叫转移业务
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [Out] None
//             [Return] ual_tele_ss_result_e :关闭所有呼叫转移的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_close_all_callforward(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description: 发起呼叫等待请求
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] callforwaiting_param:呼叫等待业务的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :呼叫等待请求的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_callwaiting(ual_tele_sim_id_e sim_id, ual_tele_callwaiting_param_info_t callwaiting_param);

/*******************************************************************************/
//  Description: 查询呼叫等待信息
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] callwaiting_param:呼叫等待业务的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :呼叫等待查询的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_callwaiting_info(ual_tele_sim_id_e sim_id, ual_tele_callwaiting_param_info_t callwaiting_param);

/*******************************************************************************/
//  Description: 发起呼叫限制请求
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] callbar_param:呼叫限制业务的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :呼叫限制请求的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_callbarring(ual_tele_sim_id_e sim_id, ual_tele_callbarring_param_info_t callbarring_param);

/*******************************************************************************/
//  Description: 查询呼叫限制信息
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] callbar_param:呼叫限制业务的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :呼叫限制查询的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_callbarring_info(ual_tele_sim_id_e sim_id, ual_tele_callbarring_param_info_t callbarring_param);

/*******************************************************************************/
//  Description: 关闭所有呼叫限制业务
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [Out] None
//             [Return] ual_tele_ss_result_e :关闭所有呼叫限制的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_close_all_callbarring(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description: 实现隐藏本机号码
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] clir_param:隐藏本机号码的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :隐藏本机号码的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_clir(ual_tele_sim_id_e sim_id, ual_tele_clir_param_info_t clir_param);

/*******************************************************************************/
//  Description: 查询隐藏本机号码相关信息
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] clir_param:隐藏本机号码的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :隐藏本机号码信息查询的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_clir_info(ual_tele_sim_id_e sim_id, ual_tele_clir_param_info_t clir_param);

/*******************************************************************************/
//  Description: 实现隐藏对方号码
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] clip_param:隐藏对方号码的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :隐藏对方号码的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_clip(ual_tele_sim_id_e sim_id, ual_tele_clip_param_info_t clip_param);

/*******************************************************************************/
//  Description: 查询隐藏对方号码相关信息
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] clip_param:隐藏对方号码的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :隐藏对方号码信息查询的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_clip_info(ual_tele_sim_id_e sim_id, ual_tele_clip_param_info_t clip_param);

/*******************************************************************************/
//  Description: 修改呼叫限制密码
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] pwd_info:修改密码时，输入的旧密码、新密码与确认密码
//             [Out] None
//             [Return] ual_tele_ss_result_e :修改密码的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_change_barring_password(ual_tele_sim_id_e sim_id, ual_tele_ss_pwd_info_t pwd_info);

/*******************************************************************************/
//  Description: 实现SS业务的请求与查询
//  Parameter: [In] sim_id:当前使用卡的sim  id
//             [In] ss_param:SS业务的参数信息
//             [Out] None
//             [Return] ual_tele_ss_result_e :SS业务的返回结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request(ual_tele_sim_id_e sim_id, ual_tele_ss_parameter_t ss_param);

/*******************************************************************************/
//  Description: ss业务进行过程中的取消操作
//  Parameter: [In] sim_id
//             [Out] None
//             [Return] None
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
void ual_tele_ss_cancel(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
// Description :响应ss PS层上报的消息
// Parameter: [In] app_ptr
//            [In] msg_id
//            [In] param
//            [Out] none
//            [Return]none
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
ual_tele_ss_result_e handleTeleSsPsMsg(uint16 msg_id, void *p_param);

#endif

