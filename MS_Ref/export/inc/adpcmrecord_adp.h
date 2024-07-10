/******************************************************************************
 ** File Name:      AdpcmRecord_adp.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           01/04/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Adpcm Record Codec adapter layer.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/04/2006     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

#ifndef _ADPCMRECORD_ADP_H
#define _ADPCMRECORD_ADP_H


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "asm.h"
#include "audio_config.h"

//count of param to set in recording
#define RECORD_PARAM_SET_COUNT 9
#define RECORD_GET_RECORD_TIME_COUNT 1
#define RECORD_GET_RECORD_VOICEDB_COUNT 1
#define RECORD_SET_IF_FROM_NET_COUNT 1
#define RECORD_MP3_PARAM_SET_COUNT 9 //for mp3 record
#define RECORD_AMR_PARAM_SET_COUNT 11

#define RECORD_GET_FILE_SIZE_COUNT 1
#define RECORD_SET_END_TIME_COUNT 1
#define RECORD_SET_CB_TIME_COUNT 2
#define RECORD_SET_END_SIZE_COUNT 1

#define RECORD_PARAM_SET_STRAM_OUT_COUNT 4

typedef void (*RECORD_SREAM_UPLOAD_CALLBACK)(int16 * p_buf ,uint32 length);
typedef void (*REC_NOTIFY_CALLBACK)(HAUDIO hAudHandle); 
typedef enum
{
    RECORD_OUTPUT_SAVE_TO_FILE, 
    RECORD_OUTPUT_ARM_LOG,
    RECORD_OUTPUT_ARM_STREAM,
    RECORD_OUTPUT_BUFFER,
    RECORD_OUTPUT_MAX
}RECORD_OUTPUT_CHANNEL_E_;

typedef uint32 RECORD_OUTPUT_CHANNEL_E;

typedef struct
{
    AUD_REC_FORMAT_E            format;//ADPCM OR SOME MODE OF DSP_DATA_TO_TEST
    RECORD_OUTPUT_CHANNEL_E     output_channel;
    uint32                      time_limit;//unit:s,save file when recording time_limit(s)
    uint32                      frame_length;//the length of data from dsp to arm in once time.   
    uint16                      block_count;//the count of block. 
    uint16                      arfcn;
    uint32                      reserved;
    uint32                      samplerate;
    int16                      blockalign;
}RECORD_PARAM_T;

typedef struct
{
    AUD_REC_FORMAT_E            format;//ADPCM OR SOME MODE OF DSP_DATA_TO_TEST
    RECORD_OUTPUT_CHANNEL_E     output_channel;
    RECORD_SREAM_UPLOAD_CALLBACK    stream_upload_calback;
    void*                       other;
}RECORD_UPLOAD_PARAM_T;

typedef struct
{
    AUD_REC_FORMAT_E            format;
    RECORD_OUTPUT_CHANNEL_E     output_channel;
    uint32  time_limit;     //unit:s,save file when recording time_limit(s)
    uint32  sample_rate;      // sample rate of encoding music, 8K---48K
    uint32  bit_rate;         // bitrate of encoding music, 32k---320k.
    uint32  ch_count;         // channel count of encoding music,1 or 2
    uint16  MS_sign;          // MS sign, default 1
    uint16  VBR_sign;          // VBR_sign = 0 for CBR ; VBR_sign=1 for VBR.
    uint32  cut_off;          //should be (sample_rate/2)
}MP3_ENC_CFG_T;

typedef struct
{
    AUD_REC_FORMAT_E            format;//ADPCM OR SOME MODE OF DSP_DATA_TO_TEST
    RECORD_OUTPUT_CHANNEL_E     output_channel;
    uint32                      time_limit;//unit:s,save file when recording time_limit(s)
    uint32                      frame_length;//the length of data from dsp to arm in once time.   
    uint16                      block_count;//the count of block. 
    uint16                      arfcn;
    uint32                      reserved;
    uint32                      samplerate;
    int16                      blockalign;
    uint16                      amr_wb_mode;
    uint16                      stream_fmt;//0:default, 1:itu, 2:mime
}AMR_SW_ENC_PARAM_T;

typedef struct
{
	uint32  ui_record_time_in_ms;//[out]
}RECORD_TIME_IN_MS_T;

typedef struct
{
	uint32  db;//[out]
}RECORD_VOICEDB_T;

typedef struct
{
	BOOLEAN  b_if_from_net; //SCI_TRUE:from net, SCI_FALSE:not from net
}RECORD_IF_FROM_NET_T;

typedef struct
{
    uint32  cur_file_size;//[out]
}RECORD_FILE_SIZE;

typedef struct
{
    uint32  end_time;//[in]
}RECORD_END_TIME;

typedef struct
{
    REC_NOTIFY_CALLBACK pf_notify;    
    uint32  time_period;//[in] unit: ms
}RECORD_CB_TIME;

typedef struct 
{
    uint32 end_size; //input param, unit: bytes.
}RECORD_END_SIZE_T;

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Register ADPCM record to the audio server.
//  Author:         Jing.Li
//  Note:
/*****************************************************************************/
PUBLIC HAUDIOCODEC Record_RegCodecPlugger(
           HCOLLISION hCollision, AUDIO_CODEC_EXT_CFGINFO_T  *ptExtCfgInfo);

/*****************************************************************************/
//  Description:    Query encoder capability.
//  Author:         Kai.Wan
//  Note:    return TRUE if the format is supported
/*****************************************************************************/
PUBLIC BOOLEAN Record_QueryEncoderCapability(
           AUD_REC_FORMAT_E format);


/*
ext operation:

//operation1:
argc:RECORD_PARAM_SET_COUNT
argv:address of variable which type is RECORD_PARAM_T
ANSI2UINT16("SET_RECORDPARAM");

//operation2:
argc:RECORD_GET_RECORD_TIME_COUNT
argv:address of variable which type is RECORD_TIME_IN_MS_T
ANSI2UINT16("GET_RECORDTIME");

//operation3:
argc:RECORD_SET_IF_FROM_NET_COUNT
argv:address of variable which type is RECORD_IF_FROM_NET_T
ANSI2UINT16("SET_RECORDIFNET");
*/
           
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _ADPCMRECORD_ADP_H

// End of AdpcmRecord_adp.h

