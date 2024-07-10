/*****************************************************************************
** File Name:      mmisrvap_export.h                                         *
** Author:                                                                   *
** Date:           11/07/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio file srv fsm                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012       robert.wang       Create                                    *
******************************************************************************/
#ifndef _MMISRVAP_EXPORT_H_
#define _MMISRVAP_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"

#include "mmisrvaud_api.h"
#include "mmisrv_fsm.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMISRVAP_TRACE_LOW SCI_TRACE_LOW



/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef FSMSRV_HANDLE MMISRVAP_HANDLE;

typedef enum
{
    MMISRVAP_ERROR_NONE,
    MMISRVAP_ERROR_PARAM,
    MMISRVAP_ERROR_NOT_ENOUGH_MEMORY,
    MMISRVAP_ERROR_FILE_FORMAT_NOT_SUPPORT,               //��ʽ��֧��
    MMISRVAP_ERROR_REQ, //����ʧ��
    MMISRVAP_ERROR_UNKNOWN, //δ֪����
    MMISRVAP_ERROR_MAX
}MMISRVAP_ERROR_E;

typedef enum
{
    MMISRVAP_EVENT_OPEN,
    MMISRVAP_EVENT_PLAYING,
    MMISRVAP_EVENT_PLAY_END,
    MMISRVAP_EVENT_PAUSE,
    MMISRVAP_EVENT_RESUME,
    MMISRVAP_EVENT_SEEK,
    MMISRVAP_EVENT_STOP,
    MMISRVAP_EVENT_EXT, //such as BT event
#ifdef BT_DIALER_SUPPORT
    MMISRVAP_EVENT_EARPHONE, // earphone event
#endif
    MMISRVAP_EVENT_REVIEW_START,
    MMISRVAP_EVENT_REVIEW_STOP,
    MMISRVAP_EVENT_REVIEW_PLAY_END, //review ����Ĳ��Ž����¼�
    MMISRVAP_EVENT_PERIOD_TIMER,
    MMISRVAP_EVENT_TO_DO_PAUSE, //notify caller to do
    MMISRVAP_EVENT_TO_DO_RESUME, //notify caller to do
    MMISRVAP_EVENT_MGR_RESUME,
    MMISRVAP_EVENT_UPDATE_AUDIO_HANDLE, //update audio handle
    MMISRVAP_EVENT_MAX
}MMISRVAP_EVENT_E;

typedef enum
{
    MMISRVAP_PLAYING_NONE, //δ����
    MMISRVAP_PLAYING_OK, //���ųɹ�
    MMISRVAP_PLAYING_PENDING, //��������
    MMISRVAP_PLAYING_MAX
}MMISRVAP_PLAYING_STATE_E;

typedef struct
{
    uint32 start_point; //��ʼλ��,����Ϊ0
    uint32 end_point; //����λ��, ����Ϊ0
}MMISRVAP_REQ_REVIEW_PARAM_T;

typedef struct
{
    wchar*                  full_path_ptr;      //�����ļ��ľ���·��(�ӱ����ļ�����ʱ��Ч)
    uint16                  full_path_len;      //�����ļ��ľ���·������(�ӱ����ļ�����ʱ��Ч)
    MMISRVAUD_VOLUME_T      vol;                 //����
    BOOLEAN                 is_support_bt; //�Ƿ�֧����������
    uint32                  period_time; //����ʱ��
    uint32                  offset_point; //�������
    MMISRVAUD_RING_FMT_E audio_type; //audio type, maybe drm audio type

    MMISRVAP_REQ_REVIEW_PARAM_T review_param; //review
}MMISRVAP_REQ_PLAY_PARAM_T;

typedef struct
{
    uint32 offset;
}MMISRVAP_REQ_SEEK_PARAM_T;


typedef struct
{
    BOOLEAN is_end_file; //�Ƿ����ļ�����
    uint32 total_time; //�ļ���ʱ��
    uint32 total_length; //�ļ��ܳ���
}MMISRVAP_CALLBACK_REVIEW_PLAY_END_PARAM_T;

typedef struct
{
    MMISRVAUD_BT_EVENT_E bt_event;
}MMISRVAP_CALLBACK_BT_PARAM_T;

typedef struct
{
    MMISRVAP_PLAYING_STATE_E play_ret; //���Ž��
}MMISRVAP_CALLBACK_PLAYING_PARAM_T;

//�ص������Ĳ����ṹ
typedef struct
{
    MMISRVAP_EVENT_E event_id; //�¼�ID
    MMISRVAP_HANDLE handle;
    MMISRVAP_ERROR_E error_code;
    void *param_ptr;
    uint32 size_of_param;
} MMISRVAP_CALLBACK_PARAM_T;

typedef void (*MMISRVAP_CALLBACK) (MMISRVAP_CALLBACK_PARAM_T *callback_param_ptr, uint32 size_of_callback_param);//״̬����ui������Ϣ�Ļص�����

typedef struct
{
    MMISRVAUD_VOLUME_T      vol;                 //����
    BOOLEAN                 is_support_bt; //�Ƿ�֧����������

    MMISRVAP_CALLBACK callback_func; //ap srv ֪ͨ caller�Ļص�����
}MMISRVAP_OPEN_PARAM_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init module
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_InitModule(void);

/*****************************************************************************/
//  Description : ����audio file srv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_HANDLE MMISRVAP_Open(
    MMISRVAP_OPEN_PARAM_T *open_param_ptr //in
);
/*****************************************************************************/
//  Description : MMISRVAP_ResetReviewInfo
//  Global resource dependence :  none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_ResetReviewInfo(
	MMISRVAP_HANDLE ap_handle //in
);
/*****************************************************************************/
//  Description : �ر�audio file srv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_Close(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : �����ļ�
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Play(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_PLAY_PARAM_T *play_param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : ��ͣ����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Pause(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : �ָ�����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Resume(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : ����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SeekTo(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_SEEK_PARAM_T *seek_param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : ֹͣ����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Stop(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : ��ò����е���Ϣ
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_GetPlayingInfo(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAUD_PLAY_INFO_T *playing_info_ptr //out
);

/*****************************************************************************/
//  Description : ��������
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SetVolume(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAUD_VOLUME_T vol //in
);

/*****************************************************************************/
//  Description : ���ø���λ��
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SetReviewPoint(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_REVIEW_PARAM_T *review_param_ptr //in
);

/*****************************************************************************/
//  Description : ��������
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_StartReview(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : ֹͣ����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_StopReview(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : mgr resume
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: ������handle���ڷǼ���״̬��
//       �ñ�handle���ڼ���״̬
//    1) ���Ǳ�audio srv��ͣ��,����resume����
//    2) ��δ��audio srv��ͣ������ԭ��״̬
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_MGR_Resume(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : request audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_RequestAudioHandle(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : release audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_ReleaseAudioHandle(
    MMISRVAP_HANDLE ap_handle //in
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif




