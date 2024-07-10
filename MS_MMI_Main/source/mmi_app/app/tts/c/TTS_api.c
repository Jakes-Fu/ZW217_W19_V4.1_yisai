/****************************************************************************
** File Name:      tts_api.c                                                 *
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
#if defined(TTS_SUPPORT) || defined(HERO_ENGINE_TTS_SUPPORT )
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "threadx_os.h"
#include "os_apiext.h"
//#include "block_mem.h"

//#include "ivTTS.h"

#include "os_api.h"
#include "mmk_type.h"
#include "mmi_text.h"
#include "mmitts_export.h"

#ifdef TTS_SUPPORT_YOUNGTONE
#include "tts_youngtong_api.h"
#include "tts_youngtong_audio.h"
#endif

#ifdef TTS_SUPPORT_IFLYTEK
#include "tts_iflytek_api.h"
#include "tts_iflytek_audio.h"
#endif

#ifdef HERO_ENGINE_TTS_SUPPORT
#include "hero_tts_api.h"
#include "hero_tts_audio.h"
#endif

#ifdef TTS_SUPPORT_EJ
#include "ejTTSPlayer_api.h"
#endif
LOCAL uint32 s_num_of_applet_forbidden_to_play_menu_tts = 0;
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#if defined(TTS_FILE_RES) || defined(HERO_ENGINE_TTS_SUPPORT)

/*****************************************************************************/
//  Description : check  tts file 
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_CheckResFile(void)
{
#ifdef HERO_ENGINE_TTS_INROM_SUPPORT
	return 1;
#else
    return TTSAPI_checkResFile(); 
#endif
}

/*****************************************************************************/
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: tonny.chen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITTS_PlayPcmStream(uint32 pcm_stream_id)
{
    TTSAPI_PlayPcmStream(pcm_stream_id);
}

#endif //TTS_FILE_RES

/*****************************************************************************/
//  Description : TTS模块初始化
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPITTS_Init(void)
{
    TTSAPI_Init();
}

/*****************************************************************************/
//  Description : 获取audio 打开的状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_GetAudioIsOpen(void)
{
    return TTSAPI_GetAudioIsOpen();
}

/*****************************************************************************/
//  Description : 设置TTS有关窗口的softekey
//  Global resource dependence :
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPITTS_SetSoftkeyTextId( 
                                    MMI_WIN_ID_T    win_id,        // 窗口id
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update // whether update)
                                    )
{
    TTSAPI_SetSoftkeyTextId(win_id, leftsoft_id, rightsoft_id, is_need_update);
}

/*****************************************************************************/
//  Description : 判断是否在播放
//  Global resource dependence :
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_IsPlayRing(TTS_ALL_RING_TYPE_E ring_type)
{
	BOOLEAN result = FALSE;

	result = TTSAPI_IsPlayRing(ring_type); 
	SCI_TRACE_LOW("MMIAPITTS_IsPlayRing, result = %d",result);
	
	return result;
}

/*****************************************************************************/
//  Description : 同步停tts
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 同步停tts,绝大数情况下都应调用此函数....
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopSynthTextByTask(void)
{
    TTSAPI_StopSynthTextByTask();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: jun.hu
//  Note:// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT，  故请谨慎调用此函数 ...(jun.hu)....
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopASynchTextByTask(void)
{
    TTSAPI_StopASynchTextByTask();
}

/*****************************************************************************/
//  Description : 同步tts
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPITTS_SynthTextByTask(void)
{
    return TTSAPI_SynthTextByTask();
}

/*****************************************************************************/
//  Description : 获取play状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_GetPlayStatus(void)
{
    return TTSAPI_GetPlayStatus();
}

/*****************************************************************************/
//  Description : 获取tts 声音类型
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC TTS_ALL_RING_TYPE_E MMIAPITTS_GetTTSRingType(void)
{
    return TTS_GetTTSRingType();
}

/*****************************************************************************/
//  Description : 停止 play text 接口
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopPlayText()
{
    if(TTSAPI_GetPlayStatus())
    {
        TTSAPI_StopPlayText();
    }
}

/*****************************************************************************/
//  Description : 停止 play text 接口
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type)
{
    TTSAPI_StopPlayTextByType(ring_type);
}

/*****************************************************************************/
//  Description : play text 接口
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITTS_PlayText(MMITTS_PLAY_PARAM_T * play_param_ptr)
{
    return TTSAPI_PlayText(play_param_ptr);    
}

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
                                      )
{
     if(0 < s_num_of_applet_forbidden_to_play_menu_tts)
    {
        return;
    }
     
    TTSAPI_PlayTextForControl(str_ptr, num, tts_route);
}

/*****************************************************************************/
//  Description : applets consuming much memory call this function to stop menu tts.
//  Global resource dependence : none
//  Author: Glen.Li
//  Note: refernce count adds 1; applets have to use it matching with MMIAPITTS_RestorePlayTextForControl;
/*****************************************************************************/
PUBLIC void MMIAPITTS_StopPlayTextForControl(void)
{
#ifndef HERO_ENGINE_TTS_SUPPORT
     s_num_of_applet_forbidden_to_play_menu_tts += 1;
#endif
     MMIAPITTS_StopPlayText();
}

/*****************************************************************************/
//  Description :  applets consuming much memory call this function to restore menu tts only for itself.
//  Global resource dependence : none
//  Author: Glen.Li
//  Note: refernce count subtracts 1;applets have to use it matching with MMIAPITTS_StopPlayTextForControl;
/*****************************************************************************/
PUBLIC void MMIAPITTS_RestorePlayTextForControl(void)
{
    if(0 < s_num_of_applet_forbidden_to_play_menu_tts)
    {
        s_num_of_applet_forbidden_to_play_menu_tts -= 1;
    }
    else
    {
        s_num_of_applet_forbidden_to_play_menu_tts = 0;
    }
}
#endif

/*****************************************************************************/
//  Description :  设置TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPITTS_SetTTSVolume(uint16 set_vol)
{
    TTS_SetTTSVolume(set_vol);
}


#ifdef   __cplusplus
    }
#endif

#endif //TTS_SUPPORT
