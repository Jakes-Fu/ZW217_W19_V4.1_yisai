/*****************************************************************************
** File Name:      tts_iflytek_api.h                                         *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      All Rights Reserved.                                      *
** Description:    Iflytek tts                                               *
*****************************************************************************/

#ifndef _TTS_IFLYTEK_H_
#define _TTS_IFLYTEK_H_

#include "sci_types.h"
#include "os_api.h"
#include "guifont.h"

//#include "mmirecord_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmitts_export.h"
#include "Mmisrvaud_api.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//TTSһ�δ������󳤶�(��bytes��)
#define MMITTS_MAX_STRING_LEN_IN_ONESHOT (__IS_UCS2) ((300/6 -2)*((__IS_UCS2)?2:1))//(126*((__IS_UCS2)?2:1))     

#define MMITTS_MAX_VOICE_DATA_SIZE       (300 * 1024)//֧�ֵ����VOICE DATA��С

/*
#define KiTTS_HEAP_BUFFER_SIZE           (100*1024)  //(400*1024)

#ifdef TTS_FILE_RES
#define KiTTS_DATA_BUFFER_SIZE           (180*1024)
  #ifdef TTS_SUPPORT_LOW_MEMORY
    #define KiTTS_RESCATHEBUFFER_SIZE        (132*1024)
  #else
    #define KiTTS_RESCATHEBUFFER_SIZE        (516*1024)
  #endif
#else
#define KiTTS_DATA_BUFFER_SIZE           (80*1024)  
#define KiTTS_RESCATHEBUFFER_SIZE        (0)        //added by jun.hu 2010/11/5
#endif
*/

#define MMITTS_STRING_SIZE               (1024*2)


//tts �¼ӵĲ���
/* the range of voice speed value is from -32768 to +32767, ����ʵ��ʹ�õ���Ҫ���˵��� */
#define TTS_SPEED_SLOW                  -10000      /* slowest voice speed */
#define TTS_SPEED_NORMAL                 0          /* normal voice speed (default) */
#define TTS_SPEED_FAST                  +10000      /* fastest voice speed */

typedef struct
{
    uint16   str_len;                               /*!< string length in bytes */  
    BOOLEAN  is_ucs2;                               /*!< string is ASCII or UCS2 */
    uint8    str_arr[MMITTS_STRING_SIZE+2];         /*!< string arr */
    
    uint16   tts_vol;                               /*!< tts vol*/
    uint16   play_times;                            /*!< repeat play*/
    
    TTS_ALL_RING_TYPE_E  ring_type;                 /*!< ring type*/
    uint16   tts_start_pos;                        /*!< the position which the string begin to play*/    
    uint16   tts_str_len;                          /*!< play string length in bytes */  
    MMI_HANDLE_T win_handle;
	MMISRVAUD_ROUTE_T    tts_route;                           /*tts route*/
	MMITTS_DIGIT_MODE_TYPE_E                   digit_mode;
	MMITTS_PUNC_MODE_TYPE_E                   punc_mode;
	MMISRVAUD_VOLUME_TYPE_E volume_type;
}MMITTS_STRING_T;

typedef enum
{
    MMITTS_INVALID_PARAM,
    MMITTS_NO_FILE_RES,
    MMITTS_NO_MEMORY,
    MMITTS_PLAY_ERROR,
    MMITTS_PLAY_SUCCESS,
    MMITTS_RESULT_MAX
} MMITTS_RESULT_E;

PUBLIC BOOLEAN TTSAPI_PlayText(MMITTS_PLAY_PARAM_T * play_param_ptr);
PUBLIC void TTSAPI_StopPlayText(void);
PUBLIC BOOLEAN TTSAPI_GetPlayStatus(void);
PUBLIC BOOLEAN  TTSAPI_SynthTextByTask(void );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_NotifyTTSOver(void);

/*****************************************************************************/
//  Description : stop tts by type
//  Global resource dependence :
//  Author: aoke.hu
//  Note: ����ֵ true:ttsδ���Ż������ֹͣ����;  false:tts�Ծ��ڲ���
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description :  ͬ��ͣtts
//  Global resource dependence : none
//  Author: jun hu 2010/10 �޸��˴˺���....
//  Note: ͬ��ͣtts,����������¶�Ӧ���ô˺���....
/*****************************************************************************/
PUBLIC void TTSAPI_StopSynthTextByTask(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: jun.hu
//  Note:// for�첽ֹͣTTS������������������ʱ����, ��������û���ͷ�TTS��ռ�õ��ڴ�
// ������һЩASSERT��  ����������ô˺��� ...(jun.hu)....
/*****************************************************************************/
PUBLIC void TTSAPI_StopASynchTextByTask(void);
/*****************************************************************************/
//  Description : �ͷ��ڴ���Դ
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTS_MemoryRelease(void);
/*****************************************************************************/
//  Description : �ж��Ƿ��ڲ���
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_IsPlayRing(TTS_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description : ��ȡaudio����״̬
//  Global resource dependence :
//  Author: violent.wei
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN TTSAPI_GetAudioIsOpen(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_Init(void);
/*****************************************************************************/
//  Description : ����TTS�йش��ڵ�softekey
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_SetSoftkeyTextId( 
                                    MMI_WIN_ID_T    win_id,        // ����id
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update // whether update)
                                    );

/*****************************************************************************/
// 	Description : ����PcmStream
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTSAPI_PlayPcmStream(uint32 pcm_stream_id);
/*****************************************************************************/
//  Description : check  tts file 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_checkResFile(void);

/*****************************************************************************/
//  Description : gui ���ô˽ӿ��ʶ��˵� 
//  Global resource dependence : none
//  Author: violent.wei
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_PlayTextForControl(
                                      MMI_STRING_T* str_ptr, 
                                      uint32 num,
									  MMISRVAUD_ROUTE_T tts_route
                                      );

/*****************************************************************************/
//  Description : check repeate play text 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_IsRepeatePlayText(void);

/*****************************************************************************/
//  Description : ����TTS����״̬
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSRunStatus(BOOLEAN tts_is_run);

/*****************************************************************************/
//  Description :  ��ȡTTS����״̬
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_GetTTSRunStatus(void);

/*****************************************************************************/
//  Description :  ��ȡTTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC TTS_ALL_RING_TYPE_E TTS_GetTTSRingType(void);

/*****************************************************************************/
//  Description :  ��ȡTTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 TTS_GetTTSVolume(void);

/*****************************************************************************/
//  Description :  ��ȡTTS volume type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_VOLUME_TYPE_E TTS_GetTTSVolumeType(void);

/*****************************************************************************/
//  Description :  ����TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSVolume(uint16 set_vol);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
