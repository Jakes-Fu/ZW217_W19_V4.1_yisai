/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _RPC_AUD_H_
#define _RPC_AUD_H_

//#include <stdint.h>
#ifdef UINT8
#undef UINT8
#endif
#define UINT8 uint8

#ifdef UINT16
#undef UINT16
#endif
#define UINT16 uint16

#ifdef UINT32
#undef UINT32
#endif
#define UINT32 uint32

#ifdef INT8
#undef INT8
#endif
#define INT8 int8

#ifdef INT16
#undef INT16
#endif
#define INT16 int16

#ifdef INT32
#undef INT32
#endif
#define INT32 int32

#ifdef BOOL
#undef BOOL
#endif
#define BOOL int8

#ifdef VOID
#undef VOID
#endif
#define VOID void

#if 1 // TODO: LJF TEST
typedef enum
{
    /// No error occured
    HAL_ERR_NO,

    /// A resource reset is required
    HAL_ERR_RESOURCE_RESET,

    /// An attempt to access a busy resource failed
    HAL_ERR_RESOURCE_BUSY,

    /// Timeout while trying to access the resource
    HAL_ERR_RESOURCE_TIMEOUT,

    /// An attempt to access a resource that is not enabled
    HAL_ERR_RESOURCE_NOT_ENABLED,

    /// Invalid parameter
    HAL_ERR_BAD_PARAMETER,

    /// Uart RX FIFO overflowed
    HAL_ERR_UART_RX_OVERFLOW,

    /// Uart TX FIFO overflowed
    HAL_ERR_UART_TX_OVERFLOW,
    HAL_ERR_UART_PARITY,
    HAL_ERR_UART_FRAMING,
    HAL_ERR_UART_BREAK_INT,
    HAL_ERR_TIM_RTC_NOT_VALID,
    HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED,
    HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED,

    /// Communication failure
    HAL_ERR_COMMUNICATION_FAILED,

    HAL_ERR_QTY
} HAL_ERR_T;
typedef enum
{
    /// No error occured
    AUD_ERR_NO,

    /// An attempt to access a busy resource failed
    /// (Resource use not possible at the time)
    AUD_ERR_RESOURCE_BUSY,

    /// Attempt to open a resource already opened,
    /// (no need to open again to use).
    AUD_ERR_ALREADY_OPENED,

    /// Timeout while trying to access the resource
    AUD_ERR_RESOURCE_TIMEOUT,

    /// Invalid parameter
    AUD_ERR_BAD_PARAMETER,

    /// The specified interface does not exist
    AUD_ERR_NO_ITF,

    /// Unspecified error
    AUD_ERR_UNKNOWN,

    AUD_ERR_QTY
} AUD_ERR_T;

typedef enum
{
    /// No test mode.
    AUD_TEST_NO,

    /// For audio test loop; analog to DAI loop + DAI to analog loop.
    AUD_TEST_LOOP_ACOUSTIC,

    /// For audio test loop; radio loop on DAI interface
    AUD_TEST_LOOP_RF_DAI,

    /// For audio test loop; radio loop on analog interface
    AUD_TEST_LOOP_RF_ANALOG,

    /// For board check: mic input is fedback to the speaker output.
    AUD_TEST_SIDE_TEST,

    /// For board check; audio loop in VOC
    AUD_TEST_LOOP_VOC,

    /// For board check; regular mic input and earpiece output
    AUD_TEST_RECVMIC_IN_EARPIECE_OUT,

    AUD_TEST_MODE_QTY
} AUD_TEST_T;

/*xin.xu1 BBAT test*/
typedef enum
{
    AUD_TEST_VOICE_MODE_NB = 0,
    AUD_TEST_VOICE_MODE_WB,
    AUD_TEST_VOICE_MODE_PCM,
    AUD_TEST_VOICE_MODE_ERROR
}AUD_TEST_VOICE_MODE_T;

/* Speaker volume */
typedef enum
{
    DM_AUDIO_SPK_MUTE = 0,
    DM_AUDIO_SPK_VOL_1,
    DM_AUDIO_SPK_VOL_2,
    DM_AUDIO_SPK_VOL_3,
    DM_AUDIO_SPK_VOL_4,
    DM_AUDIO_SPK_VOL_5,
    DM_AUDIO_SPK_VOL_6,
    DM_AUDIO_SPK_VOL_7,
    DM_AUDIO_SPK_VOL_8,
    DM_AUDIO_SPK_VOL_9,
    DM_AUDIO_SPK_VOL_10,
    DM_AUDIO_SPK_VOL_11,
    DM_AUDIO_SPK_VOL_12,
    DM_AUDIO_SPK_VOL_13,
    DM_AUDIO_SPK_VOL_14,
    DM_AUDIO_SPK_VOL_15,
    DM_AUDIO_SPK_GAIN_QTY,
    DM_AUDIO_SPK_ALIGNED_4BYTES = 0xF0000000
} DM_SpeakerGain;
typedef enum
{
    SND_ITF_RECEIVER                            = 0x00000000,
    SND_ITF_EAR_PIECE                           = 0x00000001,
    SND_ITF_LOUD_SPEAKER                        = 0x00000002,
    SND_ITF_BLUETOOTH                           = 0x00000003,
    SND_ITF_FM                                  = 0x00000004,
    SND_ITF_TV                                  = 0x00000005,
/// Number (max) of available interface to the SND driver
    SND_ITF_QTY                                 = 0x00000006,
    SND_ITF_CLOSE                               = 0x000000AA,
    SND_ITF_NONE                                = 0x000000FF
} SND_ITF_T;
typedef enum
{
    SND_SPK_MUTE                                = 0x00,
    SND_SPK_VOL_1                               = 0x01,
    SND_SPK_VOL_2                               = 0x02,
    SND_SPK_VOL_3                               = 0x03,
    SND_SPK_VOL_4                               = 0x04,
    SND_SPK_VOL_5                               = 0x05,
    SND_SPK_VOL_6                               = 0x06,
    SND_SPK_VOL_7                               = 0x07,
    SND_SPK_VOL_8                               = 0x08,
    SND_SPK_VOL_9                               = 0x09,
    SND_SPK_VOL_10                               = 0x0A,
    SND_SPK_VOL_11                               = 0x0B,
    SND_SPK_VOL_12                               = 0x0C,
    SND_SPK_VOL_13                               = 0x0D,
    SND_SPK_VOL_14                               = 0x0E,
    SND_SPK_VOL_15                               = 0x0F,
    SND_SPK_VOL_QTY
} SND_SPK_LEVEL_T;
typedef enum
{
    SND_MIC_MUTE                                = 0x00,
    SND_MIC_ENABLE                              = 0x01,
    SND_MIC_VOL_CALL                            = 0x01,
    SND_MIC_VOL_RECORD                          = 0x02,
    SND_MIC_VOL_FM                                  = 0x03,
    SND_MIC_VOL_QTY                             = 0x04
} SND_MIC_LEVEL_T;
typedef enum
{
    SND_SIDE_MUTE                               = 0x00,
    SND_SIDE_VOL_1                              = 0x01,
    SND_SIDE_VOL_2                              = 0x02,
    SND_SIDE_VOL_3                              = 0x03,
    SND_SIDE_VOL_4                              = 0x04,
    SND_SIDE_VOL_5                              = 0x05,
    SND_SIDE_VOL_6                              = 0x06,
    SND_SIDE_VOL_7                              = 0x07,
    SND_SIDE_VOL_8                              = 0x08,
    SND_SIDE_VOL_9                              = 0x09,
    SND_SIDE_VOL_10                              = 0x0a,
    SND_SIDE_VOL_11                              = 0x0b,
    SND_SIDE_VOL_12                              = 0x0c,
    SND_SIDE_VOL_13                              = 0x0d,
    SND_SIDE_VOL_14                              = 0x0e,
    SND_SIDE_VOL_15                              = 0x0f,
    SND_SIDE_VOL_TEST,
    SND_SIDE_VOL_QTY
} SND_SIDE_LEVEL_T;
typedef enum
{
/// No attenuation
    SND_TONE_0DB                                = 0x00,
/// -3dB
    SND_TONE_M3DB                               = 0x01,
/// -9db
    SND_TONE_M9DB                               = 0x02,
/// -15dB
    SND_TONE_M15DB                              = 0x03,
    SND_TONE_VOL_QTY                            = 0x04
} SND_TONE_ATTENUATION_T;

typedef enum
{
    SND_BT_WORK_MODE_NB                   = 0x00000000,
    SND_BT_WORK_MODE_WB                     = 0x00000001,
    SND_BT_WORK_MODE_NO                     = 0x00000002, 
    SND_BT_WORK_MODE_QTY                         = 0x00000003,
} SND_BT_WORK_MODE_T;

typedef enum
{
    SND_APP_MODE_VOICE                   = 0x00,
    SND_APP_MODE_MUSIC                     = 0x01,
    SND_APP_MODE_RECORD                  = 0x02,
    SND_APP_MODE_FMPLAY                  = 0x03,
    SND_APP_MODE_FMRECORD             = 0x04,
    SND_APP_MODE_LOOPBACK               = 0x05,
    SND_APP_MODE_TONEPLAY                = 0x06,
    SND_APP_MODE_QTY                         = 0x07,
} SND_APP_MODE_T;
typedef struct
{
    /// Speaker level,
    SND_SPK_LEVEL_T spkLevel;

    /// Microphone level: muted or enabled
    SND_MIC_LEVEL_T micLevel;

    /// Sidetone
    SND_SIDE_LEVEL_T sideLevel;

    SND_TONE_ATTENUATION_T toneLevel;
    SND_APP_MODE_T appMode;
} AUD_LEVEL_T;
typedef enum
{
    HAL_AIF_FREQ_8000HZ  =  8000,
    HAL_AIF_FREQ_11025HZ = 11025,
    HAL_AIF_FREQ_12000HZ = 12000,
    HAL_AIF_FREQ_16000HZ = 16000,
    HAL_AIF_FREQ_22050HZ = 22050,
    HAL_AIF_FREQ_24000HZ = 24000,
    HAL_AIF_FREQ_32000HZ = 32000,
    HAL_AIF_FREQ_44100HZ = 44100,
    HAL_AIF_FREQ_48000HZ = 48000,
    HAL_AIF_FREQ_96000HZ = 96000,
    HAL_AIF_FREQ_128000HZ = 128000,
} HAL_AIF_FREQ_T;
typedef enum
{
    HAL_AIF_MONO = 1,
    HAL_AIF_STEREO = 2,
} HAL_AIF_CH_NB_T;
typedef void (*HAL_AIF_HANDLER_T)(void);
typedef struct
{
    uint32* startAddress;
    /// Stream length in bytes.
    /// The length of a stream must be a multiple of 16 bytes.
    /// The maximum size is 32 KB.
    uint16 length;
    HAL_AIF_FREQ_T sampleRate;
    HAL_AIF_CH_NB_T channelNb;
    /// True if this is a voice stream, coded on 13 bits, mono, at 8kHz.
    /// Voice quality streams are the only that can be output through
    /// the receiver interface.
    int32 voiceQuality;
    /// True if the play stream is started along with the record stream.
    /// In this case, the play stream will be configured at first,
    /// but it is not started until the record stream is ready to start.
    int32 playSyncWithRecord;
    /// Handler called when the middle of the buffer is reached.
    /// If this field is left blank (NULL), no interrupt will be
    /// generated.
    HAL_AIF_HANDLER_T halfHandler;
    /// Handler called when the end of the buffer is reached.
    /// If this field is left blank (NULL), no interrupt will be
    /// generated.
    HAL_AIF_HANDLER_T endHandler;
} HAL_AIF_STREAM_T;
#endif
typedef struct 
{
    UINT32 uiLength; // size of var_data        ......        
    char acPcmData[1];    
} BBAT_PCM_STREAM_T;

    
typedef enum
{
    AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL,
    AUD_INPUT_MIC_CIRCUITY_TYPE_SINGLEEND,
} AUD_INPUT_MIC_CIRCUITY_TYPE_T;


typedef enum
{
    AUD_INPUT_TYPE_MAINMIC,
    AUD_INPUT_TYPE_AUXMIC,
    AUD_INPUT_TYPE_DUALMIC,
    AUD_INPUT_TYPE_HPMIC_USE_L,
    AUD_INPUT_TYPE_HPMIC_USE_R,
} AUD_INPUT_TYPE_T;

typedef enum
{
    AUD_INPUT_PATH1,
    AUD_INPUT_PATH2,
} AUD_INPUT_PATH_T;

typedef enum
{
    AUD_LINE_PATH1,
    AUD_LINE_PATH2,
    AUD_LINE_PATH3,
} AUD_LINE_PATH_T;

typedef enum
{
    AUD_SPKPA_TYPE_CLASSAB,
    AUD_INPUT_TYPE_CLASSD,
    AUD_INPUT_TYPE_CLASSK,
} AUD_SPKPA_TYPE_T;

typedef struct
{
    /// Type of speaker
    AUD_INPUT_TYPE_T inputType;
    AUD_INPUT_MIC_CIRCUITY_TYPE_T    inputCircuityType;
    AUD_INPUT_PATH_T   inputPath;
    AUD_LINE_PATH_T     linePath;
    /// Type of mic
    AUD_SPKPA_TYPE_T    spkpaType;
} AUD_DEVICE_CFG_EXT_T;

typedef enum
{
    AUD_ITF_RECEIVER = 0,
    AUD_ITF_EAR_PIECE,
    AUD_ITF_LOUD_SPEAKER,
    AUD_ITF_BLUETOOTH,
    AUD_ITF_FM,
    AUD_ITF_TV,

    /// Number (max) of available interface to the AUD driver
    AUD_ITF_QTY,
    AUD_ITF_NONE = 0xFF

} AUD_ITF_T;

typedef enum
{
    AUD_SPK_MUTE = 0,
    AUD_SPK_VOL_1,
    AUD_SPK_VOL_2,
    AUD_SPK_VOL_3,
    AUD_SPK_VOL_4,
    AUD_SPK_VOL_5,
    AUD_SPK_VOL_6,
    AUD_SPK_VOL_7,
    AUD_SPK_VOL_8,
    AUD_SPK_VOL_9,
    AUD_SPK_VOL_10,
    AUD_SPK_VOL_11,
    AUD_SPK_VOL_12,
    AUD_SPK_VOL_13,
    AUD_SPK_VOL_14,
    AUD_SPK_VOL_15,


    AUD_SPK_VOL_QTY
} AUD_SPK_LEVEL_T;

typedef enum
{
    AUD_MIC_MUTE = 0,
    AUD_MIC_ENABLE,
    AUD_MIC_VOL_CALL = AUD_MIC_ENABLE,
    AUD_MIC_VOL_RECORD,

    AUD_MIC_VOL_QTY
} AUD_MIC_LEVEL_T;

typedef enum
{
    AUD_SIDE_MUTE = 0,
    AUD_SIDE_VOL_1,
    AUD_SIDE_VOL_2,
    AUD_SIDE_VOL_3,
    AUD_SIDE_VOL_4,
    AUD_SIDE_VOL_5,
    AUD_SIDE_VOL_6,
    AUD_SIDE_VOL_7,
    AUD_SIDE_VOL_8,
    AUD_SIDE_VOL_9,
    AUD_SIDE_VOL_10,
    AUD_SIDE_VOL_11,
    AUD_SIDE_VOL_12,
    AUD_SIDE_VOL_13,
    AUD_SIDE_VOL_14,
    AUD_SIDE_VOL_15,

    AUD_SIDE_VOL_TEST,

    AUD_SIDE_VOL_QTY
} AUD_SIDE_LEVEL_T;

typedef enum
{
    /// No attenuation
    AUD_TONE_0dB,
    /// -3dB
    AUD_TONE_m3dB,
    /// -9db
    AUD_TONE_m9dB,
    /// -15dB
    AUD_TONE_m15dB,

    AUD_TONE_VOL_QTY
} AUD_TONE_ATTENUATION_T;

#define DM_TONE_DTMF_0                           0
#define DM_TONE_DTMF_1                           1
#define DM_TONE_DTMF_2                           2
#define DM_TONE_DTMF_3                           3
#define DM_TONE_DTMF_4                           4
#define DM_TONE_DTMF_5                           5
#define DM_TONE_DTMF_6                           6
#define DM_TONE_DTMF_7                           7
#define DM_TONE_DTMF_8                           8
#define DM_TONE_DTMF_9                           9
#define DM_TONE_DTMF_A                           10
#define DM_TONE_DTMF_B                           11
#define DM_TONE_DTMF_C                           12
#define DM_TONE_DTMF_D                           13
#define DM_TONE_DTMF_S                           14
#define DM_TONE_DTMF_P                           15
#define DM_TONE_DIAL                             16
#define DM_TONE_SUBSCRIBER_BUSY                  17
#define DM_TONE_CONGESTION                       18
#define DM_TONE_RADIO_PATHACKNOWLEDGEMENT        19
#define DM_TONE_CALL_DROPPED                     20
#define DM_TONE_SPECIAL_INFORMATION              21
#define DM_TONE_CALL_WAITING                     22
#define DM_TONE_RINGING                          23


// =============================================================================
// VOIS_AUDIO_CFG_T
// -----------------------------------------------------------------------------
/// Configuration structure.
/// 
/// A configuration structure allows to record a stream with the proper configuration
/// set as far as the audio interface and the decoding are concerned.
// =============================================================================
typedef struct
{
    /// Speaker level.
    AUD_SPK_LEVEL_T spkLevel;

    /// Mic level
    AUD_MIC_LEVEL_T micLevel;

    /// Side tone
    AUD_SIDE_LEVEL_T sideLevel;

    /// Tone level
    AUD_TONE_ATTENUATION_T toneLevel;
    
    /// encoder gain
    int16 encMute;

    /// decoder gain
    int16 decMute;
       
} VOIS_AUDIO_CFG_T;

typedef enum
{
    /// No error occured
    VOIS_ERR_NO,

    /// An attempt to access a busy resource failed
    /// (Resource use not possible)
    VOIS_ERR_RESOURCE_BUSY,

    /// Attempt to open a resource already opened,
    /// (no need to open again to use).
    VOIS_ERR_ALREADY_OPENED,
    
    /// Timeout while trying to access the resource
    VOIS_ERR_RESOURCE_TIMEOUT,
    
    /// Invalid parameter
    VOIS_ERR_BAD_PARAMETER,

    /// The specified interface does not exist
    VOIS_ERR_NO_ITF,

    /// What ?
    VOIS_ERR_UNKNOWN,


    VOIS_ERR_QTY
} VOIS_ERR_T;

#define AUDIO_26M_USED_BY_MUSIC             (BIT_0)
#define AUDIO_26M_USED_BY_LAYER1            (BIT_1)
#define AUDIO_26M_USED_BY_RECORD            (BIT_2)
#define AUDIO_26M_USED_BY_FM                    (BIT_3)
#define AUDIO_26M_USED_BY_TONE                  (BIT_4)
#define AUDIO_26M_USED_BY_LocalTest             (BIT_5)
#if 0
#define AUDIO_DEV_MUSIC                     (BIT_0)
#define AUDIO_DEV_LAYER1                    (BIT_1)
#define AUDIO_DEV_RECORD                    (BIT_2)
#define AUDIO_DEV_FM                            (BIT_3)
#define AUDIO_DEV_TONE                          (BIT_4)
#define AUDIO_DEV_MP4RECORD                 (BIT_5)
#else
typedef enum
{
    AUDIO_DEV_MUSIC,
    AUDIO_DEV_LAYER1,
    AUDIO_DEV_RECORD,
    AUDIO_DEV_FM,
    AUDIO_DEV_TONE,
    AUDIO_DEV_MP4RECORD,    
    AUDIO_DEV_DUALCHN_PLAYRECORD, // support play and record at the same time
    AUDIO_DEV_RESERVE2,   
    AUDIO_DEV_MAX    
}AUDIO_DEV_TYPE_E;
#endif
#define AUDIO_CP_STOPPED                                0
#define AUDIO_CP_MUSIC_PLAYING                  (BIT_0)
#define AUDIO_CP_VOICE_PLAYING                  (BIT_1)
#define AUDIO_CP_RECORD_PLAYING                 (BIT_2)
#define AUDIO_CP_FM_PLAYING                         (BIT_3)
#define AUDIO_CP_TONE_PLAYING                   (BIT_4)
#define AUDIO_CP_MP4RECORD_PLAYING          (BIT_5)
//#define AUDIO_CP_SHARKE_PLAYING          (BIT_7)
#define AUDIO_CP_STOPPING                           (BIT_8)//has startted to stop but not stopped.
typedef enum
{
    CODEC_STOPPED,
    CODEC_PLAYING,
    CODEC_DONE,
    AUDIO_CODEC_MAX    
}AUDIO_CODEC_STATUS_E;
typedef enum
{
    AUD_NULL,
    CODEC_STREAM_START_BEGIN,
    CODEC_STREAM_START_DONE,
    CODEC_STREAM_STOP_BEGIN,
    CODEC_STREAM_STOP_DONE,  
    CODEC_RECORD_START_BEGIN,
    CODEC_RECORD_START_DONE,
    CODEC_RECORD_STOP_BEGIN,
    CODEC_RECORD_STOP_DONE,
    CODEC_VOIS_START_BEGIN,
    CODEC_VOIS_START_DONE,
    CODEC_VOIS_STOP_BEGIN,
    CODEC_VOIS_STOP_DONE,
    ZSP_START_BEGIN, 
    ZSP_START_DONE,  
    ZSP_STOP_BEGIN,
    ZSP_STOP_DONE,
    AUD_CODEC_SETUP_BEGIN,
    AUD_CODEC_SETUP_DONE,
    VOIS_SETUP_BEGIN,
    VOIS_SETUP_DONE,
    AUD_TESTMODE_SETUP_BEGIN,
    AUD_TESTMODE_SETUP_DONE,
    AUD_CODEC_STATUS_MAX  = 0xFF000000   
}AUD_CP_STATUS_E;

typedef struct
{
	uint16 musicMode;
	uint16 sbcOutFlag;
}AUD_AP2CP_PARA_T;
#endif
