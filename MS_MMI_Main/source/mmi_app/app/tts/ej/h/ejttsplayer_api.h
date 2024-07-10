/*****************************************************************************
** File Name:      ejTTSPlayer_api.h                                       *
** Author:                                                                   *
** Date:           2007.5.8                                                  *
** Copyright:      All Rights Reserved.                                      *
** Description:    ej tts                                             *
*****************************************************************************/

#ifndef _TTS_EJ_API_H_
#define _TTS_EJ_API_H_

#include "sci_types.h"
#include "os_api.h"
#include "guifont.h"
#include "mmiset_export.h"
#include "mmitts_export.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//#define MSG_TTS_OVER    0x1c02

//TTSһ�δ������󳤶�(��bytes��)
#define MMITTS_MAX_STRING_LEN_IN_ONESHOT (__IS_UCS2) ((300/6 -2)*((__IS_UCS2)?2:1))//(126*((__IS_UCS2)?2:1))     

#define MMITTS_MAX_VOICE_DATA_SIZE       (300 * 1024)//֧�ֵ����VOICE DATA��С

#define MMITTS_STRING_SIZE               (1024*2)


typedef struct
{
    uint8    str_arr[MMITTS_STRING_SIZE+2];         /*!< string arr */
    uint16   str_len;                               /*!< string length in bytes */  
    BOOLEAN  is_ucs2;                               /*!< string is ASCII or UCS2 */
//    uint8    str_arr[MMITTS_STRING_SIZE+2];         /*!< string arr */
    
    uint16   tts_vol;                               /*!< tts vol*/
    uint16   play_times;                            /*!< repeat play*/
    
    TTS_ALL_RING_TYPE_E     ring_type;              /*!< ring type*/
    uint16   tts_start_pos;                        /*!< the position which the string begin to play*/    
    uint16   tts_str_len;                          /*!< play string length in bytes */  
    MMI_HANDLE_T win_handle;
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

/*****************************************************************************/
//  Description : ���audio_handleֵ
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_GetAudioIsOpen(void);

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
//  Description :  ����TTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSRingType(TTS_ALL_RING_TYPE_E ring_type);

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
PUBLIC uint16 TTS_GetTTSVolumeType(void);

/*****************************************************************************/
//  Description :  ����TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSVolume(uint16 set_vol);

/*****************************************************************************/
//  Description :  TTSAPI_IsPromptWhenErr
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_IsPromptWhenErr(void);

/*****************************************************************************/
// 	Description : TTS_GetManualStop
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_GetManualStop(void);

/*****************************************************************************/
// 	Description : TTS_SetManualStop
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTS_SetManualStop(BOOLEAN is_manul_stop);

/*****************************************************************************/
// 	Description : ����¼��������������
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTSAPI_PlayPcmStream(uint32 pcm_stream_id);

/*****************************************************************************/
//  Description : ���audio_handleֵ
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_GetAudiohandle(void);

/*****************************************************************************/
//  Description : ����audio_handleֵ
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_SetAudiohandle(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : TTS_IsCommandStatusNone
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_IsCommandStatusNone(void);

/*****************************************************************************/
//  Description : TTS_IsPlayStatus
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_IsPlayStatus(void);



/*****************************************************************************/
//  Description : TTS_GetRoute
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T TTS_GetRoute(void);

/*****************************************************************************/
//  Description : TTS_GetPuncMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16  TTS_GetPuncMode(void);

/*****************************************************************************/
//  Description : GetDitigMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 TTS_GetDigitMode(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

