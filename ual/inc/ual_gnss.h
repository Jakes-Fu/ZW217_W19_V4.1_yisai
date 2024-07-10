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
** File Name:       ual_gnss.h                                               *
** Author:           zirui.li                                                *
** Date:            09/07/2022                                               *
** Description:    This file is used to define ual gnss export               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 09/07/2022         zirui.li                  Create                       *
******************************************************************************/
#ifndef _UAL_GNSS_H
#define _UAL_GNSS_H
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ual_type.h"
#include "os_api.h"
#include "ual_cms.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
/*nmea���ݰ�����󳤶�*/
#define UAL_GNSS_NMEA_DATA_MAX_SIZE  3072
/*һ�����������ǵ��������*/
#define UAL_GNSS_SATELLITE_MAX_NUM  32
/*---------------------------------------------------------------------------*/
/*                          ENUM AND STRUCT                                  */
/*---------------------------------------------------------------------------*/

/*��λģʽ*/
typedef enum
{
    UAL_GNSS_MODE_NONE,
    UAL_GNSS_MODE_GPS,                     //ȫ��λϵͳ
    UAL_GNSS_MODE_BDS,                     //�������ǵ���ϵͳ
    UAL_GNSS_MODE_GPS_BDS,                //ȫ��λϵͳ�ͱ������ǵ���ϵͳ
    UAL_GNSS_MODE_GLONASS,                //������˹���ǵ���ϵͳ
    UAL_GNSS_MODE_GPS_GLONASS,            //ȫ��λ�͸�����˹����
    UAL_GNSS_MODE_GPS_BDS_GALILEO = 19,   //ȫ��λ���������ǡ�٤��������
    UAL_GNSS_MODE_GPS_B1C_GLONASS_GALILEO = 29,  //��ģ:ȫ��λ���������ǡ�٤�������ǡ�������˹����
    UAL_GNSS_MODE_MAX
}ual_gnss_mode_e;

/*����ģʽ*/
typedef enum
{
    UAL_GNSS_START_MODE_HOT,               //������
    UAL_GNSS_START_MODE_COLD,              //������
    UAL_GNSS_START_MODE_WARM,              //������
    UAL_GNSS_START_MODE_FACTORY,           //����ģʽ
    UAL_GNSS_START_MODE_MAX
}ual_gnss_start_mode_e;

/*NMEA��ʽ����*/
typedef enum
{
    UAL_GNSS_NMEA_NONE_TYPE = 0,         //disable output
    UAL_GNSS_NMEA_GGA_TYPE = 0x01,       //GGS
    UAL_GNSS_NMEA_GLL_TYPE = 0x02,       //GLL
    UAL_GNSS_NMEA_GSA_TYPE = 0x04,       //GSA
    UAL_GNSS_NMEA_GSV_TYPE = 0x08,       //GSV
    UAL_GNSS_NMEA_RMC_TYPE = 0x10,       //RMC
    UAL_GNSS_NMEA_VTG_TYPE = 0x20,       //VTG
    UAL_GNSS_NMEA_ALL_TYPE = 0x3F        //������������
}ual_gnss_nmea_type_e;

/*ual gnss״̬����״̬*/
typedef enum
{
    UAL_GNSS_STATE_IDLE,    //IDLE״̬
    UAL_GNSS_STATE_READY,   //READY״̬����ʼ�����
    UAL_GNSS_STATE_WORK,    //work״̬�����ڶ�λ
    UAL_GNSS_STATE_MAX
}ual_gnss_state_e;

/*ual gnss���ϲ��ϱ��Ĵ�����*/
typedef enum
{
    UAL_GNSS_RES_SUCCESS,             //�ɹ�
    UAL_GNSS_RES_PARAM_ERR = UAL_MODULE_GNSS << 16,    //��������
    UAL_GNSS_RES_REGISTER_CMS_FAIL,    //ע��cmsʧ��
    UAL_GNSS_RES_UNREGISTER_CMS_FAIL,  //ȥע��cmsʧ��
    UAL_GNSS_RES_CREATE_FSM_FAIL,     //����״̬��ʧ��
    UAL_GNSS_RES_DESTROY_FSM_FAIL,     //����״̬��ʧ��
    UAL_GNSS_RES_STATE_ERR,           //״̬����(Я����ǰ״̬)
    UAL_GNSS_RES_HAVE_DONE,           //��Ӧ�������Ѿ������������
    UAL_GNSS_RES_OTHER_ERR,           //��������
    UAL_GNSS_RES_MAX
}ual_gnss_result_e;

/*�ⲿģ��ע��ĺ�������*/
typedef boolean (*ual_gnss_injection_func) (uint32 event, void* p_param, uint32 param_size);

/*ual gnss���ϲ��ϱ�����Ϣ*/
typedef enum
{
    /*����ע�뺯��*/
    MSG_UAL_GNSS_START_REQ,              //������λ����
    MSG_UAL_GNSS_STOP_REQ,               //ֹͣ��λ����
    MSG_UAL_GNSS_POWEROFF_REQ,           //�رն�λ����

    /*������Ϣ*/
    MSG_UAL_GNSS_START_CNF = UAL_MODULE_GNSS<<16,    //������λ����(Я���ɹ���ʧ�ܵĲ���)����ʼ����
    MSG_UAL_GNSS_STOP_CNF,             //ֹͣ��λ����(Я���ɹ���ʧ�ܵĲ���)
    MSG_UAL_GNSS_POWEROFF_CNF,         //����͹��ķ���(Я���ɹ���ʧ�ܵĲ���)
    MSG_UAL_GNSS_SET_AGNSS_DNS_CNF,    //����AGNSS��������
    MSG_UAL_GNSS_SET_GNSS_MODE_CNF,    //���ö�λģʽ����(Я���ɹ���ʧ�ܵĲ���)
    MSG_UAL_GNSS_READ_INFO_CNF,        //��ȡλ����Ϣ����(Я����λ����)
    MSG_UAL_GNSS_OUTPUT_NMEA_CNF,  //�������NMEA���ݷ���(Я���ɹ���ʧ�ܵĲ���)
    MSG_UAL_GNSS_DEL_AID_DATA_CNF,     //��ն�λ���ݷ���(Я���ɹ���ʧ�ܵĲ���)

    /*�����ϱ�����Ϣ*/
    MSG_UAL_GNSS_SET_STARTMODE_IND,         //Ŀǰ����֧�ֵ�����ģʽ(Я����ǰ����������ģʽ)
    MSG_UAL_GNSS_FIX_IND,                  //��λ�ɹ�
    MSG_UAL_GNSS_OUTPUT_NMEA_IND,         //���NMEAλ����Ϣ(Я����λ����)
    MSG_UAL_GNSS_MAX
}ual_gnss_msg_e;

/*��ʼ��λ��Ҫ�����ò���*/
typedef struct
{
    //ual_gnss_mode_e gnss_mode;     //��λģʽ(�ڳ�ʼ��ʱĬ�϶�λģʽΪGNSS_GLONASS,���������Ȳ��ô˲�����
                                  //���Ҫ���ö�λģʽ�������ual_gnss_set_gnss_mode)
    ual_gnss_start_mode_e     start_mode;         //����ģʽ
}ual_gnss_start_param_t;

/*����NMEA������Ҫ�����ò���*/
typedef struct
{
    ual_gnss_nmea_type_e nmea_type;  //NMEA��ʽ
    uint32 time_interval;           //�ϱ�NMEA���ݵ�ʱ����(��λ:��)
}ual_gnss_nmea_output_param_t;

/*ʱ��*/
typedef struct
{
    uint16 year;
    uint16 month;
    uint16 day;
    uint16 hour;
    uint16 minute;
    uint16 second;
}ual_gnss_time_stamp_t;

typedef struct
{
   uint32 milliseconds;
   uint32 second;
}ual_gnss_system_time_t;

/*��λ��Ϣ*/
typedef struct
{
    float longitude;     // ����
    float latitude;      // γ��
    float altitude;      // �߶�
    ual_gnss_time_stamp_t utc_time;  // �����׼ʱ��
    uint32 ttff_time;    // �״ζ�λʱ��
    uint8 satellite_num; // ��������
    float speed;         // �ٶ�
    float course;        // ����
    uint16 clockdrift;   // ʱ��Ư��
    ual_gnss_system_time_t SystemTimeStamp; // RTC or any other system time
    uint32 m_nUTCTime;                  // second
    uint8 uncertaintySemiMajor;
    uint8 uncertaintySemiMinor;
    float Bearing;            // increments of 1 measured clockwise from the North.
    uint16 Direction;         // UP=0, Down=1
    float HorizontalVelocity; // m/s
    float VerticalVelocity;   // m/s
    uint32 fix_flag;
    float HorizontalAccuracy;  //ˮƽ����
    float PDOP; // position dilution of precision,λ�þ���(γ�ȡ����Ⱥ͸̵߳����ƽ���͵Ŀ�����ֵ)
    float HDOP; // horizontal dilution of precision,ˮƽ����
    float VDOP; // vertical dilution of precision,��ֱ����
}ual_gnss_gps_info_t;

/*������Ϣ*/
typedef struct
{
    uint16 satellite_identifier;  //���Ǳ�ʶ��
    uint16 cn0;                  //�����
    uint16 elevation;            //����
    uint16 azimuth;             //��λ��
    uint8 is_used;              //�Ƿ��õ�������
}ual_gnss_satellite_info_t;

/*������Ϣ�б�*/
typedef struct
{
    uint8  satellite_num;                             //��������
    ual_gnss_satellite_info_t  satellite_info[UAL_GNSS_SATELLITE_MAX_NUM];   //������Ϣ
}ual_gnss_satellite_info_list_t;

/*MSG_UAL_GNSS_READ_INFO_CNFЯ���Ĳ���*/
typedef struct
{
    ual_gnss_gps_info_t  gps_info;                          //��λ��Ϣ
    ual_gnss_satellite_info_list_t  satellite_list_info;   //������Ϣ�б�
}ual_gnss_read_info_cnf_t;

/*MSG_UAL_GNSS_OUTPUT_NMEA_INDЯ���Ĳ���*/
typedef struct
{
    ual_gnss_nmea_type_e type;                       //nmea��ʽ
    uint8 nmea_data[UAL_GNSS_NMEA_DATA_MAX_SIZE];   //nmea����
    uint16 length;                                 //nmea���ݳ���
}ual_gnss_output_nmea_ind_t;

/*MSG_UAL_GNSS_FIX_INDЯ���Ĳ���*/
typedef struct
{
    float longitude;                  //����
    float latitude;                   //γ��
    ual_gnss_time_stamp_t utc_time;  //����ͳһʱ��
    uint32 ttff_time;               //�״ζ�λʱ��
    uint16 cn0;                     //�����
}ual_gnss_fix_ind_t;

typedef struct
{
    boolean is_ok;   //��Ӧ�������Ƿ�ɹ�
}ual_gnss_download_cnf_t,
ual_gnss_start_cnf_t,
ual_gnss_stop_cnf_t,
ual_gnss_poweroff_cnf_t,
ual_gnss_output_nmea_cnf_t,
ual_gnss_set_gnss_mode_cnf_t,
ual_gnss_set_agnss_dns_cnf_t,
ual_gnss_del_aid_data_cnf_t;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: ע��callback�����ڼ���ual gnss�ϱ�����Ϣ
//  Parameter: [In] p_callback   //app��ual gnssע���callback���ڴ�callback�п��Խ���ual gnss�ϱ�����Ϣ
//             [Out] p_handle    //ע��õ���cms handle��ȥע���ʱ����Ҫ�����handle
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_register(_ual_cms_client_register_callback p_callback, uint32* p_handle);

/*****************************************************************************/
//  Description: callbackȥע�ᣬȥע����ټ���ual gnss�ϱ�����Ϣ
//  Parameter: [In] handle   //ual_gnss_registerʱ�������handle
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_unregister(uint32 handle);

/*****************************************************************************/
//  Description: ע�뺯��
//  Parameter: [In] p_func   //�ⲿģ����ual gnssע��ĺ���
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:�ⲿģ��ɵ��ô˽ӿ�ע�뺯����p_func��ual_gnss_start�ӿ��лᱻ����
/*****************************************************************************/
ual_gnss_result_e ual_gnss_inject_function(ual_gnss_injection_func p_func);

/*****************************************************************************/
//  Description:ȡ��ע�뺯��
//  Parameter: [In] none
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:�ⲿģ��ɵ��ô˽ӿ�ȡ��ע�뺯����������˽ӿں�ual_gnss_start�н������ٵ���ע�뺯��
/*****************************************************************************/
ual_gnss_result_e ual_gnss_uninject_function(void);

/*****************************************************************************/
//  Description: ual gnss��ʼ��
//  Parameter: [In] none
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:������ʱ�����
/*****************************************************************************/
ual_gnss_result_e ual_gnss_init(void);

/*****************************************************************************/
//  Description: ������λ
//  Parameter: [In] p_start_param     //��ʼ��λ��Ҫ����Ĳ���
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:��λ�ɹ��󣬻�ÿ��1s�ϱ�MSG_UAL_GNSS_OUTPUT_INFO_IND��Я������type��nmea����
//       �����Ҫ�޸�nmea type���ϱ�ʱ�������ɵ���ual_gnss_set_nmea_output�ӿ�
/*****************************************************************************/
ual_gnss_result_e ual_gnss_start(ual_gnss_start_param_t* p_start_param);

/*****************************************************************************/
//  Description: ֹͣ��λ
//  Parameter: [In] none
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_stop(void);

/*****************************************************************************/
//  Description: �رն�λ
//  Parameter: [In] none
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_poweroff(void);

/*****************************************************************************/
//  Description: ����AGNSS����
//  Parameter: [In] p_url      //url
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_set_agnss_dns(char* p_url);

/*****************************************************************************/
//  Description: ���ò��붨λ������ģʽ
//  Parameter: [In] gnss_mode   //��λģʽ
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_set_gnss_mode(ual_gnss_mode_e gnss_mode);

/*****************************************************************************/
//  Description: ��ȡ��λ��Ϣ
//  Parameter: [In] none
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_read_info(void);

/*****************************************************************************/
//  Description: �������NMEA����
//  Parameter: [In] p_param    //���õĲ���
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_output_nmea(ual_gnss_nmea_output_param_t* p_param);

/*****************************************************************************/
//  Description: �������������
//  Parameter: [In] none
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_del_aid_data(void);

/*****************************************************************************/
//  Description: ����libgps������signal
//  Parameter: [In] p_sig    //signal
//             [Out] none
//             [Return] ������
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_process_libgps_msg(void* p_sig);

/*****************************************************************************/
//  Description: ���ual gnss״̬���ĵ�ǰ״̬
//  Parameter: [In] none
//             [Out] none
//             [Return] ��ǰ״̬
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_state_e ual_gnss_get_cur_state(void);

/*****************************************************************************/
//  Description: ���cp mode
//  Parameter: [In] none
//             [Out] none
//             [Return] cp mode
//  Author: zirui.li
//  Note:
/*****************************************************************************/
int ual_gnss_get_cp_mode(void);
#endif //_UAL_GNSS_H
