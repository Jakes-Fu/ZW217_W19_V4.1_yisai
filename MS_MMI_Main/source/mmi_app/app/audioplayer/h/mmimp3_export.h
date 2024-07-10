/*****************************************************************************
** File Name:      mmimp3_export.h                                           *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine           Create
******************************************************************************/

#ifndef _MMIMP3_H_
#define _MMIMP3_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "mmibt_export.h"
#include "mmibt_app.h"
#include "mmiaudio_ctrl.h"
#include "mmiap_common.h"
#include "mmiapwin_common.h"
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
#define MMIMP3_NAME_MAX_LEN     (FMM_SEARCH_FILENAME_MAXLEN + 1)//MP3名字的最大长度(文件名+后缀名),255个unicode
#define MMIMP3_COMMON_TAG_LEN   32 //artist,album描述一般长度，与AUDIO_ID3_TAG_T对应

//"Mp3_res"
#define MMIMP3_RESERVED_DIR         (g_mmimp3_reserved_dir)//"\x00\x4d\x00\x70\x00\x33\x00\x5f\x00\x72\x00\x65\x00\x73\x00\x00"
#define MMIMP3_RESERVED_DIR_LEN     7

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//Mp3 module state
typedef enum
{
    MMIMP3_PLAYER_STATE_NONE,   //state none
    MMIMP3_PLAYER_AUDIO_ACTIVE, //audio playing
    MMIMP3_PLAYER_MUSIC_SWITCH, //music switching
    MMIMP3_PLAYER_SWITCH_PAUSED,//pause while switching
    MMIMP3_PLAYER_STATE_MAX
} MMIMP3_PLAYER_STATE_E;

//Mp3 audio state, concept belongs to MMIMP3_PLAYER_AUDIO_ACTIVE
typedef enum
{
    MMIMP3_AUDIO_OPENED,  //open
    MMIMP3_AUDIO_PLAYING, //play
    MMIMP3_AUDIO_PAUSED,  //pause
    MMIMP3_AUDIO_STOPPED, //stop
    MMIMP3_AUDIO_CLOSED,  //close
    MMIMP3_AUDIO_STATE_MAX
} MMIMP3_AUDIO_STATE_E;

//Mp3 review state
typedef enum
{
    MMIMP3_REVIEW_STATE_NONE,   //none
    MMIMP3_REVIEW_STATE_SET,    //setting
    MMIMP3_REVIEW_STATE_PLAY,   //play
} MMIMP3_REVIEW_STATE_TYPE_E;

//当前播放的动态状态信息
typedef struct
{
    uint32                       total_time;         //当前激活的Mp3播放时长
    uint32                       cur_time;           //当前音频播放的播放时刻
    uint32                       total_length;       //当前激活的Mp3文件大小(字节长度)
    uint32                       cur_offset;         //当前音频播放的播放位
    uint32                       bit_rate;           //bit rate
} MMIMP3_PLAYING_DYN_INFO_T;

//专辑信息
typedef struct MMIMP3_ALBUM_INFO_T_tag
{
    wchar   artist[MMIMP3_COMMON_TAG_LEN +1];
    wchar   album[MMIMP3_COMMON_TAG_LEN +1];
    
    uint8   *img_data_ptr;
    uint32  data_size;
    
} MMIMP3_ALBUM_INFO_T;

extern const wchar g_mmimp3_reserved_dir[];

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIMP3_GetBGPlayerHandle        MMIAPIMP3_GetBGPlayerHandle
#ifdef MMIAP_LYRIC_SUPPORT
#define MMIMP3_GetLyricTimerId          MMIAPIMP3_GetLyricTimerId
#endif
#ifdef BLUETOOTH_SUPPORT
#define MMIMP3_IsThroughBt              MMIAPIMP3_IsThroughBt
#endif
#define MMIMP3_SetVolume                MMIAPIMP3_SetVolume
#define MMIMP3_PauseMp3                 MMIAPIMP3_PauseMp3
#define MMIMP3_ResumeMp3                MMIAPIMP3_ResumeMp3
#define MMIMP3_ForwardMp3               MMIAPIMP3_ForwardMp3
#define MMIMP3_BackwardMp3              MMIAPIMP3_BackwardMp3
#define MMIMP3_GetReviewStartPoint      MMIAPIMP3_GetReviewStartPoint
#define MMIMP3_GetReviewEndPoint        MMIAPIMP3_GetReviewEndPoint
#ifdef DRM_SUPPORT
#define MMIMP3_IsDrmFile                MMIAPIMP3_IsDrmFile
#define MMIMP3_ConsumeDrmRight          MMIAPIMP3_ConsumeDrmRight
#endif
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
#define MMIMP3_GetPreOrNextMp3Index	    MMIAPIMP3_GetPreOrNextMp3Index
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
//  Description : initialize mp3 module
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_InitModule(void);

/*****************************************************************************/
//  Description : initialize Mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_Init(void);

/*****************************************************************************/
//  Description : get ap button info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void GetAPButtonInfo(MMIAP_BUTTON_INFO_T *button_ptr);

/*****************************************************************************/
//  Description : set ap button info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void SetAPButtonInfo(MMIAP_BUTTON_INFO_T *button_ptr);

/*****************************************************************************/
//  Description : get mp3bg handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAPIMP3_GetBGPlayerHandle(void);

/*****************************************************************************/
//  Description : open player menu window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_OpenPlayerMenuWin(void);

/*****************************************************************************/
//  Description : open mp3 player
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_OpenMp3Player(void);

/*****************************************************************************/
//  Description : handle progress timer msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleProgressTimer(uint8 timer_id, uint32 param);
#ifdef MMIAP_LYRIC_SUPPORT
/*****************************************************************************/
//  Description : get lyric display timer id
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPIMP3_GetLyricTimerId(void);

/*****************************************************************************/
//  Description : handle mp3 lyric timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleLyricTimer(uint8 timer_id, uint32 param);
#endif
/*****************************************************************************/
//  Description : Mp3正常播放结束的回调函数。
//              根据播放模式重新显示进度条和button和是否继续播放其他mp3等
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_PlayMp3CallBack(void);

/*****************************************************************************/
//  Description : 跳转到前一首或者下一首Mp3。
//              根据播放模式重新显示进度条和button和是否继续播放其他mp3等
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_GoToPreOrNextMp3(
                        BOOLEAN      is_prev
                        );

/*****************************************************************************/
//  Description : set the volume of mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_SetMp3Volume(//volume is change
                      BOOLEAN  is_increase
                      );

/*****************************************************************************/
//  Description : get current mp3 name,include suffix
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: only for sub screen and idle
/*****************************************************************************/
PUBLIC void MMIAPIMP3_GetCurMp3Name(
                    MMI_STRING_T   *mp3_name_ptr,
                    uint16         max_name_len
                    );

/*****************************************************************************/
//  Description : handle camera key
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleCameraKey(void);

/*****************************************************************************/
//  Description : when format udisk,reset global variabel
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_ResetAllMp3(void);

/*****************************************************************************/
//  Description : if it is current mp3 background play
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsBgPlayMp3(
                                     const wchar *full_name_ptr, //in
                                     uint16      full_name_len   //in
                                     );

/*****************************************************************************/
//  Description : play music from my document
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_PlayMyDocumentMusic(
                                          MMIFILE_FILE_INFO_T   *music_file_info_ptr
                                          );

/*****************************************************************************/
//  Description : set mp3 as call ring
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SetMp3AsCallRing(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set the mode of mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_EnableEq(MMISRVAUD_EQ_MODE_E  eq_mode);

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopOperaSDPlug(void);

/*****************************************************************************/
//  Description : stop current operation when bt receive file
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopOperaBtAction(void);

/*****************************************************************************/
//  Description :  close music play from my document
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_CloseMyDocumentMusicPlay(void);

/*****************************************************************************/
//  Description : stop music
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopMusicPlay(void);

/*****************************************************************************/
//  Description : To judge whether the file is the type which is loaded by mp3 player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsMp3PlayerLoadType(
                                             const wchar *full_path_ptr,     //in
                                             uint16      full_path_len       //in，以双字节计算
                                             );

/*****************************************************************************/
//  Description : Init my music module.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_InitMyMusic(void);

/*****************************************************************************/
//  Description : Play Fmm music.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PlayFmmMusic(
                    MMIFMM_DATA_INFO_T  *file_info_ptr,
                    uint16              file_num,
                    uint16              file_index,
                    wchar               *dir_path_ptr,
                    uint16              dir_path_len
                    );

/*****************************************************************************/
//  Description : Play Fmm music file
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PlayFile(const wchar *file_name_ptr,  
                                  const uint32 filename_len
                                  );

/*****************************************************************************/
//  Description : Get player state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIMP3_PLAYER_STATE_E MMIAPIMP3_GetPlayerState(
                                    void
                                    );

/*****************************************************************************/
//  Description : Set player state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SetPlayerState(
                    MMIMP3_PLAYER_STATE_E cur_state
                    );
#ifdef MMIAP_LYRIC_SUPPORT
/*****************************************************************************/
//  Description : Whether it is the lyric of the current background play
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsBgPlayLyric(
                        uint16      *full_name_ptr, //in
                        uint16     full_name_len   //in
                        );
#endif
/*****************************************************************************/
//  Description : Whether the file can be deleted.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsFileUsed(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        );

/*****************************************************************************/
//  Description : Whether the file is in the folder of MMIMP3_RESERVED_DIR.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsInternalDataFile(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        );

/*****************************************************************************/
//  Description : Judge whether the music is supported to play with a2dp.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsFileSupportA2DP(
                    const wchar *full_path_ptr,
                    uint16      full_path_len
                    );

/*****************************************************************************/
//  Description : whether the audio player is playing
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerPlaying(void);

/*****************************************************************************/
//  Description : whether the audio player is paused
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerPaused(void);

/*****************************************************************************/
//  Description : whether the audio player is stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerStopped(void);

/*****************************************************************************/
//  Description : whether the audio player is not stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerActive(void);

/*****************************************************************************/
//  Description : Get the delay timer, for playing mp3 in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIMP3_GetDelayPlayTimer(
                    void
                    );

/*****************************************************************************/
//  Description : Play mp3 in a delayed manner, handle delay timer,begin to play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleDelayPlayTimer(
                   uint8 timer_id,
                   uint32 param
                    );

/*****************************************************************************/
//  Description : Start play audio player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PlayAudioPlayer(
                    uint32    mp3_offset
                    );

/*****************************************************************************/
//  Description : Stop audio player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopAudioPlayer(
                    void
                    );
#ifdef MMIAP_LYRIC_SUPPORT
/*****************************************************************************/
//  Description : start mp3 lyric asynchronously
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StartLyric(
                    void
                    );
#endif
/*****************************************************************************/
//  Description : Handle Fatal Error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleFatalError(void);

/*****************************************************************************/
//  Description : MMI API MP3 Restore Eq Setting
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_RestoreEqSetting(void);

/*****************************************************************************/
//  Description : To judge whether the music is supported to play with a2dp.
//  Global resource dependence : 
//  Author:
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsSupportA2DPForCurPlay(void);

/*****************************************************************************/
//  Description : enter audio player after check
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_OpenAudioPlayerAfterCheck(void);

/*****************************************************************************/
//  Description : current play music file info is init or not 
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsLoadCurPlayInfo(void);

/*****************************************************************************/
//  Description : handle Folder Full
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_FolderFull(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : Reset all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_ReSetFactory(
                    void
                    );

/*****************************************************************************/
//  Description : stop music
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopMusicPlayer(void);
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : Is Through Bt
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsThroughBt(
                    void
                    );
#endif
/*****************************************************************************/
//  Description : set the volume of mp3
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_SetVolume(
                        uint8  volume
                        );

/*****************************************************************************/
//  Description : Get current audio state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIMP3_AUDIO_STATE_E MMIAPIMP3_GetAudioState(
                                void
                                );

/*****************************************************************************/
//  Description : Pause the audio BG player
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PauseMp3(
    BOOLEAN is_notify_by_audio_service
);

/*****************************************************************************/
//  Description : Resume audio BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_ResumeMp3(
    BOOLEAN is_notify_by_audio_service
);

/*****************************************************************************/
//  Description : forward mp3
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_ForwardMp3(void);

/*****************************************************************************/
//  Description : backward mp3
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_BackwardMp3(void);

/*****************************************************************************/
//  Description : Get the playing Dyn Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_GetPlayingDynInfo(
                MMIMP3_PLAYING_DYN_INFO_T  *dyn_info_ptr,
                BOOLEAN                    is_real_time 
                );

/*****************************************************************************/
//  Description : Get the review state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIMP3_REVIEW_STATE_TYPE_E MMIAPIMP3_GetReviewStatus(
                                        void
                                        );

/*****************************************************************************/
//  Description : Get mp3 review starting point.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetReviewStartPoint(
                    uint32 *start_point_ptr
                    );

/*****************************************************************************/
//  Description : Get mp3 review end point.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetReviewEndPoint(
                    uint32 *end_point_ptr
                    );
#ifdef MMIAP_LYRIC_SUPPORT
/*****************************************************************************/
//  Description : seek lyric by time offset
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SeekLyric(
                uint32     mp3_offset,
                BOOLEAN    is_update
                );
#endif

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : Is Drm File Type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsDrmFile(
                const wchar    *file_name_ptr,      //[IN]
                uint16          name_len,           //[IN]
                MMISRVAUD_RING_FMT_E *audio_type_ptr,    //[OUT]
                BOOLEAN         *is_audio_drm_ptr,  //[OUT]
                BOOLEAN         *is_drm_valid_ptr   //[OUT]
                );

/*****************************************************************************/
//  Description : consume drm right
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_ConsumeDrmRight(void);

#endif  //#ifdef DRM_SUPPORT

/*****************************************************************************/
//  Description : Stop MP3 applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopApplet(BOOLEAN is_minimize);

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
/*****************************************************************************/
//  Description : 获取前一首或者下一首mp3索引
//  Global resource dependence : 
//  Author:xin.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetPreOrNextMp3Index(
                                           BOOLEAN is_prev,
                                           uint32  *index_ptr //out
                                           );

/*****************************************************************************/
//  Description : 获取当前mp3专辑信息，包括歌手、专辑图片数据等.
//  Global resource dependence : 
//  Author:xin.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetCurMp3AlbumInfo(
                                         MMIMP3_ALBUM_INFO_T *album_ptr  //in/out
                                         );

/*****************************************************************************/
//  Description : update all album information.
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMP3_UpdateAllAlbumInfo(BOOLEAN is_prev);       

#endif //MMI_AUDIOPLAYER_ALBUM_SUPPORT

/*****************************************************************************/
//  Description : update album anim ctrl info
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 切换专辑图片信息
//        无专辑图片信息时默认专辑图片，非专辑显示模式时使用默认背景图片
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SwitchAnimCtrlInfo(BOOLEAN is_update, BOOLEAN is_prev);

/*****************************************************************************/
//  Description : whether the audio player is opened
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_IsOpened(void);

/*****************************************************************************/
//  Description : start mp3 by background
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAP_StartByBG(void);

/*****************************************************************************/
//  Description : destroy mp3 by background
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAP_DestroyByBG(void);

/*****************************************************************************/
//  Description : get cur file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetCurPlayFileFullPath(
    wchar * full_path_ptr,
    uint16 *full_path_len_ptr
);

#ifdef MMIAP_F_RELEASE_BG_AUDIO
/*****************************************************************************/
//  Description : set release audio handle flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAP_SetReleaseAudioHandleFlag(
    BOOLEAN is_release
);

/*****************************************************************************/
//  Description : get release audio handle flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_IsReleaseAudioHandle(
    void
);

/*****************************************************************************/
//  Description : release audio handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_ReleaseAudioHandle(
    void
);
#endif

/*****************************************************************************/
//  Description : get display flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_IsDisplayForPlayStateNone(
    BOOLEAN is_over_offset_zero, //in
    BOOLEAN is_check_total_length //in
);

/*****************************************************************************/
//  Description : get current play offset
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIAP_GetCurPlayOffset(
    uint32 org_offset
);

/*****************************************************************************/
//  Description : MMIMP3_SetReviewState
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetReviewState(BOOLEAN is_review_process_sucessful);

#ifdef BT_AV_SNK_SUPPORT
/*****************************************************************************/
//  Description : Update A2DP and AVRCP connect status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdateA2dpAvrcpConnectionStatus(BOOLEAN a2dp, BOOLEAN avrcp);

/*****************************************************************************/
//  Description : Update MP3 player attributes
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayerAttributes(AvrcpCtElementAttributes attr);

/*****************************************************************************/
//  Description : Update MP3 player status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayerStatus(AvrcpCtPlayStatus status);

/*****************************************************************************/
//  Description : Update MP3 player playback status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayback(uint8 status);

/*****************************************************************************/
//  Description : Update MP3 player playing position
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayPosition(uint32 position);

/*****************************************************************************/
//  Description : AVRCP Track changed
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_AvrcpTrackChanged(void);
#endif

#ifdef BT_AV_CT_SUPPORT
/*****************************************************************************/
//  Description : get avrcp play status
//  Global resource dependence :
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC AvrcpCtPlayStatus MMIAPIMp3_GetAvrcpPlayStatus(void);
/*****************************************************************************/
//  Description : get is alloc avrcp string memory
//  Global resource dependence :
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMp3_IsAllocAvrcpStringMemory(void);

/*****************************************************************************/
//  Description : init A2DP and AVRCP connect status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void InitA2dpAvrcpConnectionStatus(void);

/*****************************************************************************/
//  Description : Allocate AVRCP string memory
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void AllocateAvrcpStringMemory(void);

/*****************************************************************************/
//  Description : release AVRCP string memory
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void FreeAvrcpStringMemory(void); 

/*****************************************************************************/
//  Description : update avrcp player display
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void UpdateAvrcpPlayerDisplay(void);

/*****************************************************************************/
//  Description : Update AVRCP player button display
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void UpdateAvrcpPlayerButton(void);
#endif

#if defined BT_AV_CT_SUPPORT || defined BT_AV_SNK_SUPPORT
/*****************************************************************************/
//  Description : get is bt music
//  Global resource dependence :
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMp3_IsBtMusic(void);
#endif


/*****************************************************************************/
//  Description :headset plug in or out
//  Global resource dependence :
//  Author: chunjuan.liang
//  Note:bug909351
/*****************************************************************************/
PUBLIC void MMIAPIMp3_HeadsetInd(MMI_MESSAGE_ID_E msg_id);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
