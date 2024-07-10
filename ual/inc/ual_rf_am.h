/*****************************************************************************
** Copyright 2021-2022 Unisoc(Shanghai) Technologies Co.,Ltd                 *
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
** File Name:       ual_rf_am.h                                              *
** Author:          zhenpeng.yao                                             *
** Date:            09/21/2022                                               *
** Description:    This file is used to define ual rf_am export              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 09/21/2022         zhenpeng.yao              Create                       *
******************************************************************************/
#ifndef _UAL_RF_AM_H
#define _UAL_RF_AM_H
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ual_type.h"
#include "ual_common.h"

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          ENUM AND STRUCT                                  */
/*---------------------------------------------------------------------------*/
/*cur state*/
typedef enum
{
    RF_AM_STATE_IDLE,              //wifi and gnss and lte not work
    RF_AM_STATE_GNSS_COLD_WORK,    //gnss work
    RF_AM_STATE_GNSS_HOT_WORK,     //gnss wokr
    RF_AM_STATE_WIFISCAN_WORK,     //wifiscan work
    RF_AM_STATE_LTE_WORK,          //lte work
    RF_AM_STATE_MAX
}ual_rf_am_state_e;

//�¼�����
typedef enum
{
    MSG_UAL_RF_AM_START_GNSS_REJECT = UAL_MODULE_RF_AM << 16,
    MSG_UAL_RF_AM_NOTIFY_CC_HANGUP,
    MSG_UAL_RF_AM_MAX
} ual_rf_am_msg_e;

/*cur state*/
typedef enum
{
    RF_AM_LTE_STATE_IDLE,
    RF_AM_LTE_STATE_CONNECT,
    RF_AM_LTE_STATE_MAX
}ual_rf_am_lte_state_e;

/*����ģʽ*/
typedef enum
{
    UAL_RF_AM_BACKGROUND_POSITION_MODE = 1,//��̨��λģʽ
    UAL_RF_AM_SPORT_POSITION_MODE,     //�˶�ģʽ
    UAL_RF_AM_COMMUNICATION_MODE,      //LTEͨ��ģʽ
    UAL_RF_AM_MODE_MAX
}ual_rf_am_mode_e;

/*�ӿڵ��÷���ֵ*/
typedef enum
{
    UAL_RF_AM_RES_SUCCESS,
    UAL_RF_AM_RES_PARAM_ERR = UAL_MODULE_RF_AM<<16,
    UAL_RF_AM_RES_STATE_ERR,
    UAL_RF_AM_RES_OTHER_ERR,
    UAL_RF_AM_RES_PERMIT_RUN,
    UAL_RF_AM_RES_MAX
}ual_rf_am_result_e;

#ifdef  UAL_TELE_SUPPORT
#define CALL_NUM_MAX_LEN            (40) //�����볤��
typedef struct
{
    char    call_num[CALL_NUM_MAX_LEN + 1];//������Ϣ
    uint8   call_num_len;                       //���볤��
    boolean  is_ecc_present;
    uint8    emgcy_ctgry_value;                 //��������Я��catgryֵ
    uint32  clir_type;                //�Ƿ��������
    uint32  call_type;
} call_make_call_t;
typedef struct
{
    uint32 sim_id;
    call_make_call_t make_call_info;
}ual_rf_am_make_call_info_t;//make call info
#endif

typedef struct
{
    MN_DUAL_SYS_E dual_sys;
    MN_SMS_MO_SMS_T   mo_sms;
    MN_SMS_STORAGE_E storage;
    MN_SMS_PATH_E send_path;
    boolean is_more_msg;
}ual_rf_am_sms_send_info_t;//send sms info

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: ���ù���ģʽ
//  Parameter: [In] mode   //Ҫ���õĹ���ģʽ
//             [Out] none
//             [Return] ������
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_set_mode(ual_rf_am_mode_e mode);

/*****************************************************************************/
//  Description: ��ȡ����ģʽ
//  Parameter: [In] none
//             [Out] none
//             [Return] ��ǰ����ģʽ
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_mode_e ual_rf_am_get_current_mode(void);

/*****************************************************************************/
//  Description: ������ʼ��
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_rf_am_result_e
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_init(void);
/*****************************************************************************/
//  Description: ���մ�sms������Ϣ
//  Parameter:
//             [In] msg_id
//             [In] param
//             [Out] none
//             [Return] MMI_RESULT_E
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e rf_am_handle_sms_msg(void* p_param, uint32 param_size);

/*****************************************************************************/
//  Description: ��ȡ����״̬
//  Parameter: [In] none
//             [Out] none
//             [Return] ��ǰ����״̬
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_state_e ual_rf_am_get_current_status(void);

/*****************************************************************************/
//  Description: ��ȡlte����״̬
//  Parameter: [In] none
//             [Out] none
//             [Return] ��ǰ����״̬
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_lte_state_e ual_rf_am_get_current_lte_status(void);

/*****************************************************************************/
//  Description: ǿ��lte��idel
//  Parameter: [In] none
//             [Out] none
//             [Return] uint32
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_force_lte_to_idle(void);

/*****************************************************************************/
//  Description: ע��cb
//  Parameter: [In] p_callback   //app��ual rf amע���callback
//             [Out] p_handle    //ע��õ���cms handle��ȥע���ʱ����Ҫ�����handle
//             [Return] ������
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_register(_ual_cms_client_register_callback p_callback, uint32* p_handle);

/*****************************************************************************/
//  Description: callbackȥע��
//  Parameter: [In] handle
//             [Return] ������
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_unregister(uint32 handle);

#endif
