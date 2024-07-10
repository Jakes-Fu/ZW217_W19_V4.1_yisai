/*****************************************************************************
** File Name:      mmivp_fsm.h                                                   *
** Author:                                                                   *
** Date:           2010/3/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/18      apple.zhang       Create
******************************************************************************/

#ifndef _MMIVP_FSM_H_
#define _MMIVP_FSM_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmivp_export.h"
#include "dal_player.h"
#include "mn_type.h"
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "strm_api.h"
#else
#include "mmivp_dummy.h"
#endif
#ifdef MMI_KING_MOVIE_SUPPORT
#include "video_security.h"
#endif
#include "mmisrvaud_api.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VP_SUPPORTED_MAX_HEIGHT       480    //��������Ƶ�����߶�
#define VP_SUPPORTED_MAX_WIDTH        640   //��������Ƶ�������
    
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   
//״̬�����յ���Ϣ
typedef enum
{
    VP_INIT_REQ = VP_MSG_MAX,       //�����ʼ�� 0x412c
        VP_STRM_INIT_CNF,           //streaming��ʼ������Ӧ��0x412d
        VP_RELEASE_REQ,             //�����ͷ�0x412e
        VP_PLAY_REQ,                //���󲥷�0x412f
        VP_STRM_PLAY_CNF,           //streaming��������Ӧ��0x4130
        VP_STOP_REQ,                //����ֹͣ0x4131
        VP_STRM_STOP_CNF,           //streamingֹͣ����Ӧ��0x4132
        VP_PAUSE_REQ,               //������ͣ0x4133
        VP_STRM_PAUSE_CNF,          //streaming��ͣ����Ӧ��0x4134
        VP_RESUME_REQ,              //����ָ�����0x4135
        VP_STRM_RESUME_CNF,         //streaming�ָ���������Ӧ��0x4136
        VP_SEEK_REQ,                //����λ����0x4137
        VP_STRM_SEEK_CNF,           //streaming��λ����Ӧ��0x4138
        VP_AUTH_RSP,                //�����Ȩ0x4139
        VP_AUTHIND_REQ,             //�����ȡ��Ȩ��Ϣ0x413a
        VP_STRM_CLOSE_CNF,          //streaming�ر�����Ӧ��0x413b
        VP_STRM_BUFFERING_IND,      //streaming���忪ʼ0x413c
        VP_STRM_BUFFER_END_IND,     //�������0x413d
        VP_STRM_DATA_TIMEOUT_IND,   //streaming��ȡ���ݳ�ʱ0x413e
        VP_STRM_GOODBYE_IND,        //streaming���Ž���0x413f
        VP_CALLBACK_END_IND,        //dplayer���Ž���0x4140
        VP_FRAME_END_IND,           //dplayerһ֡����    0x4141
        VP_PDP_ACTIVE_CNF,          //pdp����Ӧ�� 0x4142
        VP_PDP_DEACTIVE_IND,        //���������Ͽ�0x4143
        VP_STRM_SERVER_CLOSE_IND,   //streaming�������ر�0x4144
        VP_ENTRY_REQ,                       //0x4145
        VP_EXIT_REQ,                //0x4146
        VP_PLAY_NEXT_FILE,          //0x4147
        VP_START_SET_RATE,          //0x4148
        VP_STOP_SET_RATE,          //0x4149
        VP_BUFFER_PERCENT,           //0x414a
        VP_FRAME_END_NEED_DATA,     //0x414b
        VP_REQ_MSG_MAX              //0x414c
} VP_FSM_MSG_E; 

//���Ʋ����Ϣ�ṹ
typedef struct 
{
    VP_HANDLE           fsm_handle;     //״̬��
    VP_FSM_MSG_E        msg_id;         //��Ϣid
    uint32              msg_size;       //��Ϣ��С
    void                *msg_body;      //��Ϣ��
}VP_FSM_MSG_T;


//״̬����Ϣ�ṹ
typedef struct  
{
    DPLAYER_HANDLE              dplayer_handle;             //dplayer handle
    MMISRV_HANDLE_T             srv_handle;             //audio service handle
#ifdef MMI_KING_MOVIE_SUPPORT
    VPLAYER_SECURITY_HANDLE         sec_handle;        //SECURITY_HANDLE
    BOOLEAN  b_sprd_kmv;
#endif
    STRM_HANDLE                 strm_handle;                //streaming handle
    uint32                      task_id;                    //playing task id
    VP_STATE_E                  state;                      //״̬��״̬
    VP_PLAY_TYPE_E              play_type;                  //���ŷ�ʽ
    VP_SUPPORTED_STATUS_TYPE_E  support_type;               //֧�̶ֳ�
    VP_CALLBACK                 vpcallback;                 //ui�ص�����
    uint32                      process_time;               //��ǰ����ʱ��
    uint32                      total_time;                 //��ʱ��
    wchar*                      full_path_ptr;              //ý������(�����ļ�Ϊȫ·��)
    uint16                      full_path_len;              //ý�����ֳ��� 
    uint8                       *url_ptr;                   //���粥�ŵ�url
    VP_DISPLAY_PARAM_T          display_param;              //������ʾ����,��ʾ����Ϊ�߼�����
    BOOLEAN                     is_scal_up;                 //�Ƿ�����
    VP_REPEAT_PLAY_MODE_E       mode;                       //����ģʽ
    BOOLEAN                     is_random_play;             //�Ƿ����������
    uint8                       vol;                        //����
    MN_DUAL_SYS_E               dual_sys;                   //���粥��ʱʹ�õ�sim��
    BOOLEAN                     is_to_pause;                //ǿ����ͣ��־
    BOOLEAN                     is_to_resume;               //ǿ�ƻָ���־
    BOOLEAN                     is_to_exit;                 //ǿ��ֹͣ�ͷ�
    VP_PLAY_END_RESULT_E        has_play_end_reason;            //�Ѿ�������ԭ�������Ѿ����������ǻ�û�����ļ�����
    BOOLEAN                     is_save_history;            //�Ƿ񱣴���ʷ��¼
    BOOLEAN                     is_not_notify_ui;           //�Ƿ�֪ͨuiˢ�� falseΪ֪ͨ��trueΪ��֪ͨ
    uint32                      buf_percent;                 //����ٷֱ�
    BOOLEAN                     is_bt_support;
    BOOLEAN                     is_a2dp_support;
    BOOLEAN                     is_share_play;      //�����ر߲��� 
    BOOLEAN                     is_cr_video_layer;          //�Ƿ񴴽���ͼ���
    BOOLEAN			is_audio_coexist;   
    DPLAYER_IMAGE_DATA_T        image_data;           //ͼ�������
	VP_PLAYBACK_MODE_E          playback_mode;        //�����ļ����ŷ�ʽ
	BOOLEAN                     is_seek_able;        //�Ƿ����seek
}VP_FSM_T;


//״̬���б� ������չ��״̬��������Ŀǰ֧�ֵ���״̬��
typedef struct VP_FSM_LIST_tag
{
    VP_FSM_T                *fsm_info_ptr;       //״̬����Ϣ
    struct VP_FSM_LIST_tag   *next_fsm_ptr;     //��һ��״̬��
}VP_FSM_LIST_T;



typedef BOOLEAN    (*FSM_DISPATCH)(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);       //״̬����Ϣ���亯��
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
