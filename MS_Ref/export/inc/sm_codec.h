/******************************************************************************
 ** File Name:      sm_codec.h                                                *
 ** Author:         wangxufeng                                                *
 ** DATE:           23/01/2010                                                *
 ** Copyright:      2010 Hisense, Incoporated. All Rights Reserved.           *
 ** Description:    This file defines the codec common data structure         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 23/01/2010     wangxufeng       Create.                                   *
 ******************************************************************************/

#ifndef __SM_VIDEO_CODEC_H__
#define __SM_VIDEO_CODEC_H__

#include "sci_types.h"
#include "audio_api.h"

#define SM_AUDIO_FRAME_NUM        5

#define CODEC_MEMORY              1024*110
#define FRAME_BUFFER              1024*1100

#define VIDEO_BUFFSIZE            1024*32//for 384kbps I Frame //1024*100
#define AUDIO_BUFFSIZE            1024*5
#define AUDIO_DATA_FRAME_NUM      200

typedef enum
{
    SM_AUDIO_TYPE_NOAUDIO,
    SM_AUDIO_TYPE_AMRNB,
    SM_AUDIO_TYPE_AMRWB,
    SM_AUDIO_TYPE_AAC,
    SM_AUDIO_TYPE_PCM,
    SM_AUDIO_TYPE_MP3,
    SM_AUDIO_TYPE_MP3_2,
    SM_AUDIO_TYPE_G723,
    SM_AUDIO_TYPE_MIDI,
    SM_AUDIO_TYPE_AACP,
    SM_AUDIO_TYPE_EVRC,
    SM_AUDIO_TYPE_QCELP,
    SM_AUDIO_TYPE_OGG,
    SM_AUDIO_TYPE_RA,
    SM_AUDIO_TYPE_UNKNOWN,
    SM_AUDIO_TYPE_MAX = SM_AUDIO_TYPE_UNKNOWN,
    SM_AUDIO_TYPE_TO_INT = 0x7FFFFFFF
} SM_AUDIO_TYPE_E;

typedef enum
{
	UNKNOWN_CODEC,
	EVRC_CODEC,
	QCELP_CODEC,
	AAC_CODEC,
	GSM_AMR_CODEC,
	MPEG4_CODEC,
	H263_CODEC,
	H264_CODEC,
	STILL_IMAGE_CODEC,
	TIMED_TEXT_CODEC,
	JPEG_CODEC,
	MP3_CODEC
}SM_VIDEO_TYPE;

typedef enum
{
	SM_AUDIO_IDLE,
	SM_AUDIO_PLAYING,
	SM_AUDIO_PAUSE,
	SM_AUDIO_RESUME,
	SM_AUDIO_STOP,
	SM_AUDIO_CLOSE,
	SM_AUDIO_MAX
}SM_AUDIO_STATE_E;

typedef struct
{
    uint8 * frame_ptr;
    uint32  frame_size[SM_AUDIO_FRAME_NUM];
    uint32  frame_num;
}AUDIO_FRAME_INFO_T;

typedef struct
{
    HAUDIO              audiohandle;
    SM_AUDIO_STATE_E    state;
    AUDIO_FRAME_INFO_T  ready_data;
    AUDIO_FRAME_INFO_T  play_data;
    uint32              allframe_size;
    uint32              cur_play_pos;
    uint32              req_data_pos;
    BOOLEAN             newdata_flag;
}AUDIO_PLAY_CTRL_T;

#define HS_CODEC_DEBUG
#ifdef  HS_CODEC_DEBUG
#define SM_CODEC_PRINT        SCI_TRACE_LOW
#else
#define SM_CODEC_PRINT(...)
#endif

#endif

