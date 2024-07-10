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
** File Name:       ual_tele_call.h                                            *
** Author:          donglin.su                                                 *
** Date:             18/08/2022                                                *
** Description:    This file is used to define ual tele export api             *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 18/08/2022          donglin.su               Create                         *
********************************************************************************/
#ifndef _UAL_TELE_CALL_H_
#define _UAL_TELE_CALL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UAL_TELE_CALL_TELE_NUM_MAX        40
#define UAL_TELE_CALL_TELE_NAME_MAX_LEN   40
#define UAL_TELE_CALL_ECC_MAX_COUNT       (20)
#define UAL_TELE_CALL_ECC_NUM_MAX_LEN     (10)

#define UAL_TELE_CALL_COMMON_BASE_ATTR \
                uint32              sim_id; \
                uint8               call_id; \
                char                tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1]; \
                uint8               tele_num_len; \
                boolean             is_ps_call;


/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
/*                          ENUM AND STRUCT                                  *
/*---------------------------------------------------------------------------*/
typedef enum
{
    UAL_TELE_CALL_RES_SUCCESS = UAL_TELE_RES_SUCCESS,
    UAL_TELE_CALL_RES_ERROR = UAL_MODULE_TELE_CALL <<16,
    UAL_TELE_CALL_RES_ERR_NUM_NULL, //����Ϊ��
    UAL_TELE_CALL_RES_ERR_NUM_INVALID, //�����ʽ����
    UAL_TELE_CALL_RES_ERR_SIM_ID, //SIM ID error
    UAL_TELE_CALL_RES_ERR_CRSS, //CRSS��������
    UAL_TELE_CALL_RES_ERR_BUSY, //MO/MT/������
    UAL_TELE_CALL_RES_ERR_FLY_MODE, //����ģʽ����
    UAL_TELE_CALL_RES_ERR_ECC_IN_FLY_MODE, //���������ҷ���ģʽ����
    UAL_TELE_CALL_RES_ERR_NETWORK_LIMIT, //����״̬Ϊ�޷����ע����
    UAL_TELE_CALL_RES_ERR_ECC_ONLY, //���޽�������
    UAL_TELE_CALL_RES_ERR_SIM_NOT_READY, //SIM״̬�쳣
    UAL_TELE_CALL_RES_ERR_CALL_FULL, //������Ŀ�ﵽ����
    UAL_TELE_CALL_RES_ERR_IMS_NOT_READY, //IMSû�гɹ�ע��
    UAL_TELE_CALL_RES_ERR_ECC_NOT_SUPPORT,//��֧�ֽ�������
    UAL_TELE_CALL_RES_MAX
}ual_tele_call_result_e;

//���嵥ͨͨ����״̬
typedef enum
{
    UAL_TELE_CALL_STATE_NULL,       //����״̬
    UAL_TELE_CALL_STATE_OUTGOING,   //���ں���
    UAL_TELE_CALL_STATE_INCOMING,   //������
    UAL_TELE_CALL_STATE_ACTIVE,     //ͨ����
    UAL_TELE_CALL_STATE_HOLD,       //������
    UAL_TELE_CALL_STATE_MAX
} ual_tele_call_state_e;

//�¼�����
typedef enum
{
    MSG_UAL_TELE_CALL_MO_START_IND = UAL_MODULE_TELE_CALL << 16,     //Э��ջ�Ѿ�������з�������
    MSG_UAL_TELE_CALL_MT_START_IND,     //�������յ����罨������
    MSG_UAL_TELE_CALL_MO_ALERT_IND,     //Զ���Ѿ���������׶�
    MSG_UAL_TELE_CALL_MO_CONNECTED_CNF, //ȥ�罨���ɹ�
    MSG_UAL_TELE_CALL_MT_CONNECTED_CNF, //���罨���ɹ�
    MSG_UAL_TELE_CALL_DISCONNECTED_CNF, //ͨ�������Ҷϳɹ�
    MSG_UAL_TELE_CALL_DISCONNECTED_IND, //ͨ�������Ҷϳɹ����Է��Ҷϻ�����ԭ��Ͽ�
    MSG_UAL_TELE_CALL_HOLD_CNF,         //ͨ������
    MSG_UAL_TELE_CALL_UNHOLD_CNF,       //ͨ���ָ�
    MSG_UAL_TELE_CALL_AMR_TYPE_IND,     //�Ƿ���ʾHD
    MSG_UAL_TELE_CALL_HO_PROGRESS_IND,  //����֪ͨ
    MSG_UAL_TELE_CALL_QUERY_DTMF_IND,   //ͨ������������Ҫѯ���Ƿ���DTMF�ַ��������ʹ��¼�ѯ��
    MSG_UAL_TELE_CALL_START_DTMF_IND,   //��ʼ����DTMF�ַ���
    MSG_UAL_TELE_CALL_SEND_DTMF_CNF,    //����һ���ַ���/�ַ����
#if defined (BT_HFG_SUPPORT)
    MSG_UAL_TELE_CALL_BT_AUDIO_VOL_UP_CNF,    //ͨ�������У������豸��������
    MSG_UAL_TELE_CALL_BT_AUDIO_VOL_DOWN_CNF,  //ͨ�������У������豸��������
    MSG_UAL_TELE_CALL_BT_AUDIO_OPENED_CNF,    //ͨ�������У������豸��
    MSG_UAL_TELE_CALL_BT_AUDIO_CLOSED_CNF,    //ͨ�������У������豸�ر�
#endif
    MSG_UAL_TELE_CALL_MAX
} ual_tele_call_msg_e;

//�������ʱ�������Ĳ�����Դ
typedef enum
{
    UAL_TELE_CALL_ALERT_TYPE_REMOTE,         //Զ������
    UAL_TELE_CALL_ALERT_TYPE_LOCAL,          //��������
    UAL_TELE_CALL_ALERT_TYPE_MAX
} ual_tele_call_alert_type_e;

//����ͨ��������ԭ��
typedef enum
{
    UAL_TELE_CALL_DISC_CAUSE_ERROR_NONE,
    UAL_TELE_CALL_DISC_CAUSE_NORMAL_CLEARING,//�����Ҷ�
    UAL_TELE_CALL_DISC_CAUSE_USER_BUSY,      //�û���æ
    UAL_TELE_CALL_DISC_CAUSE_OTHER,          //����
    UAL_TELE_CALL_DISC_CAUSE_MAX
} ual_tele_call_disconnect_cause_e;


typedef enum
{
    UAL_TELE_CALL_AUDIO_DEV_HH,        //��Ͳ
    UAL_TELE_CALL_AUDIO_DEV_EP,        //����
    UAL_TELE_CALL_AUDIO_DEV_HF,        //����
    UAL_TELE_CALL_AUDIO_DEV_BT,        //�����첽����ģʽ
    UAL_TELE_CALL_AUDIO_DEV_MAX        //ö�����ֵ
} ual_tele_call_audio_dev_e;

typedef struct
{
    ual_tele_sim_id_e       sim_id; //��ǰ��ʹ��SIM��ID
    boolean                 is_valid;//��·call_info�Ƿ���Ч
    uint8                   call_id; //��ͨCALL��ID
    uint8                   tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1];
    uint8                   tele_num_len;
    boolean                 is_cnap_name_exist;
    uint16                  cnap_name[UAL_TELE_CALL_TELE_NUM_MAX + 1];
    uint8                   cnap_name_len;
    ual_tele_call_state_e   call_state;   //��ͨCALL��״̬
    uint32                  start_call_time; //ͨ����ʼ��ʱ��,��¼����ʱ��
    uint32                  end_call_time; //ͨ��������ʱ��
    uint8                   call_direction; //MO or MT
    uint16                  call_type; //ͨ������
    uint16                  call_route; //����Դ
    boolean                 is_ps_call;
    boolean                 is_dtmf_exist;
    uint8                   dtmf_str[UAL_TELE_CALL_TELE_NUM_MAX + 1];
    uint8                   dtmf_len;
}ual_tele_call_data_info_t; //ÿͨCALL����Ϣ

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
} ual_tele_call_mo_start_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR

    boolean is_exist_cnap_name; //�Ƿ��������·���CNAP
    uint16  cnap_name[UAL_TELE_CALL_TELE_NAME_MAX_LEN +1]; //CNAP����
    uint8   cnap_name_len;      //CNAP���Ƴ���
} ual_tele_call_mt_start_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    uint32  alert_type; //�������ͣ�Զ�˻򱾵����� //ual_tele_call_alert_type_e
} ual_tele_call_mo_alert_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
} ual_tele_call_mo_connected_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
} ual_tele_call_mt_connected_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    uint32 cause; //ͨ���Ͽ�ԭ�� //ual_tele_call_disconnect_cause_e
    uint32 start_call_time; //ͨ����ʼ��ʱ��
    uint32 end_call_time;   //ͨ��������ʱ��
    uint8  call_direction;  //MO or MT
} ual_tele_call_disconnected_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    uint32 cause; //ual_tele_call_disconnect_cause_e
    uint32 start_call_time; //ͨ����ʼ��ʱ��
    uint32 end_call_time;   //ͨ��������ʱ��
    uint8  call_direction;  //MO or MT
} ual_tele_call_disconnected_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    boolean is_accepted; //Э��ջ�Ƿ���������ɹ�
} ual_tele_call_hold_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    boolean is_accepted;
} ual_tele_call_unhold_cnf_t;

typedef struct
{
    uint32              sim_id; //ual_tele_sim_id_e
    uint8               amr_type; /*0:AMR-NB, 1:AMR-WB*/
} ual_tele_call_amr_type_ind_t;

typedef struct
{
    uint32              sim_id; //ual_tele_sim_id_e
    uint8               ho_progress_type; /*0:PS->CS, 1:CS->PS*/
} ual_tele_call_ho_progress_ind_t;


//������������Ҫ���͵��ַ���
typedef struct
{
    uint32 sim_id; //ual_tele_sim_id_e
    uint8 dtmf_len;
    uint8 dtmf_str[UAL_TELE_CALL_TELE_NUM_MAX + 1];
}ual_tele_call_dtmf_info_t;

typedef struct
{
    ual_tele_call_dtmf_info_t dtmf_info;
} ual_tele_call_query_dtmf_ind_t;

typedef struct
{
    ual_tele_call_dtmf_info_t dtmf_info;
} ual_tele_call_start_dtmf_ind_t;

typedef struct
{
    ual_tele_call_dtmf_info_t dtmf_info;
    boolean is_success; //Э��ջ�Ƿ���������ɹ�
} ual_tele_call_send_dtmf_cnf_t;


//���ý�����������
typedef enum
{
    UAL_TELE_CALL_NOT_ECC,       //���ǽ�������
    UAL_TELE_CALL_CPHS_ECC,     //CPHSЭ��涨�Ľ������룬ͨ����999
    UAL_TELE_CALL_NETWORK_ECC,  //�����·��Ľ�������
    UAL_TELE_CALL_SIM_ECC,      //������SIM���еĽ�������
    UAL_TELE_CALL_NOSIM_ECC,    //�޿���������
    UAL_TELE_CALL_FAKE_ECC,     //�ٽ������У�����״̬�õ�ʱ��callʱ����Э��ջnormal���ͣ�����״̬����ʱ�������ײ�emergency����
    UAL_TELE_CALL_CUS_ECC,
    UAL_TELE_CALL_LOCAL_ECC,
    UAL_TELE_CALL_MAX
}ual_tele_call_ecc_type_e;

//���ý����������ݽṹ��
typedef struct
{
    uint32 sim_id; //ual_tele_sim_id_e
    uint8 ecc_num[UAL_TELE_CALL_ECC_NUM_MAX_LEN];  //emmergency number
    uint8 ecc_mum_len;  //emmergency number length
    boolean category_bit_flag;//is have categorybit
    uint8 category_bit;
    uint32 ecc_type; //ual_tele_call_ecc_type_e
}ual_tele_call_ecc_t;

/*�ⲿģ��ע��ĺ�������*/
typedef boolean (*ual_tele_call_injection_func) (uint32 event, void* p_param, uint32 param_size);

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description:use to make call
// Parameter: [In] dual_sys:sim id
//                 tele_num:tele number
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_make_call(
    ual_tele_sim_id_e dual_sys,
    uint8 tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1]);

/*****************************************************************************/
// Description:use to reject current call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_reject_call(void);

/*****************************************************************************/
// Description:use to release current call
// Parameter: [In] call_id
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_release_call(uint8 call_id);

/*****************************************************************************/
// Description:use to release all call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_release_all(void);

/*****************************************************************************/
// Description:use to answer call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_answer_call(void);

/*****************************************************************************/
// Description:use to release and answer call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_release_and_answer(void);

/*****************************************************************************/
// Description:use to hold call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_hold_call(void);

/*****************************************************************************/
// Description:use to unhold call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_unhold_call(void);

/*****************************************************************************/
// Description: get current call state
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_state_e
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
ual_tele_call_state_e ual_tele_call_get_current_call_state(void);

/*****************************************************************************/
// Description: get call state by call id
// Parameter: [In] call_id: Э��ջΪһ·ͨ�������id
//            [Out] none
//            [Return]ual_tele_call_state_e
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
ual_tele_call_state_e ual_tele_call_get_call_state(uint8 call_id);

/*****************************************************************************/
//  Description: ual_tele_call_set_cus_emergency_num
//  Parameter: [In] sim id:current sim id
//             [In] p_ecc_data:emergency data info
//             [In] ecc_data_count:emergency data count
//             [Out] none
//             [Return] ual_tele_call_result_e
//  Author: chunjuan.liang
//  Note:app set customise emergency num to ual
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_set_cus_emergency_num(ual_tele_sim_id_e sim_id,ual_tele_call_ecc_t *p_ecc_data,uint16 ecc_data_count);

/*****************************************************************************/
//  Description: ual_tele_call_is_emergency_num
//  Parameter: [In] sim id:current sim id
//             [In] p_tele_num:emergency num
//             [In] tele_num_len:emergency num length
//             [Out] none
//             [Return] BOOLEAN:TRUE:�ǽ������룻FALSE:�ǽ�������
//  Author: chunjuan.liang
//  Note:input tele number ,judge this number whether is emergency
/*****************************************************************************/
boolean ual_tele_call_is_emergency_num(ual_tele_sim_id_e sim_id,uint8 *p_tele_num,uint8 tele_num_len);

/*****************************************************************************/
//  Description: ual_tele_call_is_part_emergency_num
//  Parameter: [In] sim id:current sim id
//             [In] tele_num:emergency num
//             [In] tele_num_len:emergency num length
//             [Out] none
//             [Return] BOOLEAN:TRUE:�ǽ������룻FALSE:�ǽ�������
//  Author: chunjuan.liang
//  Note:input 1 or 2,3 number,judge this number whether is emergency
/*****************************************************************************/
boolean ual_tele_call_is_part_emergency_num(ual_tele_sim_id_e sim_id,uint8 *p_tele_num,uint8 tele_num_len);

/*****************************************************************************/
//  Description: ual_tele_call_get_ecc_type
//  Parameter: [In] sim id:current sim id
//             [In] p_tele_num:emergency num
//             [In] tele_num_len:emergency num length
//             [In/Out] p_ecc_type:emergency type
//             [Return] ual_tele_call_result_e:success
//  Author: chunjuan.liang
//  Note:get ecc type,��ǰֻ�пͻ����ã��Ż���fake ecc
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_get_ecc_type(ual_tele_sim_id_e sim_id,uint8 *p_tele_num,uint16 tele_num_len,ual_tele_call_ecc_type_e *p_ecc_type);

/*****************************************************************************/
//  Description : ���� stop dtmf
//  Parameter: [In] sim_id: ���ۺ�
//             [Out] none
//             [Return] ual_tele_call_result_e:�ӿڵĴ�����
//  Author: mingwei.jia
//  Note: APP���Ʒ���stop dtmfʱ���ô˽ӿ�
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_stop_single_dtmf(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
//  Description : ���͵��� dtmf �ַ�
//  Parameter: [In] sim_id: ���ۺ�
//             [In] dtmf_char :Ҫ���͵�һ���ַ�
//             [In] is_stop_immediately :�Ƿ�����ֹͣ
//             [Out] none
//             [Return] ual_tele_call_result_e:�ӿڵĴ�����
//  Author: mingwei.jia
//  Note: APP���Ʒ��͵����ַ�ʱ���ô˽ӿ�
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_start_single_dtmf(ual_tele_sim_id_e sim_id,
                                                               uint8 dtmf_char,
                                                               boolean is_stop_immediately);

/*****************************************************************************/
//  Description : ������ dtmf �ַ���
//  Parameter: [In] sim_id: ���ۺ�
//             [Out] none
//             [Return] ual_tele_call_result_e:�ӿڵĴ�����
//  Author: mingwei.jia
//  Note: APP����ȡ������һ���ַ���ʱ���ô˽ӿ�
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_cancel_string_dtmf(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
//  Description : ���� dtmf �ַ���
//  Parameter: [In] dtmf_ch :�����͵�DTMF�ַ���
//             [Out] none
//             [Return] ual_tele_call_result_e:�ӿڵĴ�����
//  Author: mingwei.jia
//  Note: APP���Ʒ���һ���ַ���ʱ���ô˽ӿ�
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_send_string_dtmf(ual_tele_call_dtmf_info_t dtmf_info);

/*******************************************************************************/
//  Description: ��ͨ�������У��л���Ƶͨ·
//  Parameter: [In] is_enable : �򿪡��ر�dev��ʶ����Ƶ�豸
//             [In] dev :��Ƶ�豸
//             [Out] None
//             [Return] ual_tele_call_result_e :�л���Ƶͨ·�Ľ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_switch_audio_dev(boolean is_enable, ual_tele_call_audio_dev_e dev);

/*******************************************************************************/
//  Description: ��ȡ��ǰ��Ƶͨ·
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_tele_call_audio_dev_e :��ǰ��Ƶͨ·
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_audio_dev_e ual_tele_call_get_current_audio_dev(void);

/*******************************************************************************/
//  Description: ����ͨ������
//  Parameter: [In] volume : ͨ������
//             [Out] None
//             [Return] ual_tele_call_result_e :����ͨ�������Ľ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_set_call_volume(uint8 volume);

/*******************************************************************************/
//  Description: ��ȡͨ������
//  Parameter: [In] None
//             [Out] None
//             [Return] uint8 : ͨ������
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
uint8 ual_tele_call_get_call_volume(void);

/*******************************************************************************/
//  Description: �ڲ���绰ʱ���Է��Ѿ����壬����δ�յ������·��Ļ�����ʱ�����ű��ػ�����
//  Parameter: [In] duration : ���ػ������ĳ���ʱ�������ŵ�ʱ��
//             [In] ring_timer :���ػ�����timer
//             [In] freq1 :��Ƶʱ�ĵ�һ��Ƶ��
//             [In] freq2 :��Ƶʱ�ĵڶ���Ƶ��
//             [In] freq3 :��Ƶʱ�ĵ�����Ƶ��
//             [Out] None
//             [Return] ual_tele_call_result_e :??????????
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_play_local_netring(uint32 duration, uint32 ring_timer, uint32 freq1, uint32 freq2, uint32 freq3);

/*******************************************************************************/
//  Description: ֹͣ���ػ�����
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_tele_call_result_e :ֹͣ���ű��ػ�����
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_stop_local_netring(void);

/*******************************************************************************/
//  Description: ����������·����
//  Parameter: [In] is_mute : ��/�ر�������·����
//             [Out] None
//             [Return] ual_tele_call_result_e :����������·�����Ľ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_set_mic_mute(boolean is_mute);

/*******************************************************************************/
//  Description: ��ȡ������·�����Ƿ��
//  Parameter: [In] None
//             [Out] None
//             [Return] boolean : ������·�����Ƿ�򿪵Ľ��
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
boolean ual_tele_call_get_mic_mute(void);

/*****************************************************************************/
//  Description: ��ǰsim��volte��������
//  Parameter: [In] sim_id //��ǰsim��
//             [In] is_on //����״̬, TRUE or FALSE
//             [Out] none
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: yanli.yang
//  Note: �ýӿ���ual_tele_radio_set_volte����һ�£�����ģ����ж��壬�û�ѡ��
//        ʹ���ĸ�����
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_set_volte(ual_tele_sim_id_e sim_id, boolean is_on);

/*****************************************************************************/
//     Description :����sim����VoLTE����
//     Parameter : [In]  sim_id : sim��
//                 [Out] none
//                 [Return] ��(TRUE)���ر�(FALSE)
//     Author: yanli.yang
//     Note: �ýӿ���ual_tele_radio_get_volte_state����һ�£�����ģ����ж��壬
//           �û�ѡ��ʹ���ĸ�����
/*****************************************************************************/
boolean ual_tele_call_get_volte_state(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
// Description: get current call info
// Parameter: [In] none
//            [Out] current call info
//            [Return]result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_get_current_call_info(ual_tele_call_data_info_t* p_call_info);

/*****************************************************************************/
// Description: get call info
// Parameter: [In] call_id: Э��ջΪһ·ͨ�������id
//            [Out] call info
//            [Return]result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_get_call_info(uint8 call_id,ual_tele_call_data_info_t* p_call_info);

/*****************************************************************************/
// Description:user to get all calls number
// Parameter: [In] none
//            [Out] none
//            [Return] all calls number
//  Author:mingwei.jia
//  Note:
/*****************************************************************************/
uint8 ual_tele_call_get_call_count(void);

/*******************************************************************************/
//	Description: ual_tele_call_register
//	Parameter: [In] p_callback private call back
//	           [Out] p_handle out of phandle
//	           [Return] ual_tele_call_result_e
//	Author: mingwei.jia
//	Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_register(_ual_cms_client_register_callback p_callback, uint32 *p_handle);

/*******************************************************************************/
//	Description: ual_tele_call_unregister
//	Parameter: [in] handle
//	           [Out] none
//	           [Return] ual_tele_call_result_e
//	Author: mingwei.jia
//	Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_unregister(uint32 handle);

/*****************************************************************************/
//  Description: ע�뺯��
//  Parameter: [In] p_func   //�ⲿģ����ual tele callע��ĺ���
//             [Out] none
//             [Return] ������
//  Author: mingwei.jia
//  Note:�ⲿģ��ɵ��ô˽ӿ�ע�뺯����p_func��make call�ӿ��лᱻ����
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_inject_function(ual_tele_call_injection_func p_func);

/*****************************************************************************/
//  Description:ȡ��ע�뺯��
//  Parameter: [In] none
//             [Out] none
//             [Return] ������
//  Author: mingwei.jia
//  Note:�ⲿģ��ɵ��ô˽ӿ�ȡ��ע�뺯����������˽ӿں�make call�н������ٵ���ע�뺯��
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_uninject_function(void);

/*******************************************************************************/
//	Description: ual_tele_call_init
//	Parameter: [in] none
//	           [Out] none
//	           [Return] none
//	Author: tingting.zheng
//	Note:
/*******************************************************************************/
void ual_tele_call_init( void );

#endif

