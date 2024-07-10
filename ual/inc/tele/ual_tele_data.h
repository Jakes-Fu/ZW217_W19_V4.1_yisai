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
#define UAL_TELE_DATA_CARRIER_APN_MAX_LEN         0x20   //��������󳤶�
#define UAL_TELE_DATA_CARRIER_USERNAME_MAX_LEN    0x20   //�û�������󳤶�
#define UAL_TELE_DATA_CARRIER_PASSWORD_MAX_LEN    0x20   //�������󳤶�
#define UAL_TELE_DATA_IPV6_LEN                    16     //IPV6��ַ�ĳ���
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
    MSG_UAL_TELE_DATA_ACTIVE_CNF = UAL_MODULE_TELE_DATA << 16,      //client���󼤻�
    MSG_UAL_TELE_DATA_DEACTIVE_CNF,    //client����ȥ����
    MSG_UAL_TELE_DATA_DEACTIVE_IND,    //�����ϱ�ȥ����
    MSG_UAL_TELE_DATA_END,
}ual_tele_data_msg_e;

typedef enum
{
    UAL_TELE_DATA_ROAMING_DISABLE,     //������������
    UAL_TELE_DATA_ROAMING_ENABLE,      //������������
    UAL_TELE_DATA_ROAMING_MAX
}ual_tele_data_roaming_mode_e;

typedef enum
{
    UAL_TELE_DATA_AUTH_TYPE_PAP = 0,           //PAP����
    UAL_TELE_DATA_AUTH_TYPE_CHAP = 1,          //CHAP����
    UAL_TELE_DATA_AUTH_TYPE_PAP_CHAP = 2,      //CHAP��PAP����
    UAL_TELE_DATA_AUTH_TYPE_NONE = 3,          //����֤����
    UAL_TELE_DATA_AUTH_TYPE_MAX
}ual_tele_data_auth_type_e;

typedef enum
{
    UAL_TELE_DATA_IP_V4,             //Ipv4
    UAL_TELE_DATA_IP_V6,             //Ipv6
    UAL_TELE_DATA_IP_V4V6,           //Ipv4��Ipv6
    UAL_TELE_DATA_IP_MAX
}ual_tele_data_ip_type_e;

typedef struct
{
    char  apn[UAL_TELE_DATA_CARRIER_APN_MAX_LEN + 1];            //���������
    char  username[UAL_TELE_DATA_CARRIER_USERNAME_MAX_LEN + 1];	 //�û���
    char  password[UAL_TELE_DATA_CARRIER_PASSWORD_MAX_LEN + 1];  //����
    uint32 auth_type;                         //��֤���� //ual_tele_data_auth_type_e
    uint32 ip_type;                             //ip type //ual_tele_data_ip_type_e
}ual_tele_data_apn_info_t;

typedef struct
{
    boolean is_ip4;                                //�Ƿ�֧��Ipv4
    boolean is_ip6;                                //�Ƿ�֧��Ipv6
    uint32  ipaddr;                                //Ipv4��ַ
    uint32  snmask;                                //Ipv4��������
    uint32  dns1;                                  //Ipv4����������1
    uint32  dns2;                                  //Ipv4����������2
    uint32  gateway;                               //Ipv4���ص�ַ
    uint8   ip6addr[UAL_TELE_DATA_IPV6_LEN];       //Ipv6��ַ
    uint8   dns6_pri[UAL_TELE_DATA_IPV6_LEN];      //Ipv6����������1
    uint8   dns6_sec[UAL_TELE_DATA_IPV6_LEN];      //Ipv6����������2
    uint32  net_id;                                //nsapi,�ṩ��DAPS
}ual_tele_data_net_para_t;

typedef enum
{
    UAL_TELE_DATA_ERR_SUCCESS = 0,     //�����ɹ�
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
    UAL_TELE_DATA_ERR_START                            =   0xff,      //���ض����ʧ��ԭ�����ʼֵ 218583 newms
    UAL_TELE_DATA_ERR_PRIMITIVE,       //�²㴫��������Ϣ���ʹ���
    UAL_TELE_DATA_ERR_MEM_ALLOC,       //�����ڴ治�ɹ�
    UAL_TELE_DATA_ERR_NO_NSAPI,        //NSAPI�Ѿ�����
    UAL_TELE_DATA_ERR_SENDER,          //
    UAL_TELE_DATA_ERR_PDP_TYPE,        //PDP���Ͳ�֧��
    UAL_TELE_DATA_ERR_ATC_PARAM,       //���ú���ʱ����Ĳ�������
    UAL_TELE_DATA_ERR_PDP_ID,          //��ʱδ��
    UAL_TELE_DATA_ERR_SPACE_NOT_ENOUGH,//��ʱδ��
    UAL_TELE_DATA_ERR_ACTIVE_REJCET,   //PDP�����ļ���ܾ�
    UAL_TELE_DATA_ERR_SAME_PDP_CONTEXT,//PDP�������Ѿ���������ټ���
    UAL_TELE_DATA_ERR_NSAPI,           //���NSAPI����ʹ��
    UAL_TELE_DATA_ERR_MODIFY_REJ,      //�޸�PDP�����ĵ���������˾ܾ�
    UAL_TELE_DATA_ERR_READ_TYPE,       //û��ָ�����͵���Ϣ
    UAL_TELE_DATA_ERR_PDP_CONTEXT_ACTIVATED,   //PDP�������Ѿ��������ɾ��
    UAL_TELE_DATA_ERR_NO_PDP_CONTEXT,           // û���ҵ�ָ����PDP������.
    UAL_TELE_DATA_ERR_PERMENANT_PROBLEM ,   //�������⣬MMIӦ��ʾ�û����һ��ʱ�����ԡ�
    UAL_TELE_DATA_ERR_TEMPORARILY_BLOCKED,       //�ײ����ڴ����������̣�MMI���Լ���϶�ʱ�����Զ�Ρ�
    UAL_DATA_RES_RETRYING,                  //��ʱ���⣬MMI���Լ��һ��ʱ�����ԣ�����1~3�Ρ�
    UAL_TELE_DATA_ERR_UNKNOWN_ERROR,             //δ֪����
    UAL_TELE_DATA_ERR_SERVICE_OPTION_NOT_SUPPORTED,          //����ѡ�֧��
    UAL_TELE_DATA_ERR_REQUEST_SERVICE_OPTION_NOT_SUBSCRIBED, //û��ָ������ѡ��
    UAL_TELE_DATA_ERR_SERVICE_OPTION_OUTOF_ORDER,            //����ѡ��˳�򲻶�
    UAL_TELE_DATA_ERR_PDP_AUTHENTICATION_FAILED,             //����ʧ��
    UAL_TELE_DATA_ERR_MISSING_OR_UNKOWN_APN,                 //ȱ�ٻ�ܹ�ʶ��apn
    UAL_TELE_DATA_ERR_OPERATION_NOT_ALLOWED,
    UAL_TELE_DATA_ERR_THE_LAST_DEFAULT_CONTEXT,               // in lte rat, the last default context can't be deactivated
    UAL_TELE_DATA_ERR_NO_SATISFIED_RESOURCE,

    //ual pdp add new result
    UAL_TELE_DATA_ERR_PS_ACTIVE_FAIL,
#ifndef GPRSMPDP_SUPPORT
    UAL_TELE_DATA_ERR_AT_IS_ON, //����ʹ��AT����
#endif
    UAL_TELE_DATA_ERR_DIFFER_DUALSYS,
    UAL_TELE_DATA_ERR_TIMEOUT,
    UAL_TELE_DATA_ERR_NOT_PERMIT, //������ʹ�����磬�������ģʽ
#ifdef DATA_ROAMING_SUPPORT
    UAL_TELE_DATA_ERR_NOT_PERMIT_ROAMING, //����ģʽ�ر�����(added by feng.xiao)
#endif
    UAL_TELE_DATA_ERR_FDN_NOT_PERMIT, //������ʹ�����磬FDN only
}ual_tele_data_err_e;

typedef struct
{
    uint32                         sim_id;//ual_tele_sim_id_e
    uint32                         result;//SPRDЭ��ջ�ο�ual_tele_data_err_e��RDAЭ��ջ�ο�MMIPDP_RESULT_E
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
//  Description : �򿪷�������ҵ��
//  Parameter: [In] sim_id sim��ID
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note: ����������ݶ˵�pdp����
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_active(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description : �رշ�������ҵ��
//  Parameter: [In] sim_id sim��ID
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note: ȥ����������ݶ˵�pdp����
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_deactive(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description : ��������ҵ���Ƿ񼤻�
//  Parameter: [In] sim_id sim��ID
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note:
/*******************************************************************************/
boolean ual_tele_data_get_is_active(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description : �л�Ϊ�û���ѡ��sim��id
//  Parameter: [In] sim_id
//             [Out] none
//             [Return]ual_tele_sim_id_e
//  Author: shuting.ma
//  Note:
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_set_current_data_card(ual_tele_sim_id_e  sim_id);

/*******************************************************************************/
//  Description : ��ȡ��ǰʹ��
//  Parameter: [In] none
//             [Out] none
//             [Return]ual_tele_sim_id_e
//  Author: shuting.ma
//  Note:
/*******************************************************************************/
ual_tele_sim_id_e ual_tele_data_get_current_data_card(void);

/*******************************************************************************/
//  Description : ����internet����APN���������APN���͵�pdp����
//  Parameter: [In] sim_id: sim��ID
//                  p_apn_info: ���º��apn��Ϣ
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author: shuting.ma
//  Note:�ýӿڽ���internet����apn��Ϣ�и���ʱ����
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_connect_apn(ual_tele_sim_id_e sim_id, ual_tele_data_apn_info_t *p_new_apn_info);

/*******************************************************************************/
//  Description :��ȡ��������ģʽ��ֻ�п�����ģʽ�ſ��Լ���PDP
//  Parameter: [In] sim_id: sim��ID
//             [Out] none
//             [Return] data_roaming_mode: roaming status(enable/disable)
//  Author:shuting.ma
//  Note:�ýӿ���radio������ģʽ��ͬ
/*******************************************************************************/
ual_tele_data_roaming_mode_e ual_tele_data_get_roaming_mode(ual_tele_sim_id_e sim_id);
/*******************************************************************************/
//  Description :������������ģʽ��ֻ�п�����ģʽ�ſ��Լ���PDP
//  Parameter: [In] sim_id: sim��ID
//                  data_roaming_mode: roaming status(enable/disable)
//             [Out] none
//             [Return]ual_tele_data_result_e
//  Author:shuting.ma
//  Note:�ýӿ���radio������ģʽ��ͬ
/*******************************************************************************/
ual_tele_data_result_e ual_tele_data_set_roaming_mode(ual_tele_sim_id_e sim_id, ual_tele_data_roaming_mode_e data_roaming_mode);

/*****************************************************************************/
//  Description :ual tele data inject function for virtual card
//  Parameter: [In] p_virtual_netcard_callback
//             [Out] ��
//             [Return]ual_tele_data_result_e
//  Author:shuting.ma
//  Note:
/*****************************************************************************/
ual_tele_data_result_e ual_tele_data_inject_function(ual_tele_data_injection_func p_virtual_netcard_callback);

/*****************************************************************************/
//  Description :ual_tele_data_uninject_function_for_virtual_card
//  Parameter: [In] ��
//             [Out] ��
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
//  Parameter: [In] ��
//             [Out] ��
//             [Return]ual_tele_data_result_e
//  Author:shuting.ma
//  Note:
/*****************************************************************************/
ual_tele_data_result_e ual_tele_data_init(void);

#endif
