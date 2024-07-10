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
** File Name:       ual_tele_radio.h                                           *
** Author:            siyuan.fei                                               *
** Date:            2022/08/11                                                 *
** Description:This file is used to define ual tele radio it cases export      *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 2022/08/11          siyuan.fei                  Create                      *
********************************************************************************/
#ifndef _UAL_TELE_RADIO_H_
#define _UAL_TELE_RADIO_H_

/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "ual_tele_data.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define UAL_TELE_RADIO_IMEI_MAX_LEN                (15) //imei str max len
#define UAL_TELE_RADIO_MEID_MAX_LEN                (15) //meid str max len
#define UAL_TELE_RADIO_ITEM_ARR_MAX_SIZE           (50) //item arr max count
#define UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT      (5)  //gsm neighbor max count
#define UAL_TELE_RADIO_UMTS_NEIGHBOR_MAX_COUNT     (18) //umts neighbor max count
#define UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT      (8)  //lte neighbor max count
#define UAL_TELE_RADIO_PLMN_MAX_NUM (16) //�ֶ����������PLMN��������

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//RADIOģ�鷵�ؽ��
typedef enum
{
    UAL_TELE_RADIO_RES_SUCCESS = UAL_TELE_RES_SUCCESS,
    UAL_TELE_RADIO_RES_PARAM_ERROR = UAL_MODULE_TELE_RADIO << 16,//1179648
    UAL_TELE_RADIO_RES_FAIL,
    UAL_TELE_RADIO_RES_ASYNC = UAL_TELE_RADIO_RES_FAIL + 0xFF,
    UAL_TELE_RADIO_RES_MAX
} ual_tele_radio_result_e;

// RADIOģ�鷵�ظ�client�˵���Ϣö��
typedef enum
{
    MSG_UAL_TELE_RADIO_RSSI_IND = UAL_MODULE_TELE_RADIO << 16,
    MSG_UAL_TELE_RADIO_PLMN_LIST_CNF,
    MSG_UAL_TELE_RADIO_SELECT_PLMN_CNF,
    MSG_UAL_TELE_RADIO_IMS_STATE_IND,
    MSG_UAL_TELE_RADIO_NETWORK_STATUS_IND,
    MSG_UAL_TELE_RADIO_VOLTE_STATE_IND,
    MSG_UAL_TELE_RADIO_LTE_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_LTE_NEIGH_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_GSM_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_GSM_NEIGH_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_MAX
}ual_tele_radio_msg_e;

//�����źŵȼ���Ŀǰ��Ϊ0~5��6������Ӧ��Ӧ���豸�ź�����ʾ
typedef enum
{
    UAL_TELE_RADIO_RSSI_LEVEL0,
    UAL_TELE_RADIO_RSSI_LEVEL1,
    UAL_TELE_RADIO_RSSI_LEVEL2,
    UAL_TELE_RADIO_RSSI_LEVEL3,
    UAL_TELE_RADIO_RSSI_LEVEL4,
    UAL_TELE_RADIO_RSSI_LEVEL5,
    UAL_TELE_RADIO_RSSI_MAX
} ual_tele_radio_rssi_level_e;
//��MSG_UAL_TELE_RADIO_RSSI_IND�ϱ��Ľṹ�壬��ʾsim���ź�ǿ��
typedef struct
{
    uint32 sim_id; //sim id //ual_tele_sim_id_e
    uint32 rssi_level; //�ź�ǿ��(0-5) //ual_tele_radio_rssi_level_e
    uint32 cell_id; //С��ʶ����
    uint32 physical_cell_id; //����С��ʶ����
}ual_tele_radio_rssi_ind_t;

// MSG_UAL_TELE_RADIO_VOLTE_STATE_IND��Ϣ�ϱ��Ľṹ�壬���ص�ǰ�����Ƿ�֧��volte
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    boolean is_volte_support; // ��ǰ�����Ƿ�֧��VoLTE
}ual_tele_radio_volte_state_ind_t;

//�����������ͣ���Ϊ6��
typedef enum
{
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM = 0,  // only 2G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_3G, // only 3G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM_AND_3G, // 2G & 3G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_LTE = 0x10, // only 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM_LTE, // 2G & 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_3G_LTE, // 3G & 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM_3G_LTE, // 2G & 3G & 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_MAX
} ual_tele_radio_prefer_network_type_e;
//��ǰ��������
typedef enum
{
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_2G = 0, // 2G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_3G, // 3G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_4G, // 4G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_5G, // 5G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_UNKNOWN, // δ ֪
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_MAX
} ual_tele_radio_current_network_type_e;

//�ֶ�������������
typedef enum
{
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM = 0,  // only 2G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_3G, // only 3G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM_AND_3G, // 2G & 3G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_LTE = 0x10, // only 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM_LTE, // 2G & 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_3G_LTE, // 3G & 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM_3G_LTE, // 2G & 3G & 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_MAX
} ual_tele_radio_manual_network_type_e;
//plmn״̬ö��,4��
typedef enum
{
    UAL_TELE_RADIO_PLMN_STATE_NO_SERVICE, // �޷���
    UAL_TELE_RADIO_PLMN_STATE_EMERGENCY, // ����״̬
    UAL_TELE_RADIO_PLMN_STATE_REGISTERING, // ע����
    UAL_TELE_RADIO_PLMN_STATE_NORMAL, // ����״̬
    UAL_TELE_RADIO_PLMN_STATE_MAX
} ual_tele_radio_plmn_state_e;
//APN����,6��
typedef enum
{
    UAL_TELE_RADIO_APN_TYPE_INTERNET,
    UAL_TELE_RADIO_APN_TYPE_MMS,
    UAL_TELE_RADIO_APN_TYPE_IMS,
    UAL_TELE_RADIO_APN_TYPE_IA,
    UAL_TELE_RADIO_APN_TYPE_XCAP,
    UAL_TELE_RADIO_APN_TYPE_SOS,
    UAL_TELE_RADIO_APN_TYPE_MAX
} ual_tele_radio_apn_type_e;
//APN��Ϣ
typedef struct
{
    ual_tele_data_apn_info_t ia_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t internet_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t xcap_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t ims_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t sos_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t mms_apn_info[SIM_ID_MAX];
} ual_tele_radio_apn_info_t;

//IMS״̬ö��,��8��
typedef enum
{
    UAL_TELE_RADIO_IMS_STATE_INACTIVE, // δ����
    UAL_TELE_RADIO_IMS_STATE_REGISTERED, // ��ע��
    UAL_TELE_RADIO_IMS_STATE_REGISTERING, // ע����
    UAL_TELE_RADIO_IMS_STATE_FAILED, // ʧ��
    UAL_TELE_RADIO_IMS_STATE_UNKNOWN, // δ֪
    UAL_TELE_RADIO_IMS_STATE_ROAMING, // ����
    UAL_TELE_RADIO_IMS_STATE_DEREGISTERING, // ע����
    UAL_TELE_RADIO_IMS_STATE_INVALID, // ���Ϸ�
    UAL_TELE_RADIO_IMS_STATE_MAX
} ual_tele_radio_ims_state_e;
//��MSG_UAL_TELE_RADIO_IMS_STATE_IND��Ϣ�ϱ��Ľṹ�壬���ص�ǰ����ims״̬
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    uint32 ims_state; // ims״̬ //ual_tele_radio_ims_state_e
} ual_tele_radio_ims_state_ind_t;
//С����Ϣ
typedef struct
{
    uint32 cell_id;
    uint16 mcc;
    uint16 mnc;
} ual_tele_radio_cell_info_t;
//gsmС����Ϣ
typedef struct
{
    uint16 arfcn;      // ��������Ƶ�ʱ�� Absolute Radio Frequency Channel Number
    uint8 bsic;       // ��վʶ���� Base-Station Identification Code
    uint16 cell_id;    // С��ʶ����
    uint8 rxqual;     // �źŽ�������  Received Signal Quality
    uint8 rxlev;      // ƽ�����յ�ƽ Received Signal Level
    uint16 scell_band; // ��С��Ƶ��
    uint8 txpwr;      // ���书��
    uint8 ta_value;   // ʱ����ǰ�� Timing advance
    uint16 lac;       // λ�������� Location Area Code
    uint8 mcc;        // �ƶ����Һ� Mobile Country Code
    uint8 mnc;        // �ƶ����� Mobile Country Code
} ual_tele_radio_gsm_cell_info_t;
//umtsС����Ϣ
typedef struct
{
    uint16 lac;   // λ�������� Location Area Code
    uint16 mcc;   // �ƶ����Һ� Mobile Country Code
    uint16 mnc;   // �ƶ����� Mobile Country Code
    int16 psc;    // ��ͬ���� Primary Synchronization Code
    int16 uarfcn; // ��������Ƶ�ŵ��� UTRA Absolute Radio Frequency Channel Number
    uint32 ucid;  // ͨ�ñ�ʶ�� Universal Cell Identifier
    int16 rscp;   // ��Ƶ�ŵ��ź�ǿ�� receive sigal code power,range -120dbm tp -20 dbm,-1 if unknown
    int32 rncid;  // �����������ʶ Radio Network Controller Identifier
    int32 ecno;   // Energy per chip over noise destiny
} ual_tele_radio_umts_cell_info_t;
//radio item
typedef struct
{
    uint8 count;
    uint32 values[UAL_TELE_RADIO_ITEM_ARR_MAX_SIZE];
} ual_tele_radio_item_t;
//lteС����Ϣ
typedef struct
{
    ual_tele_radio_item_t band;   // Ƶ��
    ual_tele_radio_item_t earfcn; // LTE����Ƶ��� E-UTRA Absolute Radio Frequency Channel Number
    ual_tele_radio_item_t pci;    // ����С����ʶ Physical Cell Identifier
    ual_tele_radio_item_t rsrp;   // �ο��źŽ��չ��� Reference Signal Received Power
    ual_tele_radio_item_t rsrq;   // �ο��źŽ������� Reference Signal Received Quality
    ual_tele_radio_item_t bw;     // ���� Bandwidth
    ual_tele_radio_item_t ta;     // ������ Tracking Area
    ual_tele_radio_item_t tac;    // ���������� Tracking Area Code
    ual_tele_radio_item_t mcc;
    ual_tele_radio_item_t mnc;
    ual_tele_radio_item_t cell_id;// С��ʶ����
    ual_tele_radio_item_t srxlev; // �����źŵȼ�/��ƽ Received Signal Level
} ual_tele_radio_lte_cell_info_t;
//gsm��С����Ϣ
typedef struct
{
    int16 arfcn;   // ��������Ƶ�ʱ�� Absolute Radio Frequency Channel Number
    int16 bsic;    // ��վʶ���� Base-Station Identification Code
    int16 cell_id; // С��ʶ����
    int16 rxqual;  // �źŽ�������  Received Signal Quality
    int16 rxlev;   // �����źŵȼ�/��ƽ Received Signal Level
    uint16 lac;    // λ�������� Location Area Code
    uint8 mcc;
    uint8 mnc;
}ual_tele_radio_gsm_neighbor_cell_info_t;
//��MSG_UAL_TELE_RADIO_GSM_NEIGH_CELL_INFO_CNF�ϱ�
typedef struct
{
    uint8 count;
    ual_tele_radio_gsm_neighbor_cell_info_t gsm_neighbor_info[UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT];
} ual_tele_radio_gsm_ncell_cnf_t;
//umts��С����Ϣ
typedef struct
{
    int16 psc;    // ��ͬ���� Primary Synchronization Code
    int16 uarfcn; // ��������Ƶ�ŵ��� UTRA Absolute Radio Frequency Channel Number
    int16 rscp;   // ��Ƶ�ŵ��ź�ǿ�� Receive Signal Channel Power
} ual_tele_radio_umts_neighbor_cell_info_t;
//lte��С����Ϣ
typedef struct
{
    int16 pci;  // ����С����ʶ Physical Cell Identifier
    int16 rsrp; // �ο��źŽ��չ��� Reference Signal Received Power
    int16 rsrq; // �ο��źŽ������� Reference Signal Received Quality
    int16 rat;  // ���߽��뼼�� Radio Access Technology
    int32 earfcn; // LTE����Ƶ��� E-UTRA Absolute Radio Frequency Channel Number
    uint8 mcc;
    uint8 mnc;
    uint16 tac; // �����������
    uint32 cell_id; // С��ʶ����
    int16 srxlev; // �����źŵȼ�/��ƽ Received Signal Level
    uint32 frequency;
} ual_tele_radio_lte_neighbor_cell_info_t;

//��MSG_UAL_TELE_RADIO_LTE_NEIGH_CELL_INFO_CNF�ϱ�
typedef struct
{
    uint8 count;
    ual_tele_radio_lte_neighbor_cell_info_t lte_neighbor_info[UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT];
} ual_tele_radio_lte_ncell_cnf_t;

//����� Signal to InterferenceNoiseRatio
typedef ual_tele_radio_item_t ual_tele_radio_snr_t;
//����״̬
typedef enum
{
    UAL_TELE_RADIO_ROAMING_DISABLE, //�ر�
    UAL_TELE_RADIO_ROAMING_ENABLE, //����
    UAL_TELE_RADIO_ROAMING_MAX
} ual_tele_radio_roaming_mode_e;
//plmn����ģʽ���ֶ����Զ�
typedef enum
{
    UAL_TELE_RADIO_PLMN_SEARCH_MODE_MANUAL, //�ֶ�
    UAL_TELE_RADIO_PLMN_SEARCH_MODE_AUTO,  //�Զ�
    UAL_TELE_RADIO_PLMN_SEARCH_MODE_MAX
} ual_tele_radio_plmn_search_mode_e;
//��MSG_UAL_TELE_RADIO_NETWORK_STATUS_IND�ϱ��Ľṹ�壬�ϱ�����״̬
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    ual_tele_plmn_info_t network_plmn_info; // �����plmn��Ϣ
    uint16 lac; // ������
    uint16 tac; // �����������
    boolean is_roaming; // �Ƿ�������
    uint32 network_type; // ��ǰ�������� //ual_tele_radio_current_network_type_e
    uint32 plmn_state; // plmn״̬ //ual_tele_radio_plmn_state_e
    boolean is_gprs_available; // gprs�Ƿ����
} ual_tele_radio_network_status_ind_t;
//����plmn����ϸ��Ϣ
typedef struct
{
    ual_tele_plmn_info_t plmn_info; // sim����plmn��Ϣ
    uint32 reg_rat; // �������� //ual_tele_radio_prefer_network_type_e
    uint16 lac; // ������
    boolean plmn_is_forbidden; // �Ƿ��ֹ
}ual_tele_radio_plmn_single_t;
//��MSG_UAL_TELE_RADIO_PLMN_LIST_CNF�ϱ��Ľṹ�壬�ϱ�plmn�б�
typedef struct
{
    uint32 sim_id; //ual_tele_sim_id_e
    uint32 result; // ������� //ual_tele_radio_result_e
    uint8 plmn_number; // ��������plmn����
    ual_tele_radio_plmn_single_t plmn_single[UAL_TELE_RADIO_PLMN_MAX_NUM]; // ����plmn����ϸ��Ϣ
} ual_tele_radio_plmn_list_cnf_t;
//��MSG_UAL_TELE_RADIO_SELECT_PLMN_CNF�ϱ��Ľṹ�壬�ϱ�plmnѡȡ����Ϣ
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    boolean plmn_is_selected; // plmn�Ƿ�ѡ��
    uint32 cell_id; // С��id
    ual_tele_plmn_info_t plmn_info; // sim��plmn��Ϣ
} ual_tele_radio_select_plmn_cnf_t;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: ע��tele radioģ��ӿ�
//  Parameter: [In] p_callback //ע��Ļص�����
//             [Out] p_handle //������tele radio��handle
//             [Return] ual_tele_radio_result_e //ע����
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_register(_ual_cms_client_register_callback p_callback, uint32 *p_handle);
/*****************************************************************************/
//  Description: ע��tele radioģ��ӿ�
//  Parameter: [In] handle //tele radio��handle
//             [Out] none
//             [Return] ual_tele_radio_result_e //ע�����
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_unregister(uint32 handle);
/*****************************************************************************/
//  Description: ��ʼ��tele radioģ��
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
void ual_tele_radio_init(void);
/*****************************************************************************/
//  Description: ��ȡ��ǰsim���ź�ǿ��
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ual_tele_radio_rssi_level_e //�ź�ǿ��rssi_level(��Χ0-5)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_rssi_level_e ual_tele_radio_get_rssi_strength_level(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ��ȡ��ǰsim����������
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ual_tele_radio_current_network_type_e //��ǰ��������
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_current_network_type_e ual_tele_radio_get_current_network_type(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ��ȡsim��������������
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ual_tele_radio_prefer_network_type_e //������������
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_prefer_network_type_e ual_tele_radio_get_prefer_network_type(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: set prefer network type
//  Parameter: [In] sim_id //��ǰsim��
//             [In] prefer_type //������������
//             [Out] none
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_prefer_network_type(ual_tele_sim_id_e sim_id, ual_tele_radio_prefer_network_type_e prefer_type);
/*****************************************************************************/
//  Description: �����ֶ�������������
//  Parameter: [In] sim_id //��ǰsim��
//             [In] plmn_info //plmn��Ϣ
//             [In] network_type //���õ���������
//             [Out] none
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_manual_network_type(ual_tele_sim_id_e sim_id, ual_tele_plmn_info_t plmn_info, ual_tele_radio_manual_network_type_e network_type);
/*****************************************************************************/
//  Description: ��ȡ��ǰsim��plmn��Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] p_plmn //plmn��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_current_plmn_info(ual_tele_sim_id_e sim_id, ual_tele_plmn_info_t* p_plmn);
/*****************************************************************************/
//  Description: ��ȡ��ǰsim��plmn״̬
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ual_tele_radio_plmn_state_e //plmn״̬
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_plmn_state_e ual_tele_radio_get_current_plmn_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ����apn��Ϣ�����ڽ���ƥ��APN�����ø�radioģ��
//  Parameter: [In] sim_id: ��ǰsim����index
//             [In] p_apn_info: ��ǰsim��ƥ����apn��Ϣ
//             [Out] None
//             [Return] ����apn��Ϣ�Ľ��ö��ֵ
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_apn_info(ual_tele_sim_id_e sim_id, ual_tele_radio_apn_info_t* p_apn_info);

/*****************************************************************************/
//  Description: ��ȡ��ǰsim��apn��Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] p_internet_apn //internet apn��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_internet_apn_info(ual_tele_sim_id_e sim_id, ual_tele_data_apn_info_t* p_internet_apn);
/*****************************************************************************/
//  Description: ���µ�ǰsim��Internet apn��Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [In] p_internet_apn_info //����apn��Ϣ
//             [Out] none
//             [Return] ual_tele_radio_result_e //���½��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_update_internet_apn_info(ual_tele_sim_id_e sim_id, ual_tele_data_apn_info_t* p_internet_apn_info);
/*****************************************************************************/
//  Description: ��ȡIMS״̬
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ual_tele_radio_ims_state_e //IMS״̬
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_ims_state_e ual_tele_radio_get_ims_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ��ȡ��ǰsim��volte״̬
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ��(TRUE)���ر�(FALSE)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_volte_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ��ȡ��ǰsim���Ƿ�֧��volte
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ֧��(TRUE)����֧��(FALSE)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_current_network_volte_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ��ǰsim��volte��������
//  Parameter: [In] sim_id //��ǰsim��
//             [In] is_on //����״̬, TRUE or FALSE
//             [Out] none
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_volte(ual_tele_sim_id_e sim_id, boolean is_on);
/*****************************************************************************/
//  Description: ��ȡ��ǰradioģ�����ģʽ����״̬
//  Parameter: [In] none
//             [Out] none
//             [Return] ����״̬(TRUE)���ر�״̬(FALSE)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_fly_mode_state(void);
/*****************************************************************************/
//  Description: ����radioģ�����ģʽ
//  Parameter: [In] is_on //������رշ���ģʽ
//             [Out] none
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_fly_mode(boolean is_on);
/*****************************************************************************/
//  Description: �ر�PS,�ػ�ʱ����
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
void ual_tele_radio_power_off_ps(void);
/*****************************************************************************/
//  Description: ��ȡ��ǰС����Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] p_current_cell_info //��ǰС����Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_current_cellinfo(ual_tele_sim_id_e sim_id, ual_tele_radio_cell_info_t *p_current_cell_info);
/*****************************************************************************/
//  Description: ��ȡgsm��Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] p_gsm_cell_info //gsm��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_gsm_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_gsm_cell_info_t *p_gsm_cell_info);
//  Description: ��ȡ3gС����Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] p_umts_cell_info //3g��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_3g_cell_info (ual_tele_sim_id_e sim_id, ual_tele_radio_umts_cell_info_t *p_umts_cell_info);
/*****************************************************************************/
//  Description: ��ȡLTEС����Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] p_lte_cell_info //LTE��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_lte_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_lte_cell_info_t *p_lte_cell_info);
/*****************************************************************************/
//  Description: ��ȡgsm��С����Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] gsm_neighbor_cell_info[UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT] //��С��gsm��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_gsm_neighbor_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_gsm_neighbor_cell_info_t gsm_neighbor_cell_info[UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT]);
/*****************************************************************************/
//  Description: ��ȡumts��С����Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] umts_neighbor_cell_info[UAL_TELE_RADIO_UMTS_NEIGHBOR_MAX_COUNT] //��С��umts��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_umts_neighbor_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_umts_neighbor_cell_info_t umts_neighbor_cell_info[UAL_TELE_RADIO_UMTS_NEIGHBOR_MAX_COUNT]);
/*****************************************************************************/
//  Description: ��ȡlte��С����Ϣ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] lte_neighbor_cell_info[UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT] //��С��lte��Ϣ
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_lte_neighbor_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_lte_neighbor_cell_info_t lte_neighbor_cell_info[UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT]);
/*****************************************************************************/
//  Description: ��ȡlte��ʽ����ȣ����������
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] p_snr //�����
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_lte_snr(ual_tele_sim_id_e sim_id, ual_tele_radio_snr_t *p_snr);
/*****************************************************************************/
//  Description: ��ȡimei��
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1] // char����
//             [Return] ual_tele_radio_result_e //��ȡ���
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_imei(ual_tele_sim_id_e sim_id, char imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1]);
/*****************************************************************************/
//  Description: ����imei��
//  Parameter: [In] sim_id //��ǰsim��
//                  [In] imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1] // char����
//             [Out] none
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_imei (ual_tele_sim_id_e sim_id, char imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1]);
/*****************************************************************************/
//  Description: ��ȡmeid��
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] meid[UAL_TELE_RADIO_MEID_MAX_LEN + 1] // char����
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_meid (ual_tele_sim_id_e sim_id, char meid[UAL_TELE_RADIO_MEID_MAX_LEN + 1]);
/*****************************************************************************/
//  Description: ��ȡ����ģʽ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ual_tele_radio_roaming_mode_e // ����ģʽ
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_roaming_mode_e ual_tele_radio_get_roaming_mode(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ��ȡplmn����ģʽ
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] ual_tele_radio_plmn_search_mode_e // plmn����ģʽ���ֶ����Զ�
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_plmn_search_mode_e ual_tele_radio_get_plmn_search_mode(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: ����plmn����ģʽ
//  Parameter: [In] sim_id //��ǰsim��
//                  [In] search_mode // ���õ�plmn����ģʽ���ֶ����Զ�
//             [Out] none
//             [Return] ual_tele_radio_result_e //���ý��
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_plmn_search_mode (ual_tele_sim_id_e sim_id, ual_tele_radio_plmn_search_mode_e search_mode);
/*****************************************************************************/
//  Description: ��ȡ��ǰsim��gprs״̬
//  Parameter: [In] sim_id //��ǰsim��
//             [Out] none
//             [Return] boolean //gprs״̬
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_gprs_state(ual_tele_sim_id_e sim_id);
#endif
