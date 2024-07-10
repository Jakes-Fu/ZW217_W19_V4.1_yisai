/*****************************************************************************
** File Name:      mmivp_export.h                                                   *
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

#ifndef _MMIVP_EXPORT_H_
#define _MMIVP_EXPORT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_player.h"
#include "ffs.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "mmi_custom_define.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_appmsg.h"
#include "mmifmm_export.h"

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
    
#define VP_MAX_IP_LENGTH            15                       //ip��ַ����󳤶�
#define VP_DEFAULT_DIR              MMIMULTIM_DIR_MOVIE    //Ĭ���ļ���
#define VP_DEFAULT_DIR_LEN          MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE)  //Ĭ���ļ������Ƴ���

#define KM_DEFAULT_DIR              MMIMULTIM_DIR_KINGMOVIE    //��Ӱ���ļ���
#define KM_DEFAULT_DIR_LEN          MMIAPICOM_Wstrlen(MMIMULTIM_DIR_KINGMOVIE)  //��Ӱ���ļ������Ƴ���

////�ļ��ĺ������ 
typedef void (*VP_POST_PROGRESS)(wchar*  full_path_name,uint16 file_full_path_len,int32 process_time);


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   
//��������Ƶ����������Ƶ�ļ�֧�ֵĳ̶����͡�
typedef enum
{
    VP_SUPPORTED_AUDIO_ONLY,            //��֧����Ƶ���벥��
        VP_SUPPORTED_VIDEO_ONLY,        //��֧��ͼ����벥��
        VP_SUPPORTED_BOTH,              //���������ţ�ͼ��������Ƶ�����֧��
        VP_SUPPORTED_NEITHER,           //�޷����ţ�ͼ��������Ƶ�������֧��
        VP_SUPPORTED_OVER_SCALED,       //ƬԴ������ֱ��ʣ�����ƽ̨��֧�ַ�Χ ƬԴ������ֱ��ʣ���640*480
        VP_SUPPORTED_STATUS_TYPE_MAX    //ϵͳ����������ʹ��
} VP_SUPPORTED_STATUS_TYPE_E;  

//��Ƶ���������Ʋ�״̬����״̬
typedef enum
{
    VP_STATE_IDLE,              //����
        VP_STATE_READY,         //׼����
        VP_STATE_PDP_ACTIVING,  //PDP������
        VP_STATE_INIT,          //�ѳ�ʼ��
        VP_STATE_INIT_PAUSED_EX,  //�ѳ�ʼ�����ʱ����������ͣ״̬
        VP_STATE_INIT_AUTH,     //��֤��
        VP_STATE_PLAY_PENDING,  //����׼����
        VP_STATE_PLAY_AUTH,     //��֤��
        VP_STATE_PLAYING,       //������
        VP_STATE_BUFFERING,     //������
        VP_STATE_SEEKING,       //���Ŷ�λ��
        VP_STATE_PAUSING,       //��ͣ��
        VP_STATE_PAUSED,        //����ͣ
        VP_STATE_PAUSED_EX,     //��ͣ�����streaming��ͣ��playerֹͣ
        VP_STATE_PAUSED_SEEKING, //��ͣ�ж�λ����
        VP_STATE_RESUMING,      //�ָ���
        VP_STATE_STOPING,       //ֹͣ��
        VP_STATE_STOPED,        //��ֹͣ
        VP_STATE_CLOSING,       //�ر���
        VP_STATE_MAX            //����
} VP_STATE_E; 

//���Ʋ��UI�㷢�͵���Ϣ����
typedef enum
{
    VP_BT_STATE_NONE,                   //������
    VP_BT_STATE_OPEN_WATTING,           //�ȴ���
    VP_BT_STATE_PLAY,                   //������
    VP_BT_STATE_PAUSE,                   //������
    VP_BT_STATE_PAUSE_WATTING,         //�ȴ�pause
    VP_BT_STATE_RESUME,                 //�ָ�
    VP_BT_STATE_RESUME_WATTING,         //�ȴ�RESUME
    VP_BT_STATE_MAX                     //����
} VP_BT_STATE_E; 

//���Ʋ��UI�㷢�͵���Ϣ����
typedef enum
{
    VP_PREPARE_IND = MSG_VP_INTERNAL_MSG_BEGIN,         //0x4120��������׼����ϡ�UI������ݷ��ز�����ʾƬԴ����ʱ������Ϣ
        VP_PLAY_PROCESS_IND,    //0x4121���浱ǰ�Ĳ��Ž��ȣ�ÿ����һ֡���ᱨ����Ϣ��
        VP_BUFFERING_IND,       //���忪ʼ
        VP_BUFFER_END_IND,      //�������
        VP_BUFFER_PROCESS_IND,  //���浱ǰ�������
        VP_FATAL_ERROR_IND,     //0x4125���Ź����г�����������
        VP_AUTH_IND,            //��Ҫ��Ȩ��֤
        VP_PLAY_END_IND,        //������Ͻ���
        VP_STOP_IND,            //������ֹͣ
        VP_PLAY_CNF,            //��ʼ����
        VP_PAUSE_CNF,           //0x412a��ͣ
        VP_SEEK_CNF,            //��λ�ָ�
        VP_MSG_MAX              //0x412cϵͳ����������ʹ��
} VP_MSG_E; 


//���ŷ�ʽ
typedef enum
{
    VP_PLAY_FROM_NONE = 0,           //nothing
    VP_PLAY_FROM_FILE ,              //�ӱ����ļ�����
        VP_PLAY_FROM_BUFFER,        //�ӱ���BUFFER����
        VP_PLAY_FROM_NET,           //�����粥��
        VP_PLAY_FROM_LOCALLIST,     //�ӱ��ز����б��в���
        VP_PLAY_FROM_MAX            //����
} VP_PLAY_TYPE_E;

//ѭ������ģʽ����
typedef enum
{
    VP_REPEAT_PLAY_MODE_CLOSE = 0,      //�ر�ѭ������
        VP_REPEAT_PLAY_MODE_ALL,        //ȫ����Ŀѭ������
        VP_REPEAT_PLAY_MODE_ONE,        //������Ŀѭ������
        VP_REPEAT_PLAY_MODE_MAX         //����
} VP_REPEAT_PLAY_MODE_E;

#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
typedef enum
{
    VP_SHUFFLE_PLAY_MODE_OFF = 0,       //�ر����򲥷�
    VP_SHUFFLE_PLAY_MODE_ON,            //�����򲥷�
    VP_SHUFFLE_PLAY_MODE_MAX            //����
} VP_SHUFFLE_PLAY_MODE_E;
#endif
typedef enum
{
    VP_PLAY_END_NONE,
    VP_PLAY_END_END_SUCCESS,            //��������
        VP_PLAY_END_ERROR,              //�����������
        VP_PLAY_END_NOT_SUPPORT,        //������󣬸�ʽ��֧��
        VP_PLAY_END_STOPPED_BY_USER,    //�û�ȡ��
        VP_PLAY_END_RESULT_MAX          //����
} VP_PLAY_END_RESULT_E;

typedef enum
{
    VP_ENTRY_FROM_PLAYER,           //��������������
        VP_ENTRY_FROM_FILE,         //ֱ�Ӳ����ļ�
        VP_ENTRY_FROM_NET,
        VP_ENTRY_FROM_MAX           //����
} VP_ENTRY_E;


//������Ƶ���ŷ�ʽ����
typedef enum
{
    VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN = 0, //����Ƶͬ������
	VP_PLAYBACK_MODE_SCREEN_FLOW,        //��������������
    VP_PLAYBACK_MODE_MAX         //����
} VP_PLAYBACK_MODE_E;

typedef DPLAYER_DISPLAY_PARAM_T VP_DISPLAY_PARAM_T  ;       //��ʾ����
typedef uint32 VP_HANDLE;                               //Videoplayer control Init�󷵻ظ�UI��handler

//�ص������Ĳ����ṹ
typedef struct
{
    uint16                   msg_id; //��ϢID
    uint32                  handle; //�����initʱ���ظ�UI��handle��    
    void*                   para;   //��Ϣ�����Ĳ���
} VP_CALLBACK_T;

//���Ž���
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_PLAY_END_RESULT_E  result;   //videoplayer end reason
} VP_PLAY_END_IND_T;

//buffer����ʱ�����ݲ���
typedef struct
{
    uint8*                  video_buf_ptr;  //��ƵBUFFER���ݵ�ָ��
    uint32                  video_buf_len;  //��ƵBUFFER���ݵĳ���
    MMICOM_VIDEO_TYPE_E         type;           //��ƵBUFFER���ݵĸ�ʽ
} VP_PLAY_BUF_INFO_T;


//���Ų���
typedef struct
{
    VP_PLAY_TYPE_E          play_type;          //���ŷ�ʽ
    VP_REPEAT_PLAY_MODE_E   mode;               //����ģʽ    
    MN_DUAL_SYS_E           dual_sys;           //sim��
    wchar*                  full_path_ptr;      //�����ļ��ľ���·��(�ӱ����ļ�����ʱ��Ч)
    uint16                  full_path_len;      //�����ļ��ľ���·������(�ӱ����ļ�����ʱ��Ч)
    VP_PLAY_BUF_INFO_T      video_buf_info;     //��ƵBUFFER���ݲ���(�ӱ���BUFFER����ʱ��Ч)    
    VP_DISPLAY_PARAM_T      display_param;      //������ʾ����    
    BOOLEAN                 is_scal_up;         //�Ƿ�����
    BOOLEAN                 is_save_history;    //�Ƿ񱣴���ʷ��¼
    BOOLEAN                 is_random_play ;    //�Ƿ����������
    char                    *url_ptr;           //���粥�ŵ�URL��ַ(�����粥��ʱ��Ч)
    uint8                   vol;                 //����
    BOOLEAN                 is_bt_support;
    BOOLEAN                 is_a2dp_support;
    BOOLEAN                 is_share_play;      //�����ر߲���       
    BOOLEAN                 is_cr_video_layer;          //�Ƿ񴴽���ͼ���
    BOOLEAN			        is_audio_coexist;          
    //GUI_LCD_DEV_INFO        dev_layer_handle;           //ͼ����handle
} VP_PLAY_PARAM_T;

//��λ���Ų��� seek_time��seek_frame��������һ�����ɡ�ȫ'F'��ʾ������Ч
typedef struct
{
    uint32       seek_time; //��Ҫseek����ʱ���
    uint32       seek_frame; //��Ҫseek����֡��
} VP_SEEK_PARAM_T;


typedef void (*VP_CALLBACK) (VP_CALLBACK_T *msg_ptr);       //״̬����ui������Ϣ�Ļص�����

//�������ò���
typedef struct
{
    uint8                   brightness;                         //��������   
    uint8                   net_index[MMI_DUAL_SYS_MAX];         //�����������õ�index
    uint16                  lowest_port;  //RTP/RTCP��Ͷ˿�
    uint16                  highest_port; //RTP/RTCP��߶˿�    
    uint8                   agent_ip[VP_MAX_IP_LENGTH+1];       //����IP
    uint8                   agent_ip_length;
    uint16                  agent_port;   //����˿�
    BOOLEAN                 is_use_agent;                       //�Ƿ�ʹ�ô���
    BOOLEAN                 is_random_play;                     //�Ƿ����������
    VP_REPEAT_PLAY_MODE_E   mode;                               //����ģʽ
	VP_PLAYBACK_MODE_E      playback_mode;                      //�����ļ����ŷ�ʽ
} VP_NV_SETTING_T;
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define VP_Init                 MMIAPIVP_FSMInit
#define VP_Release              MMIAPIVP_FSMRelease
#define VP_Play                 MMIAPIVP_FSMPlay
#define VP_Stop                 MMIAPIVP_FSMStop
#define VP_Pause                MMIAPIVP_FSMPause
#define VP_Resume               MMIAPIVP_FSMResume
#define VP_CompulsivePause      MMIAPIVP_FSMCompulsivePause
#define VP_CompulsiveResume     MMIAPIVP_FSMCompulsiveResume
#define VP_Seek                 MMIAPIVP_FSMSeek
#define VP_SwitchPlayDevice     MMIAPIVP_FSMSwitchPlayDevice
#define VP_SwitchDisplayMode    MMIAPIVP_FSMSwitchDisplayMode
#define VP_SwitchAudioSupport   MMIAPIVP_FSMSwitchAudioSupport
#define VP_GetState             MMIAPIVP_FSMGetState
#define VP_Exit                 MMIAPIVP_FSMExit
#define VP_GetTimeOffset        MMIAPIVP_FSMGetTimeOffset
#define VP_GetFilerInfo         MMIAPIVP_GetFilerInfo
#ifdef JAVA_SUPPORT_SUN         //norflashcut
#define VP_GetFrameOffset       MMIAPIVP_FSMGetFrameOffset
#endif
#define VP_UpdateCurrentVol     MMIAPIVP_FSMUpdateCurrentVol
#define VP_FSMUpdatePlayMode    MMIAPIVP_FSMUpdatePlayMode
#ifdef MOBILE_VIDEO_SUPPORT     //norflashcut
#define VP_FSMGetCurBufPercent  MMIAPIVP_FSMGetCurBufPercent
#endif
#define MMIVP_TranslateDisplayParamLogicToPhysical  MMIAPIVP_TranslateDisplayParamLogicToPhysical
#define MMIVP_GetDefaultFolderPath                  MMIAPIVP_GetDefaultFolderPath
#define MMIVP_GetTransparenceColor                  MMIAPIVP_GetTransparenceColor
#define MMIVP_FillRectWithTransparenceColor         MMIAPIVP_FillRectWithTransparenceColor

#ifdef VIDEOTHUMBNAIL_SUPPORT
#define VP_ThumbnailOpen            MMIAPIVP_ThumbnailOpen
#define VP_ThumbnailClose           MMIAPIVP_ThumbnailClose
#define VP_ThumbnailGetVideo        MMIAPIVP_ThumbnailGetVideo
#define VP_ThumbnailGetAngle        MMIAPIVP_ThumbnailGetAngle
#define VP_IsThumbnailRunning       MMIAPIVP_IsThumbnailRunning
#define VP_IsThumbnailOpen          MMIAPIVP_IsThumbnailOpen
#define VP_ThumbnailResume          MMIAPIVP_ThumbnailResume
#define VP_ThumbnailVideoAdd        MMIAPIVP_ThumbnailVideoAdd
#define VP_ThumbnailStop            MMIAPIVP_ThumbnailStop
#define VP_ThumbnailSetAngle        MMIAPIVP_ThumbnailSetAngle
#define VP_ThumbnailSetDPLAYERParam MMIAPIVP_ThumbnailSetDPLAYERParam
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
//  Description : ����videoplayer״̬��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC  VP_HANDLE  MMIAPIVP_FSMInit(VP_CALLBACK vpcallback);

/*****************************************************************************/
//  Description : ȥ��PDP������videoplayer״̬��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMRelease(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : ������Ƶ���ţ����ػ����磩
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMPlay(VP_HANDLE   vp_handle, VP_PLAY_PARAM_T  *play_param_ptr);

/*****************************************************************************/
//  Description : ��ֹ��Ƶ���ţ����ػ����磩��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMStop(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : ��ͣ��Ƶ���ţ����ػ����磩��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMPause(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : �ָ���Ƶ���ţ����ػ����磩
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMResume(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : ǿ����ͣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: ����lose focus��ͣ������
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMCompulsivePause(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : ǿ�ƻָ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: ����get focus�ָ�������
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMCompulsiveResume(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : ���ж�λ��Ƶ���ţ����ػ����磩��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSeek(VP_HANDLE   vp_handle,
                        VP_SEEK_PARAM_T  *seek_param_ptr);

/*****************************************************************************/
//  Description : �л������豸
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchPlayDevice(VP_HANDLE  vp_handle,
                                   BOOLEAN    is_a2dp_support,
                                   BOOLEAN    is_bt_support);

/*****************************************************************************/
//  Description : �л���ʾģʽ��ȫ����������TVOUT)
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchDisplayMode(VP_HANDLE  vp_handle,
                                    VP_DISPLAY_PARAM_T  *display_param_ptr,
                                    BOOLEAN is_only_rect);

/*****************************************************************************/
//  Description : �л�����ģʽ 
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchAudioSupport(VP_HANDLE  vp_handle,
                                    BOOLEAN is_support);

/*****************************************************************************/
//  Description : ��ȡ��ǰ����״̬
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_STATE_E MMIAPIVP_FSMGetState(VP_HANDLE    vp_handle);

/*****************************************************************************/
//  Description : ִ���˳����̣�������״̬����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: ����һ���˳�������
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMExit(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : �����Ƶ���ŵĵ�ǰʱ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetTimeOffset(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : �ָ���ʼ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ResetSettingInfo(void);


#ifdef JAVA_SUPPORT_SUN//norflashcut
/*****************************************************************************/
//  Description : �����Ƶ���ŵĵ�ǰ֡����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetFrameOffset(VP_HANDLE  vp_handle);
#endif

/*****************************************************************************/
//  Description : ������Ƶ�ļ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Play (
                                VP_PLAY_PARAM_T  *play_param_ptr,
                                VP_CALLBACK        call_back_func,   
                                uint32                    start_time
                                );

/*****************************************************************************/
//  Description : �˳�������Ƶ�ļ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ExitPlay(void);

/*****************************************************************************/
//  Description : ͨ��ȫ·�������ý���ļ���Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_GetInfoFromFullPathName(
                                            const wchar            *full_file_name, 
                                            uint16                  full_name_len,
                                            DPLAYER_MEDIA_INFO_T    *media_ptr
                                            );

/*****************************************************************************/
//  Description : ͨ����Ƶbuf�л��ý���ļ���Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetMediaInfoFromBuf(
                                        uint8                   *video_buf,
                                        uint32                  video_length,
                                        MMICOM_VIDEO_TYPE_E         video_type,
                                        DPLAYER_MEDIA_INFO_T   *media_ptr
                                        );

/*****************************************************************************/
//  Description : �����ж���Ƶ�������Ը���Ƶ�ļ���֧�̶ֳ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_SUPPORTED_STATUS_TYPE_E MMIAPIVP_GetSupportedStatus(
                                                        wchar   *full_file_name, 
                                                        uint16    full_name_len
                                                        );

/*****************************************************************************/
//  Description : init vp module
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_InitModule(void);
/*****************************************************************************/
//  Description : init vp
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_Init(void);
/*****************************************************************************/
//  Description : ������Ƶ������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_MainEntry(void);

/*****************************************************************************/
//  Description : �˳���Ƶ������
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ExitVideoPlayer(void);

/*****************************************************************************/
//  Description : ������Ƶ�ļ�
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_E vp_entry,
                                  wchar *full_path_name_ptr,
                                  uint16 full_path_name_len,
                                  uint8 *url_ptr
                                  );
/*****************************************************************************/
//  Description : ������Ƶ�ļ�
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_PlayVideoFromVideoPlayerEx(VP_ENTRY_E vp_entry,
                                    wchar* full_path_name_ptr,
                                  uint16 full_path_name_len,
                                  uint8 *url_ptr,
                                  uint32 progress_time, //��ĳһ��ʱ��㿪ʼ����
                                  VP_POST_PROGRESS post_progrecss  //����ӿ� 
                                  );

/*****************************************************************************/
//  Description : MMIAPIVP_IsOpenPlayerWin
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsOpenPlayerWin(void);

#if 0//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_IsFocusPlayerWin
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsFocusPlayerWin(void);
#endif

/*****************************************************************************/
//  Description : notify vp stop
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_NotifyVPStop(void);

#if 0//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_HandleFileChange
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_HandleFileChange(void);
#endif

//add CR233033
/*****************************************************************************/
//  Description : get full path name of playing file
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetPlayingFilename(wchar * full_path_name_ptr);

#ifdef JAVA_SUPPORT_SUN//norflashcut
/*****************************************************************************/
//  Description : register current task for playing video
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_RegTaskID(VP_HANDLE vp_handle, uint32 task_id);
/*****************************************************************************/
//  Description : unregister current task for playing video
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_UnRegTaskID(VP_HANDLE vp_handle, uint32 task_id);
#endif


/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsLiving(void);

/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsPlayWinFocus(void);

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdateCurrentVol
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdateCurrentVol(VP_HANDLE vp_handle, uint8 vol);

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdatePlayMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdatePlayMode(VP_HANDLE vp_handle, 
                              VP_REPEAT_PLAY_MODE_E mode,
                              BOOLEAN is_random_play
                              );

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdatePlayBackMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdatePlayBackMode(VP_HANDLE vp_handle, 
                              VP_PLAYBACK_MODE_E mode
                              );


#ifdef MOBILE_VIDEO_SUPPORT//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_FSMGetCurBufPercent
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetCurBufPercent(VP_HANDLE vp_handle);
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT//norflashcut
/*****************************************************************************/
//  Description : ���ڽ�������Ϣ���浽nv��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_SetNVSettingInfo(VP_NV_SETTING_T nv_setting_info);

/*****************************************************************************/
//  Description : ��nv�л�ȡ������Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_NV_SETTING_T MMIAPIVP_GetNVSettingInfo(void);

/*****************************************************************************/
//  Description : get  vp net win id
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIVP_GetVPNetWinID(void);

#endif
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : CreateVideoLayer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CreateVideoLayer(void);
#endif
/*****************************************************************************/
//  Description : destroy video layer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_DestroyVideoLayer(void);

/*****************************************************************************/
//  Description : ��ֹ��ָ���Ƶ
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CurrentVideoEnableAudio(BOOLEAN is_enable);

/*****************************************************************************/
//  Description : switch current video
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CurrentVideoLcdSwitch(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_SetBgVideoAudioSupport(BOOLEAN is_audio_support);


/*****************************************************************************/
//  Description : ��ͣ��Ƶ����
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: vp_handleΪ0ʱ��ͣ��ǰ���ŵ���Ƶ
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Pause(VP_HANDLE vp_handle);

/*****************************************************************************/
//  Description : �ָ���Ƶ����
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: vp_handleΪ0ʱ�ָ���ǰ������Ƶ�ļ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Resume(VP_HANDLE vp_handle);

/*****************************************************************************/
//  Description : ��չʾ�������߼�ת������
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_TranslateDisplayParamLogicToPhysical(VP_DISPLAY_PARAM_T *disp_para, BOOLEAN is_need_fill);

/*****************************************************************************/
//  Description : ��ȡĬ�ϴ����Ƶ�ļ����ļ���·��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_GetDefaultFolderPath(
                                   wchar          *full_path_name_ptr,
                                   uint16          *full_path_len_ptr
                                   );

/*****************************************************************************/
//  Description : ��ȡ͸��ɫ
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIAPIVP_GetTransparenceColor(void);

/*****************************************************************************/
//  Description : ���͸��ɫ
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FillRectWithTransparenceColor(
                                           uint32 *width_ptr, //[IN/OUT]
                                           uint32 *height_ptr, //[IN/OUT]
                                           uint32 *x_offset_ptr, //[IN/OUT]
                                           uint32 * y_offset_ptr, //[IN/OUT]
                                           uint32 RotateAngle,
                                           BOOLEAN is_need_fill                                  
                                           );

/*****************************************************************************/
//  Description : ��ȡɸѡ��Ƶ�ļ���ɸѡ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetFilerInfo(MMIFMM_FILTER_T * filter);


 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
/*****************************************************************************/
//  Description : VP_ThumbnailOpenDPLAYER , ��������ͼ��ȡ���� ���򿪳ɹ�s_thumbnail_info.is_open��Ϊtrue
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailOpen(
                                      wchar          *full_path_name_ptr,    
                                      uint16            full_path_len,      
                                      uint32            seektime,      //��Ҫ��ȡ������ͼʱ���
                                      BOOLEAN  (*vp_callback)(DPLAYER_IMAGE_DATA_T *image_data),//ui�Ļص����� 
                                      DPLAYER_DISPLAY_PARAM_T *display_param,
                                      BOOLEAN   is_videoqueue) ;//�Ƿ�Ϊ��Ƶ����
/*****************************************************************************/
//  Description : MMIAPIVP_ThumbnailOpenByHandle , ��������ͼ��ȡ���� ���򿪳ɹ�s_thumbnail_info.is_open��Ϊtrue
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailOpenByHandle(
                                      VP_HANDLE  vphandle,
                                      uint32            seektime,      //in��Ҫ��ȡ����ͼ�ı�����Ƶ�ļ�����ʱ���
                                      BOOLEAN  (*vp_callback)(DPLAYER_IMAGE_DATA_T *image_data),//in,�Ƿ�Ϊ��Ƶ�б�������ȡ
                                      DPLAYER_DISPLAY_PARAM_T *display_param);//in,DPLAYER���Ų���                                       
/*****************************************************************************/
//  Description : �ر�����ͼ��ȡ���� ��ֹͣdplyer������״̬���������Ƶ���У����ͷ��б������Դ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  void  MMIAPIVP_ThumbnailClose(void);
/*****************************************************************************/
//  Description : MMIAPIVP_ThumbnailGetVideo
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  wchar*  MMIAPIVP_ThumbnailGetVideo(void);
/*****************************************************************************/
//  Description : ��ȡ  ��ǰlcd �Ƕȣ���������������л����»�ȡ������ͼ�첽����
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC LCD_ANGLE_E  MMIAPIVP_ThumbnailGetAngle(void);
/*****************************************************************************/
//  Description :     �Ƿ� ���ڽ���һ����Ƶ����ͼ��ȡ  ����DPLAYER_Play  ���ڲ���һ����Ƶ 
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_IsThumbnailRunning(void);

/*****************************************************************************/
//  Description :����ͼ��ȡ�Ƿ��Ѿ�����,MMIAPIVP_ThumbnailOpen ���óɹ���Ϊtrue,VP_ThumbnailClose��Ϊfalse
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_IsThumbnailOpen(void);

/*****************************************************************************/
//  Description : ��Ƶ����������ȡʱ���ָ� ��Ƶ����������ȡ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailResume(void) ;
/*****************************************************************************/
//  Description :  ��Ƶ����������ȡʱ�����б�β�����һ����Ƶ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_ThumbnailVideoAdd(wchar  *video_fullpath_name);
/*****************************************************************************/
//  Description : ��Ƶ����������ȡʱ����ʱ���  ��Ƶ����������ȡ��ͨ��VP_ThumbnailResume �ָ� 
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailStop(void);
/*****************************************************************************/
//  Description : ��ȡ  ��ǰ��Ƶ�����д���������Ƶ����
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPIVP_ThumbnailGetVideoQueueNum(void);

/*****************************************************************************/
//  Description : ���õ�ǰlcd �Ƕȣ���������������л����»�ȡ������ͼ�첽����
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIVP_ThumbnailSetAngle(LCD_ANGLE_E lcd_angle);
/*****************************************************************************/
//  Description : ����DPLAYER ���Ų���
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIVP_ThumbnailSetDPLAYERParam(DPLAYER_DISPLAY_PARAM_T *display_param);

#endif
/*****************************************************************************/
//  Description :����VPWin֧�����нǶȴ�����ʾ 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPIVP_SetVPWinSupportAllAngle(void);

/*****************************************************************************/
//  Description : is VP playing
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsVPPlaying(void);

#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
/*****************************************************************************/
//  Description : ������Ƶ�ļ�
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_MiniFunction_PlayVideo(wchar *full_path_name_ptr,
                                                                                            uint16 full_path_name_len);
/*****************************************************************************/
//  Description : ������Ƶ�ļ�
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_MiniFunction_ExitVideoPlayer(void);

/*****************************************************************************/
//  Description : post message to the main win 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_SDCardPlugOut(void);

#endif
/*****************************************************************************/
//  Description : ��ʾOSD
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVP_ResetSetSeekTime(void);

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIVP_IsSetSpecialPause
//  Global resource dependence : s_is_special_pause
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_IsSetSpecialPause(BOOLEAN is_set);
#endif


#ifdef DC_WATCH_UI_SUPPORT

/*****************************************************************************/
//  Description: get vp handle for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] vp_handle: uint32, vp handle.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC uint32 WatchVP_GetHandle_ForDv(void);

/*****************************************************************************/
//  Description: init vp for video play
//  Parameter: [In] VP_CALLBACK: function callback for caller to handle vp msg.
//             [Out] None
//             [Return] vp_handle: uint32, vp handle.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC uint32 WatchVP_Init_ForDv(VP_CALLBACK vpcallback);

/*****************************************************************************/
//  Description: start play for video play
//  Parameter: [In] filepath: video file path,  fileLen: file length.
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Play_ForDv(const wchar *filePath, uint32 fileLen);

/*****************************************************************************/
//  Description: pause play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Pause_ForDv(void);

/*****************************************************************************/
//  Description: resume play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Resume_ForDv(void);

/*****************************************************************************/
//  Description: stop play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, stop result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui. 
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Stop_ForDv(void);

#endif

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif


