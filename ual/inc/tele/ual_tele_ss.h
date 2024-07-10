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
//�궨��
#define UAL_TELE_SS_MAX_ADDR_BCD_LEN         (20)
#define UAL_TELE_SS_MAX_STR_LEN              (255)
#define UAL_TELE_SS_CALLBARRING_PWD_LEN (4)   //the max length of the password
#define UAL_TELE_SS_CALLFORWARD_NUM_MAX_LEN (40) //����ת�ƺ������󳤶�


//ss ���ص�errorֵ
typedef enum
{
    UAL_TELE_SS_RES_SUCCESS = UAL_TELE_RES_SUCCESS,   //�ɹ�
    UAL_TELE_SS_RES_FAIL = UAL_MODULE_TELE_SS <<16,  //ʧ��
    UAL_TELE_SS_RES_PARAM_ERROR,                     //��������
    UAL_TELE_SS_RES_NETWORK_ERROR,                   //�������
    UAL_TELE_SS_RES_SIM_NOT_READY,                   //SIM ��δ���ڴ���״̬
    UAL_TELE_SS_RES_IS_BUSY,                         //���ڽ���SSҵ��
    UAL_TELE_SS_RES_ERROR_MAX
}ual_tele_ss_result_e;

//�¼�����
typedef enum
{
    MSG_UAL_TELE_SS_CALLFORWARD_REQUEST_CNF = UAL_MODULE_TELE_SS <<16, //�������ת����������Ϣ�ϱ�
    MSG_UAL_TELE_SS_CALLFORWARD_QUERY_CNF,       //�������ת�Ʋ�ѯ�����Ϣ�ϱ�
    MSG_UAL_TELE_SS_CALLWAITING_REQUEST_CNF,    //������еȴ���������Ϣ�ϱ�
    MSG_UAL_TELE_SS_CALLWAITING_QUERY_CNF,      //������еȴ���ѯ�����Ϣ�ϱ�
    MSG_UAL_TELE_SS_CALLBARRING_REQUEST_CNF,    //�������������������Ϣ�ϱ�
    MSG_UAL_TELE_SS_CALLBARRING_QUERY_CNF,      //����������Ʋ�ѯ�����Ϣ�ϱ�
    MSG_UAL_TELE_SS_CLIR_REQUEST_CNF,           //�������ر���������������Ϣ�ϱ�
    MSG_UAL_TELE_SS_CLIR_QUERY_CNF,             //�������ر��������ѯ�����Ϣ�ϱ�
    MSG_UAL_TELE_SS_CLIP_REQUEST_CNF,           //�������ضԷ�������������Ϣ�ϱ�
    MSG_UAL_TELE_SS_CLIP_QUERY_CNF,             //�������ضԷ������ѯ�����Ϣ�ϱ�
    MSG_UAL_TELE_SS_OTHERS_REQUEST_CNF,         //��������������������Ϣ�ϱ���ͨ�����뷢������ʱ�ϱ�
    MSG_UAL_TELE_SS_OTHERS_QUERY_CNF,           //�����������Ͳ�ѯ�����Ϣ�ϱ���ͨ�����뷢������ʱ�ϱ�
    MSG_UAL_TELE_SS_REGISTER_PASSWORD_REQUEST_CNF,//�����޸�������������Ϣ�ϱ�
    MSG_UAL_TELE_SS_RELEASE_REQUEST_CNF,        //����Release��������Ϣ�ϱ�
    MSG_UAL_TELE_SS_MAX
} ual_tele_ss_msg_e;

//SSҵ�������
typedef enum
{
    UAL_TELE_SS_TYPE_CALLFORWARD,   //����ת��
    UAL_TELE_SS_TYPE_CALLWAITING,   //���еȴ�
    UAL_TELE_SS_TYPE_CALLBARRING,   //��������
    UAL_TELE_SS_TYPE_CLIR,          //���ر�������
    UAL_TELE_SS_TYPE_CLIP,          //���ضԷ�����
    UAL_TELE_SS_TYPE_MAX
}ual_tele_ss_type_e;

//SSҵ��Ĳ������ͣ�����ע�ᣬ���������ȥ�����ѯ��ע�������
typedef enum
{
    UAL_TELE_SS_OPER_TYPE_REGISTER,           //ע��
    UAL_TELE_SS_OPER_TYPE_ERASE,              //����
    UAL_TELE_SS_OPER_TYPE_ACTIVATE,           //����
    UAL_TELE_SS_OPER_TYPE_DEACTIVATE,         //ȥ����
    UAL_TELE_SS_OPER_TYPE_QUERY,              //��ѯ
    UAL_TELE_SS_OPER_TYPE_REGISTER_PASSWORD,  //ע������
    UAL_TELE_SS_OPER_TYPE_MAX
}ual_tele_ss_oper_type_e;

//����ת�Ƶ�����
typedef enum
{
    UAL_TELE_SS_CALLFORWARD_TYPE_ALL,               //���к���ת��
    UAL_TELE_SS_CALLFORWARD_TYPE_UNCONDITIONAL,     //����������ת��
    UAL_TELE_SS_CALLFORWARD_TYPE_ALL_CONDITIONAL,   //������������ת��
    UAL_TELE_SS_CALLFORWARD_TYPE_WHEN_BUSY,         //��æ����ת��
    UAL_TELE_SS_CALLFORWARD_TYPE_NO_ANSWER,         //��Ӧ�����ת��
    UAL_TELE_SS_CALLFORWARD_TYPE_NOT_REACHABLE      //�޷������ƶ��û��ϵĺ���ת��
}ual_tele_ss_callforward_type_e;

//�������Ƶ�����
typedef enum
{
    UAL_TELE_SS_CALLBARRING_TYPE_ALL_BAR_SERVICE,       //���к�������
    UAL_TELE_SS_CALLBARRING_TYPE_MO_CALLS,              //��ֹ����ҵ��
    UAL_TELE_SS_CALLBARRING_TYPE_ALL_MO_CALLS,          //��ֹ���к�������
    UAL_TELE_SS_CALLBARRING_TYPE_MO_INT_CALLS,          //��ֹ���й��ʺ�������
    UAL_TELE_SS_CALLBARRING_TYPE_MO_CALLS_INT_ROAM,     //��ֹ���г�����PLMN������Ĺ��ʺ�������
    UAL_TELE_SS_CALLBARRING_TYPE_MT_CALLS,              //��ֹ����ҵ��
    UAL_TELE_SS_CALLBARRING_TYPE_ALL_MT_CALLS,          //��ֹ���к������
    UAL_TELE_SS_CALLBARRING_TYPE_MT_CALLS_ROAM          //�����γ�����PLMN���Һ󣬽�ֹ���к������
}ual_tele_ss_callbarring_type_e;

typedef enum
{
    UAL_TELE_SS_OPER_RES_ACTIVE,             //�ɹ��Ѿ�����
    UAL_TELE_SS_OPER_RES_DEACTIVE,           //�ɹ�δ����
    UAL_TELE_SS_OPER_RES_NOT_PROVITION,      //�ɹ�δ�ṩ
    UAL_TELE_SS_OPER_RES_NOT_REGISTER,       //�ɹ�δע��
    UAL_TELE_SS_OPER_RES_BARRING_SUCCESS,    //�������Ƴɹ�
    UAL_TELE_SS_OPER_RES_PARSE_SUCCESS,      //Ĭ��ֵ���ɹ�
    UAL_TELE_SS_OPER_RES_PDP_ACTIVE_FAIL,    //pdp����ʧ�ܣ�����ssʱֱ�ӷ��ص�error
    UAL_TELE_SS_OPER_RES_FDN_ONLY,           //ֻ�ܲ���̶�����
    UAL_TELE_SS_OPER_RES_NET_NOT_OK,         //���粻��
    UAL_TELE_SS_OPER_RES_PW_ERROR,           //���������������
    UAL_TELE_SS_OPER_RES_PW_INVALID,         //��������������Ч
    UAL_TELE_SS_OPER_RES_ACTIVE_FAIL,        //ע��/����ʧ��
    UAL_TELE_SS_OPER_RES_DEACTIVE_FAIL,      //ȥע��/ȥ����ʧ��
    UAL_TELE_SS_OPER_RES_PARSE_ERROR,        //�ӽ����ײ���Ϣ���ص�error
    UAL_TELE_SS_OPER_RES_PARSE_REJECT,       //�ӽ����ײ���Ϣ���ص�reject
    UAL_TELE_SS_OPER_RES_MAX
}ual_tele_ss_oper_result_e;

typedef struct
{
    uint8   call_number[UAL_TELE_SS_CALLFORWARD_NUM_MAX_LEN + 1];  //�洢�ĺ���
}ual_tele_ss_call_number_t;

typedef struct
{
    uint32        operation_type;    //����ת��ҵ��Ĳ������� //ual_tele_ss_oper_type_e
    uint32        callforward_type;  //����ת�Ƶ����� //ual_tele_ss_callforward_type_e
    ual_tele_ss_call_number_t  forward_num;      //��Ӧ�����ת��ʱ��ת�Ƶ��ĸ�����
    uint8                      no_reply_time;    //��Ӧ�����ת�Ƶ�ʱ��,ֻ����Ӧ�����ת��ʱ����Ҫ����˲���
}ual_tele_callforward_param_info_t;

typedef struct
{
    uint32        operation_type;               //��������ҵ��Ĳ������� //ual_tele_ss_oper_type_e
    uint32        callbar_type;                 //�������Ƶ����� //ual_tele_ss_callbarring_type_e
    uint8         pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1]; //�������Ƶ�����
}ual_tele_callbarring_param_info_t;

typedef struct
{
    uint32        operation_type;    //���еȴ�ҵ��Ĳ������� //ual_tele_ss_oper_type_e
}ual_tele_callwaiting_param_info_t;

typedef struct
{
    uint32        operation_type;    //���ر�������ҵ��Ĳ������� //ual_tele_ss_oper_type_e
}ual_tele_clir_param_info_t;

typedef struct
{
    uint32        operation_type;    //���ضԷ�����ҵ��Ĳ������� //ual_tele_ss_oper_type_e
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

// call barring����
typedef struct
{
    uint8 old_pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1];    //������
    uint8 new_pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1];    //������
    uint8 cnf_pwd[UAL_TELE_SS_CALLBARRING_PWD_LEN + 1];    //ȷ������
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
    ual_tele_ss_number_t dual_sys_forward_num;//����˫������ת�ƣ���ŵ�һ�ŷ������ת�ƵĿ��š�
    uint8                no_reply_time;
}ual_tele_ss_register_param_t;

typedef enum
{
    UAL_TELE_SS_CLIR_NOT_PROVISION,          // δ�ṩCLIR/OIR����
    UAL_TELE_SS_CLIR_PERMANENT,              // ������·��ʾ���ý�ֹ
    UAL_TELE_SS_CLIR_UNKNOWN,                // δ֪
    UAL_TELE_SS_CLIR_TMP_DEFAULT_RESTRICTED, // ������·��ʾ��ʱ��ֹ��ȱʡΪ��ʾ��ֹ
    UAL_TELE_SS_CLIR_TMP_DEFAULT_ALLOWED     // ������·��ʾ��ʱ��ֹ��ȱʡΪ��ʾ����
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

//����ss����Ҫ�����ݽṹ
typedef struct
{

    uint32                           ss_oper;//ssҵ���Ǽ����ѯ������ȥ���� //ual_tele_ss_operation_code_e
    uint32                           ss_code;//ssҵ����cfu����MN_SS_CFNRY  �� //ual_tele_ss_code_e
    ual_tele_ss_basic_service_t      basic_service;//��ѯʱΪall service,����һ��Ϊtelservice
    ual_tele_ss_register_param_t     register_param;//��Ӧ��ת��,���������Ӧ��ʱ��,��ת�ƺ��룬һ�������Ϊ0
    ual_tele_ss_string_t             ss_string;//һ�������Ϊ0����idle���ǽ�ת�ƺ��븳�����ֵ
} ual_tele_ss_parameter_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 callforward_type;  //����ת�Ƶ�����,ual_tele_ss_callforward_type_e
}ual_tele_ss_callforward_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 callforward_type;  //����ת�Ƶ�����,ual_tele_ss_callforward_type_e
    ual_tele_ss_interrogate_info_t ss_info; //����ҵ���ѯ����ṹ
}ual_tele_ss_callforward_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_type;           //����ҵ�������,ual_tele_ss_type_e
}ual_tele_ss_callwaiting_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_type;           //����ҵ�������,ual_tele_ss_type_e
    ual_tele_ss_interrogate_info_t ss_info; //����ҵ���ѯ����ṹ
}ual_tele_ss_callwaiting_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 callbarring_type;  //�������Ƶ�����,ual_tele_ss_callbarring_type_e
}ual_tele_ss_callbarring_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 callbarring_type;  //�������Ƶ�����,ual_tele_ss_callbarring_type_e
    ual_tele_ss_interrogate_info_t ss_info; //����ҵ���ѯ����ṹ
}ual_tele_ss_callbarring_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_type;           //����ҵ�������,ual_tele_ss_type_e
}ual_tele_ss_clir_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_type;           //����ҵ�������,ual_tele_ss_type_e
    ual_tele_ss_interrogate_info_t ss_info; //����ҵ���ѯ����ṹ
}ual_tele_ss_clir_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_type;           //����ҵ�������,ual_tele_ss_type_e
}ual_tele_ss_clip_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_type;           //����ҵ�������,ual_tele_ss_type_e
    ual_tele_ss_interrogate_info_t ss_info; //����ҵ���ѯ����ṹ
}ual_tele_ss_clip_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_code;           //����ҵ�������,ual_tele_ss_code_e
}ual_tele_ss_others_request_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_code;           //����ҵ�������,ual_tele_ss_code_e
    ual_tele_ss_interrogate_info_t ss_info; //����ҵ���ѯ����ṹ
}ual_tele_ss_others_query_cnf_t;

typedef struct
{
    uint32 cur_sim_id;        //��ǰʹ�ÿ���sim id,ual_tele_sim_id_e
    uint32 ss_oper_result;    //����ת������Ľ��,ual_tele_ss_oper_result_e
    uint32 ss_code;           //����ҵ�������,ual_tele_ss_code_e
}ual_tele_ss_register_password_request_cnf_t;

typedef struct
{
    boolean         error_present;  // whether gsm cause present in signal
    uint32          error_code;     // release cause
}ual_tele_ss_release_request_cnf_t;

/*******************************************************************************/
//  Description: ע�����ual tele ssģ��Ļص�����
//  Parameter: [In] ss_callback:callback
//             [Out] p_handle:tele ss handle
//             [Return] ual_tele_ss_result_e :ע����������ķ���ֵ
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_register(_ual_cms_client_register_callback ss_callback, uint32 *p_handle);

/*******************************************************************************/
//  Description: ȥע�����ual tele ssģ�鷵�ص���Ϣ
//  Parameter: [In] handle:ע���������ʱ�õ���ual tele ss handle
//             [Out] None
//             [Return] ual_tele_ss_result_e :ע����������ķ���ֵ
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_unregister(uint32 handle);

/*******************************************************************************/
//  Description: �������ת������
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] callforward_param:����ת��ҵ��Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :����ת������ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_callforward(ual_tele_sim_id_e sim_id, ual_tele_callforward_param_info_t callforward_param);

/*******************************************************************************/
//  Description: ��ѯ����ת����Ϣ
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] callforward_param:����ת��ҵ��Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :����ת�Ʋ�ѯ�ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_callforward_info(ual_tele_sim_id_e sim_id, ual_tele_callforward_param_info_t callforward_param);

/*******************************************************************************/
//  Description: �ر����к���ת��ҵ��
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [Out] None
//             [Return] ual_tele_ss_result_e :�ر����к���ת�Ƶķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_close_all_callforward(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description: ������еȴ�����
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] callforwaiting_param:���еȴ�ҵ��Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :���еȴ�����ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_callwaiting(ual_tele_sim_id_e sim_id, ual_tele_callwaiting_param_info_t callwaiting_param);

/*******************************************************************************/
//  Description: ��ѯ���еȴ���Ϣ
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] callwaiting_param:���еȴ�ҵ��Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :���еȴ���ѯ�ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_callwaiting_info(ual_tele_sim_id_e sim_id, ual_tele_callwaiting_param_info_t callwaiting_param);

/*******************************************************************************/
//  Description: ���������������
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] callbar_param:��������ҵ��Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :������������ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_callbarring(ual_tele_sim_id_e sim_id, ual_tele_callbarring_param_info_t callbarring_param);

/*******************************************************************************/
//  Description: ��ѯ����������Ϣ
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] callbar_param:��������ҵ��Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :�������Ʋ�ѯ�ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_callbarring_info(ual_tele_sim_id_e sim_id, ual_tele_callbarring_param_info_t callbarring_param);

/*******************************************************************************/
//  Description: �ر����к�������ҵ��
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [Out] None
//             [Return] ual_tele_ss_result_e :�ر����к������Ƶķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_close_all_callbarring(ual_tele_sim_id_e sim_id);

/*******************************************************************************/
//  Description: ʵ�����ر�������
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] clir_param:���ر�������Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :���ر�������ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_clir(ual_tele_sim_id_e sim_id, ual_tele_clir_param_info_t clir_param);

/*******************************************************************************/
//  Description: ��ѯ���ر������������Ϣ
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] clir_param:���ر�������Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :���ر���������Ϣ��ѯ�ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_clir_info(ual_tele_sim_id_e sim_id, ual_tele_clir_param_info_t clir_param);

/*******************************************************************************/
//  Description: ʵ�����ضԷ�����
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] clip_param:���ضԷ�����Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :���ضԷ�����ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request_clip(ual_tele_sim_id_e sim_id, ual_tele_clip_param_info_t clip_param);

/*******************************************************************************/
//  Description: ��ѯ���ضԷ����������Ϣ
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] clip_param:���ضԷ�����Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :���ضԷ�������Ϣ��ѯ�ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_query_clip_info(ual_tele_sim_id_e sim_id, ual_tele_clip_param_info_t clip_param);

/*******************************************************************************/
//  Description: �޸ĺ�����������
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] pwd_info:�޸�����ʱ������ľ����롢��������ȷ������
//             [Out] None
//             [Return] ual_tele_ss_result_e :�޸�����ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_change_barring_password(ual_tele_sim_id_e sim_id, ual_tele_ss_pwd_info_t pwd_info);

/*******************************************************************************/
//  Description: ʵ��SSҵ����������ѯ
//  Parameter: [In] sim_id:��ǰʹ�ÿ���sim  id
//             [In] ss_param:SSҵ��Ĳ�����Ϣ
//             [Out] None
//             [Return] ual_tele_ss_result_e :SSҵ��ķ��ؽ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_ss_result_e ual_tele_ss_request(ual_tele_sim_id_e sim_id, ual_tele_ss_parameter_t ss_param);

/*******************************************************************************/
//  Description: ssҵ����й����е�ȡ������
//  Parameter: [In] sim_id
//             [Out] None
//             [Return] None
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
void ual_tele_ss_cancel(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
// Description :��Ӧss PS���ϱ�����Ϣ
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

