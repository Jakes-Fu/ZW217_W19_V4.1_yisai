#if 0
/******************************************************************************
 ** File Name:      mmiaudio_ctrl.h                                           *
 ** Author:         haiyang.hu                                                *
 ** DATE:           09/25/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file define all audio ctrl                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/25/2006                      Create                                    *
 ******************************************************************************/

#ifndef _MMIAUDIO_CTRL_H
#define _MMIAUDIO_CTRL_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmiaudio.h"
#include "mmiset.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//modules that can interrupt background play(mp3 and fm)
typedef enum
{
    MMIBGPLAY_MODULE_NONE                   = 0x00,
    MMIBGPLAY_MODULE_CC                     = 0x01,
    MMIBGPLAY_MODULE_ALARM                  = 0x02,
    MMIBGPLAY_MODULE_NEWSMS                 = 0x03,
    MMIBGPLAY_MODULE_NEWMMS                 = 0x04,
    MMIBGPLAY_MODULE_MMS                    = 0x05,
    MMIBGPLAY_MODULE_MMS_INBOX              = 0x06,
    MMIBGPLAY_MODULE_MMS_DOWNLOAD           = 0x07,
    MMIBGPLAY_MODULE_EDITMMS                = 0x08,
    MMIBGPLAY_MODULE_WAP                    = 0x09,
    MMIBGPLAY_MODULE_WAP_MAINMENU           = 0x0a,
    MMIBGPLAY_MODULE_DC                     = 0x0b,
    MMIBGPLAY_MODULE_DV                     = 0x0c,
    MMIBGPLAY_MODULE_MPEG4                  = 0x0d,
    MMIBGPLAY_MODULE_MYDOCUMENT             = 0x0e,
    MMIBGPLAY_MODULE_RECORDER               = 0x0f,
    MMIBGPLAY_MODULE_UDISK                  = 0x10,
    MMIBGPLAY_MODULE_ENVSET                 = 0x11,
    MMIBGPLAY_MODULE_SCREENSAVER_SET        = 0x12,
    MMIBGPLAY_MODULE_POWER_ON_ANIM_SET      = 0x13,
    MMIBGPLAY_MODULE_POWER_OFF_ANIM_SET     = 0x14,
    MMIBGPLAY_MODULE_ALARMSET               = 0x15,
    MMIBGPLAY_MODULE_SCHEDULESET            = 0x16,
    MMIBGPLAY_MODULE_PHONEBOOK              = 0x17,
    MMIBGPLAY_MODULE_LOWPOWER               = 0x18,
    MMIBGPLAY_MODULE_SD                     = 0x19,
    MMIBGPLAY_MODULE_SELFROM_MYDOC          = 0x1a,
    MMIBGPLAY_MODULE_RINGSEL_FROMSET        = 0x1b,
    MMIBGPLAY_MODULE_BLUETOOTH              = 0x1c,
    MMIBGPLAY_MODULE_NEW_MESSAGE            = 0x1d, //including sms, mms, wap push.
    MMIBGPLAY_MODULE_JAVA                   = 0x1e,
    MMIBGPLAY_MODULE_POWERONOFF             = 0x1f,
    MMIBGPLAY_MODULE_PBVIEW                 = 0x20,
    MMIBGPLAY_MODULE_EARPHONE               = 0x21,
    MMIBGPLAY_MODULE_FM                     = 0x22,
    MMIBGPLAY_MODULE_MP3                    = 0x23,
    MMIBGPLAY_MODULE_FMM                    = 0x24,
    MMIBGPLAY_MODULE_FMM_COPY               = 0x25,
    MMIBGPLAY_MODULE_ST                     = 0x26,
    MMIBGPLAY_MODULE_EBOOK                  = 0x27,
    MMIBGPLAY_MODULE_KURO                   = 0x28,
    MMIBGPLAY_MODULE_COUNTEDTIME            = 0x29,
    MMIBGPLAY_MODULE_MTV                    = 0x2a,
    MMIBGPLAY_MODULE_WWW                    = 0x2b,
    MMIBGPLAY_MODULE_RESET_FACT             = 0x2c,
    MMIBGPLAY_MODULE_ENG_TEST               = 0x2d,
    MMIBGPLAY_MODULE_GPS                    = 0x2e,
    MMIBGPLAY_MODULE_TTS                    = 0x2f,
    MMIBGPLAY_MODULE_ATV                    = 0x30,
    MMIBGPLAY_MODULE_GET_MEDIA_INFO         = 0x31,
    MMIBGPLAY_MODULE_ASP                    = 0x32,
    MMIBGPLAY_MODULE_DM                     = 0x33,
    MMIBGPLAY_MODULE_IM                     = 0x34, //input method
    MMIBGPLAY_MODULE_SETTHEME               = 0x35,    
    MMIBGPLAY_MODULE_MV                     = 0x36,
    MMIBGPLAY_MODULE_PIM                    = 0x37,
    MMIBGPLAY_MODULE_AZAN                   = 0x38,
#ifdef WRE_SUPPORT
    MMIBGPLAY_MODULE_WRE                    = 0x39,
#endif
    MMIBGPLAY_MODULE_VIDEO_WALLPAPER        = 0x3a,
    MMIBGPLAY_MODULE_MAX                    = 0x40
}MMIBGPLAY_MODULE_E;

//background play type(mp3 and fm)
typedef enum
{
    MMIBGPLAY_TYPE_NONE,    //none
    MMIBGPLAY_TYPE_MP3,     //mp3
    MMIBGPLAY_TYPE_FM,      //fm
    MMIBGPLAY_TYPE_KURO,    //KURO
    MMIBGPLAY_TYPE_MUSIC,   //KURO
    MMIBGPLAY_TYPE_JAVA_BG, //Java BG
    MMIBGPLAY_TYPE_BT,      //bt
#ifdef WRE_SUPPORT
    MMIBGPLAY_TYPE_WRE_BG, //WRE BG
#endif
    MMIBGPLAY_TYPE_ALL,     //all
} MMIBGPLAY_TYPE_E;

typedef enum
{
    MMIAUDIO_KEY_RING,  //key
    MMIAUDIO_TP_RING,   //tp
    MMIAUDIO_KEY_ALL    //all
} MMIAUDIO_KEY_TYPE_E;

typedef void (* MMIAUDIO_VIBRATE_CALLBACK)(void);  //振动自动结束后的回调函数

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : clear bg play settings.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ClearBgPlay(
                  MMIBGPLAY_TYPE_E      bg_type,    //[IN]
                  MMIBGPLAY_MODULE_E    module      //[IN]
                  );

/*****************************************************************************/
//  Description : MMIAUDIO_PauseBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PauseBgPlay(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  );
                  
/*****************************************************************************/
//  Description : MMIAUDIO_ResumeBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ResumeBgPlay(
                      MMIBGPLAY_MODULE_E    module          //module exit
                      );
                      
/*****************************************************************************/
//  Description : MMIAUDIO_StopBgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopBgPlay(
                 MMIBGPLAY_MODULE_E module  //module entered
                 );


/*****************************************************************************/
//  Description : MMIAUDIO_PauseBgPlay, except MMIBGPLAY_TYPE_MUSIC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PauseBgPlayRemainMusic(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  );

/*****************************************************************************/
//  Description : MMIAUDIO_ResumeBgPlay, except MMIBGPLAY_TYPE_MUSIC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ResumeBgPlayRemainMusic(
                  MMIBGPLAY_MODULE_E    module      //module entered
                  );
                  
/*****************************************************************************/
//  Description : MMIAUDIO_IsBgPlayIdleDisplay.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsBgPlayIdleDisplay(
                    MMIBGPLAY_TYPE_E      bg_type
                    );

/*****************************************************************************/
//  Description : MMIAUDIO_ Pause Other Bg Play,except pause_type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PauseOtherBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type //in
                  );

/*****************************************************************************/
//  Description : MMIAUDIO_ Resume Other Bg Play,except resume_type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_ResumeOtherBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type
                  );

/*****************************************************************************/
//  Description : MMIAUDIO_ Stop Other Bg Play,except stop_type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: not used
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopOtherBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type //in
                  );

/*****************************************************************************/
//  Description : MMIAUDIO_ Is Bg Play Paused by module
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsBgPlayPausedByModule(
                                               MMIBGPLAY_TYPE_E  bg_type
                                               );

/*****************************************************************************/
//  Description : MMIAUDIO_ Is Bg Play Paused
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsBgPlayPaused(
                  MMIBGPLAY_TYPE_E  bg_type
                  );

/*****************************************************************************/
//  Description : MMIAUDIO Is Active BgPlay
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsActiveBgPlay(
                  MMIBGPLAY_TYPE_E  bg_type
                  );

/*****************************************************************************/
//  Description : MMIAUDIO Is exist BgPlay
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsExistBgPlay(void);

/*****************************************************************************/
//  Description : init when phone power on
//  Global resource dependence : none
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
void MMIAUDIO_InitBgPlay(void);

/*****************************************************************************/
//  Description : judge whether can play new message alert ring
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsPermitPlayNewMsgRing(void);
/*****************************************************************************/
//  Description : start vibrator
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_Vibrate(uint32 duaration, BOOLEAN is_period, MMIAUDIO_VIBRATE_CALLBACK callback);

/*****************************************************************************/
//  Description : handle vibrtor timer over message
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_HandleVibratorTimer( uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_HandleNewMsgRing(void);
/*****************************************************************************/
//  Description : stop vibrate
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StopVibrate(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_HandleNewMsgKey(void);

/*****************************************************************************/
//  Description : Get the result whether mp3 bg play is paused by any application.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAudioPlayerBgPaused(
                    void
                    );

/*****************************************************************************/
//  Description : Get the result whether kuro bg play is paused by any application.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsKuroPlayerBgPaused(
                          void
                          );

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
                );
                
/*****************************************************************************/
//  Description : is tp ring enable.        
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsEnableTpRing( void );

/*****************************************************************************/
//  Description : is tp ring enable.        
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsEnableKeyRing( void );

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
                          );

/*****************************************************************************/
//  Description : MMIAUDIO SwitchDevice
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SwitchDevice(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    MMIAUDIO_DEVICE_TYPE_E  device
                    );

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
                );

/*****************************************************************************/
//  Description : Set last key \tp key play end notify.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_LastKeyTpkeyNotify(
                MMI_WIN_ID_T        win_id,
                MMI_MESSAGE_ID_E    msg_id
                );

/*****************************************************************************/
//  Description : Send last key \tp key play end notify.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SendLastKeyTpkeyNotify(
                    void
                    );
                    
/*****************************************************************************/
//  Description : Get the app status.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAppBusy(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_HandleRingCallBack(MMIAUDIO_RESULT result);
                    
#ifdef __cplusplus
    }
#endif

#endif 



#endif
