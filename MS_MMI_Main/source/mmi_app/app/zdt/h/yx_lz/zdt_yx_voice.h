#ifndef __ZDT_YX_VOICE_H__
#define __ZDT_YX_VOICE_H__
/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "zdt_common.h"
#include "zdt_yx_api.h"


#define MAX_YX_VOC_FILENAME_SIZE 40

#define MAX_YX_VOC_SAVE_SIZE 10 //default use 20


#define YX_VOICE_SEND_DIR "D:\\YXVocSend"
#define YX_VOICE_RCV_DIR "D:\\YXVocRcv"

#define YX_VOICE_RCV_LAST_FILENAME "Last.amr"

typedef struct _YX_VOC_SEND_DATA_t
{
    uint8          * pFilename;
    uint16         len;
} YX_VOC_SEND_DATA_T;

typedef struct _YX_VOC_SEND_NODE_t
{
       YX_VOC_SEND_DATA_T  data;
       struct _YX_VOC_SEND_NODE_t * next_ptr;
} YX_VOC_SEND_NODE_T;

typedef struct _YX_VOC_RCV_FILE_DATA_t
{
    uint16 voc_idx;
    uint8 * voc_data;
    uint16 voc_data_len;
    uint8 pack_idx;
    uint8 pack_sum;
}YX_VOC_RCV_FILE_DATA_T;

typedef struct _YX_VOC_RCV_FILE_NODE_t
{
       YX_VOC_RCV_FILE_DATA_T  data;
       struct _YX_VOC_RCV_FILE_NODE_t * next_ptr;
}YX_VOC_RCV_FILE_NODE_T;


typedef enum  _YX_RECORDER_DISPLAY_STAT
{
    YX_RECOREDER_DISP_IDLE,
    YX_RECOREDER_DISP_RECORDING,
    YX_RECOREDER_DISP_RECORD_OK,
    YX_RECOREDER_DISP_RECORD_FAIL,
    YX_RECOREDER_DISP_SENDING,
    YX_RECOREDER_DISP_SEND_OK,
    YX_RECOREDER_DISP_SEND_FAIL,
    YX_RECOREDER_DISP_MAX
}YX_RECORDER_DISPLAY_STAT;

typedef enum  _YX_RECORDER_STAT
{
	YX_RECOREDER_STOP,
	YX_RECOREDER_RECORD
}YXRecorderStatus;

typedef enum  _YX_VocRspType_STAT
{
	YX_VOCRSP_TYPE_RECORD,
	YX_VOCRSP_TYPE_RECORDSTOP,
	YX_VOCRSP_TYPE_SEND,
	YX_VOCRSP_TYPE_PLAY,
	YX_VOCRSP_TYPE_PLAYSTOP,
	YX_VOCRSP_TYPE_MAX
}YXVocRspTypeStatus;

typedef enum  _YX_VocRecordErr_STAT
{
	YX_VOCRECORD_SUCCESS,
	YX_VOCRECORD_ERR,
	YX_VOCRECORD_ERR_RECORDING,
	YX_VOCRECORD_ERR_NO_CARD,
	YX_VOCRECORD_ERR_NO_SRV,
	YX_VOCRECORD_ERR_LAND,
	YX_VOCRECORD_ERR_SPACE,
	YX_VOCRECORD_ERR_LOWTIME,
	YX_VOCRECORD_ERR_MP3PLAY,
	YX_VOCRECORD_ERR_FMON,
	YX_VOCRECORD_ERR_NET,
	YX_VOCRECORD_ERR_INCALL,
	YX_VOCRECORD_ERR_INHIDDEN,
	YX_VOCRECORD_ERR_MAX
}YXVocRecordErrStatus;

typedef enum  _YX_VocRcdStopErr_STAT
{
	YX_VOCRCDSTOP_SUCCESS,
	YX_VOCRCDSTOP_ERR,
	YX_VOCRCDSTOP_ERR_SPACE,
	YX_VOCRCDSTOP_ERR_LOWTIME,
	YX_VOCRCDSTOP_ERR_TIMEOUT,
	YX_VOCRCDSTOP_ERR_MAX
}YXVocRcdStopErrStatus;

typedef enum  _YX_VocSendErr_STAT
{
	YX_VOCSEND_SUCCESS,
	YX_VOCSEND_ERR,
	YX_VOCSEND_ERR_NET,
	YX_VOCSEND_ERR_FILE,
	YX_VOCSEND_ERR_UNREG,
	YX_VOCSEND_ERR_MAX
}YXVocSendErrStatus;

typedef enum  _YX_FILEPLAY_STAT
{
	YX_FILE_PLAYSTOP,
	YX_FILE_PLAYING,
}YXFilePlayStatus;

typedef enum  _YX_FilePlayErr_STAT
{
	YX_FILEPLAY_SUCCESS,
	YX_FILEPLAY_ERR,
	YX_FILEPLAY_ERR_PLAYING,
	YX_FILEPLAY_ERR_RECORRDING,
	YX_FILEPLAY_ERR_INCALL,
	YX_FILEPLAY_ERR_MP3PLAY,
	YX_FILEPLAY_ERR_FMON,
	YX_FILEPLAY_ERR_HIDDEN,
	YX_FILEPLAY_ERR_MAX
}YXFilePlayErrStatus;

extern BOOLEAN  YX_VOC_Send_Link_DelAll(YX_APP_T *pMe);
extern BOOLEAN YX_Voice_SendAlert(YX_APP_T * pMe,uint16 wParam);
extern int YX_Voice_HandleStop(YX_APP_T * pMe);
extern BOOLEAN YX_Voice_FileSendAllRemove(YX_APP_T * pMe);
extern BOOLEAN YX_Voice_FileRcvAllRemove(YX_APP_T * pMe);

extern BOOLEAN YX_Voice_FilePlay_Stop(YX_APP_T * pMe);
extern BOOLEAN YX_Voice_FilePlay_SetVolume(YX_APP_T * pMe,uint16 vol);

extern BOOLEAN YX_Voice_Record_Start(YX_APP_T * pMe);
extern BOOLEAN YX_Voice_Record_Stop(YX_APP_T * pMe);
extern BOOLEAN YX_Voice_Record_StopTimout(YX_APP_T * pMe);

extern int YX_Net_Receive_TK_VocFile(YX_APP_T *pMe,uint8 * pFiledata,uint16 DataLen);

extern int32 YX_Net_Send_TK_VocFile_Start(YX_APP_T *pMe);
extern void YX_Net_Send_TK_VocFile_End(YX_APP_T *pMe,uint8 err_id);

extern BOOLEAN YX_Voice_Send_Start(YX_APP_T * pMe);
extern BOOLEAN YX_Voice_FilePlay_Start(YX_APP_T * pMe);

extern int YX_API_Record_Start(void);
extern int YX_API_Record_Stop(void);
extern int YX_API_Record_Send(void);
extern BOOLEAN YX_API_Record_IsFilePlay(void);
extern BOOLEAN YX_API_Record_IsRecording(void);
extern int YX_API_Record_FilePlay(void);
extern int YX_API_Record_PlayStop(void);
extern int YX_API_SetVocPlayVolume(uint16 vol);
extern int YX_API_SetVocPlayVolumeUp(void);
extern int YX_API_SetVocPlayVolumeDown(void);
extern void YX_API_Record_SetPlayAll(BOOLEAN is_play_all);
extern BOOLEAN YX_API_Record_FileDelAll(YX_APP_T * pMe);
#ifdef FEATURE_APP_MP3PLAY
extern BOOLEAN bMP3Play;
#endif
extern BOOLEAN YX_Voice_Allow_Receive(YX_APP_T * pMe);

#endif
