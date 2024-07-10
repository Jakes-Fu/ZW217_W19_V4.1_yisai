/****************************************************************************
** File Name:      mmitts_export.h                                                *
** Author:         tonny.chen                                                 *
** Date:           08/06/2011                                             *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/06/2011  tonny.chen         Create                                  *
**                                                                         *
****************************************************************************/


#ifndef _TTS_API_H_
#define _TTS_API_H_

#include "sci_types.h"
#include "os_api.h"
#include "guifont.h"
//#include "mmiset.h"
#include "mmiset_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#if defined(TTS_SUPPORT_IFLYTEK) || defined(HERO_ENGINE_TTS_SUPPORT )
#define KiTTS_HEAP_BUFFER_SIZE           (100*1024)  //(400*1024)

#if defined(HERO_ENGINE_TTS_INROM_SUPPORT) 
#define KiTTS_DATA_BUFFER_SIZE           (80*1024)  
#define KiTTS_RESCATHEBUFFER_SIZE        (0)        //added by jun.hu 2010/11/5

#elif defined(TTS_FILE_RES) 
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

#endif //TTS_SUPPORT_IFLYTEK

#ifdef TTS_SUPPORT_YOUNGTONE
#ifndef YT_TTS_TWO_RES_FILE
#define KiTTS_HEAP_BUFFER_SIZE			(200*1024)
#else
#define KiTTS_HEAP_BUFFER_SIZE			(320*1024) //(358436)
#endif


#ifdef TTS_FILE_RES
#define KiTTS_DATA_BUFFER_SIZE             (100*1024)
#define KiTTS_RESCATHEBUFFER_SIZE        ((134+553)*1024)

#else //TTS_FILE_RES
#define KiTTS_DATA_BUFFER_SIZE           (80*1024)  
#define KiTTS_RESCATHEBUFFER_SIZE        (0)        //added by jun.hu 2010/11/5
#endif //TTS_FILE_RES

#endif //TTS_SUPPORT_YOUNGTONE

#ifdef TTS_SUPPORT_EJ
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

#endif //TTS_SUPPORT_EJ


#define MSG_TTS_PLAY                     0x1c00
#define MSG_TTS_STOP                     0x1c01
#define MSG_TTS_OVER                     0x1c02
#define MSG_TTS_AUDIO_PLAY               0x1c03
#define MSG_TTS_ERROR_RESOURCE           0x1c04

typedef enum
{
	TTS_RING_TYPE_NONE,
    TTS_RING_TYPE_CALL,      //来单铃声播报
    TTS_RING_TYPE_SMS,
    TTS_RING_TYPE_MMS,
    TTS_RING_TYPE_EBOOK,
    TTS_RING_TYPE_MENU,
#ifdef HERO_ENGINE_TTS_SUPPORT
    TTS_RING_TYPE_AUTO_TIME,
    TTS_RING_TYPE_DIAL,
    TTS_RING_TYPE_AKEY_TIME,
    TTS_RING_TYPE_NEW_SMS,
    TTS_RING_TYPE_CHARGE,
    TTS_RING_TYPE_LOW_POWER,
    TTS_RING_TYPE_LOCK_SCREEN,
    TTS_RING_TYPE_AKEY_POWER_LEVEL,
    TTS_RING_TYPE_APP,
#endif
    TTS_RING_TYPE_MAX   //枚举类型按优先级排列
}TTS_ALL_RING_TYPE_E;

typedef enum
{
	TTS_DIGIT_AUTO,		    // auto
    TTS_DIGIT_NUMBER,		// 按照号码方式读
    TTS_DIGIT_VALUE,		//  按照值读
	TTS_DIGIT_MODE_MAX
}MMITTS_DIGIT_MODE_TYPE_E;

typedef enum
{
	TTS_PUNC_OFF,		
    TTS_PUNC_ON,		 
	TTS_PUNC_MODE_MAX
}MMITTS_PUNC_MODE_TYPE_E;

typedef struct  
{
    uint8*              txt_addr;
    uint16              txt_len_in_byte;
    BOOLEAN             is_ucs2;
    uint16              play_times;
    uint8               tts_vol;
    TTS_ALL_RING_TYPE_E ring_type;
    MMI_HANDLE_T        win_handle;
	MMISRVAUD_ROUTE_T    tts_route;
	MMITTS_DIGIT_MODE_TYPE_E  digit_mode;
	MMITTS_PUNC_MODE_TYPE_E  punc_mode;
	MMISRVAUD_VOLUME_TYPE_E volume_type;
}MMITTS_PLAY_PARAM_T;



/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMITTS_PlayText             MMIAPITTS_PlayText
#define MMITTS_StopPlayTextByType   MMIAPITTS_StopPlayTextByType
#define MMITTS_StopPlayText         MMIAPITTS_StopPlayText
#define MMITTS_GetPlayStatus        MMIAPITTS_GetPlayStatus
#define MMITTS_SynthTextByTask      MMIAPITTS_SynthTextByTask
#define MMITTS_GetTTSRingType       MMIAPITTS_GetTTSRingType
#define MMITTS_StopSynthTextByTask  MMIAPITTS_StopSynthTextByTask
#define MMITTS_StopASynchTextByTask MMIAPITTS_StopASynchTextByTask
#define MMITTS_IsPlayRing           MMIAPITTS_IsPlayRing
#define MMITTS_GetAudioIsOpen       MMIAPITTS_GetAudioIsOpen
#define MMITTS_Init                 MMIAPITTS_Init
#define MMITTS_SetSoftkeyTextId     MMIAPITTS_SetSoftkeyTextId
#if defined(TTS_FILE_RES) && !defined(HERO_ENGINE_TTS_INROM_SUPPORT)
#define MMITTS_PlayPcmStream        MMIAPITTS_PlayPcmStream
#define MMITTS_CheckResFile         MMIAPITTS_CheckResFile
#endif
#if defined(MMI_READMENU_ENABLE) || defined(HERO_ENGINE_TTS_SUPPORT)
#define MMITTS_PlayTextForControl   MMIAPITTS_PlayTextForControl
#define MMITTS_StopPlayTextForControl      MMIAPITTS_StopPlayTextForControl
#define MMITTS_RestorePlayTextForControl   MMIAPITTS_RestorePlayTextForControl
#endif
#define MMITTS_SetTTSVolume         MMIAPITTS_SetTTSVolume

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
//  Description : 播放文本
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_PlayText(MMITTS_PLAY_PARAM_T * play_param_ptr);

/*****************************************************************************/
//  Description : 停止播放(按类型)
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description : 停止播放
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopPlayText(void);

/*****************************************************************************/
//  Description : 获取播放状态
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_GetPlayStatus(void);


/*****************************************************************************/
//  Description : 同步TTS
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPITTS_SynthTextByTask(void );

/*****************************************************************************/
//  Description : 获取tts 声音类型
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC TTS_ALL_RING_TYPE_E MMIAPITTS_GetTTSRingType(void);

/*****************************************************************************/
//  Description :  同步停tts
//  Global resource dependence : none
//  Author: jun hu 2010/10 修改了此函数....
//  Note: 同步停tts,绝大数情况下都应调用此函数....
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopSynthTextByTask(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: jun.hu
//  Note:// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT，  故请谨慎调用此函数 ...(jun.hu)....
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopASynchTextByTask(void);

/*****************************************************************************/
//  Description : 判断是否在播放
//  Global resource dependence :
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_IsPlayRing(TTS_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description : MMIAPITTS_GetAudioIsOpen
//  Global resource dependence :
//  Author: violent.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_GetAudioIsOpen(void);

/*****************************************************************************/
//  Description : init tts 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPITTS_Init(void);

/*****************************************************************************/
//  Description : 设置TTS有关窗口的softekey
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPITTS_SetSoftkeyTextId( 
                                    MMI_WIN_ID_T    win_id,        // 窗口id
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update // whether update)
                                    );

#if defined(TTS_FILE_RES) && !defined(HERO_ENGINE_TTS_INROM_SUPPORT)
/*****************************************************************************/
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: tonny.chen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITTS_PlayPcmStream(uint32 pcm_stream_id);

/*****************************************************************************/
//  Description : check  tts file 
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_CheckResFile(void);
#endif

#if defined(MMI_READMENU_ENABLE) || defined(HERO_ENGINE_TTS_SUPPORT)
/*****************************************************************************/
//  Description : gui 调用此接口朗读菜单 
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPITTS_PlayTextForControl(
                                      MMI_STRING_T* str_ptr, 
                                      uint32 num,
									  MMISRVAUD_ROUTE_T tts_route
                                      );

/*****************************************************************************/
//  Description : applets consuming much memory call this function to stop menu tts.
//  Global resource dependence : none
//  Author: Glen.Li
//  Note: refernce count adds 1. matching with MMIAPITTS_RestorePlayTextForControl;
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopPlayTextForControl(void);

/*****************************************************************************/
//  Description :  applets consuming much memory call this function to restore menu tts only for itself.
//  Global resource dependence : none
//  Author: Glen.Li
//  Note: refernce count subtracts 1.
/*****************************************************************************/
PUBLIC void MMIAPITTS_RestorePlayTextForControl(void);
#endif

/*****************************************************************************/
//  Description :  设置TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPITTS_SetTTSVolume(uint16 set_vol);



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
