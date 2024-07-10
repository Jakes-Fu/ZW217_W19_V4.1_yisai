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

//TTS一次处理的最大长度(以bytes计)
#define MMITTS_MAX_STRING_LEN_IN_ONESHOT (__IS_UCS2) ((300/6 -2)*((__IS_UCS2)?2:1))//(126*((__IS_UCS2)?2:1))     

#define MMITTS_MAX_VOICE_DATA_SIZE       (300 * 1024)//支持的最大VOICE DATA大小

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


//tts 新加的部分
/* the range of voice speed value is from -32768 to +32767, 根据实际使用的需要做了调整 */
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
//  Description : 获取audio播放状态
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
// 	Description : 播放PcmStream
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
//  Description : check repeate play text 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_IsRepeatePlayText(void);

/*****************************************************************************/
//  Description : 设置TTS运行状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSRunStatus(BOOLEAN tts_is_run);

/*****************************************************************************/
//  Description :  获取TTS运行状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_GetTTSRunStatus(void);

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
PUBLIC MMISRVAUD_VOLUME_TYPE_E TTS_GetTTSVolumeType(void);

/*****************************************************************************/
//  Description :  设置TTS volume
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
