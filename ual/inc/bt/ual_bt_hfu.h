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
** File Name:       ual_bt_hfu.h                                             *
** Author:          fangfang.yao                                             *
** Date:            2021/01/08                                               *
** Description:     This file is used to define export ual api of bt hfu     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION                      *
** 01/08/2021           fangfang.yao          Create                         *
******************************************************************************/

#ifndef _UAL_BT_HFU_H_
#define _UAL_BT_HFU_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "ual_bt_common.h"
#include "ual_cms_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UAL_BT_MAX_TELE_NUM_LEN (40)

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: ʵ��HFU init
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
void ual_bt_hfu_init( void );

/*****************************************************************************/
//  Description: ʵ��HFUע�Ṧ��
//  Parameter: [In] p_callback : �ص�����
//             [In] p_regiser_handle: HFU����handle id
//             [Out] None
//             [Return] ����ע���¼�����Ľ��
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_register(_ual_cms_client_register_callback *p_callback , uint32 *p_regiser_handle);

/*****************************************************************************/
//  Description: ʵ��HFUȥע�Ṧ��
//  Parameter: [In] hfu_handle : HFU����handle id
//             [Out] None
//             [Return] TRUE: ȥע��ɹ� ; FALSE: ȥע��ʧ��
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_unregister(uint32 hfu_handle);

/*****************************************************************************/
//  Description: ��ȡHFU����״̬
//  Parameter: [In] None
//             [Out] None
//             [Return] �����¼�����Ľ��
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_get_connect_status(void);

/*****************************************************************************/
//  Description: ����DTMF
//  Parameter: [In] dtmf_ch :  �����͵ĵ���DTMF�ַ�
//             [Out] None
//             [Return] �����¼�����Ĵ�������
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_send_dtmf(uint8 dtmf_ch);

/*****************************************************************************/
//  Description: �������������Ӧ��
//  Parameter: [In] None
//             [Out] None
//             [Return] �����¼�����Ĵ�������
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_anwser_call(void);

/*****************************************************************************/
//  Description: ���ͺ��к���
//  Parameter: [In] p_phone_num :  �����͵ĺ���
//             [Out] None
//             [Return] �����¼�����Ĵ�������
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_make_call(const uint8 *p_phone_num);

/*****************************************************************************/
//  Description: ��ͨͨ���ҶϹ���
//  Parameter: [In] None
//             [Out] None
//             [Return] �����¼�����Ĵ�������
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_release_call(void);

/*****************************************************************************/
//  Description: ȫ��ͨ���ҶϹ���
//  Parameter: [In] None
//             [Out] None
//             [Return] �����¼�����Ĵ�������
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_release_allcall(void);

/*****************************************************************************/
//  Description: ���ص�ǰͨ����Ϣ
//  Parameter: [In] p_phone_num : ��ǰͨ���ĺ���
//             [In] p_num_len����ǰͨ������ĳ���
//             [Out] None
//             [Return] TRUE: ��ȡ�ɹ� ; FALSE: ��ȡʧ��
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean  ual_bt_get_call_info( 
                                uint8 *p_phone_num,
                                uint8 *p_num_len
                                );
/*****************************************************************************/
//  Description: ���ص�ǰͨ����ʼʱ��
//  Parameter: [In/Out] p_stat_time : ��ǰͨ���Ŀ�ʼʱ��
//             [Return] TRUE: ��ȡ�ɹ� ; FALSE: ��ȡʧ��
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_get_current_call_start_time( uint32 *p_stat_time );

/*****************************************************************************/
//  Description : get whether exist bt call
//  Parameter: [In] None
//             [Out] None
//             [Return] s_bt_call_flag
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_get_bt_call_flag(void);

/*****************************************************************************/
//  Description : set whether exist  bt call
//  Parameter: [In] bt_call_flag
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
void ual_bt_set_bt_call_flag(boolean bt_call_flag);

/*****************************************************************************/
//  Description : set whether is exist incoming call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : �������ں���ĵ绰 ; FALSE : ������
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_incoming_call(void);

/*****************************************************************************/
//  Description : set whether is exist outgoing call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : ��������ͨ�������ĵ绰 ; FALSE : ������
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_outgoing_call(void);

/*****************************************************************************/
//  Description : set whether is exist active call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : ��������ͨ���ĵ绰 ; FALSE : ������
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_active_call(void);

/*****************************************************************************/
//  Description : set whether is exist active call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : �������ڱ��ֵĵ绰 ; FALSE : ������
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_hold_call(void);

/*****************************************************************************/
//  Description : set whether is exist mpty call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : ���ڻ���绰 ; FALSE : ������
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_mpty_call(void);

/*****************************************************************************/
//  Description : return current index
//  Parameter: [In] None
//             [Out] None
//             [Return] current index
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
uint8 ual_bt_get_current_index(void);

/*****************************************************************************/
//  Description : ��������ͨ������
//  Parameter: [In] volume
//             [In] type LOCAL:ֻ���ñ��� BOTH:ͬʱ���ñ��˺�Զ��
//             [Out] None
//             [Return] �ɹ�/ʧ��
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_set_volume( ual_bt_hfu_aud_type_e type, uint32 volume );

/*****************************************************************************/
//  Description : ��ȡ����ͨ������
//  Parameter: [In] None
//             [Out] None
//             [Return] ����ͨ������
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
uint32 ual_bt_hfu_get_volume( void );

/*****************************************************************************/
//  Description : ��������ͨ������
//  Parameter: [In] volume
//             [In] type LOCAL:ֻ���ñ��� BOTH:ͬʱ���ñ��˺�Զ��
//             [Out] None
//             [Return] �ɹ�/ʧ��
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_set_mute( ual_bt_hfu_aud_type_e type, boolean is_mute );

/*****************************************************************************/
//  Description : ��ȡ����ͨ���Ƿ���
//  Parameter: [In] None
//             [Out] None
//             [Return] ����ͨ���Ƿ���
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_get_is_mute( void );

/*****************************************************************************/
//  Description : �л���Ƶ����ͨ�� ����/Զ��
//  Parameter: [In] direction
//             [Out] None
//             [Return] �л��Ƿ�ɹ�
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_trans_aud_conn( ual_bt_hfu_aud_trans_e direction );

/*****************************************************************************/
//  Description : ��ǰͨ�������Ƿ��ڱ��˲���
//  Parameter: [In] None
//             [Out] None
//             [Return] ��ǰͨ�������Ƿ��ڱ��˲���
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_aud_is_conn( void );

/*****************************************************************************/
//  Description:ʵ��HFU���ӹ���
//  Parameter: [In] p_addr : bt dialer ��ַ
//             [Out] None
//             [Return] �����¼�����Ĵ�������
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_hfu_connect ( uint8 *p_addr );

/*****************************************************************************/
//  Description: ʵ��HFUȥ���ӹ���
//  Parameter: [In] None
//             [Out] None
//             [Return] �����¼�����Ĵ�������
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_hfu_disconnect( void );

/*****************************************************************************/
//  Description : return total call num
//  Parameter: [In] None
//             [Out] None
//             [Return] total call num
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
uint8 ual_bt_get_total_call_num(void);

#endif
