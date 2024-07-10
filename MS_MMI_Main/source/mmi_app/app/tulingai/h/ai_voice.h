#ifndef __AI_VOICE_H__
#define __AI_VOICE_H__
/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "ai_common.h"
#include "ai_api.h"
//#include "ai_voice.h"



#define MAX_AI_VOC_FILENAME_SIZE 40

#define AI_VOICE_SEND_DIR "D:\\AIVocSend"
#define AI_VOICE_RCV_DIR "D:\\AIVocRcv"

#define AI_VOICE_RCV_LAST_FILENAME "AIRcvLast.amr"

#define AI_FIX_BUF_COUNT 6
#define AI_FIX_BUF_SIZE  128*1024



typedef struct mp3_data
{
    uint8* data;
    uint32 data_len;
    uint32 alloc_len;
    uint16 buf_index;
    uint8 malloc_index;
}Mp3Data_T;

typedef struct Mp3_Buf_Queue
{
    Mp3Data_T mp3_data;
    struct Mp3_Buf_Queue *next;

}Mp3BufList_T;



typedef struct Mp3_FixBuf
{
    //uint8 data[AI_FIX_BUF_COUNT][AI_FIX_BUF_SIZE];
    uint8 *data[AI_FIX_BUF_COUNT];
    uint32 data_len[AI_FIX_BUF_COUNT];
    uint8 inserting_index;
    uint8 play_index;
    BOOLEAN is_init;
}Mp3FixBufHolder_T;




typedef struct _AI_VOC_SEND_DATA_t
{
    uint8          * pFilename;
    uint16         len;
} AI_VOC_SEND_DATA_T;

typedef struct _AI_VOC_SEND_NODE_t
{
       AI_VOC_SEND_DATA_T  data;
       struct _AI_VOC_SEND_NODE_t * next_ptr;
} AI_VOC_SEND_NODE_T;

typedef struct _AI_VOC_RCV_FILE_DATA_t
{
    uint16 voc_idx;
    uint8 * voc_data;
    uint16 voc_data_len;
    uint8 pack_idx;
    uint8 pack_sum;
}AI_VOC_RCV_FILE_DATA_T;

typedef struct _AI_VOC_RCV_FILE_NODE_t
{
       AI_VOC_RCV_FILE_DATA_T  data;
       struct _AI_VOC_RCV_FILE_NODE_t * next_ptr;
}AI_VOC_RCV_FILE_NODE_T;


typedef enum  _AI_RECORDER_DISPLAY_STAT
{
    AI_RECORDER_DISP_IDLE,
    AI_RECORDER_DISP_RECORDING,
    AI_RECORDER_DISP_RECORD_OK,
    AI_RECORDER_DISP_RECORD_FAIL,
    AI_RECORDER_DISP_SENDING,
    AI_RECORDER_DISP_SEND_OK,
    AI_RECORDER_DISP_SEND_FAIL,
    AI_RECORDER_DISP_MAX
}AI_RECORDER_DISPLAY_STAT;



typedef enum  _AI_VocRspType_STAT
{
	AI_VOCRSP_TYPE_RECORD,
	AI_VOCRSP_TYPE_RECORDSTOP,
	AI_VOCRSP_TYPE_SEND,
	AI_VOCRSP_TYPE_PLAY,
	AI_VOCRSP_TYPE_PLAYSTOP,
	AI_VOCRSP_TYPE_MAX
}AIVocRspTypeStatus;

typedef enum  _AI_VocRecordErr_STAT
{
	AI_VOCRECORD_SUCCESS,
	AI_VOCRECORD_ERR,
	AI_VOCRECORD_ERR_RECORDING,
	AI_VOCRECORD_ERR_NO_CARD,
	AI_VOCRECORD_ERR_NO_SRV,
	AI_VOCRECORD_ERR_LAND,
	AI_VOCRECORD_ERR_SPACE,
	AI_VOCRECORD_ERR_LOWTIME,
	AI_VOCRECORD_ERR_MP3PLAY,
	AI_VOCRECORD_ERR_FMON,
	AI_VOCRECORD_ERR_NET,
	AI_VOCRECORD_ERR_INCALL,
	AI_VOCRECORD_ERR_INHIDDEN,
	AI_VOCRECORD_ERR_MAX
}AIVocRecordErrStatus;

typedef enum  _AI_VocRcdStopErr_STAT
{
	AI_VOCRCDSTOP_SUCCESS,
	AI_VOCRCDSTOP_ERR,
	AI_VOCRCDSTOP_ERR_SPACE,
	AI_VOCRCDSTOP_ERR_LOWTIME,
	AI_VOCRCDSTOP_ERR_TIMEOUT,
	AI_VOCRCDSTOP_ERR_MAX
}AIVocRcdStopErrStatus;

typedef enum  _AI_VocSendErr_STAT
{
	AI_VOCSEND_SUCCESS,
	AI_VOCSEND_ERR,
	AI_VOCSEND_ERR_NET,
	AI_VOCSEND_ERR_FILE,
	AI_VOCSEND_ERR_UNREG,
	AI_VOCSEND_ERR_MAX
}AIVocSendErrStatus;



typedef enum  _AI_FilePlayErr_STAT
{
	AI_FILEPLAY_SUCCESS,
	AI_FILEPLAY_ERR,
	AI_FILEPLAY_ERR_PLAYING,
	AI_FILEPLAY_ERR_RECORRDING,
	AI_FILEPLAY_ERR_INCALL,
	AI_FILEPLAY_ERR_MP3PLAY,
	AI_FILEPLAY_ERR_FMON,
	AI_FILEPLAY_ERR_HIDDEN,
	AI_FILEPLAY_ERR_MAX
}AIFilePlayErrStatus;

 BOOLEAN  AI_VOC_Send_Link_DelAll(AI_APP_T *pMe);
 BOOLEAN AI_Voice_SendAlert(AI_APP_T * pMe,uint16 wParam);
extern int AI_Voice_HandleStop(AI_APP_T * pMe);
extern BOOLEAN AI_Voice_FileSendAllRemove(AI_APP_T * pMe);
extern BOOLEAN AI_Voice_FileRcvAllRemove(AI_APP_T * pMe);

extern BOOLEAN AI_Voice_FilePlay_Stop(AI_APP_T * pMe);
extern BOOLEAN AI_Voice_FilePlay_SetVolume(AI_APP_T * pMe,uint16 vol);

extern BOOLEAN AI_Voice_Record_Start(AI_APP_T * pMe);
extern BOOLEAN AI_Voice_Record_Stop(AI_APP_T * pMe);
extern BOOLEAN AI_Voice_Record_StopTimout(AI_APP_T * pMe);

extern int AI_Net_Receive_TK_VocFile(AI_APP_T *pMe,uint8 * pFiledata,uint16 DataLen);

//extern int32 AI_Net_Send_TK_VocFile_Start(AI_APP_T *pMe);
//extern void AI_Net_Send_TK_VocFile_End(AI_APP_T *pMe,uint8 err_id);

extern BOOLEAN AI_Voice_Send_Start(AI_APP_T * pMe);
extern BOOLEAN AI_Voice_FilePlay_Start(AI_APP_T * pMe);

extern int AI_API_Record_Start(void);
extern int AI_API_Record_Stop(void);
extern int AI_API_Record_Send(void);
extern BOOLEAN AI_API_Record_IsFilePlay(void);
extern BOOLEAN AI_API_Record_IsRecording(void);
extern int AI_API_Record_FilePlay(void);
extern int AI_API_Record_PlayStop(void);
extern int AI_API_SetVocPlayVolume(uint16 vol);
extern int AI_API_SetVocPlayVolumeUp(void);
extern int AI_API_SetVocPlayVolumeDown(void);
extern void AI_API_Record_SetPlayAll(BOOLEAN is_play_all);
extern BOOLEAN AI_API_Record_FileDelAll(AI_APP_T * pMe);
#ifdef FEATURE_APP_MP3PLAY
extern BOOLEAN bMP3Play;
#endif
extern BOOLEAN AI_Voice_Allow_Receive(AI_APP_T * pMe);
extern int AI_API_Rcv_Mp3_PlayStart(void);
extern void AI_Voice_Rcv_Mp3_StopPlayBuf(AI_APP_T * pMe);

//extern uint8 AI_Mp3BufList_InsertData(uint8* data, uint32 data_len, BOOLEAN *result);
//extern uint8 AI_Mp3BufList_InsertData(uint8* data, uint32 data_len, BOOLEAN *result, BOOLEAN download_finish);
extern uint8 AI_Mp3BufList_InsertData(uint8* data, uint32 data_len, BOOLEAN *result, uint32 *inserted_len, BOOLEAN download_finish);

extern void AI_Mp3InsertData(uint8* data, uint32 data_len);


extern void AI_Voice_Rcv_Mp3_PlayBufThreadStart();



#endif
