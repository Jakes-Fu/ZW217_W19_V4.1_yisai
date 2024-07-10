/*****************************************************************************
** File Name:      mmivp_internal.h                                                   *
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

#ifndef _MMIVP_INTERNAL_H_
#define _MMIVP_INTERNAL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmivp_export.h"
#include "mmifmm_export.h"
#include "mmivp_locallist.h"
#include "mmicom_panel.h"

#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#ifdef KING_MOVIE_SUPPORT
#include "video_security.h"
#endif
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
#define VP_STRACE_LOW  SCI_TRACE_LOW
#ifdef AVI_DEC_SUPPORT
#define VP_IS_SUPPORT_AVI TRUE
#else
#define VP_IS_SUPPORT_AVI FALSE
#endif

#ifdef FLV_DEC_SUPPORT
#define VP_IS_FLV_SUPPORT            TRUE
#else
#define VP_IS_FLV_SUPPORT            FALSE
#endif

#ifdef MMI_RMVB_SUPPORT
#define VP_IS_RMVB_SUPPORT            TRUE
#else
#define VP_IS_RMVB_SUPPORT            FALSE
#endif

#define VP_MAX_PORT_LENGTH          5                        //�˿ڵ���󳤶�

#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
#define VP_MAX_URL_LEN              1                      //url����󳤶�
#else
#define VP_MAX_URL_LEN              1024                      //url����󳤶�
#endif
#define VP_MAX_NAME_LEN             MMIFMM_FILE_FILE_NAME_MAX_LEN    //���Ƶ���󳤶�
#define VP_AUTH_MAX_PASSWORD_LEN    20 
#define VP_AUTH_MAX_NAME_LEN        512

#define VP_DEFAULT_VOLUME           3            //Ĭ�ϲ�������
#define VP_DEFAULT_BRIGHTNESS       MMISET_EDEFAULT_CONTRAST            //Ĭ�ϵ�����
#define VP_DEFAULT_REPEAT_MODE      0            //Ĭ�Ϲر�ѭ��ģʽ
#define VP_DEFAULT_RANDOM_PLAY      FALSE        //Ĭ�Ϲر��������
#define VP_DEFAULT_NEXT_INDEX       0            //Ĭ�ϵ���������
    
#define VP_BOOKMARK_MAX_ITEM        50                         //�������ǩ��������

#define VP_MAX_INVALID_VALUE              0xffffffff      //��Ч���ֵ    

#define VP_SEC_TO_MSEL         1000        //1s = 1000ms

#define  HTTP_URL_HEAD_ASIC "http://"
#define  HTTP_URL_HEAD_LEN 7
#define  RTSP_URL_HEAD_ASIC "rtsp://"
#define RTSP_URL_HEAD_LEN 7
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

//��ť�Ĵ�����
typedef void (*VP_BUTTON_CALL_BACK)(BOOLEAN is_open);


//��������
typedef enum
{
        VP_ERROR_NONE = 0,                      //�޴���
        VP_ERROR_NULL_FILE,                 //���ļ�
        VP_ERROR_NOT_SUPPORT,               //��ʽ��֧��
        VP_ERROR_INTERNAL_SERVER_ERROR,     //�������ڲ�����
        VP_ERROR_BAD_REQUEST,               //���������
        VP_ERROR_UNAUTHORIZED,              //��֤ʧ��
        VP_ERROR_NOT_FOUND,                 //δ�ҵ�ý��
        VP_ERROR_PDP_ACTIVE_ERROR,          //Pdp����ʧ��
        VP_ERROR_PDP_DEACTIVE_IND,          //PDP���������Ͽ�
        VP_ERROR_SERVER_CLOSE_IND,          //�����������ر�����
        VP_ERROR_STRM_INIT_ERROR,           //Streaming���ʼ������
        VP_ERROR_PAUSE_ERROR,               //��ͣ���ų���
        VP_ERROR_RESUME_ERROR,              //�ָ����ų���
        VP_ERROR_SEEK_ERROR,                //��λ���ų���
        VP_ERROR_DATA_TIMEOUT,              //streaming�ȴ����ݳ�ʱ
        VP_ERROR_CONNECT_TIMEOUT,
        VP_ERROR_DRM_RIGHT_INVALID,         //drm��Ȩ��Ч
        VP_ERROR_DRM_NO_SYS_NITZ,         //drm��Ȩ��Ч
        VP_ERROR_OTHERS,                    //��������
        VP_ERROR_MAX                        //����
} VP_ERROR_E; 

//�����б����ԭ��
typedef enum
{
    VP_LIST_ERROR_NONE = 0,         //�ɹ�
        VP_LIST_ERROR_TOO_MUCH,     //�������ƣ�����ӵ�̫�ࣩ
        VP_LIST_ERROR_NO_SPACE,     //û�пռ���
        VP_LIST_ERROR_OTHER,        //��������
        VP_PLAY_MODE_MAX            //����
} VP_LIST_ERROR_E;

//��ť��״̬
typedef enum
{
    VP_BUTTON_NONE,             //����ʾ
        VP_BUTTON_NORMAL,       //������ʾ
        VP_BUTTON_FOCUS,        //��ý���
        VP_BUTTON_GREY,         //�һ���ʾ
        VP_BUTTON_STATE_MAX     //����
}VP_BUTTON_DISPLAY_MODE_E;

//��ť����
typedef enum
{
    VP_BUTTON_FILE_ICON,            //�ļ�ѡ��icon
        VP_BUTTON_REPEAT_MODE,      //ѭ������ѡ��icon
        VP_BUTTON_RANDOM_MODE,      //�������ѡ��icon
        VP_BUTTON_PROCESS,          //������        
        VP_BUTTON_VOLUME,           //����icon
        VP_BUTTON_BACKWARD,         //����/��һ��icon
        VP_BUTTON_PAUSE_PLAY,       //����/��ͣ icon
        VP_BUTTON_FORWARD,          //ǰ��/��һ��icon
        VP_BUTTON_SWITCH_SCREEN,    //�л���Ļicon      
        VP_BUTTON_CONTROL_PANEL_INC, //��������inc icon
        VP_BUTTON_CONTROL_PANEL_DEC, //��������dec icon
        VP_BUTTON_CONTROL_PANEL_CLOSE, //��������close icon
        VP_BUTTON_MAX               //����
}VP_BUTTON_TYPE_E;


//��ť��Ϣ
typedef struct 
{    
    GUI_RECT_T                      rect;       //��ť����
    VP_BUTTON_DISPLAY_MODE_E        mode;       //��ʾģʽ
    BOOLEAN                         is_tp_down;
    MMI_MESSAGE_ID_E                keydown_msg;   //��Ӧ��keydown��Ϣ
    MMI_MESSAGE_ID_E                keyup_msg;   //��Ӧ��keyup��Ϣ
    MMI_MESSAGE_ID_E                keylong_msg; //��Ӧ�ĳ�����Ϣ��
    MMI_MESSAGE_ID_E                keylongup_msg;//��Ӧ�ĳ���������Ϣ
    MMI_IMAGE_ID_T                  image_id;   //��ťͼƬid
}BUTTON_INFO_T;

typedef enum
{
    VP_TIMER_NONE_TYPE,

    VP_TIMER_VOL_INC_TYPE,
    VP_TIMER_VOL_DEC_TYPE,

    VP_TIMER_BRIGHT_INC_TYPE,
    VP_TIMER_BRIGHT_DEC_TYPE,
    VP_TIMER_FORWARD_TYPE,
    VP_TIMER_BACKWARD_TYPE,   
 
}VP_TIMER_TYPE_E;

typedef struct  
{
    VP_TIMER_TYPE_E  timer_type;
    uint8          timer_id;
}VP_TIMER_T;

//��Ȩ��Ϣ
typedef struct
{
    char                *username_ptr;      //�û���
    char                *password_ptr;      //����
} VP_AUTH_INFO_T;

//��ý����ַ��Ϣ
typedef struct
{
    wchar       title_arr[VP_MAX_NAME_LEN+1];       //��ý����ַ����
    uint32      title_len;                          //��ý����ַ���Ƴ���
    char        url_ptr[VP_MAX_URL_LEN+1];            //��ý����ַ
} VP_NET_MOVIE_INFO_T;


//�����ļ���Ϣ
typedef struct
{
    wchar           full_path_ptr[VP_MAX_NAME_LEN +1];  //�ļ��ľ���·��
    uint16          full_path_len;                      //�ļ��ľ���·������
    uint32          create_time;                        //�ļ�����ʱ��
    uint32          width;                              //��Ƶͼ����
    uint32          height;                             //��Ƶͼ��߶�
    uint32          media_length;                       //��Ƶ����ʱ��
    uint32          file_size;                          //��Ƶ�ļ���С
} VP_LOCAL_MOVIE_FILE_INFO_T;

//��������׼����ϡ�UI������ݷ��ز�����ʾƬԴ����ʱ������Ϣ��
typedef struct
{
    VP_HANDLE                   vp_handler;     //Videoplayer controller handler
    uint32                      start_time;     //���ſ�ʼʱ��
    uint32                      total_time;     //ƬԴ��ʱ�䣨ȫF����ֱ����
    wchar*                      file_name_ptr;  //ƬԴ����,����Ǳ����ļ���Ӧ����ȫ·��
    uint16                      file_name_len;  //ƬԴ���Ƴ���
    VP_PLAY_TYPE_E              play_type;      //��������
    VP_SUPPORTED_STATUS_TYPE_E  support_type;   //֧�ֵĳ̶�
} VP_PREPARE_IND_T;

//���浱ǰ�Ĳ��Ž��ȣ�ÿ����һ֡���ᱨ����Ϣ��
typedef struct
{
    VP_HANDLE               vp_handler;     //Videoplayer controller handler
    uint32                  process_time;   //��ǰ���ŵ�ʱ��
    uint32                  total_time;     //ƬԴ��ʱ�䣨ȫF����ֱ����
} VP_PLAY_PROCESS_IND_T;

//���浱ǰ�Ļ�����ȣ�ÿ����һ֡���ᱨ����Ϣ��
typedef struct
{
    VP_HANDLE           vp_handler;     //Videoplayer controller handler
    uint32              buffer_percent;
} VP_BUFFERING_PROCESS_IND_T;

//���������
typedef struct
{
    VP_HANDLE           vp_handler;     //Videoplayer controller handler
} VP_BUFFERING_IND_T;

//���������
typedef struct
{
    VP_HANDLE           vp_handler;     //Videoplayer controller handler
} VP_BUFFER_END_IND_T;

//���Ź����г�����������
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //���Ŵ�������
} VP_FATAL_ERROR_IND_T;

//��Ҫ��Ȩ��֤
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
}VP_AUTH_IND_T;



//������ֹͣ
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler 
} VP_STOP_IND_T;

typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //���Ŵ�������
} VP_PAUSE_CNF_T;

typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //���Ŵ�������
} VP_PLAY_CNF_T;

typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //���Ŵ�������
    uint32              cur_time;   //��ǰ����ʱ��
    uint32              total_time; //�ܵĲ���ʱ��
} VP_SEEK_CNF_T;

typedef enum
{
    VP_TO_DO_NONE,
    VP_TO_DO_PLAY_FOR_STOP,
    VP_TO_DO_PLAY_FOR_FULL_PAINT,
    VP_TO_DO_PLAY_FOR_LOAD_FILE,
    VP_TO_DO_SEEK,
    VP_TO_DO_STOP,
    VP_TO_DO_MAX
}VP_TO_DO_TYPE_E;

#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
//��Ƶ��������Ϣ
typedef struct  
{
    VP_HANDLE           vp_handle;                              //״̬��handle
    VP_ENTRY_E          vp_entry;                               //���벥�����ķ�ʽ����
    VP_STATE_E          state;                                  //������ui״̬
    VP_PLAY_TYPE_E      play_type;                              //��������  
    uint32              total_time;                             //��ʱ��
    uint32              process_time;                           //���Ž���  
    uint32              buffer_percent;                         //�������
    VP_SUPPORTED_STATUS_TYPE_E support_type;                    //֧�ֵĳ̶�   
    VP_TO_DO_TYPE_E     to_do_type;                             //��Ҫ�������������
    uint32              seek_time;                              //    
    uint8               display_state_timer_id;                 //״̬��ʾtimer id
    uint8               url[VP_MAX_URL_LEN +1];                 //���粥�ŵ�url
    wchar               full_path_name[VP_MAX_NAME_LEN +1];     //���ز���ʱΪȫ·�������粥��ʱΪƬԴ����
    uint16              full_path_len;                          //���Ƴ���    
    LCD_ANGLE_E         cur_angle;                             //��ǰ��Ƶ���ڽǶ�
    BOOLEAN             is_display_win;                         //ȫ��״̬���Ƿ���ʾ�������
#ifdef KING_MOVIE_SUPPORT
    BOOLEAN             is_kmv_file;
#endif
    VP_POST_PROGRESS    post_progrecss;//�ļ��ĺ������ 
    uint8               display_win_timer_id;                   //ȫ��״̬�¿��������ʾtimer
    VP_TIMER_T          timer;                                  //����timer_id
    MMI_TEXT_ID_T       station_text_id;                        //״̬��ʾ
    MN_DUAL_SYS_E       dual_sys;
    VP_BT_STATE_E       bt_state;                                //����״̬
	BOOLEAN             is_adjust_bright;
    //GUI_LCD_DEV_INFO    dev_layer_handle;                        //video���handle
    MMISRV_HANDLE_T     srv_handle;
}VP_INTERFACE_PLAYING_INFO_T;

//��ʷ��¼��Ϣ
typedef struct VP_HISTORY_ITEM_tag
{
    VP_PLAY_TYPE_E                      type;                       //��¼����Ƶ���ŷ�ʽ���ͣ����磬���أ�
    VP_NET_MOVIE_INFO_T                 *net_movie_info_ptr;        //��ý����ַ��Ϣ
    VP_LOCAL_MOVIE_FILE_INFO_T          *local_movie_file_info_ptr; //����������Ϣ
    uint32                              last_play_point;            //�ü�¼��һ�β���ʱ���жϴ���ֱ��Ϊ0��
    uint32                              total_time;                 //��ʱ��
    int32                               locallist_index;
    VP_LOCALLIST_TYPE_E                 locallist_type;
    struct VP_HISTORY_ITEM_tag          *next_item_ptr;             //��һ����ʷ��¼
} VP_HISTORY_ITEM_T;

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
#ifdef MMI_PDA_SUPPORT
    MMIVP_PDA_SETTINGS_REPEAT,
    MMIVP_PDA_SETTINGS_RANDOM,
#endif    
	MMIVP_PDA_SETTINGS_PLAY_METHOD,
    MMIVP_PDA_SETTINGS_NET1,
    MMIVP_PDA_SETTINGS_NET2,
    MMIVP_PDA_SETTINGS_NET3,
    MMIVP_PDA_SETTINGS_NET4,
    MMIVP_PDA_SETTINGS_RTCP_RTP,
    MMIVP_PDA_SETTINGS_AGENT,
    MMIVP_PDA_SETTINGS_FLOWRATE,
    MMIVP_PDA_SETTINGS_RESET,
#if defined MMI_VIDEOPLAYER_HELP_SUPPORT 
    MMIVP_PDA_SETTINGS_HELP,
#endif
    
    MMIVP_PDA_SETTINGS_MAX
}MMIVP_PDA_SETTINGS_TYPE_E;

typedef enum
{
    VP_RANDOM_PLAY_MODE_OPEN,
    VP_RANDOM_PLAY_MODE_CLOSE,

    VP_RANDOM_PLAY_MODE_MAX
}VP_RANDOM_PLAY_MODE_E;

#endif //MMI_PDA_SUPPORT

 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
#define THUMBNAIL_QUEUENUM  7  //��Ƶ���еĴ�С 

typedef BOOLEAN  (*THUMBNAIL_CALLBACK)(DPLAYER_IMAGE_DATA_T *image_data); 
typedef BOOLEAN  (*STOPCNF_CALLBACK)(void); 
typedef struct
{
    wchar  thumbnail_name [THUMBNAIL_QUEUENUM][MMIFMM_FULL_FILENAME_LEN+1];
    uint8    videonum;
}VP_THUMBNAIL_VIDEOQUEUE;

// ����ͼ��Ϣ�ṹ�� 
typedef struct  
{
    wchar                        full_path_ptr[MMIFMM_FILE_FILE_NAME_MAX_LEN];              //ý������(�����ļ�Ϊȫ·��)
    uint16                         full_path_len;              //ý�����ֳ��� 
    BOOLEAN                    is_dplayerhandle;        //�Ƿ�ͨ�� dphandle����ȡ����ͼ�ffull_path_ptr ΪNULLʱ��Ч
    BOOLEAN                    is_open;                      //MMIAPIVP_ThumbnailOpen ���óɹ���Ϊtrue,VP_ThumbnailClose��Ϊfalse
    BOOLEAN                    is_loading;                   // �Ƿ� ��������һ������  
    LCD_ANGLE_E             lcd_angle;                          //��ǰlcd �Ƕȣ���������������л����»�ȡ������ͼ�첽����
    uint32                          seektime;                   // ��Ҫ��ȡ����ͼ��ʱ���
    DPLAYER_HANDLE        dphandle;                   //dplayer handle
    THUMBNAIL_CALLBACK                 tn_callback;              //ui�Ļص�����
    DPLAYER_IMAGE_DATA_T *image_data;        //ͼ������,out
    DPLAYER_DISPLAY_PARAM_T  display_param;//dplay���� ,in
    uint32                                    taskid;  //Ĭ��P_APP
#ifdef KING_MOVIE_SUPPORT
    VPLAYER_SECURITY_HANDLE         sec_handle;        //SECURITY_HANDLE  VPLAYER_SECURITY_HANDLE
    BOOLEAN  b_sprd_kmv;
#endif

    BOOLEAN                               is_videoqueue;
    VP_THUMBNAIL_VIDEOQUEUE     *videoqueue;
}VP_THUMBNAIL_T;  
#endif


/*****************************************************************************/
//  Description : ���RTP/RTCP�����߶˿ڡ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_GetRTPPort(uint16 *lowest_port_ptr,
                                  uint16 *highest_port_ptr);

/*****************************************************************************/
//  Description : ���ô��������
//  Global resource dependence :  none
//  Author: apple.zhang
/*****************************************************************************/
PUBLIC void VP_Setting_SetAgent(BOOLEAN  is_use_agent,
                                uint8 *ip_ptr,
                                uint8  ip_length,
                                uint16 port);

/*****************************************************************************/
//  Description : ��ô��������
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_GetAgent(BOOLEAN  *is_use_agent_ptr,
                                uint8 *ip_ptr,
                                uint8 ip_max_length,
                                uint16 *port_ptr);
                                
/*****************************************************************************/
//  Description : ��ȡ�Ƿ����������ģʽ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_Setting_GetIsRandomPlayMode(void);

/*****************************************************************************/
//  Description : �����Ƿ����������ģʽ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetIsRandomPlayMode(BOOLEAN is_open);

/*****************************************************************************/
//  Description : ��ȡ�����������õ�index
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 VP_Setting_GetNetIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : ���������������õ�index��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetNetIndex(MN_DUAL_SYS_E dual_sys,
                                    uint8         index);
/*****************************************************************************/
//  Description : ����RTP/RTCP�����߶˿ڡ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRTPPort(uint16 lowest_port,
                                  uint16 highest_port);
                                    
/*****************************************************************************/
//  Description : ��ȡ���ȴ�С
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 VP_Setting_GetBrightness(void);

/*****************************************************************************/
//  Description : ��������
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetBrightness(uint8 brightness);

/*****************************************************************************/
//  Description : ��ȡ�����ļ����ŷ�ʽ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_PLAYBACK_MODE_E VP_Setting_GetPlaybackMode(void);

/*****************************************************************************/
//  Description : ���ñ����ļ����ŷ�ʽ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetPlaybackMode(VP_PLAYBACK_MODE_E playback_mode);

/*****************************************************************************/
//  Description : ��ȡѭ������ģʽ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_REPEAT_PLAY_MODE_E VP_Setting_GetRepeatPlayMode(void);


/*****************************************************************************/
//  Description : ����ѭ������ģʽ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_E mode);

/*****************************************************************************/
//  Description : ����ѭ������ģʽ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_E mode);

/*****************************************************************************/
//  Description : �ָ���ʼ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: ���ص�ָ����Ҫ����DestroyMsg�ͷ�
/*****************************************************************************/
PUBLIC void VP_Setting_ResetSettingInfo(void);

/*****************************************************************************/
//  Description : ��ȡ������Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: ���ص�ָ����Ҫ����DestroyMsg�ͷ�
/*****************************************************************************/
PUBLIC VP_NV_SETTING_T VP_Setting_GetSettingInfo(void);

/*****************************************************************************/
//  Description : �����Ƶ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_GetMediaInfo(VP_HANDLE  vp_handle,
                            DPLAYER_MEDIA_INFO_T    *media_info_ptr);

/*****************************************************************************/
//  Description : ���мӿ�������Ƶ���ű��ػ�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_StartSetRate(VP_HANDLE   vp_handle,
                               BOOLEAN   is_backward);

/*****************************************************************************/
//  Description : ֹͣ���мӿ�������Ƶ���ű��ػ�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_StopSetRate(VP_HANDLE   vp_handle,
                               BOOLEAN   is_backward);

/*****************************************************************************/
//  Description : �����Ȩ��֤
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
PUBLIC BOOLEAN VP_AuthRsp(VP_HANDLE   vp_handle,
                        VP_AUTH_INFO_T  *auth_info_ptr);
#endif
/*****************************************************************************/
//  Description : ���ڴ�������task��jvm���͵���Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVP_DispatchSignalToFsmMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
/*****************************************************************************/
//  Description : create mutex 
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_CreateFsmMutex(void);
#if  0//norflashcut
/*****************************************************************************/
//  Description : VP_UpdateCurrentFrame
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_UpdateCurrentFrame(VP_HANDLE vp_handle);
#endif
/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIVP_GetAlign4Offset(uint32 x);

/*****************************************************************************/
//  Description : DelaySwitchVideoPlayer
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC void DelaySwitchVideoPlayer(uint32 param);

/*****************************************************************************/
//  Description : HandleDelaySwitchVideoPlayerTimer
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC void HandleDelaySwitchVideoPlayerTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : StopDelaySwitchVideoPlayerTimer
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC void StopDelaySwitchVideoPlayerTimer(void);

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIvp_RequestHandle(void);

/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIvp_GetAudioHandle(void);

/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIvp_SetAudioHandle(MMISRV_HANDLE_T handle);

#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
//  Description : ����osd��͸��ɫ��ĳЩ����� ��Ҫ��ÿ�β���ʱ��Ҫ����һ�Σ�����������
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void VP_SetBlockTypeForSS(void);
#endif

#ifdef MMI_AMRBT_VPSUPPORT 
/*****************************************************************************/
//  Description : bt play next file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventNext(void);

/*****************************************************************************/
//  Description : bt play pre file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventPrev(void);

/*****************************************************************************/
//  Description : bt notify video player play or pause 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventPlayOrPause(void);
#endif
/*****************************************************************************/
//  Description : ������������
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);

/*****************************************************************************/
//  Description : ���s_current_fsm_ptr
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC VP_HANDLE VP_GetCurrentFSMHandle(void);
/*****************************************************************************/
//  Description : ���dphandle
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC DPLAYER_HANDLE VP_GetDPLAYERhandlebyVPhandle(VP_HANDLE vphandle);


 #ifdef  VIDEOTHUMBNAIL_SUPPORT 
/*****************************************************************************/
//  Description :�յ���һ�� ���ųɹ���Ϣ, ��ȡ����ͼ����UI�ص�����  
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void HandleGetThumbnaiData(void);
/*****************************************************************************/
//  Description : HandleOpenNextVideo
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void HandleOpenNextVideo(void);
#endif
#ifdef KING_MOVIE_SUPPORT
/*****************************************************************************/
//  Description : VP_dplayerOpenSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC VPLAYER_SECURITY_HANDLE VP_dplayerOpenSecurty(void * pvoid,BOOLEAN *b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T** sec_info_ptr);//andy.he_kingmv

/*****************************************************************************/
//  Description : VP_dplayerCloseSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerCloseSecurty(void * pvoid,VPLAYER_SECURITY_HANDLE sec_handle,BOOLEAN b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T** sec_info_ptr);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlAddSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlAddSecurty(void * pvoid,DPLAYER_HANDLE player_handle);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlDeltSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlDeltSecurty(void * pvoid,DPLAYER_HANDLE player_handle, VPLAYER_SECURITY_HANDLE sec_handle);
#endif
#else
    /*****************************************************************************/
//  Description : ���ڴ�������task��jvm���͵���Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVP_DispatchSignalToFsmMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
/*****************************************************************************/
//  Description : ������������
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);
/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIVP_GetAlign4Offset(uint32 x);
#ifdef KING_MOVIE_SUPPORT
/*****************************************************************************/
//  Description : VP_dplayerOpenSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC VPLAYER_SECURITY_HANDLE VP_dplayerOpenSecurty(void * pvoid,BOOLEAN *b_sprd_kmv);//andy.he_kingmv

/*****************************************************************************/
//  Description : VP_dplayerCloseSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerCloseSecurty(void * pvoid,VPLAYER_SECURITY_HANDLE sec_handle,BOOLEAN b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T* sec_info_ptr);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlAddSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlAddSecurty(void * pvoid,DPLAYER_HANDLE player_handle);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlDeltSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlDeltSecurty(void * pvoid,DPLAYER_HANDLE player_handle, VPLAYER_SECURITY_HANDLE sec_handle);
#endif


#endif //MMI_VIDEOPLAYER_MINI_FUNCTION
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


#endif
