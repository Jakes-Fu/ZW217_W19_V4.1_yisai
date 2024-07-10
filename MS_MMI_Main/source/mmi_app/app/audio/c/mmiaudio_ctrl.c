#if 0
/*****************************************************************************
** File Name:      mmiaudio_ctrl.c                                           *
** Author:                                                                   *
** Date:           9/26/2006                                                 *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe audio ctrl                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2006                         Create
*****************************************************************************/

#define _MMIAUDIO_CTRL_C

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_audio_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiaudio_ctrl.h" 
#include "mmimp3_export.h"
#include "mmifm_export.h"
//#include "sci_types.h"

#include "mmiudisk_export.h"
#include "mmiset.h"
#include "mmidc_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#include "gpio_prod_api.h"
#include "mmk_timer.h"
#include "mmibt_export.h"// baokun add
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#endif
#include "mmimms_export.h"
#include "mmidc_export.h"
#include "mmienvset.h"
#include "mmimtv_export.h"
#include "mmiatv_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_export.h "	
#endif
#include "mmisms_export.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    uint32 bottom;
    uint32 top;
    uint32 size;
    uint32 * stack_ptr;
}MMI_AUDIO_STACK_T;

typedef struct
{
    MMI_WIN_ID_T        win_id;
    MMI_MESSAGE_ID_E    msg_id;
}MMI_AUDIO_KEY_NOTIFY_T;

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
//Background play(mp3, fm) manage function begin
LOCAL BOOLEAN s_bgplay_modules[MMIBGPLAY_MODULE_MAX] = {0};
LOCAL BOOLEAN s_music_modules[MMIBGPLAY_MODULE_MAX] = {0};  //just for MMIBGPLAY_TYPE_MUSIC
LOCAL BOOLEAN s_key_ring_cfg[MMIAUDIO_KEY_ALL][MMIBGPLAY_MODULE_MAX] = {0};
LOCAL BOOLEAN s_is_enable_keyring = TRUE;     //键盘音
LOCAL BOOLEAN s_is_enable_tp_ring = TRUE;     //触笔音
LOCAL MMI_AUDIO_KEY_NOTIFY_T s_last_key_notify = {0};   //最近一次按键音播放结束的通知结构--窗口和消息
LOCAL SCI_MUTEX_PTR s_audio_ctrl_mutex = PNULL;
LOCAL MMI_AUDIO_STACK_T s_audio_stack = {0};
LOCAL uint32 bg_stack[MMIBGPLAY_TYPE_ALL - 1] = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : InitStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitStack(void);

/*****************************************************************************/
//  Description : ClearStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ClearStack(void);

/*****************************************************************************/
//  Description : PopStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void PopStack(void);

/*****************************************************************************/
//  Description : PushStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PushStack(uint32 item);

/*****************************************************************************/
//  Description : IsEmptyStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsEmptyStack(void);

/*****************************************************************************/
//  Description : IsFullStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsFullStack(void);

/*****************************************************************************/
//  Description : PopBgItem
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void PopBgItem(uint32 item);

/*****************************************************************************/
//  Description : PushBgItem
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PushBgItem(uint32 item);

/*****************************************************************************/
//  Description : IsActiveBg
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsActiveBg(MMIBGPLAY_TYPE_E bg_type);

/*****************************************************************************/
//  Description : GetActiveBg
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMIBGPLAY_TYPE_E GetActiveBg(void);

/*****************************************************************************/
//  Description : IsBgPlayInStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsBgPlayInStack(
                  MMIBGPLAY_TYPE_E  bg_type
                  );
                  
/*****************************************************************************/
//  Description : PauseBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void PauseBgPlay(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  );

/*****************************************************************************/
//  Description : ResumeBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ResumeBgPlay(
                      MMIBGPLAY_MODULE_E    module          //module exit
                      );

/*****************************************************************************/
//  Description : Init key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKeyCfg(void);

/*****************************************************************************/
//  Description : Init key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void StopKeyRing(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                );

/*****************************************************************************/
//  Description : update key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void UpdateKeyCfg(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                );

/*****************************************************************************/
//  Description : update key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void UpdateKeyStatus(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                );

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : InitStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitStack(void)
{
    SCI_MEMSET(&s_audio_stack, 0x00, sizeof(MMI_AUDIO_STACK_T));
    s_audio_stack.stack_ptr = bg_stack;
    s_audio_stack.size = sizeof(bg_stack)/sizeof(uint32);
}


/*****************************************************************************/
//  Description : ClearStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ClearStack(void)
{
    InitStack();
}

/*****************************************************************************/
//  Description : PopStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void PopStack(void)
{
    if(!IsEmptyStack())
    {
        s_audio_stack.top --;
    }
}

/*****************************************************************************/
//  Description : PushStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PushStack(uint32 item)
{
    BOOLEAN result = FALSE;

    if(!IsFullStack())
    {
        s_audio_stack.stack_ptr[s_audio_stack.top] = item;
        s_audio_stack.top ++;
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : IsEmptyStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsEmptyStack(void)
{
    return (s_audio_stack.top == s_audio_stack.bottom);
}


/*****************************************************************************/
//  Description : IsFullStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsFullStack(void)
{
    return (s_audio_stack.top == s_audio_stack.bottom + s_audio_stack.size);
}


/*****************************************************************************/
//  Description : PopBgItem
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void PopBgItem(uint32 item)
{
    uint32  item_num = 0;
    uint32  i = 0;
    uint32  j = 0;
    
    if(!IsEmptyStack())
    {
        item_num = s_audio_stack.top - s_audio_stack.bottom;
    }

    for(i = 0; i<item_num; i++)
    {
        if(s_audio_stack.stack_ptr[i] == item)
        {
            for(j = i; j+1<item_num; j++)
            {
                s_audio_stack.stack_ptr[j] = s_audio_stack.stack_ptr[j+1];
            }
            s_audio_stack.top --;
        }
    }
}

/*****************************************************************************/
//  Description : PushBgItem
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PushBgItem(uint32 item)
{
    PopBgItem(item);

    return PushStack(item);
}

/*****************************************************************************/
//  Description : IsActiveBg
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsActiveBg(MMIBGPLAY_TYPE_E bg_type)
{
    BOOLEAN result = FALSE;
        
    if(!IsEmptyStack())
    {
        if(s_audio_stack.stack_ptr[s_audio_stack.top - 1] == (uint32)bg_type)
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : GetActiveBg
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMIBGPLAY_TYPE_E GetActiveBg(void)
{
    MMIBGPLAY_TYPE_E bg_type = MMIBGPLAY_TYPE_NONE;
    
    if(!IsEmptyStack())
    {
        bg_type = (MMIBGPLAY_TYPE_E)s_audio_stack.stack_ptr[s_audio_stack.top - 1];
    }

    return bg_type;
}

/*****************************************************************************/
//  Description : IsBgPlayInStack
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsBgPlayInStack(
                  MMIBGPLAY_TYPE_E  bg_type
                  )
{
    BOOLEAN     result = FALSE;
    uint32      i = 0;
    
    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);
    
    for (i=0; i<s_audio_stack.top; i++)
    {
        if(s_audio_stack.stack_ptr[i] == (uint32)bg_type)
        {
            result = TRUE;
            break;
        }
    }
    
    SCI_PutMutex(s_audio_ctrl_mutex);

    //SCI_TRACE_LOW:"[MMIAUDIO]:IsBgPlayInStack,bg_type = 0x%x, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_391_112_2_18_2_3_40_117,(uint8*)"dd",bg_type, result);
    
    return (result);
}

/*****************************************************************************/
//  Description : init when phone power on
//  Global resource dependence : none
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
void MMIAUDIO_InitBgPlay(void)
{
    uint32  i = 0;

    for(i=0; i<MMIBGPLAY_MODULE_MAX; i++)
    {
        s_bgplay_modules[i] = FALSE;
        s_music_modules[i] = FALSE;
    }

    InitStack();

    if (PNULL == s_audio_ctrl_mutex)
    {
        s_audio_ctrl_mutex = SCI_CreateMutex("MMI_AUDIO_CTRL",SCI_INHERIT);
        SCI_ASSERT(PNULL != s_audio_ctrl_mutex); /*assert verified*/
    }
}

/*****************************************************************************/
//  Description : clear bg play settings.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ClearBgPlay(
                  MMIBGPLAY_TYPE_E      bg_type,    //[IN]
                  MMIBGPLAY_MODULE_E    module      //[IN]
                  )
{
    uint32  i = 0;

    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);
    
    if(MMIBGPLAY_MODULE_MAX == module)
    {
        if(MMIBGPLAY_TYPE_ALL == bg_type)
        {
            for(i=0; i<MMIBGPLAY_MODULE_MAX; i++)
            {
                s_bgplay_modules[i] = FALSE;
                s_music_modules[i] = FALSE;
            }
        }
        else if(MMIBGPLAY_TYPE_MUSIC == bg_type)
        {
            for(i=0; i<MMIBGPLAY_MODULE_MAX; i++)
            {
                s_music_modules[i] = FALSE;
            }
        }
    }
    else if(MMIBGPLAY_MODULE_MAX > module)
    {
        if(MMIBGPLAY_TYPE_ALL == bg_type)
        {
            s_bgplay_modules[module] = FALSE;
            s_music_modules[module] = FALSE;
        }
        else if(MMIBGPLAY_TYPE_MUSIC == bg_type)
        {
            s_music_modules[module] = FALSE;
        }
    }

    SCI_PutMutex(s_audio_ctrl_mutex);
}
/*****************************************************************************/
//  Description : PauseBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void PauseBgPlay(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  )
{
    MMIBGPLAY_TYPE_E bg_type = MMIBGPLAY_TYPE_NONE;
    
    if(MMIBGPLAY_MODULE_MAX <= module)
    {
        return;
    }

    bg_type = GetActiveBg();

    //SCI_TRACE_LOW:"[MMIAUDIO]:PauseBgPlay: bg_type = %d, module = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_482_112_2_18_2_3_40_118,(uint8*)"dd",bg_type,module);
    
    s_bgplay_modules[module] = TRUE;
    s_music_modules[module] = TRUE;
    
    switch (bg_type)
    {
    case MMIBGPLAY_TYPE_MP3:
//        MMIAPIMP3_PauseAudioPlayer(FALSE);
        break;

    case MMIBGPLAY_TYPE_MUSIC:
//        MMIAPIMP3_PauseMusicPlayer(FALSE);
        break;
        
    case MMIBGPLAY_TYPE_KURO:
        MMIAPIKUR_PauseKurPlayer(FALSE);
        break;

    case MMIBGPLAY_TYPE_FM:
        MMIAPIFM_Pause(FALSE);
        break;

    case MMIBGPLAY_TYPE_JAVA_BG:
        #ifdef JAVA_SUPPORT
        MMIAPIJAVA_PauseBackgroundJVM();
        #endif
        break;

#ifdef WRE_SUPPORT
    case MMIBGPLAY_TYPE_WRE_BG:
        MMIWRE_PauseMM();
        break;
#endif
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : ResumeBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ResumeBgPlay(
                      MMIBGPLAY_MODULE_E    module          //module exit
                      )
{
    MMIBGPLAY_TYPE_E bg_type = MMIBGPLAY_TYPE_NONE;
    
    if(MMIBGPLAY_MODULE_MAX <= module)
    {
        return;
    }

    bg_type = GetActiveBg();

    //SCI_TRACE_LOW:"[MMIAUDIO]:ResumeBgPlay,bg_type = 0x%x,module = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_538_112_2_18_2_3_40_119,(uint8*)"dd",bg_type,module);
    
    s_bgplay_modules[module] = FALSE;
    s_music_modules[module] = FALSE;
    
    if(!MMIAUDIO_IsBgPlayPaused(bg_type))
    {
        switch (bg_type)
        {
        case MMIBGPLAY_TYPE_MP3:
            //MMIAPIMP3_ResumeAudioPlayer(FALSE);
            break;

        case MMIBGPLAY_TYPE_MUSIC:
            //MMIAPIMP3_ResumeMusicPlayer(FALSE);
            break;
            
        case MMIBGPLAY_TYPE_KURO:
            MMIAPIKUR_ResumeKurPlayer(FALSE);
            break;

        case MMIBGPLAY_TYPE_FM:
            MMIAPIFM_Resume(FALSE);
            break;

        case MMIBGPLAY_TYPE_JAVA_BG:
            #ifdef JAVA_SUPPORT
            MMIAPIJAVA_ResumeBackgroundJVM();
            #endif
            break;

#ifdef WRE_SUPPORT
        case MMIBGPLAY_TYPE_WRE_BG:
            MMIWRE_ResumeMM();
            break;
#endif

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : MMIAUDIO_PauseBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PauseBgPlay(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  )
{
    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);

    PauseBgPlay(module);

    SCI_PutMutex(s_audio_ctrl_mutex);
}

/*****************************************************************************/
//  Description : MMIAUDIO_ResumeBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ResumeBgPlay(
                      MMIBGPLAY_MODULE_E    module          //module exit
                      )
{
    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);

    ResumeBgPlay(module);

    SCI_PutMutex(s_audio_ctrl_mutex);
}

/*****************************************************************************/
//  Description : MMIAUDIO_StopBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopBgPlay(
                 MMIBGPLAY_MODULE_E module  //module entered
                 )
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopBgPlay,module = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_619_112_2_18_2_3_41_120,(uint8*)"d",module);
    if(MMIBGPLAY_MODULE_MAX <= module)
    {
        return;
    }

    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);

    ClearStack();
    
    s_bgplay_modules[module] = FALSE;
    s_music_modules[module] = FALSE;

    MMIAPIMP3_StopAudioPlayer();
    
    MMIAPIMP3_StopMusicPlayer();
    
    MMIAPIKUR_StopKurPlayer();

    MMIAPIFM_Stop();

    #ifdef JAVA_SUPPORT
    MMIAPIJAVA_StopAudioAndJVM();
    #endif
    #ifdef WRE_SUPPORT
    MMIWRE_StopMM();
    #endif

    SCI_PutMutex(s_audio_ctrl_mutex);
}

/*****************************************************************************/
//  Description : MMIAUDIO_PauseBgPlay, except MMIBGPLAY_TYPE_MUSIC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PauseBgPlayRemainMusic(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  )
{
    MMIAUDIO_PauseBgPlay(module);
    s_music_modules[module] = FALSE;
}


/*****************************************************************************/
//  Description : MMIAUDIO_ResumeBgPlay, except MMIBGPLAY_TYPE_MUSIC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ResumeBgPlayRemainMusic(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  )
{
    MMIAUDIO_ResumeBgPlay(module);
    s_music_modules[module] = FALSE;
}

/*****************************************************************************/
//  Description : MMIAUDIO_IsBgPlayIdleDisplay.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsBgPlayIdleDisplay(
                    MMIBGPLAY_TYPE_E      bg_type
                    )
{
    BOOLEAN result = FALSE;
    MMIBGPLAY_TYPE_E local_bg_type = GetActiveBg();
    
    switch(bg_type)
    {
    case MMIBGPLAY_TYPE_MP3:
    case MMIBGPLAY_TYPE_FM:
    case MMIBGPLAY_TYPE_KURO:
        result = (MMIBGPLAY_TYPE_MUSIC   == local_bg_type || 
                  MMIBGPLAY_TYPE_JAVA_BG == local_bg_type || 
#ifdef WRE_SUPPORT
                  MMIBGPLAY_TYPE_WRE_BG == local_bg_type || 
#endif
                  local_bg_type == bg_type);
        break;
        
    case MMIBGPLAY_TYPE_MUSIC:
        break;
        
    case MMIBGPLAY_TYPE_JAVA_BG:
        break;
        
    default:
        break;
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_IsBgPlayIdleDisplay,bg_type = 0x%x, local_bg_type = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_709_112_2_18_2_3_41_121,(uint8*)"dd",bg_type, local_bg_type);
    
    return result;
}

/*****************************************************************************/
//  Description : MMIAUDIO_ Pause Other Bg Play,except pause_type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PauseOtherBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type //in
                  )
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_PauseOtherBgPlay,bg_type = 0x%x, active_type = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_722_112_2_18_2_3_41_122,(uint8*)"dd",bg_type, GetActiveBg());

    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);

#ifdef MMI_VIDEOWALLPAPER_SUPPORT 
    if(MMIAPISETVWP_IsAudioUsing())
    {
        MMIAPIVP_SetBgVideoAudioSupport(FALSE);
    }
#endif

    if(bg_type              != GetActiveBg() &&
       MMIBGPLAY_TYPE_NONE  != GetActiveBg())
    {
        PauseBgPlay(MMIBGPLAY_MODULE_NONE);
    }

    //java有时运行时未处理audio相关接口，需要额外考虑，以及需求
    //java后续需求:前台播放后不恢复其他背景播放，后台播放需要对铃声按次pause bg、resume bg
    
    #ifdef JAVA_SUPPORT
    if(bg_type != MMIBGPLAY_TYPE_JAVA_BG)
    {
        if(MMIAPIJAVA_IsJavaRuning())   //java_export.c 部分工程没有加入
        {
            if(!IsBgPlayInStack(MMIBGPLAY_TYPE_JAVA_BG))
            {
                PushBgItem((uint32)MMIBGPLAY_TYPE_JAVA_BG);
                
                MMIAPIJAVA_PauseBackgroundJVM();
                
            }
        }
    }
    #endif
    
    PushBgItem((uint32)bg_type);

    ResumeBgPlay(MMIBGPLAY_MODULE_NONE);

    SCI_PutMutex(s_audio_ctrl_mutex);
}

/*****************************************************************************/
//  Description : MMIAUDIO_ Resume Other Bg Play,except resume_type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ResumeOtherBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type
                  )
{   
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_ResumeOtherBgPlay,bg_type = 0x%x, active_type = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_773_112_2_18_2_3_41_123,(uint8*)"dd",bg_type, GetActiveBg());

    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);
    
    PopBgItem((uint32)bg_type);

    if(bg_type              != GetActiveBg() &&
       MMIBGPLAY_TYPE_NONE  != GetActiveBg())
    {
        ResumeBgPlay(MMIBGPLAY_MODULE_NONE);
    }
#ifdef MMI_VIDEOWALLPAPER_SUPPORT 
    else
    {
        if(MMIAPISETVWP_IsAudioUsing())
        {
            MMIAPIVP_SetBgVideoAudioSupport(TRUE);
        }
    }
#endif

    SCI_PutMutex(s_audio_ctrl_mutex);
}

/*****************************************************************************/
//  Description : MMIAUDIO_ Stop Other Bg Play,except stop_type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: not used
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopOtherBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type //in
                  )
{
/*
    uint32      i = 0;
    
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopOtherBgPlay,bg_type = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_807_112_2_18_2_3_41_124,(uint8*)"d",bg_type);

    MMIAUDIO_StopBgPlay(MMIBGPLAY_MODULE_NONE);
    
    for (i=0; i<MMIBGPLAY_TYPE_ALL; i++)
    {
        if(i != bg_type)
        {
            PopBgItem((uint32)bg_type);
        }
    }
*/
}

/*****************************************************************************/
//  Description : MMIAUDIO Is Active BgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsActiveBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type
                  )
{
    return (IsActiveBg(bg_type));
}

/*****************************************************************************/
//  Description : MMIAUDIO Is exist BgPlay
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsExistBgPlay(void)
{
    MMIBGPLAY_TYPE_E  bg_type = GetActiveBg();
    
    if(MMIBGPLAY_TYPE_NONE == bg_type)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : MMIAUDIO_ Is Bg Play Paused by module
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsBgPlayPausedByModule(
                                               MMIBGPLAY_TYPE_E  bg_type
                                               )
{
    BOOLEAN     is_paused = FALSE;
    uint32      i = 0;

    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);
    
    if(MMIBGPLAY_TYPE_MUSIC == bg_type)
    {
        for (i=1; i<MMIBGPLAY_MODULE_MAX; i++)
        {
            if (s_music_modules[i])
            {
                is_paused = TRUE;
                //SCI_TRACE_LOW:"MMIAUDIO_IsBgPlayPausedByModule: bg = %d has suspend by module id = 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_870_112_2_18_2_3_41_125,(uint8*)"dd",bg_type,i);
                break;
            }
        }
    }
    else
    {
        for (i=1; i<MMIBGPLAY_MODULE_MAX; i++)
        {
            if (s_bgplay_modules[i])
            {
                is_paused = TRUE;
                //SCI_TRACE_LOW:"MMIAUDIO_IsBgPlayPausedByModule: bg = %d has suspend by module id = 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_882_112_2_18_2_3_41_126,(uint8*)"dd",bg_type,i);
                break;
            }
        }
    }
    
    SCI_PutMutex(s_audio_ctrl_mutex);
    
    return (is_paused);
}

/*****************************************************************************/
//  Description : MMIAUDIO_ Is Bg Play Paused
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsBgPlayPaused(
                  MMIBGPLAY_TYPE_E  bg_type
                  )
{
    BOOLEAN     is_paused = FALSE;
    uint32      i = 0;
    MMIBGPLAY_TYPE_E local_bg_type = MMIBGPLAY_TYPE_NONE;

    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);

    local_bg_type = GetActiveBg();
    
    if(local_bg_type != bg_type &&
       local_bg_type != MMIBGPLAY_TYPE_NONE)
    {
        //SCI_TRACE_LOW:"MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by local_bg_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_912_112_2_18_2_3_41_127,(uint8*)"dd",bg_type,local_bg_type);
        is_paused = TRUE;
    }
    else
    {
        if(MMIBGPLAY_TYPE_MUSIC == bg_type)
        {
            for (i=1; i<MMIBGPLAY_MODULE_MAX; i++)
            {
                if (s_music_modules[i])
                {
                    is_paused = TRUE;
                    //SCI_TRACE_LOW:"MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by module id = 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_924_112_2_18_2_3_41_128,(uint8*)"dd",bg_type,i);
                    break;
                }
            }
        }
        else
        {
            for (i=1; i<MMIBGPLAY_MODULE_MAX; i++)
            {
                if (s_bgplay_modules[i])
                {
                    is_paused = TRUE;
                    //SCI_TRACE_LOW:"MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by module id = 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_936_112_2_18_2_3_41_129,(uint8*)"dd",bg_type,i);
                    break;
                }
            }
        }
    }
    
    SCI_PutMutex(s_audio_ctrl_mutex);
    
    return (is_paused);
}

/*****************************************************************************/
//  Description : judge whether can play new message alert ring
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsPermitPlayNewMsgRing(void)
{
#ifdef MMI_RECORD_SUPPORT
    if( MMIAPIRECORD_IsOpened()
        //|| MMIAPIKUR_IsPlayerFocus()
        || MMIAPIDC_IsOpened()//因为isp不能和audio共用，会有冲突。所以在dc的整个过程都不能开audio。
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
#ifdef VIDEO_PLAYER_SUPPORT        
        //|| MMIAPIVP_IsOpenPlayerWin()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        //|| MMIAPIMV_IsOpenMobileVideo()
#endif
        || MMIAPIUDISK_IsInPCCamera()
#if defined(MMI_GPS_SUPPORT) 
        //|| MMIAPIGPS_IsGpsOpened( )     // GPS
#endif
#ifdef TTS_SUPPORT
        //|| MMIAPITTS_GetPlayStatus()
#endif
#ifdef ASP_SUPPORT
	    || MMIAPIASP_lsPlayingMusic()
#endif
#ifdef FM_SUPPORT
        ||MMIAPIFM_IsOpened()
#endif
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
//#ifdef MMI_BLUETOOTH
        //|| MMIAPIBT_IsOpenFileWin()  //蓝牙在接收或传送文件时，会使信息铃声断续
//#endif
        //|| MMIAPIMMS_IsAudioInUse()
        //|| MMIAPIMMS_IsVideoInUse()
        //|| MMIAPIENG_IsFocusInLoopBack()
        //|| MMIAPIMTV_IsMTVOpen()
        //|| MMIAPIATV_IsATVOpen()
        //|| MMIAUDIO_IsInVoiceLoop()
        )
    {   
        return FALSE;
    }
    else
#endif
    {   
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : Get the result whether mp3 bg play is paused by any application.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAudioPlayerBgPaused(
                          void
                          )
{
    return (MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_MP3));
}

/*****************************************************************************/
//  Description : Get the result whether kuro bg play is paused by any application.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsKuroPlayerBgPaused(
                          void
                          )
{
    return (MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_KURO));
}

/*****************************************************************************/
//  Description : MMIAUDIO _Get Audio Buf Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetAudioBufInfo(
                          const void*       audio_data,         //[IN]
                          uint32            data_len,           //[IN]
                          MMIAUDIO_TYPE_E   audio_media_type,   //[IN]
                          MMIAUDIO_INFO_T * audio_info          //[OUT]
                          )
{
    BOOLEAN result = FALSE;
    AUDIO_FORMATINFO_T format_info = {0};
    MMISET_ALL_RING_TYPE_E ring_type = MMISET_RING_TYPE_OTHER;

    MMIAUDIO_PauseBgPlay( MMIBGPLAY_MODULE_GET_MEDIA_INFO);
    
    if (MMIAPISET_IsPermitPlayRing(ring_type))
    {
        //MMIAPISET_StopAllVibrator(); 
        MMIAPISET_StopAllRing(TRUE);
        
        MMIAPISET_SetCurRingType(ring_type);

        if(MMIAUDIO_CreateAudioBufHandle(FALSE, MMIAPISET_GetRingHtype(ring_type), audio_media_type, (const uint8 *)audio_data, data_len))
        {
            if(MMIAUDIO_AudioGetFileInfo(MMIAPISET_GetRingHtype(ring_type), &format_info))
            {
                audio_info->time_len = format_info.uiTotalTime;
                audio_info->data_len = format_info.uiTotalDataLength;
                result = TRUE;
            }
            MMIAUDIO_CloseAudioHandle(MMIAPISET_GetRingHtype(ring_type));
        }
    }
    
    MMIAUDIO_ResumeBgPlay( MMIBGPLAY_MODULE_GET_MEDIA_INFO);
    
    return (result);
}

/*****************************************************************************/
//  Description : Init key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKeyCfg(void)
{
    uint32  i = 0;
    uint32  j = 0;

    for (j=0; j<MMIAUDIO_KEY_ALL; j++)
    {
        for(i=0; i<MMIBGPLAY_MODULE_MAX; i++)
        {
            s_key_ring_cfg[j][i] = FALSE;
        }
    }
}

/*****************************************************************************/
//  Description : Init key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void StopKeyRing(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                )
{
    if(!is_enable && module_type < MMIBGPLAY_MODULE_MAX)
    {
        switch(ring_type)
        {
        case MMIAUDIO_KEY_RING:
            if(MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_KEY))
            {
                SCI_Sleep(100);
            }
            MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);
            break;
        
        case MMIAUDIO_TP_RING:
            if(MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_TKEY))
            {
                SCI_Sleep(100);
            }
            MMIAPISET_StopRing(MMISET_RING_TYPE_TKEY);
            break;

        case MMIAUDIO_KEY_ALL:
            if(MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_KEY) ||
               MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_TKEY))
            {
                SCI_Sleep(100);
            }
            MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);
            MMIAPISET_StopRing(MMISET_RING_TYPE_TKEY);
            break;

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : update key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void UpdateKeyCfg(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                )
{
    if(module_type < MMIBGPLAY_MODULE_MAX)
    {
        switch(ring_type)
        {
        case MMIAUDIO_KEY_RING:
        case MMIAUDIO_TP_RING:
            s_key_ring_cfg[ring_type][module_type] = !is_enable;
            break;

        case MMIAUDIO_KEY_ALL:
            s_key_ring_cfg[MMIAUDIO_KEY_RING][module_type] = !is_enable;
            s_key_ring_cfg[MMIAUDIO_TP_RING][module_type]  = !is_enable;
            break;

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : update key config
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void UpdateKeyStatus(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                )
{
    uint32  i = 0;
    uint32  j = 0;
    BOOLEAN is_key_enable = TRUE;
    BOOLEAN is_tp_enable = TRUE;
    
    if(module_type < MMIBGPLAY_MODULE_MAX)
    {
        switch(ring_type)
        {
        case MMIAUDIO_KEY_RING:
            for(i=0; i<MMIBGPLAY_MODULE_MAX; i++)
            {
                if(s_key_ring_cfg[ring_type][i])
                {
                    is_key_enable = FALSE;
                    break;
                }
            }
            s_is_enable_keyring = is_key_enable;
            break;
            
        case MMIAUDIO_TP_RING:
            for(i=0; i<MMIBGPLAY_MODULE_MAX; i++)
            {
                if(s_key_ring_cfg[ring_type][i])
                {
                    is_tp_enable = FALSE;
                    break;
                }
            }
            s_is_enable_tp_ring = is_tp_enable;
            break;

        case MMIAUDIO_KEY_ALL:
            for (j=0; j<MMIAUDIO_KEY_ALL; j++)
            {
                for(i=0; i<MMIBGPLAY_MODULE_MAX; i++)
                {
                    if(s_key_ring_cfg[j][i])
                    {
                        if(MMIAUDIO_KEY_RING == j)
                        {
                            is_key_enable = FALSE;
                        }
                        else if(MMIAUDIO_TP_RING == j)
                        {
                            is_tp_enable = FALSE;
                        }
                        break;
                    }
                }
            }
            s_is_enable_keyring = is_key_enable;
            s_is_enable_tp_ring = is_tp_enable;
            break;

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : Enable key ring
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableKeyRing(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                )
{
    BOOLEAN is_key_enable = s_is_enable_keyring;
    BOOLEAN is_tp_enable  = s_is_enable_tp_ring;
    
    StopKeyRing(ring_type,module_type,is_enable);
    UpdateKeyCfg(ring_type,module_type,is_enable);
    UpdateKeyStatus(ring_type,module_type,is_enable);

    //SCI_TRACE_LOW:"[MMIAUDIO] MMIAUDIO_EnableKeyRing: ring_type = 0x%x, module = 0x%x, is_enable = 0x%x, key = 0x%x 0x%x, tp = 0x%x 0x%x!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_1251_112_2_18_2_3_42_130,(uint8*)"ddddddd",ring_type,module_type,is_enable,is_key_enable,s_is_enable_keyring,is_tp_enable,s_is_enable_tp_ring);
}

/*****************************************************************************/
//  Description : Enable key ring, without stop ring, just handle key ring flsgs.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableKeyRingEx(
                MMIAUDIO_KEY_TYPE_E ring_type,
                MMIBGPLAY_MODULE_E  module_type,
                BOOLEAN             is_enable
                )
{
    BOOLEAN is_key_enable = s_is_enable_keyring;
    BOOLEAN is_tp_enable  = s_is_enable_tp_ring;
    
    //StopKeyRing(ring_type,module_type,is_enable);
    UpdateKeyCfg(ring_type,module_type,is_enable);
    UpdateKeyStatus(ring_type,module_type,is_enable);

    //SCI_TRACE_LOW:"[MMIAUDIO] MMIAUDIO_EnableKeyRingEx: ring_type = 0x%x, module = 0x%x, is_enable = 0x%x, key = 0x%x 0x%x, tp = 0x%x 0x%x!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_1271_112_2_18_2_3_42_131,(uint8*)"ddddddd",ring_type,module_type,is_enable,is_key_enable,s_is_enable_keyring,is_tp_enable,s_is_enable_tp_ring);
}

/*****************************************************************************/
//  Description : Set last key \tp key play end notify.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetLastKeyTpkeyNotify(
                MMI_WIN_ID_T        win_id,
                MMI_MESSAGE_ID_E    msg_id
                )
{
    s_last_key_notify.msg_id = msg_id;  //此 MSG  最好统一使用 MSG_AUDIO_LAST_KEY_END_NOTIFY
    s_last_key_notify.win_id = win_id;

    //SCI_TRACE_LOW:"[MMIAUDIO] MMIAUDIO_LastKeyTpkeyNotify: win_id = 0x%x, msg_id = 0x%x!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_1286_112_2_18_2_3_42_132,(uint8*)"dd",win_id, msg_id);
    
    if(MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_KEY))
    {
        if(MMIAUDIO_IsAudioPlayEnd(MMIAUDIO_GetAudioHandle(MMIAPISET_GetRingHtype(MMISET_RING_TYPE_KEY))))
        {
            MMIAUDIO_SendLastKeyTpkeyNotify();
        }
    }
    else if(MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_TKEY))
    {
        if(MMIAUDIO_IsAudioPlayEnd(MMIAUDIO_GetAudioHandle(MMIAPISET_GetRingHtype(MMISET_RING_TYPE_TKEY))))
        {
            MMIAUDIO_SendLastKeyTpkeyNotify();
        }
    }
    else
    {
        MMIAUDIO_SendLastKeyTpkeyNotify();
    }
}

/*****************************************************************************/
//  Description : Send last key \tp key play end notify.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SendLastKeyTpkeyNotify(
                    void
                    )
{
    if(s_last_key_notify.win_id != NULL &&
       s_last_key_notify.msg_id != NULL)
    {
        //SCI_TRACE_LOW:"[MMIAUDIO] MMIAUDIO_SendLastKeyTpkeyNotify: POST MSG win_id = 0x%x, msg_id = 0x%x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_1319_112_2_18_2_3_42_133,(uint8*)"dd",s_last_key_notify.win_id, s_last_key_notify.msg_id);
        MMK_PostMsg(s_last_key_notify.win_id, s_last_key_notify.msg_id, PNULL, NULL);
        s_last_key_notify.msg_id = NULL;
        s_last_key_notify.win_id = NULL;
    }
}

/*****************************************************************************/
//  Description : is tp ring enable.        
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsEnableTpRing( void )
{
    return s_is_enable_tp_ring;
}

/*****************************************************************************/
//  Description : is tp ring enable.        
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsEnableKeyRing( void )
{
    return s_is_enable_keyring;
}

/*****************************************************************************/
//  Description : MMIAUDIO SwitchDevice
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SwitchDevice(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    MMIAUDIO_DEVICE_TYPE_E  device
                    )
{
    if(MMIAUDIO_SwitchDeviceExt(MMIAPISET_GetRingHtype(ring_type), device))
    {
        switch(device)
        {
            case MMIAUDIO_DEVICE_BT:
                if(MMIAPISET_IsPlayingRingType(ring_type))
                {
                    MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_EARPHONE);
                }
                break;

            case MMIAUDIO_DEVICE_OTHER:
                if(MMIAPISET_IsPlayingRingType(ring_type))
                {
                    AUDIO_DEVICE_MODE_TYPE_E  dev_mode = AUDIO_DEVICE_MODE_HANDFREE;
                    MMIAPISET_GetDeviceMode(&dev_mode);
                    MMIAUDIO_SetCurDevMode(dev_mode);
                }
                break;

            default:
                break;
        }
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Get the app status.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAppBusy(void)
{
    BOOLEAN result = FALSE;

    //if(MMIAPISMS_IsSMSBusy())
    {
        result = TRUE;
    }
   
    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_HandleNewMsgRing(void)
{
    //pause background play
    MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_NEW_MESSAGE);

    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_HandleNewMsgKey(void)
{
/* marked by Yintang to avoid assert */
#if 0
    BOOLEAN     result = TRUE;

    SCI_GetMutex(s_audio_ctrl_mutex,SCI_WAIT_FOREVER);
    
    MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);

    if (s_bgplay_modules[MMIBGPLAY_MODULE_NEW_MESSAGE])  //此处本不需要判断，只是避免输出log太多
    {
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
        MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_NEW_MESSAGE);
    }

    SCI_PutMutex(s_audio_ctrl_mutex);
    
    return (result);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_HandleRingCallBack(MMIAUDIO_RESULT result)
{
    //将之前在phone.c中的MSG_MP3_CONTINUE消息处理代码移到此函数，作为同步调用。
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_HandleRingCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_CTRL_1443_112_2_18_2_3_42_134,(uint8*)"");
    if (MMIAUDIO_RLT_STOP != result 
        && !MMIAPTSET_ExitedWinForbidResumingMp3() 
         )
    {
        //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_NEWSMS);
        //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_NEWMMS);
        MMIAUDIO_HandleNewMsgKey();
    }
}

#endif
