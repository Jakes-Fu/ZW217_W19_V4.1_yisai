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

//TTS一次处理的最大长度(以bytes计)
#define MMITTS_MAX_STRING_LEN_IN_ONESHOT (__IS_UCS2) ((300/6 -2)*((__IS_UCS2)?2:1))//(126*((__IS_UCS2)?2:1))     

#define MMITTS_MAX_VOICE_DATA_SIZE       (300 * 1024)//支持的最大VOICE DATA大小

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
//  Description : 获得audio_handle值
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
//  Note: 返回值 true:tts未播放或调用了停止播放;  false:tts仍旧在播放
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description :  同步停tts
//  Global resource dependence : none
//  Author: jun hu 2010/10 修改了此函数....
//  Note: 同步停tts,绝大数情况下都应调用此函数....
/*****************************************************************************/
PUBLIC void TTSAPI_StopSynthTextByTask(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: jun.hu
//  Note:// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT，  故请谨慎调用此函数 ...(jun.hu)....
/*****************************************************************************/
PUBLIC void TTSAPI_StopASynchTextByTask(void);
/*****************************************************************************/
//  Description : 释放内存资源
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTS_MemoryRelease(void);
/*****************************************************************************/
//  Description : 判断是否在播放
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
//  Description : 设置TTS有关窗口的softekey
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_SetSoftkeyTextId( 
                                    MMI_WIN_ID_T    win_id,        // 窗口id
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
//  Description : gui 调用此接口朗读菜单 
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
//  Description :  设置TTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSRingType(TTS_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description :  获取TTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC TTS_ALL_RING_TYPE_E TTS_GetTTSRingType(void);

/*****************************************************************************/
//  Description :  获取TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 TTS_GetTTSVolume(void);

/*****************************************************************************/
//  Description :  获取TTS volume type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 TTS_GetTTSVolumeType(void);

/*****************************************************************************/
//  Description :  设置TTS volume
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
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTSAPI_PlayPcmStream(uint32 pcm_stream_id);

/*****************************************************************************/
//  Description : 获得audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_GetAudiohandle(void);

/*****************************************************************************/
//  Description : 设置audio_handle值
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

