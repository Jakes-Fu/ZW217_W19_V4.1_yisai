/*****************************************************************************
** File Name:               mmiap_play.h                                     *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifndef _MMIAP_PLAY_H_
#define _MMIAP_PLAY_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrvaud_api.h"

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmisrvap_export.h"


#include "mmimp3_export.h"
#include "mmiapwin_main.h"
#include "mmiapwin_common.h"

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
#ifdef DRM_SUPPORT
#define MMIMP3_SUFFIX_DRM           ".dm"   //DRM�ļ���׺��
#define MMIMP3_SUFFIX_DCF           ".dcf"  //DCF�ļ���׺��

#define MMIMP3_SUFFIX_DRM_NO_DOT    "dm"   //DRM�ļ���׺��
#define MMIMP3_SUFFIX_DCF_NO_DOT    "dcf"  //DCF�ļ���׺��
#endif


typedef uint32 MMIAP_PLAY_WIN_FLAG_T;
#define MMIAPWIN_PLAY_FLAG_ALL_PANEL        0x00000001 /*��������*/
#define MMIAPWIN_PLAY_FLAG_PRECHECK_FILE    0x00000002 /*pre check file for drm*/
#define MMIAPWIN_PLAY_FLAG_ALERT_MSG        0x00000004 /*alert msg*/
#define MMIAPWIN_PLAY_FLAG_START_SPECTRUM   0x00000008 /*start spectrum*/
#define MMIAPWIN_PLAY_FLAG_BUTTON_PROGRESS  0x00000010 /*display button and progress info*/
#define MMIAPWIN_PLAY_FLAG_ONLY_PROGRESS    0x00000020 /*only display progress info*/
#define MMIAPWIN_PLAY_FLAG_WIN_SET_ALERT    0x00000040 /*mmiset alert win*/
#define MMIAPWIN_PLAY_FLAG_SWITCH_ALBUM     0x00000080 /*switch album info*/
#define MMIAPWIN_PLAY_FLAG_MSG_SOFT_BAR     0x00000100 /*post soft bar msg*/
#define MMIAPWIN_PLAY_FLAG_SHOW_LYRIC_NOW   0x00000200 /*show lyric immediately*/
#define MMIAPWIN_PLAY_FLAG_PLAY_CALLBACK    0x00000400 /*virtual win play callback*/
#define MMIAPWIN_PLAY_FLAG_FATAL_ERROR      0x00000800 /*virtual win fatal error*/
#define MMIAPWIN_PLAY_FLAG_UPDATE_MP3       0x00001000 /*main idle win update mp3*/
#define MMIAPWIN_PLAY_FLAG_SWITCH_MP3       0x00002000 /*main idle win widget mp3 switch*/
#define MMIAPWIN_PLAY_FLAG_UPDATE_BUTTON_STATE  0x00004000 /*update button state*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
//Mp3 bt mode setting
//BT �������ӳ�
typedef enum
{
    MMIMP3_BT_MODE_NONE, //��ʼ״̬, δ���ù�BT����״̬�Զ����Ӷ���
    MMIMP3_BT_MODE_ON, //ѡ��BT������BT
    MMIMP3_BT_MODE_OFF, //ѡ��BT��δ����BT
    MMIMP3_BT_SETTING_MAX = MMIMP3_BT_MODE_OFF,
} MMIMP3_BT_SETTING_TYPE;

//��ͣ����
typedef enum
{
    MMIMP3_PAUSED_BY_NONE,
    MMIMP3_PAUSED_BY_USER,
    MMIMP3_PAUSED_BY_APP,
    MMIMP3_PAUSED_BY_USER_AND_APP,
    MMIMP3_PAUSED_TYPE_MAX
}MMIMP3_PAUSED_TYPE_E;

//Mp3 review control info
typedef struct
{
    BOOLEAN is_end_file_end;    //����������Ϊ�ļ�β��
    uint32  start_point;        //������ʼ��(bytes)
    uint32  end_point;          //����������(bytes)
    uint32  start_time;         //������ʼʱ��(Second)
    uint32  end_time;           //��������ʱ��(Second)
    MMIMP3_REVIEW_STATE_TYPE_E state;   //����״̬
} MMIMP3_REVIEW_INFO_T;

typedef struct
{
    MMIMP3_PLAYER_STATE_E        player_state;
    MMIMP3_AUDIO_STATE_E         audio_state;
}MMIMP3_OLD_INFO_T;

//���ſ��ƽṹ
typedef struct
{
    BOOLEAN                      is_first_init;
    BOOLEAN                      is_mute;            //mute state
    MMIMP3_BT_SETTING_TYPE       bt_mode;            //bt mode state
    uint8                        switch_delay_timer; //�����л��ӳ�timer
    uint32                       switch_delay_offset;//�����л�ʱ��ʼoffset
    MMIMP3_PLAYER_STATE_E        player_state;       //������״̬
    MMIMP3_AUDIO_STATE_E         audio_state;        //��ǰ��audio״̬
    MMIMP3_PLAYING_DYN_INFO_T    dyn_playing_info;   //��ǰ���ŵĶ�̬״̬��Ϣ
    BOOLEAN                      is_paused_by_service;
#ifdef MMIAP_F_HISTORY_PLAYLIST    
    MMIMP3_OLD_INFO_T            old_info;
#endif    
} MMIMP3_PLAY_INFO_T;


typedef struct
{
    MMIAP_PLAY_WIN_FLAG_T flag;
    wchar *full_path_name_ptr; //mp3 file name
#ifdef DRM_SUPPORT    
    DRMFILE_PRE_CHECK_STATUS_E *drm_status_ptr;
#endif    
    MMIMP3_ALERT_MSG_E alert_msg; //alert msg
    MMISRVAP_ERROR_E  srvap_result; //from ap srv result
    void* param_ptr;
}MMIAP_PLAY_WIN_CALLBACK_PARAM_T;


typedef struct
{
    MMIMP3_PLAY_INFO_T play_info;
    MMIMP3_REVIEW_INFO_T review_info;
    uint16  no_support_num;
    MMISRVAP_HANDLE srvap_handle;
    MMISRV_HANDLE_T audio_srv_handle; 
#ifdef MMIAP_F_FIRST_SEARCH
    BOOLEAN is_first_search;
#endif

    BOOLEAN is_release_audio_handle; /*flag*/
}MMIAP_PLAY_CTRL_INFO_T;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Play mp3 in a delayed manner, start delay timer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DelayPlayMp3(
                    uint32    mp3_offset
                    );

/*****************************************************************************/
//  Description : Stop the delay timer, for playing mp3 in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopDelayPlayTimer(
                    void
                    );

/*****************************************************************************/
//  Description : play mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for mp3 module
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_PlayMp3(
                      uint32    mp3_offset
                      );

/*****************************************************************************/
//  Description : Stop the audio BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_StopMp3(
                    void
                    );

/*****************************************************************************/
//  Description : after play mp3 ,callback function
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_CallBackAfterPlay(
                                     MMISRVAP_ERROR_E  result,
                                     DPARAM                     param
                                     );

/*****************************************************************************/
//  Description : Check Mp3 No Support Num
//  Global resource dependence : g_mp3_play_info
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_NoSupportNumInc(void);

/*****************************************************************************/
//  Description : Check Mp3 No Support Num
//  Global resource dependence : g_mp3_play_info
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_NoSupportNumClear(void);

/*****************************************************************************/
//  Description : Reset mp3 review info.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_ResetReviewInfo(
                    void
                    );

/*****************************************************************************/
//  Description : Set mp3 review starting point.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetReviewStartPoint(
                    void
                    );

/*****************************************************************************/
//  Description : Set mp3 review end point.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetReviewEndPoint(
                    void
                    );

/*****************************************************************************/
//  Description : Start review.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_StartReview(
                    void
                    );

#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
/*****************************************************************************/
//  Description : Exit review.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_ExitReview(
                    void
                    );

/*****************************************************************************/
//  Description : Handle the section review.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_HandleMp3ReviewPlay(
                    void
                    );
#endif

/*****************************************************************************/
//  Description : Get the review state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIMP3_REVIEW_STATE_TYPE_E MMIMP3_GetReviewStatus(
                                        void
                                        );

/*****************************************************************************/
//  Description : Handle the end of the review section.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_HandleMp3ReviewEnd(
                    uint32 cur_offset
                    );

/*****************************************************************************/
//  Description : Get current audio state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIMP3_AUDIO_STATE_E MMIMP3_GetAudioState(
                                void
                                );

/*****************************************************************************/
//  Description : Set current audio state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_SetAudioState(
                        MMIMP3_AUDIO_STATE_E cur_state
                        );

/*****************************************************************************/
//  Description : To judge whether the current playing file support seek.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: ���ݺ�׺�ж�
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsCurFileSeekSupport(
                    void
                    );

/*****************************************************************************/
//  Description : To judge whether the music is supported for seeking.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: ���ݺ�׺�ж�
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsFileSeekSupport(
                                        const wchar *full_path_ptr,
                                        uint16 full_path_len
                                        );


/*****************************************************************************/
//  Description : Get the playing Dyn Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetPlayingDynInfo(
                MMIMP3_PLAYING_DYN_INFO_T  *dyn_info_ptr,
                BOOLEAN                    is_real_time 
                );

/*****************************************************************************/
//  Description : Reset the playing Dyn Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ResetPlayingDynInfo(
                void
                );

/*****************************************************************************/
//  Description : Fatal error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_FatalError(void);

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : Set the mp3 bt setting mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetBtSettingMode(
                MMIMP3_BT_SETTING_TYPE bt_mode
                );

/*****************************************************************************/
//  Description : Get the mp3 bt setting mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_BT_SETTING_TYPE MMIMP3_GetBtSettingMode(
                                    void
                                    );
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsFirstInit(
                    void
                    );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_SetFirstInit(
                    BOOLEAN is_first_init
                    );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsPausedByService(
                    void
                    );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_SetPausedByService(
                    BOOLEAN is_paused_by_service
                    );

/*****************************************************************************/
//  Description : Request mp3 play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_ReqPlayHandle(void);

/*****************************************************************************/
//  Description : Release mp3 play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_ReleasePlayHandle(void);

/*****************************************************************************/
//  Description : Request mp3 play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_ReqPlaySrv(const wchar * file_name_ptr,
                                 uint16 file_name_len,
                                 MMISRVAUD_RING_FMT_E audio_type,
                                 uint32 offset
                                 );

#ifdef MMIAP_F_HISTORY_PLAYLIST
/*****************************************************************************/
//  Description : get audio player current play info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_INFO_T *MMIAP_GetAPCurPlayInfo(void);

/*****************************************************************************/
//  Description : init audio player old info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InitOldInfo(void);

/*****************************************************************************/
//  Description : get audio player old info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_OLD_INFO_T *MMIAP_GetCurOldInfo(void);
#endif //#ifdef MMIAP_F_HISTORY_PLAYLIST

/*****************************************************************************/
//  Description : get current file content
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurFileContent(void);


/*****************************************************************************/
//  Description : forward or backward mp3
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_ForwardOrBackwardMp3(
    BOOLEAN              is_tp_progress, //�Ƿ���Tp������
    BOOLEAN              is_forward,
    uint32               mp3_offset,     //����:Mp3 pauseλ������,Tp������:Mp3 0λ������
    MMIMP3_AUDIO_STATE_E cur_state
);

/*****************************************************************************/
//  Description : mp3 alert call back
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_Mp3AlertCallback(
    void
);

/*****************************************************************************/
//  Description : init play ctrl info
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitPlayCtrlInfo(
    void
);

/*****************************************************************************/
//  Description : get ap audio srv handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAP_GetAPAudioSrvHandle(
void
);

/*****************************************************************************/
//  Description : destroy ap play ctrl info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyAPPlayCtrlInfo(
void
);

/*****************************************************************************/
//  Description : set review info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetReviewInfo(
    MMIMP3_REVIEW_INFO_T *review_ptr //in
);

/*****************************************************************************/
//  Description : get review info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_GetReviewInfo(
    MMIMP3_REVIEW_INFO_T *review_ptr //out
);

#ifdef MMIAP_F_FIRST_SEARCH
/*****************************************************************************/
//  Description : is first search or not
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsFirstSearch(void);

/*****************************************************************************/
//  Description : CheckSearchFlag
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_CheckSearchFlag(void);
#endif

/*****************************************************************************/
//  Description : mgr resume
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_MGR_Resume(
    void
);

#ifdef MMIAP_F_RELEASE_BG_AUDIO
/*****************************************************************************/
//  Description : request audio handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_RequestAudioHandle(
    void
);
#endif

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif


