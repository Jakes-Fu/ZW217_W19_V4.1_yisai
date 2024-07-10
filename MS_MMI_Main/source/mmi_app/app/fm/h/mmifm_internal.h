/*****************************************************************************
** File Name:      mmifm_exprot.h                                            *
** Author:         baokunyin                                                 *
** Date:           2008-12-1                                                 *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is header of fm                                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008-12-1      baokunyin       Creat
******************************************************************************/

#ifndef _MMI_FM_INTERNAL_H_
#define _MMI_FM_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifm_export.h"
#include "mmisrvaud_api.h"
#ifdef WIN32
#include "mmifm_win32_simu.h"
#else
#include "fm_drv.h"
#endif


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define MMIFM_FREQ_STEP                100//FM freq step
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
#define MMIFM_CHANNEL_NAME_LEN         30 //FM channel name max length
#else
#define MMIFM_CHANNEL_NAME_LEN         20 //FM channel name max length of MINI
#endif
#define MMIFM_FREQ_MAX_LEN             20 //FM freqency number max length
#define MMIFM_DIGIT_MAX_LEN            5 //FM freqency number max length
#define MMIFM_MAX_CHANNEL_NUM          40 //FM channel max number
#define MMIFM_FREQ_RANGE_LOW                875     //FM lowest freq
#define MMIFM_FREQ_RANGE_HIGH               1080    //FM highest freq
#define MMIFM_FREQ_INITIAL_VAL              875     //initial FM freq
#define MMIFM_INVALID_CHANNEL          0xFFFF//invalid channel index
#define MMIFMRECORD_MAX_FILE_NAME_LEN          MMIFMM_FULL_FILENAME_LEN
#define MMIFMRECORD_MIN_NEED_SPACE        20000//2000    //录音最小需要空间
#define MMIFMRECORD_UDISK_MIN_NEED_SPACE       100000    //录音U盘最小需要空间
#define MMIFM_RECORD_INITIAL_FILE_ID              0     //initial FM freq
#define FMRECORD_FILE_NAME_ID_MAX				    10000000
#define FMRECORD_FILE_SEARCH_MAX_COUNT                   1000
#define MMIFM_ENDURE_TIME_MAX_LEN      4 //FM record endure time max length

#define MMIFM_VOL_LEVEL_MAX    MMISRVAUD_VOLUME_LEVEL_MAX
#define MMIFM_VOL_LEVEL_0      0    

#define MMIFILE_SLASH                       0x005c    //"\"

//#define  MMIFM_SUPPORT_FM_RECORD

#define  MMIFM_FREQ_SCALE          1

#ifdef MMI_PDA_SUPPORT
#ifndef FM_NEW_UI
#define FM_NEW_UI
#endif
#endif

#ifdef FM_NEW_UI

#define MMIFM_SLIDE_TIME   10
#define MMIFM_SEEK_TIME    800

#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_BASE 40 //320X480 
#define FM_GRENN_BACKGROUND_X_END_H_BASE  260

#ifdef MAINLCD_SIZE_320X480
#define FM_V_LEFT_ADJUST_BUTTON_SPACE       9
#define FM_V_FREQ_LIST_BUTTON_START_X       16
#define FM_V_FREQ_LIST_BUTTON_START_Y       198
#define FM_V_FRQ_MHZ_START_X                225
#define FM_V_FRQ_MHZ_BOTTOM_Y               144
#define FM_V_FRQ_DATA_SPACE                 8
#define FM_V_FRQ_BG_START_X                 13
#define FM_V_FRQ_BG_START_Y                 29
#define FM_V_FRQ_BG_WIDTH                   294
#define FM_V_FRQ_POINTER_CENTER_X           160
#define FM_V_FRQ_POINTER_CENTER_Y           361
#define FM_V_RETURN_BUTTON_X_START          15
#define FM_V_RETURN_BUTTON_Y_START          35

#define FM_H_LEFT_ADJUST_BUTTON_X_SPACE     12
#define FM_H_FREQ_LIST_BG_X_SPACE           3
#define FM_H_FREQ_LIST_BUTTON_START_Y       266
#define FM_H_FRQ_MHZ_START_X                150
#define FM_H_FRQ_MHZ_BOTTOM_Y               215
#define FM_H_FRQ_DATA_BOTTOM_Y              176
#define FM_H_FRQ_DATA_SPACE                 8
#define FM_H_FRQ_BG_START_X                 11
#define FM_H_FRQ_BG_START_Y                 27
#define FM_H_FRQ_BG_WIDTH                   244
#define FM_H_FRQ_POINTER_CENTER_X           368
#define FM_H_FRQ_POINTER_CENTER_Y           152
#define FM_H_RETURN_BUTTON_X_START          13
#define FM_H_RETURN_BUTTON_Y_START          33
#define FM_H_BUTTON_START_X  260
#define FM_H_SLIDE_START_Y  75
#define FM_V_SLIDE_START_Y  242
#define FM_H_FRE_DIGITAL_Y  144
#define FM_V_FRE_DIGITAL_Y  89
#define FM_H_FRE_DIGITAL_RIGHT  170
#define FM_V_FRE_DIGITAL_RIGHT  202
#define FM_ONE_SLIDE_WIDTH      5
#define FM_V_SLIDE_INTERVAL     12
#define FM_H_SLIDE_INTERVAL     9

#define MMIFM_SLIDE_NEED_STEP   10

#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_BASE //320X480
#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_BASE
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_V  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V/2)
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_H  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H/2)
#define FM_GRENN_BACKGROUND_X_END_H  FM_GRENN_BACKGROUND_X_END_H_BASE

#define FM_LAYER_HEIGHT_V  340 
#else 
#ifdef MAINLCD_SIZE_240X400  //for pda
#define FM_V_LEFT_ADJUST_BUTTON_SPACE       3
#define FM_V_FREQ_LIST_BUTTON_START_X       10
#define FM_V_FREQ_LIST_BUTTON_START_Y       166
#define FM_V_FRQ_MHZ_START_X                182
#define FM_V_FRQ_MHZ_BOTTOM_Y               123
#define FM_V_FRQ_DATA_SPACE                 5
#define FM_V_FRQ_BG_START_X                 8
#define FM_V_FRQ_BG_START_Y                 28
#define FM_V_FRQ_BG_WIDTH                   224
#define FM_V_FRQ_POINTER_CENTER_X           120
#define FM_V_FRQ_POINTER_CENTER_Y           297
#define FM_V_RETURN_BUTTON_X_START          9
#define FM_V_RETURN_BUTTON_Y_START          33

#define FM_H_LEFT_ADJUST_BUTTON_X_SPACE     8
#define FM_H_FREQ_LIST_BG_X_SPACE           2
#define FM_H_FREQ_LIST_BUTTON_START_Y       200
#define FM_H_FRQ_MHZ_START_X                172
#define FM_H_FRQ_MHZ_BOTTOM_Y               141
#define FM_H_FRQ_DATA_BOTTOM_Y              FM_H_FRQ_MHZ_BOTTOM_Y
#define FM_H_FRQ_DATA_SPACE                 5
#define FM_H_FRQ_BG_START_X                 9
#define FM_H_FRQ_BG_START_Y                 26
#define FM_H_FRQ_BG_WIDTH                   213
#define FM_H_FRQ_POINTER_CENTER_X           311
#define FM_H_FRQ_POINTER_CENTER_Y           111
#define FM_H_RETURN_BUTTON_X_START          10
#define FM_H_RETURN_BUTTON_Y_START          32
#define FM_H_BUTTON_START_X  224
#define FM_H_SLIDE_START_Y  57
#define FM_V_SLIDE_START_Y  207
#define FM_H_FRE_DIGITAL_Y  107
#define FM_V_FRE_DIGITAL_Y  77
#define FM_H_FRE_DIGITAL_RIGHT  139
#define FM_V_FRE_DIGITAL_RIGHT  152
#define FM_ONE_SLIDE_WIDTH      5
#define FM_V_SLIDE_INTERVAL     11
#define FM_H_SLIDE_INTERVAL     10

#define MMIFM_SLIDE_NEED_STEP   10

#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V 30 //320X480 
#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H 38
#define FM_GRENN_BACKGROUND_X_END_H  224
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_V  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V/2)
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_H  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H/2)

#define FM_LAYER_HEIGHT_V  270 
#else 
#ifdef  MAINLCD_SIZE_240X320//for pda
#define FM_V_LEFT_ADJUST_BUTTON_SPACE       10
#define FM_V_FREQ_LIST_BUTTON_START_X       10
#define FM_V_FREQ_LIST_BUTTON_START_Y       135
#define FM_V_FRQ_MHZ_START_X                182
#define FM_V_FRQ_MHZ_BOTTOM_Y               111
#define FM_V_FRQ_DATA_SPACE                 5
#define FM_V_FRQ_BG_START_X                 8
#define FM_V_FRQ_BG_START_Y                 27
#define FM_V_FRQ_BG_WIDTH                   224
#define FM_V_FRQ_POINTER_CENTER_X           120
#define FM_V_FRQ_POINTER_CENTER_Y           244
#define FM_V_RETURN_BUTTON_X_START          9
#define FM_V_RETURN_BUTTON_Y_START          32

#define FM_H_LEFT_ADJUST_BUTTON_X_SPACE     4
#define FM_H_FREQ_LIST_BG_X_SPACE           2
#define FM_H_FREQ_LIST_BUTTON_START_Y       195
#define FM_H_FRQ_MHZ_START_X                111
#define FM_H_FRQ_MHZ_BOTTOM_Y               162
#define FM_H_FRQ_DATA_BOTTOM_Y              135
#define FM_H_FRQ_DATA_SPACE                 3
#define FM_H_FRQ_BG_START_X                 7
#define FM_H_FRQ_BG_START_Y                 31
#define FM_H_FRQ_BG_WIDTH                   162
#define FM_H_FRQ_POINTER_CENTER_X           230
#define FM_H_FRQ_POINTER_CENTER_Y           101
#define FM_H_RETURN_BUTTON_X_START          8
#define FM_H_RETURN_BUTTON_Y_START          36
#define FM_H_BUTTON_START_X  170//播放右边界
#define FM_H_SLIDE_START_Y  65//滚轮外框开始处
#define FM_V_SLIDE_START_Y  155
#define FM_H_FRE_DIGITAL_Y  110//屏显数字频率的y值
#define FM_V_FRE_DIGITAL_Y  63
#define FM_H_FRE_DIGITAL_RIGHT  114//屏显数字频率的右边界
#define FM_V_FRE_DIGITAL_RIGHT  154
#define FM_ONE_SLIDE_WIDTH      4
#define FM_V_SLIDE_INTERVAL     11// 滚轮与滚轮shadow的间距
#define FM_H_SLIDE_INTERVAL     10

#define MMIFM_SLIDE_NEED_STEP   10 //一周滚轮数目

#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V   29

#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H 43
#define FM_GRENN_BACKGROUND_X_END_H  169
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_V  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V/2)
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_H  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H/2)

#define FM_LAYER_HEIGHT_V  200 
#else
#define FM_H_BUTTON_START_X  260
#define FM_H_SLIDE_START_Y  75
#define FM_V_SLIDE_START_Y  242
#define FM_H_FRE_DIGITAL_Y  144
#define FM_V_FRE_DIGITAL_Y  89
#define FM_H_FRE_DIGITAL_RIGHT  170
#define FM_V_FRE_DIGITAL_RIGHT  202
#define FM_ONE_SLIDE_WIDTH      4
#define FM_V_SLIDE_INTERVAL     12
#define FM_H_SLIDE_INTERVAL     9

#define MMIFM_SLIDE_NEED_STEP   15
#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V 40 //320X480 
#define FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H 50
#define FM_GRENN_BACKGROUND_X_END_H  (FM_GRENN_BACKGROUND_X_END_H_BASE*320/480)
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_V  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V/2)
#define FM_RETURN_MENU_BUTTON_X_START_OFFSET_H  (FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H/2)

#define FM_LAYER_HEIGHT_V  340 
#endif
#endif
#endif
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
//FM's sound mode
typedef enum
{
    FM_HEADSET_MODE,
    FM_SPEAKER_MODE
}MMIFM_AUDIO_DEVICE_MODE_E;

//FM search frequency type
typedef enum
{
    MMIFM_AUTOSEARCH = 0,
    MMIFM_GET_ONE_VALID
}MMIFM_SEARCH_FREQUENCY_TYPE_E;

//FM record type
typedef enum
{
    FM_record = 0,
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    FM_timer_record
#endif
}MMIFM_record_type_E;

//FM's start way
typedef enum
{
    NORMAL_START_FM,    
    PREVIEW_START_FM    //open fm by alarm
}MMIFM_START_WAY_E;
typedef enum
{
    HEADSET_BUTTON_SHORT_PRESS,    
    HEADSET_BUTTON_LONG_PRESS    
}MMIFM_HEADSET_BUTTON_ACTION_TYPE_E;
//FM app information struct
typedef struct  
{
    MMIFM_STATUS_E              status;         //current status
    uint16                      cur_freq;       //current freq
    uint16                      cur_channel_index;    //index of channels
    MMIFM_AUDIO_DEVICE_MODE_E   sound_mode;     //FM's sound mode 
    BOOLEAN                     is_bgplay;      //bg play setting
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)        
    uint32                      fm_record_file_id; //record file id
    MMIFILE_DEVICE_E            dev;
#endif
    BOOLEAN                     suspend_by_fm;  //suspend is set by fm
    BOOLEAN                     paused_with_fm;  //TRUE:record和fm同时暂停;FALSE:record界面单独暂停
    uint16                      export_freq;       //be used by other app
#ifdef MMIFM_SUPPORT_FORMAT_AMR 
    MMISRVAUD_RECORD_FMT_E      record_file_format;
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE
    MMISRVAUD_EQ_MODE_E         fm_eq_mode;
#endif
#ifdef FM_RDS_SUPPORT
    MMIFM_RDS_STATUS_E    rds_open;  //   rds status ,open or close
#endif
}MMIFM_CUR_INFO_T;

//FM timer record information struct
typedef struct  
{    
    uint16              endure_time;
    uint16 				record_freq;
}MMIFM_TIMER_RECORD_INFO_T;

//FM pre-defined freq list struct
typedef struct 
{                                      
    wchar       name[MMIFM_CHANNEL_NAME_LEN + 1]; //channel's name
    uint16      freqency;                    //channel's freqency
}MMIFM_CHANNEL_T;

typedef enum
{
    MMIFMRECORD_STATE_NONE,          // start state
    MMIFMRECORD_STATE_RECORDING,      // recording
    MMIFMRECORD_STATE_RECORD_PAUSED,
    MMIFMRECORD_STATE_MAX             // record is closed
}MMIFMRECORD_STATE_E;  //record state

typedef enum
{
    MMIFMRECORD_RESULT_SUCCESS,   //函数调用成功
    MMIFMRECORD_RESULT_ERROR,     //函数调用错误
    MMIFMRECORD_RESULT_RENAME_EMPTY,   //原名字或目标名字为空
    MMIFMRECORD_RESULT_RENAME_SAME,   //原名字和目标名字相同
    MMIFMRECORD_RESULT_RENAME_EXIST,  //目标名字对应文件已经存在
    MMIFMRECORD_RESULT_MAX_FILE,  //文件达到最大数目
    MMIFMRECORD_RESULT_NO_SPACE,   //空间不足
    MMIFMRECORD_RESULT_FOLDER_FULL, //文件夹数量满
    MMIFMRECORD_RESULT_DEVICE_NOT_EXIST //设备不存在
}MMIFMRECORD_RESULT_E;    //函数调用结果
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIFM_DEFAULT_BIG_FONT	        SONG_FONT_18
#elif defined(MAINLCD_SIZE_128X64) 
#define MMIFM_DEFAULT_BIG_FONT	        SONG_FONT_14
#elif defined(MAINLCD_SIZE_240X400)
#define MMIFM_DEFAULT_BIG_FONT	        SONG_FONT_24
#elif defined(MAINLCD_SIZE_240X320)
#define MMIFM_DEFAULT_BIG_FONT	        SONG_FONT_24
#else
#define MMIFM_DEFAULT_BIG_FONT	        SONG_FONT_24
#endif

#ifdef FM_NEW_UI
typedef struct 
{
    MMI_HANDLE_T   handle;
    MMK_TP_SLIDE_E slide_state; 
    GUI_POINT_T slide_pre_point;
    uint8       draw_slide_timer_id;
    uint16      cur_freq;
}MMIFM_SLIDE_INFO_T;
#endif

typedef struct
{
    uint16 channel_index;
    uint16 freq;
}MMIFM_FREQ_LIST_INFO_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                        *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init FM application
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIFM_Init(void);

/*****************************************************************************/
// 	Description : intial FM module  
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
void MMIFM_InitModule(void);

/*****************************************************************************/
// 	Description : open play FM main window
//	Global resource dependence : 
//  Author
//	Note: 
/*****************************************************************************/
PUBLIC void MMIFM_OpenMainWin(void);

/*****************************************************************************/
// 	Description : Play FM
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Play(BOOLEAN is_fm_first_open);

/*****************************************************************************/
// 	Description : get FM's status, play, stop or suspended
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
MMIFM_STATUS_E MMIFM_GetStatus(void);

/*****************************************************************************/
// 	Description : adjust FM sound volume
//	Global resource dependence : 
//  Parameter: adjust volume to current volume + step 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustVolume(int32 step);
/*****************************************************************************/
// 	Description : adjust FM sound volume TP
//	Global resource dependence : 
//  Parameter:  
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustVolumeTP(int32 step);
/*****************************************************************************/
// 	Description : 退出FM
//	Global resource dependence : 
//  Author:
//  Parameter: TRUE: only exit fm, do not stop fm play; FALSE:exit fm, and stop fm play
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Exit(BOOLEAN is_set_bg_play);

/*****************************************************************************/
// 	Description : stop fm playing
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Stop(void);

/*****************************************************************************/
// 	Description : pause fm playing
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Pause(BOOLEAN suspend_by_fm);

/*****************************************************************************/
// 	Description : get FM's status, play, stop or suspended
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
MMIFM_STATUS_E MMIFM_GetStatus(void);

/*****************************************************************************/
// 	Description : adjust FM sound volume
//	Global resource dependence : 
//  Author:
//  Parameter: adjust volume to current volume + step 
//	Note: 
/*****************************************************************************/
BOOLEAN MMIFM_AdjustVolume(int32 step);

/*****************************************************************************/
// 	Description : adjust fm frequency, increase 0.1 MHz or decrease 0.1 MHz freq
//	Global resource dependence : 
//  Author:
//  Parameter: step: adjust steps, 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustFrequency(int32 step);

/*****************************************************************************/
// 	Description : adjust fm channel
//	Global resource dependence : 
//  Author:
//  Parameter: step: adjust steps, 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustChannel(int32 step);

/*****************************************************************************/
// 	Description : 切换声音设备
//	Global resource dependence : 
//  Author:
//  Parameter: 无
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_SwitchAudioDevice(void);

/*****************************************************************************/
// 	Description : 搜索下一个或上一个有效频率
//	Global resource dependence : 
//  Author:
//  Parameter: direct >= 0:搜索下一个有效频率; direct < 0 :上一个有效频率
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFM_SearchValidFreq(int32 direct);

/*****************************************************************************/
// 	Description : save one channel
//	Global resource dependence : 
//  Parameter : channel_index[IN]: channel index to be saved
//              channel_ptr[IN]: channel info to be saved
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_SetOneChannel(uint16 channel_index, const MMIFM_CHANNEL_T * channel_ptr);

/*****************************************************************************/
// 	Description : delete one channel
//	Global resource dependence : 
//  Parameter : channel_index[IN]: channel index to be readed
//              channel_ptr[OUT]: the information of this channel
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_GetOneChannel(uint16 channel_index, MMIFM_CHANNEL_T * channel_ptr);

/*****************************************************************************/
// 	Description : get fm current inof
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC MMIFM_CUR_INFO_T * MMIFM_GetFMCurrentInfo(void);
/*****************************************************************************/
//  Description : Get FM Timer Record Info
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_TIMER_RECORD_INFO_T * MMIFM_GetFMTimerRecordInfo(void);
/*****************************************************************************/
// 	Description : get FM Record File ID
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
MMIFM_STATUS_E MMIFM_GetRecordFileId(void);
/*****************************************************************************/
// 	Description : get fm audio current device mode
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC MMIFM_AUDIO_DEVICE_MODE_E MMIFM_GetFMCurrentDeviceMode(void);

/*****************************************************************************/
// 	Description : auto search freqency for every channels
//	Global resource dependence : 
//  Parameter : pre_search_channel [IN] current channel need to searched
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFM_AuotSearchFreq(uint16 cur_search_channel);
/*****************************************************************************/
// 	Description : play start from the channel frequency
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_PlayChannel(uint16 ch_index);

/*****************************************************************************/
// 	Description : delete one channel
//	Global resource dependence : 
//  Parameter : channel[IN]: channel index to be delete
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_DeleteOneChannel(uint16 ch_index);

/*****************************************************************************/
// 	Description : delete all channels
//	Global resource dependence : 
//  Parameter : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_DeleteAllChannels(void);

/*****************************************************************************/
// 	Description : is channel exist
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_IsChannelExist(uint16 ch_index);

/*****************************************************************************/
// 	Description : is valid channel
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_IsChannelValid(const MMIFM_CHANNEL_T *channel_ptr);

/*****************************************************************************/
// 	Description : get exist channel total count
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC uint16  MMIFM_GetValidChannelCount(void);

/*****************************************************************************/
// 	Description : register fm module nv len and max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIFM_RegFMNv(void);

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIFM_RegWinIdNameArr(void);

/*****************************************************************************/
// 	Description : handle headset plug in and plug out action
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIFM_HandleHeadsetAction(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : close all of fm windows
//	Global resource dependence : 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIFM_ExitAllWindows(void);


/*****************************************************************************/
// 	Description : auto test Play FM
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_EngTestPlay(void);

/*****************************************************************************/
// 	Description : stop FM in eng test
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIFM_EngTestStop(void);

/*****************************************************************************/
//  Discription: stop auto search
//  Global resource dependence: none 
//  Parameter: 
//  Note:
//
/*****************************************************************************/
PUBLIC  void  MMIFM_StopAutoSearch(void);

/*****************************************************************************/
// 	Description : set current channel index
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_SetCurrrentChannel(const uint16 ch_index);

/*****************************************************************************/
// 	Description : get channel's display name
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIFM_GetChannelDispName(const MMIFM_CHANNEL_T *channel_ptr, 
                              uint16 channel_index, 
                              wchar *disp_name_ptr, 
                              uint16 max_name_len);

/*****************************************************************************/
// 	Description : get channel index by frequency
//	Global resource dependence : 
//  Author:
//  return return value is not MMIFM_INVALID_CHANNEL, the frequency is in channel
//	Note: 
/*****************************************************************************/
PUBLIC uint16  MMIFM_GetChannelIndexByFreq(uint16 frequency);

/*****************************************************************************/
// 	Description : enable FM mute 
//	Global resource dependence : 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_EnableMute(void);

/*****************************************************************************/
// 	Description : restore FM to user setting volume
//	Global resource dependence : 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_DisableMute(void);

/*****************************************************************************/
// 	Description : set fm digital type
//	Global resource dependence : 
//  Parameter:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIFM_SetFmDigitalType(void);
/*****************************************************************************/
// 	Description : Play FM
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_PlayInputFreq(uint16 input_freq,MMISRV_HANDLE_T audio_handle,BOOLEAN is_alarm_freq);
/*****************************************************************************/
// 	Description : Create Alarm FM Channel List
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIFM_CreateAlarmChannelList(ADD_DATA param);
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : get record status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_STATE_E MMIFM_GetRecordStatus(void);

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordConstructor(void);
/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordDestructor(void);

/*****************************************************************************/
// 	Description : Get Record File Name
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIFM_GetRecordFileName(void);
/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIFM_GetRecordFilePath(void);
/*****************************************************************************/
//  Description : 暂停录音
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordPauseRecord(BOOLEAN suspend_with_fm);
/*****************************************************************************/
//  Description : 继续录音
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordResumeRecord(BOOLEAN resume_with_fm);
/*****************************************************************************/
//  Description : set record status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_E state);
/*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_RESULT_E MMIFM_RecordStopRecord(void);
/*****************************************************************************/
//  Description : get finally slash index
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFM_RecordGetFinallySlashIndex(const wchar *file_name_ptr,const int32 file_len);
/*****************************************************************************/
//  Description : open record file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordOpenRecordFileList(void);
/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMRECORD_SetRecordStorage(MMIFILE_DEVICE_E storage);
/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIFMRECORD_GetRecordStorage(void);
#endif
#ifdef MMIFM_SUPPORT_FORMAT_AMR 
/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RECORD_FMT_E MMIFMRECORD_GetFileFormat(void);
/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMRECORD_SetFileFormat(MMISRVAUD_RECORD_FMT_E file_format);
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE 
/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_EQ_MODE_E MMIFM_GetEqMode(void);
/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_SetEqMode(MMISRVAUD_EQ_MODE_E fm_eq_mode);
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_RESULT_E MMIFM_RecordDeleteTempFile(void);
/*****************************************************************************/
// 	Description : create capture photo name
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIFM_GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type);
/*****************************************************************************/
//  Description : 开始录音（通常情况）
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFM_StartRecord(void);
#endif
/*****************************************************************************/
// 	Description : if fm suspend status is setted by itself 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_SetSuspendByFM(BOOLEAN suspend_by_fm);
/*****************************************************************************/
// 	Description : get who set fm suspend status 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_GetSuspendByFM(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Parameter: TRUE:record和fm同时暂停;FALSE:record界面单独暂停
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_SetPausedWithFM(BOOLEAN suspend_with_fm);
/*****************************************************************************/
//  Description : get who set fm suspend status 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_GetPausedWithFM(void);
/*****************************************************************************/
// 	Description : set FM's status, play, stop or suspended
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIFM_SetStatus(MMIFM_STATUS_E status);
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
// 	Description : if record name be modified 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_CheckRecordNameBeModified(const wchar *input_str);
/*****************************************************************************/
// 	Description : Close FM Record, used for other apps
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
#endif
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)
PUBLIC void MMIFM_StopRecord(void);
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Discription: append item to list control
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIFM_SetStringListItem(
                                      MMI_TEXT_ID_T    text_id,
                                      GUIITEM_STYLE_E  item_style,
                                      MMI_TEXT_ID_T    left_softkey_id,
                                      MMI_TEXT_ID_T    middle_softkey_id,
                                      MMI_TEXT_ID_T    right_softkey_id,
                                      MMI_CTRL_ID_T    ctrl_id,
                                      BOOLEAN          is_update,
                                      uint16           pos
                                      );
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Discription: Get Timer Endure
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_GetTimerRecordInfo(MMIFM_TIMER_RECORD_INFO_T *record_info_ptr);
/*****************************************************************************/
//  Discription: Set Timer Endure
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_SetTimerRecordInfo(const MMIFM_TIMER_RECORD_INFO_T *record_info_ptr);
/*****************************************************************************/
// 	Description : change  MMI_STRING_T to integer value
//	Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC int16 ConvertStringToInt(const MMI_STRING_T *str_ptr, int32 mul_scale);
/*****************************************************************************/
//  Description : Create FM timer record 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void StartFMTimerRecord(void);
/*****************************************************************************/
//  Description : Create FM timer record 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_StartFMTimerRecord(void);
/*****************************************************************************/
//  Description : Create FM Timer Record Win
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CreateFMTimerRecordInfoWin(void);
#endif
/*****************************************************************************/
//  Description : reset timer fm info
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_Reset(void);

/*****************************************************************************/
//  Description : set fm audio device mode
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_SetFMDeviceMode(MMIFM_AUDIO_DEVICE_MODE_E sound_mode);

/*****************************************************************************/
//  Description : is valid FRE
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_IsValidFre(uint16 fre);

#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : adjust fm frequency, increase 0.1 MHz or decrease 0.1 MHz freq
//  Global resource dependence : 
//  Author:
//  Parameter: step[IN]: adjust steps, 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustFrequencyByFre(uint16 fre);
#endif

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register FM module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RegAppletInfo(void);

 /*****************************************************************************/
//  Description : Stop FM applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopApplet(BOOLEAN is_minimize);

#endif
/*****************************************************************************/
//  Description : preview fm frequency status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_IsPreviewFM(void);
#endif


/*****************************************************************************/
//  Description : Request FM play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_ReqPlaySrv(void);

/*****************************************************************************/
//  Description : Request FM play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_ReqPlaySrv(void);

/*****************************************************************************/
//  Description : Get FM play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIFM_GetPlayHandle(void);

/*****************************************************************************/
//  Description : Get FM record time.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIFM_GetRecTime(void);
/*****************************************************************************/
//  Description : Check Headset Status
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_CheckHeadsetStatus(void);

/*****************************************************************************/
//  Description : Play fm freq for others app module
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_PlayByAudioHandle(uint16 input_freq,MMISRV_HANDLE_T audio_handle);
/*****************************************************************************/
//  Description : Play specified freq
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_PlayFreq(uint16 freq);
/*****************************************************************************/
//  Description : Handle headset button action
//  Global resource dependence : 
//  Author:
//  Parameter: 无
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_HandleHeadsetButtonAction(MMIFM_HEADSET_BUTTON_ACTION_TYPE_E action_type);
