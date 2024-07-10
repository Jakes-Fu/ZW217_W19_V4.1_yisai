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
** File Name:       ual_tele_data.h                                            *
** Author:            shuting.ma                                               *
** Date:             11/08/2022                                                *
** Description:    This file is used to define ual data export api             *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 11/08/2022           shuting.ma              Create                         *
********************************************************************************/
#ifndef _UAL_TELE_DATA_H_
#define _UAL_TELE_DATA_H_

//
#include "ual_tele_common.h"

/**----------------------------------------------------------------------------*
 **                         Include Files                                      *
 **----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                   *
 **----------------------------------------------------------------------------*/
#define UAL_TELE_DATA_CARRIER_APN_MAX_LEN         0x20   //接入点的最大长度
#define UAL_TELE_DATA_CARRIER_USERNAME_MAX_LEN    0x20   //用户名的最大长度
#define UAL_TELE_DATA_CARRIER_PASSWORD_MAX_LEN    0x20   //密码的最大长度
#define UAL_TELE_DATA_IPV6_LEN                    16     //IPV6地址的长度
/**----------------------------------------------------------------------------*
**                         TYPE DEFINITION                                     *
**-----------------------------------------------------------------------------*/
typedef enum
{
    UAL_TELE_DATA_RES_SUCCESS = UAL_TELE_RES_SUCCESS,
    UAL_TELE_DATA_RES_ERROR = UAL_MODULE_TELE_DATA <<16,
    UAL_TELE_DATA_RES_PARAM_ERROR,
    UAL_TELE_DATA_RES_MAX
}ual_tele_data_result_e;

typedef enum
{
    MSG_UAL_TELE_DATA_ACTIVE_CNF = UAL_MODULE_TELE_DATA << 16,      //client请求激活
    MSG_UAL_TELE_DATA_DEACTIVE_CNF,    //client请求去激活
    MSG_UAL_TELE_DATA_DEACTIVE_IND,    //主动上报去激活
    MSG_UAL_TELE_DATA_END,
}ual_tele_data_msg_e;

typedef enum
{
    UAL_TELE_DATA_ROAMING_DISABLE,     //禁用数据漫游
    UAL_TELE_DATA_ROAMING_ENABLE,      //启用数据漫游
    UAL_TELE_DATA_ROAMING_MAX
}ual_tele_data_roaming_mode_e;

typedef enum
{
    UAL_TELE_DATA_AUTH_TYPE_PAP = 0,           //PAP类型
    UAL_TELE_DATA_AUTH_TYPE_CHAP = 1,          //CHAP类型
    UAL_TELE_DATA_AUTH_TYPE_PAP_CHAP = 2,      //CHAP或PAP类型
    UAL_TELE_DATA_AUTH_TYPE_NONE = 3,          //无认证类型
    UAL_TELE_DATA_AUTH_TYPE_MAX
}ual_tele_data_auth_type_e;

typedef enum
{
    UAL_TELE_DATA_IP_V4,             //Ipv4
    UAL_TELE_DATA_IP_V6,             //Ipv6
    UAL_TELE_DATA_IP_V4V6,           //Ipv4或Ipv6
    UAL_TELE_DATA_IP_MAX
}ual_tele_data_ip_type_e;

typedef struct
{
    char  apn[UAL_TELE_DATA_CARRIER_APN_MAX_LEN + 1];            //接入点名称
    char  username[UAL_TELE_DATA_CARRIER_USERNAME_MAX_LEN + 1];	 //用户名
    char  password[UAL_TELE_DATA_CARRIER_PASSWORD_MAX_LEN + 1];  //密码
    uint32 auth_type;                         //认证类型 //ual_tele_data_auth_type_e
    uint32 ip_type;                             //ip type //ual_tele_data_ip_type_e
}ual_tele_data_apn_info_t;

typedef struct
{
    boolean is_ip4;                                //是否支持Ipv4
    boolean is_ip6;                                //是否支持Ipv6
    uint32  ipaddr;                                //Ipv4地址
    uint32  snmask;                                //Ipv4子网掩码
    uint32  dns1;                                  //Ipv4域名服务器1
    uint32  dns2;                                  //Ipv4域名服务器2
    uint32  gateway;                               //Ipv4网关地址
    uint8   ip6addr[UAL_TELE_DATA_IPV6_LEN];       //Ipv6地址
    uint8   dns6_pri[UAL_TELE_DATA_IPV6_LEN];      //Ipv6域名服务器1
    uint8   dns6_sec[UAL_TELE_DATA_IPV6_LEN];      //Ipv6域名服务器2
    uint32  net_id;                                //nsapi,提供给DAPS
}ual_tele_data_net_para_t;

typedef enum
{
    UAL_TELE_DATA_ERR_SUCCESS = 0,     //操作成功
    /* +network err, values from 0x08 to 0x70, the same as "context_cause_enum"*/
    UAL_TELE_DATA_ERR_OPERATOR_DETERMINE_BAR           =   0x08,
    UAL_TELE_DATA_ERR_LLC_SND_FAILURE                  =   0x19,
    UAL_TELE_DATA_ERR_INSUFF_RESOURCE                  =   0x1A,
    UAL_TELE_DATA_ERR_UNKNOWN_APN                      =   0x1B,
    UAL_TELE_DATA_ERR_UNKNOWN_PDP_ADDR_OR_TYPE         =   0x1C,
    UAL_TELE_DATA_ERR_AUTHENTICATION_FAILURE           =   0x1D,
    UAL_TELE_DATA_ERR_ACTIVATION_REJ_GGSN              =   0x1E,
    UAL_TELE_DATA_ERR_ACTIVATION_REJ                   =   0x1F,
    UAL_TELE_DATA_ERR_UNSUPPORTED_SERVICE_OPTION       =   0x20,
    UAL_TELE_DATA_ERR_UNSUBSCRIBED_SERVICE_OPTION      =   0x21,
    UAL_TELE_DATA_ERR_OUT_OF_ORDER_SERVICE_OPTION      =   0x22,
    UAL_TELE_DATA_ERR_NSAPI_ALREADY_USED               =   0x23,
    UAL_TELE_DATA_ERR_REGULAR_DEACTIVATION             =   0x24,
    UAL_TELE_DATA_ERR_QOS_NOT_ACCEPTED                 =   0x25,
    UAL_TELE_DATA_ERR_NETWORK_FAIL                     =   0x26,
    UAL_TELE_DATA_ERR_REACTIVATION_REQD                =   0x27,
    UAL_TELE_DATA_ERR_FEATURE_NOT_SUPPORTED            =   0x28,
    UAL_TELE_DATA_ERR_EMANTIC_ERROR_IN_TFT_OP          =   0x29,
    UAL_TELE_DATA_ERR_YNTACTICAL_ERROR_IN_TFT_OP       =   0x2A,
    UAL_TELE_DATA_ERR_UNKNOWN_PDP_CONTEXT              =   0x2B,
    UAL_TELE_DATA_ERR_SEMANTIC_ERROR_IN_PACKET_FILTER  =   0x2C,
    UAL_TELE_DATA_ERR_SYNTAX_ERROR_IN_PACKET_FILTER    =   0x2D,
    UAL_TELE_DATA_ERR_PDP_CONTEXT_WO_TFT_ALREADY_ACT   =   0x2E,
    UAL_TELE_DATA_ERR_SM_ERR_UNSPECIFIED               =   0x2F,
    UAL_TELE_DATA_ERR_PDN_type_IPv4_only_allowed       =   0x32,
    UAL_TELE_DATA_ERR_PDN_type_IPv6_only_allowed       =   0x33,
    UAL_TELE_DATA_ERR_ESM_information_not_received     =   0x35,
    UAL_TELE_DATA_ERR_PDN_connection_does_not_exist    =   0x36,
    UAL_TELE_DATA_ERR_multiple_PDN_connections_for_a_given_APN_not_allowed   =   0x37,
    UAL_TELE_DATA_ERR_maximum_number_of_EPS_bearers_reached                  =   0x41,
    UAL_TELE_DATA_ERR_requested_APN_not_supported_in_current_RAT_and_PLMN_combination  =   0x42,
    UAL_TELE_DATA_ERR_COLLISION_WITH_NW_INI_REQUEST    =   0x38,/* bug541193 */
    UAL_TELE_DATA_ERR_INVALID_TI                       =   0x51,
    UAL_TELE_DATA_ERR_INCORRECT_MSG                    =   0x5F,
    UAL_TELE_DATA_ERR_INVALID_MAND_INFO                =   0x60,
    UAL_TELE_DATA_ERR_UNIMPLE_MSG_TYPE                 =   0x61,
    UAL_TELE_DATA_ERR_INCOMPAT_MSG_TYP_PROTO_STAT      =   0x62,
    UAL_TELE_DATA_ERR_UNIMPLE_IE                       =   0x63,
    UAL_TELE_DATA_ERR_CONTEXT_CAUSE_CONDITIONAL_IE_ERROR  =   0x64,
    UAL_TELE_DATA_ERR_INCOMP_MSG_PROTO_STAT            =   0x65,
    UAL_TELE_DATA_ERR_UNSPECIFIED                      =   0x6F,
    UAL_TELE_DATA_ERR_STARTUP_FAILURE                  =   0x70,
    UAL_TELE_DATA_ERR_DEACT_DEDI_BRR_BY_NW             =   0x90,/*bug734145 k*/
    UAL_TELE_DATA_ERR_NW_RE_ACTIVATE_DEDICATED_BEARER = 0X94,/*24.301 6.4.2.5 b)*/

    /* CDMA error cause start */
    UAL_TELE_DATA_ERR_GENERAL_ERROR=0XA0,  /*NETWORK GENERATES ERROR CODES*/
    UAL_TELE_DATA_ERR_PGW_REJECT=0XA1,  /*PGW reject, ---*/
    UAL_TELE_DATA_ERR_RESOURCE_UNAVAILABLE=0XA2,	/*Resource Unavailable, Retry---*/
    UAL_TELE_DATA_ERR_RECONNECT_NOT_ALLOWED=0XA3,  /*APN isn't allowed to reconnect*/
    UAL_TELE_DATA_ERR_EAPAKA_FAILURE=0XA4,  /*EAP,AKA Failure, No Need to retry */
    UAL_TELE_DATA_ERR_NETWORK_NO_RSP=0XA5,  /*PKT Module has already retry, Network no response ---*/
    UAL_TELE_DATA_ERR_PDN_ATTACH_ABORT=0XA6,	/*Retry Count exceed ---*/
    UAL_TELE_DATA_ERR_INVALID_PDN_ATTACH_REQ=0XA7,  /*PDN id invalid, No Need to retry*/
    UAL_TELE_DATA_ERR_PDN_REC_FAILURE=0XA8,  /*parameters error, No Need to retry*/
    UAL_TELE_DATA_ERR_MAIN_CONN_SETUP_FAILURE=0XA9,  /*PKT Module has already retry, ---*/
    UAL_TELE_DATA_ERR_BEARER_RESOURCE_UNAVAILABLE=0XAA,  /*Resource Unavailable, Need Retry ---*/
    UAL_TELE_DATA_ERR_EAPAKA_REJECT=0XAB,  /*No Need to retry*/
    UAL_TELE_DATA_ERR_LCP_NEGO3_FAILURE=0XAC,  /*PKT Module has already retry --*/
    UAL_TELE_DATA_ERR_TCH_SETUP_FAILURE=0XAD,  /*PKT Module has already retry--*/
    UAL_TELE_DATA_ERR_NW_NO_RSP_IN_LCP=0XAE,	/*PKT Module has already retry--*/
    UAL_TELE_DATA_ERR_NW_NO_RSP_INAUTH=0XAF,	/*PKT Module has already retry--*/
    UAL_TELE_DATA_ERR_PDN_TIMEOUT=0XB0,  /*Retry--*/
    UAL_TELE_DATA_ERR_DEFCONT_FAILURE=0XB1,  /* Add for PKT check param error */
    UAL_TELE_DATA_ERR_DETACHED=0XB2,	/*detach*/
    UAL_TELE_DATA_ERR_INTERNAL_FAILURE=0XB3,	/*internal error*/
    UAL_TELE_DATA_ERR_LOCAL_DETACH_DUE_TO_IRAT=0XB4,	/*detach due to irat*/
    UAL_TELE_DATA_ERR_UNAUTHORIZED_APN=0XB5,	/*Not Use*/
    UAL_TELE_DATA_ERR_PDN_LIMIT_EXCEEDED=0XB6,  /*Cid already in activate request state*/
    UAL_TELE_DATA_ERR_NO_PGW_AVAILABLE=0XB7,	/*Not Use*/
    UAL_TELE_DATA_ERR_PGW_UNREACHABLE=0XB8,  /*Not Use*/
    UAL_TELE_DATA_ERR_INSUFFICIENT_PARAMETERS=0XB9,  /*Not Use, insufficient parameters*/
    UAL_TELE_DATA_ERR_ADMIN_PROHIBITED=0XBA,	/*Not Use*/
    UAL_TELE_DATA_ERR_PDNID_ALREADY_INUSE=0XBB,  /*Not Use*/
    UAL_TELE_DATA_ERR_SUBSCRIPTION_LIMITATION=0XBC,  /*Not Use*/
    UAL_TELE_DATA_ERR_PDNCONN_ALREADY_EXIST_FORPDN=0XBD,	/*Not Use*/
    UAL_TELE_DATA_ERR_EMERGENCY_NOT_SUPPORTED=0XBE,  /*Not Use*/
    UAL_TELE_DATA_ERR_RETRY_TMR_THROTTLING=0XBF,	/*Retry Throttle, No Need to retry*/
    UAL_TELE_DATA_ERR_PDN_LIMIT_EXCEEDED_INUESIDE=0XC0,  /*Exceed max num in UE side*/
    UAL_TELE_DATA_ERR_PDNID_ALREADY_INUSE_INUESIDE=0XC1,	/*PDN id already in use*/
    UAL_TELE_DATA_ERR_OP_ABORT_BY_USER=0XC2,	/*abort by user, No Need to retry*/
    UAL_TELE_DATA_ERR_RTT_DATA_CONNECTED=0XC3,  /*No Use*/
    UAL_TELE_DATA_ERR_ALREADY_IN_REQUEST_STATE=0XC4,	/*already in request state*/
    UAL_TELE_DATA_ERR_POWER_DOWN=0XC5,  /*power down*/
    /* End of CDMA Error */

    /* -network error */
    UAL_TELE_DATA_ERR_START                            =   0xff,      //本地定义的失败原因的起始值 218583 newms
    UAL_TELE_DATA_ERR_PRIMITIVE,       //下层传上来的消息类型错误
    UAL_TELE_DATA_ERR_MEM_ALLOC,       //申请内存不成功
    UAL_TELE_DATA_ERR_NO_NSAPI,        //NSAPI已经用完
    UAL_TELE_DATA_ERR_SENDER,          //
    UAL_TELE_DATA_ERR_PDP_TYPE,        //PDP类型不支持
    UAL_TELE_DATA_ERR_ATC_PARAM,       //调用函数时传入的参数不对
    UAL_TELE_DATA_ERR_PDP_ID,          //暂时未用
    UAL_TELE_DATA_ERR_SPACE_NOT_ENOUGH,//暂时未用
    UAL_TELE_DATA_ERR_ACTIVE_REJCET,   //PDP上下文激活被拒绝
    UAL_TELE_DATA_ERR_SAME_PDP_CONTEXT,//PDP上下文已经激活，不需再激活
    UAL_TELE_DATA_ERR_NSAPI,           //这个NSAPI正在使用
    UAL_TELE_DATA_ERR_MODIFY_REJ,      //修改PDP上下文的请求被网络端拒绝
    UAL_TELE_DATA_ERR_READ_TYPE,       //没有指定类型的信息
    UAL_TELE_DATA_ERR_PDP_CONTEXT_ACTIVATED,   //PDP上下文已经激活，不能删除
    UAL_TELE_DATA_ERR_NO_PDP_CONTEXT,           // 没有找到指定的PDP上下文.
    UAL_TELE_DATA_ERR_PERMENANT_PROBLEM ,   //持续问题，MMI应提示用户间隔一段时间再试。
    UAL_TELE_DATA_ERR_TEMPORARILY_BLOCKED,       //底层正在处理信令流程，MMI可以间隔较短时间重试多次。
    UAL_DATA_RES_RETRYING,                  //临时问题，MMI可以间隔一段时间再试，重试1~3次。
    UAL_TELE_DATA_ERR_UNKNOWN_ERROR,             //未知错误。
    UAL_TELE_DATA_ERR_SERVICE_OPTION_NOT_SUPPORTED,          //服务选项不支持
    UAL_TELE_DATA_ERR_REQUEST_SERVICE_OPTION_NOT_SUBSCRIBED, //没有指定服务选项
    UAL_TELE_DATA_ERR_SERVICE_OPTION_OUTOF_ORDER,            //服务选项顺序不对
    UAL_TELE_DATA_ERR_PDP_AUTHENTICATION_FAILED,             //鉴定失败
    UAL_TELE_DATA_ERR_MISSING_OR_UNKOWN_APN,                 //缺少活不能够识别apn
    UAL_TELE_DATA_ERR_OPERATION_NOT_ALLOWED,
    UAL_TELE_DATA_ERR_THE_LAST_DEFAULT_CONTEXT,               // in lte rat, the last default context can't be deactivated
    UAL_TELE_DATA_ERR_NO_SATISFIED_RESOURCE,

    //ual pdp add new result
    UAL_TELE_DATA_ERR_PS_ACTIVE_FAIL,
#ifndef GPRSMPDP_SUPPORT
    UAL_TELE_DATA_ERR_AT_IS_ON, //正在使用AT拨号
#endif
    UAL_TELE_DATA_ERR_DIFFER_DUALSYS,
    UAL_TELE_DATA_ERR_TIMEOUT,
    UAL_TELE_DATA_ERR_NOT_PERMIT, //不允许使用网络，比如飞行模式
#ifdef DATA_ROAMING_SUPPORT
    UAL_TELE_DATA_ERR_NOT_PERMIT_ROAMING, //漫游模式关闭网络(added by feng.xiao)
#endif
    UAL_TELE_DATA_ERR_FDN_NOT_PERMIT, //不允许使用网络，FDN only
}ual_tele_data_err_e;

typedef struct
{
    uint32                         sim_id;//ual_tele_sim_id_e
    uint32                         result;//SPRD协议栈参考ual_tele_data_err_e；RDA协议栈参考MMIPDP_RESULT_E
    ual_tele_data_net_para_t       net_para;
}ual_tele_data_active_cnf_ex_t;

typedef struct
{
    uint32   sim_id;//ual_tele_sim_id_e
    uint32   result;//ual_tele_data_err_e
}ual_tele_data_deactive_cnf_t;


typedef struct
{
    uint32   sim_id;//ual_tele_sim_id_e
    uint32   result;//ual_tele_data_err_e
}ual_tele_data_deactive_ind_t;

typedef boolean (*ual_tele_data_injection_func)(uint32 module_id, uint32 event_id, void* p_param, uint32 param_size);

/**----------------------------------------------------------------------------*
**                         MACRO DEFINITION                                    *
**-----------------------------------------------------------------------------*/

/*******************************************************************************/
//	Description: ual_tele_data_register
//	Parameter: [In] p_callback private call back
//	           [Out] p_handle out of phandle
//	           [Return] ual_tele_data_result_e
//	Author: shuting.ma
//	Note:
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_register(_ual_cms_client_register_callback p_callback, uint32 *p_handle);

/*******************************************************************************/
//	Description: ual_tele_data_unregister
//	Parameter: [in] handle
//	           [Out] none
//	           [Return] ual_tele_data_result_e
//	Author: shuting.ma
//	Note:
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_unregister(uint32 handle);

/*******************************************************************************/
//  Description : 打开蜂窝数据业务
//  Parameter: [In] sim_id sim卡ID
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note: 激活蜂窝数据端的pdp链接
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_active(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description : 关闭蜂窝数据业务
//  Parameter: [In] sim_id sim卡ID
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note: 去激活蜂窝数据端的pdp链接
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_deactive(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description : 蜂窝数据业务是否激活
//  Parameter: [In] sim_id sim卡ID
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note:
/*******************************************************************************/
boolean ual_tele_data_get_is_active(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description : 切换为用户所选的sim卡id
//  Parameter: [In] sim_id
//             [Out] none
//             [Return]ual_tele_sim_id_e
//  Author: shuting.ma
//  Note:
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_set_current_data_card(ual_tele_sim_id_e  sim_id);

/*******************************************************************************/
//  Description : 获取当前使用
//  Parameter: [In] none
//             [Out] none
//             [Return]ual_tele_sim_id_e
//  Author: shuting.ma
//  Note:
/*******************************************************************************/
ual_tele_sim_id_e ual_tele_data_get_current_data_card(void);

/*******************************************************************************/
//  Description : 设置internet类型APN，并激活该APN类型的pdp链接
//  Parameter: [In] sim_id: sim卡ID
//                  p_apn_info: 更新后的apn信息
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note:该接口仅在internet类型apn信息有更新时调用
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_connect_apn(ual_tele_sim_id_e sim_id, ual_tele_data_apn_info_t *p_new_apn_info);

/*******************************************************************************/
//  Description :获取数据漫游模式，只有开启改模式才可以激活PDP
//  Parameter: [In] sim_id: sim卡ID
//             [Out] none
//             [Return] data_roaming_mode: roaming status(enable/disable)
//  Author:shuting.ma
//  Note:该接口与radio中漫游模式不同
/*******************************************************************************/
ual_tele_data_roaming_mode_e ual_tele_data_get_roaming_mode(ual_tele_sim_id_e sim_id);
/*******************************************************************************/
//  Description :设置数据漫游模式，只有开启改模式才可以激活PDP
//  Parameter: [In] sim_id: sim卡ID
//                  data_roaming_mode: roaming status(enable/disable)
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author:shuting.ma
//  Note:该接口与radio中漫游模式不同
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_set_roaming_mode(ual_tele_sim_id_e sim_id, ual_tele_data_roaming_mode_e data_roaming_mode);

/*****************************************************************************/
//  Description :ual tele data inject function for virtual card
//  Parameter: [In] p_virtual_netcard_callback
//             [Out] 无
//             [Return]ual_tele_data_result_e
//  Author:shuting.ma
//  Note:
/*****************************************************************************/
ual_tele_data_result_e ual_tele_data_inject_function(ual_tele_data_injection_func p_virtual_netcard_callback);

/*****************************************************************************/
//  Description :ual_tele_data_uninject_function_for_virtual_card
//  Parameter: [In] 无
//             [Out] 无
//             [Return]ual_tele_data_result_e
//  Author:shuting.ma
//  Note:
/*****************************************************************************/
ual_tele_data_result_e ual_tele_data_uninject_function(void);

/*****************************************************************************/
//  Description:ual_tele_data_get_netid
//  Parameter: [In] none
//             [Out] none
//             [Return] net id
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
uint32 ual_tele_data_get_netid(void);

/*****************************************************************************/
//  Description :init tele data, create fsm
//  Parameter: [In] 无
//             [Out] 无
//             [Return]ual_tele_data_result_e
//  Author:shuting.ma
//  Note:
/*****************************************************************************/
ual_tele_data_result_e ual_tele_data_init(void);

#endif
