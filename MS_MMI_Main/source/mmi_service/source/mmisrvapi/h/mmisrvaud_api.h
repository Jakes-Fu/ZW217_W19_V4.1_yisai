/****************************************************************************
** File Name:      mmisrvaud.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             22/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the data struct of                 
**                     AUDIO service module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 22/04/2011       Yintang.ren         Create
** 
****************************************************************************/
#ifndef _MMISRV_AUD_API_H_
#define _MMISRV_AUD_API_H_

#include "mmisrv.h"
#include "mmisrvmgr.h"
#include "mp3_adp.h"
#include "midi_adp.h"
#include "wav_adp.h"

#define STR_SRV_AUD_NAME    "audio service"

/* the name length of the client name, the client is the app module  who request the service */
#define MMISRVAUD_STR_CLIENT_NAME_LEN 30

#define MMISRVAUD_TONE_FREQ_DEFAULT 2000

/* prioity */
#define MMISRVAUD_PRI_HIGH_P10    310
#define MMISRVAUD_PRI_HIGH         300
#define MMISRVAUD_PRI_HIGH_N10    290

#define MMISRVAUD_PRI_NORMAL_P10 210
#define MMISRVAUD_PRI_NORMAL      200
#define MMISRVAUD_PRI_NORMAL_N10 190

#define MMISRVAUD_PRI_LOW_P10     110
#define MMISRVAUD_PRI_LOW          100
#define MMISRVAUD_PRI_LOW_N10     90

typedef  uint16 MMISRVAUD_PRI_T;

#define MMISRVAUD_EARPHONE_EVENT_CLK        0xe100
#define MMISRVAUD_EARPHONE_EVENT_LONG       0xe200
#define MMISRVAUD_EARPHONE_EVENT_DBLCLK     0xe300
typedef uint32 MMISRVAUD_EARPHONE_EVENT_ID;

typedef enum
{
    MMISRVAUD_GENERIC_TONE,
    MMISRVAUD_CUSTOM_TONE,
    MMISRVAUD_DTMF_TONE,
    MMISRVAUD_TONE_MAX
}MMISRVAUD_TONE_TYPE_E;

typedef enum
{
    MMISRVAUD_RING_FMT_NONE = -1,
    MMISRVAUD_RING_FMT_MIDI,
    MMISRVAUD_RING_FMT_GENERIC_TONE,
    MMISRVAUD_RING_FMT_CUSTOM_TONE,
    MMISRVAUD_RING_FMT_DTMF_TONE,
    MMISRVAUD_RING_FMT_SMAF,
    MMISRVAUD_RING_FMT_WAVE,
    MMISRVAUD_RING_FMT_MP3,
    MMISRVAUD_RING_FMT_WMA,
    MMISRVAUD_RING_FMT_AMR,
    MMISRVAUD_RING_FMT_AAC,
    MMISRVAUD_RING_FMT_M4A,    
    MMISRVAUD_RING_FMT_MAX
} MMISRVAUD_RING_FMT_E;

typedef enum
{
    MMISRVAUD_GENERIC_TONE_PRESS_KEY = 0x0000,
    MMISRVAUD_GENERIC_TONE_WARNING,
    MMISRVAUD_GENERIC_TONE_BATT_WARNING,
    MMISRVAUD_GENERIC_TONE_DIAL,
    MMISRVAUD_GENERIC_TONE_CONGESTION,
    MMISRVAUD_GENERIC_TONE_SUBSCRIBER_BUSY,
    MMISRVAUD_GENERIC_TONE_SPECIAL_DIAL,
    MMISRVAUD_GENERIC_TONE_SV_CALL_WAITING,
    MMISRVAUD_GENERIC_TONE_SV_RADIO_PATH_ACK,
    MMISRVAUD_GENERIC_TONE_SV_RADIO_PATH_NOT_AVAIL,
    MMISRVAUD_GENERIC_TONE_SV_ERROR_INFO,
    MMISRVAUD_GENERIC_TONE_INFO_FREE,
    MMISRVAUD_GENERIC_TONE_MAX
}MMISRVAUD_GENERIC_TONE_E;

typedef enum
{
    MMISRVAUD_DTMF_TONE_ONE = 0x0000,
    MMISRVAUD_DTMF_TONE_TWO,
    MMISRVAUD_DTMF_TONE_THREE,
    MMISRVAUD_DTMF_TONE_LETTERA,    
    MMISRVAUD_DTMF_TONE_FOUR,    
    MMISRVAUD_DTMF_TONE_FIVE,    
    MMISRVAUD_DTMF_TONE_SIX,    
    MMISRVAUD_DTMF_TONE_LETTERB,
    MMISRVAUD_DTMF_TONE_SEVEN,  
    MMISRVAUD_DTMF_TONE_EIGHT,  
    MMISRVAUD_DTMF_TONE_NINE,   
    MMISRVAUD_DTMF_TONE_LETTERC,
    MMISRVAUD_DTMF_TONE_STAR,   
    MMISRVAUD_DTMF_TONE_ZERO,  
    MMISRVAUD_DTMF_TONE_POND,  
    MMISRVAUD_DTMF_TONE_LETTERD,
    MMISRVAUD_DTMF_TONE_MAX
}MMISRVAUD_DTMF_TONE_E;

typedef enum
{
    MMISRVAUD_REPORT_RESULT_SUCESS = 0,
    MMISRVAUD_REPORT_RESULT_STOP,
    MMISRVAUD_REPORT_RESULT_NOT_SUPPORT,
    MMISRVAUD_REPORT_RESULT_ERROR,
    MMISRVAUD_REPORT_RESULT_ERROR_UNKNOWN,
    
    MMISRVAUD_REPORT_RESULT_MAX
}MMISRVAUD_REPORT_RESULT_E;

/* return value */
typedef enum
{
    MMISRVAUD_RET_ERROR,        /* operation error */
    MMISRVAUD_RET_OK,             /* operation OK */
    MMISRVAUD_RET_PENDING,    /* operation was not excuted, it is waiting some other condition such like BT connected */
    MMISRVAUD_RET_MAX
}MMISRVAUD_RET_E;

typedef enum
{
    MMISRVAUD_BT_EVENT_NONE,
    MMISRVAUD_BT_EVENT_ACTIVE,
    MMISRVAUD_BT_EVENT_DEACTIVE,
    MMISRVAUD_BT_EVENT_OPENED,
    MMISRVAUD_BT_EVENT_CLOSED,
    MMISRVAUD_BT_EVENT_KEY_PAUSE,
    MMISRVAUD_BT_EVENT_KEY_RESUME,
    MMISRVAUD_BT_EVENT_KEY_STOP,
    MMISRVAUD_BT_EVENT_KEY_PREV,
    MMISRVAUD_BT_EVENT_KEY_NEXT,
    MMISRVAUD_BT_EVENT_KEY_VOL_UP,
    MMISRVAUD_BT_EVENT_KEY_VOL_DOWN,
    MMISRVAUD_BT_EVENT_MAX
}MMISRVAUD_BT_EVENT_E;

typedef struct
{
    char tag_code_type;            //0:ascii;    1:unicode; 
    char year[5];				//year published	4bytes
    char res0;
    char res1;
    char tag[4];				// "TAG"		3 bytes
    char title[32];				//music title	30bytes
    char artist[32];			//music artist		30bytes
    char album[32];				//music album	30bytes
    char comment[32];			// comment		30bytes
    char genre[32];				//genre		30bytes
    char content[32];
    char track[32];
	
    char apic_mime[32];			//for ex. "image/jpeg"
    char apic_desc[32];			//description
    uint32 apic_offset;			//picture offset
    uint32 apic_length;			//picture length    
}MMISRVAUD_ID3_TAG_T;

typedef union
{
    MIDI_FILEINFO_T midi_ext_info;
    MP3_EXT_FORMATINFO_T mp3_ext_info;
    WAV_EXT_FORMATINFO_T wav_ext_info;
}MMISRVAUD_CONTENT_EXT_INFO_T;

typedef struct
{
    uint32 total_data_length;    /*!<File length in bytes*/
    uint32 total_time;       /*!<File length in seconds*/
    uint32 samplerate;      /*!<Audio data's samplerate*/
    uint32 channel_num;    /*!<Audio data's channel number, mono or stereo or some others.*/
    uint32 bitrate;          /*!<bitrate*/
    MMISRVAUD_ID3_TAG_T id3_tag;/*!<ID3 tag info */
    MMISRVAUD_CONTENT_EXT_INFO_T ext_info;
}MMISRVAUD_CONTENT_INFO_T;

typedef struct play_info_struct
{
    uint32	total_time;
    uint32 cur_time;
    uint32 total_data_length;
    uint32 cur_data_offset;
}MMISRVAUD_PLAY_INFO_T;

typedef struct
{
    uint32	total_time;
}MMISRVAUD_RECORD_INFO_T;

typedef struct
{
    uint32	total_size;
}MMISRVAUD_RECORD_SIZE_T;
/*
    The followed MACRO is the route definition, they can do OR operation, 
    such like MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_SPEAKER.
*/
#define MMISRVAUD_ROUTE_NONE     0x0000
/* AUTO means the audio should played through BT/EARPHONE/SPEAKER, and the priority from high to low is BT -> EARPHONE -> SPEAKER */
#define MMISRVAUD_ROUTE_AUTO     MMISRVAUD_ROUTE_NONE
/* BLUETOOTH */
#define MMISRVAUD_ROUTE_BLUETOOTH       (0x0001)
/* EARPHONE */
#define MMISRVAUD_ROUTE_EARPHONE         (0x0001 << 1)
/* EARPHONE and SPEARER*/
#define MMISRVAUD_ROUTE_EARFREE           (0x0001 << 2)
/* SPEAKER */
#define MMISRVAUD_ROUTE_SPEAKER           (0x0001 << 3)
/* HANDHOLD, used as call and keypad sound etc. */
#define MMISRVAUD_ROUTE_HANDHOLD        (0x0001 << 4)
/* Only used by TV */
#define MMISRVAUD_ROUTE_TVOUT               (0x0001 << 5)
/* Not supported now */
#define MMISRVAUD_ROUTE_BLUEPHONE       (0x0001 << 6)

#define MMISRVAUD_ROUTE_AUTO_NO_BLUETOOTH  (MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE)

typedef uint16 MMISRVAUD_ROUTE_T;

typedef enum
{  
    MMISRVAUD_EQ_REGULAR,      //³£¹æ
    MMISRVAUD_EQ_CLASSIC,      //¹Åµä
    MMISRVAUD_EQ_ODEUM,        //ÒôÀÖÌü
    MMISRVAUD_EQ_JAZZ,         //¾ôÊ¿
    MMISRVAUD_EQ_ROCK,         //Ò¡¹ö
    MMISRVAUD_EQ_SOFT_ROCK,    //ÈíÒ¡¹ö

    MMISRVAUD_EQ_MODE_MAX,
    MMISRVAUD_EQ_MODE_INVALID = 0xFF 
}MMISRVAUD_EQ_MODE_E;	

#define MMISRVAUD_VOLUME_LEVEL_NONE     0xFFFF
#define MMISRVAUD_VOLUME_LEVEL_0   0x0000
#if 0
#define MMISRVAUD_VOLUME_LEVEL_1   0x0001
#define MMISRVAUD_VOLUME_LEVEL_2   0x0002
#define MMISRVAUD_VOLUME_LEVEL_3   0x0003
#define MMISRVAUD_VOLUME_LEVEL_4   0x0004
#define MMISRVAUD_VOLUME_LEVEL_5   0x0005
#define MMISRVAUD_VOLUME_LEVEL_6   0x0006
#define MMISRVAUD_VOLUME_LEVEL_7   0x0007
#define MMISRVAUD_VOLUME_LEVEL_8   0x0008
#define MMISRVAUD_VOLUME_LEVEL_9   0x0009
#endif
#define MMISRVAUD_VOLUME_LEVEL_MAX   9
typedef uint32 MMISRVAUD_VOLUME_T;
typedef int32 MMISRVAUD_SPEED_T;

typedef enum
{
    MMISRVAUD_PLAY_RATE_SLOW_2 = -2,
    MMISRVAUD_PLAY_RATE_SLOW_1 = -1,
    MMISRVAUD_PLAY_RATE_NORMAL = 0,
    MMISRVAUD_PLAY_RATE_FAST_1,
    MMISRVAUD_PLAY_RATE_FAST_2,
    MMISRVAUD_PLAY_RATE_MAX
}MMISRVAUD_PLAY_RATE_E;

typedef enum
{
    MMISRVAUD_RECORD_FRAME_LEN_DEFAULT = 0,
    MMISRVAUD_RECORD_FRAME_LEN_32 = 0,
    MMISRVAUD_RECORD_FRAME_LEN_27,
    MMISRVAUD_RECORD_FRAME_LEN_21,
    MMISRVAUD_RECORD_FRAME_LEN_20,
    MMISRVAUD_RECORD_FRAME_LEN_18,
    MMISRVAUD_RECORD_FRAME_LEN_16,
    MMISRVAUD_RECORD_FRAME_LEN_14,
    MMISRVAUD_RECORD_FRAME_LEN_13,
    MMISRVAUD_RECORD_FRAME_LEN_MAX
}MMISRVAUD_RECORD_FRAME_LEN_E;

typedef enum
{
    MMISRVAUD_RECORD_SOURCE_NORMAL,
    MMISRVAUD_RECORD_SOURCE_CALL,
    MMISRVAUD_RECORD_SOURCE_FM,
    MMISRVAUD_RECORD_SOURCE_MAX
}MMISRVAUD_RECORD_SOURCE_E;

typedef enum
{
    MMISRVAUD_RECORD_FMT_ADPCM,
    MMISRVAUD_RECORD_FMT_AMR,
    MMISRVAUD_RECORD_FMT_MP3,
    MMISRVAUD_RECORD_FMT_PCM,
    MMISRVAUD_RECORD_FMT_MAX
}MMISRVAUD_RECORD_FMT_E;

typedef enum
{
    /* Don't adjust volume in playing*/
    MMISRVAUD_VOLUME_TYPE_NONE,
    /* Adujst volume by system automaticlly when in background playing */
    MMISRVAUD_VOLUME_TYPE_MED,
    /* Audio service notify App, and App adjust the volume itself */
    MMISRVAUD_VOLUME_TYPE_SELF,
    
    MMISRVAUD_VOLUME_TYPE_MAX
}MMISRVAUD_VOLUME_TYPE_E;


#define MMISRVAUD_TYPE_DEF\
    \
    TYPE_DEF(MMISRVAUD_TYPE_NONE),  \
            \
    TYPE_DEF(MMISRVAUD_TYPE_RING_FILE),   \
    TYPE_DEF(MMISRVAUD_TYPE_RING_BUF),    \
    \
    TYPE_DEF(MMISRVAUD_TYPE_STREAMING),   \
    \
    TYPE_DEF(MMISRVAUD_TYPE_RECORD_FILE),\
    TYPE_DEF(MMISRVAUD_TYPE_RECORD_BUF),\
    TYPE_DEF(MMISRVAUD_TYPE_IQ),\
    \
    TYPE_DEF(MMISRVAUD_TYPE_TONE_DTMF),\
    TYPE_DEF(MMISRVAUD_TYPE_TONE_GENERIC),\
    TYPE_DEF(MMISRVAUD_TYPE_TONE_CUSTOM),\
    \
    TYPE_DEF(MMISRVAUD_TYPE_VOICE),\
    TYPE_DEF(MMISRVAUD_TYPE_VOICE_LOOPBACK),\
    TYPE_DEF(MMISRVAUD_TYPE_FM),\
    \
    TYPE_DEF(MMISRVAUD_TYPE_ASP_FILE),\
    TYPE_DEF(MMISRVAUD_TYPE_ASP_BUF),\
    \
    TYPE_DEF(MMISRVAUD_TYPE_VIBERATE),\
    \
    TYPE_DEF(MMISRVAUD_TYPE_VIDEO),\
    TYPE_DEF(MMISRVAUD_TYPE_VIDEO_WP),\
    TYPE_DEF(MMISRVAUD_TYPE_ATV),\
    TYPE_DEF(MMISRVAUD_TYPE_MV),\
    \
    TYPE_DEF(MMISRVAUD_TYPE_VIRTUAL),\
    \
#ifdef UAL_BT_HFU_SUPPORT
    TYPE_DEF(MMISRVAUD_TYPE_VOICE_BTCALL),\    
    \
    TYPE_DEF(MMISRVAUD_TYPE_BT_MUSIC),\    
    \
#endif
    TYPE_DEF(MMISRVAUD_TYPE_MAX)\
    
#define TYPE_DEF(x)  x

typedef enum
{
    MMISRVAUD_TYPE_DEF
} MMISRVAUD_TYPE_E;

#undef TYPE_DEF

typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_RING_FMT_E fmt;
    wchar *name;
    uint32 name_len;
}MMISRVAUD_RING_FILE_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_RING_FMT_E fmt;
    uint8 *data; 
    uint32 data_len;
}MMISRVAUD_RING_BUF_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_RING_FMT_E fmt;
    uint8 *data; 
    uint32 data_len;
    void *cb;
}MMISRVAUD_STREAMING_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_RECORD_FMT_E fmt;
    MMISRVAUD_RECORD_SOURCE_E source;
    MMISRVAUD_RECORD_FRAME_LEN_E frame_len;    
    uint32 file_handle;
    wchar *name; 
    uint32 name_len;
}MMISRVAUD_RECORD_FILE_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_RECORD_FMT_E fmt;
    MMISRVAUD_RECORD_SOURCE_E source;
    MMISRVAUD_RECORD_FRAME_LEN_E frame_len;    
    uint8 *data; 
    uint32 data_len;
}MMISRVAUD_RECORD_BUF_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_DTMF_TONE_E id;
    uint32 duation;
}MMISRVAUD_TONE_DTMF_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_GENERIC_TONE_E id;
    uint32 duation;
}MMISRVAUD_TONE_GENERIC_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    uint32 freq;
    uint32 duation;
}MMISRVAUD_TONE_CUSTOM_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
}MMISRVAUD_VOICE_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
}MMISRVAUD_FM_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    wchar *name_ptr;
    void *param; /* which is RECORD_PARAM_T struct's data */ 
}MMISRVAUD_IQ_T;


typedef void (* MMISRVAUD_VIBRATE_CB)(void);
typedef struct
{
    MMISRVAUD_TYPE_E type;
    MMISRVAUD_VIBRATE_CB cb;
}MMISRVAUD_VIBRATE_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    uint32 data;
}MMISRVAUD_VIDEO_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    uint32 service_id;
    uint32 region_id;
    void * param_ptr;
    uint32 param_size;
    uint32 param[20]; /* To save DRECORDER_PREVIEW_PARAM_T struct's data, App ignore this var */
}MMISRVAUD_ATV_T;

typedef struct
{
    MMISRVAUD_TYPE_E type;
    uint32 frequency_num;
    uint32 service_id;
    uint32 init_data;
    void *cb;
}MMISRVAUD_MV_T;

typedef union
{
    MMISRVAUD_TYPE_E                type;
    MMISRVAUD_RING_FILE_T         ring_file;
    MMISRVAUD_STREAMING_T       streaming;
    MMISRVAUD_RING_BUF_T          ring_buf;
    MMISRVAUD_RECORD_FILE_T     record_file;
    MMISRVAUD_RECORD_BUF_T      record_buf;
    MMISRVAUD_TONE_DTMF_T       tone_dtmf;
    MMISRVAUD_TONE_GENERIC_T   tone_generic;
    MMISRVAUD_TONE_CUSTOM_T   tone_custom;
    MMISRVAUD_VOICE_T               voice;
    MMISRVAUD_FM_T                    fm;
    MMISRVAUD_IQ_T                    iq_file;
    MMISRVAUD_VIBRATE_T            vibrate;
    MMISRVAUD_VIDEO_T               video;
    MMISRVAUD_ATV_T                  atv;
    MMISRVAUD_MV_T                   mv;
}MMISRVAUD_TYPE_U;

typedef struct
{
    MMISRVAUD_TYPE_U info;
    MMISRVAUD_TYPE_U replace_info;
    
    //MMISRVAUD_ROUTE_T route;
    MMISRVAUD_ROUTE_T default_route;
    MMISRVAUD_ROUTE_T all_support_route;
    
    MMISRVAUD_VOLUME_T volume;
    MMISRVAUD_VOLUME_TYPE_E volume_type;
    MMISRVAUD_EQ_MODE_E eq_mode;
    MMISRVAUD_SPEED_T speed;
    MMISRVAUD_PLAY_RATE_E rate;
    
    uint16 play_times;
    uint16 duation;
    uint16 interval;
    BOOLEAN is_mixing_enable;
    BOOLEAN is_standing;
}MMISRVAUD_TYPE_T;


typedef struct
{
    MMISRVAUD_ROUTE_T route;
    MMISRVAUD_VOLUME_T volume;
    MMISRVAUD_EQ_MODE_E eq_mode;
    MMISRVAUD_SPEED_T speed;    
}MMISRVAUD_AUD_CFG_T;

typedef struct
{
    uint16 *spec_buf_ptr;
    uint16 spec_num;
    uint16 max_spec_value;
}MMISRVAUD_SPECTRUM_INFO_T;

typedef enum
{
    MMISRVAUD_REPORT_PLAY = 0,
    MMISRVAUD_REPORT_END,
    MMISRVAUD_REPORT_STOP,
    MMISRVAUD_REPORT_RESUME,
    MMISRVAUD_REPORT_PAUSE,
    MMISRVAUD_REPORT_TIMER_END,
    MMISRVAUD_REPORT_INTERVAL_TIMER_END,
    MMISRVAUD_REPORT_BT_EVENT,
    MMISRVAUD_REPORT_VOLUME_ADJUST,
    MMISRVAUD_REPORT_EARPHONE_EVENT,
    
    MMISRVAUD_REPORT_MAX
}MMISRVAUD_REPORT_TYPE_E;

typedef struct
{
    MMISRVAUD_REPORT_TYPE_E report;
    uint32 data1;
    uint32 data2;
}MMISRVAUD_REPORT_T;

/*****************************************************************************/
//  Description : Play an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RET_E MMISRVAUD_Play(MMISRV_HANDLE_T audio_handle, uint32 offset);

/*****************************************************************************/
//  Description : Play an audio by handle async.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RET_E MMISRVAUD_PlayAsy(MMISRV_HANDLE_T audio_handle, uint32 offset);

/*****************************************************************************/
//  Description : Stop an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Stop(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Pause an audio  by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Pause(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Resume an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Resume(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Pause an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used for Video, to other audio type, it's useless.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_PauseSlightly(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Resume an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used for Video, to other audio type, it's useless.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_ResumeSlightly(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Seek an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Seek(MMISRV_HANDLE_T audio_handle, uint32 offset);

/*****************************************************************************/
//  Description : Seek an audio by time.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SeekByTime(MMISRV_HANDLE_T audio_handle, uint32 time);

/*****************************************************************************/
//  Description : Set new audio  info by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAudioInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_TYPE_U *audio_info);

/*****************************************************************************/
//  Description : Get an audio's info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetContentInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_CONTENT_INFO_T	*content_info_ptr);

/*****************************************************************************/
//  Description : Get an audio playing info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetPlayingInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_PLAY_INFO_T *playing_info_ptr);

/*****************************************************************************/
//  Description : Get recording info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetRecordInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_RECORD_INFO_T	*record_info_ptr);

/*****************************************************************************/
//  Description : Get recording size.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetRecordSize(MMISRV_HANDLE_T audio_handle, MMISRVAUD_RECORD_SIZE_T	*record_info_ptr); 

/*****************************************************************************/
//  Description : Set play speed.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetPlaySpeed(MMISRV_HANDLE_T audio_handle, uint32 speed);

#ifdef MP3_EQ_SUPPORT
/*****************************************************************************/
//  Description : Set EQ mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetEQMode(MMISRV_HANDLE_T audio_handle, MMISRVAUD_EQ_MODE_E eq_mode);
#endif

/*****************************************************************************/
//  Description : Get audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_VOLUME_T MMISRVAUD_GetVolume(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Set audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVolume(MMISRV_HANDLE_T audio_handle, MMISRVAUD_VOLUME_T volume);

/*****************************************************************************/
//  Description : Get audio current route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T MMISRVAUD_GetRoute(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Set audio play route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetRoute(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T audio_route);

/*****************************************************************************/
//  Description : Set audio all support route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAllSupportRoute(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T all_support_route);
/*****************************************************************************/
//  Description : Set audio manual route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetManualRoute(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T manual_route);

/*****************************************************************************/
//  Description : Set audio play times.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetPlayTimes(MMISRV_HANDLE_T audio_handle, uint32 times);

#if 0
/*****************************************************************************/
//  Description : To make a judgement if there is any audio is in playing.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsActive(void);
#endif

/*****************************************************************************/
//  Description : Mute the appointed route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SetRouteMute(MMISRVAUD_ROUTE_T route, BOOLEAN mute);

/*****************************************************************************/
//  Description : To make a judgement if the audio service has setted the route manully.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsSetRouteManully(MMISRV_HANDLE_T handle);

#if 0
/*****************************************************************************/
//  Description : If audio played end.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsAudioEnd(MMISRV_HANDLE_T handle, BOOLEAN *result);
#endif

/*****************************************************************************/
//  Description : Get spectrum info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetSpectrum(MMISRV_HANDLE_T handle, MMISRVAUD_SPECTRUM_INFO_T *info);

/*****************************************************************************/
//  Description : Open spectrum.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_OpenSpectrum(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : close spectrum.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_CloseSpectrum(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : Enable/disable voice loopback.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVoiceLB(MMISRV_HANDLE_T handle, BOOLEAN is_enable);

/*****************************************************************************/
//  Description : Enable/disable voice loopback.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_GetRecordDB(MMISRV_HANDLE_T handle);

/*****************************************************************************
 The following function is to compatible the old API.
 Whe all the app take the new API place of the old API, there function would be deleted.
*****************************************************************************/

/*****************************************************************************/
//  Description : Get current audio configure.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetAudCfg(MMISRVAUD_AUD_CFG_T *aud_cfg);

/*****************************************************************************/
//  Description : Set play speed.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetPlaySpeedNoHandle(int32 speed);

#ifdef MP3_EQ_SUPPORT
/*****************************************************************************/
//  Description : Set EQ mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetEQModeNoHandle(MMISRVAUD_EQ_MODE_E eq_mode);
#endif

/*****************************************************************************/
//  Description : Get audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint32	MMISRVAUD_GetVolumeNoHandle(void);

/*****************************************************************************/
//  Description : Set audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVolumeNoHandle(uint32 volume);

/*****************************************************************************/
//  Description : Get audio current route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T MMISRVAUD_GetRouteNoHandle(void);

/*****************************************************************************/
//  Description : Set audio play route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetRouteNohandle(MMISRVAUD_ROUTE_T audio_route);

/*****************************************************************************/
//  Description : Set mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetMuteNohandle(BOOLEAN is_mute);

/*****************************************************************************/
//  Description : Get mute statues.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetMuteNohandle(BOOLEAN *result);

/*****************************************************************************/
//  Description : Set uplink mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetUpLinkMute(BOOLEAN is_mute);

/*****************************************************************************/
//  Description : Regist audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAUD_RegistService(void);

/*****************************************************************************/
//  Description : Get an audio's info from buff.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetBuffContentInfo(uint8 *buf, uint32 buf_len, MMISRVAUD_RING_FMT_E fmt, MMISRVAUD_CONTENT_INFO_T *info_ptr);

/*****************************************************************************/
//  Description : Get an audio's info from buff.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetFileContentInfo(wchar *file_name, uint32 name_len, MMISRVAUD_CONTENT_INFO_T *info_ptr);

#ifdef AUDIO_RATE_SUPPORT
PUBLIC BOOLEAN MMISRVAUD_SetPlayRate(MMISRVAUD_PLAY_RATE_E rate);
#endif

/*****************************************************************************/
//  Description : audio init
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_Init(void);

/*****************************************************************************/
//  Description : play tone 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PlayTone(
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                );

/*****************************************************************************/
//  Description : play tone 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PlayToneEx(
                uint16 pri,
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                );

/*****************************************************************************/
//  Description : tone stop 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_StopTone(MMISRVAUD_TYPE_E tone_type);

#ifndef VIRTUAL_VIBRATE_FUNC
/*****************************************************************************/
//  Description : Start vibrate.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_StartVibrate(uint16 pri, uint16 duation, uint16 interval, uint16 times, MMISRVAUD_VIBRATE_CB callback);

/*****************************************************************************/
//  Description : Start vibrate.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC  void MMISRVAUD_StopVibrate(MMISRV_HANDLE_T handle);
#endif

/*****************************************************************************/
//  Description : headset plug in/out, BT earphone is active or switch off.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_ExtDevChg(void);

/*****************************************************************************/
//  Description : Start voice loop.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_EnableVoiceLB(BOOLEAN is_enable, uint32 volume);

/*****************************************************************************/
//  Description : To make a judgement if the audio service is playing.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsAudPlaying(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Handle audio callback message.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_HandleCallBackMsg(void *msg_ptr);

/*****************************************************************************/
//  Description : Request a virtual audio handel by name. Generally, this function is used to occupy the
//                    audio resource to prevent or stop other audio to play.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_ReqVirtualHandle(char *client_name, MMISRVAUD_PRI_T pri);

/*****************************************************************************/
//  Description : Free virtual handle by name.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_FreeVirtualHandle(char *client_name);

/*****************************************************************************/
//  Description : Get handle by volume type, only used to adjust the volume generally.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_GetHandleByVolumeType(MMISRVAUD_VOLUME_TYPE_E volume_type);

/*****************************************************************************/
//  Description : Get handle by driver handle,.
//  Global resource dependence : 
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_GetHandleByDrvHandle(uint32 drv_handle);

/*****************************************************************************/
//  Description : Get handle by volume type, only used to adjust the volume generally.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAUD_NotifyVolumeSelfApp(BOOLEAN is_increase);

/*****************************************************************************/
//  Description : Set ATV's service id.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAtvServiceId(MMISRV_HANDLE_T audio_handle, uint32 service_id);

/*****************************************************************************/
//  Description : Start record ATV.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_StartRecordAtv(MMISRV_HANDLE_T audio_handle, void *param);

/*****************************************************************************/
//  Description : headset plug in/out, BT earphone is active or switch off.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_E event);

/*****************************************************************************/
//  Description : Stop record ATV.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_StopRecordAtv(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : play tone  with route.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PlayToneWithRoute(
                MMISRVAUD_ROUTE_T route,
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                );

/*****************************************************************************/
//  Description : Close audio handle, but not free. It's means we can't play/stop/resume/pause it.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Should stop it at first.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Close(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : Set volume by volume type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Each audio handle will be setted whitch is the same type.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVolumeByType(MMISRVAUD_VOLUME_TYPE_E type, MMISRVAUD_VOLUME_T volume);

/*****************************************************************************/
//  Description : printf audio service info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used to print the mmi audio service info when MS asserted.
/*****************************************************************************/
PUBLIC void MMISRVAUD_PrintInfo(void);
                
/*******************************************************************/
//  Interface:		MMISRVAUD_HandleEarphoneEvent
//  Description : 	MMISRVAUD_HandleEarphoneEvent
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMISRVAUD_HandleEarphoneEvent(MMISRVAUD_EARPHONE_EVENT_ID event, uint32 keycode);

#endif


