#ifndef __AI_API_H__
#define __AI_API_H__

#include "ai_common.h"
#include "ai_tcp.h"
//#include "ai_voice.h"



#define MAX_AI_VOC_FILENAME_SIZE 40

//#define AI_VOICE_SEND_DIR "D:\\AIVocSend"
//#define AI_VOICE_RCV_DIR "D:\\AIVocRcv"

//#define AI_VOICE_RCV_LAST_FILENAME "aiRcvLast.amr"



typedef enum  _AI_RECORDER_STAT
{
	AI_RECORDER_STOP,
	AI_RECORDER_RECORD
}AIRecorderStatus;

typedef enum  _AI_FILEPLAY_STAT
{
	AI_FILE_PLAYSTOP,
	AI_FILE_PLAYING,
}AIFilePlayStatus;




typedef struct _AI_APP_t {
    AI_TCP_INTERFACE_T * m_ai_tcp_reg_interface;
    uint8                       m_ai_reg_timer_id;
    uint8                       m_ai_land_timer_id;
    uint8                       m_ai_heart_timer_id;
    uint8                       m_ai_1a_timer_id;
    BOOLEAN         m_ai_net_is_open;
    BOOLEAN         m_ai_net_is_init;
    BOOLEAN         m_ai_is_first_reg;
    BOOLEAN         m_ai_is_reg;
    BOOLEAN         m_ai_allow_rereg;
    BOOLEAN         m_ai_is_land;
    uint32           m_ai_reg_ip;
    uint8          m_ai_reg_err_num;
    uint16         m_ai_reg_port;
    uint16         m_ai_rsp_err_num;
    BOOLEAN         m_ai_test_stop;
	char token[64];
	char user_id[17];
	char aes_key[17];
	char api_key[33];
	char test[8];//zhoumei mark

    uint16 m_ai_voc_send_times;

    BOOLEAN           m_ai_voc_is_sending;
    BOOLEAN           m_ai_voc_is_playing;
    int32               m_ai_voc_file_seek;
    uint8               m_ai_voc_pack_num;
    uint8                m_ai_voc_pack_idx;
    uint16               m_ai_voc_data_idx;
    MMISRV_HANDLE_T      m_ai_pRecordMedia;
    MMISRV_HANDLE_T      m_ai_pPlayMedia;
    char		m_ai_pFileSendVocName[MAX_AI_VOC_FILENAME_SIZE+1]; 
    char		m_ai_pFileRcvVocName[MAX_AI_VOC_FILENAME_SIZE+1]; 
    uint16		m_ai_pFileSendVocFullName[MAX_AI_VOC_FILENAME_SIZE+1];
    uint16		m_ai_pFileSendVocFullName_len;
    uint16		m_ai_pFileRcvVocFullName[MAX_AI_VOC_FILENAME_SIZE+1];
    uint16		m_ai_pFileRcvVocFullName_len;
    uint16       m_ai_cur_rcv_is_complete;
    AIRecorderStatus   m_ai_eRecorderStatus;
    AIFilePlayStatus    m_ai_eFilePlayrStatus;

    uint8 m_ai_voc_play_time_id;
    uint8 m_ai_voc_record_time_id;
    uint8 m_ai_voc_record_stop_time_id;
    uint8 m_ai_voc_rcv_time_id;
    
    

    


}AI_APP_T;

extern AI_APP_T  g_ai_app;

#endif
