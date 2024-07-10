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
** File Name:       ual_audioplayer.h                                        *
** Author:            qi.liu1                                                *
** Date:            10/21/2021                                               *
** Description:    This file is used to define ual audioplayer export        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 10/21/2021          qi.liu1                  Create                       *
******************************************************************************/
#ifndef _UAL_AUDIOPLAYER_H_
#define _UAL_AUDIOPLAYER_H_
#include "ual_type.h"
#include "ual_cms.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
typedef  uint32 ual_audioplayer_handle;

#define  AUDIOPLAYER_INVALID_HANDLE 0xFFFFFFFF

#define UAL_AUDIOPLAYER_YEAR_LEN    10   //��� size
#define UAL_AUDIOPLAYER_ARTIST_LEN  32   //������ size
#define UAL_AUDIOPLAYER_ALBUM_LEN   32   //ר���� size
#define UAL_AUDIOPLAYER_TITLE_LEN   32   //������ size
#define UAL_AUDIOPLAYER_TRACK_LEN   32   //������ size
#define UAL_AUDIOPLAYER_GENRE_LEN   32   //������ size

//ual audioplayer ��app�ϱ�����Ϣ
typedef enum
{
    UAL_AUDIOPLAYER_MSG_AUDIO_SUSPEND = UAL_MODULE_AUDIOPLAYER << 16,  //AUDIO���񱻹���֪ͨ   //bug2058023
    UAL_AUDIOPLAYER_MSG_AUDIO_RESUME,  //AUDIO����ָ�֪ͨ
    UAL_AUDIOPLAYER_MSG_AUDIO_ROUTE_CHANGED, //��Ƶ����ͨ���仯֪ͨ
    //������������Ϣ
    UAL_AUDIOPLAYER_MSG_AUDIO_DATA_IND, //��������Ϣ�ڶ��׶�
    UAL_AUDIOPLAYER_MSG_DOWNLOAD_PROGRESS,  //������Ϣ
    //����
    UAL_AUDIOPLAYER_MSG_PLAY_IND, //AUDIO PLAYER �ڲ���play����
    UAL_AUDIOPLAYER_MSG_PLAY_END, //���Ž���
    UAL_AUDIOPLAYER_MSG_PLAY_PROGRESS,  //���Ž���
    //��ͣ
    UAL_AUDIOPLAYER_MSG_PAUSE_CNF,
    UAL_AUDIOPLAYER_MSG_PAUSE_IND,
    //����
    UAL_AUDIOPLAYER_MSG_BUFFERING_IND,
    UAL_AUDIOPLAYER_MSG_BUFFERING_END,
    //ֹͣ
    UAL_AUDIOPLAYER_MSG_STOP_CNF, //�ڶ��׶�
    UAL_AUDIOPLAYER_MSG_STOP_IND,
    //�ָ�
    UAL_AUDIOPLAYER_MSG_RESUME_IND,
    //ý����Ϣ
    UAL_AUDIOPLAYER_MSG_UPDATE_MEDIA_INFO, //����ý����Ϣ
    //ϵͳ�쳣���
    UAL_AUDIOPLAYER_MSG_ERR, //�����쳣���洢�ռ���
    UAL_AUDIOPLAYER_MSG_NET_NOT_CONNECTED,   //����δ����
    UAL_AUDIOPLAYER_MSG_NET_TIMEOUT,         //�������ӳ�ʱ
    UAL_AUDIOPLAYER_MSG_NET_CONNECT_ERR,    //��������ʧ��
    UAL_AUDIOPLAYER_MSG_SRC_FORMAT_NOT_SUPPORT,   //��ƵԴ��ʽ��֧��
    UAL_AUDIOPLAYER_MSG_MAX
}ual_audioplayer_msg_e;

//������app���������ԭ��
typedef enum
{
    UAL_AUDIOPLAYER_ERR_NONE,
    UAL_AUDIOPLAYER_PARAM_ERROR,
    UAL_AUDIOPLAYER_NO_ENOUGH_MMEMORY,
    UAL_AUDIOPLAYER_SET_VOLUME_FAIL,
    UAL_AUDIOPLAYER_SET_ROUTE_FAIL,
    UAL_AUDIOPLAYER_GET_PLAY_PROGRESS_FAIL,
    UAL_AUDIOPLAYER_PLAY_FAIL,
    UAL_AUDIOPLAYER_STOP_FAIL,
    UAL_AUDIOPLAYER_RELEASE_FAIL,
    UAL_AUDIOPLAYER_PAUSE_FAIL,
    UAL_AUDIOPLAYER_RESUME_FAIL,
    UAL_AUDIOPLAYER_SEEK_FAIL,
    UAL_AUDIOPLAYER_UNKNOW_ERROR,
    UAL_AUDIOPLAYER_CREATE_FAIL,
    UAL_AUDIOPLAYER_ERR_MAX
}ual_audioplayer_err_e;

//��Ƶͨ·������
typedef enum
{
    UAL_AUDIOPLAYER_ROUTE_NONE,
    UAL_AUDIOPLAYER_ROUTE_AUTO = UAL_AUDIOPLAYER_ROUTE_NONE,
    UAL_AUDIOPLAYER_ROUTE_BLUETOOTH = (0x0001),     //����ͨ·
    UAL_AUDIOPLAYER_ROUTE_EARPHONE =  (0x0001<<1),  //����ͨ·
    UAL_AUDIOPLAYER_ROUTE_EARFREE =   (0x0001<<2),  //���������ͨ·
    UAL_AUDIOPLAYER_ROUTE_SPEAKER =   (0x0001<<3),  //���ͨ·
    UAL_AUDIOPLAYER_ROUTE_HANDHOLD =  (0x0001<<4),  //call and key pad����ͨ·
    UAL_AUDIOPLAYER_ROUTE_MAX
}ual_audioplayer_route_e;

//audio player״̬����״̬
typedef enum
{
    UAL_AUDIOPLAYER_STATE_IDLE,
    UAL_AUDIOPLAYER_STATE_READY,
    UAL_AUDIOPLAYER_STATE_SOURCE_DATA_PREPARE,
    UAL_AUDIOPLAYER_STATE_PLAY_WAITING,
    UAL_AUDIOPLAYER_STATE_BUFFERING,
    UAL_AUDIOPLAYER_STATE_PLAYING,
    UAL_AUDIOPLAYER_STATE_PAUSE,
    UAL_AUDIOPLAYER_STATE_DESTROYING,
    UAL_AUDIOPLAYER_STATE_MAX
}ual_audioplayer_state_e;

//seek ��ʽ
typedef enum
{
    UAL_AUDIOPLAYER_SEEK_TYPE_BYTE,  //���ֽڷ�ʽ��ת
    UAL_AUDIOPLAYER_SEEK_TYPE_TIME,  //��ʱ�䷽ʽ��ת
    UAL_AUDIOPLAYER_SEEK_TYPE_MAX
}ual_audioplayer_seek_e;


//play�ӿڵ���νṹ��
typedef struct
{
    wchar*  p_audio_source; //������Ƶ·��������url·��
    uint32  audio_source_len;
    uint32  offset_time;    //������ʼʱ��
}ual_audioplayer_play_param_t;

//seek�ӿ����
typedef struct
{
    uint32  seek_pos;
    uint32  seek_time;  //��λ:s
    ual_audioplayer_seek_e  seek_type;
}ual_audioplayer_seek_param_t;

//���Ž���,��λ(��/s)
typedef struct
{
    uint32  cur_time;    //��ǰ���ų���
    uint32  total_time;  //�ܳ���
}ual_audioplayer_play_progress_info_t;

//���ؽ���
typedef struct
{
    uint32  cur_recv_data_length;  //��ǰ�յ��ĳ���
    uint32  total_data_length;     //�����ܳ���
    uint32  download_time;         //����ƫ��,��λ(��/s)
}ual_audioplayer_download_progress_info_t;

//ý����Ϣ
typedef struct
{
    uint32 total_time;
    uint32  samplerate;    //������
    uint32  channel_num;   //ͨ��
    uint32  bitrate;       //������
    wchar   year[UAL_AUDIOPLAYER_YEAR_LEN+1];     //��
    wchar   artist[UAL_AUDIOPLAYER_ARTIST_LEN+1]; //����
    wchar   album[UAL_AUDIOPLAYER_ALBUM_LEN+1];   //ר��
    wchar   title[UAL_AUDIOPLAYER_TITLE_LEN+1];   //������
    wchar   track[UAL_AUDIOPLAYER_TRACK_LEN+1];   //����
    wchar   genre[UAL_AUDIOPLAYER_GENRE_LEN+1];   //����
    uint8*  p_apic_info;    //ר��ͼƬ
    uint32  apic_info_size;//ר��ͼƬ���ݴ�С
}ual_audioplayer_media_info_t;

//���ص�֡����
typedef struct
{
    uint8*  p_data_buffer;  //������������DATA
    uint16  data_size;      //������������SIZE
}ual_audioplayer_download_data_info_t;

//play�ӿڵ���νṹ��
typedef struct
{
    ual_audioplayer_route_e cur_route;
}ual_audioplayer_route_info_t;

//�ص������Ĳ����ṹ
typedef struct
{
    ual_audioplayer_handle handle;
    void *param_ptr;
    uint32 size_of_param;
} ual_audioplayer_callback_param_t;

/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: ��ʼ��ual audioplayer
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: zirui.li
//  Note:����ʱ���ô˺�������ual audioplayer�ĳ�ʼ��
/*****************************************************************************/
void ual_audioplayer_init(void);

/*****************************************************************************/
//  Description: ����audio player Service �Ľӿ�
//  Parameter: [In] p_callback //ע��Ļص�����
//             [Out] p_handle //������Audio Player��handle
//             [Return] ual_audioplayer_err_e //�������
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_create(_ual_cms_client_register_callback p_callback,
                                                 ual_audioplayer_handle* p_handle);

/*****************************************************************************/
//  Description: ����audio player Service �Ľӿ�
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //���ٽ��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_destroy(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: ���ֲ��������Žӿ�
//  Parameter: [In] handle //���ֲ�������handle
//             [In] p_play_param //���Ų���
//             [Out] None
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_play(ual_audioplayer_handle handle,
                                               ual_audioplayer_play_param_t* p_play_param);

/*****************************************************************************/
//  Description: ���ֲ�������ͣ�ӿ�
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_pause(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: ���ֲ������ָ��ӿ�
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_resume(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: ���ֲ�����ֹͣ�ӿ�
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_stop(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: ���ֲ�����seek�ӿ�
//  Parameter: [In] handle //���ֲ�������handle
//             [In] p_seek_param //seek����
//             [Out] None
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_seek(ual_audioplayer_handle handle,
                                                ual_audioplayer_seek_param_t* p_seek_param);

/*****************************************************************************/
//  Description: ��ȡ��ǰaudio player service ��״̬
//  Parameter: [In] audioplayer_handle //���ֲ�������handle
//             [Out] None
//             [Return] ual_audioplayer_state_e //��ȡ�ĵ�ǰ״̬
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_state_e ual_audioplayer_get_state(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: ��ȡ���ֲ������Ĳ��Ž���
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] p_progress_info //���Ž�����Ϣ
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_get_play_progress_info(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_play_progress_info_t* p_progress_info);

/*****************************************************************************/
//  Description: ��ȡ���ֲ�������ý����Ϣ
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] p_media_info //ý����Ϣ
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_get_media_info(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_media_info_t* p_media_info);

/*****************************************************************************/
//  Description: ��ȡ��ǰ���صĽ�����Ϣ/�������
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] p_progress_info //���������Ϣ
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_get_download_progress_info(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_download_progress_info_t* p_progress_info);

/*****************************************************************************/
//  Description: ��������
//  Parameter: [In] handle //���ֲ�������handle
//             [In] volume //����ֵ
//             [Out] None
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_set_volume(
                                               ual_audioplayer_handle handle,
                                               uint32 volume);

/*****************************************************************************/
//  Description: ��ȡ����
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] None
//             [Return] uint32 //��ȡ������ֵ
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
uint32 ual_audioplayer_get_volume(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: ͨ·�л�
//  Parameter: [In] handle //���ֲ�������handle
//             [In] volume //����ֵ
//             [Out] None
//             [Return] ual_audioplayer_err_e //�ӿڵ��ý��
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_set_route(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_route_info_t audio_route);

/*****************************************************************************/
//  Description: ��ȡ��ǰͨ·
//  Parameter: [In] handle //���ֲ�������handle
//             [Out] None
//             [Return] ual_audioplayer_route_e //��ȡ�ĵ�ǰͨ·
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_route_e ual_audioplayer_get_route(
                                               ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: �������е�ʵ��
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
void ual_audioplayer_destroy_all_instances(void);

/*****************************************************************************/
//  Description: �ָ���������
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
void ual_audioplayer_restore_factory(void);
#endif
