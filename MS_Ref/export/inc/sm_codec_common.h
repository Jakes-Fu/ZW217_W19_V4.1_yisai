/******************************************************************************
 ** File Name:      sm_codec_common.h                                         *
 ** Author:         wangxufeng                                                *
 ** DATE:           23/01/2010                                                *
 ** Copyright:      2010 Hisense, Incoporated. All Rights Reserved.           *
 ** Description:    This file defines the codec common interface              *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 23/01/2010     wangxufeng       Create.                                   *
 ******************************************************************************/

#ifndef __SM_CODEC_COMMON_H__
#define __SM_CODEC_COMMON_H__

#include "sci_types.h"

#define SM_MAX_DATA_ITEMS      2

typedef enum
{
    SM_RET_SUCCESS,
    SM_RET_INVALID_PARAM,
    SM_RET_NOT_SUPPORT,
    SM_RET_INVALID_STATE,
    SM_RET_MEM_OVERFLOW,
    SM_RET_HREF_ERROR,
    SM_RET_DEC_FIFO_FULL,

    SM_RET_UNKNOWN_COMMAND,
    SM_RET_FAIL,
    SM_NONE_DATA,
    SM_RET_DMA_OCCUPIED,
    SM_RET_MVE_NO_EFFECT_IMAGE,

    AP_SM_RET_MAX
}SM_RESULT_TYPE_E;

typedef struct{
    uint16	   	left;        	   	/*!<left up point coordinate x */
    uint16	   	top;           	   	/*!<left up point coordinate y */
    uint16	   	right;         	   	/*!<right down point coordinate x */
    uint16	   	bottom;       	   	/*!<right down point coordinate y */
}SM_RECT_T;

typedef enum
{
    SM_ROTATE_0,
    SM_ROTATE_90,
    SM_ROTATE_180,
    SM_ROTATE_270,

    SM_ROTATE_MAX   = 0x7FFFFFFF    /*  uskim jpeg rot  */
} SM_ROTATE_TYPE;

typedef enum
{
    Audio_Sucess = 0,
    Audio_Fail,
    Audio_Frame_Dec_End,
    Audio_MAX
} SM_STATUS_TYPE;

typedef enum
{
    VIDEO_REQUEST_DATA, 
    VIDEO_STATUS,
    AUDIO_REQUEST_DATA,
    AUDIO_CMD_PLAY,
    AUDIO_CMD_STOP,     /* Stop command          */
    AUDIO_CMD_PAUSE,    /* Pause command         */
    AUDIO_CMD_RESUME,   /* Resume command        */
    AUDIO_CMD_FLUSH,    /* Data flush command    */
    AUDIO_CMD_INVALID   /* Invalid command       */
}SM_CODEC_COMMAND_CB;

typedef enum
{
    AVC_NALTYPE_SLICE = 1,  /* non-IDR non-data partition */
    AVC_NALTYPE_DPA = 2,    /* data partition A */
    AVC_NALTYPE_DPB = 3,    /* data partition B */
    AVC_NALTYPE_DPC = 4,    /* data partition C */
    AVC_NALTYPE_IDR = 5,    /* IDR NAL */
    AVC_NALTYPE_SEI = 6,    /* supplemental enhancement info */
    AVC_NALTYPE_SPS = 7,    /* sequence parameter set */
    AVC_NALTYPE_PPS = 8,    /* picture parameter set */
    AVC_NALTYPE_AUD = 9,    /* access unit delimiter */
    AVC_NALTYPE_EOSEQ = 10, /* end of sequence */
    AVC_NALTYPE_EOSTREAM = 11, /* end of stream */
    AVC_NALTYPE_FILL = 12   /* filler data */
} AVCNalUnitType;

typedef int (*CODEC_CALLBACK)(SM_CODEC_COMMAND_CB cmd, SM_STATUS_TYPE lparm ,int wparm, void * reserved);

typedef struct
{
    CODEC_CALLBACK  Video_cb;
    CODEC_CALLBACK  Audio_cb;
}SM_CODEC_CALLBACK_T;

typedef enum
{
    Video_Cb = 0,
    Audio_cb
}CODEC_CALLBACK_TYPE;

typedef enum
{
    SM_ENUM_AUD_DEC_IMMED,
    SM_ENUM_AUD_DEC_CALLBACK,
    SM_ENUM_AUD_DEC_ADD_BUF
} SM_AUD_DEC_OPCODE_TYPE;

typedef struct
{
    uint8 * pAudioBuffer;
    uint32  Frame_Num;
	uint32  Datalen;
    uint32  *Data_FrameLen;
    SM_AUD_DEC_OPCODE_TYPE opcodec;
} SM_AUDIO_INPUTDATA_PAR;

typedef struct
{
    uint8  * pAudio_Data_Ptr;
    uint32   Audio_Input_Data_Size;
    uint32 * Audio_Frame_DataSize;
    uint32   Audio_Frame_Num;
}Audio_InputData_Par;

typedef struct
{
    BOOLEAN Audio_Data_Buff_Ctl ;
    BOOLEAN Audio_DataInput_First ;
    BOOLEAN Audio_DataReq_Ctrl ;
    BOOLEAN Audio_Resume_Ctrl;
}Audio_Play_CtrPar;

typedef struct
{
   uint8  * Buffer_Ptr;
   uint32   BUFFER_SIZE;
   //uint8 * Buffer_IndPtr;
   uint32 * Frame_Len;
   uint32   Frame_Num;
}SM_DataBuffer_Par;

typedef struct{
  SM_DataBuffer_Par ItemArray[SM_MAX_DATA_ITEMS];
  uint32 nDataRequestFirstOut;
  uint32 nDataRequestLastIn;
  uint32 nDataRequestCount;
  uint32 nDataReqestMaxNum;
  uint32 nDataBufferSize;
  BOOLEAN bFirstOut;
}SM_RequestQ_T;

//DSP初始化需要的音频配置信息
typedef struct
{
   uint8  aduio_type;
   uint8  num_channels;
   uint32 sample_rate;
   uint8* p_audiodata;
   int    ndataLen;
}AUDIO_CODEC_CONFIG_T;

//interface function:
PUBLIC SM_RESULT_TYPE_E SM_VideoDecOpen(int video_type, uint8 *header, uint32 headerlen);
PUBLIC BOOLEAN SM_VideoSetDisplayParam( SM_RECT_T * rect, SM_ROTATE_TYPE rorate);
PUBLIC SM_RESULT_TYPE_E SM_VideoDecode(uint8 *pucPacket, uint32 *piPacketLength);
PUBLIC SM_RESULT_TYPE_E SM_VideoShowFrame(BOOLEAN isDrop);
PUBLIC SM_RESULT_TYPE_E SM_VideoDecClose(void);
PUBLIC int SM_Video_Display_Close(void);

PUBLIC void SM_AudioDecOpen(AUDIO_CODEC_CONFIG_T * audio_config);

PUBLIC SM_RESULT_TYPE_E SM_AudioDecInputData( SM_AUDIO_INPUTDATA_PAR *Audio_InputData_Par );
PUBLIC SM_RESULT_TYPE_E SM_AudioDecPause(void );
PUBLIC SM_RESULT_TYPE_E SM_AudioDecResume(void );
PUBLIC SM_RESULT_TYPE_E SM_AudioDecSendFrameBuffer(void );
PUBLIC void SM_AudioDecClose(void );
PUBLIC int SM_VideoDisplayOpen(void);
PUBLIC BOOLEAN SM_VideoSetDisplayParam( SM_RECT_T * rect, SM_ROTATE_TYPE rorate);

PUBLIC void SMInitQueueDataItems(void *prequestQ, int maxnum, SM_DataBuffer_Par *Item, uint32 DataBufferSize);
PUBLIC SM_RESULT_TYPE_E SM_SaveRequestQ(void *prequestQ, uint8 *pdata,uint32 Frame_Num, uint32* Frame_len, uint32 Data_len);
PUBLIC SM_RESULT_TYPE_E SM_GetRequestQ(void *prequestQ, SM_DataBuffer_Par * Item, BOOLEAN is_move_item);
PUBLIC SM_RESULT_TYPE_E SM_RemoveReqestQFirstOut(void *prequestQ);

PUBLIC SM_RESULT_TYPE_E SM_RegCB(CODEC_CALLBACK callback_fun, CODEC_CALLBACK_TYPE type);

BOOLEAN AMR_AudioOpen(AUDIO_CODEC_CONFIG_T * audio_config);
int AMR_AudioPutData(uint8 * frame_data, uint32 *frame_len, uint32 frame_num);
int AMR_AudioPlay(void);
int AMR_AudioPause(void);
int AMR_AudioResume(void);
int AMR_AudioClose(unsigned int uUserData );

BOOLEAN AAC_AudioOpen(AUDIO_CODEC_CONFIG_T * audio_config);
unsigned int AAC_AudioPutData(uint8 * frame_data, uint32 *frame_len, uint32 frame_num);
int AAC_AudioPlay(void);
int AAC_AudioPause(void);
int AAC_AudioResume(void);
unsigned int AAC_AudioClose(unsigned int uUserData );

PUBLIC int AudioCodec_PutSema(void);

extern int Pv_File_Open(const char *filename, const char *mode );
extern int Pv_File_Write(void *buffer, unsigned long int len, int fp );
extern void Pv_File_Close(int fp);

#endif

