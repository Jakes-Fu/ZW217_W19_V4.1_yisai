/****************************************************************************
** File Name:      mmicc_audio.h                                            *
** Author:                                                                  *
** Date:           24/11/2008                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message                *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2008        samboo.shen      Create
** 
****************************************************************************/
#ifndef  _MMICC_AUDIO_H_
#define  _MMICC_AUDIO_H_

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 获得当前的声音模式
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC MMICC_AUDIO_DEV_E MMICC_GetCurrentDev ( void );

/*****************************************************************************/
//  Description : construct 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ConstructAudioHandle (void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_DestroyAudioHandle (void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAudioHandleEnable (void);

/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ConstructVoiceCodec (BOOLEAN is_vt_codec);

/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_DestroyVoiceCodec (void);

/*****************************************************************************/
//  Description : 打开/关闭voice codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_EnableVoiceCodec ( BOOLEAN is_enable );

/*****************************************************************************/
//  Description : voice codec是否已经打开
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsVoiceCodecEnable ( void );


/*****************************************************************************/
//  Description : 打开/关闭 本端静音
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_EnableMute ( BOOLEAN is_mute );

/*****************************************************************************/
//  Description : 判断本端静音是否打开
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN    MMICC_IsMute ( void );

/*****************************************************************************/
//  Description : 打开/关闭 远端静音
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_EnableRemoteMute ( BOOLEAN is_enable );

/*****************************************************************************/
//  Description : 判断远端静音是否打开
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN    MMICC_IsRemoteMute ( void );

/*****************************************************************************/
//  Description : enable /disable remote mute from net, but the local tone still to be played
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_EnableNetRemoteMute ( BOOLEAN is_enable );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleVibrateTimer(uint8 timer_id, uint32 param);

/*******************************************************************************/
//  Description : 通话前停止所有的铃声和震动
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 从 MMICC_StopRingOrVibrate改造而来，所不同的是对TTS的处理采用异步调用的方式
/********************************************************************************/
PUBLIC void MMICC_StopRingOrVibrateBeforeCCing(void);

/*****************************************************************************/
//  Description :set mt_preset_codec_route
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in incomming call state
/*****************************************************************************/
PUBLIC void MMICC_SetMtPresetCodecRoute ( MMISRVAUD_ROUTE_T  route );

/*****************************************************************************/
//  Description :get mt_preset_codec_route
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in incomming call state
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T MMICC_GetMtPresetCodecRoute ( void );

#endif


